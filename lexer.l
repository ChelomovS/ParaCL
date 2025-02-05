%option c++

%{ 
#include "grammar.tab.hpp"
#include <iostream>
%}

WS             [ \t\n\v]+
NUMBER         [0-9]+
VAR            [a-zA-Z_][a-zA-Z_0-9]*

%%

{WS}                  /* skip blanks and tabs */
{NUMBER}              return yy::parser::token_type::NUMBER;
")"                   return yy::parser::token_type::RIGHT_ROUND_BRACKER; 
"("                   return yy::parser::token_type::LEFT_ROUND_BRACKER; 
"}"                   return yy::parser::token_type::RIGHT_CURLY_BRACKER; 
"{"                   return yy::parser::token_type::LEFT_CURLY_BRACKER; 
"+"                   return yy::parser::token_type::PLUS; 
"-"                   return yy::parser::token_type::MINUS; 
"/"                   return yy::parser::token_type::DIV; 
"*"                   return yy::parser::token_type::MUL; 
"="                   return yy::parser::token_type::ASSIGNMENT;
"?"                   return yy::parser::token_type::INPUT;
"!="                  return yy::parser::token_type::NOT_EQUAL;
"=="                  return yy::parser::token_type::EQUAL;
"<"                   return yy::parser::token_type::BELOW; 
"<="                  return yy::parser::token_type::EQUAL_OR_BELOW; 
">"                   return yy::parser::token_type::GREATER; 
">="                  return yy::parser::token_type::EQUAL_OR_GREATER; 
";"                   return yy::parser::token_type::SEMICOLON; 
"if"                  return yy::parser::token_type::IF; 
"else"                return yy::parser::token_type::ELSE; 
"while"               return yy::parser::token_type::WHILE;
"print"               return yy::parser::token_type::PRINT;
{VAR}                 return yy::parser::token_type::VAR; 
.                     { std::cerr << "ошибка лексера: " << yytext << std::endl; return yy::parser::token_type::ERROR; }

%%