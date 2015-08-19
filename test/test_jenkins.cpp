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

#include "murmurhash3.h" // The 'C' reference implementation.

#include <iterator>
#include <string>
#include <vector>
#include <stdint.h>

#include "../jenkins.h"
#include "../endian.h"

template <typename T>
uint32_t jenkins32(T *key, size_t length)
{
  uint32_t hash = 0;

  for (size_t i = 0; i < length; ++i)
  {
    hash += key[i];
    hash += (hash << 10);
    hash ^= (hash >> 6);
  }

  hash += (hash << 3);
  hash ^= (hash >> 11);
  hash += (hash << 15);

  return hash;
}

template <typename T>
uint64_t jenkins64(T *key, size_t length)
{
  uint64_t hash = 0;

  for (size_t i = 0; i < length; ++i)
  {
    hash += key[i];
    hash += (hash << 10);
    hash ^= (hash >> 6);
  }

  hash += (hash << 3);
  hash ^= (hash >> 11);
  hash += (hash << 15);

  return hash;
}

namespace
{		
  SUITE(test_jenkins)
  {
    //*************************************************************************
    TEST(test_jenkins_32_constructor)
    {
      std::string data("123456789");

      uint32_t hash    = etl::jenkins_32<>(data.begin(), data.end());
      uint32_t compare = jenkins32(data.c_str(), data.size());

      CHECK_EQUAL(compare, hash);
    }

    //*************************************************************************
    TEST(test_jenkins_32_add_values)
    {
      std::string data("123456789");

      etl::jenkins_32<> jenkins_32_calculator;

      for (size_t i = 0; i < data.size(); ++i)
      {
        jenkins_32_calculator += data[i];
      }

      uint32_t hash    = jenkins_32_calculator;
      uint32_t compare = jenkins32(data.c_str(), data.size());

      CHECK_EQUAL(compare, hash);
    }

    //*************************************************************************
    TEST(test_jenkins_32_add_range)
    {
      std::string data("123456789");

      etl::jenkins_32<> jenkins_32_calculator;

      jenkins_32_calculator.add(data.begin(), data.end());

      uint32_t hash = jenkins_32_calculator.value();

      uint32_t compare = jenkins32(data.c_str(), data.size());

      CHECK_EQUAL(compare, hash);
    }

    //*************************************************************************
    TEST(test_jenkins_32_add_range_endian)
    {
      std::vector<uint8_t>  data1 = { 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08 };
      std::vector<uint32_t> data2 = { 0x04030201, 0x08070605 };
      std::vector<uint32_t> data3 = { 0x01020304, 0x05060708 };

      uint32_t hash1 = etl::jenkins_32<etl::endian::little>(data1.begin(), data1.end());
      uint32_t hash2 = etl::jenkins_32<etl::endian::little>(data2.begin(), data2.end());
      uint32_t hash3 = etl::jenkins_32<etl::endian::big>(data3.begin(), data3.end());
      CHECK_EQUAL(hash1, hash2);
      CHECK_EQUAL(hash1, hash3);

      uint32_t compare1 = jenkins32((uint8_t*)&data1[0], data1.size());
      CHECK_EQUAL(compare1, hash1);

      uint32_t compare2 = jenkins32((uint8_t*)&data2[0], data2.size() * sizeof(uint32_t));
      CHECK_EQUAL(compare2, hash2);
      
      uint32_t compare3 = jenkins32((uint8_t*)&data3[0], data3.size() * sizeof(uint32_t));
      CHECK_EQUAL(compare2, hash3);
    }

    //*************************************************************************
    TEST(test_jenkins_64_constructor)
    {
      std::string data("123456789");

      uint64_t hash = etl::jenkins_64<>(data.begin(), data.end());
      uint64_t compare = jenkins64(data.c_str(), data.size());

      CHECK_EQUAL(compare, hash);
    }

    //*************************************************************************
    TEST(test_jenkins_64_add_values)
    {
      std::string data("123456789");

      etl::jenkins_64<> jenkins_64_calculator;

      for (size_t i = 0; i < data.size(); ++i)
      {
        jenkins_64_calculator += data[i];
      }

      uint64_t hash = jenkins_64_calculator;
      uint64_t compare = jenkins64(data.c_str(), data.size());

      CHECK_EQUAL(compare, hash);
    }

    //*************************************************************************
    TEST(test_jenkins_64_add_range)
    {
      std::string data("123456789");

      etl::jenkins_64<> jenkins_64_calculator;

      jenkins_64_calculator.add(data.begin(), data.end());

      uint64_t hash = jenkins_64_calculator.value();

      uint64_t compare = jenkins64(data.c_str(), data.size());

      CHECK_EQUAL(compare, hash);
    }

    //*************************************************************************
    TEST(test_jenkins_64_add_range_endian)
    {
      std::vector<uint8_t>  data1 = { 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08 };
      std::vector<uint32_t> data2 = { 0x04030201, 0x08070605 };
      std::vector<uint32_t> data3 = { 0x01020304, 0x05060708 };

      uint64_t hash1 = etl::jenkins_64<etl::endian::little>(data1.begin(), data1.end());
      uint64_t hash2 = etl::jenkins_64<etl::endian::little>(data2.begin(), data2.end());
      uint64_t hash3 = etl::jenkins_64<etl::endian::big>(data3.begin(), data3.end());
      CHECK_EQUAL(hash1, hash2);
      CHECK_EQUAL(hash1, hash3);

      uint64_t compare1 = jenkins64((uint8_t*)&data1[0], data1.size());
      CHECK_EQUAL(compare1, hash1);

      uint64_t compare2 = jenkins64((uint8_t*)&data2[0], data2.size() * sizeof(uint32_t));
      CHECK_EQUAL(compare2, hash2);

      uint64_t compare3 = jenkins64((uint8_t*)&data3[0], data3.size() * sizeof(uint32_t));
      CHECK_EQUAL(compare2, hash3);
    }
  };
}

