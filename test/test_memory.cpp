/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl
http://www.etlcpp.com

Copyright(c) 2017 jwellbelove

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

#include "UnitTest++.h"

#include "memory.h"
#include "debug_count.h"

#include <string>
#include <array>
#include <algorithm>
#include <iterator>
#include <stdint.h>

#include <vector>

namespace
{
  typedef std::string non_trivial_t;
  typedef uint32_t    trivial_t;

  const size_t SIZE = 10;

  std::array<non_trivial_t, SIZE> test_data_non_trivial =
  {
    "one", "two",   "three", "four", "five",
    "six", "seven", "eight", "nine", "ten"
  };

  std::array<trivial_t, SIZE> test_data_trivial =
  {
    0x11223344, 0x22334455, 0x33445566, 0x44556677, 0x55667788,
    0x66778899, 0x778899AA, 0x8899AABB, 0x99AABBCC, 0xAABBCCDD
  };

  non_trivial_t test_item_non_trivial("eleven");
  non_trivial_t test_item_non_trivial_null("");
  trivial_t test_item_trivial(0xBBCCDDEE);

  char buffer_non_trivial[sizeof(non_trivial_t) * SIZE];
  char buffer_trivial[sizeof(trivial_t) * SIZE];

  non_trivial_t* output_non_trivial = reinterpret_cast<non_trivial_t*>(buffer_non_trivial);
  trivial_t*    output_trivial      = reinterpret_cast<trivial_t*>(buffer_trivial);

  struct overloaded
  {
    overloaded* operator&()
    {
      return nullptr;
    }
  };
}

