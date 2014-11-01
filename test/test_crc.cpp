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

#include <UnitTest++/UnitTest++.h>

#include <iterator>
#include <cstdint>
#include "../crc8.h"
#include "../crc16.h"
#include "../crc16_ccitt.h"
#include "../crc16_kermit.h"
#include "../crc32.h"
#include "../crc64_ecma.h"

namespace
{		
  SUITE(TestCRC)
  {
    //*************************************************************************
    TEST(TestCRC8)
    {
      unsigned char data[] = { '1', '2', '3', '4', '5', '6', '7', '8', '9' };

      uint8_t crc = etl::crc8(std::begin(data), std::end(data));

      CHECK_EQUAL(0xF4, crc);
    }

    //*************************************************************************
    TEST(TestCRC16)
    {
      unsigned char data[] = { '1', '2', '3', '4', '5', '6', '7', '8', '9' };

      uint16_t crc = etl::crc16(std::begin(data), std::end(data));

      CHECK_EQUAL(0xBB3D, crc);
    }

    //*************************************************************************
    TEST(TestCRC16CCITT)
    {
      unsigned char data[] = { '1', '2', '3', '4', '5', '6', '7', '8', '9' };

      uint16_t crc = etl::crc16_ccitt(std::begin(data), std::end(data));

      CHECK_EQUAL(0x29B1, crc);
    }

    //*************************************************************************
    TEST(TestCRC16Kermit)
    {
      unsigned char data[] = { '1', '2', '3', '4', '5', '6', '7', '8', '9' };

      uint16_t crc = etl::crc16_kermit(std::begin(data), std::end(data));

      CHECK_EQUAL(0x2189, crc);
    }

    //*************************************************************************
    TEST(TestCRC32)
    {
      unsigned char data[] = { '1', '2', '3', '4', '5', '6', '7', '8', '9' };

      uint32_t crc = etl::crc32(std::begin(data), std::end(data));

      CHECK_EQUAL(0xCBF43926, crc);
    }

    //*************************************************************************
    TEST(TestCRC64ECMA)
    {
      unsigned char data[] = { '1', '2', '3', '4', '5', '6', '7', '8', '9' };

      uint64_t crc = etl::crc64_ecma(std::begin(data), std::end(data));

      CHECK_EQUAL(0x6C40DF5F0B497347, crc);
    }
  };
}

