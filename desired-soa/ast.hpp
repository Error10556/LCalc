#pragma once
#include "astlib-define-macros.include"
#include "astlib.hpp"

namespace ast {

/*******************/
/*  Custom Tokens  */
/*******************/

namespace LC {

using SpecialBegin = String;
using SpecialEnd = String;

}  // namespace LC

/****************/
/*  Categories  */
/****************/

namespace LC {
BNFC_ASTLIB_PREDEF_LABEL(Abstraction);
BNFC_ASTLIB_PREDEF_LABEL(Application);
BNFC_ASTLIB_PREDEF_LABEL(Variable);
BNFC_ASTLIB_PREDEF_LABEL(StringExpr);
BNFC_ASTLIB_PREDEF_LABEL(IntegerExpr);
BNFC_ASTLIB_PREDEF_LABEL(DoubleExpr);
BNFC_ASTLIB_PREDEF_LABEL(CharExpr);
BNFC_ASTLIB_PREDEF_LABEL(SpecialExpr);
}  // namespace LC
// clang-format off
BNFC_ASTLIB_DEFINE_CAT(namespace LC{, }, LC::, Expr, Abstraction<TAst>, Application<TAst>,
                       Variable<TAst>, StringExpr<TAst>, IntegerExpr<TAst>, DoubleExpr<TAst>,
                       CharExpr<TAst>, SpecialExpr<TAst>);
// clang-format on

namespace LC {
BNFC_ASTLIB_PREDEF_LABEL(AProgram);
}
// clang-format off
BNFC_ASTLIB_DEFINE_CAT(namespace LC{, }, LC::, Program, AProgram<TAst>);
// clang-format on

/*********************/
/*  List Categories  */
/*********************/

namespace LC {
template <lib::concepts::is_ast TAst = lib::AST<>>
using ListExpr = lib::Array<TAst, Expr<TAst>>;
}

/*****************/
/*  Rule Labels  */
/*****************/

BNFC_ASTLIB_DEFINE_LABEL_TRAITS(LC::, AProgram, Program, 0, 1)

namespace LC {
template <lib::concepts::is_ast TAst>
class AProgram final {
    BNFC_ASTLIB_IMPLEMENT_LABEL(AProgram, Program, false, 1, 0, 0, 0, 0, 0);

    AProgram(TAst& ast, lib::detail::ASTNewNodeInfo reserved_space,
             lib::ASTPtr<ListExpr<TAst>> ListExpr_);

public:
    AProgram(TAst& ast, lib::ASTPtr<ListExpr<TAst>> ListExpr_);

    lib::ASTPtr<ListExpr<TAst>> ListExpr_() const;
    void ListExpr_(lib::ASTPtr<ListExpr<TAst>> value);

    template <size_t I, bool StringsByRefs = false>
        requires(I < 1)
    decltype(auto) get() const {
        return ListExpr_();
    }
};

template <lib::concepts::is_ast TAst>
AProgram<TAst>::AProgram(TAst& ast, lib::detail::ASTNewNodeInfo reserved_space,
                         lib::ASTPtr<ListExpr<TAst>> ListExpr_)
    : base(ast, reserved_space, lib::traits::type_tag_of_v<AProgram>,
           {lib::detail::ASTPtr_privates::Index(ListExpr_)}) {}

template <lib::concepts::is_ast TAst>
AProgram<TAst>::AProgram(TAst& ast, lib::ASTPtr<ListExpr<TAst>> ListExpr_)
    : base(ast, lib::traits::type_tag_of_v<AProgram>,
           {lib::detail::ASTPtr_privates::Index(ListExpr_)}) {}

template <lib::concepts::is_ast TAst>
lib::ASTPtr<ListExpr<TAst>> AProgram<TAst>::ListExpr_() const {
    return lib::detail::ASTPtr_privates::Create<lib::ASTPtr<ListExpr<TAst>>>(
        base.template pointer<0>());
}

template <lib::concepts::is_ast TAst>
void AProgram<TAst>::ListExpr_(lib::ASTPtr<ListExpr<TAst>> value) {
    base.template pointer<0>(lib::detail::ASTPtr_privates::Index(value));
}

}  // namespace LC

BNFC_ASTLIB_DEFINE_LABEL_TRAITS(LC::, Abstraction, Expr, 0, 2);

namespace LC {
template <lib::concepts::is_ast TAst>
class Abstraction final {
    BNFC_ASTLIB_IMPLEMENT_LABEL(Abstraction, Expr, true, 1, 1, 0, 0, 0, 0);

