/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl
https://www.etlcpp.com

Copyright(c) 2024 John Wellbelove

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

#include "etl/base64_decoder.h"

#include "etl/string.h"
#include "etl/vector.h"

#include <string>
#include <array>
#include <vector>
#include <algorithm>

#if ETL_USING_STL
  #include <iterator>
#endif

#include "etl/integral_limits.h"

//*****************************************************************************
// Encode/decode using RFC-3501
//*****************************************************************************

namespace
{
  using codec               = etl::base64_rfc3501_decoder<etl::base64::Min_Decode_Buffer_Size>;
  using codec_larger_buffer = etl::base64_rfc3501_decoder<etl::base64::Min_Decode_Buffer_Size * 10>;
#if (ETL_USING_CPP14)
  using codec_full_buffer   = etl::base64_rfc3501_decoder< etl::base64_rfc3501_decoder<>::safe_output_buffer_size(342)>;
#else
  using codec_full_buffer   = etl::base64_rfc3501_decoder<256>;
#endif

  std::vector<unsigned char> decoded_output;
  bool received_final_block = false;

  auto lambda = [](const codec::span_type& sp)
    {
      if (sp.empty())
      {
        received_final_block = true;
      }
      else
      {
        std::copy(sp.begin(), sp.end(), std::back_inserter(decoded_output));
      }
    };

  codec::callback_type callback = lambda;

  std::array<unsigned char, 256> input_data =
  {
    0x3B, 0x27, 0x03, 0x43, 0x2D, 0xFB, 0x28, 0x2A, 0x61, 0xAE, 0xBC, 0x49, 0x71, 0x32, 0x01, 0x15,
    0x69, 0x5C, 0x5E, 0xF5, 0xD5, 0x9B, 0xDE, 0xA5, 0x57, 0xC9, 0xC1, 0x7D, 0x80, 0xDE, 0x4C, 0x81,
    0xC0, 0xCF, 0x2A, 0xD1, 0x86, 0x56, 0xD5, 0x71, 0x37, 0xEB, 0x80, 0x32, 0xDF, 0xE4, 0xDF, 0xB6,
    0xEE, 0x3F, 0xDC, 0x79, 0xB3, 0x17, 0x8E, 0x76, 0x65, 0x8E, 0x96, 0x21, 0xB9, 0x88, 0xD1, 0x6D,
    0xD0, 0xDD, 0xFF, 0xDA, 0xA8, 0x7A, 0x4D, 0xF5, 0x71, 0x77, 0xFD, 0x2E, 0xF2, 0xE4, 0x40, 0x72,
    0x8C, 0x83, 0x00, 0x6F, 0x13, 0x72, 0x53, 0xE4, 0x6B, 0x70, 0x0E, 0x37, 0xCA, 0x25, 0xCD, 0x68,
    0x62, 0xC0, 0xAB, 0x14, 0xC7, 0x59, 0x83, 0xD2, 0x82, 0x8C, 0x93, 0x6D, 0x13, 0x21, 0xC0, 0x08,
    0xF9, 0x6D, 0xAC, 0x84, 0x78, 0x49, 0x84, 0x6F, 0x6B, 0xFB, 0x20, 0x3B, 0x9C, 0x49, 0xFB, 0x4E,
    0x80, 0x69, 0x82, 0x25, 0x86, 0x95, 0xD5, 0x4D, 0x91, 0xED, 0xD2, 0x77, 0x2A, 0x24, 0x40, 0x8A,
    0xDF, 0x4D, 0x80, 0x2D, 0xCD, 0xD5, 0x5A, 0x26, 0xA6, 0x71, 0x15, 0x42, 0x0E, 0x3F, 0xB2, 0x70,
    0x14, 0x29, 0x1F, 0x8D, 0x23, 0x2E, 0xC1, 0xEA, 0xCE, 0xF9, 0x7E, 0x6C, 0xDF, 0x1C, 0xA3, 0x84,
    0x2B, 0x24, 0x35, 0xA7, 0x63, 0xC8, 0x0B, 0x1F, 0x8B, 0xBA, 0x51, 0xBF, 0xE9, 0x51, 0x80, 0xD2,
    0x23, 0xB5, 0xD1, 0xB4, 0x59, 0xAE, 0x7D, 0x30, 0x1D, 0x00, 0x1C, 0xD8, 0x70, 0x6C, 0x16, 0x71,
    0xC7, 0x56, 0x08, 0xFE, 0x81, 0xAE, 0xFB, 0xE0, 0x92, 0xD8, 0xDB, 0xB9, 0x57, 0x7C, 0x99, 0xCB,
    0x42, 0xEF, 0xFC, 0xB3, 0x56, 0x1E, 0xD1, 0x42, 0xD3, 0x0C, 0x18, 0xB3, 0xEE, 0xAF, 0x1A, 0x77,
    0xA8, 0x52, 0x3C, 0x9E, 0xCD, 0xDE, 0x21, 0x34, 0x3E, 0x1F, 0xB5, 0x54, 0xD7, 0xFB, 0xB4, 0xBD
  };

