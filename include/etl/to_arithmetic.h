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
#include "limits.h"
#include "string_view.h"
#include "basic_string.h"
#include "format_spec.h"
#include "radix.h"
#include "string_utilities.h"
#include "iterator.h"
#include "bit.h"
#include "smallest.h"
#include "absolute.h"

namespace etl
{
  //***************************************************************************
  /// Status values for to_arithmetic.
  //***************************************************************************
  struct to_arithmetic_status
  {
    enum enum_type
    {
      Not_Set,
      Valid,
      Invalid_Format,
      Invalid_Float,
      Signed_To_Unsigned,
      Overflow
    };

    ETL_DECLARE_ENUM_TYPE(to_arithmetic_status, int)
    ETL_ENUM_TYPE(Not_Set,            "Not_Set")
    ETL_ENUM_TYPE(Valid,              "Valid")
    ETL_ENUM_TYPE(Invalid_Format,     "Invalid Format")
    ETL_ENUM_TYPE(Invalid_Float,      "Invalid Float")
    ETL_ENUM_TYPE(Signed_To_Unsigned, "Signed To Unsigned")
    ETL_ENUM_TYPE(Overflow,           "Overflow")
    ETL_END_ENUM_TYPE
  };

  //***************************************************************************
  /// Status values for to_arithmetic.
  //***************************************************************************
  template<typename TValue>
  class to_arithmetic_result
  {
  public:

    typedef TValue                    value_type;
    typedef etl::to_arithmetic_status status_type;

    //*******************************************
    /// Default constructor.
    //*******************************************
    to_arithmetic_result()
      : conversion_value(static_cast<value_type>(0))
      , conversion_status(status_type::Not_Set)
    {
    }

    //*******************************************
    /// Copy constructor.
    //*******************************************
    to_arithmetic_result(const to_arithmetic_result& other)
      : conversion_value(other.conversion_value)
      , conversion_status(other.conversion_status)
    {
    }

    //*******************************************
    /// Returns <b>true</b> if the result has a valid value.
    //*******************************************
    bool has_value() const
    {
      return (conversion_status == status_type::Valid);
    }

    //*******************************************
    /// Returns <b>true</b> if the result has a valid value.
    //*******************************************
    operator bool() const
    {
      return has_value();
    }

    //*******************************************
    /// Returns the value, if valid.
    /// Otherwise undefined.
    //*******************************************
    value_type value() const
    {
      return conversion_value;
    }

    //*******************************************
    /// Sets the value.
    //*******************************************
    void value(value_type value)
    {
      conversion_value = value;
    }

    //*******************************************
    /// Returns the value, if valid.
    /// Otherwise undefined.
    //*******************************************
    operator value_type() const
    {
      return value();
    }

    //*******************************************
    /// Returns the conversion status.
    /// One of the following:-
    /// Not_Set
    /// Valid
    /// Invalid_Format
    /// Signed_To_Unsigned
    /// Overflow
    //*******************************************
    status_type status() const
    {
      return conversion_status;
    }

    //*******************************************
    /// Sets the status.
    //*******************************************
    void status(status_type status_)
    {
      conversion_status = status_;
    }

  private:

    value_type  conversion_value;
    status_type conversion_status;
  };

  namespace private_to_arithmetic
  {
    //***********************************************************************
    /// 
    //***********************************************************************
    struct character_set
    {
      typedef char value_type;
      typedef const value_type* string_type;
      static ETL_CONSTANT string_type Valid_Chars          = "+-.,eE0123456789abcdefABCDEF";
      static ETL_CONSTANT int         Valid_Length         = etl::strlen(Valid_Chars);
      static ETL_CONSTANT value_type  Positive_Char        = '+';
      static ETL_CONSTANT value_type  Negative_Char        = '-';
      static ETL_CONSTANT value_type  Radix_Point1         = '.';
      static ETL_CONSTANT value_type  Radix_Point2         = ',';
      static ETL_CONSTANT value_type  Exponential          = 'e';
      static ETL_CONSTANT value_type  Unknown_Char         = '?';

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
    inline 
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
    inline 
    ETL_CONSTEXPR14
    char convert(char c)
    {
      return to_lower(c);
    }

