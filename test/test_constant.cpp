/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl
https://www.etlcpp.com

Copyright(c) 2017 John Wellbelove

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

#include "etl/constant.h"
#include "etl/integral_limits.h"

#include <stdint.h>
#include <type_traits>

namespace
{
  SUITE(test_constant)
  {
    //*************************************************************************
    TEST(test_constants)
    {
      typedef etl::constant<char,   etl::integral_limits<char>::max>   C1;
      typedef etl::constant<uint32_t, etl::integral_limits<uint32_t>::max> C2;
      typedef etl::constant<int64_t,  etl::integral_limits<int64_t>::max>  C3;

      C1 c1;
      C2 c2;
      C3 c3;

      CHECK_EQUAL((char)etl::integral_limits<char>::max, c1.value);
      CHECK_EQUAL((char)etl::integral_limits<char>::max, C1::value);
      CHECK((etl::is_same<char, C1::value_type>::value));

      CHECK_EQUAL((uint32_t)etl::integral_limits<uint32_t>::max, c2.value);
      CHECK_EQUAL((uint32_t)etl::integral_limits<uint32_t>::max, C2::value);
      CHECK((etl::is_same<uint32_t, C2::value_type>::value));

      CHECK_EQUAL((int64_t)etl::integral_limits<int64_t>::max, c3.value);
      CHECK_EQUAL((int64_t)etl::integral_limits<int64_t>::max, C3::value);
      CHECK((etl::is_same<int64_t, C3::value_type>::value));
    }
  };
}
