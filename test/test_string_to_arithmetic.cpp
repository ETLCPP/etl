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
      CHECK(!etl::to_arithmetic<int8_t>(text.c_str(), text.size(),  0));
      CHECK(!etl::to_arithmetic<int8_t>(text.c_str(), text.size(),  1));
      CHECK(!etl::to_arithmetic<int8_t>(text.c_str(), text.size(),  3));
      CHECK(!etl::to_arithmetic<int8_t>(text.c_str(), text.size(),  4));
      CHECK(!etl::to_arithmetic<int8_t>(text.c_str(), text.size(),  5));
      CHECK(!etl::to_arithmetic<int8_t>(text.c_str(), text.size(),  6));
      CHECK(!etl::to_arithmetic<int8_t>(text.c_str(), text.size(),  7));
      CHECK(!etl::to_arithmetic<int8_t>(text.c_str(), text.size(),  9));
      CHECK(!etl::to_arithmetic<int8_t>(text.c_str(), text.size(), 11));
      CHECK(!etl::to_arithmetic<int8_t>(text.c_str(), text.size(), 12));
      CHECK(!etl::to_arithmetic<int8_t>(text.c_str(), text.size(), 13));
      CHECK(!etl::to_arithmetic<int8_t>(text.c_str(), text.size(), 14));
      CHECK(!etl::to_arithmetic<int8_t>(text.c_str(), text.size(), 15));
      CHECK(!etl::to_arithmetic<int8_t>(text.c_str(), text.size(), 17));
    }

    //*************************************************************************
    TEST(test_invalid_binary_numerics)
    {
      const std::string text1(" 101");
      const std::string text2("101 ");
      const std::string text3("121");
      const std::string text4("");
      const std::string text5("-101");
      const std::string text6("100000000");
      const std::string text7("-100000000");

      CHECK(!etl::to_arithmetic<int8_t>(text1.c_str(), text1.size(), etl::bin));
      CHECK(!etl::to_arithmetic<int8_t>(text2.c_str(), text2.size(), etl::bin));
      CHECK(!etl::to_arithmetic<int8_t>(text3.c_str(), text3.size(), etl::bin));
      CHECK(!etl::to_arithmetic<int8_t>(text4.c_str(), text4.size(), etl::bin));
      CHECK(!etl::to_arithmetic<uint8_t>(text5.c_str(), text5.size(), etl::bin));
      CHECK(!etl::to_arithmetic<int8_t>(text6.c_str(), text6.size(), etl::bin));
      CHECK(!etl::to_arithmetic<int8_t>(text7.c_str(), text7.size(), etl::bin));
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
      const std::string value7("-01010100");

      CHECK_EQUAL(int(0),   int(etl::to_arithmetic<int8_t>(value1.c_str(), value1.size(), etl::bin).value()));
      CHECK_EQUAL(int(1),   int(etl::to_arithmetic<int8_t>(value2.c_str(), value2.size(), etl::bin).value()));
      CHECK_EQUAL(int(5),   int(etl::to_arithmetic<int8_t>(value3.c_str(), value3.size(), etl::bin).value()));
      CHECK_EQUAL(int(10),  int(etl::to_arithmetic<int8_t>(value4.c_str(), value4.size(), etl::bin).value()));
      CHECK_EQUAL(int(83),  int(etl::to_arithmetic<int8_t>(value5.c_str(), value5.size(), etl::bin).value()));
      CHECK_EQUAL(int(-84), int(etl::to_arithmetic<int8_t>(value6.c_str(), value6.size(), etl::bin).value()));
      CHECK_EQUAL(int(-84), int(etl::to_arithmetic<int8_t>(value7.c_str(), value7.size(), etl::bin).value()));
    }

    //*************************************************************************
    TEST(test_maximum_binary_numerics)
    {
      const std::string int8_max("01111111");
      const std::string int8_min_neg("-10000000");
      const std::string int8_min_2c("10000000");

      const std::string uint8_max("11111111");
      const std::string uint8_min("00000000");

      const std::string int16_max("0111111111111111");
      const std::string int16_min_neg("-1000000000000000");
      const std::string int16_min_2c("1000000000000000");

      const std::string uint16_max("1111111111111111");
      const std::string uint16_min("0000000000000000");

      const std::string int32_max("01111111111111111111111111111111");
      const std::string int32_min_neg("-10000000000000000000000000000000");
      const std::string int32_min_2c("10000000000000000000000000000000");

      const std::string uint32_max("11111111111111111111111111111111");
      const std::string uint32_min("0");

      const std::string int64_max("0111111111111111111111111111111111111111111111111111111111111111");
      const std::string int64_min_neg("-1000000000000000000000000000000000000000000000000000000000000000");
      const std::string int64_min_2c("1000000000000000000000000000000000000000000000000000000000000000");

      const std::string uint64_max("1111111111111111111111111111111111111111111111111111111111111111");
      const std::string uint64_min("0");

      CHECK_EQUAL(127,  etl::to_arithmetic<int8_t>(int8_max.c_str(),     int8_max.size(),     etl::bin));
      CHECK_EQUAL(-128, etl::to_arithmetic<int8_t>(int8_min_neg.c_str(), int8_min_neg.size(), etl::bin));
      CHECK_EQUAL(-128, etl::to_arithmetic<int8_t>(int8_min_2c.c_str(),  int8_min_2c.size(),  etl::bin));

      CHECK_EQUAL(255, etl::to_arithmetic<uint8_t>(uint8_max.c_str(), uint8_max.size(), etl::bin));
      CHECK_EQUAL(0,   etl::to_arithmetic<uint8_t>(uint8_min.c_str(), uint8_min.size(), etl::bin));

      CHECK_EQUAL(32767,  etl::to_arithmetic<int16_t>(int16_max.c_str(),     int16_max.size(),     etl::bin));
      CHECK_EQUAL(-32768, etl::to_arithmetic<int16_t>(int16_min_neg.c_str(), int16_min_neg.size(), etl::bin));
      CHECK_EQUAL(-32768, etl::to_arithmetic<int16_t>(int16_min_2c.c_str(),  int16_min_2c.size(),  etl::bin));

      CHECK_EQUAL(65535, etl::to_arithmetic<uint16_t>(uint16_max.c_str(), uint16_max.size(), etl::bin));
      CHECK_EQUAL(0,     etl::to_arithmetic<uint16_t>(uint16_min.c_str(), uint16_min.size(), etl::bin));

      CHECK_EQUAL(2147483647LL,  etl::to_arithmetic<int32_t>(int32_max.c_str(),     int32_max.size(),     etl::bin));
      CHECK_EQUAL(-2147483648LL, etl::to_arithmetic<int32_t>(int32_min_neg.c_str(), int32_min_neg.size(), etl::bin));
      CHECK_EQUAL(-2147483648LL, etl::to_arithmetic<int32_t>(int32_min_2c.c_str(),  int32_min_2c.size(),  etl::bin));

      CHECK_EQUAL(4294967295, etl::to_arithmetic<uint32_t>(uint32_max.c_str(), uint32_max.size(), etl::bin));
      CHECK_EQUAL(0,          etl::to_arithmetic<uint32_t>(uint32_min.c_str(), uint32_min.size(), etl::bin));

      CHECK_EQUAL(9223372036854775807LL,  etl::to_arithmetic<int64_t>(int64_max.c_str(), int64_max.size(),         etl::bin));
      CHECK_EQUAL(-9223372036854775808LL, etl::to_arithmetic<int64_t>(int64_min_neg.c_str(), int64_min_neg.size(), etl::bin));
      CHECK_EQUAL(-9223372036854775808LL, etl::to_arithmetic<int64_t>(int64_min_2c.c_str(), int64_min_2c.size(),   etl::bin));

      CHECK_EQUAL(18446744073709551615ULL, etl::to_arithmetic<uint64_t>(uint64_max.c_str(), uint64_max.size(), etl::bin));
      CHECK_EQUAL(0ULL,                    etl::to_arithmetic<uint64_t>(uint64_min.c_str(), uint64_min.size(), etl::bin));
    }

    //*************************************************************************
    TEST(test_binary_numerics_overflow)
    {
      const std::string int8_overflow_max("100000000");
      const std::string int8_overflow_min("-100000000");

      const std::string uint8_overflow_max("100000000");
      const std::string uint8_overflow_min("-00000001");

      const std::string int16_overflow_max("10000000000000000");
      const std::string int16_overflow_min("-10000000000000000");

      const std::string uint16_overflow_max("10000000000000000");
      const std::string uint16_overflow_min("-0000000000000001");

      const std::string int32_overflow_max("100000000000000000000000000000000");
      const std::string int32_overflow_min("-100000000000000000000000000000000");

      const std::string uint32_overflow_max("100000000000000000000000000000000");
      const std::string uint32_overflow_min("-00000000000000000000000000000001");

      const std::string int64_overflow_max("10000000000000000000000000000000000000000000000000000000000000000");
      const std::string int64_overflow_min("-10000000000000000000000000000000000000000000000000000000000000000");

      const std::string uint64_overflow_max("10000000000000000000000000000000000000000000000000000000000000000");
      const std::string uint64_overflow_min("-0000000000000000000000000000000000000000000000000000000000000001");

      CHECK(!etl::to_arithmetic<int8_t>(int8_overflow_max.c_str(), int8_overflow_max.size(), etl::bin));
      CHECK(!etl::to_arithmetic<int8_t>(int8_overflow_min.c_str(), int8_overflow_min.size(), etl::bin));

      CHECK(!etl::to_arithmetic<uint8_t>(uint8_overflow_max.c_str(), uint8_overflow_max.size(), etl::bin));
      CHECK(!etl::to_arithmetic<uint8_t>(uint8_overflow_min.c_str(), uint8_overflow_min.size(), etl::bin));

      CHECK(!etl::to_arithmetic<int16_t>(int16_overflow_max.c_str(), int16_overflow_max.size(), etl::bin));
      CHECK(!etl::to_arithmetic<int16_t>(int16_overflow_min.c_str(), int16_overflow_min.size(), etl::bin));

      CHECK(!etl::to_arithmetic<uint16_t>(uint16_overflow_max.c_str(), uint16_overflow_max.size(), etl::bin));
      CHECK(!etl::to_arithmetic<uint16_t>(uint16_overflow_min.c_str(), uint16_overflow_min.size(), etl::bin));

      CHECK(!etl::to_arithmetic<int32_t>(int32_overflow_max.c_str(), int32_overflow_max.size(), etl::bin));
      CHECK(!etl::to_arithmetic<int32_t>(int32_overflow_min.c_str(), int32_overflow_min.size(), etl::bin));

      CHECK(!etl::to_arithmetic<uint32_t>(uint32_overflow_max.c_str(), uint32_overflow_max.size(), etl::bin));
      CHECK(!etl::to_arithmetic<uint32_t>(uint32_overflow_min.c_str(), uint32_overflow_min.size(), etl::bin));

      CHECK(!etl::to_arithmetic<int64_t>(int64_overflow_max.c_str(), int64_overflow_max.size(), etl::bin));
      CHECK(!etl::to_arithmetic<int64_t>(int64_overflow_min.c_str(), int64_overflow_min.size(), etl::bin));

      CHECK(!etl::to_arithmetic<uint64_t>(uint64_overflow_max.c_str(), uint64_overflow_max.size(), etl::bin));
      CHECK(!etl::to_arithmetic<uint64_t>(uint64_overflow_min.c_str(), uint64_overflow_min.size(), etl::bin));
    }

    //*************************************************************************
    TEST(test_invalid_octal_numerics)
    {
      const std::string text1(" 127");
      const std::string text2("127 ");
      const std::string text3("187");
      const std::string text4("-187");

      CHECK(!etl::to_arithmetic<int8_t>(text1.c_str(),  text1.size(), etl::oct));
      CHECK(!etl::to_arithmetic<int8_t>(text2.c_str(),  text2.size(), etl::oct));
      CHECK(!etl::to_arithmetic<int8_t>(text3.c_str(),  text3.size(), etl::oct));
      CHECK(!etl::to_arithmetic<uint8_t>(text4.c_str(), text4.size(), etl::oct));
    }

    //*************************************************************************
    TEST(test_valid_octal_numerics)
    {
      const std::string value1("0");
      const std::string value2("01");
      const std::string value3("05");
      const std::string value4("012");
      const std::string value5("0123");
      const std::string value6("0254");
      const std::string value7("-0124");

      CHECK_EQUAL(int(0), int(etl::to_arithmetic<int8_t>(value1.c_str(), value1.size(), etl::oct).value()));
      CHECK_EQUAL(int(1), int(etl::to_arithmetic<int8_t>(value2.c_str(), value2.size(), etl::oct).value()));
      CHECK_EQUAL(int(5), int(etl::to_arithmetic<int8_t>(value3.c_str(), value3.size(), etl::oct).value()));
      CHECK_EQUAL(int(10), int(etl::to_arithmetic<int8_t>(value4.c_str(), value4.size(), etl::oct).value()));
      CHECK_EQUAL(int(83), int(etl::to_arithmetic<int8_t>(value5.c_str(), value5.size(), etl::oct).value()));
      CHECK_EQUAL(int(-84), int(etl::to_arithmetic<int8_t>(value6.c_str(), value6.size(), etl::oct).value()));
      CHECK_EQUAL(int(-84), int(etl::to_arithmetic<int8_t>(value7.c_str(), value7.size(), etl::oct).value()));
    }

    //*************************************************************************
    TEST(test_maximum_octal_numerics)
    {
      const std::string int8_max("177");
      const std::string int8_min_neg("-200");
      const std::string int8_min_2c("200");

      const std::string uint8_max("377");
      const std::string uint8_min("0");

      const std::string int16_max("77777");
      const std::string int16_min_neg("-100000");
      const std::string int16_min_2c("100000");

      const std::string uint16_max("177777");
      const std::string uint16_min("0");

      const std::string int32_max("17777777777");
      const std::string int32_min_neg("-20000000000");
      const std::string int32_min_2c("20000000000");

      const std::string uint32_max("37777777777");
      const std::string uint32_min("0");

      const std::string int64_max("777777777777777777777");
      const std::string int64_min_neg("-1000000000000000000000");
      const std::string int64_min_2c("1000000000000000000000");

      const std::string uint64_max("1777777777777777777777");
      const std::string uint64_min("0");

      CHECK_EQUAL(127,  etl::to_arithmetic<int8_t>(int8_max.c_str(), int8_max.size(),         etl::oct));
      CHECK_EQUAL(-128, etl::to_arithmetic<int8_t>(int8_min_neg.c_str(), int8_min_neg.size(), etl::oct));
      CHECK_EQUAL(-128, etl::to_arithmetic<int8_t>(int8_min_2c.c_str(), int8_min_2c.size(),   etl::oct));

      CHECK_EQUAL(255, etl::to_arithmetic<uint8_t>(uint8_max.c_str(), uint8_max.size(), etl::oct));
      CHECK_EQUAL(0,   etl::to_arithmetic<uint8_t>(uint8_min.c_str(), uint8_min.size(), etl::oct));

      CHECK_EQUAL(32767,  etl::to_arithmetic<int16_t>(int16_max.c_str(), int16_max.size(),         etl::oct));
      CHECK_EQUAL(-32768, etl::to_arithmetic<int16_t>(int16_min_neg.c_str(), int16_min_neg.size(), etl::oct));
      CHECK_EQUAL(-32768, etl::to_arithmetic<int16_t>(int16_min_2c.c_str(), int16_min_2c.size(),   etl::oct));

      CHECK_EQUAL(65535, etl::to_arithmetic<uint16_t>(uint16_max.c_str(), uint16_max.size(), etl::oct));
      CHECK_EQUAL(0,     etl::to_arithmetic<uint16_t>(uint16_min.c_str(), uint16_min.size(), etl::oct));

      CHECK_EQUAL(2147483647LL,  etl::to_arithmetic<int32_t>(int32_max.c_str(), int32_max.size(),         etl::oct));
      CHECK_EQUAL(-2147483648LL, etl::to_arithmetic<int32_t>(int32_min_neg.c_str(), int32_min_neg.size(), etl::oct));
      CHECK_EQUAL(-2147483648LL, etl::to_arithmetic<int32_t>(int32_min_2c.c_str(), int32_min_2c.size(),   etl::oct));

      CHECK_EQUAL(4294967295, etl::to_arithmetic<uint32_t>(uint32_max.c_str(), uint32_max.size(), etl::oct));
      CHECK_EQUAL(0,          etl::to_arithmetic<uint32_t>(uint32_min.c_str(), uint32_min.size(), etl::oct));

      CHECK_EQUAL(9223372036854775807LL,  etl::to_arithmetic<int64_t>(int64_max.c_str(), int64_max.size(),         etl::oct));
      CHECK_EQUAL(-9223372036854775808LL, etl::to_arithmetic<int64_t>(int64_min_neg.c_str(), int64_min_neg.size(), etl::oct));
      CHECK_EQUAL(-9223372036854775808LL, etl::to_arithmetic<int64_t>(int64_min_2c.c_str(), int64_min_2c.size(),   etl::oct));

      CHECK_EQUAL(18446744073709551615ULL, etl::to_arithmetic<uint64_t>(uint64_max.c_str(), uint64_max.size(), etl::oct));
      CHECK_EQUAL(0ULL,                    etl::to_arithmetic<uint64_t>(uint64_min.c_str(), uint64_min.size(), etl::oct));
    }

    //*************************************************************************
    TEST(test_octal_numerics_overflow)
    {
      const std::string int8_overflow_max("400");
      const std::string int8_overflow_min("-400");

      const std::string uint8_overflow_max("400");
      const std::string uint8_overflow_min("-001");

      const std::string int16_overflow_max("200000");
      const std::string int16_overflow_min("-200000");

      const std::string uint16_overflow_max("200000");
      const std::string uint16_overflow_min("-000001");

      const std::string int32_overflow_max("40000000000");
      const std::string int32_overflow_min("-40000000000");

      const std::string uint32_overflow_max("40000000000");
      const std::string uint32_overflow_min("-00000000001");

      const std::string int64_overflow_max("2000000000000000000000");
      const std::string int64_overflow_min("-2000000000000000000000");

      const std::string uint64_overflow_max("2000000000000000000000");
      const std::string uint64_overflow_min("-0000000000000000000001");

      CHECK(!etl::to_arithmetic<int8_t>(int8_overflow_max.c_str(), int8_overflow_max.size(), etl::oct));
      CHECK(!etl::to_arithmetic<int8_t>(int8_overflow_min.c_str(), int8_overflow_min.size(), etl::oct));

      CHECK(!etl::to_arithmetic<uint8_t>(uint8_overflow_max.c_str(), uint8_overflow_max.size(), etl::oct));
      CHECK(!etl::to_arithmetic<uint8_t>(uint8_overflow_min.c_str(), uint8_overflow_min.size(), etl::oct));

      CHECK(!etl::to_arithmetic<int16_t>(int16_overflow_max.c_str(), int16_overflow_max.size(), etl::oct));
      CHECK(!etl::to_arithmetic<int16_t>(int16_overflow_min.c_str(), int16_overflow_min.size(), etl::oct));

      CHECK(!etl::to_arithmetic<uint16_t>(uint16_overflow_max.c_str(), uint16_overflow_max.size(), etl::oct));
      CHECK(!etl::to_arithmetic<uint16_t>(uint16_overflow_min.c_str(), uint16_overflow_min.size(), etl::oct));

      CHECK(!etl::to_arithmetic<int32_t>(int32_overflow_max.c_str(), int32_overflow_max.size(), etl::oct));
      CHECK(!etl::to_arithmetic<int32_t>(int32_overflow_min.c_str(), int32_overflow_min.size(), etl::oct));

      CHECK(!etl::to_arithmetic<uint32_t>(uint32_overflow_max.c_str(), uint32_overflow_max.size(), etl::oct));
      CHECK(!etl::to_arithmetic<uint32_t>(uint32_overflow_min.c_str(), uint32_overflow_min.size(), etl::oct));

      CHECK(!etl::to_arithmetic<int64_t>(int64_overflow_max.c_str(), int64_overflow_max.size(), etl::oct));
      CHECK(!etl::to_arithmetic<int64_t>(int64_overflow_min.c_str(), int64_overflow_min.size(), etl::oct));

      CHECK(!etl::to_arithmetic<uint64_t>(uint64_overflow_max.c_str(), uint64_overflow_max.size(), etl::oct));
      CHECK(!etl::to_arithmetic<uint64_t>(uint64_overflow_min.c_str(), uint64_overflow_min.size(), etl::oct));
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

      CHECK(!etl::to_arithmetic<int8_t>(text1.c_str(), text1.size(), etl::dec));
      CHECK(!etl::to_arithmetic<int8_t>(text2.c_str(), text2.size(), etl::dec));
      CHECK(!etl::to_arithmetic<int8_t>(text3.c_str(), text3.size(), etl::dec));
      CHECK(!etl::to_arithmetic<int8_t>(text4.c_str(), text4.size(), etl::dec));
      CHECK(!etl::to_arithmetic<int8_t>(text5.c_str(), text5.size(), etl::dec));
      CHECK(!etl::to_arithmetic<int8_t>(text6.c_str(), text6.size(), etl::dec));
      CHECK(!etl::to_arithmetic<int8_t>(text7.c_str(), text7.size(), etl::dec));
      CHECK(!etl::to_arithmetic<int8_t>(text8.c_str(), text8.size(), etl::dec));
      CHECK(!etl::to_arithmetic<int8_t>(text9.c_str(), text9.size(), etl::dec));
      CHECK(!etl::to_arithmetic<int8_t>(text10.c_str(), text10.size(), etl::dec));
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
      CHECK_EQUAL(int(83), int(etl::to_arithmetic<uint8_t>(text5.c_str(), text5.size(), etl::dec).value()));
      CHECK_EQUAL(int(-84), int(etl::to_arithmetic<int8_t>(text6.c_str(), text6.size(), etl::dec).value()));
    }

    //*************************************************************************
    TEST(test_maximum_decimal_numerics)
    {
      const std::string int8_max("127");
      const std::string int8_min("-128");

      const std::string uint8_max("255");
      const std::string uint8_min("0");

      const std::string int16_max("32767");
      const std::string int16_min("-32768");

      const std::string uint16_max("65535");
      const std::string uint16_min("0");

      const std::string int32_max("2147483647");
      const std::string int32_min("-2147483648");

      const std::string uint32_max("4294967295");
      const std::string uint32_min("0");

      const std::string int64_max("9223372036854775807");
      const std::string int64_min("-9223372036854775808");

      const std::string uint64_max("18446744073709551615");
      const std::string uint64_min("0");

      CHECK_EQUAL( 127,  etl::to_arithmetic<int8_t>(int8_max.c_str(), int8_max.size(), etl::dec));
      CHECK_EQUAL(-128, etl::to_arithmetic<int8_t>(int8_min.c_str(), int8_min.size(), etl::dec));

      CHECK_EQUAL(255, etl::to_arithmetic<uint8_t>(uint8_max.c_str(), uint8_max.size(), etl::dec));
      CHECK_EQUAL(  0, etl::to_arithmetic<uint8_t>(uint8_min.c_str(), uint8_min.size(), etl::dec));

      CHECK_EQUAL( 32767, etl::to_arithmetic<int16_t>(int16_max.c_str(), int16_max.size(), etl::dec));
      CHECK_EQUAL(-32768, etl::to_arithmetic<int16_t>(int16_min.c_str(), int16_min.size(), etl::dec));

      CHECK_EQUAL(65535, etl::to_arithmetic<uint16_t>(uint16_max.c_str(), uint16_max.size(), etl::dec));
      CHECK_EQUAL(    0, etl::to_arithmetic<uint16_t>(uint16_min.c_str(), uint16_min.size(), etl::dec));

      CHECK_EQUAL( 2147483647LL, etl::to_arithmetic<int32_t>(int32_max.c_str(), int32_max.size(), etl::dec));
      CHECK_EQUAL(-2147483648LL, etl::to_arithmetic<int32_t>(int32_min.c_str(), int32_min.size(), etl::dec));

      CHECK_EQUAL(4294967295ULL, etl::to_arithmetic<uint32_t>(uint32_max.c_str(), uint32_max.size(), etl::dec));
      CHECK_EQUAL(         0ULL, etl::to_arithmetic<uint32_t>(uint32_min.c_str(), uint32_min.size(), etl::dec));

      CHECK_EQUAL( 9223372036854775807LL, etl::to_arithmetic<int64_t>(int64_max.c_str(), int64_max.size(), etl::dec));
      CHECK_EQUAL(-9223372036854775808LL, etl::to_arithmetic<int64_t>(int64_min.c_str(), int64_min.size(), etl::dec));

      CHECK_EQUAL(18446744073709551615ULL, etl::to_arithmetic<uint64_t>(uint64_max.c_str(), uint64_max.size(), etl::dec));
      CHECK_EQUAL(                   0ULL, etl::to_arithmetic<uint64_t>(uint64_min.c_str(), uint64_min.size(), etl::dec));
    }

    //*************************************************************************
    TEST(test_decimal_numerics_overflow)
    {
      const std::string int8_overflow_max("128");
      const std::string int8_overflow_min("-129");

      const std::string int8_overflow_max_plus("1000");
      const std::string int8_overflow_min_plus("-1000");

      const std::string uint8_overflow_max("256");
      const std::string uint8_overflow_min("-1");

      const std::string int16_overflow_max("32768");
      const std::string int16_overflow_min("-32769");
      
      const std::string uint16_overflow_max("65536");
      const std::string uint16_overflow_min("-1");

      const std::string int32_overflow_max("2147483648");
      const std::string int32_overflow_min("-2147483649");
      
      const std::string uint32_overflow_max("4294967296");
      const std::string uint32_overflow_min("-1");

      const std::string int64_overflow_max("9223372036854775808");
      const std::string int64_overflow_min("-9223372036854775809");

      const std::string uint64_overflow_max("18446744073709551616");
      const std::string uint64_overflow_min("-1");

      CHECK(!etl::to_arithmetic<int8_t>(int8_overflow_max.c_str(), int8_overflow_max.size(), etl::dec));
      CHECK(!etl::to_arithmetic<int8_t>(int8_overflow_min.c_str(), int8_overflow_min.size(), etl::dec));

      CHECK(!etl::to_arithmetic<int8_t>(int8_overflow_max_plus.c_str(), int8_overflow_max_plus.size(), etl::dec));
      CHECK(!etl::to_arithmetic<int8_t>(int8_overflow_min_plus.c_str(), int8_overflow_min_plus.size(), etl::dec));

      CHECK(!etl::to_arithmetic<uint8_t>(uint8_overflow_max.c_str(), uint8_overflow_max.size(), etl::dec));
      CHECK(!etl::to_arithmetic<uint8_t>(uint8_overflow_min.c_str(), uint8_overflow_min.size(), etl::dec));

      CHECK(!etl::to_arithmetic<int16_t>(int16_overflow_max.c_str(), int16_overflow_max.size(), etl::dec));
      CHECK(!etl::to_arithmetic<int16_t>(int16_overflow_min.c_str(), int16_overflow_min.size(), etl::dec));

      CHECK(!etl::to_arithmetic<uint16_t>(uint16_overflow_max.c_str(), uint16_overflow_max.size(), etl::dec));
      CHECK(!etl::to_arithmetic<uint16_t>(uint16_overflow_min.c_str(), uint16_overflow_min.size(), etl::dec));

      CHECK(!etl::to_arithmetic<int32_t>(int32_overflow_max.c_str(), int32_overflow_max.size(), etl::dec));
      CHECK(!etl::to_arithmetic<int32_t>(int32_overflow_min.c_str(), int32_overflow_min.size(), etl::dec));

      CHECK(!etl::to_arithmetic<uint32_t>(uint32_overflow_max.c_str(), uint32_overflow_max.size(), etl::dec));
      CHECK(!etl::to_arithmetic<uint32_t>(uint32_overflow_min.c_str(), uint32_overflow_min.size(), etl::dec));

      CHECK(!etl::to_arithmetic<int64_t>(int64_overflow_max.c_str(), int64_overflow_max.size(), etl::dec));
      CHECK(!etl::to_arithmetic<int64_t>(int64_overflow_min.c_str(), int64_overflow_min.size(), etl::dec));

      CHECK(!etl::to_arithmetic<uint64_t>(uint64_overflow_max.c_str(), uint64_overflow_max.size(), etl::dec));
      CHECK(!etl::to_arithmetic<uint64_t>(uint64_overflow_min.c_str(), uint64_overflow_min.size(), etl::dec));
    }

    //*************************************************************************
    TEST(test_invalid_hex_numerics)
    {
      const std::string text1(" 1Af");
      const std::string text2("1Af ");
      const std::string text3("1Gf");
      const std::string text4("-1Af");

      CHECK(!etl::to_arithmetic<int8_t>(text1.c_str(), text1.size(),  etl::hex));
      CHECK(!etl::to_arithmetic<int8_t>(text2.c_str(), text2.size(),  etl::hex));
      CHECK(!etl::to_arithmetic<int8_t>(text3.c_str(), text3.size(),  etl::hex));
      CHECK(!etl::to_arithmetic<uint8_t>(text4.c_str(), text4.size(), etl::hex));
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
      const std::string text7("-54");

      CHECK_EQUAL(int(0),   int(etl::to_arithmetic<int8_t>(text1.c_str(), text1.size(), etl::hex).value()));
      CHECK_EQUAL(int(1),   int(etl::to_arithmetic<int8_t>(text2.c_str(), text2.size(), etl::hex).value()));
      CHECK_EQUAL(int(5),   int(etl::to_arithmetic<int8_t>(text3.c_str(), text3.size(), etl::hex).value()));
      CHECK_EQUAL(int(10),  int(etl::to_arithmetic<int8_t>(text4.c_str(), text4.size(), etl::hex).value()));
      CHECK_EQUAL(int(83),  int(etl::to_arithmetic<int8_t>(text5.c_str(), text5.size(), etl::hex).value()));
      CHECK_EQUAL(int(-84), int(etl::to_arithmetic<int8_t>(text6.c_str(), text6.size(), etl::hex).value()));
      CHECK_EQUAL(int(-84), int(etl::to_arithmetic<int8_t>(text7.c_str(), text7.size(), etl::hex).value()));
    }

    //*************************************************************************
    TEST(test_maximum_hex_numerics)
    {
      const std::string int8_max("7F");
      const std::string int8_min_neg("-80");
      const std::string int8_min_2c("80");

      const std::string uint8_max("FF");
      const std::string uint8_min("0");

      const std::string int16_max("7FFF");
      const std::string int16_min_neg("-8000");
      const std::string int16_min_2c("8000");

      const std::string uint16_max("FFFF");
      const std::string uint16_min("0");

      const std::string int32_max("7FFFFFFF");
      const std::string int32_min_neg("-80000000");
      const std::string int32_min_2c("80000000");

      const std::string uint32_max("FFFFFFFF");
      const std::string uint32_min("0");

      const std::string int64_max("7FFFFFFFFFFFFFFF");
      const std::string int64_min_neg("-8000000000000000");
      const std::string int64_min_2c("8000000000000000");

      const std::string uint64_max("FFFFFFFFFFFFFFFF");
      const std::string uint64_min("0");

      CHECK_EQUAL(127,  etl::to_arithmetic<int8_t>(int8_max.c_str(), int8_max.size(), etl::hex));
      CHECK_EQUAL(-128, etl::to_arithmetic<int8_t>(int8_min_neg.c_str(), int8_min_neg.size(), etl::hex));
      CHECK_EQUAL(-128, etl::to_arithmetic<int8_t>(int8_min_2c.c_str(), int8_min_2c.size(),   etl::hex));

      CHECK_EQUAL(255, etl::to_arithmetic<uint8_t>(uint8_max.c_str(), uint8_max.size(), etl::hex));
      CHECK_EQUAL(0,   etl::to_arithmetic<uint8_t>(uint8_min.c_str(), uint8_min.size(), etl::hex));

      CHECK_EQUAL(32767,  etl::to_arithmetic<int16_t>(int16_max.c_str(), int16_max.size(),         etl::hex));
      CHECK_EQUAL(-32768, etl::to_arithmetic<int16_t>(int16_min_neg.c_str(), int16_min_neg.size(), etl::hex));
      CHECK_EQUAL(-32768, etl::to_arithmetic<int16_t>(int16_min_2c.c_str(), int16_min_2c.size(),   etl::hex));

      CHECK_EQUAL(65535, etl::to_arithmetic<uint16_t>(uint16_max.c_str(), uint16_max.size(), etl::hex));
      CHECK_EQUAL(0,     etl::to_arithmetic<uint16_t>(uint16_min.c_str(), uint16_min.size(), etl::hex));

      CHECK_EQUAL(2147483647LL,  etl::to_arithmetic<int32_t>(int32_max.c_str(), int32_max.size(),         etl::hex));
      CHECK_EQUAL(-2147483648LL, etl::to_arithmetic<int32_t>(int32_min_neg.c_str(), int32_min_neg.size(), etl::hex));
      CHECK_EQUAL(-2147483648LL, etl::to_arithmetic<int32_t>(int32_min_2c.c_str(), int32_min_2c.size(),   etl::hex));

      CHECK_EQUAL(4294967295, etl::to_arithmetic<uint32_t>(uint32_max.c_str(), uint32_max.size(), etl::hex));
      CHECK_EQUAL(0,          etl::to_arithmetic<uint32_t>(uint32_min.c_str(), uint32_min.size(), etl::hex));

      CHECK_EQUAL(9223372036854775807LL,  etl::to_arithmetic<int64_t>(int64_max.c_str(), int64_max.size(),         etl::hex));
      CHECK_EQUAL(-9223372036854775808LL, etl::to_arithmetic<int64_t>(int64_min_neg.c_str(), int64_min_neg.size(), etl::hex));
      CHECK_EQUAL(-9223372036854775808LL, etl::to_arithmetic<int64_t>(int64_min_2c.c_str(), int64_min_2c.size(),   etl::hex));

      CHECK_EQUAL(18446744073709551615ULL, etl::to_arithmetic<uint64_t>(uint64_max.c_str(), uint64_max.size(), etl::hex));
      CHECK_EQUAL(0ULL,                    etl::to_arithmetic<uint64_t>(uint64_min.c_str(), uint64_min.size(), etl::hex));
    }

    //*************************************************************************
    TEST(test_hex_numerics_overflow)
    {
      const std::string int8_overflow_max("100");
      const std::string int8_overflow_min("-100");

      const std::string uint8_overflow_max("100");
      const std::string uint8_overflow_min("-01");

      const std::string int16_overflow_max("10000");
      const std::string int16_overflow_min("-10000");

      const std::string uint16_overflow_max("10000");
      const std::string uint16_overflow_min("-01");

      const std::string int32_overflow_max("100000000");
      const std::string int32_overflow_min("-100000000");

      const std::string uint32_overflow_max("100000000");
      const std::string uint32_overflow_min("-000000001");

      const std::string int64_overflow_max("10000000000000000");
      const std::string int64_overflow_min("-10000000000000000");

      const std::string uint64_overflow_max("10000000000000000");
      const std::string uint64_overflow_min("-0000000000000001");

      CHECK(!etl::to_arithmetic<int8_t>(int8_overflow_max.c_str(), int8_overflow_max.size(), etl::hex));
      CHECK(!etl::to_arithmetic<int8_t>(int8_overflow_min.c_str(), int8_overflow_min.size(), etl::hex));

      CHECK(!etl::to_arithmetic<uint8_t>(uint8_overflow_max.c_str(), uint8_overflow_max.size(), etl::hex));
      CHECK(!etl::to_arithmetic<uint8_t>(uint8_overflow_min.c_str(), uint8_overflow_min.size(), etl::hex));

      CHECK(!etl::to_arithmetic<int16_t>(int16_overflow_max.c_str(), int16_overflow_max.size(), etl::hex));
      CHECK(!etl::to_arithmetic<int16_t>(int16_overflow_min.c_str(), int16_overflow_min.size(), etl::hex));

      CHECK(!etl::to_arithmetic<uint16_t>(uint16_overflow_max.c_str(), uint16_overflow_max.size(), etl::hex));
      CHECK(!etl::to_arithmetic<uint16_t>(uint16_overflow_min.c_str(), uint16_overflow_min.size(), etl::hex));

      CHECK(!etl::to_arithmetic<int32_t>(int32_overflow_max.c_str(), int32_overflow_max.size(), etl::hex));
      CHECK(!etl::to_arithmetic<int32_t>(int32_overflow_min.c_str(), int32_overflow_min.size(), etl::hex));

      CHECK(!etl::to_arithmetic<uint32_t>(uint32_overflow_max.c_str(), uint32_overflow_max.size(), etl::hex));
      CHECK(!etl::to_arithmetic<uint32_t>(uint32_overflow_min.c_str(), uint32_overflow_min.size(), etl::hex));

      CHECK(!etl::to_arithmetic<int64_t>(int64_overflow_max.c_str(), int64_overflow_max.size(), etl::hex));
      CHECK(!etl::to_arithmetic<int64_t>(int64_overflow_min.c_str(), int64_overflow_min.size(), etl::hex));

      CHECK(!etl::to_arithmetic<uint64_t>(uint64_overflow_max.c_str(), uint64_overflow_max.size(), etl::hex));
      CHECK(!etl::to_arithmetic<uint64_t>(uint64_overflow_min.c_str(), uint64_overflow_min.size(), etl::hex));
    }

    //*************************************************************************
    TEST(test_valid_float)
    {
    //  const std::string text1("-123.456789");

    //  float f1 = strtof(text1.c_str(), nullptr);
    //  float f2 = etl::to_arithmetic<float>(text1.c_str(), text1.size()).value();

    // CHECK_EQUAL(f1, f2);

    // const std::string text2("-1.23456789e2");
    // float f3 = etl::to_arithmetic<float>(text2.c_str(), text2.size()).value();

    // CHECK_EQUAL(f1, f3);
    }

    //*************************************************************************
    TEST(test_valid_double)
    {
    //  const std::string text1("-123.45678901234567");

    //  double f1 = strtod(text1.c_str(), nullptr);
    //  double f2 = etl::to_arithmetic<double>(text1.c_str(), text1.size()).value();

    //  CHECK_EQUAL(f1, f2);
    }

    //*************************************************************************
    TEST(test_constexpr_integral)
    {
    //  constexpr const char* text{ "123" };
    //  constexpr etl::string_view view(text, 3);

    //  constexpr etl::optional<int> opt = etl::to_arithmetic<int>(view, etl::radix::decimal);
    //  constexpr int i = opt.value();

    //  CHECK_EQUAL(123, i);
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

