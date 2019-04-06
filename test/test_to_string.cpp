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

#include <string>

#include "etl/to_string.h"
#include "etl/cstring.h"
#include "etl/format_spec.h"

#undef STR
#define STR(x) x

namespace
{
  typedef etl::format_spec<etl::istring> Format;

  SUITE(test_string_char)
  {
    //*************************************************************************
    TEST(test_default_format_no_append)
    {
      etl::string<20> str;

      CHECK(etl::string<20>(STR("0"))                    == etl::to_string(uint8_t(0), str));
      CHECK(etl::string<20>(STR("0"))                    == etl::to_string(uint16_t(0), str));
      CHECK(etl::string<20>(STR("0"))                    == etl::to_string(uint32_t(0), str));
      CHECK(etl::string<20>(STR("0"))                    == etl::to_string(uint64_t(0), str));

      CHECK(etl::string<20>(STR("128"))                  == etl::to_string(uint8_t(128), str));
      CHECK(etl::string<20>(STR("32768"))                == etl::to_string(uint16_t(32768), str));
      CHECK(etl::string<20>(STR("2147483648"))           == etl::to_string(uint32_t(2147483648ul), str));
      CHECK(etl::string<20>(STR("9223372036854775808"))  == etl::to_string(uint64_t(9223372036854775808ull), str));

      CHECK(etl::string<20>(STR("127"))                  == etl::to_string(int8_t(127), str));
      CHECK(etl::string<20>(STR("32767"))                == etl::to_string(int16_t(32767), str));
      CHECK(etl::string<20>(STR("2147483647"))           == etl::to_string(int32_t(2147483647ll), str));
      CHECK(etl::string<20>(STR("9223372036854775807"))  == etl::to_string(int64_t(9223372036854775807ll), str));

      CHECK(etl::string<20>(STR("-128"))                 == etl::to_string(int8_t(-128), str));
      CHECK(etl::string<20>(STR("-32768"))               == etl::to_string(int16_t(-32768), str));
      CHECK(etl::string<20>(STR("-2147483648"))          == etl::to_string(int32_t(-2147483648ll), str));
      CHECK(etl::string<20>(STR("-9223372036854775808")) == etl::to_string(int64_t(-9223372036854775808ll), str));
    }
  };
}
