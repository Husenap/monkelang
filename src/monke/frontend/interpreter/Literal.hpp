#pragma once

#include <memory>
#include <string>
#include <variant>

namespace monke {

struct Callable;

template <class... Ts>
struct overloaded : Ts... {
  using Ts::operator()...;
};

using Literal = std::variant<std::monostate, double, bool, std::string, std::shared_ptr<Callable>>;

} // namespace monke
