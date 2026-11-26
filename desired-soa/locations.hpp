#pragma once
#include <compare>

namespace ast::lib::locations {

struct TextLocationDiff;
using TextLocation = TextLocationDiff;

// operator+ : NOT commutative; associative
struct TextLocationDiff {
    unsigned int n_newlines{};
    unsigned int n_last_line_columns{};
    TextLocationDiff& operator+=(TextLocationDiff other) {
        if (other.n_newlines) {
            n_newlines += other.n_newlines;
            n_last_line_columns = other.n_last_line_columns;
        } else
            n_last_line_columns += other.n_last_line_columns;
        return *this;
    }
    TextLocationDiff operator+(TextLocationDiff other) const {
        return TextLocationDiff(*this) += other;
    }
    static TextLocationDiff Difference(TextLocation begin, TextLocation end) {
        if (begin.n_newlines > end.n_newlines) [[unlikely]]
            return {};
        if (begin.n_newlines < end.n_newlines)
            return {end.n_newlines - begin.n_newlines, end.n_last_line_columns};
        if (begin.n_last_line_columns > end.n_last_line_columns) [[unlikely]]
            return {};
        return {0U, end.n_last_line_columns - begin.n_last_line_columns};
    }
    std::strong_ordering operator<=>(const TextLocationDiff& other) const = default;
    bool operator==(const TextLocationDiff& other) const = default;
    bool operator!=(const TextLocationDiff& other) const = default;
};

using byte_location_diff_t = unsigned long;
using byte_location_t = byte_location_diff_t;

struct TextRange {
    TextLocation start;
    TextLocation end;
};

struct ByteRange {
    byte_location_t start;
    byte_location_t end;
};

struct DynamicTextRangeAnnotation {
    TextLocationDiff preWhitespace;
    TextLocationDiff total;
};

struct DynamicNodeByteRangeAnnotation {
    TextLocationDiff preWhitespace;
    TextLocationDiff total;
};

}  // namespace ast::lib::locations
