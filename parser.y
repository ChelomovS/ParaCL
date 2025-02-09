%language "c++"

%skeleton "lalr1.cc"
%defines

%define api.value.type variant 
%define parse.error verbose

%param {yy::Driver* driver}

%locations

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

parser::token_type yylex(parser::semantic_type* yylval, 
                         parser::location_type* loc_type, 
                         Driver* driver);

} // namespace yy

}

%{
#include "spdlog/spdlog.h"
#if 0
#define parse_trace(...) spdlog::trace(__VA_ARGS__)
#else
#define parse_trace(...) ;
#endif
%}

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
%nterm <ast::TreeNode*>        var_deref
%nterm <ast::TreeNode*>        assignment
%nterm <ast::TreeNode*>        value
%nterm <ast::TreeNode*>        print
%nterm <ast::TreeNode*>        stmt
%nterm <ast::TreeNode*>        stmts
%nterm <ast::TreeNode*>        scope
%nterm <ast::TreeNode*>        begin_of_scope
%nterm <ast::TreeNode*>        end_of_scope

%% 

program: stmts 
        { 
            parse_trace("Reducing: program -> stmts"); 
        }
;

stmts: stmt stmts 
        { 
            parse_trace("Reducing: stmts -> stmt stmts"); 
            driver->add_node($1); 
        }
     | stmt
        { 
            parse_trace("Reducing: stmts -> stmt"); 
            driver->add_node($1); 
        }
;

scope: begin_of_scope stmts end_of_scope 
        { 
            parse_trace("Reducing: scope -> begin_of_scope stmts end_of_scope"); 
            $$ = $3; 
        }
;

begin_of_scope: LEFT_CURLY_BRACKER  
        {   
            parse_trace("Reducing: begin_of_scope -> LEFT_CURLY_BRACKER"); 
            driver->current_scope_ = driver->tree.insert_scope_node(driver->current_scope_); 
        }
;

end_of_scope: RIGHT_CURLY_BRACKER
        { 
            parse_trace("Reducing: end_of_scope -> RIGHT_CURLY_BRACKER"); 
            $$ = driver->current_scope_; 
            driver->current_scope_ = driver->current_scope_->close_scope();
        }
;

stmt: print SEMICOLON
        { 
            parse_trace("Reducing: stmt -> print SEMICOLON"); 
            $$ = $1; 
        }
    | if 
        { 
            parse_trace("Reducing: stmt -> if"); 
            $$ = $1; 
        }
    | while 
        {  
            parse_trace("Reducing: stmt -> while"); 
            $$ = $1; 
        }
    | assignment SEMICOLON
        { 
            parse_trace("Reducing: stmt -> assignment SEMICOLON"); 
            $$ = $1; 
        }
    | comp_expr SEMICOLON
        { 
            parse_trace("Reducing: stmt -> comp_expr SEMICOLON"); 
            $$ = $1; 
        }
    | scope 
        { 
            parse_trace("Reducing: stmt -> scope"); 
            $$ = $1; 
        }
;


assignment: decl ASSIGNMENT comp_expr 
        { 
            parse_trace("Reducing: assignment -> decl ASSIGNMENT comp_expr"); 
            $$ = driver->tree.insert_assignment_node($1, $3); 
        }
;

print: PRINT LEFT_ROUND_BRACKER comp_expr RIGHT_ROUND_BRACKER 
        { 
            parse_trace("Reducing: print -> PRINT LEFT_ROUND_BRACKER comp_expr RIGHT_ROUND_BRACKER"); 
            $$ = driver->tree.insert_print_node($3); 
        }
;

if: IF LEFT_ROUND_BRACKER comp_expr RIGHT_ROUND_BRACKER stmt else
        { 
            parse_trace("Reducing: if -> IF LEFT_ROUND_BRACKER comp_expr RIGHT_ROUND_BRACKER stmt else"); 
            $$ = driver->tree.insert_if_node($3, $5, $6); 
        }
;

else: ELSE stmt
        { 
            parse_trace("Reducing: else -> ELSE stmt"); 
            $$ = driver->tree.insert_else_node($2); 
        }
    | %empty 
        { 
            parse_trace("Reducing: else -> <empty>"); 
            $$ = nullptr; 
        }
;

