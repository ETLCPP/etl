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
    //*************************************************************************
    TEST(test_invalid_radixes)
    {
      const char* decimal = "128";
      CHECK_THROW(etl::to_arithmetic<int8_t>(decimal, strlen(decimal),  0), etl::to_arithmetic_radix_not_supported);
      CHECK_THROW(etl::to_arithmetic<int8_t>(decimal, strlen(decimal),  1), etl::to_arithmetic_radix_not_supported);
      CHECK_THROW(etl::to_arithmetic<int8_t>(decimal, strlen(decimal),  3), etl::to_arithmetic_radix_not_supported);
      CHECK_THROW(etl::to_arithmetic<int8_t>(decimal, strlen(decimal),  4), etl::to_arithmetic_radix_not_supported);
      CHECK_THROW(etl::to_arithmetic<int8_t>(decimal, strlen(decimal),  5), etl::to_arithmetic_radix_not_supported);
      CHECK_THROW(etl::to_arithmetic<int8_t>(decimal, strlen(decimal),  6), etl::to_arithmetic_radix_not_supported);
      CHECK_THROW(etl::to_arithmetic<int8_t>(decimal, strlen(decimal),  7), etl::to_arithmetic_radix_not_supported);
      CHECK_THROW(etl::to_arithmetic<int8_t>(decimal, strlen(decimal),  9), etl::to_arithmetic_radix_not_supported);
      CHECK_THROW(etl::to_arithmetic<int8_t>(decimal, strlen(decimal), 11), etl::to_arithmetic_radix_not_supported);
      CHECK_THROW(etl::to_arithmetic<int8_t>(decimal, strlen(decimal), 12), etl::to_arithmetic_radix_not_supported);
      CHECK_THROW(etl::to_arithmetic<int8_t>(decimal, strlen(decimal), 13), etl::to_arithmetic_radix_not_supported);
      CHECK_THROW(etl::to_arithmetic<int8_t>(decimal, strlen(decimal), 14), etl::to_arithmetic_radix_not_supported);
      CHECK_THROW(etl::to_arithmetic<int8_t>(decimal, strlen(decimal), 15), etl::to_arithmetic_radix_not_supported);
      CHECK_THROW(etl::to_arithmetic<int8_t>(decimal, strlen(decimal), 17), etl::to_arithmetic_radix_not_supported);
    }

    //*************************************************************************
    TEST(test_invalid_binary_numerics)
    {
      const char* decimal1 = " 101";
      const char* decimal2 = "101 ";
      const char* decimal3 = "121";
      const char* decimal4 = "-101";

      CHECK_THROW(etl::to_arithmetic<int8_t>(decimal1, strlen(decimal1), etl::bin), etl::to_arithmetic_invalid_format);
      CHECK_THROW(etl::to_arithmetic<int8_t>(decimal2, strlen(decimal2), etl::bin), etl::to_arithmetic_invalid_format);
      CHECK_THROW(etl::to_arithmetic<int8_t>(decimal3, strlen(decimal3), etl::bin), etl::to_arithmetic_invalid_format);
      CHECK_THROW(etl::to_arithmetic<int8_t>(decimal4, strlen(decimal4), etl::bin), etl::to_arithmetic_invalid_format);
    }

    //*************************************************************************
    TEST(test_valid_binary_numerics)
    {
      const std::string value1("0");
      const std::string value2("1");
      const std::string value3("0101");
      const std::string value4("1010");
      const std::string value5("01010011");
      const std::string value6("10101100");

      CHECK_EQUAL(int(0), int(etl::to_arithmetic<int8_t>(value1.c_str(), value1.size(), etl::bin).value()));
      CHECK_EQUAL(int(1), int(etl::to_arithmetic<int8_t>(value2.c_str(), value2.size(), etl::bin).value()));
      CHECK_EQUAL(int(5), int(etl::to_arithmetic<int8_t>(value3.c_str(), value3.size(), etl::bin).value()));
      CHECK_EQUAL(int(10), int(etl::to_arithmetic<int8_t>(value4.c_str(), value4.size(), etl::bin).value()));
      CHECK_EQUAL(int(83), int(etl::to_arithmetic<int8_t>(value5.c_str(), value5.size(), etl::bin).value()));
      CHECK_EQUAL(int(-84), int(etl::to_arithmetic<int8_t>(value6.c_str(), value6.size(), etl::bin).value()));
    }

    //*************************************************************************
    TEST(test_big_binary_numerics)
    {
      const std::string value1("1000000000000000000000000000000000000000000000000000000000000000");
      const std::string value2("0111111111111111111111111111111111111111111111111111111111111111");
      const std::string value3("1111111111111111111111111111111111111111111111111111111111111111");

      CHECK_EQUAL(uint64_t(0x8000000000000000ULL), etl::to_arithmetic<uint64_t>(value1.c_str(), value1.size(), etl::bin).value());
      CHECK_EQUAL(uint64_t(0x7FFFFFFFFFFFFFFFULL), etl::to_arithmetic<uint64_t>(value2.c_str(), value2.size(), etl::bin).value());
      CHECK_EQUAL(uint64_t(0xFFFFFFFFFFFFFFFFULL), etl::to_arithmetic<uint64_t>(value3.c_str(), value3.size(), etl::bin).value());
    }

    //*************************************************************************
    TEST(test_invalid_octal_numerics)
    {
      const char* decimal1 = " 127";
      const char* decimal2 = "127 ";
      const char* decimal3 = "187";
      const char* decimal4 = "-127";

      CHECK_THROW(etl::to_arithmetic<int8_t>(decimal1, strlen(decimal1), etl::oct), etl::to_arithmetic_invalid_format);
      CHECK_THROW(etl::to_arithmetic<int8_t>(decimal2, strlen(decimal2), etl::oct), etl::to_arithmetic_invalid_format);
      CHECK_THROW(etl::to_arithmetic<int8_t>(decimal3, strlen(decimal3), etl::oct), etl::to_arithmetic_invalid_format);
      CHECK_THROW(etl::to_arithmetic<int8_t>(decimal4, strlen(decimal4), etl::oct), etl::to_arithmetic_invalid_format);
    }

    //*************************************************************************
    TEST(test_valid_octal_numerics)
    {
      const std::string value1("0");
      const std::string value2("1");
      const std::string value3("5");
      const std::string value4("12");
      const std::string value5("123");
      const std::string value6("254");

      CHECK_EQUAL(int(0), int(etl::to_arithmetic<int8_t>(value1.c_str(), value1.size(), etl::oct).value()));
      CHECK_EQUAL(int(1), int(etl::to_arithmetic<int8_t>(value2.c_str(), value2.size(), etl::oct).value()));
      CHECK_EQUAL(int(5), int(etl::to_arithmetic<int8_t>(value3.c_str(), value3.size(), etl::oct).value()));
      CHECK_EQUAL(int(10), int(etl::to_arithmetic<int8_t>(value4.c_str(), value4.size(), etl::oct).value()));
      CHECK_EQUAL(int(83), int(etl::to_arithmetic<int8_t>(value5.c_str(), value5.size(), etl::oct).value()));
      CHECK_EQUAL(int(-84), int(etl::to_arithmetic<int8_t>(value6.c_str(), value6.size(), etl::oct).value()));
    }

    //*************************************************************************
    TEST(test_big_octal_numerics)
    {
      const std::string value1("1000000000000000000000");
      const std::string value2("0777777777777777777777");
      const std::string value3("1777777777777777777777");

      CHECK_EQUAL(uint64_t(0x8000000000000000ULL), etl::to_arithmetic<uint64_t>(value1.c_str(), value1.size(), etl::oct).value());
      CHECK_EQUAL(uint64_t(0x7FFFFFFFFFFFFFFFULL), etl::to_arithmetic<uint64_t>(value2.c_str(), value2.size(), etl::oct).value());
      CHECK_EQUAL(uint64_t(0xFFFFFFFFFFFFFFFFULL), etl::to_arithmetic<uint64_t>(value3.c_str(), value3.size(), etl::oct).value());
    }

    //*************************************************************************
    TEST(test_invalid_decimal_numerics)
    {
      const char* decimal1 = " 128";
      const char* decimal2 = "128 ";
      const char* decimal3 = "1A8";
      const char* decimal4 = "++128";
      const char* decimal5 = "-+128";
      const char* decimal6 = "+-128";
      const char* decimal7 = "--128";
      const char* decimal8 = "-127";

      CHECK_THROW(etl::to_arithmetic<int8_t>(decimal1, strlen(decimal1), etl::dec), etl::to_arithmetic_invalid_format);
      CHECK_THROW(etl::to_arithmetic<int8_t>(decimal2, strlen(decimal2), etl::dec), etl::to_arithmetic_invalid_format);
      CHECK_THROW(etl::to_arithmetic<int8_t>(decimal3, strlen(decimal3), etl::dec), etl::to_arithmetic_invalid_format);
      CHECK_THROW(etl::to_arithmetic<int8_t>(decimal4, strlen(decimal4), etl::dec), etl::to_arithmetic_invalid_format);
      CHECK_THROW(etl::to_arithmetic<int8_t>(decimal5, strlen(decimal5), etl::dec), etl::to_arithmetic_invalid_format);
      CHECK_THROW(etl::to_arithmetic<int8_t>(decimal6, strlen(decimal6), etl::dec), etl::to_arithmetic_invalid_format);
      CHECK_THROW(etl::to_arithmetic<int8_t>(decimal7, strlen(decimal7), etl::dec), etl::to_arithmetic_invalid_format);
    }

    //*************************************************************************
    TEST(test_valid_decimal_numerics)
    {
      const std::string value1("0");
      const std::string value2("1");
      const std::string value3("5");
      const std::string value4("+10");
      const std::string value5("83");
      const std::string value6("-84");

      CHECK_EQUAL(int(0), int(etl::to_arithmetic<int8_t>(value1.c_str(), value1.size(), etl::dec).value()));
      CHECK_EQUAL(int(1), int(etl::to_arithmetic<int8_t>(value2.c_str(), value2.size(), etl::dec).value()));
      CHECK_EQUAL(int(5), int(etl::to_arithmetic<int8_t>(value3.c_str(), value3.size(), etl::dec).value()));
      CHECK_EQUAL(int(10), int(etl::to_arithmetic<int8_t>(value4.c_str(), value4.size(), etl::dec).value()));
      CHECK_EQUAL(int(83), int(etl::to_arithmetic<int8_t>(value5.c_str(), value5.size(), etl::dec).value()));
      CHECK_EQUAL(int(-84), int(etl::to_arithmetic<int8_t>(value6.c_str(), value6.size(), etl::dec).value()));
    }

    //*************************************************************************
    TEST(test_big_decimal_numerics)
    {
      const std::string value1("-9223372036854775808");
      const std::string value2("9223372036854775807");
      const std::string value3("-1");

      CHECK_EQUAL(int64_t(0x8000000000000000ULL), etl::to_arithmetic<int64_t>(value1.c_str(), value1.size(), etl::dec).value());
      CHECK_EQUAL(int64_t(0x7FFFFFFFFFFFFFFFULL), etl::to_arithmetic<int64_t>(value2.c_str(), value2.size(), etl::dec).value());
      CHECK_EQUAL(int64_t(0xFFFFFFFFFFFFFFFFULL), etl::to_arithmetic<int64_t>(value3.c_str(), value3.size(), etl::dec).value());
    }

    //*************************************************************************
    TEST(test_invalid_hex_numerics)
    {
      const char* decimal1 = " 1Af";
      const char* decimal2 = "1Af ";
      const char* decimal3 = "1Gf";
      const char* decimal4 = "-1Af";

      CHECK_THROW(etl::to_arithmetic<int8_t>(decimal1, strlen(decimal1), etl::dec), etl::to_arithmetic_invalid_format);
      CHECK_THROW(etl::to_arithmetic<int8_t>(decimal2, strlen(decimal2), etl::dec), etl::to_arithmetic_invalid_format);
      CHECK_THROW(etl::to_arithmetic<int8_t>(decimal3, strlen(decimal3), etl::dec), etl::to_arithmetic_invalid_format);
      CHECK_THROW(etl::to_arithmetic<int8_t>(decimal4, strlen(decimal4), etl::dec), etl::to_arithmetic_invalid_format);
    }

    //*************************************************************************
    TEST(test_valid_hex_numerics)
    {
      const std::string value1("0");
      const std::string value2("1");
      const std::string value3("5");
      const std::string value4("a");
      const std::string value5("53");
      const std::string value6("Ac");

      CHECK_EQUAL(int(0), int(etl::to_arithmetic<int8_t>(value1.c_str(), value1.size(), etl::hex).value()));
      CHECK_EQUAL(int(1), int(etl::to_arithmetic<int8_t>(value2.c_str(), value2.size(), etl::hex).value()));
      CHECK_EQUAL(int(5), int(etl::to_arithmetic<int8_t>(value3.c_str(), value3.size(), etl::hex).value()));
      CHECK_EQUAL(int(10), int(etl::to_arithmetic<int8_t>(value4.c_str(), value4.size(), etl::hex).value()));
      CHECK_EQUAL(int(83), int(etl::to_arithmetic<int8_t>(value5.c_str(), value5.size(), etl::hex).value()));
      CHECK_EQUAL(int(-84), int(etl::to_arithmetic<int8_t>(value6.c_str(), value6.size(), etl::hex).value()));
    }

    //*************************************************************************
    TEST(test_big_hex_numerics)
    {
      const std::string value1("8000000000000000");
      const std::string value2("7FFFFFFFFFFFFFFF");
      const std::string value3("FFFFFFFFFFFFFFFF");

      CHECK_EQUAL(uint64_t(0x8000000000000000ULL), etl::to_arithmetic<uint64_t>(value1.c_str(), value1.size(), etl::hex).value());
      CHECK_EQUAL(uint64_t(0x7FFFFFFFFFFFFFFFULL), etl::to_arithmetic<uint64_t>(value2.c_str(), value2.size(), etl::hex).value());
      CHECK_EQUAL(uint64_t(0xFFFFFFFFFFFFFFFFULL), etl::to_arithmetic<uint64_t>(value3.c_str(), value3.size(), etl::hex).value());
    }
  }
}

