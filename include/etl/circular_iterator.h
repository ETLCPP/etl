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
#include "static_assert.h"

///\defgroup iterator Iterator types

namespace etl
{
  namespace private_circular_iterator
  {
    //***************************************************************************
    /// Common circular iterator implementation.
    //***************************************************************************
    template <typename TIterator>
    class circular_iterator_common
      : public etl::iterator<typename etl::iterator_traits<TIterator>::iterator_category, typename etl::iterator_traits<TIterator>::value_type>
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
      ETL_CONSTEXPR14 circular_iterator_common()
        : itr_begin(TIterator())
        , itr_end(TIterator())
        , itr(TIterator())
      {
      }

      //***************************************************************************
      /// Construct from iterators.
      //***************************************************************************
      ETL_CONSTEXPR14 circular_iterator_common(TIterator itr_begin_, TIterator itr_end_, TIterator start_)
        : itr_begin(itr_begin_)
        , itr_end(itr_end_)
        , itr(start_)
      {
      }

      //***************************************************************************
      /// Copy constructor
      //***************************************************************************
      ETL_CONSTEXPR14 circular_iterator_common(const circular_iterator_common& other)
        : itr_begin(other.itr_begin)
        , itr_end(other.itr_end)
        , itr(other.itr)
      {
      }

      //***************************************************************************
      /// Assignment
      //***************************************************************************
      ETL_CONSTEXPR14 circular_iterator_common& operator =(const circular_iterator_common& other)
      {
        itr_begin = other.itr_begin;
        itr_end = other.itr_end;
        itr = other.itr;

        return *this;
      }

      //***************************************************************************
      /// Beginning of the range.
      //***************************************************************************
      ETL_CONSTEXPR14 TIterator begin() const
      {
        return itr_begin;
      }

      //***************************************************************************
      /// End of the range.
      //***************************************************************************
      ETL_CONSTEXPR14 TIterator end() const
      {
        return itr_end;
      }

      //***************************************************************************
      /// How long is the range?
      //***************************************************************************
      ETL_CONSTEXPR14 size_t size() const
      {
        return etl::distance(itr_begin, itr_end);
      }

      //***************************************************************************
      /// Is there nothing to iterate over?
      //***************************************************************************
      ETL_CONSTEXPR14 bool empty() const
      {
        return (itr_begin == itr_end);
      }

      //***************************************************************************
      /// Dereference operator.
      //***************************************************************************
      ETL_CONSTEXPR14 value_type operator *()
      {
        return *itr;
      }

      //***************************************************************************
      /// Dereference operator.
      //***************************************************************************
      ETL_CONSTEXPR14 const value_type operator *() const
      {
        return *itr;
      }

      //***************************************************************************
      /// -> operator.
      //***************************************************************************
      ETL_CONSTEXPR14 TIterator operator ->()
      {
        return itr;
      }

      //***************************************************************************
      /// -> operator.
      //***************************************************************************
      ETL_CONSTEXPR14 const TIterator operator ->() const
      {
        return itr;
      }

      //***************************************************************************
      /// Conversion operator.
      //***************************************************************************
      ETL_CONSTEXPR14 operator TIterator() const
      {
        return itr;
      }

      //***************************************************************************
      /// Conversion to base iterator type.
      //***************************************************************************
      ETL_CONSTEXPR14 TIterator current() const
      {
        return itr;
      }

    protected:

      TIterator itr_begin; ///< The underlying begin iterator.
      TIterator itr_end;   ///< The underlying end iterator.
      TIterator itr;       ///< The underlying iterator.
    };

