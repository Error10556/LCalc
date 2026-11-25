/*******************/
/*  DOCUMENTATION  */
/*******************/

/* Overview of the ast namespace:
('detail' namespaces contain internal implementation details and are therefore omitted)

== namespace ast { =================================================================================
  Contains classes encapsulating the AST data structure.

using Ident = std::string
  The identifier built-in token.

using String = std::string
  The UTF-8-encoded string literal built-in token.

using Integer = long
  The 64-bit signed integer literal built-in token.

using Double = double
  The double-precision floating point literal built-in token.

using Char = char32_t
  The UTF-32-encoded (UCS-4) character literal built-in token.

using Bool = bool
  The boolean value literal built-in token.

}  // namespace ast

== namespace ast::lib { ============================================================================
  Abstractions accessible to the user and automatically used by the generator.

auto deep_copy<DFSOrder = PreOrder, MoveStrings = false>
    (concepts::is_ast auto& source_ast, concepts::is_ast auto& dest_ast,
        concepts::is_ast_ptr auto ptr) -> decltype(ptr)
  Copies the subtree. The source and destination AST data structures may be the same.
  New nodes will be stored in depth-first pre- or post-order depending on the DFSOrder
  template value parameter.
  If MoveStrings is true, string token values will be moved to the destination instead of copied.
  Returns the pointer to the cloned subtree root in dest_ast.

auto shallow_copy<MoveStrings = false>
    (concepts::is_ast auto& ast, concepts::is_ast_ptr auto ptr) -> decltype(ptr)
  Copies just the node identified by ptr, re-using the pointers used by that node.
  Token values, however, will be copied or moved.
  Returns the pointer to the cloned node.

class AST<TAnnotations...>
│ Represents the entire Abstract Syntax Tree. Encapsulates node access.
│ If annotation structs are specified, creates one of each for every syntax node.
│ Token values are not syntax nodes and cannot be annotated.
│
├── AST()
│     New empty syntax tree.
│
├── AST(AST<TOtherAnnots...>&& rvalue_old_tree)
│     Changes the annotation list: default-constructs new annotations and keeps existing ones.
│     The argument must be discarded after the call.
│
├── AST<TAnnotations..., TMore...> annotate<TMore...>() &&
│     Adds more annotations, returns a new tree with all existing and additional annotations.
│     Annotation objects are default-constructed for each node. Must be called on an rvalue.
│
├── TAnnot&       annotation<TAnnot>(TPtr node_pointer)
├── const TAnnot& annotation<TAnnot>(TPtr node_pointer) const
│     Accesses an annotation object of a syntax node identified by node_pointer.
│
├── Visit<AST, TPtr...> visit(TPtr... ptrs)
│     Allows dynamic dispatch on one or more polymorphic syntax nodes (see operator|).
│
├── TLabel unchecked_get<TLabel>(TPtr ptr)
│     Returns a node accessor for a labelled rule object without checking if TLabel is
│     the correct type for it. If the type does not match, the behavior is undefined!!
│
├── std::optional<TLabel> checked_get<TLabel>(TPtr ptr)
│     Returns a node accessor for a labelled rule object, checking if TLabel is
│     the correct type for it. If the type does not match, returns an empty optional.
│
├── TLabel get(ASTPtr<TLabel> ptr)
│     Returns a node accessor for a labelled rule object.
│     Only defined for pointers representing LBNF categories with exactly one rule.
│
├── TArray get(ASTPtr<TArray> ptr)
│     Returns a list accessor.
│
└── std::pair<AST, TPtr> preorder(TPtr ptr)
      Returns a subtree of this tree rooted at the node identified by ptr, with the nodes
      ordered in depth-first pre-order.
      If called on a non-const rvalue, moves the values of string literals;
      the current AST object must be discarded after the call.
      If not, copies values of involved string literals.
      The returned pair contains the subtree as a separate AST and
      the root node pointer for the returned tree.

class ASTPtr<...>
│ Has several specializations:
│ 1. ASTPtr<TLabels...> (with 2 or more type parameters)
│    A polymorphic syntax node generated from an LBNF category with 2 or more labelled
│    rules. To access the node, use dynamic dispatch via visitation
│    (see operator|(Visit, Visitor)).
│ 2. ASTPtr<TLabel>
│    A syntax node pointer generated from an LBNF category with only 1 labelled rule.
│    However, traits::ast_class_type_v<> still gives ASTClassType::VariantPtr.
│ 3. ASTPtr<TArray>
│    A pointer to a BNFC list.
│
├── Visit<TAst, ASTPtr> visit(TAst& ast) const  (if specialization 1 or 2)
│     Allows dynamic dispatch.
│
├── TLabel unchecked_get<TLabel>(TAst& ast)     (if specialization 1)
│     Returns a node accessor for a labelled rule object without checking if TLabel is
│     the correct type for it. If the type does not match, the behavior is undefined!!
│
├── std::optional<TLabel> checked_get<TLabel>(TAst& ast)  (if specialization 1)
│     Returns a node accessor for a labelled rule object, checking if TLabel is
│     the correct type for it. If the type does not match, returns an empty optional.
│
├── TLabel get(TAst& ast)  (if specialization 2)
│     Returns a node accessor to the labelled rule object.
│
├── TArray get(TAst& ast)  (if specialization 3)
│     Returns a node accessor to the list object.
│
├── TAnnot&       annotation<TAnnot>(      AST<TAnnotations...>&)
├── const TAnnot& annotation<TAnnot>(const AST<TAnnotations...>&)
│     Returns an annotation of this node.
│
├── ASTPtr<...> update_annotations<TNewAst>() const
└── ASTPtr<...> update_annotations(const TNewAst&) const
      Returns the same pointer for the AST with new annotations.

class Visit<TAst, TVariantPtr...>
│ Contains the AST object reference and the pointers.
│ Used to dynamically dispatch visitation of one or several polymorphic nodes.
│
├── Visit(TAst& ast, TVariantPtr... pointers)
│     Stores the reference and the node addresses;
│     does not determine dynamic types of nodes.
│
├── decltype(auto) operator()(Visitor&& visitor_forwarding_reference)
│     Performs dynamic dispatch (similarly to std::visit).
│
└── decltype(auto) visit(Visitor&& visitor_forwarding_reference)
      Performs dynamic dispatch (similarly to std::visit).

decltype(auto) operator|(Visit&& visited_fwd_ref, Visitor&& visitor_fwd_ref)
  Performs dynamic dispatch (similarly to std::visit).

class Array<TAst, TItem>
│ Accesses a list of syntax node pointers or token values.
│ TItem can be an ASTPtr or any token value type.
│ The list size cannot be changed, hence the name.
│
├── Array(TAst&, TItem...)
│     Creates a new array syntax node in the tree with the provided items.
│     In particular, Array(TAst&) creates an empty array.
│
├── Array(TAst&, Range&& container_fwd_ref)
├── Array(TAst&, std::initializer_list<TItem>)
├── Array(TAst&, std::initializer_list<std::string_view>)  (if TItem == std::string)
├── Array(TAst&, std::initializer_list<const char*>)       (if TItem == std::string)
│     Creates a new array syntax node in the tree with the items taken from the range.
│     Use ranges::subrange(make_move_iterator(begin), make_move_iterator(end))
│     to avoid copying strings from a container.
│
├── TAst& ast() const
│     Returns the owning AST data structure.
│
├── uint32_t size() const
│     Returns the number of elements in the array.
│
├── bool empty() const
│     Equivalent to `size() == 0`.
│
├── ASTPtr<Array> pointer() const
│     Returns the pointer.
│
├── Array<TNewAst, ...> update_annotations(TNewAst& ast) const
│     Changes the stored AST reference. Use after calling the method AST::annotate or
│     the constructor AST(std::move(old_ast)).
│
├── struct [const_][reverse_]iterator
│     A random access read-write iterator.
│     If TItem != std::string, implements all operators except operator->().
│     If TItem == std::string, implements all operators.
│
├── struct ItemRefWrapper  (provided if TItem != std::string)
│     Returned instead of non-const item references.
│     Invalidated after any change has been made to the AST structure!
│
├── ItemRefWrapper     at(uint32_t index)        (if TItem != std::string)
├── TItem              at(uint32_t index) const  (if TItem != std::string)
├── std::string&       at(uint32_t index)        (if TItem == std::string)
├── const std::string& at(uint32_t index) const  (if TItem == std::string)
│     Performs bounds checking and returns an element or throws std::range_error.
│
├── ItemRefWrapper     operator[](uint32_t index)        (if TItem != std::string)
├── TItem              operator[](uint32_t index) const  (if TItem != std::string)
├── std::string&       operator[](uint32_t index)        (if TItem == std::string)
├── const std::string& operator[](uint32_t index) const  (if TItem == std::string)
│     Returns an element. Does not perform bounds checking.
│     Out-of-bounds errors can be catastrophic and/or difficult to
│     debug since all objects are packed together contiguously in memory.
│
└── [const_][reverse_]iterator [c][r]begin(), [c][r]end()
      Iterators pointing to the first (begin) and past-the-last (end) elements.
      Dereferencing end(), cend(), rend(), and rcend() invokes undefined behavior
      (see warning for operator[]).

class LabelledRuleObject<TAst>
│ Accesses a concrete node variant corresponding to a grammar rule.
│ Not part of the library; generated from definitions in the grammar file.
│
├── LabelledRuleObject(TAst&, ...args)
│     Creates a new rule object in the AST from the values of its subtree pointers and/or
│     token values. The order of arguments is the same as in the grammar production.
│
├── TAst& ast() const
│     Returns the owning AST data structure.
│
├── ASTPtr<LabelledRuleObject> pointer() const
│     Returns the pointer to this object.
│
├── LabelledRuleObject<TNewAst> update_annotations(TNewAst& ast) const
│     Changes the stored AST reference. Use after calling the method AST::annotate or
│     the constructor AST(std::move(old_ast)).
│
├── MemberType Member_() const
├── void Member_(MemberType)
│     Gets/sets a field.
│
└── const auto& [new_variables] = labelledRuleObject;
      Decomposes the object into its constructor arguments (without the AST&
      reference).

}  // namespace ast::lib

== namespace ast::lib::traits { ====================================================================
  Information about AST classes.

bool is_ast_v<T>
  'true' if T is an instantiation of 'ast::lib::AST<...>'.

enum class ASTClassType
│ Kinds of objects used in this module.
├── VariantPtr  = pointer (ASTPtr) to a polymorphic node object (LBNF category)
├── ArrayPtr    = pointer (ASTPtr) to a list of nodes or tokens
├── Array       = accessor representing a list of nodes or tokens
├── Label       = accessor representing a node of known type (LBNF labelled rule)
├── StringToken = any token represented as a std::string
├── ScalarToken = any token represented as a fundamental type
└── Unknown     = the type is unrelated

ASTClassType ast_class_type_v<T>
  Classifies the type T as one of the above.

using lbnf_category_of_t<TLabel>
  The LBNF category that the labelled rule belongs to.

int lbnf_coercion_level_v<TLabel/TArray>
  The "coercion level" of a rule (represents operator priority).
  Always 0 for arrays.

std::string lbnf_name<T>::get_name() requires (ast_class_type_v<T> != Unknown)
  The name of a BNFC-generated type as seen in the grammar file.

uint32_t type_tag_of_v<TLabel>
  The rule index in the LBNF category that the labelled rule belongs to.
  Internally used as the tag for dynamic dispatch.

using ast_array_value_t<TArray>
  The type of items in the array.

bool is_ast_annotated_by_v<Ast<...>, Annotations...>
  True iff the AST instantiation contains each of the listed annotation structs.

}  // namespace ast::lib::traits

== namespace ast::lib::concepts { ==================================================================
  Contains useful concepts for template definitions.

is_ast<T>
  T must be an ast::lib::AST<...> instantiation.

ast_annotated_by<AST<...>, Annotations...>
  The AST instantiation must contain each of the listed annotation structs.

is_ast_variant_ptr<T>
  T must be a pointer (ASTPtr) to a polymorphic node object (LBNF category).

is_ast_array_ptr<T>
  T must be a pointer (ASTPtr) to a list of syntax nodes or token values.

is_ast_ptr<T>
  T must be an AST object pointer (ASTPtr).

is_ast_label<T>
  T must be an accessor to a polymorphic node object (LBNF category).

is_ast_array<T>
  T must be an accessor to a list of syntax nodes or token values.

is_ast_node<T>
  T must be an accessor to a list or label.

is_ast_scalar_token<T>
  T must be a token value represented as a fundamental type (Double, Integer, Char, Bool).

is_ast_string_token<T>
  T must be a token value represented as a std::string.

is_ast_token<T>
  T must be a type of a token value (std::string, Double, Integer, Char, Bool).

}  // namespace ast::lib::concepts

 */

