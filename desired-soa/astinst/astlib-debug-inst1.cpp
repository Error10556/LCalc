#include "astlib-debug-inst.hpp"

namespace ast::lib::debug {

template ASTDump::ASTDump(const AST<>& ast);
template void ASTDump::generate_labels<AST<>, LC::Program<AST<>>>(AST<>&, LC::Program<AST<>>);
template void ASTDump::generate_labels<AST<>, LC::Expr<AST<>>>(AST<>&, LC::Expr<AST<>>);
template void ASTDump::generate_labels<AST<>, lib::ASTPtr<LC::ListExpr<AST<>>>>(AST<>&, lib::ASTPtr<LC::ListExpr<AST<>>>);

}
