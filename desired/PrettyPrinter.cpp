/**** Disclaimer ****
 * The default PrettyPrinter implementation makes a number of assumptions about
 * the target language. Namely:
 * * All tokens are separated from each other by a space, except for commas ',',
 *   which are only separated from the right, and brackets '[]' or
 *   parentheses '()', which are not separated from the enclosed text.
 * * Curly braces '{}' (and only those) enclose an indented block. The
 * indentation equals 4 spaces. The left curly brace causes one line break
 * _after_ itself, the right one causes one _before_ itself. The right brace is
 * not indented.
 * * Semicolons ';' cause a line break after themselves.
 * * Precedence is always resolved by enclosing a term in parentheses '()'.
 *
 * Since the above limitations likely make the pretty-printed code look far from
 * pretty, you are encouraged to patch this file with your own implementations
 * of some methods.
 */

#include "PrettyPrinter.hpp"

#include "Absyn.hpp"

namespace LC {

PrettyPrinter::PrettyPrinter(std::ostream& out, unsigned int indent,
                             int coercionLevel)
    : out(out), indent(indent), coercionLevel(coercionLevel) {}

void PrettyPrinter::NewLine(unsigned int indent) const {
    out << '\n' << std::string(indent, ' ');
}

void PrettyPrinter::NewLine() const { NewLine(indent); }

PrettyPrinter PrettyPrinter::WithCoercionLevel(int level) const {
    return {out, indent, level};
}

PrettyPrinter PrettyPrinter::Indented(unsigned int plusIndent,
                                      int coercionLevel) const {
    return {out, indent + plusIndent, coercionLevel};
}

PrettyPrinter PrettyPrinter::Dedented(unsigned int minusIndent,
                                      int coercionLevel) const {
    return {out, minusIndent > indent ? 0 : indent - minusIndent,
            coercionLevel};
}

void PrettyPrinter::operator()(const Program& v) const { std::visit(*this, v); }

void PrettyPrinter::operator()(const ListExpr& v) const {
    if (coercionLevel > reflection::CoercionLevel<ListExpr>) out << '(';
    PrettyPrinter itemprinter = WithCoercionLevel(0);
    auto iter = v.begin();
    auto end = v.end();
    if (iter != end) itemprinter(*iter++);
    for (; iter != end; ++iter) {
        out << " ;";
        NewLine();
        itemprinter(*iter);
    }
    if (coercionLevel > reflection::CoercionLevel<ListExpr>) out << ')';
}

void PrettyPrinter::operator()(const Expr& v) const { std::visit(*this, v); }

void PrettyPrinter::operator()(const AProgram& v) const {
    if (coercionLevel > reflection::CoercionLevel<AProgram>) out << '(';
    WithCoercionLevel(0)(v.ListExpr_);
    if (coercionLevel > reflection::CoercionLevel<AProgram>) out << ')';
}

void PrettyPrinter::operator()(const Abstraction& v) const {
    if (coercionLevel > reflection::CoercionLevel<Abstraction>) out << '(';
    out << 'l';
    out << ' ';
    WithCoercionLevel(0)(v.Ident_);
    out << ' ';
    out << '.';
    out << ' ';
    std::visit(WithCoercionLevel(0), *v.Expr_);
    if (coercionLevel > reflection::CoercionLevel<Abstraction>) out << ')';
}

void PrettyPrinter::operator()(const Application& v) const {
    if (coercionLevel > reflection::CoercionLevel<Application>) out << '(';
    WithCoercionLevel(1)(*v.Expr_1);
    out << ' ';
    WithCoercionLevel(2)(*v.Expr_2);
    if (coercionLevel > reflection::CoercionLevel<Application>) out << ')';
}

void PrettyPrinter::operator()(const Variable& v) const {
    if (coercionLevel > reflection::CoercionLevel<Variable>) out << '(';
    WithCoercionLevel(0)(v.Ident_);
    if (coercionLevel > reflection::CoercionLevel<Variable>) out << ')';
}

void PrettyPrinter::operator()(const Ident& v) const {
    if (coercionLevel > reflection::CoercionLevel<Ident>) out << '(';
    out << v.String;
    if (coercionLevel > reflection::CoercionLevel<Ident>) out << ')';
}

#define PrettyPrinterSHL(type)                                               \
    const PrettyPrinter& operator<<(const PrettyPrinter& p, const type& v) { \
        p(v);                                                                \
        return p;                                                            \
    }

PrettyPrinterSHL(Program);
PrettyPrinterSHL(ListExpr);
PrettyPrinterSHL(Expr);
PrettyPrinterSHL(AProgram);
PrettyPrinterSHL(Abstraction);
PrettyPrinterSHL(Application);
PrettyPrinterSHL(Variable);

const PrettyPrinter& operator<<(const PrettyPrinter& p, std::string_view v) {
    p.out << v;
    return p;
}

}  // namespace LC
