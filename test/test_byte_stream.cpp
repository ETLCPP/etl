/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl
https://www.etlcpp.com

Copyright(c) 2021 jwellbelove

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

#include "etl/byte_stream.h"
#include "etl/optional.h"

#include <array>
#include <numeric>

namespace
{
  //***********************************
  struct Object
  {
    etl::optional<int16_t> i;
    etl::optional<double>  d;
    etl::optional<uint8_t> c;
  };

  bool operator ==(const Object& lhs, const Object& rhs)
  {
    return (lhs.i == rhs.i) &&
           (lhs.d == rhs.d) &&
           (lhs.c == rhs.c);
  }

  std::ostream& operator << (std::ostream& os, const Object& object)
  {
    os << object.i.value() << "," << object.d.value() << "," << (int)object.c.value();
    return os;
  }
}

namespace etl
{
  //***********************************
  bool byte_stream_write(etl::byte_stream_writer& stream, const Object& object)
  {
    bool success = true;

    if (!stream.write(object.i.value()))
    {
      success = false;
    }

    if (!stream.write(object.d.value()))
    {
      success = false;
    }

    if (!stream.write(object.c.value()))
    {
      success = false;
    }

    return success;
  }

  //***********************************
  bool byte_stream_read(etl::byte_stream_reader& stream, Object& object)
  {
    bool success = true;

    if (object.i = stream.read<int16_t>())
    {
      success = false;
    }

    if (object.d = stream.read<double>())
    {
      success = false;
    }

    if (object.c = stream.read<uint8_t>())
    {
      success = false;
    }

    return success;
  }
}

