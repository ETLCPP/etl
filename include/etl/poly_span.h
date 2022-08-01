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

#if ETL_USING_STL && ETL_USING_CPP11
  #include <array>
#endif

///\defgroup poly_span poly_span
///\ingroup containers

namespace etl
{
  template <typename U, size_t Extent>
  class poly_span;

  namespace private_poly_span
  {
    //*************************************************************************
    // Iterator
    //*************************************************************************
    template <typename TBase>
    class iterator
    {
    public:

      template <typename U, size_t Extent>
      friend class etl::poly_span;

      template <typename UBase>
      friend class const_iterator;

      typedef TBase     value_type;
      typedef ptrdiff_t difference_type;
      typedef TBase*    pointer;
      typedef TBase&    reference;
      typedef ETL_OR_STD::random_access_iterator_tag iterator_category;

      //*****************************************
      iterator()
        : ptr(ETL_NULLPTR)
        , element_size(0U)
      {
      }

      //*****************************************
      iterator(const iterator& other)
        : ptr(other.ptr)
        , element_size(other.element_size)
      {
      }

      //*****************************************
      iterator& operator =(const iterator& rhs)
      {
        ptr = rhs.ptr;
        element_size = rhs.element_size;

        return *this;
      }

      //*****************************************
      TBase& operator *() const
      {
        return *ptr;
      }

      //*****************************************
      TBase* operator ->() const
      {
        return ptr;
      }

      //*****************************************
      iterator& operator ++()
      {
        ptr = reinterpret_cast<pointer>(reinterpret_cast<char*>(ptr) + element_size);
        return *this;
      }

      //*****************************************
      iterator operator ++(int)
      {
        iterator temp(*this);
        ptr = reinterpret_cast<pointer>(reinterpret_cast<char*>(ptr) + element_size);
        return temp;
      }

      //*****************************************
      iterator& operator --()
      {
        ptr = reinterpret_cast<pointer>(reinterpret_cast<char*>(ptr) - element_size);
        return *this;
      }

      //*****************************************
      iterator operator --(int)
      {
        iterator temp(*this);
        ptr = reinterpret_cast<pointer>(reinterpret_cast<char*>(ptr) - element_size);
        return temp;
      }

      //***************************************************
      iterator& operator +=(difference_type offset)
      {
        ptr = reinterpret_cast<pointer>(reinterpret_cast<char*>(ptr) + (offset * difference_type(element_size)));
        return *this;
      }

      //***************************************************
      iterator& operator -=(difference_type offset)
      {
        ptr = reinterpret_cast<pointer>(reinterpret_cast<char*>(ptr) - (offset * difference_type(element_size)));
        return *this;
      }

      //***************************************************
      friend bool operator == (const iterator& lhs, const iterator& rhs)
      {
        return (lhs.ptr == rhs.ptr) && (lhs.element_size == rhs.element_size);
      }

      //***************************************************
      friend bool operator != (const iterator& lhs, const iterator& rhs)
      {
        return !(lhs == rhs);
      }

      //***************************************************
      friend bool operator < (const iterator& lhs, const iterator& rhs)
      {
        return lhs.ptr < rhs.ptr;
      }

      //***************************************************
      friend bool operator <= (const iterator& lhs, const iterator& rhs)
      {
        return !(lhs > rhs);
      }

      //***************************************************
      friend bool operator > (const iterator& lhs, const iterator& rhs)
      {
        return (rhs < lhs);
      }

      //***************************************************
      friend bool operator >= (const iterator& lhs, const iterator& rhs)
      {
        return !(lhs < rhs);
      }

      //***************************************************
      friend iterator operator + (const iterator& lhs, difference_type offset)
      {
        iterator temp(lhs);
        temp += offset;
        return temp;
      }

      //***************************************************
      friend iterator operator + (difference_type offset, const iterator& rhs)
      {
        iterator temp(rhs);
        temp += offset;
        return temp;
      }

      //***************************************************
      friend iterator operator - (const iterator& lhs, difference_type offset)
      {
        iterator temp(lhs);
        temp -= offset;
        return temp;
      }

