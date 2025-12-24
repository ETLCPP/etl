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

#include <limits>

#include "etl/binary.h"
#include "etl/bit.h"
#include "etl/fnv_1.h"

#include "etl/private/diagnostic_useless_cast_push.h"

namespace
{
  //***********************************
  // Count bits the long way.
  template <typename T>
  int test_count(T value)
  {
    int count = 0;

    for (int i = 0; i < etl::integral_limits<T>::bits; ++i)
    {
      if ((value & (T(1) << i)) != 0)
      {
        ++count;
      }
    }

    return count;
  }

  //***********************************
  // Count trailing zeros the long way.
  template <typename T>
  int test_trailing_zeros(T value)
  {
    int count = 0;

    for (int i = 0; i < etl::integral_limits<T>::bits; ++i)
    {
      if ((value & 1) == 0)
      {
        ++count;
      }
      else
      {
        return count;
      }

      value >>= 1;
    }

    return count;
  }

  //***********************************
  // Count leading zeros the long way.
  template <typename T>
  int test_leading_zeros(T value)
  {
    value = etl::reverse_bits(value);
    return test_trailing_zeros(value);
  }

  //***********************************
  // Count trailing ones the long way.
  template <typename T>
  int test_trailing_ones(T value)
  {
    int count = 0UL;

    for (int i = 0; i < etl::integral_limits<T>::bits; ++i)
    {
      if ((value & 1) == 1)
      {
        ++count;
      }
      else
      {
        return count;
      }

      value >>= 1;
    }

    return count;
  }

  //***********************************
  // Count leading ones the long way.
  template <typename T>
  int test_leading_ones(T value)
  {
    value = etl::reverse_bits(value);
    return test_trailing_ones(value);
  }

  //***********************************
  // Check parity the long way.
  template <typename T>
  size_t test_parity(T value)
  {
    size_t count = test_count(value);

    return count & 1;
  }

  //***********************************
  // Count the bit width the long way.
  template <typename T>
  T test_bit_width(T value)
  {
    value = etl::reverse_bits(value);

    size_t width = 0U;

    for (int i = 0; i < etl::integral_limits<T>::bits; ++i)
    {
      if ((value & T(T(1) << i)) == 0)
      {
        ++width;
      }
      else
      {
        break;
      }
    }

    return T(etl::integral_limits<T>::bits - width);
  }

  //***********************************
  // Count the bit ceil the long way.
  template <typename T>
  T test_bit_ceil(T value)
  {
    T ceil;

    if (value == T(0))
    {
      ceil = T(1);
    }
    else
    {
      ceil = T(1) << test_bit_width(T(value - T(1)));
    }

    return ceil;
  }

  //***********************************
  // Count the bit floor the long way.
  template <typename T>
  T test_bit_floor(T value)
  {
    if (value == 0)
    {
      return 0;
    }

    T floor = T(T(1) << (etl::integral_limits<T>::bits - T(1)));

    for (int i = etl::integral_limits<T>::bits - 1U; (i > 0) && (floor > value); ++i)
    {
      floor >>= 1;
    }

    return floor;
  }

