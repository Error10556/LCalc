#include "PrinterCommon.hpp"
#include <charconv>
#include <system_error>

namespace LC {

void PrintEscapedCharRaw(std::ostream& out, int32_t ch) {
    switch (ch) {
        case '\0':
            out << "\\0";
            break;
        case '\a':
            out << "\\a";
            break;
        case '\b':
            out << "\\b";
            break;
        case '\f':
            out << "\\f";
            break;
        case '\n':
            out << "\\n";
            break;
        case '\r':
            out << "\\r";
            break;
        case '\t':
            out << "\\t";
            break;
        case '\v':
            out << "\\v";
            break;
        case '\\':
            out << "\\\\";
            break;
        default:
            if (0x20 <= ch && ch < 0x7F) {
                out << static_cast<char>(ch);
                break;
            }
            int digitcount;
            if (0 <= ch && ch <= 0xFF) {
                out << "\\x";
                digitcount = 2;
            } else if (0 <= ch && ch <= 0xFFFF) {
                out << "\\u";
                digitcount = 4;
            } else {
                out << "\\U";
                digitcount = 8;
            }
            for (int i = (digitcount - 1) * 4; i >= 0; i -= 4)
                out << "0123456789ABCDEF"[(ch >> i) & 0xF];
            break;
    }
}

void PrintEscapedString(std::ostream& out, std::string_view s) {
    out << '"';
    for (char ch : s) {
        if (ch < 0)  // do not touch utf-8 non-ascii
            out << ch;
        else if (ch == '"')
            out << "\\\"";
        else
            PrintEscapedCharRaw(out, ch);
    }
    out << '"';
}

void PrintEscapedChar(std::ostream& out, int32_t ch) {
    out << '\'';
    if (ch < 0)
        out << "BAD_CHAR";
    else if (ch == static_cast<int32_t>('\''))
        out << "\\'";
    else
        PrintEscapedCharRaw(out, ch);
    out << '\'';
}

void PrintDouble(std::ostream& out, double val) {
    char buf[128];
    auto res = std::to_chars(buf, buf + sizeof(buf), val);
    if (res.ec != std::errc())
        throw std::system_error(std::make_error_code(res.ec));
    out << std::string_view(buf, res.ptr - buf);
}

}  // namespace LC
