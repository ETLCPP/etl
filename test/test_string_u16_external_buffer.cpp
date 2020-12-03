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

#include <string>
#include <array>
#include <algorithm>

#include "etl/u16string.h"
#include "etl/fnv_1.h"

#undef STR
#define STR(x) u##x

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
    static constexpr size_t SIZE   = 11;
    static constexpr size_t SIZE_L = 52;
    static constexpr size_t SIZE_S = 4;

    using Text         = etl::u16string_ext;
    using IText        = etl::iu16string;
    using TextT        = etl::u16string<SIZE>;
    using Compare_Text = std::u16string;
    using value_t      = Text::value_type;

    using TextBuffer  = std::array<IText::value_type, SIZE + 1>;
    using TextBufferL = std::array<IText::value_type, SIZE_L + 1>;
    using TextBufferS = std::array<IText::value_type, SIZE_S + 1>;

    Compare_Text initial_text;
    Compare_Text less_text;
    Compare_Text greater_text;
    Compare_Text shorter_text;
    Compare_Text different_text;
    Compare_Text insert_text;
    Compare_Text longer_text;
    Compare_Text short_text;

    const value_t* pinitial_text = STR("Hello World");

    value_t  array_text[12];
    value_t* p_text = array_text;

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

        std::copy(pinitial_text, pinitial_text + etl::strlen(pinitial_text), array_text);
      }
    };

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_default_constructor)
    {
      TextBuffer buffer;
      Text text(buffer.data(), buffer.size());

      CHECK_EQUAL(text.size(), size_t(0));
      CHECK(text.empty());
      CHECK_EQUAL(SIZE, text.capacity());
      CHECK_EQUAL(SIZE, text.max_size());
      CHECK(text.begin() == text.end());
#if ETL_STRING_TRUNCATION_CHECKS_ENABLED
      CHECK(!text.is_truncated());
#endif
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_default_constructor_use_buffer_and_size)
    {
      size_t length = etl::strlen(p_text);
      Text text(p_text, length + 1);

      CHECK_EQUAL(0U, text.size());
      CHECK(text.empty());
      CHECK_EQUAL(length, text.capacity());
      CHECK_EQUAL(length, text.max_size());
      CHECK(text.begin() == text.end());
#if ETL_STRING_TRUNCATION_CHECKS_ENABLED
      CHECK(!text.is_truncated());
#endif
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_default_constructor_use_buffer_text_and_size)
    {
      Text text(p_text, p_text, etl::strlen(p_text) + 1);

      CHECK_EQUAL(text.size(), etl::strlen(p_text));
      CHECK(!text.empty());
      CHECK_EQUAL(etl::strlen(p_text), text.capacity());
      CHECK_EQUAL(etl::strlen(p_text), text.max_size());
      CHECK(text.begin() != text.end());
#if ETL_STRING_TRUNCATION_CHECKS_ENABLED
      CHECK(!text.is_truncated());
#endif
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_default_constructor_use_array_buffer)
    {
      Text text(array_text, etl::size(array_text));

      CHECK_EQUAL(0U, text.size());
      CHECK(text.empty());
      CHECK_EQUAL(etl::size(array_text) - 1, text.capacity());
      CHECK_EQUAL(etl::size(array_text) - 1, text.max_size());
      CHECK(text.begin() == text.end());
#if ETL_STRING_TRUNCATION_CHECKS_ENABLED
      CHECK(!text.is_truncated());
#endif
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_default_constructor_use_array_buffer_text)
    {
      Text text(array_text, array_text, etl::size(array_text));

      CHECK_EQUAL(text.size(), etl::strlen(array_text));
      CHECK(!text.empty());
      CHECK_EQUAL(etl::size(array_text) - 1, text.capacity());
      CHECK_EQUAL(etl::size(array_text) - 1, text.max_size());
      CHECK(text.begin() != text.end());
#if ETL_STRING_TRUNCATION_CHECKS_ENABLED
      CHECK(!text.is_truncated());
#endif
    }

    //*************************************************************************
    TEST(test_iterator_comparison_empty)
    {
      TextBuffer buffer;
      Text text(buffer.data(), buffer.size());

      CHECK(text.begin() == text.end());
      CHECK(text.cbegin() == text.cend());
      CHECK(text.rbegin() == text.rend());
      CHECK(text.crbegin() == text.crend());
#if ETL_STRING_TRUNCATION_CHECKS_ENABLED
      CHECK(!text.is_truncated());
#endif
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_constructor_size_value)
    {
      const size_t  INITIAL_SIZE = 5;
      const value_t INITIAL_VALUE = STR('A');
      TextBuffer    buffer;

      Compare_Text compare_text(INITIAL_SIZE, INITIAL_VALUE);
      Text text(INITIAL_SIZE, INITIAL_VALUE, buffer.data(), buffer.size());

      CHECK(text.size() == INITIAL_SIZE);
      CHECK(!text.empty());

      bool is_equal = Equal(compare_text, text);

      CHECK(is_equal);
#if ETL_STRING_TRUNCATION_CHECKS_ENABLED
      CHECK(!text.is_truncated());
#endif
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_constructor_size_excess)
    {
      TextBuffer buffer;
      Text text(buffer.size() + 1, STR('A'), buffer.data(), buffer.size());

      CHECK_EQUAL(buffer.size() - 1, text.size());
#if ETL_STRING_TRUNCATION_CHECKS_ENABLED
      CHECK(text.is_truncated());
#endif
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_constructor_char_pointer)
    {
      TextBuffer buffer;
      Compare_Text compare_text(initial_text.c_str());

      Text text(initial_text.c_str(), buffer.data(), buffer.size());

      CHECK(!text.empty());

      bool is_equal = Equal(compare_text, text);
      CHECK(is_equal);
#if ETL_STRING_TRUNCATION_CHECKS_ENABLED
      CHECK(!text.is_truncated());
#endif
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_constructor_char_pointer_excess)
    {
      Compare_Text compare_text(initial_text.c_str());

      TextBuffer buffer;
      Text text(longer_text.c_str(), buffer.data(), buffer.size());

      CHECK(!text.empty());

      bool is_equal = Equal(compare_text, text);
      CHECK(is_equal);
#if ETL_STRING_TRUNCATION_CHECKS_ENABLED
      CHECK(text.is_truncated());
#endif
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_constructor_char_pointer_size)
    {
      Compare_Text compare_text(SIZE, STR('A'));

      TextBuffer buffer;
      Text text(SIZE, STR('A'), buffer.data(), buffer.size());

      CHECK(!text.empty());

      bool is_equal = Equal(compare_text, text);
      CHECK(is_equal);
#if ETL_STRING_TRUNCATION_CHECKS_ENABLED
      CHECK(!text.is_truncated());
#endif
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_constructor_char_pointer_size_excess)
    {
      Compare_Text compare_text(SIZE, STR('A'));

      TextBuffer buffer;
      Text text(SIZE + 1, STR('A'), buffer.data(), buffer.size());

      CHECK(!text.empty());

      bool is_equal = Equal(compare_text, text);
      CHECK(is_equal);
#if ETL_STRING_TRUNCATION_CHECKS_ENABLED
      CHECK(text.is_truncated());
#endif
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_constructor_size_char)
    {
      Compare_Text compare_text(initial_text.c_str(), initial_text.size() / 2);

      TextBuffer buffer;
      Text text(initial_text.c_str(), initial_text.size() / 2, buffer.data(), buffer.size());

      CHECK(!text.empty());

      bool is_equal = Equal(compare_text, text);
      CHECK(is_equal);
#if ETL_STRING_TRUNCATION_CHECKS_ENABLED
      CHECK(!text.is_truncated());
#endif
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_constructor_size_char_excess)
    {
      Compare_Text compare_text(initial_text.c_str(), initial_text.size());

      TextBuffer buffer;
      Text text(longer_text.c_str(), longer_text.size(), buffer.data(), buffer.size());

      CHECK(!text.empty());

      bool is_equal = Equal(compare_text, text);
      CHECK(is_equal);
#if ETL_STRING_TRUNCATION_CHECKS_ENABLED
      CHECK(text.is_truncated());
#endif
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_constructor_range)
    {
      Compare_Text compare_text(initial_text.begin(), initial_text.end());

      TextBuffer buffer;
      Text text(compare_text.begin(), compare_text.end(), buffer.data(), buffer.size());

      CHECK(text.size() == SIZE);
      CHECK(!text.empty());
#if ETL_STRING_TRUNCATION_CHECKS_ENABLED
      CHECK(!text.is_truncated());
#endif
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_constructor_range_excess)
    {
      TextBuffer buffer;
      Text text(longer_text.begin(), longer_text.end(), buffer.data(), buffer.size());

      bool is_equal = Equal(initial_text, text);
      CHECK(is_equal);
      CHECK(text.size() == SIZE);
      CHECK(!text.empty());
#if ETL_STRING_TRUNCATION_CHECKS_ENABLED
      CHECK(text.is_truncated());
#endif
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_constructor_from_literal)
    {
      TextBuffer buffer;
      Text text(STR("Hello World"), buffer.data(), buffer.size());

      bool is_equal = Equal(initial_text, text);
      CHECK(is_equal);
      CHECK(text.size() == SIZE);
      CHECK(!text.empty());
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_constructor_from_string_view)
    {
      etl::u16string_view view(initial_text.data(), initial_text.size());

      TextBuffer buffer;
      Text text(view, buffer.data(), buffer.size());

      bool is_equal = Equal(initial_text, text);
      CHECK(is_equal);
      CHECK(text.size() == SIZE);
      CHECK(!text.empty());
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_copy_constructor)
    {
      TextBuffer buffer;
      Text text(initial_text.c_str(), buffer.data(), buffer.size());

      TextBuffer buffer2;
      Text text2(text, buffer2.data(), buffer2.size());
      CHECK(text2 == text);
#if ETL_STRING_TRUNCATION_CHECKS_ENABLED
      CHECK(!text2.is_truncated());
#endif
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_copy_constructor_i)
    {
      TextBuffer buffer;
      Text text(initial_text.c_str(), buffer.data(), buffer.size());
      IText& itext = text;

      TextBuffer buffer2;
      Text text2(itext, buffer2.data(), buffer2.size());
      CHECK(text2 == text);
#if ETL_STRING_TRUNCATION_CHECKS_ENABLED
      CHECK(!text2.is_truncated());
#endif
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_copy_constructor_excess)
    {
      TextBuffer buffer;
      Text  text(initial_text.c_str(), buffer.data(), buffer.size());

      TextBufferL bufferl;
      Text textl(longer_text.c_str(), bufferl.data(), bufferl.size());

      TextBuffer buffer2;
      Text  text2(textl, buffer2.data(), buffer2.size());
      CHECK(text2 == text);
#if ETL_STRING_TRUNCATION_CHECKS_ENABLED
      CHECK(text2.is_truncated());
#endif
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_copy_constructor_from_truncated)
    {
      TextBuffer buffer;
      Text  text(longer_text.c_str(), buffer.data(), buffer.size());

      TextBuffer buffer2;
      Text  text2(text, buffer2.data(), buffer2.size());

#if ETL_STRING_TRUNCATION_CHECKS_ENABLED
      CHECK(text2.is_truncated());
#endif
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_construct_position_length)
    {
      Compare_Text compare_text(initial_text.c_str());
      Compare_Text compare_text2(compare_text, 2, 4);

      TextBuffer buffer;
      Text text(initial_text.c_str(), buffer.data(), buffer.size());

      TextBuffer buffer2;
      Text text2(text, buffer2.data(), buffer2.size(), 2, 4);

      bool is_equal = Equal(compare_text2, text2);
      CHECK(is_equal);
#if ETL_STRING_TRUNCATION_CHECKS_ENABLED
      CHECK(!text2.is_truncated());
#endif
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_construct_position_length_excess)
    {
      Compare_Text compare_text(longer_text.c_str());
      Compare_Text compare_text2(compare_text, 2, 11);

      TextBufferL bufferl;
      Text textl(longer_text.c_str(), bufferl.data(), bufferl.size());

      TextBuffer buffer;
      Text text2(textl, buffer.data(), buffer.size(), 2, 12);

      bool is_equal = Equal(compare_text2, text2);
      CHECK(is_equal);
#if ETL_STRING_TRUNCATION_CHECKS_ENABLED
      CHECK(text2.is_truncated());
#endif
    }

#if ETL_USING_STL
    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_construct_initializer_list)
    {
      Compare_Text compare_text = { STR('H'), STR('e'), STR('l') , STR('l') , STR('o') };

      std::initializer_list<Text::value_type> il = { STR('H'), STR('e'), STR('l') , STR('l') , STR('o') };
      TextBuffer buffer;
      Text text(il, buffer.data(), buffer.size());

      bool is_equal = Equal(compare_text, text);
      CHECK(is_equal);
#if ETL_STRING_TRUNCATION_CHECKS_ENABLED
      CHECK(!text.is_truncated());
#endif
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_construct_initializer_list_excess)
    {
      Compare_Text compare_text = { STR('H'), STR('e'), STR('l'), STR('l'), STR('o'),  STR(' '),
                                    STR('W'), STR('o'), STR('r'), STR('l'), STR('d') };

      std::initializer_list<Text::value_type> il = { STR('H'), STR('e'), STR('l'), STR('l'), STR('o'),  STR(' '),
                                                     STR('W'), STR('o'), STR('r'), STR('l'), STR('d'),  STR(' '),
                                                     STR('T'), STR('h'), STR('e'), STR('r'), STR('e') };
      TextBuffer buffer;
      Text text(il, buffer.data(), buffer.size());

      bool is_equal = Equal(compare_text, text);
      CHECK(is_equal);
#if ETL_STRING_TRUNCATION_CHECKS_ENABLED
      CHECK(text.is_truncated());
#endif
    }
