cmake_minimum_required(VERSION 3.0.0)
project(Rippy VERSION 0.1.0)

set (CMAKE_CXX_STANDARD 20)

add_executable(Rippy main.cpp cmdline.cpp)

target_include_directories(Rippy PRIVATE include)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
