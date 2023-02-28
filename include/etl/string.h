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

#ifndef ETL_STRING_INCLUDED
#define ETL_STRING_INCLUDED

#include "platform.h"
#include "basic_string.h"
#include "string_view.h"
#include "hash.h"
#include "initializer_list.h"

#include <ctype.h>

#include "private/minmax_push.h"

namespace etl
{
#if ETL_USING_CPP11
  inline namespace literals
  {
    inline namespace string_literals
    {
      constexpr etl::string_view operator ""_sv(const char* str, size_t length) noexcept
      {
        return etl::string_view{ str, length };
      }
    }
  }
#endif

  typedef etl::ibasic_string<char> istring;

  //***************************************************************************
  /// A string implementation that uses a fixed size buffer.
  ///\tparam MAX_SIZE_ The maximum number of elements that can be stored.
  ///\ingroup string
  //***************************************************************************
  template <const size_t MAX_SIZE_>
  class string : public istring
  {
  public:

    typedef istring base_type;
    typedef istring interface_type;

    typedef istring::value_type value_type;

    static ETL_CONSTANT size_t MAX_SIZE = MAX_SIZE_;

    //*************************************************************************
    /// Constructor.
    //*************************************************************************
    string()
      : istring(reinterpret_cast<value_type*>(&buffer), MAX_SIZE)
    {
      this->initialise();
    }

    //*************************************************************************
    /// Copy constructor.
    ///\param other The other string.
    //*************************************************************************
    string(const etl::string<MAX_SIZE_>& other)
      : istring(reinterpret_cast<value_type*>(&buffer), MAX_SIZE)
    {
      this->assign(other);
    }

    //*************************************************************************
    /// From other istring.
    ///\param other The other istring.
    //*************************************************************************
    string(const etl::istring& other)
      : istring(reinterpret_cast<value_type*>(&buffer), MAX_SIZE)
    {
      this->assign(other);
    }

    //*************************************************************************
    /// From other string, position, length.
    ///\param other The other string.
    ///\param position The position of the first character.
    ///\param length   The number of characters. Default = npos.
    //*************************************************************************
    string(const etl::istring& other, size_t position, size_t length = npos)
      : istring(reinterpret_cast<value_type*>(&buffer), MAX_SIZE)
    {
      ETL_ASSERT(position < other.size(), ETL_ERROR(string_out_of_bounds));

      this->assign(other, position, length);
    }

    //*************************************************************************
    /// Constructor, from null terminated text.
    ///\param text The initial text of the string.
    //*************************************************************************
    ETL_EXPLICIT_STRING_FROM_CHAR string(const value_type* text)
      : istring(reinterpret_cast<value_type*>(&buffer), MAX_SIZE)
    {
      this->assign(text, text + etl::char_traits<value_type>::length(text));
    }

    //*************************************************************************
    /// Constructor, from null terminated text and count.
    ///\param text  The initial text of the string.
    ///\param count The number of characters to copy.
    //*************************************************************************
    string(const value_type* text, size_t count)
      : istring(reinterpret_cast<value_type*>(&buffer), MAX_SIZE)
    {
      this->assign(text, text + count);
    }

    //*************************************************************************
    /// Constructor, from initial size and value.
    ///\param initialSize  The initial size of the string.
    ///\param value        The value to fill the string with.
    //*************************************************************************
    string(size_type count, value_type c)
      : istring(reinterpret_cast<value_type*>(&buffer), MAX_SIZE)
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
    string(TIterator first, TIterator last, typename etl::enable_if<!etl::is_integral<TIterator>::value, int>::type = 0)
      : istring(reinterpret_cast<value_type*>(&buffer), MAX_SIZE)
    {
      this->assign(first, last);
    }

#if ETL_HAS_INITIALIZER_LIST
    //*************************************************************************
    /// Construct from initializer_list.
    //*************************************************************************
    string(std::initializer_list<value_type> init)
      : istring(reinterpret_cast<value_type*>(&buffer), MAX_SIZE)
    {
      this->assign(init.begin(), init.end());
    }
#endif

