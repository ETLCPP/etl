cmake_minimum_required(VERSION 3.5.0)
project(queued_fsm)

add_definitions(-DETL_DEBUG)

include_directories(${UTPP_INCLUDE_DIRS} ${PROJECT_SOURCE_DIR}/../../include)

set(SOURCE_FILES QueuedFSM.cpp)

add_executable(queued_fsm ${SOURCE_FILES})
target_include_directories(queued_fsm
  PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}
  )

set_property(TARGET queued_fsm PROPERTY CXX_STANDARD 17)

