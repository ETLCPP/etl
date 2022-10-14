///\file
/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl
https://www.etlcpp.com

Copyright(c) 2022 John Wellbelove

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

#ifndef ETL_TO_ARITHMETIC_INCLUDED
#define ETL_TO_ARITHMETIC_INCLUDED

#include "platform.h"
#include "type_traits.h"
#include "integral_limits.h"
#include "string_view.h"
#include "basic_string.h"
#include "format_spec.h"
#include "radix.h"
#include "optional.h"
#include "string_utilities.h"
#include "iterator.h"
#include "bit.h"
#include "smallest.h"
#include "absolute.h"

namespace etl
{
  namespace private_to_arithmetic
  {
    //***********************************************************************
    /// 
    //***********************************************************************
    struct character_set
    {
      typedef char value_type;
      typedef const value_type* string_type;
      static ETL_CONSTANT string_type Valid_Chars   = "+-.,eE0123456789abcdefABCDEF";
      static ETL_CONSTANT string_type Numeric_Chars = "0123456789abcdef";
      static ETL_CONSTANT int         Valid_Length  = etl::strlen(Numeric_Chars);
      static ETL_CONSTANT value_type  Positive_Char = '+';
      static ETL_CONSTANT value_type  Negative_Char = '-';
      static ETL_CONSTANT value_type  Radix_Point1  = '.';
      static ETL_CONSTANT value_type  Radix_Point2  = ',';
      static ETL_CONSTANT value_type  Exponential   = 'e';
      static ETL_CONSTANT value_type  Unknown_Char  = '?';

      //*******************************************
      ETL_NODISCARD
      ETL_CONSTEXPR14
      static bool is_valid(char c, etl::radix::value_type radix)
      {
        switch (radix)
        {
          case etl::radix::binary:
          {
            return (c >= '0') && (c <= '1');
            break;
          }

          case etl::radix::octal:
          {
            return (c >= '0') && (c <= '7');
            break;
          }

          case etl::radix::decimal:
          {
            return (c >= '0') && (c <= '9');
            break;
          }

          case etl::radix::hex:
          {
            return ((c >= '0') && (c <= '9')) || ((c >= 'a') && (c <= 'f'));
            break;
          }

          default:
          {
            return false;
            break;
          }
        }
      }

      //*******************************************
      ETL_NODISCARD
      ETL_CONSTEXPR14
      static char digit_value(char c, etl::radix::value_type radix)
      {
        switch (radix)
        {
          case etl::radix::binary:
          case etl::radix::octal:
          case etl::radix::decimal: 
          { 
            return c - '0';
            break; 
          }

          case etl::radix::hex:
          {
            if ((c >= '0') && (c <= '9'))
            {
              return c - '0';
            }
            else
            {
              return (c - 'a') + 10;
            }
            break;
          }

          default: 
          { 
            return 0;
            break; 
          }
        }
      }
    };

    //*******************************************
    ETL_NODISCARD
    ETL_CONSTEXPR14
    char to_lower(char c)
    {
      if ((c >= 'A') && (c <= 'Z'))
      {
        c += 32;
      }

      return c;
    }

    //*******************************************
    ETL_NODISCARD
    static ETL_CONSTEXPR14
    char convert(char c)
    {
      return to_lower(c);
    }

    //*******************************************
    ETL_NODISCARD
    static
    ETL_CONSTEXPR14
    char convert(wchar_t c)
    {
      ETL_STATIC_CONSTANT wchar_t* Valid_Chars = L"+-.,eE0123456789abcdefABCDEF";

      for (int i = 0; i < character_set::Valid_Length; ++i)
      {
        if (c == Valid_Chars[i])
        {
          return to_lower(character_set::Valid_Chars[i]);
        }
      }

      return character_set::Unknown_Char;
    }

    //*******************************************
    ETL_NODISCARD
    static 
    ETL_CONSTEXPR14
    char convert(char16_t c)
    {
      typedef char16_t value_type;
      typedef const value_type* string_type;

      ETL_STATIC_CONSTANT char16_t* Valid_Chars = u"+-.,eE0123456789abcdefABCDEF";

      for (int i = 0; i < character_set::Valid_Length; ++i)
      {
        if (c == Valid_Chars[i])
        {
          return to_lower(character_set::Valid_Chars[i]);
        }
      }

      return character_set::Unknown_Char;
    }
 
