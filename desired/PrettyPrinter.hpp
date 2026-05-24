#pragma once
#include <iostream>
#include <string_view>
#include "Absyn.hpp"

namespace LC {

// for use with std::visit and operator<<
class PrettyPrinter {
    std::ostream& out;
    int coercionLevel;
    unsigned int indent;
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
const PrettyPrinter& operator<<(const PrettyPrinter&, std::string_view);

}
