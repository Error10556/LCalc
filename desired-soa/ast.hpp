#pragma once
// we assume read-write access to the AST (TODO: implement read-only labels)
#include <algorithm>
#include <array>
#include <cstdint>
#include <iterator>
#include <optional>
#include <ranges>
#include <string>
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
};

template <class T>
struct ast_class_type {};

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

}  // namespace traits

namespace concepts {

template <class T>
concept is_ast = traits::is_ast_v<std::remove_cvref_t<T>>;

template <class T>
concept is_ast_variant_ptr =
    traits::ast_class_type_v<std::remove_cvref_t<T>> == traits::ASTClassType::VariantPtr;

template <class T>
concept is_ast_array_ptr =
    traits::ast_class_type_v<std::remove_cvref_t<T>> == traits::ASTClassType::ArrayPtr;

template <class T>
concept is_ast_ptr = is_ast_variant_ptr<T> || is_ast_array_ptr<T>;

template <class T>
concept is_ast_label =
    traits::ast_class_type_v<std::remove_cvref_t<T>> == traits::ASTClassType::Label;

template <class T>
concept is_ast_array =
    traits::ast_class_type_v<std::remove_cvref_t<T>> == traits::ASTClassType::Array;

template <class T>
concept is_ast_node = is_ast_label<T> || is_ast_array<T>;

template <class T>
concept is_ast_scalar_token =
    traits::ast_class_type_v<std::remove_cvref_t<T>> == traits::ASTClassType::ScalarToken;

template <class T>
concept is_ast_string_token =
    traits::ast_class_type_v<std::remove_cvref_t<T>> == traits::ASTClassType::StringToken;

template <class T>
concept is_ast_token = is_ast_string_token<T> || is_ast_scalar_token<T>;

}  // namespace concepts

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
    static std::vector<detail::ASTNodeItem>& tree(TAst&);
    template <concepts::is_ast TAst>
    static const std::vector<detail::ASTNodeItem>& tree(const TAst&);
    template <concepts::is_ast TAst>
    static std::vector<std::string>& string_tokens(TAst&);
    template <concepts::is_ast TAst>
    static const std::vector<std::string>& string_tokens(const TAst&);
    template <concepts::is_ast TAst>
    static std::vector<uint32_t>& extra(TAst&);
    template <concepts::is_ast TAst>
    static const std::vector<uint32_t>& extra(const TAst&);
    template <class TAnnot, class... Annots>
        requires concepts::detail::same_as_any_of<TAnnot, Annots...>
    static std::vector<TAnnot>& annots(AST<Annots...>&);
    template <class TAnnot, class... Annots>
        requires concepts::detail::same_as_any_of<TAnnot, Annots...>
    static const std::vector<TAnnot>& annots(const AST<Annots...>&);

    template <concepts::is_ast TAst>
    static ASTNewNodeInfo emplace_back(TAst&, uint32_t nextra, uint32_t nstrings,
                                       bool needs_8byte_align);
};

struct NodeMemoryPosition {
    uint32_t index;
    std::pair<uint32_t, uint32_t> extras_range;
    std::pair<uint32_t, uint32_t> strings_range;
};

}  // namespace detail

template <concepts::is_ast TAst, concepts::is_ast_variant_ptr... TASTPtr>
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

    detail::ASTNewNodeInfo emplace_back(uint32_t nextra, uint32_t nstrings,
                                        bool needs_8byte_align) {
        uint32_t res_index = tree.size();
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
    friend detail::AST_privates;

public:
    AST() = default;
    template <concepts::detail::same_as_any_of<TAnnotations...>... TOtherAnnots>
    AST(AST<TOtherAnnots...>&& other)
        : tree{std::move(detail::AST_privates::tree(other))},
          string_tokens{std::move(detail::AST_privates::string_tokens(other))},
          extra{std::move(detail::AST_privates::extra(other))},
          annots{transform_annotations(std::move(other).annots, tree.size())} {}
    uint32_t size() { return tree.size(); }
    template <concepts::detail::default_constructible... T>
        requires(sizeof...(T) > 0)
    AST<TAnnotations..., T...> annotate() && {
        return AST<TAnnotations..., T...>(std::move(*this));
    }

    template <class TAnnot, concepts::is_ast_ptr TPtr>
        requires(std::same_as<TAnnot, TAnnotations> || ...)
    TAnnot& annotation(TPtr ptr)
        requires(sizeof...(TAnnotations) > 0);
    template <class TAnnot, concepts::is_ast_ptr TPtr>
        requires(std::same_as<TAnnot, TAnnotations> || ...)
    const TAnnot& annotation(TPtr ptr) const
        requires(sizeof...(TAnnotations) > 0);

    template <concepts::is_ast_variant_ptr... TPtr>
        requires(sizeof...(TPtr) > 0)
    Visit<AST, TPtr...> visit(TPtr... ptrs) {
        return {*this, ptrs...};
    }
    template <concepts::is_ast_label TLabel, concepts::is_ast_label... TLs>
        requires(concepts::detail::same_as_any_of<TLabel, TLs...> && sizeof...(TLs) > 1)
    TLabel unchecked_get(ASTPtr<TLs...> ptr);
    template <concepts::is_ast_label TLabel, concepts::is_ast_label... TLs>
        requires(concepts::detail::same_as_any_of<TLabel, TLs...>)
    std::optional<TLabel> checked_get(ASTPtr<TLs...> ptr);
    template <concepts::is_ast_label TLabel>
    TLabel get(ASTPtr<TLabel> ptr);
    template <concepts::is_ast_array TArray>
    TArray get(ASTPtr<TArray> ptr);

private:
    template <class FuncDataMove>
        requires(std::same_as<std::invoke_result_t<FuncDataMove, detail::NodeMemoryPosition, bool>,
                              detail::ASTNewNodeInfo>)
    class PreorderTransducer {
    private:
        FuncDataMove transduce;
        AST &oldast, &newast;

    public:
        PreorderTransducer(FuncDataMove&& f_transduce, AST& oldast, AST& newast)
            : transduce{std::move(f_transduce)}, oldast{oldast}, newast{newast} {}
        template <concepts::is_ast_label TLabel>
        uint32_t operator()(TLabel);
        template <class TAst, concepts::is_ast_ptr TPtr>
        uint32_t operator()(Array<TAst, TPtr>);
        template <class TAst, concepts::is_ast_token Tk>
        uint32_t operator()(Array<TAst, Tk>);
        template <concepts::is_ast_variant_ptr TPtr>
        uint32_t operator()(TPtr);
        template <concepts::is_ast_array_ptr TPtr>
        uint32_t operator()(TPtr);
    };

    AST new_ast_with_reserved_containers() const {
        AST res;
        const auto n = size();
        res.tree.reserve(n);
        res.extra.reserve(extra.size() + n);
        res.string_tokens.reserve(string_tokens.size());
        [n, res]<size_t... Indices>(std::index_sequence<Indices...>) {
            (std::get<Indices>(res.annots).reserve(n), ...);
        }(std::make_index_sequence<std::size(annots)>());
        return res;
    }

public:
    template <concepts::is_ast_ptr TPtr>
    AST preorder(TPtr& root) &;
    // template <concepts::is_ast_ptr TPtr>
    // AST preorder(TPtr& root) const&;
    template <concepts::is_ast_ptr TPtr>
    AST preorder(TPtr& root) &&;
    // template <concepts::is_ast_ptr TPtr>
    // AST preorder(TPtr& root) const&&;
    // TODO definitions
};

