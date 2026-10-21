// we assume read-write access to the AST (TODO: implement read-only labels)
#include <array>
#include <cstdint>
#include <iterator>
#include <optional>
#include <string>
#include <tuple>
#include <type_traits>
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
concept is_ast_scalar_token =
    traits::ast_class_type_v<std::remove_cvref_t<T>> == traits::ASTClassType::ScalarToken;

template <class T>
concept is_ast_string_token =
    traits::ast_class_type_v<std::remove_cvref_t<T>> == traits::ASTClassType::StringToken;

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

struct AST_privates {
    template <class... Annots>
    static std::vector<detail::ASTNodeItem>& tree(AST<Annots...>&);
    template <class... Annots>
    static const std::vector<detail::ASTNodeItem>& tree(const AST<Annots...>&);
    template <class... Annots>
    static std::vector<std::string>& string_tokens(AST<Annots...>&);
    template <class... Annots>
    static const std::vector<std::string>& string_tokens(const AST<Annots...>&);
    template <class... Annots>
    static std::vector<uint32_t>& extra(AST<Annots...>&);
    template <class... Annots>
    static const std::vector<uint32_t>& extra(const AST<Annots...>&);
    template <class TAnnot, class... Annots>
        requires concepts::detail::same_as_any_of<TAnnot, Annots...>
    static std::vector<TAnnot>& annots(AST<Annots...>&);
    template <class TAnnot, class... Annots>
        requires concepts::detail::same_as_any_of<TAnnot, Annots...>
    static const std::vector<TAnnot>& annots(const AST<Annots...>&);
};

}  // namespace detail

template <concepts::is_ast TAst, concepts::is_ast_variant_ptr... TASTPtr>
class Visit;

template <class...>
class ASTPtr;

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

    uint32_t emplace_back(uint32_t nextra, uint32_t nstrings, bool needs_8byte_align) {
        uint32_t res = tree.size();
        tree.emplace_back();
        //TODO
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
    // {
    //     return std::get<TAnnot>(annots)[detail::ASTPtr_privates::Index(ptr)];
    // }
    template <class TAnnot, concepts::is_ast_ptr TPtr>
        requires(std::same_as<TAnnot, TAnnotations> || ...)
    const TAnnot& annotation(TPtr ptr) const
        requires(sizeof...(TAnnotations) > 0);
    // {
    //     return std::get<TAnnot>(annots)[detail::ASTPtr_privates::Index(ptr)];
    // }

    template <concepts::is_ast_variant_ptr... TPtr>
        requires(sizeof...(TPtr) > 0)
    Visit<AST, TPtr...> visit(TPtr... ptrs) {
        return {*this, ptrs...};
    }
    template <concepts::is_ast_label TLabel, concepts::is_ast_label... TLs>
        requires(concepts::detail::same_as_any_of<TLabel, TLs...>)
    TLabel unchecked_get(ASTPtr<TLs...> ptr);
    template <concepts::is_ast_label TLabel, concepts::is_ast_label... TLs>
        requires(concepts::detail::same_as_any_of<TLabel, TLs...>)
    std::optional<TLabel> checked_get(ASTPtr<TLs...> ptr);
    template <concepts::is_ast_label TLabel>
    TLabel get(ASTPtr<TLabel> ptr);
    template <concepts::is_ast_array TArray>
    TArray get(ASTPtr<TArray> ptr);
    // TODO definitions
};

namespace traits {
template <class... Ts>
struct is_ast<AST<Ts...>> {
    static constexpr bool value = true;
};
}  // namespace traits

