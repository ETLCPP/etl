/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl
https://www.etlcpp.com

Copyright(c) 2023 John Wellbelove

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

#include "etl/platform.h"
#if ETL_USING_CPP20

#include "unit_test_framework.h"

#include <ostream>
#include <sstream>
#include <iomanip>

#include "etl/u8string_stream.h"
#include "etl/u8string.h"
#include "etl/format_spec.h"

#undef STR
#define STR(x) u8##x

namespace
{
  using String = etl::u8string<50>;
  using IString = etl::iu8string;
  using Stream = etl::u8string_stream;
  using Format = etl::u8format_spec;

  //***********************************
  struct Custom
  {
    int x;
    int y;
  };

  //***********************************
  Stream& operator <<(Stream& ss, const Custom& value)
  {
    ss << STR("X = ") << value.x << STR(" : Y = ") << value.y;
    return ss;
  }
}

namespace etl
{
  //***********************************
  std::ostream& operator << (std::ostream& os, const IString& str)
  {
    for (auto c : str)
    {
      os << uint16_t(c);
    }

    return os;
  }
}

namespace
{
  SUITE(test_string_stream)
  {
    //*************************************************************************
    TEST(test_default_format_from_const_char)
    {
      String str;

      Stream ss(str);

      int value = 123;
      String hello(STR("Hello"));
      ss << hello << STR(" World ") << value;

      String result = ss.str();

      CHECK_EQUAL(String(STR("Hello World 123")), result);
    }

    //*************************************************************************
    TEST(test_default_format_from_char)
    {
      String str;

      Stream ss(str);

      int value = 123;
      String hello(STR("Hello"));
      ss << hello << const_cast<char8_t*>(STR(" World ")) << value;

      String result = ss.str();

      CHECK_EQUAL(String(STR("Hello World 123")), result);
    }

    //*************************************************************************
    TEST(test_custom_format)
    {
      String str;
      Format format = Format().base(10).width(10).fill(STR('#'));

      Stream ss(str);

      int value = 123;
      String hello(STR("Hello"));
      ss << format << hello << STR("World") << value;

      String result = ss.str();

      CHECK_EQUAL(String(STR("#####Hello#####World#######123")), result);
    }

    //*************************************************************************
    TEST(test_custom_multi_format)
    {
      String str;
      Format format1 = Format().base(10).width(10).fill(STR('#'));
      Format format2 = Format().base(10).width(8).fill(STR('*')).left();
      Format format3 = Format().base(16).width(4).fill(STR(' ')).right();

      Stream ss(str);

      int value = 123;
      String hello(STR("Hello"));
      ss << format1 << hello << format2 << STR("World") << format3 << value;

      String result = ss.str();

      CHECK_EQUAL(String(STR("#####HelloWorld***  7b")), result);
    }

    //*************************************************************************
    TEST(test_custom_inline_format)
    {
      String str;
      Stream ss(str);

      int value = 123456;

      ss << etl::setw(20) << etl::setfill(STR('-')) << etl::left;

      ss << etl::bin << value;
      CHECK_EQUAL(String(STR("11110001001000000---")), ss.str());

      ss.str().clear();
      ss << etl::oct << value;
      CHECK_EQUAL(String(STR("361100--------------")), ss.str());

      ss.str().clear();
      ss << etl::dec << value;
      CHECK_EQUAL(String(STR("123456--------------")), ss.str());

      ss.str().clear();
      ss << etl::hex << etl::uppercase << value;
      CHECK_EQUAL(String(STR("1E240---------------")), ss.str());

      ss.str().clear();
      ss << etl::hex << etl::nouppercase << value;
      CHECK_EQUAL(String(STR("1e240---------------")), ss.str());

      ss.str().clear();
      ss << etl::setw(0);
      ss << etl::noboolalpha << false << STR(" ") << true << STR(" ") << etl::boolalpha << false << STR(" ") << true;
      CHECK_EQUAL(String(STR("0 1 false true")), ss.str());

      ss.str().clear();
      ss << etl::setprecision(4) << 3.1415927;
      CHECK_EQUAL(String(STR("3.1416")), ss.str());

      ss.str().clear();
      ss << STR("abcdef") << STR(" ") << etl::uppercase << STR("abcdef");
      CHECK_EQUAL(String(STR("abcdef abcdef")), ss.str());
    }

    //*************************************************************************
    TEST(test_custom_multi_inline_format)
    {
      String str;
      Stream ss(str);

      int value = 123;
      String hello(STR("Hello"));
      ss << etl::dec << etl::setw(10) << etl::setfill(STR('#')) << hello
         << etl::setw(8) << etl::setfill(STR('*')) << etl::left << STR("World")
         << etl::hex << etl::setw(4) << etl::setfill(STR(' ')) << etl::right << value;

      String result = ss.str();

      CHECK_EQUAL(String(STR("#####HelloWorld***  7b")), result);
    }

    //*************************************************************************
    TEST(test_custom_class_default_format)
    {
      String str;

      Stream ss(str);

      String value_str(STR("Value: "));
      IString& ivalue_str = value_str;
      Custom value = { 1, 2 };

      ss << ivalue_str << value;

      String result = ss.str();

      CHECK_EQUAL(String(STR("Value: X = 1 : Y = 2")), result);
    }

    //*************************************************************************
    TEST(test_get_format)
    {
      String str;
      Format format = Format().base(16).width(4).fill(STR(' ')).right();

      Stream ss(str, format);

      Format format2 = ss.get_format();

      CHECK(format == format2);
    }

    //*************************************************************************
    TEST(test_set_from_character_pointer)
    {
      String str;
      Stream ss(str);

      ss.str(STR("Hello"));

      CHECK_EQUAL(String(STR("Hello")), ss.str());
    }

    //*************************************************************************
    TEST(test_set_from_string)
    {
      String str;
      Stream ss(str);

      ss.str(String(STR("Hello")));

      CHECK_EQUAL(String(STR("Hello")), ss.str());
    }

    //*************************************************************************
    TEST(test_get_istring)
    {
      String str;
      Stream ss(str);

      ss.str(String(STR("Hello")));

      IString& istr = ss.str();
      istr.resize(istr.size() - 1);

      CHECK_EQUAL(String(STR("Hell")), istr);
    }

    //*************************************************************************
    TEST(test_get_const_istring)
    {
      String str;
      Stream ss(str);

      ss.str(String(STR("Hello")));

      const IString& istr = ss.str();

      CHECK_EQUAL(String(STR("Hello")), istr);
    }
  };
}

#endif