      //***************************************************
      friend iterator operator - (difference_type offset, const iterator& rhs)
      {
        iterator temp(rhs);
        temp -= offset;
        return temp;
      }

      //***************************************************
      friend difference_type operator - (const iterator& lhs, const iterator& rhs)
      {
        return lhs.ptr - rhs.ptr;
      }

    private:

      //***************************************************
      iterator(TBase* pbegin_, size_t index_, size_t element_size_)
        : element_size(element_size_)
      {
        ptr = reinterpret_cast<pointer>(reinterpret_cast<char*>(pbegin_) + (index_ * element_size));
      }

      TBase* ptr;
      size_t element_size;
    };
  }

  //***************************************************************************
  /// Poly Span - Fixed Extent
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
    typedef private_poly_span::iterator<TBase>   iterator;

    typedef ETL_OR_STD::reverse_iterator<iterator> reverse_iterator;

    static ETL_CONSTANT size_t extent = Extent;

    template <typename UBase, size_t UExtent>
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
    ETL_CONSTEXPR poly_span(const TIterator begin_, const TSize /*size_*/) ETL_NOEXCEPT
      : pbegin(etl::addressof(*begin_))
      , element_size(sizeof(typename etl::iterator_traits<TIterator>::value_type))
    {
      typedef typename etl::iterator_traits<TIterator>::value_type        data_type;

      ETL_STATIC_ASSERT((etl::is_same<ETL_OR_STD::random_access_iterator_tag, typename etl::iterator_traits<TIterator>::iterator_category>::value), "Not a random access iterator");
      ETL_STATIC_ASSERT((etl::is_base_of<TBase, data_type>::value || etl::is_same<TBase, data_type>::value), "TBase not a base of the data type");
    }

    //*************************************************************************
    /// Construct from iterators
    //*************************************************************************
    template <typename TIterator>
    ETL_CONSTEXPR poly_span(const TIterator begin_, const TIterator /*end_*/)
      : pbegin(etl::addressof(*begin_))
      , element_size(sizeof(typename etl::iterator_traits<TIterator>::value_type))
    {
      typedef typename etl::iterator_traits<TIterator>::value_type        data_type;
      typedef typename etl::iterator_traits<TIterator>::iterator_category iterator_category;

      ETL_STATIC_ASSERT((etl::is_same<ETL_OR_STD::random_access_iterator_tag, iterator_category>::value),    "Not a random access iterator");
      ETL_STATIC_ASSERT((etl::is_base_of<TBase, data_type>::value || etl::is_same<TBase, data_type>::value), "TBase not a base of the data type");
    }

    //*************************************************************************
    /// Construct from C array
    //*************************************************************************
    template<typename U, size_t N>
    ETL_CONSTEXPR poly_span(U(&begin_)[N]) ETL_NOEXCEPT
      : pbegin(begin_)
      , element_size(sizeof(U))
    {
      ETL_STATIC_ASSERT(N <= Extent, "Array data overflow");
      ETL_STATIC_ASSERT((etl::is_base_of<TBase, U>::value || etl::is_same<TBase, U>::value), "TBase not a base of the data type");
    }

    //*************************************************************************
    /// Construct from etl::array.
    //*************************************************************************
    template <typename U, size_t N>
    ETL_CONSTEXPR poly_span(etl::array<U, N>& a) ETL_NOEXCEPT
      : pbegin(a.data())
      , element_size(sizeof(U))
    {
      ETL_STATIC_ASSERT(N <= Extent, "Array data overflow");
      ETL_STATIC_ASSERT((etl::is_base_of<TBase, U>::value || etl::is_same<TBase, U>::value), "TBase not a base of the data type");
    }

