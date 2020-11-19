///\file

/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl
https://www.etlcpp.com

Copyright(c) 2016 jwellbelove

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

#if ETL_CPP11_SUPPORTED && ETL_NOT_USING_STLPORT && ETL_USING_STL
  #include <initializer_list>
#endif

#include "private/minmax_push.h"

namespace etl
{
  typedef ibasic_string<char16_t> iu16string;

  //***************************************************************************
  /// A u16string implementation that uses a fixed size buffer.
  ///\tparam MAX_SIZE_ The maximum number of elements that can be stored.
  ///\ingroup u16string
  //***************************************************************************
  template <const size_t MAX_SIZE_>
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
    u16string(const etl::iu16string& other, size_type position, size_type length_ = npos)
      : iu16string(reinterpret_cast<value_type*>(&buffer), MAX_SIZE)
    {
      ETL_ASSERT(position < other.size(), ETL_ERROR(string_out_of_bounds));

      this->assign(other.begin() + position, other.begin() + position + length_);

#if ETL_STRING_TRUNCATION_CHECKS_ENABLED
      if (other.is_truncated())
      {
        this->set_truncated(true);

#if defined(ETL_STRING_TRUNCATION_IS_ERROR)
        ETL_ALWAYS_ASSERT(ETL_ERROR(string_truncation));
#endif
      }
#endif
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
    u16string(TIterator first, TIterator last)
      : iu16string(reinterpret_cast<value_type*>(&buffer), MAX_SIZE)
    {
      this->assign(first, last);
    }

#if ETL_CPP11_SUPPORTED && ETL_NOT_USING_STLPORT && ETL_USING_STL
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
    /// Fix the internal pointers after a low level memory copy.
    //*************************************************************************
    void repair()
#ifdef ETL_ISTRING_REPAIR_ENABLE
      ETL_OVERRIDE
#endif
    {
      etl::iu16string::repair_buffer(buffer);
    }

  private:

    value_type buffer[MAX_SIZE + 1];
  };

  //***************************************************************************
  /// A u16string implementation that uses a fixed size buffer.
  /// A specialisation that requires an external buffer to be specified.
  ///\ingroup u16string
  //***************************************************************************
  template <>
  class u16string<0U> : public iu16string
  {
  public:

    typedef iu16string base_type;
    typedef iu16string interface_type;

    typedef iu16string::value_type value_type;

    //*************************************************************************
    /// Constructor.
    //*************************************************************************
    u16string(value_type* buffer, size_type buffer_size)
      : iu16string(buffer, buffer_size - 1U)
    {
      this->initialise();
    }

    //*************************************************************************
    /// Copy constructor.
    ///\param other The other u16string.
    //*************************************************************************
    u16string(const etl::u16string<0U>& other, value_type* buffer, size_type buffer_size)
      : iu16string(buffer, buffer_size - 1U)
    {
      this->assign(other);
    }

    //*************************************************************************
    /// From other iu16string.
    ///\param other The other iu16string.
    //*************************************************************************
    u16string(const etl::iu16string& other, value_type* buffer, size_type buffer_size)
      : iu16string(buffer, buffer_size - 1U)
    {
      this->assign(other);
    }

    //*************************************************************************
    /// From other u16string, position, length.
    ///\param other The other u16string.
    ///\param position The position of the first character.
    ///\param length   The number of characters. Default = npos.
    //*************************************************************************
    u16string(const etl::iu16string& other, value_type* buffer, size_type buffer_size, size_type position, size_type length_ = npos)
      : iu16string(buffer, buffer_size - 1U)
    {
      ETL_ASSERT(position < other.size(), ETL_ERROR(string_out_of_bounds));

      this->assign(other.begin() + position, other.begin() + position + length_);

#if ETL_STRING_TRUNCATION_CHECKS_ENABLED
      if (other.is_truncated())
      {
        this->set_truncated(true);

#if defined(ETL_STRING_TRUNCATION_IS_ERROR)
        ETL_ALWAYS_ASSERT(ETL_ERROR(u16string_truncation));
#endif
      }
#endif
    }

    //*************************************************************************
    /// Constructor, from null terminated text.
    ///\param text The initial text of the u16string.
    //*************************************************************************
    u16string(const value_type* text, value_type* buffer, size_type buffer_size)
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
    ///\param text  The initial text of the u16string.
    ///\param count The number of characters to copy.
    //*************************************************************************
    u16string(const value_type* text, size_type count, value_type* buffer, size_type buffer_size)
      : iu16string(buffer, buffer_size - 1U)
    {
      this->assign(text, text + count);
    }

    //*************************************************************************
    /// Constructor, from initial size and value.
    ///\param initialSize  The initial size of the u16string.
    ///\param value        The value to fill the u16string with.
    //*************************************************************************
    u16string(size_type count, value_type c, value_type* buffer, size_type buffer_size)
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
    u16string(TIterator first, TIterator last, value_type* buffer, size_type buffer_size)
      : iu16string(buffer, buffer_size - 1U)
    {
      this->assign(first, last);
    }

#if ETL_CPP11_SUPPORTED && ETL_NOT_USING_STLPORT && ETL_USING_STL
    //*************************************************************************
    /// Construct from initializer_list.
    //*************************************************************************
    u16string(std::initializer_list<value_type> init, value_type* buffer, size_type buffer_size)
      : iu16string(buffer, buffer_size - 1U)
    {
      this->assign(init.begin(), init.end());
    }
#endif

    //*************************************************************************
    /// From u16string_view.
    ///\param view The u16string_view.
    //*************************************************************************
    explicit u16string(const etl::u16string_view& view, value_type* buffer, size_type buffer_size)
      : iu16string(buffer, buffer_size - 1U)
    {
      this->assign(view.begin(), view.end());
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
    u16string& operator = (const iu16string& rhs)
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
    /// Fix the internal pointers after a low level memory copy.
    //*************************************************************************
    void repair()
#ifdef ETL_ISTRING_REPAIR_ENABLE
      ETL_OVERRIDE
#endif
    {
    }

  private:

    //*************************************************************************
    /// Deleted.
    //*************************************************************************
    u16string(const u16string& other) ETL_DELETE;
  };

  //*************************************************************************
  /// Hash function.
  //*************************************************************************
#if ETL_8BIT_SUPPORT
  template <>
  struct hash<etl::iu16string>
  {
    size_t operator()(const etl::iu16string& text) const
    {
      return etl::private_hash::generic_hash<size_t>(reinterpret_cast<const uint8_t*>(&text[0]),
                                                     reinterpret_cast<const uint8_t*>(&text[text.size()]));
    }
  };

  template <const size_t SIZE>
  struct hash<etl::u16string<SIZE> >
  {
    size_t operator()(const etl::u16string<SIZE>& text) const
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
  etl::u16string<ARRAY_SIZE == 1 ? 1 : ARRAY_SIZE - 1> make_string(const char16_t(&text)[ARRAY_SIZE])
  {
    return etl::u16string<ARRAY_SIZE == 1 ? 1 : ARRAY_SIZE - 1>(text, ARRAY_SIZE - 1);
  }

  //***************************************************************************
  /// Make string with max capacity from string literal or array
  //***************************************************************************
  template<const size_t MAX_SIZE, const size_t SIZE>
  etl::u16string<MAX_SIZE> make_string_with_capacity(const char16_t(&text)[SIZE])
  {
    return etl::u16string<MAX_SIZE>(text, SIZE - 1);
  }
}

#include "private/minmax_pop.h"

#endif
