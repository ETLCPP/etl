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

#include "etl/byte.h"

namespace
{
  SUITE(test_byte)
  {
    //*************************************************************************
    TEST(test_byte_equality)
    {
      etl::byte b1 = etl::byte(0x5A);
      etl::byte b2 = etl::byte(0x5A);
      etl::byte b3 = etl::byte(0xA5);

      CHECK(b1 == b2);
      CHECK(b1 != b3);
    }

    //*************************************************************************
    TEST(test_byte_to_integer)
    {
      etl::byte b = etl::byte(0x5A);

      CHECK_EQUAL(0x5A,       etl::to_integer<char>(b));
      CHECK_EQUAL(0x005AU,    etl::to_integer<uint16_t>(b));
      CHECK_EQUAL(0x0000005A, etl::to_integer<int32_t>(b));
    }

    //*************************************************************************
    TEST(test_byte_shift_left)
    {
      unsigned char B = 0xB7;

      etl::byte b = etl::byte(B);

      CHECK_EQUAL((unsigned char)(B << 0U), etl::to_integer<unsigned char>(b << 0U));
      CHECK_EQUAL((unsigned char)(B << 1U), etl::to_integer<unsigned char>(b << 1U));
      CHECK_EQUAL((unsigned char)(B << 2U), etl::to_integer<unsigned char>(b << 2U));
      CHECK_EQUAL((unsigned char)(B << 3U), etl::to_integer<unsigned char>(b << 3U));
      CHECK_EQUAL((unsigned char)(B << 4U), etl::to_integer<unsigned char>(b << 4U));
      CHECK_EQUAL((unsigned char)(B << 5U), etl::to_integer<unsigned char>(b << 5U));
      CHECK_EQUAL((unsigned char)(B << 6U), etl::to_integer<unsigned char>(b << 6U));
      CHECK_EQUAL((unsigned char)(B << 7U), etl::to_integer<unsigned char>(b << 7U));
      CHECK_EQUAL((unsigned char)(B << 8U), etl::to_integer<unsigned char>(b << 8U));
    }

    //*************************************************************************
    TEST(test_byte_shift_left_equals)
    {
      unsigned char B = 0xB7;

      etl::byte b;

      b = etl::byte(B);
      CHECK_EQUAL((unsigned char)(B << 0U), etl::to_integer<unsigned char>(b <<= 0U));
      b = etl::byte(B);
      CHECK_EQUAL((unsigned char)(B << 1U), etl::to_integer<unsigned char>(b <<= 1U));
      b = etl::byte(B);
      CHECK_EQUAL((unsigned char)(B << 2U), etl::to_integer<unsigned char>(b <<= 2U));
      b = etl::byte(B);
      CHECK_EQUAL((unsigned char)(B << 3U), etl::to_integer<unsigned char>(b <<= 3U));
      b = etl::byte(B);
      CHECK_EQUAL((unsigned char)(B << 4U), etl::to_integer<unsigned char>(b <<= 4U));
      b = etl::byte(B);
      CHECK_EQUAL((unsigned char)(B << 5U), etl::to_integer<unsigned char>(b <<= 5U));
      b = etl::byte(B);
      CHECK_EQUAL((unsigned char)(B << 6U), etl::to_integer<unsigned char>(b <<= 6U));
      b = etl::byte(B);
      CHECK_EQUAL((unsigned char)(B << 7U), etl::to_integer<unsigned char>(b <<= 7U));
      b = etl::byte(B);
      CHECK_EQUAL((unsigned char)(B << 8U), etl::to_integer<unsigned char>(b <<= 8U));
    }

    //*************************************************************************
    TEST(test_byte_shift_right)
    {
      unsigned char B = 0xB7;

      etl::byte b = etl::byte(B);

      CHECK_EQUAL((unsigned char)(B >> 0U), etl::to_integer<unsigned char>(b >> 0U));
      CHECK_EQUAL((unsigned char)(B >> 1U), etl::to_integer<unsigned char>(b >> 1U));
      CHECK_EQUAL((unsigned char)(B >> 2U), etl::to_integer<unsigned char>(b >> 2U));
      CHECK_EQUAL((unsigned char)(B >> 3U), etl::to_integer<unsigned char>(b >> 3U));
      CHECK_EQUAL((unsigned char)(B >> 4U), etl::to_integer<unsigned char>(b >> 4U));
      CHECK_EQUAL((unsigned char)(B >> 5U), etl::to_integer<unsigned char>(b >> 5U));
      CHECK_EQUAL((unsigned char)(B >> 6U), etl::to_integer<unsigned char>(b >> 6U));
      CHECK_EQUAL((unsigned char)(B >> 7U), etl::to_integer<unsigned char>(b >> 7U));
    }
  }

