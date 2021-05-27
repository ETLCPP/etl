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

#include "etl/experimental/mem_cast.h"
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
    double d;
    std::array<int, 3> a;
  };

  constexpr size_t Size      = etl::largest<char, double, Data>::size;
  constexpr size_t Alignment = etl::largest<char, double, Data>::alignment;

  // Test variant types.
  using MemCast      = etl::mem_cast<Size, Alignment>;
  using MemCastTypes = etl::mem_cast_types<char, double, Data>;

  char   c;
  double d;
  Data   data;

  SUITE(test_mem_cast)
  {
    //*************************************************************************
    TEST(test_size)
    {
      MemCast      memCast;
      MemCastTypes memCastTypes;

      CHECK(sizeof(char)  <= MemCast::Size);
      CHECK(sizeof(short) <= MemCast::Size);
      CHECK(sizeof(Data)  <= MemCast::Size);

      CHECK(sizeof(char)  <= memCast.Size);
      CHECK(sizeof(short) <= memCast.Size);
      CHECK(sizeof(Data)  <= memCast.Size);

      CHECK(sizeof(char)  <= memCast.size());
      CHECK(sizeof(short) <= memCast.size());
      CHECK(sizeof(Data)  <= memCast.size());

      CHECK(sizeof(char)  <= MemCastTypes::Size);
      CHECK(sizeof(short) <= MemCastTypes::Size);
      CHECK(sizeof(Data)  <= MemCastTypes::Size);

      CHECK(sizeof(char)  <= memCastTypes.Size);
      CHECK(sizeof(short) <= memCastTypes.Size);
      CHECK(sizeof(Data)  <= memCastTypes.Size);

      CHECK(sizeof(char)  <= memCastTypes.size());
      CHECK(sizeof(short) <= memCastTypes.size());
      CHECK(sizeof(Data)  <= memCastTypes.size());
    }

    //*************************************************************************
    TEST(test_alignment)
    {
      MemCast      memCast;
      MemCastTypes memCastTypes;

      CHECK(alignof(char)  <= MemCast::Alignment);
      CHECK(alignof(short) <= MemCast::Alignment);
      CHECK(alignof(Data)  <= MemCast::Alignment);

      CHECK(alignof(char)  <= memCast.Alignment);
      CHECK(alignof(short) <= memCast.Alignment);
      CHECK(alignof(Data)  <= memCast.Alignment);

      CHECK(alignof(char)  <= memCast.alignment());
      CHECK(alignof(short) <= memCast.alignment());
      CHECK(alignof(Data)  <= memCast.alignment());

      CHECK(alignof(char)  <= MemCastTypes::Alignment);
      CHECK(alignof(short) <= MemCastTypes::Alignment);
      CHECK(alignof(Data)  <= MemCastTypes::Alignment);

      CHECK(alignof(char)  <= memCastTypes.Alignment);
      CHECK(alignof(short) <= memCastTypes.Alignment);
      CHECK(alignof(Data)  <= memCastTypes.Alignment);

      CHECK(alignof(char)  <= memCastTypes.alignment());
      CHECK(alignof(short) <= memCastTypes.alignment());
      CHECK(alignof(Data)  <= memCastTypes.alignment());
    }

    //*************************************************************************
    TEST(test_mem_cast_to_type)
    {
      MemCast memCast;

      char* pbuffer = memCast.data();
      *pbuffer = 123;
      CHECK_EQUAL(123, memCast.get<char>());

      *reinterpret_cast<double*>(pbuffer) = 1.23;
      CHECK_EQUAL(1.23, memCast.get<double>());

      *reinterpret_cast<Data*>(pbuffer) = { 123, 1.23, { 1, 2, 3 } };
      CHECK(123 == memCast.get<Data>().c);
      CHECK(1.23 == memCast.get<Data>().d);
      CHECK((std::array { 1, 2, 3 }) == memCast.get<Data>().a);
    }
  };
}
