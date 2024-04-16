///\file

/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl
https://www.etlcpp.com

Copyright(c) 2020 John Wellbelove

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

#ifndef ETL_SPAN_INCLUDED
#define ETL_SPAN_INCLUDED

#include "platform.h"
#include "iterator.h"
#include "algorithm.h"
#include "circular_iterator.h"
#include "nullptr.h"
#include "hash.h"
#include "type_traits.h"
#include "integral_limits.h"
#include "memory.h"
#include "array.h"
#include "byte.h"
#include "static_assert.h"

#include "private/dynamic_extent.h"

#if ETL_USING_CPP20 && ETL_USING_STL
  #include <span>
#endif

///\defgroup span span
///\ingroup containers

namespace etl
{
  //***************************************************************************
  /// Span - Fixed Extent
  //***************************************************************************
  template <typename T, size_t Extent = etl::dynamic_extent>
  class span
  {
  public:

    typedef T                                element_type;
    typedef typename etl::remove_cv<T>::type value_type;
    typedef size_t                           size_type;
    typedef T&                               reference;
    typedef const T&                         const_reference;
    typedef T*                               pointer;
    typedef const T*                         const_pointer;

    typedef T*                                     iterator;
    typedef ETL_OR_STD::reverse_iterator<iterator> reverse_iterator;

    typedef etl::circular_iterator<pointer>                                circular_iterator;
    typedef etl::circular_iterator<ETL_OR_STD::reverse_iterator<pointer> > reverse_circular_iterator;

    static ETL_CONSTANT size_t extent = Extent;

    //*************************************************************************
    /// Construct from iterators + size
    //*************************************************************************
    template <typename TIterator, typename TSize>
    ETL_CONSTEXPR explicit span(const TIterator begin_, const TSize /*size_*/) ETL_NOEXCEPT
      : pbegin(etl::addressof(*begin_))
    {
    }

    //*************************************************************************
    /// Construct from iterators
    //*************************************************************************
    template <typename TIterator>
    ETL_CONSTEXPR explicit span(const TIterator begin_, const TIterator /*end_*/)
      : pbegin(etl::addressof(*begin_))
    {
    }

    //*************************************************************************
    /// Construct from C array
    //*************************************************************************
    template<size_t Array_Size>
    ETL_CONSTEXPR span(element_type(&begin_)[Array_Size]) ETL_NOEXCEPT
      : pbegin(begin_)
    {
    }

#if ETL_USING_CPP11
    //*************************************************************************
    /// Construct from a container or other type that supports
    /// data() and size() member functions.
    //*************************************************************************
    template <typename TContainer, typename = typename etl::enable_if<!etl::is_pointer<etl::remove_reference_t<TContainer>>::value &&
                                                                      !etl::is_array<etl::remove_reference_t<TContainer>>::value&&
                                                                      etl::is_same<etl::remove_cv_t<T>, etl::remove_cv_t<typename etl::remove_reference_t<TContainer>::value_type>>::value, void>::type>
      ETL_CONSTEXPR span(TContainer&& a) ETL_NOEXCEPT
      : pbegin(a.data())
    {
    }
#else
    //*************************************************************************
    /// Construct from a container or other type that supports
    /// data() and size() member functions.
    //*************************************************************************
    template <typename TContainer>
    span(TContainer& a, typename etl::enable_if<!etl::is_pointer<typename etl::remove_reference<TContainer>::type>::value &&
                                                !etl::is_array<TContainer>::value &&
                                                etl::is_same<typename etl::remove_cv<T>::type, typename etl::remove_cv<typename etl::remove_reference<TContainer>::type::value_type>::type>::value, void>::type* = 0) ETL_NOEXCEPT
      : pbegin(a.data())
    {
    }

