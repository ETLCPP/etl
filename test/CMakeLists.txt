cmake_minimum_required(VERSION 3.5.0)
project(etl_unit_tests)

add_definitions(-DETL_DEBUG)


if(NOT UnitTest++_FOUND)
  # Add unittest-cpp as an ExternalProject
  include(cmake/unit-test_external_project.cmake)
  add_unittest_cpp()
else()
  include(${UnitTest++_CONFIG})
  # The CMake install of UnitTest++ puts the include directory to ${CMAKE_INSTALL_PREFIX}/include
  # under the assumptions that clients will consume the headers via
  # #include<UnitTest++/UnitTest++.h>
  # However the tests in etl include the headers directly, e.g.
  # #include "UnitTest++.h"
  # Therefore we update the provide UTPP_INCLUDE_DIRS here
  set(UTPP_INCLUDE_DIRS "${UTPP_INCLUDE_DIRS}/UnitTest++")
endif()

include_directories(${UTPP_INCLUDE_DIRS} ${PROJECT_SOURCE_DIR}/../include)

set(TEST_SOURCE_FILES
  main.cpp
  murmurhash3.cpp
  test_algorithm.cpp
  test_alignment.cpp
  test_array.cpp
  test_array_view.cpp
  test_array_wrapper.cpp
  test_binary.cpp
  test_bitset.cpp
  test_bloom_filter.cpp
  test_bsd_checksum.cpp
  test_callback_timer.cpp
  test_checksum.cpp
  test_compare.cpp
  test_compiler_settings.cpp
  test_constant.cpp
  test_container.cpp
  test_crc.cpp
  test_cyclic_value.cpp
  test_debounce.cpp
  test_deque.cpp
  test_endian.cpp
  test_enum_type.cpp
  test_error_handler.cpp
  test_exception.cpp
  test_fixed_iterator.cpp
  test_flat_map.cpp
  test_flat_multimap.cpp
  test_flat_multiset.cpp
  test_flat_set.cpp
  test_fnv_1.cpp
  test_forward_list.cpp
  test_fsm.cpp
  test_functional.cpp
  test_function.cpp
  test_hash.cpp
  test_hsm.cpp
  test_instance_count.cpp
  test_integral_limits.cpp
  test_intrusive_forward_list.cpp
  test_intrusive_links.cpp
  test_intrusive_list.cpp
  test_intrusive_queue.cpp
  test_intrusive_stack.cpp
  test_io_port.cpp
  test_iterator.cpp
  test_jenkins.cpp
  test_largest.cpp
  test_list.cpp
  test_map.cpp
  test_maths.cpp
  test_memory.cpp
  test_message_bus.cpp
  test_message_router.cpp
  test_message_timer.cpp
  test_multimap.cpp
  test_multiset.cpp
  test_murmur3.cpp
  test_numeric.cpp
  test_observer.cpp
  test_optional.cpp
  test_packet.cpp
  test_parameter_type.cpp
  test_pearson.cpp
  test_pool.cpp
  test_priority_queue.cpp
  test_queue.cpp
  test_random.cpp
  test_reference_flat_map.cpp
  test_reference_flat_multimap.cpp
  test_reference_flat_multiset.cpp
  test_reference_flat_set.cpp
  test_set.cpp
  test_smallest.cpp
  test_stack.cpp
  test_string_char.cpp
  test_string_u16.cpp
  test_string_u32.cpp
  test_string_wchar_t.cpp
  test_task_scheduler.cpp
  test_type_def.cpp
  test_type_lookup.cpp
  test_type_traits.cpp
  test_unordered_map.cpp
  test_unordered_multimap.cpp
  test_unordered_multiset.cpp
  test_unordered_set.cpp
  test_user_type.cpp
  test_utility.cpp
  test_variant.cpp
  test_variant_pool.cpp
  test_vector.cpp
  test_vector_non_trivial.cpp
  test_vector_pointer.cpp
  test_visitor.cpp
  test_xor_checksum.cpp
  test_xor_rotate_checksum.cpp
  test_atomic_std.cpp
  test_callback_service.cpp
  test_cumulative_moving_average.cpp
  test_delegate.cpp
  test_delegate_service.cpp
  test_forward_list_shared_pool.cpp
  test_list_shared_pool.cpp
  test_multi_array.cpp
  test_no_stl_algorithm.cpp
  test_no_stl_functional.cpp
  test_no_stl_iterator.cpp
  test_no_stl_limits.cpp
  test_no_stl_utility.cpp
  test_queue_memory_model_small.cpp
  test_queue_mpmc_mutex.cpp
  test_queue_mpmc_mutex_small.cpp
  test_queue_spsc_atomic.cpp
  test_queue_spsc_atomic_small.cpp
  test_queue_spsc_isr.cpp
  test_queue_spsc_isr_small.cpp
  test_queue_spsc_locked.cpp
  test_queue_spsc_locked_small.cpp
  test_scaled_rounding.cpp
  test_state_chart.cpp
  test_string_view.cpp
  test_to_string.cpp
  test_to_u16string.cpp
  test_to_u32string.cpp
  test_to_wstring.cpp
  test_type_select.cpp
  test_vector_external_buffer.cpp
  test_vector_pointer_external_buffer.cpp
  )



if (WIN32)
  # test_error_handler.cpp uses windows APIs that assume the microsoft project
  # is setup for unicode support.  Note: This may have adverse effects on client
  # projects and the tests should be updated to be independent of project setup
  ADD_DEFINITIONS(-DUNICODE)
  ADD_DEFINITIONS(-D_UNICODE)
endif()

if (CMAKE_CXX_COMPILER_ID MATCHES "GNU")
  list(APPEND TEST_SOURCE_FILES "test_atomic_gcc_sync.cpp")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fexceptions")
endif()
add_executable(etl_tests
  ${TEST_SOURCE_FILES}
  )
#target_link_libraries(etl_tests etl UnitTest++)
target_link_libraries(etl_tests UnitTest++)
target_include_directories(etl_tests
  PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}
  )

# Enable the 'make test' CMake target using the executable defined above
add_test(etl_unit_tests etl_tests)

# Since ctest will only show you the results of the single executable
# define a target that will output all of the failing or passing tests
# as they appear from UnitTest++
add_custom_target(test_verbose COMMAND ${CMAKE_CTEST_COMMAND} --verbose)


#RSG
set_property(TARGET etl_tests PROPERTY CXX_STANDARD 17)

