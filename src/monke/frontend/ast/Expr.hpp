#pragma once

#include <memory>
#include <string>
#include <vector>

#include "Node.hpp"

namespace monke {

struct Expr : Node {};

struct NumberExpr : Expr {
  NumberExpr(double value) : value(value) {}

  double value{};

  void visit(AstVisitor& v) override;
};

struct IdUseExpr : Expr {
  IdUseExpr(std::string id) : id(id) {}

  std::string id{};

  void visit(AstVisitor& v) override;
};

struct AssignmentExpr : Expr {
  AssignmentExpr(std::string id, std::unique_ptr<Expr> value) : id(id), value(std::move(value)) {}
  std::string           id;
  std::unique_ptr<Expr> value;

  void visit(AstVisitor& v) override;
};

struct FuncCallExpr : Expr {
  FuncCallExpr(std::string func_name, std::vector<std::unique_ptr<Expr>> args = {})
      : func_name(func_name), args(std::move(args)) {}
  std::string                        func_name{};
  std::vector<std::unique_ptr<Expr>> args{};

  void visit(AstVisitor& v) override;
};

enum BinOp {
  Equal,
  NotEqual,
  LessThan,
  LessOrEqual,
  GreaterThan,
  GreaterOrEqual,
  Add,
  Sub,
  Mul,
  Div,
  Mod,
};
static const char* BinOpSymbol[] = {"==", "!=", "<", "<=", ">", ">=", "+", "-", "*", "/", "%"};
struct BinOpExpr : Expr {
  BinOpExpr(BinOp op, std::unique_ptr<Expr> lhs, std::unique_ptr<Expr> rhs)
      : op(op), lhs(std::move(lhs)), rhs(std::move(rhs)) {}

  BinOp                 op;
  std::unique_ptr<Expr> lhs;
  std::unique_ptr<Expr> rhs;

  void visit(AstVisitor& v) override;
};

} // namespace monke
