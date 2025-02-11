/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl
https://www.etlcpp.com

Copyright(c) 2022 John Wellbelove

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

#include "etl/platform.h"
#include "etl/enum_type.h"

#if defined(ETL_COMPILER_MICROSOFT)
  #include "etl/atomic/atomic_std.h"
#elif defined(ETL_COMPILER_ARM5)
  #include "atomic/atomic_arm.h"
#elif defined(ETL_COMPILER_ARM6)
  #include "atomic/atomic_arm.h"
#elif defined(ETL_COMPILER_GCC)
  #include "etl/atomic/atomic_gcc.h"
#elif defined(ETL_COMPILER_CLANG)
  #include "etl/atomic/atomic_clang.h"
#endif

#include <atomic>
#include <thread>

#if defined(ETL_TARGET_OS_WINDOWS)
  #include <Windows.h>
#endif

#define REALTIME_TEST 0

namespace
{
  struct Enum
  {
    enum enum_type
    {
      Zero = 0,
      One = 1,
      Two = 2
    };
 
    ETL_DECLARE_ENUM_TYPE(Enum, int)
    ETL_ENUM_TYPE(Zero, "0")
    ETL_ENUM_TYPE(One,  "1")
    ETL_ENUM_TYPE(Two,  "2")
    ETL_END_ENUM_TYPE
  };

