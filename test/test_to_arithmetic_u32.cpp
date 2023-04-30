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
#include <limits>

#include "etl/to_arithmetic.h"
#include "etl/u32string.h"
#include "etl/format_spec.h"

#undef STR
#define STR(x) U##x
using Text = std::u32string;

namespace
{
  //*************************************************************************
  template <typename T>
  std::ostream& operator <<(std::ostream& os, const etl::to_arithmetic_result<T>& result)
  {
    if (result.has_value())
    {
      os << result.value();
    }
    else
    {
      os << result.error().c_str();
    }

    return os;
  }

  typedef etl::format_spec Format;

  SUITE(test_to_arithmetic)
  {
    //*************************************************************************
    TEST(test_invalid_radixes)
    {
      const Text text(STR("128"));
      CHECK(!etl::to_arithmetic<int8_t>(text.c_str(), text.size(), 0));
      CHECK(!etl::to_arithmetic<int8_t>(text.c_str(), text.size(), 1));
      CHECK(!etl::to_arithmetic<int8_t>(text.c_str(), text.size(), 3));
      CHECK(!etl::to_arithmetic<int8_t>(text.c_str(), text.size(), 4));
      CHECK(!etl::to_arithmetic<int8_t>(text.c_str(), text.size(), 5));
      CHECK(!etl::to_arithmetic<int8_t>(text.c_str(), text.size(), 6));
      CHECK(!etl::to_arithmetic<int8_t>(text.c_str(), text.size(), 7));
      CHECK(!etl::to_arithmetic<int8_t>(text.c_str(), text.size(), 9));
      CHECK(!etl::to_arithmetic<int8_t>(text.c_str(), text.size(), 11));
      CHECK(!etl::to_arithmetic<int8_t>(text.c_str(), text.size(), 12));
      CHECK(!etl::to_arithmetic<int8_t>(text.c_str(), text.size(), 13));
      CHECK(!etl::to_arithmetic<int8_t>(text.c_str(), text.size(), 14));
      CHECK(!etl::to_arithmetic<int8_t>(text.c_str(), text.size(), 15));
      CHECK(!etl::to_arithmetic<int8_t>(text.c_str(), text.size(), 17));

      CHECK(!etl::to_arithmetic<int8_t>(text.c_str(), text.size(), 0).has_value());
      CHECK(!etl::to_arithmetic<int8_t>(text.c_str(), text.size(), 1).has_value());
      CHECK(!etl::to_arithmetic<int8_t>(text.c_str(), text.size(), 3).has_value());
      CHECK(!etl::to_arithmetic<int8_t>(text.c_str(), text.size(), 4).has_value());
      CHECK(!etl::to_arithmetic<int8_t>(text.c_str(), text.size(), 5).has_value());
      CHECK(!etl::to_arithmetic<int8_t>(text.c_str(), text.size(), 6).has_value());
      CHECK(!etl::to_arithmetic<int8_t>(text.c_str(), text.size(), 7).has_value());
      CHECK(!etl::to_arithmetic<int8_t>(text.c_str(), text.size(), 9).has_value());
      CHECK(!etl::to_arithmetic<int8_t>(text.c_str(), text.size(), 11).has_value());
      CHECK(!etl::to_arithmetic<int8_t>(text.c_str(), text.size(), 12).has_value());
      CHECK(!etl::to_arithmetic<int8_t>(text.c_str(), text.size(), 13).has_value());
      CHECK(!etl::to_arithmetic<int8_t>(text.c_str(), text.size(), 14).has_value());
      CHECK(!etl::to_arithmetic<int8_t>(text.c_str(), text.size(), 15).has_value());
      CHECK(!etl::to_arithmetic<int8_t>(text.c_str(), text.size(), 17).has_value());

      CHECK_EQUAL(etl::to_arithmetic_status::Invalid_Radix, etl::to_arithmetic<int8_t>(text.c_str(), text.size(), 0).error());
      CHECK_EQUAL(etl::to_arithmetic_status::Invalid_Radix, etl::to_arithmetic<int8_t>(text.c_str(), text.size(), 1).error());
      CHECK_EQUAL(etl::to_arithmetic_status::Invalid_Radix, etl::to_arithmetic<int8_t>(text.c_str(), text.size(), 3).error());
      CHECK_EQUAL(etl::to_arithmetic_status::Invalid_Radix, etl::to_arithmetic<int8_t>(text.c_str(), text.size(), 4).error());
      CHECK_EQUAL(etl::to_arithmetic_status::Invalid_Radix, etl::to_arithmetic<int8_t>(text.c_str(), text.size(), 5).error());
      CHECK_EQUAL(etl::to_arithmetic_status::Invalid_Radix, etl::to_arithmetic<int8_t>(text.c_str(), text.size(), 6).error());
      CHECK_EQUAL(etl::to_arithmetic_status::Invalid_Radix, etl::to_arithmetic<int8_t>(text.c_str(), text.size(), 7).error());
      CHECK_EQUAL(etl::to_arithmetic_status::Invalid_Radix, etl::to_arithmetic<int8_t>(text.c_str(), text.size(), 9).error());
      CHECK_EQUAL(etl::to_arithmetic_status::Invalid_Radix, etl::to_arithmetic<int8_t>(text.c_str(), text.size(), 11).error());
      CHECK_EQUAL(etl::to_arithmetic_status::Invalid_Radix, etl::to_arithmetic<int8_t>(text.c_str(), text.size(), 12).error());
      CHECK_EQUAL(etl::to_arithmetic_status::Invalid_Radix, etl::to_arithmetic<int8_t>(text.c_str(), text.size(), 13).error());
      CHECK_EQUAL(etl::to_arithmetic_status::Invalid_Radix, etl::to_arithmetic<int8_t>(text.c_str(), text.size(), 14).error());
      CHECK_EQUAL(etl::to_arithmetic_status::Invalid_Radix, etl::to_arithmetic<int8_t>(text.c_str(), text.size(), 15).error());
      CHECK_EQUAL(etl::to_arithmetic_status::Invalid_Radix, etl::to_arithmetic<int8_t>(text.c_str(), text.size(), 17).error());
    }

    //*************************************************************************
    TEST(test_invalid_binary_numerics)
    {
      const Text text1(STR(" 101"));
      const Text text2(STR("101 "));
      const Text text3(STR("121"));
      const Text text4(STR(""));
      const Text text5(STR("-101"));

      CHECK(!etl::to_arithmetic<int8_t>(text1.c_str(), text1.size(), etl::bin));
      CHECK(!etl::to_arithmetic<int8_t>(text2.c_str(), text2.size(), etl::bin));
      CHECK(!etl::to_arithmetic<int8_t>(text3.c_str(), text3.size(), etl::bin));
      CHECK(!etl::to_arithmetic<int8_t>(text4.c_str(), text4.size(), etl::bin));
      CHECK(!etl::to_arithmetic<uint8_t>(text5.c_str(), text5.size(), etl::bin));

      CHECK_EQUAL(etl::to_arithmetic_status::Invalid_Format, etl::to_arithmetic<int8_t>(text1.c_str(), text1.size(), etl::bin).error());
      CHECK_EQUAL(etl::to_arithmetic_status::Invalid_Format, etl::to_arithmetic<int8_t>(text2.c_str(), text2.size(), etl::bin).error());
      CHECK_EQUAL(etl::to_arithmetic_status::Invalid_Format, etl::to_arithmetic<int8_t>(text3.c_str(), text3.size(), etl::bin).error());
      CHECK_EQUAL(etl::to_arithmetic_status::Invalid_Format, etl::to_arithmetic<int8_t>(text4.c_str(), text4.size(), etl::bin).error());
      CHECK_EQUAL(etl::to_arithmetic_status::Signed_To_Unsigned, etl::to_arithmetic<uint8_t>(text5.c_str(), text5.size(), etl::bin).error());
    }

    //*************************************************************************
    TEST(test_valid_binary_numerics)
    {
      const Text value1(STR("0"));
      const Text value2(STR("1"));
      const Text value3(STR("0101"));
      const Text value4(STR("1010"));
      const Text value5(STR("01010011"));
      const Text value6(STR("10101100"));
      const Text value7(STR("-01010100"));

      CHECK_EQUAL(int(0), int(etl::to_arithmetic<int8_t>(value1.c_str(), value1.size(), etl::bin).value()));
      CHECK_EQUAL(int(1), int(etl::to_arithmetic<int8_t>(value2.c_str(), value2.size(), etl::bin).value()));
      CHECK_EQUAL(int(5), int(etl::to_arithmetic<int8_t>(value3.c_str(), value3.size(), etl::bin).value()));
      CHECK_EQUAL(int(10), int(etl::to_arithmetic<int8_t>(value4.c_str(), value4.size(), etl::bin).value()));
      CHECK_EQUAL(int(83), int(etl::to_arithmetic<int8_t>(value5.c_str(), value5.size(), etl::bin).value()));
      CHECK_EQUAL(int(-84), int(etl::to_arithmetic<int8_t>(value6.c_str(), value6.size(), etl::bin).value()));
      CHECK_EQUAL(int(-84), int(etl::to_arithmetic<int8_t>(value7.c_str(), value7.size(), etl::bin).value()));

      CHECK(etl::to_arithmetic<int8_t>(value1.c_str(), value1.size(), etl::bin).has_value());
      CHECK(etl::to_arithmetic<int8_t>(value2.c_str(), value2.size(), etl::bin).has_value());
      CHECK(etl::to_arithmetic<int8_t>(value3.c_str(), value3.size(), etl::bin).has_value());
      CHECK(etl::to_arithmetic<int8_t>(value4.c_str(), value4.size(), etl::bin).has_value());
      CHECK(etl::to_arithmetic<int8_t>(value5.c_str(), value5.size(), etl::bin).has_value());
      CHECK(etl::to_arithmetic<int8_t>(value6.c_str(), value6.size(), etl::bin).has_value());
      CHECK(etl::to_arithmetic<int8_t>(value7.c_str(), value7.size(), etl::bin).has_value());
    }

    //*************************************************************************
    TEST(test_maximum_binary_numerics)
    {
      const Text int8_max(STR("01111111"));
      const Text int8_min_neg(STR("-10000000"));
      const Text int8_min_2c(STR("10000000"));

      const Text uint8_max(STR("11111111"));
      const Text uint8_min(STR("00000000"));

      const Text int16_max(STR("0111111111111111"));
      const Text int16_min_neg(STR("-1000000000000000"));
      const Text int16_min_2c(STR("1000000000000000"));

      const Text uint16_max(STR("1111111111111111"));
      const Text uint16_min(STR("0000000000000000"));

      const Text int32_max(STR("01111111111111111111111111111111"));
      const Text int32_min_neg(STR("-10000000000000000000000000000000"));
      const Text int32_min_2c(STR("10000000000000000000000000000000"));

      const Text uint32_max(STR("11111111111111111111111111111111"));
      const Text uint32_min(STR("0"));

      const Text int64_max(STR("0111111111111111111111111111111111111111111111111111111111111111"));
      const Text int64_min_neg(STR("-1000000000000000000000000000000000000000000000000000000000000000"));
      const Text int64_min_2c(STR("1000000000000000000000000000000000000000000000000000000000000000"));

      const Text uint64_max(STR("1111111111111111111111111111111111111111111111111111111111111111"));
      const Text uint64_min(STR("0"));

      CHECK_EQUAL(std::numeric_limits<int8_t>::max(), etl::to_arithmetic<int8_t>(int8_max.c_str(), int8_max.size(), etl::bin).value());
      CHECK_EQUAL(std::numeric_limits<int8_t>::min(), etl::to_arithmetic<int8_t>(int8_min_neg.c_str(), int8_min_neg.size(), etl::bin).value());
      CHECK_EQUAL(std::numeric_limits<int8_t>::min(), etl::to_arithmetic<int8_t>(int8_min_2c.c_str(), int8_min_2c.size(), etl::bin).value());

      CHECK_EQUAL(std::numeric_limits<uint8_t>::max(), etl::to_arithmetic<uint8_t>(uint8_max.c_str(), uint8_max.size(), etl::bin).value());
      CHECK_EQUAL(std::numeric_limits<uint8_t>::min(), etl::to_arithmetic<uint8_t>(uint8_min.c_str(), uint8_min.size(), etl::bin).value());

      CHECK_EQUAL(std::numeric_limits<int16_t>::max(), etl::to_arithmetic<int16_t>(int16_max.c_str(), int16_max.size(), etl::bin).value());
      CHECK_EQUAL(std::numeric_limits<int16_t>::min(), etl::to_arithmetic<int16_t>(int16_min_neg.c_str(), int16_min_neg.size(), etl::bin).value());
      CHECK_EQUAL(std::numeric_limits<int16_t>::min(), etl::to_arithmetic<int16_t>(int16_min_2c.c_str(), int16_min_2c.size(), etl::bin).value());

      CHECK_EQUAL(std::numeric_limits<uint16_t>::max(), etl::to_arithmetic<uint16_t>(uint16_max.c_str(), uint16_max.size(), etl::bin).value());
      CHECK_EQUAL(std::numeric_limits<uint16_t>::min(), etl::to_arithmetic<uint16_t>(uint16_min.c_str(), uint16_min.size(), etl::bin).value());

      CHECK_EQUAL(std::numeric_limits<int32_t>::max(), etl::to_arithmetic<int32_t>(int32_max.c_str(), int32_max.size(), etl::bin).value());
      CHECK_EQUAL(std::numeric_limits<int32_t>::min(), etl::to_arithmetic<int32_t>(int32_min_neg.c_str(), int32_min_neg.size(), etl::bin).value());
      CHECK_EQUAL(std::numeric_limits<int32_t>::min(), etl::to_arithmetic<int32_t>(int32_min_2c.c_str(), int32_min_2c.size(), etl::bin).value());

      CHECK_EQUAL(std::numeric_limits<uint32_t>::max(), etl::to_arithmetic<uint32_t>(uint32_max.c_str(), uint32_max.size(), etl::bin).value());
      CHECK_EQUAL(std::numeric_limits<uint32_t>::min(), etl::to_arithmetic<uint32_t>(uint32_min.c_str(), uint32_min.size(), etl::bin).value());

      CHECK_EQUAL(std::numeric_limits<int64_t>::max(), etl::to_arithmetic<int64_t>(int64_max.c_str(), int64_max.size(), etl::bin).value());
      CHECK_EQUAL(std::numeric_limits<int64_t>::min(), etl::to_arithmetic<int64_t>(int64_min_neg.c_str(), int64_min_neg.size(), etl::bin).value());
      CHECK_EQUAL(std::numeric_limits<int64_t>::min(), etl::to_arithmetic<int64_t>(int64_min_2c.c_str(), int64_min_2c.size(), etl::bin).value());

      CHECK_EQUAL(std::numeric_limits<uint64_t>::max(), etl::to_arithmetic<uint64_t>(uint64_max.c_str(), uint64_max.size(), etl::bin).value());
      CHECK_EQUAL(std::numeric_limits<uint64_t>::min(), etl::to_arithmetic<uint64_t>(uint64_min.c_str(), uint64_min.size(), etl::bin).value());
    }

    //*************************************************************************
    TEST(test_binary_numerics_overflow)
    {
      const Text int8_overflow_max(STR("100000000"));
      const Text int8_overflow_min(STR("-100000000"));

      const Text uint8_overflow_max(STR("100000000"));

      const Text int16_overflow_max(STR("10000000000000000"));
      const Text int16_overflow_min(STR("-10000000000000000"));

      const Text uint16_overflow_max(STR("10000000000000000"));

      const Text int32_overflow_max(STR("100000000000000000000000000000000"));
      const Text int32_overflow_min(STR("-100000000000000000000000000000000"));

      const Text uint32_overflow_max(STR("100000000000000000000000000000000"));

      const Text int64_overflow_max(STR("10000000000000000000000000000000000000000000000000000000000000000"));
      const Text int64_overflow_min(STR("-10000000000000000000000000000000000000000000000000000000000000000"));

      const Text uint64_overflow_max(STR("10000000000000000000000000000000000000000000000000000000000000000"));

      CHECK(!etl::to_arithmetic<int8_t>(int8_overflow_max.c_str(), int8_overflow_max.size(), etl::bin));
      CHECK(!etl::to_arithmetic<int8_t>(int8_overflow_min.c_str(), int8_overflow_min.size(), etl::bin));

      CHECK(!etl::to_arithmetic<uint8_t>(uint8_overflow_max.c_str(), uint8_overflow_max.size(), etl::bin));

      CHECK(!etl::to_arithmetic<int16_t>(int16_overflow_max.c_str(), int16_overflow_max.size(), etl::bin));
      CHECK(!etl::to_arithmetic<int16_t>(int16_overflow_min.c_str(), int16_overflow_min.size(), etl::bin));

      CHECK(!etl::to_arithmetic<uint16_t>(uint16_overflow_max.c_str(), uint16_overflow_max.size(), etl::bin));

      CHECK(!etl::to_arithmetic<int32_t>(int32_overflow_max.c_str(), int32_overflow_max.size(), etl::bin));
      CHECK(!etl::to_arithmetic<int32_t>(int32_overflow_min.c_str(), int32_overflow_min.size(), etl::bin));

      CHECK(!etl::to_arithmetic<uint32_t>(uint32_overflow_max.c_str(), uint32_overflow_max.size(), etl::bin));

      CHECK(!etl::to_arithmetic<int64_t>(int64_overflow_max.c_str(), int64_overflow_max.size(), etl::bin));
      CHECK(!etl::to_arithmetic<int64_t>(int64_overflow_min.c_str(), int64_overflow_min.size(), etl::bin));

      CHECK(!etl::to_arithmetic<uint64_t>(uint64_overflow_max.c_str(), uint64_overflow_max.size(), etl::bin));


      CHECK_EQUAL(etl::to_arithmetic_status::Overflow, etl::to_arithmetic<int8_t>(int8_overflow_max.c_str(), int8_overflow_max.size(), etl::bin).error());
      CHECK_EQUAL(etl::to_arithmetic_status::Overflow, etl::to_arithmetic<int8_t>(int8_overflow_min.c_str(), int8_overflow_min.size(), etl::bin).error());

      CHECK_EQUAL(etl::to_arithmetic_status::Overflow, etl::to_arithmetic<uint8_t>(uint8_overflow_max.c_str(), uint8_overflow_max.size(), etl::bin).error());

      CHECK_EQUAL(etl::to_arithmetic_status::Overflow, etl::to_arithmetic<int16_t>(int16_overflow_max.c_str(), int16_overflow_max.size(), etl::bin).error());
      CHECK_EQUAL(etl::to_arithmetic_status::Overflow, etl::to_arithmetic<int16_t>(int16_overflow_min.c_str(), int16_overflow_min.size(), etl::bin).error());

      CHECK_EQUAL(etl::to_arithmetic_status::Overflow, etl::to_arithmetic<uint16_t>(uint16_overflow_max.c_str(), uint16_overflow_max.size(), etl::bin).error());

      CHECK_EQUAL(etl::to_arithmetic_status::Overflow, etl::to_arithmetic<int32_t>(int32_overflow_max.c_str(), int32_overflow_max.size(), etl::bin).error());
      CHECK_EQUAL(etl::to_arithmetic_status::Overflow, etl::to_arithmetic<int32_t>(int32_overflow_min.c_str(), int32_overflow_min.size(), etl::bin).error());

      CHECK_EQUAL(etl::to_arithmetic_status::Overflow, etl::to_arithmetic<uint32_t>(uint32_overflow_max.c_str(), uint32_overflow_max.size(), etl::bin).error());

      CHECK_EQUAL(etl::to_arithmetic_status::Overflow, etl::to_arithmetic<int64_t>(int64_overflow_max.c_str(), int64_overflow_max.size(), etl::bin).error());
      CHECK_EQUAL(etl::to_arithmetic_status::Overflow, etl::to_arithmetic<int64_t>(int64_overflow_min.c_str(), int64_overflow_min.size(), etl::bin).error());

      CHECK_EQUAL(etl::to_arithmetic_status::Overflow, etl::to_arithmetic<uint64_t>(uint64_overflow_max.c_str(), uint64_overflow_max.size(), etl::bin).error());
    }

    //*************************************************************************
    TEST(test_invalid_octal_numerics)
    {
      const Text text1(STR(" 127"));
      const Text text2(STR("127 "));
      const Text text3(STR("187"));
      const Text text4(STR("-187"));

      CHECK(!etl::to_arithmetic<int8_t>(text1.c_str(), text1.size(), etl::oct));
      CHECK(!etl::to_arithmetic<int8_t>(text2.c_str(), text2.size(), etl::oct));
      CHECK(!etl::to_arithmetic<int8_t>(text3.c_str(), text3.size(), etl::oct));
      CHECK(!etl::to_arithmetic<uint8_t>(text4.c_str(), text4.size(), etl::oct));
    }

    //*************************************************************************
    TEST(test_valid_octal_numerics)
    {
      const Text value1(STR("0"));
      const Text value2(STR("01"));
      const Text value3(STR("05"));
      const Text value4(STR("012"));
      const Text value5(STR("0123"));
      const Text value6(STR("0254"));
      const Text value7(STR("-0124"));

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
      const Text int8_max(STR("177"));
      const Text int8_min_neg(STR("-200"));
      const Text int8_min_2c(STR("200"));

      const Text uint8_max(STR("377"));
      const Text uint8_min(STR("0"));

      const Text int16_max(STR("77777"));
      const Text int16_min_neg(STR("-100000"));
      const Text int16_min_2c(STR("100000"));

      const Text uint16_max(STR("177777"));
      const Text uint16_min(STR("0"));

      const Text int32_max(STR("17777777777"));
      const Text int32_min_neg(STR("-20000000000"));
      const Text int32_min_2c(STR("20000000000"));

      const Text uint32_max(STR("37777777777"));
      const Text uint32_min(STR("0"));

      const Text int64_max(STR("777777777777777777777"));
      const Text int64_min_neg(STR("-1000000000000000000000"));
      const Text int64_min_2c(STR("1000000000000000000000"));

      const Text uint64_max(STR("1777777777777777777777"));
      const Text uint64_min(STR("0"));

      CHECK_EQUAL(std::numeric_limits<int8_t>::max(), etl::to_arithmetic<int8_t>(int8_max.c_str(), int8_max.size(), etl::oct).value());
      CHECK_EQUAL(std::numeric_limits<int8_t>::min(), etl::to_arithmetic<int8_t>(int8_min_neg.c_str(), int8_min_neg.size(), etl::oct).value());
      CHECK_EQUAL(std::numeric_limits<int8_t>::min(), etl::to_arithmetic<int8_t>(int8_min_2c.c_str(), int8_min_2c.size(), etl::oct).value());

      CHECK_EQUAL(std::numeric_limits<uint8_t>::max(), etl::to_arithmetic<uint8_t>(uint8_max.c_str(), uint8_max.size(), etl::oct).value());
      CHECK_EQUAL(std::numeric_limits<uint8_t>::min(), etl::to_arithmetic<uint8_t>(uint8_min.c_str(), uint8_min.size(), etl::oct).value());

      CHECK_EQUAL(std::numeric_limits<int16_t>::max(), etl::to_arithmetic<int16_t>(int16_max.c_str(), int16_max.size(), etl::oct).value());
      CHECK_EQUAL(std::numeric_limits<int16_t>::min(), etl::to_arithmetic<int16_t>(int16_min_neg.c_str(), int16_min_neg.size(), etl::oct).value());
      CHECK_EQUAL(std::numeric_limits<int16_t>::min(), etl::to_arithmetic<int16_t>(int16_min_2c.c_str(), int16_min_2c.size(), etl::oct).value());

      CHECK_EQUAL(std::numeric_limits<uint16_t>::max(), etl::to_arithmetic<uint16_t>(uint16_max.c_str(), uint16_max.size(), etl::oct).value());
      CHECK_EQUAL(std::numeric_limits<uint16_t>::min(), etl::to_arithmetic<uint16_t>(uint16_min.c_str(), uint16_min.size(), etl::oct).value());

      CHECK_EQUAL(std::numeric_limits<int32_t>::max(), etl::to_arithmetic<int32_t>(int32_max.c_str(), int32_max.size(), etl::oct).value());
      CHECK_EQUAL(std::numeric_limits<int32_t>::min(), etl::to_arithmetic<int32_t>(int32_min_neg.c_str(), int32_min_neg.size(), etl::oct).value());
      CHECK_EQUAL(std::numeric_limits<int32_t>::min(), etl::to_arithmetic<int32_t>(int32_min_2c.c_str(), int32_min_2c.size(), etl::oct).value());

      CHECK_EQUAL(std::numeric_limits<uint32_t>::max(), etl::to_arithmetic<uint32_t>(uint32_max.c_str(), uint32_max.size(), etl::oct).value());
      CHECK_EQUAL(std::numeric_limits<uint32_t>::min(), etl::to_arithmetic<uint32_t>(uint32_min.c_str(), uint32_min.size(), etl::oct).value());

      CHECK_EQUAL(std::numeric_limits<int64_t>::max(), etl::to_arithmetic<int64_t>(int64_max.c_str(), int64_max.size(), etl::oct).value());
      CHECK_EQUAL(std::numeric_limits<int64_t>::min(), etl::to_arithmetic<int64_t>(int64_min_neg.c_str(), int64_min_neg.size(), etl::oct).value());
      CHECK_EQUAL(std::numeric_limits<int64_t>::min(), etl::to_arithmetic<int64_t>(int64_min_2c.c_str(), int64_min_2c.size(), etl::oct).value());

      CHECK_EQUAL(std::numeric_limits<uint64_t>::max(), etl::to_arithmetic<uint64_t>(uint64_max.c_str(), uint64_max.size(), etl::oct).value());
      CHECK_EQUAL(std::numeric_limits<uint64_t>::min(), etl::to_arithmetic<uint64_t>(uint64_min.c_str(), uint64_min.size(), etl::oct).value());
    }

    //*************************************************************************
    TEST(test_octal_numerics_overflow)
    {
      const Text int8_overflow_max(STR("400"));
      const Text int8_overflow_min(STR("-400"));

      const Text uint8_overflow_max(STR("400"));

      const Text int16_overflow_max(STR("200000"));
      const Text int16_overflow_min(STR("-200000"));

      const Text uint16_overflow_max(STR("200000"));

      const Text int32_overflow_max(STR("40000000000"));
      const Text int32_overflow_min(STR("-40000000000"));

      const Text uint32_overflow_max(STR("40000000000"));

      const Text int64_overflow_max(STR("2000000000000000000000"));
      const Text int64_overflow_min(STR("-2000000000000000000000"));

      const Text uint64_overflow_max(STR("2000000000000000000000"));

      CHECK(!etl::to_arithmetic<int8_t>(int8_overflow_max.c_str(), int8_overflow_max.size(), etl::oct));
      CHECK(!etl::to_arithmetic<int8_t>(int8_overflow_min.c_str(), int8_overflow_min.size(), etl::oct));

      CHECK(!etl::to_arithmetic<uint8_t>(uint8_overflow_max.c_str(), uint8_overflow_max.size(), etl::oct));

      CHECK(!etl::to_arithmetic<int16_t>(int16_overflow_max.c_str(), int16_overflow_max.size(), etl::oct));
      CHECK(!etl::to_arithmetic<int16_t>(int16_overflow_min.c_str(), int16_overflow_min.size(), etl::oct));

      CHECK(!etl::to_arithmetic<uint16_t>(uint16_overflow_max.c_str(), uint16_overflow_max.size(), etl::oct));

      CHECK(!etl::to_arithmetic<int32_t>(int32_overflow_max.c_str(), int32_overflow_max.size(), etl::oct));
      CHECK(!etl::to_arithmetic<int32_t>(int32_overflow_min.c_str(), int32_overflow_min.size(), etl::oct));

      CHECK(!etl::to_arithmetic<uint32_t>(uint32_overflow_max.c_str(), uint32_overflow_max.size(), etl::oct));

      CHECK(!etl::to_arithmetic<int64_t>(int64_overflow_max.c_str(), int64_overflow_max.size(), etl::oct));
      CHECK(!etl::to_arithmetic<int64_t>(int64_overflow_min.c_str(), int64_overflow_min.size(), etl::oct));

      CHECK(!etl::to_arithmetic<uint64_t>(uint64_overflow_max.c_str(), uint64_overflow_max.size(), etl::oct));
    }

    //*************************************************************************
    TEST(test_invalid_decimal_numerics)
    {
      const Text text1(STR(" 128"));
      const Text text2(STR("128 "));
      const Text text3(STR("1A8"));
      const Text text4(STR("++128"));
      const Text text5(STR("-+128"));
      const Text text6(STR("+-128"));
      const Text text7(STR("--128"));
      const Text text8(STR("+"));
      const Text text9(STR("-"));
      const Text text10(STR(""));

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
      const Text text1(STR("0"));
      const Text text2(STR("1"));
      const Text text3(STR("5"));
      const Text text4(STR("+10"));
      const Text text5(STR("83"));
      const Text text6(STR("-84"));

      CHECK_EQUAL(int(0), int(etl::to_arithmetic<int8_t>(text1.c_str(), text1.size(), etl::dec).value()));
      CHECK_EQUAL(int(1), int(etl::to_arithmetic<int8_t>(text2.c_str(), text2.size(), etl::dec).value()));
      CHECK_EQUAL(int(5), int(etl::to_arithmetic<int8_t>(text3.c_str(), text3.size(), etl::dec).value()));
      CHECK_EQUAL(int(10), int(etl::to_arithmetic<int8_t>(text4.c_str(), text4.size(), etl::dec).value()));
      CHECK_EQUAL(int(83), int(etl::to_arithmetic<int8_t>(text5.c_str(), text5.size(), etl::dec).value()));
      CHECK_EQUAL(int(83), int(etl::to_arithmetic<uint8_t>(text5.c_str(), text5.size(), etl::dec).value()));
      CHECK_EQUAL(int(-84), int(etl::to_arithmetic<int8_t>(text6.c_str(), text6.size(), etl::dec).value()));
    }

    //*************************************************************************
    TEST(test_maximum_decimal_numerics)
    {
      const Text int8_max(STR("127"));
      const Text int8_min(STR("-128"));

      const Text uint8_max(STR("255"));
      const Text uint8_min(STR("0"));

      const Text int16_max(STR("32767"));
      const Text int16_min(STR("-32768"));

      const Text uint16_max(STR("65535"));
      const Text uint16_min(STR("0"));

      const Text int32_max(STR("2147483647"));
      const Text int32_min(STR("-2147483648"));

      const Text uint32_max(STR("4294967295"));
      const Text uint32_min(STR("0"));

      const Text int64_max(STR("9223372036854775807"));
      const Text int64_min(STR("-9223372036854775808"));

      const Text uint64_max(STR("18446744073709551615"));
      const Text uint64_min(STR("0"));

      CHECK_EQUAL(std::numeric_limits<int8_t>::max(), etl::to_arithmetic<int8_t>(int8_max.c_str(), int8_max.size(), etl::dec).value());
      CHECK_EQUAL(std::numeric_limits<int8_t>::min(), etl::to_arithmetic<int8_t>(int8_min.c_str(), int8_min.size(), etl::dec).value());

      CHECK_EQUAL(std::numeric_limits<uint8_t>::max(), etl::to_arithmetic<uint8_t>(uint8_max.c_str(), uint8_max.size(), etl::dec).value());
      CHECK_EQUAL(std::numeric_limits<uint8_t>::min(), etl::to_arithmetic<uint8_t>(uint8_min.c_str(), uint8_min.size(), etl::dec).value());

      CHECK_EQUAL(std::numeric_limits<int16_t>::max(), etl::to_arithmetic<int16_t>(int16_max.c_str(), int16_max.size(), etl::dec).value());
      CHECK_EQUAL(std::numeric_limits<int16_t>::min(), etl::to_arithmetic<int16_t>(int16_min.c_str(), int16_min.size(), etl::dec).value());

      CHECK_EQUAL(std::numeric_limits<uint16_t>::max(), etl::to_arithmetic<uint16_t>(uint16_max.c_str(), uint16_max.size(), etl::dec).value());
      CHECK_EQUAL(std::numeric_limits<uint16_t>::min(), etl::to_arithmetic<uint16_t>(uint16_min.c_str(), uint16_min.size(), etl::dec).value());

      CHECK_EQUAL(std::numeric_limits<int32_t>::max(), etl::to_arithmetic<int32_t>(int32_max.c_str(), int32_max.size(), etl::dec).value());
      CHECK_EQUAL(std::numeric_limits<int32_t>::min(), etl::to_arithmetic<int32_t>(int32_min.c_str(), int32_min.size(), etl::dec).value());

      CHECK_EQUAL(std::numeric_limits<uint32_t>::max(), etl::to_arithmetic<uint32_t>(uint32_max.c_str(), uint32_max.size(), etl::dec).value());
      CHECK_EQUAL(std::numeric_limits<uint32_t>::min(), etl::to_arithmetic<uint32_t>(uint32_min.c_str(), uint32_min.size(), etl::dec).value());

      CHECK_EQUAL(std::numeric_limits<int64_t>::max(), etl::to_arithmetic<int64_t>(int64_max.c_str(), int64_max.size(), etl::dec).value());
      CHECK_EQUAL(std::numeric_limits<int64_t>::min(), etl::to_arithmetic<int64_t>(int64_min.c_str(), int64_min.size(), etl::dec).value());

      CHECK_EQUAL(std::numeric_limits<uint64_t>::max(), etl::to_arithmetic<uint64_t>(uint64_max.c_str(), uint64_max.size(), etl::dec).value());
      CHECK_EQUAL(std::numeric_limits<uint64_t>::min(), etl::to_arithmetic<uint64_t>(uint64_min.c_str(), uint64_min.size(), etl::dec).value());
    }

    //*************************************************************************
    TEST(test_decimal_numerics_overflow)
    {
      const Text int8_overflow_max(STR("128"));
      const Text int8_overflow_min(STR("-129"));

      const Text int8_overflow_max_plus(STR("1000"));
      const Text int8_overflow_min_plus(STR("-1000"));

      const Text uint8_overflow_max(STR("256"));

      const Text int16_overflow_max(STR("32768"));
      const Text int16_overflow_min(STR("-32769"));

      const Text uint16_overflow_max(STR("65536"));

      const Text int32_overflow_max(STR("2147483648"));
      const Text int32_overflow_min(STR("-2147483649"));

      const Text uint32_overflow_max(STR("4294967296"));

      const Text int64_overflow_max(STR("9223372036854775808"));
      const Text int64_overflow_min(STR("-9223372036854775809"));

      const Text uint64_overflow_max(STR("18446744073709551616"));

      CHECK(!etl::to_arithmetic<int8_t>(int8_overflow_max.c_str(), int8_overflow_max.size(), etl::dec));
      CHECK(!etl::to_arithmetic<int8_t>(int8_overflow_min.c_str(), int8_overflow_min.size(), etl::dec));

      CHECK(!etl::to_arithmetic<int8_t>(int8_overflow_max_plus.c_str(), int8_overflow_max_plus.size(), etl::dec));
      CHECK(!etl::to_arithmetic<int8_t>(int8_overflow_min_plus.c_str(), int8_overflow_min_plus.size(), etl::dec));

      CHECK(!etl::to_arithmetic<uint8_t>(uint8_overflow_max.c_str(), uint8_overflow_max.size(), etl::dec));

      CHECK(!etl::to_arithmetic<int16_t>(int16_overflow_max.c_str(), int16_overflow_max.size(), etl::dec));
      CHECK(!etl::to_arithmetic<int16_t>(int16_overflow_min.c_str(), int16_overflow_min.size(), etl::dec));

      CHECK(!etl::to_arithmetic<uint16_t>(uint16_overflow_max.c_str(), uint16_overflow_max.size(), etl::dec));

      CHECK(!etl::to_arithmetic<int32_t>(int32_overflow_max.c_str(), int32_overflow_max.size(), etl::dec));
      CHECK(!etl::to_arithmetic<int32_t>(int32_overflow_min.c_str(), int32_overflow_min.size(), etl::dec));

      CHECK(!etl::to_arithmetic<uint32_t>(uint32_overflow_max.c_str(), uint32_overflow_max.size(), etl::dec));

      CHECK(!etl::to_arithmetic<int64_t>(int64_overflow_max.c_str(), int64_overflow_max.size(), etl::dec));
      CHECK(!etl::to_arithmetic<int64_t>(int64_overflow_min.c_str(), int64_overflow_min.size(), etl::dec));

      CHECK(!etl::to_arithmetic<uint64_t>(uint64_overflow_max.c_str(), uint64_overflow_max.size(), etl::dec));
    }

    //*************************************************************************
    TEST(test_invalid_hex_numerics)
    {
      const Text text1(STR(" 1Af"));
      const Text text2(STR("1Af "));
      const Text text3(STR("1Gf"));
      const Text text4(STR("-1Af"));

      CHECK(!etl::to_arithmetic<int8_t>(text1.c_str(), text1.size(), etl::hex));
      CHECK(!etl::to_arithmetic<int8_t>(text2.c_str(), text2.size(), etl::hex));
      CHECK(!etl::to_arithmetic<int8_t>(text3.c_str(), text3.size(), etl::hex));
      CHECK(!etl::to_arithmetic<uint8_t>(text4.c_str(), text4.size(), etl::hex));
    }

    //*************************************************************************
    TEST(test_valid_hex_numerics)
    {
      const Text text1(STR("0"));
      const Text text2(STR("1"));
      const Text text3(STR("5"));
      const Text text4(STR("a"));
      const Text text5(STR("53"));
      const Text text6(STR("Ac"));
      const Text text7(STR("-54"));

      CHECK_EQUAL(int(0), int(etl::to_arithmetic<int8_t>(text1.c_str(), text1.size(), etl::hex).value()));
      CHECK_EQUAL(int(1), int(etl::to_arithmetic<int8_t>(text2.c_str(), text2.size(), etl::hex).value()));
      CHECK_EQUAL(int(5), int(etl::to_arithmetic<int8_t>(text3.c_str(), text3.size(), etl::hex).value()));
      CHECK_EQUAL(int(10), int(etl::to_arithmetic<int8_t>(text4.c_str(), text4.size(), etl::hex).value()));
      CHECK_EQUAL(int(83), int(etl::to_arithmetic<int8_t>(text5.c_str(), text5.size(), etl::hex).value()));
      CHECK_EQUAL(int(-84), int(etl::to_arithmetic<int8_t>(text6.c_str(), text6.size(), etl::hex).value()));
      CHECK_EQUAL(int(-84), int(etl::to_arithmetic<int8_t>(text7.c_str(), text7.size(), etl::hex).value()));
    }

    //*************************************************************************
    TEST(test_maximum_hex_numerics)
    {
      const Text int8_max(STR("7F"));
      const Text int8_min_neg(STR("-80"));
      const Text int8_min_2c(STR("80"));

      const Text uint8_max(STR("FF"));
      const Text uint8_min(STR("0"));

      const Text int16_max(STR("7FFF"));
      const Text int16_min_neg(STR("-8000"));
      const Text int16_min_2c(STR("8000"));

      const Text uint16_max(STR("FFFF"));
      const Text uint16_min(STR("0"));

      const Text int32_max(STR("7FFFFFFF"));
      const Text int32_min_neg(STR("-80000000"));
      const Text int32_min_2c(STR("80000000"));

      const Text uint32_max(STR("FFFFFFFF"));
      const Text uint32_min(STR("0"));

      const Text int64_max(STR("7FFFFFFFFFFFFFFF"));
      const Text int64_min_neg(STR("-8000000000000000"));
      const Text int64_min_2c(STR("8000000000000000"));

      const Text uint64_max(STR("FFFFFFFFFFFFFFFF"));
      const Text uint64_min(STR("0"));

      CHECK_EQUAL(std::numeric_limits<int8_t>::max(), etl::to_arithmetic<int8_t>(int8_max.c_str(), int8_max.size(), etl::hex).value());
      CHECK_EQUAL(std::numeric_limits<int8_t>::min(), etl::to_arithmetic<int8_t>(int8_min_neg.c_str(), int8_min_neg.size(), etl::hex).value());
      CHECK_EQUAL(std::numeric_limits<int8_t>::min(), etl::to_arithmetic<int8_t>(int8_min_2c.c_str(), int8_min_2c.size(), etl::hex).value());

      CHECK_EQUAL(std::numeric_limits<uint8_t>::max(), etl::to_arithmetic<uint8_t>(uint8_max.c_str(), uint8_max.size(), etl::hex).value());
      CHECK_EQUAL(std::numeric_limits<uint8_t>::min(), etl::to_arithmetic<uint8_t>(uint8_min.c_str(), uint8_min.size(), etl::hex).value());

      CHECK_EQUAL(std::numeric_limits<int16_t>::max(), etl::to_arithmetic<int16_t>(int16_max.c_str(), int16_max.size(), etl::hex).value());
      CHECK_EQUAL(std::numeric_limits<int16_t>::min(), etl::to_arithmetic<int16_t>(int16_min_neg.c_str(), int16_min_neg.size(), etl::hex).value());
      CHECK_EQUAL(std::numeric_limits<int16_t>::min(), etl::to_arithmetic<int16_t>(int16_min_2c.c_str(), int16_min_2c.size(), etl::hex).value());

      CHECK_EQUAL(std::numeric_limits<uint16_t>::max(), etl::to_arithmetic<uint16_t>(uint16_max.c_str(), uint16_max.size(), etl::hex).value());
      CHECK_EQUAL(std::numeric_limits<uint16_t>::min(), etl::to_arithmetic<uint16_t>(uint16_min.c_str(), uint16_min.size(), etl::hex).value());

      CHECK_EQUAL(std::numeric_limits<int32_t>::max(), etl::to_arithmetic<int32_t>(int32_max.c_str(), int32_max.size(), etl::hex).value());
      CHECK_EQUAL(std::numeric_limits<int32_t>::min(), etl::to_arithmetic<int32_t>(int32_min_neg.c_str(), int32_min_neg.size(), etl::hex).value());
      CHECK_EQUAL(std::numeric_limits<int32_t>::min(), etl::to_arithmetic<int32_t>(int32_min_2c.c_str(), int32_min_2c.size(), etl::hex).value());

      CHECK_EQUAL(std::numeric_limits<uint32_t>::max(), etl::to_arithmetic<uint32_t>(uint32_max.c_str(), uint32_max.size(), etl::hex).value());
      CHECK_EQUAL(std::numeric_limits<uint32_t>::min(), etl::to_arithmetic<uint32_t>(uint32_min.c_str(), uint32_min.size(), etl::hex).value());

      CHECK_EQUAL(std::numeric_limits<int64_t>::max(), etl::to_arithmetic<int64_t>(int64_max.c_str(), int64_max.size(), etl::hex).value());
      CHECK_EQUAL(std::numeric_limits<int64_t>::min(), etl::to_arithmetic<int64_t>(int64_min_neg.c_str(), int64_min_neg.size(), etl::hex).value());
      CHECK_EQUAL(std::numeric_limits<int64_t>::min(), etl::to_arithmetic<int64_t>(int64_min_2c.c_str(), int64_min_2c.size(), etl::hex).value());

      CHECK_EQUAL(std::numeric_limits<uint64_t>::max(), etl::to_arithmetic<uint64_t>(uint64_max.c_str(), uint64_max.size(), etl::hex).value());
      CHECK_EQUAL(std::numeric_limits<uint64_t>::min(), etl::to_arithmetic<uint64_t>(uint64_min.c_str(), uint64_min.size(), etl::hex).value());
    }

    //*************************************************************************
    TEST(test_hex_numerics_overflow)
    {
      const Text int8_overflow_max(STR("100"));
      const Text int8_overflow_min(STR("-100"));

      const Text uint8_overflow_max(STR("100"));

      const Text int16_overflow_max(STR("10000"));
      const Text int16_overflow_min(STR("-10000"));

      const Text uint16_overflow_max(STR("10000"));

      const Text int32_overflow_max(STR("100000000"));
      const Text int32_overflow_min(STR("-100000000"));

      const Text uint32_overflow_max(STR("100000000"));

      const Text int64_overflow_max(STR("10000000000000000"));
      const Text int64_overflow_min(STR("-10000000000000000"));

      const Text uint64_overflow_max(STR("10000000000000000"));

      CHECK(!etl::to_arithmetic<int8_t>(int8_overflow_max.c_str(), int8_overflow_max.size(), etl::hex));
      CHECK(!etl::to_arithmetic<int8_t>(int8_overflow_min.c_str(), int8_overflow_min.size(), etl::hex));

      CHECK(!etl::to_arithmetic<uint8_t>(uint8_overflow_max.c_str(), uint8_overflow_max.size(), etl::hex));

      CHECK(!etl::to_arithmetic<int16_t>(int16_overflow_max.c_str(), int16_overflow_max.size(), etl::hex));
      CHECK(!etl::to_arithmetic<int16_t>(int16_overflow_min.c_str(), int16_overflow_min.size(), etl::hex));

      CHECK(!etl::to_arithmetic<uint16_t>(uint16_overflow_max.c_str(), uint16_overflow_max.size(), etl::hex));

      CHECK(!etl::to_arithmetic<int32_t>(int32_overflow_max.c_str(), int32_overflow_max.size(), etl::hex));
      CHECK(!etl::to_arithmetic<int32_t>(int32_overflow_min.c_str(), int32_overflow_min.size(), etl::hex));

      CHECK(!etl::to_arithmetic<uint32_t>(uint32_overflow_max.c_str(), uint32_overflow_max.size(), etl::hex));

      CHECK(!etl::to_arithmetic<int64_t>(int64_overflow_max.c_str(), int64_overflow_max.size(), etl::hex));
      CHECK(!etl::to_arithmetic<int64_t>(int64_overflow_min.c_str(), int64_overflow_min.size(), etl::hex));

      CHECK(!etl::to_arithmetic<uint64_t>(uint64_overflow_max.c_str(), uint64_overflow_max.size(), etl::hex));
    }

    //*************************************************************************
    TEST(test_valid_float)
    {
      Text text;

      //*********************************
      text = STR("-123.456789");

      float f1 = strtof("-123.456789", nullptr);
      float f2 = etl::to_arithmetic<float>(text.c_str(), text.size()).value();

      CHECK_CLOSE(f1, f2, 0.00001);
      CHECK(etl::to_arithmetic<float>(text.c_str(), text.size()).has_value());

      //*********************************
      text = STR("123.456789");

      f1 = strtof("123.456789", nullptr);
      f2 = etl::to_arithmetic<float>(text.c_str(), text.size()).value();

      CHECK_CLOSE(f1, f2, 0.00001);
      CHECK(etl::to_arithmetic<float>(text.c_str(), text.size()).has_value());

      //*********************************
      text = STR("-1.23456789e2");

      f1 = strtof("-1.23456789e2", nullptr);
      f2 = etl::to_arithmetic<float>(text.c_str(), text.size()).value();

      CHECK_CLOSE(f1, f2, 0.00001);
      CHECK(etl::to_arithmetic<float>(text.c_str(), text.size()).has_value());

      //*********************************
      text = STR("-12345.6789e-2");

      f1 = strtof("-12345.6789e-2", nullptr);
      f2 = etl::to_arithmetic<float>(text.c_str(), text.size()).value();

      CHECK_CLOSE(f1, f2, 0.00001);
      CHECK(etl::to_arithmetic<float>(text.c_str(), text.size()).has_value());

      //*********************************
      text = STR("+12345E-2");

      f1 = strtof("+12345E-2", nullptr);
      f2 = etl::to_arithmetic<float>(text.c_str(), text.size()).value();

      CHECK_CLOSE(f1, f2, 0.00001);
      CHECK(etl::to_arithmetic<float>(text.c_str(), text.size()).has_value());

      //*********************************
      text = STR("+12345.6789E000");

      f1 = strtof("+12345.6789E000", nullptr);
      f2 = etl::to_arithmetic<float>(text.c_str(), text.size()).value();

      CHECK_CLOSE(f1, f2, 0.00001);
      CHECK(etl::to_arithmetic<float>(text.c_str(), text.size()).has_value());
    }

    //*************************************************************************
    TEST(test_invalid_float)
    {
      Text text;

      //*********************************
      text = STR(" -123.456789");
      CHECK_EQUAL(etl::to_arithmetic_status::Invalid_Format, etl::to_arithmetic<float>(text.c_str(), text.size()).error());

      //*********************************
      text = STR("-123.456789 ");
      CHECK_EQUAL(etl::to_arithmetic_status::Invalid_Format, etl::to_arithmetic<float>(text.c_str(), text.size()).error());

      //*********************************
      text = STR("-12A.456789");
      CHECK_EQUAL(etl::to_arithmetic_status::Invalid_Format, etl::to_arithmetic<float>(text.c_str(), text.size()).error());

      //*********************************
      text = STR("-123.456A89");
      CHECK_EQUAL(etl::to_arithmetic_status::Invalid_Format, etl::to_arithmetic<float>(text.c_str(), text.size()).error());

      //*********************************
      text = STR("123.-456789");
      CHECK_EQUAL(etl::to_arithmetic_status::Invalid_Format, etl::to_arithmetic<float>(text.c_str(), text.size()).error());

      //*********************************
      text = STR("123E.45");
      CHECK_EQUAL(etl::to_arithmetic_status::Invalid_Format, etl::to_arithmetic<float>(text.c_str(), text.size()).error());

      //*********************************
      text = STR("123.45E10000");
      CHECK_EQUAL(etl::to_arithmetic_status::Overflow, etl::to_arithmetic<float>(text.c_str(), text.size()).error());

      //*********************************
      text = STR("-123.45E10000");
      CHECK_EQUAL(etl::to_arithmetic_status::Overflow, etl::to_arithmetic<float>(text.c_str(), text.size()).error());
    }

    //*************************************************************************
    TEST(test_valid_double)
    {
      Text text;

      //*********************************
      text = STR("-123.45678901234567");

      double f1 = strtod("-123.45678901234567", nullptr);
      double f2 = etl::to_arithmetic<double>(text.c_str(), text.size()).value();

      CHECK_CLOSE(f1, f2, 0.00001);
      CHECK(etl::to_arithmetic<double>(text.c_str(), text.size()).has_value());

      //*********************************
      text = STR("123.45678901234567");

      f1 = strtod("123.45678901234567", nullptr);
      f2 = etl::to_arithmetic<double>(text.c_str(), text.size()).value();

      CHECK_CLOSE(f1, f2, 0.00001);
      CHECK(etl::to_arithmetic<double>(text.c_str(), text.size()).has_value());

      //*********************************
      text = STR("-1.2345678901234567e7");

      f1 = strtod("-1.2345678901234567e7", nullptr);
      f2 = etl::to_arithmetic<double>(text.c_str(), text.size()).value();

      CHECK_CLOSE(f1, f2, 0.00001);
      CHECK(etl::to_arithmetic<double>(text.c_str(), text.size()).has_value());

      //*********************************
      text = STR("-12345.678901234567e-2");

      f1 = strtod("-12345.678901234567e-2", nullptr);
      f2 = etl::to_arithmetic<double>(text.c_str(), text.size()).value();

      CHECK_CLOSE(f1, f2, 0.00001);
      CHECK(etl::to_arithmetic<double>(text.c_str(), text.size()).has_value());

      //*********************************
      text = STR("+12345E-2");

      f1 = strtod("+12345E-2", nullptr);
      f2 = etl::to_arithmetic<double>(text.c_str(), text.size()).value();

      CHECK_CLOSE(f1, f2, 0.00001);
      CHECK(etl::to_arithmetic<double>(text.c_str(), text.size()).has_value());

      //*********************************
      text = STR("+12345.678901234567E0");

      f1 = strtod("+12345.678901234567E0", nullptr);
      f2 = etl::to_arithmetic<double>(text.c_str(), text.size()).value();

      CHECK_CLOSE(f1, f2, 0.00001);
      CHECK(etl::to_arithmetic<double>(text.c_str(), text.size()).has_value());
    }

    //*************************************************************************
    TEST(test_invalid_double)
    {
      Text text;

      //*********************************
      text = STR(" -123.456789");
      CHECK_EQUAL(etl::to_arithmetic_status::Invalid_Format, etl::to_arithmetic<double>(text.c_str(), text.size()).error());

      //*********************************
      text = STR("-123.456789 ");
      CHECK_EQUAL(etl::to_arithmetic_status::Invalid_Format, etl::to_arithmetic<double>(text.c_str(), text.size()).error());

      //*********************************
      text = STR("-12A.456789");
      CHECK_EQUAL(etl::to_arithmetic_status::Invalid_Format, etl::to_arithmetic<double>(text.c_str(), text.size()).error());

      //*********************************
      text = STR("-123.456A89");
      CHECK_EQUAL(etl::to_arithmetic_status::Invalid_Format, etl::to_arithmetic<double>(text.c_str(), text.size()).error());

      //*********************************
      text = STR("123.-456789");
      CHECK_EQUAL(etl::to_arithmetic_status::Invalid_Format, etl::to_arithmetic<double>(text.c_str(), text.size()).error());

      //*********************************
      text = STR("123E.45");
      CHECK_EQUAL(etl::to_arithmetic_status::Invalid_Format, etl::to_arithmetic<double>(text.c_str(), text.size()).error());

      //*********************************
      text = STR("123.45E10000");
      CHECK_EQUAL(etl::to_arithmetic_status::Overflow, etl::to_arithmetic<double>(text.c_str(), text.size()).error());

      //*********************************
      text = STR("-123.45E10000");
      CHECK_EQUAL(etl::to_arithmetic_status::Overflow, etl::to_arithmetic<double>(text.c_str(), text.size()).error());
    }

    //*************************************************************************
    TEST(test_valid_long_double)
    {
      Text text;

      //*********************************
      text = STR("-123.45678901234567");

      long double f1 = strtold("-123.45678901234567", nullptr);
      long double f2 = etl::to_arithmetic<long double>(text.c_str(), text.size()).value();

      CHECK_CLOSE(f1, f2, 0.00001);
      CHECK(etl::to_arithmetic<double>(text.c_str(), text.size()).has_value());

      //*********************************
      text = STR("123.45678901234567");

      f1 = strtold("123.45678901234567", nullptr);
      f2 = etl::to_arithmetic<long double>(text.c_str(), text.size()).value();

      CHECK_CLOSE(f1, f2, 0.00001);
      CHECK(etl::to_arithmetic<double>(text.c_str(), text.size()).has_value());

      //*********************************
      text = STR("-1.2345678901234567e2");

      f1 = strtold("-1.2345678901234567e2", nullptr);
      f2 = etl::to_arithmetic<long double>(text.c_str(), text.size()).value();

      CHECK_CLOSE(f1, f2, 0.00001);
      CHECK(etl::to_arithmetic<double>(text.c_str(), text.size()).has_value());

      //*********************************
      text = STR("-12345.678901234567e-2");

      f1 = strtold("-12345.678901234567e-2", nullptr);
      f2 = etl::to_arithmetic<long double>(text.c_str(), text.size()).value();

      CHECK_CLOSE(f1, f2, 0.00001);
      CHECK(etl::to_arithmetic<double>(text.c_str(), text.size()).has_value());

      //*********************************
      text = STR("+12345E-2");

      f1 = strtold("+12345E-2", nullptr);
      f2 = etl::to_arithmetic<long double>(text.c_str(), text.size()).value();

      CHECK_CLOSE(f1, f2, 0.00001);
      CHECK(etl::to_arithmetic<double>(text.c_str(), text.size()).has_value());

      //*********************************
      text = STR("+12345.678901234567E000");

      f1 = strtold("+12345.678901234567E000", nullptr);
      f2 = etl::to_arithmetic<long double>(text.c_str(), text.size()).value();

      CHECK_CLOSE(f1, f2, 0.00001);
      CHECK(etl::to_arithmetic<double>(text.c_str(), text.size()).has_value());
    }

    //*************************************************************************
    TEST(test_invalid_long_double)
    {
      Text text;

      //*********************************
      text = STR(" -123.456789");
      CHECK_EQUAL(etl::to_arithmetic_status::Invalid_Format, etl::to_arithmetic<long double>(text.c_str(), text.size()).error());

      //*********************************
      text = STR("-123.456789 ");
      CHECK_EQUAL(etl::to_arithmetic_status::Invalid_Format, etl::to_arithmetic<long double>(text.c_str(), text.size()).error());

      //*********************************
      text = STR("-12A.456789");
      CHECK_EQUAL(etl::to_arithmetic_status::Invalid_Format, etl::to_arithmetic<long double>(text.c_str(), text.size()).error());

      //*********************************
      text = STR("-123.456A89");
      CHECK_EQUAL(etl::to_arithmetic_status::Invalid_Format, etl::to_arithmetic<long double>(text.c_str(), text.size()).error());

      //*********************************
      text = STR("123.-456789");
      CHECK_EQUAL(etl::to_arithmetic_status::Invalid_Format, etl::to_arithmetic<long double>(text.c_str(), text.size()).error());

      //*********************************
      text = STR("123E.45");
      CHECK_EQUAL(etl::to_arithmetic_status::Invalid_Format, etl::to_arithmetic<long double>(text.c_str(), text.size()).error());

      //*********************************
      text = STR("123.45E10000");
      CHECK_EQUAL(etl::to_arithmetic_status::Overflow, etl::to_arithmetic<long double>(text.c_str(), text.size()).error());

      //*********************************
      text = STR("-123.45E10000");
      CHECK_EQUAL(etl::to_arithmetic_status::Overflow, etl::to_arithmetic<long double>(text.c_str(), text.size()).error());
    }

    //*************************************************************************
#if ETL_USING_CPP14
    TEST(test_constexpr_integral)
    {
      constexpr Text::const_pointer text{ STR("123") };

      constexpr etl::to_arithmetic_result<int> result = etl::to_arithmetic<int>(text, 3U, etl::radix::decimal);
      constexpr int i = result.value();

      CHECK_EQUAL(123, i);
    }
#endif
  }
}
