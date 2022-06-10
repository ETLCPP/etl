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

#include "etl/jenkins.h"

template <typename TIterator>
uint32_t jenkins(TIterator begin, TIterator end)
{
  uint32_t hash = 0;

  while (begin != end)
  {
    hash += *begin++;
    hash += (hash << 10U);
    hash ^= (hash >> 6U);
  }

  hash += (hash << 3U);
  hash ^= (hash >> 11U);
  hash += (hash << 15U);

  return hash;
}

namespace
{
  SUITE(test_jenkins)
  {
    //*************************************************************************
    TEST(test_jenkins_constructor)
    {
      std::string data("123456789");

      uint32_t hash    = etl::jenkins(data.begin(), data.end());
      uint32_t compare = jenkins(data.begin(), data.end());

      CHECK_EQUAL(compare, hash);
    }

    //*************************************************************************
    TEST(test_jenkins_add_values)
    {
      std::string data("123456789");

      etl::jenkins jenkins_calculator;

      for (size_t i = 0UL; i < data.size(); ++i)
      {
        jenkins_calculator.add(data[i]);
      }

      uint32_t hash    = jenkins_calculator;
      uint32_t compare = jenkins(data.begin(), data.end());

      CHECK_EQUAL(compare, hash);
    }

    //*************************************************************************
    TEST(test_jenkins_add_range)
    {
      std::string data("123456789");

      etl::jenkins jenkins_calculator;

      jenkins_calculator.add(data.begin(), data.end());

      uint32_t hash = jenkins_calculator.value();

      uint32_t compare = jenkins(data.begin(), data.end());

      CHECK_EQUAL(compare, hash);
    }

    //*************************************************************************
    TEST(test_jenkins_add_range_via_iterator)
    {
      std::string data("123456789");

      etl::jenkins jenkins_calculator;

      std::copy(data.begin(), data.end(), jenkins_calculator.input());

      uint32_t hash = jenkins_calculator.value();

      uint32_t compare = jenkins(data.begin(), data.end());

      CHECK_EQUAL(compare, hash);
    }

    //*************************************************************************
    TEST(test_jenkins_add_range_endian)
    {
      std::vector<uint8_t>  data1 = { 0x01U, 0x02U, 0x03U, 0x04U, 0x05U, 0x06U, 0x07U, 0x08U };
      std::vector<uint32_t> data2 = { 0x04030201UL, 0x08070605UL };
      std::vector<uint8_t>  data3 = { 0x08U, 0x07U, 0x06U, 0x05U, 0x04U, 0x03U, 0x02U, 0x01U };

      uint32_t hash1 = etl::jenkins(data1.begin(), data1.end());
      uint32_t hash2 = etl::jenkins((uint8_t*)&data2[0], (uint8_t*)&data2[0] + (data2.size() * sizeof(uint32_t)));
      uint32_t hash3 = etl::jenkins(data3.rbegin(), data3.rend());

      CHECK_EQUAL(hash1, hash2);
      CHECK_EQUAL(hash1, hash3);

      uint64_t compare1 = jenkins(data1.begin(), data1.end());
      CHECK_EQUAL(compare1, hash1);

      uint64_t compare2 = jenkins((uint8_t*)&data2[0], (uint8_t*)&data2[0] + (data2.size() * sizeof(uint32_t)));
      CHECK_EQUAL(compare2, hash2);

      uint64_t compare3 = jenkins(data3.rbegin(), data3.rend());
      CHECK_EQUAL(compare3, hash3);
    }

    //*************************************************************************
    TEST(test_jenkins_finalised_exception)
    {
      std::string data("123456789");

      etl::jenkins j32;
      j32.add(data.begin(), data.end());

      j32.value();

      CHECK_THROW(j32.add(0), etl::hash_finalized);
    }
  };
}

