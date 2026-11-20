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

#pragma once
#include <cctype>
#include <ostream>
#include <string_view>

#include "ast.hpp"
#include "printer-common.hpp"

namespace ast::printing {

template <lib::concepts::is_ast TAst>
class ClassicPrettyPrinter {
    std::ostream& out;
    unsigned int tabSize;
    unsigned int indentInTabs = 0;
    int coercionLevel = 0;
    bool needSpace = false;
    bool needNewline = false;
    ClassicPrettyPrinter& SimplePut(std::string_view);
    static void PutLinebreak(std::ostream& out, unsigned int indent) {
        out << '\n' << std::string(indent, ' ');
    }

public:
    ClassicPrettyPrinter(std::ostream&, unsigned int tabSize = 4);

    ClassicPrettyPrinter(const ClassicPrettyPrinter&) = default;
    ClassicPrettyPrinter(ClassicPrettyPrinter&&) = default;
    ClassicPrettyPrinter& operator=(const ClassicPrettyPrinter&) = delete;
    ClassicPrettyPrinter& operator=(ClassicPrettyPrinter&&) = delete;
    ~ClassicPrettyPrinter() = default;

    unsigned int TabSize() const { return tabSize; }
    // In tabs
    unsigned int IndentationLevel() const { return indentInTabs; }
    int CoercionLevel() const { return coercionLevel; }

    ClassicPrettyPrinter& SetTabSize(unsigned int tabSize);
    ClassicPrettyPrinter& SetCoercionLevel(int lvl);
    ClassicPrettyPrinter& Indent(unsigned int tabs = 1);
    ClassicPrettyPrinter& Dedent(unsigned int tabs = 1);
    ClassicPrettyPrinter& SetIndent(unsigned int tabs);
    ClassicPrettyPrinter& SkipSpaceHere();
    ClassicPrettyPrinter& NeedSpaceHere();
    ClassicPrettyPrinter& OnNewLine();
    ClassicPrettyPrinter& FlushWhitespace();

    ClassicPrettyPrinter& PutToken(std::string_view);
    ClassicPrettyPrinter& PutCharLiteral(int32_t);
    ClassicPrettyPrinter& PutStringLiteral(std::string_view);
    ClassicPrettyPrinter& PutDoubleLiteral(double);
    ClassicPrettyPrinter& PutIntegerLiteral(long);

