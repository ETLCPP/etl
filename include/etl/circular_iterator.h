///\file

/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl
https://www.etlcpp.com

Copyright(c) 2022 John Wellbelove

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

#ifndef ETL_CIRCULAR_ITERATOR_INCLUDED
#define ETL_CIRCULAR_ITERATOR_INCLUDED

#include "platform.h"
#include "iterator.h"
#include "span.h"

///\defgroup iterator Iterator types

namespace etl
{
  /// A circular iterator class.
  /// This iterator can be given a pair of iterator values, which will loop if the start or end of the range is reached.
  ///\ingroup iterator
  template <typename TIterator>
  class circular_iterator : etl::iterator<typename etl::iterator_traits<TIterator>::iterator_category, typename etl::iterator_traits<TIterator>::value_type>
  {
  public:

    typedef typename etl::iterator_traits<TIterator>::value_type        value_type;
    typedef typename etl::iterator_traits<TIterator>::difference_type   difference_type;
    typedef typename etl::iterator_traits<TIterator>::pointer           pointer;
    typedef typename etl::iterator_traits<TIterator>::reference         reference;
    typedef typename etl::iterator_traits<TIterator>::iterator_category iterator_category;

    //***************************************************************************
    /// Default constructor.
    //***************************************************************************
    circular_iterator()
      : itr_begin(TIterator())
      , itr_end(TIterator())
      , itr(TIterator())
    {
    }

    //***************************************************************************
    /// Construct from iterators.
    //***************************************************************************
    circular_iterator(TIterator itr_begin_, TIterator itr_end_)
      : itr_begin(itr_begin_)
      , itr_end(itr_end_)
      , itr(itr_begin_)
    {
    }

    //***************************************************************************
    /// Construct from span.
    //***************************************************************************
    template <typename T>
    circular_iterator(const etl::span<T>& span_)
      : itr_begin(span_.begin())
      , itr_end(span_.end())
      , itr(itr_begin)
    {
    }

    //***************************************************************************
    /// Copy constructor
    //***************************************************************************
    circular_iterator(const circular_iterator& other)
      : itr_begin(other.itr_begin)
      , itr_end(other.itr_end)
      , itr(other.itr)
    {
    }

    //***************************************************************************
    /// Beginning of the range.
    //***************************************************************************
    TIterator begin() const
    {
      return itr_begin;
    }

    //***************************************************************************
    /// End of the range.
    //***************************************************************************
    TIterator end() const
    {
      return itr_end;
    }

    //***************************************************************************
    /// How long is the range?
    //***************************************************************************
    size_t size() const
    {
      return etl::distance(itr_begin, itr_end);
    }

    //***************************************************************************
    /// Is there nothing to iteratte over?
    //***************************************************************************
    bool empty() const
    {
      return (itr_begin == itr_end);
    }

    //***************************************************************************
    /// Increment.
    //***************************************************************************
    circular_iterator& operator ++()
    {
      if (++itr == itr_end)
      {
        itr = itr_begin;
      }

      return *this;
    }

    //***************************************************************************
    /// Increment.
    //***************************************************************************
    circular_iterator operator ++(int)
    {
      circular_iterator original(*this);

      ++(*this);

      return (original);
    }

    //***************************************************************************
    /// Decrement.
    //***************************************************************************
    circular_iterator& operator --()
    {
      if (itr == itr_begin)
      {
        typename etl::reverse_iterator<TIterator> ritr(itr_end);
        ++ritr;
        itr = ritr.base();
      }
      else
      {
        --itr;
      }

      return *this;
    }

    //***************************************************************************
    /// Decrement.
    //***************************************************************************
    circular_iterator operator --(int)
    {
      circular_iterator original(*this);

      --(*this);

      return (original);
    }

    //***************************************************************************
    /// Dereference operator.
    //***************************************************************************
    value_type operator *()
    {
      return *itr;
    }

    //***************************************************************************
    /// Dereference operator.
    //***************************************************************************
    const value_type operator *() const
    {
      return *itr;
    }

    //***************************************************************************
    /// -> operator.
    //***************************************************************************
    TIterator operator ->()
    {
      return itr;
    }

    //***************************************************************************
    /// -> operator.
    //***************************************************************************
    const TIterator operator ->() const
    {
      return itr;
    }

    //***************************************************************************
    /// Conversion operator.
    //***************************************************************************
    operator TIterator() const
    {
      return itr;
    }

