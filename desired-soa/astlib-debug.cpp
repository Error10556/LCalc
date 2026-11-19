#include "astlib-debug.hpp"

#include <charconv>
#include <ostream>
#include <sstream>
#include <string_view>

namespace ast::lib::debug {
namespace {

constexpr char hex_digits[] = "0123456789abcdef";

void write_hex_byte(std::ostream& out, unsigned char byte) {
    out.put(hex_digits[byte >> 4]);
    out.put(hex_digits[byte & 0x0f]);
}

void write_value(std::ostream& out, std::uint32_t word) {
    // out << "0x";
    for (int shift = 24; shift >= 0; shift -= 8)
        write_hex_byte(out, static_cast<unsigned char>(word >> shift));
}

void write_value(std::ostream& out, const std::array<std::uint32_t, 4>& row) {
    write_value(out, row.front());
    for (std::size_t i = 1; i < row.size(); ++i) {
        out.put(' ');
        write_value(out, row[i]);
    }
}

void write_value(std::ostream& out, const std::string& value) {
    out.put('"');
    for (unsigned char byte : value) {
        switch (byte) {
            case '\\':
                out << "\\\\";
                break;
            case '"':
                out << "\\\"";
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
            default:
                if (byte >= 0x20 && byte <= 0x7e) {
                    out.put(static_cast<char>(byte));
                } else {
                    out << "\\x";
                    write_hex_byte(out, byte);
                }
        }
    }
    out.put('"');
}

void write_value(std::ostream& out, const ASTDump::Blob& blob) { out << blob; }

bool parse_index(std::string_view text, std::size_t& index) {
    const auto end = text.data() + text.size();
    uint32_t radix = 10;
    auto start = text.data();
    if (text.starts_with("0x")) {
        radix = 16;
        start += 2;
    }
    const auto result = std::from_chars(start, end, index, radix);
    return result.ec == std::errc{} && result.ptr == end;
}

template <class T>
void write_rows(std::istream& args, std::ostream& out, const std::vector<T>& rows) {
    std::size_t first = 0;
    std::size_t count = rows.size();
    std::string index_text, count_text, extra;
    if (args >> index_text) {
        count = 1;
        if (!parse_index(index_text, first) ||
            ((args >> count_text) && !parse_index(count_text, count)) || (args >> extra) ||
            count == 0) {
            out << "Expected a decimal index and an optional positive count.\n";
            return;
        }
        if (first >= rows.size() || count > rows.size() - first) {
            out << "Row range out of bounds (" << rows.size() << " entries).\n";
            return;
        }
    }
    if (count == 0) out << "(empty)\n";
    for (std::size_t i = first; i < first + count; ++i) {
        write_value(out, static_cast<uint32_t>(i));
        out << ": ";
        write_value(out, rows[i]);
        out.put('\n');
    }
}

constexpr char help[] =
    "summary                         Show the number of entries in each section\n"
    "tree [index [count]]            Show tree words\n"
    "extras [index [count]]          Show extra words\n"
    "strings [index [count]]         Show string tokens with byte escapes\n"
    "annots [column [index [count]]] List annotation columns or show their bytes\n"
    "help                            Show this help\n"
    "quit                            End inspection\n"
    "Indices and column numbers are zero-based decimal numbers or hexadecimal if prefixed with 0x.\n"
    "Omit the index to show all rows. With an index, the count defaults to one.\n";

}  // namespace

std::ostream& operator<<(std::ostream& out, const ASTDump::Blob& blob) {
    bool first = true;
    for (unsigned char byte : blob.span()) {
        if (!first) out.put(' ');
        write_hex_byte(out, byte);
        first = false;
    }
    return out;
}

void ASTDump::interact(std::istream& in, std::ostream& out) {
    out << "AST dump. Type help for commands.\n";
    std::string line;
    while ((out << "ast> " << std::flush) && std::getline(in, line)) {
        std::istringstream args(line);
        std::string command;
        if (!(args >> command)) continue;

        if (command == "tree") {
            write_rows(args, out, tree);
        } else if (command == "extras") {
            write_rows(args, out, extras);
        } else if (command == "strings") {
            write_rows(args, out, strings);
        } else if (command == "annots") {
            std::string column_text;
            if (args >> column_text) {
                std::size_t column;
                if (!parse_index(column_text, column)) {
                    out << "Expected a decimal annotation column number.\n";
                } else if (column >= annots.size()) {
                    out << "Annotation column out of bounds (" << annots.size() << " columns).\n";
                } else {
                    write_rows(args, out, annots[column]);
                }
            } else {
                if (annots.empty()) out << "(empty)\n";
                for (std::size_t i = 0; i < annots.size(); ++i) {
                    const std::string_view name =
                        i < annot_names.size() ? std::string_view(annot_names[i]) : "(unnamed)";
                    out << i << ": " << name << " (" << annots[i].size() << " entries)\n";
                }
            }
        } else if (command == "help" || command == "summary" || command == "quit") {
            std::string extra;
            if (args >> extra) {
                out << command << " takes no arguments.\n";
            } else if (command == "quit") {
                return;
            } else if (command == "summary") {
                out << "Tree rows: " << tree.size() << '\n'
                    << "Extra words: " << extras.size() << '\n'
                    << "String tokens: " << strings.size() << '\n'
                    << "Annotation columns: " << annots.size() << '\n';
            } else {
                out << help;
            }
        } else {
            out << "Unknown command. Type help for commands.\n";
        }
    }
}

}  // namespace ast::lib::debug
