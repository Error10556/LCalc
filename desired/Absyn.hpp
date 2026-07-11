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
    Ident(const std::string&);  /* implicit */
    Ident(std::string&&);  /* implicit */
    Ident& operator=(const std::string&);
    Ident& operator=(std::string&&);
};

struct Char {
public:
    int32_t Value;
    Char() = default;
    Char(const Char&) = default;
    Char& operator=(const Char&) = default;
    Char& operator=(Char&&) = default;
    Char(int32_t);  /* implicit */
    Char& operator=(int32_t);
};

struct Double {
public:
    double Value;
    Double() = default;
    Double(const Double&) = default;
    Double& operator=(const Double&) = default;
    Double& operator=(Double&&) = default;
    Double(double);  /* implicit */
    Double& operator=(double);
};

struct Integer {
public:
    long Value;
    Integer() = default;
    Integer(const Integer&) = default;
    Integer& operator=(const Integer&) = default;
    Integer& operator=(Integer&&) = default;
    Integer(long);  /* implicit */
    Integer& operator=(long);
};

struct String {
public:
    std::string Value;
    String() = default;
    String(const String&) = default;
    String(String&&) = default;
    String& operator=(const String&) = default;
    String& operator=(String&&) = default;
    String(const std::string&);  /* implicit */
    String(std::string&&);  /* implicit */
    String& operator=(const std::string&);
    String& operator=(std::string&&);
};

struct SpecialBegin {
public:
    std::string Value;
    SpecialBegin() = default;
    SpecialBegin(const SpecialBegin&) = default;
    SpecialBegin(SpecialBegin&&) = default;
    SpecialBegin& operator=(const SpecialBegin&) = default;
    SpecialBegin& operator=(SpecialBegin&&) = default;
    SpecialBegin(const std::string&);  /* implicit */
    SpecialBegin(std::string&&);  /* implicit */
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
    SpecialEnd(const std::string&);  /* implicit */
    SpecialEnd(std::string&&);  /* implicit */
    SpecialEnd& operator=(const std::string&);
    SpecialEnd& operator=(std::string&&);
};

class Expr;
class SpecialExpr {
public:
    SpecialExpr() = default;
    SpecialExpr(const SpecialExpr&);  /* clone */
    SpecialExpr(SpecialExpr&&) = default;
    SpecialExpr& operator=(const SpecialExpr&);  /* discard & replace */
    SpecialExpr& operator=(SpecialExpr&&) = default;
    SpecialExpr(SpecialBegin&&, Expr&&, SpecialEnd&&);
    SpecialBegin SpecialBegin_;
    std::unique_ptr<Expr> Expr_;
    SpecialEnd SpecialEnd_;
};

class CharExpr {
public:
    CharExpr() = default;
    CharExpr(const CharExpr&);  /* clone */
    CharExpr(CharExpr&&) = default;
    CharExpr& operator=(const CharExpr&);  /* discard & replace */
    CharExpr& operator=(CharExpr&&) = default;
    CharExpr(Char&&);
    Char Char_;
};

class DoubleExpr {
public:
    DoubleExpr() = default;
    DoubleExpr(const DoubleExpr&);  /* clone */
    DoubleExpr(DoubleExpr&&) = default;
    DoubleExpr& operator=(const DoubleExpr&);  /* discard & replace */
    DoubleExpr& operator=(DoubleExpr&&) = default;
    DoubleExpr(Double&&);
    Double Double_;
};

class IntegerExpr {
public:
    IntegerExpr() = default;
    IntegerExpr(const IntegerExpr&);  /* clone */
    IntegerExpr(IntegerExpr&&) = default;
    IntegerExpr& operator=(const IntegerExpr&);  /* discard & replace */
    IntegerExpr& operator=(IntegerExpr&&) = default;
    IntegerExpr(Integer&&);
    Integer Integer_;
};

class StringExpr {
public:
    StringExpr() = default;
    StringExpr(const StringExpr&);  /* clone */
    StringExpr(StringExpr&&) = default;
    StringExpr& operator=(const StringExpr&);  /* discard & replace */
    StringExpr& operator=(StringExpr&&) = default;
    StringExpr(String&&);
    String String_;
};

class Abstraction {
public:
    Abstraction() = default;
    Abstraction(const Abstraction&);  /* clone */
    Abstraction(Abstraction&&) = default;
    Abstraction& operator=(const Abstraction&);  /* discard & replace */
    Abstraction& operator=(Abstraction&&) = default;
    Abstraction(Ident&&, Expr&&);
    Ident Ident_;
    std::unique_ptr<Expr> Expr_;
};

