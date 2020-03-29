cmake_minimum_required(VERSION 3.5.0)
project(queued_message_router)

add_definitions(-DETL_DEBUG)

include_directories(${UTPP_INCLUDE_DIRS} ${PROJECT_SOURCE_DIR}/../../include)

set(SOURCE_FILES QueuedMessageRouter.cpp)

add_executable(queued_message_router ${SOURCE_FILES})
target_include_directories(queued_message_router
  PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}
  )

set_property(TARGET queued_message_router PROPERTY CXX_STANDARD 17)

