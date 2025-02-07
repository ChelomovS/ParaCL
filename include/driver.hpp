#ifndef DRIVER_HPP
#define DRIVER_HPP

#include "grammar.tab.hpp"
#include "ast.hpp"

#include <FlexLexer.h>
#include <iostream>
#include <string_view>

#include "entity_table.hpp"

namespace yy {

class Driver {
  private:
    FlexLexer *plex_;

  public:
    AST::Ast tree;
    AST::ScopeNode* current_scope_;
    // maybe тут calculator
    Driver(FlexLexer *plex) : plex_{plex}, current_scope_{tree.insert_scope_node(nullptr)} {tree.root_ = current_scope_;};
    
    parser::token_type yylex(parser::semantic_type *yylval) {
        parser::token_type tt = static_cast<parser::token_type>(plex_->yylex());

        switch (tt) {
            case parser::token_type::NUMBER:
                yylval->as<int>() = std::stoi(plex_->YYText());
                break;
            case parser::token_type::VAR: {
                std::string name_of_variable = plex_->YYText();
                yylval->emplace<std::string>(name_of_variable);
                break;
            }
            default:
                break;
        }

        return tt; 
    }
      
    bool parse() {
        parser parser(this);
        bool res = parser.parse();
        return !res;
    }
    
    void add_node(AST::TreeNode* new_node) {
        current_scope_->add_node(new_node);
    }
      
};    
    
} // namespace yy
      
#endif // DRIVER_HPP
