/********************************  Disclaimer  *********************************

The ContextFreePrettyPrinter class attempts to implement pretty-printing while
having as little internal state as possible, only depending on the indentation
level and the expected precedence level of the printed syntax subtree. The
minimal state should help the programmer modify the pretty-printer.

The default ContextFreePrettyPrinter implementation tries to follow these rules:

* All tokens are separated from each other by a space. The exceptions are:
  - commas ',' and semicolons ';', which are only separated from the right;
  - brackets '[]' and parentheses '()', which are not separated from the
    enclosed text;
  - tokens that start and/or end with whitespace, which are not separated on the
    whitespace side(s);
  - empty tokens prevent space-separation.

* Curly braces '{}' (and only those) enclose an indented block. The indentation
  is controlled by a global constant INDENT and equals 4 spaces by default. The
  left curly brace causes one line break _after_ itself, the right one causes
  breaks _before and after_ itself. The right brace is not indented.

  NB: brace pairs are only detected within one syntax node. Braces without pairs
  are treated like parentheses (not space-separated from what is inside).

* Semicolons ';' cause a line break after themselves.

* Precedence is always resolved by enclosing a term in parentheses '()'.

Note that only tokens specified in the grammar as literal strings affect
spacing, indentation, and line breaks in the generated implementation; tokens
defined using pragmas do not.

You are encouraged to _PATCH(1)_ this file with your own implementations of some
methods. If that is unacceptable and the printed text is not pretty, try
ClassicPrettyPrinter.

*******************************************************************************/

#include "ContextFreePrettyPrinter.hpp"

#include "PrinterCommon.hpp"

