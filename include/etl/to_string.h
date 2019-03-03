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

#ifndef ETL_TO_STRING_INCLUDED
#define ETL_TO_STRING_INCLUDED

///\ingroup utilities

#include "platform.h"
#include "type_traits.h"
#include "stl/algorithm.h"

namespace etl
{
  template <typename T, typename TString>
  void to_string_helper(T value, const bool negative, TString& str, const int base, const bool upper_case)
  {
    typedef typename TString::value_type type;
    typedef typename TString::iterator   iterator;

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
        T remainder = value % base;
        str.push_back((remainder > 9) ? type('a' + (remainder - 10)) : type('0' + remainder));
        value = value / base;
      }

      // If number is negative, append '-'
      if ((base == 10) && negative)
      {
        str.push_back(type('-'));
      }

      // Reverse the string we appended.
      std::reverse(start, str.end());
    }
  }

  //***************************************************************************
  ///
  //***************************************************************************
  template <typename T, typename TString>
  typename etl::enable_if<etl::is_integral<T>::value && etl::is_signed<T>::value, void>::type
    to_string(T value, TString& str, const int base = 10, const bool upper_case = true)
  {
    typedef typename etl::make_unsigned<T>::type Unsigned;

    const bool     negative = (value < 0);
    const Unsigned uvalue   = (negative ? -value : value);

    to_string_helper(uvalue, negative, str, base, upper_case);
  }

  //***************************************************************************
  ///
  //***************************************************************************
  template <typename T, typename TString>
  typename etl::enable_if<etl::is_integral<T>::value && etl::is_unsigned<T>::value, void>::type
    to_string(T value, TString& str, const int base = 10, const bool upper_case = true)
  {
    to_string_helper(value, false, str, base, upper_case);
  }
}

#endif