namespace traits {
template <class... Ts>
struct is_ast<AST<Ts...>> {
    static constexpr bool value = true;
};
}  // namespace traits

namespace detail {
template <concepts::is_ast TAst>
inline std::vector<detail::ASTNodeItem>& AST_privates::tree(TAst& ast) {
    return ast.tree;
}
template <concepts::is_ast TAst>
inline const std::vector<detail::ASTNodeItem>& AST_privates::tree(const TAst& ast) {
    return ast.tree;
}
template <concepts::is_ast TAst>
inline std::vector<std::string>& AST_privates::string_tokens(TAst& ast) {
    return ast.string_tokens;
}
template <concepts::is_ast TAst>
inline const std::vector<std::string>& AST_privates::string_tokens(const TAst& ast) {
    return ast.string_tokens;
}
template <concepts::is_ast TAst>
inline std::vector<uint32_t>& AST_privates::extra(TAst& ast) {
    return ast.extra;
}
template <concepts::is_ast TAst>
inline const std::vector<uint32_t>& AST_privates::extra(const TAst& ast) {
    return ast.extra;
}
template <class TAnnot, class... Annots>
    requires concepts::detail::same_as_any_of<TAnnot, Annots...>
inline std::vector<TAnnot>& AST_privates::annots(AST<Annots...>& ast) {
    return std::get<std::vector<TAnnot>>(ast.annots);
}
template <class TAnnot, class... Annots>
    requires concepts::detail::same_as_any_of<TAnnot, Annots...>
inline const std::vector<TAnnot>& AST_privates::annots(const AST<Annots...>& ast) {
    return std::get<std::vector<TAnnot>>(ast.annots);
}
template <concepts::is_ast TAst>
inline ASTNewNodeInfo AST_privates::emplace_back(TAst& ast, uint32_t nextra, uint32_t nstrings,
                                                 bool needs_8byte_align) {
    return ast.emplace_back(nextra, nstrings, needs_8byte_align);
}
}  // namespace detail

/**************************/
/* Node Pointer (general) */
/**************************/

template <class...>
class ASTPtr {
    static_assert(false, "This kind of AST pointer is not supported");
};

namespace detail {
struct ASTPtr_privates {
    template <class... T>
    static uint32_t Index(const ASTPtr<T...> ptr) {
        return ptr.index;
    }
    template <concepts::is_ast_ptr TPtr>
    static TPtr Create(uint32_t index) {
        return TPtr{index};
    }
};
}  // namespace detail

/************************/
/* Variant Node Pointer */
/************************/

template <concepts::is_ast_label... Ts>
class ASTPtr<Ts...> {
    uint32_t index;
    ASTPtr(uint32_t index) : index{index} {}

public:
    // TODO ?
};

template <class... Funcs>
class Cases : private Funcs... {
public:
    using Funcs::operator()...;
    constexpr Cases(Funcs&&... f) : Funcs{std::forward<Funcs>(f)}... {}
};

namespace traits {
template <concepts::is_ast_label... Ts>
struct ast_class_type<ASTPtr<Ts...>> {
    static constexpr ASTClassType value = ASTClassType::VariantPtr;
};
}  // namespace traits

