#include "ast.hpp"
#include "classic-pretty-printer.hpp"
#include "annotation.hpp"

namespace ast {

template class lib::AST<>;
template class Abstraction<lib::AST<>>;
template class Application<lib::AST<>>;
template class Variable<lib::AST<>>;
template class StringExpr<lib::AST<>>;
template class IntegerExpr<lib::AST<>>;
template class DoubleExpr<lib::AST<>>;
template class CharExpr<lib::AST<>>;
template class SpecialExpr<lib::AST<>>;
template class AProgram<lib::AST<>>;
template class printing::ClassicPrettyPrinter<lib::AST<>>;

template class lib::AST<TravTime>;
template class Abstraction<lib::AST<TravTime>>;
template class Application<lib::AST<TravTime>>;
template class Variable<lib::AST<TravTime>>;
template class StringExpr<lib::AST<TravTime>>;
template class IntegerExpr<lib::AST<TravTime>>;
template class DoubleExpr<lib::AST<TravTime>>;
template class CharExpr<lib::AST<TravTime>>;
template class SpecialExpr<lib::AST<TravTime>>;
template class AProgram<lib::AST<TravTime>>;
template class printing::ClassicPrettyPrinter<lib::AST<TravTime>>;

}
