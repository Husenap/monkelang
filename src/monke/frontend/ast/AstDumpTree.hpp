#pragma once

#include <iostream>

#include "Expr.hpp"
#include "Stmt.hpp"

namespace monke {

struct AstDumpTree : public ExprVisitor, public StmtVisitor {
  std::ostream& out;
  AstDumpTree(std::ostream& out = std::cout) : out(out) {}

  void visit(IdDeclStmt& stmt) override {
    output("IdDeclStmt") << "id:" << stmt.id << " type:" << stmt.type << std::endl;
    indent();
    if (stmt.value) stmt.value->visit(*this);
    dedent();
  }
  void visit(StmtList& stmt) override {
    for (auto& param : stmt.stmts) {
      param->visit(*this);
    }
  }
  void visit(FuncDeclStmt& stmt) override {
    output("FuncDeclStmt") << "name:" << stmt.func_name << " return:" << stmt.ret_type << std::endl;
    indent();

    output("Params") << std::endl;
    indent();
    for (auto& param : stmt.params) param->visit(*this);
    dedent();

    output("Body") << std::endl;
    indent();
    stmt.body->visit(*this);
    dedent();

    dedent();
  }
  void visit(BlockStmt& stmt) override {
    output("BlockStmt") << std::endl;
    indent();
    stmt.body->visit(*this);
    dedent();
  }
  void visit(WhileStmt& stmt) override {
    output("WhileStmt") << std::endl;
    indent();

    output("Cond") << std::endl;
    indent();
    stmt.cond->visit(*this);
    dedent();

    output("Body") << std::endl;
    indent();
    stmt.body->visit(*this);
    dedent();

    dedent();
  }
  void visit(IfStmt& stmt) override {
    output("IfStmt") << std::endl;
    indent();

    output("Condition") << std::endl;
    indent();
    stmt.cond->visit(*this);
    dedent();

    output("Body") << std::endl;
    indent();
    stmt.body->visit(*this);
    dedent();

    if (!stmt.elseif_branches.empty()) {
      for (auto& [elseif_cond, elseif_body] : stmt.elseif_branches) {
        output("Else-If Branch") << std::endl;
        indent();
        output("Condition") << std::endl;
        elseif_cond->visit(*this);
        output("Body") << std::endl;
        elseif_body->visit(*this);
        dedent();
      }
    }

    if (stmt.else_body) {
      output("Else Branch") << std::endl;
      indent();
      stmt.else_body->visit(*this);
      dedent();
    }

    dedent();
  }
  void visit(ReadStmt& stmt) override {
    output("ReadStmt") << "type:" << stmt.type << " id:" << stmt.id << std::endl;
  }
  void visit(PrintStmt& stmt) override {
    output("PrintStmt") << std::endl;
    indent();
    stmt.value->visit(*this);
    dedent();
  }
  void visit(ReturnStmt& stmt) override {
    output("ReturnStmt") << std::endl;
    indent();
    stmt.value->visit(*this);
    dedent();
  }
  void visit(ExprStmt& stmt) override {
    output("ExprStmt") << std::endl;
    indent();
    stmt.expr->visit(*this);
    dedent();
  }

  void visit(FuncCallExpr& expr) override {
    output("FuncCallExpr") << expr.func_name << std::endl;
    indent();

    output("Args") << std::endl;
    indent();
    for (auto& arg : expr.args) arg->visit(*this);
    dedent();

    dedent();
  }
  void visit(AssignmentExpr& expr) override {
    output("AssignmentExpr") << expr.id << std::endl;
    indent();
    expr.value->visit(*this);
    dedent();
  }
  void visit(IdUseExpr& expr) override {
    output("IdUseExpr") << expr.id << std::endl;
  }
  void visit(NumberExpr& expr) override {
    output("NumberExpr") << expr.value << std::endl;
  }
  void visit(BinOpExpr& expr) override {
    output("BinOpExpr") << BinOpSymbol[expr.op] << std::endl;
    indent();
    expr.lhs->visit(*this);
    expr.rhs->visit(*this);
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

  std::string_view indentation = "  ";
  int              depth       = 0;
};

} // namespace monke
