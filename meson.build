project('PROJECT_NAME',
	['cpp'],
	default_options : [
		'warning_level=2',
		'werror=false',
		# `build.*` options affect `native: true targets`
		# plain options affect `native: false` targets.
		'cpp_std=c++17', 'build.cpp_std=c++17',
	],
	meson_version: '>=0.54.0',
	version: '18.0'
)

######################
# Library Dependency #
######################

etl_dep = declare_dependency(
	include_directories: include_directories('include', is_system: true),
)

#########
# Tests #
#########

# Disabled 'test/tests below fail to compile in our configuration, often due to iostream use
etl_test_sources = files(
	'test/main.cpp',
	'test/murmurhash3.cpp',
	'test/test_algorithm.cpp',
	'test/test_alignment.cpp',
	'test/test_array.cpp',
	'test/test_array_view.cpp',
	'test/test_array_wrapper.cpp',
	'test/test_binary.cpp',
	'test/test_bitset.cpp',
	'test/test_bloom_filter.cpp',
	'test/test_bsd_checksum.cpp',
	'test/test_callback_timer.cpp',
	'test/test_checksum.cpp',
	'test/test_compare.cpp',
	'test/test_compiler_settings.cpp',
	'test/test_constant.cpp',
	'test/test_container.cpp',
	'test/test_crc.cpp',
	'test/test_cyclic_value.cpp',
	'test/test_debounce.cpp',
	'test/test_endian.cpp',
	'test/test_enum_type.cpp',
	'test/test_error_handler.cpp',
	'test/test_exception.cpp',
	'test/test_fixed_iterator.cpp',
	'test/test_fnv_1.cpp',
	'test/test_forward_list.cpp',
	'test/test_fsm.cpp',
	'test/test_functional.cpp',
	'test/test_function.cpp',
	'test/test_hash.cpp',
	'test/test_instance_count.cpp',
	'test/test_integral_limits.cpp',
	'test/test_intrusive_forward_list.cpp',
	'test/test_intrusive_links.cpp',
	'test/test_intrusive_list.cpp',
	'test/test_intrusive_queue.cpp',
	'test/test_intrusive_stack.cpp',
	'test/test_io_port.cpp',
	'test/test_iterator.cpp',
	'test/test_jenkins.cpp',
	'test/test_largest.cpp',
	'test/test_list.cpp',
	'test/test_maths.cpp',
	'test/test_memory.cpp',
	'test/test_message_bus.cpp',
	'test/test_message_router.cpp',
	'test/test_message_timer.cpp',
	'test/test_murmur3.cpp',
	'test/test_numeric.cpp',
	'test/test_observer.cpp',
	'test/test_optional.cpp',
	'test/test_packet.cpp',
	'test/test_parameter_type.cpp',
	'test/test_parity_checksum.cpp',
	'test/test_pool.cpp',
	'test/test_priority_queue.cpp',
	'test/test_queue.cpp',
	'test/test_queue_memory_model_small.cpp',
	'test/test_queue_mpmc_mutex.cpp',
	'test/test_queue_mpmc_mutex_small.cpp',
	'test/test_queue_spsc_isr.cpp',
	'test/test_queue_spsc_isr_small.cpp',
	'test/test_queue_spsc_atomic.cpp',
	'test/test_queue_spsc_atomic_small.cpp',
	'test/test_queue_spsc_isr.cpp',
	'test/test_queue_spsc_isr_small.cpp',
	'test/test_queue_spsc_locked.cpp',
	'test/test_queue_spsc_locked_small.cpp',
	'test/test_random.cpp',
	'test/test_smallest.cpp',
	'test/test_stack.cpp',
	'test/test_string_char.cpp',
	'test/test_string_u16.cpp',
	'test/test_string_u32.cpp',
	'test/test_string_wchar_t.cpp',
	'test/test_task_scheduler.cpp',
	'test/test_type_def.cpp',
	'test/test_type_lookup.cpp',
	'test/test_type_traits.cpp',
	'test/test_user_type.cpp',
	'test/test_utility.cpp',
	'test/test_variant.cpp',
	'test/test_variant_pool.cpp',
	'test/test_vector.cpp',
	'test/test_vector_non_trivial.cpp',
	'test/test_vector_pointer.cpp',
	'test/test_visitor.cpp',
	'test/test_xor_checksum.cpp',
	'test/test_xor_rotate_checksum.cpp',
	'test/test_atomic_std.cpp',
	'test/test_callback_service.cpp',
	'test/test_cumulative_moving_average.cpp',
	'test/test_delegate.cpp',
	'test/test_delegate_service.cpp',
	'test/test_forward_list_shared_pool.cpp',
	'test/test_list_shared_pool.cpp',
	'test/test_multi_array.cpp',
	'test/test_queue_memory_model_small.cpp',
	'test/test_queue_mpmc_mutex.cpp',
	'test/test_queue_mpmc_mutex_small.cpp',
	'test/test_queue_spsc_atomic.cpp',
	'test/test_queue_spsc_atomic_small.cpp',
	'test/test_queue_spsc_isr.cpp',
	'test/test_queue_spsc_isr_small.cpp',
	'test/test_queue_spsc_locked.cpp',
	'test/test_queue_spsc_locked_small.cpp',
	'test/test_scaled_rounding.cpp',
	'test/test_state_chart.cpp',
	'test/test_type_select.cpp',
	'test/test_vector_external_buffer.cpp',
	'test/test_vector_pointer_external_buffer.cpp',
)

