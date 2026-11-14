#pragma once
#include "ast.hpp"

namespace ast {

extern template class lib::AST<>;
extern template class Abstraction<lib::AST<>>;
extern template class Application<lib::AST<>>;
extern template class Variable<lib::AST<>>;
extern template class StringExpr<lib::AST<>>;
extern template class IntegerExpr<lib::AST<>>;
extern template class DoubleExpr<lib::AST<>>;
extern template class CharExpr<lib::AST<>>;
extern template class SpecialExpr<lib::AST<>>;
extern template class AProgram<lib::AST<>>;

}
