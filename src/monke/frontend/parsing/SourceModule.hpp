#pragma once

#include <memory>
#include <vector>

#include "monke/frontend/ast/AstVisitor.hpp"
#include "monke/frontend/ast/Stmt.hpp"

namespace monke {
class SourceModule {
public:
  void visit(AstVisitor& visitor) {
    statement->visit(visitor);
  }
  void fill(std::unique_ptr<Stmt> stmt) {
    statement = std::move(stmt);
  }

private:
  std::unique_ptr<Stmt> statement;
};
} // namespace monke
