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
  constexpr bool operator <(const Key& lhs, const Key& rhs) ETL_NOEXCEPT
  {
    return (lhs.k < rhs.k);
  }

  // Less-than operator for Key < char
  constexpr bool operator <(const Key& lhs, char rhs) ETL_NOEXCEPT
  {
    return (lhs.k < rhs);
  }

  // Less-than operator for char < Key
  constexpr bool operator <(char lhs, const Key& rhs) ETL_NOEXCEPT
  {
    return (lhs < rhs.k);
  }

  // Greater-than operator for Key > char
  constexpr bool operator >(const Key& lhs, const Key& rhs) ETL_NOEXCEPT
  {
    return (lhs.k > rhs.k);
  }

  // Greater-than operator for char > Key
  constexpr bool operator ==(const Key& lhs, const Key& rhs) ETL_NOEXCEPT
  {
    return (lhs.k == rhs.k);
  }

  //#define TEST_GREATER_THAN
  #ifdef TEST_GREATER_THAN
    using Data                      = etl::const_map<Key, int, Max_Size, std::greater<Key>>;
    using DataTransparentComparator = etl::const_map<Key, int, Max_Size, std::greater<>>;
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
#include "etl/private/diagnostic_null_dereference_push.h"
    template <typename T1, typename T2>
    bool Check_Equal(T1 begin1, T1 end1, T2 begin2)
    {
      while (begin1 != end1)
      {
        if ((begin1->first != begin2->first) || (begin1->second != begin2->second))
        {
          return false;
        }

        ++begin1;
        ++begin2;
      }

      return true;
    }
