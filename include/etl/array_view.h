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

#ifndef ETL_ARRAY_VIEW_INCLUDED
#define ETL_ARRAY_VIEW_INCLUDED

#include "platform.h"
#include "memory.h"
#include "iterator.h"
#include "error_handler.h"
#include "exception.h"
#include "nullptr.h"
#include "hash.h"
#include "algorithm.h"

#include "stl/algorithm.h"

///\defgroup array array
/// A wrapper for arrays
///\ingroup containers

#undef ETL_FILE
#define ETL_FILE "41"

namespace etl
{
  //***************************************************************************
  /// The base class for array_view exceptions.
  //***************************************************************************
  class array_view_exception : public exception
  {
  public:

    array_view_exception(string_type reason_, string_type file_name_, numeric_type line_number_)
      : exception(reason_, file_name_, line_number_)
    {
    }
  };

  //***************************************************************************
  ///\ingroup stack
  /// The exception thrown when the index is out of bounds.
  //***************************************************************************
  class array_view_bounds : public array_view_exception
  {
  public:

    array_view_bounds(string_type file_name_, numeric_type line_number_)
      : array_view_exception(ETL_ERROR_TEXT("array_view:bounds", ETL_FILE"A"), file_name_, line_number_)
    {
    }
  };

  //***************************************************************************
  ///\ingroup stack
  /// The exception thrown when the view is uninitialised.
  //***************************************************************************
  class array_view_uninitialised : public array_view_exception
  {
  public:

    array_view_uninitialised(string_type file_name_, numeric_type line_number_)
      : array_view_exception(ETL_ERROR_TEXT("array_view:uninitialised", ETL_FILE"B"), file_name_, line_number_)
    {
    }
  };

  //***************************************************************************
  /// Array view.
  //***************************************************************************
  template <typename T>
  class array_view
  {
  public:

    typedef T                                     value_type;
    typedef size_t                           size_type;
    typedef T&                                    reference;
    typedef const T&                              const_reference;
    typedef T*                                    pointer;
    typedef const T*                              const_pointer;
    typedef T*                                    iterator;
    typedef const T*                              const_iterator;
    typedef std::reverse_iterator<iterator>       reverse_iterator;
    typedef std::reverse_iterator<const_iterator> const_reverse_iterator;

    //*************************************************************************
    /// Default constructor.
    //*************************************************************************
    ETL_CONSTEXPR array_view()
      : mbegin(nullptr),
        mend(nullptr)
    {
    }

    //*************************************************************************
    /// Construct from std::array or etl::array or other type that supports
    /// data() and size() member functions.
    //*************************************************************************
    template <typename TArray>
    ETL_CONSTEXPR explicit array_view(TArray& a)
      : mbegin(a.data()),
        mend(a.data() + a.size())
    {
    }

    //*************************************************************************
    /// Construct from iterators
    //*************************************************************************
    template <typename TIterator>
    ETL_CONSTEXPR array_view(TIterator begin_, TIterator end_)
      : mbegin(etl::addressof(*begin_)),
        mend(etl::addressof(*begin_) + std::distance(begin_, end_))
    {
    }

    //*************************************************************************
    /// Construct from C array
    //*************************************************************************
    template <typename TIterator, 
              typename TSize>
    ETL_CONSTEXPR array_view(TIterator begin_, TSize size_)
      : mbegin(etl::addressof(*begin_)),
        mend(etl::addressof(*begin_) + size_)
    {
    }
    
    //*************************************************************************
    /// Construct from C array
    //*************************************************************************
    template<const size_t ARRAY_SIZE>
    ETL_CONSTEXPR explicit array_view(T(&begin_)[ARRAY_SIZE])
      : mbegin(begin_),
        mend(begin_ + ARRAY_SIZE)
    {
    }

    //*************************************************************************
    /// Copy constructor
    //*************************************************************************
    ETL_CONSTEXPR array_view(const array_view& other)
      : mbegin(other.mbegin),
        mend(other.mend)
    {
    }

    //*************************************************************************
    /// Returns a reference to the first element.
    //*************************************************************************
    reference front()
    {
      return *mbegin;
    }

    //*************************************************************************
    /// Returns a const reference to the first element.
    //*************************************************************************
    const_reference front() const
    {
      return *mbegin;
    }

