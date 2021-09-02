///\file

/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl
https://www.etlcpp.com

Copyright(c) 2021 jwellbelove

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

#ifndef ETL_BYTE_STREAM_INCLUDED
#define ETL_BYTE_STREAM_INCLUDED

#include <stdint.h>
#include <limits.h>

#include "platform.h"
#include "type_traits.h"
#include "nullptr.h"
#include "endianness.h"
#include "integral_limits.h"
#include "algorithm.h"
#include "iterator.h"
#include "memory.h"
#include "span.h"
#include "iterator.h"
#include "optional.h"

namespace etl
{
  namespace private_byte_stream
  {
    class byte_stream_common
    {
    public:

      typedef const char* const_iterator;

      //***************************************************************************
      /// Sets the index back to the position in the stream. Default = 0.
      //***************************************************************************
      void restart(size_t n = 0U)
      {
        pcurrent = const_cast<char*>(pdata + n);
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
        return pcurrent;
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
      const_iterator cend() const
      {
        return pcurrent;
      }

      //***************************************************************************
      /// Returns a span of the used portion of the stream.
      //***************************************************************************
      etl::span<char> used_data() const
      {
        return etl::span<char>(pdata, pcurrent);
      }

      //***************************************************************************
      /// Returns a span of the free portion of the stream.
      //***************************************************************************
      etl::span<char> free_data() const
      {
        return etl::span<char>(pcurrent, pdata + length);
      }

      //***************************************************************************
      /// Returns a span of whole the stream.
      //***************************************************************************
      etl::span<char> data() const
      {
        return etl::span<char>(pdata, pdata + length);
      }

    protected:

      byte_stream_common(char* pdata_, size_t length_, etl::endian buffer_endianness_)
        : pdata(pdata_)
        , pcurrent(pdata_)
        , length(length_)
        , buffer_endianness(buffer_endianness_)
      {
      }

      void copy_value(const char* source, char* destination, size_t length) const
      {
        const etl::endian platform_endianness = etl::endianness::value();

        if (buffer_endianness == platform_endianness)
        {
          etl::copy(source, source + length, destination);
        }
        else
        {
          etl::reverse_copy(source, source + length, destination);
        }
      }

      char* const       pdata;             ///< The start of the byte stream buffer.
      char*             pcurrent;          ///< The current position in the byte stream buffer.
      const size_t      length;            ///< The length of the byte stream buffer.
      const etl::endian buffer_endianness; ///< The endianness of the buffer data.
    };
  }

  //***************************************************************************
  /// Encodes a byte stream.
  //***************************************************************************
  class byte_stream_writer : public private_byte_stream::byte_stream_common
  {
  public:

    //***************************************************************************
    /// Construct from span.
    //***************************************************************************
    byte_stream_writer(etl::span<char> span_, etl::endian buffer_endianness_ = etl::endian::big)
      : byte_stream_common(span_.begin(), span_.size_bytes(), buffer_endianness_)
    {
    }

    //***************************************************************************
    /// Construct from range.
    //***************************************************************************
    byte_stream_writer(char* begin_, char* end_, etl::endian buffer_endianness_ = etl::endian::big)
      : byte_stream_common(begin_, etl::distance(begin_, end_), buffer_endianness_)
    {
    }

    //***************************************************************************
    /// Construct from begin and length.
    //***************************************************************************
    byte_stream_writer(char* begin_, size_t length_, etl::endian buffer_endianness_ = etl::endian::big)
      : byte_stream_common(begin_, length_, buffer_endianness_)
    {
    }

    //***************************************************************************
    /// Construct from array.
    //***************************************************************************
    template <typename T, size_t Size>
    byte_stream_writer(T(&begin_)[Size], etl::endian buffer_endianness_ = etl::endian::big)
      : byte_stream_common(begin_, begin_ + (Size * sizeof(T)), buffer_endianness_)
    {
    }

    //***************************************************************************
    /// Writes a boolean to the stream
    //***************************************************************************
    bool write(bool value)
    {
      bool success = false;

      if (available<bool>() > 0U)
      {
        *pcurrent++ = value ? 1 : 0;
        success = true;
      }
 
      return success;
    }