  //*************************************************************************
  TEST(test_byte_shift_right_equals)
  {
    unsigned char B = 0xB7;

    etl::byte b;

    b = etl::byte(B);
    CHECK_EQUAL((unsigned char)(B >> 0U), etl::to_integer<unsigned char>(b >>= 0U));
    b = etl::byte(B);
    CHECK_EQUAL((unsigned char)(B >> 1U), etl::to_integer<unsigned char>(b >>= 1U));
    b = etl::byte(B);
    CHECK_EQUAL((unsigned char)(B >> 2U), etl::to_integer<unsigned char>(b >>= 2U));
    b = etl::byte(B);
    CHECK_EQUAL((unsigned char)(B >> 3U), etl::to_integer<unsigned char>(b >>= 3U));
    b = etl::byte(B);
    CHECK_EQUAL((unsigned char)(B >> 4U), etl::to_integer<unsigned char>(b >>= 4U));
    b = etl::byte(B);
    CHECK_EQUAL((unsigned char)(B >> 5U), etl::to_integer<unsigned char>(b >>= 5U));
    b = etl::byte(B);
    CHECK_EQUAL((unsigned char)(B >> 6U), etl::to_integer<unsigned char>(b >>= 6U));
    b = etl::byte(B);
    CHECK_EQUAL((unsigned char)(B >> 7U), etl::to_integer<unsigned char>(b >>= 7U));
  }

  //*************************************************************************
  TEST(test_byte_and)
  {
    unsigned char B = 0xB7;

    etl::byte b = etl::byte(B);

    for (unsigned int i = 0; i < 256; ++i)
    {
      CHECK_EQUAL((unsigned char)(B & i), etl::to_integer<unsigned char>(b & etl::byte(i)));
    }
  }

  //*************************************************************************
  TEST(test_byte_and_equals)
  {
    unsigned char B = 0xB7;

    etl::byte b;

    for (unsigned int i = 0; i < 256; ++i)
    {
      b = etl::byte(B);
      CHECK_EQUAL((unsigned char)(B & i), etl::to_integer<unsigned char>(b &= etl::byte(i)));
    }
  }

  //*************************************************************************
  TEST(test_byte_or)
  {
    unsigned char B = 0xB7;

    etl::byte b = etl::byte(B);

    for (unsigned int i = 0; i < 256; ++i)
    {
      CHECK_EQUAL((unsigned char)(B | i), etl::to_integer<unsigned char>(b | etl::byte(i)));
    }
  }

  //*************************************************************************
  TEST(test_byte_or_equals)
  {
    unsigned char B = 0xB7;

    etl::byte b;

    for (unsigned int i = 0; i < 256; ++i)
    {
      b = etl::byte(B);
      CHECK_EQUAL((unsigned char)(B | i), etl::to_integer<unsigned char>(b |= etl::byte(i)));
    }
  }

  //*************************************************************************
  TEST(test_byte_xor)
  {
    unsigned char B = 0xB7;

    etl::byte b = etl::byte(B);

    for (unsigned int i = 0; i < 256; ++i)
    {
      CHECK_EQUAL((unsigned char)(B ^ i), etl::to_integer<unsigned char>(b ^ etl::byte(i)));
    }
  }

  //*************************************************************************
  TEST(test_byte_xor_equals)
  {
    unsigned char B = 0xB7;

    etl::byte b;

    for (unsigned int i = 0; i < 256; ++i)
    {
      b = etl::byte(B);
      CHECK_EQUAL((unsigned char)(B ^ i), etl::to_integer<unsigned char>(b ^= etl::byte(i)));
    }
  }

  //*************************************************************************
  TEST(test_byte_not)
  {
    for (unsigned int i = 0; i < 256; ++i)
    {
      CHECK_EQUAL((unsigned char)(~i), etl::to_integer<unsigned char>(~etl::byte(i)));
    }
  }
}