    //*******************************************
    ETL_NODISCARD
    static
    ETL_CONSTEXPR14
    char convert(char32_t c)
    {
      typedef char32_t value_type;
      typedef const char32_t* string_type;
      
      ETL_STATIC_CONSTANT string_type Valid_Chars = U"+-.,eE0123456789abcdefABCDEF";

      for (int i = 0; i < character_set::Valid_Length; ++i)
      {
        if (c == Valid_Chars[i])
        {
          return to_lower(character_set::Valid_Chars[i]);
        }
      }

      return character_set::Unknown_Char;
    }

    //***************************************************************************
    /// 
    //***************************************************************************
    template <typename TValue>
    ETL_NODISCARD
    ETL_CONSTEXPR14
    typename etl::enable_if<etl::is_floating_point<TValue>::value, TValue>::type
      accumulate_floating_point_value(TValue value, char digit, bool is_negative)
    {
      value *= etl::radix::decimal;
      is_negative ? value -= digit : value += digit;

      return value;
    }

    //***************************************************************************
    /// 
    //***************************************************************************
    template <typename TChar>
    ETL_NODISCARD
    ETL_CONSTEXPR14
    bool check_and_remove_sign_prefix(etl::basic_string_view<TChar>& view)
    {
      // Check for prefix.
      const char c = convert(view[0]);
      const bool has_positive_prefix = (c == character_set::Positive_Char);
      const bool has_negative_prefix = (c == character_set::Negative_Char);

      // Step over the prefix, if present.
      if (has_positive_prefix || has_negative_prefix)
      {
        view.remove_prefix(1);
      }

      return has_negative_prefix;
    }

    //***************************************************************************
    /// 
    //***************************************************************************
    template <typename TValue>
    struct float_accumulator
    {
      //*********************************
      ETL_NODISCARD
      ETL_CONSTEXPR14
        float_accumulator(bool is_negative_)
        : value(0)
        , is_negative(is_negative_)
        , valid_value(false)
      {
      }

      //*********************************
      ETL_NODISCARD
      ETL_CONSTEXPR14
      bool add(char c)
      {
        bool is_valid = character_set::is_valid(c, etl::radix::decimal);

        if (is_valid)
        {
          value = accumulate_floating_point_value(value, character_set::digit_value(c, etl::radix::decimal), is_negative);
          valid_value = true;
        }

        return is_valid;
      }

      //*********************************
      ETL_NODISCARD
      ETL_CONSTEXPR14
      bool has_value() const
      {
        return (valid_value == true);
      }

      //*********************************
      ETL_NODISCARD
      ETL_CONSTEXPR14
      TValue value() const
      {
        return value;
      }

    private:

      TValue value;
      bool   is_negative;
      bool   valid_value;
    };
    
    //***************************************************************************
    /// 
    //***************************************************************************
    ETL_NODISCARD
    ETL_CONSTEXPR14
    bool is_valid_radix(const etl::radix::value_type radix)
    {
      return (radix == etl::radix::binary)  ||
             (radix == etl::radix::octal)   ||
             (radix == etl::radix::decimal) ||
             (radix == etl::radix::hex);
    }

    //***************************************************************************
    /// 
    //***************************************************************************
    template <typename TValue>
    struct integral_accumulator
    {
      //*********************************
      ETL_NODISCARD 
      ETL_CONSTEXPR14
      integral_accumulator(etl::radix::value_type radix_, TValue maximum_)
        : radix(radix_)
        , maximum(maximum_)
        , integral_value(0)
        , value_is_valid(false)
      {
      }

      //*********************************
      ETL_NODISCARD
      ETL_CONSTEXPR14
      bool add(const char c)
      {
        value_is_valid = false;

        if (character_set::is_valid(c, radix))
        {
          TValue old_value = integral_value;
          integral_value *= radix;

          // No multipication overflow?
          if ((integral_value / radix) == old_value)
          {
            const char digit = character_set::digit_value(c, radix);

            // No addition overflow?
            if ((maximum - digit) >= integral_value)
            {
              integral_value += digit;
              value_is_valid = true;
            }
          }
        }

        return value_is_valid;
      }

      //*********************************
      ETL_NODISCARD
      ETL_CONSTEXPR14
      bool has_value() const
      {
        return value_is_valid;
      }
      //*********************************
      ETL_NODISCARD
      ETL_CONSTEXPR14
      TValue value() const
      {
        return integral_value;
      }

    private:

      etl::radix::value_type radix;
      TValue maximum;
      TValue integral_value;
      bool value_is_valid;
    };

