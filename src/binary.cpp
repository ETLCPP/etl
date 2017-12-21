///\file

/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl
http://www.etlcpp.com

Copyright(c) 2017 jwellbelove

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

#include "platform.h"
#include "binary.h"

namespace etl
{
#if ETL_8BIT_SUPPORT
  //***************************************************************************
  /// Reverse 8 bits.
  //***************************************************************************
  uint8_t reverse_bits(uint8_t value)
  {
    value = ((value & 0xAA) >> 1) | ((value & 0x55) << 1);
    value = ((value & 0xCC) >> 2) | ((value & 0x33) << 2);
    value = (value >> 4) | (value << 4);

    return value;
  }
#endif

  //***************************************************************************
  /// Reverse 16 bits.
  //***************************************************************************
  uint16_t reverse_bits(uint16_t value)
  {
    value = ((value & 0xAAAA) >> 1) | ((value & 0x5555) << 1);
    value = ((value & 0xCCCC) >> 2) | ((value & 0x3333) << 2);
    value = ((value & 0xF0F0) >> 4) | ((value & 0x0F0F) << 4);
    value = (value >> 8) | (value << 8);

    return value;
  }
 
  //***************************************************************************
  /// Reverse 32 bits.
  //***************************************************************************
  uint32_t reverse_bits(uint32_t value)
  {
    value = ((value & 0xAAAAAAAA) >>  1) | ((value & 0x55555555) <<  1);
    value = ((value & 0xCCCCCCCC) >>  2) | ((value & 0x33333333) <<  2);
    value = ((value & 0xF0F0F0F0) >>  4) | ((value & 0x0F0F0F0F) <<  4);
    value = ((value & 0xFF00FF00) >>  8) | ((value & 0x00FF00FF) <<  8);
    value = (value >> 16) | (value << 16);

    return value;
  }

  //***************************************************************************
  /// Reverse 64 bits.
  //***************************************************************************
  uint64_t reverse_bits(uint64_t value)
  {
    value = ((value & 0xAAAAAAAAAAAAAAAA) >>  1) | ((value & 0x5555555555555555) <<  1);
    value = ((value & 0xCCCCCCCCCCCCCCCC) >>  2) | ((value & 0x3333333333333333) <<  2);
    value = ((value & 0xF0F0F0F0F0F0F0F0) >>  4) | ((value & 0x0F0F0F0F0F0F0F0F) <<  4);
    value = ((value & 0xFF00FF00FF00FF00) >>  8) | ((value & 0x00FF00FF00FF00FF) <<  8);
    value = ((value & 0xFFFF0000FFFF0000) >> 16) | ((value & 0x0000FFFF0000FFFF) << 16);
    value = (value >> 32) | (value << 32);

    return value;
  }

  //***************************************************************************
  /// Reverse bytes 16 bit.
  //***************************************************************************
  uint16_t reverse_bytes(uint16_t value)
  {
    value = (value >> 8) | (value << 8);

    return value;
  }

  //***************************************************************************
  /// Reverse bytes 32 bit.
  //***************************************************************************
  uint32_t reverse_bytes(uint32_t value)
  {
    value = ((value & 0xFF00FF00) >> 8) | ((value & 0x00FF00FF) << 8);
    value = (value >> 16) | (value << 16);

    return value;
  }
  
  //***************************************************************************
  /// Reverse bytes 64 bit.
  //***************************************************************************
  uint64_t reverse_bytes(uint64_t value)
  {
    value = ((value & 0xFF00FF00FF00FF00) >> 8)  | ((value & 0x00FF00FF00FF00FF) << 8);
    value = ((value & 0xFFFF0000FFFF0000) >> 16) | ((value & 0x0000FFFF0000FFFF) << 16);
    value = (value >> 32) | (value << 32);

    return value;
  }

#if ETL_8BIT_SUPPORT
  //***************************************************************************
  /// Converts Gray code to binary.
  //***************************************************************************
  uint8_t gray_to_binary(uint8_t value)
  {
    value ^= (value >> 4);
    value ^= (value >> 2);
    value ^= (value >> 1);

    return value;
  }
#endif

