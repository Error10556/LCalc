#pragma once
#include <memory>
#include <string>
#include <deque>
#include <variant>

namespace LC {

struct Ident {
public:
    std::string Value;
    Ident() = default;
    Ident(const Ident&) = default;
    Ident(Ident&&) = default;
    Ident& operator=(const Ident&) = default;
    Ident& operator=(Ident&&) = default;
    Ident(const std::string&); /* implicit */
    Ident(std::string&&);
    Ident& operator=(const std::string&);
    Ident& operator=(std::string&&);
};

struct String {
public:
    std::string Value;
    String() = default;
    String(const String&) = default;
    String(String&&) = default;
    String& operator=(const String&) = default;
    String& operator=(String&&) = default;
    String(const std::string&); /* implicit */
    String(std::string&&);
    String& operator=(const std::string&);
    String& operator=(std::string&&);
};

struct Double {
public:
    double Value;
    Double() = default;
    Double(const Double&) = default;
    Double(Double&&) = default;
    Double& operator=(const Double&) = default;
    Double& operator=(Double&&) = default;
    Double(const double&); /* implicit */
    Double& operator=(const double&);
};

struct Integer {
public:
    long Value;
    Integer() = default;
    Integer(const Integer&) = default;
    Integer(Integer&&) = default;
    Integer& operator=(const Integer&) = default;
    Integer& operator=(Integer&&) = default;
    Integer(const long&); /* implicit */
    Integer& operator=(const long&);
};

struct Char {
public:
    int32_t Value;
    Char() = default;
    Char(const Char&) = default;
    Char(Char&&) = default;
    Char& operator=(const Char&) = default;
    Char& operator=(Char&&) = default;
    Char(const int32_t&); /* implicit */
    Char& operator=(const int32_t&);
};

struct SpecialBegin {
public:
    std::string Value;
    SpecialBegin() = default;
    SpecialBegin(const SpecialBegin&) = default;
    SpecialBegin(SpecialBegin&&) = default;
    SpecialBegin& operator=(const SpecialBegin&) = default;
    SpecialBegin& operator=(SpecialBegin&&) = default;
    SpecialBegin(const std::string&); /* implicit */
    SpecialBegin(std::string&&);
    SpecialBegin& operator=(const std::string&);
    SpecialBegin& operator=(std::string&&);
};

struct SpecialEnd {
public:
    std::string Value;
    SpecialEnd() = default;
    SpecialEnd(const SpecialEnd&) = default;
    SpecialEnd(SpecialEnd&&) = default;
    SpecialEnd& operator=(const SpecialEnd&) = default;
    SpecialEnd& operator=(SpecialEnd&&) = default;
    SpecialEnd(const std::string&); /* implicit */
    SpecialEnd(std::string&&);
    SpecialEnd& operator=(const std::string&);
    SpecialEnd& operator=(std::string&&);
};

class AProgram;
using Program = std::variant<AProgram>;

class Abstraction; class Application; class Variable;
class StringExpr; class CharExpr; class IntegerExpr; class DoubleExpr;
class SpecialExpr;
using Expr = std::variant<Abstraction, Application, Variable, StringExpr, CharExpr, IntegerExpr, DoubleExpr, SpecialExpr>;

using ListExpr = std::deque<Expr>;

class AProgram {
public:
    AProgram() = default;
    AProgram(const AProgram&);
    AProgram(AProgram&&) = default;
    AProgram& operator=(const AProgram&);
    AProgram& operator=(AProgram&&) = default;
    AProgram(ListExpr&&);
    ListExpr ListExpr_;
};

class Abstraction {
public:
    Abstraction() = default;
    Abstraction(const Abstraction&);
    Abstraction(Abstraction&&) = default;
    Abstraction& operator=(const Abstraction&);
    Abstraction& operator=(Abstraction&&) = default;
    Abstraction(Ident&&, Expr&&);
    Ident Ident_;
    std::unique_ptr<Expr> Expr_;
};

class Application {
public:
    Application() = default;
    Application(const Application&);
    Application(Application&&) = default;
    Application& operator=(const Application&);
    Application& operator=(Application&&) = default;
    Application(Expr&&, Expr&&);
    std::unique_ptr<Expr> Expr_1, Expr_2;
};

class Variable {
public:
    Variable() = default;
    Variable(const Variable&);
    Variable(Variable&&) = default;
    Variable& operator=(const Variable&);
    Variable& operator=(Variable&&) = default;
    Variable(Ident&&);
    Ident Ident_;
};

class StringExpr {
public:
    StringExpr() = default;
    StringExpr(const StringExpr&);
    StringExpr(StringExpr&&) = default;
    StringExpr& operator=(const StringExpr&);
    StringExpr& operator=(StringExpr&&) = default;
    StringExpr(String&&);
    String String_;
};

class IntegerExpr {
public:
    IntegerExpr() = default;
    IntegerExpr(const IntegerExpr&);
    IntegerExpr(IntegerExpr&&) = default;
    IntegerExpr& operator=(const IntegerExpr&);
    IntegerExpr& operator=(IntegerExpr&&) = default;
    IntegerExpr(Integer&&);
    Integer Integer_;
};

class DoubleExpr {
public:
    DoubleExpr() = default;
    DoubleExpr(const DoubleExpr&);
    DoubleExpr(DoubleExpr&&) = default;
    DoubleExpr& operator=(const DoubleExpr&);
    DoubleExpr& operator=(DoubleExpr&&) = default;
    DoubleExpr(Double&&);
    Double Double_;
};

class CharExpr {
public:
    CharExpr() = default;
    CharExpr(const CharExpr&);
    CharExpr(CharExpr&&) = default;
    CharExpr& operator=(const CharExpr&);
    CharExpr& operator=(CharExpr&&) = default;
    CharExpr(Char&&);
    Char Char_;
};

class SpecialExpr {
public:
    SpecialExpr() = default;
    SpecialExpr(const SpecialExpr&);
    SpecialExpr(SpecialExpr&&) = default;
    SpecialExpr& operator=(const SpecialExpr&);
    SpecialExpr& operator=(SpecialExpr&&) = default;
    SpecialExpr(SpecialBegin&&, Expr&&, SpecialEnd&&);
    SpecialBegin SpecialBegin_;
    std::unique_ptr<Expr> Expr_;
    SpecialEnd SpecialEnd_;
};

namespace reflection {

template <class T>
struct CoercionLevel_t {};

template <class T>
struct SyntaxNodeName_t {};

template<> struct SyntaxNodeName_t<Program> { static constexpr const char* value = "Program"; };
template<> struct SyntaxNodeName_t<Expr> { static constexpr const char* value = "Expr"; };
template<> struct CoercionLevel_t<Ident> { static constexpr int value = 0; };
template<> struct SyntaxNodeName_t<Ident> { static constexpr const char* value = "Ident"; };
template<> struct CoercionLevel_t<String> { static constexpr int value = 0; };
template<> struct SyntaxNodeName_t<String> { static constexpr const char* value = "String"; };
template<> struct CoercionLevel_t<Integer> { static constexpr int value = 0; };
template<> struct SyntaxNodeName_t<Integer> { static constexpr const char* value = "Integer"; };
template<> struct CoercionLevel_t<Char> { static constexpr int value = 0; };
template<> struct SyntaxNodeName_t<Char> { static constexpr const char* value = "Char"; };
template<> struct CoercionLevel_t<Double> { static constexpr int value = 0; };
template<> struct SyntaxNodeName_t<Double> { static constexpr const char* value = "Double"; };
template<> struct CoercionLevel_t<SpecialBegin> { static constexpr int value = 0; };
template<> struct SyntaxNodeName_t<SpecialBegin> { static constexpr const char* value = "SpecialBegin"; };
template<> struct CoercionLevel_t<SpecialEnd> { static constexpr int value = 0; };
template<> struct SyntaxNodeName_t<SpecialEnd> { static constexpr const char* value = "SpecialEnd"; };
template<> struct CoercionLevel_t<ListExpr> { static constexpr int value = 0; };
template<> struct SyntaxNodeName_t<ListExpr> { static constexpr const char* value = "ListExpr"; };
template<> struct CoercionLevel_t<AProgram> { static constexpr int value = 0; };
template<> struct SyntaxNodeName_t<AProgram> { static constexpr const char* value = "AProgram"; };
template<> struct CoercionLevel_t<Abstraction> { static constexpr int value = 0; };
template<> struct SyntaxNodeName_t<Abstraction> { static constexpr const char* value = "Abstraction"; };
template<> struct CoercionLevel_t<Application> { static constexpr int value = 1; };
template<> struct SyntaxNodeName_t<Application> { static constexpr const char* value = "Application"; };
template<> struct CoercionLevel_t<Variable> { static constexpr int value = 2; };
template<> struct SyntaxNodeName_t<Variable> { static constexpr const char* value = "Variable"; };
template<> struct CoercionLevel_t<StringExpr> { static constexpr int value = 0; };
template<> struct SyntaxNodeName_t<StringExpr> { static constexpr const char* value = "StringExpr"; };
template<> struct CoercionLevel_t<CharExpr> { static constexpr int value = 0; };
template<> struct SyntaxNodeName_t<CharExpr> { static constexpr const char* value = "CharExpr"; };
template<> struct CoercionLevel_t<IntegerExpr> { static constexpr int value = 0; };
template<> struct SyntaxNodeName_t<IntegerExpr> { static constexpr const char* value = "IntegerExpr"; };
template<> struct CoercionLevel_t<DoubleExpr> { static constexpr int value = 0; };
template<> struct SyntaxNodeName_t<DoubleExpr> { static constexpr const char* value = "DoubleExpr"; };
template<> struct CoercionLevel_t<SpecialExpr> { static constexpr int value = 0; };
template<> struct SyntaxNodeName_t<SpecialExpr> { static constexpr const char* value = "SpecialExpr"; };

template<class T>
constexpr int CoercionLevel = CoercionLevel_t<T>::value;
template<class T>
constexpr const char* SyntaxNodeName = SyntaxNodeName_t<T>::value;

}

}
