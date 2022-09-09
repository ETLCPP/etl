///\file

/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl
https://www.etlcpp.com

Copyright(c) 2019 John Wellbelove

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
#include "../basic_format_spec.h"
#include "../type_traits.h"
#include "../container.h"
#include "../absolute.h"
#include "../algorithm.h"
#include "../iterator.h"
#include "../limits.h"

#include <math.h>

#if ETL_USING_STL && ETL_USING_CPP11
  #include <iterator> // For std::begin, std::end and std::size
#endif

namespace etl
{
  namespace private_to_string
  {
#if ETL_NOT_USING_64BIT_TYPES
    typedef int32_t workspace_t;
    typedef uint32_t uworkspace_t;
#else
    typedef int64_t workspace_t;
    typedef uint64_t uworkspace_t;
#endif

    //***************************************************************************
    /// Helper function for left/right alignment.
    //***************************************************************************
    template <typename TIString>
    void add_alignment(TIString& str, typename TIString::iterator position, const etl::basic_format_spec<TIString>& format)
    {
      uint32_t length = static_cast<uint32_t>(etl::distance(position, str.end()));

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
          str.insert(str.end(), ETL_OR_STD11::begin(t), ETL_OR_STD11::end(t));
        }
        else
        {
          str.insert(str.end(), ETL_OR_STD11::begin(f), ETL_OR_STD11::end(f));
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
                      bool append,
                      const bool negative)
    {
      typedef typename TIString::value_type type;
      typedef typename TIString::iterator   iterator;

      if (!append)
      {
        str.clear();
      }

      iterator start = str.end();

      if (value == 0)
      {
        // If number is negative, append '-' (a negative zero might occur for fractional numbers > -1.0)
        if ((format.get_base() == 10U) && negative)
        {
          str.push_back(type('-'));
        }

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
        if ((format.get_base() == 10U) && negative)
        {
          str.push_back(type('-'));
        }

        if (format.is_show_base())
        {
          switch (format.get_base())
          {
            case 2U:
            {
              str.push_back(format.is_upper_case() ? type('B') : type('b'));
              str.push_back(type('0'));
              break;
            }

            case 8U:
            {
              str.push_back(type('0'));
              break;
            }

            case 16U:
            {
              str.push_back(format.is_upper_case() ? type('X') : type('x'));
              str.push_back(type('0'));
              break;
            }

            default:
            {
              break;
            }
          }
        }

        // Reverse the string we appended.
        etl::reverse(start, str.end());
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
        str.insert(str.end(), ETL_OR_STD11::begin(n), ETL_OR_STD11::end(n));
      }
      else if (infinity)
      {
        str.insert(str.end(), ETL_OR_STD11::begin(i), ETL_OR_STD11::end(i));
      }
    }

    //***************************************************************************
    /// Helper function for floating point integral and fractional.
    //***************************************************************************
    template <typename TIString>
    void add_integral_and_fractional(const uint32_t integral,
                                     const uint32_t fractional,
                                     TIString& str,
                                     const etl::basic_format_spec<TIString>& integral_format,
                                     const etl::basic_format_spec<TIString>& fractional_format,
                                     const bool negative)
    {
      typedef typename TIString::value_type type;

      etl::private_to_string::add_integral(integral, str, integral_format, true, negative);

      if (fractional_format.get_precision() > 0)
      {
        str.push_back(type('.'));
        etl::private_to_string::add_integral(fractional, str, fractional_format, true, false);
      }
    }

#if ETL_USING_64BIT_TYPES
    //***************************************************************************
    /// Helper function for floating point integral and fractional.
    //***************************************************************************
    template <typename TIString>
    void add_integral_and_fractional(const uint64_t integral,
                                     const uint64_t fractional,
                                     TIString& str,
                                     const etl::basic_format_spec<TIString>& integral_format,
                                     const etl::basic_format_spec<TIString>& fractional_format,
                                     const bool negative)
    {
      typedef typename TIString::value_type type;

      etl::private_to_string::add_integral(integral, str, integral_format, true, negative);

      if (fractional_format.get_precision() > 0)
      {
        str.push_back(type('.'));
        etl::private_to_string::add_integral(fractional, str, fractional_format, true, false);
      }
    }
#endif

