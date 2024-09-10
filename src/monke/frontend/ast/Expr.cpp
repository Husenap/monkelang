#include "Expr.hpp"

#include "AstVisitor.hpp"

namespace monke {

void NumberExpr::visit(AstVisitor& v) {
  v.visit(*this);
}
void IdUseExpr::visit(AstVisitor& v) {
  v.visit(*this);
}
void FuncCallExpr::visit(AstVisitor& v) {
  v.visit(*this);
}
void AssignmentExpr::visit(AstVisitor& v) {
  v.visit(*this);
}
void BinOpExpr::visit(AstVisitor& v) {
  v.visit(*this);
}

} // namespace monke
