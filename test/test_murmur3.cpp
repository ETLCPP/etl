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

#include "murmurhash3.h" // The 'C' reference implementation.

#include <iterator>
#include <string>
#include <vector>
#include <stdint.h>

#include "../src/murmur3.h"
#include "../src/endian.h"

namespace
{		
  SUITE(test_murmur3)
  {
    //*************************************************************************
    TEST(test_murmur3_32_constructor)
    {
      std::string data("123456789");

      uint32_t hash = etl::murmur3<uint32_t>(data.begin(), data.end());

      uint32_t compare;
      MurmurHash3_x86_32(data.c_str(), data.size(), 0, &compare);

      CHECK_EQUAL(compare, hash);
    }

    //*************************************************************************
    TEST(test_murmur3_32_add_values)
    {
      std::string data("123456789");

      etl::murmur3<uint32_t> murmur3_32_calculator;

      for (size_t i = 0; i < data.size(); ++i)
      {
        murmur3_32_calculator.add(data[i]);
      }

      uint32_t hash = murmur3_32_calculator;

      uint32_t compare;
      MurmurHash3_x86_32(data.c_str(), data.size(), 0, &compare);

      CHECK_EQUAL(compare, hash);
    }

    //*************************************************************************
    TEST(test_murmur3_32_add_range)
    {
      std::string data("123456789");

      etl::murmur3<uint32_t> murmur3_32_calculator;

      murmur3_32_calculator.add(data.begin(), data.end());

      uint32_t hash = murmur3_32_calculator.value();

      uint32_t compare;
      MurmurHash3_x86_32(data.c_str(), data.size(), 0, &compare);

      CHECK_EQUAL(compare, hash);
    }

    //*************************************************************************
    TEST(test_murmur3_32_add_range_endian)
    {
      std::vector<uint8_t>  data1 = { 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08 };
      std::vector<uint32_t> data2 = { 0x04030201, 0x08070605 };

      uint32_t hash1 = etl::murmur3<uint32_t>(data1.begin(), data1.end());
      uint32_t hash2 = etl::murmur3<uint32_t>((uint8_t*)&data2[0], (uint8_t*)&data2[0] + (data2.size() * sizeof(uint32_t)));

      CHECK_EQUAL(hash1, hash2);

      uint32_t compare1;
      MurmurHash3_x86_32(&*data1.begin(), data1.size(), 0, &compare1);
      CHECK_EQUAL(compare1, hash1);

      uint32_t compare2;
      MurmurHash3_x86_32((uint8_t*)&data2[0], data2.size() * sizeof(uint32_t), 0, &compare2);
      CHECK_EQUAL(compare2, hash2);
    }
  };
}

