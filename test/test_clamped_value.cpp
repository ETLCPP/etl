///\file

/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl
https://www.etlcpp.com

Copyright(c) 2026 John Wellbelove

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

#include "unit_test_framework.h"

#include "etl/clamped_value.h"

#include <limits.h>
#include <stdint.h>

namespace
{
  SUITE(test_clamped_value)
  {
    //*************************************************************************
    TEST(test_compile_time_initialisation)
    {
      etl::clamped_value<int, 2, 7>       value;
      const etl::clamped_value<int, 2, 7> const_value(value);
      int                                 converted_value       = value;
      int                                 converted_const_value = const_value;
      CHECK_EQUAL(2, value.get());
      CHECK_EQUAL(2, value.min());
      CHECK_EQUAL(7, value.max());
      CHECK_EQUAL(2, converted_value);
      CHECK_EQUAL(2, converted_const_value);
    }

    //*************************************************************************
    TEST(test_compile_time_bounds_accessors_are_const)
    {
      const etl::clamped_value<int, 2, 7> value(4);
      CHECK_EQUAL(2, value.min());
      CHECK_EQUAL(7, value.max());
    }

    //*************************************************************************
    TEST(test_run_time_initialisation)
    {
      etl::clamped_value<int>       value(2, 7, 5);
      etl::clamped_value<int>       default_value;
      etl::clamped_value<int>       range_value(2, 7);
      const etl::clamped_value<int> const_value(value);
      int                           converted_value       = value;
      int                           converted_const_value = const_value;
      CHECK_EQUAL(5, value.get());
      CHECK_EQUAL(2, value.min());
      CHECK_EQUAL(7, value.max());
      CHECK_EQUAL(etl::numeric_limits<int>::lowest(), default_value.min());
      CHECK_EQUAL(etl::numeric_limits<int>::max(), default_value.max());
      CHECK_EQUAL(etl::numeric_limits<int>::lowest(), default_value.get());
      CHECK_EQUAL(2, range_value.get());
      CHECK_EQUAL(5, converted_value);
      CHECK_EQUAL(5, converted_const_value);
    }

    //*************************************************************************
    TEST(test_set_and_assignment_clamp_values)
    {
      etl::clamped_value<int, 2, 7> compile_time(1);
      etl::clamped_value<int>       run_time(2, 7, 8);
      CHECK_EQUAL(2, compile_time.get());
      CHECK_EQUAL(7, run_time.get());
      compile_time = 8;
      run_time.set(1);
      CHECK_EQUAL(7, compile_time.get());
      CHECK_EQUAL(2, run_time.get());
    }

    //*************************************************************************
    TEST(test_increment_and_decrement_saturate)
    {
      etl::clamped_value<int, 2, 7> compile_time(7);
      etl::clamped_value<int>       run_time(2, 7, 2);
      ++compile_time;
      --run_time;
      CHECK_EQUAL(7, compile_time.get());
      CHECK_EQUAL(2, run_time.get());
      compile_time.to_min();
      run_time.to_max();
      --compile_time;
      ++run_time;
      CHECK_EQUAL(2, compile_time.get());
      CHECK_EQUAL(7, run_time.get());
    }

    //*************************************************************************
    TEST(test_postfix_operators)
    {
      etl::clamped_value<int, 2, 7> value(4);
      etl::clamped_value<int, 2, 7> previous_increment = value++;
      CHECK_EQUAL(4, previous_increment.get());
      CHECK_EQUAL(5, value.get());
      etl::clamped_value<int, 2, 7> previous_decrement = value--;
      CHECK_EQUAL(5, previous_decrement.get());
      CHECK_EQUAL(4, value.get());
    }

    //*************************************************************************
    TEST(test_advance_saturates)
    {
      etl::clamped_value<int, 2, 7> value;
      value.advance(100);
      CHECK_EQUAL(7, value.get());
      value.advance(-100);
      CHECK_EQUAL(2, value.get());
    }

    //*************************************************************************
    TEST(test_set_run_time_bounds)
    {
      etl::clamped_value<int> value(2, 7, 6);
      value.set(3, 8);
      CHECK_EQUAL(3, value.get());
      CHECK_EQUAL(3, value.min());
      CHECK_EQUAL(8, value.max());
    }

    //*************************************************************************
    TEST(test_copy_equality_and_swap_compile_time)
    {
      etl::clamped_value<int, 2, 7> value1(4);
      etl::clamped_value<int, 2, 7> value2(value1);
      CHECK(value1 == value2);
      value2 = 6;
      CHECK(value1 != value2);
      swap(value1, value2);
      CHECK_EQUAL(6, value1.get());
      CHECK_EQUAL(4, value2.get());
    }

    //*************************************************************************
    TEST(test_compile_time_instances_with_different_bounds)
    {
      etl::clamped_value<int, 2, 7> value1(3);
      etl::clamped_value<int, 2, 8> value2(3);
      CHECK(value1 == value2);
    }

    //*************************************************************************
    TEST(test_runtime_equality_uses_current_value)
    {
      etl::clamped_value<int> equal_value1(2, 7, 3);
      etl::clamped_value<int> equal_value2(2, 8, 3);
      etl::clamped_value<int> different_value(2, 8, 4);
      CHECK(equal_value1 == equal_value2);
      CHECK(!(equal_value1 != equal_value2));
      CHECK(equal_value1 != different_value);
    }

    //*************************************************************************
    TEST(test_copy_assignment_and_self_assignment_run_time)
    {
      etl::clamped_value<int> value1(2, 7, 5);
      etl::clamped_value<int> value2(3, 8, 6);
      value1 = value2;
      CHECK_EQUAL(6, value1.get());
      CHECK_EQUAL(3, value1.min());
      CHECK_EQUAL(8, value1.max());
      etl::clamped_value<int>* value1_pointer = &value1;
      value1                                  = *value1_pointer;
      CHECK_EQUAL(6, value1.get());
      CHECK_EQUAL(3, value1.min());
      CHECK_EQUAL(8, value1.max());
    }

    //*************************************************************************
    TEST(test_unsigned_boundary_values)
    {
      etl::clamped_value<unsigned, 0U, 2U> value(2U);
      ++value;
      CHECK_EQUAL(2U, value.get());
      value.to_min();
      --value;
      CHECK_EQUAL(0U, value.get());
    }

    //*************************************************************************
    TEST(test_narrow_signed_range_spanning_lowest_value)
    {
      etl::clamped_value<int8_t, INT8_MIN, -10> value(-50);

      value += 10;
      CHECK_EQUAL(-40, value.get());
      value -= 10;
      CHECK_EQUAL(-50, value.get());
      value.advance(INT8_MIN);
      CHECK_EQUAL(INT8_MIN, value.get());
      value.advance(INT8_MAX);
      CHECK_EQUAL(-10, value.get());
    }

    //*************************************************************************
    TEST(test_runtime_postfix_and_advance)
    {
      etl::clamped_value<int> value(2, 7, 4);
      etl::clamped_value<int> previous_increment = value++;
      CHECK_EQUAL(4, previous_increment.get());
      CHECK_EQUAL(5, value.get());
      etl::clamped_value<int> previous_decrement = value--;
      CHECK_EQUAL(5, previous_decrement.get());
      CHECK_EQUAL(4, value.get());
      value.advance(0);
      CHECK_EQUAL(4, value.get());
      value.advance(3);
      CHECK_EQUAL(7, value.get());
      value.advance(-3);
      CHECK_EQUAL(4, value.get());
    }

    //*************************************************************************
    TEST(test_wide_and_negative_ranges)
    {
      etl::clamped_value<int, -7, -2>                      negative(-4);
      etl::clamped_value<int64_t, -INT64_C(7), INT64_C(7)> wide(0);
      etl::clamped_value<unsigned>                         unsigned_value(2U, 7U, 4U);

      negative.advance(100);
      wide.advance(INT64_MAX);
      unsigned_value.advance(3);

      CHECK_EQUAL(-2, negative.get());
      CHECK_EQUAL(INT64_C(7), wide.get());
      CHECK_EQUAL(7U, unsigned_value.get());
    }

    //*************************************************************************
    TEST(test_compound_and_comparison_operators)
    {
      etl::clamped_value<int, 2, 7> value(4);
      etl::clamped_value<int, 2, 7> other(6);

      value += 10;
      CHECK_EQUAL(7, value.get());
      value -= 10;
      CHECK_EQUAL(2, value.get());
      CHECK(value < other);
      CHECK(value < 6);
      CHECK(4 < other);
      CHECK(value <= other);
      CHECK(value <= 4);
      CHECK(4 <= other);
      CHECK(other > value);
      CHECK(other > 4);
      CHECK(6 > value);
      CHECK(other >= value);
      CHECK(other >= 6);
      CHECK(6 >= value);
      CHECK(value == 2);
      CHECK(2 == value);
      CHECK(value != 6);
      CHECK(6 != value);
    }

    //*************************************************************************
    TEST(test_mixed_type_comparisons_are_not_ambiguous)
    {
      etl::clamped_value<uint8_t, 0, 200> narrow(10);
      etl::clamped_value<int, 0, 200>     value(10);

      CHECK(narrow < 20);
      CHECK(value < 20L);
      CHECK(20L > value);
      CHECK(value <= 20L);
      CHECK(20L >= value);
    }

    //*************************************************************************
    TEST(test_runtime_comparison_operator_overloads)
    {
      etl::clamped_value<int> value(2, 7, 4);
      etl::clamped_value<int> other(2, 8, 6);

      CHECK(value < other);
      CHECK(value < 6);
      CHECK(4 < other);
      CHECK(value <= other);
      CHECK(value <= 4);
      CHECK(4 <= other);
      CHECK(other > value);
      CHECK(other > 4);
      CHECK(6 > value);
      CHECK(other >= value);
      CHECK(other >= 6);
      CHECK(6 >= value);
    }

    //*************************************************************************
    TEST(test_advance_extreme_values)
    {
      etl::clamped_value<int, 2, 7> compile_time(4);
      etl::clamped_value<int>       run_time(2, 7, 4);
      compile_time.advance(INT_MAX);
      run_time.advance(INT_MIN);
      CHECK_EQUAL(7, compile_time.get());
      CHECK_EQUAL(2, run_time.get());
    }

    //*************************************************************************
    TEST(test_subtract_lowest_difference_saturates)
    {
      etl::clamped_value<int, -10, 10> compile_time(0);
      etl::clamped_value<int>          run_time(-10, 10, 0);

      compile_time -= etl::clamped_value<int, -10, 10>::difference_limits_type::lowest();
      run_time -= etl::clamped_value<int>::difference_limits_type::lowest();

      CHECK_EQUAL(10, compile_time.get());
      CHECK_EQUAL(10, run_time.get());
    }

    //*************************************************************************
    TEST(test_runtime_compound_assignment_and_swap)
    {
      etl::clamped_value<int> value1(-4, 8, 2);
      etl::clamped_value<int> value2(10, 20, 15);

      value1 += 20;
      value2 -= 20;
      value1.swap(value2);

      CHECK_EQUAL(10, value1.get());
      CHECK_EQUAL(10, value1.min());
      CHECK_EQUAL(20, value1.max());
      CHECK_EQUAL(8, value2.get());
      CHECK_EQUAL(-4, value2.min());
      CHECK_EQUAL(8, value2.max());
    }

    //*************************************************************************
    TEST(test_runtime_reversed_bounds_are_rejected)
    {
      CHECK_THROW((etl::clamped_value<int>(7, 2)), etl::exception);
      CHECK_THROW((etl::clamped_value<int>(7, 2, 4)), etl::exception);

      etl::clamped_value<int> value;
      CHECK_THROW(value.set(7, 2), etl::exception);
    }

#if ETL_HAS_FLOATING_POINT_CLAMPED_VALUE
    //*************************************************************************
    TEST(test_compile_time_floating_clamps_and_advances)
    {
      etl::clamped_value<float, -1.5f, 2.5f> value(4.0f);
      CHECK_CLOSE(2.5f, value.get(), 0.0001f);

      value = -4.0f;
      CHECK_CLOSE(-1.5f, value.get(), 0.0001f);

      value += 0.75f;
      CHECK_CLOSE(-0.75f, value.get(), 0.0001f);
      value -= 0.5f;
      CHECK_CLOSE(-1.25f, value.get(), 0.0001f);
      value.advance(10.0f);
      CHECK_CLOSE(2.5f, value.get(), 0.0001f);
    }

    //*************************************************************************
    TEST(test_runtime_floating_clamps_and_advances)
    {
      etl::clamped_value<double> value(-1.5, 2.5, 4.0);
      CHECK_CLOSE(2.5, value.get(), 0.0001);

      value = -4.0;
      value += 0.75;
      value -= 0.5;
      CHECK_CLOSE(-1.25, value.get(), 0.0001);

      value.advance(-10.0);
      CHECK_CLOSE(-1.5, value.get(), 0.0001);
    }

    //*************************************************************************
    TEST(test_floating_increment_and_decrement_saturate_at_fractional_bounds)
    {
      etl::clamped_value<float, -0.5f, 0.5f> compile_time(0.25f);
      etl::clamped_value<float>              run_time(-0.5f, 0.5f, -0.25f);

      ++compile_time;
      --run_time;

      CHECK_CLOSE(0.5f, compile_time.get(), 0.0001f);
      CHECK_CLOSE(-0.5f, run_time.get(), 0.0001f);
    }

    //*************************************************************************
    TEST(test_floating_comparison_and_swap)
    {
      etl::clamped_value<double, -2.0, 2.0> compile_time1(-0.5);
      etl::clamped_value<double, -2.0, 2.0> compile_time2(1.5);
      etl::clamped_value<double>            run_time1(-3.0, 3.0, -1.0);
      etl::clamped_value<double>            run_time2(4.0, 8.0, 6.0);

      CHECK(compile_time1 < compile_time2);
      CHECK(compile_time1 < 1.0);
      swap(compile_time1, compile_time2);
      swap(run_time1, run_time2);

      CHECK_CLOSE(1.5, compile_time1.get(), 0.0001);
      CHECK_CLOSE(6.0, run_time1.get(), 0.0001);
      CHECK_CLOSE(4.0, run_time1.min(), 0.0001);
      CHECK_CLOSE(-1.0, run_time2.get(), 0.0001);
    }

    //*************************************************************************
    TEST(test_floating_infinity_saturates)
    {
      const double                          infinity = etl::numeric_limits<double>::infinity();
      etl::clamped_value<double, -2.0, 2.0> value(0.0);
      etl::clamped_value<double>            infinite_range(-infinity, infinity, 0.0);

      value += infinity;
      CHECK_CLOSE(2.0, value.get(), 0.0001);
      value -= infinity;
      CHECK_CLOSE(-2.0, value.get(), 0.0001);

      infinite_range = infinity;
      CHECK(etl::is_infinity(infinite_range.get()));
    }

    //*************************************************************************
    TEST(test_floating_finite_overflow_is_prevented)
    {
      const double               maximum = etl::numeric_limits<double>::max();
      etl::clamped_value<double> value(-maximum, maximum, maximum / 2.0);

      value += maximum;
      CHECK_EQUAL(maximum, value.get());

      value = -maximum / 2.0;
      value -= maximum;
      CHECK_EQUAL(-maximum, value.get());

      value = -maximum;
      value += maximum;
      CHECK_EQUAL(0.0, value.get());
      value += maximum;
      CHECK_EQUAL(maximum, value.get());
    }

    //*************************************************************************
    TEST(test_floating_nan_is_rejected)
    {
      const double nan = etl::numeric_limits<double>::quiet_NaN();
      CHECK_THROW((etl::clamped_value<double>(nan, 1.0)), etl::exception);
      CHECK_THROW((etl::clamped_value<double>(-1.0, nan)), etl::exception);
      CHECK_THROW((etl::clamped_value<double>(-1.0, 1.0, nan)), etl::exception);

      etl::clamped_value<double> value(-1.0, 1.0, 0.0);
      CHECK_THROW(value.set(nan, 1.0), etl::exception);
      CHECK_THROW(value.set(-1.0, nan), etl::exception);
      CHECK_THROW(value = nan, etl::exception);
      CHECK_THROW(value.advance(nan), etl::exception);
      CHECK_THROW(value += nan, etl::exception);
      CHECK_THROW(value -= nan, etl::exception);
    }

    //*************************************************************************
    TEST(test_default_floating_range)
    {
      etl::clamped_value<double> value;
      CHECK_EQUAL(etl::numeric_limits<double>::lowest(), value.min());
      CHECK_EQUAL(etl::numeric_limits<double>::max(), value.max());
      CHECK_EQUAL(etl::numeric_limits<double>::lowest(), value.get());
    }
#endif

#if ETL_USING_CPP14
    //*************************************************************************
    TEST(test_clamped_value_constexpr_ctor)
    {
      constexpr etl::clamped_value<int, 0, 9> value;
      static_assert(value.get() == 0, "constexpr clamped_value constructor");
      CHECK(true);
    }

  #if ETL_HAS_FLOATING_POINT_CLAMPED_VALUE
    //*************************************************************************
    TEST(test_floating_clamped_value_constexpr)
    {
      constexpr etl::clamped_value<double, -1.0, 1.0> value(0.5);
      static_assert((value.get() > 0.49) && (value.get() < 0.51), "constexpr floating clamped_value constructor");
      CHECK(true);
    }
  #endif
#endif
  }
} // namespace
