%option warn nodefault
%option 8bit reentrant bison-bridge
%option noyywrap
%option prefix="LC"
%option outfile="LCLex.cpp"
%option 

%{
/* Compile grammar.l with:
 * $ flex grammar.l
 * Compile LC-lex.c with:
 * $ c++ -std=c++17 LC-lex.cpp -c -o LC-lex.o
 */
#include <variant>
#include <string>
#include <string_view>
#include <system_error>
#include "grammar.tab.hpp"

// to parse values
using LexExtra = std::variant<std::string, double, int>;

#define LC_DECL Parser::symbol_type LClex();

%}

%option extra-type="LexExtra"

%s COMMENT

LETTER [a-zA-Z]
CAPITAL [A-Z]
SMALL [a-z]
DIGIT [0-9]
IDENT [a-zA-Z0-9'_]

%%

<INITIAL>/\* BEGIN(COMMENT);
<COMMENT>[^*]+ ;
<COMMENT>\*/ BEGIN(INITIAL);
<INITIAL>//.* ;

<INITIAL>{LETTER}{IDENT}* return LC::Parser::make_IDENT(LCtext);
<INITIAL>"l" return LC::Parser::make_KW_l();
<INITIAL>"(" return LC::Parser::make_LPAREN();
<INITIAL>")" return LC::Parser::make_RPAREN();
<INITIAL>"." return LC::Parser::make_DOT();
<INITIAL>";" return LC::Parser::make_SEMI();
<INITIAL><<EOF>> return LC::Parser::make_EOF();
<INITIAL>[[:space:]] ;

. return LC::Parser::make_

%%

static LCscan_t LCNewScanner() {
    LCscan_t scanner;
    int err = LClex_init_extra(LexExtra(), &scanner);
    if (err) throw std::system_error(err, std::generic_category(),
        "Cannot create LC scanner");
    return scanner;
}

std::pair<LCscan_t LCNewScanner(FILE* file) {
    LCscan_t scanner = LCNewScanner();
    LCrestart(file, scanner);
    return scanner;
}

LCscan_t LCNewScanner(std::string_view str) {
    LCscan_t scanner = LCNewScanner();
    LC_delete_buffer(LC_CURRENT_BUFFER);
    LC_scan_bytes(str.data(), str.size());
    return scanner;
}
