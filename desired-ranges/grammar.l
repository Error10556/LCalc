%option warn nodefault
%option 8bit reentrant
%option noyywrap noinput nounput
%option prefix="LC"
%option outfile="grammar.lex.cpp"

%{
#undef yylex
#include <charconv>
#include <string>
#include <string_view>
#include <system_error>
#include "grammar.tab.hpp"

namespace LC {

struct Extra {
    std::string parsed;
    position start;
    position end;

    Extra(std::string* optFilename)
        : start(optFilename, 1, 1)
        , end(optFilename, 1, 1) {}
    inline location TakeLoc() {
        location res = {start, end};
        start = end;
        return res;
    }
};

Parser::symbol_type yylex(yyscan_t yyscanner, Parser* thisparser);

}  // namespace LC

#define YY_DECL LC::Parser::symbol_type LC::yylex(yyscan_t yyscanner, LC::Parser* thisparser)
#define YY_USER_ACTION                   \
    {                                    \
        for (int i = 0; i < yyleng; i++) \
            if (yytext[i] == '\n')       \
                yyextra->end.lines();    \
            else                         \
                yyextra->end.columns();  \
    }
#define yyterminate() return LC::Parser::make_YYEOF(yyextra->TakeLoc())

#define LEXER_ERROR(loc) LexerError(thisparser, loc)
static inline LC::Parser::symbol_type LexerError(
        LC::Parser* thisparser, const LC::location& loc) {
    thisparser->error(loc, "Lexical error");
    return LC::Parser::make_YYerror(loc);
}

static inline int hexDigitValue(char ch) {
    if ('0' <= ch && ch <= '9') return ch - '0';
    if ('a' <= ch && ch <= 'f') return ch - 'a' + 10;
    if ('A' <= ch && ch <= 'F') return ch - 'A' + 10;
    return 0;
}

static inline int32_t hexInt32(const char* start, int len) {
    int32_t val = 0;
    for (int i = 0; i < len; i++) val = (val << 4) | hexDigitValue(start[i]);
    return val;
}

static inline int32_t minCharForEncodedLen(int len) {
    if (len < 2) return 0;
    if (len == 2) return 0x80;
    return static_cast<int32_t>(1) << (11 + 5 * (len - 3));
}

    /* returns -1 if a character uses a non-minimal # of bytes */
static inline int decodeUTF8(const char* start, int len) {
    if (len == 1) return *start;
    int firstbits = 7 - len;
    int32_t res = *start & ((1 << firstbits) - 1);
    for (int i = 1; i < len; i++) res = (res << 6) | (start[i] & 0x3f);
    return res >= minCharForEncodedLen(len) ? res : -1;
}

static inline void encodeUTF8(std::string& dest, int32_t ch) {
    if (ch < 0) {
        dest.push_back(0xFF);
        return;
    }
    if (ch <= 0x7F) {
        dest.push_back(static_cast<char>(ch));
        return;
    }
    char buf[7];
    int sz = 0;
    while (ch) {
        buf[sz++] = ch & 0x3F;
        ch >>= 6;
    }
    buf[sz] = 0;
    sz += (static_cast<int>(buf[sz - 1]) >= 1 << (8 - sz - 1));
    buf[sz] = 0;
    for (int i = 0, j = sz - 1; i < j; ++i, --j) std::swap(buf[i], buf[j]);
    buf[0] |= static_cast<char>(0xFF << (8 - sz));
    for (int i = 1; i < sz; i++) buf[i] |= 0x80;
    dest.append(buf);
}

%}

%option extra-type="LC::Extra*"
%s STRING ESCAPE
%s CHAR

HEXINT [0-7][0-9a-fA-F]{7}|[0-9a-fA-F]{1,7}
HEXSHORT [0-9a-fA-F]{1,4}
HEXBYTE [0-9a-fA-F]{1,2}

    /* 2-6 bytes. Does not handle the [\x00-\x7F] case.
     * This regex permits non-minimal-length encodings,
     * but they are rejected in decodeUTF8.
     */
