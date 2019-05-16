///\file

/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl
http://www.etlcpp.com

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

#ifndef ETL_U32STRING_INCLUDED
#define ETL_U32STRING_INCLUDED

#include "platform.h"
#include "basic_string.h"
#include "hash.h"

#if ETL_CPP11_SUPPORTED && !defined(ETL_STLPORT) && !defined(ETL_NO_STL)
  #include <initializer_list>
#endif

#include "private/minmax_push.h"

namespace etl
{
  typedef ibasic_string<char32_t> iu32string;

  //***************************************************************************
  /// A u32string implementation that uses a fixed size buffer.
  ///\tparam MAX_SIZE_ The maximum number of elements that can be stored.
  ///\ingroup u32string
  //***************************************************************************
  template <const size_t MAX_SIZE_>
  class u32string : public iu32string
  {
  public:

    typedef iu32string base_type;
    typedef iu32string interface_type;

    typedef iu32string::value_type value_type;

    static const size_t MAX_SIZE = MAX_SIZE_;

    //*************************************************************************
    /// Constructor.
    //*************************************************************************
    u32string()
      : iu32string(reinterpret_cast<value_type*>(&buffer), MAX_SIZE)
    {
      this->initialise();
    }

    //*************************************************************************
    /// Copy constructor.
    ///\param other The other string.
    //*************************************************************************
    u32string(const etl::u32string<MAX_SIZE_>& other)
      : iu32string(reinterpret_cast<value_type*>(&buffer), MAX_SIZE)
    {
      this->assign(other);
    }

    //*************************************************************************
    /// From other iu32string.
    ///\param other The other iu32string.
    //*************************************************************************
    u32string(const etl::iu32string& other)
      : iu32string(reinterpret_cast<value_type*>(&buffer), MAX_SIZE)
    {
      this->assign(other);
    }

    //*************************************************************************
    /// From other string, position, length.
    ///\param other The other string.
    ///\param position The position of the first character.
    ///\param length   The number of characters. Default = npos.
    //*************************************************************************
    u32string(const etl::iu32string& other, size_t position, size_t length_ = npos)
      : iu32string(reinterpret_cast<value_type*>(&buffer), MAX_SIZE)
    {
      ETL_ASSERT(position < other.size(), ETL_ERROR(string_out_of_bounds));

      this->assign(other.begin() + position, other.begin() + position + length_);

      if (other.truncated())
      {
        this->is_truncated = true;
      }
    }

    //*************************************************************************
    /// Constructor, from null terminated text.
    ///\param text The initial text of the u32string.
    //*************************************************************************
    u32string(const value_type* text)
      : iu32string(reinterpret_cast<value_type*>(&buffer), MAX_SIZE)
    {
      this->assign(text, text + etl::char_traits<value_type>::length(text));
    }

    //*************************************************************************
    /// Constructor, from null terminated text and count.
    ///\param text  The initial text of the u32string.
    ///\param count The number of characters to copy.
    //*************************************************************************
    u32string(const value_type* text, size_t count)
      : iu32string(reinterpret_cast<value_type*>(&buffer), MAX_SIZE)
    {
      this->assign(text, text + count);
    }

    //*************************************************************************
    /// Constructor, from initial size and value.
    ///\param initialSize  The initial size of the u32string.
    ///\param value        The value to fill the u32string with.
    //*************************************************************************
    u32string(size_t count, value_type c)
      : iu32string(reinterpret_cast<value_type*>(&buffer), MAX_SIZE)
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
    u32string(TIterator first, TIterator last)
      : iu32string(reinterpret_cast<value_type*>(&buffer), MAX_SIZE)
    {
      this->assign(first, last);
    }

#if ETL_CPP11_SUPPORTED && !defined(ETL_STLPORT) && !defined(ETL_NO_STL)
    //*************************************************************************
    /// Construct from initializer_list.
    //*************************************************************************
    u32string(std::initializer_list<value_type> init)
      : iu32string(reinterpret_cast<value_type*>(&buffer), MAX_SIZE)
    {
      this->assign(init.begin(), init.end());
    }
#endif

    //*************************************************************************
    /// Returns a sub-string.
    ///\param position The position of the first character. Default = 0.
    ///\param length   The number of characters. Default = npos.
    //*************************************************************************
    etl::u32string<MAX_SIZE_> substr(size_t position = 0, size_t length_ = npos) const
    {
      etl::u32string<MAX_SIZE_> new_string;

      if (position != size())
      {
        ETL_ASSERT(position < size(), ETL_ERROR(string_out_of_bounds));

        length_ = std::min(length_, size() - position);

        new_string.assign(buffer + position, buffer + position + length_);
      }

      return new_string;
    }

    //*************************************************************************
    /// Assignment operator.
    //*************************************************************************
    u32string& operator = (const u32string& rhs)
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
    u32string& operator = (const value_type* text)
    {
      this->assign(text);

      return *this;
    }

    //*************************************************************************
    /// Fix the internal pointers after a low level memory copy.
    //*************************************************************************
#ifdef ETL_ISTRING_REPAIR_ENABLE
    virtual
#endif
    void repair()
    {
      etl::iu32string::repair_buffer(buffer);
    }

  private:

    value_type buffer[MAX_SIZE + 1];
  };

  //*************************************************************************
  /// Hash function.
  //*************************************************************************
#if ETL_8BIT_SUPPORT
  template <>
  struct hash<etl::iu32string>
  {
    size_t operator()(const etl::iu32string& text) const
    {
      return etl::private_hash::generic_hash<size_t>(reinterpret_cast<const uint8_t*>(&text[0]),
                                                         reinterpret_cast<const uint8_t*>(&text[text.size()]));
    }
  };

  template <const size_t SIZE>
  struct hash<etl::u32string<SIZE> >
  {
    size_t operator()(const etl::u32string<SIZE>& text) const
    {
      return etl::private_hash::generic_hash<size_t>(reinterpret_cast<const uint8_t*>(&text[0]),
                                                         reinterpret_cast<const uint8_t*>(&text[text.size()]));
    }
  };
#endif
}

#include "private/minmax_pop.h"

#endif
