#pragma once

#include "Expr.hpp"
#include "Node.hpp"

namespace monke {

struct Stmt : Node {};

struct ExprStmt : Stmt {
  ExprStmt(std::unique_ptr<Expr> expr) : expr(std::move(expr)) {}
  std::unique_ptr<Expr> expr;

  void visit(AstVisitor& v) override;
};

} // namespace monke