#pragma once
// we assume read-write access to the AST (TODO: implement read-only labels)
#include <algorithm>
#include <array>
#include <cstdint>
#include <initializer_list>
#include <iterator>
#include <optional>
#include <ranges>
#include <string>
#include <string_view>
#include <tuple>
#include <type_traits>
#include <utility>
#include <vector>

namespace ast {

/**********/
/* Tokens */
/**********/

using Ident = std::string;
using String = std::string;
using Integer = int64_t;
using Double = double;
using Char = char32_t;
using Bool = bool;
static_assert(sizeof(Double) == 8);
static_assert(sizeof(Bool) == 1);

namespace lib {

/*******************/
/* Metaprogramming */
/*******************/

namespace concepts::detail {
template <class T>
concept default_constructible = std::is_default_constructible_v<T>;
template <class T, class... Ts>
concept same_as_any_of = (std::same_as<T, Ts> || ...);
}  // namespace concepts::detail

namespace detail {

template <class... Ts>
struct pairwise_different {};

template <>
struct pairwise_different<> {
    static constexpr bool value = true;
};

template <class T>
struct pairwise_different<T> {
    static constexpr bool value = true;
};

template <class T, class... Ts>
struct pairwise_different<T, Ts...> {
    static constexpr bool value =
        !concepts::detail::same_as_any_of<T, Ts...> && pairwise_different<Ts...>::value;
};

template <class... Ts>
static constexpr bool pairwise_different_v = pairwise_different<Ts...>::value;

template <size_t Index, class T, class... Ts>
struct pack_index {
    using value_t = pack_index<Index - 1, Ts...>::value_t;
};

template <class T, class... Ts>
struct pack_index<0ULL, T, Ts...> {
    using value_t = T;
};

template <size_t Index, class... Ts>
using pack_index_t = pack_index<Index, Ts...>::value_t;

template <class T, class... Ts>
struct pack_find {};

template <class T>
struct pack_find<T> {
    static constexpr int value = -1;
};

template <class T, class U, class... Ts>
struct pack_find<T, U, Ts...> {
private:
    static consteval int inc_if_valid(int val) { return val + (val >= 0); }

public:
    static constexpr int value =
        (std::same_as<T, U> ? 0 : inc_if_valid(pack_find<T, Ts...>::value));
};

template <class T, class... Ts>
static constexpr int pack_find_v = pack_find<T, Ts...>::value;

template <class... Ts>
struct pack_of_types {};

template <size_t Index, class Pack>
struct pack_of_types_index;

template <size_t Index, class... Ts>
struct pack_of_types_index<Index, pack_of_types<Ts...>> {
    using value_t = pack_index<Index, Ts...>::value_t;
};

template <class T, class Pack>
struct pack_of_types_find;

template <class T, class... Ts>
struct pack_of_types_find<T, pack_of_types<Ts...>> {
    static constexpr int value = pack_find<T, Ts...>::value;
};

template <class T, class Pack>
constexpr int pack_of_types_find_v = pack_of_types_find<T, Pack>::value;

template <class Pack>
struct pack_of_types_size {};

template <class... Ts>
struct pack_of_types_size<pack_of_types<Ts...>> {
    static constexpr size_t value = sizeof...(Ts);
};

template <class Pack>
constexpr size_t pack_of_types_size_v = pack_of_types_size<Pack>::value;

template <size_t Index, class... Ts>
using pack_of_types_index_t = pack_of_types_index<Index, Ts...>::value_t;

template <size_t Index, class... Packs>
struct packs_mdindex {};

template <size_t Index, class... Ts, class... Packs>
struct packs_mdindex<Index, pack_of_types<Ts...>, Packs...> {
private:
    static constexpr size_t coef = (pack_of_types_size_v<Packs> * ...);
    using rest_t = packs_mdindex<Index % coef, Packs...>::value_t;
    template <class, class>
    struct Prepender {};
    template <class L, class... Rs>
    struct Prepender<L, pack_of_types<Rs...>> {
        using value_t = pack_of_types<L, Rs...>;
    };

public:
    using value_t = Prepender<pack_index_t<Index / coef, Ts...>, rest_t>::value_t;
};

template <size_t Index, class... Ts>
struct packs_mdindex<Index, pack_of_types<Ts...>> {
    using value_t = pack_of_types<pack_index_t<Index, Ts...>>;
};

template <size_t Index, class... Packs>
using packs_mdindex_t = packs_mdindex<Index, Packs...>::value_t;

template <class... Packs>
struct concat_packs;

template <class... Ts, class... Packs>
struct concat_packs<pack_of_types<Ts...>, Packs...> {
private:
    template <class Pack>
    struct helper;
    template <class... Us>
    struct helper<pack_of_types<Us...>> {
        using value_t = pack_of_types<Ts..., Us...>;
    };

public:
    using value_t = helper<typename concat_packs<Packs...>::value_t>::value_t;
};

template <>
struct concat_packs<> {
    using value_t = pack_of_types<>;
};

template <class... Packs>
using concat_packs_t = concat_packs<Packs...>::value_t;

template <class Pack>
struct pack_to_tuple;

template <class... Ts>
struct pack_to_tuple<pack_of_types<Ts...>> {
    using value_t = std::tuple<Ts...>;
};

template <class Pack>
using pack_to_tuple_t = pack_to_tuple<Pack>::value_t;

template <size_t N, class T>
struct repeat_pack {
private:
    template <class Pack>
    struct helper;
    template <class... Ts>
    struct helper<pack_of_types<Ts...>> {
        using value_t =
            std::conditional_t<N & 1, pack_of_types<T, Ts..., Ts...>, pack_of_types<Ts..., Ts...>>;
    };

public:
    using value_t = helper<typename repeat_pack<N / 2, T>::value_t>::value_t;
};
template <class T>
struct repeat_pack<0, T> {
    using value_t = pack_of_types<>;
};
template <class T>
struct repeat_pack<1, T> {
    using value_t = pack_of_types<T>;
};
template <class T>
struct repeat_pack<2, T> {
    using value_t = pack_of_types<T, T>;
};

template <size_t N, class T>
using repeat_pack_t = repeat_pack<N, T>::value_t;

}  // namespace detail

/*******************/
/* AST type traits */
/*******************/

namespace traits {

template <class T>
struct is_ast {
    static constexpr bool value = false;
};

template <class T>
static constexpr bool is_ast_v = is_ast<T>::value;

enum class ASTClassType {
    VariantPtr,
    ArrayPtr,
    Array,
    Label,
    StringToken,
    ScalarToken,
    Unknown,
};

template <class T>
struct ast_class_type {
    static constexpr ASTClassType value = ASTClassType::Unknown;
};

template <class T>
static constexpr ASTClassType ast_class_type_v = ast_class_type<T>::value;

template <>
struct ast_class_type<std::string> {
    static constexpr ASTClassType value = ASTClassType::StringToken;
};

template <>
struct ast_class_type<Integer> {
    static constexpr ASTClassType value = ASTClassType::ScalarToken;
};

template <>
struct ast_class_type<bool> {
    static constexpr ASTClassType value = ASTClassType::ScalarToken;
};

template <>
struct ast_class_type<Double> {
    static constexpr ASTClassType value = ASTClassType::ScalarToken;
};

template <>
struct ast_class_type<Char> {
    static constexpr ASTClassType value = ASTClassType::ScalarToken;
};

template <class TAst, class... Annots>
struct is_ast_annotated_by {
    static constexpr bool value = false;
};

// Definition of is_ast_annotated_by is after class AST

template <class TAst, class... Annots>
constexpr bool is_ast_annotated_by_v = is_ast_annotated_by<TAst, Annots...>::value;

}  // namespace traits

namespace concepts {

template <class T>
concept is_ast = traits::is_ast_v<T>;

template <class TAst, class... Annots>
concept ast_annotated_by = traits::is_ast_annotated_by_v<TAst, Annots...>;

template <class T>
concept is_ast_variant_ptr = traits::ast_class_type_v<T> == traits::ASTClassType::VariantPtr;

template <class T>
concept is_ast_array_ptr = traits::ast_class_type_v<T> == traits::ASTClassType::ArrayPtr;

template <class T>
concept is_ast_ptr = is_ast_variant_ptr<T> || is_ast_array_ptr<T>;

template <class T>
concept is_ast_label = traits::ast_class_type_v<T> == traits::ASTClassType::Label;

template <class T>
concept is_ast_array = traits::ast_class_type_v<T> == traits::ASTClassType::Array;

template <class T>
concept is_ast_node = is_ast_label<T> || is_ast_array<T>;

template <class T>
concept is_ast_scalar_token = traits::ast_class_type_v<T> == traits::ASTClassType::ScalarToken;

template <class T>
concept is_ast_string_token = traits::ast_class_type_v<T> == traits::ASTClassType::StringToken;

template <class T>
concept is_ast_token = is_ast_string_token<T> || is_ast_scalar_token<T>;

}  // namespace concepts

namespace traits {

template <concepts::is_ast_label TLabel>
struct lbnf_category_of;

template <concepts::is_ast_label TLabel>
using lbnf_category_of_t = lbnf_category_of<TLabel>::value_t;

template <concepts::is_ast_label TLabel>
struct type_tag_of {
private:
    using cat_t = lbnf_category_of_t<TLabel>;
    static constexpr int signed_val = detail::pack_of_types_find_v<TLabel, cat_t>;
    static_assert(signed_val >= 0);

public:
    static constexpr uint32_t value = signed_val;
};

template <concepts::is_ast_label TLabel>
constexpr uint32_t type_tag_of_v = type_tag_of<TLabel>::value;

template <concepts::is_ast_array TArray>
struct ast_array_value;

template <concepts::is_ast_array TArray>
using ast_array_value_t = ast_array_value<TArray>::value_t;

template <concepts::is_ast_node T>
struct lbnf_coercion_level;

template <concepts::is_ast_array T>
struct lbnf_coercion_level<T> {
    static constexpr int value = 0;
};

template <concepts::is_ast_node T>
constexpr int lbnf_coercion_level_v = lbnf_coercion_level<T>::value;

template <class T>
    requires(ast_class_type_v<T> != ASTClassType::Unknown)
struct lbnf_name;

template <>
struct lbnf_name<std::string> {
    static constexpr std::string get_name() { return "Stringlike"; }
};

template <>
struct lbnf_name<Integer> {
    static constexpr std::string get_name() { return "Integer"; }
};

template <>
struct lbnf_name<Double> {
    static constexpr std::string get_name() { return "Double"; }
};

template <>
struct lbnf_name<Char> {
    static constexpr std::string get_name() { return "Char"; }
};

template <>
struct lbnf_name<Bool> {
    static constexpr std::string get_name() { return "Bool"; }
};

// lbnf_name<Array<>> after Array definition

}  // namespace traits

/************************/
/* Abstract Syntax Tree */
/************************/

template <concepts::detail::default_constructible... TAnnotations>
    requires(detail::pairwise_different_v<TAnnotations...>)
class AST;

namespace detail {

struct ASTNodeItem {
    uint32_t info;
    std::array<uint32_t, 3> children;
};

struct ASTNewNodeInfo {
    uint32_t node_index;
    uint32_t extras_index;
    uint32_t strings_index;
};

struct AST_privates {
    template <concepts::is_ast TAst>
    static std::vector<detail::ASTNodeItem>& tree(TAst& ast) {
        return ast.tree;
    }
    template <concepts::is_ast TAst>
    static const std::vector<detail::ASTNodeItem>& tree(const TAst& ast) {
        return ast.tree;
    }
    template <concepts::is_ast TAst>
    static std::vector<std::string>& string_tokens(TAst& ast) {
        return ast.string_tokens;
    }
    template <concepts::is_ast TAst>
    static const std::vector<std::string>& string_tokens(const TAst& ast) {
        return ast.string_tokens;
    }
    template <concepts::is_ast TAst>
    static std::vector<uint32_t>& extra(TAst& ast) {
        return ast.extra;
    }
    template <concepts::is_ast TAst>
    static const std::vector<uint32_t>& extra(const TAst& ast) {
        return ast.extra;
    }
    template <class TAnnot, class... Annots>
        requires concepts::detail::same_as_any_of<TAnnot, Annots...>
    static std::vector<TAnnot>& annots(AST<Annots...>& ast) {
        return std::get<std::vector<TAnnot>>(ast.annots);
    }
    template <class TAnnot, class... Annots>
        requires concepts::detail::same_as_any_of<TAnnot, Annots...>
    static const std::vector<TAnnot>& annots(const AST<Annots...>& ast) {
        return std::get<std::vector<TAnnot>>(ast.annots);
    }
    template <concepts::is_ast TAst>
    static auto& annots_tuple(TAst& ast) {
        return ast.annots;
    }
    template <concepts::is_ast TAst>
    static const auto& annots_tuple(const TAst& ast) {
        return ast.annots;
    }
    template <concepts::is_ast TAst>
    static ASTNewNodeInfo emplace_back(TAst& ast, uint32_t nextra, uint32_t nstrings,
                                       bool needs_8byte_align) {
        return ast.emplace_back(nextra, nstrings, needs_8byte_align);
    }
};

}  // namespace detail

template <concepts::is_ast TAst, concepts::is_ast_variant_ptr... TASTPtr>
    requires(sizeof...(TASTPtr) > 0)
class Visit;
template <class...>
class ASTPtr;
template <concepts::is_ast, class>
class Array;

template <concepts::detail::default_constructible... TAnnotations>
    requires(detail::pairwise_different_v<TAnnotations...>)
class AST {
    std::vector<detail::ASTNodeItem> tree;
    std::vector<std::string> string_tokens;
    std::vector<uint32_t> extra;
    std::tuple<std::vector<TAnnotations>...> annots;

