%language "c++"

%skeleton "lalr1.cc"
%defines
%define api.value.type variant 
%param {yy::Driver* driver}

%code requires 
{
#include <iostream>
#include <string>
#include "ast.hpp"

namespace yy { class Driver; }
}

%code
{
#include "driver.hpp"

namespace yy {

parser::token_type yylex(parser::semantic_type* yylval, Driver* driver);

}

}

%token 
    RIGHT_ROUND_BRACKER ")"
    LEFT_ROUND_BRACKER  "("
    RIGHT_CURLY_BRACKER "}"
    LEFT_CURLY_BRACKER  "{"
    PLUS                "+"
    MINUS               "-"
    DIV                 "/"
    MUL                 "*"
    ASSIGNMENT          "="
    INPUT               "?"
    NOT_EQUAL           "!="
    EQUAL               "=="
    BELOW               "<"
    EQUAL_OR_BELOW      "<="
    GREATER             ">"
    EQUAL_OR_GREATER    ">="
    SEMICOLON           ";"
    IF                  "if"
    ELSE                "else"
    WHILE               "while"
    PRINT               "print"
    ERROR
;

%left PLUS MINUS
%left MUL DIV
%nonassoc EQUAL NOT_EQUAL BELOW GREATER EQUAL_OR_BELOW EQUAL_OR_GREATER

%right ASSIGNMENT

%token <int>                   NUMBER
%token <std::string>           VAR 

%nterm <ast::TreeNode*>        program
%nterm <ast::TreeNode*>        while
%nterm <ast::TreeNode*>        if
%nterm <ast::TreeNode*>        else
%nterm <ast::TreeNode*>        expr 
%nterm <ast::TreeNode*>        comp_expr
%nterm <ast::TreeNode*>        decl 
%nterm <ast::TreeNode*>        assignment
%nterm <ast::TreeNode*>        value
%nterm <ast::TreeNode*>        print
%nterm <ast::TreeNode*>        input
%nterm <ast::TreeNode*>        stmt
%nterm <ast::TreeNode*>        stmts
%nterm <ast::TreeNode*>        scope
%nterm <ast::TreeNode*>        begin_of_scope
%nterm <ast::TreeNode*>        end_of_scope

%% 

program: stmts {};
;

stmts: stmt stmts 
        { driver->add_node($1); }
     | stmt
        { driver->add_node($1); }
;

scope: begin_of_scope stmts end_of_scope 
        { $$ = $3; }

begin_of_scope: LEFT_CURLY_BRACKER  
        { driver->current_scope_ = driver->tree.insert_scope_node(driver->current_scope_); }
;

end_of_scope: RIGHT_CURLY_BRACKER
        { $$ = driver->current_scope_; 
          driver->current_scope_ = driver->current_scope_->close_scope();}
;

stmt: print SEMICOLON
        { $$ = $1; }
    | if 
        { $$ = $1; }
    | while 
        { $$ = $1; }
    | assignment SEMICOLON
        { $$ = $1; }
    | comp_expr SEMICOLON
        { $$ = $1; }
    | input SEMICOLON 
        { $$ = $1; }
    | scope 
        { $$ = $1; } 
;


assignment: decl ASSIGNMENT comp_expr 
        { $$ = driver->tree.insert_assignment_node($1, $3); }
;

print: PRINT LEFT_ROUND_BRACKER comp_expr RIGHT_ROUND_BRACKER 
        { $$ = driver->tree.insert_print_node($3); }
;

if: IF LEFT_ROUND_BRACKER comp_expr RIGHT_ROUND_BRACKER stmt else
       { $$ = driver->tree.insert_if_node($3, $5, $6); }
;

else: ELSE stmt
        { $$ = driver->tree.insert_else_node($2); }
    | %empty 
        { $$ = nullptr; }
;

input: decl ASSIGNMENT INPUT 
        { $$ = driver->tree.insert_question_mark_node($1); };

while: WHILE LEFT_ROUND_BRACKER comp_expr RIGHT_ROUND_BRACKER stmt 
        { $$ = driver->tree.insert_while_node($3, $5); }
;

comp_expr: expr 
        { $$= driver->tree.insert_expr_node($1); }
; 

expr: expr PLUS expr 
        { $$ = driver->tree.insert_bin_op_node(ast::BinaryOpType::kAdd, $1, $3);}
    | expr MINUS expr 
        { $$ = driver->tree.insert_bin_op_node(ast::BinaryOpType::kSub, $1, $3);}
    | expr MUL expr 
        { $$ = driver->tree.insert_bin_op_node(ast::BinaryOpType::kMul, $1, $3); }
    | expr DIV expr 
        { $$ = driver->tree.insert_bin_op_node(ast::BinaryOpType::kDiv, $1, $3); }
    | LEFT_ROUND_BRACKER expr RIGHT_ROUND_BRACKER 
        { $$ = $2; }
    | expr EQUAL expr 
        { $$ = driver->tree.insert_log_op_node(ast::LogicalOpType::kEqual, $1, $3); }
    | expr NOT_EQUAL expr 
        { $$ = driver->tree.insert_log_op_node(ast::LogicalOpType::kNotEqual, $1, $3); }
    | expr BELOW expr 
        { $$ = driver->tree.insert_log_op_node(ast::LogicalOpType::kBelow, $1, $3); }
    | expr GREATER expr 
        { $$ = driver->tree.insert_log_op_node(ast::LogicalOpType::kGreater, $1, $3); }
    | expr EQUAL_OR_BELOW expr 
        { $$ = driver->tree.insert_log_op_node(ast::LogicalOpType::kEqualOrBelow, $1, $3); }
    | expr EQUAL_OR_GREATER expr 
        { $$ = driver->tree.insert_log_op_node(ast::LogicalOpType::kEqualOrGreater, $1, $3); }
    | decl 
        { $$ = $1; }
    | value 
        { $$ = $1; }
;

value: NUMBER 
        { $$ = driver->tree.insert_value_node($1); }
;

decl: VAR 
        { $$ = driver->tree.insert_decl_node($1); }
;

%%

namespace yy {

parser::token_type yylex(parser::semantic_type* yylval,                         
                         Driver* driver)
{
  return driver->yylex(yylval);
}

void parser::error(const std::string&) { std::cerr << "Error" << std::endl; }
}
