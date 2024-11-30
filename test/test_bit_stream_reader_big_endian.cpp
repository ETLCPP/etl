/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl
https://www.etlcpp.com

Copyright(c) 2018 jwellbelove

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

#include "etl/bit_stream.h"

#include <array>
#include <numeric>

#include "etl/private/diagnostic_unused_function_push.h"
#include "etl/private/diagnostic_useless_cast_push.h"

namespace
{
  //***********************************
  struct Object
  {
    int16_t s;
    int32_t i;
    uint8_t c;
  };

  bool operator ==(const Object& lhs, const Object& rhs)
  {
    return (lhs.s == rhs.s) &&
           (lhs.i == rhs.i) &&
           (lhs.c == rhs.c);
  }

  std::ostream& operator << (std::ostream& os, const Object& object)
  {
    os << object.s << "," << object.i << "," << (int)object.c;
    return os;
  }
}

namespace etl
{
  //***********************************
  template <>
  Object read_unchecked<Object>(etl::bit_stream_reader& stream)
  {
    int16_t result_s = stream.read_unchecked<int16_t>(14);
    int32_t result_i = stream.read_unchecked<int32_t>(23);
    uint8_t result_c = stream.read_unchecked<uint8_t>();

    return Object{ result_s, result_i, result_c };
  }

  //***********************************
  template <>
  optional<Object> read<Object>(etl::bit_stream_reader& stream)
  {
    optional<Object> result;

    etl::optional<int16_t> result_s = stream.read<int16_t>(14);
    etl::optional<int32_t> result_i = stream.read<int32_t>(23);
    etl::optional<uint8_t> result_c = stream.read<uint8_t>();

    if (result_s.has_value() && result_i.has_value() && result_c.has_value())
    {
      result = Object{ result_s.value(), result_i.value(), result_c.value() };
    }

    return result;
  }
}

