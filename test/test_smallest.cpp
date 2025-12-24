/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl
https://www.etlcpp.com

Copyright(c) 2014 John Wellbelove

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

#include "etl/smallest.h"

#include <type_traits>

namespace
{
  SUITE(test_smallest)
  {
    //*************************************************************************
    TEST(test_pod)
    {
      size_t size;
      bool   type;

      size = etl::smallest_type<char, short, int>::size;
      type = etl::is_same<char, etl::smallest_type<char, short, int>::type>::value;

      CHECK_EQUAL(sizeof(char), size);
      CHECK(type);

      size = etl::smallest_type<int, char, short>::size;
      type = etl::is_same<char, etl::smallest_type<char, short, int>::type>::value;

      CHECK_EQUAL(sizeof(char), size);
      CHECK(type);
    }


    //*************************************************************************
    TEST(test_pod_vt)
    {
      size_t size;
      bool   type;

#if ETL_USING_CPP17
      size = etl::smallest_type_v<char, short, int>;
      CHECK_EQUAL(sizeof(char), size);
#endif
      type = etl::is_same<char, etl::smallest_type_t<char, short, int>>::value;
      CHECK(type);

      size = etl::smallest_type<int, char, short>::size;
      CHECK_EQUAL(sizeof(char), size);

      type = etl::is_same<char, etl::smallest_type<char, short, int>::type>::value;
      CHECK(type);
    }

    //*************************************************************************
    TEST(test_non_pod)
    {
      size_t size;
      bool   type;

      struct S1 { char a; char  b; char c; };
      struct S2 { char a; short b; char c; };
      struct S3 { int  a; short b; char c; };

      size = etl::smallest_type<S1, S2, S3>::size;
      type = etl::is_same<S1, etl::smallest_type<S1, S2, S3>::type>::value;

      CHECK_EQUAL(sizeof(S1), size);
      CHECK(type);

      size = etl::smallest_type<S2, S3, S1>::size;
      type = etl::is_same<S1, etl::smallest_type<S2, S3, S1>::type>::value;

      CHECK_EQUAL(sizeof(S1), size);
      CHECK(type);
    }

    //*************************************************************************
    TEST(test_non_pod_vt)
    {
      bool   type;

      struct S1 { char a; char  b; char c; };
      struct S2 { char a; short b; char c; };
      struct S3 { int  a; short b; char c; };

#if ETL_USING_CPP17
      size_t size;
      
      size = etl::smallest_type_v<S1, S2, S3>;
      CHECK_EQUAL(sizeof(S1), size);
#endif

      type = etl::is_same<S1, etl::smallest_type_t<S1, S2, S3>>::value;
      CHECK(type);

#if ETL_USING_CPP17
      size = etl::smallest_type_v<S2, S3, S1>;
      CHECK_EQUAL(sizeof(S1), size);
#endif

      type = etl::is_same<S1, etl::smallest_type_t<S2, S3, S1>>::value;
      CHECK(type);
    }

    //*************************************************************************
    TEST(test_smallest_uint_for_bits)
    {
      bool type;

      type = etl::is_same<uint_least8_t, etl::smallest_uint_for_bits<7>::type>::value;
      CHECK(type);

      type = etl::is_same<uint_least8_t, etl::smallest_uint_for_bits<8>::type>::value;
      CHECK(type);

      type = etl::is_same<uint_least16_t, etl::smallest_uint_for_bits<9>::type>::value;
      CHECK(type);

      type = etl::is_same<uint_least16_t, etl::smallest_uint_for_bits<15>::type>::value;
      CHECK(type);

      type = etl::is_same<uint_least16_t, etl::smallest_uint_for_bits<16>::type>::value;
      CHECK(type);

      type = etl::is_same<uint_least32_t, etl::smallest_uint_for_bits<17>::type>::value;
      CHECK(type);

      type = etl::is_same<uint_least32_t, etl::smallest_uint_for_bits<31>::type>::value;
      CHECK(type);

      type = etl::is_same<uint_least32_t, etl::smallest_uint_for_bits<32>::type>::value;
      CHECK(type);

      type = etl::is_same<uint_least64_t, etl::smallest_uint_for_bits<33>::type>::value;
      CHECK(type);

      type = etl::is_same<uint_least64_t, etl::smallest_uint_for_bits<63>::type>::value;
      CHECK(type);

      type = etl::is_same<uint_least64_t, etl::smallest_uint_for_bits<64>::type>::value;
      CHECK(type);

      type = etl::is_same<uint_least64_t, etl::smallest_uint_for_bits<65>::type>::value;
      CHECK(type);
    }

    //*************************************************************************
    TEST(test_smallest_uint_for_bits_t)
    {
      bool type;

      type = etl::is_same<uint_least8_t, etl::smallest_uint_for_bits_t<7>>::value;
      CHECK(type);

      type = etl::is_same<uint_least8_t, etl::smallest_uint_for_bits_t<8>>::value;
      CHECK(type);

      type = etl::is_same<uint_least16_t, etl::smallest_uint_for_bits_t<9>>::value;
      CHECK(type);

      type = etl::is_same<uint_least16_t, etl::smallest_uint_for_bits_t<15>>::value;
      CHECK(type);

      type = etl::is_same<uint_least16_t, etl::smallest_uint_for_bits_t<16>>::value;
      CHECK(type);

      type = etl::is_same<uint_least32_t, etl::smallest_uint_for_bits_t<17>>::value;
      CHECK(type);

      type = etl::is_same<uint_least32_t, etl::smallest_uint_for_bits_t<31>>::value;
      CHECK(type);

      type = etl::is_same<uint_least32_t, etl::smallest_uint_for_bits_t<32>>::value;
      CHECK(type);

      type = etl::is_same<uint_least64_t, etl::smallest_uint_for_bits_t<33>>::value;
      CHECK(type);

      type = etl::is_same<uint_least64_t, etl::smallest_uint_for_bits_t<63>>::value;
      CHECK(type);

      type = etl::is_same<uint_least64_t, etl::smallest_uint_for_bits_t<64>>::value;
      CHECK(type);

      type = etl::is_same<uint_least64_t, etl::smallest_uint_for_bits_t<65>>::value;
      CHECK(type);
    }

    //*************************************************************************
    TEST(test_smallest_int_for_bits)
    {
      bool type;

      type = etl::is_same<int_least8_t, etl::smallest_int_for_bits<7>::type>::value;
      CHECK(type);

      type = etl::is_same<int_least8_t, etl::smallest_int_for_bits<8>::type>::value;
      CHECK(type);

      type = etl::is_same<int_least16_t, etl::smallest_int_for_bits<9>::type>::value;
      CHECK(type);

      type = etl::is_same<int_least16_t, etl::smallest_int_for_bits<15>::type>::value;
      CHECK(type);

      type = etl::is_same<int_least16_t, etl::smallest_int_for_bits<16>::type>::value;
      CHECK(type);

      type = etl::is_same<int_least32_t, etl::smallest_int_for_bits<17>::type>::value;
      CHECK(type);

      type = etl::is_same<int_least32_t, etl::smallest_int_for_bits<31>::type>::value;
      CHECK(type);

      type = etl::is_same<int_least32_t, etl::smallest_int_for_bits<32>::type>::value;
      CHECK(type);

      type = etl::is_same<int_least64_t, etl::smallest_int_for_bits<33>::type>::value;
      CHECK(type);

      type = etl::is_same<int_least64_t, etl::smallest_int_for_bits<63>::type>::value;
      CHECK(type);

      type = etl::is_same<int_least64_t, etl::smallest_int_for_bits<64>::type>::value;
      CHECK(type);

      type = etl::is_same<int_least64_t, etl::smallest_int_for_bits<65>::type>::value;
      CHECK(type);
    }

    //*************************************************************************
    TEST(test_smallest_int_for_bits_t)
    {
      bool type;

      type = etl::is_same<int_least8_t, etl::smallest_int_for_bits_t<7>>::value;
      CHECK(type);

      type = etl::is_same<int_least8_t, etl::smallest_int_for_bits_t<8>>::value;
      CHECK(type);

      type = etl::is_same<int_least16_t, etl::smallest_int_for_bits_t<9>>::value;
      CHECK(type);

      type = etl::is_same<int_least16_t, etl::smallest_int_for_bits_t<15>>::value;
      CHECK(type);

      type = etl::is_same<int_least16_t, etl::smallest_int_for_bits_t<16>>::value;
      CHECK(type);

      type = etl::is_same<int_least32_t, etl::smallest_int_for_bits_t<17>>::value;
      CHECK(type);

      type = etl::is_same<int_least32_t, etl::smallest_int_for_bits_t<31>>::value;
      CHECK(type);

      type = etl::is_same<int_least32_t, etl::smallest_int_for_bits_t<32>>::value;
      CHECK(type);

      type = etl::is_same<int_least64_t, etl::smallest_int_for_bits_t<33>>::value;
      CHECK(type);

      type = etl::is_same<int_least64_t, etl::smallest_int_for_bits_t<63>>::value;
      CHECK(type);

      type = etl::is_same<int_least64_t, etl::smallest_int_for_bits_t<64>>::value;
      CHECK(type);

      type = etl::is_same<int_least64_t, etl::smallest_int_for_bits_t<65>>::value;
      CHECK(type);
    }

    //*************************************************************************
    TEST(test_smallest_uint_for_value)
    {
      bool type;

      type = etl::is_same<uint_least8_t, etl::smallest_uint_for_value<0>::type>::value;
      CHECK(type);

      type = etl::is_same<uint_least8_t, etl::smallest_uint_for_value<UINT8_MAX>::type>::value;
      CHECK(type);

      type = etl::is_same<uint_least16_t, etl::smallest_uint_for_value<UINT8_MAX + 1>::type>::value;
      CHECK(type);

      type = etl::is_same<uint_least16_t, etl::smallest_uint_for_value<UINT16_MAX>::type>::value;
      CHECK(type);

      type = etl::is_same<uint_least32_t, etl::smallest_uint_for_value<UINT16_MAX + 1>::type>::value;
      CHECK(type);

      type = etl::is_same<uint_least32_t, etl::smallest_uint_for_value<UINT32_MAX>::type>::value;
      CHECK(type);

      type = etl::is_same<uint_least64_t, etl::smallest_uint_for_value<uint64_t(UINT32_MAX) + 1>::type>::value;
      CHECK(type);

      type = etl::is_same<uint_least64_t, etl::smallest_uint_for_value<UINT64_MAX>::type>::value;
      CHECK(type);
    }

    //*************************************************************************
    TEST(test_smallest_uint_for_value_t)
    {
      bool type;

      type = etl::is_same<uint_least8_t, etl::smallest_uint_for_value_t<0>>::value;
      CHECK(type);

      type = etl::is_same<uint_least8_t, etl::smallest_uint_for_value_t<UINT8_MAX>>::value;
      CHECK(type);

      type = etl::is_same<uint_least16_t, etl::smallest_uint_for_value_t<UINT8_MAX + 1>>::value;
      CHECK(type);

      type = etl::is_same<uint_least16_t, etl::smallest_uint_for_value_t<UINT16_MAX>>::value;
      CHECK(type);

      type = etl::is_same<uint_least32_t, etl::smallest_uint_for_value_t<UINT16_MAX + 1>>::value;
      CHECK(type);

      type = etl::is_same<uint_least32_t, etl::smallest_uint_for_value_t<UINT32_MAX>>::value;
      CHECK(type);

      type = etl::is_same<uint_least64_t, etl::smallest_uint_for_value_t<uint64_t(UINT32_MAX) + 1>>::value;
      CHECK(type);

      type = etl::is_same<uint_least64_t, etl::smallest_uint_for_value_t<UINT64_MAX>>::value;
      CHECK(type);
    }

    //*************************************************************************
    TEST(test_smallest_int_for_value)
    {
      bool type;

      type = etl::is_same<int_least8_t, etl::smallest_int_for_value<0>::type>::value;
      CHECK(type);

      type = etl::is_same<int_least8_t, etl::smallest_int_for_value<INT8_MIN>::type>::value;
      CHECK(type);

      type = etl::is_same<int_least8_t, etl::smallest_int_for_value<INT8_MAX>::type>::value;
      CHECK(type);

      type = etl::is_same<int_least16_t, etl::smallest_int_for_value<INT8_MIN - 1>::type>::value;
      CHECK(type);

      type = etl::is_same<int_least16_t, etl::smallest_int_for_value<INT8_MAX + 1>::type>::value;
      CHECK(type);

      type = etl::is_same<int_least16_t, etl::smallest_int_for_value<INT16_MIN>::type>::value;
      CHECK(type);

      type = etl::is_same<int_least16_t, etl::smallest_int_for_value<INT16_MAX>::type>::value;
      CHECK(type);

      type = etl::is_same<int_least32_t, etl::smallest_int_for_value<INT16_MIN - 1>::type>::value;
      CHECK(type);

      type = etl::is_same<int_least32_t, etl::smallest_int_for_value<INT16_MAX + 1>::type>::value;
      CHECK(type);

      type = etl::is_same<int_least32_t, etl::smallest_int_for_value<INT32_MIN>::type>::value;
      CHECK(type);

      type = etl::is_same<int_least32_t, etl::smallest_int_for_value<INT32_MAX>::type>::value;
      CHECK(type);

      type = etl::is_same<int_least64_t, etl::smallest_int_for_value<intmax_t(INT32_MIN) - 1>::type>::value;
      CHECK(type);

      type = etl::is_same<int_least64_t, etl::smallest_int_for_value<intmax_t(INT32_MAX) + 1>::type>::value;
      CHECK(type);

      type = etl::is_same<int_least64_t, etl::smallest_int_for_value<INT64_MIN>::type>::value;
      CHECK(type);

      type = etl::is_same<int_least64_t, etl::smallest_int_for_value<INT64_MAX>::type>::value;
      CHECK(type);
    }

    //*************************************************************************
    TEST(test_smallest_int_for_value_t)
    {
      bool type;

      type = etl::is_same<int_least8_t, etl::smallest_int_for_value_t<0>>::value;
      CHECK(type);

      type = etl::is_same<int_least8_t, etl::smallest_int_for_value_t<INT8_MIN>>::value;
      CHECK(type);

      type = etl::is_same<int_least8_t, etl::smallest_int_for_value_t<INT8_MAX>>::value;
      CHECK(type);

      type = etl::is_same<int_least16_t, etl::smallest_int_for_value_t<INT8_MIN - 1>>::value;
      CHECK(type);

      type = etl::is_same<int_least16_t, etl::smallest_int_for_value_t<INT8_MAX + 1>>::value;
      CHECK(type);

      type = etl::is_same<int_least16_t, etl::smallest_int_for_value_t<INT16_MIN>>::value;
      CHECK(type);

      type = etl::is_same<int_least16_t, etl::smallest_int_for_value_t<INT16_MAX>>::value;
      CHECK(type);

      type = etl::is_same<int_least32_t, etl::smallest_int_for_value_t<INT16_MIN - 1>>::value;
      CHECK(type);

      type = etl::is_same<int_least32_t, etl::smallest_int_for_value_t<INT16_MAX + 1>>::value;
      CHECK(type);

      type = etl::is_same<int_least32_t, etl::smallest_int_for_value_t<INT32_MIN>>::value;
      CHECK(type);

      type = etl::is_same<int_least32_t, etl::smallest_int_for_value_t<INT32_MAX>>::value;
      CHECK(type);

      type = etl::is_same<int_least64_t, etl::smallest_int_for_value_t<intmax_t(INT32_MIN) - 1>>::value;
      CHECK(type);

      type = etl::is_same<int_least64_t, etl::smallest_int_for_value_t<intmax_t(INT32_MAX) + 1>>::value;
      CHECK(type);

      type = etl::is_same<int_least64_t, etl::smallest_int_for_value_t<INT64_MIN>>::value;
      CHECK(type);

      type = etl::is_same<int_least64_t, etl::smallest_int_for_value_t<INT64_MAX>>::value;
      CHECK(type);
    }
  };
}
