#include "Expr.hpp"

#include "AstVisitor.hpp"

namespace monke {

void IntegerExpr::visit(AstVisitor& v) {
  v.visit(*this);
}

void BinOpExpr::visit(AstVisitor& v) {
  v.visit(*this);
}

} // namespace monke
