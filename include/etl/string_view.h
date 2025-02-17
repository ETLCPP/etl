///\file

/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl
https://www.etlcpp.com

Copyright(c) 2017 John Wellbelove

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

#ifndef ETL_STRING_VIEW_INCLUDED
#define ETL_STRING_VIEW_INCLUDED

#include "platform.h"
#include "memory.h"
#include "iterator.h"
#include "error_handler.h"
#include "exception.h"
#include "char_traits.h"
#include "integral_limits.h"
#include "hash.h"
#include "basic_string.h"
#include "algorithm.h"
#include "private/minmax_push.h"

#if ETL_USING_STL && ETL_USING_CPP17
  #include <string_view>
#endif

#include <stdint.h>

namespace etl
{
  //***************************************************************************
  /// The base class for basic_string_view exceptions.
  //***************************************************************************
  class string_view_exception : public exception
  {
  public:

    string_view_exception(string_type reason_, string_type file_name_, numeric_type line_number_)
      : exception(reason_, file_name_, line_number_)
    {
    }
  };

  //***************************************************************************
  ///\ingroup stack
  /// The exception thrown when the index is out of bounds.
  //***************************************************************************
  class string_view_bounds : public string_view_exception
  {
  public:

    string_view_bounds(string_type file_name_, numeric_type line_number_)
      : string_view_exception(ETL_ERROR_TEXT("basic_string_view:bounds", ETL_STRING_VIEW_FILE_ID"A"), file_name_, line_number_)
    {
    }
  };

  //***************************************************************************
  ///\ingroup stack
  /// The exception thrown when the view is uninitialised.
  //***************************************************************************
  class string_view_uninitialised : public string_view_exception
  {
  public:

    string_view_uninitialised(string_type file_name_, numeric_type line_number_)
      : string_view_exception(ETL_ERROR_TEXT("basic_string_view:uninitialised", ETL_STRING_VIEW_FILE_ID"B"), file_name_, line_number_)
    {
    }
  };

  //***************************************************************************
  /// String view.
  //***************************************************************************
  template <typename T, typename TTraits = etl::char_traits<T> >
  class basic_string_view
  {
  public:

    typedef T        value_type;
    typedef TTraits  traits_type;
    typedef size_t   size_type;
    typedef const T& const_reference;
    typedef const T* const_pointer;
    typedef const T* const_iterator;
    typedef ETL_OR_STD::reverse_iterator<const_iterator> const_reverse_iterator;

    enum
    {
      npos = etl::integral_limits<size_t>::max
    };

    //*************************************************************************
    /// Default constructor.
    //*************************************************************************
    ETL_CONSTEXPR basic_string_view() ETL_NOEXCEPT
      : mbegin(ETL_NULLPTR)
      , mend(ETL_NULLPTR)
    {
    }

    //*************************************************************************
    /// Construct from string.
    //*************************************************************************
    ETL_CONSTEXPR basic_string_view(const etl::ibasic_string<T>& str)
      : mbegin(str.begin())
      , mend(str.end())
    {
    }

    //*************************************************************************
    /// Construct from T*.
    //*************************************************************************
    ETL_CONSTEXPR14  ETL_EXPLICIT_STRING_FROM_CHAR basic_string_view(const T* begin_)
      : mbegin(begin_)
      , mend(begin_ + TTraits::length(begin_))
    {
    }

    //*************************************************************************
    /// Construct from pointer range.
    //*************************************************************************
    ETL_CONSTEXPR basic_string_view(const T* begin_, const T* end_)
      : mbegin(begin_)
      , mend(end_)
    {
    }

    //*************************************************************************
    /// Construct from pointer/size.
    //*************************************************************************
    ETL_CONSTEXPR basic_string_view(const T* begin_, size_t size_)
      : mbegin(begin_)
      , mend(begin_ + size_)
    {
    }

    //*************************************************************************
    /// Copy constructor
    //*************************************************************************
    ETL_CONSTEXPR basic_string_view(const basic_string_view& other) ETL_NOEXCEPT
      : mbegin(other.mbegin)
      , mend(other.mend)
    {
    }

    //*************************************************************************
    /// Returns a const reference to the first element.
    //*************************************************************************
    ETL_CONSTEXPR const_reference front() const
    {
      return *mbegin;
    }

    //*************************************************************************
    /// Returns a const reference to the last element.
    //*************************************************************************
    ETL_CONSTEXPR const_reference back() const
    {
      return *(mend - 1);
    }

