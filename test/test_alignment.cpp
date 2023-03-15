/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl
https://www.etlcpp.com

Copyright(c) 2014 John Wellbelove

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

#include "etl/alignment.h"
#include "etl/type_traits.h"

#include <type_traits>
#include <utility>
#include <string>
#include <ostream>

#if defined(ETL_COMPILER_MICROSOFT)
  #pragma warning(disable : 4996)
#endif

#ifdef ETL_COMPILER_GCC
  #pragma GCC diagnostic ignored "-Wdeprecated-declarations"
#endif

void f(int)
{
}

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
        CHECK_EQUAL(0U, size_t(&data32[1]) % expected);
      }

      etl::aligned_storage<100, 8>::type data9;
      f(static_cast<int>(data9));
    }

    //*************************************************************************
    TEST(test_aligned_storage_conversion_operators)
    {
      typedef etl::aligned_storage<sizeof(uint32_t), etl::alignment_of<uint32_t>::value>::type storage32_t;
      static storage32_t data;

      void* pdata = &data.data;

      uint32_t& ref        = data;
      const uint32_t& cref = data;
      CHECK(&ref  == pdata);
      CHECK(&cref == pdata);

      uint32_t* ptr        = data;
      const uint32_t* cptr = data;
      CHECK(ptr  == pdata);
      CHECK(cptr == pdata);

      uint32_t& ref2        = data.get_reference<uint32_t>();
      const uint32_t& cref2 = data.get_reference<const uint32_t>();
      CHECK(&ref2  == pdata);
      CHECK(&cref2 == pdata);
      
      uint32_t* ptr2        = data.get_address<uint32_t>();
      const uint32_t* cptr2 = data.get_address<const uint32_t>();
      CHECK(ptr2  == pdata);
      CHECK(cptr2 == pdata);
    }

    //*************************************************************************
    TEST(test_aligned_storage_as)
    {
      size_t alignment;
      size_t expected;

      typedef etl::aligned_storage_as<sizeof(uint16_t), uint32_t>::type storage32_t;
      static storage32_t data32[10];

      alignment = etl::alignment_of<storage32_t>::value;
      expected = std::alignment_of<uint32_t>::value;

      CHECK_EQUAL(expected, alignment);

      for (int i = 0; i < 10; ++i)
      {
        CHECK_EQUAL(0U, size_t(&data32[1]) % expected);
      }

      etl::aligned_storage<100, 8>::type data9;
      f(static_cast<int>(data9));
    }

    //*************************************************************************
    TEST(test_is_aligned_tests)
    {
      alignas(uint32_t) char buffer[2U * sizeof(uint32_t)];

      char* p = buffer;

      CHECK_TRUE(etl::is_aligned(p, std::alignment_of<uint32_t>()));
      CHECK_TRUE(etl::is_aligned<alignof(uint32_t)>(p));
      CHECK_TRUE(etl::is_aligned<uint32_t>(p));

      ++p;
      CHECK_FALSE(etl::is_aligned(p, std::alignment_of<uint32_t>()));
      CHECK_FALSE(etl::is_aligned<alignof(uint32_t)>(p));
      CHECK_FALSE(etl::is_aligned<uint32_t>(p));
    }
  };
}