    //***************************************************************************
    /// Write a value to the stream.
    //***************************************************************************
    template <typename T>
    typename etl::enable_if<etl::is_integral<T>::value || etl::is_floating_point<T>::value, bool>::type
      write(T value)
    {
      bool success = false;

      if (available<T>() > 0U)
      {
        to_bytes<T>(value);
        success = true;
      }

      return success;
    }

    //***************************************************************************
    /// Write a range of T to the stream.
    //***************************************************************************
    template <typename T>
    typename etl::enable_if<etl::is_integral<T>::value || etl::is_floating_point<T>::value, bool>::type
      write(const etl::span<T>& range)
    {
      bool success = false;

      if (available<T>() >= range.size())
      {
        typename etl::span<T>::const_iterator itr = range.begin();

        while (itr != range.end())
        {
          to_bytes(*itr++);
        }

        success = true;
      }

      return success;
    }

    //***************************************************************************
    /// Returns <b>true</b> if the byte stream index has reached the end.
    //***************************************************************************
    bool full() const
    {
      return size_bytes() == capacity();
    }

    //***************************************************************************
    /// Returns <b>true</b> if the byte stream is empty.
    //***************************************************************************
    bool empty() const
    {
      return size_bytes() == 0U;
    }

    //***************************************************************************
    /// Returns the number of bytes used in the stream.
    //***************************************************************************
    size_t size_bytes() const
    {
      return etl::distance(pdata, static_cast<char* const>(pcurrent));
    }

    //***************************************************************************
    /// Returns the maximum number of bytes in the stream.
    //***************************************************************************
    size_t capacity() const
    {
      return length;
    }

    //***************************************************************************
    /// The number of T left in the stream.
    //***************************************************************************
    template <typename T>
    size_t available() const
    {
      return (capacity() - size_bytes()) / sizeof(T);
    }

  private:

    //***************************************************************************
    /// to_bytes
    //***************************************************************************
    template <typename T>
    typename etl::enable_if<sizeof(T) == 1U, void>::type
      to_bytes(const T value)
    {
      *pcurrent++ = static_cast<char>(value);
    }

    //*********************************
    template <typename T>
    typename etl::enable_if<sizeof(T) != 1U, void>::type
      to_bytes(const T value)
    {
      const char* pv = reinterpret_cast<const char*>(&value);
      copy_value(pv, pcurrent, sizeof(T));
      pcurrent += sizeof(T);
    }
  };

  //***************************************************************************
  /// Decodes byte streams.
  /// Data must be stored in the stream in network order.
  //***************************************************************************
  class byte_stream_reader : public private_byte_stream::byte_stream_common
  {
  public:

    //***************************************************************************
    /// Construct from span.
    //***************************************************************************
    byte_stream_reader(etl::span<char> span_, etl::endian buffer_endianness_ = etl::endian::big)
      : byte_stream_common(span_.begin(), span_.size_bytes(), buffer_endianness_)
    {
    }

    //***************************************************************************
    /// Construct from range.
    //***************************************************************************
    byte_stream_reader(char* begin_, char* end_, etl::endian buffer_endianness_ = etl::endian::big)
      : byte_stream_common(begin_, etl::distance(begin_, end_), buffer_endianness_)
    {
    }

    //***************************************************************************
    /// Construct from begin and length.
    //***************************************************************************
    byte_stream_reader(char* begin_, size_t length_, etl::endian buffer_endianness_ = etl::endian::big)
      : byte_stream_common(begin_, length_, buffer_endianness_)
    {
    }

    //***************************************************************************
    /// Construct from array.
    //***************************************************************************
    template <typename T, size_t Size>
    byte_stream_reader(T(&begin_)[Size], etl::endian buffer_endianness_ = etl::endian::big)
      : byte_stream_common(begin_, begin_ + (Size * sizeof(T)), buffer_endianness_)
    {
    }

    //***************************************************************************
    /// Read a value from the stream.
    //***************************************************************************
    template <typename T>
    typename etl::enable_if<etl::is_integral<T>::value || etl::is_floating_point<T>::value, etl::optional<T> >::type
      read()
    {
      etl::optional<T> result;

      // Do we have enough room?
      if (available<T>() > 0U)
      {
        result = from_bytes<T>();
      }

      return result;
    }

