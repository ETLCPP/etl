///\file

/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl
https://www.etlcpp.com

Copyright(c) 2025 John Wellbelove

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

#ifndef ETL_CONST_U16STRING_INCLUDED
#define ETL_CONST_U16STRING_INCLUDED

#include "platform.h"
#include "const_basic_string.h"

#if ETL_USING_CPP14

namespace etl
{
  //***************************************************************************
  /// A string implementation that uses a fixed size external const buffer.
  ///\ingroup string
  //***************************************************************************
  class const_u16string : public etl::const_basic_string<char16_t>
  {
  public:

    using value_type      = const_basic_string<char16_t>::value_type;
    using size_type       = const_basic_string<char16_t>::size_type;
    using const_pointer   = const_basic_string<char16_t>::const_pointer;
    using const_reference = const_basic_string<char16_t>::const_reference;
    using const_iterator  = const_basic_string<char16_t>::const_iterator;

    using const_reverse_iterator = const_basic_string<char16_t>::const_reverse_iterator;

    //*************************************************************************
    /// Constructor.
    //*************************************************************************
    constexpr const_u16string(const value_type* buffer, bool is_truncated = false)
      : const_basic_string(buffer, etl::strlen(buffer), is_truncated)
    {
    }

    //*************************************************************************
    /// Constructor.
    //*************************************************************************
    constexpr const_u16string(const value_type* buffer, size_t length, bool is_truncated = false)
      : const_basic_string(buffer, length, is_truncated)
    {
    }

    //*************************************************************************
    /// Constructor.
    //*************************************************************************
    constexpr const_u16string(const value_type* buffer, const value_type* buffer_end, bool is_truncated = false)
      : const_basic_string(buffer, buffer_end - buffer, is_truncated)
    {
    }

    //*************************************************************************
    /// Copy constructor.
    //*************************************************************************
    constexpr const_u16string(const etl::const_u16string& other)
      : const_basic_string(other.data(), other.size(), other.is_truncated())
    {
    }
  };
}

#endif
#endif