template <concepts::is_ast TAst, concepts::is_ast_variant_ptr... TASTPtr>
class Visit {
    TAst& ast;
    std::array<uint32_t, sizeof...(TASTPtr)> indices;

public:
    constexpr Visit(TAst& ast, TASTPtr... ptrs)
        : ast{ast}, indices{detail::ASTPtr_privates::Index(ptrs)...} {}
    template <class Visitor>
    auto operator()(Visitor&&);
    template <class Visitor>
    auto visit(Visitor&& visitor) {
        return (*this)(std::forward<Visitor>(visitor));
    }
};

namespace detail {
template <class... Ts>
struct pack_of_types {};

template <size_t Index, class Pack>
struct pack_of_types_index {};

template <size_t Index, class... Ts>
struct pack_of_types_index<Index, pack_of_types<Ts...>> {
    using value_t = pack_index<Index, Ts...>::value_t;
};

template <size_t Index, class... Ts>
struct pack_of_types_index<Index, ASTPtr<Ts...>> {
    using value_t = pack_index<Index, Ts...>::value_t;
};

template <class Pack>
struct pack_of_types_size {};

template <class... Ts>
struct pack_of_types_size<pack_of_types<Ts...>> {
    static constexpr size_t value = sizeof...(Ts);
};

template <class... Ts>
struct pack_of_types_size<ASTPtr<Ts...>> {
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
struct packs_mdindex<Index, pack_of_types<Ts...>> {
    using value_t = pack_of_types<pack_index_t<Index, Ts...>>;
};

template <size_t Index, class... Ts>
struct packs_mdindex<Index, ASTPtr<Ts...>> {
    using value_t = pack_of_types<pack_index_t<Index, Ts...>>;
};

template <size_t Index, class... Packs>
using packs_mdindex_t = packs_mdindex<Index, Packs...>::value_t;

}  // namespace detail

template <class TAst, class... TASTPtr, class Visitor>
auto operator|(Visit<TAst, TASTPtr...>&& visited, Visitor&& f) {
    return std::move(visited)(f);
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
    template <concepts::is_ast_node TNode>
    static NodeMemoryPosition position_in_memory(const TNode& node) {
        return node.position_in_memory();
    }
    template <concepts::is_ast_node TNode>
    static void on_node_moved(TNode& node, uint32_t new_extras_ptr, uint32_t new_strings_ptr) {
        return node.on_moved(new_extras_ptr, new_strings_ptr);
    }
    template <concepts::is_ast_label TLabel>
    using label_base_t = TLabel::base_t;
};

}  // namespace detail

