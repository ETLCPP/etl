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
/// Additional new variants of certain algorithms.
///\ingroup utilities

#include <algorithm>
#include <iterator>
#include <utility>
#include <functional>
#include <iterator>
#include <stdint.h>

#include "iterator.h"
#include "type_traits.h"

namespace etl
{
  //***************************************************************************
  /// Finds the greatest and the smallest element in the range (begin, end).<br>
  ///<a href="http://en.cppreference.com/w/cpp/algorithm/minmax_element"></a>
  ///\ingroup algorithm
  //***************************************************************************
  template <typename TIterator,
            typename TCompare>
  std::pair<TIterator, TIterator> minmax_element(TIterator begin,
                                                 TIterator end,
                                                 TCompare  compare)
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
  std::pair<TIterator, TIterator> minmax_element(TIterator begin,
                                                 TIterator end)
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
  std::pair<const T&, const T&> minmax(const T& a,
                                       const T& b)
  {
    return (b < a) ? std::pair<const T&, const T&>(b, a) : std::pair<const T&, const T&>(a, b);
  }

  //***************************************************************************
  /// minmax
  ///\ingroup algorithm
  ///<a href="http://en.cppreference.com/w/cpp/algorithm/minmax"></a>
  //***************************************************************************
  template <typename T,
            typename TCompare>
  std::pair<const T&, const T&> minmax(const T& a,
                                       const T& b,
                                       TCompare compare)
  {
    return compare(b, a) ? std::pair<const T&, const T&>(b, a) : std::pair<const T&, const T&>(a, b);
  }

  //***************************************************************************
  /// is_sorted_until
  ///\ingroup algorithm
  ///<a href="http://en.cppreference.com/w/cpp/algorithm/is_sorted_until"></a>
  //***************************************************************************
  template <typename TIterator>
  TIterator is_sorted_until(TIterator begin,
                            TIterator end)
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
  template <typename TIterator,
            typename TCompare>
  TIterator is_sorted_until(TIterator begin,
                            TIterator end,
                            TCompare  compare)
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
  bool is_sorted(TIterator begin,
                 TIterator end)
  {
    return etl::is_sorted_until(begin, end) == end;
  }

  //***************************************************************************
  /// is_sorted
  ///\ingroup algorithm
  ///<a href="http://en.cppreference.com/w/cpp/algorithm/is_sorted"></a>
  //***************************************************************************
  template<typename TIterator,
           typename TCompare>
  bool is_sorted(TIterator begin,
                 TIterator end,
                 TCompare  compare)
  {
    return etl::is_sorted_until(begin, end, compare) == end;
  }

  //***************************************************************************
  /// copy
  /// A form of copy where the smallest of the two ranges is used.
  /// There is currently no STL equivalent.
  /// Specialisation for random access iterators.
  ///\param i_begin Beginning of the input range.
  ///\param i_end   End of the input range.
  ///\param o_begin Beginning of the output range.
  ///\param o_end   End of the output range.
  ///\ingroup algorithm
  //***************************************************************************
  template <typename TInputIterator,
            typename TOutputIterator>
  typename etl::enable_if<etl::is_random_iterator<TInputIterator>::value &&
                          etl::is_random_iterator<TOutputIterator>::value, TOutputIterator>::type
   copy(TInputIterator  i_begin,
        TInputIterator  i_end,
        TOutputIterator o_begin,
        TOutputIterator o_end)
  {
      size_t s_size = std::distance(i_begin, i_end);
      size_t d_size = std::distance(o_begin, o_end);
      size_t size   = (s_size < d_size) ? s_size : d_size;

      return std::copy(i_begin, i_begin + size, o_begin);
  }

  //***************************************************************************
  /// copy
  /// A form of copy where the smallest of the two ranges is used.
  /// There is currently no STL equivalent.
  /// Specialisation for non random access iterators.
  ///\param i_begin Beginning of the input range.
  ///\param i_end   End of the input range.
  ///\param o_begin Beginning of the output range.
  ///\param o_end   End of the output range.
  ///\ingroup algorithm
  //***************************************************************************
  template <typename TInputIterator,
            typename TOutputIterator>
  typename etl::enable_if<!etl::is_random_iterator<TInputIterator>::value ||
                          !etl::is_random_iterator<TOutputIterator>::value, TOutputIterator>::type
   copy(TInputIterator  i_begin,
        TInputIterator  i_end,
        TOutputIterator o_begin,
        TOutputIterator o_end)
  {
    while ((i_begin != i_end) && (o_begin != o_end))
    {
      *o_begin++ = *i_begin++;
    }

    return o_begin;
  }

