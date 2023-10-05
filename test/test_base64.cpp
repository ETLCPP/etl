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

#include "etl/base64.h"

#include "etl/string.h"
#include "etl/vector.h"

#include <string>
#include <array>
#include <vector>
#include <algorithm>

namespace
{
  std::array<unsigned char, 256> input_data_unsigned_char =
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

  std::array<int8_t, 256> input_data_int8_t =
  {
    int8_t(0x3B), int8_t(0x27), int8_t(0x03), int8_t(0x43), int8_t(0x2D), int8_t(0xFB), int8_t(0x28), int8_t(0x2A), int8_t(0x61), int8_t(0xAE), int8_t(0xBC), int8_t(0x49), int8_t(0x71), int8_t(0x32), int8_t(0x01), int8_t(0x15),
    int8_t(0x69), int8_t(0x5C), int8_t(0x5E), int8_t(0xF5), int8_t(0xD5), int8_t(0x9B), int8_t(0xDE), int8_t(0xA5), int8_t(0x57), int8_t(0xC9), int8_t(0xC1), int8_t(0x7D), int8_t(0x80), int8_t(0xDE), int8_t(0x4C), int8_t(0x81),
    int8_t(0xC0), int8_t(0xCF), int8_t(0x2A), int8_t(0xD1), int8_t(0x86), int8_t(0x56), int8_t(0xD5), int8_t(0x71), int8_t(0x37), int8_t(0xEB), int8_t(0x80), int8_t(0x32), int8_t(0xDF), int8_t(0xE4), int8_t(0xDF), int8_t(0xB6),
    int8_t(0xEE), int8_t(0x3F), int8_t(0xDC), int8_t(0x79), int8_t(0xB3), int8_t(0x17), int8_t(0x8E), int8_t(0x76), int8_t(0x65), int8_t(0x8E), int8_t(0x96), int8_t(0x21), int8_t(0xB9), int8_t(0x88), int8_t(0xD1), int8_t(0x6D),
    int8_t(0xD0), int8_t(0xDD), int8_t(0xFF), int8_t(0xDA), int8_t(0xA8), int8_t(0x7A), int8_t(0x4D), int8_t(0xF5), int8_t(0x71), int8_t(0x77), int8_t(0xFD), int8_t(0x2E), int8_t(0xF2), int8_t(0xE4), int8_t(0x40), int8_t(0x72),
    int8_t(0x8C), int8_t(0x83), int8_t(0x00), int8_t(0x6F), int8_t(0x13), int8_t(0x72), int8_t(0x53), int8_t(0xE4), int8_t(0x6B), int8_t(0x70), int8_t(0x0E), int8_t(0x37), int8_t(0xCA), int8_t(0x25), int8_t(0xCD), int8_t(0x68),
    int8_t(0x62), int8_t(0xC0), int8_t(0xAB), int8_t(0x14), int8_t(0xC7), int8_t(0x59), int8_t(0x83), int8_t(0xD2), int8_t(0x82), int8_t(0x8C), int8_t(0x93), int8_t(0x6D), int8_t(0x13), int8_t(0x21), int8_t(0xC0), int8_t(0x08),
    int8_t(0xF9), int8_t(0x6D), int8_t(0xAC), int8_t(0x84), int8_t(0x78), int8_t(0x49), int8_t(0x84), int8_t(0x6F), int8_t(0x6B), int8_t(0xFB), int8_t(0x20), int8_t(0x3B), int8_t(0x9C), int8_t(0x49), int8_t(0xFB), int8_t(0x4E),
    int8_t(0x80), int8_t(0x69), int8_t(0x82), int8_t(0x25), int8_t(0x86), int8_t(0x95), int8_t(0xD5), int8_t(0x4D), int8_t(0x91), int8_t(0xED), int8_t(0xD2), int8_t(0x77), int8_t(0x2A), int8_t(0x24), int8_t(0x40), int8_t(0x8A),
    int8_t(0xDF), int8_t(0x4D), int8_t(0x80), int8_t(0x2D), int8_t(0xCD), int8_t(0xD5), int8_t(0x5A), int8_t(0x26), int8_t(0xA6), int8_t(0x71), int8_t(0x15), int8_t(0x42), int8_t(0x0E), int8_t(0x3F), int8_t(0xB2), int8_t(0x70),
    int8_t(0x14), int8_t(0x29), int8_t(0x1F), int8_t(0x8D), int8_t(0x23), int8_t(0x2E), int8_t(0xC1), int8_t(0xEA), int8_t(0xCE), int8_t(0xF9), int8_t(0x7E), int8_t(0x6C), int8_t(0xDF), int8_t(0x1C), int8_t(0xA3), int8_t(0x84),
    int8_t(0x2B), int8_t(0x24), int8_t(0x35), int8_t(0xA7), int8_t(0x63), int8_t(0xC8), int8_t(0x0B), int8_t(0x1F), int8_t(0x8B), int8_t(0xBA), int8_t(0x51), int8_t(0xBF), int8_t(0xE9), int8_t(0x51), int8_t(0x80), int8_t(0xD2),
    int8_t(0x23), int8_t(0xB5), int8_t(0xD1), int8_t(0xB4), int8_t(0x59), int8_t(0xAE), int8_t(0x7D), int8_t(0x30), int8_t(0x1D), int8_t(0x00), int8_t(0x1C), int8_t(0xD8), int8_t(0x70), int8_t(0x6C), int8_t(0x16), int8_t(0x71),
    int8_t(0xC7), int8_t(0x56), int8_t(0x08), int8_t(0xFE), int8_t(0x81), int8_t(0xAE), int8_t(0xFB), int8_t(0xE0), int8_t(0x92), int8_t(0xD8), int8_t(0xDB), int8_t(0xB9), int8_t(0x57), int8_t(0x7C), int8_t(0x99), int8_t(0xCB),
    int8_t(0x42), int8_t(0xEF), int8_t(0xFC), int8_t(0xB3), int8_t(0x56), int8_t(0x1E), int8_t(0xD1), int8_t(0x42), int8_t(0xD3), int8_t(0x0C), int8_t(0x18), int8_t(0xB3), int8_t(0xEE), int8_t(0xAF), int8_t(0x1A), int8_t(0x77),
    int8_t(0xA8), int8_t(0x52), int8_t(0x3C), int8_t(0x9E), int8_t(0xCD), int8_t(0xDE), int8_t(0x21), int8_t(0x34), int8_t(0x3E), int8_t(0x1F), int8_t(0xB5), int8_t(0x54), int8_t(0xD7), int8_t(0xFB), int8_t(0xB4), int8_t(0xBD)
  };

