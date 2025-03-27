#ifndef LEXER_HPP_
#define LEXER_HPP_

#include <fstream>
#include <string>
#include <cassert>

#include <spdlog/spdlog.h>

#undef yyFlexLexer
#include <FlexLexer.h>
#include <location.hh>
#include <grammar.tab.hpp>

namespace yy {

class PclLexer : public yyFlexLexer {
  private:
    location cur_loc_;
  public:
    PclLexer(std::ifstream* in_stream) : yyFlexLexer(in_stream), cur_loc_(nullptr, 1, 1) {}
    
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

    parser::token_type yylex(parser::semantic_type* yylval, location* yyloc) {
        assert(yylval != nullptr);
        assert(yyloc != nullptr);  

        parser::token_type tt = static_cast<parser::token_type>(yylex());
        switch (tt) {
            case parser::token_type::NUMBER:
                yylval->as<int>() = std::stoi(YYText());
                break;
            case parser::token_type::VAR: {
                std::string name_of_variable = YYText();
                yylval->emplace<std::string>(name_of_variable);
                break;
            }
            default:
                break;
        }

        location current_loc = cur_loc_;
        yyloc->initialize(nullptr, current_loc.begin.line, current_loc.begin.column);
        spdlog::trace("location: {} {} {} {}", yyloc->begin.line, yyloc->begin.column, yyloc->end.line, yyloc->end.column);
        spdlog::trace("lenght: {}", YYLeng());
        spdlog::trace("text: {}", YYText());

        return tt; 
    }
};

} // namespace yy

#endif // LEXER_HPP_
