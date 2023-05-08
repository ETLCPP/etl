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

#include "murmurhash3.h" // The 'C' reference implementation.

#include <iterator>
#include <string>
#include <vector>
#include <stdint.h>

#include "etl/murmur3.h"

namespace
{
  SUITE(test_murmur3)
  {
    //*************************************************************************
    TEST(test_murmur3_32_constructor)
    {
      typename std::aligned_storage<sizeof(char), std::alignment_of<uint32_t>::value>::type storage[10];
      std::string data("123456789");      

      char* begin = (char*)&storage[0];
      strcpy(begin, data.c_str());
      
      uint32_t hash = etl::murmur3<uint32_t>(begin, begin + data.size());

      uint32_t compare;
      MurmurHash3_x86_32(begin, data.size(), 0, &compare);

      CHECK_EQUAL(compare, hash);
    }

    //*************************************************************************
    TEST(test_murmur3_32_add_values)
    {
      typename std::aligned_storage<sizeof(char), std::alignment_of<uint32_t>::value>::type storage[10];
      std::string data("123456789");

      char* begin = (char*)&storage[0];
      strcpy(begin, data.c_str());

      etl::murmur3<uint32_t> murmur3_32_calculator;

      for (size_t i = 0UL; i < data.size(); ++i)
      {
        murmur3_32_calculator.add(data[i]);
      }

      uint32_t hash = murmur3_32_calculator;

      uint32_t compare;
      MurmurHash3_x86_32(begin, data.size(), 0, &compare);

      CHECK_EQUAL(compare, hash);
    }

    //*************************************************************************
    TEST(test_murmur3_32_add_range)
    {
      typename std::aligned_storage<sizeof(char), std::alignment_of<uint32_t>::value>::type storage[10];
      std::string data("123456789");

      char* begin = (char*)&storage[0];
      strcpy(begin, data.c_str());

      etl::murmur3<uint32_t> murmur3_32_calculator;

      murmur3_32_calculator.add(data.begin(), data.end());

      uint32_t hash = murmur3_32_calculator.value();

      uint32_t compare;
      MurmurHash3_x86_32(begin, data.size(), 0, &compare);

      CHECK_EQUAL(compare, hash);
    }

    //*************************************************************************
    TEST(test_murmur3_32_add_range_endian)
    {
      std::vector<uint8_t>  data1 = { 0x01U, 0x02U, 0x03U, 0x04U, 0x05U, 0x06U, 0x07U, 0x08U };
      std::vector<uint32_t> data2 = { 0x04030201UL, 0x08070605UL };

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

