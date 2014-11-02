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
#include <string>
#include <cstdint>
#include "../crc8_ccitt.h"
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
      std::string data("123456789");

      uint8_t crc = etl::crc8_ccitt(data.begin(), data.end());

      CHECK_EQUAL(0xF4, crc);
    }

    //*************************************************************************
    TEST(TestCRC16)
    {
      std::string data("123456789");

      uint16_t crc = etl::crc16(data.begin(), data.end());

      CHECK_EQUAL(0xBB3D, crc);
    }

    //*************************************************************************
    TEST(TestCRC16CCITT)
    {
      std::string data("123456789");

      uint16_t crc = etl::crc16_ccitt(data.begin(), data.end());

      CHECK_EQUAL(0x29B1, crc);
    }

    //*************************************************************************
    TEST(TestCRC16Kermit)
    {
      std::string data("123456789");

      uint16_t crc = etl::crc16_kermit(data.begin(), data.end());

      CHECK_EQUAL(0x2189, crc);
    }

    //*************************************************************************
    TEST(TestCRC32)
    {
      std::string data("123456789");

      uint32_t crc = etl::crc32(data.begin(), data.end());

      CHECK_EQUAL(0xCBF43926, crc);
    }

    //*************************************************************************
    TEST(TestCRC64ECMA)
    {
      std::string data("123456789");

      uint64_t crc = etl::crc64_ecma(data.begin(), data.end());

      CHECK_EQUAL(0x6C40DF5F0B497347, crc);
    }
  };
}