namespace detail {
template <class... Annots>
inline std::vector<detail::ASTNodeItem>& AST_privates::tree(AST<Annots...>& ast) {
    return ast.tree;
}
template <class... Annots>
inline const std::vector<detail::ASTNodeItem>& AST_privates::tree(const AST<Annots...>& ast) {
    return ast.tree;
}
template <class... Annots>
inline std::vector<std::string>& AST_privates::string_tokens(AST<Annots...>& ast) {
    return ast.string_tokens;
}
template <class... Annots>
inline const std::vector<std::string>& AST_privates::string_tokens(const AST<Annots...>& ast) {
    return ast.string_tokens;
}
template <class... Annots>
inline std::vector<uint32_t>& AST_privates::extra(AST<Annots...>& ast) {
    return ast.extra;
}
template <class... Annots>
inline const std::vector<uint32_t>& AST_privates::extra(const AST<Annots...>& ast) {
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
struct AstFactory {
    template <concepts::is_ast_label Label, class... Args>
    static Label create_label(Args&&... args) {
        return Label(std::forward<Args>(args)...);
    }
    template <concepts::is_ast_array TArray, class... Args>
    static TArray create_array(Args&&... args) {
        return TArray(std::forward<Args>(args)...);
    }
};
}  // namespace detail

template <concepts::is_ast TAst, concepts::is_ast_variant_ptr... TASTPtr>
template <class Visitor>
inline auto Visit<TAst, TASTPtr...>::operator()(Visitor&& visitor) {
    using namespace detail;
    constexpr size_t npacks = sizeof...(TASTPtr);
    using proxy_t = std::invoke_result_t<Visitor, pack_of_types_index_t<0ULL, TASTPtr>...> (*)(
        Visitor, AST_privates&, const std::array<uint32_t, npacks>&);
    static constexpr std::array vtable =
        []<size_t... TIndices>(std::integer_sequence<size_t, TIndices...>) {
            return std::array<proxy_t, sizeof...(TIndices)>{
                [](Visitor vis, AST_privates& ast, const std::array<uint32_t, npacks>& indices) {
                    using MyTypesPack = packs_mdindex<TIndices, TASTPtr...>;
                    return [&]<class... MyTypes, size_t... MyTypeIndices>(
                               pack_of_types<MyTypes...>,
                               std::integer_sequence<size_t, MyTypeIndices...>) {
                        return std::forward<Visitor>(vis)(
                            AstFactory::create_label<MyTypes>(ast, indices[MyTypeIndices])...);
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
    auto extras() const {
        return detail::AST_privates::extra(ast).begin() + fields.children[0];
    }
    uint32_t& get_by_index(uint32_t index) {
        if (size() > 3)
            return index < 2 ? fields.children[index + 1] : extras()[index - 2];
        else
            return fields.children[index];
    }
    uint32_t get_by_index(uint32_t index) const {
        if (size() > 3)
            return index < 2 ? fields.children[index + 1] : extras()[index - 2];
        else
            return fields.children.at(index);
    }
    Array(TAst& ast, uint32_t index) : ast{ast}, fields{ast.tree[index]}, myindex{index} {}
    Array(TAst& ast, detail::ASTNodeItem fields, uint32_t index)
        : ast{ast}, fields{fields}, myindex{index} {}
    friend detail::AstFactory;

public:
    struct iterator;
    struct ItemRefWrapper {
        uint32_t& item;
        operator Item() const { return detail::ASTPtr_privates::Create<Item>(item); }
        ItemRefWrapper& operator=(Item other) {
            item = detail::ASTPtr_privates::Index(other);
            return *this;
        }

    private:
        friend Array;
        friend iterator;
        explicit ItemRefWrapper(uint32_t& item) : item{item} {}
    };

    uint32_t size() const { return fields.info; }
    ItemRefWrapper at(uint32_t index) { return {get_by_index(index)}; }
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
        return detail::AstFactory::create_array<Array<NewAst, Item>>(new_ast, this->fields,
                                                                     myindex);
    }
};

template <concepts::is_ast TAst>
class Array<TAst, std::string> {
    // ast node: {size, {strings_ptr, myindex, 0}}
    TAst& ast;
    uint32_t size_;
    uint32_t strings_ptr;
    uint32_t myindex;
    auto strings() const {
        return detail::AST_privates::string_tokens(ast).begin() + strings_ptr;
    }
    Array(TAst& ast, uint32_t index) : Array(ast, ast.tree[index], index) {}
    Array(TAst& ast, detail::ASTNodeItem data, uint32_t index)
        : ast{ast}, size_{data.info}, strings_ptr{data.children[0]}, myindex{index} {}
    Array(TAst& ast, uint32_t size, uint32_t strings_ptr, uint32_t index)
        : ast{ast}, size_{size}, strings_ptr{strings_ptr}, myindex{index} {}
    friend detail::AstFactory;

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
        return detail::AstFactory::create_array<Array<NewAst, std::string>>(new_ast, size_,
                                                                            strings_ptr, myindex);
    }
};

template <concepts::is_ast TAst, concepts::is_ast_scalar_token Token>
class Array<TAst, Token> {
    TAst& ast;
    detail::ASTNodeItem fields;  // {size, {data|extras_ptr, data, data}}
    uint32_t myindex;
    static constexpr uint32_t two_fields_hold_tokens = sizeof(uint32_t) * 2 / sizeof(Token);
    static constexpr uint32_t three_fields_hold_tokens = sizeof(uint32_t) * 3 / sizeof(Token);
    Token* extras() const {
        return detail::AST_privates::string_tokens(ast).data() + fields.children[0];
    }
    Array(TAst& ast, uint32_t index) : ast{ast}, fields{ast.tree[index]} {}
    Token& get_by_index(uint32_t index) {
        if (size() <= three_fields_hold_tokens)
            return *(reinterpret_cast<Token*>(&fields.children[0]) + index);
        return index < two_fields_hold_tokens
                   ? *(reinterpret_cast<Token*>(&fields.children[1]) + index)
                   : extras()[index - two_fields_hold_tokens];
    }
    Token get_by_index(uint32_t index) const {
        if (size() <= three_fields_hold_tokens)
            return *(reinterpret_cast<Token*>(&fields.children[0]) + index);
        return index < two_fields_hold_tokens
                   ? *(reinterpret_cast<Token*>(&fields.children[1]) + index)
                   : extras()[index - two_fields_hold_tokens];
    }
    friend detail::AstFactory;

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
        return detail::AstFactory::create_array<Array<NewAst, std::string>>(new_ast, this->fields);
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
// template <concepts::is_ast TAst, bool HasStringTokens, bool HasExtra>
// class Label {
// protected:
//     TAst& ast;
//     detail::ASTNodeItem data;
//     Label(TAst& ast, uint32_t index) : ast{ast}, data{ast.tree[index]} { data.info = index; }
//     Label(TAst& ast, ASTNodeItem data) : ast{ast}, data{data} {}
//     uint32_t index() const { return data.info; }
//     uint32_t string_token_offset() const { return data.children[2]; }
//     uint32_t extra_offset() const { return data.children[2 - !!HasStringTokens]; }
//
//     template <class T>
//     T& get_extra(uint32_t offset) {
//         return reinterpret_cast<T&>(ast.extra[extra_offset() + offset]);
//     }
//     std::string& get_string(uint32_t index) {
//         return ast.string_tokens[string_token_offset() + index];
//     }
//
// public:
//     Label(const Label&) = delete;
//     Label(Label&&) = delete;
//     Label& operator=(const Label&) = delete;
//     Label& operator=(Label&&) = delete;
//     ~Label() = default;
//     // We only inherit privately, so no virtual dtor needed
// };

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
    static consteval std::tuple<scalars_layout_t, size_t, bool> make_scalars_layout() {
        scalars_layout_t res;
        size_t inline_pos = sizeof(detail::ASTNodeItem) - InlineSpace;
        size_t iptr = 0;
        size_t idouble = 0;
        size_t iint = 0;
        size_t ichar = 0;
        size_t ibool = 0;
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
        return {res, extra_pos, needs_8byte_align};
    }
    static constexpr std::tuple<scalars_layout_t, size_t, bool> ScalarsResult_ =
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
    static constexpr bool Needs8ByteAlignment = get<2>(ScalarsResult_);
    static constexpr size_t StringCount = NStrings;
    static constexpr size_t ExtraSpaceInts = (get<1>(ScalarsResult_) + 3) / 4;
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

// using l_t = LabelLayout<true, 2, 0, 0, 1, 0, 0>;
// static_assert(l_t::Needs8ByteAlignment);
// static_assert(l_t::ExtraSpaceInts == 2);
// static_assert(l_t::ExtraPtrOffset() == 4);
// static_assert(l_t::PointerPos(0) == LabelFieldBytePos{LabelFieldLocation::Inline, 8UL});
// static_assert(l_t::PointerPos(1) == LabelFieldBytePos{LabelFieldLocation::Inline, 12UL});
// static_assert(l_t::ScalarPos<Integer>(0) == LabelFieldBytePos{LabelFieldLocation::Extra, 0UL});

struct NodeMemoryPosition {
    uint32_t index;
    std::pair<uint32_t, uint32_t> extras_range;
    std::pair<uint32_t, uint32_t> strings_range;
};

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

private:
    using layout_t = LabelLayout<TStoresTag, NPtrs, NStrings, NDoubles, NInts, NChars, NBools>;

    std::array<char, sizeof(detail::ASTNodeItem)> inline_data;
    ConditionalIndex<!TStoresTag> cond_index [[no_unique_address]];

    template <class TScalar, size_t Pos>
    TScalar& get_inline_scalar() {
        return *reinterpret_cast<TScalar*>(inline_data.data() + Pos);
    }

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
        if constexpr (!TStoresTag)
            return cond_index.separately_stored_index();
        else
            return get_inline_scalar<uint32_t, 0>();
    }

    template <class TScalar, LabelFieldBytePos Pos>
    TScalar& get_scalar() {
        if constexpr (Pos.first == LabelFieldLocation::Inline)
            return get_inline_scalar<TScalar, Pos.second>();
        else
            return *reinterpret_cast<TScalar*>(extras() + Pos.second);
    }

    template <class TScalar, LabelFieldBytePos Pos>
    TScalar get_scalar() const {
        if constexpr (Pos.first == LabelFieldLocation::Inline)
            return get_inline_scalar<TScalar, Pos.second>();
        else
            return *reinterpret_cast<TScalar*>(extras() + Pos.second);
    }

    Label(TAst& ast, const decltype(inline_data)& data, decltype(cond_index) cond_index)
        : ast{ast}, inline_data{inline_data}, cond_index{cond_index} {}
    friend AstFactory;

public:
    template <size_t Index>
    std::string& get_string()
        requires(NStrings > 0)
    {
        return AST_privates::string_tokens(ast)[strings_offset() + Index];
    }

    template <size_t Index>
    const std::string& get_string() const
        requires(NStrings > 0)
    {
        return AST_privates::string_tokens(ast)[strings_offset() + Index];
    }

    template <size_t Index>
    uint32_t& get_pointer() {
        return get_scalar<uint32_t, layout_t::PointerPos(Index)>();
    }
    template <size_t Index>
    uint32_t get_pointer() const {
        return get_scalar<uint32_t, layout_t::PointerPos(Index)>();
    }
    template <class TScalar, size_t Index>
    TScalar& get_scalar_token() {
        return get_scalar<TScalar, layout_t::template ScalarPos<TScalar>(Index)>();
    }
    template <class TScalar, size_t Index>
    TScalar get_scalar_token() const {
        return get_scalar<TScalar, layout_t::template ScalarPos<TScalar>(Index)>();
    }

    uint32_t index() const {
        if constexpr (!TStoresTag)
            return cond_index.separately_stored_index();
        else
            return get_inline_scalar<uint32_t, 0>();
    }

    NodeMemoryPosition in_memory_position() {
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
        if constexpr (TStoresTag)
            mut_index() = index;
        else
            cond_index.separately_stored_index() = index;
    }

    template <concepts::is_ast TNewAst>
    Label<TNewAst, TStoresTag, NPtrs, NStrings, NDoubles, NInts, NChars, NBools> with_ast(
        TNewAst& new_ast) {
        return AstFactory::create_label<
            Label<TNewAst, TStoresTag, NPtrs, NStrings, NDoubles, NInts, NChars, NBools>>(
            new_ast, inline_data, cond_index);
    }

    ~Label() = default;
};

}  // namespace detail
}  // namespace lib

