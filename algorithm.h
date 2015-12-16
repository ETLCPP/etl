///\file

/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl
http://www.etlcpp.com

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

#ifndef __ETL_ALGORITHM__
#define __ETL_ALGORITHM__

///\defgroup algorithm algorithm
/// Reverse engineered algorithms from C++ 0x11
///\ingroup utilities

#include <algorithm>
#include <iterator>
#include <utility>
#include <functional>
#include <stdint.h>

#include "type_traits.h"

namespace etl
{
  //***************************************************************************
  /// Finds the greatest and the smallest element in the range (begin, end).<br>
  ///<a href="http://en.cppreference.com/w/cpp/algorithm/minmax_element"></a>
  ///\ingroup algorithm
  //***************************************************************************
  template <typename TIterator, typename TCompare>
  std::pair<TIterator, TIterator> minmax_element(TIterator begin, TIterator end, TCompare compare)
  {
    TIterator minimum = begin;
    TIterator maximum = begin;

    while (begin != end)
    {
      if (compare(*begin, *minimum))
      {
        minimum = begin;
      }

      if (compare(*maximum, *begin))
      {
        maximum = begin;
      }
        
      ++begin;
    }

    return std::pair<TIterator, TIterator>(minimum, maximum);
  }

  //***************************************************************************
  /// minmax_element
  ///\ingroup algorithm
  ///<a href="http://en.cppreference.com/w/cpp/algorithm/minmax_element"></a>
  //***************************************************************************
  template <typename TIterator>
  std::pair<TIterator, TIterator> minmax_element(TIterator begin, TIterator end)
  {
      typedef typename std::iterator_traits<TIterator>::value_type value_t;

      return etl::minmax_element(begin, end, std::less<value_t>());
  }

  //***************************************************************************
  /// minmax
  ///\ingroup algorithm
  ///<a href="http://en.cppreference.com/w/cpp/algorithm/minmax"></a>
  //***************************************************************************
  template <typename T>
  std::pair<const T&, const T&> minmax(const T& a, const T& b)
  {
	  return (b < a) ? std::pair<const T&, const T&>(b, a) : std::pair<const T&, const T&>(a, b);
  }

  //***************************************************************************
  /// minmax
  ///\ingroup algorithm
  ///<a href="http://en.cppreference.com/w/cpp/algorithm/minmax"></a>
  //***************************************************************************
  template <typename T, typename TCompare>
  std::pair<const T&, const T&> minmax(const T& a, const T& b, TCompare compare)
  {
    return compare(b, a) ? std::pair<const T&, const T&>(b, a) : std::pair<const T&, const T&>(a, b);
  }

  //***************************************************************************
  /// is_sorted_until
  ///\ingroup algorithm
  ///<a href="http://en.cppreference.com/w/cpp/algorithm/is_sorted_until"></a>
  //***************************************************************************
  template <typename TIterator>
  TIterator is_sorted_until(TIterator begin, TIterator end)
  {
    if (begin != end)
    {
      TIterator next = begin;

      while (++next != end)
      {
        if (*next < *begin)
        {
          return next;
        }

        ++begin;
      }
    }

    return end;
  }

  //***************************************************************************
  /// is_sorted_until
  ///\ingroup algorithm
  ///<a href="http://en.cppreference.com/w/cpp/algorithm/is_sorted_until"></a>
  //***************************************************************************
  template <typename TIterator, typename TCompare>
  TIterator is_sorted_until(TIterator begin, TIterator end, TCompare compare)
  {
    if (begin != end)
    {
      TIterator next = begin;

      while (++next != end)
      {
        if (compare(*next, *begin))
        {
          return next;
        }

        ++begin;
      }
    }

    return end;
  }

  //***************************************************************************
  /// is_sorted
  ///\ingroup algorithm
  ///<a href="http://en.cppreference.com/w/cpp/algorithm/is_sorted"></a>
  //***************************************************************************
  template<typename TIterator>
  bool is_sorted(TIterator begin, TIterator end)
  {
    return etl::is_sorted_until(begin, end) == end;
  }

