/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl
https://www.etlcpp.com

Copyright(c) 2018 John Wellbelove

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

#include "etl/limits.h"
#include <limits>

namespace
{
  SUITE(test_limits)
  {
    //*************************************************************************
    TEST(test_bool)
    {
      typedef etl::numeric_limits<bool> ETL_NL;
      typedef std::numeric_limits<bool> STD_NL;

      CHECK_EQUAL(STD_NL::denorm_min(),      ETL_NL::denorm_min());
      CHECK_EQUAL(STD_NL::digits,            ETL_NL::digits);
      CHECK_EQUAL(STD_NL::digits10,          ETL_NL::digits10);
      CHECK_EQUAL(STD_NL::epsilon(),         ETL_NL::epsilon());
      CHECK_EQUAL(int(STD_NL::has_denorm),   int(ETL_NL::has_denorm));
      CHECK_EQUAL(STD_NL::has_denorm_loss,   ETL_NL::has_denorm_loss);
      CHECK_EQUAL(STD_NL::has_infinity,      ETL_NL::has_infinity);
      CHECK_EQUAL(STD_NL::has_quiet_NaN,     ETL_NL::has_quiet_NaN);
      CHECK_EQUAL(STD_NL::has_signaling_NaN, ETL_NL::has_signaling_NaN);
      CHECK_EQUAL(STD_NL::infinity(),        ETL_NL::infinity());
      CHECK_EQUAL(STD_NL::is_bounded,        ETL_NL::is_bounded);
      CHECK_EQUAL(STD_NL::is_exact,          ETL_NL::is_exact);
      CHECK_EQUAL(STD_NL::is_iec559,         ETL_NL::is_iec559);
      CHECK_EQUAL(STD_NL::is_integer,        ETL_NL::is_integer);
      CHECK_EQUAL(STD_NL::is_modulo,         ETL_NL::is_modulo);
      CHECK_EQUAL(STD_NL::is_signed,         ETL_NL::is_signed);
      CHECK_EQUAL(STD_NL::is_specialized,    ETL_NL::is_specialized);
      CHECK_EQUAL(STD_NL::lowest(),          ETL_NL::lowest());
      CHECK_EQUAL(STD_NL::max(),             ETL_NL::max());
      CHECK_EQUAL(STD_NL::max_digits10,      ETL_NL::max_digits10);
      CHECK_EQUAL(STD_NL::max_exponent,      ETL_NL::max_exponent);
      CHECK_EQUAL(STD_NL::max_exponent10,    ETL_NL::max_exponent10);
      CHECK_EQUAL(STD_NL::min(),             ETL_NL::min());
      CHECK_EQUAL(STD_NL::min_exponent,      ETL_NL::min_exponent);
      CHECK_EQUAL(STD_NL::min_exponent10,    ETL_NL::min_exponent10);
      CHECK_EQUAL(STD_NL::quiet_NaN(),       ETL_NL::quiet_NaN());
      CHECK_EQUAL(STD_NL::radix,             ETL_NL::radix);
      CHECK_EQUAL(STD_NL::round_error(),     ETL_NL::round_error());
      CHECK_EQUAL(int(STD_NL::round_style),  int(ETL_NL::round_style));
      CHECK_EQUAL(STD_NL::signaling_NaN(),   ETL_NL::signaling_NaN());
      CHECK_EQUAL(STD_NL::tinyness_before,   ETL_NL::tinyness_before);
    }

    //*************************************************************************
    TEST(test_char)
    {
      typedef etl::numeric_limits<char> ETL_NL;
      typedef std::numeric_limits<char> STD_NL;

      CHECK_EQUAL(STD_NL::denorm_min(),      ETL_NL::denorm_min());
      CHECK_EQUAL(STD_NL::digits,            ETL_NL::digits);
      CHECK_EQUAL(STD_NL::digits10,          ETL_NL::digits10);
      CHECK_EQUAL(STD_NL::epsilon(),         ETL_NL::epsilon());
      CHECK_EQUAL(int(STD_NL::has_denorm),   int(ETL_NL::has_denorm));
      CHECK_EQUAL(STD_NL::has_denorm_loss,   ETL_NL::has_denorm_loss);
      CHECK_EQUAL(STD_NL::has_infinity,      ETL_NL::has_infinity);
      CHECK_EQUAL(STD_NL::has_quiet_NaN,     ETL_NL::has_quiet_NaN);
      CHECK_EQUAL(STD_NL::has_signaling_NaN, ETL_NL::has_signaling_NaN);
      CHECK_EQUAL(STD_NL::infinity(),        ETL_NL::infinity());
      CHECK_EQUAL(STD_NL::is_bounded,        ETL_NL::is_bounded);
      CHECK_EQUAL(STD_NL::is_exact,          ETL_NL::is_exact);
      CHECK_EQUAL(STD_NL::is_iec559,         ETL_NL::is_iec559);
      CHECK_EQUAL(STD_NL::is_integer,        ETL_NL::is_integer);
      CHECK_EQUAL(STD_NL::is_modulo,         ETL_NL::is_modulo);
      CHECK_EQUAL(STD_NL::is_signed,         ETL_NL::is_signed);
      CHECK_EQUAL(STD_NL::is_specialized,    ETL_NL::is_specialized);
      CHECK_EQUAL(STD_NL::lowest(),          ETL_NL::lowest());
      CHECK_EQUAL(STD_NL::max(),             ETL_NL::max());
      CHECK_EQUAL(STD_NL::max_digits10,      ETL_NL::max_digits10);
      CHECK_EQUAL(STD_NL::max_exponent,      ETL_NL::max_exponent);
      CHECK_EQUAL(STD_NL::max_exponent10,    ETL_NL::max_exponent10);
      CHECK_EQUAL(STD_NL::min(),             ETL_NL::min());
      CHECK_EQUAL(STD_NL::min_exponent,      ETL_NL::min_exponent);
      CHECK_EQUAL(STD_NL::min_exponent10,    ETL_NL::min_exponent10);
      CHECK_EQUAL(STD_NL::quiet_NaN(),       ETL_NL::quiet_NaN());
      CHECK_EQUAL(STD_NL::radix,             ETL_NL::radix);
      CHECK_EQUAL(STD_NL::round_error(),     ETL_NL::round_error());
      CHECK_EQUAL(int(STD_NL::round_style),  int(ETL_NL::round_style));
      CHECK_EQUAL(STD_NL::signaling_NaN(),   ETL_NL::signaling_NaN());
      CHECK_EQUAL(STD_NL::tinyness_before,   ETL_NL::tinyness_before);
    }

    //*************************************************************************
    TEST(test_unsigned_char)
    {
      typedef etl::numeric_limits<unsigned char> ETL_NL;
      typedef std::numeric_limits<unsigned char> STD_NL;

      CHECK_EQUAL(STD_NL::denorm_min(),      ETL_NL::denorm_min());
      CHECK_EQUAL(STD_NL::digits,            ETL_NL::digits);
      CHECK_EQUAL(STD_NL::digits10,          ETL_NL::digits10);
      CHECK_EQUAL(STD_NL::epsilon(),         ETL_NL::epsilon());
      CHECK_EQUAL(int(STD_NL::has_denorm),   int(ETL_NL::has_denorm));
      CHECK_EQUAL(STD_NL::has_denorm_loss,   ETL_NL::has_denorm_loss);
      CHECK_EQUAL(STD_NL::has_infinity,      ETL_NL::has_infinity);
      CHECK_EQUAL(STD_NL::has_quiet_NaN,     ETL_NL::has_quiet_NaN);
      CHECK_EQUAL(STD_NL::has_signaling_NaN, ETL_NL::has_signaling_NaN);
      CHECK_EQUAL(STD_NL::infinity(),        ETL_NL::infinity());
      CHECK_EQUAL(STD_NL::is_bounded,        ETL_NL::is_bounded);
      CHECK_EQUAL(STD_NL::is_exact,          ETL_NL::is_exact);
      CHECK_EQUAL(STD_NL::is_iec559,         ETL_NL::is_iec559);
      CHECK_EQUAL(STD_NL::is_integer,        ETL_NL::is_integer);
      CHECK_EQUAL(STD_NL::is_modulo,         ETL_NL::is_modulo);
      CHECK_EQUAL(STD_NL::is_signed,         ETL_NL::is_signed);
      CHECK_EQUAL(STD_NL::is_specialized,    ETL_NL::is_specialized);
      CHECK_EQUAL(STD_NL::lowest(),          ETL_NL::lowest());
      CHECK_EQUAL(STD_NL::max(),             ETL_NL::max());
      CHECK_EQUAL(STD_NL::max_digits10,      ETL_NL::max_digits10);
      CHECK_EQUAL(STD_NL::max_exponent,      ETL_NL::max_exponent);
      CHECK_EQUAL(STD_NL::max_exponent10,    ETL_NL::max_exponent10);
      CHECK_EQUAL(STD_NL::min(),             ETL_NL::min());
      CHECK_EQUAL(STD_NL::min_exponent,      ETL_NL::min_exponent);
      CHECK_EQUAL(STD_NL::min_exponent10,    ETL_NL::min_exponent10);
      CHECK_EQUAL(STD_NL::quiet_NaN(),       ETL_NL::quiet_NaN());
      CHECK_EQUAL(STD_NL::radix,             ETL_NL::radix);
      CHECK_EQUAL(STD_NL::round_error(),     ETL_NL::round_error());
      CHECK_EQUAL(int(STD_NL::round_style),  int(ETL_NL::round_style));
      CHECK_EQUAL(STD_NL::signaling_NaN(),   ETL_NL::signaling_NaN());
      CHECK_EQUAL(STD_NL::tinyness_before,   ETL_NL::tinyness_before);
    }

    //*************************************************************************
    TEST(test_signed_char)
    {
      typedef etl::numeric_limits<signed char> ETL_NL;
      typedef std::numeric_limits<signed char> STD_NL;

      CHECK_EQUAL(STD_NL::denorm_min(),      ETL_NL::denorm_min());
      CHECK_EQUAL(STD_NL::digits,            ETL_NL::digits);
      CHECK_EQUAL(STD_NL::digits10,          ETL_NL::digits10);
      CHECK_EQUAL(STD_NL::epsilon(),         ETL_NL::epsilon());
      CHECK_EQUAL(int(STD_NL::has_denorm),   int(ETL_NL::has_denorm));
      CHECK_EQUAL(STD_NL::has_denorm_loss,   ETL_NL::has_denorm_loss);
      CHECK_EQUAL(STD_NL::has_infinity,      ETL_NL::has_infinity);
      CHECK_EQUAL(STD_NL::has_quiet_NaN,     ETL_NL::has_quiet_NaN);
      CHECK_EQUAL(STD_NL::has_signaling_NaN, ETL_NL::has_signaling_NaN);
      CHECK_EQUAL(STD_NL::infinity(),        ETL_NL::infinity());
      CHECK_EQUAL(STD_NL::is_bounded,        ETL_NL::is_bounded);
      CHECK_EQUAL(STD_NL::is_exact,          ETL_NL::is_exact);
      CHECK_EQUAL(STD_NL::is_iec559,         ETL_NL::is_iec559);
      CHECK_EQUAL(STD_NL::is_integer,        ETL_NL::is_integer);
      CHECK_EQUAL(STD_NL::is_modulo,         ETL_NL::is_modulo);
      CHECK_EQUAL(STD_NL::is_signed,         ETL_NL::is_signed);
      CHECK_EQUAL(STD_NL::is_specialized,    ETL_NL::is_specialized);
      CHECK_EQUAL(STD_NL::lowest(),          ETL_NL::lowest());
      CHECK_EQUAL(STD_NL::max(),             ETL_NL::max());
      CHECK_EQUAL(STD_NL::max_digits10,      ETL_NL::max_digits10);
      CHECK_EQUAL(STD_NL::max_exponent,      ETL_NL::max_exponent);
      CHECK_EQUAL(STD_NL::max_exponent10,    ETL_NL::max_exponent10);
      CHECK_EQUAL(STD_NL::min(),             ETL_NL::min());
      CHECK_EQUAL(STD_NL::min_exponent,      ETL_NL::min_exponent);
      CHECK_EQUAL(STD_NL::min_exponent10,    ETL_NL::min_exponent10);
      CHECK_EQUAL(STD_NL::quiet_NaN(),       ETL_NL::quiet_NaN());
      CHECK_EQUAL(STD_NL::radix,             ETL_NL::radix);
      CHECK_EQUAL(STD_NL::round_error(),     ETL_NL::round_error());
      CHECK_EQUAL(int(STD_NL::round_style),  int(ETL_NL::round_style));
      CHECK_EQUAL(STD_NL::signaling_NaN(),   ETL_NL::signaling_NaN());
      CHECK_EQUAL(STD_NL::tinyness_before,   ETL_NL::tinyness_before);
    }

    //*************************************************************************
    TEST(test_char16_t)
    {
      typedef etl::numeric_limits<char16_t> ETL_NL;
      typedef std::numeric_limits<char16_t> STD_NL;

      CHECK_EQUAL(STD_NL::denorm_min(),      ETL_NL::denorm_min());
      CHECK_EQUAL(STD_NL::digits,            ETL_NL::digits);
      CHECK_EQUAL(STD_NL::digits10,          ETL_NL::digits10);
      CHECK_EQUAL(STD_NL::epsilon(),         ETL_NL::epsilon());
      CHECK_EQUAL(int(STD_NL::has_denorm),   int(ETL_NL::has_denorm));
      CHECK_EQUAL(STD_NL::has_denorm_loss,   ETL_NL::has_denorm_loss);
      CHECK_EQUAL(STD_NL::has_infinity,      ETL_NL::has_infinity);
      CHECK_EQUAL(STD_NL::has_quiet_NaN,     ETL_NL::has_quiet_NaN);
      CHECK_EQUAL(STD_NL::has_signaling_NaN, ETL_NL::has_signaling_NaN);
      CHECK_EQUAL(STD_NL::infinity(),        ETL_NL::infinity());
      CHECK_EQUAL(STD_NL::is_bounded,        ETL_NL::is_bounded);
      CHECK_EQUAL(STD_NL::is_exact,          ETL_NL::is_exact);
      CHECK_EQUAL(STD_NL::is_iec559,         ETL_NL::is_iec559);
      CHECK_EQUAL(STD_NL::is_integer,        ETL_NL::is_integer);
      CHECK_EQUAL(STD_NL::is_modulo,         ETL_NL::is_modulo);
      CHECK_EQUAL(STD_NL::is_signed,         ETL_NL::is_signed);
      CHECK_EQUAL(STD_NL::is_specialized,    ETL_NL::is_specialized);
      CHECK_EQUAL(STD_NL::lowest(),          ETL_NL::lowest());
      CHECK_EQUAL(STD_NL::max(),             ETL_NL::max());
      CHECK_EQUAL(STD_NL::max_digits10,      ETL_NL::max_digits10);
      CHECK_EQUAL(STD_NL::max_exponent,      ETL_NL::max_exponent);
      CHECK_EQUAL(STD_NL::max_exponent10,    ETL_NL::max_exponent10);
      CHECK_EQUAL(STD_NL::min(),             ETL_NL::min());
      CHECK_EQUAL(STD_NL::min_exponent,      ETL_NL::min_exponent);
      CHECK_EQUAL(STD_NL::min_exponent10,    ETL_NL::min_exponent10);
      CHECK_EQUAL(STD_NL::quiet_NaN(),       ETL_NL::quiet_NaN());
      CHECK_EQUAL(STD_NL::radix,             ETL_NL::radix);
      CHECK_EQUAL(STD_NL::round_error(),     ETL_NL::round_error());
      CHECK_EQUAL(int(STD_NL::round_style),  int(ETL_NL::round_style));
      CHECK_EQUAL(STD_NL::signaling_NaN(),   ETL_NL::signaling_NaN());
      CHECK_EQUAL(STD_NL::tinyness_before,   ETL_NL::tinyness_before);
    }

    //*************************************************************************
    TEST(test_char32_t)
    {
      typedef etl::numeric_limits<char32_t> ETL_NL;
      typedef std::numeric_limits<char32_t> STD_NL;

      CHECK_EQUAL(STD_NL::denorm_min(),      ETL_NL::denorm_min());
      CHECK_EQUAL(STD_NL::digits,            ETL_NL::digits);
      CHECK_EQUAL(STD_NL::digits10,          ETL_NL::digits10);
      CHECK_EQUAL(STD_NL::epsilon(),         ETL_NL::epsilon());
      CHECK_EQUAL(int(STD_NL::has_denorm),   int(ETL_NL::has_denorm));
      CHECK_EQUAL(STD_NL::has_denorm_loss,   ETL_NL::has_denorm_loss);
      CHECK_EQUAL(STD_NL::has_infinity,      ETL_NL::has_infinity);
      CHECK_EQUAL(STD_NL::has_quiet_NaN,     ETL_NL::has_quiet_NaN);
      CHECK_EQUAL(STD_NL::has_signaling_NaN, ETL_NL::has_signaling_NaN);
      CHECK_EQUAL(STD_NL::infinity(),        ETL_NL::infinity());
      CHECK_EQUAL(STD_NL::is_bounded,        ETL_NL::is_bounded);
      CHECK_EQUAL(STD_NL::is_exact,          ETL_NL::is_exact);
      CHECK_EQUAL(STD_NL::is_iec559,         ETL_NL::is_iec559);
      CHECK_EQUAL(STD_NL::is_integer,        ETL_NL::is_integer);
      CHECK_EQUAL(STD_NL::is_modulo,         ETL_NL::is_modulo);
      CHECK_EQUAL(STD_NL::is_signed,         ETL_NL::is_signed);
      CHECK_EQUAL(STD_NL::is_specialized,    ETL_NL::is_specialized);
      CHECK_EQUAL(STD_NL::lowest(),          ETL_NL::lowest());
      CHECK_EQUAL(STD_NL::max(),             ETL_NL::max());
      CHECK_EQUAL(STD_NL::max_digits10,      ETL_NL::max_digits10);
      CHECK_EQUAL(STD_NL::max_exponent,      ETL_NL::max_exponent);
      CHECK_EQUAL(STD_NL::max_exponent10,    ETL_NL::max_exponent10);
      CHECK_EQUAL(STD_NL::min(),             ETL_NL::min());
      CHECK_EQUAL(STD_NL::min_exponent,      ETL_NL::min_exponent);
      CHECK_EQUAL(STD_NL::min_exponent10,    ETL_NL::min_exponent10);
      CHECK_EQUAL(STD_NL::quiet_NaN(),       ETL_NL::quiet_NaN());
      CHECK_EQUAL(STD_NL::radix,             ETL_NL::radix);
      CHECK_EQUAL(STD_NL::round_error(),     ETL_NL::round_error());
      CHECK_EQUAL(int(STD_NL::round_style),  int(ETL_NL::round_style));
      CHECK_EQUAL(STD_NL::signaling_NaN(),   ETL_NL::signaling_NaN());
      CHECK_EQUAL(STD_NL::tinyness_before,   ETL_NL::tinyness_before);
    }

    //*************************************************************************
    TEST(test_wchar_t)
    {
      typedef etl::numeric_limits<wchar_t> ETL_NL;
      typedef std::numeric_limits<wchar_t> STD_NL;

      CHECK_EQUAL(STD_NL::denorm_min(),      ETL_NL::denorm_min());
      CHECK_EQUAL(STD_NL::digits,            ETL_NL::digits);
      CHECK_EQUAL(STD_NL::digits10,          ETL_NL::digits10);
      CHECK_EQUAL(STD_NL::epsilon(),         ETL_NL::epsilon());
      CHECK_EQUAL(int(STD_NL::has_denorm),   int(ETL_NL::has_denorm));
      CHECK_EQUAL(STD_NL::has_denorm_loss,   ETL_NL::has_denorm_loss);
      CHECK_EQUAL(STD_NL::has_infinity,      ETL_NL::has_infinity);
      CHECK_EQUAL(STD_NL::has_quiet_NaN,     ETL_NL::has_quiet_NaN);
      CHECK_EQUAL(STD_NL::has_signaling_NaN, ETL_NL::has_signaling_NaN);
      CHECK_EQUAL(STD_NL::infinity(),        ETL_NL::infinity());
      CHECK_EQUAL(STD_NL::is_bounded,        ETL_NL::is_bounded);
      CHECK_EQUAL(STD_NL::is_exact,          ETL_NL::is_exact);
      CHECK_EQUAL(STD_NL::is_iec559,         ETL_NL::is_iec559);
      CHECK_EQUAL(STD_NL::is_integer,        ETL_NL::is_integer);
      CHECK_EQUAL(STD_NL::is_modulo,         ETL_NL::is_modulo);
      CHECK_EQUAL(STD_NL::is_signed,         ETL_NL::is_signed);
      CHECK_EQUAL(STD_NL::is_specialized,    ETL_NL::is_specialized);
      CHECK_EQUAL(STD_NL::lowest(),          ETL_NL::lowest());
      CHECK_EQUAL(STD_NL::max(),             ETL_NL::max());
      CHECK_EQUAL(STD_NL::max_digits10,      ETL_NL::max_digits10);
      CHECK_EQUAL(STD_NL::max_exponent,      ETL_NL::max_exponent);
      CHECK_EQUAL(STD_NL::max_exponent10,    ETL_NL::max_exponent10);
      CHECK_EQUAL(STD_NL::min(),             ETL_NL::min());
      CHECK_EQUAL(STD_NL::min_exponent,      ETL_NL::min_exponent);
      CHECK_EQUAL(STD_NL::min_exponent10,    ETL_NL::min_exponent10);
      CHECK_EQUAL(STD_NL::quiet_NaN(),       ETL_NL::quiet_NaN());
      CHECK_EQUAL(STD_NL::radix,             ETL_NL::radix);
      CHECK_EQUAL(STD_NL::round_error(),     ETL_NL::round_error());
      CHECK_EQUAL(int(STD_NL::round_style),  int(ETL_NL::round_style));
      CHECK_EQUAL(STD_NL::signaling_NaN(),   ETL_NL::signaling_NaN());
      CHECK_EQUAL(STD_NL::tinyness_before,   ETL_NL::tinyness_before);
    }

    //*************************************************************************
    TEST(test_short)
    {
      typedef etl::numeric_limits<short> ETL_NL;
      typedef std::numeric_limits<short> STD_NL;

      CHECK_EQUAL(STD_NL::denorm_min(),      ETL_NL::denorm_min());
      CHECK_EQUAL(STD_NL::digits,            ETL_NL::digits);
      CHECK_EQUAL(STD_NL::digits10,          ETL_NL::digits10);
      CHECK_EQUAL(STD_NL::epsilon(),         ETL_NL::epsilon());
      CHECK_EQUAL(int(STD_NL::has_denorm),   int(ETL_NL::has_denorm));
      CHECK_EQUAL(STD_NL::has_denorm_loss,   ETL_NL::has_denorm_loss);
      CHECK_EQUAL(STD_NL::has_infinity,      ETL_NL::has_infinity);
      CHECK_EQUAL(STD_NL::has_quiet_NaN,     ETL_NL::has_quiet_NaN);
      CHECK_EQUAL(STD_NL::has_signaling_NaN, ETL_NL::has_signaling_NaN);
      CHECK_EQUAL(STD_NL::infinity(),        ETL_NL::infinity());
      CHECK_EQUAL(STD_NL::is_bounded,        ETL_NL::is_bounded);
      CHECK_EQUAL(STD_NL::is_exact,          ETL_NL::is_exact);
      CHECK_EQUAL(STD_NL::is_iec559,         ETL_NL::is_iec559);
      CHECK_EQUAL(STD_NL::is_integer,        ETL_NL::is_integer);
      CHECK_EQUAL(STD_NL::is_modulo,         ETL_NL::is_modulo);
      CHECK_EQUAL(STD_NL::is_signed,         ETL_NL::is_signed);
      CHECK_EQUAL(STD_NL::is_specialized,    ETL_NL::is_specialized);
      CHECK_EQUAL(STD_NL::lowest(),          ETL_NL::lowest());
      CHECK_EQUAL(STD_NL::max(),             ETL_NL::max());
      CHECK_EQUAL(STD_NL::max_digits10,      ETL_NL::max_digits10);
      CHECK_EQUAL(STD_NL::max_exponent,      ETL_NL::max_exponent);
      CHECK_EQUAL(STD_NL::max_exponent10,    ETL_NL::max_exponent10);
      CHECK_EQUAL(STD_NL::min(),             ETL_NL::min());
      CHECK_EQUAL(STD_NL::min_exponent,      ETL_NL::min_exponent);
      CHECK_EQUAL(STD_NL::min_exponent10,    ETL_NL::min_exponent10);
      CHECK_EQUAL(STD_NL::quiet_NaN(),       ETL_NL::quiet_NaN());
      CHECK_EQUAL(STD_NL::radix,             ETL_NL::radix);
      CHECK_EQUAL(STD_NL::round_error(),     ETL_NL::round_error());
      CHECK_EQUAL(int(STD_NL::round_style),  int(ETL_NL::round_style));
      CHECK_EQUAL(STD_NL::signaling_NaN(),   ETL_NL::signaling_NaN());
      CHECK_EQUAL(STD_NL::tinyness_before,   ETL_NL::tinyness_before);
    }

    //*************************************************************************
    TEST(test_unsigned_short)
    {
      typedef etl::numeric_limits<unsigned short> ETL_NL;
      typedef std::numeric_limits<unsigned short> STD_NL;

      CHECK_EQUAL(STD_NL::denorm_min(),      ETL_NL::denorm_min());
      CHECK_EQUAL(STD_NL::digits,            ETL_NL::digits);
      CHECK_EQUAL(STD_NL::digits10,          ETL_NL::digits10);
      CHECK_EQUAL(STD_NL::epsilon(),         ETL_NL::epsilon());
      CHECK_EQUAL(int(STD_NL::has_denorm),   int(ETL_NL::has_denorm));
      CHECK_EQUAL(STD_NL::has_denorm_loss,   ETL_NL::has_denorm_loss);
      CHECK_EQUAL(STD_NL::has_infinity,      ETL_NL::has_infinity);
      CHECK_EQUAL(STD_NL::has_quiet_NaN,     ETL_NL::has_quiet_NaN);
      CHECK_EQUAL(STD_NL::has_signaling_NaN, ETL_NL::has_signaling_NaN);
      CHECK_EQUAL(STD_NL::infinity(),        ETL_NL::infinity());
      CHECK_EQUAL(STD_NL::is_bounded,        ETL_NL::is_bounded);
      CHECK_EQUAL(STD_NL::is_exact,          ETL_NL::is_exact);
      CHECK_EQUAL(STD_NL::is_iec559,         ETL_NL::is_iec559);
      CHECK_EQUAL(STD_NL::is_integer,        ETL_NL::is_integer);
      CHECK_EQUAL(STD_NL::is_modulo,         ETL_NL::is_modulo);
      CHECK_EQUAL(STD_NL::is_signed,         ETL_NL::is_signed);
      CHECK_EQUAL(STD_NL::is_specialized,    ETL_NL::is_specialized);
      CHECK_EQUAL(STD_NL::lowest(),          ETL_NL::lowest());
      CHECK_EQUAL(STD_NL::max(),             ETL_NL::max());
      CHECK_EQUAL(STD_NL::max_digits10,      ETL_NL::max_digits10);
      CHECK_EQUAL(STD_NL::max_exponent,      ETL_NL::max_exponent);
      CHECK_EQUAL(STD_NL::max_exponent10,    ETL_NL::max_exponent10);
      CHECK_EQUAL(STD_NL::min(),             ETL_NL::min());
      CHECK_EQUAL(STD_NL::min_exponent,      ETL_NL::min_exponent);
      CHECK_EQUAL(STD_NL::min_exponent10,    ETL_NL::min_exponent10);
      CHECK_EQUAL(STD_NL::quiet_NaN(),       ETL_NL::quiet_NaN());
      CHECK_EQUAL(STD_NL::radix,             ETL_NL::radix);
      CHECK_EQUAL(STD_NL::round_error(),     ETL_NL::round_error());
      CHECK_EQUAL(int(STD_NL::round_style),  int(ETL_NL::round_style));
      CHECK_EQUAL(STD_NL::signaling_NaN(),   ETL_NL::signaling_NaN());
      CHECK_EQUAL(STD_NL::tinyness_before,   ETL_NL::tinyness_before);
    }

    //*************************************************************************
    TEST(test_int)
    {
      typedef etl::numeric_limits<int> ETL_NL;
      typedef std::numeric_limits<int> STD_NL;

      CHECK_EQUAL(STD_NL::denorm_min(),      ETL_NL::denorm_min());
      CHECK_EQUAL(STD_NL::digits,            ETL_NL::digits);
      CHECK_EQUAL(STD_NL::digits10,          ETL_NL::digits10);
      CHECK_EQUAL(STD_NL::epsilon(),         ETL_NL::epsilon());
      CHECK_EQUAL(int(STD_NL::has_denorm),   int(ETL_NL::has_denorm));
      CHECK_EQUAL(STD_NL::has_denorm_loss,   ETL_NL::has_denorm_loss);
      CHECK_EQUAL(STD_NL::has_infinity,      ETL_NL::has_infinity);
      CHECK_EQUAL(STD_NL::has_quiet_NaN,     ETL_NL::has_quiet_NaN);
      CHECK_EQUAL(STD_NL::has_signaling_NaN, ETL_NL::has_signaling_NaN);
      CHECK_EQUAL(STD_NL::infinity(),        ETL_NL::infinity());
      CHECK_EQUAL(STD_NL::is_bounded,        ETL_NL::is_bounded);
      CHECK_EQUAL(STD_NL::is_exact,          ETL_NL::is_exact);
      CHECK_EQUAL(STD_NL::is_iec559,         ETL_NL::is_iec559);
      CHECK_EQUAL(STD_NL::is_integer,        ETL_NL::is_integer);
      CHECK_EQUAL(STD_NL::is_modulo,         ETL_NL::is_modulo);
      CHECK_EQUAL(STD_NL::is_signed,         ETL_NL::is_signed);
      CHECK_EQUAL(STD_NL::is_specialized,    ETL_NL::is_specialized);
      CHECK_EQUAL(STD_NL::lowest(),          ETL_NL::lowest());
      CHECK_EQUAL(STD_NL::max(),             ETL_NL::max());
      CHECK_EQUAL(STD_NL::max_digits10,      ETL_NL::max_digits10);
      CHECK_EQUAL(STD_NL::max_exponent,      ETL_NL::max_exponent);
      CHECK_EQUAL(STD_NL::max_exponent10,    ETL_NL::max_exponent10);
      CHECK_EQUAL(STD_NL::min(),             ETL_NL::min());
      CHECK_EQUAL(STD_NL::min_exponent,      ETL_NL::min_exponent);
      CHECK_EQUAL(STD_NL::min_exponent10,    ETL_NL::min_exponent10);
      CHECK_EQUAL(STD_NL::quiet_NaN(),       ETL_NL::quiet_NaN());
      CHECK_EQUAL(STD_NL::radix,             ETL_NL::radix);
      CHECK_EQUAL(STD_NL::round_error(),     ETL_NL::round_error());
      CHECK_EQUAL(int(STD_NL::round_style),  int(ETL_NL::round_style));
      CHECK_EQUAL(STD_NL::signaling_NaN(),   ETL_NL::signaling_NaN());
      CHECK_EQUAL(STD_NL::tinyness_before,   ETL_NL::tinyness_before);
    }

    //*************************************************************************
    TEST(test_unsigned_int)
    {
      typedef etl::numeric_limits<unsigned int> ETL_NL;
      typedef std::numeric_limits<unsigned int> STD_NL;

      CHECK_EQUAL(STD_NL::denorm_min(),      ETL_NL::denorm_min());
      CHECK_EQUAL(STD_NL::digits,            ETL_NL::digits);
      CHECK_EQUAL(STD_NL::digits10,          ETL_NL::digits10);
      CHECK_EQUAL(STD_NL::epsilon(),         ETL_NL::epsilon());
      CHECK_EQUAL(int(STD_NL::has_denorm),   int(ETL_NL::has_denorm));
      CHECK_EQUAL(STD_NL::has_denorm_loss,   ETL_NL::has_denorm_loss);
      CHECK_EQUAL(STD_NL::has_infinity,      ETL_NL::has_infinity);
      CHECK_EQUAL(STD_NL::has_quiet_NaN,     ETL_NL::has_quiet_NaN);
      CHECK_EQUAL(STD_NL::has_signaling_NaN, ETL_NL::has_signaling_NaN);
      CHECK_EQUAL(STD_NL::infinity(),        ETL_NL::infinity());
      CHECK_EQUAL(STD_NL::is_bounded,        ETL_NL::is_bounded);
      CHECK_EQUAL(STD_NL::is_exact,          ETL_NL::is_exact);
      CHECK_EQUAL(STD_NL::is_iec559,         ETL_NL::is_iec559);
      CHECK_EQUAL(STD_NL::is_integer,        ETL_NL::is_integer);
      CHECK_EQUAL(STD_NL::is_modulo,         ETL_NL::is_modulo);
      CHECK_EQUAL(STD_NL::is_signed,         ETL_NL::is_signed);
      CHECK_EQUAL(STD_NL::is_specialized,    ETL_NL::is_specialized);
      CHECK_EQUAL(STD_NL::lowest(),          ETL_NL::lowest());
      CHECK_EQUAL(STD_NL::max(),             ETL_NL::max());
      CHECK_EQUAL(STD_NL::max_digits10,      ETL_NL::max_digits10);
      CHECK_EQUAL(STD_NL::max_exponent,      ETL_NL::max_exponent);
      CHECK_EQUAL(STD_NL::max_exponent10,    ETL_NL::max_exponent10);
      CHECK_EQUAL(STD_NL::min(),             ETL_NL::min());
      CHECK_EQUAL(STD_NL::min_exponent,      ETL_NL::min_exponent);
      CHECK_EQUAL(STD_NL::min_exponent10,    ETL_NL::min_exponent10);
      CHECK_EQUAL(STD_NL::quiet_NaN(),       ETL_NL::quiet_NaN());
      CHECK_EQUAL(STD_NL::radix,             ETL_NL::radix);
      CHECK_EQUAL(STD_NL::round_error(),     ETL_NL::round_error());
      CHECK_EQUAL(int(STD_NL::round_style),  int(ETL_NL::round_style));
      CHECK_EQUAL(STD_NL::signaling_NaN(),   ETL_NL::signaling_NaN());
      CHECK_EQUAL(STD_NL::tinyness_before,   ETL_NL::tinyness_before);
    }

    //*************************************************************************
    TEST(test_long)
    {
      typedef etl::numeric_limits<long> ETL_NL;
      typedef std::numeric_limits<long> STD_NL;

      CHECK_EQUAL(STD_NL::denorm_min(),      ETL_NL::denorm_min());
      CHECK_EQUAL(STD_NL::digits,            ETL_NL::digits);
      CHECK_EQUAL(STD_NL::digits10,          ETL_NL::digits10);
      CHECK_EQUAL(STD_NL::epsilon(),         ETL_NL::epsilon());
      CHECK_EQUAL(int(STD_NL::has_denorm),   int(ETL_NL::has_denorm));
      CHECK_EQUAL(STD_NL::has_denorm_loss,   ETL_NL::has_denorm_loss);
      CHECK_EQUAL(STD_NL::has_infinity,      ETL_NL::has_infinity);
      CHECK_EQUAL(STD_NL::has_quiet_NaN,     ETL_NL::has_quiet_NaN);
      CHECK_EQUAL(STD_NL::has_signaling_NaN, ETL_NL::has_signaling_NaN);
      CHECK_EQUAL(STD_NL::infinity(),        ETL_NL::infinity());
      CHECK_EQUAL(STD_NL::is_bounded,        ETL_NL::is_bounded);
      CHECK_EQUAL(STD_NL::is_exact,          ETL_NL::is_exact);
      CHECK_EQUAL(STD_NL::is_iec559,         ETL_NL::is_iec559);
      CHECK_EQUAL(STD_NL::is_integer,        ETL_NL::is_integer);
      CHECK_EQUAL(STD_NL::is_modulo,         ETL_NL::is_modulo);
      CHECK_EQUAL(STD_NL::is_signed,         ETL_NL::is_signed);
      CHECK_EQUAL(STD_NL::is_specialized,    ETL_NL::is_specialized);
      CHECK_EQUAL(STD_NL::lowest(),          ETL_NL::lowest());
      CHECK_EQUAL(STD_NL::max(),             ETL_NL::max());
      CHECK_EQUAL(STD_NL::max_digits10,      ETL_NL::max_digits10);
      CHECK_EQUAL(STD_NL::max_exponent,      ETL_NL::max_exponent);
      CHECK_EQUAL(STD_NL::max_exponent10,    ETL_NL::max_exponent10);
      CHECK_EQUAL(STD_NL::min(),             ETL_NL::min());
      CHECK_EQUAL(STD_NL::min_exponent,      ETL_NL::min_exponent);
      CHECK_EQUAL(STD_NL::min_exponent10,    ETL_NL::min_exponent10);
      CHECK_EQUAL(STD_NL::quiet_NaN(),       ETL_NL::quiet_NaN());
      CHECK_EQUAL(STD_NL::radix,             ETL_NL::radix);
      CHECK_EQUAL(STD_NL::round_error(),     ETL_NL::round_error());
      CHECK_EQUAL(int(STD_NL::round_style),  int(ETL_NL::round_style));
      CHECK_EQUAL(STD_NL::signaling_NaN(),   ETL_NL::signaling_NaN());
      CHECK_EQUAL(STD_NL::tinyness_before,   ETL_NL::tinyness_before);
    }

    //*************************************************************************
    TEST(test_unsigned_long)
    {
      typedef etl::numeric_limits<unsigned long> ETL_NL;
      typedef std::numeric_limits<unsigned long>    STD_NL;

      CHECK_EQUAL(STD_NL::denorm_min(),      ETL_NL::denorm_min());
      CHECK_EQUAL(STD_NL::digits,            ETL_NL::digits);
      CHECK_EQUAL(STD_NL::digits10,          ETL_NL::digits10);
      CHECK_EQUAL(STD_NL::epsilon(),         ETL_NL::epsilon());
      CHECK_EQUAL(int(STD_NL::has_denorm),   int(ETL_NL::has_denorm));
      CHECK_EQUAL(STD_NL::has_denorm_loss,   ETL_NL::has_denorm_loss);
      CHECK_EQUAL(STD_NL::has_infinity,      ETL_NL::has_infinity);
      CHECK_EQUAL(STD_NL::has_quiet_NaN,     ETL_NL::has_quiet_NaN);
      CHECK_EQUAL(STD_NL::has_signaling_NaN, ETL_NL::has_signaling_NaN);
      CHECK_EQUAL(STD_NL::infinity(),        ETL_NL::infinity());
      CHECK_EQUAL(STD_NL::is_bounded,        ETL_NL::is_bounded);
      CHECK_EQUAL(STD_NL::is_exact,          ETL_NL::is_exact);
      CHECK_EQUAL(STD_NL::is_iec559,         ETL_NL::is_iec559);
      CHECK_EQUAL(STD_NL::is_integer,        ETL_NL::is_integer);
      CHECK_EQUAL(STD_NL::is_modulo,         ETL_NL::is_modulo);
      CHECK_EQUAL(STD_NL::is_signed,         ETL_NL::is_signed);
      CHECK_EQUAL(STD_NL::is_specialized,    ETL_NL::is_specialized);
      CHECK_EQUAL(STD_NL::lowest(),          ETL_NL::lowest());
      CHECK_EQUAL(STD_NL::max(),             ETL_NL::max());
      CHECK_EQUAL(STD_NL::max_digits10,      ETL_NL::max_digits10);
      CHECK_EQUAL(STD_NL::max_exponent,      ETL_NL::max_exponent);
      CHECK_EQUAL(STD_NL::max_exponent10,    ETL_NL::max_exponent10);
      CHECK_EQUAL(STD_NL::min(),             ETL_NL::min());
      CHECK_EQUAL(STD_NL::min_exponent,      ETL_NL::min_exponent);
      CHECK_EQUAL(STD_NL::min_exponent10,    ETL_NL::min_exponent10);
      CHECK_EQUAL(STD_NL::quiet_NaN(),       ETL_NL::quiet_NaN());
      CHECK_EQUAL(STD_NL::radix,             ETL_NL::radix);
      CHECK_EQUAL(STD_NL::round_error(),     ETL_NL::round_error());
      CHECK_EQUAL(int(STD_NL::round_style),  int(ETL_NL::round_style));
      CHECK_EQUAL(STD_NL::signaling_NaN(),   ETL_NL::signaling_NaN());
      CHECK_EQUAL(STD_NL::tinyness_before,   ETL_NL::tinyness_before);
    }

    //*************************************************************************
    TEST(test_long_long)
    {
      typedef etl::numeric_limits<long long> ETL_NL;
      typedef std::numeric_limits<long long> STD_NL;

      CHECK_EQUAL(STD_NL::denorm_min(),      ETL_NL::denorm_min());
      CHECK_EQUAL(STD_NL::digits,            ETL_NL::digits);
      CHECK_EQUAL(STD_NL::digits10,          ETL_NL::digits10);
      CHECK_EQUAL(STD_NL::epsilon(),         ETL_NL::epsilon());
      CHECK_EQUAL(int(STD_NL::has_denorm),   int(ETL_NL::has_denorm));
      CHECK_EQUAL(STD_NL::has_denorm_loss,   ETL_NL::has_denorm_loss);
      CHECK_EQUAL(STD_NL::has_infinity,      ETL_NL::has_infinity);
      CHECK_EQUAL(STD_NL::has_quiet_NaN,     ETL_NL::has_quiet_NaN);
      CHECK_EQUAL(STD_NL::has_signaling_NaN, ETL_NL::has_signaling_NaN);
      CHECK_EQUAL(STD_NL::infinity(),        ETL_NL::infinity());
      CHECK_EQUAL(STD_NL::is_bounded,        ETL_NL::is_bounded);
      CHECK_EQUAL(STD_NL::is_exact,          ETL_NL::is_exact);
      CHECK_EQUAL(STD_NL::is_iec559,         ETL_NL::is_iec559);
      CHECK_EQUAL(STD_NL::is_integer,        ETL_NL::is_integer);
      CHECK_EQUAL(STD_NL::is_modulo,         ETL_NL::is_modulo);
      CHECK_EQUAL(STD_NL::is_signed,         ETL_NL::is_signed);
      CHECK_EQUAL(STD_NL::is_specialized,    ETL_NL::is_specialized);
      CHECK_EQUAL(STD_NL::lowest(),          ETL_NL::lowest());
      CHECK_EQUAL(STD_NL::max(),             ETL_NL::max());
      CHECK_EQUAL(STD_NL::max_digits10,      ETL_NL::max_digits10);
      CHECK_EQUAL(STD_NL::max_exponent,      ETL_NL::max_exponent);
      CHECK_EQUAL(STD_NL::max_exponent10,    ETL_NL::max_exponent10);
      CHECK_EQUAL(STD_NL::min(),             ETL_NL::min());
      CHECK_EQUAL(STD_NL::min_exponent,      ETL_NL::min_exponent);
      CHECK_EQUAL(STD_NL::min_exponent10,    ETL_NL::min_exponent10);
      CHECK_EQUAL(STD_NL::quiet_NaN(),       ETL_NL::quiet_NaN());
      CHECK_EQUAL(STD_NL::radix,             ETL_NL::radix);
      CHECK_EQUAL(STD_NL::round_error(),     ETL_NL::round_error());
      CHECK_EQUAL(int(STD_NL::round_style),  int(ETL_NL::round_style));
      CHECK_EQUAL(STD_NL::signaling_NaN(),   ETL_NL::signaling_NaN());
      CHECK_EQUAL(STD_NL::tinyness_before,   ETL_NL::tinyness_before);
    }

    //*************************************************************************
    TEST(test_unsigned_long_long)
    {
      typedef etl::numeric_limits<unsigned long long> ETL_NL;
      typedef std::numeric_limits<unsigned long long> STD_NL;

      CHECK_EQUAL(STD_NL::denorm_min(),      ETL_NL::denorm_min());
      CHECK_EQUAL(STD_NL::digits,            ETL_NL::digits);
      CHECK_EQUAL(STD_NL::digits10,          ETL_NL::digits10);
      CHECK_EQUAL(STD_NL::epsilon(),         ETL_NL::epsilon());
      CHECK_EQUAL(int(STD_NL::has_denorm),   int(ETL_NL::has_denorm));
      CHECK_EQUAL(STD_NL::has_denorm_loss,   ETL_NL::has_denorm_loss);
      CHECK_EQUAL(STD_NL::has_infinity,      ETL_NL::has_infinity);
      CHECK_EQUAL(STD_NL::has_quiet_NaN,     ETL_NL::has_quiet_NaN);
      CHECK_EQUAL(STD_NL::has_signaling_NaN, ETL_NL::has_signaling_NaN);
      CHECK_EQUAL(STD_NL::infinity(),        ETL_NL::infinity());
      CHECK_EQUAL(STD_NL::is_bounded,        ETL_NL::is_bounded);
      CHECK_EQUAL(STD_NL::is_exact,          ETL_NL::is_exact);
      CHECK_EQUAL(STD_NL::is_iec559,         ETL_NL::is_iec559);
      CHECK_EQUAL(STD_NL::is_integer,        ETL_NL::is_integer);
      CHECK_EQUAL(STD_NL::is_modulo,         ETL_NL::is_modulo);
      CHECK_EQUAL(STD_NL::is_signed,         ETL_NL::is_signed);
      CHECK_EQUAL(STD_NL::is_specialized,    ETL_NL::is_specialized);
      CHECK_EQUAL(STD_NL::lowest(),          ETL_NL::lowest());
      CHECK_EQUAL(STD_NL::max(),             ETL_NL::max());
      CHECK_EQUAL(STD_NL::max_digits10,      ETL_NL::max_digits10);
      CHECK_EQUAL(STD_NL::max_exponent,      ETL_NL::max_exponent);
      CHECK_EQUAL(STD_NL::max_exponent10,    ETL_NL::max_exponent10);
      CHECK_EQUAL(STD_NL::min(),             ETL_NL::min());
      CHECK_EQUAL(STD_NL::min_exponent,      ETL_NL::min_exponent);
      CHECK_EQUAL(STD_NL::min_exponent10,    ETL_NL::min_exponent10);
      CHECK_EQUAL(STD_NL::quiet_NaN(),       ETL_NL::quiet_NaN());
      CHECK_EQUAL(STD_NL::radix,             ETL_NL::radix);
      CHECK_EQUAL(STD_NL::round_error(),     ETL_NL::round_error());
      CHECK_EQUAL(int(STD_NL::round_style),  int(ETL_NL::round_style));
      CHECK_EQUAL(STD_NL::signaling_NaN(),   ETL_NL::signaling_NaN());
      CHECK_EQUAL(STD_NL::tinyness_before,   ETL_NL::tinyness_before);
    }

    //*************************************************************************
    TEST(test_float)
    {
      typedef etl::numeric_limits<float> ETL_NL;
      typedef std::numeric_limits<float> STD_NL;

      CHECK_EQUAL(STD_NL::digits,            ETL_NL::digits);
      CHECK_EQUAL(STD_NL::digits10,          ETL_NL::digits10);
      CHECK_EQUAL(STD_NL::epsilon(),         ETL_NL::epsilon());
      CHECK_EQUAL(STD_NL::is_bounded,        ETL_NL::is_bounded);
      CHECK_EQUAL(STD_NL::is_exact,          ETL_NL::is_exact);
      CHECK_EQUAL(STD_NL::is_integer,        ETL_NL::is_integer);
      CHECK_EQUAL(STD_NL::is_modulo,         ETL_NL::is_modulo);
      CHECK_EQUAL(STD_NL::is_signed,         ETL_NL::is_signed);
      CHECK_EQUAL(STD_NL::is_specialized,    ETL_NL::is_specialized);
      CHECK_EQUAL(STD_NL::lowest(),          ETL_NL::lowest());
      CHECK_EQUAL(STD_NL::max(),             ETL_NL::max());
      CHECK_EQUAL(STD_NL::max_digits10,      ETL_NL::max_digits10);
      CHECK_EQUAL(STD_NL::max_exponent,      ETL_NL::max_exponent);
      CHECK_EQUAL(STD_NL::max_exponent10,    ETL_NL::max_exponent10);
      CHECK_EQUAL(STD_NL::min(),             ETL_NL::min());
      CHECK_EQUAL(STD_NL::min_exponent,      ETL_NL::min_exponent);
      CHECK_EQUAL(STD_NL::min_exponent10,    ETL_NL::min_exponent10);
      CHECK_EQUAL(STD_NL::radix,             ETL_NL::radix);
      CHECK_EQUAL(STD_NL::round_error(),     ETL_NL::round_error());
    }

    //*************************************************************************
    TEST(test_double)
    {
      typedef etl::numeric_limits<double> ETL_NL;
      typedef std::numeric_limits<double> STD_NL;

      CHECK_EQUAL(STD_NL::digits,            ETL_NL::digits);
      CHECK_EQUAL(STD_NL::digits10,          ETL_NL::digits10);
      CHECK_EQUAL(STD_NL::epsilon(),         ETL_NL::epsilon());
      CHECK_EQUAL(STD_NL::is_bounded,        ETL_NL::is_bounded);
      CHECK_EQUAL(STD_NL::is_exact,          ETL_NL::is_exact);
      CHECK_EQUAL(STD_NL::is_integer,        ETL_NL::is_integer);
      CHECK_EQUAL(STD_NL::is_modulo,         ETL_NL::is_modulo);
      CHECK_EQUAL(STD_NL::is_signed,         ETL_NL::is_signed);
      CHECK_EQUAL(STD_NL::is_specialized,    ETL_NL::is_specialized);
      CHECK_EQUAL(STD_NL::lowest(),          ETL_NL::lowest());
      CHECK_EQUAL(STD_NL::max(),             ETL_NL::max());
      CHECK_EQUAL(STD_NL::max_digits10,      ETL_NL::max_digits10);
      CHECK_EQUAL(STD_NL::max_exponent,      ETL_NL::max_exponent);
      CHECK_EQUAL(STD_NL::max_exponent10,    ETL_NL::max_exponent10);
      CHECK_EQUAL(STD_NL::min(),             ETL_NL::min());
      CHECK_EQUAL(STD_NL::min_exponent,      ETL_NL::min_exponent);
      CHECK_EQUAL(STD_NL::min_exponent10,    ETL_NL::min_exponent10);
      CHECK_EQUAL(STD_NL::radix,             ETL_NL::radix);
      CHECK_EQUAL(STD_NL::round_error(),     ETL_NL::round_error());
    }

    //*************************************************************************
    TEST(test_long_double)
    {
      typedef etl::numeric_limits<long double> ETL_NL;
      typedef std::numeric_limits<long double> STD_NL;

      CHECK_EQUAL(STD_NL::digits,            ETL_NL::digits);
      CHECK_EQUAL(STD_NL::digits10,          ETL_NL::digits10);
      CHECK_EQUAL(STD_NL::epsilon(),         ETL_NL::epsilon());
      CHECK_EQUAL(STD_NL::is_bounded,        ETL_NL::is_bounded);
      CHECK_EQUAL(STD_NL::is_exact,          ETL_NL::is_exact);
      CHECK_EQUAL(STD_NL::is_integer,        ETL_NL::is_integer);
      CHECK_EQUAL(STD_NL::is_modulo,         ETL_NL::is_modulo);
      CHECK_EQUAL(STD_NL::is_signed,         ETL_NL::is_signed);
      CHECK_EQUAL(STD_NL::is_specialized,    ETL_NL::is_specialized);
      CHECK_EQUAL(STD_NL::lowest(),          ETL_NL::lowest());
      CHECK_EQUAL(STD_NL::max(),             ETL_NL::max());
      CHECK_EQUAL(STD_NL::max_digits10,      ETL_NL::max_digits10);
      CHECK_EQUAL(STD_NL::max_exponent,      ETL_NL::max_exponent);
      CHECK_EQUAL(STD_NL::max_exponent10,    ETL_NL::max_exponent10);
      CHECK_EQUAL(STD_NL::min(),             ETL_NL::min());
      CHECK_EQUAL(STD_NL::min_exponent,      ETL_NL::min_exponent);
      CHECK_EQUAL(STD_NL::min_exponent10,    ETL_NL::min_exponent10);
      CHECK_EQUAL(STD_NL::radix,             ETL_NL::radix);
      CHECK_EQUAL(STD_NL::round_error(),     ETL_NL::round_error());
    }
  };
}
