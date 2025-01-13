%language "c++"

%skeleton "lalr1.cc"
%defines
%define api.value.type variant 
%param {yy::Driver* driver}

%code requires 
{
#include <iostream>
#include <string>

#include "ast.hxx"

namespace yy { class Driver; }
}

%code
{
#include "driver.hxx"

namespace yy {

parser::token_type yylex(parser::semantic_type* yylval,
                         Driver* driver)
}
}

%token 
    RIGHT_ROUND_BRACKER ")"
    LEFT_ROUND_BRACKER  "("
    RIGHT_CURLY_BRACKER "{"
    LEFT_CURLY_BRACKER  "}"
    PLUS                "+"
    MINUS               "-"
    DIV                 "/"
    MUL                 "*"
    ASSINGMENT          "="
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

%left PLUS MINUS DIV MUL

%right ASSIGNMENT

%nonassoc BELOW EQUAL_OR_BELOW GREATER EQUAL_OR_GREATER

%token <int>                    INT_NUMBER
%token <std::string>            VAR 

%nterm <Ast::WhileNode*>        while
%nterm <Ast::IfNode*>           if
%nterm <Ast::ElseNode*>         else
%nterm <Ast::TreeNode*>         expr ///???
%nterm <Ast::DeclNode*>         decl 
%nterm <Ast::AssignmentNode>    assigment
%nterm <Ast::ValueNode*>        value
%nterm <Ast::PrintNode*>        print
%nterm <Ast::BinOpNode*>        plus minus mult div
%nterm <Ast::QuestionMarkNode*> input
%nterm <Ast::ScopeNode*>        stmts

program: stmts { driver->tree.calculate(); } ????
;

stmts: stmts stmt { driver->tree.insert_scope($1, $2); } ????
     | %empty {}
;

stmt: print SEMICOLON
        { $$ = $1; }
    | if 
        { $$ = $1; }
    | while 
        { $$ = $1; }
    | assigment SEMICOLON
        { $$ = $1; }
    | expr SEMICOLON
        { $$ = $1; }
;
    
assigment: decl ASSIGMENT expr { $$ = driver->tree.insert_assigment_node($1, $3); }
;

expr: expr PLUS expr   
      { $$ = driver->tree.insert_binary_op_node(Ast::BinaryOpType::kAdd, $1, $3); }

    | expr MINUS expr  
      { $$ = driver->tree.insert_binary_op_node(Ast::BinaryOpType::kSub, $1, $3); }

    | expr MUL expr    
      { $$ = driver->tree.insert_binary_op_node(Ast::BinaryOpType::kMul, $1, $3); }

    | expr DIV expr    
      { $$ = driver->tree.insert_binary_op_node(Ast::BinaryOpType::kDiv, $1, $3); }

    | LEFT_ROUND_BRACKER expr RIGHT_ROUND_BRACKER 
      { $$ = $2; }

    | expr EQUAL expr            
      { $$ = driver->tree.insert_log_op_node(Ast::LogicalOpOpType::kEqual, $1, $3); }

    | expr NOT_EQUAL expr 
      { $$ = driver->tree.insert_log_op_node(Ast::LogicalOpOpType::kNotEqual, $1, $3); }

    | expr BELOW expr 
      { $$ = driver->tree.insert_log_op_node(Ast::LogicalOpOpType::kBelow, $1, $3); }

    | expr GREATER expr 
      { $$ = driver->tree.insert_log_op_node(Ast::LogicalOpOpType::kGreater, $1, $3); }

    | expr EQUAL_OR_BELOW expr
      { $$ = driver->tree.insert_log_op_node(LogicalOperation::Ast::LogicalOpOpType::kEqualOrBelow, $1, $3); }

    | expr EQUAL_OR_GREATER expr
      { $$ = driver->tree.insert_log_op_node(LogicalOperation::Ast::LogicalOpOpType::kEqualOrGreater, $1, $3); }

    | decl 
      { $$ = $1; }

    | value 
      { $$ = $1; }

    | assigment
      { $$ = $1; }
;

if: IF LEFT_ROUND_BRACKER expr RIGHT_ROUND_BRACKER 
    LEFT_CURLY_BRACKER stmts RIGHT_CURLY_BRACKER
    else
      { $$ = driver->tree.insert_if_node($3, $6, $8); }
;

else: ELSE LEFT_CURLY_BRACKER stmts RIGHT_CURLY_BRACKER
        { $$ = driver->tree.insert_else_node($3); }
    | %empty 
        { $$ = nullptr; }
;

print: PRINT LEFT_ROUND_BRACKER decl RIGHT_ROUND_BRACKER { $$ = driver->tree.insert_print_node($2); }
;

input: decl ASSIGMENT INPUT 
         { $$ = driver->tree.insert_question_mark_node($1); }
;

while: WHILE LEFT_ROUND_BRACKER expr RIGHT_ROUND_BRACKER LEFT_CURLY_BRACKER stmts RIGHT_CURLY_BRACKER
         { $$ = driver->tree.insert_while_node($3, $6); }
;

value: INT_NUMBER { $$ = driver->tree.insert_value_node($1); }
;

decl: VAR { && = driver->tree.insert_decl_node($1); }
;