    //*************************************************************************
    /// Construct from a container or other type that supports
    /// data() and size() member functions.
    //*************************************************************************
    template <typename TContainer>
    ETL_CONSTEXPR span(const TContainer& a, typename etl::enable_if<!etl::is_pointer<typename etl::remove_reference<TContainer>::type>::value &&
                                                                    !etl::is_array<TContainer>::value&&
                                                                    etl::is_same<typename etl::remove_cv<T>::type, typename etl::remove_cv<typename etl::remove_reference<TContainer>::type::value_type>::type>::value, void>::type* = 0) ETL_NOEXCEPT
      : pbegin(a.data())
    {
    }
#endif

    //*************************************************************************
    /// Copy constructor
    //*************************************************************************
    ETL_CONSTEXPR span(const span& other) ETL_NOEXCEPT
      : pbegin(other.pbegin)
    {
    }

    //*************************************************************************
    /// Copy constructor
    //*************************************************************************
    template <typename U, size_t N>
    ETL_CONSTEXPR span(const etl::span<U, N>& other, typename etl::enable_if<(Extent == etl::dynamic_extent) || (N == etl::dynamic_extent) || (N == Extent), void>::type) ETL_NOEXCEPT
      : pbegin(other.data())
    {
    }

    //*************************************************************************
    /// Returns a reference to the first element.
    //*************************************************************************
    ETL_NODISCARD ETL_CONSTEXPR reference front() const ETL_NOEXCEPT
    {
      return *pbegin;
    }

    //*************************************************************************
    /// Returns a reference to the last element.
    //*************************************************************************
    ETL_NODISCARD ETL_CONSTEXPR reference back() const ETL_NOEXCEPT
    {
      return *((pbegin + Extent) - 1);
    }

    //*************************************************************************
    /// Returns a pointer to the first element of the internal storage.
    //*************************************************************************
    ETL_NODISCARD ETL_CONSTEXPR pointer data() const ETL_NOEXCEPT
    {
      return pbegin;
    }

    //*************************************************************************
    /// Returns an iterator to the beginning of the span.
    //*************************************************************************
    ETL_NODISCARD ETL_CONSTEXPR iterator begin() const ETL_NOEXCEPT
    {
      return pbegin;
    }

    //*************************************************************************
    /// Returns a circular iterator to the beginning of the span.
    //*************************************************************************
    ETL_NODISCARD ETL_CONSTEXPR circular_iterator begin_circular() const ETL_NOEXCEPT
    {
      return circular_iterator(begin(), end());
    }

    //*************************************************************************
    /// Returns an iterator to the end of the span.
    //*************************************************************************
    ETL_NODISCARD ETL_CONSTEXPR iterator end() const ETL_NOEXCEPT
    {
      return (pbegin + Extent);
    }

    //*************************************************************************
    // Returns an reverse iterator to the reverse beginning of the span.
    //*************************************************************************
    ETL_NODISCARD ETL_CONSTEXPR reverse_iterator rbegin() const ETL_NOEXCEPT
    {
      return reverse_iterator((pbegin + Extent));
    }

    //*************************************************************************
    /// Returns a reverse circular iterator to the end of the span.
    //*************************************************************************
    ETL_NODISCARD ETL_CONSTEXPR reverse_circular_iterator rbegin_circular() const ETL_NOEXCEPT
    {
      return reverse_circular_iterator(rbegin(), rend());
    }

    //*************************************************************************
    /// Returns a reverse iterator to the end of the span.
    //*************************************************************************
    ETL_NODISCARD ETL_CONSTEXPR reverse_iterator rend() const ETL_NOEXCEPT
    {
      return reverse_iterator(pbegin);
    }

    //*************************************************************************
    /// Returns <b>true</b> if the span size is zero.
    //*************************************************************************
    ETL_NODISCARD ETL_CONSTEXPR bool empty() const ETL_NOEXCEPT
    {
      return false;
    }

    //*************************************************************************
    /// Returns the size of the span.
    //*************************************************************************
    ETL_NODISCARD ETL_CONSTEXPR size_t size() const ETL_NOEXCEPT
    {
      return Extent;
    }

