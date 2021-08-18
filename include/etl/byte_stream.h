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

#include "platform.h"

#if ETL_USING_8BIT_TYPES
#include <stdint.h>
#include <limits.h>

#include "type_traits.h"
#include "nullptr.h"
#include "endianness.h"
#include "integral_limits.h"
#include "algorithm.h"
#include "iterator.h"
#include "memory.h"
#include "span.h"

namespace etl
{
  class byte_stream_base
  {
  public:

    typedef const int8_t* const_iterator;

    //***************************************************************************
    /// Construct from begin and length.
    //***************************************************************************
    void set_stream(int8_t* begin_, size_t length_)
    {
      pdata    = begin_;
      pcurrent = begin_;
      length   = length_;
    }

    //***************************************************************************
    /// Construct from begin and length.
    //***************************************************************************
    void set_stream(uint8_t* begin_, size_t length_)
    {
      pdata    = reinterpret_cast<int8_t*>(begin_);
      pcurrent = begin_;
      length   = length_;
    }

    //***************************************************************************
    /// Construct from range.
    //***************************************************************************
    void set_stream(int8_t* begin_, int8_t* end_)
    {
      set_stream(begin_, etl::distance(begin_, end_));
    }

    //***************************************************************************
    /// Construct from range.
    //***************************************************************************
    void set_stream(uint8_t* begin_, uint8_t* end_)
    {
      set_stream(begin_, etl::distance(begin_, end_));
    }


    //***************************************************************************
    /// Sets the index back to the beginning of the stream.
    //***************************************************************************
    void restart()
    {
      pcurrent = pdata;
    }

    //***************************************************************************
    /// Returns <b>true</b> if the byte stream index has reached the end.
    //***************************************************************************
    bool at_end() const
    {
      return (pcurrent == (pdata + length));
    }

    //***************************************************************************
    /// Returns <b>true</b> if the byte stream index has reached the end.
    //***************************************************************************
    bool full() const
    {
      return at_end();
    }

    //***************************************************************************
    /// Returns the number of bytes used in the stream.
    //***************************************************************************
    size_t size() const
    {
      return etl::distance(pdata, pcurrent);
    }

    //***************************************************************************
    /// Returns the maximum number of bytes in the stream.
    //***************************************************************************
    size_t capacity() const
    {
      return length;
    }

    //***************************************************************************
    /// The number of bytes left in the stream.
    //***************************************************************************
    size_t available() const
    {
      return capacity() - size();
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

    byte_stream_base(int8_t* pdata_, size_t length_)
      : pdata(pdata_)
      , pcurrent(pdata_)
      , length(length_)
    {
    }

    byte_stream_base(uint8_t* pdata_, size_t length_)
      : pdata(reinterpret_cast<int8_t*>(pdata_))
      , pcurrent(pdata_)
      , length(length_)
    {
    }

    void copy_value(const int8_t* sb, const int8_t* se, int8_t* db)
    {
      etl::endian platform_endianness = etl::endianness::value();

      if (network_endianness == platform_endianness)
      {
        etl::copy(sb, se, db);
      }
      else
      {
        etl::reverse_copy(sb, se, db);
      }
    }

    const int8_t*     pdata;    ///< The start of the byte stream buffer.
    mutable int8_t*   pcurrent; ///< The current position in the byte stream buffer.
    const size_t      length;   ///< The length of the byte stream buffer.
    const etl::endian network_endianness;
  };

  //***************************************************************************
  /// Encodes a byte stream.
  //***************************************************************************
  class byte_stream_writer : public byte_stream_base
  {
  public:

    //***************************************************************************
    /// Default constructor.
    //***************************************************************************
    byte_stream_writer()
      : byte_stream_base()
    {
    }

    //***************************************************************************
    /// Construct from range.
    //***************************************************************************
    byte_stream_writer(int8_t* begin_, int8_t* end_)
      : byte_stream_base(begin_, etl::distance(begin_, end_))
    {
    }

    //***************************************************************************
    /// Construct from range.
    //***************************************************************************
    byte_stream_writer(uint8_t* begin_, uint8_t* end_)
      : byte_stream_base(begin_), etl::distance(begin_, end_))
    {
    }

    //***************************************************************************
    /// Construct from begin and length.
    //***************************************************************************
    byte_stream_writer(int8_t* begin_, size_t length_)
      : byte_stream_base(begin_), length_)
    {
    }

