#include "HaskellPrinter.hpp"

#include "PrinterCommon.hpp"

namespace LC {

HaskellPrinter::HaskellPrinter(std::ostream& out, bool inExpression)
    : out(out)
    , inExpression(inExpression) {}

HaskellPrinter::HaskellPrinter(std::ostream& out)
    : HaskellPrinter(out, false) {}

HaskellPrinter HaskellPrinter::PrintConstructorArg() const {
    return {out, true};
}

void HaskellPrinter::operator()(const Ident& v) const {
    PrintEscapedString(out, v.Value);
}

void HaskellPrinter::operator()(const Char& v) const {
    PrintEscapedChar(out, v.Value);
}

void HaskellPrinter::operator()(const Double& v) const {
    PrintDouble(out, v.Value);
}

void HaskellPrinter::operator()(const Integer& v) const {
    out << v.Value;
}

void HaskellPrinter::operator()(const String& v) const {
    PrintEscapedString(out, v.Value);
}

void HaskellPrinter::operator()(const SpecialBegin& v) const {
    PrintEscapedString(out, v.Value);
}

void HaskellPrinter::operator()(const SpecialEnd& v) const {
    PrintEscapedString(out, v.Value);
}

void HaskellPrinter::operator()(const Expr& v) const {
    std::visit(*this, v);
}

void HaskellPrinter::operator()(const Variable& v) const {
    if (inExpression) out << '(';
    out << "Variable ";
    const HaskellPrinter printField = PrintConstructorArg();
    printField(v.Ident_);
    if (inExpression) out << ')';
}

void HaskellPrinter::operator()(const Application& v) const {
    if (inExpression) out << '(';
    out << "Application ";
    const HaskellPrinter printField = PrintConstructorArg();
    printField(*v.Expr_1);
    out << ' ';
    printField(*v.Expr_2);
    if (inExpression) out << ')';
}

void HaskellPrinter::operator()(const Abstraction& v) const {
    if (inExpression) out << '(';
    out << "Abstraction ";
    const HaskellPrinter printField = PrintConstructorArg();
    printField(v.Ident_);
    out << ' ';
    printField(*v.Expr_);
    if (inExpression) out << ')';
}

void HaskellPrinter::operator()(const StringExpr& v) const {
    if (inExpression) out << '(';
    out << "StringExpr ";
    const HaskellPrinter printField = PrintConstructorArg();
    printField(v.String_);
    if (inExpression) out << ')';
}

void HaskellPrinter::operator()(const IntegerExpr& v) const {
    if (inExpression) out << '(';
    out << "IntegerExpr ";
    const HaskellPrinter printField = PrintConstructorArg();
    printField(v.Integer_);
    if (inExpression) out << ')';
}

void HaskellPrinter::operator()(const DoubleExpr& v) const {
    if (inExpression) out << '(';
    out << "DoubleExpr ";
    const HaskellPrinter printField = PrintConstructorArg();
    printField(v.Double_);
    if (inExpression) out << ')';
}

void HaskellPrinter::operator()(const CharExpr& v) const {
    if (inExpression) out << '(';
    out << "CharExpr ";
    const HaskellPrinter printField = PrintConstructorArg();
    printField(v.Char_);
    if (inExpression) out << ')';
}

void HaskellPrinter::operator()(const SpecialExpr& v) const {
    if (inExpression) out << '(';
    out << "SpecialExpr ";
    const HaskellPrinter printField = PrintConstructorArg();
    printField(v.SpecialBegin_);
    out << ' ';
    printField(*v.Expr_);
    out << ' ';
    printField(v.SpecialEnd_);
    if (inExpression) out << ')';
}

void HaskellPrinter::operator()(const Program& v) const {
    std::visit(*this, v);
}

void HaskellPrinter::operator()(const AProgram& v) const {
    if (inExpression) out << '(';
    out << "AProgram ";
    const HaskellPrinter printField = PrintConstructorArg();
    printField(v.ListExpr_);
    if (inExpression) out << ')';
}

void HaskellPrinter::operator()(const ListExpr& v) const {
    out << '[';
    if (!v.empty()) {
        const HaskellPrinter printItem(out);
        auto last = std::prev(v.cend());
        for (auto i = v.cbegin(); i != last; ++i) {
            printItem(*i);
            out << ',';
        }
        printItem(*last);
    }
    out << ']';
}

#define HaskellPrinterSHL(type)                                              \
    const HaskellPrinter& operator<<(const HaskellPrinter& p, const type& v) \
    { p(v); return p; }

HaskellPrinterSHL(Ident);
HaskellPrinterSHL(Char);
HaskellPrinterSHL(Double);
HaskellPrinterSHL(Integer);
HaskellPrinterSHL(String);
HaskellPrinterSHL(SpecialBegin);
HaskellPrinterSHL(SpecialEnd);
HaskellPrinterSHL(Expr);
HaskellPrinterSHL(Variable);
HaskellPrinterSHL(Application);
HaskellPrinterSHL(Abstraction);
HaskellPrinterSHL(StringExpr);
HaskellPrinterSHL(IntegerExpr);
HaskellPrinterSHL(DoubleExpr);
HaskellPrinterSHL(CharExpr);
HaskellPrinterSHL(SpecialExpr);
HaskellPrinterSHL(Program);
HaskellPrinterSHL(AProgram);
HaskellPrinterSHL(ListExpr);

const HaskellPrinter& operator<<(const HaskellPrinter& p, std::string_view v) {
    p.out << v;
    return p;
}

}  // namespace LC