    //*************************************************************************
    /// Returns the size of the span in bytes.
    //*************************************************************************
    ETL_NODISCARD ETL_CONSTEXPR size_t size_bytes() const ETL_NOEXCEPT
    {
      return sizeof(element_type) * Extent;
    }

    //*************************************************************************
    /// Returns the maximum possible size of the span.
    //*************************************************************************
    ETL_NODISCARD ETL_CONSTEXPR size_t max_size() const ETL_NOEXCEPT
    {
      return size();
    }

    //*************************************************************************
    /// Assign from a span.
    //*************************************************************************
    ETL_CONSTEXPR14 span& operator =(const span& other) ETL_NOEXCEPT
    {
      pbegin = other.pbegin;
      return *this;
    }

    //*************************************************************************
    /// Returns a reference to the indexed value.
    //*************************************************************************
    ETL_CONSTEXPR reference operator[](const size_t i) const
    {
      return pbegin[i];
    }

    //*************************************************************************
    /// Obtains a span that is a view over the first COUNT elements of this span.
    //*************************************************************************
    template <size_t COUNT>
    ETL_NODISCARD ETL_CONSTEXPR etl::span<element_type, COUNT> first() const ETL_NOEXCEPT
    {
      // If Extent is static, check that original span contains at least COUNT elements
      ETL_STATIC_ASSERT((Extent != etl::dynamic_extent) ? COUNT <= Extent : true, "Original span does not contain COUNT elements");

      return etl::span<element_type, COUNT>(pbegin, pbegin + COUNT);
    }

    //*************************************************************************
    /// Obtains a span that is a view over the first count elements of this span.
    //*************************************************************************
    ETL_NODISCARD ETL_CONSTEXPR etl::span<element_type, etl::dynamic_extent> first(size_t count) const ETL_NOEXCEPT
    {
      return etl::span<element_type, etl::dynamic_extent>(pbegin, pbegin + count);
    }

    //*************************************************************************
    /// Obtains a span that is a view over the last COUNT elements of this span.
    //*************************************************************************
    template <size_t COUNT>
    ETL_NODISCARD ETL_CONSTEXPR etl::span<element_type, COUNT> last() const ETL_NOEXCEPT
    {
      // If Extent is static, check that original span contains at least COUNT elements
      ETL_STATIC_ASSERT((Extent != etl::dynamic_extent) ? COUNT <= Extent : true, "Original span does not contain COUNT elements");

      return etl::span<element_type, COUNT>(pbegin + Extent - COUNT, (pbegin + Extent));
    }

