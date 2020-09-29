/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl
https://www.etlcpp.com

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

#include "UnitTest++/UnitTest++.h"

#include "etl/memory.h"
#include "etl/debug_count.h"

#include "data.h"

#include <string>
#include <array>
#include <algorithm>
#include <iterator>
#include <numeric>
#include <stdint.h>

#include <vector>

namespace
{
  typedef std::string non_trivial_t;
  typedef uint32_t    trivial_t;
  typedef TestDataM<int> moveable_t;

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
  char buffer_moveable[sizeof(moveable_t) * SIZE];

  non_trivial_t* output_non_trivial = reinterpret_cast<non_trivial_t*>(buffer_non_trivial);
  trivial_t*     output_trivial     = reinterpret_cast<trivial_t*>(buffer_trivial);
  moveable_t*    output_moveable    = reinterpret_cast<moveable_t*>(buffer_moveable);

  struct overloaded
  {
    overloaded* operator&()
    {
      return nullptr;
    }
  };

  //***********************************
  template <typename T>
  struct NoDelete
  {
    NoDelete()
    {
    }

    void operator()(T* p) const
    {
    }
  };

  //***********************************
  template <typename T>
  struct NoDelete<T[]>
  {
    NoDelete()
    {
    }

    template <class U>
    void operator()(U* p) const
    {
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
    TEST(test_uninitialized_move)
    {
      bool is_equal;

      // Non count.
      moveable_t* p = reinterpret_cast<moveable_t*>(buffer_moveable);

      std::fill(std::begin(buffer_moveable), std::end(buffer_moveable), 0);

      {
        std::array<moveable_t, SIZE> test_data_moveable =
        {
          moveable_t(0), moveable_t(1), moveable_t(2), moveable_t(3), moveable_t(4),
          moveable_t(5), moveable_t(6), moveable_t(7), moveable_t(8), moveable_t(9)
        };

        etl::uninitialized_move(test_data_moveable.begin(), test_data_moveable.end(), p);
      }

      is_equal = (output_moveable[0] == moveable_t(0)) &&
        (output_moveable[1] == moveable_t(1)) &&
        (output_moveable[2] == moveable_t(2)) &&
        (output_moveable[3] == moveable_t(3)) &&
        (output_moveable[4] == moveable_t(4)) &&
        (output_moveable[5] == moveable_t(5)) &&
        (output_moveable[6] == moveable_t(6)) &&
        (output_moveable[7] == moveable_t(7)) &&
        (output_moveable[8] == moveable_t(8)) &&
        (output_moveable[9] == moveable_t(9));

      CHECK(is_equal);
      etl::destroy(p, p + SIZE);

      // Count.
      size_t count = 0;
      std::fill(std::begin(buffer_non_trivial), std::end(buffer_non_trivial), 0);

      {
        std::array<moveable_t, SIZE> test_data_moveable =
        {
          moveable_t(0), moveable_t(1), moveable_t(2), moveable_t(3), moveable_t(4),
          moveable_t(5), moveable_t(6), moveable_t(7), moveable_t(8), moveable_t(9)
        };

        etl::uninitialized_move(test_data_moveable.begin(), test_data_moveable.end(), p, count);
      }

      is_equal = (output_moveable[0] == moveable_t(0)) &&
        (output_moveable[1] == moveable_t(1)) &&
        (output_moveable[2] == moveable_t(2)) &&
        (output_moveable[3] == moveable_t(3)) &&
        (output_moveable[4] == moveable_t(4)) &&
        (output_moveable[5] == moveable_t(5)) &&
        (output_moveable[6] == moveable_t(6)) &&
        (output_moveable[7] == moveable_t(7)) &&
        (output_moveable[8] == moveable_t(8)) &&
        (output_moveable[9] == moveable_t(9));

      CHECK(is_equal);
      CHECK_EQUAL(SIZE, count);
      etl::destroy(p, p + SIZE, count);
      CHECK_EQUAL(0U, count);
    }

    //*************************************************************************
    TEST(test_uninitialized_move_n)
    {
      bool is_equal;

      // Non count.
      moveable_t* p = reinterpret_cast<moveable_t*>(buffer_moveable);

      std::fill(std::begin(buffer_moveable), std::end(buffer_moveable), 0);

      {
        std::array<moveable_t, SIZE> test_data_moveable =
        {
          moveable_t(0), moveable_t(1), moveable_t(2), moveable_t(3), moveable_t(4),
          moveable_t(5), moveable_t(6), moveable_t(7), moveable_t(8), moveable_t(9)
        };

        etl::uninitialized_move_n(test_data_moveable.begin(), SIZE, p);
      }

      is_equal = (output_moveable[0] == moveable_t(0)) &&
        (output_moveable[1] == moveable_t(1)) &&
        (output_moveable[2] == moveable_t(2)) &&
        (output_moveable[3] == moveable_t(3)) &&
        (output_moveable[4] == moveable_t(4)) &&
        (output_moveable[5] == moveable_t(5)) &&
        (output_moveable[6] == moveable_t(6)) &&
        (output_moveable[7] == moveable_t(7)) &&
        (output_moveable[8] == moveable_t(8)) &&
        (output_moveable[9] == moveable_t(9));

      CHECK(is_equal);
      etl::destroy(p, p + SIZE);

      // Count.
      size_t count = 0;
      std::fill(std::begin(buffer_non_trivial), std::end(buffer_non_trivial), 0);

      {
        std::array<moveable_t, SIZE> test_data_moveable =
        {
          moveable_t(0), moveable_t(1), moveable_t(2), moveable_t(3), moveable_t(4),
          moveable_t(5), moveable_t(6), moveable_t(7), moveable_t(8), moveable_t(9)
        };

        etl::uninitialized_move_n(test_data_moveable.begin(), SIZE, p, count);
      }

      is_equal = (output_moveable[0] == moveable_t(0)) &&
        (output_moveable[1] == moveable_t(1)) &&
        (output_moveable[2] == moveable_t(2)) &&
        (output_moveable[3] == moveable_t(3)) &&
        (output_moveable[4] == moveable_t(4)) &&
        (output_moveable[5] == moveable_t(5)) &&
        (output_moveable[6] == moveable_t(6)) &&
        (output_moveable[7] == moveable_t(7)) &&
        (output_moveable[8] == moveable_t(8)) &&
        (output_moveable[9] == moveable_t(9));

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

    //*************************************************************************
    TEST(test_memory_clear)
    {
      struct Data
      {
        uint32_t d1;
        char     d2;
      };

      Data data = { 0xFFFFFFFF, char(0xFF) };

      etl::memory_clear(data);

      CHECK_EQUAL(0x00000000, data.d1);
      CHECK_EQUAL(0x00, data.d2);
    }

    //*************************************************************************
    TEST(test_memory_clear_range_pointer_n)
    {
      struct Data
      {
        uint32_t d1;
        char     d2;
      };

      Data data[3] = { { 0xFFFFFFFF, char(0xFF) }, { 0xFFFFFFFF, char(0xFF) }, { 0xFFFFFFFF, char(0xFF) } };

      etl::memory_clear_range(data, 3);

      CHECK_EQUAL(0x00000000, data[0].d1);
      CHECK_EQUAL(0x00, data[0].d2);

      CHECK_EQUAL(0x00000000, data[1].d1);
      CHECK_EQUAL(0x00, data[1].d2);

      CHECK_EQUAL(0x00000000, data[2].d1);
      CHECK_EQUAL(0x00, data[2].d2);
    }

    //*************************************************************************
    TEST(test_memory_clear_range_pointer_pointer)
    {
      struct Data
      {
        uint32_t d1;
        char     d2;
      };

      Data data[3] = { { 0xFFFFFFFF, char(0xFF) }, { 0xFFFFFFFF, char(0xFF) }, { 0xFFFFFFFF, char(0xFF) } };

      etl::memory_clear_range(std::begin(data), std::end(data));

      CHECK_EQUAL(0x00000000, data[0].d1);
      CHECK_EQUAL(0x00, data[0].d2);

      CHECK_EQUAL(0x00000000, data[1].d1);
      CHECK_EQUAL(0x00, data[1].d2);

      CHECK_EQUAL(0x00000000, data[2].d1);
      CHECK_EQUAL(0x00, data[2].d2);
    }

    //*************************************************************************
    TEST(test_memory_set)
    {
      struct Data
      {
        uint32_t d1;
        char     d2;
      };

      Data data = { 0xFFFFFFFF, char(0xFF) };

      etl::memory_set(data, 0x5A);

      CHECK_EQUAL(0x5A5A5A5A, data.d1);
      CHECK_EQUAL(0x5A, data.d2);
    }

    //*************************************************************************
    TEST(test_memory_set_range_pointer_n)
    {
      struct Data
      {
        uint32_t d1;
        char     d2;
      };

      Data data[3] = { { 0xFFFFFFFF, char(0xFF) }, { 0xFFFFFFFF, char(0xFF) }, { 0xFFFFFFFF, char(0xFF) } };

      etl::memory_set_range(data, 3, 0x5A);

      CHECK_EQUAL(0x5A5A5A5A, data[0].d1);
      CHECK_EQUAL(0x5A, data[0].d2);

      CHECK_EQUAL(0x5A5A5A5A, data[1].d1);
      CHECK_EQUAL(0x5A, data[1].d2);

      CHECK_EQUAL(0x5A5A5A5A, data[2].d1);
      CHECK_EQUAL(0x5A, data[2].d2);
    }

    //*************************************************************************
    TEST(test_memory_set_range_pointer_pointer)
    {
      struct Data
      {
        uint32_t d1;
        char     d2;
      };

      Data data[3] = { { 0xFFFFFFFF, char(0xFF) }, { 0xFFFFFFFF, char(0xFF) }, { 0xFFFFFFFF, char(0xFF) } };

      etl::memory_set_range(std::begin(data), std::end(data), 0x5A);

      CHECK_EQUAL(0x5A5A5A5A, data[0].d1);
      CHECK_EQUAL(0x5A, data[0].d2);

      CHECK_EQUAL(0x5A5A5A5A, data[1].d1);
      CHECK_EQUAL(0x5A, data[1].d2);

      CHECK_EQUAL(0x5A5A5A5A, data[2].d1);
      CHECK_EQUAL(0x5A, data[2].d2);
    }

    //*************************************************************************
    TEST(test_unique_ptr_default_construction)
    {
      etl::unique_ptr<int> up;

      CHECK(up.get() == nullptr);
      CHECK(!bool(up));
    }

    //*************************************************************************
    TEST(test_unique_ptr_from_pointer_construction)
    {
      etl::unique_ptr<int> up(new int(1));

      CHECK(up.get() != nullptr);
      CHECK(bool(up));
      CHECK_EQUAL(1, *up);
    }

    //*************************************************************************
    TEST(test_unique_ptr_move_construction)
    {
      etl::unique_ptr<int> up1(new int(1));
      etl::unique_ptr<int> up2(std::move(up1));

      CHECK(up1.get() == nullptr);
      CHECK(!bool(up1));
      CHECK(up2.get() != nullptr);
      CHECK(bool(up2));
      CHECK_EQUAL(1, *up2);
    }

    //*************************************************************************
    TEST(test_unique_ptr_release)
    {
      etl::unique_ptr<int> up(new int);

      CHECK(up.release() != nullptr);
      CHECK(!bool(up));
    }

    //*************************************************************************
    TEST(test_unique_ptr_reset)
    {
      etl::unique_ptr<int> up(new int(1));
      int* p = new int(2);

      CHECK_EQUAL(1, *up);
      up.reset(p);
      CHECK_EQUAL(2, *up);
    }

    //*************************************************************************
    TEST(test_unique_ptr_swap)
    {
      etl::unique_ptr<int> up1(new int(1));
      etl::unique_ptr<int> up2(new int(2));

      up1.swap(up2);

      CHECK_EQUAL(2, *up1);
      CHECK_EQUAL(1, *up2);
    }

    //*************************************************************************
    TEST(test_unique_ptr_from_nullptr_assignment)
    {
      etl::unique_ptr<int> up(new int);

      up = nullptr;

      CHECK(up.get() == nullptr);
      CHECK(!bool(up));
    }

    //*************************************************************************
    TEST(test_unique_ptr_move_assignment)
    {
      etl::unique_ptr<int> up1(new int(1));
      etl::unique_ptr<int> up2(new int(2));

      up1 = std::move(up2);

      CHECK(!bool(up2));
      CHECK_EQUAL(2, *up1);
    }

    //*************************************************************************
    TEST(test_unique_ptr_comparison_tests)
    {
      int* p1 = (int*)1U;
      int* p2 = (int*)2U;

      etl::unique_ptr<int, NoDelete<int>> up1(p1);
      etl::unique_ptr<int, NoDelete<int>> up2(p1);
      etl::unique_ptr<int, NoDelete<int>> up3(p2);

      CHECK(up1 == up2);
      CHECK(!(up1 == up3));
      CHECK(!(up1 < up2));
      CHECK(up1 < up3);
      CHECK(!(up3 <= up1));
      CHECK(up1 <= up2);
      CHECK(up1 <= up3);
      CHECK(!(up1 > up2));
      CHECK(up3 > up1);
      CHECK(!(up1 >= up3));
      CHECK(up2 >= up1);
      CHECK(up3 >= up1);
    }

    //*************************************************************************
    TEST(test_unique_ptr_from_array_pointer_construction)
    {
      etl::unique_ptr<int> up(new int[4]);
      std::iota(&up[0], &up[4], 0);

      CHECK(up.get() != nullptr);
      CHECK(bool(up));
      CHECK_EQUAL(0, up[0]);
      CHECK_EQUAL(1, up[1]);
      CHECK_EQUAL(2, up[2]);
      CHECK_EQUAL(3, up[3]);
    }

    //*************************************************************************
    TEST(test_unique_ptr_move_array_construction)
    {
      etl::unique_ptr<int> up1(new int[4]);
      std::iota(&up1[0], &up1[4], 0);
      etl::unique_ptr<int> up2(std::move(up1));

      CHECK(up1.get() == nullptr);
      CHECK(!bool(up1));
      CHECK(up2.get() != nullptr);
      CHECK(bool(up2));
      CHECK_EQUAL(0, up2[0]);
      CHECK_EQUAL(1, up2[1]);
      CHECK_EQUAL(2, up2[2]);
      CHECK_EQUAL(3, up2[3]);
    }

    //*************************************************************************
    TEST(test_unique_ptr_array_release)
    {
      etl::unique_ptr<int> up(new int[4]);
      std::iota(&up[0], &up[4], 0);

      CHECK(up.release() != nullptr);
      CHECK(!bool(up));
    }

    //*************************************************************************
    TEST(test_unique_ptr_array_reset)
    {
      etl::unique_ptr<int> up(new int[4]);
      std::iota(&up[0], &up[4], 0);

      int* p = new int[4];
      std::iota(p, p + 4, 4);

      CHECK_EQUAL(0, up[0]);
      CHECK_EQUAL(1, up[1]);
      CHECK_EQUAL(2, up[2]);
      CHECK_EQUAL(3, up[3]);
      up.reset(p);
      CHECK_EQUAL(4, up[0]);
      CHECK_EQUAL(5, up[1]);
      CHECK_EQUAL(6, up[2]);
      CHECK_EQUAL(7, up[3]);
    }

    //*************************************************************************
    TEST(test_unique_ptr_array_swap)
    {
      etl::unique_ptr<int> up1(new int[4]);
      std::iota(&up1[0], &up1[4], 0);

      etl::unique_ptr<int> up2(new int[4]);
      std::iota(&up2[0], &up2[4], 4);

      up1.swap(up2);

      CHECK_EQUAL(4, up1[0]);
      CHECK_EQUAL(5, up1[1]);
      CHECK_EQUAL(6, up1[2]);
      CHECK_EQUAL(7, up1[3]);

      CHECK_EQUAL(0, up2[0]);
      CHECK_EQUAL(1, up2[1]);
      CHECK_EQUAL(2, up2[2]);
      CHECK_EQUAL(3, up2[3]);
    }

    //*************************************************************************
    TEST(test_unique_ptr_array_from_nullptr_assignment)
    {
      etl::unique_ptr<int> up(new int[4]);

      up = nullptr;

      CHECK(up.get() == nullptr);
      CHECK(!bool(up));
    }

    //*************************************************************************
    TEST(test_unique_ptr_array_move_assignment)
    {
      etl::unique_ptr<int> up1(new int[4]);
      std::iota(&up1[0], &up1[4], 0);

      etl::unique_ptr<int> up2(new int[4]);
      std::iota(&up2[0], &up2[4], 4);

      up1 = std::move(up2);

      CHECK(!bool(up2));
      CHECK_EQUAL(4, up1[0]);
      CHECK_EQUAL(5, up1[1]);
      CHECK_EQUAL(6, up1[2]);
      CHECK_EQUAL(7, up1[3]);
    }
  };
}
