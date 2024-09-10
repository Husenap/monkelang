%{
#include "Parser.hpp"
#include "Scanner.hpp"

#define tk(tk)                                                            \
  ctx.loc.columns(yyleng);                                                \
  return monke::Parser::make_##tk(ctx.loc)
#define tkv(tk, v)                                                        \
  ctx.loc.columns(yyleng);                                                \
  return monke::Parser::make_##tk(v, ctx.loc)

#define YY_DECL                                                           \
  monke::Parser::symbol_type monke::Scanner::lex(monke::LexContext &ctx,  \
                                                 monke::SourceModule &sm)

%}
 
%option noyylineno noyywrap nounput noinput batch
%option c++ interactive outfile="Lexer.cpp"

dseq            ([[:digit:]]+)
integer         ({dseq})
 
%%
  ctx.loc.step();
 
{integer}                 { tkv(NUMBER, strtoll(yytext, nullptr, 10)); }
"+"                       { tk(PLUS); }
"-"                       { tk(MINUS); }
";"                       { tk(SEMICOLON); }
"//".*                    { ctx.loc.columns(); ctx.loc.lines(); }

"monkey say"(.*[^"monkey do"]*)*"monkey do" { }

[ \t]                     { ctx.loc.columns(); }
[\n]                      { ctx.loc.lines(); }
<<EOF>>                   { tk(END); }
.                         { ctx.loc.columns();
                            ctx.loc.lines(); 
                            std::string str("Unrecognized char '");
                            str += yytext[0];
                            str += "'";
                            throw monke::SyntaxError(ctx.loc, str); }
 
%%

int yyFlexLexer::yylex() {
    throw std::runtime_error("Bad call to yyFlexLexer::yylex()");
}
