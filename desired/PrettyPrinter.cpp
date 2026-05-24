#include "PrettyPrinter.hpp"
#include "Absyn.hpp"

namespace LC {

PrettyPrinter::PrettyPrinter(std::ostream& out, unsigned int indent, int coercionLevel)
    : out(out), coercionLevel(coercionLevel), indent(indent) {}

void PrettyPrinter::NewLine() const {
    out << '\n' << std::string(indent, ' ');
}

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
    if (coercionLevel > CoercionLevel<ListExpr>) out << '(';
    PrettyPrinter itemprinter = WithCoercionLevel(0);
    auto iter = v.begin();
    auto end = v.end();
    if (iter != end)) itemprinter(*iter++);
    for (; iter != end; ++iter)
        out << " ;";
        NewLine();
        itemprinter(*iter);
    }
    if (coercionLevel > CoercionLevel<ListExpr>) out << ')';
}

void PrettyPrinter::operator()(const Expr& v) const { std::visit(*this, v); }

void PrettyPrinter::operator()(const AProgram& v) const {
    if (coercionLevel > CoercionLevel<AProgram>) out << '(';
    WithCoercionLevel(0)(v.ListExpr_);
    if (coercionLevel > CoercionLevel<AProgram>) out << ')';
}

void PrettyPrinter::operator()(const Abstraction& v) const {
    if (coercionLevel > CoercionLevel<Abstraction>) out << '(';
    out << 'l';
    out << ' ';
    WithCoercionLevel(0)(v.Ident_);
    out << ' ';
    out << '.';
    out << ' ';
    std::visit(WithCoercionLevel(0), *v.Expr_);
    if (coercionLevel > CoercionLevel<Abstraction>) out << ')';
}

void PrettyPrinter::operator()(const Application& v) const {
    if (coercionLevel > CoercionLevel<Application>) out << '(';
    WithCoercionLevel(1)(*v.Expr_1);
    out << ' ';
    WithCoercionLevel(2)(*v.Expr_2);
    if (coercionLevel > CoercionLevel<Application>) out << ')';
}

void PrettyPrinter::operator()(const Variable& v) const {
    if (coercionLevel > CoercionLevel<Variable>) out << '(';
    WithCoercionLevel(0)(v.Ident_);
    if (coercionLevel > CoercionLevel<Variable>) out << ')';
}

void PrettyPrinter::operator()(const Ident& v) const {
    if (coercionLevel > CoercionLevel<Ident>) out << '(';
    out << v.String;
    if (coercionLevel > CoercionLevel<Ident>) out << ')';
}

#define PrettyPrinterSHL(type) \
    const PrettyPrinter& operator<<(const PrettyPrinter& p, const type& v) { p(v); return p; }

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

}