    //***************************************************************************
    // Define an unsigned intermediate type that is at least as large as TValue.
    //***************************************************************************
    template <typename TValue>
    struct intermediate
    {
      typedef typename etl::smallest_uint_for_bits<etl::integral_limits<TValue>::bits>::type type;
    };

    //***************************************************************************
    /// Text to integral from view and radix value type.
    //***************************************************************************
    template <typename TChar, typename TAccumulatorType>
    ETL_NODISCARD
    ETL_CONSTEXPR14
    etl::optional<TAccumulatorType> to_arithmetic_integral(const etl::basic_string_view<TChar>& view,
                                                           const etl::radix::value_type         radix,
                                                           const TAccumulatorType               maximum)
    {
      etl::optional<TAccumulatorType> intermediate_result;

      etl::basic_string_view<TChar>::const_iterator       itr     = view.begin();
      const etl::basic_string_view<TChar>::const_iterator itr_end = view.end();

      if (itr != itr_end)
      {
        const char first_char = convert(*itr);

        integral_accumulator<TAccumulatorType> accumulator(radix, maximum);

        while ((itr != itr_end) && accumulator.add(convert(*itr)))
        {
          // Keep looping until done or an error occurs.
          ++itr;
        }

        if (accumulator.has_value())
        {
          intermediate_result = accumulator.value();
        }
      }

      return intermediate_result;
    }
  }

  //***************************************************************************
  /// Text to integral from view and radix value type.
  //***************************************************************************
  template <typename TValue, typename TChar>
  ETL_NODISCARD
  ETL_CONSTEXPR14
  typename etl::enable_if<etl::is_integral<TValue>::value, etl::optional<TValue> >::type
    to_arithmetic(etl::basic_string_view<TChar> view,
                  const etl::radix::value_type  radix)
  {
    using namespace etl::private_to_arithmetic;

    etl::optional<TValue> result;
       
    // Is this a negative number?
    const bool is_negative = check_and_remove_sign_prefix(view);

    // Make sure we're not trying to put a negative value into an unsigned type.
    if (!(is_negative && etl::is_unsigned<TValue>::value))
    {
      const bool is_decimal = (radix == etl::radix::decimal);

      // What's the maximum absolute value for the type value we're trying to convert to?
      typedef typename intermediate<TValue>::type intermediate_type;

      const intermediate_type maximum = is_negative ? etl::absolute_unsigned(etl::integral_limits<TValue>::min)
                                                    : is_decimal ? etl::integral_limits<TValue>::max
                                                                 : etl::integral_limits<typename etl::make_unsigned<TValue>::type>::max;
      // Do the conversion.
      etl::optional<intermediate_type> intermediate_result = to_arithmetic_integral<TChar>(view, radix, maximum);

      // Was it successful?
      if (intermediate_result.has_value())
      {
        // Convert from the intermediate type to the desired type.
        result = is_negative ? TValue(0) - intermediate_result.value() : etl::bit_cast<TValue>(intermediate_result.value());
      }
    }

    return result;
  }

  //***************************************************************************
  /// Text to integral from view and default decimal radix.
  //***************************************************************************
  template <typename TValue, typename TChar>
  ETL_NODISCARD
  ETL_CONSTEXPR14
  typename etl::enable_if<etl::is_integral<TValue>::value, etl::optional<TValue> >::type
    to_arithmetic(const etl::basic_string_view<TChar>& view)
  {
    return etl::to_arithmetic<TValue, TChar>(view, etl::radix::decimal);
  }

  //***************************************************************************
  /// Text to integral from view and radix format spec.
  //***************************************************************************
  template <typename TValue, typename TChar>
  ETL_NODISCARD
  ETL_CONSTEXPR14 
  typename etl::enable_if<etl::is_integral<TValue>::value, etl::optional<TValue> >::type
    to_arithmetic(const etl::basic_string_view<TChar>& view, const etl::basic_format_spec<etl::ibasic_string<TChar> >& spec)
  {
    return etl::to_arithmetic<TValue, TChar>(view, spec.get_base());
  }

  //***************************************************************************
  /// Text to integral from pointer, length and radix value type.
  //***************************************************************************
  template <typename TValue, typename TChar>
  ETL_NODISCARD
  ETL_CONSTEXPR14
  typename etl::enable_if<etl::is_integral<TValue>::value, etl::optional<TValue> >::type
    to_arithmetic(const TChar* cp, size_t length, const etl::radix::value_type radix)
  {
    return etl::to_arithmetic<TValue, TChar>(etl::basic_string_view<TChar>(cp, length), radix);
  }

