add_library(pcl-interpreter
    STATIC
        interpreter.cpp
)

target_include_directories(pcl-interpreter
    PUBLIC
        ${CMAKE_SOURCE_DIR}/include/
)

target_link_libraries(pcl-interpreter
    PUBLIC
        spdlog
        pcl-defaults
        pcl-parser
)
