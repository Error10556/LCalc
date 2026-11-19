#pragma once

#include <iosfwd>
#include <string_view>

#include "ast.hpp"

namespace ast::printing {

class ClassicPrettyPrinter {
    std::ostream& out;
    unsigned int tabSize;
    unsigned int indentInTabs = 0;
    int coercionLevel = 0;
    bool needSpace = false;
    bool needNewline = false;

    ClassicPrettyPrinter& FlushHere();
    ClassicPrettyPrinter& SimplePut(std::string_view);

    template <class Print>
    ClassicPrettyPrinter& PutParenthesized(bool parens, Print print) {
        if (parens) PutToken("(");
        print();
        if (parens) PutToken(")");
        return *this;
    }

    template <class TAst>
    ClassicPrettyPrinter& PutAtLevel(const Variable<TAst>& v, int level) {
        return PutParenthesized(level > 2, [&] { PutToken(v.Ident_()); });
    }

    template <class TAst>
    ClassicPrettyPrinter& PutAtLevel(const Application<TAst>& v, int level) {
        return PutParenthesized(level > 1, [&] {
            Put(v.ast(), v.Expr_1(), 1);
            Put(v.ast(), v.Expr_2(), 2);
        });
    }

    template <class TAst>
    ClassicPrettyPrinter& PutAtLevel(const Abstraction<TAst>& v, int level) {
        return PutParenthesized(level > 0, [&] {
            PutToken("l").PutToken(v.Ident_()).PutToken(".");
            Put(v.ast(), v.Expr_(), 0);
        });
    }

    template <class TAst>
    ClassicPrettyPrinter& PutAtLevel(const StringExpr<TAst>& v, int level) {
        return PutParenthesized(level > 0, [&] { PutToken("str:").PutStringLiteral(v.String_()); });
    }

    template <class TAst>
    ClassicPrettyPrinter& PutAtLevel(const IntegerExpr<TAst>& v, int level) {
        return PutParenthesized(level > 0,
                                [&] { PutToken("int:").PutIntegerLiteral(v.Integer_()); });
    }

    template <class TAst>
    ClassicPrettyPrinter& PutAtLevel(const DoubleExpr<TAst>& v, int level) {
        return PutParenthesized(level > 0, [&] { PutToken("dbl:").PutDoubleLiteral(v.Double_()); });
    }

    template <class TAst>
    ClassicPrettyPrinter& PutAtLevel(const CharExpr<TAst>& v, int level) {
        return PutParenthesized(level > 0, [&] { PutToken("chr:").PutCharLiteral(v.Char_()); });
    }

    template <class TAst>
    ClassicPrettyPrinter& PutAtLevel(const SpecialExpr<TAst>& v, int level) {
        return PutParenthesized(level > 0, [&] {
            PutToken(v.SpecialBegin_());
            Put(v.ast(), v.Expr_(), 0);
            PutToken(v.SpecialEnd_());
        });
    }

    template <class TAst>
    ClassicPrettyPrinter& PutAtLevel(const AProgram<TAst>& v, int level) {
        return PutParenthesized(level > 0, [&] { Put(v.ListExpr_().get(v.ast())); });
    }

public:
    ClassicPrettyPrinter(std::ostream&, unsigned int tabSize = 4);
    unsigned int TabSize() const;
    unsigned int IndentationLevel() const;
    int CoercionLevel() const;

    ClassicPrettyPrinter& WithTabSize(unsigned int tabSize);
    ClassicPrettyPrinter& WithCoercionLevel(int level);
    ClassicPrettyPrinter& Indent(unsigned int tabs = 1);
    ClassicPrettyPrinter& Dedent(unsigned int tabs = 1);
    ClassicPrettyPrinter& WithIndent(unsigned int tabs);
    ClassicPrettyPrinter& SkipSpaceHere();
    ClassicPrettyPrinter& NeedSpaceHere();

    ClassicPrettyPrinter& PutToken(std::string_view);
    ClassicPrettyPrinter& PutCharLiteral(Char);
    ClassicPrettyPrinter& PutStringLiteral(std::string_view);
    ClassicPrettyPrinter& PutDoubleLiteral(Double);
    ClassicPrettyPrinter& PutIntegerLiteral(Integer);
    ClassicPrettyPrinter& OnNewLine();

    // String literals share the identifier type. Use PutStringLiteral to quote a value.
    ClassicPrettyPrinter& Put(std::string_view token) { return PutToken(token); }
    ClassicPrettyPrinter& Put(Char v) { return PutCharLiteral(v); }
    ClassicPrettyPrinter& Put(Double v) { return PutDoubleLiteral(v); }
    ClassicPrettyPrinter& Put(Integer v) { return PutIntegerLiteral(v); }

    template <class TAst>
    ClassicPrettyPrinter& Put(TAst& tree, Expr<TAst> expr, int level = 0) {
        expr.visit(tree)([&](const auto& node) { PutAtLevel(node, level); });
        return *this;
    }

    template <class TAst>
    ClassicPrettyPrinter& Put(TAst& tree, Program<TAst> program, int level = 0) {
        return PutAtLevel(program.get(tree), level);
    }

    template <class TAst>
    ClassicPrettyPrinter& Put(lib::Array<TAst, Expr<TAst>> list) {
        const auto size = list.size();
        for (decltype(list.size()) i = 0; i < size; ++i) {
            if (i != 0) PutToken(";");
            Put(list.ast(), static_cast<Expr<TAst>>(list[i]));
        }
        return *this;
    }

    template <lib::concepts::is_ast_label Node>
    ClassicPrettyPrinter& Put(const Node& node) {
        return PutAtLevel(node, coercionLevel);
    }

    template <class T>
    void operator()(const T& value) {
        Put(value);
    }
};

template <class T>
    requires requires(ClassicPrettyPrinter& printer, const T& value) { printer.Put(value); }
ClassicPrettyPrinter& operator<<(ClassicPrettyPrinter& printer, const T& value) {
    return printer.Put(value);
}

}  // namespace ast::printing