    //*************************************************************************
    /// Construct from etl::array.
    //*************************************************************************
    template <typename U, size_t N>
    ETL_CONSTEXPR poly_span(const etl::array<U, N>& a) ETL_NOEXCEPT
      : pbegin(a.data())
      , element_size(sizeof(U))
    {
      ETL_STATIC_ASSERT(N <= Extent, "Array data overflow");
      ETL_STATIC_ASSERT((etl::is_base_of<TBase, U>::value || etl::is_same<TBase, U>::value), "TBase not a base of the data type");
    }

#if ETL_USING_STL && ETL_USING_CPP11
    //*************************************************************************
    /// Construct from std::array.
    //*************************************************************************
    template <typename U, size_t N>
    ETL_CONSTEXPR poly_span(std::array<U, N>& a) ETL_NOEXCEPT
      : pbegin(a.data())
      , element_size(sizeof(U))
    {
      ETL_STATIC_ASSERT(N <= Extent, "Array data overflow");
      ETL_STATIC_ASSERT((etl::is_base_of<TBase, U>::value || etl::is_same<TBase, U>::value), "TBase not a base of U");
    }

    //*************************************************************************
    /// Construct from std::array.
    //*************************************************************************
    template <typename U, size_t N>
    ETL_CONSTEXPR poly_span(const std::array<U, N>& a) ETL_NOEXCEPT
      : pbegin(a.data())
      , element_size(sizeof(U))
    {
      ETL_STATIC_ASSERT(N <= Extent, "Array data overflow");
      ETL_STATIC_ASSERT((etl::is_base_of<TBase, U>::value || etl::is_same<TBase, U>::value), "TBase not a base of U");
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
      return *element_at(Extent - 1U);
    }

    //*************************************************************************
    /// Returns a pointer to the first element of the internal storage.
    //*************************************************************************
    ETL_NODISCARD ETL_CONSTEXPR pointer data() const ETL_NOEXCEPT
    {
      return pbegin;
    }

    //*************************************************************************
    /// Returns an iterator to the beginning of the poly_span.
    //*************************************************************************
    ETL_NODISCARD ETL_CONSTEXPR iterator begin() const ETL_NOEXCEPT
    {
      return iterator(pbegin, 0U, element_size);
    }

    //*************************************************************************
    /// Returns an iterator to the end of the poly_span.
    //*************************************************************************
    ETL_NODISCARD ETL_CONSTEXPR iterator end() const ETL_NOEXCEPT
    {
      return iterator(pbegin, Extent, element_size);
    }

    //*************************************************************************
    // Returns an reverse iterator to the reverse beginning of the poly_span.
    //*************************************************************************
    ETL_CONSTEXPR reverse_iterator rbegin() const ETL_NOEXCEPT
    {
      return reverse_iterator(end());
    }

    //*************************************************************************
    /// Returns a reverse iterator to the end of the poly_span.
    //*************************************************************************
    ETL_CONSTEXPR reverse_iterator rend() const ETL_NOEXCEPT
    {
      return reverse_iterator(begin());
    }