    Abstraction(TAst& ast, lib::detail::ASTNewNodeInfo reserved_space, Ident Ident_,
                Expr<TAst> Expr_);

public:
    Abstraction(TAst& ast, Ident Ident_, Expr<TAst> Expr_)
        : base(ast, lib::traits::type_tag_of_v<Abstraction>,
               {lib::detail::ASTPtr_privates::Index(Expr_), std::move(Ident_)}) {}

    const Ident& Ident_() const;
    Ident& Ident_();
    void Ident_(const Ident& value);
    void Ident_(Ident&& value);
    Expr<TAst> Expr_() const;
    void Expr_(Expr<TAst> value);

    template <size_t I, bool StringsByRefs = false>
        requires(I < 2)
    decltype(auto) get() const {
        if constexpr (I == 0) {
            if constexpr (StringsByRefs)
                return base.template string<0>();
            else
                return Ident(Ident_());
        } else
            return Expr_();
    }
};

template <lib::concepts::is_ast TAst>
Abstraction<TAst>::Abstraction(TAst& ast, lib::detail::ASTNewNodeInfo reserved_space, Ident Ident_,
                               Expr<TAst> Expr_)
    : base(ast, reserved_space, lib::traits::type_tag_of_v<Abstraction>,
           {lib::detail::ASTPtr_privates::Index(Expr_), std::move(Ident_)}) {}

template <lib::concepts::is_ast TAst>
const Ident& Abstraction<TAst>::Ident_() const {
    return base.template string<0>();
}
template <lib::concepts::is_ast TAst>
Ident& Abstraction<TAst>::Ident_() {
    return base.template string<0>();
}
template <lib::concepts::is_ast TAst>
void Abstraction<TAst>::Ident_(const Ident& value) {
    base.template string<0>() = value;
}
template <lib::concepts::is_ast TAst>
void Abstraction<TAst>::Ident_(Ident&& value) {
    base.template string<0>() = std::move(value);
}
template <lib::concepts::is_ast TAst>
Expr<TAst> Abstraction<TAst>::Expr_() const {
    return lib::detail::ASTPtr_privates::Create<Expr<TAst>>(base.template pointer<0>());
}
template <lib::concepts::is_ast TAst>
void Abstraction<TAst>::Expr_(Expr<TAst> value) {
    base.template pointer<0>(lib::detail::ASTPtr_privates::Index(value));
}

}  // namespace LC

BNFC_ASTLIB_DEFINE_LABEL_TRAITS(LC::, Application, Expr, 1, 2);

namespace LC {
template <lib::concepts::is_ast TAst>
class Application final {
    BNFC_ASTLIB_IMPLEMENT_LABEL(Application, Expr, true, 2, 0, 0, 0, 0, 0);

    Application(TAst& ast, lib::detail::ASTNewNodeInfo reserved_space, Expr<TAst> Expr_1,
                Expr<TAst> Expr_2);

public:
    Application(TAst& ast, Expr<TAst> Expr_1, Expr<TAst> Expr_2);

    Expr<TAst> Expr_1() const;
    void Expr_1(Expr<TAst> value);
    Expr<TAst> Expr_2() const;
    void Expr_2(Expr<TAst> value);

