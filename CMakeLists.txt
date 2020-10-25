cmake_minimum_required(VERSION 3.10)

project(waied LANGUAGES CXX)

option(USE_CONAN "Use Conan package manager" OFF)
option(STATIC_BUILD "Static build" OFF)

add_subdirectory(submodules EXCLUDE_FROM_ALL)
add_subdirectory(src)
add_subdirectory(tests EXCLUDE_FROM_ALL)
