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

#ifndef ETL_U16STRING_INCLUDED
#define ETL_U16STRING_INCLUDED

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
      constexpr etl::u16string_view operator ""_sv(const char16_t* str, size_t length) noexcept
      {
        return etl::u16string_view{ str, length };
      }
    }
  }
#endif

  typedef ibasic_string<char16_t> iu16string;

  //***************************************************************************
  /// A u16string implementation that uses a fixed size buffer.
  ///\tparam MAX_SIZE_ The maximum number of elements that can be stored.
  ///\ingroup u16string
  //***************************************************************************
  template <size_t MAX_SIZE_>
  class u16string : public iu16string
  {
  public:

    typedef iu16string base_type;
    typedef iu16string interface_type;

    typedef iu16string::value_type value_type;

    static const size_t MAX_SIZE = MAX_SIZE_;

    //*************************************************************************
    /// Constructor.
    //*************************************************************************
    u16string()
      : iu16string(reinterpret_cast<value_type*>(&buffer), MAX_SIZE)
    {
      this->initialise();
    }

    //*************************************************************************
    /// Copy constructor.
    ///\param other The other string.
    //*************************************************************************
    u16string(const etl::u16string<MAX_SIZE_>& other)
      : iu16string(reinterpret_cast<value_type*>(&buffer), MAX_SIZE)
    {
      this->assign(other);
    }

    //*************************************************************************
    /// From other iu16string.
    ///\param other The other iu16string.
    //*************************************************************************
    u16string(const etl::iu16string& other)
      : iu16string(reinterpret_cast<value_type*>(&buffer), MAX_SIZE)
    {
      this->assign(other);
    }

    //*************************************************************************
    /// From other string, position, length.
    ///\param other The other string.
    ///\param position The position of the first character.
    ///\param length   The number of characters. Default = npos.
    //*************************************************************************
    u16string(const etl::iu16string& other, size_type position, size_type length = npos)
      : iu16string(reinterpret_cast<value_type*>(&buffer), MAX_SIZE)
    {
      ETL_ASSERT(position < other.size(), ETL_ERROR(string_out_of_bounds));

      this->assign(other, position, length);
    }

    //*************************************************************************
    /// Constructor, from null terminated text.
    ///\param text The initial text of the u16string.
    //*************************************************************************
    ETL_EXPLICIT_STRING_FROM_CHAR u16string(const value_type* text)
      : iu16string(reinterpret_cast<value_type*>(&buffer), MAX_SIZE)
    {
      this->assign(text, text + etl::char_traits<value_type>::length(text));
    }

    //*************************************************************************
    /// Constructor, from null terminated text and count.
    ///\param text  The initial text of the u16string.
    ///\param count The number of characters to copy.
    //*************************************************************************
    u16string(const value_type* text, size_type count)
      : iu16string(reinterpret_cast<value_type*>(&buffer), MAX_SIZE)
    {
      this->assign(text, text + count);
    }

    //*************************************************************************
    /// Constructor, from initial size and value.
    ///\param initialSize  The initial size of the u16string.
    ///\param value        The value to fill the u16string with.
    //*************************************************************************
    u16string(size_type count, value_type c)
      : iu16string(reinterpret_cast<value_type*>(&buffer), MAX_SIZE)
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
    u16string(TIterator first, TIterator last, typename etl::enable_if<!etl::is_integral<TIterator>::value, int>::type = 0)
      : iu16string(reinterpret_cast<value_type*>(&buffer), MAX_SIZE)
    {
      this->assign(first, last);
    }

#if ETL_HAS_INITIALIZER_LIST
    //*************************************************************************
    /// Construct from initializer_list.
    //*************************************************************************
    u16string(std::initializer_list<value_type> init)
      : iu16string(reinterpret_cast<value_type*>(&buffer), MAX_SIZE)
    {
      this->assign(init.begin(), init.end());
    }
#endif

    //*************************************************************************
    /// From string_view.
    ///\param view The string_view.
    //*************************************************************************
    explicit u16string(const etl::u16string_view& view)
      : iu16string(reinterpret_cast<value_type*>(&buffer), MAX_SIZE)
    {
      this->assign(view.begin(), view.end());
    }

    //*************************************************************************
    /// Returns a sub-string.
    ///\param position The position of the first character. Default = 0.
    ///\param length   The number of characters. Default = npos.
    //*************************************************************************
    etl::u16string<MAX_SIZE_> substr(size_type position = 0, size_type length_ = npos) const
    {
      etl::u16string<MAX_SIZE_> new_string;

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
    u16string& operator = (const u16string& rhs)
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
    u16string& operator = (const value_type* text)
    {
      this->assign(text);

      return *this;
    }

    //*************************************************************************
    /// Assignment operator.
    //*************************************************************************
    u16string& operator = (const etl::u16string_view& view)
    {
      this->assign(view);

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
      etl::iu16string::repair_buffer(buffer);
    }

  private:

    value_type buffer[MAX_SIZE + 1];
  };

  //***************************************************************************
  /// A u16string_ex implementation that uses a fixed external buffer.
  ///\ingroup u16string
  //***************************************************************************
  class u16string_ext : public iu16string
  {
  public:

    typedef iu16string base_type;
    typedef iu16string interface_type;

    typedef iu16string::value_type value_type;

    //*************************************************************************
    /// Constructor.
    //*************************************************************************
    u16string_ext(value_type* buffer, size_type buffer_size)
      : iu16string(buffer, buffer_size - 1U)
    {
      this->initialise();
    }

    //*************************************************************************
    /// Copy constructor.
    ///\param other The other u16string_ext.
    //*************************************************************************
    u16string_ext(const etl::u16string_ext& other, value_type* buffer, size_type buffer_size)
      : iu16string(buffer, buffer_size - 1U)
    {
      this->assign(other);
    }

    //*************************************************************************
    /// From other iu16string.
    ///\param other The other iu16string.
    //*************************************************************************
    u16string_ext(const etl::iu16string& other, value_type* buffer, size_type buffer_size)
      : iu16string(buffer, buffer_size - 1U)
    {
      this->assign(other);
    }

    //*************************************************************************
    /// From other u16string_ext, position, length.
    ///\param other The other u16string_ext.
    ///\param position The position of the first character.
    ///\param length   The number of characters. Default = npos.
    //*************************************************************************
    u16string_ext(const etl::iu16string& other, value_type* buffer, size_type buffer_size, size_type position, size_type length = npos)
      : iu16string(buffer, buffer_size - 1U)
    {
      ETL_ASSERT(position < other.size(), ETL_ERROR(string_out_of_bounds));

      this->assign(other, position, length);
    }

    //*************************************************************************
    /// Constructor, from null terminated text.
    ///\param text The initial text of the u16string_ext.
    //*************************************************************************
    u16string_ext(const value_type* text, value_type* buffer, size_type buffer_size)
      : iu16string(buffer, buffer_size - 1U)
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
    ///\param text  The initial text of the u16string_ext.
    ///\param count The number of characters to copy.
    //*************************************************************************
    u16string_ext(const value_type* text, size_type count, value_type* buffer, size_type buffer_size)
      : iu16string(buffer, buffer_size - 1U)
    {
      this->assign(text, text + count);
    }

    //*************************************************************************
    /// Constructor, from initial size and value.
    ///\param initialSize  The initial size of the u16string_ext.
    ///\param value        The value to fill the u16string_ext with.
    //*************************************************************************
    u16string_ext(size_type count, value_type c, value_type* buffer, size_type buffer_size)
      : iu16string(buffer, buffer_size - 1U)
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
    u16string_ext(TIterator first, TIterator last, value_type* buffer, size_type buffer_size, typename etl::enable_if<!etl::is_integral<TIterator>::value, int>::type = 0)
      : iu16string(buffer, buffer_size - 1U)
    {
      this->assign(first, last);
    }

#if ETL_HAS_INITIALIZER_LIST
    //*************************************************************************
    /// Construct from initializer_list.
    //*************************************************************************
    u16string_ext(std::initializer_list<value_type> init, value_type* buffer, size_type buffer_size)
      : iu16string(buffer, buffer_size - 1U)
    {
      this->assign(init.begin(), init.end());
    }
#endif

    //*************************************************************************
    /// From string_view.
    ///\param view The string_view.
    //*************************************************************************
    explicit u16string_ext(const etl::u16string_view& view, value_type* buffer, size_type buffer_size)
      : iu16string(buffer, buffer_size - 1U)
    {
      this->assign(view.begin(), view.end());
    }

    //*************************************************************************
    /// Assignment operator.
    //*************************************************************************
    u16string_ext& operator = (const u16string_ext& rhs)
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
    u16string_ext& operator = (const iu16string& rhs)
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
    u16string_ext& operator = (const value_type* text)
    {
      this->assign(text);

      return *this;
    }

    //*************************************************************************
    /// Assignment operator.
    //*************************************************************************
    u16string_ext& operator = (const etl::u16string_view& view)
    {
      this->assign(view);

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
    u16string_ext(const u16string_ext& other) ETL_DELETE;
  };

  //*************************************************************************
  /// Hash function.
  //*************************************************************************
#if ETL_USING_8BIT_TYPES
  template <>
  struct hash<etl::iu16string>
  {
    size_t operator()(const etl::iu16string& text) const
    {
      return etl::private_hash::generic_hash<size_t>(reinterpret_cast<const uint8_t*>(text.data()),
                                                     reinterpret_cast<const uint8_t*>(text.data() + text.size()));
    }
  };

  template <size_t SIZE>
  struct hash<etl::u16string<SIZE> >
  {
    size_t operator()(const etl::u16string<SIZE>& text) const
    {
      return etl::private_hash::generic_hash<size_t>(reinterpret_cast<const uint8_t*>(text.data()),
                                                     reinterpret_cast<const uint8_t*>(text.data() + text.size()));
    }
  };

  template <>
  struct hash<etl::u16string_ext >
  {
    size_t operator()(const etl::u16string_ext& text) const
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
  etl::u16string<Array_Size - 1U> make_string(const char16_t(&text)[Array_Size])
  {
    return etl::u16string<Array_Size - 1U>(text, etl::strlen(text, Array_Size - 1U));
  }

  //***************************************************************************
  /// Make string with max capacity from string literal or array
  //***************************************************************************
  template<size_t MAX_SIZE, size_t SIZE>
  etl::u16string<MAX_SIZE> make_string_with_capacity(const char16_t(&text)[SIZE])
  {
    return etl::u16string<MAX_SIZE>(text, etl::strlen(text, SIZE));
  }
}

#include "private/minmax_pop.h"

#endif