    template <class... TOtherAnnots>
    static std::tuple<std::vector<TAnnotations>...> transform_annotations(
        std::tuple<TOtherAnnots...>&& other, uint32_t default_size);

    detail::ASTNewNodeInfo emplace_back(uint32_t nextra, uint32_t nstrings, bool needs_8byte_align);
    friend detail::AST_privates;

public:
    AST() = default;
    template <class... TOtherAnnots>
    AST(AST<TOtherAnnots...>&& other);
    uint32_t size() const { return tree.size(); }
    template <concepts::detail::default_constructible... T>
        requires(sizeof...(T) > 0)
    AST<TAnnotations..., T...> annotate() && {
        return AST<TAnnotations..., T...>(std::move(*this));
    }

    template <class TAnnot, concepts::is_ast_ptr TPtr>
        requires(std::same_as<TAnnot, TAnnotations> || ...)
    inline TAnnot& annotation(TPtr ptr)
        requires(sizeof...(TAnnotations) > 0);
    template <class TAnnot, concepts::is_ast_ptr TPtr>
        requires(std::same_as<TAnnot, TAnnotations> || ...)
    inline const TAnnot& annotation(TPtr ptr) const
        requires(sizeof...(TAnnotations) > 0);

    template <concepts::is_ast_variant_ptr... TPtr>
        requires(sizeof...(TPtr) > 0)
    inline Visit<AST, TPtr...> visit(TPtr... ptrs) {
        return {*this, ptrs...};
    }
    template <concepts::is_ast_label TLabel, concepts::is_ast_label... TLs>
        requires(concepts::detail::same_as_any_of<TLabel, TLs...> && sizeof...(TLs) > 1)
    inline TLabel unchecked_get(ASTPtr<TLs...> ptr);
    template <concepts::is_ast_label TLabel, concepts::is_ast_label... TLs>
        requires(concepts::detail::same_as_any_of<TLabel, TLs...>)
    inline std::optional<TLabel> checked_get(ASTPtr<TLs...> ptr);
    template <concepts::is_ast_label TLabel>
    inline TLabel get(ASTPtr<TLabel> ptr);
    template <concepts::is_ast_array TArray>
    inline TArray get(ASTPtr<TArray> ptr);

public:
    template <concepts::is_ast_ptr TPtr>
    inline std::pair<AST, TPtr> preorder(TPtr root) &;
    template <concepts::is_ast_ptr TPtr>
    inline std::pair<AST, TPtr> preorder(TPtr root) &&;
};

template <concepts::detail::default_constructible... TAnnotations>
    requires(detail::pairwise_different_v<TAnnotations...>)
template <class... TOtherAnnots>
AST<TAnnotations...>::AST(AST<TOtherAnnots...>&& other)
    : tree{std::move(detail::AST_privates::tree(other))},
      string_tokens{std::move(detail::AST_privates::string_tokens(other))},
      extra{std::move(detail::AST_privates::extra(other))},
      annots{transform_annotations(std::move(detail::AST_privates::annots_tuple(other)),
                                   tree.size())} {}

template <concepts::detail::default_constructible... TAnnotations>
    requires(detail::pairwise_different_v<TAnnotations...>)
detail::ASTNewNodeInfo AST<TAnnotations...>::emplace_back(uint32_t nextra, uint32_t nstrings,
                                                          bool needs_8byte_align) {
    uint32_t res_index = tree.size();
    tree.emplace_back();
    if (needs_8byte_align && nextra && (extra.size() & 1)) {
        extra.emplace_back();
    }
    uint32_t extra_index = extra.size();
    uint32_t strings_index = string_tokens.size();
    extra.resize(extra_index + nextra);
    string_tokens.resize(strings_index + nstrings);
    [this]<size_t... Indices>(std::index_sequence<Indices...>) {
        (std::get<Indices>(annots).emplace_back(), ...);
    }(std::make_index_sequence<sizeof...(TAnnotations)>());
    return {res_index, extra_index, strings_index};
}

template <concepts::detail::default_constructible... TAnnotations>
    requires(detail::pairwise_different_v<TAnnotations...>)
template <class... TOtherAnnots>
std::tuple<std::vector<TAnnotations>...> AST<TAnnotations...>::transform_annotations(
    std::tuple<TOtherAnnots...>&& other, uint32_t default_size) {
    using return_type = std::tuple<std::vector<TAnnotations>...>;
    static constexpr auto other_indices =
        std::integer_sequence<int, detail::pack_find_v<TAnnotations, TOtherAnnots...>...>();
    return [&]<int... OtherIndices, size_t... MyIndices>(
               std::integer_sequence<int, OtherIndices...>,
               std::index_sequence<MyIndices...>) -> return_type {
        return {[&]() -> std::vector<TAnnotations> {
            if constexpr (OtherIndices == -1) {
                return std::vector<TAnnotations>(default_size);
            } else {
                return std::move(std::get<OtherIndices>(other));
            }
        }()...};
    }(other_indices, std::make_index_sequence<sizeof...(TAnnotations)>());
}

namespace traits {

template <class... Ts>
struct is_ast<AST<Ts...>> {
    static constexpr bool value = true;
};

template <class... RealAnnots, class... NeededAnnots>
struct is_ast_annotated_by<AST<RealAnnots...>, NeededAnnots...> {
    static constexpr bool value = (... && (detail::pack_find_v<NeededAnnots, RealAnnots...> >= 0));
};

}  // namespace traits

/**************************/
/* Node Pointer (general) */
/**************************/

template <class...>
class ASTPtr {
    static_assert(false, "This kind of AST pointer is not supported");
};

namespace detail {
struct ASTPtr_privates {
    template <concepts::is_ast_ptr TPtr>
    static uint32_t Index(TPtr ptr) {
        return ptr.index;
    }
    template <concepts::is_ast_ptr TPtr>
    static TPtr Create(uint32_t index) {
        return TPtr(index);
    }
};
}  // namespace detail

/************************/
/* Variant Node Pointer */
/************************/

namespace detail {

template <size_t Index, class... Ts>
struct pack_of_types_index<Index, ASTPtr<Ts...>> {
    using value_t = pack_index<Index, Ts...>::value_t;
};

template <class T, class... Ts>
struct pack_of_types_find<T, ASTPtr<Ts...>> {
    static constexpr int value = pack_find<T, Ts...>::value;
};

template <class... Ts>
struct pack_of_types_size<ASTPtr<Ts...>> {
    static constexpr size_t value = sizeof...(Ts);
};

template <size_t Index, class... Ts, class... Packs>
struct packs_mdindex<Index, ASTPtr<Ts...>, Packs...> {
private:
    static constexpr size_t coef = (pack_of_types_size_v<Packs> * ...);
    using rest_t = packs_mdindex<Index % coef, Packs...>::value_t;
    template <class, class>
    struct Prepender {};
    template <class L, class... Rs>
    struct Prepender<L, pack_of_types<Rs...>> {
        using value_t = pack_of_types<L, Rs...>;
    };

public:
    using value_t = Prepender<pack_index_t<Index / coef, Ts...>, rest_t>::value_t;
};

template <size_t Index, class... Ts>
struct packs_mdindex<Index, ASTPtr<Ts...>> {
    using value_t = pack_of_types<pack_index_t<Index, Ts...>>;
};

template <concepts::is_ast_ptr TPtr, concepts::is_ast TAst>
struct astptr_with_updated_annotations;

template <class... Ts, concepts::is_ast TAst>
struct astptr_with_updated_annotations<ASTPtr<Ts...>, TAst> {
    using value_t =
        ASTPtr<decltype(std::declval<Ts>().update_annotations(std::declval<TAst&>()))...>;
};

template <concepts::is_ast_ptr TPtr, concepts::is_ast TAst>
using astptr_with_updated_annotations_t = astptr_with_updated_annotations<TPtr, TAst>::value_t;

}  // namespace detail

template <concepts::is_ast_label... Ts>
    requires(sizeof...(Ts) > 1)
class ASTPtr<Ts...> {
    uint32_t index;
    ASTPtr(uint32_t index) : index{index} {}
    friend detail::ASTPtr_privates;

public:
    template <concepts::is_ast TAst>
    Visit<TAst, ASTPtr> visit(TAst& ast) const {
        return {ast, *this};
    }
    template <concepts::detail::same_as_any_of<Ts...> TLabel, concepts::is_ast TAst>
    std::optional<TLabel> checked_get(TAst& ast) const {
        return ast.template checked_get<TLabel>(*this);
    }
    template <concepts::detail::same_as_any_of<Ts...> TLabel, concepts::is_ast TAst>
    TLabel unchecked_get(TAst& ast) const {
        return ast.template unchecked_get<TLabel>(*this);
    }
    template <concepts::detail::default_constructible TAnnot, class... TAnnots>
        requires concepts::detail::same_as_any_of<TAnnot, TAnnots...>
    TAnnot& annotation(AST<TAnnots...>& ast) {
        return ast.template annotation<TAnnot>(*this);
    }
    template <concepts::detail::default_constructible TAnnot, class... TAnnots>
        requires concepts::detail::same_as_any_of<TAnnot, TAnnots...>
    const TAnnot& annotation(const AST<TAnnots...>& ast) {
        return ast.template annotation<TAnnot>(*this);
    }
    template <concepts::is_ast TNewAst>
    detail::astptr_with_updated_annotations_t<ASTPtr, TNewAst> update_annotations() const {
        return detail::ASTPtr_privates::Create<
            detail::astptr_with_updated_annotations_t<ASTPtr, TNewAst>>(index);
    }
    template <concepts::is_ast TNewAst>
    detail::astptr_with_updated_annotations_t<ASTPtr, TNewAst> update_annotations(
        const TNewAst&) const {
        return update_annotations<TNewAst>();
    }
};

template <concepts::is_ast_label TLabel>
class ASTPtr<TLabel> {
    uint32_t index;
    ASTPtr(uint32_t index) : index{index} {}
    friend detail::ASTPtr_privates;

public:
    template <concepts::is_ast TAst>
    Visit<TAst, ASTPtr> visit(TAst& ast) const {
        return {ast, *this};
    }
    template <concepts::is_ast TAst>
    TLabel get(TAst& ast) const {
        return ast.get(*this);
    }
    template <concepts::detail::default_constructible TAnnot, class... TAnnots>
        requires concepts::detail::same_as_any_of<TAnnot, TAnnots...>
    TAnnot& annotation(AST<TAnnots...>& ast) {
        return ast.template annotation<TAnnot>(*this);
    }
    template <concepts::detail::default_constructible TAnnot, class... TAnnots>
        requires concepts::detail::same_as_any_of<TAnnot, TAnnots...>
    const TAnnot& annotation(const AST<TAnnots...>& ast) {
        return ast.template annotation<TAnnot>(*this);
    }
    template <concepts::is_ast TNewAst>
    detail::astptr_with_updated_annotations_t<ASTPtr, TNewAst> update_annotations() const {
        return detail::ASTPtr_privates::Create<
            detail::astptr_with_updated_annotations_t<ASTPtr, TNewAst>>(index);
    }
    template <concepts::is_ast TNewAst>
    detail::astptr_with_updated_annotations_t<ASTPtr, TNewAst> update_annotations(
        const TNewAst&) const {
        return update_annotations<TNewAst>();
    }
};

namespace traits {
template <concepts::is_ast_label... Ts>
struct ast_class_type<ASTPtr<Ts...>> {
    static constexpr ASTClassType value = ASTClassType::VariantPtr;
};
}  // namespace traits

template <class... Funcs>
class Cases : private Funcs... {
public:
    using Funcs::operator()...;
};
template <class... Funcs>
Cases(Funcs&&...) -> Cases<Funcs...>;

template <concepts::is_ast TAst, concepts::is_ast_variant_ptr... TASTPtr>
    requires(sizeof...(TASTPtr) > 0)
class Visit {
    TAst& ast;
    std::array<uint32_t, sizeof...(TASTPtr)> indices;

public:
    Visit(TAst& ast, TASTPtr... ptrs)
        : ast{ast}, indices{detail::ASTPtr_privates::Index(ptrs)...} {}
    template <class Visitor>
    auto operator()(Visitor&&)
        -> std::invoke_result_t<Visitor&&, detail::pack_of_types_index_t<0ULL, TASTPtr>...>;
    template <class Visitor>
    decltype(auto) visit(Visitor&& visitor) {
        return (*this)(std::forward<Visitor>(visitor));
    }
};

template <class TAst, class... TASTPtr, class Visitor>
inline decltype(auto) operator|(Visit<TAst, TASTPtr...>&& visited, Visitor&& f) {
    return std::move(visited)(std::forward<Visitor>(f));
}

namespace detail {

struct AnyAstNode_privates {
    template <concepts::is_ast_label Label, class... Args>
    static Label create_label(Args&&... args) {
        return Label(std::forward<Args>(args)...);
    }
    template <concepts::is_ast_array TArray, class... Args>
    static TArray create_array(Args&&... args) {
        return TArray(std::forward<Args>(args)...);
    }
    template <concepts::is_ast_label TLabel, concepts::is_ast TAst>
    static ASTNewNodeInfo reserve_space(TAst& ast) {
        return TLabel::base_t::reserve_space(ast);
    }
    template <concepts::is_ast_array TArray, concepts::is_ast TAst>
    static ASTNewNodeInfo reserve_space(TAst& ast, uint32_t size) {
        return TArray::reserve_space(ast, size);
    }
    template <concepts::is_ast_label TLabel>
    using label_base_t = TLabel::base_t;
};

}  // namespace detail

template <concepts::is_ast TAst, concepts::is_ast_variant_ptr... TASTPtr>
    requires(sizeof...(TASTPtr) > 0)
template <class Visitor>
auto Visit<TAst, TASTPtr...>::operator()(Visitor&& visitor)
    -> std::invoke_result_t<Visitor&&, detail::pack_of_types_index_t<0ULL, TASTPtr>...> {
    using namespace detail;
    constexpr size_t npacks = sizeof...(TASTPtr);
    using proxy_t = std::invoke_result_t<Visitor&&, pack_of_types_index_t<0ULL, TASTPtr>...> (*)(
        Visitor&&, TAst&, const std::array<uint32_t, npacks>&);
    static constexpr std::array vtable = []<size_t... TIndices>(
                                             std::integer_sequence<size_t, TIndices...>) {
        return std::array<proxy_t, sizeof...(TIndices)>{
            [](Visitor&& vis, TAst& ast,
               const std::array<uint32_t, npacks>& indices) -> decltype(auto) {
                using MyTypesPack = packs_mdindex_t<TIndices, TASTPtr...>;
                return [&]<class... MyTypes, size_t... MyTypeIndices>(
                           pack_of_types<MyTypes...>,
                           std::integer_sequence<size_t, MyTypeIndices...>) -> decltype(auto) {
                    return std::forward<Visitor>(vis)(
                        AnyAstNode_privates::create_label<MyTypes>(ast, indices[MyTypeIndices])...);
                }(MyTypesPack(), std::make_index_sequence<pack_of_types_size_v<MyTypesPack>>());
            }...};
    }(std::make_index_sequence<(pack_of_types_size_v<TASTPtr> * ...)>());
    static constexpr std::array<size_t, npacks> coefs{pack_of_types_size_v<TASTPtr>...};
    size_t totalindex = 0;
    const auto& treenodes = detail::AST_privates::tree(ast);
    for (size_t i = 0; i < std::size(indices); i++) {
        if (coefs[i] == 1) continue;  // Must not access treenodes[i].info of a singleton variant
        totalindex = totalindex * coefs[i] + treenodes[indices[i]].info;
    }
    return vtable[totalindex](std::forward<Visitor>(visitor), ast, indices);
}

/**********/
/* Arrays */
/**********/

template <concepts::is_ast, class>
class Array {
    static_assert(false, "Arrays of this type are not supported in the AST");
};

namespace traits {
template <class TAst, class Item>
struct ast_class_type<Array<TAst, Item>> {
    static constexpr ASTClassType value = ASTClassType::Array;
};
template <concepts::is_ast TAst, class T>
struct ast_array_value<Array<TAst, T>> {
    using value_t = T;
};
template <concepts::is_ast TAst, class T>
struct lbnf_name<Array<TAst, T>> {
public:
    static constexpr std::string get_name() {
        return std::string("List") + lbnf_name<T>::get_name();
    }
};

}  // namespace traits

namespace detail {
template <concepts::is_ast TAst, class... Ts>
    requires(sizeof...(Ts) > 0) &&
            (... &&
             std::same_as<std::remove_cvref_t<pack_index_t<0, Ts...>>, std::remove_cvref_t<Ts>>)
std::pair<ASTNodeItem, ASTNewNodeInfo> emplace_scalar_array(TAst& ast, Ts&&... args) {
    using T = std::remove_cvref_t<pack_index_t<0, Ts...>>;
    static constexpr bool Is8ByteAligned = sizeof(T) == 8;
    static constexpr uint32_t LongListInlineTs = sizeof(uint32_t) * 2 / sizeof(T);
    static constexpr uint32_t ShortListInlineTs =
        sizeof(uint32_t) * (3 - Is8ByteAligned) / sizeof(T);
    static constexpr size_t N = sizeof...(args);
    static constexpr bool IsShortList = N <= ShortListInlineTs;
    static constexpr size_t ExtrasSize =
        IsShortList
            ? 0
            : (N - LongListInlineTs) * ((sizeof(T) + sizeof(uint32_t) - 1) / sizeof(uint32_t));
    static constexpr size_t InlineItems = IsShortList ? ShortListInlineTs : LongListInlineTs;
    std::pair<ASTNodeItem, ASTNewNodeInfo> res;
    auto& [item, mempos] = res;
    mempos = AST_privates::emplace_back(ast, ExtrasSize, 0, Is8ByteAligned);
    item.info = N;
    T* ptr_inline = reinterpret_cast<T*>(&item.children[Is8ByteAligned]);
    T* ptr_extras;
    if constexpr (IsShortList)
        ptr_extras = nullptr;
    else
        ptr_extras = reinterpret_cast<T*>(&AST_privates::extra(ast)[mempos.extras_index]);

    [&]<size_t... Indices>(std::index_sequence<Indices...>) {
        (
            [&]() {
                if constexpr (Indices < InlineItems) {
                    ptr_inline[Indices] = std::forward<Ts>(args);
                } else {
                    ptr_extras[Indices - InlineItems] = std::forward<Ts>(args);
                }
            }(),
            ...);
    }(std::make_index_sequence<N>());
    detail::AST_privates::tree(ast)[mempos.node_index] = item;
    return res;
}

template <class TOwner, bool IsReverse>
struct iterator_templated {
private:
    static constexpr bool IsConst = std::is_same_v<TOwner, const TOwner>;
    using T = decltype(std::declval<const TOwner>().operator[](0U));
    using ItemRefWrapper = typename TOwner::ItemRefWrapper;
    friend iterator_templated<const TOwner, IsReverse>;

public:
    iterator_templated(const iterator_templated&) = default;
    iterator_templated(iterator_templated&&) = default;
    iterator_templated& operator=(const iterator_templated&) = delete;
    iterator_templated& operator=(iterator_templated&&) = delete;
    ~iterator_templated() = default;
    iterator_templated(const iterator_templated<std::remove_const_t<TOwner>, IsReverse>& other)
        requires(IsConst)
        : owner{other.owner}, index{other.index} {}
    using iterator_category = std::random_access_iterator_tag;
    using difference_type = int32_t;
    using pointer = T*;
    using reference = std::conditional_t<IsConst, T, ItemRefWrapper>;
    using value_type = T;

private:
    TOwner& owner;
    uint32_t index;

