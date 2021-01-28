cmake_minimum_required(VERSION 3.5.0)
project(FunctionInterruptSimulation)

add_definitions(-DETL_DEBUG)

include_directories(${UTPP_INCLUDE_DIRS} ${PROJECT_SOURCE_DIR}/../../include)

set(SOURCE_FILES FunctionInterruptSimulation.cpp)

add_executable(FunctionInterruptSimulation ${SOURCE_FILES})
target_include_directories(FunctionInterruptSimulation
  PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}
  )

set_property(TARGET FunctionInterruptSimulation PROPERTY CXX_STANDARD 17)

