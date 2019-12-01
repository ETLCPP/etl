
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

#ifndef ETL_STL_ALTERNATE_ITERATOR_INCLUDED
#define ETL_STL_ALTERNATE_ITERATOR_INCLUDED

#include "../../platform.h"
#include "../../type_traits.h"

#include <string.h>

#include "../../private/choose_tag_types.h"
#include "../../private/choose_pair_types.h"

namespace etlstd
{
  //***************************************************************************
  // iterator tags
  struct input_iterator_tag {};
  struct output_iterator_tag {};
  struct forward_iterator_tag : public input_iterator_tag {};
  struct bidirectional_iterator_tag : public forward_iterator_tag {};
  struct random_access_iterator_tag : public bidirectional_iterator_tag {};

  //***************************************************************************
  // iterator_traits
  template <typename TIterator>
  struct iterator_traits
  {
    typedef typename TIterator::iterator_category iterator_category;
    typedef typename TIterator::value_type        value_type;
    typedef typename TIterator::difference_type   difference_type;
    typedef typename TIterator::pointer           pointer;
    typedef typename TIterator::reference         reference;
  };

  template <typename T>
  struct iterator_traits<T*>
  {
    typedef ETL_RANDOM_ACCESS_ITERATOR_TAG iterator_category;
    typedef T                              value_type;
    typedef ptrdiff_t                      difference_type;
    typedef T*                             pointer;
    typedef T&                             reference;
  };

  template <typename T>
  struct iterator_traits<const T*>
  {
    typedef ETL_RANDOM_ACCESS_ITERATOR_TAG iterator_category;
    typedef T                              value_type;
    typedef ptrdiff_t                      difference_type;
    typedef const T*                       pointer;
    typedef const T&                       reference;
  };

  //***************************************************************************
  // advance
  template <typename TIterator, typename TDistance>
  ETL_CONSTEXPR17 void advance_helper(TIterator& itr, TDistance n, ETL_INPUT_ITERATOR_TAG)
  {
    while (n--)
    {
      ++itr;
    }
  }

  template <typename TIterator, typename TDistance>
  ETL_CONSTEXPR17 void advance_helper(TIterator& itr, TDistance n, ETL_OUTPUT_ITERATOR_TAG)
  {
    while (n--)
    {
      ++itr;
    }
  }

  template <typename TIterator, typename TDistance>
  ETL_CONSTEXPR17 void advance_helper(TIterator& itr, TDistance n, ETL_FORWARD_ITERATOR_TAG)
  {
    while (n--)
    {
      ++itr;
    }
  }

  template <typename TIterator, typename TDistance>
  ETL_CONSTEXPR17 void advance_helper(TIterator& itr, TDistance n, ETL_BIDIRECTIONAL_ITERATOR_TAG)
  {
    if (n > 0)
    {
      while (n--)
      {
        ++itr;
      }
    }
    else
    {
      while (n++)
      {
        --itr;
      }
    }
  }

  template <typename TIterator, typename TDistance>
  ETL_CONSTEXPR17 void advance_helper(TIterator& itr, TDistance n, ETL_RANDOM_ACCESS_ITERATOR_TAG)
  {
    itr += n;
  }

  template <typename TIterator, typename TDistance>
  ETL_CONSTEXPR17 void advance(TIterator& itr, TDistance n)
  {
    typedef typename etlstd::iterator_traits<TIterator>::iterator_category tag;

    advance_helper(itr, n, tag());
  }

  //***************************************************************************
  // distance
  template<typename TIterator>
  ETL_CONSTEXPR17 typename etlstd::iterator_traits<TIterator>::difference_type distance_helper(TIterator first, TIterator last, ETL_INPUT_ITERATOR_TAG)
  {
    typename etlstd::iterator_traits<TIterator>::difference_type d = 0;

    while (first != last)
    {
      ++d;
      ++first;
    }

    return d;
  }

  template<typename TIterator>
  ETL_CONSTEXPR17 typename etlstd::iterator_traits<TIterator>::difference_type distance_helper(TIterator first, TIterator last, ETL_FORWARD_ITERATOR_TAG)
  {
    typename etlstd::iterator_traits<TIterator>::difference_type d = 0;

    while (first != last)
    {
      ++d;
      ++first;
    }

    return d;
  }

  template<typename TIterator>
  ETL_CONSTEXPR17 typename etlstd::iterator_traits<TIterator>::difference_type distance_helper(TIterator first, TIterator last, ETL_BIDIRECTIONAL_ITERATOR_TAG)
  {
    typename etlstd::iterator_traits<TIterator>::difference_type d = 0;

    while (first != last)
    {
      ++d;
      ++first;
    }

    return d;
  }

  template<typename TIterator>
  ETL_CONSTEXPR17 typename etlstd::iterator_traits<TIterator>::difference_type distance_helper(TIterator first, TIterator last, ETL_RANDOM_ACCESS_ITERATOR_TAG)
  {
    return last - first;
  }

  template<typename TIterator>
  ETL_CONSTEXPR17 typename etlstd::iterator_traits<TIterator>::difference_type distance(TIterator first, TIterator last)
  {
    typedef typename etlstd::iterator_traits<TIterator>::iterator_category tag;

    return distance_helper(first, last, tag());
  }