    //*************************************************************************
    /// Obtains a span that is a view over the last count elements of this span.
    //*************************************************************************
    ETL_NODISCARD ETL_CONSTEXPR etl::span<element_type, etl::dynamic_extent> last(size_t count) const ETL_NOEXCEPT
    {
      return etl::span<element_type, etl::dynamic_extent>((pbegin + Extent) - count, (pbegin + Extent));
    }

#if ETL_USING_CPP11
    //*************************************************************************
    /// Obtains a span that is a view from OFFSET over the next COUNT elements of this span.
    //*************************************************************************
    template <size_t OFFSET, size_t COUNT = etl::dynamic_extent>
    ETL_NODISCARD ETL_CONSTEXPR
    etl::span<element_type, COUNT != etl::dynamic_extent ? COUNT : Extent - OFFSET> subspan() const ETL_NOEXCEPT
    {
      // If Extent is static, check that OFFSET is within the original span
      ETL_STATIC_ASSERT((Extent != etl::dynamic_extent) ? OFFSET <= Extent : true, "OFFSET is not within the original span");

      // If count is also static, check that OFFSET + COUNT is within the original span
      ETL_STATIC_ASSERT((Extent != etl::dynamic_extent) && (COUNT != etl::dynamic_extent) ? COUNT <= (Extent - OFFSET) : true, "OFFSET + COUNT is not within the original span");
      
      return (COUNT == etl::dynamic_extent) ? etl::span<element_type, COUNT != etl::dynamic_extent ? COUNT : Extent - OFFSET>(pbegin + OFFSET, (pbegin + Extent))
                                            : etl::span<element_type, COUNT != etl::dynamic_extent ? COUNT : Extent - OFFSET>(pbegin + OFFSET, pbegin + OFFSET + COUNT);
    }
#else
    //*************************************************************************
    /// Obtains a span that is a view from OFFSET over the next COUNT elements of this span.
    //*************************************************************************
    template <size_t OFFSET, size_t COUNT>
    etl::span<element_type, COUNT != etl::dynamic_extent ? COUNT : Extent - OFFSET> subspan() const
    {
      // If Extent is static, check that OFFSET is within the original span
      ETL_STATIC_ASSERT((Extent != etl::dynamic_extent) ? OFFSET <= Extent : true, "OFFSET is not within the original span");

      // If count is also static, check that OFFSET + COUNT is within the original span
      ETL_STATIC_ASSERT((Extent != etl::dynamic_extent) && (COUNT != etl::dynamic_extent) ? COUNT <= (Extent - OFFSET) : true, "OFFSET + COUNT is not within the original span");
      
      if (COUNT == etl::dynamic_extent)
      {
        return etl::span<element_type, (COUNT != etl::dynamic_extent ? COUNT : Extent - OFFSET)>(pbegin + OFFSET, (pbegin + Extent));
      }
      else
      {
        return etl::span<element_type, COUNT != etl::dynamic_extent ? COUNT : Extent - OFFSET>(pbegin + OFFSET, pbegin + OFFSET + COUNT);
      }
    }
#endif

    //*************************************************************************
    /// Obtains a span that is a view from 'offset' over the next 'count' elements of this span.
    //*************************************************************************
    ETL_NODISCARD ETL_CONSTEXPR etl::span<element_type, etl::dynamic_extent> subspan(size_t offset, size_t count = etl::dynamic_extent) const ETL_NOEXCEPT
    {
      return (count == etl::dynamic_extent) ? etl::span<element_type, etl::dynamic_extent>(pbegin + offset, (pbegin + Extent))
                                            : etl::span<element_type, etl::dynamic_extent>(pbegin + offset, pbegin + offset + count);
    }

  private:

    pointer pbegin;
  };

  //***************************************************************************
  /// Span - Dynamic Extent
  //***************************************************************************
  template <typename T>
  class  span<T, etl::dynamic_extent>
  {
  public:

    typedef T        element_type;
    typedef typename etl::remove_cv<T>::type value_type;
    typedef size_t   size_type;
    typedef T&       reference;
    typedef const T& const_reference;
    typedef T*       pointer;
    typedef const T* const_pointer;
    
    typedef T*                                     iterator;
    typedef ETL_OR_STD::reverse_iterator<iterator> reverse_iterator;

    typedef etl::circular_iterator<pointer>                                circular_iterator;
    typedef etl::circular_iterator<ETL_OR_STD::reverse_iterator<pointer> > reverse_circular_iterator;

    static ETL_CONSTANT size_t extent = etl::dynamic_extent;

    //*************************************************************************
    /// Default constructor.
    //*************************************************************************
    ETL_CONSTEXPR span() ETL_NOEXCEPT
      : pbegin(ETL_NULLPTR)
      , pend(ETL_NULLPTR)
    {
    }

    //*************************************************************************
    /// Construct from pointer + size
    //*************************************************************************
    template <typename TIterator, typename TSize>
    ETL_CONSTEXPR span(const TIterator begin_, const TSize size_) ETL_NOEXCEPT
      : pbegin(etl::addressof(*begin_))
      , pend(etl::addressof(*begin_) + size_)
    {
    }

