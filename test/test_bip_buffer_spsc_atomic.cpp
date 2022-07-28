/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl
https://www.etlcpp.com

Copyright(c) 2021 John Wellbelove

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

#include <thread>
#include <chrono>
#include <vector>

#include "etl/bip_buffer_spsc_atomic.h"

#include "data.h"

#if ETL_HAS_ATOMIC

#if defined(ETL_TARGET_OS_WINDOWS)
  #include <Windows.h>
#endif

#define REALTIME_TEST 0

namespace
{
  SUITE(test_bip_buffer_spsc_atomic)
  {
    //*************************************************************************
    TEST(test_constructor)
    {
      etl::bip_buffer_spsc_atomic<int, 5> stream;

      CHECK_EQUAL(5U, stream.max_size());
      CHECK_EQUAL(5U, stream.capacity());
    }

    //*************************************************************************
    TEST(test_size_write_read)
    {
      etl::bip_buffer_spsc_atomic<int, 5> stream;
      etl::ibip_buffer_spsc_atomic<int>& istream = stream;

      // Verify empty buffer
      CHECK_EQUAL(0U, stream.size());
      CHECK(stream.empty());
      CHECK((stream.max_size() / 2U) <= stream.available());
      CHECK(stream.available() <= stream.max_size());

      auto reader = istream.read_reserve(1U);
      CHECK_EQUAL(0U, reader.size());

      // Write partially
      auto writer = istream.write_reserve(1U);
      CHECK_EQUAL(1U, writer.size());
      writer[0] = 1;

      CHECK(stream.empty());

      istream.write_commit(writer); // 1 * * * *
      CHECK_EQUAL(1U, stream.size());

      writer = istream.write_reserve(1U);
      CHECK_EQUAL(1U, writer.size());
      writer[0] = 2;

      istream.write_commit(writer); // 1 2 * * *
      CHECK_EQUAL(2U, stream.size());

      // Write to capacity
      writer = istream.write_reserve(istream.available());
      CHECK_EQUAL(3U, writer.size());
      writer[0] = 3;
      writer[1] = 4;
      writer[2] = 5;

      istream.write_commit(writer); // 1 2 3 4 5

      // Verify full buffer
      CHECK_EQUAL(0U, stream.available());
      CHECK(stream.full());
      CHECK((stream.max_size() - 1) <= stream.size());
      CHECK(stream.size() <= stream.max_size());

      writer = istream.write_reserve(1U);
      CHECK_EQUAL(0U, writer.size());

      // Read partially
      reader = istream.read_reserve(1U);
      CHECK_EQUAL(1U, reader.size());
      CHECK_EQUAL(1, reader[0]);
      CHECK_EQUAL(5U, stream.size());

      istream.read_commit(reader); // * 2 3 4 5
      CHECK_EQUAL(4U, stream.size());

      reader = istream.read_reserve(1U);
      CHECK_EQUAL(1U, reader.size());
      CHECK_EQUAL(2, reader[0]);
      CHECK_EQUAL(4U, stream.size());

      istream.read_commit(reader); // * * 3 4 5
      CHECK_EQUAL(3U, stream.size());

      // Write to wraparound area (one element remains reserved)
      writer = istream.write_reserve(istream.available());
      CHECK_EQUAL(1U, writer.size());
      CHECK_EQUAL(1U, stream.available());
      writer[0] = 6;

      istream.write_commit(writer); // 6 * 3 4 5

      // Verify full buffer
      CHECK_EQUAL(0U, stream.available());
      CHECK(stream.full());
      CHECK((stream.max_size() - 1) <= stream.size());
      CHECK(stream.size() <= stream.max_size());

      writer = istream.write_reserve(1U);
      CHECK_EQUAL(0U, writer.size());

      // Read to capacity
      reader = istream.read_reserve();
      CHECK_EQUAL(3U, reader.size());
      CHECK_EQUAL(3, reader[0]);
      CHECK_EQUAL(4, reader[1]);
      CHECK_EQUAL(5, reader[2]);
      CHECK_EQUAL(4U, stream.size());

      istream.read_commit(reader); // * 2 * * *
      CHECK_EQUAL(1U, stream.size());

      reader = istream.read_reserve();
      CHECK_EQUAL(1U, reader.size());
      CHECK_EQUAL(6, reader[0]);
      CHECK_EQUAL(1U, stream.size());

      istream.read_commit(reader); // * * * * *

      // Verify empty buffer
      CHECK_EQUAL(0U, stream.size());
      CHECK(stream.empty());
      CHECK((stream.max_size() / 2U) <= stream.available());
      CHECK(stream.available() <= stream.max_size());
    }

    //*************************************************************************
    TEST(test_optimal_write)
    {
        etl::bip_buffer_spsc_atomic<int, 5> stream;
        etl::ibip_buffer_spsc_atomic<int>& istream = stream;

        // Prepare buffer for bipartite split
        auto writer = istream.write_reserve_optimal();
        CHECK_EQUAL(5U, writer.size());
        writer[0] = 1;
        writer[1] = 2;
        writer[2] = 3;
        writer[3] = 4;
        istream.write_commit(writer.subspan(0U, 4U)); // 1 2 3 4 *

        auto reader = istream.read_reserve(3U);
        istream.read_commit(reader); // * * * 4 *
        CHECK_EQUAL(1U, stream.size());
        CHECK_EQUAL(2U, stream.available());

        // Write to remaining linear area
        writer = istream.write_reserve_optimal();
        CHECK_EQUAL(1U, writer.size());
        writer[0] = 5;

        istream.write_commit(writer); // * * * 4 5

        // Read to capacity
        reader = istream.read_reserve();
        CHECK_EQUAL(2U, reader.size());
        CHECK_EQUAL(4, reader[0]);
        CHECK_EQUAL(5, reader[1]);

        istream.read_commit(reader); // * * * * *

        // Verify empty buffer
        CHECK_EQUAL(0U, stream.size());
        CHECK(stream.empty());
        CHECK((stream.max_size() / 2U) <= stream.available());
        CHECK(stream.available() <= stream.max_size());
    }

    //*************************************************************************
    TEST(test_clear)
    {
      etl::bip_buffer_spsc_atomic<int, 5> stream;
      etl::ibip_buffer_spsc_atomic<int>& istream = stream;

      CHECK(stream.empty());

      // Write the whole buffer
      auto writer = istream.write_reserve(istream.capacity());
      // data is committed without set to valid value (it won't be read anyway)
      istream.write_commit(writer);
      CHECK(stream.full());

      istream.clear();
      CHECK(stream.empty());

      // Repeat to see that clear() resets the internal state completely and correctly
      writer = istream.write_reserve(istream.capacity());
      // data is committed without set to valid value (it won't be read anyway)
      istream.write_commit(writer);
      CHECK(stream.full());

      istream.clear();
      CHECK(stream.empty());
    }

    //*************************************************************************
    TEST(test_partial_commits)
    {
      etl::bip_buffer_spsc_atomic<int, 5> stream;
      etl::ibip_buffer_spsc_atomic<int>& istream = stream;

      // Write reserve available
      auto writer_1 = istream.write_reserve(istream.capacity());
      CHECK_EQUAL(5U, stream.available());
      CHECK_EQUAL(5U, writer_1.size());

      // Write and commit partially
      writer_1[0] = 1;
      writer_1[1] = 2;
      writer_1[2] = 3;
      writer_1[3] = 4;

      // Cannot commit subspans with offset
      CHECK_THROW(istream.write_commit(writer_1.subspan(1U, 3U)), etl::bip_buffer_reserve_invalid);
      CHECK_THROW(istream.write_commit(writer_1.subspan(2U, 2U)), etl::bip_buffer_reserve_invalid);
      CHECK_THROW(istream.write_commit(writer_1.subspan(3U, 1U)), etl::bip_buffer_reserve_invalid);

      CHECK_NO_THROW(istream.write_commit(writer_1.subspan(0U, 4U))); // 1 2 3 4 *
      CHECK_EQUAL(4U, stream.size());

      // Can only commit once for each reserve (provided they don't cover a valid area)
      CHECK_THROW(istream.write_commit(writer_1), etl::bip_buffer_reserve_invalid);
      CHECK_THROW(istream.write_commit(writer_1.subspan(0U, 4U)), etl::bip_buffer_reserve_invalid);

      // Read reserve available
      auto reader_1 = istream.read_reserve(istream.capacity());
      CHECK_EQUAL(4U, reader_1.size());

      // Read and commit partially
      CHECK_EQUAL(1, reader_1[0]);
      CHECK_EQUAL(2, reader_1[1]);
      CHECK_EQUAL(3, reader_1[2]);

      // Cannot commit subspans with offset
      CHECK_THROW(istream.read_commit(reader_1.subspan(1U, 2U)), etl::bip_buffer_reserve_invalid);
      CHECK_THROW(istream.read_commit(reader_1.subspan(2U, 1U)), etl::bip_buffer_reserve_invalid);

      CHECK_NO_THROW(istream.read_commit(reader_1.subspan(0U, 3U))); // * * * 4 *
      CHECK_EQUAL(1U, stream.size());

      // Can only commit once for each reserve (provided they don't cover a valid area)
      CHECK_THROW(istream.read_commit(reader_1), etl::bip_buffer_reserve_invalid);
      CHECK_THROW(istream.read_commit(reader_1.subspan(0U, 3U)), etl::bip_buffer_reserve_invalid);

      // Write reserve available
      auto writer_2 = istream.write_reserve(istream.capacity());
      CHECK_EQUAL(2U, stream.available());
      CHECK_EQUAL(2U, writer_2.size());

      // Write and commit partially
      writer_2[0] = 5;
      CHECK_NO_THROW(istream.write_commit(writer_2.subspan(0U, 1U))); // 5 * * 4 *
      CHECK_EQUAL(2U, stream.size());
      // Even though the second committed span could have fit at the end,
      // the reservation asked for the largest consecutive block,
      // which resulted in a wraparound span to be allocated

      // Can only commit once for each reserve (provided they don't cover a valid area)
      CHECK_THROW(istream.write_commit(writer_1), etl::bip_buffer_reserve_invalid);
      CHECK_THROW(istream.write_commit(writer_2), etl::bip_buffer_reserve_invalid);
      CHECK_THROW(istream.write_commit(writer_2.subspan(0U, 1U)), etl::bip_buffer_reserve_invalid);

      // Read reserve available
      auto reader_2 = istream.read_reserve(istream.capacity());
      CHECK_EQUAL(1U, reader_2.size());

      // Read and commit
      CHECK_EQUAL(4, reader_2[0]);
      CHECK_NO_THROW(istream.read_commit(reader_2)); // 5 * * * *
      CHECK_EQUAL(1U, stream.size());

      // Can only commit once for each reserve (provided they don't cover a valid area)
      CHECK_THROW(istream.read_commit(reader_1), etl::bip_buffer_reserve_invalid);
      CHECK_THROW(istream.read_commit(reader_2), etl::bip_buffer_reserve_invalid);

      // Read reserve available
      auto reader_3 = istream.read_reserve(istream.capacity());
      CHECK_EQUAL(1U, reader_3.size());

      // Read and commit
      CHECK_EQUAL(5, reader_3[0]);
      CHECK_NO_THROW(istream.read_commit(reader_3)); // * * * * *
      CHECK_EQUAL(0U, stream.size());

      // Can only commit once for each reserve (provided they don't cover a valid area)
      CHECK_THROW(istream.read_commit(reader_1), etl::bip_buffer_reserve_invalid);
      CHECK_THROW(istream.read_commit(reader_2), etl::bip_buffer_reserve_invalid);
      CHECK_THROW(istream.read_commit(reader_3), etl::bip_buffer_reserve_invalid);

      CHECK(stream.empty());
    }

    //*************************************************************************
#if REALTIME_TEST && defined(ETL_COMPILER_MICROSOFT)
    #if defined(ETL_TARGET_OS_WINDOWS) // Only Windows priority is currently supported
      #define FIX_PROCESSOR_AFFINITY1 SetThreadAffinityMask(GetCurrentThread(), 1);
      #define FIX_PROCESSOR_AFFINITY2 SetThreadAffinityMask(GetCurrentThread(), 2);
    #else
      #error No thread priority modifier defined
    #endif

    etl::bip_buffer_spsc_atomic<int, 100> stream;

    const size_t LENGTH = 100UL;

    void timer_event()
    {
      FIX_PROCESSOR_AFFINITY1;

      const size_t write_chunk_size = 7UL;
      size_t tick = 0UL;

      while (tick < LENGTH)
      {
        auto writer = stream.write_reserve(min(write_chunk_size, LENGTH - tick));
        for (auto& item : writer)
        {
          item = tick++;
        }
        stream.write_commit(writer);
      }
    }

    TEST(bip_buffer_threads)
    {
      FIX_PROCESSOR_AFFINITY2;

      const size_t read_chunk_size = stream.capacity();

      std::vector<int> tick_list;
      tick_list.reserve(LENGTH);

      std::thread t1(timer_event);

      while (tick_list.size() < LENGTH)
      {
        etl::span<int> reader = stream.read_reserve(read_chunk_size);
        tick_list.insert(tick_list.end(), reader.begin(), reader.end());
        stream.read_commit(reader);
      }

      // Join the thread with the main thread
      t1.join();

      CHECK_EQUAL(LENGTH, tick_list.size());

      for (size_t i = 0UL; i < LENGTH; i++)
      {
        CHECK_EQUAL(i, tick_list[i]);
      }
    }
#endif
  };
}

#endif // ETL_HAS_ATOMIC
