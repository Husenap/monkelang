#pragma once

#include <string>
#include <vector>

#include "Expr.hpp"

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

struct StmtVisitor {
  virtual void visit(IdDeclStmt&)   = 0;
  virtual void visit(StmtList&)     = 0;
  virtual void visit(FuncDeclStmt&) = 0;
  virtual void visit(BlockStmt&)    = 0;
  virtual void visit(WhileStmt&)    = 0;
  virtual void visit(IfStmt&)       = 0;
  virtual void visit(ReadStmt&)     = 0;
  virtual void visit(PrintStmt&)    = 0;
  virtual void visit(ReturnStmt&)   = 0;
  virtual void visit(ExprStmt&)     = 0;
};

struct Stmt {
  virtual ~Stmt()                    = default;
  virtual void visit(StmtVisitor& v) = 0;
};

struct IdDeclStmt : Stmt {
  IdDeclStmt(std::string type, std::string id, std::unique_ptr<Expr> value)
      : type(type), id(id), value(std::move(value)) {}
  std::string           type{};
  std::string           id{};
  std::unique_ptr<Expr> value;

  void visit(StmtVisitor& v) override {
    v.visit(*this);
  }
};

struct StmtList : Stmt {
  StmtList(std::vector<std::unique_ptr<Stmt>> stmts = {}) : stmts(std::move(stmts)) {}
  StmtList(std::unique_ptr<Stmt> stmt) {
    stmts.push_back(std::move(stmt));
  }
  std::vector<std::unique_ptr<Stmt>> stmts;

  void push_back(std::unique_ptr<Stmt>&& stmt) {
    stmts.push_back(std::move(stmt));
  }

  void visit(StmtVisitor& v) override {
    v.visit(*this);
  }
};

struct FuncDeclStmt : Stmt {
  FuncDeclStmt(std::string                        func_name,
               std::vector<std::unique_ptr<Stmt>> params,
               std::string                        ret_type,
               std::unique_ptr<Stmt>              body)
      : func_name(func_name), params(std::move(params)), ret_type(ret_type), body(std::move(body)) {}
  FuncDeclStmt(std::string func_name, std::string ret_type, std::unique_ptr<Stmt> body)
      : func_name(func_name), ret_type(ret_type), body(std::move(body)) {}
  std::string                        func_name{};
  std::vector<std::unique_ptr<Stmt>> params{};
  std::string                        ret_type{};
  std::unique_ptr<Stmt>              body{};

  void visit(StmtVisitor& v) override {
    v.visit(*this);
  }
};

struct ExprStmt : Stmt {
  ExprStmt(std::unique_ptr<Expr> expr) : expr(std::move(expr)) {}
  std::unique_ptr<Expr> expr;

  void visit(StmtVisitor& v) override {
    v.visit(*this);
  }
};

struct BlockStmt : Stmt {
  BlockStmt(std::unique_ptr<Stmt> body) : body(std::move(body)) {}
  std::unique_ptr<Stmt> body;

  void visit(StmtVisitor& v) override {
    v.visit(*this);
  }
};

struct WhileStmt : Stmt {
  WhileStmt(std::unique_ptr<Expr> cond, std::unique_ptr<Stmt> body) : cond(std::move(cond)), body(std::move(body)) {}
  std::unique_ptr<Expr> cond;
  std::unique_ptr<Stmt> body;

  void visit(StmtVisitor& v) override {
    v.visit(*this);
  }
};

struct IfStmt : Stmt {
  IfStmt(std::unique_ptr<Expr>                                                cond,
         std::unique_ptr<Stmt>                                                body,
         std::vector<std::pair<std::unique_ptr<Expr>, std::unique_ptr<Stmt>>> elseif_branches,
         std::unique_ptr<Stmt>                                                else_body)
      : cond(std::move(cond)),
        body(std::move(body)),
        elseif_branches(std::move(elseif_branches)),
        else_body(std::move(else_body)) {}
  std::unique_ptr<Expr>                                                cond;
  std::unique_ptr<Stmt>                                                body;
  std::vector<std::pair<std::unique_ptr<Expr>, std::unique_ptr<Stmt>>> elseif_branches;
  std::unique_ptr<Stmt>                                                else_body;

  void visit(StmtVisitor& v) override {
    v.visit(*this);
    ;
  }
};

struct ReadStmt : Stmt {
  ReadStmt(std::string type, std::string id) : type(type), id(id) {}
  std::string type;
  std::string id;

  void visit(StmtVisitor& v) override {
    v.visit(*this);
  }
};

struct PrintStmt : Stmt {
  PrintStmt(std::unique_ptr<Expr> value) : value(std::move(value)) {}
  std::unique_ptr<Expr> value;

  void visit(StmtVisitor& v) override {
    v.visit(*this);
  }
};

struct ReturnStmt : Stmt {
  ReturnStmt(std::unique_ptr<Expr> value) : value(std::move(value)) {}
  std::unique_ptr<Expr> value;

  void visit(StmtVisitor& v) override {
    v.visit(*this);
  }
};

} // namespace monke
