#pragma once

#include <cmath>
#include <exception>
#include <iostream>
#include <memory>
#include <sstream>
#include <stack>
#include <stdexcept>
#include <string>
#include <variant>

#include "Callable.hpp"
#include "Literal.hpp"
#include "monke/frontend/ast/Expr.hpp"
#include "monke/frontend/ast/Stmt.hpp"
#include "monke/frontend/interpreter/Environment.hpp"
#include "monke/frontend/interpreter/Function.hpp"
#include "monke/frontend/interpreter/Return.hpp"
#include "monke/frontend/interpreter/natives/Clock.hpp"

namespace monke {

class Interpreter : public StmtVisitor, public ExprVisitor {
  std::ostream&                            out;
  std::istream&                            in;
  std::stack<Literal>                      stack;
  std::stack<std::shared_ptr<Environment>> environment;

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

  bool isTruthy(Literal value) {
    if (std::holds_alternative<std::monostate>(value)) return false;
    if (std::holds_alternative<bool>(value)) return std::get<bool>(value);
    if (std::holds_alternative<double>(value)) return std::get<double>(value) != 0.0;
    if (std::holds_alternative<std::string>(value)) return std::get<std::string>(value) != "";
    return true;
  }

  std::string toString(Literal value) {
    return std::visit(overloaded{
                          [](auto&&) -> std::string { return "nil"; },
                          [](std::shared_ptr<Callable> arg) { return arg->toString(); },
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

  std::shared_ptr<Environment> makeEnv() {
    return std::make_shared<Environment>(environment.top().get());
  }

  struct EnvironmentPopper {
    std::stack<std::shared_ptr<Environment>>& environment;
    EnvironmentPopper(std::stack<std::shared_ptr<Environment>>& environment) : environment(environment) {}
    ~EnvironmentPopper() {
      environment.pop();
    }
  };

public:
  Environment& globals;

  Interpreter(std::ostream& out = std::cout, std::istream& in = std::cin)
      : out(out), in(in), environment({std::make_shared<Environment>()}), globals(*environment.top()) {
    globals.define("clock", std::make_shared<Clock>());
  }

  void executeBlock(Stmt& stmt, std::shared_ptr<Environment> env) {
    environment.push(env);
    EnvironmentPopper popper(environment);
    stmt.visit(*this);
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
  virtual void visit(FuncDeclStmt& stmt) {
    auto function = std::make_shared<Function>(stmt);
    environment.top()->define(stmt.func_name, function);
  }
  virtual void visit(BlockStmt& stmt) {
    executeBlock(*stmt.body, makeEnv());
  }
  virtual void visit(WhileStmt& stmt) {
    int iters = 0;
    while (isTruthy((stmt.cond->visit(*this), popStack()))) {
      executeBlock(*stmt.body, makeEnv());
      if (++iters > 1000000) break;
    }
  }
  virtual void visit(IfStmt& stmt) {
    stmt.cond->visit(*this);
    if (isTruthy(popStack())) {
      executeBlock(*stmt.body, makeEnv());
      return;
    }
    for (auto& [cond, body] : stmt.elseif_branches) {
      cond->visit(*this);
      if (isTruthy(popStack())) {
        executeBlock(*body, makeEnv());
        return;
      }
    }
    if (stmt.else_body) {
      executeBlock(*stmt.else_body, makeEnv());
    }
  }
  virtual void visit(ReadStmt& stmt) {
    if (stmt.type == "number") {
      double v;
      in >> v;
      environment.top()->define(stmt.id, v);
    } else if (stmt.type == "string") {
      std::string v;
      in >> v;
      environment.top()->define(stmt.id, v);
    } else if (stmt.type == "boolean") {
      std::string v;
      in >> v;
      if (v == "true") {
        environment.top()->define(stmt.id, true);
      } else if (v == "false") {
        environment.top()->define(stmt.id, false);
      } else {
        throw std::runtime_error("Expected to read 'true' or 'false'!");
      }
    } else {
      throw std::runtime_error("Read statements expect the type to be 'number', 'string' or 'boolean'!");
    }
  }
  virtual void visit(PrintStmt& stmt) {
    stmt.value->visit(*this);
    auto value = popStack();
    out << toString(value) << std::endl;
  }
  virtual void visit(ReturnStmt& stmt) {
    Literal value = std::monostate{};
    if (stmt.value) {
      stmt.value->visit(*this);
      value = popStack();
    }
    throw Return(value);
  }
  virtual void visit(ExprStmt& stmt) {
    stmt.expr->visit(*this);
    popStack();
  }

  virtual void visit(FuncCallExpr& expr) {
    expr.callee->visit(*this);
    auto callee = popStack();

    std::vector<Literal> arguments;
    for (auto& arg : expr.args) {
      arg->visit(*this);
      arguments.push_back(popStack());
    }

    if (!std::holds_alternative<std::shared_ptr<Callable>>(callee)) {
      throw std::runtime_error("Can only call functions.");
    }
    auto function = std::get<std::shared_ptr<Callable>>(callee);

    if (arguments.size() != function->arity()) {
      std::stringstream ss;
      ss << "Expected " << function->arity() << " arguments but got " << arguments.size() << ".";
      throw std::runtime_error(ss.str());
    }

    stack.push(function->call(*this, std::move(arguments)));
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
      stack.push(toString(lhs) + toString(rhs));
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