    //*************************************************************************
    /// Construct from iterators
    //*************************************************************************
    template <typename TIterator>
    ETL_CONSTEXPR span(const TIterator begin_, const TIterator end_)
      : pbegin(etl::addressof(*begin_))
      , pend(etl::addressof(*begin_) + etl::distance(begin_, end_))
    {
    }

    //*************************************************************************
    /// Construct from C array
    //*************************************************************************
    template<size_t Array_Size>
    ETL_CONSTEXPR span(element_type(&begin_)[Array_Size]) ETL_NOEXCEPT
      : pbegin(begin_)
      , pend(begin_ + Array_Size)
    {
    }

#if ETL_USING_CPP11
    //*************************************************************************
    /// Construct from a container or other type that supports
    /// data() and size() member functions.
    //*************************************************************************
    template <typename TContainer, typename = typename etl::enable_if<!etl::is_pointer<etl::remove_reference_t<TContainer>>::value &&
                                                                      !etl::is_array<etl::remove_reference_t<TContainer>>::value &&
                                                                      etl::is_same<etl::remove_cv_t<T>, etl::remove_cv_t<typename etl::remove_reference_t<TContainer>::value_type>>::value, void>::type>
      ETL_CONSTEXPR span(TContainer&& a) ETL_NOEXCEPT
      : pbegin(a.data())
      , pend(a.data() + a.size())
    {
    }
#else
    //*************************************************************************
    /// Construct from a container or other type that supports
    /// data() and size() member functions.
    //*************************************************************************
    template <typename TContainer>
    ETL_CONSTEXPR span(TContainer& a, typename etl::enable_if<!etl::is_pointer<typename etl::remove_reference<TContainer>::type>::value &&
                                                              !etl::is_array<TContainer>::value &&
                                                              etl::is_same<typename etl::remove_cv<T>::type, typename etl::remove_cv<typename etl::remove_reference<TContainer>::type::value_type>::type>::value, void>::type* = 0) ETL_NOEXCEPT
      : pbegin(a.data())
      , pend(a.data() + a.size())
    {
    }

    //*************************************************************************
    /// Construct from a container or other type that supports
    /// data() and size() member functions.
    //*************************************************************************
    template <typename TContainer>
    ETL_CONSTEXPR span(const TContainer& a, typename etl::enable_if<!etl::is_pointer<typename etl::remove_reference<TContainer>::type>::value &&
                                                                    !etl::is_array<TContainer>::value &&
                                                                    etl::is_same<typename etl::remove_cv<T>::type, typename etl::remove_cv<typename etl::remove_reference<TContainer>::type::value_type>::type>::value, void>::type* = 0) ETL_NOEXCEPT
      : pbegin(a.data())
      , pend(a.data() + a.size())
    {
    }
#endif

    //*************************************************************************
    /// Copy constructor
    //*************************************************************************
    ETL_CONSTEXPR span(const span& other) ETL_NOEXCEPT
      : pbegin(other.pbegin)
      , pend(other.pend)
    {
    }

    //*************************************************************************
    /// Copy constructor
    //*************************************************************************
    template <typename U, size_t N>
    ETL_CONSTEXPR span(const etl::span<U, N>& other) ETL_NOEXCEPT
      : pbegin(other.data())
      , pend(other.data() + other.size())
    {
    }

    //*************************************************************************
    /// Returns a reference to the first element.
    //*************************************************************************
    ETL_NODISCARD ETL_CONSTEXPR reference front() const ETL_NOEXCEPT
    {
      return *pbegin;
    }

    //*************************************************************************
    /// Returns a reference to the last element.
    //*************************************************************************
    ETL_NODISCARD ETL_CONSTEXPR reference back() const ETL_NOEXCEPT
    {
      return *(pend - 1);
    }

    //*************************************************************************
    /// Returns a pointer to the first element of the internal storage.
    //*************************************************************************
    ETL_NODISCARD ETL_CONSTEXPR pointer data() const ETL_NOEXCEPT
    {
      return pbegin;
    }

