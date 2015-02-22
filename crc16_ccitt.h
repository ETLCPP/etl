///\file

/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl

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

#ifndef __ETL_CRC16_CCITT__
#define __ETL_CRC16_CCITT__

#include <stdint.h>

#include "static_assert.h"
#include "type_traits.h"
#include "endian.h"
#include "ihash.h"

#if defined(COMPILER_KEIL)
#pragma diag_suppress 1300 
#endif

///\defgroup crc16_ccitt 16 bit CRC CCITT calculation
///\ingroup crc

namespace etl
{
  //***************************************************************************
  /// CRC-CCITT table
  /// \ingroup crc16_ccitt
  //***************************************************************************
  extern const uint16_t CRC_CCITT[];

  //***************************************************************************
  /// Calculates CRC-CCITT using polynomial 0x1021
  ///\tparam ENDIANNESS The endianness of the calculation for input types larger than uint8_t. Default = endian::little.
  /// \ingroup crc16_ccitt
  //***************************************************************************
  template <const int ENDIANNESS = endian::little>
  class crc16_ccitt : public etl::ihash
  {
  public:

    typedef uint16_t value_type;
    typedef uint16_t argument_type;

    //*************************************************************************
    /// Default constructor.
    //*************************************************************************
    crc16_ccitt()
      : ihash(etl::endian(ENDIANNESS))
    {
      reset();
    }

    //*************************************************************************
    /// Constructor from range.
    /// \param begin Start of the range.
    /// \param end   End of the range.
    //*************************************************************************
    template<typename TIterator>
    crc16_ccitt(TIterator begin, const TIterator end)
      : ihash(etl::endian(ENDIANNESS))
    {
      reset();
      add(begin, end);
    }

    //*************************************************************************
    /// Resets the CRC to the initial state.
    //*************************************************************************
    void reset()
    {
      crc = 0xFFFF;
    }

    //*************************************************************************
    /// Adds a range.
    /// \param begin
    /// \param end
    //*************************************************************************
    template<typename TIterator>
    void add(TIterator begin, const TIterator end)
    {
      ihash::add(begin, end);
    }

    //*************************************************************************
    /// Adds a value.
    /// \param value The value to add to the checksum.
    //*************************************************************************
    template<typename TValue>
    void add(TValue value)
    {
      ihash::add(value);
    }

    //*************************************************************************
    /// \param value The uint8_t to add to the CRC.
    //*************************************************************************
    void add(uint8_t value)
    {
      crc = (crc << 8) ^ CRC_CCITT[((crc >> 8) ^ value) & 0xFF];
    }

    //*************************************************************************
    /// Gets the CRC value.
    //*************************************************************************
    value_type value() const
    {
      return crc;
    }

    //*************************************************************************
    /// Conversion operator to value_type.
    //*************************************************************************
    operator value_type () const
    {
      return crc;
    }

    //*************************************************************************
    /// Gets the generic digest value.
    //*************************************************************************
    generic_digest digest() const
    {
      return ihash::get_digest(crc);
    }

  private:

    value_type crc;
  };
}

#endif