    static constexpr int IndexIncrement = (!IsReverse) * 2 - 1;

    iterator_templated(TOwner& owner, uint32_t index) : owner{owner}, index{index} {}
    friend std::remove_const_t<TOwner>;

public:
    reference operator*() const { return owner[index - !!IsReverse]; }
    iterator_templated& operator--() {
        index -= IndexIncrement;
        return *this;
    }
    iterator_templated operator--(int) {
        auto res = *this;
        index -= IndexIncrement;
        return res;
    }
    iterator_templated& operator++() {
        index += IndexIncrement;
        return *this;
    }
    iterator_templated operator++(int) {
        auto res = *this;
        index += IndexIncrement;
        return res;
    }
    iterator_templated& operator+=(difference_type d) {
        index += IndexIncrement * static_cast<int32_t>(d);
        return *this;
    }
    iterator_templated& operator-=(difference_type d) {
        index -= IndexIncrement * static_cast<int32_t>(d);
        return *this;
    }
    iterator_templated operator+(difference_type d) const { return iterator_templated{*this} += d; }
    iterator_templated operator-(difference_type d) const { return iterator_templated{*this} -= d; }
    std::strong_ordering operator<=>(const iterator_templated& other) const {
        if constexpr (IsReverse)
            return other.index <=> index;
        else
            return index <=> other.index;
    }
    bool operator==(const iterator_templated& other) const { return index == other.index; }
    bool operator!=(const iterator_templated& other) const { return index != other.index; }
};

template <concepts::is_ast TAst, class T>
class ArrayOfScalars {
    static constexpr bool Is8ByteAligned = sizeof(T) == 8;
    static constexpr uint32_t LongListInlineTs = sizeof(uint32_t) * 2 / sizeof(T);
    static constexpr uint32_t ShortListInlineTs =
        sizeof(uint32_t) * (3 - Is8ByteAligned) / sizeof(T);

protected:
    TAst& ast;

private:
    detail::ASTNodeItem fields alignas(8){};  // {size, {data|extra_ptr, data, data}}

protected:
    uint32_t myindex;

private:
    uint32_t extras_index() const { return fields.children[0]; }
    uint32_t& extras_index() { return fields.children[0]; }
    T* extras() const {
        return reinterpret_cast<T*>(detail::AST_privates::extra(ast).data() + extras_index());
    }
    bool is_short_list() const { return is_short_list(size()); }
    static constexpr bool is_short_list(uint32_t size) { return size <= ShortListInlineTs; }
    detail::ASTNodeItem& ast_tree_storage() { return detail::AST_privates::tree(ast)[myindex]; }
    std::pair<T&, T*> get_by_index_transparent(uint32_t index) {
        using result_t = std::pair<T&, T*>;
        if (is_short_list())
            return {reinterpret_cast<T*>(&fields.children[Is8ByteAligned])[index],
                    reinterpret_cast<T*>(&ast_tree_storage().children[Is8ByteAligned]) + index};
        else
            return index < ShortListInlineTs
                       ? result_t(reinterpret_cast<T*>(&fields.children[1])[index],
                                  reinterpret_cast<T*>(&ast_tree_storage().children[1]) + index)
                       : result_t(extras()[index - ShortListInlineTs], nullptr);
    }
    T get_by_index(uint32_t index) const {
        if (is_short_list())
            return reinterpret_cast<const T*>(&fields.children[Is8ByteAligned])[index];
        else
            return index < ShortListInlineTs
                       ? reinterpret_cast<const T*>(&fields.children[1])[index]
                       : extras()[index - ShortListInlineTs];
    }

public:
    ArrayOfScalars(TAst& ast, uint32_t index)
        : ast{ast}, fields{AST_privates::tree(ast)[index]}, myindex{index} {}
    ArrayOfScalars(TAst& ast, detail::ASTNodeItem fields, uint32_t index)
        : ast{ast}, fields{fields}, myindex{index} {}