    //*************************************************************************
    /// Returns an iterator to the beginning of the span.
    //*************************************************************************
    ETL_NODISCARD ETL_CONSTEXPR iterator begin() const ETL_NOEXCEPT
    {
      return pbegin;
    }

    //*************************************************************************
    /// Returns a circular iterator to the beginning of the span.
    //*************************************************************************
    ETL_NODISCARD ETL_CONSTEXPR circular_iterator begin_circular() const ETL_NOEXCEPT
    {
      return circular_iterator(begin(), end());
    }

    //*************************************************************************
    /// Returns an iterator to the end of the span.
    //*************************************************************************
    ETL_NODISCARD ETL_CONSTEXPR iterator end() const ETL_NOEXCEPT
    {
      return pend;
    }

    //*************************************************************************
    // Returns an reverse iterator to the reverse beginning of the span.
    //*************************************************************************
    ETL_NODISCARD ETL_CONSTEXPR reverse_iterator rbegin() const ETL_NOEXCEPT
    {
      return reverse_iterator(pend);
    }

    //*************************************************************************
    /// Returns a reverse circular iterator to the end of the span.
    //*************************************************************************
    ETL_NODISCARD ETL_CONSTEXPR reverse_circular_iterator rbegin_circular() const ETL_NOEXCEPT
    {
      return reverse_circular_iterator(rbegin(), rend());
    }

    //*************************************************************************
    /// Returns a reverse iterator to the end of the span.
    //*************************************************************************
    ETL_NODISCARD ETL_CONSTEXPR reverse_iterator rend() const ETL_NOEXCEPT
    {
      return reverse_iterator(pbegin);
    }

    //*************************************************************************
    /// Returns <b>true</b> if the span size is zero.
    //*************************************************************************
    ETL_NODISCARD ETL_CONSTEXPR bool empty() const ETL_NOEXCEPT
    {
      return (pbegin == pend);
    }

    //*************************************************************************
    /// Returns the size of the span.
    //*************************************************************************
    ETL_NODISCARD ETL_CONSTEXPR size_t size() const ETL_NOEXCEPT
    {
      return (pend - pbegin);
    }

    //*************************************************************************
    /// Returns the size of the span in bytes.
    //*************************************************************************
    ETL_NODISCARD ETL_CONSTEXPR size_t size_bytes() const ETL_NOEXCEPT
    {
      return sizeof(element_type) * (pend - pbegin);
    }

    //*************************************************************************
    /// Returns the maximum possible size of the span.
    //*************************************************************************
    ETL_NODISCARD ETL_CONSTEXPR size_t max_size() const ETL_NOEXCEPT
    {
      return size();
    }

    //*************************************************************************
    /// Assign from a span.
    //*************************************************************************
    ETL_CONSTEXPR14 span& operator =(const span& other) ETL_NOEXCEPT
    {
      pbegin = other.pbegin;
      pend = other.pend;
      return *this;
    }

    //*************************************************************************
    /// Returns a reference to the indexed value.
    //*************************************************************************
    ETL_CONSTEXPR reference operator[](const size_t i) const
    {
      return pbegin[i];
    }

    //*************************************************************************
    /// Obtains a span that is a view over the first COUNT elements of this span.
    //*************************************************************************
    template <size_t COUNT>
    ETL_NODISCARD ETL_CONSTEXPR etl::span<element_type, COUNT> first() const ETL_NOEXCEPT
    {
      return etl::span<element_type, COUNT>(pbegin, pbegin + COUNT);
    }

    //*************************************************************************
    /// Obtains a span that is a view over the first count elements of this span.
    //*************************************************************************
    ETL_NODISCARD ETL_CONSTEXPR etl::span<element_type, etl::dynamic_extent> first(size_t count) const ETL_NOEXCEPT
    {
      return etl::span<element_type, etl::dynamic_extent>(pbegin, pbegin + count);
    }

