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

#ifndef ETL_CIRCULAR_SPAN_INCLUDED
#define ETL_CIRCULAR_SPAN_INCLUDED

#include "platform.h"
#include "iterator.h"
#include "hash.h"
#include "array.h"
#include "circular_iterator.h"
#include "span.h"

#include "private/dynamic_extent.h"

#if ETL_USING_STL && ETL_USING_CPP11
  #include <array>
#endif

///\defgroup circular_span circular_span
///\ingroup containers

namespace etl
{
  //***************************************************************************
  /// Circular Span - Fixed Extent
  //***************************************************************************
  template <typename T, size_t Extent = etl::dynamic_extent>
  class circular_span
  {
  public:

    typedef T                                element_type;
    typedef typename etl::remove_cv<T>::type value_type;
    typedef size_t                           size_type;
    typedef T&                               reference;
    typedef const T&                         const_reference;
    typedef T*                               pointer;
    typedef const T*                         const_pointer;

    typedef etl::circular_iterator<pointer> iterator;
    typedef etl::circular_iterator<ETL_OR_STD::reverse_iterator<pointer> > reverse_iterator;

    typedef typename etl::span<T>::iterator         span_iterator;
    typedef typename etl::span<T>::reverse_iterator span_reverse_iterator;

    static ETL_CONSTANT size_t extent = Extent;

    //*************************************************************************
    /// Default constructor.
    //*************************************************************************
    ETL_CONSTEXPR circular_span() ETL_NOEXCEPT
      : linear_span()
    {
    }

    //*************************************************************************
    /// Construct from pointer + size
    //*************************************************************************
    template <typename TIterator, typename TSize>
    ETL_CONSTEXPR circular_span(const TIterator begin_, const TSize size_) ETL_NOEXCEPT
      : linear_span(begin_, size_)
    {
    }

    //*************************************************************************
    /// Construct from iterators
    //*************************************************************************
    template <typename TIterator>
    ETL_CONSTEXPR circular_span(const TIterator begin_, const TIterator end_)
      : linear_span(begin_, end_)
    {
    }

    //*************************************************************************
    /// Construct from C array
    //*************************************************************************
    template<size_t ARRAY_SIZE>
    ETL_CONSTEXPR circular_span(element_type(&begin_)[ARRAY_SIZE]) ETL_NOEXCEPT
      : linear_span(begin_)
    {
    }

    //*************************************************************************
    /// Construct from etl::array.
    //*************************************************************************
    template <typename U, size_t N>
    ETL_CONSTEXPR circular_span(etl::array<U, N>& a) ETL_NOEXCEPT
      : linear_span(a)
    {
    }

    //*************************************************************************
    /// Construct from etl::array.
    //*************************************************************************
    template <typename U, size_t N>
    ETL_CONSTEXPR circular_span(const etl::array<U, N>& a) ETL_NOEXCEPT
      : linear_span(a)
    {
    }

#if ETL_USING_STL && ETL_USING_CPP11
    //*************************************************************************
    /// Construct from std::array.
    //*************************************************************************
    template <typename U, size_t N>
    ETL_CONSTEXPR circular_span(std::array<U, N>& a) ETL_NOEXCEPT
      : linear_span(a)
    {
    }

    //*************************************************************************
    /// Construct from std::array.
    //*************************************************************************
    template <typename U, size_t N>
    ETL_CONSTEXPR circular_span(const std::array<U, N>& a) ETL_NOEXCEPT
      : linear_span(a)
    {
    }
#endif

#if ETL_USING_CPP11
    //*************************************************************************
    /// Construct from a container or other type that supports
    /// data() and size() member functions.
    //*************************************************************************
    template <typename TContainer>
      ETL_CONSTEXPR circular_span(TContainer&& a) ETL_NOEXCEPT
      : linear_span(a)
    {
    }
#else
    //*************************************************************************
    /// Construct from a container or other type that supports
    /// data() and size() member functions.
    //*************************************************************************
    template <typename TContainer>
    circular_span(TContainer& a) ETL_NOEXCEPT
      : linear_span(a)
    {
    }

    //*************************************************************************
    /// Construct from a container or other type that supports
    /// data() and size() member functions.
    //*************************************************************************
    template <typename TContainer>
    ETL_CONSTEXPR circular_span(const TContainer& a) ETL_NOEXCEPT
      : linear_span(a)
    {
    }
#endif

