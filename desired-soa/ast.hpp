#pragma once
#include "astlib-define-macros.include"
#include "astlib.hpp"

namespace ast {

/*******************/
/*  Custom Tokens  */
/*******************/

using SpecialBegin = String;
using SpecialEnd = String;

/****************/
/*  Categories  */
/****************/

BNFC_ASTLIB_PREDEF_LABEL(Abstraction);
BNFC_ASTLIB_PREDEF_LABEL(Application);
BNFC_ASTLIB_PREDEF_LABEL(Variable);
BNFC_ASTLIB_PREDEF_LABEL(StringExpr);
BNFC_ASTLIB_PREDEF_LABEL(IntegerExpr);
BNFC_ASTLIB_PREDEF_LABEL(DoubleExpr);
BNFC_ASTLIB_PREDEF_LABEL(CharExpr);
BNFC_ASTLIB_PREDEF_LABEL(SpecialExpr);
BNFC_ASTLIB_DEFINE_CAT(Expr, Abstraction<TAst>, Application<TAst>, Variable<TAst>, StringExpr<TAst>,
                       IntegerExpr<TAst>, DoubleExpr<TAst>, CharExpr<TAst>, SpecialExpr<TAst>);

BNFC_ASTLIB_PREDEF_LABEL(AProgram);
BNFC_ASTLIB_DEFINE_CAT(Program, AProgram<TAst>);

/*****************/
/*  Rule Labels  */
/*****************/

BNFC_ASTLIB_DEFINE_LABEL_TRAITS(AProgram, Program, 0, 1, ASTPtr<Array<TAst, Expr<TAst>>>)

template <lib::concepts::is_ast TAst>
class AProgram final {
    BNFC_ASTLIB_IMPLEMENT_LABEL(AProgram, Program, false, 1, 0, 0, 0, 0, 0);

    AProgram(TAst& ast, lib::ASTPtr<lib::Array<TAst, Expr<TAst>>> ListExpr_)
        : base(ast, lib::traits::type_tag_of_v<AProgram>,
               {lib::detail::ASTPtr_privates::Index(ListExpr_)}) {}

    lib::ASTPtr<lib::Array<TAst, Expr<TAst>>> ListExpr_() const {
        return lib::detail::ASTPtr_privates::Create<lib::ASTPtr<lib::Array<TAst, Expr<TAst>>>>(
            base.template pointer<0>());
    }
    void ListExpr_(lib::ASTPtr<lib::Array<TAst, Expr<TAst>>> value) {
        base.template pointer<0>(lib::detail::ASTPtr_privates::Index(value));
    }

    template <size_t I>
        requires(I < 1)
    lib::ASTPtr<lib::Array<TAst, Expr<TAst>>> get() const {
        return ListExpr_();
    }
};

BNFC_ASTLIB_DEFINE_LABEL_TRAITS(Abstraction, Expr, 0, 2, Expr<TAst>);

template <lib::concepts::is_ast TAst>
class Abstraction final {
    BNFC_ASTLIB_IMPLEMENT_LABEL(Abstraction, Expr, true, 1, 1, 0, 0, 0, 0);

    Abstraction(TAst& ast, Ident Ident_, Expr<TAst> Expr_)
        : base(ast, lib::traits::type_tag_of_v<Abstraction>,
               {lib::detail::ASTPtr_privates::Index(Expr_), std::move(Ident_)}) {}

    const Ident& Ident_() const { return base.template string<0>(); }
    Ident& Ident_() { return base.template string<0>(); }
    void Ident_(const Ident& value) { base.template string<0>() = value; }
    void Ident_(Ident&& value) { base.template string<0>() = std::move(value); }
    Expr<TAst> Expr_() const {
        return lib::detail::ASTPtr_privates::Create<Expr<TAst>>(base.template pointer<0>());
    }
    void Expr_(Expr<TAst> value) {
        base.template pointer<0>(lib::detail::ASTPtr_privates::Index(value));
    }

