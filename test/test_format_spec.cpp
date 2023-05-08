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

#include "etl/format_spec.h"
#include "etl/wformat_spec.h"
#include "etl/u16format_spec.h"
#include "etl/u32format_spec.h"

namespace
{
  SUITE(test_format_spec)
  {
    //*************************************************************************
    TEST(test_default_format)
    {
      etl::format_spec format;

      CHECK_EQUAL(10,    format.get_base());
      CHECK_EQUAL(' ',   format.get_fill());
      CHECK_EQUAL(0,     format.get_precision());
      CHECK_EQUAL(0,     format.get_width());
      CHECK_EQUAL(false, format.is_boolalpha());
      CHECK_EQUAL(false, format.is_left());
      CHECK_EQUAL(true,  format.is_right());
      CHECK_EQUAL(false, format.is_show_base());
      CHECK_EQUAL(false, format.is_upper_case());
    }

    //*************************************************************************
    TEST(test_constexpr_parameterised_format)
    {
      constexpr etl::format_spec format(16U, 4, 6, true, false, true, false, '?');

      CHECK_EQUAL(16, format.get_base());
      CHECK_EQUAL('?', format.get_fill());
      CHECK_EQUAL(6, format.get_precision());
      CHECK_EQUAL(4, format.get_width());
      CHECK_EQUAL(true, format.is_boolalpha());
      CHECK_EQUAL(false, format.is_left());
      CHECK_EQUAL(true, format.is_right());
      CHECK_EQUAL(false, format.is_show_base());
      CHECK_EQUAL(true, format.is_upper_case());
    }

    //*************************************************************************
    TEST(test_format)
    {
      etl::format_spec format;

      format.base(16).boolalpha(true).fill('?').left().precision(6).show_base(true).upper_case(true).width(10);

      CHECK_EQUAL(16,    format.get_base());
      CHECK_EQUAL('?',   format.get_fill());
      CHECK_EQUAL(6,     format.get_precision());
      CHECK_EQUAL(10,    format.get_width());
      CHECK_EQUAL(true,  format.is_boolalpha());
      CHECK_EQUAL(true,  format.is_left());
      CHECK_EQUAL(false, format.is_right());
      CHECK_EQUAL(true,  format.is_show_base());
      CHECK_EQUAL(true,  format.is_upper_case());
    }

    //*************************************************************************
#if ETL_USING_CPP14
    TEST(test_format_constexpr)
    {
      constexpr etl::format_spec format = etl::format_spec().base(16).boolalpha(true).fill('?').left().precision(6).show_base(true).upper_case(true).width(10);

      constexpr int  base       = format.get_base();
      constexpr char fill       = format.get_fill();
      constexpr int  precision  = format.get_precision();
      constexpr int  width      = format.get_width();
      constexpr bool boolalpha  = format.is_boolalpha();
      constexpr bool left       = format.is_left();
      constexpr bool right      = format.is_right();
      constexpr bool show_base  = format.is_show_base();
      constexpr bool upper_case = format.is_upper_case();

      CHECK_EQUAL(16,    base);
      CHECK_EQUAL('?',   fill);
      CHECK_EQUAL(6,     precision);
      CHECK_EQUAL(10,    width);
      CHECK_EQUAL(true,  boolalpha);
      CHECK_EQUAL(true,  left);
      CHECK_EQUAL(false, right);
      CHECK_EQUAL(true,  show_base);
      CHECK_EQUAL(true,  upper_case);
    }
#endif
  };
}

