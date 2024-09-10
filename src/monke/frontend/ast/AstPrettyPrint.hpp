#pragma once

#include <iostream>

#include "Expr.hpp"
#include "Stmt.hpp"
#include "monke/frontend/ast/AstVisitor.hpp"

namespace monke {

struct AstPrettyPrint : public AstVisitor {
  std::ostream& out;
  AstPrettyPrint(std::ostream& out = std::cout) : out(out) {}
};

} // namespace monke