while: WHILE LEFT_ROUND_BRACKER comp_expr RIGHT_ROUND_BRACKER stmt
        { 
            parse_trace("Reducing: while -> WHILE LEFT_ROUND_BRACKER comp_expr RIGHT_ROUND_BRACKER stmt"); 
            $$ = driver->tree.insert_while_node($3, $5); 
        }
;

comp_expr: expr 
        { 
            parse_trace("Reducing: comp_expr -> expr"); 
            $$ = driver->tree.insert_expr_node($1); 
        }
; 

expr: expr PLUS expr 
        { 
            parse_trace("Reducing: expr -> expr PLUS expr"); 
            $$ = driver->tree.insert_bin_op_node(ast::BinaryOpType::kAdd, $1, $3);
        }
    | expr MINUS expr 
        { 
            parse_trace("Reducing: expr -> expr MINUS expr"); 
            $$ = driver->tree.insert_bin_op_node(ast::BinaryOpType::kSub, $1, $3);
        }
    | expr MUL expr 
        { 
            parse_trace("Reducing: expr -> expr MUL expr"); 
            $$ = driver->tree.insert_bin_op_node(ast::BinaryOpType::kMul, $1, $3); 
        }
    | expr DIV expr 
        { 
            parse_trace("Reducing: expr -> expr DIV expr"); 
            $$ = driver->tree.insert_bin_op_node(ast::BinaryOpType::kDiv, $1, $3); 
        }
    | LEFT_ROUND_BRACKER expr RIGHT_ROUND_BRACKER 
        { 
            parse_trace("Reducing: expr -> LEFT_ROUND_BRACKER expr RIGHT_ROUND_BRACKER"); 
            $$ = $2; 
        }
    | expr EQUAL expr 
        { 
            parse_trace("Reducing: expr -> expr EQUAL expr"); 
            $$ = driver->tree.insert_log_op_node(ast::LogicalOpType::kEqual, $1, $3); 
        }
    | expr NOT_EQUAL expr 
        { 
            parse_trace("Reducing: expr -> expr NOT_EQUAL expr"); 
            $$ = driver->tree.insert_log_op_node(ast::LogicalOpType::kNotEqual, $1, $3); 
        }
    | expr BELOW expr 
        { 
            parse_trace("Reducing: expr -> expr BELOW expr"); 
            $$ = driver->tree.insert_log_op_node(ast::LogicalOpType::kBelow, $1, $3); 
        }
    | expr GREATER expr 
        { 
            parse_trace("Reducing: expr -> expr GREATER expr"); 
            $$ = driver->tree.insert_log_op_node(ast::LogicalOpType::kGreater, $1, $3); 
        }
    | expr EQUAL_OR_BELOW expr 
        { 
            parse_trace("Reducing: expr -> expr EQUAL_OR_BELOW expr"); 
            $$ = driver->tree.insert_log_op_node(ast::LogicalOpType::kEqualOrBelow, $1, $3); 
        }
    | expr EQUAL_OR_GREATER expr 
        { 
            parse_trace("Reducing: expr -> expr EQUAL_OR_GREATER expr"); 
            $$ = driver->tree.insert_log_op_node(ast::LogicalOpType::kEqualOrGreater, $1, $3); 
        }
    | var_deref 
        { 
            parse_trace("Reducing: expr -> var_deref"); 
            $$ = $1; 
        }
    | value 
        { 
            parse_trace("Reducing: expr -> value"); 
            $$ = $1; 
        }
    | INPUT
        {
            parse_trace("Reducing: expr -> input"); 
            $$ = driver->tree.insert_question_mark_node();
        }
;

value: NUMBER 
        { 
            parse_trace("Reducing: value -> NUMBER"); 
            $$ = driver->tree.insert_value_node($1); 
        }
;

decl: VAR 
        { 
            parse_trace("Reducing: decl -> VAR"); 
            $$ = driver->tree.insert_decl_node($1); 
        }
;

var_deref: VAR
        { 
            parse_trace("Reducing: var_deref -> VAR"); 
            $$ = driver->tree.insert_var_deref_node($1); 
        }
;

%%

namespace yy {

parser::token_type yylex(parser::semantic_type* yylval,
                         location* yyloc,
                         Driver* driver)
{
    return driver->yylex(yylval, yyloc);
}

void parser::error(const location& loc, const std::string& error) {
    std::cerr << "Error: " << error << std::endl;
    std::cerr << "\tLine: " << loc.begin.line << std::endl;
}

} // namespace yy
