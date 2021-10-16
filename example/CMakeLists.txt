cmake_minimum_required(VERSION 3.0.0)
project(gltxt_test VERSION 0.1.0)


if (MSVC)
include("C:/vcpkg/scripts/buildsystems/vcpkg.cmake")
endif()

find_package(glm REQUIRED)
find_package(glfw3 3.3 REQUIRED)

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
include_directories(include)
add_definitions(-DGLFW_INCLUDE_NONE)
file(GLOB GLTXT_SOURCES 
"src/*.cpp"
"src/**/*.cpp"
"src/*.c"
"src/**/*.c")

add_executable(${PROJECT_NAME} main.cpp ${GLTXT_SOURCES})

if (UNIX)
target_link_libraries(${PROJECT_NAME} PRIVATE ${CMAKE_DL_LIBS})
endif()

target_link_libraries(${PROJECT_NAME} PRIVATE glfw gltxt)
target_include_directories(${PROJECT_NAME} PRIVATE ${GLM_INCLUDE_DIRS})

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)

set(${PROJECT_NAME}_RESOURCE_DIR
    ${CMAKE_CURRENT_SOURCE_DIR}/resources
    CACHE INTERNAL "${PROJECT_NAME}: Resources Directories" FORCE)

ADD_CUSTOM_COMMAND(TARGET ${PROJECT_NAME}
    PRE_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory $<TARGET_FILE_DIR:${PROJECT_NAME}>/resources
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/resources $<TARGET_FILE_DIR:${PROJECT_NAME}>/resources
)