    //*************************************************************************
    /// Returns a const pointer to the first element of the internal storage.
    //*************************************************************************
    ETL_CONSTEXPR const_pointer data() const
    {
      return mbegin;
    }

    //*************************************************************************
    /// Returns a const iterator to the beginning of the array.
    //*************************************************************************
    ETL_CONSTEXPR const_iterator begin() const
    {
      return mbegin;
    }

    //*************************************************************************
    /// Returns a const iterator to the beginning of the array.
    //*************************************************************************
    ETL_CONSTEXPR const_iterator cbegin() const
    {
      return mbegin;
    }

    //*************************************************************************
    /// Returns a const iterator to the end of the array.
    //*************************************************************************
    ETL_CONSTEXPR const_iterator end() const
    {
      return mend;
    }

    //*************************************************************************
    // Returns a const iterator to the end of the array.
    //*************************************************************************
    ETL_CONSTEXPR const_iterator cend() const
    {
      return mend;
    }

    //*************************************************************************
    /// Returns a const reverse iterator to the reverse beginning of the array.
    //*************************************************************************
    ETL_CONSTEXPR const_reverse_iterator rbegin() const
    {
      return const_reverse_iterator(mend);
    }

    //*************************************************************************
    /// Returns a const reverse iterator to the reverse beginning of the array.
    //*************************************************************************
    ETL_CONSTEXPR const_reverse_iterator crbegin() const
    {
      return const_reverse_iterator(mend);
    }

    //*************************************************************************
    /// Returns a const reverse iterator to the end of the array.
    //*************************************************************************
    ETL_CONSTEXPR const_reverse_iterator rend() const
    {
      return const_reverse_iterator(mbegin);
    }

    //*************************************************************************
    /// Returns a const reverse iterator to the end of the array.
    //*************************************************************************
    ETL_CONSTEXPR const_reverse_iterator crend() const
    {
      return const_reverse_iterator(mbegin);
    }

    //*************************************************************************
    // Capacity
    //*************************************************************************

    //*************************************************************************
    /// Returns <b>true</b> if the array size is zero.
    //*************************************************************************
    ETL_CONSTEXPR bool empty() const
    {
      return (mbegin == mend);
    }

    //*************************************************************************
    /// Returns the size of the array.
    //*************************************************************************
    ETL_CONSTEXPR size_t size() const
    {
      return static_cast<size_t>(mend - mbegin);
    }

    //*************************************************************************
    /// Returns the size of the array.
    //*************************************************************************
    ETL_CONSTEXPR size_t length() const
    {
      return size();
    }

    //*************************************************************************
    /// Returns the maximum possible size of the array.
    //*************************************************************************
    ETL_CONSTEXPR size_t max_size() const
    {
      return size();
    }

    //*************************************************************************
    /// Assign from a view.
    //*************************************************************************
    ETL_CONSTEXPR14 etl::basic_string_view<T, TTraits>& operator=(const etl::basic_string_view<T, TTraits>& other)
    {
      mbegin = other.mbegin;
      mend   = other.mend;
      return *this;
    }

    //*************************************************************************
    /// Assign from iterators
    //*************************************************************************
    ETL_CONSTEXPR14 void assign(const_pointer begin_, const_pointer end_)
    {
      mbegin = begin_;
      mend   = end_;
    }

    //*************************************************************************
    /// Assign from iterator and size.
    //*************************************************************************
    ETL_CONSTEXPR14 void assign(const_pointer begin_, size_t size_)
    {
      mbegin = begin_;
      mend   = begin_ + size_;
    }

    //*************************************************************************
    /// Returns a const reference to the indexed value.
    //*************************************************************************
    ETL_CONSTEXPR const_reference operator[](size_t i) const
    {
      return mbegin[i];
    }

    //*************************************************************************
    /// Returns a const reference to the indexed value.
    //*************************************************************************
    const_reference at(size_t i) const
    {
      ETL_ASSERT((mbegin != ETL_NULLPTR && mend != ETL_NULLPTR), ETL_ERROR(string_view_uninitialised));
      ETL_ASSERT(i < size(), ETL_ERROR(string_view_bounds));
      return mbegin[i];
    }

    //*************************************************************************
    /// Swaps with another basic_string_view.
    //*************************************************************************
    ETL_CONSTEXPR14 void swap(basic_string_view& other) ETL_NOEXCEPT
    {
      using ETL_OR_STD::swap; // Allow ADL

      swap(mbegin, other.mbegin);
      swap(mend, other.mend);
    }

