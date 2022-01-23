/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl
https://www.etlcpp.com

Copyright(c) 2022 jwellbelove

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

#include "etl/unaligned_type.h"
#include "etl/integral_limits.h"

namespace
{
  SUITE(test_unaligned_type)
  {
    //*************************************************************************
    TEST(test_copy_construction)
    {
      etl::le_uint32_t le_v1(0x01234567);
      etl::be_uint32_t be_v1(0x01234567);
      etl::le_uint32_t le_v2(le_v1); // Copy construct le from le.
      etl::be_uint32_t be_v2(be_v1); // Copy construct be from be.
      etl::le_uint32_t le_v3(be_v1); // Copy construct le from be.
      etl::be_uint32_t be_v3(le_v1); // Copy construct be from le.

      CHECK_EQUAL(int(0x01234567), int(le_v2));
      CHECK_EQUAL(int(0x01234567), int(be_v2));
      CHECK_EQUAL(int(0x01234567), int(le_v3));
      CHECK_EQUAL(int(0x01234567), int(be_v3));
    }

    //*************************************************************************
    TEST(test_endianness)
    {
      CHECK_EQUAL(etl::endian::big,    etl::be_int16_t::Endian);
      CHECK_EQUAL(etl::endian::little, etl::le_int16_t::Endian);
    }

    //*************************************************************************
    TEST(test_le_sizes)
    {
      CHECK_EQUAL(sizeof(char),               etl::le_char_t::Size);
      CHECK_EQUAL(sizeof(signed char),        etl::le_schar_t::Size);
      CHECK_EQUAL(sizeof(unsigned char),      etl::le_uchar_t::Size);
      CHECK_EQUAL(sizeof(short),              etl::le_short_t::Size);
      CHECK_EQUAL(sizeof(unsigned short),     etl::le_ushort_t::Size);
      CHECK_EQUAL(sizeof(int),                etl::le_int_t::Size);
      CHECK_EQUAL(sizeof(unsigned int),       etl::le_uint_t::Size);
      CHECK_EQUAL(sizeof(long),               etl::le_long_t::Size);
      CHECK_EQUAL(sizeof(unsigned long),      etl::le_ulong_t::Size);
      CHECK_EQUAL(sizeof(long long),          etl::le_long_long_t::Size);
      CHECK_EQUAL(sizeof(unsigned long long), etl::le_ulong_long_t::Size);
      CHECK_EQUAL(sizeof(float),              etl::le_float_t::Size);
      CHECK_EQUAL(sizeof(double),             etl::le_double_t::Size);
      CHECK_EQUAL(sizeof(long double),        etl::le_long_double_t::Size);

      CHECK_EQUAL(sizeof(int8_t),   etl::le_int8_t::Size);
      CHECK_EQUAL(sizeof(uint8_t),  etl::le_uint8_t::Size);
      CHECK_EQUAL(sizeof(int16_t),  etl::le_int16_t::Size);
      CHECK_EQUAL(sizeof(uint16_t), etl::le_uint16_t::Size);
      CHECK_EQUAL(sizeof(int32_t),  etl::le_int32_t::Size);
      CHECK_EQUAL(sizeof(uint32_t), etl::le_uint32_t::Size);
      CHECK_EQUAL(sizeof(int64_t),  etl::le_int64_t::Size);
      CHECK_EQUAL(sizeof(uint64_t), etl::le_uint64_t::Size);
    }

    //*************************************************************************
    TEST(test_be_sizes)
    {
      CHECK_EQUAL(sizeof(char),               etl::be_char_t::Size);
      CHECK_EQUAL(sizeof(signed char),        etl::be_schar_t::Size);
      CHECK_EQUAL(sizeof(unsigned char),      etl::be_uchar_t::Size);
      CHECK_EQUAL(sizeof(short),              etl::be_short_t::Size);
      CHECK_EQUAL(sizeof(unsigned short),     etl::be_ushort_t::Size);
      CHECK_EQUAL(sizeof(int),                etl::be_int_t::Size);
      CHECK_EQUAL(sizeof(unsigned int),       etl::be_uint_t::Size);
      CHECK_EQUAL(sizeof(long),               etl::be_long_t::Size);
      CHECK_EQUAL(sizeof(unsigned long),      etl::be_ulong_t::Size);
      CHECK_EQUAL(sizeof(long long),          etl::be_long_long_t::Size);
      CHECK_EQUAL(sizeof(unsigned long long), etl::be_ulong_long_t::Size);
      CHECK_EQUAL(sizeof(float),              etl::be_float_t::Size);
      CHECK_EQUAL(sizeof(double),             etl::be_double_t::Size);
      CHECK_EQUAL(sizeof(long double),        etl::be_long_double_t::Size);

      CHECK_EQUAL(sizeof(int8_t),   etl::be_int8_t::Size);
      CHECK_EQUAL(sizeof(uint8_t),  etl::be_uint8_t::Size);
      CHECK_EQUAL(sizeof(int16_t),  etl::be_int16_t::Size);
      CHECK_EQUAL(sizeof(uint16_t), etl::be_uint16_t::Size);
      CHECK_EQUAL(sizeof(int32_t),  etl::be_int32_t::Size);
      CHECK_EQUAL(sizeof(uint32_t), etl::be_uint32_t::Size);
      CHECK_EQUAL(sizeof(int64_t),  etl::be_int64_t::Size);
      CHECK_EQUAL(sizeof(uint64_t), etl::be_uint64_t::Size);
    }

    //*************************************************************************
    TEST(test_net_sizes)
    {
      CHECK_EQUAL(sizeof(char),               etl::net_char_t::Size);
      CHECK_EQUAL(sizeof(signed char),        etl::net_schar_t::Size);
      CHECK_EQUAL(sizeof(unsigned char),      etl::net_uchar_t::Size);
      CHECK_EQUAL(sizeof(short),              etl::net_short_t::Size);
      CHECK_EQUAL(sizeof(unsigned short),     etl::net_ushort_t::Size);
      CHECK_EQUAL(sizeof(int),                etl::net_int_t::Size);
      CHECK_EQUAL(sizeof(unsigned int),       etl::net_uint_t::Size);
      CHECK_EQUAL(sizeof(long),               etl::net_long_t::Size);
      CHECK_EQUAL(sizeof(unsigned long),      etl::net_ulong_t::Size);
      CHECK_EQUAL(sizeof(long long),          etl::net_long_long_t::Size);
      CHECK_EQUAL(sizeof(unsigned long long), etl::net_ulong_long_t::Size);
      CHECK_EQUAL(sizeof(float),              etl::net_float_t::Size);
      CHECK_EQUAL(sizeof(double),             etl::net_double_t::Size);
      CHECK_EQUAL(sizeof(long double),        etl::net_long_double_t::Size);

      CHECK_EQUAL(sizeof(int8_t),   etl::net_int8_t::Size);
      CHECK_EQUAL(sizeof(uint8_t),  etl::net_uint8_t::Size);
      CHECK_EQUAL(sizeof(int16_t),  etl::net_int16_t::Size);
      CHECK_EQUAL(sizeof(uint16_t), etl::net_uint16_t::Size);
      CHECK_EQUAL(sizeof(int32_t),  etl::net_int32_t::Size);
      CHECK_EQUAL(sizeof(uint32_t), etl::net_uint32_t::Size);
      CHECK_EQUAL(sizeof(int64_t),  etl::net_int64_t::Size);
      CHECK_EQUAL(sizeof(uint64_t), etl::net_uint64_t::Size);
    }

#if ETL_ENDIANNESS_IS_CONSTEXPR
    //*************************************************************************
    TEST(test_host_sizes)
    {
      CHECK_EQUAL(sizeof(char),               etl::host_char_t::Size);
      CHECK_EQUAL(sizeof(signed char),        etl::host_schar_t::Size);
      CHECK_EQUAL(sizeof(unsigned char),      etl::host_uchar_t::Size);
      CHECK_EQUAL(sizeof(short),              etl::host_short_t::Size);
      CHECK_EQUAL(sizeof(unsigned short),     etl::host_ushort_t::Size);
      CHECK_EQUAL(sizeof(int),                etl::host_int_t::Size);
      CHECK_EQUAL(sizeof(unsigned int),       etl::host_uint_t::Size);
      CHECK_EQUAL(sizeof(long),               etl::host_long_t::Size);
      CHECK_EQUAL(sizeof(unsigned long),      etl::host_ulong_t::Size);
      CHECK_EQUAL(sizeof(long long),          etl::host_long_long_t::Size);
      CHECK_EQUAL(sizeof(unsigned long long), etl::host_ulong_long_t::Size);
      CHECK_EQUAL(sizeof(float),              etl::host_float_t::Size);
      CHECK_EQUAL(sizeof(double),             etl::host_double_t::Size);
      CHECK_EQUAL(sizeof(long double),        etl::host_long_double_t::Size);

      CHECK_EQUAL(sizeof(int8_t),   etl::host_int8_t::Size);
      CHECK_EQUAL(sizeof(uint8_t),  etl::host_uint8_t::Size);
      CHECK_EQUAL(sizeof(int16_t),  etl::host_int16_t::Size);
      CHECK_EQUAL(sizeof(uint16_t), etl::host_uint16_t::Size);
      CHECK_EQUAL(sizeof(int32_t),  etl::host_int32_t::Size);
      CHECK_EQUAL(sizeof(uint32_t), etl::host_uint32_t::Size);
      CHECK_EQUAL(sizeof(int64_t),  etl::host_int64_t::Size);
      CHECK_EQUAL(sizeof(uint64_t), etl::host_uint64_t::Size);
    }
#endif

    //*************************************************************************
    TEST(test_le_equality)
    {
      // char
      CHECK(etl::le_char_t(0x01) == etl::le_char_t(0x01));
      CHECK(char(0x01)           == etl::le_char_t(0x01));
      CHECK(etl::le_char_t(0x01) == char(0x01));

      CHECK(etl::le_schar_t(0x01) == etl::le_schar_t(0x01));
      CHECK((signed char)(0x01)   == etl::le_schar_t(0x01));
      CHECK(etl::le_schar_t(0x01) == (signed char)(0x01));

      CHECK(etl::le_uchar_t(0x01U) == etl::le_uchar_t(0x01U));
      CHECK((unsigned char)(0x01U) == etl::le_uchar_t(0x01U));
      CHECK(etl::le_uchar_t(0x01U) == (unsigned char)(0x01U));

      // short
      CHECK(etl::le_short_t(0x0123) == etl::le_short_t(0x0123));
      CHECK(short(0x0123)           == etl::le_short_t(0x0123));
      CHECK(etl::le_short_t(0x0123) == short(0x0123));

      CHECK(etl::le_ushort_t(0x0123) == etl::le_ushort_t(0x0123));
      CHECK((unsigned short)(0x0123) == etl::le_ushort_t(0x0123));
      CHECK(etl::le_ushort_t(0x0123) == (unsigned short)(0x0123));

      // int
      CHECK(etl::le_int_t(0x012345678) == etl::le_int_t(0x012345678));
      CHECK(int(0x012345678)           == etl::le_int_t(0x012345678));
      CHECK(etl::le_int_t(0x012345678) == int(0x012345678));

      CHECK(etl::le_uint_t(0x012345678U) == etl::le_uint_t(0x012345678U));
      CHECK((unsigned int)(0x012345678U) == etl::le_uint_t(0x012345678U));
      CHECK(etl::le_uint_t(0x012345678U) == (unsigned int)(0x012345678U));

      if (sizeof(long) == 4U)
      {
        // long
        CHECK(etl::le_long_t(0x012345678) == etl::le_long_t(0x012345678));
        CHECK(long(0x012345678)           == etl::le_long_t(0x012345678));
        CHECK(etl::le_long_t(0x012345678) == long(0x012345678));

        CHECK(etl::le_ulong_t(0x012345678U) == etl::le_ulong_t(0x012345678U));
        CHECK((unsigned long)(0x012345678U) == etl::le_ulong_t(0x012345678U));
        CHECK(etl::le_ulong_t(0x012345678U) == (unsigned long)(0x012345678U));
      }

      if (sizeof(long long) == 8U)
      {
        // long long
        CHECK(etl::le_long_long_t(0x0123456789ABCDEF) == etl::le_long_long_t(0x0123456789ABCDEF));
        CHECK((long long)(0x0123456789ABCDEF)         == etl::le_long_long_t(0x0123456789ABCDEF));
        CHECK(etl::le_long_long_t(0x0123456789ABCDEF) == (long long)(0x0123456789ABCDEF));

        CHECK(etl::le_ulong_long_t(0x0123456789ABCDEFU) == etl::le_ulong_long_t(0x0123456789ABCDEFU));
        CHECK((unsigned long long)(0x0123456789ABCDEFU) == etl::le_ulong_long_t(0x0123456789ABCDEFU));
        CHECK(etl::le_ulong_long_t(0x0123456789ABCDEFU) == (unsigned long long)(0x0123456789ABCDEFU));
      }
    }

    //*************************************************************************
    TEST(test_le_inequality)
    {
      // char
      CHECK(etl::le_char_t(0x01) != etl::le_char_t(0x02));
      CHECK(char(0x01)           != etl::le_char_t(0x02));
      CHECK(etl::le_char_t(0x01) != char(0x02));

      CHECK(etl::le_schar_t(0x01) != etl::le_schar_t(0x02));
      CHECK((signed char)(0x01)   != etl::le_schar_t(0x02));
      CHECK(etl::le_schar_t(0x01) != (signed char)(0x02));

      CHECK(etl::le_uchar_t(0x01U) != etl::le_uchar_t(0x02U));
      CHECK((unsigned char)(0x01U) != etl::le_uchar_t(0x02U));
      CHECK(etl::le_uchar_t(0x01U) != (unsigned char)(0x02U));

      // short
      CHECK(etl::le_short_t(0x0123) != etl::le_short_t(0x0223));
      CHECK(short(0x0123)           != etl::le_short_t(0x0223));
      CHECK(etl::le_short_t(0x0123) != short(0x0223));

      CHECK(etl::le_ushort_t(0x0123) != etl::le_ushort_t(0x0223));
      CHECK((unsigned short)(0x0123) != etl::le_ushort_t(0x0223));
      CHECK(etl::le_ushort_t(0x0123) != (unsigned short)(0x0223));

      // int
      CHECK(etl::le_int_t(0x012345678) != etl::le_int_t(0x022345678));
      CHECK(int(0x012345678)           != etl::le_int_t(0x022345678));
      CHECK(etl::le_int_t(0x012345678) != int(0x022345678));

      CHECK(etl::le_uint_t(0x012345678U) != etl::le_uint_t(0x022345678U));
      CHECK((unsigned int)(0x012345678U) != etl::le_uint_t(0x022345678U));
      CHECK(etl::le_uint_t(0x012345678U) != (unsigned int)(0x022345678U));

      if (sizeof(long) == 4U)
      {
        // long
        CHECK(etl::le_long_t(0x012345678) != etl::le_long_t(0x022345678));
        CHECK(long(0x012345678)           != etl::le_long_t(0x022345678));
        CHECK(etl::le_long_t(0x012345678) != long(0x022345678));

        CHECK(etl::le_ulong_t(0x012345678U) != etl::le_ulong_t(0x022345678U));
        CHECK((unsigned long)(0x012345678U) != etl::le_ulong_t(0x022345678U));
        CHECK(etl::le_ulong_t(0x012345678U) != (unsigned long)(0x022345678U));
      }

      if (sizeof(long long) == 8U)
      {
        // long long
        CHECK(etl::le_long_long_t(0x0123456789ABCDEF) != etl::le_long_long_t(0x0223456789ABCDEF));
        CHECK((long long)(0x0123456789ABCDEF)           != etl::le_long_long_t(0x0223456789ABCDEF));
        CHECK(etl::le_long_long_t(0x0123456789ABCDEF) != (long long)(0x0223456789ABCDEF));

        CHECK(etl::le_ulong_long_t(0x0123456789ABCDEFU) != etl::le_ulong_long_t(0x0223456789ABCDEFU));
        CHECK((unsigned long long)(0x0123456789ABCDEFU) != etl::le_ulong_long_t(0x0223456789ABCDEFU));
        CHECK(etl::le_ulong_long_t(0x0123456789ABCDEFU) != (unsigned long long)(0x0223456789ABCDEFU));
      }
    }

    //*************************************************************************
    TEST(test_be_equality)
    {
      // char
      CHECK(etl::be_char_t(0x01) == etl::be_char_t(0x01));
      CHECK(char(0x01)           == etl::be_char_t(0x01));
      CHECK(etl::be_char_t(0x01) == char(0x01));

      CHECK(etl::be_schar_t(0x01) == etl::be_schar_t(0x01));
      CHECK((signed char)(0x01)   == etl::be_schar_t(0x01));
      CHECK(etl::be_schar_t(0x01) == (signed char)(0x01));

      CHECK(etl::be_uchar_t(0x01U) == etl::be_uchar_t(0x01U));
      CHECK((unsigned char)(0x01U) == etl::be_uchar_t(0x01U));
      CHECK(etl::be_uchar_t(0x01U) == (unsigned char)(0x01U));

      // short
      CHECK(etl::be_short_t(0x0123) == etl::be_short_t(0x0123));
      CHECK(short(0x0123)           == etl::be_short_t(0x0123));
      CHECK(etl::be_short_t(0x0123) == short(0x0123));

      CHECK(etl::be_ushort_t(0x0123) == etl::be_ushort_t(0x0123));
      CHECK((unsigned short)(0x0123) == etl::be_ushort_t(0x0123));
      CHECK(etl::be_ushort_t(0x0123) == (unsigned short)(0x0123));

      // int
      CHECK(etl::be_int_t(0x012345678) == etl::be_int_t(0x012345678));
      CHECK(int(0x012345678)           == etl::be_int_t(0x012345678));
      CHECK(etl::be_int_t(0x012345678) == int(0x012345678));

      CHECK(etl::be_uint_t(0x012345678U) == etl::be_uint_t(0x012345678U));
      CHECK((unsigned int)(0x012345678U) == etl::be_uint_t(0x012345678U));
      CHECK(etl::be_uint_t(0x012345678U) == (unsigned int)(0x012345678U));

      if (sizeof(long) == 4U)
      {
        // long
        CHECK(etl::be_long_t(0x012345678) == etl::be_long_t(0x012345678));
        CHECK(long(0x012345678)           == etl::be_long_t(0x012345678));
        CHECK(etl::be_long_t(0x012345678) == long(0x012345678));

        CHECK(etl::be_ulong_t(0x012345678U) == etl::be_ulong_t(0x012345678U));
        CHECK((unsigned long)(0x012345678U) == etl::be_ulong_t(0x012345678U));
        CHECK(etl::be_ulong_t(0x012345678U) == (unsigned long)(0x012345678U));
      }

      if (sizeof(long long) == 8U)
      {
        // long long
        CHECK(etl::be_long_long_t(0x0123456789ABCDEF) == etl::be_long_long_t(0x0123456789ABCDEF));
        CHECK((long long)(0x0123456789ABCDEF)           == etl::be_long_long_t(0x0123456789ABCDEF));
        CHECK(etl::be_long_long_t(0x0123456789ABCDEF) == (long long)(0x0123456789ABCDEF));

        CHECK(etl::be_ulong_long_t(0x0123456789ABCDEFU) == etl::be_ulong_long_t(0x0123456789ABCDEFU));
        CHECK((unsigned long long)(0x0123456789ABCDEFU) == etl::be_ulong_long_t(0x0123456789ABCDEFU));
        CHECK(etl::be_ulong_long_t(0x0123456789ABCDEFU) == (unsigned long long)(0x0123456789ABCDEFU));
      }
    }

    //*************************************************************************
    TEST(test_be_inequality)
    {
      // char
      CHECK(etl::be_char_t(0x01) != etl::be_char_t(0x02));
      CHECK(char(0x01)           != etl::be_char_t(0x02));
      CHECK(etl::be_char_t(0x01) != char(0x02));

      CHECK(etl::be_schar_t(0x01) != etl::be_schar_t(0x02));
      CHECK((signed char)(0x01)   != etl::be_schar_t(0x02));
      CHECK(etl::be_schar_t(0x01) != (signed char)(0x02));

      CHECK(etl::be_uchar_t(0x01U) != etl::be_uchar_t(0x02U));
      CHECK((unsigned char)(0x01U) != etl::be_uchar_t(0x02U));
      CHECK(etl::be_uchar_t(0x01U) != (unsigned char)(0x02U));

      // short
      CHECK(etl::be_short_t(0x0123) != etl::be_short_t(0x0223));
      CHECK(short(0x0123)           != etl::be_short_t(0x0223));
      CHECK(etl::be_short_t(0x0123) != short(0x0223));

      CHECK(etl::be_ushort_t(0x0123) != etl::be_ushort_t(0x0223));
      CHECK((unsigned short)(0x0123) != etl::be_ushort_t(0x0223));
      CHECK(etl::be_ushort_t(0x0123) != (unsigned short)(0x0223));

      // int
      CHECK(etl::be_int_t(0x012345678) != etl::be_int_t(0x022345678));
      CHECK(int(0x012345678)           != etl::be_int_t(0x022345678));
      CHECK(etl::be_int_t(0x012345678) != int(0x022345678));

      CHECK(etl::be_uint_t(0x012345678U) != etl::be_uint_t(0x022345678U));
      CHECK((unsigned int)(0x012345678U) != etl::be_uint_t(0x022345678U));
      CHECK(etl::be_uint_t(0x012345678U) != (unsigned int)(0x022345678U));

      if (sizeof(long) == 4U)
      {
        // long
        CHECK(etl::be_long_t(0x012345678) != etl::be_long_t(0x022345678));
        CHECK(long(0x012345678)           != etl::be_long_t(0x022345678));
        CHECK(etl::be_long_t(0x012345678) != long(0x022345678));

        CHECK(etl::be_ulong_t(0x012345678U) != etl::be_ulong_t(0x022345678U));
        CHECK((unsigned long)(0x012345678U) != etl::be_ulong_t(0x022345678U));
        CHECK(etl::be_ulong_t(0x012345678U) != (unsigned long)(0x022345678U));
      }

      if (sizeof(long long) == 8U)
      {
        // long long
        CHECK(etl::be_long_long_t(0x0123456789ABCDEF) != etl::be_long_long_t(0x0223456789ABCDEF));
        CHECK((long long)(0x0123456789ABCDEF)           != etl::be_long_long_t(0x0223456789ABCDEF));
        CHECK(etl::be_long_long_t(0x0123456789ABCDEF) != (long long)(0x0223456789ABCDEF));

        CHECK(etl::be_ulong_long_t(0x0123456789ABCDEFU) != etl::be_ulong_long_t(0x0223456789ABCDEFU));
        CHECK((unsigned long long)(0x0123456789ABCDEFU) != etl::be_ulong_long_t(0x0223456789ABCDEFU));
        CHECK(etl::be_ulong_long_t(0x0123456789ABCDEFU) != (unsigned long long)(0x0223456789ABCDEFU));
      }
    }

    //*************************************************************************
    TEST(test_le_assignment)
    {
      etl::le_char_t le_char;
      le_char = 0x01;
      CHECK(0x01 == (char)le_char);

      etl::le_schar_t le_schar;
      le_schar = 0x01;
      CHECK(0x01 == (signed char)le_schar);

      etl::le_uchar_t le_uchar;
      le_uchar = 0x01;
      CHECK(0x01 == (unsigned char)le_uchar);

      etl::le_short_t le_short;
      le_short = 0x0123;
      CHECK(0x0123 == (short)le_short);

      etl::le_ushort_t le_ushort;
      le_ushort = 0x0123;
      CHECK(0x0123 == (unsigned short)le_ushort);

      etl::le_int_t le_int;
      le_int = 0x01234567;
      CHECK(0x01234567 == (int)le_int);

      etl::le_uint_t le_uint;
      le_uint = 0x01234567;
      CHECK(0x01234567 == (unsigned int)le_uint);

      if (sizeof(long) == 4U)
      {
        etl::le_long_t le_long;
        le_long = 0x01234567;
        CHECK(0x01234567 == (long)le_long);

        etl::le_ulong_t le_ulong;
        le_ulong = 0x01234567;
        CHECK(0x01234567 == (unsigned long)le_ulong);
      }

      if (sizeof(long long) == 8U)
      {
        etl::le_long_long_t le_long;
        le_long = 0x0123456789ABCDEF;
        CHECK(0x0123456789ABCDEF == (long long)le_long);

        etl::le_ulong_long_t le_ulong;
        le_ulong = 0x0123456789ABCDEF;
        CHECK(0x0123456789ABCDEF == (unsigned long long)le_ulong);
      }
    }

    //*************************************************************************
    TEST(test_be_assignment)
    {
      etl::le_char_t be_char;
      be_char = 0x01;
      CHECK(0x01 == (char)be_char);

      etl::be_schar_t be_schar;
      be_schar = 0x01;
      CHECK(0x01 == (signed char)be_schar);

      etl::be_uchar_t be_uchar;
      be_uchar = 0x01;
      CHECK(0x01 == (unsigned char)be_uchar);

      etl::be_short_t be_short;
      be_short = 0x0123;
      CHECK(0x0123 == (short)be_short);

      etl::be_ushort_t be_ushort;
      be_ushort = 0x0123;
      CHECK(0x0123 == (unsigned short)be_ushort);

      etl::be_int_t be_int;
      be_int = 0x01234567;
      CHECK(0x01234567 == (int)be_int);

      etl::be_uint_t be_uint;
      be_uint = 0x01234567;
      CHECK(0x01234567 == (unsigned int)be_uint);

      if (sizeof(long) == 4U)
      {
        etl::be_long_t be_long;
        be_long = 0x01234567;
        CHECK(0x01234567 == (long)be_long);

        etl::be_ulong_t be_ulong;
        be_ulong = 0x01234567;
        CHECK(0x01234567 == (unsigned long)be_ulong);
      }

      if (sizeof(long long) == 8U)
      {
        etl::be_long_long_t be_long;
        be_long = 0x0123456789ABCDEF;
        CHECK(0x0123456789ABCDEF == (long long)be_long);

        etl::be_ulong_long_t be_ulong;
        be_ulong = 0x0123456789ABCDEF;
        CHECK(0x0123456789ABCDEF == (unsigned long long)be_ulong);
      }
    }

    //*************************************************************************
    TEST(test_cross_assignment)
    {
      etl::le_uint32_t le_v1(0x01234567);
      etl::be_uint32_t be_v1(0x01234567);
      etl::le_uint32_t le_v2;
      etl::be_uint32_t be_v2;
      etl::le_uint32_t le_v3;
      etl::be_uint32_t be_v3;

      le_v2 = le_v1; // Assign le from le.
      be_v2 = be_v1; // Assign be from be.
      le_v3 = be_v1; // Assign le from be.
      be_v3 = le_v1; // Assign be from le.

      CHECK_EQUAL(int(0x01234567), int(le_v2));
      CHECK_EQUAL(int(0x01234567), int(be_v2));
      CHECK_EQUAL(int(0x01234567), int(le_v3));
      CHECK_EQUAL(int(0x01234567), int(be_v3));
    }

    //*************************************************************************
    TEST(test_data)
    {
      etl::be_uint16_t test(0x1234);
      
      CHECK_EQUAL(0x12, *test.data());
      CHECK_EQUAL(0x34, *(test.data() + 1U));
    }

    //*************************************************************************
    TEST(test_begin_end_iterator)
    {
      using iterator       = etl::be_uint16_t::iterator;
      using const_iterator = etl::be_uint16_t::const_iterator;

      etl::be_uint16_t       test(0x1234);
      const etl::be_uint16_t const_test(0x1234);

      iterator itr;
      const_iterator citr;

      //*******************************
      itr = test.begin();
      CHECK_EQUAL(int(0x12), int(*itr));
      ++itr;
      CHECK_EQUAL(int(0x34), int(*itr));
      ++itr;
      CHECK(itr == test.end());

      //*******************************
      itr = test.begin();
      *itr = 0x34;
      CHECK_EQUAL(int(0x34), int(*itr));
      ++itr;
      *itr = 0x12;
      CHECK_EQUAL(int(0x12), int(*itr));
      ++itr;
      CHECK(itr == test.end());

      //*******************************
      citr = const_test.begin();
      CHECK_EQUAL(int(0x12), int(*citr));
      ++citr;
      CHECK_EQUAL(int(0x34), int(*citr));
      ++citr;
      CHECK(citr == const_test.end());

      //*******************************
      citr = const_test.cbegin();
      CHECK_EQUAL(int(0x12), int(*citr));
      ++citr;
      CHECK_EQUAL(int(0x34), int(*citr));
      ++citr;
      CHECK(citr == const_test.cend());
    }

    //*************************************************************************
    TEST(test_rbegin_rend_iterator)
    {
      using reverse_iterator       = etl::be_uint16_t::reverse_iterator;
      using const_reverse_iterator = etl::be_uint16_t::const_reverse_iterator;

      etl::be_uint16_t       test(0x1234);
      const etl::be_uint16_t const_test(0x1234);

      reverse_iterator       itr;
      const_reverse_iterator citr;

      //*******************************
      itr = test.rbegin();
      CHECK_EQUAL(int(0x34), int(*itr));
      ++itr;
      CHECK_EQUAL(int(0x12), int(*itr));
      ++itr;
      CHECK(itr == test.rend());

      //*******************************
      itr = test.rbegin();
      *itr = 0x12;
      CHECK_EQUAL(int(0x12), int(*itr));
      ++itr;
      *itr = 0x34;
      CHECK_EQUAL(int(0x34), int(*itr));
      ++itr;
      CHECK(itr == test.rend());

      //*******************************
      citr = const_test.rbegin();
      CHECK_EQUAL(int(0x34), int(*citr));
      ++citr;
      CHECK_EQUAL(int(0x12), int(*citr));
      ++citr;
      CHECK(citr == const_test.rend());

      //*******************************
      citr = const_test.crbegin();
      CHECK_EQUAL(int(0x34), int(*citr));
      ++citr;
      CHECK_EQUAL(int(0x12), int(*citr));
      ++citr;
      CHECK(citr == const_test.crend());
    }

    //*************************************************************************
    TEST(test_index_operator)
    {
      etl::le_uint16_t       test_le(0x1234);
      const etl::le_uint16_t const_test_le(0x1234);

      etl::be_uint16_t       test_be(0x1234);
      const etl::be_uint16_t const_test_be(0x1234);

      CHECK_EQUAL(0x34, test_le[0]);
      CHECK_EQUAL(0x12, test_le[1]);

      test_le[0] = 0x56;
      test_le[1] = 0x78;
      CHECK_EQUAL(0x56, test_le[0]);
      CHECK_EQUAL(0x78, test_le[1]);

      CHECK_EQUAL(0x34, const_test_le[0]);
      CHECK_EQUAL(0x12, const_test_le[1]);

      CHECK_EQUAL(0x12, test_be[0]);
      CHECK_EQUAL(0x34, test_be[1]);

      test_be[0] = 0x56;
      test_be[1] = 0x78;
      CHECK_EQUAL(0x56, test_be[0]);
      CHECK_EQUAL(0x78, test_be[1]);

      CHECK_EQUAL(0x12, const_test_be[0]);
      CHECK_EQUAL(0x34, const_test_be[1]);
    }
  };
}
