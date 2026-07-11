// The default context-free pretty printer does not suit all languages and is
// meant to be modified. See ContextFreePrettyPrinter.cpp for details.

#pragma once
#include <iostream>
#include <string_view>
#include "Absyn.hpp"

namespace LC {

class ContextFreePrettyPrinter {
    std::ostream& out;
    unsigned int indent;
    int coercionLevel;
    void NewLine(unsigned int indent) const;

    friend const ContextFreePrettyPrinter& operator<<(
        const ContextFreePrettyPrinter&, std::string_view);

public:
    ContextFreePrettyPrinter(std::ostream&, unsigned int indent = 0,
                  int coercionLevel = 0);
    ContextFreePrettyPrinter Indented(unsigned int plusIndent = 4,
                           int coercionLevel = 0) const;
    ContextFreePrettyPrinter Dedented(unsigned int minusIndent = 4,
                           int coercionLevel = 0) const;
    ContextFreePrettyPrinter WithCoercionLevel(int level) const;
    void NewLine() const;
    void operator()(const Ident&) const;
    void operator()(const Char&) const;
    void operator()(const Double&) const;
    void operator()(const Integer&) const;
    void operator()(const String&) const;
    void operator()(const SpecialBegin&) const;
    void operator()(const SpecialEnd&) const;
    void operator()(const Expr&) const;
    void operator()(const Variable&) const;
    void operator()(const Application&) const;
    void operator()(const Abstraction&) const;
    void operator()(const StringExpr&) const;
    void operator()(const IntegerExpr&) const;
    void operator()(const DoubleExpr&) const;
    void operator()(const CharExpr&) const;
    void operator()(const SpecialExpr&) const;
    void operator()(const Program&) const;
    void operator()(const AProgram&) const;
    void operator()(const ListExpr&) const;
};

const ContextFreePrettyPrinter& operator<<(const ContextFreePrettyPrinter&, const Ident&);
const ContextFreePrettyPrinter& operator<<(const ContextFreePrettyPrinter&, const Char&);
const ContextFreePrettyPrinter& operator<<(const ContextFreePrettyPrinter&, const Double&);
const ContextFreePrettyPrinter& operator<<(const ContextFreePrettyPrinter&, const Integer&);
const ContextFreePrettyPrinter& operator<<(const ContextFreePrettyPrinter&, const String&);
const ContextFreePrettyPrinter& operator<<(const ContextFreePrettyPrinter&, const SpecialBegin&);
const ContextFreePrettyPrinter& operator<<(const ContextFreePrettyPrinter&, const SpecialEnd&);
const ContextFreePrettyPrinter& operator<<(const ContextFreePrettyPrinter&, const Expr&);
const ContextFreePrettyPrinter& operator<<(const ContextFreePrettyPrinter&, const Variable&);
const ContextFreePrettyPrinter& operator<<(const ContextFreePrettyPrinter&, const Application&);
const ContextFreePrettyPrinter& operator<<(const ContextFreePrettyPrinter&, const Abstraction&);
const ContextFreePrettyPrinter& operator<<(const ContextFreePrettyPrinter&, const StringExpr&);
const ContextFreePrettyPrinter& operator<<(const ContextFreePrettyPrinter&, const IntegerExpr&);
const ContextFreePrettyPrinter& operator<<(const ContextFreePrettyPrinter&, const DoubleExpr&);
const ContextFreePrettyPrinter& operator<<(const ContextFreePrettyPrinter&, const CharExpr&);
const ContextFreePrettyPrinter& operator<<(const ContextFreePrettyPrinter&, const SpecialExpr&);
const ContextFreePrettyPrinter& operator<<(const ContextFreePrettyPrinter&, const Program&);
const ContextFreePrettyPrinter& operator<<(const ContextFreePrettyPrinter&, const AProgram&);
const ContextFreePrettyPrinter& operator<<(const ContextFreePrettyPrinter&, const ListExpr&);
const ContextFreePrettyPrinter& operator<<(const ContextFreePrettyPrinter&, std::string_view);

}  // namespace LC
