/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl
https://www.etlcpp.com

Copyright(c) 2018 John Wellbelove

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
#include "etl/math.h"

#include <array>
#include <numeric>

#include "etl/private/diagnostic_useless_cast_push.h"

namespace
{
  //***********************************
  struct Object
  {
    int16_t i;
    double  d;
    uint8_t c;
  };

  bool operator ==(const Object& lhs, const Object& rhs)
  {
#include "etl/private/diagnostic_float_equal_push.h"
    return (lhs.i == rhs.i) &&
           (lhs.d == rhs.d) &&
           (lhs.c == rhs.c);
#include "etl/private/diagnostic_pop.h"
  }

  std::ostream& operator << (std::ostream& os, const Object& object)
  {
    os << object.i << "," << object.d << "," << (int)object.c;
    return os;
  }
}

namespace etl
{
  //***********************************
  bool bit_stream_put(etl::bit_stream& stream, const Object& object)
  {
    bool success = true;

    if (!stream.put(object.i, 14))
    {
      success = false;
    }

    if (!stream.put(object.d))
    {
      success = false;
    }

    if (!stream.put(object.c))
    {
      success = false;
    }

    return success;
  }

  //***********************************
  bool bit_stream_get(etl::bit_stream& stream, Object& object)
  {
    bool success = true;

    if (!stream.get(object.i, 14))
    {
      success = false;
    }

    if (!stream.get(object.d))
    {
      success = false;
    }

    if (!stream.get(object.c))
    {
      success = false;
    }

    return success;
  }
}

