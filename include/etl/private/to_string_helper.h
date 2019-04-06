///\file

/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl
https://www.etlcpp.com

Copyright(c) 2019 jwellbelove

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

#ifndef ETL_TO_STRING_HELPER_INCLUDED
#define ETL_TO_STRING_HELPER_INCLUDED

///\ingroup private

#include "../platform.h"
#include "../absolute.h"
#include "../negative.h"
#include "../format_spec.h"
#include "../type_traits.h"

#include "../stl/algorithm.h"
#include "../stl/iterator.h"

namespace etl
{
  //***************************************************************************
  /// Helper function for integrals.
  //***************************************************************************
  template <typename T, typename TIString>
  typename etl::enable_if<etl::is_integral<T>::value, const TIString&>::type
    to_string_helper(T value,
                     TIString& str,
                     const etl::format_spec<TIString>& format,
                     const bool append)
  {
    typedef typename TIString::value_type type;
    typedef typename TIString::iterator   iterator;

    const bool negative = etl::is_negative(value);

    if (!append)
    {
      str.clear();
    }

    iterator start = str.end();

    if (value == 0)
    {
      str.push_back(type('0'));
    }
    else
    {
      // Extract the digits, in reverse order.
      while (value != 0)
      {
        T remainder = etl::absolute(value % T(format.base()));
        str.push_back((remainder > 9) ? type('a' + (remainder - 10)) : type('0' + remainder));
        value = value / T(format.base());
      }

      // If number is negative, append '-'
      if ((format.base() == 10) && negative)
      {
        str.push_back(type('-'));
      }

      // Reverse the string we appended.
      std::reverse(start, str.end());
    }

    // Do we have a width specification?
    if (format.width() != 0)
    {
      uint32_t length = static_cast<uint32_t>(std::distance(start, str.end()));

      // Is the length of the string less than the width?
      if (length < format.width())
      {
        uint32_t fill_length = format.width() - length;

        if (format.right_justified())
        {
          // Insert fill characters on the left.
          str.insert(start, fill_length, format.fill());
        }
        else
        {
          // Insert fill characters on the right.
          str.insert(str.end(), fill_length, format.fill());
        }
      }
    }

    return str;
  }
}

#endif
