#include "astlib-debug-inst.hpp"

namespace ast::lib::debug {

template ASTDump::ASTDump(const AST<>& ast);
template void ASTDump::generate_labels<AST<>, LC::Program<AST<>>>(AST<>&, LC::Program<AST<>>);
template void ASTDump::generate_labels<AST<>, LC::Expr<AST<>>>(AST<>&, LC::Expr<AST<>>);
template void ASTDump::generate_labels<AST<>, lib::NodePtr<LC::ListExpr<AST<>>>>(AST<>&, lib::NodePtr<LC::ListExpr<AST<>>>);

template ASTDump::ASTDump(const AST<TravTime>& ast);
template void ASTDump::generate_labels<AST<TravTime>, LC::Program<AST<TravTime>>>(AST<TravTime>&, LC::Program<AST<TravTime>>);
template void ASTDump::generate_labels<AST<TravTime>, LC::Expr<AST<TravTime>>>(AST<TravTime>&, LC::Expr<AST<TravTime>>);
template void ASTDump::generate_labels<AST<TravTime>, lib::NodePtr<LC::ListExpr<AST<TravTime>>>>(AST<TravTime>&, lib::NodePtr<LC::ListExpr<AST<TravTime>>>);

}
