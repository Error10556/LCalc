#pragma once
#include "astlib-debug.hpp"
#include "annotation.hpp"
#include "ast.hpp"

namespace ast::lib::debug {

extern template ASTDump::ASTDump(const AST<>& ast);
extern template ASTDump::ASTDump(const AST<TravTime>& ast);
extern template void ASTDump::generate_labels<AST<>, LC::Program<AST<>>>(AST<>&, LC::Program<AST<>>);
extern template void ASTDump::generate_labels<AST<>, LC::Expr<AST<>>>(AST<>&, LC::Expr<AST<>>);
extern template void ASTDump::generate_labels<AST<>, lib::ASTPtr<LC::ListExpr<AST<>>>>(AST<>&, lib::ASTPtr<LC::ListExpr<AST<>>>);
extern template void ASTDump::generate_labels<AST<TravTime>, LC::Program<AST<TravTime>>>(AST<TravTime>&, LC::Program<AST<TravTime>>);
extern template void ASTDump::generate_labels<AST<TravTime>, LC::Expr<AST<TravTime>>>(AST<TravTime>&, LC::Expr<AST<TravTime>>);
extern template void ASTDump::generate_labels<AST<TravTime>, lib::ASTPtr<LC::ListExpr<AST<TravTime>>>>(AST<TravTime>&, lib::ASTPtr<LC::ListExpr<AST<TravTime>>>);

}
