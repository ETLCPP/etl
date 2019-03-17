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

///\defgroup array array
/// A wrapper for arrays
///\ingroup containers

#undef ETL_FILE
#define ETL_FILE "42"

#include "private/minmax_push.h"

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
      : string_view_exception(ETL_ERROR_TEXT("basic_string_view:bounds", ETL_FILE"A"), file_name_, line_number_)
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
      : string_view_exception(ETL_ERROR_TEXT("basic_string_view:uninitialised", ETL_FILE"B"), file_name_, line_number_)
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

    typedef T                                     value_type;
    typedef TTraits                               traits_type;
    typedef size_t                                size_type;
    typedef const T&                              const_reference;
    typedef const T*                              const_pointer;
    typedef const T*                              const_iterator;
    typedef std::reverse_iterator<const_iterator> const_reverse_iterator;

    enum
    {
      npos = etl::integral_limits<size_t>::max
    };

    //*************************************************************************
    /// Default constructor.
    //*************************************************************************
    ETL_CONSTEXPR basic_string_view()
      : mbegin(nullptr),
        mend(nullptr)
    {
    }

    //*************************************************************************
    /// Construct from string.
    //*************************************************************************
    ETL_CONSTEXPR basic_string_view(const etl::ibasic_string<T>& str)
      : mbegin(str.begin()),
        mend(str.end())
    {
    }

    //*************************************************************************
    /// Construct from T*.
    //*************************************************************************
    ETL_CONSTEXPR basic_string_view(const T* begin_)
      : mbegin(begin_),
        mend(begin_ + TTraits::length(begin_))
    {
    }

    //*************************************************************************
    /// Construct from pointer range.
    //*************************************************************************
    ETL_CONSTEXPR basic_string_view(const T* begin_, const T* end_)
      : mbegin(begin_),
        mend(end_)
    {
    }

    //*************************************************************************
    /// Construct from iterator/size.
    //*************************************************************************
    template <typename TSize, typename TDummy = typename etl::enable_if<etl::is_integral<TSize>::value, void>::type>
    ETL_CONSTEXPR basic_string_view(const T* begin_, TSize size_)
      : mbegin(begin_),
        mend(begin_ + size_)
    {
    }

    //*************************************************************************
    /// Copy constructor
    //*************************************************************************
    ETL_CONSTEXPR basic_string_view(const basic_string_view& other)
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
    /// Returns the size of the array.
    //*************************************************************************
    size_t length() const
    {
      return size();
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
    etl::basic_string_view<T, TTraits>& operator=(const etl::basic_string_view<T, TTraits>& other)
    {
      mbegin = other.mbegin;
      mend = other.mend;
      return *this;
    }

    //*************************************************************************
    /// Assign from iterators
    //*************************************************************************
    template <typename TIterator,
              typename TDummy = typename etl::enable_if<etl::is_random_iterator<TIterator>::value, void>::type>
      void assign(TIterator begin_, TIterator end_)
    {
      mbegin = etl::addressof(*begin_);
      mend = etl::addressof(*begin_) + std::distance(begin_, end_);
    }

    //*************************************************************************
    /// Assign from iterator and size.
    //*************************************************************************
    template <typename TIterator,
              typename TSize,
              typename TDummy = typename etl::enable_if<etl::is_random_iterator<TIterator>::value, void>::type>
      void assign(TIterator begin_, TSize size_)
    {
      mbegin = etl::addressof(*begin_);
      mend = etl::addressof(*begin_) + size_;
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
      ETL_ASSERT((mbegin != nullptr && mend != nullptr), ETL_ERROR(string_view_uninitialised));
      ETL_ASSERT(i < size(), ETL_ERROR(string_view_bounds));
      return mbegin[i];
    }

    //*************************************************************************
    /// Swaps with another basic_string_view.
    //*************************************************************************
    void swap(basic_string_view& other)
    {
      std::swap(mbegin, other.mbegin);
      std::swap(mend, other.mend);
    }

    //*************************************************************************
    /// Copies characters
    //*************************************************************************
    size_type copy(T* destination, size_type count, size_type position = 0) const
    {
      size_t n = 0;

      if (position < size())
      {
        n = std::min(count, size() - position);

        std::copy(mbegin + position, mbegin + position + n, destination);
      }

      return n;
    }

    //*************************************************************************
    /// Returns a substring
    //*************************************************************************
    basic_string_view substr(size_type position = 0, size_type count = npos) const
    {
      basic_string_view view;

      if (position < size())
      {
        size_t n = std::min(count, size() - position);

        view = basic_string_view(mbegin + position, mbegin + position + n);
      }

      return view;
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
    /// Compares two views
    //*************************************************************************
    int compare(basic_string_view<T, TTraits> view) const
    {
      return (*this == view) ? 0 : ((*this > view) ? 1 : -1);
    }

    int compare(size_type position, size_type count, basic_string_view view) const
    {
      return substr(position, count).compare(view);
    }

    int compare(size_type position1, size_type count1,
                basic_string_view view,
                size_type position2, size_type count2) const
    {
      return substr(position1, count1).compare(view.substr(position2, count2));
    }

    int compare(const T* text) const
    {
      return compare(etl::basic_string_view<T, TTraits>(text));
    }

    int compare(size_type position, size_type count, const T* text) const
    {
      return substr(position, count).compare(etl::basic_string_view<T, TTraits>(text));
    }

    int compare(size_type position, size_type count1, const T* text, size_type count2) const
    {
      return substr(position, count1).compare(etl::basic_string_view<T, TTraits>(text, count2));
    }

    //*************************************************************************
    /// Checks if the string view starts with the given prefix
    //*************************************************************************
    bool starts_with(etl::basic_string_view<T, TTraits> view) const
    {
      return (size() >= view.size()) &&
             (compare(0, view.size(), view) == 0);
    }

    bool starts_with(T c) const
    {
      return !empty() && (front() == c);
    }

    bool starts_with(const T* text) const
    {
      size_t lengthtext = TTraits::length(text);

      return (size() >= lengthtext) &&
             (compare(0, lengthtext, text) == 0);
    }

    //*************************************************************************
    /// Checks if the string view ends with the given suffix
    //*************************************************************************
    bool ends_with(etl::basic_string_view<T, TTraits> view) const
    {
      return (size() >= view.size()) &&
             (compare(size() - view.size(), npos, view) == 0);
    }

    bool ends_with(T c) const
    {
      return !empty() && (back() == c);
    }

    bool ends_with(const T* text) const
    {
      size_t lengthtext = TTraits::length(text);
      size_t lengthview = size();

      return (lengthview >= lengthtext) &&
             (compare(lengthview - lengthtext, lengthtext, text) == 0);
    }

    //*************************************************************************
    /// Find characters in the view
    //*************************************************************************
    size_type find(etl::basic_string_view<T, TTraits> view, size_type position = 0) const
    {
      if ((size() < view.size()))
      {
        return npos;
      }

      const_iterator iposition = std::search(begin() + position, end(), view.begin(), view.end());

      if (iposition == end())
      {
        return npos;
      }
      else
      {
        return std::distance(begin(), iposition);
      }
    }

    size_type find(T c, size_type position = 0) const
    {
      return find(etl::basic_string_view<T, TTraits>(&c, 1), position);
    }

    size_type find(const T* text, size_type position, size_type count) const
    {
      return find(etl::basic_string_view<T, TTraits>(text, count), position);
    }

    size_type find(const T* text, size_type position = 0) const
    {
      return find(etl::basic_string_view<T, TTraits>(text), position);
    }

    //*************************************************************************
    /// Find the last occurrence of a substring
    //*************************************************************************
    size_type rfind(etl::basic_string_view<T, TTraits> view, size_type position = npos) const
    {
      if ((size() < view.size()))
      {
        return npos;
      }

      position = std::min(position, size());

      const_iterator iposition = std::find_end(begin(),
                                               begin() + position,
                                               view.begin(),
                                               view.end());

      if (iposition == end())
      {
        return npos;
      }
      else
      {
        return std::distance(begin(), iposition);
      }
    }

    size_type rfind(T c, size_type position = npos) const
    {
      return rfind(etl::basic_string_view<T, TTraits>(&c, 1), position);
    }

    size_type rfind(const T* text, size_type position, size_type count) const
    {
      return rfind(etl::basic_string_view<T, TTraits>(text, count), position);
    }

    size_type rfind(const T* text, size_type position = npos) const
    {
      return rfind(etl::basic_string_view<T, TTraits>(text), position);
    }

    //*************************************************************************
    ///  Find first occurrence of characters
    //*************************************************************************
    size_type find_first_of(etl::basic_string_view<T, TTraits> view, size_type position = 0) const
    {
      const size_t lengthtext = size();

      if (position < lengthtext)
      {
        for (size_t i = position; i < lengthtext; ++i)
        {
          const size_t lengthview = view.size();

          for (size_t j = 0; j < lengthview; ++j)
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

    size_type find_first_of(T c, size_type position = 0) const
    {
      return find_first_of(etl::basic_string_view<T, TTraits>(&c, 1), position);
    }

    size_type find_first_of(const T* text, size_type position, size_type count) const
    {
      return find_first_of(etl::basic_string_view<T, TTraits>(text, count), position);
    }

    size_type find_first_of(const T* text, size_type position = 0) const
    {
      return find_first_of(etl::basic_string_view<T, TTraits>(text), position);
    }

    //*************************************************************************
    /// Find last occurrence of characters
    //*************************************************************************
    size_type find_last_of(etl::basic_string_view<T, TTraits> view, size_type position = npos) const
    {
      if (empty())
      {
        return npos;
      }

      position = std::min(position, size() - 1);

      const_reverse_iterator it = rbegin() + size() - position - 1;

      while (it != rend())
      {
        const size_t viewlength = view.size();

        for (size_t j = 0; j < viewlength; ++j)
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

    size_type find_last_of(T c, size_type position = npos) const
    {
      return find_last_of(etl::basic_string_view<T, TTraits>(&c, 1), position);
    }

    size_type find_last_of(const T* text, size_type position, size_type count) const
    {
      return find_last_of(etl::basic_string_view<T, TTraits>(text, count), position);
    }

    size_type find_last_of(const T* text, size_type position = npos) const
    {
      return find_last_of(etl::basic_string_view<T, TTraits>(text), position);
    }

    //*************************************************************************
    /// Find first absence of characters
    //*************************************************************************
    size_type find_first_not_of(etl::basic_string_view<T, TTraits> view, size_type position = 0) const
    {
      const size_t lengthtext = size();

      if (position < lengthtext)
      {
        for (size_t i = position; i < lengthtext; ++i)
        {
          bool found = false;

          const size_t viewlength = view.size();

          for (size_t j = 0; j < viewlength; ++j)
          {
            if (mbegin[i] == view[j])
            {
              found = true;
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

    size_type find_first_not_of(T c, size_type position = 0) const
    {
      return find_first_not_of(etl::basic_string_view<T, TTraits>(&c, 1), position);
    }

    size_type find_first_not_of(const T* text, size_type position, size_type count) const
    {
      return find_first_not_of(etl::basic_string_view<T, TTraits>(text, count), position);
    }

    size_type find_first_not_of(const T* text, size_type position = 0) const
    {
      return find_first_not_of(etl::basic_string_view<T, TTraits>(text), position);
    }

    //*************************************************************************
    /// Find last absence of characters
    //*************************************************************************
    size_type find_last_not_of(etl::basic_string_view<T, TTraits> view, size_type position = npos) const
    {
      if (empty())
      {
        return npos;
      }

      position = std::min(position, size() - 1);

      const_reverse_iterator it = rbegin() + size() - position - 1;

      while (it != rend())
      {
        bool found = false;

        const size_t viewlength = view.size();

        for (size_t j = 0; j < viewlength; ++j)
        {
          if (mbegin[position] == view[j])
          {
            found = true;
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

    size_type find_last_not_of(T c, size_type position = npos) const
    {
      return find_last_not_of(etl::basic_string_view<T, TTraits>(&c, 1), position);
    }

    size_type find_last_not_of(const T* text, size_type position, size_type count) const
    {
      return find_last_not_of(etl::basic_string_view<T, TTraits>(text, count), position);
    }

    size_type find_last_not_of(const T* text, size_type position = npos) const
    {
      return find_last_not_of(etl::basic_string_view<T, TTraits>(text), position);
    }

    //*************************************************************************
    /// Equality for array views.
    //*************************************************************************
    friend bool operator == (const etl::basic_string_view<T, TTraits>& lhs, const etl::basic_string_view<T, TTraits>& rhs)
    {
      return (lhs.size() == rhs.size()) &&
        std::equal(lhs.begin(), lhs.end(), rhs.begin());
    }

    //*************************************************************************
    /// Inequality for array views.
    //*************************************************************************
    friend bool operator != (const etl::basic_string_view<T, TTraits>& lhs, const etl::basic_string_view<T, TTraits>& rhs)
    {
      return !(lhs == rhs);
    }

    //*************************************************************************
    /// Less-than for array views.
    //*************************************************************************
    friend bool operator < (const etl::basic_string_view<T, TTraits>& lhs, const etl::basic_string_view<T, TTraits>& rhs)
    {
      return std::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
    }

    //*************************************************************************
    /// Greater-than for array views.
    //*************************************************************************
    friend bool operator > (const etl::basic_string_view<T, TTraits>& lhs, const etl::basic_string_view<T, TTraits>& rhs)
    {
      return rhs < lhs;
    }

    //*************************************************************************
    /// Less-than-equal for array views.
    //*************************************************************************
    friend bool operator <= (const etl::basic_string_view<T, TTraits>& lhs, const etl::basic_string_view<T, TTraits>& rhs)
    {
      return !(lhs > rhs);
    }

    //*************************************************************************
    /// Greater-than-equal for array views.
    //*************************************************************************
    friend bool operator >= (const etl::basic_string_view<T, TTraits>& lhs, const etl::basic_string_view<T, TTraits>& rhs)
    {
      return !(lhs < rhs);
    }

  private:

    const T* mbegin;
    const T* mend;
  };

  typedef etl::basic_string_view<char>     string_view;
  typedef etl::basic_string_view<wchar_t>  wstring_view;
  typedef etl::basic_string_view<char16_t> u16string_view;
  typedef etl::basic_string_view<char32_t> u32string_view;

  //*************************************************************************
  /// Hash function.
  //*************************************************************************
#if ETL_8BIT_SUPPORT
  template <>
  struct hash<etl::string_view>
  {
    size_t operator()(const etl::string_view& text) const
    {
      return etl::private_hash::generic_hash<size_t>(reinterpret_cast<const uint8_t*>(&text[0]),
                                                         reinterpret_cast<const uint8_t*>(&text[text.size()]));
    }
  };

  template <>
  struct hash<etl::wstring_view>
  {
    size_t operator()(const etl::wstring_view& text) const
    {
      return etl::private_hash::generic_hash<size_t>(reinterpret_cast<const uint8_t*>(&text[0]),
                                                         reinterpret_cast<const uint8_t*>(&text[text.size()]));
    }
  };

  template <>
  struct hash<etl::u16string_view>
  {
    size_t operator()(const etl::u16string_view& text) const
    {
      return etl::private_hash::generic_hash<size_t>(reinterpret_cast<const uint8_t*>(&text[0]),
                                                         reinterpret_cast<const uint8_t*>(&text[text.size()]));
    }
  };

  template <>
  struct hash<etl::u32string_view>
  {
    size_t operator()(const etl::u32string_view& text) const
    {
      return etl::private_hash::generic_hash<size_t>(reinterpret_cast<const uint8_t*>(&text[0]),
                                                         reinterpret_cast<const uint8_t*>(&text[text.size()]));
    }
  };
#endif
}

//*************************************************************************
/// Swaps the values.
//*************************************************************************
template <typename T, typename TTraits = etl::char_traits<T> >
void swap(etl::basic_string_view<T, TTraits>& lhs, etl::basic_string_view<T, TTraits>& rhs)
{
  lhs.swap(rhs);
}

#include "private/minmax_pop.h"

#undef ETL_FILE

#endif

