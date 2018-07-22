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

#ifndef ETL_CRC32_C_INCLUDED
#define ETL_CRC32_C_INCLUDED

#include <stdint.h>

#include "platform.h"
#include "frame_check_sequence.h"

#include "stl/iterator.h"

#if defined(ETL_COMPILER_KEIL)
#pragma diag_suppress 1300
#endif

///\defgroup crc32_c 32 bit CRC-C calculation (Castagnoli)
///\ingroup crc

namespace etl
{
  //***************************************************************************
  /// CRC32_C table
  /// \ingroup crc32_c
  //***************************************************************************
  extern const uint32_t CRC32_C[];

  //***************************************************************************
  /// CRC32_C policy.
  /// Calculates CRC32_C using polynomial 0x1EDC6F41.
  //***************************************************************************
  struct crc_policy_32_c
  {
    typedef uint32_t value_type;

    inline uint32_t initial() const
    {
      return 0xFFFFFFFF;
    }

    inline uint32_t add(uint32_t crc, uint8_t value) const
    {
      return  (crc >> 8) ^ CRC32_C[(crc ^ value) & 0xFF];
    }

    inline uint32_t final(uint32_t crc) const
    {
      return crc ^ 0xFFFFFFFF;
    }
  };

  //*************************************************************************
  /// CRC32_C
  //*************************************************************************
  class crc32_c : public etl::frame_check_sequence<etl::crc_policy_32_c>
  {
  public:

    //*************************************************************************
    /// Default constructor.
    //*************************************************************************
    crc32_c()
    {
      this->reset();
    }

    //*************************************************************************
    /// Constructor from range.
    /// \param begin Start of the range.
    /// \param end   End of the range.
    //*************************************************************************
    template<typename TIterator>
    crc32_c(TIterator begin, const TIterator end)
    {
      this->reset();
      this->add(begin, end);
    }
  };
}

#endif
