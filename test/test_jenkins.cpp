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

template <typename TIterator>
uint32_t jenkins32(TIterator begin, TIterator end)
{
  uint32_t hash = 0;

  while (begin != end)
  {
    hash += *begin++;
    hash += (hash << 10);
    hash ^= (hash >> 6);
  }

  hash += (hash << 3);
  hash ^= (hash >> 11);
  hash += (hash << 15);

  return hash;
}

template <typename TIterator>
uint64_t jenkins64(TIterator begin, TIterator end)
{
  uint64_t hash = 0;

  while (begin != end)
  {
    hash += *begin++;
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

      uint32_t hash    = etl::jenkins<uint32_t>(data.begin(), data.end());
      uint32_t compare = jenkins32(data.begin(), data.end());

      CHECK_EQUAL(compare, hash);
    }

    //*************************************************************************
    TEST(test_jenkins_32_add_values)
    {
      std::string data("123456789");

      etl::jenkins<uint32_t> jenkins_32_calculator;

      for (size_t i = 0; i < data.size(); ++i)
      {
        jenkins_32_calculator.add(data[i]);
      }

      uint32_t hash    = jenkins_32_calculator;
      uint32_t compare = jenkins32(data.begin(), data.end());

      CHECK_EQUAL(compare, hash);
    }

    //*************************************************************************
    TEST(test_jenkins_32_add_range)
    {
      std::string data("123456789");

      etl::jenkins<uint32_t> jenkins_32_calculator;

      jenkins_32_calculator.add(data.begin(), data.end());

      uint32_t hash = jenkins_32_calculator.value();

      uint32_t compare = jenkins32(data.begin(), data.end());

      CHECK_EQUAL(compare, hash);
    }

    //*************************************************************************
    TEST(test_jenkins_32_add_range_endian)
    {
      std::vector<uint8_t>  data1 = { 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08 };
      std::vector<uint32_t> data2 = { 0x04030201, 0x08070605 };
      std::vector<uint8_t>  data3 = { 0x08, 0x07, 0x06, 0x05, 0x04, 0x03, 0x02, 0x01 };

      uint32_t hash1 = etl::jenkins<uint32_t>(data1.begin(), data1.end());
      uint32_t hash2 = etl::jenkins<uint32_t>((uint8_t*)&data2[0], (uint8_t*)&data2[0] + (data2.size() * sizeof(uint32_t)));
      uint32_t hash3 = etl::jenkins<uint32_t>(data3.rbegin(), data3.rend());

      CHECK_EQUAL(hash1, hash2);
      CHECK_EQUAL(hash1, hash3);

      uint64_t compare1 = jenkins32(data1.begin(), data1.end());
      CHECK_EQUAL(compare1, hash1);

      uint64_t compare2 = jenkins32((uint8_t*)&data2[0], (uint8_t*)&data2[0] + (data2.size() * sizeof(uint32_t)));
      CHECK_EQUAL(compare2, hash2);

      uint64_t compare3 = jenkins32(data3.rbegin(), data3.rend());
      CHECK_EQUAL(compare2, hash3);
    }

    //*************************************************************************
    TEST(test_jenkins_64_constructor)
    {
      std::string data("123456789");

      uint64_t hash = etl::jenkins<uint64_t>(data.begin(), data.end());
      uint64_t compare = jenkins64(data.begin(), data.end());

      CHECK_EQUAL(compare, hash);
    }

    //*************************************************************************
    TEST(test_jenkins_64_add_values)
    {
      std::string data("123456789");

      etl::jenkins<uint64_t> jenkins_64_calculator;

      for (size_t i = 0; i < data.size(); ++i)
      {
        jenkins_64_calculator.add(data[i]);
      }

      uint64_t hash = jenkins_64_calculator;
      uint64_t compare = jenkins64(data.begin(), data.end());

      CHECK_EQUAL(compare, hash);
    }

    //*************************************************************************
    TEST(test_jenkins_64_add_range)
    {
      std::string data("123456789");

      etl::jenkins<uint64_t> jenkins_64_calculator;

      jenkins_64_calculator.add(data.begin(), data.end());

      uint64_t hash = jenkins_64_calculator.value();

      uint64_t compare = jenkins64(data.begin(), data.end());

      CHECK_EQUAL(compare, hash);
    }

    //*************************************************************************
    TEST(test_jenkins_64_add_range_endian)
    {
      std::vector<uint8_t>  data1 = { 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08 };
      std::vector<uint32_t> data2 = { 0x04030201, 0x08070605 };
      std::vector<uint8_t>  data3 = { 0x08, 0x07, 0x06, 0x05, 0x04, 0x03, 0x02, 0x01 };

      uint64_t hash1 = etl::jenkins<uint64_t>(data1.begin(), data1.end());
      uint64_t hash2 = etl::jenkins<uint64_t>((uint8_t*)&data2[0], (uint8_t*)&data2[0] + (data2.size() * sizeof(uint32_t)));
      uint64_t hash3 = etl::jenkins<uint64_t>(data3.rbegin(), data3.rend());
      CHECK_EQUAL(hash1, hash2);
      CHECK_EQUAL(hash1, hash3);

      uint64_t compare1 = jenkins64(data1.begin(), data1.end());
      CHECK_EQUAL(compare1, hash1);

      uint64_t compare2 = jenkins64((uint8_t*)&data2[0], (uint8_t*)&data2[0] + (data2.size() * sizeof(uint32_t)));
      CHECK_EQUAL(compare2, hash2);

      uint64_t compare3 = jenkins64(data3.rbegin(), data3.rend());
      CHECK_EQUAL(compare2, hash3);
    }
  };
}