#endif

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_assignment)
    {
      TextBuffer buffer;
      Text text(initial_text.begin(), initial_text.end(), buffer.data(), buffer.size());

      TextBuffer buffer2;
      Text other_text(buffer2.data(), buffer2.size());

      other_text = text;

      bool is_equal = Equal(text, other_text);
      CHECK(is_equal);
#if ETL_STRING_TRUNCATION_CHECKS_ENABLED
      CHECK(!text.is_truncated());
      CHECK(!other_text.is_truncated());
#endif
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_assignment_excess)
    {
      TextBuffer buffer;
      Text text(longer_text.begin(), longer_text.end(), buffer.data(), buffer.size());

      TextBuffer buffer2;
      Text other_text(buffer2.data(), buffer2.size());

      other_text = text;

      bool is_equal = Equal(text, other_text);
      CHECK(is_equal);
#if ETL_STRING_TRUNCATION_CHECKS_ENABLED
      CHECK(text.is_truncated());
      CHECK(other_text.is_truncated());
#endif
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_assignment_iterface)
    {
      TextBuffer buffer;
      Text text1(initial_text.begin(), initial_text.end(), buffer.data(), buffer.size());

      TextBuffer buffer2;
      Text text2(buffer2.data(), buffer2.size());

      IText& itext1 = text1;
      IText& itext2 = text2;

      itext2 = itext1;

      bool is_equal = Equal(text1, text2);

      CHECK(is_equal);
#if ETL_STRING_TRUNCATION_CHECKS_ENABLED
      CHECK(!text1.is_truncated());
      CHECK(!text2.is_truncated());
#endif
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_assignment_iterface_excess)
    {
      TextBuffer buffer;
      Text text1(longer_text.begin(), longer_text.end(), buffer.data(), buffer.size());

      TextBuffer buffer2;
      Text text2(buffer2.data(), buffer2.size());

      IText& itext1 = text1;
      IText& itext2 = text2;

      itext2 = itext1;

      bool is_equal = Equal(text1, text2);

      CHECK(is_equal);
#if ETL_STRING_TRUNCATION_CHECKS_ENABLED
      CHECK(text1.is_truncated());
      CHECK(text2.is_truncated());
#endif
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_self_assignment)
    {
      TextBuffer buffer;
      Text text(initial_text.begin(), initial_text.end(), buffer.data(), buffer.size());

      TextBuffer buffer2;
      Text other_text(text, buffer2.data(), buffer2.size());

      other_text = other_text;

      bool is_equal = Equal(text, other_text);

      CHECK(is_equal);
#if ETL_STRING_TRUNCATION_CHECKS_ENABLED
      CHECK(!text.is_truncated());
      CHECK(!other_text.is_truncated());
#endif
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_self_assignment_excess)
    {
      TextBuffer buffer;
      Text text(longer_text.begin(), longer_text.end(), buffer.data(), buffer.size());

      TextBuffer buffer2;
      Text other_text(text, buffer2.data(), buffer2.size());

      other_text = other_text;

      bool is_equal = Equal(text, other_text);

      CHECK(is_equal);
#if ETL_STRING_TRUNCATION_CHECKS_ENABLED
      CHECK(text.is_truncated());
      CHECK(other_text.is_truncated());
#endif
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_assignment_from_literal)
    {
      TextBuffer buffer;
      Text text(buffer.data(), buffer.size());

      text = STR("Hello World");

      bool is_equal = Equal(std::u16string(STR("Hello World")), text);
      CHECK(is_equal);
#if ETL_STRING_TRUNCATION_CHECKS_ENABLED
      CHECK(!text.is_truncated());
#endif
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_assignment_from_literal_excess)
    {
      TextBuffer buffer;
      Text text(buffer.data(), buffer.size());

      text = STR("Hello World There");

      bool is_equal = Equal(std::u16string(STR("Hello World")), text);
      CHECK(is_equal);
#if ETL_STRING_TRUNCATION_CHECKS_ENABLED
      CHECK(text.is_truncated());
#endif
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_assignment_from_literal_via_interface)
    {
      TextBuffer buffer;
      Text text(buffer.data(), buffer.size());
      IText& itext = text;

      itext = STR("Hello World");

      bool is_equal = Equal(std::u16string(STR("Hello World")), itext);
      CHECK(is_equal);
#if ETL_STRING_TRUNCATION_CHECKS_ENABLED
      CHECK(!itext.is_truncated());
#endif
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_assignment_from_literal_via_interface_excess)
    {
      TextBuffer buffer;
      Text text(buffer.data(), buffer.size());
      IText& itext = text;

      itext = STR("Hello World There");

      bool is_equal = Equal(std::u16string(STR("Hello World")), itext);
      CHECK(is_equal);
#if ETL_STRING_TRUNCATION_CHECKS_ENABLED
      CHECK(itext.is_truncated());
#endif
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_begin)
    {
      TextBuffer buffer;
      Text text(initial_text.c_str(), buffer.data(), buffer.size());

      TextBuffer buffer2;
      const Text constText(initial_text.c_str(), buffer2.data(), buffer2.size());

      CHECK_EQUAL(&text[0], text.begin());
      CHECK_EQUAL(&constText[0], constText.begin());
    }


    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_end)
    {
      TextBuffer buffer;
      Text text(initial_text.c_str(), buffer.data(), buffer.size());

      TextBuffer buffer2;
      const Text constText(initial_text.c_str(), buffer2.data(), buffer2.size());

      CHECK_EQUAL(&text[initial_text.size()], text.end());
      CHECK_EQUAL(&constText[initial_text.size()], constText.end());
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_resize_up)
    {
      const size_t INITIAL_SIZE = 5;
      const size_t NEW_SIZE = 8;

      TextBuffer buffer;
      Text text(initial_text.c_str(), INITIAL_SIZE, buffer.data(), buffer.size());
      text.resize(NEW_SIZE);

      CHECK_EQUAL(text.size(), NEW_SIZE);
#if ETL_STRING_TRUNCATION_CHECKS_ENABLED
      CHECK(!text.is_truncated());
#endif
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_resize_up_value)
    {
      const size_t INITIAL_SIZE = 5;
      const size_t NEW_SIZE = 8;
      const value_t INITIAL_VALUE = STR('A');

      TextBuffer buffer;
      Text text(INITIAL_SIZE, INITIAL_VALUE, buffer.data(), buffer.size());
      text.resize(NEW_SIZE, INITIAL_VALUE);

      std::array<char, NEW_SIZE> compare_text;
      compare_text.fill(INITIAL_VALUE);

      bool is_equal = Equal(compare_text, text);
      CHECK(is_equal);
#if ETL_STRING_TRUNCATION_CHECKS_ENABLED
      CHECK(!text.is_truncated());
#endif
    }


    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_resize_excess)
    {
      const size_t INITIAL_SIZE = 5;
      const size_t NEW_SIZE = SIZE + 1;

      TextBuffer buffer;
      Text text(INITIAL_SIZE, STR('A'), buffer.data(), buffer.size());
      text.resize(NEW_SIZE, STR('A'));
      CHECK_EQUAL(SIZE, text.size());
#if ETL_STRING_TRUNCATION_CHECKS_ENABLED
      CHECK(text.is_truncated());
#endif
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_resize_down)
    {
      const size_t INITIAL_SIZE = 5;
      const size_t NEW_SIZE = 2;

      TextBuffer buffer;
      Text text(INITIAL_SIZE, STR('A'), buffer.data(), buffer.size());
      text.resize(NEW_SIZE);

      CHECK_EQUAL(text.size(), NEW_SIZE);
#if ETL_STRING_TRUNCATION_CHECKS_ENABLED
      CHECK(!text.is_truncated());
#endif
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_resize_down_value)
    {
      const size_t INITIAL_SIZE = 5;
      const size_t NEW_SIZE = 2;
      const value_t INITIAL_VALUE = STR('A');

      TextBuffer buffer;
      Text text(INITIAL_SIZE, INITIAL_VALUE, buffer.data(), buffer.size());
      text.resize(NEW_SIZE, INITIAL_VALUE);

      CHECK_EQUAL(text.size(), NEW_SIZE);

      std::array<char, NEW_SIZE> compare_text;
      compare_text.fill(INITIAL_VALUE);

      bool is_equal = Equal(compare_text, text);
      CHECK(is_equal);
#if ETL_STRING_TRUNCATION_CHECKS_ENABLED
      CHECK(!text.is_truncated());
#endif
    }


    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_empty_full)
    {
      TextBuffer buffer;
      Text text(buffer.data(), buffer.size());
      text.resize(text.max_size(), STR('A'));

      CHECK(!text.empty());
#if ETL_STRING_TRUNCATION_CHECKS_ENABLED
      CHECK(!text.is_truncated());
#endif
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_empty_half)
    {
      TextBuffer buffer;
      Text text(buffer.data(), buffer.size());
      text.resize(text.max_size() / 2, STR('A'));

      CHECK(!text.empty());
#if ETL_STRING_TRUNCATION_CHECKS_ENABLED
      CHECK(!text.is_truncated());
#endif
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_empty_empty)
    {
      TextBuffer buffer;
      Text text(buffer.data(), buffer.size());

      CHECK(text.empty());
#if ETL_STRING_TRUNCATION_CHECKS_ENABLED
      CHECK(!text.is_truncated());
#endif
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_full_full)
    {
      TextBuffer buffer;
      Text text(buffer.data(), buffer.size());
      text.resize(text.max_size(), STR('A'));

      CHECK(text.full());
#if ETL_STRING_TRUNCATION_CHECKS_ENABLED
      CHECK(!text.is_truncated());
#endif
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_full_half)
    {
      TextBuffer buffer;
      Text text(buffer.data(), buffer.size());
      text.resize(text.max_size() / 2, STR('A'));

      CHECK(!text.full());
#if ETL_STRING_TRUNCATION_CHECKS_ENABLED
      CHECK(!text.is_truncated());
#endif
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_full_empty)
    {
      TextBuffer buffer;
      Text text(buffer.data(), buffer.size());

      CHECK(!text.full());
#if ETL_STRING_TRUNCATION_CHECKS_ENABLED
      CHECK(!text.is_truncated());
#endif
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_index)
    {
      Compare_Text compare_text(initial_text.c_str());

      TextBuffer buffer;
      Text text(initial_text.c_str(), buffer.data(), buffer.size());

      for (size_t i = 0; i < text.size(); ++i)
      {
        CHECK_EQUAL(text[i], compare_text[i]);
      }

#if ETL_STRING_TRUNCATION_CHECKS_ENABLED
      CHECK(!text.is_truncated());
#endif
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_index_const)
    {
      const Compare_Text compare_text(initial_text.c_str());

      TextBuffer buffer;
      const Text text(initial_text.c_str(), buffer.data(), buffer.size());

      for (size_t i = 0; i < text.size(); ++i)
      {
        CHECK_EQUAL(text[i], compare_text[i]);
      }

#if ETL_STRING_TRUNCATION_CHECKS_ENABLED
      CHECK(!text.is_truncated());
#endif
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_at)
    {
      Compare_Text compare_text(initial_text.c_str());

      TextBuffer buffer;
      Text text(initial_text.c_str(), buffer.data(), buffer.size());

      for (size_t i = 0; i < text.size(); ++i)
      {
        CHECK_EQUAL(text.at(i), compare_text.at(i));
      }

#if ETL_STRING_TRUNCATION_CHECKS_ENABLED
      CHECK(!text.is_truncated());
#endif

      CHECK_THROW(text.at(text.size()), etl::string_out_of_bounds);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_at_const)
    {
      const Compare_Text compare_text(initial_text.c_str());

      TextBuffer buffer;
      const Text text(initial_text.c_str(), buffer.data(), buffer.size());

      for (size_t i = 0; i < text.size(); ++i)
      {
        CHECK_EQUAL(text.at(i), compare_text.at(i));
      }

#if ETL_STRING_TRUNCATION_CHECKS_ENABLED
      CHECK(!text.is_truncated());
#endif

      CHECK_THROW(text.at(text.size()), etl::string_out_of_bounds);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_front)
    {
      Compare_Text compare_text(initial_text.c_str());

      TextBuffer buffer;
      Text text(initial_text.c_str(), buffer.data(), buffer.size());

      CHECK(text.front() == compare_text.front());
#if ETL_STRING_TRUNCATION_CHECKS_ENABLED
      CHECK(!text.is_truncated());
#endif
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_front_const)
    {
      const Compare_Text compare_text(initial_text.c_str());

      TextBuffer buffer;
      const Text text(initial_text.c_str(), buffer.data(), buffer.size());

      CHECK(text.front() == compare_text.front());
#if ETL_STRING_TRUNCATION_CHECKS_ENABLED
      CHECK(!text.is_truncated());
#endif
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_back)
    {
      Compare_Text compare_text(initial_text.c_str());

      TextBuffer buffer;
      Text text(initial_text.c_str(), buffer.data(), buffer.size());

      CHECK(text.back() == compare_text.back());
#if ETL_STRING_TRUNCATION_CHECKS_ENABLED
      CHECK(!text.is_truncated());
#endif
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_back_const)
    {
      const Compare_Text compare_text(initial_text.c_str());

      TextBuffer buffer;
      const Text text(initial_text.c_str(), buffer.data(), buffer.size());

      CHECK(text.back() == compare_text.back());
#if ETL_STRING_TRUNCATION_CHECKS_ENABLED
      CHECK(!text.is_truncated());
#endif
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_data)
    {
      Compare_Text compare_text(initial_text.c_str());

      TextBuffer buffer;
      Text text(compare_text.begin(), compare_text.end(), buffer.data(), buffer.size());

      bool is_equal = std::equal(text.data(),
        text.data() + text.size(),
        compare_text.begin());

      CHECK(is_equal);
#if ETL_STRING_TRUNCATION_CHECKS_ENABLED
      CHECK(!text.is_truncated());
#endif
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_data_const)
    {
      Compare_Text compare_text(initial_text.c_str());

      TextBuffer buffer;
      const Text text(compare_text.begin(), compare_text.end(), buffer.data(), buffer.size());

      bool is_equal = std::equal(text.data(),
        text.data() + text.size(),
        compare_text.begin());

      CHECK(is_equal);
#if ETL_STRING_TRUNCATION_CHECKS_ENABLED
      CHECK(!text.is_truncated());
#endif
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_assign_string)
    {
      Compare_Text compare_input(initial_text.c_str());

      TextBuffer buffer;
      Text input(initial_text.c_str(), buffer.data(), buffer.size());

      Compare_Text compare_text;

      TextBuffer buffer2;
      Text text(buffer2.data(), buffer2.size());

      compare_text.assign(compare_input);
      text.assign(input);

      bool is_equal = Equal(compare_text, text);
      CHECK(is_equal);
#if ETL_STRING_TRUNCATION_CHECKS_ENABLED
      CHECK(!text.is_truncated());
#endif
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_assign_string_excess)
    {
      Compare_Text compare_input(initial_text.c_str());

      TextBufferL bufferl;
      Text input(longer_text.c_str(), bufferl.data(), bufferl.size());

      Compare_Text compare_text;

      TextBuffer buffer;
      Text text(buffer.data(), buffer.size());

      compare_text.assign(compare_input);
      text.assign(input);

      bool is_equal = Equal(compare_text, text);
      CHECK(is_equal);
#if ETL_STRING_TRUNCATION_CHECKS_ENABLED
      CHECK(text.is_truncated());
#endif
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_assign_pointer)
    {
      Compare_Text compare_text(initial_text.c_str());

      TextBuffer buffer;
      Text text(buffer.data(), buffer.size());
      text.assign(initial_text.c_str());

      bool is_equal = Equal(compare_text, text);
      CHECK(is_equal);
#if ETL_STRING_TRUNCATION_CHECKS_ENABLED
      CHECK(!text.is_truncated());
#endif
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_assign_pointer_excess)
    {
      Compare_Text compare_text(initial_text.c_str());

      TextBuffer buffer;
      Text text(buffer.data(), buffer.size());
      text.assign(longer_text.c_str());

      bool is_equal = Equal(compare_text, text);
      CHECK(is_equal);
#if ETL_STRING_TRUNCATION_CHECKS_ENABLED
      CHECK(text.is_truncated());
#endif
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_assign_pointer_length)
    {
      Compare_Text compare_text(initial_text.c_str());

      TextBuffer buffer;
      Text text(buffer.data(), buffer.size());
      text.assign(initial_text.c_str(), initial_text.size());

      bool is_equal = Equal(compare_text, text);
      CHECK(is_equal);
#if ETL_STRING_TRUNCATION_CHECKS_ENABLED
      CHECK(!text.is_truncated());
#endif
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_assign_pointer_length_excess)
    {
      Compare_Text compare_text(longer_text.c_str());

      TextBuffer buffer;
      Text text(buffer.data(), buffer.size());
      text.assign(longer_text.c_str(), longer_text.size());

      compare_text.resize(text.max_size());

      bool is_equal = Equal(compare_text, text);
      CHECK(is_equal);
#if ETL_STRING_TRUNCATION_CHECKS_ENABLED
      CHECK(text.is_truncated());
#endif
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_assign_range)
    {
      Compare_Text compare_text(initial_text.c_str());

      TextBuffer buffer;
      Text text(buffer.data(), buffer.size());

      text.assign(compare_text.begin(), compare_text.end());

      bool is_equal = Equal(compare_text, text);
      CHECK(is_equal);
#if ETL_STRING_TRUNCATION_CHECKS_ENABLED
      CHECK(!text.is_truncated());
#endif
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_assign_range_excess)
    {
      TextBuffer buffer;
      Text text(buffer.data(), buffer.size());

      text.assign(longer_text.begin(), longer_text.end());

      CHECK_EQUAL(initial_text.size(), text.size());

      bool is_equal = Equal(initial_text, text);
      CHECK(is_equal);
#if ETL_STRING_TRUNCATION_CHECKS_ENABLED
      CHECK(text.is_truncated());
#endif
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_assign_size_value)
    {
      const size_t INITIAL_SIZE = 5;
      const value_t INITIAL_VALUE = STR('A');

      std::array<value_t, INITIAL_SIZE> compare_text;
      compare_text.fill(INITIAL_VALUE);

      TextBuffer buffer;
      Text text(buffer.data(), buffer.size());
      text.assign(INITIAL_SIZE, INITIAL_VALUE);

      CHECK(text.size() == INITIAL_SIZE);

      bool is_equal = Equal(compare_text, text);
      CHECK(is_equal);
#if ETL_STRING_TRUNCATION_CHECKS_ENABLED
      CHECK(!text.is_truncated());
#endif
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_assign_size_value_excess)
    {
      const size_t INITIAL_SIZE = SIZE;
      const size_t EXCESS_SIZE = SIZE + 1;
      const value_t INITIAL_VALUE = STR('A');
      std::array<value_t, INITIAL_SIZE> compare_text;
      compare_text.fill(INITIAL_VALUE);

      TextBuffer buffer;
      Text text(buffer.data(), buffer.size());
      text.assign(EXCESS_SIZE, INITIAL_VALUE);

      bool is_equal = Equal(compare_text, text);
      CHECK(is_equal);
#if ETL_STRING_TRUNCATION_CHECKS_ENABLED
      CHECK(text.is_truncated());
#endif
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_push_back)
    {
      Compare_Text compare_text;

      TextBuffer buffer;
      Text text(buffer.data(), buffer.size());

      for (size_t i = 0; i < SIZE; ++i)
      {
        compare_text.push_back(STR('A') + value_t(i));
      }

      for (size_t i = 0; i < SIZE; ++i)
      {
        text.push_back(STR('A') + value_t(i));
      }

      CHECK_EQUAL(etl::strlen(compare_text.data()), etl::strlen(text.data()));
      CHECK_EQUAL(compare_text.size(), text.size());

      bool is_equal = Equal(compare_text, text);
      CHECK(is_equal);
#if ETL_STRING_TRUNCATION_CHECKS_ENABLED
      CHECK(!text.is_truncated());
#endif
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_push_back_excess)
    {
      Compare_Text compare_text;

      TextBuffer buffer;
      Text text(buffer.data(), buffer.size());

      for (size_t i = 0; i < SIZE; ++i)
      {
        compare_text.push_back(STR('A') + value_t(i));
      }

      for (size_t i = 0; i < SIZE; ++i)
      {
        text.push_back(STR('A') + value_t(i));
#if ETL_STRING_TRUNCATION_CHECKS_ENABLED
        CHECK(!text.is_truncated());
#endif
      }

      text.push_back(STR('A') + value_t(SIZE));

      CHECK_EQUAL(etl::strlen(compare_text.data()), etl::strlen(text.data()));
      CHECK_EQUAL(compare_text.size(), text.size());

      bool is_equal = Equal(compare_text, text);
      CHECK(is_equal);
#if ETL_STRING_TRUNCATION_CHECKS_ENABLED
      CHECK(text.is_truncated());
#endif
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_pop_back)
    {
      Compare_Text compare_text(initial_text.c_str());

      TextBuffer buffer;
      Text text(initial_text.c_str(), buffer.data(), buffer.size());

      compare_text.pop_back();
      compare_text.pop_back();

      text.pop_back();
      text.pop_back();

      CHECK_EQUAL(compare_text.size(), text.size());

      bool is_equal = Equal(compare_text, text);

      CHECK(is_equal);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_insert_position_value)
    {
      const size_t INITIAL_SIZE = 5;
      const value_t INITIAL_VALUE = STR('A');

      for (size_t offset = 0; offset <= INITIAL_SIZE; ++offset)
      {
        Compare_Text compare_text;

        TextBuffer buffer;
        Text text(buffer.data(), buffer.size());

        text.assign(initial_text.begin(), initial_text.begin() + INITIAL_SIZE);
        compare_text.assign(initial_text.begin(), initial_text.begin() + INITIAL_SIZE);

        text.insert(text.begin() + offset, INITIAL_VALUE);
        compare_text.insert(compare_text.begin() + offset, INITIAL_VALUE);

        CHECK_EQUAL(compare_text.size(), text.size());

        bool is_equal = Equal(compare_text, text);

        CHECK(is_equal);
      }
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_insert_position_value_excess)
    {
      Compare_Text compare_text(initial_text.begin(), initial_text.end());

      TextBuffer buffer;
      Text text(initial_text.begin(), initial_text.end(), buffer.data(), buffer.size());

      const value_t INITIAL_VALUE = STR('A');

      size_t offset = 2;
      text.insert(text.begin() + offset, INITIAL_VALUE);
      compare_text.insert(compare_text.begin() + offset, INITIAL_VALUE);
      compare_text.erase(compare_text.end() - 1);

#if ETL_STRING_TRUNCATION_CHECKS_ENABLED
      CHECK(text.is_truncated());
#endif

      bool is_equal = Equal(compare_text, text);
      CHECK(is_equal);

      offset = 0;
      text.insert(text.begin() + offset, STR('A'));
      compare_text.insert(compare_text.begin() + offset, STR('A'));
      compare_text.erase(compare_text.end() - 1);

#if ETL_STRING_TRUNCATION_CHECKS_ENABLED
      CHECK(text.is_truncated());
#endif

      is_equal = Equal(compare_text, text);
      CHECK(is_equal);

      offset = text.size();
      text.insert(text.begin() + offset, STR('A'));
      compare_text.insert(compare_text.begin() + offset, STR('A'));
      compare_text.erase(compare_text.end() - 1);

#if ETL_STRING_TRUNCATION_CHECKS_ENABLED
      CHECK(text.is_truncated());
#endif

      is_equal = Equal(compare_text, text);
      CHECK(is_equal);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_insert_position_n_value)
    {
      Compare_Text compare_text;

      TextBuffer buffer;
      Text text(buffer.data(), buffer.size());

      const size_t INITIAL_SIZE = 5;
      const size_t INSERT_SIZE = 3;
      const value_t INITIAL_VALUE = STR('A');

      for (size_t offset = 0; offset <= INITIAL_SIZE; ++offset)
      {
        text.assign(initial_text.begin(), initial_text.begin() + INITIAL_SIZE);
        compare_text.assign(initial_text.begin(), initial_text.begin() + INITIAL_SIZE);
        text.insert(text.begin() + offset, INSERT_SIZE, INITIAL_VALUE);
        compare_text.insert(compare_text.begin() + offset, INSERT_SIZE, INITIAL_VALUE);

#if ETL_STRING_TRUNCATION_CHECKS_ENABLED
        CHECK(!text.is_truncated());
#endif

        bool is_equal = Equal(compare_text, text);
        CHECK(is_equal);
      }
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_insert_position_n_value_excess)
    {
      Compare_Text compare_text;

      TextBuffer buffer;
      Text text(buffer.data(), buffer.size());

      const size_t INSERT_SIZE = 4;
      const value_t INSERT_VALUE = STR('A');

      size_t offset = 0;
      compare_text.assign(initial_text.begin(), initial_text.end());
      text.assign(initial_text.begin(), initial_text.end());
      compare_text.insert(compare_text.begin() + offset, INSERT_SIZE, INSERT_VALUE);
      compare_text.erase(compare_text.end() - INSERT_SIZE, compare_text.end());
      text.insert(text.begin() + offset, INSERT_SIZE, INSERT_VALUE);

#if ETL_STRING_TRUNCATION_CHECKS_ENABLED
      CHECK(text.is_truncated());
#endif

      bool is_equal = Equal(compare_text, text);
      CHECK(is_equal);

      offset = 2;
      compare_text.assign(initial_text.begin(), initial_text.end());
      text.assign(initial_text.begin(), initial_text.end());
      compare_text.insert(compare_text.begin() + offset, INSERT_SIZE, INSERT_VALUE);
      compare_text.erase(compare_text.end() - INSERT_SIZE, compare_text.end());
      text.assign(initial_text.begin(), initial_text.end());
      text.insert(text.begin() + offset, INSERT_SIZE, INSERT_VALUE);

#if ETL_STRING_TRUNCATION_CHECKS_ENABLED
      CHECK(text.is_truncated());
#endif

      is_equal = Equal(compare_text, text);
      CHECK(is_equal);

      offset = 4;
      compare_text.assign(initial_text.begin(), initial_text.end());
      text.assign(initial_text.begin(), initial_text.end());
      compare_text.insert(compare_text.begin() + offset, INSERT_SIZE, INSERT_VALUE);
      compare_text.erase(compare_text.end() - INSERT_SIZE, compare_text.end());
      text.assign(initial_text.begin(), initial_text.end());
      text.insert(text.begin() + offset, INSERT_SIZE, INSERT_VALUE);

#if ETL_STRING_TRUNCATION_CHECKS_ENABLED
      CHECK(text.is_truncated());
#endif

      is_equal = Equal(compare_text, text);
      CHECK(is_equal);

      offset = text.size();
      compare_text.assign(initial_text.begin(), initial_text.end());
      text.assign(initial_text.begin(), initial_text.end());
      compare_text.insert(compare_text.begin() + offset, INSERT_SIZE, INSERT_VALUE);
      compare_text.erase(compare_text.end() - INSERT_SIZE, compare_text.end());
      text.assign(initial_text.begin(), initial_text.end());
      text.insert(text.begin() + offset, INSERT_SIZE, INSERT_VALUE);

#if ETL_STRING_TRUNCATION_CHECKS_ENABLED
      CHECK(text.is_truncated());
#endif

      is_equal = Equal(compare_text, text);
      CHECK(is_equal);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_insert_position_range)
    {
      const size_t INITIAL_SIZE = 5;

      for (size_t offset = 0; offset <= INITIAL_SIZE; ++offset)
      {
        Compare_Text compare_text;

        TextBuffer buffer;
        Text text(buffer.data(), buffer.size());

        text.assign(initial_text.begin(), initial_text.begin() + INITIAL_SIZE);
        compare_text.assign(initial_text.begin(), initial_text.begin() + INITIAL_SIZE);
        text.insert(text.begin() + offset, insert_text.begin(), insert_text.end());
        compare_text.insert(compare_text.begin() + offset, insert_text.begin(), insert_text.end());

#if ETL_STRING_TRUNCATION_CHECKS_ENABLED
        CHECK(!text.is_truncated());
#endif

        bool is_equal = Equal(compare_text, text);
        CHECK(is_equal);
      }
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_insert_position_range_excess)
    {
      const size_t INITIAL_SIZE = 5;
      const value_t INITIAL_VALUE = STR('A');

      Compare_Text compare_text;

      TextBuffer buffer;
      Text text(buffer.data(), buffer.size());

      size_t offset = 0;

      compare_text.assign(INITIAL_SIZE, INITIAL_VALUE);
      text.assign(INITIAL_SIZE, INITIAL_VALUE);
      compare_text.insert(compare_text.begin() + offset, initial_text.begin(), initial_text.end());
      compare_text.resize(initial_text.size());
      text.insert(text.begin() + offset, initial_text.begin(), initial_text.end());

#if ETL_STRING_TRUNCATION_CHECKS_ENABLED
      CHECK(text.is_truncated());
#endif

      bool is_equal = Equal(compare_text, text);
      CHECK(is_equal);

      offset = 2;

      compare_text.assign(INITIAL_SIZE, INITIAL_VALUE);
      text.assign(INITIAL_SIZE, INITIAL_VALUE);
      compare_text.insert(compare_text.begin() + offset, initial_text.begin(), initial_text.end());
      compare_text.resize(initial_text.size());
      text.insert(text.begin() + offset, initial_text.begin(), initial_text.end());

#if ETL_STRING_TRUNCATION_CHECKS_ENABLED
      CHECK(text.is_truncated());
#endif

      is_equal = Equal(compare_text, text);
      CHECK(is_equal);

      offset = 4;

      compare_text.assign(INITIAL_SIZE, INITIAL_VALUE);
      text.assign(INITIAL_SIZE, INITIAL_VALUE);
      compare_text.insert(compare_text.begin() + offset, initial_text.begin(), initial_text.end());
      compare_text.resize(initial_text.size());
      text.insert(text.begin() + offset, initial_text.begin(), initial_text.end());

#if ETL_STRING_TRUNCATION_CHECKS_ENABLED
      CHECK(text.is_truncated());
#endif

      CHECK_EQUAL(compare_text.size(), text.size());
      is_equal = Equal(compare_text, text);
      CHECK(is_equal);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_insert_position_range_self)
    {
      size_t length = SIZE_L / 2;

      for (size_t offset = 10; offset < length; ++offset)
      {
        Compare_Text compare_text = STR("ABCDEFGHIJKLMNOPQRSTUVWXYZ");
        TextBufferL bufferl;
        Text text(bufferl.data(), bufferl.size());
        text = STR("ABCDEFGHIJKLMNOPQRSTUVWXYZ");

        text.insert(text.begin() + offset, text.begin() + 5, text.begin() + 10);
        compare_text.insert(compare_text.begin() + offset, compare_text.begin() + 5, compare_text.begin() + 10);

        bool is_equal = Equal(compare_text, text);
        CHECK(is_equal);
#if ETL_STRING_TRUNCATION_CHECKS_ENABLED
        CHECK(!text.is_truncated());
#endif
      }
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_insert_size_t_position_string)
    {
      for (size_t offset = 0; offset <= short_text.size(); ++offset)
      {
        Compare_Text compare_text(short_text.begin(), short_text.end());

        TextBuffer buffer;
        Text text(short_text.begin(), short_text.end(), buffer.data(), buffer.size());

        TextBuffer buffer2;
        Text insert(insert_text.begin(), insert_text.end(), buffer2.data(), buffer2.size());

        text.insert(offset, insert);
        compare_text.insert(offset, insert_text);
        compare_text.resize(std::min(compare_text.size(), SIZE));

        bool is_equal = Equal(compare_text, text);
        CHECK(is_equal);
#if ETL_STRING_TRUNCATION_CHECKS_ENABLED
        CHECK(!text.is_truncated());
#endif
      }
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_insert_size_t_position_string_excess)
    {
      for (size_t offset = 0; offset <= initial_text.size(); ++offset)
      {
        Compare_Text compare_text(initial_text.begin(), initial_text.end());

        TextBuffer buffer;
        Text text(initial_text.begin(), initial_text.end(), buffer.data(), buffer.size());

        TextBuffer buffer2;
        Text insert(insert_text.begin(), insert_text.end(), buffer2.data(), buffer2.size());

        text.insert(offset, insert);
        compare_text.insert(offset, insert_text);
        compare_text.resize(std::min(compare_text.size(), SIZE));

        bool is_equal = Equal(compare_text, text);
        CHECK(is_equal);
#if ETL_STRING_TRUNCATION_CHECKS_ENABLED
        CHECK(text.is_truncated());
#endif
      }
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_insert_size_t_position_string_from_truncated)
    {
      for (size_t offset = 0; offset <= short_text.size(); ++offset)
      {
        Compare_Text compare_text(short_text.begin(), short_text.end());

        TextBuffer buffer;
        Text text(short_text.begin(), short_text.end(), buffer.data(), buffer.size());

        TextBuffer buffer2;
        Text insert(longer_text.begin(), longer_text.end(), buffer2.data(), buffer2.size());

        insert.erase(insert.begin(), insert.end());
        insert.append(insert_text.begin(), insert_text.end());

        text.insert(offset, insert);
        compare_text.insert(offset, insert_text);
        compare_text.resize(std::min(compare_text.size(), SIZE));

        bool is_equal = Equal(compare_text, text);
        CHECK(is_equal);
#if ETL_STRING_TRUNCATION_CHECKS_ENABLED
        CHECK(text.is_truncated());
#endif
      }
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_insert_size_t_position_string_subpos_sunlen)
    {
      Compare_Text compare_text(short_text.begin(), short_text.end());

      TextBuffer buffer;
      Text text(short_text.begin(), short_text.end(), buffer.data(), buffer.size());

      TextBuffer buffer2;
      Text insert(insert_text.begin(), insert_text.end(), buffer2.data(), buffer2.size());

      text.insert(0, insert, 0, insert.size());
      compare_text.insert(0, insert_text, 0, insert_text.size());
      compare_text.resize(std::min(compare_text.size(), SIZE));

      bool is_equal = Equal(compare_text, text);
      CHECK(is_equal);
#if ETL_STRING_TRUNCATION_CHECKS_ENABLED
      CHECK(!text.is_truncated());
#endif

      compare_text.assign(short_text.begin(), short_text.end());
      text.assign(short_text.begin(), short_text.end());

      text.insert(2, insert, 2, insert.size() - 2);
      compare_text.insert(2, insert_text, 2, insert_text.size() - 2);
      compare_text.resize(std::min(compare_text.size(), SIZE));

      CHECK_EQUAL(compare_text.size(), text.size());

      is_equal = Equal(compare_text, text);
      CHECK(is_equal);
#if ETL_STRING_TRUNCATION_CHECKS_ENABLED
      CHECK(!text.is_truncated());
#endif

      compare_text.assign(short_text.begin(), short_text.end());
      text.assign(short_text.begin(), short_text.end());

      text.insert(short_text.size(), insert, 0, insert.size());
      compare_text.insert(short_text.size(), insert_text, 0, insert_text.size());
      compare_text.resize(std::min(compare_text.size(), SIZE));

      is_equal = Equal(compare_text, text);
      CHECK(is_equal);
#if ETL_STRING_TRUNCATION_CHECKS_ENABLED
      CHECK(!text.is_truncated());
#endif
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_append_string)
    {
      Compare_Text compare_text(short_text.c_str());

      TextBuffer buffer;
      Text text(short_text.c_str(), buffer.data(), buffer.size());

      TextBuffer buffer2;
      Text append(insert_text.c_str(), buffer2.data(), buffer2.size());

      // Non-overflow.
      compare_text.append(insert_text);
      text.append(append);

      bool is_equal = Equal(compare_text, text);
      CHECK(is_equal);
#if ETL_STRING_TRUNCATION_CHECKS_ENABLED
      CHECK(!text.is_truncated());
#endif

      // Overflow.
      compare_text.assign(short_text.c_str());
      text.assign(short_text.c_str());
      append.assign(initial_text.c_str());

      compare_text.append(initial_text);
      compare_text.resize(std::min(compare_text.size(), SIZE));
      text.append(append);

      is_equal = Equal(compare_text, text);
      CHECK(is_equal);
#if ETL_STRING_TRUNCATION_CHECKS_ENABLED
      CHECK(text.is_truncated());
#endif
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_append_truncated_string)
    {
      TextBuffer buffer;
      Text text(short_text.c_str(), buffer.data(), buffer.size());

      TextBufferS buffers;
      Text append(short_text.c_str(), buffers.data(), buffers.size());
#if ETL_STRING_TRUNCATION_CHECKS_ENABLED
      CHECK(append.is_truncated());
#endif

      text.append(append);
#if ETL_STRING_TRUNCATION_CHECKS_ENABLED
      CHECK(text.is_truncated());
#endif
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_append_string_to_self)
    {
      Compare_Text compare_text(short_text.c_str());

      TextBuffer buffer;
      Text text(short_text.c_str(), buffer.data(), buffer.size());

      // Non-overflow.
      compare_text.append(compare_text);
      text.append(text);

      bool is_equal = Equal(compare_text, text);
      CHECK(is_equal);
#if ETL_STRING_TRUNCATION_CHECKS_ENABLED
      CHECK(!text.is_truncated());
#endif

      // Overflow.
      compare_text.assign(shorter_text.c_str());
      text.assign(shorter_text.c_str());

      compare_text.append(compare_text);
      compare_text.resize(std::min(compare_text.size(), SIZE));
      text.append(text);

      is_equal = Equal(compare_text, text);
      CHECK(is_equal);
#if ETL_STRING_TRUNCATION_CHECKS_ENABLED
      CHECK(text.is_truncated());
#endif
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_append_string_subpos_sublen)
    {
      Compare_Text compare_text(short_text.c_str());

      TextBuffer buffer;
      Text text(short_text.c_str(), buffer.data(), buffer.size());

      TextBuffer buffer2;
      Text append(insert_text.c_str(), buffer2.data(), buffer2.size());

      // Whole string.
      compare_text.append(insert_text, 0, std::u16string::npos);
      text.append(append, 0, Text::npos);

      bool is_equal = Equal(compare_text, text);
      CHECK(is_equal);
#if ETL_STRING_TRUNCATION_CHECKS_ENABLED
      CHECK(!text.is_truncated());
#endif

      // Partial string.
      compare_text.assign(short_text.c_str());
      text.assign(short_text.c_str());
      append.assign(initial_text.c_str());

      compare_text.append(short_text, 1, 3);
      compare_text.resize(std::min(compare_text.size(), SIZE));
      text.append(append, 1, 3);

      is_equal = Equal(compare_text, text);
      CHECK(is_equal);
#if ETL_STRING_TRUNCATION_CHECKS_ENABLED
      CHECK(!text.is_truncated());
#endif

      // Overflow.
      compare_text.assign(short_text.c_str());
      text.assign(short_text.c_str());
      append.assign(initial_text.c_str());

      compare_text.append(initial_text, 1, initial_text.size());
      compare_text.resize(std::min(compare_text.size(), SIZE));
      text.append(append, 1, append.size());

      is_equal = Equal(compare_text, text);
      CHECK(is_equal);
#if ETL_STRING_TRUNCATION_CHECKS_ENABLED
      CHECK(text.is_truncated());
#endif
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_append_truncated_string_subpos_sublen)
    {
      TextBuffer buffer;
      Text text(short_text.c_str(), buffer.data(), buffer.size());

      TextBufferS buffer2;
      Text append(short_text.c_str(), buffer2.data(), buffer2.size());
#if ETL_STRING_TRUNCATION_CHECKS_ENABLED
      CHECK(append.is_truncated());
#endif

      text.append(append, 1, 2);
#if ETL_STRING_TRUNCATION_CHECKS_ENABLED
      CHECK(text.is_truncated());
#endif
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_append_c_string)
    {
      // Non-overflow.
      Compare_Text compare_text(short_text.c_str());

      TextBuffer buffer;
      Text text(short_text.c_str(), buffer.data(), buffer.size());

      // Whole string.
      compare_text.append(insert_text.c_str());
      text.append(insert_text.c_str());

      bool is_equal = Equal(compare_text, text);
      CHECK(is_equal);
#if ETL_STRING_TRUNCATION_CHECKS_ENABLED
      CHECK(!text.is_truncated());
#endif

      // Overflow.
      compare_text.assign(short_text.c_str());
      text.assign(short_text.c_str());

      compare_text.append(initial_text.c_str());
      compare_text.resize(std::min(compare_text.size(), SIZE));
      text.append(initial_text.c_str());

      is_equal = Equal(compare_text, text);
      CHECK(is_equal);
#if ETL_STRING_TRUNCATION_CHECKS_ENABLED
      CHECK(text.is_truncated());
#endif
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_append_n_c)
    {
      // Non-overflow.
      Compare_Text compare_text(short_text.c_str());

      TextBuffer buffer;
      Text text(short_text.c_str(), buffer.data(), buffer.size());

      // Non-overflow.
      compare_text.append(5, STR('A'));
      text.append(5, STR('A'));

      bool is_equal = Equal(compare_text, text);
      CHECK(is_equal);
#if ETL_STRING_TRUNCATION_CHECKS_ENABLED
      CHECK(!text.is_truncated());
#endif

      // Overflow.
      compare_text.assign(short_text.c_str());
      text.assign(short_text.c_str());

      compare_text.append(SIZE, STR('A'));
      compare_text.resize(std::min(compare_text.size(), SIZE));
      text.append(SIZE, STR('A'));

      is_equal = Equal(compare_text, text);
      CHECK(is_equal);
#if ETL_STRING_TRUNCATION_CHECKS_ENABLED
      CHECK(text.is_truncated());
#endif
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_append_range)
    {
      // Non-overflow.
      Compare_Text compare_text(short_text.c_str());

      TextBuffer buffer;
      Text text(short_text.c_str(), buffer.data(), buffer.size());

      TextBuffer buffer2;
      Text append(insert_text.c_str(), buffer2.data(), buffer2.size());

      compare_text.append(insert_text.begin(), insert_text.end());
      text.append(append.begin(), append.end());

      bool is_equal = Equal(compare_text, text);
      CHECK(is_equal);
#if ETL_STRING_TRUNCATION_CHECKS_ENABLED
      CHECK(!text.is_truncated());
#endif

      // Overflow.
      compare_text.assign(short_text.c_str());
      text.assign(short_text.c_str());
      append.assign(initial_text.c_str());

      compare_text.append(initial_text.begin(), initial_text.end());
      compare_text.resize(std::min(compare_text.size(), SIZE));
      text.append(append.begin(), append.end());

      is_equal = Equal(compare_text, text);
      CHECK(is_equal);
#if ETL_STRING_TRUNCATION_CHECKS_ENABLED
      CHECK(text.is_truncated());
#endif
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_replace_position_length_string)
    {
      // Non-overflow short text, npos.
      Compare_Text compare_text(short_text.c_str());

      TextBuffer buffer;
      Text text(short_text.c_str(), buffer.data(), buffer.size());

      compare_text.replace(2, Compare_Text::npos, Compare_Text(STR("Replace")));
      compare_text.resize(std::min(compare_text.size(), SIZE));
      text.replace(2, Text::npos, TextT(STR("Replace")));

      bool is_equal = Equal(compare_text, text);
      CHECK(is_equal);
#if ETL_STRING_TRUNCATION_CHECKS_ENABLED
      CHECK(!text.is_truncated());
#endif

      // Non-overflow short text.
      compare_text.assign(short_text.c_str());
      text.assign(short_text.c_str());

      compare_text.replace(2, 2, Compare_Text(STR("Replace")));
      compare_text.resize(std::min(compare_text.size(), SIZE));
      text.replace(2, 2, TextT(STR("Replace")));

      is_equal = Equal(compare_text, text);
      CHECK(is_equal);
#if ETL_STRING_TRUNCATION_CHECKS_ENABLED
      CHECK(!text.is_truncated());
#endif

      // Overflow short text.
      compare_text.assign(short_text.c_str());
      text.assign(short_text.c_str());

      compare_text.replace(2, 2, Compare_Text(STR("Replace with some text")));
      compare_text.resize(std::min(compare_text.size(), SIZE));
      text.replace(2, 2, TextT(STR("Replace with some text")));

      is_equal = Equal(compare_text, text);
      CHECK(is_equal);
#if ETL_STRING_TRUNCATION_CHECKS_ENABLED
      CHECK(text.is_truncated());
#endif

      // Overflow short text, npos.
      compare_text.assign(short_text.c_str());
      text.assign(short_text.c_str());

      compare_text.replace(2, Compare_Text::npos, Compare_Text(STR("Replace with some text")));
      compare_text.resize(std::min(compare_text.size(), SIZE));
      text.replace(2, Text::npos, TextT(STR("Replace with some text")));

      is_equal = Equal(compare_text, text);
      CHECK(is_equal);
#if ETL_STRING_TRUNCATION_CHECKS_ENABLED
      CHECK(text.is_truncated());
#endif

      // Non-overflow.
      compare_text.assign(initial_text.c_str());
      text.assign(initial_text.c_str());

      compare_text.replace(2, 7, Compare_Text(STR("Replace")));
      compare_text.resize(std::min(compare_text.size(), SIZE));
      text.replace(2, 7, TextT(STR("Replace")));

      is_equal = Equal(compare_text, text);
      CHECK(is_equal);
#if ETL_STRING_TRUNCATION_CHECKS_ENABLED
      CHECK(!text.is_truncated());
#endif

      // Overflow.
      compare_text.assign(initial_text.c_str());
      text.assign(initial_text.c_str());

      compare_text.replace(2, 2, Compare_Text(STR("Replace with some text")));
      compare_text.resize(std::min(compare_text.size(), SIZE));
      text.replace(2, 2, TextT(STR("Replace with some text")));

      is_equal = Equal(compare_text, text);
      CHECK(is_equal);
#if ETL_STRING_TRUNCATION_CHECKS_ENABLED
      CHECK(text.is_truncated());
#endif

      // Overflow, npos.
      compare_text.assign(initial_text.c_str());
      text.assign(initial_text.c_str());

      compare_text.replace(2, Compare_Text::npos, Compare_Text(STR("Replace with some text")));
      compare_text.resize(std::min(compare_text.size(), SIZE));
      text.replace(2, Text::npos, TextT(STR("Replace with some text")));

      is_equal = Equal(compare_text, text);
      CHECK(is_equal);
#if ETL_STRING_TRUNCATION_CHECKS_ENABLED
      CHECK(text.is_truncated());
#endif
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_replace_first_last_string)
    {
      // Non-overflow short text.
      Compare_Text compare_text(short_text.c_str());

      TextBuffer buffer;
      Text text(short_text.c_str(), buffer.data(), buffer.size());

      compare_text.replace(compare_text.begin() + 2, compare_text.begin() + 4, Compare_Text(STR("Replace")));
      compare_text.resize(std::min(compare_text.size(), SIZE));
      text.replace(text.begin() + 2, text.begin() + 4, TextT(STR("Replace")));

      bool is_equal = Equal(compare_text, text);
      CHECK(is_equal);
#if ETL_STRING_TRUNCATION_CHECKS_ENABLED
      CHECK(!text.is_truncated());
#endif

      // Overflow short text.
      compare_text.assign(short_text.c_str());
      text.assign(short_text.c_str());

      compare_text.replace(compare_text.begin() + 2, compare_text.begin() + 4, Compare_Text(STR("Replace with some text")));
      compare_text.resize(std::min(compare_text.size(), SIZE));
      text.replace(text.begin() + 2, text.begin() + 4, TextT(STR("Replace with some text")));

      is_equal = Equal(compare_text, text);
      CHECK(is_equal);
#if ETL_STRING_TRUNCATION_CHECKS_ENABLED
      CHECK(text.is_truncated());
#endif

      // Non-overflow.
      compare_text.assign(initial_text.c_str());
      text.assign(initial_text.c_str());

      compare_text.replace(compare_text.begin() + 2, compare_text.begin() + 9, Compare_Text(STR("Replace")));
      compare_text.resize(std::min(compare_text.size(), SIZE));
      text.replace(text.begin() + 2, text.begin() + 9, TextT(STR("Replace")));

      is_equal = Equal(compare_text, text);
      CHECK(is_equal);
#if ETL_STRING_TRUNCATION_CHECKS_ENABLED
      CHECK(!text.is_truncated());
#endif

      // Overflow.
      compare_text.assign(initial_text.c_str());
      text.assign(initial_text.c_str());

      compare_text.replace(compare_text.begin() + 2, compare_text.begin() + 4, Compare_Text(STR("Replace with some text")));
      compare_text.resize(std::min(compare_text.size(), SIZE));
      text.replace(text.begin() + 2, text.begin() + 4, TextT(STR("Replace with some text")));

      is_equal = Equal(compare_text, text);
      CHECK(is_equal);
#if ETL_STRING_TRUNCATION_CHECKS_ENABLED
      CHECK(text.is_truncated());
#endif
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_replace_position_length_string_subposition_sublength)
    {
      // Non-overflow short text.
      Compare_Text compare_text(short_text.c_str());

      TextBuffer buffer;
      Text text(short_text.c_str(), buffer.data(), buffer.size());

      compare_text.replace(2, 4, Compare_Text(STR("Replace")), 1, 5);
      compare_text.resize(std::min(compare_text.size(), SIZE));
      text.replace(2, 4, TextT(STR("Replace")), 1, 5);

      bool is_equal = Equal(compare_text, text);
      CHECK(is_equal);
#if ETL_STRING_TRUNCATION_CHECKS_ENABLED
      CHECK(!text.is_truncated());
#endif

      // Non-overflow short text, npos.
      compare_text.assign(short_text.c_str());
      text.assign(short_text.c_str());

      compare_text.replace(2, Compare_Text::npos, Compare_Text(STR("Replace")), 1, Compare_Text::npos);
      compare_text.resize(std::min(compare_text.size(), SIZE));
      text.replace(2, Text::npos, TextT(STR("Replace")), 1, Text::npos);

      is_equal = Equal(compare_text, text);
      CHECK(is_equal);
#if ETL_STRING_TRUNCATION_CHECKS_ENABLED
      CHECK(!text.is_truncated());
#endif

      // Overflow short text.
      compare_text.assign(short_text.c_str());
      text.assign(short_text.c_str());

      compare_text.replace(2, 4, Compare_Text(STR("Replace with some text")), 1, 15);
      compare_text.resize(std::min(compare_text.size(), SIZE));
      text.replace(2, 4, TextT(STR("Replace with some text")), 1, 15);

      is_equal = Equal(compare_text, text);
      CHECK(is_equal);
#if ETL_STRING_TRUNCATION_CHECKS_ENABLED
      CHECK(text.is_truncated());
#endif

      // Overflow short text, npos.
      compare_text.assign(short_text.c_str());
      text.assign(short_text.c_str());

      compare_text.replace(2, Compare_Text::npos, Compare_Text(STR("Replace with some text")), 1, Compare_Text::npos);
      compare_text.resize(std::min(compare_text.size(), SIZE));
      text.replace(2, Text::npos, TextT(STR("Replace with some text")), 1, Text::npos);

      is_equal = Equal(compare_text, text);
      CHECK(is_equal);
#if ETL_STRING_TRUNCATION_CHECKS_ENABLED
      CHECK(text.is_truncated());
#endif

      // Non-overflow.
      compare_text.assign(initial_text.c_str());
      text.assign(initial_text.c_str());

      compare_text.replace(2, 7, Compare_Text(STR("Replace")), 1, 5);
      compare_text.resize(std::min(compare_text.size(), SIZE));
      text.replace(2, 7, TextT(STR("Replace")), 1, 5);

      is_equal = Equal(compare_text, text);
      CHECK(is_equal);
#if ETL_STRING_TRUNCATION_CHECKS_ENABLED
      CHECK(!text.is_truncated());
#endif

      // Non-overflow, npos.
      compare_text.assign(initial_text.c_str());
      text.assign(initial_text.c_str());

      compare_text.replace(2, Compare_Text::npos, Compare_Text(STR("Replace")), 1, Compare_Text::npos);
      compare_text.resize(std::min(compare_text.size(), SIZE));
      text.replace(2, Text::npos, TextT(STR("Replace")), 1, Text::npos);

      is_equal = Equal(compare_text, text);
      CHECK(is_equal);
#if ETL_STRING_TRUNCATION_CHECKS_ENABLED
      CHECK(!text.is_truncated());
#endif

      // Overflow.
      compare_text.assign(initial_text.c_str());
      text.assign(initial_text.c_str());

      compare_text.replace(2, 4, Compare_Text(STR("Replace with some text")), 1, 15);
      compare_text.resize(std::min(compare_text.size(), SIZE));
      text.replace(2, 4, TextT(STR("Replace with some text")), 1, 15);

      is_equal = Equal(compare_text, text);
      CHECK(is_equal);
#if ETL_STRING_TRUNCATION_CHECKS_ENABLED
      CHECK(text.is_truncated());
#endif

      // Overflow, npos.
      compare_text.assign(initial_text.c_str());
      text.assign(initial_text.c_str());

      compare_text.replace(2, Compare_Text::npos, Compare_Text(STR("Replace with some text")), 1, Compare_Text::npos);
      compare_text.resize(std::min(compare_text.size(), SIZE));
      text.replace(2, Text::npos, TextT(STR("Replace with some text")), 1, Text::npos);

      is_equal = Equal(compare_text, text);
      CHECK(is_equal);
#if ETL_STRING_TRUNCATION_CHECKS_ENABLED
      CHECK(text.is_truncated());
#endif
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_replace_position_length_pointer)
    {
      // Non-overflow short text.
      Compare_Text compare_text(short_text.c_str());

      TextBuffer buffer;
      Text text(short_text.c_str(), buffer.data(), buffer.size());

      compare_text.replace(2, 4, Compare_Text(STR("Replace")).c_str());
      compare_text.resize(std::min(compare_text.size(), SIZE));
      text.replace(2, 4, TextT(STR("Replace")).c_str());

      bool is_equal = Equal(compare_text, text);
      CHECK(is_equal);
#if ETL_STRING_TRUNCATION_CHECKS_ENABLED
      CHECK(!text.is_truncated());
#endif

      // Non-overflow short text, npos.
      compare_text.assign(short_text.c_str());
      text.assign(short_text.c_str());

      compare_text.replace(2, Compare_Text::npos, Compare_Text(STR("Replace")).c_str());
      compare_text.resize(std::min(compare_text.size(), SIZE));
      text.replace(2, Text::npos, TextT(STR("Replace")).c_str());

      is_equal = Equal(compare_text, text);
      CHECK(is_equal);
#if ETL_STRING_TRUNCATION_CHECKS_ENABLED
      CHECK(!text.is_truncated());
#endif

      // Overflow short text.
      compare_text.assign(short_text.c_str());
      text.assign(short_text.c_str());

      compare_text.replace(2, 4, Compare_Text(STR("Replace with some text")).c_str());
      compare_text.resize(std::min(compare_text.size(), SIZE));
      text.replace(2, 4, TextT(STR("Replace with some text")).c_str());

      is_equal = Equal(compare_text, text);
      CHECK(is_equal);
#if ETL_STRING_TRUNCATION_CHECKS_ENABLED
      CHECK(text.is_truncated());
#endif

      // Overflow short text, npos.
      compare_text.assign(short_text.c_str());
      text.assign(short_text.c_str());

      compare_text.replace(2, Compare_Text::npos, Compare_Text(STR("Replace with some text")).c_str());
      compare_text.resize(std::min(compare_text.size(), SIZE));
      text.replace(2, Text::npos, TextT(STR("Replace with some text")).c_str());

      is_equal = Equal(compare_text, text);
      CHECK(is_equal);
#if ETL_STRING_TRUNCATION_CHECKS_ENABLED
      CHECK(text.is_truncated());
#endif

      // Non-overflow.
      compare_text.assign(initial_text.c_str());
      text.assign(initial_text.c_str());

      compare_text.replace(2, 7, Compare_Text(STR("Replace")).c_str());
      compare_text.resize(std::min(compare_text.size(), SIZE));
      text.replace(2, 7, TextT(STR("Replace")).c_str());

      is_equal = Equal(compare_text, text);
      CHECK(is_equal);
#if ETL_STRING_TRUNCATION_CHECKS_ENABLED
      CHECK(!text.is_truncated());
#endif

      // Non-overflow, npos.
      compare_text.assign(initial_text.c_str());
      text.assign(initial_text.c_str());

      compare_text.replace(2, Compare_Text::npos, Compare_Text(STR("Replace")).c_str());
      compare_text.resize(std::min(compare_text.size(), SIZE));
      text.replace(2, Text::npos, TextT(STR("Replace")).c_str());

      is_equal = Equal(compare_text, text);
      CHECK(is_equal);
#if ETL_STRING_TRUNCATION_CHECKS_ENABLED
      CHECK(!text.is_truncated());
#endif

      // Overflow.
      compare_text.assign(initial_text.c_str());
      text.assign(initial_text.c_str());

      compare_text.replace(2, 4, Compare_Text(STR("Replace with some text")).c_str());
      compare_text.resize(std::min(compare_text.size(), SIZE));
      text.replace(2, 4, TextT(STR("Replace with some text")).c_str());

      is_equal = Equal(compare_text, text);
      CHECK(is_equal);
#if ETL_STRING_TRUNCATION_CHECKS_ENABLED
      CHECK(text.is_truncated());
#endif

      // Overflow, npos.
      compare_text.assign(initial_text.c_str());
      text.assign(initial_text.c_str());

      compare_text.replace(2, Compare_Text::npos, Compare_Text(STR("Replace with some text")).c_str());
      compare_text.resize(std::min(compare_text.size(), SIZE));
      text.replace(2, Text::npos, TextT(STR("Replace with some text")).c_str());

      is_equal = Equal(compare_text, text);
      CHECK(is_equal);
#if ETL_STRING_TRUNCATION_CHECKS_ENABLED
      CHECK(text.is_truncated());
#endif
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_replace_first_last_pointer)
    {
      // Non-overflow short text.
      Compare_Text compare_text(short_text.c_str());

      TextBuffer buffer;
      Text text(short_text.c_str(), buffer.data(), buffer.size());

      compare_text.replace(compare_text.begin() + 2, compare_text.begin() + 4, Compare_Text(STR("Replace")).c_str());
      compare_text.resize(std::min(compare_text.size(), SIZE));
      text.replace(text.begin() + 2, text.begin() + 4, TextT(STR("Replace")).c_str());

      bool is_equal = Equal(compare_text, text);
      CHECK(is_equal);
#if ETL_STRING_TRUNCATION_CHECKS_ENABLED
      CHECK(!text.is_truncated());
#endif

      // Overflow short text.
      compare_text.assign(short_text.c_str());
      text.assign(short_text.c_str());

      compare_text.replace(compare_text.begin() + 2, compare_text.begin() + 4, Compare_Text(STR("Replace with some text")).c_str());
      compare_text.resize(std::min(compare_text.size(), SIZE));
      text.replace(text.begin() + 2, text.begin() + 4, TextT(STR("Replace with some text")).c_str());

      is_equal = Equal(compare_text, text);
      CHECK(is_equal);
#if ETL_STRING_TRUNCATION_CHECKS_ENABLED
      CHECK(text.is_truncated());
#endif

      // Non-overflow.
      compare_text.assign(initial_text.c_str());
      text.assign(initial_text.c_str());

      compare_text.replace(compare_text.begin() + 2, compare_text.begin() + 9, Compare_Text(STR("Replace")).c_str());
      compare_text.resize(std::min(compare_text.size(), SIZE));
      text.replace(text.begin() + 2, text.begin() + 9, TextT(STR("Replace")).c_str());

      is_equal = Equal(compare_text, text);
      CHECK(is_equal);
#if ETL_STRING_TRUNCATION_CHECKS_ENABLED
      CHECK(!text.is_truncated());
#endif

      // Overflow.
      compare_text.assign(initial_text.c_str());
      text.assign(initial_text.c_str());

      compare_text.replace(compare_text.begin() + 2, compare_text.begin() + 4, Compare_Text(STR("Replace with some text")).c_str());
      compare_text.resize(std::min(compare_text.size(), SIZE));
      text.replace(text.begin() + 2, text.begin() + 4, TextT(STR("Replace with some text")).c_str());

      is_equal = Equal(compare_text, text);
      CHECK(is_equal);
#if ETL_STRING_TRUNCATION_CHECKS_ENABLED
      CHECK(text.is_truncated());
#endif
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_replace_position_length_pointer_n)
    {
      // Non-overflow short text.
      Compare_Text compare_text(short_text.c_str());

      TextBuffer buffer;
      Text text(short_text.c_str(), buffer.data(), buffer.size());

      compare_text.replace(2, 4, Compare_Text(STR("Replace")).c_str(), 5);
      compare_text.resize(std::min(compare_text.size(), SIZE));
      text.replace(2, 4, TextT(STR("Replace")).c_str(), 5);

      bool is_equal = Equal(compare_text, text);
      CHECK(is_equal);
#if ETL_STRING_TRUNCATION_CHECKS_ENABLED
      CHECK(!text.is_truncated());
#endif

      // Non-overflow short text, npos.
      compare_text.assign(short_text.c_str());
      text.assign(short_text.c_str());

      compare_text.replace(2, Compare_Text::npos, Compare_Text(STR("Replace")).c_str(), 5);
      compare_text.resize(std::min(compare_text.size(), SIZE));
      text.replace(2, Text::npos, TextT(STR("Replace")).c_str(), 5);

      is_equal = Equal(compare_text, text);
      CHECK(is_equal);
#if ETL_STRING_TRUNCATION_CHECKS_ENABLED
      CHECK(!text.is_truncated());
#endif

      // Overflow short text.
      compare_text.assign(short_text.c_str());
      text.assign(short_text.c_str());

      compare_text.replace(2, 4, Compare_Text(STR("Replace with some text")).c_str(), 15);
      compare_text.resize(std::min(compare_text.size(), SIZE));
      text.replace(2, 4, TextT(STR("Replace with some text")).c_str(), 15);

      is_equal = Equal(compare_text, text);
      CHECK(is_equal);
#if ETL_STRING_TRUNCATION_CHECKS_ENABLED
      CHECK(text.is_truncated());
#endif

      // Overflow short text, npos.
      compare_text.assign(short_text.c_str());
      text.assign(short_text.c_str());

      compare_text.replace(2, Compare_Text::npos, Compare_Text(STR("Replace with some text")).c_str(), 15);
      compare_text.resize(std::min(compare_text.size(), SIZE));
      text.replace(2, Text::npos, TextT(STR("Replace with some text")).c_str(), 15);

      is_equal = Equal(compare_text, text);
      CHECK(is_equal);
#if ETL_STRING_TRUNCATION_CHECKS_ENABLED
      CHECK(text.is_truncated());
#endif

      // Non-overflow.
      compare_text.assign(initial_text.c_str());
      text.assign(initial_text.c_str());

      compare_text.replace(2, 7, Compare_Text(STR("Replace")).c_str(), 5);
      compare_text.resize(std::min(compare_text.size(), SIZE));
      text.replace(2, 7, TextT(STR("Replace")).c_str(), 5);

      is_equal = Equal(compare_text, text);
      CHECK(is_equal);
#if ETL_STRING_TRUNCATION_CHECKS_ENABLED
      CHECK(!text.is_truncated());
#endif

      // Non-overflow, npos.
      compare_text.assign(initial_text.c_str());
      text.assign(initial_text.c_str());

      compare_text.replace(2, Compare_Text::npos, Compare_Text(STR("Replace")).c_str(), 5);
      compare_text.resize(std::min(compare_text.size(), SIZE));
      text.replace(2, Text::npos, TextT(STR("Replace")).c_str(), 5);

      is_equal = Equal(compare_text, text);
      CHECK(is_equal);
#if ETL_STRING_TRUNCATION_CHECKS_ENABLED
      CHECK(!text.is_truncated());
#endif

      // Overflow.
      compare_text.assign(initial_text.c_str());
      text.assign(initial_text.c_str());

      compare_text.replace(2, 4, Compare_Text(STR("Replace with some text")).c_str(), 15);
      compare_text.resize(std::min(compare_text.size(), SIZE));
      text.replace(2, 4, TextT(STR("Replace with some text")).c_str(), 15);

      is_equal = Equal(compare_text, text);
      CHECK(is_equal);
#if ETL_STRING_TRUNCATION_CHECKS_ENABLED
      CHECK(text.is_truncated());
#endif

      // Overflow, npos.
      compare_text.assign(initial_text.c_str());
      text.assign(initial_text.c_str());

      compare_text.replace(2, Compare_Text::npos, Compare_Text(STR("Replace with some text")).c_str(), 15);
      compare_text.resize(std::min(compare_text.size(), SIZE));
      text.replace(2, Text::npos, TextT(STR("Replace with some text")).c_str(), 15);

      is_equal = Equal(compare_text, text);
      CHECK(is_equal);
#if ETL_STRING_TRUNCATION_CHECKS_ENABLED
      CHECK(text.is_truncated());
#endif
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_replace_first_last_pointer_n)
    {
      // Non-overflow short text.
      Compare_Text compare_text(short_text.c_str());

      TextBuffer buffer;
      Text text(short_text.c_str(), buffer.data(), buffer.size());

      compare_text.replace(compare_text.begin() + 2, compare_text.begin() + 4, Compare_Text(STR("Replace")).c_str(), 5);
      compare_text.resize(std::min(compare_text.size(), SIZE));
      text.replace(text.begin() + 2, text.begin() + 4, TextT(STR("Replace")).c_str(), 5);

      bool is_equal = Equal(compare_text, text);
      CHECK(is_equal);
#if ETL_STRING_TRUNCATION_CHECKS_ENABLED
      CHECK(!text.is_truncated());
#endif

      // Overflow short text.
      compare_text.assign(short_text.c_str());
      text.assign(short_text.c_str());

      compare_text.replace(compare_text.begin() + 2, compare_text.begin() + 4, Compare_Text(STR("Replace with some text")).c_str(), 15);
      compare_text.resize(std::min(compare_text.size(), SIZE));
      text.replace(text.begin() + 2, text.begin() + 4, TextT(STR("Replace with some text")).c_str(), 15);

      is_equal = Equal(compare_text, text);
      CHECK(is_equal);
#if ETL_STRING_TRUNCATION_CHECKS_ENABLED
      CHECK(text.is_truncated());
#endif

      // Non-overflow.
      compare_text.assign(initial_text.c_str());
      text.assign(initial_text.c_str());

      compare_text.replace(compare_text.begin() + 2, compare_text.begin() + 9, Compare_Text(STR("Replace")).c_str(), 5);
      compare_text.resize(std::min(compare_text.size(), SIZE));
      text.replace(text.begin() + 2, text.begin() + 9, TextT(STR("Replace")).c_str(), 5);

      is_equal = Equal(compare_text, text);
      CHECK(is_equal);
#if ETL_STRING_TRUNCATION_CHECKS_ENABLED
      CHECK(!text.is_truncated());
#endif

      // Overflow.
      compare_text.assign(initial_text.c_str());
      text.assign(initial_text.c_str());

      compare_text.replace(compare_text.begin() + 2, compare_text.begin() + 4, Compare_Text(STR("Replace with some text")).c_str(), 15);
      compare_text.resize(std::min(compare_text.size(), SIZE));
      text.replace(text.begin() + 2, text.begin() + 4, TextT(STR("Replace with some text")).c_str(), 15);

      is_equal = Equal(compare_text, text);
      CHECK(is_equal);
#if ETL_STRING_TRUNCATION_CHECKS_ENABLED
      CHECK(text.is_truncated());
#endif
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_replace_position_length_n_c)
    {
      // Non-overflow short text.
      Compare_Text compare_text(short_text.c_str());

      TextBuffer buffer;
      Text text(short_text.c_str(), buffer.data(), buffer.size());

      compare_text.replace(2, 4, 7, STR('A'));
      compare_text.resize(std::min(compare_text.size(), SIZE));
      text.replace(2, 4, 7, STR('A'));

      bool is_equal = Equal(compare_text, text);
      CHECK(is_equal);
#if ETL_STRING_TRUNCATION_CHECKS_ENABLED
      CHECK(!text.is_truncated());
#endif

      // Non-overflow short text, npos.
      compare_text.assign(short_text.c_str());
      text.assign(short_text.c_str());

      compare_text.replace(2, Compare_Text::npos, 7, STR('A'));
      compare_text.resize(std::min(compare_text.size(), SIZE));
      text.replace(2, Text::npos, 7, STR('A'));

      is_equal = Equal(compare_text, text);
      CHECK(is_equal);
#if ETL_STRING_TRUNCATION_CHECKS_ENABLED
      CHECK(!text.is_truncated());
#endif

      // Overflow short text.
      compare_text.assign(short_text.c_str());
      text.assign(short_text.c_str());

      compare_text.replace(2, 4, 15, STR('A'));
      compare_text.resize(std::min(compare_text.size(), SIZE));
      text.replace(2, 4, 15, STR('A'));

      is_equal = Equal(compare_text, text);
      CHECK(is_equal);
#if ETL_STRING_TRUNCATION_CHECKS_ENABLED
      CHECK(text.is_truncated());
#endif

      // Overflow short text, npos.
      compare_text.assign(short_text.c_str());
      text.assign(short_text.c_str());

      compare_text.replace(2, Compare_Text::npos, 15, STR('A'));
      compare_text.resize(std::min(compare_text.size(), SIZE));
      text.replace(2, Text::npos, 15, STR('A'));

      is_equal = Equal(compare_text, text);
      CHECK(is_equal);
#if ETL_STRING_TRUNCATION_CHECKS_ENABLED
      CHECK(text.is_truncated());
#endif

      // Non-overflow.
      compare_text.assign(initial_text.c_str());
      text.assign(initial_text.c_str());

      compare_text.replace(2, 7, 7, STR('A'));
      compare_text.resize(std::min(compare_text.size(), SIZE));
      text.replace(2, 7, 7, STR('A'));

      is_equal = Equal(compare_text, text);
      CHECK(is_equal);
#if ETL_STRING_TRUNCATION_CHECKS_ENABLED
      CHECK(!text.is_truncated());
#endif

      // Non-overflow, npos.
      compare_text.assign(initial_text.c_str());
      text.assign(initial_text.c_str());

      compare_text.replace(2, Compare_Text::npos, 7, STR('A'));
      compare_text.resize(std::min(compare_text.size(), SIZE));
      text.replace(2, Text::npos, 7, STR('A'));

      is_equal = Equal(compare_text, text);
      CHECK(is_equal);
#if ETL_STRING_TRUNCATION_CHECKS_ENABLED
      CHECK(!text.is_truncated());
#endif

      // Overflow.
      compare_text.assign(initial_text.c_str());
      text.assign(initial_text.c_str());

      compare_text.replace(2, 4, 15, STR('A'));
      compare_text.resize(std::min(compare_text.size(), SIZE));
      text.replace(2, 4, 15, STR('A'));

      is_equal = Equal(compare_text, text);
      CHECK(is_equal);
#if ETL_STRING_TRUNCATION_CHECKS_ENABLED
      CHECK(text.is_truncated());
#endif

      // Overflow, npos.
      compare_text.assign(initial_text.c_str());
      text.assign(initial_text.c_str());

      compare_text.replace(2, Compare_Text::npos, 15, STR('A'));
      compare_text.resize(std::min(compare_text.size(), SIZE));
      text.replace(2, Text::npos, 15, STR('A'));

      is_equal = Equal(compare_text, text);
      CHECK(is_equal);
#if ETL_STRING_TRUNCATION_CHECKS_ENABLED
      CHECK(text.is_truncated());
#endif
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_replace_first_last_n_c)
    {
      // Non-overflow short text.
      Compare_Text compare_text(short_text.c_str());

      TextBuffer buffer;
      Text text(short_text.c_str(), buffer.data(), buffer.size());

      compare_text.replace(compare_text.begin() + 2, compare_text.begin() + 4, 7, STR('A'));
      compare_text.resize(std::min(compare_text.size(), SIZE));
      text.replace(text.begin() + 2, text.begin() + 4, 7, STR('A'));

      bool is_equal = Equal(compare_text, text);
      CHECK(is_equal);
#if ETL_STRING_TRUNCATION_CHECKS_ENABLED
      CHECK(!text.is_truncated());
#endif

      // Overflow short text.
      compare_text.assign(short_text.c_str());
      text.assign(short_text.c_str());

      compare_text.replace(compare_text.begin() + 2, compare_text.begin() + 4, 15, STR('A'));
      compare_text.resize(std::min(compare_text.size(), SIZE));
      text.replace(text.begin() + 2, text.begin() + 4, 15, STR('A'));

      is_equal = Equal(compare_text, text);
      CHECK(is_equal);
#if ETL_STRING_TRUNCATION_CHECKS_ENABLED
      CHECK(text.is_truncated());
#endif

      // Non-overflow.
      compare_text.assign(initial_text.c_str());
      text.assign(initial_text.c_str());

      compare_text.replace(compare_text.begin() + 2, compare_text.begin() + 9, 7, STR('A'));
      compare_text.resize(std::min(compare_text.size(), SIZE));
      text.replace(text.begin() + 2, text.begin() + 9, 7, STR('A'));

      is_equal = Equal(compare_text, text);
      CHECK(is_equal);
#if ETL_STRING_TRUNCATION_CHECKS_ENABLED
      CHECK(!text.is_truncated());
#endif

      // Overflow.
      compare_text.assign(initial_text.c_str());
      text.assign(initial_text.c_str());

      compare_text.replace(compare_text.begin() + 2, compare_text.begin() + 4, 15, STR('A'));
      compare_text.resize(std::min(compare_text.size(), SIZE));
      text.replace(text.begin() + 2, text.begin() + 4, 15, STR('A'));

      is_equal = Equal(compare_text, text);
      CHECK(is_equal);
#if ETL_STRING_TRUNCATION_CHECKS_ENABLED
      CHECK(text.is_truncated());
#endif
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_replace_first_last_first_last)
    {
      // Non-overflow short text.
      Compare_Text compare_text(short_text.c_str());

      TextBuffer buffer;
      Text text(short_text.c_str(), buffer.data(), buffer.size());

      Compare_Text replace(STR("Replace"));
      Compare_Text replace_long(STR("Replace with some text"));

      compare_text.replace(compare_text.begin() + 2, compare_text.begin() + 4, replace.begin() + 1, replace.begin() + 5);
      compare_text.resize(std::min(compare_text.size(), SIZE));
      text.replace(text.begin() + 2, text.begin() + 4, replace.begin() + 1, replace.begin() + 5);

      bool is_equal = Equal(compare_text, text);
      CHECK(is_equal);
#if ETL_STRING_TRUNCATION_CHECKS_ENABLED
      CHECK(!text.is_truncated());
#endif

      // Overflow short text.
      compare_text.assign(short_text.c_str());
      text.assign(short_text.c_str());

      compare_text.replace(compare_text.begin() + 2, compare_text.begin() + 4, replace_long.begin() + 1, replace_long.begin() + 15);
      compare_text.resize(std::min(compare_text.size(), SIZE));
      text.replace(text.begin() + 2, text.begin() + 4, replace_long.begin() + 1, replace_long.begin() + 15);

      is_equal = Equal(compare_text, text);
      CHECK(is_equal);
#if ETL_STRING_TRUNCATION_CHECKS_ENABLED
      CHECK(text.is_truncated());
#endif

      // Non-overflow.
      compare_text.assign(initial_text.c_str());
      text.assign(initial_text.c_str());

      compare_text.replace(compare_text.begin() + 2, compare_text.begin() + 7, replace.begin() + 1, replace.begin() + 5);
      compare_text.resize(std::min(compare_text.size(), SIZE));
      text.replace(text.begin() + 2, text.begin() + 7, replace_long.begin() + 1, replace_long.begin() + 5);

      is_equal = Equal(compare_text, text);
      CHECK(is_equal);
#if ETL_STRING_TRUNCATION_CHECKS_ENABLED
      CHECK(!text.is_truncated());
#endif

      // Overflow.
      compare_text.assign(initial_text.c_str());
      text.assign(initial_text.c_str());

      compare_text.replace(compare_text.begin() + 2, compare_text.begin() + 4, replace_long.begin() + 1, replace_long.begin() + 15);
      compare_text.resize(std::min(compare_text.size(), SIZE));
      text.replace(text.begin() + 2, text.begin() + 4, replace_long.begin() + 1, replace_long.begin() + 15);

      is_equal = Equal(compare_text, text);
      CHECK(is_equal);
#if ETL_STRING_TRUNCATION_CHECKS_ENABLED
      CHECK(text.is_truncated());
#endif
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_erase_single)
    {
      Compare_Text compare_text(initial_text.c_str());

      TextBuffer buffer;
      Text text(initial_text.c_str(), buffer.data(), buffer.size());

      compare_text.erase(compare_text.begin() + 2);
      text.erase(text.begin() + 2);

      bool is_equal = Equal(compare_text, text);
      CHECK(is_equal);
#if ETL_STRING_TRUNCATION_CHECKS_ENABLED
      CHECK(!text.is_truncated());
#endif
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_erase_range)
    {
      Compare_Text compare_text(initial_text.c_str());

      TextBuffer buffer;
      Text text(initial_text.c_str(), buffer.data(), buffer.size());

      compare_text.erase(compare_text.begin() + 2, compare_text.begin() + 4);

      text.erase(text.begin() + 2, text.begin() + 4);

      bool is_equal = Equal(compare_text, text);
      CHECK(is_equal);
#if ETL_STRING_TRUNCATION_CHECKS_ENABLED
      CHECK(!text.is_truncated());
#endif
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_clear)
    {
      TextBuffer buffer;
      Text text(initial_text.c_str(), buffer.data(), buffer.size());
      text.clear();

      CHECK_EQUAL(text.size(), size_t(0));
#if ETL_STRING_TRUNCATION_CHECKS_ENABLED
      CHECK(!text.is_truncated());
#endif
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_iterator)
    {
      Compare_Text compare_text(initial_text.c_str());

      TextBuffer buffer;
      Text text(initial_text.c_str(), buffer.data(), buffer.size());

      bool is_equal = std::equal(text.begin(), text.end(), compare_text.begin());
      CHECK(is_equal);
#if ETL_STRING_TRUNCATION_CHECKS_ENABLED
      CHECK(!text.is_truncated());
#endif
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_const_iterator)
    {
      Compare_Text compare_text(initial_text.c_str());

      TextBuffer buffer;
      Text text(initial_text.c_str(), buffer.data(), buffer.size());

      bool is_equal = std::equal(text.cbegin(), text.cend(), compare_text.cbegin());
      CHECK(is_equal);
#if ETL_STRING_TRUNCATION_CHECKS_ENABLED
      CHECK(!text.is_truncated());
#endif
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_reverse_iterator)
    {
      Compare_Text compare_text(initial_text.c_str());

      TextBuffer buffer;
      Text text(initial_text.c_str(), buffer.data(), buffer.size());

      bool is_equal = std::equal(text.rbegin(), text.rend(), compare_text.rbegin());
      CHECK(is_equal);
#if ETL_STRING_TRUNCATION_CHECKS_ENABLED
      CHECK(!text.is_truncated());
#endif
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_const_reverse_iterator)
    {
      Compare_Text compare_text(initial_text.c_str());

      TextBuffer buffer;
      Text text(initial_text.c_str(), buffer.data(), buffer.size());

      bool is_equal = std::equal(text.crbegin(), text.crend(), compare_text.crbegin());
      CHECK(is_equal);
#if ETL_STRING_TRUNCATION_CHECKS_ENABLED
      CHECK(!text.is_truncated());
#endif
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_equal)
    {
      TextBuffer buffer;
      const Text initial1(initial_text.c_str(), buffer.data(), buffer.size());

      TextBuffer buffer2;
      const Text initial2(initial_text.c_str(), buffer2.data(), buffer2.size());

      CHECK(initial1 == initial2);

      TextBuffer buffer3;
      const Text different(different_text.c_str(), buffer3.data(), buffer3.size());

      CHECK(!(initial1 == different));

      TextBuffer buffer4;
      const Text shorter(shorter_text.c_str(), buffer4.data(), buffer4.size());

      CHECK(!(shorter == initial1));
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_not_equal)
    {
      TextBuffer buffer;
      const Text initial1(initial_text.c_str(), buffer.data(), buffer.size());

      TextBuffer buffer2;
      const Text initial2(initial_text.c_str(), buffer2.data(), buffer2.size());

      CHECK(!(initial1 != initial2));

      TextBuffer buffer3;
      const Text different(different_text.begin(), different_text.end(), buffer3.data(), buffer3.size());

      CHECK(initial1 != different);

      TextBuffer buffer4;
      const Text shorter(shorter_text.begin(), shorter_text.end(), buffer4.data(), buffer4.size());

      CHECK(shorter != initial1);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_less_than)
    {
      TextBuffer buffer;
      const Text less(less_text.c_str(), buffer.data(), buffer.size());

      TextBuffer buffer2;
      const Text initial(initial_text.c_str(), buffer2.data(), buffer2.size());

      // String-String
      CHECK((less < initial) == (less_text < initial_text));
      CHECK((initial < less) == (initial_text < less_text));

      TextBuffer buffer3;
      const Text greater(greater_text.c_str(), buffer3.data(), buffer3.size());
      CHECK((greater < initial) == (greater_text < initial_text));
      CHECK((initial < greater) == (initial_text < greater_text));

      TextBuffer buffer4;
      const Text shorter(shorter_text.c_str(), buffer4.data(), buffer4.size());
      CHECK((shorter < initial) == (shorter_text < initial_text));
      CHECK((initial < shorter) == (initial_text < shorter_text));

      CHECK((initial < initial) == (initial_text < initial_text));
      CHECK((initial < initial) == (initial_text < initial_text));

      // String-Pointer Pointer-String
      CHECK((less < pinitial_text) == (less_text < pinitial_text));
      CHECK((pinitial_text < less) == (pinitial_text < less_text));

      CHECK((greater < pinitial_text) == (greater_text < pinitial_text));
      CHECK((pinitial_text < greater) == (pinitial_text < greater_text));

      CHECK((shorter < pinitial_text) == (shorter_text < pinitial_text));
      CHECK((pinitial_text < shorter) == (pinitial_text < shorter_text));

      CHECK((initial < pinitial_text) == (initial_text < pinitial_text));
      CHECK((pinitial_text < initial) == (pinitial_text < initial_text));
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_less_than_or_equal)
    {
      TextBuffer buffer;
      const Text less(less_text.c_str(), buffer.data(), buffer.size());

      TextBuffer buffer2;
      const Text initial(initial_text.c_str(), buffer2.data(), buffer2.size());

      // String-String
      CHECK((less <= initial) == (less_text <= initial_text));
      CHECK((initial <= less) == (initial_text <= less_text));

      TextBuffer buffer3;
      const Text greater(greater_text.c_str(), buffer3.data(), buffer3.size());
      CHECK((greater <= initial) == (greater_text <= initial_text));
      CHECK((initial <= greater) == (initial_text <= greater_text));

      TextBuffer buffer4;
      const Text shorter(shorter_text.c_str(), buffer4.data(), buffer4.size());
      CHECK((shorter <= initial) == (shorter_text <= initial_text));
      CHECK((initial <= shorter) == (initial_text <= shorter_text));

      CHECK((initial <= initial) == (initial_text <= initial_text));
      CHECK((initial <= initial) == (initial_text <= initial_text));

      // String-Pointer Pointer-String
      CHECK((less <= pinitial_text) == (less_text <= pinitial_text));
      CHECK((pinitial_text <= less) == (pinitial_text <= less_text));

      CHECK((greater <= pinitial_text) == (greater_text <= pinitial_text));
      CHECK((pinitial_text <= greater) == (pinitial_text <= greater_text));

      CHECK((shorter <= pinitial_text) == (shorter_text <= pinitial_text));
      CHECK((pinitial_text <= shorter) == (pinitial_text <= shorter_text));

      CHECK((initial <= pinitial_text) == (initial_text <= pinitial_text));
      CHECK((pinitial_text <= initial) == (pinitial_text <= initial_text));
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_greater_than)
    {
      TextBuffer buffer;
      const Text less(less_text.c_str(), buffer.data(), buffer.size());

      TextBuffer buffer2;
      const Text initial(initial_text.c_str(), buffer2.data(), buffer2.size());

      // String-String
      CHECK((less > initial) == (less_text > initial_text));
      CHECK((initial > less) == (initial_text > less_text));

      TextBuffer buffer3;
      const Text greater(greater_text.c_str(), buffer3.data(), buffer3.size());
      CHECK((greater > initial) == (greater_text > initial_text));
      CHECK((initial > greater) == (initial_text > greater_text));

      TextBuffer buffer4;
      const Text shorter(shorter_text.c_str(), buffer4.data(), buffer4.size());
      CHECK((shorter > initial) == (shorter_text > initial_text));
      CHECK((initial > shorter) == (initial_text > shorter_text));

      CHECK((initial > initial) == (initial_text > initial_text));
      CHECK((initial > initial) == (initial_text > initial_text));

      // String-Pointer Pointer-String
      CHECK((less > pinitial_text) == (less_text > pinitial_text));
      CHECK((pinitial_text > less) == (pinitial_text > less_text));

      CHECK((greater > pinitial_text) == (greater_text > pinitial_text));
      CHECK((pinitial_text > greater) == (pinitial_text > greater_text));

      CHECK((shorter > pinitial_text) == (shorter_text > pinitial_text));
      CHECK((pinitial_text > shorter) == (pinitial_text > shorter_text));

      CHECK((initial > pinitial_text) == (initial_text > pinitial_text));
      CHECK((pinitial_text > initial) == (pinitial_text > initial_text));
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_greater_than_or_equal)
    {
      TextBuffer buffer;
      const Text less(less_text.begin(), less_text.end(), buffer.data(), buffer.size());

      TextBuffer buffer2;
      const Text initial(initial_text.begin(), initial_text.end(), buffer2.data(), buffer2.size());

      // String-String
      CHECK((less >= initial) == (less_text >= initial_text));
      CHECK((initial >= less) == (initial_text >= less_text));

      TextBuffer buffer3;
      const Text greater(greater_text.begin(), greater_text.end(), buffer3.data(), buffer3.size());
      CHECK((greater >= initial) == (greater_text >= initial_text));
      CHECK((initial >= greater) == (initial_text >= greater_text));

      TextBuffer buffer4;
      const Text shorter(shorter_text.begin(), shorter_text.end(), buffer4.data(), buffer4.size());
      CHECK((shorter >= initial) == (shorter_text >= initial_text));
      CHECK((initial >= shorter) == (initial_text > shorter_text));

      CHECK((initial >= initial) == (initial_text >= initial_text));
      CHECK((initial >= initial) == (initial_text >= initial_text));

      // String-Pointer Pointer-String
      CHECK((less >= pinitial_text) == (less_text >= pinitial_text));
      CHECK((pinitial_text >= less) == (pinitial_text >= less_text));

      CHECK((greater >= pinitial_text) == (greater_text >= pinitial_text));
      CHECK((pinitial_text >= greater) == (pinitial_text >= greater_text));

      CHECK((shorter >= pinitial_text) == (shorter_text >= pinitial_text));
      CHECK((pinitial_text >= shorter) == (pinitial_text >= shorter_text));

      CHECK((initial >= pinitial_text) == (initial_text >= pinitial_text));
      CHECK((pinitial_text >= initial) == (pinitial_text >= initial_text));
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_copy)
    {
      Compare_Text compare_text(initial_text.c_str());

      TextBuffer buffer;
      Text text(initial_text.c_str(), buffer.data(), buffer.size());

      value_t buffer1[SIZE];
      value_t buffer2[SIZE];

      size_t length1 = compare_text.copy(buffer1, 5, 2);
      buffer1[length1] = STR('\0');

      size_t length2 = text.copy(buffer2, 5, 2);
      buffer2[length2] = STR('\0');

      CHECK_EQUAL(length1, length2);
#if ETL_STRING_TRUNCATION_CHECKS_ENABLED
      CHECK(!text.is_truncated());
#endif

      bool is_equal = std::equal(buffer1,
        buffer1 + length1,
        buffer2);
      CHECK(is_equal);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_find_string)
    {
      const value_t* the_haystack = STR("A haystack with a needle and another needle");

      std::u16string compare_needle(STR("needle"));

      TextBuffer buffer;
      Text needle(STR("needle"), buffer.data(), buffer.size());

      std::u16string compare_haystack(the_haystack);

      TextBufferL buffer2;
      Text haystack(the_haystack, buffer2.data(), buffer2.size());

      size_t position1 = 0;
      size_t position2 = 0;

      position1 = compare_haystack.find(compare_needle, position1);
      position2 = haystack.find(needle, position2);
      CHECK_EQUAL(position1, position2);

      position1 = compare_haystack.find(compare_needle, position1 + 1);
      position2 = haystack.find(needle, position2 + 1);
      CHECK_EQUAL(position1, position2);

      position2 = haystack.find(needle, position2 + 1);
      CHECK_EQUAL(etl::u16string<50>::npos, position2);

      etl::u16string<50> pin(STR("pin"));
      position2 = haystack.find(pin);
      CHECK_EQUAL(IText::npos, position2);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_find_pointer)
    {
      const value_t* the_haystack = STR("A haystack with a needle and another needle");

      const value_t* needle = STR("needle");

      std::u16string compare_haystack(the_haystack);

      TextBufferL buffer;
      Text haystack(the_haystack, buffer.data(), buffer.size());

      size_t position1 = 0;
      size_t position2 = 0;

      position1 = compare_haystack.find(needle, position1);
      position2 = haystack.find(needle, position2);
      CHECK_EQUAL(position1, position2);

      position1 = compare_haystack.find(needle, position1 + 1);
      position2 = haystack.find(needle, position2 + 1);
      CHECK_EQUAL(position1, position2);

      position2 = haystack.find(needle, position2 + 1);
      CHECK_EQUAL(IText::npos, position2);

      const value_t* pin = STR("pin");
      position2 = haystack.find(pin);
      CHECK_EQUAL(IText::npos, position2);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_find_char_pointer_n)
    {
      const value_t* the_haystack = STR("A haystack with a needle and another needle");

      const value_t* needle = STR("needle");

      std::u16string compare_haystack(the_haystack);

      TextBufferL buffer;
      Text haystack(the_haystack, buffer.data(), buffer.size());

      size_t position1 = 0;
      size_t position2 = 0;

      position1 = compare_haystack.find(needle, position1, 3);
      position2 = haystack.find(needle, position2, 3);
      CHECK_EQUAL(position1, position2);

      position1 = compare_haystack.find(needle, position1 + 1, 3);
      position2 = haystack.find(needle, position2 + 1, 3);
      CHECK_EQUAL(position1, position2);

      position2 = haystack.find(needle, position2 + 1, 3);
      CHECK_EQUAL(IText::npos, position2);

      const value_t* pin = STR("pin");
      position2 = haystack.find(pin, 0, 3);
      CHECK_EQUAL(IText::npos, position2);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_rfind_string)
    {
      const value_t* the_haystack = STR("A haystack with a needle and another needle");

      std::u16string compare_needle(STR("needle"));

      TextBufferL buffer;
      Text needle(STR("needle"), buffer.data(), buffer.size());

      std::u16string compare_haystack(the_haystack);

      TextBufferL buffer2;
      Text haystack(the_haystack, buffer2.data(), buffer2.size());

      size_t position1 = std::u16string::npos;
      size_t position2 = etl::u16string<50>::npos;

      position1 = compare_haystack.rfind(compare_needle, position1);
      position2 = haystack.rfind(needle, position2);
      CHECK_EQUAL(position1, position2);

      position1 = compare_haystack.rfind(compare_needle, compare_haystack.size() - 10);
      position2 = haystack.rfind(needle, haystack.size() - 10);
      CHECK_EQUAL(position1, position2);

      TextBufferL buffer3;
      Text pin(STR("pin"), buffer3.data(), buffer3.size());
      position2 = haystack.rfind(pin);
      CHECK_EQUAL(IText::npos, position2);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_rfind_pointer)
    {
      const value_t* the_haystack = STR("A haystack with a needle and another needle");

      std::u16string compare_haystack(the_haystack);

      TextBufferL buffer;
      Text haystack(the_haystack, buffer.data(), buffer.size());

      const value_t* needle = STR("needle");

      size_t position1 = std::u16string::npos;
      size_t position2 = etl::u16string<50>::npos;

      position1 = compare_haystack.rfind(needle, position1);
      position2 = haystack.rfind(needle, position2);
      CHECK_EQUAL(position1, position2);

      position1 = compare_haystack.rfind(needle, compare_haystack.size() - 10);
      position2 = haystack.rfind(needle, haystack.size() - 10);
      CHECK_EQUAL(position1, position2);

      TextBufferL buffer2;
      Text pin(STR("pin"), buffer2.data(), buffer2.size());
      position2 = haystack.rfind(pin);
      CHECK_EQUAL(IText::npos, position2);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_rfind_pointer_n)
    {
      const value_t* the_haystack = STR("A haystack with a needle and another needle");

      std::u16string compare_haystack(the_haystack);

      TextBufferL buffer;
      Text haystack(the_haystack, buffer.data(), buffer.size());

      const value_t* needle = STR("needle");

      size_t position1 = std::u16string::npos;

      size_t position2 = Text::npos;

      position1 = compare_haystack.rfind(needle, position1, 3);
      position2 = haystack.rfind(needle, position2, 3);
      CHECK_EQUAL(position1, position2);

      position1 = compare_haystack.rfind(needle, compare_haystack.size() - 10, 3);
      position2 = haystack.rfind(needle, haystack.size() - 10, 3);
      CHECK_EQUAL(position1, position2);

      position2 = haystack.rfind(STR("pin"), 3);
      CHECK_EQUAL(IText::npos, position2);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_rfind_c_position)
    {
      const value_t* the_haystack = STR("A haystack with a needle and another needle");

      std::u16string compare_haystack(the_haystack);

      TextBufferL buffer;
      Text haystack(the_haystack, buffer.data(), buffer.size());

      size_t position1 = std::u16string::npos;
      size_t position2 = etl::u16string<50>::npos;

      position1 = compare_haystack.rfind(STR('e'), position1);
      position2 = haystack.rfind(STR('e'), position2);
      CHECK_EQUAL(position1, position2);

      position1 = compare_haystack.rfind(STR('e'), compare_haystack.size() - 10);
      position2 = haystack.rfind(STR('e'), haystack.size() - 10);
      CHECK_EQUAL(position1, position2);

      position2 = haystack.rfind(STR('z'));
      CHECK_EQUAL(IText::npos, position2);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_compare_string)
    {
      Compare_Text compare_text(STR("ABCDEF"));

      TextBuffer buffer;
      Text text(STR("ABCDEF"), buffer.data(), buffer.size());

      int compare_result;
      int result;

      // Equal.
      compare_result = compare_text.compare(Compare_Text(STR("ABCDEF")));
      result = text.compare(TextT(STR("ABCDEF")));
      CHECK(compares_agree(compare_result, result));

      // Less.
      compare_result = compare_text.compare(Compare_Text(STR("ABCDEE")));
      result = text.compare(TextT(STR("ABCDEE")));
      CHECK(compares_agree(compare_result, result));

      // Greater.
      compare_result = compare_text.compare(Compare_Text(STR("ABCDEG")));
      result = text.compare(TextT(STR("ABCDEG")));
      CHECK(compares_agree(compare_result, result));

      // Shorter.
      compare_result = compare_text.compare(Compare_Text(STR("ABCDE")));
      result = text.compare(TextT(STR("ABCDE")));
      CHECK(compares_agree(compare_result, result));

      // Longer.
      compare_result = compare_text.compare(Compare_Text(STR("ABCDEFG")));
      result = text.compare(TextT(STR("ABCDEFG")));
      CHECK(compares_agree(compare_result, result));
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_compare_positon_length_string)
    {
      Compare_Text compare_text(STR("xxxABCDEFyyy"));

      TextBuffer buffer;
      Text text(STR("xxxABCDEFyyy"), buffer.data(), buffer.size());

      int compare_result;
      int result;

      // Equal.
      compare_result = compare_text.compare(3, 6, Compare_Text(STR("ABCDEF")));
      result = text.compare(3, 6, TextT(STR("ABCDEF")));
      CHECK(compares_agree(compare_result, result));

      // Less.
      compare_result = compare_text.compare(3, 6, Compare_Text(STR("ABCDEE")));
      result = text.compare(3, 6, TextT(STR("ABCDEE")));
      CHECK(compares_agree(compare_result, result));

      // Greater.
      compare_result = compare_text.compare(3, 6, Compare_Text(STR("ABCDEG")));
      result = text.compare(3, 6, TextT(STR("ABCDEG")));
      CHECK(compares_agree(compare_result, result));

      // Shorter.
      compare_result = compare_text.compare(3, 6, Compare_Text(STR("ABCDE")));
      result = text.compare(3, 6, TextT(STR("ABCDE")));
      CHECK(compares_agree(compare_result, result));

      // Longer.
      compare_result = compare_text.compare(3, 6, Compare_Text(STR("ABCDEFG")));
      result = text.compare(3, 6, TextT(STR("ABCDEFG")));
      CHECK(compares_agree(compare_result, result));
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_compare_positon_length_string_subposition_sublength)
    {
      Compare_Text compare_text(STR("xxxABCDEFyyy"));

      TextBuffer buffer;
      Text text(STR("xxxABCDEFyyy"), buffer.data(), buffer.size());

      int compare_result;
      int result;

      // Equal.
      compare_result = compare_text.compare(3, 6, Compare_Text(STR("aaABCDEFbb")), 2, 6);
      result = text.compare(3, 6, TextT(STR("aaABCDEFbb")), 2, 6);
      CHECK(compares_agree(compare_result, result));

      // Less.
      compare_result = compare_text.compare(3, 6, Compare_Text(STR("aaABCDEEbb")), 2, 6);
      result = text.compare(3, 6, TextT(STR("aaABCDEEbb")), 2, 6);
      CHECK(compares_agree(compare_result, result));

      // Greater.
      compare_result = compare_text.compare(3, 6, Compare_Text(STR("aaABCDEGbb")), 2, 6);
      result = text.compare(3, 6, TextT(STR("aaABCDEGbb")), 2, 6);
      CHECK(compares_agree(compare_result, result));

      // Shorter.
      compare_result = compare_text.compare(3, 6, Compare_Text(STR("aaABCDEbb")), 2, 5);
      result = text.compare(3, 6, TextT(STR("aaABCDEbb")), 2, 5);
      CHECK(compares_agree(compare_result, result));

      // Longer.
      compare_result = compare_text.compare(3, 6, Compare_Text(STR("aaABCDEFGbb")), 2, 7);
      result = text.compare(3, 6, TextT(STR("aaABCDEFGbb")), 2, 7);
      CHECK(compares_agree(compare_result, result));
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_compare_c_string)
    {
      Compare_Text compare_text(STR("ABCDEF"));

      TextBuffer buffer;
      Text text(STR("ABCDEF"), buffer.data(), buffer.size());

      int compare_result;
      int result;

      // Equal.
      compare_result = compare_text.compare(STR("ABCDEF"));
      result = text.compare(STR("ABCDEF"));
      CHECK(compares_agree(compare_result, result));

      // Less.
      compare_result = compare_text.compare(STR("ABCDEE"));
      result = text.compare(STR("ABCDEE"));
      CHECK(compares_agree(compare_result, result));

      // Greater.
      compare_result = compare_text.compare(STR("ABCDEG"));
      result = text.compare(STR("ABCDEG"));
      CHECK(compares_agree(compare_result, result));

      // Shorter.
      compare_result = compare_text.compare(STR("ABCDE"));
      result = text.compare(STR("ABCDE"));
      CHECK(compares_agree(compare_result, result));

      // Longer.
      compare_result = compare_text.compare(STR("ABCDEFG"));
      result = text.compare(STR("ABCDEFG"));
      CHECK(compares_agree(compare_result, result));
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_compare_positon_length_c_string)
    {
      Compare_Text compare_text(STR("xxxABCDEFyyy"));

      TextBuffer buffer;
      Text text(STR("xxxABCDEFyyy"), buffer.data(), buffer.size());

      int compare_result;
      int result;

      // Equal.
      compare_result = compare_text.compare(3, 6, STR("ABCDEF"));
      result = text.compare(3, 6, STR("ABCDEF"));
      CHECK(compares_agree(compare_result, result));

      // Less.
      compare_result = compare_text.compare(3, 6, STR("ABCDEE"));
      result = text.compare(3, 6, STR("ABCDEE"));
      CHECK(compares_agree(compare_result, result));

      // Greater.
      compare_result = compare_text.compare(3, 6, STR("ABCDEG"));
      result = text.compare(3, 6, STR("ABCDEG"));
      CHECK(compares_agree(compare_result, result));

      // Shorter.
      compare_result = compare_text.compare(3, 6, STR("ABCDE"));
      result = text.compare(3, 6, STR("ABCDE"));
      CHECK(compares_agree(compare_result, result));

      // Longer.
      compare_result = compare_text.compare(3, 6, STR("ABCDEFG"));
      result = text.compare(3, 6, STR("ABCDEFG"));
      CHECK(compares_agree(compare_result, result));
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_compare_positon_length_c_string_n)
    {
      Compare_Text compare_text(STR("xxxABCDEFyyy"));

      TextBuffer buffer;
      Text text(STR("xxxABCDEFyyy"), buffer.data(), buffer.size());

      int compare_result;
      int result;

      // Equal.
      compare_result = compare_text.compare(3, 6, STR("ABCDEFbb"), 6);
      result = text.compare(3, 6, STR("ABCDEFbb"), 6);
      CHECK(compares_agree(compare_result, result));

      // Less.
      compare_result = compare_text.compare(3, 6, STR("ABCDEEbb"), 6);
      result = text.compare(3, 6, STR("ABCDEEbb"), 6);
      CHECK(compares_agree(compare_result, result));

      // Greater.
      compare_result = compare_text.compare(3, 6, STR("ABCDEGbb"), 6);
      result = text.compare(3, 6, STR("ABCDEGbb"), 6);
      CHECK(compares_agree(compare_result, result));

      // Shorter.
      compare_result = compare_text.compare(3, 6, STR("ABCDEbb"), 5);
      result = text.compare(3, 6, STR("ABCDEbb"), 5);
      CHECK(compares_agree(compare_result, result));

      // Longer.
      compare_result = compare_text.compare(3, 6, STR("ABCDEFGbb"), 7);
      result = text.compare(3, 6, STR("ABCDEFGbb"), 7);
      CHECK(compares_agree(compare_result, result));
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_find_first_of_string_position)
    {
      Compare_Text compare_text(STR("ABCDEF"));

      TextBuffer buffer;
      Text text(STR("ABCDEF"), buffer.data(), buffer.size());

      size_t position1 = compare_text.find_first_of(Compare_Text(STR("ZCXF")));
      size_t position2 = text.find_first_of(TextT(STR("ZCXF")));

      CHECK_EQUAL(position1, position2);

      position1 = compare_text.find_first_of(Compare_Text(STR("WXYZ")));
      position2 = text.find_first_of(TextT(STR("WXYZ")));

      CHECK_EQUAL(position1, position2);

      position1 = compare_text.find_first_of(Compare_Text(STR("ZCXF")), 3);
      position2 = text.find_first_of(TextT(STR("ZCXF")), 3);

      CHECK_EQUAL(position1, position2);

      position1 = compare_text.find_first_of(Compare_Text(STR("ZCXF")), 100);
      position2 = text.find_first_of(TextT(STR("ZCXF")), 100);

      CHECK_EQUAL(position1, position2);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_find_first_of_pointer_position)
    {
      Compare_Text compare_text(STR("ABCDEF"));

      TextBuffer buffer;
      Text text(STR("ABCDEF"), buffer.data(), buffer.size());

      size_t position1 = compare_text.find_first_of(STR("ZCXF"));
      size_t position2 = text.find_first_of(STR("ZCXF"));

      CHECK_EQUAL(position1, position2);

      position1 = compare_text.find_first_of(STR("WXYZ"));
      position2 = text.find_first_of(STR("WXYZ"));

      CHECK_EQUAL(position1, position2);

      position1 = compare_text.find_first_of(STR("ZCXF"), 3);
      position2 = text.find_first_of(STR("ZCXF"), 3);

      CHECK_EQUAL(position1, position2);

      position1 = compare_text.find_first_of(STR("ZCXF"), 100);
      position2 = text.find_first_of(STR("ZCXF"), 100);

      CHECK_EQUAL(position1, position2);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_find_first_of_pointer_position_n)
    {
      Compare_Text compare_text(STR("ABCDEF"));

      TextBuffer buffer;
      Text text(STR("ABCDEF"), buffer.data(), buffer.size());

      size_t position1 = compare_text.find_first_of(STR("ZCXF"), 0, 4);
      size_t position2 = text.find_first_of(STR("ZCXF"), 0, 4);

      CHECK_EQUAL(position1, position2);

      position1 = compare_text.find_first_of(STR("WXYZ"), 0, 4);
      position2 = text.find_first_of(STR("WXYZ"), 0, 4);

      CHECK_EQUAL(position1, position2);

      position1 = compare_text.find_first_of(STR("ZCXF"), 1, 3);
      position2 = text.find_first_of(STR("ZCXF"), 1, 3);

      CHECK_EQUAL(position1, position2);

      position1 = compare_text.find_first_of(STR("ZCXF"), 3, 3);
      position2 = text.find_first_of(STR("ZCXF"), 3, 3);

      CHECK_EQUAL(position1, position2);

      position1 = compare_text.find_first_of(STR("ZCXF"), 100);
      position2 = text.find_first_of(STR("ZCXF"), 100);

      CHECK_EQUAL(position1, position2);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_find_first_of_character_position)
    {
      Compare_Text compare_text(STR("ABCDEF"));

      TextBuffer buffer;
      Text text(STR("ABCDEF"), buffer.data(), buffer.size());

      size_t position1 = compare_text.find_first_of(STR('C'));
      size_t position2 = text.find_first_of(STR('C'));

      CHECK_EQUAL(position1, position2);

      position1 = compare_text.find_first_of(STR('Z'));
      position2 = text.find_first_of(STR('Z'));

      CHECK_EQUAL(position1, position2);

      position1 = compare_text.find_first_of(STR('F'), 3);
      position2 = text.find_first_of(STR('F'), 3);

      CHECK_EQUAL(position1, position2);

      position1 = compare_text.find_first_of(STR('C'), 3);
      position2 = text.find_first_of(STR('C'), 3);

      CHECK_EQUAL(position1, position2);

      position1 = compare_text.find_first_of(STR('C'), compare_text.size());
      position2 = text.find_first_of(STR('C'), text.size());

      CHECK_EQUAL(position1, position2);

      position1 = compare_text.find_first_of(STR('C'), 100);
      position2 = text.find_first_of(STR('C'), 100);

      CHECK_EQUAL(position1, position2);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_find_last_of_string_position)
    {
      Compare_Text compare_text(STR("ABCDEFABCDE"));

      TextBuffer buffer;
      Text text(STR("ABCDEFABCDE"), buffer.data(), buffer.size());

      size_t position1 = compare_text.find_last_of(Compare_Text(STR("ZCXE")));
      size_t position2 = text.find_last_of(TextT(STR("ZCXE")));

      CHECK_EQUAL(position1, position2);

      position1 = compare_text.find_last_of(Compare_Text(STR("WXYZ")), 3);
      position2 = text.find_last_of(TextT(STR("WXYZ")), 3);

      CHECK_EQUAL(position1, position2);

      position1 = compare_text.find_last_of(Compare_Text(STR("ZCXE")), 5);
      position2 = text.find_last_of(TextT(STR("ZCXE")), 5);

      CHECK_EQUAL(position1, position2);

      position1 = compare_text.find_last_of(Compare_Text(STR("ZCXE")), compare_text.size());
      position2 = text.find_last_of(TextT(STR("ZCXE")), text.size());

      CHECK_EQUAL(position1, position2);

      position1 = compare_text.find_last_of(Compare_Text(STR("ZCXE")), 100);
      position2 = text.find_last_of(TextT(STR("ZCXE")), 100);

      CHECK_EQUAL(position1, position2);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_find_last_of_pointer_position)
    {
      Compare_Text compare_text(STR("ABCDEFABCDE"));

      TextBuffer buffer;
      Text text(STR("ABCDEFABCDE"), buffer.data(), buffer.size());

      size_t position1 = compare_text.find_last_of(STR("ZCXE"));
      size_t position2 = text.find_last_of(STR("ZCXE"));

      CHECK_EQUAL(position1, position2);

      position1 = compare_text.find_last_of(STR("WXYZ"), 3);
      position2 = text.find_last_of(STR("WXYZ"), 3);

      CHECK_EQUAL(position1, position2);

      position1 = compare_text.find_last_of(STR("ZCXE"), 5);
      position2 = text.find_last_of(STR("ZCXE"), 5);

      CHECK_EQUAL(position1, position2);

      position1 = compare_text.find_last_of(STR("ZCXE"), 6);
      position2 = text.find_last_of(STR("ZCXE"), 6);

      CHECK_EQUAL(position1, position2);

      position1 = compare_text.find_last_of(STR("ZCXE"), compare_text.size());
      position2 = text.find_last_of(STR("ZCXE"), text.size());

      CHECK_EQUAL(position1, position2);

      position1 = compare_text.find_last_of(STR("ZCXE"), 100);
      position2 = text.find_last_of(STR("ZCXE"), 100);

      CHECK_EQUAL(position1, position2);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_find_last_of_pointer_position_n)
    {
      Compare_Text compare_text(STR("ABCDEFABCDE"));

      TextBuffer buffer;
      Text text(STR("ABCDEFABCDE"), buffer.data(), buffer.size());

      size_t position1 = compare_text.find_last_of(STR("AZCXE"), 0, 4);
      size_t position2 = text.find_last_of(STR("AZCXE"), 0, 4);

      CHECK_EQUAL(position1, position2);

      position1 = compare_text.find_last_of(STR("WXYZ"), 4, 3);
      position2 = text.find_last_of(STR("WXYZ"), 4, 3);

      CHECK_EQUAL(position1, position2);

      position1 = compare_text.find_last_of(STR("ZCXE"), 5, 3);
      position2 = text.find_last_of(STR("ZCXE"), 5, 3);

      CHECK_EQUAL(position1, position2);

      position1 = compare_text.find_last_of(STR("ZCXE"), 1, 3);
      position2 = text.find_last_of(STR("ZCXE"), 1, 3);

      CHECK_EQUAL(position1, position2);

      position1 = compare_text.find_last_of(STR("ZCXE"), compare_text.size(), 4);
      position2 = text.find_last_of(STR("ZCXE"), text.size(), 4);

      CHECK_EQUAL(position1, position2);

      position1 = compare_text.find_last_of(STR("ZCXE"), 100, 4);
      position2 = text.find_last_of(STR("ZCXE"), 100, 4);

      CHECK_EQUAL(position1, position2);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_find_last_of_character_position)
    {
      Compare_Text compare_text(STR("ABCDEF"));

      TextBuffer buffer;
      Text text(STR("ABCDEF"), buffer.data(), buffer.size());

      size_t position1 = compare_text.find_last_of(STR('C'));
      size_t position2 = text.find_last_of(STR('C'));

      CHECK_EQUAL(position1, position2);

      position1 = compare_text.find_last_of(STR('Z'));
      position2 = text.find_last_of(STR('Z'));

      CHECK_EQUAL(position1, position2);

      position1 = compare_text.find_last_of(STR('F'), compare_text.size());
      position2 = text.find_last_of(STR('F'), text.size());

      CHECK_EQUAL(position1, position2);

      position1 = compare_text.find_last_of(STR('C'), 3);
      position2 = text.find_last_of(STR('C'), 3);

      CHECK_EQUAL(position1, position2);

      position1 = compare_text.find_last_of(STR('C'), compare_text.size());
      position2 = text.find_last_of(STR('C'), text.size());

      CHECK_EQUAL(position1, position2);

      position1 = compare_text.find_last_of(STR('C'), 100);
      position2 = text.find_last_of(STR('C'), 100);

      CHECK_EQUAL(position1, position2);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_find_first_not_of_string_position)
    {
      Compare_Text compare_text(STR("ABCDEF"));

      TextBuffer buffer;
      Text text(STR("ABCDEF"), buffer.data(), buffer.size());

      size_t position1 = compare_text.find_first_not_of(Compare_Text(STR("ZAXB")));
      size_t position2 = text.find_first_not_of(TextT(STR("ZAXB")));

      CHECK_EQUAL(position1, position2);

      position1 = compare_text.find_first_not_of(Compare_Text(STR("ZAXB")));
      position2 = text.find_first_not_of(TextT(STR("ZAXB")));

      CHECK_EQUAL(position1, position2);

      position1 = compare_text.find_first_not_of(Compare_Text(STR("ZAXB")), 3);
      position2 = text.find_first_not_of(TextT(STR("ZAXB")), 3);

      CHECK_EQUAL(position1, position2);

      position1 = compare_text.find_first_not_of(Compare_Text(STR("ZAXB")), compare_text.size());
      position2 = text.find_first_not_of(TextT(STR("ZAXB")), text.size());

      CHECK_EQUAL(position1, position2);

      position1 = compare_text.find_first_not_of(Compare_Text(STR("ZAXB")), 100);
      position2 = text.find_first_not_of(TextT(STR("ZAXB")), 100);

      CHECK_EQUAL(position1, position2);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_find_first_not_of_pointer_position)
    {
      Compare_Text compare_text(STR("ABCDEF"));

      TextBuffer buffer;
      Text text(STR("ABCDEF"), buffer.data(), buffer.size());

      size_t position1 = compare_text.find_first_not_of(STR("ZAXB"));
      size_t position2 = text.find_first_not_of(STR("ZAXB"));

      CHECK_EQUAL(position1, position2);

      position1 = compare_text.find_first_not_of(STR("ZAXB"));
      position2 = text.find_first_not_of(STR("ZAXB"));

      CHECK_EQUAL(position1, position2);

      position1 = compare_text.find_first_not_of(STR("ZAXB"), 3);
      position2 = text.find_first_not_of(STR("ZAXB"), 3);

      CHECK_EQUAL(position1, position2);

      position1 = compare_text.find_first_not_of(STR("ZAXB"), compare_text.size());
      position2 = text.find_first_not_of(STR("ZAXB"), text.size());

      CHECK_EQUAL(position1, position2);

      position1 = compare_text.find_first_not_of(STR("ZAXB"), 100);
      position2 = text.find_first_not_of(STR("ZAXB"), 100);

      CHECK_EQUAL(position1, position2);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_find_first_not_of_pointer_position_n)
    {
      Compare_Text compare_text(STR("ABCDEF"));

      TextBuffer buffer;
      Text text(STR("ABCDEF"), buffer.data(), buffer.size());

      size_t position1 = compare_text.find_first_not_of(STR("ZAXB"), 0, 4);
      size_t position2 = text.find_first_not_of(STR("ZAXB"), 0, 4);

      CHECK_EQUAL(position1, position2);

      position1 = compare_text.find_first_not_of(STR("ZAXB"), 0, 4);
      position2 = text.find_first_not_of(STR("ZAXB"), 0, 4);

      CHECK_EQUAL(position1, position2);

      position1 = compare_text.find_first_not_of(STR("ZAXB"), 1, 3);
      position2 = text.find_first_not_of(STR("ZAXB"), 1, 3);

      CHECK_EQUAL(position1, position2);

      position1 = compare_text.find_first_not_of(STR("ZAXB"), 3, 3);
      position2 = text.find_first_not_of(STR("ZAXB"), 3, 3);

      CHECK_EQUAL(position1, position2);

      position1 = compare_text.find_first_not_of(STR("ZAXB"), compare_text.size(), 3);
      position2 = text.find_first_not_of(STR("ZAXB"), text.size(), 3);

      CHECK_EQUAL(position1, position2);

      position1 = compare_text.find_first_not_of(STR("ZAXB"), 100);
      position2 = text.find_first_not_of(STR("ZAXB"), 100);

      CHECK_EQUAL(position1, position2);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_find_first_not_of_character_position)
    {
      Compare_Text compare_text(STR("ABCDEF"));

      TextBuffer buffer;
      Text text(STR("ABCDEF"), buffer.data(), buffer.size());

      size_t position1 = compare_text.find_first_not_of(STR('A'));
      size_t position2 = text.find_first_not_of(STR('A'));

      CHECK_EQUAL(position1, position2);

      position1 = compare_text.find_first_not_of(STR('B'));
      position2 = text.find_first_not_of(STR('B'));

      CHECK_EQUAL(position1, position2);

      position1 = compare_text.find_first_not_of(STR('C'), 3);
      position2 = text.find_first_not_of(STR('C'), 3);

      CHECK_EQUAL(position1, position2);

      position1 = compare_text.find_first_not_of(STR('D'), 3);
      position2 = text.find_first_not_of(STR('D'), 3);

      CHECK_EQUAL(position1, position2);

      position1 = compare_text.find_first_not_of(STR('C'), compare_text.size());
      position2 = text.find_first_not_of(STR('C'), text.size());

      CHECK_EQUAL(position1, position2);

      position1 = compare_text.find_first_not_of(STR('C'), 100);
      position2 = text.find_first_not_of(STR('C'), 100);

      CHECK_EQUAL(position1, position2);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_find_last_not_of_string_position)
    {
      Compare_Text compare_text(STR("ABCDEFABCDE"));

      TextBuffer buffer;
      Text text(STR("ABCDEFABCDE"), buffer.data(), buffer.size());

      size_t position1 = compare_text.find_last_not_of(Compare_Text(STR("ZEXD")));
      size_t position2 = text.find_last_not_of(TextT(STR("ZEXD")));

      CHECK_EQUAL(position1, position2);

      position1 = compare_text.find_last_not_of(Compare_Text(STR("ZEXD")), 3);
      position2 = text.find_last_not_of(TextT(STR("ZEXD")), 3);

      CHECK_EQUAL(position1, position2);

      position1 = compare_text.find_last_not_of(Compare_Text(STR("ZEXD")), 5);
      position2 = text.find_last_not_of(TextT(STR("ZEXD")), 5);

      CHECK_EQUAL(position1, position2);

      position1 = compare_text.find_last_not_of(Compare_Text(STR("ZEXD")), compare_text.size());
      position2 = text.find_last_not_of(TextT(STR("ZEXD")), text.size());

      CHECK_EQUAL(position1, position2);

      position1 = compare_text.find_last_not_of(Compare_Text(STR("ZEXD")), 100);
      position2 = text.find_last_not_of(TextT(STR("ZEXD")), 100);

      CHECK_EQUAL(position1, position2);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_find_last_not_of_pointer_position)
    {
      Compare_Text compare_text(STR("ABCDEFABCDE"));

      TextBuffer buffer;
      Text text(STR("ABCDEFABCDE"), buffer.data(), buffer.size());

      size_t position1 = compare_text.find_last_not_of(STR("ZEXD"));
      size_t position2 = text.find_last_not_of(STR("ZEXD"));

      CHECK_EQUAL(position1, position2);

      position1 = compare_text.find_last_not_of(STR("ZEXD"), 3);
      position2 = text.find_last_not_of(STR("ZEXD"), 3);

      CHECK_EQUAL(position1, position2);

      position1 = compare_text.find_last_not_of(STR("ZEXD"), 5);
      position2 = text.find_last_not_of(STR("ZEXD"), 5);

      CHECK_EQUAL(position1, position2);

      position1 = compare_text.find_last_not_of(STR("ZEXD"), compare_text.size());
      position2 = text.find_last_not_of(STR("ZEXD"), text.size());

      CHECK_EQUAL(position1, position2);

      position1 = compare_text.find_last_not_of(STR("ZEXD"), 100);
      position2 = text.find_last_not_of(STR("ZEXD"), 100);

      CHECK_EQUAL(position1, position2);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_find_last_not_of_pointer_position_n)
    {
      Compare_Text compare_text(STR("ABCDEFABCDE"));

      TextBuffer buffer;
      Text text(STR("ABCDEFABCDE"), buffer.data(), buffer.size());

      size_t position1 = compare_text.find_last_not_of(STR("ZEXD"), 0, 4);
      size_t position2 = text.find_last_not_of(STR("ZEXD"), 0, 4);

      CHECK_EQUAL(position1, position2);

      position1 = compare_text.find_last_not_of(STR("ZEXD"), 5, 3);
      position2 = text.find_last_not_of(STR("ZEXD"), 5, 3);

      CHECK_EQUAL(position1, position2);

      position1 = compare_text.find_last_not_of(STR("ZEXD"), 1, 3);
      position2 = text.find_last_not_of(STR("ZEXD"), 1, 3);

      CHECK_EQUAL(position1, position2);

      position1 = compare_text.find_last_not_of(STR("ZEXD"), compare_text.size(), 4);
      position2 = text.find_last_not_of(STR("ZEXD"), text.size(), 4);

      CHECK_EQUAL(position1, position2);

      position1 = compare_text.find_last_not_of(STR("ZEXD"), 100, 4);
      position2 = text.find_last_not_of(STR("ZEXD"), 100, 4);

      CHECK_EQUAL(position1, position2);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_find_last_not_of_character_position)
    {
      Compare_Text compare_text(STR("ABCDEF"));

      TextBuffer buffer;
      Text text(STR("ABCDEF"), buffer.data(), buffer.size());

      size_t position1 = compare_text.find_last_not_of(STR('F'));
      size_t position2 = text.find_last_not_of(STR('F'));

      CHECK_EQUAL(position1, position2);

      position1 = compare_text.find_last_not_of(STR('Z'));
      position2 = text.find_last_not_of(STR('Z'));

      CHECK_EQUAL(position1, position2);

      position1 = compare_text.find_last_not_of(STR('A'), compare_text.size());
      position2 = text.find_last_not_of(STR('A'), text.size());

      CHECK_EQUAL(position1, position2);

      position1 = compare_text.find_last_not_of(STR('C'), 3);
      position2 = text.find_last_not_of(STR('C'), 3);

      CHECK_EQUAL(position1, position2);

      position1 = compare_text.find_last_not_of(STR('C'), compare_text.size());
      position2 = text.find_last_not_of(STR('C'), text.size());

      CHECK_EQUAL(position1, position2);

      position1 = compare_text.find_last_not_of(STR('C'), 100);
      position2 = text.find_last_not_of(STR('C'), 100);

      CHECK_EQUAL(position1, position2);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_hash)
    {
      // Test with actual string type.
      TextBuffer buffer;
      Text text(STR("ABCDEFHIJKL"), buffer.data(), buffer.size());
      size_t hash = etl::hash<Text>()(text);
      size_t compare_hash = etl::private_hash::generic_hash<size_t>(reinterpret_cast<const uint8_t*>(&text[0]), reinterpret_cast<const uint8_t*>(&text[text.size()]));
      CHECK_EQUAL(compare_hash, hash);

      // Test with interface string type.
      IText& itext = text;
      hash = etl::hash<IText>()(itext);
      CHECK_EQUAL(compare_hash, hash);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_memcpy_repair)
    {
      TextBuffer buffer;
      Text text(buffer.data(), buffer.size());

      text.assign(STR("ABCDEF"));

      char buffer2[sizeof(Text)];

      // Will not copy the buffer!
      memcpy(&buffer2, (const void*)&text, sizeof(text));

      Text& rtext(*reinterpret_cast<Text*>(buffer2));
      rtext.repair();

      CHECK(!rtext.empty());
      CHECK(!rtext.full());

      bool is_equal = Equal(text, rtext);
      CHECK(is_equal);

      text = STR("GHIJKL");
      is_equal = Equal(text, rtext);
      CHECK(is_equal);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_memcpy_repair_virtual)
    {
      TextBuffer buffer;
      Text text(buffer.data(), buffer.size());

      text.assign(STR("ABCDEF"));

      char buffer2[sizeof(Text)];

      // Will not copy the buffer!
      memcpy(&buffer2, (const void*)&text, sizeof(text));

      IText& itext(*reinterpret_cast<IText*>(buffer2));
      itext.repair();

      CHECK(!itext.empty());
      CHECK(!itext.full());

      bool is_equal = Equal(text, itext);
      CHECK(is_equal);

      text = STR("GHIJKL");
      is_equal = Equal(text, itext);
      CHECK(is_equal);
    }

