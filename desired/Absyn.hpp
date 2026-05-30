#pragma once
#include <memory>
#include <string>
#include <deque>
#include <variant>

namespace LC {

struct Ident {
public:
    std::string String;
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

class AProgram;
using Program = std::variant<AProgram>;

class Abstraction; class Application; class Variable;
using Expr = std::variant<Abstraction, Application, Variable>;

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

namespace reflection {

template <class T>
struct CoercionLevel_t {};

template <class T>
struct SyntaxNodeName_t {};

template<> struct SyntaxNodeName_t<Program> { static constexpr const char* value = "Program"; };
template<> struct SyntaxNodeName_t<Expr> { static constexpr const char* value = "Expr"; };
template<> struct CoercionLevel_t<Ident> { static constexpr int value = 0; };
template<> struct SyntaxNodeName_t<Ident> { static constexpr const char* value = "Ident"; };
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

template<class T>
constexpr int CoercionLevel = CoercionLevel_t<T>::value;
template<class T>
constexpr const char* SyntaxNodeName = SyntaxNodeName_t<T>::value;

}

}
