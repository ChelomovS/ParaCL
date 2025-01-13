%option c++

WS             [ \n\t\v]+
   
DIGIT          -|[1-9]
DIGIT1         [0-9]

RIGHT_ROUND_BRACKER [)]
LEFT_ROUND_BRACKER  [(]
RIGHT_CURLY_BRACKER [{]
LEFT_CURLY_BRACKER  [}]

PLUS           [+]
MINUS          [-]
DIV            [/]
MUL            [*]

ASSIGMENT      [=]

INPUT          [?]

NOT_EQUAL      [!=]
EQUAL          [==]
BELOW          [<]
EQUAL_OR_BELOW [<=]
GREATER        [>]

SEMICOLON      [;]

IF             [if]
ELSE           [else]
WHILE          [while]

PRINT          [print]

VAR            [a-zA-Z_][a-zA-Z_0-9]*

%%
WS                    /* skip blanks and tabs */
{DIGIT1}{DIGIT}*      return yy::parser::token_type::NUMBER;
{RIGHT_ROUND_BRACKER} return yy::parser::token_type::RIGHT_ROUND_BRACKER;
{LEFT_ROUND_BRACKER}  return yy::parser::token_type::LEFT_ROUND_BRACKER;
{RIGHT_CURLY_BRACKER} return yy::parser::token_type::RIGHT_CURLY_BRACKER;
{LEFT_CURLY_BRACKER}  return yy::parser::token_type::LEFT_CURLY_BRACKER;
{PLUS}                return yy::parser::token_type::PLUS;
{MINUS}               return yy::parser::token_type::MINUS;
{DIV}                 return yy::parser::token_type::DIV;
{MUL}                 return yy::parser::token_type::MUL;
{ASSIGNMENT}          return yy::parser::token_type::ASSIGNMENT;
{INPUT}               return yy::parser::token_type::INPUT;
{NOT_EQUAL}           return yy::parser::token_type::NOT_EQUAL;
{EQUAL}               return yy::parser::token_type::EQUAL;
{BELOW}               return yy::parser::token_type::BELOW;
{EQUAL_OR_BELOW}      return yy::parser::token_type::EQUAL_OR_BELOW;
{GREATER}             return yy::parser::token_type::GREATER;
{SEMICOLON}           return yy::parser::token_type::SEMICOLON;
{IF}                  return yy::parser::token_type::IF;
{ELSE}                return yy::parser::token_type::ELSE;
{WHILE}               return yy::parser::token_type::WHILE;
{PRINT}               return yy::parser::token_type::PRINT;
.                     return yy::parser::token_type::ERROR;