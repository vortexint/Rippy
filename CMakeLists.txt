cmake_minimum_required(VERSION 3.0.0)
project(Rippy VERSION 0.1.0)

set (CMAKE_CXX_STANDARD 20)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/modules/")
set(CMAKE_PREFIX_PATH ${CMAKE_PREFIX_PATH} "${CMAKE_SOURCE_DIR}/modules/")

add_executable(Rippy main.cpp cmdline.cpp)

# add library yaml-cpp (project YAML-CPP) at modules/yaml-cpp
add_subdirectory(modules/yaml-cpp)

target_include_directories(Rippy PRIVATE include ${CMAKE_PREFIX_PATH}/yaml-cpp/include)

target_link_libraries(Rippy PRIVATE yaml-cpp::yaml-cpp)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
