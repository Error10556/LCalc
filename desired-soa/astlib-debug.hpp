#pragma once

#include <array>
#include <bit>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <iosfwd>
#include <memory>
#include <span>
#include <string>
#include <typeinfo>
#include <vector>

#include "astlib.hpp"

namespace ast::lib {
namespace debug {

namespace traits {

template <class T>
struct ast_obj_name {
    static constexpr std::string get_name() {
        using std::literals::operator""s;
        return "(typeid)"s + typeid(T).name();
    }
};

template <>
struct ast_obj_name<std::string> {
    static constexpr std::string get_name() { return "Stringlike"; }
};

template <>
struct ast_obj_name<Integer> {
    static constexpr std::string get_name() { return "Integer"; }
};

template <>
struct ast_obj_name<Double> {
    static constexpr std::string get_name() { return "Double"; }
};

template <>
struct ast_obj_name<Char> {
    static constexpr std::string get_name() { return "Char"; }
};

template <>
struct ast_obj_name<Bool> {
    static constexpr std::string get_name() { return "Bool"; }
};

template <class... Ts>
struct ast_obj_name<ASTPtr<Ts...>> {
    static constexpr std::string get_name() {
        using std::literals::operator""s;
        std::string res = "ASTPtr<"s + (... + (ast_obj_name<Ts>::get_name() + ", "));
        res.erase(res.size() - 1);
        res.back() = '>';
        return res;
    }
};

template <concepts::is_ast TAst, class Item>
struct ast_obj_name<Array<TAst, Item>> {
    static constexpr std::string get_name() {
        return "Array<" + ast_obj_name<Item>::get_name() + ">";
    }
};

}  // namespace traits

class ASTDump {
public:
    struct Blob {
        std::size_t size;
        std::unique_ptr<unsigned char[]> container;
        template <class T>
        Blob(const T& obj) : size{sizeof(T)} {
            container = std::make_unique<unsigned char[]>(sizeof(T));
            std::memcpy(container.get(), &obj, sizeof(T));
        }
        std::span<unsigned char> span() const { return {container.get(), size}; }
    };
    std::vector<std::array<std::uint32_t, 4>> tree;
    std::vector<std::uint32_t> extras;
    std::vector<std::string> strings;
    std::vector<std::vector<Blob>> annots;
    std::vector<std::string> annot_names;
    template <class... Annots>
    ASTDump(const AST<Annots...>& ast) {
        {
            auto&& asttree = detail::AST_privates::tree(ast);
            tree.resize(asttree.size());
            for (std::size_t i = 0; i < tree.size(); i++)
                tree[i] = std::bit_cast<std::array<std::uint32_t, 4>>(asttree[i]);
        }
        extras = detail::AST_privates::extra(ast);
        strings = detail::AST_privates::string_tokens(ast);
        annot_names = {traits::ast_obj_name<Annots>::get_name()...};
        [[maybe_unused]] auto blobify_vector = []<class T>(const std::vector<T>& v) {
            std::vector<Blob> res;
            res.reserve(v.size());
            for (const T& item : v) res.emplace_back(item);
            return res;
        };
        annots.reserve(sizeof...(Annots));
        (annots.push_back(blobify_vector(detail::AST_privates::annots<Annots>(ast))), ...);
    }

    void interact(std::istream& in, std::ostream& out);
};

std::ostream& operator<<(std::ostream& out, const ASTDump::Blob& blob);

}  // namespace debug
}  // namespace ast::lib