namespace
{
  SUITE(test_byte_stream)
  {
    //*************************************************************************
    TEST(write_bool)
    {
      char storage[8];
      char result[8] = { 0, 1, 0, 1, 1, 0, 1, 0 };
      
      etl::byte_stream_writer byte_stream(storage, 8U);

      CHECK(byte_stream.write(false));
      CHECK(byte_stream.write(true));
      CHECK(byte_stream.write(false));
      CHECK(byte_stream.write(true));
      CHECK(byte_stream.write(true));
      CHECK(byte_stream.write(false));
      CHECK(byte_stream.write(true));
      CHECK(byte_stream.write(false));

      // One too many.
      CHECK(!byte_stream.write(true));

      CHECK_ARRAY_EQUAL(result, storage, 8);
    }

    //*************************************************************************
    TEST(write_int8_t)
    {
      std::array<char, 256> storage;

      std::array<char, 256> compare_data;
      std::iota(compare_data.begin(), compare_data.end(), 0);

      etl::byte_stream_writer byte_stream(storage.data(), storage.size());

      for (size_t i = 0; i < 256; ++i)
      {
        CHECK(byte_stream.write(int8_t(i)));
      }

      // One too many.
      CHECK(!byte_stream.write(int8_t(0)));

      for (size_t i = 0; i < storage.size(); ++i)
      {
        CHECK_EQUAL(int(compare_data[i]), int(storage[i]));
      }
    }

    //*************************************************************************
    TEST(write_uint8_t)
    {
      std::array<char, 256> storage;

      std::array<char, 256> compare_data;
      std::iota(compare_data.begin(), compare_data.end(), 0);

      etl::byte_stream_writer byte_stream(storage.data(), storage.size());

      for (size_t i = 0; i < 256; ++i)
      {
        CHECK(byte_stream.write(uint8_t(i)));
      }

      // One too many.
      CHECK(!byte_stream.write(uint8_t(0)));

      for (size_t i = 0; i < storage.size(); ++i)
      {
        CHECK_EQUAL(int(compare_data[i]), int(storage[i]));
      }
    }

    //*************************************************************************
    TEST(write_int16_t)
    {
      // Tests assume big endian.
      std::array<char, sizeof(int16_t) * 4> storage;
      std::array<char, sizeof(int16_t) * 4> compare_data = { char(0x00), char(0x01), char(0x5A), char(0xA5), char(0xA5), char(0x5A), char(0xFF), char(0xFF) };

      CHECK(compare_data.size() == storage.size());

      etl::byte_stream_writer byte_stream(storage.data(), storage.size());

      CHECK(byte_stream.write(int16_t(0x0001)));
      CHECK(byte_stream.write(int16_t(0x5AA5)));
      CHECK(byte_stream.write(int16_t(0xA55A)));
      CHECK(byte_stream.write(int16_t(0xFFFF)));

      // One too many.
      CHECK(!byte_stream.write(int16_t(0)));

      for (size_t i = 0; i < storage.size(); ++i)
      {
        CHECK_EQUAL(int(compare_data[i]), int(storage[i]));
      }
    }

    //*************************************************************************
    TEST(write_uint16_t)
    {
      // Tests assume big endian.
      std::array<char, sizeof(uint16_t) * 4> storage;
      std::array<char, sizeof(uint16_t) * 4> compare_data = { char(0x00), char(0x01), char(0x5A), char(0xA5), char(0xA5), char(0x5A), char(0xFF), char(0xFF) };

      CHECK(compare_data.size() == storage.size());

      etl::byte_stream_writer byte_stream(storage.data(), storage.size());

      CHECK(byte_stream.write(uint16_t(0x0001)));
      CHECK(byte_stream.write(uint16_t(0x5AA5)));
      CHECK(byte_stream.write(uint16_t(0xA55A)));
      CHECK(byte_stream.write(uint16_t(0xFFFF)));

      // One too many.
      CHECK(!byte_stream.write(uint16_t(0)));

      for (size_t i = 0; i < storage.size(); ++i)
      {
        CHECK_EQUAL(int(compare_data[i]), int(storage[i]));
      }
    }

    //*************************************************************************
    TEST(write_int32_t)
    {
      // Tests assume big endian.
      std::array<char, sizeof(int32_t) * 4> storage;
      std::array<char, sizeof(int32_t) * 4> compare_data = { char(0x00), char(0x00), char(0x00), char(0x01),
                                                             char(0x5A), char(0xA5), char(0xA5), char(0x5A),
                                                             char(0xA5), char(0x5A), char(0x5A), char(0xA5),
                                                             char(0xFF), char(0xFF), char(0xFF), char(0xFF) };

      CHECK(compare_data.size() == storage.size());

      etl::byte_stream_writer byte_stream(storage.data(), storage.size());

      CHECK(byte_stream.write(int32_t(0x00000001)));
      CHECK(byte_stream.write(int32_t(0x5AA5A55A)));
      CHECK(byte_stream.write(int32_t(0xA55A5AA5)));
      CHECK(byte_stream.write(int32_t(0xFFFFFFFF)));

      // One too many.
      CHECK(!byte_stream.write(int32_t(0)));

      for (size_t i = 0; i < storage.size(); ++i)
      {
        CHECK_EQUAL(int(compare_data[i]), int(storage[i]));
      }
    }

    //*************************************************************************
    TEST(write_uint32_t)
    {
      // Tests assume big endian.
      std::array<char, sizeof(uint32_t) * 4> storage;
      std::array<char, sizeof(uint32_t) * 4> compare_data = { char(0x00), char(0x00), char(0x00), char(0x01),
                                                              char(0x5A), char(0xA5), char(0xA5), char(0x5A),
                                                              char(0xA5), char(0x5A), char(0x5A), char(0xA5),
                                                              char(0xFF), char(0xFF), char(0xFF), char(0xFF) };

      CHECK(compare_data.size() == storage.size());

      etl::byte_stream_writer byte_stream(storage.data(), storage.size());

      CHECK(byte_stream.write(uint32_t(0x00000001)));
      CHECK(byte_stream.write(uint32_t(0x5AA5A55A)));
      CHECK(byte_stream.write(uint32_t(0xA55A5AA5)));
      CHECK(byte_stream.write(uint32_t(0xFFFFFFFF)));

      // One too many.
      CHECK(!byte_stream.write(uint32_t(0)));

      for (size_t i = 0; i < storage.size(); ++i)
      {
        CHECK_EQUAL(int(compare_data[i]), int(storage[i]));
      }
    }

    //*************************************************************************
    TEST(write_int64_t)
    {
      // Tests assume big endian.
      std::array<char, sizeof(int64_t) * 4> storage;
      std::array<char, sizeof(int64_t) * 4> compare_data = { char(0x00), char(0x00), char(0x00), char(0x00), char(0x00), char(0x00), char(0x00), char(0x01),
                                                             char(0x5A), char(0xA5), char(0xA5), char(0x5A), char(0xA5), char(0x5A), char(0x5A), char(0xA5),
                                                             char(0xA5), char(0x5A), char(0x5A), char(0xA5), char(0x5A), char(0xA5), char(0xA5), char(0x5A),
                                                             char(0xFF), char(0xFF), char(0xFF), char(0xFF), char(0xFF), char(0xFF), char(0xFF), char(0xFF) };

      CHECK(compare_data.size() == storage.size());

      etl::byte_stream_writer byte_stream(storage.data(), storage.size());

      CHECK(byte_stream.write(int64_t(0x0000000000000001)));
      CHECK(byte_stream.write(int64_t(0x5AA5A55AA55A5AA5)));
      CHECK(byte_stream.write(int64_t(0xA55A5AA55AA5A55A)));
      CHECK(byte_stream.write(int64_t(0xFFFFFFFFFFFFFFFF)));

      // One too many.
      CHECK(!byte_stream.write(int64_t(0)));

      for (size_t i = 0; i < storage.size(); ++i)
      {
        CHECK_EQUAL(int(compare_data[i]), int(storage[i]));
      }
    }

    //*************************************************************************
    TEST(write_uint64_t)
    {
      // Tests assume big endian.
      std::array<char, sizeof(uint64_t) * 4> storage;
      std::array<char, sizeof(uint64_t) * 4> compare_data = { char(0x00), char(0x00), char(0x00), char(0x00), char(0x00), char(0x00), char(0x00), char(0x01),
                                                              char(0x5A), char(0xA5), char(0xA5), char(0x5A), char(0xA5), char(0x5A), char(0x5A), char(0xA5),
                                                              char(0xA5), char(0x5A), char(0x5A), char(0xA5), char(0x5A), char(0xA5), char(0xA5), char(0x5A),
                                                              char(0xFF), char(0xFF), char(0xFF), char(0xFF), char(0xFF), char(0xFF), char(0xFF), char(0xFF) };

      CHECK(compare_data.size() == storage.size());

      etl::byte_stream_writer byte_stream(storage.data(), storage.size());

      CHECK(byte_stream.write(uint64_t(0x0000000000000001)));
      CHECK(byte_stream.write(uint64_t(0x5AA5A55AA55A5AA5)));
      CHECK(byte_stream.write(uint64_t(0xA55A5AA55AA5A55A)));
      CHECK(byte_stream.write(uint64_t(0xFFFFFFFFFFFFFFFF)));

      // One too many.
      CHECK(!byte_stream.write(uint64_t(0)));

      for (size_t i = 0; i < storage.size(); ++i)
      {
        CHECK_EQUAL(int(compare_data[i]), int(storage[i]));
      }
    }

    //*************************************************************************
    TEST(write_read_bool)
    {
      std::array<bool, 8> flags =
      {
        false, true, false, true, true, false, true, false
      };

      char storage[8];
      etl::byte_stream_writer byte_stream_w(storage, 8);

      for (size_t i = 0; i < flags.size(); ++i)
      {
        byte_stream_w.write(flags[i]);
      }

      etl::byte_stream_reader byte_stream_r(storage, 8);

      for (size_t i = 0; i < flags.size(); ++i)
      {
        etl::optional<bool> flag;

        CHECK(flag = byte_stream_r.read<bool>());
        CHECK_EQUAL(flags[i], flag.value());
      }
    }

    //*************************************************************************
    TEST(write_read_int8_t)
    {
      std::array<char, 4 * sizeof(int8_t)> storage;
      std::array<int8_t, 4> put_data = { int8_t(0x01), int8_t(0x5A), int8_t(0xA5), int8_t(0xFF) };
      std::array<etl::optional<int8_t>, 4> get_data = { int8_t(0x00), int8_t(0x00), int8_t(0x00), int8_t(0x00) };

      etl::byte_stream_writer byte_stream_w(storage.data(), storage.size());

      // Insert into the stream
      byte_stream_w.write(put_data[0]);
      byte_stream_w.write(put_data[1]);
      byte_stream_w.write(put_data[2]);
      byte_stream_w.write(put_data[3]);

      etl::byte_stream_reader byte_stream_r(storage.data(), storage.size());

      CHECK(get_data[0] = byte_stream_r.read<int8_t>());
      CHECK_EQUAL(put_data[0], int(get_data[0].value()));

      CHECK(get_data[1] = byte_stream_r.read<int8_t>());
      CHECK_EQUAL(put_data[1], int(get_data[1].value()));

      CHECK(get_data[2] = byte_stream_r.read<int8_t>());
      CHECK_EQUAL(put_data[2], int(get_data[2].value()));

      CHECK(get_data[3] = byte_stream_r.read<int8_t>());
      CHECK_EQUAL(put_data[3], int(get_data[3].value()));
    }

    //*************************************************************************
    TEST(write_read_uint8_t)
    {
      std::array<char, 4 * sizeof(uint8_t)> storage;
      std::array<uint8_t, 4> put_data = { uint8_t(0x01), uint8_t(0x5A), uint8_t(0xA5), uint8_t(0xFF) };
      std::array<etl::optional<uint8_t>, 4> get_data = { uint8_t(0x00), uint8_t(0x00), uint8_t(0x00), uint8_t(0x00) };

      etl::byte_stream_writer byte_stream_w(storage.data(), storage.size());

      // Insert into the stream
      byte_stream_w.write(put_data[0]);
      byte_stream_w.write(put_data[1]);
      byte_stream_w.write(put_data[2]);
      byte_stream_w.write(put_data[3]);

      etl::byte_stream_reader byte_stream_r(storage.data(), storage.size());

      CHECK(get_data[0] = byte_stream_r.read<uint8_t>());
      CHECK_EQUAL(put_data[0], int(get_data[0].value()));

      CHECK(get_data[1] = byte_stream_r.read<uint8_t>());
      CHECK_EQUAL(put_data[1], int(get_data[1].value()));

      CHECK(get_data[2] = byte_stream_r.read<uint8_t>());
      CHECK_EQUAL(put_data[2], int(get_data[2].value()));

      CHECK(get_data[3] = byte_stream_r.read<uint8_t>());
      CHECK_EQUAL(put_data[3], int(get_data[3].value()));
    }

    ////*************************************************************************
    //TEST(put_get_int16_t)
    //{
    //  std::array<char, 4 * sizeof(int16_t)> storage;
    //  std::array<int16_t, 4> put_data = { int16_t(0x0001), int16_t(0xA55A), int16_t(0x5AA5), int16_t(0xFFFF) };
    //  std::array<int16_t, 4> get_data = { int16_t(0x0000), int16_t(0x0000), int16_t(0x0000), int16_t(0x0000) };

    //  etl::bit_stream bit_stream(storage.data(), storage.size());

    //  // Insert into the stream
    //  bit_stream.put(put_data[0]);
    //  bit_stream.put(put_data[1]);
    //  bit_stream.put(put_data[2]);
    //  bit_stream.put(put_data[3]);

    //  bit_stream.restart();

    //  CHECK(bit_stream.get(get_data[0]));
    //  CHECK_EQUAL(put_data[0], get_data[0]);

    //  CHECK(bit_stream.get(get_data[1]));
    //  CHECK_EQUAL(put_data[1], get_data[1]);

    //  CHECK(bit_stream.get(get_data[2]));
    //  CHECK_EQUAL(put_data[2], get_data[2]);

    //  CHECK(bit_stream.get(get_data[3]));
    //  CHECK_EQUAL(put_data[3], get_data[3]);
    //}

    ////*************************************************************************
    //TEST(put_get_uint16_t)
    //{
    //  std::array<char, 4 * sizeof(uint16_t)> storage;
    //  std::array<uint16_t, 4> put_data = { uint16_t(0x0001), uint16_t(0xA55A), uint16_t(0x5AA5), uint16_t(0xFFFF) };
    //  std::array<uint16_t, 4> get_data = { uint16_t(0x0000), uint16_t(0x0000), uint16_t(0x0000), uint16_t(0x0000) };

    //  etl::bit_stream bit_stream(storage.data(), storage.size());

    //  // Insert into the stream
    //  bit_stream.put(put_data[0]);
    //  bit_stream.put(put_data[1]);
    //  bit_stream.put(put_data[2]);
    //  bit_stream.put(put_data[3]);

    //  bit_stream.restart();

    //  CHECK(bit_stream.get(get_data[0]));
    //  CHECK_EQUAL(put_data[0], get_data[0]);

    //  CHECK(bit_stream.get(get_data[1]));
    //  CHECK_EQUAL(put_data[1], get_data[1]);

    //  CHECK(bit_stream.get(get_data[2]));
    //  CHECK_EQUAL(put_data[2], get_data[2]);

    //  CHECK(bit_stream.get(get_data[3]));
    //  CHECK_EQUAL(put_data[3], get_data[3]);
    //}

    ////*************************************************************************
    //TEST(put_get_int32_t)
    //{
    //  std::array<char, 4 * sizeof(int32_t)> storage;
    //  std::array<int32_t, 4> put_data = { int32_t(0x00000001), int32_t(0xA55AA55A), int32_t(0x5AA55AA5), int32_t(0xFFFFFFFF) };
    //  std::array<int32_t, 4> get_data = { int32_t(0x00000000), int32_t(0x00000000), int32_t(0x00000000), int32_t(0x00000000) };

    //  etl::bit_stream bit_stream(storage.data(), storage.size());

    //  // Insert into the stream
    //  bit_stream.put(put_data[0]);
    //  bit_stream.put(put_data[1]);
    //  bit_stream.put(put_data[2]);
    //  bit_stream.put(put_data[3]);

    //  bit_stream.restart();

    //  CHECK(bit_stream.get(get_data[0]));
    //  CHECK_EQUAL(put_data[0], get_data[0]);

    //  CHECK(bit_stream.get(get_data[1]));
    //  CHECK_EQUAL(put_data[1], get_data[1]);

    //  CHECK(bit_stream.get(get_data[2]));
    //  CHECK_EQUAL(put_data[2], get_data[2]);

    //  CHECK(bit_stream.get(get_data[3]));
    //  CHECK_EQUAL(put_data[3], get_data[3]);
    //}

    ////*************************************************************************
    //TEST(put_get_uint32_t)
    //{
    //  std::array<char, 4 * sizeof(uint32_t)> storage;
    //  std::array<uint32_t, 4> put_data = { uint32_t(0x00000001), uint32_t(0xA55AA55A), uint32_t(0x5AA55AA5), uint32_t(0xFFFFFFFF) };
    //  std::array<uint32_t, 4> get_data = { uint32_t(0x00000000), uint32_t(0x00000000), uint32_t(0x00000000), uint32_t(0x00000000) };

    //  etl::bit_stream bit_stream(storage.data(), storage.size());

    //  // Insert into the stream
    //  bit_stream.put(put_data[0]);
    //  bit_stream.put(put_data[1]);
    //  bit_stream.put(put_data[2]);
    //  bit_stream.put(put_data[3]);

    //  bit_stream.restart();

    //  CHECK(bit_stream.get(get_data[0]));
    //  CHECK_EQUAL(put_data[0], get_data[0]);

    //  CHECK(bit_stream.get(get_data[1]));
    //  CHECK_EQUAL(put_data[1], get_data[1]);

    //  CHECK(bit_stream.get(get_data[2]));
    //  CHECK_EQUAL(put_data[2], get_data[2]);

    //  CHECK(bit_stream.get(get_data[3]));
    //  CHECK_EQUAL(put_data[3], get_data[3]);
    //}

    ////*************************************************************************
    //TEST(put_get_int64_t)
    //{
    //  std::array<char, 4 * sizeof(int64_t)> storage;
    //  std::array<int64_t, 4> put_data    = { int64_t(0x0000000000000001), int64_t(0xA55AA55AA55AA55A), int64_t(0x5AA55AA55AA55AA5), int64_t(0xFFFFFFFFFFFFFFFF) };
    //  std::array<int64_t, 4> expect_data = { int64_t(0x0000000000000001), int64_t(0xA55AA55AA55AA55A), int64_t(0x5AA55AA55AA55AA5), int64_t(0xFFFFFFFFFFFFFFFF) };
    //  std::array<int64_t, 4> get_data    = { int64_t(0x0000000000000000), int64_t(0x0000000000000000), int64_t(0x0000000000000000), int64_t(0x0000000000000000) };

    //  etl::bit_stream bit_stream(storage.data(), storage.size());

    //  // Insert into the stream
    //  bit_stream.put(put_data[0]);
    //  bit_stream.put(put_data[1]);
    //  bit_stream.put(put_data[2]);
    //  bit_stream.put(put_data[3]);

    //  bit_stream.restart();

    //  CHECK(bit_stream.get(get_data[0]));
    //  CHECK_EQUAL(expect_data[0], get_data[0]);

    //  CHECK(bit_stream.get(get_data[1]));
    //  CHECK_EQUAL(expect_data[1], get_data[1]);

    //  CHECK(bit_stream.get(get_data[2]));
    //  CHECK_EQUAL(expect_data[2], get_data[2]);

    //  CHECK(bit_stream.get(get_data[3]));
    //  CHECK_EQUAL(expect_data[3], get_data[3]);
    //}

    ////*************************************************************************
    //TEST(put_get_uint64_t)
    //{
    //  std::array<char, 4 * sizeof(uint64_t)> storage;
    //  std::array<uint64_t, 4> put_data    = { uint64_t(0x0000000000000001), uint64_t(0xA55AA55AA55AA55A), uint64_t(0x5AA55AA55AA55AA5), uint64_t(0xFFFFFFFFFFFFFFFF) };
    //  std::array<uint64_t, 4> expect_data = { uint64_t(0x0000000000000001), uint64_t(0xA55AA55AA55AA55A), uint64_t(0x5AA55AA55AA55AA5), uint64_t(0xFFFFFFFFFFFFFFFF) };
    //  std::array<uint64_t, 4> get_data    = { uint64_t(0x0000000000000000), uint64_t(0x0000000000000000), uint64_t(0x0000000000000000), uint64_t(0x0000000000000000) };

    //  etl::bit_stream bit_stream(storage.data(), storage.size());

    //  // Insert into the stream
    //  bit_stream.put(put_data[0]);
    //  bit_stream.put(put_data[1]);
    //  bit_stream.put(put_data[2]);
    //  bit_stream.put(put_data[3]);

    //  bit_stream.restart();

    //  CHECK(bit_stream.get(get_data[0]));
    //  CHECK_EQUAL(expect_data[0], get_data[0]);

    //  CHECK(bit_stream.get(get_data[1]));
    //  CHECK_EQUAL(expect_data[1], get_data[1]);

    //  CHECK(bit_stream.get(get_data[2]));
    //  CHECK_EQUAL(expect_data[2], get_data[2]);

    //  CHECK(bit_stream.get(get_data[3]));
    //  CHECK_EQUAL(expect_data[3], get_data[3]);
    //}

    ////*************************************************************************
    //TEST(put_get_multiple_full_size)
    //{
    //  char c1 = 90;
    //  char c2 = -91;
    //  unsigned short s1 = 23205;
    //  unsigned short s2 = 42330;
    //  int32_t i1 = 1520786085;   // 0x5AA55AA5
    //  int32_t i2 = -1520786086;  // 0xA55AA55A
    //  float f = 3.1415927f;
    //  double d = 3.1415927;

    //  std::array<char, 100> storage;

    //  etl::bit_stream bit_stream(storage.data(), storage.size());

    //  // Insert into the stream.
    //  bit_stream.put(c1);
    //  bit_stream.put(s1);
    //  bit_stream.put(i1);
    //  bit_stream.put(f);
    //  bit_stream.put(i2);
    //  bit_stream.put(d);
    //  bit_stream.put(s2);
    //  bit_stream.put(c2);

    //  bit_stream.restart();

    //  char rc1;
    //  char rc2;
    //  unsigned short rs1;
    //  unsigned short rs2;
    //  int32_t ri1;
    //  int32_t ri2;
    //  float rf;
    //  double rd;

    //  // Read them all back.
    //  CHECK(bit_stream.get(rc1));
    //  CHECK_EQUAL(int(c1), int(rc1));

    //  CHECK(bit_stream.get(rs1));
    //  CHECK_EQUAL(s1, rs1);

    //  CHECK(bit_stream.get(ri1));
    //  CHECK_EQUAL(i1, ri1);

    //  CHECK(bit_stream.get(rf));
    //  CHECK_CLOSE(f, rf, 0.1f);

    //  CHECK(bit_stream.get(ri2));
    //  CHECK_EQUAL(i2, ri2);

    //  CHECK(bit_stream.get(rd));
    //  CHECK_CLOSE(d, rd, 0.1);

    //  CHECK(bit_stream.get(rs2));
    //  CHECK_EQUAL(s2, rs2);

    //  CHECK(bit_stream.get(rc2));
    //  CHECK_EQUAL(int(c2), int(rc2));
    //}

    ////*************************************************************************
    //TEST(put_get_object_global)
    //{
    //  std::array<char, 2 * sizeof(Object)> storage;

    //  etl::bit_stream bit_stream(storage.data(), storage.size());

    //  Object object1 = { -1234, 2.71578369, 250 };
    //  Object object2 = {  5678, 5.24685744, 126 };

    //  CHECK(etl::bit_stream_put(bit_stream, object1));
    //  CHECK(etl::bit_stream_put(bit_stream, object2));

    //  Object object1a;
    //  Object object2a;

    //  bit_stream.restart();

    //  CHECK(etl::bit_stream_get(bit_stream, object1a));
    //  CHECK(etl::bit_stream_get(bit_stream, object2a));

    //  CHECK_EQUAL(object1, object1a);
    //  CHECK_EQUAL(object2, object2a);
    //}

    //*************************************************************************
    TEST(write_read_multiple_float)
    {

      float  f = 3.1415927f;
      double d = 3.1415927;

      std::array<char, 12> storage;

      etl::byte_stream_writer byte_stream_w(storage.data(), storage.size());

      byte_stream_w.write(f);
      byte_stream_w.write(d);

      etl::byte_stream_reader byte_stream_r(storage.data(), storage.size());

      etl::optional<float>  rf;
      etl::optional<double> rd;

      CHECK(rf = byte_stream_r.read<float>());
      CHECK_CLOSE(f, rf.value(), 0.1f);

      CHECK(rd = byte_stream_r.read<double>());
      CHECK_CLOSE(d, rd.value(), 0.1);
    }
  };
}
