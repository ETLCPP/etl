cmake_minimum_required(VERSION 3.5.0)
project(etl_error_handler_unit_tests)

add_definitions(-DETL_DEBUG)
add_definitions(-DETL_THROW_EXCEPTIONS)

include_directories(${PROJECT_SOURCE_DIR}/../../../include)

set(TEST_SOURCE_FILES
	test_error_handler.cpp
  )

add_executable(etl_tests
  ${TEST_SOURCE_FILES}
  )

if (ETL_CXX_STANDARD MATCHES "98")
	message(STATUS "Compiling for C++98")
    set_property(TARGET etl_tests PROPERTY CXX_STANDARD 98)
elseif (ETL_CXX_STANDARD MATCHES "03")
	message(STATUS "Compiling for C++98")    
	set_property(TARGET etl_tests PROPERTY CXX_STANDARD 98)
elseif (ETL_CXX_STANDARD MATCHES "11")
	message(STATUS "Compiling for C++11")    
	set_property(TARGET etl_tests PROPERTY CXX_STANDARD 11)
elseif (ETL_CXX_STANDARD MATCHES "14")
	message(STATUS "Compiling for C++14")    
	set_property(TARGET etl_tests PROPERTY CXX_STANDARD 14)
elseif (ETL_CXX_STANDARD MATCHES "17")
	message(STATUS "Compiling for C++17")    
	set_property(TARGET etl_tests PROPERTY CXX_STANDARD 17)
elseif (ETL_CXX_STANDARD MATCHES "20")
	message(STATUS "Compiling for C++20")
	set_property(TARGET etl_tests PROPERTY CXX_STANDARD 20)
elseif (ETL_CXX_STANDARD MATCHES "23")
	if (CMAKE_VERSION VERSION_GREATER_EQUAL "3.20")
		message(STATUS "Compiling for C++23")
		set_property(TARGET etl_tests PROPERTY CXX_STANDARD 23)
	else()
		message(STATUS "CMake version ${CMAKE_VERSION} does not support C++23, falling back to C++20")
		set_property(TARGET etl_tests PROPERTY CXX_STANDARD 20)
	endif()
elseif (ETL_CXX_STANDARD MATCHES "26")
	if (CMAKE_VERSION VERSION_GREATER_EQUAL "3.25")
		message(STATUS "Compiling for C++26")
		set_property(TARGET etl_tests PROPERTY CXX_STANDARD 26)
	elseif (CMAKE_VERSION VERSION_GREATER_EQUAL "3.20")
		message(STATUS "CMake version ${CMAKE_VERSION} does not support C++26, falling back to C++23")
		set_property(TARGET etl_tests PROPERTY CXX_STANDARD 23)
	else()
		message(FATAL_ERROR "CMake version ${CMAKE_VERSION} is too old to support C++23 or C++26. Please upgrade to CMake 3.20 or later.")
	endif()
else()
	message(STATUS "Compiling for C++17")
	set_property(TARGET etl_tests PROPERTY CXX_STANDARD 17)
endif()

if (ETL_OPTIMISATION MATCHES "-O1")
  message(STATUS "Compiling with -O1 optimisations")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O1")
endif()

if (ETL_OPTIMISATION MATCHES "-O2")
  message(STATUS "Compiling with -O2 optimisations")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2")
endif()

if (ETL_OPTIMISATION MATCHES "-O3")
  message(STATUS "Compiling with -O3 optimisations")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")
endif()

target_include_directories(etl_tests
  PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}
  )

if (CMAKE_CXX_COMPILER_ID MATCHES "GNU")
	message(STATUS "Using GCC compiler")
endif ()

if (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    message(STATUS "Using Clang compiler")
endif ()

if (CMAKE_CXX_COMPILER_ID MATCHES "GNU")
	target_compile_options(etl_tests
			PRIVATE
			-fno-omit-frame-pointer
			-fno-common
			-Wall
			-Wextra
			-Werror
			-Wfloat-equal
			-Wshadow
			-Wnull-dereference
			)
endif ()

if (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
	target_compile_options(etl_tests
			PRIVATE
			-fno-omit-frame-pointer
			-fno-common
			-Wall
			-Wextra
			-Werror
			-Wfloat-equal
			-Wshadow
			-Wnull-dereference
			)
endif ()

if ((CMAKE_CXX_COMPILER_ID MATCHES "GNU") OR (CMAKE_CXX_COMPILER_ID MATCHES "Clang"))
	if (ETL_ENABLE_SANITIZER MATCHES "ON")
		message(STATUS "Compiling with Sanitizer enabled")
		# MinGW doesn't presently support sanitization
		if (NOT MINGW)
			target_compile_options(etl_tests
				PRIVATE
				-fsanitize=address,undefined,bounds
				)

			target_link_options(etl_tests
				PRIVATE
				-fsanitize=address,undefined,bounds
				)	
		endif()
	endif ()
endif ()

# Enable the 'make test' CMake target using the executable defined above
add_test(etl_error_handler_unit_tests etl_tests)

# Since ctest will only show you the results of the single executable
# define a target that will output all of the failing or passing tests
# as they appear from UnitTest++
add_custom_target(test_verbose COMMAND ${CMAKE_CTEST_COMMAND} --verbose)


