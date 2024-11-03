cmake_minimum_required(VERSION 3.17)
project(ParaCL)

set(CMAKE_CXX_STANDARD          20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(GTest REQUIRED)
enable_testing()

set(INCLUDE_DIR ${PROJECT_SOURCE_DIR}/include)

aux_source_directory(source SRC_FILES)

add_executable(ParaCL ${SRC_FILES})

target_include_directories(ParaCL PRIVATE ${INCLUDE_DIR} ${GTEST_INCLUDE_DIRS})

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

target_link_libraries(ParaCL ${GTEST_BOTH_LIBRARIES})
