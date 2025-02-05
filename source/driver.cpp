#include <iostream>

#include "driver.hpp"
#include "caclulator.hpp"

static std::ofstream dump_file{};

int yyFlexLexer::yywrap() { return 1; }

int main() {
  FlexLexer *lexer = new yyFlexLexer;
  yy::Driver driver(lexer);
  driver.parse();
  driver.tree.print(dump_file);
  delete lexer;
}