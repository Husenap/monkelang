#pragma once

#include <memory>
#include <string>
#include <vector>

namespace monke {

struct FuncCallExpr;
struct AssignmentExpr;
struct IdUseExpr;
struct BinOpExpr;
struct NumberExpr;
struct BooleanExpr;
struct StringExpr;

struct ExprVisitor {
  virtual void visit(FuncCallExpr&)   = 0;
  virtual void visit(AssignmentExpr&) = 0;
  virtual void visit(IdUseExpr&)      = 0;
  virtual void visit(BinOpExpr&)      = 0;
  virtual void visit(NumberExpr&)     = 0;
  virtual void visit(BooleanExpr&)    = 0;
  virtual void visit(StringExpr&)     = 0;
};

struct Expr {
  virtual ~Expr()                    = default;
  virtual void visit(ExprVisitor& v) = 0;
};

struct NumberExpr : Expr {
  NumberExpr(double value) : value(value) {}

  double value{};

  void visit(ExprVisitor& v) override {
    v.visit(*this);
  }
};

struct BooleanExpr : Expr {
  BooleanExpr(bool value) : value(value) {}
  bool value{};

  void visit(ExprVisitor& v) override {
    v.visit(*this);
  }
};

struct StringExpr : Expr {
  StringExpr(std::string value) : value(value) {}
  std::string value{};

  void visit(ExprVisitor& v) override {
    v.visit(*this);
  }
};

struct IdUseExpr : Expr {
  IdUseExpr(std::string id) : id(id) {}

  std::string id{};

  void visit(ExprVisitor& v) override {
    v.visit(*this);
  }
};

struct AssignmentExpr : Expr {
  AssignmentExpr(std::string id, std::unique_ptr<Expr> value) : id(id), value(std::move(value)) {}
  std::string           id;
  std::unique_ptr<Expr> value;

  void visit(ExprVisitor& v) override {
    v.visit(*this);
  }
};

struct FuncCallExpr : Expr {
  FuncCallExpr(std::unique_ptr<Expr> callee, std::vector<std::unique_ptr<Expr>> args = {})
      : callee(std::move(callee)), args(std::move(args)) {}
  std::unique_ptr<Expr>              callee{};
  std::vector<std::unique_ptr<Expr>> args{};

  void visit(ExprVisitor& v) override {
    v.visit(*this);
  }
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

  void visit(ExprVisitor& v) override {
    v.visit(*this);
  }
};

} // namespace monke
