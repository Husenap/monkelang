#include "Parser.hpp"
#include "Scanner.hpp"
#include "monke/frontend/ast/AstPrettyPrint.hpp"
#include "monke/frontend/interpreter/Interpreter.hpp"

int main() {
  monke::Scanner      scanner{std::cin, std::cerr};
  monke::LexContext   ctx;
  monke::SourceModule sm;
  monke::Parser       parser{scanner, ctx, sm};

  auto result = parser.parse();
  if (result != 0) return result;

  monke::Interpreter interpreter;
  sm.visit(interpreter);

  return 0;
}
