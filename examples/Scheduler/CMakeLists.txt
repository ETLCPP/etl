cmake_minimum_required(VERSION 3.5.0)
project(scheduler)

add_definitions(-DETL_DEBUG)

include_directories(${UTPP_INCLUDE_DIRS} ${PROJECT_SOURCE_DIR}/../../include)

set(SOURCE_FILES Scheduler.cpp)

add_executable(scheduler ${SOURCE_FILES})
target_include_directories(scheduler
  PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}
  )

set_property(TARGET scheduler PROPERTY CXX_STANDARD 17)

