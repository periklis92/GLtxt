find_package(OpenGL REQUIRED)

add_library(    gltxt
                "${CMAKE_SOURCE_DIR}/include/gltxt/character.h"
                "${CMAKE_SOURCE_DIR}/include/gltxt/font.h"
                "${CMAKE_SOURCE_DIR}/include/gltxt/gltxt.h"
                "${CMAKE_SOURCE_DIR}/include/gltxt/label.h"
                "${CMAKE_SOURCE_DIR}/include/gltxt/mesh.h"
                gltxt.cpp character.cpp font.cpp
                internal.h internal.cpp label.cpp)

target_include_directories( gltxt PUBLIC
                            "$<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>"
                            "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>")