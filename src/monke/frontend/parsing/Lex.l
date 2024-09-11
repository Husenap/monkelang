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
 
%option noyywrap nounput noinput batch
%option c++ interactive outfile="Lexer.cpp"

%x COMMENT

number                    (([1-9][0-9]*)|0)\.?[0-9]*
identifier                ([_a-zA-Z][_a-zA-Z0-9]*)
string                    \"(\\.|[^\\"])*\"	
 
%%
  ctx.loc.step();

"monke"                             { tk(MONKE); }
"has"                               { tk(HAS); }
"want"                              { tk(WANT); }
"say"                               { tk(SAY); }
"work"                              { tk(WORK); }
"do"                                { tk(DO); }
"done"                              { tk(DONE); }
"with"                              { tk(WITH); }
"and"                               { tk(AND); }
"give"                              { tk(GIVE); }
"while"                             { tk(WHILE); }
"monke think"                       { tk(THINK); }
"monke think again"                 { tk(THINK_AGAIN); }
"monke change mind"                 { tk(CHANGE_MIND); }
"monke thinked"                     { tk(THINKED); }
"="|"is"                            { tk(IS); }

"=="|"is equal to"                  { tk(EQ); }
"!="|"is not equal to"              { tk(NEQ); }
"<"|"is less than"                  { tk(LT); }
"<="|"is less than or equal to"     { tk(LEQ); }
">"|"is greater than"               { tk(GT); }
">="|"is greater than or equal to"  { tk(GEQ); }

"+"|"plus"                          { tk(PLUS); }
"-"|"minus"                         { tk(MINUS); }
"*"|"times"                         { tk(MUL); }
"/"|"divided by"                    { tk(DIV); }
"%"|"modulo"                        { tk(MOD); }

"number"                            { tkv(TYPE, yytext); }
"string"                            { tkv(TYPE, yytext); }
"boolean"                           { tkv(TYPE, yytext); }

"true"                              { tkv(BOOLEAN, true); }
"false"                             { tkv(BOOLEAN, false); }

{number}                            { tkv(NUMBER, strtod(yytext, nullptr)); }
{identifier}                        { tkv(IDENTIFIER, yytext); }
{string}                            { tkv(STRING, yytext); }
 
"//".*                              { ctx.loc.columns(); ctx.loc.lines(); }
"monke note"                        { BEGIN(COMMENT); }
<COMMENT>([^"monke noted"])*"monke noted" { BEGIN(INITIAL); }
<COMMENT>.|\n                       { }

[ \t\r\v]                           { ctx.loc.columns(); }
[\n]                                { ctx.loc.lines(); }
<<EOF>>                             { tk(END); }

.                                   { ctx.loc.columns();
                                      ctx.loc.lines(); 
                                      std::string str("Unrecognized char '");
                                      str += yytext[0];
                                      str += "'";
                                      throw monke::SyntaxError(ctx.loc, str); }
 
%%

int yyFlexLexer::yylex() {
    throw std::runtime_error("Bad call to yyFlexLexer::yylex()");
}
