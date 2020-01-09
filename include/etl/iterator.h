///\file

/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl
https://www.etlcpp.com

Copyright(c) 2017 jwellbelove

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

#ifndef ETL_ITERATOR_INCLUDED
#define ETL_ITERATOR_INCLUDED

#include "platform.h"
#include "type_traits.h"

#if !defined(ETL_NO_STL)
  #include <iterator>
#endif

///\defgroup iterator iterator
///\ingroup utilities

namespace etl
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
    typedef ETL_OR_STD::random_access_iterator_tag iterator_category;
    typedef T                              value_type;
    typedef ptrdiff_t                      difference_type;
    typedef T*                             pointer;
    typedef T&                             reference;
  };

  template <typename T>
  struct iterator_traits<const T*>
  {
    typedef ETL_OR_STD::random_access_iterator_tag iterator_category;
    typedef T                              value_type;
    typedef ptrdiff_t                      difference_type;
    typedef const T*                       pointer;
    typedef const T&                       reference;
  };

  //***************************************************************************
  // advance
#if defined(ETL_NO_STL)

  template <typename TIterator, typename TDistance>
  ETL_CONSTEXPR17 void advance_helper(TIterator& itr, TDistance n, ETL_OR_STD::output_iterator_tag)
  {
    while (n--)
    {
      ++itr;
    }
  }

  template <typename TIterator, typename TDistance>
  ETL_CONSTEXPR17 void advance_helper(TIterator& itr, TDistance n, ETL_OR_STD::forward_iterator_tag)
  {
    while (n--)
    {
      ++itr;
    }
  }

  template <typename TIterator, typename TDistance>
  ETL_CONSTEXPR17 void advance_helper(TIterator& itr, TDistance n, ETL_OR_STD::bidirectional_iterator_tag)
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
  ETL_CONSTEXPR17 void advance_helper(TIterator& itr, TDistance n, ETL_OR_STD::random_access_iterator_tag)
  {
    itr += n;
  }

  template <typename TIterator, typename TDistance>
  ETL_CONSTEXPR17 void advance(TIterator& itr, TDistance n)
  {
    typedef typename etl::iterator_traits<TIterator>::iterator_category tag;

    advance_helper(itr, n, tag());
  }

#else

  template <typename TIterator, typename TDistance>
  ETL_CONSTEXPR17 void advance(TIterator& itr, TDistance n)
  {
    std::advance(itr, n);
  }

#endif

  //***************************************************************************
  // distance
#if defined(ETL_NO_STL)
  template<typename TIterator>
  ETL_CONSTEXPR17 typename etl::iterator_traits<TIterator>::difference_type distance_helper(TIterator first, TIterator last, ETL_OR_STD::input_iterator_tag)
  {
    typename etl::iterator_traits<TIterator>::difference_type d = 0;

    while (first != last)
    {
      ++d;
      ++first;
    }

    return d;
  }

  template<typename TIterator>
  ETL_CONSTEXPR17 typename etl::iterator_traits<TIterator>::difference_type distance_helper(TIterator first, TIterator last, ETL_OR_STD::forward_iterator_tag)
  {
    typename etl::iterator_traits<TIterator>::difference_type d = 0;

    while (first != last)
    {
      ++d;
      ++first;
    }

    return d;
  }

  template<typename TIterator>
  ETL_CONSTEXPR17 typename etl::iterator_traits<TIterator>::difference_type distance_helper(TIterator first, TIterator last, ETL_OR_STD::bidirectional_iterator_tag)
  {
    typename etl::iterator_traits<TIterator>::difference_type d = 0;

    while (first != last)
    {
      ++d;
      ++first;
    }

    return d;
  }

  template<typename TIterator>
  ETL_CONSTEXPR17 typename etl::iterator_traits<TIterator>::difference_type distance_helper(TIterator first, TIterator last, ETL_OR_STD::random_access_iterator_tag)
  {
    return last - first;
  }

  template<typename TIterator>
  ETL_CONSTEXPR17 typename etl::iterator_traits<TIterator>::difference_type distance(TIterator first, TIterator last)
  {
    typedef typename etl::iterator_traits<TIterator>::iterator_category tag;

    return distance_helper(first, last, tag());
  }