    //*************************************************************************
    /// Copy constructor
    //*************************************************************************
    ETL_CONSTEXPR circular_span(const circular_span& other) ETL_NOEXCEPT
      : linear_span(other.linear_span)
    {
    }

    //*************************************************************************
    /// Returns a reference to the first element.
    //*************************************************************************
    ETL_NODISCARD ETL_CONSTEXPR reference front() const ETL_NOEXCEPT
    {
      return linear_span.front();
    }

    //*************************************************************************
    /// Returns a reference to the last element.
    //*************************************************************************
    ETL_NODISCARD ETL_CONSTEXPR reference back() const ETL_NOEXCEPT
    {
      return linear_span.back();
    }

    //*************************************************************************
    /// Returns a pointer to the first element of the internal storage.
    //*************************************************************************
    ETL_NODISCARD ETL_CONSTEXPR pointer data() const ETL_NOEXCEPT
    {
      return linear_span.data();
    }

    //*************************************************************************
    /// Returns an iterator to the beginning of the circular_span.
    //*************************************************************************
    ETL_NODISCARD ETL_CONSTEXPR iterator begin() const ETL_NOEXCEPT
    {
      return iterator(linear_span.begin(), linear_span.end(), linear_span.begin());
    }

    //*************************************************************************
    // Returns an reverse iterator to the reverse beginning of the circular_span.
    //*************************************************************************
    ETL_NODISCARD ETL_CONSTEXPR reverse_iterator rbegin() const ETL_NOEXCEPT
    {
      return reverse_iterator(linear_span.rbegin(), linear_span.rend());
    }

    //*************************************************************************
    /// Returns an iterator to the beginning of the circular_span.
    //*************************************************************************
    ETL_NODISCARD ETL_CONSTEXPR span_iterator span_begin() const ETL_NOEXCEPT
    {
      return linear_span.begin();
    }

    //*************************************************************************
    /// Returns a reverse iterator to the reverse beginning of the circular_span.
    //*************************************************************************
    ETL_NODISCARD ETL_CONSTEXPR span_reverse_iterator span_rbegin() const ETL_NOEXCEPT
    {
      return linear_span.rbegin();
    }

    //*************************************************************************
    /// Returns an iterator to the end of the circular_span.
    //*************************************************************************
    ETL_NODISCARD ETL_CONSTEXPR span_iterator span_end() const ETL_NOEXCEPT
    {
      return linear_span.end();
    }

    //*************************************************************************
    /// Returns a reverse iterator to the reverse end of the circular_span.
    //*************************************************************************
    ETL_NODISCARD ETL_CONSTEXPR span_reverse_iterator span_rend() const ETL_NOEXCEPT
    {
      return linear_span.rend();
    }

    //*************************************************************************
    /// Returns <b>true</b> if the circular_span size is zero.
    //*************************************************************************
    ETL_NODISCARD ETL_CONSTEXPR bool empty() const ETL_NOEXCEPT
    {
      return linear_span.empty();
    }

    //*************************************************************************
    /// Returns the size of the circular_span.
    //*************************************************************************
    ETL_NODISCARD ETL_CONSTEXPR size_t size() const ETL_NOEXCEPT
    {
      return linear_span.size();
    }

    //*************************************************************************
    /// Returns the size of the circular_span in bytes.
    //*************************************************************************
    ETL_NODISCARD ETL_CONSTEXPR size_t size_bytes() const ETL_NOEXCEPT
    {
      return linear_span.size_bytes();
    }

    //*************************************************************************
    /// Returns the maximum possible size of the circular_span.
    //*************************************************************************
    ETL_NODISCARD ETL_CONSTEXPR size_t max_size() const ETL_NOEXCEPT
    {
      return linear_span.max_size();
    }

    //*************************************************************************
    /// Assign from a circular_span.
    //*************************************************************************
    ETL_CONSTEXPR14 circular_span& operator =(const circular_span& other) ETL_NOEXCEPT
    {
      linear_span = other.linear_span;

      return *this;
    }

    //*************************************************************************
    /// Returns a reference to the indexed value.
    //*************************************************************************
    ETL_CONSTEXPR reference operator[](const size_t i) const
    {
      return linear_span.operator[](i);
    }

    //*************************************************************************
    /// Obtains a circular_span that is a view over the first Count elements of this circular_span.
    //*************************************************************************
    template <size_t Count>
    ETL_NODISCARD ETL_CONSTEXPR etl::circular_span<element_type, Count> first() const ETL_NOEXCEPT
    {
      return etl::circular_span<element_type, Count>(linear_span.data(), linear_span.data() + Count);
    }

