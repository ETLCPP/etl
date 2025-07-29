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
    using Text    = etl::const_string;
    using IText   = etl::istring;
    using TextSTD = std::string;
    using View    = etl::string_view;

    constexpr const char* text_pointer  = STR("Hello World");

    static constexpr const char* ccp_hello = STR("Hello");
    static constexpr const char* ccp_world = STR("World");
    static constexpr const char* ccp_worls = STR("Worls");

    static constexpr Text text_he(STR("He"));
    static constexpr Text text_llo_world(STR("llo World"));
    static constexpr Text text_hello(STR("Hello"));
    static constexpr Text text_word(STR("Word"));
    static constexpr Text text_rd(STR("rd"));

    static constexpr Text text(STR("Hello World"));
    static constexpr Text text_same(STR("Hello World"));
    static constexpr Text text_less(STR("Hello Wnrld"));
    static constexpr Text text_greater(STR("Hello Wprld"));
    static constexpr Text text_less_length(STR("Hello Worl"));
    static constexpr Text text_greater_length(STR("Hello World!"));

    static constexpr size_t length = etl::strlen(STR("Hello World"));

    static constexpr Text text_xxx(STR("xxxHello Worldxxx"));
    static constexpr Text text_same_xxx(STR("xxxHello World"));
    static constexpr Text text_less_xxx(STR("xxxHello Wnrldxxx"));
    static constexpr Text text_greater_xxx(STR("xxxHello Wprldxxx"));
    static constexpr Text text_less_length_xxx(STR("xxxHello Worlxxx"));
    static constexpr Text text_greater_length_xxx(STR("xxxHello World!xxx"));

    static constexpr const IText& itext = text;

    //*************************************************************************
    template <typename T1, typename T2>
    bool Equal(const T1& compare_text, const T2& text)
    {
      return (compare_text.size() == text.size()) && std::equal(text.begin(), text.end(), compare_text.begin());
    }

    //*************************************************************************
    template <typename T1, typename T2>
    bool Equal(const T1* compare_text, const T2& text)
    {
      return (etl::strlen(compare_text) == text.size()) && std::equal(text.begin(), text.end(), compare_text);
    }

    //*************************************************************************
    TEST(test_constructor_null_string)
    {
      static constexpr Text text_from_literal_empty(STR(""));

      static constexpr auto text_empty = text_from_literal_empty.empty();
      CHECK_TRUE(text_empty);

      static constexpr auto text_full = text_from_literal_empty.full();
      CHECK_TRUE(text_full);

      static constexpr auto text_capacity = text_from_literal_empty.capacity();
      CHECK_EQUAL(0, text_capacity);

      static constexpr auto text_max_size = text_from_literal_empty.max_size();
      CHECK_EQUAL(0, text_max_size);

      static constexpr auto text_size = text_from_literal_empty.size();
      CHECK_EQUAL(0, text_size);

      static constexpr auto text_length = text_from_literal_empty.length();
      CHECK_EQUAL(0, text_length);

      static constexpr auto text_available = text_from_literal_empty.available();
      CHECK_EQUAL(0, text_available);

      static constexpr auto text_is_truncated = text_from_literal_empty.is_truncated();
      CHECK_FALSE(text_is_truncated);

      static constexpr auto text_is_secure = text_from_literal_empty.is_secure();
      CHECK_FALSE(text_is_secure);
    }

    //*************************************************************************
    TEST(test_constructor_char_pointer)
    {
      static constexpr Text text_from_pointer(text_pointer);

      static constexpr auto text_empty = text_from_pointer.empty();
      CHECK_FALSE(text_empty);

      static constexpr auto text_full = text_from_pointer.full();
      CHECK_TRUE(text_full);

      static constexpr auto text_capacity = text_from_pointer.capacity();
      CHECK_EQUAL(length, text_capacity);

      static constexpr auto text_max_size = text_from_pointer.max_size();
      CHECK_EQUAL(length, text_max_size);

      static constexpr auto text_size = text_from_pointer.size();
      CHECK_EQUAL(length, text_size);

      static constexpr auto text_length = text_from_pointer.length();
      CHECK_EQUAL(length, text_length);

      static constexpr auto text_available = text_from_pointer.available();
      CHECK_EQUAL(0, text_available);

      static constexpr auto text_is_truncated = text_from_pointer.is_truncated();
      CHECK_FALSE(text_is_truncated);

      static constexpr auto text_is_secure = text_from_pointer.is_secure();
      CHECK_FALSE(text_is_secure);

      static constexpr const char* text_data = text_from_pointer.data();
      CHECK_EQUAL(&text_pointer[0], text_data);

      static constexpr const char* text_data_end = text_from_pointer.data_end();
      CHECK_EQUAL(&text_pointer[length], text_data_end);

      static constexpr auto text_begin = text_from_pointer.begin();
      CHECK_EQUAL(&text_pointer[0], text_begin);

      static constexpr auto text_cbegin = text_from_pointer.cbegin();
      CHECK_EQUAL(&text_pointer[0], text_cbegin);

      static constexpr auto text_rbegin = text_from_pointer.rbegin();
      CHECK_EQUAL(&text_pointer[length - 1], etl::addressof(*text_rbegin));

      static constexpr auto text_crbegin = text_from_pointer.crbegin();
      CHECK_EQUAL(&text_pointer[length - 1], etl::addressof(*text_crbegin));

      static constexpr auto text_end = text_from_pointer.end();
      CHECK_EQUAL(&text_pointer[length], text_end);

      static constexpr auto text_cend = text_from_pointer.cend();
      CHECK_EQUAL(&text_pointer[length], text_cend);

      static constexpr auto text_rend = text_from_pointer.rend();
      CHECK_EQUAL(&text_pointer[0] - 1, etl::addressof(*text_rend));

      static constexpr auto text_crend = text_from_pointer.crend();
      CHECK_EQUAL(&text_pointer[0] - 1, etl::addressof(*text_crend));

      bool is_equal = Equal(std::string(text_pointer), text);
      CHECK(is_equal);
    }

    //*************************************************************************
    TEST(test_constructor_from_literal)
    {
      static constexpr Text text_from_literal(STR("Hello World"));

      static constexpr auto text_empty = text_from_literal.empty();
      CHECK_FALSE(text_empty);

      static constexpr auto text_full = text_from_literal.full();
      CHECK_TRUE(text_full);

      static constexpr auto text_capacity = text_from_literal.capacity();
      CHECK_EQUAL(length, text_capacity);

      static constexpr auto text_max_size = text_from_literal.max_size();
      CHECK_EQUAL(length, text_max_size);

      static constexpr auto text_size = text_from_literal.size();
      CHECK_EQUAL(length, text_size);

      static constexpr auto text_length = text_from_literal.length();
      CHECK_EQUAL(length, text_length);

      static constexpr auto text_available = text_from_literal.available();
      CHECK_EQUAL(0, text_available);

      static constexpr auto text_is_truncated = text_from_literal.is_truncated();
      CHECK_FALSE(text_is_truncated);

      static constexpr auto text_is_secure = text_from_literal.is_secure();
      CHECK_FALSE(text_is_secure);

      static constexpr const char* text_data = text_from_literal.data();
      CHECK_EQUAL(&text_pointer[0], text_data);

      static constexpr const char* text_data_end = text_from_literal.data_end();
      CHECK_EQUAL(&text_pointer[length], text_data_end);

      static constexpr auto text_begin = text_from_literal.begin();
      CHECK_EQUAL(&text_pointer[0], text_begin);

      static constexpr auto text_cbegin = text_from_literal.cbegin();
      CHECK_EQUAL(&text_pointer[0], text_cbegin);

      static constexpr auto text_rbegin = text_from_literal.rbegin();
      CHECK_EQUAL(&text_pointer[length - 1], etl::addressof(*text_rbegin));

      static constexpr auto text_crbegin = text_from_literal.crbegin();
      CHECK_EQUAL(&text_pointer[length - 1], etl::addressof(*text_crbegin));

      static constexpr auto text_end = text_from_literal.end();
      CHECK_EQUAL(&text_pointer[length], text_end);

      static constexpr auto text_cend = text_from_literal.cend();
      CHECK_EQUAL(&text_pointer[length], text_cend);

      static constexpr auto text_rend = text_from_literal.rend();
      CHECK_EQUAL(&text_pointer[0] - 1, etl::addressof(*text_rend));

      static constexpr auto text_crend = text_from_literal.crend();
      CHECK_EQUAL(&text_pointer[0] - 1, etl::addressof(*text_crend));

      bool is_equal = Equal(std::string(text_pointer), text);
      CHECK(is_equal);
    }

    //*************************************************************************
    TEST(test_constructor_from_etl_string_view)
    {
      static constexpr Text text_from_view(View(STR("Hello World")));

      static constexpr auto text_empty = text_from_view.empty();
      CHECK_FALSE(text_empty);

      static constexpr auto text_full = text_from_view.full();
      CHECK_TRUE(text_full);

      static constexpr auto text_capacity = text_from_view.capacity();
      CHECK_EQUAL(length, text_capacity);

      static constexpr auto text_max_size = text_from_view.max_size();
      CHECK_EQUAL(length, text_max_size);

      static constexpr auto text_size = text_from_view.size();
      CHECK_EQUAL(length, text_size);

      static constexpr auto text_length = text_from_view.length();
      CHECK_EQUAL(length, text_length);

      static constexpr auto text_available = text_from_view.available();
      CHECK_EQUAL(0, text_available);

      static constexpr auto text_is_truncated = text_from_view.is_truncated();
      CHECK_FALSE(text_is_truncated);

      static constexpr auto text_is_secure = text_from_view.is_secure();
      CHECK_FALSE(text_is_secure);

      static constexpr const char* text_data = text_from_view.data();
      CHECK_EQUAL(&text_pointer[0], text_data);

      static constexpr const char* text_data_end = text_from_view.data_end();
      CHECK_EQUAL(&text_pointer[length], text_data_end);

      static constexpr auto text_begin = text_from_view.begin();
      CHECK_EQUAL(&text_pointer[0], text_begin);

      static constexpr auto text_cbegin = text_from_view.cbegin();
      CHECK_EQUAL(&text_pointer[0], text_cbegin);

      static constexpr auto text_rbegin = text_from_view.rbegin();
      CHECK_EQUAL(&text_pointer[length - 1], etl::addressof(*text_rbegin));

      static constexpr auto text_crbegin = text_from_view.crbegin();
      CHECK_EQUAL(&text_pointer[length - 1], etl::addressof(*text_crbegin));

      static constexpr auto text_end = text_from_view.end();
      CHECK_EQUAL(&text_pointer[length], text_end);

      static constexpr auto text_cend = text_from_view.cend();
      CHECK_EQUAL(&text_pointer[length], text_cend);

      static constexpr auto text_rend = text_from_view.rend();
      CHECK_EQUAL(&text_pointer[0] - 1, etl::addressof(*text_rend));

      static constexpr auto text_crend = text_from_view.crend();
      CHECK_EQUAL(&text_pointer[0] - 1, etl::addressof(*text_crend));

      bool is_equal = Equal(std::string(text_pointer), text);
      CHECK(is_equal);
    }

    //*************************************************************************
    TEST(test_begin)
    {
      static constexpr Text::const_iterator p_begin  = text.begin();
      static constexpr Text::const_iterator p_cbegin = text.cbegin();

      CHECK_EQUAL(&text[0], p_begin);
      CHECK_EQUAL(&text[0], p_cbegin);
    }

    //*************************************************************************
    TEST(test_end)
    {
      static constexpr Text::const_iterator p_end  = text.end();
      static constexpr Text::const_iterator p_cend = text.cend();

      CHECK_EQUAL(&text[length], p_end);
      CHECK_EQUAL(&text[length], p_cend);
    }

    //*************************************************************************
    TEST(test_empty_full)
    {
      static constexpr bool text_empty = text.empty();
      static constexpr bool text_full  = text.full();

      CHECK_FALSE(text_empty);
      CHECK_TRUE(text_full);
    }

    //*************************************************************************
    TEST(test_index)
    {
      static constexpr Text::value_type c0  = text[0];
      static constexpr Text::value_type c1  = text[1];
      static constexpr Text::value_type c2  = text[2];
      static constexpr Text::value_type c3  = text[3];
      static constexpr Text::value_type c4  = text[4];
      static constexpr Text::value_type c5  = text[5];
      static constexpr Text::value_type c6  = text[6];
      static constexpr Text::value_type c7  = text[7];
      static constexpr Text::value_type c8  = text[8];
      static constexpr Text::value_type c9  = text[9];
      static constexpr Text::value_type c10 = text[10];
      static constexpr Text::value_type c11 = text[11];

      CHECK_EQUAL(text_pointer[0],  c0);
      CHECK_EQUAL(text_pointer[1],  c1);
      CHECK_EQUAL(text_pointer[2],  c2);
      CHECK_EQUAL(text_pointer[3],  c3);
      CHECK_EQUAL(text_pointer[4],  c4);
      CHECK_EQUAL(text_pointer[5],  c5);
      CHECK_EQUAL(text_pointer[6],  c6);
      CHECK_EQUAL(text_pointer[7],  c7);
      CHECK_EQUAL(text_pointer[8],  c8);
      CHECK_EQUAL(text_pointer[9],  c9);
      CHECK_EQUAL(text_pointer[10], c10);
      CHECK_EQUAL(text_pointer[11], c11);
    }

    //*************************************************************************
    TEST(test_at)
    {
      static constexpr Text::value_type c0  = text.at(0);
      static constexpr Text::value_type c1  = text.at(1);
      static constexpr Text::value_type c2  = text.at(2);
      static constexpr Text::value_type c3  = text.at(3);
      static constexpr Text::value_type c4  = text.at(4);
      static constexpr Text::value_type c5  = text.at(5);
      static constexpr Text::value_type c6  = text.at(6);
      static constexpr Text::value_type c7  = text.at(7);
      static constexpr Text::value_type c8  = text.at(8);
      static constexpr Text::value_type c9  = text.at(9);
      static constexpr Text::value_type c10 = text.at(10);

      CHECK_EQUAL(text_pointer[0],  c0);
      CHECK_EQUAL(text_pointer[1],  c1);
      CHECK_EQUAL(text_pointer[2],  c2);
      CHECK_EQUAL(text_pointer[3],  c3);
      CHECK_EQUAL(text_pointer[4],  c4);
      CHECK_EQUAL(text_pointer[5],  c5);
      CHECK_EQUAL(text_pointer[6],  c6);
      CHECK_EQUAL(text_pointer[7],  c7);
      CHECK_EQUAL(text_pointer[8],  c8);
      CHECK_EQUAL(text_pointer[9],  c9);
      CHECK_EQUAL(text_pointer[10], c10);
    }

    //*************************************************************************
    TEST(test_front)
    {
      CHECK_TRUE(text_pointer[0] == text.front());
    }

    //*************************************************************************
    TEST(test_back)
    {
      CHECK_TRUE(text_pointer[length - 1] == text.back());
    }

    //*************************************************************************
    TEST(test_data)
    {
      static constexpr Text::const_pointer p_data = text.data();

      CHECK_EQUAL(&text[0], p_data);
    }

    //*************************************************************************
    TEST(test_const_iterator)
    {
      static constexpr Text::const_iterator itr = text.begin();

      bool is_equal = std::equal(text_pointer, text_pointer + length, itr);
      CHECK_TRUE(is_equal);
    }

    //*************************************************************************
    TEST(test_const_reverse_iterator)
    {
      static constexpr Text::const_reverse_iterator ritr = text.rbegin();

      etl::reverse_iterator<const char*> rbegin(text_pointer + length);
      etl::reverse_iterator<const char*> rend(text_pointer);

      bool is_equal = std::equal(rbegin, rend, ritr);
      CHECK_TRUE(is_equal);
    }

    //*************************************************************************
    TEST(test_equal)
    {
      static constexpr bool text_compare_with_same = (text == text_same);
      CHECK_TRUE(text_compare_with_same);

      static constexpr bool text_compare_with_less = (text == text_less);
      CHECK_FALSE(text_compare_with_less);

      static constexpr bool text_compare_with_greater = (text == text_greater);
      CHECK_FALSE(text_compare_with_greater);

      static constexpr bool text_compare_with_less_length = (text == text_less_length);
      CHECK_FALSE(text_compare_with_less_length);

      static constexpr bool text_compare_with_greater_length = (text == text_greater_length);
      CHECK_FALSE(text_compare_with_greater_length);
    }

    //*************************************************************************
    TEST(test_not_equal)
    {
      static constexpr bool text_compare_with_same = (text != text_same);
      CHECK_FALSE(text_compare_with_same);

      static constexpr bool text_compare_with_less = (text != text_less);
      CHECK_TRUE(text_compare_with_less);

      static constexpr bool text_compare_with_greater = (text != text_greater);
      CHECK_TRUE(text_compare_with_greater);

      static constexpr bool text_compare_with_less_length = (text != text_less_length);
      CHECK_TRUE(text_compare_with_less_length);

      static constexpr bool text_compare_with_greater_length = (text != text_greater_length);
      CHECK_TRUE(text_compare_with_greater_length);
    }

    //*************************************************************************
    TEST(test_less_than)
    {
      static constexpr bool text_compare_with_same = (text < text_same);
      CHECK_FALSE(text_compare_with_same);

      static constexpr bool text_compare_with_less = (text < text_less);
      CHECK_FALSE(text_compare_with_less);

      static constexpr bool text_compare_with_greater = (text < text_greater);
      CHECK_TRUE(text_compare_with_greater);

      static constexpr bool text_compare_with_less_length = (text < text_less_length);
      CHECK_FALSE(text_compare_with_less_length);

      static constexpr bool text_compare_with_greater_length = (text < text_greater_length);
      CHECK_TRUE(text_compare_with_greater_length);
    }

    //*************************************************************************
    TEST(test_less_than_or_equal)
    {
      static constexpr bool text_compare_with_same = (text <= text_same);
      CHECK_TRUE(text_compare_with_same);

      static constexpr bool text_compare_with_less = (text <= text_less);
      CHECK_FALSE(text_compare_with_less);

      static constexpr bool text_compare_with_greater = (text <= text_greater);
      CHECK_TRUE(text_compare_with_greater);

      static constexpr bool text_compare_with_less_length = (text <= text_less_length);
      CHECK_FALSE(text_compare_with_less_length);

      static constexpr bool text_compare_with_greater_length = (text <= text_greater_length);
      CHECK_TRUE(text_compare_with_greater_length);
    }

    //*************************************************************************
    TEST(test_greater_than)
    {
      static constexpr bool text_compare_with_same = (text > text_same);
      CHECK_FALSE(text_compare_with_same);

      static constexpr bool text_compare_with_less = (text > text_less);
      CHECK_TRUE(text_compare_with_less);

      static constexpr bool text_compare_with_greater = (text > text_greater);
      CHECK_FALSE(text_compare_with_greater);

      static constexpr bool text_compare_with_less_length = (text > text_less_length);
      CHECK_TRUE(text_compare_with_less_length);

      static constexpr bool text_compare_with_greater_length = (text > text_greater_length);
      CHECK_FALSE(text_compare_with_greater_length);
    }

    //*************************************************************************
    TEST(test_greater_than_or_equal)
    {
      static constexpr bool text_compare_with_same = (text >= text_same);
      CHECK_TRUE(text_compare_with_same);

      static constexpr bool text_compare_with_less = (text >= text_less);
      CHECK_TRUE(text_compare_with_less);

      static constexpr bool text_compare_with_greater = (text >= text_greater);
      CHECK_FALSE(text_compare_with_greater);

      static constexpr bool text_compare_with_less_length = (text >= text_less_length);
      CHECK_TRUE(text_compare_with_less_length);

      static constexpr bool text_compare_with_greater_length = (text >= text_greater_length);
      CHECK_FALSE(text_compare_with_greater_length);
    }

    //*************************************************************************
    TEST(test_find_string)
    {
      static constexpr Text::const_pointer the_haystack = STR("A haystack with a needle and another needle");
      static constexpr Text                haystack(the_haystack);

      static constexpr Text needle(STR("needle"));
      static constexpr Text pin(STR("pin"));

      static constexpr size_t position_needle = haystack.find(needle);
      CHECK_EQUAL(18, position_needle);

      static constexpr size_t position_pin = haystack.find(pin);
      CHECK_EQUAL(Text::npos, position_pin);
    }

    //*************************************************************************
    TEST(test_find_string_position)
    {
      static constexpr Text::const_pointer the_haystack = STR("A haystack with a needle and another needle");
      static constexpr Text                haystack(the_haystack);

      static constexpr Text needle(STR("needle"));
      static constexpr Text pin(STR("pin"));

      static constexpr size_t position_needle1 = haystack.find(needle, 0ULL);
      CHECK_EQUAL(18, position_needle1);

      static constexpr size_t position_needle2 = haystack.find(needle, position_needle1 + 1);
      CHECK_EQUAL(37, position_needle2);

      static constexpr size_t position_pin = haystack.find(pin, 0ULL);
      CHECK_EQUAL(Text::npos, position_pin);
    }

    //*************************************************************************
    TEST(test_find_view)
    {
      static constexpr Text::const_pointer the_haystack = STR("A haystack with a needle and another needle");
      static constexpr Text                haystack(the_haystack);

      static constexpr View needle(STR("needle"));
      static constexpr View pin(STR("pin"));

      static constexpr size_t position_needle = haystack.find(needle);
      CHECK_EQUAL(18, position_needle);

      static constexpr size_t position_pin = haystack.find(pin);
      CHECK_EQUAL(Text::npos, position_pin);
    }

    //*************************************************************************
    TEST(test_find_view_position)
    {
      static constexpr Text::const_pointer the_haystack = STR("A haystack with a needle and another needle");
      static constexpr Text                haystack(the_haystack);

      static constexpr View needle(STR("needle"));
      static constexpr View pin(STR("pin"));

      static constexpr size_t position_needle1 = haystack.find(needle, 0ULL);
      CHECK_EQUAL(18, position_needle1);

      static constexpr size_t position_needle2 = haystack.find(needle, position_needle1 + 1);
      CHECK_EQUAL(37, position_needle2);

      static constexpr size_t position_pin = haystack.find(pin, 0ULL);
      CHECK_EQUAL(Text::npos, position_pin);
    }

    //*************************************************************************
    TEST(test_find_pointer)
    {
      static constexpr Text::const_pointer the_haystack = STR("A haystack with a needle and another needle");
      static constexpr Text                haystack(the_haystack);

      static constexpr Text::const_pointer needle(STR("needle"));
      static constexpr Text::const_pointer pin(STR("pin"));

      static constexpr size_t position_needle = haystack.find(needle);
      CHECK_EQUAL(18, position_needle);

      static constexpr size_t position_pin = haystack.find(pin);
      CHECK_EQUAL(Text::npos, position_pin);
    }

    //*************************************************************************
    TEST(test_find_pointer_position)
    {
      static constexpr Text::const_pointer the_haystack = STR("A haystack with a needle and another needle");
      static constexpr Text                haystack(the_haystack);

      static constexpr Text::const_pointer needle(STR("needle"));
      static constexpr Text::const_pointer pin(STR("pin"));

      static constexpr size_t position_needle1 = haystack.find(needle, 0ULL);
      CHECK_EQUAL(18, position_needle1);

      static constexpr size_t position_needle2 = haystack.find(needle, position_needle1 + 1);
      CHECK_EQUAL(37, position_needle2);

      static constexpr size_t position_pin = haystack.find(pin, 0ULL);
      CHECK_EQUAL(Text::npos, position_pin);
    }

    //*************************************************************************
    TEST(test_find_pointer_position_n)
    {
      static constexpr Text::const_pointer the_haystack = STR("A haystack with a needle and another needle");
      static constexpr Text                haystack(the_haystack);

      static constexpr Text::const_pointer needle(STR("needle"));
      static constexpr Text::const_pointer pin(STR("pin"));

      static constexpr size_t position_needle1 = haystack.find(needle, 0ULL, 4);
      CHECK_EQUAL(18, position_needle1);

      static constexpr size_t position_needle2 = haystack.find(needle, position_needle1 + 1, 4);
      CHECK_EQUAL(37, position_needle2);

      static constexpr size_t position_pin = haystack.find(pin, 0ULL);
      CHECK_EQUAL(Text::npos, position_pin);
    }

    //*************************************************************************
    TEST(test_find_character)
    {
      static constexpr Text::const_pointer the_haystack = STR("A haystack with a needle and another needle");
      static constexpr Text                haystack(the_haystack);

      static constexpr Text::value_type needle(STR('n'));
      static constexpr Text::value_type pin(STR('p'));

      static constexpr size_t position_needle = haystack.find(needle);
      CHECK_EQUAL(18, position_needle);

      static constexpr size_t position_pin = haystack.find(pin);
      CHECK_EQUAL(Text::npos, position_pin);
    }

    //*************************************************************************
    TEST(test_find_character_position)
    {
      static constexpr Text::const_pointer the_haystack = STR("A haystack with a needle and another needle");
      static constexpr Text                haystack(the_haystack);

      static constexpr Text::value_type needle(STR('n'));
      static constexpr Text::value_type pin(STR('p'));

      static constexpr size_t position_needle1 = haystack.find(needle, 0ULL);
      CHECK_EQUAL(18, position_needle1);

      static constexpr size_t position_needle2 = haystack.find(needle, position_needle1 + 1);
      CHECK_EQUAL(26, position_needle2);

      static constexpr size_t position_pin = haystack.find(pin, 0ULL);
      CHECK_EQUAL(Text::npos, position_pin);
    }

    //*************************************************************************
    TEST(test_contains_string)
    {
      static constexpr Text::const_pointer the_haystack = (STR("A haystack with a needle and nothing else"));
      static constexpr Text                haystack(the_haystack);

      static constexpr Text needle(STR("needle"));
      static constexpr Text pin(STR("pin" ));
      static constexpr Text excess(STR("A really gigantic pin or needle that's really really big"));     

      static constexpr bool contains_needle = haystack.contains(needle);
      static constexpr bool constains_pin   = haystack.contains(pin);
      static constexpr bool contains_excess = haystack.contains(excess);

      CHECK_TRUE(contains_needle);
      CHECK_FALSE(constains_pin);
      CHECK_FALSE(contains_excess);
    }

    //*************************************************************************
    TEST(test_contains_view)
    {
      static constexpr Text::const_pointer the_haystack = (STR("A haystack with a needle and nothing else"));
      static constexpr Text                haystack(the_haystack);

      static constexpr View needle(STR("needle"));
      static constexpr View pin(STR("pin" ));
      static constexpr View excess(STR("A really gigantic pin or needle that's really really big"));     

      static constexpr bool contains_needle = haystack.contains(needle);
      static constexpr bool constains_pin   = haystack.contains(pin);
      static constexpr bool contains_excess = haystack.contains(excess);

      CHECK_TRUE(contains_needle);
      CHECK_FALSE(constains_pin);
      CHECK_FALSE(contains_excess);
    }

    //*************************************************************************
    TEST(test_contains_pointer)
    {
      static constexpr Text::const_pointer the_haystack = (STR("A haystack with a needle and nothing else"));
      static constexpr Text                haystack(the_haystack);

      static constexpr Text::const_pointer needle(STR("needle"));
      static constexpr Text::const_pointer pin(STR("pin" ));
      static constexpr Text::const_pointer excess(STR("A really gigantic pin or needle that's really really big"));     

      static constexpr bool contains_needle = haystack.contains(needle);
      static constexpr bool constains_pin   = haystack.contains(pin);
      static constexpr bool contains_excess = haystack.contains(excess);

      CHECK_TRUE(contains_needle);
      CHECK_FALSE(constains_pin);
      CHECK_FALSE(contains_excess);
    }

    //*************************************************************************
    TEST(test_contains_char)
    {
      static constexpr Text::const_pointer the_haystack = (STR("A haystack with a needle and nothing else"));
      static constexpr Text                haystack(the_haystack);

      static constexpr Text::value_type needle(STR('n'));
      static constexpr Text::value_type pin(STR('p' ));

      static constexpr bool contains_needle = haystack.contains(needle);
      static constexpr bool constains_pin   = haystack.contains(pin);

      CHECK_TRUE(contains_needle);
      CHECK_FALSE(constains_pin);
    }

    //*************************************************************************
    TEST(test_starts_with_string)
    {
      static constexpr Text::const_pointer the_haystack = (STR("A haystack with a needle and nothing else"));
      static constexpr Text                haystack(the_haystack);

      static constexpr Text start_text(STR("A haystack"));
      static constexpr Text not_start_text(STR("nothing else"));
      static constexpr Text excess(STR("A really gigantic pin or needle that's really really big"));     

      static constexpr bool contains_start_text      = haystack.starts_with(start_text);
      static constexpr bool constains_not_start_text = haystack.starts_with(not_start_text);
      static constexpr bool contains_excess          = haystack.starts_with(excess);

      CHECK_TRUE(contains_start_text);
      CHECK_FALSE(constains_not_start_text);
      CHECK_FALSE(contains_excess);
    }

    //*************************************************************************
    TEST(test_starts_with_view)
    {
      static constexpr Text::const_pointer the_haystack = (STR("A haystack with a needle and nothing else"));
      static constexpr Text                haystack(the_haystack);

      static constexpr View start_text(STR("A haystack"));
      static constexpr View not_start_text(STR("nothing else"));
      static constexpr View excess(STR("A really gigantic pin or needle that's really really big"));     

      static constexpr bool contains_start_text      = haystack.starts_with(start_text);
      static constexpr bool constains_not_start_text = haystack.starts_with(not_start_text);

      CHECK_TRUE(contains_start_text);
      CHECK_FALSE(constains_not_start_text);
    }

    //*************************************************************************
    TEST(test_starts_with_pointer)
    {
      static constexpr Text::const_pointer the_haystack = (STR("A haystack with a needle and nothing else"));
      static constexpr Text                haystack(the_haystack);

      static constexpr Text::const_pointer start_text(STR("A haystack"));
      static constexpr Text::const_pointer not_start_text(STR("nothing else"));
      static constexpr Text::const_pointer excess(STR("A really gigantic pin or needle that's really really big"));     

      static constexpr bool contains_start_text      = haystack.starts_with(start_text);
      static constexpr bool constains_not_start_text = haystack.starts_with(not_start_text);

      CHECK_TRUE(contains_start_text);
      CHECK_FALSE(constains_not_start_text);
    }

    //*************************************************************************
    TEST(test_starts_with_char)
    {
      static constexpr Text::const_pointer the_haystack = (STR("A haystack with a needle and nothing else"));
      static constexpr Text                haystack(the_haystack);

      static constexpr Text::value_type start_text(STR('A'));
      static constexpr Text::value_type not_start_text(STR('e'));

      static constexpr bool contains_start_text      = haystack.starts_with(start_text);
      static constexpr bool constains_not_start_text = haystack.starts_with(not_start_text);

      CHECK_TRUE(contains_start_text);
      CHECK_FALSE(constains_not_start_text);
    }

    //*************************************************************************
    TEST(test_ends_with_string)
    {
      static constexpr Text::const_pointer the_haystack = (STR("A haystack with a needle and nothing else"));
      static constexpr Text                haystack(the_haystack);

      static constexpr Text not_end_text(STR("A haystack"));
      static constexpr Text end_text(STR("nothing else"));
      static constexpr Text excess(STR("A really gigantic pin or needle that's really really big"));     

      static constexpr bool contains_end_text      = haystack.ends_with(end_text);
      static constexpr bool constains_not_end_text = haystack.ends_with(not_end_text);
      static constexpr bool contains_excess        = haystack.ends_with(excess);

      CHECK_TRUE(contains_end_text);
      CHECK_FALSE(constains_not_end_text);
      CHECK_FALSE(contains_excess);
    }

    //*************************************************************************
    TEST(test_ends_with_view)
    {
      static constexpr Text::const_pointer the_haystack = (STR("A haystack with a needle and nothing else"));
      static constexpr Text                haystack(the_haystack);

      static constexpr View not_end_text(STR("A haystack"));
      static constexpr View end_text(STR("nothing else"));
      static constexpr View excess(STR("A really gigantic pin or needle that's really really big"));     

      static constexpr bool contains_end_text      = haystack.ends_with(end_text);
      static constexpr bool constains_not_end_text = haystack.ends_with(not_end_text);
      static constexpr bool contains_excess        = haystack.ends_with(excess);

      CHECK_TRUE(contains_end_text);
      CHECK_FALSE(constains_not_end_text);
      CHECK_FALSE(contains_excess);
    }

    //*************************************************************************
    TEST(test_ends_with_pointer)
    {
      static constexpr Text::const_pointer the_haystack = (STR("A haystack with a needle and nothing else"));
      static constexpr Text                haystack(the_haystack);

      static constexpr Text::const_pointer not_end_text(STR("A haystack"));
      static constexpr Text::const_pointer end_text(STR("nothing else"));
      static constexpr Text::const_pointer excess(STR("A really gigantic pin or needle that's really really big"));     

      static constexpr bool contains_end_text      = haystack.ends_with(end_text);
      static constexpr bool constains_not_end_text = haystack.ends_with(not_end_text);
      static constexpr bool contains_excess        = haystack.ends_with(excess);

      CHECK_TRUE(contains_end_text);
      CHECK_FALSE(constains_not_end_text);
      CHECK_FALSE(contains_excess);
    }

    //*************************************************************************
    TEST(test_ends_with_char)
    {
      static constexpr Text::const_pointer the_haystack = (STR("A haystack with a needle and nothing else"));
      static constexpr Text                haystack(the_haystack);

      static constexpr Text::value_type not_end_text(STR('A'));
      static constexpr Text::value_type end_text(STR('e'));

      static constexpr bool contains_end_text      = haystack.ends_with(end_text);
      static constexpr bool constains_not_end_text = haystack.ends_with(not_end_text);

      CHECK_TRUE(contains_end_text);
      CHECK_FALSE(constains_not_end_text);
    }

    //*************************************************************************
    TEST(test_rfind_string)
    {
      static constexpr Text::const_pointer the_haystack = STR("A haystack with a needle and another needle");
      static constexpr Text                haystack(the_haystack);

      static constexpr Text needle(STR("needle"));
      static constexpr Text pin(STR("pin"));

      static constexpr size_t position_needle = haystack.rfind(needle);
      CHECK_EQUAL(37, position_needle);

      static constexpr size_t position_pin = haystack.rfind(pin);
      CHECK_EQUAL(Text::npos, position_pin);
    }

    //*************************************************************************
    TEST(test_rfind_string_position)
    {
      static constexpr Text::const_pointer the_haystack = STR("A haystack with a needle and another needle");
      static constexpr Text                haystack(the_haystack);

      static constexpr Text needle(STR("needle"));
      static constexpr Text pin(STR("pin"));

      static constexpr size_t position_needle1 = haystack.rfind(needle, Text::npos);
      CHECK_EQUAL(37, position_needle1);

      static constexpr size_t position_needle2 = haystack.rfind(needle, position_needle1);
      CHECK_EQUAL(18, position_needle2);

      static constexpr size_t position_pin = haystack.rfind(pin, 0ULL);
      CHECK_EQUAL(Text::npos, position_pin);
    }

    //*************************************************************************
    TEST(test_rfind_view)
    {
      static constexpr Text::const_pointer the_haystack = STR("A haystack with a needle and another needle");
      static constexpr Text                haystack(the_haystack);

      static constexpr View needle(STR("needle"));
      static constexpr View pin(STR("pin"));

      static constexpr size_t position_needle = haystack.rfind(needle);
      CHECK_EQUAL(37, position_needle);

      static constexpr size_t position_pin = haystack.rfind(pin);
      CHECK_EQUAL(Text::npos, position_pin);
    }

    //*************************************************************************
    TEST(test_rfind_view_position)
    {
      static constexpr Text::const_pointer the_haystack = STR("A haystack with a needle and another needle");
      static constexpr Text                haystack(the_haystack);

      static constexpr View needle(STR("needle"));
      static constexpr View pin(STR("pin"));

      static constexpr size_t position_needle1 = haystack.rfind(needle, Text::npos);
      CHECK_EQUAL(37, position_needle1);

      static constexpr size_t position_needle2 = haystack.rfind(needle, position_needle1);
      CHECK_EQUAL(18, position_needle2);

      static constexpr size_t position_pin = haystack.rfind(pin, Text::npos);
      CHECK_EQUAL(Text::npos, position_pin);
    }

    //*************************************************************************
    TEST(test_rfind_pointer)
    {
      static constexpr Text::const_pointer the_haystack = STR("A haystack with a needle and another needle");
      static constexpr Text                haystack(the_haystack);

      static constexpr Text::const_pointer needle(STR("needle"));
      static constexpr Text::const_pointer pin(STR("pin"));

      static constexpr size_t position_needle1 = haystack.rfind(needle, Text::npos);
      CHECK_EQUAL(37, position_needle1);

      static constexpr size_t position_needle2 = haystack.rfind(needle, position_needle1);
      CHECK_EQUAL(18, position_needle2);

      static constexpr size_t position_pin = haystack.rfind(pin, Text::npos);
      CHECK_EQUAL(Text::npos, position_pin);
    }

    //*************************************************************************
    TEST(test_rfind_pointer_n)
    {
      static constexpr Text::const_pointer the_haystack = STR("A haystack with a needle and another needle");
      static constexpr Text                haystack(the_haystack);

      static constexpr Text::const_pointer needle(STR("needle"));
      static constexpr Text::const_pointer pin(STR("pin"));

      static constexpr size_t position_needle1 = haystack.rfind(needle, Text::npos, 4);
      CHECK_EQUAL(37, position_needle1);

      static constexpr size_t position_needle2 = haystack.rfind(needle, position_needle1 + 1, 4);
      CHECK_EQUAL(18, position_needle2);

      static constexpr size_t position_pin = haystack.rfind(pin, Text::npos, 4);
      CHECK_EQUAL(Text::npos, position_pin);
    }
    
    //*************************************************************************
    TEST(test_rfind_character)
    {
      static constexpr Text::const_pointer the_haystack = STR("A haystack with a needle and another needle");
      static constexpr Text                haystack(the_haystack);

      static constexpr Text::value_type needle(STR('n'));
      static constexpr Text::value_type pin(STR('p'));

      static constexpr size_t position_needle = haystack.rfind(needle);
      CHECK_EQUAL(37, position_needle);

      static constexpr size_t position_pin = haystack.rfind(pin);
      CHECK_EQUAL(Text::npos, position_pin);
    }

    //*************************************************************************
    TEST(test_rfind_character_position)
    {
      static constexpr Text::const_pointer the_haystack = STR("A haystack with a needle and another needle");
      static constexpr Text                haystack(the_haystack);

      static constexpr Text::value_type needle(STR('n'));
      static constexpr Text::value_type pin(STR('p'));

      static constexpr size_t position_needle1 = haystack.rfind(needle, Text::npos);
      CHECK_EQUAL(37, position_needle1);

      static constexpr size_t position_needle2 = haystack.rfind(needle, position_needle1);
      CHECK_EQUAL(30, position_needle2);

      static constexpr size_t position_pin = haystack.rfind(pin, Text::npos);
      CHECK_EQUAL(Text::npos, position_pin);
    }

    //*************************************************************************
    TEST(test_substr)
    {
      // Equal.
      static constexpr Text sub1(text.substr(text.size()));
      CHECK_EQUAL(0, sub1.size());

      // Whole string.
      static constexpr Text sub2(text.substr());
      CHECK_TRUE(Equal(text, sub2));

      // Starting from position 2.
      static constexpr Text sub3(text.substr(2));
      CHECK_TRUE(Equal(View(text.begin() + 2, text.end()), sub3));

      // Starting from position 2 for 3 characters.
      static constexpr Text sub4(text.substr(2, 3));
      CHECK_TRUE(Equal(View(text.begin() + 2, text.begin() + 2 + 3), sub4));

      // Starting from position 2 for too many characters.
      static constexpr Text sub5(text.substr(2, text.size()));
      CHECK_TRUE(Equal(View(text.begin() + 2, text.end()), sub3));
    }

    //*************************************************************************
    TEST(test_compare_string)
    {
      // Equal.
      static constexpr auto text_compare_with_same = text.compare(text_same);
      CHECK_EQUAL(0, text_compare_with_same);

      // Less.
      static constexpr auto text_compare_with_less = text.compare(text_less);
      CHECK_EQUAL(1, text_compare_with_less);

      // Greater.
      static constexpr auto text_compare_with_greater = text.compare(text_greater);
      CHECK_EQUAL(-1, text_compare_with_greater);

      // Shorter
      static constexpr auto text_compare_with_less_length = text.compare(text_less_length);
      CHECK_EQUAL(1, text_compare_with_less_length);

      // Longer.
      static constexpr auto text_compare_with_greater_length = text.compare(text_greater_length);
      CHECK_EQUAL(-1, text_compare_with_greater_length);
    }

    //*************************************************************************
    TEST(test_compare_position_length_string)
    {
      // Equal.
      static constexpr auto text_compare_with_same = text_xxx.compare(3, length, text_same);
      CHECK_EQUAL(0, text_compare_with_same);

      // Less.
      static constexpr auto text_compare_with_less = text_xxx.compare(3, length, text_less);
      CHECK_EQUAL(1, text_compare_with_less);

      // Greater.
      static constexpr auto text_compare_with_greater = text_xxx.compare(3, length, text_greater);
      CHECK_EQUAL(-1, text_compare_with_greater);

      // Shorter
      static constexpr auto text_compare_with_less_length = text_xxx.compare(3, length, text_less_length);
      CHECK_EQUAL(1, text_compare_with_less_length);

      // Longer.
      static constexpr auto text_compare_with_greater_length = text_xxx.compare(3, length, text_greater_length);
      CHECK_EQUAL(-1, text_compare_with_greater_length);
    }
    
    //*************************************************************************
    TEST(test_compare_position_length_string_subposition_sublength)
    {
      // Equal.
      static constexpr auto text_compare_with_same = text_xxx.compare(3, length, text_same_xxx, 3, text_same.size());
      CHECK_EQUAL(0, text_compare_with_same);

      // Less.
      static constexpr auto text_compare_with_less = text_xxx.compare(3, length, text_less_xxx, 3, text_less.size());
      CHECK_EQUAL(1, text_compare_with_less);

      // Greater.
      static constexpr auto text_compare_with_greater = text_xxx.compare(3, length, text_greater_xxx, 3, text_greater.size());
      CHECK_EQUAL(-1, text_compare_with_greater);

      // Shorter
      static constexpr auto text_compare_with_less_length = text_xxx.compare(3, length, text_less_length_xxx, 3, text_less_length.size());
      CHECK_EQUAL(1, text_compare_with_less_length);

      // Longer.
      static constexpr auto text_compare_with_greater_length = text_xxx.compare(3, length, text_greater_length_xxx, 3, text_greater_length.size());
      CHECK_EQUAL(-1, text_compare_with_greater_length);
    }
    
    //*************************************************************************
    TEST(test_compare_view)
    {
      // Equal.
      static constexpr auto text_compare_with_same = text.compare(View(text_same));
      CHECK_EQUAL(0, text_compare_with_same);

      // Less.
      static constexpr auto text_compare_with_less = text.compare(View(text_less));
      CHECK_EQUAL(1, text_compare_with_less);

      // Greater.
      static constexpr auto text_compare_with_greater = text.compare(View(text_greater));
      CHECK_EQUAL(-1, text_compare_with_greater);

      // Shorter
      static constexpr auto text_compare_with_less_length = text.compare(View(text_less_length));
      CHECK_EQUAL(1, text_compare_with_less_length);

      // Longer.
      static constexpr auto text_compare_with_greater_length = text.compare(View(text_greater_length));
      CHECK_EQUAL(-1, text_compare_with_greater_length);
    }

    //*************************************************************************
    TEST(test_compare_position_length_view)
    {
      // Equal.
      static constexpr auto text_compare_with_same = text_xxx.compare(3, length, View(text_same));
      CHECK_EQUAL(0, text_compare_with_same);

      // Less.
      static constexpr auto text_compare_with_less = text_xxx.compare(3, length, View(text_less));
      CHECK_EQUAL(1, text_compare_with_less);

      // Greater.
      static constexpr auto text_compare_with_greater = text_xxx.compare(3, length, View(text_greater));
      CHECK_EQUAL(-1, text_compare_with_greater);

      // Shorter
      static constexpr auto text_compare_with_less_length = text_xxx.compare(3, length, View(text_less_length));
      CHECK_EQUAL(1, text_compare_with_less_length);

      // Longer.
      static constexpr auto text_compare_with_greater_length = text_xxx.compare(3, length, View(text_greater_length));
      CHECK_EQUAL(-1, text_compare_with_greater_length);
    }

    //*************************************************************************
    TEST(test_compare_position_length_view_subposition_sublength)
    {
      // Equal.
      static constexpr auto text_compare_with_same = text_xxx.compare(3, length, View(text_same_xxx), 3, text_same.size());
      CHECK_EQUAL(0, text_compare_with_same);

      // Less.
      static constexpr auto text_compare_with_less = text_xxx.compare(3, length, View(text_less_xxx), 3, text_less.size());
      CHECK_EQUAL(1, text_compare_with_less);

      // Greater.
      static constexpr auto text_compare_with_greater = text_xxx.compare(3, length, View(text_greater_xxx), 3, text_greater.size());
      CHECK_EQUAL(-1, text_compare_with_greater);

      // Shorter
      static constexpr auto text_compare_with_less_length = text_xxx.compare(3, length, View(text_less_length_xxx), 3, text_less_length.size());
      CHECK_EQUAL(1, text_compare_with_less_length);

      // Longer.
      static constexpr auto text_compare_with_greater_length = text_xxx.compare(3, length, View(text_greater_length_xxx), 3, text_greater_length.size());
      CHECK_EQUAL(-1, text_compare_with_greater_length);
    }

    //*************************************************************************
    TEST(test_compare_c_string)
    {
      // Equal.
      static constexpr auto text_compare_with_same = text.compare(text_same.data());
      CHECK_EQUAL(0, text_compare_with_same);

      // Less.
      static constexpr auto text_compare_with_less = text.compare(text_less.data());
      CHECK_EQUAL(1, text_compare_with_less);

      // Greater.
      static constexpr auto text_compare_with_greater = text.compare(text_greater.data());
      CHECK_EQUAL(-1, text_compare_with_greater);

      // Shorter
      static constexpr auto text_compare_with_less_length = text.compare(text_less_length.data());
      CHECK_EQUAL(1, text_compare_with_less_length);

      // Longer.
      static constexpr auto text_compare_with_greater_length = text.compare(text_greater_length.data());
      CHECK_EQUAL(-1, text_compare_with_greater_length);
    }

    //*************************************************************************
    TEST(test_compare_position_length_c_string)
    {
      // Equal.
      static constexpr auto text_compare_with_same = text_xxx.compare(3, length, text_same.data());
      CHECK_EQUAL(0, text_compare_with_same);

      // Less.
      static constexpr auto text_compare_with_less = text_xxx.compare(3, length, text_less.data());
      CHECK_EQUAL(1, text_compare_with_less);

      // Greater.
      static constexpr auto text_compare_with_greater = text_xxx.compare(3, length, text_greater.data());
      CHECK_EQUAL(-1, text_compare_with_greater);

      // Shorter
      static constexpr auto text_compare_with_less_length = text_xxx.compare(3, length, text_less_length.data());
      CHECK_EQUAL(1, text_compare_with_less_length);

      // Longer.
      static constexpr auto text_compare_with_greater_length = text_xxx.compare(3, length, text_greater_length.data());
      CHECK_EQUAL(-1, text_compare_with_greater_length);
    }

    //*************************************************************************
    TEST(test_compare_position_length_c_string_n)
    {
      // Equal.
      static constexpr auto text_compare_with_same = text_xxx.compare(3, length, text_same.data(), text_same.size());
      CHECK_EQUAL(0, text_compare_with_same);

      // Less.
      static constexpr auto text_compare_with_less = text_xxx.compare(3, length, text_less.data(), text_less.size());
      CHECK_EQUAL(1, text_compare_with_less);

      // Greater.
      static constexpr auto text_compare_with_greater = text_xxx.compare(3, length, text_greater.data(), text_greater.size());
      CHECK_EQUAL(-1, text_compare_with_greater);

      // Shorter
      static constexpr auto text_compare_with_less_length = text_xxx.compare(3, length, text_less_length.data(), text_less_length.size());
      CHECK_EQUAL(1, text_compare_with_less_length);

      // Longer.
      static constexpr auto text_compare_with_greater_length = text_xxx.compare(3, length, text_greater_length.data(), text_greater_length.size());
      CHECK_EQUAL(-1, text_compare_with_greater_length);
    }

    //*************************************************************************
    TEST(test_find_first_of_string_position)
    {
      static constexpr auto text_find_first_of_word = text.find_first_of(text_word, 2);
      CHECK_EQUAL(4, text_find_first_of_word);

      static constexpr auto text_find_first_of_he = text.find_first_of(text_he, 2);
      CHECK_EQUAL(Text::npos, text_find_first_of_he);
    }

    //*************************************************************************
    TEST(test_find_first_of_view_position)
    {
      static constexpr auto text_find_first_of_word = text.find_first_of(View(text_word), 2);
      CHECK_EQUAL(4, text_find_first_of_word);

      static constexpr auto text_find_first_of_he = text.find_first_of(View(text_he), 2);
      CHECK_EQUAL(Text::npos, text_find_first_of_he);
    }

    //*************************************************************************
    TEST(test_find_first_of_pointer_position)
    {
      static constexpr auto text_find_first_of_word = text.find_first_of(text_word.data(), 2);
      CHECK_EQUAL(4, text_find_first_of_word);

      static constexpr auto text_find_first_of_he = text.find_first_of(text_he.data(), 2);
      CHECK_EQUAL(Text::npos, text_find_first_of_he);
    }

    //*************************************************************************
    TEST(test_find_first_of_pointer_position_n)
    {
      static constexpr auto text_find_first_of_word = text.find_first_of(text_word.data(), 2, text_word.size());
      CHECK_EQUAL(4, text_find_first_of_word);

      static constexpr auto text_find_first_of_he = text.find_first_of(text_he.data(), 2, text_he.size());
      CHECK_EQUAL(Text::npos, text_find_first_of_he);
    }

    //*************************************************************************
    TEST(test_find_first_of_character_position)
    {
      static constexpr auto text_find_first_of_o = text.find_first_of(STR('o'), 2);
      CHECK_EQUAL(4, text_find_first_of_o);

      static constexpr auto text_find_first_of_h = text.find_first_of(STR('h'), 2);
      CHECK_EQUAL(Text::npos, text_find_first_of_h);
    }

    //*************************************************************************
    TEST(test_find_last_of_string_position)
    {
      static constexpr auto text_find_last_of_word = text.find_last_of(text_word, 9);
      CHECK_EQUAL(8, text_find_last_of_word);

      static constexpr auto text_find_last_of_rd = text.find_last_of(text_rd, 7);
      CHECK_EQUAL(Text::npos, text_find_last_of_rd);
    }

    //*************************************************************************
    TEST(test_find_last_of_view_position)
    {
      static constexpr auto text_find_last_of_word = text.find_last_of(View(text_word), 9);
      CHECK_EQUAL(8, text_find_last_of_word);

      static constexpr auto text_find_last_of_rd = text.find_last_of(View(text_rd), 7);
      CHECK_EQUAL(Text::npos, text_find_last_of_rd);
    }

    //*************************************************************************
    TEST(test_find_last_of_pointer_position)
    {
      static constexpr auto text_find_last_of_word = text.find_last_of(text_word.data(), 9);
      CHECK_EQUAL(8, text_find_last_of_word);

      static constexpr auto text_find_last_of_rd = text.find_last_of(text_rd.data(), 7);
      CHECK_EQUAL(Text::npos, text_find_last_of_rd);
    }

    //*************************************************************************
    TEST(test_find_last_of_pointer_position_n)
    {
      static constexpr auto text_find_last_of_word = text.find_last_of(text_word.data(), 9, text_word.size());
      CHECK_EQUAL(8, text_find_last_of_word);

      static constexpr auto text_find_last_of_rd = text.find_last_of(text_rd.data(), 7, text_rd.size());
      CHECK_EQUAL(Text::npos, text_find_last_of_rd);
    }

    //*************************************************************************
    TEST(test_find_last_of_character_position)
    {
      static constexpr auto text_find_last_of_o = text.find_last_of(STR('o'), 9);
      CHECK_EQUAL(7, text_find_last_of_o);

      static constexpr auto text_find_last_of_h = text.find_last_of(STR('h'), 7);
      CHECK_EQUAL(Text::npos, text_find_last_of_h);
    }

    //*************************************************************************
    TEST(test_find_first_not_of_string_position)
    {
      static constexpr auto text_find_first_not_of_word = text.find_first_not_of(text_hello, 2);
      CHECK_EQUAL(5, text_find_first_not_of_word);

      static constexpr auto text_find_first_not_of_llo_world = text.find_first_not_of(text_llo_world, 2);
      CHECK_EQUAL(Text::npos, text_find_first_not_of_llo_world);
    }

    //*************************************************************************
    TEST(test_find_first_not_of_view_position)
    {
      static constexpr auto text_find_first_not_of_word = text.find_first_not_of(View(text_hello), 2);
      CHECK_EQUAL(5, text_find_first_not_of_word);

      static constexpr auto text_find_first_not_of_llo_world = text.find_first_not_of(View(text_llo_world), 2);
      CHECK_EQUAL(Text::npos, text_find_first_not_of_llo_world);
    }

    //*************************************************************************
    TEST(test_find_first_not_of_pointer_position)
    {
      static constexpr auto text_find_first_not_of_word = text.find_first_not_of(text_hello.data(), 2);
      CHECK_EQUAL(5, text_find_first_not_of_word);

      static constexpr auto text_find_first_not_of_llo_world = text.find_first_not_of(text_llo_world.data(), 2);
      CHECK_EQUAL(Text::npos, text_find_first_not_of_llo_world);
    }

    //*************************************************************************
    TEST(test_find_first_not_of_pointer_position_n)
    {
      static constexpr auto text_find_first_not_of_word = text.find_first_not_of(text_hello.data(), 2, text_hello.size());
      CHECK_EQUAL(5, text_find_first_not_of_word);

      static constexpr auto text_find_first_not_of_llo_world = text.find_first_not_of(text_llo_world.data(), 2, text_llo_world.size());
      CHECK_EQUAL(Text::npos, text_find_first_not_of_llo_world);
    }

    //*************************************************************************
    TEST(test_find_first_not_of_character_position)
    {
      static constexpr auto text_find_first_not_of_o = text.find_first_not_of(STR('l'), 2);
      CHECK_EQUAL(4, text_find_first_not_of_o);

      static constexpr auto text_find_first_not_of_h = text.find_first_not_of(STR('h'), 2);
      CHECK_EQUAL(2, text_find_first_not_of_h);
    }

