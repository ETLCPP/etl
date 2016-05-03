/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl
http://www.etlcpp.com

Copyright(c) 2014 jwellbelove

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

#include <UnitTest++/UnitTest++.h>

#include <cstdint>
#include <type_traits>

#include "../src/binary.h"
#include "../src/bitset.h"
#include "../src/fnv_1.h"
#include "../src/integral_limits.h"

#undef max

// Count bits the easy way.
template <typename T>
size_t test_count(T value)
{
  size_t count = 0;

  for (int i = 0; i < etl::integral_limits<T>::bits; ++i)
  {
    if ((value & (T(1) << i)) != 0)
    {
      ++count;
    }
  }

  return count;
}

// Check parity the easy way.
template <typename T>
size_t test_parity(T value)
{
  size_t count = test_count(value);

  return count & 1;
}

// Power of 2.
uint64_t test_power_of_2(int power)
{
  uint64_t result = 1;

  for (int i = 0; i < power; ++i)
  {
    result *= 2;
  }

  return result;
}

// Fold bits.
template <typename TReturn>
TReturn test_fold_bits(uint64_t value, int size)
{
  int bits_remaining = 64;
  uint64_t mask = test_power_of_2(size) - 1;
  TReturn  result = 0;

  while (bits_remaining > size)
  {
    result = result ^ (value & mask);
    value  = value >> size;
    bits_remaining -= size;
  }

  result = result ^ (value & mask);

  return result;
}

// Slow gray to binary
template <typename T>
T compare_gray_to_binary(T value)
{
  T mask;
  for (mask = value >> 1; mask != 0; mask = mask >> 1)
  {
    value = value ^ mask;
  }

  return value;
}

