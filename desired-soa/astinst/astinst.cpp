#include "astinst.hpp"

namespace ast {

template class lib::AST<>;
namespace LC {
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
}

template LC::Program<lib::AST<>> lib::deep_copy<lib::DFSOrder::PreOrder, false, lib::AST<>, lib::AST<>, LC::Program<lib::AST<>>>(lib::AST<>&, lib::AST<>&, LC::Program<lib::AST<>> ptr);
template LC::Program<lib::AST<>> lib::deep_copy<lib::DFSOrder::PreOrder, true, lib::AST<>, lib::AST<>, LC::Program<lib::AST<>>>(lib::AST<>&, lib::AST<>&, LC::Program<lib::AST<>> ptr);
template LC::Program<lib::AST<>> lib::deep_copy<lib::DFSOrder::PostOrder, false, lib::AST<>, lib::AST<>, LC::Program<lib::AST<>>>(lib::AST<>&, lib::AST<>&, LC::Program<lib::AST<>> ptr);
template LC::Program<lib::AST<>> lib::deep_copy<lib::DFSOrder::PostOrder, true, lib::AST<>, lib::AST<>, LC::Program<lib::AST<>>>(lib::AST<>&, lib::AST<>&, LC::Program<lib::AST<>> ptr);
template LC::Expr<lib::AST<>> lib::deep_copy<lib::DFSOrder::PreOrder, false, lib::AST<>, lib::AST<>, LC::Expr<lib::AST<>>>(lib::AST<>&, lib::AST<>&, LC::Expr<lib::AST<>> ptr);
template LC::Expr<lib::AST<>> lib::deep_copy<lib::DFSOrder::PreOrder, true, lib::AST<>, lib::AST<>, LC::Expr<lib::AST<>>>(lib::AST<>&, lib::AST<>&, LC::Expr<lib::AST<>> ptr);
template LC::Expr<lib::AST<>> lib::deep_copy<lib::DFSOrder::PostOrder, false, lib::AST<>, lib::AST<>, LC::Expr<lib::AST<>>>(lib::AST<>&, lib::AST<>&, LC::Expr<lib::AST<>> ptr);
template LC::Expr<lib::AST<>> lib::deep_copy<lib::DFSOrder::PostOrder, true, lib::AST<>, lib::AST<>, LC::Expr<lib::AST<>>>(lib::AST<>&, lib::AST<>&, LC::Expr<lib::AST<>> ptr);
template lib::ASTPtr<LC::ListExpr<lib::AST<>>> lib::deep_copy<lib::DFSOrder::PreOrder, false, lib::AST<>, lib::AST<>, lib::ASTPtr<LC::ListExpr<lib::AST<>>>>(lib::AST<>&, lib::AST<>&, lib::ASTPtr<LC::ListExpr<lib::AST<>>> ptr);
template lib::ASTPtr<LC::ListExpr<lib::AST<>>> lib::deep_copy<lib::DFSOrder::PreOrder, true, lib::AST<>, lib::AST<>, lib::ASTPtr<LC::ListExpr<lib::AST<>>>>(lib::AST<>&, lib::AST<>&, lib::ASTPtr<LC::ListExpr<lib::AST<>>> ptr);
template lib::ASTPtr<LC::ListExpr<lib::AST<>>> lib::deep_copy<lib::DFSOrder::PostOrder, false, lib::AST<>, lib::AST<>, lib::ASTPtr<LC::ListExpr<lib::AST<>>>>(lib::AST<>&, lib::AST<>&, lib::ASTPtr<LC::ListExpr<lib::AST<>>> ptr);
template lib::ASTPtr<LC::ListExpr<lib::AST<>>> lib::deep_copy<lib::DFSOrder::PostOrder, true, lib::AST<>, lib::AST<>, lib::ASTPtr<LC::ListExpr<lib::AST<>>>>(lib::AST<>&, lib::AST<>&, lib::ASTPtr<LC::ListExpr<lib::AST<>>> ptr);

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
