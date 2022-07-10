///\file

/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl
https://www.etlcpp.com

Copyright(c) 2022 John Wellbelove

Inspired by the techniques used in https://github.com/mrshurik/poly_span
Copyright(c) 2020 Dr. Alexander Bulovyatov

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

#ifndef ETL_POLY_SPAN_INCLUDED
#define ETL_POLY_SPAN_INCLUDED

#include "platform.h"
#include "iterator.h"
#include "nullptr.h"
#include "hash.h"
#include "type_traits.h"
#include "integral_limits.h"
#include "memory.h"
#include "array.h"

#include "private/dynamic_extent.h"

#if ETL_USING_CPP11 && ETL_USING_STL
#include <array>
#endif

///\defgroup poly_span poly_span
///\ingroup containers

namespace etl
{
  //***************************************************************************
  /// Poly Span
  //***************************************************************************
  template <typename TBase, size_t Extent = etl::dynamic_extent>
  class poly_span
  {
  public:

    typedef TBase                                element_type;
    typedef typename etl::remove_cv<TBase>::type value_type;
    typedef size_t                               size_type;
    typedef TBase&                               reference;
    typedef const TBase&                         const_reference;
    typedef TBase*                               pointer;
    typedef const TBase*                         const_pointer;
    //typedef TBase*                               iterator;
    //typedef const TBase*                         const_iterator;

    //typedef ETL_OR_STD::reverse_iterator<iterator>       reverse_iterator;
    //typedef ETL_OR_STD::reverse_iterator<const_iterator> const_reverse_iterator;

    static ETL_CONSTANT size_t extent = Extent;

    template <typename U, size_t Extent>
    friend class poly_span;

    //*************************************************************************
    /// Default constructor.
    //*************************************************************************
    ETL_CONSTEXPR poly_span() ETL_NOEXCEPT
      : pbegin(ETL_NULLPTR)
      , element_size(0U)
    {
    }

    //*************************************************************************
    /// Construct from iterator + size
    //*************************************************************************
    template <typename TIterator, typename TSize>
    ETL_CONSTEXPR poly_span(const TIterator begin_, const TSize size_) ETL_NOEXCEPT
      : pbegin(etl::addressof(*begin_))
    {
      typedef typename etl::iterator_traits<TIterator>::value_type        data_type;
      typedef typename etl::iterator_traits<TIterator>::iterator_category iterator_category;

      ETL_STATIC_ASSERT((etl::is_same<ETL_OR_STD::random_access_iterator_tag, typename etl::iterator_traits<TIterator>::iterator_category>::value), "Not a random access iterator");
      ETL_STATIC_ASSERT((etl::is_base_of<TBase, data_type>::value || etl::is_same<TBase, data_type>::value), "TBase not a base of the data type");

      static ETL_CONSTANT size_t Element_Size = sizeof(data_type);

      element_size = Element_Size;
    }

    //*************************************************************************
    /// Construct from iterators
    //*************************************************************************
    template <typename TIterator>
    ETL_CONSTEXPR poly_span(const TIterator begin_, const TIterator end_)
      : pbegin(etl::addressof(*begin_))
    {
      typedef typename etl::iterator_traits<TIterator>::value_type        data_type;
      typedef typename etl::iterator_traits<TIterator>::iterator_category iterator_category;

      ETL_STATIC_ASSERT((etl::is_same<ETL_OR_STD::random_access_iterator_tag, iterator_category>::value),    "Not a random access iterator");
      ETL_STATIC_ASSERT((etl::is_base_of<TBase, data_type>::value || etl::is_same<TBase, data_type>::value), "TBase not a base of the data type");

      static ETL_CONSTANT size_t Element_Size = sizeof(data_type);

      element_size = Element_Size;
    }

    //*************************************************************************
    /// Construct from C array
    //*************************************************************************
    template<typename U, size_t N>
    ETL_CONSTEXPR poly_span(U(&begin_)[N]) ETL_NOEXCEPT
      : pbegin(begin_)
    {
      ETL_STATIC_ASSERT(N <= Extent, "Array data overflow");
      ETL_STATIC_ASSERT((etl::is_base_of<TBase, U>::value || etl::is_same<TBase, U>::value), "TBase not a base of the data type");

      static ETL_CONSTANT size_t Element_Size = sizeof(U);

      element_size = Element_Size;
    }

