#include "annotation.hpp"
#include "astinst.hpp"
#include "ast.hpp"
#include <variant>

using ast::lib::AST;

template <ast::lib::concepts::is_ast TAst = AST<>>
auto clone(TAst& ast, ast::lib::concepts::is_ast_ptr auto ptr) {
    return ast.visit(ptr)([](auto obj) {
        using label_t = decltype(obj);
        // TODO after decomposition
    });
}

/*

FILE:

// (power (3) (3)) (successor) (0)
(l a. l b. l s. l z. a b s z) (l s. l z. s (s (s z))) (l s. l z. s (s (s z)))(l b. l f. f (l a. a) b) (l z. z);

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

std::pair<ast::lib::AST<>, ast::Program<ast::lib::AST<>>> pseudo_parse() {
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

}

int main() {
    
}
