#pragma once
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

template <lib::concepts::is_ast TAst>
class Abstraction;
template <lib::concepts::is_ast TAst>
class Application;
template <lib::concepts::is_ast TAst>
class Variable;
template <lib::concepts::is_ast TAst>
class StringExpr;
template <lib::concepts::is_ast TAst>
class IntegerExpr;
template <lib::concepts::is_ast TAst>
class DoubleExpr;
template <lib::concepts::is_ast TAst>
class CharExpr;
template <lib::concepts::is_ast TAst>
class SpecialExpr;
template <lib::concepts::is_ast TAst>
using Expr = lib::ASTPtr<Abstraction<TAst>, Application<TAst>, Variable<TAst>, StringExpr<TAst>,
                         IntegerExpr<TAst>, DoubleExpr<TAst>, CharExpr<TAst>, SpecialExpr<TAst>>;

template <lib::concepts::is_ast TAst>
class AProgram;
template <lib::concepts::is_ast TAst>
using Program = lib::ASTPtr<AProgram<TAst>>;

/*****************/
/*  Rule Labels  */
/*****************/

namespace lib {
namespace traits {
template <class TAst>
struct bnf_category_of<AProgram<TAst>> {
    using value_t = Program<TAst>;
};
template <class TAst>
struct ast_class_type<AProgram<TAst>> {
    static constexpr ASTClassType value = ASTClassType::Label;
};
}  // namespace traits
namespace detail {
template <class TAst>
struct label_pointer_fields<AProgram<TAst>> {
    using value_t = pack_of_types<ASTPtr<Array<TAst, Expr<TAst>>>>;
};
}  // namespace detail
}  // namespace lib

template <lib::concepts::is_ast TAst>
class AProgram final {
    friend lib::detail::AnyAstNode_privates;
    using base_t = lib::detail::Label<TAst, false, 1, 0, 0, 0, 0, 0>;
    base_t base;

    lib::detail::NodeMemoryPosition position_in_memory() const { return base.position_in_memory(); }

    AProgram(TAst& ast, uint32_t index) : base{ast, index} {}
    AProgram(base_t base) : base{base} {}

public:
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

    Program<TAst> pointer() const {
        return lib::detail::ASTPtr_privates::Create<Program<TAst>>(base.index());
    }
    template <class T>
    T& annotation() {
        return base.ast.template annotation<T>(pointer());
    }
    template <class T>
    const T& annotation() const {
        return base.ast.template annotation<T>(pointer());
    }
    template <lib::concepts::is_ast NewAst>
    AProgram<NewAst> with_ast(NewAst& new_ast) const {
        return lib::detail::AnyAstNode_privates::create_label<AProgram<NewAst>>(
            base.with_ast(new_ast));
    }
};

namespace lib {
namespace traits {
template <class TAst>
struct bnf_category_of<Abstraction<TAst>> {
    using value_t = Expr<TAst>;
};
template <class TAst>
struct ast_class_type<Abstraction<TAst>> {
    static constexpr ASTClassType value = ASTClassType::Label;
};
}  // namespace traits
namespace detail {
template <class TAst>
struct label_pointer_fields<Abstraction<TAst>> {
    using value_t = pack_of_types<Expr<TAst>>;
};
}  // namespace detail
}  // namespace lib

template <lib::concepts::is_ast TAst>
class Abstraction final {
    friend lib::detail::AnyAstNode_privates;
    using base_t = lib::detail::Label<TAst, true, 1, 1, 0, 0, 0, 0>;
    base_t base;

    lib::detail::NodeMemoryPosition position_in_memory() const { return base.position_in_memory(); }

    Abstraction(TAst& ast, uint32_t index) : base{ast, index} {}
    Abstraction(base_t base) : base{base} {}

public:
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

    Expr<TAst> pointer() const {
        return lib::detail::ASTPtr_privates::Create<Expr<TAst>>(base.index());
    }
    template <class T>
    T& annotation() {
        return base.ast.template annotation<T>(pointer());
    }
    template <class T>
    const T& annotation() const {
        return base.ast.template annotation<T>(pointer());
    }
    template <lib::concepts::is_ast NewAst>
    Abstraction<NewAst> with_ast(NewAst& new_ast) const {
        return lib::detail::AnyAstNode_privates::create_label<Abstraction<NewAst>>(
            base.with_ast(new_ast));
    }
};