    template <size_t I, bool StringsByRefs = false>
        requires(I < 2)
    decltype(auto) get() const {
        if constexpr (I == 0)
            return Expr_1();
        else
            return Expr_2();
    }
};

template <lib::concepts::is_ast TAst>
Application<TAst>::Application(TAst& ast, lib::detail::ASTNewNodeInfo reserved_space,
                               Expr<TAst> Expr_1, Expr<TAst> Expr_2)
    : base(ast, reserved_space, lib::traits::type_tag_of_v<Application>,
           {lib::detail::ASTPtr_privates::Index(Expr_1),
            lib::detail::ASTPtr_privates::Index(Expr_2)}) {}
template <lib::concepts::is_ast TAst>
Application<TAst>::Application(TAst& ast, Expr<TAst> Expr_1, Expr<TAst> Expr_2)
    : base(ast, lib::traits::type_tag_of_v<Application>,
           {lib::detail::ASTPtr_privates::Index(Expr_1),
            lib::detail::ASTPtr_privates::Index(Expr_2)}) {}
template <lib::concepts::is_ast TAst>
Expr<TAst> Application<TAst>::Expr_1() const {
    return lib::detail::ASTPtr_privates::Create<Expr<TAst>>(base.template pointer<0>());
}
template <lib::concepts::is_ast TAst>
void Application<TAst>::Expr_1(Expr<TAst> value) {
    base.template pointer<0>(lib::detail::ASTPtr_privates::Index(value));
}
template <lib::concepts::is_ast TAst>
Expr<TAst> Application<TAst>::Expr_2() const {
    return lib::detail::ASTPtr_privates::Create<Expr<TAst>>(base.template pointer<1>());
}
template <lib::concepts::is_ast TAst>
void Application<TAst>::Expr_2(Expr<TAst> value) {
    base.template pointer<1>(lib::detail::ASTPtr_privates::Index(value));
}

}  // namespace LC

BNFC_ASTLIB_DEFINE_LABEL_TRAITS(LC::, Variable, Expr, 2, 1);

namespace LC {
template <lib::concepts::is_ast TAst>
class Variable final {
    BNFC_ASTLIB_IMPLEMENT_LABEL(Variable, Expr, true, 0, 1, 0, 0, 0, 0);

    Variable(TAst& ast, lib::detail::ASTNewNodeInfo reserved_space, Ident Ident_);

public:
    Variable(TAst& ast, Ident Ident_);

    const Ident& Ident_() const;
    Ident& Ident_();
    void Ident_(const Ident& value);
    void Ident_(Ident&& value);

    template <size_t I, bool StringsByRefs = false>
        requires(I < 1)
    decltype(auto) get() const {
        {
            if constexpr (StringsByRefs)
                return base.template string<0>();
            else
                return Ident(Ident_());
        }
    }
};

template <lib::concepts::is_ast TAst>
Variable<TAst>::Variable(TAst& ast, lib::detail::ASTNewNodeInfo reserved_space, Ident Ident_)
    : base(ast, reserved_space, lib::traits::type_tag_of_v<Variable>, {std::move(Ident_)}) {}
template <lib::concepts::is_ast TAst>
Variable<TAst>::Variable(TAst& ast, Ident Ident_)
    : base(ast, lib::traits::type_tag_of_v<Variable>, {std::move(Ident_)}) {}
template <lib::concepts::is_ast TAst>
const Ident& Variable<TAst>::Ident_() const {
    return base.template string<0>();
}
template <lib::concepts::is_ast TAst>
Ident& Variable<TAst>::Ident_() {
    return base.template string<0>();
}
template <lib::concepts::is_ast TAst>
void Variable<TAst>::Ident_(const Ident& value) {
    base.template string<0>() = value;
}
template <lib::concepts::is_ast TAst>
void Variable<TAst>::Ident_(Ident&& value) {
    base.template string<0>() = std::move(value);
}

}  // namespace LC

BNFC_ASTLIB_DEFINE_LABEL_TRAITS(LC::, StringExpr, Expr, 0, 1);

namespace LC {
template <lib::concepts::is_ast TAst>
class StringExpr final {
    BNFC_ASTLIB_IMPLEMENT_LABEL(StringExpr, Expr, true, 0, 1, 0, 0, 0, 0);

