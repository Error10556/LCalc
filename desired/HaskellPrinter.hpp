#pragma once
#include <iostream>
#include <string_view>
#include "Absyn.hpp"

namespace LC {

class HaskellPrinter {
    std::ostream& out;
    bool inExpression;
    friend const HaskellPrinter& operator<<(const HaskellPrinter&,
                                           std::string_view);
    HaskellPrinter(std::ostream& out, bool inExpression);
    HaskellPrinter PrintConstructorArg() const;

public:
    explicit HaskellPrinter(std::ostream& out);
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

const HaskellPrinter& operator<<(const HaskellPrinter&, const Ident&);
const HaskellPrinter& operator<<(const HaskellPrinter&, const Char&);
const HaskellPrinter& operator<<(const HaskellPrinter&, const Double&);
const HaskellPrinter& operator<<(const HaskellPrinter&, const Integer&);
const HaskellPrinter& operator<<(const HaskellPrinter&, const String&);
const HaskellPrinter& operator<<(const HaskellPrinter&, const SpecialBegin&);
const HaskellPrinter& operator<<(const HaskellPrinter&, const SpecialEnd&);
const HaskellPrinter& operator<<(const HaskellPrinter&, const Expr&);
const HaskellPrinter& operator<<(const HaskellPrinter&, const Variable&);
const HaskellPrinter& operator<<(const HaskellPrinter&, const Application&);
const HaskellPrinter& operator<<(const HaskellPrinter&, const Abstraction&);
const HaskellPrinter& operator<<(const HaskellPrinter&, const StringExpr&);
const HaskellPrinter& operator<<(const HaskellPrinter&, const IntegerExpr&);
const HaskellPrinter& operator<<(const HaskellPrinter&, const DoubleExpr&);
const HaskellPrinter& operator<<(const HaskellPrinter&, const CharExpr&);
const HaskellPrinter& operator<<(const HaskellPrinter&, const SpecialExpr&);
const HaskellPrinter& operator<<(const HaskellPrinter&, const Program&);
const HaskellPrinter& operator<<(const HaskellPrinter&, const AProgram&);
const HaskellPrinter& operator<<(const HaskellPrinter&, const ListExpr&);
const HaskellPrinter& operator<<(const HaskellPrinter&, std::string_view);

}  // namespace LC
