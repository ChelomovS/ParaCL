#ifndef DRIVER_HPP
#define DRIVER_HPP

#include <fstream>
#include <memory>
#include <string>
#include <cstdlib>
#include <utility>

#undef yyFlexLexer
#include <FlexLexer.h>

#include <spdlog/spdlog.h>

#include "grammar.tab.hpp"

#include "location.hh"
#include "parser/ast.hpp"

namespace yy {

class PclLexer : public yyFlexLexer {
  private:
    location cur_loc_{nullptr, 1, 1};
  public:
    PclLexer(std::ifstream* in_stream) : yyFlexLexer(in_stream) {}
    
    void token() {
        cur_loc_.step();
        cur_loc_.columns(yyleng);
    }
    void new_line() {
        cur_loc_.step();
        cur_loc_.lines(yyleng);
    }
    location current_loc() {
        return cur_loc_;
    }

    int yylex() override;
};

class Driver {
  private:
    std::unique_ptr<PclLexer> plex_;
  public:
    ast::Ast tree;
    ast::ScopeNode* current_scope_ = nullptr;

    Driver(PclLexer* plex) : plex_{plex} {
        current_scope_ = tree.insert_scope_node(nullptr);
        tree.root_ = current_scope_;
    }
    parser::token_type yylex(parser::semantic_type* yylval, location* yyloc) {
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

        location current_loc = plex_->current_loc();
        yyloc->initialize(nullptr, current_loc.begin.line, current_loc.begin.column);
        spdlog::trace("location: {} {} {} {}", yyloc->begin.line, yyloc->begin.column, yyloc->end.line, yyloc->end.column);
        spdlog::trace("lenght: {}", plex_->YYLeng());
        spdlog::trace("text: {}", plex_->YYText());

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
