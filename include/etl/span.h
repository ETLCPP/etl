///\file

/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl
https://www.etlcpp.com

Copyright(c) 2020 jwellbelove

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
#include "nullptr.h"
#include "hash.h"
#include "type_traits.h"
#include "integral_limits.h"
#include "memory.h"
#include "array.h"

#if ETL_CPP11_SUPPORTED && ETL_USING_STL
  #include <array>
#endif

///\defgroup span span
///\ingroup containers

namespace etl
{
  static ETL_CONSTANT size_t dynamic_extent = etl::integral_limits<size_t>::max;

  //***************************************************************************
  /// Span
  //***************************************************************************
  template <typename T, size_t EXTENT = etl::dynamic_extent>
  class span
  {
  public:

    typedef T                                            element_type;
    typedef typename etl::remove_cv<T>::type             value_type;
    typedef size_t                                       size_type;
    typedef T& reference;
    typedef const T& const_reference;
    typedef T* pointer;
    typedef const T* const_pointer;
    typedef T* iterator;
    typedef const T* const_iterator;
    typedef ETL_OR_STD::reverse_iterator<iterator>       reverse_iterator;
    typedef ETL_OR_STD::reverse_iterator<const_iterator> const_reverse_iterator;

    static ETL_CONSTANT size_t extent = EXTENT;

    //*************************************************************************
    /// Default constructor.
    //*************************************************************************
    ETL_CONSTEXPR span() ETL_NOEXCEPT
      : mbegin(ETL_NULLPTR)
      , mend(ETL_NULLPTR)
    {
    }

    //*************************************************************************
    /// Construct from pointer + size
    //*************************************************************************
    template <typename TIterator, typename TSize>
    ETL_CONSTEXPR span(const TIterator begin_, const TSize size_) ETL_NOEXCEPT
      : mbegin(etl::addressof(*begin_))
      , mend(etl::addressof(*begin_) + size_)
    {
    }

    //*************************************************************************
    /// Construct from iterators
    //*************************************************************************
    template <typename TIterator>
    ETL_CONSTEXPR span(const TIterator begin_, const TIterator end_)
      : mbegin(etl::addressof(*begin_))
      , mend(etl::addressof(*begin_) + etl::distance(begin_, end_))
    {
    }

    //*************************************************************************
    /// Construct from C array
    //*************************************************************************
    template<size_t ARRAY_SIZE>
    ETL_CONSTEXPR span(element_type(&begin_)[ARRAY_SIZE]) ETL_NOEXCEPT
      : mbegin(begin_)
      , mend(begin_ + ARRAY_SIZE)
    {
    }

#if ETL_CPP11_SUPPORTED
    //*************************************************************************
    /// Construct from etl::array.
    //*************************************************************************
    template <typename U, size_t N, typename = typename etl::enable_if<(N == EXTENT) && etl::is_same<etl::remove_cv_t<T>, etl::remove_cv_t<U>>::value, void>::type>
    ETL_CONSTEXPR span(etl::array<U, N>& a) ETL_NOEXCEPT
      : mbegin(a.data())
      , mend(a.data() + a.size())
    {
    }

    //*************************************************************************
    /// Construct from etl::array.
    //*************************************************************************
    template <typename U, size_t N, typename = typename etl::enable_if<(N == EXTENT) && etl::is_same<etl::remove_cv_t<T>, etl::remove_cv_t<U>>::value, void>::type>
    ETL_CONSTEXPR span(const etl::array<U, N>& a) ETL_NOEXCEPT
      : mbegin(a.data())
      , mend(a.data() + a.size())
    {
    }
#else
    //*************************************************************************
    /// Construct from etl::array.
    //*************************************************************************
    template <typename U, size_t N>
    ETL_CONSTEXPR span(etl::array<U, N>& a, typename etl::enable_if<(N == EXTENT) && etl::is_same<typename etl::remove_cv<T>::type, typename etl::remove_cv<U>::type>::value, void>::type) ETL_NOEXCEPT
      : mbegin(a.data())
      , mend(a.data() + a.size())
    {
    }

