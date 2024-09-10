#include "Stmt.hpp"

#include "AstVisitor.hpp"

namespace monke {

void ExprStmt::visit(AstVisitor& v) {
  v.visit(*this);
};

} // namespace monke