  SUITE(test_atomic)
  {
    //*************************************************************************
    TEST(test_atomic_integer_is_lock_free)
    {
      std::atomic<int> compare;
      etl::atomic<int> test;

      CHECK_EQUAL(compare.is_lock_free(), test.is_lock_free());

#if ETL_NOT_USING_STL && ETL_HAS_ATOMIC
      CHECK_TRUE(etl::atomic<int>::is_always_lock_free);
      CHECK_TRUE(test.is_always_lock_free);
#endif
    }

    //*************************************************************************
    TEST(test_atomic_pointer_is_lock_free)
    {
      std::atomic<int*> compare;
      etl::atomic<int*> test;

      CHECK_EQUAL(compare.is_lock_free(), test.is_lock_free());

#if ETL_NOT_USING_STL && ETL_HAS_ATOMIC
      CHECK_TRUE(etl::atomic<int*>::is_always_lock_free);
      CHECK_TRUE(test.is_always_lock_free);
#endif
    }

    //*************************************************************************
    TEST(test_atomic_float_is_lock_free)
    {
      std::atomic<float> compare;
      etl::atomic<float> test;

      CHECK_EQUAL(compare.is_lock_free(), test.is_lock_free());

#if ETL_NOT_USING_STL && ETL_HAS_ATOMIC
      CHECK_TRUE(etl::atomic<float>::is_always_lock_free);
      CHECK_TRUE(test.is_always_lock_free);
#endif
    }

    //*************************************************************************
    TEST(test_atomic_double_is_lock_free)
    {
      std::atomic<double> compare;
      etl::atomic<double> test;

      CHECK_EQUAL(compare.is_lock_free(), test.is_lock_free());

#if ETL_NOT_USING_STL && ETL_HAS_ATOMIC
      CHECK_TRUE(etl::atomic<double>::is_always_lock_free);
      CHECK_TRUE(test.is_always_lock_free);
#endif
    }

#if ETL_NOT_USING_STL && ETL_HAS_ATOMIC
    //*************************************************************************
    TEST(test_atomic_is_always_lock_free)
    {
      struct S 
      {
        char a;
        char b;
        short c;
        int d;
      };

      constexpr bool ialf_int    = etl::atomic<int>::is_always_lock_free;
      constexpr bool ialf_pint   = etl::atomic<int*>::is_always_lock_free;
      constexpr bool ialf_s      = etl::atomic<S>::is_always_lock_free;
      constexpr bool ialf_double = etl::atomic<double>::is_always_lock_free;

      CHECK_TRUE(ialf_int);
      CHECK_TRUE(ialf_pint);
      CHECK_TRUE(ialf_s);
      CHECK_TRUE(ialf_double);
    }
#endif

    //*************************************************************************
    TEST(test_atomic_integer_load)
    {
      std::atomic<int> compare(1);
      etl::atomic<int> test(1);

      CHECK_EQUAL(compare.load(), test.load());
    }

    //*************************************************************************
    TEST(test_atomic_enum_load)
    {
      std::atomic<Enum> compare(Enum::One);
      etl::atomic<Enum> test(Enum::One);

      CHECK_EQUAL(compare.load(), test.load());
    }

    //*************************************************************************
    TEST(test_atomic_pointer_load)
    {
      int i;

      std::atomic<int*> compare(&i);
      etl::atomic<int*> test(&i);

      CHECK_EQUAL(compare.load(), test.load());
    }

    //*************************************************************************
    TEST(test_atomic_bool_load)
    {
      std::atomic<bool> compare(true);
      etl::atomic<bool> test(true);

      CHECK_EQUAL(compare.load(), test.load());
    }

    //*************************************************************************
    TEST(test_atomic_float_load)
    {
      std::atomic<float> compare(1.23f);
      etl::atomic<float> test(1.23f);

      CHECK_EQUAL(compare.load(), test.load());
    }

    //*************************************************************************
    TEST(test_atomic_double_load)
    {
      std::atomic<double> compare(1.23);
      etl::atomic<double> test(1.23);

      CHECK_EQUAL(compare.load(), test.load());
    }

    //*************************************************************************
    TEST(test_atomic_struct_load)
    {
      struct S 
      {
        char a;
        char b;
        short c;
        int d;
      };

      S s1 = { char(0xA5), char(0x5A), short(0x55AA), int(0xFF5511AA) };

      etl::atomic<S> test(s1);

      S s2 = test.load();

      CHECK_EQUAL(s1.a, s2.a);
      CHECK_EQUAL(s1.b, s2.b);
      CHECK_EQUAL(s1.c, s2.c);
      CHECK_EQUAL(s1.d, s2.d);
    }
    
    //*************************************************************************
    TEST(test_atomic_integer_implicit_conversion)
    {
      std::atomic<int> compare(1);
      etl::atomic<int> test(1);

      int c = compare;
      int t = test;

      CHECK_EQUAL(c, t);
    }

    //*************************************************************************
    TEST(test_atomic_enum_implicit_conversion)
    {
      std::atomic<Enum> compare(Enum::One);
      etl::atomic<Enum> test(Enum::One);

      Enum c = compare;
      Enum t = test;

      CHECK_EQUAL(c, t);
    }

    //*************************************************************************
    TEST(test_atomic_pointer_implicit_conversion)
    {
      int i;

      std::atomic<int*> compare(&i);
      etl::atomic<int*> test(&i);

      int* c = compare;
      int* t = test;

      CHECK_EQUAL(c, t);
    }

    //*************************************************************************
    TEST(test_atomic_bool_implicit_conversion)
    {
      std::atomic<bool> compare(true);
      etl::atomic<bool> test(true);

      bool c = compare;
      bool t = test;

      CHECK_EQUAL(c, t);
    }

    //*************************************************************************
    TEST(test_atomic_float_implicit_conversion)
    {
      std::atomic<float> compare(1.23f);
      etl::atomic<float> test(1.23f);

      float c = compare;
      float t = test;

      CHECK_EQUAL(c, t);
    }

    //*************************************************************************
    TEST(test_atomic_double_implicit_conversion)
    {
      std::atomic<double> compare(1.23);
      etl::atomic<double> test(1.23);

      double c = compare;
      double t = test;

      CHECK_EQUAL(c, t);
    }

    //*************************************************************************
    TEST(test_atomic_struct_implicit_conversion)
    {
      struct S 
      {
        char a;
        char b;
        short c;
        int d;
      };

      S s1 = { char(0xA5), char(0x5A), short(0x55AA), int(0xFF5511AA) };

      etl::atomic<S> test(s1);

      S s2 = test;

      CHECK_EQUAL(s1.a, s2.a);
      CHECK_EQUAL(s1.b, s2.b);
      CHECK_EQUAL(s1.c, s2.c);
      CHECK_EQUAL(s1.d, s2.d);
    }

    //*************************************************************************
    TEST(test_atomic_integer_store)
    {
      std::atomic<int> compare(1);
      etl::atomic<int> test(1);

      compare.store(2);
      test.store(2);
      CHECK_EQUAL(compare.load(), test.load());
    }

    //*************************************************************************
    TEST(test_atomic_enum_store)
    {
      std::atomic<Enum> compare(Enum::One);
      etl::atomic<Enum> test(Enum::One);

      compare.store(Enum::Two);
      test.store(Enum::Two);
      CHECK_EQUAL(compare.load(), test.load());
    }

    //*************************************************************************
    TEST(test_atomic_pointer_store)
    {
      int i;
      int j;

      std::atomic<int*> compare(&i);
      etl::atomic<int*> test(&i);

      compare.store(&j);
      test.store(&j);
      CHECK_EQUAL(compare.load(), test.load());
    }

    //*************************************************************************
    TEST(test_atomic_bool_store)
    {
      std::atomic<bool> compare(false);
      etl::atomic<bool> test(false);

      compare.store(true);
      test.store(true);
      CHECK_EQUAL(compare.load(), test.load());
    }

    //*************************************************************************
    TEST(test_atomic_float_store)
    {
      std::atomic<float> compare(1.23f);
      etl::atomic<float> test(1.23f);

      compare.store(1.23f);
      test.store(1.23f);
      CHECK_EQUAL(compare.load(), test.load());
    }

    //*************************************************************************
    TEST(test_atomic_double_store)
    {
      std::atomic<double> compare(1.23);
      etl::atomic<double> test(1.23);

      compare.store(1.23);
      test.store(1.23);
      CHECK_EQUAL(compare.load(), test.load());
    }

    //*************************************************************************
    TEST(test_atomic_struct_store)
    {
      struct S 
      {
        char a;
        char b;
        short c;
        int d;
      };

      S s1 = { char(0xA5), char(0x5A), short(0x55AA), int(0xFF5511AA) };

      etl::atomic<S> test;
      test.store(s1);

      S s2 = test.load();

      CHECK_EQUAL(s1.a, s2.a);
      CHECK_EQUAL(s1.b, s2.b);
      CHECK_EQUAL(s1.c, s2.c);
      CHECK_EQUAL(s1.d, s2.d);
    }

    //*************************************************************************
    TEST(test_atomic_integer_assignment)
    {
      std::atomic<int> compare(1);
      etl::atomic<int> test(1);

      compare = 2;
      test = 2;
      CHECK_EQUAL(compare.load(), test.load());
    }

    //*************************************************************************
    TEST(test_atomic_enum_assignment)
    {
      std::atomic<Enum> compare(Enum::One);
      etl::atomic<Enum> test(Enum::One);

      compare = Enum::Two;
      test = Enum::Two;
      CHECK_EQUAL(compare.load(), test.load());
    }

    //*************************************************************************
    TEST(test_atomic_pointer_assignment)
    {
      int i;
      int j;

      std::atomic<int*> compare(&i);
      etl::atomic<int*> test(&i);

      compare = &j;
      test = &j;
      CHECK_EQUAL(compare.load(), test.load());
    }

    //*************************************************************************
    TEST(test_atomic_bool_assignment)
    {
      std::atomic<bool> compare(false);
      etl::atomic<bool> test(false);

      compare = true;
      test = true;
      CHECK_EQUAL(compare.load(), test.load());
    }

    //*************************************************************************
    TEST(test_atomic_float_assignment)
    {
      std::atomic<double> compare(1.23f);
      etl::atomic<double> test(1.23f);

      compare = 1.23f;
      test = 1.23f;
      CHECK_EQUAL(compare.load(), test.load());
    }

    //*************************************************************************
    TEST(test_atomic_double_assignment)
    {
      std::atomic<double> compare(1.23f);
      etl::atomic<double> test(1.23f);

      compare = 1.23f;
      test = 1.23f;
      CHECK_EQUAL(compare.load(), test.load());
    }

    //*************************************************************************
    TEST(test_atomic_struct_assignment)
    {
      struct S 
      {
        char a;
        char b;
        short c;
        int d;
      };

      S s1 = { char(0x00), char(0x00), 0x0000, 0x00000000 };
      S s2 = { char(0xA5), char(0x5A), short(0x55AA), int(0xFF5511AA) };

      etl::atomic<S> test(s1);
      test = s2;

      S s3 = test.load();

      CHECK_EQUAL(s2.a, s3.a);
      CHECK_EQUAL(s2.b, s3.b);
      CHECK_EQUAL(s2.c, s3.c);
      CHECK_EQUAL(s2.d, s3.d);
    }

    //*************************************************************************
    TEST(test_atomic_operator_integer_pre_increment)
    {
      std::atomic<int> compare(1);
      etl::atomic<int> test(1);

      CHECK_EQUAL(++compare, ++test);
      CHECK_EQUAL(++compare, ++test);
    }

    //*************************************************************************
    TEST(test_atomic_operator_integer_post_increment)
    {
      std::atomic<int> compare(1);
      etl::atomic<int> test(1);

      CHECK_EQUAL(compare++, test++);
      CHECK_EQUAL(compare++, test++);
    }

    //*************************************************************************
    TEST(test_atomic_operator_integer_pre_decrement)
    {
      std::atomic<int> compare(1);
      etl::atomic<int> test(1);

      CHECK_EQUAL(--compare, --test);
      CHECK_EQUAL(--compare, --test);
    }

    //*************************************************************************
    TEST(test_atomic_operator_integer_post_decrement)
    {
      std::atomic<int> compare(1);
      etl::atomic<int> test(1);

      CHECK_EQUAL(compare--, test--);
      CHECK_EQUAL(compare--, test--);
    }

    //*************************************************************************
    TEST(test_atomic_operator_pointer_pre_increment)
    {
      int data[] = { 1, 2, 3, 4 };

      std::atomic<int*> compare(&data[0]);
      etl::atomic<int*> test(&data[0]);

      CHECK_EQUAL(++compare, ++test);
      CHECK_EQUAL(++compare, ++test);
    }

    //*************************************************************************
    TEST(test_atomic_operator_pointer_post_increment)
    {
      int data[] = { 1, 2, 3, 4 };

      std::atomic<int*> compare(&data[0]);
      etl::atomic<int*> test(&data[0]);

      CHECK_EQUAL(compare++, test++);
      CHECK_EQUAL(compare++, test++);
    }

    //*************************************************************************
    TEST(test_atomic_operator_pointer_pre_decrement)
    {
      int data[] = { 1, 2, 3, 4 };

      std::atomic<int*> compare(&data[3]);
      etl::atomic<int*> test(&data[3]);

      CHECK_EQUAL(--compare, --test);
      CHECK_EQUAL(--compare, --test);
    }

    //*************************************************************************
    TEST(test_atomic_operator_pointer_post_decrement)
    {
      int data[] = { 1, 2, 3, 4 };

      std::atomic<int*> compare(&data[3]);
      etl::atomic<int*> test(&data[3]);

      CHECK_EQUAL(compare--, test--);
      CHECK_EQUAL(compare--, test--);
    }

    //*************************************************************************
    TEST(test_atomic_operator_integer_fetch_add)
    {
      std::atomic<int> compare(1);
      etl::atomic<int> test(1);

      CHECK_EQUAL(compare.fetch_add(2), test.fetch_add(2));
    }

    //*************************************************************************
    TEST(test_atomic_operator_pointer_fetch_add)
    {
      int data[] = { 1, 2, 3, 4 };

      std::atomic<int*> compare(&data[0]);
      etl::atomic<int*> test(&data[0]);

      CHECK_EQUAL(compare.fetch_add(ptrdiff_t(10)), test.fetch_add(ptrdiff_t(10)));
    }

    //*************************************************************************
    TEST(test_atomic_operator_integer_plus_equals)
    {
      std::atomic<int> compare(1);
      etl::atomic<int> test(1);

      compare += 2;
      test += 2;

      CHECK_EQUAL(compare, test);
    }

    //*************************************************************************
    TEST(test_atomic_operator_pointer_plus_equals)
    {
      int data[] = { 1, 2, 3, 4 };

      std::atomic<int*> compare(&data[0]);
      etl::atomic<int*> test(&data[0]);

      compare += 2;
      test += 2;

      CHECK_EQUAL(compare, test);
    }

    //*************************************************************************
    TEST(test_atomic_operator_integer_minus_equals)
    {
      std::atomic<int> compare(1);
      etl::atomic<int> test(1);

      compare -= 2;
      test -= 2;

      CHECK_EQUAL(compare, test);
    }

    //*************************************************************************
    TEST(test_atomic_operator_pointer_minus_equals)
    {
      int data[] = { 1, 2, 3, 4 };

      std::atomic<int*> compare(&data[3]);
      etl::atomic<int*> test(&data[3]);

      compare -= 2;
      test -= 2;

      CHECK_EQUAL(compare, test);
    }

    //*************************************************************************
    TEST(test_atomic_operator_integer_and_equals)
    {
      std::atomic<int> compare(0x0000FFFFUL);
      etl::atomic<int> test(0x0000FFFFUL);

      compare &= 0x55AA55AAUL;
      test &= 0x55AA55AAUL;

      CHECK_EQUAL(compare, test);
    }

    //*************************************************************************
    TEST(test_atomic_operator_integer_or_equals)
    {
      std::atomic<int> compare(0x0000FFFFUL);
      etl::atomic<int> test(0x0000FFFFUL);

      compare |= 0x55AA55AAUL;
      test |= 0x55AA55AAUL;

      CHECK_EQUAL(compare, test);
    }

    //*************************************************************************
    TEST(test_atomic_operator_integer_xor_equals)
    {
      std::atomic<int> compare(0x0000FFFFUL);
      etl::atomic<int> test(0x0000FFFFUL);

      compare ^= 0x55AA55AAUL;
      test ^= 0x55AA55AAUL;

      CHECK_EQUAL(compare, test);
    }

    //*************************************************************************
    TEST(test_atomic_operator_integer_fetch_sub)
    {
      std::atomic<int> compare(1);
      etl::atomic<int> test(1);

      CHECK_EQUAL(compare.fetch_sub(2), test.fetch_sub(2));
    }

    //*************************************************************************
    TEST(test_atomic_operator_pointer_fetch_sub)
    {
      int data[] = { 1, 2, 3, 4 };

      std::atomic<int*> compare(&data[0]);
      etl::atomic<int*> test(&data[0]);

      CHECK_EQUAL(compare.fetch_add(ptrdiff_t(10)), test.fetch_add(ptrdiff_t(10)));
    }

    //*************************************************************************
    TEST(test_atomic_operator_fetch_and)
    {
      std::atomic<int> compare(0xFFFFFFFFUL);
      etl::atomic<int> test(0xFFFFFFFFUL);

      CHECK_EQUAL(compare.fetch_and(0x55AA55AAUL), test.fetch_and(0x55AA55AAUL));
    }

    //*************************************************************************
    TEST(test_atomic_operator_fetch_or)
    {
      std::atomic<int> compare(0x0000FFFFUL);
      etl::atomic<int> test(0x0000FFFFUL);

      CHECK_EQUAL(compare.fetch_or(0x55AA55AAUL), test.fetch_or(0x55AA55AAUL));
    }

    //*************************************************************************
    TEST(test_atomic_operator_fetch_xor)
    {
      std::atomic<int> compare(0x0000FFFFUL);
      etl::atomic<int> test(0x0000FFFFUL);

      CHECK_EQUAL(compare.fetch_xor(0x55AA55AAUL), test.fetch_xor(0x55AA55AAUL));
    }

    //*************************************************************************
    TEST(test_atomic_integer_exchange)
    {
      std::atomic<int> compare(1);
      etl::atomic<int> test(1);

      CHECK_EQUAL(compare.exchange(2), test.exchange(2));
    }

    //*************************************************************************
    TEST(test_atomic_enum_exchange)
    {
      std::atomic<Enum> compare(Enum::One);
      etl::atomic<Enum> test(Enum::One);

      CHECK_EQUAL(compare.exchange(Enum::Two), test.exchange(Enum::Two));
    }

    //*************************************************************************
    TEST(test_atomic_pointer_exchange)
    {
      int i;
      int j;

      std::atomic<int*> compare(&i);
      etl::atomic<int*> test(&i);

      CHECK_EQUAL(compare.exchange(&j), test.exchange(&j));
    }

    //*************************************************************************
    TEST(test_atomic_bool_exchange)
    {
      std::atomic<bool> compare(false);
      etl::atomic<bool> test(false);

      CHECK_EQUAL(compare.exchange(true), test.exchange(true));
    }

    //*************************************************************************
    TEST(test_atomic_struct_exchange)
    {
      struct S 
      {
        char a;
        char b;
        short c;
        int d;
      };

      S s1 = { char(0x00), char(0x00), 0x0000, 0x00000000 };
      S s2 = { char(0xA5), char(0x5A), short(0x55AA), int(0xFF5511AA) };

      etl::atomic<S> test(s1);

      S s3 = test.exchange(s2);
      S s4 = test.load();

      CHECK_EQUAL(s1.a, s3.a);
      CHECK_EQUAL(s1.b, s3.b);
      CHECK_EQUAL(s1.c, s3.c);
      CHECK_EQUAL(s1.d, s3.d);

      CHECK_EQUAL(s2.a, s4.a);
      CHECK_EQUAL(s2.b, s4.b);
      CHECK_EQUAL(s2.c, s4.c);
      CHECK_EQUAL(s2.d, s4.d);
    }

    //*************************************************************************
    TEST(test_atomic_compare_exchange_weak_fail_for_integer)
    {
      std::atomic<int> compare;
      etl::atomic<int> test;

      int actual = 1;

      compare = actual;
      test    = actual;

      int compare_expected = 2;
      int test_expected    = 2;
      int desired  = 3;

      bool compare_result = compare.compare_exchange_weak(compare_expected, desired);
      bool test_result    = test.compare_exchange_weak(test_expected, desired);

      CHECK_EQUAL(compare_result,   test_result);
      CHECK_EQUAL(compare_expected, test_expected);
      CHECK_EQUAL(compare.load(),   test.load());
    }

    //*************************************************************************
    TEST(test_atomic_compare_exchange_weak_pass_for_integer)
    {
      std::atomic<int> compare;
      etl::atomic<int> test;

      int actual = 1;

      compare = actual;
      test    = actual;

      int compare_expected = actual;
      int test_expected    = actual;
      int desired  = 3;

      bool compare_result = compare.compare_exchange_weak(compare_expected, desired);
      bool test_result    = test.compare_exchange_weak(test_expected, desired);

      CHECK_EQUAL(compare_result,   test_result);
      CHECK_EQUAL(compare_expected, test_expected);
      CHECK_EQUAL(compare.load(),   test.load());
    }

    //*************************************************************************
    TEST(test_atomic_compare_exchange_weak_pass_for_enum)
    {
      std::atomic<Enum> compare(Enum::Zero);
      etl::atomic<Enum> test(Enum::Zero);

      Enum actual = Enum::One;

      compare = actual;
      test = actual;

      Enum compare_expected = actual;
      Enum test_expected = actual;
      Enum desired = Enum::Two;

      bool compare_result = compare.compare_exchange_weak(compare_expected, desired);
      bool test_result = test.compare_exchange_weak(test_expected, desired);

      CHECK_EQUAL(compare_result, test_result);
      CHECK_EQUAL(compare_expected, test_expected);
      CHECK_EQUAL(compare.load(), test.load());
    }

    //*************************************************************************
    TEST(test_atomic_compare_exchange_weak_pass_for_bool)
    {
      std::atomic<bool> compare;
      etl::atomic<bool> test;

      bool actual = false;

      compare = actual;
      test = actual;

      bool compare_expected = actual;
      bool test_expected = actual;
      bool desired = true;

      bool compare_result = compare.compare_exchange_weak(compare_expected, desired);
      bool test_result = test.compare_exchange_weak(test_expected, desired);

      CHECK_EQUAL(compare_result, test_result);
      CHECK_EQUAL(compare_expected, test_expected);
      CHECK_EQUAL(compare.load(), test.load());
    }

    //*************************************************************************
    TEST(test_atomic_compare_exchange_weak_pass_for_float)
    {
      std::atomic<float> compare;
      etl::atomic<float> test;

      float actual = 1.23f;

      compare = actual;
      test = actual;

      float compare_expected = actual;
      float test_expected = actual;
      float desired = 2.34f;

      bool compare_result = compare.compare_exchange_weak(compare_expected, desired);
      bool test_result = test.compare_exchange_weak(test_expected, desired);

      CHECK_EQUAL(compare_result, test_result);
      CHECK_EQUAL(compare_expected, test_expected);
      CHECK_EQUAL(compare.load(), test.load());
    }

    //*************************************************************************
    TEST(test_atomic_compare_exchange_weak_pass_for_double)
    {
      std::atomic<double> compare;
      etl::atomic<double> test;

      double actual = 1.23;

      compare = actual;
      test = actual;

      double compare_expected = actual;
      double test_expected = actual;
      double desired = 2.34;

      bool compare_result = compare.compare_exchange_weak(compare_expected, desired);
      bool test_result = test.compare_exchange_weak(test_expected, desired);

      CHECK_EQUAL(compare_result, test_result);
      CHECK_EQUAL(compare_expected, test_expected);
      CHECK_EQUAL(compare.load(), test.load());
    }

    //*************************************************************************
    TEST(test_atomic_compare_exchange_weak_pass_for_struct)
    {
      struct S 
      {
        char a;
        char b;
        short c;
        int d;
      };

      S s1 = { char(0x00), char(0x00), 0x0000, 0x00000000 };
      S s2 = { char(0xA5), char(0x5A), short(0x55AA), int(0xFF5511AA) };

      etl::atomic<S> test;

      S actual = s1;
      test = actual;

      S test_expected = actual;
      S desired = s2;

      bool test_result = test.compare_exchange_weak(test_expected, desired);

      S result = test.load();

      CHECK_TRUE(test_result);
      CHECK_EQUAL(result.a, desired.a);
      CHECK_EQUAL(result.b, desired.b);
      CHECK_EQUAL(result.c, desired.c);
      CHECK_EQUAL(result.d, desired.d);

    }

    //*************************************************************************
    TEST(test_atomic_compare_exchange_strong_fail_for_integer)
    {
      std::atomic<int> compare;
      etl::atomic<int> test;

      int actual = 1;

      compare = actual;
      test = actual;

      int compare_expected = 2;
      int test_expected = 2;
      int desired = 3;

      bool compare_result = compare.compare_exchange_strong(compare_expected, desired);
      bool test_result = test.compare_exchange_strong(test_expected, desired);

      CHECK_EQUAL(compare_result, test_result);
      CHECK_EQUAL(compare_expected, test_expected);
      CHECK_EQUAL(compare.load(), test.load());
    }

    //*************************************************************************
    TEST(test_atomic_compare_exchange_strong_pass_for_integer)
    {
      std::atomic<int> compare;
      etl::atomic<int> test;

      int actual = 1;

      compare = actual;
      test = actual;

      int compare_expected = actual;
      int test_expected = actual;
      int desired = 3;

      bool compare_result = compare.compare_exchange_strong(compare_expected, desired);
      bool test_result = test.compare_exchange_strong(test_expected, desired);

      CHECK_EQUAL(compare_result, test_result);
      CHECK_EQUAL(compare_expected, test_expected);
      CHECK_EQUAL(compare.load(), test.load());
    }

    //*************************************************************************
    TEST(test_atomic_compare_exchange_strong_pass_for_enum)
    {
      std::atomic<Enum> compare(Enum::Zero);
      etl::atomic<Enum> test(Enum::Zero);

      Enum actual = Enum::One;

      compare = actual;
      test = actual;

      Enum compare_expected = actual;
      Enum test_expected = actual;
      Enum desired = Enum::Two;

      bool compare_result = compare.compare_exchange_strong(compare_expected, desired);
      bool test_result    = test.compare_exchange_strong(test_expected, desired);

      CHECK_EQUAL(compare_result,   test_result);
      CHECK_EQUAL(compare_expected, test_expected);
      CHECK_EQUAL(compare.load(),   test.load());
    }

    //*************************************************************************
    TEST(test_atomic_compare_exchange_strong_pass_for_bool)
    {
      std::atomic<bool> compare;
      etl::atomic<bool> test;

      bool actual = false;

      compare = actual;
      test = actual;

      bool compare_expected = actual;
      bool test_expected = actual;
      bool desired = true;

      bool compare_result = compare.compare_exchange_strong(compare_expected, desired);
      bool test_result = test.compare_exchange_strong(test_expected, desired);

      CHECK_EQUAL(compare_result, test_result);
      CHECK_EQUAL(compare_expected, test_expected);
      CHECK_EQUAL(compare.load(), test.load());
    }

    //*************************************************************************
    TEST(test_atomic_compare_exchange_strong_pass_for_float)
    {
      std::atomic<float> compare;
      etl::atomic<float> test;

      float actual = 1.23f;

      compare = actual;
      test = actual;

      float compare_expected = actual;
      float test_expected = actual;
      float desired = 2.34f;

      bool compare_result = compare.compare_exchange_strong(compare_expected, desired);
      bool test_result = test.compare_exchange_strong(test_expected, desired);

      CHECK_EQUAL(compare_result, test_result);
      CHECK_EQUAL(compare_expected, test_expected);
      CHECK_EQUAL(compare.load(), test.load());
    }

    //*************************************************************************
    TEST(test_atomic_compare_exchange_strong_pass_for_double)
    {
      std::atomic<double> compare;
      etl::atomic<double> test;

      double actual = 1.23;

      compare = actual;
      test = actual;

      double compare_expected = actual;
      double test_expected = actual;
      double desired = 2.34f;

      bool compare_result = compare.compare_exchange_strong(compare_expected, desired);
      bool test_result = test.compare_exchange_strong(test_expected, desired);

      CHECK_EQUAL(compare_result, test_result);
      CHECK_EQUAL(compare_expected, test_expected);
      CHECK_EQUAL(compare.load(), test.load());
    }

    //*************************************************************************
#if REALTIME_TEST

#if defined(ETL_TARGET_OS_WINDOWS) // Only Windows priority is currently supported
  #define RAISE_THREAD_PRIORITY  SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_HIGHEST)
  #define FIX_PROCESSOR_AFFINITY1 SetThreadAffinityMask(GetCurrentThread(), 1)
  #define FIX_PROCESSOR_AFFINITY2 SetThreadAffinityMask(GetCurrentThread(), 2)
#else
  #define RAISE_THREAD_PRIORITY
  #define FIX_PROCESSOR_AFFINITY1
  #define FIX_PROCESSOR_AFFINITY2
#endif

    etl::atomic_int32_t atomic_value = 0U;
    etl::atomic<int>    start = false;

    void thread1()
    {
      RAISE_THREAD_PRIORITY;
      FIX_PROCESSOR_AFFINITY1;

      while (!start.load());

      for (int i = 0; i < 10000000; ++i)
      {
        ++atomic_value;
      }
    }

    void thread2()
    {
      RAISE_THREAD_PRIORITY;
      FIX_PROCESSOR_AFFINITY2;

      while (!start.load());

      for (int i = 0; i < 10000000; ++i)
      {
        --atomic_value;
      }
    }

    TEST(test_atomic_multi_thread)
    {
      std::thread t1(thread1);
      std::thread t2(thread2);

      start.store(true);

      t1.join();
      t2.join();

      CHECK_EQUAL(0, atomic_value.load());
    }
#endif
  };
}
