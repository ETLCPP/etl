#######################################################################
# The Embedded Template Library (https://www.etlcpp.com/)
#######################################################################
cmake_minimum_required(VERSION 3.5.0)
project(etl)

option(BUILD_TESTS "Build unit tests" OFF)
option(NO_STL "No STL" OFF)

add_library(${PROJECT_NAME} INTERFACE)

target_include_directories(${PROJECT_NAME} SYSTEM INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
        )

target_link_libraries(${PROJECT_NAME} INTERFACE)

# only install if top level project
if(${CMAKE_PROJECT_NAME} STREQUAL ${PROJECT_NAME})
   install(TARGETS ${PROJECT_NAME}  EXPORT ${PROJECT_NAME}Config)
   install(EXPORT ${PROJECT_NAME}Config  DESTINATION lib/cmake/${PROJECT_NAME})
   install(DIRECTORY include/${PROJECT_NAME}  DESTINATION include)
endif()

if (BUILD_TESTS)
  enable_testing()
  add_subdirectory(test) 
endif()