  //***************************************************************************
  /// copy_n
  ///\ingroup algorithm
  ///<a href="http://en.cppreference.com/w/cpp/algorithm/copy_n"></a>
  //***************************************************************************
  template <typename TInputIterator,
            typename TSize,
            typename TOutputIterator>
  TOutputIterator copy_n(TInputIterator  begin,
                         TSize           count,
                         TOutputIterator result)
  {
    return std::copy(begin, begin + count, result);
  }

  //***************************************************************************
  /// copy_n
  /// A form of copy_n where the smallest of the two ranges is used.
  ///\ingroup algorithm
  //***************************************************************************
  template <typename TInputIterator,
            typename TSize,
            typename TOutputIterator>
  TOutputIterator copy_n(TInputIterator  i_begin,
                         TSize           count,
                         TOutputIterator o_begin,
                         TOutputIterator o_end)
  {
    return  etl::copy(i_begin, i_begin + count, o_begin, o_end);;
  }

  //***************************************************************************
  /// copy_if
  ///\ingroup algorithm
  ///<a href="http://en.cppreference.com/w/cpp/algorithm/copy"></a>
  //***************************************************************************
  template <typename TIterator,
            typename TOutputIterator,
            typename TUnaryPredicate>
  TOutputIterator copy_if(TIterator       begin,
                          TIterator       end,
                          TOutputIterator out,
                          TUnaryPredicate predicate)
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
  /// copy_if
  /// A form of copy_if where it terminates when the first end iterator is reached.
  /// There is currently no STL equivelent.
  ///\ingroup algorithm
  //***************************************************************************
  template <typename TInputIterator,
            typename TOutputIterator,
            typename TUnaryPredicate>
  TOutputIterator copy_if(TInputIterator  i_begin,
                          TInputIterator  i_end,
                          TOutputIterator o_begin,
                          TOutputIterator o_end,
                          TUnaryPredicate predicate)
  {
    while ((i_begin != i_end) && (o_begin != o_end))
    {
      if (predicate(*i_begin))
      {
        *o_begin++ = *i_begin;
      }

      ++i_begin;
    }

    return o_begin;
  }

  //***************************************************************************
  /// find_if_not
  ///\ingroup algorithm
  ///<a href="http://en.cppreference.com/w/cpp/algorithm/find"></a>
  //***************************************************************************
  template <typename TIterator,
            typename TUnaryPredicate>
  TIterator find_if_not(TIterator       begin,
                        TIterator       end,
                        TUnaryPredicate predicate)
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
  template <typename TIterator,
            typename TUnaryPredicate>
  bool all_of(TIterator       begin,
              TIterator       end,
              TUnaryPredicate predicate)
  {
    return etl::find_if_not(begin, end, predicate) == end;
  }

  //***************************************************************************
  /// any_of
  ///\ingroup algorithm
  ///<a href="http://en.cppreference.com/w/cpp/algorithm/all_any_none_of"></a>
  //***************************************************************************
  template <typename TIterator,
            typename TUnaryPredicate>
  bool any_of(TIterator       begin,
              TIterator       end,
              TUnaryPredicate predicate)
  {
    return std::find_if(begin, end, predicate) != end;
  }

  //***************************************************************************
  /// none_of
  ///\ingroup algorithm
  ///<a href="http://en.cppreference.com/w/cpp/algorithm/all_any_none_of"></a>
  //***************************************************************************
  template <typename TIterator,
            typename TUnaryPredicate>
  bool none_of(TIterator       begin,
               TIterator       end,
               TUnaryPredicate predicate)
  {
    return std::find_if(begin, end, predicate) == end;
  }

