/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl
https://www.etlcpp.com

Copyright(c) 2019 John Wellbelove

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

#include <ostream>
#include <sstream>
#include <iomanip>

#include "etl/to_arithmetic.h"
#include "etl/string.h"
#include "etl/format_spec.h"

#undef STR
#define STR(x) x

namespace
{
  typedef etl::format_spec Format;

  SUITE(test_to_arithmetic)
  {
    TEST(test_to_integer_from_char_pointer_to_decimals)
    {
      const char* pp = " -128 ";
      const char* pn = " -123";

      const std::string huge_n("8000000000000001");
      const std::string huge_p("7FFFFFFFFFFFFFFF");

      //CHECK_EQUAL(int(-128), etl::to_integer<int8_t>(pp, strlen(pp), etl::dec).value());

      CHECK_EQUAL(std::numeric_limits<int64_t>::min(), etl::to_integer<int64_t>(huge_n.c_str(), huge_n.size(), etl::hex).value());
      CHECK_EQUAL(std::numeric_limits<int64_t>::max(), etl::to_integer<int64_t>(huge_p.c_str(), huge_p.size(), etl::hex).value());

      //CHECK_EQUAL(123, etl::to_integer<char>(pp, 3, etl::dec));
      //CHECK_EQUAL(123, etl::to_integer<char>(pp, 3, etl::radix::decimal));
      //CHECK_EQUAL(123, etl::to_integer<char>(pp, 3, etl::radix(etl::radix::decimal)));
      //CHECK_EQUAL(123, etl::to_integer<char>(pp, 3, 10));
    }
  }
}