#if ETL_STRING_TRUNCATION_CHECKS_ENABLED
    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_truncate_over_many_operations)
    {
      TextBuffer buffer;
      Text text(short_text.c_str(), buffer.data(), buffer.size());
      CHECK(!text.is_truncated());

      text.insert(3, initial_text.c_str());
      CHECK(text.is_truncated());

      while (text.size() != 0)
      {
        text.pop_back();
        CHECK(text.is_truncated());
      }

      text.clear();
      CHECK(!text.is_truncated());

      text.assign(longer_text.c_str());
      CHECK(text.is_truncated());

      text.assign(short_text.c_str());
      CHECK(!text.is_truncated());
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_add_from_truncated)
    {
      TextBuffer buffer;
      Text  text1(short_text.c_str(), buffer.data(), buffer.size());

      TextBufferS buffers;
      Text text2(short_text.c_str(), buffers.data(), buffers.size());

      CHECK(!text1.is_truncated());
      CHECK(text2.is_truncated());

      // text2 has the truncate flag set.
      text1 += text2;

      CHECK(text1.is_truncated());
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_add_to_truncated)
    {
      TextBuffer buffer;
      Text text1(longer_text.c_str(), buffer.data(), buffer.size());

      TextBuffer buffer2;
      Text text2(short_text.c_str(), buffer2.data(), buffer2.size());

      CHECK(text1.is_truncated());
      CHECK(!text2.is_truncated());

      // Clear text but not the truncate flag.
      text1.erase(text1.begin(), text1.end());

      // text1 still has the truncate flag set.
      text1 += text2;

      CHECK(text1.is_truncated());
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_clear_truncated)
    {
      TextBuffer buffer;
      Text text(longer_text.c_str(), buffer.data(), buffer.size());
      CHECK(text.is_truncated());

      text.clear_truncated();
      CHECK(!text.is_truncated());
    }
