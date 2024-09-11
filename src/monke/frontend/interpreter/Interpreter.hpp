#pragma once

#include <cmath>
#include <iostream>
#include <memory>
#include <sstream>
#include <stack>
#include <stdexcept>
#include <string>
#include <variant>

#include "Literal.hpp"
#include "monke/frontend/ast/Expr.hpp"
#include "monke/frontend/ast/Stmt.hpp"
#include "monke/frontend/interpreter/Environment.hpp"

namespace monke {

class Interpreter : public StmtVisitor, public ExprVisitor {
  std::ostream&                            out;
  std::istream&                            in;
  std::stack<Literal>                      stack;
  std::stack<std::unique_ptr<Environment>> environment;

  Literal popStack() {
    if (stack.empty()) {
      throw std::runtime_error("Trying to pop element off of empty stack!");
    }
    auto result = stack.top();
    stack.pop();
    return result;
  }
  Literal peekStack() {
    if (stack.empty()) {
      throw std::runtime_error("Trying to peek element off of empty stack!");
    }
    auto result = stack.top();
    return result;
  }

  void beginScope() {
    environment.push(std::make_unique<Environment>(environment.top().get()));
  }
  void endScope() {
    environment.pop();
  }

  bool isTruthy(Literal value) {
    if (std::holds_alternative<std::monostate>(value)) return false;
    if (std::holds_alternative<bool>(value)) return std::get<bool>(value);
    if (std::holds_alternative<double>(value)) return std::get<double>(value) != 0.0;
    if (std::holds_alternative<std::string>(value)) return std::get<std::string>(value) != "";
    return true;
  }

  std::string to_string(Literal value) {
    return std::visit(overloaded{
                          [](auto&&) -> std::string { return "nil"; },
                          [](double arg) {
                            std::stringstream ss;
                            ss << arg;
                            return ss.str();
                          },
                          [](std::string arg) -> std::string { return arg; },
                          [](bool arg) -> std::string { return arg ? "true" : "false"; },
                      },
                      value);
  }

public:
  Interpreter(std::ostream& out = std::cout, std::istream& in = std::cin) : out(out), in(in) {
    environment.push(std::make_unique<Environment>());
  }

  virtual void visit(IdDeclStmt& stmt) {
    Literal value = std::monostate{};
    if (stmt.value) {
      stmt.value->visit(*this);
      value = popStack();
    }
    environment.top()->define(stmt.id, value);
  }
  virtual void visit(StmtList& stmt) {
    for (auto& s : stmt.stmts) {
      s->visit(*this);
    }
  }
  virtual void visit(FuncDeclStmt&) {
    throw std::runtime_error("FuncDeclStmt not implemented yet!");
  }
  virtual void visit(BlockStmt& stmt) {
    beginScope();
    stmt.body->visit(*this);
    endScope();
  }
  virtual void visit(WhileStmt& stmt) {
    while (isTruthy((stmt.cond->visit(*this), popStack()))) {
      beginScope();
      stmt.body->visit(*this);
      endScope();
    }
  }
  virtual void visit(IfStmt& stmt) {
    stmt.cond->visit(*this);
    if (isTruthy(popStack())) {
      beginScope();
      stmt.body->visit(*this);
      endScope();
      return;
    }
    for (auto& [cond, body] : stmt.elseif_branches) {
      cond->visit(*this);
      if (isTruthy(popStack())) {
        beginScope();
        body->visit(*this);
        endScope();
        return;
      }
    }
    if (stmt.else_body) {
      beginScope();
      stmt.else_body->visit(*this);
      endScope();
    }
  }
  virtual void visit(ReadStmt&) {
    throw std::runtime_error("ReadStmt not implemented yet!");
  }
  virtual void visit(PrintStmt& stmt) {
    stmt.value->visit(*this);
    auto value = popStack();
    out << to_string(value) << std::endl;
  }
  virtual void visit(ReturnStmt&) {
    throw std::runtime_error("ReturnStmt not implemented yet!");
  }
  virtual void visit(ExprStmt& stmt) {
    stmt.expr->visit(*this);
    popStack();
  }