    //***************************************************************************
    /// Helper function for floating point.
    //***************************************************************************
    template <typename T, typename TIString>
    void add_floating_point(const T value,
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

      if (isnan(value) || isinf(value))
      {
        etl::private_to_string::add_nan_inf(isnan(value), isinf(value), str);
      }
      else
      {
        // Make sure we format the two halves correctly.
        uint32_t max_precision = etl::numeric_limits<T>::digits10;

#if ETL_NOT_USING_64BIT_TYPES
        if (max_precision > 9)
        {
          max_precision = 9;
        }
#endif

        etl::basic_format_spec<TIString> integral_format = format;
        integral_format.decimal().width(0).precision(format.get_precision() > max_precision ? max_precision : format.get_precision());

        etl::basic_format_spec<TIString> fractional_format = integral_format;
        fractional_format.width(integral_format.get_precision()).fill(type('0')).right();

        uworkspace_t multiplier = 1U;

        for (uint32_t i = 0U; i < fractional_format.get_precision(); ++i)
        {
          multiplier *= 10U;
        }

        // Find the integral part of the floating point
        T f_integral = floor(etl::absolute(value));
        uworkspace_t integral = static_cast<uworkspace_t>(f_integral);

        // Find the fractional part of the floating point.
        uworkspace_t fractional = static_cast<uworkspace_t>(round((etl::absolute(value) - f_integral) * multiplier));

        // Check for a rounding carry to the integral.
        if (fractional == multiplier)
        {
          ++integral;
          fractional = 0U;
        }

        etl::private_to_string::add_integral_and_fractional(integral, fractional, str, integral_format, fractional_format, etl::is_negative(value));
      }

      etl::private_to_string::add_alignment(str, start, format);
    }

    //***************************************************************************
    /// Helper function for denominated integers.
    //***************************************************************************
    template <typename T, typename TIString>
    void add_integral_denominated(const T value,
                                  const uint32_t denominator_exponent,
                                  TIString& str,
                                  const etl::basic_format_spec<TIString>& format,
                                  const bool append = false)
    {
      typedef typename TIString::iterator          iterator;
      typedef typename TIString::value_type        type;
      typedef typename etl::make_unsigned<T>::type working_t;

      if (!append)
      {
        str.clear();
      }

      iterator start = str.end();

      // Calculate the denominator.
      working_t denominator = 1U;

      for (uint32_t i = 0U; i < denominator_exponent; ++i)
      {
        denominator *= 10U;
      }

      // Get the absolute value, taking care of minimum negative values.
      working_t abs_value = etl::absolute_unsigned(value);

      // Figure out how many decimal digits we have in the value.
      const uint32_t& original_decimal_digits = denominator_exponent;

      // How many decimal digits are we displaying.
      const uint32_t displayed_decimal_digits = (format.get_precision() > original_decimal_digits) ? original_decimal_digits : format.get_precision();

      // Format for the integral part.
      etl::basic_format_spec<TIString> integral_format = format;
      integral_format.decimal().width(0U);

      // Format for the fractional part.      
      etl::basic_format_spec<TIString> fractional_format = integral_format;
      fractional_format.precision(displayed_decimal_digits).width(displayed_decimal_digits).fill(type('0')).right();

      // Do we need to check for rounding?
      if (original_decimal_digits > displayed_decimal_digits)
      {
        // Which digit to adjust?
        uint32_t count = original_decimal_digits - fractional_format.get_width();

        // The 'round-away-from-zero' value.
        uint32_t rounding = 5U;

        while (count-- > 1U)
        {
          rounding *= 10U;
        }

        abs_value += rounding;
      }

      // Split the value into integral and fractional.
      working_t integral   = abs_value / denominator;
      working_t fractional = abs_value % denominator;

      // Move the fractional part to the right place.
      uint32_t count = original_decimal_digits - fractional_format.get_width();
      while (count-- > 0U)
      {
        fractional /= 10U;
      }    

      // Create the string.
      etl::private_to_string::add_integral_and_fractional(integral, fractional, str, integral_format, fractional_format, etl::is_negative(value));
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

      return etl::private_to_string::add_integral(p, str, format, append, false);
    }

    //***************************************************************************
    /// Helper function for strings.
    //***************************************************************************
    template <typename TIString>
    void add_string(const TIString& value,
                    TIString& str,
                    const etl::basic_format_spec<TIString>& format,
                    const bool append)
    {
      if (!append)
      {
        str.clear();
      }

      typename TIString::iterator start = str.end();

      str.insert(str.end(), value.begin(), value.end());

      etl::private_to_string::add_alignment(str, start, format);
    }

