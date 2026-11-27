#pragma once
#include <compare>
#include <algorithm>

namespace ast::lib::locations {

// operator+ on *Diff, *Range, Dynamic*Annotation: NOT commutative; associative

struct TextLocationDiff;
using TextLocation = TextLocationDiff;

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
    void add_newline() {
        ++n_newlines;
        n_last_line_columns = 0;
    }
    void add_column() { ++n_last_line_columns; }
};

static_assert((TextLocation{0, 0} <=> TextLocation{0, 0}) == 0);
static_assert((TextLocation{0, 1} <=> TextLocation{0, 0}) > 0);
static_assert((TextLocation{1, 0} <=> TextLocation{0, 0}) > 0);
static_assert((TextLocation{1, 1} <=> TextLocation{0, 0}) > 0);
static_assert((TextLocation{0, 0} <=> TextLocation{0, 1}) < 0);
static_assert((TextLocation{0, 1} <=> TextLocation{0, 1}) == 0);
static_assert((TextLocation{1, 0} <=> TextLocation{0, 1}) > 0);
static_assert((TextLocation{1, 1} <=> TextLocation{0, 1}) > 0);
static_assert((TextLocation{0, 0} <=> TextLocation{1, 0}) < 0);
static_assert((TextLocation{0, 1} <=> TextLocation{1, 0}) < 0);
static_assert((TextLocation{1, 0} <=> TextLocation{1, 0}) == 0);
static_assert((TextLocation{1, 1} <=> TextLocation{1, 0}) > 0);
static_assert((TextLocation{0, 0} <=> TextLocation{1, 1}) < 0);
static_assert((TextLocation{0, 1} <=> TextLocation{1, 1}) < 0);
static_assert((TextLocation{1, 0} <=> TextLocation{1, 1}) < 0);
static_assert((TextLocation{1, 1} <=> TextLocation{1, 1}) == 0);

using ByteLocationDiff = unsigned long;
using ByteLocation = ByteLocationDiff;

struct CombinedLocationDiff;
using CombinedLocation = CombinedLocationDiff;

struct CombinedLocationDiff {
    TextLocationDiff text;
    ByteLocationDiff byte{};
    CombinedLocationDiff& operator+=(const CombinedLocationDiff& other) {
        text += other.text;
        byte += other.byte;
        return *this;
    }
    CombinedLocationDiff operator+(const CombinedLocationDiff& other) const {
        return CombinedLocationDiff(*this) += other;
    }
    static CombinedLocationDiff Difference(const CombinedLocation& begin,
                                           const CombinedLocation& end) {
        return {TextLocationDiff::Difference(begin.text, end.text), end.byte - begin.byte};
    }
    constexpr bool operator<(const CombinedLocationDiff& other) const {
        return text < other.text && byte < other.byte;
    }
    constexpr std::partial_ordering operator<=>(const CombinedLocationDiff& other) const {
        if (*this == other) return std::partial_ordering::equivalent;
        if (*this < other) return std::partial_ordering::less;
        if (other < *this) return std::partial_ordering::greater;
        return std::partial_ordering::unordered;
    }
    constexpr bool operator==(const CombinedLocationDiff& other) const {
        return text == other.text && byte == other.byte;
    }
    constexpr bool operator!=(const CombinedLocationDiff& other) const {
        return text != other.text || byte != other.byte;
    }
};

struct TextRange {
    TextLocation start;
    TextLocation end;
    TextRange& operator+=(const TextRange& other) {
        end += other.end;
        return *this;
    }
    TextRange operator+(const TextRange& other) const {
        return TextRange(*this) += other;
    }
    constexpr bool operator<(const TextRange& other) const {
        return start <= other.start && end <= other.end && *this != other;
    }
    bool operator==(const TextRange& other) const = default;
    bool operator!=(const TextRange& other) const = default;
    constexpr std::partial_ordering operator<=>(const TextRange& other) const {
        if (*this == other) return std::partial_ordering::equivalent;
        if (*this < other) return std::partial_ordering::less;
        if (other < *this) return std::partial_ordering::greater;
        return std::partial_ordering::unordered;
    }
    bool Intersects(const TextRange& other) const {
        return !(end <= other.start || other.end <= start);
    }
    TextRange Intersection(const TextRange& other) const {
        return {std::max(start, other.start), std::min(end, other.end)};
    }
    bool Contains(const TextRange& other) const {
        return start <= other.start && other.end <= end;
    }
    TextRange Union(const TextRange& other) const {
        return {std::min(start, other.start), std::max(end, other.end)};
    }
};

