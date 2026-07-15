#include "Absyn.hpp"

namespace LC {

template <class T>
static std::unique_ptr<T> ClonePtr(const std::unique_ptr<T>& p) {
    if (!p) return {};
    return std::make_unique<T>(*p);
}

// token: Ident

Ident::Ident(const std::string& v)
    : Value(v) {}

Ident::Ident(std::string&& v)
    : Value(std::move(v)) {}

Ident& Ident::operator=(const std::string& v) {
    Value = v;
    return *this;
}

Ident& Ident::operator=(std::string&& v) {
    Value = std::move(v);
    return *this;
}

// token: Char

Char::Char(int32_t v)
    : Value(v) {}

Char& Char::operator=(int32_t v) {
    Value = v;
    return *this;
}

// token: Double

Double::Double(double v)
    : Value(v) {}

Double& Double::operator=(double v) {
    Value = v;
    return *this;
}

// token: Integer

Integer::Integer(long v)
    : Value(v) {}

Integer& Integer::operator=(long v) {
    Value = v;
    return *this;
}

// token: String

String::String(const std::string& v)
    : Value(v) {}

String::String(std::string&& v)
    : Value(std::move(v)) {}

String& String::operator=(const std::string& v) {
    Value = v;
    return *this;
}

String& String::operator=(std::string&& v) {
    Value = std::move(v);
    return *this;
}

// token: SpecialBegin

SpecialBegin::SpecialBegin(const std::string& v)
    : Value(v) {}

SpecialBegin::SpecialBegin(std::string&& v)
    : Value(std::move(v)) {}

SpecialBegin& SpecialBegin::operator=(const std::string& v) {
    Value = v;
    return *this;
}

SpecialBegin& SpecialBegin::operator=(std::string&& v) {
    Value = std::move(v);
    return *this;
}

// token: SpecialEnd

SpecialEnd::SpecialEnd(const std::string& v)
    : Value(v) {}

SpecialEnd::SpecialEnd(std::string&& v)
    : Value(std::move(v)) {}

SpecialEnd& SpecialEnd::operator=(const std::string& v) {
    Value = v;
    return *this;
}

SpecialEnd& SpecialEnd::operator=(std::string&& v) {
    Value = std::move(v);
    return *this;
}

// SpecialExpr

SpecialExpr::SpecialExpr(const SpecialExpr& other [[maybe_unused]])
    : SpecialBegin_(other.SpecialBegin_)
    , Expr_(ClonePtr<Expr>(other.Expr_))
    , SpecialEnd_(other.SpecialEnd_) {}

SpecialExpr& SpecialExpr::operator=(const SpecialExpr& other [[maybe_unused]]) {
    SpecialBegin_ = other.SpecialBegin_;
    Expr_ = ClonePtr<Expr>(other.Expr_);
    SpecialEnd_ = other.SpecialEnd_;
    return *this;
}

SpecialExpr::SpecialExpr(SpecialBegin&& _1, Expr&& _2, SpecialEnd&& _3)
    : SpecialBegin_(std::move(_1))
    , Expr_(std::make_unique<Expr>(std::move(_2)))
    , SpecialEnd_(std::move(_3)) {}

// CharExpr

CharExpr::CharExpr(const CharExpr& other [[maybe_unused]])
    : Char_(other.Char_) {}

CharExpr& CharExpr::operator=(const CharExpr& other [[maybe_unused]]) {
    Char_ = other.Char_;
    return *this;
}

CharExpr::CharExpr(Char&& _1)
    : Char_(std::move(_1)) {}

// DoubleExpr

DoubleExpr::DoubleExpr(const DoubleExpr& other [[maybe_unused]])
    : Double_(other.Double_) {}

DoubleExpr& DoubleExpr::operator=(const DoubleExpr& other [[maybe_unused]]) {
    Double_ = other.Double_;
    return *this;
}

DoubleExpr::DoubleExpr(Double&& _1)
    : Double_(std::move(_1)) {}

// IntegerExpr

IntegerExpr::IntegerExpr(const IntegerExpr& other [[maybe_unused]])
    : Integer_(other.Integer_) {}

IntegerExpr& IntegerExpr::operator=(const IntegerExpr& other [[maybe_unused]]) {
    Integer_ = other.Integer_;
    return *this;
}

IntegerExpr::IntegerExpr(Integer&& _1)
    : Integer_(std::move(_1)) {}

// StringExpr

StringExpr::StringExpr(const StringExpr& other [[maybe_unused]])
    : String_(other.String_) {}

StringExpr& StringExpr::operator=(const StringExpr& other [[maybe_unused]]) {
    String_ = other.String_;
    return *this;
}

StringExpr::StringExpr(String&& _1)
    : String_(std::move(_1)) {}

// Abstraction

Abstraction::Abstraction(const Abstraction& other [[maybe_unused]])
    : Ident_(other.Ident_)
    , Expr_(ClonePtr<Expr>(other.Expr_)) {}

Abstraction& Abstraction::operator=(const Abstraction& other [[maybe_unused]]) {
    Ident_ = other.Ident_;
    Expr_ = ClonePtr<Expr>(other.Expr_);
    return *this;
}

Abstraction::Abstraction(Ident&& _1, Expr&& _2)
    : Ident_(std::move(_1))
    , Expr_(std::make_unique<Expr>(std::move(_2))) {}

// Application

Application::Application(const Application& other [[maybe_unused]])
    : Expr_1(ClonePtr<Expr>(other.Expr_1))
    , Expr_2(ClonePtr<Expr>(other.Expr_2)) {}

Application& Application::operator=(const Application& other [[maybe_unused]]) {
    Expr_1 = ClonePtr<Expr>(other.Expr_1);
    Expr_2 = ClonePtr<Expr>(other.Expr_2);
    return *this;
}

Application::Application(Expr&& _1, Expr&& _2)
    : Expr_1(std::make_unique<Expr>(std::move(_1)))
    , Expr_2(std::make_unique<Expr>(std::move(_2))) {}

// Variable

Variable::Variable(const Variable& other [[maybe_unused]])
    : Ident_(other.Ident_) {}

Variable& Variable::operator=(const Variable& other [[maybe_unused]]) {
    Ident_ = other.Ident_;
    return *this;
}

Variable::Variable(Ident&& _1)
    : Ident_(std::move(_1)) {}

// AProgram

AProgram::AProgram(const AProgram& other [[maybe_unused]])
    : ListExpr_(other.ListExpr_) {}

AProgram& AProgram::operator=(const AProgram& other [[maybe_unused]]) {
    ListExpr_ = other.ListExpr_;
    return *this;
}

AProgram::AProgram(ListExpr&& _1)
    : ListExpr_(std::move(_1)) {}

// User-defined functions

Expr make_id(Ident&& varname) {
    Ident varname_1 = varname;
    return Expr(Abstraction(std::move(varname_1),
                            Expr(Variable(std::move(varname)))));
};

Expr make_const(Ident&& varname) {
    Ident varname_1 = varname;
    return Expr(Abstraction(std::move(varname_1),
                            make_id(std::move(varname))));
};

}  // namespace LC