namespace lib {
namespace traits {
template <class TAst>
struct bnf_category_of<Application<TAst>> {
    using value_t = Expr<TAst>;
};
template <class TAst>
struct ast_class_type<Application<TAst>> {
    static constexpr ASTClassType value = ASTClassType::Label;
};
}  // namespace traits
namespace detail {
template <class TAst>
struct label_pointer_fields<Application<TAst>> {
    using value_t = pack_of_types<Expr<TAst>>;
};
}  // namespace detail
}  // namespace lib

template <lib::concepts::is_ast TAst>
class Application final {
    friend lib::detail::AnyAstNode_privates;
    using base_t = lib::detail::Label<TAst, true, 2, 0, 0, 0, 0, 0>;
    base_t base;

    lib::detail::NodeMemoryPosition position_in_memory() const { return base.position_in_memory(); }

    Application(TAst& ast, uint32_t index) : base{ast, index} {}
    Application(base_t base) : base{base} {}

public:
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

    Expr<TAst> pointer() const {
        return lib::detail::ASTPtr_privates::Create<Expr<TAst>>(base.index());
    }
    template <class T>
    T& annotation() {
        return base.ast.template annotation<T>(pointer());
    }
    template <class T>
    const T& annotation() const {
        return base.ast.template annotation<T>(pointer());
    }
    template <lib::concepts::is_ast NewAst>
    Application<NewAst> with_ast(NewAst& new_ast) const {
        return lib::detail::AnyAstNode_privates::create_label<Application<NewAst>>(
            base.with_ast(new_ast));
    }
};

namespace lib {
namespace traits {
template <class TAst>
struct bnf_category_of<Variable<TAst>> {
    using value_t = Expr<TAst>;
};
template <class TAst>
struct ast_class_type<Variable<TAst>> {
    static constexpr ASTClassType value = ASTClassType::Label;
};
}  // namespace traits
namespace detail {
template <class TAst>
struct label_pointer_fields<Variable<TAst>> {
    using value_t = pack_of_types<Expr<TAst>>;
};
}  // namespace detail
}  // namespace lib

template <lib::concepts::is_ast TAst>
class Variable final {
    friend lib::detail::AnyAstNode_privates;
    using base_t = lib::detail::Label<TAst, true, 0, 1, 0, 0, 0, 0>;
    base_t base;

    lib::detail::NodeMemoryPosition position_in_memory() const { return base.position_in_memory(); }

    Variable(TAst& ast, uint32_t index) : base{ast, index} {}
    Variable(base_t base) : base{base} {}

public:
    Variable(TAst& ast, Ident Ident_)
        : base(ast, lib::traits::type_tag_of_v<Variable>, {std::move(Ident_)}) {}

    const Ident& Ident_() const { return base.template string<0>(); }
    Ident& Ident_() { return base.template string<0>(); }
    void Ident_(const Ident& value) { base.template string<0>() = value; }
    void Ident_(Ident&& value) { base.template string<0>() = std::move(value); }

    Expr<TAst> pointer() const {
        return lib::detail::ASTPtr_privates::Create<Expr<TAst>>(base.index());
    }
    template <class T>
    T& annotation() {
        return base.ast.template annotation<T>(pointer());
    }
    template <class T>
    const T& annotation() const {
        return base.ast.template annotation<T>(pointer());
    }
    template <lib::concepts::is_ast NewAst>
    Variable<NewAst> with_ast(NewAst& new_ast) const {
        return lib::detail::AnyAstNode_privates::create_label<Variable<NewAst>>(
            base.with_ast(new_ast));
    }
};

namespace lib {
namespace traits {
template <class TAst>
struct bnf_category_of<StringExpr<TAst>> {
    using value_t = Expr<TAst>;
};
template <class TAst>
struct ast_class_type<StringExpr<TAst>> {
    static constexpr ASTClassType value = ASTClassType::Label;
};
}  // namespace traits
namespace detail {
template <class TAst>
struct label_pointer_fields<StringExpr<TAst>> {
    using value_t = pack_of_types<>;
};
}  // namespace detail
}  // namespace lib

template <lib::concepts::is_ast TAst>
class StringExpr final {
    friend lib::detail::AnyAstNode_privates;
    using base_t = lib::detail::Label<TAst, true, 0, 1, 0, 0, 0, 0>;
    base_t base;

