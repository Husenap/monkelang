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
 
%token END 0
%token MONKE
%token HAS WANT SAY
%token WORK DO DONE WITH AND GIVE
%token WHILE THINK THINK_AGAIN CHANGE_MIND THINKED
%token TYPE IDENTIFIER NUMBER
%token IS EQ NEQ LT LEQ GT GEQ PLUS MINUS MUL DIV MOD

%type <std::unique_ptr<StmtList>> declaration_list;
%type <std::unique_ptr<Stmt>> declaration;
%type <std::unique_ptr<Stmt>> variable_declaration;
%type <std::vector<std::unique_ptr<Stmt>>> parameter_list;
%type <std::unique_ptr<Stmt>> statement;
%type <std::unique_ptr<Stmt>> block;
%type <std::unique_ptr<Stmt>> if_statement;
%type <std::vector<std::pair<std::unique_ptr<Expr>,std::unique_ptr<Stmt>>>> elseif_list;
%type <std::pair<std::unique_ptr<Expr>,std::unique_ptr<Stmt>>> elseif_branch;
%type <std::unique_ptr<Stmt>> else_branch;
%type <std::unique_ptr<Expr>> expression;
%type <std::unique_ptr<Expr>> assignment;
%type <std::unique_ptr<Expr>> equality;
%type <std::unique_ptr<Expr>> relational;
%type <std::unique_ptr<Expr>> term;
%type <std::unique_ptr<Expr>> factor;
%type <std::unique_ptr<Expr>> call;
%type <std::vector<std::unique_ptr<Expr>>> argument_list;
%type <std::unique_ptr<Expr>> primary;

%type <std::string> IDENTIFIER;
%type <std::string> TYPE;
%type <double> NUMBER;


// =================================== Grammar Definition ================================
 
%%

%start module;

module
  : declaration_list END { sm.fill(std::move($1)); }
  ;

declaration_list
  : %empty { $$ = std::make_unique<StmtList>(); }
  | declaration_list[list] declaration[decl] { $list->push_back(std::move($decl)); $$ = std::move($list); }
  ;
declaration
  : MONKE HAS variable_declaration[var] { $$ = std::move($var); }
  | MONKE WORK IDENTIFIER[func_name]
    WANT parameter_list[params]
    GIVE TYPE[ret_type]
    block[body]
    { $$ = std::make_unique<FuncDeclStmt>($func_name, std::move($params), $ret_type, std::move($body)); }
  | MONKE WORK IDENTIFIER[func_name]
    GIVE TYPE[ret_type]
    block[body]
    { $$ = std::make_unique<FuncDeclStmt>($func_name, $ret_type, std::move($body)); }
  | statement[stmt] { $$ = std::move($stmt); }
  ;

variable_declaration
  : TYPE[type] IDENTIFIER[id] { $$ = std::make_unique<IdDeclStmt>($type, $id, nullptr); }
  | TYPE[type] IDENTIFIER[id] IS expression[value] { $$ = std::make_unique<IdDeclStmt>($type, $id, std::move($value)); }
  ;

parameter_list
  : variable_declaration[decl] { $$.push_back(std::move($decl)); }
  | parameter_list[list] AND variable_declaration[decl] { $list.push_back(std::move($decl)); $$ = std::move($list); }
  ;

statement
  : expression { $$ = std::make_unique<ExprStmt>(std::move($1)); }
  | block { $$ = std::move($1); }
  | MONKE WANT TYPE[type] IDENTIFIER[id] { $$ = std::make_unique<ReadStmt>($type, $id); }
  | MONKE SAY expression[expr] { $$ = std::make_unique<PrintStmt>(std::move($expr)); }
  | GIVE expression[expr] { $$ = std::make_unique<ReturnStmt>(std::move($expr)); }
  | WHILE expression[cond] block[body] { $$ = std::make_unique<WhileStmt>(std::move($cond), std::move($body)); }
  | if_statement { $$ = std::move($1); }
  ;

