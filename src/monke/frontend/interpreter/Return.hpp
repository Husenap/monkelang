#pragma once

#include <exception>

#include "Literal.hpp"

namespace monke {

struct Return : public std::exception {
  Return(Literal value) : value(value) {}
  Literal value;
};

} // namespace monke
