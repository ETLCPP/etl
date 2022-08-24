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

#include "etl/fnv_1.h"

namespace
{
  SUITE(test_fnv1)
  {
    //*************************************************************************
    TEST(test_fnv_1_32_constructor)
    {
      std::string data("123456789");

      uint32_t hash = etl::fnv_1_32(data.begin(), data.end());

      CHECK_EQUAL(0x24148816UL, hash);
    }

    //*************************************************************************
    TEST(test_fnv_1_32_add_values)
    {
      std::string data("123456789");

      etl::fnv_1_32 fnv_1_32_calculator;

      for (size_t i = 0UL; i < data.size(); ++i)
      {
        fnv_1_32_calculator.add(data[i]);
      }

      uint32_t hash = fnv_1_32_calculator.value();

      CHECK_EQUAL(0x24148816UL, hash);
    }

    //*************************************************************************
    TEST(test_fnv_1_32_add_range)
    {
      std::string data("123456789");

      etl::fnv_1_32 fnv_1_32_calculator;

      fnv_1_32_calculator.add(data.begin(), data.end());

      uint32_t hash = fnv_1_32_calculator.value();

      CHECK_EQUAL(0x24148816UL, hash);
    }

    //*************************************************************************
    TEST(test_fnv_1_32_add_range_via_iterator)
    {
      std::string data("123456789");

      etl::fnv_1_32 fnv_1_32_calculator;

      std::copy(data.begin(), data.end(), fnv_1_32_calculator.input());

      uint32_t hash = fnv_1_32_calculator.value();

      CHECK_EQUAL(0x24148816UL, hash);
    }

    //*************************************************************************
    TEST(test_fnv_1_32_add_range_endian)
    {
      std::vector<uint8_t>  data1 = { 0x01U, 0x02U, 0x03U, 0x04U, 0x05U, 0x06U, 0x07U, 0x08U };
      std::vector<uint32_t> data2 = { 0x04030201UL, 0x08070605UL };
      std::vector<uint8_t>  data3 = { 0x08U, 0x07U, 0x06U, 0x05U, 0x04U, 0x03U, 0x02U, 0x01U };

      uint32_t hash1 = etl::fnv_1_32(data1.begin(), data1.end());
      uint32_t hash2 = etl::fnv_1_32((uint8_t*)&data2[0], (uint8_t*)(&data2[0] + data2.size()));
      CHECK_EQUAL(int(hash1), int(hash2));

      uint32_t hash3 = etl::fnv_1_32(data3.rbegin(), data3.rend());
      CHECK_EQUAL(int(hash1), int(hash3));
    }

    //*************************************************************************
    TEST(test_fnv_1a_32_constructor)
    {
      std::string data("123456789");

      uint32_t hash = etl::fnv_1a_32(data.begin(), data.end());

      CHECK_EQUAL(0xBB86B11CUL, hash);
    }

    //*************************************************************************
    TEST(test_fnv_1a_32_add_values)
    {
      std::string data("123456789");

      etl::fnv_1a_32 fnv_1a_32_calculator;

      for (size_t i = 0UL; i < data.size(); ++i)
      {
        fnv_1a_32_calculator.add(data[i]);
      }

      uint32_t hash = fnv_1a_32_calculator.value();

      CHECK_EQUAL(0xBB86B11CUL, hash);
    }

    //*************************************************************************
    TEST(test_fnv_1a_32_add_range)
    {
      std::string data("123456789");

      etl::fnv_1a_32 fnv_1a_32_calculator;

      fnv_1a_32_calculator.add(data.begin(), data.end());

      uint32_t hash = fnv_1a_32_calculator.value();

      CHECK_EQUAL(0xBB86B11CUL, hash);
    }

    //*************************************************************************
    TEST(test_fnv_1a_32_add_range_via_iterator)
    {
      std::string data("123456789");

      etl::fnv_1a_32 fnv_1a_32_calculator;

      std::copy(data.begin(), data.end(), fnv_1a_32_calculator.input());

      uint32_t hash = fnv_1a_32_calculator.value();

      CHECK_EQUAL(0xBB86B11CUL, hash);
    }

    //*************************************************************************
    TEST(test_fnv_1a_32_add_range_endian)
    {
      std::vector<uint8_t>  data1 = { 0x01U, 0x02U, 0x03U, 0x04U, 0x05U, 0x06U, 0x07U, 0x08U };
      std::vector<uint32_t> data2 = { 0x04030201UL, 0x08070605UL };
      std::vector<uint8_t>  data3 = { 0x08U, 0x07U, 0x06U, 0x05U, 0x04U, 0x03U, 0x02U, 0x01U };

      uint32_t hash1 = etl::fnv_1a_32(data1.begin(), data1.end());
      uint32_t hash2 = etl::fnv_1a_32((uint8_t*)&data2[0], (uint8_t*)(&data2[0] + data2.size()));
      CHECK_EQUAL(int(hash1), int(hash2));

      uint32_t hash3 = etl::fnv_1a_32(data3.rbegin(), data3.rend());
      CHECK_EQUAL(int(hash1), int(hash3));
    }

    //*************************************************************************
    TEST(test_fnv_1_64_constructor)
    {
      std::string data("123456789");

      uint64_t hash = etl::fnv_1_64(data.begin(), data.end());

      CHECK_EQUAL(0xA72FFC362BF916D6ULL, hash);
    }

    //*************************************************************************
    TEST(test_fnv_1_64_add_values)
    {
      std::string data("123456789");

      etl::fnv_1_64 fnv_1_64_calculator;

      for (size_t i = 0UL; i < data.size(); ++i)
      {
        fnv_1_64_calculator.add(data[i]);
      }

      uint64_t hash = fnv_1_64_calculator;

      CHECK_EQUAL(0xA72FFC362BF916D6ULL, hash);
    }

    //*************************************************************************
    TEST(test_fnv_1_64_add_range)
    {
      std::string data("123456789");

      etl::fnv_1_64 fnv_1_64_calculator;

      fnv_1_64_calculator.add(data.begin(), data.end());

      uint64_t hash = fnv_1_64_calculator.value();

      CHECK_EQUAL(0xA72FFC362BF916D6ULL, hash);
    }

    //*************************************************************************
    TEST(test_fnv_1_64_add_range_via_iterator)
    {
      std::string data("123456789");

      etl::fnv_1_64 fnv_1_64_calculator;

      std::copy(data.begin(), data.end(), fnv_1_64_calculator.input());

      uint64_t hash = fnv_1_64_calculator.value();

      CHECK_EQUAL(0xA72FFC362BF916D6ULL, hash);
    }

    //*************************************************************************
    TEST(test_fnv_1_64_add_range_endian)
    {
      std::vector<uint8_t>  data1 = { 0x01U, 0x02U, 0x03U, 0x04U, 0x05U, 0x06U, 0x07U, 0x08U };
      std::vector<uint32_t> data2 = { 0x04030201UL, 0x08070605UL };
      std::vector<uint8_t>  data3 = { 0x08U, 0x07U, 0x06U, 0x05U, 0x04U, 0x03U, 0x02U, 0x01U };

      uint64_t hash1 = etl::fnv_1_64(data1.begin(), data1.end());
      uint64_t hash2 = etl::fnv_1_64((uint8_t*)&data2[0], (uint8_t*)(&data2[0] + data2.size()));
      CHECK_EQUAL(int(hash1), int(hash2));

      uint64_t hash3 = etl::fnv_1_64(data3.rbegin(), data3.rend());
      CHECK_EQUAL(int(hash1), int(hash3));
    }

    //*************************************************************************
    TEST(test_fnv_1a_64_constructor)
    {
      std::string data("123456789");

      uint64_t hash = etl::fnv_1a_64(data.begin(), data.end());

      CHECK_EQUAL(0x06D5573923C6CDFCULL, hash);
    }

    //*************************************************************************
    TEST(test_fnv_1a_64_add_values)
    {
      std::string data("123456789");

      etl::fnv_1a_64 fnv_1a_64_calculator;

      for (size_t i = 0UL; i < data.size(); ++i)
      {
        fnv_1a_64_calculator.add(data[i]);
      }

      uint64_t hash = fnv_1a_64_calculator;

      CHECK_EQUAL(0x06D5573923C6CDFCULL, hash);
    }

    //*************************************************************************
    TEST(test_fnv_1a_64_add_range)
    {
      std::string data("123456789");

      etl::fnv_1a_64 fnv_1a_64_calculator;

      fnv_1a_64_calculator.add(data.begin(), data.end());

      uint64_t hash = fnv_1a_64_calculator.value();

      CHECK_EQUAL(0x06D5573923C6CDFCULL, hash);
    }

    //*************************************************************************
    TEST(test_fnv_1a_64_add_range_via_iterator)
    {
      std::string data("123456789");

      etl::fnv_1a_64 fnv_1a_64_calculator;

      std::copy(data.begin(), data.end(), fnv_1a_64_calculator.input());

      uint64_t hash = fnv_1a_64_calculator.value();

      CHECK_EQUAL(0x06D5573923C6CDFCULL, hash);
    }

    //*************************************************************************
    TEST(test_fnv_1a_64_add_range_endian)
    {
      std::vector<uint8_t>  data1 = { 0x01U, 0x02U, 0x03U, 0x04U, 0x05U, 0x06U, 0x07U, 0x08U };
      std::vector<uint32_t> data2 = { 0x04030201UL, 0x08070605UL };
      std::vector<uint8_t>  data3 = { 0x08U, 0x07U, 0x06U, 0x05U, 0x04U, 0x03U, 0x02U, 0x01U };

      uint64_t hash1 = etl::fnv_1a_64(data1.begin(), data1.end());
      uint64_t hash2 = etl::fnv_1a_64((uint8_t*)&data2[0], (uint8_t*)(&data2[0] + data2.size()));
      CHECK_EQUAL(int(hash1), int(hash2));

      uint64_t hash3 = etl::fnv_1a_64(data3.rbegin(), data3.rend());
      CHECK_EQUAL(int(hash1), int(hash3));
    }
  };
}

