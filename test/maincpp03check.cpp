/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://www.etlcpp.com
https://github.com/ETLCPP/etl

Copyright(c) 2019 jwellbelove

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files(the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and / or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions :

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
******************************************************************************/

#include "UnitTest++/UnitTest++.h"

#include "etl/absolute.h"
#include "etl/algorithm.h"
#include "etl/alignment.h"
#include "etl/array.h"
#include "etl/array_view.h"
#include "etl/array_wrapper.h"
#include "etl/atomic.h"
#include "etl/basic_string.h"
#include "etl/binary.h"
#include "etl/bitset.h"
#include "etl/bit_stream.h"
#include "etl/bloom_filter.h"
#include "etl/callback.h"
#include "etl/callback_service.h"
#include "etl/callback_timer.h"
#include "etl/char_traits.h"
#include "etl/checksum.h"
#include "etl/combinations.h"
#include "etl/compare.h"
#include "etl/constant.h"
#include "etl/container.h"
#include "etl/crc16.h"
#include "etl/crc16_ccitt.h"
#include "etl/crc16_kermit.h"
#include "etl/crc16_modbus.h"
#include "etl/crc32.h"
#include "etl/crc32_c.h"
#include "etl/crc64_ecma.h"
#include "etl/crc8_ccitt.h"
#include "etl/cstring.h"
#include "etl/cumulative_moving_average.h"
#include "etl/cyclic_value.h"
#include "etl/debounce.h"
#include "etl/debug_count.h"
#include "etl/deque.h"
#include "etl/endianness.h"
#include "etl/enum_type.h"
#include "etl/error_handler.h"
#include "etl/exception.h"
#include "etl/factorial.h"
#include "etl/fibonacci.h"
#include "etl/fixed_iterator.h"
#include "etl/flat_map.h"
#include "etl/flat_multimap.h"
#include "etl/flat_multiset.h"
#include "etl/flat_set.h"
#include "etl/fnv_1.h"
#include "etl/forward_list.h"
#include "etl/frame_check_sequence.h"
#include "etl/fsm.h"
#include "etl/function.h"
#include "etl/functional.h"
#include "etl/hash.h"
#include "etl/ihash.h"
#include "etl/instance_count.h"
#include "etl/integral_limits.h"
#include "etl/intrusive_forward_list.h"
#include "etl/intrusive_links.h"
#include "etl/intrusive_list.h"
#include "etl/intrusive_queue.h"
#include "etl/intrusive_stack.h"
#include "etl/io_port.h"
#include "etl/iterator.h"
#include "etl/jenkins.h"
#include "etl/largest.h"
#include "etl/list.h"
#include "etl/log.h"
#include "etl/macros.h"
#include "etl/map.h"
#include "etl/math_constants.h"
#include "etl/memory.h"
#include "etl/memory_model.h"
#include "etl/message.h"
#include "etl/message_bus.h"
#include "etl/message_router.h"
#include "etl/message_timer.h"
#include "etl/message_types.h"
#include "etl/multimap.h"
#include "etl/multiset.h"
#include "etl/murmur3.h"
#include "etl/mutex.h"
#include "etl/nullptr.h"
#include "etl/null_type.h"
#include "etl/numeric.h"
#include "etl/observer.h"
#include "etl/optional.h"
#include "etl/packet.h"
#include "etl/parameter_type.h"
#include "etl/pearson.h"
#include "etl/permutations.h"
#include "etl/platform.h"
#include "etl/pool.h"
#include "etl/power.h"
#include "etl/priority_queue.h"
#include "etl/queue.h"
#include "etl/queue_mpmc_mutex.h"
#include "etl/queue_spsc_atomic.h"
#include "etl/queue_spsc_isr.h"
#include "etl/radix.h"
#include "etl/random.h"
#include "etl/ratio.h"
#include "etl/reference_flat_map.h"
#include "etl/reference_flat_multimap.h"
#include "etl/reference_flat_multiset.h"
#include "etl/reference_flat_set.h"
#include "etl/scaled_rounding.h"
#include "etl/scheduler.h"
#include "etl/set.h"
#include "etl/smallest.h"
#include "etl/sqrt.h"
#include "etl/stack.h"
#include "etl/state_chart.h"
#include "etl/static_assert.h"
#include "etl/string_view.h"
#include "etl/task.h"
#include "etl/timer.h"
#include "etl/type_def.h"
#include "etl/type_lookup.h"
#include "etl/type_select.h"
#include "etl/type_traits.h"
#include "etl/u16string.h"
#include "etl/u32string.h"
#include "etl/unordered_map.h"
#include "etl/unordered_multimap.h"
#include "etl/unordered_multiset.h"
#include "etl/unordered_set.h"
#include "etl/user_type.h"
#include "etl/utility.h"
#include "etl/variant.h"
#include "etl/variant_pool.h"
#include "etl/vector.h"
#include "etl/version.h"
#include "etl/visitor.h"
#include "etl/wstring.h"

int main()
{
  return UnitTest::RunAllTests();
}
