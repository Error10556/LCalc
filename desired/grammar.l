%option warn nodefault
%option 8bit reentrant
%option noyywrap
%option prefix="LC"
%option outfile="grammar.lex.cpp"

%{
/* Compile grammar.l with:
 * $ flex grammar.l
 * Compile LCLex.cpp with:
 * $ c++ -std=c++17 LCLex.cpp -c -o LCLex.o
 */
#include <string>
#include <system_error>
#include <string_view>
#include "grammar.tab.hpp"

#define YY_DECL LC::Parser::symbol_type LClex(yyscan_t yyscanner)

%}

%option extra-type="std::string*"

%s COMMENT

LETTER [a-zA-Z]
CAPITAL [A-Z]
SMALL [a-z]
DIGIT [0-9]
IDENT [a-zA-Z0-9'_]

%%

<INITIAL>"/*" BEGIN(COMMENT);
<COMMENT>[^*]+ ;
<COMMENT>"*" ;
<COMMENT>"*/" BEGIN(INITIAL);
<INITIAL>"//".* ;

<INITIAL>"l" return LC::Parser::make_KW_l();
<INITIAL>{LETTER}{IDENT}* return LC::Parser::make_IDENT(yytext);
<INITIAL>"(" return LC::Parser::make_LPAREN();
<INITIAL>")" return LC::Parser::make_RPAREN();
<INITIAL>"." return LC::Parser::make_DOT();
<INITIAL>";" return LC::Parser::make_SEMI();
<INITIAL><<EOF>> return LC::Parser::make_YYEOF();
<INITIAL>[[:space:]] ;

. return LC::Parser::make_YYerror();

%%

namespace LC {

class LCScanner {
    yyscan_t scanner;
    LCScanner();

public:
    LCScanner(FILE* file);
    LCScanner(std::string_view str);
    yyscan_t FlexScanner() const;
    ~LCScanner();
};

LCScanner::LCScanner() {
    int err = LClex_init_extra(new std::string(), &scanner);
    if (err) throw std::system_error(err, std::generic_category(),
        "Cannot create LC scanner");
}

LCScanner::LCScanner(FILE* file) : LCScanner() {
    LCrestart(file, scanner);
}

LCScanner::LCScanner(std::string_view str) : LCScanner() {
    LC_scan_bytes(str.data(), str.size(), scanner);
}

yyscan_t LCScanner::FlexScanner() const { return scanner; }

LCScanner::~LCScanner() {
    delete LCget_extra(scanner);
    LClex_destroy(scanner);
}

}