    using iterator = iterator_templated<ArrayOfScalars, false>;
    using const_iterator = iterator_templated<const ArrayOfScalars, false>;
    using reverse_iterator = iterator_templated<ArrayOfScalars, true>;
    using const_reverse_iterator = iterator_templated<const ArrayOfScalars, true>;

    struct ItemRefWrapper {
        T& item;
        T* opt_storage_ptr;
        operator T() const { return item; }
        T read() const { return item; }
        ItemRefWrapper& operator=(T other) {
            item = other;
            if (opt_storage_ptr) *opt_storage_ptr = other;
            return *this;
        }
        ItemRefWrapper& assign(T other) { return *this = other; }

    private:
        friend ArrayOfScalars;
        friend iterator;
        explicit ItemRefWrapper(T& item) : item{item}, opt_storage_ptr{nullptr} {}
        ItemRefWrapper(T& item, T* storage_ptr) : item{item}, opt_storage_ptr{storage_ptr} {}
        explicit ItemRefWrapper(std::pair<T&, T*> p) : ItemRefWrapper(p.first, p.second) {}
    };

    uint32_t size() const { return fields.info; }
    bool empty() const { return !fields.info; }
    ItemRefWrapper at(uint32_t index) {
        if (index >= size()) return std::range_error("AST List access: index out of range");
        return (*this)[index];
    }
    ItemRefWrapper operator[](uint32_t index) {
        return ItemRefWrapper(get_by_index_transparent(index));
    }
    T at(uint32_t index) const {
        if (index >= size()) return std::range_error("AST List access: index out of range");
        return (*this)[index];
    }
    T operator[](uint32_t index) const { return get_by_index(index); }

    iterator begin() { return {*this, 0}; }
    iterator end() { return {*this, size()}; }
    const_iterator begin() const { return {*this, 0}; }
    const_iterator end() const { return {*this, size()}; }
    const_iterator cbegin() const { return {*this, 0}; }
    const_iterator cend() const { return {*this, size()}; }
    reverse_iterator rbegin() { return {*this, 0}; }
    reverse_iterator rend() { return {*this, size()}; }
    const_reverse_iterator rbegin() const { return {*this, 0}; }
    const_reverse_iterator rend() const { return {*this, size()}; }
    const_reverse_iterator crbegin() const { return {*this, 0}; }
    const_reverse_iterator crend() const { return {*this, size()}; }

    template <class U>
    U& annotation() {
        return AST_privates::annots<U>(this->ast)[myindex];
    }
    template <class U>
    const U& annotation() const {
        return AST_privates::annots<U>(this->ast)[myindex];
    }

    static ASTNewNodeInfo reserve_space(TAst& ast, uint32_t size) {
        if (is_short_list(size))
            return detail::AST_privates::emplace_back(ast, 0, 0, Is8ByteAligned);
        return detail::AST_privates::emplace_back(ast, size - LongListInlineTs, 0, Is8ByteAligned);
    }

    template <std::ranges::sized_range Range>
        requires(std::same_as<std::ranges::range_value_t<Range>, T>)
    ArrayOfScalars(TAst& ast, ASTNewNodeInfo reserved_space, Range&& range) : ast{ast} {
        const uint32_t n = fields.info = std::size(range);
        if (!is_short_list(n)) extras_index() = reserved_space.extras_index;
        myindex = reserved_space.node_index;
        auto my_iter = begin();
        for (auto&& i : std::forward<Range>(range)) {
            *my_iter = i;
            ++my_iter;
        }
        detail::AST_privates::tree(ast)[reserved_space.node_index] = fields;
    }

    template <std::ranges::sized_range Range>
        requires(std::same_as<std::ranges::range_value_t<Range>, T>)
    ArrayOfScalars(TAst& ast, Range&& range)
        : ArrayOfScalars(ast, reserve_space(ast, std::ranges::size(range)),
                         std::forward<Range>(range)) {}
    template <std::same_as<T>... Items>
    ArrayOfScalars(TAst& ast, Items... items) : ast{ast} {
        auto [fields, mempos] = detail::emplace_scalar_array(ast, items...);
        this->fields = fields;
        this->myindex = mempos.node_index;
    }
    template <std::ranges::range Range>
        requires(std::same_as<std::ranges::range_value_t<Range>, T> &&
                 !std::ranges::sized_range<Range>)
    ArrayOfScalars(TAst& ast, Range&& range)
        : ArrayOfScalars(ast, std::vector(std::forward<Range>(range).begin(),
                                          std::forward<Range>(range).end())) {}
    template <std::ranges::range Range>
        requires(std::same_as<std::ranges::range_value_t<Range>, T> &&
                 !std::ranges::sized_range<Range>)
    ArrayOfScalars(TAst& ast, ASTNewNodeInfo reserved_space, Range&& range)
        : ArrayOfScalars(
              ast, reserved_space,
              std::vector(std::forward<Range>(range).begin(), std::forward<Range>(range).end())) {}
};

}  // namespace detail

template <concepts::is_ast TAst, concepts::is_ast_ptr Item>
class Array<TAst, Item> : private detail::ArrayOfScalars<TAst, uint32_t> {
    using base_t = detail::ArrayOfScalars<TAst, uint32_t>;

private:
    using base_t::base_t;
    using base_t::reserve_space;
    friend detail::AnyAstNode_privates;

public:
    using base_t::empty;
    using base_t::size;

    TAst& ast() const { return base_t::ast; }

    ASTPtr<Array> pointer() const {
        return detail::ASTPtr_privates::Create<ASTPtr<Array>>(this->myindex);
    }
    template <lib::concepts::is_ast NewAst>
    Array<NewAst, detail::astptr_with_updated_annotations_t<Item, NewAst>> update_annotations(
        NewAst& new_ast) const {
        return detail::AnyAstNode_privates::create_array<
            Array<NewAst, detail::astptr_with_updated_annotations_t<Item, NewAst>>>(
            new_ast, this->fields, this->myindex);
    }

    using iterator = detail::iterator_templated<Array, false>;
    using const_iterator = detail::iterator_templated<const Array, false>;
    using reverse_iterator = detail::iterator_templated<Array, true>;
    using const_reverse_iterator = detail::iterator_templated<const Array, true>;

    struct ItemRefWrapper {
    private:
        friend Array;
        friend iterator;
        friend reverse_iterator;
        base_t::ItemRefWrapper w;
        ItemRefWrapper(base_t::ItemRefWrapper w) : w{w} {}

    public:
        ItemRefWrapper& operator=(Item ptr) {
            w = detail::ASTPtr_privates::Index(ptr);
            return *this;
        }
        operator Item() const { return read(); }
        Item read() const {
            return detail::ASTPtr_privates::Create<Item>(static_cast<uint32_t>(w));
        }
        void assign(Item val) { (*this) = val; }
    };

    Item at(uint32_t index) const {
        return detail::ASTPtr_privates::Create<Item>(base_t::at(index));
    }
    ItemRefWrapper at(uint32_t index) { return {base_t::at(index)}; }
    Item operator[](uint32_t index) const {
        return detail::ASTPtr_privates::Create<Item>(base_t::operator[](index));
    }
    ItemRefWrapper operator[](uint32_t index) { return {base_t::operator[](index)}; }

    iterator begin() { return {*this, 0}; }
    iterator end() { return {*this, size()}; }
    const_iterator begin() const { return {*this, 0}; }
    const_iterator end() const { return {*this, size()}; }
    const_iterator cbegin() const { return {*this, 0}; }
    const_iterator cend() const { return {*this, size()}; }
    reverse_iterator rbegin() { return {*this, size()}; }
    reverse_iterator rend() { return {*this, 0}; }
    const_reverse_iterator rbegin() const { return {*this, size()}; }
    const_reverse_iterator rend() const { return {*this, 0}; }
    const_reverse_iterator crbegin() const { return {*this, size()}; }
    const_reverse_iterator crend() const { return {*this, 0}; }

private:
    static constexpr auto ExtractIndicesFromPointers =
        std::ranges::views::transform(detail::ASTPtr_privates::Index<Item>);
    template <std::ranges::range Range>
        requires(std::same_as<std::ranges::range_value_t<Range>, Item>)
    Array(TAst& ast, detail::ASTNewNodeInfo reserved_space, Range&& range)
        : base_t(ast, reserved_space, std::forward<Range>(range) | ExtractIndicesFromPointers) {}

public:
    template <std::ranges::range Range>
        requires(std::same_as<std::ranges::range_value_t<Range>, Item>)
    Array(TAst& ast, Range&& range)
        : base_t(ast, std::forward<Range>(range) | ExtractIndicesFromPointers) {}
    Array(TAst& ast, std::initializer_list<Item> items)
        : base_t(ast, items | ExtractIndicesFromPointers) {}
    template <std::same_as<Item>... Items>
    Array(TAst& ast, Items... items) : base_t(ast, detail::ASTPtr_privates::Index(items)...) {}
};

template <concepts::is_ast TAst, concepts::is_ast_scalar_token Token>
class Array<TAst, Token> : private detail::ArrayOfScalars<TAst, Token> {
    using base_t = detail::ArrayOfScalars<TAst, Token>;

private:
    using base_t::base_t;
    using base_t::reserve_space;
    friend detail::AnyAstNode_privates;

public:
    TAst& ast() const { return base_t::ast; }

