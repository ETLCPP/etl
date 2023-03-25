///******************************************************************************
//The MIT License(MIT)
//
//Embedded Template Library.
//https://github.com/ETLCPP/etl
//https://www.etlcpp.com
//
//Copyright(c) 2022 jwellbelove
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
//#include <vector>
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
//  bool write(etl::bit_stream_writer& stream, const Object& object)
//  {
//    bool success = true;
//
//    if (!stream.write(object.s, 14))
//    {
//      success = false;
//    }
//
//    if (!stream.write(object.i, 23))
//    {
//      success = false;
//    }
//
//    if (!stream.write(object.c))
//    {
//      success = false;
//    }
//
//    return success;
//  }
//}
//
//namespace
//{
//  class Accumulator
//  {
//  public:
//
//    void Add(etl::bit_stream_writer::callback_parameter_type s)
//    {
//      data.insert(data.end(), s.begin(), s.end());
//    }
//
//    void Clear()
//    {
//      data.clear();
//    }
//
//    const std::vector<char>& GetData() const
//    {
//      return data;
//    }
//
//  private:
//
//    std::vector<char> data;
//  };
//
//  SUITE(test_bit_stream_big_endian)
//  {
//    //*************************************************************************
//    TEST(test_bit_stream_writer_construction)
//    {
//      std::array<char, 256> storage;
//
//      etl::bit_stream_writer bit_stream(storage.data(), storage.size(), etl::endian::big);
//
//      CHECK_EQUAL(storage.size(), bit_stream.available(CHAR_BIT));
//      CHECK_EQUAL(storage.size(), bit_stream.available<CHAR_BIT>());
//      CHECK_EQUAL(storage.size(), bit_stream.available<unsigned char>());
//
//      CHECK_EQUAL(storage.size() * CHAR_BIT, bit_stream.available_bits());
//
//      CHECK_EQUAL(0U, bit_stream.size_bits());
//      CHECK_EQUAL(0U, bit_stream.size_bytes());
//
//      CHECK_EQUAL(storage.size(), bit_stream.data().size());
//      CHECK_EQUAL(0U,             bit_stream.used_data().size());
//    }
//
//    //*************************************************************************
//    TEST(test_write_bool)
//    {
//      unsigned char storage = 0;
//      unsigned char expected = 0x5AU;
//
//      etl::bit_stream_writer bit_stream(&storage, 1U, etl::endian::big);
//
//      CHECK(bit_stream.write(false));
//      CHECK_EQUAL(1U, bit_stream.used_data().size());
//      CHECK(bit_stream.write(true));
//      CHECK_EQUAL(1U, bit_stream.used_data().size());
//      CHECK(bit_stream.write(false));
//      CHECK_EQUAL(1U, bit_stream.used_data().size());
//      CHECK(bit_stream.write(true));
//      CHECK_EQUAL(1U, bit_stream.used_data().size());
//      CHECK(bit_stream.write(true));
//      CHECK_EQUAL(1U, bit_stream.used_data().size());
//      CHECK(bit_stream.write(false));
//      CHECK_EQUAL(1U, bit_stream.used_data().size());
//      CHECK(bit_stream.write(true));
//      CHECK_EQUAL(1U, bit_stream.used_data().size());
//      CHECK(bit_stream.write(false));
//      CHECK_EQUAL(1U, bit_stream.used_data().size());
//
//      // One too many.
//      CHECK_THROW(bit_stream.write(true), etl::bit_stream_overflow);
//
//      CHECK_EQUAL(1U, bit_stream.data().size());
//      CHECK_EQUAL(1U, bit_stream.used_data().size());
//
//      CHECK_EQUAL(int(expected), int(storage));
//    }
//
//    //*************************************************************************
//    TEST(test_write_int8_t)
//    {
//      std::array<char, 256U> storage;
//
//      std::array<char, 256U> expected;
//      std::iota(expected.begin(), expected.end(), 0);
//
//      etl::bit_stream_writer bit_stream(storage.data(), storage.size(), etl::endian::big);
//
//      for (size_t i = 0UL; i < 256UL; ++i)
//      {
//        CHECK(bit_stream.write(int8_t(i)));
//        CHECK_EQUAL(i + 1U, bit_stream.used_data().size());
//      }
//
//      // One too many.
//      CHECK_THROW(bit_stream.write(int8_t(0)), etl::bit_stream_overflow);
//
//      CHECK_EQUAL(256U, bit_stream.data().size());
//      CHECK_EQUAL(256U, bit_stream.used_data().size());
//
//      for (size_t i = 0UL; i < storage.size(); ++i)
//      {
//        CHECK_EQUAL(int(expected[i]), int(storage[i]));
//      }
//    }
//
//    //*************************************************************************
//    TEST(test_write_uint8_t)
//    {
//      std::array<char, 256> storage;
//
//      std::array<char, 256> expected;
//      std::iota(expected.begin(), expected.end(), 0);
//
//      etl::bit_stream_writer bit_stream(storage.data(), storage.size(), etl::endian::big);
//
//      for (size_t i = 0UL; i < 256UL; ++i)
//      {
//        CHECK(bit_stream.write(uint8_t(i)));
//        CHECK_EQUAL(i + 1U, bit_stream.used_data().size());
//      }
//
//      // One too many.
//      CHECK_THROW(bit_stream.write(int8_t(0)), etl::bit_stream_overflow);
//
//      CHECK_EQUAL(256U, bit_stream.data().size());
//      CHECK_EQUAL(256U, bit_stream.used_data().size());
//
//      for (size_t i = 0UL; i < storage.size(); ++i)
//      {
//        CHECK_EQUAL(int(expected[i]), int(storage[i]));
//      }
//    }
//
//    //*************************************************************************
//    TEST(test_write_int16_t)
//    {
//      std::array<char, sizeof(int16_t) * 4> storage;
//      std::array<char, sizeof(int16_t) * 4> expected = { char(0x00), char(0x01), 
//                                                         char(0x5A), char(0xA5),
//                                                         char(0xA5), char(0x5A),
//                                                         char(0xFF), char(0xFF) };
//
//      CHECK(expected.size() == storage.size());
//
//      etl::bit_stream_writer bit_stream(storage.data(), storage.size(), etl::endian::big);
//
//      CHECK(bit_stream.write(int16_t(0x0001)));
//      CHECK_EQUAL(sizeof(int16_t) * 1, bit_stream.used_data().size());
//      CHECK(bit_stream.write(int16_t(0x5AA5)));
//      CHECK_EQUAL(sizeof(int16_t) * 2, bit_stream.used_data().size());
//      CHECK(bit_stream.write(int16_t(0xA55A)));
//      CHECK_EQUAL(sizeof(int16_t) * 3, bit_stream.used_data().size());
//      CHECK(bit_stream.write(int16_t(0xFFFF)));
//      CHECK_EQUAL(sizeof(int16_t) * 4, bit_stream.used_data().size());
//
//      // One too many.
//      CHECK_THROW(bit_stream.write(int16_t(0)), etl::bit_stream_overflow);
//
//      CHECK_EQUAL(sizeof(int16_t) * 4, bit_stream.data().size());
//      CHECK_EQUAL(sizeof(int16_t) * 4, bit_stream.used_data().size());
//
//      for (size_t i = 0UL; i < storage.size(); ++i)
//      {
//        CHECK_EQUAL(int(expected[i]), int(storage[i]));
//      }
//    }
//
//    //*************************************************************************
//    TEST(test_write_uint16_t)
//    {
//      std::array<char, sizeof(uint16_t) * 4> storage;
//      std::array<char, sizeof(uint16_t) * 4> expected = { char(0x00), char(0x01),
//                                                          char(0x5A), char(0xA5),
//                                                          char(0xA5), char(0x5A),
//                                                          char(0xFF), char(0xFF) };
//
//      CHECK(expected.size() == storage.size());
//
//      etl::bit_stream_writer bit_stream(storage.data(), storage.size(), etl::endian::big);
//
//      CHECK(bit_stream.write(uint16_t(0x0001)));
//      CHECK_EQUAL(sizeof(uint16_t) * 1, bit_stream.used_data().size());
//      CHECK(bit_stream.write(uint16_t(0x5AA5)));
//      CHECK_EQUAL(sizeof(uint16_t) * 2, bit_stream.used_data().size());
//      CHECK(bit_stream.write(uint16_t(0xA55A)));
//      CHECK_EQUAL(sizeof(uint16_t) * 3, bit_stream.used_data().size());
//      CHECK(bit_stream.write(uint16_t(0xFFFF)));
//      CHECK_EQUAL(sizeof(uint16_t) * 4, bit_stream.used_data().size());
//
//      // One too many.
//      CHECK_THROW(bit_stream.write(uint16_t(0)), etl::bit_stream_overflow);
//
//      CHECK_EQUAL(sizeof(uint16_t) * 4, bit_stream.data().size());
//      CHECK_EQUAL(sizeof(uint16_t) * 4, bit_stream.used_data().size());
//
//      for (size_t i = 0UL; i < storage.size(); ++i)
//      {
//        CHECK_EQUAL(int(expected[i]), int(storage[i]));
//      }
//    }
//
//    //*************************************************************************
//    TEST(test_write_int32_t)
//    {
//      std::array<char, sizeof(int32_t) * 4> storage;
//      std::array<char, sizeof(int32_t) * 4> expected = { char(0x00), char(0x00), char(0x00), char(0x01),
//                                                         char(0x5A), char(0xA5), char(0xA5), char(0x5A),
//                                                         char(0xA5), char(0x5A), char(0x5A), char(0xA5),
//                                                         char(0xFF), char(0xFF), char(0xFF), char(0xFF) };
//
//      CHECK(expected.size() == storage.size());
//
//      etl::bit_stream_writer bit_stream(storage.data(), storage.size(), etl::endian::big);
//
//      CHECK(bit_stream.write(int32_t(0x00000001)));
//      CHECK_EQUAL(sizeof(int32_t) * 1, bit_stream.used_data().size());
//      CHECK(bit_stream.write(int32_t(0x5AA5A55A)));
//      CHECK_EQUAL(sizeof(int32_t) * 2, bit_stream.used_data().size());
//      CHECK(bit_stream.write(int32_t(0xA55A5AA5)));
//      CHECK_EQUAL(sizeof(int32_t) * 3, bit_stream.used_data().size());
//      CHECK(bit_stream.write(int32_t(0xFFFFFFFF)));
//      CHECK_EQUAL(sizeof(int32_t) * 4, bit_stream.used_data().size());
//
//      // One too many.
//      CHECK_THROW(bit_stream.write(int32_t(0)), etl::bit_stream_overflow);
//
//      CHECK_EQUAL(sizeof(int32_t) * 4, bit_stream.data().size());
//      CHECK_EQUAL(sizeof(int32_t) * 4, bit_stream.used_data().size());
//
//      for (size_t i = 0UL; i < storage.size(); ++i)
//      {
//        CHECK_EQUAL(int(expected[i]), int(storage[i]));
//      }
//    }
//
//    //*************************************************************************
//    TEST(test_write_uint32_t)
//    {
//      std::array<char, sizeof(uint32_t) * 4> storage;
//      std::array<char, sizeof(uint32_t) * 4> expected = { char(0x00), char(0x00), char(0x00), char(0x01),
//                                                          char(0x5A), char(0xA5), char(0xA5), char(0x5A),
//                                                          char(0xA5), char(0x5A), char(0x5A), char(0xA5),
//                                                          char(0xFF), char(0xFF), char(0xFF), char(0xFF) };
//
//      CHECK(expected.size() == storage.size());
//
//      etl::bit_stream_writer bit_stream(storage.data(), storage.size(), etl::endian::big);
//
//      CHECK(bit_stream.write(uint32_t(0x00000001)));
//      CHECK_EQUAL(sizeof(uint32_t) * 1, bit_stream.used_data().size());
//      CHECK(bit_stream.write(uint32_t(0x5AA5A55A)));
//      CHECK_EQUAL(sizeof(uint32_t) * 2, bit_stream.used_data().size());
//      CHECK(bit_stream.write(uint32_t(0xA55A5AA5)));
//      CHECK_EQUAL(sizeof(uint32_t) * 3, bit_stream.used_data().size());
//      CHECK(bit_stream.write(uint32_t(0xFFFFFFFF)));
//      CHECK_EQUAL(sizeof(uint32_t) * 4, bit_stream.used_data().size());
//
//      // One too many.
//      CHECK_THROW(bit_stream.write(uint32_t(0)), etl::bit_stream_overflow);
//
//      CHECK_EQUAL(sizeof(uint32_t) * 4, bit_stream.data().size());
//      CHECK_EQUAL(sizeof(uint32_t) * 4, bit_stream.used_data().size());
//
//      for (size_t i = 0UL; i < storage.size(); ++i)
//      {
//        CHECK_EQUAL(int(expected[i]), int(storage[i]));
//      }
//    }
//
//    //*************************************************************************
//    TEST(test_write_int64_t)
//    {
//      std::array<char, sizeof(int64_t) * 4> storage;
//      std::array<char, sizeof(int64_t) * 4> expected = { char(0x00), char(0x00), char(0x00), char(0x00), char(0x00), char(0x00), char(0x00), char(0x01),
//                                                         char(0x5A), char(0xA5), char(0xA5), char(0x5A), char(0xA5), char(0x5A), char(0x5A), char(0xA5),
//                                                         char(0xA5), char(0x5A), char(0x5A), char(0xA5), char(0x5A), char(0xA5), char(0xA5), char(0x5A),
//                                                         char(0xFF), char(0xFF), char(0xFF), char(0xFF), char(0xFF), char(0xFF), char(0xFF), char(0xFF) };
//
//      CHECK(expected.size() == storage.size());
//
//      etl::bit_stream_writer bit_stream(storage.data(), storage.size(), etl::endian::big);
//
//      CHECK(bit_stream.write(int64_t(0x0000000000000001LL)));
//      CHECK_EQUAL(sizeof(int64_t) * 1, bit_stream.used_data().size());
//      CHECK(bit_stream.write(int64_t(0x5AA5A55AA55A5AA5LL)));
//      CHECK_EQUAL(sizeof(int64_t) * 2, bit_stream.used_data().size());
//      CHECK(bit_stream.write(int64_t(0xA55A5AA55AA5A55ALL)));
//      CHECK_EQUAL(sizeof(int64_t) * 3, bit_stream.used_data().size());
//      CHECK(bit_stream.write(int64_t(0xFFFFFFFFFFFFFFFFLL)));
//      CHECK_EQUAL(sizeof(int64_t) * 4, bit_stream.used_data().size());
//
//      // One too many.
//      CHECK_THROW(bit_stream.write(int64_t(0)), etl::bit_stream_overflow);
//
//      CHECK_EQUAL(sizeof(int64_t) * 4, bit_stream.data().size());
//      CHECK_EQUAL(sizeof(int64_t) * 4, bit_stream.used_data().size());
//
//      for (size_t i = 0UL; i < storage.size(); ++i)
//      {
//        CHECK_EQUAL(int(expected[i]), int(storage[i]));
//      }
//    }
//
//    //*************************************************************************
//    TEST(test_write_uint64_t)
//    {
//      std::array<char, sizeof(uint64_t) * 4> storage;
//      std::array<char, sizeof(uint64_t) * 4> expected = { char(0x00), char(0x00), char(0x00), char(0x00), char(0x00), char(0x00), char(0x00), char(0x01),
//                                                          char(0x5A), char(0xA5), char(0xA5), char(0x5A), char(0xA5), char(0x5A), char(0x5A), char(0xA5),
//                                                          char(0xA5), char(0x5A), char(0x5A), char(0xA5), char(0x5A), char(0xA5), char(0xA5), char(0x5A),
//                                                          char(0xFF), char(0xFF), char(0xFF), char(0xFF), char(0xFF), char(0xFF), char(0xFF), char(0xFF) };
//
//      CHECK(expected.size() == storage.size());
//
//      etl::bit_stream_writer bit_stream(storage.data(), storage.size(), etl::endian::big);
//
//      CHECK(bit_stream.write(uint64_t(0x0000000000000001LL)));
//      CHECK_EQUAL(sizeof(uint64_t) * 1, bit_stream.used_data().size());
//      CHECK(bit_stream.write(uint64_t(0x5AA5A55AA55A5AA5LL)));
//      CHECK_EQUAL(sizeof(uint64_t) * 2, bit_stream.used_data().size());
//      CHECK(bit_stream.write(uint64_t(0xA55A5AA55AA5A55ALL)));
//      CHECK_EQUAL(sizeof(uint64_t) * 3, bit_stream.used_data().size());
//      CHECK(bit_stream.write(uint64_t(0xFFFFFFFFFFFFFFFFLL)));
//      CHECK_EQUAL(sizeof(uint64_t) * 4, bit_stream.used_data().size());
//
//      // One too many.
//      CHECK_THROW(bit_stream.write(uint64_t(0)), etl::bit_stream_overflow);
//
//      CHECK_EQUAL(sizeof(uint64_t) * 4, bit_stream.data().size());
//      CHECK_EQUAL(sizeof(uint64_t) * 4, bit_stream.used_data().size());
//
//      for (size_t i = 0UL; i < storage.size(); ++i)
//      {
//        CHECK_EQUAL(int(expected[i]), int(storage[i]));
//      }
//    }
//
//    //*************************************************************************
//    TEST(test_write_int8_t_5bits)
//    {
//      std::array<char, 4 * sizeof(char)> storage;
//      std::array<int8_t, 4> write_data = { int8_t(0x01), int8_t(0xF5), int8_t(0x05), int8_t(0xFF) }; // 1, -11, 10, -1         
//      std::array<char, 4> expected = { char(0x0D), char(0x4B), char(0xF0), char(0x00) }; // 1, -11, 10, -1
//
//      etl::bit_stream_writer bit_stream(storage.data(), storage.size(), etl::endian::big);
//
//      // Insert into the stream
//      CHECK(bit_stream.write(write_data[0], 5));
//      CHECK_EQUAL(1, bit_stream.used_data().size());
//      CHECK(bit_stream.write(write_data[1], 5));
//      CHECK_EQUAL(2, bit_stream.used_data().size());
//      CHECK(bit_stream.write(write_data[2], 5));
//      CHECK_EQUAL(2, bit_stream.used_data().size());
//      CHECK(bit_stream.write(write_data[3], 5));
//      CHECK_EQUAL(3, bit_stream.used_data().size());
//
//      CHECK_EQUAL((int)expected[0], (int)storage[0]);
//      CHECK_EQUAL((int)expected[1], (int)storage[1]);
//      CHECK_EQUAL((int)expected[2], (int)storage[2]);
//      // 4th byte not used.
//    }
//
//    //*************************************************************************
//    TEST(test_write_int16_t_10bits)
//    {
//      std::array<char, 4 * sizeof(int16_t)> storage;
//      std::array<int16_t, 4>                write_data = { int16_t(0x0001), int16_t(0xA55A), int16_t(0x5AA5), int16_t(0xFFFF) };
//      std::array<char, 4 * sizeof(int16_t)> expected = { char(0x00), char(0x55), char(0xAA), char(0x97), 
//                                                          char(0xFF), char(0x00), char(0x00), char(0x00) };
//
//      etl::bit_stream_writer bit_stream(storage.data(), storage.size(), etl::endian::big);
//
//      // Insert into the stream
//      CHECK(bit_stream.write(write_data[0], 10));
//      CHECK_EQUAL(2, bit_stream.used_data().size());
//      CHECK(bit_stream.write(write_data[1], 10));
//      CHECK_EQUAL(3, bit_stream.used_data().size());
//      CHECK(bit_stream.write(write_data[2], 10));
//      CHECK_EQUAL(4, bit_stream.used_data().size());
//      CHECK(bit_stream.write(write_data[3], 10));
//      CHECK_EQUAL(5, bit_stream.used_data().size());
//
//      CHECK_EQUAL((int)expected[0], (int)storage[0]);
//      CHECK_EQUAL((int)expected[1], (int)storage[1]);
//      CHECK_EQUAL((int)expected[2], (int)storage[2]);
//      CHECK_EQUAL((int)expected[3], (int)storage[3]);
//      CHECK_EQUAL((int)expected[4], (int)storage[4]);
//      // 6th to 8th bytes not used.
//    }
//
//    //*************************************************************************
//    TEST(test_write_uint32_t_22bits)
//    {
//      std::array<char, 4 * sizeof(uint32_t)> storage;
//      std::array<uint32_t, 4>                write_data = { uint32_t(0x00000001UL), uint32_t(0xA55AA55AUL), uint32_t(0x5AA55AA5UL), uint32_t(0xFFFFFFFFUL) };
//      std::array<char, 4 * sizeof(uint32_t)> expected = { char(0x00), char(0x00), char(0x05), char(0xAA), 
//                                                          char(0x55), char(0xA9), char(0x56), char(0xA9),
//                                                          char(0x7F), char(0xFF), char(0xFF), char(0x00),
//                                                          char(0x00), char(0x00), char(0x00), char(0x00) };
//
//      etl::bit_stream_writer bit_stream(storage.data(), storage.size(), etl::endian::big);
//
//      // Insert into the stream
//      CHECK(bit_stream.write(write_data[0], 22));
//      CHECK_EQUAL(3, bit_stream.used_data().size());
//      CHECK(bit_stream.write(write_data[1], 22));
//      CHECK_EQUAL(6, bit_stream.used_data().size());
//      CHECK(bit_stream.write(write_data[2], 22));
//      CHECK_EQUAL(9, bit_stream.used_data().size());
//      CHECK(bit_stream.write(write_data[3], 22));
//      CHECK_EQUAL(11, bit_stream.used_data().size());
//
//      CHECK_EQUAL((int)expected[0], (int)storage[0]);
//      CHECK_EQUAL((int)expected[1], (int)storage[1]);
//      CHECK_EQUAL((int)expected[2], (int)storage[2]);
//      CHECK_EQUAL((int)expected[3], (int)storage[3]);
//      CHECK_EQUAL((int)expected[4], (int)storage[4]);
//      CHECK_EQUAL((int)expected[5], (int)storage[5]);
//      CHECK_EQUAL((int)expected[6], (int)storage[6]);
//      CHECK_EQUAL((int)expected[7], (int)storage[7]);
//      CHECK_EQUAL((int)expected[8], (int)storage[8]);
//      CHECK_EQUAL((int)expected[9], (int)storage[9]);
//      CHECK_EQUAL((int)expected[10], (int)storage[10]);
//      // 12th to 16th to bytes not used.
//    }
//
//    //*************************************************************************
//    TEST(test_write_int64_t_47bits)
//    {
//      std::array<char, 4 * sizeof(int64_t)> storage;
//      std::array<int64_t, 4>                write_data    = { int64_t(0x0000000000000001LL), int64_t(0xA55AA55AA55AA55ALL), int64_t(0x5AA55AA55AA55AA5LL), int64_t(0xFFFFFFFFFFFFFFFFLL) };
//      std::array<char, 4 * sizeof(int64_t)> expected = { char(0x00), char(0x00), char(0x00), char(0x00),
//                                                         char(0x00), char(0x02), char(0x95), char(0x6A),
//                                                         char(0x95), char(0x6A), char(0x95), char(0x6A),
//                                                         char(0xD5), char(0x2A), char(0xD5), char(0x2A),
//                                                         char(0xD5), char(0x2F), char(0xFF), char(0xFF),
//                                                         char(0xFF), char(0xFF), char(0xFF), char(0xF0) };
//
//      etl::bit_stream_writer bit_stream(storage.data(), storage.size(), etl::endian::big);
//
//      // Insert into the stream
//      bit_stream.write(write_data[0], 47);
//      CHECK_EQUAL(6, bit_stream.used_data().size());
//      bit_stream.write(write_data[1], 47);
//      CHECK_EQUAL(12, bit_stream.used_data().size());
//      bit_stream.write(write_data[2], 47);
//      CHECK_EQUAL(18, bit_stream.used_data().size());
//      bit_stream.write(write_data[3], 47);
//      CHECK_EQUAL(24, bit_stream.used_data().size());
//
//      CHECK_EQUAL((int)expected[0], (int)storage[0]);
//      CHECK_EQUAL((int)expected[1], (int)storage[1]);
//      CHECK_EQUAL((int)expected[2], (int)storage[2]);
//      CHECK_EQUAL((int)expected[3], (int)storage[3]);
//      CHECK_EQUAL((int)expected[4], (int)storage[4]);
//      CHECK_EQUAL((int)expected[5], (int)storage[5]);
//      CHECK_EQUAL((int)expected[6], (int)storage[6]);
//      CHECK_EQUAL((int)expected[7], (int)storage[7]);
//      CHECK_EQUAL((int)expected[8], (int)storage[8]);
//      CHECK_EQUAL((int)expected[9], (int)storage[9]);
//      CHECK_EQUAL((int)expected[10], (int)storage[10]);
//      CHECK_EQUAL((int)expected[11], (int)storage[11]);
//      CHECK_EQUAL((int)expected[12], (int)storage[12]);
//      CHECK_EQUAL((int)expected[13], (int)storage[13]);
//      CHECK_EQUAL((int)expected[14], (int)storage[14]);
//      CHECK_EQUAL((int)expected[15], (int)storage[15]);
//      CHECK_EQUAL((int)expected[16], (int)storage[16]);
//      CHECK_EQUAL((int)expected[17], (int)storage[17]);
//      CHECK_EQUAL((int)expected[18], (int)storage[18]);
//      CHECK_EQUAL((int)expected[19], (int)storage[19]);
//      CHECK_EQUAL((int)expected[20], (int)storage[20]);
//      CHECK_EQUAL((int)expected[21], (int)storage[21]);
//      CHECK_EQUAL((int)expected[22], (int)storage[22]);
//      CHECK_EQUAL((int)expected[23], (int)storage[23]);
//      // 25th to 32nd bytes not used.
//    }
//
//    //*************************************************************************
//    TEST(test_write_uint64_t_47bits)
//    {
//      std::array<char, 4 * sizeof(uint64_t)> storage;
//      std::array<uint64_t, 4>                write_data = { uint64_t(0x0000000000000001ULL), uint64_t(0xA55AA55AA55AA55AULL), uint64_t(0x5AA55AA55AA55AA5ULL), uint64_t(0xFFFFFFFFFFFFFFFFULL) };
//      std::array<char, 4 * sizeof(uint64_t)> expected = { char(0x00), char(0x00), char(0x00), char(0x00),
//                                                          char(0x00), char(0x02), char(0x95), char(0x6A),
//                                                          char(0x95), char(0x6A), char(0x95), char(0x6A),
//                                                          char(0xD5), char(0x2A), char(0xD5), char(0x2A),
//                                                          char(0xD5), char(0x2F), char(0xFF), char(0xFF),
//                                                          char(0xFF), char(0xFF), char(0xFF), char(0xF0) };
//
//      etl::bit_stream_writer bit_stream(storage.data(), storage.size(), etl::endian::big);
//
//      // Insert into the stream
//      bit_stream.write(write_data[0], 47);
//      CHECK_EQUAL(6, bit_stream.used_data().size());
//      bit_stream.write(write_data[1], 47);
//      CHECK_EQUAL(12, bit_stream.used_data().size());
//      bit_stream.write(write_data[2], 47);
//      CHECK_EQUAL(18, bit_stream.used_data().size());
//      bit_stream.write(write_data[3], 47);
//      CHECK_EQUAL(24, bit_stream.used_data().size());
//
//      CHECK_EQUAL((int)expected[0], (int)storage[0]);
//      CHECK_EQUAL((int)expected[1], (int)storage[1]);
//      CHECK_EQUAL((int)expected[2], (int)storage[2]);
//      CHECK_EQUAL((int)expected[3], (int)storage[3]);
//      CHECK_EQUAL((int)expected[4], (int)storage[4]);
//      CHECK_EQUAL((int)expected[5], (int)storage[5]);
//      CHECK_EQUAL((int)expected[6], (int)storage[6]);
//      CHECK_EQUAL((int)expected[7], (int)storage[7]);
//      CHECK_EQUAL((int)expected[8], (int)storage[8]);
//      CHECK_EQUAL((int)expected[9], (int)storage[9]);
//      CHECK_EQUAL((int)expected[10], (int)storage[10]);
//      CHECK_EQUAL((int)expected[11], (int)storage[11]);
//      CHECK_EQUAL((int)expected[12], (int)storage[12]);
//      CHECK_EQUAL((int)expected[13], (int)storage[13]);
//      CHECK_EQUAL((int)expected[14], (int)storage[14]);
//      CHECK_EQUAL((int)expected[15], (int)storage[15]);
//      CHECK_EQUAL((int)expected[16], (int)storage[16]);
//      CHECK_EQUAL((int)expected[17], (int)storage[17]);
//      CHECK_EQUAL((int)expected[18], (int)storage[18]);
//      CHECK_EQUAL((int)expected[19], (int)storage[19]);
//      CHECK_EQUAL((int)expected[20], (int)storage[20]);
//      CHECK_EQUAL((int)expected[21], (int)storage[21]);
//      CHECK_EQUAL((int)expected[22], (int)storage[22]);
//      CHECK_EQUAL((int)expected[23], (int)storage[23]);
//      // 25th to 32nd bytes not used.
//    }
//
//    //*************************************************************************
//    TEST(test_write_multiple_full_size)
//    {
//      char c1 = 90;              // 0x5A
//      char c2 = -91;             // 0xA5
//      unsigned short s1 = 4660;  // 0x1234
//      unsigned short s2 = 22136; // 0x5678
//      int32_t i1 = 0x89ABCDEF;   // 0x89ABCDEF
//      int32_t i2 = 0xFEDCBA98;   // 0xFEDCBA98
//
//      std::array<char, 14> storage;
//      std::array<char, 14> expected = { char(0x5A),
//                                        char(0x12), char(0x34),
//                                        char(0x89), char(0xAB), char(0xCD), char(0xEF),
//                                        char(0xFE), char(0xDC), char(0xBA), char(0x98),
//                                        char(0x56), char(0x78),
//                                        char(0xA5) };
//
//      etl::bit_stream_writer bit_stream(storage.data(), storage.size(), etl::endian::big);
//
//      // Insert into the stream.
//      bit_stream.write(c1);
//      CHECK_EQUAL(1, bit_stream.used_data().size());
//      bit_stream.write(s1);
//      CHECK_EQUAL(3, bit_stream.used_data().size());
//      bit_stream.write(i1);
//      CHECK_EQUAL(7, bit_stream.used_data().size());
//      bit_stream.write(i2);
//      CHECK_EQUAL(11, bit_stream.used_data().size());
//      bit_stream.write(s2);
//      CHECK_EQUAL(13, bit_stream.used_data().size());
//      bit_stream.write(c2);
//      CHECK_EQUAL(14, bit_stream.used_data().size());
//
//      CHECK_EQUAL((int)expected[0], (int)storage[0]);
//      CHECK_EQUAL((int)expected[1], (int)storage[1]);
//      CHECK_EQUAL((int)expected[2], (int)storage[2]);
//      CHECK_EQUAL((int)expected[3], (int)storage[3]);
//      CHECK_EQUAL((int)expected[4], (int)storage[4]);
//      CHECK_EQUAL((int)expected[5], (int)storage[5]);
//      CHECK_EQUAL((int)expected[6], (int)storage[6]);
//      CHECK_EQUAL((int)expected[7], (int)storage[7]);
//      CHECK_EQUAL((int)expected[8], (int)storage[8]);
//      CHECK_EQUAL((int)expected[9], (int)storage[9]);
//      CHECK_EQUAL((int)expected[10], (int)storage[10]);
//      CHECK_EQUAL((int)expected[11], (int)storage[11]);
//      CHECK_EQUAL((int)expected[12], (int)storage[12]);
//      CHECK_EQUAL((int)expected[13], (int)storage[13]);
//    }
//
//    //*************************************************************************
//    TEST(test_write_multiple_variable_size)
//    {
//      char c1 = 90;              // 0x5A       6 bits
//      char c2 = -91;             // 0xA5       7 bits
//      unsigned short s1 = 4660;  // 0x1234     13 bits
//      unsigned short s2 = 22136; // 0x5678     11 bits
//      int32_t i1 = 0x89ABCDEF;   // 0x89ABCDEF 23 bits
//      int32_t i2 = 0xFEDCBA98;   // 0xFEDCBA98 25 bits
//
//      std::array<char, 14> storage;
//      std::array<char, 14> expected = { char(0x6A), char(0x46), char(0x8A), char(0xF3), 
//                                        char(0x7B), char(0xDB), char(0x97), char(0x53),
//                                        char(0x19), char(0xE1), char(0x28) };
//
//      etl::bit_stream_writer bit_stream(storage.data(), storage.size(), etl::endian::big);
//
//      // Insert into the stream.
//      bit_stream.write(c1, 6);
//      CHECK_EQUAL(1, bit_stream.used_data().size());
//      bit_stream.write(s1, 13);
//      CHECK_EQUAL(3, bit_stream.used_data().size());
//      bit_stream.write(i1, 23);
//      CHECK_EQUAL(6, bit_stream.used_data().size());
//      bit_stream.write(i2, 25);
//      CHECK_EQUAL(9, bit_stream.used_data().size());
//      bit_stream.write(s2, 11);
//      CHECK_EQUAL(10, bit_stream.used_data().size());
//      bit_stream.write(c2, 7);
//      CHECK_EQUAL(11, bit_stream.used_data().size());
//
//      CHECK_EQUAL((int)expected[0], (int)storage[0]);
//      CHECK_EQUAL((int)expected[1], (int)storage[1]);
//      CHECK_EQUAL((int)expected[2], (int)storage[2]);
//      CHECK_EQUAL((int)expected[3], (int)storage[3]);
//      CHECK_EQUAL((int)expected[4], (int)storage[4]);
//      CHECK_EQUAL((int)expected[5], (int)storage[5]);
//      CHECK_EQUAL((int)expected[6], (int)storage[6]);
//      CHECK_EQUAL((int)expected[7], (int)storage[7]);
//      CHECK_EQUAL((int)expected[8], (int)storage[8]);
//      CHECK_EQUAL((int)expected[9], (int)storage[9]);
//      CHECK_EQUAL((int)expected[10], (int)storage[10]);
//    }
//
//    //*************************************************************************
//    TEST(test_write_multiple_variable_size_with_callback)
//    {
//      Accumulator accumulator;
//
//      char c1 = 90;               // 0x5A       6 bits
//      char c2 = -91;              // 0xA5       7 bits
//      unsigned short s1 = 4660U;  // 0x1234     13 bits
//      unsigned short s2 = 22136U; // 0x5678     11 bits
//      int32_t i1 = 0x89ABCDEF;    // 0x89ABCDEF 23 bits
//      int32_t i2 = 0xFEDCBA98;    // 0xFEDCBA98 25 bits
//
//      std::array<char, 14> storage;
//      storage.fill(0);
//      std::array<char, 14> expected = { char(0x6A), char(0x46), char(0x8A), char(0xF3),
//                                        char(0x7B), char(0xDB), char(0x97), char(0x53),
//                                        char(0x19), char(0xE1), char(0x28) };
//
//      auto callback = etl::bit_stream_writer::callback_type::create<Accumulator, &Accumulator::Add>(accumulator);
//
//      etl::bit_stream_writer bit_stream(storage.data(), storage.size(), etl::endian::big, callback);
//
//      // Insert into the stream.
//      bit_stream.write(c1, 6);
//      bit_stream.write(s1, 13);
//      bit_stream.write(i1, 23);
//      bit_stream.write(i2, 25);
//      bit_stream.write(s2, 11);
//      bit_stream.write(c2, 7);
//      bit_stream.flush();
//
//      CHECK_EQUAL(bit_stream.capacity_bytes(), bit_stream.available<char>());
//
//      CHECK_EQUAL(11U, accumulator.GetData().size());
//
//      CHECK_EQUAL((int)expected[0], (int)accumulator.GetData()[0]);
//      CHECK_EQUAL((int)expected[1], (int)accumulator.GetData()[1]);
//      CHECK_EQUAL((int)expected[2], (int)accumulator.GetData()[2]);
//      CHECK_EQUAL((int)expected[3], (int)accumulator.GetData()[3]);
//      CHECK_EQUAL((int)expected[4], (int)accumulator.GetData()[4]);
//      CHECK_EQUAL((int)expected[5], (int)accumulator.GetData()[5]);
//      CHECK_EQUAL((int)expected[6], (int)accumulator.GetData()[6]);
//      CHECK_EQUAL((int)expected[7], (int)accumulator.GetData()[7]);
//      CHECK_EQUAL((int)expected[8], (int)accumulator.GetData()[8]);
//      CHECK_EQUAL((int)expected[9], (int)accumulator.GetData()[9]);
//      CHECK_EQUAL((int)expected[10], (int)accumulator.GetData()[10]);
//    }
//
//    //*************************************************************************
//    TEST(test_write_object)
//    {
//      std::array<char, 2 * sizeof(Object)> storage;
//      storage.fill(0);
//      std::array expected{ char(0xEC), char(0xBA), char(0xDE), char(0x68),
//                           char(0xAF), char(0xD2), char(0xC5), char(0xC8),
//                           char(0x65), char(0xD3), char(0xDF), char(0x80) };
//
//      etl::bit_stream_writer bit_stream(storage.data(), storage.size(), etl::endian::big);
//
//      Object object1 = { -1234,  123456789, 250 };
//      Object object2 = {  5678, -987654321, 126 };
//
//      CHECK(etl::write(bit_stream, object1));
//      CHECK(etl::write(bit_stream, object2));
//
//      CHECK_EQUAL((int)expected[0], (int)storage[0]);
//      CHECK_EQUAL((int)expected[1], (int)storage[1]);
//      CHECK_EQUAL((int)expected[2], (int)storage[2]);
//      CHECK_EQUAL((int)expected[3], (int)storage[3]);
//      CHECK_EQUAL((int)expected[4], (int)storage[4]);
//      CHECK_EQUAL((int)expected[5], (int)storage[5]);
//      CHECK_EQUAL((int)expected[6], (int)storage[6]);
//      CHECK_EQUAL((int)expected[7], (int)storage[7]);
//      CHECK_EQUAL((int)expected[8], (int)storage[8]);
//      CHECK_EQUAL((int)expected[9], (int)storage[9]);
//      CHECK_EQUAL((int)expected[10], (int)storage[10]);
//      CHECK_EQUAL((int)expected[11], (int)storage[11]);
//    }
//  };
//}
//
