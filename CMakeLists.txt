cmake_minimum_required(VERSION 3.10)

project(waied LANGUAGES CXX)
set(CMAKE_VERBOSE_MAKEFILE ON)
add_subdirectory(submodules EXCLUDE_FROM_ALL)
add_subdirectory(src)