    //*************************************************************************
    /// Returns <b>true</b> if the poly_span size is zero.
    //*************************************************************************
    ETL_NODISCARD ETL_CONSTEXPR bool empty() const ETL_NOEXCEPT
    {
      return (Extent == 0U);
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

    //*************************************************************************
    /// Assign from a poly_span.
    //*************************************************************************
    ETL_CONSTEXPR14 poly_span& operator =(const poly_span& other) ETL_NOEXCEPT
    {
      pbegin = other.pbegin;
      element_size = other.element_size;
      return *this;
    }

    //*************************************************************************
    /// Assign from a poly_span.
    //*************************************************************************
    template <typename UBase>
    ETL_CONSTEXPR14 poly_span& operator =(const poly_span<UBase, Extent>& other) ETL_NOEXCEPT
    {
      pbegin = other.pbegin;
      element_size = other.element_size;
      return *this;
    }

    //*************************************************************************
    /// Returns a reference to the indexed value.
    //*************************************************************************
    ETL_CONSTEXPR reference operator[](size_t i) const
    {
      return *element_at(i);
    }

    //*************************************************************************
    /// Obtains a poly_span that is a view over the first COUNT elements of this poly_span.
    //*************************************************************************
    template <size_t COUNT>
    ETL_NODISCARD ETL_CONSTEXPR etl::poly_span<element_type, COUNT> first() const ETL_NOEXCEPT
    {
      return etl::poly_span<element_type, COUNT>(pbegin, 0U, COUNT, element_size);
    }

    //*************************************************************************
    /// Obtains a poly_span that is a view over the first count elements of this poly_span.
    //*************************************************************************
    ETL_NODISCARD ETL_CONSTEXPR etl::poly_span<element_type, etl::dynamic_extent> first(size_t count) const
    {
      return etl::poly_span<element_type, etl::dynamic_extent>(pbegin, 0U, count, element_size);
    }

    //*************************************************************************
    /// Obtains a poly_span that is a view over the last COUNT elements of this poly_span.
    //*************************************************************************
    template <size_t COUNT>
    ETL_NODISCARD ETL_CONSTEXPR etl::poly_span<element_type, COUNT> last() const ETL_NOEXCEPT
    {
      return etl::poly_span<element_type, COUNT>(pbegin, Extent - COUNT, COUNT, element_size);
    }

    //*************************************************************************
    /// Obtains a poly_span that is a view over the last count elements of this poly_span.
    //*************************************************************************
    ETL_NODISCARD ETL_CONSTEXPR etl::poly_span<element_type, etl::dynamic_extent> last(size_t count) const ETL_NOEXCEPT
    {
      return etl::poly_span<element_type, etl::dynamic_extent>(pbegin, Extent - count, count, element_size);
    }

#if ETL_USING_CPP11
    //*************************************************************************
    /// Obtains a poly_span that is a view from OFFSET over the next COUNT elements of this poly_span.
    //*************************************************************************
    template <const size_t OFFSET, size_t COUNT = etl::dynamic_extent>
    ETL_NODISCARD ETL_CONSTEXPR etl::poly_span<element_type, COUNT != etl::dynamic_extent ? COUNT : Extent - OFFSET> subspan() const ETL_NOEXCEPT
    {
      return (COUNT == etl::dynamic_extent) ? etl::poly_span<element_type, COUNT != etl::dynamic_extent ? COUNT : Extent - OFFSET>(pbegin, OFFSET, Extent, element_size)
                                            : etl::poly_span<element_type, COUNT != etl::dynamic_extent ? COUNT : Extent - OFFSET>(pbegin, OFFSET, COUNT, element_size);
    }
#else
    //*************************************************************************
    /// Obtains a poly_span that is a view from OFFSET over the next COUNT elements of this poly_span.
    //*************************************************************************
    template <const size_t OFFSET, size_t COUNT>
    etl::poly_span<element_type, COUNT != etl::dynamic_extent ? COUNT : Extent - OFFSET> subspan() const
    {
      if (COUNT == etl::dynamic_extent)
      {
        return etl::poly_span<element_type, COUNT != etl::dynamic_extent ? COUNT : Extent - OFFSET>(pbegin, OFFSET, Extent, element_size);
      }
      else
      {
        return etl::poly_span<element_type, COUNT != etl::dynamic_extent ? COUNT : Extent - OFFSET>(pbegin, OFFSET, element_size);
      }
    }
#endif

    //*************************************************************************
    /// Obtains a poly_span that is a view from 'offset' over the next 'count' elements of this poly_span.
    //*************************************************************************
    ETL_NODISCARD ETL_CONSTEXPR etl::poly_span<element_type, etl::dynamic_extent> subspan(size_t offset, size_t count = etl::dynamic_extent) const ETL_NOEXCEPT
    {
      return (count == etl::dynamic_extent) ? etl::poly_span<element_type, etl::dynamic_extent>(pbegin, offset, Extent, element_size)
                                            : etl::poly_span<element_type, etl::dynamic_extent>(pbegin, offset, count, element_size);
    }

  protected:

    //***************************************************************************
    template <typename TFrom>
    struct char_ptr_type
    {
      typedef typename etl::conditional<etl::is_const<TFrom>::value, const char*, char*>::type type;
    };

    typedef typename char_ptr_type<TBase>::type char_ptr_t;

    //***************************************************************************
    ETL_CONSTEXPR pointer element_at(size_t index) const ETL_NOEXCEPT
    {
      char_ptr_t base = reinterpret_cast<char_ptr_t>(pbegin);
      return reinterpret_cast<pointer>(base + (index * element_size));
    }

    //*************************************************************************
    /// Construct from iterator + offset + element size
    /// extent_ is ignored.
    //*************************************************************************
    ETL_CONSTEXPR poly_span(TBase* pbegin_, size_t offset_, size_t /*extent_*/, size_t element_size_) ETL_NOEXCEPT
      : pbegin(reinterpret_cast<pointer>(reinterpret_cast<char_ptr_t>(pbegin_) + (offset_ * element_size_)))
      , element_size(element_size_)
    {
    }

  private:

    pointer pbegin;
    size_t  element_size;
  };

