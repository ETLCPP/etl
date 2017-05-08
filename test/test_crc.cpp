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

#include "UnitTest++.h"

#include <iterator>
#include <string>
#include <vector>
#include <stdint.h>

#include "crc8_ccitt.h"
#include "crc16.h"
#include "crc16_ccitt.h"
#include "crc16_kermit.h"
#include "crc32.h"
#include "crc64_ecma.h"

namespace
{
  SUITE(test_crc)
  {
    //*************************************************************************
    TEST(test_crc8_ccitt_constructor)
    {
      std::string data("123456789");

      uint8_t crc = etl::crc8_ccitt(data.begin(), data.end());

      CHECK_EQUAL(0xF4, int(crc));
    }

    //*************************************************************************
    TEST(test_crc8_ccitt_add_values)
    {
      std::string data("123456789");

      etl::crc8_ccitt crc_calculator;

      for (size_t i = 0; i < data.size(); ++i)
      {
        crc_calculator.add(data[i]);
      }

      uint8_t crc = crc_calculator;

      CHECK_EQUAL(0xF4, crc);
    }

    //*************************************************************************
    TEST(test_crc8_ccitt_add_range)
    {
      std::string data("123456789");

      etl::crc8_ccitt crc_calculator;

      crc_calculator.add(data.begin(), data.end());

      uint8_t crc = crc_calculator.value();

      CHECK_EQUAL(0xF4, crc);
    }

    //*************************************************************************
    TEST(test_crc8_ccitt_add_range_endian)
    {
      std::vector<uint8_t>  data1 = { 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08 };
      std::vector<uint32_t> data2 = { 0x04030201, 0x08070605 };
      std::vector<uint8_t>  data3 = { 0x08, 0x07, 0x06, 0x05, 0x04, 0x03, 0x02, 0x01 };

      uint8_t crc1 = etl::crc32(data1.begin(), data1.end());
      uint8_t crc2 = etl::crc32((uint8_t*)&data2[0], (uint8_t*)(&data2[0] + data2.size()));
      CHECK_EQUAL(int(crc1), int(crc2));

      uint8_t crc3 = etl::crc32(data3.rbegin(), data3.rend());
      CHECK_EQUAL(int(crc1), int(crc3));
    }

    //*************************************************************************
    TEST(test_crc16)
    {
      std::string data("123456789");

      uint16_t crc = etl::crc16(data.begin(), data.end());

      CHECK_EQUAL(0xBB3D, crc);
    }

    //*************************************************************************
    TEST(test_crc16_add_values)
    {
      std::string data("123456789");

      etl::crc16 crc_calculator;

      for (size_t i = 0; i < data.size(); ++i)
      {
        crc_calculator.add(data[i]);
      }

      uint16_t crc = crc_calculator;

      CHECK_EQUAL(0xBB3D, crc);
    }

    //*************************************************************************
    TEST(test_crc16_add_range)
    {
      std::string data("123456789");

      etl::crc16 crc_calculator;

      crc_calculator.add(data.begin(), data.end());

      uint16_t crc = crc_calculator.value();

      CHECK_EQUAL(0xBB3D, crc);
    }

    //*************************************************************************
    TEST(test_crc16_add_range_endian)
    {
      std::vector<uint8_t>  data1 = { 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08 };
      std::vector<uint32_t> data2 = { 0x04030201, 0x08070605 };
      std::vector<uint8_t>  data3 = { 0x08, 0x07, 0x06, 0x05, 0x04, 0x03, 0x02, 0x01 };

      uint16_t crc1 = etl::crc16(data1.begin(), data1.end());
      uint16_t crc2 = etl::crc16((uint8_t*)&data2[0], (uint8_t*)(&data2[0] + data2.size()));
      CHECK_EQUAL(crc1, crc2);

      uint16_t crc3 = etl::crc16(data3.rbegin(), data3.rend());
      CHECK_EQUAL(crc1, crc3);
    }

    //*************************************************************************
    TEST(test_crc16_ccitt)
    {
      std::string data("123456789");

      uint16_t crc = etl::crc16_ccitt(data.begin(), data.end());

      CHECK_EQUAL(0x29B1, crc);
    }

    //*************************************************************************
    TEST(test_crc16_ccitt_add_values)
    {
      std::string data("123456789");

      etl::crc16_ccitt crc_calculator;

      for (size_t i = 0; i < data.size(); ++i)
      {
        crc_calculator.add(data[i]);
      }

      uint16_t crc = crc_calculator;

      CHECK_EQUAL(0x29B1, crc);
    }

    //*************************************************************************
    TEST(test_crc16_ccitt_add_range)
    {
      std::string data("123456789");

      etl::crc16_ccitt crc_calculator;

      crc_calculator.add(data.begin(), data.end());

      uint16_t crc = crc_calculator.value();

      CHECK_EQUAL(0x29B1, crc);
    }

    //*************************************************************************
    TEST(test_crc16_ccitt_add_range_endian)
    {
      std::vector<uint8_t>  data1 = { 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08 };
      std::vector<uint32_t> data2 = { 0x04030201, 0x08070605 };
      std::vector<uint8_t>  data3 = { 0x08, 0x07, 0x06, 0x05, 0x04, 0x03, 0x02, 0x01 };

      uint16_t crc1 = etl::crc16_ccitt(data1.begin(), data1.end());
      uint16_t crc2 = etl::crc16_ccitt((uint8_t*)&data2[0], (uint8_t*)(&data2[0] + data2.size()));
      CHECK_EQUAL(crc1, crc2);

      uint16_t crc3 = etl::crc16_ccitt(data3.rbegin(), data3.rend());
      CHECK_EQUAL(crc1, crc3);
    }

    //*************************************************************************
    TEST(test_crc16_kermit)
    {
      std::string data("123456789");

      uint16_t crc = etl::crc16_kermit(data.begin(), data.end());

      CHECK_EQUAL(0x2189, crc);
    }

    //*************************************************************************
    TEST(test_crc16_kermit_add_values)
    {
      std::string data("123456789");

      etl::crc16_kermit crc_calculator;

      for (size_t i = 0; i < data.size(); ++i)
      {
        crc_calculator.add(data[i]);
      }

      uint16_t crc = crc_calculator;

      CHECK_EQUAL(0x2189, crc);
    }

    //*************************************************************************
    TEST(test_crc16_kermit_add_range)
    {
      std::string data("123456789");

      etl::crc16_kermit crc_calculator;

      crc_calculator.add(data.begin(), data.end());

      uint16_t crc = crc_calculator.value();

      CHECK_EQUAL(0x2189, crc);
    }

    //*************************************************************************
    TEST(test_crc16_kermit_add_range_endian)
    {
      std::vector<uint8_t>  data1 = { 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08 };
      std::vector<uint32_t> data2 = { 0x04030201, 0x08070605 };
      std::vector<uint8_t>  data3 = { 0x08, 0x07, 0x06, 0x05, 0x04, 0x03, 0x02, 0x01 };

      uint16_t crc1 = etl::crc16_kermit(data1.begin(), data1.end());
      uint16_t crc2 = etl::crc16_kermit((uint8_t*)&data2[0], (uint8_t*)(&data2[0] + data2.size()));
      CHECK_EQUAL(crc1, crc2);

      uint16_t crc3 = etl::crc16_kermit(data3.rbegin(), data3.rend());
      CHECK_EQUAL(crc1, crc3);
    }

    //*************************************************************************
    TEST(test_crc32)
    {
      std::string data("123456789");

      uint32_t crc = etl::crc32(data.begin(), data.end());

      CHECK_EQUAL(0xCBF43926, crc);
    }

    //*************************************************************************
    TEST(test_crc32_add_values)
    {
      std::string data("123456789");

      etl::crc32 crc_calculator;

      for (size_t i = 0; i < data.size(); ++i)
      {
        crc_calculator.add(data[i]);
      }

      uint32_t crc = crc_calculator;


      CHECK_EQUAL(0xCBF43926, crc);
    }

    //*************************************************************************
    TEST(test_crc32_add_range)
    {
      std::string data("123456789");

      etl::crc32 crc_calculator;

      crc_calculator.add(data.begin(), data.end());

      uint32_t crc = crc_calculator.value();

      CHECK_EQUAL(0xCBF43926, crc);
    }

    //*************************************************************************
    TEST(test_crc32_add_range_endian)
    {
      std::vector<uint8_t>  data1 = { 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08 };
      std::vector<uint32_t> data2 = { 0x04030201, 0x08070605 };
      std::vector<uint8_t>  data3 = { 0x08, 0x07, 0x06, 0x05, 0x04, 0x03, 0x02, 0x01 };

      uint32_t crc1 = etl::crc32(data1.begin(), data1.end());
      uint32_t crc2 = etl::crc32((uint8_t*)&data2[0], (uint8_t*)(&data2[0] + data2.size()));
      CHECK_EQUAL(crc1, crc2);

      uint32_t crc3 = etl::crc32(data3.rbegin(), data3.rend());
      CHECK_EQUAL(crc1, crc3);
    }

    //*************************************************************************
    TEST(test_crc64_ecma)
    {
      std::string data("123456789");

      uint64_t crc = etl::crc64_ecma(data.begin(), data.end());

      CHECK_EQUAL(0x6C40DF5F0B497347U, crc);
    }

    //*************************************************************************
    TEST(test_crc64_ecma_add_values)
    {
      std::string data("123456789");

      etl::crc64_ecma crc_calculator;

      for (size_t i = 0; i < data.size(); ++i)
      {
        crc_calculator.add(data[i]);
      }

      uint64_t crc = crc_calculator;

      CHECK_EQUAL(0x6C40DF5F0B497347U, crc);
    }

    //*************************************************************************
    TEST(test_crc64_ecma_add_range)
    {
      std::string data("123456789");

      etl::crc64_ecma crc_calculator;

      crc_calculator.add(data.begin(), data.end());

      uint64_t crc = crc_calculator.value();

      CHECK_EQUAL(0x6C40DF5F0B497347U, crc);
    }

    //*************************************************************************
    TEST(test_crc64_ecma_add_range_endian)
    {
      std::vector<uint8_t>  data1 = { 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08 };
      std::vector<uint32_t> data2 = { 0x04030201, 0x08070605 };
      std::vector<uint8_t>  data3 = { 0x08, 0x07, 0x06, 0x05, 0x04, 0x03, 0x02, 0x01 };

      uint64_t crc1 = etl::crc64_ecma(data1.begin(), data1.end());
      uint64_t crc2 = etl::crc64_ecma((uint8_t*)&data2[0], (uint8_t*)(&data2[0] + data2.size()));
      CHECK_EQUAL(crc1, crc2);

      uint64_t crc3 = etl::crc64_ecma(data3.rbegin(), data3.rend());
      CHECK_EQUAL(crc1, crc3);
    }
  };
}