    //*************************************************************************
    /// Construct from etl::array.
    //*************************************************************************
    template <typename U, size_t N>
    ETL_CONSTEXPR span(const etl::array<U, N>& a, typename etl::enable_if<(N == EXTENT) && etl::is_same<typename etl::remove_cv<T>::type, typename etl::remove_cv<U>::type>::value, void>::type) ETL_NOEXCEPT
      : mbegin(a.data())
      , mend(a.data() + a.size())
    {
    }
#endif

#if ETL_CPP11_SUPPORTED && ETL_USING_STL
    //*************************************************************************
    /// Construct from std::array.
    //*************************************************************************
    template <typename U, size_t N, typename = typename etl::enable_if<(N == EXTENT) && etl::is_same<etl::remove_cv_t<T>, etl::remove_cv_t<U>>::value, void>::type>
    ETL_CONSTEXPR span(std::array<U, N>& a) ETL_NOEXCEPT
      : mbegin(a.data())
      , mend(a.data() + a.size())
    {
    }

    //*************************************************************************
    /// Construct from std::array.
    //*************************************************************************
    template <typename U, size_t N, typename = typename etl::enable_if<(N == EXTENT) && etl::is_same<etl::remove_cv_t<T>, etl::remove_cv_t<U>>::value, void>::type>
    ETL_CONSTEXPR span(const std::array<U, N>& a) ETL_NOEXCEPT
      : mbegin(a.data())
      , mend(a.data() + a.size())
    {
    }
#endif

    //*************************************************************************
    /// Copy constructor
    //*************************************************************************
    ETL_CONSTEXPR span(const span& other) ETL_NOEXCEPT
      : mbegin(other.mbegin)
      , mend(other.mend)
    {
    }

    //*************************************************************************
    /// Returns a reference to the first element.
    //*************************************************************************
    ETL_CONSTEXPR reference front() const ETL_NOEXCEPT
    {
      return *mbegin;
    }

    //*************************************************************************
    /// Returns a reference to the last element.
    //*************************************************************************
    ETL_CONSTEXPR reference back() const ETL_NOEXCEPT
    {
      return *(mend - 1);
    }

    //*************************************************************************
    /// Returns a pointer to the first element of the internal storage.
    //*************************************************************************
    ETL_CONSTEXPR pointer data() const ETL_NOEXCEPT
    {
      return mbegin;
    }

    //*************************************************************************
    /// Returns an iterator to the beginning of the span.
    //*************************************************************************
    ETL_CONSTEXPR iterator begin() const ETL_NOEXCEPT
    {
      return mbegin;
    }

    //*************************************************************************
    /// Returns an iterator to the end of the span.
    //*************************************************************************
    ETL_CONSTEXPR iterator end() const ETL_NOEXCEPT
    {
      return mend;
    }

    //*************************************************************************
    // Returns an reverse iterator to the reverse beginning of the span.
    //*************************************************************************
    ETL_CONSTEXPR reverse_iterator rbegin() const ETL_NOEXCEPT
    {
      return reverse_iterator(mend);
    }

    //*************************************************************************
    /// Returns a reverse iterator to the end of the span.
    //*************************************************************************
    ETL_CONSTEXPR reverse_iterator rend() const ETL_NOEXCEPT
    {
      return reverse_iterator(mbegin);
    }

    //*************************************************************************
    /// Returns <b>true</b> if the span size is zero.
    //*************************************************************************
    ETL_CONSTEXPR bool empty() const ETL_NOEXCEPT
    {
      return (mbegin == mend);
    }

    //*************************************************************************
    /// Returns the size of the span.
    //*************************************************************************
    ETL_CONSTEXPR size_t size() const ETL_NOEXCEPT
    {
      return (mend - mbegin);
    }

    //*************************************************************************
    /// Returns the size of the span in bytes.
    //*************************************************************************
    ETL_CONSTEXPR size_t size_bytes() const ETL_NOEXCEPT
    {
      return sizeof(element_type) * (mend - mbegin);
    }

    //*************************************************************************
    /// Returns the maximum possible size of the span.
    //*************************************************************************
    ETL_CONSTEXPR size_t max_size() const ETL_NOEXCEPT
    {
      return size();
    }

    //*************************************************************************
    /// Assign from a span.
    //*************************************************************************
    ETL_CONSTEXPR14 span& operator =(const span& other) ETL_NOEXCEPT
    {
      mbegin = other.mbegin;
      mend = other.mend;
      return *this;
    }

    //*************************************************************************
    /// Returns a reference to the indexed value.
    //*************************************************************************
    ETL_CONSTEXPR reference operator[](const size_t i) const
    {
      return mbegin[i];
    }

    //*************************************************************************
    /// Obtains a span that is a view over the first COUNT elements of this span.
    //*************************************************************************
    template <size_t COUNT>
    ETL_CONSTEXPR etl::span<element_type, COUNT> first() const
    {
      return etl::span<element_type, COUNT>(mbegin, mbegin + COUNT);
    }

