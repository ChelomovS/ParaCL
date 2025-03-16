cmake_minimum_required(VERSION 3.13)

project(gtest)

enable_testing()

find_package(GTest REQUIRED)

add_executable(unit_tests
    unit/unit_tests.cpp
)

target_include_directories(unit_tests 
    PRIVATE 
        ${GTEST_INCLUDE_DIRS}
)

target_link_libraries(unit_tests 
    PRIVATE
        pcl-parser
        pcl-interpreter
        pcl-defaults

        spdlog
        ${GTEST_BOTH_LIBRARIES}
)

add_test(NAME unit_tests COMMAND unit_tests)