  //***************************************************************************
  /// Poly Span - Dynamic Extent
  //***************************************************************************
  template <typename TBase>
  class poly_span<TBase, etl::dynamic_extent>
  {
  public:

    typedef TBase        element_type;
    typedef typename etl::remove_cv<TBase>::type value_type;
    typedef size_t       size_type;
    typedef TBase&       reference;
    typedef const TBase& const_reference;
    typedef TBase*       pointer;
    typedef const TBase* const_pointer;
    typedef etl::private_poly_span::iterator<TBase> iterator;
    typedef ETL_OR_STD::reverse_iterator<iterator> reverse_iterator;
 
    static ETL_CONSTANT size_t extent = etl::dynamic_extent;

    template <typename UBase, size_t UExtent>
    friend class poly_span;

    //*************************************************************************
    /// Default constructor.
    //*************************************************************************
    ETL_CONSTEXPR poly_span() ETL_NOEXCEPT
      : pbegin(ETL_NULLPTR)
      , element_size(0U)
      , span_extent(0U)
    {
    }

    //*************************************************************************
    /// Construct from iterator + size
    //*************************************************************************
    template <typename TIterator, typename TSize>
    ETL_CONSTEXPR poly_span(const TIterator begin_, const TSize size_) ETL_NOEXCEPT
      : pbegin(etl::addressof(*begin_))
      , element_size(sizeof(typename etl::iterator_traits<TIterator>::value_type))
      , span_extent(size_)
    {
      typedef typename etl::iterator_traits<TIterator>::value_type        data_type;
      typedef typename etl::iterator_traits<TIterator>::iterator_category iterator_category;

      ETL_STATIC_ASSERT((etl::is_same<ETL_OR_STD::random_access_iterator_tag, iterator_category>::value), "Not a random access iterator");
      ETL_STATIC_ASSERT((etl::is_base_of<TBase, data_type>::value || etl::is_same<TBase, data_type>::value), "TBase not a base of the data type");
    }

    //*************************************************************************
    /// Construct from iterators
    //*************************************************************************
    template <typename TIterator>
    ETL_CONSTEXPR poly_span(const TIterator begin_, const TIterator end_)
      : pbegin(etl::addressof(*begin_))
      , element_size(sizeof(typename etl::iterator_traits<TIterator>::value_type))
      , span_extent(size_t(etl::distance(begin_, end_)))
    {
      typedef typename etl::iterator_traits<TIterator>::value_type        data_type;
      typedef typename etl::iterator_traits<TIterator>::iterator_category iterator_category;

      ETL_STATIC_ASSERT((etl::is_same<ETL_OR_STD::random_access_iterator_tag, iterator_category>::value), "Not a random access iterator");
      ETL_STATIC_ASSERT((etl::is_base_of<TBase, data_type>::value || etl::is_same<TBase, data_type>::value), "TBase not a base of the data type");
    }

    //*************************************************************************
    /// Construct from C array
    //*************************************************************************
    template<typename U, size_t N>
    ETL_CONSTEXPR poly_span(U(&begin_)[N]) ETL_NOEXCEPT
      : pbegin(begin_)
      , element_size(sizeof(U))
      , span_extent(N)
    {
      ETL_STATIC_ASSERT((etl::is_base_of<TBase, U>::value || etl::is_same<TBase, U>::value), "TBase not a base of the data type");
    }

