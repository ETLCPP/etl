cmake_minimum_required(VERSION 3.5.0)
project(etl_unit_tests)

add_definitions(-DETL_DEBUG)

option(ETL_NO_STL "No STL" OFF)

if (NO_STL OR ETL_NO_STL)
	message(STATUS "Compiling for No STL")
	add_definitions(-DETL_NO_STL)
else()
	message(STATUS "Compiling for STL")
endif()

if (ETL_USE_TYPE_TRAITS_BUILTINS)
	message(STATUS "Compiling for built-in type traits")
	add_definitions(-DETL_USE_TYPE_TRAITS_BUILTINS)
endif()

if (ETL_USE_MEM_BUILTINS)
	message(STATUS "Compiling for built-in memory functions")
	add_definitions(-DETL_USE_MEM_BUILTINS)
endif()

if (ETL_USER_DEFINED_TYPE_TRAITS)
	message(STATUS "Compiling for user defined type traits")
	add_definitions(-DETL_USER_DEFINED_TYPE_TRAITS)
endif()

if (ETL_FORCE_CONSTEXPR_ALGORITHMS)
	message(STATUS "Compiling with forced constexpr algorithms")
	add_definitions(-DETL_FORCE_CONSTEXPR_ALGORITHMS)
endif()

if (ETL_FORCE_TEST_CPP03)
	message(STATUS "Compiling for C++03 tests")
	add_definitions(-DETL_FORCE_TEST_CPP03)
endif()