    //*************************************************************************
    /// Construct from etl::array.
    //*************************************************************************
    template <typename U, size_t N>
    ETL_CONSTEXPR poly_span(etl::array<U, N>& a) ETL_NOEXCEPT
      : pbegin(a.data())
    {
      ETL_STATIC_ASSERT(N <= Extent, "Array data overflow");
      ETL_STATIC_ASSERT((etl::is_base_of<TBase, U>::value || etl::is_same<TBase, U>::value), "TBase not a base of the data type");

      static ETL_CONSTANT size_t Element_Size = sizeof(U);

      element_size = Element_Size;
    }

    //*************************************************************************
    /// Construct from etl::array.
    //*************************************************************************
    template <typename U, size_t N>
    ETL_CONSTEXPR poly_span(const etl::array<U, N>& a) ETL_NOEXCEPT
      : pbegin(a.data())
    {
      ETL_STATIC_ASSERT(N <= Extent, "Array data overflow");
      ETL_STATIC_ASSERT((etl::is_base_of<TBase, U>::value || etl::is_same<TBase, U>::value), "TBase not a base of the data type");

      static ETL_CONSTANT size_t Element_Size = sizeof(U);

      element_size = Element_Size;
    }

#if ETL_USING_CPP11 && ETL_USING_STL
    //*************************************************************************
    /// Construct from std::array.
    //*************************************************************************
    template <typename U, size_t N>
    ETL_CONSTEXPR poly_span(std::array<U, N>& a) ETL_NOEXCEPT
      : pbegin(a.data())
    {
      ETL_STATIC_ASSERT(N <= Extent, "Array data overflow");
      ETL_STATIC_ASSERT((etl::is_base_of<TBase, U>::value || etl::is_same<TBase, U>::value), "TBase not a base of U");

      static ETL_CONSTANT size_t Element_Size = sizeof(U);

      element_size = Element_Size;
    }

    //*************************************************************************
    /// Construct from std::array.
    //*************************************************************************
    template <typename U, size_t N>
    ETL_CONSTEXPR poly_span(const std::array<U, N>& a) ETL_NOEXCEPT
      : pbegin(a.data())
    {
      ETL_STATIC_ASSERT(N <= Extent, "Array data overflow");
      ETL_STATIC_ASSERT((etl::is_base_of<TBase, U>::value || etl::is_same<TBase, U>::value), "TBase not a base of U");

      static ETL_CONSTANT size_t Element_Size = sizeof(U);

      element_size = Element_Size;
    }
#endif

    //*************************************************************************
    /// Copy constructor
    //*************************************************************************
    ETL_CONSTEXPR poly_span(const poly_span<TBase, Extent>& other) ETL_NOEXCEPT
      : pbegin(other.pbegin)
      , element_size(other.element_size)
    {
    }

    //*************************************************************************
    /// Copy constructor
    //*************************************************************************
    template <typename UBase>
    ETL_CONSTEXPR poly_span(const poly_span<UBase, Extent>& other) ETL_NOEXCEPT
      : pbegin(other.pbegin)
      , element_size(other.element_size)
    {
    }
//
//    //*************************************************************************
//    /// Returns a reference to the first element.
//    //*************************************************************************
//    ETL_NODISCARD ETL_CONSTEXPR reference front() const ETL_NOEXCEPT
//    {
//      return *pbegin;
//    }
//
//    //*************************************************************************
//    /// Returns a reference to the last element.
//    //*************************************************************************
//    ETL_NODISCARD ETL_CONSTEXPR reference back() const ETL_NOEXCEPT
//    {
//      return *(pend - 1);
//    }
//
    //*************************************************************************
    /// Returns a pointer to the first element of the internal storage.
    //*************************************************************************
    ETL_NODISCARD ETL_CONSTEXPR pointer data() const ETL_NOEXCEPT
    {
      return pbegin;
    }

    ////*************************************************************************
    ///// Returns an iterator to the beginning of the poly_span.
    ////*************************************************************************
    //ETL_NODISCARD ETL_CONSTEXPR iterator begin() const ETL_NOEXCEPT
    //{
    //  return iterator(pbegin, 0U, element_size);
    //}

