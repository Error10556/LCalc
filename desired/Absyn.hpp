#pragma once
#include <memory>
#include <string>
#include <vector>
#include <variant>

namespace LC {

using Ident = std::string;

class AProgram;
using Program = std::variant<AProgram>;

class Abstraction; class Application; class Variable;
using Expr = std::variant<Abstraction, Application, Variable>;

using ListExpr = std::vector<Expr>;

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

}
