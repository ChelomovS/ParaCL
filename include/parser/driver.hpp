#ifndef DRIVER_HPP
#define DRIVER_HPP

#include <memory>
#include <cstdlib>
#include <cassert>

#include <spdlog/spdlog.h>

#include <location.hh>
#include <grammar.tab.hpp>

#include "parser/ast.hpp"
#include "parser/lexer.hpp"

namespace yy {

class Driver {
  private:
    std::unique_ptr<PclLexer> plex_;
  public:
    ast::Ast tree;
    ast::ScopeNode* current_scope_ = nullptr;
    Driver(PclLexer* plex) : plex_{plex} {
        assert(plex != nullptr);

        current_scope_ = tree.insert_scope_node(nullptr);
        tree.root_ = current_scope_;
    }

    parser::token_type yylex(parser::semantic_type* yylval, location* yyloc) {
        return plex_->yylex(yylval, yyloc);
    }
      
    // returns true iff parsing succeededs
    bool parse() {
        parser parser(this);
        bool res = parser.parse();
        return !res;
    }
    
    void add_node(ast::TreeNode* new_node) {
        assert(new_node != nullptr);

        current_scope_->add_node(new_node);
    }
};

} // namespace yy
      
#endif // DRIVER_HPP
