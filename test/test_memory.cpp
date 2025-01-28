/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl
https://www.etlcpp.com

Copyright(c) 2017 John Wellbelove

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

#include "etl/memory.h"
#include "etl/list.h"
#include "etl/debug_count.h"

#include "data.h"

#include <string>
#include <array>
#include <algorithm>
#include <iterator>
#include <numeric>
#include <stdint.h>
#include <vector>
#include <memory>

namespace
{
  typedef std::string    non_trivial_t;
  typedef uint32_t       trivial_t;
  typedef TestDataM<int> moveable_t;

  const size_t SIZE = 10UL;

  std::array<non_trivial_t, SIZE> test_data_non_trivial =
  {
    "one", "two",   "three", "four", "five",
    "six", "seven", "eight", "nine", "ten"
  };

  std::array<trivial_t, SIZE> test_data_trivial =
  {
    0x11223344UL, 0x22334455UL, 0x33445566UL, 0x44556677UL, 0x55667788UL,
    0x66778899UL, 0x778899AAUL, 0x8899AABBUL, 0x99AABBCCUL, 0xAABBCCDDUL
  };

  non_trivial_t test_item_non_trivial("eleven");
  non_trivial_t test_item_non_trivial_null("");
  trivial_t test_item_trivial(0xBBCCDDEEUL);

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

    void operator()(T*) const
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
    void operator()(U* /*p*/) const
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
      std::fill(std::begin(n), std::end(n), 0xFFU);
      etl::create_default_at(pn);
      CHECK_EQUAL(0xFFFFFFFFUL, *pn);
      etl::destroy_at(pn);
      CHECK_EQUAL(0xFFFFFFFFUL, *pn);

      std::fill(std::begin(n), std::end(n), 0xFFU);
      etl::create_value_at(pn);
      CHECK_EQUAL(0x00000000UL, *pn);
      etl::destroy_at(pn);
      CHECK_EQUAL(0x00000000UL, *pn);

      std::fill(std::begin(n), std::end(n), 0xFFU);
      etl::create_copy_at(pn, test_item_trivial);
      CHECK_EQUAL(test_item_trivial, *pn);
      etl::destroy_at(pn);
      CHECK_EQUAL(test_item_trivial, *pn);

      // Count.
      size_t count = 0UL;

      std::fill(std::begin(n), std::end(n), 0xFFU);
      etl::create_default_at(pn, count);
      CHECK_EQUAL(0xFFFFFFFFUL, *pn);
      CHECK_EQUAL(1U, count);
      etl::destroy_at(pn, count);
      CHECK_EQUAL(0xFFFFFFFFUL, *pn);
      CHECK_EQUAL(0U, count);

      std::fill(std::begin(n), std::end(n), 0xFFU);
      etl::create_value_at(pn, count);
      CHECK_EQUAL(0x00000000UL, *pn);
      CHECK_EQUAL(1U, count);
      etl::destroy_at(pn, count);
      CHECK_EQUAL(0x00000000UL, *pn);
      CHECK_EQUAL(0U, count);

      std::fill(std::begin(n), std::end(n), 0xFFU);
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
      std::fill(std::begin(n), std::end(n), 0xFFU);
      etl::create_default_at(pn);
      CHECK_EQUAL(test_item_non_trivial_null, *pn);
      etl::destroy_at(pn);

      std::fill(std::begin(n), std::end(n), 0xFFU);
      etl::create_value_at(pn);
      CHECK_EQUAL(test_item_non_trivial_null, *pn);
      etl::destroy_at(pn);

      std::fill(std::begin(n), std::end(n), 0xFFU);
      etl::create_copy_at(pn, test_item_non_trivial);
      CHECK_EQUAL(test_item_non_trivial, *pn);
      etl::destroy_at(pn);

      // Count.
      size_t count = 0UL;

      std::fill(std::begin(n), std::end(n), 0xFFU);
      etl::create_default_at(pn, count);
      CHECK_EQUAL(test_item_non_trivial_null, *pn);
      CHECK_EQUAL(1U, count);
      etl::destroy_at(pn, count);
      CHECK_EQUAL(0U, count);

      count = 0;
      std::fill(std::begin(n), std::end(n), 0xFFU);
      etl::create_value_at(pn, count);
      CHECK_EQUAL(test_item_non_trivial_null, *pn);
      CHECK_EQUAL(1U, count);
      etl::destroy_at(pn, count);
      CHECK_EQUAL(0U, count);