    //*************************************************************************
    /// Obtains a circular_span that is a view over the first count elements of this circular_span.
    //*************************************************************************
    ETL_NODISCARD ETL_CONSTEXPR etl::circular_span<element_type, etl::dynamic_extent> first(size_t count) const ETL_NOEXCEPT
    {
      return etl::circular_span<element_type, etl::dynamic_extent>(linear_span.data(), linear_span.data() + count);
    }

    //*************************************************************************
    /// Obtains a circular_span that is a view over the last Count elements of this circular_span.
    //*************************************************************************
    template <size_t Count>
    ETL_NODISCARD ETL_CONSTEXPR etl::circular_span<element_type, Count> last() const ETL_NOEXCEPT
    {
      return etl::circular_span<element_type, Count>(linear_span.data() + Extent - Count, (linear_span.data() + Extent));
    }

    //*************************************************************************
    /// Obtains a circular_span that is a view over the last count elements of this circular_span.
    //*************************************************************************
    ETL_NODISCARD ETL_CONSTEXPR etl::circular_span<element_type, etl::dynamic_extent> last(size_t count) const ETL_NOEXCEPT
    {
      return etl::circular_span<element_type, etl::dynamic_extent>((linear_span.data() + Extent) - count, (linear_span.data() + Extent));
    }

#if ETL_USING_CPP11
    //*************************************************************************
    /// Obtains a circular_span that is a view from Offset over the next Count elements of this circular_span.
    //*************************************************************************
    template <const size_t Offset, size_t Count = etl::dynamic_extent>
    ETL_NODISCARD ETL_CONSTEXPR
    etl::circular_span<element_type, Count != etl::dynamic_extent ? Count : Extent - Offset> subspan() const ETL_NOEXCEPT
    {
      return (Count == etl::dynamic_extent) ? etl::circular_span<element_type, Count != etl::dynamic_extent ? Count : Extent - Offset>(linear_span.data() + Offset, (linear_span.data() + Extent))
                                            : etl::circular_span<element_type, Count != etl::dynamic_extent ? Count : Extent - Offset>(linear_span.data() + Offset, linear_span.data() + Offset + Count);
    }
#else
    //*************************************************************************
    /// Obtains a circular_span that is a view from Offset over the next Count elements of this circular_span.
    //*************************************************************************
    template <const size_t Offset, size_t Count>
    etl::circular_span<element_type, Count != etl::dynamic_extent ? Count : Extent - Offset> subspan() const
    {
      if (Count == etl::dynamic_extent)
      {
        return etl::circular_span<element_type, (Count != etl::dynamic_extent ? Count : Extent - Offset)>(linear_span.data() + Offset, (linear_span.data() + Extent));
      }
      else
      {
        return etl::circular_span<element_type, Count != etl::dynamic_extent ? Count : Extent - Offset>(linear_span.data() + Offset, linear_span.data() + Offset + Count);
      }
    }
#endif

    //*************************************************************************
    /// Obtains a circular span that is a view from 'offset' over the next 'count' elements of this span.
    //*************************************************************************
    ETL_NODISCARD ETL_CONSTEXPR etl::circular_span<element_type, etl::dynamic_extent> subspan(size_t offset, size_t count = etl::dynamic_extent) const ETL_NOEXCEPT
    {
      return (count == etl::dynamic_extent) ? etl::circular_span<element_type, etl::dynamic_extent>(linear_span.begin() + offset, (linear_span.begin() + Extent))
                                            : etl::circular_span<element_type, etl::dynamic_extent>(linear_span.begin() + offset, linear_span.begin() + offset + count);
    }

  private:

    etl::span<T, Extent> linear_span;
  };

  //***************************************************************************
  /// Circular Span - Dynamic Extent
  //***************************************************************************
  template <typename T>
  class circular_span<T, etl::dynamic_extent>
  {
  public:

    typedef T                                element_type;
    typedef typename etl::remove_cv<T>::type value_type;
    typedef size_t                           size_type;
    typedef T&                               reference;
    typedef const T&                         const_reference;
    typedef T*                               pointer;
    typedef const T*                         const_pointer;