    //***************************************************************************
    /// Construct from begin and length.
    //***************************************************************************
    byte_stream_writer(uint8_t* begin_, size_t length_)
      : byte_stream_base(begin_), length_)
    {
    }

    //***************************************************************************
    /// Writes a boolean to the stream
    //***************************************************************************
    bool write(bool value)
    {
      bool success = false;

      if (available() > 0)
      {
        *pdata++ = value ? 1 : 0;
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

      if (available() >= sizeof(T))
      {
        to_bytes(value, pdata);
        pdata += sizeof(T);
        success = true;
      }

      return success;
    }

    //***************************************************************************
    /// Write a range of value to the stream.
    //***************************************************************************
    template <typename T>
    typename etl::enable_if<etl::is_integral<T>::value || etl::is_floating_point<T>::value, bool>::type
    bool write(const etl::span<T>& range)
    {
      bool success = false;

      size_t length = range.size_bytes();

      if (available() >= length)
      {
        etl::span<int8_t>::const_iterator itr = range.begin();

        while (itr != range.end())
        {
          to_bytes(*itr++);
        }
      }

      return success;
    }

  private:

    //***************************************************************************
    /// to_bytes
    //***************************************************************************
    void to_bytes(int8_t value)
    {
      *pcurrent++ = value;
    }

    void to_bytes(uint8_t value)
    {
      *pcurrent++ = value;
    }

    template <typename T>
    void to_bytes(T value)
    {
      const int8_t* pv = reinterpret_cast<int8_t*>(&value);

      copy_value(pv, pv + sizeof(T), pcurrent);

      pcurrent += sizeof(T);
    }
  };

  //***************************************************************************
  /// Decodes byte streams.
  /// Data must be stored in the stream in network order.
  //***************************************************************************
  class byte_stream_reader : public byte_stream_base
  {
  public:

    //***************************************************************************
    /// Construct from range.
    //***************************************************************************
    byte_stream_reader(int8_t* begin_, int8_t* end_)
      : byte_stream_base(begin_, etl::distance(begin_, end_))
    {
    }

    //***************************************************************************
    /// Construct from range.
    //***************************************************************************
    byte_stream_reader(uint8_t* begin_, uint8_t* end_)
      : byte_stream_base(begin_), etl::distance(begin_, end_))
    {
    }

    //***************************************************************************
    /// Construct from begin and length.
    //***************************************************************************
    byte_stream_reader(int8_t* begin_, size_t length_)
      : byte_stream_base(begin_), length_)
    {
    }

    //***************************************************************************
    /// Construct from begin and length.
    //***************************************************************************
    byte_stream_reader(uint8_t* begin_, size_t length_)
      : byte_stream_base(begin_), length_)
    {
    }

    //***************************************************************************
    /// Read a value from the stream.
    //***************************************************************************
    template <typename T>
    typename etl::enable_if<etl::is_bool<T>::value || etl::is_integral<T>::value || etl::is_floating_point<T>::value, etl::optional<T>>::type
      read() const
    {
      etl::optional<T> result;

      // Do we have enough bytes?
      if (available() >= size_t(T))
      {
        result = from_bytes<T>();
      }

      return result;
    }

    //***************************************************************************
    /// Read a range from the stream.
    //***************************************************************************
    template <typename T>
    typename etl::enable_if<etl::is_bool<T>::value || etl::is_integral<T>::value || etl::is_floating_point<T>::value, etl::span<T>>::type
    etl::span<T> read(size_t length) const
    {
      etl::span<T> result;

      if (available() >= (length * sizeof(T)))
      {
        const int8_t* pend = pcurrent + (length * sizeof(T));

        result = etl::span<T>(reinterpret_cast<const T*>(pcurrent), reinterpret_cast<const T*>(pend));
        pcurrent += length * sizeof(T);
      }

      return result;
    }

  private:

    //***************************************************************************
    /// from_bytes
    //***************************************************************************
    template <typename T>
    T from_bytes() const
    {
      T value;

      int8_t* pv = reinterpret_cast<int8_t*>(&value);

      if ETL_IF_CONSTEXPR(sizeof(T) == 1U)
      {
        *pv = *pcurrent;
      }
      else
      {
        copy_value(pcurrent, pcurrent + sizeof(T), pv);
      }

      pcurrent += sizeof(T);
    }
  };
}

#endif

#endif
