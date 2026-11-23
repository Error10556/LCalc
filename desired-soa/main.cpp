#include <iostream>
#include <string_view>
#include <utility>
#include <variant>

#include "annotation.hpp"
#include "ast.hpp"
#include "astinst.hpp"
#include "astlib-debug.hpp"
#include "astlib.hpp"
#include "classic-pretty-printer.hpp"

using ast::lib::AST;

/*

FILE:

// (power (3) (3)) (prepend 'id') ([])
(l a. l b. l s. l z. a b s z) (l s. l z. s (s (s z))) (l s. l z. s (s (s z)))
(l b. l f. f (l a. a) b) (l z. z);

str: "Привет!\x21\U0001F525";
chr: '🔥';
(l a. a)(l b. b);
dbl: 90.4e10;
dbl: 9e-9;
int: 78000;
<:123 (l a. (l a. a) a) 2:>;
<:1<:123 (l a. (l a. a) a) 2:>9:>;

// user-defined functions:
// id. Expr2 ::= "id" "<" Ident ">"
// define id varname = Abstraction varname (Variable varname)
// const. Expr2 ::= "const" "<" Ident ">"
// define const varname = Abstraction varname (id varname)
l _. id<a>;
const <a>;

*/

std::pair<AST<>, ast::Program<>> pseudo_parse() {
    using std::literals::operator""s;
    AST<> ast;
    // l a. l b. l s. l z. a b s z
    auto ppower = [&ast] {
        auto inner_app =
            ast::Application(
                ast,
                ast::Application(ast,
                                 ast::Application(ast, ast::Variable(ast, "a").pointer(),
                                                  ast::Variable(ast, "b").pointer())
                                     .pointer(),
                                 ast::Variable(ast, "s").pointer())
                    .pointer(),
                ast::Variable(ast, "z").pointer())
                .pointer();
        auto lz = ast::Abstraction(ast, "z", inner_app).pointer();
        auto ls = ast::Abstraction(ast, "s", lz).pointer();
        auto lb = ast::Abstraction(ast, "b", ls).pointer();
        auto la = ast::Abstraction(ast, "a", lb).pointer();
        return la;
    }();

    // l s. l z. s (s (s z))
    auto p3 = [&ast] {
        auto z = ast::Variable<>(ast, "z").pointer();
        auto inner_app = z;
        for (int i = 0; i < 3; i++)
            inner_app =
                ast::Application(ast, ast::Variable(ast, "s").pointer(), inner_app).pointer();
        auto lz = ast::Abstraction(ast, "z", inner_app).pointer();
        auto ls = ast::Abstraction(ast, "s", lz).pointer();
        return ls;
    }();

    auto p3_right = ast::lib::deep_copy<ast::lib::DFSOrder::PreOrder, true>(ast, ast, p3);

    // l b. l f. f (l a. a) b
    auto prepender = [&ast] {
        auto f = ast::Variable(ast, "f").pointer();
        auto id = ast::Abstraction(ast, "a", ast::Variable(ast, "a").pointer()).pointer();
        auto b = ast::Variable(ast, "b").pointer();
        auto lf = ast::Abstraction(
                      ast, "f",
                      ast::Application(ast, ast::Application(ast, f, id).pointer(), b).pointer())
                      .pointer();
        auto lb = ast::Abstraction(ast, "b", lf).pointer();
        return lb;
    }();

    // l z. z
    auto nul = ast::Abstraction(ast, "z", ast::Variable(ast, "z").pointer()).pointer();

    auto powerexpr =
        ast::Application(
            ast,
            ast::Application(
                ast,
                ast::Application(ast, ast::Application(ast, ppower, p3).pointer(), p3_right)
                    .pointer(),
                prepender)
                .pointer(),
            nul)
            .pointer();
    auto powerexpr2 = ast::lib::deep_copy(ast, ast, powerexpr);
    auto prog = ast::AProgram(ast, ast::ListExpr<>(ast, powerexpr, powerexpr2).pointer()).pointer();
    return {std::move(ast), prog};
}

int traverse(ast::lib::concepts::ast_annotated_by<TravTime> auto& ast,
             ast::lib::concepts::is_ast_ptr auto root, int nx_time) {
    root.template annotation<TravTime>(ast).tin = nx_time++;
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
    root.template annotation<TravTime>(ast).tout = nx_time++;
    return nx_time;
}

int main() {
    auto [ast, prog] = pseudo_parse();
    auto var = prog.get(ast)
                   .ListExpr_()
                   .get(ast)[0]
                   .read()
                   .unchecked_get<ast::Application<>>(ast)
                   .Expr_2()
                   .unchecked_get<ast::Abstraction<>>(ast)
                   .Expr_()
                   .unchecked_get<ast::Variable<>>(ast);
    {
        auto printer = ast::printing::ClassicPrettyPrinter<AST<>>(std::cout);
        ast.visit(prog) | printer;
        printer.OnNewLine().FlushWhitespace();
    }
    auto aast = std::move(ast).annotate<TravTime>();
    auto aprog = prog.update_annotations(aast);
    auto avar = var.update_annotations(aast);
    traverse(aast, aprog, 0);
    auto dump = ast::lib::debug::ASTDump(aast);
    dump.generate_labels(aast, prog);
    dump.interact(std::cin, std::cout);
}
