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

template lib::AST<TravTime>::AST(AST<>&&);
template lib::AST<>::AST(AST<TravTime>&&);

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

}