    ////*************************************************************************
    ///// Returns an iterator to the end of the poly_span.
    ////*************************************************************************
    //ETL_NODISCARD ETL_CONSTEXPR iterator end() const ETL_NOEXCEPT
    //{
    //  return iterator(pbegin, extent, element_size);
    //}

//    //*************************************************************************
//    // Returns an reverse iterator to the reverse beginning of the poly_span.
//    //*************************************************************************
//    ETL_CONSTEXPR reverse_iterator rbegin() const ETL_NOEXCEPT
//    {
//      return reverse_iterator(pend);
//    }
//
//    //*************************************************************************
//    /// Returns a reverse iterator to the end of the poly_span.
//    //*************************************************************************
//    ETL_CONSTEXPR reverse_iterator rend() const ETL_NOEXCEPT
//    {
//      return reverse_iterator(pbegin);
//    }
//
    //*************************************************************************
    /// Returns <b>true</b> if the poly_span size is zero.
    //*************************************************************************
    ETL_NODISCARD ETL_CONSTEXPR bool empty() const ETL_NOEXCEPT
    {
      return (Extent != 0U)
    }

    //*************************************************************************
    /// Returns the size of the poly_span.
    //*************************************************************************
    ETL_NODISCARD ETL_CONSTEXPR size_t size() const ETL_NOEXCEPT
    {
      return Extent;
    }

    //*************************************************************************
    /// Returns the size of the type stored in the poly_span.
    //*************************************************************************
    ETL_NODISCARD ETL_CONSTEXPR size_t size_of_element() const ETL_NOEXCEPT
    {
      return element_size;
    }

    //*************************************************************************
    /// Returns the size of the poly_span in bytes.
    //*************************************************************************
    ETL_NODISCARD ETL_CONSTEXPR size_t size_bytes() const ETL_NOEXCEPT
    {
      return Extent * element_size;
    }

//    //*************************************************************************
//    /// Assign from a poly_span.
//    //*************************************************************************
//    ETL_CONSTEXPR14 poly_span& operator =(const poly_span& other) ETL_NOEXCEPT
//    {
//      pbegin = other.pbegin;
//      pend = other.pend;
//      return *this;
//    }
//
//    //*************************************************************************
//    /// Returns a reference to the indexed value.
//    //*************************************************************************
//    ETL_CONSTEXPR reference operator[](const size_t i) const
//    {
//      return pbegin[i];
//    }
//
//    //*************************************************************************
//    /// Obtains a poly_span that is a view over the first COUNT elements of this poly_span.
//    //*************************************************************************
//    template <size_t COUNT>
//    ETL_CONSTEXPR etl::poly_span<element_type, COUNT> first() const
//    {
//      return etl::poly_span<element_type, COUNT>(pbegin, pbegin + COUNT);
//    }
//
//    //*************************************************************************
//    /// Obtains a poly_span that is a view over the first count elements of this poly_span.
//    //*************************************************************************
//    ETL_CONSTEXPR etl::poly_span<element_type, etl::dynamic_extent> first(size_t count) const
//    {
//      return etl::poly_span<element_type, etl::dynamic_extent>(pbegin, pbegin + count);
//    }
//
//    //*************************************************************************
//    /// Obtains a poly_span that is a view over the last COUNT elements of this poly_span.
//    //*************************************************************************
//    template <size_t COUNT>
//    ETL_CONSTEXPR etl::poly_span<element_type, COUNT> last() const
//    {
//      return etl::poly_span<element_type, COUNT>(pend - COUNT, pend);
//    }
//
//    //*************************************************************************
//    /// Obtains a poly_span that is a view over the last count elements of this poly_span.
//    //*************************************************************************
//    ETL_CONSTEXPR etl::poly_span<element_type, etl::dynamic_extent> last(size_t count) const
//    {
//      return etl::poly_span<element_type, etl::dynamic_extent>(pend - count, pend);
//    }
//
//#if ETL_USING_CPP11
//    //*************************************************************************
//    /// Obtains a poly_span that is a view from OFFSET over the next COUNT elements of this poly_span.
//    //*************************************************************************
//    template <const size_t OFFSET, size_t COUNT = etl::dynamic_extent>
//    ETL_CONSTEXPR
//      etl::poly_span<element_type, (COUNT != etl::dynamic_extent ? COUNT : (Extent != etl::dynamic_extent ? Extent - OFFSET : etl::dynamic_extent))> subspan() const
//    {
//      if (COUNT == etl::dynamic_extent)
//      {
//        return etl::poly_span<element_type, (COUNT != etl::dynamic_extent ? COUNT : (Extent != etl::dynamic_extent ? Extent - OFFSET : etl::dynamic_extent))>(pbegin + OFFSET, pend);
//      }
//      else
//      {
//        return etl::poly_span<element_type, (COUNT != etl::dynamic_extent ? COUNT : (Extent != etl::dynamic_extent ? Extent - OFFSET : etl::dynamic_extent))>(pbegin + OFFSET, pbegin + OFFSET + COUNT);
//      }
//    }
//#else
//    //*************************************************************************
//    /// Obtains a poly_span that is a view from OFFSET over the next COUNT elements of this poly_span.
//    //*************************************************************************
//    template <const size_t OFFSET, size_t COUNT>
//    ETL_CONSTEXPR
//      etl::poly_span<element_type, (COUNT != etl::dynamic_extent ? COUNT : (Extent != etl::dynamic_extent ? Extent - OFFSET : etl::dynamic_extent))> subspan() const
//    {
//      if (COUNT == etl::dynamic_extent)
//      {
//        return etl::poly_span<element_type, (COUNT != etl::dynamic_extent ? COUNT : (Extent != etl::dynamic_extent ? Extent - OFFSET : etl::dynamic_extent))>(pbegin + OFFSET, pend);
//      }
//      else
//      {
//        return etl::poly_span<element_type, (COUNT != etl::dynamic_extent ? COUNT : (Extent != etl::dynamic_extent ? Extent - OFFSET : etl::dynamic_extent))>(pbegin + OFFSET, pbegin + OFFSET + COUNT);
//      }
//    }
//#endif
//
//    //*************************************************************************
//    /// Obtains a poly_span that is a view from 'offset' over the next 'count' elements of this poly_span.
//    //*************************************************************************
//    ETL_CONSTEXPR14 etl::poly_span<element_type, etl::dynamic_extent> subspan(size_t offset, size_t count = etl::dynamic_extent) const
//    {
//      if (count == etl::dynamic_extent)
//      {
//        return etl::poly_span<element_type, etl::dynamic_extent>(pbegin + offset, pend);
//      }
//      else
//      {
//        return etl::poly_span<element_type, etl::dynamic_extent>(pbegin + offset, pbegin + offset + count);
//      }
//    }

