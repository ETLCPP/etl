/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl
https://www.etlcpp.com

Copyright(c) 2025 John Wellbelove, rlindeman

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files(the 'Software'), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and / or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions :

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED 'AS IS', WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
******************************************************************************/

#include "unit_test_framework.h"

#include <map>
#include <array>
#include <algorithm>
#include <utility>
#include <iterator>
#include <string>
#include <vector>

#include "etl/const_map.h"
#include "etl/map.h"
#include "etl/string.h"

#include "data.h"

namespace
{
  static const size_t Max_Size = 10UL;

  //*************************************************************************
  // The key type
  //*************************************************************************
  struct Key
  {
    // Default constructor
    constexpr Key()
      : k(0)
    {
    }

    // Construct from char key
    constexpr explicit Key(char k_)
      : k(k_)
    {
    }

    char k;
  };

  // Less-than operator for Key < Key
  constexpr bool operator <(const Key& lhs, const Key& rhs) noexcept
  {
    return (lhs.k < rhs.k);
  }

  // Less-than operator for Key < char
  constexpr bool operator <(const Key& lhs, char rhs) noexcept
  {
    return (lhs.k < rhs);
  }

  // Less-than operator for char < Key
  constexpr bool operator <(char lhs, const Key& rhs) noexcept
  {
    return (lhs < rhs.k);
  }

  // Greater-than operator for Key < Key
  constexpr bool operator >(const Key& lhs, const Key& rhs) noexcept
  {
    return (lhs.k > rhs.k);
  }

  // Greater-than operator for Key > char
  constexpr bool operator >(const Key& lhs, char rhs) noexcept
  {
    return (lhs.k > rhs);
  }

  // Greater-than operator for char > Key
  constexpr bool operator >(char lhs, const Key& rhs) noexcept
  {
    return (lhs > rhs.k);
  }

  // Greater-than operator for char > Key
  constexpr bool operator ==(const Key& lhs, const Key& rhs) noexcept
  {
    return (lhs.k == rhs.k);
  }

  //#define TEST_GREATER_THAN
  #ifdef TEST_GREATER_THAN
    using Data                      = etl::const_map<Key, int, Max_Size, etl::greater<Key>>;
    using DataTransparentComparator = etl::const_map<Key, int, Max_Size, etl::greater<>>;
  #else
    using Data                      = etl::const_map<Key, int, Max_Size, etl::less<Key>>;
    using DataTransparentComparator = etl::const_map<Key, int, Max_Size, etl::less<>>;
  #endif

  using value_type = Data::value_type;

  using Data_iterator       = Data::iterator;
  using Data_const_iterator = Data::const_iterator;

