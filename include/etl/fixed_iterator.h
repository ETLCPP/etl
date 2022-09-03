///\file

/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl
https://www.etlcpp.com

Copyright(c) 2015 John Wellbelove

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

#ifndef ETL_FIXED_ITERATOR_INCLUDED
#define ETL_FIXED_ITERATOR_INCLUDED

#include "platform.h"
#include "iterator.h"

///\defgroup iterator Iterator types

namespace etl
{
  /// A fixed iterator class.
  /// This iterator can be given an iterator value, which will not be allowed to be incremented or decremented.
  /// This can be useful when using STL algorithms to interact with fixed memory locations such as registers.
  ///\ingroup iterator
  template <typename TIterator>
  class fixed_iterator : etl::iterator<typename etl::iterator_traits<TIterator>::iterator_category, typename etl::iterator_traits<TIterator>::value_type>
  {
  public:

    //***************************************************************************
    /// Default constructor.
    //***************************************************************************
    fixed_iterator()
      : it(TIterator())
    {
    }

    //***************************************************************************
    /// Construct from iterator.
    //***************************************************************************
    fixed_iterator(TIterator it_)
      : it(it_)
    {
    }

    //***************************************************************************
    /// Copy constructor
    //***************************************************************************
    fixed_iterator(const fixed_iterator& other)
      : it(other.it)
    {
    }

    //***************************************************************************
    /// Increment (Does nothing).
    //***************************************************************************
    fixed_iterator& operator ++()
    {
      return *this;
    }

    //***************************************************************************
    /// Increment (Does nothing).
    //***************************************************************************
    fixed_iterator operator ++(int)
    {
      return *this;
    }

    //***************************************************************************
    /// Decrement (Does nothing).
    //***************************************************************************
    fixed_iterator& operator --()
    {
      return *this;
    }

    //***************************************************************************
    /// Decrement (Does nothing).
    //***************************************************************************
    fixed_iterator operator --(int)
    {
      return *this;
    }

    //***************************************************************************
    /// Dereference operator.
    //***************************************************************************
    typename etl::iterator_traits<TIterator>::value_type operator *()
    {
      return *it;
    }

    //***************************************************************************
    /// Dereference operator.
    //***************************************************************************
    const typename etl::iterator_traits<TIterator>::value_type operator *() const
    {
      return *it;
    }

    //***************************************************************************
    /// -> operator.
    //***************************************************************************
    TIterator operator ->()
    {
      return it;
    }

    //***************************************************************************
    /// -> operator.
    //***************************************************************************
    const TIterator operator ->() const
    {
      return it;
    }

    //***************************************************************************
    /// Conversion operator.
    //***************************************************************************
    operator TIterator() const
    {
      return it;
    }

    //***************************************************************************
    /// += operator.
    //***************************************************************************
    fixed_iterator& operator +=(typename etl::iterator_traits<TIterator>::difference_type /*offset*/)
    {
      return *this;
    }

    //***************************************************************************
    /// -= operator.
    //***************************************************************************
    fixed_iterator& operator -=(typename etl::iterator_traits<TIterator>::difference_type /*offset*/)
    {
      return *this;
    }

    //***************************************************************************
    /// Assignment from iterator.
    //***************************************************************************
    fixed_iterator& operator =(TIterator new_it)
    {
      it = new_it;
      return *this;
    }

    //***************************************************************************
    /// Assignment from fixed_iterator.
    //***************************************************************************
    fixed_iterator& operator =(fixed_iterator other)
    {
      it = other.it;
      return *this;
    }

  private:

    TIterator it; ///< The underlying iterator.
  };

  //*****************************************************************************
  /// + difference operator.
  //*****************************************************************************
  template <typename TIterator>
  etl::fixed_iterator<TIterator>& operator +(etl::fixed_iterator<TIterator>& lhs,
                                            typename etl::iterator_traits<TIterator>::difference_type /*rhs*/)
  {
    return lhs;
  }

  //*****************************************************************************
  /// - difference operator.
  //*****************************************************************************
  template <typename TIterator>
  etl::fixed_iterator<TIterator>& operator -(etl::fixed_iterator<TIterator>& lhs,
                                            typename etl::iterator_traits<TIterator>::difference_type /*rhs*/)
  {
    return lhs;
  }

  //*****************************************************************************
  /// - fixed_iterator operator.
  //*****************************************************************************
  template <typename TIterator>
  typename etl::iterator_traits<TIterator>::difference_type operator -(const etl::fixed_iterator<TIterator>& lhs,
                                                                       const etl::fixed_iterator<TIterator>& rhs)
  {
    return TIterator(lhs) - TIterator(rhs);
  }

  //*****************************************************************************
  /// Equality operator. fixed_iterator == fixed_iterator.
  //*****************************************************************************
  template <typename TIterator>
  bool operator ==(const etl::fixed_iterator<TIterator>& lhs,
                   const etl::fixed_iterator<TIterator>& rhs)
  {
    return TIterator(lhs) == TIterator(rhs);
  }

  //*****************************************************************************
  /// Equality operator. fixed_iterator == iterator.
  //*****************************************************************************
  template <typename TIterator>
  bool operator ==(const etl::fixed_iterator<TIterator>& lhs,
                   TIterator rhs)
  {
    return TIterator(lhs) == rhs;
  }

  //*****************************************************************************
  /// Equality operator. iterator == fixed_iterator.
  //*****************************************************************************
  template <typename TIterator>
  bool operator ==(TIterator lhs,
                   const etl::fixed_iterator<TIterator>& rhs)
  {
    return lhs == TIterator(rhs);
  }


  //*****************************************************************************
  /// Inequality operator. fixed_iterator == fixed_iterator.
  //*****************************************************************************
  template <typename TIterator>
  bool operator !=(const etl::fixed_iterator<TIterator>& lhs,
                   const etl::fixed_iterator<TIterator>& rhs)
  {
    return !(lhs == rhs);
  }

  //*****************************************************************************
  /// Inequality operator. fixed_iterator == iterator.
  //*****************************************************************************
  template <typename TIterator>
  bool operator !=(const etl::fixed_iterator<TIterator>& lhs,
                   TIterator rhs)
  {
    return !(lhs == rhs);
  }

  //*****************************************************************************
  /// Inequality operator. iterator == fixed_iterator.
  //*****************************************************************************
  template <typename TIterator>
  bool operator !=(TIterator& lhs,
                   const etl::fixed_iterator<TIterator>& rhs)
  {
    return !(lhs == rhs);
  }
}

#endif
