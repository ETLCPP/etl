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
#include "expected.h"
#include "math.h"

#include <math.h>

namespace etl
{
  //***************************************************************************
  /// Status values for to_arithmetic.
  //***************************************************************************
  struct to_arithmetic_status
  {
    enum enum_type
    {
      Valid,
      Invalid_Radix,
      Invalid_Format,
      Invalid_Float,
      Signed_To_Unsigned,
      Overflow
    };

    ETL_DECLARE_ENUM_TYPE(to_arithmetic_status, int)
    ETL_ENUM_TYPE(Valid,              "Valid")
    ETL_ENUM_TYPE(Invalid_Radix,      "Invalid Radix")
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

    typedef TValue                                     value_type;
    typedef etl::to_arithmetic_status                  error_type;
    typedef etl::unexpected<etl::to_arithmetic_status> unexpected_type;

    //*******************************************
    /// Default constructor.
    //*******************************************
    ETL_CONSTEXPR14
    to_arithmetic_result()
      : conversion_value(static_cast<value_type>(0))
      , conversion_status(error_type::Valid)
    {
    }

    //*******************************************
    /// Copy constructor.
    //*******************************************
    ETL_CONSTEXPR14
    to_arithmetic_result(const to_arithmetic_result& other)
      : conversion_value(other.conversion_value)
      , conversion_status(other.conversion_status)
    {
    }

    //*******************************************
    /// Returns <b>true</b> if the result has a valid value.
    //*******************************************
    ETL_NODISCARD
    ETL_CONSTEXPR14
    bool has_value() const
    {
      return (conversion_status.error() == error_type::Valid);
    }

    //*******************************************
    /// Returns <b>true</b> if the result has a valid value.
    //*******************************************
    ETL_NODISCARD
    ETL_CONSTEXPR14
    operator bool() const
    {
      return has_value();
    }

    //*******************************************
    /// Returns the value, if valid.
    /// Otherwise undefined.
    //*******************************************
    ETL_NODISCARD
    ETL_CONSTEXPR14
    value_type value() const
    {
      return conversion_value;
    }

    //*******************************************
    /// Returns the value, if valid.
    /// Otherwise undefined.
    //*******************************************
    ETL_NODISCARD
    ETL_CONSTEXPR14
    operator value_type() const
    {
      return value();
    }

    //*******************************************
    /// Returns the conversion status.
    /// One of the following:-
    /// Valid
    /// Invalid_Radix
    /// Invalid_Format
    /// Invalid_Float
    /// Signed_To_Unsigned
    /// Overflow
    //*******************************************
    ETL_NODISCARD
    ETL_CONSTEXPR14
    error_type error() const
    {
      return etl::to_arithmetic_status(conversion_status.error());
    }

    //*******************************************
    /// Assignment from a value.
    //*******************************************
    ETL_CONSTEXPR14
    to_arithmetic_result& operator =(value_type value_)
    {
      conversion_value = value_;

      return *this;
    }

    //*******************************************
    /// Assignment from an unexpected_type.
    //*******************************************
    ETL_CONSTEXPR14
    to_arithmetic_result& operator =(unexpected_type status_)
    {
      conversion_status = status_;

      return *this;
    }

  private:

    value_type      conversion_value;
    unexpected_type conversion_status;
  };

  namespace private_to_arithmetic
  {
    template <typename T = void>
    struct char_statics
    {
      static ETL_CONSTANT char Positive_Char = '+';
      static ETL_CONSTANT char Negative_Char = '-';
      static ETL_CONSTANT char Radix_Point1_Char = '.';
      static ETL_CONSTANT char Radix_Point2_Char = ',';
      static ETL_CONSTANT char Exponential_Char = 'e';
    };

    template <typename T>
    ETL_CONSTANT char char_statics<T>::Positive_Char;

    template <typename T>
    ETL_CONSTANT char char_statics<T>::Negative_Char;

    template <typename T>
    ETL_CONSTANT char char_statics<T>::Radix_Point1_Char;

    template <typename T>
    ETL_CONSTANT char char_statics<T>::Radix_Point2_Char;

    template <typename T>
    ETL_CONSTANT char char_statics<T>::Exponential_Char;

    struct char_constant : char_statics<>
    {
    };

    //*******************************************
    ETL_NODISCARD
    inline
    ETL_CONSTEXPR14
    bool is_valid(char c, etl::radix::value_type radix)
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

        case etl::radix::hexadecimal:
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
    inline
    ETL_CONSTEXPR14
    char digit_value(char c, etl::radix::value_type radix)
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

