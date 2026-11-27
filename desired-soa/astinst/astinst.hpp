#pragma once
#include "../ast.hpp"
#include "../annotation.hpp"
#include "../classic-pretty-printer.hpp"

namespace ast {

extern template class lib::AST<>;
namespace LC {
extern template class Abstraction<lib::AST<>>;
extern template class Application<lib::AST<>>;
extern template class Variable<lib::AST<>>;
extern template class StringExpr<lib::AST<>>;
extern template class IntegerExpr<lib::AST<>>;
extern template class DoubleExpr<lib::AST<>>;
extern template class CharExpr<lib::AST<>>;
extern template class SpecialExpr<lib::AST<>>;
extern template class AProgram<lib::AST<>>;
extern template class printing::ClassicPrettyPrinter<lib::AST<>>;
}

extern template class lib::AST<TravTime>;
namespace LC {
extern template class Abstraction<lib::AST<TravTime>>;
extern template class Application<lib::AST<TravTime>>;
extern template class Variable<lib::AST<TravTime>>;
extern template class StringExpr<lib::AST<TravTime>>;
extern template class IntegerExpr<lib::AST<TravTime>>;
extern template class DoubleExpr<lib::AST<TravTime>>;
extern template class CharExpr<lib::AST<TravTime>>;
extern template class SpecialExpr<lib::AST<TravTime>>;
extern template class AProgram<lib::AST<TravTime>>;
extern template class printing::ClassicPrettyPrinter<lib::AST<TravTime>>;
}

extern template LC::Program<lib::AST<>> lib::deep_copy<lib::DFSOrder::PreOrder, false, lib::AST<>, lib::AST<>, LC::Program<lib::AST<>>>(lib::AST<>&, lib::AST<>&, LC::Program<lib::AST<>> ptr);
extern template LC::Program<lib::AST<>> lib::deep_copy<lib::DFSOrder::PreOrder, true, lib::AST<>, lib::AST<>, LC::Program<lib::AST<>>>(lib::AST<>&, lib::AST<>&, LC::Program<lib::AST<>> ptr);
extern template LC::Program<lib::AST<>> lib::deep_copy<lib::DFSOrder::PostOrder, false, lib::AST<>, lib::AST<>, LC::Program<lib::AST<>>>(lib::AST<>&, lib::AST<>&, LC::Program<lib::AST<>> ptr);
extern template LC::Program<lib::AST<>> lib::deep_copy<lib::DFSOrder::PostOrder, true, lib::AST<>, lib::AST<>, LC::Program<lib::AST<>>>(lib::AST<>&, lib::AST<>&, LC::Program<lib::AST<>> ptr);
extern template LC::Expr<lib::AST<>> lib::deep_copy<lib::DFSOrder::PreOrder, false, lib::AST<>, lib::AST<>, LC::Expr<lib::AST<>>>(lib::AST<>&, lib::AST<>&, LC::Expr<lib::AST<>> ptr);
extern template LC::Expr<lib::AST<>> lib::deep_copy<lib::DFSOrder::PreOrder, true, lib::AST<>, lib::AST<>, LC::Expr<lib::AST<>>>(lib::AST<>&, lib::AST<>&, LC::Expr<lib::AST<>> ptr);
extern template LC::Expr<lib::AST<>> lib::deep_copy<lib::DFSOrder::PostOrder, false, lib::AST<>, lib::AST<>, LC::Expr<lib::AST<>>>(lib::AST<>&, lib::AST<>&, LC::Expr<lib::AST<>> ptr);
extern template LC::Expr<lib::AST<>> lib::deep_copy<lib::DFSOrder::PostOrder, true, lib::AST<>, lib::AST<>, LC::Expr<lib::AST<>>>(lib::AST<>&, lib::AST<>&, LC::Expr<lib::AST<>> ptr);
extern template lib::NodePtr<LC::ListExpr<lib::AST<>>> lib::deep_copy<lib::DFSOrder::PreOrder, false, lib::AST<>, lib::AST<>, lib::NodePtr<LC::ListExpr<lib::AST<>>>>(lib::AST<>&, lib::AST<>&, lib::NodePtr<LC::ListExpr<lib::AST<>>> ptr);
extern template lib::NodePtr<LC::ListExpr<lib::AST<>>> lib::deep_copy<lib::DFSOrder::PreOrder, true, lib::AST<>, lib::AST<>, lib::NodePtr<LC::ListExpr<lib::AST<>>>>(lib::AST<>&, lib::AST<>&, lib::NodePtr<LC::ListExpr<lib::AST<>>> ptr);
extern template lib::NodePtr<LC::ListExpr<lib::AST<>>> lib::deep_copy<lib::DFSOrder::PostOrder, false, lib::AST<>, lib::AST<>, lib::NodePtr<LC::ListExpr<lib::AST<>>>>(lib::AST<>&, lib::AST<>&, lib::NodePtr<LC::ListExpr<lib::AST<>>> ptr);
extern template lib::NodePtr<LC::ListExpr<lib::AST<>>> lib::deep_copy<lib::DFSOrder::PostOrder, true, lib::AST<>, lib::AST<>, lib::NodePtr<LC::ListExpr<lib::AST<>>>>(lib::AST<>&, lib::AST<>&, lib::NodePtr<LC::ListExpr<lib::AST<>>> ptr);

extern template LC::Program<lib::AST<TravTime>> lib::deep_copy<lib::DFSOrder::PreOrder, false, lib::AST<TravTime>, lib::AST<TravTime>, LC::Program<lib::AST<TravTime>>>(lib::AST<TravTime>&, lib::AST<TravTime>&, LC::Program<lib::AST<TravTime>> ptr);
extern template LC::Program<lib::AST<TravTime>> lib::deep_copy<lib::DFSOrder::PreOrder, true, lib::AST<TravTime>, lib::AST<TravTime>, LC::Program<lib::AST<TravTime>>>(lib::AST<TravTime>&, lib::AST<TravTime>&, LC::Program<lib::AST<TravTime>> ptr);
extern template LC::Program<lib::AST<TravTime>> lib::deep_copy<lib::DFSOrder::PostOrder, false, lib::AST<TravTime>, lib::AST<TravTime>, LC::Program<lib::AST<TravTime>>>(lib::AST<TravTime>&, lib::AST<TravTime>&, LC::Program<lib::AST<TravTime>> ptr);
extern template LC::Program<lib::AST<TravTime>> lib::deep_copy<lib::DFSOrder::PostOrder, true, lib::AST<TravTime>, lib::AST<TravTime>, LC::Program<lib::AST<TravTime>>>(lib::AST<TravTime>&, lib::AST<TravTime>&, LC::Program<lib::AST<TravTime>> ptr);
extern template LC::Expr<lib::AST<TravTime>> lib::deep_copy<lib::DFSOrder::PreOrder, false, lib::AST<TravTime>, lib::AST<TravTime>, LC::Expr<lib::AST<TravTime>>>(lib::AST<TravTime>&, lib::AST<TravTime>&, LC::Expr<lib::AST<TravTime>> ptr);
extern template LC::Expr<lib::AST<TravTime>> lib::deep_copy<lib::DFSOrder::PreOrder, true, lib::AST<TravTime>, lib::AST<TravTime>, LC::Expr<lib::AST<TravTime>>>(lib::AST<TravTime>&, lib::AST<TravTime>&, LC::Expr<lib::AST<TravTime>> ptr);
extern template LC::Expr<lib::AST<TravTime>> lib::deep_copy<lib::DFSOrder::PostOrder, false, lib::AST<TravTime>, lib::AST<TravTime>, LC::Expr<lib::AST<TravTime>>>(lib::AST<TravTime>&, lib::AST<TravTime>&, LC::Expr<lib::AST<TravTime>> ptr);
extern template LC::Expr<lib::AST<TravTime>> lib::deep_copy<lib::DFSOrder::PostOrder, true, lib::AST<TravTime>, lib::AST<TravTime>, LC::Expr<lib::AST<TravTime>>>(lib::AST<TravTime>&, lib::AST<TravTime>&, LC::Expr<lib::AST<TravTime>> ptr);
extern template lib::NodePtr<LC::ListExpr<lib::AST<TravTime>>> lib::deep_copy<lib::DFSOrder::PreOrder, false, lib::AST<TravTime>, lib::AST<TravTime>, lib::NodePtr<LC::ListExpr<lib::AST<TravTime>>>>(lib::AST<TravTime>&, lib::AST<TravTime>&, lib::NodePtr<LC::ListExpr<lib::AST<TravTime>>> ptr);
extern template lib::NodePtr<LC::ListExpr<lib::AST<TravTime>>> lib::deep_copy<lib::DFSOrder::PreOrder, true, lib::AST<TravTime>, lib::AST<TravTime>, lib::NodePtr<LC::ListExpr<lib::AST<TravTime>>>>(lib::AST<TravTime>&, lib::AST<TravTime>&, lib::NodePtr<LC::ListExpr<lib::AST<TravTime>>> ptr);
extern template lib::NodePtr<LC::ListExpr<lib::AST<TravTime>>> lib::deep_copy<lib::DFSOrder::PostOrder, false, lib::AST<TravTime>, lib::AST<TravTime>, lib::NodePtr<LC::ListExpr<lib::AST<TravTime>>>>(lib::AST<TravTime>&, lib::AST<TravTime>&, lib::NodePtr<LC::ListExpr<lib::AST<TravTime>>> ptr);
extern template lib::NodePtr<LC::ListExpr<lib::AST<TravTime>>> lib::deep_copy<lib::DFSOrder::PostOrder, true, lib::AST<TravTime>, lib::AST<TravTime>, lib::NodePtr<LC::ListExpr<lib::AST<TravTime>>>>(lib::AST<TravTime>&, lib::AST<TravTime>&, lib::NodePtr<LC::ListExpr<lib::AST<TravTime>>> ptr);

}
