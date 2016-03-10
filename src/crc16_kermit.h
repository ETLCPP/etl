///\file

/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl
http://www.etlcpp.com

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

#ifndef __ETL_CRC16_KERMIT__
#define __ETL_CRC16_KERMIT__

#include <stdint.h>
#include <iterator>

#include "platform.h"
#include "frame_check_sequence.h"

#if defined(ETL_COMPILER_KEIL)
#pragma diag_suppress 1300
#endif

///\defgroup crc16_kermit 16 bit CRC Kermit calculation
///\ingroup crc

namespace etl
{
  //***************************************************************************
  /// CRC Kermit table
  /// \ingroup crc
  //***************************************************************************
  extern const uint16_t CRC_KERMIT[];

  //***************************************************************************
  /// CRC16 Kermit policy.
  /// Calculates CRC16 Kermit using polynomial 0x1021
  //***************************************************************************
  struct crc_policy_16_kermit
  {
    typedef uint16_t value_type;

    inline uint16_t initial() const
    {
      return 0;
    }

    inline uint16_t add(uint16_t crc, uint8_t value) const
    {
      return (crc >> 8) ^ CRC_KERMIT[(crc ^ value) & 0xFF];
    }

    inline uint16_t final(uint16_t crc) const
    {
      return crc;
    }
  };

  //*************************************************************************
  /// CRC16 Kermit
  //*************************************************************************
  class crc16_kermit : public etl::frame_check_sequence<etl::crc_policy_16_kermit>
  {
  public:

    //*************************************************************************
    /// Default constructor.
    //*************************************************************************
    crc16_kermit()
    {
      this->reset();
    }

    //*************************************************************************
    /// Constructor from range.
    /// \param begin Start of the range.
    /// \param end   End of the range.
    //*************************************************************************
    template<typename TIterator>
    crc16_kermit(TIterator begin, const TIterator end)
    {
      this->reset();
      this->add(begin, end);
    }
  };
}

#endif
