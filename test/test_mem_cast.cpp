/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl
https://www.etlcpp.com

Copyright(c) 2021 John Wellbelove

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

#include "etl/mem_cast.h"
#include "etl/largest.h"

#include <array>
#include <vector>
#include <algorithm>
#include <string>

namespace
{
  struct Data
  {
    Data()
      : c(0)
      , d(0)
      , a()
    {
    }

    Data(char c_, double d_, std::array<int, 3> a_)
      : c(c_)
      , d(d_)
      , a(a_)
    {
    }

    char c;
    double d;
    std::array<int, 3> a;
  };

  constexpr size_t Size      = etl::largest<char, double, Data>::size;
  constexpr size_t Alignment = etl::largest<char, double, Data>::alignment;

  // Test variant types.
  using MemCast      = etl::mem_cast<Size, Alignment>;
  using MemCastTypes = etl::mem_cast_types<char, double, Data>;

  Data data;

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
    TEST(test_mem_cast_assign_type)
    {
      MemCast memCast;

      memCast.assign<char>(123);
      CHECK_EQUAL(123, memCast.ref<char>());

      memCast.assign<double>(1.23);
      CHECK_EQUAL(1.23, memCast.ref<double>());

      Data data(123, 1.23, std::array<int, 3>{ 1, 2, 3 });
      memCast.assign<Data>(data);
      CHECK_EQUAL(123,  memCast.ref<Data>().c);
      CHECK_EQUAL(1.23, memCast.ref<Data>().d);
      CHECK_EQUAL(1,    memCast.ref<Data>().a[0]);
      CHECK_EQUAL(2,    memCast.ref<Data>().a[1]);
      CHECK_EQUAL(3,    memCast.ref<Data>().a[2]);
    }

    //*************************************************************************
    TEST(test_mem_cast_assign_type_at_dynamic_offset)
    {
      MemCast memCast; 

      using Array = std::array<int, 3>;
      Array a{ 1, 2, 3 };

      size_t offset = offsetof(Data, a);

      memCast.assign_at_offset<Array>(offset, a);
      CHECK_EQUAL(a[0], memCast.ref<Data>().a[0]);
      CHECK_EQUAL(a[1], memCast.ref<Data>().a[1]);
      CHECK_EQUAL(a[2], memCast.ref<Data>().a[2]);
    }

    //*************************************************************************
    TEST(test_mem_cast_assign_type_at_static_offset)
    {
      MemCast memCast; 

      using Array = std::array<int, 3>;
      Array a{ 1, 2, 3 };

      constexpr size_t Offset = offsetof(Data, a);

      memCast.assign_at_offset<Array, Offset>(a);
      CHECK_EQUAL(a[0], memCast.ref<Data>().a[0]);
      CHECK_EQUAL(a[1], memCast.ref<Data>().a[1]);
      CHECK_EQUAL(a[2], memCast.ref<Data>().a[2]);
    }

    //*************************************************************************
    TEST(test_mem_cast_emplace_type)
    {
      MemCast memCast; 

      memCast.emplace<char>(123);
      CHECK_EQUAL(123, memCast.ref<char>());

      memCast.emplace<double>(1.23);
      CHECK_EQUAL(1.23, memCast.ref<double>());

      memCast.emplace<Data>(123, 1.23, std::array<int, 3>{ 1, 2, 3 });
      CHECK_EQUAL(123,  memCast.ref<Data>().c);
      CHECK_EQUAL(1.23, memCast.ref<Data>().d);
      CHECK_EQUAL(1,    memCast.ref<Data>().a[0]);
      CHECK_EQUAL(2,    memCast.ref<Data>().a[1]);
      CHECK_EQUAL(3,    memCast.ref<Data>().a[2]);
    }

    //*************************************************************************
    TEST(test_mem_cast_emplace_type_at_dynamic_offset)
    {
      MemCast memCast; 

      using Array = std::array<int, 3>;
      Array a{ 1, 2, 3 };

      size_t offset = offsetof(Data, a);

      memCast.emplace_at_offset<Array>(offset, a);
      CHECK_EQUAL(a[0], memCast.ref<Data>().a[0]);
      CHECK_EQUAL(a[1], memCast.ref<Data>().a[1]);
      CHECK_EQUAL(a[2], memCast.ref<Data>().a[2]);
    }

    //*************************************************************************
    TEST(test_mem_cast_emplace_type_at_static_offset)
    {
      MemCast memCast; 

      using Array = std::array<int, 3>;
      Array a{ 1, 2, 3 };

      constexpr size_t Offset = offsetof(Data, a);

      memCast.emplace_at_offset<Array, Offset>(a);
      CHECK_EQUAL(a[0], memCast.ref<Data>().a[0]);
      CHECK_EQUAL(a[1], memCast.ref<Data>().a[1]);
      CHECK_EQUAL(a[2], memCast.ref<Data>().a[2]);
    }

    //*************************************************************************
    TEST(test_mem_cast_to_type)
    {
      MemCast memCast; 

      char* pbuffer = memCast.data();
      *pbuffer = 123;
      CHECK_EQUAL(123, memCast.ref<char>());

      *reinterpret_cast<double*>(pbuffer) = 1.23;
      CHECK_EQUAL(1.23, memCast.ref<double>());

      *reinterpret_cast<Data*>(pbuffer) = { 123, 1.23, { 1, 2, 3 } };
      CHECK(123 == memCast.ref<Data>().c);
      CHECK(1.23 == memCast.ref<Data>().d);
      CHECK_EQUAL(1, memCast.ref<Data>().a[0]);
      CHECK_EQUAL(2, memCast.ref<Data>().a[1]);
      CHECK_EQUAL(3, memCast.ref<Data>().a[2]);
    }

    //*************************************************************************
    TEST(test_const_mem_cast_to_type)
    {
      MemCast memCast;
      const MemCast& memCastRef = memCast;

      char* pbuffer = memCast.data();
      *pbuffer = 123;
      CHECK_EQUAL(123, memCastRef.ref<char>());

      *reinterpret_cast<double*>(pbuffer) = 1.23;
      CHECK_EQUAL(1.23, memCastRef.ref<double>());

      *reinterpret_cast<Data*>(pbuffer) = { 123, 1.23, { 1, 2, 3 } };
      CHECK(123 == memCastRef.ref<Data>().c);
      CHECK(1.23 == memCastRef.ref<Data>().d);
      CHECK_EQUAL(1, memCast.ref<Data>().a[0]);
      CHECK_EQUAL(2, memCast.ref<Data>().a[1]);
      CHECK_EQUAL(3, memCast.ref<Data>().a[2]);
    }

    //*************************************************************************
    TEST(test_mem_cast_types_to_type)
    {
      MemCastTypes memCastTypes;

      char* pbuffer = memCastTypes.data();
      *pbuffer = 123;
      CHECK_EQUAL(123, memCastTypes.ref<char>());

      *reinterpret_cast<double*>(pbuffer) = 1.23;
      CHECK_EQUAL(1.23, memCastTypes.ref<double>());

      *reinterpret_cast<Data*>(pbuffer) = { 123, 1.23, { 1, 2, 3 } };
      CHECK(123 == memCastTypes.ref<Data>().c);
      CHECK(1.23 == memCastTypes.ref<Data>().d);
      CHECK_EQUAL(1, memCastTypes.ref<Data>().a[0]);
      CHECK_EQUAL(2, memCastTypes.ref<Data>().a[1]);
      CHECK_EQUAL(3, memCastTypes.ref<Data>().a[2]);
    }
  };
}
