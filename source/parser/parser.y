%language "c++"

%skeleton "lalr1.cc"
%defines

%define api.value.type variant 
%define parse.error verbose
%locations
%define api.location.file "location.hh"

%param {yy::Driver* driver}

%code requires 
{
#include <iostream>
#include <string>

#include "parser/ast.hpp"

namespace yy { class Driver; }
}

%code
{
#include "parser/driver.hpp"

namespace yy {

parser::token_type yylex(parser::semantic_type* yylval, 
                         parser::location_type* loc_type, 
                         Driver* driver);

} // namespace yy

}

%{
#include <spdlog/spdlog.h>
#if 1
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
    MODULUS             "%"
    ASSIGNMENT          "="
    INPUT               "?"
    NOT_EQUAL           "!="
    EQUAL               "=="
    BELOW               "<"
    EQUAL_OR_BELOW      "<="
    GREATER             ">"
    EQUAL_OR_GREATER    ">="
    NOT                 "!"
    AND                 "&&"
    OR                  "||"
    XOR                 "^"
    SEMICOLON           ";"
    IF                  "if"
    ELSE                "else"
    WHILE               "while"
    PRINT               "print"
    ERROR
;

%right ASSIGNMENT
%nonassoc EQUAL NOT_EQUAL BELOW GREATER EQUAL_OR_BELOW EQUAL_OR_GREATER
%left XOR
%left OR
%left AND
%left PLUS MINUS
%left MUL DIV MODULUS
%nonassoc UMINUS NOT
%nonassoc XIF
%nonassoc ELSE

%token <int>                    NUMBER
%token <std::string>            VAR 

%nterm <ast::TreeNode*>         program
%nterm <ast::TreeNode*>         expr 
%nterm <ast::TreeNode*>         expr_stmt
%nterm <ast::TreeNode*>         decl
%nterm <ast::TreeNode*>         var_deref
%nterm <ast::TreeNode*>         assignment
%nterm <ast::TreeNode*>         value
%nterm <ast::TreeNode*>         print
%nterm <ast::TreeNode*>         if_stmt
%nterm <ast::TreeNode*>         while_stmt
%nterm <ast::TreeNode*>         else

%nterm <ast::TreeNode*>         stmt
%nterm <ast::TreeNode*>         stmts
%nterm <ast::TreeNode*>         scope

%nterm <ast::TreeNode*>         begin_of_scope
%nterm <ast::TreeNode*>         end_of_scope

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
    | %empty 
        {
            parse_trace("Reducing: stmts -> <empty>");
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

stmt: print
        { 
            parse_trace("Reducing: stmt -> print"); 
            $$ = $1; 
        }
    | if_stmt 
        { 
            parse_trace("Reducing: stmt -> if_stmt"); 
            $$ = $1; 
        }
    | while_stmt 
        {  
            parse_trace("Reducing: stmt -> while_stmt"); 
            $$ = $1; 
        }
    | expr_stmt
        { 
            parse_trace("Reducing: stmt -> comp_expr"); 
            $$ = $1; 
        }
    | scope 
        { 
            parse_trace("Reducing: stmt -> scope"); 
            $$ = $1; 
        }
;

if_stmt: IF LEFT_ROUND_BRACKER expr RIGHT_ROUND_BRACKER stmt else
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
    | %prec XIF %empty
        { 
            parse_trace("Reducing: else -> <prec> XIF <empty>"); 
            $$ = nullptr; 
        }
;

while_stmt: WHILE LEFT_ROUND_BRACKER expr RIGHT_ROUND_BRACKER stmt
        { 
            parse_trace("Reducing: while -> WHILE LEFT_ROUND_BRACKER comp_expr RIGHT_ROUND_BRACKER stmt"); 
            $$ = driver->tree.insert_while_node($3, $5); 
        }
;

print: PRINT expr SEMICOLON 
        {
            parse_trace("Reducing: print -> PRINT comp_expr SEMICOLON"); 
            $$ = driver->tree.insert_print_node($2); 
        }
;

expr_stmt: SEMICOLON
        {
            parse_trace("Reducing: expr_stmt -> SEMICOLON");
        }
    | expr SEMICOLON
        { 
            parse_trace("Reducing: expr_stmt -> expr SEMICOLON"); 
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
    | expr MODULUS expr
        {
            parse_trace("Reducing: expr -> expr MODULUS expr"); 
            $$ = driver->tree.insert_bin_op_node(ast::BinaryOpType::kModulus, $1, $3); 
        }
    | NOT expr  
        {
            parse_trace("Reducing: expr -> NOT expr"); 
            $$ = driver->tree.insert_un_op_node(ast::UnaryOpType::kNot, $2);
        }
    | MINUS expr  
        {
            parse_trace("Reducing: expr -> MINUS expr"); 
            $$ = driver->tree.insert_un_op_node(ast::UnaryOpType::kMinus, $2);
        }
    | PLUS expr 
        {
            parse_trace("Reducing: expr -> PLUS expr"); 
            $$ = driver->tree.insert_un_op_node(ast::UnaryOpType::kPlus, $2);
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
    | expr AND expr 
        { 
            parse_trace("Reducing: expr -> expr AND expr"); 
            $$ = driver->tree.insert_log_op_node(ast::LogicalOpType::kAnd, $1, $3); 
        }   
    | expr OR expr 
        { 
            parse_trace("Reducing: expr -> expr OR expr"); 
            $$ = driver->tree.insert_log_op_node(ast::LogicalOpType::kOr, $1, $3); 
        }
    | expr XOR expr 
        { 
            parse_trace("Reducing: expr -> expr XOR expr"); 
            $$ = driver->tree.insert_log_op_node(ast::LogicalOpType::kXor, $1, $3); 
        }
    | assignment
        {
            parse_trace("Reducing: expr -> assignment"); 
            $$ = $1; 
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
            parse_trace("Reducing: expr -> INPUT"); 
            $$ = driver->tree.insert_question_mark_node();
        }
;

assignment: decl ASSIGNMENT expr
        { 
            parse_trace("Reducing: assignment -> decl ASSIGNMENT expr"); 
            $$ = driver->tree.insert_assignment_node($1, $3); 
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
    std::cerr << "\tLocation: " << loc.begin.line << ":" << loc.begin.column << std::endl;
}

} // namespace yy