namespace
{
  SUITE(test_memory)
  {
    //*************************************************************************
    TEST(test_addressof)
    {
      int i;
      CHECK(&i == etl::addressof(i));

      overloaded ol;
      CHECK(&ol != etl::addressof(ol));
      CHECK(reinterpret_cast<overloaded*>(&reinterpret_cast<char&>(ol)) == etl::addressof(ol));
    }

    //*************************************************************************
    TEST(test_create_destroy_trivial)
    {
      char n[sizeof(trivial_t)];
      trivial_t* pn = reinterpret_cast<trivial_t*>(n);

      // Non count.
      std::fill(std::begin(n), std::end(n), 0xFF);
      etl::create_default_at(pn);
      CHECK_EQUAL(0xFFFFFFFF, *pn);
      etl::destroy_at(pn);
      CHECK_EQUAL(0xFFFFFFFF, *pn);

      std::fill(std::begin(n), std::end(n), 0xFF);
      etl::create_value_at(pn);
      CHECK_EQUAL(0x0000000U, *pn);
      etl::destroy_at(pn);
      CHECK_EQUAL(0x0000000U, *pn);

      std::fill(std::begin(n), std::end(n), 0xFF);
      etl::create_copy_at(pn, test_item_trivial);
      CHECK_EQUAL(test_item_trivial, *pn);
      etl::destroy_at(pn);
      CHECK_EQUAL(test_item_trivial, *pn);

      // Count.
      size_t count = 0;

      std::fill(std::begin(n), std::end(n), 0xFF);
      etl::create_default_at(pn, count);
      CHECK_EQUAL(0xFFFFFFFF, *pn);
      CHECK_EQUAL(1U, count);
      etl::destroy_at(pn, count);
      CHECK_EQUAL(0xFFFFFFFF, *pn);
      CHECK_EQUAL(0U, count);

      std::fill(std::begin(n), std::end(n), 0xFF);
      etl::create_value_at(pn, count);
      CHECK_EQUAL(0x0000000U, *pn);
      CHECK_EQUAL(1U, count);
      etl::destroy_at(pn, count);
      CHECK_EQUAL(0x0000000U, *pn);
      CHECK_EQUAL(0U, count);

      std::fill(std::begin(n), std::end(n), 0xFF);
      etl::create_copy_at(pn, test_item_trivial, count);
      CHECK_EQUAL(test_item_trivial, *pn);
      CHECK_EQUAL(1U, count);
      etl::destroy_at(pn, count);
      CHECK_EQUAL(test_item_trivial, *pn);
      CHECK_EQUAL(0U, count);
    }

    //*************************************************************************
    TEST(test_create_destroy_non_trivial)
    {
      char n[sizeof(non_trivial_t)];
      non_trivial_t* pn = reinterpret_cast<non_trivial_t*>(n);

      // Non count.
      std::fill(std::begin(n), std::end(n), 0xFF);
      etl::create_default_at(pn);
      CHECK_EQUAL(test_item_non_trivial_null, *pn);
      etl::destroy_at(pn);

      std::fill(std::begin(n), std::end(n), 0xFF);
      etl::create_value_at(pn);
      CHECK_EQUAL(test_item_non_trivial_null, *pn);
      etl::destroy_at(pn);

      std::fill(std::begin(n), std::end(n), 0xFF);
      etl::create_copy_at(pn, test_item_non_trivial);
      CHECK_EQUAL(test_item_non_trivial, *pn);
      etl::destroy_at(pn);

      // Count.
      size_t count = 0;

      std::fill(std::begin(n), std::end(n), 0xFF);
      etl::create_default_at(pn, count);
      CHECK_EQUAL(test_item_non_trivial_null, *pn);
      CHECK_EQUAL(1U, count);
      etl::destroy_at(pn, count);
      CHECK_EQUAL(0U, count);

      count = 0;
      std::fill(std::begin(n), std::end(n), 0xFF);
      etl::create_value_at(pn, count);
      CHECK_EQUAL(test_item_non_trivial_null, *pn);
      CHECK_EQUAL(1U, count);
      etl::destroy_at(pn, count);
      CHECK_EQUAL(0U, count);

      count = 0;
      std::fill(std::begin(n), std::end(n), 0xFF);
      etl::create_copy_at(pn, test_item_non_trivial, count);
      CHECK_EQUAL(test_item_non_trivial, *pn);
      CHECK_EQUAL(1U, count);
      etl::destroy_at(pn, count);
      CHECK_EQUAL(0U, count);
    }

    //*************************************************************************
    TEST(test_uninitialized_fill_n_trivial)
    {
      // Also tests uninitialized_fill.

      // Non count.
      trivial_t* p = reinterpret_cast<trivial_t*>(buffer_trivial);

      std::fill(std::begin(buffer_trivial), std::end(buffer_trivial), 0);
      etl::uninitialized_fill_n(p, SIZE, test_item_trivial);

      trivial_t* result;

      result = std::find_if_not(output_trivial, output_trivial + SIZE, [](trivial_t i) { return i == test_item_trivial; });

      CHECK(result == output_trivial + SIZE);
      etl::destroy(p, p + SIZE);

      // Count.
      size_t count = 0;
      std::fill(std::begin(buffer_trivial), std::end(buffer_trivial), 0);
      etl::uninitialized_fill_n(p, SIZE, test_item_trivial, count);

      result = std::find_if_not(output_trivial, output_trivial + SIZE, [](trivial_t i) { return i == test_item_trivial; });

      CHECK(result == output_trivial + SIZE);
      CHECK_EQUAL(SIZE, count);

      etl::destroy(p, p + SIZE, count);
      CHECK_EQUAL(0U, count);
    }

    //*************************************************************************
    TEST(test_uninitialized_fill_n_non_trivial)
    {
      // Also tests uninitialized_fill.

      // Non count.
      non_trivial_t* p = reinterpret_cast<non_trivial_t*>(buffer_non_trivial);

      std::fill(std::begin(buffer_non_trivial), std::end(buffer_non_trivial), 0);
      etl::uninitialized_fill_n(p, SIZE, test_item_non_trivial);

      non_trivial_t* result;

      result = std::find_if_not(output_non_trivial, output_non_trivial + SIZE, [](const non_trivial_t& i) { return i == test_item_non_trivial; });

      CHECK(result == output_non_trivial + SIZE);
      etl::destroy(p, p + SIZE);

      // Count.
      size_t count = 0;
      std::fill(std::begin(buffer_non_trivial), std::end(buffer_non_trivial), 0);
      etl::uninitialized_fill_n(p, SIZE, test_item_non_trivial, count);

      result = std::find_if_not(output_non_trivial,
                                output_non_trivial + SIZE,
                                [](non_trivial_t i) { return i == test_item_non_trivial; });

      CHECK(result == output_non_trivial + SIZE);
      CHECK_EQUAL(SIZE, count);

      etl::destroy(p, p + SIZE, count);
      CHECK_EQUAL(0U, count);
    }

    //*************************************************************************
    TEST(test_uninitialized_copy_n_trivial)
    {
      // Also tests uninitialized_copy.

      bool is_equal;

      // Non count.
      trivial_t* p = reinterpret_cast<trivial_t*>(buffer_trivial);

      std::fill(std::begin(buffer_trivial), std::end(buffer_trivial), 0);
      etl::uninitialized_copy_n(test_data_trivial.begin(), SIZE, p);

      is_equal = std::equal(output_trivial, output_trivial + SIZE, test_data_trivial.begin());
      CHECK(is_equal);
      etl::destroy(p, p + SIZE);

      // Count.
      size_t count = 0;
      std::fill(std::begin(buffer_trivial), std::end(buffer_trivial), 0);
      etl::uninitialized_copy_n(test_data_trivial.begin(), SIZE, p, count);

      is_equal = std::equal(output_trivial, output_trivial + SIZE, test_data_trivial.begin());
      CHECK(is_equal);
      CHECK_EQUAL(SIZE, count);
      etl::destroy(p, p + SIZE, count);
      CHECK_EQUAL(0U, count);
    }

    //*************************************************************************
    TEST(test_uninitialized_copy_n_non_trivial)
    {
      // Also tests uninitialized_copy.

      bool is_equal;

      // Non count.
      non_trivial_t* p = reinterpret_cast<non_trivial_t*>(buffer_non_trivial);

      std::fill(std::begin(buffer_non_trivial), std::end(buffer_non_trivial), 0);
      etl::uninitialized_copy_n(test_data_non_trivial.begin(), SIZE, p);

      is_equal = std::equal(output_non_trivial, output_non_trivial + SIZE, test_data_non_trivial.begin());
      CHECK(is_equal);
      etl::destroy(p, p + SIZE);

      // Count.
      size_t count = 0;
      std::fill(std::begin(buffer_non_trivial), std::end(buffer_non_trivial), 0);
      etl::uninitialized_copy_n(test_data_non_trivial.begin(), SIZE, p, count);

      is_equal = std::equal(output_non_trivial, output_non_trivial + SIZE, test_data_non_trivial.begin());
      CHECK(is_equal);
      CHECK_EQUAL(SIZE, count);
      etl::destroy(p, p + SIZE, count);
      CHECK_EQUAL(0U, count);
    }

    //*************************************************************************
    TEST(test_uninitialized_default_construct_n_trivial)
    {
      // Also tests uninitialized_default_construct.

      // Non count.
      trivial_t* p = reinterpret_cast<trivial_t*>(buffer_trivial);

      std::fill(std::begin(buffer_trivial), std::end(buffer_trivial), 0xFF);
      etl::uninitialized_default_construct_n(p, SIZE);

      trivial_t* result;

      result = std::find_if_not(output_trivial, output_trivial + SIZE, [](trivial_t i) { return i == 0xFFFFFFFF; });

      CHECK(result == output_trivial + SIZE);
      etl::destroy(p, p + SIZE);

      // Count.
      size_t count = 0;
      std::fill(std::begin(buffer_trivial), std::end(buffer_trivial), 0xFF);
      etl::uninitialized_default_construct_n(p, SIZE, count);

      result = std::find_if_not(output_trivial, output_trivial + SIZE, [](trivial_t i) { return i == 0xFFFFFFFF; });

      CHECK(result == output_trivial + SIZE);
      CHECK_EQUAL(SIZE, count);

      etl::destroy(p, p + SIZE, count);
      CHECK_EQUAL(0U, count);
    }

    //*************************************************************************
    TEST(test_uninitialized_default_construct_n_non_trivial)
    {
      // Also tests uninitialized_default_construct.

      // Non count.
      non_trivial_t* p = reinterpret_cast<non_trivial_t*>(buffer_non_trivial);

      std::fill(std::begin(buffer_non_trivial), std::end(buffer_non_trivial), 0xFF);
      etl::uninitialized_default_construct_n(p, SIZE);

      non_trivial_t* result;

      result = std::find_if_not(output_non_trivial, output_non_trivial + SIZE, [](non_trivial_t i) { return i == test_item_non_trivial_null; });

      CHECK(result == output_non_trivial + SIZE);
      etl::destroy(p, p + SIZE);

      // Count.
      size_t count = 0;
      std::fill(std::begin(buffer_non_trivial), std::end(buffer_non_trivial), 0xFF);
      etl::uninitialized_default_construct_n(p, SIZE, count);

      result = std::find_if_not(output_non_trivial, output_non_trivial + SIZE, [](non_trivial_t i) { return i == test_item_non_trivial_null; });

      CHECK(result == output_non_trivial + SIZE);
      CHECK_EQUAL(SIZE, count);

      etl::destroy(p, p + SIZE, count);
      CHECK_EQUAL(0U, count);
    }

    //*************************************************************************
    TEST(test_uninitialized_value_construct_n_trivial)
    {
      // Also tests uninitialized_default_construct.

      // Non count.
      trivial_t* p = reinterpret_cast<trivial_t*>(buffer_trivial);

      std::fill(std::begin(buffer_trivial), std::end(buffer_trivial), 0xFF);
      etl::uninitialized_value_construct_n(p, SIZE);

      trivial_t* result;

      result = std::find_if_not(output_trivial, output_trivial + SIZE, [](trivial_t i) { return i == trivial_t(); });

      CHECK(result == output_trivial + SIZE);
      etl::destroy(p, p + SIZE);

      // Count.
      size_t count = 0;
      std::fill(std::begin(buffer_trivial), std::end(buffer_trivial), 0xFF);
      etl::uninitialized_value_construct_n(p, SIZE, count);

      result = std::find_if_not(output_trivial, output_trivial + SIZE, [](trivial_t i) { return i == trivial_t(); });

      CHECK(result == output_trivial + SIZE);
      CHECK_EQUAL(SIZE, count);

      etl::destroy(p, p + SIZE, count);
      CHECK_EQUAL(0U, count);
    }

    //*************************************************************************
    TEST(test_uninitialized_value_construct_n_non_trivial)
    {
      // Also tests uninitialized_default_construct.

      // Non count.
      non_trivial_t* p = reinterpret_cast<non_trivial_t*>(buffer_non_trivial);

      std::fill(std::begin(buffer_non_trivial), std::end(buffer_non_trivial), 0xFF);
      etl::uninitialized_value_construct_n(p, SIZE);

      non_trivial_t* result;

      result = std::find_if_not(output_non_trivial, output_non_trivial + SIZE, [](non_trivial_t i) { return i == non_trivial_t(); });

      CHECK(result == output_non_trivial + SIZE);
      etl::destroy(p, p + SIZE);

      // Count.
      size_t count = 0;
      std::fill(std::begin(buffer_non_trivial), std::end(buffer_non_trivial), 0xFF);
      etl::uninitialized_value_construct_n(p, SIZE, count);

      result = std::find_if_not(output_non_trivial, output_non_trivial + SIZE, [](non_trivial_t i) { return i == non_trivial_t(); });

      CHECK(result == output_non_trivial + SIZE);
      CHECK_EQUAL(SIZE, count);

      etl::destroy(p, p + SIZE, count);
      CHECK_EQUAL(0U, count);
    }

    //*************************************************************************
    TEST(test_create_copy)
    {
      struct Test : etl::create_copy<Test>
      {
        std::string text;
      };

      char buffer[sizeof(Test)];

      Test test1;
      test1.text = "12345678";
      test1.create_copy_at(buffer);
      test1.text = "87654321";

      Test& test2 = *reinterpret_cast<Test*>(buffer);

      CHECK_EQUAL(std::string("87654321"), test1.text);
      CHECK_EQUAL(std::string("12345678"), test2.text);

      int count = 0;
      test1.create_copy_at(buffer, count);

      CHECK_EQUAL(1, count);
    }

    //*************************************************************************
    TEST(test_create_make_copy)
    {
      struct Test : etl::create_copy<Test>
      {
        std::string text;
      };

      char buffer[sizeof(Test)];

      Test test1;
      test1.text = "12345678";
      Test& test2 = test1.make_copy_at(buffer);
      test1.text = "87654321";

      CHECK_EQUAL(std::string("87654321"), test1.text);
      CHECK_EQUAL(std::string("12345678"), test2.text);

      int count = 0;
      test1.make_copy_at(buffer, count);

      CHECK_EQUAL(1, count);
    }

    //*************************************************************************
    TEST(test_make_trivial)
    {
      char n[sizeof(trivial_t)];
      trivial_t* pn = reinterpret_cast<trivial_t*>(n);

      // Non count.
      std::fill(std::begin(n), std::end(n), 0xFF);
      CHECK_EQUAL(0x00000000U, etl::make_default_at(pn));

      std::fill(std::begin(n), std::end(n), 0x00);      
      CHECK_EQUAL(0xFFFFFFFFU, etl::make_value_at(pn, 0xFFFFFFFFU));

      std::fill(std::begin(n), std::end(n), 0xFF);
      etl::make_copy_at(pn, test_item_trivial);
      CHECK_EQUAL(test_item_trivial, etl::make_copy_at(pn, test_item_trivial));

      // Count.
      size_t count = 0;

      std::fill(std::begin(n), std::end(n), 0xFF);
      CHECK_EQUAL(0x00000000U, etl::make_default_at(pn, count));
      CHECK_EQUAL(1U, count);

      std::fill(std::begin(n), std::end(n), 0x00);
      CHECK_EQUAL(0xFFFFFFFFU, etl::make_value_at(pn, 0xFFFFFFFFU, count));
      CHECK_EQUAL(2U, count);

      std::fill(std::begin(n), std::end(n), 0xFF);
      CHECK_EQUAL(test_item_trivial, etl::make_copy_at(pn, test_item_trivial, count));
      CHECK_EQUAL(3U, count);
    }
  };
}