    //*************************************************************************
    /// Copies characters
    //*************************************************************************
    ETL_CONSTEXPR14 size_type copy(T* destination, size_type count, size_type position = 0) const
    {
      size_t n = 0UL;

      if (position < size())
      {
        n = etl::min(count, size() - position);

        etl::copy(mbegin + position, mbegin + position + n, destination);
      }

      return n;
    }

    //*************************************************************************
    /// Returns a substring
    //*************************************************************************
    ETL_CONSTEXPR14 basic_string_view substr(size_type position = 0, size_type count = npos) const
    {
      basic_string_view view;

      if (position < size())
      {
        size_t n = etl::min(count, size() - position);

        view = basic_string_view(mbegin + position, mbegin + position + n);
      }

      return view;
    }

    //*************************************************************************
    /// Shrinks the view by moving its start forward.
    //*************************************************************************
    ETL_CONSTEXPR14 void remove_prefix(size_type n)
    {
      mbegin += n;
    }

    //*************************************************************************
    /// Shrinks the view by moving its end backward.
    //*************************************************************************
    ETL_CONSTEXPR14 void remove_suffix(size_type n)
    {
      mend -= n;
    }

    //*************************************************************************
    /// Compares two views
    //*************************************************************************
    ETL_CONSTEXPR14 int compare(basic_string_view<T, TTraits> view) const
    {
      return (*this == view) ? 0 : ((*this > view) ? 1 : -1);
    }

    ETL_CONSTEXPR14 int compare(size_type position, size_type count, basic_string_view view) const
    {
      return substr(position, count).compare(view);
    }

    ETL_CONSTEXPR14 int compare(size_type position1, size_type count1,
                                basic_string_view view,
                                size_type position2, size_type count2) const
    {
      return substr(position1, count1).compare(view.substr(position2, count2));
    }

    ETL_CONSTEXPR14 int compare(const T* text) const
    {
      const T* view_itr = mbegin;
      const T* text_itr = text;

      while (view_itr != mend && *text_itr != T(0))
      {
        if (*view_itr < *text_itr)
        {
          return -1;
        }
        else if (*view_itr > *text_itr)
        {
          return 1;
        }
        ++view_itr;
        ++text_itr;
      }

      if ((view_itr == mend) && (*text_itr == T(0)))
      {
        return 0;
      }
      else if (view_itr == mend)
      {
        return -1;
      }
      else
      {
        return 1;
      }
    }

    ETL_CONSTEXPR14 int compare(size_type position, size_type count, const T* text) const
    {
      return substr(position, count).compare(text);
    }

    ETL_CONSTEXPR14 int compare(size_type position, size_type count1, const T* text, size_type count2) const
    {
      return substr(position, count1).compare(etl::basic_string_view<T, TTraits>(text, count2));
    }

    //*************************************************************************
    /// Checks if the string view starts with the given prefix
    //*************************************************************************
    ETL_CONSTEXPR14 bool starts_with(etl::basic_string_view<T, TTraits> view) const
    {
      return (size() >= view.size()) &&
             (compare(0, view.size(), view) == 0);
    }

    ETL_CONSTEXPR14 bool starts_with(T c) const
    {
      return !empty() && (front() == c);
    }

    ETL_CONSTEXPR14 bool starts_with(const T* text) const
    {
      size_t lengthtext = TTraits::length(text);

      return (size() >= lengthtext) &&
             (compare(0, lengthtext, text) == 0);
    }

    //*************************************************************************
    /// Checks if the string view ends with the given suffix
    //*************************************************************************
    ETL_CONSTEXPR14 bool ends_with(etl::basic_string_view<T, TTraits> view) const
    {
      return (size() >= view.size()) &&
             (compare(size() - view.size(), npos, view) == 0);
    }

    ETL_CONSTEXPR14 bool ends_with(T c) const
    {
      return !empty() && (back() == c);
    }

    ETL_CONSTEXPR14 bool ends_with(const T* text) const
    {
      size_t lengthtext = TTraits::length(text);
      size_t lengthview = size();

      return (lengthview >= lengthtext) &&
             (compare(lengthview - lengthtext, lengthtext, text) == 0);
    }

