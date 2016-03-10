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

#include <UnitTest++/UnitTest++.h>

#include <iterator>
#include <string>
#include <vector>
#include <stdint.h>

#include "../src/checksum.h"

namespace
{
  template <typename TSum, typename TIterator>
  TSum reference_checksum(TIterator begin, TIterator end)
  {
    typedef typename std::iterator_traits<TIterator>::value_type value_type;
    TSum checksum = 0;

    while (begin != end)
    {
      value_type value = *begin++;

      for (size_t i = 0; i < sizeof(value_type); ++i)
      {
        uint8_t byte = (value >> (i * 8)) & 0xFF;
        checksum = etl::rotate_right(checksum) + byte;
      }
    }

    return checksum;
  }

  SUITE(test_checksum)
  {
    //*************************************************************************
    TEST(test_checksum_constructor)
    {
      std::string data("123456789");

      uint8_t sum     = etl::bsd_checksum<uint8_t>(data.begin(), data.end());
      uint8_t compare = reference_checksum<uint8_t>(data.begin(), data.end());

      CHECK_EQUAL(int(compare), int(sum));
    }

    //*************************************************************************
    TEST(test_checksum_add_values8_add)
    {
      std::string data("123456789");

      etl::bsd_checksum<uint8_t> checksum_calculator;

      for (size_t i = 0; i < data.size(); ++i)
      {
        checksum_calculator.add(data[i]);
      }

      uint8_t sum     = checksum_calculator;
      uint8_t compare = reference_checksum<uint8_t>(data.begin(), data.end());

      CHECK_EQUAL(int(compare), int(sum));
    }

    //*************************************************************************
    TEST(test_checksum_add_values8_operator_plus_equals)
    {
      std::string data("123456789");

      etl::bsd_checksum<uint8_t> checksum_calculator;

      for (size_t i = 0; i < data.size(); ++i)
      {
        checksum_calculator.add(data[i]);
      }

      uint8_t sum     = checksum_calculator;
      uint8_t compare = reference_checksum<uint8_t>(data.begin(), data.end());

      CHECK_EQUAL(int(compare), int(sum));
    }

    //*************************************************************************
    TEST(test_checksum_add_range)
    {
      std::string data("123456789");

      etl::bsd_checksum<uint8_t> checksum_calculator;

      checksum_calculator.add(data.begin(), data.end());

      uint8_t sum     = checksum_calculator.value();
      uint8_t compare = reference_checksum<uint8_t>(data.begin(), data.end());

      CHECK_EQUAL(int(compare), int(sum));
    }

    //*************************************************************************
    TEST(test_checksum_add_range_sum32)
    {
      std::string data("1");

      etl::bsd_checksum<uint32_t> checksum_calculator;

      checksum_calculator.add(data.begin(), data.end());

      uint32_t sum     = checksum_calculator.value();
      uint32_t compare = reference_checksum<uint32_t>(data.begin(), data.end());

      CHECK_EQUAL(compare, sum);
    }

    //*************************************************************************
    TEST(test_checksum_add_range_endian)
    {
      std::vector<uint8_t>  data1 = { 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08 };
      std::vector<uint32_t> data2 = { 0x04030201, 0x08070605 };
      std::vector<uint8_t>  data3 = { 0x08, 0x07, 0x06, 0x05, 0x04, 0x03, 0x02, 0x01 };

      uint64_t hash1 = etl::bsd_checksum<uint8_t>(data1.begin(), data1.end());
      uint64_t hash2 = etl::bsd_checksum<uint8_t>((uint8_t*)&data2[0], (uint8_t*)&data2[0] + (data2.size() * sizeof(uint32_t)));
      uint64_t hash3 = etl::bsd_checksum<uint8_t>(data3.rbegin(), data3.rend());
      CHECK_EQUAL(hash1, hash2);
      CHECK_EQUAL(hash1, hash3);
    }
  };
}

