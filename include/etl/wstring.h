///\file

/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl
https://www.etlcpp.com

Copyright(c) 2016 John Wellbelove

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

#ifndef ETL_WSTRING_INCLUDED
#define ETL_WSTRING_INCLUDED

#include "platform.h"
#include "basic_string.h"
#include "string_view.h"
#include "hash.h"
#include "initializer_list.h"

#include "private/minmax_push.h"

namespace etl
{
#if ETL_USING_CPP11
  inline namespace literals
  {
    inline namespace string_literals
    {
      constexpr etl::wstring_view operator ""_sv(const wchar_t* str, size_t length) noexcept
      {
        return etl::wstring_view{ str, length };
      }
    }
  }
#endif

  typedef ibasic_string<wchar_t> iwstring;

  //***************************************************************************
  /// A wstring implementation that uses a fixed size buffer.
  ///\tparam MAX_SIZE_ The maximum number of elements that can be stored.
  ///\ingroup wstring
  //***************************************************************************
  template <size_t MAX_SIZE_>
  class wstring : public iwstring
  {
  public:

    typedef iwstring base_type;
    typedef iwstring interface_type;

    typedef iwstring::value_type value_type;

    static const size_t MAX_SIZE = MAX_SIZE_;

    //*************************************************************************
    /// Constructor.
    //*************************************************************************
    wstring()
      : iwstring(reinterpret_cast<value_type*>(&buffer), MAX_SIZE)
    {
      this->initialise();
    }

    //*************************************************************************
    /// Copy constructor.
    ///\param other The other string.
    //*************************************************************************
    wstring(const etl::wstring<MAX_SIZE_>& other)
      : iwstring(reinterpret_cast<value_type*>(&buffer), MAX_SIZE)
    {
      this->assign(other);
    }

    //*************************************************************************
    /// From other iwstring.
    ///\param other The other iwstring.
    //*************************************************************************
    wstring(const etl::iwstring& other)
      : iwstring(reinterpret_cast<value_type*>(&buffer), MAX_SIZE)
    {
      this->assign(other);
    }

    //*************************************************************************
    /// From other string, position, length.
    ///\param other The other string.
    ///\param position The position of the first character.
    ///\param length   The number of characters. Default = npos.
    //*************************************************************************
    wstring(const etl::iwstring& other, size_type position, size_type length = npos)
      : iwstring(reinterpret_cast<value_type*>(&buffer), MAX_SIZE)
    {
      ETL_ASSERT(position < other.size(), ETL_ERROR(string_out_of_bounds));

      this->assign(other, position, length);
    }

    //*************************************************************************
    /// Constructor, from null terminated text.
    ///\param text The initial text of the wstring.
    //*************************************************************************
    ETL_EXPLICIT_STRING_FROM_CHAR wstring(const value_type* text)
      : iwstring(reinterpret_cast<value_type*>(&buffer), MAX_SIZE)
    {
      this->assign(text, text + etl::char_traits<value_type>::length(text));
    }

    //*************************************************************************
    /// Constructor, from null terminated text and count.
    ///\param text  The initial text of the wstring.
    ///\param count The number of characters to copy.
    //*************************************************************************
    wstring(const value_type* text, size_type count)
      : iwstring(reinterpret_cast<value_type*>(&buffer), MAX_SIZE)
    {
      this->assign(text, text + count);
    }

    //*************************************************************************
    /// Constructor, from initial size and value.
    ///\param initialSize  The initial size of the wstring.
    ///\param value        The value to fill the wstring with.
    //*************************************************************************
    wstring(size_type count, value_type c)
      : iwstring(reinterpret_cast<value_type*>(&buffer), MAX_SIZE)
    {
      this->initialise();
      this->resize(count, c);
    }

    //*************************************************************************
    /// Constructor, from an iterator range.
    ///\tparam TIterator The iterator type.
    ///\param first The iterator to the first element.
    ///\param last  The iterator to the last element + 1.
    //*************************************************************************
    template <typename TIterator>
    wstring(TIterator first, TIterator last, typename etl::enable_if<!etl::is_integral<TIterator>::value, int>::type = 0)
      : iwstring(reinterpret_cast<value_type*>(&buffer), MAX_SIZE)
    {
      this->assign(first, last);
    }

#if ETL_HAS_INITIALIZER_LIST
    //*************************************************************************
    /// Construct from initializer_list.
    //*************************************************************************
    wstring(std::initializer_list<value_type> init)
      : iwstring(reinterpret_cast<value_type*>(&buffer), MAX_SIZE)
    {
      this->assign(init.begin(), init.end());
    }
#endif