    //*************************************************************************
    /// Find characters in the view
    //*************************************************************************
    ETL_CONSTEXPR14 size_type find(etl::basic_string_view<T, TTraits> view, size_type position = 0) const
    {
      if ((size() < view.size()))
      {
        return npos;
      }

      const_iterator iposition = etl::search(begin() + position, end(), view.begin(), view.end());

      if (iposition == end())
      {
        return npos;
      }
      else
      {
        return etl::distance(begin(), iposition);
      }
    }

    ETL_CONSTEXPR14 size_type find(T c, size_type position = 0) const
    {
      return find(etl::basic_string_view<T, TTraits>(&c, 1), position);
    }

    ETL_CONSTEXPR14 size_type find(const T* text, size_type position, size_type count) const
    {
      return find(etl::basic_string_view<T, TTraits>(text, count), position);
    }

    ETL_CONSTEXPR14 size_type find(const T* text, size_type position = 0) const
    {
      return find(etl::basic_string_view<T, TTraits>(text), position);
    }

    //*************************************************************************
    /// Find the last occurrence of a substring
    //*************************************************************************
    ETL_CONSTEXPR14 size_type rfind(etl::basic_string_view<T, TTraits> view, size_type position = npos) const
    {
      if ((size() < view.size()))
      {
        return npos;
      }

      position = etl::min(position, size());

      const_iterator iposition = etl::find_end(begin(),
        begin() + position,
        view.begin(),
        view.end());

      if (iposition == end())
      {
        return npos;
      }
      else
      {
        return etl::distance(begin(), iposition);
      }
    }

    ETL_CONSTEXPR14 size_type rfind(T c, size_type position = npos) const
    {
      return rfind(etl::basic_string_view<T, TTraits>(&c, 1), position);
    }

    ETL_CONSTEXPR14 size_type rfind(const T* text, size_type position, size_type count) const
    {
      return rfind(etl::basic_string_view<T, TTraits>(text, count), position);
    }

    ETL_CONSTEXPR14 size_type rfind(const T* text, size_type position = npos) const
    {
      return rfind(etl::basic_string_view<T, TTraits>(text), position);
    }

    //*************************************************************************
    ///  Find first occurrence of characters
    //*************************************************************************
    ETL_CONSTEXPR14 size_type find_first_of(etl::basic_string_view<T, TTraits> view, size_type position = 0) const
    {
      const size_t lengthtext = size();

      if (position < lengthtext)
      {
        for (size_t i = position; i < lengthtext; ++i)
        {
          const size_t lengthview = view.size();

          for (size_t j = 0UL; j < lengthview; ++j)
          {
            if (mbegin[i] == view[j])
            {
              return i;
            }
          }
        }
      }

      return npos;
    }

    ETL_CONSTEXPR14 size_type find_first_of(T c, size_type position = 0) const
    {
      return find_first_of(etl::basic_string_view<T, TTraits>(&c, 1), position);
    }

    ETL_CONSTEXPR14 size_type find_first_of(const T* text, size_type position, size_type count) const
    {
      return find_first_of(etl::basic_string_view<T, TTraits>(text, count), position);
    }

    ETL_CONSTEXPR14 size_type find_first_of(const T* text, size_type position = 0) const
    {
      return find_first_of(etl::basic_string_view<T, TTraits>(text), position);
    }

    //*************************************************************************
    /// Find last occurrence of characters
    //*************************************************************************
    ETL_CONSTEXPR14 size_type find_last_of(etl::basic_string_view<T, TTraits> view, size_type position = npos) const
    {
      if (empty())
      {
        return npos;
      }

      position = etl::min(position, size() - 1);

      const_reverse_iterator it = rbegin() + size() - position - 1;

      while (it != rend())
      {
        const size_t viewlength = view.size();

        for (size_t j = 0UL; j < viewlength; ++j)
        {
          if (mbegin[position] == view[j])
          {
            return position;
          }
        }

        ++it;
        --position;
      }

      return npos;
    }

    ETL_CONSTEXPR14 size_type find_last_of(T c, size_type position = npos) const
    {
      return find_last_of(etl::basic_string_view<T, TTraits>(&c, 1), position);
    }

    ETL_CONSTEXPR14 size_type find_last_of(const T* text, size_type position, size_type count) const
    {
      return find_last_of(etl::basic_string_view<T, TTraits>(text, count), position);
    }

    ETL_CONSTEXPR14 size_type find_last_of(const T* text, size_type position = npos) const
    {
      return find_last_of(etl::basic_string_view<T, TTraits>(text), position);
    }