    typedef etl::circular_iterator<pointer> iterator;
    typedef etl::circular_iterator<ETL_OR_STD::reverse_iterator<pointer> > reverse_iterator;

    typedef typename etl::span<T>::iterator         span_iterator;
    typedef typename etl::span<T>::reverse_iterator span_reverse_iterator;

    static ETL_CONSTANT size_t extent = etl::dynamic_extent;

    //*************************************************************************
    /// Default constructor.
    //*************************************************************************
    ETL_CONSTEXPR circular_span() ETL_NOEXCEPT
      : linear_span()
    {
    }

    //*************************************************************************
    /// Construct from pointer + size
    //*************************************************************************
    template <typename TIterator, typename TSize>
    ETL_CONSTEXPR circular_span(const TIterator begin_, const TSize size_) ETL_NOEXCEPT
      : linear_span(begin_, size_)
    {
    }

    //*************************************************************************
    /// Construct from iterators
    //*************************************************************************
    template <typename TIterator>
    ETL_CONSTEXPR circular_span(const TIterator begin_, const TIterator end_)
      : linear_span(begin_, end_)
    {
    }

    //*************************************************************************
    /// Construct from C array
    //*************************************************************************
    template<size_t ARRAY_SIZE>
    ETL_CONSTEXPR circular_span(element_type(&begin_)[ARRAY_SIZE]) ETL_NOEXCEPT
      : linear_span(begin_)
    {
    }

    //*************************************************************************
    /// Construct from etl::array.
    //*************************************************************************
    template <typename U, size_t N>
    ETL_CONSTEXPR circular_span(etl::array<U, N>& a) ETL_NOEXCEPT
      : linear_span(a)
    {
    }

    //*************************************************************************
    /// Construct from etl::array.
    //*************************************************************************
    template <typename U, size_t N>
    ETL_CONSTEXPR circular_span(const etl::array<U, N>& a) ETL_NOEXCEPT
      : linear_span(a)
    {
    }

#if ETL_USING_STL && ETL_USING_CPP11
    //*************************************************************************
    /// Construct from std::array.
    //*************************************************************************
    template <typename U, size_t N>
    ETL_CONSTEXPR circular_span(std::array<U, N>& a) ETL_NOEXCEPT
      : linear_span(a)
    {
    }

    //*************************************************************************
    /// Construct from std::array.
    //*************************************************************************
    template <typename U, size_t N>
    ETL_CONSTEXPR circular_span(const std::array<U, N>& a) ETL_NOEXCEPT
      : linear_span(a)
    {
    }
#endif

#if ETL_USING_CPP11
    //*************************************************************************
    /// Construct from a container or other type that supports
    /// data() and size() member functions.
    //*************************************************************************
    template <typename TContainer>
    ETL_CONSTEXPR circular_span(TContainer&& a) ETL_NOEXCEPT
      : linear_span(a)
    {
    }
#else
    //*************************************************************************
    /// Construct from a container or other type that supports
    /// data() and size() member functions.
    //*************************************************************************
    template <typename TContainer>
    circular_span(TContainer& a) ETL_NOEXCEPT
      : linear_span(a)
    {
    }

    //*************************************************************************
    /// Construct from a container or other type that supports
    /// data() and size() member functions.
    //*************************************************************************
    template <typename TContainer>
    ETL_CONSTEXPR circular_span(const TContainer& a) ETL_NOEXCEPT
      : linear_span(a)
    {
    }
#endif

    //*************************************************************************
    /// Copy constructor
    //*************************************************************************
    ETL_CONSTEXPR circular_span(const circular_span& other) ETL_NOEXCEPT
      : linear_span(other.linear_span)
    {
    }

    //*************************************************************************
    /// Returns a reference to the first element.
    //*************************************************************************
    ETL_NODISCARD ETL_CONSTEXPR reference front() const ETL_NOEXCEPT
    {
      return linear_span.front();
    }

    //*************************************************************************
    /// Returns a reference to the last element.
    //*************************************************************************
    ETL_NODISCARD ETL_CONSTEXPR reference back() const ETL_NOEXCEPT
    {
      return linear_span.back();
    }

    //*************************************************************************
    /// Returns a pointer to the first element of the internal storage.
    //*************************************************************************
    ETL_NODISCARD ETL_CONSTEXPR pointer data() const ETL_NOEXCEPT
    {
      return linear_span.data();
    }

