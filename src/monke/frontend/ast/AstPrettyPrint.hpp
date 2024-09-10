#pragma once

#include <iostream>

#include "Expr.hpp"
#include "Stmt.hpp"
#include "monke/frontend/ast/AstVisitor.hpp"

namespace monke {

struct AstPrettyPrint : public AstVisitor {
  std::ostream& out;
  AstPrettyPrint(std::ostream& out = std::cout) : out(out) {}

  void visit(BinOpExpr& expr) override {
    expr.lhs->visit(*this);
    out << " " << BinOpSymbol[expr.op] << " ";
    expr.rhs->visit(*this);
  }
  void visit(IntegerExpr& expr) override {
    out << expr.v;
  }
  void visit(ExprStmt& stmt) override {
    stmt.expr->visit(*this);
    out << ";" << std::endl;
  }
};

} // namespace monke