    StringExpr(TAst& ast, lib::detail::ASTNewNodeInfo reserved_space, String String_);

public:
    StringExpr(TAst& ast, String String_);

    const String& String_() const;
    String& String_();
    void String_(const String& value);
    void String_(String&& value);

    template <size_t I, bool StringsByRefs = false>
        requires(I < 1)
    decltype(auto) get() const {
        {
            if constexpr (StringsByRefs)
                return base.template string<0>();
            else
                return String(String_());
        }
    }
};

template <lib::concepts::is_ast TAst>
StringExpr<TAst>::StringExpr(TAst& ast, lib::detail::ASTNewNodeInfo reserved_space, String String_)
    : base(ast, reserved_space, lib::traits::type_tag_of_v<StringExpr>, {std::move(String_)}) {}
template <lib::concepts::is_ast TAst>
StringExpr<TAst>::StringExpr(TAst& ast, String String_)
    : base(ast, lib::traits::type_tag_of_v<StringExpr>, {std::move(String_)}) {}

template <lib::concepts::is_ast TAst>
const String& StringExpr<TAst>::String_() const {
    return base.template string<0>();
}
template <lib::concepts::is_ast TAst>
String& StringExpr<TAst>::String_() {
    return base.template string<0>();
}
template <lib::concepts::is_ast TAst>
void StringExpr<TAst>::String_(const String& value) {
    base.template string<0>() = value;
}
template <lib::concepts::is_ast TAst>
void StringExpr<TAst>::String_(String&& value) {
    base.template string<0>() = std::move(value);
}

}  // namespace LC

BNFC_ASTLIB_DEFINE_LABEL_TRAITS(LC::, IntegerExpr, Expr, 0, 1);

namespace LC {
template <lib::concepts::is_ast TAst>
class IntegerExpr final {
    BNFC_ASTLIB_IMPLEMENT_LABEL(IntegerExpr, Expr, true, 0, 0, 0, 1, 0, 0);

    IntegerExpr(TAst& ast, lib::detail::ASTNewNodeInfo reserved_space, Integer Integer_);

public:
    IntegerExpr(TAst& ast, Integer Integer_);

    Integer Integer_() const;
    void Integer_(Integer value);

    template <size_t I, bool StringsByRefs = false>
        requires(I < 1)
    decltype(auto) get() const {
        return Integer_();
    }
};

template <lib::concepts::is_ast TAst>
IntegerExpr<TAst>::IntegerExpr(TAst& ast, lib::detail::ASTNewNodeInfo reserved_space,
                               Integer Integer_)
    : base(ast, reserved_space, lib::traits::type_tag_of_v<IntegerExpr>, {Integer_}) {}
template <lib::concepts::is_ast TAst>
IntegerExpr<TAst>::IntegerExpr(TAst& ast, Integer Integer_)
    : base(ast, lib::traits::type_tag_of_v<IntegerExpr>, {Integer_}) {}

template <lib::concepts::is_ast TAst>
Integer IntegerExpr<TAst>::Integer_() const {
    return base.template get_scalar_token<Integer, 0>();
}
template <lib::concepts::is_ast TAst>
void IntegerExpr<TAst>::Integer_(Integer value) {
    base.template set_scalar_token<Integer, 0>(value);
}

}  // namespace LC

BNFC_ASTLIB_DEFINE_LABEL_TRAITS(LC::, DoubleExpr, Expr, 0, 1);

namespace LC {
template <lib::concepts::is_ast TAst>
class DoubleExpr final {
    BNFC_ASTLIB_IMPLEMENT_LABEL(DoubleExpr, Expr, true, 0, 0, 1, 0, 0, 0);