    //*************************************************************************
    /// Obtains a span that is a view over the first count elements of this span.
    //*************************************************************************
    ETL_CONSTEXPR etl::span<element_type, etl::dynamic_extent> first(size_t count) const
    {
      return etl::span<element_type, etl::dynamic_extent>(mbegin, mbegin + count);
    }

    //*************************************************************************
    /// Obtains a span that is a view over the last COUNT elements of this span.
    //*************************************************************************
    template <size_t COUNT>
    ETL_CONSTEXPR etl::span<element_type, COUNT> last() const
    {
      return etl::span<element_type, COUNT>(mend - COUNT, mend);
    }

    //*************************************************************************
    /// Obtains a span that is a view over the last count elements of this span.
    //*************************************************************************
    ETL_CONSTEXPR etl::span<element_type, etl::dynamic_extent> last(size_t count) const
    {
      return etl::span<element_type, etl::dynamic_extent>(mend - count, mend);
    }

#if ETL_CPP11_SUPPORTED
    //*************************************************************************
    /// Obtains a span that is a view from OFFSET over the next COUNT elements of this span.
    //*************************************************************************
    template <const size_t OFFSET, size_t COUNT = etl::dynamic_extent>
    ETL_CONSTEXPR 
    etl::span<element_type, (COUNT != etl::dynamic_extent ? COUNT : (EXTENT != etl::dynamic_extent ? EXTENT - OFFSET : etl::dynamic_extent))> subspan() const
    {
      if (COUNT == etl::dynamic_extent)
      {
        return etl::span<element_type, (COUNT != etl::dynamic_extent ? COUNT : (EXTENT != etl::dynamic_extent ? EXTENT - OFFSET : etl::dynamic_extent))>(mbegin + OFFSET, mend);
      }
      else
      {
        return etl::span<element_type, (COUNT != etl::dynamic_extent ? COUNT : (EXTENT != etl::dynamic_extent ? EXTENT - OFFSET : etl::dynamic_extent))>(mbegin + OFFSET, mbegin + OFFSET + COUNT);
      }
    }
#else
    //*************************************************************************
    /// Obtains a span that is a view from OFFSET over the next COUNT elements of this span.
    //*************************************************************************
    template <const size_t OFFSET, size_t COUNT>
    ETL_CONSTEXPR 
      etl::span<element_type, (COUNT != etl::dynamic_extent ? COUNT : (EXTENT != etl::dynamic_extent ? EXTENT - OFFSET : etl::dynamic_extent))> subspan() const
    {
      if (COUNT == etl::dynamic_extent)
      {
        return etl::span<element_type, (COUNT != etl::dynamic_extent ? COUNT : (EXTENT != etl::dynamic_extent ? EXTENT - OFFSET : etl::dynamic_extent))>(mbegin + OFFSET, mend);
      }
      else
      {
        return etl::span<element_type, (COUNT != etl::dynamic_extent ? COUNT : (EXTENT != etl::dynamic_extent ? EXTENT - OFFSET : etl::dynamic_extent))>(mbegin + OFFSET, mbegin + OFFSET + COUNT);
      }
    }
#endif

    //*************************************************************************
    /// Obtains a span that is a view from 'offset' over the next 'count' elements of this span.
    //*************************************************************************
    ETL_CONSTEXPR14 etl::span<element_type, etl::dynamic_extent> subspan(size_t offset, size_t count = etl::dynamic_extent) const
    {
      if (count == etl::dynamic_extent)
      {
        return etl::span<element_type, etl::dynamic_extent>(mbegin + offset, mend);
      }
      else
      {
        return etl::span<element_type, etl::dynamic_extent>(mbegin + offset, mbegin + offset + count);
      }
    }

  private:

    element_type* mbegin;
    element_type* mend;
  };

  //***************************************************************************
  /// Span specialisation
  //***************************************************************************
  template <typename T>
  class span<T, etl::dynamic_extent>
  {
  public:

    typedef T        element_type;
    typedef typename etl::remove_cv<T>::type value_type;
    typedef size_t   size_type;
    typedef T&       reference;
    typedef const T& const_reference;
    typedef T*       pointer;
    typedef const T* const_pointer;
    typedef T*       iterator;
    typedef const T* const_iterator;
    typedef ETL_OR_STD::reverse_iterator<iterator>       reverse_iterator;
    typedef ETL_OR_STD::reverse_iterator<const_iterator> const_reverse_iterator;

