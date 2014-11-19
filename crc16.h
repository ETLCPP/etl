///\file

/******************************************************************************
The MIT License(MIT)

Embedded Template Library.

Copyright(c) 2014 jwellbelove

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

#ifndef __ETL_CRC16__
#define __ETL_CRC16__

#include <cstdint>

#include "static_assert.h"
#include "type_traits.h"
#include "endian.h"

///\defgroup crc16 Sixteen bit CRC calculation
///\ingroup crc

namespace etl
{
  //***************************************************************************
  /// CRC16 table
  /// \ingroup crc16
  //***************************************************************************
  extern const uint16_t CRC16[];

  //***************************************************************************
  /// Calculates CRC16 using polynomial 0x8005.
  /// \ingroup crc16
  //***************************************************************************
  template <const int ENDIANNESS = endian::little>
  class crc16
  {
  public:

    typedef uint16_t value_type;

    //*************************************************************************
    /// Default constructor.
    //*************************************************************************
    crc16()
    {
      reset();
    }

    //*************************************************************************
    /// Constructor from range.
    /// \param begin Start of the range.
    /// \param end   End of the range.
    /// \return The CRC result.
    //*************************************************************************
    template<typename TIterator>
    crc16(TIterator begin, const TIterator end)
    {
      reset();
      add(begin, end);
    }

    //*************************************************************************
    /// Resets the CRC to the initial state.
    //*************************************************************************
    void reset()
    {
      crc = 0;
    }

    //*************************************************************************
    /// \param value The value to add to the CRC.
    /// \return The CRC result.
    //*************************************************************************
    template<typename TValue>
    value_type add(TValue value)
    {
      static_assert(is_integral<TValue>::value, "Non-integral parameter");

      if (ENDIANNESS == endian::little)
      {
        for (int i = 0; i < sizeof(TValue); ++i)
        {
          add(uint8_t((value >> (i * 8)) & 0xFF));
        }
      }
      else
      {
        for (int i = sizeof(TValue) - 1; i >= 0; --i)
        {
          add(uint8_t((value >> (i * 8)) & 0xFF));
        }
      }

      return crc;
    }

    //*************************************************************************
    /// \param value The uint8_t to add to the CRC.
    /// \return The CRC result.
    //*************************************************************************
    value_type add(uint8_t value)
    {
      crc = (crc >> 8) ^ CRC16[(crc ^ value) & 0xFF];

      return crc;
    }

    //*************************************************************************
    /// \param begin Start of the range.
    /// \param end   End of the range.
    /// \return The CRC result.
    //*************************************************************************
    template<typename TIterator>
    value_type add(TIterator begin, const TIterator end)
    {
      while (begin != end)
      {
        add(*begin);
        ++begin;
      }

      return crc;
    }

    //*************************************************************************
    /// Gets the CRC value.
    //*************************************************************************
    value_type value() const
    {
      return crc;
    }

    //*************************************************************************
    /// \param value The value to add to the CRC.
    //*************************************************************************
    template<typename TValue>
    crc16<ENDIANNESS>& operator +=(TValue value)
    {
      add(value);

      return *this;
    }

    //*************************************************************************
    /// Conversion operator to value_type.
    //*************************************************************************
    operator value_type () const
    {
      return crc;
    }

  private:

    value_type crc;
  };
}

#endif