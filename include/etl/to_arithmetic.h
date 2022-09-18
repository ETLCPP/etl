
#include "platform.h"
#include "type_traits.h"
#include "string_view.h"
#include "basic_string.h"
#include "format_spec.h"
#include "radix.h"
#include "optional.h"
#include "string_utilities.h"
#include "iterator.h"
#include "largest.h"
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

  namespace private_to_arithmetic
  {
    template <typename TChar>
    struct character_set;

    template <>
    struct character_set<char>
    {
      typedef char value_type;
      typedef const value_type* string_type;
      static ETL_CONSTANT string_type numeric_chars = "0123456789abcdefABCDEF";
      static ETL_CONSTANT value_type  positive_char = '+';
      static ETL_CONSTANT value_type  negative_char = '-';
    };

    template <>
    struct character_set<wchar_t>
    {
      typedef wchar_t value_type;
      typedef const value_type* string_type;
      static ETL_CONSTANT string_type numeric_chars = L"0123456789abcdefABCDEF";
      static ETL_CONSTANT value_type  positive_char = L'+';
      static ETL_CONSTANT value_type  negative_char = L'-';
    };

    template <>
    struct character_set<char16_t>
    {
      typedef char16_t value_type;
      typedef const value_type* string_type;
      static ETL_CONSTANT string_type numeric_chars = u"0123456789abcdefABCDEF";
      static ETL_CONSTANT value_type  positive_char = u'+';
      static ETL_CONSTANT value_type  negative_char = u'-';
    };

    template <>
    struct character_set<char32_t>
    {
      typedef char32_t value_type;
      typedef const value_type* string_type;
      static ETL_CONSTANT string_type numeric_chars = U"0123456789abcdefABCDEF";
      static ETL_CONSTANT value_type  positive_char = U'+';
      static ETL_CONSTANT value_type  negative_char = U'-';
    };

    static ETL_CONSTANT char binary_length  = 2;
    static ETL_CONSTANT char octal_length   = 8;
    static ETL_CONSTANT char decimal_length = 10;
    static ETL_CONSTANT char hex_length     = 16 + 6;

    //*******************************
    template <typename TChar>
    bool has_valid_characters(etl::basic_string_view<TChar> view, etl::basic_string_view<TChar> valid_characters)
    {
      if (valid_characters.empty())
      {
        return false;
      }

      size_t position = view.find_first_not_of(valid_characters);

      return position == etl::basic_string_view<TChar>::npos;
    }

    //*******************************
    template <typename TChar>
    char get_digit_value(TChar c)
    {
      size_t length = etl::strlen(character_set<TChar>::numeric_chars);

      for (int i = 0; i < length; ++i)
      {
        if (c == character_set<TChar>::numeric_chars[i])
        {
          return (i < 16) ? i : i - 6;
        }
      }

      return 0;
    }

    //*******************************
    template <typename TChar>
    struct processed_string
    {
      etl::basic_string_view<TChar> view;
      bool is_negative;
      bool is_valid;
    };

    //***************************************************************************
    /// 
    //***************************************************************************
    template <typename TChar>
    processed_string<TChar> preprocess_string(etl::basic_string_view<TChar> view, const etl::radix::value_type radix)
    {
      using namespace etl::private_to_arithmetic;

      view = etl::trim_view_whitespace(view);

      const bool has_positive_prefix = (view[0] == character_set<TChar>::positive_char);
      const bool has_negative_prefix = (view[0] == character_set<TChar>::negative_char);

      // Remove positive or negative prefixes.
      if (has_positive_prefix || has_negative_prefix)
      {
        view.remove_prefix(1);
      }
      
      etl::basic_string_view<TChar> valid_characters;

      switch (radix)
      {
        case etl::radix::binary:  { valid_characters = etl::basic_string_view<TChar>(character_set<TChar>::numeric_chars, binary_length);  break; }
        case etl::radix::octal:   { valid_characters = etl::basic_string_view<TChar>(character_set<TChar>::numeric_chars, octal_length);   break; }
        case etl::radix::decimal: { valid_characters = etl::basic_string_view<TChar>(character_set<TChar>::numeric_chars, decimal_length); break; }
        case etl::radix::hex:     { valid_characters = etl::basic_string_view<TChar>(character_set<TChar>::numeric_chars, hex_length);     break; }
        default: { break; } // No conversion available.
      }

      const bool has_valid_prefix = !has_negative_prefix || (radix == etl::radix::decimal);
      const bool is_valid         = has_valid_prefix && has_valid_characters(view, valid_characters);

      return processed_string<TChar>{ view, has_negative_prefix, is_valid };
    }
  }

  //***************************************************************************
  /// 
  //***************************************************************************
  template <typename T, typename TChar>
  ETL_CONSTEXPR14 typename etl::enable_if<etl::is_integral<T>::value, etl::optional<T> >::type
    to_integer(etl::basic_string_view<TChar> view, const etl::radix::value_type radix)
  {
    using namespace etl::private_to_arithmetic;

    processed_string<TChar> data = preprocess_string(view, radix);

    etl::optional<T> result;

    // String input was not in a valid format.    
    if (!data.is_valid)
    {
      ETL_ASSERT_FAIL_AND_RETURN_VALUE(ETL_ERROR(etl::to_arithmetic_invalid_format), result);
    }

    // Can't convert signed to unsigned.    
    if (data.is_negative && etl::is_unsigned<T>::value)
    {
      ETL_ASSERT_FAIL_AND_RETURN_VALUE(ETL_ERROR(etl::to_arithmetic_signed_to_unsigned), result);
    }

    if (data.is_valid)
    {
      T value = 0;

      switch (radix)
      {
        case etl::radix::decimal:
        {
          for (etl::basic_string_view<TChar>::const_iterator itr = data.view.begin(); itr != data.view.end(); ++itr)
          {
            value *= radix;

            char digit = get_digit_value(*itr);

            data.is_negative ? value -= digit : value += digit;
          }

          break;
        }

        default:
        {
          int shift = (radix == etl::radix::binary) ? 1 : (radix == etl::radix::octal) ? 3 : 4;

          for (etl::basic_string_view<TChar>::const_iterator itr = data.view.begin(); itr != data.view.end(); ++itr)
          {
            value <<= shift;

            char digit = get_digit_value(*itr);

            value = value | digit;
          }

          break;
        }

      }

      result = value;
    }

    return result;
  }

  //***************************************************************************
  /// 
  //***************************************************************************
  template <typename T, typename TChar>
  ETL_CONSTEXPR14 typename etl::enable_if<etl::is_integral<T>::value, etl::optional<T> >::type
    to_integer(const etl::basic_string_view<TChar>& view)
  {
    return etl::to_integer<T, TChar>(view, etl::radix::decimal);
  }

  //***************************************************************************
  /// 
  //***************************************************************************
  template <typename T, typename TChar>
  ETL_CONSTEXPR14 typename etl::enable_if<etl::is_integral<T>::value, etl::optional<T> >::type
    to_integer(const etl::basic_string_view<TChar>& view, const etl::basic_format_spec<etl::ibasic_string<TChar> >& spec)
  {
    return etl::to_integer<T, TChar>(view, spec.get_base());
  }

  //***************************************************************************
  /// 
  //***************************************************************************
  template <typename T, typename TChar>
  ETL_CONSTEXPR14 typename etl::enable_if<etl::is_integral<T>::value, etl::optional<T> >::type
    to_integer(const TChar* cp, size_t length, const etl::radix::value_type radix)
  {
    return etl::to_integer<T, TChar>(etl::basic_string_view<TChar>(cp, length), radix);
  }

  //***************************************************************************
  /// 
  //***************************************************************************
  template <typename T, typename TChar>
  ETL_CONSTEXPR14 typename etl::enable_if<etl::is_integral<T>::value, etl::optional<T> >::type
    to_integer(const TChar* cp, size_t length)
  {
    return etl::to_integer<T, TChar>(etl::basic_string_view<TChar>(cp, length), etl::radix::decimal);;
  }

  //***************************************************************************
  /// 
  //***************************************************************************
  template <typename T, typename TChar>
  ETL_CONSTEXPR14 typename etl::enable_if<etl::is_integral<T>::value, etl::optional<T> >::type
    to_integer(const TChar* cp, size_t length, const typename etl::private_basic_format_spec::base_spec& spec)
  {
    return etl::to_integer<T, TChar>(etl::basic_string_view<TChar>(cp, length), spec.base);;
  }

  //***************************************************************************
  /// 
  //***************************************************************************
  template <typename T, typename TChar>
  ETL_CONSTEXPR14 typename etl::enable_if<etl::is_integral<T>::value, etl::optional<T> >::type
    to_integer(const etl::ibasic_string<TChar>& str, const etl::radix::value_type radix)
  {
    return etl::to_integer<T, TChar>(etl::basic_string_view<TChar>(str), radix);;
  }

  //***************************************************************************
  /// 
  //***************************************************************************
  template <typename T, typename TChar>
  ETL_CONSTEXPR14 typename etl::enable_if<etl::is_integral<T>::value, etl::optional<T> >::type
    to_integer(const etl::ibasic_string<TChar>& str)
  {
    return etl::to_integer<T, TChar>(etl::basic_string_view<TChar>(str), radix);;
  }

  //***************************************************************************
  /// 
  //***************************************************************************
  template <typename T, typename TChar>
  ETL_CONSTEXPR14 typename etl::enable_if<etl::is_integral<T>::value, etl::optional<T> >::type
    to_integer(const etl::ibasic_string<TChar>& str, const etl::basic_format_spec<etl::ibasic_string<TChar> >& spec)
  {
    return etl::to_integer<T, TChar>(etl::basic_string_view<TChar>(str), spec);;
  }

  template <typename T, typename TChar>
  ETL_CONSTEXPR14 typename etl::enable_if<etl::is_integral<T>::value, etl::optional<T> >::type
    to_arithmetic(etl::basic_string_view<TChar> view, const etl::radix::value_type radix)
  {
    return to_integer<T>(view, radix);
  }

  template <typename T, typename TChar>
  ETL_CONSTEXPR14 typename etl::enable_if<etl::is_floating_point<T>::value, etl::optional<T> >::type
    to_arithmetic(etl::basic_string_view<TChar> view, const etl::radix::value_type radix)
  {
    return to_floating_point<T>(view, radix);
  }
}
