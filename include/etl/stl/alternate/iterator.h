
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

#include <string.h>
#include "../../type_traits.h"

#if defined(ETL_IN_UNIT_TEST)
namespace etlstd
#else
namespace std
#endif
{
  //***************************************************************************
  // iterator
  template <typename TCategory, typename T, typename TDistance = ptrdiff_t, typename TPointer = T* , typename TReference = T&>
  struct iterator
  {
    typedef T          value_type;
    typedef TDistance  difference_type;
    typedef TPointer   pointer;
    typedef TReference reference;
    typedef TCategory  iterator_category;
  };

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
    typedef typename TIterator::difference_type   difference_type;
    typedef typename TIterator::value_type        value_type;
    typedef typename TIterator::pointer           pointer;
    typedef typename TIterator::reference         reference;
    typedef typename TIterator::iterator_category iterator_category;
  };

  template <typename T>
  struct iterator_traits<T*>
  {
    typedef ptrdiff_t                  difference_type;
    typedef T                          value_type;
    typedef T*                         pointer;
    typedef T&                         reference;
    typedef random_access_iterator_tag iterator_category ;
  };

  template <typename T>
  struct iterator_traits<const T*>
  {
    typedef ptrdiff_t                  difference_type;
    typedef T                          value_type;
    typedef T*                         pointer;
    typedef T&                         reference;
    typedef random_access_iterator_tag iterator_category ;
  };

  //***************************************************************************
  // advance
  template <typename TIterator, typename TDistance>
  typename etl::enable_if<etl::is_same<typename iterator_traits<TIterator>::iterator_catagory, input_iterator_tag>::value, void>::type
    advance(TIterator& itr, TDistance n)
  {
    while (n--)
    {
      ++itr;
    }
  }

  template <typename TIterator, typename TDistance>
  typename etl::enable_if<etl::is_same<typename iterator_traits<TIterator>::iterator_catagory, output_iterator_tag>::value, void>::type
    advance(TIterator& itr, TDistance n)
  {
    while (n--)
    {
      ++itr;
    }
  }

  template <typename TIterator, typename TDistance>
  typename etl::enable_if<etl::is_same<typename iterator_traits<TIterator>::iterator_catagory, forward_iterator_tag>::value, void>::type
    advance(TIterator& itr, TDistance n)
  {
    while (n--)
    {
      ++itr;
    }
  }

  template <typename TIterator, typename TDistance>
  typename etl::enable_if<etl::is_same<typename iterator_traits<TIterator>::iterator_catagory, bidirectional_iterator_tag>::value, void>::type
    advance(TIterator& itr, TDistance n)
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
      while (n--)
      {
        --itr;
      }
    }
  }

  template <typename TIterator, typename TDistance>
  typename etl::enable_if<etl::is_same<typename iterator_traits<TIterator>::iterator_catagory, random_access_iterator_tag>::value, void>::type
    advance(TIterator& itr, TDistance n)
  {
    itr += n;
  }

  //***************************************************************************
  // distance
  template<typename TIterator>
  typename etl::enable_if<!etl::is_same<typename iterator_traits<TIterator>::iterator_catagory, random_access_iterator_tag>::value,
                          typename iterator_traits<TIterator>::difference_type>::type
    distance(TIterator first, TIterator last)
  {
    typename iterator_traits<TIterator>::difference_type d = 0;

    while (first != last)
    {
      ++d;
    }

    return d;
  }

  template<typename TIterator>
  typename etl::enable_if<etl::is_same<typename iterator_traits<TIterator>::iterator_catagory, random_access_iterator_tag>::value,
                          typename iterator_traits<TIterator>::difference_type>::type
    distance(TIterator first, TIterator last)
  {
    return last - first;
  }

  //***************************************************************************
  // reverse_iterator
  template <typename TIterator>
  struct reverse_iterator
  {
  public:

    typedef typename iterator_traits<TIterator>::difference_type   difference_type;
    typedef typename iterator_traits<TIterator>::value_type        value_type;
    typedef typename iterator_traits<TIterator>::pointer           pointer;
    typedef typename iterator_traits<TIterator>::reference         reference;
    typedef typename iterator_traits<TIterator>::iterator_category iterator_category;

    reverse_iterator()
    {
    }

    explicit reverse_iterator(TIterator itr) : current(itr)
    {
    }

    reverse_iterator(const reverse_iterator<TIterator>& itr)
      : current(itr.current)
    {
    }

    template <typename UITerator>
    reverse_iterator(const reverse_iterator<UITerator>& itr)
      : current(itr.base())
    {
    }

    reverse_iterator<TIterator>& operator = (const reverse_iterator<TIterator>& itr)
    {
      current = itr.base();
      return *this;
    }

    template <typename UITerator>
    reverse_iterator<TIterator>& operator = (const reverse_iterator<UITerator>& itr)
    {
      current = itr.base();
      return *this;
    }

    TIterator base() const
    {
      return current;
    }

    ETL_CONSTEXPR reference operator *() const
    {
      TIterator temp = current;
      --temp;
      return *temp;
    }

    ETL_CONSTEXPR pointer operator ->() const
    {
      TIterator temp = current;
      --temp;
      return &(*temp);
    }

    ETL_CONSTEXPR reverse_iterator<TIterator>& operator ++()
    {
      --current;
      return *this;
    }

    ETL_CONSTEXPR reverse_iterator<TIterator> operator ++(int)
    {
      reverse_iterator<TIterator> temp = *this;
      --current;
      return temp;
    }

    ETL_CONSTEXPR reverse_iterator<TIterator>& operator --()
    {
      ++current;
      return *this;
    }

    ETL_CONSTEXPR reverse_iterator<TIterator> operator --(int)
    {
      reverse_iterator<TIterator> temp = *this;
      ++current;
      return temp;
    }

    ETL_CONSTEXPR reverse_iterator<TIterator> operator +(difference_type n) const
    {
      return reverse_iterator<TIterator>(current - n);
    }

    ETL_CONSTEXPR reverse_iterator<TIterator>& operator +=(difference_type n)
    {
      current -= n;
      return *this;
    }

    ETL_CONSTEXPR reverse_iterator<TIterator> operator -(difference_type n) const
    {
      return reverse_iterator<TIterator>(current + n);
    }

    ETL_CONSTEXPR reverse_iterator<TIterator>& operator -=(difference_type n)
    {
      current += n;
      return *this;
    }

    ETL_CONSTEXPR reference operator [](difference_type n) const
    {
      return *(*this + n);
    }

  protected:

    TIterator current;
  };

  template <class TIterator>
  inline ETL_CONSTEXPR bool operator <(const reverse_iterator<TIterator>& lhs, const reverse_iterator<TIterator>& rhs)
  {
    return rhs.base() < lhs.base();
  }

  template <class TIterator>
  inline ETL_CONSTEXPR bool operator !=(const reverse_iterator<TIterator>& lhs, const reverse_iterator<TIterator>& rhs)
  {
    return !(lhs == rhs);
  }

  template <class TIterator>
  inline ETL_CONSTEXPR bool operator >(const reverse_iterator<TIterator>& lhs, const reverse_iterator<TIterator>& rhs)
  {
    return rhs < lhs;
  }

  template <class TIterator>
  inline ETL_CONSTEXPR bool operator <=(const reverse_iterator<TIterator>& lhs, const reverse_iterator<TIterator>& rhs)
  {
    return !(rhs < lhs);
  }

  template <class TIterator>
  inline ETL_CONSTEXPR bool operator >=(const reverse_iterator<TIterator>& lhs, const reverse_iterator<TIterator>& rhs)
  {
    return !(lhs < rhs);
  }

  template <class TIterator>
  inline ETL_CONSTEXPR typename reverse_iterator<TIterator>::difference_type operator -(const reverse_iterator<TIterator>& lhs, const reverse_iterator<TIterator>& rhs)
  {
    return rhs.base() - lhs.base();
  }

  template <class TIterator, class TDifference>
  inline ETL_CONSTEXPR reverse_iterator<TIterator> operator +(TDifference n, const reverse_iterator<TIterator>& itr)
  {
    return itr.operator +(n);
  }
}

#endif
