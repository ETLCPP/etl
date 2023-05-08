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

#include "etl/byte_stream.h"
#include "etl/optional.h"
#include "etl/span.h"

#include <array>
#include <numeric>
#include <vector>

namespace
{
  //***********************************
  struct Object
  {
    int16_t i;
    double  d;
    uint8_t c;
  };
}

namespace etl
{
  //***********************************
  template <>
  void write_unchecked<Object>(etl::byte_stream_writer& stream, const Object& object)
  {
    stream.write_unchecked(object.i);
    stream.write_unchecked(object.d);
    stream.write_unchecked(object.c);
  }

  //***********************************
  template <>
  bool write<Object>(etl::byte_stream_writer& stream, const Object& object)
  {
    bool success_i = stream.write(object.i);
    bool success_d = stream.write(object.d);
    bool success_c = stream.write(object.c);

    return success_i && success_d && success_c;
  }

  //***********************************
  template <>
  Object read_unchecked<Object>(etl::byte_stream_reader& stream)
  {
    int16_t i = stream.read_unchecked<int16_t>();
    double  d = stream.read_unchecked<double>();
    uint8_t c = stream.read_unchecked<uint8_t>();

    Object object{ i, d, c };

    return object;
  }

  //***********************************
  template <>
  etl::optional<Object> read<Object>(etl::byte_stream_reader& stream)
  {
    etl::optional<Object> result;

    etl::optional<int16_t> i = stream.read<int16_t>();
    etl::optional<double>  d = stream.read<double>();
    etl::optional<uint8_t> c = stream.read<uint8_t>();

    Object object{ i.value(), d.value(), c.value() };

    result = object;

    return result;
  }
}

