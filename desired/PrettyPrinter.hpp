#pragma once
#include <iostream>
#include <string_view>
#include "Absyn.hpp"

namespace LC {

void Print(std::ostream&, const Program&, int coercionLevel = 0);
void Print(std::ostream&, const ListExpr&, int coercionLevel = 0);
void Print(std::ostream&, const Expr&, int coercionLevel = 0);
void Print(std::ostream&, const AProgram&, int coercionLevel = 0);
void Print(std::ostream&, const Abstraction&, int coercionLevel = 0);
void Print(std::ostream&, const Application&, int coercionLevel = 0);
void Print(std::ostream&, const Variable&, int coercionLevel = 0);
void Print(std::ostream&, const Ident&, int coercionLevel = 0);

// curried output and coercionLevel
// for use with std::visit and operator<<
class PrettyPrinter {
    std::ostream& out;
    int coercionLevel;
    // unsigned int indent; TODO
    friend PrettyPrinter& operator<<(PrettyPrinter&, const std::string_view&);
public:
    PrettyPrinter(std::ostream&, int coercionLevel = 0);
    void operator()(const Program&) const;
    void operator()(const ListExpr&) const;
    void operator()(const Expr&) const;
    void operator()(const AProgram&) const;
    void operator()(const Abstraction&) const;
    void operator()(const Application&) const;
    void operator()(const Variable&) const;
};

const PrettyPrinter& operator<<(const PrettyPrinter&, const Program&);
const PrettyPrinter& operator<<(const PrettyPrinter&, const ListExpr&);
const PrettyPrinter& operator<<(const PrettyPrinter&, const Expr&);
const PrettyPrinter& operator<<(const PrettyPrinter&, const AProgram&);
const PrettyPrinter& operator<<(const PrettyPrinter&, const Abstraction&);
const PrettyPrinter& operator<<(const PrettyPrinter&, const Application&);
const PrettyPrinter& operator<<(const PrettyPrinter&, const Variable&);
const PrettyPrinter& operator<<(const PrettyPrinter&, const std::string_view&);

}