  //***************************************************************************
  /// is_permutation
  ///\ingroup algorithm
  ///<a href="http://en.cppreference.com/w/cpp/algorithm/is_permutation"></a>
  //***************************************************************************
  template <typename TIterator1,
            typename TIterator2>
  bool is_permutation(TIterator1 begin1,
                      TIterator1 end1,
                      TIterator2 begin2)
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
  template <typename TIterator1,
            typename TIterator2>
  bool is_permutation(TIterator1 begin1,
                      TIterator1 end1,
                      TIterator2 begin2,
                      TIterator2 end2)
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
  template <typename TIterator1,
            typename TIterator2,
            typename TBinaryPredicate>
  bool is_permutation(TIterator1       begin1,
                      TIterator1       end1,
                      TIterator2       begin2,
                      TBinaryPredicate predicate)
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
  template <typename TIterator1,
            typename TIterator2,
            typename TBinaryPredicate>
  bool is_permutation(TIterator1       begin1,
                      TIterator1       end1,
                      TIterator2       begin2,
                      TIterator2       end2,
                      TBinaryPredicate predicate)
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
  template <typename TIterator,
            typename TUnaryPredicate>
  bool is_partitioned(TIterator       begin,
                      TIterator       end,
                      TUnaryPredicate predicate)
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
  template <typename TIterator,
            typename TUnaryPredicate>
  TIterator partition_point(TIterator       begin,
                            TIterator       end,
                            TUnaryPredicate predicate)
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
  template <typename TSource,
            typename TDestinationTrue,
            typename TDestinationFalse,
            typename TUnaryPredicate>
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

  //***************************************************************************
  /// Like std::for_each but applies a predicate before calling the function.
  ///\ingroup algorithm
  //***************************************************************************
  template <typename TIterator,
            typename TUnaryFunction,
            typename TUnaryPredicate>
  TUnaryFunction for_each_if(TIterator       begin,
                             const TIterator end,
                             TUnaryFunction  function,
                             TUnaryPredicate predicate)
  {
    while (begin != end)
    {
      if (predicate(*begin))
      {
        function(*begin);
      }

      ++begin;
    }

    return function;
  }

  //***************************************************************************
  /// A form of std::transform where the transform returns when the first range
  /// end is reached.
  /// There is currently no STL equivalent.
  ///\ingroup algorithm
  //***************************************************************************
  template <typename TInputIterator,
            typename TOutputIterator,
            typename TUnaryFunction>
  void transform(TInputIterator  i_begin,
                 TInputIterator  i_end,
                 TOutputIterator o_begin,
                 TOutputIterator o_end,
                 TUnaryFunction  function)
  {
    while ((i_begin != i_end) && (o_begin != o_end))
    {
      *o_begin++ = function(*i_begin++);
    }
  }

  //***************************************************************************
  /// Transform 'n' items.
  /// Random iterators.
  /// There is currently no STL equivalent.
  ///\ingroup algorithm
  //***************************************************************************
  template <typename TInputIterator,
            typename TSize,
            typename TOutputIterator,
            typename TUnaryFunction>
  typename etl::enable_if<etl::is_random_iterator<TInputIterator>::value, void>::type
   transform_n(TInputIterator  i_begin,
               TSize           n,
               TOutputIterator o_begin,
               TUnaryFunction  function)
  {
    std::transform(i_begin, i_begin + n, o_begin, function);
  }

  //***************************************************************************
  /// Transform 'n' items from two ranges.
  /// Random iterators.
  /// There is currently no STL equivalent.
  ///\ingroup algorithm
  //***************************************************************************
  template <typename TInputIterator1,
            typename TInputIterator2,
            typename TSize,
            typename TOutputIterator,
            typename TBinaryFunction>
  typename etl::enable_if<etl::is_random_iterator<TInputIterator1>::value &&
                          etl::is_random_iterator<TInputIterator2>::value, void>::type
   transform_n(TInputIterator1 i_begin1,
               TInputIterator2 i_begin2,
               TSize           n,
               TOutputIterator o_begin,
               TBinaryFunction function)
  {
    std::transform(i_begin1, i_begin1 + n, i_begin2, o_begin, function);
  }

  //***************************************************************************
  /// Transform 'n' items.
  /// Non-random iterators.
  /// There is currently no STL equivalent.
  ///\ingroup algorithm
  //***************************************************************************
  template <typename TInputIterator,
            typename TSize,
            typename TOutputIterator,
            typename TUnaryFunction>
  typename etl::enable_if<!etl::is_random_iterator<TInputIterator>::value, void>::type
   transform_n(TInputIterator  i_begin,
               TSize           n,
               TOutputIterator o_begin,
               TUnaryFunction  function)
  {
    while (n > 0)
    {
      *o_begin++ = function(*i_begin++);
      --n;
    }
  }

