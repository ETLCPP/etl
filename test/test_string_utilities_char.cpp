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
    TEST(test_trim_left_empty)
    {
      String text(STR(""));
      String expected(STR(""));

      etl::trim_left(text);

      CHECK(expected == text);
    }

    //*************************************************************************
    TEST(test_trim_left)
    {
      String text(STR(" \t\n\r\f\vHello World \t\n\r\f\v "));
      String expected(STR("Hello World \t\n\r\f\v "));

      etl::trim_left(text);

      CHECK(expected == text);
    }

    //*************************************************************************
    TEST(test_trim_left_nothing_to_trim)
    {
      String text(STR("Hello World \t\n\r\f\v "));
      String expected(STR("Hello World \t\n\r\f\v "));

      etl::trim_left(text);

      CHECK(expected == text);
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
    TEST(test_trim_left_pointer_delimiters_nothing_to_trim)
    {
      String text(STR("Hello Worldqztfp"));
      String expected(STR("Hello Worldqztfp"));

      etl::trim_left(text, STR("Hel"));

      CHECK(expected == text);
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
    TEST(test_trim_left_string_delimiters_nothing_to_trim)
    {
      String text(STR("Hello Worldqztfp"));
      String expected("Hello Worldqztfp");

      etl::trim_left(text, String(STR("Hel")));

      CHECK(expected == text);
    }

    //*************************************************************************
    TEST(test_trim_left_string_view_pointer_delimiters)
    {
      String text(STR("qztfpHello Worldqztfp"));
      String expected("Hello Worldqztfp");
      StringView view("Hel");

      etl::trim_left(text, view.begin(), view.size());

      CHECK(expected == text);
    }

    //*************************************************************************
    TEST(test_trim_left_string_view_pointer_delimiters_nothing_to_trim)
    {
      String text(STR("Hello Worldqztfp"));
      String expected("Hello Worldqztfp");
      StringView view("Hel");

      etl::trim_left(text, view);

      CHECK(expected == text);
    }

    //*************************************************************************
    TEST(test_trim_left_string_view_delimiters)
    {
      String text(STR("qztfpHello Worldqztfp"));
      String expected("Hello Worldqztfp");
      StringView view("Hel");

      etl::trim_left(text, view);

      CHECK(expected == text);
    }

    //*************************************************************************
    TEST(test_trim_left_string_view_delimiters_nothing_to_trim)
    {
      String text(STR("Hello Worldqztfp"));
      String expected("Hello Worldqztfp");
      StringView view("Hel");

      etl::trim_left(text, view.begin(), view.size());

      CHECK(expected == text);
    }

    //*************************************************************************
    TEST(test_trim_right)
    {
      String text(STR(" \t\n\r\f\vHello World \t\n\r\f\v "));
      String expected(STR(" \t\n\r\f\vHello World"));

      etl::trim_right(text);

      CHECK(expected == text);
    }

    //*************************************************************************
    TEST(test_trim_right_nothing_to_trim)
    {
      String text(STR(" \t\n\r\f\vHello World"));
      String expected(STR(" \t\n\r\f\vHello World"));

      etl::trim_right(text);

      CHECK(expected == text);
    }

    //*************************************************************************
    TEST(test_trim_right_pointer_delimiters)
    {
      String text(STR("qztfpHello Worldqztfp"));
      String expected(STR("qztfpHello World"));

      etl::trim_right(text, "rld");

      CHECK(expected == text);
    }

    //*************************************************************************
    TEST(test_trim_right_pointer_delimiters_nothing_to_trim)
    {
      String text(STR("qztfpHello World"));
      String expected(STR("qztfpHello World"));

      etl::trim_right(text, "rld");

      CHECK(expected == text);
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
    TEST(test_trim_right_string_delimiters_nothing_to_trim)
    {
      String text(STR("qztfpHello Worldq"));
      String expected("qztfpHello World");

      etl::trim_right(text, String(STR("rld")));

      CHECK(expected == text);
    }

    //*************************************************************************
    TEST(test_trim_right_string_view_delimiters)
    {
      String text(STR("qztfpHello Worldqztfp"));
      String expected("qztfpHello World");
      StringView view("rld");

      etl::trim_right(text, view.begin(), view.size());

      CHECK(expected == text);
    }

    //*************************************************************************
    TEST(test_trim_right_string_view_delimiters_nothing_to_trim)
    {
      String text(STR("qztfpHello World"));
      String expected("qztfpHello World");
      StringView view("rld");

      etl::trim_right(text, view.begin(), view.size());

      CHECK(expected == text);
    }

    //*************************************************************************
    TEST(test_trim)
    {
      String text(STR(" \t\n\r\f\vHello World \t\n\r\f\v "));
      String expected(STR("Hello World"));

      etl::trim(text);

      CHECK(expected == text);
    }

    //*************************************************************************
    TEST(test_trim_nothing_to_trim)
    {
      String text(STR("Hello World"));
      String expected(STR("Hello World"));

      etl::trim(text);

      CHECK(expected == text);
    }

    ////*************************************************************************
    //TEST(test_trim_view)
    //{
    //  String text(STR(" \t\n\r\f\vHello World \t\n\r\f\v "));
    //  String expected(STR("Hello World"));

    //  StringView view = etl::trim_view(text);

    //  String view_text(view.begin(), view.end());
    //  CHECK(expected == view_text);
    //}

    ////*************************************************************************
    //TEST(test_trim_view_nothing_to_trim)
    //{
    //  String text(STR("Hello World"));
    //  String expected(STR("Hello World"));

    //  StringView view = etl::trim_view(text);

    //  String view_text(view.begin(), view.end());
    //  CHECK(expected == view_text);
    //}

    ////*************************************************************************
    //TEST(test_reverse)
    //{
    //  String text(STR("Hello World"));
    //  String expected(STR("dlroW olleH"));

    //  etl::reverse(text);

    //  CHECK(expected == text);
    //}
  };
}
