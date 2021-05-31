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
  using MemCast = etl::mem_cast_ptr<Size>;
  
  char   c;
  double d;
  Data   data;

  std::aligned_storage_t<Size, Alignment> buffer;
  
  SUITE(test_mem_cast)
  {
    //*************************************************************************
    TEST(test_size)
    {
      MemCast memCast;

      CHECK(sizeof(char)  <= MemCast::Size);
      CHECK(sizeof(short) <= MemCast::Size);
      CHECK(sizeof(Data)  <= MemCast::Size);

      CHECK(sizeof(char)  <= memCast.Size);
      CHECK(sizeof(short) <= memCast.Size);
      CHECK(sizeof(Data)  <= memCast.Size);

      CHECK(sizeof(char)  <= memCast.size());
      CHECK(sizeof(short) <= memCast.size());
      CHECK(sizeof(Data)  <= memCast.size());
    }

    //*************************************************************************
    TEST(test_mem_cast_emplace_type)
    {
      char* pbuffer = reinterpret_cast<char*>(&buffer);

      MemCast memCast(pbuffer); 

      memCast.emplace<char>(123);
      CHECK_EQUAL(123, memCast.ref<char>());

      memCast.emplace<double>(1.23);
      CHECK_EQUAL(1.23, memCast.ref<double>());

      memCast.emplace<Data>(123, 1.23, std::array<int, 3>{ 1, 2, 3 });
      CHECK(123 == memCast.ref<Data>().c);
      CHECK(1.23 == memCast.ref<Data>().d);
      CHECK((std::array { 1, 2, 3 }) == memCast.ref<Data>().a);
    }

    //*************************************************************************
    TEST(test_mem_cast_to_type)
    {
      char* pbuffer = reinterpret_cast<char*>(&buffer);

      MemCast memCast(pbuffer); 

      pbuffer = memCast.data();
      *pbuffer = 123;
      CHECK_EQUAL(123, memCast.ref<char>());

      *reinterpret_cast<double*>(pbuffer) = 1.23;
      CHECK_EQUAL(1.23, memCast.ref<double>());

      *reinterpret_cast<Data*>(pbuffer) = { 123, 1.23, { 1, 2, 3 } };
      CHECK(123 == memCast.ref<Data>().c);
      CHECK(1.23 == memCast.ref<Data>().d);
      CHECK((std::array { 1, 2, 3 }) == memCast.ref<Data>().a);
    }

    //*************************************************************************
    TEST(test_const_mem_cast_to_type)
    {
      char* pbuffer = reinterpret_cast<char*>(&buffer);

      const MemCast memCast(pbuffer); 

      *pbuffer = 123;
      CHECK_EQUAL(123, memCast.ref<char>());

      *reinterpret_cast<double*>(pbuffer) = 1.23;
      CHECK_EQUAL(1.23, memCast.ref<double>());

      *reinterpret_cast<Data*>(pbuffer) = { 123, 1.23, { 1, 2, 3 } };
      CHECK(123 == memCast.ref<Data>().c);
      CHECK(1.23 == memCast.ref<Data>().d);
      CHECK((std::array { 1, 2, 3 }) == memCast.ref<Data>().a);
    }

    //*************************************************************************
    TEST(test_mem_cast_to_type_no_buffer)
    {
      MemCast memCast; 

      CHECK_THROW(memCast.ref<char>(), etl::mem_cast_nullptr_exception);
    }

    //*************************************************************************
    TEST(test_const_mem_cast_to_type_no_buffer)
    {
      const MemCast memCast; 

      CHECK_THROW(memCast.ref<char>(), etl::mem_cast_nullptr_exception);
    }

    //*************************************************************************
    TEST(test_mem_cast_to_type_misaligned_buffer)
    {
      double d;
      char* pbuffer = reinterpret_cast<char*>(&d);
      MemCast memCast(pbuffer + 1); 

      CHECK_THROW(memCast.ref<double>(), etl::mem_cast_alignment_exception);
    }

    //*************************************************************************
    TEST(test_const_mem_cast_to_type_misaligned_buffer)
    {
      double d;
      char* pbuffer = reinterpret_cast<char*>(&d);
      const MemCast memCast(pbuffer + 1); 

      CHECK_THROW(memCast.ref<double>(), etl::mem_cast_alignment_exception);
    }
  };
}