  std::array<std::string, 257> encoded =
  {
    "",
    "Ow==",
    "Oyc=",
    "OycD",
    "OycDQw==",
    "OycDQy0=",
    "OycDQy37",
    "OycDQy37KA==",
    "OycDQy37KCo=",
    "OycDQy37KCph",
    "OycDQy37KCphrg==",
    "OycDQy37KCphrrw=",
    "OycDQy37KCphrrxJ",
    "OycDQy37KCphrrxJcQ==",
    "OycDQy37KCphrrxJcTI=",
    "OycDQy37KCphrrxJcTIB",
    "OycDQy37KCphrrxJcTIBFQ==",
    "OycDQy37KCphrrxJcTIBFWk=",
    "OycDQy37KCphrrxJcTIBFWlc",
    "OycDQy37KCphrrxJcTIBFWlcXg==",
    "OycDQy37KCphrrxJcTIBFWlcXvU=",
    "OycDQy37KCphrrxJcTIBFWlcXvXV",
    "OycDQy37KCphrrxJcTIBFWlcXvXVmw==",
    "OycDQy37KCphrrxJcTIBFWlcXvXVm94=",
    "OycDQy37KCphrrxJcTIBFWlcXvXVm96l",
    "OycDQy37KCphrrxJcTIBFWlcXvXVm96lVw==",
    "OycDQy37KCphrrxJcTIBFWlcXvXVm96lV8k=",
    "OycDQy37KCphrrxJcTIBFWlcXvXVm96lV8nB",
    "OycDQy37KCphrrxJcTIBFWlcXvXVm96lV8nBfQ==",
    "OycDQy37KCphrrxJcTIBFWlcXvXVm96lV8nBfYA=",
    "OycDQy37KCphrrxJcTIBFWlcXvXVm96lV8nBfYDe",
    "OycDQy37KCphrrxJcTIBFWlcXvXVm96lV8nBfYDeTA==",
    "OycDQy37KCphrrxJcTIBFWlcXvXVm96lV8nBfYDeTIE=",
    "OycDQy37KCphrrxJcTIBFWlcXvXVm96lV8nBfYDeTIHA",
    "OycDQy37KCphrrxJcTIBFWlcXvXVm96lV8nBfYDeTIHAzw==",
    "OycDQy37KCphrrxJcTIBFWlcXvXVm96lV8nBfYDeTIHAzyo=",
    "OycDQy37KCphrrxJcTIBFWlcXvXVm96lV8nBfYDeTIHAzyrR",
    "OycDQy37KCphrrxJcTIBFWlcXvXVm96lV8nBfYDeTIHAzyrRhg==",
    "OycDQy37KCphrrxJcTIBFWlcXvXVm96lV8nBfYDeTIHAzyrRhlY=",
    "OycDQy37KCphrrxJcTIBFWlcXvXVm96lV8nBfYDeTIHAzyrRhlbV",
    "OycDQy37KCphrrxJcTIBFWlcXvXVm96lV8nBfYDeTIHAzyrRhlbVcQ==",
    "OycDQy37KCphrrxJcTIBFWlcXvXVm96lV8nBfYDeTIHAzyrRhlbVcTc=",
    "OycDQy37KCphrrxJcTIBFWlcXvXVm96lV8nBfYDeTIHAzyrRhlbVcTfr",
    "OycDQy37KCphrrxJcTIBFWlcXvXVm96lV8nBfYDeTIHAzyrRhlbVcTfrgA==",
    "OycDQy37KCphrrxJcTIBFWlcXvXVm96lV8nBfYDeTIHAzyrRhlbVcTfrgDI=",
    "OycDQy37KCphrrxJcTIBFWlcXvXVm96lV8nBfYDeTIHAzyrRhlbVcTfrgDLf",
    "OycDQy37KCphrrxJcTIBFWlcXvXVm96lV8nBfYDeTIHAzyrRhlbVcTfrgDLf5A==",
    "OycDQy37KCphrrxJcTIBFWlcXvXVm96lV8nBfYDeTIHAzyrRhlbVcTfrgDLf5N8=",
    "OycDQy37KCphrrxJcTIBFWlcXvXVm96lV8nBfYDeTIHAzyrRhlbVcTfrgDLf5N+2",
    "OycDQy37KCphrrxJcTIBFWlcXvXVm96lV8nBfYDeTIHAzyrRhlbVcTfrgDLf5N+27g==",
    "OycDQy37KCphrrxJcTIBFWlcXvXVm96lV8nBfYDeTIHAzyrRhlbVcTfrgDLf5N+27j8=",
    "OycDQy37KCphrrxJcTIBFWlcXvXVm96lV8nBfYDeTIHAzyrRhlbVcTfrgDLf5N+27j/c",
    "OycDQy37KCphrrxJcTIBFWlcXvXVm96lV8nBfYDeTIHAzyrRhlbVcTfrgDLf5N+27j/ceQ==",
    "OycDQy37KCphrrxJcTIBFWlcXvXVm96lV8nBfYDeTIHAzyrRhlbVcTfrgDLf5N+27j/cebM=",
    "OycDQy37KCphrrxJcTIBFWlcXvXVm96lV8nBfYDeTIHAzyrRhlbVcTfrgDLf5N+27j/cebMX",
    "OycDQy37KCphrrxJcTIBFWlcXvXVm96lV8nBfYDeTIHAzyrRhlbVcTfrgDLf5N+27j/cebMXjg==",
    "OycDQy37KCphrrxJcTIBFWlcXvXVm96lV8nBfYDeTIHAzyrRhlbVcTfrgDLf5N+27j/cebMXjnY=",
    "OycDQy37KCphrrxJcTIBFWlcXvXVm96lV8nBfYDeTIHAzyrRhlbVcTfrgDLf5N+27j/cebMXjnZl",
    "OycDQy37KCphrrxJcTIBFWlcXvXVm96lV8nBfYDeTIHAzyrRhlbVcTfrgDLf5N+27j/cebMXjnZljg==",
    "OycDQy37KCphrrxJcTIBFWlcXvXVm96lV8nBfYDeTIHAzyrRhlbVcTfrgDLf5N+27j/cebMXjnZljpY=",
    "OycDQy37KCphrrxJcTIBFWlcXvXVm96lV8nBfYDeTIHAzyrRhlbVcTfrgDLf5N+27j/cebMXjnZljpYh",
    "OycDQy37KCphrrxJcTIBFWlcXvXVm96lV8nBfYDeTIHAzyrRhlbVcTfrgDLf5N+27j/cebMXjnZljpYhuQ==",
    "OycDQy37KCphrrxJcTIBFWlcXvXVm96lV8nBfYDeTIHAzyrRhlbVcTfrgDLf5N+27j/cebMXjnZljpYhuYg=",
    "OycDQy37KCphrrxJcTIBFWlcXvXVm96lV8nBfYDeTIHAzyrRhlbVcTfrgDLf5N+27j/cebMXjnZljpYhuYjR",
    "OycDQy37KCphrrxJcTIBFWlcXvXVm96lV8nBfYDeTIHAzyrRhlbVcTfrgDLf5N+27j/cebMXjnZljpYhuYjRbQ==",
    "OycDQy37KCphrrxJcTIBFWlcXvXVm96lV8nBfYDeTIHAzyrRhlbVcTfrgDLf5N+27j/cebMXjnZljpYhuYjRbdA=",
    "OycDQy37KCphrrxJcTIBFWlcXvXVm96lV8nBfYDeTIHAzyrRhlbVcTfrgDLf5N+27j/cebMXjnZljpYhuYjRbdDd",
    "OycDQy37KCphrrxJcTIBFWlcXvXVm96lV8nBfYDeTIHAzyrRhlbVcTfrgDLf5N+27j/cebMXjnZljpYhuYjRbdDd/w==",
    "OycDQy37KCphrrxJcTIBFWlcXvXVm96lV8nBfYDeTIHAzyrRhlbVcTfrgDLf5N+27j/cebMXjnZljpYhuYjRbdDd/9o=",
    "OycDQy37KCphrrxJcTIBFWlcXvXVm96lV8nBfYDeTIHAzyrRhlbVcTfrgDLf5N+27j/cebMXjnZljpYhuYjRbdDd/9qo",
    "OycDQy37KCphrrxJcTIBFWlcXvXVm96lV8nBfYDeTIHAzyrRhlbVcTfrgDLf5N+27j/cebMXjnZljpYhuYjRbdDd/9qoeg==",
    "OycDQy37KCphrrxJcTIBFWlcXvXVm96lV8nBfYDeTIHAzyrRhlbVcTfrgDLf5N+27j/cebMXjnZljpYhuYjRbdDd/9qoek0=",
    "OycDQy37KCphrrxJcTIBFWlcXvXVm96lV8nBfYDeTIHAzyrRhlbVcTfrgDLf5N+27j/cebMXjnZljpYhuYjRbdDd/9qoek31",
    "OycDQy37KCphrrxJcTIBFWlcXvXVm96lV8nBfYDeTIHAzyrRhlbVcTfrgDLf5N+27j/cebMXjnZljpYhuYjRbdDd/9qoek31cQ==",
    "OycDQy37KCphrrxJcTIBFWlcXvXVm96lV8nBfYDeTIHAzyrRhlbVcTfrgDLf5N+27j/cebMXjnZljpYhuYjRbdDd/9qoek31cXc=",
    "OycDQy37KCphrrxJcTIBFWlcXvXVm96lV8nBfYDeTIHAzyrRhlbVcTfrgDLf5N+27j/cebMXjnZljpYhuYjRbdDd/9qoek31cXf9",
    "OycDQy37KCphrrxJcTIBFWlcXvXVm96lV8nBfYDeTIHAzyrRhlbVcTfrgDLf5N+27j/cebMXjnZljpYhuYjRbdDd/9qoek31cXf9Lg==",
    "OycDQy37KCphrrxJcTIBFWlcXvXVm96lV8nBfYDeTIHAzyrRhlbVcTfrgDLf5N+27j/cebMXjnZljpYhuYjRbdDd/9qoek31cXf9LvI=",
    "OycDQy37KCphrrxJcTIBFWlcXvXVm96lV8nBfYDeTIHAzyrRhlbVcTfrgDLf5N+27j/cebMXjnZljpYhuYjRbdDd/9qoek31cXf9LvLk",
    "OycDQy37KCphrrxJcTIBFWlcXvXVm96lV8nBfYDeTIHAzyrRhlbVcTfrgDLf5N+27j/cebMXjnZljpYhuYjRbdDd/9qoek31cXf9LvLkQA==",
    "OycDQy37KCphrrxJcTIBFWlcXvXVm96lV8nBfYDeTIHAzyrRhlbVcTfrgDLf5N+27j/cebMXjnZljpYhuYjRbdDd/9qoek31cXf9LvLkQHI=",
    "OycDQy37KCphrrxJcTIBFWlcXvXVm96lV8nBfYDeTIHAzyrRhlbVcTfrgDLf5N+27j/cebMXjnZljpYhuYjRbdDd/9qoek31cXf9LvLkQHKM",
    "OycDQy37KCphrrxJcTIBFWlcXvXVm96lV8nBfYDeTIHAzyrRhlbVcTfrgDLf5N+27j/cebMXjnZljpYhuYjRbdDd/9qoek31cXf9LvLkQHKMgw==",
    "OycDQy37KCphrrxJcTIBFWlcXvXVm96lV8nBfYDeTIHAzyrRhlbVcTfrgDLf5N+27j/cebMXjnZljpYhuYjRbdDd/9qoek31cXf9LvLkQHKMgwA=",
    "OycDQy37KCphrrxJcTIBFWlcXvXVm96lV8nBfYDeTIHAzyrRhlbVcTfrgDLf5N+27j/cebMXjnZljpYhuYjRbdDd/9qoek31cXf9LvLkQHKMgwBv",
    "OycDQy37KCphrrxJcTIBFWlcXvXVm96lV8nBfYDeTIHAzyrRhlbVcTfrgDLf5N+27j/cebMXjnZljpYhuYjRbdDd/9qoek31cXf9LvLkQHKMgwBvEw==",
    "OycDQy37KCphrrxJcTIBFWlcXvXVm96lV8nBfYDeTIHAzyrRhlbVcTfrgDLf5N+27j/cebMXjnZljpYhuYjRbdDd/9qoek31cXf9LvLkQHKMgwBvE3I=",
    "OycDQy37KCphrrxJcTIBFWlcXvXVm96lV8nBfYDeTIHAzyrRhlbVcTfrgDLf5N+27j/cebMXjnZljpYhuYjRbdDd/9qoek31cXf9LvLkQHKMgwBvE3JT",
    "OycDQy37KCphrrxJcTIBFWlcXvXVm96lV8nBfYDeTIHAzyrRhlbVcTfrgDLf5N+27j/cebMXjnZljpYhuYjRbdDd/9qoek31cXf9LvLkQHKMgwBvE3JT5A==",
    "OycDQy37KCphrrxJcTIBFWlcXvXVm96lV8nBfYDeTIHAzyrRhlbVcTfrgDLf5N+27j/cebMXjnZljpYhuYjRbdDd/9qoek31cXf9LvLkQHKMgwBvE3JT5Gs=",
    "OycDQy37KCphrrxJcTIBFWlcXvXVm96lV8nBfYDeTIHAzyrRhlbVcTfrgDLf5N+27j/cebMXjnZljpYhuYjRbdDd/9qoek31cXf9LvLkQHKMgwBvE3JT5Gtw",
    "OycDQy37KCphrrxJcTIBFWlcXvXVm96lV8nBfYDeTIHAzyrRhlbVcTfrgDLf5N+27j/cebMXjnZljpYhuYjRbdDd/9qoek31cXf9LvLkQHKMgwBvE3JT5GtwDg==",
    "OycDQy37KCphrrxJcTIBFWlcXvXVm96lV8nBfYDeTIHAzyrRhlbVcTfrgDLf5N+27j/cebMXjnZljpYhuYjRbdDd/9qoek31cXf9LvLkQHKMgwBvE3JT5GtwDjc=",
    "OycDQy37KCphrrxJcTIBFWlcXvXVm96lV8nBfYDeTIHAzyrRhlbVcTfrgDLf5N+27j/cebMXjnZljpYhuYjRbdDd/9qoek31cXf9LvLkQHKMgwBvE3JT5GtwDjfK",
    "OycDQy37KCphrrxJcTIBFWlcXvXVm96lV8nBfYDeTIHAzyrRhlbVcTfrgDLf5N+27j/cebMXjnZljpYhuYjRbdDd/9qoek31cXf9LvLkQHKMgwBvE3JT5GtwDjfKJQ==",
    "OycDQy37KCphrrxJcTIBFWlcXvXVm96lV8nBfYDeTIHAzyrRhlbVcTfrgDLf5N+27j/cebMXjnZljpYhuYjRbdDd/9qoek31cXf9LvLkQHKMgwBvE3JT5GtwDjfKJc0=",
    "OycDQy37KCphrrxJcTIBFWlcXvXVm96lV8nBfYDeTIHAzyrRhlbVcTfrgDLf5N+27j/cebMXjnZljpYhuYjRbdDd/9qoek31cXf9LvLkQHKMgwBvE3JT5GtwDjfKJc1o",
    "OycDQy37KCphrrxJcTIBFWlcXvXVm96lV8nBfYDeTIHAzyrRhlbVcTfrgDLf5N+27j/cebMXjnZljpYhuYjRbdDd/9qoek31cXf9LvLkQHKMgwBvE3JT5GtwDjfKJc1oYg==",
    "OycDQy37KCphrrxJcTIBFWlcXvXVm96lV8nBfYDeTIHAzyrRhlbVcTfrgDLf5N+27j/cebMXjnZljpYhuYjRbdDd/9qoek31cXf9LvLkQHKMgwBvE3JT5GtwDjfKJc1oYsA=",
    "OycDQy37KCphrrxJcTIBFWlcXvXVm96lV8nBfYDeTIHAzyrRhlbVcTfrgDLf5N+27j/cebMXjnZljpYhuYjRbdDd/9qoek31cXf9LvLkQHKMgwBvE3JT5GtwDjfKJc1oYsCr",
    "OycDQy37KCphrrxJcTIBFWlcXvXVm96lV8nBfYDeTIHAzyrRhlbVcTfrgDLf5N+27j/cebMXjnZljpYhuYjRbdDd/9qoek31cXf9LvLkQHKMgwBvE3JT5GtwDjfKJc1oYsCrFA==",
    "OycDQy37KCphrrxJcTIBFWlcXvXVm96lV8nBfYDeTIHAzyrRhlbVcTfrgDLf5N+27j/cebMXjnZljpYhuYjRbdDd/9qoek31cXf9LvLkQHKMgwBvE3JT5GtwDjfKJc1oYsCrFMc=",
    "OycDQy37KCphrrxJcTIBFWlcXvXVm96lV8nBfYDeTIHAzyrRhlbVcTfrgDLf5N+27j/cebMXjnZljpYhuYjRbdDd/9qoek31cXf9LvLkQHKMgwBvE3JT5GtwDjfKJc1oYsCrFMdZ",
    "OycDQy37KCphrrxJcTIBFWlcXvXVm96lV8nBfYDeTIHAzyrRhlbVcTfrgDLf5N+27j/cebMXjnZljpYhuYjRbdDd/9qoek31cXf9LvLkQHKMgwBvE3JT5GtwDjfKJc1oYsCrFMdZgw==",
    "OycDQy37KCphrrxJcTIBFWlcXvXVm96lV8nBfYDeTIHAzyrRhlbVcTfrgDLf5N+27j/cebMXjnZljpYhuYjRbdDd/9qoek31cXf9LvLkQHKMgwBvE3JT5GtwDjfKJc1oYsCrFMdZg9I=",
    "OycDQy37KCphrrxJcTIBFWlcXvXVm96lV8nBfYDeTIHAzyrRhlbVcTfrgDLf5N+27j/cebMXjnZljpYhuYjRbdDd/9qoek31cXf9LvLkQHKMgwBvE3JT5GtwDjfKJc1oYsCrFMdZg9KC",
    "OycDQy37KCphrrxJcTIBFWlcXvXVm96lV8nBfYDeTIHAzyrRhlbVcTfrgDLf5N+27j/cebMXjnZljpYhuYjRbdDd/9qoek31cXf9LvLkQHKMgwBvE3JT5GtwDjfKJc1oYsCrFMdZg9KCjA==",
    "OycDQy37KCphrrxJcTIBFWlcXvXVm96lV8nBfYDeTIHAzyrRhlbVcTfrgDLf5N+27j/cebMXjnZljpYhuYjRbdDd/9qoek31cXf9LvLkQHKMgwBvE3JT5GtwDjfKJc1oYsCrFMdZg9KCjJM=",
    "OycDQy37KCphrrxJcTIBFWlcXvXVm96lV8nBfYDeTIHAzyrRhlbVcTfrgDLf5N+27j/cebMXjnZljpYhuYjRbdDd/9qoek31cXf9LvLkQHKMgwBvE3JT5GtwDjfKJc1oYsCrFMdZg9KCjJNt",
    "OycDQy37KCphrrxJcTIBFWlcXvXVm96lV8nBfYDeTIHAzyrRhlbVcTfrgDLf5N+27j/cebMXjnZljpYhuYjRbdDd/9qoek31cXf9LvLkQHKMgwBvE3JT5GtwDjfKJc1oYsCrFMdZg9KCjJNtEw==",
    "OycDQy37KCphrrxJcTIBFWlcXvXVm96lV8nBfYDeTIHAzyrRhlbVcTfrgDLf5N+27j/cebMXjnZljpYhuYjRbdDd/9qoek31cXf9LvLkQHKMgwBvE3JT5GtwDjfKJc1oYsCrFMdZg9KCjJNtEyE=",
    "OycDQy37KCphrrxJcTIBFWlcXvXVm96lV8nBfYDeTIHAzyrRhlbVcTfrgDLf5N+27j/cebMXjnZljpYhuYjRbdDd/9qoek31cXf9LvLkQHKMgwBvE3JT5GtwDjfKJc1oYsCrFMdZg9KCjJNtEyHA",
    "OycDQy37KCphrrxJcTIBFWlcXvXVm96lV8nBfYDeTIHAzyrRhlbVcTfrgDLf5N+27j/cebMXjnZljpYhuYjRbdDd/9qoek31cXf9LvLkQHKMgwBvE3JT5GtwDjfKJc1oYsCrFMdZg9KCjJNtEyHACA==",
    "OycDQy37KCphrrxJcTIBFWlcXvXVm96lV8nBfYDeTIHAzyrRhlbVcTfrgDLf5N+27j/cebMXjnZljpYhuYjRbdDd/9qoek31cXf9LvLkQHKMgwBvE3JT5GtwDjfKJc1oYsCrFMdZg9KCjJNtEyHACPk=",
    "OycDQy37KCphrrxJcTIBFWlcXvXVm96lV8nBfYDeTIHAzyrRhlbVcTfrgDLf5N+27j/cebMXjnZljpYhuYjRbdDd/9qoek31cXf9LvLkQHKMgwBvE3JT5GtwDjfKJc1oYsCrFMdZg9KCjJNtEyHACPlt",
    "OycDQy37KCphrrxJcTIBFWlcXvXVm96lV8nBfYDeTIHAzyrRhlbVcTfrgDLf5N+27j/cebMXjnZljpYhuYjRbdDd/9qoek31cXf9LvLkQHKMgwBvE3JT5GtwDjfKJc1oYsCrFMdZg9KCjJNtEyHACPltrA==",
    "OycDQy37KCphrrxJcTIBFWlcXvXVm96lV8nBfYDeTIHAzyrRhlbVcTfrgDLf5N+27j/cebMXjnZljpYhuYjRbdDd/9qoek31cXf9LvLkQHKMgwBvE3JT5GtwDjfKJc1oYsCrFMdZg9KCjJNtEyHACPltrIQ=",
    "OycDQy37KCphrrxJcTIBFWlcXvXVm96lV8nBfYDeTIHAzyrRhlbVcTfrgDLf5N+27j/cebMXjnZljpYhuYjRbdDd/9qoek31cXf9LvLkQHKMgwBvE3JT5GtwDjfKJc1oYsCrFMdZg9KCjJNtEyHACPltrIR4",
    "OycDQy37KCphrrxJcTIBFWlcXvXVm96lV8nBfYDeTIHAzyrRhlbVcTfrgDLf5N+27j/cebMXjnZljpYhuYjRbdDd/9qoek31cXf9LvLkQHKMgwBvE3JT5GtwDjfKJc1oYsCrFMdZg9KCjJNtEyHACPltrIR4SQ==",
    "OycDQy37KCphrrxJcTIBFWlcXvXVm96lV8nBfYDeTIHAzyrRhlbVcTfrgDLf5N+27j/cebMXjnZljpYhuYjRbdDd/9qoek31cXf9LvLkQHKMgwBvE3JT5GtwDjfKJc1oYsCrFMdZg9KCjJNtEyHACPltrIR4SYQ=",
    "OycDQy37KCphrrxJcTIBFWlcXvXVm96lV8nBfYDeTIHAzyrRhlbVcTfrgDLf5N+27j/cebMXjnZljpYhuYjRbdDd/9qoek31cXf9LvLkQHKMgwBvE3JT5GtwDjfKJc1oYsCrFMdZg9KCjJNtEyHACPltrIR4SYRv",
    "OycDQy37KCphrrxJcTIBFWlcXvXVm96lV8nBfYDeTIHAzyrRhlbVcTfrgDLf5N+27j/cebMXjnZljpYhuYjRbdDd/9qoek31cXf9LvLkQHKMgwBvE3JT5GtwDjfKJc1oYsCrFMdZg9KCjJNtEyHACPltrIR4SYRvaw==",
    "OycDQy37KCphrrxJcTIBFWlcXvXVm96lV8nBfYDeTIHAzyrRhlbVcTfrgDLf5N+27j/cebMXjnZljpYhuYjRbdDd/9qoek31cXf9LvLkQHKMgwBvE3JT5GtwDjfKJc1oYsCrFMdZg9KCjJNtEyHACPltrIR4SYRva/s=",
    "OycDQy37KCphrrxJcTIBFWlcXvXVm96lV8nBfYDeTIHAzyrRhlbVcTfrgDLf5N+27j/cebMXjnZljpYhuYjRbdDd/9qoek31cXf9LvLkQHKMgwBvE3JT5GtwDjfKJc1oYsCrFMdZg9KCjJNtEyHACPltrIR4SYRva/sg",
    "OycDQy37KCphrrxJcTIBFWlcXvXVm96lV8nBfYDeTIHAzyrRhlbVcTfrgDLf5N+27j/cebMXjnZljpYhuYjRbdDd/9qoek31cXf9LvLkQHKMgwBvE3JT5GtwDjfKJc1oYsCrFMdZg9KCjJNtEyHACPltrIR4SYRva/sgOw==",
    "OycDQy37KCphrrxJcTIBFWlcXvXVm96lV8nBfYDeTIHAzyrRhlbVcTfrgDLf5N+27j/cebMXjnZljpYhuYjRbdDd/9qoek31cXf9LvLkQHKMgwBvE3JT5GtwDjfKJc1oYsCrFMdZg9KCjJNtEyHACPltrIR4SYRva/sgO5w=",
    "OycDQy37KCphrrxJcTIBFWlcXvXVm96lV8nBfYDeTIHAzyrRhlbVcTfrgDLf5N+27j/cebMXjnZljpYhuYjRbdDd/9qoek31cXf9LvLkQHKMgwBvE3JT5GtwDjfKJc1oYsCrFMdZg9KCjJNtEyHACPltrIR4SYRva/sgO5xJ",
    "OycDQy37KCphrrxJcTIBFWlcXvXVm96lV8nBfYDeTIHAzyrRhlbVcTfrgDLf5N+27j/cebMXjnZljpYhuYjRbdDd/9qoek31cXf9LvLkQHKMgwBvE3JT5GtwDjfKJc1oYsCrFMdZg9KCjJNtEyHACPltrIR4SYRva/sgO5xJ+w==",
    "OycDQy37KCphrrxJcTIBFWlcXvXVm96lV8nBfYDeTIHAzyrRhlbVcTfrgDLf5N+27j/cebMXjnZljpYhuYjRbdDd/9qoek31cXf9LvLkQHKMgwBvE3JT5GtwDjfKJc1oYsCrFMdZg9KCjJNtEyHACPltrIR4SYRva/sgO5xJ+04=",
    "OycDQy37KCphrrxJcTIBFWlcXvXVm96lV8nBfYDeTIHAzyrRhlbVcTfrgDLf5N+27j/cebMXjnZljpYhuYjRbdDd/9qoek31cXf9LvLkQHKMgwBvE3JT5GtwDjfKJc1oYsCrFMdZg9KCjJNtEyHACPltrIR4SYRva/sgO5xJ+06A",
    "OycDQy37KCphrrxJcTIBFWlcXvXVm96lV8nBfYDeTIHAzyrRhlbVcTfrgDLf5N+27j/cebMXjnZljpYhuYjRbdDd/9qoek31cXf9LvLkQHKMgwBvE3JT5GtwDjfKJc1oYsCrFMdZg9KCjJNtEyHACPltrIR4SYRva/sgO5xJ+06AaQ==",
    "OycDQy37KCphrrxJcTIBFWlcXvXVm96lV8nBfYDeTIHAzyrRhlbVcTfrgDLf5N+27j/cebMXjnZljpYhuYjRbdDd/9qoek31cXf9LvLkQHKMgwBvE3JT5GtwDjfKJc1oYsCrFMdZg9KCjJNtEyHACPltrIR4SYRva/sgO5xJ+06AaYI=",
    "OycDQy37KCphrrxJcTIBFWlcXvXVm96lV8nBfYDeTIHAzyrRhlbVcTfrgDLf5N+27j/cebMXjnZljpYhuYjRbdDd/9qoek31cXf9LvLkQHKMgwBvE3JT5GtwDjfKJc1oYsCrFMdZg9KCjJNtEyHACPltrIR4SYRva/sgO5xJ+06AaYIl",
    "OycDQy37KCphrrxJcTIBFWlcXvXVm96lV8nBfYDeTIHAzyrRhlbVcTfrgDLf5N+27j/cebMXjnZljpYhuYjRbdDd/9qoek31cXf9LvLkQHKMgwBvE3JT5GtwDjfKJc1oYsCrFMdZg9KCjJNtEyHACPltrIR4SYRva/sgO5xJ+06AaYIlhg==",
    "OycDQy37KCphrrxJcTIBFWlcXvXVm96lV8nBfYDeTIHAzyrRhlbVcTfrgDLf5N+27j/cebMXjnZljpYhuYjRbdDd/9qoek31cXf9LvLkQHKMgwBvE3JT5GtwDjfKJc1oYsCrFMdZg9KCjJNtEyHACPltrIR4SYRva/sgO5xJ+06AaYIlhpU=",
    "OycDQy37KCphrrxJcTIBFWlcXvXVm96lV8nBfYDeTIHAzyrRhlbVcTfrgDLf5N+27j/cebMXjnZljpYhuYjRbdDd/9qoek31cXf9LvLkQHKMgwBvE3JT5GtwDjfKJc1oYsCrFMdZg9KCjJNtEyHACPltrIR4SYRva/sgO5xJ+06AaYIlhpXV",
    "OycDQy37KCphrrxJcTIBFWlcXvXVm96lV8nBfYDeTIHAzyrRhlbVcTfrgDLf5N+27j/cebMXjnZljpYhuYjRbdDd/9qoek31cXf9LvLkQHKMgwBvE3JT5GtwDjfKJc1oYsCrFMdZg9KCjJNtEyHACPltrIR4SYRva/sgO5xJ+06AaYIlhpXVTQ==",
    "OycDQy37KCphrrxJcTIBFWlcXvXVm96lV8nBfYDeTIHAzyrRhlbVcTfrgDLf5N+27j/cebMXjnZljpYhuYjRbdDd/9qoek31cXf9LvLkQHKMgwBvE3JT5GtwDjfKJc1oYsCrFMdZg9KCjJNtEyHACPltrIR4SYRva/sgO5xJ+06AaYIlhpXVTZE=",
    "OycDQy37KCphrrxJcTIBFWlcXvXVm96lV8nBfYDeTIHAzyrRhlbVcTfrgDLf5N+27j/cebMXjnZljpYhuYjRbdDd/9qoek31cXf9LvLkQHKMgwBvE3JT5GtwDjfKJc1oYsCrFMdZg9KCjJNtEyHACPltrIR4SYRva/sgO5xJ+06AaYIlhpXVTZHt",
    "OycDQy37KCphrrxJcTIBFWlcXvXVm96lV8nBfYDeTIHAzyrRhlbVcTfrgDLf5N+27j/cebMXjnZljpYhuYjRbdDd/9qoek31cXf9LvLkQHKMgwBvE3JT5GtwDjfKJc1oYsCrFMdZg9KCjJNtEyHACPltrIR4SYRva/sgO5xJ+06AaYIlhpXVTZHt0g==",
    "OycDQy37KCphrrxJcTIBFWlcXvXVm96lV8nBfYDeTIHAzyrRhlbVcTfrgDLf5N+27j/cebMXjnZljpYhuYjRbdDd/9qoek31cXf9LvLkQHKMgwBvE3JT5GtwDjfKJc1oYsCrFMdZg9KCjJNtEyHACPltrIR4SYRva/sgO5xJ+06AaYIlhpXVTZHt0nc=",
    "OycDQy37KCphrrxJcTIBFWlcXvXVm96lV8nBfYDeTIHAzyrRhlbVcTfrgDLf5N+27j/cebMXjnZljpYhuYjRbdDd/9qoek31cXf9LvLkQHKMgwBvE3JT5GtwDjfKJc1oYsCrFMdZg9KCjJNtEyHACPltrIR4SYRva/sgO5xJ+06AaYIlhpXVTZHt0ncq",
    "OycDQy37KCphrrxJcTIBFWlcXvXVm96lV8nBfYDeTIHAzyrRhlbVcTfrgDLf5N+27j/cebMXjnZljpYhuYjRbdDd/9qoek31cXf9LvLkQHKMgwBvE3JT5GtwDjfKJc1oYsCrFMdZg9KCjJNtEyHACPltrIR4SYRva/sgO5xJ+06AaYIlhpXVTZHt0ncqJA==",
    "OycDQy37KCphrrxJcTIBFWlcXvXVm96lV8nBfYDeTIHAzyrRhlbVcTfrgDLf5N+27j/cebMXjnZljpYhuYjRbdDd/9qoek31cXf9LvLkQHKMgwBvE3JT5GtwDjfKJc1oYsCrFMdZg9KCjJNtEyHACPltrIR4SYRva/sgO5xJ+06AaYIlhpXVTZHt0ncqJEA=",
    "OycDQy37KCphrrxJcTIBFWlcXvXVm96lV8nBfYDeTIHAzyrRhlbVcTfrgDLf5N+27j/cebMXjnZljpYhuYjRbdDd/9qoek31cXf9LvLkQHKMgwBvE3JT5GtwDjfKJc1oYsCrFMdZg9KCjJNtEyHACPltrIR4SYRva/sgO5xJ+06AaYIlhpXVTZHt0ncqJECK",
    "OycDQy37KCphrrxJcTIBFWlcXvXVm96lV8nBfYDeTIHAzyrRhlbVcTfrgDLf5N+27j/cebMXjnZljpYhuYjRbdDd/9qoek31cXf9LvLkQHKMgwBvE3JT5GtwDjfKJc1oYsCrFMdZg9KCjJNtEyHACPltrIR4SYRva/sgO5xJ+06AaYIlhpXVTZHt0ncqJECK3w==",
    "OycDQy37KCphrrxJcTIBFWlcXvXVm96lV8nBfYDeTIHAzyrRhlbVcTfrgDLf5N+27j/cebMXjnZljpYhuYjRbdDd/9qoek31cXf9LvLkQHKMgwBvE3JT5GtwDjfKJc1oYsCrFMdZg9KCjJNtEyHACPltrIR4SYRva/sgO5xJ+06AaYIlhpXVTZHt0ncqJECK300=",
    "OycDQy37KCphrrxJcTIBFWlcXvXVm96lV8nBfYDeTIHAzyrRhlbVcTfrgDLf5N+27j/cebMXjnZljpYhuYjRbdDd/9qoek31cXf9LvLkQHKMgwBvE3JT5GtwDjfKJc1oYsCrFMdZg9KCjJNtEyHACPltrIR4SYRva/sgO5xJ+06AaYIlhpXVTZHt0ncqJECK302A",
    "OycDQy37KCphrrxJcTIBFWlcXvXVm96lV8nBfYDeTIHAzyrRhlbVcTfrgDLf5N+27j/cebMXjnZljpYhuYjRbdDd/9qoek31cXf9LvLkQHKMgwBvE3JT5GtwDjfKJc1oYsCrFMdZg9KCjJNtEyHACPltrIR4SYRva/sgO5xJ+06AaYIlhpXVTZHt0ncqJECK302ALQ==",
    "OycDQy37KCphrrxJcTIBFWlcXvXVm96lV8nBfYDeTIHAzyrRhlbVcTfrgDLf5N+27j/cebMXjnZljpYhuYjRbdDd/9qoek31cXf9LvLkQHKMgwBvE3JT5GtwDjfKJc1oYsCrFMdZg9KCjJNtEyHACPltrIR4SYRva/sgO5xJ+06AaYIlhpXVTZHt0ncqJECK302ALc0=",
    "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"
  };