  SUITE(test_const_map)
  {
    //*************************************************************************
    TEST(test_default_constructor)
    {
      static constexpr Data data;

      static constexpr bool   is_valid            = data.is_valid();
      static constexpr size_t size                = data.size();
      static constexpr bool   empty               = data.empty();
      static constexpr bool   full                = data.full();
      static constexpr size_t capacity            = data.capacity();
      static constexpr size_t max_size            = data.max_size();
      static constexpr Data::const_iterator begin = data.begin();
      static constexpr Data::const_iterator end   = data.end();

      CHECK_TRUE(is_valid);
      CHECK_TRUE(size == 0UL);
      CHECK_TRUE(empty);
      CHECK_FALSE(full);
      CHECK_TRUE(capacity == Max_Size);
      CHECK_TRUE(max_size == Max_Size);
      CHECK_TRUE(begin == end);
    }

    //*************************************************************************
    TEST(test_constructor_min_size)
    {
      static constexpr Data data{ value_type{Key('A'), 0 } };

      static constexpr bool   is_valid            = data.is_valid();
      static constexpr size_t size                = data.size();
      static constexpr bool   empty               = data.empty();
      static constexpr bool   full                = data.full();
      static constexpr size_t capacity            = data.capacity();
      static constexpr size_t max_size            = data.max_size();
      static constexpr Data::const_iterator begin = data.begin();
      static constexpr Data::const_iterator end   = data.end();

      CHECK_TRUE(is_valid);
      CHECK_TRUE(size == 1U);
      CHECK_FALSE(empty);
      CHECK_FALSE(full);
      CHECK_TRUE(capacity == Max_Size);
      CHECK_TRUE(max_size == Max_Size);
      CHECK_FALSE(begin == end);
    }

    //*************************************************************************
    TEST(test_constructor_max_size)
    {
#ifdef TEST_GREATER_THAN
      static constexpr Data data{ value_type{Key('J'), 9 }, value_type{Key('I'), 8 }, value_type{Key('H'), 7 }, value_type{Key('G'), 6 }, value_type{Key('F'), 5 },
                                  value_type{Key('E'), 4 }, value_type{Key('D'), 3 }, value_type{Key('C'), 2 }, value_type{Key('B'), 1 }, value_type{Key('A'), 0 } };
#else
      static constexpr Data data{ value_type{Key('A'), 0 }, value_type{Key('B'), 1 }, value_type{Key('C'), 2 }, value_type{Key('D'), 3 }, value_type{Key('E'), 4 },
                                  value_type{Key('F'), 5 }, value_type{Key('G'), 6 }, value_type{Key('H'), 7 }, value_type{Key('I'), 8 }, value_type{Key('J'), 9 } };
#endif

      static constexpr bool   is_valid            = data.is_valid();
      static constexpr size_t size                = data.size();
      static constexpr bool   empty               = data.empty();
      static constexpr bool   full                = data.full();
      static constexpr size_t capacity            = data.capacity();
      static constexpr size_t max_size            = data.max_size();
      static constexpr Data::const_iterator begin = data.begin();
      static constexpr Data::const_iterator end   = data.end();

      CHECK_TRUE(is_valid);
      CHECK_TRUE(size == Max_Size);
      CHECK_FALSE(empty);
      CHECK_TRUE(full);
      CHECK_TRUE(capacity == Max_Size);
      CHECK_TRUE(max_size == Max_Size);
      CHECK_FALSE(begin == end);
    }

    ////*************************************************************************
    // Enable to check static_assert "Number of elements exceeds capacity"
    ////*************************************************************************
    //TEST(test_constructor_excess_size)
    //{
    //  static constexpr Data data{ value_type{Key('A'), 0 }, value_type{Key('B'), 1 }, value_type{Key('C'), 2 }, value_type{Key('D'), 3 }, value_type{Key('E'), 4 },
    //                              value_type{Key('F'), 5 }, value_type{'G'), 6 }, value_type{'H'), 7 }, value_type{Key('I'), 8 }, value_type{Key('J'), 9 },
    //                              value_type{Key('K'), 10 } };
    //}

#if ETL_USING_CPP17
    //*************************************************************************
    TEST(test_cpp17_deduced_constructor)
    {
#ifdef TEST_GREATER_THAN
      static constexpr Data data{ value_type{Key('J'), 9 }, value_type{Key('I'), 8 }, value_type{Key('H'), 7 }, value_type{Key('G'), 6 }, value_type{Key('F'), 5 },
                                  value_type{Key('E'), 4 }, value_type{Key('D'), 3 }, value_type{Key('C'), 2 }, value_type{Key('B'), 1 }, value_type{Key('A'), 0 } };
#else
      static constexpr Data data{ value_type{Key('A'), 0 }, value_type{Key('B'), 1 }, value_type{Key('C'), 2 }, value_type{Key('D'), 3 }, value_type{Key('E'), 4 },
                                  value_type{Key('F'), 5 }, value_type{Key('G'), 6 }, value_type{Key('H'), 7 }, value_type{Key('I'), 8 }, value_type{Key('J'), 9 } };
#endif

#ifdef TEST_GREATER_THAN
      etl::const_map<Key, int, 10U> check{ value_type{Key('J'), 9 }, value_type{Key('I'), 8 }, value_type{Key('H'), 7 }, value_type{Key('G'), 6 }, value_type{Key('F'), 5 },
                                           value_type{Key('E'), 4 }, value_type{Key('D'), 3 }, value_type{Key('C'), 2 }, value_type{Key('B'), 1 }, value_type{Key('A'), 0 } };
#else
      etl::const_map<Key, int, 10U> check{ value_type{Key('A'), 0 }, value_type{Key('B'), 1 }, value_type{Key('C'), 2 }, value_type{Key('D'), 3 }, value_type{Key('E'), 4 },
                                           value_type{Key('F'), 5 }, value_type{Key('G'), 6 }, value_type{Key('H'), 7 }, value_type{Key('I'), 8 }, value_type{Key('J'), 9 } };
#endif
      CHECK_TRUE(data.is_valid());
      CHECK_TRUE(data.size() == Max_Size);
      CHECK_FALSE(data.empty());
      CHECK_TRUE(data.full());
      CHECK_TRUE(data.capacity() == Max_Size);
      CHECK_TRUE(data.max_size() == Max_Size);
      CHECK_FALSE(data.begin() == data.end());
    }
#endif

    //*************************************************************************
    TEST(test_begin)
    {
#ifdef TEST_GREATER_THAN
      static constexpr Data data{ value_type{Key('J'), 9 }, value_type{Key('I'), 8 }, value_type{Key('H'), 7 }, value_type{Key('G'), 6 }, value_type{Key('F'), 5 },
                                  value_type{Key('E'), 4 }, value_type{Key('D'), 3 }, value_type{Key('C'), 2 }, value_type{Key('B'), 1 }, value_type{Key('A'), 0 } };
#else
      static constexpr Data data{ value_type{Key('A'), 0 }, value_type{Key('B'), 1 }, value_type{Key('C'), 2 }, value_type{Key('D'), 3 }, value_type{Key('E'), 4 },
                                  value_type{Key('F'), 5 }, value_type{Key('G'), 6 }, value_type{Key('H'), 7 }, value_type{Key('I'), 8 }, value_type{Key('J'), 9 } };
#endif

      CHECK_TRUE(data.is_valid());
      auto value = *data.begin();

#ifdef TEST_GREATER_THAN
      CHECK_TRUE((value_type{ Key('J'), 9 }) == value);
#else
      CHECK_TRUE((value_type{ Key('A'), 0 }) == value);
#endif
    }

    //*************************************************************************
    TEST(test_begin_constexpr)
    {
#ifdef TEST_GREATER_THAN
      static constexpr Data data{ value_type{Key('J'), 9 }, value_type{Key('I'), 8 }, value_type{Key('H'), 7 }, value_type{Key('G'), 6 }, value_type{Key('F'), 5 },
                                  value_type{Key('E'), 4 }, value_type{Key('D'), 3 }, value_type{Key('C'), 2 }, value_type{Key('B'), 1 }, value_type{Key('A'), 0 } };
#else
      static constexpr Data data{ value_type{Key('A'), 0 }, value_type{Key('B'), 1 }, value_type{Key('C'), 2 }, value_type{Key('D'), 3 }, value_type{Key('E'), 4 },
                                  value_type{Key('F'), 5 }, value_type{Key('G'), 6 }, value_type{Key('H'), 7 }, value_type{Key('I'), 8 }, value_type{Key('J'), 9 } };
#endif

      CHECK_TRUE(data.is_valid());
      static constexpr auto value = *data.begin();

#ifdef TEST_GREATER_THAN
      CHECK_TRUE((value_type{ Key('J'), 9 }) == value);
#else
      CHECK_TRUE((value_type{ Key('A'), 0 }) == value);
#endif
    }

    //*************************************************************************
    TEST(test_end)
    {
      static constexpr Data data{ value_type{Key('A'), 0 }, value_type{Key('B'), 1 }, value_type{Key('C'), 2 }, value_type{Key('D'), 3 }, value_type{Key('E'), 4 },
                                  value_type{Key('F'), 5 }, value_type{Key('G'), 6 }, value_type{Key('H'), 7 }, value_type{Key('I'), 8 }, value_type{Key('J'), 9 } };

      CHECK_TRUE(data.end() == (data.begin() + data.size()));
    }

    //*************************************************************************
    TEST(test_end_constexpr)
    {
      static constexpr Data data{ value_type{Key('A'), 0 }, value_type{Key('B'), 1 }, value_type{Key('C'), 2 }, value_type{Key('D'), 3 }, value_type{Key('E'), 4 },
                                  value_type{Key('F'), 5 }, value_type{Key('G'), 6 }, value_type{Key('H'), 7 }, value_type{Key('I'), 8 }, value_type{Key('J'), 9 } };

      static constexpr Data::const_iterator end_itr = data.end();

      CHECK_TRUE(end_itr == (data.begin() + data.size()));
    }

    //*************************************************************************
    TEST(test_index)
    {
#ifdef TEST_GREATER_THAN
      static constexpr Data data{ value_type{Key('J'), 9 }, value_type{Key('I'), 8 }, value_type{Key('H'), 7 }, value_type{Key('G'), 6 }, value_type{Key('F'), 5 },
                                  value_type{Key('E'), 4 }, value_type{Key('D'), 3 }, value_type{Key('C'), 2 }, value_type{Key('B'), 1 }, value_type{Key('A'), 0 } };
#else
      static constexpr Data data{ value_type{Key('A'), 0 }, value_type{Key('B'), 1 }, value_type{Key('C'), 2 }, value_type{Key('D'), 3 }, value_type{Key('E'), 4 },
                                  value_type{Key('F'), 5 }, value_type{Key('G'), 6 }, value_type{Key('H'), 7 }, value_type{Key('I'), 8 }, value_type{Key('J'), 9 } };
#endif

      CHECK_TRUE(data.is_valid());
      CHECK(data[Key('A')] == 0);
      CHECK(data[Key('B')] == 1);
      CHECK(data[Key('C')] == 2);
      CHECK(data[Key('D')] == 3);
      CHECK(data[Key('E')] == 4);
      CHECK(data[Key('F')] == 5);
      CHECK(data[Key('G')] == 6);
      CHECK(data[Key('H')] == 7);
      CHECK(data[Key('I')] == 8);
      CHECK(data[Key('J')] == 9);
    }

    //*************************************************************************
    TEST(test_index_constexpr)
    {
#ifdef TEST_GREATER_THAN
      static constexpr Data data{ value_type{Key('J'), 9 }, value_type{Key('I'), 8 }, value_type{Key('H'), 7 }, value_type{Key('G'), 6 }, value_type{Key('F'), 5 },
                                  value_type{Key('E'), 4 }, value_type{Key('D'), 3 }, value_type{Key('C'), 2 }, value_type{Key('B'), 1 }, value_type{Key('A'), 0 } };
#else
      static constexpr Data data{ value_type{Key('A'), 0 }, value_type{Key('B'), 1 }, value_type{Key('C'), 2 }, value_type{Key('D'), 3 }, value_type{Key('E'), 4 },
                                  value_type{Key('F'), 5 }, value_type{Key('G'), 6 }, value_type{Key('H'), 7 }, value_type{Key('I'), 8 }, value_type{Key('J'), 9 } };
#endif

      static constexpr Data::mapped_type atA = data[Key('A')];
      static constexpr Data::mapped_type atB = data[Key('B')];
      static constexpr Data::mapped_type atC = data[Key('C')];
      static constexpr Data::mapped_type atD = data[Key('D')];
      static constexpr Data::mapped_type atE = data[Key('E')];
      static constexpr Data::mapped_type atF = data[Key('F')];
      static constexpr Data::mapped_type atG = data[Key('G')];
      static constexpr Data::mapped_type atH = data[Key('H')];
      static constexpr Data::mapped_type atI = data[Key('I')];
      static constexpr Data::mapped_type atJ = data[Key('J')];

      CHECK_TRUE(data.is_valid());
      CHECK(atA == 0);
      CHECK(atB == 1);
      CHECK(atC == 2);
      CHECK(atD == 3);
      CHECK(atE == 4);
      CHECK(atF == 5);
      CHECK(atG == 6);
      CHECK(atH == 7);
      CHECK(atI == 8);
      CHECK(atJ == 9);
    }

    //*************************************************************************
    TEST(test_index_using_transparent_comparator)
    {
#ifdef TEST_GREATER_THAN
      static constexpr DataTransparentComparator data{ value_type{Key('J'), 9 }, value_type{Key('I'), 8 }, value_type{Key('H'), 7 }, value_type{Key('G'), 6 }, value_type{Key('F'), 5 },
                                                       value_type{Key('E'), 4 }, value_type{Key('D'), 3 }, value_type{Key('C'), 2 }, value_type{Key('B'), 1 }, value_type{Key('A'), 0 } };
#else
      static constexpr DataTransparentComparator data{ value_type{Key('A'), 0 }, value_type{Key('B'), 1 }, value_type{Key('C'), 2 }, value_type{Key('D'), 3 }, value_type{Key('E'), 4 },
                                                       value_type{Key('F'), 5 }, value_type{Key('G'), 6 }, value_type{Key('H'), 7 }, value_type{Key('I'), 8 }, value_type{Key('J'), 9 } };
#endif

      CHECK_TRUE(data.is_valid());
      CHECK(data['A'] == 0);
      CHECK(data['B'] == 1);
      CHECK(data['C'] == 2);
      CHECK(data['D'] == 3);
      CHECK(data['E'] == 4);
      CHECK(data['F'] == 5);
      CHECK(data['G'] == 6);
      CHECK(data['H'] == 7);
      CHECK(data['I'] == 8);
      CHECK(data['J'] == 9);
    }

    //*************************************************************************
    TEST(test_index_constexpr_using_transparent_comparator)
    {
#ifdef TEST_GREATER_THAN
      static constexpr DataTransparentComparator data{ value_type{Key('J'), 9 }, value_type{Key('I'), 8 }, value_type{Key('H'), 7 }, value_type{Key('G'), 6 }, value_type{Key('F'), 5 },
                                                       value_type{Key('E'), 4 }, value_type{Key('D'), 3 }, value_type{Key('C'), 2 }, value_type{Key('B'), 1 }, value_type{Key('A'), 0 } };
#else
      static constexpr DataTransparentComparator data{ value_type{Key('A'), 0 }, value_type{Key('B'), 1 }, value_type{Key('C'), 2 }, value_type{Key('D'), 3 }, value_type{Key('E'), 4 },
                                                       value_type{Key('F'), 5 }, value_type{Key('G'), 6 }, value_type{Key('H'), 7 }, value_type{Key('I'), 8 }, value_type{Key('J'), 9 } };
#endif

      static constexpr Data::mapped_type atA = data['A'];
      static constexpr Data::mapped_type atB = data['B'];
      static constexpr Data::mapped_type atC = data['C'];
      static constexpr Data::mapped_type atD = data['D'];
      static constexpr Data::mapped_type atE = data['E'];
      static constexpr Data::mapped_type atF = data['F'];
      static constexpr Data::mapped_type atG = data['G'];
      static constexpr Data::mapped_type atH = data['H'];
      static constexpr Data::mapped_type atI = data['I'];
      static constexpr Data::mapped_type atJ = data['J'];

      CHECK_TRUE(data.is_valid());
      CHECK(atA == 0);
      CHECK(atB == 1);
      CHECK(atC == 2);
      CHECK(atD == 3);
      CHECK(atE == 4);
      CHECK(atF == 5);
      CHECK(atG == 6);
      CHECK(atH == 7);
      CHECK(atI == 8);
      CHECK(atJ == 9);
    }

    //*************************************************************************
    TEST(test_at)
    {
#ifdef TEST_GREATER_THAN
    static constexpr Data data{ value_type{Key('J'), 9 }, value_type{Key('I'), 8 }, value_type{Key('H'), 7 }, value_type{Key('G'), 6 }, value_type{Key('F'), 5 },
                                value_type{Key('E'), 4 }, value_type{Key('D'), 3 }, value_type{Key('C'), 2 }, value_type{Key('B'), 1 }, value_type{Key('A'), 0 } };
#else
    static constexpr Data data{ value_type{Key('A'), 0 }, value_type{Key('B'), 1 }, value_type{Key('C'), 2 }, value_type{Key('D'), 3 }, value_type{Key('E'), 4 },
                                value_type{Key('F'), 5 }, value_type{Key('G'), 6 }, value_type{Key('H'), 7 }, value_type{Key('I'), 8 }, value_type{Key('J'), 9 } };
#endif

      CHECK_TRUE(data.is_valid());
      CHECK(data.at(Key('A')) == 0);
      CHECK(data.at(Key('B')) == 1);
      CHECK(data.at(Key('C')) == 2);
      CHECK(data.at(Key('D')) == 3);
      CHECK(data.at(Key('E')) == 4);
      CHECK(data.at(Key('F')) == 5);
      CHECK(data.at(Key('G')) == 6);
      CHECK(data.at(Key('H')) == 7);
      CHECK(data.at(Key('I')) == 8);
      CHECK(data.at(Key('J')) == 9);
    }

    //*************************************************************************
    TEST(test_at_constexpr)
    {
#ifdef TEST_GREATER_THAN
      static constexpr Data data{ value_type{Key('J'), 9 }, value_type{Key('I'), 8 }, value_type{Key('H'), 7 }, value_type{Key('G'), 6 }, value_type{Key('F'), 5 },
                                  value_type{Key('E'), 4 }, value_type{Key('D'), 3 }, value_type{Key('C'), 2 }, value_type{Key('B'), 1 }, value_type{Key('A'), 0 } };
#else
      static constexpr Data data{ value_type{Key('A'), 0 }, value_type{Key('B'), 1 }, value_type{Key('C'), 2 }, value_type{Key('D'), 3 }, value_type{Key('E'), 4 },
                                  value_type{Key('F'), 5 }, value_type{Key('G'), 6 }, value_type{Key('H'), 7 }, value_type{Key('I'), 8 }, value_type{Key('J'), 9 } };
#endif

      static constexpr Data::mapped_type atA = data.at(Key('A'));
      static constexpr Data::mapped_type atB = data.at(Key('B'));
      static constexpr Data::mapped_type atC = data.at(Key('C'));
      static constexpr Data::mapped_type atD = data.at(Key('D'));
      static constexpr Data::mapped_type atE = data.at(Key('E'));
      static constexpr Data::mapped_type atF = data.at(Key('F'));
      static constexpr Data::mapped_type atG = data.at(Key('G'));
      static constexpr Data::mapped_type atH = data.at(Key('H'));
      static constexpr Data::mapped_type atI = data.at(Key('I'));
      static constexpr Data::mapped_type atJ = data.at(Key('J'));

      CHECK_TRUE(data.is_valid());
      CHECK(atA == 0);
      CHECK(atB == 1);
      CHECK(atC == 2);
      CHECK(atD == 3);
      CHECK(atE == 4);
      CHECK(atF == 5);
      CHECK(atG == 6);
      CHECK(atH == 7);
      CHECK(atI == 8);
      CHECK(atJ == 9);
    }

    //*************************************************************************
    TEST(test_at_using_transparent_comparator)
    {
      #ifdef TEST_GREATER_THAN
          static constexpr DataTransparentComparator data{ value_type{Key('J'), 9 }, value_type{Key('I'), 8 }, value_type{Key('H'), 7 }, value_type{Key('G'), 6 }, value_type{Key('F'), 5 },
                                                           value_type{Key('E'), 4 }, value_type{Key('D'), 3 }, value_type{Key('C'), 2 }, value_type{Key('B'), 1 }, value_type{Key('A'), 0 } };
      #else
          static constexpr DataTransparentComparator data{ value_type{Key('A'), 0 }, value_type{Key('B'), 1 }, value_type{Key('C'), 2 }, value_type{Key('D'), 3 }, value_type{Key('E'), 4 },
                                                           value_type{Key('F'), 5 }, value_type{Key('G'), 6 }, value_type{Key('H'), 7 }, value_type{Key('I'), 8 }, value_type{Key('J'), 9 } };
      #endif

          CHECK_TRUE(data.is_valid());
          CHECK(data.at('A') == 0);
          CHECK(data.at('B') == 1);
          CHECK(data.at('C') == 2);
          CHECK(data.at('D') == 3);
          CHECK(data.at('E') == 4);
          CHECK(data.at('F') == 5);
          CHECK(data.at('G') == 6);
          CHECK(data.at('H') == 7);
          CHECK(data.at('I') == 8);
          CHECK(data.at('J') == 9);
    }

    //*************************************************************************
    TEST(test_at_constexpr_using_transparent_comparator)
    {
#ifdef TEST_GREATER_THAN
      static constexpr DataTransparentComparator data{ value_type{Key('J'), 9 }, value_type{Key('I'), 8 }, value_type{Key('H'), 7 }, value_type{Key('G'), 6 }, value_type{Key('F'), 5 },
                                                       value_type{Key('E'), 4 }, value_type{Key('D'), 3 }, value_type{Key('C'), 2 }, value_type{Key('B'), 1 }, value_type{Key('A'), 0 } };
#else
      static constexpr DataTransparentComparator data{ value_type{Key('A'), 0 }, value_type{Key('B'), 1 }, value_type{Key('C'), 2 }, value_type{Key('D'), 3 }, value_type{Key('E'), 4 },
                                                       value_type{Key('F'), 5 }, value_type{Key('G'), 6 }, value_type{Key('H'), 7 }, value_type{Key('I'), 8 }, value_type{Key('J'), 9 } };
#endif

      static constexpr Data::mapped_type atA = data.at('A');
      static constexpr Data::mapped_type atB = data.at('B');
      static constexpr Data::mapped_type atC = data.at('C');
      static constexpr Data::mapped_type atD = data.at('D');
      static constexpr Data::mapped_type atE = data.at('E');
      static constexpr Data::mapped_type atF = data.at('F');
      static constexpr Data::mapped_type atG = data.at('G');
      static constexpr Data::mapped_type atH = data.at('H');
      static constexpr Data::mapped_type atI = data.at('I');
      static constexpr Data::mapped_type atJ = data.at('J');

      CHECK_TRUE(data.is_valid());
      CHECK(atA == 0);
      CHECK(atB == 1);
      CHECK(atC == 2);
      CHECK(atD == 3);
      CHECK(atE == 4);
      CHECK(atF == 5);
      CHECK(atG == 6);
      CHECK(atH == 7);
      CHECK(atI == 8);
      CHECK(atJ == 9);
    }

    //*************************************************************************
    TEST(test_equal_range)
    {
#ifdef TEST_GREATER_THAN
      static constexpr Data data{ value_type{Key('J'), 9 }, value_type{Key('I'), 8 }, value_type{Key('H'), 7 }, value_type{Key('G'), 6 }, value_type{Key('F'), 5 },
                                  value_type{Key('E'), 4 }, value_type{Key('D'), 3 }, value_type{Key('C'), 2 }, value_type{Key('B'), 1 }, value_type{Key('A'), 0 } };
#else
      static constexpr Data data{ value_type{Key('A'), 0 }, value_type{Key('B'), 1 }, value_type{Key('C'), 2 }, value_type{Key('D'), 3 }, value_type{Key('E'), 4 },
                                  value_type{Key('F'), 5 }, value_type{Key('G'), 6 }, value_type{Key('H'), 7 }, value_type{Key('I'), 8 }, value_type{Key('J'), 9 } };
#endif

      ETL_OR_STD::pair<Data::const_iterator, Data::const_iterator> result0 = data.equal_range(Key('A'));
      ETL_OR_STD::pair<Data::const_iterator, Data::const_iterator> result1 = data.equal_range(Key('B'));
      ETL_OR_STD::pair<Data::const_iterator, Data::const_iterator> result2 = data.equal_range(Key('C'));
      ETL_OR_STD::pair<Data::const_iterator, Data::const_iterator> result3 = data.equal_range(Key('D'));
      ETL_OR_STD::pair<Data::const_iterator, Data::const_iterator> result4 = data.equal_range(Key('E'));
      ETL_OR_STD::pair<Data::const_iterator, Data::const_iterator> result5 = data.equal_range(Key('F'));
      ETL_OR_STD::pair<Data::const_iterator, Data::const_iterator> result6 = data.equal_range(Key('G'));
      ETL_OR_STD::pair<Data::const_iterator, Data::const_iterator> result7 = data.equal_range(Key('H'));
      ETL_OR_STD::pair<Data::const_iterator, Data::const_iterator> result8 = data.equal_range(Key('I'));
      ETL_OR_STD::pair<Data::const_iterator, Data::const_iterator> result9 = data.equal_range(Key('J'));

#ifdef TEST_GREATER_THAN
      CHECK_EQUAL(9, (std::distance(data.begin(), result0.first)));
      CHECK_EQUAL(8, (std::distance(data.begin(), result1.first)));
      CHECK_EQUAL(7, (std::distance(data.begin(), result2.first)));
      CHECK_EQUAL(6, (std::distance(data.begin(), result3.first)));
      CHECK_EQUAL(5, (std::distance(data.begin(), result4.first)));
      CHECK_EQUAL(4, (std::distance(data.begin(), result5.first)));
      CHECK_EQUAL(3, (std::distance(data.begin(), result6.first)));
      CHECK_EQUAL(2, (std::distance(data.begin(), result7.first)));
      CHECK_EQUAL(1, (std::distance(data.begin(), result8.first)));
      CHECK_EQUAL(0, (std::distance(data.begin(), result9.first)));

      CHECK_EQUAL(10, (std::distance(data.begin(), result0.second)));
      CHECK_EQUAL(9, (std::distance(data.begin(), result1.second)));
      CHECK_EQUAL(8, (std::distance(data.begin(), result2.second)));
      CHECK_EQUAL(7, (std::distance(data.begin(), result3.second)));
      CHECK_EQUAL(6, (std::distance(data.begin(), result4.second)));
      CHECK_EQUAL(5, (std::distance(data.begin(), result5.second)));
      CHECK_EQUAL(4, (std::distance(data.begin(), result6.second)));
      CHECK_EQUAL(3, (std::distance(data.begin(), result7.second)));
      CHECK_EQUAL(2, (std::distance(data.begin(), result8.second)));
      CHECK_EQUAL(1, (std::distance(data.begin(), result9.second)));
#else
      CHECK_EQUAL(0, (std::distance(data.begin(), result0.first)));
      CHECK_EQUAL(1, (std::distance(data.begin(), result1.first)));
      CHECK_EQUAL(2, (std::distance(data.begin(), result2.first)));
      CHECK_EQUAL(3, (std::distance(data.begin(), result3.first)));
      CHECK_EQUAL(4, (std::distance(data.begin(), result4.first)));
      CHECK_EQUAL(5, (std::distance(data.begin(), result5.first)));
      CHECK_EQUAL(6, (std::distance(data.begin(), result6.first)));
      CHECK_EQUAL(7, (std::distance(data.begin(), result7.first)));
      CHECK_EQUAL(8, (std::distance(data.begin(), result8.first)));
      CHECK_EQUAL(9, (std::distance(data.begin(), result9.first)));

      CHECK_EQUAL(1, (std::distance(data.begin(), result0.second)));
      CHECK_EQUAL(2, (std::distance(data.begin(), result1.second)));
      CHECK_EQUAL(3, (std::distance(data.begin(), result2.second)));
      CHECK_EQUAL(4, (std::distance(data.begin(), result3.second)));
      CHECK_EQUAL(5, (std::distance(data.begin(), result4.second)));
      CHECK_EQUAL(6, (std::distance(data.begin(), result5.second)));
      CHECK_EQUAL(7, (std::distance(data.begin(), result6.second)));
      CHECK_EQUAL(8, (std::distance(data.begin(), result7.second)));
      CHECK_EQUAL(9, (std::distance(data.begin(), result8.second)));
      CHECK_EQUAL(10, (std::distance(data.begin(), result9.second)));
#endif
    }

    //*************************************************************************
    TEST(test_equal_range_constexpr)
    {
#ifdef TEST_GREATER_THAN
      static constexpr Data data{ value_type{Key('J'), 9 }, value_type{Key('I'), 8 }, value_type{Key('H'), 7 }, value_type{Key('G'), 6 }, value_type{Key('F'), 5 },
                                  value_type{Key('E'), 4 }, value_type{Key('D'), 3 }, value_type{Key('C'), 2 }, value_type{Key('B'), 1 }, value_type{Key('A'), 0 } };
#else
      static constexpr Data data{ value_type{Key('A'), 0 }, value_type{Key('B'), 1 }, value_type{Key('C'), 2 }, value_type{Key('D'), 3 }, value_type{Key('E'), 4 },
                                  value_type{Key('F'), 5 }, value_type{Key('G'), 6 }, value_type{Key('H'), 7 }, value_type{Key('I'), 8 }, value_type{Key('J'), 9 } };
#endif

      static constexpr ETL_OR_STD::pair<Data::const_iterator, Data::const_iterator> result0 = data.equal_range(Key('A'));
      static constexpr ETL_OR_STD::pair<Data::const_iterator, Data::const_iterator> result1 = data.equal_range(Key('B'));
      static constexpr ETL_OR_STD::pair<Data::const_iterator, Data::const_iterator> result2 = data.equal_range(Key('C'));
      static constexpr ETL_OR_STD::pair<Data::const_iterator, Data::const_iterator> result3 = data.equal_range(Key('D'));
      static constexpr ETL_OR_STD::pair<Data::const_iterator, Data::const_iterator> result4 = data.equal_range(Key('E'));
      static constexpr ETL_OR_STD::pair<Data::const_iterator, Data::const_iterator> result5 = data.equal_range(Key('F'));
      static constexpr ETL_OR_STD::pair<Data::const_iterator, Data::const_iterator> result6 = data.equal_range(Key('G'));
      static constexpr ETL_OR_STD::pair<Data::const_iterator, Data::const_iterator> result7 = data.equal_range(Key('H'));
      static constexpr ETL_OR_STD::pair<Data::const_iterator, Data::const_iterator> result8 = data.equal_range(Key('I'));
      static constexpr ETL_OR_STD::pair<Data::const_iterator, Data::const_iterator> result9 = data.equal_range(Key('J'));

#ifdef TEST_GREATER_THAN
      CHECK_EQUAL(9, (std::distance(data.begin(), result0.first)));
      CHECK_EQUAL(8, (std::distance(data.begin(), result1.first)));
      CHECK_EQUAL(7, (std::distance(data.begin(), result2.first)));
      CHECK_EQUAL(6, (std::distance(data.begin(), result3.first)));
      CHECK_EQUAL(5, (std::distance(data.begin(), result4.first)));
      CHECK_EQUAL(4, (std::distance(data.begin(), result5.first)));
      CHECK_EQUAL(3, (std::distance(data.begin(), result6.first)));
      CHECK_EQUAL(2, (std::distance(data.begin(), result7.first)));
      CHECK_EQUAL(1, (std::distance(data.begin(), result8.first)));
      CHECK_EQUAL(0, (std::distance(data.begin(), result9.first)));

      CHECK_EQUAL(10, (std::distance(data.begin(), result0.second)));
      CHECK_EQUAL(9, (std::distance(data.begin(), result1.second)));
      CHECK_EQUAL(8, (std::distance(data.begin(), result2.second)));
      CHECK_EQUAL(7, (std::distance(data.begin(), result3.second)));
      CHECK_EQUAL(6, (std::distance(data.begin(), result4.second)));
      CHECK_EQUAL(5, (std::distance(data.begin(), result5.second)));
      CHECK_EQUAL(4, (std::distance(data.begin(), result6.second)));
      CHECK_EQUAL(3, (std::distance(data.begin(), result7.second)));
      CHECK_EQUAL(2, (std::distance(data.begin(), result8.second)));
      CHECK_EQUAL(1, (std::distance(data.begin(), result9.second)));
#else
      CHECK_EQUAL(0, (std::distance(data.begin(), result0.first)));
      CHECK_EQUAL(1, (std::distance(data.begin(), result1.first)));
      CHECK_EQUAL(2, (std::distance(data.begin(), result2.first)));
      CHECK_EQUAL(3, (std::distance(data.begin(), result3.first)));
      CHECK_EQUAL(4, (std::distance(data.begin(), result4.first)));
      CHECK_EQUAL(5, (std::distance(data.begin(), result5.first)));
      CHECK_EQUAL(6, (std::distance(data.begin(), result6.first)));
      CHECK_EQUAL(7, (std::distance(data.begin(), result7.first)));
      CHECK_EQUAL(8, (std::distance(data.begin(), result8.first)));
      CHECK_EQUAL(9, (std::distance(data.begin(), result9.first)));

      CHECK_EQUAL(1, (std::distance(data.begin(), result0.second)));
      CHECK_EQUAL(2, (std::distance(data.begin(), result1.second)));
      CHECK_EQUAL(3, (std::distance(data.begin(), result2.second)));
      CHECK_EQUAL(4, (std::distance(data.begin(), result3.second)));
      CHECK_EQUAL(5, (std::distance(data.begin(), result4.second)));
      CHECK_EQUAL(6, (std::distance(data.begin(), result5.second)));
      CHECK_EQUAL(7, (std::distance(data.begin(), result6.second)));
      CHECK_EQUAL(8, (std::distance(data.begin(), result7.second)));
      CHECK_EQUAL(9, (std::distance(data.begin(), result8.second)));
      CHECK_EQUAL(10, (std::distance(data.begin(), result9.second)));
#endif
    }

    //*************************************************************************
    TEST(test_equal_range_using_transparent_comparator)
    {
#ifdef TEST_GREATER_THAN
      static constexpr DataTransparentComparator data{ value_type{Key('J'), 9 }, value_type{Key('I'), 8 }, value_type{Key('H'), 7 }, value_type{Key('G'), 6 }, value_type{Key('F'), 5 },
                                                       value_type{Key('E'), 4 }, value_type{Key('D'), 3 }, value_type{Key('C'), 2 }, value_type{Key('B'), 1 }, value_type{Key('A'), 0 } };
#else
      static constexpr DataTransparentComparator data{ value_type{Key('A'), 0 }, value_type{Key('B'), 1 }, value_type{Key('C'), 2 }, value_type{Key('D'), 3 }, value_type{Key('E'), 4 },
                                                       value_type{Key('F'), 5 }, value_type{Key('G'), 6 }, value_type{Key('H'), 7 }, value_type{Key('I'), 8 }, value_type{Key('J'), 9 } };
#endif

      ETL_OR_STD::pair<Data::const_iterator, Data::const_iterator> result0 = data.equal_range('A');
      ETL_OR_STD::pair<Data::const_iterator, Data::const_iterator> result1 = data.equal_range('B');
      ETL_OR_STD::pair<Data::const_iterator, Data::const_iterator> result2 = data.equal_range('C');
      ETL_OR_STD::pair<Data::const_iterator, Data::const_iterator> result3 = data.equal_range('D');
      ETL_OR_STD::pair<Data::const_iterator, Data::const_iterator> result4 = data.equal_range('E');
      ETL_OR_STD::pair<Data::const_iterator, Data::const_iterator> result5 = data.equal_range('F');
      ETL_OR_STD::pair<Data::const_iterator, Data::const_iterator> result6 = data.equal_range('G');
      ETL_OR_STD::pair<Data::const_iterator, Data::const_iterator> result7 = data.equal_range('H');
      ETL_OR_STD::pair<Data::const_iterator, Data::const_iterator> result8 = data.equal_range('I');
      ETL_OR_STD::pair<Data::const_iterator, Data::const_iterator> result9 = data.equal_range('J');

#ifdef TEST_GREATER_THAN
      CHECK_EQUAL(9, (std::distance(data.begin(), result0.first)));
      CHECK_EQUAL(8, (std::distance(data.begin(), result1.first)));
      CHECK_EQUAL(7, (std::distance(data.begin(), result2.first)));
      CHECK_EQUAL(6, (std::distance(data.begin(), result3.first)));
      CHECK_EQUAL(5, (std::distance(data.begin(), result4.first)));
      CHECK_EQUAL(4, (std::distance(data.begin(), result5.first)));
      CHECK_EQUAL(3, (std::distance(data.begin(), result6.first)));
      CHECK_EQUAL(2, (std::distance(data.begin(), result7.first)));
      CHECK_EQUAL(1, (std::distance(data.begin(), result8.first)));
      CHECK_EQUAL(0, (std::distance(data.begin(), result9.first)));

      CHECK_EQUAL(10, (std::distance(data.begin(), result0.second)));
      CHECK_EQUAL(9, (std::distance(data.begin(), result1.second)));
      CHECK_EQUAL(8, (std::distance(data.begin(), result2.second)));
      CHECK_EQUAL(7, (std::distance(data.begin(), result3.second)));
      CHECK_EQUAL(6, (std::distance(data.begin(), result4.second)));
      CHECK_EQUAL(5, (std::distance(data.begin(), result5.second)));
      CHECK_EQUAL(4, (std::distance(data.begin(), result6.second)));
      CHECK_EQUAL(3, (std::distance(data.begin(), result7.second)));
      CHECK_EQUAL(2, (std::distance(data.begin(), result8.second)));
      CHECK_EQUAL(1, (std::distance(data.begin(), result9.second)));
#else
      CHECK_EQUAL(0, (std::distance(data.begin(), result0.first)));
      CHECK_EQUAL(1, (std::distance(data.begin(), result1.first)));
      CHECK_EQUAL(2, (std::distance(data.begin(), result2.first)));
      CHECK_EQUAL(3, (std::distance(data.begin(), result3.first)));
      CHECK_EQUAL(4, (std::distance(data.begin(), result4.first)));
      CHECK_EQUAL(5, (std::distance(data.begin(), result5.first)));
      CHECK_EQUAL(6, (std::distance(data.begin(), result6.first)));
      CHECK_EQUAL(7, (std::distance(data.begin(), result7.first)));
      CHECK_EQUAL(8, (std::distance(data.begin(), result8.first)));
      CHECK_EQUAL(9, (std::distance(data.begin(), result9.first)));

      CHECK_EQUAL(1, (std::distance(data.begin(), result0.second)));
      CHECK_EQUAL(2, (std::distance(data.begin(), result1.second)));
      CHECK_EQUAL(3, (std::distance(data.begin(), result2.second)));
      CHECK_EQUAL(4, (std::distance(data.begin(), result3.second)));
      CHECK_EQUAL(5, (std::distance(data.begin(), result4.second)));
      CHECK_EQUAL(6, (std::distance(data.begin(), result5.second)));
      CHECK_EQUAL(7, (std::distance(data.begin(), result6.second)));
      CHECK_EQUAL(8, (std::distance(data.begin(), result7.second)));
      CHECK_EQUAL(9, (std::distance(data.begin(), result8.second)));
      CHECK_EQUAL(10, (std::distance(data.begin(), result9.second)));
#endif
    }

    //*************************************************************************
    TEST(test_equal_range_constexpr_using_transparent_comparator)
    {
#ifdef TEST_GREATER_THAN
      static constexpr DataTransparentComparator data{ value_type{Key('J'), 9 }, value_type{Key('I'), 8 }, value_type{Key('H'), 7 }, value_type{Key('G'), 6 }, value_type{Key('F'), 5 },
                                                       value_type{Key('E'), 4 }, value_type{Key('D'), 3 }, value_type{Key('C'), 2 }, value_type{Key('B'), 1 }, value_type{Key('A'), 0 } };
#else
      static constexpr DataTransparentComparator data{ value_type{Key('A'), 0 }, value_type{Key('B'), 1 }, value_type{Key('C'), 2 }, value_type{Key('D'), 3 }, value_type{Key('E'), 4 },
                                                       value_type{Key('F'), 5 }, value_type{Key('G'), 6 }, value_type{Key('H'), 7 }, value_type{Key('I'), 8 }, value_type{Key('J'), 9 } };
#endif

      static constexpr ETL_OR_STD::pair<Data::const_iterator, Data::const_iterator> result0 = data.equal_range('A');
      static constexpr ETL_OR_STD::pair<Data::const_iterator, Data::const_iterator> result1 = data.equal_range('B');
      static constexpr ETL_OR_STD::pair<Data::const_iterator, Data::const_iterator> result2 = data.equal_range('C');
      static constexpr ETL_OR_STD::pair<Data::const_iterator, Data::const_iterator> result3 = data.equal_range('D');
      static constexpr ETL_OR_STD::pair<Data::const_iterator, Data::const_iterator> result4 = data.equal_range('E');
      static constexpr ETL_OR_STD::pair<Data::const_iterator, Data::const_iterator> result5 = data.equal_range('F');
      static constexpr ETL_OR_STD::pair<Data::const_iterator, Data::const_iterator> result6 = data.equal_range('G');
      static constexpr ETL_OR_STD::pair<Data::const_iterator, Data::const_iterator> result7 = data.equal_range('H');
      static constexpr ETL_OR_STD::pair<Data::const_iterator, Data::const_iterator> result8 = data.equal_range('I');
      static constexpr ETL_OR_STD::pair<Data::const_iterator, Data::const_iterator> result9 = data.equal_range('J');

#ifdef TEST_GREATER_THAN
      CHECK_EQUAL(9, (std::distance(data.begin(), result0.first)));
      CHECK_EQUAL(8, (std::distance(data.begin(), result1.first)));
      CHECK_EQUAL(7, (std::distance(data.begin(), result2.first)));
      CHECK_EQUAL(6, (std::distance(data.begin(), result3.first)));
      CHECK_EQUAL(5, (std::distance(data.begin(), result4.first)));
      CHECK_EQUAL(4, (std::distance(data.begin(), result5.first)));
      CHECK_EQUAL(3, (std::distance(data.begin(), result6.first)));
      CHECK_EQUAL(2, (std::distance(data.begin(), result7.first)));
      CHECK_EQUAL(1, (std::distance(data.begin(), result8.first)));
      CHECK_EQUAL(0, (std::distance(data.begin(), result9.first)));

      CHECK_EQUAL(10, (std::distance(data.begin(), result0.second)));
      CHECK_EQUAL(9, (std::distance(data.begin(), result1.second)));
      CHECK_EQUAL(8, (std::distance(data.begin(), result2.second)));
      CHECK_EQUAL(7, (std::distance(data.begin(), result3.second)));
      CHECK_EQUAL(6, (std::distance(data.begin(), result4.second)));
      CHECK_EQUAL(5, (std::distance(data.begin(), result5.second)));
      CHECK_EQUAL(4, (std::distance(data.begin(), result6.second)));
      CHECK_EQUAL(3, (std::distance(data.begin(), result7.second)));
      CHECK_EQUAL(2, (std::distance(data.begin(), result8.second)));
      CHECK_EQUAL(1, (std::distance(data.begin(), result9.second)));
#else
      CHECK_EQUAL(0, (std::distance(data.begin(), result0.first)));
      CHECK_EQUAL(1, (std::distance(data.begin(), result1.first)));
      CHECK_EQUAL(2, (std::distance(data.begin(), result2.first)));
      CHECK_EQUAL(3, (std::distance(data.begin(), result3.first)));
      CHECK_EQUAL(4, (std::distance(data.begin(), result4.first)));
      CHECK_EQUAL(5, (std::distance(data.begin(), result5.first)));
      CHECK_EQUAL(6, (std::distance(data.begin(), result6.first)));
      CHECK_EQUAL(7, (std::distance(data.begin(), result7.first)));
      CHECK_EQUAL(8, (std::distance(data.begin(), result8.first)));
      CHECK_EQUAL(9, (std::distance(data.begin(), result9.first)));

      CHECK_EQUAL(1, (std::distance(data.begin(), result0.second)));
      CHECK_EQUAL(2, (std::distance(data.begin(), result1.second)));
      CHECK_EQUAL(3, (std::distance(data.begin(), result2.second)));
      CHECK_EQUAL(4, (std::distance(data.begin(), result3.second)));
      CHECK_EQUAL(5, (std::distance(data.begin(), result4.second)));
      CHECK_EQUAL(6, (std::distance(data.begin(), result5.second)));
      CHECK_EQUAL(7, (std::distance(data.begin(), result6.second)));
      CHECK_EQUAL(8, (std::distance(data.begin(), result7.second)));
      CHECK_EQUAL(9, (std::distance(data.begin(), result8.second)));
      CHECK_EQUAL(10, (std::distance(data.begin(), result9.second)));
#endif
    }

    //*************************************************************************
    TEST(test_lower_bound)
    {
#ifdef TEST_GREATER_THAN
      static constexpr Data data{ value_type{Key('K'), 10 }, value_type{Key('J'), 9 }, value_type{Key('I'), 8 }, value_type{Key('H'), 7 }, value_type{Key('G'), 6 },
                                  value_type{Key('F'), 5 }, value_type{Key('E'), 4 }, value_type{Key('C'), 2 }, value_type{Key('B'), 1 }, value_type{Key('A'), 0 } };
#else
      static constexpr Data data{ value_type{Key('A'), 0 }, value_type{Key('B'), 1 }, value_type{Key('C'), 2 }, value_type{Key('E'), 4 }, value_type{Key('F'), 5 }, 
                                  value_type{Key('G'), 6 }, value_type{Key('H'), 7 }, value_type{Key('I'), 8 }, value_type{Key('J'), 9 }, value_type{Key('K'), 10 } };
#endif

      Data::const_iterator result0 = data.lower_bound(Key('A'));
      Data::const_iterator result1 = data.lower_bound(Key('B'));
      Data::const_iterator result2 = data.lower_bound(Key('C'));
      Data::const_iterator result3 = data.lower_bound(Key('D'));
      Data::const_iterator result4 = data.lower_bound(Key('E'));
      Data::const_iterator result5 = data.lower_bound(Key('F'));
      Data::const_iterator result6 = data.lower_bound(Key('G'));
      Data::const_iterator result7 = data.lower_bound(Key('H'));
      Data::const_iterator result8 = data.lower_bound(Key('I'));
      Data::const_iterator result9 = data.lower_bound(Key('J'));
      Data::const_iterator result10 = data.lower_bound(Key('K'));

#ifdef TEST_GREATER_THAN
      CHECK_EQUAL(10, (std::distance(data.begin(), result0)));
      CHECK_EQUAL(9, (std::distance(data.begin(), result1)));
      CHECK_EQUAL(8, (std::distance(data.begin(), result2)));
      CHECK_EQUAL(7, (std::distance(data.begin(), result3)));
      CHECK_EQUAL(6, (std::distance(data.begin(), result4)));
      CHECK_EQUAL(5, (std::distance(data.begin(), result5)));
      CHECK_EQUAL(4, (std::distance(data.begin(), result6)));
      CHECK_EQUAL(3, (std::distance(data.begin(), result7)));
      CHECK_EQUAL(2, (std::distance(data.begin(), result8)));
      CHECK_EQUAL(1, (std::distance(data.begin(), result9)));
      CHECK_EQUAL(0, (std::distance(data.begin(), result10)));
#else
      CHECK_EQUAL(0, (std::distance(data.begin(), result0)));
      CHECK_EQUAL(1, (std::distance(data.begin(), result1)));
      CHECK_EQUAL(2, (std::distance(data.begin(), result2)));
      CHECK_EQUAL(3, (std::distance(data.begin(), result3)));
      CHECK_EQUAL(3, (std::distance(data.begin(), result4)));
      CHECK_EQUAL(4, (std::distance(data.begin(), result5)));
      CHECK_EQUAL(5, (std::distance(data.begin(), result6)));
      CHECK_EQUAL(6, (std::distance(data.begin(), result7)));
      CHECK_EQUAL(7, (std::distance(data.begin(), result8)));
      CHECK_EQUAL(8, (std::distance(data.begin(), result9)));
      CHECK_EQUAL(9, (std::distance(data.begin(), result10)));
#endif
    }

    //*************************************************************************
    TEST(test_lower_bound_constexpr)
    {
#ifdef TEST_GREATER_THAN
      static constexpr Data data{ value_type{Key('K'), 10 }, value_type{Key('J'), 9 }, value_type{Key('I'), 8 }, value_type{Key('H'), 7 }, value_type{Key('G'), 6 },
                                  value_type{Key('F'), 5 }, value_type{Key('E'), 4 }, value_type{Key('C'), 2 }, value_type{Key('B'), 1 }, value_type{Key('A'), 0 } };
#else
      static constexpr Data data{ value_type{Key('A'), 0 }, value_type{Key('B'), 1 }, value_type{Key('C'), 2 }, value_type{Key('E'), 4 }, value_type{Key('F'), 5 }, 
                                  value_type{Key('G'), 6 }, value_type{Key('H'), 7 }, value_type{Key('I'), 8 }, value_type{Key('J'), 9 }, value_type{Key('K'), 10 } };
#endif

      static constexpr Data::const_iterator result0 = data.lower_bound(Key('A'));
      static constexpr Data::const_iterator result1 = data.lower_bound(Key('B'));
      static constexpr Data::const_iterator result2 = data.lower_bound(Key('C'));
      static constexpr Data::const_iterator result3 = data.lower_bound(Key('D'));
      static constexpr Data::const_iterator result4 = data.lower_bound(Key('E'));
      static constexpr Data::const_iterator result5 = data.lower_bound(Key('F'));
      static constexpr Data::const_iterator result6 = data.lower_bound(Key('G'));
      static constexpr Data::const_iterator result7 = data.lower_bound(Key('H'));
      static constexpr Data::const_iterator result8 = data.lower_bound(Key('I'));
      static constexpr Data::const_iterator result9 = data.lower_bound(Key('J'));
      static constexpr Data::const_iterator result10 = data.lower_bound(Key('K'));

#ifdef TEST_GREATER_THAN
      CHECK_EQUAL(10, (std::distance(data.begin(), result0)));
      CHECK_EQUAL(9, (std::distance(data.begin(), result1)));
      CHECK_EQUAL(8, (std::distance(data.begin(), result2)));
      CHECK_EQUAL(7, (std::distance(data.begin(), result3)));
      CHECK_EQUAL(6, (std::distance(data.begin(), result4)));
      CHECK_EQUAL(5, (std::distance(data.begin(), result5)));
      CHECK_EQUAL(4, (std::distance(data.begin(), result6)));
      CHECK_EQUAL(3, (std::distance(data.begin(), result7)));
      CHECK_EQUAL(2, (std::distance(data.begin(), result8)));
      CHECK_EQUAL(1, (std::distance(data.begin(), result9)));
      CHECK_EQUAL(0, (std::distance(data.begin(), result10)));
#else
      CHECK_EQUAL(0, (std::distance(data.begin(), result0)));
      CHECK_EQUAL(1, (std::distance(data.begin(), result1)));
      CHECK_EQUAL(2, (std::distance(data.begin(), result2)));
      CHECK_EQUAL(3, (std::distance(data.begin(), result3)));
      CHECK_EQUAL(3, (std::distance(data.begin(), result4)));
      CHECK_EQUAL(4, (std::distance(data.begin(), result5)));
      CHECK_EQUAL(5, (std::distance(data.begin(), result6)));
      CHECK_EQUAL(6, (std::distance(data.begin(), result7)));
      CHECK_EQUAL(7, (std::distance(data.begin(), result8)));
      CHECK_EQUAL(8, (std::distance(data.begin(), result9)));
      CHECK_EQUAL(9, (std::distance(data.begin(), result10)));
#endif
    }

    //*************************************************************************
    TEST(test_lower_bound_using_transparent_comparator)
    {

    }

    //*************************************************************************
    TEST(test_lower_bound_constexpr_using_transparent_comparator)
    {

    }

    //*************************************************************************
    TEST(test_upper_bound)
    {
    }

    //*************************************************************************
    TEST(test_upper_bound_constexpr)
    {
    }

    //*************************************************************************
    TEST(test_upper_bound_using_transparent_comparator)
    {
    }

    //*************************************************************************
    TEST(test_upper_bound_constexpr_using_transparent_comparator)
    {
    }

    //*************************************************************************
    TEST(test_count)
    {
#ifdef TEST_GREATER_THAN
      static constexpr Data data{ value_type{Key('J'), 9 }, value_type{Key('I'), 8 }, value_type{Key('H'), 7 }, value_type{Key('G'), 6 }, value_type{Key('F'), 5 },
                                  value_type{Key('E'), 4 }, value_type{Key('D'), 3 }, value_type{Key('C'), 2 }, value_type{Key('B'), 1 }, value_type{Key('A'), 0 } };
#else
      static constexpr Data data{ value_type{Key('A'), 0 }, value_type{Key('B'), 1 }, value_type{Key('C'), 2 }, value_type{Key('D'), 3 }, value_type{Key('E'), 4 },
                                  value_type{Key('F'), 5 }, value_type{Key('G'), 6 }, value_type{Key('H'), 7 }, value_type{Key('I'), 8 }, value_type{Key('J'), 9 } };
#endif

      CHECK_EQUAL(1, data.count(Key('A')));
      CHECK_EQUAL(1, data.count(Key('B')));
      CHECK_EQUAL(1, data.count(Key('C')));
      CHECK_EQUAL(1, data.count(Key('D')));
      CHECK_EQUAL(1, data.count(Key('E')));
      CHECK_EQUAL(1, data.count(Key('F')));
      CHECK_EQUAL(1, data.count(Key('G')));
      CHECK_EQUAL(1, data.count(Key('H')));
      CHECK_EQUAL(1, data.count(Key('I')));
      CHECK_EQUAL(1, data.count(Key('J')));
      CHECK_EQUAL(0, data.count(Key('K')));
    }

    //*************************************************************************
    TEST(test_count_constexpr)
    {
#ifdef TEST_GREATER_THAN
      static constexpr Data data{ value_type{Key('J'), 9 }, value_type{Key('I'), 8 }, value_type{Key('H'), 7 }, value_type{Key('G'), 6 }, value_type{Key('F'), 5 },
                                  value_type{Key('E'), 4 }, value_type{Key('D'), 3 }, value_type{Key('C'), 2 }, value_type{Key('B'), 1 }, value_type{Key('A'), 0 } };
#else
      static constexpr Data data{ value_type{Key('A'), 0 }, value_type{Key('B'), 1 }, value_type{Key('C'), 2 }, value_type{Key('D'), 3 }, value_type{Key('E'), 4 },
                                  value_type{Key('F'), 5 }, value_type{Key('G'), 6 }, value_type{Key('H'), 7 }, value_type{Key('I'), 8 }, value_type{Key('J'), 9 } };
#endif

      static constexpr size_t countA = data.count(Key('A'));
      static constexpr size_t countB = data.count(Key('B'));
      static constexpr size_t countC = data.count(Key('C'));
      static constexpr size_t countD = data.count(Key('D'));
      static constexpr size_t countE = data.count(Key('E'));
      static constexpr size_t countF = data.count(Key('F'));
      static constexpr size_t countG = data.count(Key('G'));
      static constexpr size_t countH = data.count(Key('H'));
      static constexpr size_t countI = data.count(Key('I'));
      static constexpr size_t countJ = data.count(Key('J'));
      static constexpr size_t countK = data.count(Key('K'));

      CHECK_EQUAL(1, countA);
      CHECK_EQUAL(1, countB);
      CHECK_EQUAL(1, countC);
      CHECK_EQUAL(1, countD);
      CHECK_EQUAL(1, countE);
      CHECK_EQUAL(1, countF);
      CHECK_EQUAL(1, countG);
      CHECK_EQUAL(1, countH);
      CHECK_EQUAL(1, countI);
      CHECK_EQUAL(1, countJ);
      CHECK_EQUAL(0, countK);
    }

    //*************************************************************************
    TEST(test_count_using_transparent_comparator)
    {
#ifdef TEST_GREATER_THAN
      static constexpr DataTransparentComparator data{ value_type{Key('J'), 9 }, value_type{Key('I'), 8 }, value_type{Key('H'), 7 }, value_type{Key('G'), 6 }, value_type{Key('F'), 5 },
                                                       value_type{Key('E'), 4 }, value_type{Key('D'), 3 }, value_type{Key('C'), 2 }, value_type{Key('B'), 1 }, value_type{Key('A'), 0 } };
#else
      static constexpr DataTransparentComparator data{ value_type{Key('A'), 0 }, value_type{Key('B'), 1 }, value_type{Key('C'), 2 }, value_type{Key('D'), 3 }, value_type{Key('E'), 4 },
                                                       value_type{Key('F'), 5 }, value_type{Key('G'), 6 }, value_type{Key('H'), 7 }, value_type{Key('I'), 8 }, value_type{Key('J'), 9 } };
#endif

      CHECK_EQUAL(1, data.count('A'));
      CHECK_EQUAL(1, data.count('B'));
      CHECK_EQUAL(1, data.count('C'));
      CHECK_EQUAL(1, data.count('D'));
      CHECK_EQUAL(1, data.count('E'));
      CHECK_EQUAL(1, data.count('F'));
      CHECK_EQUAL(1, data.count('G'));
      CHECK_EQUAL(1, data.count('H'));
      CHECK_EQUAL(1, data.count('I'));
      CHECK_EQUAL(1, data.count('J'));
      CHECK_EQUAL(0, data.count('K'));
    }

    //*************************************************************************
    TEST(test_count_constexpr_using_transparent_comparator)
    {
#ifdef TEST_GREATER_THAN
      static constexpr DataTransparentComparator data{ value_type{Key('J'), 9 }, value_type{Key('I'), 8 }, value_type{Key('H'), 7 }, value_type{Key('G'), 6 }, value_type{Key('F'), 5 },
                                                       value_type{Key('E'), 4 }, value_type{Key('D'), 3 }, value_type{Key('C'), 2 }, value_type{Key('B'), 1 }, value_type{Key('A'), 0 } };
#else
      static constexpr DataTransparentComparator data{ value_type{Key('A'), 0 }, value_type{Key('B'), 1 }, value_type{Key('C'), 2 }, value_type{Key('D'), 3 }, value_type{Key('E'), 4 },
                                                       value_type{Key('F'), 5 }, value_type{Key('G'), 6 }, value_type{Key('H'), 7 }, value_type{Key('I'), 8 }, value_type{Key('J'), 9 } };
#endif

      static constexpr size_t countA = data.count('A');
      static constexpr size_t countB = data.count('B');
      static constexpr size_t countC = data.count('C');
      static constexpr size_t countD = data.count('D');
      static constexpr size_t countE = data.count('E');
      static constexpr size_t countF = data.count('F');
      static constexpr size_t countG = data.count('G');
      static constexpr size_t countH = data.count('H');
      static constexpr size_t countI = data.count('I');
      static constexpr size_t countJ = data.count('J');
      static constexpr size_t countK = data.count('K');

      CHECK_EQUAL(1, countA);
      CHECK_EQUAL(1, countB);
      CHECK_EQUAL(1, countC);
      CHECK_EQUAL(1, countD);
      CHECK_EQUAL(1, countE);
      CHECK_EQUAL(1, countF);
      CHECK_EQUAL(1, countG);
      CHECK_EQUAL(1, countH);
      CHECK_EQUAL(1, countI);
      CHECK_EQUAL(1, countJ);
      CHECK_EQUAL(0, countK);
    }

    //*************************************************************************
    TEST(test_iterator)
    {
#ifdef TEST_GREATER_THAN
      static constexpr Data data{ value_type{Key('J'), 9 }, value_type{Key('I'), 8 }, value_type{Key('H'), 7 }, value_type{Key('G'), 6 }, value_type{Key('F'), 5 },
                                  value_type{Key('E'), 4 }, value_type{Key('D'), 3 }, value_type{Key('C'), 2 }, value_type{Key('B'), 1 }, value_type{Key('A'), 0 } };
#else
      static constexpr Data data{ value_type{Key('A'), 0 }, value_type{Key('B'), 1 }, value_type{Key('C'), 2 }, value_type{Key('D'), 3 }, value_type{Key('E'), 4 },
                                  value_type{Key('F'), 5 }, value_type{Key('G'), 6 }, value_type{Key('H'), 7 }, value_type{Key('I'), 8 }, value_type{Key('J'), 9 } };
#endif

      Data::iterator itr = data.begin();

#ifdef TEST_GREATER_THAN
      CHECK_TRUE((value_type{Key('J'), 9 }) == *itr++);
      CHECK_TRUE((value_type{Key('I'), 8 }) == *itr++);
      CHECK_TRUE((value_type{Key('H'), 7 }) == *itr++);
      CHECK_TRUE((value_type{Key('G'), 6 }) == *itr++);
      CHECK_TRUE((value_type{Key('F'), 5 }) == *itr++);
      CHECK_TRUE((value_type{Key('E'), 4 }) == *itr++);
      CHECK_TRUE((value_type{Key('D'), 3 }) == *itr++);
      CHECK_TRUE((value_type{Key('C'), 2 }) == *itr++);
      CHECK_TRUE((value_type{Key('B'), 1 }) == *itr++);
      CHECK_TRUE((value_type{Key('A'), 0 }) == *itr++);
      CHECK_TRUE(itr == data.end());
#else
      CHECK_TRUE((value_type{Key('A'), 0 }) == *itr++);
      CHECK_TRUE((value_type{Key('B'), 1 }) == *itr++);
      CHECK_TRUE((value_type{Key('C'), 2 }) == *itr++);
      CHECK_TRUE((value_type{Key('D'), 3 }) == *itr++);
      CHECK_TRUE((value_type{Key('E'), 4 }) == *itr++);
      CHECK_TRUE((value_type{Key('F'), 5 }) == *itr++);
      CHECK_TRUE((value_type{Key('G'), 6 }) == *itr++);
      CHECK_TRUE((value_type{Key('H'), 7 }) == *itr++);
      CHECK_TRUE((value_type{Key('I'), 8 }) == *itr++);
      CHECK_TRUE((value_type{Key('J'), 9 }) == *itr++);
      CHECK_TRUE(itr == data.end());
#endif      
    }

    //*************************************************************************
    TEST(test_const_iterator)
    {
#ifdef TEST_GREATER_THAN
      static constexpr Data data{ value_type{Key('J'), 9 }, value_type{Key('I'), 8 }, value_type{Key('H'), 7 }, value_type{Key('G'), 6 }, value_type{Key('F'), 5 },
                                  value_type{Key('E'), 4 }, value_type{Key('D'), 3 }, value_type{Key('C'), 2 }, value_type{Key('B'), 1 }, value_type{Key('A'), 0 } };
#else
      static constexpr Data data{ value_type{Key('A'), 0 }, value_type{Key('B'), 1 }, value_type{Key('C'), 2 }, value_type{Key('D'), 3 }, value_type{Key('E'), 4 },
                                  value_type{Key('F'), 5 }, value_type{Key('G'), 6 }, value_type{Key('H'), 7 }, value_type{Key('I'), 8 }, value_type{Key('J'), 9 } };
#endif

      Data::const_iterator itr = data.begin();

#ifdef TEST_GREATER_THAN
      CHECK_TRUE((value_type{Key('J'), 9 }) == *itr++);
      CHECK_TRUE((value_type{Key('I'), 8 }) == *itr++);
      CHECK_TRUE((value_type{Key('H'), 7 }) == *itr++);
      CHECK_TRUE((value_type{Key('G'), 6 }) == *itr++);
      CHECK_TRUE((value_type{Key('F'), 5 }) == *itr++);
      CHECK_TRUE((value_type{Key('E'), 4 }) == *itr++);
      CHECK_TRUE((value_type{Key('D'), 3 }) == *itr++);
      CHECK_TRUE((value_type{Key('C'), 2 }) == *itr++);
      CHECK_TRUE((value_type{Key('B'), 1 }) == *itr++);
      CHECK_TRUE((value_type{Key('A'), 0 }) == *itr++);
      CHECK_TRUE(itr == data.end());
#else
      CHECK_TRUE((value_type{Key('A'), 0 }) == *itr++);
      CHECK_TRUE((value_type{Key('B'), 1 }) == *itr++);
      CHECK_TRUE((value_type{Key('C'), 2 }) == *itr++);
      CHECK_TRUE((value_type{Key('D'), 3 }) == *itr++);
      CHECK_TRUE((value_type{Key('E'), 4 }) == *itr++);
      CHECK_TRUE((value_type{Key('F'), 5 }) == *itr++);
      CHECK_TRUE((value_type{Key('G'), 6 }) == *itr++);
      CHECK_TRUE((value_type{Key('H'), 7 }) == *itr++);
      CHECK_TRUE((value_type{Key('I'), 8 }) == *itr++);
      CHECK_TRUE((value_type{Key('J'), 9 }) == *itr++);
      CHECK_TRUE(itr == data.end());
#endif      
    }

    //*************************************************************************
    TEST(test_find)
    {
#ifdef TEST_GREATER_THAN
      static constexpr Data data{ value_type{Key('J'), 9 }, value_type{Key('I'), 8 }, value_type{Key('H'), 7 }, value_type{Key('G'), 6 }, value_type{Key('F'), 5 },
                                  value_type{Key('E'), 4 }, value_type{Key('D'), 3 }, value_type{Key('C'), 2 }, value_type{Key('B'), 1 }, value_type{Key('A'), 0 } };
#else
      static constexpr Data data{ value_type{Key('A'), 0 }, value_type{Key('B'), 1 }, value_type{Key('C'), 2 }, value_type{Key('D'), 3 }, value_type{Key('E'), 4 },
                                  value_type{Key('F'), 5 }, value_type{Key('G'), 6 }, value_type{Key('H'), 7 }, value_type{Key('I'), 8 }, value_type{Key('J'), 9 } };
#endif

      Data::const_iterator result0 = data.find(Key('A'));
      Data::const_iterator result1 = data.find(Key('B'));
      Data::const_iterator result2 = data.find(Key('C'));
      Data::const_iterator result3 = data.find(Key('D'));
      Data::const_iterator result4 = data.find(Key('E'));
      Data::const_iterator result5 = data.find(Key('F'));
      Data::const_iterator result6 = data.find(Key('G'));
      Data::const_iterator result7 = data.find(Key('H'));
      Data::const_iterator result8 = data.find(Key('I'));
      Data::const_iterator result9 = data.find(Key('J'));

#ifdef TEST_GREATER_THAN
      CHECK_EQUAL(9, (std::distance(data.begin(), result0)));
      CHECK_EQUAL(8, (std::distance(data.begin(), result1)));
      CHECK_EQUAL(7, (std::distance(data.begin(), result2)));
      CHECK_EQUAL(6, (std::distance(data.begin(), result3)));
      CHECK_EQUAL(5, (std::distance(data.begin(), result4)));
      CHECK_EQUAL(4, (std::distance(data.begin(), result5)));
      CHECK_EQUAL(3, (std::distance(data.begin(), result6)));
      CHECK_EQUAL(2, (std::distance(data.begin(), result7)));
      CHECK_EQUAL(1, (std::distance(data.begin(), result8)));
      CHECK_EQUAL(0, (std::distance(data.begin(), result9)));
#else
      CHECK_EQUAL(0, (std::distance(data.begin(), result0)));
      CHECK_EQUAL(1, (std::distance(data.begin(), result1)));
      CHECK_EQUAL(2, (std::distance(data.begin(), result2)));
      CHECK_EQUAL(3, (std::distance(data.begin(), result3)));
      CHECK_EQUAL(4, (std::distance(data.begin(), result4)));
      CHECK_EQUAL(5, (std::distance(data.begin(), result5)));
      CHECK_EQUAL(6, (std::distance(data.begin(), result6)));
      CHECK_EQUAL(7, (std::distance(data.begin(), result7)));
      CHECK_EQUAL(8, (std::distance(data.begin(), result8)));
      CHECK_EQUAL(9, (std::distance(data.begin(), result9)));
#endif
    }

    //*************************************************************************
    TEST(test_find_constexpr)
    {
#ifdef TEST_GREATER_THAN
      static constexpr Data data{ value_type{Key('J'), 9 }, value_type{Key('I'), 8 }, value_type{Key('H'), 7 }, value_type{Key('G'), 6 }, value_type{Key('F'), 5 },
                                  value_type{Key('E'), 4 }, value_type{Key('D'), 3 }, value_type{Key('C'), 2 }, value_type{Key('B'), 1 }, value_type{Key('A'), 0 } };
#else
      static constexpr Data data{ value_type{Key('A'), 0 }, value_type{Key('B'), 1 }, value_type{Key('C'), 2 }, value_type{Key('D'), 3 }, value_type{Key('E'), 4 },
                                  value_type{Key('F'), 5 }, value_type{Key('G'), 6 }, value_type{Key('H'), 7 }, value_type{Key('I'), 8 }, value_type{Key('J'), 9 } };
#endif

      static constexpr Data::const_iterator result0 = data.find(Key('A'));
      static constexpr Data::const_iterator result1 = data.find(Key('B'));
      static constexpr Data::const_iterator result2 = data.find(Key('C'));
      static constexpr Data::const_iterator result3 = data.find(Key('D'));
      static constexpr Data::const_iterator result4 = data.find(Key('E'));
      static constexpr Data::const_iterator result5 = data.find(Key('F'));
      static constexpr Data::const_iterator result6 = data.find(Key('G'));
      static constexpr Data::const_iterator result7 = data.find(Key('H'));
      static constexpr Data::const_iterator result8 = data.find(Key('I'));
      static constexpr Data::const_iterator result9 = data.find(Key('J'));

#ifdef TEST_GREATER_THAN
      CHECK_EQUAL(9, (std::distance(data.begin(), result0)));
      CHECK_EQUAL(8, (std::distance(data.begin(), result1)));
      CHECK_EQUAL(7, (std::distance(data.begin(), result2)));
      CHECK_EQUAL(6, (std::distance(data.begin(), result3)));
      CHECK_EQUAL(5, (std::distance(data.begin(), result4)));
      CHECK_EQUAL(4, (std::distance(data.begin(), result5)));
      CHECK_EQUAL(3, (std::distance(data.begin(), result6)));
      CHECK_EQUAL(2, (std::distance(data.begin(), result7)));
      CHECK_EQUAL(1, (std::distance(data.begin(), result8)));
      CHECK_EQUAL(0, (std::distance(data.begin(), result9)));
#else
      CHECK_EQUAL(0, (std::distance(data.begin(), result0)));
      CHECK_EQUAL(1, (std::distance(data.begin(), result1)));
      CHECK_EQUAL(2, (std::distance(data.begin(), result2)));
      CHECK_EQUAL(3, (std::distance(data.begin(), result3)));
      CHECK_EQUAL(4, (std::distance(data.begin(), result4)));
      CHECK_EQUAL(5, (std::distance(data.begin(), result5)));
      CHECK_EQUAL(6, (std::distance(data.begin(), result6)));
      CHECK_EQUAL(7, (std::distance(data.begin(), result7)));
      CHECK_EQUAL(8, (std::distance(data.begin(), result8)));
      CHECK_EQUAL(9, (std::distance(data.begin(), result9)));
#endif
    }

    //*************************************************************************
    TEST(test_find_using_transparent_comparator)
    {
#ifdef TEST_GREATER_THAN
      static constexpr DataTransparentComparator data{ value_type{Key('J'), 9 }, value_type{Key('I'), 8 }, value_type{Key('H'), 7 }, value_type{Key('G'), 6 }, value_type{Key('F'), 5 },
                                                       value_type{Key('E'), 4 }, value_type{Key('D'), 3 }, value_type{Key('C'), 2 }, value_type{Key('B'), 1 }, value_type{Key('A'), 0 } };
#else
      static constexpr DataTransparentComparator data{ value_type{Key('A'), 0 }, value_type{Key('B'), 1 }, value_type{Key('C'), 2 }, value_type{Key('D'), 3 }, value_type{Key('E'), 4 },
                                                       value_type{Key('F'), 5 }, value_type{Key('G'), 6 }, value_type{Key('H'), 7 }, value_type{Key('I'), 8 }, value_type{Key('J'), 9 } };
#endif

      Data::const_iterator result0 = data.find('A');
      Data::const_iterator result1 = data.find('B');
      Data::const_iterator result2 = data.find('C');
      Data::const_iterator result3 = data.find('D');
      Data::const_iterator result4 = data.find('E');
      Data::const_iterator result5 = data.find('F');
      Data::const_iterator result6 = data.find('G');
      Data::const_iterator result7 = data.find('H');
      Data::const_iterator result8 = data.find('I');
      Data::const_iterator result9 = data.find('J');

#ifdef TEST_GREATER_THAN
      CHECK_EQUAL(9, (std::distance(data.begin(), result0)));
      CHECK_EQUAL(8, (std::distance(data.begin(), result1)));
      CHECK_EQUAL(7, (std::distance(data.begin(), result2)));
      CHECK_EQUAL(6, (std::distance(data.begin(), result3)));
      CHECK_EQUAL(5, (std::distance(data.begin(), result4)));
      CHECK_EQUAL(4, (std::distance(data.begin(), result5)));
      CHECK_EQUAL(3, (std::distance(data.begin(), result6)));
      CHECK_EQUAL(2, (std::distance(data.begin(), result7)));
      CHECK_EQUAL(1, (std::distance(data.begin(), result8)));
      CHECK_EQUAL(0, (std::distance(data.begin(), result9)));
#else
      CHECK_EQUAL(0, (std::distance(data.begin(), result0))); 
      CHECK_EQUAL(1, (std::distance(data.begin(), result1)));
      CHECK_EQUAL(2, (std::distance(data.begin(), result2)));
      CHECK_EQUAL(3, (std::distance(data.begin(), result3)));
      CHECK_EQUAL(4, (std::distance(data.begin(), result4)));
      CHECK_EQUAL(5, (std::distance(data.begin(), result5)));
      CHECK_EQUAL(6, (std::distance(data.begin(), result6)));
      CHECK_EQUAL(7, (std::distance(data.begin(), result7)));
      CHECK_EQUAL(8, (std::distance(data.begin(), result8)));
      CHECK_EQUAL(9, (std::distance(data.begin(), result9)));
#endif
    }

    //*************************************************************************
    TEST(test_find_constexpr_using_transparent_comparator)
    {
#ifdef TEST_GREATER_THAN
      static constexpr DataTransparentComparator data{ value_type{Key('J'), 9 }, value_type{Key('I'), 8 }, value_type{Key('H'), 7 }, value_type{Key('G'), 6 }, value_type{Key('F'), 5 },
                                                       value_type{Key('E'), 4 }, value_type{Key('D'), 3 }, value_type{Key('C'), 2 }, value_type{Key('B'), 1 }, value_type{Key('A'), 0 } };
#else
      static constexpr DataTransparentComparator data{ value_type{Key('A'), 0 }, value_type{Key('B'), 1 }, value_type{Key('C'), 2 }, value_type{Key('D'), 3 }, value_type{Key('E'), 4 },
                                                       value_type{Key('F'), 5 }, value_type{Key('G'), 6 }, value_type{Key('H'), 7 }, value_type{Key('I'), 8 }, value_type{Key('J'), 9 } };
#endif

      static constexpr Data::const_iterator result0 = data.find('A');
      static constexpr Data::const_iterator result1 = data.find('B');
      static constexpr Data::const_iterator result2 = data.find('C');
      static constexpr Data::const_iterator result3 = data.find('D');
      static constexpr Data::const_iterator result4 = data.find('E');
      static constexpr Data::const_iterator result5 = data.find('F');
      static constexpr Data::const_iterator result6 = data.find('G');
      static constexpr Data::const_iterator result7 = data.find('H');
      static constexpr Data::const_iterator result8 = data.find('I');
      static constexpr Data::const_iterator result9 = data.find('J');

#ifdef TEST_GREATER_THAN
      CHECK_EQUAL(9, (std::distance(data.begin(), result0)));
      CHECK_EQUAL(8, (std::distance(data.begin(), result1)));
      CHECK_EQUAL(7, (std::distance(data.begin(), result2)));
      CHECK_EQUAL(6, (std::distance(data.begin(), result3)));
      CHECK_EQUAL(5, (std::distance(data.begin(), result4)));
      CHECK_EQUAL(4, (std::distance(data.begin(), result5)));
      CHECK_EQUAL(3, (std::distance(data.begin(), result6)));
      CHECK_EQUAL(2, (std::distance(data.begin(), result7)));
      CHECK_EQUAL(1, (std::distance(data.begin(), result8)));
      CHECK_EQUAL(0, (std::distance(data.begin(), result9)));
#else
      CHECK_EQUAL(0, (std::distance(data.begin(), result0))); 
      CHECK_EQUAL(1, (std::distance(data.begin(), result1)));
      CHECK_EQUAL(2, (std::distance(data.begin(), result2)));
      CHECK_EQUAL(3, (std::distance(data.begin(), result3)));
      CHECK_EQUAL(4, (std::distance(data.begin(), result4)));
      CHECK_EQUAL(5, (std::distance(data.begin(), result5)));
      CHECK_EQUAL(6, (std::distance(data.begin(), result6)));
      CHECK_EQUAL(7, (std::distance(data.begin(), result7)));
      CHECK_EQUAL(8, (std::distance(data.begin(), result8)));
      CHECK_EQUAL(9, (std::distance(data.begin(), result9)));
#endif
    }

    //*************************************************************************
    TEST(test_equal)
    {
      static constexpr DataTransparentComparator data1{ value_type{Key('A'), 0 }, value_type{Key('B'), 1 }, value_type{Key('C'), 2 }, value_type{Key('D'), 3 }, value_type{Key('E'), 4 },
                                                        value_type{Key('F'), 5 }, value_type{Key('G'), 6 }, value_type{Key('H'), 7 }, value_type{Key('I'), 8 }, value_type{Key('J'), 9 } };

      static constexpr DataTransparentComparator data2{ value_type{Key('A'), 0 }, value_type{Key('B'), 1 }, value_type{Key('C'), 2 }, value_type{Key('D'), 3 }, value_type{Key('E'), 4 },
                                                        value_type{Key('F'), 5 }, value_type{Key('G'), 6 }, value_type{Key('H'), 7 }, value_type{Key('I'), 8 }, value_type{Key('J'), 9 } };

      static constexpr DataTransparentComparator data3{ value_type{Key('A'), 0 }, value_type{Key('B'), 1 }, value_type{Key('C'), 2 }, value_type{Key('D'), 3 }, value_type{Key('E'), 4 },
                                                        value_type{Key('F'), 5 }, value_type{Key('G'), 6 }, value_type{Key('H'), 6 }, value_type{Key('I'), 8 }, value_type{Key('J'), 9 } };


      CHECK_TRUE(data1 == data2);
      CHECK_FALSE(data1 == data3);
    }

    //*************************************************************************
    TEST(test_not_equal)
    {
      static constexpr DataTransparentComparator data1{ value_type{Key('A'), 0 }, value_type{Key('B'), 1 }, value_type{Key('C'), 2 }, value_type{Key('D'), 3 }, value_type{Key('E'), 4 },
                                                        value_type{Key('F'), 5 }, value_type{Key('G'), 6 }, value_type{Key('H'), 7 }, value_type{Key('I'), 8 }, value_type{Key('J'), 9 } };

      static constexpr DataTransparentComparator data2{ value_type{Key('A'), 0 }, value_type{Key('B'), 1 }, value_type{Key('C'), 2 }, value_type{Key('D'), 3 }, value_type{Key('E'), 4 },
                                                        value_type{Key('F'), 5 }, value_type{Key('G'), 6 }, value_type{Key('H'), 7 }, value_type{Key('I'), 8 }, value_type{Key('J'), 9 } };

      static constexpr DataTransparentComparator data3{ value_type{Key('A'), 0 }, value_type{Key('B'), 1 }, value_type{Key('C'), 2 }, value_type{Key('D'), 3 }, value_type{Key('E'), 4 },
                                                        value_type{Key('F'), 5 }, value_type{Key('G'), 6 }, value_type{Key('H'), 6 }, value_type{Key('I'), 8 }, value_type{Key('J'), 9 } };


      CHECK_FALSE(data1 != data2);
      CHECK_TRUE(data1 != data3);
    }

//    //*************************************************************************
//    TEST(test_key_compare)
//    {
//      const Data data(initial_data.begin(), initial_data.end());
//
//      Data::key_compare compare = data.key_comp();
//
//      Data::key_type a(Key('A');
//      Data::key_type b(Key('B');
//
//#ifdef TEST_GREATER_THAN
//      CHECK(!compare(a, b));
//      CHECK(compare(b, a));
//#else
//      CHECK(compare(a, b));
//      CHECK(!compare(b, a));
//#endif
//    }

//    //*************************************************************************
//    TEST(test_key_compare_using_transparent_comparator)
//    {
//      using EMap = etl::map<char, int, Max_Size, etl::less<>>;
//      const EMap data(initial_data.begin(), initial_data.end());
//
//      EMap::key_compare compare = data.key_comp();
//
//      char a(Key('A');
//      Key b(Key('B');
//
//      CHECK(compare(a, b));
//      CHECK(!compare(b, a));
//   }

//    //*************************************************************************
//    TEST(test_value_compare)
//    {
//      const Data data(initial_data.begin(), initial_data.end());
//
//      Data::value_compare compare = data.value_comp();
//
//      Data::value_type a(char(Key('A'), 0);
//      Data::value_type b(char(Key('B'), 1);
//
//#ifdef TEST_GREATER_THAN
//      CHECK(!compare(a, b));
//      CHECK(compare(b, a));
//#else
//      CHECK(compare(a, b));
//      CHECK(!compare(b, a));
//#endif
//    }

//    //*************************************************************************
//    TEST(test_compare_lower_upper_bound)
//    {
//      Data data(initial_data_even.begin(), initial_data_even.end());
//      Compare_Data compare(initial_data_even.begin(), initial_data_even.end());
//
//      std::vector<ETL_OR_STD::pair<char, int> > tab(test_data.begin(), test_data.end());
//
//      //make sure both data and compare contain same elements
//      std::vector<ETL_OR_STD::pair<char, int> > data_elements(data.begin(), data.end());
//      std::vector<ETL_OR_STD::pair<char, int> > compare_data_elements(compare.begin(), compare.end());
//
//      CHECK(data_elements == compare_data_elements);
//      CHECK(data_elements.size() == Max_Size);
//
//      for(std::vector<ETL_OR_STD::pair<char, int> >::iterator it = tab.begin() ; it != tab.end() ; ++it)
//      {
//        char i = it->first;
//
//        //lower_bound
//        CHECK((compare.lower_bound(i) == compare.end()) == (data.lower_bound(i) == data.end()));
//        //if both end, or none
//        if((compare.lower_bound(i) == compare.end()) == (data.lower_bound(i) == data.end()))
//        {
//          //if both are not end
//          if(compare.lower_bound(i) != compare.end())
//          {
//            CHECK((*compare.lower_bound(i)) == (*data.lower_bound(i)));
//          }
//
//          ETL_OR_STD::pair<Compare_Data::const_iterator, Compare_Data::const_iterator> stlret = compare.equal_range(i);
//          ETL_OR_STD::pair<Data::const_iterator, Data::const_iterator> etlret = data.equal_range(i);
//
//          CHECK((stlret.first == compare.end()) == (etlret.first == data.end()));
//          if((stlret.first != compare.end()) && (etlret.first != data.end()))
//          {
//            CHECK((*stlret.first) == (*etlret.first));
//          }
//          CHECK((stlret.second == compare.end()) == (etlret.second == data.end()));
//          if((stlret.second != compare.end()) && (etlret.second != data.end()))
//          {
//            CHECK((*stlret.second) == (*etlret.second));
//          }
//        }
//
//        //upper_bound
//        CHECK((compare.upper_bound(i) == compare.end()) == (data.upper_bound(i) == data.end()));
//        //if both end, or none
//        if((compare.upper_bound(i) == compare.end()) == (data.upper_bound(i) == data.end()))
//        {
//          //if both are not end
//          if(compare.upper_bound(i) != compare.end())
//          {
//            CHECK((*compare.upper_bound(i)) == (*data.upper_bound(i)));
//          }
//        }
//      }
//    }

//    //*************************************************************************
//#if ETL_USING_CPP17 && ETL_HAS_INITIALIZER_LIST && !defined(ETL_TEMPLATE_DEDUCTION_GUIDE_TESTS_DISABLED)
//    TEST(test_map_template_deduction)
//    {
//      using Pair = std::pair<const etl::string<1>, int>;
//
//      etl::map data { Pair{'0', 0}, Pair{'1', 1}, Pair{'2', 2}, Pair{'3', 3}, Pair{'4', 4}, Pair{'5', 5} };
//
//      auto v = *data.begin();
//      using Type = decltype(v);
//      CHECK((std::is_same_v<Pair, Type>));
//
//      CHECK_EQUAL(0, data.at('0'));
//      CHECK_EQUAL(1, data.at('1'));
//      CHECK_EQUAL(2, data.at('2'));
//      CHECK_EQUAL(3, data.at('3'));
//      CHECK_EQUAL(4, data.at('4'));
//      CHECK_EQUAL(5, data.at('5'));
//    }
//#endif

    //*************************************************************************
    TEST(test_contains)
    {
#ifdef TEST_GREATER_THAN
      static constexpr Data data{ value_type{Key('J'), 9 }, value_type{Key('I'), 8 }, value_type{Key('H'), 7 }, value_type{Key('G'), 6 }, value_type{Key('F'), 5 },
                                  value_type{Key('E'), 4 }, value_type{Key('D'), 3 }, value_type{Key('C'), 2 }, value_type{Key('B'), 1 }, value_type{Key('A'), 0 } };
#else
      static constexpr Data data{ value_type{Key('A'), 0 }, value_type{Key('B'), 1 }, value_type{Key('C'), 2 }, value_type{Key('D'), 3 }, value_type{Key('E'), 4 },
                                  value_type{Key('F'), 5 }, value_type{Key('G'), 6 }, value_type{Key('H'), 7 }, value_type{Key('I'), 8 }, value_type{Key('J'), 9 } };
#endif

      CHECK_TRUE(data.contains(Key('A')));
      CHECK_TRUE(data.contains(Key('B')));
      CHECK_TRUE(data.contains(Key('C')));
      CHECK_TRUE(data.contains(Key('D')));
      CHECK_TRUE(data.contains(Key('E')));
      CHECK_TRUE(data.contains(Key('F')));
      CHECK_TRUE(data.contains(Key('G')));
      CHECK_TRUE(data.contains(Key('H')));
      CHECK_TRUE(data.contains(Key('I')));
      CHECK_TRUE(data.contains(Key('J')));
      CHECK_FALSE(data.contains(Key('K')));
    }

    //*************************************************************************
    TEST(test_contains_constexpr)
    {
#ifdef TEST_GREATER_THAN
      static constexpr Data data{ value_type{Key('J'), 9 }, value_type{Key('I'), 8 }, value_type{Key('H'), 7 }, value_type{Key('G'), 6 }, value_type{Key('F'), 5 },
        value_type{Key('E'), 4 }, value_type{Key('D'), 3 }, value_type{Key('C'), 2 }, value_type{Key('B'), 1 }, value_type{Key('A'), 0 } };
#else
      static constexpr Data data{ value_type{Key('A'), 0 }, value_type{Key('B'), 1 }, value_type{Key('C'), 2 }, value_type{Key('D'), 3 }, value_type{Key('E'), 4 },
        value_type{Key('F'), 5 }, value_type{Key('G'), 6 }, value_type{Key('H'), 7 }, value_type{Key('I'), 8 }, value_type{Key('J'), 9 } };
#endif

      static constexpr bool containsA = data.contains(Key('A'));
      static constexpr bool containsB = data.contains(Key('B'));
      static constexpr bool containsC = data.contains(Key('C'));
      static constexpr bool containsD = data.contains(Key('D'));
      static constexpr bool containsE = data.contains(Key('E'));
      static constexpr bool containsF = data.contains(Key('F'));
      static constexpr bool containsG = data.contains(Key('G'));
      static constexpr bool containsH = data.contains(Key('H'));
      static constexpr bool containsI = data.contains(Key('I'));
      static constexpr bool containsJ = data.contains(Key('J'));
      static constexpr bool containsK = data.contains(Key('K'));

      CHECK_TRUE(containsA);
      CHECK_TRUE(containsB);
      CHECK_TRUE(containsC);
      CHECK_TRUE(containsD);
      CHECK_TRUE(containsE);
      CHECK_TRUE(containsF);
      CHECK_TRUE(containsG);
      CHECK_TRUE(containsH);
      CHECK_TRUE(containsI);
      CHECK_TRUE(containsJ);
      CHECK_FALSE(containsK);
    }

    //*************************************************************************
    TEST(test_contains_with_transparent_comparator)
    {
#ifdef TEST_GREATER_THAN
      static constexpr DataTransparentComparator data{ value_type{Key('J'), 9 }, value_type{Key('I'), 8 }, value_type{Key('H'), 7 }, value_type{Key('G'), 6 }, value_type{Key('F'), 5 },
                                                       value_type{Key('E'), 4 }, value_type{Key('D'), 3 }, value_type{Key('C'), 2 }, value_type{Key('B'), 1 }, value_type{Key('A'), 0 } };
#else
      static constexpr DataTransparentComparator data{ value_type{Key('A'), 0 }, value_type{Key('B'), 1 }, value_type{Key('C'), 2 }, value_type{Key('D'), 3 }, value_type{Key('E'), 4 },
                                                       value_type{Key('F'), 5 }, value_type{Key('G'), 6 }, value_type{Key('H'), 7 }, value_type{Key('I'), 8 }, value_type{Key('J'), 9 } };
#endif

      CHECK_TRUE(data.contains('A'));
      CHECK_TRUE(data.contains('B'));
      CHECK_TRUE(data.contains('C'));
      CHECK_TRUE(data.contains('D'));
      CHECK_TRUE(data.contains('E'));
      CHECK_TRUE(data.contains('F'));
      CHECK_TRUE(data.contains('G'));
      CHECK_TRUE(data.contains('H'));
      CHECK_TRUE(data.contains('I'));
      CHECK_TRUE(data.contains('J'));
      CHECK_FALSE(data.contains('K'));
    }

    //*************************************************************************
    TEST(test_contains_with_transparent_comparator_constexpr)
    {
#ifdef TEST_GREATER_THAN
      static constexpr DataTransparentComparator data{ value_type{Key('J'), 9 }, value_type{Key('I'), 8 }, value_type{Key('H'), 7 }, value_type{Key('G'), 6 }, value_type{Key('F'), 5 },
                                                       value_type{Key('E'), 4 }, value_type{Key('D'), 3 }, value_type{Key('C'), 2 }, value_type{Key('B'), 1 }, value_type{Key('A'), 0 } };
#else
      static constexpr DataTransparentComparator data{ value_type{Key('A'), 0 }, value_type{Key('B'), 1 }, value_type{Key('C'), 2 }, value_type{Key('D'), 3 }, value_type{Key('E'), 4 },
                                                       value_type{Key('F'), 5 }, value_type{Key('G'), 6 }, value_type{Key('H'), 7 }, value_type{Key('I'), 8 }, value_type{Key('J'), 9 } };
#endif

      static constexpr bool containsA = data.contains('A');
      static constexpr bool containsB = data.contains('B');
      static constexpr bool containsC = data.contains('C');
      static constexpr bool containsD = data.contains('D');
      static constexpr bool containsE = data.contains('E');
      static constexpr bool containsF = data.contains('F');
      static constexpr bool containsG = data.contains('G');
      static constexpr bool containsH = data.contains('H');
      static constexpr bool containsI = data.contains('I');
      static constexpr bool containsJ = data.contains('J');
      static constexpr bool containsK = data.contains('K');

      CHECK_TRUE(containsA);
      CHECK_TRUE(containsB);
      CHECK_TRUE(containsC);
      CHECK_TRUE(containsD);
      CHECK_TRUE(containsE);
      CHECK_TRUE(containsF);
      CHECK_TRUE(containsG);
      CHECK_TRUE(containsH);
      CHECK_TRUE(containsI);
      CHECK_TRUE(containsJ);
      CHECK_FALSE(containsK);
    }

    //*************************************************************************
    TEST(test_key_comp)
    {
      static constexpr Data data;
      static constexpr Data::key_compare compare = data.key_comp();

#ifdef TEST_GREATER_THAN
      CHECK_FALSE(compare(Key{ 'A' }, Key{ 'A' }));
      CHECK_TRUE(compare(Key{ 'B' }, Key{ 'A' }));
      CHECK_FALSE(compare(Key{ 'A' }, Key{ 'B' }));
#else
      CHECK_FALSE(compare(Key{ 'A' }, Key{ 'A' }));
      CHECK_FALSE(compare(Key{ 'B' }, Key{ 'A' }));
      CHECK_TRUE(compare(Key{ 'A' }, Key{ 'B' }));
#endif
    }

    //*************************************************************************
    TEST(test_key_comp_transparent_comparator)
    {
      static constexpr DataTransparentComparator data;
      static constexpr DataTransparentComparator::key_compare compare = data.key_comp();

#ifdef TEST_GREATER_THAN
      CHECK_FALSE(compare('A', 'A'));
      CHECK_TRUE(compare('B', 'A'));
      CHECK_FALSE(compare('A', 'B'));
#else
      CHECK_FALSE(compare('A', 'A'));
      CHECK_FALSE(compare('B', 'A'));
      CHECK_TRUE(compare('A', 'B'));
#endif
    }

    //*************************************************************************
    TEST(test_key_comp_constexpr)
    {
      static constexpr Data data;
      static constexpr Data::key_compare compare = data.key_comp();

      static constexpr bool compareAA = compare(Key{ 'A' }, Key{ 'A' });
      static constexpr bool compareBA = compare(Key{ 'B' }, Key{ 'A' });
      static constexpr bool compareAB = compare(Key{ 'A' }, Key{ 'B' });

#ifdef TEST_GREATER_THAN
      CHECK_FALSE(compareAA);
      CHECK_TRUE(compareBA);
      CHECK_FALSE(compareAB);
#else
      CHECK_FALSE(compareAA);
      CHECK_FALSE(compareBA);
      CHECK_TRUE(compareAB);
#endif
    }

    //*************************************************************************
    TEST(test_key_comp_constexpr_transparent_comparator)
    {
      static constexpr DataTransparentComparator data;
      static constexpr DataTransparentComparator::key_compare compare = data.key_comp();

      static constexpr bool compareAA = compare('A', 'A');
      static constexpr bool compareBA = compare('B', 'A');
      static constexpr bool compareAB = compare('A', 'B');

#ifdef TEST_GREATER_THAN
      CHECK_FALSE(compareAA);
      CHECK_TRUE(compareBA);
      CHECK_FALSE(compareAB);
#else
      CHECK_FALSE(compareAA);
      CHECK_FALSE(compareBA);
      CHECK_TRUE(compareAB);
#endif
    }

    //*************************************************************************
    TEST(test_value_comp)
    {
      static constexpr Data data;
      static constexpr Data::value_compare compare = data.value_comp();

#ifdef TEST_GREATER_THAN
      CHECK_FALSE(compare(value_type{ Key{ 'A' }, 0 }, value_type{ Key{ 'A' }, 0 }));
      CHECK_FALSE(compare(value_type{ Key{ 'A' }, 0 }, Key{ 'A' }));
      CHECK_FALSE(compare(Key{ 'A' }, value_type{ Key{ 'A' }, 0 }));

      CHECK_TRUE(compare(value_type{ Key{ 'B' }, 1 }, value_type{ Key{ 'A' }, 0 }));
      CHECK_TRUE(compare(value_type{ Key{ 'B' }, 1 }, Key{ 'A' }));
      CHECK_TRUE(compare(Key{ 'B' }, value_type{ Key{ 'A' }, 0 }));

      CHECK_FALSE(compare(value_type{ Key{ 'A' }, 0 }, value_type{ Key{ 'B' }, 1 }));
      CHECK_FALSE(compare(value_type{ Key{ 'A' }, 0 }, Key{ 'B' }));
      CHECK_FALSE(compare(Key{ 'A' }, value_type{ Key{ 'B' }, 1 }));
#else
      CHECK_FALSE(compare(value_type{ Key{ 'A' }, 0 }, value_type{ Key{ 'A' }, 0 }));
      CHECK_FALSE(compare(value_type{ Key{ 'A' }, 0 }, Key{ 'A' }));
      CHECK_FALSE(compare(Key{ 'A' }, value_type{ Key{ 'A' }, 0 }));

      CHECK_FALSE(compare(value_type{ Key{ 'B' }, 1 }, value_type{ Key{ 'A' }, 0 }));
      CHECK_FALSE(compare(value_type{ Key{ 'B' }, 1 }, Key{ 'A' }));
      CHECK_FALSE(compare(Key{ 'B' }, value_type{ Key{ 'A' }, 0 }));

      CHECK_TRUE(compare(value_type{ Key{ 'A' }, 0 }, value_type{ Key{ 'B' }, 1 }));
      CHECK_TRUE(compare(value_type{ Key{ 'A' }, 0 }, Key{ 'B' }));
      CHECK_TRUE(compare(Key{ 'A' }, value_type{ Key{ 'B' }, 1 }));
#endif
    }

    //*************************************************************************
    TEST(test_value_comp_transparent_comparator)
    {
      static constexpr DataTransparentComparator data;
      static constexpr DataTransparentComparator::value_compare compare = data.value_comp();

#ifdef TEST_GREATER_THAN
      CHECK_FALSE(compare(value_type{ 'A', 0 }, value_type{ 'A', 0 }));
      CHECK_FALSE(compare(value_type{ 'A', 0 }, 'A'));
      CHECK_FALSE(compare('A', value_type{ 'A', 0 }));

      CHECK_TRUE(compare(value_type{ 'B', 1 }, value_type{ 'A', 0 }));
      CHECK_TRUE(compare(value_type{ 'B', 1 }, 'A'));
      CHECK_TRUE(compare('B', value_type{ 'A', 0 }));

      CHECK_FALSE(compare(value_type{ 'A', 0 }, value_type{ 'B', 1 }));
      CHECK_FALSE(compare(value_type{ 'A', 0 }, 'B'));
      CHECK_FALSE(compare('A', value_type{ 'B', 1 }));
#else
      CHECK_FALSE(compare(value_type{ 'A', 0 }, value_type{ 'A', 0 }));
      CHECK_FALSE(compare(value_type{ 'A', 0 }, 'A'));
      CHECK_FALSE(compare('A', value_type{ 'A', 0 }));

      CHECK_FALSE(compare(value_type{ 'B', 1 }, value_type{ 'A', 0 }));
      CHECK_FALSE(compare(value_type{ 'B', 1 }, 'A'));
      CHECK_FALSE(compare('B', value_type{ 'A', 0 }));

      CHECK_TRUE(compare(value_type{ 'A', 0 }, value_type{ 'B', 1 }));
      CHECK_TRUE(compare(value_type{ 'A', 0 }, 'B'));
      CHECK_TRUE(compare('A', value_type{ 'B', 1 }));
#endif
    }

    //*************************************************************************
    TEST(test_value_comp_constexpr)
    {
      static constexpr Data data;
      static constexpr Data::value_compare compare = data.value_comp();

      static constexpr bool compareAA1 = compare(value_type{ Key{ 'A' }, 0 }, value_type{ Key{ 'A' }, 0 });
      static constexpr bool compareAA2 = compare(value_type{ Key{ 'A' }, 0 }, Key{ 'A' });
      static constexpr bool compareAA3 = compare(Key{ 'A' }, value_type{ Key{ 'A' }, 0 });

      static constexpr bool compareBA1 = compare(value_type{ Key{ 'B' }, 0 }, value_type{ Key{ 'A' }, 0 });
      static constexpr bool compareBA2 = compare(value_type{ Key{ 'B' }, 0 }, Key{ 'A' });
      static constexpr bool compareBA3 = compare(Key{ 'B' }, value_type{ Key{ 'A' }, 0 });
          
      static constexpr bool compareAB1 = compare(value_type{ Key{ 'A' }, 0 }, value_type{ Key{ 'B' }, 0 });
      static constexpr bool compareAB2 = compare(value_type{ Key{ 'A' }, 0 }, Key{ 'B' });
      static constexpr bool compareAB3 = compare(Key{ 'A' }, value_type{ Key{ 'B' }, 0 });;

#ifdef TEST_GREATER_THAN
      CHECK_FALSE(compareAA1);
      CHECK_FALSE(compareAA2);
      CHECK_FALSE(compareAA3);

      CHECK_TRUE(compareBA1);
      CHECK_TRUE(compareBA2);
      CHECK_TRUE(compareBA3);

      CHECK_FALSE(compareAB1);
      CHECK_FALSE(compareAB2);
      CHECK_FALSE(compareAB3);
#else
      CHECK_FALSE(compareAA1);
      CHECK_FALSE(compareAA2);
      CHECK_FALSE(compareAA3);

      CHECK_FALSE(compareBA1);
      CHECK_FALSE(compareBA2);
      CHECK_FALSE(compareBA3);

      CHECK_TRUE(compareAB1);
      CHECK_TRUE(compareAB2);
      CHECK_TRUE(compareAB3);
#endif
    }

    //*************************************************************************
    TEST(test_value_comp_constexpr_transparent_comparator)
    {
      static constexpr DataTransparentComparator data;
      static constexpr DataTransparentComparator::value_compare compare = data.value_comp();

      static constexpr bool compareAA1 = compare(value_type{ 'A', 0 }, value_type{ 'A', 0 });
      static constexpr bool compareAA2 = compare(value_type{ 'A', 0 }, 'A');
      static constexpr bool compareAA3 = compare('A', value_type{ 'A', 0 });

      static constexpr bool compareBA1 = compare(value_type{ 'B', 0 }, value_type{ 'A', 0 });
      static constexpr bool compareBA2 = compare(value_type{ 'B', 0 }, 'A');
      static constexpr bool compareBA3 = compare('B', value_type{ 'A', 0 });

      static constexpr bool compareAB1 = compare(value_type{ 'A', 0 }, value_type{ 'B', 0 });
      static constexpr bool compareAB2 = compare(value_type{ 'A', 0 }, 'B');
      static constexpr bool compareAB3 = compare('A', value_type{ 'B', 0 });;

#ifdef TEST_GREATER_THAN
      CHECK_FALSE(compareAA1);
      CHECK_FALSE(compareAA2);
      CHECK_FALSE(compareAA3);

      CHECK_TRUE(compareBA1);
      CHECK_TRUE(compareBA2);
      CHECK_TRUE(compareBA3);

      CHECK_FALSE(compareAB1);
      CHECK_FALSE(compareAB2);
      CHECK_FALSE(compareAB3);
#else
      CHECK_FALSE(compareAA1);
      CHECK_FALSE(compareAA2);
      CHECK_FALSE(compareAA3);

      CHECK_FALSE(compareBA1);
      CHECK_FALSE(compareBA2);
      CHECK_FALSE(compareBA3);

      CHECK_TRUE(compareAB1);
      CHECK_TRUE(compareAB2);
      CHECK_TRUE(compareAB3);
#endif
    }
  };
}
