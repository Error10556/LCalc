#include "astlib-debug-inst.hpp"

namespace ast::lib::debug {

template ASTDump::ASTDump(const AST<TravTime>& ast);
template void ASTDump::generate_labels<AST<TravTime>, LC::Program<AST<TravTime>>>(AST<TravTime>&, LC::Program<AST<TravTime>>);
template void ASTDump::generate_labels<AST<TravTime>, LC::Expr<AST<TravTime>>>(AST<TravTime>&, LC::Expr<AST<TravTime>>);
template void ASTDump::generate_labels<AST<TravTime>, lib::ASTPtr<LC::ListExpr<AST<TravTime>>>>(AST<TravTime>&, lib::ASTPtr<LC::ListExpr<AST<TravTime>>>);

}