    //*************************************************************************
    /// Returns an iterator to the beginning of the circular_span.
    //*************************************************************************
    ETL_NODISCARD ETL_CONSTEXPR iterator begin() const ETL_NOEXCEPT
    {
      return iterator(linear_span.begin(), linear_span.end(), linear_span.begin());
    }

    //*************************************************************************
    // Returns an reverse iterator to the reverse beginning of the circular_span.
    //*************************************************************************
    ETL_NODISCARD ETL_CONSTEXPR reverse_iterator rbegin() const ETL_NOEXCEPT
    {
      return reverse_iterator(linear_span.end(), linear_span.begin(), linear_span.end());
    }

    //*************************************************************************
    /// Returns an iterator to the beginning of the circular_span.
    //*************************************************************************
    ETL_NODISCARD ETL_CONSTEXPR span_iterator span_begin() const ETL_NOEXCEPT
    {
      return linear_span.begin();
    }

    //*************************************************************************
    /// Returns a reverse iterator to the reverse beginning of the circular_span.
    //*************************************************************************
    ETL_NODISCARD ETL_CONSTEXPR span_reverse_iterator span_rbegin() const ETL_NOEXCEPT
    {
      return linear_span.rbegin();
    }

    //*************************************************************************
    /// Returns an iterator to the end of the circular_span.
    //*************************************************************************
    ETL_NODISCARD ETL_CONSTEXPR span_iterator span_end() const ETL_NOEXCEPT
    {
      return linear_span.end();
    }

    //*************************************************************************
    /// Returns a reverse iterator to the reverse end of the circular_span.
    //*************************************************************************
    ETL_NODISCARD ETL_CONSTEXPR span_reverse_iterator span_rend() const ETL_NOEXCEPT
    {
      return linear_span.rend();
    }

    //*************************************************************************
    /// Returns <b>true</b> if the circular_span size is zero.
    //*************************************************************************
    ETL_NODISCARD ETL_CONSTEXPR bool empty() const ETL_NOEXCEPT
    {
      return linear_span.empty();
    }

    //*************************************************************************
    /// Returns the size of the circular_span.
    //*************************************************************************
    ETL_NODISCARD ETL_CONSTEXPR size_t size() const ETL_NOEXCEPT
    {
      return linear_span.size();
    }

    //*************************************************************************
    /// Returns the size of the circular_span in bytes.
    //*************************************************************************
    ETL_NODISCARD ETL_CONSTEXPR size_t size_bytes() const ETL_NOEXCEPT
    {
      return linear_span.size_bytes();
    }

    //*************************************************************************
    /// Returns the maximum possible size of the circular_span.
    //*************************************************************************
    ETL_NODISCARD ETL_CONSTEXPR size_t max_size() const ETL_NOEXCEPT
    {
      return linear_span.max_size();
    }

    //*************************************************************************
    /// Assign from a circular_span.
    //*************************************************************************
    ETL_CONSTEXPR14 circular_span& operator =(const circular_span& other) ETL_NOEXCEPT
    {
      linear_span = other.linear_span;

      return *this;
    }

    //*************************************************************************
    /// Returns a reference to the indexed value.
    //*************************************************************************
    ETL_CONSTEXPR reference operator[](const size_t i) const
    {
      return linear_span.operator[](i);
    }

    //*************************************************************************
    /// Obtains a span that is a view over the first COUNT elements of this span.
    //*************************************************************************
    template <size_t Count>
    ETL_NODISCARD ETL_CONSTEXPR etl::circular_span<element_type, Count> first() const ETL_NOEXCEPT
    {
      return etl::circular_span<element_type, Count>(linear_span.begin(), linear_span.begin() + Count);
    }

    //*************************************************************************
    /// Obtains a span that is a view over the first count elements of this span.
    //*************************************************************************
    ETL_NODISCARD ETL_CONSTEXPR etl::circular_span<element_type, etl::dynamic_extent> first(size_t count) const ETL_NOEXCEPT
    {
      return etl::circular_span<element_type, etl::dynamic_extent>(linear_span.begin(), linear_span.begin() + count);
    }

    //*************************************************************************
    /// Obtains a span that is a view over the last COUNT elements of this span.
    //*************************************************************************
    template <size_t Count>
    ETL_NODISCARD ETL_CONSTEXPR etl::circular_span<element_type, Count> last() const ETL_NOEXCEPT
    {
      return etl::circular_span<element_type, Count>(linear_span.end() - Count, linear_span.end());
    }