    //*************************************************************************
    /// Returns a reference to the last element.
    //*************************************************************************
    reference back()
    {
      return *(mend - 1);
    }

    //*************************************************************************
    /// Returns a const reference to the last element.
    //*************************************************************************
    const_reference back() const
    {
      return *(mend - 1);
    }

    //*************************************************************************
    /// Returns a pointer to the first element of the internal storage.
    //*************************************************************************
    pointer data()
    {
      return mbegin;
    }

    //*************************************************************************
    /// Returns a const pointer to the first element of the internal storage.
    //*************************************************************************
    const_pointer data() const
    {
      return mbegin;
    }

    //*************************************************************************
    /// Returns an iterator to the beginning of the array.
    //*************************************************************************
    iterator begin()
    {
      return mbegin;
    }

    //*************************************************************************
    /// Returns a const iterator to the beginning of the array.
    //*************************************************************************
    const_iterator begin() const
    {
      return mbegin;
    }

    //*************************************************************************
    /// Returns a const iterator to the beginning of the array.
    //*************************************************************************
    const_iterator cbegin() const
    {
      return mbegin;
    }

    //*************************************************************************
    /// Returns an iterator to the end of the array.
    //*************************************************************************
    iterator end()
    {
      return mend;
    }

    //*************************************************************************
    /// Returns a const iterator to the end of the array.
    //*************************************************************************
    const_iterator end() const
    {
      return mend;
    }

    //*************************************************************************
    // Returns a const iterator to the end of the array.
    //*************************************************************************
    const_iterator cend() const
    {
      return mend;
    }

    //*************************************************************************
    // Returns an reverse iterator to the reverse beginning of the array.
    //*************************************************************************
    reverse_iterator rbegin()
    {
      return reverse_iterator(mend);
    }

    //*************************************************************************
    /// Returns a const reverse iterator to the reverse beginning of the array.
    //*************************************************************************
    const_reverse_iterator rbegin() const
    {
      return const_reverse_iterator(mend);
    }

    //*************************************************************************
    /// Returns a const reverse iterator to the reverse beginning of the array.
    //*************************************************************************
    const_reverse_iterator crbegin() const
    {
      return const_reverse_iterator(mend);
    }

    //*************************************************************************
    /// Returns a reverse iterator to the end of the array.
    //*************************************************************************
    reverse_iterator rend()
    {
      return reverse_iterator(mbegin);
    }

    //*************************************************************************
    /// Returns a const reverse iterator to the end of the array.
    //*************************************************************************
    const_reverse_iterator rend() const
    {
      return const_reverse_iterator(mbegin);
    }

    //*************************************************************************
    /// Returns a const reverse iterator to the end of the array.
    //*************************************************************************
    const_reverse_iterator crend() const
    {
      return const_reverse_iterator(mbegin);
    }

    //*************************************************************************
    /// Returns <b>true</b> if the array size is zero.
    //*************************************************************************
    bool empty() const
    {
      return (mbegin == mend);
    }

    //*************************************************************************
    /// Returns the size of the array.
    //*************************************************************************
    size_t size() const
    {
      return (mend - mbegin);
    }

    //*************************************************************************
    /// Returns the maximum possible size of the array.
    //*************************************************************************
    size_t max_size() const
    {
      return size();
    }

    //*************************************************************************
    /// Assign from a view.
    //*************************************************************************
    array_view& operator=(const array_view& other)
    {
      mbegin = other.mbegin;
      mend   = other.mend;
      return *this;
    }

    //*************************************************************************
    /// Assign from iterators
    //*************************************************************************
    template <typename TIterator>
    void assign(TIterator begin_, TIterator end_)
    {
      mbegin = etl::addressof(*begin_);
      mend   = etl::addressof(*begin_) + std::distance(begin_, end_);
    }

    //*************************************************************************
    /// Assign from iterator and size.
    //*************************************************************************
    template <typename TIterator,
              typename TSize>
    void assign(TIterator begin_, TSize size_)
    {
      mbegin = etl::addressof(*begin_);
      mend   = etl::addressof(*begin_) + size_;
    }

    //*************************************************************************
    /// Returns a reference to the indexed value.
    //*************************************************************************
    reference operator[](size_t i)
    {
      return mbegin[i];
    }