template <concepts::is_ast TAst, concepts::is_ast_variant_ptr... TASTPtr>
template <class Visitor>
inline auto Visit<TAst, TASTPtr...>::operator()(Visitor&& visitor) {
    using namespace detail;
    constexpr size_t npacks = sizeof...(TASTPtr);
    using proxy_t = std::invoke_result_t<Visitor, pack_of_types_index_t<0ULL, TASTPtr>...> (*)(
        Visitor, AST_privates&, const std::array<uint32_t, npacks>&);
    static constexpr std::array vtable = []<size_t... TIndices>(
                                             std::integer_sequence<size_t, TIndices...>) {
        return std::array<proxy_t, sizeof...(TIndices)>{
            [](Visitor vis, AST_privates& ast, const std::array<uint32_t, npacks>& indices) {
                using MyTypesPack = packs_mdindex<TIndices, TASTPtr...>;
                return [&]<class... MyTypes, size_t... MyTypeIndices>(
                           pack_of_types<MyTypes...>,
                           std::integer_sequence<size_t, MyTypeIndices...>) {
                    return std::forward<Visitor>(vis)(
                        AnyAstNode_privates::create_label<MyTypes>(ast, indices[MyTypeIndices])...);
                }(MyTypesPack{}, std::make_index_sequence<sizeof...(TIndices)>());
            }...};
    }(std::make_index_sequence<(pack_of_types_size_v<TASTPtr> * ...)>());
    static constexpr std::array<size_t, npacks> coefs{pack_of_types_size_v<TASTPtr>...};
    size_t totalindex = 0;
    const auto& treenodes = ast.tree;
    for (uint32_t i : indices) {
        if (coefs[i] == 1) continue;  // Must not access treenodes[i].info of a singleton variant
        totalindex = totalindex * coefs[i] + treenodes[i].info;
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
}  // namespace traits

template <concepts::is_ast TAst, concepts::is_ast_ptr Item>
class Array<TAst, Item> {
    TAst& ast;
    detail::ASTNodeItem fields;  // {size, {data|extra_ptr, data, data}}
    uint32_t myindex;
    uint32_t extras_index() const { return fields.children[0]; }
    uint32_t& extras_index() { return fields.children[0]; }
    auto extras() const { return detail::AST_privates::extra(ast).begin() + extras_index(); }
    bool is_small_list() const { return is_small_list(size()); }
    static bool is_small_list(uint32_t size) { return size <= 3; }
    detail::ASTNodeItem& ast_tree_storage() { return detail::AST_privates::tree(ast)[myindex]; }
    std::pair<uint32_t&, uint32_t*> get_by_index_transparent(uint32_t index) {
        using result_t = std::pair<uint32_t&, uint32_t*>;
        if (is_small_list())
            return {fields.children[index], ast_tree_storage().children.data() + index};
        else
            return index < 2 ? result_t{fields.children[index + 1],
                                        ast_tree_storage().children.data() + (index + 1)}
                             : result_t{extras()[index - 2], nullptr};
    }
    uint32_t get_by_index(uint32_t index) const {
        if (is_small_list())
            return fields.children.at(index);
        else
            return index < 2 ? fields.children[index + 1] : extras()[index - 2];
    }
    Array(TAst& ast, uint32_t index) : ast{ast}, fields{ast.tree[index]}, myindex{index} {}
    Array(TAst& ast, detail::ASTNodeItem fields, uint32_t index)
        : ast{ast}, fields{fields}, myindex{index} {}
    detail::NodeMemoryPosition position_in_memory() const {
        uint32_t n = size();
        if (n > 3) {  // !is_small_list
            uint32_t ex_index = extras_index();
            return {myindex, {ex_index, ex_index + (n - 2)}, {0, 0}};
        }
        return {myindex, {0, 0}, {0, 0}};
    }
    void on_moved(uint32_t new_extras_ptr, uint32_t /*new_strings_ptr*/) {
        ast_tree_storage().children[0] = fields.children[0] = new_extras_ptr;
    }
    friend detail::AnyAstNode_privates;

public:
    struct iterator;
    struct ItemRefWrapper {
        uint32_t& item;
        uint32_t* opt_storage_ptr;
        operator Item() const { return detail::ASTPtr_privates::Create<Item>(item); }
        ItemRefWrapper& operator=(Item other) {
            item = detail::ASTPtr_privates::Index(other);
            if (opt_storage_ptr) *opt_storage_ptr = detail::ASTPtr_privates::Index(other);
            return *this;
        }

    private:
        friend Array;
        friend iterator;
        explicit ItemRefWrapper(uint32_t& item) : item{item}, opt_storage_ptr{nullptr} {}
        ItemRefWrapper(uint32_t& item, uint32_t* storage_ptr)
            : item{item}, opt_storage_ptr{storage_ptr} {}
        explicit ItemRefWrapper(std::pair<uint32_t&, uint32_t*> p)
            : ItemRefWrapper(p.first, p.second) {}
    };

    uint32_t size() const { return fields.info; }
    ItemRefWrapper at(uint32_t index) { return {get_by_index_transparent(index)}; }
    ItemRefWrapper operator[](uint32_t index) { return at(index); }
    Item at(uint32_t index) const {
        return detail::ASTPtr_privates::Create<Item>(get_by_index(index));
    }
    Item operator[](uint32_t index) const { return at(index); }

    struct iterator {
        using iterator_category = std::random_access_iterator_tag;
        using difference_type = uint32_t;
        using pointer = Item*;
        using reference = ItemRefWrapper;
        using value_type = Item;

        Array& owner;
        uint32_t index;

    private:
        iterator(Array& owner, uint32_t index) : owner{owner}, index{index} {}
        friend Array;

    public:
        auto operator*() { return owner.at(index); }
        iterator& operator--() {
            --index;
            return *this;
        }
        iterator operator--(int) {
            auto res = *this;
            --index;
            return res;
        }
        iterator& operator++() {
            ++index;
            return *this;
        }
        iterator operator++(int) {
            auto res = *this;
            ++index;
            return res;
        }
        iterator& operator+=(difference_type d) {
            index += d;
            return *this;
        }
        iterator& operator-=(difference_type d) {
            index -= d;
            return *this;
        }
        iterator operator+(difference_type d) const { return iterator{*this} += d; }
        iterator operator-(difference_type d) const { return iterator{*this} -= d; }
        std::strong_ordering operator<=>(const iterator& other) const {
            return index <=> other.index;
        }
        bool operator==(const iterator& other) const { return index == other.index; }
        bool operator!=(const iterator& other) const { return index != other.index; }
    };

    iterator begin() { return iterator{*this, 0}; }
    iterator end() { return iterator{*this, size()}; }

    ASTPtr<Array> pointer() const {
        return detail::ASTPtr_privates::Create<ASTPtr<Array>>(myindex);
    }
    template <class T>
    T& annotation() {
        return this->ast.template annotation<T>(pointer());
    }
    template <class T>
    const T& annotation() const {
        return this->ast.template annotation<T>(pointer());
    }
    template <lib::concepts::is_ast NewAst>
    Array<NewAst, std::string> with_ast(NewAst& new_ast) {
        return detail::AnyAstNode_privates::create_array<Array<NewAst, Item>>(new_ast, this->fields,
                                                                              myindex);
    }

    template <std::ranges::range Range>
        requires(std::same_as<std::ranges::range_value_t<Range>, Item> &&
                 requires(Range range) { std::size(range); })
    Array(TAst& ast, Range&& range) : ast{ast} {
        const uint32_t n = fields.info = std::size(range);
        detail::ASTNewNodeInfo new_node_info;
        if (is_small_list(n)) {
            new_node_info = detail::AST_privates::emplace_back(ast, 0, 0, false);
        } else {
            new_node_info = detail::AST_privates::emplace_back(ast, n - 2, 0, false);
            extras_index() = new_node_info.extras_index;
        }
        myindex = new_node_info.node_index;
        auto my_iter = begin();
        for (const auto& i : std::forward<Range>(range)) {
            *my_iter = i;
            ++my_iter;
        }
        detail::AST_privates::tree(ast)[new_node_info.node_index] = fields;
    }
    template <std::ranges::range Range>
        requires(std::same_as<std::ranges::range_value_t<Range>, Item> &&
                 !requires(Range range) { std::size(range); })
    Array(TAst& ast, Range&& range)
        : Array(ast,
                std::vector(std::forward<Range>(range).begin(), std::forward<Range>(range).end())) {
    }
};

template <concepts::is_ast TAst>
class Array<TAst, std::string> {
    // ast node: {size, {strings_ptr, 0, 0}}
    TAst& ast;
    uint32_t size_;
    uint32_t strings_ptr;
    uint32_t myindex;
    auto strings() const { return detail::AST_privates::string_tokens(ast).begin() + strings_ptr; }
    Array(TAst& ast, uint32_t index) : Array(ast, ast.tree[index], index) {}
    Array(TAst& ast, detail::ASTNodeItem data, uint32_t index)
        : ast{ast}, size_{data.info}, strings_ptr{data.children[0]}, myindex{index} {}
    Array(TAst& ast, uint32_t size, uint32_t strings_ptr, uint32_t index)
        : ast{ast}, size_{size}, strings_ptr{strings_ptr}, myindex{index} {}
    void on_moved(uint32_t /*new_extras_ptr*/, uint32_t new_strings_ptr) {
        detail::AST_privates::tree(ast)[myindex].children[0] = strings_ptr = new_strings_ptr;
    }
    friend detail::AnyAstNode_privates;

public:
    using iterator = std::vector<std::string>::iterator;
    using const_iterator = std::vector<std::string>::const_iterator;

    uint32_t size() const { return size_; }
    std::string& at(uint32_t index) { return strings()[index]; }
    const std::string& at(uint32_t index) const { return strings()[index]; }
    std::string& operator[](uint32_t index) { return at(index); }
    const std::string& operator[](uint32_t index) const { return at(index); }

    iterator begin() { return strings(); }
    iterator end() { return strings() + size(); }
    const_iterator begin() const { return strings(); }
    const_iterator end() const { return strings() + size(); }

    ASTPtr<Array> pointer() const { return lib::detail::ASTPtr_privates::Create<Array>(myindex); }
    template <class T>
    T& annotation() {
        return this->ast.template annotation<T>(pointer());
    }
    template <class T>
    const T& annotation() const {
        return this->ast.template annotation<T>(pointer());
    }
    template <lib::concepts::is_ast NewAst>
    Array<NewAst, std::string> with_ast(NewAst& new_ast) {
        return detail::AnyAstNode_privates::create_array<Array<NewAst, std::string>>(
            new_ast, size_, strings_ptr, myindex);
    }

    template <std::ranges::range Range>
        requires(std::same_as<std::ranges::range_value_t<Range>, std::string> &&
                 requires(Range range) { std::size(range); })
    Array(TAst& ast, Range&& range) : ast{ast}, size_{std::size(range)} {
        detail::ASTNewNodeInfo new_node_info =
            detail::AST_privates::emplace_back(ast, 0, size(), false);
        myindex = new_node_info.node_index;
        strings_ptr = new_node_info.strings_index;
        std::ranges::copy(
            std::forward<Range>(range),
            detail::AST_privates::string_tokens(ast).begin() + new_node_info.strings_index);
        detail::AST_privates::tree(ast)[myindex] = {size(), {strings_ptr, 0, 0}};
    }
    template <std::ranges::range Range>
        requires(std::same_as<std::ranges::range_value_t<Range>, std::string> &&
                 !requires(Range range) { std::size(range); })
    Array(TAst& ast, Range&& range) : ast{ast} {
        std::vector<std::string> v(range.begin(), range.end());
        new (this) Array(
            ast, std::ranges::subrange(std::move_iterator(v.begin()), std::move_iterator(v.end())));
    }
};

// TODO emplacing constructor
template <concepts::is_ast TAst, concepts::is_ast_scalar_token Token>
class Array<TAst, Token> {
    TAst& ast;
    detail::ASTNodeItem fields;  // {size, {data|extras_ptr, data, data}}
    uint32_t myindex;
    static constexpr uint32_t two_fields_hold_tokens = sizeof(uint32_t) * 2 / sizeof(Token);
    static constexpr uint32_t three_fields_hold_tokens = sizeof(uint32_t) * 3 / sizeof(Token);
    Token* extras() const {
        return reinterpret_cast<Token*>(detail::AST_privates::extra(ast).data() +
                                        fields.children[0]);
    }
    inline bool is_small_list() { return size() <= three_fields_hold_tokens; }
    Array(TAst& ast, uint32_t index) : ast{ast}, fields{ast.tree[index]} {}
    Token& get_by_index(uint32_t index) {
        if (is_small_list()) return *(reinterpret_cast<Token*>(&fields.children[0]) + index);
        return index < two_fields_hold_tokens
                   ? *(reinterpret_cast<Token*>(&fields.children[1]) + index)
                   : extras()[index - two_fields_hold_tokens];
    }
    Token get_by_index(uint32_t index) const {
        if (is_small_list()) return *(reinterpret_cast<Token*>(&fields.children[0]) + index);
        return index < two_fields_hold_tokens
                   ? *(reinterpret_cast<Token*>(&fields.children[1]) + index)
                   : extras()[index - two_fields_hold_tokens];
    }
    void on_moved(uint32_t new_extras_ptr, uint32_t /*new_strings_ptr*/) {
        if (!is_small_list())
            detail::AST_privates::tree(ast)[myindex].children[0] = fields.children[0] =
                new_extras_ptr;
    }
    friend detail::AnyAstNode_privates;

public:
    uint32_t size() const { return fields.info; }
    Token& at(uint32_t index) { return get_by_index(index); }
    Token at(uint32_t index) const { return get_by_index(index); }
    Token& operator[](uint32_t index) { return at(index); }
    Token operator[](uint32_t index) const { return at(index); }

    struct iterator {
        using iterator_category = std::random_access_iterator_tag;
        using difference_type = uint32_t;
        using pointer = Token*;
        using reference = Token&;
        using value_type = Token;

    private:
        Array& owner;
        uint32_t index;
        explicit iterator(Array& owner, uint32_t index) : owner{owner}, index{index} {}

    public:
        reference operator*() { return owner.at(index); }
        pointer operator->() { return &owner.at(index); }

        iterator& operator--() {
            --index;
            return *this;
        }
        iterator operator--(int) {
            auto res = *this;
            --index;
            return res;
        }
        iterator& operator++() {
            ++index;
            return *this;
        }
        iterator operator++(int) {
            auto res = *this;
            ++index;
            return res;
        }
        iterator& operator+=(difference_type d) {
            index += d;
            return *this;
        }
        iterator& operator-=(difference_type d) {
            index -= d;
            return *this;
        }
        iterator operator+(difference_type d) const { return iterator{*this} += d; }
        iterator operator-(difference_type d) const { return iterator{*this} -= d; }
        std::strong_ordering operator<=>(const iterator& other) const {
            return index <=> other.index;
        }
        bool operator==(const iterator& other) const { return index == other.index; }
        bool operator!=(const iterator& other) const { return index != other.index; }
    };

    iterator begin() const { return iterator(*this, 0); }
    iterator end() const { return iterator(*this, size()); }

    ASTPtr<Array> pointer() const {
        return lib::detail::ASTPtr_privates::Create<Array>(this->myindex);
    }
    template <class T>
    T& annotation() {
        return this->ast.template annotation<T>(pointer());
    }
    template <class T>
    const T& annotation() const {
        return this->ast.template annotation<T>(pointer());
    }
    template <lib::concepts::is_ast NewAst>
    Array<NewAst, std::string> with_ast(NewAst& new_ast) const {
        return detail::AnyAstNode_privates::create_array<Array<NewAst, std::string>>(new_ast,
                                                                                     this->fields);
    }
};

template <concepts::is_ast_array TArray>
class ASTPtr<TArray> {
    uint32_t index;
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
    // TODO more?
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
    std::array<char, sizeof(detail::ASTNodeItem)> inline_data;
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
            return get_inline_scalar<uint32_t, layout_t::PlaceToStoreNodeIndex>();
    }

    template <class TScalar, LabelFieldBytePos Pos>
    TScalar get_scalar() const {
        if constexpr (Pos.first == LabelFieldLocation::Inline)
            return get_inline_scalar<TScalar, Pos.second>();
        else
            return *reinterpret_cast<TScalar*>(extras() + Pos.second);
    }

    Label(TAst& ast, const decltype(inline_data)& data, decltype(cond_index) cond_index)
        : ast{ast}, inline_data{data}, cond_index{cond_index} {}

    void on_moved(uint32_t new_extras_ptr, uint32_t new_strings_ptr) {
        if constexpr (layout_t::ExtraSpaceInts)
            set_inline_scalar<uint32_t, layout_t::ExtraPtrOffset>(new_extras_ptr);
        if constexpr (NStrings)
            set_inline_scalar<uint32_t, layout_t::StringPtrOffset>(new_strings_ptr);
    }

    friend AnyAstNode_privates;

public:
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

    // void set_extras_offset(uint32_t value) {
    //     set_inline_scalar<uint32_t, layout_t::ExtraPtrOffset()>(value);
    // }
    //
    // void set_strings_offset(uint32_t value) {
    //     return set_inline_scalar<uint32_t, layout_t::StringPtrOffset()>(value);
    // }

public:
    using creation_params_t =
        std::tuple<std::array<uint32_t, NPtrs>, std::array<std::string_view, NStrings>,
                   std::array<Double, NDoubles>, std::array<Integer, NInts>,
                   std::array<Char, NChars>, std::array<Bool, NBools>>;

private:
    void initialize(ASTNewNodeInfo new_node_info, uint32_t tag_if_needed,
                    const creation_params_t& vals) {
        if constexpr (TStoresTag)
            set_inline_scalar<uint32_t, layout_t::TagOffset, false>(tag_if_needed);
        if constexpr (layout_t::ExtraSpaceInts)
            set_inline_scalar<uint32_t, layout_t::ExtraPtrOffset, false>(
                new_node_info.extras_index);
        if constexpr (NStrings)
            set_inline_scalar<uint32_t, layout_t::StringPtrOffset, false>(
                new_node_info.strings_index);
        {
            auto& pointer_values = get<0>(vals);
            [&]<size_t... Indices>(std::index_sequence<Indices...>) {
                (pointer<Indices, false>(pointer_values.at(Indices)), ...);
            }(std::make_index_sequence<NPtrs>());
        }
        {
            auto& string_values = get<1>(vals);
            std::string* mystrings =
                AST_privates::string_tokens(ast).data() + new_node_info.strings_index;
            for (size_t i = 0; i < NStrings; i++) {
                mystrings[i] = string_values[i];
            }
        }
        [&]<size_t... TokenKindIndices>(std::index_sequence<TokenKindIndices...>) {
            (
                [&]() {
                    auto& values = get<TokenKindIndices>(vals);
                    [&]<size_t... Indices>(std::index_sequence<Indices...>) {
                        (
                            [&]() {
                                auto& value = values[Indices];
                                set_scalar_token<std::remove_reference_t<decltype(value)>, Indices,
                                                 false>(value);
                            }(),
                            ...);
                    }(std::make_index_sequence<std::size(values)>());
                }(),
                ...);
        }(std::index_sequence<2, 3, 4, 5>());
        AST_privates::tree(ast)[new_node_info.node_index] =
            reinterpret_cast<ASTNodeItem&>(inline_data);
        mut_index() = new_node_info.node_index;
    }

public:
    template <size_t Index>
    std::string& string()
        requires(NStrings > 0)
    {
        return AST_privates::string_tokens(ast)[strings_offset() + Index];
    }

    template <size_t Index>
    const std::string& string() const
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
    void set_scalar_token(TScalar value) const {
        set_scalar<TScalar, layout_t::template ScalarPos<TScalar>(Index), Commit>(value);
    }

    NodeMemoryPosition position_in_memory() const {
        if constexpr (layout_t::ExtraSpaceInts > 0) {
            uint32_t extras = extras_offset();
            if constexpr (NStrings > 0) {
                uint32_t strings = strings_offset();
                return {index(),
                        {extras, extras + layout_t::ExtraSpaceInts},
                        {strings, strings + NStrings}};
            } else {
                return {index(), {extras, extras + layout_t::ExtraSpaceInts}, {0, 0}};
            }
        } else {
            if constexpr (NStrings > 0) {
                uint32_t strings = strings_offset();
                return {index(), {0, 0}, {strings, strings + NStrings}};
            } else {
                return {index(), {0, 0}, {0, 0}};
            }
        }
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
    Label(TAst& ast, uint32_t tag_if_needed, creation_params_t vals) : ast{ast} {
        auto info = AST_privates::emplace_back(ast, layout_t::ExtraSpaceInts, NStrings,
                                               layout_t::Needs8ByteAlignment);
        initialize(ast, info, tag_if_needed, vals);
    }

    template <concepts::is_ast TNewAst>
    Label<TNewAst, TStoresTag, NPtrs, NStrings, NDoubles, NInts, NChars, NBools> with_ast(
        TNewAst& new_ast) {
        return AnyAstNode_privates::create_label<
            Label<TNewAst, TStoresTag, NPtrs, NStrings, NDoubles, NInts, NChars, NBools>>(
            new_ast, inline_data, cond_index);
    }

    ~Label() = default;
};

template <concepts::is_ast_label TLabel>
struct label_pointer_fields;

template <concepts::is_ast_label TLabel>
using label_pointer_fields_t = label_pointer_fields<TLabel>::value_t;

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
    return std::get<TAnnot>(annots)[detail::ASTPtr_privates::Index(ptr)];
}