    ASTPtr<Array> pointer() const {
        return detail::ASTPtr_privates::Create<ASTPtr<Array>>(this->myindex);
    }
    template <lib::concepts::is_ast NewAst>
    Array<NewAst, Token> update_annotations(NewAst& new_ast) const {
        return detail::AnyAstNode_privates::create_array<Array<NewAst, Token>>(
            new_ast, this->fields, this->myindex);
    }

    using base_t::at;
    using base_t::ItemRefWrapper;
    using base_t::iterator;
    using base_t::operator[];
    using base_t::begin;
    using base_t::cbegin;
    using base_t::cend;
    using base_t::crbegin;
    using base_t::crend;
    using base_t::empty;
    using base_t::end;
    using base_t::rbegin;
    using base_t::rend;
    using base_t::size;

    template <std::ranges::range Range>
        requires(std::same_as<std::ranges::range_value_t<Range>, Token>)
    Array(TAst& ast, Range&& range) : base_t(ast, std::forward<Range>(range)) {}
    Array(TAst& ast, std::initializer_list<Token> items) : base_t(ast, items) {}
    template <std::same_as<Token>... Tokens>
    Array(TAst& ast, Tokens... tks) : base_t(ast, tks...) {}
};

template <concepts::is_ast TAst>
class Array<TAst, std::string> {
    // ast node: {size, {strings_ptr, 0, 0}}
    TAst& m_ast;
    uint32_t m_size;
    uint32_t strings_ptr;
    uint32_t myindex;
    std::vector<std::string>& string_token_vector() const {
        return detail::AST_privates::string_tokens(m_ast);
    }
    auto strings() const { return string_token_vector().begin() + strings_ptr; }
    Array(TAst& ast, uint32_t index) : Array(ast, ast.tree[index], index) {}
    Array(TAst& ast, detail::ASTNodeItem data, uint32_t index)
        : m_ast{ast}, m_size{data.info}, strings_ptr{data.children[0]}, myindex{index} {}
    Array(TAst& ast, uint32_t size, uint32_t strings_ptr, uint32_t index)
        : m_ast{ast}, m_size{size}, strings_ptr{strings_ptr}, myindex{index} {}

    friend detail::AnyAstNode_privates;

public:
    TAst& ast() const { return m_ast; }

    using iterator = std::vector<std::string>::iterator;
    using const_iterator = std::vector<std::string>::const_iterator;
    using reverse_iterator = std::vector<std::string>::reverse_iterator;
    using const_reverse_iterator = std::vector<std::string>::const_reverse_iterator;

    uint32_t size() const { return m_size; }
    bool empty() const { return !m_size; }
    std::string& at(uint32_t index) {
        if (index >= size())
            throw std::range_error("Array<*, std::string> access: index out of range");
        return strings()[index];
    }
    const std::string& at(uint32_t index) const {
        if (index >= size())
            throw std::range_error("Array<*, std::string> access: index out of range");
        return strings()[index];
    }
    std::string& operator[](uint32_t index) { return strings()[index]; }
    const std::string& operator[](uint32_t index) const { return strings()[index]; }

    iterator begin() { return strings(); }
    iterator end() { return strings() + size(); }
    const_iterator begin() const { return string_token_vector().cbegin() + strings_ptr; }
    const_iterator end() const { return string_token_vector().cbegin() + (strings_ptr + m_size); }
    const_iterator cbegin() const { return string_token_vector().cbegin() + strings_ptr; }
    const_iterator cend() const { return string_token_vector().cbegin() + (strings_ptr + m_size); }
    reverse_iterator rbegin() { return string_token_vector().rend() - (strings_ptr + m_size); }
    reverse_iterator rend() { return string_token_vector().rend() - strings_ptr; }
    const_reverse_iterator rbegin() const {
        return string_token_vector().crend() - (strings_ptr + m_size);
    }
    const_reverse_iterator rend() const { return string_token_vector().crend() - strings_ptr; }
    const_reverse_iterator crbegin() const {
        return string_token_vector().crend() - (strings_ptr + m_size);
    }
    const_reverse_iterator crend() const { return string_token_vector().crend() - strings_ptr; }

    ASTPtr<Array> pointer() const { return lib::detail::ASTPtr_privates::Create<Array>(myindex); }
    template <class T>
    T& annotation() {
        return this->m_ast.template annotation<T>(pointer());
    }
    template <class T>
    const T& annotation() const {
        return this->m_ast.template annotation<T>(pointer());
    }
    template <lib::concepts::is_ast NewAst>
    Array<NewAst, std::string> update_annotations(NewAst& new_ast) const {
        return detail::AnyAstNode_privates::create_array<Array<NewAst, std::string>>(
            new_ast, m_size, strings_ptr, myindex);
    }

private:
    struct private_tag_t {};

    static detail::ASTNewNodeInfo reserve_space(TAst& ast, uint32_t size) {
        return detail::AST_privates::emplace_back(ast, 0, size, false);
    }

    template <std::ranges::sized_range Range>
        requires(std::same_as<std::ranges::range_value_t<Range>, std::string>)
    Array(TAst& ast, detail::ASTNewNodeInfo reserved_space, Range&& range)
        : m_ast{ast}, m_size{std::ranges::size(range)} {
        myindex = reserved_space.node_index;
        strings_ptr = reserved_space.strings_index;
        std::ranges::copy(
            std::forward<Range>(range),
            detail::AST_privates::string_tokens(ast).begin() + reserved_space.strings_index);
        detail::AST_privates::tree(ast)[myindex] = {size(), {strings_ptr, 0, 0}};
    }

    template <std::ranges::sized_range Range>
        requires(std::same_as<std::ranges::range_value_t<Range>, std::string>)
    Array(TAst& ast, Range&& range, private_tag_t)
        : Array(ast, reserve_space(ast, std::ranges::size(range)), std::forward<Range>(range)) {}

    struct MovableStrings {
        using container_t = std::vector<std::string>;
        container_t v;

        template <std::ranges::sized_range Range>
        MovableStrings(Range&& range) {
            v.reserve(std::ranges::size(range));
            v.insert(v.end(), std::forward<Range>(range).begin(), std::forward<Range>(range).end());
        }
        template <std::ranges::range Range>
            requires(!std::ranges::sized_range<Range>)
        MovableStrings(Range&& range)
            : v(std::forward<Range>(range).begin(), std::forward<Range>(range).end()) {}

        auto begin() { return std::make_move_iterator(v.begin()); }
        auto end() { return std::make_move_iterator(v.end()); }
        size_t size() const { return v.size(); }
    };

public:
    template <std::ranges::sized_range Range>
        requires(std::same_as<std::ranges::range_value_t<Range>, std::string>)
    Array(TAst& ast, Range&& range) : Array(ast, std::forward<Range>(range), private_tag_t{}) {}
    template <std::ranges::range Range>
        requires(std::same_as<std::ranges::range_value_t<Range>, std::string> &&
                 !std::ranges::sized_range<Range>)
    Array(TAst& ast, Range&& range)
        : Array(ast, MovableStrings(std::forward<Range>(range)), private_tag_t{}) {}
    Array(TAst& ast, std::initializer_list<std::string> lst) : Array(ast, lst, private_tag_t{}) {}
    Array(TAst& ast, std::initializer_list<std::string_view> lst)
        : Array(ast, MovableStrings(lst), private_tag_t{}) {}
    Array(TAst& ast, std::initializer_list<const char*> lst)
        : Array(ast, MovableStrings(lst), private_tag_t{}) {}
};

template <concepts::is_ast_array TArray>
class ASTPtr<TArray> {
    uint32_t index;
    ASTPtr(uint32_t index) : index{index} {}
    friend detail::ASTPtr_privates;

public:
    template <concepts::is_ast TAst>
    TArray get(TAst& ast) {
        return ast.get(*this);
    }
    template <concepts::detail::default_constructible TAnnot, class... TAnnots>
        requires concepts::detail::same_as_any_of<TAnnot, TAnnots...>
    TAnnot& annotation(AST<TAnnots...>& ast) {
        return ast.template annotation<TAnnot>(*this);
    }
    template <concepts::detail::default_constructible TAnnot, class... TAnnots>
        requires concepts::detail::same_as_any_of<TAnnot, TAnnots...>
    const TAnnot& annotation(const AST<TAnnots...>& ast) {
        return ast.template annotation<TAnnot>(*this);
    }
    template <concepts::is_ast TNewAst>
    detail::astptr_with_updated_annotations_t<ASTPtr, TNewAst> update_annotations() const {
        return detail::ASTPtr_privates::Create<
            detail::astptr_with_updated_annotations_t<ASTPtr, TNewAst>>(index);
    }
    template <concepts::is_ast TNewAst>
    detail::astptr_with_updated_annotations_t<ASTPtr, TNewAst> update_annotations(
        const TNewAst&) const {
        return update_annotations<TNewAst>();
    }
};

namespace traits {
template <concepts::is_ast_array TArray>
struct ast_class_type<ASTPtr<TArray>> {
    static constexpr ASTClassType value = ASTClassType::ArrayPtr;
};
}  // namespace traits

/**************/
/* BNF Labels */
/**************/

namespace detail {

enum class LabelFieldLocation : char { Inline, Extra };
using LabelFieldBytePos = std::pair<LabelFieldLocation, size_t>;

static consteval LabelFieldBytePos label_field_pos_advance(LabelFieldBytePos pos, size_t delta,
                                                           size_t maxinlinepos) {
    pos.second += delta;
    if (pos.first == LabelFieldLocation::Inline && pos.second > maxinlinepos)
        return {LabelFieldLocation::Extra, 0UL};
    return pos;
}

static_assert(sizeof(Double) == 8);
static_assert(sizeof(Integer) == 8);
static_assert(sizeof(Char) == 4);
static_assert(sizeof(Bool) == 1);

template <bool TStoresTag, size_t NPtrs, size_t NStrings, size_t NDoubles, size_t NInts,
          size_t NChars, size_t NBools>
struct LabelLayout {
public:
    using Location = LabelFieldLocation;
    using BytePos = LabelFieldBytePos;

private:
    static constexpr size_t InlineSpaceAssumingNoExtra = sizeof(detail::ASTNodeItem) -
                                                         !!TStoresTag * sizeof(uint32_t) -
                                                         !!NStrings * sizeof(uint32_t);
    static constexpr size_t TotalSpaceTaken = NPtrs * sizeof(uint32_t) + NDoubles * sizeof(Double) +
                                              NInts * sizeof(Integer) + NChars * sizeof(Char) +
                                              (NBools * sizeof(Bool) + 3) / 4 * 4;
    static constexpr size_t InlineSpace =
        InlineSpaceAssumingNoExtra - !!(InlineSpaceAssumingNoExtra < TotalSpaceTaken) * 4;