    //***************************************************************************
    /// Helper function for string views.
    //***************************************************************************
    template <typename TSringView, typename TIString>
    void add_string_view(const TSringView& value,
                         TIString& str,
                         const etl::basic_format_spec<TIString>& format,
                         const bool append)
    {
      if (!append)
      {
        str.clear();
      }

      typename TIString::iterator start = str.end();

      str.insert(str.end(), value.begin(), value.end());

      etl::private_to_string::add_alignment(str, start, format);
    }

    //*********************************************************************************************************

    //***************************************************************************
    /// For booleans.
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
    /// For pointers.
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

#if ETL_USING_64BIT_TYPES
    //***************************************************************************
    /// For integrals less than 64 bits.
    //***************************************************************************
    template <typename T, typename TIString>
    typename etl::enable_if<etl::is_integral<T>::value &&
                            !etl::is_same<T, bool>::value &&
                            !etl::is_one_of<T, int64_t, uint64_t>::value, const TIString&>::type
      to_string(const T value, TIString& str, const etl::basic_format_spec<TIString>& format, const bool append = false)
    {
      typedef typename etl::conditional<etl::is_signed<T>::value, int32_t, uint32_t>::type type;

      etl::private_to_string::add_integral(type(value), str, format, append, etl::is_negative(value));

      return str;
    }

    //***************************************************************************
    /// For 64 bit integrals.
    //***************************************************************************
    template <typename T, typename TIString>
    typename etl::enable_if<etl::is_integral<T>::value &&
                            !etl::is_same<T, bool>::value &&
                            etl::is_one_of<T, int64_t, uint64_t>::value, const TIString&>::type
      to_string(const T value, TIString& str, const etl::basic_format_spec<TIString>& format, const bool append = false)
    {
      etl::private_to_string::add_integral(value, str, format, append, etl::is_negative(value));

      return str;
    }

    //***************************************************************************
    /// For denominated integrals less than 64 bits.
    //***************************************************************************
    template <typename T, typename TIString>
    typename etl::enable_if<etl::is_integral<T>::value &&
                            !etl::is_same<T, bool>::value &&
                            !etl::is_one_of<T, int64_t, uint64_t>::value, const TIString&>::type
      to_string(const T value, uint32_t denominator_exponent, TIString& str, const etl::basic_format_spec<TIString>& format, const bool append = false)
    {
      typedef typename etl::conditional<etl::is_signed<T>::value, int32_t, uint32_t>::type type;

      etl::private_to_string::add_integral_denominated(type(value), denominator_exponent, str, format, append);

      return str;
    }

    //***************************************************************************
    /// For denominated 64 bit integrals.
    //***************************************************************************
    template <typename T, typename TIString>
    typename etl::enable_if<etl::is_integral<T>::value&&
                            !etl::is_same<T, bool>::value&&
                            etl::is_one_of<T, int64_t, uint64_t>::value, const TIString&>::type
      to_string(const T value, uint32_t denominator_exponent, TIString& str, const etl::basic_format_spec<TIString>& format, const bool append = false)
    {
      etl::private_to_string::add_integral_denominated(value, denominator_exponent, str, format, append);

      return str;
    }
#else
    //***************************************************************************
    /// For integrals less than 64 bits.
    //***************************************************************************
    template <typename T, typename TIString>
    typename etl::enable_if<etl::is_integral<T>::value &&
      !etl::is_same<T, bool>::value>::value, const TIString& > ::type
      to_string(const T value, TIString& str, const etl::basic_format_spec<TIString>& format, const bool append = false)
    {
      typedef typename etl::conditional<etl::is_signed<T>::value, int32_t, uint32_t>::type type;

      etl::private_to_string::add_integral(type(value), str, format, append, false);

      return str;
    }

    //***************************************************************************
    /// For denominated integrals less than 64 bits.
    //***************************************************************************
    template <typename T, typename TIString>
    typename etl::enable_if<etl::is_integral<T>::value &&
      !etl::is_same<T, bool>::value>::value, const TIString& > ::type
      to_string(const T value, uint32_t denominator_exponent, TIString& str, const etl::basic_format_spec<TIString>& format, const bool append = false)
    {
      etl::private_to_string::add_integral_denominated(type(value), denominator_exponent, str, format, append, false);

      return str;
    }
#endif

    //***************************************************************************
    /// For floating point.
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
