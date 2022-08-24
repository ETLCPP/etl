/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl
https://www.etlcpp.com

Copyright(c) 2014 John Wellbelove

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

#include "unit_test_framework.h"

#include <iterator>
#include <string>
#include <vector>
#include <stdint.h>

#include "etl/checksum.h"

namespace
{		
  SUITE(test_checksum)
  {
    //*************************************************************************
    TEST(test_checksum_constructor)
    {
      std::string data("123456789");

      uint8_t sum = etl::checksum<uint8_t>(data.begin(), data.end());

      CHECK_EQUAL(221, int(sum));
    }

    //*************************************************************************
    TEST(test_checksum_add_values8_add)
    {
      std::string data("123456789");

      etl::checksum<uint8_t> checksum_calculator;

      for (size_t i = 0UL; i < data.size(); ++i)
      {
        checksum_calculator.add(data[i]);
      }
      
      uint8_t sum = checksum_calculator;

      CHECK_EQUAL(221, int(sum));
    }

    //*************************************************************************
    TEST(test_checksum_add_values8_operator_plus_equals)
    {
      std::string data("123456789");

      etl::checksum<uint8_t> checksum_calculator;

      for (size_t i = 0UL; i < data.size(); ++i)
      {
        checksum_calculator.add(data[i]);
      }

      uint8_t sum = checksum_calculator;

      CHECK_EQUAL(221, int(sum));
    }

    //*************************************************************************
    TEST(test_checksum_add_range)
    {
      std::string data("123456789");

      etl::checksum<uint8_t> checksum_calculator;

      checksum_calculator.add(data.begin(), data.end());

      uint8_t sum = checksum_calculator.value();

      CHECK_EQUAL(221, int(sum));
    }

    //*************************************************************************
    TEST(test_checksum_add_range_via_iterator)
    {
      std::string data("123456789");

      etl::checksum<uint8_t> checksum_calculator;

      std::copy(data.begin(), data.end(), checksum_calculator.input());

      uint8_t sum = checksum_calculator.value();

      CHECK_EQUAL(221, int(sum));
    }

    //*************************************************************************
    TEST(test_checksum_add_range_sum16)
    {
      std::string data("123456789");

      etl::checksum<uint16_t> checksum_calculator;

      checksum_calculator.add(data.begin(), data.end());

      uint16_t sum = checksum_calculator.value();

      CHECK_EQUAL(477, int(sum));
    }

    //*************************************************************************
    TEST(test_checksum_add_range_sum16_via_iterator)
    {
      std::string data("123456789");

      etl::checksum<uint16_t> checksum_calculator;

      std::copy(data.begin(), data.end(), checksum_calculator.input());

      uint16_t sum = checksum_calculator.value();

      CHECK_EQUAL(477, int(sum));
    }

    //*************************************************************************
    TEST(test_checksum_add_range_sum32)
    {
      std::string data("123456789");

      etl::checksum<uint32_t> checksum_calculator;

      checksum_calculator.add(data.begin(), data.end());

      uint32_t sum = checksum_calculator.value();

      CHECK_EQUAL(477, int(sum));
    }

    //*************************************************************************
    TEST(test_checksum_add_range_sum32_via_iterator)
    {
      std::string data("123456789");

      etl::checksum<uint32_t> checksum_calculator;

      std::copy(data.begin(), data.end(), checksum_calculator.input());

      uint32_t sum = checksum_calculator.value();

      CHECK_EQUAL(477, int(sum));
    }

    //*************************************************************************
    TEST(test_checksum_add_range_endian)
    {
      std::vector<uint8_t>  data1 = { 0x01U, 0x02U, 0x03U, 0x04U, 0x05U, 0x06U, 0x07U, 0x08U };
      std::vector<uint32_t> data2 = { 0x04030201UL, 0x08070605UL };
      std::vector<uint8_t>  data3 = { 0x08U, 0x07U, 0x06U, 0x05U, 0x04U, 0x03U, 0x02U, 0x01U };

      uint32_t hash1 = etl::checksum<uint32_t>(data1.begin(), data1.end());
      uint32_t hash2 = etl::checksum<uint32_t>((uint8_t*)&data2[0], (uint8_t*)(&data2[0] + data2.size()));
      CHECK_EQUAL(int(hash1), int(hash2));

      uint32_t hash3 = etl::checksum<uint32_t>(data3.rbegin(), data3.rend());
      CHECK_EQUAL(int(hash1), int(hash3));
    }
  };
}

