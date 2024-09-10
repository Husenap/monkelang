#pragma once

namespace monke {

struct AstVisitor;

struct Node {
  virtual ~Node() = default;

  virtual void visit(AstVisitor& visitor) = 0;
};

} // namespace monke
