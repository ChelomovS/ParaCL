#include <cstdlib>
#include <fstream>
#include <iostream>
#include <utility>
#include <stdexcept>

#include <spdlog/spdlog.h>
#include <spdlog/common.h>
#include <spdlog/sinks/basic_file_sink.h>

#include "parser/ast.hpp"
#include "parser/driver.hpp"
#include "parser/lexer.hpp"
#include "interpreter/interpreter.hpp"

#if defined (GRAPHVIZ)
#include "graph_dump/graph_dump.hpp"
#endif 

int main(const int argc, const char* argv[]) {
    auto logger = spdlog::basic_logger_mt("paracl", "paracl.log", /*override log?*/ true);
    spdlog::set_default_logger(logger);

#if defined (NDEBUG)
    spdlog::set_level(spdlog::level::info);
#else // NDEBUG
    // spdlog::flush_on(spdlog::level::trace);
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

    yy::PclLexer *lexer = new yy::PclLexer(&pcl_src_file);
    yy::Driver driver(lexer);
    bool no_errors = driver.parse();
    spdlog::info("parse completed: {}", no_errors); 

    ast::Ast ast = std::move(driver.tree);
#if defined (GRAPHVIZ)
    gdump::GraphDump graph_dump{&ast};
    graph_dump.visit_all();

    spdlog::debug("graphviz dumped");
#endif 

    if (!no_errors) {
        return EXIT_FAILURE;
    }

    intpr::Interpreter interpreter(&ast);
    try {
        interpreter.visit_all();
    } catch (const std::runtime_error& e) {
        std::cerr << e.what() << std::endl;
    } catch (...) {
        std::cerr << "Unexpected error" << std::endl;
    }
    spdlog::info("interpreter completed");

    return EXIT_SUCCESS;
}