    //*************************************************************************
    /// Obtains a span that is a view over the last COUNT elements of this span.
    //*************************************************************************
    template <size_t COUNT>
    ETL_NODISCARD ETL_CONSTEXPR etl::span<element_type, COUNT> last() const ETL_NOEXCEPT
    {
      return etl::span<element_type, COUNT>(pend - COUNT, pend);
    }

    //*************************************************************************
    /// Obtains a span that is a view over the last count elements of this span.
    //*************************************************************************
    ETL_NODISCARD ETL_CONSTEXPR etl::span<element_type, etl::dynamic_extent> last(size_t count) const ETL_NOEXCEPT
    {
      return etl::span<element_type, etl::dynamic_extent>(pend - count, pend);
    }

#if ETL_USING_CPP11
    //*************************************************************************
    /// Obtains a span that is a view from OFFSET over the next COUNT elements of this span.
    //*************************************************************************
    template <size_t OFFSET, size_t COUNT = etl::dynamic_extent>
    ETL_NODISCARD ETL_CONSTEXPR
    etl::span<element_type, COUNT != etl::dynamic_extent ? COUNT : etl::dynamic_extent> subspan() const ETL_NOEXCEPT
    {
      return (COUNT == etl::dynamic_extent) ? etl::span<element_type, COUNT != etl::dynamic_extent ? COUNT : etl::dynamic_extent>(pbegin + OFFSET, pend)
                                            : etl::span<element_type, COUNT != etl::dynamic_extent ? COUNT : etl::dynamic_extent>(pbegin + OFFSET, pbegin + OFFSET + COUNT);
    }
#else
    //*************************************************************************
    /// Obtains a span that is a view from OFFSET over the next COUNT elements of this span.
    //*************************************************************************
    template <size_t OFFSET, size_t COUNT>
    etl::span<element_type, COUNT != etl::dynamic_extent ? COUNT : etl::dynamic_extent> subspan() const
    {
      if (COUNT == etl::dynamic_extent)
      {
        return etl::span<element_type, COUNT != etl::dynamic_extent ? COUNT : etl::dynamic_extent>(pbegin + OFFSET, pend);
      }
      else
      {
        return etl::span<element_type, COUNT != etl::dynamic_extent ? COUNT : etl::dynamic_extent>(pbegin + OFFSET, pbegin + OFFSET + COUNT);
      }
    }
#endif

    //*************************************************************************
    /// Obtains a span that is a view from 'offset' over the next 'count' elements of this span.
    //*************************************************************************
    ETL_NODISCARD ETL_CONSTEXPR14 etl::span<element_type, etl::dynamic_extent> subspan(size_t offset, size_t count = etl::dynamic_extent) const ETL_NOEXCEPT
    {
      return (count == etl::dynamic_extent) ? etl::span<element_type, etl::dynamic_extent>(pbegin + offset, pend)
                                            : etl::span<element_type, etl::dynamic_extent>(pbegin + offset, pbegin + offset + count);
    }

  private:

    pointer pbegin;
    pointer pend;
  };

  template <typename T, size_t Extent>
  ETL_CONSTANT size_t span<T, Extent>::extent;

  template <typename T>
  ETL_CONSTANT size_t span<T, etl::dynamic_extent>::extent;

  //*************************************************************************
  /// Compare two spans for equality.
  //*************************************************************************
  template <typename T1, size_t N1, typename T2, size_t N2>
  ETL_NODISCARD 
  ETL_CONSTEXPR
  typename etl::enable_if<etl::is_same<typename etl::remove_cv<T1>::type, typename etl::remove_cv<T2>::type>::value, bool>::type
    operator ==(const etl::span<T1, N1>& lhs, const etl::span<T2, N2>& rhs) ETL_NOEXCEPT
  {
    return (lhs.begin() == rhs.begin()) && (lhs.size() == rhs.size());
  }

