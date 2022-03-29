/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl
https://www.etlcpp.com

Copyright(c) 2017 jwellbelove

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

#if defined(ETL_COMPILER_CLANG)

#include "etl/atomic/atomic_clang_sync.h"

#include <atomic>
#include <thread>

#if defined(ETL_TARGET_OS_WINDOWS)
  #include <Windows.h>
#endif

#define REALTIME_TEST 0

namespace
{
  SUITE(test_atomic_clang)
  {
    //*************************************************************************
    TEST(test_atomic_integer_is_lock_free)
    {
      std::atomic<int> compare;
      etl::atomic<int> test;

      CHECK_EQUAL(compare.is_lock_free(), test.is_lock_free());
    }

    //*************************************************************************
    TEST(test_atomic_pointer_is_lock_free)
    {
      std::atomic<int*> compare;
      etl::atomic<int*> test;

      CHECK_EQUAL(compare.is_lock_free(), test.is_lock_free());
    }

    //*************************************************************************
    TEST(test_atomic_bool_is_lock_free)
    {
      std::atomic<bool> compare;
      etl::atomic<bool> test;

      CHECK_EQUAL(compare.is_lock_free(), test.is_lock_free());
    }

    //*************************************************************************
    TEST(test_atomic_integer_load)
    {
      std::atomic<int> compare(1);
      etl::atomic<int> test(1);

      CHECK_EQUAL((int)compare.load(), (int)test.load());
    }

    //*************************************************************************
    TEST(test_atomic_pointer_load)
    {
      int i = 1;

      std::atomic<int*> compare(&i);
      etl::atomic<int*> test(&i);

      CHECK_EQUAL((int*)compare.load(), (int*)test.load());
    }

    //*************************************************************************
    TEST(test_atomic_bool_load)
    {
      bool i = true;

      std::atomic<bool> compare(i);
      etl::atomic<bool> test(i);

      CHECK_EQUAL((bool)compare.load(), (bool)test.load());
    }

    //*************************************************************************
    TEST(test_atomic_integer_store)
    {
      std::atomic<int> compare(1);
      etl::atomic<int> test(1);

      compare.store(2);
      test.store(2);
      CHECK_EQUAL((int)compare.load(), (int)test.load());
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
      CHECK_EQUAL((int*)compare.load(), (int*)test.load());
    }

    //*************************************************************************
    TEST(test_atomic_bool_store)
    {
      bool i = true;
      bool j = false;

      std::atomic<bool> compare(i);
      etl::atomic<bool> test(i);

      compare.store(j);
      test.store(j);
      CHECK_EQUAL((bool)compare.load(), (bool)test.load());
    }

    //*************************************************************************
    TEST(test_atomic_integer_assignment)
    {
      std::atomic<int> compare(1);
      etl::atomic<int> test(1);

      compare = 2;
      test = 2;
      CHECK_EQUAL((int)compare.load(), (int)test.load());
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
      CHECK_EQUAL((int*)compare.load(), (int*)test.load());
    }

    //*************************************************************************
    TEST(test_atomic_bool_assignment)
    {
      bool i = true;
      bool j = false;

      std::atomic<bool> compare(i);
      etl::atomic<bool> test(i);

      compare = j;
      test = j;
      CHECK_EQUAL((bool)compare.load(), (bool)test.load());
    }

    //*************************************************************************
    TEST(test_atomic_operator_integer_pre_increment)
    {
      std::atomic<int> compare(1);
      etl::atomic<int> test(1);

      CHECK_EQUAL((int)++compare, (int)++test);
      CHECK_EQUAL((int)++compare, (int)++test);
    }

    //*************************************************************************
    TEST(test_atomic_operator_integer_post_increment)
    {
      std::atomic<int> compare(1);
      etl::atomic<int> test(1);

      CHECK_EQUAL((int)compare++, (int)test++);
      CHECK_EQUAL((int)compare++, (int)test++);
    }

    //*************************************************************************
    TEST(test_atomic_operator_integer_pre_decrement)
    {
      std::atomic<int> compare(1);
      etl::atomic<int> test(1);

      CHECK_EQUAL((int)--compare, (int)--test);
      CHECK_EQUAL((int)--compare, (int)--test);
    }

    //*************************************************************************
    TEST(test_atomic_operator_integer_post_decrement)
    {
      std::atomic<int> compare(1);
      etl::atomic<int> test(1);

      CHECK_EQUAL((int)compare--, (int)test--);
      CHECK_EQUAL((int)compare--, (int)test--);
    }

    //*************************************************************************
    TEST(test_atomic_operator_pointer_pre_increment)
    {
      int data[] = { 1, 2, 3, 4 };

      std::atomic<int*> compare(&data[0]);
      etl::atomic<int*> test(&data[0]);

      CHECK_EQUAL((int*)++compare, (int*)++test);
      CHECK_EQUAL((int*)++compare, (int*)++test);
    }

    //*************************************************************************
    TEST(test_atomic_operator_pointer_post_increment)
    {
      int data[] = { 1, 2, 3, 4 };

      std::atomic<int*> compare(&data[0]);
      etl::atomic<int*> test(&data[0]);

      CHECK_EQUAL((int*)compare++, (int*)test++);
      CHECK_EQUAL((int*)compare++, (int*)test++);
    }

    //*************************************************************************
    TEST(test_atomic_operator_pointer_pre_decrement)
    {
      int data[] = { 1, 2, 3, 4 };

      std::atomic<int*> compare(&data[3]);
      etl::atomic<int*> test(&data[3]);

      CHECK_EQUAL((int*)--compare, (int*)--test);
      CHECK_EQUAL((int*)--compare, (int*)--test);
    }

    //*************************************************************************
    TEST(test_atomic_operator_pointer_post_decrement)
    {
      int data[] = { 1, 2, 3, 4 };

      std::atomic<int*> compare(&data[3]);
      etl::atomic<int*> test(&data[3]);

      CHECK_EQUAL((int*)compare--, (int*)test--);
      CHECK_EQUAL((int*)compare--, (int*)test--);
    }

    //*************************************************************************
    TEST(test_atomic_operator_integer_fetch_add)
    {
      std::atomic<int> compare(1);
      etl::atomic<int> test(1);

      CHECK_EQUAL((int)compare.fetch_add(2), (int)test.fetch_add(2));
    }

    //*************************************************************************
    TEST(test_atomic_operator_pointer_fetch_add)
    {
      int data[] = { 1, 2, 3, 4 };

      std::atomic<int*> compare(&data[0]);
      etl::atomic<int*> test(&data[0]);

      CHECK_EQUAL((int*)compare.fetch_add(ptrdiff_t(10)), (int*)test.fetch_add(ptrdiff_t(10)));
    }

    //*************************************************************************
    TEST(test_atomic_operator_integer_plus_equals)
    {
      std::atomic<int> compare(1);
      etl::atomic<int> test(1);

      compare += 2;
      test += 2;

      CHECK_EQUAL((int)compare, (int)test);
    }

    //*************************************************************************
    TEST(test_atomic_operator_pointer_plus_equals)
    {
      int data[] = { 1, 2, 3, 4 };

      std::atomic<int*> compare(&data[0]);
      etl::atomic<int*> test(&data[0]);

      compare += 2;
      test += 2;

      CHECK_EQUAL((int*)compare, (int*)test);
    }

    //*************************************************************************
    TEST(test_atomic_operator_integer_minus_equals)
    {
      std::atomic<int> compare(1);
      etl::atomic<int> test(1);

      compare -= 2;
      test -= 2;

      CHECK_EQUAL((int)compare, (int)test);
    }

    //*************************************************************************
    TEST(test_atomic_operator_pointer_minus_equals)
    {
      int data[] = { 1, 2, 3, 4 };

      std::atomic<int*> compare(&data[3]);
      etl::atomic<int*> test(&data[3]);

      compare -= 2;
      test -= 2;

      CHECK_EQUAL((int*)compare, (int*)test);
    }

    //*************************************************************************
    TEST(test_atomic_operator_integer_and_equals)
    {
      std::atomic<int> compare(0x0000FFFFUL);
      etl::atomic<int> test(0x0000FFFFUL);

      compare &= 0x55AA55AAUL;
      test &= 0x55AA55AAUL;

      CHECK_EQUAL((int)compare, (int)test);
    }

    //*************************************************************************
    TEST(test_atomic_operator_integer_or_equals)
    {
      std::atomic<int> compare(0x0000FFFFUL);
      etl::atomic<int> test(0x0000FFFFUL);

      compare |= 0x55AA55AAUL;
      test |= 0x55AA55AAUL;

      CHECK_EQUAL((int)compare, (int)test);
    }

    //*************************************************************************
    TEST(test_atomic_operator_integer_xor_equals)
    {
      std::atomic<int> compare(0x0000FFFFUL);
      etl::atomic<int> test(0x0000FFFFUL);

      compare ^= 0x55AA55AAUL;
      test ^= 0x55AA55AAUL;

      CHECK_EQUAL((int)compare, (int)test);
    }

    //*************************************************************************
    TEST(test_atomic_operator_integer_fetch_sub)
    {
      std::atomic<int> compare(1);
      etl::atomic<int> test(1);

      CHECK_EQUAL((int)compare.fetch_sub(2), (int)test.fetch_sub(2));
    }

    //*************************************************************************
    TEST(test_atomic_operator_pointer_fetch_sub)
    {
      int data[] = { 1, 2, 3, 4 };

      std::atomic<int*> compare(&data[0]);
      etl::atomic<int*> test(&data[0]);

      CHECK_EQUAL((int*)compare.fetch_add(ptrdiff_t(10)), (int*)test.fetch_add(ptrdiff_t(10)));
    }

    //*************************************************************************
    TEST(test_atomic_operator_fetch_and)
    {
      std::atomic<int> compare(0xFFFFFFFFUL);
      etl::atomic<int> test(0xFFFFFFFFUL);

      CHECK_EQUAL((int)compare.fetch_and(0x55AA55AAUL), (int)test.fetch_and(0x55AA55AAUL));
    }

    //*************************************************************************
    TEST(test_atomic_operator_fetch_or)
    {
      std::atomic<int> compare(0x0000FFFFUL);
      etl::atomic<int> test(0x0000FFFFUL);

      CHECK_EQUAL((int)compare.fetch_or(0x55AA55AAUL), (int)test.fetch_or(0x55AA55AAUL));
    }

    //*************************************************************************
    TEST(test_atomic_operator_fetch_xor)
    {
      std::atomic<int> compare(0x0000FFFFUL);
      etl::atomic<int> test(0x0000FFFFUL);

      CHECK_EQUAL((int)compare.fetch_xor(0x55AA55AAUL), (int)test.fetch_xor(0x55AA55AAUL));
    }

    //*************************************************************************
    TEST(test_atomic_integer_exchange)
    {
      std::atomic<int> compare(1);
      etl::atomic<int> test(1);

      CHECK_EQUAL((int)compare.exchange(2), (int)test.exchange(2));
    }

    //*************************************************************************
    TEST(test_atomic_pointer_exchange)
    {
      int i;
      int j;

      std::atomic<int*> compare(&i);
      etl::atomic<int*> test(&i);

      CHECK_EQUAL((int*)compare.exchange(&j), (int*)test.exchange(&j));
    }

    //*************************************************************************
    TEST(test_atomic_bool_exchange)
    {
      bool i = true;
      bool j = false;

      std::atomic<bool> compare(i);
      etl::atomic<bool> test(i);

      CHECK_EQUAL((bool)compare.exchange(j), (bool)test.exchange(j));
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

#endif
