set(FLEX_INCLUDE_DIR "/usr/include" CACHE PATH "path to flex include dir")

find_package(BISON REQUIRED)
find_package(FLEX REQUIRED)

set(GRAMMAR "parser.y" CACHE STRING "file with grammar" FORCE)

flex_target(scanner
    lexer.l
    ${CMAKE_BINARY_DIR}/lexer.cpp
)

bison_target(parser
    ${GRAMMAR}
    ${CMAKE_BINARY_DIR}/parser.cpp
    DEFINES_FILE ${CMAKE_BINARY_DIR}/grammar.tab.hpp
)

add_flex_bison_dependency(scanner parser)

add_library(pcl-parser
    STATIC
        ast.cpp        
        ${BISON_parser_OUTPUTS}
        ${FLEX_scanner_OUTPUTS}
)

target_include_directories(pcl-parser
    PUBLIC
        ${CMAKE_SOURCE_DIR}/include/
        ${FLEX_INCLUDE_DIRS}
        ${CMAKE_BINARY_DIR}
)

target_link_libraries(pcl-parser
    PUBLIC
        spdlog
        pcl-defaults
)