    //*************************************************************************
    /// Construct from etl::array.
    //*************************************************************************
    template <typename U, size_t N>
    ETL_CONSTEXPR poly_span(etl::array<U, N>& a) ETL_NOEXCEPT
      : pbegin(a.data())
      , element_size(sizeof(U))
      , span_extent(N)
    {
      ETL_STATIC_ASSERT((etl::is_base_of<TBase, U>::value || etl::is_same<TBase, U>::value), "TBase not a base of the data type");
    }

    //*************************************************************************
    /// Construct from etl::array.
    //*************************************************************************
    template <typename U, size_t N>
    ETL_CONSTEXPR poly_span(const etl::array<U, N>& a) ETL_NOEXCEPT
      : pbegin(a.data())
      , element_size(sizeof(U))
      , span_extent(N)
    {
      ETL_STATIC_ASSERT((etl::is_base_of<TBase, U>::value || etl::is_same<TBase, U>::value), "TBase not a base of the data type");
    }

#if ETL_USING_STL && ETL_USING_CPP11
    //*************************************************************************
    /// Construct from std::array.
    //*************************************************************************
    template <typename U, size_t N>
    ETL_CONSTEXPR poly_span(std::array<U, N>& a) ETL_NOEXCEPT
      : pbegin(a.data())
      , element_size(sizeof(U))
      , span_extent(N)
    {
      ETL_STATIC_ASSERT((etl::is_base_of<TBase, U>::value || etl::is_same<TBase, U>::value), "TBase not a base of U");
    }

    //*************************************************************************
    /// Construct from std::array.
    //*************************************************************************
    template <typename U, size_t N>
    ETL_CONSTEXPR poly_span(const std::array<U, N>& a) ETL_NOEXCEPT
      : pbegin(a.data())
      , element_size(sizeof(U))
      , span_extent(N)
    {
      ETL_STATIC_ASSERT((etl::is_base_of<TBase, U>::value || etl::is_same<TBase, U>::value), "TBase not a base of U");
    }
#endif

    //*************************************************************************
    /// Copy constructor
    //*************************************************************************
    ETL_CONSTEXPR poly_span(const poly_span<TBase, etl::dynamic_extent>& other) ETL_NOEXCEPT
      : pbegin(other.pbegin)
      , element_size(other.element_size)
      , span_extent(other.span_extent)
    {
    }

    //*************************************************************************
    /// Copy constructor
    //*************************************************************************
    template <typename UBase>
    ETL_CONSTEXPR poly_span(const poly_span<UBase, etl::dynamic_extent>& other) ETL_NOEXCEPT
      : pbegin(other.pbegin)
      , element_size(other.element_size)
      , span_extent(other.span_extent)
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
      return *element_at(span_extent - 1U);
    }

    //*************************************************************************
    /// Returns a pointer to the first element of the internal storage.
    //*************************************************************************
    ETL_NODISCARD ETL_CONSTEXPR pointer data() const ETL_NOEXCEPT
    {
      return pbegin;
    }

    //*************************************************************************
    /// Returns an iterator to the beginning of the poly_span.
    //*************************************************************************
    ETL_NODISCARD ETL_CONSTEXPR iterator begin() const ETL_NOEXCEPT
    {
      return iterator(pbegin, 0U, element_size);
    }

    //*************************************************************************
    /// Returns an iterator to the end of the poly_span.
    //*************************************************************************
    ETL_NODISCARD ETL_CONSTEXPR iterator end() const ETL_NOEXCEPT
    {
      return iterator(pbegin, span_extent, element_size);
    }

    //*************************************************************************
    // Returns an reverse iterator to the reverse beginning of the poly_span.
    //*************************************************************************
    ETL_NODISCARD ETL_CONSTEXPR reverse_iterator rbegin() const ETL_NOEXCEPT
    {
      return reverse_iterator(end());
    }

    //*************************************************************************
    /// Returns a reverse iterator to the end of the poly_span.
    //*************************************************************************
    ETL_NODISCARD ETL_CONSTEXPR reverse_iterator rend() const ETL_NOEXCEPT
    {
      return reverse_iterator(begin());
    }

