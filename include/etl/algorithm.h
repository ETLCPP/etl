///\file

/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl
https://www.etlcpp.com

Documentation: https://www.etlcpp.com/algorithm.html

Copyright(c) 2014 John Wellbelove

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

#ifndef ETL_ALGORITHM_INCLUDED
#define ETL_ALGORITHM_INCLUDED

///\defgroup algorithm algorithm
/// Including reverse engineered algorithms from C++11, 14, 17
/// Additional new variants of certain algorithms.
///\ingroup utilities

#include "platform.h"
#include "error_handler.h"
#include "exception.h"
#include "functional.h"
#include "gcd.h"
#include "initializer_list.h"
#include "invoke.h"
#include "iterator.h"
#include "largest.h"
#include "ranges.h"
#include "type_traits.h"
#include "utility.h"

#include <stddef.h>
#include <string.h>

#include "private/minmax_push.h"

#if ETL_USING_STL
  #include <algorithm>
  #include <functional>
  #include <iterator>
  #include <numeric>
  #include <utility>
#endif

namespace etl
{
  // Declare prototypes of the ETL's sort functions
  template <typename TIterator>
#if ETL_USING_STD_NAMESPACE
  ETL_CONSTEXPR20
#else
  ETL_CONSTEXPR14
#endif
    void
    shell_sort(TIterator first, TIterator last);

  template <typename TIterator, typename TCompare>
#if ETL_USING_STD_NAMESPACE
  ETL_CONSTEXPR20
#else
  ETL_CONSTEXPR14
#endif
    void
    shell_sort(TIterator first, TIterator last, TCompare compare);

  template <typename TIterator>
  ETL_CONSTEXPR14 void insertion_sort(TIterator first, TIterator last);

  template <typename TIterator, typename TCompare>
  ETL_CONSTEXPR14 void insertion_sort(TIterator first, TIterator last, TCompare compare);

} // namespace etl

//*****************************************************************************
// Algorithms defined by the ETL
//*****************************************************************************
namespace etl
{
  namespace private_algorithm
  {
    template <bool use_swap>
    struct swap_impl;

    // Generic swap
    template <>
    struct swap_impl<false>
    {
      template <typename TIterator1, typename TIterator2>
      static void do_swap(TIterator1 a, TIterator2 b)
      {
        typename etl::iterator_traits<TIterator1>::value_type tmp = *a;
        *a                                                        = *b;
        *b                                                        = tmp;
      }
    };

    // Specialised swap
    template <>
    struct swap_impl<true>
    {
      template <typename TIterator1, typename TIterator2>
      static void do_swap(TIterator1 a, TIterator2 b)
      {
        using ETL_OR_STD::swap; // Allow ADL
        swap(*a, *b);
      }
    };
  } // namespace private_algorithm

  //***************************************************************************
  // iter_swap
  //***************************************************************************
  template <typename TIterator1, typename TIterator2>
#if ETL_USING_STD_NAMESPACE
  ETL_CONSTEXPR20
#else
  ETL_CONSTEXPR14
#endif
    void
    iter_swap(TIterator1 a, TIterator2 b)
  {
    typedef etl::iterator_traits<TIterator1> traits1;
    typedef etl::iterator_traits<TIterator2> traits2;

    typedef typename traits1::value_type v1;
    typedef typename traits2::value_type v2;

    typedef typename traits1::reference r1;
    typedef typename traits2::reference r2;

    const bool use_swap = etl::is_same<v1, v2>::value && etl::is_reference<r1>::value && etl::is_reference<r2>::value;

    private_algorithm::swap_impl<use_swap>::do_swap(a, b);
  }

  //***************************************************************************
  // swap_ranges
  //***************************************************************************
  template <typename TIterator1, typename TIterator2>
#if ETL_USING_STD_NAMESPACE
  ETL_CONSTEXPR20
#else
  ETL_CONSTEXPR14
#endif
    TIterator2
    swap_ranges(TIterator1 first1, TIterator1 last1, TIterator2 first2)
  {
    while (first1 != last1)
    {
      etl::iter_swap(first1, first2);
      ++first1;
      ++first2;
    }

    return first2;
  }

  //***************************************************************************
  // generate
  template <typename TIterator, typename TFunction>
  ETL_CONSTEXPR14 void generate(TIterator db, TIterator de, TFunction funct)
  {
    while (db != de)
    {
      *db++ = funct();
    }
  }

  //***************************************************************************
  // copy
#if ETL_USING_STL && ETL_USING_CPP20
  // Use the STL constexpr implementation.
  template <typename TIterator1, typename TIterator2>
  constexpr TIterator2 copy(TIterator1 sb, TIterator1 se, TIterator2 db)
  {
    return std::copy(sb, se, db);
  }
#else
  // Non-pointer or not trivially copyable or not using builtin memcpy.
  template <typename TIterator1, typename TIterator2>
  ETL_CONSTEXPR14 TIterator2 copy(TIterator1 sb, TIterator1 se, TIterator2 db)
  {
    while (sb != se)
    {
      *db = *sb;
      ++db;
      ++sb;
    }

    return db;
  }
#endif

  //***************************************************************************
  // reverse_copy
#if ETL_USING_STL && ETL_USING_CPP20
  template <typename TIterator1, typename TIterator2>
  constexpr TIterator2 reverse_copy(TIterator1 sb, TIterator1 se, TIterator2 db)
  {
    return std::reverse_copy(sb, se, db);
  }
#else
  template <typename TIterator1, typename TIterator2>
  ETL_CONSTEXPR14 TIterator2 reverse_copy(TIterator1 sb, TIterator1 se, TIterator2 db)
  {
    while (sb != se)
    {
      *db = *--se;
      ++db;
    }

    return db;
  }
#endif

  //***************************************************************************
  // copy_n
#if ETL_USING_STL && ETL_USING_CPP20
  // Use the STL implementation
  template <typename TIterator1, typename TSize, typename TIterator2>
  constexpr TIterator2 copy_n(TIterator1 sb, TSize count, TIterator2 db)
  {
    return std::copy_n(sb, count, db);
  }
#else
  // Non-pointer or not trivially copyable or not using builtin memcpy.
  template <typename TIterator1, typename TSize, typename TIterator2>
  ETL_CONSTEXPR14 TIterator2 copy_n(TIterator1 sb, TSize count, TIterator2 db)
  {
    while (count != 0)
    {
      *db = *sb;
      ++db;
      ++sb;
      --count;
    }

    return db;
  }
#endif

  //***************************************************************************
  // copy_backward
#if ETL_USING_STL && ETL_USING_CPP20
  template <typename TIterator1, typename TIterator2>
  constexpr TIterator2 copy_backward(TIterator1 sb, TIterator1 se, TIterator2 de)
  {
    return std::copy_backward(sb, se, de);
  }
#else
  template <typename TIterator1, typename TIterator2>
  ETL_CONSTEXPR14 TIterator2 copy_backward(TIterator1 sb, TIterator1 se, TIterator2 de)
  {
    while (se != sb)
    {
      *(--de) = *(--se);
    }

    return de;
  }
#endif

  //***************************************************************************
  // move
#if ETL_USING_STL && ETL_USING_CPP20
  template <typename TIterator1, typename TIterator2>
  constexpr TIterator2 move(TIterator1 sb, TIterator1 se, TIterator2 db)
  {
    return std::move(sb, se, db);
  }
#elif ETL_USING_CPP11
  // For C++11
  template <typename TIterator1, typename TIterator2>
  ETL_CONSTEXPR14 TIterator2 move(TIterator1 sb, TIterator1 se, TIterator2 db)
  {
    while (sb != se)
    {
      *db = etl::move(*sb);
      ++db;
      ++sb;
    }

    return db;
  }
#else
  // For C++03
  template <typename TIterator1, typename TIterator2>
  ETL_CONSTEXPR14 TIterator2 move(TIterator1 sb, TIterator1 se, TIterator2 db)
  {
    return copy(sb, se, db);
  }
#endif

  //***************************************************************************
  // move_backward
#if ETL_USING_STL && ETL_USING_CPP20
  template <typename TIterator1, typename TIterator2>
  ETL_CONSTEXPR20 TIterator2 move_backward(TIterator1 sb, TIterator1 se, TIterator2 de)
  {
  #include "etl/private/diagnostic_array_bounds_push.h"
    return std::move_backward(sb, se, de);
  #include "etl/private/diagnostic_pop.h"
  }
#elif ETL_USING_CPP11
  // For C++11
  template <typename TIterator1, typename TIterator2>
  ETL_CONSTEXPR14 TIterator2 move_backward(TIterator1 sb, TIterator1 se, TIterator2 de)
  {
    while (sb != se)
    {
      *(--de) = etl::move(*(--se));
    }

    return de;
  }
#else
  // For C++03
  template <typename TIterator1, typename TIterator2>
  ETL_CONSTEXPR14 TIterator2 move_backward(TIterator1 sb, TIterator1 se, TIterator2 de)
  {
    return etl::copy_backward(sb, se, de);
  }
#endif

  //***************************************************************************
  // reverse
  //***************************************************************************
  // Pointers
  template <typename TIterator>
  ETL_CONSTEXPR14 typename etl::enable_if<etl::is_pointer<TIterator>::value, void>::type reverse(TIterator b, TIterator e)
  {
    if (b != e)
    {
      while (b < --e)
      {
        etl::iter_swap(b, e);
        ++b;
      }
    }
  }

  // Non-pointers
  template <typename TIterator>
  ETL_CONSTEXPR14 typename etl::enable_if<!etl::is_pointer<TIterator>::value, void>::type reverse(TIterator b, TIterator e)
  {
    while ((b != e) && (b != --e))
    {
      etl::iter_swap(b++, e);
    }
  }

  //***************************************************************************
  // lower_bound
  //***************************************************************************
  template <typename TIterator, typename TValue, typename TCompare>
  ETL_NODISCARD ETL_CONSTEXPR14 TIterator lower_bound(TIterator first, TIterator last, const TValue& value, TCompare compare)
  {
    typedef typename etl::iterator_traits<TIterator>::difference_type difference_t;

    difference_t count = etl::distance(first, last);

    while (count > 0)
    {
      TIterator    itr  = first;
      difference_t step = count / 2;

      etl::advance(itr, step);

      if (compare(*itr, value))
      {
        first = ++itr;
        count -= step + 1;
      }
      else
      {
        count = step;
      }
    }

    return first;
  }

  template <typename TIterator, typename TValue>
  ETL_NODISCARD ETL_CONSTEXPR14 TIterator lower_bound(TIterator first, TIterator last, const TValue& value)
  {
    typedef etl::less<typename etl::iterator_traits<TIterator>::value_type> compare;

    return etl::lower_bound(first, last, value, compare());
  }

  //***************************************************************************
  // upper_bound
  //***************************************************************************
  template <typename TIterator, typename TValue, typename TCompare>
  ETL_NODISCARD ETL_CONSTEXPR14 TIterator upper_bound(TIterator first, TIterator last, const TValue& value, TCompare compare)
  {
    typedef typename etl::iterator_traits<TIterator>::difference_type difference_t;

    difference_t count = etl::distance(first, last);

    while (count > 0)
    {
      TIterator    itr  = first;
      difference_t step = count / 2;

      etl::advance(itr, step);

      if (!compare(value, *itr))
      {
        first = ++itr;
        count -= step + 1;
      }
      else
      {
        count = step;
      }
    }

    return first;
  }

  template <typename TIterator, typename TValue>
  ETL_NODISCARD ETL_CONSTEXPR14 TIterator upper_bound(TIterator first, TIterator last, const TValue& value)
  {
    typedef etl::less<typename etl::iterator_traits<TIterator>::value_type> compare;

    return etl::upper_bound(first, last, value, compare());
  }

  //***************************************************************************
  // equal_range
  //***************************************************************************
  template <typename TIterator, typename TValue, typename TCompare>
  ETL_NODISCARD ETL_CONSTEXPR14 ETL_OR_STD::pair<TIterator, TIterator> equal_range(TIterator first, TIterator last, const TValue& value, TCompare compare)
  {
    return ETL_OR_STD::make_pair(etl::lower_bound(first, last, value, compare), etl::upper_bound(first, last, value, compare));
  }

  template <typename TIterator, typename TValue>
  ETL_NODISCARD
  ETL_OR_STD::pair<TIterator, TIterator> equal_range(TIterator first, TIterator last, const TValue& value)
  {
    typedef etl::less<typename etl::iterator_traits<TIterator>::value_type> compare;

    return ETL_OR_STD::make_pair(etl::lower_bound(first, last, value, compare()), etl::upper_bound(first, last, value, compare()));
  }

  //***************************************************************************
  // binary_search
  //***************************************************************************
  template <typename TIterator, typename T, typename Compare>
  ETL_NODISCARD ETL_CONSTEXPR14 bool binary_search(TIterator first, TIterator last, const T& value, Compare compare)
  {
    first = etl::lower_bound(first, last, value, compare);

    return (!(first == last) && !(compare(value, *first)));
  }

  template <typename TIterator, typename T>
  ETL_NODISCARD ETL_CONSTEXPR14 bool binary_search(TIterator first, TIterator last, const T& value)
  {
    typedef etl::less<typename etl::iterator_traits<TIterator>::value_type> compare;

    return binary_search(first, last, value, compare());
  }

  //***************************************************************************
  // find_if
  //***************************************************************************
  template <typename TIterator, typename TUnaryPredicate>
  ETL_NODISCARD ETL_CONSTEXPR14 TIterator find_if(TIterator first, TIterator last, TUnaryPredicate predicate)
  {
    while (first != last)
    {
      if (predicate(*first))
      {
        return first;
      }

      ++first;
    }

    return last;
  }

  //***************************************************************************
  // find
  //***************************************************************************
  template <typename TIterator, typename T>
  ETL_NODISCARD ETL_CONSTEXPR14 TIterator find(TIterator first, TIterator last, const T& value)
  {
    while (first != last)
    {
      if (*first == value)
      {
        return first;
      }

      ++first;
    }

    return last;
  }

  //***************************************************************************
  // fill
#if ETL_USING_STL && ETL_USING_CPP20
  template <typename TIterator, typename TValue>
  constexpr void fill(TIterator first, TIterator last, const TValue& value)
  {
    std::fill(first, last, value);
  }
#else
  template <typename TIterator, typename TValue>
  ETL_CONSTEXPR14 void fill(TIterator first, TIterator last, const TValue& value)
  {
    while (first != last)
    {
      // This cast is necessary because the signedness can differ
      *first = static_cast<typename etl::iterator_traits<TIterator>::value_type>(value);
      ++first;
    }
  }
#endif

  //***************************************************************************
  // fill_n
#if ETL_USING_STL && ETL_USING_CPP20
  template <typename TIterator, typename TSize, typename TValue>
  constexpr TIterator fill_n(TIterator first, TSize count, const TValue& value)
  {
    return std::fill_n(first, count, value);
  }
#else
  template <typename TIterator, typename TSize, typename TValue>
  ETL_CONSTEXPR14 TIterator fill_n(TIterator first, TSize count, const TValue& value)
  {
    while (count != 0)
    {
      *first++ = value;
      --count;
    }

    return first;
  }
#endif

  //***************************************************************************
  // count
  //***************************************************************************
  template <typename TIterator, typename T>
  ETL_NODISCARD ETL_CONSTEXPR14 typename etl::iterator_traits<TIterator>::difference_type count(TIterator first, TIterator last, const T& value)
  {
    typename iterator_traits<TIterator>::difference_type n = 0;

    while (first != last)
    {
      if (*first == value)
      {
        ++n;
      }

      ++first;
    }

    return n;
  }

  //***************************************************************************
  // count_if
  //***************************************************************************
  template <typename TIterator, typename TUnaryPredicate>
  ETL_NODISCARD ETL_CONSTEXPR14 typename etl::iterator_traits<TIterator>::difference_type count_if(TIterator first, TIterator last, TUnaryPredicate predicate)
  {
    typename iterator_traits<TIterator>::difference_type n = 0;

    while (first != last)
    {
      if (predicate(*first))
      {
        ++n;
      }

      ++first;
    }

    return n;
  }

  //***************************************************************************
  // equal
#if ETL_USING_STL && ETL_USING_CPP20
  // Three parameter
  template <typename TIterator1, typename TIterator2>
  [[nodiscard]]
  constexpr bool equal(TIterator1 first1, TIterator1 last1, TIterator2 first2)
  {
    return std::equal(first1, last1, first2);
  }

  // Three parameter + predicate
  template <typename TIterator1, typename TIterator2, typename TPredicate>
  [[nodiscard]]
  constexpr bool equal(TIterator1 first1, TIterator1 last1, TIterator2 first2, TPredicate predicate)
  {
    return std::equal(first1, last1, first2, predicate);
  }

  // Four parameter
  template <typename TIterator1, typename TIterator2>
  [[nodiscard]]
  constexpr bool equal(TIterator1 first1, TIterator1 last1, TIterator2 first2, TIterator2 last2)
  {
    return std::equal(first1, last1, first2, last2);
  }

  // Four parameter + Predicate
  template <typename TIterator1, typename TIterator2, typename TPredicate>
  [[nodiscard]]
  constexpr bool equal(TIterator1 first1, TIterator1 last1, TIterator2 first2, TIterator2 last2, TPredicate predicate)
  {
    return std::equal(first1, last1, first2, last2, predicate);
  }

#else

  template <typename TIterator1, typename TIterator2>
  ETL_NODISCARD ETL_CONSTEXPR14 bool equal(TIterator1 first1, TIterator1 last1, TIterator2 first2)
  {
    while (first1 != last1)
    {
      if (*first1 != *first2)
      {
        return false;
      }

      ++first1;
      ++first2;
    }

    return true;
  }

  // Predicate
  template <typename TIterator1, typename TIterator2, typename TPredicate>
  ETL_NODISCARD ETL_CONSTEXPR14 bool equal(TIterator1 first1, TIterator1 last1, TIterator2 first2, TPredicate predicate)
  {
    while (first1 != last1)
    {
      if (!predicate(*first1, *first2))
      {
        return false;
      }

      ++first1;
      ++first2;
    }

    return true;
  }

  // Four parameter
  template <typename TIterator1, typename TIterator2>
  ETL_NODISCARD ETL_CONSTEXPR14 bool equal(TIterator1 first1, TIterator1 last1, TIterator2 first2, TIterator2 last2)
  {
    while ((first1 != last1) && (first2 != last2))
    {
      if (*first1 != *first2)
      {
        return false;
      }

      ++first1;
      ++first2;
    }

    return (first1 == last1) && (first2 == last2);
  }

  // Four parameter, Predicate
  template <typename TIterator1, typename TIterator2, typename TPredicate>
  ETL_NODISCARD ETL_CONSTEXPR14 bool equal(TIterator1 first1, TIterator1 last1, TIterator2 first2, TIterator2 last2, TPredicate predicate)
  {
    while ((first1 != last1) && (first2 != last2))
    {
      if (!predicate(*first1, *first2))
      {
        return false;
      }

      ++first1;
      ++first2;
    }

    return (first1 == last1) && (first2 == last2);
  }
#endif

  //***************************************************************************
  // lexicographical_compare
  //***************************************************************************
  template <typename TIterator1, typename TIterator2, typename TCompare>
  ETL_NODISCARD ETL_CONSTEXPR14 bool lexicographical_compare(TIterator1 first1, TIterator1 last1, TIterator2 first2, TIterator2 last2, TCompare compare)
  {
    while ((first1 != last1) && (first2 != last2))
    {
      if (compare(*first1, *first2))
      {
        return true;
      }

      if (compare(*first2, *first1))
      {
        return false;
      }

      ++first1;
      ++first2;
    }

    return (first1 == last1) && (first2 != last2);
  }

  // lexicographical_compare
  template <typename TIterator1, typename TIterator2>
  ETL_NODISCARD ETL_CONSTEXPR14 bool lexicographical_compare(TIterator1 first1, TIterator1 last1, TIterator2 first2, TIterator2 last2)
  {
    typedef etl::less<typename etl::iterator_traits<TIterator1>::value_type> compare;

    return etl::lexicographical_compare(first1, last1, first2, last2, compare());
  }

  //***************************************************************************
  // min
  //***************************************************************************
  template <typename T, typename TCompare>
  ETL_NODISCARD ETL_CONSTEXPR const T& min(const T& a, const T& b, TCompare compare)
  {
    return (compare(a, b)) ? a : b;
  }

  template <typename T>
  ETL_NODISCARD ETL_CONSTEXPR const T& min(const T& a, const T& b)
  {
    typedef etl::less<T> compare;

    return etl::min(a, b, compare());
  }

  //***************************************************************************
  // max
  //***************************************************************************
  template <typename T, typename TCompare>
  ETL_NODISCARD ETL_CONSTEXPR const T& max(const T& a, const T& b, TCompare compare)
  {
    return (compare(a, b)) ? b : a;
  }

  template <typename T>
  ETL_NODISCARD ETL_CONSTEXPR const T& max(const T& a, const T& b)
  {
    typedef etl::less<T> compare;

    return etl::max(a, b, compare());
  }

  //***************************************************************************
  // for_each
  //***************************************************************************
  template <typename TIterator, typename TUnaryOperation>
  ETL_CONSTEXPR14 TUnaryOperation for_each(TIterator first, TIterator last, TUnaryOperation unary_operation)
  {
    while (first != last)
    {
      unary_operation(*first);
      ++first;
    }

    return unary_operation;
  }

  //***************************************************************************
  // transform
  //***************************************************************************
  template <typename TIteratorIn, typename TIteratorOut, typename TUnaryOperation>
  ETL_CONSTEXPR14 TIteratorOut transform(TIteratorIn first1, TIteratorIn last1, TIteratorOut d_first, TUnaryOperation unary_operation)
  {
    while (first1 != last1)
    {
      *d_first = unary_operation(*first1);

      ++d_first;
      ++first1;
    }

    return d_first;
  }

  template <typename TIteratorIn1, typename TIteratorIn2, typename TIteratorOut, typename TBinaryOperation>
  ETL_CONSTEXPR14 TIteratorOut transform(TIteratorIn1 first1, TIteratorIn1 last1, TIteratorIn2 first2, TIteratorOut d_first,
                                   TBinaryOperation binary_operation)
  {
    while (first1 != last1)
    {
      *d_first = binary_operation(*first1, *first2);

      ++d_first;
      ++first1;
      ++first2;
    }

    return d_first;
  }

  //***************************************************************************
  // replace
  //***************************************************************************
  template <typename TIterator, typename T>
  ETL_CONSTEXPR14 void replace(TIterator first, TIterator last, const T& old_value, const T& new_value)
  {
    while (first != last)
    {
      if (*first == old_value)
      {
        *first = new_value;
      }

      ++first;
    }
  }

  //***************************************************************************
  // replace_if
  //***************************************************************************
  template <typename TIterator, typename TPredicate, typename T>
  ETL_CONSTEXPR14 void replace_if(TIterator first, TIterator last, TPredicate predicate, const T& new_value)
  {
    while (first != last)
    {
      if (predicate(*first))
      {
        *first = new_value;
      }

      ++first;
    }
  }

  //***************************************************************************
  // Heap
  //***************************************************************************
  namespace private_heap
  {
    // Push Heap Helper
    template <typename TIterator, typename TDistance, typename TValue, typename TCompare>
    ETL_CONSTEXPR14 void push_heap(TIterator first, TDistance value_index, TDistance top_index, TValue value, TCompare compare)
    {
      TDistance parent = (value_index - 1) / 2;

#include "etl/private/diagnostic_array_bounds_push.h"
      while ((value_index > top_index) && compare(first[parent], value))
      {
        first[value_index] = ETL_MOVE(first[parent]);
        value_index        = parent;
        parent             = (value_index - 1) / 2;
      }

      first[value_index] = ETL_MOVE(value);
#include "etl/private/diagnostic_pop.h"
    }

    // Adjust Heap Helper
    template <typename TIterator, typename TDistance, typename TValue, typename TCompare>
    ETL_CONSTEXPR14 void adjust_heap(TIterator first, TDistance value_index, TDistance length, TValue value, TCompare compare)
    {
      TDistance top_index = value_index;
      TDistance child2nd  = (2 * value_index) + 2;

      while (child2nd < length)
      {
        if (compare(first[child2nd], first[child2nd - 1]))
        {
          --child2nd;
        }

        first[value_index] = ETL_MOVE(first[child2nd]);
        value_index        = child2nd;
        child2nd           = 2 * (child2nd + 1);
      }

      if (child2nd == length)
      {
        first[value_index] = ETL_MOVE(first[child2nd - 1]);
        value_index        = child2nd - 1;
      }

      push_heap(first, value_index, top_index, ETL_MOVE(value), compare);
    }

    // Is Heap Helper
    template <typename TIterator, typename TDistance, typename TCompare>
    ETL_CONSTEXPR14 bool is_heap(const TIterator first, const TDistance n, TCompare compare)
    {
      TDistance parent = 0;

      for (TDistance child = 1; child < n; ++child)
      {
        if (compare(first[parent], first[child]))
        {
          return false;
        }

        if ((child & 1) == 0)
        {
          ++parent;
        }
      }

      return true;
    }
  } // namespace private_heap

  // Pop Heap
  template <typename TIterator, typename TCompare>
  ETL_CONSTEXPR14 void pop_heap(TIterator first, TIterator last, TCompare compare)
  {
    typedef typename etl::iterator_traits<TIterator>::value_type      value_t;
    typedef typename etl::iterator_traits<TIterator>::difference_type distance_t;

    value_t value = ETL_MOVE(last[-1]);
    last[-1]      = ETL_MOVE(first[0]);

    private_heap::adjust_heap(first, distance_t(0), distance_t(last - first - 1), ETL_MOVE(value), compare);
  }

  // Pop Heap
  template <typename TIterator>
  ETL_CONSTEXPR14 void pop_heap(TIterator first, TIterator last)
  {
    typedef etl::less<typename etl::iterator_traits<TIterator>::value_type> compare;

    etl::pop_heap(first, last, compare());
  }

  // Push Heap
  template <typename TIterator, typename TCompare>
  ETL_CONSTEXPR14 void push_heap(TIterator first, TIterator last, TCompare compare)
  {
    typedef typename etl::iterator_traits<TIterator>::difference_type difference_t;
    typedef typename etl::iterator_traits<TIterator>::value_type      value_t;

    private_heap::push_heap(first, difference_t(last - first - 1), difference_t(0), value_t(ETL_MOVE(*(last - 1))), compare);
  }

  // Push Heap
  template <typename TIterator>
  ETL_CONSTEXPR14 void push_heap(TIterator first, TIterator last)
  {
    typedef etl::less<typename etl::iterator_traits<TIterator>::value_type> compare;

    etl::push_heap(first, last, compare());
  }

  // Make Heap
  template <typename TIterator, typename TCompare>
  ETL_CONSTEXPR14 void make_heap(TIterator first, TIterator last, TCompare compare)
  {
    typedef typename etl::iterator_traits<TIterator>::difference_type difference_t;

    if ((last - first) < 2)
    {
      return;
    }

    difference_t length = last - first;
    difference_t parent = (length - 2) / 2;

    while (true)
    {
      private_heap::adjust_heap(first, parent, length, ETL_MOVE(*(first + parent)), compare);

      if (parent == 0)
      {
        return;
      }

      --parent;
    }
  }

  // Make Heap
  template <typename TIterator>
  ETL_CONSTEXPR14 void make_heap(TIterator first, TIterator last)
  {
    typedef etl::less<typename etl::iterator_traits<TIterator>::value_type> compare;

    etl::make_heap(first, last, compare());
  }

  // Is Heap
  template <typename TIterator>
  ETL_NODISCARD ETL_CONSTEXPR14 bool is_heap(TIterator first, TIterator last)
  {
    typedef etl::less<typename etl::iterator_traits<TIterator>::value_type> compare;

    return private_heap::is_heap(first, last - first, compare());
  }

  // Is Heap
  template <typename TIterator, typename TCompare>
  ETL_NODISCARD ETL_CONSTEXPR14 bool is_heap(TIterator first, TIterator last, TCompare compare)
  {
    return private_heap::is_heap(first, last - first, compare);
  }

  // Sort Heap
  template <typename TIterator>
  ETL_CONSTEXPR14 void sort_heap(TIterator first, TIterator last)
  {
    while (first != last)
    {
      etl::pop_heap(first, last);
      --last;
    }
  }

  // Sort Heap
  template <typename TIterator, typename TCompare>
  ETL_CONSTEXPR14 void sort_heap(TIterator first, TIterator last, TCompare compare)
  {
    while (first != last)
    {
      etl::pop_heap(first, last, compare);
      --last;
    }
  }

