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

#include <cmath>

#include "../platform.h"
#include "../absolute.h"
#include "../negative.h"
#include "../basic_format_spec.h"
#include "../type_traits.h"
#include "../container.h"

#include "../stl/algorithm.h"
#include "../stl/iterator.h"
#include "../stl/limits.h"

namespace etl
{
  namespace private_to_string
  {
    //***************************************************************************
    /// Helper function for left/right alignment.
    //***************************************************************************
    template <typename TIString>
    void add_alignment(TIString& str, typename TIString::iterator position, const etl::basic_format_spec<TIString>& format)
    {
      uint32_t length = static_cast<uint32_t>(std::distance(position, str.end()));

      if (length < format.get_width())
      {
        uint32_t fill_length = format.get_width() - length;

        if (format.is_left())
        {
          // Insert fill characters on the right.
          str.insert(str.end(), fill_length, format.get_fill());
        }
        else
        {
          // Insert fill characters on the left.
          str.insert(position, fill_length, format.get_fill());
        }
      }
    }

    //***************************************************************************
    /// Helper function for booleans.
    //***************************************************************************
    template <typename TIString>
    void add_boolean(const bool value,
                     TIString& str,
                     const etl::basic_format_spec<TIString>& format,
                     const bool append)
    {
      typedef typename TIString::value_type type;
      typedef typename TIString::iterator   iterator;

      static const type t[] = { 't', 'r', 'u', 'e' };
      static const type f[] = { 'f', 'a', 'l', 's', 'e' };

      if (!append)
      {
        str.clear();
      }

      iterator start = str.end();

      if (format.is_boolalpha())
      {
        if (value)
        {
          str.insert(str.end(), etl::begin(t), etl::end(t));
        }
        else
        {
          str.insert(str.end(), etl::begin(f), etl::end(f));
        }
      }
      else
      {
        if (value)
        {
          str.push_back(type('1'));
        }
        else
        {
          str.push_back(type('0'));
        }
      }

      etl::private_to_string::add_alignment(str, start, format);
    }

    //***************************************************************************
    /// Helper function for integrals.
    //***************************************************************************
    template <typename T, typename TIString>
    void add_integral(T value,
                      TIString& str,
                      const etl::basic_format_spec<TIString>& format,
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
          T remainder = etl::absolute(value % T(format.get_base()));
          str.push_back((remainder > 9) ? (format.is_upper_case() ? type('A' + (remainder - 10)) : type('a' + (remainder - 10))) : type('0' + remainder));
          value = value / T(format.get_base());
        }

        // If number is negative, append '-'
        if ((format.get_base() == 10) && negative)
        {
          str.push_back(type('-'));
        }

        // Reverse the string we appended.
        std::reverse(start, str.end());
      }

