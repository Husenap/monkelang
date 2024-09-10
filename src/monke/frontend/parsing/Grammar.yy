%skeleton "lalr1.cc"
%require "3.8.2"
%header
%language "C++"

%defines "Parser.hpp"
%output "Parser.cpp"

%locations
%define api.location.file "SourceLoc.h"
 
%define api.namespace {monke}
%define api.parser.class {Parser}

%define api.value.type variant
%define api.token.constructor
%define api.token.raw

%define parse.trace
%define parse.assert
%define parse.error verbose
%define parse.lac full

// =================================== Require code ======================================

%code requires{
#include "LexContext.hpp"
#include "SourceModule.hpp"
#include "SyntaxError.hpp"
#include "monke/frontend/ast/Expr.hpp"
#include "monke/frontend/ast/Stmt.hpp"

namespace monke{
  class Scanner;
}
}
 
// This is a parameter to the parser constructor
%parse-param {Scanner &scanner}

// These are parameters to the yylex function
%param { LexContext &ctx }
%param { SourceModule &sm }


%code
{
#include "Scanner.hpp"
#define yylex(ctx, sm) scanner.lex(ctx, sm)
void monke::Parser::error(const location &loc, const std::string &error) {
  throw monke::SyntaxError(loc, error);
}
}


// =================================== Token Definitions =================================
 
%token PLUS MINUS SEMICOLON
%token NUMBER
%token END 0


%type <std::vector<std::unique_ptr<Stmt>>> stmt_list;
%type <std::unique_ptr<Stmt>> stmt;
%type <std::unique_ptr<Stmt>> stmt_expression
%type <std::unique_ptr<Expr>> expr
%type <std::unique_ptr<Expr>> literal;
%type <int> NUMBER;

%left PLUS MINUS

// =================================== Grammar Definition ================================
 
%%
%start module;

module
  : stmt_list END { sm.fill(std::move($1)); }
  ;

stmt_list
  : %empty { }
  | stmt_list stmt { $1.push_back(std::move($2)); $$ = std::move($1); }
  ;

stmt
  : stmt_expression { $$ = std::move($1); }
  ;

stmt_expression: expr SEMICOLON { $$ = std::make_unique<ExprStmt>(std::move($1)); };
 
expr
  : expr PLUS expr { $$ = std::make_unique<BinOpExpr>(BinOp::Add, std::move($1), std::move($3)); }
  | expr MINUS expr { $$ = std::make_unique<BinOpExpr>(BinOp::Sub, std::move($1), std::move($3)); }
  | literal        { $$ = std::move($1); }
  ;

literal
  : NUMBER { $$ = std::make_unique<IntegerExpr>($1); }
  ;
%%
 
