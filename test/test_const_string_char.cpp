/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl
https://www.etlcpp.com

Copyright(c) 2025 John Wellbelove

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

#include <string>
#include <array>
#include <algorithm>
#include <atomic>

#include "etl/const_string.h"
#include "etl/string_view.h"
#include "etl/fnv_1.h"

#undef STR
#define STR(x) x

namespace
{
  bool compares_agree(int result1, int result2)
  {
    return ((result1 < 0)  && (result2 < 0)) ||
           ((result1 == 0) && (result2 == 0)) ||
           ((result1 > 0)  && (result2 > 0));
  }

  SUITE(test_string_char)
  {
    static const size_t SIZE   = 11UL;

    using Text    = etl::const_string;
    using IText   = const etl::istring;
    using TextSTD = std::string;
    using value_t = Text::value_type;
    //using TextL   = etl::string<52>;
    //using TextS   = etl::string<4>;
    //using View    = etl::string_view;

    TextSTD initial_text;
    TextSTD less_text;
    TextSTD greater_text;
    TextSTD shorter_text;
    TextSTD different_text;
    TextSTD insert_text;
    TextSTD longer_text;
    TextSTD short_text;

    const value_t* pinitial_text = STR("Hello World");

    //*************************************************************************
    template <typename T1, typename T2>
    bool Equal(const T1& compare_text, const T2& text)
    {
      return (compare_text.size() == text.size()) && std::equal(text.begin(), text.end(), compare_text.begin());
    }

    //*************************************************************************
    struct SetupFixture
    {
      SetupFixture()
      {
        initial_text   = STR("Hello World");
        insert_text    = STR("Insert");
        less_text      = STR("Hello Vorld");
        greater_text   = STR("Hello Xorld");
        shorter_text   = STR("Hello Worl");
        different_text = STR("Byee Planet");
        longer_text    = STR("Hello World There");
        short_text     = STR("Hello");
      }
    };

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_constructor_char_pointer)
    {
      TextSTD compare_text(initial_text.c_str());

      static constexpr const char* initial = "Hello World";
      static constexpr Text        text(initial);
      static constexpr size_t      length = etl::strlen(initial);

      static constexpr auto text_empty = text.empty();
      CHECK_FALSE(text_empty);

      static constexpr auto text_full = text.full();
      CHECK_TRUE(text_full);

      static constexpr auto text_capacity = text.capacity();
      CHECK_EQUAL(length, text_capacity);

      static constexpr auto text_max_size = text.max_size();
      CHECK_EQUAL(length, text_max_size);

      static constexpr auto text_size = text.size();
      CHECK_EQUAL(length, text_size);

      static constexpr auto text_length = text.length();
      CHECK_EQUAL(length, text_length);

      static constexpr auto text_available = text.available();
      CHECK_EQUAL(0, text_available);

      static constexpr auto text_is_truncated = text.is_truncated();
      CHECK_FALSE(text_is_truncated);

      static constexpr auto text_is_secure = text.is_secure();
      CHECK_FALSE(text_is_secure);

      static constexpr const char* text_data = text.data();
      CHECK_EQUAL(&initial[0], text_data);

      static constexpr const char* text_data_end = text.data_end();
      CHECK_EQUAL(&initial[length], text_data_end);

      static constexpr auto text_begin = text.begin();
      CHECK_EQUAL(&initial[0], text_begin);

      static constexpr auto text_cbegin = text.cbegin();
      CHECK_EQUAL(&initial[0], text_cbegin);

      static constexpr auto text_rbegin = text.rbegin();
      CHECK_EQUAL(&initial[length - 1], etl::addressof(*text_rbegin));

      static constexpr auto text_crbegin = text.crbegin();
      CHECK_EQUAL(&initial[length - 1], etl::addressof(*text_crbegin));

      static constexpr auto text_end = text.end();
      CHECK_EQUAL(&initial[length], text_end);

      static constexpr auto text_cend = text.cend();
      CHECK_EQUAL(&initial[length], text_cend);

      static constexpr auto text_rend = text.rend();
      CHECK_EQUAL(&initial[0] - 1, etl::addressof(*text_rend));

      static constexpr auto text_crend = text.crend();
      CHECK_EQUAL(&initial[0] - 1, etl::addressof(*text_crend));

      bool is_equal = Equal(std::string(initial), text);
      CHECK(is_equal);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_contains)
    {
      static constexpr Text text("Hello World");

      static constexpr auto text_contains_world = text.contains("World");
      CHECK_TRUE(text_contains_world);

      static constexpr auto text_contains_worls = text.contains("Worls");
      CHECK_FALSE(text_contains_worls);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_start_with_ends_with)
    {
      static constexpr Text text("Hello World");

      static constexpr auto text_starts_with_hello = text.starts_with("Hello");
      CHECK_TRUE(text_starts_with_hello);

      static constexpr auto text_starts_with_world = text.starts_with("World");
      CHECK_FALSE(text_starts_with_world);

      static constexpr auto text_ends_with_hello = text.ends_with("Hello");
      CHECK_FALSE(text_ends_with_hello);

      static constexpr auto text_ends_with_world = text.ends_with("World");
      CHECK_TRUE(text_ends_with_world);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_find)
    {
      static constexpr Text text("Hello World");

      static constexpr auto text_find_world = text.find("World");
      CHECK_EQUAL(6, text_find_world);

      static constexpr auto text_find_worls = text.find("Worls");
      CHECK_EQUAL(Text::npos, text_find_worls);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_find_first_of)
    {
      static constexpr Text text("Hello World");
      static constexpr auto text_find_first_of_o = text.find_first_of(STR('o'), 5);
      CHECK_EQUAL(7, text_find_first_of_o);

      static constexpr auto text_find_first_of_s = text.find_first_of(STR('s'), 5);
      CHECK_EQUAL(Text::npos, text_find_first_of_s);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_find_first_not_of)
    {
      static constexpr Text text("Hello World");
      static constexpr auto text_find_first_not_of_l = text.find_first_not_of(STR('l'), 2);
      CHECK_EQUAL(4, text_find_first_not_of_l);

      static constexpr auto text_find_first_not_of_s = text.find_first_not_of(STR('s'), 2);
      CHECK_EQUAL(2, text_find_first_not_of_s);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_find_last_of)
    {
      static constexpr Text text("Hello World");

      static constexpr auto text_find_last_of_o = text.find_last_of(STR('o'), 9);
      CHECK_EQUAL(7, text_find_last_of_o);

      static constexpr auto text_find_last_of_s = text.find_last_of(STR('s'), 9);
      CHECK_EQUAL(Text::npos, text_find_last_of_s);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_find_last_not_of)
    {
      static constexpr Text text("Hello World");

      static constexpr auto text_find_last_not_of_l = text.find_last_not_of(STR('l'), 9);
      CHECK_EQUAL(8, text_find_last_not_of_l);

      static constexpr auto text_find_last_not_of_s = text.find_last_not_of(STR('s'), 9);
      CHECK_EQUAL(9, text_find_last_not_of_s);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_access_functions)
    {
      static constexpr const char* initial = "Hello World";
      static constexpr Text        text(initial);
      static constexpr size_t      length = etl::strlen(initial);

      static constexpr auto text_index_operator = text[3];
      CHECK_EQUAL(initial[3], text_index_operator);

      static constexpr auto text_at = text.at(3);
      CHECK_EQUAL(initial[3], text_at);

      static constexpr auto text_front = text.front();
      CHECK_EQUAL(initial[0], text_front);

      static constexpr auto text_back = text.back();
      CHECK_EQUAL(initial[length - 1], text_back);

      static constexpr const char* text_c_str = text.c_str();
      CHECK_EQUAL(&initial[0], text_c_str);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_compare_functions)
    {
      static constexpr Text text("Hello World");
      static constexpr Text text_same("Hello World");
      static constexpr Text text_less("Hello Wnrld");
      static constexpr Text text_greater("Hello Wprld");
      static constexpr Text text_less_length("Hello Worl");
      static constexpr Text text_greater_length("Hello World!");

      static constexpr auto text_compare_with_same = text.compare(text_same);
      CHECK_EQUAL(0, text_compare_with_same);

      static constexpr auto text_compare_with_less = text.compare(text_less);
      CHECK_EQUAL(1, text_compare_with_less);

      static constexpr auto text_compare_with_greater = text.compare(text_greater);
      CHECK_EQUAL(-1, text_compare_with_greater);

      static constexpr auto text_compare_with_less_length = text.compare(text_less_length);
      CHECK_EQUAL(1, text_compare_with_less_length);

      static constexpr auto text_compare_with_greater_length = text.compare(text_greater_length);
      CHECK_EQUAL(-1, text_compare_with_greater_length);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_compare_operators)
    {
      static constexpr Text text("Hello World");
      static constexpr Text text_same("Hello World");
      static constexpr Text text_less("Hello Wnrld");
      static constexpr Text text_greater("Hello Wprld");
      static constexpr Text text_less_length("Hello Worl");
      static constexpr Text text_greater_length("Hello World!");

      //static constexpr bool text_compare_with_same = (text == text_same);
      //CHECK_TRUE(text_compare_with_same);

      //static constexpr auto text_compare_with_less = text.compare(text_less);
      //CHECK_EQUAL(1, text_compare_with_less);

      //static constexpr auto text_compare_with_greater = text.compare(text_greater);
      //CHECK_EQUAL(-1, text_compare_with_greater);

      //static constexpr auto text_compare_with_less_length = text.compare(text_less_length);
      //CHECK_EQUAL(1, text_compare_with_less_length);

      //static constexpr auto text_compare_with_greater_length = text.compare(text_greater_length);
      //CHECK_EQUAL(-1, text_compare_with_greater_length);
    }
  };
}
