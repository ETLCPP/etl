///\file

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
#include "etl/error_handler.h"
#include "etl/exception.h"
#include "etl/type_traits.h"
#include "etl/binary.h"
#include "etl/algorithm.h"
#include "etl/integral_limits.h"

#include <stdint.h>

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

  class base64
  {
  public:

    //*************************************************************************
    template <typename T>
    ETL_CONSTEXPR14
    static 
    typename etl::enable_if<etl::is_integral<T>::value && (etl::integral_limits<T>::bits == 8U), etl::span<const char> >::type 
      encode(const T* input, size_t input_length, char* output, size_t output_length)
    {
      etl::span<const char> result;

      int count = 0;

      if (input_length > 0U)
      {
        // Figure out if the output buffer is large enough.
        size_t required_output_length = (input_length * 8U) / 6U;

        if ((input_length % 3U) != 0U)
        {
          while ((required_output_length % 4U) != 0)
          {
            ++required_output_length;
          }
        }

        ETL_ASSERT_OR_RETURN_VALUE(output_length >= required_output_length, ETL_ERROR(base64_overflow), etl::span<const T>());

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
            case First_Sextet:
            {
              index = static_cast<char>((c & b11111100) >> 2);
              next_sextet = Second_Sextet;
              break;
            }

            case Second_Sextet:
            {
              index = static_cast<char>((c & b00000011) << 4);
              ++p_in;
              ++count;

              // Next byte valid?
              if (p_in != p_in_end)
              {
                c = *p_in;
                index = index | ((c & b11110000) >> 4);
              }
              next_sextet = Third_Sextet;
              break;
            }

            case Third_Sextet:
            {
              index = (c & b00001111) << 2;
              ++p_in;
              ++count;

              // Next byte valid?
              if (p_in != p_in_end)
              {
                c = *p_in;
                index = index | static_cast<char>((c & b11000000) >> 6);
              }
              next_sextet = Fourth_Sextet;
              break;
            }

            case Fourth_Sextet:
            {
              index = static_cast<char>(c & b00111111);
              ++p_in;
              ++count;
              next_sextet = First_Sextet;
              break;
            }

            default:
            {
              // Should never get here.
              break;
            }
          }

          *p_out = get_sextet_from_index(index);
          ++p_out;
        }

        // Pad out the end of the output buffer.
        while (p_out != p_out_end)
        {
          *p_out++ = Padding;
        }

        result = etl::span<const char>(output, p_out);
      }

      return result;
    }

    //*************************************************************************
    template <typename T, size_t Length1, size_t Length2>
    ETL_CONSTEXPR14
    static
    typename etl::enable_if<etl::is_integral<T>::value && (etl::integral_limits<T>::bits == 8U), etl::span<const char> >::type
     encode(const etl::span<const T, Length1>& input_span,
            const etl::span<char, Length2>&    output_span)
    {
      return encode(input_span.begin(), input_span.size(),
        output_span.begin(), output_span.size());
    }

    //*************************************************************************
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
    template <typename T>
    ETL_CONSTEXPR14
    static
    typename etl::enable_if<etl::is_integral<T>::value && (etl::integral_limits<T>::bits == 8U), etl::span<const T> >::type 
      decode(const char* input, size_t input_length, T* output, size_t output_length)
    {
      etl::span<const T> result;

      if (input_length > 0)
      {
        // Figure out if the output buffer is large enough.
        size_t length = etl::distance(input, etl::find(input, input + input_length, Padding)) - 1;
        size_t required_output_length = length - (length / 4U);

        ETL_ASSERT_OR_RETURN_VALUE(output_length >= required_output_length, ETL_ERROR(base64_overflow), etl::span<const T>());

        const char* p_in     = input;
        const char* p_in_end = input + input_length;

        T* p_out     = output;
        T* p_out_end = output + output_length;

        T c = 0;
        int next_sextet = First_Sextet;

        // Step through the input buffer, creating the output binary.
        while (p_in != p_in_end)
        {
          char sextet = *p_in++; // Get the sextet as a T.

          if (sextet == Padding)
          {
            break;
          }

          char index = get_index_from_sextet(sextet);

          switch (next_sextet)
          {
            case First_Sextet:
            {
              c = (index & b00111111) << 2;
              next_sextet = Second_Sextet;
              break;
            }

            case Second_Sextet:
            {
              c |= (index & b00110000) >> 4;
              *p_out++ = static_cast<T>(c);
              c = (index & b00001111) << 4;
              next_sextet = Third_Sextet;
              break;
            }

            case Third_Sextet:
            {
              c |= (index & b00111100) >> 2;
              *p_out++ = static_cast<T>(c);
              c = (index & b00000011) << 6;
              next_sextet = Fourth_Sextet;
              break;
            }

            case Fourth_Sextet:
            {
              c |= (index & b00111111);
              *p_out++ = static_cast<T>(c);
              next_sextet = First_Sextet;
              break;
            }

            default:
            {
              // Should never get here.
              break;
            }
          }
        }

        result = etl::span<const T>(output, p_out);
      }

      return result;
    }

    //*************************************************************************
    template <typename T, size_t Length1, size_t Length2>
    ETL_CONSTEXPR14
    static
    typename etl::enable_if<etl::is_integral<T>::value && (etl::integral_limits<T>::bits == 8U), etl::span<const T> >::type
      decode(const etl::span<const char, Length1>& input_span,
             const etl::span<T, Length2>&          output_span)
    {
      return decode(input_span.begin(),  input_span.size(),
                    output_span.begin(), output_span.size());
    }

    ////*************************************************************************
    //template <typename T, size_t Length>
    //ETL_CONSTEXPR14
    //static
    //typename etl::enable_if<etl::is_integral<T>::value && (etl::integral_limits<T>::bits == 8U), etl::span<const T> >::type
    //  decode_size(const etl::span<const char, Length>& input_span)
    //{
    //  // Figure out the minimum output buffer size.
    //  size_t length = etl::distance(input, etl::find(input, input + input_length, Padding)) - 1;
    //  size_t required_output_length = length - (length / 4U);

    //  return required_output_length;
    //}

    //*************************************************************************
    ETL_CONSTEXPR14
    static size_t decode_size(const char* input, size_t input_length)
    {
      // Figure out the minimum output buffer size.
      size_t length = etl::distance(input, etl::find(input, input + input_length, Padding)) - 1;
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

    //*************************************************************************
    /// The Base64 padding character
    static ETL_CONSTANT char Padding = '=';

    //*************************************************************************
    // Sextet lookup.
    static ETL_CONSTANT char lookup_char[64] =
    {
        'A', 'B', 'C', 'D', 'E', 'F' , 'G', 'H',
        'I', 'J', 'K', 'L', 'M', 'N' , 'O', 'P',
        'Q', 'R', 'S', 'T', 'U', 'V' , 'W', 'X',
        'Y', 'Z', 'a', 'b', 'c', 'd' , 'e', 'f',
        'g', 'h', 'i', 'j', 'k', 'l' , 'm', 'n',
        'o', 'p', 'q', 'r', 's', 't' , 'u', 'v',
        'w', 'x', 'y', 'z', '0', '1' , '2', '3',
        '4', '5', '6', '7', '8', '9' , '+', '/'
    };

    //*************************************************************************
    // Translates an index into a sextet
    ETL_CONSTEXPR14 static char get_sextet_from_index(char index)
    {
      return lookup_char[index];
    }

    //*************************************************************************
    // Translates a sextet into an index 
    ETL_CONSTEXPR14 static char get_index_from_sextet(char sextet)
    {
      const char* p = etl::find(lookup_char, lookup_char + 64, sextet);

      return static_cast<char>(etl::distance(lookup_char, p));
    }
  };
}

#endif