template <concepts::detail::default_constructible... TAnnotations>
    requires(detail::pairwise_different_v<TAnnotations...>)
template <class TAnnot, concepts::is_ast_ptr TPtr>
    requires(std::same_as<TAnnot, TAnnotations> || ...)
inline const TAnnot& AST<TAnnotations...>::annotation(TPtr ptr) const
    requires(sizeof...(TAnnotations) > 0)
{
    return std::get<TAnnot>(annots)[detail::ASTPtr_privates::Index(ptr)];
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

template <concepts::detail::default_constructible... TAnnotations>
    requires(detail::pairwise_different_v<TAnnotations...>)
template <class FuncDataMove>
    requires(std::same_as<std::invoke_result_t<FuncDataMove, detail::NodeMemoryPosition, bool>,
                          detail::ASTNewNodeInfo>)
template <concepts::is_ast_label TLabel>
uint32_t AST<TAnnotations...>::PreorderTransducer<FuncDataMove>::operator()(TLabel l) {
    detail::NodeMemoryPosition oldpos = detail::AnyAstNode_privates::position_in_memory(l);
    using TBase = detail::AnyAstNode_privates::label_base_t<TLabel>;
    detail::ASTNewNodeInfo newpos = transduce(oldpos, TBase::layout_t::Needs8ByteAlignment);
    TBase oldbase{oldast, oldpos.index};
    TBase newbase{newast, newpos.node_index};
    detail::AnyAstNode_privates::on_node_moved(newbase, newpos.extras_index, newpos.strings_index);
    using TPointerFields = detail::label_pointer_fields_t<TLabel>;
    [&]<class... TPtrs, size_t... Indices>(detail::pack_of_types<TPtrs...>,
                                           std::index_sequence<Indices...>) {
        (newbase.template pointer<Indices>(
             (*this)(detail::ASTPtr_privates::Create<TPtrs>(oldbase.template pointer<Indices>()))),
         ...);
    }(TPointerFields(), std::make_index_sequence<detail::pack_of_types_size_v<TPointerFields>>());
    return newpos.node_index;
}

template <concepts::detail::default_constructible... TAnnotations>
    requires(detail::pairwise_different_v<TAnnotations...>)
template <class FuncDataMove>
    requires(std::same_as<std::invoke_result_t<FuncDataMove, detail::NodeMemoryPosition, bool>,
                          detail::ASTNewNodeInfo>)
template <class TAst, concepts::is_ast_ptr TPtr>
uint32_t AST<TAnnotations...>::PreorderTransducer<FuncDataMove>::operator()(Array<TAst, TPtr> arr) {
    detail::NodeMemoryPosition oldpos = detail::AnyAstNode_privates::position_in_memory(arr);
    detail::ASTNewNodeInfo newpos = transduce(oldpos, false);
    auto newarr =
        detail::AnyAstNode_privates::create_array<decltype(arr)>(newast, newpos.node_index);
    detail::AnyAstNode_privates::on_node_moved(newarr, newpos.extras_index, newpos.strings_index);
    auto old_iter = arr.begin();
    for (auto newptr_ref : newarr) {
        newptr_ref = detail::ASTPtr_privates::Create<TPtr>((*this)(*old_iter));
        ++old_iter;
    }
    return newpos.node_index;
}

template <concepts::detail::default_constructible... TAnnotations>
    requires(detail::pairwise_different_v<TAnnotations...>)
template <class FuncDataMove>
    requires(std::same_as<std::invoke_result_t<FuncDataMove, detail::NodeMemoryPosition, bool>,
                          detail::ASTNewNodeInfo>)
template <class TAst, concepts::is_ast_token Tk>
uint32_t AST<TAnnotations...>::PreorderTransducer<FuncDataMove>::operator()(Array<TAst, Tk> arr) {
    detail::NodeMemoryPosition oldpos = detail::AnyAstNode_privates::position_in_memory(arr);
    detail::ASTNewNodeInfo newpos = transduce(oldpos, sizeof(Tk) == 8);
    auto newarr =
        detail::AnyAstNode_privates::create_array<decltype(arr)>(newast, newpos.node_index);
    detail::AnyAstNode_privates::on_node_moved(newarr, newpos.extras_index, newpos.strings_index);
    return newarr.node_index;
}

template <concepts::detail::default_constructible... TAnnotations>
    requires(detail::pairwise_different_v<TAnnotations...>)
template <class FuncDataMove>
    requires(std::same_as<std::invoke_result_t<FuncDataMove, detail::NodeMemoryPosition, bool>,
                          detail::ASTNewNodeInfo>)
template <concepts::is_ast_variant_ptr TPtr>
uint32_t AST<TAnnotations...>::PreorderTransducer<FuncDataMove>::operator()(TPtr ptr) {
    return oldast.visit(ptr) | *this;
}

template <concepts::detail::default_constructible... TAnnotations>
    requires(detail::pairwise_different_v<TAnnotations...>)
template <class FuncDataMove>
    requires(std::same_as<std::invoke_result_t<FuncDataMove, detail::NodeMemoryPosition, bool>,
                          detail::ASTNewNodeInfo>)
template <concepts::is_ast_array_ptr TPtr>
uint32_t AST<TAnnotations...>::PreorderTransducer<FuncDataMove>::operator()(TPtr ptr) {
    return (*this)(oldast.get(ptr));
}

template <concepts::detail::default_constructible... TAnnotations>
    requires(detail::pairwise_different_v<TAnnotations...>)
template <concepts::is_ast_ptr TPtr>
inline AST<TAnnotations...> AST<TAnnotations...>::preorder(TPtr& root) & {
    AST res = new_ast_with_reserved_containers();
    PreorderTransducer dfs(
        [&](detail::NodeMemoryPosition oldpos) {
            uint32_t newindex = res.tree.size();
            res.tree.push_back(tree[oldpos.index]);
        },
        *this, res);
}

template <concepts::detail::default_constructible... TAnnotations>
    requires(detail::pairwise_different_v<TAnnotations...>)
template <concepts::is_ast_ptr TPtr>
inline AST<TAnnotations...> AST<TAnnotations...>::preorder(TPtr& root) && {
    // TODO
    throw 0;
}

}  // namespace lib

