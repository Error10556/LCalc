%option warn nodefault
%option 8bit reentrant
%option noyywrap
%option prefix="LC"
%option outfile="grammar.lex.cpp"

%{
#include <charconv>
#include <string>
#include <string_view>
#include <system_error>
#include "grammar.tab.hpp"

#define YY_DECL LC::Parser::symbol_type LClex(yyscan_t yyscanner)

inline int hexDigitValue(char ch) {
    if ('0' <= ch && ch <= '9') return ch - '0';
    if ('a' <= ch && ch <= 'f') return ch - 'a' + 10;
    if ('A' <= ch && ch <= 'F') return ch - 'A' + 10;
    return 0;
}

inline int32_t hexInt32(const char* start, int len) {
    int32_t val = 0;
    for (int i = 0; i < len; i++) val = (val << 8) | hexDigitValue(start[i]);
    return val;
}

inline int32_t minCharForEncodedLen(int len) {
    if (len < 2) return 0;
    if (len == 2) return 0x80;
    return static_cast<int32_t>(1) << (11 + 5 * (len - 3));
}

    /* returns -1 if a character uses a non-minimal # of bytes */
inline int decodeUTF8(const char* start, int len) {
    if (len == 1) return *start;
    int firstbits = 7 - len;
    int32_t res = *start & ((1 << firstbits) - 1);
    for (int i = 1; i < len; i++) res = (res << 6) | (start[i] & 0x3f);
    return res >= minCharForEncodedLen(len) ? res : -1;
}

inline int32_t bitSegment(int shiftr, int masklen, int32_t val) {
    return (val >> shiftr) & ((static_cast<int32_t>(1) << masklen) - 1);
}

inline void encodeUTF8(std::string& dest, int32_t ch) {
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
    for (int i = 0, j = sz - 1; i < j; ++i, --j) std::swap(buf[i], buf[j]);
    buf[0] |= static_cast<char>(0xFF << (8 - sz));
    for (int i = 1; i < sz; i++) buf[i] |= 0x80;
    dest.append(buf);
}

%}

%option extra-type="std::string*"

%s COMMENT STRING ESCAPE CHAR

HEXINT [0-7][0-9a-fA-F]{7}|[0-9a-fA-F]{1,7}

    /* 2-6 bytes. Does not handle the [\x00-\x7F] case.
     * This regex permits non-minimal-length encodings,
     * but they are rejected in decodeUTF8.
     */
UTF8MULTICHAR [\xC0-\xDF][\x80-\xBF]|[\xE0-\xEF][\x80-\xBF]{2}|[\xF0-\xF7][\x80-\xBF]{3}|[\xF8-\xFB][\x80-\xBF]{4}|[\xFC-\xFD][\x80-\xBF]{5}

%%

<INITIAL>"/*" BEGIN(COMMENT);
<COMMENT>[^*]+ ;
<COMMENT>"*" ;
<COMMENT>"*/" BEGIN(INITIAL);
<INITIAL>"//".* ;

    /* String */
<INITIAL>\" BEGIN(STRING); yyextra->clear();
<STRING>\" BEGIN(INITIAL); return LC::Parser::make_STRING(*yyextra);
<STRING>\\ BEGIN(ESCAPE);
<STRING>. yyextra->push_back(*yytext);
<ESCAPE>0 BEGIN(STRING); yyextra->push_back('\0');
<ESCAPE>a BEGIN(STRING); yyextra->push_back('\a');
<ESCAPE>b BEGIN(STRING); yyextra->push_back('\b');
<ESCAPE>f BEGIN(STRING); yyextra->push_back('\f');
<ESCAPE>n BEGIN(STRING); yyextra->push_back('\n');
<ESCAPE>r BEGIN(STRING); yyextra->push_back('\r');
<ESCAPE>t BEGIN(STRING); yyextra->push_back('\t');
<ESCAPE>v BEGIN(STRING); yyextra->push_back('\v');
<ESCAPE>x{HEXINT} {
        BEGIN(STRING);
        encodeUTF8(*yyextra, hexInt32(yytext, yylen - 1));
    }
<ESCAPE>. BEGIN(STRING); yyextra->push_back(*yytext);

    /* Integer (must be above Double) */
<INITIAL>[+\-]?[0-9]+ {
        const char* const start = yytext + (*yytext == '+');
        const char* const end = yytext + yylen;
        long num;
        auto res = std::from_chars(start, end, num);
        if (res.ec == std::errc() && res.ptr == end)
            return LC::Parser::make_INTEGER(num);
        else
            return LC::Parser::make_YYerror();
    }

    /* Double */
<INITIAL>[+\-]?[0-9]+(\.[0-9]+)?([eE][+\-]?[0-9]+)? {
        const char* const start = yytext + (*yytext == '+');
        const char* const end = yytext + yylen;
        double num;
        auto res = std::from_chars(start, end, num);
        if (res.ec == std::errc() && res.ptr == end)
            return LC::Parser::make_DOUBLE(num);
        else
            return LC::Parser::make_YYerror();
    }

    /* Char in UTF-8 */
<INITIAL>' BEGIN(CHAR);
<CHAR>\\0' BEGIN(INITIAL); return LC::Parser::make_CHAR('\0');
<CHAR>\\a' BEGIN(INITIAL); return LC::Parser::make_CHAR('\a');
<CHAR>\\b' BEGIN(INITIAL); return LC::Parser::make_CHAR('\b');
<CHAR>\\f' BEGIN(INITIAL); return LC::Parser::make_CHAR('\f');
<CHAR>\\n' BEGIN(INITIAL); return LC::Parser::make_CHAR('\n');
<CHAR>\\r' BEGIN(INITIAL); return LC::Parser::make_CHAR('\r');
<CHAR>\\t' BEGIN(INITIAL); return LC::Parser::make_CHAR('\t');
<CHAR>\\v' BEGIN(INITIAL); return LC::Parser::make_CHAR('\v');
<CHAR>\\x{HEXINT}' {
        BEGIN(INITIAL);
        return LC::Parser::make_CHAR(hexInt32(yytext + 2, yylen - 2));
    }
<CHAR>\\({UTF8MULTICHAR}|.)' {
        BEGIN(INITIAL);
        int32_t charcode = decodeUTF8(yytext + 1, yylen - 2);
        if (charcode == -1) return LC::Parser::make_YYerror();
        return LC::Parser::make_CHAR(charcode);
    }
<CHAR>({UTF8MULTICHAR}|[^'\\])' {
        BEGIN(INITIAL);
        int32_t charcode = decodeUTF8(yytext, yylen - 1);
        if (charcode == -1) return LC::Parser::make_YYerror();
        return LC::Parser::make_CHAR(charcode);
    }

<INITIAL>"l" return LC::Parser::make_KW_l();
<INITIAL>[_a-zA-Z][_a-zA-Z0-9]* return LC::Parser::make_IDENT(yytext);
<INITIAL>"(" return LC::Parser::make_LPAREN();
<INITIAL>")" return LC::Parser::make_RPAREN();
<INITIAL>"." return LC::Parser::make_DOT();
<INITIAL>";" return LC::Parser::make_SEMI();
<INITIAL><<EOF>> return LC::Parser::make_YYEOF();
<INITIAL>[ \n\t\f\v] ;

[\x00-\xff] return LC::Parser::make_YYerror();

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