  //***************************************************************************
  /// Text to integral from pointer, length and default decimal radix.
  //***************************************************************************
  template <typename TValue, typename TChar>
  ETL_NODISCARD
  ETL_CONSTEXPR14 typename etl::enable_if<etl::is_integral<TValue>::value, etl::optional<TValue> >::type
    to_arithmetic(const TChar* cp, size_t length)
  {
    return etl::to_arithmetic<TValue, TChar>(etl::basic_string_view<TChar>(cp, length), etl::radix::decimal);;
  }

  //***************************************************************************
  /// Text to integral from pointer, length and radix format spec.
  //***************************************************************************
  template <typename TValue, typename TChar>
  ETL_NODISCARD
  ETL_CONSTEXPR14 typename etl::enable_if<etl::is_integral<TValue>::value, etl::optional<TValue> >::type
    to_arithmetic(const TChar* cp, size_t length, const typename etl::private_basic_format_spec::base_spec& spec)
  {
    return etl::to_arithmetic<TValue, TChar>(etl::basic_string_view<TChar>(cp, length), spec.base);;
  }

  //***************************************************************************
  /// Text to integral from string and radix value type.
  //***************************************************************************
  template <typename TValue, typename TChar>
  ETL_NODISCARD
  ETL_CONSTEXPR14 typename etl::enable_if<etl::is_integral<TValue>::value, etl::optional<TValue> >::type
    to_arithmetic(const etl::ibasic_string<TChar>& str, const etl::radix::value_type radix)
  {
    return etl::to_arithmetic<TValue, TChar>(etl::basic_string_view<TChar>(str), radix);;
  }

  //***************************************************************************
  /// Text to integral from string and default decimal radix.
  //***************************************************************************
  template <typename TValue, typename TChar>
  ETL_NODISCARD
  ETL_CONSTEXPR14 typename etl::enable_if<etl::is_integral<TValue>::value, etl::optional<TValue> >::type
    to_arithmetic(const etl::ibasic_string<TChar>& str)
  {
    return etl::to_arithmetic<TValue, TChar>(etl::basic_string_view<TChar>(str), etl::radix::decimal);;
  }

  //***************************************************************************
  /// Text to integral from string and radix format spec.
  //***************************************************************************
  template <typename TValue, typename TChar>
  ETL_NODISCARD
  ETL_CONSTEXPR14 typename etl::enable_if<etl::is_integral<TValue>::value, etl::optional<TValue> >::type
    to_arithmetic(const etl::ibasic_string<TChar>& str, const etl::basic_format_spec<etl::ibasic_string<TChar> >& spec)
  {
    return etl::to_arithmetic<TValue, TChar>(etl::basic_string_view<TChar>(str), spec);;
  }

  //***************************************************************************
  /// Floating point from view.
  //***************************************************************************
  template <typename TValue, typename TChar>
  ETL_NODISCARD
  ETL_CONSTEXPR14 typename etl::enable_if<etl::is_floating_point<TValue>::value, etl::optional<TValue> >::type
    to_arithmetic(const etl::basic_string_view<TChar>& view)
  {
    using namespace etl::private_to_arithmetic;

    etl::optional<TValue>         result;

    bool is_negative = false;

    if (!view.empty())
    {
      is_negative = check_and_remove_sign_prefix(view);
    }

    if (!view.empty())
    {
      bool parsing = true;

      if (parsing)
      {
        // Integral part.


        // Fractional part.

        // Exponetial part.

      }
    }

    return result;
  }

  //***************************************************************************
  /// Floating point from pointer and length.
  //***************************************************************************
  template <typename TValue, typename TChar>
  ETL_NODISCARD
  ETL_CONSTEXPR14 typename etl::enable_if<etl::is_floating_point<TValue>::value, etl::optional<TValue> >::type
    to_arithmetic(const TChar* cp, size_t length)
  {
    return etl::to_arithmetic<TValue, TChar>(etl::basic_string_view<TChar>(cp, length));
  }

  //***************************************************************************
  /// Floating point from string.
  //***************************************************************************
  template <typename TValue, typename TChar>
  ETL_NODISCARD
  ETL_CONSTEXPR14 typename etl::enable_if<etl::is_floating_point<TValue>::value, etl::optional<TValue> >::type
    to_arithmetic(const etl::ibasic_string<TChar>& str)
  {
    return etl::to_arithmetic<TValue, TChar>(etl::basic_string_view<TChar>(str));
  }
}

#endif