    using scalars_layout_t =
        std::tuple<std::array<LabelFieldBytePos, NPtrs>, std::array<LabelFieldBytePos, NDoubles>,
                   std::array<LabelFieldBytePos, NInts>, std::array<LabelFieldBytePos, NChars>,
                   std::array<LabelFieldBytePos, NBools>>;
    static consteval std::tuple<scalars_layout_t, size_t, int, bool> make_scalars_layout() {
        scalars_layout_t res;
        size_t inline_pos = sizeof(detail::ASTNodeItem) - InlineSpace;
        size_t iptr = 0;
        size_t idouble = 0;
        size_t iint = 0;
        size_t ichar = 0;
        size_t ibool = 0;
        // Tag (at pos 0) is not needed in the label
        int place_to_store_node_index = TStoresTag ? 0 : -1;
        while (inline_pos < sizeof(detail::ASTNodeItem)) {
            if (iptr < NPtrs) {
                get<0>(res)[iptr++] = {Location::Inline, inline_pos};
                inline_pos += sizeof(uint32_t);
                continue;
            }
            if (inline_pos % 8 == 0) {
                if (iint < NInts) {
                    get<2>(res)[iint++] = {Location::Inline, inline_pos};
                    inline_pos += sizeof(Integer);
                    continue;
                }
                if (idouble < NDoubles) {
                    get<1>(res)[idouble++] = {Location::Inline, inline_pos};
                    inline_pos += sizeof(Double);
                    continue;
                }
            }
            if (ichar < NChars) {
                get<3>(res)[ichar++] = {Location::Inline, inline_pos};
                inline_pos += sizeof(Char);
                continue;
            }
            if (ibool < NBools) {
                size_t boolcount = std::min(4UL, NBools - ibool);
                auto& bool_array = get<4>(res);
                for (size_t i = 0; i < boolcount; i++)
                    bool_array[ibool + i] = {Location::Inline, inline_pos + i};
                ibool += boolcount;
                inline_pos += 4UL;
                continue;
            }
            if (place_to_store_node_index == -1) place_to_store_node_index = inline_pos;
            inline_pos += 4UL;
        }
        size_t extra_pos = 0;
        bool needs_8byte_align = iint < NInts || idouble < NDoubles;
        for (; iint < NInts; iint++) {
            get<2>(res)[iint] = {Location::Extra, extra_pos};
            extra_pos += sizeof(Integer);
        }
        for (; idouble < NDoubles; idouble++) {
            get<1>(res)[idouble] = {Location::Extra, extra_pos};
            extra_pos += sizeof(Double);
        }
        for (; iptr < NPtrs; iptr++) {
            get<0>(res)[iptr] = {Location::Extra, extra_pos};
            extra_pos += sizeof(uint32_t);
        }
        for (; ichar < NChars; ichar++) {
            get<3>(res)[ichar] = {Location::Extra, extra_pos};
            extra_pos += sizeof(Char);
        }
        for (; ibool < NBools; ibool++) {
            get<4>(res)[ibool] = {Location::Extra, extra_pos};
            extra_pos += sizeof(Bool);
        }
        return {res, extra_pos, place_to_store_node_index, needs_8byte_align};
    }
    static constexpr std::tuple<scalars_layout_t, size_t, int, bool> ScalarsResult_ =
        make_scalars_layout();
    static constexpr scalars_layout_t ScalarsLayout = get<0>(ScalarsResult_);

public:
    static constexpr bool StoresTag = TStoresTag;
    static constexpr size_t TagOffset()
        requires StoresTag
    {
        return 0UL;
    }
    static constexpr size_t StringPtrOffset()
        requires(NStrings > 0)
    {
        return 4UL * !!TStoresTag;
    }
    static constexpr bool Needs8ByteAlignment = get<3>(ScalarsResult_);
    static constexpr size_t StringCount = NStrings;
    static constexpr size_t ExtraSpaceInts = (get<1>(ScalarsResult_) + 3) / 4;
    static constexpr int PlaceToStoreNodeIndex = get<2>(ScalarsResult_);
    static constexpr size_t ExtraPtrOffset()
        requires(ExtraSpaceInts > 0)
    {
        return 4UL * (!!TStoresTag + !!NStrings);
    }
    template <class TScalar>
    static consteval BytePos ScalarPos(size_t index) {
        constexpr int tuple_index = pack_find_v<TScalar, Double, Integer, Char, Bool>;
        static_assert(tuple_index >= 0, "Bad scalar");
        return get<tuple_index + 1>(ScalarsLayout)[index];
    }
    static constexpr BytePos PointerPos(size_t index) { return get<0>(ScalarsLayout)[index]; }
};

// using l_t = LabelLayout<false, 2, 1, 0, 0, 0, 0>;
// static_assert(l_t::PlaceToStoreNodeIndex == 12);
// static_assert(l_t::Needs8ByteAlignment == false);
// static_assert(l_t::ExtraSpaceInts == 0);
// // static_assert(l_t::ExtraPtrOffset() == 0);
// static_assert(l_t::PointerPos(0) == LabelFieldBytePos{LabelFieldLocation::Inline, 4UL});
// static_assert(l_t::PointerPos(1) == LabelFieldBytePos{LabelFieldLocation::Inline, 8UL});

template <bool Enabled>
struct ConditionalIndex {
private:
    uint32_t index;

public:
    uint32_t& separately_stored_index() { return index; }
    uint32_t separately_stored_index() const { return index; }
};

template <>
struct ConditionalIndex<false> {
public:
    uint32_t separately_stored_index() const { return -1; }
};

template <concepts::is_ast TAst, bool TStoresTag, size_t NPtrs, size_t NStrings, size_t NDoubles,
          size_t NInts, size_t NChars, size_t NBools>
class Label {
public:
    TAst& ast;

    using layout_t = LabelLayout<TStoresTag, NPtrs, NStrings, NDoubles, NInts, NChars, NBools>;

private:
    std::array<char, sizeof(detail::ASTNodeItem)> inline_data alignas(8){};
    ConditionalIndex<layout_t::PlaceToStoreNodeIndex == -1> cond_index [[no_unique_address]];

    template <class TScalar, size_t Pos>
    TScalar get_inline_scalar() const {
        return *reinterpret_cast<const TScalar*>(inline_data.data() + Pos);
    }

    uint32_t extras_offset() const {
        return get_inline_scalar<uint32_t, layout_t::ExtraPtrOffset()>();
    }

    char* extras() const
        requires(layout_t::ExtraSpaceInts > 0)
    {
        return reinterpret_cast<char*>(&AST_privates::extra(ast)[extras_offset()]);
    }

    uint32_t strings_offset() const {
        return get_inline_scalar<uint32_t, layout_t::StringPtrOffset()>();
    }

    uint32_t& mut_index() {
        if constexpr (layout_t::PlaceToStoreNodeIndex == -1)
            return cond_index.separately_stored_index();
        else
            // return get_inline_scalar<uint32_t, layout_t::PlaceToStoreNodeIndex>();
            return *reinterpret_cast<uint32_t*>(inline_data.data() +
                                                layout_t::PlaceToStoreNodeIndex);
    }

    template <class TScalar, LabelFieldBytePos Pos>
    TScalar get_scalar() const {
        if constexpr (Pos.first == LabelFieldLocation::Inline)
            return get_inline_scalar<TScalar, Pos.second>();
        else
            return *reinterpret_cast<TScalar*>(extras() + Pos.second);
    }

public:
    Label(TAst& ast, const decltype(inline_data)& data, decltype(cond_index) cond_index)
        : ast{ast}, inline_data{data}, cond_index{cond_index} {}

    uint32_t index() const {
        if constexpr (layout_t::PlaceToStoreNodeIndex == -1)
            return cond_index.separately_stored_index();
        else
            return get_inline_scalar<uint32_t, layout_t::PlaceToStoreNodeIndex>();
    }

private:
    template <class TScalar, size_t Pos, bool Commit = true>
    void set_inline_scalar(TScalar value) {
        *reinterpret_cast<TScalar*>(inline_data.data() + Pos) = value;
        if constexpr (Commit)
            *reinterpret_cast<TScalar*>(reinterpret_cast<char*>(&AST_privates::tree(ast)[index()]) +
                                        Pos) = value;
    }

    template <class TScalar, LabelFieldBytePos Pos, bool Commit = true>
    void set_scalar(TScalar value) {
        if constexpr (Pos.first == LabelFieldLocation::Inline)
            set_inline_scalar<TScalar, Pos.second, Commit>(value);
        else
            *reinterpret_cast<TScalar*>(extras() + Pos.second) = value;
    }

public:
    using creation_params_t = pack_to_tuple_t<
        concat_packs_t<repeat_pack_t<NPtrs, uint32_t>, repeat_pack_t<NStrings, std::string>,
                       repeat_pack_t<NDoubles, Double>, repeat_pack_t<NInts, Integer>,
                       repeat_pack_t<NChars, Char>, repeat_pack_t<NBools, Bool>>>;

private:
    void initialize(ASTNewNodeInfo new_node_info, uint32_t tag_if_needed, creation_params_t&& vals);

public:
    template <size_t Index>
    std::string& string() const
        requires(NStrings > 0)
    {
        return AST_privates::string_tokens(ast)[strings_offset() + Index];
    }

    template <size_t Index>
    uint32_t pointer() const {
        return get_scalar<uint32_t, layout_t::PointerPos(Index)>();
    }
    template <size_t Index, bool Commit = true>
    void pointer(uint32_t value) {
        set_scalar<uint32_t, layout_t::PointerPos(Index), Commit>(value);
    }
    template <class TScalar, size_t Index>
    TScalar get_scalar_token() const {
        return get_scalar<TScalar, layout_t::template ScalarPos<TScalar>(Index)>();
    }
    template <class TScalar, size_t Index, bool Commit = true>
    void set_scalar_token(TScalar value) {
        set_scalar<TScalar, layout_t::template ScalarPos<TScalar>(Index), Commit>(value);
    }

    Label(const Label&) = default;
    Label(Label&&) = default;
    Label& operator=(const Label&) = delete;
    Label& operator=(Label&&) = delete;
    Label(TAst& ast, uint32_t index)
        : ast{ast},
          inline_data{
              reinterpret_cast<const decltype(inline_data)&>(AST_privates::tree(ast)[index])} {
        mut_index() = index;
    }
    static ASTNewNodeInfo reserve_space(TAst& ast) {
        return AST_privates::emplace_back(ast, layout_t::ExtraSpaceInts, NStrings,
                                          layout_t::Needs8ByteAlignment);
    }
    Label(TAst& ast, uint32_t tag_if_needed, creation_params_t&& vals) : ast{ast} {
        initialize(reserve_space(ast), tag_if_needed, std::move(vals));
    }
    Label(TAst& ast, ASTNewNodeInfo reserved_space, uint32_t tag_if_needed,
          creation_params_t&& vals)
        : ast{ast} {
        initialize(reserved_space, tag_if_needed, std::move(vals));
    }

    template <concepts::is_ast TNewAst>
    Label<TNewAst, TStoresTag, NPtrs, NStrings, NDoubles, NInts, NChars, NBools> update_annotations(
        TNewAst& new_ast) const {
        return Label<TNewAst, TStoresTag, NPtrs, NStrings, NDoubles, NInts, NChars, NBools>(
            new_ast, inline_data, cond_index);
    }