  //*************************************************************************
  /// Compare two spans for inequality.
  //*************************************************************************
  template <typename T1, size_t N1, typename T2, size_t N2>
  ETL_NODISCARD
  ETL_CONSTEXPR
  bool operator !=(const etl::span<T1, N1>& lhs, const etl::span<T2, N2>& rhs) ETL_NOEXCEPT
  {
    return !(lhs == rhs);
  }

  //*************************************************************************
  /// Equality function.
  /// Performs a comparison of the range values.
  /// Returns <b>true</b> if one of the following are <b>true</b>
  /// 1. Both spans are empty.
  /// 2. They both point to the same range of data.
  /// 3. The values in the two ranges are equal.
  //*************************************************************************
  template <typename T1, size_t N1, typename T2, size_t N2>
  typename etl::enable_if<etl::is_same<typename etl::remove_cv<T1>::type, typename etl::remove_cv<T2>::type>::value, bool>::type
    equal(const etl::span<T1, N1>& lhs, const etl::span<T2, N2>& rhs)
  {
    return (lhs.empty() && rhs.empty()) ||
           ((lhs.begin() == rhs.begin()) && (lhs.size() == rhs.size())) ||
           etl::equal(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
  }

  //*************************************************************************
  /// Template deduction guides.
  //*************************************************************************
#if ETL_USING_CPP17
  template <typename TIterator>
  span(const TIterator begin_, const TIterator end_)
    ->span<etl::remove_pointer_t<TIterator>, etl::dynamic_extent>;

  template <typename TIterator, typename TSize>
  span(const TIterator begin_, const TSize size_)
    ->span<etl::remove_pointer_t<TIterator>, etl::dynamic_extent>;

  template <typename T, size_t N>
  span(T(&)[N])
    -> span<T, N>;

  template <typename T, size_t N>
  span(etl::array<T, N>&)
    -> span<T, N>;

  template <typename T, size_t N>
  span(const etl::array<T, N>&)
    -> span<const T, N>;

#if ETL_USING_STL && ETL_USING_CPP11
  template <typename T, size_t N>
  span(std::array<T, N>&)
    ->span<T, N>;

  template <typename T, size_t N>
  span(const std::array<T, N>&)
    ->span<const T, N>;
#endif
#endif 

  //*************************************************************************
  /// Hash function.
  //*************************************************************************
#if ETL_USING_8BIT_TYPES
  template <typename T, size_t Extent>
  struct hash<etl::span<T, Extent> >
  {
    size_t operator()(const etl::span<T>& view) const
    {
      return etl::private_hash::generic_hash<size_t>(reinterpret_cast<const uint8_t*>(view.data()),
                                                     reinterpret_cast<const uint8_t*>(view.data() + view.size()));
    }
  };
#endif

  //*************************************************************************
  /// Obtains a view to the byte representation of the elements of the span s.
  //*************************************************************************
  template <class T, size_t N>
  span<const byte, (N == etl::dynamic_extent) ? (etl::dynamic_extent) : (N * sizeof(T))> 
    as_bytes(span<T, N> s) ETL_NOEXCEPT
  {
    return span<const byte, (N == etl::dynamic_extent) ? (etl::dynamic_extent) : (N * sizeof(T))>(reinterpret_cast<byte*>(s.data()), s.size_bytes());
  }

  //*************************************************************************
  /// Obtains a view to the byte representation of the elements of the span s.
  //*************************************************************************
  template <class T, size_t N>
  span<byte, (N == etl::dynamic_extent) ? (etl::dynamic_extent) : (N * sizeof(T))> 
    as_writable_bytes(span<T, N> s) ETL_NOEXCEPT
  {
    ETL_STATIC_ASSERT(!etl::is_const<T>::value, "span<T> must be of non-const type");
    return span<byte, (N == etl::dynamic_extent) ? (etl::dynamic_extent) : (N * sizeof(T))>(reinterpret_cast<byte*>(s.data()), s.size_bytes());
  }
}

#endif
