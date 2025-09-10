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

// Demonstrator class for etl::typed_storage tests
struct A_t
{
  A_t(uint32_t v_x, uint8_t v_y)
   : x(v_x)
   , y(v_y)
  {
  }

  // Just for test purpose. In production code, etl::typed_storage
  // actually supports the use case of destructors being optimized
  // away since they are not necessary for global objects that are
  // never destroyed
  ~A_t()
  {
    x = 0;
    y = 0;
  }

  // etl::typed_storage helps implementing the use case of becoming
  // independent of the destructor. By deleting the assignment operator,
  // we make sure that the destructor is not linked
  A_t& operator=(const A_t&) = delete;

  bool operator==(const A_t& other) const
  {
    return other.x == x && other.y == y;
  }

  uint32_t x;
  uint8_t y;
};

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

    //*************************************************************************
    TEST(test_is_aligned_tests_const)
    {
      alignas(uint32_t) const char buffer[2U * sizeof(uint32_t)] = {0, 1};

      const char* p = buffer;

      CHECK_TRUE(etl::is_aligned(p, std::alignment_of<const uint32_t>()));
      CHECK_TRUE(etl::is_aligned<alignof(const uint32_t)>(p));
      CHECK_TRUE(etl::is_aligned<const uint32_t>(p));

      ++p;
      CHECK_FALSE(etl::is_aligned(p, std::alignment_of<const uint32_t>()));
      CHECK_FALSE(etl::is_aligned<alignof(const uint32_t)>(p));
      CHECK_FALSE(etl::is_aligned<const uint32_t>(p));
    }

    //*************************************************************************
    TEST(test_type_with_alignment)
    {
      CHECK_EQUAL(1,  alignof(etl::type_with_alignment_t<1>));
      CHECK_EQUAL(2,  alignof(etl::type_with_alignment_t<2>));
      CHECK_EQUAL(4,  alignof(etl::type_with_alignment_t<4>));
      CHECK_EQUAL(8,  alignof(etl::type_with_alignment_t<8>));
      CHECK_EQUAL(16, alignof(etl::type_with_alignment_t<16>));
      CHECK_EQUAL(32, alignof(etl::type_with_alignment_t<32>));
      CHECK_EQUAL(64, alignof(etl::type_with_alignment_t<64>));
    }

    //*************************************************************************
    TEST(test_typed_storage)
    {
      etl::typed_storage<A_t> a;
      CHECK_FALSE(a.has_value());

      // Construct in place.
      etl::typed_storage<A_t> b(789, 10);  
      CHECK_TRUE(b.has_value());
      CHECK_EQUAL(b->x, 789);
      CHECK_EQUAL(b->y, 10);
      
      // Create in place.
      auto& ref = a.create(123, 4);
      CHECK_TRUE(a.has_value());

      CHECK_EQUAL(a->x, 123);
      CHECK_EQUAL(a->y, 4);

      CHECK_EQUAL(ref.x, 123);
      CHECK_EQUAL(ref.y, 4);

      CHECK_TRUE(*a == ref);

      // Destroy
      CHECK_TRUE(a.has_value());
      a.destroy();
      CHECK_FALSE(a.has_value());
    }

    //*************************************************************************
    TEST(test_typed_storage_ext)
    {
      alignas(A_t) char buffer1[sizeof(A_t)] = {0};
      alignas(A_t) char buffer2[sizeof(A_t)] = {0};

      // Construct.
      etl::typed_storage_ext<A_t> a(buffer1);
      CHECK_FALSE(a.has_value());

      // Construct in place.
      etl::typed_storage_ext<A_t> b(buffer2, 789, 10);
      CHECK_TRUE(b.has_value());
      CHECK_EQUAL(b->x, 789);
      CHECK_EQUAL(b->y, 10);

      // Create in place.
      auto& ref = a.create(123, 4);  
      CHECK_EQUAL(ref.x, 123);
      CHECK_EQUAL(ref.y, 4);

      CHECK_TRUE(a.has_value());
      CHECK_EQUAL(a->x, 123);
      CHECK_EQUAL(a->y, 4);
      CHECK_TRUE(*a == ref);

      // Swap
      etl::swap(a, b);
      CHECK_TRUE(a.has_value());
      CHECK_EQUAL(a->x, 789);
      CHECK_EQUAL(a->y, 10);

      CHECK_TRUE(b.has_value());
      CHECK_EQUAL(b->x, 123);
      CHECK_EQUAL(b->y, 4);

      // Move contruct
      etl::typed_storage_ext<A_t> c(etl::move(a));
      CHECK_FALSE(a.has_value());
      CHECK_TRUE(c.has_value());
      CHECK_EQUAL(c->x, 789);
      CHECK_EQUAL(c->y, 10);

      // Destroy
      CHECK_TRUE(c.has_value());
      c.destroy();
      CHECK_FALSE(c.has_value());
    }
  };
}