    //*************************************************************************
    /// Returns a const reference to the indexed value.
    //*************************************************************************
    const_reference operator[](size_t i) const
    {
      return mbegin[i];
    }

    //*************************************************************************
    /// Returns a reference to the indexed value.
    //*************************************************************************
    reference at(size_t i)
    {
      ETL_ASSERT((mbegin != nullptr && mend != nullptr), ETL_ERROR(array_view_uninitialised));
      ETL_ASSERT(i < size(), ETL_ERROR(array_view_bounds));
      return mbegin[i];
    }

    //*************************************************************************
    /// Returns a const reference to the indexed value.
    //*************************************************************************
    const_reference at(size_t i) const
    {
      ETL_ASSERT((mbegin != nullptr && mend != nullptr), ETL_ERROR(array_view_uninitialised));
      ETL_ASSERT(i < size(), ETL_ERROR(array_view_bounds));
      return mbegin[i];
    }

    //*************************************************************************
    /// Swaps with another array_view.
    //*************************************************************************
    void swap(array_view& other)
    {
      std::swap(mbegin, other.mbegin);
      std::swap(mend, other.mend);
    }

    //*************************************************************************          
    /// Shrinks the view by moving its start forward.
    //*************************************************************************          
    void remove_prefix(size_type n)
    {
      mbegin += n;
    }

    //*************************************************************************          
    /// Shrinks the view by moving its end backward.
    //*************************************************************************          
    void remove_suffix(size_type n)
    {
      mend -= n;
    }

    //*************************************************************************
    /// Equality for array views.
    //*************************************************************************
    friend bool operator == (const array_view<T>& lhs, const array_view<T>& rhs)
    {
      return (lhs.size() == rhs.size()) &&
             std::equal(lhs.begin(), lhs.end(), rhs.begin());
    }

    //*************************************************************************
    /// Inequality for array views.
    //*************************************************************************
    friend bool operator != (const array_view<T>& lhs, const array_view<T>& rhs)
    {
      return !(lhs == rhs);
    }

    //*************************************************************************
    /// Less-than for array views.
    //*************************************************************************
    friend bool operator < (const array_view<T>& lhs, const array_view<T>& rhs)
    {
      return std::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
    }

    //*************************************************************************
    /// Greater-than for array views.
    //*************************************************************************
    friend bool operator > (const array_view<T>& lhs, const array_view<T>& rhs)
    {
      return rhs < lhs;
    }

    //*************************************************************************
    /// Less-than-equal for array views.
    //*************************************************************************
    friend bool operator <= (const array_view<T>& lhs, const array_view<T>& rhs)
    {
      return !(lhs > rhs);
    }

    //*************************************************************************
    /// Greater-than-equal for array views.
    //*************************************************************************
    friend bool operator >= (const array_view<T>& lhs, const array_view<T>& rhs)
    {
      return !(lhs < rhs);
    }

  private:

    T* mbegin;
    T* mend;
  };

  //***************************************************************************
  /// Constant array view.
  //***************************************************************************
  template <typename T>
  class const_array_view
  {
  public:

    typedef T                                     value_type;
    typedef size_t                           size_type;
    typedef const T&                              const_reference;
    typedef const T*                              const_pointer;
    typedef const T*                              const_iterator;
    typedef std::reverse_iterator<const_iterator> const_reverse_iterator;

    //*************************************************************************
    /// Default constructor.
    //*************************************************************************
    ETL_CONSTEXPR const_array_view()
      : mbegin(nullptr),
        mend(nullptr)
    {
    }

    //*************************************************************************
    /// Construct from std::array or etl::array or other type that supports
    /// data() and size() member functions.
    //*************************************************************************
    template <typename TArray>
    ETL_CONSTEXPR explicit const_array_view(TArray& a)
      : mbegin(a.data()),
        mend(a.data() + a.size())
    {
    }

    //*************************************************************************
    /// Construct from iterators
    //*************************************************************************
    template <typename TIterator>
    ETL_CONSTEXPR const_array_view(TIterator begin_, TIterator end_)
      : mbegin(etl::addressof(*begin_)),
        mend(etl::addressof(*begin_) + std::distance(begin_, end_))
    {
    }