    //*************************************************************************
    /// From string_view.
    ///\param view The string_view.
    //*************************************************************************
    explicit wstring(const etl::wstring_view& view)
      : iwstring(reinterpret_cast<value_type*>(&buffer), MAX_SIZE)
    {
      this->assign(view.begin(), view.end());
    }

    //*************************************************************************
    /// Returns a sub-string.
    ///\param position The position of the first character. Default = 0.
    ///\param length   The number of characters. Default = npos.
    //*************************************************************************
    etl::wstring<MAX_SIZE_> substr(size_type position = 0, size_type length_ = npos) const
    {
      etl::wstring<MAX_SIZE_> new_string;

      if (position != size())
      {
        ETL_ASSERT(position < size(), ETL_ERROR(string_out_of_bounds));

        length_ = etl::min(length_, size() - position);

        new_string.assign(buffer + position, buffer + position + length_);
      }

      return new_string;
    }

    //*************************************************************************
    /// Assignment operator.
    //*************************************************************************
    wstring& operator = (const wstring& rhs)
    {
      if (&rhs != this)
      {
        this->assign(rhs);
      }

      return *this;
    }

    //*************************************************************************
    /// Assignment operator.
    //*************************************************************************
    wstring& operator = (const value_type* text)
    {
      this->assign(text);

      return *this;
    }

    //*************************************************************************
    /// Fix the internal pointers after a low level memory copy.
    //*************************************************************************
#if ETL_HAS_ISTRING_REPAIR
    virtual void repair() ETL_OVERRIDE
#else
    void repair()
#endif
    {
      etl::iwstring::repair_buffer(buffer);
    }

  private:

    value_type buffer[MAX_SIZE + 1];
  };

  //***************************************************************************
  /// A wstring implementation that uses a fixed size external buffer.
  ///\ingroup wstring
  //***************************************************************************
  class wstring_ext : public iwstring
  {
  public:

    typedef iwstring base_type;
    typedef iwstring interface_type;

    typedef iwstring::value_type value_type;

    //*************************************************************************
    /// Constructor.
    //*************************************************************************
    wstring_ext(value_type* buffer, size_type buffer_size)
      : iwstring(buffer, buffer_size - 1U)
    {
      this->initialise();
    }

    //*************************************************************************
    /// Copy constructor.
    ///\param other The other wstring_ext.
    //*************************************************************************
    wstring_ext(const etl::wstring_ext& other, value_type* buffer, size_type buffer_size)
      : iwstring(buffer, buffer_size - 1U)
    {
      this->assign(other);
    }

    //*************************************************************************
    /// From other iwstring.
    ///\param other The other iwstring.
    //*************************************************************************
    wstring_ext(const etl::iwstring& other, value_type* buffer, size_type buffer_size)
      : iwstring(buffer, buffer_size - 1U)
    {
      this->assign(other);
    }

    //*************************************************************************
    /// From other wstring_ext, position, length.
    ///\param other The other wstring_ext.
    ///\param position The position of the first character.
    ///\param length   The number of characters. Default = npos.
    //*************************************************************************
    wstring_ext(const etl::iwstring& other, value_type* buffer, size_type buffer_size, size_type position, size_type length = npos)
      : iwstring(buffer, buffer_size - 1U)
    {
      ETL_ASSERT(position < other.size(), ETL_ERROR(string_out_of_bounds));

      this->assign(other, position, length);
    }

    //*************************************************************************
    /// Constructor, from null terminated text.
    ///\param text The initial text of the wstring_ext.
    //*************************************************************************
    wstring_ext(const value_type* text, value_type* buffer, size_type buffer_size)
      : iwstring(buffer, buffer_size - 1U)
    {
      // Is the initial text at the same address as the buffer?
      if (text == buffer)
      {
        this->current_size = etl::strlen(buffer);
      }
      else
      {
        this->assign(text, text + etl::strlen(text));
      }
    }

    //*************************************************************************
    /// Constructor, from null terminated text and count.
    ///\param text  The initial text of the wstring_ext.
    ///\param count The number of characters to copy.
    //*************************************************************************
    wstring_ext(const value_type* text, size_type count, value_type* buffer, size_type buffer_size)
      : iwstring(buffer, buffer_size - 1U)
    {
      this->assign(text, text + count);
    }

