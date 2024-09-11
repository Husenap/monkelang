#pragma once

#include <chrono>

#include "../Callable.hpp"

namespace monke {

struct Clock : Callable {
  int arity() override {
    return 0;
  }
  Literal call(Interpreter& interpreter, std::vector<Literal> arguments) override {
    std::chrono::duration<double> fs = std::chrono::high_resolution_clock::now().time_since_epoch();
    return fs.count();
  }
};

} // namespace monke