#endif

#if ETL_STRING_CLEAR_AFTER_USE_ENABLED
    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_secure_after_destructor)
    {
      char buffer[sizeof(Text)];
      std::fill_n(buffer, sizeof(Text), 0);

      TextBuffer buffer2;
      ::new (buffer) Text(STR("ABCDEF"), buffer2.data(), buffer2.size());

      Text& text = *reinterpret_cast<Text*>(buffer);
      text.set_secure();

      CHECK(TextT(STR("ABCDEF")) == text);

      Text::pointer pb = text.begin();
      Text::pointer pe = text.end();

      // Destroy the text object.
      text.~Text();

      // Check there no non-zero values in the string.
      CHECK(std::find_if(pb, pe, [](Text::value_type x) { return x != 0; }) == pe);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_secure_after_assign)
    {
      TextBuffer buffer;
      Text text(buffer.data(), buffer.size());
      text.set_secure();
      text.assign(STR("ABCDEF"));

      Text::pointer pe = text.end();

      text.assign(STR("ABC"));

      CHECK(std::find_if(text.end(), pe, [](Text::value_type x) { return x != 0; }) == pe);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_secure_after_resize_down)
    {
      TextBuffer buffer;
      Text text(buffer.data(), buffer.size());
      text.set_secure();
      text.assign(STR("ABCDEF"));

      Text::pointer pe = text.end();

      text.resize(text.size() - 3U);

      CHECK(std::find_if(text.end(), pe, [](Text::value_type x) { return x != 0; }) == pe);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_secure_after_erase)
    {
      TextBuffer buffer;
      Text text(buffer.data(), buffer.size());
      text.set_secure();
      text.assign(STR("ABCDEF"));

      Text::pointer pb = text.begin();
      Text::pointer pe = text.end();

      text.erase(pb + 2, pb + 5);

      // Check there no non-zero values in the remainder of the string.
      CHECK(std::find_if(text.end(), pe, [](Text::value_type x) { return x != 0; }) == pe);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_secure_after_replace)
    {
      TextBuffer buffer;
      Text text(buffer.data(), buffer.size());
      text.set_secure();
      text.assign(STR("ABCDEF"));

      Text::pointer pb = text.begin();
      Text::pointer pe = text.end();

      text.replace(pb + 1, pb + 4, STR("G"));

      // Check there no non-zero values in the remainder of the string.
      CHECK(std::find_if(text.end(), pe, [](Text::value_type x) { return x != 0; }) == pe);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_secure_after_clear)
    {
      TextBuffer buffer;
      Text text(buffer.data(), buffer.size());
      text.set_secure();
      text.assign(STR("ABCDEF"));

      Text::pointer pb = text.begin();
      Text::pointer pe = text.end();

      text.clear();

      // Check there no non-zero values in the remainder of the string.
      CHECK(std::find_if(pb, pe, [](Text::value_type x) { return x != 0; }) == pe);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_secure_flag_after_copy)
    {
      TextBuffer buffer1;
      Text text1(STR("Hello World"), buffer1.data(), buffer1.size());
      text1.set_secure();

      TextBuffer buffer2;
      Text text2(text1, buffer2.data(), buffer2.size());

      TextBuffer buffer3;
      Text text3(buffer3.data(), buffer3.size());
      text3 = text1;

      TextBuffer buffer4;
      Text text4(text1, buffer4.data(), buffer4.size(), 6U, 2U);

      CHECK(text2.is_secure());
      CHECK(text3.is_secure());
      CHECK(text4.is_secure());
    }
#endif
  };
}