#include "etl/private/diagnostic_pop.h"

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

      CHECK_TRUE(data.is_valid());
      CHECK_TRUE(data.size() == Max_Size);
      CHECK_FALSE(data.empty());
      CHECK_TRUE(data.full());
      CHECK_TRUE(data.capacity() == Max_Size);
      CHECK_TRUE(data.max_size() == Max_Size);
      CHECK_FALSE(data.begin() == data.end());
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
    TEST(test_end)
    {
#ifdef TEST_GREATER_THAN
      static constexpr Data data{ value_type{Key('J'), 9 }, value_type{Key('I'), 8 }, value_type{Key('H'), 7 }, value_type{Key('G'), 6 }, value_type{Key('F'), 5 },
                                  value_type{Key('E'), 4 }, value_type{Key('D'), 3 }, value_type{Key('C'), 2 }, value_type{Key('B'), 1 }, value_type{Key('A'), 0 } };
#else
      static constexpr Data data{ value_type{Key('A'), 0 }, value_type{Key('B'), 1 }, value_type{Key('C'), 2 }, value_type{Key('D'), 3 }, value_type{Key('E'), 4 },
                                  value_type{Key('F'), 5 }, value_type{Key('G'), 6 }, value_type{Key('H'), 7 }, value_type{Key('I'), 8 }, value_type{Key('J'), 9 } };
#endif

      CHECK_TRUE(data.is_valid());
      CHECK_TRUE(data.end() == (data.begin() + data.size()));
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
    TEST(test_index__using_transparent_comparator)
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

//    //*************************************************************************
//    TEST(test_equal_range)
//    {
//      Compare_Data compare_data(random_data.begin(), random_data.end());
//      Data data(random_data.begin(), random_data.end());
//
//      ETL_OR_STD::pair<Data::iterator, Data::iterator> data_result = data.equal_range('2');
//      Data::iterator data_lb = data.lower_bound('2');
//      ETL_OR_STD::pair<Compare_Data::iterator, Compare_Data::iterator> compare_result = compare_data.equal_range('2');
//      Compare_Data::iterator compare_data_lb = compare_data.lower_bound('2');
//
//      // Check that both return the same return results
//      CHECK(data_lb->first == compare_data_lb->first);
//      CHECK(data_lb->second == compare_data_lb->second);
//      CHECK(data_result.first->first == compare_result.first->first);
//      CHECK(data_result.first->second == compare_result.first->second);
//      CHECK(data_result.second->first == compare_result.second->first);
//      CHECK(data_result.second->second == compare_result.second->second);
//
//      bool isEqual = Check_Equal(data.begin(), data.end(), compare_data.begin());
//
//      CHECK(isEqual);
//    }

//    //*************************************************************************
//    TEST(test_equal_range_using_transparent_comparator)
//    {
//      using CMap = std::map<char, int, std::less<char>>;
//      using EMap = etl::map<char, int, Max_Size, etl::less<>>;
//
//      CMap compare_data(random_data.begin(), random_data.end());
//      EMap data(random_data.begin(), random_data.end());
//
//      ETL_OR_STD::pair<EMap::iterator, EMap::iterator> data_result = data.equal_range(Key('2'));
//      EMap::iterator data_lb = data.lower_bound('2');
//      ETL_OR_STD::pair<CMap::iterator, CMap::iterator> compare_result = compare_data.equal_range('2');
//      CMap::iterator compare_data_lb = compare_data.lower_bound('2');
//
//      // Check that both return the same return results
//      CHECK(data_lb->first == compare_data_lb->first);
//      CHECK(data_lb->second == compare_data_lb->second);
//      CHECK(data_result.first->first == compare_result.first->first);
//      CHECK(data_result.first->second == compare_result.first->second);
//      CHECK(data_result.second->first == compare_result.second->first);
//      CHECK(data_result.second->second == compare_result.second->second);
//
//      bool isEqual = Check_Equal(data.begin(), data.end(), compare_data.begin());
//
//      CHECK(isEqual);
//    }

//    //*************************************************************************
//    TEST(test_count)
//    {
//      const Data data(initial_data.begin(), initial_data.end());
//
//      CHECK(data.count('3') == 1UL);
//
//      CHECK(data.count(Key('A') == 0UL);
//    }

//    //*************************************************************************
//    TEST(test_count_using_transparent_comparator)
//    {
//      using EMap = etl::map<char, int, Max_Size, etl::less<>>;
//
//      const EMap data(initial_data.begin(), initial_data.end());
//
//      CHECK(data.count(Key('3')) == 1UL);
//
//      CHECK(data.count(Key(Key('A')) == 0UL);
//    }

//    //*************************************************************************
//    TEST(test_iterator)
//    {
//      Compare_Data compare_data(initial_data.begin(), initial_data.end());
//
//      Data data(compare_data.begin(), compare_data.end());
//
//      bool isEqual = Check_Equal(data.begin(),
//                                 data.end(),
//                                 compare_data.begin());
//
//      CHECK(isEqual);
//    }

//    //*************************************************************************
//    TEST(test_const_iterator)
//    {
//      Compare_Data compare_data(initial_data.begin(), initial_data.end());
//
//      Data data(compare_data.begin(), compare_data.end());
//
//      bool isEqual = Check_Equal(data.cbegin(),
//                                 data.cend(),
//                                 compare_data.cbegin());
//
//      CHECK(isEqual);
//    }

//    //*************************************************************************
//    TEST(test_find)
//    {
//      Data data(initial_data.begin(), initial_data.end());
//
//      Data::iterator it = data.find('3');
//      CHECK(3 == it->second);
//
//      it = data.find(Key('A');
//      CHECK(data.end() == it);
//
//      it = data.find('!');
//      CHECK(data.end() == it);
//    }

//    //*************************************************************************
//    TEST(test_find_using_transparent_comparator)
//    {
//      using EMap = etl::map<char, int, Max_Size, etl::less<>>;
//
//      EMap data(initial_data.begin(), initial_data.end());
//
//      EMap::iterator it = data.find(Key('3'));
//      CHECK(3 == it->second);
//
//      it = data.find(Key(Key('A'));
//      CHECK(data.end() == it);
//
//      it = data.find(Key('!'));
//      CHECK(data.end() == it);
//    }

//    //*************************************************************************
//    TEST(test_equal)
//    {
//      const Data initial1(initial_data.begin(), initial_data.end());
//      const Data initial2(initial_data.begin(), initial_data.end());
//
//      CHECK(initial1 == initial2);
//
//      const Data different(different_data.begin(), different_data.end());
//
//      CHECK(!(initial1 == different));
//    }

//    //*************************************************************************
//    TEST(test_not_equal)
//    {
//      const Data initial1(initial_data.begin(), initial_data.end());
//      const Data initial2(initial_data.begin(), initial_data.end());
//
//      CHECK(!(initial1 != initial2));
//
//      const Data different(different_data.begin(), different_data.end());
//
//      CHECK(initial1 != different);
//    }

//    //*************************************************************************
//    TEST(test_lower_bound)
//    {
//      Compare_Data compare_data(initial_data.begin(), initial_data.end());
//      Data data(initial_data.begin(), initial_data.end());
//
//      Compare_Data::iterator i_compare = compare_data.lower_bound('8');
//      Data::iterator i_data = data.lower_bound('8');
//      CHECK(i_compare->second == i_data->second);
//
//#ifdef TEST_GREATER_THAN
//      i_compare = compare_data.lower_bound('.');
//      CHECK(compare_data.end() == i_compare);
//
//      i_data = data.lower_bound('.');
//      CHECK(data.end() == i_data);
//
//      i_compare = compare_data.lower_bound(Key('A');
//      i_data = data.lower_bound(Key('A');
//      CHECK(i_compare->second == i_data->second);
//#else
//      i_compare = compare_data.lower_bound('.');
//      i_data = data.lower_bound('.');
//      CHECK(i_compare->second == i_data->second);
//
//      i_compare = compare_data.lower_bound(Key('A');
//      CHECK(compare_data.end() == i_compare);
//
//      i_data = data.lower_bound(Key('A');
//      CHECK(data.end() == i_data);
//#endif
//    }

//    //*************************************************************************
//    TEST(test_lower_bound_using_transparent_comparator)
//    {
//      using CMap = std::map<char, int, std::less<char>>;
//      using EMap = etl::map<char, int, Max_Size, etl::less<>>;
//
//      CMap compare_data(initial_data.begin(), initial_data.end());
//      EMap data(initial_data.begin(), initial_data.end());
//
//      CMap::iterator i_compare = compare_data.lower_bound('8');
//      EMap::iterator i_data = data.lower_bound(Key('8'));
//      CHECK(i_compare->second == i_data->second);
//
//      i_compare = compare_data.lower_bound('.');
//      i_data = data.lower_bound(Key('.'));
//      CHECK(i_compare->second == i_data->second);
//
//      i_compare = compare_data.lower_bound(Key('A');
//      CHECK(compare_data.end() == i_compare);
//
//      i_data = data.lower_bound(Key(Key('A'));
//      CHECK(data.end() == i_data);
//    }

//    //*************************************************************************
//    TEST(test_upper_bound)
//    {
//      Compare_Data compare_data(initial_data.begin(), initial_data.end());
//      Data data(initial_data.begin(), initial_data.end());
//
//      Compare_Data::iterator i_compare = compare_data.upper_bound('2');
//      Data::iterator i_data = data.upper_bound('2');
//      CHECK(i_compare->second == i_data->second);
//
//#ifdef TEST_GREATER_THAN
//      i_compare = compare_data.upper_bound('.');
//      CHECK(compare_data.end() == i_compare);
//
//      i_data = data.upper_bound('.');
//      CHECK(data.end() == i_data);
//
//      i_compare = compare_data.upper_bound(Key('A');
//      i_data = data.upper_bound(Key('A');
//      CHECK(i_compare->second == i_data->second);
//#else
//      i_compare = compare_data.upper_bound('.');
//      i_data = data.upper_bound('.');
//      CHECK(i_compare->second == i_data->second);
//
//      i_compare = compare_data.upper_bound(Key('A');
//      CHECK(compare_data.end() == i_compare);
//
//      i_data = data.upper_bound(Key('A');
//      CHECK(data.end() == i_data);
//#endif
//    }

//    //*************************************************************************
//    TEST(test_upper_bound_using_transparent_comparator)
//    {
//      using CMap = std::map<char, int, std::less<char>>;
//      using EMap = etl::map<char, int, Max_Size, etl::less<>>;
//
//      CMap compare_data(initial_data.begin(), initial_data.end());
//      EMap data(initial_data.begin(), initial_data.end());
//
//      CMap::iterator i_compare = compare_data.upper_bound('2');
//      EMap::iterator i_data = data.upper_bound(Key('2'));
//      CHECK(i_compare->second == i_data->second);
//
//      i_compare = compare_data.upper_bound('.');
//      i_data = data.upper_bound(Key('.'));
//      CHECK(i_compare->second == i_data->second);
//
//      i_compare = compare_data.upper_bound(Key('A');
//      CHECK(compare_data.end() == i_compare);
//
//      i_data = data.upper_bound(Key(Key('A'));
//      CHECK(data.end() == i_data);
//    }

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

//    //*************************************************************************
//    TEST(test_contains)
//    {
//      Data data(initial_data.begin(), initial_data.end());
//
//      CHECK(data.contains(char('1')));
//      CHECK(!data.contains(char('99')));
//    }

//    //*************************************************************************
//    TEST(test_contains_with_transparent_comparator)
//    {
//      etl::map<char, int, Max_Size, etl::less<>> data(initial_data.begin(), initial_data.end());
//
//      CHECK(data.contains(char('1')));
//      CHECK(data.contains(Key('1')));
//
//      CHECK(!data.contains(char('99')));
//      CHECK(!data.contains(Key('99')));
//    }
  };
}