struct ByteRange {
    ByteLocation start{};
    ByteLocation end{};
    ByteRange& operator+=(const ByteRange& other) {
        end += other.end;
        return *this;
    }
    ByteRange operator+(const ByteRange& other) const {
        return ByteRange(*this) += other;
    }
    bool operator==(const ByteRange& other) const = default;
    bool operator!=(const ByteRange& other) const = default;
    constexpr bool operator<(const ByteRange& other) const {
        return start <= other.start && end <= other.end && *this != other;
    }
    constexpr std::partial_ordering operator<=>(const ByteRange& other) const {
        if (*this == other) return std::partial_ordering::equivalent;
        if (*this < other) return std::partial_ordering::less;
        if (other < *this) return std::partial_ordering::greater;
        return std::partial_ordering::unordered;
    }
    bool Intersects(const ByteRange& other) const {
        return !(end <= other.start || other.end <= start);
    }
    ByteRange Intersection(const ByteRange& other) const {
        return {std::max(start, other.start), std::min(end, other.end)};
    }
    bool Contains(const ByteRange& other) const {
        return start <= other.start && other.end <= end;
    }
    ByteRange Union(const ByteRange& other) const {
        return {std::min(start, other.start), std::max(end, other.end)};
    }
};

struct CombinedRange {
    TextRange text;
    ByteRange byte;
    CombinedRange& operator+=(const CombinedRange& other) {
        text += other.text;
        byte += other.byte;
        return *this;
    }
    CombinedRange operator+(const CombinedRange& other) const {
        return CombinedRange(*this) += other;
    }
    bool operator==(const CombinedRange& other) const = default;
    bool operator!=(const CombinedRange& other) const = default;
    constexpr bool operator<(const CombinedRange& other) const {
        return text < other.text && byte < other.byte;
    }
    constexpr std::partial_ordering operator<=>(const CombinedRange& other) const {
        if (*this == other) return std::partial_ordering::equivalent;
        if (*this < other) return std::partial_ordering::less;
        if (other < *this) return std::partial_ordering::greater;
        return std::partial_ordering::unordered;
    }
    bool Intersects(const CombinedRange& other) const {
        return text.Intersects(other.text) && byte.Intersects(other.byte);
    }
    CombinedRange Intersection(const CombinedRange& other) const {
        return {text.Intersection(other.text), byte.Intersection(other.byte)};
    }
    bool Contains(const CombinedRange& other) const {
        return text.Contains(other.text) && byte.Contains(other.byte);
    }
    CombinedRange Union(const CombinedRange& other) const {
        return {text.Union(other.text), byte.Union(other.byte)};
    }
};

struct DynamicTextRangeAnnotation {
    TextLocationDiff preWhitespace;
    TextLocationDiff total;
    DynamicTextRangeAnnotation& operator+=(const DynamicTextRangeAnnotation& other) {
        total += other.total;
        return *this;
    }
    DynamicTextRangeAnnotation operator+(const DynamicTextRangeAnnotation& other) const {
        return DynamicTextRangeAnnotation(*this) += other;
    }
    TextRange Bake(TextLocation whitespaceStart) const {
        return {whitespaceStart + preWhitespace, whitespaceStart + total};
    }
};

struct DynamicByteRangeAnnotation {
    ByteLocationDiff preWhitespace;
    ByteLocationDiff total;
    DynamicByteRangeAnnotation& operator+=(const DynamicByteRangeAnnotation& other) {
        total += other.total;
        return *this;
    }
    DynamicByteRangeAnnotation operator+(const DynamicByteRangeAnnotation& other) const {
        return DynamicByteRangeAnnotation(*this) += other;
    }
    ByteRange Bake(ByteLocation whitespaceStart) const {
        return {whitespaceStart + preWhitespace, whitespaceStart + total};
    }
};

}  // namespace ast::lib::locations
