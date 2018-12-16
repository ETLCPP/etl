///\file

/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl
https://www.etlcpp.com

Copyright(c) 2018 jwellbelove

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

#ifndef ETL_STL_ALTERNATE_ALGORITHM_INCLUDED
#define ETL_STL_ALTERNATE_ALGORITHM_INCLUDED

#include "../../platform.h"

#include <string.h>

#include "../../type_traits.h"

#include "iterator.h"
#include "functional.h"
#include "utility.h"

#if defined(ETL_IN_UNIT_TEST)
  #if !defined(ETLSTD)
#define ETLSTD etlstd
  #endif
namespace etlstd
#else
  #if !defined(ETLSTD)
#define ETLSTD std
  #endif
namespace std
#endif
{
  //***************************************************************************
  // advance
  template <typename TIterator, typename TDistance>
  typename etl::enable_if<!etl::is_same<typename ETLSTD::iterator_traits<TIterator>::iterator_tag, ETLSTD::random_access_iterator_tag>::value, void>::type
    advance(TIterator itr, TDistance distance)
  {
    while (distance-- != 0)
    {
      ++itr;
    }
  }

  template <typename TIterator, typename TDistance>
  typename etl::enable_if<etl::is_same<typename ETLSTD::iterator_traits<TIterator>::iterator_tag, ETLSTD::random_access_iterator_tag>::value, void>::type
    advance(TIterator itr, TDistance distance)
  {
    return itr += distance;
  }

  //***************************************************************************
  // copy
  // Pointer
  template <typename TIterator1, typename TIterator2>
  typename etl::enable_if<etl::is_pointer<TIterator1>::value &&
                          etl::is_pointer<TIterator2>::value &&
                          etl::is_pod<typename ETLSTD::iterator_traits<TIterator1>::value_type>::value, TIterator2>::type
    copy(TIterator1 sb, TIterator1 se, TIterator2 db)
  {
    typedef typename ETLSTD::iterator_traits<TIterator1>::value_type value_t;

    return TIterator2(memcpy(db, sb, sizeof(value_t) * (se - sb)));
  }

  // Other iterator
  template <typename TIterator1, typename TIterator2>
  typename etl::enable_if<!etl::is_pointer<TIterator1>::value ||
                          !etl::is_pointer<TIterator2>::value ||
                          !etl::is_pod<typename ETLSTD::iterator_traits<TIterator1>::value_type>::value, TIterator2>::type
    copy(TIterator1 sb, TIterator1 se, TIterator2 db)
  {
    while (sb != se)
    {
      *db++ = *sb++;
    }

    return db;
  }

  //***************************************************************************
  // copy_n
  // Pointer
  template <typename TIterator1, typename TSize, typename TIterator2>
  typename etl::enable_if<etl::is_pointer<TIterator1>::value &&
                          etl::is_pointer<TIterator2>::value &&
                          etl::is_pod<typename ETLSTD::iterator_traits<TIterator1>::value_type>::value, TIterator2>::type
    copy_n(TIterator1 sb, TSize count, TIterator2 db)
  {
    typedef typename ETLSTD::iterator_traits<TIterator1>::value_type value_t;

    return TIterator2(memcpy(db, sb, sizeof(value_t) * count));
  }

  // Other iterator
  template <typename TIterator1, typename TSize, typename TIterator2>
  typename etl::enable_if<!etl::is_pointer<TIterator1>::value ||
                          !etl::is_pointer<TIterator2>::value ||
                          !etl::is_pod<typename ETLSTD::iterator_traits<TIterator1>::value_type>::value, TIterator2>::type
    copy_n(TIterator1 sb, TSize count, TIterator2 db)
  {
    while (count != 0)
    {
      *db++ = *sb++;
      --count;
    }

    return db;
  }

  //***************************************************************************
  // copy_backward
  // Pointer
  template <typename TIterator1, typename TIterator2>
  typename etl::enable_if<etl::is_pointer<TIterator1>::value &&
                          etl::is_pointer<TIterator2>::value &&
                          etl::is_pod<typename ETLSTD::iterator_traits<TIterator1>::value_type>::value, TIterator2>::type
    copy_backward(TIterator1 sb, TIterator1 se, TIterator2 de)
  {
    typedef typename ETLSTD::iterator_traits<TIterator1>::value_type value_t;

    const size_t length = (se - sb);

    return TIterator2(memmove(de - length, sb, sizeof(value_t) * length));
  }

  // Other iterator
  template <typename TIterator1, typename TIterator2>
  typename etl::enable_if<!etl::is_pointer<TIterator1>::value ||
                          !etl::is_pointer<TIterator2>::value ||
                          !etl::is_pod<typename ETLSTD::iterator_traits<TIterator1>::value_type>::value, TIterator2>::type
    copy_backward(TIterator1 sb, TIterator1 se, TIterator2 de)
  {
    while (se != sb)
    {
      *(--de) = *(--se);
    }

    return de;
  }

  //***************************************************************************
  // move
  template <typename TIterator1, typename TIterator2>
  TIterator2 move(TIterator1 sb, TIterator1 se, TIterator2 db)
  {
    while (sb != se)
    {
      *db++ = std::move(*sb++);
    }

    return db;
  }

  //***************************************************************************
  // move_backward
  template <typename TIterator1, typename TIterator2>
  TIterator2 move_backward(TIterator1 sb, TIterator1 se, TIterator2 de)
  {
    while (sb != se)
    {
      *(--de) = std::move(*(--se));
    }

    return de;
  }

  //***************************************************************************
  // lower_bound
  template<typename TIterator, typename TValue, typename TCompare>
  TIterator lower_bound(TIterator first, TIterator last, const TValue& value, TCompare compare)
  {
    typedef typename ETLSTD::iterator_traits<TIterator>::difference_type difference_t;

    difference_t count = ETLSTD::distance(first, last);

    while (count > 0)
    {
      TIterator    itr = first;
      difference_t step = count / 2;

      ETLSTD::advance(itr, step);

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
  TIterator lower_bound(TIterator first, TIterator last, const TValue& value)
  {
    typedef ETLSTD::less<typename ETLSTD::iterator_traits<TIterator>::value_type> compare;

    return ETLSTD::lower_bound(first, last, value, compare());
  }

  //***************************************************************************
  // upper_bound
  template<typename TIterator, typename TValue, typename TCompare>
  TIterator upper_bound(TIterator first, TIterator last, const TValue& value, TCompare compare)
  {
    typedef typename ETLSTD::iterator_traits<TIterator>::difference_type difference_t;

    difference_t count = ETLSTD::distance(first, last);

    while (count > 0)
    {
      TIterator    itr = first;
      difference_t step = count / 2;

      ETLSTD::advance(itr, step);

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
  TIterator upper_bound(TIterator first, TIterator last, const TValue& value)
  {
    typedef ETLSTD::less<typename ETLSTD::iterator_traits<TIterator>::value_type> compare;

    return ETLSTD::upper_bound(first, last, value, compare());
  }

  //***************************************************************************
  // equal_range
  template<typename TIterator, typename TValue, typename TCompare>
  ETLSTD::pair  <TIterator, TIterator> equal_range(TIterator first, TIterator last, const TValue& value, TCompare compare)
  {
    return ETLSTD::make_pair(ETLSTD::lower_bound(first, last, value, compare),
                          ETLSTD::upper_bound(first, last, value, compare));
  }

  template<typename TIterator, typename TValue>
  ETLSTD::pair<TIterator, TIterator> equal_range(TIterator first, TIterator last, const TValue& value)
  {
    typedef ETLSTD::less<typename ETLSTD::iterator_traits<TIterator>::value_type> compare;

    return ETLSTD::make_pair(ETLSTD::lower_bound(first, last, value, compare()),
                          ETLSTD::upper_bound(first, last, value, compare()));
  }

  //***************************************************************************
  // find_if
  template <typename TIterator, typename TUnaryPredicate>
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
  template <typename TIterator, typename T>
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
  template<typename TIterator, typename TValue>
  typename etl::enable_if<!(etl::is_same<char, TValue>::value || etl::is_same<unsigned char, TValue>::value) || !etl::is_pointer<TIterator>::value, void>::type
    fill(TIterator first, TIterator last, const TValue& value)
  {
    while (first != last)
    {
      *first++ = value;
    }
  }

  template<typename TIterator, typename TValue>
  typename etl::enable_if<(etl::is_same<char, TValue>::value || etl::is_same<unsigned char, TValue>::value) && etl::is_pointer<TIterator>::value, void>::type
    fill(TIterator first, TIterator last, const TValue& value)
  {
    memset(first, value, last - first);
  }

  //***************************************************************************
  // fill_n
  template<typename TIterator, typename TSize, typename TValue>
  typename etl::enable_if<!(etl::is_same<char, TValue>::value || etl::is_same<unsigned char, TValue>::value) || !etl::is_pointer<TIterator>::value, TIterator>::type
    fill_n(TIterator first, TSize count, const TValue& value)
  {
    for (TSize i = 0; i < count; ++i)
    {
      *first++ = value;
    }

    return first;
  }

  template<typename TIterator, typename TSize, typename TValue>
  typename etl::enable_if<(etl::is_same<char, TValue>::value || etl::is_same<unsigned char, TValue>::value) && etl::is_pointer<TIterator>::value, void>::type
    fill_n(TIterator first, TSize count, const TValue& value)
  {
    memset(first, value, count);
  }

  //***************************************************************************
  // count
  template <typename TIterator, typename T>
  typename iterator_traits<TIterator>::difference_type count(TIterator first, TIterator last, const T& value)
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
  // count
  template <typename TIterator, typename TUnaryPredicate>
  typename iterator_traits<TIterator>::difference_type count_if(TIterator first, TIterator last, TUnaryPredicate predicate)
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
  // swap
  template <typename T>
  void swap(T& a, T& b)
  {
    T c = a;
    a = b;
    b = c;
  }

  //***************************************************************************
  // iter_swap
  template <typename TIterator1, typename TIterator2>
  void iter_swap(TIterator1 a, TIterator2 b)
  {
    typename ETLSTD::iterator_traits<TIterator1>::value_type c = *a;
    *a = *b;
    *b = c;
  }

  //***************************************************************************
  // equal
  template <typename TIterator1, typename TIterator2>
  typename etl::enable_if<!etl::is_pointer<TIterator1>::value || !etl::is_pointer<TIterator2>::value || !etl::is_pod<typename ETLSTD::iterator_traits<TIterator1>::value_type>::value, bool>::type
    equal(TIterator1 first1, TIterator1 last1, TIterator2 first2)
  {
    while (first1 != last1)
    {
      if (*first1++ != *first2++)
      {
        return false;
      }
    }

    return true;
  }

  template <typename TIterator1, typename TIterator2>
  typename etl::enable_if<etl::is_pointer<TIterator1>::value && etl::is_pointer<TIterator2>::value && etl::is_pod<typename ETLSTD::iterator_traits<TIterator1>::value_type>::value, bool>::type
    equal(TIterator1 first1, TIterator1 last1, TIterator2 first2)
  {
    typedef typename ETLSTD::iterator_traits<TIterator1>::value_type value_t;

    return (memcmp(first1, first2, sizeof(value_t) * (last1 - last1)) == 0);
  }

  //***************************************************************************
  // lexicographical_compare
  template <typename TIterator1, typename TIterator2, typename TCompare>
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

    //***************************************************************************
  // lexicographical_compare
  template <typename TIterator1, typename TIterator2>
  bool lexicographical_compare(TIterator1 first1, TIterator1 last1,
                               TIterator2 first2, TIterator2 last2)
  {
    typedef ETLSTD::less<typename ETLSTD::iterator_traits<TIterator1>::value_type> compare;

     return ETLSTD::lexicographical_compare(first1, last1, first2, last2, compare());
  }

  //***************************************************************************
  // min
  template <typename T, typename TCompare>
  const T& min(const T& a, const T& b, TCompare compare)
  {
    return (compare(a, b)) ? a : b;
  }

  template <typename T>
  const T& min(const T& a, const T& b)
  {
    typedef ETLSTD::less<T> compare;

    return ETLSTD::min(a, b, compare());
  }

  //***************************************************************************
  // max
  template <typename T, typename TCompare>
  const T& max(const T& a, const T& b, TCompare compare)
  {
    return (compare(a, b)) ? b : a;
  }

  template <typename T>
  const T& max(const T& a, const T& b)
  {
    typedef ETLSTD::less<T> compare;

    return ETLSTD::max(a, b, compare());
  }

  //***************************************************************************
  // transform
  template <typename TIteratorIn, typename TIteratorOut, typename TUnaryOperation>
  TIteratorOut transform(TIteratorIn first1, TIteratorIn last1, TIteratorOut d_first, TUnaryOperation unary_operation)
  {
    while (first1 != last1)
    {
      *d_first++ = unary_operation(*first1++);
    }

    return d_first;
  }

  template <typename TIteratorIn1, typename TIteratorIn2, typename TIteratorOut, typename TBinaryOperation>
  TIteratorOut transform(TIteratorIn1 first1, TIteratorIn1 last1, TIteratorIn2 first2, TIteratorOut d_first, TBinaryOperation binary_operation)
  {
    while (first1 != last1)
    {
      *d_first++ = binary_operation(*first1++, *first2++);
    }

    return d_first;
  }

  //***************************************************************************
  // Heap
  namespace private_heap
  {
    // Push Heap Helper
    template <typename TIterator, typename TDistance, typename TValue, typename TCompare>
    void push_heap(TIterator first, TDistance value_index, TDistance top_index, TValue value, TCompare compare)
    {
      TDistance parent = (value_index - 1) / 2;

      while ((value_index > top_index) && compare(first[parent], value))
      {
        first[value_index] = first[parent];
        value_index = parent;
        parent = (value_index - 1) / 2;
      }

      first[value_index] = value;
    }

    // Adjust Heap Helper
    template <typename TIterator, typename TDistance, typename TValue, typename TCompare>
    void adjust_heap(TIterator first, TDistance value_index, TDistance length, TValue value, TCompare compare)
    {
      TDistance top_index = value_index;
      TDistance child2nd  = (2 * value_index) + 2;

      while (child2nd < length)
      {
        if (compare(first[child2nd], first[child2nd - 1]))
        {
          child2nd--;
        }

        first[value_index] = first[child2nd];
        value_index = child2nd;
        child2nd = 2 * (child2nd + 1);
      }

      if (child2nd == length)
      {
        first[value_index] = first[child2nd - 1];
        value_index = child2nd - 1;
      }

      push_heap(first, value_index, top_index, value, compare);
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
    typedef typename ETLSTD::iterator_traits<TIterator>::value_type value_t;
    typedef typename ETLSTD::iterator_traits<TIterator>::difference_type distance_t;

    value_t value = last[-1];
    last[-1] = first[0];

    private_heap::adjust_heap(first, distance_t(0), distance_t(last - first - 1), value, compare);
  }

  // Pop Heap
  template <typename TIterator>
  void pop_heap(TIterator first, TIterator last)
  {
    typedef ETLSTD::less<typename ETLSTD::iterator_traits<TIterator>::value_type> compare;

    ETLSTD::pop_heap(first, last, compare());
  }

  // Push Heap
  template <typename TIterator, typename TCompare>
  void push_heap(TIterator first, TIterator last, TCompare compare)
  {
    typedef typename ETLSTD::iterator_traits<TIterator>::difference_type difference_t;
    typedef typename ETLSTD::iterator_traits<TIterator>::value_type      value_t;

    private_heap::push_heap(first, difference_t(last - first - 1), difference_t(0), value_t(*(last - 1)), compare);
  }

  // Push Heap
  template <typename TIterator>
  void push_heap(TIterator first, TIterator last)
  {
    typedef ETLSTD::less<typename ETLSTD::iterator_traits<TIterator>::value_type> compare;

    ETLSTD::push_heap(first, last, compare());
  }

  // Make Heap
  template <typename TIterator, typename TCompare>
  void make_heap(TIterator first, TIterator last, TCompare compare)
  {
    typedef typename ETLSTD::iterator_traits<TIterator>::difference_type difference_t;

    if ((last - first) < 2)
    {
      return;
    }

    difference_t length = last - first;
    difference_t parent = (length - 2) / 2;

    while (true)
    {
      private_heap::adjust_heap(first, parent, length, *(first + parent), compare);

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
    typedef ETLSTD::less<typename ETLSTD::iterator_traits<TIterator>::value_type> compare;

    ETLSTD::make_heap(first, last, compare());
  }

  // Is Heap
  template <typename TIterator>
  bool is_heap(TIterator first, TIterator last)
  {
    typedef ETLSTD::less<typename ETLSTD::iterator_traits<TIterator>::value_type> compare;

    return private_heap::is_heap(first, last - first, compare());
  }

  // Is Heap
  template <typename TIterator, typename TCompare>
  bool is_heap(TIterator first, TIterator last, TCompare compare)
  {
    return private_heap::is_heap(first, last - first, compare);
  }

  // Search
  template<typename TIterator1, typename TIterator2, typename TCompare>
  TIterator1 search(TIterator1 first, TIterator1 last, TIterator2 search_first, TIterator2 search_last, TCompare compare)
  {
    if (search_first == search_last)
    {
      return first;
    }

    while (first != last)
    {
      TIterator1 itr1 = first;
      TIterator2 itr2 = search_first;

      while (compare(*itr1,*itr2))
      {
        if (itr2 == search_last)
        {
          return first;
        }

        if (itr1 == last)
        {
          return last;
        }

        ++itr1;
        ++itr2;
      }

      ++first;
    }

    return last;
  }

  // Search
  template<typename TIterator1, class TIterator2>
  TIterator1 search(TIterator1 first, TIterator1 last, TIterator2 search_first, TIterator2 search_last)
  {
    typedef ETLSTD::equal_to<typename ETLSTD::iterator_traits<TIterator1>::value_type> compare;

    return ETLSTD::search(first, last, search_first, search_last, compare());
  }
}

#endif


