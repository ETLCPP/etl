/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl
https://www.etlcpp.com

Copyright(c) 2022 John Wellbelove

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

#if ETL_USING_CPP14

namespace
{
  SUITE(test_unaligned_type_constexpr)
  {
    //*************************************************************************
    TEST(test_construction)
    {
      constexpr etl::le_uint32_t le_v1(0x01234567);
      constexpr etl::be_uint32_t be_v1(0x01234567);

      CHECK_EQUAL(int(0x01234567), int(le_v1));
      CHECK_EQUAL(int(0x01234567), int(be_v1));
    }

    //*************************************************************************
    TEST(test_copy_construction)
    {
      constexpr etl::le_uint32_t le_v1(0x01234567);
      constexpr etl::be_uint32_t be_v1(0x01234567);
      constexpr etl::le_uint32_t le_v2(le_v1); // Copy construct le from le.
      constexpr etl::be_uint32_t be_v2(be_v1); // Copy construct be from be.
      constexpr etl::le_uint32_t le_v3(be_v1); // Copy construct le from be.
      constexpr etl::be_uint32_t be_v3(le_v1); // Copy construct be from le.

      CHECK_EQUAL(int(0x01234567), int(le_v2));
      CHECK_EQUAL(int(0x01234567), int(be_v2));
      CHECK_EQUAL(int(0x01234567), int(le_v3));
      CHECK_EQUAL(int(0x01234567), int(be_v3));
    }

    //*************************************************************************
    TEST(test_le_equality)
    {
      constexpr bool check_char1 = etl::le_char_t(0x01) == etl::le_char_t(0x01);
      constexpr bool check_char2 = (char)(0x01)         == etl::le_char_t(0x01);
      constexpr bool check_char3 = etl::le_char_t(0x01) == (char)(0x01);

      CHECK_EQUAL(true, check_char1);
      CHECK(check_char2);
      CHECK(check_char3);

      constexpr bool check_schar1 = etl::le_schar_t(0x01) == etl::le_schar_t(0x01);
      constexpr bool check_schar2 = (signed char)(0x01)   == etl::le_schar_t(0x01);
      constexpr bool check_schar3 = etl::le_schar_t(0x01) == (signed char)(0x01);
      
      CHECK(check_schar1);
      CHECK(check_schar2);
      CHECK(check_schar3);

      constexpr bool check_uchar1 = etl::le_uchar_t(0x01) == etl::le_uchar_t(0x01);
      constexpr bool check_uchar2 = (unsigned char)(0x01) == etl::le_uchar_t(0x01);
      constexpr bool check_uchar3 = etl::le_uchar_t(0x01) == (unsigned char)(0x01);

      CHECK(check_uchar1);
      CHECK(check_uchar2);
      CHECK(check_uchar3);

      constexpr bool check_short1 = (short)(0x0123)         == etl::le_short_t(0x0123);
      constexpr bool check_short2 = etl::le_short_t(0x0123) == (short)(0x0123);
      constexpr bool check_short3 = (short)(0x0123)         == etl::le_short_t(0x0123);
      
      CHECK(check_short1);
      CHECK(check_short2);
      CHECK(check_short3);

      constexpr bool check_ushort1 = (unsigned short)(0x0123) == etl::le_ushort_t(0x0123);
      constexpr bool check_ushort2 = etl::le_ushort_t(0x0123) == (unsigned short)(0x0123);
      constexpr bool check_ushort3 = (unsigned short)(0x0123) == etl::le_ushort_t(0x0123);

      CHECK(check_ushort1);
      CHECK(check_ushort2);
      CHECK(check_ushort3);

      constexpr bool check_int1 = (int)(0x01234567)         == etl::le_int_t(0x01234567);
      constexpr bool check_int2 = etl::le_int_t(0x01234567) == (int)(0x01234567);
      constexpr bool check_int3 = (int)(0x01234567)         == etl::le_int_t(0x01234567);

      CHECK(check_int1);
      CHECK(check_int2);
      CHECK(check_int3);

      constexpr bool check_uint1 = (unsigned int)(0x01234567) == etl::le_uint_t(0x01234567);
      constexpr bool check_uint2 = etl::le_uint_t(0x01234567) == (unsigned int)(0x01234567);
      constexpr bool check_uint3 = (unsigned int)(0x01234567) == etl::le_uint_t(0x01234567);

      CHECK(check_uint1);
      CHECK(check_uint2);
      CHECK(check_uint3);

      if (sizeof(long) == 4U)
      {
        constexpr bool check_long1 = (long)(0x01234567)         == etl::le_long_t(0x01234567);
        constexpr bool check_long2 = etl::le_long_t(0x01234567) == (long)(0x01234567);
        constexpr bool check_long3 = (long)(0x01234567)         == etl::le_long_t(0x01234567);
        
        CHECK(check_long1);
        CHECK(check_long2);
        CHECK(check_long3);

        constexpr bool check_ulong1 = (unsigned long)(0x01234567) == etl::le_ulong_t(0x01234567);
        constexpr bool check_ulong2 = etl::le_ulong_t(0x01234567) == (unsigned long)(0x01234567);
        constexpr bool check_ulong3 = (unsigned long)(0x01234567) == etl::le_ulong_t(0x01234567);

        CHECK(check_ulong1);
        CHECK(check_ulong2);
        CHECK(check_ulong3);
      }

      if (sizeof(long long) == 8U)
      {
        constexpr bool check_long_long1 = (long long)(0x0123456789ABCDEF)         == etl::le_long_long_t(0x0123456789ABCDEF);
        constexpr bool check_long_long2 = etl::le_long_long_t(0x0123456789ABCDEF) == (long long)(0x0123456789ABCDEF);
        constexpr bool check_long_long3 = (long long)(0x0123456789ABCDEF)         == etl::le_long_long_t(0x0123456789ABCDEF);

        CHECK(check_long_long1);
        CHECK(check_long_long2);
        CHECK(check_long_long3);

        constexpr bool check_ulong_long1 = (unsigned long long)(0x0123456789ABCDEF) == etl::le_ulong_long_t(0x0123456789ABCDEF);
        constexpr bool check_ulong_long2 = etl::le_ulong_long_t(0x0123456789ABCDEF) == (unsigned long long)(0x0123456789ABCDEF);
        constexpr bool check_ulong_long3 = (unsigned long long)(0x0123456789ABCDEF) == etl::le_ulong_long_t(0x0123456789ABCDEF);

        CHECK(check_ulong_long1);
        CHECK(check_ulong_long2);
        CHECK(check_ulong_long3);
      }
    }

    //*************************************************************************
    TEST(test_le_inequality)
    {
      constexpr bool check_char1 = etl::le_char_t(0x01) != etl::le_char_t(0x02);
      constexpr bool check_char2 = (char)(0x01)         != etl::le_char_t(0x02);
      constexpr bool check_char3 = etl::le_char_t(0x01) != (char)(0x02);

      CHECK(check_char1);
      CHECK(check_char2);
      CHECK(check_char3);

      constexpr bool check_schar1 = etl::le_schar_t(0x01) != etl::le_schar_t(0x02);
      constexpr bool check_schar2 = (signed char)(0x01)   != etl::le_schar_t(0x02);
      constexpr bool check_schar3 = etl::le_schar_t(0x01) != (signed char)(0x02);

      CHECK(check_schar1);
      CHECK(check_schar2);
      CHECK(check_schar3);

      constexpr bool check_uchar1 = etl::le_uchar_t(0x01) != etl::le_uchar_t(0x02);
      constexpr bool check_uchar2 = (unsigned char)(0x01) != etl::le_uchar_t(0x02);
      constexpr bool check_uchar3 = etl::le_uchar_t(0x01) != (unsigned char)(0x02);

      CHECK(check_uchar1);
      CHECK(check_uchar2);
      CHECK(check_uchar3);

      constexpr bool check_short1 = (short)(0x0123)         != etl::le_short_t(0x0124);
      constexpr bool check_short2 = etl::le_short_t(0x0123) != (short)(0x0124);
      constexpr bool check_short3 = (short)(0x0123)         != etl::le_short_t(0x0124);

      CHECK(check_short1);
      CHECK(check_short2);
      CHECK(check_short3);

      constexpr bool check_ushort1 = (unsigned short)(0x0123) != etl::le_ushort_t(0x0124);
      constexpr bool check_ushort2 = etl::le_ushort_t(0x0123) != (unsigned short)(0x0124);
      constexpr bool check_ushort3 = (unsigned short)(0x0123) != etl::le_ushort_t(0x0124);

      CHECK(check_ushort1);
      CHECK(check_ushort2);
      CHECK(check_ushort3);

      constexpr bool check_int1 = (int)(0x01234567)         != etl::le_int_t(0x01244567);
      constexpr bool check_int2 = etl::le_int_t(0x01234567) != (int)(0x01244567);
      constexpr bool check_int3 = (int)(0x01234567)         != etl::le_int_t(0x01244567);

      CHECK(check_int1);
      CHECK(check_int2);
      CHECK(check_int3);

      constexpr bool check_uint1 = (unsigned int)(0x01234567) != etl::le_uint_t(0x01244567);
      constexpr bool check_uint2 = etl::le_uint_t(0x01234567) != (unsigned int)(0x01244567);
      constexpr bool check_uint3 = (unsigned int)(0x01234567) != etl::le_uint_t(0x01244567);

      CHECK(check_uint1);
      CHECK(check_uint2);
      CHECK(check_uint3);

      if (sizeof(long) == 4U)
      {
        constexpr bool check_long1 = (long)(0x01234567)         != etl::le_long_t(0x01244567);
        constexpr bool check_long2 = etl::le_long_t(0x01234567) != (long)(0x01244567);
        constexpr bool check_long3 = (long)(0x01234567)         != etl::le_long_t(0x01244567);

        CHECK(check_long1);
        CHECK(check_long2);
        CHECK(check_long3);

        constexpr bool check_ulong1 = (unsigned long)(0x01234567) != etl::le_ulong_t(0x01244567);
        constexpr bool check_ulong2 = etl::le_ulong_t(0x01234567) != (unsigned long)(0x01244567);
        constexpr bool check_ulong3 = (unsigned long)(0x01234567) != etl::le_ulong_t(0x01244567);

        CHECK(check_ulong1);
        CHECK(check_ulong2);
        CHECK(check_ulong3);
      }

      if (sizeof(long long) == 8U)
      {
        constexpr bool check_long_long1 = (long long)(0x0123456789ABCDEF)         != etl::le_long_long_t(0x0123457789ABCDEF);
        constexpr bool check_long_long2 = etl::le_long_long_t(0x0123456789ABCDEF) != (long long)(0x0123457789ABCDEF);
        constexpr bool check_long_long3 = (long long)(0x0123456789ABCDEF)         != etl::le_long_long_t(0x0123457789ABCDEF);

        CHECK(check_long_long1);
        CHECK(check_long_long2);
        CHECK(check_long_long3);

        constexpr bool check_ulong_long1 = (unsigned long long)(0x0123456789ABCDEF) != etl::le_ulong_long_t(0x0123457789ABCDEF);
        constexpr bool check_ulong_long2 = etl::le_ulong_long_t(0x0123456789ABCDEF) != (unsigned long long)(0x0123457789ABCDEF);
        constexpr bool check_ulong_long3 = (unsigned long long)(0x0123456789ABCDEF) != etl::le_ulong_long_t(0x0123457789ABCDEF);

        CHECK(check_ulong_long1);
        CHECK(check_ulong_long2);
        CHECK(check_ulong_long3);
      }
    }

    //*************************************************************************
    TEST(test_be_equality)
    {
      constexpr bool check_char1 = etl::be_char_t(0x01) == etl::be_char_t(0x01);
      constexpr bool check_char2 = (char)(0x01)         == etl::be_char_t(0x01);
      constexpr bool check_char3 = etl::be_char_t(0x01) == (char)(0x01);

      CHECK(check_char1);
      CHECK(check_char2);
      CHECK(check_char3);

      constexpr bool check_schar1 = etl::be_schar_t(0x01) == etl::be_schar_t(0x01);
      constexpr bool check_schar2 = (signed char)(0x01)   == etl::be_schar_t(0x01);
      constexpr bool check_schar3 = etl::be_schar_t(0x01) == (signed char)(0x01);

      CHECK(check_schar1);
      CHECK(check_schar2);
      CHECK(check_schar3);

      constexpr bool check_uchar1 = etl::be_uchar_t(0x01) == etl::be_uchar_t(0x01);
      constexpr bool check_uchar2 = (unsigned char)(0x01) == etl::be_uchar_t(0x01);
      constexpr bool check_uchar3 = etl::be_uchar_t(0x01) == (unsigned char)(0x01);

      CHECK(check_uchar1);
      CHECK(check_uchar2);
      CHECK(check_uchar3);

      constexpr bool check_short1 = (short)(0x0123)         == etl::be_short_t(0x0123);
      constexpr bool check_short2 = etl::be_short_t(0x0123) == (short)(0x0123);
      constexpr bool check_short3 = (short)(0x0123)         == etl::be_short_t(0x0123);

      CHECK(check_short1);
      CHECK(check_short2);
      CHECK(check_short3);

      constexpr bool check_ushort1 = (unsigned short)(0x0123) == etl::be_ushort_t(0x0123);
      constexpr bool check_ushort2 = etl::be_ushort_t(0x0123) == (unsigned short)(0x0123);
      constexpr bool check_ushort3 = (unsigned short)(0x0123) == etl::be_ushort_t(0x0123);

      CHECK(check_ushort1);
      CHECK(check_ushort2);
      CHECK(check_ushort3);

      constexpr bool check_int1 = (int)(0x01234567)         == etl::be_int_t(0x01234567);
      constexpr bool check_int2 = etl::be_int_t(0x01234567) == (int)(0x01234567);
      constexpr bool check_int3 = (int)(0x01234567)         == etl::be_int_t(0x01234567);

      CHECK(check_int1);
      CHECK(check_int2);
      CHECK(check_int3);

      constexpr bool check_uint1 = (unsigned int)(0x01234567) == etl::be_uint_t(0x01234567);
      constexpr bool check_uint2 = etl::be_uint_t(0x01234567) == (unsigned int)(0x01234567);
      constexpr bool check_uint3 = (unsigned int)(0x01234567) == etl::be_uint_t(0x01234567);

      CHECK(check_uint1);
      CHECK(check_uint2);
      CHECK(check_uint3);

      if (sizeof(long) == 4U)
      {
        constexpr bool check_long1 = (long)(0x01234567)         == etl::be_long_t(0x01234567);
        constexpr bool check_long2 = etl::be_long_t(0x01234567) == (long)(0x01234567);
        constexpr bool check_long3 = (long)(0x01234567)         == etl::be_long_t(0x01234567);

        CHECK(check_long1);
        CHECK(check_long2);
        CHECK(check_long3);

        constexpr bool check_ulong1 = (unsigned long)(0x01234567) == etl::be_ulong_t(0x01234567);
        constexpr bool check_ulong2 = etl::be_ulong_t(0x01234567) == (unsigned long)(0x01234567);
        constexpr bool check_ulong3 = (unsigned long)(0x01234567) == etl::be_ulong_t(0x01234567);

        CHECK(check_ulong1);
        CHECK(check_ulong2);
        CHECK(check_ulong3);
      }

      if (sizeof(long long) == 8U)
      {
        constexpr bool check_long_long1 = (long long)(0x0123456789ABCDEF)         == etl::be_long_long_t(0x0123456789ABCDEF);
        constexpr bool check_long_long2 = etl::be_long_long_t(0x0123456789ABCDEF) == (long long)(0x0123456789ABCDEF);
        constexpr bool check_long_long3 = (long long)(0x0123456789ABCDEF)         == etl::be_long_long_t(0x0123456789ABCDEF);

        CHECK(check_long_long1);
        CHECK(check_long_long2);
        CHECK(check_long_long3);

        constexpr bool check_ulong_long1 = (unsigned long long)(0x0123456789ABCDEF) == etl::be_ulong_long_t(0x0123456789ABCDEF);
        constexpr bool check_ulong_long2 = etl::be_ulong_long_t(0x0123456789ABCDEF) == (unsigned long long)(0x0123456789ABCDEF);
        constexpr bool check_ulong_long3 = (unsigned long long)(0x0123456789ABCDEF) == etl::be_ulong_long_t(0x0123456789ABCDEF);

        CHECK(check_ulong_long1);
        CHECK(check_ulong_long2);
        CHECK(check_ulong_long3);
      }
    }

    //*************************************************************************
    TEST(test_be_inequality)
    {
      constexpr bool check_char1 = etl::be_char_t(0x01) != etl::be_char_t(0x02);
      constexpr bool check_char2 = (char)(0x01)         != etl::be_char_t(0x02);
      constexpr bool check_char3 = etl::be_char_t(0x01) != (char)(0x02);

      CHECK(check_char1);
      CHECK(check_char2);
      CHECK(check_char3);

      constexpr bool check_schar1 = etl::be_schar_t(0x01) != etl::be_schar_t(0x02);
      constexpr bool check_schar2 = (signed char)(0x01)   != etl::be_schar_t(0x02);
      constexpr bool check_schar3 = etl::be_schar_t(0x01) != (signed char)(0x02);

      CHECK(check_schar1);
      CHECK(check_schar2);
      CHECK(check_schar3);

      constexpr bool check_uchar1 = etl::be_uchar_t(0x01) != etl::be_uchar_t(0x02);
      constexpr bool check_uchar2 = (unsigned char)(0x01) != etl::be_uchar_t(0x02);
      constexpr bool check_uchar3 = etl::be_uchar_t(0x01) != (unsigned char)(0x02);

      CHECK(check_uchar1);
      CHECK(check_uchar2);
      CHECK(check_uchar3);

      constexpr bool check_short1 = (short)(0x0123)         != etl::be_short_t(0x0124);
      constexpr bool check_short2 = etl::be_short_t(0x0123) != (short)(0x0124);
      constexpr bool check_short3 = (short)(0x0123)         != etl::be_short_t(0x0124);

      CHECK(check_short1);
      CHECK(check_short2);
      CHECK(check_short3);

      constexpr bool check_ushort1 = (unsigned short)(0x0123) != etl::be_ushort_t(0x0124);
      constexpr bool check_ushort2 = etl::be_ushort_t(0x0123) != (unsigned short)(0x0124);
      constexpr bool check_ushort3 = (unsigned short)(0x0123) != etl::be_ushort_t(0x0124);

      CHECK(check_ushort1);
      CHECK(check_ushort2);
      CHECK(check_ushort3);

      constexpr bool check_int1 = (int)(0x01234567)         != etl::be_int_t(0x01244567);
      constexpr bool check_int2 = etl::be_int_t(0x01234567) != (int)(0x01244567);
      constexpr bool check_int3 = (int)(0x01234567)         != etl::be_int_t(0x01244567);

      CHECK(check_int1);
      CHECK(check_int2);
      CHECK(check_int3);

      constexpr bool check_uint1 = (unsigned int)(0x01234567) != etl::be_uint_t(0x01244567);
      constexpr bool check_uint2 = etl::be_uint_t(0x01234567) != (unsigned int)(0x01244567);
      constexpr bool check_uint3 = (unsigned int)(0x01234567) != etl::be_uint_t(0x01244567);

      CHECK(check_uint1);
      CHECK(check_uint2);
      CHECK(check_uint3);

      if (sizeof(long) == 4U)
      {
        constexpr bool check_long1 = (long)(0x01234567)         != etl::be_long_t(0x01244567);
        constexpr bool check_long2 = etl::be_long_t(0x01234567) != (long)(0x01244567);
        constexpr bool check_long3 = (long)(0x01234567)         != etl::be_long_t(0x01244567);

        CHECK(check_long1);
        CHECK(check_long2);
        CHECK(check_long3);

        constexpr bool check_ulong1 = (unsigned long)(0x01234567) != etl::be_ulong_t(0x01244567);
        constexpr bool check_ulong2 = etl::be_ulong_t(0x01234567) != (unsigned long)(0x01244567);
        constexpr bool check_ulong3 = (unsigned long)(0x01234567) != etl::be_ulong_t(0x01244567);

        CHECK(check_ulong1);
        CHECK(check_ulong2);
        CHECK(check_ulong3);
      }

      if (sizeof(long long) == 8U)
      {
        constexpr bool check_long_long1 = (long long)(0x0123456789ABCDEF)         != etl::be_long_long_t(0x0123457789ABCDEF);
        constexpr bool check_long_long2 = etl::be_long_long_t(0x0123456789ABCDEF) != (long long)(0x0123457789ABCDEF);
        constexpr bool check_long_long3 = (long long)(0x0123456789ABCDEF)         != etl::be_long_long_t(0x0123457789ABCDEF);

        CHECK(check_long_long1);
        CHECK(check_long_long2);
        CHECK(check_long_long3);

        constexpr bool check_ulong_long1 = (unsigned long long)(0x0123456789ABCDEF) != etl::be_ulong_long_t(0x0123457789ABCDEF);
        constexpr bool check_ulong_long2 = etl::be_ulong_long_t(0x0123456789ABCDEF) != (unsigned long long)(0x0123457789ABCDEF);
        constexpr bool check_ulong_long3 = (unsigned long long)(0x0123456789ABCDEF) != etl::be_ulong_long_t(0x0123457789ABCDEF);

        CHECK(check_ulong_long1);
        CHECK(check_ulong_long2);
        CHECK(check_ulong_long3);
      }
    }

    //*************************************************************************
    TEST(test_le_assignment)
    {
      constexpr etl::le_char_t le_char = 0x01;
      CHECK(0x01 == (signed char)le_char);

      constexpr etl::le_schar_t le_schar = 0x01;
      CHECK(0x01 == (signed char)le_schar);

      constexpr etl::le_uchar_t le_uchar = 0x01;
      CHECK(0x01 == (unsigned char)le_uchar);

      constexpr etl::le_short_t le_short = 0x0123;
      CHECK(0x0123 == (short)le_short);

      constexpr etl::le_ushort_t le_ushort = 0x0123;
      CHECK(0x0123 == (unsigned short)le_ushort);

      constexpr etl::le_int_t le_int = 0x01234567;
      CHECK(0x01234567 == (int)le_int);

      constexpr etl::le_uint_t le_uint = 0x01234567;
      CHECK(0x01234567 == (unsigned int)le_uint);

      if (sizeof(long) == 4U)
      {
        constexpr etl::le_long_t le_long = 0x01234567;
        CHECK(0x01234567 == (long)le_long);

        constexpr etl::le_ulong_t le_ulong = 0x01234567;
        CHECK(0x01234567 == (unsigned long)le_ulong);
      }

      if (sizeof(long long) == 8U)
      {
        constexpr etl::le_long_long_t le_long = 0x0123456789ABCDEF;
        CHECK(0x0123456789ABCDEF == (long long)le_long);

        constexpr etl::le_ulong_long_t le_ulong = 0x0123456789ABCDEF;
        CHECK(0x0123456789ABCDEF == (unsigned long long)le_ulong);
      }
    }

    //*************************************************************************
    TEST(test_be_assignment)
    {
      constexpr etl::be_char_t be_char = 0x01;
      CHECK(0x01 == (signed char)be_char);

      constexpr etl::be_schar_t be_schar = 0x01;
      CHECK(0x01 == (signed char)be_schar);

      constexpr etl::be_uchar_t be_uchar = 0x01;
      CHECK(0x01 == (unsigned char)be_uchar);

      constexpr etl::be_short_t be_short = 0x0123;
      CHECK(0x0123 == (short)be_short);

      constexpr etl::be_ushort_t be_ushort = 0x0123;
      CHECK(0x0123 == (unsigned short)be_ushort);

      constexpr etl::be_int_t be_int = 0x01234567;
      CHECK(0x01234567 == (int)be_int);

      constexpr etl::be_uint_t be_uint = 0x01234567;
      CHECK(0x01234567 == (unsigned int)be_uint);

      if (sizeof(long) == 4U)
      {
        constexpr etl::be_long_t be_long = 0x01234567;
        CHECK(0x01234567 == (long)be_long);

        constexpr etl::be_ulong_t be_ulong = 0x01234567;
        CHECK(0x01234567 == (unsigned long)be_ulong);
      }

      if (sizeof(long long) == 8U)
      {
        constexpr etl::be_long_long_t be_long = 0x0123456789ABCDEF;
        CHECK(0x0123456789ABCDEF == (long long)be_long);

        constexpr etl::be_ulong_long_t be_ulong = 0x0123456789ABCDEF;
        CHECK(0x0123456789ABCDEF == (unsigned long long)be_ulong);
      }
    }

    //*************************************************************************
    TEST(test_data)
    {
      static constexpr etl::be_uint16_t test(0x1234);
      
      constexpr etl::be_uint16_t::const_pointer p1 = test.data();
      constexpr etl::be_uint16_t::const_pointer p2 = test.data() + 1U;

      CHECK_EQUAL(0x12, *p1);
      CHECK_EQUAL(0x34, *p2);
    }

    //*************************************************************************
    TEST(test_begin_end_iterator)
    {
      using const_iterator = etl::be_uint16_t::const_iterator;

      static constexpr etl::be_uint16_t test(0x1234);

      //*******************************
      constexpr const_iterator itr1 = test.begin();
      CHECK_EQUAL(int(0x12), int(*itr1));

      constexpr const_iterator itr2 = test.begin() + 1U;
      CHECK_EQUAL(int(0x34), int(*itr2));

      constexpr const_iterator itr3 = test.begin() + 2U;
      CHECK(test.end() == itr3);
    }

    //*************************************************************************
    TEST(test_rbegin_rend_iterator)
    {
      using const_reverse_iterator = etl::be_uint16_t::const_reverse_iterator;

      static constexpr etl::be_uint16_t test(0x1234);

      //*******************************
      constexpr const_reverse_iterator itr1 = test.rbegin();
      CHECK_EQUAL(int(0x34), int(*itr1));

      constexpr const_reverse_iterator itr2 = test.rbegin() + 1U;
      CHECK_EQUAL(int(0x12), int(*itr2));

      constexpr const_reverse_iterator itr3 = test.rbegin() + 2U;
      CHECK(test.rend() == itr3);
    }

    //*************************************************************************
    TEST(test_index_operator)
    {
      constexpr etl::le_uint16_t test_le(0x1234);
      constexpr etl::be_uint16_t test_be(0x1234);

      constexpr const char lea = test_le[0];
      constexpr const char leb = test_le[1];

      constexpr const char bea = test_be[0];
      constexpr const char beb = test_be[1];

      CHECK_EQUAL(0x34, lea);
      CHECK_EQUAL(0x12, leb);

      CHECK_EQUAL(0x12, bea);
      CHECK_EQUAL(0x34, beb);
    }

    //*************************************************************************
    TEST(test_value)
    {
      constexpr etl::le_uint16_t test_le(0x1234);
      constexpr etl::be_uint16_t test_be(0x1234);

      constexpr uint16_t lev = test_le.value();
      constexpr uint16_t bev = test_be.value();

      CHECK_EQUAL(0x1234, lev);
      CHECK_EQUAL(0x1234, bev);
    }

    //*************************************************************************
    TEST(test_storage_bytes)
    {
      constexpr etl::le_uint16_t test_le(0x1234);
      constexpr etl::be_uint16_t test_be(0x1234);

      constexpr int lev0 = test_le[0];
      constexpr int lev1 = test_le[1];
      
      constexpr int bev0 = test_be[0];
      constexpr int bev1 = test_be[1];

      CHECK_EQUAL(0x34, lev0);
      CHECK_EQUAL(0x12, lev1);
      CHECK_EQUAL(0x12, bev0);
      CHECK_EQUAL(0x34, bev1);
    }
  };
}

#endif