  //***************************************************************************
  /// Converts Gray code to binary.
  //***************************************************************************
  uint16_t gray_to_binary(uint16_t value)
  {
    value ^= (value >> 8);
    value ^= (value >> 4);
    value ^= (value >> 2);
    value ^= (value >> 1);

    return value;
  }

  //***************************************************************************
  /// Converts Gray code to binary.
  //***************************************************************************
  uint32_t gray_to_binary(uint32_t value)
  {
    value ^= (value >> 16);
    value ^= (value >> 8);
    value ^= (value >> 4);
    value ^= (value >> 2);
    value ^= (value >> 1);

    return value;
  }

  //***************************************************************************
  /// Converts Gray code to binary.
  //***************************************************************************
  uint64_t gray_to_binary(uint64_t value)
  {
    value ^= (value >> 32);
    value ^= (value >> 16);
    value ^= (value >> 8);
    value ^= (value >> 4);
    value ^= (value >> 2);
    value ^= (value >> 1);

    return value;
  }

#if ETL_8BIT_SUPPORT
  //***************************************************************************
  /// Count set bits. 8 bits.
  //***************************************************************************
  uint_least8_t count_bits(uint8_t value)
  {
    uint32_t count;
    static const int S[] = { 1, 2, 4 };
    static const uint8_t B[] = { 0x55, 0x33, 0x0F };

    count = value - ((value >> 1) & B[0]);
    count = ((count >> S[1]) & B[1]) + (count & B[1]);
    count = ((count >> S[2]) + count) & B[2];

    return uint_least8_t(count);
  }
#endif

  //***************************************************************************
  /// Count set bits. 16 bits.
  //***************************************************************************
  uint_least8_t count_bits(uint16_t value)
  {
    uint32_t count;
    static const int S[] = { 1, 2, 4, 8 };
    static const uint16_t B[] = { 0x5555, 0x3333, 0x0F0F, 0x00FF };

    count = value - ((value >> 1) & B[0]);
    count = ((count >> S[1]) & B[1]) + (count & B[1]);
    count = ((count >> S[2]) + count) & B[2];
    count = ((count >> S[3]) + count) & B[3];

    return count;
  }

  //***************************************************************************
  /// Count set bits. 32 bits.
  //***************************************************************************
  uint_least8_t count_bits(uint32_t value)
  {
    uint32_t count;

    value = value - ((value >> 1) & 0x55555555);
    value = (value & 0x33333333) + ((value >> 2) & 0x33333333);
    count = (((value + (value >> 4)) & 0xF0F0F0F) * 0x1010101) >> 24;

    return uint_least8_t(count);
  }

  //***************************************************************************
  /// Count set bits. 64 bits.
  //***************************************************************************
  uint_least8_t count_bits(uint64_t value)
  {
    uint64_t count;
    static const int S[] = { 1, 2, 4, 8, 16, 32 };
    static const uint64_t B[] = { 0x5555555555555555, 0x3333333333333333, 0x0F0F0F0F0F0F0F0F, 0x00FF00FF00FF00FF, 0x0000FFFF0000FFFF, 0x00000000FFFFFFFF };

    count = value - ((value >> 1) & B[0]);
    count = ((count >> S[1]) & B[1]) + (count & B[1]);
    count = ((count >> S[2]) + count) & B[2];
    count = ((count >> S[3]) + count) & B[3];
    count = ((count >> S[4]) + count) & B[4];
    count = ((count >> S[5]) + count) & B[5];

    return uint_least8_t(count);
  }

#if ETL_8BIT_SUPPORT
  //***************************************************************************
  /// Parity. 8bits. 0 = even, 1 = odd
  //***************************************************************************
  uint_least8_t parity(uint8_t value)
  {
    value ^= value >> 4;
    value &= 0x0F;
    return (0x6996 >> value) & 1;
  }
#endif

