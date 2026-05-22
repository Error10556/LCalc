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
class Printer {
    std::ostream& out;
    int coercionLevel;
    friend Printer& operator<<(Printer&, const std::string_view&);
public:
    Printer(std::ostream&, int coercionLevel = 0);
    void operator()(const Program&) const;
    void operator()(const ListExpr&) const;
    void operator()(const Expr&) const;
    void operator()(const AProgram&) const;
    void operator()(const Abstraction&) const;
    void operator()(const Application&) const;
    void operator()(const Variable&) const;
};

Printer& operator<<(Printer&, const Program&);
Printer& operator<<(Printer&, const ListExpr&);
Printer& operator<<(Printer&, const Expr&);
Printer& operator<<(Printer&, const AProgram&);
Printer& operator<<(Printer&, const Abstraction&);
Printer& operator<<(Printer&, const Application&);
Printer& operator<<(Printer&, const Variable&);
Printer& operator<<(Printer&, const std::string_view&);

}