    //*************************************************************************
    /// From string_view.
    ///\param view The string_view.
    //*************************************************************************
    explicit string(const etl::string_view& view)
      : istring(reinterpret_cast<value_type*>(&buffer), MAX_SIZE)
    {
      this->assign(view.begin(), view.end());
    }

    //*************************************************************************
    /// Returns a sub-string.
    ///\param position The position of the first character.  Default = 0.
    ///\param length   The number of characters. Default = npos.
    //*************************************************************************
    etl::string<MAX_SIZE_> substr(size_type position = 0, size_type length_ = npos) const
    {
      etl::string<MAX_SIZE_> new_string;

      if (position != this->size())
      {
        ETL_ASSERT(position < this->size(), ETL_ERROR(string_out_of_bounds));

        length_ = etl::min(length_, this->size() - position);

        new_string.assign(buffer + position, buffer + position + length_);
      }

      return new_string;
    }

    //*************************************************************************
    /// Assignment operator.
    //*************************************************************************
    string& operator = (const string& rhs)
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
    string& operator = (const istring& rhs)
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
    string& operator = (const value_type* text)
    {
      this->assign(text);

      return *this;
    }

    //*************************************************************************
    /// Fix the internal pointers after a low level memory copy.
    //*************************************************************************
    void repair()
#if ETL_HAS_ISTRING_REPAIR
      ETL_OVERRIDE
#endif
    {
      etl::istring::repair_buffer(buffer);
    }

  private:

    value_type buffer[MAX_SIZE + 1];
  };

  //***************************************************************************
  /// A string implementation that uses a fixed size external buffer.
  ///\ingroup string
  //***************************************************************************
  class string_ext : public istring
  {
  public:

    typedef istring base_type;
    typedef istring interface_type;

    typedef istring::value_type value_type;
    typedef istring::size_type size_type;

    //*************************************************************************
    /// Constructor.
    //*************************************************************************
    string_ext(value_type* buffer, size_type buffer_size)
      : istring(buffer, buffer_size - 1U)
    {
      this->initialise();
    }

    //*************************************************************************
    /// Copy constructor.
    ///\param other The other string_ext.
    //*************************************************************************
    string_ext(const etl::string_ext& other, value_type* buffer, size_type buffer_size)
      : istring(buffer, buffer_size - 1U)
    {
      this->assign(other);
    }

    //*************************************************************************
    /// From other istring.
    ///\param other The other istring.
    //*************************************************************************
    string_ext(const etl::istring& other, value_type* buffer, size_type buffer_size)
      : istring(buffer, buffer_size - 1U)
    {
      this->assign(other);
    }

    //*************************************************************************
    /// From other string_ext, position, length.
    ///\param other The other string_ext.
    ///\param position The position of the first character.
    ///\param length   The number of characters. Default = npos.
    //*************************************************************************
    string_ext(const etl::istring& other, value_type* buffer, size_type buffer_size, size_type position, size_type length = npos)
      : istring(buffer, buffer_size - 1U)
    {
      ETL_ASSERT(position < other.size(), ETL_ERROR(string_out_of_bounds));

      this->assign(other, position, length);
    }

    //*************************************************************************
    /// Constructor, from null terminated text.
    ///\param text The initial text of the string_ext.
    //*************************************************************************
    string_ext(const char* text, char* buffer, size_type buffer_size)
      : istring(buffer, buffer_size - 1U)
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
    ///\param text  The initial text of the string_ext.
    ///\param count The number of characters to copy.
    //*************************************************************************
    string_ext(const value_type* text, size_type count, value_type* buffer, size_type buffer_size)
      : istring(buffer, buffer_size - 1U)
    {
      this->assign(text, text + count);
    }

