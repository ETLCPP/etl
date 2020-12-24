/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl
https://www.etlcpp.com

Copyright(c) 2014 jwellbelove

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

#include "etl/multi_loop.h"

#include <iterator>
#include <fstream>
#include <string>
#include <forward_list>
#include <array>

namespace
{
  class Index
  {
  public:

    Index()
      : index(0)
    {
    }

    Index(int i)
      : index(i)
    {
    }

    Index& operator ++()
    {
      ++index;
      return *this;
    }

    Index& operator --()
    {
      --index;
      return *this;
    }

    Index& operator = (int i)
    {
      index = i;
      return *this;
    }

    bool operator = (const Index& other)
    {
      index = other.index;

      return *this;
    }

    bool operator < (const Index& other) const
    {
      return index < other.index;
    }

    operator int() const
    {
      return index;
    }

    friend bool operator == (const Index& lhs, const Index& rhs)
    {
      return lhs.index == rhs.index;
    }

    friend bool operator != (const Index& lhs, const Index& rhs)
    {
      return !(lhs == rhs);
    }

    int index;
  };

  std::ostream& operator << (std::ostream& os, const Index& index)
  {
    os << int(index);
    return os;
  }

  using Iterator = std::forward_list<std::string>::const_iterator;

  using Outer  = etl::multi_loop<int>;
  using Middle = etl::multi_loop<Index, etl::decrement>;
  using Inner  = etl::multi_loop<Iterator>;

  std::forward_list<std::string> strings =
  {
    "zero", "one", "two", "three"
  };

  Outer  outer(0, 4);
  Middle middle(Index(2), Index(-2));
  Inner  inner(strings.begin(), strings.end());

