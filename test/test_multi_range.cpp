/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl
https://www.etlcpp.com

Copyright(c) 2020 John Wellbelove

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

#include "etl/multi_range.h"
#include "etl/functional.h"

#include <iterator>
#include <string>
#include <forward_list>
#include <array>
#include <functional>

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

    Index(const Index& other)
      : index(other.index)
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

    Index& operator = (const Index& other)
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

  //*************************************************************************
  using StepperOuter = etl::multi_range<int>::forward_step_by;

  //*************************************************************************
  using StepperMiddle = etl::multi_range<Index>::reverse_step;

  //*************************************************************************
  using CompareMiddle = etl::multi_range<Index>::greater_than_compare;

  // Declare the loops.
  using Outer  = etl::multi_range<int>;
  using Middle = etl::multi_range<Index>;
  using Inner  = etl::multi_range<Iterator>;

  // A list of strings.
  std::forward_list<std::string> strings =
  {
    "zero", "one", "two", "three"
  };

  // Outer loop : Iterate from 0 to 6 in steps of 2.
  // Keep looping while the value is not equal to 8.
  StepperOuter stepperOuter(2);
  Outer outer(0, 8, stepperOuter);

  // Middle loop : Iterate from Index(2) to Index(-1).
  // Keep looping while the value is greater than Index(-2).
  StepperMiddle stepperMiddle;
  CompareMiddle compareMiddle;
  Middle middle(Index(2), Index(-2), stepperMiddle, compareMiddle);

  // Inner loop : Iterate from the first to the last string in the forward_list of strings.
  // Keep looping while the iterator is not equal to strings.end().
  Inner inner(strings.begin(), strings.end());

  SUITE(test_multi_range)
  {
    //*************************************************************************
    TEST(create_three_loops)
    {
      outer.append(middle);
      middle.insert(inner);

      CHECK(outer.completed());
      CHECK(middle.completed());
      CHECK(inner.completed());

      CHECK_EQUAL(3U, outer.number_of_ranges());
      CHECK_EQUAL(2U, middle.number_of_ranges());
      CHECK_EQUAL(1U, inner.number_of_ranges());

      CHECK_EQUAL(64U, outer.number_of_iterations());
      CHECK_EQUAL(16U, middle.number_of_iterations());
      CHECK_EQUAL(4U,  inner.number_of_iterations());

      CHECK_EQUAL(0,             outer.begin());
      CHECK_EQUAL(8,             outer.end());
      CHECK_EQUAL(outer.begin(), outer.value());

      CHECK_EQUAL(Index(2),       middle.begin());
      CHECK_EQUAL(Index(-2),      middle.end());
      CHECK_EQUAL(middle.begin(), middle.value());

      CHECK(strings.begin() == inner.begin());
      CHECK(strings.end()   == inner.end());
      CHECK(inner.begin()   == inner.value());

      outer.detach_all();
    }

    //*************************************************************************
    TEST(create_three_loops_different_order)
    {
      middle.append(inner);
      outer.insert(middle);

      CHECK(outer.completed());
      CHECK(middle.completed());
      CHECK(inner.completed());

      CHECK_EQUAL(3U, outer.number_of_ranges());
      CHECK_EQUAL(2U, middle.number_of_ranges());
      CHECK_EQUAL(1U, inner.number_of_ranges());

      CHECK_EQUAL(64U, outer.number_of_iterations());
      CHECK_EQUAL(16U, middle.number_of_iterations());
      CHECK_EQUAL(4U, inner.number_of_iterations());

      CHECK_EQUAL(0, outer.begin());
      CHECK_EQUAL(8, outer.end());
      CHECK_EQUAL(outer.begin(), outer.value());

      CHECK_EQUAL(Index(2), middle.begin());
      CHECK_EQUAL(Index(-2), middle.end());
      CHECK_EQUAL(middle.begin(), middle.value());

      CHECK(strings.begin() == inner.begin());
      CHECK(strings.end()   == inner.end());
      CHECK(inner.begin()   == inner.value());

      outer.detach_all();
    }

    //*************************************************************************
    TEST(create_three_loops_functional_style)
    {
      outer.append(inner).insert(middle);

      CHECK(outer.completed());
      CHECK(middle.completed());
      CHECK(inner.completed());

      CHECK_EQUAL(3U, outer.number_of_ranges());
      CHECK_EQUAL(2U, middle.number_of_ranges());
      CHECK_EQUAL(1U, inner.number_of_ranges());

      CHECK_EQUAL(64U, outer.number_of_iterations());
      CHECK_EQUAL(16U, middle.number_of_iterations());
      CHECK_EQUAL(4U, inner.number_of_iterations());

      CHECK_EQUAL(0, outer.begin());
      CHECK_EQUAL(8, outer.end());
      CHECK_EQUAL(outer.begin(), outer.value());

      CHECK_EQUAL(Index(2), middle.begin());
      CHECK_EQUAL(Index(-2), middle.end());
      CHECK_EQUAL(middle.begin(), middle.value());

      CHECK(strings.begin() == inner.begin());
      CHECK(strings.end()   == inner.end());
      CHECK(inner.begin()   == inner.value());

      outer.detach_all();
    }

    //*************************************************************************
    TEST(create_three_loops_circular_links)
    {
      middle.append(outer);

      CHECK_THROW(outer.append(middle), etl::multi_range_circular_reference);
      CHECK_THROW(inner.append(inner),  etl::multi_range_circular_reference);

      outer.detach();
      middle.detach();
      inner.detach();
    }

    //*************************************************************************
    TEST(null_loop)
    {
      Outer null_loop(2, 2);

      CHECK(null_loop.completed());

      CHECK_EQUAL(1U, null_loop.number_of_ranges());

      CHECK_EQUAL(0U, null_loop.number_of_iterations());

      CHECK_EQUAL(2, null_loop.begin());
      CHECK_EQUAL(2, null_loop.end());
      CHECK_EQUAL(null_loop.begin(), null_loop.value());
    }

    //*************************************************************************
    TEST(run_three_loops)
    {
      outer.append(middle).append(inner);

      struct result
      {
        int outer;
        Index middle;
        std::string inner;
      };

      std::array<result, 64U> results
      {
          result{ 0,  2, "zero" }, result{ 0,  2, "one" }, result{ 0,  2, "two" }, result{ 0,  2, "three" },
          result{ 0,  1, "zero" }, result{ 0,  1, "one" }, result{ 0,  1, "two" }, result{ 0,  1, "three" },
          result{ 0,  0, "zero" }, result{ 0,  0, "one" }, result{ 0,  0, "two" }, result{ 0,  0, "three" },
          result{ 0, -1, "zero" }, result{ 0, -1, "one" }, result{ 0, -1, "two" }, result{ 0, -1, "three" },
          result{ 2,  2, "zero" }, result{ 2,  2, "one" }, result{ 2,  2, "two" }, result{ 2,  2, "three" },
          result{ 2,  1, "zero" }, result{ 2,  1, "one" }, result{ 2,  1, "two" }, result{ 2,  1, "three" },
          result{ 2,  0, "zero" }, result{ 2,  0, "one" }, result{ 2,  0, "two" }, result{ 2,  0, "three" },
          result{ 2, -1, "zero" }, result{ 2, -1, "one" }, result{ 2, -1, "two" }, result{ 2, -1, "three" },
          result{ 4,  2, "zero" }, result{ 4,  2, "one" }, result{ 4,  2, "two" }, result{ 4,  2, "three" },
          result{ 4,  1, "zero" }, result{ 4,  1, "one" }, result{ 4,  1, "two" }, result{ 4,  1, "three" },
          result{ 4,  0, "zero" }, result{ 4,  0, "one" }, result{ 4,  0, "two" }, result{ 4,  0, "three" },
          result{ 4, -1, "zero" }, result{ 4, -1, "one" }, result{ 4, -1, "two" }, result{ 4, -1, "three" },
          result{ 6,  2, "zero" }, result{ 6,  2, "one" }, result{ 6,  2, "two" }, result{ 6,  2, "three" },
          result{ 6,  1, "zero" }, result{ 6,  1, "one" }, result{ 6,  1, "two" }, result{ 6,  1, "three" },
          result{ 6,  0, "zero" }, result{ 6,  0, "one" }, result{ 6,  0, "two" }, result{ 6,  0, "three" },
          result{ 6, -1, "zero" }, result{ 6, -1, "one" }, result{ 6, -1, "two" }, result{ 6, -1, "three" }
      };

      size_t i = 0UL;

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

      outer.detach_all();
    }

    //*************************************************************************
    TEST(create_three_loops_but_just_run_the_inner_two)
    {
      outer.append(middle).append(inner);

      struct result
      {
        int outer;
        Index middle;
        std::string inner;
      };

      std::array<result, 16U> results
      {
          result{ 0,  2, "zero" }, result{ 0,  2, "one" }, result{ 0,  2, "two" }, result{ 0,  2, "three" },
          result{ 0,  1, "zero" }, result{ 0,  1, "one" }, result{ 0,  1, "two" }, result{ 0,  1, "three" },
          result{ 0,  0, "zero" }, result{ 0,  0, "one" }, result{ 0,  0, "two" }, result{ 0,  0, "three" },
          result{ 0, -1, "zero" }, result{ 0, -1, "one" }, result{ 0, -1, "two" }, result{ 0, -1, "three" }
      };

      size_t i = 0UL;

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

      outer.detach_all();
    }
  };
}
