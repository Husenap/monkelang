#include "Parser.hpp"
#include "Scanner.hpp"
#include "monke/frontend/ast/AstPrettyPrint.hpp"

int main() {
  monke::Scanner      scanner{std::cin, std::cerr};
  monke::LexContext   ctx;
  monke::SourceModule sm;
  monke::Parser       parser{scanner, ctx, sm};

  auto result = parser.parse();
  if (result != 0) return result;

  monke::AstPrettyPrint prettyPrinter;
  sm.visit(prettyPrinter);

  return 0;
}
