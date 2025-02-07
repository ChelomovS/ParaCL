#include <cstdlib>
#include <fstream>
#include <iostream>

#include "driver.hpp"
#include "calculator.hpp"

int yyFlexLexer::yywrap() { return 1; }
    
int main(const int argc, const char* argv[]) {
    if (argc < 2) {
        std::cerr << "Not enough files\n";
        std::cerr << "Usage: " << argv[0] << " <src_file>\n";

        return EXIT_FAILURE;
    }

    std::ifstream pcl_src_file(argv[1]);
    if (!pcl_src_file) {
        std::cerr << "Error: could not open file: " << argv[1] << "\n";
        
        return EXIT_FAILURE;
    }

    FlexLexer *lexer = new yyFlexLexer(&pcl_src_file);
    yy::Driver driver(lexer);
    driver.parse();

    std::ofstream dump_file{}; // FIXME 
    driver.tree.print(dump_file);

    delete lexer;
}