      etl::private_to_string::add_alignment(str, start, format);
    }

    //***************************************************************************
    /// Helper function for floating point nan and inf.
    //***************************************************************************
    template <typename TIString>
    void add_nan_inf(const bool not_a_number,
                     const bool infinity,
                     TIString&  str)
    {
      typedef typename TIString::value_type type;

      static const type n[] = { 'n', 'a', 'n' };
      static const type i[] = { 'i', 'n', 'f' };

      if (not_a_number)
      {
        str.insert(str.end(), etl::begin(n), etl::end(n));
      }
      else if (infinity)
      {
        str.insert(str.end(), etl::begin(i), etl::end(i));
      }
    }

    //***************************************************************************
    /// Helper function for floating point integral and fractional.
    //***************************************************************************
    template <typename TIString>
    void add_integral_fractional(const int64_t integral,
                                 const int64_t fractional,
                                 TIString& str,
                                 const etl::basic_format_spec<TIString>& integral_format,
                                 const etl::basic_format_spec<TIString>& fractional_format)
    {
      typedef typename TIString::value_type type;

      etl::private_to_string::add_integral(integral, str, integral_format, true);

      if (fractional_format.get_precision() > 0)
      {
        str.push_back(type('.'));
        etl::private_to_string::add_integral(fractional, str, fractional_format, true);
      }
    }

    //***************************************************************************
    /// Helper function for floating point.
    //***************************************************************************
    template <typename T, typename TIString>
    void add_floating_point(T value,
                            TIString& str,
                            const etl::basic_format_spec<TIString>& format,
                            const bool append)
    {
      typedef typename TIString::iterator   iterator;
      typedef typename TIString::value_type type;

      if (!append)
      {
        str.clear();
      }

      iterator start = str.end();

      if (std::isnan(value) || std::isinf(value))
      {
        etl::private_to_string::add_nan_inf(std::isnan(value), std::isinf(value), str);
      }
      else
      {
        // Make sure we format the two halves correctly.
        uint32_t max_precision = std::numeric_limits<T>::digits10;

        etl::basic_format_spec<TIString> integral_format = format;
        integral_format.decimal().width(0).precision(format.get_precision() > max_precision ? max_precision : format.get_precision());

        etl::basic_format_spec<TIString> fractional_format = integral_format;
        fractional_format.width(integral_format.get_precision()).fill(type('0')).right();

        int64_t multiplier = 1;

        for (uint32_t i = 0; i < fractional_format.get_precision(); ++i)
        {
          multiplier *= 10U;
        }

        T f_integral = (value < T(0.0) ? ceil(value) : floor(value));
        int64_t integral = static_cast<int64_t>(f_integral);
        int64_t fractional = static_cast<int64_t>(round((value - f_integral) * multiplier));

        etl::private_to_string::add_integral_fractional(integral, fractional, str, integral_format, fractional_format);
      }

      etl::private_to_string::add_alignment(str, start, format);
    }

    //***************************************************************************
    /// Helper function for pointers.
    //***************************************************************************
    template <typename TIString>
    void add_pointer(const volatile void* value,
                     TIString& str,
                     const etl::basic_format_spec<TIString>& format,
                     const bool append)
    {
      uintptr_t p = reinterpret_cast<uintptr_t>(value);

      return etl::private_to_string::add_integral(p, str, format, append);
    }

    //*********************************************************************************************************

    //***************************************************************************
    /// For booleans. Default format spec.
    //***************************************************************************etl::basic_format_spec<TIString>
    template <typename TIString>
    const TIString& to_string(const bool value,
                              TIString& str,
                              const bool append = false)
    {
      etl::basic_format_spec<TIString> format;

      etl::private_to_string::add_boolean(value, str, format, append);

      return str;
    }

    //***************************************************************************
    /// For booleans. Supplied format spec.
    //***************************************************************************
    template <typename TIString>
    const TIString& to_string(const bool value,
                              TIString& str,
                              const etl::basic_format_spec<TIString>& format,
                              const bool append = false)
    {
      etl::private_to_string::add_boolean(value, str, format, append);

      return str;
    }

    //***************************************************************************
    /// For pointers. Default format spec.
    //***************************************************************************
    template <typename TIString>
    const TIString& to_string(const volatile void* value,
                              TIString& str,
                              const bool append = false)
    {
      etl::basic_format_spec<TIString> format;

      etl::private_to_string::add_pointer(value, str, format, append);

      return str;
    }

    //***************************************************************************
    /// For pointers. Supplied format spec.
    //***************************************************************************
    template <typename TIString>
    const TIString& to_string(const volatile void* value,
                              TIString& str,
                              const etl::basic_format_spec<TIString>& format,
                              const bool append = false)
    {
      etl::private_to_string::add_pointer(value, str, format, append);

      return str;
    }

    //***************************************************************************
    /// For signed integrals less than 64 bits. Default format spec.
    //***************************************************************************
    template <typename T, typename TIString>
    typename etl::enable_if<etl::is_integral<T>::value &&
                            etl::is_signed<T>::value &&
                            !etl::is_same<T, bool>::value &&
                            !etl::is_same<T, int64_t>::value, const TIString&>::type
     to_string(const T value, TIString& str, const bool append = false)
    {
      etl::basic_format_spec<TIString> format;

      etl::private_to_string::add_integral(int32_t(value), str, format, append);

      return str;
    }

    //***************************************************************************
    /// For signed integrals less than 64 bits. Supplied format spec.
    //***************************************************************************
    template <typename T, typename TIString>
    typename etl::enable_if<etl::is_integral<T>::value &&
                            etl::is_signed<T>::value &&
                            !etl::is_same<T, bool>::value &&
                            !etl::is_same<T, int64_t>::value, const TIString&>::type
      to_string(const T value, TIString& str, const etl::basic_format_spec<TIString>& format, const bool append = false)
    {
      etl::private_to_string::add_integral(int32_t(value), str, format, append);

      return str;
    }

    //***************************************************************************
    /// For unsigned integrals less then 64 bits. Default format spec.
    //***************************************************************************
    template <typename T, typename TIString>
    typename etl::enable_if<etl::is_integral<T>::value &&
                            etl::is_unsigned<T>::value &&
                            !etl::is_same<T, bool>::value &&
                            !etl::is_same<T, uint64_t>::value, const TIString&>::type
      to_string(const T value, TIString& str, const bool append = false)
    {
      etl::basic_format_spec<TIString> format;

      etl::private_to_string::add_integral(uint32_t(value), str, format, append);

      return str;
    }

    //***************************************************************************
    /// For unsigned integrals less than 64 bits. Supplied format spec.
    //***************************************************************************
    template <typename T, typename TIString>
    typename etl::enable_if<etl::is_integral<T>::value &&
                            etl::is_unsigned<T>::value &&
                            !etl::is_same<T, bool>::value &&
                            !etl::is_same<T, uint64_t>::value, const TIString&>::type
      to_string(const T value, TIString& str, const etl::basic_format_spec<TIString>& format, const bool append = false)
    {
      etl::private_to_string::add_integral(uint32_t(value), str, format, append);

      return str;
    }

    //***************************************************************************
    /// For signed 64 bit integrals. Default format spec.
    //***************************************************************************
    template <typename T, typename TIString>
    typename etl::enable_if<etl::is_integral<T>::value &&
                            etl::is_signed<T>::value &&
                            !etl::is_same<T, bool>::value &&
                            etl::is_same<T, int64_t>::value, const TIString&>::type
      to_string(const T value, TIString& str, const bool append = false)
    {
      etl::basic_format_spec<TIString> format;

      etl::private_to_string::add_integral(int64_t(value), str, format, append);

      return str;
    }

    //***************************************************************************
    /// For signed 64 bit integrals. Supplied format spec.
    //***************************************************************************
    template <typename T, typename TIString>
    typename etl::enable_if<etl::is_integral<T>::value &&
                            etl::is_signed<T>::value &&
                            !etl::is_same<T, bool>::value &&
                            etl::is_same<T, int64_t>::value, const TIString&>::type
      to_string(const T value, TIString& str, const etl::basic_format_spec<TIString>& format, const bool append = false)
    {
      etl::private_to_string::add_integral(int64_t(value), str, format, append);

      return str;
    }

    //***************************************************************************
    /// For unsigned 64 bit integrals. Default format spec.
    //***************************************************************************
    template <typename T, typename TIString>
    typename etl::enable_if<etl::is_integral<T>::value &&
                            etl::is_unsigned<T>::value &&
                            !etl::is_same<T, bool>::value &&
                            etl::is_same<T, uint64_t>::value, const TIString&>::type
      to_string(const T value, TIString& str, const bool append = false)
    {
      etl::basic_format_spec<TIString> format;

      etl::private_to_string::add_integral(uint64_t(value), str, format, append);

      return str;
    }

    //***************************************************************************
    /// For unsigned 64 bit integrals. Supplied format spec.
    //***************************************************************************
    template <typename T, typename TIString>
    typename etl::enable_if<etl::is_integral<T>::value &&
                            etl::is_unsigned<T>::value &&
                            !etl::is_same<T, bool>::value &&
                            etl::is_same<T, uint64_t>::value, const TIString&>::type
      to_string(const T value, TIString& str, const etl::basic_format_spec<TIString>& format, const bool append = false)
    {
      etl::private_to_string::add_integral(uint64_t(value), str, format, append);

      return str;
    }

    //***************************************************************************
    /// For floating point. Default format spec.
    //***************************************************************************
    template <typename T, typename TIString>
    typename etl::enable_if<etl::is_floating_point<T>::value, const TIString&>::type
      to_string(const T value, TIString& str, const bool append = false)
    {
      etl::basic_format_spec<TIString> format;

      etl::private_to_string::add_floating_point(value, str, format, append);

      return str;
    }

    //***************************************************************************
    /// For floating point. Supplied format spec.
    //***************************************************************************
    template <typename T, typename TIString>
    typename etl::enable_if<etl::is_floating_point<T>::value, const TIString&>::type
      to_string(const T value, TIString& str, const etl::basic_format_spec<TIString>& format, const bool append = false)
    {
      etl::private_to_string::add_floating_point(value, str, format, append);

      return str;
    }
  }
}

#endif