  SUITE(test_multi_loop)
  {  
    //*************************************************************************
    TEST(create_three_loops)
    {
      outer.clear();
      middle.clear();
      inner.clear();

      outer.append(middle);
      middle.insert(inner);

      CHECK(!outer.completed());
      CHECK(!middle.completed());
      CHECK(!inner.completed());

      CHECK_EQUAL(3U, outer.number_of_loops());
      CHECK_EQUAL(2U, middle.number_of_loops());
      CHECK_EQUAL(1U, inner.number_of_loops());

      CHECK_EQUAL(64U, outer.number_of_iterations());
      CHECK_EQUAL(16U, middle.number_of_iterations());
      CHECK_EQUAL(4U,  inner.number_of_iterations());

      CHECK_EQUAL(0,             outer.begin());
      CHECK_EQUAL(4,             outer.end());
      CHECK_EQUAL(outer.begin(), outer.value());

      CHECK_EQUAL(Index(2),       middle.begin());
      CHECK_EQUAL(Index(-2),      middle.end());
      CHECK_EQUAL(middle.begin(), middle.value());

      CHECK(strings.begin() == inner.begin());
      CHECK(strings.end()   == inner.end());
      CHECK(inner.begin()   == inner.value());
    }

    //*************************************************************************
    TEST(create_three_loops_different_order)
    {
      outer.clear();
      middle.clear();
      inner.clear();

      middle.append(inner);
      outer.insert(middle);
        
      CHECK(!outer.completed());
      CHECK(!middle.completed());
      CHECK(!inner.completed());

      CHECK_EQUAL(3U, outer.number_of_loops());
      CHECK_EQUAL(2U, middle.number_of_loops());
      CHECK_EQUAL(1U, inner.number_of_loops());

      CHECK_EQUAL(64U, outer.number_of_iterations());
      CHECK_EQUAL(16U, middle.number_of_iterations());
      CHECK_EQUAL(4U, inner.number_of_iterations());

      CHECK_EQUAL(0, outer.begin());
      CHECK_EQUAL(4, outer.end());
      CHECK_EQUAL(outer.begin(), outer.value());

      CHECK_EQUAL(Index(2), middle.begin());
      CHECK_EQUAL(Index(-2), middle.end());
      CHECK_EQUAL(middle.begin(), middle.value());

      CHECK(strings.begin() == inner.begin());
      CHECK(strings.end()   == inner.end());
      CHECK(inner.begin()   == inner.value());
    }

    //*************************************************************************
    TEST(create_three_loops_functional_style)
    {
      outer.clear();
      middle.clear();
      inner.clear();

      outer.append(inner).insert(middle);

      CHECK(!outer.completed());
      CHECK(!middle.completed());
      CHECK(!inner.completed());

      CHECK_EQUAL(3U, outer.number_of_loops());
      CHECK_EQUAL(2U, middle.number_of_loops());
      CHECK_EQUAL(1U, inner.number_of_loops());

      CHECK_EQUAL(64U, outer.number_of_iterations());
      CHECK_EQUAL(16U, middle.number_of_iterations());
      CHECK_EQUAL(4U, inner.number_of_iterations());

      CHECK_EQUAL(0, outer.begin());
      CHECK_EQUAL(4, outer.end());
      CHECK_EQUAL(outer.begin(), outer.value());

      CHECK_EQUAL(Index(2), middle.begin());
      CHECK_EQUAL(Index(-2), middle.end());
      CHECK_EQUAL(middle.begin(), middle.value());

      CHECK(strings.begin() == inner.begin());
      CHECK(strings.end()   == inner.end());
      CHECK(inner.begin()   == inner.value());
    }

    //*************************************************************************
    TEST(create_three_loops_circular_links)
    {
      outer.clear();
      middle.clear();
      inner.clear();

      middle.append(outer);

      CHECK_THROW(outer.append(middle), etl::multi_loop_circular_reference);
      CHECK_THROW(inner.append(inner),  etl::multi_loop_circular_reference);
    }

    //*************************************************************************
    TEST(run_three_loops)
    {
      outer.clear();
      middle.clear();
      inner.clear();

      outer.append(middle).append(inner);
     
      struct result
      {
        int outer;
        Index middle;
        std::string inner;
      };

      std::array results
      {
          result{ 0,  2, "zero" }, result{ 0,  2, "one" }, result{ 0,  2, "two" }, result{ 0,  2, "three" },
          result{ 0,  1, "zero" }, result{ 0,  1, "one" }, result{ 0,  1, "two" }, result{ 0,  1, "three" },
          result{ 0,  0, "zero" }, result{ 0,  0, "one" }, result{ 0,  0, "two" }, result{ 0,  0, "three" },
          result{ 0, -1, "zero" }, result{ 0, -1, "one" }, result{ 0, -1, "two" }, result{ 0, -1, "three" },
          result{ 1,  2, "zero" }, result{ 1,  2, "one" }, result{ 1,  2, "two" }, result{ 1,  2, "three" },
          result{ 1,  1, "zero" }, result{ 1,  1, "one" }, result{ 1,  1, "two" }, result{ 1,  1, "three" },
          result{ 1,  0, "zero" }, result{ 1,  0, "one" }, result{ 1,  0, "two" }, result{ 1,  0, "three" },
          result{ 1, -1, "zero" }, result{ 1, -1, "one" }, result{ 1, -1, "two" }, result{ 1, -1, "three" },
          result{ 2,  2, "zero" }, result{ 2,  2, "one" }, result{ 2,  2, "two" }, result{ 2,  2, "three" },
          result{ 2,  1, "zero" }, result{ 2,  1, "one" }, result{ 2,  1, "two" }, result{ 2,  1, "three" },
          result{ 2,  0, "zero" }, result{ 2,  0, "one" }, result{ 2,  0, "two" }, result{ 2,  0, "three" },
          result{ 2, -1, "zero" }, result{ 2, -1, "one" }, result{ 2, -1, "two" }, result{ 2, -1, "three" },
          result{ 3,  2, "zero" }, result{ 3,  2, "one" }, result{ 3,  2, "two" }, result{ 3,  2, "three" },
          result{ 3,  1, "zero" }, result{ 3,  1, "one" }, result{ 3,  1, "two" }, result{ 3,  1, "three" },
          result{ 3,  0, "zero" }, result{ 3,  0, "one" }, result{ 3,  0, "two" }, result{ 3,  0, "three" },
          result{ 3, -1, "zero" }, result{ 3, -1, "one" }, result{ 3, -1, "two" }, result{ 3, -1, "three" }
      };

      size_t i = 0U;

      //  Create const references to the loop values.
      const int&      value_outer  = outer.value();
      const Index&    value_middle = middle.value();
      const Iterator& value_inner  = inner.value();

      for (outer.start(); outer.completed() == false; outer.next())
      {
        CHECK_EQUAL(results[i].outer,  value_outer);
        CHECK_EQUAL(results[i].middle, value_middle);
        CHECK_EQUAL(results[i].inner,  *value_inner);

        // Show the iteration index if there is a failure.
        if ((results[i].outer  != value_outer)  ||
            (results[i].middle != value_middle) ||
            (results[i].inner  != *value_inner))
        {
          CHECK_EQUAL(UINT_MAX, i);
        }

        ++i;
      }

      CHECK(outer.completed());

      // Do it again to make sure things have been reset correctly.
      i = 0U;

      for (outer.start(); outer.completed() == false; outer.next())
      {
        CHECK_EQUAL(results[i].outer,  value_outer);
        CHECK_EQUAL(results[i].middle, value_middle);
        CHECK_EQUAL(results[i].inner,  *value_inner);

        // Show the iteration index if there is a failure.
        if ((results[i].outer  != value_outer)  ||
            (results[i].middle != value_middle) ||
            (results[i].inner  != *value_inner))
        {
          CHECK_EQUAL(UINT_MAX, i);
        }

        ++i;
      }

      CHECK(outer.completed());
    }

    //*************************************************************************
    TEST(create_three_loops_but_just_run_the_inner_two)
    {
      outer.clear();
      middle.clear();
      inner.clear();

      outer.append(middle).append(inner);

      struct result
      {
        int outer;
        Index middle;
        std::string inner;
      };

      std::array results
      {
          result{ 0,  2, "zero" }, result{ 0,  2, "one" }, result{ 0,  2, "two" }, result{ 0,  2, "three" },
          result{ 0,  1, "zero" }, result{ 0,  1, "one" }, result{ 0,  1, "two" }, result{ 0,  1, "three" },
          result{ 0,  0, "zero" }, result{ 0,  0, "one" }, result{ 0,  0, "two" }, result{ 0,  0, "three" },
          result{ 0, -1, "zero" }, result{ 0, -1, "one" }, result{ 0, -1, "two" }, result{ 0, -1, "three" }
      };

      size_t i = 0U;

      //  Create const references to the loop values.
      const int&      value_outer  = outer.value();
      const Index&    value_middle = middle.value();
      const Iterator& value_inner  = inner.value();

      for (middle.start(); middle.completed() == false; middle.next())
      {
        CHECK_EQUAL(results[i].outer,  value_outer);
        CHECK_EQUAL(results[i].middle, value_middle);
        CHECK_EQUAL(results[i].inner,  *value_inner);

        // Show the iteration index if there is a failure.
        if ((results[i].outer  != value_outer) ||
            (results[i].middle != value_middle) ||
            (results[i].inner  != *value_inner))
        {
          CHECK_EQUAL(UINT_MAX, i);
        }

        ++i;
      }

      CHECK(middle.completed());

      // Do it again to make sure things have been reset correctly.
      i = 0U;

      for (middle.start(); middle.completed() == false; middle.next())
      {
        CHECK_EQUAL(results[i].outer,  value_outer);
        CHECK_EQUAL(results[i].middle, value_middle);
        CHECK_EQUAL(results[i].inner,  *value_inner);

        // Show the iteration index if there is a failure.
        if ((results[i].outer  != value_outer) ||
            (results[i].middle != value_middle) ||
            (results[i].inner  != *value_inner))
        {
          CHECK_EQUAL(UINT_MAX, i);
        }

        ++i;
      }

      CHECK(middle.completed());
    }
  };
}
