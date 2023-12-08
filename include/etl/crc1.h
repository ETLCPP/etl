///\file

/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl
https://www.etlcpp.com

Copyright(c) 2023 John Wellbelove

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

#ifndef ETL_CRC1_INCLUDED
#define ETL_CRC1_INCLUDED

#include "platform.h"
#include "frame_check_sequence.h"
#include "binary.h"

namespace etl
{
  //***************************************************************************
  /// fnv_1 policy.
  /// Calculates FNV1.
  //***************************************************************************
  struct crc1_policy
  {
    typedef uint8_t value_type;

    enum
    {
      odd_parity  = 1,
      even_parity = 0
    };

    //*********************************
    value_type initial() const
    {
      return even_parity;
    }

    //*********************************
    uint8_t add(int parity, uint8_t value) const
    {
      return parity ^ etl::parity(value);
    }

    //*********************************
    uint8_t final(uint8_t parity) const
    {
      return parity;
    }
  };

  class crc1 : public etl::frame_check_sequence<crc1_policy>
  {
  public:

    enum
    {
      odd_parity  = crc1_policy::odd_parity,
      even_parity = crc1_policy::even_parity
    };

    //*************************************************************************
    /// Default constructor.
    //*************************************************************************
    crc1()
    {
      this->reset();
    }

    //*************************************************************************
    /// Constructor from range.
    /// \param begin Start of the range.
    /// \param end   End of the range.
    //*************************************************************************
    template<typename TIterator>
    crc1(TIterator begin, const TIterator end)
    {
      this->reset();
      this->add(begin, end);
    }
  };
}

#endif
