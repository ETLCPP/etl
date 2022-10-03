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
      const std::string text("128");
      CHECK_THROW((void)etl::to_arithmetic<int8_t>(text.c_str(), text.size(),  0), etl::to_arithmetic_invalid_radix);
      CHECK_THROW((void)etl::to_arithmetic<int8_t>(text.c_str(), text.size(),  1), etl::to_arithmetic_invalid_radix);
      CHECK_THROW((void)etl::to_arithmetic<int8_t>(text.c_str(), text.size(),  3), etl::to_arithmetic_invalid_radix);
      CHECK_THROW((void)etl::to_arithmetic<int8_t>(text.c_str(), text.size(),  4), etl::to_arithmetic_invalid_radix);
      CHECK_THROW((void)etl::to_arithmetic<int8_t>(text.c_str(), text.size(),  5), etl::to_arithmetic_invalid_radix);
      CHECK_THROW((void)etl::to_arithmetic<int8_t>(text.c_str(), text.size(),  6), etl::to_arithmetic_invalid_radix);
      CHECK_THROW((void)etl::to_arithmetic<int8_t>(text.c_str(), text.size(),  7), etl::to_arithmetic_invalid_radix);
      CHECK_THROW((void)etl::to_arithmetic<int8_t>(text.c_str(), text.size(),  9), etl::to_arithmetic_invalid_radix);
      CHECK_THROW((void)etl::to_arithmetic<int8_t>(text.c_str(), text.size(), 11), etl::to_arithmetic_invalid_radix);
      CHECK_THROW((void)etl::to_arithmetic<int8_t>(text.c_str(), text.size(), 12), etl::to_arithmetic_invalid_radix);
      CHECK_THROW((void)etl::to_arithmetic<int8_t>(text.c_str(), text.size(), 13), etl::to_arithmetic_invalid_radix);
      CHECK_THROW((void)etl::to_arithmetic<int8_t>(text.c_str(), text.size(), 14), etl::to_arithmetic_invalid_radix);
      CHECK_THROW((void)etl::to_arithmetic<int8_t>(text.c_str(), text.size(), 15), etl::to_arithmetic_invalid_radix);
      CHECK_THROW((void)etl::to_arithmetic<int8_t>(text.c_str(), text.size(), 17), etl::to_arithmetic_invalid_radix);
    }

    //*************************************************************************
    TEST(test_invalid_binary_numerics)
    {
      const std::string text1(" 101");
      const std::string text2("101 ");
      const std::string text3("121");
      const std::string text4("");
      const std::string text5("-101");

      CHECK_THROW((void)etl::to_arithmetic<int8_t>(text1.c_str(), text1.size(), etl::bin), etl::to_arithmetic_invalid_format);
      CHECK_THROW((void)etl::to_arithmetic<int8_t>(text2.c_str(), text2.size(), etl::bin), etl::to_arithmetic_invalid_format);
      CHECK_THROW((void)etl::to_arithmetic<int8_t>(text3.c_str(), text3.size(), etl::bin), etl::to_arithmetic_invalid_format);
      CHECK_THROW((void)etl::to_arithmetic<int8_t>(text4.c_str(), text4.size(), etl::bin), etl::to_arithmetic_invalid_format);
      CHECK_THROW((void)etl::to_arithmetic<uint8_t>(text5.c_str(), text5.size(), etl::bin), etl::to_arithmetic_signed_to_unsigned);
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
      const std::string value7("-01010011");

      CHECK_EQUAL(int(0),   int(etl::to_arithmetic<int8_t>(value1.c_str(), value1.size(), etl::bin).value()));
      CHECK_EQUAL(int(1),   int(etl::to_arithmetic<int8_t>(value2.c_str(), value2.size(), etl::bin).value()));
      CHECK_EQUAL(int(5),   int(etl::to_arithmetic<int8_t>(value3.c_str(), value3.size(), etl::bin).value()));
      CHECK_EQUAL(int(10),  int(etl::to_arithmetic<int8_t>(value4.c_str(), value4.size(), etl::bin).value()));
      CHECK_EQUAL(int(83),  int(etl::to_arithmetic<int8_t>(value5.c_str(), value5.size(), etl::bin).value()));
      CHECK_EQUAL(int(-84), int(etl::to_arithmetic<int8_t>(value6.c_str(), value6.size(), etl::bin).value()));
      CHECK_EQUAL(int(-83), int(etl::to_arithmetic<int8_t>(value7.c_str(), value7.size(), etl::bin).value()));
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
      const std::string text1(" 127");
      const std::string text2("127 ");
      const std::string text3("187");
      const std::string text4("-187");

      CHECK_THROW((void)etl::to_arithmetic<int8_t>(text1.c_str(), text1.size(), etl::oct), etl::to_arithmetic_invalid_format);
      CHECK_THROW((void)etl::to_arithmetic<int8_t>(text2.c_str(), text2.size(), etl::oct), etl::to_arithmetic_invalid_format);
      CHECK_THROW((void)etl::to_arithmetic<int8_t>(text3.c_str(), text3.size(), etl::oct), etl::to_arithmetic_invalid_format);
      CHECK_THROW((void)etl::to_arithmetic<uint8_t>(text4.c_str(), text4.size(), etl::oct), etl::to_arithmetic_signed_to_unsigned);
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
      const std::string value7("-123");

      CHECK_EQUAL(int(0),   int(etl::to_arithmetic<int8_t>(value1.c_str(), value1.size(), etl::oct).value()));
      CHECK_EQUAL(int(1),   int(etl::to_arithmetic<int8_t>(value2.c_str(), value2.size(), etl::oct).value()));
      CHECK_EQUAL(int(5),   int(etl::to_arithmetic<int8_t>(value3.c_str(), value3.size(), etl::oct).value()));
      CHECK_EQUAL(int(10),  int(etl::to_arithmetic<int8_t>(value4.c_str(), value4.size(), etl::oct).value()));
      CHECK_EQUAL(int(83),  int(etl::to_arithmetic<int8_t>(value5.c_str(), value5.size(), etl::oct).value()));
      CHECK_EQUAL(int(-84), int(etl::to_arithmetic<int8_t>(value6.c_str(), value6.size(), etl::oct).value()));
      CHECK_EQUAL(int(-83), int(etl::to_arithmetic<int8_t>(value7.c_str(), value7.size(), etl::oct).value()));
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
      const std::string text1(" 128");
      const std::string text2("128 ");
      const std::string text3("1A8");
      const std::string text4("++128");
      const std::string text5("-+128");
      const std::string text6("+-128");
      const std::string text7("--128");
      const std::string text8("+");
      const std::string text9("-");
      const std::string text10("");

      CHECK_THROW((void)etl::to_arithmetic<int8_t>(text1.c_str(), text1.size(), etl::dec), etl::to_arithmetic_invalid_format);
      CHECK_THROW((void)etl::to_arithmetic<int8_t>(text2.c_str(), text2.size(), etl::dec), etl::to_arithmetic_invalid_format);
      CHECK_THROW((void)etl::to_arithmetic<int8_t>(text3.c_str(), text3.size(), etl::dec), etl::to_arithmetic_invalid_format);
      CHECK_THROW((void)etl::to_arithmetic<int8_t>(text4.c_str(), text4.size(), etl::dec), etl::to_arithmetic_invalid_format);
      CHECK_THROW((void)etl::to_arithmetic<int8_t>(text5.c_str(), text5.size(), etl::dec), etl::to_arithmetic_invalid_format);
      CHECK_THROW((void)etl::to_arithmetic<int8_t>(text6.c_str(), text6.size(), etl::dec), etl::to_arithmetic_invalid_format);
      CHECK_THROW((void)etl::to_arithmetic<int8_t>(text7.c_str(), text7.size(), etl::dec), etl::to_arithmetic_invalid_format);
      CHECK_THROW((void)etl::to_arithmetic<int8_t>(text8.c_str(), text8.size(), etl::dec), etl::to_arithmetic_invalid_format);
      CHECK_THROW((void)etl::to_arithmetic<int8_t>(text9.c_str(), text9.size(), etl::dec), etl::to_arithmetic_invalid_format);
      CHECK_THROW((void)etl::to_arithmetic<int8_t>(text10.c_str(), text10.size(), etl::dec), etl::to_arithmetic_invalid_format);
    }

    //*************************************************************************
    TEST(test_valid_decimal_numerics)
    {
      const std::string text1("0");
      const std::string text2("1");
      const std::string text3("5");
      const std::string text4("+10");
      const std::string text5("83");
      const std::string text6("-84");

      CHECK_EQUAL(int(0),   int(etl::to_arithmetic<int8_t>(text1.c_str(), text1.size(), etl::dec).value()));
      CHECK_EQUAL(int(1),   int(etl::to_arithmetic<int8_t>(text2.c_str(), text2.size(), etl::dec).value()));
      CHECK_EQUAL(int(5),   int(etl::to_arithmetic<int8_t>(text3.c_str(), text3.size(), etl::dec).value()));
      CHECK_EQUAL(int(10),  int(etl::to_arithmetic<int8_t>(text4.c_str(), text4.size(), etl::dec).value()));
      CHECK_EQUAL(int(83),  int(etl::to_arithmetic<int8_t>(text5.c_str(), text5.size(), etl::dec).value()));
      CHECK_EQUAL(int(-84), int(etl::to_arithmetic<int8_t>(text6.c_str(), text6.size(), etl::dec).value()));
    }

    //*************************************************************************
    TEST(test_big_decimal_numerics)
    {
      const std::string text1("-9223372036854775808");
      const std::string text2("9223372036854775807");
      const std::string text3("-1");

      CHECK_EQUAL(-9223372036854775808LL, etl::to_arithmetic<int64_t>(text1.c_str(), text1.size(), etl::dec).value());
      CHECK_EQUAL(9223372036854775807LL,  etl::to_arithmetic<int64_t>(text2.c_str(), text2.size(), etl::dec).value());
      CHECK_EQUAL(-1LL,                   etl::to_arithmetic<int64_t>(text3.c_str(), text3.size(), etl::dec).value());
    }

    //*************************************************************************
    TEST(test_invalid_hex_numerics)
    {
      const std::string text1(" 1Af");
      const std::string text2("1Af ");
      const std::string text3("1Gf");
      const std::string text4("-1Af");

      CHECK_THROW((void)etl::to_arithmetic<int8_t>(text1.c_str(), text1.size(), etl::dec), etl::to_arithmetic_invalid_format);
      CHECK_THROW((void)etl::to_arithmetic<int8_t>(text2.c_str(), text2.size(), etl::dec), etl::to_arithmetic_invalid_format);
      CHECK_THROW((void)etl::to_arithmetic<int8_t>(text3.c_str(), text3.size(), etl::dec), etl::to_arithmetic_invalid_format);
      CHECK_THROW((void)etl::to_arithmetic<uint8_t>(text4.c_str(), text4.size(), etl::dec), etl::to_arithmetic_signed_to_unsigned);
    }

    //*************************************************************************
    TEST(test_valid_hex_numerics)
    {
      const std::string text1("0");
      const std::string text2("1");
      const std::string text3("5");
      const std::string text4("a");
      const std::string text5("53");
      const std::string text6("Ac");
      const std::string text7("-53");

      CHECK_EQUAL(int(0),   int(etl::to_arithmetic<int8_t>(text1.c_str(), text1.size(), etl::hex).value()));
      CHECK_EQUAL(int(1),   int(etl::to_arithmetic<int8_t>(text2.c_str(), text2.size(), etl::hex).value()));
      CHECK_EQUAL(int(5),   int(etl::to_arithmetic<int8_t>(text3.c_str(), text3.size(), etl::hex).value()));
      CHECK_EQUAL(int(10),  int(etl::to_arithmetic<int8_t>(text4.c_str(), text4.size(), etl::hex).value()));
      CHECK_EQUAL(int(83),  int(etl::to_arithmetic<int8_t>(text5.c_str(), text5.size(), etl::hex).value()));
      CHECK_EQUAL(int(-84), int(etl::to_arithmetic<int8_t>(text6.c_str(), text6.size(), etl::hex).value()));
      CHECK_EQUAL(int(-83), int(etl::to_arithmetic<int8_t>(text7.c_str(), text7.size(), etl::hex).value()));
    }

    //*************************************************************************
    TEST(test_big_hex_numerics)
    {
      const std::string text1("8000000000000000");
      const std::string text2("7FFFFFFFFFFFFFFF");
      const std::string text3("FFFFFFFFFFFFFFFF");

      CHECK_EQUAL(uint64_t(0x8000000000000000ULL), etl::to_arithmetic<uint64_t>(text1.c_str(), text1.size(), etl::hex).value());
      CHECK_EQUAL(uint64_t(0x7FFFFFFFFFFFFFFFULL), etl::to_arithmetic<uint64_t>(text2.c_str(), text2.size(), etl::hex).value());
      CHECK_EQUAL(uint64_t(0xFFFFFFFFFFFFFFFFULL), etl::to_arithmetic<uint64_t>(text3.c_str(), text3.size(), etl::hex).value());
    }

    //*************************************************************************
    TEST(test_valid_float)
    {
      const std::string text1("-123.456789");

      float f1 = strtof(text1.c_str(), nullptr);
      float f2 = etl::to_arithmetic<float>(text1.c_str(), text1.size()).value();

     CHECK_EQUAL(f1, f2);

     const std::string text2("-1.23456789e2");
     float f3 = etl::to_arithmetic<float>(text2.c_str(), text2.size()).value();

     CHECK_EQUAL(f1, f3);
    }

    //*************************************************************************
    //TEST(test_valid_double)
    //{
    //  const std::string text1("-123.45678901234567");

    //  double f1 = strtod(text1.c_str(), nullptr);
    //  double f2 = etl::to_arithmetic<double>(text1.c_str(), text1.size()).value();

    //  CHECK_EQUAL(f1, f2);
    //}

    //*************************************************************************
    TEST(test_constexpr_integral)
    {
      constexpr const char* text{ "123" };
      constexpr etl::string_view view(text, 3);

      constexpr etl::optional<int> opt = etl::to_arithmetic<int>(view, etl::radix::decimal);
      constexpr int i = opt.value();

      CHECK_EQUAL(123, i);
    }

    //*************************************************************************
    TEST(test_constexpr_floating_point)
    {
      //constexpr const char* text{ "123.456789" };
      //constexpr etl::string_view view(text, 3);

      //constexpr etl::optional<double> opt = etl::to_arithmetic<double>(view);
      //constexpr double i = opt.value();

      //CHECK_EQUAL(123, i);
    }
  }
}

