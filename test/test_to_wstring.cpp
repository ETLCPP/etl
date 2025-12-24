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

#include "etl/to_wstring.h"
#include "etl/wstring.h"
#include "etl/format_spec.h"

#undef STR
#define STR(x) L##x

namespace
{
  typedef etl::wformat_spec Format;

  SUITE(test_to_wstring)
  {
    //*************************************************************************
    TEST(test_default_format_no_append)
    {
      etl::wstring<20> str;

      CHECK(etl::wstring<20>(STR("0")) ==                    etl::to_string(uint8_t(0), str));
      CHECK(etl::wstring<20>(STR("0")) ==                    etl::to_string(uint16_t(0), str));
      CHECK(etl::wstring<20>(STR("0")) ==                    etl::to_string(uint32_t(0), str));
      CHECK(etl::wstring<20>(STR("0")) ==                    etl::to_string(uint64_t(0), str));

      CHECK(etl::wstring<20>(STR("128")) ==                  etl::to_string(uint8_t(128), str));
      CHECK(etl::wstring<20>(STR("32768")) ==                etl::to_string(uint16_t(32768), str));
      CHECK(etl::wstring<20>(STR("2147483648")) ==           etl::to_string(uint32_t(2147483648ul), str));
      CHECK(etl::wstring<20>(STR("9223372036854775808")) ==  etl::to_string(uint64_t(9223372036854775808ull), str));

      CHECK(etl::wstring<20>(STR("127")) ==                  etl::to_string(INT8_MAX, str));
      CHECK(etl::wstring<20>(STR("32767")) ==                etl::to_string(INT16_MAX, str));
      CHECK(etl::wstring<20>(STR("2147483647")) ==           etl::to_string(INT32_MAX, str));
      CHECK(etl::wstring<20>(STR("9223372036854775807")) ==  etl::to_string(INT64_MAX, str));

      CHECK(etl::wstring<20>(STR("-128")) ==                 etl::to_string(INT8_MIN, str));
      CHECK(etl::wstring<20>(STR("-32768")) ==               etl::to_string(INT16_MIN, str));
      CHECK(etl::wstring<20>(STR("-2147483648")) ==          etl::to_string(INT32_MIN, str));
      CHECK(etl::wstring<20>(STR("-9223372036854775808")) == etl::to_string(INT64_MIN, str));
    }

    //*************************************************************************
    TEST(test_default_format_append)
    {
      etl::wstring<120> str;

      CHECK(etl::wstring<120>(STR("0")) ==     etl::to_string(uint8_t(0),  str, true));
      CHECK(etl::wstring<120>(STR("00")) ==    etl::to_string(uint16_t(0), str, true));
      CHECK(etl::wstring<120>(STR("000")) ==   etl::to_string(uint32_t(0), str, true));
      CHECK(etl::wstring<120>(STR("0000")) ==  etl::to_string(uint64_t(0), str, true));

      CHECK(etl::wstring<120>(STR("0000128")) ==                                   etl::to_string(uint8_t(128), str, true));
      CHECK(etl::wstring<120>(STR("000012832768")) ==                              etl::to_string(uint16_t(32768), str, true));
      CHECK(etl::wstring<120>(STR("0000128327682147483648")) ==                    etl::to_string(uint32_t(2147483648ul), str, true));
      CHECK(etl::wstring<120>(STR("00001283276821474836489223372036854775808")) == etl::to_string(uint64_t(9223372036854775808ull), str, true));

      CHECK(etl::wstring<120>(STR("00001283276821474836489223372036854775808127")) ==                                   etl::to_string(INT8_MAX, str, true));
      CHECK(etl::wstring<120>(STR("0000128327682147483648922337203685477580812732767")) ==                              etl::to_string(INT16_MAX, str, true));
      CHECK(etl::wstring<120>(STR("00001283276821474836489223372036854775808127327672147483647")) ==                    etl::to_string(INT32_MAX, str, true));
      CHECK(etl::wstring<120>(STR("000012832768214748364892233720368547758081273276721474836479223372036854775807")) == etl::to_string(INT64_MAX, str, true));

      CHECK(etl::wstring<120>(STR("000012832768214748364892233720368547758081273276721474836479223372036854775807-128")) ==                                      etl::to_string(INT8_MIN, str, true));
      CHECK(etl::wstring<120>(STR("000012832768214748364892233720368547758081273276721474836479223372036854775807-128-32768")) ==                                etl::to_string(INT16_MIN, str, true));
      CHECK(etl::wstring<120>(STR("000012832768214748364892233720368547758081273276721474836479223372036854775807-128-32768-2147483648")) ==                     etl::to_string(INT32_MIN, str, true));
      CHECK(etl::wstring<120>(STR("000012832768214748364892233720368547758081273276721474836479223372036854775807-128-32768-2147483648-9223372036854775808")) == etl::to_string(INT64_MIN, str, true));
    }

    //*************************************************************************
    TEST(test_format_right_justified_no_append)
    {
      etl::wstring<20> str;

      Format format = Format().base(10).width(20).fill(STR('#'));

      CHECK(etl::wstring<20>(STR("###################0")) == etl::to_string(uint8_t(0),  str, format));
      CHECK(etl::wstring<20>(STR("###################0")) == etl::to_string(uint16_t(0), str, format));
      CHECK(etl::wstring<20>(STR("###################0")) == etl::to_string(uint32_t(0), str, format));
      CHECK(etl::wstring<20>(STR("###################0")) == etl::to_string(uint64_t(0), str, format));

      CHECK(etl::wstring<20>(STR("#################128")) == etl::to_string(uint8_t(128), str, format));
      CHECK(etl::wstring<20>(STR("###############32768")) == etl::to_string(uint16_t(32768), str, format));
      CHECK(etl::wstring<20>(STR("##########2147483648")) == etl::to_string(uint32_t(2147483648ul), str, format));
      CHECK(etl::wstring<20>(STR("#9223372036854775808")) == etl::to_string(uint64_t(9223372036854775808ull), str, format));

      CHECK(etl::wstring<20>(STR("#################127")) == etl::to_string(INT8_MAX, str, format));
      CHECK(etl::wstring<20>(STR("###############32767")) == etl::to_string(INT16_MAX, str, format));
      CHECK(etl::wstring<20>(STR("##########2147483647")) == etl::to_string(INT32_MAX, str, format));
      CHECK(etl::wstring<20>(STR("#9223372036854775807")) == etl::to_string(INT64_MAX, str, format));

      CHECK(etl::wstring<20>(STR("################-128")) == etl::to_string(INT8_MIN, str, format));
      CHECK(etl::wstring<20>(STR("##############-32768")) == etl::to_string(INT16_MIN, str, format));
      CHECK(etl::wstring<20>(STR("#########-2147483648")) == etl::to_string(INT32_MIN, str, format));
      CHECK(etl::wstring<20>(STR("-9223372036854775808")) == etl::to_string(INT64_MIN, str, format));
    }

    //*************************************************************************
    TEST(test_format_left_justified_no_append)
    {
      etl::wstring<20> str;

      Format format = Format().base(10).width(20).fill(STR('#')).left();

      CHECK(etl::wstring<20>(STR("0###################")) == etl::to_string(uint8_t(0),  str, format));
      CHECK(etl::wstring<20>(STR("0###################")) == etl::to_string(uint16_t(0), str, format));
      CHECK(etl::wstring<20>(STR("0###################")) == etl::to_string(uint32_t(0), str, format));
      CHECK(etl::wstring<20>(STR("0###################")) == etl::to_string(uint64_t(0), str, format));

      CHECK(etl::wstring<20>(STR("128#################")) == etl::to_string(uint8_t(128), str, format));
      CHECK(etl::wstring<20>(STR("32768###############")) == etl::to_string(uint16_t(32768), str, format));
      CHECK(etl::wstring<20>(STR("2147483648##########")) == etl::to_string(uint32_t(2147483648ul), str, format));
      CHECK(etl::wstring<20>(STR("9223372036854775808#")) == etl::to_string(uint64_t(9223372036854775808ull), str, format));

      CHECK(etl::wstring<20>(STR("127#################")) == etl::to_string(INT8_MAX, str, format));
      CHECK(etl::wstring<20>(STR("32767###############")) == etl::to_string(INT16_MAX, str, format));
      CHECK(etl::wstring<20>(STR("2147483647##########")) == etl::to_string(INT32_MAX, str, format));
      CHECK(etl::wstring<20>(STR("9223372036854775807#")) == etl::to_string(INT64_MAX, str, format));

      CHECK(etl::wstring<20>(STR("-128################")) == etl::to_string(INT8_MIN, str, format));
      CHECK(etl::wstring<20>(STR("-32768##############")) == etl::to_string(INT16_MIN, str, format));
      CHECK(etl::wstring<20>(STR("-2147483648#########")) == etl::to_string(INT32_MIN, str, format));
      CHECK(etl::wstring<20>(STR("-9223372036854775808")) == etl::to_string(INT64_MIN, str, format));
    }

    //*************************************************************************
    TEST(test_binary_format_no_append)
    {
      etl::wstring<64> str;

      CHECK(etl::wstring<64>(STR("00000000")) ==                                                          etl::to_string(uint8_t(0),  str, Format().base(2).width(8).fill(STR('0'))));
      CHECK(etl::wstring<64>(STR("0000000000000000")) ==                                                  etl::to_string(uint16_t(0), str, Format().base(2).width(16).fill(STR('0'))));
      CHECK(etl::wstring<64>(STR("00000000000000000000000000000000")) ==                                  etl::to_string(uint32_t(0), str, Format().base(2).width(32).fill(STR('0'))));
      CHECK(etl::wstring<64>(STR("0000000000000000000000000000000000000000000000000000000000000000")) ==  etl::to_string(uint64_t(0), str, Format().base(2).width(64).fill(STR('0'))));

      CHECK(etl::wstring<64>(STR("10000000")) ==                                                          etl::to_string(uint8_t(128), str, Format().base(2).width(8).fill(STR('0'))));
      CHECK(etl::wstring<64>(STR("1000000000000000")) ==                                                  etl::to_string(uint16_t(32768), str, Format().base(2).width(16).fill(STR('0'))));
      CHECK(etl::wstring<64>(STR("10000000000000000000000000000000")) ==                                  etl::to_string(uint32_t(2147483648ul), str, Format().base(2).width(32).fill(STR('0'))));
      CHECK(etl::wstring<64>(STR("1000000000000000000000000000000000000000000000000000000000000000")) ==  etl::to_string(uint64_t(9223372036854775808ull), str, Format().base(2).width(64).fill(STR('0'))));

      CHECK(etl::wstring<64>(STR("01111111")) ==                                                          etl::to_string(INT8_MAX, str, Format().base(2).width(8).fill(STR('0'))));
      CHECK(etl::wstring<64>(STR("0111111111111111")) ==                                                  etl::to_string(INT16_MAX, str, Format().base(2).width(16).fill(STR('0'))));
      CHECK(etl::wstring<64>(STR("01111111111111111111111111111111")) ==                                  etl::to_string(INT32_MAX, str, Format().base(2).width(32).fill(STR('0'))));
      CHECK(etl::wstring<64>(STR("0111111111111111111111111111111111111111111111111111111111111111")) ==  etl::to_string(INT64_MAX, str, Format().base(2).width(64).fill(STR('0'))));

      CHECK(etl::wstring<64>(STR("10000000")) ==                                                          etl::to_string(INT8_MIN, str, Format().base(2).width(8).fill(STR('0'))));
      CHECK(etl::wstring<64>(STR("1000000000000000")) ==                                                  etl::to_string(INT16_MIN, str, Format().base(2).width(16).fill(STR('0'))));
      CHECK(etl::wstring<64>(STR("10000000000000000000000000000000")) ==                                  etl::to_string(INT32_MIN, str, Format().base(2).width(32).fill(STR('0'))));
      CHECK(etl::wstring<64>(STR("1000000000000000000000000000000000000000000000000000000000000000")) ==  etl::to_string(INT64_MIN, str, Format().base(2).width(64).fill(STR('0'))));
    }

    //*************************************************************************
    TEST(test_octal_format_no_append)
    {
      etl::wstring<22> str;

      CHECK(etl::wstring<22>(STR("000")) ==                    etl::to_string(uint8_t(0),  str, Format().base(8).width(3).fill(STR('0'))));
      CHECK(etl::wstring<22>(STR("000000")) ==                 etl::to_string(uint16_t(0), str, Format().base(8).width(6).fill(STR('0'))));
      CHECK(etl::wstring<22>(STR("00000000000")) ==            etl::to_string(uint32_t(0), str, Format().base(8).width(11).fill(STR('0'))));
      CHECK(etl::wstring<22>(STR("0000000000000000000000")) == etl::to_string(uint64_t(0), str, Format().base(8).width(22).fill(STR('0'))));

      CHECK(etl::wstring<22>(STR("200")) ==                    etl::to_string(uint8_t(128), str, Format().base(8).width(3).fill(STR('0'))));
      CHECK(etl::wstring<22>(STR("100000")) ==                 etl::to_string(uint16_t(32768), str, Format().base(8).width(6).fill(STR('0'))));
      CHECK(etl::wstring<22>(STR("20000000000")) ==            etl::to_string(uint32_t(2147483648ul), str, Format().base(8).width(11).fill(STR('0'))));
      CHECK(etl::wstring<22>(STR("1000000000000000000000")) == etl::to_string(uint64_t(9223372036854775808ull), str, Format().base(8).width(22).fill(STR('0'))));

      CHECK(etl::wstring<22>(STR("177")) ==                    etl::to_string(INT8_MAX, str, Format().base(8).width(3).fill(STR('0'))));
      CHECK(etl::wstring<22>(STR("077777")) ==                 etl::to_string(INT16_MAX, str, Format().base(8).width(6).fill(STR('0'))));
      CHECK(etl::wstring<22>(STR("17777777777")) ==            etl::to_string(INT32_MAX, str, Format().base(8).width(11).fill(STR('0'))));
      CHECK(etl::wstring<22>(STR("0777777777777777777777")) == etl::to_string(INT64_MAX, str, Format().base(8).width(22).fill(STR('0'))));

      CHECK(etl::wstring<22>(STR("200")) ==                    etl::to_string(INT8_MIN, str, Format().base(8).width(3).fill(STR('0'))));
      CHECK(etl::wstring<22>(STR("100000")) ==                 etl::to_string(INT16_MIN, str, Format().base(8).width(6).fill(STR('0'))));
      CHECK(etl::wstring<22>(STR("20000000000")) ==            etl::to_string(INT32_MIN, str, Format().base(8).width(11).fill(STR('0'))));
      CHECK(etl::wstring<22>(STR("1000000000000000000000")) == etl::to_string(INT64_MIN, str, Format().base(8).width(22).fill(STR('0'))));
    }

    //*************************************************************************
    TEST(test_hex_format_no_append)
    {
      etl::wstring<16> str;

      CHECK(etl::wstring<16>(STR("00")) ==               etl::to_string(uint8_t(0),  str, Format().base(16).width(2).fill(STR('0'))));
      CHECK(etl::wstring<16>(STR("0000")) ==             etl::to_string(uint16_t(0), str, Format().base(16).width(4).fill(STR('0'))));
      CHECK(etl::wstring<16>(STR("00000000")) ==         etl::to_string(uint32_t(0), str, Format().base(16).width(8).fill(STR('0'))));
      CHECK(etl::wstring<16>(STR("0000000000000000")) == etl::to_string(uint64_t(0), str, Format().base(16).width(16).fill(STR('0'))));

      CHECK(etl::wstring<16>(STR("80")) ==               etl::to_string(uint8_t(128), str, Format().base(16).width(2).fill(STR('0'))));
      CHECK(etl::wstring<16>(STR("8000")) ==             etl::to_string(uint16_t(32768), str, Format().base(16).width(4).fill(STR('0'))));
      CHECK(etl::wstring<16>(STR("80000000")) ==         etl::to_string(uint32_t(2147483648ul), str, Format().base(16).width(8).fill(STR('0'))));
      CHECK(etl::wstring<16>(STR("8000000000000000")) == etl::to_string(uint64_t(9223372036854775808ull), str, Format().base(16).width(16).fill(STR('0'))));

      CHECK(etl::wstring<16>(STR("7f")) ==               etl::to_string(INT8_MAX, str, Format().base(16).width(2).fill(STR('0'))));
      CHECK(etl::wstring<16>(STR("7fff")) ==             etl::to_string(INT16_MAX, str, Format().base(16).width(4).fill(STR('0'))));
      CHECK(etl::wstring<16>(STR("7fffffff")) ==         etl::to_string(INT32_MAX, str, Format().base(16).width(8).fill(STR('0'))));
      CHECK(etl::wstring<16>(STR("7fffffffffffffff")) == etl::to_string(INT64_MAX, str, Format().base(16).width(16).fill(STR('0'))));

      CHECK(etl::wstring<16>(STR("80")) ==               etl::to_string(INT8_MIN, str, Format().base(16).width(2).fill(STR('0'))));
      CHECK(etl::wstring<16>(STR("8000")) ==             etl::to_string(INT16_MIN, str, Format().base(16).width(4).fill(STR('0'))));
      CHECK(etl::wstring<16>(STR("80000000")) ==         etl::to_string(INT32_MIN, str, Format().base(16).width(8).fill(STR('0'))));
      CHECK(etl::wstring<16>(STR("8000000000000000")) == etl::to_string(INT64_MIN, str, Format().base(16).width(16).fill(STR('0'))));
    }

    //*************************************************************************
    TEST(test_named_format_no_append)
    {
      etl::wstring<17> str;

      CHECK(etl::wstring<17>(STR("11110001001000000")) == etl::to_string(123456, str, Format().binary()));
      CHECK(etl::wstring<17>(STR("361100")) ==            etl::to_string(123456, str, Format().octal()));
      CHECK(etl::wstring<17>(STR("123456")) ==            etl::to_string(123456, str, Format().decimal()));
      CHECK(etl::wstring<17>(STR("1e240")) ==             etl::to_string(123456, str, Format().hex()));
    }

    //*************************************************************************
    TEST(test_floating_point_no_append)
    {
      etl::wstring<20> str;

      CHECK(etl::wstring<20>(STR("  0.000000")) == etl::to_string(0.0, str, Format().precision(6).width(10).right()));
      CHECK(etl::wstring<20>(STR("0.000000  ")) == etl::to_string(0.0, str, Format().precision(6).width(10).left()));

      CHECK(etl::wstring<20>(STR("  0.000001")) == etl::to_string(0.000001, str, Format().precision(6).width(10).right()));
      CHECK(etl::wstring<20>(STR("0.000001  ")) == etl::to_string(0.000001, str, Format().precision(6).width(10).left()));

      CHECK(etl::wstring<20>(STR("  1.000000")) == etl::to_string(1.0, str, Format().precision(6).width(10).right()));
      CHECK(etl::wstring<20>(STR("1.000000  ")) == etl::to_string(1.0, str, Format().precision(6).width(10).left()));

      CHECK(etl::wstring<20>(STR("  1.000001")) == etl::to_string(1.000001, str, Format().precision(6).width(10).right()));
      CHECK(etl::wstring<20>(STR("1.000001  ")) == etl::to_string(1.000001, str, Format().precision(6).width(10).left()));

      CHECK(etl::wstring<20>(STR(" 12.345678")) == etl::to_string(12.345678, str, Format().precision(6).width(10).right()));
      CHECK(etl::wstring<20>(STR("12.345678 ")) == etl::to_string(12.345678, str, Format().precision(6).width(10).left()));

      CHECK(etl::wstring<20>(STR(" -12.345678")) == etl::to_string(-12.345678, str, Format().precision(6).width(11).right()));
      CHECK(etl::wstring<20>(STR("-12.345678 ")) == etl::to_string(-12.345678, str, Format().precision(6).width(11).left()));

      CHECK(etl::wstring<20>(STR(" -0.123456")) == etl::to_string(-0.123456, str, Format().precision(6).width(10).right()));
      CHECK(etl::wstring<20>(STR("-0.123456 ")) == etl::to_string(-0.123456, str, Format().precision(6).width(10).left()));
    }

    //*************************************************************************
    TEST(test_floating_point_append)
    {
      etl::wstring<20> str;

      str.assign(STR("Result "));
      CHECK(etl::wstring<20>(STR("Result  12.345678")) == etl::to_string(12.345678, str, Format().precision(6).width(10).right(), true));

      str.assign(STR("Result "));
      CHECK(etl::wstring<20>(STR("Result 12.345678 ")) == etl::to_string(12.345678, str, Format().precision(6).width(10).left(), true));

      str.assign(STR("Result "));
      CHECK(etl::wstring<20>(STR("Result  -12.345678")) == etl::to_string(-12.345678, str, Format().precision(6).width(11).right(), true));

      str.assign(STR("Result "));
      CHECK(etl::wstring<20>(STR("Result -12.345678 ")) == etl::to_string(-12.345678, str, Format().precision(6).width(11).left(), true));

      str.assign(STR("Result "));
      CHECK(etl::wstring<20>(STR("Result -0.123456 ")) == etl::to_string(-0.123456, str, Format().precision(6).width(10).left(), true));
    }

    //*************************************************************************
    TEST(test_floating_point_rounding)
    {
      etl::wstring<20> str;

      CHECK(etl::wstring<20>(STR("0.00001")) == etl::to_string(0.000009, str, Format().precision(5).width(7).right()));
      CHECK(etl::wstring<20>(STR("0.0001")) ==  etl::to_string(0.000099, str, Format().precision(4).width(6).right()));
      CHECK(etl::wstring<20>(STR("0.001")) ==   etl::to_string(0.000999, str, Format().precision(3).width(5).right()));
      CHECK(etl::wstring<20>(STR("0.01")) ==    etl::to_string(0.009999, str, Format().precision(2).width(4).right()));
      CHECK(etl::wstring<20>(STR("0.1")) ==     etl::to_string(0.099999, str, Format().precision(1).width(3).right()));
      CHECK(etl::wstring<20>(STR("1.0")) ==     etl::to_string(0.999999, str, Format().precision(1).width(3).right()));
      CHECK(etl::wstring<20>(STR("1")) ==       etl::to_string(0.999999, str, Format().precision(0).width(1).right()));
      CHECK(etl::wstring<20>(STR("10.0")) ==    etl::to_string(9.999999, str, Format().precision(1).width(4).right()));
      CHECK(etl::wstring<20>(STR("2")) ==       etl::to_string(1.999999, str, Format().precision(0).width(1).right()));
      CHECK(etl::wstring<20>(STR("10.0")) ==    etl::to_string(9.999999, str, Format().precision(1).width(4).right()));
      CHECK(etl::wstring<20>(STR("20.0")) ==    etl::to_string(19.999999, str, Format().precision(1).width(4).right()));
    }

    //*************************************************************************
    TEST(test_bool_no_append)
    {
      etl::wstring<20> str;

      CHECK(etl::wstring<20>(STR("         0")) == to_string(false, str, Format().precision(6).width(10).right().boolalpha(false)));
      CHECK(etl::wstring<20>(STR("         1")) == to_string(true,  str, Format().precision(6).width(10).right().boolalpha(false)));
      CHECK(etl::wstring<20>(STR("0         ")) == to_string(false, str, Format().precision(6).width(10).left().boolalpha(false)));
      CHECK(etl::wstring<20>(STR("1         ")) == to_string(true,  str, Format().precision(6).width(10).left().boolalpha(false)));

      CHECK(etl::wstring<20>(STR("     false")) == to_string(false, str, Format().precision(6).width(10).right().boolalpha(true)));
      CHECK(etl::wstring<20>(STR("      true")) == to_string(true,  str, Format().precision(6).width(10).right().boolalpha(true)));
      CHECK(etl::wstring<20>(STR("false     ")) == to_string(false, str, Format().precision(6).width(10).left().boolalpha(true)));
      CHECK(etl::wstring<20>(STR("true      ")) == to_string(true,  str, Format().precision(6).width(10).left().boolalpha(true)));
    }

    //*************************************************************************
    TEST(test_bool_append)
    {
      etl::wstring<20> str;

      str.assign(STR("Result "));
      CHECK(etl::wstring<20>(STR("Result          0")) == to_string(false, str, Format().precision(6).width(10).right().boolalpha(false), true));

      str.assign(STR("Result "));
      CHECK(etl::wstring<20>(STR("Result          1")) == to_string(true, str, Format().precision(6).width(10).right().boolalpha(false), true));

      str.assign(STR("Result "));
      CHECK(etl::wstring<20>(STR("Result 0         ")) == to_string(false, str, Format().precision(6).width(10).left().boolalpha(false), true));

      str.assign(STR("Result "));
      CHECK(etl::wstring<20>(STR("Result 1         ")) == to_string(true, str, Format().precision(6).width(10).left().boolalpha(false), true));

      str.assign(STR("Result "));
      CHECK(etl::wstring<20>(STR("Result      false")) == to_string(false, str, Format().precision(6).width(10).right().boolalpha(true), true));

      str.assign(STR("Result "));
      CHECK(etl::wstring<20>(STR("Result       true")) == to_string(true, str, Format().precision(6).width(10).right().boolalpha(true), true));

      str.assign(STR("Result "));
      CHECK(etl::wstring<20>(STR("Result false     ")) == to_string(false, str, Format().precision(6).width(10).left().boolalpha(true), true));

      str.assign(STR("Result "));
      CHECK(etl::wstring<20>(STR("Result true      ")) == to_string(true, str, Format().precision(6).width(10).left().boolalpha(true), true));
    }

    //*************************************************************************
    TEST(test_pointer_no_append)
    {
      etl::wstring<20> str;

      static const volatile int cvi = 0;

      std::wostringstream oss;
      oss.width(10);
      oss.fill(STR('0'));
      oss << std::hex << std::right << uintptr_t(&cvi);
      std::wstring temp(oss.str());
      etl::wstring<20> compare(temp.begin(), temp.end());

      to_string(&cvi, str, Format().hex().width(10).right().fill(STR('0')));
      CHECK(compare == str);

      oss.clear();
      oss.str(STR(""));
      oss.width(10);
      oss.fill(STR('0'));
      oss << std::hex << std::left << uintptr_t(&cvi);
      temp = oss.str();
      compare.assign(temp.begin(), temp.end());

      to_string(&cvi, str, Format().hex().width(10).left().fill(STR('0')));
      CHECK(compare == str);
    }

    //*************************************************************************
    TEST(test_pointer_append)
    {
      etl::wstring<20> str;

      static const volatile int cvi = 0;

      std::wostringstream oss;
      oss.width(10);
      oss.fill(STR('0'));
      oss << std::hex << std::right << uintptr_t(&cvi);
      std::wstring temp(STR("Result "));
      temp.append(oss.str());
      etl::wstring<20> compare(temp.begin(), temp.end());

      str.assign(STR("Result "));
      to_string(&cvi, str, Format().hex().width(10).right().fill(STR('0')), true);
      CHECK(compare == str);

      oss.clear();
      oss.str(STR(""));
      oss.width(10);
      oss.fill(STR('0'));
      oss << std::hex << std::left << uintptr_t(&cvi);
      temp = STR("Result ");
      temp.append(oss.str());
      compare.assign(temp.begin(), temp.end());

      str.assign(STR("Result "));
      to_string(&cvi, str, Format().hex().width(10).left().fill(STR('0')), true);
      CHECK(compare == str);
    }

    //*************************************************************************
    TEST(test_integer_denominator_default_format)
    {
      etl::wstring<20> result;
      int value = -1234567;

      etl::to_string(value, 6U, result);

      CHECK(etl::wstring<20>(STR("-1")) == result);
    }

    //*************************************************************************
    TEST(test_integer_denominator_huge_precision)
    {
      etl::wstring<20> result;
      int value = -1234560;

      Format format = Format().precision(100);

      etl::to_string(value, 6U, result, format);

      CHECK(etl::wstring<20>(STR("-1.234560")) == result);
    }

    //*************************************************************************
    TEST(test_integer_denominator_huge_precision_64bit)
    {
      etl::wstring<20> result;
      int64_t value = INT64_MIN;

      Format format = Format().precision(100);

      etl::to_string(value, 12U, result, format);

      CHECK(etl::wstring<20>(STR("-9223372.036854775808")) == result);
    }

    //*************************************************************************
    TEST(test_integer_denominator_zero_fractional)
    {
      etl::wstring<20> result_i;
      int value_i = -1000000;

      etl::wstring<20> result_d;
      double value_d = -1.000000;

      Format format = Format().precision(4);

      etl::to_string(value_i, 6U, result_i, format);
      etl::to_string(value_d, result_d, format);

      CHECK(etl::wstring<20>(STR("-1.0000")) == result_i);
      CHECK(result_d == result_i);
    }

    //*************************************************************************
    TEST(test_integer_denominator_zero_value)
    {
      etl::wstring<20> result_i;
      int value_i = 0;

      etl::wstring<20> result_d;
      double value_d = -0.000000;

      Format format = Format().precision(4);

      etl::to_string(value_i, 6U, result_i, format);
      etl::to_string(value_d, result_d, format);

      CHECK(etl::wstring<20>(STR("0.0000")) == result_i);
      CHECK(result_d == result_i);
    }

    //*************************************************************************
    TEST(test_integer_denominator_zero_integral_small_fractional)
    {
      etl::wstring<20> result_i;
      int value_i = -400;

      etl::wstring<20> result_d;
      double value_d = -0.000400;

      Format format = Format().precision(4);

      etl::to_string(value_i, 6U, result_i, format);
      etl::to_string(value_d, result_d, format);

      CHECK(etl::wstring<20>(STR("-0.0004")) == result_i);
      CHECK(result_d == result_i);
    }

    //*************************************************************************
    TEST(test_integer_denominator_small_fractional)
    {
      etl::wstring<20> result_i;
      int value_i = -123000400;

      etl::wstring<20> result_d;
      double value_d = -123.000400;

      Format format = Format().precision(4);

      etl::to_string(value_i, 6U, result_i, format);
      etl::to_string(value_d, result_d, format);

      CHECK(etl::wstring<20>(STR("-123.0004")) == result_i);
      CHECK(result_d == result_i);
    }

    //*************************************************************************
    TEST(test_integer_denominator_very_small_fractional)
    {
      etl::wstring<20> result_i;
      int value_i = -123000004;

      etl::wstring<20> result_d;
      double value_d = -123.000004;

      Format format = Format().precision(4);

      etl::to_string(value_i, 6U, result_i, format);
      etl::to_string(value_d, result_d, format);

      CHECK(etl::wstring<20>(STR("-123.0000")) == result_i);
      CHECK(result_d == result_i);
    }

    //*************************************************************************
    TEST(test_integer_denominator_very_small_fractional_rounded_up)
    {
      etl::wstring<20> result_i;
      int value_i = -123000050;

      etl::wstring<20> result_d;
      double value_d = -123.000050;

      Format format = Format().precision(4);

      etl::to_string(value_i, 6U, result_i, format);
      etl::to_string(value_d, result_d, format);

      CHECK(etl::wstring<20>(STR("-123.0001")) == result_i);
      CHECK(result_d == result_i);
    }

    //*************************************************************************
    TEST(test_integer_denominator_shorter_width)
    {
      etl::wstring<20> result_i;
      int value_i = -123456780;

      etl::wstring<20> result_d;
      double value_d = -123.456780;

      Format format = Format().precision(4).width(6).right();

      etl::to_string(value_i, 6U, result_i, format);
      etl::to_string(value_d, result_d, format);

      CHECK(etl::wstring<20>(STR("-123.4568")) == result_i);
      CHECK(result_d == result_i);
    }

    //*************************************************************************
    TEST(test_integer_denominator_larger_width)
    {
      etl::wstring<20> result_i;
      int value_i = -123456780;

      etl::wstring<20> result_d;
      double value_d = -123.456780;

      Format format = Format().precision(4).width(15).right();

      etl::to_string(value_i, 6U, result_i, format);
      etl::to_string(value_d, result_d, format);

      CHECK(etl::wstring<20>(STR("      -123.4568")) == result_i);
      CHECK(result_d == result_i);
    }

    //*************************************************************************
    TEST(test_integer_denominator_positive_rollover)
    {
      etl::wstring<20> result_i;
      int value_i = 123999990;

      etl::wstring<20> result_d;
      double value_d = 123.999990;

      Format format = Format().precision(4).right();

      etl::to_string(value_i, 6U, result_i, format);
      etl::to_string(value_d, result_d, format);

      CHECK(etl::wstring<20>(STR("124.0000")) == result_i);
      CHECK(result_d == result_i);
    }

    //*************************************************************************
    TEST(test_integer_denominator_negative_rollover)
    {
      etl::wstring<20> result_i;
      int value_i = -123999990;

      etl::wstring<20> result_d;
      double value_d = -123.999990;

      Format format = Format().precision(4).right();

      etl::to_string(value_i, 6U, result_i, format);
      etl::to_string(value_d, result_d, format);

      CHECK(etl::wstring<20>(STR("-124.0000")) == result_i);
      CHECK(result_d == result_i);
    }
  };
}

