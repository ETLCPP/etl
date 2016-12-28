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

#ifndef __ETL_STRING__
#define __ETL_STRING__

#include "platform.h"
#include "basic_string.h"
#include "ibasic_string.h"
#include "hash.h"

#if defined(ETL_COMPILER_MICROSOFT)
  #undef min
#endif

namespace etl
{
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

    typedef istring::value_type value_type;

    static const size_t MAX_SIZE = MAX_SIZE_;

    //*************************************************************************
    /// Constructor.
    //*************************************************************************
    string()
      : istring(reinterpret_cast<value_type*>(&buffer), MAX_SIZE)
    {
      istring::initialise();
    }

    //*************************************************************************
    /// Copy constructor.
    ///\param other The other string.
    //*************************************************************************
    string(const etl::string<MAX_SIZE_>& other)
      : istring(reinterpret_cast<value_type*>(&buffer), MAX_SIZE)
    {
      istring::initialise();
      istring::assign(other.begin(), other.end());
    }

    //*************************************************************************
    /// From other string, position, length.
    ///\param other The other string.
    ///\param position The position of the first character.
    ///\param length   The number of characters. Default = npos.
    //*************************************************************************
    string(const etl::string<MAX_SIZE_>& other, size_t position, size_t length = npos)
      : istring(reinterpret_cast<value_type*>(&buffer), MAX_SIZE)
    {
      ETL_ASSERT(position < other.size(), ETL_ERROR(string_out_of_bounds));

      // Set the length to the exact amount.
      length = (length > MAX_SIZE_) ? MAX_SIZE_ : length;

      istring::initialise();
      istring::assign(other.begin() + position, other.begin() + position + length);
    }

    //*************************************************************************
    /// Constructor, from null terminated text.
    ///\param text The initial text of the string.
    //*************************************************************************
    string(const value_type* text)
      : istring(reinterpret_cast<value_type*>(&buffer), MAX_SIZE)
    {
      istring::initialise();
      istring::assign(text, text + etl::char_traits<value_type>::length(text));
    }

    //*************************************************************************
    /// Constructor, from null terminated text and count.
    ///\param text  The initial text of the string.
    ///\param count The number of characters to copy.
    //*************************************************************************
    string(const value_type* text, size_t count)
      : istring(reinterpret_cast<value_type*>(&buffer), MAX_SIZE)
    {
      istring::initialise();
      istring::assign(text, text + count);
    }

    //*************************************************************************
    /// Constructor, from initial size and value.
    ///\param initialSize  The initial size of the string.
    ///\param value        The value to fill the string with.
    //*************************************************************************
    string(size_t count, value_type c)
      : istring(reinterpret_cast<value_type*>(&buffer), MAX_SIZE)
    {
      istring::initialise();
      istring::resize(count, c);
    }

    //*************************************************************************
    /// Constructor, from an iterator range.
    ///\tparam TIterator The iterator type.
    ///\param first The iterator to the first element.
    ///\param last  The iterator to the last element + 1.
    //*************************************************************************
    template <typename TIterator>
    string(TIterator first, TIterator last)
      : istring(reinterpret_cast<value_type*>(&buffer), MAX_SIZE)
    {
      istring::assign(first, last);
    }

    //*************************************************************************
    /// Returns a sub-string.
    ///\param position The position of the first character.  Default = 0.
    ///\param length   The number of characters. Default = npos.
    //*************************************************************************
    etl::string<MAX_SIZE_> substr(size_t position = 0, size_t length = npos) const
    {
      etl::string<MAX_SIZE_> new_string;

      if (position != this->size())
      {
        ETL_ASSERT(position < this->size(), ETL_ERROR(string_out_of_bounds));

        length = std::min(length, this->size() - position);

        new_string.assign(buffer + position, buffer + position + length);
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
        istring::assign(rhs.cbegin(), rhs.cend());
      }

      return *this;
    }

  private:

    value_type buffer[MAX_SIZE + 1];
  };

  //*************************************************************************
  /// Hash function.
  //*************************************************************************
  template <>
  struct hash<etl::istring>
  {
    size_t operator()(const etl::istring& text) const
    {
      return etl::__private_hash__::generic_hash<>(reinterpret_cast<const uint8_t*>(&text[0]), 
                                                   reinterpret_cast<const uint8_t*>(&text[text.size()]));
    }
  };
}

#if defined(ETL_COMPILER_MICROSOFT)
  #define min(a,b) (((a) < (b)) ? (a) : (b))
#endif

#endif