    //***************************************************************************
    /// A circular iterator class.
    /// This iterator can be given a pair of iterator values, which will loop if the start or end of the range is reached.
    ///\ingroup iterator
    //***************************************************************************
    template <typename TIterator, typename TTag = typename etl::iterator_traits<TIterator>::iterator_category>
    class circular_iterator_impl
    {
      ETL_STATIC_ASSERT((etl::is_same<TTag, ETL_OR_STD::input_iterator_tag>::value_type), "input_iterator_catagory is not supported by circular_iterator");
      ETL_STATIC_ASSERT((etl::is_same<TTag, ETL_OR_STD::output_iterator_tag>::value_type), "output_iterator_catagory is not supported by circular_iterator");
    };

    //***************************************************************************
    /// A circular iterator class.
    /// Specialisation for forward iterators.
    ///\ingroup iterator
    /// //***************************************************************************
    template <typename TIterator>
    class circular_iterator_impl<TIterator, ETL_OR_STD::forward_iterator_tag>
      : public circular_iterator_common<TIterator>
    {
    private:

      typedef circular_iterator_common<TIterator> common_t;

    public:

      using common_t::operator=;

      typedef typename common_t::value_type        value_type;
      typedef typename common_t::difference_type   difference_type;
      typedef typename common_t::pointer           pointer;
      typedef typename common_t::reference         reference;
      typedef typename common_t::iterator_category iterator_category;

      //***************************************************************************
      /// Default constructor.
      //***************************************************************************
      ETL_CONSTEXPR14 circular_iterator_impl()
        : common_t()
      {
      }

      //***************************************************************************
      /// Construct from iterators.
      //***************************************************************************
      ETL_CONSTEXPR14 circular_iterator_impl(TIterator itr_begin_, TIterator itr_end_)
        : common_t(itr_begin_, itr_end_, itr_begin_)
      {
      }

      //***************************************************************************
      /// Construct from start + iterators.
      //***************************************************************************
      ETL_CONSTEXPR14 circular_iterator_impl(TIterator itr_begin_, TIterator itr_end_, TIterator start_)
        : common_t(itr_begin_, itr_end_, start_)
      {
      }

      //***************************************************************************
      /// Copy constructor
      //***************************************************************************
      ETL_CONSTEXPR14 circular_iterator_impl(const circular_iterator_impl& other)
        : common_t(other)
      {
      }

      //***************************************************************************
      /// Assignment
      //***************************************************************************
      ETL_CONSTEXPR14 circular_iterator_impl& operator =(const circular_iterator_impl& other)
      {
        common_t::operator=(other);

        return *this;
      }

      //***************************************************************************
      /// Increment.
      //***************************************************************************
      ETL_CONSTEXPR14 circular_iterator_impl& operator ++()
      {
        if (++this->itr == this->itr_end)
        {
          this->itr = this->itr_begin;
        }

        return *this;
      }

      //***************************************************************************
      /// Increment.
      //***************************************************************************
      ETL_CONSTEXPR14 circular_iterator_impl operator ++(int)
      {
        circular_iterator_impl original(*this);

        ++(*this);

        return (original);
      }
    };

