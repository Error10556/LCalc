#pragma once
#include <iostream>
#include <string_view>
#include "Absyn.hpp"

namespace LC {

class ClassicPrettyPrinter {
    std::ostream& out;
    unsigned int tabSize;
    unsigned int indentInTabs = 0;
    int coercionLevel = 0;
    bool needSpace = false;
    bool onEmptyLine = true;

public:
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
    ClassicPrettyPrinter& FlushSpaceHere();

    ClassicPrettyPrinter& PutVerbatim(std::string_view);
    ClassicPrettyPrinter& PutToken(std::string_view);
    ClassicPrettyPrinter& PutCharLiteral(int32_t);
    ClassicPrettyPrinter& PutStringLiteral(std::string_view);
    ClassicPrettyPrinter& PutDoubleLiteral(double);
    ClassicPrettyPrinter& PutIntegerLiteral(long);
    ClassicPrettyPrinter& OnNewLine();

    ClassicPrettyPrinter& Put(const Ident&);
    ClassicPrettyPrinter& Put(const Char&);
    ClassicPrettyPrinter& Put(const Double&);
    ClassicPrettyPrinter& Put(const Integer&);
    ClassicPrettyPrinter& Put(const String&);
    ClassicPrettyPrinter& Put(const SpecialBegin&);
    ClassicPrettyPrinter& Put(const SpecialEnd&);
    ClassicPrettyPrinter& Put(const Expr&, int coercLvl);  // Returns with current lvl
    ClassicPrettyPrinter& Put(const Program&, int coercLvl);  // Returns with current lvl
    ClassicPrettyPrinter& Put(const Variable&);
    ClassicPrettyPrinter& Put(const Application&);
    ClassicPrettyPrinter& Put(const Abstraction&);
    ClassicPrettyPrinter& Put(const StringExpr&);
    ClassicPrettyPrinter& Put(const IntegerExpr&);
    ClassicPrettyPrinter& Put(const DoubleExpr&);
    ClassicPrettyPrinter& Put(const CharExpr&);
    ClassicPrettyPrinter& Put(const SpecialExpr&);
    ClassicPrettyPrinter& Put(const AProgram&);
    ClassicPrettyPrinter& Put(const ListExpr&);
    inline void operator()(const Ident&);
    inline void operator()(const Char&);
    inline void operator()(const Double&);
    inline void operator()(const Integer&);
    inline void operator()(const String&);
    inline void operator()(const SpecialBegin&);
    inline void operator()(const SpecialEnd&);
    inline void operator()(const Expr&);
    inline void operator()(const Variable&);
    inline void operator()(const Application&);
    inline void operator()(const Abstraction&);
    inline void operator()(const StringExpr&);
    inline void operator()(const IntegerExpr&);
    inline void operator()(const DoubleExpr&);
    inline void operator()(const CharExpr&);
    inline void operator()(const SpecialExpr&);
    inline void operator()(const Program&);
    inline void operator()(const AProgram&);
    inline void operator()(const ListExpr&);
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

}  // namespace LC
