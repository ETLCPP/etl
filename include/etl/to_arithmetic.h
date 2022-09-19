
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

  //***************************************************************************
  /// 
  //***************************************************************************
  class to_arithmetic_radix_not_supported : public to_arithmetic_exception
  {
  public:

    to_arithmetic_radix_not_supported(string_type file_name_, numeric_type line_number_)
      : to_arithmetic_exception(ETL_ERROR_TEXT("to arithmetic:radix not supported", ETL_TO_ARITHMETIC_FILE_ID"C"), file_name_, line_number_)
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

    //***************************************************************************
    /// 
    //***************************************************************************
    template <typename TChar>
    ETL_CONSTEXPR14 bool is_valid_numeric_character(const TChar c, 
                                                    const etl::basic_string_view<TChar>& valid_characters)
    {
      using namespace etl::private_to_arithmetic;

      etl::basic_string_view<TChar> itr = etl::find(valid_characters.begin(), valid_characters.end(), c);

      return (itr != valid_characters.end());
    }

    //***************************************************************************
/// 
//***************************************************************************
    template <typename T>
    ETL_CONSTEXPR14 T accumulate_value(T value, const char digit, const bool is_negative, int shift, const etl::radix::value_type radix)
    {
      switch (radix)
      {
        case etl::radix::decimal:
        {
          value *= radix;
          is_negative ? value -= digit : value += digit;
          break;
        }

        default: // Binary, octal or hex.
        {
          value <<= shift;
          value = value | digit;
          break;
        }
      }

      return value;
    }
  }

  //***************************************************************************
  /// 
  //***************************************************************************
  template <typename T, typename TChar>
  ETL_CONSTEXPR14 typename etl::enable_if<etl::is_integral<T>::value, etl::optional<T> >::type
    to_arithmetic(etl::basic_string_view<TChar> view, const etl::radix::value_type radix)
  {
    using namespace etl::private_to_arithmetic;

    etl::optional<T> result;

    bool finished_parsing = false;

    etl::basic_string_view<TChar> valid_characters;

    switch (radix)
    {
      case etl::radix::binary:  { valid_characters = etl::basic_string_view<TChar>(character_set<TChar>::numeric_chars, binary_length);  break; }
      case etl::radix::octal:   { valid_characters = etl::basic_string_view<TChar>(character_set<TChar>::numeric_chars, octal_length);   break; }
      case etl::radix::decimal: { valid_characters = etl::basic_string_view<TChar>(character_set<TChar>::numeric_chars, decimal_length); break; }
      case etl::radix::hex:     { valid_characters = etl::basic_string_view<TChar>(character_set<TChar>::numeric_chars, hex_length);     break; }
      default:                  { ETL_ASSERT_FAIL(ETL_ERROR(etl::to_arithmetic_radix_not_supported)); finished_parsing = false; break; }
    }

    if (!finished_parsing)
    {
      T value = 0;

      // Only used for binary, octal and hex.
      int shift = (radix == etl::radix::binary) ? 1 : (radix == etl::radix::octal) ? 3 : 4;

      etl::basic_string_view<TChar>::const_iterator itr = view.begin();

      // Search for a prefix.
      const bool has_positive_prefix = (*itr == character_set<TChar>::positive_char);
      const bool has_negative_prefix = (*itr == character_set<TChar>::negative_char);

      if (has_positive_prefix || has_negative_prefix)
      {
        ++itr;
      }

      // Negative prefix is only allowed with decimals.
      if (has_negative_prefix && (radix != etl::radix::decimal))
      {
        ETL_ASSERT_FAIL(ETL_ERROR(etl::to_arithmetic_invalid_format));
        finished_parsing = true;
      }
      
      // Parse the numeric part.
      while (!finished_parsing)
      {
        if (is_valid_numeric_character(*itr, valid_characters))
        {
          value = accumulate_value(value, get_digit_value(*itr), has_negative_prefix, shift, radix);

          ++itr;

          if (itr == view.end())
          {
            result = value;
            finished_parsing = true;
          }
        }
        else
        {
          // Character was not a valid numeric, so fail.
          ETL_ASSERT_FAIL(ETL_ERROR(etl::to_arithmetic_invalid_format));
          finished_parsing = true;
        }
      }
    }

    return result;
  }

  //***************************************************************************
  /// 
  //***************************************************************************
  template <typename T, typename TChar>
  ETL_CONSTEXPR14 typename etl::enable_if<etl::is_integral<T>::value, etl::optional<T> >::type
    to_arithmetic(const etl::basic_string_view<TChar>& view)
  {
    return etl::to_arithmetic<T, TChar>(view, etl::radix::decimal);
  }

  //***************************************************************************
  /// 
  //***************************************************************************
  template <typename T, typename TChar>
  ETL_CONSTEXPR14 typename etl::enable_if<etl::is_integral<T>::value, etl::optional<T> >::type
    to_arithmetic(const etl::basic_string_view<TChar>& view, const etl::basic_format_spec<etl::ibasic_string<TChar> >& spec)
  {
    return etl::to_arithmetic<T, TChar>(view, spec.get_base());
  }

  //***************************************************************************
  /// 
  //***************************************************************************
  template <typename T, typename TChar>
  ETL_CONSTEXPR14 typename etl::enable_if<etl::is_integral<T>::value, etl::optional<T> >::type
    to_arithmetic(const TChar* cp, size_t length, const etl::radix::value_type radix)
  {
    return etl::to_arithmetic<T, TChar>(etl::basic_string_view<TChar>(cp, length), radix);
  }

  //***************************************************************************
  /// 
  //***************************************************************************
  template <typename T, typename TChar>
  ETL_CONSTEXPR14 typename etl::enable_if<etl::is_integral<T>::value, etl::optional<T> >::type
    to_arithmetic(const TChar* cp, size_t length)
  {
    return etl::to_arithmetic<T, TChar>(etl::basic_string_view<TChar>(cp, length), etl::radix::decimal);;
  }

  //***************************************************************************
  /// 
  //***************************************************************************
  template <typename T, typename TChar>
  ETL_CONSTEXPR14 typename etl::enable_if<etl::is_integral<T>::value, etl::optional<T> >::type
    to_arithmetic(const TChar* cp, size_t length, const typename etl::private_basic_format_spec::base_spec& spec)
  {
    return etl::to_arithmetic<T, TChar>(etl::basic_string_view<TChar>(cp, length), spec.base);;
  }

  //***************************************************************************
  /// 
  //***************************************************************************
  template <typename T, typename TChar>
  ETL_CONSTEXPR14 typename etl::enable_if<etl::is_integral<T>::value, etl::optional<T> >::type
    to_arithmetic(const etl::ibasic_string<TChar>& str, const etl::radix::value_type radix)
  {
    return etl::to_arithmetic<T, TChar>(etl::basic_string_view<TChar>(str), radix);;
  }

  //***************************************************************************
  /// 
  //***************************************************************************
  template <typename T, typename TChar>
  ETL_CONSTEXPR14 typename etl::enable_if<etl::is_integral<T>::value, etl::optional<T> >::type
    to_arithmetic(const etl::ibasic_string<TChar>& str)
  {
    return etl::to_arithmetic<T, TChar>(etl::basic_string_view<TChar>(str), radix);;
  }

  //***************************************************************************
  /// 
  //***************************************************************************
  template <typename T, typename TChar>
  ETL_CONSTEXPR14 typename etl::enable_if<etl::is_integral<T>::value, etl::optional<T> >::type
    to_arithmetic(const etl::ibasic_string<TChar>& str, const etl::basic_format_spec<etl::ibasic_string<TChar> >& spec)
  {
    return etl::to_arithmetic<T, TChar>(etl::basic_string_view<TChar>(str), spec);;
  }

  //template <typename T, typename TChar>
  //ETL_CONSTEXPR14 typename etl::enable_if<etl::is_floating_point<T>::value, etl::optional<T> >::type
  //  to_arithmetic(etl::basic_string_view<TChar> view, const etl::radix::value_type radix)
  //{
  //  return to_floating_point<T>(view, radix);
  //}
}