    static ETL_CONSTANT size_t extent = etl::dynamic_extent;

    //*************************************************************************
    /// Default constructor.
    //*************************************************************************
    ETL_CONSTEXPR span() ETL_NOEXCEPT
      : mbegin(ETL_NULLPTR)
      , mend(ETL_NULLPTR)
    {
    }

    //*************************************************************************
    /// Construct from pointer + size
    //*************************************************************************
    template <typename TIterator, typename TSize>
    ETL_CONSTEXPR span(const TIterator begin_, const TSize size_) ETL_NOEXCEPT
      : mbegin(etl::addressof(*begin_))
      , mend(etl::addressof(*begin_) + size_)
    {
    }

    //*************************************************************************
    /// Construct from iterators
    //*************************************************************************
    template <typename TIterator>
    ETL_CONSTEXPR span(const TIterator begin_, const TIterator end_)
      : mbegin(etl::addressof(*begin_))
      , mend(etl::addressof(*begin_) + etl::distance(begin_, end_))
    {
    }

    //*************************************************************************
    /// Construct from C array
    //*************************************************************************
    template<size_t ARRAY_SIZE>
    ETL_CONSTEXPR span(element_type(&begin_)[ARRAY_SIZE]) ETL_NOEXCEPT
      : mbegin(begin_)
      , mend(begin_ + ARRAY_SIZE)
    {
    }

#if ETL_CPP11_SUPPORTED
    //*************************************************************************
    /// Construct from etl::array.
    //*************************************************************************
    template <typename U, size_t N, typename = typename etl::enable_if<etl::is_same<etl::remove_cv_t<T>, etl::remove_cv_t<U>>::value, void>::type>
    ETL_CONSTEXPR span(etl::array<U, N>& a) ETL_NOEXCEPT
      : mbegin(a.data())
      , mend(a.data() + a.size())
    {
    }

    //*************************************************************************
    /// Construct from etl::array.
    //*************************************************************************
    template <typename U, size_t N, typename = typename etl::enable_if<etl::is_same<etl::remove_cv_t<T>, etl::remove_cv_t<U>>::value, void>::type>
    ETL_CONSTEXPR span(const etl::array<U, N>& a) ETL_NOEXCEPT
      : mbegin(a.data())
      , mend(a.data() + a.size())
    {
    }
#else
    //*************************************************************************
    /// Construct from etl::array.
    //*************************************************************************
    template <typename U, size_t N>
    ETL_CONSTEXPR span(etl::array<U, N>& a, typename etl::enable_if<etl::is_same<typename etl::remove_cv<T>::type, typename etl::remove_cv<U>::type>::value, void>::type) ETL_NOEXCEPT
      : mbegin(a.data())
      , mend(a.data() + a.size())
    {
    }

    //*************************************************************************
    /// Construct from etl::array.
    //*************************************************************************
    template <typename U, size_t N>
    ETL_CONSTEXPR span(const etl::array<U, N>& a, typename etl::enable_if<etl::is_same<typename etl::remove_cv<T>::type, typename etl::remove_cv<U>::type>::value, void>::type) ETL_NOEXCEPT
      : mbegin(a.data())
      , mend(a.data() + a.size())
    {
    }
#endif

#if ETL_CPP11_SUPPORTED && ETL_USING_STL
    //*************************************************************************
    /// Construct from std::array.
    //*************************************************************************
    template <typename U, size_t N, typename = typename etl::enable_if<etl::is_same<etl::remove_cv_t<T>, etl::remove_cv_t<U>>::value, void>::type>
    ETL_CONSTEXPR span(std::array<U, N>& a) ETL_NOEXCEPT
      : mbegin(a.data())
      , mend(a.data() + a.size())
    {
    }

