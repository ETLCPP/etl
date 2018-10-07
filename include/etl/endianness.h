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

#ifndef ETL_ENDIAN_INCLUDED
#define ETL_ENDIAN_INCLUDED

#include <stdint.h>

#include "platform.h"
#include "enum_type.h"
#include "binary.h"

///\defgroup endian endian
/// Constants & utilities for endianess
///\ingroup utilities

namespace etl
{
  //***************************************************************************
  /// Constants to denote endianness of operations.
  ///\ingroup endian
  //***************************************************************************
  struct endian
  {
    enum enum_type
    {
      little,
      big,
      native
    };

    ETL_DECLARE_ENUM_TYPE(endian, int)
    ETL_ENUM_TYPE(little, "little")
    ETL_ENUM_TYPE(big,    "big")
    ETL_ENUM_TYPE(native, "native")
    ETL_END_ENUM_TYPE
  };

  //***************************************************************************
  /// Checks the endianness of the platform.
  ///\ingroup endian
  //***************************************************************************
  struct endianness
  {
    etl::endian operator ()() const
    {
      return etl::endian(*this);
    }

    operator etl::endian() const
    {
      return get();
    }

    static etl::endian value()
    {
      return get();
    }

  private:

    static etl::endian get()
    {
      static const union U
      {
        U()
          : ui32(0x12345678)
        {
        }

        uint32_t ui32;
        uint16_t ui16[2];
      } u;

      return (u.ui16[0] == 0x5678) ? etl::endian::little : etl::endian::big;
    }
  };

  //***************************************************************************
  inline uint8_t ntoh(const uint8_t network)
  {
    return network;
  }

  //***************************************************************************
  inline uint16_t ntoh(const uint16_t network)
  {
    if (endianness::value() == endian::little)
    {
      return etl::reverse_bytes(network);
    }
    else
    {
      return network;
    }
  }

  //***************************************************************************
  inline uint32_t ntoh(const uint32_t network)
  {
    if (endianness::value() == endian::little)
    {
      return etl::reverse_bytes(network);
    }
    else
    {
      return network;
    }
  }

  //***************************************************************************
  inline uint64_t ntoh(const uint64_t network)
  {
    if (endianness::value() == endian::little)
    {
      return etl::reverse_bytes(network);
    }
    else
    {
      return network;
    }
  }

  //***************************************************************************
  inline uint8_t hton(const uint8_t host)
  {
    return host;
  }

  //***************************************************************************
  inline uint16_t hton(const uint16_t host)
  {
    if (endianness::value() == endian::little)
    {
      return etl::reverse_bytes(host);
    }
    else
    {
      return host;
    }
  }

  //***************************************************************************
  inline uint32_t hton(const uint32_t host)
  {
    if (endianness::value() == endian::little)
    {
      return etl::reverse_bytes(host);
    }
    else
    {
      return host;
    }
  }

  //***************************************************************************
  inline uint64_t hton(const uint64_t host)
  {
    if (endianness::value() == endian::little)
    {
      return etl::reverse_bytes(host);
    }
    else
    {
      return host;
    }
  }
}

#endif
