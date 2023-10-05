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

#if ETL_USING_STL
  #include <string>
#endif

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
  /// Codec for Base64
  //*************************************************************************
  class base64
  {
  public:

    //*************************************************************************
    /// Encode to Base64 from and to pointer/length
    //*************************************************************************
    template <typename T>
    ETL_CONSTEXPR14
    static 
    typename etl::enable_if<etl::is_integral<T>::value && (etl::integral_limits<T>::bits == 8U), size_t>::type 
      encode(const T* input, size_t input_length, char* output, size_t output_length)
    {
      if (input_length == 0U)
      {
        return 0;
      }

      // Figure out if the output buffer is large enough.
      size_t required_output_length = encode_size(input_length);

      ETL_ASSERT_OR_RETURN_VALUE(output_length >= required_output_length, ETL_ERROR(base64_overflow), 0U);

      const T* p_in     = input;
      const T* p_in_end = input + input_length;

      char* p_out     = output;
      char* p_out_end = output + required_output_length;

      int next_sextet = First_Sextet;

      // Step through the input buffer, creating the output sextets.
      while (p_in != p_in_end)
      {
        T c = *p_in;
        char  index = 0;

        switch (next_sextet)
        {
          //**************************
          case First_Sextet:
          {
            index = static_cast<char>((*p_in & b11111100) >> 2);
            next_sextet = Second_Sextet;
            break;
          }

          //**************************
          case Second_Sextet:
          {
            index = static_cast<char>((c & b00000011) << 4);
            ++p_in;

            // Next byte valid?
            if (p_in != p_in_end)
            {
              index = index | ((*p_in & b11110000) >> 4);
            }
            next_sextet = Third_Sextet;
            break;
          }

          //**************************
          case Third_Sextet:
          {
            index = (c & b00001111) << 2;
            ++p_in;

            // Next byte valid?
            if (p_in != p_in_end)
            {
              index = index | static_cast<char>((*p_in & b11000000) >> 6);
            }
            next_sextet = Fourth_Sextet;
            break;
          }

          //**************************
          case Fourth_Sextet:
          {
            index = static_cast<char>(c & b00111111);
            ++p_in;
            next_sextet = First_Sextet;
            break;
          }

          //**************************
          default:
          {
            // Should never get here.
            assert(false);
            break;
          }
        }

        *p_out = get_sextet_from_index(index);
        ++p_out;
      }

      // Pad out the end of the output buffer.
      while (p_out != p_out_end)
      {
        *p_out++ = padding();
      }

      return static_cast<size_t>(etl::distance(output, p_out));
    }

    //*************************************************************************
    /// Encode to Base64 from and to pointer/pointer
    //*************************************************************************
    template <typename T>
    ETL_CONSTEXPR14
      static
      typename etl::enable_if<etl::is_integral<T>::value && (etl::integral_limits<T>::bits == 8U), size_t>::type
      encode(const T* input_begin, const T* input_end, char* output_begin, char* output_end)
    {
      return encode(input_begin, static_cast<size_t>(etl::distance(input_begin, input_end)),
        output_begin, static_cast<size_t>(etl::distance(output_begin, output_end)));
    }

    //*************************************************************************
    /// Encode to Base64 from and to span/span
    //*************************************************************************
    template <typename T, size_t Length1, size_t Length2>
    ETL_CONSTEXPR14
    static
    typename etl::enable_if<etl::is_integral<T>::value && (etl::integral_limits<T>::bits == 8U), size_t>::type
      encode(const etl::span<const T, Length1>& input_span,
             const etl::span<char, Length2>&    output_span)
    {
      return encode(input_span.begin(),  input_span.size(),
                    output_span.begin(), output_span.size());
    }

    //*************************************************************************
    /// Encode to Base64 from pointer/length to etl::istring
    //*************************************************************************
    template <typename T>
    ETL_CONSTEXPR14
    static
    typename etl::enable_if<etl::is_integral<T>::value && (etl::integral_limits<T>::bits == 8U), size_t>::type
      encode(const T* input_begin, size_t input_length,
             etl::istring& output)
    {
      output.resize(etl::base64::encode_size(input_length));

      return encode(input_begin,   input_length,
                    output.data(), output.size());
    }

    //*************************************************************************
    /// Encode to Base64 from pointer/pointer to etl::istring
    //*************************************************************************
    template <typename T>
    ETL_CONSTEXPR14
    static
    typename etl::enable_if<etl::is_integral<T>::value && (etl::integral_limits<T>::bits == 8U), size_t>::type
      encode(const T* input_begin, const T* input_end,
             etl::istring& output)
    {
      output.resize(etl::base64::encode_size(etl::distance(input_begin, input_end)));

      return encode(input_begin,   static_cast<size_t>(etl::distance(input_begin, input_end)),
                    output.data(), output.size());
    }

    //*************************************************************************
    /// Encode to Base64 from span to etl::istring
    //*************************************************************************
    template <typename T, size_t Length1>
    ETL_CONSTEXPR14
    static
    typename etl::enable_if<etl::is_integral<T>::value && (etl::integral_limits<T>::bits == 8U), size_t>::type
      encode(const etl::span<const T, Length1>& input_span,
             etl::istring& output)
    {
      output.resize(etl::base64::encode_size(Length1));

      return encode(input_span.begin(), input_span.size(),
                    output.data(),      output.size());
    }

    //*************************************************************************
    /// Calculates the buffer size required to encode to Base64
    //*************************************************************************
    ETL_NODISCARD
    ETL_CONSTEXPR14
    static
    size_t encode_size(size_t input_length)
    {
      size_t required_output_length = (input_length * 8U) / 6U;

      if ((input_length % 3U) != 0U)
      {
        while ((required_output_length % 4U) != 0)
        {
          ++required_output_length;
        }
      }

      return required_output_length;
    }

    //*************************************************************************
    /// Decode from Base64 from and to pointer/length
    //*************************************************************************
    template <typename T>
    ETL_CONSTEXPR14
    static
    typename etl::enable_if<etl::is_integral<T>::value && (etl::integral_limits<T>::bits == 8U), size_t>::type 
      decode(const char* input, size_t input_length, T* output, size_t output_length)
    {
      if (input_length == 0)
      {
        return 0;
      }

      // Figure out if the output buffer is large enough.
      size_t required_output_length = etl::base64::decode_size(input, input_length);

      ETL_ASSERT_OR_RETURN_VALUE(output_length >= required_output_length, ETL_ERROR(base64_overflow), 0U);

      const char* p_in     = input;
      const char* p_in_end = input + input_length;

      T* p_out = output;

      T c = 0;
      int next_sextet = First_Sextet;

      // Step through the input buffer, creating the output binary.
      while (p_in != p_in_end)
      {
        char sextet = *p_in++; // Get the sextet as a T.

        if (sextet == padding())
        {
          break;
        }

        char index = get_index_from_sextet(sextet);

        switch (next_sextet)
        {
          //**************************
          case First_Sextet:
          {
            c = (index & b00111111) << 2;
            next_sextet = Second_Sextet;
            break;
          }

          //**************************
          case Second_Sextet:
          {
            c |= (index & b00110000) >> 4;
            *p_out++ = static_cast<T>(c);
            c = (index & b00001111) << 4;
            next_sextet = Third_Sextet;
            break;
          }

          //**************************
          case Third_Sextet:
          {
            c |= (index & b00111100) >> 2;
            *p_out++ = static_cast<T>(c);
            c = (index & b00000011) << 6;
            next_sextet = Fourth_Sextet;
            break;
          }

          //**************************
          case Fourth_Sextet:
          {
            c |= (index & b00111111);
            *p_out++ = static_cast<T>(c);
            next_sextet = First_Sextet;
            break;
          }

          //**************************
          default:
          {
            // Should never get here.
            assert(false);
            break;
          }
        }
      }

      return static_cast<size_t>(etl::distance(output, p_out));
    }

    //*************************************************************************
    /// Decode from Base64 from and to pointer/pointer
    //*************************************************************************
    template <typename T>
    ETL_CONSTEXPR14
    static
    typename etl::enable_if<etl::is_integral<T>::value && (etl::integral_limits<T>::bits == 8U), size_t>::type
      decode(const char* input_begin, const char* input_end, T* output_begin, T* output_end)
    {
      return decode(input_begin,  static_cast<size_t>(etl::distance(input_begin,  input_end)),
                    output_begin, static_cast<size_t>(etl::distance(output_begin, output_end)));
    }

    //*************************************************************************
    /// Decode from Base64 from and to span/span
    //*************************************************************************
    template <typename T, size_t Length1, size_t Length2>
    ETL_CONSTEXPR14
      static
      typename etl::enable_if<etl::is_integral<T>::value && (etl::integral_limits<T>::bits == 8U), size_t>::type
      decode(const etl::span<const char, Length1>& input_span,
      const etl::span<T, Length2>& output_span)
    {
      return decode(input_span.begin(), input_span.size(),
        output_span.begin(), output_span.size());
    }

    //*************************************************************************
    /// Calculates the buffer size required to decode from Base64
    //*************************************************************************
    ETL_NODISCARD
    ETL_CONSTEXPR14
    static size_t decode_size(const char* input, size_t input_length)
    {
      if (input_length == 0U)
      {
        return 0U;
      }

      // Figure out the minimum output buffer size.
      size_t length = static_cast<size_t>(etl::distance(input, etl::find(input, input + input_length, padding())) - 1);
      size_t required_output_length = length - (length / 4U);

      return required_output_length;
    }

  private:

    //*************************************************************************
    /// Sextet index id.
    enum
    {
      First_Sextet,
      Second_Sextet,
      Third_Sextet,
      Fourth_Sextet
    };

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
    ETL_CONSTEXPR14 static char get_sextet_from_index(char index)
    {
      if ((index >= 0) && (index < 26))
      {
        return 'A' + index;
      }
      else if ((index >= 26) && (index < 52))
      {
        index -= 26;
        return 'a' + index;
      }
      else if ((index >= 52) && (index < 62))
      {
        index -= 52;
        return '0' + index;
      }
      else if (index == 62)
      {
        return '+';
      }
      else if (index == 63)
      {
        return '/';
      }
      else
      {
        // Should never get here.
        assert(false);
        return padding();
      }
    }

    //*************************************************************************
    // Translates a sextet into an index 
    //*************************************************************************
    ETL_CONSTEXPR14 static char get_index_from_sextet(char sextet)
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
      else if (sextet == '+')
      {
        return 62;
      }
      else if (sextet == '/')
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
    ETL_NODISCARD
    ETL_CONSTEXPR14
    static char padding()
    {
      return '=';
    }
  };
}

#endif
