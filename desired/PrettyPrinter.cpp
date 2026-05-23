#include "PrettyPrinter.hpp"
#include "Absyn.hpp"

namespace LC {

void Print(std::ostream& out, const Program& v, int coercionLevel) {
    std::visit(PrettyPrinter(out, coercionLevel), v);
}

void Print(std::ostream& out, const ListExpr& v, int coercionLevel) {
    if (coercionLevel > CoercionLevel<ListExpr>) out << '(';
    bool first = true;
    for (const auto& item : v) {
        [[unlikely]]
        if (first)
            first = false;
        else
            out << " ; ";
        PrettyPrinter(out, 0)(item);
    }
    if (coercionLevel > CoercionLevel<ListExpr>) out << ')';
}

void Print(std::ostream& out, const Expr& v, int coercionLevel) {
    std::visit(PrettyPrinter(out, coercionLevel), v);
}

void Print(std::ostream& out, const AProgram& v, int coercionLevel) {
    if (coercionLevel > CoercionLevel<AProgram>) out << '(';
    Print(out, v.ListExpr_, 0);
    if (coercionLevel > CoercionLevel<AProgram>) out << ')';
}

void Print(std::ostream& out, const Abstraction& v, int coercionLevel) {
    if (coercionLevel > CoercionLevel<Abstraction>) out << '(';
    out << 'l';
    out << ' ';
    Print(out, v.Ident_, 0);
    out << ' ';
    out << '.';
    out << ' ';
    Print(out, *v.Expr_, 0);
    if (coercionLevel > CoercionLevel<Abstraction>) out << ')';
}

void Print(std::ostream& out, const Application& v, int coercionLevel) {
    if (coercionLevel > CoercionLevel<Application>) out << '(';
    Print(out, *v.Expr_1, 1);
    out << ' ';
    Print(out, *v.Expr_2, 2);
    if (coercionLevel > CoercionLevel<Application>) out << ')';
}

void Print(std::ostream& out, const Variable& v, int coercionLevel) {
    if (coercionLevel > CoercionLevel<Variable>) out << '(';
    Print(out, v.Ident_, 0);
    if (coercionLevel > CoercionLevel<Variable>) out << ')';
}

void Print(std::ostream& out, const Ident& v, int coercionLevel) {
    if (coercionLevel > CoercionLevel<Variable>) out << '(';
    out << v.String;
    if (coercionLevel > CoercionLevel<Variable>) out << ')';
}

PrettyPrinter::PrettyPrinter(std::ostream& out, int coercionLevel)
    : out(out), coercionLevel(coercionLevel) {}
void PrettyPrinter::operator()(const Program& v) const { std::visit(*this, v); }
void PrettyPrinter::operator()(const ListExpr& v) const { Print(out, v, coercionLevel); }
void PrettyPrinter::operator()(const Expr& v) const { std::visit(*this, v); }
void PrettyPrinter::operator()(const AProgram& v) const { Print(out, v, coercionLevel); }
void PrettyPrinter::operator()(const Abstraction& v) const { Print(out, v, coercionLevel); }
void PrettyPrinter::operator()(const Application& v) const { Print(out, v, coercionLevel); }
void PrettyPrinter::operator()(const Variable& v) const { Print(out, v, coercionLevel); }

#define PrettyPrinterSHL(type) \
    const PrettyPrinter& operator<<(const PrettyPrinter& p, const type& v) { p(v); return p; }

PrettyPrinterSHL(Program);
PrettyPrinterSHL(ListExpr);
PrettyPrinterSHL(Expr);
PrettyPrinterSHL(AProgram);
PrettyPrinterSHL(Abstraction);
PrettyPrinterSHL(Application);
PrettyPrinterSHL(Variable);

PrettyPrinter& operator<<(PrettyPrinter& p, const std::string_view& v) {
    p.out << v;
    return p;
}

}