    //***************************************************************************
    /// A circular iterator class.
    /// Specialisation for random access iterators.
    ///\ingroup iterator
    /// //***************************************************************************
    template <typename TIterator>
    class circular_iterator_impl<TIterator, ETL_OR_STD::bidirectional_iterator_tag>
      : public circular_iterator_common<TIterator>
    {
    private:

      typedef circular_iterator_common<TIterator> common_t;

    public:

      using common_t::operator=;

      typedef typename common_t::value_type        value_type;
      typedef typename common_t::difference_type   difference_type;
      typedef typename common_t::pointer           pointer;
      typedef typename common_t::reference         reference;
      typedef typename common_t::iterator_category iterator_category;

      //***************************************************************************
      /// Default constructor.
      //***************************************************************************
      ETL_CONSTEXPR14 circular_iterator_impl()
        : common_t()
      {
      }

      //***************************************************************************
      /// Construct from iterators.
      //***************************************************************************
      ETL_CONSTEXPR14 circular_iterator_impl(TIterator itr_begin_, TIterator itr_end_)
        : common_t(itr_begin_, itr_end_, itr_begin_)
      {
      }

      //***************************************************************************
      /// Construct from start + iterators.
      //***************************************************************************
      ETL_CONSTEXPR14 circular_iterator_impl(TIterator itr_begin_, TIterator itr_end_, TIterator start_)
        : common_t(itr_begin_, itr_end_, start_)
      {
      }

      //***************************************************************************
      /// Copy constructor
      //***************************************************************************
      ETL_CONSTEXPR14 circular_iterator_impl(const circular_iterator_impl& other)
        : common_t(other)
      {
      }

      //***************************************************************************
      /// Assignment
      //***************************************************************************
      ETL_CONSTEXPR14 circular_iterator_impl& operator =(const circular_iterator_impl& other)
      {
        common_t::operator=(other);

        return *this;
      }

      //***************************************************************************
      /// Increment.
      //***************************************************************************
      ETL_CONSTEXPR14 circular_iterator_impl& operator ++()
      {
        if (++this->itr == this->itr_end)
        {
          this->itr = this->itr_begin;
        }

        return *this;
      }

      //***************************************************************************
      /// Increment.
      //***************************************************************************
      ETL_CONSTEXPR14 circular_iterator_impl operator ++(int)
      {
        circular_iterator_impl original(*this);

        ++(*this);

        return (original);
      }

      //***************************************************************************
      /// Decrement.
      //***************************************************************************
      ETL_CONSTEXPR14 circular_iterator_impl& operator --()
      {
        if (this->itr == this->itr_begin)
        {
          typename etl::reverse_iterator<TIterator> ritr(this->itr_end);
          ++ritr;
          this->itr = ritr.base();
        }
        else
        {
          --this->itr;
        }

        return *this;
      }

      //***************************************************************************
      /// Decrement.
      //***************************************************************************
      ETL_CONSTEXPR14 circular_iterator_impl operator --(int)
      {
        circular_iterator_impl original(*this);

        --(*this);

        return (original);
      }
    };