  SUITE(test_base64)
  {
    //*************************************************************************
    TEST(test_encode_unsigned_char_pointer_size)
    {
      std::array<char, 344U> encoded_output;

      for (size_t i = 0; i < 256; ++i)
      {
        encoded_output.fill(0);

        auto size = etl::base64::encode(input_data_unsigned_char.data(), i,
          encoded_output.data(), encoded_output.size());

        std::string expected(encoded[i]);
        std::string actual(encoded_output.data(), size);

        CHECK_EQUAL(expected, actual);
        CHECK_EQUAL(etl::base64::encode_size(i), size);
      }
    }

    //*************************************************************************
    TEST(test_encode_unsigned_char_pointer_pointer)
    {
      std::array<char, 344U> encoded_output;

      for (size_t i = 0; i < 256; ++i)
      {
        encoded_output.fill(0);

        auto size = etl::base64::encode(input_data_unsigned_char.data(), input_data_unsigned_char.data() + i,
          encoded_output.data(), encoded_output.data() + encoded_output.size());

        std::string expected(encoded[i]);
        std::string actual(encoded_output.data(), size);

        CHECK_EQUAL(expected, actual);
        CHECK_EQUAL(etl::base64::encode_size(i), size);
      }
    }

    //*************************************************************************
    TEST(test_encode_unsigned_char_span)
    {
      std::array<char, 344U> encoded_output;

      for (size_t i = 0; i < 256; ++i)
      {
        encoded_output.fill(0);

        etl::span<const unsigned char> input_span(input_data_unsigned_char.data(), i);
        etl::span<char> output_span(encoded_output.data(), encoded_output.size());

        auto size = etl::base64::encode(input_span, output_span);

        std::string expected(encoded[i]);
        std::string actual(encoded_output.data(), size);

        CHECK_EQUAL(expected, actual);
        CHECK_EQUAL(etl::base64::encode_size(i), size);
      }
    }

    //*************************************************************************
    TEST(test_encode_unsigned_char_pointer_size_to_etl_string)
    {
      etl::string<344U> encoded_output;

      for (size_t i = 0; i < 256; ++i)
      {
        encoded_output.clear();

        auto size = etl::base64::encode(input_data_unsigned_char.data(), i,
                                        encoded_output);

        std::string expected(encoded[i]);
        std::string actual(encoded_output.data(), size);

        CHECK_EQUAL(expected, actual);
        CHECK_EQUAL(etl::base64::encode_size(i), size);
      }
    }

    //*************************************************************************
    TEST(test_encode_unsigned_char_pointer_pointer_to_etl_string)
    {
      etl::string<344U> encoded_output;

      for (size_t i = 0; i < 256; ++i)
      {
        encoded_output.clear();

        auto size = etl::base64::encode(input_data_unsigned_char.data(), input_data_unsigned_char.data() + i,
                                        encoded_output);

        std::string expected(encoded[i]);
        std::string actual(encoded_output.data(), size);

        CHECK_EQUAL(expected, actual);
        CHECK_EQUAL(etl::base64::encode_size(i), size);
      }
    }

    //*************************************************************************
    TEST(test_encode_unsigned_char_span_to_etl_string)
    {
      etl::string<344U> encoded_output;

      for (size_t i = 0; i < 256; ++i)
      {
        encoded_output.clear();

        etl::span<const unsigned char> input_span(input_data_unsigned_char.data(), i);

        auto size = etl::base64::encode(input_span,
                                        encoded_output);

        std::string expected(encoded[i]);
        std::string actual(encoded_output.data(), size);

        CHECK_EQUAL(expected, actual);
        CHECK_EQUAL(etl::base64::encode_size(i), size);
      }
    }

    //*************************************************************************
    TEST(test_encode_int8_t_pointer_size_to_etl_string)
    {
      etl::string<344U> encoded_output;

      for (size_t i = 0; i < 256; ++i)
      {
        encoded_output.clear();

        auto size = etl::base64::encode(input_data_int8_t.data(), i,
                                        encoded_output);

        std::string expected(encoded[i]);
        std::string actual(encoded_output.data(), size);

        CHECK_EQUAL(expected, actual);
        CHECK_EQUAL(etl::base64::encode_size(i), size);
      }
    }

    //*************************************************************************
    TEST(test_encode_int8_t_pointer_pointer_to_etl_string)
    {
      etl::string<344U> encoded_output;

      for (size_t i = 0; i < 256; ++i)
      {
        encoded_output.clear();

        auto size = etl::base64::encode(input_data_int8_t.data(), input_data_int8_t.data() + i,
                                        encoded_output);

        std::string expected(encoded[i]);
        std::string actual(encoded_output.data(), size);

        CHECK_EQUAL(expected, actual);
        CHECK_EQUAL(etl::base64::encode_size(i), size);
      }
    }

    //*************************************************************************
    TEST(test_encode_int8_t_span_to_etl_string)
    {
      etl::string<344U> encoded_output;

      for (size_t i = 0; i < 256; ++i)
      {
        encoded_output.clear();

        etl::span<const int8_t> input_span(input_data_int8_t.data(), i);

        auto size = etl::base64::encode(input_span,
                                        encoded_output);

        std::string expected(encoded[i]);
        std::string actual(encoded_output.data(), size);

        CHECK_EQUAL(expected, actual);
        CHECK_EQUAL(etl::base64::encode_size(i), size);
      }
    }

    //*************************************************************************
    TEST(test_encode_int8_t_pointer_size)
    {
      std::array<char, 344U> encoded_output;

      for (size_t i = 0; i < 256; ++i)
      {
        encoded_output.fill(0);

        auto size = etl::base64::encode(input_data_int8_t.data(), i,
                                        encoded_output.data(), encoded_output.size());

        std::string expected(encoded[i]);
        std::string actual(encoded_output.data(), size);

        CHECK_EQUAL(expected, actual);
        CHECK_EQUAL(etl::base64::encode_size(i), size);
      }
    }

    //*************************************************************************
    TEST(test_encode_int8_t_pointer_pointer)
    {
      std::array<char, 344U> encoded_output;

      for (size_t i = 0; i < 256; ++i)
      {
        encoded_output.fill(0);

        auto size = etl::base64::encode(input_data_int8_t.data(), input_data_int8_t.data() + i,
          encoded_output.data(), encoded_output.data() + encoded_output.size());

        std::string expected(encoded[i]);
        std::string actual(encoded_output.data(), size);

        CHECK_EQUAL(expected, actual);
        CHECK_EQUAL(etl::base64::encode_size(i), size);
      }
    }

    //*************************************************************************
    TEST(test_encode_int8_t_span)
    {
      std::array<char, 344U> encoded_output;

      for (size_t i = 0; i < 256; ++i)
      {
        encoded_output.fill(0);

        etl::span<const int8_t> input_span(input_data_int8_t.data(), i);
        etl::span<char> output_span(encoded_output.data(), encoded_output.size());

        auto size = etl::base64::encode(input_span, output_span);

        std::string expected(encoded[i]);
        std::string actual(encoded_output.data(), size);

        CHECK_EQUAL(expected, actual);
        CHECK_EQUAL(etl::base64::encode_size(i), size);
      }
    }

    //*************************************************************************
#if ETL_USING_CPP14
    template <size_t Size>
    constexpr auto GetConstexprBase64(const etl::array<int8_t, Size> input) noexcept
    {
      constexpr size_t encode_size = etl::base64::encode_size(Size);
      etl::array<char, etl::base64::encode_size(Size)> output{ 0 };

      etl::base64::encode(input.begin(), Size,
        output._buffer, encode_size);

      return output;
    }

    TEST(test_encode_int8_t_constexpr)
    {
      constexpr etl::array<int8_t, 10> input = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };

      constexpr auto output{ GetConstexprBase64(input) };

      std::string expected("AAECAwQFBgcICQ==");
      std::string actual(output.data(), output.size());

      CHECK_EQUAL(expected, actual);
      CHECK_EQUAL(etl::base64::encode_size(10), output.size());
    }
#endif

