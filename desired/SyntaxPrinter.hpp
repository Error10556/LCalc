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
                                           const std::string_view&);
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
};

const SyntaxPrinter& operator<<(const SyntaxPrinter&, const Program&);
const SyntaxPrinter& operator<<(const SyntaxPrinter&, const ListExpr&);
const SyntaxPrinter& operator<<(const SyntaxPrinter&, const Expr&);
const SyntaxPrinter& operator<<(const SyntaxPrinter&, const AProgram&);
const SyntaxPrinter& operator<<(const SyntaxPrinter&, const Abstraction&);
const SyntaxPrinter& operator<<(const SyntaxPrinter&, const Application&);
const SyntaxPrinter& operator<<(const SyntaxPrinter&, const Variable&);
const SyntaxPrinter& operator<<(const SyntaxPrinter&, const std::string_view&);

}