#else

  template<typename TIterator>
  ETL_CONSTEXPR17 typename std::iterator_traits<TIterator>::difference_type distance(TIterator first, TIterator last)
  {
    return std::distance(first, last);
  }

#endif

  //***************************************************************************
  // Previous
  template<typename TIterator>
  ETL_CONSTEXPR17 TIterator prev(TIterator itr, typename etl::iterator_traits<TIterator>::difference_type n = 1)
  {
#if defined(ETL_NO_STL) || !ETL_CPP11_SUPPORTED
    etl::advance(itr, -n);
#else
    std::advance(itr, -n);
#endif
    return itr;
  }

  //***************************************************************************
  // Next
  template<typename TIterator>
  ETL_CONSTEXPR17 TIterator next(TIterator itr, typename etl::iterator_traits<TIterator>::difference_type n = 1)
  {
#if defined(ETL_NO_STL) || !ETL_CPP11_SUPPORTED
    etl::advance(itr, n);
#else
    std::advance(itr, n);
#endif
    return itr;
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
  /// iterator
  //***************************************************************************
  template <typename TCategory, typename T, typename TDistance = ptrdiff_t, typename TPointer = T* , typename TReference = T& >
  struct iterator
  {
    typedef T          value_type;
    typedef TDistance  difference_type;
    typedef TPointer   pointer;
    typedef TReference reference;
    typedef TCategory  iterator_category;
  };

}

namespace etl
{
  //***************************************************************************
// Helper templates.
//***************************************************************************
  template <typename T>
  struct is_input_iterator
  {
    static ETL_CONST_OR_CONSTEXPR bool value = etl::is_same<typename etl::iterator_traits<T>::iterator_category, ETL_OR_STD::input_iterator_tag>::value;
  };

  template <typename T>
  struct is_output_iterator
  {
    static ETL_CONST_OR_CONSTEXPR bool value = etl::is_same<typename etl::iterator_traits<T>::iterator_category, ETL_OR_STD::output_iterator_tag>::value;
  };

  template <typename T>
  struct is_forward_iterator
  {
    static ETL_CONST_OR_CONSTEXPR bool value = etl::is_same<typename etl::iterator_traits<T>::iterator_category, ETL_OR_STD::forward_iterator_tag>::value;
  };

  template <typename T>
  struct is_bidirectional_iterator
  {
    static ETL_CONST_OR_CONSTEXPR bool value = etl::is_same<typename etl::iterator_traits<T>::iterator_category, ETL_OR_STD::bidirectional_iterator_tag>::value;
  };

  template <typename T>
  struct is_random_iterator
  {
    static ETL_CONST_OR_CONSTEXPR bool value = etl::is_same<typename etl::iterator_traits<T>::iterator_category, ETL_OR_STD::random_access_iterator_tag>::value;
  };

  template <typename T>
  struct is_input_iterator_concept
  {
    static ETL_CONST_OR_CONSTEXPR bool value = etl::is_input_iterator<T>::value ||
                                               etl::is_forward_iterator<T>::value ||
                                               etl::is_bidirectional_iterator<T>::value ||
                                               etl::is_random_iterator<T>::value;
  };

  template <typename T>
  struct is_output_iterator_concept
  {
    static ETL_CONST_OR_CONSTEXPR bool value = etl::is_output_iterator<T>::value ||
                                               etl::is_forward_iterator<T>::value ||
                                               etl::is_bidirectional_iterator<T>::value ||
                                               etl::is_random_iterator<T>::value;
  };

  template <typename T>
  struct is_forward_iterator_concept
  {
    static ETL_CONST_OR_CONSTEXPR bool value = etl::is_forward_iterator<T>::value ||
                                               etl::is_bidirectional_iterator<T>::value ||
                                               etl::is_random_iterator<T>::value;
  };

  template <typename T>
  struct is_bidirectional_iterator_concept
  {
    static ETL_CONST_OR_CONSTEXPR bool value = etl::is_bidirectional_iterator<T>::value ||
                                               etl::is_random_iterator<T>::value;
  };

  template <typename T>
  struct is_random_iterator_concept
  {
    static ETL_CONST_OR_CONSTEXPR bool value = etl::is_random_iterator<T>::value;
  };
}

#endif

