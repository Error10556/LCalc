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

SpecialEnd::SpecialEnd(const location& l, const std::string& v)
    : loc(l)
    , Value(v) {}

SpecialEnd::SpecialEnd(const location& l, std::string&& v)
    : loc(l)
    , Value(std::move(v)) {}

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
    : loc(other.loc)
    , SpecialBegin_(other.SpecialBegin_)
    , Expr_(ClonePtr<Expr>(other.Expr_))
    , SpecialEnd_(other.SpecialEnd_) {}

SpecialExpr& SpecialExpr::operator=(const SpecialExpr& other [[maybe_unused]]) {
    loc = other.loc;
    SpecialBegin_ = other.SpecialBegin_;
    Expr_ = ClonePtr<Expr>(other.Expr_);
    SpecialEnd_ = other.SpecialEnd_;
    return *this;
}

SpecialExpr::SpecialExpr(const location& l, SpecialBegin&& _1, Expr&& _2, SpecialEnd&& _3)
    : loc(l)
    , SpecialBegin_(std::move(_1))
    , Expr_(std::make_unique<Expr>(std::move(_2)))
    , SpecialEnd_(std::move(_3)) {}

// CharExpr

CharExpr::CharExpr(const CharExpr& other [[maybe_unused]])
    : loc(other.loc)
    , Char_(other.Char_) {}

CharExpr& CharExpr::operator=(const CharExpr& other [[maybe_unused]]) {
    loc = other.loc;
    Char_ = other.Char_;
    return *this;
}

CharExpr::CharExpr(const location& l, Char&& _1)
    : loc(l)
    , Char_(std::move(_1)) {}

// DoubleExpr

DoubleExpr::DoubleExpr(const DoubleExpr& other [[maybe_unused]])
    : loc(other.loc)
    , Double_(other.Double_) {}

DoubleExpr& DoubleExpr::operator=(const DoubleExpr& other [[maybe_unused]]) {
    loc = other.loc;
    Double_ = other.Double_;
    return *this;
}

DoubleExpr::DoubleExpr(const location& l, Double&& _1)
    : loc(l)
    , Double_(std::move(_1)) {}

// IntegerExpr

IntegerExpr::IntegerExpr(const IntegerExpr& other [[maybe_unused]])
    : loc(other.loc)
    , Integer_(other.Integer_) {}

IntegerExpr& IntegerExpr::operator=(const IntegerExpr& other [[maybe_unused]]) {
    loc = other.loc;
    Integer_ = other.Integer_;
    return *this;
}

IntegerExpr::IntegerExpr(const location& l, Integer&& _1)
    : loc(l)
    , Integer_(std::move(_1)) {}

// StringExpr

StringExpr::StringExpr(const StringExpr& other [[maybe_unused]])
    : loc(other.loc)
    , String_(other.String_) {}

StringExpr& StringExpr::operator=(const StringExpr& other [[maybe_unused]]) {
    loc = other.loc;
    String_ = other.String_;
    return *this;
}

StringExpr::StringExpr(const location& l, String&& _1)
    : loc(l)
    , String_(std::move(_1)) {}

// Abstraction

Abstraction::Abstraction(const Abstraction& other [[maybe_unused]])
    : loc(other.loc)
    , Ident_(other.Ident_)
    , Expr_(ClonePtr<Expr>(other.Expr_)) {}

Abstraction& Abstraction::operator=(const Abstraction& other [[maybe_unused]]) {
    loc = other.loc;
    Ident_ = other.Ident_;
    Expr_ = ClonePtr<Expr>(other.Expr_);
    return *this;
}

Abstraction::Abstraction(const location& l, Ident&& _1, Expr&& _2)
    : loc(l)
    , Ident_(std::move(_1))
    , Expr_(std::make_unique<Expr>(std::move(_2))) {}

// Application

Application::Application(const Application& other [[maybe_unused]])
    : loc(other.loc)
    , Expr_1(ClonePtr<Expr>(other.Expr_1))
    , Expr_2(ClonePtr<Expr>(other.Expr_2)) {}

Application& Application::operator=(const Application& other [[maybe_unused]]) {
    loc = other.loc;
    Expr_1 = ClonePtr<Expr>(other.Expr_1);
    Expr_2 = ClonePtr<Expr>(other.Expr_2);
    return *this;
}

Application::Application(const location& l, Expr&& _1, Expr&& _2)
    : loc(l)
    , Expr_1(std::make_unique<Expr>(std::move(_1)))
    , Expr_2(std::make_unique<Expr>(std::move(_2))) {}

// Variable

Variable::Variable(const Variable& other [[maybe_unused]])
    : loc(other.loc)
    , Ident_(other.Ident_) {}

Variable& Variable::operator=(const Variable& other [[maybe_unused]]) {
    loc = other.loc;
    Ident_ = other.Ident_;
    return *this;
}

Variable::Variable(const location& l, Ident&& _1)
    : loc(l)
    , Ident_(std::move(_1)) {}

// Expr

const location& Expr::Location() const {
    return std::visit([](const auto& v) -> const location& {
        return v.loc;
    }, *this);
}

location& Expr::Location() {
    return std::visit([](auto& v) -> location& {
        return v.loc;
    }, *this);
}

// AProgram

AProgram::AProgram(const AProgram& other [[maybe_unused]])
    : loc(other.loc)
    , ListExpr_(other.ListExpr_) {}

AProgram& AProgram::operator=(const AProgram& other [[maybe_unused]]) {
    loc = other.loc;
    ListExpr_ = other.ListExpr_;
    return *this;
}

AProgram::AProgram(const location& l, ListExpr&& _1)
    : loc(l)
    , ListExpr_(std::move(_1)) {}

// User-defined functions

Expr make_id(const location& l, Ident&& varname) {
    Ident varname_1 = varname;
    return Expr(Abstraction(l, std::move(varname_1),
                            Expr(Variable(l, std::move(varname)))));
};

Expr make_const(const location& l, Ident&& varname) {
    Ident varname_1 = varname;
    return Expr(Abstraction(l, std::move(varname_1),
                            make_id(l, std::move(varname))));
};

}  // namespace LC
