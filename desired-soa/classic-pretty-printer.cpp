#include "classic-pretty-printer.hpp"

#include <cctype>

#include "printer-common.hpp"

namespace ast::printing::LC {

namespace {
inline void PutLinebreak(std::ostream& out, unsigned int indent) {
    out << '\n' << std::string(indent, ' ');
}
}  // namespace

#define BNFC_ASTLIB_PRINT_IF_BAD_COERC(output)                                                \
    if (lib::traits::lbnf_coercion_level_v<std::remove_cvref_t<decltype(v)>> < coercionLevel) \
        PutToken(output);

ClassicPrettyPrinter& ClassicPrettyPrinter::SimplePut(std::string_view s) {
    FlushWhitespace();
    out << s;
    needSpace = true;
    return *this;
}

ClassicPrettyPrinter& ClassicPrettyPrinter::FlushWhitespace() {
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

ClassicPrettyPrinter& ClassicPrettyPrinter::SetTabSize(unsigned int tabSize) {
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

ClassicPrettyPrinter& ClassicPrettyPrinter::SetIndent(unsigned int tabs) {
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
    if (std::isspace(s.front())) SkipSpaceHere();
    SimplePut(s);
    if (std::isspace(s.back())) SkipSpaceHere();
    return *this;
}

ClassicPrettyPrinter& ClassicPrettyPrinter::PutCharLiteral(int32_t ch) {
    FlushWhitespace();
    print_escaped_char(out, ch);
    return NeedSpaceHere();
}

ClassicPrettyPrinter& ClassicPrettyPrinter::PutStringLiteral(std::string_view s) {
    FlushWhitespace();
    print_escaped_string(out, s);
    return NeedSpaceHere();
}

ClassicPrettyPrinter& ClassicPrettyPrinter::PutDoubleLiteral(double v) {
    FlushWhitespace();
    print_double(out, v);
    return NeedSpaceHere();
}

ClassicPrettyPrinter& ClassicPrettyPrinter::PutIntegerLiteral(long v) {
    FlushWhitespace();
    out << v;
    return NeedSpaceHere();
}

ClassicPrettyPrinter& ClassicPrettyPrinter::OnNewLine() {
    needSpace = false;
    needNewline = true;
    return *this;
}

ClassicPrettyPrinter& ClassicPrettyPrinter::Put(const Char& v) { return PutCharLiteral(v); }

ClassicPrettyPrinter& ClassicPrettyPrinter::Put(const Double& v) { return PutDoubleLiteral(v); }

ClassicPrettyPrinter& ClassicPrettyPrinter::Put(const Integer& v) { return PutIntegerLiteral(v); }

ClassicPrettyPrinter& ClassicPrettyPrinter::PutString(const String& v) {
    return PutStringLiteral(v);
}

ClassicPrettyPrinter& ClassicPrettyPrinter::PutSpecialBegin(const SpecialBegin& v) {
    return PutToken(v);
}

ClassicPrettyPrinter& ClassicPrettyPrinter::PutSpecialEnd(const SpecialEnd& v) {
    return PutToken(v);
}

ClassicPrettyPrinter& ClassicPrettyPrinter::Put(TAst& ast, const Expr<TAst>& v, int coercLvl) {
    int cur = coercionLevel;
    coercionLevel = coercLvl;
    ast.visit(v)(*this);
    coercionLevel = cur;
    return *this;
}

ClassicPrettyPrinter& ClassicPrettyPrinter::Put(TAst& ast, const Program<TAst>& v, int coercLvl) {
    int cur = coercionLevel;
    coercionLevel = coercLvl;
    ast.visit(v)(*this);
    coercionLevel = cur;
    return *this;
}

ClassicPrettyPrinter& ClassicPrettyPrinter::Put(const Variable<TAst>& v) {
    BNFC_ASTLIB_PRINT_IF_BAD_COERC("(");
    PutToken(v.Ident_());
    BNFC_ASTLIB_PRINT_IF_BAD_COERC(")");
    return *this;
}

ClassicPrettyPrinter& ClassicPrettyPrinter::Put(const Application<TAst>& v) {
    BNFC_ASTLIB_PRINT_IF_BAD_COERC("(");
    Put(v.ast(), v.Expr_1(), 1);
    Put(v.ast(), v.Expr_2(), 2);
    BNFC_ASTLIB_PRINT_IF_BAD_COERC(")");
    return *this;
}

ClassicPrettyPrinter& ClassicPrettyPrinter::Put(const Abstraction<TAst>& v) {
    BNFC_ASTLIB_PRINT_IF_BAD_COERC("(");
    PutToken("l");
    PutToken(v.Ident_());
    PutToken(".");
    Put(v.ast(), v.Expr_(), 0);
    BNFC_ASTLIB_PRINT_IF_BAD_COERC(")");
    return *this;
}

ClassicPrettyPrinter& ClassicPrettyPrinter::Put(const StringExpr<TAst>& v) {
    BNFC_ASTLIB_PRINT_IF_BAD_COERC("(");
    PutToken("str:");
    PutString(v.String_());
    BNFC_ASTLIB_PRINT_IF_BAD_COERC(")");
    return *this;
}

ClassicPrettyPrinter& ClassicPrettyPrinter::Put(const IntegerExpr<TAst>& v) {
    BNFC_ASTLIB_PRINT_IF_BAD_COERC("(");
    PutToken("int:");
    Put(v.Integer_());
    BNFC_ASTLIB_PRINT_IF_BAD_COERC(")");
    return *this;
}

ClassicPrettyPrinter& ClassicPrettyPrinter::Put(const DoubleExpr<TAst>& v) {
    BNFC_ASTLIB_PRINT_IF_BAD_COERC("(");
    PutToken("dbl:");
    Put(v.Double_());
    BNFC_ASTLIB_PRINT_IF_BAD_COERC(")");
    return *this;
}

ClassicPrettyPrinter& ClassicPrettyPrinter::Put(const CharExpr<TAst>& v) {
    BNFC_ASTLIB_PRINT_IF_BAD_COERC("(");
    PutToken("chr:");
    Put(v.Char_());
    BNFC_ASTLIB_PRINT_IF_BAD_COERC(")");
    return *this;
}

ClassicPrettyPrinter& ClassicPrettyPrinter::Put(const SpecialExpr<TAst>& v) {
    BNFC_ASTLIB_PRINT_IF_BAD_COERC("(");
    PutToken(v.SpecialBegin_());
    Put(v.ast(), v.Expr_(), 0);
    PutToken(v.SpecialEnd_());
    BNFC_ASTLIB_PRINT_IF_BAD_COERC(")");
    return *this;
}

ClassicPrettyPrinter& ClassicPrettyPrinter::Put(const AProgram<TAst>& v) {
    BNFC_ASTLIB_PRINT_IF_BAD_COERC("(");
    Put(v.ListExpr_().get(v.ast()));
    BNFC_ASTLIB_PRINT_IF_BAD_COERC(")");
    return *this;
}

ClassicPrettyPrinter& ClassicPrettyPrinter::Put(const ListExpr<TAst>& v) {
    if (v.empty()) {
    } else {
        auto last = std::prev(v.cend());
        for (auto i = v.begin(); i != last; ++i) {
            Put(v.ast(), *i, 0);
            PutToken(";");
        }
        Put(v.ast(), *last, 0);
    }
    return *this;
}

}  // namespace ast::printing::LC