  SUITE(test_bit)
  {
    //*************************************************************************
    TEST(test_bit_cast_integrals)
    {
      int32_t  i;
      uint32_t ui;

      i  = 0x12345678;
      ui = etl::bit_cast<uint32_t>(i);
      CHECK_EQUAL(i, ui);

      i  = -1234567890;
      ui = etl::bit_cast<uint32_t>(i);
      CHECK_EQUAL(i, static_cast<int32_t>(ui));
    }

    //*************************************************************************
    TEST(test_bit_cast_different_types)
    {
      using Int = etl::smallest_int_for_bits_t<sizeof(float) * CHAR_BIT>;

      Int i1;
      float f = 123.456789f;
      memcpy(&i1, &f, sizeof(float));

      Int i2 = etl::bit_cast<Int>(f);
      CHECK_EQUAL(i1, i2);
    }

    //*************************************************************************
    TEST(test_rotl_8)
    {
      uint8_t value;

      value = 0x00U;
      value = etl::rotl(value, 1);
      CHECK_EQUAL(0, int(value));

      value = 0x21U;
      value = etl::rotl(value, 1);
      CHECK_EQUAL(0x42U, int(value));

      value = 0x42U;
      value = etl::rotl(value, 1);
      CHECK_EQUAL(0x84U, int(value));

      value = 0x84U;
      value = etl::rotl(value, 1);
      CHECK_EQUAL(0x09U, int(value));

      value = 0xB7U;
      value = etl::rotl(value, 2);
      CHECK_EQUAL(0xDEU, int(value));

      value = 0xB7U;
      value = etl::rotl(value, 3);
      CHECK_EQUAL(0xBDU, int(value));

      value = 0xB7U;
      value = etl::rotl(value, 4);
      CHECK_EQUAL(0x7BU, int(value));

      value = 0xB7U;
      value = etl::rotl(value, 5);
      CHECK_EQUAL(0xF6U, int(value));

      value = 0xB7U;
      value = etl::rotl(value, 6);
      CHECK_EQUAL(0xEDU, int(value));

      value = 0xB7U;
      value = etl::rotl(value, 7);
      CHECK_EQUAL(0xDBU, int(value));

      value = 0xB7U;
      value = etl::rotl(value, 8);
      CHECK_EQUAL(0xB7U, int(value));

      value = 0xB7U;
      value = etl::rotl(value, 9);
      CHECK_EQUAL(0x6FU, int(value));

      value = 0xB7U;
      value = etl::rotl(value, -1);
      CHECK_EQUAL(0xDBU, int(value));
    }

#if ETL_USING_CPP14
    //*************************************************************************
    TEST(test_rotl_8_constexpr)
    {
      constexpr size_t Size = etl::rotl(uint8_t(0xAAU), 1);
      char temp[Size];

      CHECK_EQUAL(etl::rotl(uint8_t(0xAAU), 1), sizeof(temp));
    }
#endif

    //*************************************************************************
    TEST(test_rotl_16)
    {
      uint16_t value;

      value = 0x0000U;
      value = etl::rotl(value, 1);
      CHECK_EQUAL(0, value);

      value = 0x8421U;
      value = etl::rotl(value, 1);
      CHECK_EQUAL(0x0843U, value);

      value = 0x0843U;
      value = etl::rotl(value, 1);
      CHECK_EQUAL(0x1086U, value);

      value = 0x1086U;
      value = etl::rotl(value, 1);
      CHECK_EQUAL(0x210CU, value);

      value = 0xB73CU;
      value = etl::rotl(value, 2);
      CHECK_EQUAL(0xDCF2U, int(value));

      value = 0xB73CU;
      value = etl::rotl(value, 3);
      CHECK_EQUAL(0xB9E5U, int(value));

      value = 0xB73CU;
      value = etl::rotl(value, 4);
      CHECK_EQUAL(0x73CBU, int(value));

      value = 0xB73CU;
      value = etl::rotl(value, 5);
      CHECK_EQUAL(0xE796U, int(value));

      value = 0xB73CU;
      value = etl::rotl(value, 6);
      CHECK_EQUAL(0xCF2DU, int(value));

      value = 0xB73CU;
      value = etl::rotl(value, 7);
      CHECK_EQUAL(0x9E5BU, int(value));

      value = 0xB73CU;
      value = etl::rotl(value, 8);
      CHECK_EQUAL(0x3CB7U, int(value));

      value = 0xB73CU;
      value = etl::rotl(value, 9);
      CHECK_EQUAL(0x796EU, int(value));

      value = 0xB73CU;
      value = etl::rotl(value, 9);
      CHECK_EQUAL(0x796EU, int(value));

      value = 0xB73CU;
      value = etl::rotl(value, 10);
      CHECK_EQUAL(0xF2DCU, int(value));

      value = 0xB73CU;
      value = etl::rotl(value, 11);
      CHECK_EQUAL(0xE5B9U, int(value));

      value = 0xB73CU;
      value = etl::rotl(value, 12);
      CHECK_EQUAL(0xCB73U, int(value));

      value = 0xB73CU;
      value = etl::rotl(value, 13);
      CHECK_EQUAL(0x96E7U, int(value));

      value = 0xB73CU;
      value = etl::rotl(value, 14);
      CHECK_EQUAL(0x2DCFU, int(value));

      value = 0xB73CU;
      value = etl::rotl(value, 15);
      CHECK_EQUAL(0x5B9EU, int(value));

      value = 0xB73CU;
      value = etl::rotl(value, 16);
      CHECK_EQUAL(0xB73CU, int(value));

      value = 0xB73CU;
      value = etl::rotl(value, 17);
      CHECK_EQUAL(0x6E79U, int(value));

      value = 0xB73CU;
      value = etl::rotl(value, -1);
      CHECK_EQUAL(0x5B9EU, int(value));
    }

#if ETL_USING_CPP14
    //*************************************************************************
    TEST(test_rotl_16_constexpr)
    {
      constexpr size_t Size = etl::rotl(uint16_t(0xAAU), 1);
      char temp[Size];

      CHECK_EQUAL(etl::rotl(uint16_t(0xAAU), 1), sizeof(temp));
    }
#endif

    //*************************************************************************
    TEST(test_test_rotr_8)
    {
      uint8_t value;

      value = 0x00U;
      value = etl::rotr(value, 1);
      CHECK_EQUAL(0, int(value));

      value = 0x21U;
      value = etl::rotr(value, 1);
      CHECK_EQUAL(0x90U, int(value));

      value = 0x42U;
      value = etl::rotr(value, 1);
      CHECK_EQUAL(0x21U, int(value));

      value = 0x84U;
      value = etl::rotr(value, 1);
      CHECK_EQUAL(0x42U, int(value));

      value = 0xB7U;
      value = etl::rotr(value, 2);
      CHECK_EQUAL(0xEDU, int(value));

      value = 0xB7U;
      value = etl::rotr(value, 3);
      CHECK_EQUAL(0xF6U, int(value));

      value = 0xB7U;
      value = etl::rotr(value, 4);
      CHECK_EQUAL(0x7BU, int(value));

      value = 0xB7U;
      value = etl::rotr(value, 5);
      CHECK_EQUAL(0xBDU, int(value));

      value = 0xB7U;
      value = etl::rotr(value, 6);
      CHECK_EQUAL(0xDEU, int(value));

      value = 0xB7U;
      value = etl::rotr(value, 7);
      CHECK_EQUAL(0x6FU, int(value));

      value = 0xB7U;
      value = etl::rotr(value, 8);
      CHECK_EQUAL(0xB7U, int(value));

      value = 0xB7U;
      value = etl::rotr(value, 9);
      CHECK_EQUAL(0xDBU, int(value));

      value = 0xB7U;
      value = etl::rotr(value, -1);
      CHECK_EQUAL(0x6FU, int(value));
    }

#if ETL_USING_CPP14
    //*************************************************************************
    TEST(test_rotr_8_constexpr)
    {
      constexpr size_t Size = etl::rotr(uint8_t(0xAAU), 1);
      char temp[Size];

      CHECK_EQUAL(etl::rotr(uint8_t(0xAAU), 1), sizeof(temp));
    }
#endif

    //*************************************************************************
    TEST(test_rotr_16)
    {
      uint16_t value;

      value = 0x0000U;
      value = etl::rotr(value, 1);
      CHECK_EQUAL(0, value);

      value = 0x8421U;
      value = etl::rotr(value, 1);
      CHECK_EQUAL(0xC210U, value);

      value = 0xC210U;
      value = etl::rotr(value, 1);
      CHECK_EQUAL(0x6108U, value);

      value = 0x6108U;
      value = etl::rotr(value, 1);
      CHECK_EQUAL(0x3084U, value);

      value = 0xB73CU;
      value = etl::rotr(value, 2);
      CHECK_EQUAL(0x2DCFU, int(value));

      value = 0xB73CU;
      value = etl::rotr(value, 3);
      CHECK_EQUAL(0x96E7U, int(value));

      value = 0xB73CU;
      value = etl::rotr(value, 4);
      CHECK_EQUAL(0xCB73U, int(value));

      value = 0xB73CU;
      value = etl::rotr(value, 5);
      CHECK_EQUAL(0xE5B9U, int(value));

      value = 0xB73CU;
      value = etl::rotr(value, 6);
      CHECK_EQUAL(0xF2DCU, int(value));

      value = 0xB73CU;
      value = etl::rotr(value, 7);
      CHECK_EQUAL(0x796EU, int(value));

      value = 0xB73CU;
      value = etl::rotr(value, 8);
      CHECK_EQUAL(0x3CB7U, int(value));

      value = 0xB73CU;
      value = etl::rotr(value, 9);
      CHECK_EQUAL(0x9E5BU, int(value));

      value = 0xB73CU;
      value = etl::rotr(value, 9);
      CHECK_EQUAL(0x9E5BU, int(value));

      value = 0xB73CU;
      value = etl::rotr(value, 10);
      CHECK_EQUAL(0xCF2DU, int(value));

      value = 0xB73CU;
      value = etl::rotr(value, 11);
      CHECK_EQUAL(0xE796U, int(value));

      value = 0xB73CU;
      value = etl::rotr(value, 12);
      CHECK_EQUAL(0x73CBU, int(value));

      value = 0xB73CU;
      value = etl::rotr(value, 13);
      CHECK_EQUAL(0xB9E5U, int(value));

      value = 0xB73CU;
      value = etl::rotr(value, 14);
      CHECK_EQUAL(0xDCF2U, int(value));

      value = 0xB73CU;
      value = etl::rotr(value, 15);
      CHECK_EQUAL(0x6E79U, int(value));

      value = 0xB73CU;
      value = etl::rotr(value, 16);
      CHECK_EQUAL(0xB73CU, int(value));

      value = 0xB73CU;
      value = etl::rotr(value, 17);
      CHECK_EQUAL(0x5B9EU, int(value));

      value = 0xB73CU;
      value = etl::rotr(value, -1);
      CHECK_EQUAL(0x6E79U, int(value));
    }

#if ETL_USING_CPP14
    //*************************************************************************
    TEST(test_rotr_16_constexpr)
    {
      constexpr size_t Size = etl::rotr(uint16_t(0xAAU), 1);
      char temp[Size];

      CHECK_EQUAL(etl::rotr(uint16_t(0xAAU), 1), sizeof(temp));
    }
#endif

    //*************************************************************************
    TEST(test_rotate16)
    {
      uint16_t value;
      int offset;

      offset = 4;
      value = 0xB73CU;
      value = etl::rotate(value, offset);
      CHECK_EQUAL(0x73CBU, int(value));

      offset = -4;
      value = 0xB73CU;
      value = etl::rotate(value, offset);
      CHECK_EQUAL(0xCB73U, int(value));
    }

#if ETL_USING_CPP14
    //*************************************************************************
    TEST(test_rotate16_constexpr)
    {
      constexpr size_t Size = etl::rotate(uint16_t(0xAAU), 1);
      char temp[Size];

      CHECK_EQUAL(etl::rotate(uint16_t(0xAAU), 1), sizeof(temp));
    }
#endif

    //*************************************************************************
    TEST(test_byteswap_8)
    {
      uint8_t value;

      value = 0x5AU;
      value = etl::byteswap(value);
      CHECK_EQUAL(0x5AU, value);
    }

#if ETL_USING_CPP14
    //*************************************************************************
    TEST(test_byteswap_8_constexpr)
    {
      constexpr size_t Size = etl::byteswap(uint8_t(0x12U));
      char temp[Size];

      CHECK_EQUAL(etl::byteswap(uint8_t(0x12)), sizeof(temp));
    }
#endif

    //*************************************************************************
    TEST(test_byteswap_16)
    {
      uint16_t value;

      value = 0xFC5AU;
      value = etl::byteswap(value);
      CHECK_EQUAL(0x5AFCU, value);

      value = 0x5AA5U;
      value = etl::byteswap(value);
      CHECK_EQUAL(0xA55AU, value);

      value = 0xA55AU;
      value = etl::byteswap(value);
      CHECK_EQUAL(0x5AA5U, value);
    }

#if ETL_USING_CPP14
    //*************************************************************************
    TEST(test_byteswap_16_constexpr)
    {
      constexpr size_t Size = etl::byteswap(uint16_t(0xA500U));
      char temp[Size];

      CHECK_EQUAL(etl::byteswap(uint16_t(0xA500U)), sizeof(temp));
    }
#endif

    //*************************************************************************
    TEST(test_byteswap_32)
    {
      uint32_t value;

      value = 0xF0C3A55AUL;
      value = etl::byteswap(value);
      CHECK_EQUAL(0x5AA5C3F0UL, value);

      value = 0xA5A55A5AUL;
      value = etl::byteswap(value);
      CHECK_EQUAL(0x5A5AA5A5UL, value);

      value = 0x5A5AA5A5UL;
      value = etl::byteswap(value);
      CHECK_EQUAL(0xA5A55A5AUL, value);
    }

#if ETL_USING_CPP14
    //*************************************************************************
    TEST(test_byteswap_32_constexpr)
    {
      constexpr uint32_t Size = etl::byteswap(uint32_t(0xA5000000UL));
      char temp[Size];

      CHECK_EQUAL(etl::byteswap(uint32_t(0xA5000000UL)), sizeof(temp));
    }
#endif

    //*************************************************************************
    TEST(test_byteswap_64)
    {
      uint64_t value;

      value = 0x0123456789ABCDEFULL;
      value = etl::byteswap(value);
      CHECK_EQUAL(0xEFCDAB8967452301ULL, value);

      value = 0xA5A55A5AA5A55A5AULL;
      value = etl::byteswap(value);
      CHECK_EQUAL(0x5A5AA5A55A5AA5A5ULL, value);

      value = 0x5A5AA5A55A5AA5A5ULL;
      value = etl::byteswap(value);
      CHECK_EQUAL(0xA5A55A5AA5A55A5AULL, value);
    }

#if ETL_USING_CPP14
    //*************************************************************************
    TEST(test_byteswap_64_constexpr)
    {
      constexpr uint64_t Size = etl::byteswap(uint64_t(0xA500000000000000ULL));
      char temp[Size];

      CHECK_EQUAL(etl::byteswap(uint64_t(0xA500000000000000ULL)), sizeof(temp));
    }
#endif

    //*************************************************************************
    TEST(test_popcount_8)
    {
      for (size_t i = 1UL; i <= std::numeric_limits<uint8_t>::max(); ++i)
      {
        CHECK_EQUAL(test_count(i), etl::popcount(uint8_t(i)));
      }
    }

#if ETL_USING_CPP14
    //*************************************************************************
    TEST(test_popcount_8_constexpr)
    {
      constexpr size_t Size = etl::popcount(uint8_t(0xFFU));     
      char temp[Size];

      CHECK_EQUAL(etl::popcount(uint8_t(0xFFU)), sizeof(temp));
    }
#endif

    //*************************************************************************
    TEST(test_popcount_16)
    {
      for (size_t i = 1UL; i <= std::numeric_limits<uint16_t>::max(); ++i)
      {
        CHECK_EQUAL(test_count(i), etl::popcount(uint16_t(i)));
      }
    }

#if ETL_USING_CPP14
    //*************************************************************************
    TEST(test_popcount_16_constexpr)
    {
      constexpr size_t Size = etl::popcount(uint16_t(0xFFU));
      char temp[Size];

      CHECK_EQUAL(etl::popcount(uint16_t(0xFFU)), sizeof(temp));
    }
#endif

    //*************************************************************************
    TEST(test_popcount_32)
    {
      etl::fnv_1a_32 hash;

      for (size_t i = 0UL; i < 1000000UL; ++i)
      {
        hash.add(1);

        uint32_t value = hash.value();

        CHECK_EQUAL(test_count(value), etl::popcount(value));
      }
    }

#if ETL_USING_CPP14
    //*************************************************************************
    TEST(test_popcount_32_constexpr)
    {
      constexpr size_t Size = etl::popcount(uint32_t(0xFFU));
      char temp[Size];

      CHECK_EQUAL(etl::popcount(uint32_t(0xFFU)), sizeof(temp));
    }
#endif

    //*************************************************************************
    TEST(test_popcount_64)
    {
      etl::fnv_1a_64 hash;

      for (size_t i = 0UL; i < 1000000UL; ++i)
      {
        hash.add(1);

        uint64_t value = hash.value();

        CHECK_EQUAL(test_count(value), etl::popcount(value));
      }
    }

#if ETL_USING_CPP14
    //*************************************************************************
    TEST(test_popcount_64_constexpr)
    {
      constexpr size_t Size = etl::popcount(uint64_t(0xFFU));
      char temp[Size];

      CHECK_EQUAL(etl::popcount(uint64_t(0xFFU)), sizeof(temp));
    }
#endif

    //*************************************************************************
    TEST(test_countr_zero_8)
    {
      etl::fnv_1a_32 hash;

      for (size_t i = 0; i < 256; ++i)
      {
        uint8_t value = uint8_t(i);

        CHECK_EQUAL(test_trailing_zeros(value), etl::countr_zero(value));
      }
    }

#if ETL_USING_CPP14
    //*************************************************************************
    TEST(test_countr_zero_8_constexpr)
    {
      constexpr size_t Size = etl::countr_zero(uint8_t(0x08));
      char temp[Size];

      CHECK_EQUAL(etl::countr_zero(uint8_t(0x08)), sizeof(temp));
    }
#endif

    //*************************************************************************
    TEST(test_countr_zero_16)
    {
      for (size_t i = 0; i < 65536; ++i)
      {
        uint16_t value = uint16_t(i);

        CHECK_EQUAL(test_trailing_zeros(value), etl::countr_zero(value));
      }
    }

#if ETL_USING_CPP14
    //*************************************************************************
    TEST(test_countr_zero_16_constexpr)
    {
      constexpr size_t Size = etl::countr_zero(uint16_t(0x08));
      char temp[Size];

      CHECK_EQUAL(etl::countr_zero(uint16_t(0x08)), sizeof(temp));
    }
#endif

    //*************************************************************************
    TEST(test_countr_zero_32)
    {
      etl::fnv_1a_32 hash;

      for (size_t i = 0UL; i < 100000UL; ++i)
      {
        hash.add(1);

        uint32_t value = hash.value();

        CHECK_EQUAL(test_trailing_zeros(value), etl::countr_zero(value));
      }
    }

#if ETL_USING_CPP14
    //*************************************************************************
    TEST(test_countr_zero_32_constexpr)
    {
      constexpr size_t Size = etl::countr_zero(uint32_t(0x08));
      char temp[Size];

      CHECK_EQUAL(etl::countr_zero(uint32_t(0x08)), sizeof(temp));
    }
#endif

    //*************************************************************************
    TEST(test_countr_zero_64)
    {
      etl::fnv_1a_64 hash;

      for (size_t i = 0UL; i < 100000UL; ++i)
      {
        hash.add(1);

        uint64_t value = hash.value();

        CHECK_EQUAL(test_trailing_zeros(value), etl::countr_zero(value));
      }
    }

#if ETL_USING_CPP14
    //*************************************************************************
    TEST(test_countr_zero_64_constexpr)
    {
      constexpr size_t Size = etl::countr_zero(uint64_t(0x08));
      char temp[Size];

      CHECK_EQUAL(etl::countr_zero(uint64_t(0x08)), sizeof(temp));
    }
#endif

    //*************************************************************************
    TEST(test_countr_one_8)
    {
      for (size_t i = 0; i < 256; ++i)
      {
        uint8_t value = uint8_t(i);

        CHECK_EQUAL(int(test_trailing_ones(value)), int(etl::countr_one(value)));

        if (test_trailing_ones(value) != etl::countr_one(value))
        {
          break;
        }
      }
    }

#if ETL_USING_CPP14
    //*************************************************************************
    TEST(test_countr_one_8_constexpr)
    {
      constexpr size_t count = etl::countr_one(uint8_t(0x0F));

      CHECK_EQUAL(test_trailing_ones(uint8_t(0x0F)), count);
    }
#endif

    //*************************************************************************
    TEST(test_countr_one_16)
    {
      for (size_t i = 0; i < 65536; ++i)
      {
        uint16_t value = uint16_t(i);

        CHECK_EQUAL(int(test_trailing_ones(value)), int(etl::countr_one(value)));

        if (test_trailing_ones(value) != etl::countr_one(value))
        {
          break;
        }
      }
    }

#if ETL_USING_CPP14
    //*************************************************************************
    TEST(test_countr_one_16_constexpr)
    {
      constexpr size_t Size = etl::countr_one(uint16_t(0x000F));
      char temp[Size];

      CHECK_EQUAL(test_trailing_ones(uint16_t(0x000F)), sizeof(temp));
    }
#endif

    //*************************************************************************
    TEST(test_countr_one_32)
    {
      etl::fnv_1a_32 hash;

      for (size_t i = 0UL; i < 100000UL; ++i)
      {
        hash.add(1);

        uint32_t value = hash.value();

        CHECK_EQUAL(test_trailing_ones(value), etl::countr_one(value));

        if (test_trailing_ones(value) != etl::countr_one(value))
        {
          break;
        }
      }
    }

#if ETL_USING_CPP14
    //*************************************************************************
    TEST(test_countr_one_32_constexpr)
    {
      constexpr size_t Size = etl::countr_one(uint32_t(0x0000000F));
      char temp[Size];

      CHECK_EQUAL(test_trailing_ones(uint32_t(0x0000000F)), sizeof(temp));
    }
#endif

    //*************************************************************************
    TEST(test_countr_one_64)
    {
      etl::fnv_1a_64 hash;

      for (size_t i = 0UL; i < 100000UL; ++i)
      {
        hash.add(1);

        uint64_t value = hash.value();

        CHECK_EQUAL(test_trailing_ones(value), etl::countr_one(value));

        if (test_trailing_ones(value) != etl::countr_one(value))
        {
          break;
        }
      }
    }
#if ETL_USING_CPP14
    //*************************************************************************
    TEST(test_countr_one_64_constexpr)
    {
      constexpr size_t Size = etl::countr_one(uint64_t(0x000000000000000F));
      char temp[Size];

      CHECK_EQUAL(test_trailing_ones(uint64_t(0x000000000000000F)), sizeof(temp));
    }
#endif

    //*************************************************************************
    TEST(test_countl_zero_8)
    {
      for (size_t i = 0; i < 256; ++i)
      {
        uint8_t value = uint8_t(i);

        CHECK_EQUAL(int(test_leading_zeros(value)), int(etl::countl_zero(value)));

        if (test_leading_zeros(value) != etl::countl_zero(value))
        {
          break;
        }
      }
    }

#if ETL_USING_CPP14
    //*************************************************************************
    TEST(test_countl_zero_8_constexpr)
    {
      constexpr size_t Size = etl::countl_zero(uint8_t(0x01U));
      char temp[Size];

      CHECK_EQUAL(test_leading_zeros(uint8_t(0x01U)), sizeof(temp));
    }
#endif

    //*************************************************************************
    TEST(test_countl_zero_16)
    {
      for (size_t i = 0; i < 65536; ++i)
      {
        uint16_t value = uint16_t(i);

        CHECK_EQUAL(int(test_leading_zeros(value)), int(etl::countl_zero(value)));

        if (test_leading_zeros(value) != etl::countl_zero(value))
        {
          break;
        }
      }
    }

#if ETL_USING_CPP14
    //*************************************************************************
    TEST(test_countl_zero_16_constexpr)
    {
      constexpr size_t Size = etl::countl_zero(uint16_t(0x0800U));
      char temp[Size];

      CHECK_EQUAL(test_leading_zeros(uint16_t(0x0800U)), sizeof(temp));
    }
#endif

    //*************************************************************************
    TEST(test_countl_zero_32)
    {
      etl::fnv_1a_32 hash;

      for (size_t i = 0; i < 100000; ++i)
      {
        hash.add(1);

        uint32_t value = hash.value();

        CHECK_EQUAL(int(test_leading_zeros(value)), int(etl::countl_zero(value)));

        if (test_leading_zeros(value) != etl::countl_zero(value))
        {
          break;
        }
      }
    }

#if ETL_USING_CPP14
    //*************************************************************************
    TEST(test_countl_zero_32_constexpr)
    {
      constexpr size_t Size = etl::countl_zero(uint32_t(0x08000000U));
      char temp[Size];

      CHECK_EQUAL(test_leading_zeros(uint32_t(0x08000000U)), sizeof(temp));
    }
#endif

    //*************************************************************************
    TEST(test_countl_zero_64)
    {
      etl::fnv_1a_64 hash;

      for (size_t i = 0; i < 100000; ++i)
      {
        hash.add(1);

        uint64_t value = hash.value();

        CHECK_EQUAL(int(test_leading_zeros(value)), int(etl::countl_zero(value)));

        if (test_leading_zeros(value) != etl::countl_zero(value))
        {
          break;
        }
      }
    }

#if ETL_USING_CPP14
    //*************************************************************************
    TEST(test_countl_zero_64_constexpr)
    {
      constexpr size_t Size = etl::countl_zero(uint64_t(0x0800000000000000U));
      char temp[Size];

      CHECK_EQUAL(test_leading_zeros(uint64_t(0x0800000000000000U)), sizeof(temp));
    }
#endif

    //*************************************************************************
    TEST(test_countl_one_8)
    {
      for (size_t i = 0; i < 256; ++i)
      {
        uint8_t value = uint8_t(i);

        CHECK_EQUAL(int(test_leading_ones(value)), int(etl::countl_one(value)));

        if (test_leading_ones(value) != etl::countl_one(value))
        {
          break;
        }
      }
    }

#if ETL_USING_CPP14
    //*************************************************************************
    TEST(test_countl_one_8_constexpr)
    {
      constexpr size_t Size = etl::countl_one(uint8_t(0xF0U));
      char temp[Size];

      CHECK_EQUAL(test_leading_ones(uint8_t(0xF0U)), sizeof(temp));
    }
#endif

    //*************************************************************************
    TEST(test_countl_one_16)
    {
      for (size_t i = 0; i < 65536; ++i)
      {
        uint16_t value = uint16_t(i);

        CHECK_EQUAL(int(test_leading_ones(value)), int(etl::countl_one(value)));

        if (test_leading_ones(value) != etl::countl_one(value))
        {
          break;
        }
      }
    }

#if ETL_USING_CPP14
    //*************************************************************************
    TEST(test_countl_one_16_constexpr)
    {
      constexpr size_t Size = etl::countl_one(uint16_t(0xF000U));
      char temp[Size];

      CHECK_EQUAL(test_leading_ones(uint16_t(0xF000U)), sizeof(temp));
    }
#endif

    //*************************************************************************
    TEST(test_countl_one_32)
    {
      etl::fnv_1a_32 hash;

      for (size_t i = 0; i < 100000; ++i)
      {
        hash.add(1);

        uint32_t value = hash.value();

        CHECK_EQUAL(int(test_leading_ones(value)), int(etl::countl_one(value)));

        if (test_leading_ones(value) != etl::countl_one(value))
        {
          break;
        }
      }
    }

#if ETL_USING_CPP14
    //*************************************************************************
    TEST(test_countl_one_32_constexpr)
    {
      constexpr size_t Size = etl::countl_one(uint32_t(0xF0000000UL));
      char temp[Size];

      CHECK_EQUAL(test_leading_ones(uint32_t(0xF0000000UL)), sizeof(temp));
    }
#endif

    //*************************************************************************
    TEST(test_countl_one_64)
    {
      etl::fnv_1a_64 hash;

      for (size_t i = 0; i < 100000; ++i)
      {
        hash.add(1);

        uint64_t value = hash.value();

        CHECK_EQUAL(int(test_leading_ones(value)), int(etl::countl_one(value)));

        if (test_leading_ones(value) != etl::countl_one(value))
        {
          break;
        }
      }
    }

#if ETL_USING_CPP14
    //*************************************************************************
    TEST(test_countl_one_64_constexpr)
    {
      constexpr size_t Size = etl::countl_one(uint64_t(0xF000000000000000UL));
      char temp[Size];

      CHECK_EQUAL(test_leading_ones(uint64_t(0xF000000000000000UL)), sizeof(temp));
    }
#endif

    //*************************************************************************
    TEST(test_bit_ceil_8)
    {
      for (size_t i = 0; i < 128; ++i)
      {
        uint8_t value = uint8_t(i);

        CHECK_EQUAL(int(test_bit_ceil(value)), int(etl::bit_ceil(value)));
      }
    }

#if ETL_USING_CPP14
    //*************************************************************************
    TEST(test_bit_ceil_8_constexpr)
    {
      constexpr size_t Size = etl::bit_ceil(uint8_t(1));
      char temp[Size];

      CHECK_EQUAL(test_bit_ceil(uint8_t(1)), sizeof(temp));
    }
#endif

    //*************************************************************************
    TEST(test_bit_ceil_16)
    {
      etl::fnv_1a_32 hash;

      for (size_t i = 0; i < 100000; ++i)
      {
        hash.add(1);

        uint16_t value = hash.value() & 0x7FFF;

        CHECK_EQUAL(test_bit_ceil(value), etl::bit_ceil(value));

        if (test_bit_ceil(value) != etl::bit_ceil(value))
        {
          break;
        }
      }
    }

#if ETL_USING_CPP14
    //*************************************************************************
    TEST(test_bit_ceil_16_constexpr)
    {
      constexpr size_t Size = etl::bit_ceil(uint16_t(1));
      char temp[Size];

      CHECK_EQUAL(test_bit_ceil(uint16_t(1)), sizeof(temp));
    }
#endif

    //*************************************************************************
    TEST(test_bit_ceil_32)
    {
      etl::fnv_1a_32 hash;

      for (size_t i = 0; i < 100000; ++i)
      {
        hash.add(1);

        uint32_t value = hash.value() & 0x7FFFFFFF;

        CHECK_EQUAL(test_bit_ceil(value), etl::bit_ceil(value));

        if (test_bit_ceil(value) != etl::bit_ceil(value))
        {
          break;
        }
      }
    }

#if ETL_USING_CPP14
    //*************************************************************************
    TEST(test_bit_ceil_32_constexpr)
    {
      constexpr size_t Size = etl::bit_ceil(uint32_t(1));
      char temp[Size];

      CHECK_EQUAL(test_bit_ceil(uint32_t(1)), sizeof(temp));
    }
#endif

    //*************************************************************************
    TEST(test_bit_ceil_64)
    {
      etl::fnv_1a_64 hash;

      for (size_t i = 0; i < 100000; ++i)
      {
        hash.add(1);

        uint64_t value = hash.value() & 0x7FFFFFFFFFFFFFFF;

        CHECK_EQUAL(test_bit_ceil(value), etl::bit_ceil(value));

        if (test_bit_ceil(value) != etl::bit_ceil(value))
        {
          break;
        }
      }
    }

#if ETL_USING_CPP14
    //*************************************************************************
    TEST(test_bit_ceil_64_constexpr)
    {
      constexpr uint64_t Size = etl::bit_ceil(uint64_t(1));
      char temp[Size];

      CHECK_EQUAL(test_bit_ceil(uint64_t(1)), sizeof(temp));
    }
#endif

    //*************************************************************************
    TEST(test_bit_floor_8)
    {
      for (size_t i = 0; i < 256; ++i)
      {
        uint8_t value = uint8_t(i);

        CHECK_EQUAL(int(test_bit_floor(value)), int(etl::bit_floor(value)));
      }
    }

    //*************************************************************************
    TEST(test_bit_floor_16)
    {
      etl::fnv_1a_32 hash;

      for (size_t i = 0; i < 100000; ++i)
      {
        hash.add(1);

        uint16_t value = hash.value();

        CHECK_EQUAL(test_bit_floor(value), etl::bit_floor(value));

        if (test_bit_floor(value) != etl::bit_floor(value))
        {
          break;
        }
      }
    }

#if ETL_USING_CPP14
    //*************************************************************************
    TEST(test_bit_floor_16_constexpr)
    {
      constexpr size_t Size = etl::bit_floor(uint16_t(1));
      char temp[Size];

      CHECK_EQUAL(test_bit_floor(uint16_t(1)), sizeof(temp));
    }
#endif

    //*************************************************************************
    TEST(test_bit_floor_32)
    {
      etl::fnv_1a_32 hash;

      for (size_t i = 0; i < 100000; ++i)
      {
        hash.add(1);

        uint32_t value = hash.value();

        CHECK_EQUAL(test_bit_floor(value), etl::bit_floor(value));

        if (test_bit_floor(value) != etl::bit_floor(value))
        {
          break;
        }
      }
    }

#if ETL_USING_CPP14
    //*************************************************************************
    TEST(test_bit_floor_32_constexpr)
    {
      constexpr size_t Size = etl::bit_floor(uint32_t(1));
      char temp[Size];

      CHECK_EQUAL(test_bit_floor(uint32_t(1)), sizeof(temp));
    }
#endif

    //*************************************************************************
    TEST(test_bit_floor_64)
    {
      etl::fnv_1a_64 hash;

      for (size_t i = 0; i < 100000; ++i)
      {
        hash.add(1);

        uint64_t value = hash.value();

        CHECK_EQUAL(test_bit_floor(value), etl::bit_floor(value));

        if (test_bit_floor(value) != etl::bit_floor(value))
        {
          break;
        }
      }
    }

#if ETL_USING_CPP14
    //*************************************************************************
    TEST(test_bit_floor_64_constexpr)
    {
      constexpr uint64_t Size = etl::bit_floor(uint64_t(1));
      char temp[Size];

      CHECK_EQUAL(test_bit_floor(uint64_t(1)), sizeof(temp));
    }
#endif

    //*************************************************************************
    TEST(test_bit_width_8)
    {
      for (size_t i = 0; i < 256; ++i)
      {
        uint8_t value = uint8_t(i);

        CHECK_EQUAL(int(test_bit_width(value)), int(etl::bit_width(value)));
      }
    }

#if ETL_USING_CPP14
    //*************************************************************************
    TEST(test_bit_width_8_constexpr)
    {
      constexpr size_t Size = etl::bit_width(uint8_t(0xAU));
      char temp[Size];

      CHECK_EQUAL(test_bit_width(uint8_t(0xAU)), sizeof(temp));
    }
#endif

    //*************************************************************************
    TEST(test_bit_width_16)
    {
      etl::fnv_1a_32 hash;

      for (size_t i = 0; i < 100000; ++i)
      {
        hash.add(1);

        uint16_t value = hash.value();

        CHECK_EQUAL(test_bit_width(value), etl::bit_width(value));
      }
    }

#if ETL_USING_CPP14
    //*************************************************************************
    TEST(test_bit_width_16_constexpr)
    {
      constexpr size_t Size = etl::bit_width(uint16_t(0xAU));
      char temp[Size];

      CHECK_EQUAL(test_bit_width(uint16_t(0xAU)), sizeof(temp));
    }
#endif

    //*************************************************************************
    TEST(test_bit_width_32)
    {
      etl::fnv_1a_32 hash;

      for (size_t i = 0; i < 100000; ++i)
      {
        hash.add(1);

        uint32_t value = hash.value();

        CHECK_EQUAL(test_bit_width(value), etl::bit_width(value));
      }
    }

#if ETL_USING_CPP14
    //*************************************************************************
    TEST(test_bit_width_32_constexpr)
    {
      constexpr size_t Size = etl::bit_width(uint32_t(0xAU));
      char temp[Size];

      CHECK_EQUAL(test_bit_width(uint32_t(0xAU)), sizeof(temp));
    }
#endif

    //*************************************************************************
    TEST(test_bit_width_64)
    {
      etl::fnv_1a_64 hash;

      for (size_t i = 0; i < 100000; ++i)
      {
        hash.add(1);

        uint64_t value = hash.value();

        CHECK_EQUAL(test_bit_width(value), etl::bit_width(value));
      }
    }

#if ETL_USING_CPP14
    //*************************************************************************
    TEST(test_bit_width_64_constexpr)
    {
      constexpr uint64_t Size = etl::bit_width(uint64_t(0xAU));
      char temp[Size];

      CHECK_EQUAL(test_bit_width(uint64_t(0xAU)), sizeof(temp));
    }
#endif
  };
}

#include "etl/private/diagnostic_pop.h"