if_statement
  : THINK expression[cond] MONKE DO
    declaration_list[body]
    elseif_list[elseif_branches]
    else_branch[else_body]
    THINKED
    { $$ = std::make_unique<IfStmt>(std::move($cond), std::move($body), std::move($elseif_branches), std::move($else_body)); }
  ;
elseif_list
  : %empty { }
  | elseif_list[list] elseif_branch[branch] { $list.push_back(std::move($branch)); $$ = std::move($list); }
  ;
elseif_branch
  : THINK_AGAIN expression[cond] MONKE DO declaration_list[body]
  { $$ = std::move(std::make_pair(std::move($cond), std::move($body))); }
  ;
else_branch
  : %empty { }
  | CHANGE_MIND declaration_list[body] { $$ = std::move($body); }
  ;

block : MONKE DO declaration_list[body] MONKE DONE { $$ = std::make_unique<BlockStmt>(std::move($body)); } ;

expression
  : assignment { $$ = std::move($1); }
  ;

assignment
  : IDENTIFIER[id] IS expression[expr] { $$ = std::make_unique<AssignmentExpr>($id, std::move($expr)); }
  | equality { $$ = std::move($1); }
  ;

equality
  : relational[lhs] EQ relational[rhs] { $$ = std::make_unique<BinOpExpr>(Equal, std::move($lhs), std::move($rhs)); }
  | relational[lhs] NEQ relational[rhs] { $$ = std::make_unique<BinOpExpr>(NotEqual, std::move($lhs), std::move($rhs)); }
  | relational { $$ = std::move($1); }
  ;

relational
  : term[lhs] LT term[rhs] { $$ = std::make_unique<BinOpExpr>(LessThan, std::move($lhs), std::move($rhs)); }
  | term[lhs] LEQ term[rhs] { $$ = std::make_unique<BinOpExpr>(LessOrEqual, std::move($lhs), std::move($rhs)); }
  | term[lhs] GT term[rhs] { $$ = std::make_unique<BinOpExpr>(GreaterThan, std::move($lhs), std::move($rhs)); }
  | term[lhs] GEQ term[rhs] { $$ = std::make_unique<BinOpExpr>(GreaterOrEqual, std::move($lhs), std::move($rhs)); }
  | term { $$ = std::move($1); }
  ;

term
  : term[lhs] PLUS factor[rhs] { $$ = std::make_unique<BinOpExpr>(Add, std::move($lhs), std::move($rhs)); }
  | term[lhs] MINUS factor[rhs] { $$ = std::make_unique<BinOpExpr>(Sub, std::move($lhs), std::move($rhs)); }
  | factor { $$ = std::move($1); }
  ;

factor
  : factor[lhs] MUL call[rhs] { $$ = std::make_unique<BinOpExpr>(Mul, std::move($lhs), std::move($rhs)); }
  | factor[lhs] DIV call[rhs] { $$ = std::make_unique<BinOpExpr>(Div, std::move($lhs), std::move($rhs)); }
  | factor[lhs] MOD call[rhs] { $$ = std::make_unique<BinOpExpr>(Mod, std::move($lhs), std::move($rhs)); }
  | call { $$ = std::move($1); }
  ;

call
  : MONKE DO IDENTIFIER[func_name] MONKE DONE { $$ = std::make_unique<FuncCallExpr>($func_name); }
  | MONKE DO IDENTIFIER[func_name] WITH argument_list[args] MONKE DONE { $$ = std::make_unique<FuncCallExpr>($func_name, std::move($args)); }
  | primary { $$ = std::move($1); }
  ;

argument_list
  : expression[expr] { $$.push_back(std::move($expr)); }
  | argument_list[list] AND expression[expr] { $list.push_back(std::move($expr)); $$ = std::move($list); }
  ;

primary
  : NUMBER { $$ = std::make_unique<NumberExpr>($1); }
  | IDENTIFIER { $$ = std::make_unique<IdUseExpr>($1); }
  ;

%%
 