  private:

    pointer pbegin;
    size_t  element_size;
  };

  //***************************************************************************
  /// Span specialisation
  //***************************************************************************
  template <typename TBase>
  class poly_span<TBase, etl::dynamic_extent>
  {
  public:

    typedef TBase        element_type;
    typedef typename etl::remove_cv<TBase>::type value_type;
    typedef size_t   size_type;
    typedef TBase& reference;
    typedef const TBase& const_reference;
    typedef TBase* pointer;
    typedef const TBase* const_pointer;
    typedef TBase* iterator;
    typedef const TBase* const_iterator;
    typedef ETL_OR_STD::reverse_iterator<iterator>       reverse_iterator;
    typedef ETL_OR_STD::reverse_iterator<const_iterator> const_reverse_iterator;
 
    template <typename UBase, size_t Extent>
    friend class poly_span;

    //*************************************************************************
    /// Default constructor.
    //*************************************************************************
    ETL_CONSTEXPR poly_span() ETL_NOEXCEPT
      : pbegin(ETL_NULLPTR)
      , extent(0U)
      , element_size(0U)
    {
    }

    //*************************************************************************
    /// Construct from iterator + size
    //*************************************************************************
    template <typename TIterator, typename TSize>
    ETL_CONSTEXPR poly_span(const TIterator begin_, const TSize size_) ETL_NOEXCEPT
      : pbegin(etl::addressof(*begin_))
      , extent(size_)
    {
      typedef typename etl::iterator_traits<TIterator>::value_type        data_type;
      typedef typename etl::iterator_traits<TIterator>::iterator_category iterator_category;

      ETL_STATIC_ASSERT((etl::is_same<ETL_OR_STD::random_access_iterator_tag, iterator_category>::value), "Not a random access iterator");
      ETL_STATIC_ASSERT((etl::is_base_of<TBase, data_type>::value || etl::is_same<TBase, data_type>::value), "TBase not a base of the data type");

      static ETL_CONSTANT size_t Element_Size = sizeof(data_type);
      element_size = Element_Size;
    }

