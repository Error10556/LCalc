#include "Absyn.hpp"

namespace LC {

template <class T>
static std::unique_ptr<T> ClonePtr(const std::unique_ptr<T>& p) {
    if (!p) return {};
    return std::make_unique<T>(*p);
}

Ident::Ident(const std::string& s)
    : String(s) {}

Ident::Ident(std::string&& s)
    : String(std::move(s)) {}

Ident& Ident::operator=(const std::string& s) {
    String = s;
    return *this;
}

Ident& Ident::operator=(std::string&& s) {
    String = std::move(s);
    return *this;
}

AProgram::AProgram(const AProgram& other)
    : ListExpr_(other.ListExpr_) {}

AProgram& AProgram::operator=(const AProgram& other) {
    ListExpr_ = other.ListExpr_;
    return *this;
}

AProgram::AProgram(ListExpr&& _1)
    : ListExpr_(std::move(_1)) {}

Abstraction::Abstraction(const Abstraction& other)
    : Ident_(other.Ident_), Expr_(ClonePtr<Expr>(other.Expr_)) {}

Abstraction& Abstraction::operator=(const Abstraction& other) {
    Ident_ = other.Ident_;
    Expr_ = ClonePtr<Expr>(other.Expr_);
    return *this;
}

Abstraction::Abstraction(Ident&& _1, Expr&& _2)
    : Ident_(std::move(_1)), Expr_(std::make_unique<Expr>(std::move(_2))) {}

Application::Application(const Application& other)
    : Expr_1(ClonePtr<Expr>(other.Expr_1))
    , Expr_2(ClonePtr<Expr>(other.Expr_2)) {}

Application& Application::operator=(const Application& other) {
    Expr_1 = ClonePtr<Expr>(other.Expr_1);
    Expr_2 = ClonePtr<Expr>(other.Expr_2);
    return *this;
}

Application::Application(Expr&& _1, Expr&& _2)
    : Expr_1(std::make_unique<Expr>(std::move(_1)))
    , Expr_2(std::make_unique<Expr>(std::move(_2))) {}

Variable::Variable(const Variable& other)
    : Ident_(other.Ident_) {}

Variable& Variable::operator=(const Variable& other) {
    Ident_ = other.Ident_;
    return *this;
}

Variable::Variable(Ident&& _1)
    : Ident_(std::move(_1)) {}

}
