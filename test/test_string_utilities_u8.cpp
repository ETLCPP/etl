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

#include "etl/u8string.h"
#include "etl/string_view.h"
#include "etl/string_utilities.h"
#include "etl/vector.h"

#undef STR
#define STR(x) u8##x

#undef STR_PTR
#define STR_PTR const char8_t*

namespace
{
  //***********************************
  //std::ostream& operator << (std::ostream& os, const std::u16string::value_type& c)
  //{
  //  os << uint16_t(c);

  //  return os;
  //}

  SUITE(test_string_utilities_u8)
  {
    static const size_t SIZE = 50;

    using String     = etl::u8string<SIZE>;
    using IString    = etl::iu8string;
    using StringView = etl::u8string_view;
    using Char       = etl::iu8string::value_type;
    using Vector     = etl::vector<String, 15>;

#if ETL_USING_CPP17
    constexpr auto Whitespace = etl::whitespace_v<String::value_type>;
#else
    STR_PTR Whitespace = etl::whitespace<String::value_type>::value();
#endif

    //*************************************************************************
    TEST(test_trim_whitespace_left_empty)
    {
      String text(STR(""));
      String expected(STR(""));

      etl::trim_whitespace_left(text);

      CHECK(expected == text);
    }

    //*************************************************************************
    TEST(test_trim_view_whitespace_left_empty)
    {
      String text(STR(""));
      String expected(STR(""));

      StringView textview(text.data(), text.size());
      StringView expectedview(expected.data(), expected.size());

      StringView view = etl::trim_view_whitespace_left(textview);

      CHECK_EQUAL(expectedview.size(), view.size());
      CHECK(std::equal(expectedview.begin(), expectedview.end(), view.begin()));
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
    TEST(test_trim_view_whitespace_left)
    {
      String text(STR(" \t\n\r\f\vHello World\t\n\r\f\v "));
      String expected(STR("Hello World\t\n\r\f\v "));

      StringView textview(text.data(), text.size());
      StringView expectedview(expected.data(), expected.size());

      StringView view = etl::trim_view_whitespace_left(textview);

      CHECK_EQUAL(expectedview.size(), view.size());
      CHECK(std::equal(expectedview.begin(), expectedview.end(), view.begin()));
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
    TEST(test_trim_view_whitespace_left_nothing_to_trim)
    {
      String text(STR("Hello World\t\n\r\f\v "));
      String expected(STR("Hello World\t\n\r\f\v "));

      StringView textview(text.data(), text.size());
      StringView expectedview(expected.data(), expected.size());

      StringView view = etl::trim_view_whitespace_left(textview);

      CHECK_EQUAL(expectedview.size(), view.size());
      CHECK(std::equal(expectedview.begin(), expectedview.end(), view.begin()));
    }

    //*************************************************************************
    TEST(test_trim_from_left_pointer_empty)
    {
      String text(STR(""));
      String expected(STR(""));

      etl::trim_from_left(text, STR(" \t\n\r\f\v"));

      CHECK(expected == text);
    }

    //*************************************************************************
    TEST(test_trim_from_view_left_pointer_empty)
    {
      String text(STR(""));
      String expected(STR(""));

      StringView textview(text.data(), text.size());
      StringView expectedview(expected.data(), expected.size());

      StringView view = etl::trim_from_view_left(textview, STR(" \t\n\r\f\v"));

      CHECK_EQUAL(expectedview.size(), view.size());
      CHECK(std::equal(expectedview.begin(), expectedview.end(), view.begin()));
    }

    //*************************************************************************
    TEST(test_trim_from_left_pointer)
    {
      String text(STR(" \t\n\r\f\vHello World\t\n\r\f\v "));
      String expected(STR("Hello World\t\n\r\f\v "));

      etl::trim_from_left(text, STR(" \t\n\r\f\v"));

      CHECK(expected == text);
    }

    //*************************************************************************
    TEST(test_trim_from_view_left_pointer)
    {
      String text(STR(" \t\n\r\f\vHello World\t\n\r\f\v "));
      String expected(STR("Hello World\t\n\r\f\v "));

      StringView textview(text.data(), text.size());
      StringView expectedview(expected.data(), expected.size());

      StringView view = etl::trim_from_view_left(textview, STR(" \t\n\r\f\v"));

      CHECK_EQUAL(expectedview.size(), view.size());
      CHECK(std::equal(expectedview.begin(), expectedview.end(), view.begin()));
    }

    //*************************************************************************
    TEST(test_trim_from_left_pointer_nothing_to_trim)
    {
      String text(STR("Hello World\t\n\r\f\v "));
      String expected(STR("Hello World\t\n\r\f\v "));

      etl::trim_from_left(text, STR(" \t\n\r\f\v"));

      CHECK(expected == text);
    }

    //*************************************************************************
    TEST(test_trim_from_view_left_pointer_nothing_to_trim)
    {
      String text(STR("Hello World\t\n\r\f\v "));
      String expected(STR("Hello World\t\n\r\f\v "));

      StringView textview(text.data(), text.size());
      StringView expectedview(expected.data(), expected.size());

      StringView view = etl::trim_from_view_left(textview, STR(" \t\n\r\f\v"));

      CHECK_EQUAL(expectedview.size(), view.size());
      CHECK(std::equal(expectedview.begin(), expectedview.end(), view.begin()));
    }

    //*************************************************************************
    TEST(test_trim_from_left_pointer_length_empty)
    {
      String text(STR(""));
      String expected(STR(""));

      etl::trim_from_left(text, STR(" \t\n\r\f\v"));

      CHECK(expected == text);
    }

    //*************************************************************************
    TEST(test_trim_left_delimiters)
    {
      String text(STR("qztfpHello Worldqztfp"));
      String expected(STR("Hello Worldqztfp"));

      etl::trim_left(text, STR("Hel"));

      CHECK(expected == text);
    }

    //*************************************************************************
    TEST(test_trim_view_left_delimiters)
    {
      String text(STR("qztfpHello Worldqztfp"));
      String expected(STR("Hello Worldqztfp"));

      StringView textview(text.data(), text.size());
      StringView expectedview(expected.data(), expected.size());

      StringView view = etl::trim_view_left(textview, STR("Hel"));

      CHECK_EQUAL(expectedview.size(), view.size());
      CHECK(std::equal(expectedview.begin(), expectedview.end(), view.begin()));
    }

    //*************************************************************************
    TEST(test_trim_left_delimiters_nothing_to_trim)
    {
      String text(STR("Hello Worldqztfp"));
      String expected(STR("Hello Worldqztfp"));

      etl::trim_left(text, STR("Hel"));

      CHECK(expected == text);
    }

    //*************************************************************************
    TEST(test_trim_view_left_delimiters_nothing_to_trim)
    {
      String text(STR("Hello Worldqztfp"));
      String expected(STR("Hello Worldqztfp"));

      StringView textview(text.data(), text.size());
      StringView expectedview(expected.data(), expected.size());

      StringView view = etl::trim_view_left(textview, STR("Hel"));

      CHECK_EQUAL(expectedview.size(), view.size());
      CHECK(std::equal(expectedview.begin(), expectedview.end(), view.begin()));
    }

    //*************************************************************************
    TEST(test_trim_left_delimiters_no_delimiters)
    {
      String text(STR("Hello Worldqztfp"));
      String expected(STR(""));

      etl::trim_left(text, STR("XYZ"));

      CHECK(expected == text);
    }

    //*************************************************************************
    TEST(test_trim_view_left_delimiters_no_delimiters)
    {
      String text(STR("Hello Worldqztfp"));
      String expected(STR(""));

      StringView textview(text.data(), text.size());
      StringView expectedview(expected.data(), expected.size());

      StringView view = etl::trim_view_left(textview, STR("XYZ"));

      CHECK_EQUAL(expectedview.size(), view.size());
      CHECK(std::equal(expectedview.begin(), expectedview.end(), view.begin()));
    }

    //*************************************************************************
    TEST(test_trim_view_left_string_delimiters)
    {
      String text(STR("qztfpHello Worldqztfp"));
      String expected(STR("Hello Worldqztfp"));

      StringView textview(text.data(), text.size());
      StringView expectedview(expected.data(), expected.size());

      StringView view = etl::trim_view_left(textview, STR("Hel"));

      CHECK_EQUAL(expectedview.size(), view.size());
      CHECK_EQUAL(expectedview.size(), view.size());
      CHECK(std::equal(expectedview.begin(), expectedview.end(), view.begin()));
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
    TEST(test_trim_view_whitespace_right_empty)
    {
      String text(STR(""));
      String expected(STR(""));

      StringView textview(text.data(), text.size());
      StringView expectedview(expected.data(), expected.size());

      StringView view = etl::trim_view_whitespace_right(textview);

      CHECK_EQUAL(expectedview.size(), view.size());
      CHECK(std::equal(expectedview.begin(), expectedview.end(), view.begin()));
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
    TEST(test_trim_view_whitespace_right)
    {
      String text(STR(" \t\n\r\f\vHello World\t\n\r\f\v "));
      String expected(STR(" \t\n\r\f\vHello World"));

      StringView textview(text.data(), text.size());
      StringView expectedview(expected.data(), expected.size());

      StringView view = etl::trim_view_whitespace_right(textview);

      CHECK_EQUAL(expectedview.size(), view.size());
      CHECK(std::equal(expectedview.begin(), expectedview.end(), view.begin()));
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
    TEST(test_trim_view_whitespace_right_nothing_to_trim)
    {
      String text(STR(" \t\n\r\f\vHello World"));
      String expected(STR(" \t\n\r\f\vHello World"));

      StringView textview(text.data(), text.size());
      StringView expectedview(expected.data(), expected.size());

      StringView view = etl::trim_view_whitespace_right(textview);

      CHECK_EQUAL(expectedview.size(), view.size());
      CHECK(std::equal(expectedview.begin(), expectedview.end(), view.begin()));
    }

    //*************************************************************************
    TEST(test_trim_from_right_pointer_empty)
    {
      String text(STR(""));
      String expected(STR(""));

      etl::trim_from_right(text, STR(" \t\n\r\f\v"));

      CHECK(expected == text);
    }

    //*************************************************************************
    TEST(test_trim_from_view_right_pointer_empty)
    {
      String text(STR(""));
      String expected(STR(""));

      StringView textview(text.data(), text.size());
      StringView expectedview(expected.data(), expected.size());

      StringView view = etl::trim_from_view_right(textview, STR(" \t\n\r\f\v"));

      CHECK_EQUAL(expectedview.size(), view.size());
      CHECK(std::equal(expectedview.begin(), expectedview.end(), view.begin()));
    }

    //*************************************************************************
    TEST(test_trim_from_right_pointer)
    {
      String text(STR(" \t\n\r\f\vHello World\t\n\r\f\v "));
      String expected(STR(" \t\n\r\f\vHello World"));

      etl::trim_from_right(text, STR(" \t\n\r\f\v"));

      CHECK(expected == text);
    }

    //*************************************************************************
    TEST(test_trim_from_view_right_pointer)
    {
      String text(STR(" \t\n\r\f\vHello World\t\n\r\f\v "));
      String expected(STR(" \t\n\r\f\vHello World"));

      StringView textview(text.data(), text.size());
      StringView expectedview(expected.data(), expected.size());

      StringView view = etl::trim_from_view_right(textview, STR(" \t\n\r\f\v"));

      CHECK_EQUAL(expectedview.size(), view.size());
      CHECK(std::equal(expectedview.begin(), expectedview.end(), view.begin()));
    }

    //*************************************************************************
    TEST(test_trim_from_right_pointer_nothing_to_trim)
    {
      String text(STR(" \t\n\r\f\vHello World"));
      String expected(STR(" \t\n\r\f\vHello World"));

      etl::trim_from_right(text, STR(" \t\n\r\f\v"));

      CHECK(expected == text);
    }

    //*************************************************************************
    TEST(test_trim_from_view_right_pointer_nothing_to_trim)
    {
      String text(STR(" \t\n\r\f\vHello World"));
      String expected(STR(" \t\n\r\f\vHello World"));

      StringView textview(text.data(), text.size());
      StringView expectedview(expected.data(), expected.size());

      StringView view = etl::trim_from_view_right(textview, STR(" \t\n\r\f\v"));

      CHECK_EQUAL(expectedview.size(), view.size());
      CHECK(std::equal(expectedview.begin(), expectedview.end(), view.begin()));
    }

    //*************************************************************************
    TEST(test_trim_right_delimiters)
    {
      String text(STR("qztfpHello Worldqztfp"));
      String expected(STR("qztfpHello World"));

      etl::trim_right(text, STR("rld"));

      CHECK(expected == text);
    }

    //*************************************************************************
    TEST(test_trim_view_right_delimiters)
    {
      String text(STR("qztfpHello Worldqztfp"));
      String expected(STR("qztfpHello World"));

      StringView textview(text.data(), text.size());
      StringView expectedview(expected.data(), expected.size());

      StringView view = etl::trim_view_right(textview, STR("rld"));

      CHECK_EQUAL(expectedview.size(), view.size());
      CHECK(std::equal(expectedview.begin(), expectedview.end(), view.begin()));
    }

    //*************************************************************************
    TEST(test_trim_right_delimiters_nothing_to_trim)
    {
      String text(STR("qztfpHello World"));
      String expected(STR("qztfpHello World"));

      etl::trim_right(text, STR("rld"));

      CHECK(expected == text);
    }

    //*************************************************************************
    TEST(test_trim_view_right_delimiters_nothing_to_trim)
    {
      String text(STR("qztfpHello World"));
      String expected(STR("qztfpHello World"));

      StringView textview(text.data(), text.size());
      StringView expectedview(expected.data(), expected.size());

      StringView view = etl::trim_view_right(textview, STR("rld"));

      CHECK_EQUAL(expectedview.size(), view.size());
      CHECK(std::equal(expectedview.begin(), expectedview.end(), view.begin()));
    }

    //*************************************************************************
    TEST(test_trim_right_delimiters_no_delimiters)
    {
      String text(STR("qztfpHello World"));
      String expected(STR(""));

      etl::trim_right(text, STR("XYZ"));

      CHECK(expected == text);
    }

    //*************************************************************************
    TEST(test_trim_view_right_delimiters_no_delimiters)
    {
      String text(STR("qztfpHello World"));
      String expected(STR(""));

      StringView textview(text.data(), text.size());
      StringView expectedview(expected.data(), expected.size());

      StringView view = etl::trim_view_right(textview, STR("XYZ"));

      CHECK_EQUAL(expectedview.size(), view.size());
      CHECK(std::equal(expectedview.begin(), expectedview.end(), view.begin()));
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
    TEST(test_trim_view_whitespace_empty)
    {
      String text(STR(""));
      String expected(STR(""));

      StringView textview(text.data(), text.size());
      StringView expectedview(expected.data(), expected.size());

      StringView view = etl::trim_view_whitespace(textview);

      CHECK_EQUAL(expectedview.size(), view.size());
      CHECK(std::equal(expectedview.begin(), expectedview.end(), view.begin()));
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
    TEST(test_trim_view_whitespace)
    {
      String text(STR(" \t\n\r\f\vHello World\t\n\r\f\v "));
      String expected(STR("Hello World"));

      StringView textview(text.data(), text.size());
      StringView expectedview(expected.data(), expected.size());

      StringView view = etl::trim_view_whitespace(textview);

      CHECK_EQUAL(expectedview.size(), view.size());
      CHECK(std::equal(expectedview.begin(), expectedview.end(), view.begin()));
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
    TEST(test_trim_view_whitespace_nothing_to_trim)
    {
      String text(STR("Hello World"));
      String expected(STR("Hello World"));

      StringView textview(text.data(), text.size());
      StringView expectedview(expected.data(), expected.size());

      StringView view = etl::trim_view_whitespace(textview);

      CHECK_EQUAL(expectedview.size(), view.size());
      CHECK(std::equal(expectedview.begin(), expectedview.end(), view.begin()));
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
    TEST(test_trim_from_view_empty)
    {
      String text(STR(""));
      String expected(STR(""));

      StringView textview(text.data(), text.size());
      StringView expectedview(expected.data(), expected.size());

      StringView view = etl::trim_from_view(textview, STR(" \t\n\r\f\v"));

      CHECK_EQUAL(expectedview.size(), view.size());
      CHECK(std::equal(expectedview.begin(), expectedview.end(), view.begin()));
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
    TEST(test_trim_from_view)
    {
      String text(STR(" \t\n\r\f\vHello World\t\n\r\f\v "));
      String expected(STR("Hello World"));

      StringView textview(text.data(), text.size());
      StringView expectedview(expected.data(), expected.size());

      StringView view = etl::trim_from_view(textview, STR(" \t\n\r\f\v"));

      CHECK_EQUAL(expectedview.size(), view.size());
      CHECK(std::equal(expectedview.begin(), expectedview.end(), view.begin()));
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
    TEST(test_trim_from_view_nothing_to_trim)
    {
      String text(STR("Hello World"));
      String expected(STR("Hello World"));

      StringView textview(text.data(), text.size());
      StringView expectedview(expected.data(), expected.size());

      StringView view = etl::trim_from_view(textview, STR(" \t\n\r\f\v"));

      CHECK_EQUAL(expectedview.size(), view.size());
      CHECK(std::equal(expectedview.begin(), expectedview.end(), view.begin()));
    }

    //*************************************************************************
    TEST(test_trim_delimiters)
    {
      String text(STR("qztfpHello Worldqztfp"));
      String expected(STR("Hello World"));

      etl::trim(text, STR("Hd"));

      CHECK(expected == text);
    }

    //*************************************************************************
    TEST(test_trim_view_delimiters)
    {
      String text(STR("qztfpHello Worldqztfp"));
      String expected(STR("Hello World"));

      StringView textview(text.data(), text.size());
      StringView expectedview(expected.data(), expected.size());

      StringView view = etl::trim_view(textview, STR("Hd"));

      CHECK_EQUAL(expectedview.size(), view.size());
      CHECK(std::equal(expectedview.begin(), expectedview.end(), view.begin()));
    }

    //*************************************************************************
    TEST(test_trim_delimiters_nothing_to_trim)
    {
      String text(STR("Hello World"));
      String expected(STR("Hello World"));

      etl::trim(text, STR("Hd"));

      CHECK(expected == text);
    }

    //*************************************************************************
    TEST(test_trim_view_delimiters_nothing_to_trim)
    {
      String text(STR("Hello World"));
      String expected(STR("Hello World"));

      StringView textview(text.data(), text.size());
      StringView expectedview(expected.data(), expected.size());

      StringView view = etl::trim_view(textview, STR("Hd"));

      CHECK_EQUAL(expectedview.size(), view.size());
      CHECK(std::equal(expectedview.begin(), expectedview.end(), view.begin()));
    }

    //*************************************************************************
    TEST(test_trim_delimiters_no_delimiters)
    {
      String text(STR("Hello World"));
      String expected(STR(""));

      etl::trim(text, STR("XYZ"));

      CHECK(expected == text);
    }

    //*************************************************************************
    TEST(test_trim_view_delimiters_no_delimiters)
    {
      String text(STR("Hello World"));
      String expected(STR(""));

      StringView textview(text.data(), text.size());
      StringView expectedview(expected.data(), expected.size());

      StringView view = etl::trim_view(textview, STR("XYZ"));

      CHECK_EQUAL(expectedview.size(), view.size());
      CHECK(std::equal(expectedview.begin(), expectedview.end(), view.begin()));
    }

    //*************************************************************************
    TEST(test_reverse)
    {
      String text(STR("Hello World"));
      String expected(STR("dlroW olleH"));

      etl::reverse(text);

      CHECK(expected == text);
    }

    //*************************************************************************
    TEST(test_right_n_view)
    {
      String text(STR("Hello World"));
      String expected(STR("World"));

      StringView textview(text.data(), text.size());
      StringView expectedview(expected.data(), expected.size());

      StringView view = etl::right_n_view(textview, expected.size());

      CHECK_EQUAL(expectedview.size(), view.size());
      CHECK(std::equal(expectedview.begin(), expectedview.end(), view.begin()));
    }

    //*************************************************************************
    TEST(test_right_n_view_excess)
    {
      String text(STR("Hello World"));
      String expected(STR("Hello World"));

      StringView textview(text.data(), text.size());
      StringView expectedview(expected.data(), expected.size());

      StringView view = etl::right_n_view(textview, text.size() * 2U);

      CHECK_EQUAL(expectedview.size(), view.size());
      CHECK(std::equal(expectedview.begin(), expectedview.end(), view.begin()));
    }

    //*************************************************************************
    TEST(test_right_n_string)
    {
      String text(STR("Hello World"));
      String expected(STR("World"));

      etl::right_n(text, 5U);

      CHECK(expected == text);
    }

    //*************************************************************************
    TEST(test_right_n_string_excess)
    {
      String text(STR("Hello World"));
      String expected(STR("Hello World"));

      etl::right_n(text, text.size() * 2U);

      CHECK(expected == text);
    }

    //*************************************************************************
    TEST(test_left_n_view)
    {
      String text(STR("Hello World"));
      String expected(STR("Hello"));

      StringView textview(text.data(), text.size());
      StringView expectedview(expected.data(), expected.size());

      StringView view = etl::left_n_view(textview, expected.size());

      CHECK_EQUAL(expectedview.size(), view.size());
      CHECK(std::equal(expectedview.begin(), expectedview.end(), view.begin()));
    }

    //*************************************************************************
    TEST(test_left_n_view_excess)
    {
      String text(STR("Hello World"));
      String expected(STR("Hello World"));

      StringView textview(text.data(), text.size());
      StringView expectedview(expected.data(), expected.size());

      StringView view = etl::left_n_view(textview, text.size() * 2U);

      CHECK_EQUAL(expectedview.size(), view.size());
      CHECK(std::equal(expectedview.begin(), expectedview.end(), view.begin()));
    }

    //*************************************************************************
    TEST(test_left_n_string)
    {
      String text(STR("Hello World"));
      String expected(STR("Hello"));

      etl::left_n(text, expected.size());

      CHECK(expected == text);
    }

    //*************************************************************************
    TEST(test_left_n_string_excess)
    {
      String text(STR("Hello World"));
      String expected(STR("Hello World"));

      etl::left_n(text, text.size() * 2U);

      CHECK(expected == text);
    }

    //*************************************************************************
    TEST(test_replace_characters)
    {
      String text(STR("This+++is a  *file//name:"));
      String expected(STR("This---is_a__-file__name_"));

      etl::pair<Char, Char> lookup[] =
      {
        { STR('+'), STR('-') },
        { STR(' '), STR('_') },
        { STR('*'), STR('-') },
        { STR('/'), STR('_') },
        { STR(':'), STR('_') }
      };

      etl::replace_characters(text, ETL_OR_STD11::begin(lookup), ETL_OR_STD11::end(lookup));

      CHECK(expected == text);
    }

    //*************************************************************************
    TEST(test_replace_characters_empty_string)
    {
      String text(STR(""));
      String expected(STR(""));

      etl::pair<Char, Char> lookup[] =
      {
        { STR('+'), STR('-') },
        { STR(' '), STR('_') },
        { STR('*'), STR('-') },
        { STR('/'), STR('_') },
        { STR(':'), STR('_') }
      };

      etl::replace_characters(text, ETL_OR_STD11::begin(lookup), ETL_OR_STD11::end(lookup));

      CHECK(expected == text);
    }

    //*************************************************************************
    TEST(test_replace_strings)
    {
      String text(STR("This+++is a  *file//name:"));
      String expected(STR("Thisxyis%20a%20%20-file_name.txt"));

      etl::pair<const Char*, const Char*> lookup[] =
      {
        { STR("+++"), STR("xy") },
        { STR(" "),   STR("%20") },
        { STR("*"),   STR("-") },
        { STR("//"),  STR("_") },
        { STR(":"),   STR(".txt") }
      };

      etl::replace_strings(text, ETL_OR_STD11::begin(lookup), ETL_OR_STD11::end(lookup));

      CHECK(expected == text);
    }

    //*************************************************************************
    TEST(test_replace_strings_empty_strings)
    {
      String text(STR(""));
      String expected(STR(""));

      etl::pair<const Char*, const Char*> lookup[] =
      {
        { STR("+++"), STR("xy") },
        { STR(" "),   STR("%20") },
        { STR("*"),   STR("-") },
        { STR("//"),  STR("_") },
        { STR(":"),   STR(".txt") }
      };

      etl::replace_strings(text, ETL_OR_STD11::begin(lookup), ETL_OR_STD11::end(lookup));

      CHECK(expected == text);
    }

    //*************************************************************************
    TEST(test_get_token_delimiters_ignore_empty_tokens)
    {
      String text(STR(",,,The,cat,sat,,on,the,mat,,,"));
      Vector tokens;

      StringView textview(text.data(), text.size());
      etl::optional<StringView> token;

      while ((token = etl::get_token(text, STR(","), token, true)))
      {
        tokens.emplace_back(token.value());
      }

      CHECK_EQUAL(6U, tokens.size());
    }

    //*************************************************************************
    TEST(test_get_token_delimiters_keep_empty_tokens)
    {
      String text(STR(",,,The,cat,sat,,on,the,mat,,,"));
      Vector tokens;

      StringView textview(text.data(), text.size());
      etl::optional<StringView> token;

      while ((token = etl::get_token(text, STR(","), token, false)))
      {
        tokens.emplace_back(token.value());
      }

      CHECK_EQUAL(13U, tokens.size());
    }

    //*************************************************************************
    TEST(test_get_token_delimiters_nothing_to_do)
    {
      String text(STR(""));
      Vector tokens;

      StringView textview(text.data(), text.size());
      etl::optional<StringView> token;

      while ((token = etl::get_token(text, Whitespace, token, true)))
      {
        tokens.emplace_back(token.value());
      }

      CHECK_EQUAL(0U, tokens.size());
    }

    //*************************************************************************
    TEST(test_get_token_delimiters_no_tokens)
    {
      String text(STR("   .,  ;: .,;:"));
      Vector tokens;

      StringView textview(text.data(), text.size());
      etl::optional<StringView> token;

      while ((token = etl::get_token(text, STR(" .,;:"), token, true)))
      {
        tokens.emplace_back(token.value());
      }

      CHECK_EQUAL(0U, tokens.size());
    }

    //*************************************************************************
    TEST(test_get_token_delimiters_no_delimiters)
    {
      String text(STR("The cat sat on the mat"));
      Vector tokens;

      StringView textview(text.data(), text.size());
      etl::optional<StringView> token;

      while ((token = etl::get_token(text, STR(","), token, false)))
      {
        tokens.emplace_back(token.value());
      }

      CHECK_EQUAL(1U, tokens.size());
    }

    //*************************************************************************
    TEST(test_get_token_delimiters_empty_string_view)
    {
      Vector tokens;

      StringView textview;
      etl::optional<StringView> token;

      while ((token = etl::get_token(textview, Whitespace, token, true)))
      {
        tokens.emplace_back(token.value());
      }

      CHECK_EQUAL(0U, tokens.size());
    }

    //*************************************************************************
    TEST(test_pad_left)
    {
      String text(STR("Hello World"));
      String expected(STR("xxxxHello World"));

      etl::pad_left(text, expected.size(), STR('x'));

      CHECK(expected == text);
    }

    //*************************************************************************
    TEST(test_pad_left_smaller)
    {
      String text(STR("Hello World"));
      String expected(STR("Hello World"));

      etl::pad_left(text, text.size() - 1U, STR('x'));

      CHECK(expected == text);
    }

    //*************************************************************************
    TEST(test_pad_left_greater_than_capacity)
    {
      String text(STR("Hello World"));
      String expected(text);
      expected.insert(String::size_type(0U), text.capacity() - expected.size(), STR('x'));

      etl::pad_left(text, text.capacity() + 1U, STR('x'));

      CHECK(expected == text);
    }

    //*************************************************************************
    TEST(test_pad_right)
    {
      String text(STR("Hello World"));
      String expected(STR("Hello Worldxxxx"));

      etl::pad_right(text, expected.size(), STR('x'));

      CHECK(expected == text);
    }

    //*************************************************************************
    TEST(test_pad_right_smaller)
    {
      String text(STR("Hello World"));
      String expected(STR("Hello World"));

      etl::pad_right(text, text.size() - 1U, STR('x'));

      CHECK(expected == text);
    }

    //*************************************************************************
    TEST(test_pad_right_greater_than_capacity)
    {
      String text(STR("Hello World"));
      String expected(text);
      expected.insert(text.size(), text.capacity() - expected.size(), STR('x'));

      etl::pad_right(text, text.capacity() + 1U, STR('x'));

      CHECK(expected == text);
    }

    //*************************************************************************
    TEST(test_pad_select_left)
    {
      String text(STR("Hello World"));
      String expected(STR("xxxxHello World"));

      etl::pad(text, expected.size(), etl::string_pad_direction::LEFT, STR('x'));

      CHECK(expected == text);
    }

    //*************************************************************************
    TEST(test_pad_select_left_smaller)
    {
      String text(STR("Hello World"));
      String expected(STR("Hello World"));

      etl::pad(text, text.size() - 1U, etl::string_pad_direction::LEFT, STR('x'));

      CHECK(expected == text);
    }

    //*************************************************************************
    TEST(test_pad_select_left_greater_than_capacity)
    {
      String text(STR("Hello World"));
      String expected(text);
      expected.insert(String::size_type(0U), text.capacity() - expected.size(), STR('x'));

      etl::pad(text, text.capacity() + 1U, etl::string_pad_direction::LEFT, STR('x'));

      CHECK(expected == text);
    }

    //*************************************************************************
    TEST(test_pad_select_right)
    {
      String text(STR("Hello World"));
      String expected(STR("Hello Worldxxxx"));

      etl::pad(text, expected.size(), etl::string_pad_direction::RIGHT, STR('x'));

      CHECK(expected == text);
    }

    //*************************************************************************
    TEST(test_pad_select_right_smaller)
    {
      String text(STR("Hello World"));
      String expected(STR("Hello World"));

      etl::pad(text, text.size() - 1U, etl::string_pad_direction::RIGHT, STR('x'));

      CHECK(expected == text);
    }

    //*************************************************************************
    TEST(test_pad_select_right_greater_than_capacity)
    {
      String text(STR("Hello World"));
      String expected(text);
      expected.insert(text.size(), text.capacity() - expected.size(), STR('x'));

      etl::pad(text, text.capacity() + 1U, etl::string_pad_direction::RIGHT, STR('x'));

      CHECK(expected == text);
    }

    //*************************************************************************
    TEST(test_find_first_of_iterator_iterator)
    {
      String text(STR("abcHello Worldabc"));

      String::iterator itr = etl::find_first_of(text.begin(), text.end(), STR("Hel"));

      CHECK_EQUAL(STR('H'), *itr);
    }

    //*************************************************************************
    TEST(test_find_first_of_iterator_iterator_not_found)
    {
      String text(STR("abcHello Worldabc"));

      String::iterator itr = etl::find_first_of(text.begin(), text.end(), STR("xyz"));

      CHECK(text.end() == itr);
    }

    //*************************************************************************
    TEST(test_find_first_of_const_iterator_const_iterator)
    {
      const String text(STR("abcHello Worldabc"));

      String::const_iterator itr = etl::find_first_of(text.cbegin(), text.cend(), STR("Hel"));

      CHECK_EQUAL(STR('H'), *itr);
    }

    //*************************************************************************
    TEST(test_find_first_of_const_iterator_const_iterator_not_found)
    {
      String text(STR("abcHello Worldabc"));

      String::const_iterator itr = etl::find_first_of(text.begin(), text.end(), STR("xyz"));

      CHECK(text.end() == itr);
    }

    //*************************************************************************
    TEST(test_find_first_of_string)
    {
      String text(STR("abcHello Worldabc"));

      String::iterator itr = etl::find_first_of(text, STR("Hel"));

      CHECK_EQUAL(STR('H'), *itr);
    }

    //*************************************************************************
    TEST(test_find_first_of_string_not_found)
    {
      String text(STR("abcHello Worldabc"));

      String::iterator itr = etl::find_first_of(text, STR("xyz"));

      CHECK(text.end() == itr);
    }

    //*************************************************************************
    TEST(test_find_first_of_const_string)
    {
      const String text(STR("abcHello Worldabc"));

      String::const_iterator itr = etl::find_first_of(text, STR("Hel"));

      CHECK_EQUAL(STR('H'), *itr);
    }

    //*************************************************************************
    TEST(test_find_first_of_const_string_not_found)
    {
      const String text(STR("abcHello Worldabc"));

      String::const_iterator itr = etl::find_first_of(text, STR("xyz"));

      CHECK(text.end() == itr);
    }

    //*************************************************************************
    TEST(test_find_first_of_string_view)
    {
      const String text(STR("abcHello Worldabc"));
      StringView textview(text.data(), text.size());

      StringView::const_iterator itr = etl::find_first_of(textview, STR("Hel"));

      CHECK_EQUAL(STR('H'), *itr);
    }

    //*************************************************************************
    TEST(test_find_first_of_string_view_not_found)
    {
      const String text(STR("abcHello Worldabc"));
      StringView textview(text.data(), text.size());

      StringView::const_iterator itr = etl::find_first_of(textview, STR("xyz"));

      CHECK(textview.end() == itr);
    }

    //*************************************************************************
    TEST(test_find_first_not_of_iterator_iterator)
    {
      String text(STR("abcHello Worldabc"));

      String::iterator itr = etl::find_first_not_of(text.begin(), text.end(), STR("abc"));

      CHECK_EQUAL(STR('H'), *itr);
    }

    //*************************************************************************
    TEST(test_find_first_not_of_iterator_iterator_not_found)
    {
      String text(STR("abcHello Worldabc"));
      String::iterator itr = etl::find_first_not_of(text.begin(), text.end(), STR("abcHello World"));

      CHECK(text.end() == itr);
    }

    //*************************************************************************
    TEST(test_find_first_not_of_const_iterator_const_iterator)
    {
      const String text(STR("abcHello Worldabc"));

      String::const_iterator itr = etl::find_first_not_of(text.cbegin(), text.cend(), STR("abc"));

      CHECK_EQUAL(STR('H'), *itr);
    }

    //*************************************************************************
    TEST(test_find_first_not_of_const_iterator_const_iterator_not_found)
    {
      const String text(STR("abcHello Worldabc"));

      String::const_iterator itr = etl::find_first_not_of(text.cbegin(), text.cend(), STR("abcHello World"));

      CHECK(text.end() == itr);
    }

    //*************************************************************************
    TEST(test_find_first_not_of_string)
    {
      String text(STR("abcHello Worldabc"));

      String::iterator itr = etl::find_first_not_of(text, STR("abc"));

      CHECK_EQUAL(STR('H'), *itr);
    }

    //*************************************************************************
    TEST(test_find_first_not_of_string_not_found)
    {
      String text(STR("abcHello Worldabc"));

      String::iterator itr = etl::find_first_not_of(text, STR("abcHello World"));

      CHECK(text.end() == itr);
    }

    //*************************************************************************
    TEST(test_find_first_not_of_const_string)
    {
      const String text(STR("abcHello Worldabc"));

      String::const_iterator itr = etl::find_first_not_of(text, STR("abc"));

      CHECK_EQUAL(STR('H'), *itr);
    }

    //*************************************************************************
    TEST(test_find_first_not_of_const_string_not_found)
    {
      const String text(STR("abcHello Worldabc"));

      String::const_iterator itr = etl::find_first_not_of(text, STR("abcHello World"));

      CHECK(text.end() == itr);
    }

    //*************************************************************************
    TEST(test_find_first_not_of_string_view)
    {
      const String text(STR("abcHello Worldabc"));
      StringView textview(text.data(), text.size());

      StringView::const_iterator itr = etl::find_first_not_of(textview, STR("abc"));

      CHECK_EQUAL(STR('H'), *itr);
    }

    //*************************************************************************
    TEST(test_find_first_not_of_string_view_not_found)
    {
      const String text(STR("abcHello Worldabc"));
      StringView textview(text.data(), text.size());

      StringView::const_iterator itr = etl::find_first_not_of(textview, STR("abcHello World"));

      CHECK(textview.end() == itr);
    }

    //*************************************************************************
    TEST(test_find_last_of_iterator_iterator)
    {
      String text(STR("abcHello Worldabc"));

      String::iterator itr = etl::find_last_of(text.begin(), text.end(), STR("rld"));

      CHECK_EQUAL(STR('d'), *itr);
    }

    //*************************************************************************
    TEST(test_find_last_of_iterator_iterator_not_found)
    {
      String text(STR("abcHello Worldabc"));

      String::iterator itr = etl::find_last_of(text.begin(), text.end(), STR("xyz"));

      CHECK(text.end() == itr);
    }

    //*************************************************************************
    TEST(test_find_last_of_const_iterator_const_iterator)
    {
      const String text(STR("abcHello Worldabc"));

      String::const_iterator itr = etl::find_last_of(text.cbegin(), text.cend(), STR("rld"));

      CHECK_EQUAL(STR('d'), *itr);
    }

    //*************************************************************************
    TEST(test_find_last_of_const_iterator_const_iterator_not_found)
    {
      const String text(STR("abcHello Worldabc"));

      String::const_iterator itr = etl::find_last_of(text.cbegin(), text.cend(), STR("xyz"));

      CHECK(text.end() == itr);
    }

    //*************************************************************************
    TEST(test_find_last_of_string)
    {
      String text(STR("abcHello Worldabc"));

      String::iterator itr = etl::find_last_of(text, STR("rld"));

      CHECK_EQUAL(STR('d'), *itr);
    }

    //*************************************************************************
    TEST(test_find_last_of_string_not_found)
    {
      String text(STR("abcHello Worldabc"));

      String::iterator itr = etl::find_last_of(text, STR("xyz"));

      CHECK(text.end() == itr);
    }

    //*************************************************************************
    TEST(test_find_last_of_const_string)
    {
      const String text(STR("abcHello Worldabc"));

      String::const_iterator itr = etl::find_last_of(text, STR("rld"));

      CHECK_EQUAL(STR('d'), *itr);
    }

    //*************************************************************************
    TEST(test_find_last_of_const_string_not_found)
    {
      const String text(STR("abcHello Worldabc"));

      String::const_iterator itr = etl::find_last_of(text, STR("xyz"));

      CHECK(text.end() == itr);
    }

    //*************************************************************************
    TEST(test_find_last_of_string_view)
    {
      String text(STR("abcHello Worldabc"));
      StringView textview(text.data(), text.size());

      StringView::const_iterator itr = etl::find_last_of(textview, STR("rld"));

      CHECK_EQUAL(STR('d'), *itr);
    }

    //*************************************************************************
    TEST(test_find_last_of_string_view_not_found)
    {
      String text(STR("abcHello Worldabc"));
      StringView textview(text.data(), text.size());

      StringView::const_iterator itr = etl::find_last_of(textview, STR("xyz"));

      CHECK(textview.end() == itr);
    }

    //*************************************************************************
    TEST(test_find_last_not_of_iterator_iterator)
    {
      String text(STR("abcHello Worldabc"));

      String::iterator itr = etl::find_last_not_of(text.begin(), text.end(), STR("abc"));

      CHECK_EQUAL(STR('d'), *itr);
    }

    //*************************************************************************
    TEST(test_find_last_not_of_iterator_iterator_not_found)
    {
      String text(STR("abcHello Worldabc"));

      String::iterator itr = etl::find_last_not_of(text.begin(), text.end(), STR("abcHello World"));

      CHECK(text.end() == itr);
    }

    //*************************************************************************
    TEST(test_find_last_not_of_const_iterator_const_iterator)
    {
      const String text(STR("abcHello Worldabc"));

      String::const_iterator itr = etl::find_last_not_of(text.cbegin(), text.cend(), STR("abc"));

      CHECK_EQUAL(STR('d'), *itr);
    }

    //*************************************************************************
    TEST(test_find_last_not_of_const_iterator_const_iterator_not_found)
    {
      const String text(STR("abcHello Worldabc"));

      String::const_iterator itr = etl::find_last_not_of(text.cbegin(), text.cend(), STR("abcHello World"));

      CHECK(text.end() == itr);
    }

    //*************************************************************************
    TEST(test_find_last_not_of_string)
    {
      String text(STR("abcHello Worldabc"));

      String::iterator itr = etl::find_last_not_of(text, STR("abc"));

      CHECK_EQUAL(STR('d'), *itr);
    }

    //*************************************************************************
    TEST(test_find_last_not_of_string_not_found)
    {
      String text(STR("abcHello Worldabc"));

      String::iterator itr = etl::find_last_not_of(text, STR("abcHello World"));

      CHECK(text.end() == itr);
    }

    //*************************************************************************
    TEST(test_find_last_not_of_const_string)
    {
      const String text(STR("abcHello Worldabc"));

      String::const_iterator itr = etl::find_last_not_of(text, STR("abc"));

      CHECK_EQUAL(STR('d'), *itr);
    }

    //*************************************************************************
    TEST(test_find_last_not_of_const_string_not_found)
    {
      const String text(STR("abcHello Worldabc"));

      String::const_iterator itr = etl::find_last_not_of(text, STR("abcHello World"));

      CHECK(text.end() == itr);
    }

    //*************************************************************************
    TEST(test_find_last_not_of_string_view)
    {
      String text(STR("abcHello Worldabc"));
      StringView textview(text.data(), text.size());

      StringView::const_iterator itr = etl::find_last_not_of(textview, STR("abc"));

      CHECK_EQUAL(STR('d'), *itr);
    }

    //*************************************************************************
    TEST(test_find_last_not_of_string_view_not_found)
    {
      String text(STR("abcHello Worldabc"));
      StringView textview(text.data(), text.size());

      StringView::const_iterator itr = etl::find_last_not_of(textview, STR("abcHello World"));

      CHECK(textview.end() == itr);
    }
  };
}

#endif
