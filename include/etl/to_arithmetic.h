
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
#include "exception.h"
#include "error_handler.h"

namespace etl
{
  //***************************************************************************
  /// 
  //***************************************************************************
  class to_arithmetic_exception : public etl::exception
  {
  public:

    to_arithmetic_exception(string_type reason_, string_type file_name_, numeric_type line_number_)
      : exception(reason_, file_name_, line_number_)
    {
    }
  };

  //***************************************************************************
  /// 
  //***************************************************************************
  class to_arithmetic_signed_to_unsigned : public to_arithmetic_exception
  {
  public:

    to_arithmetic_signed_to_unsigned(string_type file_name_, numeric_type line_number_)
      : to_arithmetic_exception(ETL_ERROR_TEXT("to arithmetic:signed to unsigned", ETL_TO_ARITHMETIC_FILE_ID"A"), file_name_, line_number_)
    {
    }
  };

  //***************************************************************************
  /// 
  //***************************************************************************
  class to_arithmetic_invalid_format : public to_arithmetic_exception
  {
  public:

    to_arithmetic_invalid_format(string_type file_name_, numeric_type line_number_)
      : to_arithmetic_exception(ETL_ERROR_TEXT("to arithmetic:invalid format", ETL_TO_ARITHMETIC_FILE_ID"B"), file_name_, line_number_)
    {
    }
  };

  //***************************************************************************
  /// 
  //***************************************************************************
  class to_arithmetic_invalid_radix : public to_arithmetic_exception
  {
  public:

    to_arithmetic_invalid_radix(string_type file_name_, numeric_type line_number_)
      : to_arithmetic_exception(ETL_ERROR_TEXT("to arithmetic:radix not supported", ETL_TO_ARITHMETIC_FILE_ID"C"), file_name_, line_number_)
    {
    }
  };

  namespace private_to_arithmetic
  {
    static ETL_CONSTANT char binary_length  = 2;
    static ETL_CONSTANT char octal_length   = 8;
    static ETL_CONSTANT char decimal_length = 10;
    static ETL_CONSTANT char hex_length     = 16;
    static ETL_CONSTANT int  valid_length   = 28;

    //***********************************************************************
    /// 
    //***********************************************************************
    struct valid_character_set
    {
      typedef char value_type;
      typedef const value_type* string_type;
      static ETL_CONSTANT string_type valid_chars   = "+-.,eE0123456789abcdefABCDEF";
      static ETL_CONSTANT string_type numeric_chars = "0123456789abcdef";
      static ETL_CONSTANT value_type  positive_char = '+';
      static ETL_CONSTANT value_type  negative_char = '-';
      static ETL_CONSTANT value_type  radix_point1  = '.';
      static ETL_CONSTANT value_type  radix_point2  = ',';
      static ETL_CONSTANT value_type  exponential   = 'e';
      static ETL_CONSTANT value_type  unknown_char  = '?';

      //*******************************************
      ETL_NODISCARD
      ETL_CONSTEXPR14
      bool is_valid(char c, etl::radix::value_type radix)
      {
        const string_type itr_begin = numeric_chars;
        const string_type itr_end   = numeric_chars + get_length(radix);

        string_type itr = etl::find(itr_begin, itr_end, c);

        return (itr != itr_end);
      }

      //*******************************************
      ETL_NODISCARD
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

          case etl::radix::hex:
          {
            for (int i = 0; i < 16; ++i)
            {
              if (c == numeric_chars[i])
              {
                return i;
              }
            }
          }

          default: 
          { 
            return 0;
            break; 
          }
        }
      }

    private:

