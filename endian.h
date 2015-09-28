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

#ifndef __ETL_ENDIAN__
#define __ETL_ENDIAN__

#include <stdint.h>

#include "enum_type.h"

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

    DECLARE_ENUM_TYPE(endian, int)
    ENUM_TYPE(little, "little")
    ENUM_TYPE(big,    "big")
    ENUM_TYPE(native, "native")
    END_ENUM_TYPE
  };

  //***************************************************************************
  /// Checks the endianness of the platform.
  ///\ingroup endian
  //***************************************************************************
  struct endianness
  {
    endianness()
      : ETL_ENDIAN_TEST(0x0011)
    {
    }

    endian operator ()() const
    {
      return endian(*this);
    }

    operator endian() const
    {
      return (*reinterpret_cast<const uint8_t*>(&ETL_ENDIAN_TEST) == 0x11) ? endian::little : endian::big;
    }

  private:

    const uint16_t ETL_ENDIAN_TEST;
  };
}

#endif