if(NOT UnitTest++_FOUND)
  # Add unittest-cpp as an ExternalProject
  include(cmake/unit-test_external_project.cmake)
  add_unittest_cpp()
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
	test_atomic_clang_sync.cpp
	test_atomic_gcc_sync.cpp
	test_atomic_std.cpp
	test_binary.cpp
	test_bip_buffer_spsc_atomic.cpp
	test_bit.cpp
	test_bitset.cpp
	test_bit_stream.cpp
	test_byte.cpp
	test_byte_stream.cpp
	test_bloom_filter.cpp
	test_bresenham_line.cpp
	test_bsd_checksum.cpp
	test_buffer_descriptors.cpp
	test_callback_service.cpp
	test_callback_timer.cpp
	test_callback_timer_atomic.cpp
	test_callback_timer_interrupt.cpp
	test_callback_timer_locked.cpp
	test_checksum.cpp
	test_circular_buffer.cpp
	test_circular_buffer_external_buffer.cpp
	test_compare.cpp
	test_compiler_settings.cpp
	test_constant.cpp
	test_container.cpp
	test_correlation.cpp
	test_covariance.cpp
	test_crc.cpp
	test_crc16.cpp
	test_crc16_a.cpp
	test_crc16_arc.cpp
	test_crc16_aug_ccitt.cpp
	test_crc16_buypass.cpp
	test_crc16_ccitt.cpp
	test_crc16_cdma2000.cpp
	test_crc16_dds110.cpp
	test_crc16_dectr.cpp
	test_crc16_dectx.cpp
	test_crc16_dnp.cpp
	test_crc16_en13757.cpp
	test_crc16_genibus.cpp
	test_crc16_kermit.cpp
	test_crc16_maxim.cpp
	test_crc16_mcrf4xx.cpp
	test_crc16_modbus.cpp
	test_crc16_profibus.cpp
	test_crc16_riello.cpp
	test_crc16_t10dif.cpp
	test_crc16_teledisk.cpp
	test_crc16_tms37157.cpp
	test_crc16_usb.cpp
	test_crc16_x25.cpp
	test_crc16_xmodem.cpp
	test_crc32.cpp
	test_crc32_bzip2.cpp
	test_crc32_c.cpp
	test_crc32_d.cpp
	test_crc32_jamcrc.cpp
	test_crc32_mpeg2.cpp
	test_crc32_posix.cpp
	test_crc32_q.cpp
	test_crc32_xfer.cpp
	test_crc64_ecma.cpp
	test_crc8_ccitt.cpp
	test_crc8_cdma2000.cpp
	test_crc8_darc.cpp
	test_crc8_dvbs2.cpp
	test_crc8_ebu.cpp
	test_crc8_icode.cpp
	test_crc8_itu.cpp
	test_crc8_maxim.cpp
	test_crc8_rohc.cpp
	test_crc8_wcdma.cpp
	test_cumulative_moving_average.cpp
	test_cyclic_value.cpp
	test_debounce.cpp
	test_delegate.cpp
	test_delegate_cpp03.cpp
	test_delegate_service.cpp
	test_delegate_service_compile_time.cpp
	test_deque.cpp
	test_endian.cpp
	test_enum_type.cpp
	test_error_handler.cpp
	test_exception.cpp
	test_fixed_iterator.cpp
	test_fixed_sized_memory_block_allocator.cpp
	test_flags.cpp
	test_flat_map.cpp
	test_flat_multimap.cpp
	test_flat_multiset.cpp
	test_flat_set.cpp
	test_fnv_1.cpp
	test_format_spec.cpp
	test_forward_list.cpp
	test_forward_list_shared_pool.cpp
	test_fsm.cpp
	test_function.cpp
	test_functional.cpp
	test_gamma.cpp
	test_hash.cpp
	test_hfsm.cpp
	test_histogram.cpp
	test_indirect_vector.cpp
	test_indirect_vector_external_buffer.cpp
	test_instance_count.cpp
	test_integral_limits.cpp
	test_intrusive_forward_list.cpp
	test_intrusive_links.cpp
	test_intrusive_list.cpp
	test_intrusive_queue.cpp
	test_intrusive_stack.cpp
	test_invert.cpp
	test_io_port.cpp
	test_iterator.cpp
	test_jenkins.cpp
	test_largest.cpp
	test_limiter.cpp
	test_limits.cpp
	test_list.cpp
	test_list_shared_pool.cpp
	test_make_string.cpp
	test_map.cpp
	test_maths.cpp
	test_mean.cpp
	test_mem_cast.cpp
	test_mem_cast_ptr.cpp
	test_memory.cpp
	test_message_bus.cpp
	test_message_packet.cpp
	test_message_router.cpp
	test_message_router_registry.cpp
	test_message_timer.cpp
	test_message_timer_atomic.cpp
    test_message_timer_interrupt.cpp
	test_message_timer_locked.cpp
	test_multimap.cpp
	test_multiset.cpp
	test_multi_array.cpp
	test_multi_range.cpp
	test_multi_vector.cpp
	test_murmur3.cpp
	test_numeric.cpp
	test_observer.cpp
	test_optional.cpp
	test_packet.cpp
	test_parameter_pack.cpp
	test_parameter_type.cpp
	test_parity_checksum.cpp
	test_pearson.cpp
	test_pool.cpp
	test_pool_external_buffer.cpp
	test_priority_queue.cpp
	test_quantize.cpp
	test_queue.cpp
	test_queue_lockable.cpp
	test_queue_lockable_small.cpp
	test_queue_memory_model_small.cpp
	test_queue_mpmc_mutex.cpp
	test_queue_mpmc_mutex_small.cpp
	test_queue_spsc_atomic.cpp
	test_queue_spsc_atomic_small.cpp
	test_queue_spsc_isr.cpp
	test_queue_spsc_isr_small.cpp
	test_queue_spsc_locked.cpp
	test_queue_spsc_locked_small.cpp
	test_random.cpp
	test_reference_flat_map.cpp
	test_reference_flat_multimap.cpp
	test_reference_flat_multiset.cpp
	test_reference_flat_set.cpp
	test_rescale.cpp
	test_rms.cpp
	test_scaled_rounding.cpp
	test_set.cpp
	test_shared_message.cpp
	test_singleton.cpp
	test_smallest.cpp
	test_span.cpp
	test_stack.cpp
	test_standard_deviation.cpp
	test_state_chart.cpp
	test_state_chart_with_data_parameter.cpp
	test_state_chart_with_rvalue_data_parameter.cpp
	test_state_chart_compile_time.cpp
	test_state_chart_compile_time_with_data_parameter.cpp
	test_string_char.cpp
	test_string_char_external_buffer.cpp
	test_string_stream.cpp
	test_string_stream_u16.cpp
	test_string_stream_u32.cpp
	test_string_stream_wchar_t.cpp
	test_string_u16.cpp
	test_string_u16_external_buffer.cpp
	test_string_u32.cpp
	test_string_u32_external_buffer.cpp
	test_string_utilities.cpp
	test_string_utilities_std.cpp
	test_string_utilities_std_u16.cpp
	test_string_utilities_std_u32.cpp
	test_string_utilities_std_wchar_t.cpp
	test_string_utilities_u16.cpp
	test_string_utilities_u32.cpp
	test_string_utilities_wchar_t.cpp
	test_string_view.cpp
	test_string_wchar_t.cpp
	test_string_wchar_t_external_buffer.cpp
	test_task_scheduler.cpp
	test_threshold.cpp
	test_to_string.cpp
	test_to_u16string.cpp
	test_to_u32string.cpp
	test_to_wstring.cpp
	test_type_def.cpp
	test_type_lookup.cpp
	test_type_select.cpp
	test_type_traits.cpp
	test_unaligned_type.cpp
	test_unordered_map.cpp
	test_unordered_multimap.cpp
	test_unordered_multiset.cpp
	test_unordered_set.cpp
	test_user_type.cpp
	test_utility.cpp
	test_variance.cpp
	test_variant_legacy.cpp
	test_variant_variadic.cpp
	test_variant_pool.cpp
	test_variant_pool_external_buffer.cpp
	test_vector.cpp
	test_vector_external_buffer.cpp
	test_vector_non_trivial.cpp
	test_vector_pointer.cpp
	test_vector_pointer_external_buffer.cpp
	test_visitor.cpp
	test_xor_checksum.cpp
	test_xor_rotate_checksum.cpp
  )

if (CMAKE_CXX_COMPILER_ID MATCHES "GNU")
  list(APPEND TEST_SOURCE_FILES "test_atomic_gcc_sync.cpp")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fexceptions")
endif()
add_executable(etl_tests
  ${TEST_SOURCE_FILES}
  )

if(UNIX AND NOT APPLE)
  # atomic is need on Linux with Clang
  set(THREADS_PREFER_PTHREAD_FLAG ON)
  find_package(Threads REQUIRED)
  target_link_libraries(etl_tests UnitTest++ atomic Threads::Threads)
elseif(NOT UNIX AND APPLE)
  set(THREADS_PREFER_PTHREAD_FLAG ON)
  find_package(Threads REQUIRED)
  target_link_libraries(etl_tests UnitTest++)
else()
  target_link_libraries(etl_tests UnitTest++)
endif()

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