    //*************************************************************************
    /// Construct from C array
    //*************************************************************************
    template <typename TIterator, 
              typename TSize>
    ETL_CONSTEXPR const_array_view(TIterator begin_, TSize size_)
      : mbegin(etl::addressof(*begin_)),
        mend(etl::addressof(*begin_) + size_)
    {
    }

    //*************************************************************************
    /// Construct from C array
    //*************************************************************************
    template<const size_t ARRAY_SIZE>
    ETL_CONSTEXPR explicit const_array_view(const T(&begin_)[ARRAY_SIZE])
      : mbegin(begin_),
        mend(begin_ + ARRAY_SIZE)
    {
    }

    //************0*************************************************************
    /// Copy constructor
    //*************************************************************************
    ETL_CONSTEXPR const_array_view(const array_view<T>& other)
      : mbegin(other.begin()),
        mend(other.end())
    {
    }

    //*************************************************************************
    /// Copy constructor
    //*************************************************************************
    ETL_CONSTEXPR const_array_view(const const_array_view& other)
      : mbegin(other.mbegin),
        mend(other.mend)
    {
    }

    //*************************************************************************
    /// Returns a const reference to the first element.
    //*************************************************************************
    const_reference front() const
    {
      return *mbegin;
    }

    //*************************************************************************
    /// Returns a const reference to the last element.
    //*************************************************************************
    const_reference back() const
    {
      return *(mend - 1);
    }

    //*************************************************************************
    /// Returns a const pointer to the first element of the internal storage.
    //*************************************************************************
    const_pointer data() const
    {
      return mbegin;
    }

    //*************************************************************************
    /// Returns a const iterator to the beginning of the array.
    //*************************************************************************
    const_iterator begin() const
    {
      return mbegin;
    }

    //*************************************************************************
    /// Returns a const iterator to the beginning of the array.
    //*************************************************************************
    const_iterator cbegin() const
    {
      return mbegin;
    }

    //*************************************************************************
    /// Returns a const iterator to the end of the array.
    //*************************************************************************
    const_iterator end() const
    {
      return mend;
    }

    //*************************************************************************
    // Returns a const iterator to the end of the array.
    //*************************************************************************
    const_iterator cend() const
    {
      return mend;
    }

    //*************************************************************************
    /// Returns a const reverse iterator to the reverse beginning of the array.
    //*************************************************************************
    const_reverse_iterator rbegin() const
    {
      return const_reverse_iterator(mend);
    }

    //*************************************************************************
    /// Returns a const reverse iterator to the reverse beginning of the array.
    //*************************************************************************
    const_reverse_iterator crbegin() const
    {
      return const_reverse_iterator(mend);
    }

    //*************************************************************************
    /// Returns a const reverse iterator to the end of the array.
    //*************************************************************************
    const_reverse_iterator rend() const
    {
      return const_reverse_iterator(mbegin);
    }

    //*************************************************************************
    /// Returns a const reverse iterator to the end of the array.
    //*************************************************************************
    const_reverse_iterator crend() const
    {
      return const_reverse_iterator(mbegin);
    }

    //*************************************************************************
    // Capacity
    //*************************************************************************

    //*************************************************************************
    /// Returns <b>true</b> if the array size is zero.
    //*************************************************************************
    bool empty() const
    {
      return (mbegin == mend);
    }

    //*************************************************************************
    /// Returns the size of the array.
    //*************************************************************************
    size_t size() const
    {
      return (mend - mbegin);
    }

    //*************************************************************************
    /// Returns the maximum possible size of the array.
    //*************************************************************************
    size_t max_size() const
    {
      return size();
    }

    //*************************************************************************
    /// Assign from a view.
    //*************************************************************************
    const_array_view<T>& operator=(const const_array_view<T>& other)
    {
      mbegin = other.mbegin;
      mend   = other.mend;
      return *this;
    }

    //*************************************************************************
    /// Assign from iterators
    //*************************************************************************
    template <typename TIterator>
    void assign(TIterator begin_, TIterator end_)
    {
      mbegin = etl::addressof(*begin_);
      mend   = etl::addressof(*begin_) + std::distance(begin_, end_);
    }