  //***************************************************************************
  /// partial_sort
  ///\ingroup algorithm
  ///< a href="http://en.cppreference.com/w/cpp/algorithm/partial_sort"></a>
  //***************************************************************************
  template <typename TIterator, typename TCompare>
  ETL_CONSTEXPR14 void partial_sort(TIterator first, TIterator middle, TIterator last, TCompare compare)
  {
    if (first == middle)
    {
      return;
    }

    typedef typename etl::iterator_traits<TIterator>::value_type      value_t;
    typedef typename etl::iterator_traits<TIterator>::difference_type difference_t;

    etl::make_heap(first, middle, compare);

    for (TIterator i = middle; i != last; ++i)
    {
      if (compare(*i, *first))
      {
        value_t value = ETL_MOVE(*i);
        *i            = ETL_MOVE(*first);

        private_heap::adjust_heap(first, difference_t(0), difference_t(middle - first), ETL_MOVE(value), compare);
      }
    }

    etl::sort_heap(first, middle, compare);
  }

  //***************************************************************************
  /// partial_sort
  ///\ingroup algorithm
  ///< a href="http://en.cppreference.com/w/cpp/algorithm/partial_sort"></a>
  //***************************************************************************
  template <typename TIterator>
  ETL_CONSTEXPR14 void partial_sort(TIterator first, TIterator middle, TIterator last)
  {
    typedef etl::less<typename etl::iterator_traits<TIterator>::value_type> compare;

    etl::partial_sort(first, middle, last, compare());
  }

  //***************************************************************************
  /// partial_sort_copy
  ///\ingroup algorithm
  ///< a
  ///< href="http://en.cppreference.com/w/cpp/algorithm/partial_sort_copy"></a>
  //***************************************************************************
  template <typename TInputIterator, typename TRandomAccessIterator, typename TCompare>
  ETL_CONSTEXPR14 TRandomAccessIterator partial_sort_copy(TInputIterator first, TInputIterator last, TRandomAccessIterator d_first,
                                                    TRandomAccessIterator d_last, TCompare compare)
  {
    typedef typename etl::iterator_traits<TRandomAccessIterator>::value_type      value_t;
    typedef typename etl::iterator_traits<TRandomAccessIterator>::difference_type difference_t;

    TRandomAccessIterator result = d_first;

    // Fill the destination range
    while ((first != last) && (result != d_last))
    {
      *result = *first;
      ++result;
      ++first;
    }

    if (result == d_first)
    {
      return result;
    }

    // Build a max-heap over the destination range
    etl::make_heap(d_first, result, compare);

    // Process remaining input elements
    for (TInputIterator i = first; i != last; ++i)
    {
      if (compare(*i, *d_first))
      {
        value_t value = *i;
        private_heap::adjust_heap(d_first, difference_t(0), difference_t(result - d_first), ETL_MOVE(value), compare);
      }
    }

    etl::sort_heap(d_first, result, compare);

    return result;
  }

  //***************************************************************************
  /// partial_sort_copy
  ///\ingroup algorithm
  ///< a
  ///< href="http://en.cppreference.com/w/cpp/algorithm/partial_sort_copy"></a>
  //***************************************************************************
  template <typename TInputIterator, typename TRandomAccessIterator>
  ETL_CONSTEXPR14 TRandomAccessIterator partial_sort_copy(TInputIterator first, TInputIterator last, TRandomAccessIterator d_first,
                                                    TRandomAccessIterator d_last)
  {
    typedef etl::less< typename etl::iterator_traits<TRandomAccessIterator>::value_type> compare;

    return etl::partial_sort_copy(first, last, d_first, d_last, compare());
  }

  //***************************************************************************
  // Search
  //***************************************************************************
  template <typename TIterator1, typename TIterator2, typename TCompare>
  ETL_NODISCARD ETL_CONSTEXPR14 TIterator1 search(TIterator1 first, TIterator1 last, TIterator2 search_first, TIterator2 search_last, TCompare compare)
  {
    while (true)
    {
      TIterator1 itr        = first;
      TIterator2 search_itr = search_first;

      while (true)
      {
        if (search_itr == search_last)
        {
          return first;
        }

        if (itr == last)
        {
          return last;
        }

        if (!compare(*itr, *search_itr))
        {
          break;
        }

        ++itr;
        ++search_itr;
      }

      ++first;
    }
  }

  // Search
  template <typename TIterator1, typename TIterator2>
  ETL_NODISCARD ETL_CONSTEXPR14 TIterator1 search(TIterator1 first, TIterator1 last, TIterator2 search_first, TIterator2 search_last)
  {
    typedef etl::equal_to<typename etl::iterator_traits<TIterator1>::value_type> compare;

    return etl::search(first, last, search_first, search_last, compare());
  }

  //***************************************************************************
  // Rotate
  //***************************************************************************
  namespace private_algorithm
  {
    //*********************************
    // For random access iterators
    template <typename TIterator>
    ETL_CONSTEXPR14 typename etl::enable_if<etl::is_random_access_iterator<TIterator>::value, TIterator>::type
      rotate_general(TIterator first, TIterator middle, TIterator last)
    {
      if (first == middle)
      {
        return last;
      }

      if (middle == last)
      {
        return first;
      }

      typedef typename etl::iterator_traits<TIterator>::value_type      value_type;
      typedef typename etl::iterator_traits<TIterator>::difference_type difference_type;

      difference_type n      = last - first;
      difference_type m      = middle - first;
      difference_type gcd_nm = (n == 0 || m == 0) ? n + m : etl::gcd(n, m);
      TIterator       result = first + (last - middle);

      for (difference_type i = 0; i < gcd_nm; i++)
      {
        value_type      temp = ETL_MOVE(*(first + i));
        difference_type j    = i;

        while (true)
        {
          difference_type k = j + m;

          if (k >= n)
          {
            k = k - n;
          }

          if (k == i)
          {
            break;
          }

          *(first + j) = ETL_MOVE(*(first + k));
          j            = k;
        }

        *(first + j) = ETL_MOVE(temp);
      }

      return result;
    }

    //*********************************
    // For bidirectional iterators
    template <typename TIterator>
    ETL_CONSTEXPR14 typename etl::enable_if<etl::is_bidirectional_iterator<TIterator>::value, TIterator>::type
      rotate_general(TIterator first, TIterator middle, TIterator last)
    {
      if (first == middle)
      {
        return last;
      }

      if (middle == last)
      {
        return first;
      }

      TIterator result = first;
      etl::advance(result, etl::distance(middle, last));

      reverse(first, middle);
      reverse(middle, last);
      reverse(first, last);

      return result;
    }

    //*********************************
    // For forward iterators
    template <typename TIterator>
    ETL_CONSTEXPR14 typename etl::enable_if<etl::is_forward_iterator<TIterator>::value, TIterator>::type rotate_general(TIterator first, TIterator middle,
                                                                                                                  TIterator last)
    {
      if (first == middle)
      {
        return last;
      }

      if (middle == last)
      {
        return first;
      }

      TIterator next   = middle;
      TIterator result = first;
      etl::advance(result, etl::distance(middle, last));

      while (first != next)
      {
        using ETL_OR_STD::swap;
        swap(*first++, *next++);

        if (next == last)
        {
          next = middle;
        }
        else if (first == middle)
        {
          middle = next;
        }
      }

      return result;
    }

    //*********************************
    // Simplified algorithm for rotate left by one
    template <typename TIterator>
    ETL_CONSTEXPR14 TIterator rotate_left_by_one(TIterator first, TIterator last)
    {
      typedef typename etl::iterator_traits<TIterator>::value_type value_type;

      // Save the first item.
      value_type temp(ETL_MOVE(*first));

      // Move the rest.
#include "etl/private/diagnostic_stringop_overread_push.h"
      TIterator result = etl::move(etl::next(first), last, first);
#include "etl/private/diagnostic_pop.h"

      // Restore the first item in its rotated position.
      *result = ETL_MOVE(temp);

      // The new position of the first item.
      return result;
    }

    //*********************************
    // Simplified for algorithm rotate right by one
    template <typename TIterator>
    ETL_CONSTEXPR14 TIterator rotate_right_by_one(TIterator first, TIterator last)
    {
      typedef typename etl::iterator_traits<TIterator>::value_type value_type;

      // Save the last item.
      TIterator  previous = etl::prev(last);
      value_type temp(ETL_MOVE(*previous));

      // Move the rest.
      TIterator result = etl::move_backward(first, previous, last);

      // Restore the last item in its rotated position.
      *first = ETL_MOVE(temp);

      // The new position of the first item.
      return result;
    }
  } // namespace private_algorithm

  //*********************************
  template <typename TIterator>
  ETL_CONSTEXPR14 TIterator rotate(TIterator first, TIterator middle, TIterator last)
  {
    if (first == middle)
    {
      return last;
    }
    if (middle == last)
    {
      return first;
    }

    if (etl::next(first) == middle)
    {
      return private_algorithm::rotate_left_by_one(first, last);
    }

#if ETL_USING_CPP20
    if (etl::next(middle) == last)
    {
      if ETL_IF_CONSTEXPR (etl::is_bidirectional_iterator_concept< TIterator>::value)
      {
        return private_algorithm::rotate_right_by_one(first, last);
      }
    }
#endif

    return private_algorithm::rotate_general(first, middle, last);
  }

  //***************************************************************************
  // find_end
  //***************************************************************************
  // Predicate
  template <typename TIterator1, typename TIterator2, typename TPredicate>
  ETL_NODISCARD ETL_CONSTEXPR14 TIterator1 find_end(TIterator1 b, TIterator1 e, TIterator2 sb, TIterator2 se, TPredicate predicate)
  {
    if (sb == se)
    {
      return e;
    }

    TIterator1 result = e;

    while (true)
    {
      TIterator1 new_result = etl::search(b, e, sb, se, predicate);

      if (new_result == e)
      {
        break;
      }
      else
      {
        result = new_result;
        b      = result;
        ++b;
      }
    }
    return result;
  }

  // Default
  template <typename TIterator1, typename TIterator2>
  ETL_NODISCARD ETL_CONSTEXPR14 TIterator1 find_end(TIterator1 b, TIterator1 e, TIterator2 sb, TIterator2 se)
  {
    typedef etl::equal_to<typename etl::iterator_traits<TIterator1>::value_type> predicate;

    return find_end(b, e, sb, se, predicate());
  }

  //***************************************************************************
  /// Finds the iterator to the smallest element in the range (begin, end).<br>
  ///< a href="http://en.cppreference.com/w/cpp/algorithm/min_element"></a>
  ///\ingroup algorithm
  //***************************************************************************
  template <typename TIterator, typename TCompare>
  ETL_NODISCARD ETL_CONSTEXPR14 TIterator min_element(TIterator begin, TIterator end, TCompare compare)
  {
    TIterator minimum = begin;

    if (begin != end)
    {
      ++begin;

      while (begin != end)
      {
        if (compare(*begin, *minimum))
        {
          minimum = begin;
        }

        ++begin;
      }
    }

    return minimum;
  }

  //***************************************************************************
  /// min_element
  ///\ingroup algorithm
  ///< a href="http://en.cppreference.com/w/cpp/algorithm/min_element"></a>
  //***************************************************************************
  template <typename TIterator>
  ETL_NODISCARD ETL_CONSTEXPR14 TIterator min_element(TIterator begin, TIterator end)
  {
    typedef typename etl::iterator_traits<TIterator>::value_type value_t;

    return etl::min_element(begin, end, etl::less<value_t>());
  }

  //***************************************************************************
  /// Finds the iterator to the largest element in the range (begin, end).<br>
  ///< a href="http://en.cppreference.com/w/cpp/algorithm/max_element"></a>
  ///\ingroup algorithm
  //***************************************************************************
  template <typename TIterator, typename TCompare>
  ETL_NODISCARD ETL_CONSTEXPR14 TIterator max_element(TIterator begin, TIterator end, TCompare compare)
  {
    TIterator maximum = begin;

    if (begin != end)
    {
      ++begin;

      while (begin != end)
      {
        if (compare(*maximum, *begin))
        {
          maximum = begin;
        }

        ++begin;
      }
    }

    return maximum;
  }

  //***************************************************************************
  /// max_element
  ///\ingroup algorithm
  ///< a href="http://en.cppreference.com/w/cpp/algorithm/max_element"></a>
  //***************************************************************************
  template <typename TIterator>
  ETL_NODISCARD ETL_CONSTEXPR14 TIterator max_element(TIterator begin, TIterator end)
  {
    typedef typename etl::iterator_traits<TIterator>::value_type value_t;

    return etl::max_element(begin, end, etl::less<value_t>());
  }

  //***************************************************************************
  /// Finds the greatest and the smallest element in the range (begin, end).<br>
  ///< a href="http://en.cppreference.com/w/cpp/algorithm/minmax_element"></a>
  ///\ingroup algorithm
  //***************************************************************************
  template <typename TIterator, typename TCompare>
  ETL_NODISCARD ETL_CONSTEXPR14 ETL_OR_STD::pair<TIterator, TIterator> minmax_element(TIterator begin, TIterator end, TCompare compare)
  {
    TIterator minimum = begin;
    TIterator maximum = begin;

    if (begin != end)
    {
      ++begin;

      while (begin != end)
      {
        if (compare(*begin, *minimum))
        {
          minimum = begin;
        }

        if (!compare(*begin, *maximum))
        {
          maximum = begin;
        }

        ++begin;
      }
    }

    return ETL_OR_STD::pair<TIterator, TIterator>(minimum, maximum);
  }

  //***************************************************************************
  /// minmax_element
  ///\ingroup algorithm
  ///< a href="http://en.cppreference.com/w/cpp/algorithm/minmax_element"></a>
  //***************************************************************************
  template <typename TIterator>
  ETL_NODISCARD ETL_CONSTEXPR14 ETL_OR_STD::pair<TIterator, TIterator> minmax_element(TIterator begin, TIterator end)
  {
    typedef typename etl::iterator_traits<TIterator>::value_type value_t;

    return etl::minmax_element(begin, end, etl::less<value_t>());
  }

  //***************************************************************************
  /// minmax
  ///\ingroup algorithm
  ///< a href="http://en.cppreference.com/w/cpp/algorithm/minmax"></a>
  //***************************************************************************
  template <typename T>
  ETL_NODISCARD ETL_CONSTEXPR14 ETL_OR_STD::pair<const T&, const T&> minmax(const T& a, const T& b)
  {
    return (b < a) ? ETL_OR_STD::pair<const T&, const T&>(b, a) : ETL_OR_STD::pair<const T&, const T&>(a, b);
  }

  //***************************************************************************
  /// minmax
  ///\ingroup algorithm
  ///< a href="http://en.cppreference.com/w/cpp/algorithm/minmax"></a>
  //***************************************************************************
  template <typename T, typename TCompare>
  ETL_NODISCARD ETL_CONSTEXPR14 ETL_OR_STD::pair<const T&, const T&> minmax(const T& a, const T& b, TCompare compare)
  {
    return compare(b, a) ? ETL_OR_STD::pair<const T&, const T&>(b, a) : ETL_OR_STD::pair<const T&, const T&>(a, b);
  }

  //***************************************************************************
  /// is_sorted_until
  ///\ingroup algorithm
  ///< a href="http://en.cppreference.com/w/cpp/algorithm/is_sorted_until"></a>
  //***************************************************************************
  template <typename TIterator, typename TCompare>
  ETL_NODISCARD ETL_CONSTEXPR14 TIterator is_sorted_until(TIterator begin, TIterator end, TCompare compare)
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
  /// is_sorted_until
  ///\ingroup algorithm
  ///< a href="http://en.cppreference.com/w/cpp/algorithm/is_sorted_until"></a>
  //***************************************************************************
  template <typename TIterator>
  ETL_NODISCARD ETL_CONSTEXPR14 TIterator is_sorted_until(TIterator begin, TIterator end)
  {
    typedef etl::less<typename etl::iterator_traits<TIterator>::value_type> compare;

    return etl::is_sorted_until(begin, end, compare());
  }

  //***************************************************************************
  /// is_sorted
  ///\ingroup algorithm
  ///< a href="http://en.cppreference.com/w/cpp/algorithm/is_sorted"></a>
  //***************************************************************************
  template <typename TIterator>
  ETL_NODISCARD ETL_CONSTEXPR14 bool is_sorted(TIterator begin, TIterator end)
  {
    return etl::is_sorted_until(begin, end) == end;
  }

  //***************************************************************************
  /// is_sorted
  ///\ingroup algorithm
  ///< a href="http://en.cppreference.com/w/cpp/algorithm/is_sorted"></a>
  //***************************************************************************
  template <typename TIterator, typename TCompare>
  ETL_NODISCARD ETL_CONSTEXPR14 bool is_sorted(TIterator begin, TIterator end, TCompare compare)
  {
    return etl::is_sorted_until(begin, end, compare) == end;
  }

  //***************************************************************************
  /// is_unique_sorted_until
  ///\ingroup algorithm
  //***************************************************************************
  template <typename TIterator, typename TCompare>
  ETL_NODISCARD ETL_CONSTEXPR14 TIterator is_unique_sorted_until(TIterator begin, TIterator end, TCompare compare)
  {
    if (begin != end)
    {
      TIterator next = begin;

      while (++next != end)
      {
        if (!compare(*begin, *next))
        {
          return next;
        }

        ++begin;
      }
    }

    return end;
  }

  //***************************************************************************
  /// is_unique_sorted_until
  ///\ingroup algorithm
  //***************************************************************************
  template <typename TIterator>
  ETL_NODISCARD ETL_CONSTEXPR14 TIterator is_unique_sorted_until(TIterator begin, TIterator end)
  {
    typedef etl::less<typename etl::iterator_traits<TIterator>::value_type> compare;

    return etl::is_unique_sorted_until(begin, end, compare());
  }

  //***************************************************************************
  /// is_unique_sorted
  ///\ingroup algorithm
  //***************************************************************************
  template <typename TIterator>
  ETL_NODISCARD ETL_CONSTEXPR14 bool is_unique_sorted(TIterator begin, TIterator end)
  {
    return etl::is_unique_sorted_until(begin, end) == end;
  }

  //***************************************************************************
  /// is_unique_sorted
  ///\ingroup algorithm
  //***************************************************************************
  template <typename TIterator, typename TCompare>
  ETL_NODISCARD ETL_CONSTEXPR14 bool is_unique_sorted(TIterator begin, TIterator end, TCompare compare)
  {
    return etl::is_unique_sorted_until(begin, end, compare) == end;
  }

  //***************************************************************************
  /// find_if_not
  ///\ingroup algorithm
  ///< a href="http://en.cppreference.com/w/cpp/algorithm/find"></a>
  //***************************************************************************
  template <typename TIterator, typename TUnaryPredicate>
  ETL_NODISCARD ETL_CONSTEXPR14 TIterator find_if_not(TIterator begin, TIterator end, TUnaryPredicate predicate)
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
  /// adjacent_find
  ///\ingroup algorithm
  ///< a href="http://en.cppreference.com/w/cpp/algorithm/adjacent_find"></a>
  //***************************************************************************
  template <typename TIterator, typename TBinaryPredicate>
  ETL_NODISCARD ETL_CONSTEXPR14 TIterator adjacent_find(TIterator first, TIterator last, TBinaryPredicate predicate)
  {
    if (first != last)
    {
      TIterator next = first;
      ++next;

      while (next != last)
      {
        if (predicate(*first, *next))
        {
          return first;
        }

        ++first;
        ++next;
      }
    }

    return last;
  }

  //***************************************************************************
  /// adjacent_find
  ///\ingroup algorithm
  ///< a href="http://en.cppreference.com/w/cpp/algorithm/adjacent_find"></a>
  //***************************************************************************
  template <typename TIterator>
  ETL_NODISCARD ETL_CONSTEXPR14 TIterator adjacent_find(TIterator first, TIterator last)
  {
    typedef etl::equal_to<typename etl::iterator_traits<TIterator>::value_type> predicate;

    return etl::adjacent_find(first, last, predicate());
  }

