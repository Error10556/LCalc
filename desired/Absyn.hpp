#pragma once
#include <memory>
#include <string>
#include <vector>
#include <variant>

namespace LC {

template<class T>
struct CoercionLevel_t {};

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
template<> struct CoercionLevel_t<Ident> { static constexpr int value = 0; };

class AProgram;
using Program = std::variant<AProgram>;

class Abstraction; class Application; class Variable;
using Expr = std::variant<Abstraction, Application, Variable>;

using ListExpr = std::vector<Expr>;
template<> struct CoercionLevel_t<ListExpr> { static constexpr int value = 0; };

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
template<> struct CoercionLevel_t<AProgram> { static constexpr int value = 0; };

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
template<> struct CoercionLevel_t<Abstraction> { static constexpr int value = 0; };

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
template<> struct CoercionLevel_t<Application> { static constexpr int value = 1; };

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
template<> struct CoercionLevel_t<Variable> { static constexpr int value = 2; };

template<class T>
constexpr int CoercionLevel = CoercionLevel_t<T>::value;

}