      count = 0;
      std::fill(std::begin(n), std::end(n), 0xFFU);
      etl::create_copy_at(pn, test_item_non_trivial, count);
      CHECK_EQUAL(test_item_non_trivial, *pn);
      CHECK_EQUAL(1U, count);
      etl::destroy_at(pn, count);
      CHECK_EQUAL(0U, count);
    }

    //*************************************************************************
    TEST(test_construct_destroy_trivial)
    {
      char n[sizeof(trivial_t)];
      trivial_t* pn = reinterpret_cast<trivial_t*>(n);

      // Non count.
      std::fill(std::begin(n), std::end(n), 0xFFU);
      etl::construct_at(pn);
      CHECK_EQUAL(0UL, *pn);
      etl::destroy_at(pn);
      CHECK_EQUAL(0UL, *pn);

      std::fill(std::begin(n), std::end(n), 0xFFU);
      etl::construct_at(pn);
      CHECK_EQUAL(0x00000000UL, *pn);
      etl::destroy_at(pn);
      CHECK_EQUAL(0x00000000UL, *pn);

      std::fill(std::begin(n), std::end(n), 0xFFU);
      etl::construct_at(pn, test_item_trivial);
      CHECK_EQUAL(test_item_trivial, *pn);
      etl::destroy_at(pn);
      CHECK_EQUAL(test_item_trivial, *pn);
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
      size_t count = 0UL;
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
      size_t count = 0UL;
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
      size_t count = 0UL;
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
      size_t count = 0UL;
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
      size_t count = 0UL;
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
      size_t count = 0UL;
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

      std::fill(std::begin(buffer_trivial), std::end(buffer_trivial), 0xFFU);
      etl::uninitialized_default_construct_n(p, SIZE);

      trivial_t* result;

      result = std::find_if_not(output_trivial, output_trivial + SIZE, [](trivial_t i) { return i == 0xFFFFFFFFUL; });

      CHECK(result == output_trivial + SIZE);
      etl::destroy(p, p + SIZE);

      // Count.
      size_t count = 0UL;
      std::fill(std::begin(buffer_trivial), std::end(buffer_trivial), 0xFFU);
      etl::uninitialized_default_construct_n(p, SIZE, count);

      result = std::find_if_not(output_trivial, output_trivial + SIZE, [](trivial_t i) { return i == 0xFFFFFFFFUL; });

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

      std::fill(std::begin(buffer_non_trivial), std::end(buffer_non_trivial), 0xFFU);
      etl::uninitialized_default_construct_n(p, SIZE);

      non_trivial_t* result;

      result = std::find_if_not(output_non_trivial, output_non_trivial + SIZE, [](non_trivial_t i) { return i == test_item_non_trivial_null; });

      CHECK(result == output_non_trivial + SIZE);
      etl::destroy(p, p + SIZE);

      // Count.
      size_t count = 0UL;
      std::fill(std::begin(buffer_non_trivial), std::end(buffer_non_trivial), 0xFFU);
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

      std::fill(std::begin(buffer_trivial), std::end(buffer_trivial), 0xFFU);
      etl::uninitialized_value_construct_n(p, SIZE);

      trivial_t* result;

      result = std::find_if_not(output_trivial, output_trivial + SIZE, [](trivial_t i) { return i == trivial_t(); });

      CHECK(result == output_trivial + SIZE);
      etl::destroy(p, p + SIZE);

      // Count.
      size_t count = 0UL;
      std::fill(std::begin(buffer_trivial), std::end(buffer_trivial), 0xFFU);
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

      std::fill(std::begin(buffer_non_trivial), std::end(buffer_non_trivial), 0xFFU);
      etl::uninitialized_value_construct_n(p, SIZE);

      non_trivial_t* result;

      result = std::find_if_not(output_non_trivial, output_non_trivial + SIZE, [](non_trivial_t i) { return i == non_trivial_t(); });

      CHECK(result == output_non_trivial + SIZE);
      etl::destroy(p, p + SIZE);

      // Count.
      size_t count = 0UL;
      std::fill(std::begin(buffer_non_trivial), std::end(buffer_non_trivial), 0xFFU);
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
      struct Object : etl::create_copy<Object>
      {
        std::string text;
      };

      char buffer[sizeof(Object)];

      Object object1;
      object1.text = "12345678";
      object1.create_copy_at(buffer);
      object1.text = "87654321";

      Object& object2 = *reinterpret_cast<Object*>(buffer);

      CHECK_EQUAL(std::string("87654321"), object1.text);
      CHECK_EQUAL(std::string("12345678"), object2.text);

      int count = 0;
      object1.create_copy_at(buffer, count);

      CHECK_EQUAL(1, count);
    }

    //*************************************************************************
    TEST(test_create_make_copy)
    {
      struct Object : etl::create_copy<Object>
      {
        std::string text;
      };

      char buffer[sizeof(Object)];

      Object object1;
      object1.text = "12345678";
      Object& object2 = object1.make_copy_at(buffer);
      object1.text = "87654321";

      CHECK_EQUAL(std::string("87654321"), object1.text);
      CHECK_EQUAL(std::string("12345678"), object2.text);

      int count = 0;
      object1.make_copy_at(buffer, count);

      CHECK_EQUAL(1, count);
    }

    //*************************************************************************
    TEST(test_make_trivial)
    {
      char n[sizeof(trivial_t)];
      trivial_t* pn = reinterpret_cast<trivial_t*>(n);

      // Non count.
      std::fill(std::begin(n), std::end(n), 0xFFU);
      CHECK_EQUAL(0x00000000UL, etl::make_default_at(pn));

      std::fill(std::begin(n), std::end(n), 0x00U);
      CHECK_EQUAL(0xFFFFFFFFUL, etl::make_value_at(pn, 0xFFFFFFFFU));

      std::fill(std::begin(n), std::end(n), 0xFFU);
      etl::make_copy_at(pn, test_item_trivial);
      CHECK_EQUAL(test_item_trivial, etl::make_copy_at(pn, test_item_trivial));

      // Count.
      size_t count = 0UL;

      std::fill(std::begin(n), std::end(n), 0xFFU);
      CHECK_EQUAL(0x00000000UL, etl::make_default_at(pn, count));
      CHECK_EQUAL(1U, count);

      std::fill(std::begin(n), std::end(n), 0x00U);
      CHECK_EQUAL(0xFFFFFFFFUL, etl::make_value_at(pn, 0xFFFFFFFFUL, count));
      CHECK_EQUAL(2U, count);

      std::fill(std::begin(n), std::end(n), 0xFFU);
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

      Data data = { 0xFFFFFFFFUL, char(0xFFU) };

      etl::memory_clear(data);

      CHECK_EQUAL(0x00000000UL, data.d1);
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

      Data data[3] = { { 0xFFFFFFFFUL, char(0xFFU) }, { 0xFFFFFFFFUL, char(0xFFU) }, { 0xFFFFFFFFUL, char(0xFFU) } };

      etl::memory_clear_range(data, 3);

      CHECK_EQUAL(0x00000000UL, data[0].d1);
      CHECK_EQUAL(0x00U, data[0].d2);

      CHECK_EQUAL(0x00000000UL, data[1].d1);
      CHECK_EQUAL(0x00U, data[1].d2);

      CHECK_EQUAL(0x00000000UL, data[2].d1);
      CHECK_EQUAL(0x00U, data[2].d2);
    }

    //*************************************************************************
    TEST(test_memory_clear_range_pointer_pointer)
    {
      struct Data
      {
        uint32_t d1;
        char     d2;
      };

      Data data[3] = { { 0xFFFFFFFFUL, char(0xFFU) }, { 0xFFFFFFFFUL, char(0xFFU) }, { 0xFFFFFFFFUL, char(0xFFU) } };

      etl::memory_clear_range(std::begin(data), std::end(data));

      CHECK_EQUAL(0x00000000UL, data[0].d1);
      CHECK_EQUAL(0x00U, data[0].d2);

      CHECK_EQUAL(0x00000000UL, data[1].d1);
      CHECK_EQUAL(0x00U, data[1].d2);

      CHECK_EQUAL(0x00000000UL, data[2].d1);
      CHECK_EQUAL(0x00U, data[2].d2);
    }

    //*************************************************************************
    TEST(test_memory_set)
    {
      struct Data
      {
        uint32_t d1;
        char     d2;
      };

      Data data = { 0xFFFFFFFFUL, char(0xFFU) };

      etl::memory_set(data, 0x5A);

      CHECK_EQUAL(0x5A5A5A5AUL, data.d1);
      CHECK_EQUAL(0x5AU, data.d2);
    }

    //*************************************************************************
    TEST(test_memory_set_range_pointer_n)
    {
      struct Data
      {
        uint32_t d1;
        char     d2;
      };

      Data data[3] = { { 0xFFFFFFFFUL, char(0xFFU) }, { 0xFFFFFFFFUL, char(0xFFU) }, { 0xFFFFFFFFUL, char(0xFFU) } };

      etl::memory_set_range(data, 3, 0x5A);

      CHECK_EQUAL(0x5A5A5A5AUL, data[0].d1);
      CHECK_EQUAL(0x5AU, data[0].d2);

      CHECK_EQUAL(0x5A5A5A5AUL, data[1].d1);
      CHECK_EQUAL(0x5AU, data[1].d2);

      CHECK_EQUAL(0x5A5A5A5AUL, data[2].d1);
      CHECK_EQUAL(0x5AU, data[2].d2);
    }

    //*************************************************************************
    TEST(test_memory_set_range_pointer_pointer)
    {
      struct Data
      {
        uint32_t d1;
        char     d2;
      };

      Data data[3] = { { 0xFFFFFFFFUL, char(0xFFU) }, { 0xFFFFFFFFUL, char(0xFFU) }, { 0xFFFFFFFFUL, char(0xFFU) } };

      etl::memory_set_range(std::begin(data), std::end(data), 0x5A);

      CHECK_EQUAL(0x5A5A5A5AUL, data[0].d1);
      CHECK_EQUAL(0x5AU, data[0].d2);

      CHECK_EQUAL(0x5A5A5A5AUL, data[1].d1);
      CHECK_EQUAL(0x5AU, data[1].d2);

      CHECK_EQUAL(0x5A5A5A5AUL, data[2].d1);
      CHECK_EQUAL(0x5AU, data[2].d2);
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
      auto buffer = new int;
      etl::unique_ptr<int> up(buffer);

      CHECK(up.release() != nullptr);
      CHECK(!bool(up));

      delete buffer;
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
    TEST(test_unique_ptr_nullptr_from_nullptr_assignment)
    {
      etl::unique_ptr<int> up;

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
      etl::unique_ptr<int[]> up(new int[4]);
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
      etl::unique_ptr<int[]> up1(new int[4]);
      std::iota(&up1[0], &up1[4], 0);
      etl::unique_ptr<int[]> up2(std::move(up1));

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
      auto buffer = new int[4];
      etl::unique_ptr<int[]> up(buffer);
      std::iota(&up[0], &up[4], 0);

      CHECK(up.release() != nullptr);
      CHECK(!bool(up));

      delete[] buffer;
    }

    //*************************************************************************
    TEST(test_unique_ptr_array_reset)
    {
      etl::unique_ptr<int[]> up(new int[4]);
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
      etl::unique_ptr<int[]> up1(new int[4]);
      std::iota(&up1[0], &up1[4], 0);

      etl::unique_ptr<int[]> up2(new int[4]);
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
      etl::unique_ptr<int[]> up(new int[4]);

      up = nullptr;

      CHECK(up.get() == nullptr);
      CHECK(!bool(up));
    }

    //*************************************************************************
    TEST(test_unique_ptr_array_move_assignment)
    {
      etl::unique_ptr<int[]> up1(new int[4]);
      std::iota(&up1[0], &up1[4], 0);

      etl::unique_ptr<int[]> up2(new int[4]);
      std::iota(&up2[0], &up2[4], 4);

      up1 = std::move(up2);

      CHECK(!bool(up2));
      CHECK_EQUAL(4, up1[0]);
      CHECK_EQUAL(5, up1[1]);
      CHECK_EQUAL(6, up1[2]);
      CHECK_EQUAL(7, up1[3]);
    }

    //*************************************************************************
    TEST(test_unique_ptr_custom_deleter)
    {
      //*******************************
      struct Object
      {
        Object()
          : count(1)
        {
        }

        void Delete()
        {
          count = 0;
        }

        int count;
      };

      //*******************************
      struct Deleter
      {
        void operator()(Object* p)
        {
          p->Delete();
        }
      };

      Deleter deleter;
      Object object;

      CHECK_EQUAL(1, object.count);

      {
        etl::unique_ptr<Object, Deleter> up(&object, deleter);
      }

      CHECK_EQUAL(0, object.count);
    }

    //*************************************************************************
    TEST(test_uninitialized_buffer)
    {
      typedef etl::uninitialized_buffer<sizeof(uint32_t), 4, etl::alignment_of<uint32_t>::value> storage32_t;

      size_t alignment = etl::alignment_of<storage32_t>::value;
      size_t expected  = std::alignment_of<uint32_t>::value;

      CHECK_EQUAL(expected, alignment);
    }

    //*************************************************************************
    TEST(test_uninitialized_buffer_of)
    {
      typedef etl::uninitialized_buffer_of<uint32_t, 4> storage32_t;
      static storage32_t buffer;

      uint32_t* i = buffer;
      const uint32_t* ci = buffer;

      CHECK(i == ci);

      buffer[0] = 0U;
      buffer[1] = 1U;
      buffer[2] = 2U;
      buffer[3] = 3U;

      CHECK_EQUAL(0U, buffer[0]);
      CHECK_EQUAL(1U, buffer[1]);
      CHECK_EQUAL(2U, buffer[2]);
      CHECK_EQUAL(3U, buffer[3]);

      const storage32_t& refbuffer = buffer;

      CHECK_EQUAL(0U, refbuffer[0]);
      CHECK_EQUAL(1U, refbuffer[1]);
      CHECK_EQUAL(2U, refbuffer[2]);
      CHECK_EQUAL(3U, refbuffer[3]);

      size_t alignment = etl::alignment_of<storage32_t>::value;
      size_t expected  = std::alignment_of<uint32_t>::value;

      CHECK_EQUAL(expected, alignment);
    }

    //*************************************************************************
    TEST(test_mem_copy_pointer_pointer_pointer)
    {
      uint32_t src[8] = { 0x12345678, 0x76543210, 0x01452367, 0x23670145, 0x67234501, 0x45016723, 0x01324576, 0x76453201 };
      uint32_t dst[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };

      etl::mem_copy(src, src + 8, dst);

      CHECK(std::equal(src, src + 8, dst));
    }

    //*************************************************************************
    TEST(test_mem_copy_const_pointer_const_pointer_pointer)
    {
      const uint32_t src[8] = { 0x12345678, 0x76543210, 0x01452367, 0x23670145, 0x67234501, 0x45016723, 0x01324576, 0x76453201 };
      uint32_t dst[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };

      etl::mem_copy(src, src + 8, dst);

      CHECK(std::equal(src, src + 8, dst));
    }

    //*************************************************************************
    TEST(test_mem_copy_pointer_length_pointer)
    {
      uint32_t src[8] = { 0x12345678, 0x76543210, 0x01452367, 0x23670145, 0x67234501, 0x45016723, 0x01324576, 0x76453201 };
      uint32_t dst[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };

      etl::mem_copy(src, 8, dst);
      CHECK(std::equal(src, src + 8, dst));
    }

    //*************************************************************************
    TEST(test_mem_copy_const_pointer_length_pointer)
    {
      const uint32_t src[8] = { 0x12345678, 0x76543210, 0x01452367, 0x23670145, 0x67234501, 0x45016723, 0x01324576, 0x76453201 };
      uint32_t dst[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };

      etl::mem_copy(src, 8, dst);
      CHECK(std::equal(src, src + 8, dst));
    }

    //*************************************************************************
    TEST(test_mem_move_pointer_pointer_pointer)
    {
      uint32_t expected[8] = { 0x12345678, 0x76543210, 0x01452367, 0x23670145, 0x67234501, 0x45016723, 0x01324576, 0x76453201 };
      uint32_t data[12]    = { 0x12345678, 0x76543210, 0x01452367, 0x23670145, 0x67234501, 0x45016723, 0x01324576, 0x76453201, 0, 0, 0, 0 };

      etl::mem_move(data, data + 8, data + 4);

      CHECK(std::equal(expected, expected + 8, data + 4));
    }

    //*************************************************************************
    TEST(test_mem_move_const_pointer_const_pointer_pointer)
    {
      uint32_t expected[8] = { 0x12345678, 0x76543210, 0x01452367, 0x23670145, 0x67234501, 0x45016723, 0x01324576, 0x76453201 };
      uint32_t data[12]    = { 0x12345678, 0x76543210, 0x01452367, 0x23670145, 0x67234501, 0x45016723, 0x01324576, 0x76453201, 0, 0, 0, 0 };
      const uint32_t* data_begin = &data[0];
      const uint32_t* data_end = &data[8];

      etl::mem_move(data_begin, data_end, data + 4);

      CHECK(std::equal(expected, expected + 8, data + 4));
    }

    //*************************************************************************
    TEST(test_mem_move_pointer_length_pointer)
    {
      uint32_t expected[8] = { 0x12345678, 0x76543210, 0x01452367, 0x23670145, 0x67234501, 0x45016723, 0x01324576, 0x76453201 };
      uint32_t data[12]    = { 0x12345678, 0x76543210, 0x01452367, 0x23670145, 0x67234501, 0x45016723, 0x01324576, 0x76453201, 0, 0, 0, 0 };

      etl::mem_move(data, 8, data + 4);

      CHECK(std::equal(expected, expected + 8, data + 4));
    }

    //*************************************************************************
    TEST(test_mem_move_const_pointer_length_pointer)
    {
      uint32_t expected[8] = { 0x12345678, 0x76543210, 0x01452367, 0x23670145, 0x67234501, 0x45016723, 0x01324576, 0x76453201 };
      uint32_t data[12]    = { 0x12345678, 0x76543210, 0x01452367, 0x23670145, 0x67234501, 0x45016723, 0x01324576, 0x76453201, 0, 0, 0, 0 };
      const uint32_t* data_begin = &data[0];
      etl::mem_move(data_begin, 8, data + 4);

      CHECK(std::equal(expected, expected + 8, data + 4));
    }

    //*************************************************************************
    TEST(test_mem_compare_pointer_pointer_pointer)
    {
      uint32_t data[8] = { 0x12345678, 0x76543210, 0x01452367, 0x23670145, 0x67234501, 0x45016723, 0x01324576, 0x76453201 };
      uint32_t same[8] = { 0x12345678, 0x76543210, 0x01452367, 0x23670145, 0x67234501, 0x45016723, 0x01324576, 0x76453201 };
      uint32_t grtr[8] = { 0x12345678, 0x76543210, 0x01452367, 0x23670145, 0x67235501, 0x45016723, 0x01324576, 0x76453201 };
      uint32_t less[8] = { 0x12345678, 0x76543210, 0x01452367, 0x23670145, 0x67134501, 0x45016723, 0x01324576, 0x76453201 };
      
      CHECK(etl::mem_compare(data, data + 8, same) == 0);
      CHECK(etl::mem_compare(data, data + 8, grtr) > 0);
      CHECK(etl::mem_compare(data, data + 8, less) < 0);
    }

    //*************************************************************************
    TEST(test_mem_compare_const_pointer_const_pointer_pointer)
    {
      const uint32_t data[8] = { 0x12345678, 0x76543210, 0x01452367, 0x23670145, 0x67234501, 0x45016723, 0x01324576, 0x76453201 };
      uint32_t same[8] = { 0x12345678, 0x76543210, 0x01452367, 0x23670145, 0x67234501, 0x45016723, 0x01324576, 0x76453201 };
      uint32_t grtr[8] = { 0x12345678, 0x76543210, 0x01452367, 0x23670145, 0x67235501, 0x45016723, 0x01324576, 0x76453201 };
      uint32_t less[8] = { 0x12345678, 0x76543210, 0x01452367, 0x23670145, 0x67134501, 0x45016723, 0x01324576, 0x76453201 };

      CHECK(etl::mem_compare(data, data + 8, same) == 0);
      CHECK(etl::mem_compare(data, data + 8, grtr) > 0);
      CHECK(etl::mem_compare(data, data + 8, less) < 0);
    }

    //*************************************************************************
    TEST(test_mem_compare_const_pointer_const_pointer_const_pointer)
    {
      const uint32_t data[8] = { 0x12345678, 0x76543210, 0x01452367, 0x23670145, 0x67234501, 0x45016723, 0x01324576, 0x76453201 };
      const uint32_t same[8] = { 0x12345678, 0x76543210, 0x01452367, 0x23670145, 0x67234501, 0x45016723, 0x01324576, 0x76453201 };
      uint32_t grtr[8] = { 0x12345678, 0x76543210, 0x01452367, 0x23670145, 0x67235501, 0x45016723, 0x01324576, 0x76453201 };
      uint32_t less[8] = { 0x12345678, 0x76543210, 0x01452367, 0x23670145, 0x67134501, 0x45016723, 0x01324576, 0x76453201 };

      CHECK(etl::mem_compare(data, data + 8, same) == 0);
      CHECK(etl::mem_compare(data, data + 8, grtr) > 0);
      CHECK(etl::mem_compare(data, data + 8, less) < 0);
    }

    //*************************************************************************
    TEST(test_mem_compare_pointer_length_pointer)
    {
      uint32_t data[8] = { 0x12345678, 0x76543210, 0x01452367, 0x23670145, 0x67234501, 0x45016723, 0x01324576, 0x76453201 };
      uint32_t same[8] = { 0x12345678, 0x76543210, 0x01452367, 0x23670145, 0x67234501, 0x45016723, 0x01324576, 0x76453201 };
      uint32_t grtr[8] = { 0x12345678, 0x76543210, 0x01452367, 0x23670145, 0x67235501, 0x45016723, 0x01324576, 0x76453201 };
      uint32_t less[8] = { 0x12345678, 0x76543210, 0x01452367, 0x23670145, 0x67134501, 0x45016723, 0x01324576, 0x76453201 };

      CHECK(etl::mem_compare(data, 8, same) == 0);
      CHECK(etl::mem_compare(data, 8, grtr) > 0);
      CHECK(etl::mem_compare(data, 8, less) < 0);
    }

    //*************************************************************************
    TEST(test_mem_compare_const_pointer_length_pointer)
    {
      const uint32_t data[8] = { 0x12345678, 0x76543210, 0x01452367, 0x23670145, 0x67234501, 0x45016723, 0x01324576, 0x76453201 };
      uint32_t same[8] = { 0x12345678, 0x76543210, 0x01452367, 0x23670145, 0x67234501, 0x45016723, 0x01324576, 0x76453201 };
      uint32_t grtr[8] = { 0x12345678, 0x76543210, 0x01452367, 0x23670145, 0x67235501, 0x45016723, 0x01324576, 0x76453201 };
      uint32_t less[8] = { 0x12345678, 0x76543210, 0x01452367, 0x23670145, 0x67134501, 0x45016723, 0x01324576, 0x76453201 };

      CHECK(etl::mem_compare(data, 8, same) == 0);
      CHECK(etl::mem_compare(data, 8, grtr) > 0);
      CHECK(etl::mem_compare(data, 8, less) < 0);
    }

    //*************************************************************************
    TEST(test_mem_compare_const_pointer_length_const_pointer)
    {
      const uint32_t data[8] = { 0x12345678, 0x76543210, 0x01452367, 0x23670145, 0x67234501, 0x45016723, 0x01324576, 0x76453201 };
      const uint32_t same[8] = { 0x12345678, 0x76543210, 0x01452367, 0x23670145, 0x67234501, 0x45016723, 0x01324576, 0x76453201 };
      const uint32_t grtr[8] = { 0x12345678, 0x76543210, 0x01452367, 0x23670145, 0x67235501, 0x45016723, 0x01324576, 0x76453201 };
      const uint32_t less[8] = { 0x12345678, 0x76543210, 0x01452367, 0x23670145, 0x67134501, 0x45016723, 0x01324576, 0x76453201 };

      CHECK(etl::mem_compare(data, 8, same) == 0);
      CHECK(etl::mem_compare(data, 8, grtr) > 0);
      CHECK(etl::mem_compare(data, 8, less) < 0);
    }

    //*************************************************************************
    TEST(test_mem_set_pointer_pointer)
    {
      uint32_t data[8]     = { 0, 0, 0, 0, 0, 0, 0, 0 };
      uint32_t expected[8] = { 0, 0x5A5A5A5A, 0x5A5A5A5A, 0x5A5A5A5A, 0x5A5A5A5A, 0, 0, 0 };

      etl::mem_set(data + 1, data + 5, 0x5A);

      CHECK(std::equal(expected, expected + 8, data));
    }

    //*************************************************************************
    TEST(test_mem_set_pointer_length)
    {
      uint32_t data[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };
      uint32_t expected[8] = { 0, 0x5A5A5A5A, 0x5A5A5A5A, 0x5A5A5A5A, 0x5A5A5A5A, 0, 0, 0 };

      etl::mem_set(data + 1, 4, 0x5A);

      CHECK(std::equal(expected, expected + 8, data));
    }

    //*************************************************************************
    TEST(test_mem_char_pointer_pointer)
    {
      uint32_t data[8] = { 0x12345678, 0x76543210, 0x01452367, 0x23670145, 0x67294501, 0x45016723, 0x01324576, 0x76453201 };

      char *p1 = etl::mem_char(data, data + 8, 0x29);
      char* p2 = etl::mem_char(data, data + 8, 0x99);

      CHECK_EQUAL(uint32_t(0x29), uint32_t(*p1));
      CHECK((reinterpret_cast<char*>(data) + 18) == p1);
      CHECK((reinterpret_cast<char*>(data) + 32) == p2);
    }

    //*************************************************************************
    TEST(test_mem_char_pointer_pointer_const)
    {
      const uint32_t data[8] = { 0x12345678, 0x76543210, 0x01452367, 0x23670145, 0x67294501, 0x45016723, 0x01324576, 0x76453201 };

      const char* p1 = etl::mem_char(data, data + 8, 0x29);
      const char* p2 = etl::mem_char(data, data + 8, 0x99);

      CHECK_EQUAL(uint32_t(0x29), uint32_t(*p1));
      CHECK((reinterpret_cast<const char*>(data) + 18) == p1);
      CHECK((reinterpret_cast<const char*>(data) + 32) == p2);
    }

    //*************************************************************************
    TEST(test_mem_char_pointer_length)
    {
      uint32_t data[8] = { 0x12345678, 0x76543210, 0x01452367, 0x23670145, 0x67294501, 0x45016723, 0x01324576, 0x76453201 };

      char* p1 = etl::mem_char(data, 8, 0x29);
      char* p2 = etl::mem_char(data, 8, 0x99);

      CHECK_EQUAL(uint32_t(0x29), uint32_t(*p1));
      CHECK((reinterpret_cast<char*>(data) + 18) == p1);
      CHECK((reinterpret_cast<char*>(data) + 32) == p2);
    }

    //*************************************************************************
    TEST(test_mem_char_pointer_length_const)
    {
      const uint32_t data[8] = { 0x12345678, 0x76543210, 0x01452367, 0x23670145, 0x67294501, 0x45016723, 0x01324576, 0x76453201 };

      const char* p1 = etl::mem_char(data, 8, 0x29);
      const char* p2 = etl::mem_char(data, 8, 0x99);

      CHECK_EQUAL(uint32_t(0x29), uint32_t(*p1));
      CHECK((reinterpret_cast<const char*>(data) + 18) == p1);
      CHECK((reinterpret_cast<const char*>(data) + 32) == p2);
    }

    //*************************************************************************
    class Base 
    {
    public:
      virtual ~Base() {};
      virtual void function() = 0;
    };

    static bool function_was_called = false;

    class Derived : public Base 
    {
    public:
      Derived() 
      {
        function_was_called = false;
      }

      void function() 
      {
        function_was_called = true;
      }
    };

    void call(etl::unique_ptr<Base> ptr) 
    {
      ptr->function();
    }

    TEST(test_derived_type)
    {
      CHECK(!function_was_called);

      etl::unique_ptr<Derived> ptr(new Derived());
      CHECK(ptr.get() != ETL_NULLPTR);
      
      call(etl::move(ptr));
      CHECK(function_was_called);
      CHECK(ptr.get() == ETL_NULLPTR);
    }

    
    struct Flags
    {
      Flags()
        : constructed(false)
        , destructed(false)
      {
      }

      void Clear()
      {
        constructed = false;
        destructed = false;
      }

      bool constructed;
      bool destructed;
    };

    static Flags flags;

    TEST(test_construct_get_destroy_object_aligned)
    {
      struct Data
      {
        Data()
          : a(1)
          , b(2)
        {
          flags.constructed = true;
        }

        Data(int a_, int b_)
          : a(a_)
          , b(b_)
        {
          flags.constructed = true;
        }

        ~Data()
        {
          flags.destructed = true;
        }

        int a;
        int b;
      };
     
      alignas(Data) char buffer1[sizeof(Data)];
      char* pbuffer1 = buffer1;

      alignas(Data) char buffer1b[sizeof(Data)];
      char* pbuffer1b = buffer1b;

      alignas(Data) char buffer2[sizeof(Data)];
      char* pbuffer2 = buffer2;

      alignas(Data) char buffer2b[sizeof(Data)];
      char* pbuffer2b = buffer2b;

      alignas(Data) char buffer3[sizeof(Data)];
      char* pbuffer3 = buffer3;

      alignas(Data) char buffer3b[sizeof(Data)];
      char* pbuffer3b = buffer3b;

      flags.Clear();
      Data& rdata1 = etl::construct_object_at<Data>(pbuffer1);
      CHECK_TRUE(flags.constructed);
      CHECK_FALSE(flags.destructed);
      CHECK_EQUAL(1, rdata1.a);
      CHECK_EQUAL(2, rdata1.b);

      flags.Clear();
      Data data2(3, 4);
      Data& rdata2 = etl::construct_object_at(pbuffer2, data2);
      CHECK_TRUE(flags.constructed);
      CHECK_FALSE(flags.destructed);
      CHECK_EQUAL(data2.a, rdata2.a);
      CHECK_EQUAL(data2.b, rdata2.b);

      flags.Clear();
      Data& rdata3 = etl::construct_object_at<Data>(pbuffer3, 5, 6);
      CHECK_TRUE(flags.constructed);
      CHECK_FALSE(flags.destructed);
      CHECK_EQUAL(5, rdata3.a);
      CHECK_EQUAL(6, rdata3.b);

      memcpy(buffer1b, buffer1, sizeof(Data));
      memcpy(buffer2b, buffer2, sizeof(Data));
      memcpy(buffer3b, buffer3, sizeof(Data));

      flags.Clear();
      Data& rdata1b = etl::get_object_at<Data>(pbuffer1b);
      CHECK_FALSE(flags.constructed);
      CHECK_FALSE(flags.destructed);
      CHECK_EQUAL(1, rdata1b.a);
      CHECK_EQUAL(2, rdata1b.b);
      
      flags.Clear();
      Data& rdata2b = etl::get_object_at<Data>(pbuffer2b);
      CHECK_FALSE(flags.constructed);
      CHECK_FALSE(flags.destructed);
      CHECK_EQUAL(data2.a, rdata2b.a);
      CHECK_EQUAL(data2.b, rdata2b.b);
      
      flags.Clear();
      Data& rdata3b = etl::get_object_at<Data>(pbuffer3b);
      CHECK_FALSE(flags.constructed);
      CHECK_FALSE(flags.destructed);
      CHECK_EQUAL(5, rdata3b.a);
      CHECK_EQUAL(6, rdata3b.b);

      flags.Clear();
      etl::destroy_object_at<Data>(pbuffer1b);
      CHECK_FALSE(flags.constructed);
      CHECK_TRUE(flags.destructed);

      flags.Clear();
      etl::destroy_object_at<Data>(pbuffer2b);
      CHECK_FALSE(flags.constructed);
      CHECK_TRUE(flags.destructed);

      flags.Clear();
      etl::destroy_object_at<Data>(pbuffer3b);
      CHECK_FALSE(flags.constructed);
      CHECK_TRUE(flags.destructed);
    }

    TEST(test_construct_get_destroy_object_misaligned)
    {
      struct Data
      {
        Data()
          : a(1)
          , b(2)
        {
        }

        Data(int a_, int b_)
          : a(a_)
          , b(b_)
        {
        }

        ~Data()
        {
        }

        int a;
        int b;
      };

      alignas(Data) char buffer1[sizeof(Data)];
      char* pbuffer1 = buffer1 + 1;

      alignas(Data) char buffer2[sizeof(Data)];
      char* pbuffer2 = buffer2 + 1;

      alignas(Data) char buffer3[sizeof(Data)];
      char* pbuffer3 = buffer3 + 1;

      CHECK_THROW(etl::construct_object_at<Data>(pbuffer1), etl::alignment_error);

      Data data2(3, 4);
      CHECK_THROW(etl::construct_object_at(pbuffer2, data2), etl::alignment_error);

      CHECK_THROW(etl::construct_object_at<Data>(pbuffer3, 5, 6), etl::alignment_error);

      CHECK_THROW(etl::get_object_at<Data>(pbuffer1), etl::alignment_error);

      CHECK_THROW(etl::destroy_object_at<Data>(pbuffer1), etl::alignment_error);
    }

    //*************************************************************************
    TEST(test_to_address)
    {
      int  i;
      int* pi = &i;

      etl::list<int, 4> container = { 1, 2, 3, 4 };
      etl::list<int, 4>::iterator itr = container.begin();
      std::advance(itr, 2);
      int* plist_item = &*itr;

      CHECK_EQUAL(&i, etl::to_address(pi));
      CHECK_EQUAL(plist_item, etl::to_address(itr));
    }
  };
}
