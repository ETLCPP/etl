/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl

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

#include "../alignment.h"
#include "../type_traits.h"

#include <type_traits>

namespace
{
  struct Data
  {
    int i;
    char c;
    short s[3];
  };

  SUITE(test_alignment)
  {
    //*************************************************************************
    TEST(test_align_at)
    {
      size_t alignment;

      // 1
      typedef etl::align_at<char, 1> align_char_1;
      alignment = etl::alignment_of<align_char_1>::value;
      CHECK_EQUAL(1, alignment);

      // 2
      typedef etl::align_at<char, 2> align_char_2;
      alignment = etl::alignment_of<align_char_2>::value;
      CHECK_EQUAL(2, alignment);

      align_char_2 data2[2];
      CHECK((size_t(&data2[0]) % 2) == 0);
      CHECK((size_t(&data2[1]) % 2) == 0);

      // 4
      typedef etl::align_at<char, 4> align_char_4;
      alignment = etl::alignment_of<align_char_4>::value;
      CHECK_EQUAL(4, alignment);

      align_char_4 data4[2];
      CHECK((size_t(&data4[0]) % 4) == 0);
      CHECK((size_t(&data4[1]) % 4) == 0);

      // 8
      typedef etl::align_at<char, 8> align_char_8;
      alignment = etl::alignment_of<align_char_8>::value;
      CHECK_EQUAL(8, alignment);

      align_char_8 data8[2];
      CHECK((size_t(&data8[0]) % 8) == 0);
      CHECK((size_t(&data8[1]) % 8) == 0);

      // 16
      typedef etl::align_at<char, 16> align_char_16;
      alignment = etl::alignment_of<align_char_16>::value;
      CHECK_EQUAL(16, alignment);

      align_char_16 data16[2];
      CHECK((size_t(&data16[0]) % 16) == 0);
      CHECK((size_t(&data16[1]) % 16) == 0);
    }

    //*************************************************************************
    TEST(test_align_as)
    {
      size_t alignment;
      size_t expected;

      // char
      typedef etl::align_as<char, char> align_char_char;
      alignment = etl::alignment_of<align_char_char>::value;
      expected  = std::alignment_of<char>::value;
      CHECK_EQUAL(expected, alignment);

      // short
      typedef etl::align_as<char, short> align_char_short;
      alignment = etl::alignment_of<align_char_short>::value;
      expected  = std::alignment_of<short>::value;
      CHECK_EQUAL(expected, alignment);

      align_char_short data_short[2];
      CHECK((size_t(&data_short[0].value) % expected) == 0);
      CHECK((size_t(&data_short[1].value) % expected) == 0);

      // int
      typedef etl::align_as<char, int> align_char_int;
      alignment = etl::alignment_of<align_char_int>::value;
      expected  = std::alignment_of<int>::value;
      CHECK_EQUAL(expected, alignment);

      align_char_int data_int[2];
      CHECK((size_t(&data_int[0].value) % expected) == 0);
      CHECK((size_t(&data_int[1].value) % expected) == 0);

      // long
      typedef etl::align_as<char, long> align_char_long;
      alignment = etl::alignment_of<align_char_long>::value;
      expected  = std::alignment_of<long>::value;
      CHECK_EQUAL(expected, alignment);

      align_char_long data_long[2];
      CHECK((size_t(&data_long[0].value) % expected) == 0);
      CHECK((size_t(&data_long[1].value) % expected) == 0);

      // long long
      typedef etl::align_as<char, long long> align_char_long_long;
      alignment = etl::alignment_of<align_char_long_long>::value;
      expected  = std::alignment_of<long long>::value;
      CHECK_EQUAL(expected, alignment);

      align_char_long_long data_long_long[2];
      CHECK((size_t(&data_long_long[0].value) % expected) == 0);
      CHECK((size_t(&data_long_long[1].value) % expected) == 0);

      // double
      typedef etl::align_as<char, double> align_char_double;
      alignment = etl::alignment_of<align_char_double>::value;
      expected  = std::alignment_of<double>::value;
      CHECK_EQUAL(expected, alignment);

      align_char_double data_double[2];
      CHECK((size_t(&data_double[0].value) % expected) == 0);
      CHECK((size_t(&data_double[1].value) % expected) == 0);

      // struct Data
      typedef etl::align_as<char, Data> align_char_data;
      alignment = etl::alignment_of<align_char_data>::value;
      expected  = std::alignment_of<Data>::value;
      CHECK_EQUAL(expected, alignment);

      align_char_data data_data[2];
      CHECK((size_t(&data_data[0].value) % expected) == 0);
      CHECK((size_t(&data_data[1].value) % expected) == 0);
    }
  };
}