    //*************************************************************************
    /// Find first absence of characters
    //*************************************************************************
    ETL_CONSTEXPR14 size_type find_first_not_of(etl::basic_string_view<T, TTraits> view, size_type position = 0) const
    {
      const size_t lengthtext = size();

      if (position < lengthtext)
      {
        for (size_t i = position; i < lengthtext; ++i)
        {
          bool found = false;

          const size_t viewlength = view.size();

          for (size_t j = 0UL; j < viewlength; ++j)
          {
            if (mbegin[i] == view[j])
            {
              found = true;
              break;
            }
          }

          if (!found)
          {
            return i;
          }
        }
      }

      return npos;
    }

    ETL_CONSTEXPR14 size_type find_first_not_of(T c, size_type position = 0) const
    {
      return find_first_not_of(etl::basic_string_view<T, TTraits>(&c, 1), position);
    }

    ETL_CONSTEXPR14 size_type find_first_not_of(const T* text, size_type position, size_type count) const
    {
      return find_first_not_of(etl::basic_string_view<T, TTraits>(text, count), position);
    }

    ETL_CONSTEXPR14 size_type find_first_not_of(const T* text, size_type position = 0) const
    {
      return find_first_not_of(etl::basic_string_view<T, TTraits>(text), position);
    }

    //*************************************************************************
    /// Find last absence of characters
    //*************************************************************************
    ETL_CONSTEXPR14 size_type find_last_not_of(etl::basic_string_view<T, TTraits> view, size_type position = npos) const
    {
      if (empty())
      {
        return npos;
      }

      position = etl::min(position, size() - 1);

      const_reverse_iterator it = rbegin() + size() - position - 1;

      while (it != rend())
      {
        bool found = false;

        const size_t viewlength = view.size();

        for (size_t j = 0UL; j < viewlength; ++j)
        {
          if (mbegin[position] == view[j])
          {
            found = true;
            break;
          }
        }

        if (!found)
        {
          return position;
        }

        ++it;
        --position;
      }

      return npos;
    }

    ETL_CONSTEXPR14 size_type find_last_not_of(T c, size_type position = npos) const
    {
      return find_last_not_of(etl::basic_string_view<T, TTraits>(&c, 1), position);
    }

    ETL_CONSTEXPR14 size_type find_last_not_of(const T* text, size_type position, size_type count) const
    {
      return find_last_not_of(etl::basic_string_view<T, TTraits>(text, count), position);
    }

    ETL_CONSTEXPR14 size_type find_last_not_of(const T* text, size_type position = npos) const
    {
      return find_last_not_of(etl::basic_string_view<T, TTraits>(text), position);
    }

    //*********************************************************************
    /// Checks that the view is within this string
    //*********************************************************************
    bool contains(const etl::basic_string_view<T, TTraits>& view) const 
    {
      return find(view) != npos;
    }

    //*********************************************************************
    /// Checks that text is within this string
    //*********************************************************************
    bool contains(const_pointer s) const 
    {
      return find(s) != npos;
    }

    //*********************************************************************
    /// Checks that character is within this string
    //*********************************************************************
    bool contains(value_type c) const 
    {
      return find(c) != npos;
    }

    //*************************************************************************
    /// Equality for string_view.
    //*************************************************************************
    friend ETL_CONSTEXPR14 bool operator == (const etl::basic_string_view<T, TTraits>& lhs, const etl::basic_string_view<T, TTraits>& rhs)
    {
      return (lhs.size() == rhs.size()) &&
              etl::equal(lhs.begin(), lhs.end(), rhs.begin());
    }

    //*************************************************************************
    /// Inequality for string_view.
    //*************************************************************************
    friend ETL_CONSTEXPR14 bool operator != (const etl::basic_string_view<T, TTraits>& lhs, const etl::basic_string_view<T, TTraits>& rhs)
    {
      return !(lhs == rhs);
    }

    //*************************************************************************
    /// Less-than for string_view.
    //*************************************************************************
    friend ETL_CONSTEXPR14 bool operator < (const etl::basic_string_view<T, TTraits>& lhs, const etl::basic_string_view<T, TTraits>& rhs)
    {
      return etl::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
    }

    //*************************************************************************
    /// Greater-than for string_view.
    //*************************************************************************
    friend ETL_CONSTEXPR14 bool operator > (const etl::basic_string_view<T, TTraits>& lhs, const etl::basic_string_view<T, TTraits>& rhs)
    {
      return rhs < lhs;
    }