    //*************************************************************************
    /// Constructor, from initial size and value.
    ///\param initialSize  The initial size of the string_ext.
    ///\param value        The value to fill the string_ext with.
    //*************************************************************************
    string_ext(size_type count, value_type c, value_type* buffer, size_type buffer_size)
      : istring(buffer, buffer_size - 1U)
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
    string_ext(TIterator first, TIterator last, value_type* buffer, size_type buffer_size, typename etl::enable_if<!etl::is_integral<TIterator>::value, int>::type = 0)
      : istring(buffer, buffer_size - 1U)
    {
      this->assign(first, last);
    }

#if ETL_HAS_INITIALIZER_LIST
    //*************************************************************************
    /// Construct from initializer_list.
    //*************************************************************************
    string_ext(std::initializer_list<value_type> init, value_type* buffer, size_type buffer_size)
      : istring(buffer, buffer_size - 1U)
    {
      this->assign(init.begin(), init.end());
    }
#endif

    //*************************************************************************
    /// From string_view.
    ///\param view The string_view.
    //*************************************************************************
    explicit string_ext(const etl::string_view& view, value_type* buffer, size_type buffer_size)
      : istring(buffer, buffer_size - 1U)
    {
      this->assign(view.begin(), view.end());
    }

    //*************************************************************************
    /// Assignment operator.
    //*************************************************************************
    string_ext& operator = (const string_ext& rhs)
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
    string_ext& operator = (const istring& rhs)
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
    string_ext& operator = (const value_type* text)
    {
      this->assign(text);

      return *this;
    }

    //*************************************************************************
    /// Fix the internal pointers after a low level memory copy.
    //*************************************************************************
    void repair()
#if ETL_HAS_ISTRING_REPAIR
      ETL_OVERRIDE
#endif
    {
    }

  private:

    //*************************************************************************
    /// Deleted.
    //*************************************************************************
    string_ext(const string_ext& other) ETL_DELETE;
  };

  //*************************************************************************
  /// Hash function.
  //*************************************************************************
#if ETL_USING_8BIT_TYPES
  template <>
  struct hash<etl::istring>
  {
    size_t operator()(const etl::istring& text) const
    {
      return etl::private_hash::generic_hash<size_t>(reinterpret_cast<const uint8_t*>(&text[0]),
                                                     reinterpret_cast<const uint8_t*>(&text[text.size()]));
    }
  };

  template <const size_t SIZE>
  struct hash<etl::string<SIZE> >
  {
    size_t operator()(const etl::string<SIZE>& text) const
    {
      return etl::private_hash::generic_hash<size_t>(reinterpret_cast<const uint8_t*>(&text[0]),
                                                     reinterpret_cast<const uint8_t*>(&text[text.size()]));
    }
  };

  template <>
  struct hash<etl::string_ext>
  {
    size_t operator()(const etl::string_ext& text) const
    {
      return etl::private_hash::generic_hash<size_t>(reinterpret_cast<const uint8_t*>(&text[0]),
                                                     reinterpret_cast<const uint8_t*>(&text[text.size()]));
    }
  };
#endif

  //***************************************************************************
  /// Make string from string literal or array
  //***************************************************************************
  template<size_t ARRAY_SIZE>
  etl::string<ARRAY_SIZE - 1U> make_string(const char(&text)[ARRAY_SIZE])
  {
    return etl::string<ARRAY_SIZE - 1U>(text, etl::strlen(text, ARRAY_SIZE - 1));
  }

  //***************************************************************************
  /// Make string with max capacity from string literal or array
  //***************************************************************************
  template<const size_t MAX_SIZE, const size_t SIZE>
  etl::string<MAX_SIZE> make_string_with_capacity(const char(&text)[SIZE])
  {
    return etl::string<MAX_SIZE>(text, etl::strlen(text, SIZE));
  }
}

#include "private/minmax_pop.h"

#endif