namespace
{
  SUITE(test_bit_stream_reader_big_endian)
  {
    //*************************************************************************
    TEST(test_read_bool)
    {
      char storage = 0x5AU;

      etl::bit_stream_reader bit_stream(&storage, 1, etl::endian::big);

      CHECK_EQUAL(1U, bit_stream.size_bytes());

      etl::optional<bool> result;

      result.reset();
      result = bit_stream.read<bool>();
      CHECK(result.has_value());
      CHECK_EQUAL(false, result.value());
      
      result.reset();
      result = bit_stream.read<bool>();
      CHECK(result.has_value());
      CHECK_EQUAL(true, result.value());
      
      result.reset();
      result = bit_stream.read<bool>();
      CHECK(result.has_value());
      CHECK_EQUAL(false, result.value());

      result.reset();
      result = bit_stream.read<bool>();
      CHECK(result.has_value());
      CHECK_EQUAL(true, result.value());

      result.reset();
      result = bit_stream.read<bool>();
      CHECK(result.has_value());
      CHECK_EQUAL(true, result.value());

      result.reset();
      result = bit_stream.read<bool>();
      CHECK(result.has_value());
      CHECK_EQUAL(false, result.value());

      result.reset();
      result = bit_stream.read<bool>();
      CHECK(result.has_value());
      CHECK_EQUAL(true, result.value());

      result.reset();
      result = bit_stream.read<bool>();
      CHECK(result.has_value());
      CHECK_EQUAL(false, result.value());

      // One too many.
      CHECK_FALSE(bit_stream.read<bool>());
    }

    //*************************************************************************
    TEST(test_read_bool_using_non_member_function)
    {
      char storage = 0x5AU;

      etl::bit_stream_reader bit_stream(&storage, 1, etl::endian::big);

      CHECK_EQUAL(1U, bit_stream.size_bytes());

      etl::optional<bool> result;

      result.reset();
      result = etl::read<bool>(bit_stream);
      CHECK(result.has_value());
      CHECK_EQUAL(false, result.value());

      result.reset();
      result = etl::read<bool>(bit_stream);
      CHECK(result.has_value());
      CHECK_EQUAL(true, result.value());

      result.reset();
      result = etl::read<bool>(bit_stream);
      CHECK(result.has_value());
      CHECK_EQUAL(false, result.value());

      result.reset();
      result = etl::read<bool>(bit_stream);
      CHECK(result.has_value());
      CHECK_EQUAL(true, result.value());

      result.reset();
      result = etl::read<bool>(bit_stream);
      CHECK(result.has_value());
      CHECK_EQUAL(true, result.value());

      result.reset();
      result = etl::read<bool>(bit_stream);
      CHECK(result.has_value());
      CHECK_EQUAL(false, result.value());

      result.reset();
      result = etl::read<bool>(bit_stream);
      CHECK(result.has_value());
      CHECK_EQUAL(true, result.value());

      result.reset();
      result = etl::read<bool>(bit_stream);
      CHECK(result.has_value());
      CHECK_EQUAL(false, result.value());

      // One too many.
      CHECK_FALSE(etl::read<bool>(bit_stream));
    }

    //*************************************************************************
    TEST(test_read_int8_t)
    {
      std::array<char, 4U> storage = { char(0x01), char(0x5A), char(0xA5), char(0xFF) };
      std::array<int8_t, 4U> expected = { int8_t(0x01), int8_t(0x5A), int8_t(0xA5), int8_t(0xFF) };

      etl::bit_stream_reader bit_stream(storage.data(), storage.size(), etl::endian::big);

      CHECK_EQUAL(storage.size(), bit_stream.size_bytes());

      etl::optional<int8_t> result;

      result.reset();
      result = bit_stream.read<int8_t>();
      CHECK(result.has_value());
      CHECK_EQUAL(int(expected[0]), int(result.value()));

      result.reset();
      result = bit_stream.read<int8_t>();
      CHECK(result.has_value());
      CHECK_EQUAL(int(expected[1]), int(result.value()));

      result.reset();
      result = bit_stream.read<int8_t>();
      CHECK(result.has_value());
      CHECK_EQUAL(int(expected[2]), int(result.value()));

      result.reset();
      result = bit_stream.read<int8_t>();
      CHECK(result.has_value());
      CHECK_EQUAL(int(expected[3]), int(result.value()));

      // One too many.
      CHECK_FALSE(bit_stream.read<int8_t>());
    }

    //*************************************************************************
    TEST(test_read_checked_int8_t_using_non_member_function)
    {
      std::array<char, 4U> storage = { char(0x01), char(0x5A), char(0xA5), char(0xFF) };
      std::array<int8_t, 4U> expected = { int8_t(0x01), int8_t(0x5A), int8_t(0xA5), int8_t(0xFF) };

      etl::bit_stream_reader bit_stream(storage.data(), storage.size(), etl::endian::big);

      CHECK_EQUAL(storage.size(), bit_stream.size_bytes());

      etl::optional<int8_t> result;

      result.reset();
      result = etl::read<int8_t>(bit_stream);
      CHECK(result.has_value());
      CHECK_EQUAL(int(expected[0]), int(result.value()));

      result.reset();
      result = etl::read<int8_t>(bit_stream);
      CHECK(result.has_value());
      CHECK_EQUAL(int(expected[1]), int(result.value()));

      result.reset();
      result = etl::read<int8_t>(bit_stream);
      CHECK(result.has_value());
      CHECK_EQUAL(int(expected[2]), int(result.value()));

      result.reset();
      result = etl::read<int8_t>(bit_stream);
      CHECK(result.has_value());
      CHECK_EQUAL(int(expected[3]), int(result.value()));

      // One too many.
      CHECK_FALSE(etl::read<int8_t>(bit_stream));
    }

    //*************************************************************************
    TEST(test_read_unchecked_int8_t_using_non_member_function)
    {
      std::array<char, 4U> storage = { char(0x01), char(0x5A), char(0xA5), char(0xFF) };
      std::array<int8_t, 4U> expected = { int8_t(0x01), int8_t(0x5A), int8_t(0xA5), int8_t(0xFF) };

      etl::bit_stream_reader bit_stream(storage.data(), storage.size(), etl::endian::big);

      CHECK_EQUAL(storage.size(), bit_stream.size_bytes());

      int8_t result;

      result = etl::read_unchecked<int8_t>(bit_stream);
      CHECK_EQUAL(int(expected[0]), int(result));

      result = etl::read_unchecked<int8_t>(bit_stream);
      CHECK_EQUAL(int(expected[1]), int(result));

      result = etl::read_unchecked<int8_t>(bit_stream);
      CHECK_EQUAL(int(expected[2]), int(result));

      result = etl::read_unchecked<int8_t>(bit_stream);
      CHECK_EQUAL(int(expected[3]), int(result));
    }
    
    //*************************************************************************
    TEST(test_read_int8_t_5bits)
    {
      std::array<char, 3U> storage = { char(0x0E), char(0x8B), char(0xF0) };
      std::array<int8_t, 4U> expected = { int8_t(0x01), int8_t(0xFA), int8_t(0x05), int8_t(0xFF) };

      etl::bit_stream_reader bit_stream(storage.data(), storage.size(), etl::endian::big);

      CHECK_EQUAL(storage.size(), bit_stream.size_bytes());

      etl::optional<int8_t> result;

      result.reset();
      result = bit_stream.read<int8_t>(5U);
      CHECK(result.has_value());
      CHECK_EQUAL(int(expected[0]), int(result.value()));

      result.reset();
      result = bit_stream.read<int8_t>(5U);
      CHECK(result.has_value());
      CHECK_EQUAL(int(expected[1]), int(result.value()));

      result.reset();
      result = bit_stream.read<int8_t>(5U);
      CHECK(result.has_value());
      CHECK_EQUAL(int(expected[2]), int(result.value()));

      result.reset();
      result = bit_stream.read<int8_t>(5U);
      CHECK(result.has_value());
      CHECK_EQUAL(int(expected[3]), int(result.value()));

      // One too many.
      CHECK_FALSE(bit_stream.read<int8_t>(5U));
    }

    //*************************************************************************
    TEST(test_read_int8_t_5bits_with_skip)
    {
      std::array<char, 3U> storage = { char(0x0E), char(0x8B), char(0xF0) };
      std::array<int8_t, 4U> expected = { int8_t(0x01), int8_t(0xFA), int8_t(0x05), int8_t(0xFF) };

      etl::bit_stream_reader bit_stream(storage.data(), storage.size(), etl::endian::big);

      CHECK_EQUAL(storage.size(), bit_stream.size_bytes());

      etl::optional<int8_t> result;

      result.reset();
      result = bit_stream.read<int8_t>(5U);
      CHECK(result.has_value());
      CHECK_EQUAL(int(expected[0]), int(result.value()));

      bool success = bit_stream.skip(5U);
      CHECK(success);

      result.reset();
      result = bit_stream.read<int8_t>(5U);
      CHECK(result.has_value());
      CHECK_EQUAL(int(expected[2]), int(result.value()));

      result.reset();
      result = bit_stream.read<int8_t>(5U);
      CHECK(result.has_value());
      CHECK_EQUAL(int(expected[3]), int(result.value()));

      // One too many.
      CHECK_FALSE(bit_stream.read<int8_t>(5U));
    }

    //*************************************************************************
    TEST(test_read_checked_int8_t_5bits_using_non_member_function)
    {
      std::array<char, 3U> storage = { char(0x0E), char(0x8B), char(0xF0) };
      std::array<int8_t, 4U> expected = { int8_t(0x01), int8_t(0xFA), int8_t(0x05), int8_t(0xFF) };

      etl::bit_stream_reader bit_stream(storage.data(), storage.size(), etl::endian::big);

      CHECK_EQUAL(storage.size(), bit_stream.size_bytes());

      etl::optional<int8_t> result;

      result.reset();
      result = etl::read<int8_t>(bit_stream, 5U);
      CHECK(result.has_value());
      CHECK_EQUAL(int(expected[0]), int(result.value()));

      result.reset();
      result = etl::read<int8_t>(bit_stream, 5U);
      CHECK(result.has_value());
      CHECK_EQUAL(int(expected[1]), int(result.value()));

      result.reset();
      result = etl::read<int8_t>(bit_stream, 5U);
      CHECK(result.has_value());
      CHECK_EQUAL(int(expected[2]), int(result.value()));

      result.reset();
      result = etl::read<int8_t>(bit_stream, 5U);
      CHECK(result.has_value());
      CHECK_EQUAL(int(expected[3]), int(result.value()));

      // One too many.
      CHECK_FALSE(etl::read<int8_t>(bit_stream, 5U));
    }

    //*************************************************************************
    TEST(test_read_unchecked_int8_t_5bits_using_non_member_function)
    {
      std::array<char, 3U> storage = { char(0x0E), char(0x8B), char(0xF0) };
      std::array<int8_t, 4U> expected = { int8_t(0x01), int8_t(0xFA), int8_t(0x05), int8_t(0xFF) };

      etl::bit_stream_reader bit_stream(storage.data(), storage.size(), etl::endian::big);

      CHECK_EQUAL(storage.size(), bit_stream.size_bytes());

      int8_t result;

      result = etl::read_unchecked<int8_t>(bit_stream, 5U);
      CHECK_EQUAL(int(expected[0]), int(result));

      result = etl::read_unchecked<int8_t>(bit_stream, 5U);
      CHECK_EQUAL(int(expected[1]), int(result));

      result = etl::read_unchecked<int8_t>(bit_stream, 5U);
      CHECK_EQUAL(int(expected[2]), int(result));

      result = etl::read_unchecked<int8_t>(bit_stream, 5U);
      CHECK_EQUAL(int(expected[3]), int(result));
    }

    //*************************************************************************
    TEST(test_read_uint8_t)
    {
      std::array<char, 4U>    storage  = { char(0x01), char(0x5A), char(0xA5), char(0xFF) };
      std::array<uint8_t, 4U> expected = { uint8_t(0x01), uint8_t(0x5A), uint8_t(0xA5), uint8_t(0xFF) };

      etl::bit_stream_reader bit_stream(storage.data(), storage.size(), etl::endian::big);

      CHECK_EQUAL(storage.size(), bit_stream.size_bytes());

      etl::optional<uint8_t> result;

      result.reset();
      result = bit_stream.read<uint8_t>();
      CHECK(result.has_value());
      CHECK_EQUAL(int(expected[0]), int(result.value()));

      result.reset();
      result = bit_stream.read<uint8_t>();
      CHECK(result.has_value());
      CHECK_EQUAL(int(expected[1]), int(result.value()));

      result.reset();
      result = bit_stream.read<uint8_t>();
      CHECK(result.has_value());
      CHECK_EQUAL(int(expected[2]), int(result.value()));

      result.reset();
      result = bit_stream.read<uint8_t>();
      CHECK(result.has_value());
      CHECK_EQUAL(int(expected[3]), int(result.value()));

      // One too many.
      CHECK_FALSE(bit_stream.read<uint8_t>());
    }

    //*************************************************************************
    TEST(test_read_uint8_t_5bits)
    {
      std::array<char, 3U> storage = { char(0x0E), char(0x8B), char(0xF0) };
      std::array<char, 4U> expected = { uint8_t(0x01), uint8_t(0x1A), uint8_t(0x05), uint8_t(0x1F) };

      etl::bit_stream_reader bit_stream(storage.data(), storage.size(), etl::endian::big);

      CHECK_EQUAL(storage.size(), bit_stream.size_bytes());

      etl::optional<uint8_t> result;

      result.reset();
      result = bit_stream.read<uint8_t>(5U);
      CHECK(result.has_value());
      CHECK_EQUAL(int(expected[0]), int(result.value()));

      result.reset();
      result = bit_stream.read<uint8_t>(5U);
      CHECK(result.has_value());
      CHECK_EQUAL(int(expected[1]), int(result.value()));

      result.reset();
      result = bit_stream.read<uint8_t>(5U);
      CHECK(result.has_value());
      CHECK_EQUAL(int(expected[2]), int(result.value()));

      result.reset();
      result = bit_stream.read<uint8_t>(5U);
      CHECK(result.has_value());
      CHECK_EQUAL(int(expected[3]), int(result.value()));

      // One too many.
      CHECK_FALSE(bit_stream.read<uint8_t>(5U));
    }

    //*************************************************************************
    TEST(test_read_int16_t)
    {
      std::array<char, 8U> storage = { char(0x00), char(0x01), char(0xA5), char(0x5A), char(0x5A), char(0xA5), char(0xFF), char(0xFF) };
      std::array<int16_t, 4U> expected = { int16_t(0x0001), int16_t(0xA55A), int16_t(0x5AA5), int16_t(0xFFFF) };

      etl::bit_stream_reader bit_stream(storage.data(), storage.size(), etl::endian::big);

      CHECK_EQUAL(storage.size(), bit_stream.size_bytes());

      etl::optional<int16_t> result;

      result.reset();
      result = bit_stream.read<int16_t>();
      CHECK(result.has_value());
      CHECK_EQUAL(expected[0], result.value());

      result.reset();
      result = bit_stream.read<int16_t>();
      CHECK(result.has_value());
      CHECK_EQUAL(expected[1], result.value());

      result.reset();
      result = bit_stream.read<int16_t>();
      CHECK(result.has_value());
      CHECK_EQUAL(expected[2], result.value());

      result.reset();
      result = bit_stream.read<int16_t>();
      CHECK(result.has_value());
      CHECK_EQUAL(expected[3], result.value());

      // One too many.
      CHECK_FALSE(bit_stream.read<int16_t>());
    }

    //*************************************************************************
    TEST(test_read_int16_t_10bits)
    {
      std::array<char, 5U> storage = { char(0x00), char(0x55), char(0xAA), char(0x97), char(0xFF) };
      std::array<int16_t, 4U> expected = { int16_t(0x0001), int16_t(0x015A), int16_t(0xFEA5), int16_t(0xFFFF) };

      etl::bit_stream_reader bit_stream(storage.data(), storage.size(), etl::endian::big);

      CHECK_EQUAL(storage.size(), bit_stream.size_bytes());

      etl::optional<int16_t> result;

      result.reset();
      result = bit_stream.read<int16_t>(10U);
      CHECK(result.has_value());
      CHECK_EQUAL(expected[0], result.value());

      result.reset();
      result = bit_stream.read<int16_t>(10U);
      CHECK(result.has_value());
      CHECK_EQUAL(expected[1], result.value());

      result.reset();
      result = bit_stream.read<int16_t>(10U);
      CHECK(result.has_value());
      CHECK_EQUAL(expected[2], result.value());

      result.reset();
      result = bit_stream.read<int16_t>(10U);
      CHECK(result.has_value());
      CHECK_EQUAL(expected[3], result.value());

      // One too many.
      CHECK_FALSE(bit_stream.read<int16_t>(10U));
    }

    //*************************************************************************
    TEST(test_read_uint16_t)
    {
      std::array<char, 8U> storage = { char(0x00), char(0x01), char(0xA5), char(0x5A), char(0x5A), char(0xA5), char(0xFF), char(0xFF) };
      std::array<uint16_t, 4U> expected = { uint16_t(0x0001), uint16_t(0xA55A), uint16_t(0x5AA5), uint16_t(0xFFFF) };

      etl::bit_stream_reader bit_stream(storage.data(), storage.size(), etl::endian::big);

      CHECK_EQUAL(storage.size(), bit_stream.size_bytes());

      etl::optional<uint16_t> result;

      result.reset();
      result = bit_stream.read<uint16_t>();
      CHECK(result.has_value());
      CHECK_EQUAL(expected[0], result.value());

      result.reset();
      result = bit_stream.read<uint16_t>();
      CHECK(result.has_value());
      CHECK_EQUAL(expected[1], result.value());

      result.reset();
      result = bit_stream.read<uint16_t>();
      CHECK(result.has_value());
      CHECK_EQUAL(expected[2], result.value());

      result.reset();
      result = bit_stream.read<uint16_t>();
      CHECK(result.has_value());
      CHECK_EQUAL(expected[3], result.value());

      // One too many.
      CHECK_FALSE(bit_stream.read<uint16_t>());
    }

    //*************************************************************************
    TEST(test_read_uint16_t_10bits)
    {
      std::array<char, 5U> storage = { char(0x00), char(0x55), char(0xAA), char(0x97), char(0xFF) };
      std::array<uint16_t, 4U> expected = { uint16_t(0x0001), uint16_t(0x015A), uint16_t(0x02A5), uint16_t(0x03FF) };

      etl::bit_stream_reader bit_stream(storage.data(), storage.size(), etl::endian::big);

      CHECK_EQUAL(storage.size(), bit_stream.size_bytes());

      etl::optional<uint16_t> result;

      result.reset();
      result = bit_stream.read<uint16_t>(10U);
      CHECK(result.has_value());
      CHECK_EQUAL(expected[0], result.value());

      result.reset();
      result = bit_stream.read<uint16_t>(10U);
      CHECK(result.has_value());
      CHECK_EQUAL(expected[1], result.value());

      result.reset();
      result = bit_stream.read<uint16_t>(10U);
      CHECK(result.has_value());
      CHECK_EQUAL(expected[2], result.value());

      result.reset();
      result = bit_stream.read<uint16_t>(10U);
      CHECK(result.has_value());
      CHECK_EQUAL(expected[3], result.value());

      // One too many.
      CHECK_FALSE(bit_stream.read<uint16_t>(10U));
    }

    //*************************************************************************
    TEST(test_read_int32_t)
    {
      std::array<char, 16U> storage = { char(0x00), char(0x00), char(0x00), char(0x01), 
                             char(0xA5), char(0x5A), char(0xA5), char(0x5A), 
                             char(0x5A), char(0xA5), char(0x5A), char(0xA5), 
                             char(0xFF), char(0xFF), char(0xFF), char(0xFF) };
      std::array<int32_t, 4U> expected = { int32_t(0x00000001), int32_t(0xA55AA55A), int32_t(0x5AA55AA5), int32_t(0xFFFFFFFF) };

      etl::bit_stream_reader bit_stream(storage.data(), storage.size(), etl::endian::big);

      CHECK_EQUAL(storage.size(), bit_stream.size_bytes());

      etl::optional<int32_t> result;

      result.reset();
      result = bit_stream.read<int32_t>();
      CHECK(result.has_value());
      CHECK_EQUAL(expected[0], result.value());

      result.reset();
      result = bit_stream.read<int32_t>();
      CHECK(result.has_value());
      CHECK_EQUAL(expected[1], result.value());

      result.reset();
      result = bit_stream.read<int32_t>();
      CHECK(result.has_value());
      CHECK_EQUAL(expected[2], result.value());

      result.reset();
      result = bit_stream.read<int32_t>();
      CHECK(result.has_value());
      CHECK_EQUAL(expected[3], result.value());

      // One too many.
      CHECK_FALSE(bit_stream.read<int32_t>());
    }

    //*************************************************************************
    TEST(test_read_int32_t_22bits)
    {
      std::array<char, 11U> storage = { char(0x00), char(0x00), char(0x05), char(0xAA),
                                        char(0x55), char(0xA9), char(0x56), char(0xA9),
                                        char(0x7F), char(0xFF), char(0xFF) };
      std::array<int32_t, 4U> expected = { int32_t(0x00000001), int32_t(0x001AA55A), int32_t(0xFFE55AA5), int32_t(0xFFFFFFFF) };

      etl::bit_stream_reader bit_stream(storage.data(), storage.size(), etl::endian::big);

      CHECK_EQUAL(storage.size(), bit_stream.size_bytes());

      etl::optional<int32_t> result;

      result.reset();
      result = bit_stream.read<int32_t>(22U);
      CHECK(result.has_value());
      CHECK_EQUAL(expected[0], result.value());

      result.reset();
      result = bit_stream.read<int32_t>(22U);
      CHECK(result.has_value());
      CHECK_EQUAL(expected[1], result.value());

      result.reset();
      result = bit_stream.read<int32_t>(22U);
      CHECK(result.has_value());
      CHECK_EQUAL(expected[2], result.value());

      result.reset();
      result = bit_stream.read<int32_t>(22U);
      CHECK(result.has_value());
      CHECK_EQUAL(expected[3], result.value());

      // One too many.
      CHECK_FALSE(bit_stream.read<int32_t>(22U));
    }

    //*************************************************************************
    TEST(test_read_uint32_t)
    {
      std::array<char, 16U> storage = { char(0x00), char(0x00), char(0x00), char(0x01),
                                        char(0xA5), char(0x5A), char(0xA5), char(0x5A),
                                        char(0x5A), char(0xA5), char(0x5A), char(0xA5),
                                        char(0xFF), char(0xFF), char(0xFF), char(0xFF) };
      std::array<uint32_t, 4U> expected = { uint32_t(0x00000001), uint32_t(0xA55AA55A), uint32_t(0x5AA55AA5), uint32_t(0xFFFFFFFF) };

      etl::bit_stream_reader bit_stream(storage.data(), storage.size(), etl::endian::big);

      CHECK_EQUAL(storage.size(), bit_stream.size_bytes());

      etl::optional<uint32_t> result;

      result.reset();
      result = bit_stream.read<uint32_t>();
      CHECK(result.has_value());
      CHECK_EQUAL(expected[0], result.value());

      result.reset();
      result = bit_stream.read<uint32_t>();
      CHECK(result.has_value());
      CHECK_EQUAL(expected[1], result.value());

      result.reset();
      result = bit_stream.read<uint32_t>();
      CHECK(result.has_value());
      CHECK_EQUAL(expected[2], result.value());

      result.reset();
      result = bit_stream.read<uint32_t>();
      CHECK(result.has_value());
      CHECK_EQUAL(expected[3], result.value());

      // One too many.
      CHECK_FALSE(bit_stream.read<uint32_t>());
    }

    //*************************************************************************
    TEST(test_read_uint32_t_22bits)
    {
      std::array<char, 11U> storage = { char(0x00), char(0x00), char(0x05), char(0xAA),
                                        char(0x55), char(0xA9), char(0x56), char(0xA9),
                                        char(0x7F), char(0xFF), char(0xFF) };
      std::array<uint32_t, 4U> expected = { uint32_t(0x00000001), uint32_t(0x001AA55A), uint32_t(0x00255AA5), uint32_t(0x003FFFFF) };

      etl::bit_stream_reader bit_stream(storage.data(), storage.size(), etl::endian::big);

      CHECK_EQUAL(storage.size(), bit_stream.size_bytes());

      etl::optional<uint32_t> result;

      result.reset();
      result = bit_stream.read<uint32_t>(22U);
      CHECK(result.has_value());
      CHECK_EQUAL(expected[0], result.value());

      result.reset();
      result = bit_stream.read<uint32_t>(22U);
      CHECK(result.has_value());
      CHECK_EQUAL(expected[1], result.value());

      result.reset();
      result = bit_stream.read<uint32_t>(22U);
      CHECK(result.has_value());
      CHECK_EQUAL(expected[2], result.value());

      result.reset();
      result = bit_stream.read<uint32_t>(22U);
      CHECK(result.has_value());
      CHECK_EQUAL(expected[3], result.value());

      // One too many.
      CHECK_FALSE(bit_stream.read<uint32_t>(22U));
    }

    //*************************************************************************
    TEST(test_read_int64_t)
    {
      std::array<char, 32U> storage = { char(0x00), char(0x00), char(0x00), char(0x00), char(0x00), char(0x00), char(0x00), char(0x01),
                                        char(0xA5), char(0x5A), char(0xA5), char(0x5A), char(0xA5), char(0x5A), char(0xA5), char(0x5A),
                                        char(0x5A), char(0xA5), char(0x5A), char(0xA5), char(0x5A), char(0xA5), char(0x5A), char(0xA5),
                                        char(0xFF), char(0xFF), char(0xFF), char(0xFF), char(0xFF), char(0xFF), char(0xFF), char(0xFF) };
      std::array<int64_t, 4U> expected = { int64_t(0x0000000000000001), int64_t(0xA55AA55AA55AA55A), int64_t(0x5AA55AA55AA55AA5), int64_t(0xFFFFFFFFFFFFFFFF) };

      etl::bit_stream_reader bit_stream(storage.data(), storage.size(), etl::endian::big);

      CHECK_EQUAL(storage.size(), bit_stream.size_bytes());

      etl::optional<int64_t> result;

      result.reset();
      result = bit_stream.read<int64_t>();
      CHECK(result.has_value());
      CHECK_EQUAL(expected[0], result.value());

      result.reset();
      result = bit_stream.read<int64_t>();
      CHECK(result.has_value());
      CHECK_EQUAL(expected[1], result.value());

      result.reset();
      result = bit_stream.read<int64_t>();
      CHECK(result.has_value());
      CHECK_EQUAL(expected[2], result.value());

      result.reset();
      result = bit_stream.read<int64_t>();
      CHECK(result.has_value());
      CHECK_EQUAL(expected[3], result.value());

      // One too many.
      CHECK_FALSE(bit_stream.read<int64_t>());
    }

    //*************************************************************************
    TEST(test_read_int64_t_47bits)
    {
      std::array<char, 24U> storage = { char(0x00), char(0x00), char(0x00), char(0x00), char(0x00), char(0x02), char(0x95), char(0x6A),
                                        char(0x95), char(0x6A), char(0x95), char(0x6A), char(0xD5), char(0x2A), char(0xD5), char(0x2A),
                                        char(0xD5), char(0x2F), char(0xFF), char(0xFF), char(0xFF), char(0xFF), char(0xFF), char(0xF0) };
      std::array<int64_t, 4U> expected = { int64_t(0x0000000000000001), int64_t(0x0000255AA55AA55A), int64_t(0xFFFFDAA55AA55AA5), int64_t(0xFFFFFFFFFFFFFFFF) };

      etl::bit_stream_reader bit_stream(storage.data(), storage.size(), etl::endian::big);

      CHECK_EQUAL(storage.size(), bit_stream.size_bytes());

      etl::optional<int64_t> result;

      result.reset();
      result = bit_stream.read<int64_t>(47U);
      CHECK(result.has_value());
      CHECK_EQUAL(expected[0], result.value());

      result.reset();
      result = bit_stream.read<int64_t>(47U);
      CHECK(result.has_value());
      CHECK_EQUAL(expected[1], result.value());

      result.reset();
      result = bit_stream.read<int64_t>(47U);
      CHECK(result.has_value());
      CHECK_EQUAL(expected[2], result.value());

      result.reset();
      result = bit_stream.read<int64_t>(47U);
      CHECK(result.has_value());
      CHECK_EQUAL(expected[3], result.value());

      // One too many.
      CHECK_FALSE(bit_stream.read<int64_t>(47U));
    }

    //*************************************************************************
    TEST(test_read_uint64_t)
    {
      std::array<char, 32U> storage = { char(0x00), char(0x00), char(0x00), char(0x00), char(0x00), char(0x00), char(0x00), char(0x01),
                                        char(0xA5), char(0x5A), char(0xA5), char(0x5A), char(0xA5), char(0x5A), char(0xA5), char(0x5A),
                                        char(0x5A), char(0xA5), char(0x5A), char(0xA5), char(0x5A), char(0xA5), char(0x5A), char(0xA5),
                                        char(0xFF), char(0xFF), char(0xFF), char(0xFF), char(0xFF), char(0xFF), char(0xFF), char(0xFF) };
      std::array<uint64_t, 4U> expected = { uint64_t(0x0000000000000001), uint64_t(0xA55AA55AA55AA55A), uint64_t(0x5AA55AA55AA55AA5), uint64_t(0xFFFFFFFFFFFFFFFF) };

      etl::bit_stream_reader bit_stream(storage.data(), storage.size(), etl::endian::big);

      CHECK_EQUAL(storage.size(), bit_stream.size_bytes());

      etl::optional<uint64_t> result;

      result.reset();
      result = bit_stream.read<uint64_t>();
      CHECK(result.has_value());
      CHECK_EQUAL(expected[0], result.value());

      result.reset();
      result = bit_stream.read<uint64_t>();
      CHECK(result.has_value());
      CHECK_EQUAL(expected[1], result.value());

      result.reset();
      result = bit_stream.read<uint64_t>();
      CHECK(result.has_value());
      CHECK_EQUAL(expected[2], result.value());

      result.reset();
      result = bit_stream.read<uint64_t>();
      CHECK(result.has_value());
      CHECK_EQUAL(expected[3], result.value());

      // One too many.
      CHECK_FALSE(bit_stream.read<uint64_t>());
    }

    //*************************************************************************
    TEST(test_read_uint64_t_47bits)
    {
      std::array<char, 24U> storage = { char(0x00), char(0x00), char(0x00), char(0x00), char(0x00), char(0x02), char(0x95), char(0x6A),
                             char(0x95), char(0x6A), char(0x95), char(0x6A), char(0xD5), char(0x2A), char(0xD5), char(0x2A),
                             char(0xD5), char(0x2F), char(0xFF), char(0xFF), char(0xFF), char(0xFF), char(0xFF), char(0xF0) };
      std::array<uint64_t, 4U> expected = { uint64_t(0x0000000000000001), uint64_t(0x0000255AA55AA55A), uint64_t(0x00005AA55AA55AA5), uint64_t(0x00007FFFFFFFFFFF) };

      etl::bit_stream_reader bit_stream(storage.data(), storage.size(), etl::endian::big);

      CHECK_EQUAL(storage.size(), bit_stream.size_bytes());

      etl::optional<uint64_t> result;

      result.reset();
      result = bit_stream.read<uint64_t>(47U);
      CHECK(result.has_value());
      CHECK_EQUAL(expected[0], result.value());

      result.reset();
      result = bit_stream.read<uint64_t>(47U);
      CHECK(result.has_value());
      CHECK_EQUAL(expected[1], result.value());

      result.reset();
      result = bit_stream.read<uint64_t>(47U);
      CHECK(result.has_value());
      CHECK_EQUAL(expected[2], result.value());

      result.reset();
      result = bit_stream.read<uint64_t>(47U);
      CHECK(result.has_value());
      CHECK_EQUAL(expected[3], result.value());

      // One too many.
      CHECK_FALSE(bit_stream.read<uint64_t>(47U));
    }

    //*************************************************************************
    TEST(test_read_multiple_full_size)
    {
      //int8_t   c1 = 90;          // 0x5A
      //uint16_t s1 = 4660;        // 0x1234
      //int32_t  i1 = 0x89ABCDEF; // 0x89ABCDEF
      //int32_t  i2 = 0xFEDCBA98; // 0xFEDCBA98
      //uint16_t s2 = 22136;       // 0x5678
      //int8_t   c2 = -91;         // 0xA5

      std::array<char, 14U> storage = { char(0x5A),
                                        char(0x12), char(0x34),
                                        char(0x89), char(0xAB), char(0xCD), char(0xEF),
                                        char(0xFE), char(0xDC), char(0xBA), char(0x98),
                                        char(0x56), char(0x78),
                                        char(0xA5) };

      etl::bit_stream_reader bit_stream(storage.data(), storage.size(), etl::endian::big);

      CHECK_EQUAL(storage.size(), bit_stream.size_bytes());

      auto result_c1 = bit_stream.read<int8_t>();
      CHECK(result_c1.has_value());
      CHECK_EQUAL(int8_t(0x5A), result_c1.value());

      auto result_s1 = bit_stream.read<uint16_t>();
      CHECK(result_s1.has_value());
      CHECK_EQUAL(uint16_t(0x1234), result_s1.value());

      auto result_i1 = bit_stream.read<int32_t>();
      CHECK(result_i1.has_value());
      CHECK_EQUAL(int32_t(0x89ABCDEF), result_i1.value());

      auto result_i2 = bit_stream.read<int32_t>();
      CHECK(result_i2.has_value());
      CHECK_EQUAL(int32_t(0xFEDCBA98), result_i2.value());

      auto result_s2 = bit_stream.read<uint16_t>();
      CHECK(result_s2.has_value());
      CHECK_EQUAL(uint16_t(0x5678), result_s2.value());

      auto result_c2 = bit_stream.read<int8_t>();
      CHECK(result_c2.has_value());
      CHECK_EQUAL(int8_t(0xA5), result_c2.value());
    }

    //*************************************************************************
    TEST(test_read_multiple_variable_size)
    {
      //int8_t   c1 = 90;         // 0x5A       6 bits
      //uint16_t s1 = 4660;       // 0x1234     13 bits
      //int32_t  i1 = 0x89ABCDEF; // 0x89ABCDEF 23 bits
      //int32_t  i2 = 0xFEDCBA98; // 0xFEDCBA98 25 bits
      //uint16_t s2 = 22136;      // 0x5678     11 bits
      //int8_t   c2 = -91;        // 0xA5       7 bits

      std::array<char, 11U> storage = { char(0x6A), char(0x46), char(0x8A), char(0xF3),
                                        char(0x7B), char(0xDB), char(0x97), char(0x53),
                                        char(0x19), char(0xE1), char(0x28) };

      etl::bit_stream_reader bit_stream(storage.data(), storage.size(), etl::endian::big);

      CHECK_EQUAL(storage.size(), bit_stream.size_bytes());

      auto result_c1 = bit_stream.read<int8_t>(6U);
      CHECK(result_c1.has_value());
      CHECK_EQUAL(int8_t(0x1A), result_c1.value());

      auto result_s1 = bit_stream.read<uint16_t>(13U);
      CHECK(result_s1.has_value());
      CHECK_EQUAL(uint16_t(0x1234), result_s1.value());

      auto result_i1 = bit_stream.read<int32_t>(23U);
      CHECK(result_i1.has_value());
      CHECK_EQUAL(int32_t(0x002BCDEF), result_i1.value());

      auto result_i2 = bit_stream.read<int32_t>(25U);
      CHECK(result_i2.has_value());
      CHECK_EQUAL(int32_t(0x00DCBA98), result_i2.value());

      auto result_s2 = bit_stream.read<uint16_t>(11U);
      CHECK(result_s2.has_value());
      CHECK_EQUAL(uint16_t(0x0678), result_s2.value());

      auto result_c2 = bit_stream.read<int8_t>(7U);
      CHECK(result_c2.has_value());
      CHECK_EQUAL(int8_t(0x25), result_c2.value());
    }

    //*************************************************************************
    TEST(test_read_multiple_variable_size_using_span_char_dynamic_extent_input)
    {
      //int8_t   c1 = 90;         // 0x5A       6 bits
      //uint16_t s1 = 4660;       // 0x1234     13 bits
      //int32_t  i1 = 0x89ABCDEF; // 0x89ABCDEF 23 bits
      //int32_t  i2 = 0xFEDCBA98; // 0xFEDCBA98 25 bits
      //uint16_t s2 = 22136;      // 0x5678     11 bits
      //int8_t   c2 = -91;        // 0xA5       7 bits

      std::array<char, 11U> storage = { char(0x6A), char(0x46), char(0x8A), char(0xF3),
                                        char(0x7B), char(0xDB), char(0x97), char(0x53),
                                        char(0x19), char(0xE1), char(0x28) };

      auto storage_span = etl::span<char, etl::dynamic_extent>(storage.data(), storage.size());

      etl::bit_stream_reader bit_stream(storage_span, etl::endian::big);

      CHECK_EQUAL(storage.size(), bit_stream.size_bytes());

      auto result_c1 = bit_stream.read<int8_t>(6U);
      CHECK(result_c1.has_value());
      CHECK_EQUAL(int8_t(0x1A), result_c1.value());

      auto result_s1 = bit_stream.read<uint16_t>(13U);
      CHECK(result_s1.has_value());
      CHECK_EQUAL(uint16_t(0x1234), result_s1.value());

      auto result_i1 = bit_stream.read<int32_t>(23U);
      CHECK(result_i1.has_value());
      CHECK_EQUAL(int32_t(0x002BCDEF), result_i1.value());

      auto result_i2 = bit_stream.read<int32_t>(25U);
      CHECK(result_i2.has_value());
      CHECK_EQUAL(int32_t(0x00DCBA98), result_i2.value());

      auto result_s2 = bit_stream.read<uint16_t>(11U);
      CHECK(result_s2.has_value());
      CHECK_EQUAL(uint16_t(0x0678), result_s2.value());

      auto result_c2 = bit_stream.read<int8_t>(7U);
      CHECK(result_c2.has_value());
      CHECK_EQUAL(int8_t(0x25), result_c2.value());
    }

    //*************************************************************************
    TEST(test_read_multiple_variable_size_using_span_unsigned_char_dynamic_extent_input)
    {
      //int8_t   c1 = 90;         // 0x5A       6 bits
      //uint16_t s1 = 4660;       // 0x1234     13 bits
      //int32_t  i1 = 0x89ABCDEF; // 0x89ABCDEF 23 bits
      //int32_t  i2 = 0xFEDCBA98; // 0xFEDCBA98 25 bits
      //uint16_t s2 = 22136;      // 0x5678     11 bits
      //int8_t   c2 = -91;        // 0xA5       7 bits

      std::array<unsigned char, 11U> storage = { (unsigned char)(0x6A), (unsigned char)(0x46), (unsigned char)(0x8A), (unsigned char)(0xF3),
                                                 (unsigned char)(0x7B), (unsigned char)(0xDB), (unsigned char)(0x97), (unsigned char)(0x53),
                                                 (unsigned char)(0x19), (unsigned char)(0xE1), (unsigned char)(0x28) };

      auto storage_span = etl::span<unsigned char, etl::dynamic_extent>(storage.data(), storage.size());

      etl::bit_stream_reader bit_stream(storage_span, etl::endian::big);

      CHECK_EQUAL(storage.size(), bit_stream.size_bytes());

      auto result_c1 = bit_stream.read<int8_t>(6U);
      CHECK(result_c1.has_value());
      CHECK_EQUAL(int8_t(0x1A), result_c1.value());

      auto result_s1 = bit_stream.read<uint16_t>(13U);
      CHECK(result_s1.has_value());
      CHECK_EQUAL(uint16_t(0x1234), result_s1.value());

      auto result_i1 = bit_stream.read<int32_t>(23U);
      CHECK(result_i1.has_value());
      CHECK_EQUAL(int32_t(0x002BCDEF), result_i1.value());

      auto result_i2 = bit_stream.read<int32_t>(25U);
      CHECK(result_i2.has_value());
      CHECK_EQUAL(int32_t(0x00DCBA98), result_i2.value());

      auto result_s2 = bit_stream.read<uint16_t>(11U);
      CHECK(result_s2.has_value());
      CHECK_EQUAL(uint16_t(0x0678), result_s2.value());

      auto result_c2 = bit_stream.read<int8_t>(7U);
      CHECK(result_c2.has_value());
      CHECK_EQUAL(int8_t(0x25), result_c2.value());
    }

    //*************************************************************************
    TEST(test_read_multiple_variable_size_using_span_char_fixed_extent_input)
    {
      //int8_t   c1 = 90;         // 0x5A       6 bits
      //uint16_t s1 = 4660;       // 0x1234     13 bits
      //int32_t  i1 = 0x89ABCDEF; // 0x89ABCDEF 23 bits
      //int32_t  i2 = 0xFEDCBA98; // 0xFEDCBA98 25 bits
      //uint16_t s2 = 22136;      // 0x5678     11 bits
      //int8_t   c2 = -91;        // 0xA5       7 bits

      std::array<char, 11U> storage = { char(0x6A), char(0x46), char(0x8A), char(0xF3),
                                        char(0x7B), char(0xDB), char(0x97), char(0x53),
                                        char(0x19), char(0xE1), char(0x28) };

      auto storage_span = etl::span<char, 11U>(storage.data(), storage.size());

      etl::bit_stream_reader bit_stream(storage_span, etl::endian::big);

      CHECK_EQUAL(storage.size(), bit_stream.size_bytes());

      auto result_c1 = bit_stream.read<int8_t>(6U);
      CHECK(result_c1.has_value());
      CHECK_EQUAL(int8_t(0x1A), result_c1.value());

      auto result_s1 = bit_stream.read<uint16_t>(13U);
      CHECK(result_s1.has_value());
      CHECK_EQUAL(uint16_t(0x1234), result_s1.value());

      auto result_i1 = bit_stream.read<int32_t>(23U);
      CHECK(result_i1.has_value());
      CHECK_EQUAL(int32_t(0x002BCDEF), result_i1.value());

      auto result_i2 = bit_stream.read<int32_t>(25U);
      CHECK(result_i2.has_value());
      CHECK_EQUAL(int32_t(0x00DCBA98), result_i2.value());

      auto result_s2 = bit_stream.read<uint16_t>(11U);
      CHECK(result_s2.has_value());
      CHECK_EQUAL(uint16_t(0x0678), result_s2.value());

      auto result_c2 = bit_stream.read<int8_t>(7U);
      CHECK(result_c2.has_value());
      CHECK_EQUAL(int8_t(0x25), result_c2.value());
    }

    //*************************************************************************
    TEST(test_read_multiple_variable_size_using_span_unsigned_char_fixed_extent_input)
    {
      //int8_t   c1 = 90;         // 0x5A       6 bits
      //uint16_t s1 = 4660;       // 0x1234     13 bits
      //int32_t  i1 = 0x89ABCDEF; // 0x89ABCDEF 23 bits
      //int32_t  i2 = 0xFEDCBA98; // 0xFEDCBA98 25 bits
      //uint16_t s2 = 22136;      // 0x5678     11 bits
      //int8_t   c2 = -91;        // 0xA5       7 bits

      std::array<unsigned char, 11U> storage = { (unsigned char)(0x6A), (unsigned char)(0x46), (unsigned char)(0x8A), (unsigned char)(0xF3),
                                                 (unsigned char)(0x7B), (unsigned char)(0xDB), (unsigned char)(0x97), (unsigned char)(0x53),
                                                 (unsigned char)(0x19), (unsigned char)(0xE1), (unsigned char)(0x28) };

      auto storage_span = etl::span<unsigned char, 11U>(storage.data(), storage.size());

      etl::bit_stream_reader bit_stream(storage_span, etl::endian::big);

      CHECK_EQUAL(storage.size(), bit_stream.size_bytes());

      auto result_c1 = bit_stream.read<int8_t>(6U);
      CHECK(result_c1.has_value());
      CHECK_EQUAL(int8_t(0x1A), result_c1.value());

      auto result_s1 = bit_stream.read<uint16_t>(13U);
      CHECK(result_s1.has_value());
      CHECK_EQUAL(uint16_t(0x1234), result_s1.value());

      auto result_i1 = bit_stream.read<int32_t>(23U);
      CHECK(result_i1.has_value());
      CHECK_EQUAL(int32_t(0x002BCDEF), result_i1.value());

      auto result_i2 = bit_stream.read<int32_t>(25U);
      CHECK(result_i2.has_value());
      CHECK_EQUAL(int32_t(0x00DCBA98), result_i2.value());

      auto result_s2 = bit_stream.read<uint16_t>(11U);
      CHECK(result_s2.has_value());
      CHECK_EQUAL(uint16_t(0x0678), result_s2.value());

      auto result_c2 = bit_stream.read<int8_t>(7U);
      CHECK(result_c2.has_value());
      CHECK_EQUAL(int8_t(0x25), result_c2.value());
    }

    //*************************************************************************
    TEST(test_read_multiple_variable_size_using_span_const_char_dynamic_extent_input)
    {
      //int8_t   c1 = 90;         // 0x5A       6 bits
      //uint16_t s1 = 4660;       // 0x1234     13 bits
      //int32_t  i1 = 0x89ABCDEF; // 0x89ABCDEF 23 bits
      //int32_t  i2 = 0xFEDCBA98; // 0xFEDCBA98 25 bits
      //uint16_t s2 = 22136;      // 0x5678     11 bits
      //int8_t   c2 = -91;        // 0xA5       7 bits

      std::array<const char, 11U> storage = { char(0x6A), char(0x46), char(0x8A), char(0xF3),
                                              char(0x7B), char(0xDB), char(0x97), char(0x53),
                                              char(0x19), char(0xE1), char(0x28) };

      auto storage_span = etl::span<const char, etl::dynamic_extent>(storage.data(), storage.size());

      etl::bit_stream_reader bit_stream(storage_span, etl::endian::big);

      CHECK_EQUAL(storage.size(), bit_stream.size_bytes());

      auto result_c1 = bit_stream.read<int8_t>(6U);
      CHECK(result_c1.has_value());
      CHECK_EQUAL(int8_t(0x1A), result_c1.value());

      auto result_s1 = bit_stream.read<uint16_t>(13U);
      CHECK(result_s1.has_value());
      CHECK_EQUAL(uint16_t(0x1234), result_s1.value());

      auto result_i1 = bit_stream.read<int32_t>(23U);
      CHECK(result_i1.has_value());
      CHECK_EQUAL(int32_t(0x002BCDEF), result_i1.value());

      auto result_i2 = bit_stream.read<int32_t>(25U);
      CHECK(result_i2.has_value());
      CHECK_EQUAL(int32_t(0x00DCBA98), result_i2.value());

      auto result_s2 = bit_stream.read<uint16_t>(11U);
      CHECK(result_s2.has_value());
      CHECK_EQUAL(uint16_t(0x0678), result_s2.value());

      auto result_c2 = bit_stream.read<int8_t>(7U);
      CHECK(result_c2.has_value());
      CHECK_EQUAL(int8_t(0x25), result_c2.value());
    }

    //*************************************************************************
    TEST(test_read_multiple_variable_size_using_span_const_unsigned_char_dynamic_extent_input)
    {
      //int8_t   c1 = 90;         // 0x5A       6 bits
      //uint16_t s1 = 4660;       // 0x1234     13 bits
      //int32_t  i1 = 0x89ABCDEF; // 0x89ABCDEF 23 bits
      //int32_t  i2 = 0xFEDCBA98; // 0xFEDCBA98 25 bits
      //uint16_t s2 = 22136;      // 0x5678     11 bits
      //int8_t   c2 = -91;        // 0xA5       7 bits

      std::array<const unsigned char, 11U> storage = { (unsigned char)(0x6A), (unsigned char)(0x46), (unsigned char)(0x8A), (unsigned char)(0xF3),
                                                       (unsigned char)(0x7B), (unsigned char)(0xDB), (unsigned char)(0x97), (unsigned char)(0x53),
                                                       (unsigned char)(0x19), (unsigned char)(0xE1), (unsigned char)(0x28) };

      auto storage_span = etl::span<const unsigned char, etl::dynamic_extent>(storage.data(), storage.size());

      etl::bit_stream_reader bit_stream(storage_span, etl::endian::big);

      CHECK_EQUAL(storage.size(), bit_stream.size_bytes());

      auto result_c1 = bit_stream.read<int8_t>(6U);
      CHECK(result_c1.has_value());
      CHECK_EQUAL(int8_t(0x1A), result_c1.value());

      auto result_s1 = bit_stream.read<uint16_t>(13U);
      CHECK(result_s1.has_value());
      CHECK_EQUAL(uint16_t(0x1234), result_s1.value());

      auto result_i1 = bit_stream.read<int32_t>(23U);
      CHECK(result_i1.has_value());
      CHECK_EQUAL(int32_t(0x002BCDEF), result_i1.value());

      auto result_i2 = bit_stream.read<int32_t>(25U);
      CHECK(result_i2.has_value());
      CHECK_EQUAL(int32_t(0x00DCBA98), result_i2.value());

      auto result_s2 = bit_stream.read<uint16_t>(11U);
      CHECK(result_s2.has_value());
      CHECK_EQUAL(uint16_t(0x0678), result_s2.value());

      auto result_c2 = bit_stream.read<int8_t>(7U);
      CHECK(result_c2.has_value());
      CHECK_EQUAL(int8_t(0x25), result_c2.value());
    }

    //*************************************************************************
    TEST(test_read_multiple_variable_size_using_span_const_char_fixed_extent_input)
    {
      //int8_t   c1 = 90;         // 0x5A       6 bits
      //uint16_t s1 = 4660;       // 0x1234     13 bits
      //int32_t  i1 = 0x89ABCDEF; // 0x89ABCDEF 23 bits
      //int32_t  i2 = 0xFEDCBA98; // 0xFEDCBA98 25 bits
      //uint16_t s2 = 22136;      // 0x5678     11 bits
      //int8_t   c2 = -91;        // 0xA5       7 bits

      std::array<const char, 11U> storage = { char(0x6A), char(0x46), char(0x8A), char(0xF3),
                                              char(0x7B), char(0xDB), char(0x97), char(0x53),
                                              char(0x19), char(0xE1), char(0x28) };

      auto storage_span = etl::span<const char, 11U>(storage.data(), storage.size());

      etl::bit_stream_reader bit_stream(storage_span, etl::endian::big);

      CHECK_EQUAL(storage.size(), bit_stream.size_bytes());

      auto result_c1 = bit_stream.read<int8_t>(6U);
      CHECK(result_c1.has_value());
      CHECK_EQUAL(int8_t(0x1A), result_c1.value());

      auto result_s1 = bit_stream.read<uint16_t>(13U);
      CHECK(result_s1.has_value());
      CHECK_EQUAL(uint16_t(0x1234), result_s1.value());

      auto result_i1 = bit_stream.read<int32_t>(23U);
      CHECK(result_i1.has_value());
      CHECK_EQUAL(int32_t(0x002BCDEF), result_i1.value());

      auto result_i2 = bit_stream.read<int32_t>(25U);
      CHECK(result_i2.has_value());
      CHECK_EQUAL(int32_t(0x00DCBA98), result_i2.value());

      auto result_s2 = bit_stream.read<uint16_t>(11U);
      CHECK(result_s2.has_value());
      CHECK_EQUAL(uint16_t(0x0678), result_s2.value());

      auto result_c2 = bit_stream.read<int8_t>(7U);
      CHECK(result_c2.has_value());
      CHECK_EQUAL(int8_t(0x25), result_c2.value());
    }

    //*************************************************************************
    TEST(test_read_multiple_variable_size_using_span_const_unsigned_char_fixed_extent_input)
    {
      //int8_t   c1 = 90;         // 0x5A       6 bits
      //uint16_t s1 = 4660;       // 0x1234     13 bits
      //int32_t  i1 = 0x89ABCDEF; // 0x89ABCDEF 23 bits
      //int32_t  i2 = 0xFEDCBA98; // 0xFEDCBA98 25 bits
      //uint16_t s2 = 22136;      // 0x5678     11 bits
      //int8_t   c2 = -91;        // 0xA5       7 bits

      std::array<const unsigned char, 11U> storage = { (unsigned char)(0x6A), (unsigned char)(0x46), (unsigned char)(0x8A), (unsigned char)(0xF3),
                                                       (unsigned char)(0x7B), (unsigned char)(0xDB), (unsigned char)(0x97), (unsigned char)(0x53),
                                                       (unsigned char)(0x19), (unsigned char)(0xE1), (unsigned char)(0x28) };

      auto storage_span = etl::span<const unsigned char, 11U>(storage.data(), storage.size());

      etl::bit_stream_reader bit_stream(storage_span, etl::endian::big);

      CHECK_EQUAL(storage.size(), bit_stream.size_bytes());

      auto result_c1 = bit_stream.read<int8_t>(6U);
      CHECK(result_c1.has_value());
      CHECK_EQUAL(int8_t(0x1A), result_c1.value());

      auto result_s1 = bit_stream.read<uint16_t>(13U);
      CHECK(result_s1.has_value());
      CHECK_EQUAL(uint16_t(0x1234), result_s1.value());

      auto result_i1 = bit_stream.read<int32_t>(23U);
      CHECK(result_i1.has_value());
      CHECK_EQUAL(int32_t(0x002BCDEF), result_i1.value());

      auto result_i2 = bit_stream.read<int32_t>(25U);
      CHECK(result_i2.has_value());
      CHECK_EQUAL(int32_t(0x00DCBA98), result_i2.value());

      auto result_s2 = bit_stream.read<uint16_t>(11U);
      CHECK(result_s2.has_value());
      CHECK_EQUAL(uint16_t(0x0678), result_s2.value());

      auto result_c2 = bit_stream.read<int8_t>(7U);
      CHECK(result_c2.has_value());
      CHECK_EQUAL(int8_t(0x25), result_c2.value());
    }

    //*************************************************************************
    TEST(test_read_multiple_variable_size_using_non_member_functions)
    {
      //int8_t   c1 = 90;         // 0x5A       6 bits
      //uint16_t s1 = 4660;       // 0x1234     13 bits
      //int32_t  i1 = 0x89ABCDEF; // 0x89ABCDEF 23 bits
      //int32_t  i2 = 0xFEDCBA98; // 0xFEDCBA98 25 bits
      //uint16_t s2 = 22136;      // 0x5678     11 bits
      //int8_t   c2 = -91;        // 0xA5       7 bits

      std::array<char, 11U> storage = { char(0x6A), char(0x46), char(0x8A), char(0xF3),
                                        char(0x7B), char(0xDB), char(0x97), char(0x53),
                                        char(0x19), char(0xE1), char(0x28) };

      etl::bit_stream_reader bit_stream(storage.data(), storage.size(), etl::endian::big);

      CHECK_EQUAL(storage.size(), bit_stream.size_bytes());

      auto result_c1 = etl::read<int8_t>(bit_stream, 6U);
      CHECK(result_c1.has_value());
      CHECK_EQUAL(int8_t(0x1A), result_c1.value());

      auto result_s1 = etl::read<uint16_t>(bit_stream, 13U);
      CHECK(result_s1.has_value());
      CHECK_EQUAL(uint16_t(0x1234), result_s1.value());

      auto result_i1 = etl::read<int32_t>(bit_stream, 23U);
      CHECK(result_i1.has_value());
      CHECK_EQUAL(int32_t(0x002BCDEF), result_i1.value());

      auto result_i2 = etl::read<int32_t>(bit_stream, 25U);
      CHECK(result_i2.has_value());
      CHECK_EQUAL(int32_t(0x00DCBA98), result_i2.value());

      auto result_s2 = etl::read<uint16_t>(bit_stream, 11U);
      CHECK(result_s2.has_value());
      CHECK_EQUAL(uint16_t(0x0678), result_s2.value());

      auto result_c2 = etl::read<int8_t>(bit_stream, 7U);
      CHECK(result_c2.has_value());
      CHECK_EQUAL(int8_t(0x25), result_c2.value());
    }

    //*************************************************************************
    TEST(test_read_checked_object)
    {
      std::array<char, 12U> storage = { char(0xEC), char(0xBA), char(0xDE), char(0x68),
                                        char(0xAF), char(0xD2), char(0xC5), char(0xC8),
                                        char(0x65), char(0xD3), char(0xDF), char(0x80) };

      etl::bit_stream_reader bit_stream(storage.data(), storage.size(), etl::endian::big);

      CHECK_EQUAL(storage.size(), bit_stream.size_bytes());

      Object object1 = { -1234, -2372331, 250 };
      Object object2 = {  5678,  2201423, 126 };

      etl::optional<Object> result1 = etl::read<Object>(bit_stream);
      etl::optional<Object> result2 = etl::read<Object>(bit_stream);

      CHECK(result1.has_value());
      CHECK(result2.has_value());

      CHECK_EQUAL(object1.s, result1.value().s);
      CHECK_EQUAL(object1.i, result1.value().i);
      CHECK_EQUAL(object1.c, result1.value().c);

      CHECK_EQUAL(object2.s, result2.value().s);
      CHECK_EQUAL(object2.i, result2.value().i);
      CHECK_EQUAL(object2.c, result2.value().c);
    }

    //*************************************************************************
    TEST(test_read_unchecked_object)
    {
      std::array<char, 12U> storage = { char(0xEC), char(0xBA), char(0xDE), char(0x68),
                                        char(0xAF), char(0xD2), char(0xC5), char(0xC8),
                                        char(0x65), char(0xD3), char(0xDF), char(0x80) };

      etl::bit_stream_reader bit_stream(storage.data(), storage.size(), etl::endian::big);

      CHECK_EQUAL(storage.size(), bit_stream.size_bytes());

      Object object1 = { -1234, -2372331, 250 };
      Object object2 = {  5678,  2201423, 126 };

      Object result1 = etl::read_unchecked<Object>(bit_stream);
      Object result2 = etl::read_unchecked<Object>(bit_stream);

      CHECK_EQUAL(object1.s, result1.s);
      CHECK_EQUAL(object1.i, result1.i);
      CHECK_EQUAL(object1.c, result1.c);

      CHECK_EQUAL(object2.s, result2.s);
      CHECK_EQUAL(object2.i, result2.i);
      CHECK_EQUAL(object2.c, result2.c);
    }
  };
}

#include "etl/private/diagnostic_pop.h"
#include "etl/private/diagnostic_pop.h"
