#pragma once
#include "ast.hpp"
#include "annotation.hpp"

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

extern template class lib::AST<TravTime>;
extern template class Abstraction<lib::AST<TravTime>>;
extern template class Application<lib::AST<TravTime>>;
extern template class Variable<lib::AST<TravTime>>;
extern template class StringExpr<lib::AST<TravTime>>;
extern template class IntegerExpr<lib::AST<TravTime>>;
extern template class DoubleExpr<lib::AST<TravTime>>;
extern template class CharExpr<lib::AST<TravTime>>;
extern template class SpecialExpr<lib::AST<TravTime>>;
extern template class AProgram<lib::AST<TravTime>>;

}
