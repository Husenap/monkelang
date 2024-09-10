#pragma once

namespace monke {

struct Stmt;
struct BinOpExpr;
struct IntegerExpr;
struct ExprStmt;

struct AstVisitor {
  virtual ~AstVisitor() {}

  virtual void visit(BinOpExpr& expr)   = 0;
  virtual void visit(IntegerExpr& expr) = 0;
  virtual void visit(ExprStmt& stmt)    = 0;
};

} // namespace monke