namespace LC {

#define IF_BAD_COERC(classname) \
    if (coercionLevel > reflection::CoercionLevel<classname>)

constexpr static const unsigned int INDENT [[maybe_unused]] = 4;

ContextFreePrettyPrinter::ContextFreePrettyPrinter(
    std::ostream& out, unsigned int indent, int coercionLevel)
    : out(out), indent(indent), coercionLevel(coercionLevel) {}

void ContextFreePrettyPrinter::NewLine(unsigned int indent) const {
    out << '\n' << std::string(indent, ' ');
}

void ContextFreePrettyPrinter::NewLine() const { NewLine(indent); }

ContextFreePrettyPrinter
    ContextFreePrettyPrinter::WithCoercionLevel(int level) const {
    return {out, indent, level};
}

ContextFreePrettyPrinter ContextFreePrettyPrinter::Indented(
    unsigned int plusIndent, int coercionLevel) const {
    return {out, indent + plusIndent, coercionLevel};
}

ContextFreePrettyPrinter ContextFreePrettyPrinter::Dedented(
    unsigned int minusIndent, int coercionLevel) const {
    return {out, minusIndent > indent ? 0 : indent - minusIndent,
            coercionLevel};
}

void ContextFreePrettyPrinter::operator()(const Ident& v) const {
    IF_BAD_COERC(Ident) out << '(';
    out << v.Value;
    IF_BAD_COERC(Ident) out << ')';
}

void ContextFreePrettyPrinter::operator()(const Char& v) const {
    IF_BAD_COERC(Char) out << '(';
    PrintEscapedChar(out, v.Value);
    IF_BAD_COERC(Char) out << ')';
}

void ContextFreePrettyPrinter::operator()(const Double& v) const {
    IF_BAD_COERC(Double) out << '(';
    PrintDouble(out, v.Value);
    IF_BAD_COERC(Double) out << ')';
}

void ContextFreePrettyPrinter::operator()(const Integer& v) const {
    IF_BAD_COERC(Integer) out << '(';
    out << v.Value;
    IF_BAD_COERC(Integer) out << ')';
}

void ContextFreePrettyPrinter::operator()(const String& v) const {
    IF_BAD_COERC(String) out << '(';
    PrintEscapedString(out, v.Value);
    IF_BAD_COERC(String) out << ')';
}

void ContextFreePrettyPrinter::operator()(const SpecialBegin& v) const {
    IF_BAD_COERC(SpecialBegin) out << '(';
    out << v.Value;
    IF_BAD_COERC(SpecialBegin) out << ')';
}

void ContextFreePrettyPrinter::operator()(const SpecialEnd& v) const {
    IF_BAD_COERC(SpecialEnd) out << '(';
    out << v.Value;
    IF_BAD_COERC(SpecialEnd) out << ')';
}

void ContextFreePrettyPrinter::operator()(const Expr& v) const {
    std::visit(*this, v);
}

void ContextFreePrettyPrinter::operator()(const Variable& v [[maybe_unused]]) const {
    IF_BAD_COERC(Variable) out << '(';
    WithCoercionLevel(0)(v.Ident_);
    IF_BAD_COERC(Variable) out << ')';
}

void ContextFreePrettyPrinter::operator()(const Application& v [[maybe_unused]]) const {
    IF_BAD_COERC(Application) out << '(';
    WithCoercionLevel(1)(*v.Expr_1);
    out << " ";
    WithCoercionLevel(2)(*v.Expr_2);
    IF_BAD_COERC(Application) out << ')';
}

void ContextFreePrettyPrinter::operator()(const Abstraction& v [[maybe_unused]]) const {
    IF_BAD_COERC(Abstraction) out << '(';
    out << "l ";
    WithCoercionLevel(0)(v.Ident_);
    out << " . ";
    WithCoercionLevel(0)(*v.Expr_);
    IF_BAD_COERC(Abstraction) out << ')';
}

void ContextFreePrettyPrinter::operator()(const StringExpr& v [[maybe_unused]]) const {
    IF_BAD_COERC(StringExpr) out << '(';
    out << "str: ";
    WithCoercionLevel(0)(v.String_);
    IF_BAD_COERC(StringExpr) out << ')';
}

void ContextFreePrettyPrinter::operator()(const IntegerExpr& v [[maybe_unused]]) const {
    IF_BAD_COERC(IntegerExpr) out << '(';
    out << "int: ";
    WithCoercionLevel(0)(v.Integer_);
    IF_BAD_COERC(IntegerExpr) out << ')';
}

void ContextFreePrettyPrinter::operator()(const DoubleExpr& v [[maybe_unused]]) const {
    IF_BAD_COERC(DoubleExpr) out << '(';
    out << "dbl: ";
    WithCoercionLevel(0)(v.Double_);
    IF_BAD_COERC(DoubleExpr) out << ')';
}

void ContextFreePrettyPrinter::operator()(const CharExpr& v [[maybe_unused]]) const {
    IF_BAD_COERC(CharExpr) out << '(';
    out << "chr: ";
    WithCoercionLevel(0)(v.Char_);
    IF_BAD_COERC(CharExpr) out << ')';
}

void ContextFreePrettyPrinter::operator()(const SpecialExpr& v [[maybe_unused]]) const {
    IF_BAD_COERC(SpecialExpr) out << '(';
    WithCoercionLevel(0)(v.SpecialBegin_);
    out << " ";
    WithCoercionLevel(0)(*v.Expr_);
    out << " ";
    WithCoercionLevel(0)(v.SpecialEnd_);
    IF_BAD_COERC(SpecialExpr) out << ')';
}

void ContextFreePrettyPrinter::operator()(const Program& v) const {
    std::visit(*this, v);
}

void ContextFreePrettyPrinter::operator()(const AProgram& v [[maybe_unused]]) const {
    IF_BAD_COERC(AProgram) out << '(';
    WithCoercionLevel(0)(v.ListExpr_);
    IF_BAD_COERC(AProgram) out << ')';
}

void ContextFreePrettyPrinter::operator()(const ListExpr& v) const {
    IF_BAD_COERC(ListExpr) out << '(';
    if (v.empty()) {
    } else {
        ContextFreePrettyPrinter itemprinter = WithCoercionLevel(0);
        auto last = std::prev(v.cend());
        for (auto i = v.cbegin(); i != last; ++i) {
            itemprinter(*i);
            out << ";";
            NewLine();
        }
        itemprinter(*last);
    }
    IF_BAD_COERC(ListExpr) out << ')';
}

#define ContextFreePrettyPrinterSHL(type)                                                        \
    const ContextFreePrettyPrinter& operator<<(const ContextFreePrettyPrinter& p, const type& v) \
    { p(v); return p; }

ContextFreePrettyPrinterSHL(Ident);
ContextFreePrettyPrinterSHL(Char);
ContextFreePrettyPrinterSHL(Double);
ContextFreePrettyPrinterSHL(Integer);
ContextFreePrettyPrinterSHL(String);
ContextFreePrettyPrinterSHL(SpecialBegin);
ContextFreePrettyPrinterSHL(SpecialEnd);
ContextFreePrettyPrinterSHL(Expr);
ContextFreePrettyPrinterSHL(Variable);
ContextFreePrettyPrinterSHL(Application);
ContextFreePrettyPrinterSHL(Abstraction);
ContextFreePrettyPrinterSHL(StringExpr);
ContextFreePrettyPrinterSHL(IntegerExpr);
ContextFreePrettyPrinterSHL(DoubleExpr);
ContextFreePrettyPrinterSHL(CharExpr);
ContextFreePrettyPrinterSHL(SpecialExpr);
ContextFreePrettyPrinterSHL(Program);
ContextFreePrettyPrinterSHL(AProgram);
ContextFreePrettyPrinterSHL(ListExpr);

const ContextFreePrettyPrinter& operator<<(const ContextFreePrettyPrinter& p, std::string_view v) {
    p.out << v;
    return p;
}

}  // namespace LC
