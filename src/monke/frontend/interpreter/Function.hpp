#pragma once

#include "Callable.hpp"
#include "monke/frontend/ast/Stmt.hpp"

namespace monke {

class Function : public Callable {
  FuncDeclStmt& declaration;

  Literal impl_call(Interpreter& interpreter, std::vector<Literal> arguments);

public:
  Function(FuncDeclStmt& declaration) : declaration(declaration) {}

  int arity() override {
    return declaration.params.size();
  }
  Literal call(Interpreter& interpreter, std::vector<Literal> arguments) override {
    return impl_call(interpreter, arguments);
  }

  std::string toString() override {
    return "<fn " + declaration.func_name + ">";
  }
};

} // namespace monke