namespace
{
  SUITE(test_byte_stream)
  {
    //*************************************************************************
    TEST(byte_stream_default_properties)
    {
      char storage[7];

      etl::byte_stream_writer writer(storage, ETL_OR_STD17::size(storage), etl::endian::big);
      etl::byte_stream_reader reader(storage, writer.size_bytes(), etl::endian::big); // Capacity is zero.

      CHECK(writer.empty());
      CHECK(reader.empty());

      CHECK(!writer.full());

      CHECK_EQUAL(0U, writer.size_bytes());
      CHECK_EQUAL(0U, reader.size_bytes());

      CHECK_EQUAL(ETL_OR_STD17::size(storage), writer.capacity());

      CHECK_EQUAL(0U, reader.available<int8_t>());

      CHECK_EQUAL(7U, writer.available_bytes());
      CHECK_EQUAL(7U, writer.available<int8_t>());
      CHECK_EQUAL(3U, writer.available<int16_t>());
      CHECK_EQUAL(1U, writer.available<int32_t>());
      CHECK_EQUAL(0U, writer.available<int64_t>());
    }

    //*************************************************************************
    TEST(byte_stream_properties)
    {
      char storage[8];

      etl::byte_stream_writer writer(storage, ETL_OR_STD17::size(storage), etl::endian::big);

      etl::span<char> storage_span(storage, storage + ETL_OR_STD17::size(storage));
      etl::span<char> writer_span = writer.data();
      CHECK(writer_span.begin() == storage_span.begin());
      CHECK(writer_span.end()   == storage_span.end());

      CHECK(writer.write(uint8_t(0x12U)));         // 1 written.
      CHECK(writer.write(uint16_t(0x1234U)));      // 2 more written.

      etl::span<char> used_span = writer.used_data();
      etl::span<char> free_span = writer.free_data();

      CHECK_EQUAL(sizeof(uint8_t) + sizeof(uint16_t), (std::distance(used_span.begin(), used_span.end())));
      CHECK_EQUAL(ETL_OR_STD17::size(storage) - sizeof(uint8_t) - sizeof(uint16_t), (std::distance(free_span.begin(), free_span.end())));

      CHECK(writer.write(uint32_t(0x12345678U)));       // 4 more written.
      CHECK_FALSE(writer.write(uint32_t(0x12345678U))); // Can't write 4 more.

      CHECK(!writer.empty());
      CHECK(!writer.full());
      CHECK_EQUAL(1U, writer.available<int8_t>());
      CHECK_EQUAL(7U, writer.size_bytes());
      CHECK_EQUAL(8U, writer.capacity());

      etl::byte_stream_reader reader(storage, writer.size_bytes(), etl::endian::big);

      CHECK(!reader.empty());
      CHECK_EQUAL(7U, reader.available<int8_t>());
      CHECK_EQUAL(7U, reader.size_bytes());
    }

    //*************************************************************************
    TEST(write_bool)
    {
      char storage[8];
      char result[8] = { 0, 1, 0, 1, 1, 0, 1, 0 };
      
      etl::byte_stream_writer byte_stream(storage, 8U, etl::endian::big);

      CHECK(byte_stream.write(false));
      CHECK(byte_stream.write(true));
      CHECK(byte_stream.write(false));
      CHECK(byte_stream.write(true));
      CHECK(byte_stream.write(true));
      CHECK(byte_stream.write(false));
      CHECK(byte_stream.write(true));
      CHECK(byte_stream.write(false));

      // One too many.
      CHECK_FALSE(byte_stream.write(true));

      CHECK_ARRAY_EQUAL(result, storage, 8);
    }

    //*************************************************************************
    TEST(write_int8_t)
    {
      std::array<char, 256> storage;

      std::array<char, 256> compare_data;
      std::iota(compare_data.begin(), compare_data.end(), 0);

      etl::byte_stream_writer byte_stream(storage.data(), storage.size(), etl::endian::big);

      for (size_t i = 0; i < 256; ++i)
      {
        CHECK(byte_stream.write(int8_t(i)));
      }

      // One too many.
      CHECK_FALSE(byte_stream.write(int8_t(0)));

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

      etl::byte_stream_writer byte_stream(storage.data(), storage.size(), etl::endian::big);

      for (size_t i = 0U; i < 256U; ++i)
      {
        CHECK(byte_stream.write(uint8_t(i)));
      }

      // One too many.
      CHECK_FALSE(byte_stream.write(uint8_t(0U)));

      for (size_t i = 0U; i < storage.size(); ++i)
      {
        CHECK_EQUAL(int(compare_data[i]), int(storage[i]));
      }
    }

    //*************************************************************************
    TEST(write_int16_t)
    {
      std::array<char, sizeof(int16_t) * 4> storage;
      std::array<char, sizeof(int16_t) * 4> compare_data = { char(0x00), char(0x01), char(0x5A), char(0xA5), char(0xA5), char(0x5A), char(0xFF), char(0xFF) };

      CHECK(compare_data.size() == storage.size());

      etl::byte_stream_writer byte_stream(storage.data(), storage.size(), etl::endian::big);

      CHECK(byte_stream.write(int16_t(0x0001)));
      CHECK(byte_stream.write(int16_t(0x5AA5)));
      CHECK(byte_stream.write(int16_t(0xA55A)));
      CHECK(byte_stream.write(int16_t(0xFFFF)));

      // One too many.
      CHECK_FALSE(byte_stream.write(int16_t(0)));

      for (size_t i = 0; i < storage.size(); ++i)
      {
        CHECK_EQUAL(int(compare_data[i]), int(storage[i]));
      }
    }

    //*************************************************************************
    TEST(write_uint16_t)
    {
      std::array<char, sizeof(uint16_t) * 4> storage;
      std::array<char, sizeof(uint16_t) * 4> compare_data = { char(0x00), char(0x01), char(0x5A), char(0xA5), char(0xA5), char(0x5A), char(0xFF), char(0xFF) };

      CHECK(compare_data.size() == storage.size());

      etl::byte_stream_writer byte_stream(storage.data(), storage.size(), etl::endian::big);

      CHECK(byte_stream.write(uint16_t(0x0001U)));
      CHECK(byte_stream.write(uint16_t(0x5AA5U)));
      CHECK(byte_stream.write(uint16_t(0xA55AU)));
      CHECK(byte_stream.write(uint16_t(0xFFFFU)));

      // One too many.
      CHECK_FALSE(byte_stream.write(uint16_t(0U)));

      for (size_t i = 0; i < storage.size(); ++i)
      {
        CHECK_EQUAL(int(compare_data[i]), int(storage[i]));
      }
    }

    //*************************************************************************
    TEST(write_int32_t)
    {
      std::array<char, sizeof(int32_t) * 4> storage;
      std::array<char, sizeof(int32_t) * 4> compare_data = { char(0x00), char(0x00), char(0x00), char(0x01),
                                                             char(0x5A), char(0xA5), char(0xA5), char(0x5A),
                                                             char(0xA5), char(0x5A), char(0x5A), char(0xA5),
                                                             char(0xFF), char(0xFF), char(0xFF), char(0xFF) };

      CHECK(compare_data.size() == storage.size());

      etl::byte_stream_writer byte_stream(storage.data(), storage.size(), etl::endian::big);

      CHECK(byte_stream.write(int32_t(0x00000001)));
      CHECK(byte_stream.write(int32_t(0x5AA5A55A)));
      CHECK(byte_stream.write(int32_t(0xA55A5AA5)));
      CHECK(byte_stream.write(int32_t(0xFFFFFFFF)));

      // One too many.
      CHECK_FALSE(byte_stream.write(int32_t(0)));

      for (size_t i = 0U; i < storage.size(); ++i)
      {
        CHECK_EQUAL(int(compare_data[i]), int(storage[i]));
      }
    }

    //*************************************************************************
    TEST(write_uint32_t)
    {
      std::array<char, sizeof(uint32_t) * 4> storage;
      std::array<char, sizeof(uint32_t) * 4> compare_data = { char(0x00), char(0x00), char(0x00), char(0x01),
                                                              char(0x5A), char(0xA5), char(0xA5), char(0x5A),
                                                              char(0xA5), char(0x5A), char(0x5A), char(0xA5),
                                                              char(0xFF), char(0xFF), char(0xFF), char(0xFF) };

      CHECK(compare_data.size() == storage.size());

      etl::byte_stream_writer byte_stream(storage.data(), storage.size(), etl::endian::big);

      CHECK(byte_stream.write(uint32_t(0x00000001U)));
      CHECK(byte_stream.write(uint32_t(0x5AA5A55AU)));
      CHECK(byte_stream.write(uint32_t(0xA55A5AA5U)));
      CHECK(byte_stream.write(uint32_t(0xFFFFFFFFU)));

      // One too many.
      CHECK_FALSE(byte_stream.write(uint32_t(0)));

      for (size_t i = 0U; i < storage.size(); ++i)
      {
        CHECK_EQUAL(int(compare_data[i]), int(storage[i]));
      }
    }

    //*************************************************************************
    TEST(write_int64_t)
    {
      std::array<char, sizeof(int64_t) * 4> storage;
      std::array<char, sizeof(int64_t) * 4> compare_data = { char(0x00), char(0x00), char(0x00), char(0x00), char(0x00), char(0x00), char(0x00), char(0x01),
                                                             char(0x5A), char(0xA5), char(0xA5), char(0x5A), char(0xA5), char(0x5A), char(0x5A), char(0xA5),
                                                             char(0xA5), char(0x5A), char(0x5A), char(0xA5), char(0x5A), char(0xA5), char(0xA5), char(0x5A),
                                                             char(0xFF), char(0xFF), char(0xFF), char(0xFF), char(0xFF), char(0xFF), char(0xFF), char(0xFF) };

      CHECK(compare_data.size() == storage.size());

      etl::byte_stream_writer byte_stream(storage.data(), storage.size(), etl::endian::big);

      CHECK(byte_stream.write(int64_t(0x0000000000000001)));
      CHECK(byte_stream.write(int64_t(0x5AA5A55AA55A5AA5)));
      CHECK(byte_stream.write(int64_t(0xA55A5AA55AA5A55A)));
      CHECK(byte_stream.write(int64_t(0xFFFFFFFFFFFFFFFF)));

      // One too many.
      CHECK_FALSE(byte_stream.write(int64_t(0)));

      for (size_t i = 0U; i < storage.size(); ++i)
      {
        CHECK_EQUAL(int(compare_data[i]), int(storage[i]));
      }
    }

    //*************************************************************************
    TEST(write_uint64_t)
    {
      std::array<char, sizeof(uint64_t) * 4> storage;
      std::array<char, sizeof(uint64_t) * 4> compare_data = { char(0x00), char(0x00), char(0x00), char(0x00), char(0x00), char(0x00), char(0x00), char(0x01),
                                                              char(0x5A), char(0xA5), char(0xA5), char(0x5A), char(0xA5), char(0x5A), char(0x5A), char(0xA5),
                                                              char(0xA5), char(0x5A), char(0x5A), char(0xA5), char(0x5A), char(0xA5), char(0xA5), char(0x5A),
                                                              char(0xFF), char(0xFF), char(0xFF), char(0xFF), char(0xFF), char(0xFF), char(0xFF), char(0xFF) };

      CHECK(compare_data.size() == storage.size());

      etl::byte_stream_writer byte_stream(storage.data(), storage.size(), etl::endian::big);

      CHECK(byte_stream.write(uint64_t(0x0000000000000001U)));
      CHECK(byte_stream.write(uint64_t(0x5AA5A55AA55A5AA5U)));
      CHECK(byte_stream.write(uint64_t(0xA55A5AA55AA5A55AU)));
      CHECK(byte_stream.write(uint64_t(0xFFFFFFFFFFFFFFFFU)));

      // One too many.
      CHECK_FALSE(byte_stream.write(uint64_t(0)));

      for (size_t i = 0U; i < storage.size(); ++i)
      {
        CHECK_EQUAL(int(compare_data[i]), int(storage[i]));
      }
    }

    //*************************************************************************
    TEST(write_and_skip_int32_t)
    {
      std::array<char, sizeof(int32_t) * 4> storage = { char(0xF0), char(0xF1), char(0xF2), char(0xF3),
                                                        char(0xF4), char(0xF5), char(0xF6), char(0xF7),
                                                        char(0xF8), char(0xF9), char(0xFA), char(0xFB),
                                                        char(0xFC), char(0xFD), char(0xFE), char(0xFF) };

      std::array<char, sizeof(int32_t) * 4> compare_data = { char(0x01), char(0x02), char(0x03), char(0x04),
                                                             char(0xF4), char(0xF5), char(0xF6), char(0xF7),
                                                             char(0xF8), char(0xF9), char(0xFA), char(0xFB),
                                                             char(0x05), char(0x06), char(0x07), char(0x08) };

      etl::byte_stream_writer byte_stream(storage.data(), storage.size(), etl::endian::big);

      CHECK(byte_stream.write(int32_t(0x01020304)));
      CHECK(byte_stream.skip<int32_t>(2));
      CHECK(byte_stream.write(int32_t(0x05060708)));
      CHECK_FALSE(byte_stream.skip<int32_t>(1));

      for (size_t i = 0U; i < storage.size(); ++i)
      {
        CHECK_EQUAL(int(compare_data[i]), int(storage[i]));
      }
    }

    //*************************************************************************
    TEST(read_and_skip_int32_t)
    {
      std::array<char, sizeof(int32_t) * 4> storage = { char(0x01), char(0x02), char(0x03), char(0x04),
                                                        char(0xF4), char(0xF5), char(0xF6), char(0xF7),
                                                        char(0xF8), char(0xF9), char(0xFA), char(0xFB),
                                                        char(0x05), char(0x06), char(0x07), char(0x08) };

      std::array<etl::optional<int32_t>, 4> compare = { int32_t(0x01020304), int32_t(0xF4F5F6F7), int32_t(0xF8F9FAFB), int32_t(0x05060708) };
      std::array<etl::optional<int32_t>, 4> result  = { int32_t(0xF0F1F2F3), int32_t(0xF4F5F6F7), int32_t(0xF8F9FAFB), int32_t(0xFCFDFEFF) };

      etl::byte_stream_reader byte_stream(storage.data(), storage.size(), etl::endian::big);

      CHECK(result[0] = byte_stream.read<int32_t>());
      CHECK(byte_stream.skip<int32_t>(2));
      CHECK(result[3] = byte_stream.read<int32_t>());
      CHECK_FALSE(byte_stream.skip<int32_t>(2));

      for (size_t i = 0U; i < result.size(); ++i)
      {
        CHECK_EQUAL(compare[i].value(), result[i].value());
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
      etl::byte_stream_writer writer(storage, 8, etl::endian::big);

      for (size_t i = 0U; i < flags.size(); ++i)
      {
        writer.write(flags[i]);
      }

      etl::byte_stream_reader reader(storage, 8, etl::endian::big);

      for (size_t i = 0; i < flags.size(); ++i)
      {
        etl::optional<bool> flag;

        CHECK(flag = reader.read<bool>());
        CHECK_EQUAL(flags[i], flag.value());
      }
    }

    //*************************************************************************
    TEST(write_read_int8_t)
    {
      std::array<char, 4 * sizeof(int8_t)> storage;
      std::array<int8_t, 4> put_data = { int8_t(0x01), int8_t(0x5A), int8_t(0xA5), int8_t(0xFF) };
      std::array<etl::optional<int8_t>, 4> get_data = { int8_t(0x00), int8_t(0x00), int8_t(0x00), int8_t(0x00) };

      etl::byte_stream_writer writer(storage.data(), storage.size(), etl::endian::big);

      // Insert into the stream
      CHECK(writer.empty());
      CHECK(!writer.full());
      CHECK_EQUAL(4U, writer.available<int8_t>());
      CHECK_EQUAL(0U, writer.size_bytes());
      
      writer.write(put_data[0]);
      CHECK(!writer.empty());
      CHECK(!writer.full());
      CHECK_EQUAL(3U, writer.available<int8_t>());
      
      writer.write(put_data[1]);
      CHECK(!writer.empty());
      CHECK(!writer.full());
      CHECK_EQUAL(2U, writer.available<int8_t>());
      
      writer.write(put_data[2]);
      CHECK(!writer.empty());
      CHECK(!writer.full());
      CHECK_EQUAL(1U, writer.available<int8_t>());
      
      writer.write(put_data[3]);
      CHECK(!writer.empty());
      CHECK(writer.full());
      CHECK_EQUAL(0U, writer.available<int8_t>());

      etl::byte_stream_reader reader(storage.data(), writer.size_bytes(), etl::endian::big);
      CHECK(!reader.empty());
      CHECK_EQUAL(4U, reader.available<int8_t>());
      CHECK_EQUAL(4U, reader.available_bytes());

      CHECK(get_data[0] = reader.read<int8_t>());
      CHECK_EQUAL(3U, reader.available<int8_t>());
      CHECK_EQUAL(put_data[0], int(get_data[0].value()));

      CHECK(get_data[1] = reader.read<int8_t>());
      CHECK_EQUAL(2U, reader.available<int8_t>());
      CHECK_EQUAL(put_data[1], int(get_data[1].value()));

      CHECK(get_data[2] = reader.read<int8_t>());
      CHECK_EQUAL(1U, reader.available<int8_t>());
      CHECK_EQUAL(put_data[2], int(get_data[2].value()));

      CHECK(get_data[3] = reader.read<int8_t>());
      CHECK_EQUAL(0U, reader.available<int8_t>());
      CHECK_EQUAL(put_data[3], int(get_data[3].value()));
    }

    //*************************************************************************
    TEST(write_read_uint8_t)
    {
      std::array<char, 4 * sizeof(uint8_t)> storage;
      std::array<uint8_t, 4> put_data = { uint8_t(0x01U), uint8_t(0x5AU), uint8_t(0xA5U), uint8_t(0xFFU) };
      std::array<etl::optional<uint8_t>, 4> get_data = { uint8_t(0x00U), uint8_t(0x00U), uint8_t(0x00U), uint8_t(0x00U) };

      etl::byte_stream_writer writer(storage.data(), storage.size(), etl::endian::big);

      // Insert into the stream
      CHECK(writer.empty());
      CHECK(!writer.full());
      CHECK_EQUAL(4U, writer.available<uint8_t>());
      CHECK_EQUAL(0U, writer.size_bytes());
      
      writer.write(put_data[0]);
      CHECK(!writer.empty());
      CHECK(!writer.full());
      CHECK_EQUAL(3U, writer.available<uint8_t>());
      
      writer.write(put_data[1]);
      CHECK(!writer.empty());
      CHECK(!writer.full());
      CHECK_EQUAL(2U, writer.available<uint8_t>());
      
      writer.write(put_data[2]);
      CHECK(!writer.empty());
      CHECK(!writer.full());
      CHECK_EQUAL(1U, writer.available<uint8_t>());
      
      writer.write(put_data[3]);
      CHECK(!writer.empty());
      CHECK(writer.full());
      CHECK_EQUAL(0U, writer.available<uint8_t>());

      etl::byte_stream_reader reader(storage.data(), writer.size_bytes(), etl::endian::big);
      CHECK(!reader.empty());
      CHECK_EQUAL(4U, reader.available<uint8_t>());

      CHECK(get_data[0] = reader.read<uint8_t>());
      CHECK_EQUAL(3U, reader.available<uint8_t>());
      CHECK_EQUAL(put_data[0], int(get_data[0].value()));

      CHECK(get_data[1] = reader.read<uint8_t>());
      CHECK_EQUAL(2U, reader.available<uint8_t>());
      CHECK_EQUAL(put_data[1], int(get_data[1].value()));

      CHECK(get_data[2] = reader.read<uint8_t>());
      CHECK_EQUAL(1U, reader.available<uint8_t>());
      CHECK_EQUAL(put_data[2], int(get_data[2].value()));

      CHECK(get_data[3] = reader.read<uint8_t>());
      CHECK_EQUAL(0U, reader.available<uint8_t>());
      CHECK_EQUAL(put_data[3], int(get_data[3].value()));
    }

    //*************************************************************************
    TEST(write_read_int16_t)
    {
      std::array<char, 4 * sizeof(int16_t)> storage;
      std::array<int16_t, 4> put_data = { int16_t(0x0001), int16_t(0xA55A), int16_t(0x5AA5), int16_t(0xFFFF) };
      std::array<etl::optional<int16_t>, 4> get_data = { int16_t(0x0000), int16_t(0x0000), int16_t(0x0000), int16_t(0x0000) };

      etl::byte_stream_writer writer(storage.data(), storage.size(), etl::endian::big);

      // Insert into the stream
      CHECK(writer.empty());
      CHECK(!writer.full());
      CHECK_EQUAL(4U, writer.available<int16_t>());
      CHECK_EQUAL(0U, writer.size_bytes());
      
      writer.write(put_data[0]);
      CHECK(!writer.empty());
      CHECK(!writer.full());
      CHECK_EQUAL(3U, writer.available<int16_t>());
      
      writer.write(put_data[1]);
      CHECK(!writer.empty());
      CHECK(!writer.full());
      CHECK_EQUAL(2U, writer.available<int16_t>());
      
      writer.write(put_data[2]);
      CHECK(!writer.empty());
      CHECK(!writer.full());
      CHECK_EQUAL(1U, writer.available<int16_t>());
      
      writer.write(put_data[3]);
      CHECK(!writer.empty());
      CHECK(writer.full());
      CHECK_EQUAL(0U, writer.available<int16_t>());

      etl::byte_stream_reader reader(storage.data(), writer.size_bytes(), etl::endian::big);
      CHECK(!reader.empty());
      CHECK_EQUAL(4U, reader.available<int16_t>());

      CHECK(get_data[0] = reader.read<int16_t>());
      CHECK_EQUAL(3U, reader.available<int16_t>());
      CHECK_EQUAL(put_data[0], get_data[0].value());

      CHECK(get_data[1] = reader.read<int16_t>());
      CHECK_EQUAL(2U, reader.available<int16_t>());
      CHECK_EQUAL(put_data[1], get_data[1].value());

      CHECK(get_data[2] = reader.read<int16_t>());
      CHECK_EQUAL(1U, reader.available<int16_t>());
      CHECK_EQUAL(put_data[2], get_data[2].value());

      CHECK(get_data[3] = reader.read<int16_t>());
      CHECK_EQUAL(0U, reader.available<int16_t>());
      CHECK_EQUAL(put_data[3], get_data[3].value());
    }

    //*************************************************************************
    TEST(write_read_uint16_t)
    {
      std::array<char, 4 * sizeof(uint16_t)> storage;
      std::array<uint16_t, 4> put_data = { uint16_t(0x0001U), uint16_t(0xA55AU), uint16_t(0x5AA5U), uint16_t(0xFFFFU) };
      std::array<etl::optional<uint16_t>, 4> get_data = { uint16_t(0x0000U), uint16_t(0x0000U), uint16_t(0x0000U), uint16_t(0x0000U) };

      etl::byte_stream_writer writer(storage.data(), storage.size(), etl::endian::big);

      // Insert into the stream
      CHECK(writer.empty());
      CHECK(!writer.full());
      CHECK_EQUAL(4U, writer.available<uint16_t>());
      CHECK_EQUAL(0U, writer.size_bytes());
      
      writer.write(put_data[0]);
      CHECK(!writer.empty());
      CHECK(!writer.full());
      CHECK_EQUAL(3U, writer.available<uint16_t>());
      
      writer.write(put_data[1]);
      CHECK(!writer.empty());
      CHECK(!writer.full());
      CHECK_EQUAL(2U, writer.available<uint16_t>());
      
      writer.write(put_data[2]);
      CHECK(!writer.empty());
      CHECK(!writer.full());
      CHECK_EQUAL(1U, writer.available<uint16_t>());
      
      writer.write(put_data[3]);
      CHECK(!writer.empty());
      CHECK(writer.full());
      CHECK_EQUAL(0U, writer.available<uint16_t>());

      etl::byte_stream_reader reader(storage.data(), writer.size_bytes(), etl::endian::big);
      CHECK(!reader.empty());
      CHECK_EQUAL(4U, reader.available<uint16_t>());

      CHECK(get_data[0] = reader.read<uint16_t>());
      CHECK_EQUAL(3U, reader.available<uint16_t>());
      CHECK_EQUAL(put_data[0], get_data[0].value());

      CHECK(get_data[1] = reader.read<uint16_t>());
      CHECK_EQUAL(2U, reader.available<uint16_t>());
      CHECK_EQUAL(put_data[1], get_data[1].value());

      CHECK(get_data[2] = reader.read<uint16_t>());
      CHECK_EQUAL(1U, reader.available<uint16_t>());
      CHECK_EQUAL(put_data[2], get_data[2].value());

      CHECK(get_data[3] = reader.read<uint16_t>());
      CHECK_EQUAL(0U, reader.available<uint16_t>());
      CHECK_EQUAL(put_data[3], get_data[3].value());
    }

    //*************************************************************************
    TEST(write_read_int32_t)
    {
      std::array<char, 4 * sizeof(int32_t)> storage;
      std::array<int32_t, 4> put_data = { int32_t(0x00000001), int32_t(0xA55AA55A), int32_t(0x5AA55AA5), int32_t(0xFFFFFFFF) };
      std::array<etl::optional<int32_t>, 4> get_data = { int32_t(0x00000000), int32_t(0x00000000), int32_t(0x00000000), int32_t(0x00000000) };

      etl::byte_stream_writer writer(storage.data(), storage.size(), etl::endian::big);

      // Insert into the stream
      CHECK(writer.empty());
      CHECK(!writer.full());
      CHECK_EQUAL(4U, writer.available<int32_t>());
      CHECK_EQUAL(0U, writer.size_bytes());
     
      writer.write(put_data[0]);
      CHECK(!writer.empty());
      CHECK(!writer.full());
      CHECK_EQUAL(3U, writer.available<int32_t>());
      
      writer.write(put_data[1]);
      CHECK(!writer.empty());
      CHECK(!writer.full());
      CHECK_EQUAL(2U, writer.available<int32_t>());
      
      writer.write(put_data[2]);
      CHECK(!writer.empty());
      CHECK(!writer.full());
      CHECK_EQUAL(1U, writer.available<int32_t>());
      
      writer.write(put_data[3]);
      CHECK(!writer.empty());
      CHECK(writer.full());
      CHECK_EQUAL(0U, writer.available<int32_t>());

      etl::byte_stream_reader reader(storage.data(), writer.size_bytes(), etl::endian::big);
      CHECK(!reader.empty());
      CHECK_EQUAL(4U, reader.available<int32_t>());

      CHECK(get_data[0] = reader.read<int32_t>());
      CHECK_EQUAL(3U, reader.available<int32_t>());
      CHECK_EQUAL(put_data[0], get_data[0].value());

      CHECK(get_data[1] = reader.read<int32_t>());
      CHECK_EQUAL(2U, reader.available<int32_t>());
      CHECK_EQUAL(put_data[1], get_data[1].value());

      CHECK(get_data[2] = reader.read<int32_t>());
      CHECK_EQUAL(1U, reader.available<int32_t>());
      CHECK_EQUAL(put_data[2], get_data[2].value());

      CHECK(get_data[3] = reader.read<int32_t>());
      CHECK_EQUAL(0U, reader.available<int32_t>());
      CHECK_EQUAL(put_data[3], get_data[3].value());
    }

    //*************************************************************************
    TEST(write_read_uint32_t)
    {
      std::array<char, 4 * sizeof(uint32_t)> storage;
      std::array<uint32_t, 4> put_data = { uint32_t(0x00000001U), uint32_t(0xA55AA55AU), uint32_t(0x5AA55AA5U), uint32_t(0xFFFFFFFFU) };
      std::array<etl::optional<uint32_t>, 4> get_data = { uint32_t(0x00000000U), uint32_t(0x00000000U), uint32_t(0x00000000U), uint32_t(0x00000000U) };

      etl::byte_stream_writer writer(storage.data(), storage.size(), etl::endian::big);

      // Insert into the stream
      CHECK(writer.empty());
      CHECK(!writer.full());
      CHECK_EQUAL(4U, writer.available<uint32_t>());
      CHECK_EQUAL(0U, writer.size_bytes());
      
      writer.write(put_data[0]);
      CHECK(!writer.empty());
      CHECK(!writer.full());
      CHECK_EQUAL(3U, writer.available<uint32_t>());
      
      writer.write(put_data[1]);
      CHECK(!writer.empty());
      CHECK(!writer.full());
      CHECK_EQUAL(2U, writer.available<uint32_t>());
      
      writer.write(put_data[2]);
      CHECK(!writer.empty());
      CHECK(!writer.full());
      CHECK_EQUAL(1U, writer.available<uint32_t>());
      
      writer.write(put_data[3]);
      CHECK(!writer.empty());
      CHECK(writer.full());
      CHECK_EQUAL(0U, writer.available<uint32_t>());

      etl::byte_stream_reader reader(storage.data(), writer.size_bytes(), etl::endian::big);
      CHECK(!reader.empty());
      CHECK_EQUAL(4U, reader.available<uint32_t>());

      CHECK(get_data[0] = reader.read<uint32_t>());
      CHECK_EQUAL(3U, reader.available<uint32_t>());
      CHECK_EQUAL(put_data[0], get_data[0].value());

      CHECK(get_data[1] = reader.read<uint32_t>());
      CHECK_EQUAL(2U, reader.available<uint32_t>());
      CHECK_EQUAL(put_data[1], get_data[1].value());

      CHECK(get_data[2] = reader.read<uint32_t>());
      CHECK_EQUAL(1U, reader.available<uint32_t>());
      CHECK_EQUAL(put_data[2], get_data[2].value());

      CHECK(get_data[3] = reader.read<uint32_t>());
      CHECK_EQUAL(0U, reader.available<uint32_t>());
      CHECK_EQUAL(put_data[3], get_data[3].value());
    }

    //*************************************************************************
    TEST(write_read_int64_t)
    {
      std::array<char, 4 * sizeof(int64_t)> storage;
      std::array<int64_t, 4> put_data    = { int64_t(0x0000000000000001), int64_t(0xA55AA55AA55AA55A), int64_t(0x5AA55AA55AA55AA5), int64_t(0xFFFFFFFFFFFFFFFF) };
      std::array<etl::optional<int64_t>, 4> get_data = { int64_t(0x0000000000000000), int64_t(0x0000000000000000), int64_t(0x0000000000000000), int64_t(0x0000000000000000) };

      etl::byte_stream_writer writer(storage.data(), storage.size(), etl::endian::big);

      // Insert into the stream
      CHECK(writer.empty());
      CHECK(!writer.full());
      CHECK_EQUAL(4U, writer.available<int64_t>());
      CHECK_EQUAL(0U, writer.size_bytes());
      
      writer.write(put_data[0]);
      CHECK(!writer.empty());
      CHECK(!writer.full());
      CHECK_EQUAL(3U, writer.available<int64_t>());
      
      writer.write(put_data[1]);
      CHECK(!writer.empty());
      CHECK(!writer.full());
      CHECK_EQUAL(2U, writer.available<int64_t>());
      
      writer.write(put_data[2]);
      CHECK(!writer.empty());
      CHECK(!writer.full());
      CHECK_EQUAL(1U, writer.available<int64_t>());
      
      writer.write(put_data[3]);
      CHECK(!writer.empty());
      CHECK(writer.full());
      CHECK_EQUAL(0U, writer.available<int64_t>());

      etl::byte_stream_reader reader(storage.data(), writer.size_bytes(), etl::endian::big);
      CHECK(!reader.empty());
      CHECK_EQUAL(4U, reader.available<int64_t>());

      CHECK(get_data[0] = reader.read<int64_t>());
      CHECK_EQUAL(3U, reader.available<int64_t>());
      CHECK_EQUAL(put_data[0], get_data[0].value());

      CHECK(get_data[1] = reader.read<int64_t>());
      CHECK_EQUAL(2U, reader.available<int64_t>());
      CHECK_EQUAL(put_data[1], get_data[1].value());

      CHECK(get_data[2] = reader.read<int64_t>());
      CHECK_EQUAL(1U, reader.available<int64_t>());
      CHECK_EQUAL(put_data[2], get_data[2].value());

      CHECK(get_data[3] = reader.read<int64_t>());
      CHECK_EQUAL(0U, reader.available<int64_t>());
      CHECK_EQUAL(put_data[3], get_data[3].value());
    }

    //*************************************************************************
    TEST(write_read_uint64_t)
    {
      std::array<char, 4 * sizeof(uint64_t)> storage;
      std::array<uint64_t, 4> put_data = { uint64_t(0x0000000000000001U), uint64_t(0xA55AA55AA55AA55AU), uint64_t(0x5AA55AA55AA55AA5U), uint64_t(0xFFFFFFFFFFFFFFFFU) };
      std::array<etl::optional<uint64_t>, 4> get_data = { uint64_t(0x0000000000000000U), uint64_t(0x0000000000000000U), uint64_t(0x0000000000000000U), uint64_t(0x0000000000000000U) };

      etl::byte_stream_writer writer(storage.data(), storage.size(), etl::endian::big);

      // Insert into the stream
      CHECK(writer.empty());
      CHECK(!writer.full());
      CHECK_EQUAL(4U, writer.available<uint64_t>());
      CHECK_EQUAL(0U, writer.size_bytes());

      writer.write(put_data[0]);
      CHECK(!writer.empty());
      CHECK(!writer.full());
      CHECK_EQUAL(3U, writer.available<uint64_t>());

      writer.write(put_data[1]);
      CHECK(!writer.empty());
      CHECK(!writer.full());
      CHECK_EQUAL(2U, writer.available<uint64_t>());

      writer.write(put_data[2]);
      CHECK(!writer.empty());
      CHECK(!writer.full());
      CHECK_EQUAL(1U, writer.available<uint64_t>());

      writer.write(put_data[3]);
      CHECK(!writer.empty());
      CHECK(writer.full());
      CHECK_EQUAL(0U, writer.available<uint64_t>());

      etl::byte_stream_reader reader(storage.data(), writer.size_bytes(), etl::endian::big);
      CHECK(!reader.empty());
      CHECK_EQUAL(4U, reader.available<uint64_t>());

      CHECK(get_data[0] = reader.read<uint64_t>());
      CHECK_EQUAL(3U, reader.available<uint64_t>());
      CHECK_EQUAL(put_data[0], get_data[0].value());

      CHECK(get_data[1] = reader.read<uint64_t>());
      CHECK_EQUAL(2U, reader.available<uint64_t>());
      CHECK_EQUAL(put_data[1], get_data[1].value());

      CHECK(get_data[2] = reader.read<uint64_t>());
      CHECK_EQUAL(1U, reader.available<uint64_t>());
      CHECK_EQUAL(put_data[2], get_data[2].value());

      CHECK(get_data[3] = reader.read<uint64_t>());
      CHECK_EQUAL(0U, reader.available<uint64_t>());
      CHECK_EQUAL(put_data[3], get_data[3].value());
    }

    //*************************************************************************
    TEST(write_read_char_range)
    {
      std::array<char, 5 * sizeof(char)> storage;
      std::array<char, 4> put_data = { char(0x01), char(0x5A), char(0xA5), char(0xFF) };

      etl::span<char> input(put_data.begin(), put_data.end());

      etl::byte_stream_writer writer(storage.data(), storage.size(), etl::endian::big);
      CHECK(writer.write(input));
      CHECK(writer.write(char(0x99))); // Write an extra value.

      etl::byte_stream_reader reader(storage.data(), writer.size_bytes(), etl::endian::big);

      etl::optional<etl::span<const char>> output = reader.read<char>(4U);
      CHECK_EQUAL(4U, output.value().size());
      CHECK_EQUAL(int(put_data[0]), int(output.value()[0]));
      CHECK_EQUAL(int(put_data[1]), int(output.value()[1]));
      CHECK_EQUAL(int(put_data[2]), int(output.value()[2]));
      CHECK_EQUAL(int(put_data[3]), int(output.value()[3]));

      etl::optional<char> i = reader.read<char>();  // Read back the extra value to ensure that the current index is correct.
      CHECK_EQUAL(int(char(0x99)), int(i.value()));
    }

    //*************************************************************************
    TEST(write_read_int32_t_span_range)
    {
      std::array<char, 5 * sizeof(int32_t)> storage;
      std::array<int32_t, 4> put_data = { int32_t(0x00000001), int32_t(0xA55AA55A), int32_t(0x5AA55AA5), int32_t(0xFFFFFFFF) };
      std::array<int32_t, 4> get_data = { int32_t(0x00000000), int32_t(0x00000000), int32_t(0x00000000), int32_t(0x00000000) };

      etl::span<int32_t> input(put_data.begin(), put_data.end());

      etl::byte_stream_writer writer(storage.data(), storage.size(), etl::endian::big);
      CHECK(writer.write(input));
      CHECK(writer.write(0x12345678)); // Write an extra value.

      etl::byte_stream_reader reader(storage.data(), writer.size_bytes(), etl::endian::big);

      etl::span<int32_t> output(get_data.begin(), get_data.end());

      etl::optional<etl::span<const int32_t> > result = reader.read<int32_t>(output);
      CHECK(result.has_value());
      CHECK_EQUAL(sizeof(const int32_t), result.value().size());
      CHECK_EQUAL(put_data[0], get_data[0]);
      CHECK_EQUAL(put_data[1], get_data[1]);
      CHECK_EQUAL(put_data[2], get_data[2]);
      CHECK_EQUAL(put_data[3], get_data[3]);

      etl::optional<int32_t> i = reader.read<int32_t>();  // Read back the extra value to ensure that the current index is correct.
      CHECK_EQUAL(0x12345678, i.value());
    }

    //*************************************************************************
    TEST(write_read_int32_t_start_length_range)
    {
      std::array<char, 5 * sizeof(int32_t)> storage;
      std::array<int32_t, 4> put_data = { int32_t(0x00000001), int32_t(0xA55AA55A), int32_t(0x5AA55AA5), int32_t(0xFFFFFFFF) };
      std::array<int32_t, 4> get_data = { int32_t(0x00000000), int32_t(0x00000000), int32_t(0x00000000), int32_t(0x00000000) };

      etl::byte_stream_writer writer(storage.data(), storage.size(), etl::endian::big);
      CHECK(writer.write(put_data.data(), put_data.size()));
      CHECK(writer.write(0x12345678)); // Write an extra value.

      etl::byte_stream_reader reader(storage.data(), writer.size_bytes(), etl::endian::big);

      etl::optional<etl::span<const int32_t> > result = reader.read<int32_t>(get_data.data(), get_data.size());
      CHECK(result.has_value());
      CHECK_EQUAL(sizeof(const int32_t), result.value().size());
      CHECK_EQUAL(put_data[0], get_data[0]);
      CHECK_EQUAL(put_data[1], get_data[1]);
      CHECK_EQUAL(put_data[2], get_data[2]);
      CHECK_EQUAL(put_data[3], get_data[3]);

      etl::optional<int32_t> i = reader.read<int32_t>();  // Read back the extra value to ensure that the current index is correct.
      CHECK_EQUAL(0x12345678, i.value());
    }

    //*************************************************************************
    TEST(write_read_multiple_big_endian)
    {
      char c1 = 90;
      char c2 = -91;
      unsigned short s1 = 23205;
      unsigned short s2 = 42330;
      int32_t i1 = 1520786085;   // 0x5AA55AA5
      int32_t i2 = -1520786086;  // 0xA55AA55A
      float f = 3.1415927f;
      double d = 3.1415927;

      std::array<char, 100> storage;

      etl::byte_stream_writer writer(storage.data(), storage.size(), etl::endian::big);

      // Insert into the stream.
      writer.write(c1);
      writer.write(s1);
      writer.write(i1);
      writer.write(f);
      writer.write(i2);
      writer.write(d);
      writer.write(s2);
      writer.write(c2);

      etl::byte_stream_reader reader(storage.data(), writer.size_bytes(), etl::endian::big);

      etl::optional<char> rc1;
      etl::optional<char> rc2;
      etl::optional<unsigned short> rs1;
      etl::optional<unsigned short> rs2;
      etl::optional<int32_t> ri1;
      etl::optional<int32_t> ri2;
      etl::optional<float> rf;
      etl::optional<double> rd;

      // Read them all back.
      CHECK(rc1 = reader.read<char>());
      CHECK_EQUAL(int(c1), int(rc1.value()));

      CHECK(rs1 = reader.read<unsigned short>());
      CHECK_EQUAL(s1, rs1.value());

      CHECK(ri1 = reader.read<int32_t>());
      CHECK_EQUAL(i1, ri1.value());

      CHECK(rf = reader.read<float>());
      CHECK_CLOSE(f, rf.value(), 0.1f);

      CHECK(ri2 = reader.read<int32_t>());
      CHECK_EQUAL(i2, ri2.value());

      CHECK(rd = reader.read<double>());
      CHECK_CLOSE(d, rd.value(), 0.1);

      CHECK(rs2 = reader.read<unsigned short>());
      CHECK_EQUAL(s2, rs2.value());

      CHECK(rc2 = reader.read<char>());
      CHECK_EQUAL(int(c2), int(rc2.value()));
    }

    //*************************************************************************
    TEST(write_read_multiple_little_endian)
    {
      char c1 = 90;
      char c2 = -91;
      unsigned short s1 = 23205;
      unsigned short s2 = 42330;
      int32_t i1 = 1520786085;   // 0x5AA55AA5
      int32_t i2 = -1520786086;  // 0xA55AA55A
      float f = 3.1415927f;
      double d = 3.1415927;

      std::array<char, 100> storage;

      etl::byte_stream_writer writer(storage.data(), storage.size(), etl::endian::little);

      // Insert into the stream.
      writer.write(c1);
      writer.write(s1);
      writer.write(i1);
      writer.write(f);
      writer.write(i2);
      writer.write(d);
      writer.write(s2);
      writer.write(c2);

      etl::byte_stream_reader reader(storage.data(), writer.size_bytes(), etl::endian::little);

      etl::optional<char> rc1;
      etl::optional<char> rc2;
      etl::optional<unsigned short> rs1;
      etl::optional<unsigned short> rs2;
      etl::optional<int32_t> ri1;
      etl::optional<int32_t> ri2;
      etl::optional<float> rf;
      etl::optional<double> rd;

      // Read them all back.
      CHECK(rc1 = reader.read<char>());
      CHECK_EQUAL(int(c1), int(rc1.value()));

      CHECK(rs1 = reader.read<unsigned short>());
      CHECK_EQUAL(s1, rs1.value());

      CHECK(ri1 = reader.read<int32_t>());
      CHECK_EQUAL(i1, ri1.value());

      CHECK(rf = reader.read<float>());
      CHECK_CLOSE(f, rf.value(), 0.1f);

      CHECK(ri2 = reader.read<int32_t>());
      CHECK_EQUAL(i2, ri2.value());

      CHECK(rd = reader.read<double>());
      CHECK_CLOSE(d, rd.value(), 0.1);

      CHECK(rs2 = reader.read<unsigned short>());
      CHECK_EQUAL(s2, rs2.value());

      CHECK(rc2 = reader.read<char>());
      CHECK_EQUAL(int(c2), int(rc2.value()));
    }

    //*************************************************************************
    TEST(write_read_object)
    {    
      std::array<char, 2 * sizeof(Object)> storage;

      etl::byte_stream_writer writer(storage.data(), storage.size(), etl::endian::big);

      Object object1 = { -1234, 2.71578369, 250 };
      Object object2 = {  5678, 5.24685744, 126 };

      CHECK(etl::write(writer, object1));
      CHECK(etl::write(writer, object2));

      etl::optional<Object> object1a;
      etl::optional<Object> object2a;

      etl::byte_stream_reader reader(storage.data(), writer.size_bytes(), etl::endian::big);

      CHECK(object1a = etl::read<Object>(reader));
      CHECK(object2a = etl::read<Object>(reader));

      CHECK_EQUAL(object1.i, object1a.value().i);
      CHECK_EQUAL(object1.d, object1a.value().d);
      CHECK_EQUAL(int(object1.c), int(object1a.value().c));

      CHECK_EQUAL(object2.i, object2a.value().i);
      CHECK_EQUAL(object2.d, object2a.value().d);
      CHECK_EQUAL(int(object2.c), int(object2a.value().c));
    }

    //*************************************************************************
    TEST(write_read_object_unchecked)
    {
      std::array<char, 2 * sizeof(Object)> storage;

      etl::byte_stream_writer writer(storage.data(), storage.size(), etl::endian::big);

      Object object1 = { -1234, 2.71578369, 250 };
      Object object2 = { 5678, 5.24685744, 126 };

      etl::write_unchecked(writer, object1);
      etl::write_unchecked(writer, object2);

      Object object1a;
      Object object2a;

      etl::byte_stream_reader reader(storage.data(), writer.size_bytes(), etl::endian::big);

      object1a = etl::read_unchecked<Object>(reader);
      object2a = etl::read_unchecked<Object>(reader);

      CHECK_EQUAL(object1.i, object1a.i);
      CHECK_EQUAL(object1.d, object1a.d);
      CHECK_EQUAL(int(object1.c), int(object1a.c));

      CHECK_EQUAL(object2.i, object2a.i);
      CHECK_EQUAL(object2.d, object2a.d);
      CHECK_EQUAL(int(object2.c), int(object2a.c));
    }

    //*************************************************************************
    TEST(write_read_multiple_float)
    {

      float  f = 3.1415927f;
      double d = 3.1415927;

      std::array<char, 12> storage;

      etl::byte_stream_writer writer(storage.data(), storage.size(), etl::endian::big);

      writer.write(f);
      writer.write(d);

      etl::byte_stream_reader reader(storage.data(), storage.size(), etl::endian::big);

      etl::optional<float>  rf;
      etl::optional<double> rd;

      CHECK(rf = reader.read<float>());
      CHECK_CLOSE(f, rf.value(), 0.1f);

      CHECK(rd = reader.read<double>());
      CHECK_CLOSE(d, rd.value(), 0.1);
    }

    //*************************************************************************
    TEST(write_read_int16_t_with_skip)
    {
      std::array<char, 4 * sizeof(int16_t)> storage;
      std::array<int16_t, 4> put_data = { int16_t(0x0001), int16_t(0xA55A), int16_t(0x5AA5), int16_t(0xFFFF) };
      std::array<etl::optional<int16_t>, 4> get_data = { int16_t(0x0000), int16_t(0x0000), int16_t(0x0000), int16_t(0x0000) };

      etl::byte_stream_writer writer(storage.data(), storage.size(), etl::endian::big);

      // Insert into the stream
      writer.write(put_data[0]);
      writer.write(put_data[1]);
      writer.write(put_data[2]);
      writer.write(put_data[3]);

      etl::byte_stream_reader reader(storage.data(), writer.size_bytes(), etl::endian::big);
      CHECK(get_data[0] = reader.read<int16_t>());
      CHECK_EQUAL(put_data[0], int(get_data[0].value()));

      // Skip two int16_t
      CHECK(reader.skip<int16_t>(2U));

      CHECK(get_data[3] = reader.read<int16_t>());
      CHECK_EQUAL(put_data[3], int(get_data[3].value()));

      CHECK_EQUAL(int16_t(0x0000), int(get_data[1].value()));
      CHECK_EQUAL(int16_t(0x0000), int(get_data[2].value()));

      reader.restart();
      // Skip five int16_t (too many)
      CHECK_FALSE(reader.skip<int16_t>(5U));
    }

    //*************************************************************************
    TEST(read_span_int32_t)
    {
      std::array<char, 4 * sizeof(int32_t)> storage;
      std::array<int32_t, 4> put_data = { int32_t(0x00000001), int32_t(0xA55AA55A), int32_t(0x5AA55AA5), int32_t(0xFFFFFFFF) };
      std::array<int32_t, 4> get_data = { int32_t(0x00000000), int32_t(0x00000000), int32_t(0x00000000), int32_t(0x00000000) };

      etl::span<int32_t> input(put_data.begin(), put_data.end());

      etl::byte_stream_writer writer(storage.data(), storage.size(), etl::endian::big);
      writer.write(input);

      const char* read_only_storage = reinterpret_cast<const char*>(storage.data());
      etl::byte_stream_reader reader(read_only_storage, storage.size() * sizeof(int32_t), etl::endian::big);

      etl::optional<etl::span<const int32_t> > result = reader.read(etl::span<int32_t>(get_data.begin(), get_data.end()));

      CHECK(result.has_value());

      CHECK_EQUAL(int32_t(0x00000001), result.value()[0]);
      CHECK_EQUAL(int32_t(0xA55AA55A), result.value()[1]);
      CHECK_EQUAL(int32_t(0x5AA55AA5), result.value()[2]);
      CHECK_EQUAL(int32_t(0xFFFFFFFF), result.value()[3]);

      CHECK_EQUAL(int32_t(0x00000001), get_data[0]);
      CHECK_EQUAL(int32_t(0xA55AA55A), get_data[1]);
      CHECK_EQUAL(int32_t(0x5AA55AA5), get_data[2]);
      CHECK_EQUAL(int32_t(0xFFFFFFFF), get_data[3]);
    }

    //*************************************************************************
    TEST(write_byte_stream_iterative_copy)
    {
      std::array<char, sizeof(int32_t)> storage;
      std::array<int32_t, 4> put_data = { int32_t(0x00000001), int32_t(0xA55AA55A), int32_t(0x5AA55AA5), int32_t(0xFFFFFFFF) };
      std::vector<char> expected = { char(0x00), char(0x00), char(0x00), char(0x01), 
                                     char(0xA5), char(0x5A), char(0xA5), char(0x5A), 
                                     char(0x5A), char(0xA5), char(0x5A), char(0xA5), 
                                     char(0xFF), char(0xFF), char(0xFF), char(0xFF) };
      std::vector<char> result;

      etl::byte_stream_writer writer(storage.data(), storage.size(), etl::endian::big);

      for (auto i : put_data)
      {
        writer.write(i);
        etl::span<char> s = writer.used_data();
        std::copy(s.begin(), s.end(), std::back_inserter(result));
        writer.restart();
      }

      for (size_t i = 0U; i < (4U * sizeof(int32_t)); ++i)
      {
        CHECK_EQUAL(expected[i], result[i]);
      }
    }

    //*************************************************************************
    TEST(write_byte_stream_callback)
    {
      std::array<char, sizeof(int32_t)> storage;
      std::array<int32_t, 4> put_data = { int32_t(0x00000001), int32_t(0xA55AA55A), int32_t(0x5AA55AA5), int32_t(0xFFFFFFFF) };
      std::vector<char> expected = { char(0x00), char(0x00), char(0x00), char(0x01),
                                     char(0xA5), char(0x5A), char(0xA5), char(0x5A),
                                     char(0x5A), char(0xA5), char(0x5A), char(0xA5),
                                     char(0xFF), char(0xFF), char(0xFF), char(0xFF) };
      

      static std::vector<char> result;

      etl::byte_stream_writer::callback_type callback([&](etl::byte_stream_writer::callback_parameter_type sp) 
                                                      { 
                                                        std::copy(sp.begin(), sp.end(), std::back_inserter(result));
                                                      });

      etl::byte_stream_writer writer(storage.data(), storage.size(), etl::endian::big, callback);

      for (auto i : put_data)
      {
        writer.write(i);
        writer.restart();
      }

      for (size_t i = 0U; i < (4U * sizeof(int32_t)); ++i)
      {
        CHECK_EQUAL(expected[i], result[i]);
      }
    }
    //*************************************************************************
    TEST(read_byte_stream_skip)
    {
      etl::array<uint8_t, 4> data;
      etl::byte_stream_reader r(data.begin(), data.size(), etl::endian::little);
      CHECK_TRUE(r.skip<uint8_t>(4));
      etl::optional<etl::span<const uint8_t>> result = r.read<uint8_t>(4);
      CHECK_FALSE(result.has_value());
      CHECK_TRUE(r.empty());
    }
  };
}