    template <size_t I>
        requires(I < 2)
    auto get() const {
        if constexpr (I == 0)
            return Ident_();
        else
            return Expr_();
    }
};

BNFC_ASTLIB_DEFINE_LABEL_TRAITS(Application, Expr, 1, 2, Expr<TAst>, Expr<TAst>);

template <lib::concepts::is_ast TAst>
class Application final {
    BNFC_ASTLIB_IMPLEMENT_LABEL(Application, Expr, true, 2, 0, 0, 0, 0, 0);

    Application(TAst& ast, Expr<TAst> Expr_1, Expr<TAst> Expr_2)
        : base(ast, lib::traits::type_tag_of_v<Application>,
               {lib::detail::ASTPtr_privates::Index(Expr_1),
                lib::detail::ASTPtr_privates::Index(Expr_2)}) {}

    Expr<TAst> Expr_1() const {
        return lib::detail::ASTPtr_privates::Create<Expr<TAst>>(base.template pointer<0>());
    }
    void Expr_1(Expr<TAst> value) {
        base.template pointer<0>(lib::detail::ASTPtr_privates::Index(value));
    }
    Expr<TAst> Expr_2() const {
        return lib::detail::ASTPtr_privates::Create<Expr<TAst>>(base.template pointer<1>());
    }
    void Expr_2(Expr<TAst> value) {
        base.template pointer<1>(lib::detail::ASTPtr_privates::Index(value));
    }

    template <size_t I>
        requires(I < 2)
    auto get() const {
        if constexpr (I == 0)
            return Expr_1();
        else
            return Expr_2();
    }
};

BNFC_ASTLIB_DEFINE_LABEL_TRAITS(Variable, Expr, 2, 1);

template <lib::concepts::is_ast TAst>
class Variable final {
    BNFC_ASTLIB_IMPLEMENT_LABEL(Variable, Expr, true, 0, 1, 0, 0, 0, 0);
    Variable(TAst& ast, Ident Ident_)
        : base(ast, lib::traits::type_tag_of_v<Variable>, {std::move(Ident_)}) {}

    const Ident& Ident_() const { return base.template string<0>(); }
    Ident& Ident_() { return base.template string<0>(); }
    void Ident_(const Ident& value) { base.template string<0>() = value; }
    void Ident_(Ident&& value) { base.template string<0>() = std::move(value); }

    template <size_t I>
        requires(I < 1)
    auto get() const {
        return Ident_();
    }
};

BNFC_ASTLIB_DEFINE_LABEL_TRAITS(StringExpr, Expr, 0, 1);

template <lib::concepts::is_ast TAst>
class StringExpr final {
    BNFC_ASTLIB_IMPLEMENT_LABEL(StringExpr, Expr, true, 0, 1, 0, 0, 0, 0);
    StringExpr(TAst& ast, String String_)
        : base(ast, lib::traits::type_tag_of_v<StringExpr>, {std::move(String_)}) {}

    const String& String_() const { return base.template string<0>(); }
    String& String_() { return base.template string<0>(); }
    void String_(const String& value) { base.template string<0>() = value; }
    void String_(String&& value) { base.template string<0>() = std::move(value); }

    template <size_t I>
        requires(I < 1)
    auto get() const {
        return String_();
    }
};

BNFC_ASTLIB_DEFINE_LABEL_TRAITS(IntegerExpr, Expr, 0, 1);

template <lib::concepts::is_ast TAst>
class IntegerExpr final {
    BNFC_ASTLIB_IMPLEMENT_LABEL(IntegerExpr, Expr, true, 0, 0, 0, 1, 0, 0);
    IntegerExpr(TAst& ast, Integer Integer_)
        : base(ast, lib::traits::type_tag_of_v<IntegerExpr>, {Integer_}) {}

    Integer Integer_() const { return base.template get_scalar_token<Integer, 0>(); }
    void Integer_(Integer value) { base.template set_scalar_token<Integer, 0>(value); }

