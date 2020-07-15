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
#include "etl/vector.h"

#undef STR
#define STR(x) x

namespace
{
  SUITE(test_string_utilities_char)
  {
    static const size_t SIZE = 50;

    typedef etl::string<SIZE>        String;
    typedef etl::istring             IString;
    typedef etl::string_view         StringView;
    typedef etl::istring::value_type Char;
    typedef etl::vector<String, 10>  Vector;

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
    TEST(test_trim_from_left_pointer_empty)
    {
      String text(STR(""));
      String expected(STR(""));

      etl::trim_from_left(text, STR(" \t\n\r\f\v"));

      CHECK(expected == text);
    }

    //*************************************************************************
    TEST(test_view_trim_from_left_pointer_empty)
    {
      String text(STR(""));
      String expected(STR(""));

      StringView textview(text);
      StringView expectedview(expected);

      StringView view = etl::trim_from_left(textview, STR(" \t\n\r\f\v"));

      CHECK(expectedview == view);
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
    TEST(test_view_trim_from_left_pointer)
    {
      String text(STR(" \t\n\r\f\vHello World\t\n\r\f\v "));
      String expected(STR("Hello World\t\n\r\f\v "));

      StringView textview(text);
      StringView expectedview(expected);

      StringView view = etl::trim_from_left(textview, STR(" \t\n\r\f\v"));

      CHECK(expectedview == view);
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
    TEST(test_view_trim_from_left_pointer_nothing_to_trim)
    {
      String text(STR("Hello World\t\n\r\f\v "));
      String expected(STR("Hello World\t\n\r\f\v "));

      StringView textview(text);
      StringView expectedview(expected);

      StringView view = etl::trim_from_left(textview, STR(" \t\n\r\f\v"));

      CHECK(expectedview == view);
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
    TEST(test_trim_from_right_pointer_empty)
    {
      String text(STR(""));
      String expected(STR(""));

      etl::trim_from_right(text, STR(" \t\n\r\f\v"));

      CHECK(expected == text);
    }

    //*************************************************************************
    TEST(test_view_trim_from_right_pointer_empty)
    {
      String text(STR(""));
      String expected(STR(""));

      StringView textview(text);
      StringView expectedview(expected);

      StringView view = etl::trim_from_right(textview, STR(" \t\n\r\f\v"));

      CHECK(expectedview == view);
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
    TEST(test_view_trim_from_right_pointer)
    {
      String text(STR(" \t\n\r\f\vHello World\t\n\r\f\v "));
      String expected(STR(" \t\n\r\f\vHello World"));

      StringView textview(text);
      StringView expectedview(expected);

      StringView view = etl::trim_from_right(textview, STR(" \t\n\r\f\v"));

      CHECK(expectedview == view);
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
    TEST(test_view_trim_from_right_pointer_nothing_to_trim)
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

      StringView textview(text);
      StringView expectedview(expected);

      StringView view = etl::right_n(textview, expected.size());

      CHECK(expectedview == view);
    }

    //*************************************************************************
    TEST(test_right_n_view_excess)
    {
      String text(STR("Hello World"));
      String expected(STR("Hello World"));

      StringView textview(text);
      StringView expectedview(expected);

      StringView view = etl::right_n(textview, text.size() * 2U);

      CHECK(expectedview == view);
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

      StringView textview(text);
      StringView expectedview(expected);

      StringView view = etl::left_n(textview, expected.size());

      CHECK(expectedview == view);
    }

    //*************************************************************************
    TEST(test_left_n_view_excess)
    {
      String text(STR("Hello World"));
      String expected(STR("Hello World"));

      StringView textview(text);
      StringView expectedview(expected);

      StringView view = etl::left_n(textview, text.size() * 2U);

      CHECK(expectedview == view);
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
    TEST(test_transform_characters)
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

      etl::transform(text, etl::begin(lookup), etl::end(lookup));

      CHECK(expected == text);
    }

    //*************************************************************************
    TEST(test_transform_characters_empty_string)
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

      etl::transform(text, etl::begin(lookup), etl::end(lookup));

      CHECK(expected == text);
    }

    //*************************************************************************
    TEST(test_transform_strings)
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

      etl::transform(text, etl::begin(lookup), etl::end(lookup));

      CHECK(expected == text);
    }

    //*************************************************************************
    TEST(test_transform_strings_empty_strings)
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

      etl::transform(text, etl::begin(lookup), etl::end(lookup));

      CHECK(expected == text);
    }

    //*************************************************************************
    TEST(test_get_token_pointer_delimiters)
    {
      String text(STR("  The cat.sat,  on;the:mat .,;:"));
      Vector tokens;

      StringView token;
      
      while (token.end() != text.end())
      { 
        token = etl::get_token(text, STR(" .,;:"), token);

        if (!token.empty())
        {
          tokens.emplace_back(token.begin(), token.end());
        }
      }

      CHECK_EQUAL(6U, tokens.size());
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
      expected.insert(0U, text.capacity() - expected.size(), STR('x'));

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
      expected.insert(0U, text.capacity() - expected.size(), STR('x'));

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
      StringView textview(text);

      String::const_iterator itr = etl::find_first_of(textview, STR("Hel"));

      CHECK_EQUAL(STR('H'), *itr);
    }

    //*************************************************************************
    TEST(test_find_first_of_string_view_not_found)
    {
      const String text(STR("abcHello Worldabc"));
      StringView textview(text);

      String::const_iterator itr = etl::find_first_of(textview, STR("xyz"));

      CHECK(text.end() == itr);
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
      StringView textview(text);

      String::const_iterator itr = etl::find_first_not_of(textview, STR("abc"));

      CHECK_EQUAL(STR('H'), *itr);
    }

    //*************************************************************************
    TEST(test_find_first_not_of_string_view_not_found)
    {
      const String text(STR("abcHello Worldabc"));
      StringView textview(text);

      String::const_iterator itr = etl::find_first_not_of(textview, STR("abcHello World"));

      CHECK(text.end() == itr);
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
      StringView textview(text);

      String::const_iterator itr = etl::find_last_of(textview, STR("rld"));

      CHECK_EQUAL(STR('d'), *itr);
    }

    //*************************************************************************
    TEST(test_find_last_of_string_view_not_found)
    {
      String text(STR("abcHello Worldabc"));
      StringView textview(text);

      String::const_iterator itr = etl::find_last_of(textview, STR("xyz"));

      CHECK(text.end() == itr);
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
      StringView textview(text);

      String::const_iterator itr = etl::find_last_not_of(textview, STR("abc"));

      CHECK_EQUAL(STR('d'), *itr);
    }

    //*************************************************************************
    TEST(test_find_last_not_of_string_view_not_found)
    {
      String text(STR("abcHello Worldabc"));
      StringView textview(text);

      String::const_iterator itr = etl::find_last_not_of(textview, STR("abcHello World"));

      CHECK(text.end() == itr);
    }
  };
}
