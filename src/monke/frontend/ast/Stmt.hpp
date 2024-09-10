#pragma once

#include <string>
#include <vector>

#include "Expr.hpp"
#include "Node.hpp"

namespace monke {

struct Stmt : Node {};

struct IdDeclStmt : Stmt {
  IdDeclStmt(std::string type, std::string id, std::unique_ptr<Expr> value)
      : type(type), id(id), value(std::move(value)) {}
  std::string           type{};
  std::string           id{};
  std::unique_ptr<Expr> value;

  void visit(AstVisitor& v) override;
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

  void visit(AstVisitor& v) override;
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

  void visit(AstVisitor& v) override;
};

struct ExprStmt : Stmt {
  ExprStmt(std::unique_ptr<Expr> expr) : expr(std::move(expr)) {}
  std::unique_ptr<Expr> expr;

  void visit(AstVisitor& v) override;
};

struct BlockStmt : Stmt {
  BlockStmt(std::unique_ptr<Stmt> body) : body(std::move(body)) {}
  std::unique_ptr<Stmt> body;

  void visit(AstVisitor& v) override;
};

struct WhileStmt : Stmt {
  WhileStmt(std::unique_ptr<Expr> cond, std::unique_ptr<Stmt> body) : cond(std::move(cond)), body(std::move(body)) {}
  std::unique_ptr<Expr> cond;
  std::unique_ptr<Stmt> body;

  void visit(AstVisitor& v) override;
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

  void visit(AstVisitor& v) override;
};

struct ReadStmt : Stmt {
  ReadStmt(std::string type, std::string id) : type(type), id(id) {}
  std::string type;
  std::string id;

  void visit(AstVisitor& v) override;
};

struct PrintStmt : Stmt {
  PrintStmt(std::unique_ptr<Expr> value) : value(std::move(value)) {}
  std::unique_ptr<Expr> value;

  void visit(AstVisitor& v) override;
};

struct ReturnStmt : Stmt {
  ReturnStmt(std::unique_ptr<Expr> value) : value(std::move(value)) {}
  std::unique_ptr<Expr> value;

  void visit(AstVisitor& v) override;
};

} // namespace monke
