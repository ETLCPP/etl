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

#include "etl/largest.h"

#include <type_traits>

namespace
{
  SUITE(test_largest)
  {
    //*************************************************************************
    TEST(test_pod_type)
    {
      size_t size;
      bool   type;

      size = etl::largest_type<char, short, int>::size;
      type = etl::is_same<int, etl::largest_type<char, short, int>::type>::value;

      CHECK_EQUAL(sizeof(int), size);
      CHECK(type);

      size = etl::largest_type<int, char, short>::size;
      type = etl::is_same<int, etl::largest_type<char, short, int>::type>::value;

      CHECK_EQUAL(sizeof(int), size);
      CHECK(type);
    }

#if ETL_USING_CPP17 && !defined(ETL_LARGEST_TYPE_FORCE_CPP03_IMPLEMENTATION)
    //*************************************************************************
    TEST(test_pod_type_vt)
    {
      size_t size;
      bool   type;

      size = etl::largest_type_v<char, short, int>;
      type = etl::is_same<int, etl::largest_type_t<char, short, int>>::value;

      CHECK_EQUAL(sizeof(int), size);
      CHECK(type);

      size = etl::largest_type_v<int, char, short>;
      type = etl::is_same<int, etl::largest_type_t<char, short, int>>::value;

      CHECK_EQUAL(sizeof(int), size);
      CHECK(type);
    }
#endif

    //*************************************************************************
    TEST(test_non_pod_type)
    {
      size_t size;
      bool   type;

      struct S1 { char a; char  b; char c; };
      struct S2 { char a; short b; char c; };
      struct S3 { int  a; short b; char c; };

      size = etl::largest_type<S1, S2, S3>::size;
      type = etl::is_same<S3, etl::largest_type<S1, S2, S3>::type>::value;

      CHECK_EQUAL(sizeof(S3), size);
      CHECK(type);

      size = etl::largest_type<S2, S3, S1>::size;
      type = etl::is_same<S3, etl::largest_type<S2, S3, S1>::type>::value;

      CHECK_EQUAL(sizeof(S3), size);
      CHECK(type);
    }

#if ETL_USING_CPP17 && !defined(ETL_LARGEST_TYPE_FORCE_CPP03_IMPLEMENTATION)
    //*************************************************************************
    TEST(test_non_pod_type_vt)
    {
      size_t size;
      bool   type;

      struct S1 { char a; char  b; char c; };
      struct S2 { char a; short b; char c; };
      struct S3 { int  a; short b; char c; };

      size = etl::largest_type_v<S1, S2, S3>;
      type = etl::is_same<S3, etl::largest_type_t<S1, S2, S3>>::value;

      CHECK_EQUAL(sizeof(S3), size);
      CHECK(type);

      size = etl::largest_type_v<S2, S3, S1>;
      type = etl::is_same<S3, etl::largest_type_t<S2, S3, S1>>::value;

      CHECK_EQUAL(sizeof(S3), size);
      CHECK(type);
    }
#endif

    //*************************************************************************
    TEST(test_pod_alignment)
    {
      size_t size = etl::largest_alignment<char, short, int, double>::value;

      CHECK_EQUAL(std::alignment_of<double>::value, size);
    }

#if ETL_USING_CPP17 && !defined(ETL_LARGEST_TYPE_FORCE_CPP03_IMPLEMENTATION)
    //*************************************************************************
    TEST(test_pod_alignment_v)
    {
      size_t size = etl::largest_alignment_v<char, short, int, double>;

      CHECK_EQUAL(std::alignment_of<double>::value, size);
    }
#endif

    //*************************************************************************
    TEST(test_non_pod_alignment)
    {
      struct S1 { char a; char  b; char c; };
      struct S2 { char a; short b; char c; };
      struct S3 { int  a; short b; char c; };

      size_t size = etl::largest_alignment<S1, S2, S3>::value;

      CHECK_EQUAL(std::alignment_of<S3>::value, size);
    }

#if ETL_USING_CPP17 && !defined(ETL_LARGEST_TYPE_FORCE_CPP03_IMPLEMENTATION)
    //*************************************************************************
    TEST(test_non_pod_alignment_v)
    {
      struct S1 { char a; char  b; char c; };
      struct S2 { char a; short b; char c; };
      struct S3 { int  a; short b; char c; };

      size_t size = etl::largest_alignment_v<S1, S2, S3>;

      CHECK_EQUAL(std::alignment_of<S3>::value, size);
    }
#endif

    //*************************************************************************
    TEST(test_larger_int_type)
    {
      CHECK(bool(etl::is_same<etl::larger_int_type<int8_t>::type,  int16_t>::value));
      CHECK(bool(etl::is_same<etl::larger_int_type<int16_t>::type, int32_t>::value));
      CHECK(bool(etl::is_same<etl::larger_int_type<int32_t>::type, int64_t>::value));
      CHECK(bool(etl::is_same<etl::larger_int_type<int64_t>::type, int64_t>::value));

      CHECK(bool(etl::is_same<etl::larger_int_type<uint8_t>::type,  int16_t>::value));
      CHECK(bool(etl::is_same<etl::larger_int_type<uint16_t>::type, int32_t>::value));
      CHECK(bool(etl::is_same<etl::larger_int_type<uint32_t>::type, int64_t>::value));
      CHECK(bool(etl::is_same<etl::larger_int_type<uint64_t>::type, int64_t>::value));
    }

    //*************************************************************************
    TEST(test_larger_int_type_t)
    {
      CHECK(bool(etl::is_same<etl::larger_int_type_t<int8_t>,  int16_t>::value));
      CHECK(bool(etl::is_same<etl::larger_int_type_t<int16_t>, int32_t>::value));
      CHECK(bool(etl::is_same<etl::larger_int_type_t<int32_t>, int64_t>::value));
      CHECK(bool(etl::is_same<etl::larger_int_type_t<int64_t>, int64_t>::value));

      CHECK(bool(etl::is_same<etl::larger_int_type_t<uint8_t>,  int16_t>::value));
      CHECK(bool(etl::is_same<etl::larger_int_type_t<uint16_t>, int32_t>::value));
      CHECK(bool(etl::is_same<etl::larger_int_type_t<uint32_t>, int64_t>::value));
      CHECK(bool(etl::is_same<etl::larger_int_type_t<uint64_t>, int64_t>::value));
    }

    //*************************************************************************
    TEST(test_larger_uint_type)
    {
      CHECK(bool(etl::is_same<etl::larger_uint_type<int8_t>::type,  uint16_t>::value));
      CHECK(bool(etl::is_same<etl::larger_uint_type<int16_t>::type, uint32_t>::value));
      CHECK(bool(etl::is_same<etl::larger_uint_type<int32_t>::type, uint64_t>::value));
      CHECK(bool(etl::is_same<etl::larger_uint_type<int64_t>::type, uint64_t>::value));

      CHECK(bool(etl::is_same<etl::larger_uint_type<uint8_t>::type,  uint16_t>::value));
      CHECK(bool(etl::is_same<etl::larger_uint_type<uint16_t>::type, uint32_t>::value));
      CHECK(bool(etl::is_same<etl::larger_uint_type<uint32_t>::type, uint64_t>::value));
      CHECK(bool(etl::is_same<etl::larger_uint_type<uint64_t>::type, uint64_t>::value));
    }

    //*************************************************************************
    TEST(test_larger_uint_type_t)
    {
      CHECK(bool(etl::is_same<etl::larger_uint_type_t<int8_t>,  uint16_t>::value));
      CHECK(bool(etl::is_same<etl::larger_uint_type_t<int16_t>, uint32_t>::value));
      CHECK(bool(etl::is_same<etl::larger_uint_type_t<int32_t>, uint64_t>::value));
      CHECK(bool(etl::is_same<etl::larger_uint_type_t<int64_t>, uint64_t>::value));

      CHECK(bool(etl::is_same<etl::larger_uint_type_t<uint8_t>,  uint16_t>::value));
      CHECK(bool(etl::is_same<etl::larger_uint_type_t<uint16_t>, uint32_t>::value));
      CHECK(bool(etl::is_same<etl::larger_uint_type_t<uint32_t>, uint64_t>::value));
      CHECK(bool(etl::is_same<etl::larger_uint_type_t<uint64_t>, uint64_t>::value));
    }

    //*************************************************************************
    TEST(test_larger_type)
    {
      CHECK(bool(etl::is_same<etl::larger_type<int8_t>::type,  int16_t>::value));
      CHECK(bool(etl::is_same<etl::larger_type<int16_t>::type, int32_t>::value));
      CHECK(bool(etl::is_same<etl::larger_type<int32_t>::type, int64_t>::value));
      CHECK(bool(etl::is_same<etl::larger_type<int64_t>::type, int64_t>::value));

      CHECK(bool(etl::is_same<etl::larger_type<uint8_t>::type,  uint16_t>::value));
      CHECK(bool(etl::is_same<etl::larger_type<uint16_t>::type, uint32_t>::value));
      CHECK(bool(etl::is_same<etl::larger_type<uint32_t>::type, uint64_t>::value));
      CHECK(bool(etl::is_same<etl::larger_type<uint64_t>::type, uint64_t>::value));
    }

    //*************************************************************************
    TEST(test_larger_type_t)
    {
      CHECK(bool(etl::is_same<etl::larger_type_t<int8_t>,  int16_t>::value));
      CHECK(bool(etl::is_same<etl::larger_type_t<int16_t>, int32_t>::value));
      CHECK(bool(etl::is_same<etl::larger_type_t<int32_t>, int64_t>::value));
      CHECK(bool(etl::is_same<etl::larger_type_t<int64_t>, int64_t>::value));

      CHECK(bool(etl::is_same<etl::larger_type_t<uint8_t>,  uint16_t>::value));
      CHECK(bool(etl::is_same<etl::larger_type_t<uint16_t>, uint32_t>::value));
      CHECK(bool(etl::is_same<etl::larger_type_t<uint32_t>, uint64_t>::value));
      CHECK(bool(etl::is_same<etl::larger_type_t<uint64_t>, uint64_t>::value));
    }
  };
}
