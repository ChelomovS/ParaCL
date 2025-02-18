#ifndef DRIVER_HPP
#define DRIVER_HPP

#include <string>
#include <cstdlib>

#include <FlexLexer.h>

#include "grammar.tab.hpp"
#include "ast.hpp"

namespace yy {

class Driver {
  private:
    FlexLexer *plex_;

  public:
    ast::Ast tree;
    ast::ScopeNode* current_scope_ = nullptr;

    Driver(FlexLexer* plex) : plex_{plex} {
        current_scope_ = tree.insert_scope_node(nullptr);
        tree.root_ = current_scope_;
    }
    parser::token_type yylex(parser::semantic_type* yylval, location* yyloc) {
        parser::token_type tt = static_cast<parser::token_type>(plex_->yylex());
        
        yyloc->initialize(nullptr, plex_->lineno(), 0);

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
      
    // returns true iff parsing succeededs
    bool parse() {
        parser parser(this);
        bool res = parser.parse();
        return !res;
    }
    
    void add_node(ast::TreeNode* new_node) {
        current_scope_->add_node(new_node);
    }
    
};

} // namespace yy
      
#endif // DRIVER_HPP