class Application {
public:
    Application() = default;
    Application(const Application&);  /* clone */
    Application(Application&&) = default;
    Application& operator=(const Application&);  /* discard & replace */
    Application& operator=(Application&&) = default;
    Application(Expr&&, Expr&&);
    std::unique_ptr<Expr> Expr_1;
    std::unique_ptr<Expr> Expr_2;
};

class Variable {
public:
    Variable() = default;
    Variable(const Variable&);  /* clone */
    Variable(Variable&&) = default;
    Variable& operator=(const Variable&);  /* discard & replace */
    Variable& operator=(Variable&&) = default;
    Variable(Ident&&);
    Ident Ident_;
};

class Expr : public std::variant<Variable, Application, Abstraction, StringExpr, IntegerExpr, DoubleExpr, CharExpr, SpecialExpr> {
public:
    using variant::variant;
};

class ListExpr : public std::deque<Expr> {
public:
    using deque::deque;
    template <class... TItems>
    static inline ListExpr Create(TItems&&... items) {
        ListExpr res;
        (res.emplace_back(std::forward<TItems>(items)), ...);
        return res;
    }
};

class AProgram {
public:
    AProgram() = default;
    AProgram(const AProgram&);  /* clone */
    AProgram(AProgram&&) = default;
    AProgram& operator=(const AProgram&);  /* discard & replace */
    AProgram& operator=(AProgram&&) = default;
    AProgram(ListExpr&&);
    ListExpr ListExpr_;
};

class Program : public std::variant<AProgram> {
public:
    using variant::variant;
    inline class AProgram& AProgram() {
        return std::get<class AProgram>(*this);
    }
    inline const class AProgram& AProgram() const {
        return std::get<class AProgram>(*this);
    }
};

// User-defined functions

Expr make_id(Ident&& varname);

Expr make_const(Ident&& varname);

namespace reflection {

template<class T> struct IsTokenStruct_t
{ static constexpr bool value = false; };
template<class T>
constexpr const bool IsTokenStruct = IsTokenStruct_t<T>::value;

template<class T> struct IsCategoryClass_t
{ static constexpr bool value = false; };
template<class T>
constexpr const bool IsCategoryClass = IsCategoryClass_t<T>::value;

template<class T> struct IsLabelClass_t
{ static constexpr bool value = false; };
template<class T>
constexpr const bool IsLabelClass = IsLabelClass_t<T>::value;

template<class T> struct IsParserEntrypoint_t
{ static constexpr bool value = false; };
template<class T>
constexpr const bool IsParserEntrypoint = IsParserEntrypoint_t<T>::value;

template<class T> struct CoercionLevel_t {};
template<class T> constexpr int CoercionLevel = CoercionLevel_t<T>::value;

template<class T> struct SyntaxNodeName_t {};
template<class T>
constexpr const char* SyntaxNodeName = SyntaxNodeName_t<T>::value;

#define REFL_KINDNAME(kind, _t) Is##kind##_t

#define REFL_NOCOERC(type, kind) \
template<> struct REFL_KINDNAME(kind, _t)<type> \
{ static constexpr bool value = true; }; \
template<> struct SyntaxNodeName_t<type> \
{ static constexpr const char* value = #type; }

#define REFL(type, kind, coerc) \
REFL_NOCOERC(type, kind); \
template<> struct CoercionLevel_t<type> \
{ static constexpr int value = coerc; } \

#define REFL_VAR(type) REFL_NOCOERC(type, CategoryClass)

#define ENTRYPOINT(type) \
template<> struct IsParserEntrypoint_t<type> \
{ static constexpr bool value = true; } \

REFL(Ident, TokenStruct, 0);
REFL(Char, TokenStruct, 0);
REFL(Double, TokenStruct, 0);
REFL(Integer, TokenStruct, 0);
REFL(String, TokenStruct, 0);
REFL(SpecialBegin, TokenStruct, 0);
REFL(SpecialEnd, TokenStruct, 0);

REFL(SpecialExpr, LabelClass, 0);
REFL(CharExpr, LabelClass, 0);
REFL(DoubleExpr, LabelClass, 0);
REFL(IntegerExpr, LabelClass, 0);
REFL(StringExpr, LabelClass, 0);
REFL(Abstraction, LabelClass, 0);
REFL(Application, LabelClass, 1);
REFL(Variable, LabelClass, 2);
REFL_VAR(Expr);
REFL(ListExpr, CategoryClass, 0);
REFL(AProgram, LabelClass, 0);
REFL_VAR(Program);

ENTRYPOINT(Program);

#undef ENTRYPOINT
#undef REFL_VAR
#undef REFL
#undef REFL_NOCOERC
#undef REFL_KINDNAME

}  // namespace reflection

}  // namespace LC