  //***************************************************************************
  /// Parity. 16bits. 0 = even, 1 = odd
  //***************************************************************************
  uint_least8_t parity(uint16_t value)
  {
    value ^= value >> 8;
    value ^= value >> 4;
    value &= 0x0F;
    return (0x6996 >> value) & 1;
  }

  //***************************************************************************
  /// Parity. 32bits. 0 = even, 1 = odd
  //***************************************************************************
  uint_least8_t parity(uint32_t value)
  {
    value ^= value >> 16;
    value ^= value >> 8;
    value ^= value >> 4;
    value &= 0x0F;
    return (0x6996 >> value) & 1;
  }

  //***************************************************************************
  /// Parity. 64bits. 0 = even, 1 = odd
  //***************************************************************************
  uint_least8_t parity(uint64_t value)
  {
    value ^= value >> 32;
    value ^= value >> 16;
    value ^= value >> 8;
    value ^= value >> 4;
    value &= 0x0F;
    return (0x69966996 >> value) & 1;
  }

#if ETL_8BIT_SUPPORT
  //***************************************************************************
  /// Count trailing zeros. bit.
  /// Uses a binary search.
  //***************************************************************************
  uint_least8_t count_trailing_zeros(uint8_t value)
  {
    uint_least8_t count;

    if (value & 0x1)
    {
      count = 0;
    }
    else
    {
      count = 1;

      if ((value & 0xF) == 0)
      {
        value >>= 4;
        count += 4;
      }

      if ((value & 0x3) == 0)
      {
        value >>= 2;
        count += 2;
      }

      count -= value & 0x1;
    }

    return count;
  }
#endif

  //***************************************************************************
  /// Count trailing zeros. 16bit.
  /// Uses a binary search.
  //***************************************************************************
  uint_least8_t count_trailing_zeros(uint16_t value)
  {
    uint_least8_t count;

    if (value & 0x1)
    {
      count = 0;
    }
    else
    {
      count = 1;

      if ((value & 0xFF) == 0)
      {
        value >>= 8;
        count += 8;
      }

      if ((value & 0xF) == 0)
      {
        value >>= 4;
        count += 4;
      }

      if ((value & 0x3) == 0)
      {
        value >>= 2;
        count += 2;
      }

      count -= value & 0x1;
    }

    return count;
  }

  //***************************************************************************
  /// Count trailing zeros. 32bit.
  /// Uses a binary search.
  //***************************************************************************
  uint_least8_t count_trailing_zeros(uint32_t value)
  {
    uint_least8_t count;

    if (value & 0x1)
    {
      count = 0;
    }
    else
    {
      count = 1;

      if ((value & 0xFFFF) == 0)
      {
        value >>= 16;
        count += 16;
      }

      if ((value & 0xFF) == 0)
      {
        value >>= 8;
        count += 8;
      }

      if ((value & 0xF) == 0)
      {
        value >>= 4;
        count += 4;
      }

      if ((value & 0x3) == 0)
      {
        value >>= 2;
        count += 2;
      }

      count -= value & 0x1;
    }

    return count;
  }

  //***************************************************************************
  /// Count trailing zeros. 64bit.
  /// Uses a binary search.
  //***************************************************************************
  uint_least8_t count_trailing_zeros(uint64_t value)
  {
      uint_least8_t count;

      if (value & 0x1)
      {
        count = 0;
      }
      else
      {
        count = 1;

        if ((value & 0xFFFFFFFF) == 0)
        {
          value >>= 32;
          count += 32;
        }

        if ((value & 0xFFFF) == 0)
        {
          value >>= 16;
          count += 16;
        }

        if ((value & 0xFF) == 0)
        {
          value >>= 8;
          count += 8;
        }

        if ((value & 0xF) == 0)
        {
          value >>= 4;
          count += 4;
        }

        if ((value & 0x3) == 0)
        {
          value >>= 2;
          count += 2;
        }

        count -= value & 0x1;
      }

      return count;
  }
}
