cmake_minimum_required(VERSION 3.5.0)
project(etl_unit_tests)

# Bring in the UnitTest++ include directories and libraries
find_package(UnitTest++)

if(NOT UnitTest++_FOUND)
  # Add unittest-cpp as an ExternalProject  
  include(cmake/unit-test_external_project.cmake)
  add_unittest_cpp()
else()
  include_directories(${UTPP_INCLUDE_DIRS})
endif()



add_executable(etl_tests 
  main.cpp
  test_algorithm.cpp
  )
target_link_libraries(etl_tests etl UnitTest++)
add_test(etl_test_target etl_tests)