  std::array<std::string, 257> encoded =
  {
    "",
    "Ow",
    "Oyc",
    "OycD",
    "OycDQw",
    "OycDQy0",
    "OycDQy37",
    "OycDQy37KA",
    "OycDQy37KCo",
    "OycDQy37KCph",
    "OycDQy37KCphrg",
    "OycDQy37KCphrrw",
    "OycDQy37KCphrrxJ",
    "OycDQy37KCphrrxJcQ",
    "OycDQy37KCphrrxJcTI",
    "OycDQy37KCphrrxJcTIB",
    "OycDQy37KCphrrxJcTIBFQ",
    "OycDQy37KCphrrxJcTIBFWk",
    "OycDQy37KCphrrxJcTIBFWlc",
    "OycDQy37KCphrrxJcTIBFWlcXg",
    "OycDQy37KCphrrxJcTIBFWlcXvU",
    "OycDQy37KCphrrxJcTIBFWlcXvXV",
    "OycDQy37KCphrrxJcTIBFWlcXvXVmw",
    "OycDQy37KCphrrxJcTIBFWlcXvXVm94",
    "OycDQy37KCphrrxJcTIBFWlcXvXVm96l",
    "OycDQy37KCphrrxJcTIBFWlcXvXVm96lVw",
    "OycDQy37KCphrrxJcTIBFWlcXvXVm96lV8k",
    "OycDQy37KCphrrxJcTIBFWlcXvXVm96lV8nB",
    "OycDQy37KCphrrxJcTIBFWlcXvXVm96lV8nBfQ",
    "OycDQy37KCphrrxJcTIBFWlcXvXVm96lV8nBfYA",
    "OycDQy37KCphrrxJcTIBFWlcXvXVm96lV8nBfYDe",
    "OycDQy37KCphrrxJcTIBFWlcXvXVm96lV8nBfYDeTA",
    "OycDQy37KCphrrxJcTIBFWlcXvXVm96lV8nBfYDeTIE",
    "OycDQy37KCphrrxJcTIBFWlcXvXVm96lV8nBfYDeTIHA",
    "OycDQy37KCphrrxJcTIBFWlcXvXVm96lV8nBfYDeTIHAzw",
    "OycDQy37KCphrrxJcTIBFWlcXvXVm96lV8nBfYDeTIHAzyo",
    "OycDQy37KCphrrxJcTIBFWlcXvXVm96lV8nBfYDeTIHAzyrR",
    "OycDQy37KCphrrxJcTIBFWlcXvXVm96lV8nBfYDeTIHAzyrRhg",
    "OycDQy37KCphrrxJcTIBFWlcXvXVm96lV8nBfYDeTIHAzyrRhlY",
    "OycDQy37KCphrrxJcTIBFWlcXvXVm96lV8nBfYDeTIHAzyrRhlbV",
    "OycDQy37KCphrrxJcTIBFWlcXvXVm96lV8nBfYDeTIHAzyrRhlbVcQ",
    "OycDQy37KCphrrxJcTIBFWlcXvXVm96lV8nBfYDeTIHAzyrRhlbVcTc",
    "OycDQy37KCphrrxJcTIBFWlcXvXVm96lV8nBfYDeTIHAzyrRhlbVcTfr",
    "OycDQy37KCphrrxJcTIBFWlcXvXVm96lV8nBfYDeTIHAzyrRhlbVcTfrgA",
    "OycDQy37KCphrrxJcTIBFWlcXvXVm96lV8nBfYDeTIHAzyrRhlbVcTfrgDI",
    "OycDQy37KCphrrxJcTIBFWlcXvXVm96lV8nBfYDeTIHAzyrRhlbVcTfrgDLf",
    "OycDQy37KCphrrxJcTIBFWlcXvXVm96lV8nBfYDeTIHAzyrRhlbVcTfrgDLf5A",
    "OycDQy37KCphrrxJcTIBFWlcXvXVm96lV8nBfYDeTIHAzyrRhlbVcTfrgDLf5N8",
    "OycDQy37KCphrrxJcTIBFWlcXvXVm96lV8nBfYDeTIHAzyrRhlbVcTfrgDLf5N+2",
    "OycDQy37KCphrrxJcTIBFWlcXvXVm96lV8nBfYDeTIHAzyrRhlbVcTfrgDLf5N+27g",
    "OycDQy37KCphrrxJcTIBFWlcXvXVm96lV8nBfYDeTIHAzyrRhlbVcTfrgDLf5N+27j8",
    "OycDQy37KCphrrxJcTIBFWlcXvXVm96lV8nBfYDeTIHAzyrRhlbVcTfrgDLf5N+27j,c",
    "OycDQy37KCphrrxJcTIBFWlcXvXVm96lV8nBfYDeTIHAzyrRhlbVcTfrgDLf5N+27j,ceQ",
    "OycDQy37KCphrrxJcTIBFWlcXvXVm96lV8nBfYDeTIHAzyrRhlbVcTfrgDLf5N+27j,cebM",
    "OycDQy37KCphrrxJcTIBFWlcXvXVm96lV8nBfYDeTIHAzyrRhlbVcTfrgDLf5N+27j,cebMX",
    "OycDQy37KCphrrxJcTIBFWlcXvXVm96lV8nBfYDeTIHAzyrRhlbVcTfrgDLf5N+27j,cebMXjg",
    "OycDQy37KCphrrxJcTIBFWlcXvXVm96lV8nBfYDeTIHAzyrRhlbVcTfrgDLf5N+27j,cebMXjnY",
    "OycDQy37KCphrrxJcTIBFWlcXvXVm96lV8nBfYDeTIHAzyrRhlbVcTfrgDLf5N+27j,cebMXjnZl",
    "OycDQy37KCphrrxJcTIBFWlcXvXVm96lV8nBfYDeTIHAzyrRhlbVcTfrgDLf5N+27j,cebMXjnZljg",
    "OycDQy37KCphrrxJcTIBFWlcXvXVm96lV8nBfYDeTIHAzyrRhlbVcTfrgDLf5N+27j,cebMXjnZljpY",
    "OycDQy37KCphrrxJcTIBFWlcXvXVm96lV8nBfYDeTIHAzyrRhlbVcTfrgDLf5N+27j,cebMXjnZljpYh",
    "OycDQy37KCphrrxJcTIBFWlcXvXVm96lV8nBfYDeTIHAzyrRhlbVcTfrgDLf5N+27j,cebMXjnZljpYhuQ",
    "OycDQy37KCphrrxJcTIBFWlcXvXVm96lV8nBfYDeTIHAzyrRhlbVcTfrgDLf5N+27j,cebMXjnZljpYhuYg",
    "OycDQy37KCphrrxJcTIBFWlcXvXVm96lV8nBfYDeTIHAzyrRhlbVcTfrgDLf5N+27j,cebMXjnZljpYhuYjR",
    "OycDQy37KCphrrxJcTIBFWlcXvXVm96lV8nBfYDeTIHAzyrRhlbVcTfrgDLf5N+27j,cebMXjnZljpYhuYjRbQ",
    "OycDQy37KCphrrxJcTIBFWlcXvXVm96lV8nBfYDeTIHAzyrRhlbVcTfrgDLf5N+27j,cebMXjnZljpYhuYjRbdA",
    "OycDQy37KCphrrxJcTIBFWlcXvXVm96lV8nBfYDeTIHAzyrRhlbVcTfrgDLf5N+27j,cebMXjnZljpYhuYjRbdDd",
    "OycDQy37KCphrrxJcTIBFWlcXvXVm96lV8nBfYDeTIHAzyrRhlbVcTfrgDLf5N+27j,cebMXjnZljpYhuYjRbdDd,w",
    "OycDQy37KCphrrxJcTIBFWlcXvXVm96lV8nBfYDeTIHAzyrRhlbVcTfrgDLf5N+27j,cebMXjnZljpYhuYjRbdDd,9o",
    "OycDQy37KCphrrxJcTIBFWlcXvXVm96lV8nBfYDeTIHAzyrRhlbVcTfrgDLf5N+27j,cebMXjnZljpYhuYjRbdDd,9qo",
    "OycDQy37KCphrrxJcTIBFWlcXvXVm96lV8nBfYDeTIHAzyrRhlbVcTfrgDLf5N+27j,cebMXjnZljpYhuYjRbdDd,9qoeg",
    "OycDQy37KCphrrxJcTIBFWlcXvXVm96lV8nBfYDeTIHAzyrRhlbVcTfrgDLf5N+27j,cebMXjnZljpYhuYjRbdDd,9qoek0",
    "OycDQy37KCphrrxJcTIBFWlcXvXVm96lV8nBfYDeTIHAzyrRhlbVcTfrgDLf5N+27j,cebMXjnZljpYhuYjRbdDd,9qoek31",
    "OycDQy37KCphrrxJcTIBFWlcXvXVm96lV8nBfYDeTIHAzyrRhlbVcTfrgDLf5N+27j,cebMXjnZljpYhuYjRbdDd,9qoek31cQ",
    "OycDQy37KCphrrxJcTIBFWlcXvXVm96lV8nBfYDeTIHAzyrRhlbVcTfrgDLf5N+27j,cebMXjnZljpYhuYjRbdDd,9qoek31cXc",
    "OycDQy37KCphrrxJcTIBFWlcXvXVm96lV8nBfYDeTIHAzyrRhlbVcTfrgDLf5N+27j,cebMXjnZljpYhuYjRbdDd,9qoek31cXf9",
    "OycDQy37KCphrrxJcTIBFWlcXvXVm96lV8nBfYDeTIHAzyrRhlbVcTfrgDLf5N+27j,cebMXjnZljpYhuYjRbdDd,9qoek31cXf9Lg",
    "OycDQy37KCphrrxJcTIBFWlcXvXVm96lV8nBfYDeTIHAzyrRhlbVcTfrgDLf5N+27j,cebMXjnZljpYhuYjRbdDd,9qoek31cXf9LvI",
    "OycDQy37KCphrrxJcTIBFWlcXvXVm96lV8nBfYDeTIHAzyrRhlbVcTfrgDLf5N+27j,cebMXjnZljpYhuYjRbdDd,9qoek31cXf9LvLk",
    "OycDQy37KCphrrxJcTIBFWlcXvXVm96lV8nBfYDeTIHAzyrRhlbVcTfrgDLf5N+27j,cebMXjnZljpYhuYjRbdDd,9qoek31cXf9LvLkQA",
    "OycDQy37KCphrrxJcTIBFWlcXvXVm96lV8nBfYDeTIHAzyrRhlbVcTfrgDLf5N+27j,cebMXjnZljpYhuYjRbdDd,9qoek31cXf9LvLkQHI",
    "OycDQy37KCphrrxJcTIBFWlcXvXVm96lV8nBfYDeTIHAzyrRhlbVcTfrgDLf5N+27j,cebMXjnZljpYhuYjRbdDd,9qoek31cXf9LvLkQHKM",
    "OycDQy37KCphrrxJcTIBFWlcXvXVm96lV8nBfYDeTIHAzyrRhlbVcTfrgDLf5N+27j,cebMXjnZljpYhuYjRbdDd,9qoek31cXf9LvLkQHKMgw",
    "OycDQy37KCphrrxJcTIBFWlcXvXVm96lV8nBfYDeTIHAzyrRhlbVcTfrgDLf5N+27j,cebMXjnZljpYhuYjRbdDd,9qoek31cXf9LvLkQHKMgwA",
    "OycDQy37KCphrrxJcTIBFWlcXvXVm96lV8nBfYDeTIHAzyrRhlbVcTfrgDLf5N+27j,cebMXjnZljpYhuYjRbdDd,9qoek31cXf9LvLkQHKMgwBv",
    "OycDQy37KCphrrxJcTIBFWlcXvXVm96lV8nBfYDeTIHAzyrRhlbVcTfrgDLf5N+27j,cebMXjnZljpYhuYjRbdDd,9qoek31cXf9LvLkQHKMgwBvEw",
    "OycDQy37KCphrrxJcTIBFWlcXvXVm96lV8nBfYDeTIHAzyrRhlbVcTfrgDLf5N+27j,cebMXjnZljpYhuYjRbdDd,9qoek31cXf9LvLkQHKMgwBvE3I",
    "OycDQy37KCphrrxJcTIBFWlcXvXVm96lV8nBfYDeTIHAzyrRhlbVcTfrgDLf5N+27j,cebMXjnZljpYhuYjRbdDd,9qoek31cXf9LvLkQHKMgwBvE3JT",
    "OycDQy37KCphrrxJcTIBFWlcXvXVm96lV8nBfYDeTIHAzyrRhlbVcTfrgDLf5N+27j,cebMXjnZljpYhuYjRbdDd,9qoek31cXf9LvLkQHKMgwBvE3JT5A",
    "OycDQy37KCphrrxJcTIBFWlcXvXVm96lV8nBfYDeTIHAzyrRhlbVcTfrgDLf5N+27j,cebMXjnZljpYhuYjRbdDd,9qoek31cXf9LvLkQHKMgwBvE3JT5Gs",
    "OycDQy37KCphrrxJcTIBFWlcXvXVm96lV8nBfYDeTIHAzyrRhlbVcTfrgDLf5N+27j,cebMXjnZljpYhuYjRbdDd,9qoek31cXf9LvLkQHKMgwBvE3JT5Gtw",
    "OycDQy37KCphrrxJcTIBFWlcXvXVm96lV8nBfYDeTIHAzyrRhlbVcTfrgDLf5N+27j,cebMXjnZljpYhuYjRbdDd,9qoek31cXf9LvLkQHKMgwBvE3JT5GtwDg",
    "OycDQy37KCphrrxJcTIBFWlcXvXVm96lV8nBfYDeTIHAzyrRhlbVcTfrgDLf5N+27j,cebMXjnZljpYhuYjRbdDd,9qoek31cXf9LvLkQHKMgwBvE3JT5GtwDjc",
    "OycDQy37KCphrrxJcTIBFWlcXvXVm96lV8nBfYDeTIHAzyrRhlbVcTfrgDLf5N+27j,cebMXjnZljpYhuYjRbdDd,9qoek31cXf9LvLkQHKMgwBvE3JT5GtwDjfK",
    "OycDQy37KCphrrxJcTIBFWlcXvXVm96lV8nBfYDeTIHAzyrRhlbVcTfrgDLf5N+27j,cebMXjnZljpYhuYjRbdDd,9qoek31cXf9LvLkQHKMgwBvE3JT5GtwDjfKJQ",
    "OycDQy37KCphrrxJcTIBFWlcXvXVm96lV8nBfYDeTIHAzyrRhlbVcTfrgDLf5N+27j,cebMXjnZljpYhuYjRbdDd,9qoek31cXf9LvLkQHKMgwBvE3JT5GtwDjfKJc0",
    "OycDQy37KCphrrxJcTIBFWlcXvXVm96lV8nBfYDeTIHAzyrRhlbVcTfrgDLf5N+27j,cebMXjnZljpYhuYjRbdDd,9qoek31cXf9LvLkQHKMgwBvE3JT5GtwDjfKJc1o",
    "OycDQy37KCphrrxJcTIBFWlcXvXVm96lV8nBfYDeTIHAzyrRhlbVcTfrgDLf5N+27j,cebMXjnZljpYhuYjRbdDd,9qoek31cXf9LvLkQHKMgwBvE3JT5GtwDjfKJc1oYg",
    "OycDQy37KCphrrxJcTIBFWlcXvXVm96lV8nBfYDeTIHAzyrRhlbVcTfrgDLf5N+27j,cebMXjnZljpYhuYjRbdDd,9qoek31cXf9LvLkQHKMgwBvE3JT5GtwDjfKJc1oYsA",
    "OycDQy37KCphrrxJcTIBFWlcXvXVm96lV8nBfYDeTIHAzyrRhlbVcTfrgDLf5N+27j,cebMXjnZljpYhuYjRbdDd,9qoek31cXf9LvLkQHKMgwBvE3JT5GtwDjfKJc1oYsCr",
    "OycDQy37KCphrrxJcTIBFWlcXvXVm96lV8nBfYDeTIHAzyrRhlbVcTfrgDLf5N+27j,cebMXjnZljpYhuYjRbdDd,9qoek31cXf9LvLkQHKMgwBvE3JT5GtwDjfKJc1oYsCrFA",
    "OycDQy37KCphrrxJcTIBFWlcXvXVm96lV8nBfYDeTIHAzyrRhlbVcTfrgDLf5N+27j,cebMXjnZljpYhuYjRbdDd,9qoek31cXf9LvLkQHKMgwBvE3JT5GtwDjfKJc1oYsCrFMc",
    "OycDQy37KCphrrxJcTIBFWlcXvXVm96lV8nBfYDeTIHAzyrRhlbVcTfrgDLf5N+27j,cebMXjnZljpYhuYjRbdDd,9qoek31cXf9LvLkQHKMgwBvE3JT5GtwDjfKJc1oYsCrFMdZ",
    "OycDQy37KCphrrxJcTIBFWlcXvXVm96lV8nBfYDeTIHAzyrRhlbVcTfrgDLf5N+27j,cebMXjnZljpYhuYjRbdDd,9qoek31cXf9LvLkQHKMgwBvE3JT5GtwDjfKJc1oYsCrFMdZgw",
    "OycDQy37KCphrrxJcTIBFWlcXvXVm96lV8nBfYDeTIHAzyrRhlbVcTfrgDLf5N+27j,cebMXjnZljpYhuYjRbdDd,9qoek31cXf9LvLkQHKMgwBvE3JT5GtwDjfKJc1oYsCrFMdZg9I",
    "OycDQy37KCphrrxJcTIBFWlcXvXVm96lV8nBfYDeTIHAzyrRhlbVcTfrgDLf5N+27j,cebMXjnZljpYhuYjRbdDd,9qoek31cXf9LvLkQHKMgwBvE3JT5GtwDjfKJc1oYsCrFMdZg9KC",
    "OycDQy37KCphrrxJcTIBFWlcXvXVm96lV8nBfYDeTIHAzyrRhlbVcTfrgDLf5N+27j,cebMXjnZljpYhuYjRbdDd,9qoek31cXf9LvLkQHKMgwBvE3JT5GtwDjfKJc1oYsCrFMdZg9KCjA",
    "OycDQy37KCphrrxJcTIBFWlcXvXVm96lV8nBfYDeTIHAzyrRhlbVcTfrgDLf5N+27j,cebMXjnZljpYhuYjRbdDd,9qoek31cXf9LvLkQHKMgwBvE3JT5GtwDjfKJc1oYsCrFMdZg9KCjJM",
    "OycDQy37KCphrrxJcTIBFWlcXvXVm96lV8nBfYDeTIHAzyrRhlbVcTfrgDLf5N+27j,cebMXjnZljpYhuYjRbdDd,9qoek31cXf9LvLkQHKMgwBvE3JT5GtwDjfKJc1oYsCrFMdZg9KCjJNt",
    "OycDQy37KCphrrxJcTIBFWlcXvXVm96lV8nBfYDeTIHAzyrRhlbVcTfrgDLf5N+27j,cebMXjnZljpYhuYjRbdDd,9qoek31cXf9LvLkQHKMgwBvE3JT5GtwDjfKJc1oYsCrFMdZg9KCjJNtEw",
    "OycDQy37KCphrrxJcTIBFWlcXvXVm96lV8nBfYDeTIHAzyrRhlbVcTfrgDLf5N+27j,cebMXjnZljpYhuYjRbdDd,9qoek31cXf9LvLkQHKMgwBvE3JT5GtwDjfKJc1oYsCrFMdZg9KCjJNtEyE",
    "OycDQy37KCphrrxJcTIBFWlcXvXVm96lV8nBfYDeTIHAzyrRhlbVcTfrgDLf5N+27j,cebMXjnZljpYhuYjRbdDd,9qoek31cXf9LvLkQHKMgwBvE3JT5GtwDjfKJc1oYsCrFMdZg9KCjJNtEyHA",
    "OycDQy37KCphrrxJcTIBFWlcXvXVm96lV8nBfYDeTIHAzyrRhlbVcTfrgDLf5N+27j,cebMXjnZljpYhuYjRbdDd,9qoek31cXf9LvLkQHKMgwBvE3JT5GtwDjfKJc1oYsCrFMdZg9KCjJNtEyHACA",
    "OycDQy37KCphrrxJcTIBFWlcXvXVm96lV8nBfYDeTIHAzyrRhlbVcTfrgDLf5N+27j,cebMXjnZljpYhuYjRbdDd,9qoek31cXf9LvLkQHKMgwBvE3JT5GtwDjfKJc1oYsCrFMdZg9KCjJNtEyHACPk",
    "OycDQy37KCphrrxJcTIBFWlcXvXVm96lV8nBfYDeTIHAzyrRhlbVcTfrgDLf5N+27j,cebMXjnZljpYhuYjRbdDd,9qoek31cXf9LvLkQHKMgwBvE3JT5GtwDjfKJc1oYsCrFMdZg9KCjJNtEyHACPlt",
    "OycDQy37KCphrrxJcTIBFWlcXvXVm96lV8nBfYDeTIHAzyrRhlbVcTfrgDLf5N+27j,cebMXjnZljpYhuYjRbdDd,9qoek31cXf9LvLkQHKMgwBvE3JT5GtwDjfKJc1oYsCrFMdZg9KCjJNtEyHACPltrA",
    "OycDQy37KCphrrxJcTIBFWlcXvXVm96lV8nBfYDeTIHAzyrRhlbVcTfrgDLf5N+27j,cebMXjnZljpYhuYjRbdDd,9qoek31cXf9LvLkQHKMgwBvE3JT5GtwDjfKJc1oYsCrFMdZg9KCjJNtEyHACPltrIQ",
    "OycDQy37KCphrrxJcTIBFWlcXvXVm96lV8nBfYDeTIHAzyrRhlbVcTfrgDLf5N+27j,cebMXjnZljpYhuYjRbdDd,9qoek31cXf9LvLkQHKMgwBvE3JT5GtwDjfKJc1oYsCrFMdZg9KCjJNtEyHACPltrIR4",
    "OycDQy37KCphrrxJcTIBFWlcXvXVm96lV8nBfYDeTIHAzyrRhlbVcTfrgDLf5N+27j,cebMXjnZljpYhuYjRbdDd,9qoek31cXf9LvLkQHKMgwBvE3JT5GtwDjfKJc1oYsCrFMdZg9KCjJNtEyHACPltrIR4SQ",
    "OycDQy37KCphrrxJcTIBFWlcXvXVm96lV8nBfYDeTIHAzyrRhlbVcTfrgDLf5N+27j,cebMXjnZljpYhuYjRbdDd,9qoek31cXf9LvLkQHKMgwBvE3JT5GtwDjfKJc1oYsCrFMdZg9KCjJNtEyHACPltrIR4SYQ",
    "OycDQy37KCphrrxJcTIBFWlcXvXVm96lV8nBfYDeTIHAzyrRhlbVcTfrgDLf5N+27j,cebMXjnZljpYhuYjRbdDd,9qoek31cXf9LvLkQHKMgwBvE3JT5GtwDjfKJc1oYsCrFMdZg9KCjJNtEyHACPltrIR4SYRv",
    "OycDQy37KCphrrxJcTIBFWlcXvXVm96lV8nBfYDeTIHAzyrRhlbVcTfrgDLf5N+27j,cebMXjnZljpYhuYjRbdDd,9qoek31cXf9LvLkQHKMgwBvE3JT5GtwDjfKJc1oYsCrFMdZg9KCjJNtEyHACPltrIR4SYRvaw",
    "OycDQy37KCphrrxJcTIBFWlcXvXVm96lV8nBfYDeTIHAzyrRhlbVcTfrgDLf5N+27j,cebMXjnZljpYhuYjRbdDd,9qoek31cXf9LvLkQHKMgwBvE3JT5GtwDjfKJc1oYsCrFMdZg9KCjJNtEyHACPltrIR4SYRva,s",
    "OycDQy37KCphrrxJcTIBFWlcXvXVm96lV8nBfYDeTIHAzyrRhlbVcTfrgDLf5N+27j,cebMXjnZljpYhuYjRbdDd,9qoek31cXf9LvLkQHKMgwBvE3JT5GtwDjfKJc1oYsCrFMdZg9KCjJNtEyHACPltrIR4SYRva,sg",
    "OycDQy37KCphrrxJcTIBFWlcXvXVm96lV8nBfYDeTIHAzyrRhlbVcTfrgDLf5N+27j,cebMXjnZljpYhuYjRbdDd,9qoek31cXf9LvLkQHKMgwBvE3JT5GtwDjfKJc1oYsCrFMdZg9KCjJNtEyHACPltrIR4SYRva,sgOw",
    "OycDQy37KCphrrxJcTIBFWlcXvXVm96lV8nBfYDeTIHAzyrRhlbVcTfrgDLf5N+27j,cebMXjnZljpYhuYjRbdDd,9qoek31cXf9LvLkQHKMgwBvE3JT5GtwDjfKJc1oYsCrFMdZg9KCjJNtEyHACPltrIR4SYRva,sgO5w",
    "OycDQy37KCphrrxJcTIBFWlcXvXVm96lV8nBfYDeTIHAzyrRhlbVcTfrgDLf5N+27j,cebMXjnZljpYhuYjRbdDd,9qoek31cXf9LvLkQHKMgwBvE3JT5GtwDjfKJc1oYsCrFMdZg9KCjJNtEyHACPltrIR4SYRva,sgO5xJ",
    "OycDQy37KCphrrxJcTIBFWlcXvXVm96lV8nBfYDeTIHAzyrRhlbVcTfrgDLf5N+27j,cebMXjnZljpYhuYjRbdDd,9qoek31cXf9LvLkQHKMgwBvE3JT5GtwDjfKJc1oYsCrFMdZg9KCjJNtEyHACPltrIR4SYRva,sgO5xJ+w",
    "OycDQy37KCphrrxJcTIBFWlcXvXVm96lV8nBfYDeTIHAzyrRhlbVcTfrgDLf5N+27j,cebMXjnZljpYhuYjRbdDd,9qoek31cXf9LvLkQHKMgwBvE3JT5GtwDjfKJc1oYsCrFMdZg9KCjJNtEyHACPltrIR4SYRva,sgO5xJ+04",
    "OycDQy37KCphrrxJcTIBFWlcXvXVm96lV8nBfYDeTIHAzyrRhlbVcTfrgDLf5N+27j,cebMXjnZljpYhuYjRbdDd,9qoek31cXf9LvLkQHKMgwBvE3JT5GtwDjfKJc1oYsCrFMdZg9KCjJNtEyHACPltrIR4SYRva,sgO5xJ+06A",
    "OycDQy37KCphrrxJcTIBFWlcXvXVm96lV8nBfYDeTIHAzyrRhlbVcTfrgDLf5N+27j,cebMXjnZljpYhuYjRbdDd,9qoek31cXf9LvLkQHKMgwBvE3JT5GtwDjfKJc1oYsCrFMdZg9KCjJNtEyHACPltrIR4SYRva,sgO5xJ+06AaQ",
    "OycDQy37KCphrrxJcTIBFWlcXvXVm96lV8nBfYDeTIHAzyrRhlbVcTfrgDLf5N+27j,cebMXjnZljpYhuYjRbdDd,9qoek31cXf9LvLkQHKMgwBvE3JT5GtwDjfKJc1oYsCrFMdZg9KCjJNtEyHACPltrIR4SYRva,sgO5xJ+06AaYI",
    "OycDQy37KCphrrxJcTIBFWlcXvXVm96lV8nBfYDeTIHAzyrRhlbVcTfrgDLf5N+27j,cebMXjnZljpYhuYjRbdDd,9qoek31cXf9LvLkQHKMgwBvE3JT5GtwDjfKJc1oYsCrFMdZg9KCjJNtEyHACPltrIR4SYRva,sgO5xJ+06AaYIl",
    "OycDQy37KCphrrxJcTIBFWlcXvXVm96lV8nBfYDeTIHAzyrRhlbVcTfrgDLf5N+27j,cebMXjnZljpYhuYjRbdDd,9qoek31cXf9LvLkQHKMgwBvE3JT5GtwDjfKJc1oYsCrFMdZg9KCjJNtEyHACPltrIR4SYRva,sgO5xJ+06AaYIlhg",
    "OycDQy37KCphrrxJcTIBFWlcXvXVm96lV8nBfYDeTIHAzyrRhlbVcTfrgDLf5N+27j,cebMXjnZljpYhuYjRbdDd,9qoek31cXf9LvLkQHKMgwBvE3JT5GtwDjfKJc1oYsCrFMdZg9KCjJNtEyHACPltrIR4SYRva,sgO5xJ+06AaYIlhpU",
    "OycDQy37KCphrrxJcTIBFWlcXvXVm96lV8nBfYDeTIHAzyrRhlbVcTfrgDLf5N+27j,cebMXjnZljpYhuYjRbdDd,9qoek31cXf9LvLkQHKMgwBvE3JT5GtwDjfKJc1oYsCrFMdZg9KCjJNtEyHACPltrIR4SYRva,sgO5xJ+06AaYIlhpXV",
    "OycDQy37KCphrrxJcTIBFWlcXvXVm96lV8nBfYDeTIHAzyrRhlbVcTfrgDLf5N+27j,cebMXjnZljpYhuYjRbdDd,9qoek31cXf9LvLkQHKMgwBvE3JT5GtwDjfKJc1oYsCrFMdZg9KCjJNtEyHACPltrIR4SYRva,sgO5xJ+06AaYIlhpXVTQ",
    "OycDQy37KCphrrxJcTIBFWlcXvXVm96lV8nBfYDeTIHAzyrRhlbVcTfrgDLf5N+27j,cebMXjnZljpYhuYjRbdDd,9qoek31cXf9LvLkQHKMgwBvE3JT5GtwDjfKJc1oYsCrFMdZg9KCjJNtEyHACPltrIR4SYRva,sgO5xJ+06AaYIlhpXVTZE",
    "OycDQy37KCphrrxJcTIBFWlcXvXVm96lV8nBfYDeTIHAzyrRhlbVcTfrgDLf5N+27j,cebMXjnZljpYhuYjRbdDd,9qoek31cXf9LvLkQHKMgwBvE3JT5GtwDjfKJc1oYsCrFMdZg9KCjJNtEyHACPltrIR4SYRva,sgO5xJ+06AaYIlhpXVTZHt",
    "OycDQy37KCphrrxJcTIBFWlcXvXVm96lV8nBfYDeTIHAzyrRhlbVcTfrgDLf5N+27j,cebMXjnZljpYhuYjRbdDd,9qoek31cXf9LvLkQHKMgwBvE3JT5GtwDjfKJc1oYsCrFMdZg9KCjJNtEyHACPltrIR4SYRva,sgO5xJ+06AaYIlhpXVTZHt0g",
    "OycDQy37KCphrrxJcTIBFWlcXvXVm96lV8nBfYDeTIHAzyrRhlbVcTfrgDLf5N+27j,cebMXjnZljpYhuYjRbdDd,9qoek31cXf9LvLkQHKMgwBvE3JT5GtwDjfKJc1oYsCrFMdZg9KCjJNtEyHACPltrIR4SYRva,sgO5xJ+06AaYIlhpXVTZHt0nc",
    "OycDQy37KCphrrxJcTIBFWlcXvXVm96lV8nBfYDeTIHAzyrRhlbVcTfrgDLf5N+27j,cebMXjnZljpYhuYjRbdDd,9qoek31cXf9LvLkQHKMgwBvE3JT5GtwDjfKJc1oYsCrFMdZg9KCjJNtEyHACPltrIR4SYRva,sgO5xJ+06AaYIlhpXVTZHt0ncq",
    "OycDQy37KCphrrxJcTIBFWlcXvXVm96lV8nBfYDeTIHAzyrRhlbVcTfrgDLf5N+27j,cebMXjnZljpYhuYjRbdDd,9qoek31cXf9LvLkQHKMgwBvE3JT5GtwDjfKJc1oYsCrFMdZg9KCjJNtEyHACPltrIR4SYRva,sgO5xJ+06AaYIlhpXVTZHt0ncqJA",
    "OycDQy37KCphrrxJcTIBFWlcXvXVm96lV8nBfYDeTIHAzyrRhlbVcTfrgDLf5N+27j,cebMXjnZljpYhuYjRbdDd,9qoek31cXf9LvLkQHKMgwBvE3JT5GtwDjfKJc1oYsCrFMdZg9KCjJNtEyHACPltrIR4SYRva,sgO5xJ+06AaYIlhpXVTZHt0ncqJEA",
    "OycDQy37KCphrrxJcTIBFWlcXvXVm96lV8nBfYDeTIHAzyrRhlbVcTfrgDLf5N+27j,cebMXjnZljpYhuYjRbdDd,9qoek31cXf9LvLkQHKMgwBvE3JT5GtwDjfKJc1oYsCrFMdZg9KCjJNtEyHACPltrIR4SYRva,sgO5xJ+06AaYIlhpXVTZHt0ncqJECK",
    "OycDQy37KCphrrxJcTIBFWlcXvXVm96lV8nBfYDeTIHAzyrRhlbVcTfrgDLf5N+27j,cebMXjnZljpYhuYjRbdDd,9qoek31cXf9LvLkQHKMgwBvE3JT5GtwDjfKJc1oYsCrFMdZg9KCjJNtEyHACPltrIR4SYRva,sgO5xJ+06AaYIlhpXVTZHt0ncqJECK3w",
    "OycDQy37KCphrrxJcTIBFWlcXvXVm96lV8nBfYDeTIHAzyrRhlbVcTfrgDLf5N+27j,cebMXjnZljpYhuYjRbdDd,9qoek31cXf9LvLkQHKMgwBvE3JT5GtwDjfKJc1oYsCrFMdZg9KCjJNtEyHACPltrIR4SYRva,sgO5xJ+06AaYIlhpXVTZHt0ncqJECK300",
    "OycDQy37KCphrrxJcTIBFWlcXvXVm96lV8nBfYDeTIHAzyrRhlbVcTfrgDLf5N+27j,cebMXjnZljpYhuYjRbdDd,9qoek31cXf9LvLkQHKMgwBvE3JT5GtwDjfKJc1oYsCrFMdZg9KCjJNtEyHACPltrIR4SYRva,sgO5xJ+06AaYIlhpXVTZHt0ncqJECK302A",
    "OycDQy37KCphrrxJcTIBFWlcXvXVm96lV8nBfYDeTIHAzyrRhlbVcTfrgDLf5N+27j,cebMXjnZljpYhuYjRbdDd,9qoek31cXf9LvLkQHKMgwBvE3JT5GtwDjfKJc1oYsCrFMdZg9KCjJNtEyHACPltrIR4SYRva,sgO5xJ+06AaYIlhpXVTZHt0ncqJECK302ALQ",
    "OycDQy37KCphrrxJcTIBFWlcXvXVm96lV8nBfYDeTIHAzyrRhlbVcTfrgDLf5N+27j,cebMXjnZljpYhuYjRbdDd,9qoek31cXf9LvLkQHKMgwBvE3JT5GtwDjfKJc1oYsCrFMdZg9KCjJNtEyHACPltrIR4SYRva,sgO5xJ+06AaYIlhpXVTZHt0ncqJECK302ALc0",
    "OycDQy37KCphrrxJcTIBFWlcXvXVm96lV8nBfYDeTIHAzyrRhlbVcTfrgDLf5N+27j,cebMXjnZljpYhuYjRbdDd,9qoek31cXf9LvLkQHKMgwBvE3JT5GtwDjfKJc1oYsCrFMdZg9KCjJNtEyHACPltrIR4SYRva,sgO5xJ+06AaYIlhpXVTZHt0ncqJECK302ALc3V",
    "OycDQy37KCphrrxJcTIBFWlcXvXVm96lV8nBfYDeTIHAzyrRhlbVcTfrgDLf5N+27j,cebMXjnZljpYhuYjRbdDd,9qoek31cXf9LvLkQHKMgwBvE3JT5GtwDjfKJc1oYsCrFMdZg9KCjJNtEyHACPltrIR4SYRva,sgO5xJ+06AaYIlhpXVTZHt0ncqJECK302ALc3VWg",
    "OycDQy37KCphrrxJcTIBFWlcXvXVm96lV8nBfYDeTIHAzyrRhlbVcTfrgDLf5N+27j,cebMXjnZljpYhuYjRbdDd,9qoek31cXf9LvLkQHKMgwBvE3JT5GtwDjfKJc1oYsCrFMdZg9KCjJNtEyHACPltrIR4SYRva,sgO5xJ+06AaYIlhpXVTZHt0ncqJECK302ALc3VWiY",
    "OycDQy37KCphrrxJcTIBFWlcXvXVm96lV8nBfYDeTIHAzyrRhlbVcTfrgDLf5N+27j,cebMXjnZljpYhuYjRbdDd,9qoek31cXf9LvLkQHKMgwBvE3JT5GtwDjfKJc1oYsCrFMdZg9KCjJNtEyHACPltrIR4SYRva,sgO5xJ+06AaYIlhpXVTZHt0ncqJECK302ALc3VWiam",
    "OycDQy37KCphrrxJcTIBFWlcXvXVm96lV8nBfYDeTIHAzyrRhlbVcTfrgDLf5N+27j,cebMXjnZljpYhuYjRbdDd,9qoek31cXf9LvLkQHKMgwBvE3JT5GtwDjfKJc1oYsCrFMdZg9KCjJNtEyHACPltrIR4SYRva,sgO5xJ+06AaYIlhpXVTZHt0ncqJECK302ALc3VWiamcQ",
    "OycDQy37KCphrrxJcTIBFWlcXvXVm96lV8nBfYDeTIHAzyrRhlbVcTfrgDLf5N+27j,cebMXjnZljpYhuYjRbdDd,9qoek31cXf9LvLkQHKMgwBvE3JT5GtwDjfKJc1oYsCrFMdZg9KCjJNtEyHACPltrIR4SYRva,sgO5xJ+06AaYIlhpXVTZHt0ncqJECK302ALc3VWiamcRU",
    "OycDQy37KCphrrxJcTIBFWlcXvXVm96lV8nBfYDeTIHAzyrRhlbVcTfrgDLf5N+27j,cebMXjnZljpYhuYjRbdDd,9qoek31cXf9LvLkQHKMgwBvE3JT5GtwDjfKJc1oYsCrFMdZg9KCjJNtEyHACPltrIR4SYRva,sgO5xJ+06AaYIlhpXVTZHt0ncqJECK302ALc3VWiamcRVC",
    "OycDQy37KCphrrxJcTIBFWlcXvXVm96lV8nBfYDeTIHAzyrRhlbVcTfrgDLf5N+27j,cebMXjnZljpYhuYjRbdDd,9qoek31cXf9LvLkQHKMgwBvE3JT5GtwDjfKJc1oYsCrFMdZg9KCjJNtEyHACPltrIR4SYRva,sgO5xJ+06AaYIlhpXVTZHt0ncqJECK302ALc3VWiamcRVCDg",
    "OycDQy37KCphrrxJcTIBFWlcXvXVm96lV8nBfYDeTIHAzyrRhlbVcTfrgDLf5N+27j,cebMXjnZljpYhuYjRbdDd,9qoek31cXf9LvLkQHKMgwBvE3JT5GtwDjfKJc1oYsCrFMdZg9KCjJNtEyHACPltrIR4SYRva,sgO5xJ+06AaYIlhpXVTZHt0ncqJECK302ALc3VWiamcRVCDj8",
    "OycDQy37KCphrrxJcTIBFWlcXvXVm96lV8nBfYDeTIHAzyrRhlbVcTfrgDLf5N+27j,cebMXjnZljpYhuYjRbdDd,9qoek31cXf9LvLkQHKMgwBvE3JT5GtwDjfKJc1oYsCrFMdZg9KCjJNtEyHACPltrIR4SYRva,sgO5xJ+06AaYIlhpXVTZHt0ncqJECK302ALc3VWiamcRVCDj+y",
    "OycDQy37KCphrrxJcTIBFWlcXvXVm96lV8nBfYDeTIHAzyrRhlbVcTfrgDLf5N+27j,cebMXjnZljpYhuYjRbdDd,9qoek31cXf9LvLkQHKMgwBvE3JT5GtwDjfKJc1oYsCrFMdZg9KCjJNtEyHACPltrIR4SYRva,sgO5xJ+06AaYIlhpXVTZHt0ncqJECK302ALc3VWiamcRVCDj+ycA",
    "OycDQy37KCphrrxJcTIBFWlcXvXVm96lV8nBfYDeTIHAzyrRhlbVcTfrgDLf5N+27j,cebMXjnZljpYhuYjRbdDd,9qoek31cXf9LvLkQHKMgwBvE3JT5GtwDjfKJc1oYsCrFMdZg9KCjJNtEyHACPltrIR4SYRva,sgO5xJ+06AaYIlhpXVTZHt0ncqJECK302ALc3VWiamcRVCDj+ycBQ",
    "OycDQy37KCphrrxJcTIBFWlcXvXVm96lV8nBfYDeTIHAzyrRhlbVcTfrgDLf5N+27j,cebMXjnZljpYhuYjRbdDd,9qoek31cXf9LvLkQHKMgwBvE3JT5GtwDjfKJc1oYsCrFMdZg9KCjJNtEyHACPltrIR4SYRva,sgO5xJ+06AaYIlhpXVTZHt0ncqJECK302ALc3VWiamcRVCDj+ycBQp",
    "OycDQy37KCphrrxJcTIBFWlcXvXVm96lV8nBfYDeTIHAzyrRhlbVcTfrgDLf5N+27j,cebMXjnZljpYhuYjRbdDd,9qoek31cXf9LvLkQHKMgwBvE3JT5GtwDjfKJc1oYsCrFMdZg9KCjJNtEyHACPltrIR4SYRva,sgO5xJ+06AaYIlhpXVTZHt0ncqJECK302ALc3VWiamcRVCDj+ycBQpHw",
    "OycDQy37KCphrrxJcTIBFWlcXvXVm96lV8nBfYDeTIHAzyrRhlbVcTfrgDLf5N+27j,cebMXjnZljpYhuYjRbdDd,9qoek31cXf9LvLkQHKMgwBvE3JT5GtwDjfKJc1oYsCrFMdZg9KCjJNtEyHACPltrIR4SYRva,sgO5xJ+06AaYIlhpXVTZHt0ncqJECK302ALc3VWiamcRVCDj+ycBQpH40",
    "OycDQy37KCphrrxJcTIBFWlcXvXVm96lV8nBfYDeTIHAzyrRhlbVcTfrgDLf5N+27j,cebMXjnZljpYhuYjRbdDd,9qoek31cXf9LvLkQHKMgwBvE3JT5GtwDjfKJc1oYsCrFMdZg9KCjJNtEyHACPltrIR4SYRva,sgO5xJ+06AaYIlhpXVTZHt0ncqJECK302ALc3VWiamcRVCDj+ycBQpH40j",
    "OycDQy37KCphrrxJcTIBFWlcXvXVm96lV8nBfYDeTIHAzyrRhlbVcTfrgDLf5N+27j,cebMXjnZljpYhuYjRbdDd,9qoek31cXf9LvLkQHKMgwBvE3JT5GtwDjfKJc1oYsCrFMdZg9KCjJNtEyHACPltrIR4SYRva,sgO5xJ+06AaYIlhpXVTZHt0ncqJECK302ALc3VWiamcRVCDj+ycBQpH40jLg",
    "OycDQy37KCphrrxJcTIBFWlcXvXVm96lV8nBfYDeTIHAzyrRhlbVcTfrgDLf5N+27j,cebMXjnZljpYhuYjRbdDd,9qoek31cXf9LvLkQHKMgwBvE3JT5GtwDjfKJc1oYsCrFMdZg9KCjJNtEyHACPltrIR4SYRva,sgO5xJ+06AaYIlhpXVTZHt0ncqJECK302ALc3VWiamcRVCDj+ycBQpH40jLsE",
    "OycDQy37KCphrrxJcTIBFWlcXvXVm96lV8nBfYDeTIHAzyrRhlbVcTfrgDLf5N+27j,cebMXjnZljpYhuYjRbdDd,9qoek31cXf9LvLkQHKMgwBvE3JT5GtwDjfKJc1oYsCrFMdZg9KCjJNtEyHACPltrIR4SYRva,sgO5xJ+06AaYIlhpXVTZHt0ncqJECK302ALc3VWiamcRVCDj+ycBQpH40jLsHq",
    "OycDQy37KCphrrxJcTIBFWlcXvXVm96lV8nBfYDeTIHAzyrRhlbVcTfrgDLf5N+27j,cebMXjnZljpYhuYjRbdDd,9qoek31cXf9LvLkQHKMgwBvE3JT5GtwDjfKJc1oYsCrFMdZg9KCjJNtEyHACPltrIR4SYRva,sgO5xJ+06AaYIlhpXVTZHt0ncqJECK302ALc3VWiamcRVCDj+ycBQpH40jLsHqzg",
    "OycDQy37KCphrrxJcTIBFWlcXvXVm96lV8nBfYDeTIHAzyrRhlbVcTfrgDLf5N+27j,cebMXjnZljpYhuYjRbdDd,9qoek31cXf9LvLkQHKMgwBvE3JT5GtwDjfKJc1oYsCrFMdZg9KCjJNtEyHACPltrIR4SYRva,sgO5xJ+06AaYIlhpXVTZHt0ncqJECK302ALc3VWiamcRVCDj+ycBQpH40jLsHqzvk",
    "OycDQy37KCphrrxJcTIBFWlcXvXVm96lV8nBfYDeTIHAzyrRhlbVcTfrgDLf5N+27j,cebMXjnZljpYhuYjRbdDd,9qoek31cXf9LvLkQHKMgwBvE3JT5GtwDjfKJc1oYsCrFMdZg9KCjJNtEyHACPltrIR4SYRva,sgO5xJ+06AaYIlhpXVTZHt0ncqJECK302ALc3VWiamcRVCDj+ycBQpH40jLsHqzvl+",
    "OycDQy37KCphrrxJcTIBFWlcXvXVm96lV8nBfYDeTIHAzyrRhlbVcTfrgDLf5N+27j,cebMXjnZljpYhuYjRbdDd,9qoek31cXf9LvLkQHKMgwBvE3JT5GtwDjfKJc1oYsCrFMdZg9KCjJNtEyHACPltrIR4SYRva,sgO5xJ+06AaYIlhpXVTZHt0ncqJECK302ALc3VWiamcRVCDj+ycBQpH40jLsHqzvl+bA",
    "OycDQy37KCphrrxJcTIBFWlcXvXVm96lV8nBfYDeTIHAzyrRhlbVcTfrgDLf5N+27j,cebMXjnZljpYhuYjRbdDd,9qoek31cXf9LvLkQHKMgwBvE3JT5GtwDjfKJc1oYsCrFMdZg9KCjJNtEyHACPltrIR4SYRva,sgO5xJ+06AaYIlhpXVTZHt0ncqJECK302ALc3VWiamcRVCDj+ycBQpH40jLsHqzvl+bN8",
    "OycDQy37KCphrrxJcTIBFWlcXvXVm96lV8nBfYDeTIHAzyrRhlbVcTfrgDLf5N+27j,cebMXjnZljpYhuYjRbdDd,9qoek31cXf9LvLkQHKMgwBvE3JT5GtwDjfKJc1oYsCrFMdZg9KCjJNtEyHACPltrIR4SYRva,sgO5xJ+06AaYIlhpXVTZHt0ncqJECK302ALc3VWiamcRVCDj+ycBQpH40jLsHqzvl+bN8c",
    "OycDQy37KCphrrxJcTIBFWlcXvXVm96lV8nBfYDeTIHAzyrRhlbVcTfrgDLf5N+27j,cebMXjnZljpYhuYjRbdDd,9qoek31cXf9LvLkQHKMgwBvE3JT5GtwDjfKJc1oYsCrFMdZg9KCjJNtEyHACPltrIR4SYRva,sgO5xJ+06AaYIlhpXVTZHt0ncqJECK302ALc3VWiamcRVCDj+ycBQpH40jLsHqzvl+bN8cow",
    "OycDQy37KCphrrxJcTIBFWlcXvXVm96lV8nBfYDeTIHAzyrRhlbVcTfrgDLf5N+27j,cebMXjnZljpYhuYjRbdDd,9qoek31cXf9LvLkQHKMgwBvE3JT5GtwDjfKJc1oYsCrFMdZg9KCjJNtEyHACPltrIR4SYRva,sgO5xJ+06AaYIlhpXVTZHt0ncqJECK302ALc3VWiamcRVCDj+ycBQpH40jLsHqzvl+bN8co4Q",
    "OycDQy37KCphrrxJcTIBFWlcXvXVm96lV8nBfYDeTIHAzyrRhlbVcTfrgDLf5N+27j,cebMXjnZljpYhuYjRbdDd,9qoek31cXf9LvLkQHKMgwBvE3JT5GtwDjfKJc1oYsCrFMdZg9KCjJNtEyHACPltrIR4SYRva,sgO5xJ+06AaYIlhpXVTZHt0ncqJECK302ALc3VWiamcRVCDj+ycBQpH40jLsHqzvl+bN8co4Qr",
    "OycDQy37KCphrrxJcTIBFWlcXvXVm96lV8nBfYDeTIHAzyrRhlbVcTfrgDLf5N+27j,cebMXjnZljpYhuYjRbdDd,9qoek31cXf9LvLkQHKMgwBvE3JT5GtwDjfKJc1oYsCrFMdZg9KCjJNtEyHACPltrIR4SYRva,sgO5xJ+06AaYIlhpXVTZHt0ncqJECK302ALc3VWiamcRVCDj+ycBQpH40jLsHqzvl+bN8co4QrJA",
    "OycDQy37KCphrrxJcTIBFWlcXvXVm96lV8nBfYDeTIHAzyrRhlbVcTfrgDLf5N+27j,cebMXjnZljpYhuYjRbdDd,9qoek31cXf9LvLkQHKMgwBvE3JT5GtwDjfKJc1oYsCrFMdZg9KCjJNtEyHACPltrIR4SYRva,sgO5xJ+06AaYIlhpXVTZHt0ncqJECK302ALc3VWiamcRVCDj+ycBQpH40jLsHqzvl+bN8co4QrJDU",
    "OycDQy37KCphrrxJcTIBFWlcXvXVm96lV8nBfYDeTIHAzyrRhlbVcTfrgDLf5N+27j,cebMXjnZljpYhuYjRbdDd,9qoek31cXf9LvLkQHKMgwBvE3JT5GtwDjfKJc1oYsCrFMdZg9KCjJNtEyHACPltrIR4SYRva,sgO5xJ+06AaYIlhpXVTZHt0ncqJECK302ALc3VWiamcRVCDj+ycBQpH40jLsHqzvl+bN8co4QrJDWn",
    "OycDQy37KCphrrxJcTIBFWlcXvXVm96lV8nBfYDeTIHAzyrRhlbVcTfrgDLf5N+27j,cebMXjnZljpYhuYjRbdDd,9qoek31cXf9LvLkQHKMgwBvE3JT5GtwDjfKJc1oYsCrFMdZg9KCjJNtEyHACPltrIR4SYRva,sgO5xJ+06AaYIlhpXVTZHt0ncqJECK302ALc3VWiamcRVCDj+ycBQpH40jLsHqzvl+bN8co4QrJDWnYw",
    "OycDQy37KCphrrxJcTIBFWlcXvXVm96lV8nBfYDeTIHAzyrRhlbVcTfrgDLf5N+27j,cebMXjnZljpYhuYjRbdDd,9qoek31cXf9LvLkQHKMgwBvE3JT5GtwDjfKJc1oYsCrFMdZg9KCjJNtEyHACPltrIR4SYRva,sgO5xJ+06AaYIlhpXVTZHt0ncqJECK302ALc3VWiamcRVCDj+ycBQpH40jLsHqzvl+bN8co4QrJDWnY8g",
    "OycDQy37KCphrrxJcTIBFWlcXvXVm96lV8nBfYDeTIHAzyrRhlbVcTfrgDLf5N+27j,cebMXjnZljpYhuYjRbdDd,9qoek31cXf9LvLkQHKMgwBvE3JT5GtwDjfKJc1oYsCrFMdZg9KCjJNtEyHACPltrIR4SYRva,sgO5xJ+06AaYIlhpXVTZHt0ncqJECK302ALc3VWiamcRVCDj+ycBQpH40jLsHqzvl+bN8co4QrJDWnY8gL",
    "OycDQy37KCphrrxJcTIBFWlcXvXVm96lV8nBfYDeTIHAzyrRhlbVcTfrgDLf5N+27j,cebMXjnZljpYhuYjRbdDd,9qoek31cXf9LvLkQHKMgwBvE3JT5GtwDjfKJc1oYsCrFMdZg9KCjJNtEyHACPltrIR4SYRva,sgO5xJ+06AaYIlhpXVTZHt0ncqJECK302ALc3VWiamcRVCDj+ycBQpH40jLsHqzvl+bN8co4QrJDWnY8gLHw",
    "OycDQy37KCphrrxJcTIBFWlcXvXVm96lV8nBfYDeTIHAzyrRhlbVcTfrgDLf5N+27j,cebMXjnZljpYhuYjRbdDd,9qoek31cXf9LvLkQHKMgwBvE3JT5GtwDjfKJc1oYsCrFMdZg9KCjJNtEyHACPltrIR4SYRva,sgO5xJ+06AaYIlhpXVTZHt0ncqJECK302ALc3VWiamcRVCDj+ycBQpH40jLsHqzvl+bN8co4QrJDWnY8gLH4s",
    "OycDQy37KCphrrxJcTIBFWlcXvXVm96lV8nBfYDeTIHAzyrRhlbVcTfrgDLf5N+27j,cebMXjnZljpYhuYjRbdDd,9qoek31cXf9LvLkQHKMgwBvE3JT5GtwDjfKJc1oYsCrFMdZg9KCjJNtEyHACPltrIR4SYRva,sgO5xJ+06AaYIlhpXVTZHt0ncqJECK302ALc3VWiamcRVCDj+ycBQpH40jLsHqzvl+bN8co4QrJDWnY8gLH4u6",
    "OycDQy37KCphrrxJcTIBFWlcXvXVm96lV8nBfYDeTIHAzyrRhlbVcTfrgDLf5N+27j,cebMXjnZljpYhuYjRbdDd,9qoek31cXf9LvLkQHKMgwBvE3JT5GtwDjfKJc1oYsCrFMdZg9KCjJNtEyHACPltrIR4SYRva,sgO5xJ+06AaYIlhpXVTZHt0ncqJECK302ALc3VWiamcRVCDj+ycBQpH40jLsHqzvl+bN8co4QrJDWnY8gLH4u6UQ",
    "OycDQy37KCphrrxJcTIBFWlcXvXVm96lV8nBfYDeTIHAzyrRhlbVcTfrgDLf5N+27j,cebMXjnZljpYhuYjRbdDd,9qoek31cXf9LvLkQHKMgwBvE3JT5GtwDjfKJc1oYsCrFMdZg9KCjJNtEyHACPltrIR4SYRva,sgO5xJ+06AaYIlhpXVTZHt0ncqJECK302ALc3VWiamcRVCDj+ycBQpH40jLsHqzvl+bN8co4QrJDWnY8gLH4u6Ub8",
    "OycDQy37KCphrrxJcTIBFWlcXvXVm96lV8nBfYDeTIHAzyrRhlbVcTfrgDLf5N+27j,cebMXjnZljpYhuYjRbdDd,9qoek31cXf9LvLkQHKMgwBvE3JT5GtwDjfKJc1oYsCrFMdZg9KCjJNtEyHACPltrIR4SYRva,sgO5xJ+06AaYIlhpXVTZHt0ncqJECK302ALc3VWiamcRVCDj+ycBQpH40jLsHqzvl+bN8co4QrJDWnY8gLH4u6Ub,p",
    "OycDQy37KCphrrxJcTIBFWlcXvXVm96lV8nBfYDeTIHAzyrRhlbVcTfrgDLf5N+27j,cebMXjnZljpYhuYjRbdDd,9qoek31cXf9LvLkQHKMgwBvE3JT5GtwDjfKJc1oYsCrFMdZg9KCjJNtEyHACPltrIR4SYRva,sgO5xJ+06AaYIlhpXVTZHt0ncqJECK302ALc3VWiamcRVCDj+ycBQpH40jLsHqzvl+bN8co4QrJDWnY8gLH4u6Ub,pUQ",
    "OycDQy37KCphrrxJcTIBFWlcXvXVm96lV8nBfYDeTIHAzyrRhlbVcTfrgDLf5N+27j,cebMXjnZljpYhuYjRbdDd,9qoek31cXf9LvLkQHKMgwBvE3JT5GtwDjfKJc1oYsCrFMdZg9KCjJNtEyHACPltrIR4SYRva,sgO5xJ+06AaYIlhpXVTZHt0ncqJECK302ALc3VWiamcRVCDj+ycBQpH40jLsHqzvl+bN8co4QrJDWnY8gLH4u6Ub,pUYA",
    "OycDQy37KCphrrxJcTIBFWlcXvXVm96lV8nBfYDeTIHAzyrRhlbVcTfrgDLf5N+27j,cebMXjnZljpYhuYjRbdDd,9qoek31cXf9LvLkQHKMgwBvE3JT5GtwDjfKJc1oYsCrFMdZg9KCjJNtEyHACPltrIR4SYRva,sgO5xJ+06AaYIlhpXVTZHt0ncqJECK302ALc3VWiamcRVCDj+ycBQpH40jLsHqzvl+bN8co4QrJDWnY8gLH4u6Ub,pUYDS",
    "OycDQy37KCphrrxJcTIBFWlcXvXVm96lV8nBfYDeTIHAzyrRhlbVcTfrgDLf5N+27j,cebMXjnZljpYhuYjRbdDd,9qoek31cXf9LvLkQHKMgwBvE3JT5GtwDjfKJc1oYsCrFMdZg9KCjJNtEyHACPltrIR4SYRva,sgO5xJ+06AaYIlhpXVTZHt0ncqJECK302ALc3VWiamcRVCDj+ycBQpH40jLsHqzvl+bN8co4QrJDWnY8gLH4u6Ub,pUYDSIw",
    "OycDQy37KCphrrxJcTIBFWlcXvXVm96lV8nBfYDeTIHAzyrRhlbVcTfrgDLf5N+27j,cebMXjnZljpYhuYjRbdDd,9qoek31cXf9LvLkQHKMgwBvE3JT5GtwDjfKJc1oYsCrFMdZg9KCjJNtEyHACPltrIR4SYRva,sgO5xJ+06AaYIlhpXVTZHt0ncqJECK302ALc3VWiamcRVCDj+ycBQpH40jLsHqzvl+bN8co4QrJDWnY8gLH4u6Ub,pUYDSI7U",
    "OycDQy37KCphrrxJcTIBFWlcXvXVm96lV8nBfYDeTIHAzyrRhlbVcTfrgDLf5N+27j,cebMXjnZljpYhuYjRbdDd,9qoek31cXf9LvLkQHKMgwBvE3JT5GtwDjfKJc1oYsCrFMdZg9KCjJNtEyHACPltrIR4SYRva,sgO5xJ+06AaYIlhpXVTZHt0ncqJECK302ALc3VWiamcRVCDj+ycBQpH40jLsHqzvl+bN8co4QrJDWnY8gLH4u6Ub,pUYDSI7XR",
    "OycDQy37KCphrrxJcTIBFWlcXvXVm96lV8nBfYDeTIHAzyrRhlbVcTfrgDLf5N+27j,cebMXjnZljpYhuYjRbdDd,9qoek31cXf9LvLkQHKMgwBvE3JT5GtwDjfKJc1oYsCrFMdZg9KCjJNtEyHACPltrIR4SYRva,sgO5xJ+06AaYIlhpXVTZHt0ncqJECK302ALc3VWiamcRVCDj+ycBQpH40jLsHqzvl+bN8co4QrJDWnY8gLH4u6Ub,pUYDSI7XRtA",
    "OycDQy37KCphrrxJcTIBFWlcXvXVm96lV8nBfYDeTIHAzyrRhlbVcTfrgDLf5N+27j,cebMXjnZljpYhuYjRbdDd,9qoek31cXf9LvLkQHKMgwBvE3JT5GtwDjfKJc1oYsCrFMdZg9KCjJNtEyHACPltrIR4SYRva,sgO5xJ+06AaYIlhpXVTZHt0ncqJECK302ALc3VWiamcRVCDj+ycBQpH40jLsHqzvl+bN8co4QrJDWnY8gLH4u6Ub,pUYDSI7XRtFk",
    "OycDQy37KCphrrxJcTIBFWlcXvXVm96lV8nBfYDeTIHAzyrRhlbVcTfrgDLf5N+27j,cebMXjnZljpYhuYjRbdDd,9qoek31cXf9LvLkQHKMgwBvE3JT5GtwDjfKJc1oYsCrFMdZg9KCjJNtEyHACPltrIR4SYRva,sgO5xJ+06AaYIlhpXVTZHt0ncqJECK302ALc3VWiamcRVCDj+ycBQpH40jLsHqzvl+bN8co4QrJDWnY8gLH4u6Ub,pUYDSI7XRtFmu",
    "OycDQy37KCphrrxJcTIBFWlcXvXVm96lV8nBfYDeTIHAzyrRhlbVcTfrgDLf5N+27j,cebMXjnZljpYhuYjRbdDd,9qoek31cXf9LvLkQHKMgwBvE3JT5GtwDjfKJc1oYsCrFMdZg9KCjJNtEyHACPltrIR4SYRva,sgO5xJ+06AaYIlhpXVTZHt0ncqJECK302ALc3VWiamcRVCDj+ycBQpH40jLsHqzvl+bN8co4QrJDWnY8gLH4u6Ub,pUYDSI7XRtFmufQ",
    "OycDQy37KCphrrxJcTIBFWlcXvXVm96lV8nBfYDeTIHAzyrRhlbVcTfrgDLf5N+27j,cebMXjnZljpYhuYjRbdDd,9qoek31cXf9LvLkQHKMgwBvE3JT5GtwDjfKJc1oYsCrFMdZg9KCjJNtEyHACPltrIR4SYRva,sgO5xJ+06AaYIlhpXVTZHt0ncqJECK302ALc3VWiamcRVCDj+ycBQpH40jLsHqzvl+bN8co4QrJDWnY8gLH4u6Ub,pUYDSI7XRtFmufTA",
    "OycDQy37KCphrrxJcTIBFWlcXvXVm96lV8nBfYDeTIHAzyrRhlbVcTfrgDLf5N+27j,cebMXjnZljpYhuYjRbdDd,9qoek31cXf9LvLkQHKMgwBvE3JT5GtwDjfKJc1oYsCrFMdZg9KCjJNtEyHACPltrIR4SYRva,sgO5xJ+06AaYIlhpXVTZHt0ncqJECK302ALc3VWiamcRVCDj+ycBQpH40jLsHqzvl+bN8co4QrJDWnY8gLH4u6Ub,pUYDSI7XRtFmufTAd",
    "OycDQy37KCphrrxJcTIBFWlcXvXVm96lV8nBfYDeTIHAzyrRhlbVcTfrgDLf5N+27j,cebMXjnZljpYhuYjRbdDd,9qoek31cXf9LvLkQHKMgwBvE3JT5GtwDjfKJc1oYsCrFMdZg9KCjJNtEyHACPltrIR4SYRva,sgO5xJ+06AaYIlhpXVTZHt0ncqJECK302ALc3VWiamcRVCDj+ycBQpH40jLsHqzvl+bN8co4QrJDWnY8gLH4u6Ub,pUYDSI7XRtFmufTAdAA",
    "OycDQy37KCphrrxJcTIBFWlcXvXVm96lV8nBfYDeTIHAzyrRhlbVcTfrgDLf5N+27j,cebMXjnZljpYhuYjRbdDd,9qoek31cXf9LvLkQHKMgwBvE3JT5GtwDjfKJc1oYsCrFMdZg9KCjJNtEyHACPltrIR4SYRva,sgO5xJ+06AaYIlhpXVTZHt0ncqJECK302ALc3VWiamcRVCDj+ycBQpH40jLsHqzvl+bN8co4QrJDWnY8gLH4u6Ub,pUYDSI7XRtFmufTAdABw",
    "OycDQy37KCphrrxJcTIBFWlcXvXVm96lV8nBfYDeTIHAzyrRhlbVcTfrgDLf5N+27j,cebMXjnZljpYhuYjRbdDd,9qoek31cXf9LvLkQHKMgwBvE3JT5GtwDjfKJc1oYsCrFMdZg9KCjJNtEyHACPltrIR4SYRva,sgO5xJ+06AaYIlhpXVTZHt0ncqJECK302ALc3VWiamcRVCDj+ycBQpH40jLsHqzvl+bN8co4QrJDWnY8gLH4u6Ub,pUYDSI7XRtFmufTAdABzY",
    "OycDQy37KCphrrxJcTIBFWlcXvXVm96lV8nBfYDeTIHAzyrRhlbVcTfrgDLf5N+27j,cebMXjnZljpYhuYjRbdDd,9qoek31cXf9LvLkQHKMgwBvE3JT5GtwDjfKJc1oYsCrFMdZg9KCjJNtEyHACPltrIR4SYRva,sgO5xJ+06AaYIlhpXVTZHt0ncqJECK302ALc3VWiamcRVCDj+ycBQpH40jLsHqzvl+bN8co4QrJDWnY8gLH4u6Ub,pUYDSI7XRtFmufTAdABzYcA",
    "OycDQy37KCphrrxJcTIBFWlcXvXVm96lV8nBfYDeTIHAzyrRhlbVcTfrgDLf5N+27j,cebMXjnZljpYhuYjRbdDd,9qoek31cXf9LvLkQHKMgwBvE3JT5GtwDjfKJc1oYsCrFMdZg9KCjJNtEyHACPltrIR4SYRva,sgO5xJ+06AaYIlhpXVTZHt0ncqJECK302ALc3VWiamcRVCDj+ycBQpH40jLsHqzvl+bN8co4QrJDWnY8gLH4u6Ub,pUYDSI7XRtFmufTAdABzYcGw",
    "OycDQy37KCphrrxJcTIBFWlcXvXVm96lV8nBfYDeTIHAzyrRhlbVcTfrgDLf5N+27j,cebMXjnZljpYhuYjRbdDd,9qoek31cXf9LvLkQHKMgwBvE3JT5GtwDjfKJc1oYsCrFMdZg9KCjJNtEyHACPltrIR4SYRva,sgO5xJ+06AaYIlhpXVTZHt0ncqJECK302ALc3VWiamcRVCDj+ycBQpH40jLsHqzvl+bN8co4QrJDWnY8gLH4u6Ub,pUYDSI7XRtFmufTAdABzYcGwW",
    "OycDQy37KCphrrxJcTIBFWlcXvXVm96lV8nBfYDeTIHAzyrRhlbVcTfrgDLf5N+27j,cebMXjnZljpYhuYjRbdDd,9qoek31cXf9LvLkQHKMgwBvE3JT5GtwDjfKJc1oYsCrFMdZg9KCjJNtEyHACPltrIR4SYRva,sgO5xJ+06AaYIlhpXVTZHt0ncqJECK302ALc3VWiamcRVCDj+ycBQpH40jLsHqzvl+bN8co4QrJDWnY8gLH4u6Ub,pUYDSI7XRtFmufTAdABzYcGwWcQ",
    "OycDQy37KCphrrxJcTIBFWlcXvXVm96lV8nBfYDeTIHAzyrRhlbVcTfrgDLf5N+27j,cebMXjnZljpYhuYjRbdDd,9qoek31cXf9LvLkQHKMgwBvE3JT5GtwDjfKJc1oYsCrFMdZg9KCjJNtEyHACPltrIR4SYRva,sgO5xJ+06AaYIlhpXVTZHt0ncqJECK302ALc3VWiamcRVCDj+ycBQpH40jLsHqzvl+bN8co4QrJDWnY8gLH4u6Ub,pUYDSI7XRtFmufTAdABzYcGwWccc",
    "OycDQy37KCphrrxJcTIBFWlcXvXVm96lV8nBfYDeTIHAzyrRhlbVcTfrgDLf5N+27j,cebMXjnZljpYhuYjRbdDd,9qoek31cXf9LvLkQHKMgwBvE3JT5GtwDjfKJc1oYsCrFMdZg9KCjJNtEyHACPltrIR4SYRva,sgO5xJ+06AaYIlhpXVTZHt0ncqJECK302ALc3VWiamcRVCDj+ycBQpH40jLsHqzvl+bN8co4QrJDWnY8gLH4u6Ub,pUYDSI7XRtFmufTAdABzYcGwWccdW",
    "OycDQy37KCphrrxJcTIBFWlcXvXVm96lV8nBfYDeTIHAzyrRhlbVcTfrgDLf5N+27j,cebMXjnZljpYhuYjRbdDd,9qoek31cXf9LvLkQHKMgwBvE3JT5GtwDjfKJc1oYsCrFMdZg9KCjJNtEyHACPltrIR4SYRva,sgO5xJ+06AaYIlhpXVTZHt0ncqJECK302ALc3VWiamcRVCDj+ycBQpH40jLsHqzvl+bN8co4QrJDWnY8gLH4u6Ub,pUYDSI7XRtFmufTAdABzYcGwWccdWCA",
    "OycDQy37KCphrrxJcTIBFWlcXvXVm96lV8nBfYDeTIHAzyrRhlbVcTfrgDLf5N+27j,cebMXjnZljpYhuYjRbdDd,9qoek31cXf9LvLkQHKMgwBvE3JT5GtwDjfKJc1oYsCrFMdZg9KCjJNtEyHACPltrIR4SYRva,sgO5xJ+06AaYIlhpXVTZHt0ncqJECK302ALc3VWiamcRVCDj+ycBQpH40jLsHqzvl+bN8co4QrJDWnY8gLH4u6Ub,pUYDSI7XRtFmufTAdABzYcGwWccdWCP4",
    "OycDQy37KCphrrxJcTIBFWlcXvXVm96lV8nBfYDeTIHAzyrRhlbVcTfrgDLf5N+27j,cebMXjnZljpYhuYjRbdDd,9qoek31cXf9LvLkQHKMgwBvE3JT5GtwDjfKJc1oYsCrFMdZg9KCjJNtEyHACPltrIR4SYRva,sgO5xJ+06AaYIlhpXVTZHt0ncqJECK302ALc3VWiamcRVCDj+ycBQpH40jLsHqzvl+bN8co4QrJDWnY8gLH4u6Ub,pUYDSI7XRtFmufTAdABzYcGwWccdWCP6B",
    "OycDQy37KCphrrxJcTIBFWlcXvXVm96lV8nBfYDeTIHAzyrRhlbVcTfrgDLf5N+27j,cebMXjnZljpYhuYjRbdDd,9qoek31cXf9LvLkQHKMgwBvE3JT5GtwDjfKJc1oYsCrFMdZg9KCjJNtEyHACPltrIR4SYRva,sgO5xJ+06AaYIlhpXVTZHt0ncqJECK302ALc3VWiamcRVCDj+ycBQpH40jLsHqzvl+bN8co4QrJDWnY8gLH4u6Ub,pUYDSI7XRtFmufTAdABzYcGwWccdWCP6Brg",
    "OycDQy37KCphrrxJcTIBFWlcXvXVm96lV8nBfYDeTIHAzyrRhlbVcTfrgDLf5N+27j,cebMXjnZljpYhuYjRbdDd,9qoek31cXf9LvLkQHKMgwBvE3JT5GtwDjfKJc1oYsCrFMdZg9KCjJNtEyHACPltrIR4SYRva,sgO5xJ+06AaYIlhpXVTZHt0ncqJECK302ALc3VWiamcRVCDj+ycBQpH40jLsHqzvl+bN8co4QrJDWnY8gLH4u6Ub,pUYDSI7XRtFmufTAdABzYcGwWccdWCP6Brvs",
    "OycDQy37KCphrrxJcTIBFWlcXvXVm96lV8nBfYDeTIHAzyrRhlbVcTfrgDLf5N+27j,cebMXjnZljpYhuYjRbdDd,9qoek31cXf9LvLkQHKMgwBvE3JT5GtwDjfKJc1oYsCrFMdZg9KCjJNtEyHACPltrIR4SYRva,sgO5xJ+06AaYIlhpXVTZHt0ncqJECK302ALc3VWiamcRVCDj+ycBQpH40jLsHqzvl+bN8co4QrJDWnY8gLH4u6Ub,pUYDSI7XRtFmufTAdABzYcGwWccdWCP6Brvvg",
    "OycDQy37KCphrrxJcTIBFWlcXvXVm96lV8nBfYDeTIHAzyrRhlbVcTfrgDLf5N+27j,cebMXjnZljpYhuYjRbdDd,9qoek31cXf9LvLkQHKMgwBvE3JT5GtwDjfKJc1oYsCrFMdZg9KCjJNtEyHACPltrIR4SYRva,sgO5xJ+06AaYIlhpXVTZHt0ncqJECK302ALc3VWiamcRVCDj+ycBQpH40jLsHqzvl+bN8co4QrJDWnY8gLH4u6Ub,pUYDSI7XRtFmufTAdABzYcGwWccdWCP6Brvvgkg",
    "OycDQy37KCphrrxJcTIBFWlcXvXVm96lV8nBfYDeTIHAzyrRhlbVcTfrgDLf5N+27j,cebMXjnZljpYhuYjRbdDd,9qoek31cXf9LvLkQHKMgwBvE3JT5GtwDjfKJc1oYsCrFMdZg9KCjJNtEyHACPltrIR4SYRva,sgO5xJ+06AaYIlhpXVTZHt0ncqJECK302ALc3VWiamcRVCDj+ycBQpH40jLsHqzvl+bN8co4QrJDWnY8gLH4u6Ub,pUYDSI7XRtFmufTAdABzYcGwWccdWCP6Brvvgktg",
    "OycDQy37KCphrrxJcTIBFWlcXvXVm96lV8nBfYDeTIHAzyrRhlbVcTfrgDLf5N+27j,cebMXjnZljpYhuYjRbdDd,9qoek31cXf9LvLkQHKMgwBvE3JT5GtwDjfKJc1oYsCrFMdZg9KCjJNtEyHACPltrIR4SYRva,sgO5xJ+06AaYIlhpXVTZHt0ncqJECK302ALc3VWiamcRVCDj+ycBQpH40jLsHqzvl+bN8co4QrJDWnY8gLH4u6Ub,pUYDSI7XRtFmufTAdABzYcGwWccdWCP6Brvvgktjb",
    "OycDQy37KCphrrxJcTIBFWlcXvXVm96lV8nBfYDeTIHAzyrRhlbVcTfrgDLf5N+27j,cebMXjnZljpYhuYjRbdDd,9qoek31cXf9LvLkQHKMgwBvE3JT5GtwDjfKJc1oYsCrFMdZg9KCjJNtEyHACPltrIR4SYRva,sgO5xJ+06AaYIlhpXVTZHt0ncqJECK302ALc3VWiamcRVCDj+ycBQpH40jLsHqzvl+bN8co4QrJDWnY8gLH4u6Ub,pUYDSI7XRtFmufTAdABzYcGwWccdWCP6BrvvgktjbuQ",
    "OycDQy37KCphrrxJcTIBFWlcXvXVm96lV8nBfYDeTIHAzyrRhlbVcTfrgDLf5N+27j,cebMXjnZljpYhuYjRbdDd,9qoek31cXf9LvLkQHKMgwBvE3JT5GtwDjfKJc1oYsCrFMdZg9KCjJNtEyHACPltrIR4SYRva,sgO5xJ+06AaYIlhpXVTZHt0ncqJECK302ALc3VWiamcRVCDj+ycBQpH40jLsHqzvl+bN8co4QrJDWnY8gLH4u6Ub,pUYDSI7XRtFmufTAdABzYcGwWccdWCP6BrvvgktjbuVc",
    "OycDQy37KCphrrxJcTIBFWlcXvXVm96lV8nBfYDeTIHAzyrRhlbVcTfrgDLf5N+27j,cebMXjnZljpYhuYjRbdDd,9qoek31cXf9LvLkQHKMgwBvE3JT5GtwDjfKJc1oYsCrFMdZg9KCjJNtEyHACPltrIR4SYRva,sgO5xJ+06AaYIlhpXVTZHt0ncqJECK302ALc3VWiamcRVCDj+ycBQpH40jLsHqzvl+bN8co4QrJDWnY8gLH4u6Ub,pUYDSI7XRtFmufTAdABzYcGwWccdWCP6BrvvgktjbuVd8",
    "OycDQy37KCphrrxJcTIBFWlcXvXVm96lV8nBfYDeTIHAzyrRhlbVcTfrgDLf5N+27j,cebMXjnZljpYhuYjRbdDd,9qoek31cXf9LvLkQHKMgwBvE3JT5GtwDjfKJc1oYsCrFMdZg9KCjJNtEyHACPltrIR4SYRva,sgO5xJ+06AaYIlhpXVTZHt0ncqJECK302ALc3VWiamcRVCDj+ycBQpH40jLsHqzvl+bN8co4QrJDWnY8gLH4u6Ub,pUYDSI7XRtFmufTAdABzYcGwWccdWCP6BrvvgktjbuVd8mQ",
    "OycDQy37KCphrrxJcTIBFWlcXvXVm96lV8nBfYDeTIHAzyrRhlbVcTfrgDLf5N+27j,cebMXjnZljpYhuYjRbdDd,9qoek31cXf9LvLkQHKMgwBvE3JT5GtwDjfKJc1oYsCrFMdZg9KCjJNtEyHACPltrIR4SYRva,sgO5xJ+06AaYIlhpXVTZHt0ncqJECK302ALc3VWiamcRVCDj+ycBQpH40jLsHqzvl+bN8co4QrJDWnY8gLH4u6Ub,pUYDSI7XRtFmufTAdABzYcGwWccdWCP6BrvvgktjbuVd8mcs",
    "OycDQy37KCphrrxJcTIBFWlcXvXVm96lV8nBfYDeTIHAzyrRhlbVcTfrgDLf5N+27j,cebMXjnZljpYhuYjRbdDd,9qoek31cXf9LvLkQHKMgwBvE3JT5GtwDjfKJc1oYsCrFMdZg9KCjJNtEyHACPltrIR4SYRva,sgO5xJ+06AaYIlhpXVTZHt0ncqJECK302ALc3VWiamcRVCDj+ycBQpH40jLsHqzvl+bN8co4QrJDWnY8gLH4u6Ub,pUYDSI7XRtFmufTAdABzYcGwWccdWCP6BrvvgktjbuVd8mctC",
    "OycDQy37KCphrrxJcTIBFWlcXvXVm96lV8nBfYDeTIHAzyrRhlbVcTfrgDLf5N+27j,cebMXjnZljpYhuYjRbdDd,9qoek31cXf9LvLkQHKMgwBvE3JT5GtwDjfKJc1oYsCrFMdZg9KCjJNtEyHACPltrIR4SYRva,sgO5xJ+06AaYIlhpXVTZHt0ncqJECK302ALc3VWiamcRVCDj+ycBQpH40jLsHqzvl+bN8co4QrJDWnY8gLH4u6Ub,pUYDSI7XRtFmufTAdABzYcGwWccdWCP6BrvvgktjbuVd8mctC7w",
    "OycDQy37KCphrrxJcTIBFWlcXvXVm96lV8nBfYDeTIHAzyrRhlbVcTfrgDLf5N+27j,cebMXjnZljpYhuYjRbdDd,9qoek31cXf9LvLkQHKMgwBvE3JT5GtwDjfKJc1oYsCrFMdZg9KCjJNtEyHACPltrIR4SYRva,sgO5xJ+06AaYIlhpXVTZHt0ncqJECK302ALc3VWiamcRVCDj+ycBQpH40jLsHqzvl+bN8co4QrJDWnY8gLH4u6Ub,pUYDSI7XRtFmufTAdABzYcGwWccdWCP6BrvvgktjbuVd8mctC7,w",
    "OycDQy37KCphrrxJcTIBFWlcXvXVm96lV8nBfYDeTIHAzyrRhlbVcTfrgDLf5N+27j,cebMXjnZljpYhuYjRbdDd,9qoek31cXf9LvLkQHKMgwBvE3JT5GtwDjfKJc1oYsCrFMdZg9KCjJNtEyHACPltrIR4SYRva,sgO5xJ+06AaYIlhpXVTZHt0ncqJECK302ALc3VWiamcRVCDj+ycBQpH40jLsHqzvl+bN8co4QrJDWnY8gLH4u6Ub,pUYDSI7XRtFmufTAdABzYcGwWccdWCP6BrvvgktjbuVd8mctC7,yz",
    "OycDQy37KCphrrxJcTIBFWlcXvXVm96lV8nBfYDeTIHAzyrRhlbVcTfrgDLf5N+27j,cebMXjnZljpYhuYjRbdDd,9qoek31cXf9LvLkQHKMgwBvE3JT5GtwDjfKJc1oYsCrFMdZg9KCjJNtEyHACPltrIR4SYRva,sgO5xJ+06AaYIlhpXVTZHt0ncqJECK302ALc3VWiamcRVCDj+ycBQpH40jLsHqzvl+bN8co4QrJDWnY8gLH4u6Ub,pUYDSI7XRtFmufTAdABzYcGwWccdWCP6BrvvgktjbuVd8mctC7,yzVg",
    "OycDQy37KCphrrxJcTIBFWlcXvXVm96lV8nBfYDeTIHAzyrRhlbVcTfrgDLf5N+27j,cebMXjnZljpYhuYjRbdDd,9qoek31cXf9LvLkQHKMgwBvE3JT5GtwDjfKJc1oYsCrFMdZg9KCjJNtEyHACPltrIR4SYRva,sgO5xJ+06AaYIlhpXVTZHt0ncqJECK302ALc3VWiamcRVCDj+ycBQpH40jLsHqzvl+bN8co4QrJDWnY8gLH4u6Ub,pUYDSI7XRtFmufTAdABzYcGwWccdWCP6BrvvgktjbuVd8mctC7,yzVh4",
    "OycDQy37KCphrrxJcTIBFWlcXvXVm96lV8nBfYDeTIHAzyrRhlbVcTfrgDLf5N+27j,cebMXjnZljpYhuYjRbdDd,9qoek31cXf9LvLkQHKMgwBvE3JT5GtwDjfKJc1oYsCrFMdZg9KCjJNtEyHACPltrIR4SYRva,sgO5xJ+06AaYIlhpXVTZHt0ncqJECK302ALc3VWiamcRVCDj+ycBQpH40jLsHqzvl+bN8co4QrJDWnY8gLH4u6Ub,pUYDSI7XRtFmufTAdABzYcGwWccdWCP6BrvvgktjbuVd8mctC7,yzVh7R",
    "OycDQy37KCphrrxJcTIBFWlcXvXVm96lV8nBfYDeTIHAzyrRhlbVcTfrgDLf5N+27j,cebMXjnZljpYhuYjRbdDd,9qoek31cXf9LvLkQHKMgwBvE3JT5GtwDjfKJc1oYsCrFMdZg9KCjJNtEyHACPltrIR4SYRva,sgO5xJ+06AaYIlhpXVTZHt0ncqJECK302ALc3VWiamcRVCDj+ycBQpH40jLsHqzvl+bN8co4QrJDWnY8gLH4u6Ub,pUYDSI7XRtFmufTAdABzYcGwWccdWCP6BrvvgktjbuVd8mctC7,yzVh7RQg",
    "OycDQy37KCphrrxJcTIBFWlcXvXVm96lV8nBfYDeTIHAzyrRhlbVcTfrgDLf5N+27j,cebMXjnZljpYhuYjRbdDd,9qoek31cXf9LvLkQHKMgwBvE3JT5GtwDjfKJc1oYsCrFMdZg9KCjJNtEyHACPltrIR4SYRva,sgO5xJ+06AaYIlhpXVTZHt0ncqJECK302ALc3VWiamcRVCDj+ycBQpH40jLsHqzvl+bN8co4QrJDWnY8gLH4u6Ub,pUYDSI7XRtFmufTAdABzYcGwWccdWCP6BrvvgktjbuVd8mctC7,yzVh7RQtM",
    "OycDQy37KCphrrxJcTIBFWlcXvXVm96lV8nBfYDeTIHAzyrRhlbVcTfrgDLf5N+27j,cebMXjnZljpYhuYjRbdDd,9qoek31cXf9LvLkQHKMgwBvE3JT5GtwDjfKJc1oYsCrFMdZg9KCjJNtEyHACPltrIR4SYRva,sgO5xJ+06AaYIlhpXVTZHt0ncqJECK302ALc3VWiamcRVCDj+ycBQpH40jLsHqzvl+bN8co4QrJDWnY8gLH4u6Ub,pUYDSI7XRtFmufTAdABzYcGwWccdWCP6BrvvgktjbuVd8mctC7,yzVh7RQtMM",
    "OycDQy37KCphrrxJcTIBFWlcXvXVm96lV8nBfYDeTIHAzyrRhlbVcTfrgDLf5N+27j,cebMXjnZljpYhuYjRbdDd,9qoek31cXf9LvLkQHKMgwBvE3JT5GtwDjfKJc1oYsCrFMdZg9KCjJNtEyHACPltrIR4SYRva,sgO5xJ+06AaYIlhpXVTZHt0ncqJECK302ALc3VWiamcRVCDj+ycBQpH40jLsHqzvl+bN8co4QrJDWnY8gLH4u6Ub,pUYDSI7XRtFmufTAdABzYcGwWccdWCP6BrvvgktjbuVd8mctC7,yzVh7RQtMMGA",
    "OycDQy37KCphrrxJcTIBFWlcXvXVm96lV8nBfYDeTIHAzyrRhlbVcTfrgDLf5N+27j,cebMXjnZljpYhuYjRbdDd,9qoek31cXf9LvLkQHKMgwBvE3JT5GtwDjfKJc1oYsCrFMdZg9KCjJNtEyHACPltrIR4SYRva,sgO5xJ+06AaYIlhpXVTZHt0ncqJECK302ALc3VWiamcRVCDj+ycBQpH40jLsHqzvl+bN8co4QrJDWnY8gLH4u6Ub,pUYDSI7XRtFmufTAdABzYcGwWccdWCP6BrvvgktjbuVd8mctC7,yzVh7RQtMMGLM",
    "OycDQy37KCphrrxJcTIBFWlcXvXVm96lV8nBfYDeTIHAzyrRhlbVcTfrgDLf5N+27j,cebMXjnZljpYhuYjRbdDd,9qoek31cXf9LvLkQHKMgwBvE3JT5GtwDjfKJc1oYsCrFMdZg9KCjJNtEyHACPltrIR4SYRva,sgO5xJ+06AaYIlhpXVTZHt0ncqJECK302ALc3VWiamcRVCDj+ycBQpH40jLsHqzvl+bN8co4QrJDWnY8gLH4u6Ub,pUYDSI7XRtFmufTAdABzYcGwWccdWCP6BrvvgktjbuVd8mctC7,yzVh7RQtMMGLPu",
    "OycDQy37KCphrrxJcTIBFWlcXvXVm96lV8nBfYDeTIHAzyrRhlbVcTfrgDLf5N+27j,cebMXjnZljpYhuYjRbdDd,9qoek31cXf9LvLkQHKMgwBvE3JT5GtwDjfKJc1oYsCrFMdZg9KCjJNtEyHACPltrIR4SYRva,sgO5xJ+06AaYIlhpXVTZHt0ncqJECK302ALc3VWiamcRVCDj+ycBQpH40jLsHqzvl+bN8co4QrJDWnY8gLH4u6Ub,pUYDSI7XRtFmufTAdABzYcGwWccdWCP6BrvvgktjbuVd8mctC7,yzVh7RQtMMGLPurw",
    "OycDQy37KCphrrxJcTIBFWlcXvXVm96lV8nBfYDeTIHAzyrRhlbVcTfrgDLf5N+27j,cebMXjnZljpYhuYjRbdDd,9qoek31cXf9LvLkQHKMgwBvE3JT5GtwDjfKJc1oYsCrFMdZg9KCjJNtEyHACPltrIR4SYRva,sgO5xJ+06AaYIlhpXVTZHt0ncqJECK302ALc3VWiamcRVCDj+ycBQpH40jLsHqzvl+bN8co4QrJDWnY8gLH4u6Ub,pUYDSI7XRtFmufTAdABzYcGwWccdWCP6BrvvgktjbuVd8mctC7,yzVh7RQtMMGLPurxo",
    "OycDQy37KCphrrxJcTIBFWlcXvXVm96lV8nBfYDeTIHAzyrRhlbVcTfrgDLf5N+27j,cebMXjnZljpYhuYjRbdDd,9qoek31cXf9LvLkQHKMgwBvE3JT5GtwDjfKJc1oYsCrFMdZg9KCjJNtEyHACPltrIR4SYRva,sgO5xJ+06AaYIlhpXVTZHt0ncqJECK302ALc3VWiamcRVCDj+ycBQpH40jLsHqzvl+bN8co4QrJDWnY8gLH4u6Ub,pUYDSI7XRtFmufTAdABzYcGwWccdWCP6BrvvgktjbuVd8mctC7,yzVh7RQtMMGLPurxp3",
    "OycDQy37KCphrrxJcTIBFWlcXvXVm96lV8nBfYDeTIHAzyrRhlbVcTfrgDLf5N+27j,cebMXjnZljpYhuYjRbdDd,9qoek31cXf9LvLkQHKMgwBvE3JT5GtwDjfKJc1oYsCrFMdZg9KCjJNtEyHACPltrIR4SYRva,sgO5xJ+06AaYIlhpXVTZHt0ncqJECK302ALc3VWiamcRVCDj+ycBQpH40jLsHqzvl+bN8co4QrJDWnY8gLH4u6Ub,pUYDSI7XRtFmufTAdABzYcGwWccdWCP6BrvvgktjbuVd8mctC7,yzVh7RQtMMGLPurxp3qA",
    "OycDQy37KCphrrxJcTIBFWlcXvXVm96lV8nBfYDeTIHAzyrRhlbVcTfrgDLf5N+27j,cebMXjnZljpYhuYjRbdDd,9qoek31cXf9LvLkQHKMgwBvE3JT5GtwDjfKJc1oYsCrFMdZg9KCjJNtEyHACPltrIR4SYRva,sgO5xJ+06AaYIlhpXVTZHt0ncqJECK302ALc3VWiamcRVCDj+ycBQpH40jLsHqzvl+bN8co4QrJDWnY8gLH4u6Ub,pUYDSI7XRtFmufTAdABzYcGwWccdWCP6BrvvgktjbuVd8mctC7,yzVh7RQtMMGLPurxp3qFI",
    "OycDQy37KCphrrxJcTIBFWlcXvXVm96lV8nBfYDeTIHAzyrRhlbVcTfrgDLf5N+27j,cebMXjnZljpYhuYjRbdDd,9qoek31cXf9LvLkQHKMgwBvE3JT5GtwDjfKJc1oYsCrFMdZg9KCjJNtEyHACPltrIR4SYRva,sgO5xJ+06AaYIlhpXVTZHt0ncqJECK302ALc3VWiamcRVCDj+ycBQpH40jLsHqzvl+bN8co4QrJDWnY8gLH4u6Ub,pUYDSI7XRtFmufTAdABzYcGwWccdWCP6BrvvgktjbuVd8mctC7,yzVh7RQtMMGLPurxp3qFI8",
    "OycDQy37KCphrrxJcTIBFWlcXvXVm96lV8nBfYDeTIHAzyrRhlbVcTfrgDLf5N+27j,cebMXjnZljpYhuYjRbdDd,9qoek31cXf9LvLkQHKMgwBvE3JT5GtwDjfKJc1oYsCrFMdZg9KCjJNtEyHACPltrIR4SYRva,sgO5xJ+06AaYIlhpXVTZHt0ncqJECK302ALc3VWiamcRVCDj+ycBQpH40jLsHqzvl+bN8co4QrJDWnY8gLH4u6Ub,pUYDSI7XRtFmufTAdABzYcGwWccdWCP6BrvvgktjbuVd8mctC7,yzVh7RQtMMGLPurxp3qFI8ng",
    "OycDQy37KCphrrxJcTIBFWlcXvXVm96lV8nBfYDeTIHAzyrRhlbVcTfrgDLf5N+27j,cebMXjnZljpYhuYjRbdDd,9qoek31cXf9LvLkQHKMgwBvE3JT5GtwDjfKJc1oYsCrFMdZg9KCjJNtEyHACPltrIR4SYRva,sgO5xJ+06AaYIlhpXVTZHt0ncqJECK302ALc3VWiamcRVCDj+ycBQpH40jLsHqzvl+bN8co4QrJDWnY8gLH4u6Ub,pUYDSI7XRtFmufTAdABzYcGwWccdWCP6BrvvgktjbuVd8mctC7,yzVh7RQtMMGLPurxp3qFI8ns0",
    "OycDQy37KCphrrxJcTIBFWlcXvXVm96lV8nBfYDeTIHAzyrRhlbVcTfrgDLf5N+27j,cebMXjnZljpYhuYjRbdDd,9qoek31cXf9LvLkQHKMgwBvE3JT5GtwDjfKJc1oYsCrFMdZg9KCjJNtEyHACPltrIR4SYRva,sgO5xJ+06AaYIlhpXVTZHt0ncqJECK302ALc3VWiamcRVCDj+ycBQpH40jLsHqzvl+bN8co4QrJDWnY8gLH4u6Ub,pUYDSI7XRtFmufTAdABzYcGwWccdWCP6BrvvgktjbuVd8mctC7,yzVh7RQtMMGLPurxp3qFI8ns3e",
    "OycDQy37KCphrrxJcTIBFWlcXvXVm96lV8nBfYDeTIHAzyrRhlbVcTfrgDLf5N+27j,cebMXjnZljpYhuYjRbdDd,9qoek31cXf9LvLkQHKMgwBvE3JT5GtwDjfKJc1oYsCrFMdZg9KCjJNtEyHACPltrIR4SYRva,sgO5xJ+06AaYIlhpXVTZHt0ncqJECK302ALc3VWiamcRVCDj+ycBQpH40jLsHqzvl+bN8co4QrJDWnY8gLH4u6Ub,pUYDSI7XRtFmufTAdABzYcGwWccdWCP6BrvvgktjbuVd8mctC7,yzVh7RQtMMGLPurxp3qFI8ns3eIQ",
    "OycDQy37KCphrrxJcTIBFWlcXvXVm96lV8nBfYDeTIHAzyrRhlbVcTfrgDLf5N+27j,cebMXjnZljpYhuYjRbdDd,9qoek31cXf9LvLkQHKMgwBvE3JT5GtwDjfKJc1oYsCrFMdZg9KCjJNtEyHACPltrIR4SYRva,sgO5xJ+06AaYIlhpXVTZHt0ncqJECK302ALc3VWiamcRVCDj+ycBQpH40jLsHqzvl+bN8co4QrJDWnY8gLH4u6Ub,pUYDSI7XRtFmufTAdABzYcGwWccdWCP6BrvvgktjbuVd8mctC7,yzVh7RQtMMGLPurxp3qFI8ns3eITQ",
    "OycDQy37KCphrrxJcTIBFWlcXvXVm96lV8nBfYDeTIHAzyrRhlbVcTfrgDLf5N+27j,cebMXjnZljpYhuYjRbdDd,9qoek31cXf9LvLkQHKMgwBvE3JT5GtwDjfKJc1oYsCrFMdZg9KCjJNtEyHACPltrIR4SYRva,sgO5xJ+06AaYIlhpXVTZHt0ncqJECK302ALc3VWiamcRVCDj+ycBQpH40jLsHqzvl+bN8co4QrJDWnY8gLH4u6Ub,pUYDSI7XRtFmufTAdABzYcGwWccdWCP6BrvvgktjbuVd8mctC7,yzVh7RQtMMGLPurxp3qFI8ns3eITQ+",
    "OycDQy37KCphrrxJcTIBFWlcXvXVm96lV8nBfYDeTIHAzyrRhlbVcTfrgDLf5N+27j,cebMXjnZljpYhuYjRbdDd,9qoek31cXf9LvLkQHKMgwBvE3JT5GtwDjfKJc1oYsCrFMdZg9KCjJNtEyHACPltrIR4SYRva,sgO5xJ+06AaYIlhpXVTZHt0ncqJECK302ALc3VWiamcRVCDj+ycBQpH40jLsHqzvl+bN8co4QrJDWnY8gLH4u6Ub,pUYDSI7XRtFmufTAdABzYcGwWccdWCP6BrvvgktjbuVd8mctC7,yzVh7RQtMMGLPurxp3qFI8ns3eITQ+Hw",
    "OycDQy37KCphrrxJcTIBFWlcXvXVm96lV8nBfYDeTIHAzyrRhlbVcTfrgDLf5N+27j,cebMXjnZljpYhuYjRbdDd,9qoek31cXf9LvLkQHKMgwBvE3JT5GtwDjfKJc1oYsCrFMdZg9KCjJNtEyHACPltrIR4SYRva,sgO5xJ+06AaYIlhpXVTZHt0ncqJECK302ALc3VWiamcRVCDj+ycBQpH40jLsHqzvl+bN8co4QrJDWnY8gLH4u6Ub,pUYDSI7XRtFmufTAdABzYcGwWccdWCP6BrvvgktjbuVd8mctC7,yzVh7RQtMMGLPurxp3qFI8ns3eITQ+H7U",
    "OycDQy37KCphrrxJcTIBFWlcXvXVm96lV8nBfYDeTIHAzyrRhlbVcTfrgDLf5N+27j,cebMXjnZljpYhuYjRbdDd,9qoek31cXf9LvLkQHKMgwBvE3JT5GtwDjfKJc1oYsCrFMdZg9KCjJNtEyHACPltrIR4SYRva,sgO5xJ+06AaYIlhpXVTZHt0ncqJECK302ALc3VWiamcRVCDj+ycBQpH40jLsHqzvl+bN8co4QrJDWnY8gLH4u6Ub,pUYDSI7XRtFmufTAdABzYcGwWccdWCP6BrvvgktjbuVd8mctC7,yzVh7RQtMMGLPurxp3qFI8ns3eITQ+H7VU",
    "OycDQy37KCphrrxJcTIBFWlcXvXVm96lV8nBfYDeTIHAzyrRhlbVcTfrgDLf5N+27j,cebMXjnZljpYhuYjRbdDd,9qoek31cXf9LvLkQHKMgwBvE3JT5GtwDjfKJc1oYsCrFMdZg9KCjJNtEyHACPltrIR4SYRva,sgO5xJ+06AaYIlhpXVTZHt0ncqJECK302ALc3VWiamcRVCDj+ycBQpH40jLsHqzvl+bN8co4QrJDWnY8gLH4u6Ub,pUYDSI7XRtFmufTAdABzYcGwWccdWCP6BrvvgktjbuVd8mctC7,yzVh7RQtMMGLPurxp3qFI8ns3eITQ+H7VU1w",
    "OycDQy37KCphrrxJcTIBFWlcXvXVm96lV8nBfYDeTIHAzyrRhlbVcTfrgDLf5N+27j,cebMXjnZljpYhuYjRbdDd,9qoek31cXf9LvLkQHKMgwBvE3JT5GtwDjfKJc1oYsCrFMdZg9KCjJNtEyHACPltrIR4SYRva,sgO5xJ+06AaYIlhpXVTZHt0ncqJECK302ALc3VWiamcRVCDj+ycBQpH40jLsHqzvl+bN8co4QrJDWnY8gLH4u6Ub,pUYDSI7XRtFmufTAdABzYcGwWccdWCP6BrvvgktjbuVd8mctC7,yzVh7RQtMMGLPurxp3qFI8ns3eITQ+H7VU1,s",
    "OycDQy37KCphrrxJcTIBFWlcXvXVm96lV8nBfYDeTIHAzyrRhlbVcTfrgDLf5N+27j,cebMXjnZljpYhuYjRbdDd,9qoek31cXf9LvLkQHKMgwBvE3JT5GtwDjfKJc1oYsCrFMdZg9KCjJNtEyHACPltrIR4SYRva,sgO5xJ+06AaYIlhpXVTZHt0ncqJECK302ALc3VWiamcRVCDj+ycBQpH40jLsHqzvl+bN8co4QrJDWnY8gLH4u6Ub,pUYDSI7XRtFmufTAdABzYcGwWccdWCP6BrvvgktjbuVd8mctC7,yzVh7RQtMMGLPurxp3qFI8ns3eITQ+H7VU1,u0",
    "OycDQy37KCphrrxJcTIBFWlcXvXVm96lV8nBfYDeTIHAzyrRhlbVcTfrgDLf5N+27j,cebMXjnZljpYhuYjRbdDd,9qoek31cXf9LvLkQHKMgwBvE3JT5GtwDjfKJc1oYsCrFMdZg9KCjJNtEyHACPltrIR4SYRva,sgO5xJ+06AaYIlhpXVTZHt0ncqJECK302ALc3VWiamcRVCDj+ycBQpH40jLsHqzvl+bN8co4QrJDWnY8gLH4u6Ub,pUYDSI7XRtFmufTAdABzYcGwWccdWCP6BrvvgktjbuVd8mctC7,yzVh7RQtMMGLPurxp3qFI8ns3eITQ+H7VU1,u0vQ"
  };

