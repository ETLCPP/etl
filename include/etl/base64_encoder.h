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

#ifndef ETL_BASE64_ENCODER_INCLUDED
#define ETL_BASE64_ENCODER_INCLUDED

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
  /// Base64 Encoder
  //*************************************************************************
  class ibase64_encoder : public base64
  {
  public:

    typedef etl::span<const char>                span_type;
    typedef etl::delegate<void(span_type, bool)> callback_type;

    //*************************************************************************
    /// Encode to Base64
    //*************************************************************************
    template <typename T>
    ETL_CONSTEXPR14
    void encode(T value)
    {
      ETL_STATIC_ASSERT(ETL_IS_8_BIT_INTEGRAL(T), "Input type must be an 8 bit integral");

      push_to_input_buffer(value);

      if (input_buffer_is_full())
      {
        encode_block();
        reset_input_buffer();

        if (callback.is_valid())
        {
          if (output_buffer_is_full())
          {
            callback(span(), false);
            reset_output_buffer();
          }
        }
      }
    }

    //*************************************************************************
    /// Encode to Base64
    //*************************************************************************
    template <typename TInputIterator>
    ETL_CONSTEXPR14
    void encode(TInputIterator input_begin, size_t input_length)
    {
      ETL_STATIC_ASSERT(ETL_IS_ITERATOR_TYPE_8_BIT_INTEGRAL(TInputIterator), "Input type must be an 8 bit integral");

      while (input_length-- != 0)
      {
        encode(*input_begin++);
      }
    }

    //*************************************************************************
    /// Encode to Base64
    //*************************************************************************
    template <typename TInputIterator>
    ETL_CONSTEXPR14
    void encode(TInputIterator input_begin, TInputIterator input_end)
    {
      ETL_STATIC_ASSERT(ETL_IS_ITERATOR_TYPE_8_BIT_INTEGRAL(TInputIterator), "Input type must be an 8 bit integral");

      while (input_begin != input_end)
      {
        encode(*input_begin++);
      }
    }

    //*************************************************************************
    /// Flush any remaining data to the output.
    //*************************************************************************
    ETL_CONSTEXPR14
    void flush()
    {
      // Encode any remaining input data.
      encode_block();

      if (callback.is_valid())
      {
        callback(span(), true);
        reset_output_buffer();
      }

      reset_input_buffer();
    }

    //*************************************************************************
    /// Reset the encoder.
    //*************************************************************************
    ETL_CONSTEXPR14
    void reset()
    {
      reset_input_buffer();
      reset_output_buffer();
    }

    //*************************************************************************
    /// Encode to Base64
    //*************************************************************************
    template <typename TInputIterator>
    ETL_CONSTEXPR14
    void encode_final(TInputIterator input_begin, size_t input_length)
    {
      encode(input_begin, input_length);
      flush();
    }

    //*************************************************************************
    /// Encode to Base64
    //*************************************************************************
    template <typename TInputIterator>
    ETL_CONSTEXPR14
    void encode_final(TInputIterator input_begin, TInputIterator input_end)
    {
      encode(input_begin, input_end);
      flush();
    }

    //*************************************************************************
    /// Returns the beginning of the output buffer.
    //*************************************************************************
    ETL_NODISCARD
    ETL_CONSTEXPR14
    const char* begin() const
    {
      return p_output_buffer;
    }

    //*************************************************************************
    /// This only returns a useful value if a callback has not been set or called.
    //*************************************************************************
    ETL_NODISCARD
    ETL_CONSTEXPR14
    const char* end() const
    {
      return p_output_buffer + output_buffer_length;
    }

    //*************************************************************************
    /// Returns the beginning of the output buffer.
    //*************************************************************************
    ETL_NODISCARD
    ETL_CONSTEXPR14
    const char* cbegin() const
    {
      return p_output_buffer;
    }

    //*************************************************************************
    /// This only returns a useful value if a callback has not been set or called.
    //*************************************************************************
    ETL_NODISCARD
    ETL_CONSTEXPR14
    const char* cend() const
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
    size_t max_size()
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
      return overflowed;
    }

  protected:

    //*************************************************************************
    /// Constructor
    //*************************************************************************
    ETL_CONSTEXPR14
    ibase64_encoder(Encoding      encoding_, 
                    const char*   encoder_table_, 
                    bool          use_padding_,
                    char*         p_output_buffer_, 
                    size_t        ouput_buffer_max_size_,
                    callback_type callback_)
      : base64(encoding_, encoder_table_, use_padding_)
      , input_buffer()
      , input_buffer_length(0)
      , p_output_buffer(p_output_buffer_)
      , output_buffer_length(0)
      , output_buffer_max_size(ouput_buffer_max_size_)
      , callback(callback_)
      , overflowed(false)
    {
    }

    //*************************************************************************
    /// 
    //*************************************************************************
    ETL_CONSTEXPR14
    void encode_block()
    {
      switch (input_buffer_length)
      {
        // Only triggered on call to flush().
        case 1:
        {
          uint32_t octets = input_buffer[0];
          octets = octets << 4; // Adjust one octet (8 bits) for two sextets worth of data (12 bits)

          // Write out two sextets + optional padding.
          push_to_output_buffer(encoder_table[(octets >> 6) & 0x3F]);
          push_to_output_buffer(encoder_table[(octets >> 0) & 0x3F]);

          if (use_padding)
          {
            push_to_output_buffer('=');
            push_to_output_buffer('=');
          }          
          break;
        }

        // Only triggered on call to flush().
        case 2:
        {
          uint32_t octets = (input_buffer[0] << 8) | input_buffer[1];
          octets <<= 2; // Adjust two octets (16 bits) for three sextets worth of data (18 bits)

          // Write out three sextets + optional padding.
          push_to_output_buffer(encoder_table[(octets >> 12) & 0x3F]);
          push_to_output_buffer(encoder_table[(octets >>  6) & 0x3F]);
          push_to_output_buffer(encoder_table[(octets >>  0) & 0x3F]);

          if (use_padding)
          {
            push_to_output_buffer('=');
          }
          break;
        }

        // Only triggered on call to encode().
        case 3:
        {
          uint32_t octets = (input_buffer[0] << 16) | (input_buffer[1] << 8) | input_buffer[2];

          // Write out four sextets
          push_to_output_buffer(encoder_table[(octets >> 18) & 0x3F]);
          push_to_output_buffer(encoder_table[(octets >> 12) & 0x3F]);
          push_to_output_buffer(encoder_table[(octets >>  6) & 0x3F]);
          push_to_output_buffer(encoder_table[(octets >>  0) & 0x3F]);
          break;
        }

        default:
        {
          break;
        }
      }

      ETL_ASSERT(!overflowed, ETL_ERROR(etl::base64_overflow));
    }

    //*************************************************************************
    /// Calculates the minimum buffer size required to encode to Base64
    //*************************************************************************
    ETL_NODISCARD
    static
    ETL_CONSTEXPR14
    size_t encoded_size(size_t input_length, bool use_padding)
    {
      size_t required_output_length = 0;

      if (input_length == 0U)
      {
        return 0U;
      }

      if (use_padding)
      {
        required_output_length = (input_length * 4U) / 3U;

        while ((required_output_length % 4U) != 0)
        {
          ++required_output_length;
        }
      }
      else
      {
        required_output_length = input_length + (((input_length - 1U) / 3U) + 1U);
      }

      while (required_output_length % 4)
      {
        ++required_output_length;
      }

      return required_output_length;
    }

  private:

    //*************************************************************************
    // Push to the output buffer.
    //*************************************************************************
    ETL_CONSTEXPR14 
    void push_to_output_buffer(char c)
    {
      if (output_buffer_length < output_buffer_max_size)
      {
        p_output_buffer[output_buffer_length++] = c;
      }
      else
      {
        overflowed = true;
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
      return input_buffer_length == 3U;
    }

    //*************************************************************************
    // 
    //*************************************************************************
    ETL_CONSTEXPR14
    void reset_input_buffer()
    {
      input_buffer_length = 0;
    }

    uint8_t input_buffer[3];
    size_t  input_buffer_length;

    char*        p_output_buffer;
    size_t       output_buffer_length;
    const size_t output_buffer_max_size;

    callback_type callback;
    
    bool overflowed;
  };


