#include <cstdlib>
#include <fstream>
#include <iostream>

#include <spdlog/common.h>
#include <spdlog/spdlog.h>
#include <spdlog/sinks/basic_file_sink.h>

#include "ast.hpp"
#include "driver.hpp"
#include "interpreter.hpp"

int main(const int argc, const char* argv[]) {
    auto logger = spdlog::basic_logger_mt("paracl", "../paracl.log", /*override log?*/ true);
    spdlog::set_default_logger(logger);

#if defined (NDEBUG)
    spdlog::set_level(spdlog::level::info);
#else // NDEBUG
    spdlog::flush_on(spdlog::level::trace);
    spdlog::set_level(spdlog::level::trace);
#endif // NDEBUG

    // log argv
    for (int i = 0; i < argc; i++) {
        spdlog::info("argv[{}]: {}", i, argv[i]);
    }

    if (argc < 2) {
        spdlog::error("not enough files");

        std::cerr << "Not enough files\n";
        std::cerr << "Usage: " << argv[0] << " <src_file>\n";

        return EXIT_FAILURE;
    }

    std::ifstream pcl_src_file(argv[1]);
    if (!pcl_src_file) {
        spdlog::error("could not open file {}", argv[1]);

        std::cerr << "Error: could not open file: " << argv[1] << "\n";
        
        return EXIT_FAILURE;
    }

    FlexLexer *lexer = new yyFlexLexer(&pcl_src_file);
    yy::Driver driver(lexer);
    bool no_errors = driver.parse();
    spdlog::info("parse completed: {}", no_errors);

    std::ofstream dump_file{}; // FIXME 
    driver.tree.print(dump_file);
    
    if (no_errors) {
        intpr::Interpreter interpreter(driver.tree);
        interpreter.visit_all();
        spdlog::info("interpreter completed");
    }

    delete lexer;
    
    return 0;
}