    lib::detail::NodeMemoryPosition position_in_memory() const { return base.position_in_memory(); }

    StringExpr(TAst& ast, uint32_t index) : base{ast, index} {}
    StringExpr(base_t base) : base{base} {}

public:
    StringExpr(TAst& ast, String String_)
        : base(ast, lib::traits::type_tag_of_v<StringExpr>, {std::move(String_)}) {}

    const String& String_() const { return base.template string<0>(); }
    String& String_() { return base.template string<0>(); }
    void String_(const String& value) { base.template string<0>() = value; }
    void String_(String&& value) { base.template string<0>() = std::move(value); }

    Expr<TAst> pointer() const {
        return lib::detail::ASTPtr_privates::Create<Expr<TAst>>(base.index());
    }
    template <class T>
    T& annotation() {
        return base.ast.template annotation<T>(pointer());
    }
    template <class T>
    const T& annotation() const {
        return base.ast.template annotation<T>(pointer());
    }
    template <lib::concepts::is_ast NewAst>
    StringExpr<NewAst> with_ast(NewAst& new_ast) const {
        return lib::detail::AnyAstNode_privates::create_label<StringExpr<NewAst>>(
            base.with_ast(new_ast));
    }
};

namespace lib {
namespace traits {
template <class TAst>
struct bnf_category_of<IntegerExpr<TAst>> {
    using value_t = Expr<TAst>;
};
template <class TAst>
struct ast_class_type<IntegerExpr<TAst>> {
    static constexpr ASTClassType value = ASTClassType::Label;
};
}  // namespace traits
namespace detail {
template <class TAst>
struct label_pointer_fields<IntegerExpr<TAst>> {
    using value_t = pack_of_types<>;
};
}  // namespace detail
}  // namespace lib

template <lib::concepts::is_ast TAst>
class IntegerExpr final {
    friend lib::detail::AnyAstNode_privates;
    using base_t = lib::detail::Label<TAst, true, 0, 0, 0, 1, 0, 0>;
    base_t base;

    lib::detail::NodeMemoryPosition position_in_memory() const { return base.position_in_memory(); }

    IntegerExpr(TAst& ast, uint32_t index) : base{ast, index} {}
    IntegerExpr(base_t base) : base{base} {}

public:
    IntegerExpr(TAst& ast, Integer Integer_)
        : base(ast, lib::traits::type_tag_of_v<IntegerExpr>, {Integer_}) {}

    Integer Integer_() const { return base.template get_scalar_token<Integer, 0>(); }
    void Integer_(Integer value) { base.template set_scalar_token<Integer, 0>(value); }

    Expr<TAst> pointer() const {
        return lib::detail::ASTPtr_privates::Create<Expr<TAst>>(base.index());
    }
    template <class T>
    T& annotation() {
        return base.ast.template annotation<T>(pointer());
    }
    template <class T>
    const T& annotation() const {
        return base.ast.template annotation<T>(pointer());
    }
    template <lib::concepts::is_ast NewAst>
    IntegerExpr<NewAst> with_ast(NewAst& new_ast) const {
        return lib::detail::AnyAstNode_privates::create_label<IntegerExpr<NewAst>>(
            base.with_ast(new_ast));
    }
};

namespace lib {
namespace traits {
template <class TAst>
struct bnf_category_of<DoubleExpr<TAst>> {
    using value_t = Expr<TAst>;
};
template <class TAst>
struct ast_class_type<DoubleExpr<TAst>> {
    static constexpr ASTClassType value = ASTClassType::Label;
};
}  // namespace traits
namespace detail {
template <class TAst>
struct label_pointer_fields<DoubleExpr<TAst>> {
    using value_t = pack_of_types<>;
};
}  // namespace detail
}  // namespace lib

template <lib::concepts::is_ast TAst>
class DoubleExpr final {
    friend lib::detail::AnyAstNode_privates;
    using base_t = lib::detail::Label<TAst, true, 0, 0, 1, 0, 0, 0>;
    base_t base;

    lib::detail::NodeMemoryPosition position_in_memory() const { return base.position_in_memory(); }

    DoubleExpr(TAst& ast, uint32_t index) : base{ast, index} {}
    DoubleExpr(base_t base) : base{base} {}

public:
    DoubleExpr(TAst& ast, Double Double_)
        : base(ast, lib::traits::type_tag_of_v<DoubleExpr>, {Double_}) {}

