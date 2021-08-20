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
    class byte_stream_base
    {
    public:

      typedef const char* const_iterator;

      //***************************************************************************
      /// Sets the index back to the beginning of the stream.
      //***************************************************************************
      void restart()
      {
        pcurrent = const_cast<char*>(pdata);
      }

      //***************************************************************************
      /// Returns <b>true</b> if the byte stream index has reached the end.
      //***************************************************************************
      bool full() const
      {
        return size() == capacity();
      }

      //***************************************************************************
      /// Returns <b>true</b> if the byte stream is empty.
      //***************************************************************************
      bool empty() const
      {
        return size() == 0U;
      }

      //***************************************************************************
      /// Returns the number of bytes used in the stream.
      //***************************************************************************
      size_t size() const
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
        return (capacity() - size()) / sizeof(T);
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

    protected:

      byte_stream_base(char* pdata_, size_t length_, etl::endian buffer_endianness_)
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
  class byte_stream_writer : public private_byte_stream::byte_stream_base
  {
  public:

    //***************************************************************************
    /// Construct from range.
    //***************************************************************************
    byte_stream_writer(char* begin_, char* end_, etl::endian buffer_endianness_ = etl::endian::big)
      : byte_stream_base(begin_, etl::distance(begin_, end_), buffer_endianness_)
    {
    }

    //***************************************************************************
    /// Construct from begin and length.
    //***************************************************************************
    byte_stream_writer(char* begin_, size_t length_, etl::endian buffer_endianness_ = etl::endian::big)
      : byte_stream_base(begin_, length_, buffer_endianness_)
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
    /// Write a range of value to the stream.
    //***************************************************************************
    template <typename T>
    typename etl::enable_if<etl::is_integral<T>::value || etl::is_floating_point<T>::value, bool>::type
      write(const etl::span<T>& range)
    {
      bool success = false;

      size_t span_length = range.size();

      if (available<T>() >= span_length)
      {
        etl::span<int8_t>::const_iterator itr = range.begin();

        while (itr != range.end())
        {
          to_bytes(*itr++);
        }

        success = true;
      }

      return success;
    }

  private:

    //***************************************************************************
    /// to_bytes
    //***************************************************************************
    template <typename T>
    typename etl::enable_if<sizeof(T) == 1U, void>::type
      to_bytes(const T value)
    {
      *pcurrent++ = static_cast<const char>(value);
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
  class byte_stream_reader : public private_byte_stream::byte_stream_base
  {
  public:

    //***************************************************************************
    /// Construct from range.
    //***************************************************************************
    byte_stream_reader(char* begin_, char* end_, etl::endian buffer_endianness_ = etl::endian::big)
      : byte_stream_base(begin_, etl::distance(begin_, end_), buffer_endianness_)
    {
    }

    //***************************************************************************
    /// Construct from begin and length.
    //***************************************************************************
    byte_stream_reader(char* begin_, size_t length_, etl::endian buffer_endianness_ = etl::endian::big)
      : byte_stream_base(begin_, length_, buffer_endianness_)
    {
    }

    //***************************************************************************
    /// Read a value from the stream.
    //***************************************************************************
    template <typename T>
    typename etl::enable_if<etl::is_integral<T>::value || etl::is_floating_point<T>::value, etl::optional<T>>::type
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
    /// Read a range from the stream.
    //***************************************************************************
    template <typename T>
    typename etl::enable_if<etl::is_integral<T>::value || etl::is_floating_point<T>::value, etl::span<T>>::type
      read(size_t length)
    {
      etl::span<T> result;

      const size_t buffer_length = length * sizeof(T);

      // Do we have enough room?
      if (available<T>() >= buffer_length)
      {
        const char* pend = pcurrent + buffer_length;

        result = etl::span<T>(reinterpret_cast<const T*>(pcurrent), reinterpret_cast<const T*>(pend));
        pcurrent += buffer_length;
      }

      return result;
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
}

#endif
