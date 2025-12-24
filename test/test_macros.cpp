/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl
https://www.etlcpp.com

Documentation: 

Copyright(c) 2022 John Wellbelove

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

#include "etl/platform.h"
#include "etl/macros.h"
#include "etl/char_traits.h"

#include <algorithm>

namespace
{
  SUITE(test_macros)
  {
    template <typename T>
    bool CheckEqualStrings(const T * expected, const T * actual)
    {
      return (etl::strlen(expected) == etl::strlen(actual)) &&
             std::equal(expected, expected + etl::strlen(expected), actual);
    }

    TEST(test_macros)
    {
      CHECK_TRUE(CheckEqualStrings("A, B, C, D, E, F", ETL_STRINGIFY(A, B, C, D, E, F)));

      CHECK_ARRAY_EQUAL( "ABCDEF",  ETL_STRING(ABCDEF),      etl::strlen("ABCDEF"));
      CHECK_ARRAY_EQUAL(L"ABCDEF",  ETL_WIDE_STRING(ABCDEF), etl::strlen(L"ABCDEF"));
      CHECK_ARRAY_EQUAL(u8"ABCDEF", ETL_U8_STRING(ABCDEF),   etl::strlen(u8"ABCDEF"));
      CHECK_ARRAY_EQUAL(u"ABCDEF",  ETL_U16_STRING(ABCDEF),  etl::strlen(u"ABCDEF"));
      CHECK_ARRAY_EQUAL(U"ABCDEF",  ETL_U32_STRING(ABCDEF),  etl::strlen(U"ABCDEF"));
    }
  };
}
