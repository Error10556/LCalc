#include "Absyn.hpp"

namespace LC {

template <class T>
static std::unique_ptr<T> ClonePtr(const std::unique_ptr<T>& p) {
    if (!p) return {};
    return std::make_unique<T>(*p);
}

Ident::Ident(const std::string& s)
    : Value(s) {}

Ident::Ident(std::string&& s)
    : Value(std::move(s)) {}

Ident& Ident::operator=(const std::string& s) {
    Value = s;
    return *this;
}

Ident& Ident::operator=(std::string&& s) {
    Value = std::move(s);
    return *this;
}

String::String(const std::string& s)
    : Value(s) {}

String::String(std::string&& s)
    : Value(std::move(s)) {}

String& String::operator=(const std::string& s) {
    Value = s;
    return *this;
}

String& String::operator=(std::string&& s) {
    Value = std::move(s);
    return *this;
}

Char::Char(const int32_t& v)
    : Value(v) {}

Char& Char::operator=(const int32_t& v) {
    Value = v;
    return *this;
}

Integer::Integer(const long& v)
    : Value(v) {}

Integer& Integer::operator=(const long& v) {
    Value = v;
    return *this;
}

Double::Double(const double& v)
    : Value(v) {}

Double& Double::operator=(const double& v) {
    Value = v;
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

StringExpr::StringExpr(const StringExpr& other)
    : String_(other.String_) {}

StringExpr& StringExpr::operator=(const StringExpr& other) {
    String_ = other.String_;
    return *this;
}

StringExpr::StringExpr(String&& _1)
    : String_(std::move(_1)) {}

IntegerExpr::IntegerExpr(const IntegerExpr& other)
    : Integer_(other.Integer_) {}

IntegerExpr& IntegerExpr::operator=(const IntegerExpr& other) {
    Integer_ = other.Integer_;
    return *this;
}

IntegerExpr::IntegerExpr(Integer&& _1)
    : Integer_(std::move(_1)) {}

DoubleExpr::DoubleExpr(const DoubleExpr& other)
    : Double_(other.Double_) {}

DoubleExpr& DoubleExpr::operator=(const DoubleExpr& other) {
    Double_ = other.Double_;
    return *this;
}

DoubleExpr::DoubleExpr(Double&& _1)
    : Double_(std::move(_1)) {}

CharExpr::CharExpr(const CharExpr& other)
    : Char_(other.Char_) {}

CharExpr& CharExpr::operator=(const CharExpr& other) {
    Char_ = other.Char_;
    return *this;
}

CharExpr::CharExpr(Char&& _1)
    : Char_(std::move(_1)) {}

}
