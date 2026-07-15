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
    - Left curly braces '{' are always printed on a separate line and increase
      the indentation level. The brace itself is _not_ indented.
    - Right curly braces '}' are always printed on a separate line and decrease
      the indentation level. The brace itself is _also_ de-dented.
    - Semicolons ';' cause a line break after themselves.

Precedence is always resolved by enclosing a term in parentheses '()'.

*/

#include "ClassicPrettyPrinter.hpp"

#include <string>
#include "PrinterCommon.hpp"

namespace LC {

#define IF_BAD_COERC \
    if (reflection::CoercionLevel<std::decay_t<decltype(v)>> < coercionLevel)

static void PutLinebreak(std::ostream& out, unsigned int indent) {
    out << '\n' << std::string(indent, ' ');
}

ClassicPrettyPrinter::ClassicPrettyPrinter(
    std::ostream& out, unsigned int tabSize)
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

unsigned int ClassicPrettyPrinter::TabSize() const {
    return tabSize;
}

unsigned int ClassicPrettyPrinter::IndentationLevel() const {
    return indentInTabs;
}

int ClassicPrettyPrinter::CoercionLevel() const {
    return coercionLevel;
}

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
        case ' ': case '\r': case '\n': case '\t': case '\v':
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
                OnNewLine()
                    .SimplePut(s)
                    .Indent()
                    .OnNewLine();
                break;
            case '}':
                OnNewLine()
                    .Dedent()
                    .SimplePut(s)
                    .OnNewLine();
                break;
            case ';':
                SkipSpaceHere()
                    .SimplePut(s)
                    .OnNewLine();
                break;
            case ',': case ')': case ']':
                SkipSpaceHere()
                    .SimplePut(s)
                    .NeedSpaceHere();
                break;
            case '(': case '[':
                SimplePut(s)
                    .SkipSpaceHere();
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

ClassicPrettyPrinter& ClassicPrettyPrinter::PutCharLiteral(int32_t ch) {
    FlushHere();
    PrintEscapedChar(out, ch);
    return NeedSpaceHere();
}

ClassicPrettyPrinter& ClassicPrettyPrinter::PutStringLiteral(
        std::string_view s) {
    FlushHere();
    PrintEscapedString(out, s);
    return NeedSpaceHere();
}

ClassicPrettyPrinter& ClassicPrettyPrinter::PutDoubleLiteral(double v) {
    FlushHere();
    PrintDouble(out, v);
    return NeedSpaceHere();
}

ClassicPrettyPrinter& ClassicPrettyPrinter::PutIntegerLiteral(long v) {
    FlushHere();
    PrintDouble(out, v);
    return NeedSpaceHere();
}

ClassicPrettyPrinter& ClassicPrettyPrinter::OnNewLine() {
    needSpace = false;
    needNewline = true;
    return *this;
}

ClassicPrettyPrinter& ClassicPrettyPrinter::Put(const Ident& v) {
    return PutToken(v.Value);
}

ClassicPrettyPrinter& ClassicPrettyPrinter::Put(const Char& v) {
    return PutCharLiteral(v.Value);
}

ClassicPrettyPrinter& ClassicPrettyPrinter::Put(const Double& v) {
    return PutDoubleLiteral(v.Value);
}

ClassicPrettyPrinter& ClassicPrettyPrinter::Put(const Integer& v) {
    return PutIntegerLiteral(v.Value);
}

ClassicPrettyPrinter& ClassicPrettyPrinter::Put(const String& v) {
    return PutStringLiteral(v.Value);
}

ClassicPrettyPrinter& ClassicPrettyPrinter::Put(const SpecialBegin& v) {
    return PutToken(v.Value);
}

ClassicPrettyPrinter& ClassicPrettyPrinter::Put(const SpecialEnd& v) {
    return PutToken(v.Value);
}

ClassicPrettyPrinter& ClassicPrettyPrinter::Put(const Expr& v, int coercLvl) {
    int cur = coercionLevel;
    coercionLevel = coercLvl;
    swl::visit(*this, v);
    coercionLevel = cur;
    return *this;
}

ClassicPrettyPrinter& ClassicPrettyPrinter::Put(const Program& v, int coercLvl) {
    int cur = coercionLevel;
    coercionLevel = coercLvl;
    swl::visit(*this, v);
    coercionLevel = cur;
    return *this;
}

ClassicPrettyPrinter& ClassicPrettyPrinter::Put(const Variable& v) {
    IF_BAD_COERC PutToken("(");
    Put(v.Ident_);
    IF_BAD_COERC PutToken(")");
    return *this;
}

ClassicPrettyPrinter& ClassicPrettyPrinter::Put(const Application& v) {
    IF_BAD_COERC PutToken("(");
    Put(*v.Expr_1, 1);
    Put(*v.Expr_2, 2);
    IF_BAD_COERC PutToken(")");
    return *this;
}

ClassicPrettyPrinter& ClassicPrettyPrinter::Put(const Abstraction& v) {
    IF_BAD_COERC PutToken("(");
    PutToken("l");
    Put(v.Ident_);
    PutToken(".");
    Put(*v.Expr_, 0);
    IF_BAD_COERC PutToken(")");
    return *this;
}

ClassicPrettyPrinter& ClassicPrettyPrinter::Put(const StringExpr& v) {
    IF_BAD_COERC PutToken("(");
    PutToken("str:");
    Put(v.String_);
    IF_BAD_COERC PutToken(")");
    return *this;
}

ClassicPrettyPrinter& ClassicPrettyPrinter::Put(const IntegerExpr& v) {
    IF_BAD_COERC PutToken("(");
    PutToken("int:");
    Put(v.Integer_);
    IF_BAD_COERC PutToken(")");
    return *this;
}

ClassicPrettyPrinter& ClassicPrettyPrinter::Put(const DoubleExpr& v) {
    IF_BAD_COERC PutToken("(");
    PutToken("dbl:");
    Put(v.Double_);
    IF_BAD_COERC PutToken(")");
    return *this;
}

ClassicPrettyPrinter& ClassicPrettyPrinter::Put(const CharExpr& v) {
    IF_BAD_COERC PutToken("(");
    PutToken("chr:");
    Put(v.Char_);
    IF_BAD_COERC PutToken(")");
    return *this;
}

ClassicPrettyPrinter& ClassicPrettyPrinter::Put(const SpecialExpr& v) {
    IF_BAD_COERC PutToken("(");
    Put(v.SpecialBegin_);
    Put(*v.Expr_, 0);
    Put(v.SpecialEnd_);
    IF_BAD_COERC PutToken(")");
    return *this;
}

ClassicPrettyPrinter& ClassicPrettyPrinter::Put(const AProgram& v) {
    IF_BAD_COERC PutToken("(");
    Put(v.ListExpr_);
    IF_BAD_COERC PutToken(")");
    return *this;
}

ClassicPrettyPrinter& ClassicPrettyPrinter::Put(const ListExpr& v) {
    if (v.empty()) {
    } else {
        auto last = std::prev(v.cend());
        for (auto i = v.cbegin(); i != last; ++i) {
            Put(*i, 0);
            PutToken(";");
        }
        Put(*last, 0);
    }
    return *this;
}

void ClassicPrettyPrinter::operator()(const Ident& v) {
    Put(v);
}

void ClassicPrettyPrinter::operator()(const Char& v) {
    Put(v);
}

void ClassicPrettyPrinter::operator()(const Double& v) {
    Put(v);
}

void ClassicPrettyPrinter::operator()(const Integer& v) {
    Put(v);
}

void ClassicPrettyPrinter::operator()(const String& v) {
    Put(v);
}

void ClassicPrettyPrinter::operator()(const SpecialBegin& v) {
    Put(v);
}

void ClassicPrettyPrinter::operator()(const SpecialEnd& v) {
    Put(v);
}

void ClassicPrettyPrinter::operator()(const Expr& v) {
    Put(v, 0);
}

void ClassicPrettyPrinter::operator()(const Variable& v) {
    Put(v);
}

void ClassicPrettyPrinter::operator()(const Application& v) {
    Put(v);
}

void ClassicPrettyPrinter::operator()(const Abstraction& v) {
    Put(v);
}

void ClassicPrettyPrinter::operator()(const StringExpr& v) {
    Put(v);
}

void ClassicPrettyPrinter::operator()(const IntegerExpr& v) {
    Put(v);
}

void ClassicPrettyPrinter::operator()(const DoubleExpr& v) {
    Put(v);
}

void ClassicPrettyPrinter::operator()(const CharExpr& v) {
    Put(v);
}

void ClassicPrettyPrinter::operator()(const SpecialExpr& v) {
    Put(v);
}

void ClassicPrettyPrinter::operator()(const Program& v) {
    Put(v, 0);
}

void ClassicPrettyPrinter::operator()(const AProgram& v) {
    Put(v);
}


void ClassicPrettyPrinter::operator()(const ListExpr& v) {
    Put(v);
}

#define ClassicPrettyPrinterSHL(type)                               \
    const ClassicPrettyPrinter& operator<<(ClassicPrettyPrinter& p, \
                                           const type& v) {         \
        return p.Put(v);                                            \
    }

#define ClassicPrettyPrinterSHL0(type)                              \
    const ClassicPrettyPrinter& operator<<(ClassicPrettyPrinter& p, \
                                           const type& v) {         \
        return p.Put(v, 0);                                         \
    }

ClassicPrettyPrinterSHL(Ident);
ClassicPrettyPrinterSHL(Char);
ClassicPrettyPrinterSHL(Double);
ClassicPrettyPrinterSHL(Integer);
ClassicPrettyPrinterSHL(String);
ClassicPrettyPrinterSHL(SpecialBegin);
ClassicPrettyPrinterSHL(SpecialEnd);
ClassicPrettyPrinterSHL0(Expr);
ClassicPrettyPrinterSHL(Variable);
ClassicPrettyPrinterSHL(Application);
ClassicPrettyPrinterSHL(Abstraction);
ClassicPrettyPrinterSHL(StringExpr);
ClassicPrettyPrinterSHL(IntegerExpr);
ClassicPrettyPrinterSHL(DoubleExpr);
ClassicPrettyPrinterSHL(CharExpr);
ClassicPrettyPrinterSHL(SpecialExpr);
ClassicPrettyPrinterSHL0(Program);
ClassicPrettyPrinterSHL(AProgram);
ClassicPrettyPrinterSHL(ListExpr);

const ClassicPrettyPrinter& operator<<(ClassicPrettyPrinter& p,
                                       std::string_view v) {
    return p.PutToken(v);
}

}  // namespace LC
