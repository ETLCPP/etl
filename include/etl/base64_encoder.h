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

#include "platform.h"
#include "static_assert.h"
#include "error_handler.h"
#include "type_traits.h"
#include "binary.h"
#include "algorithm.h"
#include "integral_limits.h"
#include "iterator.h"
#include "enum_type.h"
#include "delegate.h"
#include "span.h"

#include "base64.h"

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

    typedef etl::span<const char>                 span_type;
    typedef etl::delegate<void(const span_type&)> callback_type;

    //*************************************************************************
    /// Encode to Base64
    //*************************************************************************
    template <typename T>
    ETL_CONSTEXPR14
    bool encode(T value)
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
            callback(span());
            reset_output_buffer();
          }
        }
      }

      return !error();
    }

    //*************************************************************************
    /// Encode to Base64
    //*************************************************************************
    template <typename TInputIterator>
    ETL_CONSTEXPR14
    bool encode(TInputIterator input_begin, size_t input_length)
    {
      ETL_STATIC_ASSERT(ETL_IS_ITERATOR_TYPE_8_BIT_INTEGRAL(TInputIterator), "Input type must be an 8 bit integral");

      while (input_length-- != 0)
      {
        if (!encode(*input_begin++))
        {
          return false;
        }
      }

      return true;
    }

    //*************************************************************************
    /// Encode to Base64
    //*************************************************************************
    template <typename TInputIterator>
    ETL_CONSTEXPR14
    bool encode(TInputIterator input_begin, TInputIterator input_end)
    {
      ETL_STATIC_ASSERT(ETL_IS_ITERATOR_TYPE_8_BIT_INTEGRAL(TInputIterator), "Input type must be an 8 bit integral");

      while (input_begin != input_end)
      {
        if (!encode(*input_begin++))
        {
          return false;
        }
      }

      return true;
    }

    //*************************************************************************
    /// Encode to Base64
    //*************************************************************************
    template <typename TInputIterator>
    ETL_CONSTEXPR14
    bool encode_final(TInputIterator input_begin, size_t input_length)
    {
      return encode(input_begin, input_length) && flush();
    }

    //*************************************************************************
    /// Encode to Base64
    //*************************************************************************
    template <typename TInputIterator>
    ETL_CONSTEXPR14
    bool encode_final(TInputIterator input_begin, TInputIterator input_end)
    {
      return encode(input_begin, input_end) && flush();
    }

    //*************************************************************************
    /// Flush any remaining data to the output.
    //*************************************************************************
    ETL_CONSTEXPR14
    bool flush()
    {
      // Encode any remaining input data.
      bool success = encode_block();

      reset_input_buffer();

      if (success)
      {
        if (callback.is_valid())
        {
          // Send any remaining data.
          if (size() != 0)
          {
            callback(span());
          }

          // Indicate this was the final block.
          callback(span_type());

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
    size_t max_size() const
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

    //*************************************************************************
    /// Returns true if an error was detected.
    //*************************************************************************
    ETL_NODISCARD
    ETL_CONSTEXPR14
    bool error() const
    {
      return overflow();
    }

  protected:

    //*************************************************************************
    /// Constructor
    //*************************************************************************
    ETL_CONSTEXPR14
    ibase64_encoder(const char*   encoder_table_, 
                    bool          use_padding_,
                    char*         p_output_buffer_, 
                    size_t        ouput_buffer_max_size_,
                    callback_type callback_)
      : base64(encoder_table_, use_padding_)
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
    /// Encode one block of data.
    //*************************************************************************
    ETL_CONSTEXPR14
    bool encode_block()
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

      return !overflowed;
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

  //*************************************************************************
  /// Base64 RFC-2152 Encoder
  //*************************************************************************
  template <size_t Buffer_Size = etl::base64::Min_Encode_Buffer_Size>
  class base64_rfc2152_encoder : public ibase64_encoder
  {
  public:

    ETL_STATIC_ASSERT((Buffer_Size >= etl::base64::Min_Encode_Buffer_Size),       "Buffer size must be greater than etl::base64::Min_Encode_Buffer_Size");
    ETL_STATIC_ASSERT(((Buffer_Size % etl::base64::Min_Encode_Buffer_Size) == 0), "Buffer size must be a multiple of etl::base64::Min_Encode_Buffer_Size");

    //*************************************************************************
    /// Base64 RFC-2152 constructor.
    //*************************************************************************
    ETL_CONSTEXPR14
    base64_rfc2152_encoder()
      : ibase64_encoder(etl::base64::character_set_1(),
                        etl::base64::Padding::No_Padding,
                        output_buffer,
                        Buffer_Size,
                        callback_type())
      , output_buffer()
    {
    }

    //*************************************************************************
    /// Base64 RFC-2152 constructor.
    //*************************************************************************
    ETL_CONSTEXPR14
    base64_rfc2152_encoder(callback_type callback_)
      : ibase64_encoder(etl::base64::character_set_1(),
                        etl::base64::Padding::No_Padding,
                        output_buffer,
                        Buffer_Size,
                        callback_)
      , output_buffer()
    {
    }

    //*************************************************************************
    /// Calculate the required output encode buffer size.
    //*************************************************************************
    ETL_NODISCARD
    static
    ETL_CONSTEXPR14
    size_t safe_output_buffer_size(size_t input_length)
    {
      return ibase64_encoder::encoded_size(input_length, etl::base64::Padding::No_Padding);
    }

  private:

    /// The internal output buffer.
    char output_buffer[Buffer_Size];
  };

  //*************************************************************************
  /// Base64 RFC-3501 Encoder
  //*************************************************************************
  template <size_t Buffer_Size = etl::base64::Min_Encode_Buffer_Size>
  class base64_rfc3501_encoder : public ibase64_encoder
  {
  public:

    ETL_STATIC_ASSERT((Buffer_Size >= etl::base64::Min_Encode_Buffer_Size),      "Buffer size must be greater than etl::base64::Min_Encode_Buffer_Size");
    ETL_STATIC_ASSERT(((Buffer_Size% etl::base64::Min_Encode_Buffer_Size) == 0), "Buffer size must be a multiple of etl::base64::Min_Encode_Buffer_Size");

    //*************************************************************************
    /// Base64 RFC-3501 constructor.
    //*************************************************************************
    ETL_CONSTEXPR14
    base64_rfc3501_encoder()
      : ibase64_encoder(etl::base64::character_set_3(),
                        etl::base64::Padding::No_Padding,
                        output_buffer,
                        Buffer_Size,
                        callback_type())
      , output_buffer()
    {
    }

    //*************************************************************************
    /// Base64 RFC-3501 constructor.
    //*************************************************************************
    ETL_CONSTEXPR14
    base64_rfc3501_encoder(callback_type callback_)
      : ibase64_encoder(etl::base64::character_set_3(),
                        etl::base64::Padding::No_Padding,
                        output_buffer,
                        Buffer_Size,
                        callback_)
      , output_buffer()
    {
    }

    //*************************************************************************
    /// Calculate the required output encode buffer size.
    //*************************************************************************
    ETL_NODISCARD
    static
    ETL_CONSTEXPR14
    size_t safe_output_buffer_size(size_t input_length)
    {
      return ibase64_encoder::encoded_size(input_length, etl::base64::Padding::No_Padding);
    }

  private:

    /// The internal output buffer.
    char output_buffer[Buffer_Size];
  };

  //*************************************************************************
  /// Base64 RFC-4648 Encoder
  //*************************************************************************
  template <size_t Buffer_Size = etl::base64::Min_Encode_Buffer_Size>
  class base64_rfc4648_encoder : public ibase64_encoder
  {
  public:

    ETL_STATIC_ASSERT((Buffer_Size >= etl::base64::Min_Encode_Buffer_Size),       "Buffer size must be greater than etl::base64::Min_Encode_Buffer_Size");
    ETL_STATIC_ASSERT(((Buffer_Size % etl::base64::Min_Encode_Buffer_Size) == 0), "Buffer size must be a multiple of etl::base64::Min_Encode_Buffer_Size");

    //*************************************************************************
    /// Base64 RFC-4648 constructor.
    //*************************************************************************
    ETL_CONSTEXPR14
    base64_rfc4648_encoder()
      : ibase64_encoder(etl::base64::character_set_1(),
                        etl::base64::Padding::No_Padding,
                        output_buffer,
                        Buffer_Size,
                        callback_type())
      , output_buffer()
    {
    }

    //*************************************************************************
    /// Base64 RFC-4648 constructor.
    //*************************************************************************
    ETL_CONSTEXPR14
    base64_rfc4648_encoder(callback_type callback_)
      : ibase64_encoder(etl::base64::character_set_1(),
                        etl::base64::Padding::No_Padding,
                        output_buffer,
                        Buffer_Size,
                        callback_)
      , output_buffer()
    {
    }

    //*************************************************************************
    /// Calculate the required output encode buffer size.
    //*************************************************************************
    ETL_NODISCARD
    static
    ETL_CONSTEXPR14
    size_t safe_output_buffer_size(size_t input_length)
    {
      return ibase64_encoder::encoded_size(input_length, etl::base64::Padding::No_Padding);
    }

  private:

    /// The internal output buffer.
    char output_buffer[Buffer_Size];
  };

  //*************************************************************************
  /// Base64 RFC-4648-Padding Encoder
  //*************************************************************************
  template <size_t Buffer_Size = etl::base64::Min_Encode_Buffer_Size>
  class base64_rfc4648_padding_encoder : public ibase64_encoder
  {
  public:

    ETL_STATIC_ASSERT((Buffer_Size >= etl::base64::Min_Encode_Buffer_Size),      "Buffer size must be greater than etl::base64::Min_Encode_Buffer_Size");
    ETL_STATIC_ASSERT(((Buffer_Size% etl::base64::Min_Encode_Buffer_Size) == 0), "Buffer size must be a multiple of etl::base64::Min_Encode_Buffer_Size");

    //*************************************************************************
    /// Base64 RFC-4648-Padding constructor.
    //*************************************************************************
    ETL_CONSTEXPR14
    base64_rfc4648_padding_encoder()
      : ibase64_encoder(etl::base64::character_set_1(),
                        etl::base64::Padding::Use_Padding,
                        output_buffer,
                        Buffer_Size,
                        callback_type())
      , output_buffer()
    {
    }

    //*************************************************************************
    /// Base64 RFC-4648-Padding constructor.
    //*************************************************************************
    ETL_CONSTEXPR14
    base64_rfc4648_padding_encoder(callback_type callback_)
      : ibase64_encoder(etl::base64::character_set_1(),
                        etl::base64::Padding::Use_Padding,
                        output_buffer,
                        Buffer_Size,
                        callback_)
      , output_buffer()
    {
    }

    //*************************************************************************
    /// Calculate the required output encode buffer size.
    //*************************************************************************
    ETL_NODISCARD
    static
    ETL_CONSTEXPR14
    size_t safe_output_buffer_size(size_t input_length)
    {
      return ibase64_encoder::encoded_size(input_length, etl::base64::Padding::Use_Padding);
    }

  private:

    /// The internal output buffer.
    char output_buffer[Buffer_Size];
  };

  //*************************************************************************
  /// Base64 RFC-4648-URL Encoder
  //*************************************************************************
  template <size_t Buffer_Size = etl::base64::Min_Encode_Buffer_Size>
  class base64_rfc4648_url_encoder : public ibase64_encoder
  {
  public:

    ETL_STATIC_ASSERT((Buffer_Size >= etl::base64::Min_Encode_Buffer_Size),       "Buffer size must be greater than etl::base64::Min_Encode_Buffer_Size");
    ETL_STATIC_ASSERT(((Buffer_Size % etl::base64::Min_Encode_Buffer_Size) == 0), "Buffer size must be a multiple of etl::base64::Min_Encode_Buffer_Size");

    //*************************************************************************
    /// Base64 RFC-4648-URL constructor.
    //*************************************************************************
    ETL_CONSTEXPR14
    base64_rfc4648_url_encoder()
      : ibase64_encoder(etl::base64::character_set_2(),
                        etl::base64::Padding::No_Padding,
                        output_buffer,
                        Buffer_Size,
                        callback_type())
      , output_buffer()
    {
    }

    //*************************************************************************
    /// Base64 RFC-4648-URL constructor.
    //*************************************************************************
    ETL_CONSTEXPR14
    base64_rfc4648_url_encoder(callback_type callback_)
      : ibase64_encoder(etl::base64::character_set_2(),
                        etl::base64::Padding::No_Padding,
                        output_buffer,
                        Buffer_Size,
                        callback_)
      , output_buffer()
    {
    }

    //*************************************************************************
    /// Calculate the required output encode buffer size.
    //*************************************************************************
    ETL_NODISCARD
    static
    ETL_CONSTEXPR14
    size_t safe_output_buffer_size(size_t input_length)
    {
      return ibase64_encoder::encoded_size(input_length, etl::base64::Padding::No_Padding);
    }

  private:

    /// The internal output buffer.
    char output_buffer[Buffer_Size];
  };

  //*************************************************************************
  /// Base64 RFC-4648-URL_Padding Encoder
  //*************************************************************************
  template <size_t Buffer_Size = etl::base64::Min_Encode_Buffer_Size>
  class base64_rfc4648_url_padding_encoder : public ibase64_encoder
  {
  public:

    ETL_STATIC_ASSERT((Buffer_Size >= etl::base64::Min_Encode_Buffer_Size),      "Buffer size must be greater than etl::base64::Min_Encode_Buffer_Size");
    ETL_STATIC_ASSERT(((Buffer_Size% etl::base64::Min_Encode_Buffer_Size) == 0), "Buffer size must be a multiple of etl::base64::Min_Encode_Buffer_Size");

    //*************************************************************************
    /// Base64 RFC-4648-URL constructor.
    //*************************************************************************
    ETL_CONSTEXPR14
    base64_rfc4648_url_padding_encoder()
      : ibase64_encoder(etl::base64::character_set_2(),
                        etl::base64::Padding::Use_Padding,
                        output_buffer,
                        Buffer_Size,
                        callback_type())
      , output_buffer()
    {
    }

    //*************************************************************************
    /// Base64 RFC-4648-URL constructor.
    //*************************************************************************
    ETL_CONSTEXPR14
    base64_rfc4648_url_padding_encoder(callback_type callback_)
      : ibase64_encoder(etl::base64::character_set_2(),
                        etl::base64::Padding::Use_Padding,
                        output_buffer,
                        Buffer_Size,
                        callback_)
      , output_buffer()
    {
    }

    //*************************************************************************
    /// Calculate the required output encode buffer size.
    //*************************************************************************
    ETL_NODISCARD
    static
    ETL_CONSTEXPR14
    size_t safe_output_buffer_size(size_t input_length)
    {
      return ibase64_encoder::encoded_size(input_length, etl::base64::Padding::Use_Padding);
    }

  private:

    /// The internal output buffer.
    char output_buffer[Buffer_Size];
  };
}

#undef ETL_IS_TYPE_8_BIT_INTEGRAL
#undef ETL_IS_ITERATOR_TYPE_8_BIT_INTEGRAL

#endif