    //***************************************************************************
    /// A circular iterator class.
    /// Specialisation for random access iterators.
    ///\ingroup iterator
    //***************************************************************************
    template <typename TIterator>
    class circular_iterator_impl<TIterator, ETL_OR_STD::random_access_iterator_tag>
      : public etl::private_circular_iterator::circular_iterator_common<TIterator>
    {
    private:

      typedef etl::private_circular_iterator::circular_iterator_common<TIterator> common_t;

    public:

      using common_t::operator=;

      typedef typename common_t::value_type        value_type;
      typedef typename common_t::difference_type   difference_type;
      typedef typename common_t::pointer           pointer;
      typedef typename common_t::reference         reference;
      typedef typename common_t::iterator_category iterator_category;

      //***************************************************************************
      /// Default constructor.
      //***************************************************************************
      ETL_CONSTEXPR14 circular_iterator_impl()
        : common_t()
      {
      }

      //***************************************************************************
      /// Construct from iterators.
      //***************************************************************************
      ETL_CONSTEXPR14 circular_iterator_impl(TIterator itr_begin_, TIterator itr_end_)
        : common_t(itr_begin_, itr_end_, itr_begin_)
      {
      }

      //***************************************************************************
      /// Construct from start + iterators.
      //***************************************************************************
      ETL_CONSTEXPR14 circular_iterator_impl(TIterator itr_begin_, TIterator itr_end_, TIterator start_)
        : common_t(itr_begin_, itr_end_, start_)
      {
      }

      //***************************************************************************
      /// Copy constructor
      //***************************************************************************
      ETL_CONSTEXPR14 circular_iterator_impl(const circular_iterator_impl& other)
        : common_t(other)
      {
      }

      //***************************************************************************
      /// Assignment
      //***************************************************************************
      ETL_CONSTEXPR14 circular_iterator_impl& operator =(const circular_iterator_impl& other)
      {
        common_t::operator=(other);

        return *this;
      }

      //***************************************************************************
      /// Increment.
      //***************************************************************************
      ETL_CONSTEXPR14 circular_iterator_impl& operator ++()
      {
        if (++this->itr == this->itr_end)
        {
          this->itr = this->itr_begin;
        }

        return *this;
      }

      //***************************************************************************
      /// Increment.
      //***************************************************************************
      ETL_CONSTEXPR14 circular_iterator_impl operator ++(int)
      {
        circular_iterator_impl original(*this);

        ++(*this);

        return (original);
      }

      //***************************************************************************
      /// Decrement.
      //***************************************************************************
      ETL_CONSTEXPR14 circular_iterator_impl& operator --()
      {
        if (this->itr == this->itr_begin)
        {
          typename etl::reverse_iterator<TIterator> ritr(this->itr_end);
          ++ritr;
          this->itr = ritr.base();
        }
        else
        {
          --this->itr;
        }

        return *this;
      }

      //***************************************************************************
      /// Decrement.
      //***************************************************************************
      ETL_CONSTEXPR14 circular_iterator_impl operator --(int)
      {
        circular_iterator_impl original(*this);

        --(*this);

        return (original);
      }

      //***************************************************************************
      /// += operator.
      //***************************************************************************
      ETL_CONSTEXPR14 circular_iterator_impl& operator +=(difference_type offset)
      {
        const difference_type length = difference_type(this->size());
        offset %= length;

        if (offset != 0)
        {
          const difference_type distance_from_begin = etl::distance(this->itr_begin, this->itr);
          const difference_type distance_to_end = etl::distance(this->itr, this->itr_end);

          if (offset > 0)
          {
            if (distance_to_end > offset)
            {
              offset = distance_from_begin + offset;
            }
            else
            {
              offset = offset - distance_to_end;
            }
          }
          else
          {
            offset = -offset;

            if (distance_from_begin >= offset)
            {
              offset = distance_from_begin - offset;
            }
            else
            {
              offset = offset - distance_from_begin;
              offset = length - offset;
            }
          }

          this->itr = this->itr_begin + offset;
        }

        return *this;
      }

      //***************************************************************************
      /// -= operator.
      //***************************************************************************
      ETL_CONSTEXPR14 circular_iterator_impl& operator -=(typename etl::iterator_traits<TIterator>::difference_type offset)
      {
        return operator +=(-offset);
      }
    };
  }

  //***************************************************************************
  /// A circular iterator class.
  /// This iterator can be given a pair of iterator values, which will loop if the start or end of the range is reached.
  ///\ingroup iterator
  //**************************************************************************
  template <typename TIterator>
  class circular_iterator ETL_FINAL
    : public etl::private_circular_iterator::circular_iterator_impl<TIterator, typename etl::iterator_traits<TIterator>::iterator_category>
  {
  private:

    typedef typename etl::private_circular_iterator::circular_iterator_impl<TIterator, typename etl::iterator_traits<TIterator>::iterator_category> impl_t;

  public:

    using impl_t::operator=;

    typedef typename impl_t::value_type        value_type;
    typedef typename impl_t::difference_type   difference_type;
    typedef typename impl_t::pointer           pointer;
    typedef typename impl_t::reference         reference;
    typedef typename impl_t::iterator_category iterator_category;

    //***************************************************************************
    /// Default constructor.
    //***************************************************************************
    ETL_CONSTEXPR14 circular_iterator()
      : impl_t()
    {
    }

    //***************************************************************************
    /// Construct from iterators.
    //***************************************************************************
    ETL_CONSTEXPR14 circular_iterator(TIterator itr_begin_, TIterator itr_end_)
      : impl_t(itr_begin_, itr_end_, itr_begin_)
    {
    }

    //***************************************************************************
    /// Construct from start + iterators.
    //***************************************************************************
    ETL_CONSTEXPR14 circular_iterator(TIterator itr_begin_, TIterator itr_end_, TIterator start_)
      : impl_t(itr_begin_, itr_end_, start_)
    {
    }

    //***************************************************************************
    /// Copy constructor
    //***************************************************************************
    ETL_CONSTEXPR14 circular_iterator(const circular_iterator& other)
      : impl_t(other)
    {
    }

    //***************************************************************************
    /// Assignment
    //***************************************************************************
    ETL_CONSTEXPR14 circular_iterator& operator =(const circular_iterator& other)
    {
      impl_t::operator=(other);

      return *this;
    }
  };

  //*****************************************************************************
  /// + addition operator.
  //*****************************************************************************
  template <typename TIterator>
  ETL_CONSTEXPR14 etl::circular_iterator<TIterator> operator +(etl::circular_iterator<TIterator>& lhs,
                                                               typename etl::iterator_traits<TIterator>::difference_type offset)
  {
    etl::circular_iterator<TIterator> result(lhs);
    result += offset;
    
    return result;
  }

  //*****************************************************************************
  /// - offset operator.
  //*****************************************************************************
  template <typename TIterator>
  ETL_CONSTEXPR14 etl::circular_iterator<TIterator> operator -(etl::circular_iterator<TIterator>& lhs,
                                                               typename etl::iterator_traits<TIterator>::difference_type offset)
  {
    etl::circular_iterator<TIterator> result(lhs);
    result -= offset;

    return result;
  }

  //*****************************************************************************
  /// - circular_iterator difference operator.
  //*****************************************************************************
  template <typename TIterator>
  ETL_CONSTEXPR14 typename etl::iterator_traits<TIterator>::difference_type operator -(etl::circular_iterator<TIterator>& lhs,
                                                                                       etl::circular_iterator<TIterator>& rhs)
  {
    return TIterator(lhs) - TIterator(rhs);
  }

  //*****************************************************************************
  /// Equality operator. circular_iterator == circular_iterator.
  //*****************************************************************************
  template <typename TIterator>
  ETL_CONSTEXPR14 bool operator ==(const etl::circular_iterator<TIterator>& lhs,
                                   const etl::circular_iterator<TIterator>& rhs)
  {
    return TIterator(lhs) == TIterator(rhs);
  }

  //*****************************************************************************
  /// Equality operator. circular_iterator == iterator.
  //*****************************************************************************
  template <typename TIterator>
  ETL_CONSTEXPR14 bool operator ==(const etl::circular_iterator<TIterator>& lhs,
                                   TIterator rhs)
  {
    return TIterator(lhs) == rhs;
  }

  //*****************************************************************************
  /// Equality operator. iterator == circular_iterator.
  //*****************************************************************************
  template <typename TIterator>
  ETL_CONSTEXPR14 bool operator ==(TIterator lhs,
                                   const etl::circular_iterator<TIterator>& rhs)
  {
    return lhs == TIterator(rhs);
  }


  //*****************************************************************************
  /// Inequality operator. circular_iterator == circular_iterator.
  //*****************************************************************************
  template <typename TIterator>
  ETL_CONSTEXPR14 bool operator !=(const etl::circular_iterator<TIterator>& lhs,
                                   const etl::circular_iterator<TIterator>& rhs)
  {
    return !(lhs == rhs);
  }

  //*****************************************************************************
  /// Inequality operator. circular_iterator == iterator.
  //*****************************************************************************
  template <typename TIterator>
  ETL_CONSTEXPR14 bool operator !=(const etl::circular_iterator<TIterator>& lhs,
                                   TIterator rhs)
  {
    return !(lhs == rhs);
  }

  //*****************************************************************************
  /// Inequality operator. iterator == circular_iterator.
  //*****************************************************************************
  template <typename TIterator>
  ETL_CONSTEXPR14 bool operator !=(TIterator& lhs,
                                   const etl::circular_iterator<TIterator>& rhs)
  {
    return !(lhs == rhs);
  }
}

#endif