        case etl::radix::hexadecimal:
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
    template <typename TChar>
    ETL_NODISCARD
    ETL_CONSTEXPR14
    char convert(TChar c)
    {
      return to_lower(static_cast<char>(c));
    }

    //***************************************************************************
    /// Checks to see if the text starts with a '+' or '-' prefix, and modifies the view to remove it.
    /// Returns true if the text has a '-' prefix.
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
        const bool has_positive_prefix = (c == char_constant::Positive_Char);
        const bool has_negative_prefix = (c == char_constant::Negative_Char);

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
    /// Checks to see if the radix is valid.
    //***************************************************************************
    ETL_NODISCARD
    inline
    ETL_CONSTEXPR14
    bool is_valid_radix(const etl::radix::value_type radix)
    {
      return (radix == etl::radix::binary)  ||
             (radix == etl::radix::octal)   ||
             (radix == etl::radix::decimal) ||
             (radix == etl::radix::hexadecimal);
    }

    //***************************************************************************
    /// Accumulate integrals
    //***************************************************************************
    template <typename TValue>
    struct integral_accumulator
    {
      //*********************************
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

        const bool is_valid_char = is_valid(c, radix);

        if (is_valid_char)
        {
          TValue old_value = integral_value;
          integral_value *= radix;

          // No multiplication overflow?
          is_not_overflow = ((integral_value / radix) == old_value);

          if (is_not_overflow)
          {
            const char digit = digit_value(c, radix);

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
      ETL_CONSTEXPR14
      floating_point_accumulator()
        : divisor(1)
        , floating_point_value(0)
        , is_negative_mantissa(false)
        , is_negative_exponent(false)
        , expecting_sign(true)
        , exponent_value(0)
        , state(Parsing_Integral)
        , conversion_status(to_arithmetic_status::Valid)
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
          case Parsing_Integral:
          {
            if (expecting_sign && ((c == char_constant::Positive_Char) || (c == char_constant::Negative_Char)))
            {
              is_negative_mantissa = (c == char_constant::Negative_Char);
              expecting_sign = false;
            }
            // Radix point?
            else if ((c == char_constant::Radix_Point1_Char) || (c == char_constant::Radix_Point2_Char))
            {
              expecting_sign = false;
              state = Parsing_Fractional;
            }
            // Exponential?
            else if (c == char_constant::Exponential_Char)
            {
              expecting_sign = true;
              state = Parsing_Exponential;
            }
            else if (is_valid(c, etl::radix::decimal))
            {
              const char digit = digit_value(c, etl::radix::decimal);
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
          case Parsing_Fractional:
          {
            // Radix point?
            if ((c == char_constant::Radix_Point1_Char) || (c == char_constant::Radix_Point2_Char))
            {
              conversion_status = to_arithmetic_status::Invalid_Format;
              is_success = false;
            }
            // Exponential?
            else if (c == char_constant::Exponential_Char)
            {
              expecting_sign = true;
              state = Parsing_Exponential;
            }
            else if (is_valid(c, etl::radix::decimal))
            {
              const char digit = digit_value(c, etl::radix::decimal);
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
          case Parsing_Exponential:
          {
            if (expecting_sign && ((c == char_constant::Positive_Char) || (c == char_constant::Negative_Char)))
            {
              is_negative_exponent = (c == char_constant::Negative_Char);
              expecting_sign = false;
            }
            // Radix point?
            else if ((c == char_constant::Radix_Point1_Char) || (c == char_constant::Radix_Point2_Char) || (c == char_constant::Exponential_Char))
            {
              conversion_status = to_arithmetic_status::Invalid_Format;
              is_success = false;
            }
            else if (is_valid(c, etl::radix::decimal))
            {
              const char digit = digit_value(c, etl::radix::decimal);
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
        Parsing_Integral,
        Parsing_Fractional,
        Parsing_Exponential
      };

      long double divisor;
      long double floating_point_value;
      bool is_negative_mantissa;
      bool is_negative_exponent;
      bool expecting_sign;
      int  exponent_value;
      int  state;
      to_arithmetic_status conversion_status;
    };

    //***************************************************************************
    // Define an unsigned accumulator type that is at least as large as TValue.
    //***************************************************************************
    template <size_t Bits>
    struct accumulator_type_select;

    template <>
    struct accumulator_type_select<8U>
    {
      typedef uint32_t type;
    };

    template <>
    struct accumulator_type_select<16U>
    {
      typedef uint32_t type;
    };

    template <>
    struct accumulator_type_select<32U>
    {
      typedef uint32_t type;
    };

#if ETL_USING_64BIT_TYPES
    template <>
    struct accumulator_type_select<64U>
    {
      typedef uint64_t type;
    };
#endif

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
      etl::to_arithmetic_result<TAccumulatorType>   accumulator_result;
      typedef typename etl::unexpected<etl::to_arithmetic_status> unexpected_type;

      typename etl::basic_string_view<TChar>::const_iterator       itr     = view.begin();
      const typename etl::basic_string_view<TChar>::const_iterator itr_end = view.end();

      integral_accumulator<TAccumulatorType> accumulator(radix, maximum);

      while ((itr != itr_end) && accumulator.add(convert(*itr)))
      {
        // Keep looping until done or an error occurs.
        ++itr;
      }

      if (accumulator.has_value())
      {
        accumulator_result = accumulator.value();
      }
      else
      {
        accumulator_result = unexpected_type(accumulator.status());
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

    typedef etl::to_arithmetic_result<TValue>     result_type;
    typedef typename result_type::unexpected_type unexpected_type;

    result_type result;    

    if (is_valid_radix(radix))
    {
      // Is this a negative number?
      const bool is_negative = check_and_remove_sign_prefix(view);

      if (view.empty())
      {
        result = unexpected_type(to_arithmetic_status::Invalid_Format);
      }
      else
      {
        // Make sure we're not trying to put a negative value into an unsigned type.
        if (is_negative && etl::is_unsigned<TValue>::value)
        {
          result = unexpected_type(to_arithmetic_status::Signed_To_Unsigned);
        }
        else
        {
          const bool is_decimal = (radix == etl::radix::decimal);

          // Select the type we use for the accumulator.
          typedef typename accumulator_type_select<etl::integral_limits<TValue>::bits>::type accumulator_type;

          // Find the maximum absolute value for the type value we're trying to convert to.
          const accumulator_type maximum = is_negative ? etl::absolute_unsigned(etl::integral_limits<TValue>::min)
                                                       : is_decimal ? etl::integral_limits<TValue>::max
                                                                    : etl::integral_limits<typename etl::make_unsigned<TValue>::type>::max;
          // Do the conversion.
          etl::to_arithmetic_result<accumulator_type> accumulator_result = to_arithmetic_integral<TChar>(view, radix, maximum);

          result = unexpected_type(accumulator_result.error());

          // Was it successful?
          if (accumulator_result.has_value())
          {
            typedef typename etl::make_unsigned<TValue>::type uvalue_t;
            const uvalue_t uvalue = static_cast<uvalue_t>(accumulator_result.value());

            // Convert from the accumulator type to the desired type.
            result = (is_negative ? static_cast<TValue>(0) - uvalue : etl::bit_cast<TValue>(uvalue));
          }
        }
      }
    }
    else
    {
      result = unexpected_type(to_arithmetic_status::Invalid_Radix);
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
    to_arithmetic(const etl::basic_string_view<TChar>& view, const typename etl::private_basic_format_spec::base_spec& spec)
  {
    return etl::to_arithmetic<TValue, TChar>(view, spec.base);
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
    to_arithmetic(const etl::ibasic_string<TChar>& str, const typename etl::private_basic_format_spec::base_spec& spec)
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

    typedef etl::to_arithmetic_result<TValue>     result_type;
    typedef typename result_type::unexpected_type unexpected_type;

    result_type result;

    if (view.empty())
    {
      result = unexpected_type(to_arithmetic_status::Invalid_Format);
    }
    else
    {
      floating_point_accumulator accumulator;

      typename etl::basic_string_view<TChar>::const_iterator itr           = view.begin();
      const typename etl::basic_string_view<TChar>::const_iterator itr_end = view.end();

      while ((itr != itr_end) && accumulator.add(convert(*itr)))
      {
        // Keep looping until done or an error occurs.
        ++itr;
      }

      result = unexpected_type(accumulator.status());

      if (result.has_value())
      {
        TValue value = static_cast<TValue>(accumulator.value());
        int exponent = accumulator.exponent();

        value *= pow(static_cast<TValue>(10.0), static_cast<TValue>(exponent));

        // Check that the result is a valid floating point number.
        if (etl::is_infinity(value))
        {
          result = unexpected_type(to_arithmetic_status::Overflow);
        }
        else if (etl::is_nan(value))
        {
          result = unexpected_type(to_arithmetic_status::Invalid_Float);
        }
        else
        {
          result = value;
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
