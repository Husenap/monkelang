#include "Function.hpp"

#include "Interpreter.hpp"
#include "monke/frontend/interpreter/Return.hpp"

namespace monke {

Literal Function::impl_call(Interpreter& interpreter, std::vector<Literal> arguments) {
  auto environment = std::make_shared<Environment>(&interpreter.globals);

  for (int i = 0; i < declaration.params.size(); ++i) {
    environment->define(declaration.params[i]->id, arguments[i]);
  }

  try {
    interpreter.executeBlock(*declaration.body, std::move(environment));
  } catch (Return returnValue) {
    return returnValue.value;
  }

  return std::monostate{};
}

} // namespace monke