    DoubleExpr(TAst& ast, lib::detail::ASTNewNodeInfo reserved_space, Double Double_);

public:
    DoubleExpr(TAst& ast, Double Double_);

    Double Double_() const;
    void Double_(Double value);

    template <size_t I, bool StringsByRefs = false>
        requires(I < 1)
    decltype(auto) get() const {
        return Double_();
    }
};

template <lib::concepts::is_ast TAst>
DoubleExpr<TAst>::DoubleExpr(TAst& ast, lib::detail::ASTNewNodeInfo reserved_space, Double Double_)
    : base(ast, reserved_space, lib::traits::type_tag_of_v<DoubleExpr>, {Double_}) {}
template <lib::concepts::is_ast TAst>
DoubleExpr<TAst>::DoubleExpr(TAst& ast, Double Double_)
    : base(ast, lib::traits::type_tag_of_v<DoubleExpr>, {Double_}) {}

template <lib::concepts::is_ast TAst>
Double DoubleExpr<TAst>::Double_() const {
    return base.template get_scalar_token<Double, 0>();
}
template <lib::concepts::is_ast TAst>
void DoubleExpr<TAst>::Double_(Double value) {
    base.template set_scalar_token<Double, 0>(value);
}

}  // namespace LC

BNFC_ASTLIB_DEFINE_LABEL_TRAITS(LC::, CharExpr, Expr, 0, 1);

namespace LC {
template <lib::concepts::is_ast TAst>
class CharExpr final {
    BNFC_ASTLIB_IMPLEMENT_LABEL(CharExpr, Expr, true, 0, 0, 0, 0, 1, 0);
    CharExpr(TAst& ast, lib::detail::ASTNewNodeInfo reserved_space, Char Char_);

public:
    CharExpr(TAst& ast, Char Char_);
    Char Char_() const;
    void Char_(Char value);
    template <size_t I, bool StringsByRefs = false>
        requires(I < 1)
    decltype(auto) get() const {
        return Char_();
    }
};

template <lib::concepts::is_ast TAst>
CharExpr<TAst>::CharExpr(TAst& ast, lib::detail::ASTNewNodeInfo reserved_space, Char Char_)
    : base(ast, reserved_space, lib::traits::type_tag_of_v<CharExpr>, {Char_}) {}
template <lib::concepts::is_ast TAst>
CharExpr<TAst>::CharExpr(TAst& ast, Char Char_)
    : base(ast, lib::traits::type_tag_of_v<CharExpr>, {Char_}) {}

template <lib::concepts::is_ast TAst>
Char CharExpr<TAst>::Char_() const {
    return base.template get_scalar_token<Char, 0>();
}
template <lib::concepts::is_ast TAst>
void CharExpr<TAst>::Char_(Char value) {
    base.template set_scalar_token<Char, 0>(value);
}

}  // namespace LC

BNFC_ASTLIB_DEFINE_LABEL_TRAITS(LC::, SpecialExpr, Expr, 0, 3);

namespace LC {
template <lib::concepts::is_ast TAst>
class SpecialExpr final {
    BNFC_ASTLIB_IMPLEMENT_LABEL(SpecialExpr, Expr, true, 1, 2, 0, 0, 0, 0);

    SpecialExpr(TAst& ast, lib::detail::ASTNewNodeInfo reserved_space, SpecialBegin SpecialBegin_,
                Expr<TAst> Expr_, SpecialEnd SpecialEnd_);

public:
    SpecialExpr(TAst& ast, SpecialBegin SpecialBegin_, Expr<TAst> Expr_, SpecialEnd SpecialEnd_);
    const SpecialBegin& SpecialBegin_() const;
    SpecialBegin& SpecialBegin_();
    void SpecialBegin_(const SpecialBegin& value);
    void SpecialBegin_(SpecialBegin&& value);
    Expr<TAst> Expr_() const;
    void Expr_(Expr<TAst> value);
    const SpecialEnd& SpecialEnd_() const;
    SpecialEnd& SpecialEnd_();
    void SpecialEnd_(const SpecialEnd& value);
    void SpecialEnd_(SpecialEnd&& value);