    //*************************************************************************
    /// Returns <b>true</b> if the poly_span size is zero.
    //*************************************************************************
    ETL_NODISCARD ETL_CONSTEXPR bool empty() const ETL_NOEXCEPT
    {
      return (span_extent == 0);
    }

    //*************************************************************************
    /// Returns the size of the poly_span.
    //*************************************************************************
    ETL_NODISCARD ETL_CONSTEXPR size_t size() const ETL_NOEXCEPT
    {
      return span_extent;
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
      return element_size * span_extent;
    }

    //*************************************************************************
    /// Assign from a poly_span.
    //*************************************************************************
    ETL_CONSTEXPR14 poly_span& operator =(const poly_span& other) ETL_NOEXCEPT
    {
      pbegin = other.pbegin;
      element_size = other.element_size;
      span_extent = other.span_extent;
      return *this;
    }

    //*************************************************************************
    /// Assign from a poly_span.
    //*************************************************************************
    template <typename UBase>
    ETL_CONSTEXPR14 poly_span& operator =(const poly_span<UBase, etl::dynamic_extent>& other) ETL_NOEXCEPT
    {
      pbegin = other.pbegin;
      element_size = other.element_size;
      span_extent = other.span_extent;
      return *this;
    }

    //*************************************************************************
    /// Returns a reference to the indexed value.
    //*************************************************************************
    ETL_CONSTEXPR reference operator[](size_t i) const
    {
      return *element_at(i);
    }

    //*************************************************************************
    /// Obtains a poly_span that is a view over the first COUNT elements of this poly_span.
    //*************************************************************************
    template <size_t COUNT>
    ETL_NODISCARD ETL_CONSTEXPR etl::poly_span<element_type, COUNT> first() const ETL_NOEXCEPT
    {
      return etl::poly_span<element_type, COUNT>(pbegin, 0U, COUNT, element_size);
    }

    //*************************************************************************
    /// Obtains a poly_span that is a view over the first count elements of this poly_span.
    //*************************************************************************
    ETL_NODISCARD ETL_CONSTEXPR etl::poly_span<element_type, etl::dynamic_extent> first(size_t count) const ETL_NOEXCEPT
    {
      return etl::poly_span<element_type, etl::dynamic_extent>(pbegin, 0U, count, element_size);
    }

    //*************************************************************************
    /// Obtains a poly_span that is a view over the last COUNT elements of this poly_span.
    //*************************************************************************
    template <size_t COUNT>
    ETL_NODISCARD ETL_CONSTEXPR etl::poly_span<element_type, COUNT> last() const ETL_NOEXCEPT
    {
      return etl::poly_span<element_type, COUNT>(pbegin, span_extent - COUNT, COUNT, element_size);
    }

    //*************************************************************************
    /// Obtains a poly_span that is a view over the last count elements of this poly_span.
    //*************************************************************************
    ETL_NODISCARD ETL_CONSTEXPR etl::poly_span<element_type, etl::dynamic_extent> last(size_t count) const ETL_NOEXCEPT
    {
      return etl::poly_span<element_type, etl::dynamic_extent>(pbegin, span_extent - count, count, element_size);
    }

#if ETL_USING_CPP11
    //*************************************************************************
    /// Obtains a poly_span that is a view from OFFSET over the next COUNT elements of this poly_span.
    //*************************************************************************
    template <const size_t OFFSET, size_t COUNT = etl::dynamic_extent>
    ETL_NODISCARD ETL_CONSTEXPR etl::poly_span<element_type, COUNT != etl::dynamic_extent ? COUNT : etl::dynamic_extent> subspan() const ETL_NOEXCEPT
    {
      return (COUNT == etl::dynamic_extent) ? etl::poly_span<element_type, COUNT != etl::dynamic_extent ? COUNT : etl::dynamic_extent>(pbegin, OFFSET, span_extent, element_size)
                                            : etl::poly_span<element_type, COUNT != etl::dynamic_extent ? COUNT : etl::dynamic_extent>(pbegin, OFFSET, COUNT, element_size);
    }
#else
    //*************************************************************************
    /// Obtains a poly_span that is a view from OFFSET over the next COUNT elements of this poly_span.
    //*************************************************************************
    template <const size_t OFFSET, size_t COUNT>
    etl::poly_span<element_type, COUNT != etl::dynamic_extent ? COUNT : etl::dynamic_extent> subspan() const
    {
      if (COUNT == etl::dynamic_extent)
      {
        return etl::poly_span<element_type, COUNT != etl::dynamic_extent ? COUNT : etl::dynamic_extent>(pbegin, OFFSET, span_extent, element_size);
      }
      else
      {
        return etl::poly_span<element_type, COUNT != etl::dynamic_extent ? COUNT : etl::dynamic_extent>(pbegin, OFFSET, span_extent, element_size);
      }
    }
#endif