    ClassicPrettyPrinter& Put(const Char&);
    ClassicPrettyPrinter& Put(const Double&);
    ClassicPrettyPrinter& Put(const Integer&);
    ClassicPrettyPrinter& PutString(const String&);
    ClassicPrettyPrinter& PutSpecialBegin(const SpecialBegin&);
    ClassicPrettyPrinter& PutSpecialEnd(const SpecialEnd&);
    ClassicPrettyPrinter& Put(TAst& ast, const Expr<TAst>&,
                              int coercLvl);  // Returns with current lvl
    ClassicPrettyPrinter& Put(TAst& ast, const Program<TAst>&,
                              int coercLvl);  // Returns with current lvl
    ClassicPrettyPrinter& Put(const Variable<TAst>&);
    ClassicPrettyPrinter& Put(const Application<TAst>&);
    ClassicPrettyPrinter& Put(const Abstraction<TAst>&);
    ClassicPrettyPrinter& Put(const StringExpr<TAst>&);
    ClassicPrettyPrinter& Put(const IntegerExpr<TAst>&);
    ClassicPrettyPrinter& Put(const DoubleExpr<TAst>&);
    ClassicPrettyPrinter& Put(const CharExpr<TAst>&);
    ClassicPrettyPrinter& Put(const SpecialExpr<TAst>&);
    ClassicPrettyPrinter& Put(const AProgram<TAst>&);
    ClassicPrettyPrinter& Put(const ListExpr<TAst>&);
    ClassicPrettyPrinter& operator()(const std::string& s) {
        PutToken(s);
        return *this;
    }
    ClassicPrettyPrinter& operator()(const Char& c) {
        Put(c);
        return *this;
    }
    ClassicPrettyPrinter& operator()(const Double& d) {
        Put(d);
        return *this;
    }
    ClassicPrettyPrinter& operator()(const Integer& i) {
        Put(i);
        return *this;
    }
    ClassicPrettyPrinter& operator()(TAst& ast, const Expr<TAst>& v) {
        Put(ast, v, 0);
        return *this;
    }
    ClassicPrettyPrinter& operator()(const Variable<TAst>& v) {
        Put(v);
        return *this;
    }
    ClassicPrettyPrinter& operator()(const Application<TAst>& v) {
        Put(v);
        return *this;
    }
    ClassicPrettyPrinter& operator()(const Abstraction<TAst>& v) {
        Put(v);
        return *this;
    }
    ClassicPrettyPrinter& operator()(const StringExpr<TAst>& v) {
        Put(v);
        return *this;
    }
    ClassicPrettyPrinter& operator()(const IntegerExpr<TAst>& v) {
        Put(v);
        return *this;
    }
    ClassicPrettyPrinter& operator()(const DoubleExpr<TAst>& v) {
        Put(v);
        return *this;
    }
    ClassicPrettyPrinter& operator()(const CharExpr<TAst>& v) {
        Put(v);
        return *this;
    }
    ClassicPrettyPrinter& operator()(const SpecialExpr<TAst>& v) {
        Put(v);
        return *this;
    }
    ClassicPrettyPrinter& operator()(TAst& ast, const Program<TAst>& v) {
        Put(ast, v, 0);
        return *this;
    }
    ClassicPrettyPrinter& operator()(const AProgram<TAst>& v) {
        Put(v);
        return *this;
    }
    ClassicPrettyPrinter& operator()(const ListExpr<TAst>& v) {
        Put(v);
        return *this;
    }
    ClassicPrettyPrinter& operator<<(const std::string& v) {
        PutToken(v);
        return *this;
    }
    ClassicPrettyPrinter& operator<<(std::string_view v) {
        PutToken(v);
        return *this;
    }
    ClassicPrettyPrinter& operator<<(const Char& v) {
        Put(v);
        return *this;
    }
    ClassicPrettyPrinter& operator<<(const Double& v) {
        Put(v);
        return *this;
    }
    ClassicPrettyPrinter& operator<<(const Integer& v) {
        Put(v);
        return *this;
    }
    ClassicPrettyPrinter& operator<<(const Variable<TAst>& v) {
        Put(v);
        return *this;
    }
    ClassicPrettyPrinter& operator<<(const Application<TAst>& v) {
        Put(v);
        return *this;
    }
    ClassicPrettyPrinter& operator<<(const Abstraction<TAst>& v) {
        Put(v);
        return *this;
    }
    ClassicPrettyPrinter& operator<<(const StringExpr<TAst>& v) {
        Put(v);
        return *this;
    }
    ClassicPrettyPrinter& operator<<(const IntegerExpr<TAst>& v) {
        Put(v);
        return *this;
    }
    ClassicPrettyPrinter& operator<<(const DoubleExpr<TAst>& v) {
        Put(v);
        return *this;
    }
    ClassicPrettyPrinter& operator<<(const CharExpr<TAst>& v) {
        Put(v);
        return *this;
    }
    ClassicPrettyPrinter& operator<<(const SpecialExpr<TAst>& v) {
        Put(v);
        return *this;
    }
    ClassicPrettyPrinter& operator<<(const AProgram<TAst>& v) {
        Put(v);
        return *this;
    }
    ClassicPrettyPrinter& operator<<(const ListExpr<TAst>& v) {
        Put(v);
        return *this;
    }
};

#define BNFC_ASTLIB_PRINT_IF_BAD_COERC(output)                                                \
    if (lib::traits::lbnf_coercion_level_v<std::remove_cvref_t<decltype(v)>> < coercionLevel) \
        PutToken(output);

template <lib::concepts::is_ast TAst>
ClassicPrettyPrinter<TAst>& ClassicPrettyPrinter<TAst>::SimplePut(std::string_view s) {
    FlushWhitespace();
    out << s;
    needSpace = true;
    return *this;
}

template <lib::concepts::is_ast TAst>
ClassicPrettyPrinter<TAst>& ClassicPrettyPrinter<TAst>::FlushWhitespace() {
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

template <lib::concepts::is_ast TAst>
ClassicPrettyPrinter<TAst>& ClassicPrettyPrinter<TAst>::SetTabSize(unsigned int tabSize) {
    this->tabSize = tabSize;
    return *this;
}

template <lib::concepts::is_ast TAst>
ClassicPrettyPrinter<TAst>& ClassicPrettyPrinter<TAst>::Indent(unsigned int tabs) {
    indentInTabs += tabs;
    return *this;
}

template <lib::concepts::is_ast TAst>
ClassicPrettyPrinter<TAst>& ClassicPrettyPrinter<TAst>::Dedent(unsigned int tabs) {
    if (tabs > indentInTabs)
        indentInTabs = 0;
    else
        indentInTabs -= tabs;
    return *this;
}

template <lib::concepts::is_ast TAst>
ClassicPrettyPrinter<TAst>& ClassicPrettyPrinter<TAst>::SetIndent(unsigned int tabs) {
    indentInTabs = tabs;
    return *this;
}

template <lib::concepts::is_ast TAst>
ClassicPrettyPrinter<TAst>& ClassicPrettyPrinter<TAst>::SkipSpaceHere() {
    needSpace = false;
    return *this;
}

template <lib::concepts::is_ast TAst>
ClassicPrettyPrinter<TAst>& ClassicPrettyPrinter<TAst>::NeedSpaceHere() {
    needSpace = true;
    return *this;
}

template <lib::concepts::is_ast TAst>
ClassicPrettyPrinter<TAst>& ClassicPrettyPrinter<TAst>::PutToken(std::string_view s) {
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

template <lib::concepts::is_ast TAst>
ClassicPrettyPrinter<TAst>& ClassicPrettyPrinter<TAst>::PutCharLiteral(int32_t ch) {
    FlushWhitespace();
    print_escaped_char(out, ch);
    return NeedSpaceHere();
}

template <lib::concepts::is_ast TAst>
ClassicPrettyPrinter<TAst>& ClassicPrettyPrinter<TAst>::PutStringLiteral(std::string_view s) {
    FlushWhitespace();
    print_escaped_string(out, s);
    return NeedSpaceHere();
}

template <lib::concepts::is_ast TAst>
ClassicPrettyPrinter<TAst>& ClassicPrettyPrinter<TAst>::PutDoubleLiteral(double v) {
    FlushWhitespace();
    print_double(out, v);
    return NeedSpaceHere();
}

template <lib::concepts::is_ast TAst>
ClassicPrettyPrinter<TAst>& ClassicPrettyPrinter<TAst>::PutIntegerLiteral(long v) {
    FlushWhitespace();
    print_double(out, v);
    return NeedSpaceHere();
}

template <lib::concepts::is_ast TAst>
ClassicPrettyPrinter<TAst>& ClassicPrettyPrinter<TAst>::OnNewLine() {
    needSpace = false;
    needNewline = true;
    return *this;
}

template <lib::concepts::is_ast TAst>
ClassicPrettyPrinter<TAst>& ClassicPrettyPrinter<TAst>::Put(const Char& v) {
    return PutCharLiteral(v);
}

template <lib::concepts::is_ast TAst>
ClassicPrettyPrinter<TAst>& ClassicPrettyPrinter<TAst>::Put(const Double& v) {
    return PutDoubleLiteral(v);
}

template <lib::concepts::is_ast TAst>
ClassicPrettyPrinter<TAst>& ClassicPrettyPrinter<TAst>::Put(const Integer& v) {
    return PutIntegerLiteral(v);
}

template <lib::concepts::is_ast TAst>
ClassicPrettyPrinter<TAst>& ClassicPrettyPrinter<TAst>::PutString(const String& v) {
    return PutStringLiteral(v);
}

template <lib::concepts::is_ast TAst>
ClassicPrettyPrinter<TAst>& ClassicPrettyPrinter<TAst>::PutSpecialBegin(const SpecialBegin& v) {
    return PutToken(v);
}

template <lib::concepts::is_ast TAst>
ClassicPrettyPrinter<TAst>& ClassicPrettyPrinter<TAst>::PutSpecialEnd(const SpecialEnd& v) {
    return PutToken(v);
}

template <lib::concepts::is_ast TAst>
ClassicPrettyPrinter<TAst>& ClassicPrettyPrinter<TAst>::Put(TAst& ast, const Expr<TAst>& v,
                                                            int coercLvl) {
    int cur = coercionLevel;
    coercionLevel = coercLvl;
    ast.visit(v)(*this);
    coercionLevel = cur;
    return *this;
}

template <lib::concepts::is_ast TAst>
ClassicPrettyPrinter<TAst>& ClassicPrettyPrinter<TAst>::Put(TAst& ast, const Program<TAst>& v,
                                                            int coercLvl) {
    int cur = coercionLevel;
    coercionLevel = coercLvl;
    ast.visit(v)(*this);
    coercionLevel = cur;
    return *this;
}

template <lib::concepts::is_ast TAst>
ClassicPrettyPrinter<TAst>& ClassicPrettyPrinter<TAst>::Put(const Variable<TAst>& v) {
    BNFC_ASTLIB_PRINT_IF_BAD_COERC("(");
    PutToken(v.Ident_());
    BNFC_ASTLIB_PRINT_IF_BAD_COERC(")");
    return *this;
}

template <lib::concepts::is_ast TAst>
ClassicPrettyPrinter<TAst>& ClassicPrettyPrinter<TAst>::Put(const Application<TAst>& v) {
    BNFC_ASTLIB_PRINT_IF_BAD_COERC("(");
    Put(v.ast(), v.Expr_1(), 1);
    Put(v.ast(), v.Expr_2(), 2);
    BNFC_ASTLIB_PRINT_IF_BAD_COERC(")");
    return *this;
}

template <lib::concepts::is_ast TAst>
ClassicPrettyPrinter<TAst>& ClassicPrettyPrinter<TAst>::Put(const Abstraction<TAst>& v) {
    BNFC_ASTLIB_PRINT_IF_BAD_COERC("(");
    PutToken("l");
    PutToken(v.Ident_());
    PutToken(".");
    Put(v.ast(), v.Expr_(), 0);
    BNFC_ASTLIB_PRINT_IF_BAD_COERC(")");
    return *this;
}

template <lib::concepts::is_ast TAst>
ClassicPrettyPrinter<TAst>& ClassicPrettyPrinter<TAst>::Put(const StringExpr<TAst>& v) {
    BNFC_ASTLIB_PRINT_IF_BAD_COERC("(");
    PutToken("str:");
    PutString(v.String_());
    BNFC_ASTLIB_PRINT_IF_BAD_COERC(")");
    return *this;
}

template <lib::concepts::is_ast TAst>
ClassicPrettyPrinter<TAst>& ClassicPrettyPrinter<TAst>::Put(const IntegerExpr<TAst>& v) {
    BNFC_ASTLIB_PRINT_IF_BAD_COERC("(");
    PutToken("int:");
    Put(v.Integer_());
    BNFC_ASTLIB_PRINT_IF_BAD_COERC(")");
    return *this;
}

template <lib::concepts::is_ast TAst>
ClassicPrettyPrinter<TAst>& ClassicPrettyPrinter<TAst>::Put(const DoubleExpr<TAst>& v) {
    BNFC_ASTLIB_PRINT_IF_BAD_COERC("(");
    PutToken("dbl:");
    Put(v.Double_());
    BNFC_ASTLIB_PRINT_IF_BAD_COERC(")");
    return *this;
}

template <lib::concepts::is_ast TAst>
ClassicPrettyPrinter<TAst>& ClassicPrettyPrinter<TAst>::Put(const CharExpr<TAst>& v) {
    BNFC_ASTLIB_PRINT_IF_BAD_COERC("(");
    PutToken("chr:");
    Put(v.Char_());
    BNFC_ASTLIB_PRINT_IF_BAD_COERC(")");
    return *this;
}

template <lib::concepts::is_ast TAst>
ClassicPrettyPrinter<TAst>& ClassicPrettyPrinter<TAst>::Put(const SpecialExpr<TAst>& v) {
    BNFC_ASTLIB_PRINT_IF_BAD_COERC("(");
    PutToken(v.SpecialBegin_());
    Put(v.ast(), v.Expr_(), 0);
    PutToken(v.SpecialEnd_());
    BNFC_ASTLIB_PRINT_IF_BAD_COERC(")");
    return *this;
}

template <lib::concepts::is_ast TAst>
ClassicPrettyPrinter<TAst>& ClassicPrettyPrinter<TAst>::Put(const AProgram<TAst>& v) {
    BNFC_ASTLIB_PRINT_IF_BAD_COERC("(");
    Put(v.ListExpr_().get(v.ast()));
    BNFC_ASTLIB_PRINT_IF_BAD_COERC(")");
    return *this;
}

template <lib::concepts::is_ast TAst>
ClassicPrettyPrinter<TAst>& ClassicPrettyPrinter<TAst>::Put(const ListExpr<TAst>& v) {
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

}  // namespace ast::printing