    //***************************************************************************
    /// += operator.
    //***************************************************************************
    circular_iterator& operator +=(difference_type offset)
    {
      if (offset != 0)
      {
        if (offset > 0)
        {
          difference_type distance_to_end = etl::distance(itr, itr_end);
          offset %= difference_type(size());

          if (distance_to_end > offset)
          {
            etl::advance(itr, offset);
          }
          else
          {            
            offset -= distance_to_end;
            itr = itr_begin;
            etl::advance(itr, offset);
          }
        }
        else
        {
          difference_type distance_to_begin = etl::distance(itr_begin, itr);
          offset %= difference_type(size());

          if (distance_to_begin > -offset)
          {
            etl::advance(itr, offset);
          }
          else
          {
            offset += distance_to_begin;

            typename etl::reverse_iterator<TIterator> ritr(itr_end);
            etl::advance(ritr, -offset);
            itr = ritr.base();
          }
        }
      }

      return *this;
    }

    //***************************************************************************
    /// -= operator.
    //***************************************************************************
    circular_iterator& operator -=(typename etl::iterator_traits<TIterator>::difference_type offset)
    {
      return operator +=(-offset);
    }

    //***************************************************************************
    /// Assignment from span.
    //***************************************************************************
    circular_iterator& operator =(const etl::span<TIterator>& span_)
    {
      itr_begin = span_.begin();
      itr_end   = span_.end();
      itr       = itr_begin;

      return *this;
    }

    //***************************************************************************
    /// Assignment from circular_iterator.
    //***************************************************************************
    circular_iterator& operator =(circular_iterator other)
    {
      itr_begin = other.itr_begin;
      itr_end   = other.itr_end;
      itr       = other.itr;

      return *this;
    }

  private:

    TIterator itr_begin; ///< The underlying begin iterator.
    TIterator itr_end;   ///< The underlying end iterator.
    TIterator itr;       ///< The underlying iterator.
  };

  //*****************************************************************************
  /// + difference operator.
  //*****************************************************************************
  template <typename TIterator>
  etl::circular_iterator<TIterator> operator +(etl::circular_iterator<TIterator>& lhs,
                                               typename etl::iterator_traits<TIterator>::difference_type offset)
  {
    etl::circular_iterator<TIterator> result(lhs);
    result += offset;
    
    return result;
  }

  //*****************************************************************************
  /// - difference operator.
  //*****************************************************************************
  template <typename TIterator>
  etl::circular_iterator<TIterator> operator -(etl::circular_iterator<TIterator>& lhs,
    typename etl::iterator_traits<TIterator>::difference_type offset)
  {
    etl::circular_iterator<TIterator> result(lhs);
    result -= offset;

    return result;
  }

  ////*****************************************************************************
  ///// - circular_iterator operator.
  ////*****************************************************************************
  //template <typename TIterator>
  //typename etl::iterator_traits<TIterator>::difference_type operator -(etl::circular_iterator<TIterator>& lhs,
  //                                                                    etl::circular_iterator<TIterator>& rhs)
  //{
  //  return TIterator(lhs) - TIterator(rhs);
  //}

  //*****************************************************************************
  /// Equality operator. circular_iterator == circular_iterator.
  //*****************************************************************************
  template <typename TIterator>
  bool operator ==(const etl::circular_iterator<TIterator>& lhs,
                   const etl::circular_iterator<TIterator>& rhs)
  {
    return TIterator(lhs) == TIterator(rhs);
  }

  //*****************************************************************************
  /// Equality operator. circular_iterator == iterator.
  //*****************************************************************************
  template <typename TIterator>
  bool operator ==(const etl::circular_iterator<TIterator>& lhs,
                   TIterator rhs)
  {
    return TIterator(lhs) == rhs;
  }

  //*****************************************************************************
  /// Equality operator. iterator == circular_iterator.
  //*****************************************************************************
  template <typename TIterator>
  bool operator ==(TIterator lhs,
                   const etl::circular_iterator<TIterator>& rhs)
  {
    return lhs == TIterator(rhs);
  }


  //*****************************************************************************
  /// Inequality operator. circular_iterator == circular_iterator.
  //*****************************************************************************
  template <typename TIterator>
  bool operator !=(const etl::circular_iterator<TIterator>& lhs,
                   const etl::circular_iterator<TIterator>& rhs)
  {
    return !(lhs == rhs);
  }

  //*****************************************************************************
  /// Inequality operator. circular_iterator == iterator.
  //*****************************************************************************
  template <typename TIterator>
  bool operator !=(const etl::circular_iterator<TIterator>& lhs,
                   TIterator rhs)
  {
    return !(lhs == rhs);
  }

  //*****************************************************************************
  /// Inequality operator. iterator == circular_iterator.
  //*****************************************************************************
  template <typename TIterator>
  bool operator !=(TIterator& lhs,
                   const etl::circular_iterator<TIterator>& rhs)
  {
    return !(lhs == rhs);
  }
}

#endif