    ~Label() = default;
};

template <concepts::is_ast TAst, bool TStoresTag, size_t NPtrs, size_t NStrings, size_t NDoubles,
          size_t NInts, size_t NChars, size_t NBools>
void Label<TAst, TStoresTag, NPtrs, NStrings, NDoubles, NInts, NChars, NBools>::initialize(
    ASTNewNodeInfo new_node_info, uint32_t tag_if_needed, creation_params_t&& vals) {
    if constexpr (TStoresTag)
        set_inline_scalar<uint32_t, layout_t::TagOffset(), false>(tag_if_needed);
    if constexpr (layout_t::ExtraSpaceInts)
        set_inline_scalar<uint32_t, layout_t::ExtraPtrOffset(), false>(new_node_info.extras_index);
    if constexpr (NStrings)
        set_inline_scalar<uint32_t, layout_t::StringPtrOffset(), false>(
            new_node_info.strings_index);
    {
        [&]<size_t... Indices>(std::index_sequence<Indices...>) {
            (pointer<Indices, false>(std::get<Indices>(vals)), ...);
        }(std::make_index_sequence<NPtrs>());
    }
    static constexpr size_t StringsStart = NPtrs;
    {
        std::string* mystrings =
            AST_privates::string_tokens(ast).data() + new_node_info.strings_index;
        [&]<size_t... Indices>(std::index_sequence<Indices...>) {
            ((mystrings[Indices] = std::move(std::get<StringsStart + Indices>(vals))), ...);
        }(std::make_index_sequence<NStrings>());
    }
    static constexpr size_t ScalarsStart = StringsStart + NStrings;
    static constexpr size_t ScalarKindStarts[] = {
        ScalarsStart,
        ScalarsStart + NDoubles,
        ScalarsStart + NDoubles + NInts,
        ScalarsStart + NDoubles + NInts + NChars,
    };
    [&]<size_t... TokenIndices>(std::index_sequence<TokenIndices...>) {
        (
            [&]() {
                static constexpr size_t AbsIndex = ScalarsStart + TokenIndices;
                auto value = std::get<AbsIndex>(vals);
                static constexpr size_t KindAbsStart = []() consteval {
                    for (size_t i = std::size(ScalarKindStarts); i; --i) {
                        if (ScalarKindStarts[i - 1] <= AbsIndex) return ScalarKindStarts[i - 1];
                    }
                    return 0UL;
                }();
                set_scalar_token<decltype(value), AbsIndex - KindAbsStart, false>(value);
            }(),
            ...);
    }(std::make_index_sequence<NDoubles + NInts + NChars + NBools>());
    AST_privates::tree(ast)[new_node_info.node_index] = reinterpret_cast<ASTNodeItem&>(inline_data);
    mut_index() = new_node_info.node_index;
}

template <concepts::is_ast_label TLabel>
struct label_pointer_fields;

template <concepts::is_ast_label TLabel>
using label_pointer_fields_t = label_pointer_fields<TLabel>::value_t;

template <concepts::is_ast_label TLabel>
struct label_tuple_size;

}  // namespace detail

/**************************/
/* AST method definitions */
/**************************/

template <concepts::detail::default_constructible... TAnnotations>
    requires(detail::pairwise_different_v<TAnnotations...>)
template <class TAnnot, concepts::is_ast_ptr TPtr>
    requires(std::same_as<TAnnot, TAnnotations> || ...)
inline TAnnot& AST<TAnnotations...>::annotation(TPtr ptr)
    requires(sizeof...(TAnnotations) > 0)
{
    return std::get<std::vector<TAnnot>>(annots)[detail::ASTPtr_privates::Index(ptr)];
}

template <concepts::detail::default_constructible... TAnnotations>
    requires(detail::pairwise_different_v<TAnnotations...>)
template <class TAnnot, concepts::is_ast_ptr TPtr>
    requires(std::same_as<TAnnot, TAnnotations> || ...)
inline const TAnnot& AST<TAnnotations...>::annotation(TPtr ptr) const
    requires(sizeof...(TAnnotations) > 0)
{
    return std::get<std::vector<TAnnot>>(annots)[detail::ASTPtr_privates::Index(ptr)];
}

template <concepts::detail::default_constructible... TAnnotations>
    requires(detail::pairwise_different_v<TAnnotations...>)
template <concepts::is_ast_label TLabel, concepts::is_ast_label... TLs>
    requires(concepts::detail::same_as_any_of<TLabel, TLs...> && sizeof...(TLs) > 1)
inline TLabel AST<TAnnotations...>::unchecked_get(ASTPtr<TLs...> ptr) {
    return detail::AnyAstNode_privates::create_label<TLabel>(*this,
                                                             detail::ASTPtr_privates::Index(ptr));
}

template <concepts::detail::default_constructible... TAnnotations>
    requires(detail::pairwise_different_v<TAnnotations...>)
template <concepts::is_ast_label TLabel, concepts::is_ast_label... TLs>
    requires(concepts::detail::same_as_any_of<TLabel, TLs...>)
inline std::optional<TLabel> AST<TAnnotations...>::checked_get(ASTPtr<TLs...> ptr) {
    if constexpr (sizeof...(TLs) == 1)
        return get(ptr);
    else {
        uint32_t index = detail::ASTPtr_privates::Index(ptr);
        static constexpr int NeedTag = detail::pack_find_v<TLabel, TLs...>;
        if (NeedTag != -1 && (int)tree[index].info == NeedTag)
            return {detail::AnyAstNode_privates::create_label<TLabel>(*this, index)};
        else
            return {};
    }
}

template <concepts::detail::default_constructible... TAnnotations>
    requires(detail::pairwise_different_v<TAnnotations...>)
template <concepts::is_ast_label TLabel>
inline TLabel AST<TAnnotations...>::get(ASTPtr<TLabel> ptr) {
    return detail::AnyAstNode_privates::create_label<TLabel>(*this,
                                                             detail::ASTPtr_privates::Index(ptr));
}

template <concepts::detail::default_constructible... TAnnotations>
    requires(detail::pairwise_different_v<TAnnotations...>)
template <concepts::is_ast_array TArray>
inline TArray AST<TAnnotations...>::get(ASTPtr<TArray> ptr) {
    return detail::AnyAstNode_privates::create_array<TArray>(*this,
                                                             detail::ASTPtr_privates::Index(ptr));
}

namespace detail {
namespace {
template <concepts::is_ast_ptr TPtr>
[[maybe_unused]] inline void copy_annots_for_node(concepts::is_ast auto& source_ast,
                                                  concepts::is_ast auto& dest_ast, TPtr source_ptr,
                                                  TPtr dest_ptr) {
    [&]<class... SourceTypes, class... DestTypes>(const AST<SourceTypes...>&,
                                                  const AST<DestTypes...>&) {
        (
            [&] {
                if constexpr (pack_find_v<DestTypes, SourceTypes...> != -1) {
                    dest_ast.template annotation<DestTypes>(dest_ptr) =
                        source_ast.template annotation<DestTypes>(source_ptr);
                }
            }(),
            ...);
    }(source_ast, dest_ast);
}
}  // namespace
}  // namespace detail

enum class DFSOrder { PreOrder, PostOrder };

template <DFSOrder TOrder = DFSOrder::PreOrder, bool TMoveStrings = false,
          concepts::is_ast TSourceAst, concepts::is_ast TDestAst, concepts::is_ast_ptr TPtr>
TPtr deep_copy(TSourceAst& source_ast, TDestAst& dest_ast, TPtr ptr) {
    auto rec_copy = [&](auto&& obj) -> auto {  // return by value
        if constexpr (concepts::is_ast_ptr<std::remove_cvref_t<decltype(obj)>>)
            return deep_copy<TOrder, TMoveStrings>(source_ast, dest_ast, obj);
        else
            return std::forward<decltype(obj)>(obj);
    };
    auto res = source_ast.visit(ptr)([&](auto obj) {
        using obj_t = decltype(obj);
        if constexpr (concepts::is_ast_label<obj_t>) {
            static constexpr size_t argcount = std::tuple_size_v<obj_t>;
            return [&]<size_t... Indices>(std::index_sequence<Indices...>) {
                detail::ASTNewNodeInfo reserved_space{};
                if constexpr (TOrder == DFSOrder::PreOrder)
                    reserved_space = detail::AnyAstNode_privates::reserve_space<obj_t>(dest_ast);
                // copies_or_refs is evaluated in the correct order
                // Makes copies of strings if TMoveStrings = false and moves them otherwise.
                std::tuple<std::remove_cvref_t<std::tuple_element_t<Indices, obj_t>>...>
                    copies_or_refs{
                        rec_copy(std::move(obj.template get<Indices, TMoveStrings>()))...};
                if constexpr (TOrder == DFSOrder::PreOrder)
                    return detail::AnyAstNode_privates::create_label<obj_t>(
                               dest_ast, reserved_space,
                               std::move(std::get<Indices>(copies_or_refs))...)
                        .pointer();
                else
                    return obj_t(dest_ast, std::move(std::get<Indices>(copies_or_refs))...)
                        .pointer();
            }(std::make_index_sequence<argcount>());
        } else {  // array
            // TODO test this
            auto make_optionally_movable = [](auto& obj) -> decltype(auto) {
                if constexpr (TMoveStrings &&
                              std::is_same_v<traits::ast_array_value_t<obj_t>, std::string>)
                    return std::ranges::subrange(std::make_move_iterator(obj.begin()),
                                                 std::make_move_iterator(obj.end()));
                else
                    return obj;
            };
            if constexpr (TOrder == DFSOrder::PreOrder) {
                auto reserved_space =
                    detail::AnyAstNode_privates::reserve_space<obj_t>(dest_ast, obj.size());
                return detail::AnyAstNode_privates::create_array<obj_t>(
                           dest_ast, reserved_space,
                           make_optionally_movable(obj) | std::views::transform(rec_copy))
                    .pointer();
            } else
                return obj_t(dest_ast,
                             make_optionally_movable(obj) | std::views::transform(rec_copy))
                    .pointer();
        }
    });
    detail::copy_annots_for_node(source_ast, dest_ast, ptr, res);
    return res;
}

template <bool TMoveStrings = false, concepts::is_ast TAst, concepts::is_ast_ptr TPtr>
TPtr shallow_copy(TAst& ast, TPtr ptr) {
    auto res = ast.visit(ptr)([&](auto obj) {
        using obj_t = decltype(obj);
        if constexpr (concepts::is_ast_label<obj_t>) {
            static constexpr size_t argcount = std::tuple_size_v<obj_t>;
            [&]<size_t... Indices>(std::index_sequence<Indices...>) {
                return obj_t(ast, std::move(obj.template get<Indices, TMoveStrings>())...)
                    .pointer();
            }(std::make_index_sequence<argcount>());
        } else {  // array
            auto make_optionally_movable = [](auto& obj) -> decltype(auto) {
                if constexpr (TMoveStrings &&
                              std::is_same_v<traits::ast_array_value_t<obj_t>, std::string>)
                    return std::ranges::subrange(std::make_move_iterator(obj.begin()),
                                                 std::make_move_iterator(obj.end()));
                else
                    return obj;
            };
            return obj_t(ast, make_optionally_movable(obj)).pointer();
        }
    });
    detail::copy_annots_for_node(ast, ast, ptr, res);
    return res;
}

template <concepts::detail::default_constructible... TAnnotations>
    requires(detail::pairwise_different_v<TAnnotations...>)
template <concepts::is_ast_ptr TPtr>
inline std::pair<AST<TAnnotations...>, TPtr> AST<TAnnotations...>::preorder(TPtr root) & {
    AST res_ast;
    auto res_ptr = deep_copy<DFSOrder::PreOrder, false>(*this, res_ast, root);
    return {std::move(res_ast), res_ptr};
}

template <concepts::detail::default_constructible... TAnnotations>
    requires(detail::pairwise_different_v<TAnnotations...>)
template <concepts::is_ast_ptr TPtr>
inline std::pair<AST<TAnnotations...>, TPtr> AST<TAnnotations...>::preorder(TPtr root) && {
    AST res_ast;
    auto res_ptr = deep_copy<DFSOrder::PreOrder, true>(*this, res_ast, root);
    return {std::move(res_ast), res_ptr};
}

}  // namespace lib

}  // namespace ast

namespace std {

template <ast::lib::concepts::is_ast_label TLabel>
struct tuple_size<TLabel>
    : public integral_constant<size_t, ast::lib::detail::label_tuple_size<TLabel>::value> {};
template <size_t I, ast::lib::concepts::is_ast_label TLabel>
struct tuple_element<I, TLabel> {
    using type = decltype(std::declval<TLabel>().template get<I>());
};

}  // namespace std
