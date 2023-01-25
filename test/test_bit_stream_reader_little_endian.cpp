///******************************************************************************
//The MIT License(MIT)
//
//Embedded Template Library.
//https://github.com/ETLCPP/etl
//https://www.etlcpp.com
//
//Copyright(c) 2018 jwellbelove
//
//Permission is hereby granted, free of charge, to any person obtaining a copy
//of this software and associated documentation files(the "Software"), to deal
//in the Software without restriction, including without limitation the rights
//to use, copy, modify, merge, publish, distribute, sublicense, and / or sell
//copies of the Software, and to permit persons to whom the Software is
//furnished to do so, subject to the following conditions :
//
//The above copyright notice and this permission notice shall be included in all
//copies or substantial portions of the Software.
//
//THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
//AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
//OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
//SOFTWARE.
//******************************************************************************/
//
//#include "unit_test_framework.h"
//
//#include "etl/bit_stream.h"
//
//#include <array>
//#include <numeric>
//
//namespace
//{
//  //***********************************
//  struct Object
//  {
//    int16_t s;
//    int32_t i;
//    uint8_t c;
//  };
//
//#include "etl/private/diagnostic_unused_function_push.h"
//  bool operator ==(const Object& lhs, const Object& rhs)
//  {
//    return (lhs.s == rhs.s) &&
//           (lhs.i == rhs.i) &&
//           (lhs.c == rhs.c);
//  }
//
//  std::ostream& operator << (std::ostream& os, const Object& object)
//  {
//    os << object.s << "," << object.i << "," << (int)object.c;
//    return os;
//  }
//#include "etl/private/diagnostic_pop.h"
//}
//
//namespace etl
//{
//  //***********************************
//  template <>
//  Object read_unchecked<Object>(etl::bit_stream_reader& stream)
//  {
//    int16_t result_s = stream.read_unchecked<int16_t>(14);
//    int32_t result_i = stream.read_unchecked<int32_t>(23);
//    uint8_t result_c = stream.read_unchecked<uint8_t>();
//
//    return Object{ result_s, result_i, result_c };
//  }
//
//  //***********************************
//  template <>
//  optional<Object> read<Object>(etl::bit_stream_reader& stream)
//  {
//    optional<Object> result;
//
//    etl::optional<int16_t> result_s = stream.read<int16_t>(14);
//    etl::optional<int32_t> result_i = stream.read<int32_t>(23);
//    etl::optional<uint8_t> result_c = stream.read<uint8_t>();
//
//    if (result_s.has_value() && result_i.has_value() && result_c.has_value())
//    {
//      result = Object{ result_s.value(), result_i.value(), result_c.value() };
//    }
//
//    return result;
//  }
//}
//
//namespace
//{
//  SUITE(test_bit_stream_reader)
//  {
//    //*************************************************************************
//    TEST(test_read_bool)
//    {
//      char storage = 0x5AU;
//
//      etl::bit_stream_reader bit_stream(&storage, 1, etl::endian::little);
//
//      CHECK_EQUAL(1U, bit_stream.size_bytes());
//
//      etl::optional<bool> result;
//
//      result.reset();
//      result = bit_stream.read<bool>();
//      CHECK(result.has_value());
//      CHECK_EQUAL(false, result.value());
//      
//      result.reset();
//      result = bit_stream.read<bool>();
//      CHECK(result.has_value());
//      CHECK_EQUAL(true, result.value());
//      
//      result.reset();
//      result = bit_stream.read<bool>();
//      CHECK(result.has_value());
//      CHECK_EQUAL(false, result.value());
//
//      result.reset();
//      result = bit_stream.read<bool>();
//      CHECK(result.has_value());
//      CHECK_EQUAL(true, result.value());
//
//      result.reset();
//      result = bit_stream.read<bool>();
//      CHECK(result.has_value());
//      CHECK_EQUAL(true, result.value());
//
//      result.reset();
//      result = bit_stream.read<bool>();
//      CHECK(result.has_value());
//      CHECK_EQUAL(false, result.value());
//
//      result.reset();
//      result = bit_stream.read<bool>();
//      CHECK(result.has_value());
//      CHECK_EQUAL(true, result.value());
//
//      result.reset();
//      result = bit_stream.read<bool>();
//      CHECK(result.has_value());
//      CHECK_EQUAL(false, result.value());
//
//      // One too many.
//      CHECK_THROW(bit_stream.read<bool>(), etl::bit_stream_overflow);
//    }
//
//    //*************************************************************************
//    TEST(test_read_bool_using_non_member_function)
//    {
//      char storage = 0x5AU;
//
//      etl::bit_stream_reader bit_stream(&storage, 1, etl::endian::little);
//
//      CHECK_EQUAL(1U, bit_stream.size_bytes());
//
//      etl::optional<bool> result;
//
//      result.reset();
//      result = etl::read<bool>(bit_stream);
//      CHECK(result.has_value());
//      CHECK_EQUAL(false, result.value());
//
//      result.reset();
//      result = etl::read<bool>(bit_stream);
//      CHECK(result.has_value());
//      CHECK_EQUAL(true, result.value());
//
//      result.reset();
//      result = etl::read<bool>(bit_stream);
//      CHECK(result.has_value());
//      CHECK_EQUAL(false, result.value());
//
//      result.reset();
//      result = etl::read<bool>(bit_stream);
//      CHECK(result.has_value());
//      CHECK_EQUAL(true, result.value());
//
//      result.reset();
//      result = etl::read<bool>(bit_stream);
//      CHECK(result.has_value());
//      CHECK_EQUAL(true, result.value());
//
//      result.reset();
//      result = etl::read<bool>(bit_stream);
//      CHECK(result.has_value());
//      CHECK_EQUAL(false, result.value());
//
//      result.reset();
//      result = etl::read<bool>(bit_stream);
//      CHECK(result.has_value());
//      CHECK_EQUAL(true, result.value());
//
//      result.reset();
//      result = etl::read<bool>(bit_stream);
//      CHECK(result.has_value());
//      CHECK_EQUAL(false, result.value());
//
//      // One too many.
//      CHECK_THROW(etl::read<bool>(bit_stream), etl::bit_stream_overflow);
//    }
//
//    //*************************************************************************
//    TEST(test_read_int8_t)
//    {
//      std::array storage = { char(0x80), char(0x5A), char(0xA5), char(0xFF) };
//      std::array expected = { int8_t(0x01), int8_t(0x5A), int8_t(0xA5), int8_t(0xFF) };
//
//      etl::bit_stream_reader bit_stream(storage.data(), storage.size(), etl::endian::little);
//
//      CHECK_EQUAL(storage.size(), bit_stream.size_bytes());
//
//      etl::optional<int8_t> result;
//
//      result.reset();
//      result = bit_stream.read<int8_t>();
//      CHECK(result.has_value());
//      CHECK_EQUAL(int(expected[0]), int(result.value()));
//
//      result.reset();
//      result = bit_stream.read<int8_t>();
//      CHECK(result.has_value());
//      CHECK_EQUAL(int(expected[1]), int(result.value()));
//
//      result.reset();
//      result = bit_stream.read<int8_t>();
//      CHECK(result.has_value());
//      CHECK_EQUAL(int(expected[2]), int(result.value()));
//
//      result.reset();
//      result = bit_stream.read<int8_t>();
//      CHECK(result.has_value());
//      CHECK_EQUAL(int(expected[3]), int(result.value()));
//
//      // One too many.
//      CHECK_THROW(bit_stream.read<int8_t>(), etl::bit_stream_overflow);
//    }
//
//    //*************************************************************************
//    TEST(test_read_checked_int8_t_using_non_member_function)
//    {
//      std::array storage = { char(0x80), char(0x5A), char(0xA5), char(0xFF) };
//      std::array expected = { int8_t(0x01), int8_t(0x5A), int8_t(0xA5), int8_t(0xFF) };
//
//      etl::bit_stream_reader bit_stream(storage.data(), storage.size(), etl::endian::little);
//
//      CHECK_EQUAL(storage.size(), bit_stream.size_bytes());
//
//      etl::optional<int8_t> result;
//
//      result.reset();
//      result = etl::read<int8_t>(bit_stream);
//      CHECK(result.has_value());
//      CHECK_EQUAL(int(expected[0]), int(result.value()));
//
//      result.reset();
//      result = etl::read<int8_t>(bit_stream);
//      CHECK(result.has_value());
//      CHECK_EQUAL(int(expected[1]), int(result.value()));
//
//      result.reset();
//      result = etl::read<int8_t>(bit_stream);
//      CHECK(result.has_value());
//      CHECK_EQUAL(int(expected[2]), int(result.value()));
//
//      result.reset();
//      result = etl::read<int8_t>(bit_stream);
//      CHECK(result.has_value());
//      CHECK_EQUAL(int(expected[3]), int(result.value()));
//
//      // One too many.
//      CHECK_THROW(etl::read<int8_t>(bit_stream), etl::bit_stream_overflow);
//    }
//
//    //*************************************************************************
//    TEST(test_read_unchecked_int8_t_using_non_member_function)
//    {
//      std::array storage = { char(0x80), char(0x5A), char(0xA5), char(0xFF) };
//      std::array expected = { int8_t(0x01), int8_t(0x5A), int8_t(0xA5), int8_t(0xFF) };
//
//      etl::bit_stream_reader bit_stream(storage.data(), storage.size(), etl::endian::little);
//
//      CHECK_EQUAL(storage.size(), bit_stream.size_bytes());
//
//      int8_t result;
//
//      result = etl::read_unchecked<int8_t>(bit_stream);
//      CHECK_EQUAL(int(expected[0]), int(result));
//
//      result = etl::read_unchecked<int8_t>(bit_stream);
//      CHECK_EQUAL(int(expected[1]), int(result));
//
//      result = etl::read_unchecked<int8_t>(bit_stream);
//      CHECK_EQUAL(int(expected[2]), int(result));
//
//      result = etl::read_unchecked<int8_t>(bit_stream);
//      CHECK_EQUAL(int(expected[3]), int(result));
//    }
//    
//    //*************************************************************************
//    TEST(test_read_int8_t_5bits)
//    {
//      std::array storage = { char(0x85), char(0x69), char(0xF0) };
//      std::array expected = { int8_t(0x01), int8_t(0xF5), int8_t(0x05), int8_t(0xFF) };
//
//      etl::bit_stream_reader bit_stream(storage.data(), storage.size(), etl::endian::little);
//
//      CHECK_EQUAL(storage.size(), bit_stream.size_bytes());
//
//      etl::optional<int8_t> result;
//
//      result.reset();
//      result = bit_stream.read<int8_t>(5U);
//      CHECK(result.has_value());
//      CHECK_EQUAL(int(expected[0]), int(result.value()));
//
//      result.reset();
//      result = bit_stream.read<int8_t>(5U);
//      CHECK(result.has_value());
//      CHECK_EQUAL(int(expected[1]), int(result.value()));
//
//      result.reset();
//      result = bit_stream.read<int8_t>(5U);
//      CHECK(result.has_value());
//      CHECK_EQUAL(int(expected[2]), int(result.value()));
//
//      result.reset();
//      result = bit_stream.read<int8_t>(5U);
//      CHECK(result.has_value());
//      CHECK_EQUAL(int(expected[3]), int(result.value()));
//
//      // One too many.
//      CHECK_THROW(bit_stream.read<int8_t>(5U), etl::bit_stream_overflow);
//    }
//
//    //*************************************************************************
//    TEST(test_read_checked_int8_t_5bits_using_non_member_function)
//    {
//      std::array storage = { char(0x85), char(0x69), char(0xF0) };
//      std::array expected = { int8_t(0x01), int8_t(0xF5), int8_t(0x05), int8_t(0xFF) };
//
//      etl::bit_stream_reader bit_stream(storage.data(), storage.size(), etl::endian::little);
//
//      CHECK_EQUAL(storage.size(), bit_stream.size_bytes());
//
//      etl::optional<int8_t> result;
//
//      result.reset();
//      result = etl::read<int8_t>(bit_stream, 5U);
//      CHECK(result.has_value());
//      CHECK_EQUAL(int(expected[0]), int(result.value()));
//
//      result.reset();
//      result = etl::read<int8_t>(bit_stream, 5U);
//      CHECK(result.has_value());
//      CHECK_EQUAL(int(expected[1]), int(result.value()));
//
//      result.reset();
//      result = etl::read<int8_t>(bit_stream, 5U);
//      CHECK(result.has_value());
//      CHECK_EQUAL(int(expected[2]), int(result.value()));
//
//      result.reset();
//      result = etl::read<int8_t>(bit_stream, 5U);
//      CHECK(result.has_value());
//      CHECK_EQUAL(int(expected[3]), int(result.value()));
//
//      // One too many.
//      CHECK_THROW(etl::read<int8_t>(bit_stream, 5U), etl::bit_stream_overflow);
//    }
//
//    //*************************************************************************
//    TEST(test_read_unchecked_int8_t_5bits_using_non_member_function)
//    {
//      std::array storage = { char(0x85), char(0x69), char(0xF0) };
//      std::array expected = { int8_t(0x01), int8_t(0xF5), int8_t(0x05), int8_t(0xFF) };
//
//      etl::bit_stream_reader bit_stream(storage.data(), storage.size(), etl::endian::little);
//
//      CHECK_EQUAL(storage.size(), bit_stream.size_bytes());
//
//      int8_t result;
//
//      result = etl::read_unchecked<int8_t>(bit_stream, 5U);
//      CHECK_EQUAL(int(expected[0]), int(result));
//
//      result = etl::read_unchecked<int8_t>(bit_stream, 5U);
//      CHECK_EQUAL(int(expected[1]), int(result));
//
//      result = etl::read_unchecked<int8_t>(bit_stream, 5U);
//      CHECK_EQUAL(int(expected[2]), int(result));
//
//      result = etl::read_unchecked<int8_t>(bit_stream, 5U);
//      CHECK_EQUAL(int(expected[3]), int(result));
//    }
//
//    //*************************************************************************
//    TEST(test_read_uint8_t)
//    {
//    std::array storage = { char(0x80), char(0x5A), char(0xA5), char(0xFF) };
//    std::array expected = { uint8_t(0x01), uint8_t(0x5A), uint8_t(0xA5), uint8_t(0xFF) };
//
//      etl::bit_stream_reader bit_stream(storage.data(), storage.size(), etl::endian::little);
//
//      CHECK_EQUAL(storage.size(), bit_stream.size_bytes());
//
//      etl::optional<uint8_t> result;
//
//      result.reset();
//      result = bit_stream.read<uint8_t>();
//      CHECK(result.has_value());
//      CHECK_EQUAL(int(expected[0]), int(result.value()));
//
//      result.reset();
//      result = bit_stream.read<uint8_t>();
//      CHECK(result.has_value());
//      CHECK_EQUAL(int(expected[1]), int(result.value()));
//
//      result.reset();
//      result = bit_stream.read<uint8_t>();
//      CHECK(result.has_value());
//      CHECK_EQUAL(int(expected[2]), int(result.value()));
//
//      result.reset();
//      result = bit_stream.read<uint8_t>();
//      CHECK(result.has_value());
//      CHECK_EQUAL(int(expected[3]), int(result.value()));
//
//      // One too many.
//      CHECK_THROW(bit_stream.read<uint8_t>(), etl::bit_stream_overflow);
//    }
//
//    //*************************************************************************
//    TEST(test_read_uint8_t_5bits)
//    {
//      std::array storage = { char(0x85), char(0x69), char(0xF0) };
//      std::array expected = { uint8_t(0x01), uint8_t(0x15), uint8_t(0x05), uint8_t(0x1F) };
//
//      etl::bit_stream_reader bit_stream(storage.data(), storage.size(), etl::endian::little);
//
//      CHECK_EQUAL(storage.size(), bit_stream.size_bytes());
//
//      etl::optional<uint8_t> result;
//
//      result.reset();
//      result = bit_stream.read<uint8_t>(5U);
//      CHECK(result.has_value());
//      CHECK_EQUAL(int(expected[0]), int(result.value()));
//
//      result.reset();
//      result = bit_stream.read<uint8_t>(5U);
//      CHECK(result.has_value());
//      CHECK_EQUAL(int(expected[1]), int(result.value()));
//
//      result.reset();
//      result = bit_stream.read<uint8_t>(5U);
//      CHECK(result.has_value());
//      CHECK_EQUAL(int(expected[2]), int(result.value()));
//
//      result.reset();
//      result = bit_stream.read<uint8_t>(5U);
//      CHECK(result.has_value());
//      CHECK_EQUAL(int(expected[3]), int(result.value()));
//
//      // One too many.
//      CHECK_THROW(bit_stream.read<uint8_t>(5U), etl::bit_stream_overflow);
//    }
//
//    //*************************************************************************
//    TEST(test_read_uint8_t_5bits_with_skip)
//    {
//      std::array storage = { char(0x85), char(0x69), char(0xF0) };
//      std::array expected = { uint8_t(0x01), uint8_t(0x15), uint8_t(0x05), uint8_t(0x1F) };
//
//      etl::bit_stream_reader bit_stream(storage.data(), storage.size(), etl::endian::little);
//
//      CHECK_EQUAL(storage.size(), bit_stream.size_bytes());
//
//      etl::optional<uint8_t> result;
//
//      result.reset();
//      result = bit_stream.read<uint8_t>(5U);
//      CHECK(result.has_value());
//      CHECK_EQUAL(int(expected[0]), int(result.value()));
//
//      bool success = bit_stream.skip(5U);
//      CHECK(success);
//
//      result.reset();
//      result = bit_stream.read<uint8_t>(5U);
//      CHECK(result.has_value());
//      CHECK_EQUAL(int(expected[2]), int(result.value()));
//
//      result.reset();
//      result = bit_stream.read<uint8_t>(5U);
//      CHECK(result.has_value());
//      CHECK_EQUAL(int(expected[3]), int(result.value()));
//
//      // One too many.
//      CHECK_THROW(bit_stream.read<uint8_t>(5U), etl::bit_stream_overflow);
//    }
//
//    //*************************************************************************
//    TEST(test_read_int16_t)
//    {
//      std::array storage = { char(0x80), char(0x00), char(0xA5), char(0x5A),
//                             char(0x5A), char(0xA5), char(0xFF), char(0xFF) };
//      std::array expected = { int16_t(0x0001), int16_t(0x5AA5), int16_t(0xA55A), int16_t(0xFFFF) };
//
//      etl::bit_stream_reader bit_stream(storage.data(), storage.size(), etl::endian::little);
//
//      CHECK_EQUAL(storage.size(), bit_stream.size_bytes());
//
//      etl::optional<int16_t> result;
//
//      result.reset();
//      result = bit_stream.read<int16_t>();
//      CHECK(result.has_value());
//      CHECK_EQUAL(expected[0], result.value());
//
//      result.reset();
//      result = bit_stream.read<int16_t>();
//      CHECK(result.has_value());
//      CHECK_EQUAL(expected[1], result.value());
//
//      result.reset();
//      result = bit_stream.read<int16_t>();
//      CHECK(result.has_value());
//      CHECK_EQUAL(expected[2], result.value());
//
//      result.reset();
//      result = bit_stream.read<int16_t>();
//      CHECK(result.has_value());
//      CHECK_EQUAL(expected[3], result.value());
//
//      // One too many.
//      CHECK_THROW(bit_stream.read<int16_t>(), etl::bit_stream_overflow);
//    }
//
//    //*************************************************************************
//    TEST(test_read_int16_t_10bits)
//    {
//      std::array storage = { char(0x80), char(0x16), char(0xAA), char(0x57), char(0xFF) };
//      std::array expected = { int16_t(0x0001), int16_t(0x015A), int16_t(0xFEA5), int16_t(0xFFFF) };
//
//      etl::bit_stream_reader bit_stream(storage.data(), storage.size(), etl::endian::little);
//
//      CHECK_EQUAL(storage.size(), bit_stream.size_bytes());
//
//      etl::optional<int16_t> result;
//
//      result.reset();
//      result = bit_stream.read<int16_t>(10U);
//      CHECK(result.has_value());
//      CHECK_EQUAL(expected[0], result.value());
//
//      result.reset();
//      result = bit_stream.read<int16_t>(10U);
//      CHECK(result.has_value());
//      CHECK_EQUAL(expected[1], result.value());
//
//      result.reset();
//      result = bit_stream.read<int16_t>(10U);
//      CHECK(result.has_value());
//      CHECK_EQUAL(expected[2], result.value());
//
//      result.reset();
//      result = bit_stream.read<int16_t>(10U);
//      CHECK(result.has_value());
//      CHECK_EQUAL(expected[3], result.value());
//
//      // One too many.
//      CHECK_THROW(bit_stream.read<int16_t>(10U), etl::bit_stream_overflow);
//    }
//
//    //*************************************************************************
//    TEST(test_read_uint16_t)
//    {
//      std::array storage = { char(0x80), char(0x00), char(0xA5), char(0x5A),
//                             char(0x5A), char(0xA5), char(0xFF), char(0xFF) };
//      std::array expected = { uint16_t(0x0001), uint16_t(0x5AA5), uint16_t(0xA55A), uint16_t(0xFFFF) };
//
//      etl::bit_stream_reader bit_stream(storage.data(), storage.size(), etl::endian::little);
//
//      CHECK_EQUAL(storage.size(), bit_stream.size_bytes());
//
//      etl::optional<uint16_t> result;
//
//      result.reset();
//      result = bit_stream.read<uint16_t>();
//      CHECK(result.has_value());
//      CHECK_EQUAL(expected[0], result.value());
//
//      result.reset();
//      result = bit_stream.read<uint16_t>();
//      CHECK(result.has_value());
//      CHECK_EQUAL(expected[1], result.value());
//
//      result.reset();
//      result = bit_stream.read<uint16_t>();
//      CHECK(result.has_value());
//      CHECK_EQUAL(expected[2], result.value());
//
//      result.reset();
//      result = bit_stream.read<uint16_t>();
//      CHECK(result.has_value());
//      CHECK_EQUAL(expected[3], result.value());
//
//      // One too many.
//      CHECK_THROW(bit_stream.read<uint16_t>(), etl::bit_stream_overflow);
//    }
//
//    //*************************************************************************
//    TEST(test_read_uint16_t_10bits)
//    {
//      std::array storage = { char(0x80), char(0x16), char(0xAA), char(0x57), char(0xFF) };
//      std::array expected = { uint16_t(0x0001), uint16_t(0x015A), uint16_t(0x02A5), uint16_t(0x03FF) };
//
//      etl::bit_stream_reader bit_stream(storage.data(), storage.size(), etl::endian::little);
//
//      CHECK_EQUAL(storage.size(), bit_stream.size_bytes());
//
//      etl::optional<uint16_t> result;
//
//      result.reset();
//      result = bit_stream.read<uint16_t>(10U);
//      CHECK(result.has_value());
//      CHECK_EQUAL(expected[0], result.value());
//
//      result.reset();
//      result = bit_stream.read<uint16_t>(10U);
//      CHECK(result.has_value());
//      CHECK_EQUAL(expected[1], result.value());
//
//      result.reset();
//      result = bit_stream.read<uint16_t>(10U);
//      CHECK(result.has_value());
//      CHECK_EQUAL(expected[2], result.value());
//
//      result.reset();
//      result = bit_stream.read<uint16_t>(10U);
//      CHECK(result.has_value());
//      CHECK_EQUAL(expected[3], result.value());
//
//      // One too many.
//      CHECK_THROW(bit_stream.read<uint16_t>(10U), etl::bit_stream_overflow);
//    }
//
//    //*************************************************************************
//    TEST(test_read_int32_t)
//    {
//      std::array storage = { char(0x80), char(0x00), char(0x00), char(0x00),
//                             char(0x5A), char(0xA5), char(0xA5), char(0x5A),
//                             char(0xA5), char(0x5A), char(0x5A), char(0xA5),
//                             char(0xFF), char(0xFF), char(0xFF), char(0xFF) };
//      std::array expected = { int32_t(0x00000001), int32_t(0x5AA5A55A), int32_t(0xA55A5AA5), int32_t(0xFFFFFFFF) };
//
//      etl::bit_stream_reader bit_stream(storage.data(), storage.size(), etl::endian::little);
//
//      CHECK_EQUAL(storage.size(), bit_stream.size_bytes());
//
//      etl::optional<int32_t> result;
//
//      result.reset();
//      result = bit_stream.read<int32_t>();
//      CHECK(result.has_value());
//      CHECK_EQUAL(expected[0], result.value());
//
//      result.reset();
//      result = bit_stream.read<int32_t>();
//      CHECK(result.has_value());
//      CHECK_EQUAL(expected[1], result.value());
//
//      result.reset();
//      result = bit_stream.read<int32_t>();
//      CHECK(result.has_value());
//      CHECK_EQUAL(expected[2], result.value());
//
//      result.reset();
//      result = bit_stream.read<int32_t>();
//      CHECK(result.has_value());
//      CHECK_EQUAL(expected[3], result.value());
//
//      // One too many.
//      CHECK_THROW(bit_stream.read<int32_t>(), etl::bit_stream_overflow);
//    }
//
//    //*************************************************************************
//    TEST(test_read_int32_t_22bits)
//    {
//      std::array storage = { char(0x80), char(0x00), char(0x01), char(0x6A),
//                             char(0x95), char(0x6A), char(0x55), char(0xAA),
//                             char(0x7F), char(0xFF), char(0xFF) };
//      std::array expected = { int32_t(0x00000001), int32_t(0x001AA55A), int32_t(0xFFE55AA5), int32_t(0xFFFFFFFF) };
//
//      etl::bit_stream_reader bit_stream(storage.data(), storage.size(), etl::endian::little);
//
//      CHECK_EQUAL(storage.size(), bit_stream.size_bytes());
//
//      etl::optional<int32_t> result;
//
//      result.reset();
//      result = bit_stream.read<int32_t>(22U);
//      CHECK(result.has_value());
//      CHECK_EQUAL(expected[0], result.value());
//
//      result.reset();
//      result = bit_stream.read<int32_t>(22U);
//      CHECK(result.has_value());
//      CHECK_EQUAL(expected[1], result.value());
//
//      result.reset();
//      result = bit_stream.read<int32_t>(22U);
//      CHECK(result.has_value());
//      CHECK_EQUAL(expected[2], result.value());
//
//      result.reset();
//      result = bit_stream.read<int32_t>(22U);
//      CHECK(result.has_value());
//      CHECK_EQUAL(expected[3], result.value());
//
//      // One too many.
//      CHECK_THROW(bit_stream.read<int32_t>(22U), etl::bit_stream_overflow);
//    }
//
//    //*************************************************************************
//    TEST(test_read_uint32_t)
//    {
//    std::array storage = { char(0x80), char(0x00), char(0x00), char(0x00),
//                           char(0x5A), char(0xA5), char(0xA5), char(0x5A),
//                           char(0xA5), char(0x5A), char(0x5A), char(0xA5),
//                           char(0xFF), char(0xFF), char(0xFF), char(0xFF) };
//    std::array expected = { int32_t(0x00000001), int32_t(0x5AA5A55A), int32_t(0xA55A5AA5), int32_t(0xFFFFFFFF) };
//
//      etl::bit_stream_reader bit_stream(storage.data(), storage.size(), etl::endian::little);
//
//      CHECK_EQUAL(storage.size(), bit_stream.size_bytes());
//
//      etl::optional<uint32_t> result;
//
//      result.reset();
//      result = bit_stream.read<uint32_t>();
//      CHECK(result.has_value());
//      CHECK_EQUAL(expected[0], result.value());
//
//      result.reset();
//      result = bit_stream.read<uint32_t>();
//      CHECK(result.has_value());
//      CHECK_EQUAL(expected[1], result.value());
//
//      result.reset();
//      result = bit_stream.read<uint32_t>();
//      CHECK(result.has_value());
//      CHECK_EQUAL(expected[2], result.value());
//
//      result.reset();
//      result = bit_stream.read<uint32_t>();
//      CHECK(result.has_value());
//      CHECK_EQUAL(expected[3], result.value());
//
//      // One too many.
//      CHECK_THROW(bit_stream.read<uint32_t>(), etl::bit_stream_overflow);
//    }
//
//    //*************************************************************************
//    TEST(test_read_uint32_t_22bits)
//    {
//      std::array storage = { char(0x80), char(0x00), char(0x01), char(0x6A),
//                             char(0x95), char(0x6A), char(0x55), char(0xAA),
//                             char(0x7F), char(0xFF), char(0xFF) };
//      std::array expected = { uint32_t(0x00000001), uint32_t(0x001AA55A), uint32_t(0x00255AA5), uint32_t(0x003FFFFF) };
//
//      etl::bit_stream_reader bit_stream(storage.data(), storage.size(), etl::endian::little);
//
//      CHECK_EQUAL(storage.size(), bit_stream.size_bytes());
//
//      etl::optional<uint32_t> result;
//
//      result.reset();
//      result = bit_stream.read<uint32_t>(22U);
//      CHECK(result.has_value());
//      CHECK_EQUAL(expected[0], result.value());
//
//      result.reset();
//      result = bit_stream.read<uint32_t>(22U);
//      CHECK(result.has_value());
//      CHECK_EQUAL(expected[1], result.value());
//
//      result.reset();
//      result = bit_stream.read<uint32_t>(22U);
//      CHECK(result.has_value());
//      CHECK_EQUAL(expected[2], result.value());
//
//      result.reset();
//      result = bit_stream.read<uint32_t>(22U);
//      CHECK(result.has_value());
//      CHECK_EQUAL(expected[3], result.value());
//
//      // One too many.
//      CHECK_THROW(bit_stream.read<uint32_t>(22U), etl::bit_stream_overflow);
//    }
//
//    //*************************************************************************
//    TEST(test_read_int64_t)
//    {
//      std::array storage = { char(0x80), char(0x00), char(0x00), char(0x00), char(0x00), char(0x00), char(0x00), char(0x00),
//                             char(0xA5), char(0x5A), char(0x5A), char(0xA5), char(0x5A), char(0xA5), char(0xA5), char(0x5A),
//                             char(0x5A), char(0xA5), char(0xA5), char(0x5A), char(0xA5), char(0x5A), char(0x5A), char(0xA5),
//                             char(0xFF), char(0xFF), char(0xFF), char(0xFF), char(0xFF), char(0xFF), char(0xFF), char(0xFF) };
//      std::array expected = { int64_t(0x0000000000000001LL), int64_t(0x5AA5A55AA55A5AA5LL), int64_t(0xA55A5AA55AA5A55ALL), int64_t(0xFFFFFFFFFFFFFFFFLL) };
//
//      etl::bit_stream_reader bit_stream(storage.data(), storage.size(), etl::endian::little);
//
//      CHECK_EQUAL(storage.size(), bit_stream.size_bytes());
//
//      etl::optional<int64_t> result;
//
//      result.reset();
//      result = bit_stream.read<int64_t>();
//      CHECK(result.has_value());
//      CHECK_EQUAL(expected[0], result.value());
//
//      result.reset();
//      result = bit_stream.read<int64_t>();
//      CHECK(result.has_value());
//      CHECK_EQUAL(expected[1], result.value());
//
//      result.reset();
//      result = bit_stream.read<int64_t>();
//      CHECK(result.has_value());
//      CHECK_EQUAL(expected[2], result.value());
//
//      result.reset();
//      result = bit_stream.read<int64_t>();
//      CHECK(result.has_value());
//      CHECK_EQUAL(expected[3], result.value());
//
//      // One too many.
//      CHECK_THROW(bit_stream.read<int64_t>(), etl::bit_stream_overflow);
//    }
//
//    //*************************************************************************
//    TEST(test_read_int64_t_47bits)
//    {
//      std::array storage = { char(0x80), char(0x00), char(0x00), char(0x00),
//                             char(0x00), char(0x00), char(0xB5), char(0x4A),
//                             char(0xB5), char(0x4A), char(0xB5), char(0x4A),
//                             char(0x95), char(0x6A), char(0x95), char(0x6A),
//                             char(0x95), char(0x6F), char(0xFF), char(0xFF),
//                             char(0xFF), char(0xFF), char(0xFF), char(0xF0) };
//      std::array expected = { int64_t(0x0000000000000001LL), int64_t(0x0000255AA55AA55ALL), int64_t(0xFFFFDAA55AA55AA5LL), int64_t(0xFFFFFFFFFFFFFFFFLL) };
//
//      etl::bit_stream_reader bit_stream(storage.data(), storage.size(), etl::endian::little);
//
//      CHECK_EQUAL(storage.size(), bit_stream.size_bytes());
//
//      etl::optional<int64_t> result;
//
//      result.reset();
//      result = bit_stream.read<int64_t>(47U);
//      CHECK(result.has_value());
//      CHECK_EQUAL(expected[0], result.value());
//
//      result.reset();
//      result = bit_stream.read<int64_t>(47U);
//      CHECK(result.has_value());
//      CHECK_EQUAL(expected[1], result.value());
//
//      result.reset();
//      result = bit_stream.read<int64_t>(47U);
//      CHECK(result.has_value());
//      CHECK_EQUAL(expected[2], result.value());
//
//      result.reset();
//      result = bit_stream.read<int64_t>(47U);
//      CHECK(result.has_value());
//      CHECK_EQUAL(expected[3], result.value());
//
//      // One too many.
//      CHECK_THROW(bit_stream.read<int64_t>(47U), etl::bit_stream_overflow);
//    }
//
//    //*************************************************************************
//    TEST(test_read_uint64_t)
//    {
//      std::array storage = { char(0x80), char(0x00), char(0x00), char(0x00), char(0x00), char(0x00), char(0x00), char(0x00),
//                             char(0xA5), char(0x5A), char(0x5A), char(0xA5), char(0x5A), char(0xA5), char(0xA5), char(0x5A),
//                             char(0x5A), char(0xA5), char(0xA5), char(0x5A), char(0xA5), char(0x5A), char(0x5A), char(0xA5),
//                             char(0xFF), char(0xFF), char(0xFF), char(0xFF), char(0xFF), char(0xFF), char(0xFF), char(0xFF) };
//      std::array expected = { uint64_t(0x0000000000000001ULL), uint64_t(0x5AA5A55AA55A5AA5ULL), uint64_t(0xA55A5AA55AA5A55AULL), uint64_t(0xFFFFFFFFFFFFFFFFULL) };
//
//      etl::bit_stream_reader bit_stream(storage.data(), storage.size(), etl::endian::little);
//
//      CHECK_EQUAL(storage.size(), bit_stream.size_bytes());
//
//      etl::optional<uint64_t> result;
//
//      result.reset();
//      result = bit_stream.read<uint64_t>();
//      CHECK(result.has_value());
//      CHECK_EQUAL(expected[0], result.value());
//
//      result.reset();
//      result = bit_stream.read<uint64_t>();
//      CHECK(result.has_value());
//      CHECK_EQUAL(expected[1], result.value());
//
//      result.reset();
//      result = bit_stream.read<uint64_t>();
//      CHECK(result.has_value());
//      CHECK_EQUAL(expected[2], result.value());
//
//      result.reset();
//      result = bit_stream.read<uint64_t>();
//      CHECK(result.has_value());
//      CHECK_EQUAL(expected[3], result.value());
//
//      // One too many.
//      CHECK_THROW(bit_stream.read<uint64_t>(), etl::bit_stream_overflow);
//    }
//
//    //*************************************************************************
//    TEST(test_read_uint64_t_47bits)
//    {
//      std::array storage = { char(0x80), char(0x00), char(0x00), char(0x00),
//                             char(0x00), char(0x00), char(0xB5), char(0x4A),
//                             char(0xB5), char(0x4A), char(0xB5), char(0x4A),
//                             char(0x95), char(0x6A), char(0x95), char(0x6A),
//                             char(0x95), char(0x6F), char(0xFF), char(0xFF),
//                             char(0xFF), char(0xFF), char(0xFF), char(0xF0) };
//      std::array expected = { uint64_t(0x0000000000000001ULL), uint64_t(0x0000255AA55AA55AULL), uint64_t(0x00005AA55AA55AA5ULL), uint64_t(0x00007FFFFFFFFFFFULL) };
//
//      etl::bit_stream_reader bit_stream(storage.data(), storage.size(), etl::endian::little);
//
//      CHECK_EQUAL(storage.size(), bit_stream.size_bytes());
//
//      etl::optional<uint64_t> result;
//
//      result.reset();
//      result = bit_stream.read<uint64_t>(47U);
//      CHECK(result.has_value());
//      CHECK_EQUAL(expected[0], result.value());
//
//      result.reset();
//      result = bit_stream.read<uint64_t>(47U);
//      CHECK(result.has_value());
//      CHECK_EQUAL(expected[1], result.value());
//
//      result.reset();
//      result = bit_stream.read<uint64_t>(47U);
//      CHECK(result.has_value());
//      CHECK_EQUAL(expected[2], result.value());
//
//      result.reset();
//      result = bit_stream.read<uint64_t>(47U);
//      CHECK(result.has_value());
//      CHECK_EQUAL(expected[3], result.value());
//
//      // One too many.
//      CHECK_THROW(bit_stream.read<uint64_t>(47U), etl::bit_stream_overflow);
//    }
//
//    //*************************************************************************
//    TEST(test_read_multiple_full_size)
//    {
//      //int8_t   c1 = 90;         // 0x5A
//      //uint16_t s1 = 4660;       // 0x1234
//      //int32_t  i1 = 0x89ABCDEF; // 0x89ABCDEF
//      //int32_t  i2 = 0xFEDCBA98; // 0xFEDCBA98
//      //uint16_t s2 = 22136;      // 0x5678
//      //int8_t   c2 = -91;        // 0xA5
//
//      std::array storage = { char(0x5A),
//                             char(0x2C), char(0x48),
//                             char(0xF7), char(0xB3), char(0xD5), char(0x91),
//                             char(0x19), char(0x5D), char(0x3B), char(0x7F),
//                             char(0x1E), char(0x6A),
//                             char(0xA5) };
//
//      etl::bit_stream_reader bit_stream(storage.data(), storage.size(), etl::endian::little);
//
//      CHECK_EQUAL(storage.size(), bit_stream.size_bytes());
//
//      auto result_c1 = bit_stream.read<int8_t>();
//      CHECK(result_c1.has_value());
//      CHECK_EQUAL(int8_t(0x5A), result_c1.value());
//
//      auto result_s1 = bit_stream.read<uint16_t>();
//      CHECK(result_s1.has_value());
//      CHECK_EQUAL(uint16_t(0x1234), result_s1.value());
//
//      auto result_i1 = bit_stream.read<int32_t>();
//      CHECK(result_i1.has_value());
//      CHECK_EQUAL(int32_t(0x89ABCDEF), result_i1.value());
//
//      auto result_i2 = bit_stream.read<int32_t>();
//      CHECK(result_i2.has_value());
//      CHECK_EQUAL(int32_t(0xFEDCBA98), result_i2.value());
//
//      auto result_s2 = bit_stream.read<uint16_t>();
//      CHECK(result_s2.has_value());
//      CHECK_EQUAL(uint16_t(0x5678), result_s2.value());
//
//      auto result_c2 = bit_stream.read<int8_t>();
//      CHECK(result_c2.has_value());
//      CHECK_EQUAL(int8_t(0xA5), result_c2.value());
//    }
//
//    //*************************************************************************
//    TEST(test_read_multiple_variable_size)
//    {
//      //int8_t   c1 = 90;         // 0x5A       6 bits
//      //uint16_t s1 = 4660;       // 0x1234     13 bits
//      //int32_t  i1 = 0x89ABCDEF; // 0x89ABCDEF 23 bits
//      //int32_t  i2 = 0xFEDCBA98; // 0xFEDCBA98 25 bits
//      //uint16_t s2 = 22136;      // 0x5678     11 bits
//      //int8_t   c2 = -91;        // 0xA5       7 bits
//
//      std::array storage = { char(0x58), char(0xB1), char(0x3E), char(0xF6),
//                             char(0x7A), char(0x86), char(0x57), char(0x4E),
//                             char(0xC3), char(0xCE), char(0x90) };
//
//      etl::bit_stream_reader bit_stream(storage.data(), storage.size(), etl::endian::little);
//
//      CHECK_EQUAL(storage.size(), bit_stream.size_bytes());
//
//      auto result_c1 = bit_stream.read<int8_t>(6U);
//      CHECK(result_c1.has_value());
//      CHECK_EQUAL(int8_t(0x1A), result_c1.value());
//
//      auto result_s1 = bit_stream.read<uint16_t>(13U);
//      CHECK(result_s1.has_value());
//      CHECK_EQUAL(uint16_t(0x1234), result_s1.value());
//
//      auto result_i1 = bit_stream.read<int32_t>(23U);
//      CHECK(result_i1.has_value());
//      CHECK_EQUAL(int32_t(0x002BCDEF), result_i1.value());
//
//      auto result_i2 = bit_stream.read<int32_t>(25U);
//      CHECK(result_i2.has_value());
//      CHECK_EQUAL(int32_t(0x00DCBA98), result_i2.value());
//
//      auto result_s2 = bit_stream.read<uint16_t>(11U);
//      CHECK(result_s2.has_value());
//      CHECK_EQUAL(uint16_t(0x0678), result_s2.value());
//
//      auto result_c2 = bit_stream.read<int8_t>(7U);
//      CHECK(result_c2.has_value());
//      CHECK_EQUAL(int8_t(0x25), result_c2.value());
//    }
//
//    //*************************************************************************
//    TEST(test_read_multiple_variable_size_using_non_member_functions)
//    {
//      //int8_t   c1 = 90;         // 0x5A       6 bits
//      //uint16_t s1 = 4660;       // 0x1234     13 bits
//      //int32_t  i1 = 0x89ABCDEF; // 0x89ABCDEF 23 bits
//      //int32_t  i2 = 0xFEDCBA98; // 0xFEDCBA98 25 bits
//      //uint16_t s2 = 22136;      // 0x5678     11 bits
//      //int8_t   c2 = -91;        // 0xA5       7 bits
//
//      std::array storage = { char(0x58), char(0xB1), char(0x3E), char(0xF6),
//                             char(0x7A), char(0x86), char(0x57), char(0x4E),
//                             char(0xC3), char(0xCE), char(0x90) };
//
//      etl::bit_stream_reader bit_stream(storage.data(), storage.size(), etl::endian::little);
//
//      CHECK_EQUAL(storage.size(), bit_stream.size_bytes());
//
//      auto result_c1 = etl::read<int8_t>(bit_stream, 6U);
//      CHECK(result_c1.has_value());
//      CHECK_EQUAL(int8_t(0x1A), result_c1.value());
//
//      auto result_s1 = etl::read<uint16_t>(bit_stream, 13U);
//      CHECK(result_s1.has_value());
//      CHECK_EQUAL(uint16_t(0x1234), result_s1.value());
//
//      auto result_i1 = etl::read<int32_t>(bit_stream, 23U);
//      CHECK(result_i1.has_value());
//      CHECK_EQUAL(int32_t(0x002BCDEF), result_i1.value());
//
//      auto result_i2 = etl::read<int32_t>(bit_stream, 25U);
//      CHECK(result_i2.has_value());
//      CHECK_EQUAL(int32_t(0x00DCBA98), result_i2.value());
//
//      auto result_s2 = etl::read<uint16_t>(bit_stream, 11U);
//      CHECK(result_s2.has_value());
//      CHECK_EQUAL(uint16_t(0x0678), result_s2.value());
//
//      auto result_c2 = etl::read<int8_t>(bit_stream, 7U);
//      CHECK(result_c2.has_value());
//      CHECK_EQUAL(int8_t(0x25), result_c2.value());
//    }
//
//    //*************************************************************************
//    TEST(test_read_checked_object)
//    {
//      std::array storage = { char(0x74), char(0xDE), char(0xA2), char(0xCF),
//                             char(0x6A), char(0xFB), char(0xA3), char(0x5E),
//                             char(0x5D), char(0x30), char(0x9F), char(0x80) };
//
//      etl::bit_stream_reader bit_stream(storage.data(), storage.size(), etl::endian::little);
//
//      CHECK_EQUAL(storage.size(), bit_stream.size_bytes());
//
//      Object object1 = { -1234, -2372331, 250 };
//      Object object2 = {  5678,  2201423, 126 };
//
//      etl::optional<Object> result1 = etl::read<Object>(bit_stream);
//      etl::optional<Object> result2 = etl::read<Object>(bit_stream);
//
//      CHECK(result1.has_value());
//      CHECK(result2.has_value());
//
//      CHECK_EQUAL(object1.s, result1.value().s);
//      CHECK_EQUAL(object1.i, result1.value().i);
//      CHECK_EQUAL(object1.c, result1.value().c);
//
//      CHECK_EQUAL(object2.s, result2.value().s);
//      CHECK_EQUAL(object2.i, result2.value().i);
//      CHECK_EQUAL(object2.c, result2.value().c);
//    }
//
//    //*************************************************************************
//    TEST(test_read_unchecked_object)
//    {
//      std::array storage = { char(0x74), char(0xDE), char(0xA2), char(0xCF),
//                             char(0x6A), char(0xFB), char(0xA3), char(0x5E),
//                             char(0x5D), char(0x30), char(0x9F), char(0x80) };
//
//      etl::bit_stream_reader bit_stream(storage.data(), storage.size(), etl::endian::little);
//
//      CHECK_EQUAL(storage.size(), bit_stream.size_bytes());
//
//      Object object1 = { -1234, -2372331, 250 };
//      Object object2 = {  5678,  2201423, 126 };
//
//      Object result1 = etl::read_unchecked<Object>(bit_stream);
//      Object result2 = etl::read_unchecked<Object>(bit_stream);
//
//      CHECK_EQUAL(object1.s, result1.s);
//      CHECK_EQUAL(object1.i, result1.i);
//      CHECK_EQUAL(object1.c, result1.c);
//
//      CHECK_EQUAL(object2.s, result2.s);
//      CHECK_EQUAL(object2.i, result2.i);
//      CHECK_EQUAL(object2.c, result2.c);
//    }
//  };
//}
//
