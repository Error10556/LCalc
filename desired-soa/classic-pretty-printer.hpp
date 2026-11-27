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
#include <ostream>
#include <string_view>

#include "ast.hpp"

namespace ast::printing::LC {

using namespace ast::LC;

class ClassicPrettyPrinter {
    std::ostream& out;
    unsigned int tabSize;
    unsigned int indentInTabs = 0;
    int coercionLevel = 0;
    bool needSpace = false;
    bool needNewline = false;
    ClassicPrettyPrinter& SimplePut(std::string_view);

public:
    using TAst = lib::ASTRef<>;

    ClassicPrettyPrinter(std::ostream& out, unsigned int tabSize = 4)
        : out{out}, tabSize{tabSize} {}

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

}  // namespace ast::printing::LC
