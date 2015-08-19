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

#include <UnitTest++/UnitTest++.h>

#include <iterator>
#include <string>
#include <vector>
#include <stdint.h>

#include "../bsd_checksum.h"
#include "../endian.h"

template <typename TSum, const int ENDIAN, typename TIterator>
TSum reference_checksum(TIterator begin, TIterator end)
{
  typedef typename std::iterator_traits<TIterator>::value_type value_type;
    TSum checksum = 0;

    while (begin != end)
    {
      value_type value = *begin++;

      if (ENDIAN == etl::endian::little)
      {
        for (int i = 0; i < sizeof(value_type); ++i)
        {
          uint8_t byte = (value >> (i * 8)) & 0xFF;
          checksum = etl::rotate_right(checksum) + byte;
        }
      }
      else
      {
        for (int i = 0; i < sizeof(value_type); ++i)
        {
          uint8_t byte = (value >> ((sizeof(value_type) - 1 - i) * 8)) & 0xFF;
          checksum = etl::rotate_right(checksum) + byte;
        }
      }
    }

    return checksum;
}

namespace
{		
  SUITE(test_checksum)
  {
    //*************************************************************************
    TEST(test_checksum_constructor)
    {
      std::string data("123456789");

      uint8_t sum     = etl::bsd_checksum<uint8_t>(data.begin(), data.end());
      uint8_t compare = reference_checksum<uint8_t, etl::endian::little>(data.begin(), data.end());

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
      uint8_t compare = reference_checksum<uint8_t, etl::endian::little>(data.begin(), data.end());

      CHECK_EQUAL(int(compare), int(sum));
    }

    //*************************************************************************
    TEST(test_checksum_add_values8_operator_plus_equals)
    {
      std::string data("123456789");

      etl::bsd_checksum<uint8_t> checksum_calculator;

      for (size_t i = 0; i < data.size(); ++i)
      {
        checksum_calculator += data[i];
      }

      uint8_t sum     = checksum_calculator;
      uint8_t compare = reference_checksum<uint8_t, etl::endian::little>(data.begin(), data.end());

      CHECK_EQUAL(int(compare), int(sum));
    }

    //*************************************************************************
    TEST(test_checksum_add_values8_operator_stream)
    {
      std::string data("123456789");

      etl::bsd_checksum<uint8_t> checksum_calculator;

      for (size_t i = 0; i < data.size(); ++i)
      {
        checksum_calculator << data[i];
      }

      uint8_t sum     = checksum_calculator;
      uint8_t compare = reference_checksum<uint8_t, etl::endian::little>(data.begin(), data.end());

      CHECK_EQUAL(int(compare), int(sum));
    }

    //*************************************************************************
    TEST(test_checksum_add_values32_add)
    {
      std::vector<uint32_t> data = { 0x04030201, 0x08070605 };

      etl::bsd_checksum<uint32_t> checksum_calculator;

      for (size_t i = 0; i < data.size(); ++i)
      {
        checksum_calculator += data[i];
      }

      uint32_t sum     = checksum_calculator;
      uint32_t compare = reference_checksum<uint32_t, etl::endian::little>(data.begin(), data.end());

      CHECK_EQUAL(compare, sum);
    }

    //*************************************************************************
    TEST(test_checksum_add_values32_plus_equals)
    {
      std::vector<uint32_t> data = { 0x04030201, 0x08070605 };

      etl::bsd_checksum<uint32_t> checksum_calculator;

      for (size_t i = 0; i < data.size(); ++i)
      {
        checksum_calculator.add(data[i]);
      }

      uint32_t sum     = checksum_calculator;
      uint32_t compare = reference_checksum<uint32_t, etl::endian::little>(data.begin(), data.end());

      CHECK_EQUAL(compare, sum);
    }

    //*************************************************************************
    TEST(test_checksum_add_values32_operator_stream)
    {
      std::vector<uint32_t> data = { 0x04030201, 0x08070605 };

      etl::bsd_checksum<uint32_t> checksum_calculator;

      for (size_t i = 0; i < data.size(); ++i)
      {
        checksum_calculator << data[i];
      }

      uint32_t sum     = checksum_calculator;
      uint32_t compare = reference_checksum<uint32_t, etl::endian::little>(data.begin(), data.end());

      CHECK_EQUAL(compare, sum);
    }

    //*************************************************************************
    TEST(test_checksum_add_range)
    {
      std::string data("123456789");

      etl::bsd_checksum<uint8_t> checksum_calculator;

      checksum_calculator.add(data.begin(), data.end());

      uint8_t sum     = checksum_calculator.value();
      uint8_t compare = reference_checksum<uint8_t, etl::endian::little>(data.begin(), data.end());

      CHECK_EQUAL(int(compare), int(sum));
    }

    //*************************************************************************
    TEST(test_checksum_add_range_sum32)
    {
      std::string data("1");

      etl::bsd_checksum<uint32_t> checksum_calculator;

      checksum_calculator.add(data.begin(), data.end());

      uint32_t sum     = checksum_calculator.value();
      uint32_t compare = reference_checksum<uint32_t, etl::endian::little>(data.begin(), data.end());

      CHECK_EQUAL(compare, sum);
    }

    //*************************************************************************
    TEST(test_checksum_add_range_endian)
    {
      std::vector<uint8_t>  data1 = { 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08 };
      std::vector<uint32_t> data2 = { 0x04030201, 0x08070605 };
      std::vector<uint32_t> data3 = { 0x01020304, 0x05060708 };

      uint8_t sum1 = etl::bsd_checksum<uint8_t, etl::endian::little>(data1.begin(), data1.end());
      uint8_t sum2 = etl::bsd_checksum<uint8_t, etl::endian::little>(data2.begin(), data2.end());
      CHECK_EQUAL(int(sum1), int(sum2));

      uint8_t sum3 = etl::bsd_checksum<uint8_t, etl::endian::big>(data3.begin(), data3.end());
      CHECK_EQUAL(int(sum1), int(sum3));
    }

    //*************************************************************************
    TEST(test_checksum_add_double)
    {
      double d = 3.1415927;

      etl::bsd_checksum<uint8_t> checksum_calculator;

      checksum_calculator << d;

      uint8_t sum = checksum_calculator.value();

      CHECK_EQUAL(120, int(sum));
    }

    //*************************************************************************
    TEST(test_digest)
    {
      std::string data("123456789");

      etl::bsd_checksum<uint8_t>  checksum_calculator8(data.begin(), data.end());
      etl::bsd_checksum<uint16_t> checksum_calculator16(data.begin(), data.end());
      etl::bsd_checksum<uint32_t> checksum_calculator32(data.begin(), data.end());
      etl::bsd_checksum<uint64_t> checksum_calculator64(data.begin(), data.end());

      etl::ihash::generic_digest digest;

      digest = checksum_calculator8.digest();
      uint8_t compare8 = reference_checksum<uint8_t, etl::endian::little>(data.begin(), data.end());
      CHECK_EQUAL(compare8, int(*digest.first));
      CHECK_EQUAL(sizeof(uint8_t), std::distance(digest.first, digest.second));

      digest = checksum_calculator16.digest();
      uint16_t compare16 = reference_checksum<uint16_t, etl::endian::little>(data.begin(), data.end());
      CHECK_EQUAL(compare16, *reinterpret_cast<const uint16_t*>(digest.first));
      CHECK_EQUAL(sizeof(uint16_t), std::distance(digest.first, digest.second));
      
      digest = checksum_calculator32.digest();
      uint32_t compare32 = reference_checksum<uint32_t, etl::endian::little>(data.begin(), data.end());
      CHECK_EQUAL(compare32, *reinterpret_cast<const uint32_t*>(digest.first));
      CHECK_EQUAL(sizeof(uint32_t), std::distance(digest.first, digest.second));
      
      digest = checksum_calculator64.digest();
      uint64_t compare64 = reference_checksum<uint64_t, etl::endian::little>(data.begin(), data.end());
      CHECK_EQUAL(compare64, *reinterpret_cast<const uint64_t*>(digest.first));
      CHECK_EQUAL(sizeof(uint64_t), std::distance(digest.first, digest.second));
    }
  };
}

