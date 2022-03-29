cmake_minimum_required(VERSION 3.5.0)
project(etl_initializer_list_unit_tests)

add_definitions(-DETL_DEBUG)
add_definitions(-DETL_NO_STL)
add_definitions(-DETL_FORCE_ETL_INITIALIZER_LIST)

include_directories(${PROJECT_SOURCE_DIR}/../../include)

set(TEST_SOURCE_FILES
	test_initializer_list.cpp
  )

add_executable(etl_tests
  ${TEST_SOURCE_FILES}
  )

target_include_directories(etl_tests
  PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}
  )

# Enable the 'make test' CMake target using the executable defined above
add_test(etl_initializer_list_unit_tests etl_tests)

# Since ctest will only show you the results of the single executable
# define a target that will output all of the failing or passing tests
# as they appear from UnitTest++
add_custom_target(test_verbose COMMAND ${CMAKE_CTEST_COMMAND} --verbose)


#RSG
set_property(TARGET etl_tests PROPERTY CXX_STANDARD 17)

