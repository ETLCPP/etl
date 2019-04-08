/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl
https://www.etlcpp.com

Copyright(c) 2019 jwellbelove

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

#include "UnitTest++.h"

#include <ostream>

#include "etl/to_string.h"
#include "etl/cstring.h"
#include "etl/format_spec.h"

#undef STR
#define STR(x) x

namespace
{
  typedef etl::format_spec<etl::istring> Format;

  std::ostream& operator << (std::ostream& os, const etl::istring& str)
  {
    for (auto c : str)
    {
      os << c;
    }

    return os;
  }

  SUITE(test_to_string)
  {
    //*************************************************************************
    TEST(test_default_format_no_append)
    {
      etl::string<20> str;

      CHECK_EQUAL(etl::string<20>(STR("0")),                    etl::to_string(uint8_t(0), str));
      CHECK_EQUAL(etl::string<20>(STR("0")),                    etl::to_string(uint16_t(0), str));
      CHECK_EQUAL(etl::string<20>(STR("0")),                    etl::to_string(uint32_t(0), str));
      CHECK_EQUAL(etl::string<20>(STR("0")),                    etl::to_string(uint64_t(0), str));

      CHECK_EQUAL(etl::string<20>(STR("128")),                  etl::to_string(uint8_t(128), str));
      CHECK_EQUAL(etl::string<20>(STR("32768")),                etl::to_string(uint16_t(32768), str));
      CHECK_EQUAL(etl::string<20>(STR("2147483648")),           etl::to_string(uint32_t(2147483648ul), str));
      CHECK_EQUAL(etl::string<20>(STR("9223372036854775808")),  etl::to_string(uint64_t(9223372036854775808ull), str));

      CHECK_EQUAL(etl::string<20>(STR("127")),                  etl::to_string(int8_t(127), str));
      CHECK_EQUAL(etl::string<20>(STR("32767")),                etl::to_string(int16_t(32767), str));
      CHECK_EQUAL(etl::string<20>(STR("2147483647")),           etl::to_string(int32_t(2147483647ll), str));
      CHECK_EQUAL(etl::string<20>(STR("9223372036854775807")),  etl::to_string(int64_t(9223372036854775807ll), str));

      CHECK_EQUAL(etl::string<20>(STR("-128")),                 etl::to_string(int8_t(-128), str));
      CHECK_EQUAL(etl::string<20>(STR("-32768")),               etl::to_string(int16_t(-32768), str));
      CHECK_EQUAL(etl::string<20>(STR("-2147483648")),          etl::to_string(int32_t(-2147483648ll), str));
      CHECK_EQUAL(etl::string<20>(STR("-9223372036854775808")), etl::to_string(int64_t(-9223372036854775808ll), str));
    }

    //*************************************************************************
    TEST(test_default_format_append)
    {
      etl::string<120> str;

      CHECK_EQUAL(etl::string<120>(STR("0")),     etl::to_string(uint8_t(0), str, true));
      CHECK_EQUAL(etl::string<120>(STR("00")),    etl::to_string(uint16_t(0), str, true));
      CHECK_EQUAL(etl::string<120>(STR("000")),   etl::to_string(uint32_t(0), str, true));
      CHECK_EQUAL(etl::string<120>(STR("0000")),  etl::to_string(uint64_t(0), str, true));

      CHECK_EQUAL(etl::string<120>(STR("0000128")),                                   etl::to_string(uint8_t(128), str, true));
      CHECK_EQUAL(etl::string<120>(STR("000012832768")),                              etl::to_string(uint16_t(32768), str, true));
      CHECK_EQUAL(etl::string<120>(STR("0000128327682147483648")),                    etl::to_string(uint32_t(2147483648ul), str, true));
      CHECK_EQUAL(etl::string<120>(STR("00001283276821474836489223372036854775808")), etl::to_string(uint64_t(9223372036854775808ull), str, true));

      CHECK_EQUAL(etl::string<120>(STR("00001283276821474836489223372036854775808127")),                                   etl::to_string(int8_t(127), str, true));
      CHECK_EQUAL(etl::string<120>(STR("0000128327682147483648922337203685477580812732767")),                              etl::to_string(int16_t(32767), str, true));
      CHECK_EQUAL(etl::string<120>(STR("00001283276821474836489223372036854775808127327672147483647")),                    etl::to_string(int32_t(2147483647ll), str, true));
      CHECK_EQUAL(etl::string<120>(STR("000012832768214748364892233720368547758081273276721474836479223372036854775807")), etl::to_string(int64_t(9223372036854775807ll), str, true));

      CHECK_EQUAL(etl::string<120>(STR("000012832768214748364892233720368547758081273276721474836479223372036854775807-128")),                                      etl::to_string(int8_t(-128), str, true));
      CHECK_EQUAL(etl::string<120>(STR("000012832768214748364892233720368547758081273276721474836479223372036854775807-128-32768")),                                etl::to_string(int16_t(-32768), str, true));
      CHECK_EQUAL(etl::string<120>(STR("000012832768214748364892233720368547758081273276721474836479223372036854775807-128-32768-2147483648")),                     etl::to_string(int32_t(-2147483648ll), str, true));
      CHECK_EQUAL(etl::string<120>(STR("000012832768214748364892233720368547758081273276721474836479223372036854775807-128-32768-2147483648-9223372036854775808")), etl::to_string(int64_t(-9223372036854775808ll), str, true));
    }

    //*************************************************************************
    TEST(test_format_right_justified_no_append)
    {
      etl::string<20> str;

      Format format = Format().base(10).width(20).fill(STR('#'));

      CHECK_EQUAL(etl::string<20>(STR("###################0")), etl::to_string(uint8_t(0), str, format));
      CHECK_EQUAL(etl::string<20>(STR("###################0")), etl::to_string(uint16_t(0), str, format));
      CHECK_EQUAL(etl::string<20>(STR("###################0")), etl::to_string(uint32_t(0), str, format));
      CHECK_EQUAL(etl::string<20>(STR("###################0")), etl::to_string(uint64_t(0), str, format));

      CHECK_EQUAL(etl::string<20>(STR("#################128")), etl::to_string(uint8_t(128), str, format));
      CHECK_EQUAL(etl::string<20>(STR("###############32768")), etl::to_string(uint16_t(32768), str, format));
      CHECK_EQUAL(etl::string<20>(STR("##########2147483648")), etl::to_string(uint32_t(2147483648ul), str, format));
      CHECK_EQUAL(etl::string<20>(STR("#9223372036854775808")), etl::to_string(uint64_t(9223372036854775808ull), str, format));

      CHECK_EQUAL(etl::string<20>(STR("#################127")), etl::to_string(int8_t(127), str, format));
      CHECK_EQUAL(etl::string<20>(STR("###############32767")), etl::to_string(int16_t(32767), str, format));
      CHECK_EQUAL(etl::string<20>(STR("##########2147483647")), etl::to_string(int32_t(2147483647ll), str, format));
      CHECK_EQUAL(etl::string<20>(STR("#9223372036854775807")), etl::to_string(int64_t(9223372036854775807ll), str, format));

      CHECK_EQUAL(etl::string<20>(STR("################-128")), etl::to_string(int8_t(-128), str, format));
      CHECK_EQUAL(etl::string<20>(STR("##############-32768")), etl::to_string(int16_t(-32768), str, format));
      CHECK_EQUAL(etl::string<20>(STR("#########-2147483648")), etl::to_string(int32_t(-2147483648ll), str, format));
      CHECK_EQUAL(etl::string<20>(STR("-9223372036854775808")), etl::to_string(int64_t(-9223372036854775808ll), str, format));
    }

    //*************************************************************************
    TEST(test_format_left_justified_no_append)
    {
      etl::string<20> str;

      Format format = Format().base(10).width(20).fill(STR('#')).left_justified(true);

      CHECK_EQUAL(etl::string<20>(STR("0###################")), etl::to_string(uint8_t(0), str, format));
      CHECK_EQUAL(etl::string<20>(STR("0###################")), etl::to_string(uint16_t(0), str, format));
      CHECK_EQUAL(etl::string<20>(STR("0###################")), etl::to_string(uint32_t(0), str, format));
      CHECK_EQUAL(etl::string<20>(STR("0###################")), etl::to_string(uint64_t(0), str, format));

      CHECK_EQUAL(etl::string<20>(STR("128#################")), etl::to_string(uint8_t(128), str, format));
      CHECK_EQUAL(etl::string<20>(STR("32768###############")), etl::to_string(uint16_t(32768), str, format));
      CHECK_EQUAL(etl::string<20>(STR("2147483648##########")), etl::to_string(uint32_t(2147483648ul), str, format));
      CHECK_EQUAL(etl::string<20>(STR("9223372036854775808#")), etl::to_string(uint64_t(9223372036854775808ull), str, format));

      CHECK_EQUAL(etl::string<20>(STR("127#################")), etl::to_string(int8_t(127), str, format));
      CHECK_EQUAL(etl::string<20>(STR("32767###############")), etl::to_string(int16_t(32767), str, format));
      CHECK_EQUAL(etl::string<20>(STR("2147483647##########")), etl::to_string(int32_t(2147483647ll), str, format));
      CHECK_EQUAL(etl::string<20>(STR("9223372036854775807#")), etl::to_string(int64_t(9223372036854775807ll), str, format));

      CHECK_EQUAL(etl::string<20>(STR("-128################")), etl::to_string(int8_t(-128), str, format));
      CHECK_EQUAL(etl::string<20>(STR("-32768##############")), etl::to_string(int16_t(-32768), str, format));
      CHECK_EQUAL(etl::string<20>(STR("-2147483648#########")), etl::to_string(int32_t(-2147483648ll), str, format));
      CHECK_EQUAL(etl::string<20>(STR("-9223372036854775808")), etl::to_string(int64_t(-9223372036854775808ll), str, format));
    }

    //*************************************************************************
    TEST(test_binary_format_no_append)
    {
      etl::string<64> str;

      CHECK_EQUAL(etl::string<64>(STR("00000000")),                                                         etl::to_string(uint8_t(0), str, Format().base(2).width(8).fill(STR('0'))));
      CHECK_EQUAL(etl::string<64>(STR("0000000000000000")),                                                 etl::to_string(uint16_t(0), str, Format().base(2).width(16).fill(STR('0'))));
      CHECK_EQUAL(etl::string<64>(STR("00000000000000000000000000000000")),                                 etl::to_string(uint32_t(0), str, Format().base(2).width(32).fill(STR('0'))));
      CHECK_EQUAL(etl::string<64>(STR("0000000000000000000000000000000000000000000000000000000000000000")), etl::to_string(uint64_t(0), str, Format().base(2).width(64).fill(STR('0'))));

      CHECK_EQUAL(etl::string<64>(STR("10000000")),                                                         etl::to_string(uint8_t(128), str, Format().base(2).width(8).fill(STR('0'))));
      CHECK_EQUAL(etl::string<64>(STR("1000000000000000")),                                                 etl::to_string(uint16_t(32768), str, Format().base(2).width(16).fill(STR('0'))));
      CHECK_EQUAL(etl::string<64>(STR("10000000000000000000000000000000")),                                 etl::to_string(uint32_t(2147483648ul), str, Format().base(2).width(32).fill(STR('0'))));
      CHECK_EQUAL(etl::string<64>(STR("1000000000000000000000000000000000000000000000000000000000000000")), etl::to_string(uint64_t(9223372036854775808ull), str, Format().base(2).width(64).fill(STR('0'))));

      CHECK_EQUAL(etl::string<64>(STR("01111111")),                                                         etl::to_string(int8_t(127), str, Format().base(2).width(8).fill(STR('0'))));
      CHECK_EQUAL(etl::string<64>(STR("0111111111111111")),                                                 etl::to_string(int16_t(32767), str, Format().base(2).width(16).fill(STR('0'))));
      CHECK_EQUAL(etl::string<64>(STR("01111111111111111111111111111111")),                                 etl::to_string(int32_t(2147483647ll), str, Format().base(2).width(32).fill(STR('0'))));
      CHECK_EQUAL(etl::string<64>(STR("0111111111111111111111111111111111111111111111111111111111111111")), etl::to_string(int64_t(9223372036854775807ll), str, Format().base(2).width(64).fill(STR('0'))));

      CHECK_EQUAL(etl::string<64>(STR("10000000")),                                                         etl::to_string(int8_t(-128), str, Format().base(2).width(8).fill(STR('0'))));
      CHECK_EQUAL(etl::string<64>(STR("1000000000000000")),                                                 etl::to_string(int16_t(-32768), str, Format().base(2).width(16).fill(STR('0'))));
      CHECK_EQUAL(etl::string<64>(STR("10000000000000000000000000000000")),                                 etl::to_string(int32_t(-2147483648ll), str, Format().base(2).width(32).fill(STR('0'))));
      CHECK_EQUAL(etl::string<64>(STR("1000000000000000000000000000000000000000000000000000000000000000")), etl::to_string(int64_t(-9223372036854775808ll), str, Format().base(2).width(64).fill(STR('0'))));
    }

    //*************************************************************************
    TEST(test_octal_format_no_append)
    {
      etl::string<22> str;

      CHECK_EQUAL(etl::string<22>(STR("000")),                    etl::to_string(uint8_t(0), str, Format().base(8).width(3).fill(STR('0'))));
      CHECK_EQUAL(etl::string<22>(STR("000000")),                 etl::to_string(uint16_t(0), str, Format().base(8).width(6).fill(STR('0'))));
      CHECK_EQUAL(etl::string<22>(STR("00000000000")),            etl::to_string(uint32_t(0), str, Format().base(8).width(11).fill(STR('0'))));
      CHECK_EQUAL(etl::string<22>(STR("0000000000000000000000")), etl::to_string(uint64_t(0), str, Format().base(8).width(22).fill(STR('0'))));

      CHECK_EQUAL(etl::string<22>(STR("200")),                    etl::to_string(uint8_t(128), str, Format().base(8).width(3).fill(STR('0'))));
      CHECK_EQUAL(etl::string<22>(STR("100000")),                 etl::to_string(uint16_t(32768), str, Format().base(8).width(6).fill(STR('0'))));
      CHECK_EQUAL(etl::string<22>(STR("20000000000")),            etl::to_string(uint32_t(2147483648ul), str, Format().base(8).width(11).fill(STR('0'))));
      CHECK_EQUAL(etl::string<22>(STR("1000000000000000000000")), etl::to_string(uint64_t(9223372036854775808ull), str, Format().base(8).width(22).fill(STR('0'))));

      CHECK_EQUAL(etl::string<22>(STR("177")),                    etl::to_string(int8_t(127), str, Format().base(8).width(3).fill(STR('0'))));
      CHECK_EQUAL(etl::string<22>(STR("077777")),                 etl::to_string(int16_t(32767), str, Format().base(8).width(6).fill(STR('0'))));
      CHECK_EQUAL(etl::string<22>(STR("17777777777")),            etl::to_string(int32_t(2147483647ll), str, Format().base(8).width(11).fill(STR('0'))));
      CHECK_EQUAL(etl::string<22>(STR("0777777777777777777777")), etl::to_string(int64_t(9223372036854775807ll), str, Format().base(8).width(22).fill(STR('0'))));

      CHECK_EQUAL(etl::string<22>(STR("200")),                    etl::to_string(int8_t(-128), str, Format().base(8).width(3).fill(STR('0'))));
      CHECK_EQUAL(etl::string<22>(STR("100000")),                 etl::to_string(int16_t(-32768), str, Format().base(8).width(6).fill(STR('0'))));
      CHECK_EQUAL(etl::string<22>(STR("20000000000")),            etl::to_string(int32_t(-2147483648ll), str, Format().base(8).width(11).fill(STR('0'))));
      CHECK_EQUAL(etl::string<22>(STR("1000000000000000000000")), etl::to_string(int64_t(-9223372036854775808ll), str, Format().base(8).width(22).fill(STR('0'))));
    }

    //*************************************************************************
    TEST(test_hex_format_no_append)
    {
      etl::string<16> str;

      CHECK_EQUAL(etl::string<16>(STR("00")),               etl::to_string(uint8_t(0), str, Format().base(16).width(2).fill(STR('0'))));
      CHECK_EQUAL(etl::string<16>(STR("0000")),             etl::to_string(uint16_t(0), str, Format().base(16).width(4).fill(STR('0'))));
      CHECK_EQUAL(etl::string<16>(STR("00000000")),         etl::to_string(uint32_t(0), str, Format().base(16).width(8).fill(STR('0'))));
      CHECK_EQUAL(etl::string<16>(STR("0000000000000000")), etl::to_string(uint64_t(0), str, Format().base(16).width(16).fill(STR('0'))));

      CHECK_EQUAL(etl::string<16>(STR("80")),               etl::to_string(uint8_t(128), str, Format().base(16).width(2).fill(STR('0'))));
      CHECK_EQUAL(etl::string<16>(STR("8000")),             etl::to_string(uint16_t(32768), str, Format().base(16).width(4).fill(STR('0'))));
      CHECK_EQUAL(etl::string<16>(STR("80000000")),         etl::to_string(uint32_t(2147483648ul), str, Format().base(16).width(8).fill(STR('0'))));
      CHECK_EQUAL(etl::string<16>(STR("8000000000000000")), etl::to_string(uint64_t(9223372036854775808ull), str, Format().base(16).width(16).fill(STR('0'))));

      CHECK_EQUAL(etl::string<16>(STR("7F")),               etl::to_string(int8_t(127), str, Format().base(16).width(2).fill(STR('0'))));
      CHECK_EQUAL(etl::string<16>(STR("7FFF")),             etl::to_string(int16_t(32767), str, Format().base(16).width(4).fill(STR('0'))));
      CHECK_EQUAL(etl::string<16>(STR("7FFFFFFF")),         etl::to_string(int32_t(2147483647ll), str, Format().base(16).width(8).fill(STR('0'))));
      CHECK_EQUAL(etl::string<16>(STR("7FFFFFFFFFFFFFFF")), etl::to_string(int64_t(9223372036854775807ll), str, Format().base(16).width(16).fill(STR('0'))));

      CHECK_EQUAL(etl::string<16>(STR("80")),               etl::to_string(int8_t(-128), str, Format().base(16).width(2).fill(STR('0'))));
      CHECK_EQUAL(etl::string<16>(STR("8000")),             etl::to_string(int16_t(-32768), str, Format().base(16).width(4).fill(STR('0'))));
      CHECK_EQUAL(etl::string<16>(STR("80000000")),         etl::to_string(int32_t(-2147483648ll), str, Format().base(16).width(8).fill(STR('0'))));
      CHECK_EQUAL(etl::string<16>(STR("8000000000000000")), etl::to_string(int64_t(-9223372036854775808ll), str, Format().base(16).width(16).fill(STR('0'))));
    }

    //*************************************************************************
    TEST(test_named_format_no_append)
    {
      etl::string<17> str;

      CHECK_EQUAL(etl::string<17>(STR("11110001001000000")),  etl::to_string(123456, str, Format().binary()));
      CHECK_EQUAL(etl::string<17>(STR("361100")),             etl::to_string(123456, str, Format().octal()));
      CHECK_EQUAL(etl::string<17>(STR("123456")),             etl::to_string(123456, str, Format().decimal()));
      CHECK_EQUAL(etl::string<17>(STR("1E240")),              etl::to_string(123456, str, Format().hex()));
    }
  };
}