    //*******************************************
    ETL_NODISCARD
    inline
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
    inline
    ETL_CONSTEXPR14
    char convert(char16_t c)
    {
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
    inline
    ETL_CONSTEXPR14
    char convert(char32_t c)
    {
      ETL_STATIC_CONSTANT char32_t* Valid_Chars = U"+-.,eE0123456789abcdefABCDEF";

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
    template <typename TChar>
    ETL_NODISCARD
    ETL_CONSTEXPR14
    bool check_and_remove_sign_prefix(etl::basic_string_view<TChar>& view)
    {
      if (!view.empty())
      {
        // Check for prefix.
        const char c = convert(view[0]);
        const bool has_positive_prefix = (c == character_set::Positive_Char);
        const bool has_negative_prefix = (c == character_set::Negative_Char);

        // Step over the prefix, if present.
        if (has_positive_prefix || has_negative_prefix)
        {
          view.remove_prefix(1);
          return has_negative_prefix;
        }
      }

      return false;
    }
 
    //***************************************************************************
    /// 
    //***************************************************************************
    ETL_NODISCARD
    inline
    ETL_CONSTEXPR14
    bool is_valid_radix(const etl::radix::value_type radix)
    {
      return (radix == etl::radix::binary)  ||
             (radix == etl::radix::octal)   ||
             (radix == etl::radix::decimal) ||
             (radix == etl::radix::hex);
    }

    //***************************************************************************
    /// Accumulate integrals
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
        , conversion_status(to_arithmetic_status::Valid)
      {
      }

      //*********************************
      ETL_NODISCARD
      ETL_CONSTEXPR14
      bool add(const char c)
      {
        bool is_success      = false;
        bool is_not_overflow = false;

        const bool is_valid_char = character_set::is_valid(c, radix);

        if (is_valid_char)
        {
          TValue old_value = integral_value;
          integral_value *= radix;

          // No multipication overflow?
          is_not_overflow = ((integral_value / radix) == old_value);

          if (is_not_overflow)
          {
            const char digit = character_set::digit_value(c, radix);

            // No addition overflow?
            is_not_overflow = ((maximum - digit) >= integral_value);

            if ((maximum - digit) >= integral_value)
            {
              integral_value += digit;
              is_success = true;
            }
          }
        }

        // Check the status of the conversion.
        if (is_valid_char == false)
        {
          conversion_status = to_arithmetic_status::Invalid_Format;
        }
        else if (is_not_overflow == false)
        {
          conversion_status = to_arithmetic_status::Overflow;
        }

        return is_success;
      }

      //*********************************
      ETL_NODISCARD
        ETL_CONSTEXPR14
        bool has_value() const
      {
        return conversion_status == to_arithmetic_status::Valid;
      }

      //*********************************
      ETL_NODISCARD
        ETL_CONSTEXPR14
        TValue value() const
      {
        return integral_value;
      }

      //*********************************
      ETL_NODISCARD
        ETL_CONSTEXPR14
        to_arithmetic_status status() const
      {
        return conversion_status;
      }

    private:

      etl::radix::value_type radix;
      TValue maximum;
      TValue integral_value;
      to_arithmetic_status conversion_status;
    };

    //***************************************************************************
    /// Accumulate floating point
    //***************************************************************************
    struct floating_point_accumulator
    {
      //*********************************
      ETL_NODISCARD
      ETL_CONSTEXPR14
      floating_point_accumulator()
        : divisor(1)
        , floating_point_value(0)
        , is_negative_mantissa(false)
        , is_negative_exponent(false)
        , exponent_value(0)
        , conversion_status(to_arithmetic_status::Valid)
        , state(Integral)
        , expecting_sign(true)
      {
      }

      //*********************************
      ETL_NODISCARD
      ETL_CONSTEXPR14
      bool add(char c)
      {
        bool is_success = true;

        switch (state)
        {
          //***************************
          case Integral:
          {
            if (expecting_sign && ((c == character_set::Positive_Char) || (c == character_set::Negative_Char)))
            {
              is_negative_mantissa = (c == character_set::Negative_Char);
              expecting_sign = false;
            }
            // Radix point?
            else if ((c == character_set::Radix_Point1) || (c == character_set::Radix_Point2))
            {
              expecting_sign = false;
              state = Fractional;
            }
            // Exponential?
            else if (c == character_set::Exponential)
            {
              expecting_sign = true;
              state = Exponential;
            }
            else if (character_set::is_valid(c, etl::radix::decimal))
            {
              const char digit = character_set::digit_value(c, etl::radix::decimal);
              floating_point_value *= 10;
              is_negative_mantissa ? floating_point_value -= digit : floating_point_value += digit;
              conversion_status = to_arithmetic_status::Valid;
              expecting_sign = false;
            }
            else
            {
              conversion_status = to_arithmetic_status::Invalid_Format;
              is_success = false;
            }
            break;
          }

          //***************************
          case Fractional:
          {
            // Radix point?
            if ((c == character_set::Radix_Point1) || (c == character_set::Radix_Point2))
            {
              conversion_status = to_arithmetic_status::Invalid_Format;
              is_success = false;
            }
            // Exponential?
            else if (c == character_set::Exponential)
            {
              expecting_sign = true;
              state = Exponential;
            }
            else if (character_set::is_valid(c, etl::radix::decimal))
            {
              const char digit = character_set::digit_value(c, etl::radix::decimal);
              divisor *= 10;
              long double fraction = digit / divisor;
              is_negative_mantissa ? floating_point_value -= fraction : floating_point_value += fraction;
              conversion_status = to_arithmetic_status::Valid;
            }
            else
            {
              conversion_status = to_arithmetic_status::Invalid_Format;
              is_success = false;
            }
            break;
          }

          //***************************
          case Exponential:
          {
            if (expecting_sign && ((c == character_set::Positive_Char) || (c == character_set::Negative_Char)))
            {
              is_negative_exponent = (c == character_set::Negative_Char);
              expecting_sign = false;
            }
            // Radix point?
            else if ((c == character_set::Radix_Point1) || (c == character_set::Radix_Point2) || (c == character_set::Exponential))
            {
              conversion_status = to_arithmetic_status::Invalid_Format;
              is_success = false;
            }
            else if (character_set::is_valid(c, etl::radix::decimal))
            {
              const char digit = character_set::digit_value(c, etl::radix::decimal);
              exponent_value *= etl::radix::decimal;
              is_negative_exponent ? exponent_value -= digit : exponent_value += digit;
            }
            else
            {
              conversion_status = to_arithmetic_status::Invalid_Format;
              is_success = false;
            }
            break;
          }

          //***************************
          default:
          {
            is_success = false;
            break;
          }
        }

        return is_success;
      }

      //*********************************
      ETL_NODISCARD
      ETL_CONSTEXPR14
      bool has_value() const
      {
        return (conversion_status == to_arithmetic_status::Valid);
      }

      //*********************************
      ETL_NODISCARD
      ETL_CONSTEXPR14
      long double value() const
      {
        return floating_point_value;
      }

      //*********************************
      ETL_NODISCARD
      ETL_CONSTEXPR14
      to_arithmetic_status status() const
      {
        return conversion_status;
      }

      //*********************************
      ETL_NODISCARD
      ETL_CONSTEXPR14
      int exponent() const
      {
        return exponent_value;
      }

    private:

      enum
      {
        Integral,
        Fractional,
        Exponential
      };

      long double divisor;
      long double floating_point_value;
      bool   is_negative_mantissa;
      bool   is_negative_exponent;
      int    exponent_value;
      to_arithmetic_status conversion_status;
      int state;
      bool expecting_sign;
    };

    //***************************************************************************
    // Define an unsigned accumulator type that is at least as large as TValue.
    //***************************************************************************
    template <typename TValue>
    struct accumulator_type_select
    {
      typedef typename etl::smallest_uint_for_bits<etl::integral_limits<TValue>::bits>::type type;
    };

    //***************************************************************************
    /// Text to integral from view, radix value and maximum.
    //***************************************************************************
    template <typename TChar, typename TAccumulatorType>
    ETL_NODISCARD
    ETL_CONSTEXPR14
    etl::to_arithmetic_result<TAccumulatorType> to_arithmetic_integral(const etl::basic_string_view<TChar>& view,
                                                                       const etl::radix::value_type         radix,
                                                                       const TAccumulatorType               maximum)
    {
      etl::to_arithmetic_result<TAccumulatorType> accumulator_result;

      etl::basic_string_view<TChar>::const_iterator       itr     = view.begin();
      const etl::basic_string_view<TChar>::const_iterator itr_end = view.end();

      integral_accumulator<TAccumulatorType> accumulator(radix, maximum);

      while ((itr != itr_end) && accumulator.add(convert(*itr)))
      {
        // Keep looping until done or an error occurs.
        ++itr;
      }

      accumulator_result.status(accumulator.status());

      if (accumulator.has_value())
      {
        accumulator_result.value(accumulator.value());
      }

      return accumulator_result;
    }
  }