    //*************************************************************************
    /// Construct from iterators
    //*************************************************************************
    template <typename TIterator>
    ETL_CONSTEXPR poly_span(const TIterator begin_, const TIterator end_)
      : pbegin(etl::addressof(*begin_))
      , extent(size_t(etl::distance(begin_, end_)))
    {
      typedef typename etl::iterator_traits<TIterator>::value_type        data_type;
      typedef typename etl::iterator_traits<TIterator>::iterator_category iterator_category;

      ETL_STATIC_ASSERT((etl::is_same<ETL_OR_STD::random_access_iterator_tag, iterator_category>::value), "Not a random access iterator");
      ETL_STATIC_ASSERT((etl::is_base_of<TBase, data_type>::value || etl::is_same<TBase, data_type>::value), "TBase not a base of the data type");

      static ETL_CONSTANT size_t Element_Size = sizeof(data_type);
      element_size = Element_Size;
    }

    //*************************************************************************
    /// Construct from C array
    //*************************************************************************
    template<typename U, size_t N>
    ETL_CONSTEXPR poly_span(U(&begin_)[N]) ETL_NOEXCEPT
      : pbegin(begin_)
      , extent(N)
    {
      ETL_STATIC_ASSERT((etl::is_base_of<TBase, U>::value || etl::is_same<TBase, U>::value), "TBase not a base of the data type");

      static ETL_CONSTANT size_t Element_Size = sizeof(U);
      element_size = Element_Size;
    }

    //*************************************************************************
    /// Construct from etl::array.
    //*************************************************************************
    template <typename U, size_t N>
    ETL_CONSTEXPR poly_span(etl::array<U, N>& a) ETL_NOEXCEPT
      : pbegin(a.data())
      , extent(N)
    {
      ETL_STATIC_ASSERT((etl::is_base_of<TBase, U>::value || etl::is_same<TBase, U>::value), "TBase not a base of the data type");

      static ETL_CONSTANT size_t Element_Size = sizeof(U);
      element_size = Element_Size;
    }

    //*************************************************************************
    /// Construct from etl::array.
    //*************************************************************************
    template <typename U, size_t N>
    ETL_CONSTEXPR poly_span(const etl::array<U, N>& a) ETL_NOEXCEPT
      : pbegin(a.data())
      , extent(N)
    {
      ETL_STATIC_ASSERT((etl::is_base_of<TBase, U>::value || etl::is_same<TBase, U>::value), "TBase not a base of the data type");

      static ETL_CONSTANT size_t Element_Size = sizeof(U);
      element_size = Element_Size;
    }

#if ETL_USING_CPP11 && ETL_USING_STL
    //*************************************************************************
    /// Construct from std::array.
    //*************************************************************************
    template <typename U, size_t N>
    ETL_CONSTEXPR poly_span(std::array<U, N>& a) ETL_NOEXCEPT
      : pbegin(a.data())
      , extent(N)
    {
      ETL_STATIC_ASSERT((etl::is_base_of<TBase, U>::value || etl::is_same<TBase, U>::value), "TBase not a base of U");

      static ETL_CONSTANT size_t Element_Size = sizeof(U);
      element_size = Element_Size;
    }

    //*************************************************************************
    /// Construct from std::array.
    //*************************************************************************
    template <typename U, size_t N>
    ETL_CONSTEXPR poly_span(const std::array<U, N>& a) ETL_NOEXCEPT
      : pbegin(a.data())
      , extent(N)
    {
      ETL_STATIC_ASSERT((etl::is_base_of<TBase, U>::value || etl::is_same<TBase, U>::value), "TBase not a base of U");

      static ETL_CONSTANT size_t Element_Size = sizeof(U);
      element_size = Element_Size;
    }
#endif

    //*************************************************************************
    /// Copy constructor
    //*************************************************************************
    ETL_CONSTEXPR poly_span(const poly_span<TBase, etl::dynamic_extent>& other) ETL_NOEXCEPT
      : pbegin(other.pbegin)
      , extent(other.extent)
      , element_size(other.element_size)
    {
    }

