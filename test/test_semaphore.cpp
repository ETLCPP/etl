/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl
https://www.etlcpp.com

Copyright(c) 2026 Julien Hecht

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

#include "clocks_for_unit_tests.h"
#include "unit_test_framework.h"

#include <thread>

#include <etl/semaphore.h>

using namespace test::clocks;

namespace
{
  SUITE(test_semaphore)
  {
    TEST(test_semaphore_counting_nominal)
    {
      int initial_count = 10;
      int release_count = 20;
      int acquire_count = initial_count + release_count;

      etl::counting_semaphore<> sem{initial_count};

      std::thread thread(
        [&sem, &release_count]()
        {
          while (release_count > 0)
          {
            sem.release();
            release_count--;
          }
        });

      while (acquire_count > 0)
      {
        sem.acquire();
        acquire_count--;
      }

      thread.join();

      CHECK_EQUAL(release_count, 0);
      CHECK_EQUAL(acquire_count, 0);
      CHECK_FALSE(sem.try_acquire());
    }

    TEST(test_semaphore_counting_try_acquire)
    {
      etl::counting_semaphore<> sem{1};

      CHECK_TRUE(sem.try_acquire());
      CHECK_FALSE(sem.try_acquire());
    }

    TEST(test_semaphore_counting_try_for)
    {
      etl::counting_semaphore<> sem{1};

      // try_for uses steady_clock in the `gcc_sync` implementation
      // (default backend if STL is not available or FreeRTOS is not enabled)
      // During testing, this means the clock used for waiting has a granularity of 1s.
      seconds = 0;
      std::thread thread(
        []()
        {
          // Just sleep a bit, not literally 1s, because that's a wait of time
          std::this_thread::sleep_for(std::chrono::milliseconds(20));
          seconds++;
        });

      CHECK_TRUE(sem.try_acquire_for(etl::chrono::milliseconds(10)));
      CHECK_FALSE(sem.try_acquire_for(etl::chrono::milliseconds(10)));

      thread.join();
    }

    TEST(test_semaphore_counting_try_until)
    {
      etl::counting_semaphore<> sem{1};

      nanoseconds = 0;
      std::thread thread(
        []()
        {
          // Count to 20 milliseconds
          for (int i = 0; i < 20; i++)
          {
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
            nanoseconds += 1000000;
          }
        });

      auto now = etl::chrono::high_resolution_clock::now();
      CHECK_TRUE(sem.try_acquire_until(now + etl::chrono::milliseconds(10)));
      CHECK_FALSE(sem.try_acquire_until(now + etl::chrono::milliseconds(10)));

      thread.join();
    }

    TEST(test_semaphore_binary_nominal)
    {
      etl::binary_semaphore sem{0};

      sem.release();
      sem.acquire();

      CHECK_FALSE(sem.try_acquire());
    }

    TEST(test_semaphore_binary_try_acquire)
    {
      etl::binary_semaphore sem{1};

      CHECK_TRUE(sem.try_acquire());
      CHECK_FALSE(sem.try_acquire());
    }

    TEST(test_semaphore_binary_try_for)
    {
      etl::binary_semaphore sem{1};

      // try_for uses steady_clock in the `gcc_sync` implementation
      // (default backend if STL is not available or FreeRTOS is not enabled)
      // During testing, this means the clock used for waiting has a granularity of 1s.
      seconds = 0;
      std::thread thread(
        []()
        {
          // Just sleep a bit, not literally 1s, because that's a waste of time
          std::this_thread::sleep_for(std::chrono::milliseconds(20));
          seconds++;
        });

      CHECK_TRUE(sem.try_acquire_for(etl::chrono::milliseconds(10)));
      CHECK_FALSE(sem.try_acquire_for(etl::chrono::milliseconds(10)));

      thread.join();
    }

    TEST(test_semaphore_binary_try_until)
    {
      etl::binary_semaphore sem{1};

      nanoseconds = 0;
      std::thread thread(
        []()
        {
          // Count to 20 milliseconds
          for (int i = 0; i < 20; i++)
          {
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
            nanoseconds += 1000000;
          }
        });

      auto now = etl::chrono::high_resolution_clock::now();
      CHECK_TRUE(sem.try_acquire_until(now + etl::chrono::milliseconds(10)));
      CHECK_FALSE(sem.try_acquire_until(now + etl::chrono::milliseconds(10)));

      thread.join();
    }
  }
} // namespace
