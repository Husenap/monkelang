#include "Stmt.hpp"

#include "AstVisitor.hpp"

namespace monke {

void IdDeclStmt::visit(AstVisitor& v) {
  v.visit(*this);
};

void StmtList::visit(AstVisitor& v) {
  v.visit(*this);
};

void FuncDeclStmt::visit(AstVisitor& v) {
  v.visit(*this);
};

void BlockStmt::visit(AstVisitor& v) {
  v.visit(*this);
};

void WhileStmt::visit(AstVisitor& v) {
  v.visit(*this);
};
void IfStmt::visit(AstVisitor& v) {
  v.visit(*this);
};

void ReadStmt::visit(AstVisitor& v) {
  v.visit(*this);
};
void PrintStmt::visit(AstVisitor& v) {
  v.visit(*this);
};
void ReturnStmt::visit(AstVisitor& v) {
  v.visit(*this);
};
void ExprStmt::visit(AstVisitor& v) {
  v.visit(*this);
};

} // namespace monke
