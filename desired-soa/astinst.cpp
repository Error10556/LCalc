#include "ast.hpp"

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

}
