cmake_minimum_required(VERSION 3.13)
project(paracl)

find_package(BISON REQUIRED)
find_package(FLEX REQUIRED)

set(GRAMMAR "parser.y" CACHE STRING "file with grammar" FORCE)

flex_target(scanner
  lexer.l
  ${CMAKE_CURRENT_BINARY_DIR}/lexer.cpp
) # Cоздание лексера lexer.cpp через lexer.l 

bison_target(parser
  ${GRAMMAR}
  ${CMAKE_CURRENT_BINARY_DIR}/parser.cpp
  COMPILE_FLAGS "--defines=${CMAKE_CURRENT_BINARY_DIR}/grammar.tab.hpp"
) # Cоздание parser.cpp в той же директории через GRAMMAR + создание заголовочного файла grammar.tab.hpp

add_flex_bison_dependency(scanner parser) 

add_executable(${PROJECT_NAME} 
  source/driver.cpp
  ${BISON_parser_OUTPUTS}
  ${FLEX_scanner_OUTPUTS}
) # BISON_parser_OUTPUTS - parser.cpp, FLEX_scanner_OUTPUT - lexer.cpp

set(CMAKE_CXX_FLAGS_DEBUG "-Wall -Wextra -Wpedantic -g -O0 -DDEBUG \
                          -Wmissing-declarations -Wcast-align      \
                          -Wunused -Wshadow -Wsign-conversion      \
                          -fsanitize=address -fsanitize=undefined")

set_target_properties(
    ParaCL PROPERTIES
    CXX_STANDARD          20
    CXX_STANDARD_REQUIRED ON
)

add_subdirectory(tests)

