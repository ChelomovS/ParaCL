#ifndef DRIVER_HXX_
#define DRIVER_HXX_

#include "numgrammar.tab.hh"
#include <FlexLexer.h>

#include "ast.hpp"

namespace yy {

class Driver {
  private:
    FlexLexer *plex_;
  public:
    AST::Ast tree_;

  public:
    Driver(FlexLexer* plex): plex_{plex} {}

    parser::token_type yylex(parser::semantic_type *yylval) {
        parser::token_type tt = static_cast<parser::token_type>(plex_->yylex());
        if (tt == yy::parser::token_type::NUMBER)
            yylval->as<int>() = std::stoi(plex_->YYTEXT);
        if (tt == yy::parser::token_type::VAR) {
            std::string name_of_variable = plex->YYTEXT;
            yylval->emplace<std::string>(std::move(name_of_variable));
        }
        return tt;
    }

    bool parse() {
        parser parser{this};
        bool res = parser.parse();
        return !res;
    }
};

} // namespace yy

#endif // DRIVER_HXX_
