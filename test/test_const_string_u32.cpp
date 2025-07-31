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

#include "etl/const_u32string.h"
#include "etl/string_view.h"

#if ETL_USING_CPP14

#undef STR
#define STR(x) U##x

namespace
{
  SUITE(test_const_string_char32_t)
  {
    using Text = etl::const_u32string;
    using View = etl::u32string_view;

    using TextSTD = std::u32string;

    static constexpr Text::const_pointer text_pointer = STR("Hello World");
    static constexpr Text                text(STR("Hello World"));
    static constexpr size_t              Length = etl::strlen(STR("Hello World"));

    //*************************************************************************
    template <typename T1, typename T2>
    bool Equal(const T1& compare_text, const T2& test_text)
    {
      return (compare_text.size() == test_text.size()) && std::equal(test_text.begin(), test_text.end(), compare_text.begin());
    }

    //*************************************************************************
    template <typename T1, typename T2>
    bool Equal(const T1* compare_text, const T2& test_text)
    {
      return (etl::strlen(compare_text) == test_text.size()) && std::equal(test_text.begin(), test_text.end(), compare_text);
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
      CHECK_EQUAL(Length, text_capacity);

      static constexpr auto text_max_size = text_from_pointer.max_size();
      CHECK_EQUAL(Length, text_max_size);

      static constexpr auto text_size = text_from_pointer.size();
      CHECK_EQUAL(Length, text_size);

      static constexpr auto text_length = text_from_pointer.length();
      CHECK_EQUAL(Length, text_length);

      static constexpr auto text_available = text_from_pointer.available();
      CHECK_EQUAL(0, text_available);

      static constexpr auto text_is_truncated = text_from_pointer.is_truncated();
      CHECK_FALSE(text_is_truncated);

      static constexpr auto text_is_secure = text_from_pointer.is_secure();
      CHECK_FALSE(text_is_secure);

      static constexpr Text::const_pointer text_data = text_from_pointer.data();
      CHECK_EQUAL(&text_pointer[0], text_data);

      static constexpr Text::const_pointer text_data_end = text_from_pointer.data_end();
      CHECK_EQUAL(&text_pointer[Length], text_data_end);

      static constexpr auto text_begin = text_from_pointer.begin();
      CHECK_EQUAL(&text_pointer[0], text_begin);

      static constexpr auto text_cbegin = text_from_pointer.cbegin();
      CHECK_EQUAL(&text_pointer[0], text_cbegin);

      static constexpr auto text_rbegin = text_from_pointer.rbegin();
      CHECK_EQUAL(&text_pointer[Length - 1], etl::addressof(*text_rbegin));

      static constexpr auto text_crbegin = text_from_pointer.crbegin();
      CHECK_EQUAL(&text_pointer[Length - 1], etl::addressof(*text_crbegin));

      static constexpr auto text_end = text_from_pointer.end();
      CHECK_EQUAL(&text_pointer[Length], text_end);

      static constexpr auto text_cend = text_from_pointer.cend();
      CHECK_EQUAL(&text_pointer[Length], text_cend);

      static constexpr auto text_rend = text_from_pointer.rend();
      CHECK_EQUAL(&text_pointer[0] - 1, etl::addressof(*text_rend));

      static constexpr auto text_crend = text_from_pointer.crend();
      CHECK_EQUAL(&text_pointer[0] - 1, etl::addressof(*text_crend));

      bool is_equal = Equal(std::u32string(text_pointer), text);
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
      CHECK_EQUAL(Length, text_capacity);

      static constexpr auto text_max_size = text_from_literal.max_size();
      CHECK_EQUAL(Length, text_max_size);

      static constexpr auto text_size = text_from_literal.size();
      CHECK_EQUAL(Length, text_size);

      static constexpr auto text_length = text_from_literal.length();
      CHECK_EQUAL(Length, text_length);

      static constexpr auto text_available = text_from_literal.available();
      CHECK_EQUAL(0, text_available);

      static constexpr auto text_is_truncated = text_from_literal.is_truncated();
      CHECK_FALSE(text_is_truncated);

      static constexpr auto text_is_secure = text_from_literal.is_secure();
      CHECK_FALSE(text_is_secure);

      static constexpr Text::const_pointer text_data = text_from_literal.data();
      CHECK_EQUAL(&text_pointer[0], text_data);

      static constexpr Text::const_pointer text_data_end = text_from_literal.data_end();
      CHECK_EQUAL(&text_pointer[Length], text_data_end);

      static constexpr auto text_begin = text_from_literal.begin();
      CHECK_EQUAL(&text_pointer[0], text_begin);

      static constexpr auto text_cbegin = text_from_literal.cbegin();
      CHECK_EQUAL(&text_pointer[0], text_cbegin);

      static constexpr auto text_rbegin = text_from_literal.rbegin();
      CHECK_EQUAL(&text_pointer[Length - 1], etl::addressof(*text_rbegin));

      static constexpr auto text_crbegin = text_from_literal.crbegin();
      CHECK_EQUAL(&text_pointer[Length - 1], etl::addressof(*text_crbegin));

      static constexpr auto text_end = text_from_literal.end();
      CHECK_EQUAL(&text_pointer[Length], text_end);

      static constexpr auto text_cend = text_from_literal.cend();
      CHECK_EQUAL(&text_pointer[Length], text_cend);

      static constexpr auto text_rend = text_from_literal.rend();
      CHECK_EQUAL(&text_pointer[0] - 1, etl::addressof(*text_rend));

      static constexpr auto text_crend = text_from_literal.crend();
      CHECK_EQUAL(&text_pointer[0] - 1, etl::addressof(*text_crend));

      bool is_equal = Equal(std::u32string(text_pointer), text);
      CHECK(is_equal);
    }

    //*************************************************************************
    TEST(test_constructor_truncated)
    {
      static constexpr Text text_from_pointer(text_pointer, true);
      static constexpr Text text_from_literal(STR("Hello World"), true);

      static constexpr auto text_from_pointer_is_truncated = text_from_pointer.is_truncated();
      CHECK_TRUE(text_from_pointer_is_truncated);

      static constexpr auto text_from_literal_is_truncated = text_from_literal.is_truncated();
      CHECK_TRUE(text_from_literal_is_truncated);
    }

    //*************************************************************************
    TEST(test_copy_constructor)
    {
      static constexpr Text text_copy(text);

      static constexpr auto text_empty = text_copy.empty();
      CHECK_FALSE(text_empty);

      static constexpr auto text_full = text_copy.full();
      CHECK_TRUE(text_full);

      static constexpr auto text_capacity = text_copy.capacity();
      CHECK_EQUAL(Length, text_capacity);

      static constexpr auto text_max_size = text_copy.max_size();
      CHECK_EQUAL(Length, text_max_size);

      static constexpr auto text_size = text_copy.size();
      CHECK_EQUAL(Length, text_size);

      static constexpr auto text_length = text_copy.length();
      CHECK_EQUAL(Length, text_length);

      static constexpr auto text_available = text_copy.available();
      CHECK_EQUAL(0, text_available);

      static constexpr auto text_is_truncated = text_copy.is_truncated();
      CHECK_FALSE(text_is_truncated);

      static constexpr auto text_is_secure = text_copy.is_secure();
      CHECK_FALSE(text_is_secure);

      static constexpr Text::const_pointer text_data = text_copy.data();
      CHECK_EQUAL(&text_pointer[0], text_data);

      static constexpr Text::const_pointer text_data_end = text_copy.data_end();
      CHECK_EQUAL(&text_pointer[Length], text_data_end);

      static constexpr auto text_begin = text_copy.begin();
      CHECK_EQUAL(&text_pointer[0], text_begin);

      static constexpr auto text_cbegin = text_copy.cbegin();
      CHECK_EQUAL(&text_pointer[0], text_cbegin);

      static constexpr auto text_rbegin = text_copy.rbegin();
      CHECK_EQUAL(&text_pointer[Length - 1], etl::addressof(*text_rbegin));

      static constexpr auto text_crbegin = text_copy.crbegin();
      CHECK_EQUAL(&text_pointer[Length - 1], etl::addressof(*text_crbegin));

      static constexpr auto text_end = text_copy.end();
      CHECK_EQUAL(&text_pointer[Length], text_end);

      static constexpr auto text_cend = text_copy.cend();
      CHECK_EQUAL(&text_pointer[Length], text_cend);

      static constexpr auto text_rend = text_copy.rend();
      CHECK_EQUAL(&text_pointer[0] - 1, etl::addressof(*text_rend));
       
      static constexpr auto text_crend = text_copy.crend();
      CHECK_EQUAL(&text_pointer[0] - 1, etl::addressof(*text_crend));

      bool is_equal = Equal(std::u32string(text_pointer), text);
      CHECK(is_equal);
    }

    //*************************************************************************
    TEST(test_copy_constructor_from_truncated)
    {
      static constexpr Text text_original(STR("Hello World"), true);
      static constexpr Text text_copy(text_original);

      static constexpr auto text_empty = text_copy.empty();
      CHECK_FALSE(text_empty);

      static constexpr auto text_full = text_copy.full();
      CHECK_TRUE(text_full);

      static constexpr auto text_capacity = text_copy.capacity();
      CHECK_EQUAL(Length, text_capacity);

      static constexpr auto text_max_size = text_copy.max_size();
      CHECK_EQUAL(Length, text_max_size);

      static constexpr auto text_size = text_copy.size();
      CHECK_EQUAL(Length, text_size);

      static constexpr auto text_length = text_copy.length();
      CHECK_EQUAL(Length, text_length);

      static constexpr auto text_available = text_copy.available();
      CHECK_EQUAL(0, text_available);

      static constexpr auto text_is_truncated = text_copy.is_truncated();
      CHECK_TRUE(text_is_truncated);

      static constexpr auto text_is_secure = text_copy.is_secure();
      CHECK_FALSE(text_is_secure);

      static constexpr Text::const_pointer text_data = text_copy.data();
      CHECK_EQUAL(&text_pointer[0], text_data);

      static constexpr Text::const_pointer text_data_end = text_copy.data_end();
      CHECK_EQUAL(&text_pointer[Length], text_data_end);

      static constexpr auto text_begin = text_copy.begin();
      CHECK_EQUAL(&text_pointer[0], text_begin);

      static constexpr auto text_cbegin = text_copy.cbegin();
      CHECK_EQUAL(&text_pointer[0], text_cbegin);

      static constexpr auto text_rbegin = text_copy.rbegin();
      CHECK_EQUAL(&text_pointer[Length - 1], etl::addressof(*text_rbegin));

      static constexpr auto text_crbegin = text_copy.crbegin();
      CHECK_EQUAL(&text_pointer[Length - 1], etl::addressof(*text_crbegin));

      static constexpr auto text_end = text_copy.end();
      CHECK_EQUAL(&text_pointer[Length], text_end);

      static constexpr auto text_cend = text_copy.cend();
      CHECK_EQUAL(&text_pointer[Length], text_cend);

      static constexpr auto text_rend = text_copy.rend();
      CHECK_EQUAL(&text_pointer[0] - 1, etl::addressof(*text_rend));

      static constexpr auto text_crend = text_copy.crend();
      CHECK_EQUAL(&text_pointer[0] - 1, etl::addressof(*text_crend));

      bool is_equal = Equal(std::u32string(text_pointer), text);
      CHECK(is_equal);
    }

    //*************************************************************************
    TEST(test_begin)
    {
      static constexpr Text::const_iterator p_begin  = text.begin();
      static constexpr Text::const_iterator p_cbegin = text.cbegin();

      CHECK_EQUAL(&text_pointer[0], p_begin);
      CHECK_EQUAL(&text_pointer[0], p_cbegin);
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
      CHECK_TRUE(text_pointer[Length - 1] == text.back());
    }

    //*************************************************************************
    TEST(test_data)
    {
      static constexpr Text::const_pointer p_data = text.data();

      CHECK_EQUAL(&text[0], p_data);
    }

    //*************************************************************************
    TEST(test_data_end)
    {
      static constexpr Text::const_pointer p_data = text.data_end();

      CHECK_EQUAL(&text[Length], p_data);
    }

    //*************************************************************************
    TEST(test_c_str)
    {
      static constexpr Text::const_pointer p_data = text.c_str();

      CHECK_EQUAL(&text[0], p_data);
      CHECK_EQUAL(Length, etl::strlen(p_data));
    }

    //*************************************************************************
    TEST(test_const_iterator)
    {
      static constexpr Text::const_iterator itr = text.begin();

      Text::const_iterator begin = text.begin();
      Text::const_iterator end   = text.end();

      bool is_equal = std::equal(begin, end, itr);
      CHECK_TRUE(is_equal);
    }

    //*************************************************************************
    TEST(test_const_reverse_iterator)
    {
      static constexpr Text::const_reverse_iterator ritr = text.rbegin();

      etl::reverse_iterator<Text::const_pointer> rbegin(text_pointer + Length);
      etl::reverse_iterator<Text::const_pointer> rend(text_pointer);

      bool is_equal = std::equal(rbegin, rend, ritr);
      CHECK_TRUE(is_equal);
    }

    //*************************************************************************
    TEST(test_to_view)
    {
      static constexpr View view(text);

      CHECK_TRUE(text.begin() == view.begin());
      CHECK_TRUE(text.end()   == view.end());

      bool is_equal = std::equal(text.begin(), text.end(), view.begin());
      CHECK_TRUE(is_equal);
    }
  };
}

#endif