  //***************************************************************************
  // reverse_iterator
  template <typename TIterator>
  class reverse_iterator
  {
  public:

    typedef typename iterator_traits<TIterator>::iterator_category iterator_category;
    typedef typename iterator_traits<TIterator>::value_type        value_type;
    typedef typename iterator_traits<TIterator>::difference_type   difference_type;
    typedef typename iterator_traits<TIterator>::pointer           pointer;
    typedef typename iterator_traits<TIterator>::reference         reference;

    typedef TIterator iterator_type;

    ETL_CONSTEXPR17 reverse_iterator()
      : current()
    {
    }

    ETL_CONSTEXPR17 explicit reverse_iterator(TIterator itr)
      : current(itr)
    {
    }

    template <typename TOther>
    ETL_CONSTEXPR17 reverse_iterator(const reverse_iterator<TOther>& other)
      : current(other.base())
    {
    }

    template<class TOther>
    ETL_CONSTEXPR17 reverse_iterator& operator=(const reverse_iterator<TOther>& other)
    {
      current = other.base();

      return (*this);
    }

    ETL_CONSTEXPR17 TIterator base() const
    {
      return current;
    }

    ETL_NODISCARD ETL_CONSTEXPR17 reference operator*() const
    {
      TIterator temp = current;

      return *(--temp);
    }

    ETL_NODISCARD ETL_CONSTEXPR17 pointer operator->() const
    {
      TIterator temp = current;

      return &(*--temp);
    }

    ETL_CONSTEXPR17 reverse_iterator& operator++()
    {
      --current;

      return *this;
    }

    ETL_CONSTEXPR17 reverse_iterator operator++(int)
    {
      reverse_iterator temp = *this;
      --current;

      return temp;
    }

    ETL_CONSTEXPR17 reverse_iterator& operator--()
    {
      ++current;

      return (*this);
    }

    ETL_CONSTEXPR17 reverse_iterator operator--(int)
    {
      reverse_iterator temp = *this;
      ++current;

      return temp;
    }

    ETL_CONSTEXPR17 reverse_iterator& operator+=(const difference_type offset)
    {
      current -= offset;

      return (*this);
    }

    ETL_CONSTEXPR17 reverse_iterator& operator-=(const difference_type offset)
    {
      current += offset;

      return (*this);
    }

    ETL_NODISCARD ETL_CONSTEXPR17 reverse_iterator operator+(const difference_type offset) const
    {
      return reverse_iterator(current - offset);
    }

    ETL_NODISCARD ETL_CONSTEXPR17 reverse_iterator operator-(const difference_type offset) const
    {
      return (reverse_iterator(current + offset));
    }

    ETL_NODISCARD ETL_CONSTEXPR17 reference operator[](const difference_type offset) const
    {
      return (*(*this + offset));
    }

  protected:

    TIterator current;
  };

  template <class TIterator>
  inline ETL_CONSTEXPR17 bool operator ==(const reverse_iterator<TIterator>& lhs, const reverse_iterator<TIterator>& rhs)
  {
    return lhs.base() == rhs.base();
  }

  template <class TIterator>
  inline ETL_CONSTEXPR17 bool operator !=(const reverse_iterator<TIterator>& lhs, const reverse_iterator<TIterator>& rhs)
  {
    return !(lhs == rhs);
  }

  template <class TIterator>
  inline ETL_CONSTEXPR17 bool operator <(const reverse_iterator<TIterator>& lhs, const reverse_iterator<TIterator>& rhs)
  {
    return rhs.base() < lhs.base();
  }

  template <class TIterator>
  inline ETL_CONSTEXPR17 bool operator >(const reverse_iterator<TIterator>& lhs, const reverse_iterator<TIterator>& rhs)
  {
    return rhs < lhs;
  }

  template <class TIterator>
  inline ETL_CONSTEXPR17 bool operator <=(const reverse_iterator<TIterator>& lhs, const reverse_iterator<TIterator>& rhs)
  {
    return !(rhs < lhs);
  }

  template <class TIterator>
  inline ETL_CONSTEXPR17 bool operator >=(const reverse_iterator<TIterator>& lhs, const reverse_iterator<TIterator>& rhs)
  {
    return !(lhs < rhs);
  }

  template <class TIterator>
  inline ETL_CONSTEXPR17 typename reverse_iterator<TIterator>::difference_type operator -(const reverse_iterator<TIterator>& lhs, const reverse_iterator<TIterator>& rhs)
  {
    return rhs.base() - lhs.base();
  }

  template <class TIterator, class TDifference>
  inline ETL_CONSTEXPR17 reverse_iterator<TIterator> operator +(TDifference n, const reverse_iterator<TIterator>& itr)
  {
    return itr.operator +(n);
  }

  //***************************************************************************
  // Previous
  template<typename TIterator>
  ETL_CONSTEXPR17 TIterator prev(TIterator itr, typename etlstd::iterator_traits<TIterator>::difference_type n = 1)
  {
    etlstd::advance(itr, -n);
    return itr;
  }

  //***************************************************************************
  // Next
  template<typename TIterator>
  ETL_CONSTEXPR17 TIterator next(TIterator itr, typename etlstd::iterator_traits<TIterator>::difference_type n = 1)
  {
    etlstd::advance(itr, n);
    return itr;
  }
}

#endif
