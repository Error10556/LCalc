#include "SyntaxPrinter.hpp"
#include "PrinterCommon.hpp"

namespace LC {

SyntaxPrinter::SyntaxPrinter(const SyntaxPrinter* parent,
                             bool currentIndentIsBranch)
    : out(parent->out),
      currentIndentIsBranch(currentIndentIsBranch),
      maybeParent(parent) {}

SyntaxPrinter::SyntaxPrinter(std::ostream& out)
    : out(out), currentIndentIsBranch(false), maybeParent(nullptr) {}

void SyntaxPrinter::PrintIndentForHeader() const {
    if (!maybeParent) return;
    maybeParent->PrintIndentAsIs();
    out << "+-";
}

void SyntaxPrinter::PrintIndentAsIs() const {
    if (!maybeParent) return;
    maybeParent->PrintIndentAsIs();
    out << (currentIndentIsBranch ? "| " : "  ");
}

void SyntaxPrinter::operator()(const Program& v) const {
    std::visit(*this, v);
}

void SyntaxPrinter::operator()(const ListExpr& v) const {
    PrintIndentForHeader();
    size_t n = v.size();
    out << "ListExpr [" << n << "]\n";
    if (!n) return;
    if (n > 1) {
        SyntaxPrinter nonlast(this, true);
        size_t n1 = n - 1;
        for (size_t i = 0; i < n1; i++)
            std::visit(nonlast, v[i]);
    }
    std::visit(SyntaxPrinter(this, false), v.back());
}

void SyntaxPrinter::operator()(const Expr& v) const {
    std::visit(*this, v);
}

void SyntaxPrinter::operator()(const AProgram& v) const {
    PrintIndentForHeader();
    out << "AProgram\n";
    SyntaxPrinter(this, false)(v.ListExpr_);
}

void SyntaxPrinter::operator()(const Abstraction& v) const {
    PrintIndentForHeader();
    out << "Abstraction\n";
    SyntaxPrinter nonlast(this, true);
    nonlast(v.Ident_);
    SyntaxPrinter(this, false)(*v.Expr_);
}

void SyntaxPrinter::operator()(const Application& v) const {
    PrintIndentForHeader();
    out << "Application\n";
    SyntaxPrinter nonlast(this, true);
    nonlast(*v.Expr_1);
    SyntaxPrinter(this, false)(*v.Expr_2);
}

void SyntaxPrinter::operator()(const Variable& v) const {
    PrintIndentForHeader();
    out << "Variable\n";
    SyntaxPrinter(this, false)(v.Ident_);
}

void SyntaxPrinter::operator()(const Ident& v) const {
    PrintIndentForHeader();
    out << "Ident {" << v.Value << "}\n";
}

void SyntaxPrinter::operator()(const String& v) const {
    PrintIndentForHeader();
    out << "String ";
    PrintEscapedString(out, v.Value);
    out << '\n';
}

void SyntaxPrinter::operator()(const Integer& v) const {
    PrintIndentForHeader();
    out << "Integer " << v.Value << '\n';
}

void SyntaxPrinter::operator()(const Double& v) const {
    PrintIndentForHeader();
    out << "Double ";
    PrintDouble(out, v.Value);
    out << '\n';
}

void SyntaxPrinter::operator()(const Char& v) const {
    PrintIndentForHeader();
    out << "Char ";
    PrintEscapedChar(out, v.Value);
    out << '\n';
}

void SyntaxPrinter::operator()(const StringExpr& v) const {
    PrintIndentForHeader();
    out << "StringExpr\n";
    SyntaxPrinter(this, false)(v.String_);
}

void SyntaxPrinter::operator()(const IntegerExpr& v) const {
    PrintIndentForHeader();
    out << "IntegerExpr\n";
    SyntaxPrinter(this, false)(v.Integer_);
}

void SyntaxPrinter::operator()(const DoubleExpr& v) const {
    PrintIndentForHeader();
    out << "DoubleExpr\n";
    SyntaxPrinter(this, false)(v.Double_);
}

void SyntaxPrinter::operator()(const CharExpr& v) const {
    PrintIndentForHeader();
    out << "CharExpr\n";
    SyntaxPrinter(this, false)(v.Char_);
}

#define SyntaxPrinterSHL(type) \
    const SyntaxPrinter& operator<<(const SyntaxPrinter& p, const type& v) \
    { p(v); return p; }

SyntaxPrinterSHL(Program);
SyntaxPrinterSHL(ListExpr);
SyntaxPrinterSHL(Expr);
SyntaxPrinterSHL(AProgram);
SyntaxPrinterSHL(Abstraction);
SyntaxPrinterSHL(Application);
SyntaxPrinterSHL(Variable);
SyntaxPrinterSHL(Ident);
SyntaxPrinterSHL(String);
SyntaxPrinterSHL(Integer);
SyntaxPrinterSHL(Double);
SyntaxPrinterSHL(Char);
SyntaxPrinterSHL(StringExpr);
SyntaxPrinterSHL(IntegerExpr);
SyntaxPrinterSHL(DoubleExpr);
SyntaxPrinterSHL(CharExpr);

const SyntaxPrinter& operator<<(const SyntaxPrinter& p, std::string_view s) {
    p.out << s;
    return p;
}

}