    //*************************************************************************
    TEST(test_encode_overflow)
    {
      std::array<char, 1> encoded_output{ 0 };

      CHECK_THROW((etl::base64::encode(input_data_unsigned_char.data(), 10,
                                       encoded_output.data(), encoded_output.size())), etl::base64_overflow);
    }

    //*************************************************************************
    TEST(test_encode_overflow_for_etl_string)
    {
      etl::string<10> output;

      CHECK_THROW((etl::base64::encode(input_data_unsigned_char.data(), 10, output)), etl::base64_overflow);
    }

    //*************************************************************************
    TEST(test_decode_unsigned_char)
    {
      std::array<unsigned char, 256> decoded_output;

      for (size_t i = 0; i < 256; ++i)
      {
        decoded_output.fill(0);

        auto decoded_size = etl::base64::decode(encoded[i].data(), encoded[i].size(),
          decoded_output.data(), decoded_output.size());

        CHECK_ARRAY_EQUAL(input_data_unsigned_char.data(), decoded_output.data(), i);
        CHECK_EQUAL(i, etl::base64::decode_size(encoded[i].data(), encoded[i].size()));
        CHECK_EQUAL(i, decoded_size);
      }
    }

    //*************************************************************************
    TEST(test_decode_int8_t)
    {
      std::array<int8_t, 256> decoded_output;

      for (size_t i = 0; i < 256; ++i)
      {
        decoded_output.fill(0);

        auto decoded_size = etl::base64::decode(encoded[i].data(), encoded[i].size(),
          decoded_output.data(), decoded_output.size());

        CHECK_ARRAY_EQUAL(input_data_int8_t.data(), decoded_output.data(), i);
        CHECK_EQUAL(i, etl::base64::decode_size(encoded[i].data(), encoded[i].size()));
        CHECK_EQUAL(i, decoded_size);
      }
    }

    //*************************************************************************
    TEST(test_decode_overflow)
    {
      std::array<unsigned char, 1> decoded_output{ 0 };

      CHECK_THROW((etl::base64::decode(encoded[10].data(), encoded[10].size(),
        decoded_output.data(), decoded_output.size())), etl::base64_overflow);
    }
  };
}

