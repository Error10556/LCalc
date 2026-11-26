#include <iostream>
#include <utility>

#include "annotation.hpp"
#include "ast.hpp"
#include "astinst/astinst.hpp"
#include "astinst/astlib-debug-inst.hpp"
#include "classic-pretty-printer.hpp"

using ast::lib::AST;
using namespace ast::LC;

/*

FILE:

// (power (3) (3)) (prepend 'id') ([])
(l a. l b. l s. l z. a b s z) (l s. l z. s (s (s z))) (l s. l z. s (s (s z)))
(l b. l f. f (l a. a) b) (l z. z);

*/

std::pair<AST<>, Program<>> pseudo_parse() {
    using std::literals::operator""s;
    AST<> ast;
    // l a. l b. l s. l z. a b s z
    auto ppower = [&ast] {
        auto inner_app =
            Application(
                ast,
                Application(ast,
                                 Application(ast, Variable(ast, "a").pointer(),
                                                  Variable(ast, "b").pointer())
                                     .pointer(),
                                 Variable(ast, "s").pointer())
                    .pointer(),
                Variable(ast, "z").pointer())
                .pointer();
        auto lz = Abstraction(ast, "z", inner_app).pointer();
        auto ls = Abstraction(ast, "s", lz).pointer();
        auto lb = Abstraction(ast, "b", ls).pointer();
        auto la = Abstraction(ast, "a", lb).pointer();
        return la;
    }();

    // l s. l z. s (s (s z))
    auto p3 = [&ast] {
        auto z = Variable<>(ast, "z").pointer();
        auto inner_app = z;
        for (int i = 0; i < 3; i++)
            inner_app =
                Application(ast, Variable(ast, "s").pointer(), inner_app).pointer();
        auto lz = Abstraction(ast, "z", inner_app).pointer();
        auto ls = Abstraction(ast, "s", lz).pointer();
        return ls;
    }();

    auto p3_right = ast::lib::deep_copy<ast::lib::DFSOrder::PreOrder, true>(ast, ast, p3);

    // l b. l f. f (l a. a) b
    auto prepender = [&ast] {
        auto f = Variable(ast, "f").pointer();
        auto id = Abstraction(ast, "a", Variable(ast, "a").pointer()).pointer();
        auto b = Variable(ast, "b").pointer();
        auto lf = Abstraction(
                      ast, "f",
                      Application(ast, Application(ast, f, id).pointer(), b).pointer())
                      .pointer();
        auto lb = Abstraction(ast, "b", lf).pointer();
        return lb;
    }();

    // l z. z
    auto nul = Abstraction(ast, "z", Variable(ast, "z").pointer()).pointer();

    auto powerexpr =
        Application(
            ast,
            Application(
                ast,
                Application(ast, Application(ast, ppower, p3).pointer(), p3_right)
                    .pointer(),
                prepender)
                .pointer(),
            nul)
            .pointer();
    auto powerexpr2 = ast::lib::deep_copy(ast, ast, powerexpr);
    auto prog = AProgram(ast, ListExpr<>(ast, powerexpr, powerexpr2).pointer()).pointer();
    return {std::move(ast), prog};
}

static_assert(std::is_same_v<ast::lib::detail::astptr_with_updated_annotations_t<ast::lib::ASTPtr<ListExpr<>>, AST<TravTime>>, ast::lib::ASTPtr<ListExpr<AST<TravTime>>>>);

int traverse(ast::lib::concepts::ast_annotated_by<TravTime> auto& ast,
             ast::lib::concepts::is_ast_ptr auto root, int nx_time) {
    root.template annotation<TravTime>(ast).tin = nx_time++;
    if constexpr (ast::lib::concepts::is_ast_array_ptr<decltype(root)>) {
        const auto arr = ast.get(root);
        for (auto p : arr) nx_time = traverse(ast, p, nx_time);
    } else {
        root.visit(ast) | [&](auto node) {
            static constexpr size_t n = std::tuple_size_v<decltype(node)>;
            [&]<size_t... Indices>(std::index_sequence<Indices...>) {
                (
                    [&] {
                        if constexpr (ast::lib::concepts::is_ast_ptr<
                                          std::tuple_element_t<Indices, decltype(node)>>) {
                            nx_time = traverse(ast, node.template get<Indices>(), nx_time);
                        }
                    }(),
                    ...);
            }(std::make_index_sequence<n>());
        };
    }
    root.template annotation<TravTime>(ast).tout = nx_time++;
    return nx_time;
}

int main() {
    auto [ast, prog] = pseudo_parse();
    auto var = prog.get(ast)
                   .ListExpr_()
                   .get(ast)[0]
                   .read()
                   .unchecked_get<Application<>>(ast)
                   .Expr_2()
                   .unchecked_get<Abstraction<>>(ast)
                   .Expr_()
                   .unchecked_get<Variable<>>(ast);
    {
        auto printer = printing::ClassicPrettyPrinter<AST<>>(std::cout);
        ast.visit(prog) | printer;
        printer.OnNewLine().FlushWhitespace();
    }
    auto aast = std::move(ast).annotate<TravTime>();
    auto aprog = prog.update_annotations(aast);
    [[maybe_unused]] auto avar = var.update_annotations(aast);
    traverse(aast, aprog, 0);
    auto dump = ast::lib::debug::ASTDump(aast);
    // CHANGE 5
    dump.generate_labels(aast, aprog);
    dump.interact(std::cin, std::cout);
}
