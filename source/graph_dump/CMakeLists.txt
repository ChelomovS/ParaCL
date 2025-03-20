add_library(pcl-graphdump
    STATIC
        graph_dump.cpp
)

target_include_directories(pcl-graphdump
    PUBLIC
        ${CMAKE_SOURCE_DIR}/include/
)

target_link_libraries(pcl-graphdump
    PUBLIC
        spdlog
        pcl-defaults
        pcl-parser
)
