#pragma once
#include <iostream>
#include <string_view>
#include "Absyn.hpp"

namespace LC {

class SyntaxPrinter {
    std::ostream& out;
    bool currentIndentIsBranch;
    const SyntaxPrinter* maybeParent;
    SyntaxPrinter(const SyntaxPrinter* parent, bool currentIndentIsBranch);
    friend const SyntaxPrinter& operator<<(const SyntaxPrinter&,
                                           std::string_view);
    void PrintIndentForHeader() const;
    void PrintIndentAsIs() const;

public:
    SyntaxPrinter(std::ostream& out);
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
};

const SyntaxPrinter& operator<<(const SyntaxPrinter&, const Program&);
const SyntaxPrinter& operator<<(const SyntaxPrinter&, const ListExpr&);
const SyntaxPrinter& operator<<(const SyntaxPrinter&, const Expr&);
const SyntaxPrinter& operator<<(const SyntaxPrinter&, const AProgram&);
const SyntaxPrinter& operator<<(const SyntaxPrinter&, const Abstraction&);
const SyntaxPrinter& operator<<(const SyntaxPrinter&, const Application&);
const SyntaxPrinter& operator<<(const SyntaxPrinter&, const Variable&);
const SyntaxPrinter& operator<<(const SyntaxPrinter&, const Ident&);
const SyntaxPrinter& operator<<(const SyntaxPrinter&, const String&);
const SyntaxPrinter& operator<<(const SyntaxPrinter&, const Integer&);
const SyntaxPrinter& operator<<(const SyntaxPrinter&, const Double&);
const SyntaxPrinter& operator<<(const SyntaxPrinter&, const Char&);
const SyntaxPrinter& operator<<(const SyntaxPrinter&, const StringExpr&);
const SyntaxPrinter& operator<<(const SyntaxPrinter&, const IntegerExpr&);
const SyntaxPrinter& operator<<(const SyntaxPrinter&, const DoubleExpr&);
const SyntaxPrinter& operator<<(const SyntaxPrinter&, const CharExpr&);
const SyntaxPrinter& operator<<(const SyntaxPrinter&, std::string_view);

}
