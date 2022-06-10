/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl
https://www.etlcpp.com

Copyright(c) 2015 John Wellbelove

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
#include <iomanip>
#include <stdint.h>

#include "etl/pearson.h"

const size_t HASH_SIZE = 8;
typedef etl::pearson<HASH_SIZE>::value_type hash_t;

//***************************************************************************
/// Pearson lookup table
/// \ingroup pearson
//***************************************************************************
namespace etl
{
  static const uint8_t PEARSON_LOOKUP[] =
  {
    228,  39,  61,  95, 227, 187,   0, 197,  31, 189, 161, 222,  34,  15, 221, 246,
    19,  234,   6,  50, 113,   3,  91,  63,  77, 245, 144,   2, 183, 196,  25, 226,
    97,  126,  48,  59, 217,   4, 100, 145,  12,  88, 203, 149,  80, 154,  38,  27,
    224, 218, 158, 115, 202,  79,  53,  83, 242,  36, 139, 131, 136, 191,  42, 170,
    23,  99,  156,  51, 143,  60, 233, 206,  62, 108,  17,  67,  81,  71,  93, 195,
    26,  231, 247,  96,  24, 200, 176, 209, 152, 212, 138, 165,  75, 185, 130, 248,
    125, 110,  10, 116, 201,  90,  69, 204,  85, 251,  78, 157,  47, 184, 169, 141,
    134, 230,  89,  21, 146,  46,  55, 128, 148, 207, 216,  11, 114, 199, 103, 102,
    166, 244,   5, 104, 225, 160, 132,  28, 172,  65, 121, 140, 153, 119, 198, 210,
    58,  87,  117, 177,  33,  22,  13,  37,  49, 174, 109,  40,  73, 211,  18, 167,
    164, 252, 168,  74,  30, 173,  35,  98,  66, 193,  94, 175,  86,  54, 179, 122,
    220, 151, 192,  29, 133, 254, 155, 127, 240, 232, 190, 180,   8,  68, 236,  20,
    137,  92, 219, 208,  52, 250, 147, 142, 111, 112, 120,  45, 135, 255, 123, 229,
    57,  182, 243, 124, 186, 253,   7, 237,   9,  16,  70, 171, 235, 107, 223, 118,
    215, 178, 194, 181,  43, 188, 106, 105,  64, 241,  84, 238, 159,  44,  32,  76,
    213, 163, 150, 101, 129,  14, 249, 205, 214,   1,  41,  56, 162,  72, 239,  82
  };
}

//***************************************************************************
// Comparison calculator based on Wikipedia example. https://en.wikipedia.org/wiki/Pearson_hashing
//***************************************************************************
template <typename TContainer>
hash_t Pearson_Compare(const TContainer& data)
{
  hash_t hash;
  hash.fill(0);

  for (size_t j = 0UL; j < HASH_SIZE; ++j)
  {
    uint8_t subhash = etl::PEARSON_LOOKUP[(data[0] + j) % 256];

    for (size_t i = 1UL; i < data.size(); ++i)
    {
      subhash = etl::PEARSON_LOOKUP[subhash ^ data[i]];
    }

    hash[j] = subhash;
  }

  return hash;
}

//***************************************************************************
// Output stream for hash_t
//***************************************************************************
std::ostream& operator <<(std::ostream& os, const hash_t& hash)
{
  for (size_t i = 0UL; i < hash.size(); ++i)
  {
    os << int(hash[i]) << " ";
  }

  return os;
}

namespace
{
  SUITE(test_pearson)
  {
    //*************************************************************************
    TEST(test_pearson_constructor)
    {
      std::string data("123456789");

      hash_t compare = Pearson_Compare(data);
      hash_t hash    = etl::pearson<HASH_SIZE>(data.begin(), data.end());

      CHECK(compare == hash);
    }

    //*************************************************************************
    TEST(test_pearson_add_values)
    {
      std::string data("123456789");

      etl::pearson<HASH_SIZE> pearson_calculator;

      for (size_t i = 0UL; i < data.size(); ++i)
      {
        pearson_calculator.add(data[i]);
      }

      hash_t compare = Pearson_Compare(data);
      hash_t hash    = pearson_calculator;

      CHECK(compare == hash);
    }

    //*************************************************************************
    TEST(test_pearson_add_range)
    {
      std::string data("123456789");

      etl::pearson<HASH_SIZE> pearson_calculator;

      pearson_calculator.add(data.begin(), data.end());

      hash_t compare = Pearson_Compare(data);
      hash_t hash    = pearson_calculator.value();

      CHECK(compare == hash);
    }

    //*************************************************************************
    TEST(test_pearson_add_range_endian)
    {
      std::vector<uint8_t>  data1 = { 0x01U, 0x02U, 0x03U, 0x04U, 0x05U, 0x06U, 0x07U, 0x08U };
      std::vector<uint32_t> data2 = { 0x04030201UL, 0x08070605UL };
      std::vector<uint8_t>  data3 = { 0x08U, 0x07U, 0x06U, 0x05U, 0x04U, 0x03U, 0x02U, 0x01U };

      hash_t hash1 = etl::pearson<HASH_SIZE>(data1.begin(), data1.end());
      hash_t hash2 = etl::pearson<HASH_SIZE>((uint8_t*)&data2[0], (uint8_t*)(&data2[0] + data2.size()));
      CHECK(hash1 == hash2);

      hash_t hash3 = etl::pearson<HASH_SIZE>(data3.rbegin(), data3.rend());
      CHECK(hash1 == hash3);
    }
  };
}