//  //*************************************************************************
//  /// Base64 Decoder
//  //*************************************************************************
//  class ibase64_decoder : public base64
//  {
//  public:
//
//    //*************************************************************************
//    /// Encode to Base64
//    /// Four parameter
//    //*************************************************************************
//    template <typename TInputIterator, typename TOutputIterator>
//    ETL_CONSTEXPR14
//      size_t encode(TInputIterator input_begin, TInputIterator input_end, TOutputIterator output_begin, TOutputIterator output_end)
//    {
//      ETL_STATIC_ASSERT(ETL_IS_ITERATOR_TYPE_8_BIT_INTEGRAL(TInputIterator), "Input type must be an 8 bit integral");
//      ETL_STATIC_ASSERT(ETL_IS_ITERATOR_TYPE_8_BIT_INTEGRAL(TOutputIterator), "Output type must be an 8 bit integral");
//
//      const size_t input_length = static_cast<size_t>(etl::distance(input_begin, input_end));
//      const size_t output_length = static_cast<size_t>(etl::distance(output_begin, output_end));
//
//      // Figure out if the output buffer is large enough.
//      size_t required_output_length = encoded_size(input_length, use_padding);
//      ETL_ASSERT_OR_RETURN_VALUE(output_length >= required_output_length, ETL_ERROR(base64_overflow), 0U);
//
//      return process_encode(input_begin, input_length, output_begin, required_output_length);
//    }
//
//    //*************************************************************************
//    /// Encode to Base64
//    /// For TOutputIterator value type not equal to void
//    /// Three parameter
//    /// Assumes the output container is large enough.
//    //*************************************************************************
//    template <typename TInputIterator, typename TOutputIterator>
//    ETL_CONSTEXPR14
//      size_t encode(TInputIterator input_begin, TInputIterator input_end, TOutputIterator output_begin)
//    {
//      ETL_STATIC_ASSERT(ETL_IS_ITERATOR_TYPE_8_BIT_INTEGRAL(TInputIterator), "Input type must be an 8 bit integral");
//      ETL_STATIC_ASSERT(ETL_IS_ITERATOR_TYPE_8_BIT_INTEGRAL(TOutputIterator), "Output type must be an 8 bit integral");
//
//      const size_t input_length = static_cast<size_t>(etl::distance(input_begin, input_end));
//      const size_t output_length = encoded_size(input_length, use_padding);
//
//      return process_encode(input_begin, input_length, output_begin, output_length);
//    }
//
//    //*************************************************************************
//    /// Encode to Base64
//    /// For etl::back_insert_iterator
//    /// Three parameter
//    /// Assumes the output container is large enough.
//    //*************************************************************************
//    template <typename TInputIterator, typename TContainer>
//    ETL_CONSTEXPR14
//      size_t encode(TInputIterator input_begin, TInputIterator input_end, etl::back_insert_iterator<TContainer> output_begin)
//    {
//      ETL_STATIC_ASSERT(ETL_IS_ITERATOR_TYPE_8_BIT_INTEGRAL(TInputIterator), "Input type must be an 8 bit integral");
//      ETL_STATIC_ASSERT(ETL_IS_8_BIT_INTEGRAL(typename TContainer::value_type), "Output type must be an 8 bit integral");
//
//      const size_t input_length = static_cast<size_t>(etl::distance(input_begin, input_end));
//      const size_t output_length = encoded_size(input_length, use_padding);
//
//      return process_encode(input_begin, input_length, output_begin, output_length);
//    }
//
//#if ETL_USING_STL
//    //*************************************************************************
//    /// Encode to Base64
//    /// For std::back_insert_iterator
//    /// Three parameter
//    /// Assumes the output container is large enough.
//    //*************************************************************************
//    template <typename TInputIterator, typename TContainer>
//    ETL_CONSTEXPR14
//      size_t encode(TInputIterator input_begin, TInputIterator input_end, std::back_insert_iterator<TContainer> output_begin)
//    {
//      ETL_STATIC_ASSERT(ETL_IS_ITERATOR_TYPE_8_BIT_INTEGRAL(TInputIterator), "Input type must be an 8 bit integral");
//      ETL_STATIC_ASSERT(ETL_IS_8_BIT_INTEGRAL(typename TContainer::value_type), "Output type must be an 8 bit integral");
//
//      const size_t input_length = static_cast<size_t>(etl::distance(input_begin, input_end));
//      const size_t output_length = encoded_size(input_length, use_padding);
//
//      return process_encode(input_begin, input_length, output_begin, output_length);
//    }
//#endif
//
//    //*************************************************************************
//    /// Encode to Base64
//    /// Four parameter
//    //*************************************************************************
//    template <typename TInputIterator, typename TOutputIterator>
//    ETL_CONSTEXPR14
//      size_t encode(TInputIterator input_begin, size_t input_length, TOutputIterator output_begin, size_t output_length)
//    {
//      ETL_STATIC_ASSERT(ETL_IS_ITERATOR_TYPE_8_BIT_INTEGRAL(TInputIterator), "Input type must be an 8 bit integral");
//      ETL_STATIC_ASSERT(ETL_IS_ITERATOR_TYPE_8_BIT_INTEGRAL(TOutputIterator), "Output type must be an 8 bit integral");
//
//      // Figure out if the output buffer is large enough.
//      size_t required_output_length = encoded_size(input_length, use_padding);
//      ETL_ASSERT_OR_RETURN_VALUE(output_length >= required_output_length, ETL_ERROR(base64_overflow), 0U);
//
//      return process_encode(input_begin, input_length, output_begin, required_output_length);
//    }
//
//    //*************************************************************************
//    /// Encode to Base64
//    /// For TOutputIterator value_type not equal to void
//    /// Three parameter
//    /// Assumes the output container is large enough.
//    //*************************************************************************
//    template <typename TInputIterator, typename TOutputIterator>
//    ETL_CONSTEXPR14
//      size_t encode(TInputIterator input_begin, size_t input_length, TOutputIterator output_begin)
//    {
//      ETL_STATIC_ASSERT(ETL_IS_ITERATOR_TYPE_8_BIT_INTEGRAL(TInputIterator), "Input type must be an 8 bit integral");
//      ETL_STATIC_ASSERT(ETL_IS_ITERATOR_TYPE_8_BIT_INTEGRAL(TOutputIterator), "Output type must be an 8 bit integral");
//
//      const size_t output_length = encoded_size(input_length, use_padding);
//
//      return process_encode(input_begin, input_length, output_begin, output_length);
//    }
//
//    //*************************************************************************
//    /// Encode to Base64
//    /// For etl::back_insert_iterator
//    /// Three parameter
//    /// Assumes the output container is large enough.
//    //*************************************************************************
//    template <typename TInputIterator, typename TContainer>
//    ETL_CONSTEXPR14
//      size_t encode(TInputIterator  input_begin, size_t input_length, etl::back_insert_iterator<TContainer> output_begin)
//    {
//      ETL_STATIC_ASSERT(ETL_IS_ITERATOR_TYPE_8_BIT_INTEGRAL(TInputIterator), "Input type must be an 8 bit integral");
//      ETL_STATIC_ASSERT(ETL_IS_8_BIT_INTEGRAL(typename TContainer::value_type), "Output type must be an 8 bit integral");
//
//      const size_t output_length = encoded_size(input_length, use_padding);
//
//      return process_encode(input_begin, input_length, output_begin, output_length);
//    }
//
//#if ETL_USING_STL
//    //*************************************************************************
//    /// Encode to Base64
//    /// For std::back_insert_iterator
//    /// Three parameter
//    /// Assumes the output container is large enough.
//    //*************************************************************************
//    template <typename TInputIterator, typename TContainer>
//    ETL_CONSTEXPR14
//      size_t encode(TInputIterator  input_begin, size_t input_length, std::back_insert_iterator<TContainer> output_begin)
//    {
//      ETL_STATIC_ASSERT(ETL_IS_ITERATOR_TYPE_8_BIT_INTEGRAL(TInputIterator), "Input type must be an 8 bit integral");
//      ETL_STATIC_ASSERT(ETL_IS_8_BIT_INTEGRAL(typename TContainer::value_type), "Output type must be an 8 bit integral");
//
//      const size_t output_length = encoded_size(input_length, use_padding);
//
//      return process_encode(input_begin, input_length, output_begin, output_length);
//    }
//#endif
//
//    //*************************************************************************
//    /// Decode from Base64
//    /// Four parameter
//    //*************************************************************************
//    template <typename TInputIterator, typename TOutputIterator>
//    ETL_CONSTEXPR14
//      size_t decode(TInputIterator input_begin, TInputIterator input_end, TOutputIterator output_begin, TOutputIterator output_end)
//    {
//      ETL_STATIC_ASSERT(ETL_IS_ITERATOR_TYPE_8_BIT_INTEGRAL(TInputIterator), "Input type must be an 8 bit integral");
//      ETL_STATIC_ASSERT(ETL_IS_ITERATOR_TYPE_8_BIT_INTEGRAL(TOutputIterator), "Output type must be an 8 bit integral");
//
//      const size_t input_length = valid_decode_input_length(input_begin, input_end, use_padding);
//      const size_t output_length = static_cast<size_t>(etl::distance(output_begin, output_end));
//
//      // Figure out if the output buffer is large enough.
//      size_t required_output_length = decoded_size_from_valid_input_length(input_length);
//      ETL_ASSERT_OR_RETURN_VALUE(output_length >= required_output_length, ETL_ERROR(base64_overflow), 0U);
//
//      return process_decode(input_begin, input_length, output_begin, required_output_length);
//    }
//
//    //*************************************************************************
//    /// Decode from Base64
//    /// Three parameter
//    /// Assumes the output container is large enough.
//    //*************************************************************************
//    template <typename TInputIterator, typename TOutputIterator>
//    ETL_CONSTEXPR14
//      size_t decode(TInputIterator input_begin, TInputIterator input_end, TOutputIterator output_begin)
//    {
//      ETL_STATIC_ASSERT(ETL_IS_ITERATOR_TYPE_8_BIT_INTEGRAL(TInputIterator), "Input type must be an 8 bit integral");
//      ETL_STATIC_ASSERT(ETL_IS_ITERATOR_TYPE_8_BIT_INTEGRAL(TOutputIterator), "Output type must be an 8 bit integral");
//
//      // Find the length of decodable characters.
//      const size_t input_length = valid_decode_input_length(input_begin, input_end, use_padding);
//
//      // Find the length of the decoded output.
//      const size_t required_output_length = decoded_size_impl(input_begin, input_length, use_padding);
//
//      return process_decode(input_begin, input_length, output_begin, required_output_length);
//    }
//
//    //*************************************************************************
//    /// Decode from Base64
//    /// For etl::back_insert_iterator
//    /// Three parameter
//    /// Assumes the output container is large enough.
//    //*************************************************************************
//    template <typename TInputIterator, typename TContainer>
//    ETL_CONSTEXPR14
//      size_t decode(TInputIterator input_begin, TInputIterator input_end, etl::back_insert_iterator<TContainer> output_begin)
//    {
//      ETL_STATIC_ASSERT(ETL_IS_ITERATOR_TYPE_8_BIT_INTEGRAL(TInputIterator), "Input type must be an 8 bit integral");
//      ETL_STATIC_ASSERT(ETL_IS_8_BIT_INTEGRAL(typename TContainer::value_type), "Output type must be an 8 bit integral");
//
//      // Find the length of decodable characters.
//      const size_t input_length = valid_decode_input_length(input_begin, input_end, use_padding);
//
//      // Find the length of the decoded output.
//      const size_t required_output_length = decoded_size_impl(input_begin, input_length, use_padding);
//
//      return process_decode(input_begin, input_length, output_begin, required_output_length);
//    }
//
//#if ETL_USING_STL
//    //*************************************************************************
//    /// Decode from Base64
//    /// For std::back_insert_iterator
//    /// Three parameter
//    /// Assumes the output container is large enough.
//    //*************************************************************************
//    template <typename TInputIterator, typename TContainer>
//    ETL_CONSTEXPR14
//      size_t decode(TInputIterator input_begin, TInputIterator input_end, std::back_insert_iterator<TContainer> output_begin)
//    {
//      ETL_STATIC_ASSERT(ETL_IS_ITERATOR_TYPE_8_BIT_INTEGRAL(TInputIterator), "Input type must be an 8 bit integral");
//      ETL_STATIC_ASSERT(ETL_IS_8_BIT_INTEGRAL(typename TContainer::value_type), "Output type must be an 8 bit integral");
//
//      // Find the length of decodable characters.
//      const size_t input_length = valid_decode_input_length(input_begin, input_end, use_padding);
//
//      // Find the length of the decoded output.
//      const size_t required_output_length = decoded_size_impl(input_begin, input_length, use_padding);
//
//      return process_decode(input_begin, input_length, output_begin, required_output_length);
//    }
//#endif
//
//    //*************************************************************************
//    /// Decode from Base64
//    /// Four parameter
//    //*************************************************************************
//    template <typename TInputIterator, typename TOutputIterator>
//    ETL_CONSTEXPR14
//      size_t decode(TInputIterator input_begin, size_t input_length, TOutputIterator output_begin, size_t output_length)
//    {
//      ETL_STATIC_ASSERT(ETL_IS_ITERATOR_TYPE_8_BIT_INTEGRAL(TInputIterator), "Input type must be an 8 bit integral");
//      ETL_STATIC_ASSERT(ETL_IS_ITERATOR_TYPE_8_BIT_INTEGRAL(TOutputIterator), "Output type must be an 8 bit integral");
//
//      // Find the length of decodable characters.
//      input_length = valid_decode_input_length(input_begin, input_length, use_padding);
//
//      // Figure out if the output buffer is large enough.
//      size_t required_output_length = decoded_size_from_valid_input_length(input_length);
//      ETL_ASSERT_OR_RETURN_VALUE(output_length >= required_output_length, ETL_ERROR(base64_overflow), 0U);
//
//      return process_decode(input_begin, input_length, output_begin, required_output_length);
//    }
//
//    //*************************************************************************
//    /// Decode from Base64
//    /// Three parameter
//    /// Assumes the output container is large enough.
//    //*************************************************************************
//    template <typename TInputIterator, typename TOutputIterator>
//    ETL_CONSTEXPR14
//      size_t decode(TInputIterator input_begin, size_t input_length, TOutputIterator output_begin)
//    {
//      ETL_STATIC_ASSERT(ETL_IS_ITERATOR_TYPE_8_BIT_INTEGRAL(TInputIterator), "Input type must be an 8 bit integral");
//      ETL_STATIC_ASSERT(ETL_IS_ITERATOR_TYPE_8_BIT_INTEGRAL(TOutputIterator), "Output type must be an 8 bit integral");
//
//      // Find the length of decodable characters.
//      input_length = valid_decode_input_length(input_begin, input_length, use_padding);
//
//      // Find the length of the decoded output.
//      const size_t required_output_length = decoded_size_impl(input_begin, input_length, use_padding);
//
//      return process_decode(input_begin, input_length, output_begin, required_output_length);
//    }
//
//    //*************************************************************************
//    /// Decode from Base64
//    /// For etl::back_insert_iterator
//    /// Three parameter
//    /// Assumes the output container is large enough.
//    //*************************************************************************
//    template <typename TInputIterator, typename TContainer>
//    ETL_CONSTEXPR14
//      size_t decode(TInputIterator input_begin, size_t input_length, etl::back_insert_iterator<TContainer> output_begin)
//    {
//      ETL_STATIC_ASSERT(ETL_IS_ITERATOR_TYPE_8_BIT_INTEGRAL(TInputIterator), "Input type must be an 8 bit integral");
//      ETL_STATIC_ASSERT(ETL_IS_8_BIT_INTEGRAL(typename TContainer::value_type), "Output type must be an 8 bit integral");
//
//      // Find the length of decodable characters.
//      input_length = valid_decode_input_length(input_begin, input_length, use_padding);
//
//      // Find the length of the decoded output.
//      const size_t output_length = decoded_size_impl(input_begin, input_length, use_padding);
//
//      return process_decode(input_begin, input_length, output_begin, output_length);
//    }
//
//#if ETL_USING_STL
//    //*************************************************************************
//    /// Decode from Base64
//    /// For std::back_insert_iterator
//    /// Three parameter
//    /// Assumes the output container is large enough.
//    //*************************************************************************
//    template <typename TInputIterator, typename TContainer>
//    ETL_CONSTEXPR14
//      size_t decode(TInputIterator input_begin, size_t input_length, std::back_insert_iterator<TContainer> output_begin)
//    {
//      ETL_STATIC_ASSERT(ETL_IS_ITERATOR_TYPE_8_BIT_INTEGRAL(TInputIterator), "Input type must be an 8 bit integral");
//      ETL_STATIC_ASSERT(ETL_IS_8_BIT_INTEGRAL(typename TContainer::value_type), "Output type must be an 8 bit integral");
//
//      // Find the length of decodable characters.
//      input_length = valid_decode_input_length(input_begin, input_length, use_padding);
//
//      // Find the length of the decoded output.
//      const size_t output_length = decoded_size_impl(input_begin, input_length, use_padding);
//
//      return process_decode(input_begin, input_length, output_begin, output_length);
//    }
//#endif
//
//  protected:
//
//
//
//    //*************************************************************************
//    /// Calculates the minimum buffer size required to encode to Base64
//    //*************************************************************************
//    ETL_NODISCARD
//      static
//      ETL_CONSTEXPR14
//      size_t encoded_size(size_t input_length, Padding padding)
//    {
//      size_t required_output_length;
//
//      if (input_length == 0U)
//      {
//        return 0U;
//      }
//
//      if (padding == Padding::Use_Padding)
//      {
//        required_output_length = (input_length * 4U) / 3U;
//
//        while ((required_output_length % 4U) != 0)
//        {
//          ++required_output_length;
//        }
//      }
//      else
//      {
//        required_output_length = input_length + (((input_length - 1U) / 3U) + 1U);
//      }
//
//      return required_output_length;
//    }
//
//    //*************************************************************************
//    /// Discovers the number of valid decodable characters
//    //*************************************************************************
//    template <typename TInputIterator>
//    ETL_NODISCARD
//      static
//      ETL_CONSTEXPR14
//      size_t valid_decode_input_length(TInputIterator input_begin, size_t input_length, bool use_padding)
//    {
//      if (input_length == 0U)
//      {
//        return 0U;
//      }
//
//      if (use_padding)
//      {
//        TInputIterator input_end = input_begin;
//
//        if (input_length >= 2U)
//        {
//          // Jump forward to where padding characters possibly start.
//          // There are never more than two padding characters.
//          input_length -= 2U;
//          etl::advance(input_end, input_length);
//        }
//
//        typedef typename etl::iterator_traits<TInputIterator>::value_type input_type;
//
//        if (*input_end != padding<input_type>())
//        {
//          ++input_length;
//          ++input_end;
//
//          if (*input_end != padding<input_type>())
//          {
//            ++input_length;
//          }
//        }
//      }
//
//      // Not all input lengths are valid.
//      ETL_ASSERT_OR_RETURN_VALUE((((input_length - 1U) % 4U) != 0U), ETL_ERROR(etl::base64_invalid_decode_input_length), 0U);
//
//      return input_length;
//    }
//
//    //*************************************************************************
//    /// Discovers the number of valid decodable characters
//    //*************************************************************************
//    template <typename TInputIterator>
//    ETL_NODISCARD
//      static
//      ETL_CONSTEXPR14
//      size_t valid_decode_input_length(TInputIterator input_begin, TInputIterator input_end, bool use_padding)
//    {
//      return valid_decode_input_length(input_begin, static_cast<size_t>(etl::distance(input_begin, input_end)), use_padding);
//    }
//
//    //*************************************************************************
//    /// Calculates the minimum buffer size required to decode from Base64
//    //*************************************************************************
//    ETL_NODISCARD
//      static
//      ETL_CONSTEXPR14
//      size_t decoded_size_from_valid_input_length(size_t input_length)
//    {
//      if (input_length == 0U)
//      {
//        return 0U;
//      }
//
//      --input_length;
//      return input_length - (input_length / 4U);
//    }
//
//    //*************************************************************************
//    /// Calculates the minimum buffer size required to decode from Base64
//    //*************************************************************************
//    template <typename TInputIterator>
//    ETL_NODISCARD
//      static
//      ETL_CONSTEXPR14
//      size_t decoded_size_impl(TInputIterator input, size_t input_length, bool use_padding)
//    {
//      if (input_length == 0U)
//      {
//        return 0U;
//      }
//
//      input_length = valid_decode_input_length(input, input_length, use_padding);
//      --input_length;
//      return input_length - (input_length / 4U);
//    }
//
//    //*************************************************************************
//    /// Calculates the minimum buffer size required to decode from Base64
//    //*************************************************************************
//    template <typename TInputIterator>
//    ETL_NODISCARD
//      static
//      ETL_CONSTEXPR14
//      size_t decoded_size_impl(TInputIterator input_begin, TInputIterator input_end, bool use_padding)
//    {
//      return decoded_size_impl(static_cast<size_t>(etl::distance(input_begin, input_end)), use_padding);
//    }
//
//  private:
//
//    //*************************************************************************
//    // Translates an index into a sextet
//    //*************************************************************************
//    template <typename T>
//    ETL_CONSTEXPR14
//      T get_sextet_from_index(int index)
//    {
//      return static_cast<T>(p_lookup[index]);
//    }
//
//    //*************************************************************************
//    // Translates a sextet into an index 
//    //*************************************************************************
//    template <typename T>
//    ETL_CONSTEXPR14
//      uint32_t get_index_from_sextet(T sextet)
//    {
//      const char* p_lookup_end = p_lookup + 64;
//      const char* p_sextet = etl::find(p_lookup, p_lookup_end, static_cast<char>(sextet));
//
//      if (p_sextet != p_lookup_end)
//      {
//        return static_cast<uint32_t>(etl::distance(p_lookup, p_sextet));
//      }
//      else
//      {
//        ETL_ASSERT_FAIL_AND_RETURN_VALUE(ETL_ERROR(base64_invalid_character), 0);
//      }
//    }
//
//    //*************************************************************************
//    /// Gets the padding character
//    //*************************************************************************
//    template <typename T>
//    ETL_NODISCARD
//      static
//      ETL_CONSTEXPR14
//      T padding()
//    {
//      return static_cast<T>('=');
//    }
//
//    //*************************************************************************
//    /// Is the character a carriage return?
//    //*************************************************************************
//    template <typename T>
//    ETL_NODISCARD
//      static
//      ETL_CONSTEXPR14
//      bool is_carriage_return(T c)
//    {
//      return (static_cast<char>(c) == '\r');
//    }
//
//    //*************************************************************************
//    /// Is the character a line break?
//    //*************************************************************************
//    template <typename T>
//    ETL_NODISCARD
//      static
//      ETL_CONSTEXPR14
//      bool is_line_feed(T c)
//    {
//      return (static_cast<char>(c) == '\n');
//    }
//
//    //*************************************************************************
//    /// Encode to Base64 implementation
//    //*************************************************************************
//    template <typename TInputIterator, typename TOutputIterator>
//    ETL_CONSTEXPR14
//      size_t process_encode(TInputIterator  input, size_t input_length,
//      TOutputIterator output, size_t output_length)
//    {
//      if (input_length == 0U)
//      {
//        return 0;
//      }
//
//      // Count the actual number of sextets written.
//      size_t output_count = 0;
//
//      uint32_t octets = 0;
//
//      // Read octet triplets and write sextet quartets
//      while (input_length >= 3U)
//      {
//        // Read in three octets
//        octets = (static_cast<uint32_t>(static_cast<unsigned char>(*input++)) << 16);
//        octets = octets | (static_cast<uint32_t>(static_cast<unsigned char>(*input++)) << 8);
//        octets = octets | static_cast<uint32_t>(static_cast<unsigned char>(*input++));
//
//        // Write out four sextets
//        *output++ = get_sextet_from_index<char>((octets >> 18) & b00111111);
//        *output++ = get_sextet_from_index<char>((octets >> 12) & b00111111);
//        *output++ = get_sextet_from_index<char>((octets >> 6) & b00111111);
//        *output++ = get_sextet_from_index<char>((octets >> 0) & b00111111);
//
//        input_length -= 3U;
//        output_count += 4U;
//      }
//
//      // Any input left?
//      if (input_length > 0)
//      {
//        // Write out any remaining sextets
//        if (input_length == 1U)
//        {
//          // There is one octet remaining
//          octets = static_cast<uint32_t>(static_cast<unsigned char>(*input++));
//          octets <<= 4; // Adjust one octet (8 bits) for two sextets worth of data (12 bits)
//          output_count += 2U;
//        }
//        else if (input_length == 2U)
//        {
//          // There are two octets remaining
//          octets = static_cast<uint32_t>(static_cast<unsigned char>(*input++));
//          octets <<= 8;
//          octets = octets | static_cast<uint32_t>(static_cast<unsigned char>(*input++));
//          octets <<= 2; // Adjust two octets (16 bits) for three sextets worth of data (18 bits)
//          output_count += 3U;
//        }
//
//        int shift = static_cast<int>(input_length * 6U);
//
//        while (shift >= 0)
//        {
//          *output++ = get_sextet_from_index<char>((octets >> shift) & b00111111);
//          shift -= 6;
//        }
//      }
//
//      if (use_padding)
//      {
//        // Pad out the end of the output buffer.
//        while (output_count != output_length)
//        {
//          *output++ = padding<char>();
//          ++output_count;
//        }
//      }
//
//      return output_length;
//    }
//
//    //*************************************************************************
//    /// Decode from Base64 implementation
//    //*************************************************************************
//    template <typename TInputIterator, typename TOutputIterator>
//    ETL_CONSTEXPR14
//      size_t process_decode(TInputIterator input, size_t input_length, TOutputIterator output, size_t output_length)
//    {
//      if (input_length == 0)
//      {
//        return 0;
//      }
//
//      // Read sextet quartets and write octet triplets
//      while (input_length >= 4U)
//      {
//        // Read in four sextets
//        uint32_t  sextets = (get_index_from_sextet(*input++) << 18);
//        sextets = sextets | (get_index_from_sextet(*input++) << 12);
//        sextets = sextets | (get_index_from_sextet(*input++) << 6);
//        sextets = sextets | (get_index_from_sextet(*input++));
//
//        // Write out three octets
//        *output++ = (sextets >> 16) & b11111111;
//        *output++ = (sextets >> 8) & b11111111;
//        *output++ = (sextets >> 0) & b11111111;
//
//        input_length -= 4U;
//      }
//
//      // Write out any remaining octets
//      if (input_length == 2U)
//      {
//        uint32_t  sextets = (get_index_from_sextet(*input++) << 6);
//        sextets = sextets | (get_index_from_sextet(*input++));
//        *output++ = (sextets >> 4) & b11111111;
//        input_length -= 2U;
//      }
//      else if (input_length == 3U)
//      {
//        uint32_t  sextets = (get_index_from_sextet(*input++) << 12);
//        sextets = sextets | (get_index_from_sextet(*input++) << 6);
//        sextets = sextets | (get_index_from_sextet(*input++));
//        *output++ = (sextets >> 10) & b11111111;
//        *output++ = (sextets >> 2) & b11111111;
//        input_length -= 3U;
//      }
//
//      return output_length;
//    }
//  };

  //*************************************************************************
  /// Base64 RFC-2152 Encoder
  //*************************************************************************
  template <size_t Buffer_Size_ = etl::base64::Min_Buffer_Size>
  class base64_rfc2152_encoder : public ibase64_encoder
  {
  public:

    static ETL_CONSTANT size_t Min_Buffer_Size = etl::base64::Min_Buffer_Size;

    // Round up the buffer size to a multiple of Min_Buffer_Size.
    static ETL_CONSTANT size_t Buffer_Size = (Buffer_Size_ == 0)
                                             ? Min_Buffer_Size
                                             : Buffer_Size_ + ((Min_Buffer_Size - (Buffer_Size_ % Min_Buffer_Size)) % Min_Buffer_Size);

    //*************************************************************************
    /// Base64 RFC-2152-URL constructor.
    //*************************************************************************
    ETL_CONSTEXPR14
    base64_rfc2152_encoder()
      : ibase64_encoder(etl::base64::Encoding::RFC_2152,
                        etl::base64::character_set_1(),
                        etl::base64::Padding::No_Padding,
                        output_buffer,
                        Buffer_Size,
                        callback_type())
      , output_buffer{}
    {
    }

    //*************************************************************************
    /// Base64 RFC-2152-URL constructor.
    //*************************************************************************
    ETL_CONSTEXPR14
    base64_rfc2152_encoder(callback_type callback_)
      : ibase64_encoder(etl::base64::Encoding::RFC_2152,
                        etl::base64::character_set_1(),
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
      return ibase64_encoder::encoded_size(input_length, etl::base64::Padding::No_Padding);
    }

  private:

    /// The internal output buffer.
    char output_buffer[Buffer_Size];
  };

  template <size_t Buffer_Size_>
  ETL_CONSTANT size_t base64_rfc2152_encoder<Buffer_Size_>::Min_Buffer_Size;

  template <size_t Buffer_Size_>
  ETL_CONSTEXPR size_t base64_rfc2152_encoder<Buffer_Size_>::Buffer_Size;

  //*************************************************************************
  /// Base64 RFC-3501 Encoder
  //*************************************************************************
  template <size_t Buffer_Size_ = etl::base64::Min_Buffer_Size>
  class base64_rfc3501_encoder : public ibase64_encoder
  {
  public:

    static ETL_CONSTANT size_t Min_Buffer_Size = etl::base64::Min_Buffer_Size;

    // Round up the buffer size to a multiple of Min_Buffer_Size.
    static ETL_CONSTANT size_t Buffer_Size = (Buffer_Size_ == 0)
                                             ? Min_Buffer_Size
                                             : Buffer_Size_ + ((Min_Buffer_Size - (Buffer_Size_ % Min_Buffer_Size)) % Min_Buffer_Size);

    //*************************************************************************
    /// Base64 RFC-2152 constructor.
    //*************************************************************************
    ETL_CONSTEXPR14
    base64_rfc3501_encoder()
      : ibase64_encoder(etl::base64::Encoding::RFC_3501,
                        etl::base64::character_set_3(),
                        etl::base64::Padding::No_Padding,
                        output_buffer,
                        Buffer_Size,
                        callback_type())
      , output_buffer{}
    {
    }

    //*************************************************************************
    /// Base64 RFC-3501 constructor.
    //*************************************************************************
    ETL_CONSTEXPR14
    base64_rfc3501_encoder(callback_type callback_)
      : ibase64_encoder(etl::base64::Encoding::RFC_3501,
                        etl::base64::character_set_3(),
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
      return ibase64_encoder::encoded_size(input_length, etl::base64::Padding::No_Padding);
    }

  private:

    /// The internal output buffer.
    char output_buffer[Buffer_Size];
  };

  template <size_t Buffer_Size_>
  ETL_CONSTANT size_t base64_rfc3501_encoder<Buffer_Size_>::Min_Buffer_Size;

  template <size_t Buffer_Size_>
  ETL_CONSTEXPR size_t base64_rfc3501_encoder<Buffer_Size_>::Buffer_Size;

  //*************************************************************************
  /// Base64 RFC-4648 Encoder
  //*************************************************************************
  template <bool Use_Padding, size_t Buffer_Size_ = etl::base64::Min_Buffer_Size>
  class base64_rfc4648_encoder : public ibase64_encoder
  {
  public:

    static ETL_CONSTANT size_t Min_Buffer_Size = etl::base64::Min_Buffer_Size;

    // Round up the buffer size to a multiple of Min_Buffer_Size.
    static ETL_CONSTANT size_t Buffer_Size = (Buffer_Size_ == 0)
                                             ? Min_Buffer_Size
                                             : Buffer_Size_ + ((Min_Buffer_Size - (Buffer_Size_ % Min_Buffer_Size)) % Min_Buffer_Size);

    //*************************************************************************
    /// Base64 RFC-4648 constructor.
    //*************************************************************************
    ETL_CONSTEXPR14
    base64_rfc4648_encoder()
      : ibase64_encoder(etl::base64::Encoding::RFC_2152,
                        etl::base64::character_set_1(),
                        Use_Padding,
                        output_buffer,
                        Buffer_Size,
                        callback_type())
      , output_buffer{}
    {
    }

    //*************************************************************************
    /// Base64 RFC-4648 constructor.
    //*************************************************************************
    ETL_CONSTEXPR14
    base64_rfc4648_encoder(callback_type callback_)
      : ibase64_encoder(etl::base64::Encoding::RFC_2152,
                        etl::base64::character_set_1(),
                        Use_Padding,
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
      return ibase64_encoder::encoded_size(input_length, Use_Padding == etl::base64::Padding::Use_Padding);
    }

  private:

    /// The internal output buffer.
    char output_buffer[Buffer_Size];
  };

  template <bool Use_Padding, size_t Buffer_Size_>
  ETL_CONSTANT size_t base64_rfc4648_encoder<Use_Padding, Buffer_Size_>::Min_Buffer_Size;

  template <bool Use_Padding, size_t Buffer_Size_>
  ETL_CONSTEXPR size_t base64_rfc4648_encoder<Use_Padding, Buffer_Size_>::Buffer_Size;

  //*************************************************************************
  /// Base64 RFC-4648-URL Encoder
  //*************************************************************************
  template <bool Use_Padding, size_t Buffer_Size_ = etl::base64::Min_Buffer_Size>
  class base64_rfc4648_url_encoder : public ibase64_encoder
  {
  public:

    static ETL_CONSTANT size_t Min_Buffer_Size = etl::base64::Min_Buffer_Size;

    // Round up the buffer size to a multiple of Min_Buffer_Size.
        // Round up the buffer size to a multiple of Min_Buffer_Size.
    static ETL_CONSTANT size_t Buffer_Size = (Buffer_Size_ == 0)
                                             ? Min_Buffer_Size
                                             : Buffer_Size_ + ((Min_Buffer_Size - (Buffer_Size_ % Min_Buffer_Size)) % Min_Buffer_Size);

    //*************************************************************************
    /// Base64 RFC-4648-URL constructor.
    //*************************************************************************
    ETL_CONSTEXPR14
    base64_rfc4648_url_encoder()
      : ibase64_encoder(etl::base64::Encoding::RFC_4648_URL,
                        etl::base64::character_set_2(),
                        Use_Padding,
                        output_buffer,
                        Buffer_Size,
                        callback_type())
      , output_buffer{}
    {
    }

    //*************************************************************************
    /// Base64 RFC-4648-URL constructor.
    //*************************************************************************
    ETL_CONSTEXPR14
    base64_rfc4648_url_encoder(callback_type callback_)
      : ibase64_encoder(etl::base64::Encoding::RFC_4648_URL,
                        etl::base64::character_set_2(),
                        Use_Padding,
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
      return ibase64_encoder::encoded_size(input_length, Use_Padding == etl::base64::Padding::Use_Padding);
    }

  private:

    /// The internal output buffer.
    char output_buffer[Buffer_Size];
  };

  template <bool Use_Padding, size_t Buffer_Size_>
  ETL_CONSTANT size_t base64_rfc4648_url_encoder<Use_Padding, Buffer_Size_>::Min_Buffer_Size;

  template <bool Use_Padding, size_t Buffer_Size_>
  ETL_CONSTEXPR size_t base64_rfc4648_url_encoder<Use_Padding, Buffer_Size_>::Buffer_Size;
}

#undef ETL_IS_TYPE_8_BIT_INTEGRAL
#undef ETL_IS_ITERATOR_TYPE_8_BIT_INTEGRAL

#endif
