
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

  //***************************************************************************
  /// 
  //***************************************************************************
  class to_arithmetic_overflow : public to_arithmetic_exception
  {
  public:

    to_arithmetic_overflow(string_type file_name_, numeric_type line_number_)
      : to_arithmetic_exception(ETL_ERROR_TEXT("to arithmetic:overflow", ETL_TO_ARITHMETIC_FILE_ID"D"), file_name_, line_number_)
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
      static bool is_valid(char c, etl::radix::value_type radix)
      {
        const string_type itr_begin = numeric_chars;
        const string_type itr_end   = numeric_chars + get_length(radix);

        string_type itr = etl::find(itr_begin, itr_end, c);

        return (itr != itr_end);
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
            for (int i = 0; i < 16; ++i)
            {
              if (c == numeric_chars[i])
              {
                return i;
              }
            }

            return 0;
            break;
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
      static int get_length(etl::radix::value_type radix)
      {
        switch (radix)
        {
          case etl::radix::binary:  { return binary_length;  }
          case etl::radix::octal:   { return octal_length;   }
          case etl::radix::decimal: { return decimal_length; }
          case etl::radix::hex:     { return hex_length;     }
          default:                  { ETL_ASSERT_FAIL_AND_RETURN_VALUE(ETL_ERROR(etl::to_arithmetic_invalid_radix), 0) }
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
      static char lookup(char c)
      {
        return to_lower(c);
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
      static char lookup(wchar_t c)
      {
        for (int i = 0; i < valid_length; ++i)
        {
          if (c == valid_chars[i])
          {
            return to_lower(valid_character_set::valid_chars[i]);
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
      static char lookup(char16_t c)
      {
        for (int i = 0; i < valid_length; ++i)
        {
          if (c == valid_chars[i])
          {
            return to_lower(valid_character_set::valid_chars[i]);
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
      static char lookup(wchar_t c)
      {
        for (int i = 0; i < valid_length; ++i)
        {
          if (c == valid_chars[i])
          {
            return to_lower(valid_character_set::valid_chars[i]);
          }
        }

        return valid_character_set::unknown_char;
      }
    };

    //***************************************************************************
    /// 
    //***************************************************************************
    template <typename TValue>
    ETL_NODISCARD
    ETL_CONSTEXPR14
    typename etl::enable_if<etl::is_integral<TValue>::value && etl::is_unsigned<TValue>::value, TValue>::type
      accumulate_value(TValue value, char digit, etl::radix::value_type radix, bool is_negative)
    {
      value *= radix;
      value += digit;

      return value;
    }

    //***************************************************************************
    /// 
    //***************************************************************************
    template <typename TValue>
    ETL_NODISCARD
    ETL_CONSTEXPR14
    typename etl::enable_if<etl::is_integral<TValue>::value && etl::is_signed<TValue>::value, TValue>::type
      accumulate_value(TValue value, char digit, etl::radix::value_type radix, bool is_negative)
    {
      value *= radix;
      is_negative ? value -= digit : value += digit;

      return value;
    }

    //*******************************************
    template <typename TChar>
    bool check_has_negative_prefix(etl::basic_string_view<TChar>& view)
    {
      // Check for prefix.
      const char c = character_set<TChar>::lookup(view[0]);
      const bool has_positive_prefix = (c == valid_character_set::positive_char);
      const bool has_negative_prefix = (c == valid_character_set::negative_char);

      // Remove the prefix if present.
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
    struct to_integral_implementation
    {
      //*********************************
      ETL_NODISCARD
      ETL_CONSTEXPR14
      to_integral_implementation(etl::radix::value_type radix_, bool is_negative_)
        : value(0)
        , is_negative(is_negative_)
        , valid_value(false)
        , radix(radix_)
      {
      }

      //*********************************
      ETL_NODISCARD
      ETL_CONSTEXPR14
      bool add(char c)
      {
        bool is_valid = valid_character_set::is_valid(c, radix);
              
        if (is_valid)
        {
          value = accumulate_value(value, valid_character_set::digit_value(c, radix), radix, is_negative);
          valid_value = true;
        }

        return is_valid;
      }

      //*********************************
      bool has_value() const
      {
        return (valid_value == true);
      }

      //*********************************
      TValue get_value() const
      {
        return value;
      }

    private:

      TValue value;
      bool   is_negative;
      bool   valid_value;
      etl::radix::value_type radix;
    };
    
    //***************************************************************************
    /// Text to integral from view and radix value type.
    // For unsigned 32 int.
    //***************************************************************************
    template <typename TChar>
    ETL_NODISCARD
    ETL_CONSTEXPR14
    etl::optional<uint32_t> to_arithmetic_uint32_t(etl::basic_string_view<TChar> view, const etl::radix::value_type radix)
    {
      using namespace etl::private_to_arithmetic;

      etl::optional<uint32_t> result;
      bool is_negative = false;
      
      if (!view.empty())
      {
        ETL_ASSERT(!check_has_negative_prefix(view), ETL_ERROR(etl::to_arithmetic_signed_to_unsigned));

        to_integral_implementation<uint32_t> implementation(radix, is_negative);

        bool parsing = true;

        etl::basic_string_view<TChar>::const_iterator itr = view.begin();

        while (parsing)
        {
          parsing = implementation.add(character_set<TChar>::lookup(*itr));

          if (parsing)
          {
            ++itr;

            if (itr == view.end())
            {
              ETL_ASSERT(implementation.has_value(), ETL_ERROR(etl::to_arithmetic_invalid_format));

              result = implementation.get_value();
              parsing = false;
            }
          }
          else
          {
            ETL_ASSERT_FAIL(ETL_ERROR(etl::to_arithmetic_invalid_format));
          }
        }
      }
      else
      {
        ETL_ASSERT_FAIL(ETL_ERROR(etl::to_arithmetic_invalid_format));
      }

      return result;
    }

    //***************************************************************************
    /// Text to integral from view and radix value type.
    // For signed 32 int.
    //***************************************************************************
    template <typename TChar>
    ETL_NODISCARD
    ETL_CONSTEXPR14
    etl::optional<int32_t> to_arithmetic_int32_t(etl::basic_string_view<TChar> view, const etl::radix::value_type radix)
    {
      using namespace etl::private_to_arithmetic;

      etl::optional<int32_t> result;    
      bool is_negative = false;
      
      if (!view.empty())
      {
        is_negative = check_has_negative_prefix(view);
      }

      if (!view.empty())
      {
        to_integral_implementation<int32_t> implementation(radix, is_negative);

        bool parsing = true;

        etl::basic_string_view<TChar>::const_iterator itr = view.begin();

        while (parsing)
        {
          parsing = implementation.add(character_set<TChar>::lookup(*itr));

          if (parsing)
          {
            ++itr;

            if (itr == view.end())
            {
              ETL_ASSERT(implementation.has_value(), ETL_ERROR(etl::to_arithmetic_invalid_format));

              result = implementation.get_value();
              parsing = false;
            }
          }
          else
          {
            ETL_ASSERT_FAIL(ETL_ERROR(etl::to_arithmetic_invalid_format));
          }
        }
      }
      else
      {
        ETL_ASSERT_FAIL(ETL_ERROR(etl::to_arithmetic_invalid_format));
      }

      return result;
    }

#if ETL_USING_64BIT_TYPES
    //***************************************************************************
    /// Text to integral from view and radix value type.
    // For unsigned 64 int.
    //***************************************************************************
    template <typename TChar>
    ETL_NODISCARD
    ETL_CONSTEXPR14
    etl::optional<uint64_t> to_arithmetic_uint64_t(etl::basic_string_view<TChar> view, const etl::radix::value_type radix)
    {
      using namespace etl::private_to_arithmetic;

      etl::optional<uint64_t> result;
      bool is_negative = false;
      
      if (!view.empty())
      {
        ETL_ASSERT(!check_has_negative_prefix(view), ETL_ERROR(etl::to_arithmetic_signed_to_unsigned));

        to_integral_implementation<uint64_t> implementation(radix, is_negative);

        bool parsing = true;

        etl::basic_string_view<TChar>::const_iterator itr = view.begin();

        while (parsing)
        {
          parsing = implementation.add(character_set<TChar>::lookup(*itr));

          if (parsing)
          {
            ++itr;

            if (itr == view.end())
            {
              ETL_ASSERT(implementation.has_value(), ETL_ERROR(etl::to_arithmetic_invalid_format));

              result = implementation.get_value();
              parsing = false;
            }
          }
          else
          {
            ETL_ASSERT_FAIL(ETL_ERROR(etl::to_arithmetic_invalid_format));
          }
        }
      }
      else
      {
        ETL_ASSERT_FAIL(ETL_ERROR(etl::to_arithmetic_invalid_format));
      }

      return result;
    }

    //***************************************************************************
    /// Text to integral from view and radix value type.
    // For signed 64 int.
    //***************************************************************************
    template <typename TChar>
    ETL_NODISCARD
    ETL_CONSTEXPR14
    etl::optional<int64_t> to_arithmetic_int64_t(etl::basic_string_view<TChar> view, const etl::radix::value_type radix)
    {
      using namespace etl::private_to_arithmetic;

      etl::optional<int64_t> result;
      bool is_negative = false;
      
      if (!view.empty())
      {
        is_negative = check_has_negative_prefix(view);
      }

      if (!view.empty())
      {
        to_integral_implementation<int64_t> implementation(radix, is_negative);

        bool parsing = true;

        etl::basic_string_view<TChar>::const_iterator itr = view.begin();

        while (parsing)
        {
          parsing = implementation.add(character_set<TChar>::lookup(*itr));

          if (parsing)
          {
            ++itr;

            if (itr == view.end())
            {
              ETL_ASSERT(implementation.has_value(), ETL_ERROR(etl::to_arithmetic_invalid_format));

              result = implementation.get_value();
              parsing = false;
            }
          }
          else
          {
            ETL_ASSERT_FAIL(ETL_ERROR(etl::to_arithmetic_invalid_format));
          }
        }
      }
      else
      {
        ETL_ASSERT_FAIL(ETL_ERROR(etl::to_arithmetic_invalid_format));
      }

      return result;
    }
#endif
  }

  //***************************************************************************
  /// Text to integral from view and radix value type.
  /// Unsigned <= 32 bits.
  //***************************************************************************
  template <typename TValue, typename TChar>
  ETL_NODISCARD
  ETL_CONSTEXPR14
  typename etl::enable_if<etl::is_integral<TValue>::value && (etl::integral_limits<TValue>::bits <= 32U) && etl::is_unsigned<TValue>::value, etl::optional<TValue> >::type
    to_arithmetic(etl::basic_string_view<TChar> view, const etl::radix::value_type radix)
  {
    using namespace etl::private_to_arithmetic;

    etl::optional<TValue> result;

    etl::optional<uint32_t> intermediate_result = to_arithmetic_uint32_t<TChar>(view, radix);
    
    if (intermediate_result.has_value())
    {
      result = static_cast<TValue>(intermediate_result.value());
    }

    return result;
  }

  //***************************************************************************
  /// Text to integral from view and radix value type.
  /// Signed <= 32 bits.
  //***************************************************************************
  template <typename TValue, typename TChar>
  ETL_NODISCARD
  ETL_CONSTEXPR14
  typename etl::enable_if<etl::is_integral<TValue>::value && (etl::integral_limits<TValue>::bits <= 32U) && etl::is_signed<TValue>::value, etl::optional<TValue> >::type
    to_arithmetic(etl::basic_string_view<TChar> view, const etl::radix::value_type radix)
  {
    using namespace etl::private_to_arithmetic;

    etl::optional<TValue> result;

    etl::optional<int32_t> intermediate_result = to_arithmetic_int32_t<TChar>(view, radix);

    if (intermediate_result.has_value())
    {
      result = static_cast<TValue>(intermediate_result.value());
    }

    return result;
  }

#if ETL_USING_64BIT_TYPES
  //***************************************************************************
  /// Text to integral from view and radix value type.
  /// Unsigned == 64 bits.
  //***************************************************************************
  template <typename TValue, typename TChar>
  ETL_NODISCARD
  ETL_CONSTEXPR14
  typename etl::enable_if<etl::is_integral<TValue>::value && (etl::integral_limits<TValue>::bits == 64U) && etl::is_unsigned<TValue>::value, etl::optional<TValue> >::type
    to_arithmetic(etl::basic_string_view<TChar> view, const etl::radix::value_type radix)
  {
    using namespace etl::private_to_arithmetic;

    etl::optional<TValue> result;

    etl::optional<uint64_t> intermediate_result = to_arithmetic_uint64_t<TChar>(view, radix);

    if (intermediate_result.has_value())
    {
      result = static_cast<TValue>(intermediate_result.value());
    }

    return result;
  }

  //***************************************************************************
  /// Text to integral from view and radix value type.
  /// Signed == 64 bits.
  //***************************************************************************
  template <typename TValue, typename TChar>
  ETL_NODISCARD
  ETL_CONSTEXPR14
  typename etl::enable_if<etl::is_integral<TValue>::value && (etl::integral_limits<TValue>::bits == 64U) && etl::is_signed<TValue>::value, etl::optional<TValue> >::type
    to_arithmetic(etl::basic_string_view<TChar> view, const etl::radix::value_type radix)
  {
    using namespace etl::private_to_arithmetic;

    etl::optional<TValue> result;

    etl::optional<int64_t> intermediate_result = to_arithmetic_int64_t<TChar>(view, radix);
    
    if (intermediate_result.has_value())
    {
      result = static_cast<TValue>(intermediate_result.value());
    }

    return result;
  }
#endif

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