  virtual void visit(FuncCallExpr&) {
    throw std::runtime_error("FuncCallExpr not implemented yet!");
  }
  virtual void visit(AssignmentExpr& expr) {
    expr.value->visit(*this);
    auto value = peekStack();
    environment.top()->assign(expr.id, value);
  }
  virtual void visit(IdUseExpr& expr) {
    stack.push(environment.top()->get(expr.id));
  }
  virtual void visit(BinOpExpr& expr) {
    expr.rhs->visit(*this);
    auto rhs = popStack();
    expr.lhs->visit(*this);
    auto lhs = popStack();

    if (std::holds_alternative<std::string>(lhs) || std::holds_alternative<std::string>(rhs)) {
      stack.push(to_string(lhs) + to_string(rhs));
      return;
    }
    if (lhs.index() != rhs.index()) {
      throw std::runtime_error("Binary operation between two different types not allowed!");
    }

    switch (expr.op) {
    case Add:
      stack.push(std::get<double>(lhs) + std::get<double>(rhs));
      break;
    case Sub:
      stack.push(std::get<double>(lhs) - std::get<double>(rhs));
      break;
    case Mul:
      stack.push(std::get<double>(lhs) * std::get<double>(rhs));
      break;
    case Div:
      stack.push(std::get<double>(lhs) / std::get<double>(rhs));
      break;
    case Mod:
      stack.push(std::fmod(std::get<double>(lhs), std::get<double>(rhs)));
      break;
    case Equal:
      if (std::holds_alternative<double>(lhs)) {
        stack.push(std::get<double>(lhs) == std::get<double>(rhs));
      } else {
        stack.push(std::get<bool>(lhs) == std::get<bool>(rhs));
      }
      break;
    case NotEqual:
      if (std::holds_alternative<double>(lhs)) {
        stack.push(std::get<double>(lhs) != std::get<double>(rhs));
      } else {
        stack.push(std::get<bool>(lhs) != std::get<bool>(rhs));
      }
      break;
    case LessThan:
      if (std::holds_alternative<double>(lhs)) {
        stack.push(std::get<double>(lhs) < std::get<double>(rhs));
      } else {
        throw std::runtime_error("Illegal binary op");
      }
      break;
    case LessOrEqual:
      if (std::holds_alternative<double>(lhs)) {
        stack.push(std::get<double>(lhs) <= std::get<double>(rhs));
      } else {
        throw std::runtime_error("Illegal binary op");
      }
      break;
    case GreaterThan:
      if (std::holds_alternative<double>(lhs)) {
        stack.push(std::get<double>(lhs) > std::get<double>(rhs));
      } else {
        throw std::runtime_error("Illegal binary op");
      }
      break;
    case GreaterOrEqual:
      if (std::holds_alternative<double>(lhs)) {
        stack.push(std::get<double>(lhs) >= std::get<double>(rhs));
      } else {
        throw std::runtime_error("Illegal binary op");
      }
      break;
    default:
      throw std::runtime_error("Unhandled binary op");
      break;
    }
  }
  virtual void visit(NumberExpr& expr) {
    stack.push(expr.value);
  }
  virtual void visit(BooleanExpr& expr) {
    stack.push(expr.value);
  }
  virtual void visit(StringExpr& expr) {
    std::string_view raw_string     = expr.value;
    std::string      decoded_string = "";
    for (int i = 1; i < raw_string.size() - 1; ++i) {
      if (raw_string[i] == '\\') {
        switch (raw_string[i + 1]) {
        case '"':
          decoded_string += '"';
          break;
        case 'r':
          decoded_string += '\r';
          break;
        case 't':
          decoded_string += '\t';
          break;
        case 'n':
          decoded_string += '\n';
          break;
        default:
          throw std::runtime_error(std::string("Unknown escape sequence \\") + raw_string[i + 1]);
          break;
        }
        ++i;
      } else {
        decoded_string += raw_string[i];
      }
    }

    stack.push(decoded_string);
  }
};

} // namespace monke
