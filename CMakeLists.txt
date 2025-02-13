cmake_minimum_required(VERSION 3.13)
project(ParaCL)

set(FLEX_INCLUDE_DIR "/usr/include" CACHE PATH "path to flex include dir")

find_package(BISON REQUIRED)
find_package(FLEX REQUIRED)

set(GRAMMAR "parser.y" CACHE STRING "file with grammar" FORCE)

flex_target(scanner
    lexer.l
    ${CMAKE_CURRENT_BINARY_DIR}/lexer.cpp
)

bison_target(parser
    ${GRAMMAR}
    ${CMAKE_CURRENT_BINARY_DIR}/parser.cpp
    COMPILE_FLAGS "--defines=${CMAKE_CURRENT_BINARY_DIR}/grammar.tab.hpp"
)

add_flex_bison_dependency(scanner parser)

set(INCLUDE_DIR ${PROJECT_SOURCE_DIR}/include)

add_executable(${PROJECT_NAME}
    source/main.cpp
    source/ast.cpp
    source/interpreter.cpp
    ${BISON_parser_OUTPUTS}
    ${FLEX_scanner_OUTPUTS}
)

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fdiagnostics-color=always")
endif()

target_include_directories(${PROJECT_NAME} PRIVATE
    ${INCLUDE_DIR}
    ${FLEX_INCLUDE_DIRS}
    ${CMAKE_CURRENT_BINARY_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_20)

set(CMAKE_CXX_FLAGS_DEBUG "-Wall -Wextra -Wpedantic -g -Og -ggdb -DDEBUG -Wunused -Wshadow -fsanitize=address,undefined")

include(cmake/CPM.cmake)

CPMAddPackage(
    NAME spdlog
    GITHUB_REPOSITORY gabime/spdlog
    GIT_TAG v1.15.0
)

if (spdlog_ADDED)
    target_link_libraries(${PROJECT_NAME} PRIVATE spdlog)
endif() 

add_subdirectory(tests)