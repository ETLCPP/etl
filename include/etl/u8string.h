///\file

/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl
https://www.etlcpp.com

Copyright(c) 2023 John Wellbelove

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

#ifndef ETL_U8STRING_INCLUDED
#define ETL_U8STRING_INCLUDED

#include "platform.h"
#include "basic_string.h"
#include "string_view.h"
#include "hash.h"
#include "initializer_list.h"

#include <ctype.h>

#include "private/minmax_push.h"

#if ETL_HAS_CHAR8_T
namespace etl
{
#if ETL_USING_CPP11 && ETL_HAS_NATIVE_CHAR8_T
  inline namespace literals
  {
    inline namespace string_literals
    {
      constexpr etl::u8string_view operator ""_sv(const char8_t* str, size_t length) noexcept
      {
        return etl::u8string_view{ str, length };
      }
    }
  }
#endif

  typedef etl::ibasic_string<char8_t> iu8string;

  //***************************************************************************
  /// A u8string implementation that uses a fixed size buffer.
  ///\tparam MAX_SIZE_ The maximum number of elements that can be stored.
  ///\ingroup u8string
  //***************************************************************************
  template <size_t MAX_SIZE_>
  class u8string : public iu8string
  {
  public:

    typedef iu8string base_type;
    typedef iu8string interface_type;

    typedef iu8string::value_type value_type;

    static ETL_CONSTANT size_t MAX_SIZE = MAX_SIZE_;

    //*************************************************************************
    /// Constructor.
    //*************************************************************************
    u8string()
      : iu8string(reinterpret_cast<value_type*>(&buffer), MAX_SIZE)
    {
      this->initialise();
    }

    //*************************************************************************
    /// Copy constructor.
    ///\param other The other u8string.
    //*************************************************************************
    u8string(const etl::u8string<MAX_SIZE_>& other)
      : iu8string(reinterpret_cast<value_type*>(&buffer), MAX_SIZE)
    {
      this->assign(other);
    }

    //*************************************************************************
    /// From other iu8string.
    ///\param other The other iu8string.
    //*************************************************************************
    u8string(const etl::iu8string& other)
      : iu8string(reinterpret_cast<value_type*>(&buffer), MAX_SIZE)
    {
      this->assign(other);
    }

    //*************************************************************************
    /// From other u8string, position, length.
    ///\param other The other u8string.
    ///\param position The position of the first character.
    ///\param length   The number of characters. Default = npos.
    //*************************************************************************
    u8string(const etl::iu8string& other, size_t position, size_t length = npos)
      : iu8string(reinterpret_cast<value_type*>(&buffer), MAX_SIZE)
    {
      ETL_ASSERT(position < other.size(), ETL_ERROR(string_out_of_bounds));

      this->assign(other, position, length);
    }

    //*************************************************************************
    /// Constructor, from null terminated text.
    ///\param text The initial text of the u8string.
    //*************************************************************************
    ETL_EXPLICIT_STRING_FROM_CHAR u8string(const value_type* text)
      : iu8string(reinterpret_cast<value_type*>(&buffer), MAX_SIZE)
    {
      this->assign(text, text + etl::char_traits<value_type>::length(text));
    }

    //*************************************************************************
    /// Constructor, from null terminated text and count.
    ///\param text  The initial text of the u8string.
    ///\param count The number of characters to copy.
    //*************************************************************************
    u8string(const value_type* text, size_t count)
      : iu8string(reinterpret_cast<value_type*>(&buffer), MAX_SIZE)
    {
      this->assign(text, text + count);
    }