    //***************************************************************************
    /// Read a byte range from the stream.
    //***************************************************************************
    template <typename T>
    typename etl::enable_if<sizeof(T) == 1U, etl::optional<etl::span<T> > >::type
      read(size_t n)
    {
      etl::optional<etl::span<T> > result;

      // Do we have enough room?
      if (available<T>() >= n)
      {
        char* pend = pcurrent + (n * sizeof(T));

        result = etl::span<T>(reinterpret_cast<T*>(pcurrent), reinterpret_cast<T*>(pend));
        pcurrent = pend;
      }

      return result;
    }

    //***************************************************************************
    /// Read a range of T from the stream.
    //***************************************************************************
    template <typename T>
    typename etl::enable_if<etl::is_integral<T>::value || etl::is_floating_point<T>::value, etl::optional<etl::span<T> > >::type
      read(etl::span<T> range)
    {
      etl::optional<etl::span<T> > result;

      // Do we have enough room?
      if (available<T>() >= range.size())
      {
        typename etl::span<T>::iterator destination = range.begin();

        while (destination != range.end())
        {
          *destination++ = from_bytes<T>();
        }

        result = range;
      }

      return result;
    }

    //***************************************************************************
    /// Skip n items of T, up to the maximum space available.
    //***************************************************************************
    template <typename T>
    bool skip(size_t n)
    {
      size_t maximum = available<T>();

      if (n < maximum)
      {
        pcurrent += (n * sizeof(T));
        return true;
      }
      else
      {
        pcurrent += (maximum * sizeof(T));
        return false;
      }
    }

    //***************************************************************************
    /// Returns <b>true</b> if the byte stream is empty.
    //***************************************************************************
    bool empty() const
    {
      return available<char>() == 0U;
    }

    //***************************************************************************
    /// Returns the number of bytes used in the stream.
    //***************************************************************************
    size_t size_bytes() const
    {
      return length;
    }

    //***************************************************************************
    /// The number of T left in the stream.
    //***************************************************************************
    template <typename T>
    size_t available() const
    {
      return (length - etl::distance(pdata, reinterpret_cast<char* const>(pcurrent))) / sizeof(T);
    }

  private:

    //***************************************************************************
    /// from_bytes
    //***************************************************************************
    template <typename T>
    typename etl::enable_if<sizeof(T) == 1U, T>::type
      from_bytes()
    {
      return static_cast<T>(*pcurrent++);
    }

    //*********************************
    template <typename T>
    typename etl::enable_if<sizeof(T) != 1U, T>::type
      from_bytes()
    {
      T value;

      char* pv = reinterpret_cast<char*>(&value);
      copy_value(pcurrent, pv, sizeof(T));
      pcurrent += sizeof(T);

      return value;
    }
  };

  //***************************************************************************
  /// Default implementation of the write function.
  /// Overload this to support custom types.
  //***************************************************************************
  template <typename T>
  bool write(etl::byte_stream_writer& stream, const T& value)
  {
    return stream.write(value);
  }

  //***************************************************************************
  /// Default implementation of the write function.
  /// Overload this to support custom types.
  //***************************************************************************
  template <typename T>
  bool write(etl::byte_stream_writer& stream, const etl::span<T>& range)
  {
    return stream.write(range);
  }

  //***************************************************************************
  /// Default implementation of the read function.
  /// Overload this to support custom types.
  //***************************************************************************
  template <typename T>
  etl::optional<T> read(etl::byte_stream_reader& stream)
  {
    return stream.read<T>();
  }

  //***************************************************************************
  /// Default implementation of the read function.
  /// Overload this to support custom types.
  //***************************************************************************
  template <typename T>
  etl::optional<etl::span<T> > read(etl::byte_stream_reader& stream, size_t n)
  {
    return stream.read<T>(n);
  }

  //***************************************************************************
  /// Default implementation of the read function.
  /// Overload this to support custom types.
  //***************************************************************************
  template <typename T>
  etl::optional<etl::span<T> > read(etl::byte_stream_reader& stream, etl::span<T> range)
  {
    return stream.read<T>(range);
  }
}

#endif