    //*************************************************************************
    /// Assign from iterator and size.
    //*************************************************************************
    template <typename TIterator,
              typename TSize>
    void assign(TIterator begin_, TSize size_)
    {
      mbegin = etl::addressof(*begin_);
      mend   = etl::addressof(*begin_) + size_;
    }

    //*************************************************************************
    /// Returns a const reference to the indexed value.
    //*************************************************************************
    const_reference operator[](size_t i) const
    {
      return mbegin[i];
    }

    //*************************************************************************
    /// Returns a const reference to the indexed value.
    //*************************************************************************
    const_reference at(size_t i) const
    {
      ETL_ASSERT((mbegin != nullptr && mend != nullptr), ETL_ERROR(array_view_uninitialised));
      ETL_ASSERT(i < size(), ETL_ERROR(array_view_bounds));
      return mbegin[i];
    }

    //*************************************************************************
    /// Swaps with another array_view.
    //*************************************************************************
    void swap(const_array_view& other)
    {
      std::swap(mbegin, other.mbegin);
      std::swap(mend, other.mend);
    }

    //*************************************************************************          
    /// Shrinks the view by moving its start forward.
    //*************************************************************************          
    void remove_prefix(size_type n)
    {
      mbegin += n;
    }

    //*************************************************************************          
    /// Shrinks the view by moving its end backward.
    //*************************************************************************          
    void remove_suffix(size_type n)
    {
      mend -= n;
    }

    //*************************************************************************
    /// Equality for array views.
    //*************************************************************************
    friend bool operator == (const const_array_view<T>& lhs, const const_array_view<T>& rhs)
    {
      return (lhs.size() == rhs.size()) &&
        std::equal(lhs.begin(), lhs.end(), rhs.begin());
    }

    //*************************************************************************
    /// Inequality for array views.
    //*************************************************************************
    friend bool operator != (const const_array_view<T>& lhs, const const_array_view<T>& rhs)
    {
      return !(lhs == rhs);
    }

    //*************************************************************************
    /// Less-than for array views.
    //*************************************************************************
    friend bool operator < (const const_array_view<T>& lhs, const const_array_view<T>& rhs)
    {
      return std::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
    }

    //*************************************************************************
    /// Greater-than for array views.
    //*************************************************************************
    friend bool operator > (const const_array_view<T>& lhs, const const_array_view<T>& rhs)
    {
      return rhs < lhs;
    }

    //*************************************************************************
    /// Less-than-equal for array views.
    //*************************************************************************
    friend bool operator <= (const const_array_view<T>& lhs, const const_array_view<T>& rhs)
    {
      return !(lhs > rhs);
    }

    //*************************************************************************
    /// Greater-than-equal for array views.
    //*************************************************************************
    friend bool operator >= (const const_array_view<T>& lhs, const const_array_view<T>& rhs)
    {
      return !(lhs < rhs);
    }

  private:

    const T* mbegin;
    const T* mend;
  };

  //*************************************************************************
  /// Hash function.
  //*************************************************************************
#if ETL_8BIT_SUPPORT
  template <typename T>
  struct hash<etl::array_view<T> >
  {
    size_t operator()(const etl::array_view<T>& view) const
    {
      return etl::private_hash::generic_hash<size_t>(reinterpret_cast<const uint8_t*>(&view[0]),
                                                     reinterpret_cast<const uint8_t*>(&view[view.size()]));
    }
  };
 
  template <typename T>
  struct hash<etl::const_array_view<T> >
  {
    size_t operator()(const etl::const_array_view<T>& view) const
    {
      return etl::private_hash::generic_hash<size_t>(reinterpret_cast<const uint8_t*>(&view[0]),
                                                     reinterpret_cast<const uint8_t*>(&view[view.size()]));
    }
  };
#endif
}

//*************************************************************************
/// Swaps the values.
//*************************************************************************
template <typename T>
void swap(etl::array_view<T>& lhs, etl::array_view<T>& rhs)
{
  lhs.swap(rhs);
}

//*************************************************************************
/// Swaps the values.
//*************************************************************************
template <typename T>
void swap(etl::const_array_view<T>& lhs, etl::const_array_view<T>& rhs)
{
  lhs.swap(rhs);
}

#undef ETL_FILE

#endif