  SUITE(test_base64_rfc3501_decoder_with_no_padding)
  {
    //*************************************************************************
    TEST(test_check_encode_safe_buffer_sizes)
    {
      for (size_t i = 0; i < 256; ++i)
      {
        size_t minimum_size = i;
        size_t safe_size    = codec::safe_output_buffer_size(encoded[i].size());

        CHECK_TRUE(safe_size >= minimum_size);
        CHECK_TRUE((safe_size - minimum_size) <= 2U);
      }
    }

    //*************************************************************************
    TEST(test_decode_pointer_size_single_pass_with_callback)
    {
      codec b64(callback);

      for (size_t i = 0; i < 256; ++i)
      {
        decoded_output.clear();
        received_final_block = false;

        b64.decode_final(encoded[i].data(), encoded[i].size());

        std::vector<unsigned char> expected(input_data.begin(), input_data.begin() + i);
        std::vector<unsigned char> actual(decoded_output);

        CHECK_TRUE(received_final_block);
        CHECK_EQUAL(expected.size(), actual.size());
        CHECK_TRUE(std::equal(expected.begin(), expected.end(), actual.begin()));
        CHECK_TRUE(codec::safe_output_buffer_size(encoded[i].size()) >= decoded_output.size());
      }
    }

    //*************************************************************************
    TEST(test_decode_pointer_size_single_pass_with_callback_and_larger_buffer)
    {
      codec_larger_buffer b64(callback);

      for (size_t i = 0; i < 256; ++i)
      {
        decoded_output.clear();
        received_final_block = false;

        b64.decode_final(encoded[i].data(), encoded[i].size());

        std::vector<unsigned char> expected(input_data.begin(), input_data.begin() + i);
        std::vector<unsigned char> actual(decoded_output);

        CHECK_TRUE(received_final_block);
        CHECK_EQUAL(expected.size(), actual.size());
        CHECK_TRUE(std::equal(expected.begin(), expected.end(), actual.begin()));
        CHECK_TRUE(codec_larger_buffer::safe_output_buffer_size(encoded[i].size()) >= decoded_output.size());
      }
    }

    //*************************************************************************
    TEST(test_decode_pointer_size_multi_pass_with_callback)
    {
      codec b64(callback);

      for (size_t i = 0; i < 256; ++i)
      {
        decoded_output.clear();
        received_final_block = false;

        auto start  = encoded[i].data();
        auto length = encoded[i].size();

        while (length >= 5)
        {
          b64.decode(start, 5);
          length -= 5;
          start += 5;
        }

        if (length > 0)
        {
          b64.decode(start, length);
        }

        CHECK_FALSE(received_final_block);
        b64.flush();
        CHECK_TRUE(received_final_block);

        std::vector<unsigned char> expected(input_data.begin(), input_data.begin() + i);
        std::vector<unsigned char> actual(decoded_output);

        CHECK_TRUE(received_final_block);
        CHECK_EQUAL(expected.size(), actual.size());
        CHECK_TRUE(std::equal(expected.begin(), expected.end(), actual.begin()));
        CHECK_TRUE(codec::safe_output_buffer_size(encoded[i].size()) >= decoded_output.size());
      }
    }

    //*************************************************************************
    TEST(test_decode_pointer_size_multi_pass_with_callback_and_larger_buffer)
    {
      codec_larger_buffer b64(callback);

      for (size_t i = 0; i < 256; ++i)
      {
        decoded_output.clear();
        received_final_block = false;

        auto start = encoded[i].data();
        auto length = encoded[i].size();

        while (length >= 5)
        {
          b64.decode(start, 5);
          length -= 5;
          start += 5;
        }

        if (length > 0)
        {
          b64.decode(start, length);
        }

        CHECK_FALSE(received_final_block);
        b64.flush();
        CHECK_TRUE(received_final_block);

        std::vector<unsigned char> expected(input_data.begin(), input_data.begin() + i);
        std::vector<unsigned char> actual(decoded_output);

        CHECK_TRUE(received_final_block);
        CHECK_EQUAL(expected.size(), actual.size());
        CHECK_TRUE(std::equal(expected.begin(), expected.end(), actual.begin()));
        CHECK_TRUE(codec::safe_output_buffer_size(encoded[i].size()) >= decoded_output.size());
      }
    }

    //*************************************************************************
    TEST(test_decode_pointer_pointer_single_pass_with_callback)
    {
      codec b64(callback);

      for (size_t i = 0; i < 256; ++i)
      {
        decoded_output.clear();
        received_final_block = false;

        b64.decode_final(encoded[i].begin(), encoded[i].end());

        std::vector<unsigned char> expected(input_data.begin(), input_data.begin() + i);
        std::vector<unsigned char> actual(decoded_output);

        CHECK_TRUE(received_final_block);
        CHECK_EQUAL(expected.size(), actual.size());
        CHECK_TRUE(std::equal(expected.begin(), expected.end(), actual.begin()));
        CHECK_TRUE(codec::safe_output_buffer_size(encoded[i].size()) >= decoded_output.size());
      }
    }

    //*************************************************************************
    TEST(test_decode_pointer_pointer_single_pass_with_callback_and_larger_buffer)
    {
      codec_larger_buffer b64(callback);

      for (size_t i = 0; i < 256; ++i)
      {
        decoded_output.clear();
        received_final_block = false;

        b64.decode_final(encoded[i].begin(), encoded[i].end());

        std::vector<unsigned char> expected(input_data.begin(), input_data.begin() + i);
        std::vector<unsigned char> actual(decoded_output);

        CHECK_TRUE(received_final_block);
        CHECK_EQUAL(expected.size(), actual.size());
        CHECK_TRUE(std::equal(expected.begin(), expected.end(), actual.begin()));
        CHECK_TRUE(codec_larger_buffer::safe_output_buffer_size(encoded[i].size()) >= decoded_output.size());
      }
    }

    //*************************************************************************
    TEST(test_decode_pointer_pointer_multi_pass_with_callback)
    {
      codec b64(callback);

      for (size_t i = 0; i < 256; ++i)
      {
        decoded_output.clear();
        received_final_block = false;

        auto start = encoded[i].data();
        auto length = encoded[i].size();

        while (length >= 5)
        {
          b64.decode(start, 5);
          length -= 5;
          start += 5;
        }

        if (length > 0)
        {
          b64.decode(start, start + length);
        }

        CHECK_FALSE(received_final_block);
        b64.flush();
        CHECK_TRUE(received_final_block);

        std::vector<unsigned char> expected(input_data.begin(), input_data.begin() + i);
        std::vector<unsigned char> actual(decoded_output);

        CHECK_TRUE(received_final_block);
        CHECK_EQUAL(expected.size(), actual.size());
        CHECK_TRUE(std::equal(expected.begin(), expected.end(), actual.begin()));
        CHECK_TRUE(codec::safe_output_buffer_size(encoded[i].size()) >= decoded_output.size());
      }
    }

    //*************************************************************************
    TEST(test_decode_pointer_pointer_multi_pass_with_callback_and_larger_buffer)
    {
      codec_larger_buffer b64(callback);

      for (size_t i = 0; i < 256; ++i)
      {
        decoded_output.clear();
        received_final_block = false;

        auto start  = encoded[i].data();
        auto length = encoded[i].size();

        while (length >= 5)
        {
          b64.decode(start, 5);
          length -= 5;
          start += 5;
        }

        if (length > 0)
        {
          b64.decode(start, start + length);
        }

        CHECK_FALSE(received_final_block);
        b64.flush();
        CHECK_TRUE(received_final_block);

        std::vector<unsigned char> expected(input_data.begin(), input_data.begin() + i);
        std::vector<unsigned char> actual(decoded_output);

        CHECK_TRUE(received_final_block);
        CHECK_EQUAL(expected.size(), actual.size());
        CHECK_TRUE(std::equal(expected.begin(), expected.end(), actual.begin()));
        CHECK_TRUE(codec::safe_output_buffer_size(encoded[i].size()) >= decoded_output.size());
      }
    }

    //*************************************************************************
    TEST(test_decode_multi_pass_by_char_with_callback)
    {
      codec b64(callback);

      for (size_t i = 0; i < 256; ++i)
      {
        decoded_output.clear();
        received_final_block = false;

        auto start  = encoded[i].data();
        auto length = encoded[i].size();

        while (length--)
        {
          b64.decode(*start++);
        }

        CHECK_FALSE(received_final_block);
        b64.flush();
        CHECK_TRUE(received_final_block);

        std::vector<unsigned char> expected(input_data.begin(), input_data.begin() + i);
        std::vector<unsigned char> actual(decoded_output);

        CHECK_TRUE(received_final_block);
        CHECK_EQUAL(expected.size(), actual.size());
        CHECK_TRUE(std::equal(expected.begin(), expected.end(), actual.begin()));
        CHECK_TRUE(codec::safe_output_buffer_size(encoded[i].size()) >= decoded_output.size());
      }
    }

    //*************************************************************************
    TEST(test_decode_multi_pass_by_char_with_callback_and_larger_buffer)
    {
      codec_larger_buffer b64(callback);

      for (size_t i = 0; i < 256; ++i)
      {
        decoded_output.clear();
        received_final_block = false;

        auto start = encoded[i].data();
        auto length = encoded[i].size();

        while (length--)
        {
          b64.decode(*start++);
        }

        CHECK_FALSE(received_final_block);
        b64.flush();
        CHECK_TRUE(received_final_block);

        std::vector<unsigned char> expected(input_data.begin(), input_data.begin() + i);
        std::vector<unsigned char> actual(decoded_output);

        CHECK_TRUE(received_final_block);
        CHECK_EQUAL(expected.size(), actual.size());
        CHECK_TRUE(std::equal(expected.begin(), expected.end(), actual.begin()));
        CHECK_TRUE(codec::safe_output_buffer_size(encoded[i].size()) >= decoded_output.size());
      }
    }

    //*************************************************************************
    TEST(test_decode_multi_pass_blocks_with_no_callback_and_full_size_buffer)
    {
      codec_full_buffer b64;

      for (size_t i = 4; i < 256; ++i)
      {
        b64.restart();

        auto start  = encoded[i].data();
        auto length = encoded[i].size();

        while (length >= 5)
        {
          b64.decode(start, 5);
          length -= 5;
          start += 5;
        }

        if (length > 0)
        {
          b64.decode(start, length);
        }

        b64.flush();

        std::vector<unsigned char> expected(input_data.begin(), input_data.begin() + i);
        std::vector<unsigned char> actual(b64.begin(), b64.end());

        CHECK_EQUAL(expected.size(), actual.size());
        CHECK_TRUE(std::equal(expected.begin(), expected.end(), actual.begin()));
        CHECK_TRUE(codec::safe_output_buffer_size(encoded[i].size()) >= b64.size());
      }
    }

    //*************************************************************************
#if ETL_USING_CPP14
    template <size_t Size>
    constexpr auto GetConstexprBase64(const etl::array<char, Size> input) noexcept
    {
      etl::array<char, 10> output{ 0 };

      using codec = etl::base64_rfc3501_decoder<codec::safe_output_buffer_size(Size)>;

      codec b64;
      b64.decode_final(input.begin(), input.end());
      etl::copy(b64.begin(), b64.end(), output.begin());

      return output;
    }

    TEST(test_encode_constexpr)
    {
      constexpr etl::array<char, 14> input = { 'A', 'A', 'E', 'C' ,'A' ,'w', 'Q', 'F', 'B', 'g', 'c', 'I', 'C', 'Q'};

      constexpr auto output{ GetConstexprBase64(input) };

      std::vector<unsigned char> expected = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
      std::vector<unsigned char> actual(output.begin(), output.end());

      CHECK_TRUE(std::equal(expected.begin(), expected.end(), actual.begin()));
      CHECK_TRUE(codec::safe_output_buffer_size(14) >= output.size());
    }
#endif

    //*************************************************************************
    TEST(test_decode_overflow)
    {
      codec b64;

#if ETL_USING_EXCEPTIONS
      CHECK_THROW((b64.decode(encoded[10].data(), encoded[10].size())), etl::base64_overflow);
#else
      CHECK_FALSE(b64.decode(encoded[10].data(), encoded[10].size());
      CHECK_TRUE(b64.error());
#endif
    }

    //*************************************************************************
    TEST(test_decode_invalid_character)
    {
      codec_larger_buffer b64;

      std::string invalid_chararacter("OycDQ#37KA");

#if ETL_USING_EXCEPTIONS
      CHECK_THROW((b64.decode(invalid_chararacter.data(), invalid_chararacter.size())), etl::base64_invalid_data);
#else
      CHECK_FALSE(b64.decode(invalid_chararacter.data(), invalid_chararacter.size()));
      CHECK_TRUE(b64.error());
#endif
    }
  };
}