    //*************************************************************************
    /// Construct from std::array.
    //*************************************************************************
    template <typename U, size_t N, typename = typename etl::enable_if<etl::is_same<etl::remove_cv_t<T>, etl::remove_cv_t<U>>::value, void>::type>
    ETL_CONSTEXPR span(const std::array<U, N>& a) ETL_NOEXCEPT
      : mbegin(a.data())
      , mend(a.data() + a.size())
    {
    }
#endif

#if ETL_CPP11_SUPPORTED
    //*************************************************************************
    /// Construct from a container or other type that supports
    /// data() and size() member functions.
    //*************************************************************************
    template <typename TContainer, typename = typename etl::enable_if<!etl::is_pointer<etl::remove_reference_t<TContainer>>::value &&
                                                                      !etl::is_array<etl::remove_reference_t<TContainer>>::value &&
                                                                      etl::is_same<etl::remove_cv_t<T>, etl::remove_cv_t<typename etl::remove_reference_t<TContainer>::value_type>>::value, void>::type>
      ETL_CONSTEXPR span(TContainer&& a) ETL_NOEXCEPT
      : mbegin(a.data())
      , mend(a.data() + a.size())
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
                                                              etl::is_same<typename etl::remove_cv<T>::type, typename etl::remove_cv<typename etl::remove_reference<TContainer>::type::value_type>::type>::value, void>::type) ETL_NOEXCEPT
      : mbegin(a.data())
      , mend(a.data() + a.size())
    {
    }

    //*************************************************************************
    /// Construct from a container or other type that supports
    /// data() and size() member functions.
    //*************************************************************************
    template <typename TContainer>
    ETL_CONSTEXPR span(const TContainer& a, typename etl::enable_if<!etl::is_pointer<typename etl::remove_reference<TContainer>::type>::value &&
                                                                    !etl::is_array<TContainer>::value &&
                                                                    etl::is_same<typename etl::remove_cv<T>::type, typename etl::remove_cv<typename etl::remove_reference<TContainer>::type::value_type>::type>::value, void>::type) ETL_NOEXCEPT
      : mbegin(a.data())
      , mend(a.data() + a.size())
    {
    }
#endif

    //*************************************************************************
    /// Copy constructor
    //*************************************************************************
    ETL_CONSTEXPR span(const span& other) ETL_NOEXCEPT
      : mbegin(other.mbegin)
      , mend(other.mend)
    {
    }

    //*************************************************************************
    /// Returns a reference to the first element.
    //*************************************************************************
    ETL_CONSTEXPR reference front() const ETL_NOEXCEPT
    {
      return *mbegin;
    }

    //*************************************************************************
    /// Returns a reference to the last element.
    //*************************************************************************
    ETL_CONSTEXPR reference back() const ETL_NOEXCEPT
    {
      return *(mend - 1);
    }

    //*************************************************************************
    /// Returns a pointer to the first element of the internal storage.
    //*************************************************************************
    ETL_CONSTEXPR pointer data() const ETL_NOEXCEPT
    {
      return mbegin;
    }

    //*************************************************************************
    /// Returns an iterator to the beginning of the span.
    //*************************************************************************
    ETL_CONSTEXPR iterator begin() const ETL_NOEXCEPT
    {
      return mbegin;
    }

    //*************************************************************************
    /// Returns an iterator to the end of the span.
    //*************************************************************************
    ETL_CONSTEXPR iterator end() const ETL_NOEXCEPT
    {
      return mend;
    }

    //*************************************************************************
    // Returns an reverse iterator to the reverse beginning of the span.
    //*************************************************************************
    ETL_CONSTEXPR reverse_iterator rbegin() const ETL_NOEXCEPT
    {
      return reverse_iterator(mend);
    }

    //*************************************************************************
    /// Returns a reverse iterator to the end of the span.
    //*************************************************************************
    ETL_CONSTEXPR reverse_iterator rend() const ETL_NOEXCEPT
    {
      return reverse_iterator(mbegin);
    }

    //*************************************************************************
    /// Returns <b>true</b> if the span size is zero.
    //*************************************************************************
    ETL_CONSTEXPR bool empty() const ETL_NOEXCEPT
    {
      return (mbegin == mend);
    }

    //*************************************************************************
    /// Returns the size of the span.
    //*************************************************************************
    ETL_CONSTEXPR size_t size() const ETL_NOEXCEPT
    {
      return (mend - mbegin);
    }

    //*************************************************************************
    /// Returns the size of the span in bytes.
    //*************************************************************************
    ETL_CONSTEXPR size_t size_bytes() const ETL_NOEXCEPT
    {
      return sizeof(element_type) * (mend - mbegin);
    }

    //*************************************************************************
    /// Returns the maximum possible size of the span.
    //*************************************************************************
    ETL_CONSTEXPR size_t max_size() const ETL_NOEXCEPT
    {
      return size();
    }

    //*************************************************************************
    /// Assign from a span.
    //*************************************************************************
    ETL_CONSTEXPR14 span& operator =(const span& other) ETL_NOEXCEPT
    {
      mbegin = other.mbegin;
      mend = other.mend;
      return *this;
    }

