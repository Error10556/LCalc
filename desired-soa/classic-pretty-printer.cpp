/*

The ClassicPrettyPrinter class operates on tokens. Each syntax node is converted
into a sequence of tokens, which are then printed in order, separated by spaces.
However, some tokens have special effects:

** Space-separation **
    - Left brackets '[' and parentheses '(' eat the separating space after
      themselves.
    - Commas ',', semicolons ';', and right brackets ']' and parentheses ')'
      eat the separating space before themselves.
    - Tokens that start and/or end with whitespace eat the separating space on
      the whitespace side(s).
    - Empty tokens prevent spacing.

** Layout **
    - Left curly braces '{' are printed on a separate line at the current
      indentation level and increase the indentation for subsequent tokens.
    - Right curly braces '}' are always printed on a separate line and decrease
      the indentation level. The brace itself is _also_ de-dented.
    - Semicolons ';' cause a line break after themselves.

Precedence is always resolved by enclosing a term in parentheses '()'.

*/

#include "classic-pretty-printer.hpp"

#include <cstdint>
#include <ostream>
#include <string>

#include "printer-common.hpp"

namespace ast::printing {

static void PutLinebreak(std::ostream& out, unsigned int indent) {
    out << '\n' << std::string(indent, ' ');
}

ClassicPrettyPrinter::ClassicPrettyPrinter(std::ostream& out, unsigned int tabSize)
    : out(out), tabSize(tabSize) {}

ClassicPrettyPrinter& ClassicPrettyPrinter::FlushHere() {
    if (needNewline) {
        PutLinebreak(out, indentInTabs * tabSize);
        needNewline = false;
        needSpace = false;
    } else if (needSpace) {
        out << ' ';
        needSpace = false;
    }
    return *this;
}

ClassicPrettyPrinter& ClassicPrettyPrinter::SimplePut(std::string_view s) {
    FlushHere();
    out << s;
    needSpace = true;
    return *this;
}

unsigned int ClassicPrettyPrinter::TabSize() const { return tabSize; }

unsigned int ClassicPrettyPrinter::IndentationLevel() const { return indentInTabs; }

int ClassicPrettyPrinter::CoercionLevel() const { return coercionLevel; }

ClassicPrettyPrinter& ClassicPrettyPrinter::WithTabSize(unsigned int tabSize) {
    this->tabSize = tabSize;
    return *this;
}

ClassicPrettyPrinter& ClassicPrettyPrinter::Indent(unsigned int tabs) {
    indentInTabs += tabs;
    return *this;
}

ClassicPrettyPrinter& ClassicPrettyPrinter::Dedent(unsigned int tabs) {
    if (tabs > indentInTabs)
        indentInTabs = 0;
    else
        indentInTabs -= tabs;
    return *this;
}

ClassicPrettyPrinter& ClassicPrettyPrinter::WithIndent(unsigned int tabs) {
    indentInTabs = tabs;
    return *this;
}

ClassicPrettyPrinter& ClassicPrettyPrinter::SkipSpaceHere() {
    needSpace = false;
    return *this;
}

ClassicPrettyPrinter& ClassicPrettyPrinter::NeedSpaceHere() {
    needSpace = true;
    return *this;
}

static bool IsSpace(char ch) {
    switch (ch) {
        case ' ':
        case '\r':
        case '\n':
        case '\t':
        case '\v':
            return true;
        default:
            return false;
    }
}

ClassicPrettyPrinter& ClassicPrettyPrinter::PutToken(std::string_view s) {
    if (s.empty()) {
        SkipSpaceHere();
        return *this;
    }
    if (s.size() == 1) {
        bool handled = true;
        switch (s.front()) {
            case '{':
                OnNewLine().SimplePut(s).Indent().OnNewLine();
                break;
            case '}':
                OnNewLine().Dedent().SimplePut(s).OnNewLine();
                break;
            case ';':
                SkipSpaceHere().SimplePut(s).OnNewLine();
                break;
            case ',':
            case ')':
            case ']':
                SkipSpaceHere().SimplePut(s).NeedSpaceHere();
                break;
            case '(':
            case '[':
                SimplePut(s).SkipSpaceHere();
                break;
            default:
                handled = false;
                break;
        }
        if (handled) return *this;
    }
    if (IsSpace(s.front())) SkipSpaceHere();
    SimplePut(s);
    if (IsSpace(s.back())) SkipSpaceHere();
    return *this;
}

ClassicPrettyPrinter& ClassicPrettyPrinter::PutCharLiteral(Char ch) {
    FlushHere();
    print_escaped_char(out, static_cast<std::int32_t>(ch));
    return NeedSpaceHere();
}

ClassicPrettyPrinter& ClassicPrettyPrinter::PutStringLiteral(std::string_view s) {
    FlushHere();
    print_escaped_string(out, s);
    return NeedSpaceHere();
}

ClassicPrettyPrinter& ClassicPrettyPrinter::PutDoubleLiteral(double v) {
    FlushHere();
    print_double(out, v);
    return NeedSpaceHere();
}

ClassicPrettyPrinter& ClassicPrettyPrinter::PutIntegerLiteral(Integer v) {
    FlushHere();
    out << std::to_string(v);
    return NeedSpaceHere();
}

ClassicPrettyPrinter& ClassicPrettyPrinter::OnNewLine() {
    needSpace = false;
    needNewline = true;
    return *this;
}

ClassicPrettyPrinter& ClassicPrettyPrinter::WithCoercionLevel(int level) {
    coercionLevel = level;
    return *this;
}

}  // namespace ast::printing