  //***************************************************************************
  /// Transform 'n' items from two ranges.
  /// Non-random iterators.
  /// There is currently no STL equivalent.
  ///\ingroup algorithm
  //***************************************************************************
  template <typename TInputIterator1,
            typename TInputIterator2,
            typename TSize,
            typename TOutputIterator,
            typename TBinaryFunction>
  typename etl::enable_if<!etl::is_random_iterator<TInputIterator1>::value ||
                          !etl::is_random_iterator<TInputIterator2>::value, void>::type
   transform_n(TInputIterator1 i_begin1,
               TInputIterator2 i_begin2,
               TSize           n,
               TOutputIterator o_begin,
               TBinaryFunction function)
  {
    while (n > 0)
    {
      *o_begin++ = function(*i_begin1++, *i_begin2++);
      --n;
    }
  }

  //***************************************************************************
  /// Like std::transform but applies a predicate before calling the function.
  ///\ingroup algorithm
  //***************************************************************************
  template <typename TInputIterator,
            typename TOutputIterator,
            typename TUnaryFunction,
            typename TUnaryPredicate>
  TOutputIterator transform_if(TInputIterator       i_begin,
                               const TInputIterator i_end,
                               TOutputIterator      o_begin,
                               TUnaryFunction       function,
                               TUnaryPredicate      predicate)
  {
    while (i_begin != i_end)
    {
      if (predicate(*i_begin))
      {
        *o_begin++ = function(*i_begin);
      }

      ++i_begin;
    }

    return o_begin;
  }

  //***************************************************************************
  /// Like etl::transform_if but inputs from two ranges.
  ///\ingroup algorithm
  //***************************************************************************
  template <typename TInputIterator1,
            typename TInputIterator2,
            typename TOutputIterator,
            typename TBinaryFunction,
            typename TBinaryPredicate>
  TOutputIterator transform_if(TInputIterator1       i_begin1,
                               const TInputIterator1 i_end1,
                               TInputIterator2       i_begin2,
                               TOutputIterator       o_begin,
                               TBinaryFunction       function,
                               TBinaryPredicate      predicate)
  {
    while (i_begin1 != i_end1)
    {
      if (predicate(*i_begin1, *i_begin2))
      {
        *o_begin++ = function(*i_begin1, *i_begin2);
      }

      ++i_begin1;
      ++i_begin2;
    }

    return o_begin;
  }

  //***************************************************************************
  /// Transforms the elements from the range (begin, end) to two different ranges
  /// depending on the value returned by the predicate.<br>
  ///\ingroup algorithm
  //***************************************************************************
  template <typename TSource, typename TDestinationTrue, typename TDestinationFalse,
            typename TUnaryFunctionTrue, typename TUnaryFunctionFalse,
            typename TUnaryPredicate>
  std::pair<TDestinationTrue, TDestinationFalse> partition_transform(TSource             begin,
                                                                     TSource             end,
                                                                     TDestinationTrue    destination_true,
                                                                     TDestinationFalse   destination_false,
                                                                     TUnaryFunctionTrue  function_true,
                                                                     TUnaryFunctionFalse function_false,
                                                                     TUnaryPredicate     predicate)
  {
    while (begin != end)
    {
      if (predicate(*begin))
      {
        *destination_true++ = function_true(*begin++);
      }
      else
      {
        *destination_false++ = function_false(*begin++);
      }
    }

    return std::pair<TDestinationTrue, TDestinationFalse>(destination_true, destination_false);
  }

  //***************************************************************************
  /// Transforms the elements from the ranges (begin1, end1) & (begin2)
  /// to two different ranges depending on the value returned by the predicate.
  ///\ingroup algorithm
  //***************************************************************************
  template <typename TSource1,
            typename TSource2,
            typename TDestinationTrue,
            typename TDestinationFalse,
            typename TBinaryFunctionTrue,
            typename TBinaryFunctionFalse,
            typename TBinaryPredicate>
  std::pair<TDestinationTrue, TDestinationFalse> partition_transform(TSource1             begin1,
                                                                     TSource1             end1,
                                                                     TSource2             begin2,
                                                                     TDestinationTrue     destination_true,
                                                                     TDestinationFalse    destination_false,
                                                                     TBinaryFunctionTrue  function_true,
                                                                     TBinaryFunctionFalse function_false,
                                                                     TBinaryPredicate     predicate)
  {
    while (begin1 != end1)
    {
      if (predicate(*begin1, *begin2))
      {
        *destination_true++ = function_true(*begin1++, *begin2++);
      }
      else
      {
        *destination_false++ = function_false(*begin1++, *begin2++);
      }
    }

    return std::pair<TDestinationTrue, TDestinationFalse>(destination_true, destination_false);
  }
}

#endif

