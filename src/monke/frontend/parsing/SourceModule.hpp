#pragma once

#include <memory>
#include <vector>

#include "monke/frontend/ast/AstVisitor.hpp"
#include "monke/frontend/ast/Stmt.hpp"

namespace monke {
class SourceModule {
public:
  void visit(AstVisitor& visitor) {
    for (auto& stmt : statements) {
      stmt->visit(visitor);
    }
  }
  void fill(std::vector<std::unique_ptr<Stmt>> stmts) {
    statements = std::move(stmts);
  }

private:
  std::vector<std::unique_ptr<Stmt>> statements;
};
} // namespace monke