    //*************************************************************************
    /// Less-than-equal for string_view.
    //*************************************************************************
    friend ETL_CONSTEXPR14 bool operator <= (const etl::basic_string_view<T, TTraits>& lhs, const etl::basic_string_view<T, TTraits>& rhs)
    {
      return !(lhs > rhs);
    }

    //*************************************************************************
    /// Greater-than-equal for string_view.
    //*************************************************************************
    friend ETL_CONSTEXPR14 bool operator >= (const etl::basic_string_view<T, TTraits>& lhs, const etl::basic_string_view<T, TTraits>& rhs)
    {
      return !(lhs < rhs);
    }

  private:

    const_pointer mbegin;
    const_pointer mend;
  };

  typedef etl::basic_string_view<char>     string_view;
  typedef etl::basic_string_view<wchar_t>  wstring_view;
  typedef etl::basic_string_view<char8_t>  u8string_view;
  typedef etl::basic_string_view<char16_t> u16string_view;
  typedef etl::basic_string_view<char32_t> u32string_view;

  //*************************************************************************
  /// make_string_view.
  //*************************************************************************
  template<size_t Array_Size>
  ETL_CONSTEXPR14 string_view make_string_view(const char(&text)[Array_Size])
  {
    size_t length = etl::char_traits<char>::length(text, Array_Size - 1U);

    return string_view(text, length);
  }

  //***********************************
  template<size_t Array_Size>
  ETL_CONSTEXPR14 wstring_view make_string_view(const wchar_t(&text)[Array_Size])
  {
    size_t length = etl::char_traits<wchar_t>::length(text, Array_Size - 1U);

    return wstring_view(text, length);
  }

  //***********************************
  template<size_t Array_Size>
  ETL_CONSTEXPR14 u8string_view make_string_view(const char8_t(&text)[Array_Size])
  {
    size_t length = etl::char_traits<char8_t>::length(text, Array_Size - 1U);

    return u8string_view(text, length);
  }

  //***********************************
  template<size_t Array_Size>
  ETL_CONSTEXPR14 u16string_view make_string_view(const char16_t(&text)[Array_Size])
  {
    size_t length = etl::char_traits<char16_t>::length(text, Array_Size - 1U);

    return u16string_view(text, length);
  }

  //***********************************
  template<size_t Array_Size>
  ETL_CONSTEXPR14 u32string_view make_string_view(const char32_t(&text)[Array_Size])
  {
    size_t length = etl::char_traits<char32_t>::length(text, Array_Size - 1U);

    return u32string_view(text, length);
  }

  //*************************************************************************
  /// Hash function.
  //*************************************************************************
#if ETL_USING_8BIT_TYPES
  template <>
  struct hash<etl::string_view>
  {
    size_t operator()(const etl::string_view& text) const
    {
      return etl::private_hash::generic_hash<size_t>(reinterpret_cast<const uint8_t*>(text.data()),
                                                     reinterpret_cast<const uint8_t*>(text.data() + text.size()));
    }
  };

  template <>
  struct hash<etl::wstring_view>
  {
    size_t operator()(const etl::wstring_view& text) const
    {
      return etl::private_hash::generic_hash<size_t>(reinterpret_cast<const uint8_t*>(text.data()),
                                                     reinterpret_cast<const uint8_t*>(text.data() + text.size()));
    }
  };

  template <>
  struct hash<etl::u16string_view>
  {
    size_t operator()(const etl::u16string_view& text) const
    {
      return etl::private_hash::generic_hash<size_t>(reinterpret_cast<const uint8_t*>(text.data()),
                                                     reinterpret_cast<const uint8_t*>(text.data() + text.size()));
    }
  };

  template <>
  struct hash<etl::u32string_view>
  {
    size_t operator()(const etl::u32string_view& text) const
    {
      return etl::private_hash::generic_hash<size_t>(reinterpret_cast<const uint8_t*>(text.data()),
                                                     reinterpret_cast<const uint8_t*>(text.data() + text.size()));
    }
  };
#endif
}

//*************************************************************************
/// Swaps the values.
//*************************************************************************
template <typename T, typename TTraits >
void swap(etl::basic_string_view<T, TTraits>& lhs, etl::basic_string_view<T, TTraits>& rhs)
{
  lhs.swap(rhs);
}

template <typename T>
void swap(etl::basic_string_view<T, etl::char_traits<T> >& lhs, etl::basic_string_view<T, etl::char_traits<T> >& rhs)
{
  lhs.swap(rhs);
}

#include "private/minmax_pop.h"

#endif