// EXAMPLE

template <class>
class Plus;

template <class TAst>
using Expr = lib::ASTPtr<Plus<TAst>>;

template <class TAst>
class Plus final {
private:
    friend lib::detail::AnyAstNode_privates;
    using base_t = lib::detail::Label<TAst, true, 2, 0, 0, 0, 0, 0>;
    base_t base;

    lib::detail::NodeMemoryPosition position_in_memory() const { return base.position_in_memory(); }

    Plus(TAst& ast, uint32_t index) : base{ast, index} {}
    Plus(base_t base) : base{base} {}

public:
    Plus(TAst& ast, Expr<TAst> Expr_1, Expr<TAst> Expr_2)
        : base(ast, 0,
               {{lib::detail::ASTPtr_privates::Index(Expr_1),
                 lib::detail::ASTPtr_privates::Index(Expr_2)},
                {},
                {},
                {},
                {},
                {}}) {}
    Expr<TAst> Expr_1() const {
        return lib::detail::ASTPtr_privates::Create<Expr>(base.template pointer<0>());
    }
    void Expr_1(Expr<TAst> value) {
        base.template pointer<0>(lib::detail::ASTPtr_privates::Index(value));
    }
    Expr<TAst> Expr_2() const {
        return lib::detail::ASTPtr_privates::Create<Expr>(base.template pointer<1>());
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
    Plus<NewAst> with_ast(NewAst& new_ast) const {
        return lib::detail::AnyAstNode_privates::create_label<Plus<NewAst>>(base.with_ast(new_ast));
    }
};

namespace lib {
namespace traits {
template <class TAst>
struct ast_class_type<Plus<TAst>> {
    static constexpr ASTClassType value = ASTClassType::Label;
};
}  // namespace traits

namespace detail {
template <class TAst>
struct label_pointer_fields<Plus<TAst>> {
    using value_t = pack_of_types<Expr<TAst>, Expr<TAst>>;
};
}  // namespace detail
}  // namespace lib

}  // namespace ast
