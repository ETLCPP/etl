//*************************************************************************
///Decode from Base64 from and to pointer/length
//*************************************************************************///\file

/******************************************************************************
The MIT License(MIT)
Embedded Template Library.
https://github.com/ETLCPP/etl
https://www.etlcpp.com
Copyright(c) 2024 John Wellbelove
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

#ifndef ETL_BASE64_DECODER_INCLUDED
#define ETL_BASE64_DECODER_INCLUDED

#include "etl/platform.h"
#include "etl/static_assert.h"
#include "etl/error_handler.h"
#include "etl/type_traits.h"
#include "etl/binary.h"
#include "etl/algorithm.h"
#include "etl/integral_limits.h"
#include "etl/iterator.h"
#include "etl/enum_type.h"
#include "etl/delegate.h"
#include "etl/span.h"

#include "etl/base64.h"

#include <stdint.h>

#if ETL_USING_STL
  #include <iterator>
#endif

#define ETL_IS_8_BIT_INTEGRAL(Type) (etl::is_integral<typename etl::remove_cv<Type>::type>::value && \
                                     (etl::integral_limits<typename etl::remove_cv<Type>::type>::bits == 8U))

#define ETL_IS_ITERATOR_TYPE_8_BIT_INTEGRAL(Type) (etl::is_integral<typename etl::iterator_traits<typename etl::remove_cv<Type>::type>::value_type>::value && \
                                                   (etl::integral_limits<typename etl::iterator_traits<typename etl::remove_cv<Type>::type>::value_type>::bits == 8U))

namespace etl
{
  //*************************************************************************
  /// Base64 Decoder
  //*************************************************************************
  class ibase64_decoder : public base64
  {
  public:

    typedef etl::span<const unsigned char>       span_type;
    typedef etl::delegate<void(span_type, bool)> callback_type;

    //*************************************************************************
    /// Decode to Base64
    //*************************************************************************
    template <typename T>
    ETL_CONSTEXPR14
    bool decode(T value)
    {
      ETL_STATIC_ASSERT(ETL_IS_8_BIT_INTEGRAL(T), "Input type must be an 8 bit integral");

      push_to_input_buffer(value);

      if (input_buffer_is_full())
      {
        if (decode_block())
        {
          if (callback.is_valid())
          {
            if (output_buffer_is_full())
            {
              callback(span(), false);
              reset_output_buffer();
            }
          }
        }
      
        reset_input_buffer();
      }

      return !error();
    }

    //*************************************************************************
    /// Decode from Base64
    //*************************************************************************
    template <typename TInputIterator>
    ETL_CONSTEXPR14
    bool decode(TInputIterator input_begin, TInputIterator input_end)
    {
      ETL_STATIC_ASSERT(ETL_IS_ITERATOR_TYPE_8_BIT_INTEGRAL(TInputIterator), "Input type must be an 8 bit integral");

      while (input_begin != input_end)
      {
        if (!decode(*input_begin++))
        {
          return false;
        }
      }

      return true;
    }

    //*************************************************************************
    /// Decode from Base64
    //*************************************************************************
    template <typename TInputIterator>
    ETL_CONSTEXPR14
    bool decode(TInputIterator input_begin, size_t input_length)
    {
      ETL_STATIC_ASSERT(ETL_IS_ITERATOR_TYPE_8_BIT_INTEGRAL(TInputIterator), "Input type must be an 8 bit integral");
      
      while (input_length-- != 0)
      {
        if (!decode(*input_begin++))
        {
          return false;
        }
      }

      return true;
    }
    
    //*************************************************************************
    /// Decode from Base64
    //*************************************************************************
    template <typename TInputIterator>
    ETL_CONSTEXPR14
    bool decode_final(TInputIterator input_begin, TInputIterator input_end)
    {
      return (decode(input_begin, input_end) && flush());
    }

    //*************************************************************************
    /// Decode from Base64
    //*************************************************************************
    template <typename TInputIterator>
    ETL_CONSTEXPR14
    bool decode_final(TInputIterator input_begin, size_t input_length)
    {
      return (decode(input_begin, input_length) && flush());
    }

    //*************************************************************************
    /// Flush any remaining data to the output.
    //*************************************************************************
    ETL_CONSTEXPR14
    bool flush()
    {
      // Encode any remaining input data.
      bool success = decode_block();

      reset_input_buffer();

      if (success)
      {
        if (callback.is_valid())
        {
          callback(span(), true);
          reset_output_buffer();
        }
      }

      return success;
    }

    //*************************************************************************
    /// Reset the encoder.
    //*************************************************************************
    ETL_CONSTEXPR14
    void restart()
    {
      reset_input_buffer();
      reset_output_buffer();
      overflow_detected     = false;
      invalid_data_detected = false;
    }

    //*************************************************************************
    /// Returns the beginning of the output buffer.
    //*************************************************************************
    ETL_NODISCARD
    ETL_CONSTEXPR14
    const unsigned char* begin() const
    {
      return p_output_buffer;
    }

    //*************************************************************************
    /// This only returns a useful value if a callback has not been set or called.
    //*************************************************************************
    ETL_NODISCARD
    ETL_CONSTEXPR14
    const unsigned char* end() const
    {
      return p_output_buffer + output_buffer_length;
    }

    //*************************************************************************
    /// Returns the beginning of the output buffer.
    //*************************************************************************
    ETL_NODISCARD
    ETL_CONSTEXPR14
    const unsigned char* cbegin() const
    {
      return p_output_buffer;
    }

    //*************************************************************************
    /// This only returns a useful value if a callback has not been set or called.
    //*************************************************************************
    ETL_NODISCARD
    ETL_CONSTEXPR14
    const unsigned char* cend() const
    {
      return p_output_buffer + output_buffer_length;
    }

    //*************************************************************************
    /// Returns the size of the output buffer.
    /// This only returns a useful value if a callback has not been set or called.
    //*************************************************************************
    ETL_NODISCARD
    ETL_CONSTEXPR14
    size_t size() const
    {
      return output_buffer_length;
    }

    //*************************************************************************
    /// Returns the maximum size of the output buffer.
    //*************************************************************************
    ETL_NODISCARD
    ETL_CONSTEXPR14
    size_t buffer_size() const
    {
      return output_buffer_max_size;
    }

    //*************************************************************************
    /// Get a span of the output data.
    /// This only returns a useful span if a callback has not been set or called.
    //*************************************************************************
    ETL_NODISCARD
    ETL_CONSTEXPR14
    span_type span() const
    {
      return span_type(begin(), end());
    }

    //*************************************************************************
    /// Returns true if the output buffer has overflowed
    //*************************************************************************
    ETL_NODISCARD
    ETL_CONSTEXPR14
    bool overflow() const
    {
      return overflow_detected;
    }

    //*************************************************************************
    /// Returns true if an invalid character was detected.
    //*************************************************************************
    ETL_NODISCARD
    ETL_CONSTEXPR14
    bool invalid_data() const
    {
      return invalid_data_detected;
    }

    //*************************************************************************
    /// Returns true if an error was detected.
    //*************************************************************************
    ETL_NODISCARD
    ETL_CONSTEXPR14
    bool error() const
    {
      return overflow() || invalid_data();
    }

  protected:

    //*************************************************************************
    /// Constructor
    //*************************************************************************
    ETL_CONSTEXPR14
      ibase64_decoder(const char*    encoder_table_,
                      bool           use_padding_,
                      unsigned char* p_output_buffer_,
                      size_t         ouput_buffer_max_size_,
                      callback_type  callback_)
      : base64(encoder_table_, use_padding_)
      , input_buffer()
      , input_buffer_length(0)
      , p_output_buffer(p_output_buffer_)
      , output_buffer_length(0)
      , output_buffer_max_size(ouput_buffer_max_size_)
      , callback(callback_)
      , overflow_detected(false)
      , invalid_data_detected(false)
    {
    }

    //*************************************************************************
    /// Discovers the number of valid decodable characters
    //*************************************************************************
    template <typename TInputIterator>
    ETL_NODISCARD
    static
    ETL_CONSTEXPR14
    bool is_valid_decode_input_length(TInputIterator input_begin, size_t input_length, bool use_padding)
    {
      if (input_length == 0U)
      {
        return true;
      }
    
      if (use_padding)
      {
        TInputIterator input_end = input_begin;
    
        if (input_length >= 2U)
        {
          // Jump forward to where padding characters possibly start.
          // There are never more than two padding characters.
          input_length -= 2U;
          etl::advance(input_end, input_length);
        }
    
        typedef typename etl::iterator_traits<TInputIterator>::value_type input_type;
    
        if (*input_end != padding<input_type>())
        {
          ++input_length;
          ++input_end;
    
          if (*input_end != padding<input_type>())
          {
            ++input_length;
          }
        }
      }
    
      // Not all input lengths are valid.   
      return (((input_length - 1U) % 4U) != 0U);
    }
    
    //*************************************************************************
    /// Discovers the number of valid decodable characters
    //*************************************************************************
    template <typename TInputIterator>
    ETL_NODISCARD
    static
    ETL_CONSTEXPR14
    size_t valid_decode_input_length(TInputIterator input_begin, TInputIterator input_end, bool use_padding)
    {
      return valid_decode_input_length(input_begin, static_cast<size_t>(etl::distance(input_begin, input_end)), use_padding);
    }
    
    //*************************************************************************
    /// Calculates the minimum buffer size required to decode from Base64
    //*************************************************************************
    ETL_NODISCARD
    static
    ETL_CONSTEXPR14
    size_t decoded_size_from_valid_input_length(size_t input_length)
    {
      if (input_length == 0U)
      {
        return 0U;
      }
    
      --input_length;
      return input_length - (input_length / 4U);
    }
    
    //*************************************************************************
    /// Calculates the minimum buffer size required to decode from Base64
    //*************************************************************************
    template <typename TInputIterator>
    ETL_NODISCARD
    static
    ETL_CONSTEXPR14
    size_t decoded_size_impl(TInputIterator input, size_t input_length, bool use_padding)
    {
      if (input_length == 0U)
      {
        return 0U;
      }
    
      input_length = valid_decode_input_length(input, input_length, use_padding);
      --input_length;
      return input_length - (input_length / 4U);
    }
    
    //*************************************************************************
    /// Calculates the minimum buffer size required to decode from Base64
    //*************************************************************************
    template <typename TInputIterator>
    ETL_NODISCARD
    static
    ETL_CONSTEXPR14
    size_t decoded_size_impl(TInputIterator input_begin, TInputIterator input_end, bool use_padding)
    {
      return decoded_size_impl(static_cast<size_t>(etl::distance(input_begin, input_end)), use_padding);
    }
    
  private:

    //*************************************************************************
    // Translates a sextet into an index 
    //*************************************************************************
    template <typename T>
    ETL_CONSTEXPR14
    uint32_t get_index_from_sextet(T sextet)
    {
      const char* encoder_table_end = encoder_table + 64;
      const char* p_sextet     = etl::find(encoder_table, encoder_table_end, static_cast<char>(sextet));
    
      if (p_sextet != encoder_table_end)
      {
        return static_cast<uint32_t>(etl::distance(encoder_table, p_sextet));
      }
      else
      {
        invalid_data_detected = true;
        return 0;
      }
    }
    
    //*************************************************************************
    /// Gets the padding character
    //*************************************************************************
    template <typename T>
    ETL_NODISCARD
    static
    ETL_CONSTEXPR14
    T padding()
    {
      return static_cast<T>('=');
    }
    
    //*************************************************************************
    /// Decode one block of data.
    //*************************************************************************
    ETL_CONSTEXPR14
    bool decode_block()
    {
      switch (input_buffer_length)
      {
        // Only triggered on call to flush().
        case 2:
        {
          uint32_t  sextets = (get_index_from_sextet(input_buffer[0]) << 6);
          sextets = sextets | (get_index_from_sextet(input_buffer[1]));
          push_to_output_buffer((sextets >> 4) & 0xFF);
          break;
        }

        // Only triggered on call to flush().
        case 3:
        {
          uint32_t  sextets = (get_index_from_sextet(input_buffer[0]) << 12);
          sextets = sextets | (get_index_from_sextet(input_buffer[1]) << 6);
          sextets = sextets | (get_index_from_sextet(input_buffer[2]));
          push_to_output_buffer((sextets >> 10) & 0xFF);
          push_to_output_buffer((sextets >> 2)  & 0xFF);
          break;
        }

        // Only triggered on call to decode().
        case 4:
        {
          // Read in four sextets
          uint32_t  sextets = (get_index_from_sextet(input_buffer[0]) << 18);
          sextets = sextets | (get_index_from_sextet(input_buffer[1]) << 12);
          sextets = sextets | (get_index_from_sextet(input_buffer[2]) << 6);
          sextets = sextets | (get_index_from_sextet(input_buffer[3]));

          // Write out three octets
          push_to_output_buffer((sextets >> 16) & 0xFF);
          push_to_output_buffer((sextets >> 8)  & 0xFF);
          push_to_output_buffer((sextets >> 0)  & 0xFF);
          break;
        }

        default:
        {
          break;
        }
      }

      ETL_ASSERT(!invalid_data_detected, ETL_ERROR(etl::base64_invalid_data));
      ETL_ASSERT(!overflow_detected,     ETL_ERROR(etl::base64_overflow));

      return (!invalid_data_detected && !overflow_detected);
    }

    //*************************************************************************
    // Push to the output buffer.
    //*************************************************************************
    ETL_CONSTEXPR14
    void push_to_output_buffer(unsigned char c)
    {
      if (output_buffer_length < output_buffer_max_size)
      {
        p_output_buffer[output_buffer_length++] = c;
      }
      else
      {
        overflow_detected = true;
      }
    }

    //*************************************************************************
    // 
    //*************************************************************************
    ETL_CONSTEXPR14
    bool output_buffer_is_full() const
    {
      return output_buffer_length == output_buffer_max_size;
    }

    //*************************************************************************
    // 
    //*************************************************************************
    ETL_CONSTEXPR14
    bool output_buffer_is_empty() const
    {
      return output_buffer_length == 0;
    }

    //*************************************************************************
    // 
    //*************************************************************************
    ETL_CONSTEXPR14
    void reset_output_buffer()
    {
      output_buffer_length = 0;
    }

    //*************************************************************************
    // Push to the input buffer.
    //*************************************************************************
    template <typename T>
    ETL_CONSTEXPR14
    void push_to_input_buffer(T value)
    {
      input_buffer[input_buffer_length++] = static_cast<uint8_t>(value);
    }

    //*************************************************************************
    // 
    //*************************************************************************
    ETL_CONSTEXPR14
    bool input_buffer_is_full() const
    {
      return input_buffer_length == 4U;
    }

    //*************************************************************************
    // 
    //*************************************************************************
    ETL_CONSTEXPR14
    void reset_input_buffer()
    {
      input_buffer_length = 0;
    }

    char   input_buffer[4];
    size_t input_buffer_length;

    unsigned char* p_output_buffer;
    size_t         output_buffer_length;
    const size_t   output_buffer_max_size;

    callback_type callback;

    bool overflow_detected;
    bool invalid_data_detected;
  };

  //*************************************************************************
  /// Base64 RFC-2152 Decoder
  //*************************************************************************
  template <size_t Buffer_Size_ = etl::base64::Min_Decode_Buffer_Size>
  class base64_rfc2152_decoder : public ibase64_decoder
  {
  public:

    static ETL_CONSTANT etl::base64::Encoding Encoding = etl::base64::Encoding::RFC_2152;
    static ETL_CONSTANT etl::base64::Padding  Padding  = etl::base64::Padding::No_Padding;

    // Round up the buffer size to a multiple of Min_Buffer_Size.
    static ETL_CONSTANT size_t Buffer_Size = (Buffer_Size_ == 0)
                                             ? etl::base64::Min_Decode_Buffer_Size
                                             : Buffer_Size_ + ((etl::base64::Min_Decode_Buffer_Size - (Buffer_Size_ % etl::base64::Min_Decode_Buffer_Size)) % etl::base64::Min_Decode_Buffer_Size);

    //*************************************************************************
    /// Base64 RFC-2152 constructor.
    //*************************************************************************
    ETL_CONSTEXPR14
    base64_rfc2152_decoder()
      : ibase64_decoder(etl::base64::character_set_1(),
                        etl::base64::Padding::No_Padding,
                        output_buffer,
                        Buffer_Size,
                        callback_type())
      , output_buffer{}
    {
    }

    //*************************************************************************
    /// Base64 RFC-2152 constructor.
    //*************************************************************************
    ETL_CONSTEXPR14
    base64_rfc2152_decoder(callback_type callback_)
      : ibase64_decoder(etl::base64::character_set_1(),
                        etl::base64::Padding::No_Padding,
                        output_buffer,
                        Buffer_Size,
                        callback_)
      , output_buffer{}
    {
    }

    //*************************************************************************
    /// Calculate the required output encode buffer size.
    //*************************************************************************
    ETL_NODISCARD
    static
    ETL_CONSTEXPR14
    size_t required_output_buffer_size(size_t input_length)
    {
      return ibase64_decoder::decoded_size_from_valid_input_length(input_length);
    }

  private:

    /// The internal output buffer.
    unsigned char output_buffer[Buffer_Size];
  };

  template <size_t Buffer_Size_>
  ETL_CONSTANT etl::base64::Encoding base64_rfc2152_decoder<Buffer_Size_>::Encoding;

  template <size_t Buffer_Size_>
  ETL_CONSTANT etl::base64::Padding base64_rfc2152_decoder<Buffer_Size_>::Padding;

  template <size_t Buffer_Size_>
  ETL_CONSTANT size_t base64_rfc2152_decoder<Buffer_Size_>::Buffer_Size;
}

#undef ETL_IS_TYPE_8_BIT_INTEGRAL
#undef ETL_IS_ITERATOR_TYPE_8_BIT_INTEGRAL

#endif