UTF8MULTICHAR [\xC0-\xDF][\x80-\xBF]|[\xE0-\xEF][\x80-\xBF]{2}|[\xF0-\xF7][\x80-\xBF]{3}|[\xF8-\xFB][\x80-\xBF]{4}|[\xFC-\xFD][\x80-\xBF]{5}

%%

<INITIAL>\/\*([^*]|\*+[^*/])*\*+?\*\/ ;

<INITIAL>\/\/.* ;

<INITIAL>\( return LC::Parser::make_LPAREN(yyextra->TakeLoc());
<INITIAL>\. return LC::Parser::make_DOT(yyextra->TakeLoc());
<INITIAL>\) return LC::Parser::make_RPAREN(yyextra->TakeLoc());
<INITIAL>; return LC::Parser::make_SEMI(yyextra->TakeLoc());
<INITIAL>\< return LC::Parser::make_LT(yyextra->TakeLoc());
<INITIAL>\> return LC::Parser::make_GT(yyextra->TakeLoc());
<INITIAL>chr\: return LC::Parser::make_SYM_4(yyextra->TakeLoc());
<INITIAL>const return LC::Parser::make_KW_const(yyextra->TakeLoc());
<INITIAL>dbl\: return LC::Parser::make_SYM_3(yyextra->TakeLoc());
<INITIAL>id return LC::Parser::make_KW_id(yyextra->TakeLoc());
<INITIAL>int\: return LC::Parser::make_SYM_2(yyextra->TakeLoc());
<INITIAL>l return LC::Parser::make_KW_l(yyextra->TakeLoc());
<INITIAL>str\: return LC::Parser::make_SYM_1(yyextra->TakeLoc());

    /* SpecialBegin */
<INITIAL>\<\:[1-9]+ return LC::Parser::make_CUSTOM_SpecialBegin(yytext, yyextra->TakeLoc());

    /* SpecialEnd */
<INITIAL>[1-9]+\:\> return LC::Parser::make_CUSTOM_SpecialEnd(yytext, yyextra->TakeLoc());

    /* String */
<INITIAL>\" BEGIN(STRING); yyextra->parsed.clear();
<STRING>\" {
        BEGIN(INITIAL);
        std::string result;
        result.swap(yyextra->parsed);
        return LC::Parser::make_STRING(result, yyextra->TakeLoc());
    }
<STRING>\\ BEGIN(ESCAPE);
<STRING>. yyextra->parsed.push_back(*yytext);
<ESCAPE>0 BEGIN(STRING); yyextra->parsed.push_back('\0');
<ESCAPE>a BEGIN(STRING); yyextra->parsed.push_back('\a');
<ESCAPE>b BEGIN(STRING); yyextra->parsed.push_back('\b');
<ESCAPE>f BEGIN(STRING); yyextra->parsed.push_back('\f');
<ESCAPE>n BEGIN(STRING); yyextra->parsed.push_back('\n');
<ESCAPE>r BEGIN(STRING); yyextra->parsed.push_back('\r');
<ESCAPE>t BEGIN(STRING); yyextra->parsed.push_back('\t');
<ESCAPE>v BEGIN(STRING); yyextra->parsed.push_back('\v');
<ESCAPE>x{HEXBYTE} {
        BEGIN(STRING);
        yyextra->parsed.push_back(
            static_cast<char>(hexInt32(yytext + 1, yyleng - 1)));
    }
<ESCAPE>u{HEXSHORT} |
<ESCAPE>U{HEXINT} {
        BEGIN(STRING);
        encodeUTF8(yyextra->parsed, hexInt32(yytext + 1, yyleng - 1));
    }
<ESCAPE>. BEGIN(STRING); yyextra->parsed.push_back(*yytext);


    /* Char in UTF-8 */
