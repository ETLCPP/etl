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
      inline constexpr etl::string_view operator ""_sv(const char* str, size_t length) noexcept
      {
        return etl::string_view{ str, length };
      }
    }
  }
#endif

  typedef etl::ibasic_string<char> istring;

  //***************************************************************************
  /// A string implementation that uses a fixed size external buffer.
  ///\ingroup string
  //***************************************************************************
  class const_string : public istring
  {
  public:

    typedef istring base_type;
    typedef istring interface_type;

    typedef istring::value_type value_type;
    typedef istring::size_type  size_type;

    //*************************************************************************
    /// Constructor.
    //*************************************************************************
    constexpr const_string(const value_type* buffer)
      : istring(const_cast<value_type*>(buffer), etl::strlen(buffer))
    {
      this->current_size = etl::strlen(buffer);
    }

    //*************************************************************************
    /// Fix the internal pointers after a low level memory copy.
    //*************************************************************************
#if ETL_HAS_ISTRING_REPAIR
    constexpr virtual void repair() ETL_OVERRIDE
#else
    constexpr void repair()
#endif
    {
    }

  private:

    //*************************************************************************
    /// Deleted.
    //*************************************************************************
    const_string(const const_string& other) ETL_DELETE;
  };
}

#include "private/minmax_pop.h"

#endif