# These files currently fail to compile with OS X clang at the very least
if meson.get_compiler('cpp').get_id() == 'clang' and build_machine.system() == 'darwin'
	message('Some test files fail to compile with OS X clang and are disabled.')
else
	etl_test_sources += files(
		'test/test_deque.cpp',
		'test/test_flat_map.cpp',
		'test/test_flat_multimap.cpp',
		'test/test_flat_multiset.cpp',
		'test/test_flat_set.cpp',
		'test/test_map.cpp',
		'test/test_multimap.cpp',
		'test/test_multiset.cpp',
		'test/test_reference_flat_map.cpp',
		'test/test_reference_flat_multimap.cpp',
		'test/test_reference_flat_multiset.cpp',
		'test/test_reference_flat_set.cpp',
		'test/test_pearson.cpp',
		'test/test_set.cpp',
		'test/test_string_view.cpp',
		'test/test_to_u16string.cpp',
		'test/test_to_u32string.cpp',
		'test/test_to_string.cpp',
		'test/test_to_wstring.cpp',
		'test/test_unordered_map.cpp',
		'test/test_unordered_multimap.cpp',
		'test/test_unordered_multiset.cpp',
		'test/test_unordered_set.cpp',
	)
endif

if meson.get_compiler('cpp').get_id() == 'gcc'
	etl_test_sources += files('test/test_atomic_gcc_sync.cpp')
endif

unittestcpp_dep = dependency('UnitTest++',
	native: true,
	required: false,
	not_found_message: 'UnitTest++ not found, ETL unit tests will be disabled',
	disabler: true,
)

etl_unit_tests = executable('etl_unit_tests',
	include_directories: [
		include_directories('test'),
		# Included here instead of with the dependency so we can see
		# header-induced warnings when building the tests
		include_directories('include')
	],
	sources: etl_test_sources,
	dependencies: unittestcpp_dep,
	cpp_args: [
		'-fexceptions',
		'-DENABLE_ETL_UNIT_TESTS',
		'-DETL_DEBUG',
	],
	native: true,
	install: false,
	# Don't build tests by default if we are a subproject
	build_by_default: meson.is_subproject() == false
)

# Only register tests with the test runner when built as a primary project
if meson.is_subproject() == false
	test('ETL Unit Tests', etl_unit_tests)
endif
