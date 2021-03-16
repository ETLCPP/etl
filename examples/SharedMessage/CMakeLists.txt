cmake_minimum_required(VERSION 3.5.0)
project(SharedMessage)

add_definitions(-DETL_DEBUG)

include_directories(${UTPP_INCLUDE_DIRS} ${PROJECT_SOURCE_DIR}/../../include)

set(SOURCE_FILES SharedMessage.cpp)

add_executable(SharedMessage ${SOURCE_FILES})
target_include_directories(SharedMessage
  PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}
  )

set_property(TARGET SharedMessage PROPERTY CXX_STANDARD 17)

