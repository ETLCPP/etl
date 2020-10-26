#######################################################################
# The Embedded Template Library (https://www.etlcpp.com/)
#######################################################################
cmake_minimum_required(VERSION 3.5.0)
project(etl)

option(BUILD_TESTS "Build unit tests" OFF)
option(NO_STL "No STL" OFF)

add_library(etl INTERFACE)

target_include_directories(etl INTERFACE include)

target_compile_definitions(etl INTERFACE)

if (BUILD_TESTS)
  enable_testing()
  add_subdirectory(test) 
endif()
