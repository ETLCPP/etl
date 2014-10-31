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

#include "../largest.h"

#include <type_traits>

namespace
{		
  SUITE(TestLargest)
  {
    //*************************************************************************
    TEST(PODTest)
    {
      size_t size;
      bool   type;

      size = etl::largest<char, short, int>::size;
      type = std::is_same<int, etl::largest<char, short, int>::type>::value;

      CHECK_EQUAL(sizeof(int), size);
      CHECK(type);

      size = etl::largest<int, char, short>::size;
      type = std::is_same<int, etl::largest<char, short, int>::type>::value;

      CHECK_EQUAL(sizeof(int), size);
      CHECK(type);
    }

    //*************************************************************************
    TEST(NonPODTest)
    {
      size_t size;
      bool   type;

      struct S1 { char a; char  b; char c; };
      struct S2 { char a; short b; char c; };
      struct S3 { int  a; short b; char c; };

      size_t a = sizeof(S1);
      size_t b = sizeof(S2);
      size_t c = sizeof(S3);

      size = etl::largest<S1, S2, S3>::size;
      type = std::is_same<S3, etl::largest<S1, S2, S3>::type>::value;

      CHECK_EQUAL(sizeof(S3), size);
      CHECK(type);

      size = etl::largest<S2, S3, S1>::size;
      type = std::is_same<S3, etl::largest<S2, S3, S1>::type>::value;

      CHECK_EQUAL(sizeof(S3), size);
      CHECK(type);
    }
  };
}