    //*************************************************************************
    /// Constructor, from initial size and value.
    ///\param initialSize  The initial size of the u8string.
    ///\param value        The value to fill the u8string with.
    //*************************************************************************
    u8string(size_type count, value_type c)
      : iu8string(reinterpret_cast<value_type*>(&buffer), MAX_SIZE)
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
    u8string(TIterator first, TIterator last, typename etl::enable_if<!etl::is_integral<TIterator>::value, int>::type = 0)
      : iu8string(reinterpret_cast<value_type*>(&buffer), MAX_SIZE)
    {
      this->assign(first, last);
    }

#if ETL_HAS_INITIALIZER_LIST
    //*************************************************************************
    /// Construct from initializer_list.
    //*************************************************************************
    u8string(std::initializer_list<value_type> init)
      : iu8string(reinterpret_cast<value_type*>(&buffer), MAX_SIZE)
    {
      this->assign(init.begin(), init.end());
    }
#endif

    //*************************************************************************
    /// From string_view.
    ///\param view The string_view.
    //*************************************************************************
    explicit u8string(const etl::u8string_view& view)
      : iu8string(reinterpret_cast<value_type*>(&buffer), MAX_SIZE)
    {
      this->assign(view.begin(), view.end());
    }

    //*************************************************************************
    /// Returns a sub-u8string.
    ///\param position The position of the first character.  Default = 0.
    ///\param length   The number of characters. Default = npos.
    //*************************************************************************
    etl::u8string<MAX_SIZE_> substr(size_type position = 0, size_type length_ = npos) const
    {
      etl::u8string<MAX_SIZE_> new_string;

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
    u8string& operator = (const u8string& rhs)
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
    u8string& operator = (const iu8string& rhs)
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
    u8string& operator = (const value_type* text)
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
      etl::iu8string::repair_buffer(buffer);
    }

  private:

    value_type buffer[MAX_SIZE + 1];
  };

  template <size_t MAX_SIZE_>
  ETL_CONSTANT size_t u8string<MAX_SIZE_>::MAX_SIZE;

  //***************************************************************************
  /// A u8string implementation that uses a fixed size external buffer.
  ///\ingroup u8string
  //***************************************************************************
  class u8string_ext : public iu8string
  {
  public:

    typedef iu8string base_type;
    typedef iu8string interface_type;

    typedef iu8string::value_type value_type;
    typedef iu8string::size_type size_type;

    //*************************************************************************
    /// Constructor.
    //*************************************************************************
    u8string_ext(value_type* buffer, size_type buffer_size)
      : iu8string(buffer, buffer_size - 1U)
    {
      this->initialise();
    }

    //*************************************************************************
    /// Copy constructor.
    ///\param other The other u8string_ext.
    //*************************************************************************
    u8string_ext(const etl::u8string_ext& other, value_type* buffer, size_type buffer_size)
      : iu8string(buffer, buffer_size - 1U)
    {
      this->assign(other);
    }

    //*************************************************************************
    /// From other iu8string.
    ///\param other The other iu8string.
    //*************************************************************************
    u8string_ext(const etl::iu8string& other, value_type* buffer, size_type buffer_size)
      : iu8string(buffer, buffer_size - 1U)
    {
      this->assign(other);
    }

    //*************************************************************************
    /// From other u8string_ext, position, length.
    ///\param other The other u8string_ext.
    ///\param position The position of the first character.
    ///\param length   The number of characters. Default = npos.
    //*************************************************************************
    u8string_ext(const etl::iu8string& other, value_type* buffer, size_type buffer_size, size_type position, size_type length = npos)
      : iu8string(buffer, buffer_size - 1U)
    {
      ETL_ASSERT(position < other.size(), ETL_ERROR(string_out_of_bounds));

      this->assign(other, position, length);
    }

    //*************************************************************************
    /// Constructor, from null terminated text.
    ///\param text The initial text of the u8string_ext.
    //*************************************************************************
    u8string_ext(const char8_t* text, char8_t* buffer, size_type buffer_size)
      : iu8string(buffer, buffer_size - 1U)
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
    ///\param text  The initial text of the u8string_ext.
    ///\param count The number of characters to copy.
    //*************************************************************************
    u8string_ext(const value_type* text, size_type count, value_type* buffer, size_type buffer_size)
      : iu8string(buffer, buffer_size - 1U)
    {
      this->assign(text, text + count);
    }

