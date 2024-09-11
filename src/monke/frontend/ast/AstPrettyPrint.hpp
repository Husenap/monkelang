#pragma once

#include <iostream>

#include "Expr.hpp"
#include "Stmt.hpp"

namespace monke {

struct AstPrettyPrint : public StmtVisitor {
  std::ostream& out;
  AstPrettyPrint(std::ostream& out = std::cout) : out(out) {}
  virtual void visit(IdDeclStmt&) {};
  virtual void visit(StmtList&) {};
  virtual void visit(FuncDeclStmt&) {};
  virtual void visit(BlockStmt&) {};
  virtual void visit(WhileStmt&) {};
  virtual void visit(IfStmt&) {};
  virtual void visit(ReadStmt&) {};
  virtual void visit(PrintStmt&) {};
  virtual void visit(ReturnStmt&) {};
  virtual void visit(ExprStmt&) {};
};

} // namespace monke