    //*************************************************************************
    /// Obtains a poly_span that is a view from 'offset' over the next 'count' elements of this poly_span.
    //*************************************************************************
    ETL_NODISCARD ETL_CONSTEXPR etl::poly_span<element_type, etl::dynamic_extent> subspan(size_t offset, size_t count = etl::dynamic_extent) const ETL_NOEXCEPT
    {
      return (count == etl::dynamic_extent) ? etl::poly_span<element_type, etl::dynamic_extent>(pbegin, offset, span_extent - offset, element_size)
                                            : etl::poly_span<element_type, etl::dynamic_extent>(pbegin, offset, count, element_size);
    }

protected:

  //*************************************************************************
  /// Construct from iterator + offset + size + element size
  //*************************************************************************
  ETL_CONSTEXPR poly_span(TBase* pbegin_, size_t offset_, size_t extent_, size_t element_size_) ETL_NOEXCEPT
    : pbegin(reinterpret_cast<pointer>(reinterpret_cast<char_ptr_t>(pbegin_) + (offset_ * element_size_)))
    , element_size(element_size_)
    , span_extent(extent_)
  {
  }

  private:

    //***************************************************************************
    template <typename TFrom>
    struct char_ptr_type
    {
      typedef typename etl::conditional<etl::is_const<TFrom>::value, const char*, char*>::type type;
    };

    typedef typename char_ptr_type<TBase>::type char_ptr_t;

    //***************************************************************************
    ETL_CONSTEXPR pointer element_at(size_t index) const ETL_NOEXCEPT
    {
      char_ptr_t base = reinterpret_cast<char_ptr_t>(pbegin);
      return reinterpret_cast<pointer>(base + (index * element_size));
    }

    pointer pbegin;
    size_t  element_size;
    size_t  span_extent;
  };

  //*************************************************************************
  /// Template deduction guides.
  //*************************************************************************
#if ETL_USING_CPP17
  template <typename TIterator>
  poly_span(const TIterator begin_, const TIterator end_)
    ->poly_span<etl::remove_pointer_t<TIterator>, etl::dynamic_extent>;

  template <typename TIterator, typename TSize>
  poly_span(const TIterator begin_, const TSize size_)
    ->poly_span<etl::remove_pointer_t<TIterator>, etl::dynamic_extent>;

  template <typename T, size_t N>
  poly_span(T(&)[N])
    ->poly_span<T, N>;

  template <typename T, size_t N>
  poly_span(etl::array<T, N>&)
    ->poly_span<T, N>;

  template <typename T, size_t N>
  poly_span(const etl::array<T, N>&)
    ->poly_span<const T, N>;

#if ETL_USING_STL
  template <typename T, size_t N>
  poly_span(std::array<T, N>&)
    ->poly_span<T, N>;

  template <typename T, size_t N>
  poly_span(const std::array<T, N>&)
    ->poly_span<const T, N>;
#endif
#endif 

  //*************************************************************************
  /// Hash function.
  //*************************************************************************
#if ETL_USING_8BIT_TYPES
  template <typename TBase, size_t Extent>
  struct hash<etl::poly_span<TBase, Extent> >
  {
    size_t operator()(const etl::poly_span<TBase, Extent>& view) const
    {
      return etl::private_hash::generic_hash<size_t>(reinterpret_cast<const uint8_t*>(&view[0]),
                                                     reinterpret_cast<const uint8_t*>(&view[view.size()]));
    }
  };
#endif
}

#endif
