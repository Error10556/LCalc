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

void SyntaxPrinter::operator()(const Ident& v [[maybe_unused]]) const {
    PrintIndentForHeader();
    out << "Ident {" << v.Value << "}\n";
}

void SyntaxPrinter::operator()(const Char& v [[maybe_unused]]) const {
    PrintIndentForHeader();
    out << "Char ";
    PrintEscapedChar(out, v.Value);
    out << '\n';
}

void SyntaxPrinter::operator()(const Double& v [[maybe_unused]]) const {
    PrintIndentForHeader();
    out << "Double ";
    PrintDouble(out, v.Value);
    out << '\n';
}

void SyntaxPrinter::operator()(const Integer& v [[maybe_unused]]) const {
    PrintIndentForHeader();
    out << "Integer " << v.Value << '\n';
}

void SyntaxPrinter::operator()(const String& v [[maybe_unused]]) const {
    PrintIndentForHeader();
    out << "String ";
    PrintEscapedString(out, v.Value);
    out << '\n';
}

void SyntaxPrinter::operator()(const SpecialBegin& v [[maybe_unused]]) const {
    PrintIndentForHeader();
    out << "SpecialBegin ";
    PrintEscapedString(out, v.Value);
    out << '\n';
}

void SyntaxPrinter::operator()(const SpecialEnd& v [[maybe_unused]]) const {
    PrintIndentForHeader();
    out << "SpecialEnd ";
    PrintEscapedString(out, v.Value);
    out << '\n';
    PrintIndentAsIs();
    out << "  @ " << v.loc << '\n';
}

void SyntaxPrinter::operator()(const Expr& v [[maybe_unused]]) const {
    std::visit(*this, v);
}

void SyntaxPrinter::operator()(const Variable& v [[maybe_unused]]) const {
    PrintIndentForHeader();
    out << "Variable\n";
    PrintIndentAsIs();
    out << "| @ " << v.loc << '\n';
    SyntaxPrinter(this, false)(v.Ident_);
}

void SyntaxPrinter::operator()(const Application& v [[maybe_unused]]) const {
    PrintIndentForHeader();
    out << "Application\n";
    PrintIndentAsIs();
    out << "| @ " << v.loc << '\n';
    SyntaxPrinter nonlast(this, true);
    nonlast(*v.Expr_1);
    SyntaxPrinter(this, false)(*v.Expr_2);
}

void SyntaxPrinter::operator()(const Abstraction& v [[maybe_unused]]) const {
    PrintIndentForHeader();
    out << "Abstraction\n";
    PrintIndentAsIs();
    out << "| @ " << v.loc << '\n';
    SyntaxPrinter nonlast(this, true);
    nonlast(v.Ident_);
    SyntaxPrinter(this, false)(*v.Expr_);
}

void SyntaxPrinter::operator()(const StringExpr& v [[maybe_unused]]) const {
    PrintIndentForHeader();
    out << "StringExpr\n";
    PrintIndentAsIs();
    out << "| @ " << v.loc << '\n';
    SyntaxPrinter(this, false)(v.String_);
}

void SyntaxPrinter::operator()(const IntegerExpr& v [[maybe_unused]]) const {
    PrintIndentForHeader();
    out << "IntegerExpr\n";
    PrintIndentAsIs();
    out << "| @ " << v.loc << '\n';
    SyntaxPrinter(this, false)(v.Integer_);
}

void SyntaxPrinter::operator()(const DoubleExpr& v [[maybe_unused]]) const {
    PrintIndentForHeader();
    out << "DoubleExpr\n";
    PrintIndentAsIs();
    out << "| @ " << v.loc << '\n';
    SyntaxPrinter(this, false)(v.Double_);
}

void SyntaxPrinter::operator()(const CharExpr& v [[maybe_unused]]) const {
    PrintIndentForHeader();
    out << "CharExpr\n";
    PrintIndentAsIs();
    out << "| @ " << v.loc << '\n';
    SyntaxPrinter(this, false)(v.Char_);
}

void SyntaxPrinter::operator()(const SpecialExpr& v [[maybe_unused]]) const {
    PrintIndentForHeader();
    out << "SpecialExpr\n";
    PrintIndentAsIs();
    out << "| @ " << v.loc << '\n';
    SyntaxPrinter nonlast(this, true);
    nonlast(v.SpecialBegin_);
    nonlast(*v.Expr_);
    SyntaxPrinter(this, false)(v.SpecialEnd_);
}

void SyntaxPrinter::operator()(const Program& v [[maybe_unused]]) const {
    std::visit(*this, v);
}

void SyntaxPrinter::operator()(const AProgram& v [[maybe_unused]]) const {
    PrintIndentForHeader();
    out << "AProgram\n";
    PrintIndentAsIs();
    out << "| @ " << v.loc << '\n';
    SyntaxPrinter(this, false)(v.ListExpr_);
}

void SyntaxPrinter::operator()(const ListExpr& v [[maybe_unused]]) const {
    PrintIndentForHeader();
    size_t n = v.size();
    out << "ListExpr [" << n << "]\n";
    PrintIndentAsIs();
    out << (n ? "| " : "  ");
    out << "@ " << v.loc << '\n';
    if (!n) return;
    if (n > 1) {
        SyntaxPrinter nonlast(this, true);
        size_t n1 = n - 1;
        for (size_t i = 0; i < n1; i++)
            nonlast(v[i]);
    }
    SyntaxPrinter(this, false)(v.back());
}

#define SyntaxPrinterSHL(type)                                             \
    const SyntaxPrinter& operator<<(const SyntaxPrinter& p, const type& v) \
    { p(v); return p; }

SyntaxPrinterSHL(Ident);
SyntaxPrinterSHL(Char);
SyntaxPrinterSHL(Double);
SyntaxPrinterSHL(Integer);
SyntaxPrinterSHL(String);
SyntaxPrinterSHL(SpecialBegin);
SyntaxPrinterSHL(SpecialEnd);
SyntaxPrinterSHL(Expr);
SyntaxPrinterSHL(Variable);
SyntaxPrinterSHL(Application);
SyntaxPrinterSHL(Abstraction);
SyntaxPrinterSHL(StringExpr);
SyntaxPrinterSHL(IntegerExpr);
SyntaxPrinterSHL(DoubleExpr);
SyntaxPrinterSHL(CharExpr);
SyntaxPrinterSHL(SpecialExpr);
SyntaxPrinterSHL(Program);
SyntaxPrinterSHL(AProgram);
SyntaxPrinterSHL(ListExpr);

const SyntaxPrinter& operator<<(const SyntaxPrinter& p, std::string_view v) {
    p.out << v;
    return p;
}

}  // namespace LC
