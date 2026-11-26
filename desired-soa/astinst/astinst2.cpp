#include "astinst.hpp"

namespace ast {

template class lib::AST<TravTime>;
namespace LC {
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

template LC::Program<lib::AST<TravTime>> lib::deep_copy<lib::DFSOrder::PreOrder, false, lib::AST<TravTime>, lib::AST<TravTime>, LC::Program<lib::AST<TravTime>>>(lib::AST<TravTime>&, lib::AST<TravTime>&, LC::Program<lib::AST<TravTime>> ptr);
template LC::Program<lib::AST<TravTime>> lib::deep_copy<lib::DFSOrder::PreOrder, true, lib::AST<TravTime>, lib::AST<TravTime>, LC::Program<lib::AST<TravTime>>>(lib::AST<TravTime>&, lib::AST<TravTime>&, LC::Program<lib::AST<TravTime>> ptr);
template LC::Program<lib::AST<TravTime>> lib::deep_copy<lib::DFSOrder::PostOrder, false, lib::AST<TravTime>, lib::AST<TravTime>, LC::Program<lib::AST<TravTime>>>(lib::AST<TravTime>&, lib::AST<TravTime>&, LC::Program<lib::AST<TravTime>> ptr);
template LC::Program<lib::AST<TravTime>> lib::deep_copy<lib::DFSOrder::PostOrder, true, lib::AST<TravTime>, lib::AST<TravTime>, LC::Program<lib::AST<TravTime>>>(lib::AST<TravTime>&, lib::AST<TravTime>&, LC::Program<lib::AST<TravTime>> ptr);
template LC::Expr<lib::AST<TravTime>> lib::deep_copy<lib::DFSOrder::PreOrder, false, lib::AST<TravTime>, lib::AST<TravTime>, LC::Expr<lib::AST<TravTime>>>(lib::AST<TravTime>&, lib::AST<TravTime>&, LC::Expr<lib::AST<TravTime>> ptr);
template LC::Expr<lib::AST<TravTime>> lib::deep_copy<lib::DFSOrder::PreOrder, true, lib::AST<TravTime>, lib::AST<TravTime>, LC::Expr<lib::AST<TravTime>>>(lib::AST<TravTime>&, lib::AST<TravTime>&, LC::Expr<lib::AST<TravTime>> ptr);
template LC::Expr<lib::AST<TravTime>> lib::deep_copy<lib::DFSOrder::PostOrder, false, lib::AST<TravTime>, lib::AST<TravTime>, LC::Expr<lib::AST<TravTime>>>(lib::AST<TravTime>&, lib::AST<TravTime>&, LC::Expr<lib::AST<TravTime>> ptr);
template LC::Expr<lib::AST<TravTime>> lib::deep_copy<lib::DFSOrder::PostOrder, true, lib::AST<TravTime>, lib::AST<TravTime>, LC::Expr<lib::AST<TravTime>>>(lib::AST<TravTime>&, lib::AST<TravTime>&, LC::Expr<lib::AST<TravTime>> ptr);
template lib::ASTPtr<LC::ListExpr<lib::AST<TravTime>>> lib::deep_copy<lib::DFSOrder::PreOrder, false, lib::AST<TravTime>, lib::AST<TravTime>, lib::ASTPtr<LC::ListExpr<lib::AST<TravTime>>>>(lib::AST<TravTime>&, lib::AST<TravTime>&, lib::ASTPtr<LC::ListExpr<lib::AST<TravTime>>> ptr);
template lib::ASTPtr<LC::ListExpr<lib::AST<TravTime>>> lib::deep_copy<lib::DFSOrder::PreOrder, true, lib::AST<TravTime>, lib::AST<TravTime>, lib::ASTPtr<LC::ListExpr<lib::AST<TravTime>>>>(lib::AST<TravTime>&, lib::AST<TravTime>&, lib::ASTPtr<LC::ListExpr<lib::AST<TravTime>>> ptr);
template lib::ASTPtr<LC::ListExpr<lib::AST<TravTime>>> lib::deep_copy<lib::DFSOrder::PostOrder, false, lib::AST<TravTime>, lib::AST<TravTime>, lib::ASTPtr<LC::ListExpr<lib::AST<TravTime>>>>(lib::AST<TravTime>&, lib::AST<TravTime>&, lib::ASTPtr<LC::ListExpr<lib::AST<TravTime>>> ptr);
template lib::ASTPtr<LC::ListExpr<lib::AST<TravTime>>> lib::deep_copy<lib::DFSOrder::PostOrder, true, lib::AST<TravTime>, lib::AST<TravTime>, lib::ASTPtr<LC::ListExpr<lib::AST<TravTime>>>>(lib::AST<TravTime>&, lib::AST<TravTime>&, lib::ASTPtr<LC::ListExpr<lib::AST<TravTime>>> ptr);

}
