cmake_minimum_required(VERSION 3.5.0)
project(etl_unit_tests)

# Bring in the UnitTest++ include directories and libraries
find_package(UnitTest++)

if(NOT UnitTest++_FOUND)
  # Add unittest-cpp as an ExternalProject  
  include(cmake/unit-test_external_project.cmake)
  add_unittest_cpp()
endif()

include_directories(${UTPP_INCLUDE_DIRS})

add_executable(etl_tests 
  main.cpp
  murmurhash3.cpp
  test_algorithm.cpp
  test_alignment.cpp
  test_array.cpp
  test_array_view.cpp
  test_array_wrapper.cpp
  test_atomic_gcc.cpp
  test_binary.cpp
  test_bitset.cpp
  test_bloom_filter.cpp
  test_bsd_checksum.cpp
  test_callback_timer.cpp
  test_checksum.cpp
  test_compare.cpp
  test_constant.cpp
  test_container.cpp
  test_crc.cpp
  test_c_timer_framework.cpp
  test_cyclic_value.cpp
  test_debounce.cpp
  test_deque.cpp
  #test_embedded_compile.cpp
  test_endian.cpp
  test_enum_type.cpp
  test_error_handler.cpp
  test_exception.cpp
  test_factory.cpp
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

  ${CMAKE_SOURCE_DIR}/src/c/ecl_timer.c
  )
target_link_libraries(etl_tests etl UnitTest++)
target_include_directories(etl_tests
  PUBLIC 
  ${CMAKE_CURRENT_LIST_DIR} 
  )

# Enable the 'make test' CMake target using the executable defined above
add_test(etl_unit_tests etl_tests)
