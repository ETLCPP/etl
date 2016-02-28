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

#include "../src/largest.h"

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
      type = std::is_same<int, etl::largest_type<char, short, int>::type>::value;

      CHECK_EQUAL(sizeof(int), size);
      CHECK(type);

      size = etl::largest_type<int, char, short>::size;
      type = std::is_same<int, etl::largest_type<char, short, int>::type>::value;

      CHECK_EQUAL(sizeof(int), size);
      CHECK(type);
    }

    //*************************************************************************
    TEST(test_non_pod_type)
    {
      size_t size;
      bool   type;

      struct S1 { char a; char  b; char c; };
      struct S2 { char a; short b; char c; };
      struct S3 { int  a; short b; char c; };

      size = etl::largest_type<S1, S2, S3>::size;
      type = std::is_same<S3, etl::largest_type<S1, S2, S3>::type>::value;

      CHECK_EQUAL(sizeof(S3), size);
      CHECK(type);

      size = etl::largest_type<S2, S3, S1>::size;
      type = std::is_same<S3, etl::largest_type<S2, S3, S1>::type>::value;

      CHECK_EQUAL(sizeof(S3), size);
      CHECK(type);
    }

    //*************************************************************************
    TEST(test_pod_alignment)
    {
      size_t size = etl::largest_alignment<char, short, int, double>::value;

      CHECK_EQUAL(std::alignment_of<double>::value, size);
    }

    //*************************************************************************
    TEST(test_non_pod_alignment)
    {
      struct S1 { char a; char  b; char c; };
      struct S2 { char a; short b; char c; };
      struct S3 { int  a; short b; char c; };
      
      size_t size = etl::largest_alignment<S1, S2, S3>::value;

      CHECK_EQUAL(std::alignment_of<S3>::value, size);
    }
  };
}
