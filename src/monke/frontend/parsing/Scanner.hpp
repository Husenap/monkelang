#ifndef FLEX_SCANNER
#include "FlexLexer.h"
#endif

namespace monke {

class Scanner : public yyFlexLexer {
public:
  Scanner(std::istream& arg_yyin, std::ostream& arg_yyout) : yyFlexLexer(arg_yyin, arg_yyout) {}
  Scanner(std::istream* arg_yyin = nullptr, std::ostream* arg_yyout = nullptr) : yyFlexLexer(arg_yyin, arg_yyout) {}
  monke::Parser::symbol_type lex(monke::LexContext& ctx, monke::SourceModule& sm);
};

} // namespace monke
