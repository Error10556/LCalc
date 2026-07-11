#pragma once
#include <ostream>
#include <string_view>

namespace LC {

void PrintEscapedCharRaw(std::ostream& out, int32_t ch);

// As PrintEscapedCharRaw for each character,
// but also escapes double quotes (").
// Puts the string in double quotes.
void PrintEscapedString(std::ostream& out, std::string_view s);

// As PrintEscapedCharRaw, but also escapes single quotes (').
// Puts the character in single quotes.
void PrintEscapedChar(std::ostream& out, int32_t ch);

void PrintDouble(std::ostream& out, double val);

}  // namespace LC
