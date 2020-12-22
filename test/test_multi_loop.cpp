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

#include <array>
#include <fstream>

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

  constexpr size_t N_LOOPS = 3;

  using MultiLoop = etl::multi_loop<Index, N_LOOPS>;

  SUITE(test_multi_loop)
  {
    //*************************************************************************
    TEST(default_constructor)
    {
      MultiLoop loops;

      CHECK_EQUAL(N_LOOPS, loops.max_number_of_loops());
      CHECK_EQUAL(0U, loops.number_of_loops());
      CHECK(loops.completed());
    }

    //*************************************************************************
    TEST(create_three_loops)
    {
      MultiLoop loops;

      CHECK_EQUAL(N_LOOPS, loops.max_number_of_loops());

      loops.add_outer(Index(-2), Index(2)); // Middle loop
      loops.add_outer(Index(0),  Index(4)); // Outer loop
      loops.add_inner(Index(2),  Index(6)); // Inner loop

      CHECK_EQUAL(3U, loops.number_of_loops());
      CHECK(!loops.completed());

      CHECK_EQUAL(Index(0),       loops.begin(0));
      CHECK_EQUAL(Index(4),       loops.end(0));
      CHECK_EQUAL(loops.begin(0), loops.index(0));

      CHECK_EQUAL(Index(-2),      loops.begin(1));
      CHECK_EQUAL(Index(2),       loops.end(1));
      CHECK_EQUAL(loops.begin(1), loops.index(1));

      CHECK_EQUAL(Index(2),       loops.begin(2));
      CHECK_EQUAL(Index(6),       loops.end(2));
      CHECK_EQUAL(loops.begin(2), loops.index(2));
    }

    //*************************************************************************
    TEST(run_three_loops)
    {
      MultiLoop loops;

      CHECK_EQUAL(N_LOOPS, loops.max_number_of_loops());

      loops.add_outer(Index(-2), Index(2)); // Middle loop
      loops.add_outer(Index(0),  Index(4)); // Outer loop
      loops.add_inner(Index(2),  Index(6)); // Inner loop
      
      struct result
      {
        Index index0;
        Index index1;
        Index index2;
      };

      std::array results
      {       
          result{ 0, -2, 2 }, result{ 0, -2, 3 }, result{ 0, -2, 4 }, result{ 0, -2, 5 },
          result{ 0, -1, 2 }, result{ 0, -1, 3 }, result{ 0, -1, 4 }, result{ 0, -1, 5 },
          result{ 0,  0, 2 }, result{ 0,  0, 3 }, result{ 0,  0, 4 }, result{ 0,  0, 5 },
          result{ 0,  1, 2 }, result{ 0,  1, 3 }, result{ 0,  1, 4 }, result{ 0,  1, 5 },
          result{ 1, -2, 2 }, result{ 1, -2, 3 }, result{ 1, -2, 4 }, result{ 1, -2, 5 },
          result{ 1, -1, 2 }, result{ 1, -1, 3 }, result{ 1, -1, 4 }, result{ 1, -1, 5 },
          result{ 1,  0, 2 }, result{ 1,  0, 3 }, result{ 1,  0, 4 }, result{ 1,  0, 5 },
          result{ 1,  1, 2 }, result{ 1,  1, 3 }, result{ 1,  1, 4 }, result{ 1,  1, 5 },
          result{ 2, -2, 2 }, result{ 2, -2, 3 }, result{ 2, -2, 4 }, result{ 2, -2, 5 },
          result{ 2, -1, 2 }, result{ 2, -1, 3 }, result{ 2, -1, 4 }, result{ 2, -1, 5 },
          result{ 2,  0, 2 }, result{ 2,  0, 3 }, result{ 2,  0, 4 }, result{ 2,  0, 5 },
          result{ 2,  1, 2 }, result{ 2,  1, 3 }, result{ 2,  1, 4 }, result{ 2,  1, 5 },
          result{ 3, -2, 2 }, result{ 3, -2, 3 }, result{ 3, -2, 4 }, result{ 3, -2, 5 },
          result{ 3, -1, 2 }, result{ 3, -1, 3 }, result{ 3, -1, 4 }, result{ 3, -1, 5 },
          result{ 3,  0, 2 }, result{ 3,  0, 3 }, result{ 3,  0, 4 }, result{ 3,  0, 5 },
          result{ 3,  1, 2 }, result{ 3,  1, 3 }, result{ 3,  1, 4 }, result{ 3,  1, 5 }       
      };

      size_t i = 0U;

      for (loops.start(); loops.completed() == false; loops.next())
      {
        CHECK_EQUAL(results[i].index0, loops.index(0));
        CHECK_EQUAL(results[i].index1, loops.index(1));
        CHECK_EQUAL(results[i].index2, loops.index(2));

        if ((results[i].index0 != loops.index(0)) ||
            (results[i].index1 != loops.index(1)) ||
            (results[i].index2 != loops.index(2)))
        {
          CHECK_EQUAL(UINT_MAX, i);
        }

        ++i;
      }

      CHECK(loops.completed());

      // Do it again to make sure things have been reset correctly.
      i = 0U;

      for (loops.start(); loops.completed() == false; loops.next())
      {
        CHECK_EQUAL(results[i].index0, loops.index(0));
        CHECK_EQUAL(results[i].index1, loops.index(1));
        CHECK_EQUAL(results[i].index2, loops.index(2));

        if ((results[i].index0 != loops.index(0)) ||
          (results[i].index1 != loops.index(1)) ||
          (results[i].index2 != loops.index(2)))
        {
          CHECK_EQUAL(UINT_MAX, i);
        }

        ++i;
      }

      CHECK(loops.completed());
    }
  };
}
