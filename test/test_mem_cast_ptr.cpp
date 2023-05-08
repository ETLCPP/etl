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
  using MemCast = etl::mem_cast_ptr;
  
  Data   data;

  char* Ptr(int i)
  {
    return reinterpret_cast<char*>(uintptr_t(i));
  }

  typename std::aligned_storage<Size, Alignment>::type buffer;
  
  SUITE(test_mem_cast_ptr)
  {
    //*************************************************************************
    TEST(test_alignment)
    {
      MemCast memCast;

      memCast.data(Ptr(1));
      CHECK_EQUAL(1U , memCast.alignment());

      memCast.data(Ptr(2));
      CHECK_EQUAL(2U , memCast.alignment());

      memCast.data(Ptr(3));
      CHECK_EQUAL(1U , memCast.alignment());

      memCast.data(Ptr(4));
      CHECK_EQUAL(4U , memCast.alignment());

      memCast.data(Ptr(5));
      CHECK_EQUAL(1U , memCast.alignment());

      memCast.data(Ptr(6));
      CHECK_EQUAL(2U , memCast.alignment());

      memCast.data(Ptr(7));
      CHECK_EQUAL(1U , memCast.alignment());

      memCast.data(Ptr(8));
      CHECK_EQUAL(8U , memCast.alignment());

      memCast.data(Ptr(9));
      CHECK_EQUAL(1U , memCast.alignment());

      memCast.data(Ptr(10));
      CHECK_EQUAL(2U , memCast.alignment());

      memCast.data(Ptr(11));
      CHECK_EQUAL(1U , memCast.alignment());

      memCast.data(Ptr(12));
      CHECK_EQUAL(4U , memCast.alignment());

      memCast.data(Ptr(13));
      CHECK_EQUAL(1U , memCast.alignment());

      memCast.data(Ptr(14));
      CHECK_EQUAL(2U , memCast.alignment());

      memCast.data(Ptr(15));
      CHECK_EQUAL(1U , memCast.alignment());

      memCast.data(Ptr(16));
      CHECK_EQUAL(16U , memCast.alignment());

      memCast.data(Ptr(17));
      CHECK_EQUAL(1U , memCast.alignment());

      memCast.data(Ptr(18));
      CHECK_EQUAL(2U , memCast.alignment());

      memCast.data(Ptr(19));
      CHECK_EQUAL(1U , memCast.alignment());

      memCast.data(Ptr(20));
      CHECK_EQUAL(4U , memCast.alignment());

      memCast.data(Ptr(21));
      CHECK_EQUAL(1U , memCast.alignment());

      memCast.data(Ptr(22));
      CHECK_EQUAL(2U , memCast.alignment());

      memCast.data(Ptr(23));
      CHECK_EQUAL(1U , memCast.alignment());

      memCast.data(Ptr(24));
      CHECK_EQUAL(8U , memCast.alignment());

      memCast.data(Ptr(25));
      CHECK_EQUAL(1U , memCast.alignment());

      memCast.data(Ptr(26));
      CHECK_EQUAL(2U , memCast.alignment());

      memCast.data(Ptr(27));
      CHECK_EQUAL(1U , memCast.alignment());

      memCast.data(Ptr(28));
      CHECK_EQUAL(4U , memCast.alignment());

      memCast.data(Ptr(29));
      CHECK_EQUAL(1U , memCast.alignment());

      memCast.data(Ptr(30));
      CHECK_EQUAL(2U , memCast.alignment());

      memCast.data(Ptr(31));
      CHECK_EQUAL(1U , memCast.alignment());

      memCast.data(Ptr(32));
      CHECK_EQUAL(32U , memCast.alignment());
    }

    //*************************************************************************
    TEST(test_mem_cast_assign_type)
    {
      char* pbuffer = reinterpret_cast<char*>(&buffer);

      MemCast memCast(pbuffer); 

      memCast.assign<char>(123);
      CHECK_EQUAL(123, memCast.ref<char>());

      memCast.assign<double>(1.23);
      CHECK_EQUAL(1.23, memCast.ref<double>());

      Data data(123, 1.23, std::array<int, 3>{ 1, 2, 3 });
      memCast.assign<Data>(data);
      CHECK(123 == memCast.ref<Data>().c);
      CHECK(1.23 == memCast.ref<Data>().d);
//      CHECK((std::array { 1, 2, 3 }) == memCast.ref<Data>().a);
    }

    //*************************************************************************
    TEST(test_mem_cast_assign_type_at_dynamic_offset)
    {
      char* pbuffer = reinterpret_cast<char*>(&buffer);

      MemCast memCast(pbuffer); 

      using Array = std::array<int, 3>;
      Array a{ 1, 2, 3 };

      size_t offset = offsetof(Data, a);

      memCast.assign_at_offset<Array>(offset, a);
      CHECK(a == memCast.ref<Data>().a);
    }

    //*************************************************************************
    TEST(test_mem_cast_assign_type_at_static_offset)
    {
      char* pbuffer = reinterpret_cast<char*>(&buffer);

      MemCast memCast(pbuffer); 

      using Array = std::array<int, 3>;
      Array a{ 1, 2, 3 };

      constexpr size_t Offset = offsetof(Data, a);

      memCast.assign_at_offset<Array, Offset>(a);
      CHECK(a == memCast.ref<Data>().a);
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
//      CHECK((std::array { 1, 2, 3 }) == memCast.ref<Data>().a);
    }

    //*************************************************************************
    TEST(test_mem_cast_emplace_type_at_dynamic_offset)
    {
      char* pbuffer = reinterpret_cast<char*>(&buffer);

      MemCast memCast(pbuffer); 

      using Array = std::array<int, 3>;
      Array a{ 1, 2, 3 };

      size_t offset = offsetof(Data, a);

      memCast.emplace_at_offset<Array>(offset, a);
      CHECK(a == memCast.ref<Data>().a);
    }

    //*************************************************************************
    TEST(test_mem_cast_emplace_type_at_static_offset)
    {
      char* pbuffer = reinterpret_cast<char*>(&buffer);

      MemCast memCast(pbuffer);

      using Array = std::array<int, 3>;
      Array a{ 1, 2, 3 };

      constexpr size_t Offset = offsetof(Data, a);

      memCast.emplace_at_offset<Array, Offset>(a);
      CHECK(a == memCast.ref<Data>().a);
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
//      CHECK((std::array { 1, 2, 3 }) == memCast.ref<Data>().a);
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
//      CHECK((std::array { 1, 2, 3 }) == memCast.ref<Data>().a);
    }

    //*************************************************************************
    TEST(test_mem_cast_to_type_no_buffer)
    {
      MemCast memCast; 
      char c = 0;

      CHECK_THROW(c = memCast.ref<char>(), etl::mem_cast_nullptr_exception);
      CHECK(c == 0);
    }

    //*************************************************************************
    TEST(test_const_mem_cast_to_type_no_buffer)
    {
      const MemCast memCast; 
      char c = 0;
      
      CHECK_THROW(c = memCast.ref<char>(), etl::mem_cast_nullptr_exception);
      CHECK(c == 0);
    }
  };
}
