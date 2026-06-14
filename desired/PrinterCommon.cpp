#include "PrinterCommon.hpp"
#include <charconv>
#include <string_view>
#include <system_error>

namespace LC {

void PrintEscapedCharRaw(std::ostream& out, int32_t ch) {
    static constexpr const char HEX_DIGITS[] = "0123456789ABCDEF";
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
            out << '\\' << ch;
            break;
        default:
            if (0x20 <= ch && ch < 0x7F) {
                out << ch;
                break;
            }
            out << "\\x";
            for (int i = 28; i >= 0; i -= 4)
                out << HEX_DIGITS[(ch >> i) & 0xF];
            break;
    }
}

void PrintEscapedString(std::ostream& out, const std::string& s) {
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
