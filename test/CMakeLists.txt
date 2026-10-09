cmake_minimum_required(VERSION 3.10)
project(etl_unit_tests LANGUAGES CXX)

#include(FetchContent)
#FetchContent_Declare(
#  googletest
#  URL https://github.com/google/googletest/archive/refs/heads/main.zip
#)

#set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
#FetchContent_MakeAvailable(googletest)

add_executable(etl_tests
	main.cpp
	test_variant_variadic.cpp
  )

target_compile_definitions(etl_tests PRIVATE -DETL_DEBUG)

option(ETL_NO_STL "No STL" OFF)

set(EXTRA_COMPILE_OPTIONS "" CACHE STRING "Additional compiler options")
set(EXTRA_LINK_OPTIONS "" CACHE STRING "Additional linker options")
set(EXTRA_LINK_LIBS "" CACHE STRING "Additional libraries to link")
set(EXTRA_TESTING_FLAGS "" CACHE STRING "Additional testing flags for ctest")

if (ETL_CXX_STANDARD MATCHES "98")
	message(STATUS "Compiling for C++98")
    set_property(TARGET etl_tests PROPERTY CXX_STANDARD 98)
elseif (ETL_CXX_STANDARD MATCHES "03")
	message(STATUS "Compiling for C++03 (C++98)")
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

if (NO_STL OR ETL_NO_STL)
	message(STATUS "Compiling for No STL")
	target_compile_definitions(etl_tests PRIVATE -DETL_NO_STL)
else()
	message(STATUS "Compiling for STL")
endif()

if (ETL_USE_TYPE_TRAITS_BUILTINS)
	message(STATUS "Compiling for built-in type traits")
	target_compile_definitions(etl_tests PRIVATE -DETL_USE_TYPE_TRAITS_BUILTINS)
endif()

if (ETL_USER_DEFINED_TYPE_TRAITS)
	message(STATUS "Compiling for user defined type traits")
	target_compile_definitions(etl_tests PRIVATE -DETL_USER_DEFINED_TYPE_TRAITS)
endif()

if (ETL_MESSAGES_ARE_NOT_VIRTUAL)
	message(STATUS "Compiling for non-virtual messages")
	target_compile_definitions(etl_tests PRIVATE -DETL_MESSAGES_ARE_NOT_VIRTUAL)
endif()

if (ETL_FORCE_TEST_CPP03_IMPLEMENTATION)
	message(STATUS "Compiling for forced C++03 tests")
	target_compile_definitions(etl_tests PRIVATE -DETL_FORCE_TEST_CPP03_IMPLEMENTATION)
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

if ((CMAKE_CXX_COMPILER_ID MATCHES "MSVC"))
	message(STATUS "Using MSVC compiler")
	target_compile_options(etl_tests
			PRIVATE
			/Zc:__cplusplus
			)
endif ()

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
#			-fmax-errors=10
			-pedantic-errors
			-Wall
			-Wextra
			-Werror
			-Wfloat-equal
			-Wshadow
			-Wnull-dereference
			-Wsign-conversion
			-Wextra-semi
			-g
			${EXTRA_COMPILE_OPTIONS}
			)
	target_link_options(etl_tests PRIVATE ${EXTRA_LINK_OPTIONS})
endif ()

if (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
	target_compile_options(etl_tests
			PRIVATE
			-fno-omit-frame-pointer
			-fno-common
#			-ferror-limit=10
			-pedantic-errors
			-Wall
			-Wextra
			-Werror
			-Wfloat-equal
			-Wshadow
			-Wnull-dereference
			-Wsign-conversion
			-Wextra-semi
			-Wextra-semi-stmt
			-Wc++11-extra-semi
			-g
			${EXTRA_COMPILE_OPTIONS}
			)
	target_link_options(etl_tests PRIVATE ${EXTRA_LINK_OPTIONS})
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

target_include_directories(etl_tests
		PRIVATE
		${PROJECT_SOURCE_DIR}/../include)

add_subdirectory(UnitTest++)
target_link_libraries(etl_tests PRIVATE UnitTestpp ${EXTRA_LINK_LIBS})

enable_testing()
# Enable the 'make test' CMake target using the executable defined above
add_test(NAME etl_unit_tests COMMAND etl_tests ${EXTRA_TESTING_FLAGS})

# Since ctest will only show you the results of the single executable
# define a target that will output all of the failing or passing tests
# as they appear from UnitTest++
add_custom_target(test_verbose COMMAND ${CMAKE_CTEST_COMMAND} --verbose)