  //***************************************************************************
  /// Text to integral from view and radix value type.
  //***************************************************************************
  template <typename TValue, typename TChar>
  ETL_NODISCARD
  ETL_CONSTEXPR14
  typename etl::enable_if<etl::is_integral<TValue>::value, etl::to_arithmetic_result<TValue> >::type
    to_arithmetic(etl::basic_string_view<TChar> view,
                  const etl::radix::value_type  radix)
  {
    using namespace etl::private_to_arithmetic;

    etl::to_arithmetic_result<TValue> result;
       
    // Is this a negative number?
    const bool is_negative = check_and_remove_sign_prefix(view);

    if (view.empty())
    {
      result.status(to_arithmetic_status::Invalid_Format);
    }
    else
    {
      // Make sure we're not trying to put a negative value into an unsigned type.
      if (is_negative && etl::is_unsigned<TValue>::value)
      {
        result.status(to_arithmetic_status::Signed_To_Unsigned);
      }
      else
      {
        const bool is_decimal = (radix == etl::radix::decimal);

        // What's the maximum absolute value for the type value we're trying to convert to?
        typedef typename accumulator_type_select<TValue>::type accumulator_type;

        const accumulator_type maximum = is_negative ? etl::absolute_unsigned(etl::integral_limits<TValue>::min)
                                                     : is_decimal ? etl::integral_limits<TValue>::max
                                                                  : etl::integral_limits<typename etl::make_unsigned<TValue>::type>::max;
        // Do the conversion.
        etl::to_arithmetic_result<accumulator_type> accumulator_result = to_arithmetic_integral<TChar>(view, radix, maximum);

        result.status(accumulator_result.status());

        // Was it successful?
        if (accumulator_result.has_value())
        {
          // Convert from the accumulator type to the desired type.
          result.value(is_negative ? static_cast<TValue>(0) - accumulator_result.value() : etl::bit_cast<TValue>(accumulator_result.value()));
        }
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
  typename etl::enable_if<etl::is_integral<TValue>::value, etl::to_arithmetic_result<TValue> >::type
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
  typename etl::enable_if<etl::is_integral<TValue>::value, etl::to_arithmetic_result<TValue> >::type
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
  typename etl::enable_if<etl::is_integral<TValue>::value, etl::to_arithmetic_result<TValue> >::type
    to_arithmetic(const TChar* cp, size_t length, const etl::radix::value_type radix)
  {
    return etl::to_arithmetic<TValue, TChar>(etl::basic_string_view<TChar>(cp, length), radix);
  }

  //***************************************************************************
  /// Text to integral from pointer, length and default decimal radix.
  //***************************************************************************
  template <typename TValue, typename TChar>
  ETL_NODISCARD
  ETL_CONSTEXPR14 
  typename etl::enable_if<etl::is_integral<TValue>::value, etl::to_arithmetic_result<TValue> >::type
    to_arithmetic(const TChar* cp, size_t length)
  {
    return etl::to_arithmetic<TValue, TChar>(etl::basic_string_view<TChar>(cp, length), etl::radix::decimal);
  }

  //***************************************************************************
  /// Text to integral from pointer, length and radix format spec.
  //***************************************************************************
  template <typename TValue, typename TChar>
  ETL_NODISCARD
  ETL_CONSTEXPR14 
  typename etl::enable_if<etl::is_integral<TValue>::value, etl::to_arithmetic_result<TValue> >::type
    to_arithmetic(const TChar* cp, size_t length, const typename etl::private_basic_format_spec::base_spec& spec)
  {
    return etl::to_arithmetic<TValue, TChar>(etl::basic_string_view<TChar>(cp, length), spec.base);
  }

  //***************************************************************************
  /// Text to integral from pointer and radix value type.
  //***************************************************************************
  template <typename TValue, typename TChar>
  ETL_NODISCARD
  ETL_CONSTEXPR14
  typename etl::enable_if<etl::is_integral<TValue>::value, etl::to_arithmetic_result<TValue> >::type
    to_arithmetic(const TChar* cp, const etl::radix::value_type radix)
  {
    return etl::to_arithmetic<TValue, TChar>(etl::basic_string_view<TChar>(cp, etl::strlen<TChar>(cp), radix));
  }

  //***************************************************************************
  /// Text to integral from pointer and default decimal radix.
  //***************************************************************************
  template <typename TValue, typename TChar>
  ETL_NODISCARD
  ETL_CONSTEXPR14 
  typename etl::enable_if<etl::is_integral<TValue>::value, etl::to_arithmetic_result<TValue> >::type
    to_arithmetic(const TChar* cp)
  {
    return etl::to_arithmetic<TValue, TChar>(etl::basic_string_view<TChar>(cp, etl::strlen<TChar>(cp)), etl::radix::decimal);;
  }

  //***************************************************************************
  /// Text to integral from pointer and radix format spec.
  //***************************************************************************
  template <typename TValue, typename TChar>
  ETL_NODISCARD
  ETL_CONSTEXPR14 
  typename etl::enable_if<etl::is_integral<TValue>::value, etl::to_arithmetic_result<TValue> >::type
    to_arithmetic(const TChar* cp, const typename etl::private_basic_format_spec::base_spec& spec)
  {
    return etl::to_arithmetic<TValue, TChar>(etl::basic_string_view<TChar>(cp, etl::strlen<TChar>(cp)), spec.base);;
  }

  //***************************************************************************
  /// Text to integral from string and radix value type.
  //***************************************************************************
  template <typename TValue, typename TChar>
  ETL_NODISCARD
  ETL_CONSTEXPR14 
  typename etl::enable_if<etl::is_integral<TValue>::value, etl::to_arithmetic_result<TValue> >::type
    to_arithmetic(const etl::ibasic_string<TChar>& str, const etl::radix::value_type radix)
  {
    return etl::to_arithmetic<TValue, TChar>(etl::basic_string_view<TChar>(str), radix);;
  }

  //***************************************************************************
  /// Text to integral from string and default decimal radix.
  //***************************************************************************
  template <typename TValue, typename TChar>
  ETL_NODISCARD
  ETL_CONSTEXPR14 
  typename etl::enable_if<etl::is_integral<TValue>::value, etl::to_arithmetic_result<TValue> >::type
    to_arithmetic(const etl::ibasic_string<TChar>& str)
  {
    return etl::to_arithmetic<TValue, TChar>(etl::basic_string_view<TChar>(str), etl::radix::decimal);;
  }

  //***************************************************************************
  /// Text to integral from string and radix format spec.
  //***************************************************************************
  template <typename TValue, typename TChar>
  ETL_NODISCARD
  ETL_CONSTEXPR14 
  typename etl::enable_if<etl::is_integral<TValue>::value, etl::to_arithmetic_result<TValue> >::type
    to_arithmetic(const etl::ibasic_string<TChar>& str, const etl::basic_format_spec<etl::ibasic_string<TChar> >& spec)
  {
    return etl::to_arithmetic<TValue, TChar>(etl::basic_string_view<TChar>(str), spec);;
  }

  //***************************************************************************
  /// Floating point from view.
  //***************************************************************************
  template <typename TValue, typename TChar>
  ETL_NODISCARD
  ETL_CONSTEXPR14 
  typename etl::enable_if<etl::is_floating_point<TValue>::value, etl::to_arithmetic_result<TValue> >::type
    to_arithmetic(etl::basic_string_view<TChar> view)
  {
    using namespace etl::private_to_arithmetic;

    etl::to_arithmetic_result<TValue> result;

    if (view.empty())
    {
      result.status(to_arithmetic_status::Invalid_Format);
    }
    else
    {
      floating_point_accumulator accumulator;

      etl::basic_string_view<TChar>::const_iterator itr     = view.begin();
      etl::basic_string_view<TChar>::const_iterator itr_end = view.end();

      while ((itr != itr_end) && accumulator.add(convert(*itr)))
      {
        // Keep looping until done or an error occurs.
        ++itr;
      }

      result.status(accumulator.status());

      if (result.has_value())
      {
        TValue value = static_cast<TValue>(accumulator.value());
        int exponent = accumulator.exponent();

        value *= pow(10.0, exponent);

        // Check that the result is a valid floating point number.
        if (value == etl::numeric_limits<TValue>::infinity())
        {
          result.status(to_arithmetic_status::Overflow);
        }
        else if (value == etl::numeric_limits<TValue>::quiet_NaN())
        {
          result.status(to_arithmetic_status::Invalid_Float);
        }
        else
        {
          result.value(value);
        }
      }
    }

    return result;
  }

  //***************************************************************************
  /// Floating point from pointer and length.
  //***************************************************************************
  template <typename TValue, typename TChar>
  ETL_NODISCARD
  ETL_CONSTEXPR14 
  typename etl::enable_if<etl::is_floating_point<TValue>::value, etl::to_arithmetic_result<TValue> >::type
    to_arithmetic(const TChar* cp, size_t length)
  {
    return etl::to_arithmetic<TValue, TChar>(etl::basic_string_view<TChar>(cp, length));
  }

  //***************************************************************************
  /// Floating point from pointer.
  //***************************************************************************
  template <typename TValue, typename TChar>
  ETL_NODISCARD
  ETL_CONSTEXPR14
  typename etl::enable_if<etl::is_floating_point<TValue>::value, etl::to_arithmetic_result<TValue> >::type
    to_arithmetic(const TChar* cp)
  {
    return etl::to_arithmetic<TValue, TChar>(etl::basic_string_view<TChar>(cp, etl::strlen<TChar>(cp)));
  }

  //***************************************************************************
  /// Floating point from string.
  //***************************************************************************
  template <typename TValue, typename TChar>
  ETL_NODISCARD
  ETL_CONSTEXPR14 
  typename etl::enable_if<etl::is_floating_point<TValue>::value, etl::to_arithmetic_result<TValue> >::type
    to_arithmetic(const etl::ibasic_string<TChar>& str)
  {
    return etl::to_arithmetic<TValue, TChar>(etl::basic_string_view<TChar>(str));
  }
}

//***************************************************************************
/// Equality test for etl::to_arithmetic_result
//***************************************************************************
template <typename T>
ETL_CONSTEXPR14 bool operator ==(const etl::to_arithmetic_result<T>& lhs, const etl::to_arithmetic_result<T>& rhs)
{
  if (lhs.has_value() && rhs.has_value())
  {
    return (lhs.value() == rhs.value());
  }
  else
  {
    return (lhs.status() == rhs.status());
  }
}

//***************************************************************************
/// Equality test for etl::to_arithmetic_result
//***************************************************************************
template <typename T, typename U>
ETL_CONSTEXPR14 bool operator ==(const etl::to_arithmetic_result<T>& lhs, const U& rhs)
{
  return bool(lhs) ? lhs.value() == rhs : false;
}

//***************************************************************************
/// Equality test for etl::to_arithmetic_result
//***************************************************************************
template <typename T, typename U>
ETL_CONSTEXPR14 bool operator ==(const T& lhs, const etl::to_arithmetic_result<U>& rhs)
{
  return bool(rhs) ? rhs.value() == lhs : false;
}

//***************************************************************************
/// Inequality test for etl::to_arithmetic_result
//***************************************************************************
template <typename T>
ETL_CONSTEXPR14 bool operator !=(const etl::to_arithmetic_result<T>& lhs, const etl::to_arithmetic_result<T>& rhs)
{
  return !(lhs == rhs);
}

//***************************************************************************
/// Inequality test for etl::to_arithmetic_result
//***************************************************************************
template <typename T, typename U>
ETL_CONSTEXPR14 bool operator !=(const etl::to_arithmetic_result<T>& lhs, const U& rhs)
{
  return !(lhs == rhs);
}

//***************************************************************************
/// Inequality test for etl::to_arithmetic_result
//***************************************************************************
template <typename T, typename U>
ETL_CONSTEXPR14 bool operator !=(const T& lhs, const etl::to_arithmetic_result<T>& rhs)
{
  return !(lhs == rhs);
}

#endif