  //***************************************************************************
  /// is_sorted
  ///\ingroup algorithm
  ///<a href="http://en.cppreference.com/w/cpp/algorithm/is_sorted"></a>
  //***************************************************************************
  template<typename TIterator, typename TCompare>
  bool is_sorted(TIterator begin, TIterator end, TCompare compare)
  {
    return etl::is_sorted_until(begin, end, compare) == end;
  }

  //***************************************************************************
  /// copy_n
  ///\ingroup algorithm
  ///<a href="http://en.cppreference.com/w/cpp/algorithm/copy_n"></a>
  //***************************************************************************
  template <typename TInputIterator, typename Size, typename TOutputIterator>
  TOutputIterator copy_n(TInputIterator begin, Size count, TOutputIterator result)
  {
    if (count > 0)
    {
      for (Size i = 0; i < count; ++i)
      {
        *result++ = *begin++;
      }
    }

    return result;
  }

  //***************************************************************************
  /// copy_if
  ///\ingroup algorithm
  ///<a href="http://en.cppreference.com/w/cpp/algorithm/copy"></a>
  //***************************************************************************
  template <typename TIterator, typename TOutputIterator, typename TUnaryPredicate>
  TOutputIterator copy_if(TIterator begin, TIterator end, TOutputIterator out, TUnaryPredicate predicate)
  {
    while (begin != end)
    {
      if (predicate(*begin))
      {
        *out++ = *begin;
      }

      ++begin;
    }

    return out;
  }

  //***************************************************************************
  /// find_if_not
  ///\ingroup algorithm
  ///<a href="http://en.cppreference.com/w/cpp/algorithm/find"></a>
  //***************************************************************************
  template <typename TIterator, typename TUnaryPredicate>
  TIterator find_if_not(TIterator begin, TIterator end, TUnaryPredicate predicate)
  {
    while (begin != end)
    {
      if (!predicate(*begin))
      {
        return begin;
      }

      ++begin;
    }

    return end;
  }

  //***************************************************************************
  /// all_of
  ///\ingroup algorithm
  ///<a href="http://en.cppreference.com/w/cpp/algorithm/all_any_none_of"></a>
  //***************************************************************************
  template <typename TIterator, typename TUnaryPredicate >
  bool all_of(TIterator begin, TIterator end, TUnaryPredicate predicate)
  {
    return etl::find_if_not(begin, end, predicate) == end;
  }

  //***************************************************************************
  /// any_of
  ///\ingroup algorithm
  ///<a href="http://en.cppreference.com/w/cpp/algorithm/all_any_none_of"></a>
  //***************************************************************************
  template <typename TIterator, typename TUnaryPredicate >
  bool any_of(TIterator begin, TIterator end, TUnaryPredicate predicate)
  {
    return std::find_if(begin, end, predicate) != end;
  }

  //***************************************************************************
  /// none_of
  ///\ingroup algorithm
  ///<a href="http://en.cppreference.com/w/cpp/algorithm/all_any_none_of"></a>
  //***************************************************************************
  template <typename TIterator, typename TUnaryPredicate >
  bool none_of(TIterator begin, TIterator end, TUnaryPredicate predicate)
  {
    return std::find_if(begin, end, predicate) == end;
  }

  //***************************************************************************
  /// is_permutation
  ///\ingroup algorithm
  ///<a href="http://en.cppreference.com/w/cpp/algorithm/is_permutation"></a>
  //***************************************************************************
  template <typename TIterator1, typename TIterator2>
  bool is_permutation(TIterator1 begin1, TIterator1 end1, TIterator2 begin2)
  {
    if (begin1 != end1)
    {
      TIterator2 end2 = begin2;

      std::advance(end2, std::distance(begin1, end1));

      for (TIterator1 i = begin1; i != end1; ++i)
      {
        if (i == std::find(begin1, i, *i))
        {
          int32_t n = std::count(begin2, end2, *i);

          if (n == 0 || std::count(i, end1, *i) != n)
          {
            return false;
          }
        }
      }
    }

    return true;
  }