    //*************************************************************************
    /// Copy constructor
    //*************************************************************************
    template <typename UBase>
    ETL_CONSTEXPR poly_span(const poly_span<UBase, etl::dynamic_extent>& other) ETL_NOEXCEPT
      : pbegin(other.pbegin)
      , extent(other.extent)
      , element_size(other.element_size)
    {
    }
//
//    //*************************************************************************
//    /// Returns a reference to the first element.
//    //*************************************************************************
//    ETL_NODISCARD ETL_CONSTEXPR reference front() const ETL_NOEXCEPT
//    {
//      return *pbegin;
//    }
//
//    //*************************************************************************
//    /// Returns a reference to the last element.
//    //*************************************************************************
//    ETL_NODISCARD ETL_CONSTEXPR reference back() const ETL_NOEXCEPT
//    {
//      return *(pend - 1);
//    }
//
    //*************************************************************************
    /// Returns a pointer to the first element of the internal storage.
    //*************************************************************************
    ETL_NODISCARD ETL_CONSTEXPR pointer data() const ETL_NOEXCEPT
    {
      return pbegin;
    }

    ////*************************************************************************
    ///// Returns an iterator to the beginning of the poly_span.
    ////*************************************************************************
    //ETL_NODISCARD ETL_CONSTEXPR iterator begin() const ETL_NOEXCEPT
    //{
    //  return iterator(pbegin, 0U, element_size);
    //}

    ////*************************************************************************
    ///// Returns an iterator to the end of the poly_span.
    ////*************************************************************************
    //ETL_NODISCARD ETL_CONSTEXPR iterator end() const ETL_NOEXCEPT
    //{
    //  return iterator(pbegin, extent, element_size);
    //}

//    //*************************************************************************
//    // Returns an reverse iterator to the reverse beginning of the poly_span.
//    //*************************************************************************
//    ETL_NODISCARD ETL_CONSTEXPR reverse_iterator rbegin() const ETL_NOEXCEPT
//    {
//      return reverse_iterator(pend);
//    }
//
//    //*************************************************************************
//    /// Returns a reverse iterator to the end of the poly_span.
//    //*************************************************************************
//    ETL_NODISCARD ETL_CONSTEXPR reverse_iterator rend() const ETL_NOEXCEPT
//    {
//      return reverse_iterator(pbegin);
//    }
//
    //*************************************************************************
    /// Returns <b>true</b> if the poly_span size is zero.
    //*************************************************************************
    ETL_NODISCARD ETL_CONSTEXPR bool empty() const ETL_NOEXCEPT
    {
      return (extent == 0);
    }

    //*************************************************************************
    /// Returns the size of the poly_span.
    //*************************************************************************
    ETL_NODISCARD ETL_CONSTEXPR size_t size() const ETL_NOEXCEPT
    {
      return extent;
    }

    //*************************************************************************
    /// Returns the size of the type stored in the poly_span.
    //*************************************************************************
    ETL_NODISCARD ETL_CONSTEXPR size_t size_of_element() const ETL_NOEXCEPT
    {
      return element_size;
    }

    //*************************************************************************
    /// Returns the size of the poly_span in bytes.
    //*************************************************************************
    ETL_NODISCARD ETL_CONSTEXPR size_t size_bytes() const ETL_NOEXCEPT
    {
      return element_size * extent;
    }

//    //*************************************************************************
//    /// Assign from a poly_span.
//    //*************************************************************************
//    ETL_CONSTEXPR14 poly_span& operator =(const poly_span& other) ETL_NOEXCEPT
//    {
//      pbegin = other.pbegin;
//      pend = other.pend;
//      return *this;
//    }