      //*******************************************
      ETL_NODISCARD
      ETL_CONSTEXPR14
      int get_length(etl::radix::value_type radix)
      {
        switch (radix)
        {
          case etl::radix::binary:  { return binary_length;  break; }
          case etl::radix::octal:   { return octal_length;   break; }
          case etl::radix::decimal: { return decimal_length; break; }
          case etl::radix::hex:     { return hex_length;     break; }
          default:                  { return 0;              break; }
        }
      }
    };

    //***********************************************************************
    /// The character sets for character types.
    //***********************************************************************
    template <typename TChar>
    struct character_set;

    //*******************************************
    template <>
    struct character_set<char>
    {
      ETL_NODISCARD
      ETL_CONSTEXPR14
      char lookup(char c)
      {
        return c;
      }
    };

    //*******************************************
    template <>
    struct character_set<wchar_t>
    {
      typedef wchar_t value_type;
      typedef const value_type* string_type;
      static ETL_CONSTANT string_type valid_chars = L"+-.,eE0123456789abcdefABCDEF";

      ETL_NODISCARD
      ETL_CONSTEXPR14
      char lookup(wchar_t c)
      {
        for (int i = 0; i < valid_length; ++i)
        {
          if (c == valid_chars[i])
          {
            return tolower(valid_character_set::valid_chars[i]);
          }
        }

        return valid_character_set::unknown_char;
      }
    };

    //*******************************************
    template <>
    struct character_set<char16_t>
    {
      typedef char16_t value_type;
      typedef const value_type* string_type;
      static ETL_CONSTANT string_type valid_chars = u"+-.,eE0123456789abcdefABCDEF";

      ETL_NODISCARD
      ETL_CONSTEXPR14
      char lookup(wchar_t c)
      {
        for (int i = 0; i < valid_length; ++i)
        {
          if (c == valid_chars[i])
          {
            return tolower(valid_character_set::valid_chars[i]);
          }
        }

        return valid_character_set::unknown_char;
      }
    };

    //*******************************************
    template <>
    struct character_set<char32_t>
    {
      typedef char32_t value_type;
      typedef const value_type* string_type;
      static ETL_CONSTANT string_type valid_chars = U"+-.,eE0123456789abcdefABCDEF";

      ETL_NODISCARD
      ETL_CONSTEXPR14
      char lookup(wchar_t c)
      {
        for (int i = 0; i < valid_length; ++i)
        {
          if (c == valid_chars[i])
          {
            return tolower(valid_character_set::valid_chars[i]);
          }
        }

        return valid_character_set::unknown_char;
      }
    };

    //***************************************************************************
    /// 
    //***************************************************************************
    template <typename TChar>
    struct view_information
    {
      ETL_CONSTEXPR14
      view_information()
        : valid_characters()
      {
      }

      etl::basic_string_view<TChar> valid_characters;
    };

    //*********************************
    struct numeric_information
    {
      ETL_CONSTEXPR14
      numeric_information()  
        : digit(0)
        , is_negative(false)
        , radix(0)
        , radix_point_position(etl::npos)
        , exponential_position(etl::npos)
      {
      }
      
      char digit;
      bool is_negative;
      etl::radix::value_type radix;
      size_t radix_point_position;
      size_t exponential_position;
    };

    //***************************************************************************
    /// 
    //***************************************************************************
    template <typename TChar>
    ETL_NODISCARD
    ETL_CONSTEXPR14
    bool validate_information_from_view(etl::basic_string_view<TChar>& view, etl::radix::value_type radix, view_information<TChar>& view_info, numeric_information& numeric_info)
    {
      numeric_info.radix = radix;
      view_info.valid_characters = get_character_set<TChar>(numeric_info.radix);

      bool is_valid = !view_info.valid_characters.empty();    

      if (is_valid)
      {
        // Look for a prefix.
        const bool has_positive_prefix = (view[0] == character_set<TChar>::positive_char);
        numeric_info.is_negative       = (view[0] == character_set<TChar>::negative_char);

        if (has_positive_prefix || numeric_info.is_negative)
        {
          view.remove_prefix(1U);
          is_valid = (view.begin() != view.end());
        }
      }

      if (is_valid)
      {
        // Look for a radix point.
        numeric_info.radix_point_position = view.find_first_of(character_set<TChar>::radix_point);

        // Look for an exponential.
        numeric_info.exponential_position = view.find_first_of(character_set<TChar>::exponential);
      }

      ETL_ASSERT(is_valid, ETL_ERROR(etl::to_arithmetic_invalid_format));

      return is_valid;
    }

    //***************************************************************************
    /// 
    //***************************************************************************
    template <typename TValue>
    ETL_NODISCARD
    ETL_CONSTEXPR14
    typename etl::enable_if<etl::is_integral<TValue>::value && etl::is_unsigned<TValue>::value, TValue>::type
      accumulate_value(TValue value, const numeric_information& info)
    {
      value *= info.radix;
      value += info.digit;

      return value;
    }

    //***************************************************************************
    /// 
    //***************************************************************************
    template <typename TValue>
    ETL_NODISCARD
    ETL_CONSTEXPR14
    typename etl::enable_if<etl::is_integral<TValue>::value && etl::is_signed<TValue>::value, TValue>::type
      accumulate_value(TValue value, const numeric_information& info)
    {
      value *= info.radix;
      info.is_negative ? value -= info.digit : value += info.digit;

      return value;
    }

    //***************************************************************************
    /// 
    //***************************************************************************
    template <typename TValue>
    ETL_NODISCARD
    ETL_CONSTEXPR14
    typename etl::enable_if<etl::is_floating_point<TValue>::value, TValue>::type
      accumulate_integral_part(TValue value, const numeric_information& info)
    {
      value *= info.radix;
      info.is_negative ? value -= info.digit : value += info.digit;

      return value;
    }

    //***************************************************************************
    /// 
    //***************************************************************************
    template <typename TValue>
    ETL_NODISCARD
    ETL_CONSTEXPR14
    typename etl::enable_if<etl::is_floating_point<TValue>::value, TValue>::type
      accumulate_fractional_part(TValue value, const numeric_information& info)
    {
      value /= info.radix;
      TValue d = TValue(info.digit) / info.radix;
      info.is_negative ? value -= d : value += d;

      return value;
    }

    template <typename TValue>
    struct to_integral_implementation
    {
      ETL_NODISCARD
      ETL_CONSTEXPR14
      to_integral_implementation(const numeric_information& info_)
        : info(info_)
        , count(0)
        , value(0)
        , is_negative(false)
      {
      }

      ETL_NODISCARD
      ETL_CONSTEXPR14
      bool add(char c)
      {
        if (valid_character_set::is_valid(c))
        {
          value = accumulate_value(value, info)
          return true;
        }
        else
        {
          return false;
        }
      }

      const numeric_information& info;
      int    count;
      TValue value;
    };
  }

  //***************************************************************************
  /// Text to integral from view and radix value type.
  //***************************************************************************
  template <typename TValue, typename TChar>
  ETL_NODISCARD
  ETL_CONSTEXPR14
  typename etl::enable_if<etl::is_integral<TValue>::value, etl::optional<TValue> >::type
    to_arithmetic(etl::basic_string_view<TChar> view, const etl::radix::value_type radix)
  {
    using namespace etl::private_to_arithmetic;

    etl::optional<TValue>         result;
    view_information<TChar>       view_info;
    numeric_information           numeric_info;

    to_integral_implementation<TValue> implementation(numeric_info);

    bool parsing = validate_information_from_view<TChar>(view, radix, view_info, numeric_info);

    if (parsing)
    {
      if (etl::is_unsigned<TValue>::value && numeric_info.is_negative)
      {
        parsing = false;
        ETL_ASSERT_FAIL(ETL_ERROR(etl::to_arithmetic_signed_to_unsigned));
      }

      if (parsing)
      {
        // Parse the numeric part.
        TValue value = 0;

        etl::basic_string_view<TChar>::const_iterator itr = view.begin();

        while (parsing)
        {
          implementation.add(character_set<TChar>::lookup(*itr));

          ++itr;

          if (itr == view.end())
          {
            result = implementation.value;
            parsing = false;
          }
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
    to_arithmetic(etl::basic_string_view<TChar> view)
  {
    using namespace etl::private_to_arithmetic;

    etl::optional<TValue>         result;
    etl::basic_string_view<TChar> valid_characters;
    view_information<TChar>       view_info;
    numeric_information           numeric_info;

    bool parsing = validate_information_from_view<TChar>(view, etl::radix::decimal, view_info, numeric_info);

    if (parsing)
    {
      // Integral part.
      etl::basic_string_view<TChar>::const_iterator itr_integral_begin = view.begin();
      etl::basic_string_view<TChar>::const_iterator itr_integral_end   = (numeric_info.radix_point_position == etl::npos)
                                                                           ? view.end()
                                                                           : itr_integral_begin + numeric_info.radix_point_position;

      TValue intergal_value = 0;

      while (itr_integral_begin != itr_integral_end)
      {
        if (is_valid_decimal_character(*itr_integral_begin))
        {
          numeric_info.digit = get_digit_value(*itr_integral_begin);
          intergal_value = accumulate_integral_part(intergal_value, numeric_info);
          ++itr_integral_begin;
        }
        else
        {
          // Character was not a valid numeric, so fail.
          ETL_ASSERT_FAIL(ETL_ERROR(etl::to_arithmetic_invalid_format));
          parsing = false;
          itr_integral_begin = itr_integral_end;
        }
      }

      // Fractional part.
      etl::basic_string_view<TChar>::const_reverse_iterator itr_fractional_end = ETL_OR_STD::reverse_iterator(itr_integral_begin + 1);
      etl::basic_string_view<TChar>::const_reverse_iterator itr_fractional_begin = (numeric_info.radix_point_position == etl::npos)
                                                                                     ? itr_fractional_end
                                                                                     : (numeric_info.exponential_position == etl::npos)
                                                                                         ? view.rbegin()
                                                                                         : ETL_OR_STD::reverse_iterator(view.begin() + numeric_info.exponential_position);
      
      TValue fractional_value = 0;
      
      while (itr_fractional_begin != itr_fractional_end)
      {
        if (is_valid_decimal_character(*itr_fractional_begin))
        {
          numeric_info.digit = get_digit_value(*itr_fractional_begin);
          fractional_value = accumulate_fractional_part(fractional_value, numeric_info);
          ++itr_fractional_begin;
        }
        else
        {
          // Character was not a valid numeric, so fail.
          ETL_ASSERT_FAIL(ETL_ERROR(etl::to_arithmetic_invalid_format));
          parsing = false;
          itr_fractional_begin = itr_fractional_end;
        }
      }

      // Exponetial part.
      etl::basic_string_view<TChar>::const_iterator itr_exponential_end = view.end();
      etl::basic_string_view<TChar>::const_iterator itr_exponential_begin = (numeric_info.exponential_position == etl::npos)
                                                                              ? view.end()
                                                                              : view.begin() + numeric_info.exponential_position + 1;

      //optional<int> exponential_value = to_arithmetic<int, TChar>(etl::basic_string_view<TChar>(itr_exponential_begin, itr_exponential_end));

      // Combine the integral and fractional parts.
      TValue value = 0;

      if (parsing)
      {
        value = intergal_value + fractional_value;
        result = value;
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