<INITIAL>' BEGIN(CHAR);
<CHAR>\\0' BEGIN(INITIAL); return LC::Parser::make_CHAR('\0', yyextra->TakeLoc());
<CHAR>\\a' BEGIN(INITIAL); return LC::Parser::make_CHAR('\a', yyextra->TakeLoc());
<CHAR>\\b' BEGIN(INITIAL); return LC::Parser::make_CHAR('\b', yyextra->TakeLoc());
<CHAR>\\f' BEGIN(INITIAL); return LC::Parser::make_CHAR('\f', yyextra->TakeLoc());
<CHAR>\\n' BEGIN(INITIAL); return LC::Parser::make_CHAR('\n', yyextra->TakeLoc());
<CHAR>\\r' BEGIN(INITIAL); return LC::Parser::make_CHAR('\r', yyextra->TakeLoc());
<CHAR>\\t' BEGIN(INITIAL); return LC::Parser::make_CHAR('\t', yyextra->TakeLoc());
<CHAR>\\v' BEGIN(INITIAL); return LC::Parser::make_CHAR('\v', yyextra->TakeLoc());
<CHAR>\x{HEXBYTE}'  |
<CHAR>\u{HEXSHORT}' |
<CHAR>\U{HEXINT}' {
        BEGIN(INITIAL);
        return LC::Parser::make_CHAR(hexInt32(yytext + 2, yyleng - 2), yyextra->TakeLoc());
    }
<CHAR>\\({UTF8MULTICHAR}|.)' {
        BEGIN(INITIAL);
        int32_t charcode = decodeUTF8(yytext + 1, yyleng - 2);
        auto loc = yyextra->TakeLoc();
        if (charcode == -1) return LEXER_ERROR(loc);
        return LC::Parser::make_CHAR(charcode, loc);
    }
<CHAR>({UTF8MULTICHAR}|[^'\\\n])' {
        BEGIN(INITIAL);
        int32_t charcode = decodeUTF8(yytext, yyleng - 1);
        auto loc = yyextra->TakeLoc();
        if (charcode == -1) return LEXER_ERROR(loc);
        return LC::Parser::make_CHAR(charcode, loc);
    }

    /* Integer (must be above Double) */
<INITIAL>[0-9]+ {
        const char* const end = yytext + yyleng;
        long num;
        auto res = std::from_chars(yytext, end, num);
        auto loc = yyextra->TakeLoc();
        if (res.ec == std::errc() && res.ptr == end)
            return LC::Parser::make_INTEGER(num, loc);
        return LEXER_ERROR(loc);
    }

    /* Double */
<INITIAL>[0-9]+(\.[0-9]+)?([eE][+\-]?[0-9]+)? {
        const char* const end = yytext + yyleng;
        double num;
        auto res = std::from_chars(yytext, end, num);
        auto loc = yyextra->TakeLoc();
        if (res.ec == std::errc() && res.ptr == end)
            return LC::Parser::make_DOUBLE(num, loc);
        else
            return LEXER_ERROR(loc);
    }

<INITIAL>[a-zA-Z_][a-zA-Z0-9_]* return LC::Parser::make_IDENT(yytext, yyextra->TakeLoc());

<INITIAL>[\t\n\f\r\x20]+ /* whitespace */ yyextra->TakeLoc();
<INITIAL><<EOF>> return LC::Parser::make_YYEOF(yyextra->TakeLoc());

[\x00-\xff] return LEXER_ERROR(yyextra->TakeLoc());

%%

namespace LC {

// optFilename is not owned by the lexer.
class LCScanner {
    yyscan_t scanner;
    LCScanner(std::string* optFilename);

public:
    LCScanner(FILE* file, std::string* optFilename);
    LCScanner(std::string_view str, std::string* optFilename);
    yyscan_t FlexScanner() const;
    ~LCScanner();
};

LCScanner::LCScanner(std::string* optFilename) {
    int err = LClex_init_extra(new Extra(optFilename), &scanner);
    if (err) throw std::system_error(err, std::generic_category(),
        "Cannot create scanner");
}

LCScanner::LCScanner(FILE* file, std::string* optFilename) : LCScanner(optFilename) {
    LCrestart(file, scanner);
}

LCScanner::LCScanner(std::string_view str, std::string* optFilename) : LCScanner(optFilename) {
    LC_scan_bytes(str.data(), str.size(), scanner);
}

yyscan_t LCScanner::FlexScanner() const { return scanner; }

LCScanner::~LCScanner() {
    delete LCget_extra(scanner);
    LClex_destroy(scanner);
}

}  // namespace LC
