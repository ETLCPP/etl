///\file

/******************************************************************************
The MIT License(MIT)
Embedded Template Library.
https://github.com/ETLCPP/etl
https://www.etlcpp.com
Copyright(c) 2018 John Wellbelove
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

#include "platform.h"
#include "type_traits.h"
#include "nullptr.h"
#include "endianness.h"
#include "integral_limits.h"
#include "binary.h"
#include "algorithm.h"
#include "iterator.h"
#include "memory.h"
#include "delegate.h"
#include "span.h"
#include "optional.h"
#include "exception.h"
#include "error_handler.h"

#include <stdint.h>
#include <limits.h>

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
      : pdata(ETL_NULLPTR)
      , length_chars(0U)
    {
      restart();
    }

    //***************************************************************************
    /// Construct from range.
    //***************************************************************************
    bit_stream(void* begin_, void* end_)
      : pdata(reinterpret_cast<unsigned char*>(begin_))
      , length_chars(etl::distance(reinterpret_cast<unsigned char*>(begin_), reinterpret_cast<unsigned char*>(end_)))
    {
      restart();
    }

    //***************************************************************************
    /// Construct from begin and length.
    //***************************************************************************
    bit_stream(void* begin_, size_t length_)
      : pdata(reinterpret_cast<unsigned char*>(begin_))
      , length_chars(length_)
    {
      restart();
    }

    //***************************************************************************
    /// Construct from begin and length.
    //***************************************************************************
    void set_stream(void* begin_, size_t length_)
    {
      pdata  = reinterpret_cast<unsigned char*>(begin_);
      length_chars = length_;
      restart();
    }

    //***************************************************************************
    /// Construct from range.
    //***************************************************************************
    void set_stream(void* begin_, void* end_)
    {
      set_stream(begin_, etl::distance(reinterpret_cast<unsigned char*>(begin_), reinterpret_cast<unsigned char*>(end_)));
    }

    //***************************************************************************
    /// Sets the indexes back to the beginning of the stream.
    //***************************************************************************
    void restart()
    {
      bits_available_in_char = CHAR_BIT;
      char_index     = 0U;
      bits_available = CHAR_BIT * length_chars;
    }

    //***************************************************************************
    /// Returns <b>true</b> if the bitsteam indexes have reached the end.
    //***************************************************************************
    bool at_end() const
    {
      return (bits_available == 0U);
    }

    //***************************************************************************
    /// Writes a boolean to the stream
    //***************************************************************************
    bool put(bool value)
    {
      bool success = false;

      if (pdata != ETL_NULLPTR)
      {
        if (bits_available > 0)
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
      put(T value, uint_least8_t nbits = CHAR_BIT * sizeof(T))
    {
      return put_integral(static_cast<uint32_t>(value), nbits);
    }

#if ETL_USING_64BIT_TYPES
    //***************************************************************************
    /// For 64bit integral types
    //***************************************************************************
    bool put(int64_t value, uint_least8_t nbits = CHAR_BIT * sizeof(int64_t))
    {
      return put_integral(uint64_t(value), nbits);
    }

    //***************************************************************************
    /// For 64bit integral types
    //***************************************************************************
    bool put(uint64_t value, uint_least8_t nbits = CHAR_BIT * sizeof(uint64_t))
    {
      return put_integral(value, nbits);
    }
#endif

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

      for (size_t i = 0UL; i < sizeof(T); ++i)
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

      if (pdata != ETL_NULLPTR)
      {
        // Do we have enough bits?
        if (bits_available > 0U)
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
      get(T& value, uint_least8_t nbits = CHAR_BIT * sizeof(T))
    {
      bool success = false;
      uint_least8_t bits = nbits;

      if (pdata != ETL_NULLPTR)
      {
        // Do we have enough bits?
        if (bits_available >= nbits)
        {
          value = 0;

          // Get the bits from the stream.
          while (nbits != 0)
          {
            unsigned char mask_width = static_cast<unsigned char>(etl::min(nbits, bits_available_in_char));
            
            typedef typename etl::make_unsigned<T>::type chunk_t;           
            chunk_t chunk = get_chunk(mask_width);

            nbits -= mask_width;
            value |= static_cast<T>(chunk << nbits);
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

      if (pdata != ETL_NULLPTR)
      {
        uint_least8_t nbits = CHAR_BIT * sizeof(T);

        // Do we have enough bits?
        if (bits_available >= nbits)
        {
          // Temporary storage.
          etl::uninitialized_buffer_of<T, 1U> data;

          for (size_t i = 0UL; i < sizeof(T); ++i)
          {
             get(data.raw[i], CHAR_BIT);
          }

          from_bytes(reinterpret_cast<const unsigned char*>(data.raw), value);

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
      size_t s = char_index;

      // Current byte is used?
      if (bits_available_in_char != CHAR_BIT)
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
      return (length_chars * CHAR_BIT) - bits_available;
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
    bool put_integral(uint32_t value, uint_least8_t nbits)
    {
      bool success = false;

      if (pdata != ETL_NULLPTR)
      {
        // Do we have enough bits?
        if (bits_available >= nbits)
        {
          // Send the bits to the stream.
          while (nbits != 0)
          {
            unsigned char mask_width = static_cast<unsigned char>(etl::min(nbits, bits_available_in_char));
            nbits -= mask_width;
            uint32_t mask = ((uint32_t(1U) << mask_width) - 1U) << nbits;

            // Move chunk to lowest char bits.
            // Chunks are never larger than one char.
            uint32_t chunk = ((value & mask) >> nbits) << (bits_available_in_char - mask_width);

            put_chunk(static_cast<unsigned char>(chunk), mask_width);
          }

          success = true;
        }
      }

      return success;
    }

#if ETL_USING_64BIT_TYPES
    //***************************************************************************
    /// For unsigned integral types. 64bit
    //***************************************************************************
    bool put_integral(uint64_t value, uint_least8_t nbits)
    {
      bool success = false;

      if (pdata != ETL_NULLPTR)
      {
        // Do we have enough bits?
        if (bits_available >= nbits)
        {
          // Send the bits to the stream.
          while (nbits != 0)
          {
            unsigned char mask_width = static_cast<unsigned char>(etl::min(nbits, bits_available_in_char));
            nbits -= mask_width;
            uint64_t mask = ((uint64_t(1U) << mask_width) - 1U) << nbits;

            // Move chunk to lowest char bits.
            // Chunks are never larger than one char.
            uint64_t chunk = ((value & mask) >> nbits) << (bits_available_in_char - mask_width);

            put_chunk(static_cast<unsigned char>(chunk), mask_width);
          }

          success = true;
        }
      }

      return success;
    }
#endif

    //***************************************************************************
    /// Put a data chunk to the stream
    //***************************************************************************
    void put_chunk(unsigned char chunk, unsigned char nbits)
    {
      // Clear if new byte.
      if (bits_available_in_char == 8U)
      {
        pdata[char_index] = 0U;
      }

      pdata[char_index] |= chunk;
      step(nbits);
    }

    //***************************************************************************
    /// Get a data chunk from the stream
    //***************************************************************************
    unsigned char get_chunk(unsigned char nbits)
    {
      unsigned char value = pdata[char_index];

      value >>= (bits_available_in_char - nbits);

      unsigned char mask;

      if (nbits == CHAR_BIT)
      {
        mask = etl::integral_limits<unsigned char>::max;
      }
      else
      {
        mask = (1U << nbits) - 1;
      }

      value &= mask;

      step(nbits);

      return value;
    }

    //***************************************************************************
    /// Get a bool from the stream
    //***************************************************************************
    bool get_bit()
    {
      bool result = (pdata[char_index] & (1U << (bits_available_in_char - 1U))) != 0U;

      step(1U);

      return result;
    }

    //***************************************************************************
    /// Helper function for floating point types
    //***************************************************************************
    template <typename T>
    void from_bytes(const unsigned char* data, T& value)
    {
      etl::uninitialized_buffer_of<T, 1U> temp;

      // Network to host.
      if (etl::endianness::value() == etl::endian::little)
      {
        etl::reverse_copy(data, data + sizeof(T), temp.raw);
      }
      else
      {
        etl::copy(data, data + sizeof(T), temp.raw);
      }

      value = *reinterpret_cast<T*>(temp.raw);
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
        etl::reverse_copy(pf, pf + sizeof(T), data);
      }
      else
      {
        etl::copy(pf, pf + sizeof(T), data);
      }
    }

    //***************************************************************************
    /// Step the specified number of bits along the stream.
    /// The nbits will never be larger than 'bits_available_in_char'.
    //***************************************************************************
    void step(unsigned char nbits)
    {
      bits_available_in_char -= nbits;

      if (bits_available_in_char == 0)
      {
        ++char_index;
        bits_available_in_char = 8;
      }

      bits_available -= nbits;
    }

    unsigned char *pdata;                 ///< The start of the bitstream buffer.
    size_t        length_chars;           ///< The length, in char, of the bitstream buffer.
    unsigned char bits_available_in_char; ///< The number of available bits in the current char.
    size_t        char_index;             ///< The index of the char in the bitstream buffer.
    size_t        bits_available;         ///< The number of bits still available in the bitstream buffer.
  };

  //***************************************************************************
  /// Writes bits streams.
  //***************************************************************************
  class bit_stream_writer
  {
  public:

    typedef char value_type;
    typedef value_type* iterator;
    typedef const value_type* const_iterator;   
    typedef etl::span<value_type> callback_parameter_type;
    typedef etl::delegate<void(callback_parameter_type)> callback_type;

    //***************************************************************************
    /// Construct from span.
    //***************************************************************************
    bit_stream_writer(etl::span<char> span_, etl::endian stream_endianness_, callback_type callback_ = callback_type())
      : pdata(span_.begin())
      , length_chars(span_.size_bytes())
      , stream_endianness(stream_endianness_)
      , callback(callback_)
    {
      restart();
    }

    //***************************************************************************
    /// Construct from span.
    //***************************************************************************
    bit_stream_writer(etl::span<unsigned char> span_, etl::endian stream_endianness_, callback_type callback_ = callback_type())
      : pdata(reinterpret_cast<char*>(span_.begin()))
      , length_chars(span_.size_bytes())
      , stream_endianness(stream_endianness_)
      , callback(callback_)
    {
      restart();
    }

    //***************************************************************************
    /// Construct from range.
    //***************************************************************************
    bit_stream_writer(void* begin_, void* end_, etl::endian stream_endianness_, callback_type callback_ = callback_type())
      : pdata(reinterpret_cast<char*>(begin_))
      , length_chars(etl::distance(reinterpret_cast<unsigned char*>(begin_), reinterpret_cast<unsigned char*>(end_)))
      , stream_endianness(stream_endianness_)
      , callback(callback_)
    {
      restart();
    }

    //***************************************************************************
    /// Construct from begin and length.
    //***************************************************************************
    bit_stream_writer(void* begin_, size_t length_chars_, etl::endian stream_endianness_, callback_type callback_ = callback_type())
      : pdata(reinterpret_cast<char*>(begin_))
      , length_chars(length_chars_)
      , stream_endianness(stream_endianness_)
      , callback(callback_)
    {
      restart();
    }

    //***************************************************************************
    /// Sets the indexes back to the beginning of the stream.
    //***************************************************************************
    void restart()
    {
      bits_available_in_char = CHAR_BIT;
      char_index = 0U;
      bits_available = CHAR_BIT * length_chars;
    }

    //***************************************************************************
    /// Returns the maximum capacity in bits.
    //***************************************************************************
    size_t capacity_bytes() const
    {
      return length_chars;
    }

    //***************************************************************************
    size_t capacity_bits() const
    {
      return length_chars * CHAR_BIT;
    }

    //***************************************************************************
    /// Returns <b>true</b> if the bitsteam indexes have been reset.
    //***************************************************************************
    bool empty() const
    {
      return (bits_available == length_chars);
    }

    //***************************************************************************
    /// Returns <b>true</b> if the bitsteam indexes have reached the end.
    //***************************************************************************
    bool full() const
    {
      return (bits_available == 0U);
    }

    //***************************************************************************
    /// Writes a boolean to the stream
    //***************************************************************************
    void write_unchecked(bool value)
    {
      unsigned char chunk = value ? 1 : 0;
      write_data<unsigned char>(static_cast<unsigned char>(chunk), 1);
    }

    //***************************************************************************
    /// Writes a boolean to the stream
    //***************************************************************************
    bool write(bool value)
    {
      bool success = (available<1U>() > 0U);

      if (success)
      {
        write_unchecked(value);
      }

      return success;
    }

    //***************************************************************************
    /// For integral types
    //***************************************************************************
    template <typename T>
    typename etl::enable_if<etl::is_integral<T>::value, void>::type
      write_unchecked(T value, uint_least8_t nbits = CHAR_BIT * sizeof(T))
    {
      typedef typename etl::unsigned_type<T>::type unsigned_t;

      write_data<unsigned_t>(static_cast<unsigned_t>(value), nbits);
    }

    //***************************************************************************
    /// For integral types
    //***************************************************************************
    template <typename T>
    typename etl::enable_if<etl::is_integral<T>::value, bool>::type
      write(T value, uint_least8_t nbits = CHAR_BIT * sizeof(T))
    {
      bool success = (available(nbits) > 0U);

      if (success)
      {
        write_unchecked(value, nbits);
      }

      return success;
    }

    //***************************************************************************
    /// Skip n bits, up to the maximum space available.
    /// Returns <b>true</b> if the skip was possible.
    /// Returns <b>false</b> if the full skip size was not possible.
    //***************************************************************************
    bool skip(size_t nbits)
    {
      bool success = (nbits <= available_bits());

      if (success)
      {
        while (nbits > bits_available_in_char)
        {
          step(bits_available_in_char);
          nbits -= bits_available_in_char;
        }

        if (nbits != 0U)
        {
          step(static_cast<unsigned char>(nbits));
        }
      }

      return success;
    }

    //***************************************************************************
    /// Returns the number of bytes used in the stream.
    //***************************************************************************
    size_t size_bytes() const
    {
      size_t s = char_index;

      // Is the current byte partially used?
      if (bits_available_in_char != CHAR_BIT)
      {
        ++s;
      }

      return s;
    }

    //***************************************************************************
    /// Returns the number of bits used in the stream.
    //***************************************************************************
    size_t size_bits() const
    {
      return capacity_bits() - available_bits();
    }

    //***************************************************************************
    /// The number of multiples of 'Nbits' available in the stream.
    /// Compile time.
    //***************************************************************************
    template <size_t Nbits>
    size_t available() const
    {
      return bits_available / Nbits;
    }

    //***************************************************************************
    /// The number of T available in the stream.
    /// Compile time.
    //***************************************************************************
    template <typename T>
    size_t available() const
    {
      return available<CHAR_BIT * sizeof(T)>();
    }

    //***************************************************************************
    /// The number of 'bit width' available in the stream.
    /// Run time.
    //***************************************************************************
    size_t available(size_t nbits) const
    {
      return bits_available / nbits;
    }

    //***************************************************************************
    /// The number of bits left in the stream.
    //***************************************************************************
    size_t available_bits() const
    {
      return bits_available;
    }

    //***************************************************************************
    /// Returns start of the stream.
    //***************************************************************************
    iterator begin()
    {
      return pdata;
    }

    //***************************************************************************
    /// Returns start of the stream.
    //***************************************************************************
    const_iterator begin() const
    {
      return pdata;
    }

    //***************************************************************************
    /// Returns start of the stream.
    //***************************************************************************
    const_iterator cbegin() const
    {
      return pdata;
    }

    //***************************************************************************
    /// Returns end of the stream.
    //***************************************************************************
    iterator end()
    {
      return pdata + size_bytes();
    }

    //***************************************************************************
    /// Returns end of the stream.
    //***************************************************************************
    const_iterator end() const
    {
      return pdata + size_bytes();
    }

    //***************************************************************************
    /// Returns end of the stream.
    //***************************************************************************
    const_iterator cend() const
    {
      return pdata + size_bytes();
    }

    //***************************************************************************
    /// Returns a span of the used portion of the stream.
    //***************************************************************************
    etl::span<char> used_data()
    {
      return etl::span<char>(pdata, pdata + size_bytes());
    }

    //***************************************************************************
    /// Returns a span of the used portion of the stream.
    //***************************************************************************
    etl::span<const char> used_data() const
    {
      return etl::span<const char>(pdata, pdata + size_bytes());
    }

    //***************************************************************************
    /// Returns a span of whole the stream.
    //***************************************************************************
    etl::span<char> data()
    {
      return etl::span<char>(pdata, pdata + length_chars);
    }

    //***************************************************************************
    /// Returns a span of whole the stream.
    //***************************************************************************
    etl::span<const char> data() const
    {
      return etl::span<const char>(pdata, pdata + length_chars);
    }

    //***************************************************************************
    /// Flush the last byte, if partially filled, to the callback, if valid.
    //***************************************************************************
    void flush()
    {
      if (callback.is_valid())
      {
        if (bits_available_in_char != 0U)
        {
          char_index = 1U; // Indicate that the first char is actually 'full'.
          flush_full_bytes();
        }

        restart();
      }
    }

    //***************************************************************************
    /// Sets the function to call afer every write.
    //***************************************************************************
    void set_callback(callback_type callback_)
    {
      callback = callback_;
    }

    //***************************************************************************
    /// Gets the function to call afer every write.
    //***************************************************************************
    callback_type get_callback() const
    {
      return callback;
    }

  private:

    //***************************************************************************
    /// Write a value to the stream.
    /// It will be passed one of five unsigned types.
    //***************************************************************************
    template <typename T>
    void write_data(T value, uint_least8_t nbits)
    {
      // Make sure that we are not writing more bits than should be available.
      nbits = (nbits > (CHAR_BIT * sizeof(T))) ? (CHAR_BIT * sizeof(T)) : nbits;

      if (stream_endianness == etl::endian::little)
      {
        value = etl::reverse_bits(value);
        value = value >> ((CHAR_BIT * sizeof(T)) - nbits);
      }

      // Send the bits to the stream.
      while (nbits != 0)
      {
        unsigned char mask_width = static_cast<unsigned char>(etl::min(nbits, bits_available_in_char));
        nbits -= mask_width;
        T mask = ((T(1U) << mask_width) - 1U) << nbits;

        // Move chunk to lowest char bits.
        // Chunks are never larger than one char.
        T chunk = ((value & mask) >> nbits) << (bits_available_in_char - mask_width);

        write_chunk(static_cast<char>(chunk), mask_width);
      }

      if (callback.is_valid())
      {
        flush_full_bytes();
      }
    }

    //***************************************************************************
    /// Write a data chunk to the stream
    //***************************************************************************
    void write_chunk(char chunk, unsigned char nbits)
    {
      // Clear if new byte.
      if (bits_available_in_char == CHAR_BIT)
      {
        pdata[char_index] = 0U;
      }

      pdata[char_index] |= chunk;
      step(nbits);
    }

    //***************************************************************************
    /// Flush full bytes to the callback, if valid.
    /// Resets the stream to empty or the last unfilled byte.
    //***************************************************************************
    void flush_full_bytes()
    {
      // Is the first byte fully filled?
      if (char_index > 0U)
      {
        callback(callback_parameter_type(pdata, pdata + char_index));

        bits_available = CHAR_BIT * length_chars;

        if (bits_available_in_char != 0U)
        {
          // Move a partially filled last byte to the start of the buffer.
          pdata[0] = pdata[char_index];
          bits_available -= (CHAR_BIT - bits_available_in_char);
        }

        char_index = 0U;
      }
    }

    //***************************************************************************
    /// Step the specified number of bits along the stream.
    /// The width will never be larger than 'bits_available_in_char'.
    //***************************************************************************
    void step(unsigned char nbits)
    {
      bits_available_in_char -= nbits;

      if (bits_available_in_char == 0)
      {
        ++char_index;
        bits_available_in_char = CHAR_BIT;
      }

      bits_available -= nbits;
    }

    char* const       pdata;                  ///< The start of the bitstream buffer.
    const size_t      length_chars;           ///< The length of the bitstream buffer.
    const etl::endian stream_endianness;      ///< The endianness of the stream data.
    unsigned char     bits_available_in_char; ///< The number of available bits in the current char.
    size_t            char_index;             ///< The index of the current char in the bitstream buffer.
    size_t            bits_available;         ///< The number of bits still available in the bitstream buffer.
    callback_type     callback;               ///< An optional callback on every filled byte in buffer.
  };

  //***************************************************************************
  /// Implementation of the write function.
  /// For bool types only.
  //***************************************************************************
  inline void write_unchecked(etl::bit_stream_writer& stream, bool value)
  {
    stream.write_unchecked(value);
  }

  //***************************************************************************
  /// Implementation of the write function.
  /// For bool types only.
  //***************************************************************************
  inline bool write(etl::bit_stream_writer& stream, bool value)
  {
    return stream.write(value);
  }

  //***************************************************************************
  /// Default implementation of the write function.
  /// For integral types only (but not bool).
  /// Overload this to support custom types.
  //***************************************************************************
  template <typename T>
  typename etl::enable_if<etl::is_integral<T>::value, void>::type
    write_unchecked(etl::bit_stream_writer& stream, const T& value, uint_least8_t nbits = CHAR_BIT * sizeof(T))
  {
    stream.write_unchecked(value, nbits);
  }

  //***************************************************************************
  /// Default implementation of the write function.
  /// For integral types only (but not bool).
  /// Overload this to support custom types.
  //***************************************************************************
  template <typename T>
  typename etl::enable_if<etl::is_integral<T>::value, bool>::type
    write(etl::bit_stream_writer& stream, const T& value, uint_least8_t nbits = CHAR_BIT * sizeof(T))
  {
    return stream.write(value, nbits);
  }

  //***************************************************************************
  /// Reads bit streams
  //***************************************************************************
  class bit_stream_reader
  {
  public:

    typedef char value_type;
    typedef const char* const_iterator;

    //***************************************************************************
    /// Construct from span.
    //***************************************************************************
    bit_stream_reader(etl::span<char> span_, etl::endian stream_endianness_)
      : pdata(span_.begin())
      , length_chars(span_.size_bytes())
      , stream_endianness(stream_endianness_)
    {
      restart();
    }

    //***************************************************************************
    /// Construct from span.
    //***************************************************************************
    bit_stream_reader(etl::span<unsigned char> span_, etl::endian stream_endianness_)
      : pdata(reinterpret_cast<char*>(span_.begin()))
      , length_chars(span_.size_bytes())
      , stream_endianness(stream_endianness_)
    {
      restart();
    }

    //***************************************************************************
    /// Construct from range.
    //***************************************************************************
    bit_stream_reader(void* begin_, void* end_, etl::endian stream_endianness_)
      : pdata(reinterpret_cast<char*>(begin_))
      , length_chars(etl::distance(reinterpret_cast<char*>(begin_), reinterpret_cast<char*>(end_)))
      , stream_endianness(stream_endianness_)
    {
      restart();
    }

    //***************************************************************************
    /// Construct from begin and length.
    //***************************************************************************
    bit_stream_reader(void* begin_, size_t length_, etl::endian stream_endianness_)
      : pdata(reinterpret_cast<char*>(begin_))
      , length_chars(length_)
      , stream_endianness(stream_endianness_)
    {
      restart();
    }

    //***************************************************************************
    /// Sets the indexes back to the beginning of the stream.
    //***************************************************************************
    void restart()
    {
      bits_available_in_char = CHAR_BIT;
      char_index = 0U;
      bits_available = CHAR_BIT * length_chars;
    }

    //***************************************************************************
    /// For bool types
    //***************************************************************************
    template <typename T>
    typename etl::enable_if<etl::is_same<bool, T>::value, bool>::type
      read_unchecked()
    {
      return get_bit();
    }

    //***************************************************************************
    /// For bool types
    //***************************************************************************
    template <typename T>
    typename etl::enable_if<etl::is_same<bool, T>::value, etl::optional<bool> >::type
      read()
    {
      etl::optional<bool> result;

      if (bits_available > 0U)
      {
        result = read_unchecked<bool>();
      }

      return result;
    }

    //***************************************************************************
    /// For integral types
    //***************************************************************************
    template <typename T>
    typename etl::enable_if<etl::is_integral<T>::value && !etl::is_same<bool, T>::value, T>::type
      read_unchecked(uint_least8_t nbits = CHAR_BIT * sizeof(T))
    {
      typedef typename etl::unsigned_type<T>::type unsigned_t;

      T value = read_value<unsigned_t>(nbits, etl::is_signed<T>::value);

      return static_cast<T>(value);
    }

    //***************************************************************************
    /// For integral types
    //***************************************************************************
    template <typename T>
    typename etl::enable_if<etl::is_integral<T>::value && !etl::is_same<bool, T>::value, etl::optional<T> >::type
      read(uint_least8_t nbits = CHAR_BIT * sizeof(T))
    {
      etl::optional<T> result;

      // Do we have enough bits?
      if (bits_available >= nbits)
      {
        result = read_unchecked<T>(nbits); 
      }

      return result;
    }

    //***************************************************************************
    /// Returns the number of bytes in the stream buffer.
    //***************************************************************************
    size_t size_bytes() const
    {
      return length_chars;
    }

    //***************************************************************************
    /// Returns the number of bits in the stream buffer.
    //***************************************************************************
    size_t size_bits() const
    {
      return length_chars * CHAR_BIT;
    }

    //***************************************************************************
    /// Returns start of the stream.
    //***************************************************************************
    const_iterator begin() const
    {
      return pdata;
    }

    //***************************************************************************
    /// Returns start of the stream.
    //***************************************************************************
    const_iterator cbegin() const
    {
      return pdata;
    }

    //***************************************************************************
    /// Returns end of the stream.
    //***************************************************************************
    const_iterator end() const
    {
      return pdata + size_bytes();
    }

    //***************************************************************************
    /// Returns end of the stream.
    //***************************************************************************
    const_iterator cend() const
    {
      return pdata + size_bytes();
    }

    //***************************************************************************
    /// Returns a span of whole the stream.
    //***************************************************************************
    etl::span<const char> data() const
    {
      return etl::span<const char>(pdata, pdata + length_chars);
    }

    //***************************************************************************
    /// Skip n bits, up to the maximum space available.
    /// Returns <b>true</b> if the skip was possible.
    /// Returns <b>false</b> if the full skip size was not possible.
    //***************************************************************************
    bool skip(size_t nbits)
    {
      bool success = (nbits <= bits_available);

      if (success)
      {
        while (nbits > bits_available_in_char)
        {
          nbits -= bits_available_in_char;
          step(bits_available_in_char);
        }

        if (nbits != 0U)
        {
          step(static_cast<unsigned char>(nbits));
        }
      }

      return success;
    }

  private:

    //***************************************************************************
    /// Read a value from the stream.
    /// It will be passed one of five unsigned types.
    //***************************************************************************
    template <typename T>
    T read_value(uint_least8_t nbits, bool is_signed)
    {
      // Make sure that we are not reading more bits than should be available.
      nbits = (nbits > (CHAR_BIT * sizeof(T))) ? (CHAR_BIT * sizeof(T)) : nbits;

      T value = 0;
      uint_least8_t bits = nbits;

      // Get the bits from the stream.
      while (nbits != 0)
      {
        unsigned char mask_width = static_cast<unsigned char>(etl::min(nbits, bits_available_in_char));

        T chunk = get_chunk(mask_width);

        nbits -= mask_width;
        value |= static_cast<T>(chunk << nbits);
      }

      if (stream_endianness == etl::endian::little)
      {
        value = value << ((CHAR_BIT * sizeof(T)) - bits);
        value = etl::reverse_bits(value);
      }

      if (is_signed && (bits != (CHAR_BIT * sizeof(T))))
      {
        value = etl::sign_extend<T, T>(value, bits);
      }

      return value;
    }

    //***************************************************************************
    /// Get a data chunk from the stream
    //***************************************************************************
    unsigned char get_chunk(unsigned char nbits)
    {
      unsigned char value = pdata[char_index];
      value >>= (bits_available_in_char - nbits);

      unsigned char mask;

      if (nbits == CHAR_BIT)
      {
        mask = etl::integral_limits<unsigned char>::max;
      }
      else
      {
        mask = (1U << nbits) - 1;
      }

      value &= mask;

      step(nbits);

      return value;
    }

    //***************************************************************************
    /// Get a bool from the stream
    //***************************************************************************
    bool get_bit()
    {
      bool result = (pdata[char_index] & (1U << (bits_available_in_char - 1U))) != 0U;

      step(1U);

      return result;
    }

    //***************************************************************************
    /// Step the specified number of bits along the stream.
    /// The nbits will never be larger than 'bits_available_in_char'.
    //***************************************************************************
    void step(unsigned char nbits)
    {
      bits_available_in_char -= nbits;

      if (bits_available_in_char == 0)
      {
        ++char_index;
        bits_available_in_char = 8;
      }

      bits_available -= nbits;
    }

    char*             pdata;                  ///< The start of the bitstream buffer.
    size_t            length_chars;           ///< The length, in char, of the bitstream buffer.
    const etl::endian stream_endianness;      ///< The endianness of the stream data.
    unsigned char     bits_available_in_char; ///< The number of available bits in the current char.
    size_t            char_index;             ///< The index of the char in the bitstream buffer.
    size_t            bits_available;         ///< The number of bits still available in the bitstream buffer.
  };

  //***************************************************************************
  /// Read an unchecked type from a stream.
  //***************************************************************************
  template <typename T>
  T read_unchecked(etl::bit_stream_reader& stream)
  {
    return stream.read_unchecked<T>();
  }

  template <typename T>
  T read_unchecked(etl::bit_stream_reader& stream, uint_least8_t nbits)
  {
    return stream.read_unchecked<T>(nbits);
  }

  //***************************************************************************
  /// Read a checked type from a stream.
  //***************************************************************************
  template <typename T>
  etl::optional<T> read(etl::bit_stream_reader& stream)
  {
    return stream.read<T>();
  }

  template <typename T>
  etl::optional<T> read(etl::bit_stream_reader& stream, uint_least8_t nbits)
  {
    return stream.read<T>(nbits);
  }

  //***************************************************************************
  /// Read an unchecked bool from a stream.
  //***************************************************************************
  template <>
  inline bool read_unchecked<bool>(etl::bit_stream_reader& stream)
  {
    return stream.read_unchecked<bool>();
  }

  //***************************************************************************
  /// Read a bool from a stream.
  //***************************************************************************
  template <>
  inline etl::optional<bool> read<bool>(etl::bit_stream_reader& stream)
  {
    return stream.read<bool>();
  }
}

#include "private/minmax_pop.h"

#endif
