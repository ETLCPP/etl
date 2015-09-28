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

#ifndef __ETL_CRC8_CCITT__
#define __ETL_CRC8_CCITT__

#include <stdint.h>

#include "static_assert.h"
#include "type_traits.h"

#if defined(COMPILER_KEIL)
#pragma diag_suppress 1300 
#endif

///\defgroup crc8_ccitt 8 bit CRC calculation
///\ingroup crc

namespace etl
{
  //***************************************************************************
  /// CRC8 table
  /// \ingroup crc8_ccitt
  //***************************************************************************
  extern const uint8_t CRC8_CCITT[];

  //***************************************************************************
  /// Calculates CRC8 CCITT using polynomial 0x07.
  /// \ingroup crc8_ccitt
  //***************************************************************************
  class crc8_ccitt
  {
  public:

    typedef uint8_t value_type;
    typedef uint8_t argument_type;

    //*************************************************************************
    /// Default constructor.
    //*************************************************************************
    crc8_ccitt()
    {
      reset();
    }

    //*************************************************************************
    /// Constructor from range.
    /// \param begin Start of the range.
    /// \param end   End of the range.
    //*************************************************************************
    template<typename TIterator>
    crc8_ccitt(TIterator begin, const TIterator end)
    {
      STATIC_ASSERT(sizeof(typename std::iterator_traits<TIterator>::value_type) == 1, "Type not supported");
      reset();
      while (begin != end)
      {
        crc = CRC8_CCITT[crc ^ *begin++];
      }
    }

    //*************************************************************************
    /// Resets the CRC to the initial state.
    //*************************************************************************
    void reset()
    {
      crc = 0;
    }

    //*************************************************************************
    /// Adds a range.
    /// \param begin
    /// \param end
    //*************************************************************************
    template<typename TIterator>
    void add(TIterator begin, const TIterator end)
    {
      STATIC_ASSERT(sizeof(typename std::iterator_traits<TIterator>::value_type) == 1, "Type not supported");
      while (begin != end)
      {
        crc = CRC8_CCITT[crc ^ *begin++];
      }
    }

    //*************************************************************************
    /// \param value The char to add to the CRC.
    //*************************************************************************
    void add(uint8_t value)
    {
      crc = CRC8_CCITT[crc ^ value];
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

  private:

    value_type crc;
  };
}

#endif