//    //*************************************************************************
//    TEST(test_find_last_not_of_string_position)
//    {
//      TextSTD compare_text(STR("ABCDEFABCDE"));
//      Text text(STR("ABCDEFABCDE"));
//
//      size_t position1 = compare_text.find_last_not_of(TextSTD(STR("ZEXD")));
//      size_t position2 = text.find_last_not_of(Text(STR("ZEXD")));
//
//      CHECK_EQUAL(position1, position2);
//
//      position1 = compare_text.find_last_not_of(TextSTD(STR("ZEXD")), 3);
//      position2 = text.find_last_not_of(Text(STR("ZEXD")), 3);
//
//      CHECK_EQUAL(position1, position2);
//
//      position1 = compare_text.find_last_not_of(TextSTD(STR("ZEXD")), 5);
//      position2 = text.find_last_not_of(Text(STR("ZEXD")), 5);
//
//      CHECK_EQUAL(position1, position2);
//
//      position1 = compare_text.find_last_not_of(TextSTD(STR("ZEXD")), compare_text.size());
//      position2 = text.find_last_not_of(Text(STR("ZEXD")), text.size());
//
//      CHECK_EQUAL(position1, position2);
//
//#include "etl/private/diagnostic_array_bounds_push.h"
//      position1 = compare_text.find_last_not_of(TextSTD(STR("ZEXD")), 100);
//      position2 = text.find_last_not_of(Text(STR("ZEXD")), 100);
//
//      CHECK_EQUAL(position1, position2);
//#include "etl/private/diagnostic_pop.h"
//    }
//
//    //*************************************************************************
//    TEST(test_find_last_not_of_view_position)
//    {
//      TextSTD compare_text(STR("ABCDEFABCDE"));
//      Text text(STR("ABCDEFABCDE"));
//
//      size_t position1 = compare_text.find_last_not_of(TextSTD(STR("ZEXD")));
//      size_t position2 = text.find_last_not_of(View(STR("ZEXD")));
//
//      CHECK_EQUAL(position1, position2);
//
//      position1 = compare_text.find_last_not_of(TextSTD(STR("ZEXD")), 3);
//      position2 = text.find_last_not_of(View(STR("ZEXD")), 3);
//
//      CHECK_EQUAL(position1, position2);
//
//      position1 = compare_text.find_last_not_of(TextSTD(STR("ZEXD")), 5);
//      position2 = text.find_last_not_of(View(STR("ZEXD")), 5);
//
//      CHECK_EQUAL(position1, position2);
//
//      position1 = compare_text.find_last_not_of(TextSTD(STR("ZEXD")), compare_text.size());
//      position2 = text.find_last_not_of(View(STR("ZEXD")), text.size());
//
//      CHECK_EQUAL(position1, position2);
//
//#include "etl/private/diagnostic_array_bounds_push.h"
//      position1 = compare_text.find_last_not_of(TextSTD(STR("ZEXD")), 100);
//      position2 = text.find_last_not_of(View(STR("ZEXD")), 100);
//
//      CHECK_EQUAL(position1, position2);
//#include "etl/private/diagnostic_pop.h"
//    }
//
//    //*************************************************************************
//    TEST(test_find_last_not_of_pointer_position)
//    {
//      TextSTD compare_text(STR("ABCDEFABCDE"));
//      Text text(STR("ABCDEFABCDE"));
//
//      size_t position1 = compare_text.find_last_not_of(STR("ZEXD"));
//      size_t position2 = text.find_last_not_of(STR("ZEXD"));
//
//      CHECK_EQUAL(position1, position2);
//
//      position1 = compare_text.find_last_not_of(STR("ZEXD"), 3);
//      position2 = text.find_last_not_of(STR("ZEXD"), 3);
//
//      CHECK_EQUAL(position1, position2);
//
//      position1 = compare_text.find_last_not_of(STR("ZEXD"), 5);
//      position2 = text.find_last_not_of(STR("ZEXD"), 5);
//
//      CHECK_EQUAL(position1, position2);
//
//      position1 = compare_text.find_last_not_of(STR("ZEXD"), compare_text.size());
//      position2 = text.find_last_not_of(STR("ZEXD"), text.size());
//
//      CHECK_EQUAL(position1, position2);
//
//#include "etl/private/diagnostic_array_bounds_push.h"
//      position1 = compare_text.find_last_not_of(STR("ZEXD"), 100);
//      position2 = text.find_last_not_of(STR("ZEXD"), 100);
//
//      CHECK_EQUAL(position1, position2);
//#include "etl/private/diagnostic_pop.h"
//    }
//
//    //*************************************************************************
//    TEST(test_find_last_not_of_pointer_position_n)
//    {
//      TextSTD compare_text(STR("ABCDEFABCDE"));
//      Text text(STR("ABCDEFABCDE"));
//
//      size_t position1 = compare_text.find_last_not_of(STR("ZEXD"), 0, 4);
//      size_t position2 = text.find_last_not_of(STR("ZEXD"), 0, 4);
//
//      CHECK_EQUAL(position1, position2);
//
//      position1 = compare_text.find_last_not_of(STR("ZEXD"), 5, 3);
//      position2 = text.find_last_not_of(STR("ZEXD"), 5, 3);
//
//      CHECK_EQUAL(position1, position2);
//
//      position1 = compare_text.find_last_not_of(STR("ZEXD"), 1, 3);
//      position2 = text.find_last_not_of(STR("ZEXD"), 1, 3);
//
//      CHECK_EQUAL(position1, position2);
//
//      position1 = compare_text.find_last_not_of(STR("ZEXD"), compare_text.size(), 4);
//      position2 = text.find_last_not_of(STR("ZEXD"), text.size(), 4);
//
//      CHECK_EQUAL(position1, position2);
//
//      position1 = compare_text.find_last_not_of(STR("ZEXD"), 100, 4);
//      position2 = text.find_last_not_of(STR("ZEXD"), 100, 4);
//
//      CHECK_EQUAL(position1, position2);
//    }
//
//    //*************************************************************************
//    TEST(test_find_last_not_of_character_position)
//    {
//      TextSTD compare_text(STR("ABCDEF"));
//      Text text(STR("ABCDEF"));
//
//      size_t position1 = compare_text.find_last_not_of(STR('F'));
//      size_t position2 = text.find_last_not_of(STR('F'));
//
//      CHECK_EQUAL(position1, position2);
//
//      position1 = compare_text.find_last_not_of(STR('Z'));
//      position2 = text.find_last_not_of(STR('Z'));
//
//      CHECK_EQUAL(position1, position2);
//
//      position1 = compare_text.find_last_not_of(STR('A'), compare_text.size());
//      position2 = text.find_last_not_of(STR('A'), text.size());
//
//      CHECK_EQUAL(position1, position2);
//
//      position1 = compare_text.find_last_not_of(STR('C'), 3);
//      position2 = text.find_last_not_of(STR('C'), 3);
//
//      CHECK_EQUAL(position1, position2);
//
//      position1 = compare_text.find_last_not_of(STR('C'), compare_text.size());
//      position2 = text.find_last_not_of(STR('C'), text.size());
//
//      CHECK_EQUAL(position1, position2);
//
//#include "etl/private/diagnostic_array_bounds_push.h"
//      position1 = compare_text.find_last_not_of(STR('C'), 100);
//      position2 = text.find_last_not_of(STR('C'), 100);
//
//      CHECK_EQUAL(position1, position2);
//#include "etl/private/diagnostic_pop.h"
//    }
//
//    //*************************************************************************
//    TEST(test_hash)
//    {
//      // Test with actual string type.
//      Text text(STR("ABCDEFHIJKL"));
//      size_t hash = etl::hash<Text>()(text);
//      size_t compare_hash = etl::private_hash::generic_hash<size_t>(reinterpret_cast<const uint8_t*>(&text[0]), reinterpret_cast<const uint8_t*>(&text[text.size()]));
//      CHECK_EQUAL(compare_hash, hash);
//
//      // Test with interface string type.
//      IText& itext = text;
//      hash = etl::hash<IText>()(itext);
//      CHECK_EQUAL(compare_hash, hash);
//    }
//
//    //*************************************************************************
//    TEST(test_memcpy_repair)
//    {
//      Text text;
//
//      text.assign(STR("ABCDEF"));
//
//      char buffer[Sizeof(Text)];
//
//      memcpy(&buffer, (const void*)&text, Sizeof(text));
//
//      Text& rtext(*reinterpret_cast<Text*>(buffer));
//      rtext.repair();
//
//      CHECK(!rtext.empty());
//      CHECK(!rtext.full());
//
//      bool is_equal = Equal(text, rtext);
//      CHECK(is_equal);
//
//      text = STR("GHIJKL");
//      is_equal = Equal(text, rtext);
//      CHECK(!is_equal);
//    }
//
//    //*************************************************************************
//    TEST(test_memcpy_repair_virtual)
//    {
//      Text text;
//
//      text.assign(STR("ABCDEF"));
//
//      char buffer[Sizeof(Text)];
//
//      memcpy(&buffer, (const void*)&text, Sizeof(text));
//
//      IText& itext(*reinterpret_cast<IText*>(buffer));
//      itext.repair();
//
//      CHECK(!itext.empty());
//      CHECK(!itext.full());
//
//      bool is_equal = Equal(text, itext);
//      CHECK(is_equal);
//
//      text = STR("GHIJKL");
//      is_equal = Equal(text, itext);
//      CHECK(!is_equal);
//    }
//
//#if ETL_HAS_STRING_TRUNCATION_CHECKS
//    //*************************************************************************
//    TEST(test_truncate_over_many_operations)
//    {
//      Text text(short_text.c_str());
//      CHECK_FALSE(text.is_truncated());
//
//      text.insert(3, initial_text.c_str());
//      CHECK_TRUE(text.is_truncated());
//
//      while (text.size() != 0)
//      {
//        text.pop_back();
//        CHECK_TRUE(text.is_truncated());
//      }
//
//      text.clear();
//      CHECK_FALSE(text.is_truncated());
//
//      text.assign(longer_text.c_str());
//      CHECK_TRUE(text.is_truncated());
//
//      text.assign(short_text.c_str());
//      CHECK_FALSE(text.is_truncated());
//    }
//
//    //*************************************************************************
//    TEST(test_add_from_truncated)
//    {
//      Text  text1(short_text.c_str());
//      TextS text2(short_text.c_str());
//
//      CHECK_FALSE(text1.is_truncated());
//      CHECK_TRUE(text2.is_truncated());
//
//      // text2 has the truncate flag set.
//      text1 += text2;
//
//      CHECK(text1.is_truncated());
//    }
//
//    //*************************************************************************
//    TEST(test_add_to_truncated)
//    {
//      Text text1(longer_text.c_str());
//      Text text2(short_text.c_str());
//
//      CHECK(text1.is_truncated());
//      CHECK_FALSE(text2.is_truncated());
//
//      // Clear text but not the truncate flag.
//      text1.erase(text1.begin(), text1.end());
//
//      // text1 still has the truncate flag set.
//      text1 += text2;
//
//      CHECK(text1.is_truncated());
//    }
//
//    //*************************************************************************
//    TEST(test_clear_truncated)
//    {
//      Text text(longer_text.c_str());
//      CHECK_TRUE(text.is_truncated());
//
//      text.clear_truncated();
//      CHECK_FALSE(text.is_truncated());
//    }
//#endif
//
//#if ETL_HAS_STRING_CLEAR_AFTER_USE
//    //*************************************************************************
//    TEST(test_secure_after_destructor)
//    {
//      char buffer[Sizeof(Text)];
//      std::fill_n(buffer, Sizeof(Text), 0);
//      ::new (buffer) Text(STR("ABCDEF"));
//
//      Text& text = *reinterpret_cast<Text*>(buffer);
//      text.set_secure();
//
//      CHECK(Text(STR("ABCDEF")) == text);
//
//      Text::pointer pb = text.begin();
//      Text::pointer pe = text.end();
//
//      // Destroy the text object.
//      std::atomic_signal_fence(std::memory_order_seq_cst);
//      text.~Text();
//      std::atomic_signal_fence(std::memory_order_seq_cst);
//
//      // Check there no non-zero values in the string.
//      CHECK(std::find_if(pb, pe, [](Text::value_type x) { return x != 0; }) == pe);
//    }
//
//    //*************************************************************************
//    TEST(test_secure_after_assign)
//    {
//      Text text;
//      text.set_secure();
//      text.assign(STR("ABCDEF"));
//
//      Text::pointer pe = text.end();
//
//      text.assign(STR("ABC"));
//
//      CHECK(std::find_if(text.end(), pe, [](Text::value_type x) { return x != 0; }) == pe);
//    }
//
//    //*************************************************************************
//    TEST(test_secure_after_reSize_down)
//    {
//      Text text;
//      text.set_secure();
//      text.assign(STR("ABCDEF"));
//
//      Text::pointer pe = text.end();
//
//      text.reSize(text.size() - 3U);
//
//      CHECK(std::find_if(text.end(), pe, [](Text::value_type x) { return x != 0; }) == pe);
//    }
//
//    //*************************************************************************
//    TEST(test_secure_after_erase)
//    {
//      Text text;
//      text.set_secure();
//      text.assign(STR("ABCDEF"));
//
//      Text::pointer pb = text.begin();
//      Text::pointer pe = text.end();
//
//      text.erase(pb + 2, pb + 5);
//
//      // Check there no non-zero values in the remainder of the string.
//      CHECK(std::find_if(text.end(), pe, [](Text::value_type x) { return x != 0; }) == pe);
//    }
//
//    //*************************************************************************
//    TEST(test_secure_after_replace)
//    {
//      Text text;
//      text.set_secure();
//      text.assign(STR("ABCDEF"));
//
//      Text::pointer pb = text.begin();
//      Text::pointer pe = text.end();
//
//      text.replace(pb + 1, pb + 4, STR("G"));
//
//      // Check there no non-zero values in the remainder of the string.
//      CHECK(std::find_if(text.end(), pe, [](Text::value_type x) { return x != 0; }) == pe);
//    }
//
//    //*************************************************************************
//    TEST(test_secure_after_clear)
//    {
//      Text text;
//      text.set_secure();
//      text.assign(STR("ABCDEF"));
//
//      Text::pointer pb = text.begin();
//      Text::pointer pe = text.end();
//
//      text.clear();
//
//      // Check there no non-zero values in the remainder of the string.
//      CHECK(std::find_if(pb, pe, [](Text::value_type x) { return x != 0; }) == pe);
//    }
//
//    //*************************************************************************
//    TEST(test_secure_flag_after_copy)
//    {
//      Text text1 = STR("Hello World");
//      text1.set_secure();
//
//      Text text2(text1);
//
//      Text text3;
//      text3 = text1;
//
//      Text text4(text1, 6U, 3U);
//
//      CHECK(text2.is_secure());
//      CHECK(text3.is_secure());
//      CHECK(text4.is_secure());
//    }
//#endif
//
//    //*************************************************************************
//    TEST(test_initialize_free_space_empty_string)
//    {
//      Text empty;
//      Text text;
//
//      text.initialize_free_space();
//
//      CHECK(text.empty());
//      CHECK(text == empty);
//
//      for (size_t i = text.size(); i < text.max_size(); ++i)
//      {
//        CHECK_EQUAL(0, text[i]);
//      }
//    }
//
//    //*************************************************************************
//    TEST(test_initialize_free_space_part_filled_string)
//    {
//      Text empty;
//      Text initial = STR("ABC");
//      Text text(initial);
//
//      text.initialize_free_space();
//
//      CHECK(text == initial);
//      CHECK(text != empty);
//
//      for (size_t i = text.size(); i < text.max_size(); ++i)
//      {
//        CHECK_EQUAL(0, text[i]);
//      }
//    }
//
//    //*************************************************************************
//    TEST(test_update_after_c_string_max_size)
//    {
//      Text text;
//
//      text.initialize_free_space();
//      std::fill(text.data(), text.data() + text.max_size(), STR('A'));
//      text.trim_to_terminator();
//
//#if ETL_HAS_STRING_TRUNCATION_CHECKS
//      CHECK_FALSE(text.is_truncated());
//#endif
//      CHECK_EQUAL(text.max_size(), text.size());
//    }
//
//    //*************************************************************************
//    TEST(test_update_after_c_string_shorter_size)
//    {
//      Text text;
//
//      text.initialize_free_space();
//      std::fill(text.data(), text.data() + text.max_size() - 1, STR('A'));
//      text.trim_to_terminator();
//
//#if ETL_HAS_STRING_TRUNCATION_CHECKS
//      CHECK_FALSE(text.is_truncated());
//#endif
//      CHECK_EQUAL(text.max_size() - 1, text.size());
//    }
//
//    //*************************************************************************
//    TEST(test_update_after_c_string_greater_size)
//    {
//      Text text;
//
//      text.initialize_free_space();
//      std::fill(text.data(), text.data() + text.max_size() + 1, STR('A')); // Overwrites to terminating null.
//      text.trim_to_terminator();
//
//#if ETL_HAS_STRING_TRUNCATION_CHECKS
//      CHECK_TRUE(text.is_truncated());
//#else
//      CHECK_FALSE(text.is_truncated());
//#endif
//      CHECK_EQUAL(text.max_size(), text.size());
//    }
//
//    //*************************************************************************
//#if ETL_USING_STL
//    TEST(test_write_string_to_std_basic_ostream)
//    {
//      Text text1 = STR("Hello World");
//
//      std::stringstream sstream;
//
//      sstream << text1;
//
//      TextSTD sstream_string = sstream.str();
//
//      View sstream_view(sstream_string.data(), sstream_string.size());
//
//      CHECK(text1 == sstream_view);
//    }
//#endif
  };
}
