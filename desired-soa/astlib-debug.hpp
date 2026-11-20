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

template <class T>
struct ast_obj_name {
    static constexpr std::string get_name() {
        using std::literals::operator""s;
        return "(typeid)"s + typeid(T).name();
    }
};

template <class T>
    requires(traits::ast_class_type_v<T> != traits::ASTClassType::Unknown)
struct ast_obj_name<T> {
    static constexpr std::string get_name() { return traits::lbnf_name_v<T>; }
};

class ASTDump {
    static std::string show_argument(std::string s);
    static std::string show_argument(uint32_t v);

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
    std::vector<std::string> tree_labels;
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
        tree_labels.resize(tree.size());
        extras = detail::AST_privates::extra(ast);
        strings = detail::AST_privates::string_tokens(ast);
        annot_names = {ast_obj_name<Annots>::get_name()...};
        [[maybe_unused]] auto blobify_vector = []<class T>(const std::vector<T>& v) {
            std::vector<Blob> res;
            res.reserve(v.size());
            for (const T& item : v) res.emplace_back(item);
            return res;
        };
        annots.reserve(sizeof...(Annots));
        (annots.push_back(blobify_vector(detail::AST_privates::annots<Annots>(ast))), ...);
    }

    void generate_labels(concepts::is_ast auto& ast, concepts::is_ast_ptr auto ptr) {
        std::string& res = tree_labels[detail::ASTPtr_privates::Index(ptr)];
        if (res.size()) return;
        auto to_str = [](auto val) {
            using val_t = decltype(val);
            if constexpr (std::is_same_v<val_t, std::string>)
                return show_argument(val);
            else if constexpr (concepts::is_ast_ptr<val_t>)
                return show_argument(detail::ASTPtr_privates::Index(val));
            else
                return std::to_string(val);
        };
        if constexpr (concepts::is_ast_variant_ptr<decltype(ptr)>) {
            ast.visit(ptr) | [&](auto node) {
                using obj_t = decltype(node);
                res = traits::lbnf_name_v<obj_t>;
                res.push_back('(');
                res += [&]<size_t... Indices>(std::index_sequence<Indices...>) {
                    return (... + [&] {
                        auto cur = node.template get<Indices>();
                        if constexpr (concepts::is_ast_ptr<decltype(cur)>)
                            generate_labels(ast, cur);
                        if constexpr (Indices > 0)
                            return ", " + to_str(cur);
                        else
                            return to_str(cur);
                    }());
                }(std::make_index_sequence<std::tuple_size_v<obj_t>>());
                res.push_back(')');
            };
        } else {  // array
            const auto node = ptr.get(ast);
            res = traits::lbnf_name_v<std::remove_const_t<decltype(node)>>;
            res.push_back('[');
            res.append(std::to_string(std::size(node)));
            res.append("]{");
            bool first = true;
            for (auto i : node) {
                if constexpr (concepts::is_ast_ptr<decltype(i)>) generate_labels(ast, i);
                if (first)
                    first = false;
                else
                    res.append(", ");
                res.append(to_str(i));
            }
            res.push_back('}');
        }
    }

    void interact(std::istream& in, std::ostream& out);
};

std::ostream& operator<<(std::ostream& out, const ASTDump::Blob& blob);

}  // namespace debug
}  // namespace ast::lib