    //*************************************************************************
    /// Constructor, from initial size and value.
    ///\param initialSize  The initial size of the u8string_ext.
    ///\param value        The value to fill the u8string_ext with.
    //*************************************************************************
    u8string_ext(size_type count, value_type c, value_type* buffer, size_type buffer_size)
      : iu8string(buffer, buffer_size - 1U)
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
    u8string_ext(TIterator first, TIterator last, value_type* buffer, size_type buffer_size, typename etl::enable_if<!etl::is_integral<TIterator>::value, int>::type = 0)
      : iu8string(buffer, buffer_size - 1U)
    {
      this->assign(first, last);
    }

#if ETL_HAS_INITIALIZER_LIST
    //*************************************************************************
    /// Construct from initializer_list.
    //*************************************************************************
    u8string_ext(std::initializer_list<value_type> init, value_type* buffer, size_type buffer_size)
      : iu8string(buffer, buffer_size - 1U)
    {
      this->assign(init.begin(), init.end());
    }
#endif

    //*************************************************************************
    /// From string_view.
    ///\param view The string_view.
    //*************************************************************************
    explicit u8string_ext(const etl::u8string_view& view, value_type* buffer, size_type buffer_size)
      : iu8string(buffer, buffer_size - 1U)
    {
      this->assign(view.begin(), view.end());
    }

    //*************************************************************************
    /// Assignment operator.
    //*************************************************************************
    u8string_ext& operator = (const u8string_ext& rhs)
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
    u8string_ext& operator = (const iu8string& rhs)
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
    u8string_ext& operator = (const value_type* text)
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
    u8string_ext(const u8string_ext& other) ETL_DELETE;
  };

  //*************************************************************************
  /// Hash function.
  //*************************************************************************
#if ETL_USING_8BIT_TYPES
#include "private/diagnostic_useless_cast_push.h"
  template <>
  struct hash<etl::iu8string>
  {
    size_t operator()(const etl::iu8string& text) const
    {
      return etl::private_hash::generic_hash<size_t>(reinterpret_cast<const uint8_t*>(text.data()),
                                                     reinterpret_cast<const uint8_t*>(text.data() + text.size()));
    }
  };

  template <size_t SIZE>
  struct hash<etl::u8string<SIZE> >
  {
    size_t operator()(const etl::u8string<SIZE>& text) const
    {
      return etl::private_hash::generic_hash<size_t>(reinterpret_cast<const uint8_t*>(text.data()),
                                                     reinterpret_cast<const uint8_t*>(text.data() + text.size()));
    }
  };

  template <>
  struct hash<etl::u8string_ext>
  {
    size_t operator()(const etl::u8string_ext& text) const
    {
      return etl::private_hash::generic_hash<size_t>(reinterpret_cast<const uint8_t*>(text.data()),
                                                     reinterpret_cast<const uint8_t*>(text.data() + text.size()));
    }
  };
#include "private/diagnostic_pop.h"
#endif

  //***************************************************************************
  /// Make u8string from u8string literal or array
  //***************************************************************************
  template<size_t Array_Size>
  etl::u8string<Array_Size - 1U> make_string(const char(&text)[Array_Size])
  {
    return etl::u8string<Array_Size - 1U>(text, etl::strlen(text, Array_Size - 1));
  }

  //***************************************************************************
  /// Make u8string with max capacity from u8string literal or array
  //***************************************************************************
  template<size_t MAX_SIZE, size_t SIZE>
  etl::u8string<MAX_SIZE> make_string_with_capacity(const char(&text)[SIZE])
  {
    return etl::u8string<MAX_SIZE>(text, etl::strlen(text, SIZE));
  }
}
#endif

#include "private/minmax_pop.h"

#endif