  //***************************************************************************
  /// is_permutation
  ///\ingroup algorithm
  ///< a href="http://en.cppreference.com/w/cpp/algorithm/is_permutation"></a>
  //***************************************************************************
  template <typename TIterator1, typename TIterator2>
  ETL_NODISCARD ETL_CONSTEXPR14 bool is_permutation(TIterator1 begin1, TIterator1 end1, TIterator2 begin2)
  {
    if (begin1 != end1)
    {
      TIterator2 end2 = begin2;

      etl::advance(end2, etl::distance(begin1, end1));

      for (TIterator1 i = begin1; i != end1; ++i)
      {
        if (i == etl::find(begin1, i, *i))
        {
          size_t n = static_cast<size_t>(etl::count(begin2, end2, *i));

          if (n == 0 || size_t(etl::count(i, end1, *i)) != n)
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
  ///< a href="http://en.cppreference.com/w/cpp/algorithm/is_permutation"></a>
  //***************************************************************************
  template <typename TIterator1, typename TIterator2, typename TBinaryPredicate>
  ETL_NODISCARD ETL_CONSTEXPR14 bool is_permutation(TIterator1 begin1, TIterator1 end1, TIterator2 begin2, TBinaryPredicate predicate)
  {
    if (begin1 != end1)
    {
      TIterator2 end2 = begin2;

      etl::advance(end2, etl::distance(begin1, end1));

      for (TIterator1 i = begin1; i != end1; ++i)
      {
        const typename etl::binder1st<TBinaryPredicate> predicate_is_i = etl::bind1st(predicate, *i);
        if (i == etl::find_if(begin1, i, predicate_is_i))
        {
          size_t n = static_cast<size_t>(etl::count_if(begin2, end2, predicate_is_i));

          if (n == 0 || size_t(etl::count_if(i, end1, predicate_is_i)) != n)
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
  ///< a href="http://en.cppreference.com/w/cpp/algorithm/is_permutation"></a>
  //***************************************************************************
  template <typename TIterator1, typename TIterator2>
  ETL_NODISCARD ETL_CONSTEXPR14 bool is_permutation(TIterator1 begin1, TIterator1 end1, TIterator2 begin2, TIterator2 end2)
  {
    if (etl::distance(begin1, end1) != etl::distance(begin2, end2))
    {
      return false;
    }

    if (begin1 != end1)
    {
      for (TIterator1 i = begin1; i != end1; ++i)
      {
        if (i == etl::find(begin1, i, *i))
        {
          size_t n = static_cast<size_t>(etl::count(begin2, end2, *i));

          if (n == 0 || size_t(etl::count(i, end1, *i)) != n)
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
  ///< a href="http://en.cppreference.com/w/cpp/algorithm/is_permutation"></a>
  //***************************************************************************
  template <typename TIterator1, typename TIterator2, typename TBinaryPredicate>
  ETL_NODISCARD ETL_CONSTEXPR14 bool is_permutation(TIterator1 begin1, TIterator1 end1, TIterator2 begin2, TIterator2 end2, TBinaryPredicate predicate)
  {
    if (etl::distance(begin1, end1) != etl::distance(begin2, end2))
    {
      return false;
    }

    if (begin1 != end1)
    {
      for (TIterator1 i = begin1; i != end1; ++i)
      {
        const typename etl::binder1st<TBinaryPredicate> predicate_is_i = etl::bind1st(predicate, *i);
        if (i == etl::find_if(begin1, i, predicate_is_i))
        {
          size_t n = static_cast<size_t>(etl::count_if(begin2, end2, predicate_is_i));

          if (n == 0 || size_t(etl::count_if(i, end1, predicate_is_i)) != n)
          {
            return false;
          }
        }
      }
    }

    return true;
  }

  //***************************************************************************
  /// next_permutation
  ///\ingroup algorithm
  ///< a href="http://en.cppreference.com/w/cpp/algorithm/next_permutation"></a>
  //***************************************************************************
  template <typename TIterator, typename TCompare>
  ETL_CONSTEXPR14 bool next_permutation(TIterator first, TIterator last, TCompare compare)
  {
    if (first == last)
    {
      return false;
    }

    TIterator i = last;
    --i;

    if (first == i)
    {
      return false;
    }

    while (true)
    {
      TIterator i1 = i;
      --i;

      if (compare(*i, *i1))
      {
        TIterator j = last;
        --j;

        while (!compare(*i, *j))
        {
          --j;
        }

        etl::iter_swap(i, j);
        etl::reverse(i1, last);
        return true;
      }

      if (i == first)
      {
        etl::reverse(first, last);
        return false;
      }
    }
  }

  //***************************************************************************
  /// next_permutation
  ///\ingroup algorithm
  ///< a href="http://en.cppreference.com/w/cpp/algorithm/next_permutation"></a>
  //***************************************************************************
  template <typename TIterator>
  ETL_CONSTEXPR14 bool next_permutation(TIterator first, TIterator last)
  {
    typedef etl::less<typename etl::iterator_traits<TIterator>::value_type> compare;
    return etl::next_permutation(first, last, compare());
  }

  //***************************************************************************
  /// prev_permutation
  ///\ingroup algorithm
  ///< a href="http://en.cppreference.com/w/cpp/algorithm/prev_permutation"></a>
  //***************************************************************************
  template <typename TIterator, typename TCompare>
  ETL_CONSTEXPR14 bool prev_permutation(TIterator first, TIterator last, TCompare compare)
  {
    if (first == last)
    {
      return false;
    }

    TIterator i = last;
    --i;

    if (first == i)
    {
      return false;
    }

    while (true)
    {
      TIterator i1 = i;
      --i;

      if (compare(*i1, *i))
      {
        TIterator j = last;
        --j;

        while (!compare(*j, *i))
        {
          --j;
        }

        etl::iter_swap(i, j);
        etl::reverse(i1, last);
        return true;
      }

      if (i == first)
      {
        etl::reverse(first, last);
        return false;
      }
    }
  }

  //***************************************************************************
  /// prev_permutation
  ///\ingroup algorithm
  ///< a href="http://en.cppreference.com/w/cpp/algorithm/prev_permutation"></a>
  //***************************************************************************
  template <typename TIterator>
  ETL_CONSTEXPR14 bool prev_permutation(TIterator first, TIterator last)
  {
    typedef etl::less<typename etl::iterator_traits<TIterator>::value_type> compare;
    return etl::prev_permutation(first, last, compare());
  }

  //***************************************************************************
  /// is_partitioned
  ///\ingroup algorithm
  ///< a href="http://en.cppreference.com/w/cpp/algorithm/is_partitioned"></a>
  //***************************************************************************
  template <typename TIterator, typename TUnaryPredicate>
  ETL_NODISCARD ETL_CONSTEXPR14 bool is_partitioned(TIterator begin, TIterator end, TUnaryPredicate predicate)
  {
    while (begin != end)
    {
      if (!predicate(*begin))
      {
        break;
      }

      ++begin;
    }

    while (begin != end)
    {
      if (predicate(*begin))
      {
        return false;
      }

      ++begin;
    }

    return true;
  }

  //***************************************************************************
  /// partition_point
  ///< a href="http://en.cppreference.com/w/cpp/algorithm/partition_point"></a>
  ///\ingroup algorithm
  //***************************************************************************
  template <typename TIterator, typename TUnaryPredicate>
  ETL_NODISCARD ETL_CONSTEXPR14 TIterator partition_point(TIterator begin, TIterator end, TUnaryPredicate predicate)
  {
    typedef typename etl::iterator_traits<TIterator>::difference_type difference_t;

    // binary search on a partitioned range
    for (difference_t length = etl::distance(begin, end); 0 < length;)
    {
      difference_t half   = length / 2;
      TIterator    middle = etl::next(begin, half);
      if (predicate(*middle))
      {
        begin = etl::next(middle);
        length -= (half + 1);
      }
      else
      {
        length = half;
      }
    }

    return begin;
  }

  //***************************************************************************
  /// Copies the elements from the range (begin, end) to two different ranges
  /// depending on the value returned by the predicate.<br>
  ///< a href="http://en.cppreference.com/w/cpp/algorithm/partition_copy"></a>
  ///\ingroup algorithm
  //***************************************************************************
  template <typename TSource, typename TDestinationTrue, typename TDestinationFalse, typename TUnaryPredicate>
  ETL_CONSTEXPR14 ETL_OR_STD::pair<TDestinationTrue, TDestinationFalse> partition_copy(TSource begin, TSource end, TDestinationTrue destination_true,
                                                                                 TDestinationFalse destination_false, TUnaryPredicate predicate)
  {
    while (begin != end)
    {
      if (predicate(*begin))
      {
        *destination_true = *begin;
        ++destination_true;
      }
      else
      {
        *destination_false = *begin;
        ++destination_false;
      }

      ++begin;
    }

    return ETL_OR_STD::pair<TDestinationTrue, TDestinationFalse>(destination_true, destination_false);
  }

  //***************************************************************************
  /// copy_if
  ///\ingroup algorithm
  ///< a href="http://en.cppreference.com/w/cpp/algorithm/copy"></a>
  //***************************************************************************
  template <typename TIterator, typename TOutputIterator, typename TUnaryPredicate>
  ETL_CONSTEXPR14 TOutputIterator copy_if(TIterator begin, TIterator end, TOutputIterator out, TUnaryPredicate predicate)
  {
    while (begin != end)
    {
      if (predicate(*begin))
      {
        *out = *begin;
        ++out;
      }

      ++begin;
    }

    return out;
  }

  //***************************************************************************
  /// all_of
  ///\ingroup algorithm
  ///< a href="http://en.cppreference.com/w/cpp/algorithm/all_any_none_of"></a>
  //***************************************************************************
  template <typename TIterator, typename TUnaryPredicate>
  ETL_NODISCARD ETL_CONSTEXPR14 bool all_of(TIterator begin, TIterator end, TUnaryPredicate predicate)
  {
    return etl::find_if_not(begin, end, predicate) == end;
  }

  //***************************************************************************
  /// any_of
  ///\ingroup algorithm
  ///< a href="http://en.cppreference.com/w/cpp/algorithm/all_any_none_of"></a>
  //***************************************************************************
  template <typename TIterator, typename TUnaryPredicate>
  ETL_NODISCARD ETL_CONSTEXPR14 bool any_of(TIterator begin, TIterator end, TUnaryPredicate predicate)
  {
    return etl::find_if(begin, end, predicate) != end;
  }

  //***************************************************************************
  /// none_of
  ///\ingroup algorithm
  ///< a href="http://en.cppreference.com/w/cpp/algorithm/all_any_none_of"></a>
  //***************************************************************************
  template <typename TIterator, typename TUnaryPredicate>
  ETL_NODISCARD ETL_CONSTEXPR14 bool none_of(TIterator begin, TIterator end, TUnaryPredicate predicate)
  {
    return etl::find_if(begin, end, predicate) == end;
  }

#if ETL_NOT_USING_STL
  //***************************************************************************
  /// Sorts the elements.
  /// Uses user defined comparison.
  ///\ingroup algorithm
  //***************************************************************************
  template <typename TIterator, typename TCompare>
  void sort(TIterator first, TIterator last, TCompare compare)
  {
    etl::shell_sort(first, last, compare);
  }

  //***************************************************************************
  /// Sorts the elements.
  ///\ingroup algorithm
  //***************************************************************************
  template <typename TIterator>
  void sort(TIterator first, TIterator last)
  {
    etl::shell_sort(first, last, etl::less<typename etl::iterator_traits<TIterator>::value_type>());
  }

  //***************************************************************************
  /// Sorts the elements.
  /// Stable.
  /// Uses user defined comparison.
  ///\ingroup algorithm
  //***************************************************************************
  template <typename TIterator, typename TCompare>
  void stable_sort(TIterator first, TIterator last, TCompare compare)
  {
    etl::insertion_sort(first, last, compare);
  }

  //***************************************************************************
  /// Sorts the elements.
  /// Stable.
  ///\ingroup algorithm
  //***************************************************************************
  template <typename TIterator>
  void stable_sort(TIterator first, TIterator last)
  {
    etl::insertion_sort(first, last, etl::less<typename etl::iterator_traits<TIterator>::value_type>());
  }
#else
  //***************************************************************************
  /// Sorts the elements.
  /// Uses user defined comparison.
  ///\ingroup algorithm
  //***************************************************************************
  template <typename TIterator, typename TCompare>
  void sort(TIterator first, TIterator last, TCompare compare)
  {
    std::sort(first, last, compare);
  }

  //***************************************************************************
  /// Sorts the elements.
  ///\ingroup algorithm
  //***************************************************************************
  template <typename TIterator>
  void sort(TIterator first, TIterator last)
  {
    std::sort(first, last);
  }

  //***************************************************************************
  /// Sorts the elements.
  /// Stable.
  /// Uses user defined comparison.
  ///\ingroup algorithm
  //***************************************************************************
  template <typename TIterator, typename TCompare>
  void stable_sort(TIterator first, TIterator last, TCompare compare)
  {
    std::stable_sort(first, last, compare);
  }

  //***************************************************************************
  /// Sorts the elements.
  /// Stable.
  ///\ingroup algorithm
  //***************************************************************************
  template <typename TIterator>
  void stable_sort(TIterator first, TIterator last)
  {
    std::stable_sort(first, last);
  }
#endif

  //***************************************************************************
  /// Accumulates values.
  ///\ingroup algorithm
  //***************************************************************************
  template <typename TIterator, typename T>
  ETL_CONSTEXPR14 T accumulate(TIterator first, TIterator last, T sum)
  {
    while (first != last)
    {
      sum = static_cast<T>(ETL_MOVE(sum) + static_cast<T>(*first));
      ++first;
    }

    return sum;
  }

  //***************************************************************************
  /// Accumulates values.
  ///\ingroup algorithm
  //***************************************************************************
  template <typename TIterator, typename T, typename TBinaryOperation>
  ETL_CONSTEXPR14 T accumulate(TIterator first, TIterator last, T sum, TBinaryOperation operation)
  {
    while (first != last)
    {
      sum = operation(ETL_MOVE(sum), *first);
      ++first;
    }

    return sum;
  }

  //***************************************************************************
  /// Clamp values.
  ///\ingroup algorithm
  //***************************************************************************
  template <typename T, typename TCompare>
  ETL_CONSTEXPR T clamp(const T& value, const T& low, const T& high, TCompare compare)
  {
    return compare(value, low) ? low : compare(high, value) ? high : value;
  }

  template <typename T>
  ETL_CONSTEXPR T clamp(const T& value, const T& low, const T& high)
  {
    return clamp(value, low, high, etl::less<T>());
  }

  template <typename T, T Low, T High, typename TCompare>
  ETL_CONSTEXPR T clamp(const T& value, TCompare compare)
  {
    return compare(value, Low) ? Low : compare(High, value) ? High : value;
  }

  template <typename T, T Low, T High>
  ETL_CONSTEXPR T clamp(const T& value)
  {
    return clamp<T, Low, High>(value, etl::less<T>());
  }

  //***************************************************************************
  /// Remove
  ///\ingroup algorithm
  //***************************************************************************
  template <typename TIterator, typename T>
  ETL_CONSTEXPR14 TIterator remove(TIterator first, TIterator last, const T& value)
  {
    first = etl::find(first, last, value);

    if (first != last)
    {
      TIterator itr = first;

      while (++itr != last)
      {
        if (!(*itr == value))
        {
          *first++ = ETL_MOVE(*itr);
        }
      }
    }

    return first;
  }

  //***************************************************************************
  /// Remove If
  ///\ingroup algorithm
  //***************************************************************************
  template <typename TIterator, typename TUnaryPredicate>
  ETL_CONSTEXPR14 TIterator remove_if(TIterator first, TIterator last, TUnaryPredicate predicate)
  {
    first = etl::find_if(first, last, predicate);

    if (first != last)
    {
      TIterator itr = first;

      while (++itr != last)
      {
        if (!predicate(*itr))
        {
          *first++ = ETL_MOVE(*itr);
        }
      }
    }

    return first;
  }

  //***************************************************************************
  /// unique
  /// see https://en.cppreference.com/w/cpp/algorithm/unique
  ///\ingroup algorithm
  //***************************************************************************
  template <typename TIterator>
  ETL_CONSTEXPR14 TIterator unique(TIterator first, TIterator last)
  {
    if (first == last)
    {
      return last;
    }

    TIterator result = first;

    while (++first != last)
    {
      if (!(*result == *first) && (++result != first))
      {
        *result = ETL_MOVE(*first);
      }
    }

    return ++result;
  }

  //***************************************************************************
  /// unique
  /// see https://en.cppreference.com/w/cpp/algorithm/unique
  /// predicate overload to determine equality.
  ///\ingroup algorithm
  //***************************************************************************
  template <typename TIterator, typename TBinaryPredicate>
  ETL_CONSTEXPR14 TIterator unique(TIterator first, TIterator last, TBinaryPredicate predicate)
  {
    if (first == last)
    {
      return last;
    }

    TIterator result = first;

    while (++first != last)
    {
      if (!predicate(*result, *first) && (++result != first))
      {
        *result = ETL_MOVE(*first);
      }
    }

    return ++result;
  }

  //***************************************************************************
  /// unique_copy
  /// see https://en.cppreference.com/w/cpp/algorithm/unique_copy
  ///\ingroup algorithm
  //***************************************************************************
  template <typename TInputIterator, typename TOutputIterator>
  ETL_CONSTEXPR14 TOutputIterator unique_copy(TInputIterator first, TInputIterator last, TOutputIterator d_first)
  {
    if (first == last)
    {
      return d_first;
    }

    typename etl::iterator_traits<TInputIterator>::value_type prev = *first;
    *d_first                                                       = prev;

    while (++first != last)
    {
      if (!(prev == *first))
      {
        prev         = *first;
        *(++d_first) = prev;
      }
    }

    return ++d_first;
  }

  //***************************************************************************
  /// unique_copy
  /// see https://en.cppreference.com/w/cpp/algorithm/unique_copy
  /// predicate overload to determine equality.
  ///\ingroup algorithm
  //***************************************************************************
  template <typename TInputIterator, typename TOutputIterator, typename TBinaryPredicate>
  ETL_CONSTEXPR14 TOutputIterator unique_copy(TInputIterator first, TInputIterator last, TOutputIterator d_first, TBinaryPredicate predicate)
  {
    if (first == last)
    {
      return d_first;
    }

    typename etl::iterator_traits<TInputIterator>::value_type prev = *first;
    *d_first                                                       = prev;

    while (++first != last)
    {
      if (!predicate(prev, *first))
      {
        prev         = *first;
        *(++d_first) = prev;
      }
    }

    return ++d_first;
  }

  //***************************************************************************
  /// merge
  /// Merges two sorted ranges into one sorted range.
  /// see https://en.cppreference.com/w/cpp/algorithm/merge
  ///\ingroup algorithm
  //***************************************************************************
  template <typename TInputIterator1, typename TInputIterator2, typename TOutputIterator, typename TCompare>
  ETL_CONSTEXPR14 TOutputIterator merge(TInputIterator1 first1, TInputIterator1 last1, TInputIterator2 first2, TInputIterator2 last2,
                                  TOutputIterator d_first, TCompare compare)
  {
    while ((first1 != last1) && (first2 != last2))
    {
      if (compare(*first2, *first1))
      {
        *d_first = *first2;
        ++first2;
      }
      else
      {
        *d_first = *first1;
        ++first1;
      }
      ++d_first;
    }

    d_first = etl::copy(first1, last1, d_first);
    d_first = etl::copy(first2, last2, d_first);

    return d_first;
  }

  //***************************************************************************
  /// merge
  /// Merges two sorted ranges into one sorted range.
  /// Uses operator< for comparison.
  /// see https://en.cppreference.com/w/cpp/algorithm/merge
  ///\ingroup algorithm
  //***************************************************************************
  template <typename TInputIterator1, typename TInputIterator2, typename TOutputIterator>
  ETL_CONSTEXPR14 TOutputIterator merge(TInputIterator1 first1, TInputIterator1 last1, TInputIterator2 first2, TInputIterator2 last2,
                                  TOutputIterator d_first)
  {
    typedef etl::less< typename etl::iterator_traits<TInputIterator1>::value_type> compare;

    return etl::merge(first1, last1, first2, last2, d_first, compare());
  }

  //***************************************************************************
  /// inplace_merge
  /// Merges two consecutive sorted ranges [first, middle) and [middle, last)
  /// into one sorted range [first, last) in-place.
  /// Uses an iterative rotate-based algorithm that requires no additional
  /// memory, no recursion and no explicit stack, making it safe for deeply
  /// embedded targets with constrained stack sizes.
  /// Complexity: O(N log N) comparisons, O(N log N) element moves.
  /// see https://en.cppreference.com/w/cpp/algorithm/inplace_merge
  ///\ingroup algorithm
  //***************************************************************************
  template <typename TBidirectionalIterator, typename TCompare>
  void inplace_merge(TBidirectionalIterator first, TBidirectionalIterator middle, TBidirectionalIterator last, TCompare compare)
  {
    typedef typename etl::iterator_traits<TBidirectionalIterator>::difference_type difference_type;

    difference_type len1 = etl::distance(first, middle);
    difference_type len2 = etl::distance(middle, last);

    while ((len1 != 0) && (len2 != 0))
    {
      // Find where the first element of the right half belongs in the left
      // half. All elements in [first, cut1) are <= *middle, so they are already
      // in place.
      TBidirectionalIterator cut1   = etl::upper_bound(first, middle, *middle, compare);
      difference_type        prefix = etl::distance(first, cut1);
      len1 -= prefix;

      // If the entire left half is <= *middle, we are done.
      if (len1 == 0)
      {
        return;
      }

      // Advance first past the already-placed prefix.
      first = cut1;

      // Find where the first element of the (remaining) left half belongs in
      // the right half.  All elements in [middle, cut2) are < *first, so they
      // need to be moved before *first.
      TBidirectionalIterator cut2 = etl::lower_bound(middle, last, *first, compare);
      difference_type        run  = etl::distance(middle, cut2);
      len2 -= run;

      // Rotate the block [first, middle, cut2) so that [middle, cut2) moves
      // before [first, middle).  After the rotate the elements from
      // [middle, cut2) (length = run) now occupy [first, first + run) and
      // are in their final position.
      etl::rotate(first, middle, cut2);

      // Advance past the block we just placed.
      etl::advance(first, run);
      middle = cut2;
    }
  }

  //***************************************************************************
  /// inplace_merge
  /// Merges two consecutive sorted ranges [first, middle) and [middle, last)
  /// into one sorted range [first, last) in-place.
  /// Uses operator< for comparison.
  /// see https://en.cppreference.com/w/cpp/algorithm/inplace_merge
  ///\ingroup algorithm
  //***************************************************************************
  template <typename TBidirectionalIterator>
  void inplace_merge(TBidirectionalIterator first, TBidirectionalIterator middle, TBidirectionalIterator last)
  {
    typedef etl::less< typename etl::iterator_traits<TBidirectionalIterator>::value_type> compare;

    etl::inplace_merge(first, middle, last, compare());
  }
} // namespace etl

//*****************************************************************************
// ETL extensions to the STL algorithms.
//*****************************************************************************
namespace etl
{
  //***************************************************************************
  /// copy_s
  /// A safer form of copy where the smallest of the two ranges is used.
  /// There is currently no STL equivalent.
  /// Specialisation for random access iterators.
  ///\param i_begin Beginning of the input range.
  ///\param i_end   End of the input range.
  ///\param o_begin Beginning of the output range.
  ///\param o_end   End of the output range.
  ///\ingroup algorithm
  //***************************************************************************
  template <typename TInputIterator, typename TOutputIterator>
  ETL_CONSTEXPR14
    typename etl::enable_if< etl::is_random_iterator<TInputIterator>::value && etl::is_random_iterator<TOutputIterator>::value, TOutputIterator>::type
    copy_s(TInputIterator i_begin, TInputIterator i_end, TOutputIterator o_begin, TOutputIterator o_end)
  {
    typedef typename iterator_traits<TInputIterator>::difference_type  s_size_type;
    typedef typename iterator_traits<TOutputIterator>::difference_type d_size_type;

#if ETL_USING_CPP11
    typedef typename etl::common_type<s_size_type, d_size_type>::type min_size_type;
#else
    typedef typename etl::largest_type<s_size_type, d_size_type>::type min_size_type;
#endif

    s_size_type   s_size   = etl::distance(i_begin, i_end);
    d_size_type   d_size   = etl::distance(o_begin, o_end);
    min_size_type min_size = etl::min<min_size_type>(s_size, d_size);

    return etl::copy(i_begin, i_begin + min_size, o_begin);
  }

  //***************************************************************************
  /// copy
  /// A safer form of copy where the smallest of the two ranges is used.
  /// There is currently no STL equivalent.
  /// Specialisation for non random access iterators.
  ///\param i_begin Beginning of the input range.
  ///\param i_end   End of the input range.
  ///\param o_begin Beginning of the output range.
  ///\param o_end   End of the output range.
  ///\ingroup algorithm
  //***************************************************************************
  template <typename TInputIterator, typename TOutputIterator>
  ETL_CONSTEXPR14 typename etl::enable_if< !etl::is_random_iterator<TInputIterator>::value || !etl::is_random_iterator<TOutputIterator>::value,
                                     TOutputIterator>::type
    copy_s(TInputIterator i_begin, TInputIterator i_end, TOutputIterator o_begin, TOutputIterator o_end)
  {
    while ((i_begin != i_end) && (o_begin != o_end))
    {
      *o_begin = *i_begin;
      ++o_begin;
      ++i_begin;
    }

    return o_begin;
  }

  //***************************************************************************
  /// copy_n
  /// A safer form of copy_n where the smallest of the two ranges is used.
  ///\ingroup algorithm
  //***************************************************************************
  template <typename TInputIterator, typename TSize, typename TOutputIterator>
  ETL_CONSTEXPR14 TOutputIterator copy_n_s(TInputIterator i_begin, TSize n, TOutputIterator o_begin, TOutputIterator o_end)
  {
    while ((n-- > 0) && (o_begin != o_end))
    {
      *o_begin = *i_begin;
      ++o_begin;
      ++i_begin;
    }

    return o_begin;
  }

  //***************************************************************************
  /// copy_n
  /// A safer form of copy_n where the smallest of the two ranges is used.
  ///\ingroup algorithm
  //***************************************************************************
  template <typename TInputIterator, typename TSize1, typename TOutputIterator, typename TSize2>
  ETL_CONSTEXPR14 TOutputIterator copy_n_s(TInputIterator i_begin, TSize1 n1, TOutputIterator o_begin, TSize2 n2)
  {
    while ((n1-- > 0) && (n2-- > 0))
    {
      *o_begin = *i_begin;
      ++o_begin;
      ++i_begin;
    }

    return o_begin;
  }

  //***************************************************************************
  /// copy_if
  /// A safer form of copy_if where it terminates when the first end iterator is
  /// reached. There is currently no STL equivalent.
  ///\ingroup algorithm
  //***************************************************************************
  template <typename TInputIterator, typename TOutputIterator, typename TUnaryPredicate>
  ETL_CONSTEXPR14 TOutputIterator copy_if_s(TInputIterator i_begin, TInputIterator i_end, TOutputIterator o_begin, TOutputIterator o_end,
                                      TUnaryPredicate predicate)
  {
    while ((i_begin != i_end) && (o_begin != o_end))
    {
      if (predicate(*i_begin))
      {
        *o_begin = *i_begin;
        ++o_begin;
      }

      ++i_begin;
    }

    return o_begin;
  }

  //***************************************************************************
  /// copy_n_if
  /// Combination of copy_n and copy_if.
  ///\ingroup algorithm
  //***************************************************************************
  template <typename TInputIterator, typename TSize, typename TOutputIterator, typename TUnaryPredicate>
  ETL_CONSTEXPR14 TOutputIterator copy_n_if(TInputIterator i_begin, TSize n, TOutputIterator o_begin, TUnaryPredicate predicate)
  {
    while (n-- > 0)
    {
      if (predicate(*i_begin))
      {
        *o_begin = *i_begin;
        ++o_begin;
      }

      ++i_begin;
    }

    return o_begin;
  }

#if ETL_USING_CPP11
  //***************************************************************************
  /// move_s
  /// A safer form of move where the smallest of the two ranges is used.
  /// There is currently no STL equivalent.
  /// Specialisation for random access iterators.
  ///\param i_begin Beginning of the input range.
  ///\param i_end   End of the input range.
  ///\param o_begin Beginning of the output range.
  ///\param o_end   End of the output range.
  ///\ingroup algorithm
  //***************************************************************************
  template <typename TInputIterator, typename TOutputIterator>
  ETL_CONSTEXPR14
    typename etl::enable_if< etl::is_random_iterator<TInputIterator>::value && etl::is_random_iterator<TOutputIterator>::value, TOutputIterator>::type
    move_s(TInputIterator i_begin, TInputIterator i_end, TOutputIterator o_begin, TOutputIterator o_end)
  {
    using s_size_type   = typename iterator_traits<TInputIterator>::difference_type;
    using d_size_type   = typename iterator_traits<TOutputIterator>::difference_type;
    using min_size_type = typename etl::common_type<s_size_type, d_size_type>::type;

    s_size_type   s_size   = etl::distance(i_begin, i_end);
    d_size_type   d_size   = etl::distance(o_begin, o_end);
    min_size_type min_size = etl::min<min_size_type>(s_size, d_size);

  #include "etl/private/diagnostic_null_dereference_push.h"
    return etl::move(i_begin, i_begin + min_size, o_begin);
  #include "etl/private/diagnostic_pop.h"
  }

  //***************************************************************************
  /// move_s
  /// A safer form of move where the smallest of the two ranges is used.
  /// There is currently no STL equivalent.
  /// Specialisation for non random access iterators.
  ///\param i_begin Beginning of the input range.
  ///\param i_end   End of the input range.
  ///\param o_begin Beginning of the output range.
  ///\param o_end   End of the output range.
  ///\ingroup algorithm
  //***************************************************************************
  template <typename TInputIterator, typename TOutputIterator>
  ETL_CONSTEXPR14 typename etl::enable_if< !etl::is_random_iterator<TInputIterator>::value || !etl::is_random_iterator<TOutputIterator>::value,
                                     TOutputIterator>::type
    move_s(TInputIterator i_begin, TInputIterator i_end, TOutputIterator o_begin, TOutputIterator o_end)
  {
    while ((i_begin != i_end) && (o_begin != o_end))
    {
      *o_begin = etl::move(*i_begin);
      ++i_begin;
      ++o_begin;
    }

    return o_begin;
  }
#else
  //***************************************************************************
  /// move_s
  /// C++03
  /// A safer form of move where the smallest of the two ranges is used.
  /// There is currently no STL equivalent.
  /// Specialisation for non random access iterators.
  ///\param i_begin Beginning of the input range.
  ///\param i_end   End of the input range.
  ///\param o_begin Beginning of the output range.
  ///\param o_end   End of the output range.
  ///\ingroup algorithm
  //***************************************************************************
  template <typename TInputIterator, typename TOutputIterator>
  TOutputIterator move_s(TInputIterator i_begin, TInputIterator i_end, TOutputIterator o_begin, TOutputIterator o_end)
  {
    // Move not supported. Defer to copy.
    return etl::copy_s(i_begin, i_end, o_begin, o_end);
  }
#endif

  //***************************************************************************
  /// binary_find
  ///\ingroup algorithm
  /// Does a binary search and returns an iterator to the value or end if not
  /// found.
  //***************************************************************************
  template <typename TIterator, typename TValue>
  ETL_NODISCARD ETL_CONSTEXPR14 TIterator binary_find(TIterator begin, TIterator end, const TValue& value)
  {
    TIterator it = etl::lower_bound(begin, end, value);

    if ((it == end) || (*it != value))
    {
      it = end;
    }

    return it;
  }

  //***************************************************************************
  /// binary_find
  ///\ingroup algorithm
  /// Does a binary search and returns an iterator to the value or end if not
  /// found.
  //***************************************************************************
  template <typename TIterator, typename TValue, typename TBinaryPredicate, typename TBinaryEquality>
  ETL_NODISCARD ETL_CONSTEXPR14 TIterator binary_find(TIterator begin, TIterator end, const TValue& value, TBinaryPredicate predicate, TBinaryEquality equality)
  {
    TIterator it = etl::lower_bound(begin, end, value, predicate);

    if ((it == end) || !equality(*it, value))
    {
      it = end;
    }

    return it;
  }

  //***************************************************************************
  /// Like std::for_each but applies a predicate before calling the function.
  ///\ingroup algorithm
  //***************************************************************************
  template <typename TIterator, typename TUnaryFunction, typename TUnaryPredicate>
  ETL_CONSTEXPR14 TUnaryFunction for_each_if(TIterator begin, const TIterator end, TUnaryFunction function, TUnaryPredicate predicate)
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
  /// Like std::for_each but for 'n' iterations.
  ///\ingroup algorithm
  //***************************************************************************
  template <typename TIterator, typename TSize, typename TUnaryFunction>
  ETL_CONSTEXPR14 TIterator for_each_n(TIterator begin, TSize n, TUnaryFunction function)
  {
    while (n-- > 0)
    {
      function(*begin);
      ++begin;
    }

    return begin;
  }

  //***************************************************************************
  /// Like std::for_each but applies a predicate before calling the function,
  /// for 'n' iterations
  ///\ingroup algorithm
  //***************************************************************************
  template <typename TIterator, typename TSize, typename TUnaryFunction, typename TUnaryPredicate>
  ETL_CONSTEXPR14 TIterator for_each_n_if(TIterator begin, TSize n, TUnaryFunction function, TUnaryPredicate predicate)
  {
    while (n-- > 0)
    {
      if (predicate(*begin))
      {
        function(*begin);
      }

      ++begin;
    }

    return begin;
  }

  //***************************************************************************
  /// A safer form of std::transform where the transform returns when the first
  /// range end is reached.
  /// There is currently no STL equivalent.
  ///\ingroup algorithm
  //***************************************************************************
  template <typename TInputIterator, typename TOutputIterator, typename TUnaryFunction>
  ETL_CONSTEXPR14 TOutputIterator transform_s(TInputIterator i_begin, TInputIterator i_end, TOutputIterator o_begin, TOutputIterator o_end,
                                        TUnaryFunction function)
  {
    while ((i_begin != i_end) && (o_begin != o_end))
    {
      *o_begin = function(*i_begin);
      ++i_begin;
      ++o_begin;
    }

    return o_begin;
  }

  //***************************************************************************
  /// Transform 'n' items.
  /// Random iterators.
  /// There is currently no STL equivalent.
  ///\ingroup algorithm
  //***************************************************************************
  template <typename TInputIterator, typename TSize, typename TOutputIterator, typename TUnaryFunction>
  ETL_CONSTEXPR14 void transform_n(TInputIterator i_begin, TSize n, TOutputIterator o_begin, TUnaryFunction function)
  {
    TInputIterator i_end(i_begin);
    etl::advance(i_end, n);

    etl::transform(i_begin, i_end, o_begin, function);
  }

  //***************************************************************************
  /// Transform 'n' items from two ranges.
  /// Random iterators.
  /// There is currently no STL equivalent.
  ///\ingroup algorithm
  //***************************************************************************
  template <typename TInputIterator1, typename TInputIterator2, typename TSize, typename TOutputIterator, typename TBinaryFunction>
  ETL_CONSTEXPR14 void transform_n(TInputIterator1 i_begin1, TInputIterator2 i_begin2, TSize n, TOutputIterator o_begin, TBinaryFunction function)
  {
    TInputIterator1 i_end1(i_begin1);
    etl::advance(i_end1, n);

    etl::transform(i_begin1, i_end1, i_begin2, o_begin, function);
  }

  //***************************************************************************
  /// Like std::transform but applies a predicate before calling the function.
  ///\ingroup algorithm
  //***************************************************************************
  template <typename TInputIterator, typename TOutputIterator, typename TUnaryFunction, typename TUnaryPredicate>
  ETL_CONSTEXPR14 TOutputIterator transform_if(TInputIterator i_begin, const TInputIterator i_end, TOutputIterator o_begin, TUnaryFunction function,
                                         TUnaryPredicate predicate)
  {
    while (i_begin != i_end)
    {
      if (predicate(*i_begin))
      {
        *o_begin = function(*i_begin);
        ++o_begin;
      }

      ++i_begin;
    }

    return o_begin;
  }

  //***************************************************************************
  /// Like etl::transform_if but inputs from two ranges.
  ///\ingroup algorithm
  //***************************************************************************
  template <typename TInputIterator1, typename TInputIterator2, typename TOutputIterator, typename TBinaryFunction, typename TBinaryPredicate>
  ETL_CONSTEXPR14 TOutputIterator transform_if(TInputIterator1 i_begin1, const TInputIterator1 i_end1, TInputIterator2 i_begin2, TOutputIterator o_begin,
                                         TBinaryFunction function, TBinaryPredicate predicate)
  {
    while (i_begin1 != i_end1)
    {
      if (predicate(*i_begin1, *i_begin2))
      {
        *o_begin = function(*i_begin1, *i_begin2);
        ++o_begin;
      }

      ++i_begin1;
      ++i_begin2;
    }

    return o_begin;
  }

  //***************************************************************************
  /// Like std::transform_if, for 'n' items.
  ///\ingroup algorithm
  //***************************************************************************
  template <typename TInputIterator, typename TSize, typename TOutputIterator, typename TUnaryFunction, typename TUnaryPredicate>
  ETL_CONSTEXPR14 TOutputIterator transform_n_if(TInputIterator i_begin, TSize n, TOutputIterator o_begin, TUnaryFunction function,
                                           TUnaryPredicate predicate)
  {
    while (n-- > 0)
    {
      if (predicate(*i_begin))
      {
        *o_begin = function(*i_begin);
        ++o_begin;
      }

      ++i_begin;
    }

    return o_begin;
  }

  //***************************************************************************
  /// Like etl::transform_if but inputs from two ranges for 'n' items.
  ///\ingroup algorithm
  //***************************************************************************
  template <typename TInputIterator1, typename TInputIterator2, typename TSize, typename TOutputIterator, typename TBinaryFunction,
            typename TBinaryPredicate>
  ETL_CONSTEXPR14 TOutputIterator transform_n_if(TInputIterator1 i_begin1, TInputIterator2 i_begin2, TSize n, TOutputIterator o_begin,
                                           TBinaryFunction function, TBinaryPredicate predicate)
  {
    while (n-- > 0)
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
  /// Transforms the elements from the range (begin, end) to two different
  /// ranges depending on the value returned by the predicate.<br>
  ///\ingroup algorithm
  //***************************************************************************
  template <typename TSource, typename TDestinationTrue, typename TDestinationFalse, typename TUnaryFunctionTrue, typename TUnaryFunctionFalse,
            typename TUnaryPredicate>
  ETL_CONSTEXPR14 ETL_OR_STD::pair<TDestinationTrue, TDestinationFalse>
    partition_transform(TSource begin, TSource end, TDestinationTrue destination_true, TDestinationFalse destination_false,
                        TUnaryFunctionTrue function_true, TUnaryFunctionFalse function_false, TUnaryPredicate predicate)
  {
    while (begin != end)
    {
      if (predicate(*begin))
      {
        *destination_true = function_true(*begin);
        ++destination_true;
      }
      else
      {
        *destination_false = function_false(*begin);
        ++destination_false;
      }

      ++begin;
    }

    return ETL_OR_STD::pair<TDestinationTrue, TDestinationFalse>(destination_true, destination_false);
  }

  //***************************************************************************
  /// Transforms the elements from the ranges (begin1, end1) & (begin2)
  /// to two different ranges depending on the value returned by the predicate.
  ///\ingroup algorithm
  //***************************************************************************
  template <typename TSource1, typename TSource2, typename TDestinationTrue, typename TDestinationFalse, typename TBinaryFunctionTrue,
            typename TBinaryFunctionFalse, typename TBinaryPredicate>
  ETL_CONSTEXPR14 ETL_OR_STD::pair<TDestinationTrue, TDestinationFalse>
    partition_transform(TSource1 begin1, TSource1 end1, TSource2 begin2, TDestinationTrue destination_true, TDestinationFalse destination_false,
                        TBinaryFunctionTrue function_true, TBinaryFunctionFalse function_false, TBinaryPredicate predicate)
  {
    while (begin1 != end1)
    {
      if (predicate(*begin1, *begin2))
      {
        *destination_true = function_true(*begin1, *begin2);
        ++destination_true;
      }
      else
      {
        *destination_false = function_false(*begin1, *begin2);
        ++destination_false;
      }

      ++begin1;
      ++begin2;
    }

    return ETL_OR_STD::pair<TDestinationTrue, TDestinationFalse>(destination_true, destination_false);
  }

  //***************************************************************************
  /// Sorts the elements using shell sort.
  /// Uses user defined comparison.
  ///\ingroup algorithm
  //***************************************************************************
  template <typename TIterator, typename TCompare>
#if ETL_USING_STD_NAMESPACE
  ETL_CONSTEXPR20
#else
  ETL_CONSTEXPR14
#endif
    void
    shell_sort(TIterator first, TIterator last, TCompare compare)
  {
    if (first == last)
    {
      return;
    }

    typedef typename etl::iterator_traits<TIterator>::difference_type difference_t;

    difference_t n = etl::distance(first, last);

    for (difference_t i = n / 2; i > 0; i /= 2)
    {
      for (difference_t j = i; j < n; ++j)
      {
        for (difference_t k = j - i; k >= 0; k -= i)
        {
          TIterator itr1 = first;
          TIterator itr2 = first;

          etl::advance(itr1, k);
          etl::advance(itr2, k + i);

          if (compare(*itr2, *itr1))
          {
            etl::iter_swap(itr1, itr2);
          }
        }
      }
    }
  }

  //***************************************************************************
  /// Sorts the elements using shell sort.
  ///\ingroup algorithm
  //***************************************************************************
  template <typename TIterator>
#if ETL_USING_STD_NAMESPACE
  ETL_CONSTEXPR20
#else
  ETL_CONSTEXPR14
#endif
    void
    shell_sort(TIterator first, TIterator last)
  {
    etl::shell_sort(first, last, etl::less<typename etl::iterator_traits<TIterator>::value_type>());
  }

  //***************************************************************************
  /// Sorts the elements using insertion sort.
  /// Uses user defined comparison.
  ///\ingroup algorithm
  //***************************************************************************
  template <typename TIterator, typename TCompare>
  ETL_CONSTEXPR14 void insertion_sort(TIterator first, TIterator last, TCompare compare)
  {
    for (TIterator itr = first; itr != last; ++itr)
    {
      etl::rotate(etl::upper_bound(first, itr, *itr, compare), itr, etl::next(itr));
    }
  }

  //***************************************************************************
  /// Sorts the elements using insertion sort.
  ///\ingroup algorithm
  //***************************************************************************
  template <typename TIterator>
  ETL_CONSTEXPR14 void insertion_sort(TIterator first, TIterator last)
  {
    etl::insertion_sort(first, last, etl::less<typename etl::iterator_traits<TIterator>::value_type>());
  }

  //***************************************************************************
  namespace private_algorithm
  {
    template <typename TIterator>
    ETL_CONSTEXPR14 typename etl::enable_if<etl::is_forward_iterator<TIterator>::value, TIterator>::type get_before_last(TIterator first_, TIterator last_)
    {
      TIterator last      = first_;
      TIterator lastplus1 = first_;
      ++lastplus1;

      while (lastplus1 != last_)
      {
        ++last;
        ++lastplus1;
      }

      return last;
    }

    template <typename TIterator>
    ETL_CONSTEXPR14 typename etl::enable_if<etl::is_bidirectional_iterator<TIterator>::value, TIterator>::type get_before_last(TIterator /*first_*/,
                                                                                                                         TIterator last_)
    {
      TIterator last = last_;
      --last;

      return last;
    }

    template <typename TIterator>
    ETL_CONSTEXPR14 typename etl::enable_if<etl::is_random_access_iterator<TIterator>::value, TIterator>::type get_before_last(TIterator /*first_*/,
                                                                                                                         TIterator last_)
    {
      return last_ - 1;
    }
  } // namespace private_algorithm

  //***************************************************************************
  /// Sorts the elements using selection sort.
  /// Uses user defined comparison.
  ///\ingroup algorithm
  //***************************************************************************
  template <typename TIterator, typename TCompare>
  ETL_CONSTEXPR20 void selection_sort(TIterator first, TIterator last, TCompare compare)
  {
    if (first == last)
    {
      return;
    }

    TIterator       min;
    const TIterator ilast = private_algorithm::get_before_last(first, last);
    const TIterator jlast = last;

    for (TIterator i = first; i != ilast; ++i)
    {
      min = i;

      TIterator j = i;
      ++j;

      for (; j != jlast; ++j)
      {
        if (compare(*j, *min))
        {
          min = j;
        }
      }

      using ETL_OR_STD::swap; // Allow ADL
      if (min != i)
      {
        swap(*i, *min);
      }
    }
  }

  //***************************************************************************
  /// Sorts the elements using selection sort.
  ///\ingroup algorithm
  //***************************************************************************
  template <typename TIterator>
  ETL_CONSTEXPR20 void selection_sort(TIterator first, TIterator last)
  {
    selection_sort(first, last, etl::less<typename etl::iterator_traits<TIterator>::value_type>());
  }

  //***************************************************************************
  /// Sorts the elements using heap sort.
  /// Uses user defined comparison.
  ///\ingroup algorithm
  //***************************************************************************
  template <typename TIterator, typename TCompare>
  ETL_CONSTEXPR14 void heap_sort(TIterator first, TIterator last, TCompare compare)
  {
    etl::make_heap(first, last, compare);
    etl::sort_heap(first, last, compare);
  }

  //***************************************************************************
  /// Sorts the elements using heap sort.
  ///\ingroup algorithm
  //***************************************************************************
  template <typename TIterator>
  ETL_CONSTEXPR14 void heap_sort(TIterator first, TIterator last)
  {
    etl::make_heap(first, last);
    etl::sort_heap(first, last);
  }

  //***************************************************************************
  /// Returns the maximum value.
  //***************************************************************************
#if ETL_USING_CPP11
  template <typename T>
  ETL_NODISCARD
  constexpr const T& multimax(const T& a, const T& b)
  {
    return a < b ? b : a;
  }

  template <typename T, typename... Tx>
  ETL_NODISCARD
  constexpr const T& multimax(const T& t, const Tx&... tx)
  {
    return multimax(t, multimax(tx...));
  }
#endif

  //***************************************************************************
  /// Returns the maximum value.
  /// User supplied compare function.
  //***************************************************************************
#if ETL_USING_CPP11
  template <typename TCompare, typename T>
  ETL_NODISCARD
  constexpr const T& multimax_compare(TCompare compare, const T& a, const T& b)
  {
    return compare(a, b) ? b : a;
  }

  template <typename TCompare, typename T, typename... Tx>
  ETL_NODISCARD
  constexpr const T& multimax_compare(TCompare compare, const T& t, const Tx&... tx)
  {
    return multimax_compare(compare, t, multimax_compare(compare, tx...));
  }
#endif

  //***************************************************************************
  /// Returns the minimum value.
  //***************************************************************************
#if ETL_USING_CPP11
  template <typename T>
  ETL_NODISCARD
  constexpr const T& multimin(const T& a, const T& b)
  {
    return a < b ? a : b;
  }

  template <typename T, typename... Tx>
  ETL_NODISCARD
  constexpr const T& multimin(const T& t, const Tx&... tx)
  {
    return multimin(t, multimin(tx...));
  }
#endif

  //***************************************************************************
  /// Returns the minimum value.
  /// User supplied compare function.
  //***************************************************************************
#if ETL_USING_CPP11
  template <typename TCompare, typename T>
  ETL_NODISCARD
  constexpr const T& multimin_compare(TCompare compare, const T& a, const T& b)
  {
    return compare(a, b) ? a : b;
  }

  template <typename TCompare, typename T, typename... Tx>
  ETL_NODISCARD
  constexpr const T& multimin_compare(TCompare compare, const T& t, const Tx&... tx)
  {
    return multimin_compare(compare, t, multimin_compare(compare, tx...));
  }
#endif

  //***************************************************************************
  /// Returns the iterator to the maximum value.
  //***************************************************************************
#if ETL_USING_CPP11
  template <typename TIterator>
  ETL_NODISCARD
  constexpr const TIterator& multimax_iter(const TIterator& a, const TIterator& b)
  {
    return *a < *b ? b : a;
  }

  template <typename TIterator, typename... TIteratorx>
  ETL_NODISCARD
  constexpr const TIterator& multimax_iter(const TIterator& t, const TIteratorx&... tx)
  {
    return multimax_iter(t, multimax_iter(tx...));
  }
#endif

  //***************************************************************************
  /// Returns the iterator to the maximum value.
  /// User supplied compare function.
  //***************************************************************************
#if ETL_USING_CPP11
  template <typename TCompare, typename TIterator>
  ETL_NODISCARD
  constexpr const TIterator& multimax_iter_compare(TCompare compare, const TIterator& a, const TIterator& b)
  {
    return compare(*a, *b) ? b : a;
  }

  template <typename TCompare, typename TIterator, typename... TIteratorx>
  ETL_NODISCARD
  constexpr const TIterator& multimax_iter_compare(TCompare compare, const TIterator& t, const TIteratorx&... tx)
  {
    return multimax_iter_compare(compare, t, multimax_iter_compare(compare, tx...));
  }
#endif

  //***************************************************************************
  /// Returns the iterator to the minimum value.
  //***************************************************************************
#if ETL_USING_CPP11
  template <typename TIterator>
  ETL_NODISCARD
  constexpr const TIterator& multimin_iter(const TIterator& a, const TIterator& b)
  {
    return *a < *b ? a : b;
  }

  template <typename TIterator, typename... Tx>
  ETL_NODISCARD
  constexpr const TIterator& multimin_iter(const TIterator& t, const Tx&... tx)
  {
    return multimin_iter(t, multimin_iter(tx...));
  }
#endif

  //***************************************************************************
  /// Returns the iterator to the minimum value.
  /// User supplied compare function.
  //***************************************************************************
#if ETL_USING_CPP11
  template <typename TCompare, typename TIterator>
  ETL_NODISCARD
  constexpr const TIterator& multimin_iter_compare(TCompare compare, const TIterator& a, const TIterator& b)
  {
    return compare(*a, *b) ? a : b;
  }

  template <typename TCompare, typename TIterator, typename... Tx>
  ETL_NODISCARD
  constexpr const TIterator& multimin_iter_compare(TCompare compare, const TIterator& t, const Tx&... tx)
  {
    return multimin_iter_compare(compare, t, multimin_iter_compare(compare, tx...));
  }
#endif

  //***************************************************************************
  /// partition
  /// For forward iterators only
  /// Does at most etl::distance(first, last) swaps.
  //***************************************************************************
  template <typename TIterator, typename TPredicate>
  ETL_CONSTEXPR14 typename etl::enable_if<etl::is_forward_iterator<TIterator>::value, TIterator>::type partition(TIterator first, TIterator last,
                                                                                                           TPredicate predicate)
  {
    first = etl::find_if_not(first, last, predicate);

    if (first == last)
    {
      return first;
    }

    for (TIterator i = etl::next(first); i != last; ++i)
    {
      if (predicate(*i))
      {
        using ETL_OR_STD::swap;
        swap(*i, *first);
        ++first;
      }
    }

    return first;
  }

  //***************************************************************************
  /// partition
  /// For iterators that support bidirectional iteration.
  /// Does at most (etl::distance(first, last) / 2) swaps.
  //***************************************************************************
  template <typename TIterator, typename TPredicate>
  ETL_CONSTEXPR14 typename etl::enable_if< etl::is_bidirectional_iterator_concept<TIterator>::value, TIterator>::type
    partition(TIterator first, TIterator last, TPredicate predicate)
  {
    while (first != last)
    {
      first = etl::find_if_not(first, last, predicate);

      if (first == last)
      {
        break;
      }

      last = etl::find_if(etl::reverse_iterator<TIterator>(last), etl::reverse_iterator<TIterator>(first), predicate).base();

      if (first == last)
      {
        break;
      }

      --last;
      using ETL_OR_STD::swap;
      swap(*first, *last);
      ++first;
    }

    return first;
  }

  //*********************************************************
  namespace private_algorithm
  {
    using ETL_OR_STD::swap;

    template <typename TIterator, typename TCompare>
#if (ETL_USING_CPP20 && ETL_USING_STL) || (ETL_USING_CPP14 && ETL_NOT_USING_STL && !defined(ETL_IN_UNIT_TEST))
    constexpr
#endif
      TIterator
      nth_partition(TIterator first, TIterator last, TCompare compare)
    {
      typedef typename etl::iterator_traits<TIterator>::value_type value_type;

      value_type pivot_value = ETL_MOVE(*last);

      TIterator i = first;

      for (TIterator j = first; j < last; ++j)
      {
        if (!compare(pivot_value, *j)) // Hack to get '*j <= pivot_value' in
                                       // terms of 'pivot_value < *j'
        {
          swap(*i, *j);
          ++i;
        }
      }

      swap(*i, *last);

      return i;
    }
  } // namespace private_algorithm

  //*********************************************************
  /// nth_element
  /// see https://en.cppreference.com/w/cpp/algorithm/nth_element
  //*********************************************************
#if ETL_USING_CPP11
  template <typename TIterator, typename TCompare = etl::less<typename etl::iterator_traits<TIterator>::value_type>>
  #if (ETL_USING_CPP20 && ETL_USING_STL) || (ETL_USING_CPP14 && ETL_NOT_USING_STL && !defined(ETL_IN_UNIT_TEST))
  constexpr
  #endif
    typename etl::enable_if< etl::is_random_access_iterator_concept<TIterator>::value, void>::type
    nth_element(TIterator first, TIterator nth, TIterator last, TCompare compare = TCompare())
  {
    if (first == last)
    {
      return;
    }

    // 'last' must point to the actual last value.
    --last;

    while (first <= last)
    {
      TIterator p = private_algorithm::nth_partition(first, last, compare);

      if (p == nth)
      {
        return;
      }
      else if (p > nth)
      {
        last = p - 1;
      }
      else
      {
        first = p + 1;
      }
    }
  }

#else

  //*********************************************************
  template <typename TIterator, typename TCompare>
  typename etl::enable_if< etl::is_random_access_iterator_concept<TIterator>::value, void>::type nth_element(TIterator first, TIterator nth,
                                                                                                             TIterator last, TCompare compare)
  {
    if (first == last)
    {
      return;
    }

    // 'last' must point to the actual last value.
    --last;

    while (first <= last)
    {
      TIterator p = private_algorithm::nth_partition(first, last, compare);

      if (p == nth)
      {
        return;
      }
      else if (p > nth)
      {
        last = p - 1;
      }
      else
      {
        first = p + 1;
      }
    }
  }

  //*********************************************************
  template <typename TIterator>
  typename etl::enable_if< etl::is_random_access_iterator_concept<TIterator>::value, void>::type nth_element(TIterator first, TIterator nth,
                                                                                                             TIterator last)
  {
    typedef etl::less<typename etl::iterator_traits<TIterator>::value_type> compare_t;

    nth_element(first, nth, last, compare_t());
  }
#endif

#if ETL_USING_CPP17

  namespace ranges
  {
    template <class I, class F>
    struct in_fun_result
    {
      ETL_NO_UNIQUE_ADDRESS I in;
      ETL_NO_UNIQUE_ADDRESS F fun;

      template <class I2, class F2>
      constexpr operator in_fun_result<I2, F2>() const&
      {
        return {in, fun};
      }

      template <class I2, class F2>
      constexpr operator in_fun_result<I2, F2>() &&
      {
        return {etl::move(in), etl::move(fun)};
      }
    };

    template <class I1, class I2>
    struct in_in_result
    {
      ETL_NO_UNIQUE_ADDRESS I1 in1;
      ETL_NO_UNIQUE_ADDRESS I2 in2;

      template <class II1, class II2>
      constexpr operator in_in_result<II1, II2>() const&
      {
        return {in1, in2};
      }

      template <class II1, class II2>
      constexpr operator in_in_result<II1, II2>() &&
      {
        return {etl::move(in1), etl::move(in2)};
      }
    };

    template <class I, class O>
    struct in_out_result
    {
      ETL_NO_UNIQUE_ADDRESS I in;
      ETL_NO_UNIQUE_ADDRESS O out;

      template <class I2, class O2>
      constexpr operator in_out_result<I2, O2>() const&
      {
        return {in, out};
      }

      template <class I2, class O2>
      constexpr operator in_out_result<I2, O2>() &&
      {
        return {etl::move(in), etl::move(out)};
      }
    };

    template <class I1, class I2, class O>
    struct in_in_out_result
    {
      ETL_NO_UNIQUE_ADDRESS I1 in1;
      ETL_NO_UNIQUE_ADDRESS I2 in2;
      ETL_NO_UNIQUE_ADDRESS O  out;

      template <class II1, class II2, class OO>
      constexpr operator in_in_out_result<II1, II2, OO>() const&
      {
        return {in1, in2, out};
      }

      template <class II1, class II2, class OO>
      constexpr operator in_in_out_result<II1, II2, OO>() &&
      {
        return {etl::move(in1), etl::move(in2), etl::move(out)};
      }
    };

    template <class I, class O1, class O2>
    struct in_out_out_result
    {
      ETL_NO_UNIQUE_ADDRESS I  in;
      ETL_NO_UNIQUE_ADDRESS O1 out1;
      ETL_NO_UNIQUE_ADDRESS O2 out2;

      template <class II, class OO1, class OO2>
      constexpr operator in_out_out_result<II, OO1, OO2>() const&
      {
        return {in, out1, out2};
      }

      template <class II, class OO1, class OO2>
      constexpr operator in_out_out_result<II, OO1, OO2>() &&
      {
        return {etl::move(in), etl::move(out1), etl::move(out2)};
      }
    };

    template <class I>
    struct in_found_result
    {
      ETL_NO_UNIQUE_ADDRESS I in;
      bool                    found;

      template <class I2>
      constexpr operator in_found_result<I2>() const&
      {
        return {in, found};
      }

      template <class I2>
      constexpr operator in_found_result<I2>() &&
      {
        return {etl::move(in), found};
      }
    };

    template <class O, class T>
    struct out_value_result
    {
      ETL_NO_UNIQUE_ADDRESS O out;
      ETL_NO_UNIQUE_ADDRESS T value;

      template <class O2, class T2>
      constexpr operator out_value_result<O2, T2>() const&
      {
        return {out, value};
      }

      template <class O2, class T2>
      constexpr operator out_value_result<O2, T2>() &&
      {
        return {etl::move(out), etl::move(value)};
      }
    };

    template <class O, class T>
    using iota_result = out_value_result<O, T>;

    template <class I, class O>
    using copy_result = in_out_result<I, O>;

    template <class I, class O>
    using copy_n_result = in_out_result<I, O>;

    template <class I, class O>
    using copy_if_result = in_out_result<I, O>;

    template <class I, class O>
    using copy_backward_result = in_out_result<I, O>;

    template <class I, class O>
    using uninitialized_copy_result = in_out_result<I, O>;

    template <class I, class O>
    using uninitialized_copy_n_result = in_out_result<I, O>;

    template <class I, class O>
    using uninitialized_move_result = in_out_result<I, O>;

    template <class I, class O>
    using uninitialized_move_n_result = in_out_result<I, O>;

    template <class I, class O>
    using move_result = in_out_result<I, O>;

    template <class I, class O>
    using move_backward_result = in_out_result<I, O>;

    template <class I1, class I2>
    using mismatch_result = in_in_result<I1, I2>;

    template <class I1, class I2>
    using swap_ranges_result = in_in_result<I1, I2>;

    template <class I, class O>
    using unary_transform_result = in_out_result<I, O>;

    template <class I1, class I2, class O>
    using binary_transform_result = in_in_out_result<I1, I2, O>;

    template <class I, class O>
    using replace_copy_result = in_out_result<I, O>;

    template <class I, class O>
    using replace_copy_if_result = in_out_result<I, O>;

    template <class I, class O>
    using remove_copy_result = in_out_result<I, O>;

    template <class I, class O>
    using unique_copy_result = in_out_result<I, O>;

    template <class I, class O>
    using rotate_copy_result = in_out_result<I, O>;

    template <class I, class O>
    using partial_sort_copy_result = in_out_result<I, O>;

    template <class I, class O1, class O2>
    using partition_copy_result = in_out_out_result<I, O1, O2>;

    template <class I1, class I2, class O>
    using set_union_result = in_in_out_result<I1, I2, O>;

    template <class I1, class I2, class O>
    using set_intersection_result = in_in_out_result<I1, I2, O>;

    template <class I, class O>
    using set_difference_result = in_out_result<I, O>;

    template <class I1, class I2, class O>
    using set_symmetric_difference_result = in_in_out_result<I1, I2, O>;

    template <class I1, class I2, class O>
    using merge_result = in_in_out_result<I1, I2, O>;

    template <class I>
    using next_permutation_result = in_found_result<I>;

    template <class I>
    using prev_permutation_result = in_found_result<I>;

    template <class T>
    struct min_max_result
    {
      T min;
      T max;

      template <class T2>
      constexpr operator min_max_result<T2>() const&
      {
        return {min, max};
      }

      template <class T2>
      constexpr operator min_max_result<T2>() &&
      {
        return {etl::move(min), etl::move(max)};
      }
    };

    template <class T>
    using minmax_result = min_max_result<T>;

    template <class I>
    using minmax_element_result = min_max_result<I>;

    template <class I, class F>
    using for_each_result = in_fun_result<I, F>;

    struct for_each_fn
    {
      template <class I, class S, class Fun, class Proj = etl::identity, typename = etl::enable_if_t<!etl::is_range_v<I>>>
      constexpr ranges::for_each_result<I, Fun> operator()(I first, S last, Fun f, Proj proj = {}) const
      {
        for (; first != last; ++first)
        {
          etl::invoke(f, etl::invoke(proj, *first));
        }
        return {etl::move(first), etl::move(f)};
      }

      template <class R, class Fun, class Proj = etl::identity, typename = etl::enable_if_t<etl::is_range_v<R>>>
      constexpr ranges::for_each_result<ranges::borrowed_iterator_t<R>, Fun> operator()(R&& r, Fun f, Proj proj = {}) const
      {
        return (*this)(ranges::begin(r), ranges::end(r), etl::move(f), etl::ref(proj));
      }
    };

    inline constexpr for_each_fn for_each;

    template <class I, class F>
    using for_each_n_result = in_fun_result<I, F>;

    struct for_each_n_fn
    {
      template <class I, class Fun, class Proj = etl::identity, typename = etl::enable_if_t<!etl::is_range_v<I>>>
      constexpr for_each_n_result<I, Fun> operator()(I first, etl::iter_difference_t<I> n, Fun fun, Proj proj = Proj{}) const
      {
        for (; n-- > 0; ++first)
        {
          etl::invoke(fun, etl::invoke(proj, *first));
        }
        return {etl::move(first), etl::move(fun)};
      }
    };

    inline constexpr for_each_n_fn for_each_n{};

    struct find_fn
    {
      template <class I, class S, class T, class Proj = etl::identity, typename = etl::enable_if_t<!etl::is_range_v<I>>>
      constexpr I operator()(I first, S last, const T& value, Proj proj = {}) const
      {
        for (; first != last; ++first)
        {
          if (etl::invoke(proj, *first) == value)
          {
            return first;
          }
        }
        return first;
      }

      template <class R, class T, class Proj = etl::identity, typename = etl::enable_if_t<etl::is_range_v<R>>>
      constexpr ranges::borrowed_iterator_t<R> operator()(R&& r, const T& value, Proj proj = {}) const
      {
        return (*this)(ranges::begin(r), ranges::end(r), value, etl::ref(proj));
      }
    };

    inline constexpr find_fn find;

    struct find_if_fn
    {
      template <class I, class S, class Pred, class Proj = etl::identity, typename = etl::enable_if_t<!etl::is_range_v<I>>>
      constexpr I operator()(I first, S last, Pred pred, Proj proj = {}) const
      {
        for (; first != last; ++first)
        {
          if (etl::invoke(pred, etl::invoke(proj, *first)))
          {
            return first;
          }
        }
        return first;
      }

      template <class R, class Pred, class Proj = etl::identity, typename = etl::enable_if_t<etl::is_range_v<R>>>
      constexpr ranges::borrowed_iterator_t<R> operator()(R&& r, Pred pred, Proj proj = {}) const
      {
        return (*this)(ranges::begin(r), ranges::end(r), etl::ref(pred), etl::ref(proj));
      }
    };

    inline constexpr find_if_fn find_if;

    struct find_if_not_fn
    {
      template <class I, class S, class Pred, class Proj = etl::identity, typename = etl::enable_if_t<!etl::is_range_v<I>>>
      constexpr I operator()(I first, S last, Pred pred, Proj proj = {}) const
      {
        for (; first != last; ++first)
        {
          if (!etl::invoke(pred, etl::invoke(proj, *first)))
          {
            return first;
          }
        }
        return first;
      }

      template <class R, class Pred, class Proj = etl::identity, typename = etl::enable_if_t<etl::is_range_v<R>>>
      constexpr ranges::borrowed_iterator_t<R> operator()(R&& r, Pred pred, Proj proj = {}) const
      {
        return (*this)(ranges::begin(r), ranges::end(r), etl::ref(pred), etl::ref(proj));
      }
    };

    inline constexpr find_if_not_fn find_if_not;

    struct search_fn
    {
      template <class I1, class S1, class I2, class S2, class Pred = ranges::equal_to, class Proj1 = etl::identity, class Proj2 = etl::identity,
                typename = etl::enable_if_t<!etl::is_range_v<I1>>>
      constexpr ranges::subrange<I1> operator()(I1 first1, S1 last1, I2 first2, S2 last2, Pred pred = {}, Proj1 proj1 = {}, Proj2 proj2 = {}) const
      {
        for (;; ++first1)
        {
          I1 it1 = first1;
          for (I2 it2 = first2;; ++it1, ++it2)
          {
            if (it2 == last2)
            {
              return {first1, it1};
            }
            if (it1 == last1)
            {
              return {it1, it1};
            }
            if (!etl::invoke(pred, etl::invoke(proj1, *it1), etl::invoke(proj2, *it2)))
            {
              break;
            }
          }
        }
      }

      template <class R1, class R2, class Pred = ranges::equal_to, class Proj1 = etl::identity, class Proj2 = etl::identity,
                typename = etl::enable_if_t<etl::is_range_v<R1>>>
      constexpr ranges::borrowed_subrange_t<R1> operator()(R1&& r1, R2&& r2, Pred pred = {}, Proj1 proj1 = {}, Proj2 proj2 = {}) const
      {
        return (*this)(ranges::begin(r1), ranges::end(r1), ranges::begin(r2), ranges::end(r2), etl::move(pred), etl::move(proj1), etl::move(proj2));
      }
    };

    inline constexpr search_fn search{};

    struct search_n_fn
    {
      template <class I, class S, class T = etl::iter_value_t<I>, class Pred = ranges::equal_to, class Proj = etl::identity,
                typename = etl::enable_if_t<!etl::is_range_v<I>>>
      constexpr ranges::subrange<I> operator()(I first, S last, etl::iter_difference_t<I> count, const T& value, Pred pred = {}, Proj proj = {}) const
      {
        if (count <= 0)
        {
          return {first, first};
        }

        for (; first != last; ++first)
        {
          if (etl::invoke(pred, etl::invoke(proj, *first), value))
          {
            I                         start = first;
            etl::iter_difference_t<I> n     = 1;

            for (;;)
            {
              if (n >= count)
              {
                return {start, ++first};
              }
              ++first;
              if (first == last)
              {
                return {first, first};
              }
              if (!etl::invoke(pred, etl::invoke(proj, *first), value))
              {
                break;
              }
              ++n;
            }
          }
        }
        return {first, first};
      }

      template <class R, class T = etl::ranges::range_value_t<R>, class Pred = ranges::equal_to, class Proj = etl::identity,
                typename = etl::enable_if_t<etl::is_range_v<R>>>
      constexpr ranges::borrowed_subrange_t<R> operator()(R&& r, ranges::range_difference_t<R> count, const T& value, Pred pred = {},
                                                          Proj proj = {}) const
      {
        return (*this)(ranges::begin(r), ranges::end(r), count, value, etl::move(pred), etl::move(proj));
      }
    };

    inline constexpr search_n_fn search_n{};

    struct find_end_fn
    {
      template <class I1, class S1, class I2, class S2, class Pred = ranges::equal_to, class Proj1 = etl::identity, class Proj2 = etl::identity,
                typename = etl::enable_if_t<!etl::is_range_v<I1>>>
      constexpr ranges::subrange<I1> operator()(I1 first1, S1 last1, I2 first2, S2 last2, Pred pred = {}, Proj1 proj1 = {}, Proj2 proj2 = {}) const
      {
        if (first2 == last2)
        {
          auto last_it = ranges::next(first1, last1);
          return {last_it, last_it};
        }
        auto result = ranges::search(etl::move(first1), last1, first2, last2, pred, proj1, proj2);

        if (result.empty())
        {
          return result;
        }

        for (;;)
        {
          auto new_result = ranges::search(etl::next(result.begin()), last1, first2, last2, pred, proj1, proj2);
          if (new_result.empty())
          {
            return result;
          }
          else
          {
            result = etl::move(new_result);
          }
        }
      }

      template <class R1, class R2, class Pred = ranges::equal_to, class Proj1 = etl::identity, class Proj2 = etl::identity,
                typename = etl::enable_if_t<etl::is_range_v<R1>>>
      constexpr ranges::borrowed_subrange_t<R1> operator()(R1&& r1, R2&& r2, Pred pred = {}, Proj1 proj1 = {}, Proj2 proj2 = {}) const
      {
        return (*this)(ranges::begin(r1), ranges::end(r1), ranges::begin(r2), ranges::end(r2), etl::move(pred), etl::move(proj1), etl::move(proj2));
      }
    };

    inline constexpr find_end_fn find_end{};

    struct find_first_of_fn
    {
      template <class I1, class S1, class I2, class S2, class Pred = ranges::equal_to, class Proj1 = etl::identity, class Proj2 = etl::identity,
                typename = etl::enable_if_t<!etl::is_range_v<I1>>>
      constexpr I1 operator()(I1 first1, S1 last1, I2 first2, S2 last2, Pred pred = {}, Proj1 proj1 = {}, Proj2 proj2 = {}) const
      {
        for (; first1 != last1; ++first1)
        {
          for (auto i = first2; i != last2; ++i)
          {
            if (etl::invoke(pred, etl::invoke(proj1, *first1), etl::invoke(proj2, *i)))
            {
              return first1;
            }
          }
        }
        return first1;
      }

      template <class R1, class R2, class Pred = ranges::equal_to, class Proj1 = etl::identity, class Proj2 = etl::identity,
                typename = etl::enable_if_t<etl::is_range_v<R1>>>
      constexpr ranges::borrowed_iterator_t<R1> operator()(R1&& r1, R2&& r2, Pred pred = {}, Proj1 proj1 = {}, Proj2 proj2 = {}) const
      {
        return (*this)(ranges::begin(r1), ranges::end(r1), ranges::begin(r2), ranges::end(r2), etl::move(pred), etl::move(proj1), etl::move(proj2));
      }
    };

    inline constexpr find_first_of_fn find_first_of{};

    struct adjacent_find_fn
    {
      template <class I, class S, class Proj = etl::identity, class Pred = ranges::equal_to, typename = etl::enable_if_t<!etl::is_range_v<I>>>
      constexpr I operator()(I first, S last, Pred pred = {}, Proj proj = {}) const
      {
        if (first == last)
        {
          return first;
        }
        auto next_it = ranges::next(first);
        for (; next_it != last; ++next_it, ++first)
        {
          if (etl::invoke(pred, etl::invoke(proj, *first), etl::invoke(proj, *next_it)))
          {
            return first;
          }
        }
        return next_it;
      }

      template <class R, class Proj = etl::identity, class Pred = ranges::equal_to, typename = etl::enable_if_t<etl::is_range_v<R>>>
      constexpr ranges::borrowed_iterator_t<R> operator()(R&& r, Pred pred = {}, Proj proj = {}) const
      {
        return (*this)(ranges::begin(r), ranges::end(r), etl::ref(pred), etl::ref(proj));
      }
    };

    inline constexpr adjacent_find_fn adjacent_find;

    struct count_fn
    {
      template <class I, class S, class Proj = etl::identity, class T = etl::projected_value_t<I, Proj>,
                typename = etl::enable_if_t<!etl::is_range_v<I>>>
      constexpr etl::iter_difference_t<I> operator()(I first, S last, const T& value, Proj proj = {}) const
      {
        etl::iter_difference_t<I> counter = 0;
        for (; first != last; ++first)
        {
          if (etl::invoke(proj, *first) == value)
          {
            ++counter;
          }
        }
        return counter;
      }

      template <class R, class Proj = etl::identity, class T = etl::projected_value_t<ranges::iterator_t<R>, Proj>,
                typename = etl::enable_if_t<etl::is_range_v<R>>>
      constexpr ranges::range_difference_t<R> operator()(R&& r, const T& value, Proj proj = {}) const
      {
        return (*this)(ranges::begin(r), ranges::end(r), value, etl::ref(proj));
      }
    };

    inline constexpr count_fn count;

    struct count_if_fn
    {
      template <class I, class S, class Pred, class Proj = etl::identity, typename = etl::enable_if_t<!etl::is_range_v<I>>>
      constexpr etl::iter_difference_t<I> operator()(I first, S last, Pred pred, Proj proj = {}) const
      {
        etl::iter_difference_t<I> counter = 0;
        for (; first != last; ++first)
        {
          if (etl::invoke(pred, etl::invoke(proj, *first)))
          {
            ++counter;
          }
        }
        return counter;
      }

      template <class R, class Proj = etl::identity, class Pred, typename = etl::enable_if_t<etl::is_range_v<R>>>
      constexpr ranges::range_difference_t<R> operator()(R&& r, Pred pred, Proj proj = {}) const
      {
        return (*this)(ranges::begin(r), ranges::end(r), etl::ref(pred), etl::ref(proj));
      }
    };

    inline constexpr count_if_fn count_if;

    struct all_of_fn
    {
      template <class I, class S, class Pred, class Proj = etl::identity, typename = etl::enable_if_t<!etl::is_range_v<I>>>
      constexpr bool operator()(I first, S last, Pred pred, Proj proj = {}) const
      {
        return ranges::find_if_not(first, last, etl::ref(pred), etl::ref(proj)) == last;
      }

      template <class R, class Pred, class Proj = etl::identity, typename = etl::enable_if_t<etl::is_range_v<R>>>
      constexpr bool operator()(R&& r, Pred pred, Proj proj = {}) const
      {
        return operator()(ranges::begin(r), ranges::end(r), etl::ref(pred), etl::ref(proj));
      }
    };

    inline constexpr all_of_fn all_of;

    struct any_of_fn
    {
      template <class I, class S, class Pred, class Proj = etl::identity, typename = etl::enable_if_t<!etl::is_range_v<I>>>
      constexpr bool operator()(I first, S last, Pred pred, Proj proj = {}) const
      {
        return ranges::find_if(first, last, etl::ref(pred), etl::ref(proj)) != last;
      }

      template <class R, class Pred, class Proj = etl::identity, typename = etl::enable_if_t<etl::is_range_v<R>>>
      constexpr bool operator()(R&& r, Pred pred, Proj proj = {}) const
      {
        return operator()(ranges::begin(r), ranges::end(r), etl::ref(pred), etl::ref(proj));
      }
    };

    inline constexpr any_of_fn any_of;

    struct none_of_fn
    {
      template <class I, class S, class Pred, class Proj = etl::identity, typename = etl::enable_if_t<!etl::is_range_v<I>>>
      constexpr bool operator()(I first, S last, Pred pred, Proj proj = {}) const
      {
        return ranges::find_if(first, last, etl::ref(pred), etl::ref(proj)) == last;
      }

      template <class R, class Pred, class Proj = etl::identity, typename = etl::enable_if_t<etl::is_range_v<R>>>
      constexpr bool operator()(R&& r, Pred pred, Proj proj = {}) const
      {
        return operator()(ranges::begin(r), ranges::end(r), etl::ref(pred), etl::ref(proj));
      }
    };

    inline constexpr none_of_fn none_of;

    struct mismatch_fn
    {
      template <class I1, class S1, class I2, class S2, class Pred = ranges::equal_to, class Proj1 = etl::identity, class Proj2 = etl::identity,
                typename = etl::enable_if_t<!etl::is_range_v<I1>>>
      constexpr ranges::mismatch_result<I1, I2> operator()(I1 first1, S1 last1, I2 first2, S2 last2, Pred pred = {}, Proj1 proj1 = {},
                                                           Proj2 proj2 = {}) const
      {
        for (; first1 != last1 && first2 != last2; ++first1, ++first2)
        {
          if (!etl::invoke(pred, etl::invoke(proj1, *first1), etl::invoke(proj2, *first2)))
          {
            break;
          }
        }
        return {etl::move(first1), etl::move(first2)};
      }

      template <class R1, class R2, class Pred = ranges::equal_to, class Proj1 = etl::identity, class Proj2 = etl::identity,
                typename = etl::enable_if_t<etl::is_range_v<R1>>>
      constexpr ranges::mismatch_result<ranges::borrowed_iterator_t<R1>, ranges::borrowed_iterator_t<R2>>
        operator()(R1&& r1, R2&& r2, Pred pred = {}, Proj1 proj1 = {}, Proj2 proj2 = {}) const
      {
        return (*this)(ranges::begin(r1), ranges::end(r1), ranges::begin(r2), ranges::end(r2), etl::move(pred), etl::move(proj1), etl::move(proj2));
      }
    };

    inline constexpr mismatch_fn mismatch{};

    struct equal_fn
    {
      template <class I1, class S1, class I2, class S2, class Pred = ranges::equal_to, class Proj1 = etl::identity, class Proj2 = etl::identity,
                typename = etl::enable_if_t<!etl::is_range_v<I1>>>
      constexpr bool operator()(I1 first1, S1 last1, I2 first2, S2 last2, Pred pred = {}, Proj1 proj1 = {}, Proj2 proj2 = {}) const
      {
        for (; first1 != last1 && first2 != last2; ++first1, ++first2)
        {
          if (!etl::invoke(pred, etl::invoke(proj1, *first1), etl::invoke(proj2, *first2)))
          {
            return false;
          }
        }
        return first1 == last1 && first2 == last2;
      }

      template <class R1, class R2, class Pred = ranges::equal_to, class Proj1 = etl::identity, class Proj2 = etl::identity,
                typename = etl::enable_if_t<etl::is_range_v<R1>>>
      constexpr bool operator()(R1&& r1, R2&& r2, Pred pred = {}, Proj1 proj1 = {}, Proj2 proj2 = {}) const
      {
        return (*this)(ranges::begin(r1), ranges::end(r1), ranges::begin(r2), ranges::end(r2), etl::move(pred), etl::move(proj1), etl::move(proj2));
      }
    };

    inline constexpr equal_fn equal{};

    struct is_permutation_fn
    {
      template <class I1, class S1, class I2, class S2, class Pred = ranges::equal_to, class Proj1 = etl::identity, class Proj2 = etl::identity,
                typename = etl::enable_if_t<!etl::is_range_v<I1>>>
      constexpr bool operator()(I1 first1, S1 last1, I2 first2, S2 last2, Pred pred = {}, Proj1 proj1 = {}, Proj2 proj2 = {}) const
      {
        // Skip common prefix
        for (; first1 != last1 && first2 != last2; ++first1, ++first2)
        {
          if (!etl::invoke(pred, etl::invoke(proj1, *first1), etl::invoke(proj2, *first2)))
          {
            break;
          }
        }

        if (first1 == last1)
        {
          return first2 == last2;
        }

        if (first2 == last2)
        {
          return false;
        }

        // Check remaining elements
        for (I1 i = first1; i != last1; ++i)
        {
          // Check if we already counted this element
          bool already_seen = false;
          for (I1 j = first1; j != i; ++j)
          {
            if (etl::invoke(pred, etl::invoke(proj1, *i), etl::invoke(proj1, *j)))
            {
              already_seen = true;
              break;
            }
          }

          if (already_seen)
          {
            continue;
          }

          // Count occurrences in range2
          auto count2 = etl::iter_difference_t<I2>(0);
          for (I2 k = first2; k != last2; ++k)
          {
            if (etl::invoke(pred, etl::invoke(proj1, *i), etl::invoke(proj2, *k)))
            {
              ++count2;
            }
          }

          if (count2 == 0)
          {
            return false;
          }

          // Count occurrences in range1
          auto count1 = etl::iter_difference_t<I1>(0);
          for (I1 k = i; k != last1; ++k)
          {
            if (etl::invoke(pred, etl::invoke(proj1, *i), etl::invoke(proj1, *k)))
            {
              ++count1;
            }
          }

          if (count1 != count2)
          {
            return false;
          }
        }

        return true;
      }

      template <class R1, class R2, class Pred = ranges::equal_to, class Proj1 = etl::identity, class Proj2 = etl::identity,
                typename = etl::enable_if_t<etl::is_range_v<R1>>>
      constexpr bool operator()(R1&& r1, R2&& r2, Pred pred = {}, Proj1 proj1 = {}, Proj2 proj2 = {}) const
      {
        return (*this)(ranges::begin(r1), ranges::end(r1), ranges::begin(r2), ranges::end(r2), etl::move(pred), etl::move(proj1), etl::move(proj2));
      }
    };

    inline constexpr is_permutation_fn is_permutation{};

    struct starts_with_fn
    {
      template <class I1, class S1, class I2, class S2, class Pred = ranges::equal_to, class Proj1 = etl::identity, class Proj2 = etl::identity,
                typename = etl::enable_if_t<!etl::is_range_v<I1>>>
      constexpr bool operator()(I1 first1, S1 last1, I2 first2, S2 last2, Pred pred = {}, Proj1 proj1 = {}, Proj2 proj2 = {}) const
      {
        for (; first2 != last2; ++first1, ++first2)
        {
          if (first1 == last1 || !etl::invoke(pred, etl::invoke(proj1, *first1), etl::invoke(proj2, *first2)))
          {
            return false;
          }
        }
        return true;
      }

      template <class R1, class R2, class Pred = ranges::equal_to, class Proj1 = etl::identity, class Proj2 = etl::identity,
                typename = etl::enable_if_t<etl::is_range_v<R1>>>
      constexpr bool operator()(R1&& r1, R2&& r2, Pred pred = {}, Proj1 proj1 = {}, Proj2 proj2 = {}) const
      {
        return (*this)(ranges::begin(r1), ranges::end(r1), ranges::begin(r2), ranges::end(r2), etl::move(pred), etl::move(proj1), etl::move(proj2));
      }
    };

    inline constexpr starts_with_fn starts_with{};

    struct ends_with_fn
    {
      template <class I1, class S1, class I2, class S2, class Pred = ranges::equal_to, class Proj1 = etl::identity, class Proj2 = etl::identity,
                typename = etl::enable_if_t<!etl::is_range_v<I1>>>
      constexpr bool operator()(I1 first1, S1 last1, I2 first2, S2 last2, Pred pred = {}, Proj1 proj1 = {}, Proj2 proj2 = {}) const
      {
        auto n1 = etl::distance(first1, last1);
        auto n2 = etl::distance(first2, last2);

        if (n2 > n1)
        {
          return false;
        }

        ranges::advance(first1, n1 - n2);

        return starts_with(first1, last1, first2, last2, etl::move(pred), etl::move(proj1), etl::move(proj2));
      }

      template <class R1, class R2, class Pred = ranges::equal_to, class Proj1 = etl::identity, class Proj2 = etl::identity,
                typename = etl::enable_if_t<etl::is_range_v<R1>>>
      constexpr bool operator()(R1&& r1, R2&& r2, Pred pred = {}, Proj1 proj1 = {}, Proj2 proj2 = {}) const
      {
        return (*this)(ranges::begin(r1), ranges::end(r1), ranges::begin(r2), ranges::end(r2), etl::move(pred), etl::move(proj1), etl::move(proj2));
      }
    };

    inline constexpr ends_with_fn ends_with{};

    struct lexicographical_compare_fn
    {
      template <class I1, class S1, class I2, class S2, class Comp = ranges::less, class Proj1 = etl::identity, class Proj2 = etl::identity,
                typename = etl::enable_if_t<!etl::is_range_v<I1>>>
      constexpr bool operator()(I1 first1, S1 last1, I2 first2, S2 last2, Comp comp = {}, Proj1 proj1 = {}, Proj2 proj2 = {}) const
      {
        for (; first1 != last1 && first2 != last2; ++first1, ++first2)
        {
          if (etl::invoke(comp, etl::invoke(proj1, *first1), etl::invoke(proj2, *first2)))
          {
            return true;
          }

          if (etl::invoke(comp, etl::invoke(proj2, *first2), etl::invoke(proj1, *first1)))
          {
            return false;
          }
        }
        return first1 == last1 && first2 != last2;
      }

      template <class R1, class R2, class Comp = ranges::less, class Proj1 = etl::identity, class Proj2 = etl::identity,
                typename = etl::enable_if_t<etl::is_range_v<R1>>>
      constexpr bool operator()(R1&& r1, R2&& r2, Comp comp = {}, Proj1 proj1 = {}, Proj2 proj2 = {}) const
      {
        return (*this)(ranges::begin(r1), ranges::end(r1), ranges::begin(r2), ranges::end(r2), etl::move(comp), etl::move(proj1), etl::move(proj2));
      }
    };

    inline constexpr lexicographical_compare_fn lexicographical_compare{};

    template <class I, class T>
    struct in_value_result
    {
      ETL_NO_UNIQUE_ADDRESS I in;
      ETL_NO_UNIQUE_ADDRESS T value;

      template <class I2, class T2>
      constexpr operator in_value_result<I2, T2>() const&
      {
        return {in, value};
      }

      template <class I2, class T2>
      constexpr operator in_value_result<I2, T2>() &&
      {
        return {etl::move(in), etl::move(value)};
      }
    };

    template <class I, class T>
    using fold_left_with_iter_result = in_value_result<I, T>;

    struct fold_left_fn
    {
      template <class I, class S, class T, class F, typename = etl::enable_if_t<!etl::is_range_v<I>>>
      constexpr auto operator()(I first, S last, T init, F f) const -> etl::decay_t<etl::invoke_result_t<F&, T, etl::iter_reference_t<I>>>
      {
        using U = etl::decay_t<etl::invoke_result_t<F&, T, etl::iter_reference_t<I>>>;
        U accum = etl::move(init);
        for (; first != last; ++first)
        {
          accum = etl::invoke(f, etl::move(accum), *first);
        }
        return accum;
      }

      template <class R, class T, class F, typename = etl::enable_if_t<etl::is_range_v<R>>>
      constexpr auto operator()(R&& r, T init, F f) const -> etl::decay_t< etl::invoke_result_t<F&, T, etl::ranges::range_reference_t<R>>>
      {
        return (*this)(ranges::begin(r), ranges::end(r), etl::move(init), etl::move(f));
      }
    };

    inline constexpr fold_left_fn fold_left{};

    struct fold_left_with_iter_fn
    {
      template <class I, class S, class T, class F, typename = etl::enable_if_t<!etl::is_range_v<I>>>
      constexpr auto operator()(I first, S last, T init,
                                F f) const -> fold_left_with_iter_result< I, etl::decay_t<etl::invoke_result_t<F&, T, etl::iter_reference_t<I>>>>
      {
        using U = etl::decay_t<etl::invoke_result_t<F&, T, etl::iter_reference_t<I>>>;
        U accum = etl::move(init);
        for (; first != last; ++first)
        {
          accum = etl::invoke(f, etl::move(accum), *first);
        }
        return {etl::move(first), etl::move(accum)};
      }

      template <class R, class T, class F, typename = etl::enable_if_t<etl::is_range_v<R>>>
      constexpr auto operator()(R&& r, T init, F f) const
        -> fold_left_with_iter_result< ranges::borrowed_iterator_t<R>, etl::decay_t< etl::invoke_result_t<F&, T, etl::ranges::range_reference_t<R>>>>
      {
        return (*this)(ranges::begin(r), ranges::end(r), etl::move(init), etl::move(f));
      }
    };

    inline constexpr fold_left_with_iter_fn fold_left_with_iter{};

    struct fold_left_first_fn
    {
      template <class I, class S, class F, typename = etl::enable_if_t<!etl::is_range_v<I>>>
      constexpr auto operator()(I first, S last, F f) const -> etl::decay_t<etl::invoke_result_t<F&, etl::iter_value_t<I>, etl::iter_reference_t<I>>>
      {
        using U = etl::decay_t<etl::invoke_result_t<F&, etl::iter_value_t<I>, etl::iter_reference_t<I>>>;
        if (first == last)
        {
          return U{};
        }
        U accum = *first;
        ++first;
        for (; first != last; ++first)
        {
          accum = etl::invoke(f, etl::move(accum), *first);
        }
        return accum;
      }

      template <class R, class F, typename = etl::enable_if_t<etl::is_range_v<R>>>
      constexpr auto operator()(R&& r,
                                F f) const -> etl::decay_t<etl::invoke_result_t<F&, etl::ranges::range_value_t<R>, etl::ranges::range_reference_t<R>>>
      {
        return (*this)(ranges::begin(r), ranges::end(r), etl::move(f));
      }
    };

    inline constexpr fold_left_first_fn fold_left_first{};

    struct fold_left_first_with_iter_fn
    {
      template <class I, class S, class F, typename = etl::enable_if_t<!etl::is_range_v<I>>>
      constexpr auto operator()(I first, S last, F f) const
        -> fold_left_with_iter_result< I, etl::decay_t<etl::invoke_result_t<F&, etl::iter_value_t<I>, etl::iter_reference_t<I>>>>
      {
        using U = etl::decay_t<etl::invoke_result_t<F&, etl::iter_value_t<I>, etl::iter_reference_t<I>>>;
        if (first == last)
        {
          return {etl::move(first), U{}};
        }
        U accum = *first;
        ++first;
        for (; first != last; ++first)
        {
          accum = etl::invoke(f, etl::move(accum), *first);
        }
        return {etl::move(first), etl::move(accum)};
      }

      template <class R, class F, typename = etl::enable_if_t<etl::is_range_v<R>>>
      constexpr auto operator()(R&& r, F f) const
        -> fold_left_with_iter_result< ranges::borrowed_iterator_t<R>,
                                       etl::decay_t<etl::invoke_result_t<F&, etl::ranges::range_value_t<R>, etl::ranges::range_reference_t<R>>>>
      {
        return (*this)(ranges::begin(r), ranges::end(r), etl::move(f));
      }
    };

    inline constexpr fold_left_first_with_iter_fn fold_left_first_with_iter{};

    struct fold_right_fn
    {
      template <class I, class S, class T, class F, typename = etl::enable_if_t<!etl::is_range_v<I>>>
      constexpr auto operator()(I first, S last, T init, F f) const -> etl::decay_t<etl::invoke_result_t<F&, etl::iter_reference_t<I>, T>>
      {
        using U = etl::decay_t<etl::invoke_result_t<F&, etl::iter_reference_t<I>, T>>;
        U accum = etl::move(init);
        I tail  = ranges::next(first, last);
        while (tail != first)
        {
          tail  = ranges::prev(tail);
          accum = etl::invoke(f, *tail, etl::move(accum));
        }
        return accum;
      }

      template <class R, class T, class F, typename = etl::enable_if_t<etl::is_range_v<R>>>
      constexpr auto operator()(R&& r, T init, F f) const -> etl::decay_t< etl::invoke_result_t<F&, etl::ranges::range_reference_t<R>, T>>
      {
        return (*this)(ranges::begin(r), ranges::end(r), etl::move(init), etl::move(f));
      }
    };

    inline constexpr fold_right_fn fold_right{};

    struct fold_right_last_fn
    {
      template <class I, class S, class F, typename = etl::enable_if_t<!etl::is_range_v<I>>>
      constexpr auto operator()(I first, S last, F f) const -> etl::decay_t<etl::invoke_result_t<F&, etl::iter_reference_t<I>, etl::iter_value_t<I>>>
      {
        using U = etl::decay_t<etl::invoke_result_t<F&, etl::iter_reference_t<I>, etl::iter_value_t<I>>>;
        I tail  = ranges::next(first, last);
        if (tail == first)
        {
          return U{};
        }
        tail    = ranges::prev(tail);
        U accum = *tail;
        while (tail != first)
        {
          tail  = ranges::prev(tail);
          accum = etl::invoke(f, *tail, etl::move(accum));
        }
        return accum;
      }

      template <class R, class F, typename = etl::enable_if_t<etl::is_range_v<R>>>
      constexpr auto
        operator()(R&& r, F f) const -> etl::decay_t<etl::invoke_result_t< F&, etl::ranges::range_reference_t<R>, etl::ranges::range_value_t<R>>>
      {
        return (*this)(ranges::begin(r), ranges::end(r), etl::move(f));
      }
    };

    inline constexpr fold_right_last_fn fold_right_last{};

    struct copy_fn
    {
      template <class I, class S, class O, typename = etl::enable_if_t<!etl::is_range_v<I>>>
      constexpr ranges::copy_result<I, O> operator()(I first, S last, O result) const
      {
        for (; first != last; ++first, ++result)
        {
          *result = *first;
        }
        return {etl::move(first), etl::move(result)};
      }

      template <class R, class O, typename = etl::enable_if_t<etl::is_range_v<R>>>
      constexpr ranges::copy_result<ranges::borrowed_iterator_t<R>, O> operator()(R&& r, O result) const
      {
        return (*this)(ranges::begin(r), ranges::end(r), etl::move(result));
      }
    };

    inline constexpr copy_fn copy{};

    struct copy_n_fn
    {
      template <class I, class O>
      constexpr ranges::copy_n_result<I, O> operator()(I first, etl::iter_difference_t<I> n, O result) const
      {
        for (etl::iter_difference_t<I> i = 0; i < n; ++i, ++first, ++result)
        {
          *result = *first;
        }
        return {etl::move(first), etl::move(result)};
      }
    };

    inline constexpr copy_n_fn copy_n{};

    struct copy_if_fn
    {
      template <class I, class S, class O, class Pred, class Proj = etl::identity, typename = etl::enable_if_t<!etl::is_range_v<I>>>
      constexpr ranges::copy_if_result<I, O> operator()(I first, S last, O result, Pred pred, Proj proj = {}) const
      {
        for (; first != last; ++first)
        {
          if (etl::invoke(pred, etl::invoke(proj, *first)))
          {
            *result = *first;
            ++result;
          }
        }
        return {etl::move(first), etl::move(result)};
      }

      template <class R, class O, class Pred, class Proj = etl::identity, typename = etl::enable_if_t<etl::is_range_v<R>>>
      constexpr ranges::copy_if_result<ranges::borrowed_iterator_t<R>, O> operator()(R&& r, O result, Pred pred, Proj proj = {}) const
      {
        return (*this)(ranges::begin(r), ranges::end(r), etl::move(result), etl::ref(pred), etl::ref(proj));
      }
    };

    inline constexpr copy_if_fn copy_if{};

    struct copy_backward_fn
    {
      template <class I1, class S1, class I2, typename = etl::enable_if_t<!etl::is_range_v<I1>>>
      constexpr ranges::copy_backward_result<I1, I2> operator()(I1 first, S1 last, I2 result) const
      {
        I1 last_it = ranges::next(first, last);
        I1 tail    = last_it;
        while (first != tail)
        {
          *(--result) = *(--tail);
        }
        return {etl::move(last_it), etl::move(result)};
      }

      template <class R, class I, typename = etl::enable_if_t<etl::is_range_v<R>>>
      constexpr ranges::copy_backward_result<ranges::borrowed_iterator_t<R>, I> operator()(R&& r, I result) const
      {
        return (*this)(ranges::begin(r), ranges::end(r), etl::move(result));
      }
    };

    inline constexpr copy_backward_fn copy_backward{};

    struct move_fn
    {
      template <class I, class S, class O, typename = etl::enable_if_t<!etl::is_range_v<I>>>
      constexpr ranges::move_result<I, O> operator()(I first, S last, O result) const
      {
        for (; first != last; ++first, ++result)
        {
          *result = etl::move(*first);
        }
        return {etl::move(first), etl::move(result)};
      }

      template <class R, class O, typename = etl::enable_if_t<etl::is_range_v<R>>>
      constexpr ranges::move_result<ranges::borrowed_iterator_t<R>, O> operator()(R&& r, O result) const
      {
        return (*this)(ranges::begin(r), ranges::end(r), etl::move(result));
      }
    };

    inline constexpr move_fn move{};

    struct move_backward_fn
    {
      template <class I1, class S1, class I2, typename = etl::enable_if_t<!etl::is_range_v<I1>>>
      constexpr ranges::move_backward_result<I1, I2> operator()(I1 first, S1 last, I2 result) const
      {
        I1 last_it = ranges::next(first, last);
        I1 tail    = last_it;
        while (first != tail)
        {
          *(--result) = etl::move(*(--tail));
        }
        return {etl::move(last_it), etl::move(result)};
      }

      template <class R, class I, typename = etl::enable_if_t<etl::is_range_v<R>>>
      constexpr ranges::move_backward_result<ranges::borrowed_iterator_t<R>, I> operator()(R&& r, I result) const
      {
        return (*this)(ranges::begin(r), ranges::end(r), etl::move(result));
      }
    };

    inline constexpr move_backward_fn move_backward{};

    struct swap_ranges_fn
    {
      template <class I1, class S1, class I2, class S2, typename = etl::enable_if_t<!etl::is_range_v<I1>>>
      constexpr ranges::swap_ranges_result<I1, I2> operator()(I1 first1, S1 last1, I2 first2, S2 last2) const
      {
        for (; first1 != last1 && first2 != last2; ++first1, ++first2)
        {
          etl::iter_swap(first1, first2);
        }
        return {etl::move(first1), etl::move(first2)};
      }

      template <class R1, class R2, typename = etl::enable_if_t<etl::is_range_v<R1>>>
      constexpr ranges::swap_ranges_result<ranges::borrowed_iterator_t<R1>, ranges::borrowed_iterator_t<R2>> operator()(R1&& r1, R2&& r2) const
      {
        return (*this)(ranges::begin(r1), ranges::end(r1), ranges::begin(r2), ranges::end(r2));
      }
    };

    inline constexpr swap_ranges_fn swap_ranges{};

    struct replace_fn
    {
      template <class I, class S, class T1, class T2, class Proj = etl::identity, typename = etl::enable_if_t<!etl::is_range_v<I>>>
      constexpr I operator()(I first, S last, const T1& old_value, const T2& new_value, Proj proj = {}) const
      {
        for (; first != last; ++first)
        {
          if (etl::invoke(proj, *first) == old_value)
          {
            *first = new_value;
          }
        }
        return first;
      }

      template <class R, class T1, class T2, class Proj = etl::identity, typename = etl::enable_if_t<etl::is_range_v<R>>>
      constexpr ranges::borrowed_iterator_t<R> operator()(R&& r, const T1& old_value, const T2& new_value, Proj proj = {}) const
      {
        return (*this)(ranges::begin(r), ranges::end(r), old_value, new_value, etl::ref(proj));
      }
    };

    inline constexpr replace_fn replace{};

    struct replace_if_fn
    {
      template <class I, class S, class Pred, class T, class Proj = etl::identity, typename = etl::enable_if_t<!etl::is_range_v<I>>>
      constexpr I operator()(I first, S last, Pred pred, const T& new_value, Proj proj = {}) const
      {
        for (; first != last; ++first)
        {
          if (etl::invoke(pred, etl::invoke(proj, *first)))
          {
            *first = new_value;
          }
        }
        return first;
      }

      template <class R, class Pred, class T, class Proj = etl::identity, typename = etl::enable_if_t<etl::is_range_v<R>>>
      constexpr ranges::borrowed_iterator_t<R> operator()(R&& r, Pred pred, const T& new_value, Proj proj = {}) const
      {
        return (*this)(ranges::begin(r), ranges::end(r), etl::ref(pred), new_value, etl::ref(proj));
      }
    };

    inline constexpr replace_if_fn replace_if{};

    struct replace_copy_fn
    {
      template <class I, class S, class O, class T1, class T2, class Proj = etl::identity, typename = etl::enable_if_t<!etl::is_range_v<I>>>
      constexpr ranges::replace_copy_result<I, O> operator()(I first, S last, O result, const T1& old_value, const T2& new_value,
                                                             Proj proj = {}) const
      {
        for (; first != last; ++first, ++result)
        {
          if (etl::invoke(proj, *first) == old_value)
          {
            *result = new_value;
          }
          else
          {
            *result = *first;
          }
        }
        return {etl::move(first), etl::move(result)};
      }

      template <class R, class O, class T1, class T2, class Proj = etl::identity, typename = etl::enable_if_t<etl::is_range_v<R>>>
      constexpr ranges::replace_copy_result<ranges::borrowed_iterator_t<R>, O> operator()(R&& r, O result, const T1& old_value, const T2& new_value,
                                                                                          Proj proj = {}) const
      {
        return (*this)(ranges::begin(r), ranges::end(r), etl::move(result), old_value, new_value, etl::ref(proj));
      }
    };

    inline constexpr replace_copy_fn replace_copy{};

    struct replace_copy_if_fn
    {
      template <class I, class S, class O, class Pred, class T, class Proj = etl::identity, typename = etl::enable_if_t<!etl::is_range_v<I>>>
      constexpr ranges::replace_copy_if_result<I, O> operator()(I first, S last, O result, Pred pred, const T& new_value, Proj proj = {}) const
      {
        for (; first != last; ++first, ++result)
        {
          if (etl::invoke(pred, etl::invoke(proj, *first)))
          {
            *result = new_value;
          }
          else
          {
            *result = *first;
          }
        }
        return {etl::move(first), etl::move(result)};
      }

      template <class R, class O, class Pred, class T, class Proj = etl::identity, typename = etl::enable_if_t<etl::is_range_v<R>>>
      constexpr ranges::replace_copy_if_result<ranges::borrowed_iterator_t<R>, O> operator()(R&& r, O result, Pred pred, const T& new_value,
                                                                                             Proj proj = {}) const
      {
        return (*this)(ranges::begin(r), ranges::end(r), etl::move(result), etl::ref(pred), new_value, etl::ref(proj));
      }
    };

    inline constexpr replace_copy_if_fn replace_copy_if{};

    struct remove_fn
    {
      template <class I, class S, class T, class Proj = etl::identity, typename = etl::enable_if_t<!etl::is_range_v<I>>>
      constexpr ranges::subrange<I> operator()(I first, S last, const T& value, Proj proj = {}) const
      {
        first = ranges::find(first, last, value, etl::ref(proj));

        if (first != last)
        {
          I result = first;

          for (I it = result; ++it != last;)
          {
            if (!(etl::invoke(proj, *it) == value))
            {
              *result = etl::move(*it);
              ++result;
            }
          }

          return {result, last};
        }

        return {first, last};
      }

      template <class R, class T, class Proj = etl::identity, typename = etl::enable_if_t<etl::is_range_v<R>>>
      constexpr ranges::borrowed_subrange_t<R> operator()(R&& r, const T& value, Proj proj = {}) const
      {
        return (*this)(ranges::begin(r), ranges::end(r), value, etl::ref(proj));
      }
    };

    inline constexpr remove_fn remove{};

    struct remove_if_fn
    {
      template <class I, class S, class Pred, class Proj = etl::identity, typename = etl::enable_if_t<!etl::is_range_v<I>>>
      constexpr ranges::subrange<I> operator()(I first, S last, Pred pred, Proj proj = {}) const
      {
        first = ranges::find_if(first, last, etl::ref(pred), etl::ref(proj));

        if (first != last)
        {
          I result = first;

          for (I it = result; ++it != last;)
          {
            if (!etl::invoke(pred, etl::invoke(proj, *it)))
            {
              *result = etl::move(*it);
              ++result;
            }
          }

          return {result, last};
        }

        return {first, last};
      }

      template <class R, class Pred, class Proj = etl::identity, typename = etl::enable_if_t<etl::is_range_v<R>>>
      constexpr ranges::borrowed_subrange_t<R> operator()(R&& r, Pred pred, Proj proj = {}) const
      {
        return (*this)(ranges::begin(r), ranges::end(r), etl::ref(pred), etl::ref(proj));
      }
    };

    inline constexpr remove_if_fn remove_if{};

    struct remove_copy_fn
    {
      template <class I, class S, class O, class T, class Proj = etl::identity, typename = etl::enable_if_t<!etl::is_range_v<I>>>
      constexpr ranges::remove_copy_result<I, O> operator()(I first, S last, O result, const T& value, Proj proj = {}) const
      {
        for (; first != last; ++first)
        {
          if (!(etl::invoke(proj, *first) == value))
          {
            *result = *first;
            ++result;
          }
        }
        return {etl::move(first), etl::move(result)};
      }

      template <class R, class O, class T, class Proj = etl::identity, typename = etl::enable_if_t<etl::is_range_v<R>>>
      constexpr ranges::remove_copy_result<ranges::borrowed_iterator_t<R>, O> operator()(R&& r, O result, const T& value, Proj proj = {}) const
      {
        return (*this)(ranges::begin(r), ranges::end(r), etl::move(result), value, etl::ref(proj));
      }
    };

    inline constexpr remove_copy_fn remove_copy{};

    struct fill_fn
    {
      template <class I, class S, class T, typename = etl::enable_if_t<!etl::is_range_v<I>>>
      constexpr I operator()(I first, S last, const T& value) const
      {
        for (; first != last; ++first)
        {
          *first = value;
        }
        return first;
      }

      template <class R, class T, typename = etl::enable_if_t<etl::is_range_v<R>>>
      constexpr ranges::borrowed_iterator_t<R> operator()(R&& r, const T& value) const
      {
        return (*this)(ranges::begin(r), ranges::end(r), value);
      }
    };

    inline constexpr fill_fn fill{};

    struct fill_n_fn
    {
      template <class I, class T>
      constexpr I operator()(I first, etl::iter_difference_t<I> n, const T& value) const
      {
        for (; n-- > 0; ++first)
        {
          *first = value;
        }
        return first;
      }
    };

    inline constexpr fill_n_fn fill_n{};

    struct generate_fn
    {
      template <class I, class S, class F, typename = etl::enable_if_t<!etl::is_range_v<I>>>
      constexpr I operator()(I first, S last, F gen) const
      {
        for (; first != last; ++first)
        {
          *first = gen();
        }
        return first;
      }

      template <class R, class F, typename = etl::enable_if_t<etl::is_range_v<R>>>
      constexpr ranges::borrowed_iterator_t<R> operator()(R&& r, F gen) const
      {
        return (*this)(ranges::begin(r), ranges::end(r), etl::ref(gen));
      }
    };

    inline constexpr generate_fn generate{};

    struct generate_n_fn
    {
      template <class I, class F>
      constexpr I operator()(I first, etl::iter_difference_t<I> n, F gen) const
      {
        for (; n-- > 0; ++first)
        {
          *first = gen();
        }
        return first;
      }
    };

    inline constexpr generate_n_fn generate_n{};

    struct iota_fn
    {
      template <class O, class S, class T, typename = etl::enable_if_t<!etl::is_range_v<O>>>
      constexpr ranges::iota_result<O, T> operator()(O first, S last, T value) const
      {
        while (first != last)
        {
          *first = value;
          ++first;
          ++value;
        }
        return {etl::move(first), etl::move(value)};
      }

      template <class R, class T, typename = etl::enable_if_t<etl::is_range_v<R>>>
      constexpr ranges::iota_result<ranges::borrowed_iterator_t<R>, T> operator()(R&& r, T value) const
      {
        return (*this)(ranges::begin(r), ranges::end(r), etl::move(value));
      }
    };

    inline constexpr iota_fn iota{};

    struct unique_fn
    {
      template <class I, class S, class Pred = ranges::equal_to, class Proj = etl::identity, typename = etl::enable_if_t<!etl::is_range_v<I>>>
      constexpr ranges::subrange<I> operator()(I first, S last, Pred pred = {}, Proj proj = {}) const
      {
        first = ranges::adjacent_find(first, last, etl::ref(pred), etl::ref(proj));

        if (first != last)
        {
          I result = first;
          ++first;

          while (++first != last)
          {
            if (!etl::invoke(pred, etl::invoke(proj, *result), etl::invoke(proj, *first)))
            {
              *++result = etl::move(*first);
            }
          }

          return {++result, last};
        }

        return {first, last};
      }

      template <class R, class Pred = ranges::equal_to, class Proj = etl::identity, typename = etl::enable_if_t<etl::is_range_v<R>>>
      constexpr ranges::borrowed_subrange_t<R> operator()(R&& r, Pred pred = {}, Proj proj = {}) const
      {
        return (*this)(ranges::begin(r), ranges::end(r), etl::ref(pred), etl::ref(proj));
      }
    };

    inline constexpr unique_fn unique{};

    struct unique_copy_fn
    {
      template <class I, class S, class O, class Pred = ranges::equal_to, class Proj = etl::identity,
                typename = etl::enable_if_t<!etl::is_range_v<I>>>
      constexpr ranges::unique_copy_result<I, O> operator()(I first, S last, O result, Pred pred = {}, Proj proj = {}) const
      {
        if (first != last)
        {
          *result = *first;
          ++result;

          auto previous = first;
          ++first;

          for (; first != last; ++first)
          {
            if (!etl::invoke(pred, etl::invoke(proj, *previous), etl::invoke(proj, *first)))
            {
              *result = *first;
              ++result;
            }
            previous = first;
          }
        }

        return {etl::move(first), etl::move(result)};
      }

      template <class R, class O, class Pred = ranges::equal_to, class Proj = etl::identity, typename = etl::enable_if_t<etl::is_range_v<R>>>
      constexpr ranges::unique_copy_result<ranges::borrowed_iterator_t<R>, O> operator()(R&& r, O result, Pred pred = {}, Proj proj = {}) const
      {
        return (*this)(ranges::begin(r), ranges::end(r), etl::move(result), etl::ref(pred), etl::ref(proj));
      }
    };

    inline constexpr unique_copy_fn unique_copy{};

    struct transform_fn
    {
      // Unary: iterator + sentinel
      template <class I, class S, class O, class F, class Proj = etl::identity, typename = etl::enable_if_t<!etl::is_range_v<I>>>
      constexpr ranges::unary_transform_result<I, O> operator()(I first, S last, O result, F op, Proj proj = {}) const
      {
        for (; first != last; ++first, ++result)
        {
          *result = etl::invoke(op, etl::invoke(proj, *first));
        }
        return {etl::move(first), etl::move(result)};
      }

      // Unary: range
      template < class R, class O, class F, class Proj = etl::identity, typename = etl::enable_if_t<etl::is_range_v<R> && !etl::is_range_v<O>>>
      constexpr ranges::unary_transform_result<ranges::borrowed_iterator_t<R>, O> operator()(R&& r, O result, F op, Proj proj = {}) const
      {
        return (*this)(ranges::begin(r), ranges::end(r), etl::move(result), etl::ref(op), etl::ref(proj));
      }

      // Binary: iterator + sentinel
      template <class I1, class S1, class I2, class S2, class O, class F, class Proj1 = etl::identity, class Proj2 = etl::identity,
                typename = etl::enable_if_t<!etl::is_range_v<I1>>>
      constexpr ranges::binary_transform_result<I1, I2, O> operator()(I1 first1, S1 last1, I2 first2, S2 last2, O result, F op, Proj1 proj1 = {},
                                                                      Proj2 proj2 = {}) const
      {
        for (; first1 != last1 && first2 != last2; ++first1, ++first2, ++result)
        {
          *result = etl::invoke(op, etl::invoke(proj1, *first1), etl::invoke(proj2, *first2));
        }
        return {etl::move(first1), etl::move(first2), etl::move(result)};
      }

      // Binary: range
      template < class R1, class R2, class O, class F, class Proj1 = etl::identity, class Proj2 = etl::identity,
                 typename = etl::enable_if_t<etl::is_range_v<R1> && etl::is_range_v<R2>>>
      constexpr ranges::binary_transform_result< ranges::borrowed_iterator_t<R1>, ranges::borrowed_iterator_t<R2>, O>
        operator()(R1&& r1, R2&& r2, O result, F op, Proj1 proj1 = {}, Proj2 proj2 = {}) const
      {
        return (*this)(ranges::begin(r1), ranges::end(r1), ranges::begin(r2), ranges::end(r2), etl::move(result), etl::ref(op), etl::ref(proj1),
                       etl::ref(proj2));
      }
    };

    inline constexpr transform_fn transform{};

    struct reverse_fn
    {
      template <class I, class S, typename = etl::enable_if_t<!etl::is_range_v<I>>>
      constexpr I operator()(I first, S last) const
      {
        I tail   = ranges::next(first, last);
        I result = tail;

        for (; first != tail && first != --tail; ++first)
        {
          etl::iter_swap(first, tail);
        }

        return result;
      }

      template <class R, typename = etl::enable_if_t<etl::is_range_v<R>>>
      constexpr ranges::borrowed_iterator_t<R> operator()(R&& r) const
      {
        return (*this)(ranges::begin(r), ranges::end(r));
      }
    };

    inline constexpr reverse_fn reverse{};

    template <class I, class O>
    using reverse_copy_result = in_out_result<I, O>;

    struct reverse_copy_fn
    {
      template <class I, class S, class O, typename = etl::enable_if_t<!etl::is_range_v<I>>>
      constexpr ranges::reverse_copy_result<I, O> operator()(I first, S last, O result) const
      {
        I tail   = ranges::next(first, last);
        I end_it = tail;

        while (tail != first)
        {
          *result = *--tail;
          ++result;
        }

        return {etl::move(end_it), etl::move(result)};
      }

      template <class R, class O, typename = etl::enable_if_t<etl::is_range_v<R>>>
      constexpr ranges::reverse_copy_result<ranges::borrowed_iterator_t<R>, O> operator()(R&& r, O result) const
      {
        return (*this)(ranges::begin(r), ranges::end(r), etl::move(result));
      }
    };

    inline constexpr reverse_copy_fn reverse_copy{};

    template <class I>
    using rotate_result = ranges::subrange<I>;

    struct rotate_fn
    {
      template <class I, class S, typename = etl::enable_if_t<!etl::is_range_v<I>>>
      constexpr ranges::rotate_result<I> operator()(I first, I middle, S last) const
      {
        if (first == middle)
        {
          I last_it = ranges::next(first, last);
          return {last_it, last_it};
        }

        I last_it = ranges::next(first, last);

        if (middle == last_it)
        {
          return {first, last_it};
        }

        I new_first = etl::rotate(first, middle, last_it);
        return {etl::move(new_first), etl::move(last_it)};
      }

      template <class R, typename = etl::enable_if_t<etl::is_range_v<R>>>
      constexpr ranges::rotate_result<ranges::borrowed_iterator_t<R>> operator()(R&& r, ranges::iterator_t<R> middle) const
      {
        return (*this)(ranges::begin(r), etl::move(middle), ranges::end(r));
      }
    };

    inline constexpr rotate_fn rotate{};

    struct rotate_copy_fn
    {
      template <class I, class S, class O, typename = etl::enable_if_t<!etl::is_range_v<I>>>
      constexpr ranges::rotate_copy_result<I, O> operator()(I first, I middle, S last, O result) const
      {
        I last_it = ranges::next(first, last);
        O end_out = etl::copy(middle, last_it, result);
        end_out   = etl::copy(first, middle, end_out);
        return {etl::move(last_it), etl::move(end_out)};
      }

      template <class R, class O, typename = etl::enable_if_t<etl::is_range_v<R>>>
      constexpr ranges::rotate_copy_result<ranges::borrowed_iterator_t<R>, O> operator()(R&& r, ranges::iterator_t<R> middle, O result) const
      {
        return (*this)(ranges::begin(r), etl::move(middle), ranges::end(r), etl::move(result));
      }
    };

    inline constexpr rotate_copy_fn rotate_copy{};

    struct shift_left_fn
    {
      template <class I, class S, typename = etl::enable_if_t<!etl::is_range_v<I>>>
      constexpr ranges::subrange<I> operator()(I first, S last, etl::iter_difference_t<I> n) const
      {
        I last_it = ranges::next(first, last);

        if (n <= 0)
        {
          return {first, last_it};
        }

        I mid = first;
        if (ranges::advance(mid, n, last_it) != 0)
        {
          return {first, first};
        }

        I result = ranges::move(mid, last_it, first).out;
        return {first, etl::move(result)};
      }

      template <class R, typename = etl::enable_if_t<etl::is_range_v<R>>>
      constexpr ranges::borrowed_subrange_t<R> operator()(R&& r, etl::ranges::range_difference_t<R> n) const
      {
        return (*this)(ranges::begin(r), ranges::end(r), n);
      }
    };

    inline constexpr shift_left_fn shift_left{};

    struct shift_right_fn
    {
      template <class I, class S, typename = etl::enable_if_t<!etl::is_range_v<I>>>
      constexpr ranges::subrange<I> operator()(I first, S last, etl::iter_difference_t<I> n) const
      {
        I last_it = ranges::next(first, last);

        if (n <= 0)
        {
          return {first, last_it};
        }

        I trail = last_it;
        if (ranges::advance(trail, -n, first) != 0)
        {
          return {last_it, last_it};
        }

        I new_first = ranges::move_backward(first, trail, last_it).out;
        return {etl::move(new_first), etl::move(last_it)};
      }

      template <class R, typename = etl::enable_if_t<etl::is_range_v<R>>>
      constexpr ranges::borrowed_subrange_t<R> operator()(R&& r, etl::ranges::range_difference_t<R> n) const
      {
        return (*this)(ranges::begin(r), ranges::end(r), n);
      }
    };

    inline constexpr shift_right_fn shift_right{};

    struct shuffle_fn
    {
      template <class I, class S, class Gen, typename = etl::enable_if_t<!etl::is_range_v<I>>>
      I operator()(I first, S last, Gen&& gen) const
      {
        ETL_STATIC_ASSERT(etl::is_random_access_iterator<I>::value, "shuffle requires random access iterators");

        using diff_t    = etl::iter_difference_t<I>;
        using udiff_t   = etl::make_unsigned_t<diff_t>;
        using gen_t     = etl::remove_reference_t<Gen>;
        using uresult_t = decltype(gen());

        I      last_it = ranges::next(first, last);
        diff_t n       = last_it - first;

        if (n <= 1)
        {
          return last_it;
        }

        for (diff_t i = n - 1; i > 0; --i)
        {
          // Generate a uniformly distributed random index in [0, i]
          // using rejection sampling to avoid modulo bias.
          udiff_t             range  = static_cast<udiff_t>(i);
          constexpr uresult_t gmin   = gen_t::min();
          constexpr uresult_t gmax   = gen_t::max();
          constexpr uresult_t grange = gmax - gmin;

          uresult_t j;

          if ETL_IF_CONSTEXPR (grange == static_cast<uresult_t>(-1))
          {
            // Generator covers full range of uresult_t, just use modulo with
            // rejection
            uresult_t limit = (static_cast<uresult_t>(-1) / (static_cast<uresult_t>(range) + 1)) * (static_cast<uresult_t>(range) + 1);
            do {
              j = static_cast<uresult_t>(gen() - gmin);
            } while (j >= limit);
            j %= (static_cast<uresult_t>(range) + 1);
          }
          else
          {
            uresult_t limit = (grange / (static_cast<uresult_t>(range) + 1)) * (static_cast<uresult_t>(range) + 1);
            do {
              j = static_cast<uresult_t>(gen() - gmin);
            } while (j >= limit);
            j %= (static_cast<uresult_t>(range) + 1);
          }

          etl::iter_swap(first + i, first + static_cast<diff_t>(j));
        }

        return last_it;
      }

      template <class R, class Gen, typename = etl::enable_if_t<etl::is_range_v<R>>>
      ranges::borrowed_iterator_t<R> operator()(R&& r, Gen&& gen) const
      {
        ETL_STATIC_ASSERT(etl::is_random_access_iterator<ranges::iterator_t<R>>::value, "shuffle requires a range with random access iterators");

        return (*this)(ranges::begin(r), ranges::end(r), static_cast<Gen&&>(gen));
      }
    };

    inline constexpr shuffle_fn shuffle{};

    struct sample_fn
    {
      template <class I, class S, class O, class Gen, typename = etl::enable_if_t<!etl::is_range_v<I>>>
      O operator()(I first, S last, O out, etl::iter_difference_t<I> n, Gen&& gen) const
      {
        using diff_t    = etl::iter_difference_t<I>;
        using udiff_t   = etl::make_unsigned_t<diff_t>;
        using gen_t     = etl::remove_reference_t<Gen>;
        using uresult_t = decltype(gen());

        if (n <= 0)
        {
          return out;
        }

        // Compute the size of [first, last).
        I      first_copy = first;
        diff_t pop_size   = 0;
        for (I it = first_copy; it != last; ++it)
        {
          ++pop_size;
        }

        if (pop_size <= n)
        {
          // Copy all elements.
          for (; first != last; ++first, ++out)
          {
            *out = *first;
          }
          return out;
        }

        // Selection sampling (Algorithm S / Vitter).
        // For each element, decide whether to include it.
        diff_t remaining = pop_size;
        diff_t needed    = n;

        for (; first != last && needed > 0; ++first, --remaining)
        {
          // Generate a uniformly distributed random number in [0, remaining).
          udiff_t             range  = static_cast<udiff_t>(remaining - 1);
          constexpr uresult_t gmin   = gen_t::min();
          constexpr uresult_t gmax   = gen_t::max();
          constexpr uresult_t grange = gmax - gmin;

          uresult_t j;

          if ETL_IF_CONSTEXPR (grange == static_cast<uresult_t>(-1))
          {
            if (range == 0)
            {
              j = 0;
            }
            else
            {
              uresult_t limit = (static_cast<uresult_t>(-1) / (static_cast<uresult_t>(range) + 1)) * (static_cast<uresult_t>(range) + 1);
              do {
                j = static_cast<uresult_t>(gen() - gmin);
              } while (j >= limit);
              j %= (static_cast<uresult_t>(range) + 1);
            }
          }
          else
          {
            if (range == 0)
            {
              j = 0;
            }
            else
            {
              uresult_t limit = (grange / (static_cast<uresult_t>(range) + 1)) * (static_cast<uresult_t>(range) + 1);
              do {
                j = static_cast<uresult_t>(gen() - gmin);
              } while (j >= limit);
              j %= (static_cast<uresult_t>(range) + 1);
            }
          }

          if (static_cast<diff_t>(j) < needed)
          {
            *out = *first;
            ++out;
            --needed;
          }
        }

        return out;
      }

      template <class R, class O, class Gen, typename = etl::enable_if_t<etl::is_range_v<R>>>
      O operator()(R&& r, O out, etl::ranges::range_difference_t<R> n, Gen&& gen) const
      {
        return (*this)(ranges::begin(r), ranges::end(r), etl::move(out), n, static_cast<Gen&&>(gen));
      }
    };

    inline constexpr sample_fn sample{};

    struct sort_fn
    {
      template <class I, class S, class Comp = ranges::less, class Proj = etl::identity, typename = etl::enable_if_t<!etl::is_range_v<I>>>
      constexpr I operator()(I first, S last, Comp comp = {}, Proj proj = {}) const
      {
        ETL_STATIC_ASSERT(etl::is_random_access_iterator<I>::value, "sort requires random access iterators");

        I last_it = ranges::next(first, last);

        if (first == last_it)
        {
          return last_it;
        }

        // Shell sort with projection support
        auto n = etl::distance(first, last_it);

        for (auto gap = n / 2; gap > 0; gap /= 2)
        {
          for (auto i = gap; i < n; ++i)
          {
            auto temp = etl::move(*(first + i));
            auto j    = i;

            while (j >= gap && etl::invoke(comp, etl::invoke(proj, temp), etl::invoke(proj, *(first + (j - gap)))))
            {
              *(first + j) = etl::move(*(first + (j - gap)));
              j -= gap;
            }

            *(first + j) = etl::move(temp);
          }
        }

        return last_it;
      }

      template <class R, class Comp = ranges::less, class Proj = etl::identity, typename = etl::enable_if_t<etl::is_range_v<R>>>
      constexpr ranges::borrowed_iterator_t<R> operator()(R&& r, Comp comp = {}, Proj proj = {}) const
      {
        ETL_STATIC_ASSERT(etl::is_random_access_iterator<ranges::iterator_t<R>>::value, "sort requires a range with random access iterators");

        return (*this)(ranges::begin(r), ranges::end(r), etl::move(comp), etl::move(proj));
      }
    };

    inline constexpr sort_fn sort{};

    struct stable_sort_fn
    {
      template <class I, class S, class Comp = ranges::less, class Proj = etl::identity, typename = etl::enable_if_t<!etl::is_range_v<I>>>
      constexpr I operator()(I first, S last, Comp comp = {}, Proj proj = {}) const
      {
        I last_it = ranges::next(first, last);

        if (first == last_it)
        {
          return last_it;
        }

        // Insertion sort with projection support (stable)
        for (I i = ranges::next(first); i != last_it; ++i)
        {
          auto temp = etl::move(*i);
          I    j    = i;

          while (j != first)
          {
            I prev_j = ranges::prev(j);
            if (etl::invoke(comp, etl::invoke(proj, temp), etl::invoke(proj, *prev_j)))
            {
              *j = etl::move(*prev_j);
              j  = prev_j;
            }
            else
            {
              break;
            }
          }

          *j = etl::move(temp);
        }

        return last_it;
      }

      template <class R, class Comp = ranges::less, class Proj = etl::identity, typename = etl::enable_if_t<etl::is_range_v<R>>>
      constexpr ranges::borrowed_iterator_t<R> operator()(R&& r, Comp comp = {}, Proj proj = {}) const
      {
        return (*this)(ranges::begin(r), ranges::end(r), etl::move(comp), etl::move(proj));
      }
    };

    inline constexpr stable_sort_fn stable_sort{};

    struct partial_sort_fn
    {
      template <class I, class S, class Comp = ranges::less, class Proj = etl::identity, typename = etl::enable_if_t<!etl::is_range_v<I>>>
      constexpr I operator()(I first, I middle, S last, Comp comp = {}, Proj proj = {}) const
      {
        ETL_STATIC_ASSERT(etl::is_random_access_iterator<I>::value, "partial_sort requires random access iterators");

        I last_it = ranges::next(first, last);

        if (first == middle || first == last_it)
        {
          return last_it;
        }

        // Build a max-heap on [first, middle)
        auto heap_size = etl::distance(first, middle);

        // Heapify: process from the last parent down to 0
        for (auto start = (heap_size - 1) / 2; start >= 0; --start)
        {
          sift_down(first, start, heap_size, comp, proj);
        }

        // For each element in [middle, last_it), if it is smaller than the heap
        // root, swap it in and re-heapify
        for (I it = middle; it != last_it; ++it)
        {
          if (etl::invoke(comp, etl::invoke(proj, *it), etl::invoke(proj, *first)))
          {
            etl::iter_swap(it, first);
            sift_down(first, decltype(heap_size){0}, heap_size, comp, proj);
          }
        }

        // Sort the heap to produce a sorted [first, middle)
        // Repeatedly extract the max from the heap
        for (auto heap_end = heap_size - 1; heap_end > 0; --heap_end)
        {
          etl::iter_swap(first, first + heap_end);
          sift_down(first, decltype(heap_size){0}, heap_end, comp, proj);
        }

        return last_it;
      }

      template <class R, class Comp = ranges::less, class Proj = etl::identity, typename = etl::enable_if_t<etl::is_range_v<R>>>
      constexpr ranges::borrowed_iterator_t<R> operator()(R&& r, ranges::iterator_t<R> middle, Comp comp = {}, Proj proj = {}) const
      {
        ETL_STATIC_ASSERT(etl::is_random_access_iterator<ranges::iterator_t<R>>::value, "partial_sort requires a range with random access iterators");

        return (*this)(ranges::begin(r), etl::move(middle), ranges::end(r), etl::move(comp), etl::move(proj));
      }

    private:

      template <class I, class DiffType, class Comp, class Proj>
      static constexpr void sift_down(I first, DiffType index, DiffType heap_size, Comp& comp, Proj& proj)
      {
        while (true)
        {
          auto largest = index;
          auto left    = 2 * index + 1;
          auto right   = 2 * index + 2;

          if (left < heap_size && etl::invoke(comp, etl::invoke(proj, *(first + largest)), etl::invoke(proj, *(first + left))))
          {
            largest = left;
          }

          if (right < heap_size && etl::invoke(comp, etl::invoke(proj, *(first + largest)), etl::invoke(proj, *(first + right))))
          {
            largest = right;
          }

          if (largest == index)
          {
            break;
          }

          etl::iter_swap(first + index, first + largest);
          index = largest;
        }
      }
    };

    inline constexpr partial_sort_fn partial_sort{};

    struct partial_sort_copy_fn
    {
      template <class I1, class S1, class I2, class S2, class Comp = ranges::less, class Proj1 = etl::identity, class Proj2 = etl::identity,
                typename = etl::enable_if_t<!etl::is_range_v<I1>>>
      constexpr ranges::partial_sort_copy_result<I1, I2> operator()(I1 first, S1 last, I2 result_first, S2 result_last, Comp comp = {},
                                                                    Proj1 proj1 = {}, Proj2 proj2 = {}) const
      {
        ETL_STATIC_ASSERT(etl::is_random_access_iterator<I2>::value, "partial_sort_copy requires the output to be random access iterators");

        I1 in_last  = ranges::next(first, last);
        I2 out_last = ranges::next(result_first, result_last);

        I2 r = result_first;

        // Copy elements from the input range into the output range
        for (I1 it = first; it != in_last && r != out_last; ++it, ++r)
        {
          *r = *it;
        }

        auto heap_size = etl::distance(result_first, r);

        if (heap_size == 0)
        {
          return {etl::move(in_last), etl::move(r)};
        }

        // Build a max-heap on [result_first, r)
        for (auto start = (heap_size - 1) / 2; start >= 0; --start)
        {
          sift_down(result_first, start, heap_size, comp, proj2);
        }

        // For remaining elements in [first + heap_size, in_last), if smaller
        // than the heap root, swap it in and re-heapify
        I1 it = first;
        etl::advance(it, heap_size);
        for (; it != in_last; ++it)
        {
          if (etl::invoke(comp, etl::invoke(proj1, *it), etl::invoke(proj2, *result_first)))
          {
            *result_first = *it;
            sift_down(result_first, decltype(heap_size){0}, heap_size, comp, proj2);
          }
        }

  #include "etl/private/diagnostic_array_bounds_push.h"
        // Sort the heap to produce a sorted output range
        for (auto heap_end = heap_size - 1; heap_end > 0; --heap_end)
        {
          etl::iter_swap(result_first, result_first + heap_end);
          sift_down(result_first, decltype(heap_size){0}, heap_end, comp, proj2);
        }
  #include "etl/private/diagnostic_pop.h"

        return {etl::move(in_last), etl::move(r)};
      }

      template <class R1, class R2, class Comp = ranges::less, class Proj1 = etl::identity, class Proj2 = etl::identity,
                typename = etl::enable_if_t<etl::is_range_v<R1>>>
      constexpr ranges::partial_sort_copy_result< ranges::borrowed_iterator_t<R1>, ranges::borrowed_iterator_t<R2>>
        operator()(R1&& r, R2&& result_r, Comp comp = {}, Proj1 proj1 = {}, Proj2 proj2 = {}) const
      {
        ETL_STATIC_ASSERT(etl::is_random_access_iterator<ranges::iterator_t<R2>>::value,
                          "partial_sort_copy requires the output range to have random access iterators");

        return (*this)(ranges::begin(r), ranges::end(r), ranges::begin(result_r), ranges::end(result_r), etl::move(comp), etl::move(proj1),
                       etl::move(proj2));
      }

    private:

      template <class I, class DiffType, class Comp, class Proj>
      static constexpr void sift_down(I first, DiffType index, DiffType heap_size, Comp& comp, Proj& proj)
      {
        while (true)
        {
          auto largest = index;
          auto left    = 2 * index + 1;
          auto right   = 2 * index + 2;

          if (left < heap_size && etl::invoke(comp, etl::invoke(proj, *(first + largest)), etl::invoke(proj, *(first + left))))
          {
            largest = left;
          }

          if (right < heap_size && etl::invoke(comp, etl::invoke(proj, *(first + largest)), etl::invoke(proj, *(first + right))))
          {
            largest = right;
          }

          if (largest == index)
          {
            break;
          }

          etl::iter_swap(first + index, first + largest);
          index = largest;
        }
      }
    };

    inline constexpr partial_sort_copy_fn partial_sort_copy{};

    struct nth_element_fn
    {
      template <class I, class S, class Comp = ranges::less, class Proj = etl::identity, typename = etl::enable_if_t<!etl::is_range_v<I>>>
      constexpr I operator()(I first, I nth, S last, Comp comp = {}, Proj proj = {}) const
      {
        ETL_STATIC_ASSERT(etl::is_random_access_iterator<I>::value, "nth_element requires random access iterators");

        I last_it = ranges::next(first, last);

        if (first == last_it || ranges::next(first) == last_it)
        {
          return last_it;
        }

        I lo = first;
        I hi = ranges::prev(last_it);

        while (lo <= hi)
        {
          I p = nth_partition(lo, hi, comp, proj);

          if (p == nth)
          {
            return last_it;
          }
          else if (p > nth)
          {
            hi = ranges::prev(p);
          }
          else
          {
            lo = ranges::next(p);
          }
        }

        return last_it;
      }

      template <class R, class Comp = ranges::less, class Proj = etl::identity, typename = etl::enable_if_t<etl::is_range_v<R>>>
      constexpr ranges::borrowed_iterator_t<R> operator()(R&& r, ranges::iterator_t<R> nth, Comp comp = {}, Proj proj = {}) const
      {
        ETL_STATIC_ASSERT(etl::is_random_access_iterator<ranges::iterator_t<R>>::value, "nth_element requires a range with random access iterators");

        return (*this)(ranges::begin(r), etl::move(nth), ranges::end(r), etl::move(comp), etl::move(proj));
      }

    private:

      template <class I, class Comp, class Proj>
      static constexpr I nth_partition(I first, I last, Comp& comp, Proj& proj)
      {
        if (first == last)
        {
          return first;
        }

        if (last - first == 1)
        {
          if (etl::invoke(comp, etl::invoke(proj, *last), etl::invoke(proj, *first)))
          {
            etl::iter_swap(first, last);
          }
          return first;
        }

        // Median-of-three pivot selection
        I mid = first + (last - first) / 2;

        if (etl::invoke(comp, etl::invoke(proj, *mid), etl::invoke(proj, *first)))
        {
          etl::iter_swap(first, mid);
        }

        if (etl::invoke(comp, etl::invoke(proj, *last), etl::invoke(proj, *first)))
        {
          etl::iter_swap(first, last);
        }

        if (etl::invoke(comp, etl::invoke(proj, *mid), etl::invoke(proj, *last)))
        {
          etl::iter_swap(mid, last);
        }

        // Pivot is now at *last
        I i = first;
        I j = last;

        while (true)
        {
          while (etl::invoke(comp, etl::invoke(proj, *i), etl::invoke(proj, *last)))
          {
            ++i;
          }

          --j;

          while (i < j && etl::invoke(comp, etl::invoke(proj, *last), etl::invoke(proj, *j)))
          {
            --j;
          }

          if (i >= j)
          {
            break;
          }

          etl::iter_swap(i, j);
          ++i;
        }

        etl::iter_swap(i, last);
        return i;
      }
    };

    inline constexpr nth_element_fn nth_element{};

    struct partition_fn
    {
      template <class I, class S, class Pred, class Proj = etl::identity, typename = etl::enable_if_t<!etl::is_range_v<I>>>
      constexpr ranges::subrange<I> operator()(I first, S last, Pred pred, Proj proj = {}) const
      {
        first = ranges::find_if_not(first, last, etl::ref(pred), etl::ref(proj));

        if (first == last)
        {
          return {first, first};
        }

        for (I i = ranges::next(first); i != last; ++i)
        {
          if (etl::invoke(pred, etl::invoke(proj, *i)))
          {
            etl::iter_swap(i, first);
            ++first;
          }
        }

        return {first, ranges::next(first, last)};
      }

      template <class R, class Pred, class Proj = etl::identity, typename = etl::enable_if_t<etl::is_range_v<R>>>
      constexpr ranges::borrowed_subrange_t<R> operator()(R&& r, Pred pred, Proj proj = {}) const
      {
        return (*this)(ranges::begin(r), ranges::end(r), etl::ref(pred), etl::ref(proj));
      }
    };

    inline constexpr partition_fn partition{};

    struct is_partitioned_fn
    {
      template <class I, class S, class Pred, class Proj = etl::identity, typename = etl::enable_if_t<!etl::is_range_v<I>>>
      constexpr bool operator()(I first, S last, Pred pred, Proj proj = {}) const
      {
        for (; first != last; ++first)
        {
          if (!etl::invoke(pred, etl::invoke(proj, *first)))
          {
            break;
          }
        }

        for (; first != last; ++first)
        {
          if (etl::invoke(pred, etl::invoke(proj, *first)))
          {
            return false;
          }
        }

        return true;
      }

      template <class R, class Pred, class Proj = etl::identity, typename = etl::enable_if_t<etl::is_range_v<R>>>
      constexpr bool operator()(R&& r, Pred pred, Proj proj = {}) const
      {
        return (*this)(ranges::begin(r), ranges::end(r), etl::ref(pred), etl::ref(proj));
      }
    };

    inline constexpr is_partitioned_fn is_partitioned{};

    struct partition_copy_fn
    {
      template <class I, class S, class O1, class O2, class Pred, class Proj = etl::identity, typename = etl::enable_if_t<!etl::is_range_v<I>>>
      constexpr ranges::partition_copy_result<I, O1, O2> operator()(I first, S last, O1 out_true, O2 out_false, Pred pred, Proj proj = {}) const
      {
        for (; first != last; ++first)
        {
          if (etl::invoke(pred, etl::invoke(proj, *first)))
          {
            *out_true = *first;
            ++out_true;
          }
          else
          {
            *out_false = *first;
            ++out_false;
          }
        }

        return {etl::move(first), etl::move(out_true), etl::move(out_false)};
      }

      template <class R, class O1, class O2, class Pred, class Proj = etl::identity, typename = etl::enable_if_t<etl::is_range_v<R>>>
      constexpr ranges::partition_copy_result<ranges::borrowed_iterator_t<R>, O1, O2> operator()(R&& r, O1 out_true, O2 out_false, Pred pred,
                                                                                                 Proj proj = {}) const
      {
        return (*this)(ranges::begin(r), ranges::end(r), etl::move(out_true), etl::move(out_false), etl::ref(pred), etl::ref(proj));
      }
    };

    inline constexpr partition_copy_fn partition_copy{};

    struct partition_point_fn
    {
      template <class I, class S, class Pred, class Proj = etl::identity, typename = etl::enable_if_t<!etl::is_range_v<I>>>
      constexpr I operator()(I first, S last, Pred pred, Proj proj = {}) const
      {
        for (; first != last; ++first)
        {
          if (!etl::invoke(pred, etl::invoke(proj, *first)))
          {
            return first;
          }
        }

        return first;
      }

      template <class R, class Pred, class Proj = etl::identity, typename = etl::enable_if_t<etl::is_range_v<R>>>
      constexpr ranges::borrowed_iterator_t<R> operator()(R&& r, Pred pred, Proj proj = {}) const
      {
        return (*this)(ranges::begin(r), ranges::end(r), etl::ref(pred), etl::ref(proj));
      }
    };

    inline constexpr partition_point_fn partition_point{};

    struct stable_partition_fn
    {
      template <class I, class S, class Pred, class Proj = etl::identity, typename = etl::enable_if_t<!etl::is_range_v<I>>>
      constexpr ranges::subrange<I> operator()(I first, S last, Pred pred, Proj proj = {}) const
      {
        // Find the first element that does not satisfy the predicate
        first = ranges::find_if_not(first, last, etl::ref(pred), etl::ref(proj));

        if (first == last)
        {
          return {first, first};
        }

        I last_it = ranges::next(first, last);

        I pp = stable_partition_impl(first, last_it, etl::ref(pred), etl::ref(proj), etl::distance(first, last_it));

        return {pp, last_it};
      }

      template <class R, class Pred, class Proj = etl::identity, typename = etl::enable_if_t<etl::is_range_v<R>>>
      constexpr ranges::borrowed_subrange_t<R> operator()(R&& r, Pred pred, Proj proj = {}) const
      {
        return (*this)(ranges::begin(r), ranges::end(r), etl::ref(pred), etl::ref(proj));
      }

    private:

      template <class I, class Pred, class Proj>
      static constexpr I stable_partition_impl(I first, I last, Pred pred, Proj proj, typename etl::iterator_traits<I>::difference_type len)
      {
        if (len == 0)
        {
          return first;
        }

        if (len == 1)
        {
          return etl::invoke(pred, etl::invoke(proj, *first)) ? ranges::next(first) : first;
        }

        I middle = ranges::next(first, len / 2);

        I left_partition  = stable_partition_impl(first, middle, etl::ref(pred), etl::ref(proj), len / 2);
        I right_partition = stable_partition_impl(middle, last, etl::ref(pred), etl::ref(proj), len - len / 2);

        if (left_partition == middle)
        {
          return right_partition;
        }

        if (middle == right_partition)
        {
          return left_partition;
        }

        return etl::rotate(left_partition, middle, right_partition);
      }
    };

    inline constexpr stable_partition_fn stable_partition{};

    struct is_sorted_until_fn
    {
      template <class I, class S, class Comp = ranges::less, class Proj = etl::identity, typename = etl::enable_if_t<!etl::is_range_v<I>>>
      constexpr I operator()(I first, S last, Comp comp = {}, Proj proj = {}) const
      {
        if (first != last)
        {
          I next_it = ranges::next(first);

          while (next_it != last)
          {
            if (etl::invoke(comp, etl::invoke(proj, *next_it), etl::invoke(proj, *first)))
            {
              return next_it;
            }

            first = next_it;
            ++next_it;
          }
        }

        return ranges::next(first, last);
      }

      template <class R, class Comp = ranges::less, class Proj = etl::identity, typename = etl::enable_if_t<etl::is_range_v<R>>>
      constexpr ranges::borrowed_iterator_t<R> operator()(R&& r, Comp comp = {}, Proj proj = {}) const
      {
        return (*this)(ranges::begin(r), ranges::end(r), etl::move(comp), etl::move(proj));
      }
    };

    inline constexpr is_sorted_until_fn is_sorted_until{};

    struct is_sorted_fn
    {
      template <class I, class S, class Comp = ranges::less, class Proj = etl::identity, typename = etl::enable_if_t<!etl::is_range_v<I>>>
      constexpr bool operator()(I first, S last, Comp comp = {}, Proj proj = {}) const
      {
        return ranges::is_sorted_until(first, last, etl::ref(comp), etl::ref(proj)) == last;
      }

      template <class R, class Comp = ranges::less, class Proj = etl::identity, typename = etl::enable_if_t<etl::is_range_v<R>>>
      constexpr bool operator()(R&& r, Comp comp = {}, Proj proj = {}) const
      {
        return (*this)(ranges::begin(r), ranges::end(r), etl::move(comp), etl::move(proj));
      }
    };

    inline constexpr is_sorted_fn is_sorted{};

    struct lower_bound_fn
    {
      template <class I, class S, class T, class Comp = ranges::less, class Proj = etl::identity, typename = etl::enable_if_t<!etl::is_range_v<I>>>
      constexpr I operator()(I first, S last, const T& value, Comp comp = {}, Proj proj = {}) const
      {
        auto len = etl::distance(first, last);

        while (len > 0)
        {
          auto half   = len / 2;
          I    middle = ranges::next(first, half);

          if (etl::invoke(comp, etl::invoke(proj, *middle), value))
          {
            first = ranges::next(middle);
            len -= half + 1;
          }
          else
          {
            len = half;
          }
        }

        return first;
      }

      template <class R, class T, class Comp = ranges::less, class Proj = etl::identity, typename = etl::enable_if_t<etl::is_range_v<R>>>
      constexpr ranges::borrowed_iterator_t<R> operator()(R&& r, const T& value, Comp comp = {}, Proj proj = {}) const
      {
        return (*this)(ranges::begin(r), ranges::end(r), value, etl::move(comp), etl::move(proj));
      }
    };

    inline constexpr lower_bound_fn lower_bound{};

    struct upper_bound_fn
    {
      template <class I, class S, class T, class Comp = ranges::less, class Proj = etl::identity, typename = etl::enable_if_t<!etl::is_range_v<I>>>
      constexpr I operator()(I first, S last, const T& value, Comp comp = {}, Proj proj = {}) const
      {
        auto len = etl::distance(first, last);

        while (len > 0)
        {
          auto half   = len / 2;
          I    middle = ranges::next(first, half);

          if (!etl::invoke(comp, value, etl::invoke(proj, *middle)))
          {
            first = ranges::next(middle);
            len -= half + 1;
          }
          else
          {
            len = half;
          }
        }

        return first;
      }

      template <class R, class T, class Comp = ranges::less, class Proj = etl::identity, typename = etl::enable_if_t<etl::is_range_v<R>>>
      constexpr ranges::borrowed_iterator_t<R> operator()(R&& r, const T& value, Comp comp = {}, Proj proj = {}) const
      {
        return (*this)(ranges::begin(r), ranges::end(r), value, etl::move(comp), etl::move(proj));
      }
    };

    inline constexpr upper_bound_fn upper_bound{};

    struct equal_range_fn
    {
      template <class I, class S, class T, class Comp = ranges::less, class Proj = etl::identity, typename = etl::enable_if_t<!etl::is_range_v<I>>>
      constexpr ranges::subrange<I> operator()(I first, S last, const T& value, Comp comp = {}, Proj proj = {}) const
      {
        return {ranges::lower_bound(first, last, value, etl::ref(comp), etl::ref(proj)),
                ranges::upper_bound(first, last, value, etl::ref(comp), etl::ref(proj))};
      }

      template <class R, class T, class Comp = ranges::less, class Proj = etl::identity, typename = etl::enable_if_t<etl::is_range_v<R>>>
      constexpr ranges::borrowed_subrange_t<R> operator()(R&& r, const T& value, Comp comp = {}, Proj proj = {}) const
      {
        return (*this)(ranges::begin(r), ranges::end(r), value, etl::move(comp), etl::move(proj));
      }
    };

    inline constexpr equal_range_fn equal_range{};

    struct binary_search_fn
    {
      template <class I, class S, class T, class Comp = ranges::less, class Proj = etl::identity, typename = etl::enable_if_t<!etl::is_range_v<I>>>
      ETL_NODISCARD
      constexpr bool operator()(I first, S last, const T& value, Comp comp = {}, Proj proj = {}) const
      {
        first = ranges::lower_bound(first, last, value, etl::ref(comp), etl::ref(proj));

        return (!(first == last) && !(etl::invoke(comp, value, etl::invoke(proj, *first))));
      }

      template <class R, class T, class Comp = ranges::less, class Proj = etl::identity, typename = etl::enable_if_t<etl::is_range_v<R>>>
      ETL_NODISCARD
      constexpr bool operator()(R&& r, const T& value, Comp comp = {}, Proj proj = {}) const
      {
        return (*this)(ranges::begin(r), ranges::end(r), value, etl::move(comp), etl::move(proj));
      }
    };

    inline constexpr binary_search_fn binary_search{};

    struct includes_fn
    {
      template <class I1, class S1, class I2, class S2, class Comp = ranges::less, class Proj1 = etl::identity, class Proj2 = etl::identity,
                typename = etl::enable_if_t<!etl::is_range_v<I1>>>
      ETL_NODISCARD
      constexpr bool operator()(I1 first1, S1 last1, I2 first2, S2 last2, Comp comp = {}, Proj1 proj1 = {}, Proj2 proj2 = {}) const
      {
        for (; first2 != last2; ++first1)
        {
          if (first1 == last1)
          {
            return false;
          }

          if (etl::invoke(comp, etl::invoke(proj2, *first2), etl::invoke(proj1, *first1)))
          {
            return false;
          }

          if (!etl::invoke(comp, etl::invoke(proj1, *first1), etl::invoke(proj2, *first2)))
          {
            ++first2;
          }
        }

        return true;
      }

      template <class R1, class R2, class Comp = ranges::less, class Proj1 = etl::identity, class Proj2 = etl::identity,
                typename = etl::enable_if_t<etl::is_range_v<R1>>>
      ETL_NODISCARD
      constexpr bool operator()(R1&& r1, R2&& r2, Comp comp = {}, Proj1 proj1 = {}, Proj2 proj2 = {}) const
      {
        return (*this)(ranges::begin(r1), ranges::end(r1), ranges::begin(r2), ranges::end(r2), etl::move(comp), etl::move(proj1), etl::move(proj2));
      }
    };

    inline constexpr includes_fn includes{};

    struct merge_fn
    {
      template <class I1, class S1, class I2, class S2, class O, class Comp = ranges::less, class Proj1 = etl::identity, class Proj2 = etl::identity,
                typename = etl::enable_if_t<!etl::is_range_v<I1>>>
      constexpr ranges::merge_result<I1, I2, O> operator()(I1 first1, S1 last1, I2 first2, S2 last2, O result, Comp comp = {}, Proj1 proj1 = {},
                                                           Proj2 proj2 = {}) const
      {
        while (first1 != last1 && first2 != last2)
        {
          if (etl::invoke(comp, etl::invoke(proj2, *first2), etl::invoke(proj1, *first1)))
          {
            *result = *first2;
            ++first2;
          }
          else
          {
            *result = *first1;
            ++first1;
          }
          ++result;
        }

        while (first1 != last1)
        {
          *result = *first1;
          ++first1;
          ++result;
        }

        while (first2 != last2)
        {
          *result = *first2;
          ++first2;
          ++result;
        }

        return {etl::move(first1), etl::move(first2), etl::move(result)};
      }

      template <class R1, class R2, class O, class Comp = ranges::less, class Proj1 = etl::identity, class Proj2 = etl::identity,
                typename = etl::enable_if_t<etl::is_range_v<R1>>>
      constexpr ranges::merge_result<ranges::borrowed_iterator_t<R1>, ranges::borrowed_iterator_t<R2>, O>
        operator()(R1&& r1, R2&& r2, O result, Comp comp = {}, Proj1 proj1 = {}, Proj2 proj2 = {}) const
      {
        return (*this)(ranges::begin(r1), ranges::end(r1), ranges::begin(r2), ranges::end(r2), etl::move(result), etl::move(comp), etl::move(proj1),
                       etl::move(proj2));
      }
    };

    inline constexpr merge_fn merge{};

    struct inplace_merge_fn
    {
      template <class I, class S, class Comp = ranges::less, class Proj = etl::identity, typename = etl::enable_if_t<!etl::is_range_v<I>>>
      constexpr I operator()(I first, I middle, S last, Comp comp = {}, Proj proj = {}) const
      {
        I last_it = ranges::next(first, last);

        if (first == middle || middle == last_it)
        {
          return last_it;
        }

        inplace_merge_impl(first, middle, last_it, comp, proj, etl::distance(first, middle), etl::distance(middle, last_it));

        return last_it;
      }

      template <class R, class Comp = ranges::less, class Proj = etl::identity, typename = etl::enable_if_t<etl::is_range_v<R>>>
      constexpr ranges::borrowed_iterator_t<R> operator()(R&& r, ranges::iterator_t<R> middle, Comp comp = {}, Proj proj = {}) const
      {
        return (*this)(ranges::begin(r), etl::move(middle), ranges::end(r), etl::move(comp), etl::move(proj));
      }

    private:

      template <class I, class Comp, class Proj>
      static constexpr void inplace_merge_impl(I first, I middle, I last, Comp& comp, Proj& proj,
                                               typename etl::iterator_traits<I>::difference_type len1,
                                               typename etl::iterator_traits<I>::difference_type len2)
      {
        if (len1 == 0 || len2 == 0)
        {
          return;
        }

        if (len1 + len2 == 2)
        {
          if (etl::invoke(comp, etl::invoke(proj, *middle), etl::invoke(proj, *first)))
          {
            etl::iter_swap(first, middle);
          }
          return;
        }

        I                                                 first_cut;
        I                                                 second_cut;
        typename etl::iterator_traits<I>::difference_type new_len1;
        typename etl::iterator_traits<I>::difference_type new_len2;

        if (len1 > len2)
        {
          new_len1   = len1 / 2;
          first_cut  = ranges::next(first, new_len1);
          second_cut = ranges::lower_bound(middle, last, etl::invoke(proj, *first_cut), etl::ref(comp), etl::ref(proj));
          new_len2   = etl::distance(middle, second_cut);
        }
        else
        {
          new_len2   = len2 / 2;
          second_cut = ranges::next(middle, new_len2);
          first_cut  = ranges::upper_bound(first, middle, etl::invoke(proj, *second_cut), etl::ref(comp), etl::ref(proj));
          new_len1   = etl::distance(first, first_cut);
        }

        I new_middle;
        // Due to a non-standard etl::rotate implementation, we need to handle
        // the case where one of the cuts is the middle separately to avoid
        // returning an iterator outside of [first, last)
        // As soon as etl::rotate is fixed to return an iterator in the middle
        // of the rotated range, this can be simplified to just calling
        // etl::rotate
        if (first_cut == middle)
        {
          new_middle = second_cut;
        }
        else if (second_cut == middle)
        {
          new_middle = first_cut;
        }
        else
        {
          new_middle = etl::rotate(first_cut, middle, second_cut);
        }

        inplace_merge_impl(first, first_cut, new_middle, comp, proj, new_len1, new_len2);
        inplace_merge_impl(new_middle, second_cut, last, comp, proj, len1 - new_len1, len2 - new_len2);
      }
    };

    inline constexpr inplace_merge_fn inplace_merge{};

    struct set_union_fn
    {
      template <class I1, class S1, class I2, class S2, class O, class Comp = ranges::less, class Proj1 = etl::identity, class Proj2 = etl::identity,
                typename = etl::enable_if_t<!etl::is_range_v<I1>>>
      constexpr ranges::set_union_result<I1, I2, O> operator()(I1 first1, S1 last1, I2 first2, S2 last2, O result, Comp comp = {}, Proj1 proj1 = {},
                                                               Proj2 proj2 = {}) const
      {
        while (first1 != last1 && first2 != last2)
        {
          if (etl::invoke(comp, etl::invoke(proj1, *first1), etl::invoke(proj2, *first2)))
          {
            *result = *first1;
            ++first1;
          }
          else if (etl::invoke(comp, etl::invoke(proj2, *first2), etl::invoke(proj1, *first1)))
          {
            *result = *first2;
            ++first2;
          }
          else
          {
            *result = *first1;
            ++first1;
            ++first2;
          }
          ++result;
        }

        while (first1 != last1)
        {
          *result = *first1;
          ++first1;
          ++result;
        }

        while (first2 != last2)
        {
          *result = *first2;
          ++first2;
          ++result;
        }

        return {etl::move(first1), etl::move(first2), etl::move(result)};
      }

      template <class R1, class R2, class O, class Comp = ranges::less, class Proj1 = etl::identity, class Proj2 = etl::identity,
                typename = etl::enable_if_t<etl::is_range_v<R1>>>
      constexpr ranges::set_union_result<ranges::borrowed_iterator_t<R1>, ranges::borrowed_iterator_t<R2>, O>
        operator()(R1&& r1, R2&& r2, O result, Comp comp = {}, Proj1 proj1 = {}, Proj2 proj2 = {}) const
      {
        return (*this)(ranges::begin(r1), ranges::end(r1), ranges::begin(r2), ranges::end(r2), etl::move(result), etl::move(comp), etl::move(proj1),
                       etl::move(proj2));
      }
    };

    inline constexpr set_union_fn set_union{};

    struct set_intersection_fn
    {
      template <class I1, class S1, class I2, class S2, class O, class Comp = ranges::less, class Proj1 = etl::identity, class Proj2 = etl::identity,
                typename = etl::enable_if_t<!etl::is_range_v<I1>>>
      constexpr ranges::set_intersection_result<I1, I2, O> operator()(I1 first1, S1 last1, I2 first2, S2 last2, O result, Comp comp = {},
                                                                      Proj1 proj1 = {}, Proj2 proj2 = {}) const
      {
        while (first1 != last1 && first2 != last2)
        {
          if (etl::invoke(comp, etl::invoke(proj1, *first1), etl::invoke(proj2, *first2)))
          {
            ++first1;
          }
          else if (etl::invoke(comp, etl::invoke(proj2, *first2), etl::invoke(proj1, *first1)))
          {
            ++first2;
          }
          else
          {
            *result = *first1;
            ++first1;
            ++first2;
            ++result;
          }
        }

        return {etl::move(first1), etl::move(first2), etl::move(result)};
      }

      template <class R1, class R2, class O, class Comp = ranges::less, class Proj1 = etl::identity, class Proj2 = etl::identity,
                typename = etl::enable_if_t<etl::is_range_v<R1>>>
      constexpr ranges::set_intersection_result< ranges::borrowed_iterator_t<R1>, ranges::borrowed_iterator_t<R2>, O>
        operator()(R1&& r1, R2&& r2, O result, Comp comp = {}, Proj1 proj1 = {}, Proj2 proj2 = {}) const
      {
        return (*this)(ranges::begin(r1), ranges::end(r1), ranges::begin(r2), ranges::end(r2), etl::move(result), etl::move(comp), etl::move(proj1),
                       etl::move(proj2));
      }
    };

    inline constexpr set_intersection_fn set_intersection{};

    struct set_difference_fn
    {
      template <class I1, class S1, class I2, class S2, class O, class Comp = ranges::less, class Proj1 = etl::identity, class Proj2 = etl::identity,
                typename = etl::enable_if_t<!etl::is_range_v<I1>>>
      constexpr ranges::set_difference_result<I1, O> operator()(I1 first1, S1 last1, I2 first2, S2 last2, O result, Comp comp = {}, Proj1 proj1 = {},
                                                                Proj2 proj2 = {}) const
      {
        while (first1 != last1 && first2 != last2)
        {
          if (etl::invoke(comp, etl::invoke(proj1, *first1), etl::invoke(proj2, *first2)))
          {
            *result = *first1;
            ++first1;
            ++result;
          }
          else if (etl::invoke(comp, etl::invoke(proj2, *first2), etl::invoke(proj1, *first1)))
          {
            ++first2;
          }
          else
          {
            ++first1;
            ++first2;
          }
        }

        while (first1 != last1)
        {
          *result = *first1;
          ++first1;
          ++result;
        }

        return {etl::move(first1), etl::move(result)};
      }

      template <class R1, class R2, class O, class Comp = ranges::less, class Proj1 = etl::identity, class Proj2 = etl::identity,
                typename = etl::enable_if_t<etl::is_range_v<R1>>>
      constexpr ranges::set_difference_result<ranges::borrowed_iterator_t<R1>, O> operator()(R1&& r1, R2&& r2, O result, Comp comp = {},
                                                                                             Proj1 proj1 = {}, Proj2 proj2 = {}) const
      {
        return (*this)(ranges::begin(r1), ranges::end(r1), ranges::begin(r2), ranges::end(r2), etl::move(result), etl::move(comp), etl::move(proj1),
                       etl::move(proj2));
      }
    };

    inline constexpr set_difference_fn set_difference{};

    struct set_symmetric_difference_fn
    {
      template <class I1, class S1, class I2, class S2, class O, class Comp = ranges::less, class Proj1 = etl::identity, class Proj2 = etl::identity,
                typename = etl::enable_if_t<!etl::is_range_v<I1>>>
      constexpr ranges::set_symmetric_difference_result<I1, I2, O> operator()(I1 first1, S1 last1, I2 first2, S2 last2, O result, Comp comp = {},
                                                                              Proj1 proj1 = {}, Proj2 proj2 = {}) const
      {
        while (first1 != last1 && first2 != last2)
        {
          if (etl::invoke(comp, etl::invoke(proj1, *first1), etl::invoke(proj2, *first2)))
          {
            *result = *first1;
            ++first1;
            ++result;
          }
          else if (etl::invoke(comp, etl::invoke(proj2, *first2), etl::invoke(proj1, *first1)))
          {
            *result = *first2;
            ++first2;
            ++result;
          }
          else
          {
            ++first1;
            ++first2;
          }
        }

        while (first1 != last1)
        {
          *result = *first1;
          ++first1;
          ++result;
        }

        while (first2 != last2)
        {
          *result = *first2;
          ++first2;
          ++result;
        }

        return {etl::move(first1), etl::move(first2), etl::move(result)};
      }

      template <class R1, class R2, class O, class Comp = ranges::less, class Proj1 = etl::identity, class Proj2 = etl::identity,
                typename = etl::enable_if_t<etl::is_range_v<R1>>>
      constexpr ranges::set_symmetric_difference_result< ranges::borrowed_iterator_t<R1>, ranges::borrowed_iterator_t<R2>, O>
        operator()(R1&& r1, R2&& r2, O result, Comp comp = {}, Proj1 proj1 = {}, Proj2 proj2 = {}) const
      {
        return (*this)(ranges::begin(r1), ranges::end(r1), ranges::begin(r2), ranges::end(r2), etl::move(result), etl::move(comp), etl::move(proj1),
                       etl::move(proj2));
      }
    };

    inline constexpr set_symmetric_difference_fn set_symmetric_difference{};

    struct make_heap_fn
    {
    private:

      template <class I, class Comp, class Proj>
      static constexpr void sift_down(I first, typename etl::iterator_traits<I>::difference_type index,
                                      typename etl::iterator_traits<I>::difference_type length, Comp& comp, Proj& proj)
      {
        while (true)
        {
          auto child = 2 * index + 1;

          if (child >= length)
          {
            break;
          }

          if ((child + 1 < length) && etl::invoke(comp, etl::invoke(proj, *(first + child)), etl::invoke(proj, *(first + (child + 1)))))
          {
            ++child;
          }

          if (!etl::invoke(comp, etl::invoke(proj, *(first + index)), etl::invoke(proj, *(first + child))))
          {
            break;
          }

          etl::iter_swap(first + index, first + child);
          index = child;
        }
      }

    public:

      template <class I, class S, class Comp = ranges::less, class Proj = etl::identity, typename = etl::enable_if_t<!etl::is_range_v<I>>>
      constexpr I operator()(I first, S last, Comp comp = {}, Proj proj = {}) const
      {
        I last_it = ranges::next(first, last);

        auto length = etl::distance(first, last_it);

        if (length < 2)
        {
          return last_it;
        }

        auto parent = (length - 2) / 2;

        while (true)
        {
          sift_down(first, parent, length, comp, proj);

          if (parent == 0)
          {
            break;
          }

          --parent;
        }

        return last_it;
      }

      template <class R, class Comp = ranges::less, class Proj = etl::identity, typename = etl::enable_if_t<etl::is_range_v<R>>>
      constexpr ranges::borrowed_iterator_t<R> operator()(R&& r, Comp comp = {}, Proj proj = {}) const
      {
        return (*this)(ranges::begin(r), ranges::end(r), etl::move(comp), etl::move(proj));
      }
    };

    inline constexpr make_heap_fn make_heap{};

    struct push_heap_fn
    {
      template <class I, class S, class Comp = ranges::less, class Proj = etl::identity, typename = etl::enable_if_t<!etl::is_range_v<I>>>
      constexpr I operator()(I first, S last, Comp comp = {}, Proj proj = {}) const
      {
        ETL_STATIC_ASSERT(etl::is_random_access_iterator<I>::value, "push_heap requires random access iterators");

        I last_it = ranges::next(first, last);

        auto length = etl::distance(first, last_it);

        if (length < 2)
        {
          return last_it;
        }

        auto value_index = length - 1;
        auto parent      = (value_index - 1) / 2;
        auto value       = etl::move(*(first + value_index));

        while ((value_index > 0) && etl::invoke(comp, etl::invoke(proj, *(first + parent)), etl::invoke(proj, value)))
        {
          *(first + value_index) = etl::move(*(first + parent));
          value_index            = parent;
          parent                 = (value_index - 1) / 2;
        }

        *(first + value_index) = etl::move(value);

        return last_it;
      }

      template <class R, class Comp = ranges::less, class Proj = etl::identity, typename = etl::enable_if_t<etl::is_range_v<R>>>
      constexpr ranges::borrowed_iterator_t<R> operator()(R&& r, Comp comp = {}, Proj proj = {}) const
      {
        return (*this)(ranges::begin(r), ranges::end(r), etl::move(comp), etl::move(proj));
      }
    };

    inline constexpr push_heap_fn push_heap{};

    struct pop_heap_fn
    {
    private:

      template <class I, class Comp, class Proj>
      static constexpr void sift_down(I first, typename etl::iterator_traits<I>::difference_type index,
                                      typename etl::iterator_traits<I>::difference_type length, Comp& comp, Proj& proj)
      {
        while (true)
        {
          auto child = 2 * index + 1;

          if (child >= length)
          {
            break;
          }

          if ((child + 1 < length) && etl::invoke(comp, etl::invoke(proj, *(first + child)), etl::invoke(proj, *(first + (child + 1)))))
          {
            ++child;
          }

          if (!etl::invoke(comp, etl::invoke(proj, *(first + index)), etl::invoke(proj, *(first + child))))
          {
            break;
          }

          etl::iter_swap(first + index, first + child);
          index = child;
        }
      }

    public:

      template <class I, class S, class Comp = ranges::less, class Proj = etl::identity, typename = etl::enable_if_t<!etl::is_range_v<I>>>
      constexpr I operator()(I first, S last, Comp comp = {}, Proj proj = {}) const
      {
        ETL_STATIC_ASSERT(etl::is_random_access_iterator<I>::value, "pop_heap requires random access iterators");

        I last_it = ranges::next(first, last);

        auto length = etl::distance(first, last_it);

        if (length < 2)
        {
          return last_it;
        }

        --last_it;

        etl::iter_swap(first, last_it);

        sift_down(first, decltype(length)(0), etl::distance(first, last_it), comp, proj);

        return ranges::next(first, last);
      }

      template <class R, class Comp = ranges::less, class Proj = etl::identity, typename = etl::enable_if_t<etl::is_range_v<R>>>
      constexpr ranges::borrowed_iterator_t<R> operator()(R&& r, Comp comp = {}, Proj proj = {}) const
      {
        return (*this)(ranges::begin(r), ranges::end(r), etl::move(comp), etl::move(proj));
      }
    };

    inline constexpr pop_heap_fn pop_heap{};

    struct is_heap_until_fn
    {
      template <class I, class S, class Comp = ranges::less, class Proj = etl::identity, typename = etl::enable_if_t<!etl::is_range_v<I>>>
      constexpr I operator()(I first, S last, Comp comp = {}, Proj proj = {}) const
      {
        ETL_STATIC_ASSERT(etl::is_random_access_iterator<I>::value, "is_heap_until requires random access iterators");

        I last_it = ranges::next(first, last);

        auto length = etl::distance(first, last_it);

        decltype(length) parent = 0;

        for (decltype(length) child = 1; child < length; ++child)
        {
          if (etl::invoke(comp, etl::invoke(proj, *(first + parent)), etl::invoke(proj, *(first + child))))
          {
            return first + child;
          }

          if ((child & 1) == 0)
          {
            ++parent;
          }
        }

        return last_it;
      }

      template <class R, class Comp = ranges::less, class Proj = etl::identity, typename = etl::enable_if_t<etl::is_range_v<R>>>
      constexpr ranges::borrowed_iterator_t<R> operator()(R&& r, Comp comp = {}, Proj proj = {}) const
      {
        return (*this)(ranges::begin(r), ranges::end(r), etl::move(comp), etl::move(proj));
      }
    };

    inline constexpr is_heap_until_fn is_heap_until{};

    struct is_heap_fn
    {
      template <class I, class S, class Comp = ranges::less, class Proj = etl::identity, typename = etl::enable_if_t<!etl::is_range_v<I>>>
      constexpr bool operator()(I first, S last, Comp comp = {}, Proj proj = {}) const
      {
        return ranges::is_heap_until(first, last, etl::ref(comp), etl::ref(proj)) == last;
      }

      template <class R, class Comp = ranges::less, class Proj = etl::identity, typename = etl::enable_if_t<etl::is_range_v<R>>>
      constexpr bool operator()(R&& r, Comp comp = {}, Proj proj = {}) const
      {
        return (*this)(ranges::begin(r), ranges::end(r), etl::move(comp), etl::move(proj));
      }
    };

    inline constexpr is_heap_fn is_heap{};

    struct sort_heap_fn
    {
      template <class I, class S, class Comp = ranges::less, class Proj = etl::identity, typename = etl::enable_if_t<!etl::is_range_v<I>>>
      constexpr I operator()(I first, S last, Comp comp = {}, Proj proj = {}) const
      {
        ETL_STATIC_ASSERT(etl::is_random_access_iterator<I>::value, "sort_heap requires random access iterators");

        I last_it      = ranges::next(first, last);
        I current_last = last_it;

        while (first != current_last)
        {
          ranges::pop_heap(first, current_last, comp, proj);
          --current_last;
        }

        return last_it;
      }

      template <class R, class Comp = ranges::less, class Proj = etl::identity, typename = etl::enable_if_t<etl::is_range_v<R>>>
      constexpr ranges::borrowed_iterator_t<R> operator()(R&& r, Comp comp = {}, Proj proj = {}) const
      {
        return (*this)(ranges::begin(r), ranges::end(r), etl::move(comp), etl::move(proj));
      }
    };

    inline constexpr sort_heap_fn sort_heap{};

    struct min_fn
    {
      template <class T, class Comp = ranges::less, class Proj = etl::identity>
      constexpr const T& operator()(const T& a, const T& b, Comp comp = {}, Proj proj = {}) const
      {
        return etl::invoke(comp, etl::invoke(proj, b), etl::invoke(proj, a)) ? b : a;
      }

  #if ETL_HAS_INITIALIZER_LIST
      template <class T, class Comp = ranges::less, class Proj = etl::identity>
      constexpr T operator()(std::initializer_list<T> r, Comp comp = {}, Proj proj = {}) const
      {
        auto first = r.begin();
        auto last  = r.end();

        auto smallest = first;
        while (++first != last)
        {
          if (etl::invoke(comp, etl::invoke(proj, *first), etl::invoke(proj, *smallest)))
          {
            smallest = first;
          }
        }
        return *smallest;
      }
  #endif

      template <class R, class Comp = ranges::less, class Proj = etl::identity, typename = etl::enable_if_t<etl::is_range_v<R>>>
      constexpr ranges::range_value_t<R> operator()(R&& r, Comp comp = {}, Proj proj = {}) const
      {
        auto first = ranges::begin(r);
        auto last  = ranges::end(r);

        auto smallest = first;
        while (++first != last)
        {
          if (etl::invoke(comp, etl::invoke(proj, *first), etl::invoke(proj, *smallest)))
          {
            smallest = first;
          }
        }
        return *smallest;
      }
    };

    inline constexpr min_fn min{};

    struct min_element_fn
    {
      template <class I, class S, class Comp = ranges::less, class Proj = etl::identity, typename = etl::enable_if_t<!etl::is_range_v<I>>>
      constexpr I operator()(I first, S last, Comp comp = {}, Proj proj = {}) const
      {
        if (first == last)
        {
          return first;
        }

        I smallest = first;
        ++first;

        for (; first != last; ++first)
        {
          if (etl::invoke(comp, etl::invoke(proj, *first), etl::invoke(proj, *smallest)))
          {
            smallest = first;
          }
        }

        return smallest;
      }

      template <class R, class Comp = ranges::less, class Proj = etl::identity, typename = etl::enable_if_t<etl::is_range_v<R>>>
      constexpr ranges::borrowed_iterator_t<R> operator()(R&& r, Comp comp = {}, Proj proj = {}) const
      {
        return (*this)(ranges::begin(r), ranges::end(r), etl::move(comp), etl::move(proj));
      }
    };

    inline constexpr min_element_fn min_element{};

    struct max_fn
    {
      template <class T, class Comp = ranges::less, class Proj = etl::identity>
      constexpr const T& operator()(const T& a, const T& b, Comp comp = {}, Proj proj = {}) const
      {
        return etl::invoke(comp, etl::invoke(proj, a), etl::invoke(proj, b)) ? b : a;
      }

  #if ETL_HAS_INITIALIZER_LIST
      template <class T, class Comp = ranges::less, class Proj = etl::identity>
      constexpr T operator()(std::initializer_list<T> r, Comp comp = {}, Proj proj = {}) const
      {
        auto first = r.begin();
        auto last  = r.end();

        auto largest = first;
        while (++first != last)
        {
          if (etl::invoke(comp, etl::invoke(proj, *largest), etl::invoke(proj, *first)))
          {
            largest = first;
          }
        }
        return *largest;
      }
  #endif

      template <class R, class Comp = ranges::less, class Proj = etl::identity, typename = etl::enable_if_t<etl::is_range_v<R>>>
      constexpr ranges::range_value_t<R> operator()(R&& r, Comp comp = {}, Proj proj = {}) const
      {
        auto first = ranges::begin(r);
        auto last  = ranges::end(r);

        auto largest = first;
        while (++first != last)
        {
          if (etl::invoke(comp, etl::invoke(proj, *largest), etl::invoke(proj, *first)))
          {
            largest = first;
          }
        }
        return *largest;
      }
    };

    inline constexpr max_fn max{};

    struct max_element_fn
    {
      template <class I, class S, class Comp = ranges::less, class Proj = etl::identity, typename = etl::enable_if_t<!etl::is_range_v<I>>>
      constexpr I operator()(I first, S last, Comp comp = {}, Proj proj = {}) const
      {
        if (first == last)
        {
          return first;
        }

        I largest = first;
        ++first;

        for (; first != last; ++first)
        {
          if (etl::invoke(comp, etl::invoke(proj, *largest), etl::invoke(proj, *first)))
          {
            largest = first;
          }
        }

        return largest;
      }

      template <class R, class Comp = ranges::less, class Proj = etl::identity, typename = etl::enable_if_t<etl::is_range_v<R>>>
      constexpr ranges::borrowed_iterator_t<R> operator()(R&& r, Comp comp = {}, Proj proj = {}) const
      {
        return (*this)(ranges::begin(r), ranges::end(r), etl::move(comp), etl::move(proj));
      }
    };

    inline constexpr max_element_fn max_element{};

    struct minmax_fn
    {
      template <class T, class Comp = ranges::less, class Proj = etl::identity>
      constexpr ranges::minmax_result<const T&> operator()(const T& a, const T& b, Comp comp = {}, Proj proj = {}) const
      {
        if (etl::invoke(comp, etl::invoke(proj, b), etl::invoke(proj, a)))
        {
          return {b, a};
        }
        return {a, b};
      }

  #if ETL_HAS_INITIALIZER_LIST
      template <class T, class Comp = ranges::less, class Proj = etl::identity>
      constexpr ranges::minmax_result<T> operator()(std::initializer_list<T> r, Comp comp = {}, Proj proj = {}) const
      {
        auto first = r.begin();
        auto last  = r.end();

        auto smallest = first;
        auto largest  = first;

        while (++first != last)
        {
          if (etl::invoke(comp, etl::invoke(proj, *first), etl::invoke(proj, *smallest)))
          {
            smallest = first;
          }
          if (etl::invoke(comp, etl::invoke(proj, *largest), etl::invoke(proj, *first)))
          {
            largest = first;
          }
        }
        return {*smallest, *largest};
      }
  #endif

      template <class R, class Comp = ranges::less, class Proj = etl::identity, typename = etl::enable_if_t<etl::is_range_v<R>>>
      constexpr ranges::minmax_result<ranges::range_value_t<R>> operator()(R&& r, Comp comp = {}, Proj proj = {}) const
      {
        auto first = ranges::begin(r);
        auto last  = ranges::end(r);

        auto smallest = first;
        auto largest  = first;

        while (++first != last)
        {
          if (etl::invoke(comp, etl::invoke(proj, *first), etl::invoke(proj, *smallest)))
          {
            smallest = first;
          }
          if (etl::invoke(comp, etl::invoke(proj, *largest), etl::invoke(proj, *first)))
          {
            largest = first;
          }
        }
        return {*smallest, *largest};
      }
    };

    inline constexpr minmax_fn minmax{};

    struct minmax_element_fn
    {
      template <class I, class S, class Comp = ranges::less, class Proj = etl::identity, typename = etl::enable_if_t<!etl::is_range_v<I>>>
      constexpr ranges::minmax_element_result<I> operator()(I first, S last, Comp comp = {}, Proj proj = {}) const
      {
        if (first == last)
        {
          return {first, first};
        }

        I smallest = first;
        I largest  = first;
        ++first;

        for (; first != last; ++first)
        {
          if (etl::invoke(comp, etl::invoke(proj, *first), etl::invoke(proj, *smallest)))
          {
            smallest = first;
          }
          if (etl::invoke(comp, etl::invoke(proj, *largest), etl::invoke(proj, *first)))
          {
            largest = first;
          }
        }

        return {smallest, largest};
      }

      template <class R, class Comp = ranges::less, class Proj = etl::identity, typename = etl::enable_if_t<etl::is_range_v<R>>>
      constexpr ranges::minmax_element_result<ranges::borrowed_iterator_t<R>> operator()(R&& r, Comp comp = {}, Proj proj = {}) const
      {
        return (*this)(ranges::begin(r), ranges::end(r), etl::move(comp), etl::move(proj));
      }
    };

    inline constexpr minmax_element_fn minmax_element{};

    struct clamp_fn
    {
      template <class T, class Comp = ranges::less, class Proj = etl::identity>
      constexpr const T& operator()(const T& value, const T& low, const T& high, Comp comp = {}, Proj proj = {}) const
      {
        auto&& projected_value = etl::invoke(proj, value);

        return etl::invoke(comp, projected_value, etl::invoke(proj, low))    ? low
               : etl::invoke(comp, etl::invoke(proj, high), projected_value) ? high
                                                                             : value;
      }
    };

    inline constexpr clamp_fn clamp{};

    struct next_permutation_fn
    {
      template <class I, class S, class Comp = ranges::less, class Proj = etl::identity, typename = etl::enable_if_t<!etl::is_range_v<I>>>
      constexpr ranges::next_permutation_result<I> operator()(I first, S last, Comp comp = {}, Proj proj = {}) const
      {
        I last_it = ranges::next(first, last);

        // Empty or single-element range: already at last permutation
        if (first == last_it)
        {
          return {etl::move(last_it), false};
        }

        I i = last_it;
        --i;

        if (i == first)
        {
          return {etl::move(last_it), false};
        }

        for (;;)
        {
          I i1 = i;
          --i;

          // Find the rightmost element where projected *i < projected *i1
          if (etl::invoke(comp, etl::invoke(proj, *i), etl::invoke(proj, *i1)))
          {
            // Find the rightmost element j where projected *j > projected *i
            I j = last_it;
            while (!etl::invoke(comp, etl::invoke(proj, *i), etl::invoke(proj, *--j)))
            {
            }

            etl::iter_swap(i, j);

            // Reverse from i1 to last
            I left  = i1;
            I right = last_it;
            while (left != right && left != --right)
            {
              etl::iter_swap(left, right);
              ++left;
            }

            return {etl::move(last_it), true};
          }

          if (i == first)
          {
            // Already at last (ascending) permutation: wrap to first
            // (descending)
            I left  = first;
            I right = last_it;
            while (left != right && left != --right)
            {
              etl::iter_swap(left, right);
              ++left;
            }

            return {etl::move(last_it), false};
          }
        }
      }

      template <class R, class Comp = ranges::less, class Proj = etl::identity, typename = etl::enable_if_t<etl::is_range_v<R>>>
      constexpr ranges::next_permutation_result<ranges::borrowed_iterator_t<R>> operator()(R&& r, Comp comp = {}, Proj proj = {}) const
      {
        return (*this)(ranges::begin(r), ranges::end(r), etl::move(comp), etl::move(proj));
      }
    };

    inline constexpr next_permutation_fn next_permutation{};

    struct prev_permutation_fn
    {
      template <class I, class S, class Comp = ranges::less, class Proj = etl::identity, typename = etl::enable_if_t<!etl::is_range_v<I>>>
      constexpr ranges::prev_permutation_result<I> operator()(I first, S last, Comp comp = {}, Proj proj = {}) const
      {
        I last_it = ranges::next(first, last);

        // Empty or single-element range: already at last permutation
        if (first == last_it)
        {
          return {etl::move(last_it), false};
        }

        I i = last_it;
        --i;

        if (i == first)
        {
          return {etl::move(last_it), false};
        }

        for (;;)
        {
          I i1 = i;
          --i;

          // Find the rightmost element where projected *i > projected *i1
          if (etl::invoke(comp, etl::invoke(proj, *i1), etl::invoke(proj, *i)))
          {
            // Find the rightmost element j where projected *j < projected *i
            I j = last_it;
            while (!etl::invoke(comp, etl::invoke(proj, *--j), etl::invoke(proj, *i)))
            {
            }

            etl::iter_swap(i, j);

            // Reverse from i1 to last
            I left  = i1;
            I right = last_it;
            while (left != right && left != --right)
            {
              etl::iter_swap(left, right);
              ++left;
            }

            return {etl::move(last_it), true};
          }

          if (i == first)
          {
            // Already at last (descending) permutation: wrap to first
            // (ascending)
            I left  = first;
            I right = last_it;
            while (left != right && left != --right)
            {
              etl::iter_swap(left, right);
              ++left;
            }

            return {etl::move(last_it), false};
          }
        }
      }

      template <class R, class Comp = ranges::less, class Proj = etl::identity, typename = etl::enable_if_t<etl::is_range_v<R>>>
      constexpr ranges::prev_permutation_result<ranges::borrowed_iterator_t<R>> operator()(R&& r, Comp comp = {}, Proj proj = {}) const
      {
        return (*this)(ranges::begin(r), ranges::end(r), etl::move(comp), etl::move(proj));
      }
    };

    inline constexpr prev_permutation_fn prev_permutation{};
  } // namespace ranges
#endif

} // namespace etl

#include "private/minmax_pop.h"

#endif