// EXAMPLE

template <class>
class Plus;

template <class TAst>
using Expr = lib::ASTPtr<Plus<TAst>>;

template <class TAst>
class Plus final {
private:
    friend lib::detail::AstFactory;
    using base_t = lib::detail::Label<TAst, true, 2, 0, 0, 0, 0, 0>;
    base_t base;
    Plus(TAst& ast, uint32_t index) : base{ast, index} {}
    Plus(base_t base) : base{base} {}

public:
    Expr<TAst> Expr_1() const {
        return lib::detail::ASTPtr_privates::Create<Expr>(base_t::template get_pointer<0>());
    }
    void Expr_1(Expr<TAst> value) {
        base_t::template get_pointer<0>() = lib::detail::ASTPtr_privates::Index(value);
    }
    Expr<TAst> Expr_2() const {
        return lib::detail::ASTPtr_privates::Create<Expr>(base_t::template get_pointer<1>());
    }
    void Expr_2(Expr<TAst> value) {
        base_t::template get_pointer<1>() = lib::detail::ASTPtr_privates::Index(value);
    }
    Expr<TAst> pointer() const {
        return lib::detail::ASTPtr_privates::Create<Expr>(base_t::index());
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
        return lib::detail::AstFactory::create_label<Plus<NewAst>>(base.with_ast(new_ast));
    }
};

namespace lib::traits {
template <class TAst>
struct ast_class_type<Plus<TAst>> {
    static constexpr ASTClassType value = ASTClassType::Label;
};
}  // namespace lib::traits

}  // namespace ast