namespace
{
  SUITE(test_bit_stream)
  {
    //*************************************************************************
    TEST(put_bool)
    {
      unsigned char storage = 0;
      unsigned char compare_data = 0x5AU;

      etl::bit_stream bit_stream(&storage, 1);

      CHECK(bit_stream.put(false));
      CHECK(bit_stream.put(true));
      CHECK(bit_stream.put(false));
      CHECK(bit_stream.put(true));
      CHECK(bit_stream.put(true));
      CHECK(bit_stream.put(false));
      CHECK(bit_stream.put(true));
      CHECK(bit_stream.put(false));

      // One too many.
      CHECK(!bit_stream.put(true));

      CHECK_EQUAL(int(compare_data), int(storage));
    }

    //*************************************************************************
    TEST(put_int8_t)
    {
      std::array<unsigned char, 256> storage{0};

      std::array<unsigned char, 256> compare_data;
      std::iota(compare_data.begin(), compare_data.end(), 0);

      etl::bit_stream bit_stream(storage.data(), storage.size());

      for (size_t i = 0UL; i < 256UL; ++i)
      {
        CHECK(bit_stream.put(int8_t(i)));
      }

      // One too many.
      CHECK(!bit_stream.put(int8_t(0)));

      for (size_t i = 0UL; i < storage.size(); ++i)
      {
        CHECK_EQUAL(int(compare_data[i]), int(storage[i]));
      }
    }

    //*************************************************************************
    TEST(put_uint8_t)
    {
      std::array<unsigned char, 256> storage{0};

      std::array<unsigned char, 256> compare_data;
      std::iota(compare_data.begin(), compare_data.end(), 0);

      etl::bit_stream bit_stream(storage.data(), storage.size());

      for (size_t i = 0UL; i < 256UL; ++i)
      {
        CHECK(bit_stream.put(uint8_t(i)));
      }

      // One too many.
      CHECK(!bit_stream.put(uint8_t(0U)));

      for (size_t i = 0UL; i < storage.size(); ++i)
      {
        CHECK_EQUAL(int(compare_data[i]), int(storage[i]));
      }
    }

    //*************************************************************************
    TEST(put_int16_t)
    {
      // Tests assume big endian.
      std::array<unsigned char, sizeof(int16_t) * 4> storage{0};
      std::array<unsigned char, sizeof(int16_t) * 4> compare_data = { 0x00U, 0x01U, 0x5AU, 0xA5U, 0xA5U, 0x5AU, 0xFFU, 0xFFU };

      CHECK(compare_data.size() == storage.size());

      etl::bit_stream bit_stream(storage.data(), storage.size());

      CHECK(bit_stream.put(int16_t(0x0001)));
      CHECK(bit_stream.put(int16_t(0x5AA5)));
      CHECK(bit_stream.put(int16_t(0xA55A)));
      CHECK(bit_stream.put(int16_t(0xFFFF)));

      // One too many.
      CHECK(!bit_stream.put(int16_t(0)));

      for (size_t i = 0UL; i < storage.size(); ++i)
      {
        CHECK_EQUAL(int(compare_data[i]), int(storage[i]));
      }
    }

    //*************************************************************************
    TEST(put_uint16_t)
    {
      // Tests assume big endian.
      std::array<unsigned char, sizeof(uint16_t) * 4> storage{0};
      std::array<unsigned char, sizeof(uint16_t) * 4> compare_data = { 0x00U, 0x01U, 0x5AU, 0xA5U, 0xA5U, 0x5AU, 0xFFU, 0xFFU };

      CHECK(compare_data.size() == storage.size());

      etl::bit_stream bit_stream(storage.data(), storage.size());

      CHECK(bit_stream.put(uint16_t(0x0001U)));
      CHECK(bit_stream.put(uint16_t(0x5AA5U)));
      CHECK(bit_stream.put(uint16_t(0xA55AU)));
      CHECK(bit_stream.put(uint16_t(0xFFFFU)));

      // One too many.
      CHECK(!bit_stream.put(uint16_t(0U)));

      for (size_t i = 0UL; i < storage.size(); ++i)
      {
        CHECK_EQUAL(int(compare_data[i]), int(storage[i]));
      }
    }

    //*************************************************************************
    TEST(put_int32_t)
    {
      // Tests assume big endian.
      std::array<unsigned char, sizeof(int32_t) * 4> storage{0};
      std::array<unsigned char, sizeof(int32_t) * 4> compare_data = { 0x00U, 0x00U, 0x00U, 0x01U,
                                                                      0x5AU, 0xA5U, 0xA5U, 0x5AU,
                                                                      0xA5U, 0x5AU, 0x5AU, 0xA5U,
                                                                      0xFFU, 0xFFU, 0xFFU, 0xFFU };

      CHECK(compare_data.size() == storage.size());

      etl::bit_stream bit_stream(storage.data(), storage.size());

      CHECK(bit_stream.put(int32_t(0x00000001)));
      CHECK(bit_stream.put(int32_t(0x5AA5A55A)));
      CHECK(bit_stream.put(int32_t(0xA55A5AA5)));
      CHECK(bit_stream.put(int32_t(0xFFFFFFFF)));

      // One too many.
      CHECK(!bit_stream.put(int32_t(0)));

      for (size_t i = 0UL; i < storage.size(); ++i)
      {
        CHECK_EQUAL(int(compare_data[i]), int(storage[i]));
      }
    }

    //*************************************************************************
    TEST(put_uint32_t)
    {
      // Tests assume big endian.
      std::array<unsigned char, sizeof(uint32_t) * 4> storage{0};
      std::array<unsigned char, sizeof(uint32_t) * 4> compare_data = { 0x00U, 0x00U, 0x00U, 0x01U,
                                                                       0x5AU, 0xA5U, 0xA5U, 0x5AU,
                                                                       0xA5U, 0x5AU, 0x5AU, 0xA5U,
                                                                       0xFFU, 0xFFU, 0xFFU, 0xFFU };

      CHECK(compare_data.size() == storage.size());

      etl::bit_stream bit_stream(storage.data(), storage.size());

      CHECK(bit_stream.put(uint32_t(0x00000001UL)));
      CHECK(bit_stream.put(uint32_t(0x5AA5A55AUL)));
      CHECK(bit_stream.put(uint32_t(0xA55A5AA5UL)));
      CHECK(bit_stream.put(uint32_t(0xFFFFFFFFUL)));

      // One too many.
      CHECK(!bit_stream.put(uint32_t(0U)));

      for (size_t i = 0UL; i < storage.size(); ++i)
      {
        CHECK_EQUAL(int(compare_data[i]), int(storage[i]));
      }
    }

    //*************************************************************************
    TEST(put_int64_t)
    {
      // Tests assume big endian.
      std::array<unsigned char, sizeof(int64_t) * 4> storage{0};
      std::array<unsigned char, sizeof(int64_t) * 4> compare_data = { 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x01U,
                                                                      0x5AU, 0xA5U, 0xA5U, 0x5AU, 0xA5U, 0x5AU, 0x5AU, 0xA5U,
                                                                      0xA5U, 0x5AU, 0x5AU, 0xA5U, 0x5AU, 0xA5U, 0xA5U, 0x5AU,
                                                                      0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU };

      CHECK(compare_data.size() == storage.size());

      etl::bit_stream bit_stream(storage.data(), storage.size());

      CHECK(bit_stream.put(int64_t(0x0000000000000001LL)));
      CHECK(bit_stream.put(int64_t(0x5AA5A55AA55A5AA5LL)));
      CHECK(bit_stream.put(int64_t(0xA55A5AA55AA5A55ALL)));
      CHECK(bit_stream.put(int64_t(0xFFFFFFFFFFFFFFFFLL)));

      // One too many.
      CHECK(!bit_stream.put(int64_t(0)));

      for (size_t i = 0UL; i < storage.size(); ++i)
      {
        CHECK_EQUAL(int(compare_data[i]), int(storage[i]));
      }
    }

    //*************************************************************************
    TEST(put_uint64_t)
    {
      // Tests assume big endian.
      std::array<unsigned char, sizeof(uint64_t) * 4> storage{0};
      std::array<unsigned char, sizeof(uint64_t) * 4> compare_data = { 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x01U,
                                                                       0x5AU, 0xA5U, 0xA5U, 0x5AU, 0xA5U, 0x5AU, 0x5AU, 0xA5U,
                                                                       0xA5U, 0x5AU, 0x5AU, 0xA5U, 0x5AU, 0xA5U, 0xA5U, 0x5AU,
                                                                       0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU };

      CHECK(compare_data.size() == storage.size());

      etl::bit_stream bit_stream(storage.data(), storage.size());

      CHECK(bit_stream.put(uint64_t(0x0000000000000001ULL)));
      CHECK(bit_stream.put(uint64_t(0x5AA5A55AA55A5AA5ULL)));
      CHECK(bit_stream.put(uint64_t(0xA55A5AA55AA5A55AULL)));
      CHECK(bit_stream.put(uint64_t(0xFFFFFFFFFFFFFFFFULL)));

      // One too many.
      CHECK(!bit_stream.put(uint64_t(0U)));

      for (size_t i = 0UL; i < storage.size(); ++i)
      {
        CHECK_EQUAL(int(compare_data[i]), int(storage[i]));
      }
    }

    //*************************************************************************
    TEST(put_get_bool)
    {
      std::array<bool, 8> flags =
      {
        false, true, false, true, true, false, true, false
      };

      unsigned char storage = 0U;
      etl::bit_stream bit_stream(&storage, 1);

      for (size_t i = 0UL; i < flags.size(); ++i)
      {
        bit_stream.put(flags[i]);
      }

      bit_stream.restart();

      for (size_t i = 0UL; i < flags.size(); ++i)
      {
        bool flag;
        CHECK(bit_stream.get(flag));
        CHECK_EQUAL(flags[i], flag);
      }
    }

    //*************************************************************************
    TEST(put_get_int8_t)
    {
      std::array<unsigned char, 4 * sizeof(int8_t)> storage{0};
      std::array<int8_t, 4> put_data = { int8_t(0x01), int8_t(0x5A), int8_t(0xA5), int8_t(0xFF) };
      std::array<int8_t, 4> get_data = { int8_t(0x00), int8_t(0x00), int8_t(0x00), int8_t(0x00) };

      etl::bit_stream bit_stream(storage.data(), storage.size());

      // Insert into the stream
      bit_stream.put(put_data[0]);
      bit_stream.put(put_data[1]);
      bit_stream.put(put_data[2]);
      bit_stream.put(put_data[3]);

      bit_stream.restart();

      CHECK(bit_stream.get(get_data[0]));
      CHECK_EQUAL(put_data[0], (int)get_data[0]);

      CHECK(bit_stream.get(get_data[1]));
      CHECK_EQUAL(put_data[1], (int)get_data[1]);

      CHECK(bit_stream.get(get_data[2]));
      CHECK_EQUAL(put_data[2], (int)get_data[2]);

      CHECK(bit_stream.get(get_data[3]));
      CHECK_EQUAL(put_data[3], (int)get_data[3]);
    }

    //*************************************************************************
    TEST(put_get_int8_t_5bits)
    {
      std::array<unsigned char, 4 * sizeof(int8_t)> storage{0};
      std::array<int8_t, 4> put_data    = { int8_t(0x01), int8_t(0x15), int8_t(0xA5), int8_t(0x1F) }; // 1, -11, 10, -1
      std::array<int8_t, 4> expect_data = { int8_t(0x01), int8_t(0xF5), int8_t(0x05), int8_t(0xFF) }; // 1, -11, 10, -1
      std::array<int8_t, 4> get_data    = { int8_t(0x00), int8_t(0x00), int8_t(0x00), int8_t(0x00) };

      etl::bit_stream bit_stream(storage.data(), storage.size());

      // Insert into the stream
      bit_stream.put(put_data[0], 5);
      bit_stream.put(put_data[1], 5);
      bit_stream.put(put_data[2], 5);
      bit_stream.put(put_data[3], 5);

      bit_stream.restart();

      CHECK(bit_stream.get(get_data[0], 5));
      CHECK_EQUAL((int)expect_data[0], (int)get_data[0]);

      CHECK(bit_stream.get(get_data[1], 5));
      CHECK_EQUAL((int)expect_data[1], (int)get_data[1]);

      CHECK(bit_stream.get(get_data[2], 5));
      CHECK_EQUAL((int)expect_data[2], (int)get_data[2]);

      CHECK(bit_stream.get(get_data[3], 5));
      CHECK_EQUAL((int)expect_data[3], (int)get_data[3]);
    }

    //*************************************************************************
    TEST(put_get_uint8_t)
    {
      std::array<unsigned char, 4 * sizeof(uint8_t)> storage{0};
      std::array<uint8_t, 4> put_data = { uint8_t(0x01U), uint8_t(0x5AU), uint8_t(0xA5U), uint8_t(0xFFU) };
      std::array<uint8_t, 4> get_data = { uint8_t(0x00U), uint8_t(0x00U), uint8_t(0x00U), uint8_t(0x00U) };

      etl::bit_stream bit_stream(storage.data(), storage.size());

      // Insert into the stream
      bit_stream.put(put_data[0]);
      bit_stream.put(put_data[1]);
      bit_stream.put(put_data[2]);
      bit_stream.put(put_data[3]);

      bit_stream.restart();

      CHECK(bit_stream.get(get_data[0]));
      CHECK_EQUAL(put_data[0], (int)get_data[0]);

      CHECK(bit_stream.get(get_data[1]));
      CHECK_EQUAL(put_data[1], (int)get_data[1]);

      CHECK(bit_stream.get(get_data[2]));
      CHECK_EQUAL(put_data[2], (int)get_data[2]);

      CHECK(bit_stream.get(get_data[3]));
      CHECK_EQUAL(put_data[3], (int)get_data[3]);
    }

    //*************************************************************************
    TEST(put_get_uint8_t_5bits)
    {
      std::array<unsigned char, 4 * sizeof(uint8_t)> storage{0};
      std::array<uint8_t, 4> put_data    = { uint8_t(0x01U), uint8_t(0x5AU), uint8_t(0xA5U), uint8_t(0xFFU) }; // 1, -11, 10, -1
      std::array<uint8_t, 4> expect_data = { uint8_t(0x01U), uint8_t(0x1AU), uint8_t(0x05U), uint8_t(0x1FU) }; // 1, -11, 10, -1
      std::array<uint8_t, 4> get_data    = { uint8_t(0x00U), uint8_t(0x00U), uint8_t(0x00U), uint8_t(0x00U) };

      etl::bit_stream bit_stream(storage.data(), storage.size());

      // Insert into the stream
      bit_stream.put(put_data[0], 5);
      bit_stream.put(put_data[1], 5);
      bit_stream.put(put_data[2], 5);
      bit_stream.put(put_data[3], 5);

      bit_stream.restart();

      CHECK(bit_stream.get(get_data[0], 5));
      CHECK_EQUAL((int)expect_data[0], (int)get_data[0]);

      CHECK(bit_stream.get(get_data[1], 5));
      CHECK_EQUAL((int)expect_data[1], (int)get_data[1]);

      CHECK(bit_stream.get(get_data[2], 5));
      CHECK_EQUAL((int)expect_data[2], (int)get_data[2]);

      CHECK(bit_stream.get(get_data[3], 5));
      CHECK_EQUAL((int)expect_data[3], (int)get_data[3]);
    }

    //*************************************************************************
    TEST(put_get_int16_t)
    {
      std::array<unsigned char, 4 * sizeof(int16_t)> storage{0};
      std::array<int16_t, 4> put_data = { int16_t(0x0001), int16_t(0xA55A), int16_t(0x5AA5), int16_t(0xFFFF) };
      std::array<int16_t, 4> get_data = { int16_t(0x0000), int16_t(0x0000), int16_t(0x0000), int16_t(0x0000) };

      etl::bit_stream bit_stream(storage.data(), storage.size());

      // Insert into the stream
      bit_stream.put(put_data[0]);
      bit_stream.put(put_data[1]);
      bit_stream.put(put_data[2]);
      bit_stream.put(put_data[3]);

      bit_stream.restart();

      CHECK(bit_stream.get(get_data[0]));
      CHECK_EQUAL(put_data[0], get_data[0]);

      CHECK(bit_stream.get(get_data[1]));
      CHECK_EQUAL(put_data[1], get_data[1]);

      CHECK(bit_stream.get(get_data[2]));
      CHECK_EQUAL(put_data[2], get_data[2]);

      CHECK(bit_stream.get(get_data[3]));
      CHECK_EQUAL(put_data[3], get_data[3]);
    }

    //*************************************************************************
    TEST(put_get_int16_t_10bits)
    {
      std::array<unsigned char, 4 * sizeof(int16_t)> storage{0};
      std::array<int16_t, 4> put_data    = { int16_t(0x0001), int16_t(0xA55A), int16_t(0x5AA5), int16_t(0xFFFF) };
      std::array<int16_t, 4> expect_data = { int16_t(0x0001), int16_t(0x015A), int16_t(0xFEA5), int16_t(0xFFFF) };
      std::array<int16_t, 4> get_data    = { int16_t(0x0000), int16_t(0x0000), int16_t(0x0000), int16_t(0x0000) };

      etl::bit_stream bit_stream(storage.data(), storage.size());

      // Insert into the stream
      bit_stream.put(put_data[0], 10);
      bit_stream.put(put_data[1], 10);
      bit_stream.put(put_data[2], 10);
      bit_stream.put(put_data[3], 10);

      bit_stream.restart();

      CHECK(bit_stream.get(get_data[0], 10));
      CHECK_EQUAL(expect_data[0], get_data[0]);

      CHECK(bit_stream.get(get_data[1], 10));
      CHECK_EQUAL(expect_data[1], get_data[1]);

      CHECK(bit_stream.get(get_data[2], 10));
      CHECK_EQUAL(expect_data[2], get_data[2]);

      CHECK(bit_stream.get(get_data[3], 10));
      CHECK_EQUAL(expect_data[3], get_data[3]);
    }

    //*************************************************************************
    TEST(put_get_uint16_t)
    {
      std::array<unsigned char, 4 * sizeof(uint16_t)> storage{0};
      std::array<uint16_t, 4> put_data = { uint16_t(0x0001U), uint16_t(0xA55AU), uint16_t(0x5AA5U), uint16_t(0xFFFFU) };
      std::array<uint16_t, 4> get_data = { uint16_t(0x0000U), uint16_t(0x0000U), uint16_t(0x0000U), uint16_t(0x0000U) };

      etl::bit_stream bit_stream(storage.data(), storage.size());

      // Insert into the stream
      bit_stream.put(put_data[0]);
      bit_stream.put(put_data[1]);
      bit_stream.put(put_data[2]);
      bit_stream.put(put_data[3]);

      bit_stream.restart();

      CHECK(bit_stream.get(get_data[0]));
      CHECK_EQUAL(put_data[0], get_data[0]);

      CHECK(bit_stream.get(get_data[1]));
      CHECK_EQUAL(put_data[1], get_data[1]);

      CHECK(bit_stream.get(get_data[2]));
      CHECK_EQUAL(put_data[2], get_data[2]);

      CHECK(bit_stream.get(get_data[3]));
      CHECK_EQUAL(put_data[3], get_data[3]);
    }

    //*************************************************************************
    TEST(put_get_uint16_t_10bits)
    {
      std::array<unsigned char, 4 * sizeof(uint16_t)> storage{0};
      std::array<uint16_t, 4> put_data    = { uint16_t(0x0001U), uint16_t(0xA55AU), uint16_t(0x5AA5U), uint16_t(0xFFFFU) };
      std::array<uint16_t, 4> expect_data = { uint16_t(0x0001U), uint16_t(0x015AU), uint16_t(0x02A5U), uint16_t(0x03FFU) };
      std::array<uint16_t, 4> get_data    = { uint16_t(0x0000U), uint16_t(0x0000U), uint16_t(0x0000U), uint16_t(0x0000U) };

      etl::bit_stream bit_stream(storage.data(), storage.size());

      // Insert into the stream
      bit_stream.put(put_data[0], 10);
      bit_stream.put(put_data[1], 10);
      bit_stream.put(put_data[2], 10);
      bit_stream.put(put_data[3], 10);

      bit_stream.restart();

      CHECK(bit_stream.get(get_data[0], 10));
      CHECK_EQUAL(expect_data[0], get_data[0]);

      CHECK(bit_stream.get(get_data[1], 10));
      CHECK_EQUAL(expect_data[1], get_data[1]);

      CHECK(bit_stream.get(get_data[2], 10));
      CHECK_EQUAL(expect_data[2], get_data[2]);

      CHECK(bit_stream.get(get_data[3], 10));
      CHECK_EQUAL(expect_data[3], get_data[3]);
    }

    //*************************************************************************
    TEST(put_get_int32_t)
    {
      std::array<unsigned char, 4 * sizeof(int32_t)> storage{0};
      std::array<int32_t, 4> put_data = { int32_t(0x00000001L), int32_t(0xA55AA55AL), int32_t(0x5AA55AA5L), int32_t(0xFFFFFFFFL) };
      std::array<int32_t, 4> get_data = { int32_t(0x00000000L), int32_t(0x00000000L), int32_t(0x00000000L), int32_t(0x00000000L) };

      etl::bit_stream bit_stream(storage.data(), storage.size());

      // Insert into the stream
      bit_stream.put(put_data[0]);
      bit_stream.put(put_data[1]);
      bit_stream.put(put_data[2]);
      bit_stream.put(put_data[3]);

      bit_stream.restart();

      CHECK(bit_stream.get(get_data[0]));
      CHECK_EQUAL(put_data[0], get_data[0]);

      CHECK(bit_stream.get(get_data[1]));
      CHECK_EQUAL(put_data[1], get_data[1]);

      CHECK(bit_stream.get(get_data[2]));
      CHECK_EQUAL(put_data[2], get_data[2]);

      CHECK(bit_stream.get(get_data[3]));
      CHECK_EQUAL(put_data[3], get_data[3]);
    }

    //*************************************************************************
    TEST(put_get_int32_t_22bits)
    {
      std::array<unsigned char, 4 * sizeof(int32_t)> storage{0};
      std::array<int32_t, 4> put_data    = { int32_t(0x00000001L), int32_t(0xA55AA55AL), int32_t(0x5AA55AA5L), int32_t(0xFFFFFFFFL) };
      std::array<int32_t, 4> expect_data = { int32_t(0x00000001L), int32_t(0x001AA55AL), int32_t(0xFFE55AA5L), int32_t(0xFFFFFFFFL) };
      std::array<int32_t, 4> get_data    = { int32_t(0x00000000L), int32_t(0x00000000L), int32_t(0x00000000L), int32_t(0x00000000L) };

      etl::bit_stream bit_stream(storage.data(), storage.size());

      // Insert into the stream
      bit_stream.put(put_data[0], 22);
      bit_stream.put(put_data[1], 22);
      bit_stream.put(put_data[2], 22);
      bit_stream.put(put_data[3], 22);

      bit_stream.restart();

      CHECK(bit_stream.get(get_data[0], 22));
      CHECK_EQUAL(expect_data[0], get_data[0]);

      CHECK(bit_stream.get(get_data[1], 22));
      CHECK_EQUAL(expect_data[1], get_data[1]);

      CHECK(bit_stream.get(get_data[2], 22));
      CHECK_EQUAL(expect_data[2], get_data[2]);

      CHECK(bit_stream.get(get_data[3], 22));
      CHECK_EQUAL(expect_data[3], get_data[3]);
    }

    //*************************************************************************
    TEST(put_get_uint32_t)
    {
      std::array<unsigned char, 4 * sizeof(uint32_t)> storage{0};
      std::array<uint32_t, 4> put_data = { uint32_t(0x00000001UL), uint32_t(0xA55AA55AUL), uint32_t(0x5AA55AA5UL), uint32_t(0xFFFFFFFFUL) };
      std::array<uint32_t, 4> get_data = { uint32_t(0x00000000UL), uint32_t(0x00000000UL), uint32_t(0x00000000UL), uint32_t(0x00000000UL) };

      etl::bit_stream bit_stream(storage.data(), storage.size());

      // Insert into the stream
      bit_stream.put(put_data[0]);
      bit_stream.put(put_data[1]);
      bit_stream.put(put_data[2]);
      bit_stream.put(put_data[3]);

      bit_stream.restart();

      CHECK(bit_stream.get(get_data[0]));
      CHECK_EQUAL(put_data[0], get_data[0]);

      CHECK(bit_stream.get(get_data[1]));
      CHECK_EQUAL(put_data[1], get_data[1]);

      CHECK(bit_stream.get(get_data[2]));
      CHECK_EQUAL(put_data[2], get_data[2]);

      CHECK(bit_stream.get(get_data[3]));
      CHECK_EQUAL(put_data[3], get_data[3]);
    }

    //*************************************************************************
    TEST(put_get_uint32_t_22bits)
    {
      std::array<unsigned char, 4 * sizeof(uint32_t)> storage{0};
      std::array<uint32_t, 4> put_data    = { uint32_t(0x00000001UL), uint32_t(0xA55AA55AUL), uint32_t(0x5AA55AA5UL), uint32_t(0xFFFFFFFFUL) };
      std::array<uint32_t, 4> expect_data = { uint32_t(0x00000001UL), uint32_t(0x001AA55AUL), uint32_t(0x00255AA5UL), uint32_t(0x003FFFFFUL) };
      std::array<uint32_t, 4> get_data    = { uint32_t(0x00000000UL), uint32_t(0x00000000UL), uint32_t(0x00000000UL), uint32_t(0x00000000UL) };

      etl::bit_stream bit_stream(storage.data(), storage.size());

      // Insert into the stream
      bit_stream.put(put_data[0], 22);
      bit_stream.put(put_data[1], 22);
      bit_stream.put(put_data[2], 22);
      bit_stream.put(put_data[3], 22);

      bit_stream.restart();

      CHECK(bit_stream.get(get_data[0], 22));
      CHECK_EQUAL(expect_data[0], get_data[0]);

      CHECK(bit_stream.get(get_data[1], 22));
      CHECK_EQUAL(expect_data[1], get_data[1]);

      CHECK(bit_stream.get(get_data[2], 22));
      CHECK_EQUAL(expect_data[2], get_data[2]);

      CHECK(bit_stream.get(get_data[3], 22));
      CHECK_EQUAL(expect_data[3], get_data[3]);
    }

    //*************************************************************************
    TEST(put_get_int64_t)
    {
      std::array<unsigned char, 4 * sizeof(int64_t)> storage{0};
      std::array<int64_t, 4> put_data    = { int64_t(0x0000000000000001LL), int64_t(0xA55AA55AA55AA55ALL), int64_t(0x5AA55AA55AA55AA5LL), int64_t(0xFFFFFFFFFFFFFFFFLL) };
      std::array<int64_t, 4> expect_data = { int64_t(0x0000000000000001LL), int64_t(0xA55AA55AA55AA55ALL), int64_t(0x5AA55AA55AA55AA5LL), int64_t(0xFFFFFFFFFFFFFFFFLL) };
      std::array<int64_t, 4> get_data    = { int64_t(0x0000000000000000LL), int64_t(0x0000000000000000LL), int64_t(0x0000000000000000LL), int64_t(0x0000000000000000LL) };

      etl::bit_stream bit_stream(storage.data(), storage.size());

      // Insert into the stream
      bit_stream.put(put_data[0]);
      bit_stream.put(put_data[1]);
      bit_stream.put(put_data[2]);
      bit_stream.put(put_data[3]);

      bit_stream.restart();

      CHECK(bit_stream.get(get_data[0]));
      CHECK_EQUAL(expect_data[0], get_data[0]);

      CHECK(bit_stream.get(get_data[1]));
      CHECK_EQUAL(expect_data[1], get_data[1]);

      CHECK(bit_stream.get(get_data[2]));
      CHECK_EQUAL(expect_data[2], get_data[2]);

      CHECK(bit_stream.get(get_data[3]));
      CHECK_EQUAL(expect_data[3], get_data[3]);
    }

    //*************************************************************************
    TEST(put_get_int64_t_47bits)
    {
      std::array<unsigned char, 4 * sizeof(int64_t)> storage{0};
      std::array<int64_t, 4> put_data    = { int64_t(0x0000000000000001LL), int64_t(0xA55AA55AA55AA55ALL), int64_t(0x5AA55AA55AA55AA5LL), int64_t(0xFFFFFFFFFFFFFFFFLL) };
      std::array<int64_t, 4> expect_data = { int64_t(0x0000000000000001LL), int64_t(0x0000255AA55AA55ALL), int64_t(0xFFFFDAA55AA55AA5LL), int64_t(0xFFFFFFFFFFFFFFFFLL) };
      std::array<int64_t, 4> get_data    = { int64_t(0x0000000000000000LL), int64_t(0x0000000000000000LL), int64_t(0x0000000000000000LL), int64_t(0x0000000000000000LL) };

      etl::bit_stream bit_stream(storage.data(), storage.size());

      // Insert into the stream
      bit_stream.put(put_data[0], 47);
      bit_stream.put(put_data[1], 47);
      bit_stream.put(put_data[2], 47);
      bit_stream.put(put_data[3], 47);

      bit_stream.restart();

      CHECK(bit_stream.get(get_data[0], 47));
      CHECK_EQUAL(expect_data[0], get_data[0]);

      CHECK(bit_stream.get(get_data[1], 47));
      CHECK_EQUAL(expect_data[1], get_data[1]);

      CHECK(bit_stream.get(get_data[2], 47));
      CHECK_EQUAL(expect_data[2], get_data[2]);

      CHECK(bit_stream.get(get_data[3], 47));
      CHECK_EQUAL(expect_data[3], get_data[3]);
    }

    //*************************************************************************
    TEST(put_get_uint64_t)
    {
      std::array<unsigned char, 4 * sizeof(uint64_t)> storage{0};
      std::array<uint64_t, 4> put_data    = { uint64_t(0x0000000000000001ULL), uint64_t(0xA55AA55AA55AA55AULL), uint64_t(0x5AA55AA55AA55AA5ULL), uint64_t(0xFFFFFFFFFFFFFFFFULL) };
      std::array<uint64_t, 4> expect_data = { uint64_t(0x0000000000000001ULL), uint64_t(0xA55AA55AA55AA55AULL), uint64_t(0x5AA55AA55AA55AA5ULL), uint64_t(0xFFFFFFFFFFFFFFFFULL) };
      std::array<uint64_t, 4> get_data    = { uint64_t(0x0000000000000000ULL), uint64_t(0x0000000000000000ULL), uint64_t(0x0000000000000000ULL), uint64_t(0x0000000000000000ULL) };

      etl::bit_stream bit_stream(storage.data(), storage.size());

      // Insert into the stream
      bit_stream.put(put_data[0]);
      bit_stream.put(put_data[1]);
      bit_stream.put(put_data[2]);
      bit_stream.put(put_data[3]);

      bit_stream.restart();

      CHECK(bit_stream.get(get_data[0]));
      CHECK_EQUAL(expect_data[0], get_data[0]);

      CHECK(bit_stream.get(get_data[1]));
      CHECK_EQUAL(expect_data[1], get_data[1]);

      CHECK(bit_stream.get(get_data[2]));
      CHECK_EQUAL(expect_data[2], get_data[2]);

      CHECK(bit_stream.get(get_data[3]));
      CHECK_EQUAL(expect_data[3], get_data[3]);
    }

    //*************************************************************************
    TEST(put_get_uint64_t_47bits)
    {
      std::array<unsigned char, 4 * sizeof(uint64_t)> storage{0};
      std::array<uint64_t, 4> put_data    = { uint64_t(0x0000000000000001ULL), uint64_t(0xA55AA55AA55AA55AULL), uint64_t(0x5AA55AA55AA55AA5ULL), uint64_t(0xFFFFFFFFFFFFFFFFULL) };
      std::array<uint64_t, 4> expect_data = { uint64_t(0x0000000000000001ULL), uint64_t(0x0000255AA55AA55AULL), uint64_t(0x00005AA55AA55AA5ULL), uint64_t(0x00007FFFFFFFFFFFULL) };
      std::array<uint64_t, 4> get_data    = { uint64_t(0x0000000000000000ULL), uint64_t(0x0000000000000000ULL), uint64_t(0x0000000000000000ULL), uint64_t(0x0000000000000000ULL) };

      etl::bit_stream bit_stream(storage.data(), storage.size());

      // Insert into the stream
      bit_stream.put(put_data[0], 47);
      bit_stream.put(put_data[1], 47);
      bit_stream.put(put_data[2], 47);
      bit_stream.put(put_data[3], 47);

      bit_stream.restart();

      CHECK(bit_stream.get(get_data[0], 47));
      CHECK_EQUAL(expect_data[0], get_data[0]);

      CHECK(bit_stream.get(get_data[1], 47));
      CHECK_EQUAL(expect_data[1], get_data[1]);

      CHECK(bit_stream.get(get_data[2], 47));
      CHECK_EQUAL(expect_data[2], get_data[2]);

      CHECK(bit_stream.get(get_data[3], 47));
      CHECK_EQUAL(expect_data[3], get_data[3]);
    }

    //*************************************************************************
    TEST(put_get_multiple_full_size)
    {
      char c1 = 90;
      char c2 = -91;
      unsigned short s1 = 23205;
      unsigned short s2 = 42330;
      int32_t i1 = 1520786085L;   // 0x5AA55AA5
      int32_t i2 = -1520786086L;  // 0xA55AA55A
      float f = 3.1415927f;
      double d = 3.1415927;

      std::array<unsigned char, 100> storage{0};

      etl::bit_stream bit_stream(storage.data(), storage.size());

      // Insert into the stream.
      bit_stream.put(c1);
      bit_stream.put(s1);
      bit_stream.put(i1);
      bit_stream.put(f);
      bit_stream.put(i2);
      bit_stream.put(d);
      bit_stream.put(s2);
      bit_stream.put(c2);

      bit_stream.restart();

      char rc1 = 0;
      char rc2 = 0;
      unsigned short rs1 = 0;
      unsigned short rs2 = 0;
      int32_t ri1 = 0;
      int32_t ri2 = 0;
      float rf = 0;
      double rd = 0;

      // Read them all back.
      CHECK(bit_stream.get(rc1));
      CHECK_EQUAL(int(c1), int(rc1));

      CHECK(bit_stream.get(rs1));
      CHECK_EQUAL(s1, rs1);

      CHECK(bit_stream.get(ri1));
      CHECK_EQUAL(i1, ri1);

      CHECK(bit_stream.get(rf));
      CHECK_CLOSE(f, rf, 0.1f);

      CHECK(bit_stream.get(ri2));
      CHECK_EQUAL(i2, ri2);

      CHECK(bit_stream.get(rd));
      CHECK_CLOSE(d, rd, 0.1);

      CHECK(bit_stream.get(rs2));
      CHECK_EQUAL(s2, rs2);

      CHECK(bit_stream.get(rc2));
      CHECK_EQUAL(int(c2), int(rc2));
    }

    //*************************************************************************
    TEST(put_get_multiple_variable_size)
    {
      int8_t c1 = 26;             // 6 bits
      int8_t c2 = -10;            // 7 bits
      unsigned short s1 = 6742; // 13 bits
      unsigned short s2 = 1878; // 11 bits
      int32_t i1 = 2448037L;     // 23 bits
      int32_t i2 = -10836646L;   // 25 bits
      float f = 3.1415927f;
      double d = 3.1415927;
      int64_t ll = 140737488355327LL;

      std::array<unsigned char, 100> storage{0};

      etl::bit_stream bit_stream(storage.data(), storage.size());

      // Insert into the stream.
      bit_stream.put(c1, 6);
      CHECK_EQUAL(1U, bit_stream.size());
      CHECK_EQUAL(6U, bit_stream.bits());

      bit_stream.put(s1, 13);
      CHECK_EQUAL(3U,  bit_stream.size());
      CHECK_EQUAL(19U, bit_stream.bits());

      bit_stream.put(i1, 23);
      CHECK_EQUAL(6U,  bit_stream.size());
      CHECK_EQUAL(42U, bit_stream.bits());

      bit_stream.put(f);
      CHECK_EQUAL(10U, bit_stream.size());
      CHECK_EQUAL(74U, bit_stream.bits());

      bit_stream.put(i2, 25);
      CHECK_EQUAL(13U, bit_stream.size());
      CHECK_EQUAL(99U, bit_stream.bits());

      bit_stream.put(d);
      CHECK_EQUAL(21U,  bit_stream.size());
      CHECK_EQUAL(163U, bit_stream.bits());

      bit_stream.put(s2, 11);
      CHECK_EQUAL(22U,  bit_stream.size());
      CHECK_EQUAL(174U, bit_stream.bits());

      bit_stream.put(c2, 7);
      CHECK_EQUAL(23U, bit_stream.size());
      CHECK_EQUAL(181U, bit_stream.bits());

      bit_stream.put(ll, 47);
      CHECK_EQUAL(29U, bit_stream.size());
      CHECK_EQUAL(228U, bit_stream.bits());

      bit_stream.restart();

      int8_t rc1 = 0;
      int8_t rc2 = 0;
      unsigned short rs1 = 0;
      unsigned short rs2 = 0;
      int32_t ri1 = 0;
      int32_t ri2 = 0;
      float rf = 0;
      double rd = 0;
      int64_t rll = 0;

      // Read them all back.
      CHECK(bit_stream.get(rc1, 6));
      CHECK_EQUAL(int(c1), int(rc1));

      CHECK(bit_stream.get(rs1, 13));
      CHECK_EQUAL(s1, rs1);

      CHECK(bit_stream.get(ri1, 23));
      CHECK_EQUAL(i1, ri1);

      CHECK(bit_stream.get(rf));
      CHECK_CLOSE(f, rf, 0.1f);

      CHECK(bit_stream.get(ri2, 25));
      CHECK_EQUAL(i2, ri2);

      CHECK(bit_stream.get(rd));
      CHECK_CLOSE(d, rd, 0.1);

      CHECK(bit_stream.get(rs2, 11));
      CHECK_EQUAL(s2, rs2);

      CHECK(bit_stream.get(rc2, 7));
      CHECK_EQUAL(int(c2), int(rc2));

      CHECK(bit_stream.get(rll, 47));
      CHECK_EQUAL(int(ll), int(rll));
    }

    //*************************************************************************
    TEST(put_get_object_global)
    {
      std::array<unsigned char, 2 * sizeof(Object)> storage{0};

      etl::bit_stream bit_stream(storage.data(), storage.size());

      Object object1 = { -1234, 2.71578369, 250 };
      Object object2 = {  5678, 5.24685744, 126 };

      CHECK(etl::bit_stream_put(bit_stream, object1));
      CHECK(etl::bit_stream_put(bit_stream, object2));

      Object object1a;
      Object object2a;

      bit_stream.restart();

      CHECK(etl::bit_stream_get(bit_stream, object1a));
      CHECK(etl::bit_stream_get(bit_stream, object2a));

      CHECK_EQUAL(object1, object1a);
      CHECK_EQUAL(object2, object2a);
    }

    //*************************************************************************
    TEST(put_get_multiple_float)
    {

      float  f = 3.1415927f;
      double d = 3.1415927;

      std::array<unsigned char, 12> storage{0};

      etl::bit_stream bit_stream(storage.data(), storage.size());

      bit_stream.put(f);
      bit_stream.put(d);

      bit_stream.restart();

      float rf = 0;
      double rd = 0;

      CHECK(bit_stream.get(rf));
      CHECK_CLOSE(f, rf, 0.1f);

      CHECK(bit_stream.get(rd));
      CHECK_CLOSE(f, rd, 0.1f);
    }
  };
}

#include "etl/private/diagnostic_pop.h"