    //*************************************************************************
    /// Obtains a span that is a view over the last count elements of this span.
    //*************************************************************************
    ETL_NODISCARD ETL_CONSTEXPR etl::circular_span<element_type, etl::dynamic_extent> last(size_t count) const ETL_NOEXCEPT
    {
      return etl::circular_span<element_type, etl::dynamic_extent>(linear_span.end() - count, linear_span.end());
    }

#if ETL_USING_CPP11
    //*************************************************************************
    /// Obtains a circular span that is a view from Offset over the next COUNT elements of this span.
    //*************************************************************************
    template <const size_t Offset, size_t Count = etl::dynamic_extent>
    ETL_NODISCARD ETL_CONSTEXPR
      etl::circular_span<element_type, Count != etl::dynamic_extent ? Count : etl::dynamic_extent> subspan() const ETL_NOEXCEPT
    {
      return (Count == etl::dynamic_extent) ? etl::circular_span<element_type, Count != etl::dynamic_extent ? Count : etl::dynamic_extent>(linear_span.begin() + Offset, linear_span.end())
                                            : etl::circular_span<element_type, Count != etl::dynamic_extent ? Count : etl::dynamic_extent>(linear_span.begin() + Offset, linear_span.begin() + Offset + Count);
    }
#else
    //*************************************************************************
    /// Obtains a circular span that is a view from Offset over the next Count elements of this span.
    //*************************************************************************
    template <const size_t Offset, size_t Count>
    etl::circular_span<element_type, Count != etl::dynamic_extent ? Count : etl::dynamic_extent> subspan() const
    {
      if (Count == etl::dynamic_extent)
      {
        return etl::circular_span<element_type, Count != etl::dynamic_extent ? Count : etl::dynamic_extent>(linear_span.begin() + Offset, linear_span.end());
      }
      else
      {
        return etl::circular_span<element_type, Count != etl::dynamic_extent ? Count : etl::dynamic_extent>(linear_span.begin() + Offset, linear_span.begin() + Offset + Count);      }
    }
#endif

    //*************************************************************************
    /// Obtains a circular span that is a view from 'offset' over the next 'count' elements of this span.
    //*************************************************************************
    ETL_NODISCARD ETL_CONSTEXPR14 etl::circular_span<element_type, etl::dynamic_extent> subspan(size_t offset, size_t count = etl::dynamic_extent) const ETL_NOEXCEPT
    {
      return (count == etl::dynamic_extent) ? etl::circular_span<element_type, etl::dynamic_extent>(linear_span.begin() + offset, linear_span.end())
                                            : etl::circular_span<element_type, etl::dynamic_extent>(linear_span.begin() + offset, linear_span.begin() + offset + count);
    }

  private:

    etl::span<T, etl::dynamic_extent> linear_span;
  };

  //*************************************************************************
  /// Template deduction guides.
  //*************************************************************************
#if ETL_USING_CPP17
  template <typename TIterator>
  circular_span(const TIterator begin_, const TIterator end_)
    ->circular_span<etl::remove_pointer_t<TIterator>, etl::dynamic_extent>;

  template <typename TIterator, typename TSize>
  circular_span(const TIterator begin_, const TSize size_)
    ->circular_span<etl::remove_pointer_t<TIterator>, etl::dynamic_extent>;

  template <typename T, size_t N>
  circular_span(T(&)[N])
    -> circular_span<T, N>;

  template <typename T, size_t N>
  circular_span(etl::array<T, N>&)
    -> circular_span<T, N>;

  template <typename T, size_t N>
  circular_span(const etl::array<T, N>&)
    -> circular_span<const T, N>;

#if ETL_USING_STL
  template <typename T, size_t N>
  circular_span(std::array<T, N>&)
    ->circular_span<T, N>;

  template <typename T, size_t N>
  circular_span(const std::array<T, N>&)
    ->circular_span<const T, N>;
#endif
#endif 

  //*************************************************************************
  /// Hash function.
  //*************************************************************************
#if ETL_USING_8BIT_TYPES
  template <typename T, size_t Extent>
  struct hash<etl::circular_span<T, Extent> >
  {
    size_t operator()(const etl::circular_span<T>& view) const
    {
      return etl::private_hash::generic_hash<size_t>(reinterpret_cast<const uint8_t*>(&view[0]),
        reinterpret_cast<const uint8_t*>(&view[view.size()]));
    }
  };
#endif
}

#endif
