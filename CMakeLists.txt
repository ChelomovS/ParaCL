cmake_minimum_required(VERSION 3.20)
project(ParaCL CXX)

include(cmake/defaults.cmake)
include(cmake/third_party.cmake)

if (GRAPHVIZ)
    add_definitions(-DGRAPHVIZ=1)
    message(STATUS "graphviz enabled")
else()
    message(STATUS "graphviz disabled")
endif()

add_subdirectory(source/interpreter)
add_subdirectory(source/parser)

add_executable(${PROJECT_NAME})

target_sources(${PROJECT_NAME}
    PRIVATE
        source/main.cpp
)

target_link_libraries(${PROJECT_NAME}
    PRIVATE
        pcl-defaults
        pcl-interpreter
        pcl-parser

        spdlog
)

add_subdirectory(tests)