    Double Double_() const { return base.template get_scalar_token<Double, 0>(); }
    void Double_(Double value) { base.template set_scalar_token<Double, 0>(value); }

    Expr<TAst> pointer() const {
        return lib::detail::ASTPtr_privates::Create<Expr<TAst>>(base.index());
    }
    template <class T>
    T& annotation() {
        return base.ast.template annotation<T>(pointer());
    }
    template <class T>
    const T& annotation() const {
        return base.ast.template annotation<T>(pointer());
    }
    template <lib::concepts::is_ast NewAst>
    DoubleExpr<NewAst> with_ast(NewAst& new_ast) const {
        return lib::detail::AnyAstNode_privates::create_label<DoubleExpr<NewAst>>(
            base.with_ast(new_ast));
    }
};

namespace lib {
namespace traits {
template <class TAst>
struct bnf_category_of<CharExpr<TAst>> {
    using value_t = Expr<TAst>;
};
template <class TAst>
struct ast_class_type<CharExpr<TAst>> {
    static constexpr ASTClassType value = ASTClassType::Label;
};
}  // namespace traits
namespace detail {
template <class TAst>
struct label_pointer_fields<CharExpr<TAst>> {
    using value_t = pack_of_types<>;
};
}  // namespace detail
}  // namespace lib

template <lib::concepts::is_ast TAst>
class CharExpr final {
    friend lib::detail::AnyAstNode_privates;
    using base_t = lib::detail::Label<TAst, true, 0, 0, 0, 0, 1, 0>;
    base_t base;

    lib::detail::NodeMemoryPosition position_in_memory() const { return base.position_in_memory(); }

    CharExpr(TAst& ast, uint32_t index) : base{ast, index} {}
    CharExpr(base_t base) : base{base} {}

public:
    CharExpr(TAst& ast, Char Char_) : base(ast, lib::traits::type_tag_of_v<CharExpr>, {Char_}) {}

    Char Char_() const { return base.template get_scalar_token<Char, 0>(); }
    void Char_(Char value) { base.template set_scalar_token<Char, 0>(value); }

    Expr<TAst> pointer() const {
        return lib::detail::ASTPtr_privates::Create<Expr<TAst>>(base.index());
    }
    template <class T>
    T& annotation() {
        return base.ast.template annotation<T>(pointer());
    }
    template <class T>
    const T& annotation() const {
        return base.ast.template annotation<T>(pointer());
    }
    template <lib::concepts::is_ast NewAst>
    CharExpr<NewAst> with_ast(NewAst& new_ast) const {
        return lib::detail::AnyAstNode_privates::create_label<CharExpr<NewAst>>(
            base.with_ast(new_ast));
    }
};

namespace lib {
namespace traits {
template <class TAst>
struct bnf_category_of<SpecialExpr<TAst>> {
    using value_t = Expr<TAst>;
};
template <class TAst>
struct ast_class_type<SpecialExpr<TAst>> {
    static constexpr ASTClassType value = ASTClassType::Label;
};
}  // namespace traits
namespace detail {
template <class TAst>
struct label_pointer_fields<SpecialExpr<TAst>> {
    using value_t = pack_of_types<>;
};
}  // namespace detail
}  // namespace lib

template <lib::concepts::is_ast TAst>
class SpecialExpr final {
    friend lib::detail::AnyAstNode_privates;
    using base_t = lib::detail::Label<TAst, true, 1, 2, 0, 0, 0, 0>;
    base_t base;

    lib::detail::NodeMemoryPosition position_in_memory() const { return base.position_in_memory(); }

    SpecialExpr(TAst& ast, uint32_t index) : base{ast, index} {}
    SpecialExpr(base_t base) : base{base} {}

public:
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

    Expr<TAst> pointer() const {
        return lib::detail::ASTPtr_privates::Create<Expr<TAst>>(base.index());
    }
    template <class T>
    T& annotation() {
        return base.ast.template annotation<T>(pointer());
    }
    template <class T>
    const T& annotation() const {
        return base.ast.template annotation<T>(pointer());
    }
    template <lib::concepts::is_ast NewAst>
    SpecialExpr<NewAst> with_ast(NewAst& new_ast) const {
        return lib::detail::AnyAstNode_privates::create_label<SpecialExpr<NewAst>>(
            base.with_ast(new_ast));
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
