#pragma once

#include <memory>

#include "Node.hpp"

namespace monke {

struct Expr : Node {};

struct IntegerExpr : Expr {
  IntegerExpr(int i) : v(i) {}
  int v;

  void visit(AstVisitor& v) override;
};

enum BinOp {
  Add,
  Sub,
};
static const char* BinOpSymbol[] = {"+", "-"};
struct BinOpExpr : Expr {
  BinOpExpr(BinOp op, std::unique_ptr<Expr> lhs, std::unique_ptr<Expr> rhs)
      : op(op), lhs(std::move(lhs)), rhs(std::move(rhs)) {}

  BinOp                 op;
  std::unique_ptr<Expr> lhs;
  std::unique_ptr<Expr> rhs;

  void visit(AstVisitor& v) override;
};

} // namespace monke
