/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl
https://www.etlcpp.com

Copyright(c) 2020 jwellbelove

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

#include "UnitTest++/UnitTest++.h"

#include "etl/cstring.h"
#include "etl/string_view.h"
#include "etl/string_utilities.h"

#undef STR
#define STR(x) x

namespace
{
  SUITE(test_string_utilities_char)
  {
    static const size_t SIZE = 50;

    typedef etl::string<SIZE> String;
    typedef etl::istring      IString;
    typedef etl::string_view  StringView;

    //*************************************************************************
    TEST(test_trim_whitespace_left_empty)
    {
      String text(STR(""));
      String expected(STR(""));

      etl::trim_whitespace_left(text);

      CHECK(expected == text);
    }

    //*************************************************************************
    TEST(test_view_trim_whitespace_left_empty)
    {
      String text(STR(""));
      String expected(STR(""));

      StringView textview(text);
      StringView expectedview(expected);

      StringView view = etl::trim_whitespace_left(textview);

      CHECK(expectedview == view);
    }

    //*************************************************************************
    TEST(test_trim_whitespace_left)
    {
      String text(STR(" \t\n\r\f\vHello World\t\n\r\f\v "));
      String expected(STR("Hello World\t\n\r\f\v "));

      etl::trim_whitespace_left(text);

      CHECK(expected == text);
    }

    //*************************************************************************
    TEST(test_view_trim_whitespace_left)
    {
      String text(STR(" \t\n\r\f\vHello World\t\n\r\f\v "));
      String expected(STR("Hello World\t\n\r\f\v "));

      StringView textview(text);
      StringView expectedview(expected);

      StringView view = etl::trim_whitespace_left(textview);

      CHECK(expectedview == view);
    }

    //*************************************************************************
    TEST(test_trim_whitespace_left_nothing_to_trim)
    {
      String text(STR("Hello World\t\n\r\f\v "));
      String expected(STR("Hello World\t\n\r\f\v "));

      etl::trim_whitespace_left(text);

      CHECK(expected == text);
    }

    //*************************************************************************
    TEST(test_view_trim_whitespace_left_nothing_to_trim)
    {
      String text(STR("Hello World\t\n\r\f\v "));
      String expected(STR("Hello World\t\n\r\f\v "));

      StringView textview(text);
      StringView expectedview(expected);

      StringView view = etl::trim_whitespace_left(textview);

      CHECK(expectedview == view);
    }

    //*************************************************************************
    TEST(test_trim_from_left_empty)
    {
      String text(STR(""));
      String expected(STR(""));

      etl::trim_from_left(text, STR(" \t\n\r\f\v"));

      CHECK(expected == text);
    }

    //*************************************************************************
    TEST(test_view_trim_from_left_empty)
    {
      String text(STR(""));
      String expected(STR(""));

      StringView textview(text);
      StringView expectedview(expected);

      StringView view = etl::trim_from_left(textview, STR(" \t\n\r\f\v"));

      CHECK(expectedview == view);
    }

    //*************************************************************************
    TEST(test_trim_from_left)
    {
      String text(STR(" \t\n\r\f\vHello World\t\n\r\f\v "));
      String expected(STR("Hello World\t\n\r\f\v "));

      etl::trim_from_left(text, STR(" \t\n\r\f\v"));

      CHECK(expected == text);
    }

    //*************************************************************************
    TEST(test_view_trim_from_left)
    {
      String text(STR(" \t\n\r\f\vHello World\t\n\r\f\v "));
      String expected(STR("Hello World\t\n\r\f\v "));

      StringView textview(text);
      StringView expectedview(expected);

      StringView view = etl::trim_from_left(textview, STR(" \t\n\r\f\v"));

      CHECK(expectedview == view);
    }

    //*************************************************************************
    TEST(test_trim_from_left_nothing_to_trim)
    {
      String text(STR("Hello World\t\n\r\f\v "));
      String expected(STR("Hello World\t\n\r\f\v "));

      etl::trim_from_left(text, STR(" \t\n\r\f\v"));

      CHECK(expected == text);
    }

    //*************************************************************************
    TEST(test_view_trim_from_left_nothing_to_trim)
    {
      String text(STR("Hello World\t\n\r\f\v "));
      String expected(STR("Hello World\t\n\r\f\v "));

      StringView textview(text);
      StringView expectedview(expected);

      StringView view = etl::trim_from_left(textview, STR(" \t\n\r\f\v"));

      CHECK(expectedview == view);
    }

    //*************************************************************************
    TEST(test_trim_left_pointer_delimiters)
    {
      String text(STR("qztfpHello Worldqztfp"));
      String expected(STR("Hello Worldqztfp"));

      etl::trim_left(text, STR("Hel"));

      CHECK(expected == text);
    }

    //*************************************************************************
    TEST(test_view_trim_left_pointer_delimiters)
    {
      String text(STR("qztfpHello Worldqztfp"));
      String expected(STR("Hello Worldqztfp"));

      StringView textview(text);
      StringView expectedview(expected);

      StringView view = etl::trim_left(textview, STR("Hel"));

      CHECK(expectedview == view);
    }

    //*************************************************************************
    TEST(test_trim_left_pointer_delimiters_nothing_to_trim)
    {
      String text(STR("Hello Worldqztfp"));
      String expected(STR("Hello Worldqztfp"));

      etl::trim_left(text, STR("Hel"));

      CHECK(expected == text);
    }

    //*************************************************************************
    TEST(test_view_trim_left_pointer_delimiters_nothing_to_trim)
    {
      String text(STR("Hello Worldqztfp"));
      String expected(STR("Hello Worldqztfp"));

      StringView textview(text);
      StringView expectedview(expected);

      StringView view = etl::trim_left(textview, STR("Hel"));

      CHECK(expectedview == view);
    }

    //*************************************************************************
    TEST(test_trim_left_string_delimiters)
    {
      String text(STR("qztfpHello Worldqztfp"));
      String expected("Hello Worldqztfp");

      etl::trim_left(text, String(STR("Hel")));

      CHECK(expected == text);
    }

    //*************************************************************************
    TEST(test_view_trim_left_string_delimiters)
    {
      String text(STR("qztfpHello Worldqztfp"));
      String expected("Hello Worldqztfp");

      StringView textview(text);
      StringView expectedview(expected);

      StringView view = etl::trim_left(textview, STR("Hel"));

      CHECK(expectedview == view);
    }

    //*************************************************************************
    TEST(test_trim_left_string_delimiters_nothing_to_trim)
    {
      String text(STR("Hello Worldqztfp"));
      String expected("Hello Worldqztfp");

      etl::trim_left(text, String(STR("Hel")));

      CHECK(expected == text);
    }

    //*************************************************************************
    TEST(test_view_trim_left_string_delimiters_nothing_to_trim)
    {
      String text(STR("Hello Worldqztfp"));
      String expected("Hello Worldqztfp");

      StringView textview(text);
      StringView expectedview(expected);

      StringView view = etl::trim_left(textview, String(STR("Hel")));

      CHECK(expectedview == view);
    }

    //*************************************************************************
    TEST(test_trim_whitespace_right_empty)
    {
      String text(STR(""));
      String expected(STR(""));

      etl::trim_whitespace_right(text);

      CHECK(expected == text);
    }

    //*************************************************************************
    TEST(test_view_trim_whitespace_right_empty)
    {
      String text(STR(""));
      String expected(STR(""));

      StringView textview(text);
      StringView expectedview(expected);

      StringView view = etl::trim_whitespace_right(textview);

      CHECK(expectedview == view);
    }

    //*************************************************************************
    TEST(test_trim_whitespace_right)
    {
      String text(STR(" \t\n\r\f\vHello World\t\n\r\f\v "));
      String expected(STR(" \t\n\r\f\vHello World"));

      etl::trim_whitespace_right(text);

      CHECK(expected == text);
    }

    //*************************************************************************
    TEST(test_view_trim_whitespace_right)
    {
      String text(STR(" \t\n\r\f\vHello World\t\n\r\f\v "));
      String expected(STR(" \t\n\r\f\vHello World"));

      StringView textview(text);
      StringView expectedview(expected);

      StringView view = etl::trim_whitespace_right(textview);

      CHECK(expectedview == view);
    }

    //*************************************************************************
    TEST(test_trim_whitespace_right_nothing_to_trim)
    {
      String text(STR(" \t\n\r\f\vHello World"));
      String expected(STR(" \t\n\r\f\vHello World"));

      etl::trim_whitespace_right(text);

      CHECK(expected == text);
    }

    //*************************************************************************
    TEST(test_view_trim_whitespace_right_nothing_to_trim)
    {
      String text(STR(" \t\n\r\f\vHello World"));
      String expected(STR(" \t\n\r\f\vHello World"));

      StringView textview(text);
      StringView expectedview(expected);

      StringView view = etl::trim_whitespace_right(textview);

      CHECK(expectedview == view);
    }

    //*************************************************************************
    TEST(test_trim_from_right_empty)
    {
      String text(STR(""));
      String expected(STR(""));

      etl::trim_from_right(text, STR(" \t\n\r\f\v"));

      CHECK(expected == text);
    }

    //*************************************************************************
    TEST(test_view_trim_from_right_empty)
    {
      String text(STR(""));
      String expected(STR(""));

      StringView textview(text);
      StringView expectedview(expected);

      StringView view = etl::trim_from_right(textview, STR(" \t\n\r\f\v"));

      CHECK(expectedview == view);
    }

    //*************************************************************************
    TEST(test_trim_from_right)
    {
      String text(STR(" \t\n\r\f\vHello World\t\n\r\f\v "));
      String expected(STR(" \t\n\r\f\vHello World"));

      etl::trim_from_right(text, STR(" \t\n\r\f\v"));

      CHECK(expected == text);
    }

    //*************************************************************************
    TEST(test_view_trim_from_right)
    {
      String text(STR(" \t\n\r\f\vHello World\t\n\r\f\v "));
      String expected(STR(" \t\n\r\f\vHello World"));

      StringView textview(text);
      StringView expectedview(expected);

      StringView view = etl::trim_from_right(textview, STR(" \t\n\r\f\v"));

      CHECK(expectedview == view);
    }

    //*************************************************************************
    TEST(test_trim_from_right_nothing_to_trim)
    {
      String text(STR(" \t\n\r\f\vHello World"));
      String expected(STR(" \t\n\r\f\vHello World"));

      etl::trim_from_right(text, STR(" \t\n\r\f\v"));

      CHECK(expected == text);
    }

    //*************************************************************************
    TEST(test_view_trim_from_right_nothing_to_trim)
    {
      String text(STR(" \t\n\r\f\vHello World"));
      String expected(STR(" \t\n\r\f\vHello World"));

      StringView textview(text);
      StringView expectedview(expected);

      StringView view = etl::trim_from_right(textview, STR(" \t\n\r\f\v"));

      CHECK(expectedview == view);
    }

    //*************************************************************************
    TEST(test_trim_right_pointer_delimiters)
    {
      String text(STR("qztfpHello Worldqztfp"));
      String expected(STR("qztfpHello World"));

      etl::trim_right(text, STR("rld"));

      CHECK(expected == text);
    }

    //*************************************************************************
    TEST(test_view_trim_right_pointer_delimiters)
    {
      String text(STR("qztfpHello Worldqztfp"));
      String expected(STR("qztfpHello World"));

      StringView textview(text);
      StringView expectedview(expected);

      StringView view = etl::trim_right(textview, STR("rld"));

      CHECK(expectedview == view);
    }

    //*************************************************************************
    TEST(test_trim_right_pointer_delimiters_nothing_to_trim)
    {
      String text(STR("qztfpHello World"));
      String expected(STR("qztfpHello World"));

      etl::trim_right(text, STR("rld"));

      CHECK(expected == text);
    }

    //*************************************************************************
    TEST(test_view_trim_right_pointer_delimiters_nothing_to_trim)
    {
      String text(STR("qztfpHello World"));
      String expected(STR("qztfpHello World"));

      StringView textview(text);
      StringView expectedview(expected);

      StringView view = etl::trim_right(textview, STR("rld"));

      CHECK(expectedview == view);
    }

    //*************************************************************************
    TEST(test_trim_right_string_delimiters)
    {
      String text(STR("qztfpHello Worldqztfp"));
      String expected("qztfpHello World");

      etl::trim_right(text, String(STR("rld")));

      CHECK(expected == text);
    }

    //*************************************************************************
    TEST(test_view_trim_right_string_delimiters)
    {
      String text(STR("qztfpHello Worldqztfp"));
      String expected("qztfpHello World");

      StringView textview(text);
      StringView expectedview(expected);

      StringView view = etl::trim_right(textview, STR("rld"));

      CHECK(expectedview == view);
    }

    //*************************************************************************
    TEST(test_trim_right_string_delimiters_nothing_to_trim)
    {
      String text(STR("qztfpHello World"));
      String expected("qztfpHello World");

      etl::trim_right(text, String(STR("rld")));

      CHECK(expected == text);
    }

    //*************************************************************************
    TEST(test_view_trim_right_string_delimiters_nothing_to_trim)
    {
      String text(STR("qztfpHello World"));
      String expected("qztfpHello World");

      StringView textview(text);
      StringView expectedview(expected);

      StringView view = etl::trim_right(textview, String(STR("rld")));

      CHECK(expectedview == view);
    }

    //*************************************************************************
    TEST(test_trim_whitespace_empty)
    {
      String text(STR(""));
      String expected(STR(""));

      etl::trim_whitespace(text);

      CHECK(expected == text);
    }

    //*************************************************************************
    TEST(test_view_trim_whitespace_empty)
    {
      String text(STR(""));
      String expected(STR(""));

      StringView textview(text);
      StringView expectedview(expected);

      StringView view = etl::trim_whitespace(textview);

      CHECK(expectedview == view);
    }

    //*************************************************************************
    TEST(test_trim_whitespace)
    {
      String text(STR(" \t\n\r\f\vHello World\t\n\r\f\v "));
      String expected(STR("Hello World"));

      etl::trim_whitespace(text);

      CHECK(expected == text);
    }

    //*************************************************************************
    TEST(test_view_trim_whitespace)
    {
      String text(STR(" \t\n\r\f\vHello World\t\n\r\f\v "));
      String expected(STR("Hello World"));

      StringView textview(text);
      StringView expectedview(expected);

      StringView view = etl::trim_whitespace(textview);

      CHECK(expectedview == view);
    }

    //*************************************************************************
    TEST(test_trim_whitespace_nothing_to_trim)
    {
      String text(STR("Hello World"));
      String expected(STR("Hello World"));

      etl::trim_whitespace(text);

      CHECK(expected == text);
    }

    //*************************************************************************
    TEST(test_view_trim_whitespace_nothing_to_trim)
    {
      String text(STR("Hello World"));
      String expected(STR("Hello World"));

      StringView textview(text);
      StringView expectedview(expected);

      StringView view = etl::trim_whitespace(textview);

      CHECK(expectedview == view);
    }

    //*************************************************************************
    TEST(test_trim_from_empty)
    {
      String text(STR(""));
      String expected(STR(""));

      etl::trim_from(text, STR(" \t\n\r\f\v"));

      CHECK(expected == text);
    }

    //*************************************************************************
    TEST(test_view_trim_from_empty)
    {
      String text(STR(""));
      String expected(STR(""));

      StringView textview(text);
      StringView expectedview(expected);

      StringView view = etl::trim_from(textview, STR(" \t\n\r\f\v"));

      CHECK(expectedview == view);
    }

    //*************************************************************************
    TEST(test_trim_from)
    {
      String text(STR(" \t\n\r\f\vHello World\t\n\r\f\v "));
      String expected(STR("Hello World"));

      etl::trim_from(text, STR(" \t\n\r\f\v"));

      CHECK(expected == text);
    }

    //*************************************************************************
    TEST(test_view_trim_from)
    {
      String text(STR(" \t\n\r\f\vHello World\t\n\r\f\v "));
      String expected(STR("Hello World"));

      StringView textview(text);
      StringView expectedview(expected);

      StringView view = etl::trim_from(textview, STR(" \t\n\r\f\v"));

      CHECK(expectedview == view);
    }

    //*************************************************************************
    TEST(test_trim_from_nothing_to_trim)
    {
      String text(STR("Hello World"));
      String expected(STR("Hello World"));

      etl::trim_from(text, STR(" \t\n\r\f\v"));

      CHECK(expected == text);
    }

    //*************************************************************************
    TEST(test_view_trim_from_nothing_to_trim)
    {
      String text(STR("Hello World"));
      String expected(STR("Hello World"));

      StringView textview(text);
      StringView expectedview(expected);

      StringView view = etl::trim_from(textview, STR(" \t\n\r\f\v"));

      CHECK(expectedview == view);
    }

    //*************************************************************************
    TEST(test_trim_pointer_delimiters)
    {
      String text(STR("qztfpHello Worldqztfp"));
      String expected(STR("Hello World"));

      etl::trim(text, STR("Hd"));

      CHECK(expected == text);
    }

    //*************************************************************************
    TEST(test_view_trim_pointer_delimiters)
    {
      String text(STR("qztfpHello Worldqztfp"));
      String expected(STR("Hello World"));

      StringView textview(text);
      StringView expectedview(expected);

      StringView view = etl::trim(textview, STR("Hd"));

      CHECK(expectedview == view);
    }

    //*************************************************************************
    TEST(test_trim_pointer_delimiters_nothing_to_trim)
    {
      String text(STR("Hello World"));
      String expected(STR("Hello World"));

      etl::trim(text, STR("Hd"));

      CHECK(expected == text);
    }

    //*************************************************************************
    TEST(test_view_trim_pointer_delimiters_nothing_to_trim)
    {
      String text(STR("Hello World"));
      String expected(STR("Hello World"));

      StringView textview(text);
      StringView expectedview(expected);

      StringView view = etl::trim(textview, STR("Hd"));

      CHECK(expectedview == view);
    }

    //*************************************************************************
    TEST(test_trim_string_delimiters)
    {
      String text(STR("qztfpHello Worldqztfp"));
      String expected("Hello World");

      etl::trim(text, String(STR("Hd")));

      CHECK(expected == text);
    }

    //*************************************************************************
    TEST(test_view_trim_string_delimiters)
    {
      String text(STR("qztfpHello Worldqztfp"));
      String expected("Hello World");

      StringView textview(text);
      StringView expectedview(expected);

      StringView view = etl::trim(textview, STR("Hd"));

      CHECK(expectedview == view);
    }

    //*************************************************************************
    TEST(test_trim_string_delimiters_nothing_to_trim)
    {
      String text(STR("Hello World"));
      String expected("Hello World");

      etl::trim(text, String(STR("Hd")));

      CHECK(expected == text);
    }

    //*************************************************************************
    TEST(test_view_trim_string_delimiters_nothing_to_trim)
    {
      String text(STR("Hello World"));
      String expected("Hello World");

      StringView textview(text);
      StringView expectedview(expected);

      StringView view = etl::trim(textview, String(STR("Hd")));

      CHECK(expectedview == view);
    }

    //*************************************************************************
    TEST(test_reverse)
    {
      String text(STR("Hello World"));
      String expected(STR("dlroW olleH"));

      etl::reverse(text);

      CHECK(expected == text);
    }
  };
}