    template <size_t I>
        requires(I < 1)
    auto get() const {
        return Integer_();
    }
};

BNFC_ASTLIB_DEFINE_LABEL_TRAITS(DoubleExpr, Expr, 0, 1);

template <lib::concepts::is_ast TAst>
class DoubleExpr final {
    BNFC_ASTLIB_IMPLEMENT_LABEL(DoubleExpr, Expr, true, 0, 0, 1, 0, 0, 0);
    DoubleExpr(TAst& ast, Double Double_)
        : base(ast, lib::traits::type_tag_of_v<DoubleExpr>, {Double_}) {}

    Double Double_() const { return base.template get_scalar_token<Double, 0>(); }
    void Double_(Double value) { base.template set_scalar_token<Double, 0>(value); }

    template <size_t I>
        requires(I < 1)
    auto get() const {
        return Double_();
    }
};

BNFC_ASTLIB_DEFINE_LABEL_TRAITS(CharExpr, Expr, 0, 1);

template <lib::concepts::is_ast TAst>
class CharExpr final {
    BNFC_ASTLIB_IMPLEMENT_LABEL(CharExpr, Expr, true, 0, 0, 0, 0, 1, 0);
public:
    CharExpr(TAst& ast, Char Char_) : base(ast, lib::traits::type_tag_of_v<CharExpr>, {Char_}) {}

    Char Char_() const { return base.template get_scalar_token<Char, 0>(); }
    void Char_(Char value) { base.template set_scalar_token<Char, 0>(value); }

    template <size_t I>
        requires(I < 1)
    auto get() const {
        return Char_();
    }
};

BNFC_ASTLIB_DEFINE_LABEL_TRAITS(SpecialExpr, Expr, 0, 1);

template <lib::concepts::is_ast TAst>
class SpecialExpr final {
    BNFC_ASTLIB_IMPLEMENT_LABEL(SpecialExpr, Expr, true, 1, 2, 0, 0, 0, 0);
    SpecialExpr(TAst& ast, SpecialBegin SpecialBegin_, Expr<TAst> Expr_, SpecialEnd SpecialEnd_)
        : base(ast, lib::traits::type_tag_of_v<SpecialExpr>,
               {lib::detail::ASTPtr_privates::Index(Expr_), std::move(SpecialBegin_),
                std::move(SpecialEnd_)}) {}

    const SpecialBegin& SpecialBegin_() const { return base.template string<0>(); }
    SpecialBegin& SpecialBegin_() { return base.template string<0>(); }
    void SpecialBegin_(const SpecialBegin& value) { base.template string<0>() = value; }
    void SpecialBegin_(SpecialBegin&& value) { base.template string<0>() = std::move(value); }
    Expr<TAst> Expr_() const {
        return lib::detail::ASTPtr_privates::Create<Expr<TAst>>(base.template pointer<0>());
    }
    void Expr_(Expr<TAst> value) {
        base.template pointer<0>(lib::detail::ASTPtr_privates::Index(value));
    }
    const SpecialEnd& SpecialEnd_() const { return base.template string<1>(); }
    SpecialEnd& SpecialEnd_() { return base.template string<1>(); }
    void SpecialEnd_(const SpecialEnd& value) { base.template string<1>() = value; }
    void SpecialEnd_(SpecialEnd&& value) { base.template string<1>() = std::move(value); }

    template <size_t I>
        requires(I < 3)
    auto get() const {
        if constexpr (I == 0)
            return SpecialBegin_();
        else if constexpr (I == 1)
            return Expr_();
        else
            return SpecialEnd_();
    }
};

/***************/
/*  Functions  */
/***************/

template <lib::concepts::is_ast TAst>
Expr<TAst> fn_id(TAst& ast [[maybe_unused]], Ident ident) {
    auto ident_2 = ident;
    return Abstraction(ast, std::move(ident), Variable(ast, std::move(ident_2)).pointer())
        .pointer();
}

template <lib::concepts::is_ast TAst>
Expr<TAst> fn_const(TAst& ast [[maybe_unused]], Ident ident) {
    auto ident_2 = ident;
    return Abstraction(ast, std::move(ident), fn_id(ast, std::move(ident_2))).pointer();
}

}  // namespace ast

#include "astlib-undef-macros.include"
