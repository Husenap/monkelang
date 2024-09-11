#pragma once

#include <string>
#include <variant>

namespace monke {
template <class... Ts>
struct overloaded : Ts... {
  using Ts::operator()...;
};
using Literal = std::variant<std::monostate, double, bool, std::string>;

} // namespace monke