    //*************************************************************************
    /// Returns a reference to the indexed value.
    //*************************************************************************
    ETL_CONSTEXPR reference operator[](size_t i) const
    {
      return *element_at(i);
    }

//    //*************************************************************************
//    /// Obtains a poly_span that is a view over the first COUNT elements of this poly_span.
//    //*************************************************************************
//    template <size_t COUNT>
//    ETL_CONSTEXPR etl::poly_span<element_type, COUNT> first() const
//    {
//      return etl::poly_span<element_type, COUNT>(pbegin, pbegin + COUNT);
//    }
//
//    //*************************************************************************
//    /// Obtains a poly_span that is a view over the first count elements of this poly_span.
//    //*************************************************************************
//    ETL_CONSTEXPR etl::poly_span<element_type, etl::dynamic_extent> first(size_t count) const
//    {
//      return etl::poly_span<element_type, etl::dynamic_extent>(pbegin, pbegin + count);
//    }
//
//    //*************************************************************************
//    /// Obtains a poly_span that is a view over the last COUNT elements of this poly_span.
//    //*************************************************************************
//    template <size_t COUNT>
//    ETL_CONSTEXPR etl::poly_span<element_type, COUNT> last() const
//    {
//      return etl::poly_span<element_type, COUNT>(pend - COUNT, pend);
//    }
//
//    //*************************************************************************
//    /// Obtains a poly_span that is a view over the last count elements of this poly_span.
//    //*************************************************************************
//    ETL_CONSTEXPR etl::poly_span<element_type, etl::dynamic_extent> last(size_t count) const
//    {
//      return etl::poly_span<element_type, etl::dynamic_extent>(pend - count, pend);
//    }
//
//#if ETL_USING_CPP11
//    //*************************************************************************
//    /// Obtains a poly_span that is a view from OFFSET over the next COUNT elements of this poly_span.
//    //*************************************************************************
//    template <const size_t OFFSET, size_t COUNT = etl::dynamic_extent>
//    ETL_CONSTEXPR
//      etl::poly_span<element_type, COUNT> subspan() const
//    {
//      if (COUNT == etl::dynamic_extent)
//      {
//        return etl::poly_span<element_type, COUNT>(pbegin + OFFSET, pend);
//      }
//      else
//      {
//        return etl::poly_span<element_type, COUNT>(pbegin + OFFSET, pbegin + OFFSET + COUNT);
//      }
//    }
//#else
//    //*************************************************************************
//    /// Obtains a poly_span that is a view from OFFSET over the next COUNT elements of this poly_span.
//    //*************************************************************************
//    template <const size_t OFFSET, size_t COUNT>
//    ETL_CONSTEXPR
//      etl::poly_span<element_type, (COUNT != etl::dynamic_extent ? COUNT : etl::dynamic_extent)> subspan() const
//    {
//      if (COUNT == etl::dynamic_extent)
//      {
//        return etl::poly_span<element_type, COUNT>(pbegin + OFFSET, pend);
//      }
//      else
//      {
//        return etl::poly_span<element_type, COUNT>(pbegin + OFFSET, pbegin + OFFSET + COUNT);
//      }
//    }
//#endif
//
//    //*************************************************************************
//    /// Obtains a poly_span that is a view from 'offset' over the next 'count' elements of this poly_span.
//    //*************************************************************************
//    ETL_CONSTEXPR14 etl::poly_span<element_type, etl::dynamic_extent> subspan(size_t offset, size_t count = etl::dynamic_extent) const
//    {
//      if (count == etl::dynamic_extent)
//      {
//        return etl::poly_span<element_type, etl::dynamic_extent>(pbegin + offset, pend);
//      }
//      return etl::poly_span<element_type, etl::dynamic_extent>(pbegin + offset, pbegin + offset + count);
//    }
//
  private:

    //***************************************************************************
    template <typename TFrom, typename TTo>
    using transfer_const = typename etl::conditional<etl::is_const<TFrom>::value, typename etl::add_const<TTo>::type, TTo>::type;

    typedef transfer_const<TBase, char>* char_ptr_t;

    //***************************************************************************
    ETL_CONSTEXPR pointer element_at(size_t index) const ETL_NOEXCEPT
    {
      char_ptr_t base = reinterpret_cast<char_ptr_t>(pbegin);
      return reinterpret_cast<pointer>(base + (index * element_size));
    }

    pointer pbegin;
    size_t  extent;
    size_t  element_size;
  };
//
//  //*************************************************************************
//  /// Template deduction guides.
//  //*************************************************************************
//#if ETL_USING_CPP17
//  template <typename TArray>
//  poly_span(TArray& a)
//    ->poly_span<typename TArray::value_type, etl::dynamic_extent>;
//
//  template <typename TIterator>
//  poly_span(const TIterator begin_, const TIterator end_)
//    ->poly_span<etl::remove_pointer_t<TIterator>, etl::dynamic_extent>;
//
//  template <typename TIterator, typename TSize>
//  poly_span(const TIterator begin_, const TSize size_)
//    ->poly_span<etl::remove_pointer_t<TIterator>, etl::dynamic_extent>;
//#endif 
//
//  //*************************************************************************
//  /// Hash function.
//  //*************************************************************************
//#if ETL_USING_8BIT_TYPES
//  template <typename TBase>
//  struct hash<etl::poly_span<TBase> >
//  {
//    size_t operator()(const etl::poly_span<TBase>& view) const
//    {
//      return etl::private_hash::generic_hash<size_t>(reinterpret_cast<const uint8_t*>(&view[0]),
//        reinterpret_cast<const uint8_t*>(&view[view.size()]));
//    }
//  };
//#endif
}

#endif
