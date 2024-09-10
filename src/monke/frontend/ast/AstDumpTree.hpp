#pragma once

#include <iostream>

#include "Expr.hpp"
#include "Stmt.hpp"
#include "monke/frontend/ast/AstVisitor.hpp"

namespace monke {

struct AstDumpTree : public AstVisitor {
  std::ostream& out;
  AstDumpTree(std::ostream& out = std::cout) : out(out) {}

  void visit(BinOpExpr& expr) override {
    output("BinOpExpr") << BinOpSymbol[expr.op] << std::endl;
    indent();
    expr.lhs->visit(*this);
    expr.rhs->visit(*this);
    dedent();
  }
  void visit(IntegerExpr& expr) override {
    output("IntegerExpr") << expr.v << std::endl;
  }
  void visit(ExprStmt& stmt) override {
    output("ExprStmt") << std::endl;
    indent();
    stmt.expr->visit(*this);
    dedent();
  }

private:
  std::ostream& output(std::string_view name) {
    for (int i = 0; i < depth; ++i) {
      out << indentation;
    }
    out << name << ": ";
    return out;
  }
  void indent() {
    ++depth;
  };
  void dedent() {
    --depth;
  };

  std::string_view indentation = " ";
  int              depth       = 0;
};

} // namespace monke
