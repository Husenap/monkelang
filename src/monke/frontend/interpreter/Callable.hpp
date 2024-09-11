#pragma once

#include <vector>

#include "monke/frontend/interpreter/Literal.hpp"

namespace monke {

class Interpreter;

struct Callable {
  virtual ~Callable() = default;

  virtual int     arity()                                                        = 0;
  virtual Literal call(Interpreter& interpreter, std::vector<Literal> arguments) = 0;

  virtual std::string toString() {
    return "<native fn>";
  }
};

} // namespace monke
