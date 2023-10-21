//*************************************************************************
///Decode from Base64 from and to pointer/length
//*************************************************************************///\file

/******************************************************************************
The MIT License(MIT)
Embedded Template Library.
https://github.com/ETLCPP/etl
https://www.etlcpp.com
Copyright(c) 2023 John Wellbelove
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

#ifndef ETL_BASE64_INCLUDED
#define ETL_BASE64_INCLUDED

#include "etl/platform.h"
#include "etl/span.h"
#include "etl/static_assert.h"
#include "etl/error_handler.h"
#include "etl/exception.h"
#include "etl/type_traits.h"
#include "etl/binary.h"
#include "etl/algorithm.h"
#include "etl/integral_limits.h"
#include "etl/iterator.h"
#include "etl/string.h"

#include <stdint.h>

#define ETL_IS_ITERATOR_TYPE_8_BIT_INTEGRAL(Type) (etl::is_integral<typename etl::iterator_traits<typename etl::remove_cv<Type>::type>::value_type>::value && \
                                                   (etl::integral_limits<typename etl::iterator_traits<typename etl::remove_cv<Type>::type>::value_type>::bits == 8U))

namespace etl
{
  //***************************************************************************
  /// Exception base for base64
  //***************************************************************************
  class base64_exception : public etl::exception
  {
  public:

    base64_exception(string_type reason_, string_type file_name_, numeric_type line_number_)
      : exception(reason_, file_name_, line_number_)
    {
    }
  };

  //***************************************************************************
  /// Memory misalignment exception.
  //***************************************************************************
  class base64_overflow : public base64_exception
  {
  public:

    base64_overflow(string_type file_name_, numeric_type line_number_)
      : base64_exception(ETL_ERROR_TEXT("base64:overflow", ETL_BASE64_FILE_ID"A"), file_name_, line_number_)
    {
    }
  };

  //*************************************************************************
  /// Codec for Base64 (RFC 4648)
  //*************************************************************************
  class base64
  {
  public:

    //*************************************************************************
    /// Default constructor
    //*************************************************************************
    ETL_CONSTEXPR14
    base64()
      : use_padding(true)
    {
    }

    //*************************************************************************
    /// Encode to Base64
    /// Four parameter
    //*************************************************************************
    template <typename TInputIterator, typename TOutputIterator>
    ETL_CONSTEXPR14
    size_t encode(TInputIterator input_begin, TInputIterator input_end, TOutputIterator output_begin, TOutputIterator output_end)
    {
      ETL_STATIC_ASSERT(ETL_IS_ITERATOR_TYPE_8_BIT_INTEGRAL(TInputIterator),  "Input type must be an 8 bit integral");
      ETL_STATIC_ASSERT(ETL_IS_ITERATOR_TYPE_8_BIT_INTEGRAL(TOutputIterator), "Output type must be an 8 bit integral");

      const size_t input_length  = static_cast<size_t>(etl::distance(input_begin, input_end));
      const size_t output_length = static_cast<size_t>(etl::distance(output_begin, output_end));

      // Figure out if the output buffer is large enough.
      size_t required_output_length = etl::base64::encoded_size(input_length);
      ETL_ASSERT_OR_RETURN_VALUE(output_length >= required_output_length, ETL_ERROR(base64_overflow), 0U);

      return process_encode(input_begin, input_length, output_begin, required_output_length);
    }

    //*************************************************************************
    /// Encode to Base64
    /// For TOutputIterator not derived from etl::iterator<ETL_OR_STD::output_iterator_tag, void, void, void, void>
    /// Three parameter
    /// Assumes the output container is large enough.
    //*************************************************************************
    template <typename TInputIterator, typename TOutputIterator>
    ETL_CONSTEXPR14
    typename etl::enable_if<!etl::is_void<typename etl::iterator_traits<TOutputIterator>::value_type>::value, size_t>::type
      encode(TInputIterator input_begin, TInputIterator input_end, TOutputIterator output_begin)
    {
      ETL_STATIC_ASSERT(ETL_IS_ITERATOR_TYPE_8_BIT_INTEGRAL(TInputIterator),  "Input type must be an 8 bit integral");
      ETL_STATIC_ASSERT(ETL_IS_ITERATOR_TYPE_8_BIT_INTEGRAL(TOutputIterator), "Output type must be an 8 bit integral");

      const size_t input_length  = static_cast<size_t>(etl::distance(input_begin, input_end));
      const size_t output_length = encoded_size(input_length);

      return process_encode(input_begin, input_length, output_begin, output_length);
    }  

    //*************************************************************************
    /// Encode to Base64
    /// For TOutputIterator derived from etl::iterator<ETL_OR_STD::output_iterator_tag, void, void, void, void>
    /// Three parameter
    /// Assumes the output container is large enough.
    //*************************************************************************
    template <typename TInputIterator, typename TOutputIterator>
    ETL_CONSTEXPR14
    typename etl::enable_if<etl::is_void<typename etl::iterator_traits<TOutputIterator>::value_type>::value, size_t>::type
      encode(TInputIterator input_begin, TInputIterator input_end, TOutputIterator output_begin)
    {
      ETL_STATIC_ASSERT(ETL_IS_ITERATOR_TYPE_8_BIT_INTEGRAL(TInputIterator), "Input type must be an 8 bit integral");

      const size_t input_length  = static_cast<size_t>(etl::distance(input_begin, input_end));
      const size_t output_length = encoded_size(input_length);

      return process_encode(input_begin, input_length, output_begin, output_length);
    }

    //*************************************************************************
    /// Encode to Base64
    /// Four parameter
    //*************************************************************************
    template <typename TInputIterator, typename TOutputIterator>
    ETL_CONSTEXPR14
    size_t encode(TInputIterator input_begin, size_t input_length, TOutputIterator output_begin, size_t output_length)
    {
      ETL_STATIC_ASSERT(ETL_IS_ITERATOR_TYPE_8_BIT_INTEGRAL(TInputIterator),  "Input type must be an 8 bit integral");
      ETL_STATIC_ASSERT(ETL_IS_ITERATOR_TYPE_8_BIT_INTEGRAL(TOutputIterator), "Output type must be an 8 bit integral");

      // Figure out if the output buffer is large enough.
      size_t required_output_length = etl::base64::encoded_size(input_length);
      ETL_ASSERT_OR_RETURN_VALUE(output_length >= required_output_length, ETL_ERROR(base64_overflow), 0U);

      return process_encode(input_begin, input_length, output_begin, required_output_length);
    }

    //*************************************************************************
    /// Encode to Base64
    /// For TOutputIterator not derived from etl::iterator<ETL_OR_STD::output_iterator_tag, void, void, void, void>
    /// Three parameter
    /// Assumes the output container is large enough.
    //*************************************************************************
    template <typename TInputIterator, typename TOutputIterator>
    ETL_CONSTEXPR14
    typename etl::enable_if<!etl::is_void<typename etl::iterator_traits<TOutputIterator>::value_type>::value, size_t>::type
      encode(TInputIterator input_begin, size_t input_length, TOutputIterator output_begin)
    {
      ETL_STATIC_ASSERT(ETL_IS_ITERATOR_TYPE_8_BIT_INTEGRAL(TInputIterator),  "Input type must be an 8 bit integral");
      ETL_STATIC_ASSERT(ETL_IS_ITERATOR_TYPE_8_BIT_INTEGRAL(TOutputIterator), "Output type must be an 8 bit integral");

      const size_t output_length = encoded_size(input_length);

      return process_encode(input_begin, input_length, output_begin, output_length);
    }

    //*************************************************************************
    /// Encode to Base64
    /// For TOutputIterator derived from etl::iterator<ETL_OR_STD::output_iterator_tag, void, void, void, void>
    /// Three parameter
    /// Assumes the output container is large enough.
    //*************************************************************************
    template <typename TInputIterator, typename TOutputIterator>
    ETL_CONSTEXPR14
    typename etl::enable_if<etl::is_void<typename etl::iterator_traits<TOutputIterator>::value_type>::value, size_t>::type
      encode(TInputIterator  input_begin, size_t input_length, TOutputIterator output_begin)
    {
      ETL_STATIC_ASSERT(ETL_IS_ITERATOR_TYPE_8_BIT_INTEGRAL(TInputIterator), "Input type must be an 8 bit integral");

      const size_t output_length = encoded_size(input_length);

      return process_encode(input_begin, input_length, output_begin, output_length);
    }

    //*************************************************************************
    /// Calculates the minimum buffer size required to encode to Base64
    //*************************************************************************
    ETL_NODISCARD
    ETL_CONSTEXPR14
    static size_t encoded_size(size_t input_length)
    {
      size_t required_output_length = (input_length * 4U) / 3U;

      while ((required_output_length % 4U) != 0)
      {
        ++required_output_length;
      }

      return required_output_length;
    }

    //*************************************************************************
    /// Decode from Base64
    /// Four parameter
    //*************************************************************************
    template <typename TInputIterator, typename TOutputIterator>
    ETL_CONSTEXPR14
    size_t decode(TInputIterator input_begin, TInputIterator input_end, TOutputIterator output_begin, TOutputIterator output_end)
    {
      ETL_STATIC_ASSERT(ETL_IS_ITERATOR_TYPE_8_BIT_INTEGRAL(TInputIterator),  "Input type must be an 8 bit integral");
      ETL_STATIC_ASSERT(ETL_IS_ITERATOR_TYPE_8_BIT_INTEGRAL(TOutputIterator), "Output type must be an 8 bit integral");

      const size_t input_length  = valid_input_length(input_begin, input_end);
      const size_t output_length = static_cast<size_t>(etl::distance(output_begin, output_end));

      // Figure out if the output buffer is large enough.
      size_t required_output_length = etl::base64::decoded_size_from_valid_input_length(input_length);
      ETL_ASSERT_OR_RETURN_VALUE(output_length >= required_output_length, ETL_ERROR(base64_overflow), 0U);

      return process_decode(input_begin, input_length, output_begin, required_output_length);
    }

    //*************************************************************************
    /// Decode from Base64
    /// For TOutputIterator not derived from etl::iterator<ETL_OR_STD::output_iterator_tag, void, void, void, void>
    /// Three parameter
    /// Assumes the output container is large enough.
    //*************************************************************************
    template <typename TInputIterator, typename TOutputIterator>
    ETL_CONSTEXPR14
    typename etl::enable_if<!etl::is_void<typename etl::iterator_traits<TOutputIterator>::value_type>::value, size_t>::type
      decode(TInputIterator input_begin, TInputIterator input_end, TOutputIterator output_begin)
    {
      ETL_STATIC_ASSERT(ETL_IS_ITERATOR_TYPE_8_BIT_INTEGRAL(TInputIterator),  "Input type must be an 8 bit integral");
      ETL_STATIC_ASSERT(ETL_IS_ITERATOR_TYPE_8_BIT_INTEGRAL(TOutputIterator), "Output type must be an 8 bit integral");

      // Find the length of decodable characters.
      const size_t input_length = valid_input_length(input_begin, input_end);

      // Find the length of the decoded output.
      const size_t required_output_length = decoded_size(input_begin, input_length);

      return process_decode(input_begin, input_length, output_begin, required_output_length);
    }

    //*************************************************************************
    /// Decode from Base64
    /// For TOutputIterator derived from etl::iterator<ETL_OR_STD::output_iterator_tag, void, void, void, void>
    /// Three parameter
    /// Assumes the output container is large enough.
    //*************************************************************************
    template <typename TInputIterator, typename TOutputIterator>
    ETL_CONSTEXPR14
    typename etl::enable_if<etl::is_void<typename etl::iterator_traits<TOutputIterator>::value_type>::value, size_t>::type
      decode(TInputIterator input_begin, TInputIterator input_end, TOutputIterator output_begin)
    {
      ETL_STATIC_ASSERT(ETL_IS_ITERATOR_TYPE_8_BIT_INTEGRAL(TInputIterator), "Input type must be an 8 bit integral");

      // Find the length of decodable characters.
      const size_t input_length = valid_input_length(input_begin, input_end);

      // Find the length of the decoded output.
      const size_t required_output_length = decoded_size(input_begin, input_length);

      return process_decode(input_begin, input_length, output_begin, required_output_length);
    }

    //*************************************************************************
    /// Decode from Base64
    /// Four parameter
    //*************************************************************************
    template <typename TInputIterator, typename TOutputIterator>
    ETL_CONSTEXPR14
    size_t decode(TInputIterator input_begin, size_t input_length, TOutputIterator output_begin, size_t output_length)
    {
      ETL_STATIC_ASSERT(ETL_IS_ITERATOR_TYPE_8_BIT_INTEGRAL(TInputIterator),  "Input type must be an 8 bit integral");
      ETL_STATIC_ASSERT(ETL_IS_ITERATOR_TYPE_8_BIT_INTEGRAL(TOutputIterator), "Output type must be an 8 bit integral");

      // Find the length of decodable characters.
      input_length = valid_input_length(input_begin, input_length);

      // Figure out if the output buffer is large enough.
      size_t required_output_length = etl::base64::decoded_size_from_valid_input_length(input_length);
      ETL_ASSERT_OR_RETURN_VALUE(output_length >= required_output_length, ETL_ERROR(base64_overflow), 0U);

      return process_decode(input_begin, input_length, output_begin, required_output_length);
    }

    //*************************************************************************
    /// Decode from Base64
    /// For TOutputIterator not derived from etl::iterator<ETL_OR_STD::output_iterator_tag, void, void, void, void>
    /// Three parameter
    /// Assumes the output container is large enough.
    //*************************************************************************
    template <typename TInputIterator, typename TOutputIterator>
    ETL_CONSTEXPR14
    typename etl::enable_if<!etl::is_void<typename etl::iterator_traits<TOutputIterator>::value_type>::value, size_t>::type
      decode(TInputIterator input_begin, size_t input_length, TOutputIterator output_begin)
    {
      ETL_STATIC_ASSERT(ETL_IS_ITERATOR_TYPE_8_BIT_INTEGRAL(TInputIterator),  "Input type must be an 8 bit integral");
      ETL_STATIC_ASSERT(ETL_IS_ITERATOR_TYPE_8_BIT_INTEGRAL(TOutputIterator), "Output type must be an 8 bit integral");

      // Find the length of decodable characters.
      input_length = valid_input_length(input_begin, input_length);

      // Find the length of the decoded output.
      const size_t required_output_length = decoded_size(input_begin, input_length);

      return process_decode(input_begin, input_length, output_begin, required_output_length);
    }

    //*************************************************************************
    /// Decode from Base64
    /// For TOutputIterator derived from etl::iterator<ETL_OR_STD::output_iterator_tag, void, void, void, void>
    /// Three parameter
    /// Assumes the output container is large enough.
    //*************************************************************************
    template <typename TInputIterator, typename TOutputIterator>
    ETL_CONSTEXPR14
    typename etl::enable_if<etl::is_void<typename etl::iterator_traits<TOutputIterator>::value_type>::value, size_t>::type
      decode(TInputIterator  input_begin, size_t input_length, TOutputIterator output_begin)
    {
      ETL_STATIC_ASSERT(ETL_IS_ITERATOR_TYPE_8_BIT_INTEGRAL(TInputIterator), "Input type must be an 8 bit integral");

      // Find the length of decodable characters.
      input_length = valid_input_length(input_begin, input_length);

      // Find the length of the decoded output.
      const size_t output_length = decoded_size(input_begin, input_length);

      return process_decode(input_begin, input_length, output_begin, output_length);
    }

    //*************************************************************************
    /// Discovers the number of valid decodable characters
    //*************************************************************************
    template <typename TInputIterator>
    ETL_NODISCARD
    ETL_CONSTEXPR14
    static size_t valid_input_length(TInputIterator input_begin, size_t input_length)
    {
      if (input_length == 0U)
      {
        return 0U;
      }

      TInputIterator input_end = input_begin;

      if (input_length >= 2U)
      {
        // Jump forward to where padding characters possibly start.
        // There are never more than two padding characters.
        input_length -= 2U;
        etl::advance(input_end, input_length);
      }

      typedef etl::iterator_traits<TInputIterator>::value_type input_type;

      if (*input_end != padding<input_type>())
      {
        ++input_end;
        ++input_length;

        if (*input_end != padding<input_type>())
        {
          ++input_end;
          ++input_length;
        }
      }

      return input_length;
    }

    //*************************************************************************
    /// Discovers the number of valid decodable characters
    //*************************************************************************
    template <typename TInputIterator>
    ETL_NODISCARD
    ETL_CONSTEXPR14
    static size_t valid_input_length(TInputIterator input_begin, TInputIterator input_end)
    {
      return valid_input_length(input_begin, static_cast<size_t>(etl::distance(input_begin, input_end)));
    }

    //*************************************************************************
    /// Calculates the minimum buffer size required to decode from Base64
    //*************************************************************************
    template <typename TInputIterator>
    ETL_NODISCARD
    ETL_CONSTEXPR14
    static size_t decoded_size(TInputIterator input_begin, size_t input_length)
    {
      if (input_length == 0U)
      {
        return 0U;
      }

      input_length = valid_input_length(input_begin, input_length);
      --input_length;
      return input_length - (input_length / 4U);
    }

    //*************************************************************************
    /// Calculates the minimum buffer size required to decode from Base64
    //*************************************************************************
    template <typename TInputIterator>
    ETL_NODISCARD
    ETL_CONSTEXPR14
    static size_t decoded_size(TInputIterator input_begin, TInputIterator input_end)
    {
      return decoded_size(input_begin, static_cast<size_t>(etl::distance(input_begin, input_end)));
    }

    //*************************************************************************
    /// Calculates the minimum buffer size required to decode from Base64
    //*************************************************************************
    ETL_NODISCARD
    ETL_CONSTEXPR14
    static size_t decoded_size_from_valid_input_length(size_t input_length)
    {
      if (input_length == 0U)
      {
        return 0U;
      }

      --input_length;
      return input_length - (input_length / 4U);
    }

  private:

    // Sextets
    // 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H',
    // 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P',
    // 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X',
    // 'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f',
    // 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n',
    // 'o', 'p', 'q', 'r', 's', 't', 'u', 'v',
    // 'w', 'x', 'y', 'z', '0', '1', '2', '3',
    // '4', '5', '6', '7', '8', '9', '+', '/'

    //*************************************************************************
    // Translates an index into a sextet
    //*************************************************************************
    template <typename T>
    ETL_CONSTEXPR14 static T get_sextet_from_index(char index)
    {
      if ((index >= 0) && (index < 26))
      {
        return static_cast<T>('A' + index);
      }
      else if ((index >= 26) && (index < 52))
      {
        index -= 26;
        return static_cast<T>('a' + index);
      }
      else if ((index >= 52) && (index < 62))
      {
        index -= 52;
        return static_cast<T>('0' + index);
      }
      else if (index == 62)
      {
        return static_cast<T>('+');
      }
      else if (index == 63)
      {
        return static_cast<T>('/');
      }
      else
      {
        // Should never get here.
        assert(false);
        return padding<T>();
      }
    }

    //*************************************************************************
    // Translates a sextet into an index 
    //*************************************************************************
    template <typename T>
    ETL_CONSTEXPR14 static uint32_t get_index_from_sextet(T sextet)
    {
      if ((sextet >= 'A') && (sextet <= 'Z'))
      {
        return sextet - 'A';
      }
      else if ((sextet >= 'a') && (sextet <= 'z'))
      {
        return sextet - 'a' + 26;
      }
      else if ((sextet >= '0') && (sextet <= '9'))
      {
        return sextet - '0' + 52;
      }
      else if ((sextet == '+'))
      {
        return 62;
      }
      else if ((sextet == '/'))
      {
        return 63;
      }
      else
      {
        // Should never get here.
        assert(false);
        return 0;
      }
    }

    //*************************************************************************
    /// Gets the padding character
    //*************************************************************************
    template <typename T>
    ETL_NODISCARD
    ETL_CONSTEXPR14
    static T padding()
    {
      return static_cast<T>('=');
    }

    //*************************************************************************
    /// Encode to Base64 implementation
    //*************************************************************************
    template <typename TInputIterator, typename TOutputIterator>
    ETL_CONSTEXPR14
    size_t process_encode(TInputIterator  input,  size_t input_length, 
                          TOutputIterator output, size_t output_length)
    {
      if (input_length == 0U)
      {
        return 0;
      }

      // Count the actual number of sextets written.
      size_t output_count = 0;

      uint32_t octets = 0;

      // Read octet triplets and write sextet quartets
      while (input_length >= 3U)
      {
        // Read in three octets
        octets = (static_cast<uint32_t>(static_cast<unsigned char>(*input++)) << 16);
        octets = octets | (static_cast<uint32_t>(static_cast<unsigned char>(*input++)) << 8);
        octets = octets | static_cast<uint32_t>(static_cast<unsigned char>(*input++));

        // Write out four sextets
        *output++ = get_sextet_from_index<char>((octets >> 18) & b00111111);
        *output++ = get_sextet_from_index<char>((octets >> 12) & b00111111);
        *output++ = get_sextet_from_index<char>((octets >>  6) & b00111111);
        *output++ = get_sextet_from_index<char>((octets >>  0) & b00111111);

        input_length -= 3U;
        output_count += 4U;
      }

      // Any input left?
      if (input_length > 0)
      {
        // Write out any remaining sextets
        if (input_length == 1U)
        {
          // There is one octet remaining
          octets = static_cast<uint32_t>(static_cast<unsigned char>(*input++));
          octets <<= 4; // Adjust one octet (8 bits) for two sextets worth of data (12 bits)
          output_count += 2U;
        }
        else if (input_length == 2U)
        {
          // There are two octets remaining
          octets = static_cast<uint32_t>(static_cast<unsigned char>(*input++));
          octets <<= 8;
          octets = octets | static_cast<uint32_t>(static_cast<unsigned char>(*input++));
          octets <<= 2; // Adjust two octets (16 bits) for three sextets worth of data (18 bits)
          output_count += 3U;
        }

        int shift = static_cast<int>(input_length * 6U);

        while (shift >= 0)
        {
          *output++ = get_sextet_from_index<char>((octets >> shift) & b00111111);
          shift -= 6;
        }
      }

      if (use_padding)
      {
        // Pad out the end of the output buffer.
        while (output_count != output_length)
        {
          *output++ = padding<char>();
          ++output_count;
        }
      }

      return output_length;
    }

    //*************************************************************************
    /// Decode from Base64 implementation
    //*************************************************************************
    template <typename TInputIterator, typename TOutputIterator>
    ETL_CONSTEXPR14
    size_t process_decode(TInputIterator input, size_t input_length, TOutputIterator output, size_t output_length)
    {
      if (input_length == 0)
      {
        return 0;
      }

      // Read sextet quartets and write octet triplets
      while (input_length >= 4U)
      {
        // Read in four sextets
        uint32_t sextets = (get_index_from_sextet(*input++) << 18) | 
                           (get_index_from_sextet(*input++) << 12) | 
                           (get_index_from_sextet(*input++) << 6)  |
                           (get_index_from_sextet(*input++));

        // Write out three octets
        *output++ = (sextets >> 16) & b11111111;
        *output++ = (sextets >> 8)  & b11111111;
        *output++ = (sextets >> 0)  & b11111111;

        input_length -= 4U;
      }

      // Write out any remaining octets
      if (input_length == 2U)
      {
        uint32_t sextets = (get_index_from_sextet(*input++) << 6) |
                           (get_index_from_sextet(*input++));
        *output++ = (sextets >> 4) & b11111111;
        input_length -= 2U;
      }
      else if (input_length == 3U)
      {
        uint32_t sextets = (get_index_from_sextet(*input++) << 12) |
                           (get_index_from_sextet(*input++) << 6) |
                           (get_index_from_sextet(*input++));
        *output++ = (sextets >> 10) & b11111111;
        *output++ = (sextets >> 2) & b11111111;
        input_length -= 3U;
      }

      return output_length;
    }

    bool use_padding;
  };
}

#endif