    //*************************************************************************
    /// Constructor, from initial size and value.
    ///\param initialSize  The initial size of the wstring_ext.
    ///\param value        The value to fill the wstring_ext with.
    //*************************************************************************
    wstring_ext(size_type count, value_type c, value_type* buffer, size_type buffer_size)
      : iwstring(buffer, buffer_size - 1U)
    {
      this->initialise();
      this->resize(count, c);
    }

    //*************************************************************************
    /// Constructor, from an iterator range.
    ///\tparam TIterator The iterator type.
    ///\param first The iterator to the first element.
    ///\param last  The iterator to the last element + 1.
    //*************************************************************************
    template <typename TIterator>
    wstring_ext(TIterator first, TIterator last, value_type* buffer, size_type buffer_size, typename etl::enable_if<!etl::is_integral<TIterator>::value, int>::type = 0)
      : iwstring(buffer, buffer_size - 1U)
    {
      this->assign(first, last);
    }

#if ETL_HAS_INITIALIZER_LIST
    //*************************************************************************
    /// Construct from initializer_list.
    //*************************************************************************
    wstring_ext(std::initializer_list<value_type> init, value_type* buffer, size_type buffer_size)
      : iwstring(buffer, buffer_size - 1U)
    {
      this->assign(init.begin(), init.end());
    }
#endif

    //*************************************************************************
    /// From wstring_view.
    ///\param view The wstring_view.
    //*************************************************************************
    explicit wstring_ext(const etl::wstring_view& view, value_type* buffer, size_type buffer_size)
      : iwstring(buffer, buffer_size - 1U)
    {
      this->assign(view.begin(), view.end());
    }

    //*************************************************************************
    /// Assignment operator.
    //*************************************************************************
    wstring_ext& operator = (const wstring_ext& rhs)
    {
      if (&rhs != this)
      {
        this->assign(rhs);
      }

      return *this;
    }


    //*************************************************************************
    /// Assignment operator.
    //*************************************************************************
    wstring_ext& operator = (const iwstring& rhs)
    {
      if (&rhs != this)
      {
        this->assign(rhs);
      }

      return *this;
    }

    //*************************************************************************
    /// Assignment operator.
    //*************************************************************************
    wstring_ext& operator = (const value_type* text)
    {
      this->assign(text);

      return *this;
    }

    //*************************************************************************
    /// Fix the internal pointers after a low level memory copy.
    //*************************************************************************
#if ETL_HAS_ISTRING_REPAIR
    virtual void repair() ETL_OVERRIDE
#else
    void repair()
#endif
    {
    }

  private:

    //*************************************************************************
    /// Deleted.
    //*************************************************************************
    wstring_ext(const wstring_ext& other) ETL_DELETE;
  };

  //*************************************************************************
  /// Hash function.
  //*************************************************************************
#if ETL_USING_8BIT_TYPES
  template <>
  struct hash<etl::iwstring>
  {
    size_t operator()(const etl::iwstring& text) const
    {
      return etl::private_hash::generic_hash<size_t>(reinterpret_cast<const uint8_t*>(text.data()),
                                                     reinterpret_cast<const uint8_t*>(text.data() + text.size()));
    }
  };

  template <size_t SIZE>
  struct hash<etl::wstring<SIZE> >
  {
    size_t operator()(const etl::wstring<SIZE>& text) const
    {
      return etl::private_hash::generic_hash<size_t>(reinterpret_cast<const uint8_t*>(text.data()),
                                                     reinterpret_cast<const uint8_t*>(text.data() + text.size()));
    }
  };

  template <>
  struct hash<etl::wstring_ext>
  {
    size_t operator()(const etl::wstring_ext& text) const
    {
      return etl::private_hash::generic_hash<size_t>(reinterpret_cast<const uint8_t*>(text.data()),
                                                     reinterpret_cast<const uint8_t*>(text.data() + text.size()));
    }
  };
#endif

  //***************************************************************************
  /// Make string from string literal or array
  //***************************************************************************
  template<size_t Array_Size>
  etl::wstring<Array_Size - 1U> make_string(const wchar_t(&text)[Array_Size])
  {
    return etl::wstring<Array_Size - 1U>(text, etl::strlen(text, Array_Size - 1U));
  }

  //***************************************************************************
  /// Make string with max capacity from string literal or array
  //***************************************************************************
  template<size_t MAX_SIZE, size_t SIZE>
  etl::wstring<MAX_SIZE> make_string_with_capacity(const wchar_t(&text)[SIZE])
  {
    return etl::wstring<MAX_SIZE>(text, etl::strlen(text, SIZE));
  }
}

#include "private/minmax_pop.h"

#endif
