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
    ClassicPrettyPrinter& FlushHere();
    ClassicPrettyPrinter& SimplePut(std::string_view);

public:
#define BNFC_ASTLIB_PRINT_IF_BAD_COERC(output)                                              \
    if (lib::traits::lbnf_coercion_level<std::remove_cvref_t<decltype(v)>> < coercionLevel) \
        out << output;
    ClassicPrettyPrinter(std::ostream&, unsigned int tabSize = 4);
    unsigned int TabSize() const;
    // In tabs
    unsigned int IndentationLevel() const;
    int CoercionLevel() const;

    ClassicPrettyPrinter& WithTabSize(unsigned int tabSize);
    ClassicPrettyPrinter& WithCoercionLevel(int lvl);
    ClassicPrettyPrinter& Indent(unsigned int tabs = 1);
    ClassicPrettyPrinter& Dedent(unsigned int tabs = 1);
    ClassicPrettyPrinter& WithIndent(unsigned int tabs);
    ClassicPrettyPrinter& SkipSpaceHere();
    ClassicPrettyPrinter& NeedSpaceHere();

    ClassicPrettyPrinter& PutToken(std::string_view);
    ClassicPrettyPrinter& PutCharLiteral(int32_t);
    ClassicPrettyPrinter& PutStringLiteral(std::string_view);
    ClassicPrettyPrinter& PutDoubleLiteral(double);
    ClassicPrettyPrinter& PutIntegerLiteral(long);
    ClassicPrettyPrinter& OnNewLine();

    ClassicPrettyPrinter& PutIdent(const Ident&);
    ClassicPrettyPrinter& Put(const Char&);
    ClassicPrettyPrinter& Put(const Double&);
    ClassicPrettyPrinter& Put(const Integer&);
    ClassicPrettyPrinter& PutString(const String&);
    ClassicPrettyPrinter& PutSpecialBegin(const SpecialBegin&);
    ClassicPrettyPrinter& PutSpecialEnd(const SpecialEnd&);
    ClassicPrettyPrinter& Put(TAst& ast, const Expr<TAst>&, int coercLvl);     // Returns with current lvl
    ClassicPrettyPrinter& Put(TAst& ast, const Program<TAst>&, int coercLvl);  // Returns with current lvl
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
    void operator()(const Ident&);
    void operator()(const Char&);
    void operator()(const Double&);
    void operator()(const Integer&);
    void operator()(const String&);
    void operator()(const SpecialBegin&);
    void operator()(const SpecialEnd&);
    void operator()(const Expr&);
    void operator()(const Variable&);
    void operator()(const Application&);
    void operator()(const Abstraction&);
    void operator()(const StringExpr&);
    void operator()(const IntegerExpr&);
    void operator()(const DoubleExpr&);
    void operator()(const CharExpr&);
    void operator()(const SpecialExpr&);
    void operator()(const Program&);
    void operator()(const AProgram&);
    void operator()(const ListExpr&);
};

const ClassicPrettyPrinter& operator<<(const ClassicPrettyPrinter&, const Ident&);
const ClassicPrettyPrinter& operator<<(const ClassicPrettyPrinter&, const Char&);
const ClassicPrettyPrinter& operator<<(const ClassicPrettyPrinter&, const Double&);
const ClassicPrettyPrinter& operator<<(const ClassicPrettyPrinter&, const Integer&);
const ClassicPrettyPrinter& operator<<(const ClassicPrettyPrinter&, const String&);
const ClassicPrettyPrinter& operator<<(const ClassicPrettyPrinter&, const SpecialBegin&);
const ClassicPrettyPrinter& operator<<(const ClassicPrettyPrinter&, const SpecialEnd&);
const ClassicPrettyPrinter& operator<<(const ClassicPrettyPrinter&, const Expr&);
const ClassicPrettyPrinter& operator<<(const ClassicPrettyPrinter&, const Variable&);
const ClassicPrettyPrinter& operator<<(const ClassicPrettyPrinter&, const Application&);
const ClassicPrettyPrinter& operator<<(const ClassicPrettyPrinter&, const Abstraction&);
const ClassicPrettyPrinter& operator<<(const ClassicPrettyPrinter&, const StringExpr&);
const ClassicPrettyPrinter& operator<<(const ClassicPrettyPrinter&, const IntegerExpr&);
const ClassicPrettyPrinter& operator<<(const ClassicPrettyPrinter&, const DoubleExpr&);
const ClassicPrettyPrinter& operator<<(const ClassicPrettyPrinter&, const CharExpr&);
const ClassicPrettyPrinter& operator<<(const ClassicPrettyPrinter&, const SpecialExpr&);
const ClassicPrettyPrinter& operator<<(const ClassicPrettyPrinter&, const Program&);
const ClassicPrettyPrinter& operator<<(const ClassicPrettyPrinter&, const AProgram&);
const ClassicPrettyPrinter& operator<<(const ClassicPrettyPrinter&, const ListExpr&);

// Calls PutToken
const ClassicPrettyPrinter& operator<<(const ClassicPrettyPrinter&, std::string_view);

}  // namespace ast::printing
