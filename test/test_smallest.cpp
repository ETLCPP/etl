/******************************************************************************
The MIT License(MIT)

Embedded Template Library.

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

#include "../smallest.h"

#include <type_traits>

namespace
{		
  SUITE(test_smallst)
  {
    //*************************************************************************
    TEST(test_pod)
    {
      size_t size;
      bool   type;

      size = etl::smallest_type<char, short, int>::size;
      type = std::is_same<char, etl::smallest_type<char, short, int>::type>::value;

      CHECK_EQUAL(sizeof(char), size);
      CHECK(type);

      size = etl::smallest_type<int, char, short>::size;
      type = std::is_same<char, etl::smallest_type<char, short, int>::type>::value;

      CHECK_EQUAL(sizeof(char), size);
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
      type = std::is_same<S1, etl::smallest_type<S1, S2, S3>::type>::value;

      CHECK_EQUAL(sizeof(S1), size);
      CHECK(type);

      size = etl::smallest_type<S2, S3, S1>::size;
      type = std::is_same<S1, etl::smallest_type<S2, S3, S1>::type>::value;

      CHECK_EQUAL(sizeof(S1), size);
      CHECK(type);
    }

    //*************************************************************************
    TEST(test_smallest_size_for_bits)
    {
      bool type;

      type = std::is_same<uint8_t, etl::smallest_uint_for_bits<7>::type>::value;
      CHECK(type);

      type = std::is_same<uint8_t, etl::smallest_uint_for_bits<8>::type>::value;
      CHECK(type);

      type = std::is_same<uint16_t, etl::smallest_uint_for_bits<9>::type>::value;
      CHECK(type);

      type = std::is_same<uint16_t, etl::smallest_uint_for_bits<15>::type>::value;
      CHECK(type);

      type = std::is_same<uint16_t, etl::smallest_uint_for_bits<16>::type>::value;
      CHECK(type);

      type = std::is_same<uint32_t, etl::smallest_uint_for_bits<17>::type>::value;
      CHECK(type);

      type = std::is_same<uint32_t, etl::smallest_uint_for_bits<31>::type>::value;
      CHECK(type);

      type = std::is_same<uint32_t, etl::smallest_uint_for_bits<32>::type>::value;
      CHECK(type);

      type = std::is_same<uint64_t, etl::smallest_uint_for_bits<33>::type>::value;
      CHECK(type);

      type = std::is_same<uint64_t, etl::smallest_uint_for_bits<63>::type>::value;
      CHECK(type);

      type = std::is_same<uint64_t, etl::smallest_uint_for_bits<64>::type>::value;
      CHECK(type);

      type = std::is_same<uint64_t, etl::smallest_uint_for_bits<65>::type>::value;
      CHECK(type);
    }
  };
}
