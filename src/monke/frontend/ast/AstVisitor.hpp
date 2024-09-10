#pragma once

namespace monke {

struct IdDeclStmt;
struct StmtList;
struct FuncDeclStmt;
struct BlockStmt;
struct WhileStmt;
struct IfStmt;
struct ReadStmt;
struct PrintStmt;
struct ReturnStmt;
struct ExprStmt;

struct FuncCallExpr;
struct AssignmentExpr;
struct IdUseExpr;
struct BinOpExpr;
struct NumberExpr;

struct AstVisitor {
  virtual ~AstVisitor() {}

  virtual void visit(IdDeclStmt&) {}
  virtual void visit(StmtList&) {}
  virtual void visit(FuncDeclStmt&) {}
  virtual void visit(BlockStmt&) {}
  virtual void visit(WhileStmt&) {}
  virtual void visit(IfStmt&) {}
  virtual void visit(ReadStmt&) {}
  virtual void visit(PrintStmt&) {}
  virtual void visit(ReturnStmt&) {}
  virtual void visit(ExprStmt&) {}

  virtual void visit(FuncCallExpr&) {}
  virtual void visit(AssignmentExpr&) {}
  virtual void visit(IdUseExpr&) {}
  virtual void visit(BinOpExpr&) {}
  virtual void visit(NumberExpr&) {}
};

} // namespace monke
