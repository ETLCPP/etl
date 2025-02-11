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
#elif defined(ETL_COMPILER_GCC)
  #include "etl/atomic/atomic_gcc_sync.h"
#elif defined(ETL_COMPILER_CLANG)
  #include "etl/atomic/atomic_clang_sync.h"
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

//#if ETL_NOT_USING_STL && ETL_HAS_ATOMIC
//      CHECK_TRUE(etl::atomic<int>::is_always_lock_free);
//      CHECK_TRUE(test.is_always_lock_free);
//#endif
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

//#if ETL_NOT_USING_STL && ETL_HAS_ATOMIC
//    //*************************************************************************
//    TEST(test_atomic_is_always_lock_free)
//    {
//      struct S 
//      {
//        int a;
//        int b;
//        int c;
//      };
//
//      CHECK_TRUE(etl::atomic<int>::is_always_lock_free);
//      CHECK_TRUE(etl::atomic<int*>::is_always_lock_free);
//      CHECK_FALSE(etl::atomic<S>::is_always_lock_free);
//    }
//#endif

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
    TEST(test_atomic_compare_exchange_weak_fail)
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
    TEST(test_atomic_compare_exchange_weak_pass)
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
    TEST(test_atomic_compare_exchange_strong_fail)
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
    TEST(test_atomic_compare_exchange_strong_pass)
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
