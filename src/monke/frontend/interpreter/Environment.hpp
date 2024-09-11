#pragma once

#include <stdexcept>
#include <string>
#include <unordered_map>

#include "Literal.hpp"

namespace monke {

class Environment {
  std::unordered_map<std::string, Literal> values{};
  Environment*                             enclosing{};

public:
  Environment(Environment* enclosing = nullptr) : enclosing(enclosing) {}

  void define(std::string name, Literal value) {
    values[name] = value;
  }
  Literal get(std::string name) {
    auto it = values.find(name);
    if (it == values.end()) {
      if (enclosing) {
        return enclosing->get(name);
      }
      throw std::runtime_error(std::string("Undefined variable '") + name + "'");
    }
    return it->second;
  }
  void assign(std::string name, Literal value) {
    auto it = values.find(name);
    if (it == values.end()) {
      if (enclosing) {
        enclosing->assign(name, value);
        return;
      }
      throw std::runtime_error(std::string("Undefined variable '") + name + "'");
    }
    it->second = value;
  }
};

} // namespace monke
