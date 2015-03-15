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
#include <utility>
#include <string>
#include <ostream>

namespace
{
  SUITE(test_alignment)
  {
    //*************************************************************************
    TEST(test_aligned_storage)
    {
      size_t alignment;
      size_t expected;

      typedef etl::aligned_storage<sizeof(uint16_t), etl::alignment_of<uint32_t>::value>::type storage32_t;
      static storage32_t data32[10];

      alignment = etl::alignment_of<storage32_t>::value;
      expected  = std::alignment_of<uint32_t>::value;

      CHECK_EQUAL(expected, alignment);

      for (int i = 0; i < 10; ++i)
      {
        CHECK_EQUAL(0, size_t(&data32[1]) % expected);
      }
    }
  };
}
