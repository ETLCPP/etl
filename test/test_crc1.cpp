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

#include "unit_test_framework.h"

#include <iterator>
#include <string>
#include <vector>
#include <stdint.h>

#include "etl/crc1.h"

namespace
{
  //***************************************************************************
  // Calculate parity the simple way.
  //***************************************************************************
  template <typename TIterator>
  int calculate_parity(TIterator b, TIterator e)
  {
    size_t count = 0;

    while (b != e)
    {
      count += etl::count_bits(*b);
      ++b;
    }

    return ((count &= 1) == 0) ? etl::crc1::even_parity : etl::crc1::odd_parity;
  }

  SUITE(test_crc1)
  {
    //*************************************************************************
    TEST(test_crc1_constructor)
    {
      std::string data("123456789");

      uint8_t crc = etl::crc1(data.begin(), data.end());

      CHECK_EQUAL(calculate_parity(data.begin(), data.end()), int(crc));
    }

#if ETL_USING_CPP14 && !defined(ETL_CRC_FORCE_CPP03_IMPLEMENTATION)
    //*************************************************************************
    TEST(test_crc1_constructor_constexpr)
    {
      constexpr char data[] = "123456789";
      constexpr uint8_t crc = etl::crc1(data, data + 9);

      CHECK_EQUAL(calculate_parity(data, data + 9), int(crc));
    }
#endif

    //*************************************************************************
    TEST(test_crc1_add_values)
    {
      std::string data("123456789");

      etl::crc1 crc_calculator;

      for (size_t i = 0UL; i < data.size(); ++i)
      {
        crc_calculator.add(data[i]);
      }

      uint8_t crc = crc_calculator;

      CHECK_EQUAL(calculate_parity(data.begin(), data.end()), int(crc));
    }

    //*************************************************************************
    TEST(test_crc1_add_range)
    {
      std::string data("123456789");

      etl::crc1 crc_calculator;

      crc_calculator.add(data.begin(), data.end());

      uint8_t crc = crc_calculator.value();

      CHECK_EQUAL(calculate_parity(data.begin(), data.end()), int(crc));
    }

    //*************************************************************************
    TEST(test_crc1_add_range_via_iterator)
    {
      std::string data("123456789");

      etl::crc1 crc_calculator;

      std::copy(data.begin(), data.end(), crc_calculator.input());

      uint8_t crc = crc_calculator.value();

      CHECK_EQUAL(calculate_parity(data.begin(), data.end()), int(crc));
    }

    //*************************************************************************
    TEST(test_crc1_add_range_endian)
    {
      std::vector<uint8_t>  data1 = { 0x01U, 0x02U, 0x03U, 0x04U, 0x05U, 0x06U, 0x07U, 0x08U };
      std::vector<uint32_t> data2 = { 0x04030201UL, 0x08070605UL };
      std::vector<uint8_t>  data3 = { 0x08U, 0x07U, 0x06U, 0x05U, 0x04U, 0x03U, 0x02U, 0x01U };

      uint8_t crc1 = etl::crc1(data1.begin(), data1.end());
      uint8_t crc2 = etl::crc1((uint8_t*)&data2[0], (uint8_t*)(&data2[0] + data2.size()));
      CHECK_EQUAL(int(crc1), int(crc2));

      uint8_t crc3 = etl::crc1(data3.rbegin(), data3.rend());
      CHECK_EQUAL(int(crc1), int(crc3));
    }

    //*************************************************************************
    TEST(test_crc1_constants)
    {
      std::vector<uint8_t> data_odd{  etl::b00000001, etl::b00000111, etl::b01010001, etl::b10000100 };
      std::vector<uint8_t> data_even{ etl::b00000001, etl::b01000111, etl::b01010001, etl::b10000100 };

      CHECK_EQUAL(etl::crc1::odd_parity,  etl::crc1(data_odd.begin(),  data_odd.end()));
      CHECK_EQUAL(etl::crc1::even_parity, etl::crc1(data_even.begin(), data_even.end()));
    }
  };
}

