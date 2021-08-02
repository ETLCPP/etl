/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl
https://www.etlcpp.com

Copyright(c) 2021 jwellbelove

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

#include "etl/experimental/mem_type.h"
#include "etl/largest.h"

#include <array>
#include <vector>
#include <algorithm>
#include <string>

namespace
{
  struct Data
  {
    char c;
    short s;
    std::array<int, 10> a;
  };

  // Test variant types.
  using MemType = etl::mem_type<etl::largest<char, double, Data>::size, 
                                etl::largest<char, double, Data>::alignment>;

  SUITE(test_mem_type)
  {
    TEST(test_alignment)
    {
      MemType memType;

      CHECK(alignof(char)  <= MemType::Alignment);
      CHECK(alignof(short) <= MemType::Alignment);
      CHECK(alignof(Data)  <= MemType::Alignment);

      CHECK(alignof(char)  <= memType.Alignment);
      CHECK(alignof(short) <= memType.Alignment);
      CHECK(alignof(Data)  <= memType.Alignment);

      CHECK(alignof(char)  <= memType.alignment());
      CHECK(alignof(short) <= memType.alignment());
      CHECK(alignof(Data)  <= memType.alignment());
    }

    TEST(test_size)
    {
      MemType memType;

      CHECK(sizeof(char)  <= MemType::Size);
      CHECK(sizeof(short) <= MemType::Size);
      CHECK(sizeof(Data)  <= MemType::Size);

      CHECK(sizeof(char)  <= memType.Size);
      CHECK(sizeof(short) <= memType.Size);
      CHECK(sizeof(Data)  <= memType.Size);

      CHECK(sizeof(char)  <= memType.size());
      CHECK(sizeof(short) <= memType.size());
      CHECK(sizeof(Data)  <= memType.size());
    }
  };
}