    //*************************************************************************
    /// Returns a reference to the indexed value.
    //*************************************************************************
    ETL_CONSTEXPR reference operator[](const size_t i) const
    {
      return mbegin[i];
    }

    //*************************************************************************
    /// Obtains a span that is a view over the first COUNT elements of this span.
    //*************************************************************************
    template <size_t COUNT>
    ETL_CONSTEXPR etl::span<element_type, COUNT> first() const
    {
      return etl::span<element_type, COUNT>(mbegin, mbegin + COUNT);
    }

    //*************************************************************************
    /// Obtains a span that is a view over the first count elements of this span.
    //*************************************************************************
    ETL_CONSTEXPR etl::span<element_type, etl::dynamic_extent> first(size_t count) const
    {
      return etl::span<element_type, etl::dynamic_extent>(mbegin, mbegin + count);
    }

    //*************************************************************************
    /// Obtains a span that is a view over the last COUNT elements of this span.
    //*************************************************************************
    template <size_t COUNT>
    ETL_CONSTEXPR etl::span<element_type, COUNT> last() const
    {
      return etl::span<element_type, COUNT>(mend - COUNT, mend);
    }

    //*************************************************************************
    /// Obtains a span that is a view over the last count elements of this span.
    //*************************************************************************
    ETL_CONSTEXPR etl::span<element_type, etl::dynamic_extent> last(size_t count) const
    {
      return etl::span<element_type, etl::dynamic_extent>(mend - count, mend);
    }

#if ETL_CPP11_SUPPORTED
    //*************************************************************************
    /// Obtains a span that is a view from OFFSET over the next COUNT elements of this span.
    //*************************************************************************
    template <const size_t OFFSET, size_t COUNT = etl::dynamic_extent>
    ETL_CONSTEXPR 
    etl::span<element_type, COUNT> subspan() const
    {
      if (COUNT == etl::dynamic_extent)
      {
        return etl::span<element_type, COUNT>(mbegin + OFFSET, mend);
      }
      else
      {
        return etl::span<element_type, COUNT>(mbegin + OFFSET, mbegin + OFFSET + COUNT);
      }     
    }
#else
    //*************************************************************************
    /// Obtains a span that is a view from OFFSET over the next COUNT elements of this span.
    //*************************************************************************
    template <const size_t OFFSET, size_t COUNT>
    ETL_CONSTEXPR 
    etl::span<element_type, (COUNT != etl::dynamic_extent ? COUNT : etl::dynamic_extent)> subspan() const
    {
      if (COUNT == etl::dynamic_extent)
      {
        return etl::span<element_type, COUNT>(mbegin + OFFSET, mend);
      }
      else
      {
        return etl::span<element_type, COUNT>(mbegin + OFFSET, mbegin + OFFSET + COUNT);
      }
    }
#endif

    //*************************************************************************
    /// Obtains a span that is a view from 'offset' over the next 'count' elements of this span.
    //*************************************************************************
    ETL_CONSTEXPR14 etl::span<element_type, etl::dynamic_extent> subspan(size_t offset, size_t count = etl::dynamic_extent) const
    {
      if (count == etl::dynamic_extent)
      {
        return etl::span<element_type, etl::dynamic_extent>(mbegin + offset, mend);
      }
      return etl::span<element_type, etl::dynamic_extent>(mbegin + offset, mbegin + offset + count);
    }

  private:

    element_type* mbegin;
    element_type* mend;
  };

  //*************************************************************************
  /// Template deduction guides.
  //*************************************************************************
#if ETL_CPP17_SUPPORTED
  template <typename TArray>
  span(TArray& a)
    ->span<typename TArray::value_type, etl::dynamic_extent>;

  template <typename TIterator>
  span(const TIterator begin_, const TIterator end_)
    ->span<etl::remove_pointer_t<TIterator>, etl::dynamic_extent>;

  template <typename TIterator, typename TSize>
    span(const TIterator begin_, const TSize size_)
    ->span<etl::remove_pointer_t<TIterator>, etl::dynamic_extent>;
#endif 

  //*************************************************************************
  /// Hash function.
  //*************************************************************************
#if ETL_USING_8BIT_TYPES
  template <typename T>
  struct hash<etl::span<T> >
  {
    size_t operator()(const etl::span<T>& view) const
    {
      return etl::private_hash::generic_hash<size_t>(reinterpret_cast<const uint8_t*>(&view[0]),
        reinterpret_cast<const uint8_t*>(&view[view.size()]));
    }
  };
#endif
}

#endif
