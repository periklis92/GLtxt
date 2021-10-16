cmake_minimum_required(VERSION 3.0.0)
project(gltxt VERSION 0.1.0)

add_subdirectory(src)

option(GLTXT_BUILD_EXAMPLE "Build gltxt examples" ON)

if(GLTXT_BUILD_EXAMPLE)
add_subdirectory(example)
endif()