    template <size_t I, bool StringsByRefs = false>
        requires(I < 3)
    decltype(auto) get() const {
        if constexpr (I == 0) {
            if constexpr (StringsByRefs)
                return base.template string<0>();
            else
                return SpecialBegin(SpecialBegin_());
        } else if constexpr (I == 1)
            return Expr_();
        else {
            if constexpr (StringsByRefs)
                return base.template string<0>();
            else
                return SpecialEnd(SpecialEnd_());
        }
    }
};

template <lib::concepts::is_ast TAst>
SpecialExpr<TAst>::SpecialExpr(TAst& ast, lib::detail::ASTNewNodeInfo reserved_space,
                               SpecialBegin SpecialBegin_, Expr<TAst> Expr_, SpecialEnd SpecialEnd_)
    : base(ast, reserved_space, lib::traits::type_tag_of_v<SpecialExpr>,
           {lib::detail::ASTPtr_privates::Index(Expr_), std::move(SpecialBegin_),
            std::move(SpecialEnd_)}) {}
template <lib::concepts::is_ast TAst>
SpecialExpr<TAst>::SpecialExpr(TAst& ast, SpecialBegin SpecialBegin_, Expr<TAst> Expr_,
                               SpecialEnd SpecialEnd_)
    : base(ast, lib::traits::type_tag_of_v<SpecialExpr>,
           {lib::detail::ASTPtr_privates::Index(Expr_), std::move(SpecialBegin_),
            std::move(SpecialEnd_)}) {}

template <lib::concepts::is_ast TAst>
const SpecialBegin& SpecialExpr<TAst>::SpecialBegin_() const {
    return base.template string<0>();
}
template <lib::concepts::is_ast TAst>
SpecialBegin& SpecialExpr<TAst>::SpecialBegin_() {
    return base.template string<0>();
}
template <lib::concepts::is_ast TAst>
void SpecialExpr<TAst>::SpecialBegin_(const SpecialBegin& value) {
    base.template string<0>() = value;
}
template <lib::concepts::is_ast TAst>
void SpecialExpr<TAst>::SpecialBegin_(SpecialBegin&& value) {
    base.template string<0>() = std::move(value);
}
template <lib::concepts::is_ast TAst>
Expr<TAst> SpecialExpr<TAst>::Expr_() const {
    return lib::detail::ASTPtr_privates::Create<Expr<TAst>>(base.template pointer<0>());
}
template <lib::concepts::is_ast TAst>
void SpecialExpr<TAst>::Expr_(Expr<TAst> value) {
    base.template pointer<0>(lib::detail::ASTPtr_privates::Index(value));
}
template <lib::concepts::is_ast TAst>
const SpecialEnd& SpecialExpr<TAst>::SpecialEnd_() const {
    return base.template string<1>();
}
template <lib::concepts::is_ast TAst>
SpecialEnd& SpecialExpr<TAst>::SpecialEnd_() {
    return base.template string<1>();
}
template <lib::concepts::is_ast TAst>
void SpecialExpr<TAst>::SpecialEnd_(const SpecialEnd& value) {
    base.template string<1>() = value;
}
template <lib::concepts::is_ast TAst>
void SpecialExpr<TAst>::SpecialEnd_(SpecialEnd&& value) {
    base.template string<1>() = std::move(value);
}

}  // namespace LC

/***************/
/*  Functions  */
/***************/

namespace LC {
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
}  // namespace LC

}  // namespace ast

// TODO remove
// using v = decltype(std::ranges::end(std::declval<ast::LC::ListExpr<>&>()));

#include "astlib-undef-macros.include"
