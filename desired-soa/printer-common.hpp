#pragma once
#include <cstdint>
#include <ostream>
#include <string_view>

namespace ast::printing {

void print_escaped_char_raw(std::ostream& out, int32_t ch);

// As print_escaped_char_raw for each character,
// but also escapes double quotes (").
// Puts the string in double quotes.
void print_escaped_string(std::ostream& out, std::string_view s);

// As print_escaped_char_raw, but also escapes single quotes (').
// Puts the character in single quotes.
void print_escaped_char(std::ostream& out, int32_t ch);

void print_double(std::ostream& out, double val);

}  // namespace ast::printing
