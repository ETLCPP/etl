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
/// Including reverse engineered algorithms from C++ 0x11, 0x14, 0x17
/// Additional new variants of certain algorithms.
///\ingroup utilities

#include "platform.h"
#include "type_traits.h"
#include "iterator.h"
#include "functional.h"
#include "utility.h"

#include <stdint.h>
#include <string.h>

#include "private/minmax_push.h"

#if ETL_USING_STL
  #include <algorithm>
  #include <utility>
  #include <iterator>
  #include <functional>
  #include <numeric>
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
  void shell_sort(TIterator first, TIterator last);

  template <typename TIterator, typename TCompare>
#if ETL_USING_STD_NAMESPACE
  ETL_CONSTEXPR20
#else
  ETL_CONSTEXPR14
#endif
  void shell_sort(TIterator first, TIterator last, TCompare compare);

  template <typename TIterator>
  ETL_CONSTEXPR14 void insertion_sort(TIterator first, TIterator last);

  template <typename TIterator, typename TCompare>
  ETL_CONSTEXPR14 void insertion_sort(TIterator first, TIterator last, TCompare compare);
}

//*****************************************************************************
// Algorithms defined by the ETL
//*****************************************************************************
namespace etl
{
  //***************************************************************************
  // iter_swap
  //***************************************************************************
  template <typename TIterator1, typename TIterator2>
#if ETL_USING_STD_NAMESPACE
  ETL_CONSTEXPR20
#else
  ETL_CONSTEXPR14
#endif
  void iter_swap(TIterator1 a, TIterator2 b)
  {
    using ETL_OR_STD::swap; // Allow ADL
    swap(*a, *b);
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
  TIterator2 swap_ranges(TIterator1 first1,
                         TIterator1 last1,
                         TIterator2 first2)
  {
    while (first1 != last1)
    {
      iter_swap(first1, first2);
      ++first1;
      ++first2;
    }

    return first2;
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
  ETL_CONSTEXPR14
  TIterator2 reverse_copy(TIterator1 sb, TIterator1 se, TIterator2 db)
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
    return std::move_backward(sb, se, de);
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
  typename etl::enable_if<etl::is_pointer<TIterator>::value, void>::type
    reverse(TIterator b, TIterator e)
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
  typename etl::enable_if<!etl::is_pointer<TIterator>::value, void>::type
    reverse(TIterator b, TIterator e)
  {
    while ((b != e) && (b != --e))
    {
      etl::iter_swap(b++, e);
    }
  }

  //***************************************************************************
  // lower_bound
  //***************************************************************************
  template<typename TIterator, typename TValue, typename TCompare>
  ETL_NODISCARD
  ETL_CONSTEXPR14
  TIterator lower_bound(TIterator first, TIterator last, const TValue& value, TCompare compare)
  {
    typedef typename etl::iterator_traits<TIterator>::difference_type difference_t;

    difference_t count = etl::distance(first, last);

    while (count > 0)
    {
      TIterator    itr = first;
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

  template<typename TIterator, typename TValue>
  ETL_NODISCARD
  ETL_CONSTEXPR14
  TIterator lower_bound(TIterator first, TIterator last, const TValue& value)
  {
    typedef etl::less<typename etl::iterator_traits<TIterator>::value_type> compare;

    return etl::lower_bound(first, last, value, compare());
  }

  //***************************************************************************
  // upper_bound
  //***************************************************************************
  template<typename TIterator, typename TValue, typename TCompare>
  ETL_NODISCARD
  ETL_CONSTEXPR14
  TIterator upper_bound(TIterator first, TIterator last, const TValue& value, TCompare compare)
  {
    typedef typename etl::iterator_traits<TIterator>::difference_type difference_t;

    difference_t count = etl::distance(first, last);

    while (count > 0)
    {
      TIterator    itr = first;
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

  template<typename TIterator, typename TValue>
  ETL_NODISCARD
  ETL_CONSTEXPR14
  TIterator upper_bound(TIterator first, TIterator last, const TValue& value)
  {
    typedef etl::less<typename etl::iterator_traits<TIterator>::value_type> compare;

    return etl::upper_bound(first, last, value, compare());
  }

  //***************************************************************************
  // equal_range
  //***************************************************************************
  template<typename TIterator, typename TValue, typename TCompare>
  ETL_NODISCARD
  ETL_CONSTEXPR14
  ETL_OR_STD::pair<TIterator, TIterator> equal_range(TIterator first, TIterator last, const TValue& value, TCompare compare)
  {
    return ETL_OR_STD::make_pair(etl::lower_bound(first, last, value, compare),
                                 etl::upper_bound(first, last, value, compare));
  }

  template<typename TIterator, typename TValue>
  ETL_NODISCARD
  ETL_OR_STD::pair<TIterator, TIterator> equal_range(TIterator first, TIterator last, const TValue& value)
  {
    typedef etl::less<typename etl::iterator_traits<TIterator>::value_type> compare;

    return ETL_OR_STD::make_pair(etl::lower_bound(first, last, value, compare()),
                                 etl::upper_bound(first, last, value, compare()));
  }

  //***************************************************************************
  // binary_search
  //***************************************************************************
  template <typename TIterator, typename T, typename Compare>
  ETL_NODISCARD
  bool binary_search(TIterator first, TIterator last, const T& value, Compare compare)
  {
    first = etl::lower_bound(first, last, value, compare);

    return (!(first == last) && !(compare(value, *first)));
  }

  template <typename TIterator, typename T>
  ETL_NODISCARD
  bool binary_search(TIterator first, TIterator last, const T& value)
  {
    typedef etl::less<typename etl::iterator_traits<TIterator>::value_type> compare;

    return binary_search(first, last, value, compare());
  }

  //***************************************************************************
  // find_if
  //***************************************************************************
  template <typename TIterator, typename TUnaryPredicate>
  ETL_NODISCARD
  ETL_CONSTEXPR14
  TIterator find_if(TIterator first, TIterator last, TUnaryPredicate predicate)
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
  ETL_NODISCARD
  ETL_CONSTEXPR14
  TIterator find(TIterator first, TIterator last, const T& value)
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
  template<typename TIterator, typename TValue>
  constexpr void fill(TIterator first, TIterator last, const TValue& value)
  {
    std::fill(first, last, value);
  }
#else
  template<typename TIterator, typename TValue>
  ETL_CONSTEXPR14 void fill(TIterator first, TIterator last, const TValue& value)
  {
    while (first != last)
    {
      *first = value;
      ++first;
    }
  }
#endif

  //***************************************************************************
  // fill_n
#if ETL_USING_STL && ETL_USING_CPP20
  template<typename TIterator, typename TSize, typename TValue>
  constexpr TIterator fill_n(TIterator first, TSize count, const TValue& value)
  {
    return std::fill_n(first, count, value);
  }
#else
  template<typename TIterator, typename TSize, typename TValue>
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
  ETL_NODISCARD
  ETL_CONSTEXPR14
  typename etl::iterator_traits<TIterator>::difference_type count(TIterator first, TIterator last, const T& value)
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
  ETL_NODISCARD
  ETL_CONSTEXPR14
  typename etl::iterator_traits<TIterator>::difference_type 
    count_if(TIterator first, TIterator last, TUnaryPredicate predicate)
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
  constexpr
  bool equal(TIterator1 first1, TIterator1 last1, TIterator2 first2)
  {
    return std::equal(first1, last1, first2);
  }

  // Three parameter + predicate
  template <typename TIterator1, typename TIterator2, typename TPredicate>
  [[nodiscard]]
  constexpr
  bool equal(TIterator1 first1, TIterator1 last1, TIterator2 first2, TPredicate predicate)
  {
    return std::equal(first1, last1, first2, predicate);
  }

  // Four parameter
  template <typename TIterator1, typename TIterator2>
  [[nodiscard]]
  constexpr
  bool equal(TIterator1 first1, TIterator1 last1, TIterator2 first2, TIterator2 last2)
  {
    return std::equal(first1, last1, first2, last2);
  }

  // Four parameter + Predicate
  template <typename TIterator1, typename TIterator2, typename TPredicate>
  [[nodiscard]]
  constexpr
  bool equal(TIterator1 first1, TIterator1 last1, TIterator2 first2, TIterator2 last2, TPredicate predicate)
  {
    return std::equal(first1, last1, first2, last2, predicate);
  }

#else

  template <typename TIterator1, typename TIterator2>
  ETL_NODISCARD
  ETL_CONSTEXPR14
  bool equal(TIterator1 first1, TIterator1 last1, TIterator2 first2)
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
  ETL_NODISCARD
  ETL_CONSTEXPR14 
  bool equal(TIterator1 first1, TIterator1 last1, TIterator2 first2, TPredicate predicate)
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
  ETL_NODISCARD
  ETL_CONSTEXPR14
  bool equal(TIterator1 first1, TIterator1 last1, TIterator2 first2, TIterator2 last2)
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
  ETL_NODISCARD
  ETL_CONSTEXPR14
  bool equal(TIterator1 first1, TIterator1 last1, TIterator2 first2, TIterator2 last2, TPredicate predicate)
  {
    while ((first1 != last1) && (first2 != last2))
    {
      if (!predicate(*first1 , *first2))
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
  ETL_NODISCARD
  ETL_CONSTEXPR14
  bool lexicographical_compare(TIterator1 first1, TIterator1 last1,
                               TIterator2 first2, TIterator2 last2,
                               TCompare compare)
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
  ETL_NODISCARD
  ETL_CONSTEXPR14
  bool lexicographical_compare(TIterator1 first1, TIterator1 last1,
                               TIterator2 first2, TIterator2 last2)
  {
    typedef etl::less<typename etl::iterator_traits<TIterator1>::value_type> compare;

    return etl::lexicographical_compare(first1, last1, first2, last2, compare());
  }

  //***************************************************************************
  // min
  //***************************************************************************
  template <typename T, typename TCompare>
  ETL_NODISCARD
  ETL_CONSTEXPR 
  const T& min(const T& a, const T& b, TCompare compare)
  {
    return (compare(a, b)) ? a : b;
  }

  template <typename T>
  ETL_NODISCARD
  ETL_CONSTEXPR 
  const T& min(const T& a, const T& b)
  {
    typedef etl::less<T> compare;

    return etl::min(a, b, compare());
  }

  //***************************************************************************
  // max
  //***************************************************************************
  template <typename T, typename TCompare>
  ETL_NODISCARD
  ETL_CONSTEXPR 
  const T& max(const T& a, const T& b, TCompare compare)
  {
    return (compare(a, b)) ? b : a;
  }

  template <typename T>
  ETL_NODISCARD
  ETL_CONSTEXPR 
  const T& max(const T& a, const T& b)
  {
    typedef etl::less<T> compare;

    return etl::max(a, b, compare());
  }

  //***************************************************************************
  // for_each
  //***************************************************************************
  template <typename TIterator, typename TUnaryOperation>
  ETL_CONSTEXPR14 
  TUnaryOperation for_each(TIterator first, TIterator last, TUnaryOperation unary_operation)
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
  ETL_CONSTEXPR14
  TIteratorOut transform(TIteratorIn first1, TIteratorIn last1, TIteratorOut d_first, TUnaryOperation unary_operation)
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
  ETL_CONSTEXPR14
  TIteratorOut transform(TIteratorIn1 first1, TIteratorIn1 last1, TIteratorIn2 first2, TIteratorOut d_first, TBinaryOperation binary_operation)
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
    void push_heap(TIterator first, TDistance value_index, TDistance top_index, TValue value, TCompare compare)
    {
      TDistance parent = (value_index - 1) / 2;

      while ((value_index > top_index) && compare(first[parent], value))
      {
        first[value_index] = etl::move(first[parent]);
        value_index = parent;
        parent = (value_index - 1) / 2;
      }

      first[value_index] = etl::move(value);
    }

    // Adjust Heap Helper
    template <typename TIterator, typename TDistance, typename TValue, typename TCompare>
    void adjust_heap(TIterator first, TDistance value_index, TDistance length, TValue value, TCompare compare)
    {
      TDistance top_index = value_index;
      TDistance child2nd = (2 * value_index) + 2;

      while (child2nd < length)
      {
        if (compare(first[child2nd], first[child2nd - 1]))
        {
          --child2nd;
        }

        first[value_index] = etl::move(first[child2nd]);
        value_index = child2nd;
        child2nd = 2 * (child2nd + 1);
      }

      if (child2nd == length)
      {
        first[value_index] = etl::move(first[child2nd - 1]);
        value_index = child2nd - 1;
      }

      push_heap(first, value_index, top_index, etl::move(value), compare);
    }

    // Is Heap Helper
    template <typename TIterator, typename TDistance, typename TCompare>
    bool is_heap(const TIterator first, const TDistance n, TCompare compare)
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
  }

  // Pop Heap
  template <typename TIterator, typename TCompare>
  void pop_heap(TIterator first, TIterator last, TCompare compare)
  {
    typedef typename etl::iterator_traits<TIterator>::value_type value_t;
    typedef typename etl::iterator_traits<TIterator>::difference_type distance_t;

    value_t value = etl::move(last[-1]);
    last[-1] = etl::move(first[0]);

    private_heap::adjust_heap(first, distance_t(0), distance_t(last - first - 1), etl::move(value), compare);
  }

  // Pop Heap
  template <typename TIterator>
  void pop_heap(TIterator first, TIterator last)
  {
    typedef etl::less<typename etl::iterator_traits<TIterator>::value_type> compare;

    etl::pop_heap(first, last, compare());
  }

  // Push Heap
  template <typename TIterator, typename TCompare>
  void push_heap(TIterator first, TIterator last, TCompare compare)
  {
    typedef typename etl::iterator_traits<TIterator>::difference_type difference_t;
    typedef typename etl::iterator_traits<TIterator>::value_type      value_t;

    private_heap::push_heap(first, difference_t(last - first - 1), difference_t(0), value_t(etl::move(*(last - 1))), compare);
  }

  // Push Heap
  template <typename TIterator>
  void push_heap(TIterator first, TIterator last)
  {
    typedef etl::less<typename etl::iterator_traits<TIterator>::value_type> compare;

    etl::push_heap(first, last, compare());
  }

  // Make Heap
  template <typename TIterator, typename TCompare>
  void make_heap(TIterator first, TIterator last, TCompare compare)
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
      private_heap::adjust_heap(first, parent, length, etl::move(*(first + parent)), compare);

      if (parent == 0)
      {
        return;
      }

      --parent;
    }
  }

  // Make Heap
  template <typename TIterator>
  void make_heap(TIterator first, TIterator last)
  {
    typedef etl::less<typename etl::iterator_traits<TIterator>::value_type> compare;

    etl::make_heap(first, last, compare());
  }

  // Is Heap
  template <typename TIterator>
  ETL_NODISCARD
    bool is_heap(TIterator first, TIterator last)
  {
    typedef etl::less<typename etl::iterator_traits<TIterator>::value_type> compare;

    return private_heap::is_heap(first, last - first, compare());
  }

  // Is Heap
  template <typename TIterator, typename TCompare>
  ETL_NODISCARD
  bool is_heap(TIterator first, TIterator last, TCompare compare)
  {
    return private_heap::is_heap(first, last - first, compare);
  }

  // Sort Heap
  template <typename TIterator>
  void sort_heap(TIterator first, TIterator last)
  {
    while (first != last)
    {
      etl::pop_heap(first, last);
      --last;
    }
  }

  // Sort Heap
  template <typename TIterator, typename TCompare>
  void sort_heap(TIterator first, TIterator last, TCompare compare)
  {
    while (first != last)
    {
      etl::pop_heap(first, last, compare);
      --last;
    }
  }

  //***************************************************************************
  // Search
  //***************************************************************************
  template<typename TIterator1, typename TIterator2, typename TCompare>
  ETL_NODISCARD
  ETL_CONSTEXPR14
  TIterator1 search(TIterator1 first, TIterator1 last, TIterator2 search_first, TIterator2 search_last, TCompare compare)
  {
    while (true)
    {
      TIterator1 itr = first;
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
  template<typename TIterator1, typename TIterator2>
  ETL_NODISCARD
  ETL_CONSTEXPR14
  TIterator1 search(TIterator1 first, TIterator1 last, TIterator2 search_first, TIterator2 search_last)
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
    template <typename TIterator>
    ETL_CONSTEXPR14
    TIterator rotate_general(TIterator first, TIterator middle, TIterator last)
    {
      TIterator next = middle;

      while (first != next)
      {
        using ETL_OR_STD::swap; // Allow ADL

        swap(*first, *next);

        ++first;
        ++next;

        if (next == last)
        {
          next = middle;
        }
        else if (first == middle)
        {
          middle = next;
        }
      }

      return first;
    }

    //*********************************
    template <typename TIterator>
    ETL_CONSTEXPR14
    TIterator rotate_left_by_one(TIterator first, TIterator last)
    {
      typedef typename etl::iterator_traits<TIterator>::value_type value_type;

      // Save the first item.
      value_type temp(etl::move(*first));

      // Move the rest.
      TIterator result = etl::move(etl::next(first), last, first);

      // Restore the first item in its rotated position.
      *result = etl::move(temp);

      // The new position of the first item.
      return result;
    }

    //*********************************
    template <typename TIterator>
    ETL_CONSTEXPR14
    TIterator rotate_right_by_one(TIterator first, TIterator last)
    {
      typedef typename etl::iterator_traits<TIterator>::value_type value_type;

      // Save the last item.
      TIterator previous = etl::prev(last);
      value_type temp(etl::move(*previous));

      // Move the rest.
      TIterator result = etl::move_backward(first, previous, last);

      // Restore the last item in its rotated position.
      *first = etl::move(temp);

      // The new position of the first item.
      return result;
    }
  }

  //*********************************
  template<typename TIterator>
  ETL_CONSTEXPR14
  TIterator rotate(TIterator first, TIterator middle, TIterator last)
  {
    if (etl::next(first) == middle)
    {
      return private_algorithm::rotate_left_by_one(first, last);
    }

    if (etl::next(middle) == last)
    {
      return private_algorithm::rotate_right_by_one(first, last);
    }

    return private_algorithm::rotate_general(first, middle, last);
  }

  //***************************************************************************
  // find_end
  //***************************************************************************
  // Predicate
  template <typename TIterator1, typename TIterator2, typename TPredicate>
  ETL_NODISCARD
  ETL_CONSTEXPR14
  TIterator1 find_end(TIterator1 b, TIterator1 e,
                      TIterator2 sb, TIterator2 se,
                      TPredicate predicate)
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
        b = result;
        ++b;
      }
    }
    return result;
  }

  // Default
  template <typename TIterator1, typename TIterator2>
  ETL_NODISCARD
  ETL_CONSTEXPR14
  TIterator1 find_end(TIterator1 b, TIterator1 e,
                      TIterator2 sb, TIterator2 se)
  {
    typedef etl::equal_to<typename etl::iterator_traits<TIterator1>::value_type> predicate;

    return find_end(b, e, sb, se, predicate());
  }

  //***************************************************************************
  /// Finds the iterator to the smallest element in the range (begin, end).<br>
  ///<a href="http://en.cppreference.com/w/cpp/algorithm/min_element"></a>
  ///\ingroup algorithm
  //***************************************************************************
  template <typename TIterator, typename TCompare>
  ETL_NODISCARD
  ETL_CONSTEXPR14
  TIterator min_element(TIterator begin,
                        TIterator end,
                        TCompare  compare)
  {
    TIterator minimum = begin;
    ++begin;

    while (begin != end)
    {
      if (compare(*begin, *minimum))
      {
        minimum = begin;
      }

      ++begin;
    }

    return minimum;
  }

  //***************************************************************************
  /// min_element
  ///\ingroup algorithm
  ///<a href="http://en.cppreference.com/w/cpp/algorithm/min_element"></a>
  //***************************************************************************
  template <typename TIterator>
  ETL_NODISCARD
  ETL_CONSTEXPR14
  TIterator min_element(TIterator begin,
                        TIterator end)
  {
    typedef typename etl::iterator_traits<TIterator>::value_type value_t;

    return etl::min_element(begin, end, etl::less<value_t>());
  }

  //***************************************************************************
  /// Finds the iterator to the largest element in the range (begin, end).<br>
  ///<a href="http://en.cppreference.com/w/cpp/algorithm/max_element"></a>
  ///\ingroup algorithm
  //***************************************************************************
  template <typename TIterator, typename TCompare>
  ETL_NODISCARD
  ETL_CONSTEXPR14
  TIterator max_element(TIterator begin,
                        TIterator end,
                        TCompare  compare)
  {
    TIterator maximum = begin;
    ++begin;

    while (begin != end)
    {
      if (!compare(*begin, *maximum))
      {
        maximum = begin;
      }

      ++begin;
    }

    return maximum;
  }

  //***************************************************************************
  /// max_element
  ///\ingroup algorithm
  ///<a href="http://en.cppreference.com/w/cpp/algorithm/max_element"></a>
  //***************************************************************************
  template <typename TIterator>
  ETL_NODISCARD
  ETL_CONSTEXPR14
  TIterator max_element(TIterator begin,
                        TIterator end)
  {
    typedef typename etl::iterator_traits<TIterator>::value_type value_t;

    return etl::max_element(begin, end, etl::less<value_t>());
  }

  //***************************************************************************
  /// Finds the greatest and the smallest element in the range (begin, end).<br>
  ///<a href="http://en.cppreference.com/w/cpp/algorithm/minmax_element"></a>
  ///\ingroup algorithm
  //***************************************************************************
  template <typename TIterator, typename TCompare>
  ETL_NODISCARD
  ETL_CONSTEXPR14
  ETL_OR_STD::pair<TIterator, TIterator> minmax_element(TIterator begin,
                                                        TIterator end,
                                                        TCompare  compare)
  {
    TIterator minimum = begin;
    TIterator maximum = begin;
    ++begin;

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

    return ETL_OR_STD::pair<TIterator, TIterator>(minimum, maximum);
  }

  //***************************************************************************
  /// minmax_element
  ///\ingroup algorithm
  ///<a href="http://en.cppreference.com/w/cpp/algorithm/minmax_element"></a>
  //***************************************************************************
  template <typename TIterator>
  ETL_NODISCARD
  ETL_CONSTEXPR14
  ETL_OR_STD::pair<TIterator, TIterator> minmax_element(TIterator begin,
                                                        TIterator end)
  {
    typedef typename etl::iterator_traits<TIterator>::value_type value_t;

    return etl::minmax_element(begin, end, etl::less<value_t>());
  }

  //***************************************************************************
  /// minmax
  ///\ingroup algorithm
  ///<a href="http://en.cppreference.com/w/cpp/algorithm/minmax"></a>
  //***************************************************************************
  template <typename T>
  ETL_NODISCARD
  ETL_CONSTEXPR14
  ETL_OR_STD::pair<const T&, const T&> minmax(const T& a,
                                              const T& b)
  {
    return (b < a) ? ETL_OR_STD::pair<const T&, const T&>(b, a) : ETL_OR_STD::pair<const T&, const T&>(a, b);
  }

  //***************************************************************************
  /// minmax
  ///\ingroup algorithm
  ///<a href="http://en.cppreference.com/w/cpp/algorithm/minmax"></a>
  //***************************************************************************
  template <typename T, typename TCompare>
  ETL_NODISCARD
  ETL_CONSTEXPR14
  ETL_OR_STD::pair<const T&, const T&> minmax(const T& a,
                                              const T& b,
                                              TCompare compare)
  {
    return compare(b, a) ? ETL_OR_STD::pair<const T&, const T&>(b, a) : ETL_OR_STD::pair<const T&, const T&>(a, b);
  }

  //***************************************************************************
  /// is_sorted_until
  ///\ingroup algorithm
  ///<a href="http://en.cppreference.com/w/cpp/algorithm/is_sorted_until"></a>
  //***************************************************************************
  template <typename TIterator>
  ETL_NODISCARD
  ETL_CONSTEXPR14
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
  template <typename TIterator, typename TCompare>
  ETL_NODISCARD
  ETL_CONSTEXPR14
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
  ETL_NODISCARD
  ETL_CONSTEXPR14
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
  template<typename TIterator, typename TCompare>
  ETL_NODISCARD
  ETL_CONSTEXPR14
  bool is_sorted(TIterator begin,
                 TIterator end,
                 TCompare  compare)
  {
    return etl::is_sorted_until(begin, end, compare) == end;
  }

  //***************************************************************************
  /// find_if_not
  ///\ingroup algorithm
  ///<a href="http://en.cppreference.com/w/cpp/algorithm/find"></a>
  //***************************************************************************
  template <typename TIterator, typename TUnaryPredicate>
  ETL_NODISCARD
  ETL_CONSTEXPR14
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
  /// is_permutation
  ///\ingroup algorithm
  ///<a href="http://en.cppreference.com/w/cpp/algorithm/is_permutation"></a>
  //***************************************************************************
  template <typename TIterator1, typename TIterator2>
  ETL_NODISCARD
  ETL_CONSTEXPR14
  bool is_permutation(TIterator1 begin1,
                      TIterator1 end1,
                      TIterator2 begin2)
  {
    if (begin1 != end1)
    {
      TIterator2 end2 = begin2;

      etl::advance(end2, etl::distance(begin1, end1));

      for (TIterator1 i = begin1; i != end1; ++i)
      {
        if (i == etl::find(begin1, i, *i))
        {
          size_t n = etl::count(begin2, end2, *i);

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
  ///<a href="http://en.cppreference.com/w/cpp/algorithm/is_permutation"></a>
  //***************************************************************************
  template <typename TIterator1, typename TIterator2, typename TBinaryPredicate>
  ETL_NODISCARD
  ETL_CONSTEXPR14
  bool is_permutation(TIterator1       begin1,
                      TIterator1       end1,
                      TIterator2       begin2,
                      TBinaryPredicate predicate)
  {
    if (begin1 != end1)
    {
      TIterator2 end2 = begin2;

      etl::advance(end2, etl::distance(begin1, end1));

      for (TIterator1 i = begin1; i != end1; ++i)
      {
        if (i == etl::find_if(begin1, i, etl::bind1st(predicate, *i)))
        {
          size_t n = etl::count(begin2, end2, *i);

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
  ///<a href="http://en.cppreference.com/w/cpp/algorithm/is_permutation"></a>
  //***************************************************************************
  template <typename TIterator1, typename TIterator2>
  ETL_NODISCARD
  ETL_CONSTEXPR14
  bool is_permutation(TIterator1 begin1,
                      TIterator1 end1,
                      TIterator2 begin2,
                      TIterator2 end2)
  {
    if (begin1 != end1)
    {
      for (TIterator1 i = begin1; i != end1; ++i)
      {
        if (i == etl::find(begin1, i, *i))
        {
          size_t n = etl::count(begin2, end2, *i);

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
  ///<a href="http://en.cppreference.com/w/cpp/algorithm/is_permutation"></a>
  //***************************************************************************
  template <typename TIterator1, typename TIterator2, typename TBinaryPredicate>
  ETL_NODISCARD
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
        if (i == etl::find_if(begin1, i, etl::bind1st(predicate, *i)))
        {
          size_t n = etl::count(begin2, end2, *i);

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
  /// is_partitioned
  ///\ingroup algorithm
  ///<a href="http://en.cppreference.com/w/cpp/algorithm/is_partitioned"></a>
  //***************************************************************************
  template <typename TIterator, typename TUnaryPredicate>
  ETL_NODISCARD
  ETL_CONSTEXPR14
  bool is_partitioned(TIterator       begin,
                      TIterator       end,
                      TUnaryPredicate predicate)
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
  ///<a href="http://en.cppreference.com/w/cpp/algorithm/partition_point"></a>
  ///\ingroup algorithm
  //***************************************************************************
  template <typename TIterator, typename TUnaryPredicate>
  ETL_NODISCARD
  ETL_CONSTEXPR14
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
  template <typename TSource, typename TDestinationTrue, typename TDestinationFalse, typename TUnaryPredicate>
  ETL_CONSTEXPR14
  ETL_OR_STD::pair<TDestinationTrue, TDestinationFalse> partition_copy(TSource           begin,
                                                                       TSource           end,
                                                                       TDestinationTrue  destination_true,
                                                                       TDestinationFalse destination_false,
                                                                       TUnaryPredicate   predicate)
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
  ///<a href="http://en.cppreference.com/w/cpp/algorithm/copy"></a>
  //***************************************************************************
  template <typename TIterator, typename TOutputIterator, typename TUnaryPredicate>
  ETL_CONSTEXPR14
  TOutputIterator copy_if(TIterator       begin,
                          TIterator       end,
                          TOutputIterator out,
                          TUnaryPredicate predicate)
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
  ///<a href="http://en.cppreference.com/w/cpp/algorithm/all_any_none_of"></a>
  //***************************************************************************
  template <typename TIterator, typename TUnaryPredicate>
  ETL_NODISCARD
  ETL_CONSTEXPR14
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
  template <typename TIterator, typename TUnaryPredicate>
  ETL_NODISCARD
  ETL_CONSTEXPR14
  bool any_of(TIterator       begin,
              TIterator       end,
              TUnaryPredicate predicate)
  {
    return etl::find_if(begin, end, predicate) != end;
  }

  //***************************************************************************
  /// none_of
  ///\ingroup algorithm
  ///<a href="http://en.cppreference.com/w/cpp/algorithm/all_any_none_of"></a>
  //***************************************************************************
  template <typename TIterator, typename TUnaryPredicate>
  ETL_NODISCARD
  ETL_CONSTEXPR14
  bool none_of(TIterator       begin,
               TIterator       end,
               TUnaryPredicate predicate)
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
  ETL_CONSTEXPR14 
  T accumulate(TIterator first, TIterator last, T sum)
  {
    while (first != last)
    {
      sum = etl::move(sum) + *first;
      ++first;
    }
      
    return sum;
  }

  //***************************************************************************
  /// Accumulates values.
  ///\ingroup algorithm
  //***************************************************************************
  template <typename TIterator, typename T, typename TBinaryOperation>
  ETL_CONSTEXPR14 
  T accumulate(TIterator first, TIterator last, T sum, TBinaryOperation operation)
  {
    while (first != last)
    {
      sum = operation(etl::move(sum), *first);
      ++first;
    }

    return sum;
  }

  //***************************************************************************
  /// Clamp values.
  ///\ingroup algorithm
  //***************************************************************************
  template<typename T, typename TCompare>
  ETL_CONSTEXPR 
  const T& clamp(const T& value, const T& low, const T& high, TCompare compare)
  {
    return compare(value, low) ? low : compare(high, value) ? high : value;
  }
  
  template <typename T>
  ETL_CONSTEXPR 
  const T& clamp(const T& value, const T& low, const T& high )
  {
    return clamp(value, low, high, etl::less<T>());
  }

  //***************************************************************************
  /// Remove
  ///\ingroup algorithm
  //***************************************************************************
  template <typename TIterator, typename T>
  ETL_CONSTEXPR14
  TIterator remove(TIterator first, TIterator last, const T& value)
  {
    first = etl::find(first, last, value);
      
    if (first != last)
    {
      TIterator itr = first;

      while (itr != last)
      {
        if (!(*itr == value))
        {
          *first = etl::move(*itr);
          ++first;
        }

        ++itr;
      }
    }
      
    return first;
  }

  //***************************************************************************
  /// Remove If
  ///\ingroup algorithm
  //***************************************************************************
  template <typename TIterator, typename TUnaryPredicate>
  ETL_CONSTEXPR14
  TIterator remove_if(TIterator first, TIterator last, TUnaryPredicate predicate)
  {
    first = etl::find_if(first, last, predicate);

    if (first != last)
    {
      TIterator itr = first;

      while (itr != last)
      {
        if (!predicate(*itr))
        {
          *first = etl::move(*itr);
          ++first;
        }

        ++itr;
      }
    }

    return first;
  }
}

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
  template <typename TInputIterator,
            typename TOutputIterator>
  ETL_CONSTEXPR14
  typename etl::enable_if<etl::is_random_iterator<TInputIterator>::value &&
                          etl::is_random_iterator<TOutputIterator>::value, TOutputIterator>::type
   copy_s(TInputIterator  i_begin,
          TInputIterator  i_end,
          TOutputIterator o_begin,
          TOutputIterator o_end)
  {
      size_t s_size = etl::distance(i_begin, i_end);
      size_t d_size = etl::distance(o_begin, o_end);
      size_t size   = (s_size < d_size) ? s_size : d_size;

      return etl::copy(i_begin, i_begin + size, o_begin);
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
  template <typename TInputIterator,
            typename TOutputIterator>
  ETL_CONSTEXPR14
  typename etl::enable_if<!etl::is_random_iterator<TInputIterator>::value ||
                          !etl::is_random_iterator<TOutputIterator>::value, TOutputIterator>::type
   copy_s(TInputIterator  i_begin,
          TInputIterator  i_end,
          TOutputIterator o_begin,
          TOutputIterator o_end)
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
  template <typename TInputIterator,
            typename TSize,
            typename TOutputIterator>
  ETL_CONSTEXPR14
  TOutputIterator copy_n_s(TInputIterator  i_begin,
                           TSize           n,
                           TOutputIterator o_begin,
                           TOutputIterator o_end)
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
  template <typename TInputIterator,
            typename TSize1,
            typename TOutputIterator,
            typename TSize2>
  ETL_CONSTEXPR14
  TOutputIterator copy_n_s(TInputIterator  i_begin,
                           TSize1          n1,
                           TOutputIterator o_begin,
                           TSize2          n2)
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
  /// A safer form of copy_if where it terminates when the first end iterator is reached.
  /// There is currently no STL equivelent.
  ///\ingroup algorithm
  //***************************************************************************
  template <typename TInputIterator,
            typename TOutputIterator,
            typename TUnaryPredicate>
  ETL_CONSTEXPR14
  TOutputIterator copy_if_s(TInputIterator  i_begin,
                            TInputIterator  i_end,
                            TOutputIterator o_begin,
                            TOutputIterator o_end,
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
  template <typename TInputIterator,
            typename TSize,
            typename TOutputIterator,
            typename TUnaryPredicate>
  ETL_CONSTEXPR14
  TOutputIterator copy_n_if(TInputIterator  i_begin,
                            TSize           n,
                            TOutputIterator o_begin,
                            TUnaryPredicate predicate)
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
  typename etl::enable_if<etl::is_random_iterator<TInputIterator>::value &&
                          etl::is_random_iterator<TOutputIterator>::value, TOutputIterator>::type
  move_s(TInputIterator  i_begin,
         TInputIterator  i_end,
         TOutputIterator o_begin,
         TOutputIterator o_end)
  {
    size_t s_size = etl::distance(i_begin, i_end);
    size_t d_size = etl::distance(o_begin, o_end);
    size_t size = (s_size < d_size) ? s_size : d_size;

    return etl::move(i_begin, i_begin + size, o_begin);
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
  ETL_CONSTEXPR14
  typename etl::enable_if<!etl::is_random_iterator<TInputIterator>::value ||
                          !etl::is_random_iterator<TOutputIterator>::value, TOutputIterator>::type
  move_s(TInputIterator  i_begin,
         TInputIterator  i_end,
         TOutputIterator o_begin,
         TOutputIterator o_end)
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
  TOutputIterator move_s(TInputIterator  i_begin,
                         TInputIterator  i_end,
                         TOutputIterator o_begin,
                         TOutputIterator o_end)
  {
    // Move not supported. Defer to copy.
    return etl::copy_s(i_begin, i_end, o_begin, o_end);
  }
#endif

  //***************************************************************************
  /// binary_find
  ///\ingroup algorithm
  /// Does a binary search and returns an iterator to the value or end if not found.
  //***************************************************************************
  template <typename TIterator, typename TValue>
  ETL_NODISCARD
  ETL_CONSTEXPR14
  TIterator binary_find(TIterator     begin,
                        TIterator     end,
                        const TValue& value)
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
  /// Does a binary search and returns an iterator to the value or end if not found.
  //***************************************************************************
  template <typename TIterator,
            typename TValue,
            typename TBinaryPredicate,
            typename TBinaryEquality>
  ETL_NODISCARD
  ETL_CONSTEXPR14
  TIterator binary_find(TIterator        begin,
                        TIterator        end,
                        const TValue&    value,
                        TBinaryPredicate predicate,
                        TBinaryEquality  equality)
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
  template <typename TIterator,
            typename TUnaryFunction,
            typename TUnaryPredicate>
  ETL_CONSTEXPR14
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
  /// Like std::for_each but for 'n' iterations.
  ///\ingroup algorithm
  //***************************************************************************
  template <typename TIterator,
            typename TSize,
            typename TUnaryFunction>
  ETL_CONSTEXPR14
  TIterator for_each_n(TIterator       begin,
                       TSize           n,
                       TUnaryFunction  function)
  {
    while (n-- > 0)
    {
      function(*begin);
      ++begin;
    }

    return begin;
  }

  //***************************************************************************
  /// Like std::for_each but applies a predicate before calling the function, for 'n' iterations
  ///\ingroup algorithm
  //***************************************************************************
  template <typename TIterator,
            typename TSize,
            typename TUnaryFunction,
            typename TUnaryPredicate>
  ETL_CONSTEXPR14
  TIterator for_each_n_if(TIterator       begin,
                          TSize           n,
                          TUnaryFunction  function,
                          TUnaryPredicate predicate)
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
  ETL_CONSTEXPR14
  TOutputIterator transform_s(TInputIterator  i_begin,
                              TInputIterator  i_end,
                              TOutputIterator o_begin,
                              TOutputIterator o_end,
                              TUnaryFunction  function)
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
  template <typename TInputIterator,
            typename TSize,
            typename TOutputIterator,
            typename TUnaryFunction>
  ETL_CONSTEXPR14
  void transform_n(TInputIterator  i_begin,
                   TSize           n,
                   TOutputIterator o_begin,
                   TUnaryFunction  function)
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
  template <typename TInputIterator1,
            typename TInputIterator2,
            typename TSize,
            typename TOutputIterator,
            typename TBinaryFunction>
  ETL_CONSTEXPR14
  void transform_n(TInputIterator1 i_begin1,
                   TInputIterator2 i_begin2,
                   TSize           n,
                   TOutputIterator o_begin,
                   TBinaryFunction function)
  {
    TInputIterator1 i_end1(i_begin1);
    etl::advance(i_end1, n);

    etl::transform(i_begin1, i_end1, i_begin2, o_begin, function);
  }

  //***************************************************************************
  /// Like std::transform but applies a predicate before calling the function.
  ///\ingroup algorithm
  //***************************************************************************
  template <typename TInputIterator,
            typename TOutputIterator,
            typename TUnaryFunction,
            typename TUnaryPredicate>
  ETL_CONSTEXPR14
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
  template <typename TInputIterator1,
            typename TInputIterator2,
            typename TOutputIterator,
            typename TBinaryFunction,
            typename TBinaryPredicate>
  ETL_CONSTEXPR14
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
  template <typename TInputIterator,
            typename TSize,
            typename TOutputIterator,
            typename TUnaryFunction,
            typename TUnaryPredicate>
  ETL_CONSTEXPR14
  TOutputIterator transform_n_if(TInputIterator  i_begin,
                                 TSize           n,
                                 TOutputIterator o_begin,
                                 TUnaryFunction  function,
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
  template <typename TInputIterator1,
            typename TInputIterator2,
            typename TSize,
            typename TOutputIterator,
            typename TBinaryFunction,
            typename TBinaryPredicate>
  ETL_CONSTEXPR14
  TOutputIterator transform_n_if(TInputIterator1  i_begin1,
                                 TInputIterator2  i_begin2,
                                 TSize            n,
                                 TOutputIterator  o_begin,
                                 TBinaryFunction  function,
                                 TBinaryPredicate predicate)
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
  /// Transforms the elements from the range (begin, end) to two different ranges
  /// depending on the value returned by the predicate.<br>
  ///\ingroup algorithm
  //***************************************************************************
  template <typename TSource, typename TDestinationTrue, typename TDestinationFalse,
            typename TUnaryFunctionTrue, typename TUnaryFunctionFalse,
            typename TUnaryPredicate>
  ETL_CONSTEXPR14
  ETL_OR_STD::pair<TDestinationTrue, TDestinationFalse> partition_transform(TSource             begin,
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
  template <typename TSource1,
            typename TSource2,
            typename TDestinationTrue,
            typename TDestinationFalse,
            typename TBinaryFunctionTrue,
            typename TBinaryFunctionFalse,
            typename TBinaryPredicate>
  ETL_CONSTEXPR14
  ETL_OR_STD::pair<TDestinationTrue, TDestinationFalse> partition_transform(TSource1             begin1,
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
  void shell_sort(TIterator first, TIterator last, TCompare compare)
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
  void shell_sort(TIterator first, TIterator last)
  {
    etl::shell_sort(first, last, etl::less<typename etl::iterator_traits<TIterator>::value_type>());
  }

  //***************************************************************************
  /// Sorts the elements using insertion sort.
  /// Uses user defined comparison.
  ///\ingroup algorithm
  //***************************************************************************
  template <typename TIterator, typename TCompare>
  ETL_CONSTEXPR14
  void insertion_sort(TIterator first, TIterator last, TCompare compare)
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
  ETL_CONSTEXPR14
  void insertion_sort(TIterator first, TIterator last)
  {
    etl::insertion_sort(first, last, etl::less<typename etl::iterator_traits<TIterator>::value_type>());
  }

  //***************************************************************************
  namespace private_algorithm
  {
    template <typename TIterator>
    ETL_CONSTEXPR14
    typename etl::enable_if<etl::is_forward_iterator<TIterator>::value, TIterator>::type
      get_before_last(TIterator first_, TIterator last_)
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
    ETL_CONSTEXPR14
    typename etl::enable_if<etl::is_bidirectional_iterator<TIterator>::value, TIterator>::type
      get_before_last(TIterator /*first_*/, TIterator last_)
    {
      TIterator last = last_;
      --last;

      return last;
    }

    template <typename TIterator>
    ETL_CONSTEXPR14
    typename etl::enable_if<etl::is_random_access_iterator<TIterator>::value, TIterator>::type
      get_before_last(TIterator /*first_*/, TIterator last_)
    {
      return last_ - 1;
    }
  }

  //***************************************************************************
  /// Sorts the elements using selection sort.
  /// Uses user defined comparison.
  ///\ingroup algorithm
  //***************************************************************************
  template <typename TIterator, typename TCompare>
  ETL_CONSTEXPR20
  void selection_sort(TIterator first, TIterator last, TCompare compare)
  {
    TIterator min;
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
      swap(*i, *min);
    }
  }

  //***************************************************************************
  /// Sorts the elements using selection sort.
  ///\ingroup algorithm
  //***************************************************************************
  template <typename TIterator>
  ETL_CONSTEXPR20
  void selection_sort(TIterator first, TIterator last)
  {
    selection_sort(first, last, etl::less<typename etl::iterator_traits<TIterator>::value_type>());
  }

  //***************************************************************************
  /// Sorts the elements using heap sort.
  /// Uses user defined comparison.
  ///\ingroup algorithm
  //***************************************************************************
  template <typename TIterator, typename TCompare>
  ETL_CONSTEXPR14
  void heap_sort(TIterator first, TIterator last, TCompare compare)
  {
    if (!etl::is_heap(first, last, compare))
    {
      etl::make_heap(first, last, compare);
    }

    etl::sort_heap(first, last, compare);
  }

  //***************************************************************************
  /// Sorts the elements using heap sort.
  ///\ingroup algorithm
  //***************************************************************************
  template <typename TIterator>
  ETL_CONSTEXPR14
  void heap_sort(TIterator first, TIterator last)
  {
    if (!etl::is_heap(first, last))
    {
      etl::make_heap(first, last);
    }

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
  /// Returns the maximum value.
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
}

#include "private/minmax_pop.h"

#endif