namespace
{
  SUITE(test_binary)
  {
    //*************************************************************************
    TEST(test_rotate_left8)
    {
      uint8_t value;

      value = 0x00;
      value = etl::rotate_left(value);
      CHECK_EQUAL(0, int(value));

      value = 0x21;
      value = etl::rotate_left(value);
      CHECK_EQUAL(0x42, int(value));

      value = 0x42;
      value = etl::rotate_left(value);
      CHECK_EQUAL(0x84, int(value));

      value = 0x84;
      value = etl::rotate_left(value);
      CHECK_EQUAL(0x09, int(value));

      value = 0xB7;
      value = etl::rotate_left(value, 2);
      CHECK_EQUAL(0xDE, int(value));

      value = 0xB7;
      value = etl::rotate_left(value, 3);
      CHECK_EQUAL(0xBD, int(value));

      value = 0xB7;
      value = etl::rotate_left(value, 4);
      CHECK_EQUAL(0x7B, int(value));

      value = 0xB7;
      value = etl::rotate_left(value, 5);
      CHECK_EQUAL(0xF6, int(value));

      value = 0xB7;
      value = etl::rotate_left(value, 6);
      CHECK_EQUAL(0xED, int(value));

      value = 0xB7;
      value = etl::rotate_left(value, 7);
      CHECK_EQUAL(0xDB, int(value));

      value = 0xB7;
      value = etl::rotate_left(value, 8);
      CHECK_EQUAL(0xB7, int(value));

      value = 0xB7;
      value = etl::rotate_left(value, 9);
      CHECK_EQUAL(0x6F, int(value));
    }

    //*************************************************************************
    TEST(test_rotate_left16)
    {
      uint16_t value;

      value = 0x0000;
      value = etl::rotate_left(value);
      CHECK_EQUAL(0, value);

      value = 0x8421;
      value = etl::rotate_left(value);
      CHECK_EQUAL(0x0843, value);

      value = 0x0843;
      value = etl::rotate_left(value);
      CHECK_EQUAL(0x1086, value);

      value = 0x1086;
      value = etl::rotate_left(value);
      CHECK_EQUAL(0x210C, value);

      value = 0xB73C;
      value = etl::rotate_left(value, 2);
      CHECK_EQUAL(0xDCF2, int(value));

      value = 0xB73C;
      value = etl::rotate_left(value, 3);
      CHECK_EQUAL(0xB9E5, int(value));

      value = 0xB73C;
      value = etl::rotate_left(value, 4);
      CHECK_EQUAL(0x73CB, int(value));

      value = 0xB73C;
      value = etl::rotate_left(value, 5);
      CHECK_EQUAL(0xE796, int(value));

      value = 0xB73C;
      value = etl::rotate_left(value, 6);
      CHECK_EQUAL(0xCF2D, int(value));

      value = 0xB73C;
      value = etl::rotate_left(value, 7);
      CHECK_EQUAL(0x9E5B, int(value));

      value = 0xB73C;
      value = etl::rotate_left(value, 8);
      CHECK_EQUAL(0x3CB7, int(value));

      value = 0xB73C;
      value = etl::rotate_left(value, 9);
      CHECK_EQUAL(0x796E, int(value));

      value = 0xB73C;
      value = etl::rotate_left(value, 9);
      CHECK_EQUAL(0x796E, int(value));

      value = 0xB73C;
      value = etl::rotate_left(value, 10);
      CHECK_EQUAL(0xF2DC, int(value));

      value = 0xB73C;
      value = etl::rotate_left(value, 11);
      CHECK_EQUAL(0xE5B9, int(value));

      value = 0xB73C;
      value = etl::rotate_left(value, 12);
      CHECK_EQUAL(0xCB73, int(value));

      value = 0xB73C;
      value = etl::rotate_left(value, 13);
      CHECK_EQUAL(0x96E7, int(value));

      value = 0xB73C;
      value = etl::rotate_left(value, 14);
      CHECK_EQUAL(0x2DCF, int(value));

      value = 0xB73C;
      value = etl::rotate_left(value, 15);
      CHECK_EQUAL(0x5B9E, int(value));

      value = 0xB73C;
      value = etl::rotate_left(value, 16);
      CHECK_EQUAL(0xB73C, int(value));

      value = 0xB73C;
      value = etl::rotate_left(value, 17);
      CHECK_EQUAL(0x6E79, int(value));
    }

    //*************************************************************************
    TEST(test_rotate_right8)
    {
      uint8_t value;

      value = 0x00;
      value = etl::rotate_right(value);
      CHECK_EQUAL(0, int(value));

      value = 0x21;
      value = etl::rotate_right(value);
      CHECK_EQUAL(0x90, int(value));

      value = 0x42;
      value = etl::rotate_right(value);
      CHECK_EQUAL(0x21, int(value));

      value = 0x84;
      value = etl::rotate_right(value);
      CHECK_EQUAL(0x42, int(value));

      value = 0xB7;
      value = etl::rotate_right(value, 2);
      CHECK_EQUAL(0xED, int(value));

      value = 0xB7;
      value = etl::rotate_right(value, 3);
      CHECK_EQUAL(0xF6, int(value));

      value = 0xB7;
      value = etl::rotate_right(value, 4);
      CHECK_EQUAL(0x7B, int(value));

      value = 0xB7;
      value = etl::rotate_right(value, 5);
      CHECK_EQUAL(0xBD, int(value));

      value = 0xB7;
      value = etl::rotate_right(value, 6);
      CHECK_EQUAL(0xDE, int(value));

      value = 0xB7;
      value = etl::rotate_right(value, 7);
      CHECK_EQUAL(0x6F, int(value));

      value = 0xB7;
      value = etl::rotate_right(value, 8);
      CHECK_EQUAL(0xB7, int(value));

      value = 0xB7;
      value = etl::rotate_right(value, 9);
      CHECK_EQUAL(0xDB, int(value));
    }

    //*************************************************************************
    TEST(test_rotate_right16)
    {
      uint16_t value;

      value = 0x0000;
      value = etl::rotate_right(value);
      CHECK_EQUAL(0, value);

      value = 0x8421;
      value = etl::rotate_right(value);
      CHECK_EQUAL(0xC210, value);

      value = 0xC210;
      value = etl::rotate_right(value);
      CHECK_EQUAL(0x6108, value);

      value = 0x6108;
      value = etl::rotate_right(value);
      CHECK_EQUAL(0x3084, value);

      value = 0xB73C;
      value = etl::rotate_right(value, 2);
      CHECK_EQUAL(0x2DCF, int(value));

      value = 0xB73C;
      value = etl::rotate_right(value, 3);
      CHECK_EQUAL(0x96E7, int(value));

      value = 0xB73C;
      value = etl::rotate_right(value, 4);
      CHECK_EQUAL(0xCB73, int(value));

      value = 0xB73C;
      value = etl::rotate_right(value, 5);
      CHECK_EQUAL(0xE5B9, int(value));

      value = 0xB73C;
      value = etl::rotate_right(value, 6);
      CHECK_EQUAL(0xF2DC, int(value));

      value = 0xB73C;
      value = etl::rotate_right(value, 7);
      CHECK_EQUAL(0x796E, int(value));

      value = 0xB73C;
      value = etl::rotate_right(value, 8);
      CHECK_EQUAL(0x3CB7, int(value));

      value = 0xB73C;
      value = etl::rotate_right(value, 9);
      CHECK_EQUAL(0x9E5B, int(value));

      value = 0xB73C;
      value = etl::rotate_right(value, 9);
      CHECK_EQUAL(0x9E5B, int(value));

      value = 0xB73C;
      value = etl::rotate_right(value, 10);
      CHECK_EQUAL(0xCF2D, int(value));

      value = 0xB73C;
      value = etl::rotate_right(value, 11);
      CHECK_EQUAL(0xE796, int(value));

      value = 0xB73C;
      value = etl::rotate_right(value, 12);
      CHECK_EQUAL(0x73CB, int(value));

      value = 0xB73C;
      value = etl::rotate_right(value, 13);
      CHECK_EQUAL(0xB9E5, int(value));

      value = 0xB73C;
      value = etl::rotate_right(value, 14);
      CHECK_EQUAL(0xDCF2, int(value));

      value = 0xB73C;
      value = etl::rotate_right(value, 15);
      CHECK_EQUAL(0x6E79, int(value));

      value = 0xB73C;
      value = etl::rotate_right(value, 16);
      CHECK_EQUAL(0xB73C, int(value));

      value = 0xB73C;
      value = etl::rotate_right(value, 17);
      CHECK_EQUAL(0x5B9E, int(value));
    }

    //*************************************************************************
    TEST(test_rotate16)
    {
      uint16_t value;
      int offset;

      offset = 4;
      value = 0xB73C;
      value = etl::rotate(value, offset);
      CHECK_EQUAL(0x73CB, int(value));

      offset = -4;
      value = 0xB73C;
      value = etl::rotate(value, offset);
      CHECK_EQUAL(0xCB73, int(value));
    }

    //*************************************************************************
    TEST(test_reverse_bits8)
    {
      uint8_t value;

      value = 0xF0;
      value = etl::reverse_bits(value);
      CHECK_EQUAL(0x0F, int(value));

      value = 0x0F;
      value = etl::reverse_bits(value);
      CHECK_EQUAL(0xF0, int(value));

      value = 0xA5;
      value = etl::reverse_bits(value);
      CHECK_EQUAL(0xA5, int(value));

      value = 0x5A;
      value = etl::reverse_bits(value);
      CHECK_EQUAL(0x5A, int(value));

      value = 0xCA;
      value = etl::reverse_bits(value);
      CHECK_EQUAL(0x53, int(value));

      value = 0xAC;
      value = etl::reverse_bits(value);
      CHECK_EQUAL(0x35, int(value));
    }

    //*************************************************************************
    TEST(test_reverse_bits16)
    {
      uint16_t value;

      value = 0xFC5A;
      value = etl::reverse_bits(value);
      CHECK_EQUAL(0x5A3F, value);

      value = 0x5A5A;
      value = etl::reverse_bits(value);
      CHECK_EQUAL(0x5A5A, value);

      value = 0xA5A5;
      value = etl::reverse_bits(value);
      CHECK_EQUAL(0xA5A5, value);
    }

    //*************************************************************************
    TEST(test_reverse_bits32)
    {
      uint32_t value;

      value = 0xF0C3A55A;
      value = etl::reverse_bits(value);
      CHECK_EQUAL(uint32_t(0x5AA5C30F), value);

      value = 0xA5A5A5A5;
      value = etl::reverse_bits(value);
      CHECK_EQUAL(uint32_t(0xA5A5A5A5), value);

      value = 0x5A5A5A5A;
      value = etl::reverse_bits(value);
      CHECK_EQUAL(uint32_t(0x5A5A5A5A), value);
    }

    //*************************************************************************
    TEST(test_reverse_bits64)
    {
      uint64_t value;

      value = 0x1234ABCDF0C3A55A;
      value = etl::reverse_bits(value);
      CHECK_EQUAL(uint64_t(0x5AA5C30FB3D52C48), value);

      value = 0xA5A5A5A5A5A5A5A5;
      value = etl::reverse_bits(value);
      CHECK_EQUAL(uint64_t(0xA5A5A5A5A5A5A5A5), value);

      value = 0x5A5A5A5A5A5A5A5A;
      value = etl::reverse_bits(value);
      CHECK_EQUAL(uint64_t(0x5A5A5A5A5A5A5A5A), value);
    }

    //*************************************************************************
    TEST(test_reverse_bytes16)
    {
      uint16_t value;

      value = 0xFC5A;
      value = etl::reverse_bytes(value);
      CHECK_EQUAL(0x5AFC, value);

      value = 0x5AA5;
      value = etl::reverse_bytes(value);
      CHECK_EQUAL(0xA55A, value);

      value = 0xA55A;
      value = etl::reverse_bytes(value);
      CHECK_EQUAL(0x5AA5, value);
    }

    //*************************************************************************
    TEST(test_reverse_bytes32)
    {
      uint32_t value;

      value = 0xF0C3A55A;
      value = etl::reverse_bytes(value);
      CHECK_EQUAL(0x5AA5C3F0U, value);

      value = 0xA5A55A5A;
      value = etl::reverse_bytes(value);
      CHECK_EQUAL(0x5A5AA5A5U, value);

      value = 0x5A5AA5A5;
      value = etl::reverse_bytes(value);
      CHECK_EQUAL(0xA5A55A5AU, value);
    }

    //*************************************************************************
    TEST(test_reverse_bytes64)
    {
      uint64_t value;

      value = 0x0123456789ABCDEF;
      value = etl::reverse_bytes(value);
      CHECK_EQUAL(0xEFCDAB8967452301U, value);

      value = 0xA5A55A5AA5A55A5A;
      value = etl::reverse_bytes(value);
      CHECK_EQUAL(0x5A5AA5A55A5AA5A5U, value);

      value = 0x5A5AA5A55A5AA5A5;
      value = etl::reverse_bytes(value);
      CHECK_EQUAL(0xA5A55A5AA5A55A5AU, value);
    }

    //*************************************************************************
    TEST(test_binary_to_gray8)
    {
      uint8_t last_gray = 0;

      for (size_t i = 1; i <= std::numeric_limits<uint8_t>::max(); ++i)
      {
        uint8_t gray = etl::binary_to_gray(uint8_t(i));
        uint8_t result = gray ^ last_gray;
        last_gray = gray;

        bool pass = ((result != 0) && !(result & (result - 1)));

        // Only one bit should be set.
        CHECK(pass);
      }
    }

    //*************************************************************************
    TEST(test_binary_to_gray16)
    {
      uint16_t last_gray = 0;

      for (size_t i = 1; i <= std::numeric_limits<uint16_t>::max(); ++i)
      {
        uint16_t gray   = etl::binary_to_gray(uint16_t(i));
        uint16_t result = gray ^ last_gray;
        last_gray       = gray;

        bool pass = ((result != 0) && !(result & (result - 1)));

        // Only one bit should be set.
        CHECK(pass);
      }
    }

    //*************************************************************************
    TEST(test_binary_to_gray32)
    {
      etl::fnv_1a_32 hash;

      hash.add(1);

      for (size_t i = 1; i < 1000000; ++i)
      {
        uint32_t value = hash.value();

        uint32_t last_gray = etl::binary_to_gray(uint32_t(value));
        uint32_t gray      = etl::binary_to_gray(uint32_t(value + 1));
        uint32_t result    = gray ^ last_gray;

        bool pass = ((result != 0) && !(result & (result - 1)));

        hash.add(1);

        // Only one bit should be set.
        CHECK(pass);
      }
    }

    //*************************************************************************
    TEST(test_binary_to_gray64)
    {
      etl::fnv_1a_64 hash;

      hash.add(1);

      for (size_t i = 1; i < 1000000; ++i)
      {
        uint64_t value = hash.value();

        uint64_t last_gray = etl::binary_to_gray(uint64_t(value));
        uint64_t gray = etl::binary_to_gray(uint64_t(value + 1));
        uint64_t result = gray ^ last_gray;

        bool pass = ((result != 0) && !(result & (result - 1)));

        hash.add(1);

        // Only one bit should be set.
        CHECK(pass);
      }
    }

    //*************************************************************************
    TEST(test_gray_to_binary8)
    {
      for (size_t i = 1; i <= std::numeric_limits<uint8_t>::max(); ++i)
      {
        CHECK_EQUAL(compare_gray_to_binary(uint8_t(i)), etl::gray_to_binary(uint8_t(i)));
      }
    }

    //*************************************************************************
    TEST(test_gray_to_binary16)
    {
      for (size_t i = 1; i <= std::numeric_limits<uint16_t>::max(); ++i)
      {
        CHECK_EQUAL(compare_gray_to_binary(uint16_t(i)), etl::gray_to_binary(uint16_t(i)));
      }
    }

    //*************************************************************************
    TEST(test_gray_to_binary32)
    {
      etl::fnv_1a_32 hash;

      hash.add(1);

      for (size_t i = 1; i < 1000000; ++i)
      {
        uint32_t value = hash.value();
        CHECK_EQUAL(compare_gray_to_binary(value), etl::gray_to_binary(value));
      }
    }

    //*************************************************************************
    TEST(test_gray_to_binary64)
    {
      etl::fnv_1a_64 hash;

      hash.add(1);

      for (size_t i = 1; i < 1000000; ++i)
      {
        uint64_t value = hash.value();
        CHECK_EQUAL(compare_gray_to_binary(value), etl::gray_to_binary(value));
      }
    }

    //*************************************************************************
    TEST(test_count_bits_8)
    {
      for (size_t i = 1; i <= std::numeric_limits<uint8_t>::max(); ++i)
      {
        CHECK_EQUAL(test_count(i), etl::count_bits(uint8_t(i)));
      }
    }

    //*************************************************************************
    TEST(test_count_bits_16)
    {
      for (size_t i = 1; i <= std::numeric_limits<uint16_t>::max(); ++i)
      {
        CHECK_EQUAL(test_count(i), etl::count_bits(uint16_t(i)));
      }
    }

    //*************************************************************************
    TEST(test_count_bits_32)
    {
      etl::fnv_1a_32 hash;

      for (size_t i = 0; i < 1000000; ++i)
      {
        hash.add(1);

        uint32_t value = hash.value();

        CHECK_EQUAL(test_count(value), etl::count_bits(value));
      }
    }

    //*************************************************************************
    TEST(test_count_bits_64)
    {
      etl::fnv_1a_64 hash;

      for (size_t i = 0; i < 1000000; ++i)
      {
        hash.add(1);

        uint64_t value = hash.value();

        CHECK_EQUAL(test_count(value), etl::count_bits(value));
      }
    }

    //*************************************************************************
    TEST(test_parity_8)
    {
      for (size_t i = 1; i <= std::numeric_limits<uint8_t>::max(); ++i)
      {
        CHECK_EQUAL(test_parity(i), etl::parity(uint8_t(i)));
      }
    }

    //*************************************************************************
    TEST(test_parity_16)
    {
      for (size_t i = 1; i <= std::numeric_limits<uint16_t>::max(); ++i)
      {
        CHECK_EQUAL(test_parity(i), etl::parity(uint16_t(i)));
      }
    }

    //*************************************************************************
    TEST(test_parity_32)
    {
      etl::fnv_1a_32 hash;

      for (size_t i = 0; i < 1000000; ++i)
      {
        hash.add(1);

        uint32_t value = hash.value();

        CHECK_EQUAL(test_parity(value), etl::parity(value));
      }
    }

    //*************************************************************************
    TEST(test_parity_64)
    {
      etl::fnv_1a_64 hash;

      for (size_t i = 0; i < 1000000; ++i)
      {
        hash.add(1);

        uint64_t value = hash.value();

        CHECK_EQUAL(test_parity(value), etl::parity(value));
      }
    }

    //*************************************************************************
    TEST(test_fold_bits)
    {
      const uint64_t data = 0xE8C9AACCBC3D9A8F;

      CHECK_EQUAL(test_fold_bits<uint64_t>(data,  1), (etl::fold_bits<uint64_t,  1>(data)));
      CHECK_EQUAL(test_fold_bits<uint64_t>(data,  2), (etl::fold_bits<uint64_t,  2>(data)));
      CHECK_EQUAL(test_fold_bits<uint64_t>(data,  3), (etl::fold_bits<uint64_t,  3>(data)));
      CHECK_EQUAL(test_fold_bits<uint64_t>(data,  4), (etl::fold_bits<uint64_t,  4>(data)));
      CHECK_EQUAL(test_fold_bits<uint64_t>(data,  5), (etl::fold_bits<uint64_t,  5>(data)));
      CHECK_EQUAL(test_fold_bits<uint64_t>(data,  6), (etl::fold_bits<uint64_t,  6>(data)));
      CHECK_EQUAL(test_fold_bits<uint64_t>(data,  7), (etl::fold_bits<uint64_t,  7>(data)));
      CHECK_EQUAL(test_fold_bits<uint64_t>(data,  8), (etl::fold_bits<uint64_t,  8>(data)));
      CHECK_EQUAL(test_fold_bits<uint64_t>(data,  9), (etl::fold_bits<uint64_t,  9>(data)));
      CHECK_EQUAL(test_fold_bits<uint64_t>(data, 10), (etl::fold_bits<uint64_t, 10>(data)));
      CHECK_EQUAL(test_fold_bits<uint64_t>(data, 11), (etl::fold_bits<uint64_t, 11>(data)));
      CHECK_EQUAL(test_fold_bits<uint64_t>(data, 12), (etl::fold_bits<uint64_t, 12>(data)));
      CHECK_EQUAL(test_fold_bits<uint64_t>(data, 13), (etl::fold_bits<uint64_t, 13>(data)));
      CHECK_EQUAL(test_fold_bits<uint64_t>(data, 14), (etl::fold_bits<uint64_t, 14>(data)));
      CHECK_EQUAL(test_fold_bits<uint64_t>(data, 15), (etl::fold_bits<uint64_t, 15>(data)));
      CHECK_EQUAL(test_fold_bits<uint64_t>(data, 16), (etl::fold_bits<uint64_t, 16>(data)));
      CHECK_EQUAL(test_fold_bits<uint64_t>(data, 17), (etl::fold_bits<uint64_t, 17>(data)));
      CHECK_EQUAL(test_fold_bits<uint64_t>(data, 18), (etl::fold_bits<uint64_t, 18>(data)));
      CHECK_EQUAL(test_fold_bits<uint64_t>(data, 19), (etl::fold_bits<uint64_t, 19>(data)));
      CHECK_EQUAL(test_fold_bits<uint64_t>(data, 20), (etl::fold_bits<uint64_t, 20>(data)));
      CHECK_EQUAL(test_fold_bits<uint64_t>(data, 21), (etl::fold_bits<uint64_t, 21>(data)));
      CHECK_EQUAL(test_fold_bits<uint64_t>(data, 22), (etl::fold_bits<uint64_t, 22>(data)));
      CHECK_EQUAL(test_fold_bits<uint64_t>(data, 23), (etl::fold_bits<uint64_t, 23>(data)));
      CHECK_EQUAL(test_fold_bits<uint64_t>(data, 24), (etl::fold_bits<uint64_t, 24>(data)));
      CHECK_EQUAL(test_fold_bits<uint64_t>(data, 25), (etl::fold_bits<uint64_t, 25>(data)));
      CHECK_EQUAL(test_fold_bits<uint64_t>(data, 26), (etl::fold_bits<uint64_t, 26>(data)));
      CHECK_EQUAL(test_fold_bits<uint64_t>(data, 27), (etl::fold_bits<uint64_t, 27>(data)));
      CHECK_EQUAL(test_fold_bits<uint64_t>(data, 28), (etl::fold_bits<uint64_t, 28>(data)));
      CHECK_EQUAL(test_fold_bits<uint64_t>(data, 29), (etl::fold_bits<uint64_t, 29>(data)));
      CHECK_EQUAL(test_fold_bits<uint64_t>(data, 30), (etl::fold_bits<uint64_t, 30>(data)));
      CHECK_EQUAL(test_fold_bits<uint64_t>(data, 31), (etl::fold_bits<uint64_t, 31>(data)));
      CHECK_EQUAL(test_fold_bits<uint64_t>(data, 32), (etl::fold_bits<uint64_t, 32>(data)));
      CHECK_EQUAL(test_fold_bits<uint64_t>(data, 33), (etl::fold_bits<uint64_t, 33>(data)));
      CHECK_EQUAL(test_fold_bits<uint64_t>(data, 34), (etl::fold_bits<uint64_t, 34>(data)));
      CHECK_EQUAL(test_fold_bits<uint64_t>(data, 35), (etl::fold_bits<uint64_t, 35>(data)));
      CHECK_EQUAL(test_fold_bits<uint64_t>(data, 36), (etl::fold_bits<uint64_t, 36>(data)));
      CHECK_EQUAL(test_fold_bits<uint64_t>(data, 37), (etl::fold_bits<uint64_t, 37>(data)));
      CHECK_EQUAL(test_fold_bits<uint64_t>(data, 38), (etl::fold_bits<uint64_t, 38>(data)));
      CHECK_EQUAL(test_fold_bits<uint64_t>(data, 39), (etl::fold_bits<uint64_t, 39>(data)));
      CHECK_EQUAL(test_fold_bits<uint64_t>(data, 40), (etl::fold_bits<uint64_t, 40>(data)));
      CHECK_EQUAL(test_fold_bits<uint64_t>(data, 41), (etl::fold_bits<uint64_t, 41>(data)));
      CHECK_EQUAL(test_fold_bits<uint64_t>(data, 42), (etl::fold_bits<uint64_t, 42>(data)));
      CHECK_EQUAL(test_fold_bits<uint64_t>(data, 43), (etl::fold_bits<uint64_t, 43>(data)));
      CHECK_EQUAL(test_fold_bits<uint64_t>(data, 44), (etl::fold_bits<uint64_t, 44>(data)));
      CHECK_EQUAL(test_fold_bits<uint64_t>(data, 45), (etl::fold_bits<uint64_t, 45>(data)));
      CHECK_EQUAL(test_fold_bits<uint64_t>(data, 46), (etl::fold_bits<uint64_t, 46>(data)));
      CHECK_EQUAL(test_fold_bits<uint64_t>(data, 47), (etl::fold_bits<uint64_t, 47>(data)));
      CHECK_EQUAL(test_fold_bits<uint64_t>(data, 48), (etl::fold_bits<uint64_t, 48>(data)));
      CHECK_EQUAL(test_fold_bits<uint64_t>(data, 49), (etl::fold_bits<uint64_t, 49>(data)));
      CHECK_EQUAL(test_fold_bits<uint64_t>(data, 50), (etl::fold_bits<uint64_t, 50>(data)));
      CHECK_EQUAL(test_fold_bits<uint64_t>(data, 51), (etl::fold_bits<uint64_t, 51>(data)));
      CHECK_EQUAL(test_fold_bits<uint64_t>(data, 52), (etl::fold_bits<uint64_t, 52>(data)));
      CHECK_EQUAL(test_fold_bits<uint64_t>(data, 53), (etl::fold_bits<uint64_t, 53>(data)));
      CHECK_EQUAL(test_fold_bits<uint64_t>(data, 54), (etl::fold_bits<uint64_t, 54>(data)));
      CHECK_EQUAL(test_fold_bits<uint64_t>(data, 55), (etl::fold_bits<uint64_t, 55>(data)));
      CHECK_EQUAL(test_fold_bits<uint64_t>(data, 56), (etl::fold_bits<uint64_t, 56>(data)));
      CHECK_EQUAL(test_fold_bits<uint64_t>(data, 57), (etl::fold_bits<uint64_t, 57>(data)));
      CHECK_EQUAL(test_fold_bits<uint64_t>(data, 58), (etl::fold_bits<uint64_t, 58>(data)));
      CHECK_EQUAL(test_fold_bits<uint64_t>(data, 59), (etl::fold_bits<uint64_t, 59>(data)));
      CHECK_EQUAL(test_fold_bits<uint64_t>(data, 60), (etl::fold_bits<uint64_t, 60>(data)));
      CHECK_EQUAL(test_fold_bits<uint64_t>(data, 61), (etl::fold_bits<uint64_t, 61>(data)));
      CHECK_EQUAL(test_fold_bits<uint64_t>(data, 62), (etl::fold_bits<uint64_t, 62>(data)));
      CHECK_EQUAL(test_fold_bits<uint64_t>(data, 63), (etl::fold_bits<uint64_t, 63>(data)));
    }

    //*************************************************************************
    TEST(test_max_value_for_bits)
    {
        // Check that the values are correct.
        //CHECK_EQUAL(0, etl::max_value_for_nbits<0>::value);
        CHECK_EQUAL(1U, etl::max_value_for_nbits<1>::value);
        CHECK_EQUAL(3U, etl::max_value_for_nbits<2>::value);
        CHECK_EQUAL(7U, etl::max_value_for_nbits<3>::value);
        CHECK_EQUAL(15U, etl::max_value_for_nbits<4>::value);
        CHECK_EQUAL(31U, etl::max_value_for_nbits<5>::value);
        CHECK_EQUAL(63U, etl::max_value_for_nbits<6>::value);
        CHECK_EQUAL(127U, etl::max_value_for_nbits<7>::value);
        CHECK_EQUAL(255U, etl::max_value_for_nbits<8>::value);
        CHECK_EQUAL(511U, etl::max_value_for_nbits<9>::value);
        CHECK_EQUAL(1023U, etl::max_value_for_nbits<10>::value);
        CHECK_EQUAL(2047U, etl::max_value_for_nbits<11>::value);
        CHECK_EQUAL(4095U, etl::max_value_for_nbits<12>::value);
        CHECK_EQUAL(8191U, etl::max_value_for_nbits<13>::value);
        CHECK_EQUAL(16383U, etl::max_value_for_nbits<14>::value);
        CHECK_EQUAL(32767U, etl::max_value_for_nbits<15>::value);
        CHECK_EQUAL(65535U, etl::max_value_for_nbits<16>::value);
        CHECK_EQUAL(131071U, etl::max_value_for_nbits<17>::value);
        CHECK_EQUAL(262143U, etl::max_value_for_nbits<18>::value);
        CHECK_EQUAL(524287U, etl::max_value_for_nbits<19>::value);
        CHECK_EQUAL(1048575U, etl::max_value_for_nbits<20>::value);
        CHECK_EQUAL(2097151U, etl::max_value_for_nbits<21>::value);
        CHECK_EQUAL(4194303U, etl::max_value_for_nbits<22>::value);
        CHECK_EQUAL(8388607U, etl::max_value_for_nbits<23>::value);
        CHECK_EQUAL(16777215U, etl::max_value_for_nbits<24>::value);
        CHECK_EQUAL(33554431U, etl::max_value_for_nbits<25>::value);
        CHECK_EQUAL(67108863U, etl::max_value_for_nbits<26>::value);
        CHECK_EQUAL(134217727U, etl::max_value_for_nbits<27>::value);
        CHECK_EQUAL(268435455U, etl::max_value_for_nbits<28>::value);
        CHECK_EQUAL(536870911U, etl::max_value_for_nbits<29>::value);
        CHECK_EQUAL(1073741823U, etl::max_value_for_nbits<30>::value);
        CHECK_EQUAL(2147483647U, etl::max_value_for_nbits<31>::value);
        CHECK_EQUAL(4294967295U, etl::max_value_for_nbits<32>::value);
        CHECK_EQUAL(8589934591U, etl::max_value_for_nbits<33>::value);
        CHECK_EQUAL(17179869183U, etl::max_value_for_nbits<34>::value);
        CHECK_EQUAL(34359738367U, etl::max_value_for_nbits<35>::value);
        CHECK_EQUAL(68719476735U, etl::max_value_for_nbits<36>::value);
        CHECK_EQUAL(137438953471U, etl::max_value_for_nbits<37>::value);
        CHECK_EQUAL(274877906943U, etl::max_value_for_nbits<38>::value);
        CHECK_EQUAL(549755813887U, etl::max_value_for_nbits<39>::value);
        CHECK_EQUAL(1099511627775U, etl::max_value_for_nbits<40>::value);
        CHECK_EQUAL(2199023255551U, etl::max_value_for_nbits<41>::value);
        CHECK_EQUAL(4398046511103U, etl::max_value_for_nbits<42>::value);
        CHECK_EQUAL(8796093022207U, etl::max_value_for_nbits<43>::value);
        CHECK_EQUAL(17592186044415U, etl::max_value_for_nbits<44>::value);
        CHECK_EQUAL(35184372088831U, etl::max_value_for_nbits<45>::value);
        CHECK_EQUAL(70368744177663U, etl::max_value_for_nbits<46>::value);
        CHECK_EQUAL(140737488355327U, etl::max_value_for_nbits<47>::value);
        CHECK_EQUAL(281474976710655U, etl::max_value_for_nbits<48>::value);
        CHECK_EQUAL(562949953421311U, etl::max_value_for_nbits<49>::value);
        CHECK_EQUAL(1125899906842623U, etl::max_value_for_nbits<50>::value);
        CHECK_EQUAL(2251799813685247U, etl::max_value_for_nbits<51>::value);
        CHECK_EQUAL(4503599627370495U, etl::max_value_for_nbits<52>::value);
        CHECK_EQUAL(9007199254740991U, etl::max_value_for_nbits<53>::value);
        CHECK_EQUAL(18014398509481983U, etl::max_value_for_nbits<54>::value);
        CHECK_EQUAL(36028797018963967U, etl::max_value_for_nbits<55>::value);
        CHECK_EQUAL(72057594037927935U, etl::max_value_for_nbits<56>::value);
        CHECK_EQUAL(144115188075855871U, etl::max_value_for_nbits<57>::value);
        CHECK_EQUAL(288230376151711743U, etl::max_value_for_nbits<58>::value);
        CHECK_EQUAL(576460752303423487U, etl::max_value_for_nbits<59>::value);
        CHECK_EQUAL(1152921504606846975U, etl::max_value_for_nbits<60>::value);
        CHECK_EQUAL(2305843009213693951U, etl::max_value_for_nbits<61>::value);
        CHECK_EQUAL(4611686018427387903U, etl::max_value_for_nbits<62>::value);
        CHECK_EQUAL(9223372036854775807U, etl::max_value_for_nbits<63>::value);
        CHECK_EQUAL(18446744073709551615U, etl::max_value_for_nbits<64>::value);

        // Check that the value types are correct.
        CHECK((std::is_same<uint8_t,  etl::max_value_for_nbits<0>::value_type>::value));
        CHECK((std::is_same<uint8_t,  etl::max_value_for_nbits<1>::value_type>::value));
        CHECK((std::is_same<uint8_t,  etl::max_value_for_nbits<2>::value_type>::value));
        CHECK((std::is_same<uint8_t,  etl::max_value_for_nbits<3>::value_type>::value));
        CHECK((std::is_same<uint8_t,  etl::max_value_for_nbits<4>::value_type>::value));
        CHECK((std::is_same<uint8_t,  etl::max_value_for_nbits<5>::value_type>::value));
        CHECK((std::is_same<uint8_t,  etl::max_value_for_nbits<6>::value_type>::value));
        CHECK((std::is_same<uint8_t,  etl::max_value_for_nbits<7>::value_type>::value));
        CHECK((std::is_same<uint8_t,  etl::max_value_for_nbits<8>::value_type>::value));
        CHECK((std::is_same<uint16_t, etl::max_value_for_nbits<9>::value_type>::value));
        CHECK((std::is_same<uint16_t, etl::max_value_for_nbits<10>::value_type>::value));
        CHECK((std::is_same<uint16_t, etl::max_value_for_nbits<11>::value_type>::value));
        CHECK((std::is_same<uint16_t, etl::max_value_for_nbits<12>::value_type>::value));
        CHECK((std::is_same<uint16_t, etl::max_value_for_nbits<13>::value_type>::value));
        CHECK((std::is_same<uint16_t, etl::max_value_for_nbits<14>::value_type>::value));
        CHECK((std::is_same<uint16_t, etl::max_value_for_nbits<15>::value_type>::value));
        CHECK((std::is_same<uint16_t, etl::max_value_for_nbits<16>::value_type>::value));
        CHECK((std::is_same<uint32_t, etl::max_value_for_nbits<17>::value_type>::value));
        CHECK((std::is_same<uint32_t, etl::max_value_for_nbits<18>::value_type>::value));
        CHECK((std::is_same<uint32_t, etl::max_value_for_nbits<19>::value_type>::value));
        CHECK((std::is_same<uint32_t, etl::max_value_for_nbits<20>::value_type>::value));
        CHECK((std::is_same<uint32_t, etl::max_value_for_nbits<21>::value_type>::value));
        CHECK((std::is_same<uint32_t, etl::max_value_for_nbits<22>::value_type>::value));
        CHECK((std::is_same<uint32_t, etl::max_value_for_nbits<23>::value_type>::value));
        CHECK((std::is_same<uint32_t, etl::max_value_for_nbits<24>::value_type>::value));
        CHECK((std::is_same<uint32_t, etl::max_value_for_nbits<25>::value_type>::value));
        CHECK((std::is_same<uint32_t, etl::max_value_for_nbits<26>::value_type>::value));
        CHECK((std::is_same<uint32_t, etl::max_value_for_nbits<27>::value_type>::value));
        CHECK((std::is_same<uint32_t, etl::max_value_for_nbits<28>::value_type>::value));
        CHECK((std::is_same<uint32_t, etl::max_value_for_nbits<29>::value_type>::value));
        CHECK((std::is_same<uint32_t, etl::max_value_for_nbits<30>::value_type>::value));
        CHECK((std::is_same<uint32_t, etl::max_value_for_nbits<31>::value_type>::value));
        CHECK((std::is_same<uint32_t, etl::max_value_for_nbits<32>::value_type>::value));
        CHECK((std::is_same<uint64_t, etl::max_value_for_nbits<33>::value_type>::value));
        CHECK((std::is_same<uint64_t, etl::max_value_for_nbits<34>::value_type>::value));
        CHECK((std::is_same<uint64_t, etl::max_value_for_nbits<35>::value_type>::value));
        CHECK((std::is_same<uint64_t, etl::max_value_for_nbits<36>::value_type>::value));
        CHECK((std::is_same<uint64_t, etl::max_value_for_nbits<37>::value_type>::value));
        CHECK((std::is_same<uint64_t, etl::max_value_for_nbits<38>::value_type>::value));
        CHECK((std::is_same<uint64_t, etl::max_value_for_nbits<39>::value_type>::value));
        CHECK((std::is_same<uint64_t, etl::max_value_for_nbits<40>::value_type>::value));
        CHECK((std::is_same<uint64_t, etl::max_value_for_nbits<41>::value_type>::value));
        CHECK((std::is_same<uint64_t, etl::max_value_for_nbits<42>::value_type>::value));
        CHECK((std::is_same<uint64_t, etl::max_value_for_nbits<43>::value_type>::value));
        CHECK((std::is_same<uint64_t, etl::max_value_for_nbits<44>::value_type>::value));
        CHECK((std::is_same<uint64_t, etl::max_value_for_nbits<45>::value_type>::value));
        CHECK((std::is_same<uint64_t, etl::max_value_for_nbits<46>::value_type>::value));
        CHECK((std::is_same<uint64_t, etl::max_value_for_nbits<47>::value_type>::value));
        CHECK((std::is_same<uint64_t, etl::max_value_for_nbits<48>::value_type>::value));
        CHECK((std::is_same<uint64_t, etl::max_value_for_nbits<49>::value_type>::value));
        CHECK((std::is_same<uint64_t, etl::max_value_for_nbits<50>::value_type>::value));
        CHECK((std::is_same<uint64_t, etl::max_value_for_nbits<51>::value_type>::value));
        CHECK((std::is_same<uint64_t, etl::max_value_for_nbits<52>::value_type>::value));
        CHECK((std::is_same<uint64_t, etl::max_value_for_nbits<53>::value_type>::value));
        CHECK((std::is_same<uint64_t, etl::max_value_for_nbits<54>::value_type>::value));
        CHECK((std::is_same<uint64_t, etl::max_value_for_nbits<55>::value_type>::value));
        CHECK((std::is_same<uint64_t, etl::max_value_for_nbits<56>::value_type>::value));
        CHECK((std::is_same<uint64_t, etl::max_value_for_nbits<57>::value_type>::value));
        CHECK((std::is_same<uint64_t, etl::max_value_for_nbits<58>::value_type>::value));
        CHECK((std::is_same<uint64_t, etl::max_value_for_nbits<59>::value_type>::value));
        CHECK((std::is_same<uint64_t, etl::max_value_for_nbits<60>::value_type>::value));
        CHECK((std::is_same<uint64_t, etl::max_value_for_nbits<61>::value_type>::value));
        CHECK((std::is_same<uint64_t, etl::max_value_for_nbits<62>::value_type>::value));
        CHECK((std::is_same<uint64_t, etl::max_value_for_nbits<63>::value_type>::value));
        CHECK((std::is_same<uint64_t, etl::max_value_for_nbits<64>::value_type>::value));
    }

    //*************************************************************************
    TEST(test_sign_extend_template1)
    {
      uint8_t value8 = 0x2A;
      value8 &= 0x3F; // 6 bit number.

      CHECK_EQUAL(-22, (etl::sign_extend<int8_t,  6>(value8)));
      CHECK_EQUAL(-22, (etl::sign_extend<int16_t, 6>(value8)));
      CHECK_EQUAL(-22, (etl::sign_extend<int32_t, 6>(value8)));
      CHECK_EQUAL(-22, (etl::sign_extend<int64_t, 6>(value8)));

      uint16_t value16 = 0x2AAA;
      value16 &= 0x3FFF; // 14 bit number.

      CHECK_EQUAL(-5462, (etl::sign_extend<int16_t, 14>(value16)));
      CHECK_EQUAL(-5462, (etl::sign_extend<int32_t, 14>(value16)));
      CHECK_EQUAL(-5462, (etl::sign_extend<int64_t, 14>(value16)));

      uint32_t value32 = 0x2AAAAAAA;
      value32 &= 0x3FFFFFFF; // 30 bit number.

      CHECK_EQUAL(-357913942, (etl::sign_extend<int32_t, 30>(value32)));
      CHECK_EQUAL(-357913942, (etl::sign_extend<int64_t, 30>(value32)));

      value8 = 0x0A;
      value8 &= 0x3F; // 6 bit number.

      CHECK_EQUAL(10, (etl::sign_extend<int8_t, 6>(value8)));
      CHECK_EQUAL(10, (etl::sign_extend<int16_t, 6>(value8)));
      CHECK_EQUAL(10, (etl::sign_extend<int32_t, 6>(value8)));
      CHECK_EQUAL(10, (etl::sign_extend<int64_t, 6>(value8)));

      value16 = 0x0AAA;
      value16 &= 0x3FFF; // 14 bit number.

      CHECK_EQUAL(2730, (etl::sign_extend<int16_t, 14>(value16)));
      CHECK_EQUAL(2730, (etl::sign_extend<int32_t, 14>(value16)));
      CHECK_EQUAL(2730, (etl::sign_extend<int64_t, 14>(value16)));

      value32 = 0x0AAAAAAA;
      value32 &= 0x3FFFFFFF; // 30 bit number.

      CHECK_EQUAL(178956970, (etl::sign_extend<int32_t, 30>(value32)));
      CHECK_EQUAL(178956970, (etl::sign_extend<int64_t, 30>(value32)));
    }

    //*************************************************************************
    TEST(test_sign_extend_template2)
    {
      uint8_t value8 = 0x2A;
      value8 &= 0x3F; // 6 bit number.

      CHECK_EQUAL(-22, (etl::sign_extend<int8_t>(value8, 6)));
      CHECK_EQUAL(-22, (etl::sign_extend<int16_t>(value8, 6)));
      CHECK_EQUAL(-22, (etl::sign_extend<int32_t>(value8, 6)));
      CHECK_EQUAL(-22, (etl::sign_extend<int64_t>(value8, 6)));

      uint16_t value16 = 0x2AAA;
      value16 &= 0x3FFF; // 14 bit number.

      CHECK_EQUAL(-5462, (etl::sign_extend<int16_t>(value16, 14)));
      CHECK_EQUAL(-5462, (etl::sign_extend<int32_t>(value16, 14)));
      CHECK_EQUAL(-5462, (etl::sign_extend<int64_t>(value16, 14)));

      uint32_t value32 = 0x2AAAAAAA;
      value32 &= 0x3FFFFFFF; // 30 bit number.

      CHECK_EQUAL(-357913942, (etl::sign_extend<int32_t>(value32, 30)));
      CHECK_EQUAL(-357913942, (etl::sign_extend<int64_t>(value32, 30)));

      value8 = 0x0A;
      value8 &= 0x3F; // 6 bit number.

      CHECK_EQUAL(10, (etl::sign_extend<int8_t>(value8, 6)));
      CHECK_EQUAL(10, (etl::sign_extend<int16_t>(value8, 6)));
      CHECK_EQUAL(10, (etl::sign_extend<int32_t>(value8, 6)));
      CHECK_EQUAL(10, (etl::sign_extend<int64_t>(value8, 6)));

      value16 = 0x0AAA;
      value16 &= 0x3FFF; // 14 bit number.

      CHECK_EQUAL(2730, (etl::sign_extend<int16_t>(value16, 14)));
      CHECK_EQUAL(2730, (etl::sign_extend<int32_t>(value16, 14)));
      CHECK_EQUAL(2730, (etl::sign_extend<int64_t>(value16, 14)));

      value32 = 0x0AAAAAAA;
      value32 &= 0x3FFFFFFF; // 30 bit number.

      CHECK_EQUAL(178956970, (etl::sign_extend<int32_t>(value32, 30)));
      CHECK_EQUAL(178956970, (etl::sign_extend<int64_t>(value32, 30)));
    }
  };
}

