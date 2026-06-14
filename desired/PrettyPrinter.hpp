// The default pretty printer does not suit all languages.
// See PrettyPrinter.cpp for details.

#pragma once
#include <iostream>
#include <string_view>
#include "Absyn.hpp"

namespace LC {

// for use with std::visit, |PatternMatch{...}, and operator<<
class PrettyPrinter {
    std::ostream& out;
    unsigned int indent;
    int coercionLevel;
    void NewLine(unsigned int indent) const;

    friend const PrettyPrinter& operator<<(const PrettyPrinter&,
                                           std::string_view);

public:
    PrettyPrinter(std::ostream&, unsigned int indent = 0,
                  int coercionLevel = 0);
    PrettyPrinter Indented(unsigned int plusIndent = 4,
                           int coercionLevel = 0) const;
    PrettyPrinter Dedented(unsigned int minusIndent = 4,
                           int coercionLevel = 0) const;
    PrettyPrinter WithCoercionLevel(int level) const;
    void operator()(const Program&) const;
    void operator()(const ListExpr&) const;
    void operator()(const Expr&) const;
    void operator()(const AProgram&) const;
    void operator()(const Abstraction&) const;
    void operator()(const Application&) const;
    void operator()(const Variable&) const;
    void operator()(const Ident&) const;
    void operator()(const String&) const;
    void operator()(const Integer&) const;
    void operator()(const Double&) const;
    void operator()(const Char&) const;
    void operator()(const StringExpr&) const;
    void operator()(const IntegerExpr&) const;
    void operator()(const DoubleExpr&) const;
    void operator()(const CharExpr&) const;
    void NewLine() const;
};

const PrettyPrinter& operator<<(const PrettyPrinter&, const Program&);
const PrettyPrinter& operator<<(const PrettyPrinter&, const ListExpr&);
const PrettyPrinter& operator<<(const PrettyPrinter&, const Expr&);
const PrettyPrinter& operator<<(const PrettyPrinter&, const AProgram&);
const PrettyPrinter& operator<<(const PrettyPrinter&, const Abstraction&);
const PrettyPrinter& operator<<(const PrettyPrinter&, const Application&);
const PrettyPrinter& operator<<(const PrettyPrinter&, const Variable&);
const PrettyPrinter& operator<<(const PrettyPrinter&, const Ident&);
const PrettyPrinter& operator<<(const PrettyPrinter&, const String&);
const PrettyPrinter& operator<<(const PrettyPrinter&, const Integer&);
const PrettyPrinter& operator<<(const PrettyPrinter&, const Char&);
const PrettyPrinter& operator<<(const PrettyPrinter&, const Double&);
const PrettyPrinter& operator<<(const PrettyPrinter&, const StringExpr&);
const PrettyPrinter& operator<<(const PrettyPrinter&, const IntegerExpr&);
const PrettyPrinter& operator<<(const PrettyPrinter&, const CharExpr&);
const PrettyPrinter& operator<<(const PrettyPrinter&, const DoubleExpr&);
const PrettyPrinter& operator<<(const PrettyPrinter&, std::string_view);

}
