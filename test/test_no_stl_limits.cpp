/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl
https://www.etlcpp.com

Copyright(c) 2018 jwellbelove

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

#include "etl/stl/alternate/limits.h"

#include <limits>

namespace
{
  SUITE(test_no_stl_limits)
  {
    //*************************************************************************
    TEST(test_bool)
    {
      typedef etlstd::numeric_limits<bool> ETL;
      typedef std::numeric_limits<bool>    STD;

      CHECK_EQUAL(STD::denorm_min(),      ETL::denorm_min());
      CHECK_EQUAL(STD::digits,            ETL::digits);
      CHECK_EQUAL(STD::digits10,          ETL::digits10);
      CHECK_EQUAL(STD::epsilon(),         ETL::epsilon());
      CHECK_EQUAL(int(STD::has_denorm),   int(ETL::has_denorm));
      CHECK_EQUAL(STD::has_denorm_loss,   ETL::has_denorm_loss);
      CHECK_EQUAL(STD::has_infinity,      ETL::has_infinity);
      CHECK_EQUAL(STD::has_quiet_NaN,     ETL::has_quiet_NaN);
      CHECK_EQUAL(STD::has_signaling_NaN, ETL::has_signaling_NaN);
      CHECK_EQUAL(STD::infinity(),        ETL::infinity());
      CHECK_EQUAL(STD::is_bounded,        ETL::is_bounded);
      CHECK_EQUAL(STD::is_exact,          ETL::is_exact);
      CHECK_EQUAL(STD::is_iec559,         ETL::is_iec559);
      CHECK_EQUAL(STD::is_integer,        ETL::is_integer);
      CHECK_EQUAL(STD::is_modulo,         ETL::is_modulo);
      CHECK_EQUAL(STD::is_signed,         ETL::is_signed);
      CHECK_EQUAL(STD::is_specialized,    ETL::is_specialized);
      CHECK_EQUAL(STD::lowest(),          ETL::lowest());
      CHECK_EQUAL(STD::max(),             ETL::max());
      CHECK_EQUAL(STD::max_digits10,      ETL::max_digits10);
      CHECK_EQUAL(STD::max_exponent,      ETL::max_exponent);
      CHECK_EQUAL(STD::max_exponent10,    ETL::max_exponent10);
      CHECK_EQUAL(STD::min(),             ETL::min());
      CHECK_EQUAL(STD::min_exponent,      ETL::min_exponent);
      CHECK_EQUAL(STD::min_exponent10,    ETL::min_exponent10);
      CHECK_EQUAL(STD::quiet_NaN(),       ETL::quiet_NaN());
      CHECK_EQUAL(STD::radix,             ETL::radix);
      CHECK_EQUAL(STD::round_error(),     ETL::round_error());
      CHECK_EQUAL(int(STD::round_style),  int(ETL::round_style));
      CHECK_EQUAL(STD::signaling_NaN(),   ETL::signaling_NaN());
      CHECK_EQUAL(STD::tinyness_before,   ETL::tinyness_before);
      CHECK_EQUAL(false,                  ETL::traps);
    }

    //*************************************************************************
    TEST(test_char)
    {
      typedef etlstd::numeric_limits<char> ETL;
      typedef std::numeric_limits<char>    STD;

      CHECK_EQUAL(STD::denorm_min(),      ETL::denorm_min());
      CHECK_EQUAL(STD::digits,            ETL::digits);
      CHECK_EQUAL(STD::digits10,          ETL::digits10);
      CHECK_EQUAL(STD::epsilon(),         ETL::epsilon());
      CHECK_EQUAL(int(STD::has_denorm),   int(ETL::has_denorm));
      CHECK_EQUAL(STD::has_denorm_loss,   ETL::has_denorm_loss);
      CHECK_EQUAL(STD::has_infinity,      ETL::has_infinity);
      CHECK_EQUAL(STD::has_quiet_NaN,     ETL::has_quiet_NaN);
      CHECK_EQUAL(STD::has_signaling_NaN, ETL::has_signaling_NaN);
      CHECK_EQUAL(STD::infinity(),        ETL::infinity());
      CHECK_EQUAL(STD::is_bounded,        ETL::is_bounded);
      CHECK_EQUAL(STD::is_exact,          ETL::is_exact);
      CHECK_EQUAL(STD::is_iec559,         ETL::is_iec559);
      CHECK_EQUAL(STD::is_integer,        ETL::is_integer);
      CHECK_EQUAL(false,                  ETL::is_modulo);
      CHECK_EQUAL(STD::is_signed,         ETL::is_signed);
      CHECK_EQUAL(STD::is_specialized,    ETL::is_specialized);
      CHECK_EQUAL(STD::lowest(),          ETL::lowest());
      CHECK_EQUAL(STD::max(),             ETL::max());
      CHECK_EQUAL(STD::max_digits10,      ETL::max_digits10);
      CHECK_EQUAL(STD::max_exponent,      ETL::max_exponent);
      CHECK_EQUAL(STD::max_exponent10,    ETL::max_exponent10);
      CHECK_EQUAL(STD::min(),             ETL::min());
      CHECK_EQUAL(STD::min_exponent,      ETL::min_exponent);
      CHECK_EQUAL(STD::min_exponent10,    ETL::min_exponent10);
      CHECK_EQUAL(STD::quiet_NaN(),       ETL::quiet_NaN());
      CHECK_EQUAL(STD::radix,             ETL::radix);
      CHECK_EQUAL(STD::round_error(),     ETL::round_error());
      CHECK_EQUAL(int(STD::round_style),  int(ETL::round_style));
      CHECK_EQUAL(STD::signaling_NaN(),   ETL::signaling_NaN());
      CHECK_EQUAL(STD::tinyness_before,   ETL::tinyness_before);
      CHECK_EQUAL(false,                  ETL::traps);
    }

    //*************************************************************************
    TEST(test_unsigned_char)
    {
      typedef etlstd::numeric_limits<unsigned char> ETL;
      typedef std::numeric_limits<unsigned char>    STD;

      CHECK_EQUAL(STD::denorm_min(),      ETL::denorm_min());
      CHECK_EQUAL(STD::digits,            ETL::digits);
      CHECK_EQUAL(STD::digits10,          ETL::digits10);
      CHECK_EQUAL(STD::epsilon(),         ETL::epsilon());
      CHECK_EQUAL(int(STD::has_denorm),   int(ETL::has_denorm));
      CHECK_EQUAL(STD::has_denorm_loss,   ETL::has_denorm_loss);
      CHECK_EQUAL(STD::has_infinity,      ETL::has_infinity);
      CHECK_EQUAL(STD::has_quiet_NaN,     ETL::has_quiet_NaN);
      CHECK_EQUAL(STD::has_signaling_NaN, ETL::has_signaling_NaN);
      CHECK_EQUAL(STD::infinity(),        ETL::infinity());
      CHECK_EQUAL(STD::is_bounded,        ETL::is_bounded);
      CHECK_EQUAL(STD::is_exact,          ETL::is_exact);
      CHECK_EQUAL(STD::is_iec559,         ETL::is_iec559);
      CHECK_EQUAL(STD::is_integer,        ETL::is_integer);
      CHECK_EQUAL(true,                   ETL::is_modulo);
      CHECK_EQUAL(STD::is_signed,         ETL::is_signed);
      CHECK_EQUAL(STD::is_specialized,    ETL::is_specialized);
      CHECK_EQUAL(STD::lowest(),          ETL::lowest());
      CHECK_EQUAL(STD::max(),             ETL::max());
      CHECK_EQUAL(STD::max_digits10,      ETL::max_digits10);
      CHECK_EQUAL(STD::max_exponent,      ETL::max_exponent);
      CHECK_EQUAL(STD::max_exponent10,    ETL::max_exponent10);
      CHECK_EQUAL(STD::min(),             ETL::min());
      CHECK_EQUAL(STD::min_exponent,      ETL::min_exponent);
      CHECK_EQUAL(STD::min_exponent10,    ETL::min_exponent10);
      CHECK_EQUAL(STD::quiet_NaN(),       ETL::quiet_NaN());
      CHECK_EQUAL(STD::radix,             ETL::radix);
      CHECK_EQUAL(STD::round_error(),     ETL::round_error());
      CHECK_EQUAL(int(STD::round_style),  int(ETL::round_style));
      CHECK_EQUAL(STD::signaling_NaN(),   ETL::signaling_NaN());
      CHECK_EQUAL(STD::tinyness_before,   ETL::tinyness_before);
      CHECK_EQUAL(false,                  ETL::traps);
    }

    //*************************************************************************
    TEST(test_signed_char)
    {
      typedef etlstd::numeric_limits<signed char> ETL;
      typedef std::numeric_limits<signed char>    STD;

      CHECK_EQUAL(STD::denorm_min(),      ETL::denorm_min());
      CHECK_EQUAL(STD::digits,            ETL::digits);
      CHECK_EQUAL(STD::digits10,          ETL::digits10);
      CHECK_EQUAL(STD::epsilon(),         ETL::epsilon());
      CHECK_EQUAL(int(STD::has_denorm),   int(ETL::has_denorm));
      CHECK_EQUAL(STD::has_denorm_loss,   ETL::has_denorm_loss);
      CHECK_EQUAL(STD::has_infinity,      ETL::has_infinity);
      CHECK_EQUAL(STD::has_quiet_NaN,     ETL::has_quiet_NaN);
      CHECK_EQUAL(STD::has_signaling_NaN, ETL::has_signaling_NaN);
      CHECK_EQUAL(STD::infinity(),        ETL::infinity());
      CHECK_EQUAL(STD::is_bounded,        ETL::is_bounded);
      CHECK_EQUAL(STD::is_exact,          ETL::is_exact);
      CHECK_EQUAL(STD::is_iec559,         ETL::is_iec559);
      CHECK_EQUAL(STD::is_integer,        ETL::is_integer);
      CHECK_EQUAL(false,                  ETL::is_modulo);
      CHECK_EQUAL(STD::is_signed,         ETL::is_signed);
      CHECK_EQUAL(STD::is_specialized,    ETL::is_specialized);
      CHECK_EQUAL(STD::lowest(),          ETL::lowest());
      CHECK_EQUAL(STD::max(),             ETL::max());
      CHECK_EQUAL(STD::max_digits10,      ETL::max_digits10);
      CHECK_EQUAL(STD::max_exponent,      ETL::max_exponent);
      CHECK_EQUAL(STD::max_exponent10,    ETL::max_exponent10);
      CHECK_EQUAL(STD::min(),             ETL::min());
      CHECK_EQUAL(STD::min_exponent,      ETL::min_exponent);
      CHECK_EQUAL(STD::min_exponent10,    ETL::min_exponent10);
      CHECK_EQUAL(STD::quiet_NaN(),       ETL::quiet_NaN());
      CHECK_EQUAL(STD::radix,             ETL::radix);
      CHECK_EQUAL(STD::round_error(),     ETL::round_error());
      CHECK_EQUAL(int(STD::round_style),  int(ETL::round_style));
      CHECK_EQUAL(STD::signaling_NaN(),   ETL::signaling_NaN());
      CHECK_EQUAL(STD::tinyness_before,   ETL::tinyness_before);
      CHECK_EQUAL(false,                  ETL::traps);
    }

    //*************************************************************************
    TEST(test_char16_t)
    {
      typedef etlstd::numeric_limits<char16_t> ETL;
      typedef std::numeric_limits<char16_t>    STD;

      CHECK_EQUAL(STD::denorm_min(),      ETL::denorm_min());
      CHECK_EQUAL(STD::digits,            ETL::digits);
      CHECK_EQUAL(STD::digits10,          ETL::digits10);
      CHECK_EQUAL(STD::epsilon(),         ETL::epsilon());
      CHECK_EQUAL(int(STD::has_denorm),   int(ETL::has_denorm));
      CHECK_EQUAL(STD::has_denorm_loss,   ETL::has_denorm_loss);
      CHECK_EQUAL(STD::has_infinity,      ETL::has_infinity);
      CHECK_EQUAL(STD::has_quiet_NaN,     ETL::has_quiet_NaN);
      CHECK_EQUAL(STD::has_signaling_NaN, ETL::has_signaling_NaN);
      CHECK_EQUAL(STD::infinity(),        ETL::infinity());
      CHECK_EQUAL(STD::is_bounded,        ETL::is_bounded);
      CHECK_EQUAL(STD::is_exact,          ETL::is_exact);
      CHECK_EQUAL(STD::is_iec559,         ETL::is_iec559);
      CHECK_EQUAL(STD::is_integer,        ETL::is_integer);
      CHECK_EQUAL(false,                  ETL::is_modulo);
      CHECK_EQUAL(STD::is_signed,         ETL::is_signed);
      CHECK_EQUAL(STD::is_specialized,    ETL::is_specialized);
      CHECK_EQUAL(STD::lowest(),          ETL::lowest());
      CHECK_EQUAL(STD::max(),             ETL::max());
      CHECK_EQUAL(STD::max_digits10,      ETL::max_digits10);
      CHECK_EQUAL(STD::max_exponent,      ETL::max_exponent);
      CHECK_EQUAL(STD::max_exponent10,    ETL::max_exponent10);
      CHECK_EQUAL(STD::min(),             ETL::min());
      CHECK_EQUAL(STD::min_exponent,      ETL::min_exponent);
      CHECK_EQUAL(STD::min_exponent10,    ETL::min_exponent10);
      CHECK_EQUAL(STD::quiet_NaN(),       ETL::quiet_NaN());
      CHECK_EQUAL(STD::radix,             ETL::radix);
      CHECK_EQUAL(STD::round_error(),     ETL::round_error());
      CHECK_EQUAL(int(STD::round_style),  int(ETL::round_style));
      CHECK_EQUAL(STD::signaling_NaN(),   ETL::signaling_NaN());
      CHECK_EQUAL(STD::tinyness_before,   ETL::tinyness_before);
      CHECK_EQUAL(false,                  ETL::traps);
    }

    //*************************************************************************
    TEST(test_char32_t)
    {
      typedef etlstd::numeric_limits<char32_t> ETL;
      typedef std::numeric_limits<char32_t>    STD;

      CHECK_EQUAL(STD::denorm_min(),      ETL::denorm_min());
      CHECK_EQUAL(STD::digits,            ETL::digits);
      CHECK_EQUAL(STD::digits10,          ETL::digits10);
      CHECK_EQUAL(STD::epsilon(),         ETL::epsilon());
      CHECK_EQUAL(int(STD::has_denorm),   int(ETL::has_denorm));
      CHECK_EQUAL(STD::has_denorm_loss,   ETL::has_denorm_loss);
      CHECK_EQUAL(STD::has_infinity,      ETL::has_infinity);
      CHECK_EQUAL(STD::has_quiet_NaN,     ETL::has_quiet_NaN);
      CHECK_EQUAL(STD::has_signaling_NaN, ETL::has_signaling_NaN);
      CHECK_EQUAL(STD::infinity(),        ETL::infinity());
      CHECK_EQUAL(STD::is_bounded,        ETL::is_bounded);
      CHECK_EQUAL(STD::is_exact,          ETL::is_exact);
      CHECK_EQUAL(STD::is_iec559,         ETL::is_iec559);
      CHECK_EQUAL(STD::is_integer,        ETL::is_integer);
      CHECK_EQUAL(false,                  ETL::is_modulo);
      CHECK_EQUAL(STD::is_signed,         ETL::is_signed);
      CHECK_EQUAL(STD::is_specialized,    ETL::is_specialized);
      CHECK_EQUAL(STD::lowest(),          ETL::lowest());
      CHECK_EQUAL(STD::max(),             ETL::max());
      CHECK_EQUAL(STD::max_digits10,      ETL::max_digits10);
      CHECK_EQUAL(STD::max_exponent,      ETL::max_exponent);
      CHECK_EQUAL(STD::max_exponent10,    ETL::max_exponent10);
      CHECK_EQUAL(STD::min(),             ETL::min());
      CHECK_EQUAL(STD::min_exponent,      ETL::min_exponent);
      CHECK_EQUAL(STD::min_exponent10,    ETL::min_exponent10);
      CHECK_EQUAL(STD::quiet_NaN(),       ETL::quiet_NaN());
      CHECK_EQUAL(STD::radix,             ETL::radix);
      CHECK_EQUAL(STD::round_error(),     ETL::round_error());
      CHECK_EQUAL(int(STD::round_style),  int(ETL::round_style));
      CHECK_EQUAL(STD::signaling_NaN(),   ETL::signaling_NaN());
      CHECK_EQUAL(STD::tinyness_before,   ETL::tinyness_before);
      CHECK_EQUAL(false,                  ETL::traps);
    }

    //*************************************************************************
    TEST(test_wchar_t)
    {
      typedef etlstd::numeric_limits<wchar_t> ETL;
      typedef std::numeric_limits<wchar_t>    STD;

      CHECK_EQUAL(STD::denorm_min(),      ETL::denorm_min());
      CHECK_EQUAL(STD::digits,            ETL::digits);
      CHECK_EQUAL(STD::digits10,          ETL::digits10);
      CHECK_EQUAL(STD::epsilon(),         ETL::epsilon());
      CHECK_EQUAL(int(STD::has_denorm),   int(ETL::has_denorm));
      CHECK_EQUAL(STD::has_denorm_loss,   ETL::has_denorm_loss);
      CHECK_EQUAL(STD::has_infinity,      ETL::has_infinity);
      CHECK_EQUAL(STD::has_quiet_NaN,     ETL::has_quiet_NaN);
      CHECK_EQUAL(STD::has_signaling_NaN, ETL::has_signaling_NaN);
      CHECK_EQUAL(STD::infinity(),        ETL::infinity());
      CHECK_EQUAL(STD::is_bounded,        ETL::is_bounded);
      CHECK_EQUAL(STD::is_exact,          ETL::is_exact);
      CHECK_EQUAL(STD::is_iec559,         ETL::is_iec559);
      CHECK_EQUAL(STD::is_integer,        ETL::is_integer);
      CHECK_EQUAL(STD::is_modulo,         ETL::is_modulo);
      CHECK_EQUAL(STD::is_signed,         ETL::is_signed);
      CHECK_EQUAL(STD::is_specialized,    ETL::is_specialized);
      CHECK_EQUAL(STD::lowest(),          ETL::lowest());
      CHECK_EQUAL(STD::max(),             ETL::max());
      CHECK_EQUAL(STD::max_digits10,      ETL::max_digits10);
      CHECK_EQUAL(STD::max_exponent,      ETL::max_exponent);
      CHECK_EQUAL(STD::max_exponent10,    ETL::max_exponent10);
      CHECK_EQUAL(STD::min(),             ETL::min());
      CHECK_EQUAL(STD::min_exponent,      ETL::min_exponent);
      CHECK_EQUAL(STD::min_exponent10,    ETL::min_exponent10);
      CHECK_EQUAL(STD::quiet_NaN(),       ETL::quiet_NaN());
      CHECK_EQUAL(STD::radix,             ETL::radix);
      CHECK_EQUAL(STD::round_error(),     ETL::round_error());
      CHECK_EQUAL(int(STD::round_style),  int(ETL::round_style));
      CHECK_EQUAL(STD::signaling_NaN(),   ETL::signaling_NaN());
      CHECK_EQUAL(STD::tinyness_before,   ETL::tinyness_before);
      CHECK_EQUAL(false,                  ETL::traps);
    }

    //*************************************************************************
    TEST(test_short)
    {
      typedef etlstd::numeric_limits<short> ETL;
      typedef std::numeric_limits<short>    STD;

      CHECK_EQUAL(STD::denorm_min(),      ETL::denorm_min());
      CHECK_EQUAL(STD::digits,            ETL::digits);
      CHECK_EQUAL(STD::digits10,          ETL::digits10);
      CHECK_EQUAL(STD::epsilon(),         ETL::epsilon());
      CHECK_EQUAL(int(STD::has_denorm),   int(ETL::has_denorm));
      CHECK_EQUAL(STD::has_denorm_loss,   ETL::has_denorm_loss);
      CHECK_EQUAL(STD::has_infinity,      ETL::has_infinity);
      CHECK_EQUAL(STD::has_quiet_NaN,     ETL::has_quiet_NaN);
      CHECK_EQUAL(STD::has_signaling_NaN, ETL::has_signaling_NaN);
      CHECK_EQUAL(STD::infinity(),        ETL::infinity());
      CHECK_EQUAL(STD::is_bounded,        ETL::is_bounded);
      CHECK_EQUAL(STD::is_exact,          ETL::is_exact);
      CHECK_EQUAL(STD::is_iec559,         ETL::is_iec559);
      CHECK_EQUAL(STD::is_integer,        ETL::is_integer);
      CHECK_EQUAL(false,                  ETL::is_modulo);
      CHECK_EQUAL(STD::is_signed,         ETL::is_signed);
      CHECK_EQUAL(STD::is_specialized,    ETL::is_specialized);
      CHECK_EQUAL(STD::lowest(),          ETL::lowest());
      CHECK_EQUAL(STD::max(),             ETL::max());
      CHECK_EQUAL(STD::max_digits10,      ETL::max_digits10);
      CHECK_EQUAL(STD::max_exponent,      ETL::max_exponent);
      CHECK_EQUAL(STD::max_exponent10,    ETL::max_exponent10);
      CHECK_EQUAL(STD::min(),             ETL::min());
      CHECK_EQUAL(STD::min_exponent,      ETL::min_exponent);
      CHECK_EQUAL(STD::min_exponent10,    ETL::min_exponent10);
      CHECK_EQUAL(STD::quiet_NaN(),       ETL::quiet_NaN());
      CHECK_EQUAL(STD::radix,             ETL::radix);
      CHECK_EQUAL(STD::round_error(),     ETL::round_error());
      CHECK_EQUAL(int(STD::round_style),  int(ETL::round_style));
      CHECK_EQUAL(STD::signaling_NaN(),   ETL::signaling_NaN());
      CHECK_EQUAL(STD::tinyness_before,   ETL::tinyness_before);
      CHECK_EQUAL(false,                  ETL::traps);
    }

    //*************************************************************************
    TEST(test_unsigned_short)
    {
      typedef etlstd::numeric_limits<unsigned short> ETL;
      typedef std::numeric_limits<unsigned short>    STD;

      CHECK_EQUAL(STD::denorm_min(),      ETL::denorm_min());
      CHECK_EQUAL(STD::digits,            ETL::digits);
      CHECK_EQUAL(STD::digits10,          ETL::digits10);
      CHECK_EQUAL(STD::epsilon(),         ETL::epsilon());
      CHECK_EQUAL(int(STD::has_denorm),   int(ETL::has_denorm));
      CHECK_EQUAL(STD::has_denorm_loss,   ETL::has_denorm_loss);
      CHECK_EQUAL(STD::has_infinity,      ETL::has_infinity);
      CHECK_EQUAL(STD::has_quiet_NaN,     ETL::has_quiet_NaN);
      CHECK_EQUAL(STD::has_signaling_NaN, ETL::has_signaling_NaN);
      CHECK_EQUAL(STD::infinity(),        ETL::infinity());
      CHECK_EQUAL(STD::is_bounded,        ETL::is_bounded);
      CHECK_EQUAL(STD::is_exact,          ETL::is_exact);
      CHECK_EQUAL(STD::is_iec559,         ETL::is_iec559);
      CHECK_EQUAL(STD::is_integer,        ETL::is_integer);
      CHECK_EQUAL(true,                   ETL::is_modulo);
      CHECK_EQUAL(STD::is_signed,         ETL::is_signed);
      CHECK_EQUAL(STD::is_specialized,    ETL::is_specialized);
      CHECK_EQUAL(STD::lowest(),          ETL::lowest());
      CHECK_EQUAL(STD::max(),             ETL::max());
      CHECK_EQUAL(STD::max_digits10,      ETL::max_digits10);
      CHECK_EQUAL(STD::max_exponent,      ETL::max_exponent);
      CHECK_EQUAL(STD::max_exponent10,    ETL::max_exponent10);
      CHECK_EQUAL(STD::min(),             ETL::min());
      CHECK_EQUAL(STD::min_exponent,      ETL::min_exponent);
      CHECK_EQUAL(STD::min_exponent10,    ETL::min_exponent10);
      CHECK_EQUAL(STD::quiet_NaN(),       ETL::quiet_NaN());
      CHECK_EQUAL(STD::radix,             ETL::radix);
      CHECK_EQUAL(STD::round_error(),     ETL::round_error());
      CHECK_EQUAL(int(STD::round_style),  int(ETL::round_style));
      CHECK_EQUAL(STD::signaling_NaN(),   ETL::signaling_NaN());
      CHECK_EQUAL(STD::tinyness_before,   ETL::tinyness_before);
      CHECK_EQUAL(false,                  ETL::traps);
    }

    //*************************************************************************
    TEST(test_int)
    {
      typedef etlstd::numeric_limits<int> ETL;
      typedef std::numeric_limits<int>    STD;

      CHECK_EQUAL(STD::denorm_min(),      ETL::denorm_min());
      CHECK_EQUAL(STD::digits,            ETL::digits);
      CHECK_EQUAL(STD::digits10,          ETL::digits10);
      CHECK_EQUAL(STD::epsilon(),         ETL::epsilon());
      CHECK_EQUAL(int(STD::has_denorm),   int(ETL::has_denorm));
      CHECK_EQUAL(STD::has_denorm_loss,   ETL::has_denorm_loss);
      CHECK_EQUAL(STD::has_infinity,      ETL::has_infinity);
      CHECK_EQUAL(STD::has_quiet_NaN,     ETL::has_quiet_NaN);
      CHECK_EQUAL(STD::has_signaling_NaN, ETL::has_signaling_NaN);
      CHECK_EQUAL(STD::infinity(),        ETL::infinity());
      CHECK_EQUAL(STD::is_bounded,        ETL::is_bounded);
      CHECK_EQUAL(STD::is_exact,          ETL::is_exact);
      CHECK_EQUAL(STD::is_iec559,         ETL::is_iec559);
      CHECK_EQUAL(STD::is_integer,        ETL::is_integer);
      CHECK_EQUAL(false,                  ETL::is_modulo);
      CHECK_EQUAL(STD::is_signed,         ETL::is_signed);
      CHECK_EQUAL(STD::is_specialized,    ETL::is_specialized);
      CHECK_EQUAL(STD::lowest(),          ETL::lowest());
      CHECK_EQUAL(STD::max(),             ETL::max());
      CHECK_EQUAL(STD::max_digits10,      ETL::max_digits10);
      CHECK_EQUAL(STD::max_exponent,      ETL::max_exponent);
      CHECK_EQUAL(STD::max_exponent10,    ETL::max_exponent10);
      CHECK_EQUAL(STD::min(),             ETL::min());
      CHECK_EQUAL(STD::min_exponent,      ETL::min_exponent);
      CHECK_EQUAL(STD::min_exponent10,    ETL::min_exponent10);
      CHECK_EQUAL(STD::quiet_NaN(),       ETL::quiet_NaN());
      CHECK_EQUAL(STD::radix,             ETL::radix);
      CHECK_EQUAL(STD::round_error(),     ETL::round_error());
      CHECK_EQUAL(int(STD::round_style),  int(ETL::round_style));
      CHECK_EQUAL(STD::signaling_NaN(),   ETL::signaling_NaN());
      CHECK_EQUAL(STD::tinyness_before,   ETL::tinyness_before);
      CHECK_EQUAL(false,                  ETL::traps);
    }

    //*************************************************************************
    TEST(test_unsigned_int)
    {
      typedef etlstd::numeric_limits<unsigned int> ETL;
      typedef std::numeric_limits<unsigned int>    STD;

      CHECK_EQUAL(STD::denorm_min(),      ETL::denorm_min());
      CHECK_EQUAL(STD::digits,            ETL::digits);
      CHECK_EQUAL(STD::digits10,          ETL::digits10);
      CHECK_EQUAL(STD::epsilon(),         ETL::epsilon());
      CHECK_EQUAL(int(STD::has_denorm),   int(ETL::has_denorm));
      CHECK_EQUAL(STD::has_denorm_loss,   ETL::has_denorm_loss);
      CHECK_EQUAL(STD::has_infinity,      ETL::has_infinity);
      CHECK_EQUAL(STD::has_quiet_NaN,     ETL::has_quiet_NaN);
      CHECK_EQUAL(STD::has_signaling_NaN, ETL::has_signaling_NaN);
      CHECK_EQUAL(STD::infinity(),        ETL::infinity());
      CHECK_EQUAL(STD::is_bounded,        ETL::is_bounded);
      CHECK_EQUAL(STD::is_exact,          ETL::is_exact);
      CHECK_EQUAL(STD::is_iec559,         ETL::is_iec559);
      CHECK_EQUAL(STD::is_integer,        ETL::is_integer);
      CHECK_EQUAL(true,                   ETL::is_modulo);
      CHECK_EQUAL(STD::is_signed,         ETL::is_signed);
      CHECK_EQUAL(STD::is_specialized,    ETL::is_specialized);
      CHECK_EQUAL(STD::lowest(),          ETL::lowest());
      CHECK_EQUAL(STD::max(),             ETL::max());
      CHECK_EQUAL(STD::max_digits10,      ETL::max_digits10);
      CHECK_EQUAL(STD::max_exponent,      ETL::max_exponent);
      CHECK_EQUAL(STD::max_exponent10,    ETL::max_exponent10);
      CHECK_EQUAL(STD::min(),             ETL::min());
      CHECK_EQUAL(STD::min_exponent,      ETL::min_exponent);
      CHECK_EQUAL(STD::min_exponent10,    ETL::min_exponent10);
      CHECK_EQUAL(STD::quiet_NaN(),       ETL::quiet_NaN());
      CHECK_EQUAL(STD::radix,             ETL::radix);
      CHECK_EQUAL(STD::round_error(),     ETL::round_error());
      CHECK_EQUAL(int(STD::round_style),  int(ETL::round_style));
      CHECK_EQUAL(STD::signaling_NaN(),   ETL::signaling_NaN());
      CHECK_EQUAL(STD::tinyness_before,   ETL::tinyness_before);
      CHECK_EQUAL(false,                  ETL::traps);
    }

    //*************************************************************************
    TEST(test_long)
    {
      typedef etlstd::numeric_limits<long> ETL;
      typedef std::numeric_limits<long>    STD;

      CHECK_EQUAL(STD::denorm_min(),      ETL::denorm_min());
      CHECK_EQUAL(STD::digits,            ETL::digits);
      CHECK_EQUAL(STD::digits10,          ETL::digits10);
      CHECK_EQUAL(STD::epsilon(),         ETL::epsilon());
      CHECK_EQUAL(int(STD::has_denorm),   int(ETL::has_denorm));
      CHECK_EQUAL(STD::has_denorm_loss,   ETL::has_denorm_loss);
      CHECK_EQUAL(STD::has_infinity,      ETL::has_infinity);
      CHECK_EQUAL(STD::has_quiet_NaN,     ETL::has_quiet_NaN);
      CHECK_EQUAL(STD::has_signaling_NaN, ETL::has_signaling_NaN);
      CHECK_EQUAL(STD::infinity(),        ETL::infinity());
      CHECK_EQUAL(STD::is_bounded,        ETL::is_bounded);
      CHECK_EQUAL(STD::is_exact,          ETL::is_exact);
      CHECK_EQUAL(STD::is_iec559,         ETL::is_iec559);
      CHECK_EQUAL(STD::is_integer,        ETL::is_integer);
      CHECK_EQUAL(false,                  ETL::is_modulo);
      CHECK_EQUAL(STD::is_signed,         ETL::is_signed);
      CHECK_EQUAL(STD::is_specialized,    ETL::is_specialized);
      CHECK_EQUAL(STD::lowest(),          ETL::lowest());
      CHECK_EQUAL(STD::max(),             ETL::max());
      CHECK_EQUAL(STD::max_digits10,      ETL::max_digits10);
      CHECK_EQUAL(STD::max_exponent,      ETL::max_exponent);
      CHECK_EQUAL(STD::max_exponent10,    ETL::max_exponent10);
      CHECK_EQUAL(STD::min(),             ETL::min());
      CHECK_EQUAL(STD::min_exponent,      ETL::min_exponent);
      CHECK_EQUAL(STD::min_exponent10,    ETL::min_exponent10);
      CHECK_EQUAL(STD::quiet_NaN(),       ETL::quiet_NaN());
      CHECK_EQUAL(STD::radix,             ETL::radix);
      CHECK_EQUAL(STD::round_error(),     ETL::round_error());
      CHECK_EQUAL(int(STD::round_style),  int(ETL::round_style));
      CHECK_EQUAL(STD::signaling_NaN(),   ETL::signaling_NaN());
      CHECK_EQUAL(STD::tinyness_before,   ETL::tinyness_before);
      CHECK_EQUAL(false,                  ETL::traps);
    }

    //*************************************************************************
    TEST(test_unsigned_long)
    {
      typedef etlstd::numeric_limits<unsigned long> ETL;
      typedef std::numeric_limits<unsigned long>    STD;

      CHECK_EQUAL(STD::denorm_min(),      ETL::denorm_min());
      CHECK_EQUAL(STD::digits,            ETL::digits);
      CHECK_EQUAL(STD::digits10,          ETL::digits10);
      CHECK_EQUAL(STD::epsilon(),         ETL::epsilon());
      CHECK_EQUAL(int(STD::has_denorm),   int(ETL::has_denorm));
      CHECK_EQUAL(STD::has_denorm_loss,   ETL::has_denorm_loss);
      CHECK_EQUAL(STD::has_infinity,      ETL::has_infinity);
      CHECK_EQUAL(STD::has_quiet_NaN,     ETL::has_quiet_NaN);
      CHECK_EQUAL(STD::has_signaling_NaN, ETL::has_signaling_NaN);
      CHECK_EQUAL(STD::infinity(),        ETL::infinity());
      CHECK_EQUAL(STD::is_bounded,        ETL::is_bounded);
      CHECK_EQUAL(STD::is_exact,          ETL::is_exact);
      CHECK_EQUAL(STD::is_iec559,         ETL::is_iec559);
      CHECK_EQUAL(STD::is_integer,        ETL::is_integer);
      CHECK_EQUAL(true,                   ETL::is_modulo);
      CHECK_EQUAL(STD::is_signed,         ETL::is_signed);
      CHECK_EQUAL(STD::is_specialized,    ETL::is_specialized);
      CHECK_EQUAL(STD::lowest(),          ETL::lowest());
      CHECK_EQUAL(STD::max(),             ETL::max());
      CHECK_EQUAL(STD::max_digits10,      ETL::max_digits10);
      CHECK_EQUAL(STD::max_exponent,      ETL::max_exponent);
      CHECK_EQUAL(STD::max_exponent10,    ETL::max_exponent10);
      CHECK_EQUAL(STD::min(),             ETL::min());
      CHECK_EQUAL(STD::min_exponent,      ETL::min_exponent);
      CHECK_EQUAL(STD::min_exponent10,    ETL::min_exponent10);
      CHECK_EQUAL(STD::quiet_NaN(),       ETL::quiet_NaN());
      CHECK_EQUAL(STD::radix,             ETL::radix);
      CHECK_EQUAL(STD::round_error(),     ETL::round_error());
      CHECK_EQUAL(int(STD::round_style),  int(ETL::round_style));
      CHECK_EQUAL(STD::signaling_NaN(),   ETL::signaling_NaN());
      CHECK_EQUAL(STD::tinyness_before,   ETL::tinyness_before);
      CHECK_EQUAL(false,                  ETL::traps);
    }

    //*************************************************************************
    TEST(test_long_long)
    {
      typedef etlstd::numeric_limits<long long> ETL;
      typedef std::numeric_limits<long long>    STD;

      CHECK_EQUAL(STD::denorm_min(),      ETL::denorm_min());
      CHECK_EQUAL(STD::digits,            ETL::digits);
      CHECK_EQUAL(STD::digits10,          ETL::digits10);
      CHECK_EQUAL(STD::epsilon(),         ETL::epsilon());
      CHECK_EQUAL(int(STD::has_denorm),   int(ETL::has_denorm));
      CHECK_EQUAL(STD::has_denorm_loss,   ETL::has_denorm_loss);
      CHECK_EQUAL(STD::has_infinity,      ETL::has_infinity);
      CHECK_EQUAL(STD::has_quiet_NaN,     ETL::has_quiet_NaN);
      CHECK_EQUAL(STD::has_signaling_NaN, ETL::has_signaling_NaN);
      CHECK_EQUAL(STD::infinity(),        ETL::infinity());
      CHECK_EQUAL(STD::is_bounded,        ETL::is_bounded);
      CHECK_EQUAL(STD::is_exact,          ETL::is_exact);
      CHECK_EQUAL(STD::is_iec559,         ETL::is_iec559);
      CHECK_EQUAL(STD::is_integer,        ETL::is_integer);
      CHECK_EQUAL(false,                  ETL::is_modulo);
      CHECK_EQUAL(STD::is_signed,         ETL::is_signed);
      CHECK_EQUAL(STD::is_specialized,    ETL::is_specialized);
      CHECK_EQUAL(STD::lowest(),          ETL::lowest());
      CHECK_EQUAL(STD::max(),             ETL::max());
      CHECK_EQUAL(STD::max_digits10,      ETL::max_digits10);
      CHECK_EQUAL(STD::max_exponent,      ETL::max_exponent);
      CHECK_EQUAL(STD::max_exponent10,    ETL::max_exponent10);
      CHECK_EQUAL(STD::min(),             ETL::min());
      CHECK_EQUAL(STD::min_exponent,      ETL::min_exponent);
      CHECK_EQUAL(STD::min_exponent10,    ETL::min_exponent10);
      CHECK_EQUAL(STD::quiet_NaN(),       ETL::quiet_NaN());
      CHECK_EQUAL(STD::radix,             ETL::radix);
      CHECK_EQUAL(STD::round_error(),     ETL::round_error());
      CHECK_EQUAL(int(STD::round_style),  int(ETL::round_style));
      CHECK_EQUAL(STD::signaling_NaN(),   ETL::signaling_NaN());
      CHECK_EQUAL(STD::tinyness_before,   ETL::tinyness_before);
      CHECK_EQUAL(false,                  ETL::traps);
    }

    //*************************************************************************
    TEST(test_unsigned_long_long)
    {
      typedef etlstd::numeric_limits<unsigned long long> ETL;
      typedef std::numeric_limits<unsigned long long>    STD;

      CHECK_EQUAL(STD::denorm_min(),      ETL::denorm_min());
      CHECK_EQUAL(STD::digits,            ETL::digits);
      CHECK_EQUAL(STD::digits10,          ETL::digits10);
      CHECK_EQUAL(STD::epsilon(),         ETL::epsilon());
      CHECK_EQUAL(int(STD::has_denorm),   int(ETL::has_denorm));
      CHECK_EQUAL(STD::has_denorm_loss,   ETL::has_denorm_loss);
      CHECK_EQUAL(STD::has_infinity,      ETL::has_infinity);
      CHECK_EQUAL(STD::has_quiet_NaN,     ETL::has_quiet_NaN);
      CHECK_EQUAL(STD::has_signaling_NaN, ETL::has_signaling_NaN);
      CHECK_EQUAL(STD::infinity(),        ETL::infinity());
      CHECK_EQUAL(STD::is_bounded,        ETL::is_bounded);
      CHECK_EQUAL(STD::is_exact,          ETL::is_exact);
      CHECK_EQUAL(STD::is_iec559,         ETL::is_iec559);
      CHECK_EQUAL(STD::is_integer,        ETL::is_integer);
      CHECK_EQUAL(true,                   ETL::is_modulo);
      CHECK_EQUAL(STD::is_signed,         ETL::is_signed);
      CHECK_EQUAL(STD::is_specialized,    ETL::is_specialized);
      CHECK_EQUAL(STD::lowest(),          ETL::lowest());
      CHECK_EQUAL(STD::max(),             ETL::max());
      CHECK_EQUAL(STD::max_digits10,      ETL::max_digits10);
      CHECK_EQUAL(STD::max_exponent,      ETL::max_exponent);
      CHECK_EQUAL(STD::max_exponent10,    ETL::max_exponent10);
      CHECK_EQUAL(STD::min(),             ETL::min());
      CHECK_EQUAL(STD::min_exponent,      ETL::min_exponent);
      CHECK_EQUAL(STD::min_exponent10,    ETL::min_exponent10);
      CHECK_EQUAL(STD::quiet_NaN(),       ETL::quiet_NaN());
      CHECK_EQUAL(STD::radix,             ETL::radix);
      CHECK_EQUAL(STD::round_error(),     ETL::round_error());
      CHECK_EQUAL(int(STD::round_style),  int(ETL::round_style));
      CHECK_EQUAL(STD::signaling_NaN(),   ETL::signaling_NaN());
      CHECK_EQUAL(STD::tinyness_before,   ETL::tinyness_before);
      CHECK_EQUAL(false,                  ETL::traps);
    }

    //*************************************************************************
    TEST(test_float)
    {
      typedef etlstd::numeric_limits<float> ETL;
      typedef std::numeric_limits<float>    STD;

      CHECK_EQUAL(STD::digits,            ETL::digits);
      CHECK_EQUAL(STD::digits10,          ETL::digits10);
      CHECK_EQUAL(STD::epsilon(),         ETL::epsilon());
      CHECK_EQUAL(int(STD::has_denorm),   int(ETL::has_denorm));
      CHECK_EQUAL(true,                   ETL::has_denorm_loss);
      CHECK_EQUAL(false,                  ETL::has_infinity);
      CHECK_EQUAL(false,                  ETL::has_quiet_NaN);
      CHECK_EQUAL(false,                  ETL::has_signaling_NaN);
      CHECK_EQUAL(STD::is_bounded,        ETL::is_bounded);
      CHECK_EQUAL(STD::is_exact,          ETL::is_exact);
      CHECK_EQUAL(STD::is_iec559,         ETL::is_iec559);
      CHECK_EQUAL(STD::is_integer,        ETL::is_integer);
      CHECK_EQUAL(false,                  ETL::is_modulo);
      CHECK_EQUAL(STD::is_signed,         ETL::is_signed);
      CHECK_EQUAL(STD::is_specialized,    ETL::is_specialized);
      CHECK_EQUAL(STD::lowest(),          ETL::lowest());
      CHECK_EQUAL(STD::max(),             ETL::max());
      CHECK_EQUAL(STD::max_digits10,      ETL::max_digits10);
      CHECK_EQUAL(STD::max_exponent,      ETL::max_exponent);
      CHECK_EQUAL(STD::max_exponent10,    ETL::max_exponent10);
      CHECK_EQUAL(STD::min(),             ETL::min());
      CHECK_EQUAL(STD::min_exponent,      ETL::min_exponent);
      CHECK_EQUAL(STD::min_exponent10,    ETL::min_exponent10);
      CHECK_EQUAL(STD::radix,             ETL::radix);
      CHECK_EQUAL(STD::round_error(),     ETL::round_error());
      CHECK_EQUAL(int(STD::round_style),  int(ETL::round_style));
      CHECK_EQUAL(true,                   ETL::tinyness_before);
      CHECK_EQUAL(false,                  ETL::traps);

      // CHECK_EQUAL(STD::denorm_min(),      ETL::denorm_min());
      // CHECK_EQUAL(STD::signaling_NaN(),   ETL::signaling_NaN());
      // CHECK_EQUAL(STD::quiet_NaN(),       ETL::quiet_NaN());
      // CHECK_EQUAL(STD::infinity(),        ETL::infinity());
    }

    //*************************************************************************
    TEST(test_double)
    {
      typedef etlstd::numeric_limits<double> ETL;
      typedef std::numeric_limits<double>    STD;

      CHECK_EQUAL(STD::digits,            ETL::digits);
      CHECK_EQUAL(STD::digits10,          ETL::digits10);
      CHECK_EQUAL(STD::epsilon(),         ETL::epsilon());
      CHECK_EQUAL(int(STD::has_denorm),   int(ETL::has_denorm));
      CHECK_EQUAL(true,                   ETL::has_denorm_loss);
      CHECK_EQUAL(false,                  ETL::has_infinity);
      CHECK_EQUAL(false,                  ETL::has_quiet_NaN);
      CHECK_EQUAL(false,                  ETL::has_signaling_NaN);
      CHECK_EQUAL(STD::is_bounded,        ETL::is_bounded);
      CHECK_EQUAL(STD::is_exact,          ETL::is_exact);
      CHECK_EQUAL(STD::is_iec559,         ETL::is_iec559);
      CHECK_EQUAL(STD::is_integer,        ETL::is_integer);
      CHECK_EQUAL(false,                  ETL::is_modulo);
      CHECK_EQUAL(STD::is_signed,         ETL::is_signed);
      CHECK_EQUAL(STD::is_specialized,    ETL::is_specialized);
      CHECK_EQUAL(STD::lowest(),          ETL::lowest());
      CHECK_EQUAL(STD::max(),             ETL::max());
      CHECK_EQUAL(STD::max_digits10,      ETL::max_digits10);
      CHECK_EQUAL(STD::max_exponent,      ETL::max_exponent);
      CHECK_EQUAL(STD::max_exponent10,    ETL::max_exponent10);
      CHECK_EQUAL(STD::min(),             ETL::min());
      CHECK_EQUAL(STD::min_exponent,      ETL::min_exponent);
      CHECK_EQUAL(STD::min_exponent10,    ETL::min_exponent10);
      CHECK_EQUAL(STD::radix,             ETL::radix);
      CHECK_EQUAL(STD::round_error(),     ETL::round_error());
      CHECK_EQUAL(int(STD::round_style),  int(ETL::round_style));
      CHECK_EQUAL(true,                   ETL::tinyness_before);
      CHECK_EQUAL(false,                  ETL::traps);

      // CHECK_EQUAL(STD::denorm_min(),      ETL::denorm_min());
      // CHECK_EQUAL(STD::signaling_NaN(),   ETL::signaling_NaN());
      // CHECK_EQUAL(STD::quiet_NaN(),       ETL::quiet_NaN());
      // CHECK_EQUAL(STD::infinity(),        ETL::infinity());
    }

    //*************************************************************************
    TEST(test_long_double)
    {
      typedef etlstd::numeric_limits<long double> ETL;
      typedef std::numeric_limits<long double>    STD;

      CHECK_EQUAL(STD::digits,            ETL::digits);
      CHECK_EQUAL(STD::digits10,          ETL::digits10);
      CHECK_EQUAL(STD::epsilon(),         ETL::epsilon());
      CHECK_EQUAL(int(STD::has_denorm),   int(ETL::has_denorm));
      CHECK_EQUAL(true,                   ETL::has_denorm_loss);
      CHECK_EQUAL(false,                  ETL::has_infinity);
      CHECK_EQUAL(false,                  ETL::has_quiet_NaN);
      CHECK_EQUAL(false,                  ETL::has_signaling_NaN);
      CHECK_EQUAL(STD::is_bounded,        ETL::is_bounded);
      CHECK_EQUAL(STD::is_exact,          ETL::is_exact);
      CHECK_EQUAL(STD::is_iec559,         ETL::is_iec559);
      CHECK_EQUAL(STD::is_integer,        ETL::is_integer);
      CHECK_EQUAL(false,                  ETL::is_modulo);
      CHECK_EQUAL(STD::is_signed,         ETL::is_signed);
      CHECK_EQUAL(STD::is_specialized,    ETL::is_specialized);
      CHECK_EQUAL(STD::lowest(),          ETL::lowest());
      CHECK_EQUAL(STD::max(),             ETL::max());
      CHECK_EQUAL(STD::max_digits10,      ETL::max_digits10);
      CHECK_EQUAL(STD::max_exponent,      ETL::max_exponent);
      CHECK_EQUAL(STD::max_exponent10,    ETL::max_exponent10);
      CHECK_EQUAL(STD::min(),             ETL::min());
      CHECK_EQUAL(STD::min_exponent,      ETL::min_exponent);
      CHECK_EQUAL(STD::min_exponent10,    ETL::min_exponent10);
      CHECK_EQUAL(STD::radix,             ETL::radix);
      CHECK_EQUAL(STD::round_error(),     ETL::round_error());
      CHECK_EQUAL(int(STD::round_style),  int(ETL::round_style));
      CHECK_EQUAL(true,                   ETL::tinyness_before);
      CHECK_EQUAL(false,                  ETL::traps);

      // CHECK_EQUAL(STD::denorm_min(),      ETL::denorm_min());
      // CHECK_EQUAL(STD::signaling_NaN(),   ETL::signaling_NaN());
      // CHECK_EQUAL(STD::quiet_NaN(),       ETL::quiet_NaN());
      // CHECK_EQUAL(STD::infinity(),        ETL::infinity());
    }
  };
}
