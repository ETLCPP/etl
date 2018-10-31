///\file

/******************************************************************************
The MIT License(MIT)
Embedded Template Library.
https://github.com/ETLCPP/etl
https://www.etlcpp.com
Copyright(c) 2018 jwellbelove
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

#ifndef ETL_BIT_STREAM_INCLUDED
#define ETL_BIT_STREAM_INCLUDED

#include <stdint.h>
#include <limits.h>

#include "etl/platform.h"
#include "etl/type_traits.h"
#include "etl/nullptr.h"
#include "etl/endianness.h"
#include "etl/integral_limits.h"
#include "etl/binary.h"

#include "etl/stl/algorithm.h"
#include "etl/stl/iterator.h"

#include "private/minmax_push.h"

namespace etl
{
  //***************************************************************************
  /// Encodes and decodes bitstreams.
  /// Data must be stored in the stream in network order.
  //***************************************************************************
  class bit_stream
  {
  public:

    typedef const unsigned char* const_iterator;

    //***************************************************************************
    /// Default constructor.
    //***************************************************************************
    bit_stream()
      : pdata(nullptr),
        length(0U)
    {
      restart();
    }

    //***************************************************************************
    /// Construct from range.
    //***************************************************************************
    bit_stream(char* begin_, char* end_)
      : pdata(reinterpret_cast<unsigned char*>(begin_)),
        length(std::distance(begin_, end_))
    {
      restart();
    }

    //***************************************************************************
    /// Construct from range.
    //***************************************************************************
    bit_stream(unsigned char* begin_, unsigned char* end_)
      : pdata(begin_),
        length(std::distance(begin_, end_))
    {
      restart();
    }

    //***************************************************************************
    /// Construct from begin and length.
    //***************************************************************************
    bit_stream(char* begin_, size_t length_)
      : pdata(reinterpret_cast<unsigned char*>(begin_)),
        length(length_)
    {
      restart();
    }

    //***************************************************************************
    /// Construct from begin and length.
    //***************************************************************************
    bit_stream(unsigned char* begin_, size_t length_)
      : pdata(begin_),
        length(length_)
    {
      restart();
    }

    //***************************************************************************
    /// Construct from begin and length.
    //***************************************************************************
    void set_stream(char* begin_, size_t length_)
    {
      pdata  = reinterpret_cast<unsigned char*>(begin_);
      length = length_;
      restart();
    }

    //***************************************************************************
    /// Construct from begin and length.
    //***************************************************************************
    void set_stream(unsigned char* begin_, size_t length_)
    {
      pdata  = begin_;
      length = length_;
      restart();
    }

    //***************************************************************************
    /// Construct from range.
    //***************************************************************************
    void set_stream(char* begin_, char* end_)
    {
      set_stream(begin_, std::distance(begin_, end_));
    }

    //***************************************************************************
    /// Construct from range.
    //***************************************************************************
    void set_stream(unsigned char* begin_, unsigned char* end_)
    {
      set_stream(begin_, std::distance(begin_, end_));
    }

    //***************************************************************************
    /// Sets the indexes back to the beginning of the stream.
    //***************************************************************************
    void restart()
    {
      bits_in_byte   = 8;
      byte_index     = 0U;
      bits_remaining = CHAR_BIT * length;
    }

    //***************************************************************************
    /// Returns <b>true</b> if the bitsteam indexes have reached the end.
    //***************************************************************************
    bool at_end() const
    {
      return (bits_remaining == 0U);
    }

    //***************************************************************************
    /// Puts a boolean to the stream
    //***************************************************************************
    bool put(bool value)
    {
      bool success = false;

      if (pdata != nullptr)
      {
        if (bits_remaining > 0)
        {
          unsigned char chunk = value ? 1 : 0;
          put_integral(uint32_t(chunk), 1);
          success = true;
        }
      }

      return success;
    }

    //***************************************************************************
    /// For integral types
    //***************************************************************************
    template <typename T>
    typename etl::enable_if<etl::is_integral<T>::value, bool>::type
      put(T value, uint_least8_t width = CHAR_BIT * sizeof(T))
    {
      return put_integral(static_cast<uint32_t>(value), width);
    }

    //***************************************************************************
    /// For 64bit integral types
    //***************************************************************************
    bool put(int64_t value, uint_least8_t width = CHAR_BIT * sizeof(int64_t))
    {
      return put_integral(uint64_t(value), width);
    }

    //***************************************************************************
    /// For 64bit integral types
    //***************************************************************************
    bool put(uint64_t value, uint_least8_t width = CHAR_BIT * sizeof(uint64_t))
    {
      return put_integral(value, width);
    }

    //***************************************************************************
    /// For floating point types
    //***************************************************************************
    template <typename T>
    typename etl::enable_if<etl::is_floating_point<T>::value, bool>::type
      put(T value)
    {
      bool success = true;

      unsigned char data[sizeof(T)];
      to_bytes(value, data);

      for (size_t i = 0; i < sizeof(T); ++i)
      {
        if (!put_integral(uint32_t(data[i]), CHAR_BIT))
        {
          success = false;
        }
      }

      return success;
    }

    //***************************************************************************
    /// For bool types
    //***************************************************************************
    bool get(bool& value)
    {
      bool success = false;

      if (pdata != nullptr)
      {
        // Do we have enough bits?
        if (bits_remaining > 0)
        {
          value = get_bit();
          success = true;
        }
      }

      return success;
    }

    //***************************************************************************
    /// For integral types
    //***************************************************************************
    template <typename T>
    typename etl::enable_if<etl::is_integral<T>::value, bool>::type
      get(T& value, uint_least8_t width = CHAR_BIT * sizeof(T))
    {
      bool success = false;
      uint_least8_t bits = width;

      if (pdata != nullptr)
      {
        // Do we have enough bits?
        if (bits_remaining >= width)
        {
          value = 0;

          // Get the bits from the stream.
          while (width != 0)
          {
            unsigned char mask_width = static_cast<unsigned char>(std::min(width, bits_in_byte));
            unsigned char chunk = get_chunk(mask_width);

            width -= mask_width;
            value |= static_cast<T>(chunk) << width;
          }

          success = true;
        }
      }

      // Sign extend if signed type and not already full bit width.
      if (etl::is_signed<T>::value && (bits != (CHAR_BIT * sizeof(T))))
      {
        typedef typename etl::make_signed<T>::type ST;
        value = etl::sign_extend<ST, ST>(value, bits);
      }

      return success;
    }

    //***************************************************************************
    /// For floating point types
    //***************************************************************************
    template <typename T>
    typename etl::enable_if<etl::is_floating_point<T>::value, bool>::type
      get(T& value)
    {
      bool success = false;

      if (pdata != nullptr)
      {
        uint_least8_t width = CHAR_BIT * sizeof(T);

        // Do we have enough bits?
        if (bits_remaining >= width)
        {
          // Temporary storage.
          unsigned char data[sizeof(T)];

          for (size_t i = 0; i < sizeof(T); ++i)
          {
             get(data[i], CHAR_BIT);
          }

          from_bytes(data, value);

          bits_remaining -= width;

          success = true;
        }
      }

      return success;
    }

    //***************************************************************************
    /// Returns the number of bytes used in the stream.
    //***************************************************************************
    size_t size() const
    {
      size_t s = byte_index;

      // Current byte is used?
      if (bits_in_byte != CHAR_BIT)
      {
        ++s;
      }

      return s;
    }

    //***************************************************************************
    /// Returns the number of bits used in the stream.
    //***************************************************************************
    size_t bits() const
    {
      return (length * CHAR_BIT) - bits_remaining;
    }

    //***************************************************************************
    /// Returns start of the stream.
    //***************************************************************************
    const_iterator begin() const
    {
      return pdata;
    }

    //***************************************************************************
    /// Returns end of the stream.
    //***************************************************************************
    const_iterator end() const
    {
      return pdata + size();
    }

  private:

    //***************************************************************************
    /// For unsigned integral types
    //***************************************************************************
    bool put_integral(uint32_t value, uint_least8_t width)
    {
      bool success = false;

      if (pdata != nullptr)
      {
        // Do we have enough bits?
        if (bits_remaining >= width)
        {
          // Send the bits to the stream.
          while (width != 0)
          {
            unsigned char mask_width = static_cast<unsigned char>(std::min(width, bits_in_byte));
            width -= mask_width;
            uint32_t mask = ((uint32_t(1U) << mask_width) - 1U) << width;

            // Move chunk to lowest char bits.
            // Chunks are never larger than one char.
            uint32_t chunk = ((value & mask) >> width) << (bits_in_byte - mask_width);

            put_chunk(static_cast<unsigned char>(chunk), mask_width);
          }

          success = true;
        }
      }

      return success;
    }

    //***************************************************************************
    /// For unsigned integral types. 64bit
    //***************************************************************************
    bool put_integral(uint64_t value, uint_least8_t width)
    {
      bool success = false;

      if (pdata != nullptr)
      {
        // Do we have enough bits?
        if (bits_remaining >= width)
        {
          // Send the bits to the stream.
          while (width != 0)
          {
            unsigned char mask_width = static_cast<unsigned char>(std::min(width, bits_in_byte));
            width -= mask_width;
            uint64_t mask = ((uint64_t(1U) << mask_width) - 1U) << width;

            // Move chunk to lowest char bits.
            // Chunks are never larger than one char.
            uint64_t chunk = ((value & mask) >> width) << (bits_in_byte - mask_width);

            put_chunk(static_cast<unsigned char>(chunk), mask_width);
          }

          success = true;
        }
      }

      return success;
    }

    //***************************************************************************
    /// Put a data chunk to the stream
    //***************************************************************************
    void put_chunk(unsigned char chunk, unsigned char width)
    {
      // Clear if new byte.
      if (bits_in_byte == 8)
      {
        pdata[byte_index] = 0U;
      }

      pdata[byte_index] |= chunk;
      step(width);
    }

    //***************************************************************************
    /// Get a data chunk from the stream
    //***************************************************************************
    unsigned char get_chunk(unsigned char width)
    {
      unsigned char value = pdata[byte_index];

      value >>= (bits_in_byte - width);

      unsigned char mask;

      if (width == CHAR_BIT)
      {
        mask = etl::integral_limits<unsigned char>::max;
      }
      else
      {
        mask = (1U << width) - 1;
      }

      value &= mask;

      step(width);

      return value;
    }

    //***************************************************************************
    /// Get a bool from the stream
    //***************************************************************************
    bool get_bit()
    {
      bool result = (pdata[byte_index] & (1 << (bits_in_byte - 1))) != 0;

      step(1U);

      return result;
    }

    //***************************************************************************
    /// Helper function for floating point types
    //***************************************************************************
    template <typename T>
    void from_bytes(const unsigned char* data, T& value)
    {
      unsigned char temp[sizeof(T)];

      // Network to host.
      if (etl::endianness::value() == etl::endian::little)
      {
        std::reverse_copy(data, data + sizeof(T), temp);
      }
      else
      {
        std::copy(data, data + sizeof(T), temp);
      }

      value = *reinterpret_cast<T*>(temp);
    }

    //***************************************************************************
    /// Helper function for floating point types
    //***************************************************************************
    template <typename T>
    void to_bytes(T value, unsigned char* data)
    {
      unsigned char* pf = reinterpret_cast<unsigned char*>(&value);

      // Host to network.
      if (etl::endianness::value() == etl::endian::little)
      {
        std::reverse_copy(pf, pf + sizeof(T), data);
      }
      else
      {
        std::copy(pf, pf + sizeof(T), data);
      }
    }

    //***************************************************************************
    /// Step the specified number of bits along the stream.
    /// The width will never be larger than 'bits_in_byte'.
    //***************************************************************************
    void step(unsigned char width)
    {
      bits_in_byte -= width;

      if (bits_in_byte == 0)
      {
        ++byte_index;
        bits_in_byte = 8;
      }

      bits_remaining -= width;
    }

    unsigned char *pdata;         ///< The start of the bitstream buffer.
    size_t        length;         ///< The length, in unsigned char, of the bitstream buffer.
    unsigned char bits_in_byte;   ///< The number of available bits in the current char.
    size_t        byte_index;     ///< The index of the char in the bitstream buffer.
    size_t        bits_remaining; ///< The number of bits still available in the bitstream buffer.
  };
}

#include "private/minmax_pop.h"

#endif
