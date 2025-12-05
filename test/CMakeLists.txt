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
	murmurhash3.cpp
	test_algorithm.cpp
	test_alignment.cpp
	test_array.cpp
	test_array_view.cpp
	test_array_wrapper.cpp
	test_atomic.cpp
	test_base64_RFC2152_decoder.cpp
	test_base64_RFC2152_encoder.cpp
	test_base64_RFC3501_decoder.cpp
	test_base64_RFC3501_encoder.cpp
	test_base64_RFC4648_decoder_with_no_padding.cpp
	test_base64_RFC4648_decoder_with_padding.cpp
	test_base64_RFC4648_encoder_with_no_padding.cpp
	test_base64_RFC4648_encoder_with_padding.cpp
	test_base64_RFC4648_URL_decoder_with_no_padding.cpp
	test_base64_RFC4648_URL_decoder_with_padding.cpp
	test_base64_RFC4648_URL_encoder_with_no_padding.cpp
	test_base64_RFC4648_URL_encoder_with_padding.cpp
	test_binary.cpp
	test_bip_buffer_spsc_atomic.cpp
	test_bit.cpp
	test_bitset_legacy.cpp
	test_bitset_new_comparisons.cpp
	test_bitset_new_default_element_type.cpp
	test_bitset_new_explicit_single_element_type.cpp
	test_bitset_new_ext_default_element_type.cpp
	test_bitset_new_ext_explicit_single_element_type.cpp
	test_bit_stream.cpp
	test_bit_stream_reader_big_endian.cpp
	test_bit_stream_reader_little_endian.cpp
	test_bit_stream_writer_big_endian.cpp
	test_bit_stream_writer_little_endian.cpp
	test_bloom_filter.cpp
	test_bresenham_line.cpp
	test_bsd_checksum.cpp
	test_buffer_descriptors.cpp
	test_byte.cpp
	test_byte_stream.cpp
	test_callback_service.cpp
	test_callback_timer.cpp
	test_callback_timer_atomic.cpp
	test_callback_timer_deferred_locked.cpp
	test_callback_timer_interrupt.cpp
	test_callback_timer_locked.cpp
	test_char_traits.cpp
	test_checksum.cpp
	test_chrono_clocks.cpp
	test_chrono_day.cpp
	test_chrono_duration.cpp
	test_chrono_hh_mm_ss.cpp
	test_chrono_literals.cpp
	test_chrono_month.cpp
	test_chrono_month_day.cpp
	test_chrono_month_day_last.cpp
	test_chrono_month_weekday.cpp
	test_chrono_month_weekday_last.cpp
	test_chrono_operators.cpp
	test_chrono_time_point.cpp
	test_chrono_weekday.cpp
	test_chrono_weekday_indexed.cpp
	test_chrono_weekday_last.cpp
	test_chrono_year.cpp
	test_chrono_year_month.cpp
	test_chrono_year_month_day.cpp
	test_chrono_year_month_day_last.cpp
	test_chrono_year_month_weekday.cpp
	test_chrono_year_month_weekday_last.cpp
	test_circular_buffer.cpp
	test_circular_buffer_external_buffer.cpp
	test_circular_iterator.cpp
	test_closure.cpp
	test_closure_constexpr.cpp
	test_compare.cpp
	test_constant.cpp
	test_const_map.cpp
	test_const_map_constexpr.cpp
	test_const_map_ext.cpp
	test_const_map_ext_constexpr.cpp
	test_const_multimap.cpp
	test_const_multimap_constexpr.cpp
	test_const_multimap_ext.cpp
	test_const_multimap_ext_constexpr.cpp
	test_const_multiset.cpp
	test_const_multiset_constexpr.cpp
	test_const_multiset_ext.cpp
	test_const_multiset_ext_constexpr.cpp
	test_const_set.cpp
	test_const_set_constexpr.cpp
	test_const_set_ext.cpp
	test_const_set_ext_constexpr.cpp
	test_container.cpp
	test_correlation.cpp
	test_covariance.cpp
	test_crc1.cpp
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
	test_crc16_m17.cpp
	test_crc16_maxim.cpp
	test_crc16_mcrf4xx.cpp
	test_crc16_modbus.cpp
	test_crc16_opensafety_a.cpp
	test_crc16_opensafety_b.cpp
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
	test_crc64_iso.cpp
	test_crc8_ccitt.cpp
	test_crc8_cdma2000.cpp
	test_crc8_darc.cpp
	test_crc8_dvbs2.cpp
	test_crc8_ebu.cpp
	test_crc8_icode.cpp
	test_crc8_itu.cpp
	test_crc8_j1850.cpp
	test_crc8_j1850_zero.cpp
	test_crc8_maxim.cpp
	test_crc8_nrsc5.cpp
	test_crc8_opensafety.cpp
	test_crc8_rohc.cpp
	test_crc8_wcdma.cpp
	test_cyclic_value.cpp
	test_debounce.cpp
	test_delegate.cpp
	test_delegate_cpp03.cpp
	test_delegate_observable.cpp
	test_delegate_service.cpp
	test_delegate_service_compile_time.cpp
	test_delegate_service_cpp03.cpp
	test_deque.cpp
	test_endian.cpp
	test_enum_type.cpp
	test_error_handler.cpp
	test_etl_traits.cpp
	test_exception.cpp
	test_expected.cpp
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
	test_function_traits.cpp
	test_gamma.cpp
	test_hash.cpp
	test_hfsm.cpp
	test_hfsm_recurse_to_inner_state_on_start.cpp
	test_hfsm_transition_on_enter.cpp
	test_histogram.cpp
	test_index_of_type.cpp
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
	test_invoke.cpp
	test_jenkins.cpp
	test_largest.cpp
	test_limiter.cpp
	test_limits.cpp
	test_list.cpp
	test_list_shared_pool.cpp
	test_macros.cpp
	test_make_string.cpp
	test_map.cpp
	test_math.cpp
	test_math_functions.cpp
	test_mean.cpp
	test_memory.cpp
	test_mem_cast.cpp
	test_mem_cast_ptr.cpp
	test_message.cpp
	test_message_broker.cpp
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
	test_multi_span.cpp
	test_multi_vector.cpp
	test_murmur3.cpp
	test_not_null_pointer.cpp
	test_not_null_pointer_constexpr.cpp
	test_not_null_unique_pointer.cpp
	test_nth_type.cpp
	test_numeric.cpp
	test_observer.cpp
	test_optional.cpp
	test_overload.cpp
	test_packet.cpp
	test_parameter_pack.cpp
	test_parameter_type.cpp
	test_parity_checksum.cpp
	test_pearson.cpp
	test_poly_span_dynamic_extent.cpp
	test_poly_span_fixed_extent.cpp
	test_pool.cpp
	test_pool_external_buffer.cpp
	test_priority_queue.cpp
	test_pseudo_moving_average.cpp
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
	test_ratio.cpp
	test_reference_flat_map.cpp
	test_reference_flat_multimap.cpp
	test_reference_flat_multiset.cpp
	test_reference_flat_set.cpp
	test_rescale.cpp
	test_result.cpp
	test_rms.cpp
	test_rounded_integral_division.cpp
	test_scaled_rounding.cpp
	test_set.cpp
	test_shared_message.cpp
	test_signal.cpp
	test_singleton.cpp
	test_singleton_base.cpp
	test_smallest.cpp
	test_span_dynamic_extent.cpp
	test_span_fixed_extent.cpp
	test_stack.cpp
	test_standard_deviation.cpp
	test_state_chart.cpp
	test_state_chart_compile_time.cpp
	test_state_chart_compile_time_with_data_parameter.cpp
	test_state_chart_with_data_parameter.cpp
	test_state_chart_with_rvalue_data_parameter.cpp
	test_string_char.cpp
	test_string_char_external_buffer.cpp
	test_string_stream.cpp
	test_string_stream_u16.cpp
	test_string_stream_u32.cpp
	test_string_stream_u8.cpp
	test_string_stream_wchar_t.cpp
	test_string_u16.cpp
	test_string_u16_external_buffer.cpp
	test_string_u32.cpp
	test_string_u32_external_buffer.cpp
	test_string_u8.cpp
	test_string_u8_external_buffer.cpp
	test_string_utilities.cpp
	test_string_utilities_std.cpp
	test_string_utilities_std_u16.cpp
	test_string_utilities_std_u32.cpp
	test_string_utilities_std_u8.cpp
	test_string_utilities_std_wchar_t.cpp
	test_string_utilities_u16.cpp
	test_string_utilities_u32.cpp
	test_string_utilities_u8.cpp
	test_string_utilities_wchar_t.cpp
	test_string_view.cpp
	test_string_wchar_t.cpp
	test_string_wchar_t_external_buffer.cpp
	test_successor.cpp
	test_task_scheduler.cpp
	test_threshold.cpp
	test_to_arithmetic.cpp
	test_to_arithmetic_u16.cpp
	test_to_arithmetic_u32.cpp
	test_to_arithmetic_u8.cpp
	test_to_arithmetic_wchar_t.cpp
	test_to_string.cpp
	test_to_u16string.cpp
	test_to_u32string.cpp
	test_to_u8string.cpp
	test_to_wstring.cpp
	test_tuple.cpp
	test_type_def.cpp
	test_type_list.cpp
	test_type_lookup.cpp
	test_type_select.cpp
	test_type_traits.cpp
	test_unaligned_type.cpp
	test_unaligned_type_ext.cpp
	test_uncopyable.cpp
	test_unordered_map.cpp
	test_unordered_multimap.cpp
	test_unordered_multiset.cpp
	test_unordered_set.cpp
	test_user_type.cpp
	test_utility.cpp
	test_variance.cpp
	test_variant_legacy.cpp
	test_variant_pool.cpp
	test_variant_pool_external_buffer.cpp
	test_variant_variadic.cpp
	test_vector.cpp
	test_vector_external_buffer.cpp
	test_vector_non_trivial.cpp
	test_vector_pointer.cpp
	test_vector_pointer_external_buffer.cpp
	test_visitor.cpp
	test_xor_checksum.cpp
	test_xor_rotate_checksum.cpp
  )

target_compile_definitions(etl_tests PRIVATE -DETL_DEBUG)

option(ETL_NO_STL "No STL" OFF)

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
	message(STATUS "Compiling for C++23")
	set_property(TARGET etl_tests PROPERTY CXX_STANDARD 23)
else()
	message(STATUS "Unsupported C++ standard")
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
			-Wall
			-Wextra
			-Werror
			-Wfloat-equal
			-Wshadow
			-Wnull-dereference
			-g
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
			-g
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

target_include_directories(etl_tests
		PRIVATE
		${PROJECT_SOURCE_DIR}/../include)

add_subdirectory(UnitTest++)
target_link_libraries(etl_tests PRIVATE UnitTestpp)

# Enable the 'make test' CMake target using the executable defined above
add_test(etl_unit_tests etl_tests)

# Since ctest will only show you the results of the single executable
# define a target that will output all of the failing or passing tests
# as they appear from UnitTest++
add_custom_target(test_verbose COMMAND ${CMAKE_CTEST_COMMAND} --verbose)