  //***************************************************************************
  /// is_permutation
  ///\ingroup algorithm
  ///<a href="http://en.cppreference.com/w/cpp/algorithm/is_permutation"></a>
  //***************************************************************************
  template <typename TIterator1, typename TIterator2>
  bool is_permutation(TIterator1 begin1, TIterator1 end1, TIterator2 begin2, TIterator2 end2)
  {
    if (begin1 != end1)
    {
      for (TIterator1 i = begin1; i != end1; ++i)
      {
        if (i == std::find(begin1, i, *i))
        {
          int32_t n = std::count(begin2, end2, *i);

          if (n == 0 || std::count(i, end1, *i) != n)
          {
            return false;
          }
        }
      }
    }

    return true;
  }

  //***************************************************************************
  /// is_permutation
  ///\ingroup algorithm
  ///<a href="http://en.cppreference.com/w/cpp/algorithm/is_permutation"></a>
  //***************************************************************************
  template <typename TIterator1, typename TIterator2, typename TBinaryPredicate>
  bool is_permutation(TIterator1 begin1, TIterator1 end1, TIterator2 begin2, TBinaryPredicate predicate)
  {
    if (begin1 != end1)
    {
      TIterator2 end2 = begin2;

      std::advance(end2, std::distance(begin1, end1));

      for (TIterator1 i = begin1; i != end1; ++i)
      {
        if (i == std::find_if(begin1, i, std::bind1st(predicate, *i)))
        {
          int32_t n = std::count(begin2, end2, *i);

          if (n == 0 || std::count(i, end1, *i) != n)
          {
            return false;
          }
        }
      }
    }

    return true;
  }

  //***************************************************************************
  /// is_permutation
  ///\ingroup algorithm
  ///<a href="http://en.cppreference.com/w/cpp/algorithm/is_permutation"></a>
  //***************************************************************************
  template <typename TIterator1, typename TIterator2, typename TBinaryPredicate>
  bool is_permutation(TIterator1 begin1, TIterator1 end1, TIterator2 begin2, TIterator2 end2, TBinaryPredicate predicate)
  {
    if (begin1 != end1)
    {
      for (TIterator1 i = begin1; i != end1; ++i)
      {
        if (i == std::find_if(begin1, i, std::bind1st(predicate, *i)))
        {
          int32_t n = std::count(begin2, end2, *i);

          if (n == 0 || std::count(i, end1, *i) != n)
          {
            return false;
          }
        }
      }
    }

    return true;
  }

  //***************************************************************************
  /// is_partitioned
  ///\ingroup algorithm
  ///<a href="http://en.cppreference.com/w/cpp/algorithm/is_partitioned"></a>
  //***************************************************************************
  template <typename TIterator, typename TUnaryPredicate>
  bool is_partitioned(TIterator begin, TIterator end, TUnaryPredicate predicate)
  {
    while (begin != end)
    {
      if (!predicate(*begin++))
      {
        break;
      }
    }

    while (begin != end)
    {
      if (predicate(*begin++))
      {
        return false;
      }
    }

    return true;
  }

  //***************************************************************************
  /// partition_point
  ///<a href="http://en.cppreference.com/w/cpp/algorithm/partition_point"></a>
  ///\ingroup algorithm
  //***************************************************************************
  template <typename TIterator, typename TUnaryPredicate>
  TIterator partition_point(TIterator begin, TIterator end, TUnaryPredicate predicate)
  {
    while (begin != end)
    {
      if (!predicate(*begin))
      {
        return begin;
      }

      ++begin;
    }

    return begin;
  }

  //***************************************************************************
  /// Copies the elements from the range (begin, end) to two different ranges
  /// depending on the value returned by the predicate.<br>
  ///<a href="http://en.cppreference.com/w/cpp/algorithm/partition_copy"></a>
  ///\ingroup algorithm
  //***************************************************************************
  template <typename TSource, typename TDestinationTrue, typename TDestinationFalse, typename TUnaryPredicate>
  std::pair<TDestinationTrue, TDestinationFalse> partition_copy(TSource           begin,
	                                                              TSource           end,
	                                                              TDestinationTrue  destination_true,
															                                	TDestinationFalse destination_false,
															                                	TUnaryPredicate   predicate)
  {
    while (begin != end)
    {
      if (predicate(*begin))
      {
        *destination_true++ = *begin++;
      }
      else
      {
        *destination_false++ = *begin++;
      }
    }

    return std::pair<TDestinationTrue, TDestinationFalse>(destination_true, destination_false);
  }
}

#endif

