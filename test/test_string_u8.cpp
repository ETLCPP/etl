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

#include <string>
#include <array>
#include <algorithm>
#include <atomic>

#include "etl/u8string.h"

#undef STR
#define STR(x) u8##x

namespace
{
  bool compares_agree(int result1, int result2)
  {
    return ((result1 < 0)  && (result2 < 0)) ||
           ((result1 == 0) && (result2 == 0)) ||
           ((result1 > 0)  && (result2 > 0));
  }

  //***********************************
  //std::ostream& operator << (std::ostream& os, const etl::iu8string::value_type& c)
  //{
  //  os << uint16_t(c);

  //  return os;
  //}

  //***********************************
  //std::ostream& operator << (std::ostream& os, const etl::iu8string::value_type* c)
  //{
  //  os << (void*)c;

  //  return os;
  //}

  SUITE(test_string_char8_t)
  {
    static const size_t SIZE = 11;

    using Text    = etl::u8string<SIZE>;
    using IText   = etl::iu8string;
    using TextSTD = std::u8string;
    using value_t = Text::value_type;
    using TextL   = etl::u8string<52>;
    using TextS   = etl::u8string<4>;
    using View = etl::u8string_view;

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
    TEST_FIXTURE(SetupFixture, test_default_constructor)
    {
      Text text;

      CHECK_EQUAL(text.size(), size_t(0));
      CHECK(text.empty());
      CHECK_EQUAL(text.capacity(), SIZE);
      CHECK_EQUAL(text.max_size(), SIZE);
      CHECK(text.begin() == text.end());
      CHECK_FALSE(text.is_truncated());
    }

    //*************************************************************************
    TEST(test_iterator_comparison_empty)
    {
      Text text;

      CHECK(text.begin()   == text.end());
      CHECK(text.cbegin()  == text.cend());
      CHECK(text.rbegin()  == text.rend());
      CHECK(text.crbegin() == text.crend());
      CHECK_FALSE(text.is_truncated());
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_constructor_size_value)
    {
      const size_t INITIAL_SIZE = 5UL;
      const value_t   INITIAL_VALUE = STR('A');

      TextSTD compare_text(INITIAL_SIZE, INITIAL_VALUE);
      Text text(INITIAL_SIZE, INITIAL_VALUE);

      CHECK(text.size() == INITIAL_SIZE);
      CHECK(!text.empty());

      bool is_equal = Equal(compare_text, text);

      CHECK(is_equal);
      CHECK_FALSE(text.is_truncated());
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_constructor_size_excess)
    {
      Text text(SIZE + 1, STR('A'));

      CHECK_EQUAL(SIZE, text.size());
#if ETL_HAS_STRING_TRUNCATION_CHECKS
      CHECK_TRUE(text.is_truncated());
#else
      CHECK_FALSE(text.is_truncated());
#endif
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_constructor_char_pointer)
    {
      TextSTD compare_text(initial_text.c_str());

      Text text(initial_text.c_str());

      CHECK(!text.empty());

      bool is_equal = Equal(compare_text, text);
      CHECK(is_equal);
      CHECK_FALSE(text.is_truncated());
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_constructor_char_pointer_excess)
    {
      TextSTD compare_text(initial_text.c_str());

      Text text(longer_text.c_str());

      CHECK(!text.empty());

      bool is_equal = Equal(compare_text, text);
      CHECK(is_equal);
#if ETL_HAS_STRING_TRUNCATION_CHECKS
      CHECK_TRUE(text.is_truncated());
#else
      CHECK_FALSE(text.is_truncated());
#endif
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_constructor_char_pointer_size)
    {
      TextSTD compare_text(SIZE, STR('A'));

      Text text(SIZE, STR('A'));

      CHECK(!text.empty());

      bool is_equal = Equal(compare_text, text);
      CHECK(is_equal);
      CHECK_FALSE(text.is_truncated());
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_constructor_char_pointer_size_excess)
    {
      TextSTD compare_text(SIZE, STR('A'));

      Text text(SIZE + 1, STR('A'));

      CHECK(!text.empty());

      bool is_equal = Equal(compare_text, text);
      CHECK(is_equal);
#if ETL_HAS_STRING_TRUNCATION_CHECKS
      CHECK_TRUE(text.is_truncated());
#else
      CHECK_FALSE(text.is_truncated());
#endif
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_constructor_size_char)
    {
      TextSTD compare_text(initial_text.c_str(), initial_text.size() / 2);

      Text text(initial_text.c_str(), initial_text.size() / 2);

      CHECK(!text.empty());

      bool is_equal = Equal(compare_text, text);
      CHECK(is_equal);
      CHECK_FALSE(text.is_truncated());
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_constructor_size_char_excess)
    {
      TextSTD compare_text(initial_text.c_str(), initial_text.size());

      Text text(longer_text.c_str(), longer_text.size());

      CHECK(!text.empty());

      bool is_equal = Equal(compare_text, text);
      CHECK(is_equal);
#if ETL_HAS_STRING_TRUNCATION_CHECKS
      CHECK_TRUE(text.is_truncated());
#else
      CHECK_FALSE(text.is_truncated());
#endif
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_constructor_range)
    {
      TextSTD compare_text(initial_text.begin(), initial_text.end());

      Text text(compare_text.begin(), compare_text.end());

      CHECK(text.size() == SIZE);
      CHECK(!text.empty());
      CHECK_FALSE(text.is_truncated());
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_constructor_range_excess)
    {
      Text text(longer_text.begin(), longer_text.end());

      bool is_equal = Equal(initial_text, text);
      CHECK(is_equal);
      CHECK(text.size() == SIZE);
      CHECK(!text.empty());
#if ETL_HAS_STRING_TRUNCATION_CHECKS
      CHECK_TRUE(text.is_truncated());
#else
      CHECK_FALSE(text.is_truncated());
#endif
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_constructor_from_literal)
    {
      Text text(STR("Hello World"));

      bool is_equal = Equal(initial_text, text);
      CHECK(is_equal);
      CHECK(text.size() == SIZE);
      CHECK(!text.empty());
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_constructor_from_etl_string_view)
    {
      View view(initial_text.data(), initial_text.size());
      Text text(view);

      bool is_equal = Equal(initial_text, text);
      CHECK(is_equal);
      CHECK(text.size() == SIZE);
      CHECK(!text.empty());
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_copy_constructor)
    {
      Text text(initial_text.c_str());
      Text text2(text);
      CHECK(text2 == text);
      CHECK_FALSE(text2.is_truncated());
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_copy_constructor_i)
    {
      Text text(initial_text.c_str());
      IText& itext = text;
      Text text2(itext);
      CHECK(text2 == text);
      CHECK_FALSE(text2.is_truncated());
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_copy_constructor_excess)
    {
      Text  text(initial_text.c_str());
      TextL textl(longer_text.c_str());
      Text  text2(textl);
      CHECK(text2 == text);
#if ETL_HAS_STRING_TRUNCATION_CHECKS
      CHECK_TRUE(text2.is_truncated());
#else
      CHECK_FALSE(text2.is_truncated());
#endif
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_copy_constructor_from_truncated)
    {
      Text  text(longer_text.c_str());
      Text  text2(text);
#if ETL_HAS_STRING_TRUNCATION_CHECKS
      CHECK_TRUE(text2.is_truncated());
#endif
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_construct_position_length)
    {
      TextSTD compare_text(initial_text.c_str());
      TextSTD compare_text2(compare_text, 2, 4);

      Text text(initial_text.c_str());
      Text text2(text, 2, 4);

      bool is_equal = Equal(compare_text2, text2);
      CHECK(is_equal);
      CHECK_FALSE(text2.is_truncated());
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_construct_position_length_excess)
    {
      TextSTD compare_text(longer_text.c_str());
      TextSTD compare_text2(compare_text, 2, 11);

      TextL textl(longer_text.c_str());
      Text text2(textl, 2, 12);

      bool is_equal = Equal(compare_text2, text2);
      CHECK(is_equal);
#if ETL_HAS_STRING_TRUNCATION_CHECKS
      CHECK_TRUE(text2.is_truncated());
#endif
    }

#if ETL_HAS_INITIALIZER_LIST
    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_construct_initializer_list)
    {
      TextSTD compare_text = { STR('H'), STR('e'), STR('l') , STR('l') , STR('o') };
      Text text = { STR('H'), STR('e'), STR('l') , STR('l') , STR('o') };

      bool is_equal = Equal(compare_text, text);
      CHECK(is_equal);
      CHECK_FALSE(text.is_truncated());
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_construct_initializer_list_excess)
    {
      TextSTD compare_text = { STR('H'), STR('e'), STR('l'), STR('l'), STR('o'),  STR(' '),
        STR('W'), STR('o'), STR('r'), STR('l'), STR('d') };
      Text text = { STR('H'), STR('e'), STR('l'), STR('l'), STR('o'),  STR(' '),
        STR('W'), STR('o'), STR('r'), STR('l'), STR('d'),  STR(' '),
        STR('T'), STR('h'), STR('e'), STR('r'), STR('e') };

      bool is_equal = Equal(compare_text, text);
      CHECK(is_equal);
#if ETL_HAS_STRING_TRUNCATION_CHECKS
      CHECK_TRUE(text.is_truncated());
#else
      CHECK_FALSE(text.is_truncated());
#endif
    }
#endif

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_assignment)
    {
      Text text(initial_text.begin(), initial_text.end());
      Text other_text;

      other_text = text;

      bool is_equal = Equal(text, other_text);
      CHECK(is_equal);
      CHECK_FALSE(text.is_truncated());
      CHECK_FALSE(other_text.is_truncated());
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_assignment_excess)
    {
      Text text(longer_text.begin(), longer_text.end());
      Text other_text;

      other_text = text;

      bool is_equal = Equal(text, other_text);
      CHECK(is_equal);
#if ETL_HAS_STRING_TRUNCATION_CHECKS
      CHECK_TRUE(text.is_truncated());
      CHECK_TRUE(other_text.is_truncated());
#else
      CHECK_FALSE(text.is_truncated());
      CHECK_FALSE(other_text.is_truncated());
#endif
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_assignment_iterface)
    {
      Text text1(initial_text.begin(), initial_text.end());
      Text text2;

      IText& itext1 = text1;
      IText& itext2 = text2;

      itext2 = itext1;

      bool is_equal = Equal(text1, text2);

      CHECK(is_equal);
      CHECK_FALSE(text1.is_truncated());
      CHECK_FALSE(text2.is_truncated());
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_assignment_iterface_excess)
    {
      Text text1(longer_text.begin(), longer_text.end());
      Text text2;

      IText& itext1 = text1;
      IText& itext2 = text2;

      itext2 = itext1;

      bool is_equal = Equal(text1, text2);

      CHECK(is_equal);
#if ETL_HAS_STRING_TRUNCATION_CHECKS
      CHECK_TRUE(text1.is_truncated());
      CHECK_TRUE(text2.is_truncated());
#else
      CHECK_FALSE(text1.is_truncated());
      CHECK_FALSE(text2.is_truncated());
#endif
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_self_assignment)
    {
      Text text(initial_text.begin(), initial_text.end());
      Text other_text(text);

#include "etl/private/diagnostic_self_assign_overloaded_push.h" 
      other_text = other_text;
#include "etl/private/diagnostic_pop.h" 

      bool is_equal = Equal(text, other_text);

      CHECK(is_equal);
      CHECK_FALSE(text.is_truncated());
      CHECK_FALSE(other_text.is_truncated());
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_self_assignment_excess)
    {
      Text text(longer_text.begin(), longer_text.end());
      Text other_text(text);

#include "etl/private/diagnostic_self_assign_overloaded_push.h" 
      other_text = other_text;
#include "etl/private/diagnostic_pop.h" 

      bool is_equal = Equal(text, other_text);

      CHECK(is_equal);
#if ETL_HAS_STRING_TRUNCATION_CHECKS
      CHECK_TRUE(text.is_truncated());
      CHECK_TRUE(other_text.is_truncated());
#else
      CHECK_FALSE(text.is_truncated());
      CHECK_FALSE(other_text.is_truncated());
#endif
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_assignment_from_literal)
    {
      Text text;

      text = STR("Hello World");

      bool is_equal = Equal(TextSTD(STR("Hello World")), text);
      CHECK(is_equal);
      CHECK_FALSE(text.is_truncated());
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_assignment_from_literal_excess)
    {
      Text text;

      text = STR("Hello World There");

      bool is_equal = Equal(TextSTD(STR("Hello World")), text);
      CHECK(is_equal);
#if ETL_HAS_STRING_TRUNCATION_CHECKS
      CHECK_TRUE(text.is_truncated());
#else
      CHECK_FALSE(text.is_truncated());
#endif
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_assignment_from_literal_via_interface)
    {
      Text text;
      IText& itext = text;

      itext = STR("Hello World");

      bool is_equal = Equal(TextSTD(STR("Hello World")), itext);
      CHECK(is_equal);
      CHECK(!itext.is_truncated());
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_assignment_from_literal_via_interface_excess)
    {
      Text text;
      IText& itext = text;

      itext = STR("Hello World There");

      bool is_equal = Equal(TextSTD(STR("Hello World")), itext);
      CHECK(is_equal);
#if ETL_HAS_STRING_TRUNCATION_CHECKS
      CHECK(itext.is_truncated());
#endif
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_assignment_from_view)
    {
      Text text;

      text = View(STR("Hello World"));

      bool is_equal = Equal(TextSTD(STR("Hello World")), text);
      CHECK(is_equal);
      CHECK_FALSE(text.is_truncated());
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_begin)
    {
      Text text(initial_text.c_str());
      const Text constText(initial_text.c_str());

      CHECK_EQUAL(&text[0],      text.begin());
      CHECK_EQUAL(&constText[0], constText.begin());
    }


    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_end)
    {
      Text text(initial_text.c_str());
      const Text constText(initial_text.c_str());

      CHECK_EQUAL(&text[initial_text.size()],      text.end());
      CHECK_EQUAL(&constText[initial_text.size()], constText.end());
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_resize_up)
    {
      const size_t INITIAL_SIZE = 5UL;
      const size_t NEW_SIZE = 8UL;

      Text text(initial_text.c_str(), INITIAL_SIZE);
      text.resize(NEW_SIZE);

      CHECK_EQUAL(text.size(), NEW_SIZE);
      CHECK_FALSE(text.is_truncated());
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_resize_up_value)
    {
      const size_t INITIAL_SIZE = 5UL;
      const size_t NEW_SIZE = 8UL;
      const value_t INITIAL_VALUE = STR('A');

      Text text(INITIAL_SIZE, INITIAL_VALUE);
      text.resize(NEW_SIZE, INITIAL_VALUE);

      std::array<value_t, NEW_SIZE> compare_text;
      compare_text.fill(INITIAL_VALUE);

      bool is_equal = Equal(compare_text, text);
      CHECK(is_equal);
      CHECK_FALSE(text.is_truncated());
    }


    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_resize_excess)
    {
      const size_t INITIAL_SIZE = 5UL;
      const size_t NEW_SIZE = SIZE + 1UL;

      Text text(INITIAL_SIZE, STR('A'));
      text.resize(NEW_SIZE, STR('A'));
      CHECK_EQUAL(SIZE, text.size());
#if ETL_HAS_STRING_TRUNCATION_CHECKS
      CHECK_TRUE(text.is_truncated());
#else
      CHECK_FALSE(text.is_truncated());
#endif
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_resize_down)
    {
      const size_t INITIAL_SIZE = 5UL;
      const size_t NEW_SIZE = 2UL;

      Text text(INITIAL_SIZE, STR('A'));
      text.resize(NEW_SIZE);

      CHECK_EQUAL(text.size(), NEW_SIZE);
      CHECK_FALSE(text.is_truncated());
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_resize_down_value)
    {
      const size_t INITIAL_SIZE = 5UL;
      const size_t NEW_SIZE = 2UL;
      const value_t INITIAL_VALUE = STR('A');

      Text text(INITIAL_SIZE, INITIAL_VALUE);
      text.resize(NEW_SIZE, INITIAL_VALUE);

      CHECK_EQUAL(text.size(), NEW_SIZE);

      std::array<value_t, NEW_SIZE> compare_text;
      compare_text.fill(INITIAL_VALUE);

      bool is_equal = Equal(compare_text, text);
      CHECK(is_equal);
      CHECK_FALSE(text.is_truncated());
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_uninitialized_resize_up)
    {
      const size_t  INITIAL_SIZE = 5UL;
      const size_t  NEW_SIZE = 8UL;
      const value_t INITIAL_VALUE = STR('A');
      const value_t FILL_VALUE = STR('B');

      Text text(INITIAL_SIZE, INITIAL_VALUE);

      Text::pointer pbegin = &text.front();
      Text::pointer pend   = &text.back() + 1;
      Text::pointer pmax   = pbegin + text.max_size();

      // Fill free space with a pattern.
      std::fill(pend, pmax, FILL_VALUE);

      text.uninitialized_resize(NEW_SIZE);

      std::array<value_t, NEW_SIZE> compare_text;
      compare_text.fill(FILL_VALUE);
      std::fill(compare_text.begin(), compare_text.begin() + INITIAL_SIZE, INITIAL_VALUE);

      bool is_equal = Equal(compare_text, text);
      CHECK(is_equal);

      CHECK_EQUAL(NEW_SIZE, text.size());
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_uninitialized_resize_up_excess)
    {
      const size_t INITIAL_SIZE = 5UL;
      const size_t NEW_SIZE = SIZE + 1UL;

      Text text(INITIAL_SIZE, STR('A'));

      text.uninitialized_resize(NEW_SIZE);

      CHECK_EQUAL(SIZE, text.size());
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_uninitialized_resize_down)
    {
      const size_t  INITIAL_SIZE = 5UL;
      const size_t  NEW_SIZE = 2UL;
      const value_t INITIAL_VALUE = STR('A');
      const value_t FILL_VALUE = STR('B');

      Text text(INITIAL_SIZE, INITIAL_VALUE);

      Text::pointer pbegin = &text.front();
      Text::pointer pend   = &text.back() + 1;
      Text::pointer pmax   = pbegin + text.max_size();

      // Fill free space with a pattern.
      std::fill(pend, pmax, FILL_VALUE);

      text.uninitialized_resize(NEW_SIZE);

      std::array<value_t, NEW_SIZE> compare_text;
      compare_text.fill(INITIAL_VALUE);

      bool is_equal = Equal(compare_text, text);
      CHECK(is_equal);

      CHECK_EQUAL(NEW_SIZE, text.size());
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_resize_and_overwrite_up)
    {
      const size_t INITIAL_SIZE = 5UL;
      const size_t NEW_SIZE     = 8UL;

      Text text(initial_text.c_str(), INITIAL_SIZE);

      // Overwrite from index 1 to one less than the new size and set to that size.
      text.resize_and_overwrite(NEW_SIZE, [](Text::pointer p, size_t n) noexcept
                                {
                                  size_t i = 1;
                                  while (i < (n - 1))
                                  {
                                    p[i] = '1' + Text::value_type(i);
                                    ++i;
                                  }   

                                  return i;
                                });

      CHECK_EQUAL(NEW_SIZE - 1, text.size());
      CHECK_TRUE(Equal(TextSTD(STR("H234567")), text));
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_resize_and_overwrite_down)
    {
      const size_t INITIAL_SIZE = 5UL;
      const size_t NEW_SIZE     = 3UL;

      Text text(initial_text.c_str(), INITIAL_SIZE);

      // Overwrite from index 1 to one less than the new size and set to that size.
      text.resize_and_overwrite(NEW_SIZE, [](Text::pointer p, size_t n) 
                                {
                                  size_t i = 1;
                                  while (i < (n - 1))
                                  {
                                    p[i] = '1' + Text::value_type(i);
                                    ++i;
                                  }   

                                  return i;
                                });

      CHECK_EQUAL(NEW_SIZE - 1, text.size());
      CHECK_TRUE(Equal(TextSTD(STR("H2")), text));
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_resize_and_overwrite_up_excess)
    {
      Text text(initial_text.c_str(), initial_text.size());

      CHECK_THROW(text.resize_and_overwrite(text.capacity() + 1, [](Text::pointer /*p*/, size_t n) { return n; }), etl::string_out_of_bounds);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_fill)
    {
      Text text(SIZE, STR('A'));
      Text expected(SIZE, STR('B'));

      text.fill(STR('B'));

      bool is_equal = Equal(expected, text);
      CHECK(is_equal);
      CHECK_FALSE(text.is_truncated());
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_empty_full)
    {
      Text text;
      text.resize(text.max_size(), STR('A'));

      CHECK(!text.empty());
      CHECK_FALSE(text.is_truncated());
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_empty_half)
    {
      Text text;
      text.resize(text.max_size() / 2, STR('A'));

      CHECK(!text.empty());
      CHECK_FALSE(text.is_truncated());
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_empty_empty)
    {
      Text text;

      CHECK(text.empty());
      CHECK_FALSE(text.is_truncated());
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_full_full)
    {
      Text text;
      text.resize(text.max_size(), STR('A'));

      CHECK(text.full());
      CHECK_FALSE(text.is_truncated());
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_full_half)
    {
      Text text;
      text.resize(text.max_size() / 2, STR('A'));

      CHECK(!text.full());
      CHECK_FALSE(text.is_truncated());
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_full_empty)
    {
      Text text;

      CHECK(!text.full());
      CHECK_FALSE(text.is_truncated());
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_index)
    {
      TextSTD compare_text(initial_text.c_str());
      Text text(initial_text.c_str());

      for (size_t i = 0UL; i < text.size(); ++i)
      {
        CHECK_EQUAL(text[i], compare_text[i]);
      }

      CHECK_FALSE(text.is_truncated());
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_index_const)
    {
      const TextSTD compare_text(initial_text.c_str());
      const Text text(initial_text.c_str());

      for (size_t i = 0UL; i < text.size(); ++i)
      {
        CHECK_EQUAL(text[i], compare_text[i]);
      }

      CHECK_FALSE(text.is_truncated());
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_at)
    {
      TextSTD compare_text(initial_text.c_str());
      Text text(initial_text.c_str());

      for (size_t i = 0UL; i < text.size(); ++i)
      {
        CHECK_EQUAL(text.at(i), compare_text.at(i));
      }

      CHECK_FALSE(text.is_truncated());
      CHECK_THROW(text.at(text.size()), etl::string_out_of_bounds);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_at_const)
    {
      const TextSTD compare_text(initial_text.c_str());
      const Text text(initial_text.c_str());

      for (size_t i = 0UL; i < text.size(); ++i)
      {
        CHECK_EQUAL(text.at(i), compare_text.at(i));
      }

      CHECK_FALSE(text.is_truncated());
      CHECK_THROW(text.at(text.size()), etl::string_out_of_bounds);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_front)
    {
      TextSTD compare_text(initial_text.c_str());
      Text text(initial_text.c_str());

      CHECK(text.front() == compare_text.front());
      CHECK_FALSE(text.is_truncated());
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_front_const)
    {
      const TextSTD compare_text(initial_text.c_str());
      const Text text(initial_text.c_str());

      CHECK(text.front() == compare_text.front());
      CHECK_FALSE(text.is_truncated());
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_back)
    {
      TextSTD compare_text(initial_text.c_str());
      Text text(initial_text.c_str());

      CHECK(text.back() == compare_text.back());
      CHECK_FALSE(text.is_truncated());
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_back_const)
    {
      const TextSTD compare_text(initial_text.c_str());
      const Text text(initial_text.c_str());

      CHECK(text.back() == compare_text.back());
      CHECK_FALSE(text.is_truncated());
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_data)
    {
      TextSTD compare_text(initial_text.c_str());

      Text text(compare_text.begin(), compare_text.end());

      bool is_equal = std::equal(text.data(),
                                 text.data_end(),
                                 compare_text.begin());

      CHECK(is_equal);
      CHECK_FALSE(text.is_truncated());
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_data_const)
    {
      TextSTD compare_text(initial_text.c_str());

      const Text text(compare_text.begin(), compare_text.end());

      bool is_equal = std::equal(text.data(),
                                 text.data_end(),
                                 compare_text.begin());

      CHECK(is_equal);
      CHECK_FALSE(text.is_truncated());
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_assign_string)
    {
      TextSTD compare_input(initial_text.c_str());
      Text input(initial_text.c_str());

      TextSTD compare_text;
      Text text;

      compare_text.assign(compare_input);
      text.assign(input);

      bool is_equal = Equal(compare_text, text);
      CHECK(is_equal);
      CHECK_FALSE(text.is_truncated());
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_self_assign_string)
    {
      Text text(initial_text.c_str());

      text.assign(text);

      bool is_equal = Equal(initial_text, text);
      CHECK(is_equal);
      CHECK_FALSE(text.is_truncated());
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_assign_view)
    {
      TextSTD compare_input(initial_text.c_str());
      Text input(initial_text.c_str());
      View view(input);

      TextSTD compare_text;
      Text text;

      compare_text.assign(compare_input);
      text.assign(view);

      bool is_equal = Equal(compare_text, text);
      CHECK(is_equal);
      CHECK_FALSE(text.is_truncated());
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_assign_string_excess)
    {
      TextSTD compare_input(initial_text.c_str());
      TextL input(longer_text.c_str());

      TextSTD compare_text;
      Text text;

      compare_text.assign(compare_input);
      text.assign(input);

      bool is_equal = Equal(compare_text, text);
      CHECK(is_equal);
#if ETL_HAS_STRING_TRUNCATION_CHECKS
      CHECK_TRUE(text.is_truncated());
#else
      CHECK_FALSE(text.is_truncated());
#endif
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_assign_pointer)
    {
      TextSTD compare_text(initial_text.c_str());

      Text text;
      text.assign(initial_text.c_str());

      bool is_equal = Equal(compare_text, text);
      CHECK(is_equal);
      CHECK_FALSE(text.is_truncated());
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_assign_pointer_excess)
    {
      TextSTD compare_text(initial_text.c_str());

      Text text;
      text.assign(longer_text.c_str());

      bool is_equal = Equal(compare_text, text);
      CHECK(is_equal);
#if ETL_HAS_STRING_TRUNCATION_CHECKS
      CHECK_TRUE(text.is_truncated());
#else
      CHECK_FALSE(text.is_truncated());
#endif
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_assign_pointer_length)
    {
      TextSTD compare_text(initial_text.c_str());

      Text text;
      text.assign(initial_text.c_str(), initial_text.size());

      bool is_equal = Equal(compare_text, text);
      CHECK(is_equal);
      CHECK_FALSE(text.is_truncated());
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_assign_pointer_length_excess)
    {
      TextSTD compare_text(longer_text.c_str());

      Text text;
      text.assign(longer_text.c_str(), longer_text.size());

      compare_text.resize(text.max_size());

      bool is_equal = Equal(compare_text, text);
      CHECK(is_equal);
#if ETL_HAS_STRING_TRUNCATION_CHECKS
      CHECK_TRUE(text.is_truncated());
#else
      CHECK_FALSE(text.is_truncated());
#endif
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_assign_range)
    {
      TextSTD compare_text(initial_text.c_str());

      Text text;

      text.assign(compare_text.begin(), compare_text.end());

      bool is_equal = Equal(compare_text, text);
      CHECK(is_equal);
      CHECK_FALSE(text.is_truncated());
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_assign_range_excess)
    {
      Text text;

      text.assign(longer_text.begin(), longer_text.end());

      CHECK_EQUAL(initial_text.size(), text.size());

      bool is_equal = Equal(initial_text, text);
      CHECK(is_equal);
#if ETL_HAS_STRING_TRUNCATION_CHECKS
      CHECK_TRUE(text.is_truncated());
#else
      CHECK_FALSE(text.is_truncated());
#endif
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_assign_size_value)
    {
      const size_t INITIAL_SIZE = 5UL;
      const value_t INITIAL_VALUE = STR('A');

      std::array<value_t, INITIAL_SIZE> compare_text;
      compare_text.fill(INITIAL_VALUE);

      Text text;
      text.assign(INITIAL_SIZE, INITIAL_VALUE);

      CHECK(text.size() == INITIAL_SIZE);

      bool is_equal = Equal(compare_text, text);
      CHECK(is_equal);
      CHECK_FALSE(text.is_truncated());
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_assign_size_value_excess)
    {
      const size_t INITIAL_SIZE = SIZE;
      const size_t EXCESS_SIZE = SIZE + 1UL;
      const value_t INITIAL_VALUE = STR('A');
      std::array<value_t, INITIAL_SIZE> compare_text;
      compare_text.fill(INITIAL_VALUE);

      Text text;
      text.assign(EXCESS_SIZE, INITIAL_VALUE);

      bool is_equal = Equal(compare_text, text);
      CHECK(is_equal);
#if ETL_HAS_STRING_TRUNCATION_CHECKS
      CHECK_TRUE(text.is_truncated());
#else
      CHECK_FALSE(text.is_truncated());
#endif
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_push_back)
    {
      TextSTD compare_text;
      Text text;

      for (size_t i = 0UL; i < SIZE; ++i)
      {
        compare_text.push_back(STR('A') + value_t(i));
      }

      for (size_t i = 0UL; i < SIZE; ++i)
      {
        text.push_back(STR('A') + value_t(i));
      }

      CHECK_EQUAL(etl::strlen(compare_text.data()), etl::strlen(text.data()));
      CHECK_EQUAL(compare_text.size(), text.size());

      bool is_equal = Equal(compare_text, text);
      CHECK(is_equal);
      CHECK_FALSE(text.is_truncated());
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_push_back_excess)
    {
      TextSTD compare_text;
      Text text;

      for (size_t i = 0UL; i < SIZE; ++i)
      {
        compare_text.push_back(STR('A') + value_t(i));
      }

      for (size_t i = 0UL; i < SIZE; ++i)
      {
        text.push_back(STR('A') + value_t(i));
        CHECK_FALSE(text.is_truncated());
      }

      text.push_back(STR('A') + value_t(SIZE));

      CHECK_EQUAL(etl::strlen(compare_text.data()), etl::strlen(text.data()));
      CHECK_EQUAL(compare_text.size(), text.size());

      bool is_equal = Equal(compare_text, text);
      CHECK(is_equal);
#if ETL_HAS_STRING_TRUNCATION_CHECKS
      CHECK_TRUE(text.is_truncated());
#else
      CHECK_FALSE(text.is_truncated());
#endif
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_pop_back)
    {
      TextSTD compare_text(initial_text.c_str());
      Text text(initial_text.c_str());

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
      const size_t INITIAL_SIZE = 5UL;
      const value_t INITIAL_VALUE  = STR('A');

      for (size_t offset = 0UL; offset <= INITIAL_SIZE; ++offset)
      {
        TextSTD compare_text;
        Text text;

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
      TextSTD compare_text(initial_text.begin(), initial_text.end());
      Text text(initial_text.begin(), initial_text.end());

      const value_t INITIAL_VALUE = STR('A');

      size_t offset = 2UL;
      text.insert(text.cbegin() + offset, INITIAL_VALUE);
      compare_text.insert(compare_text.cbegin() + offset, INITIAL_VALUE);
      compare_text.erase(compare_text.cend() - 1);

#if ETL_HAS_STRING_TRUNCATION_CHECKS
      CHECK_TRUE(text.is_truncated());
#else
      CHECK_FALSE(text.is_truncated());
#endif

      bool is_equal = Equal(compare_text, text);
      CHECK(is_equal);

      offset = 0;
      text.insert(text.cbegin() + offset, STR('A'));
      compare_text.insert(compare_text.cbegin() + offset, STR('A'));
      compare_text.erase(compare_text.cend() - 1);

#if ETL_HAS_STRING_TRUNCATION_CHECKS
      CHECK_TRUE(text.is_truncated());
#else
      CHECK_FALSE(text.is_truncated());
#endif

      is_equal = Equal(compare_text, text);
      CHECK(is_equal);

      offset = text.size();
      text.insert(text.cbegin() + offset, STR('A'));
      compare_text.insert(compare_text.cbegin() + offset, STR('A'));
      compare_text.erase(compare_text.cend() - 1);

#if ETL_HAS_STRING_TRUNCATION_CHECKS
      CHECK_TRUE(text.is_truncated());
#else
      CHECK_FALSE(text.is_truncated());
#endif

      is_equal = Equal(compare_text, text);
      CHECK(is_equal);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_insert_position_n_value)
    {
      TextSTD compare_text;
      Text text;

      const size_t INITIAL_SIZE     = 5UL;
      const size_t INSERT_SIZE      = 3UL;
      const value_t INITIAL_VALUE      = STR('A');

      for (size_t offset = 0UL; offset <= INITIAL_SIZE; ++offset)
      {
        text.assign(initial_text.begin(), initial_text.begin() + INITIAL_SIZE);
        compare_text.assign(initial_text.begin(), initial_text.begin() + INITIAL_SIZE);
        text.insert(text.begin() + offset, INSERT_SIZE, INITIAL_VALUE);
        compare_text.insert(compare_text.begin() + offset, INSERT_SIZE, INITIAL_VALUE);

        CHECK_FALSE(text.is_truncated());

        bool is_equal = Equal(compare_text, text);
        CHECK(is_equal);
      }
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_insert_position_n_value_excess)
    {
      TextSTD compare_text;
      Text text;

      const size_t INSERT_SIZE = 4UL;
      const value_t INSERT_VALUE  = STR('A');

      size_t offset = 0UL;
      compare_text.assign(initial_text.cbegin(), initial_text.cend());
      text.assign(initial_text.cbegin(), initial_text.cend());
      compare_text.insert(compare_text.cbegin() + offset, INSERT_SIZE, INSERT_VALUE);
      compare_text.erase(compare_text.cend() - INSERT_SIZE, compare_text.cend());
      text.insert(text.cbegin() + offset, INSERT_SIZE, INSERT_VALUE);

#if ETL_HAS_STRING_TRUNCATION_CHECKS
      CHECK_TRUE(text.is_truncated());
#else
      CHECK_FALSE(text.is_truncated());
#endif

      bool is_equal = Equal(compare_text, text);
      CHECK(is_equal);

      offset = 2;
      compare_text.assign(initial_text.cbegin(), initial_text.cend());
      text.assign(initial_text.cbegin(), initial_text.cend());
      compare_text.insert(compare_text.cbegin() + offset, INSERT_SIZE, INSERT_VALUE);
      compare_text.erase(compare_text.cend() - INSERT_SIZE, compare_text.cend());
      text.assign(initial_text.cbegin(), initial_text.cend());
      text.insert(text.cbegin() + offset, INSERT_SIZE, INSERT_VALUE);

#if ETL_HAS_STRING_TRUNCATION_CHECKS
      CHECK_TRUE(text.is_truncated());
#else
      CHECK_FALSE(text.is_truncated());
#endif

      is_equal = Equal(compare_text, text);
      CHECK(is_equal);

      offset = 4;
      compare_text.assign(initial_text.cbegin(), initial_text.cend());
      text.assign(initial_text.cbegin(), initial_text.cend());
      compare_text.insert(compare_text.cbegin() + offset, INSERT_SIZE, INSERT_VALUE);
      compare_text.erase(compare_text.cend() - INSERT_SIZE, compare_text.cend());
      text.assign(initial_text.cbegin(), initial_text.cend());
      text.insert(text.cbegin() + offset, INSERT_SIZE, INSERT_VALUE);

#if ETL_HAS_STRING_TRUNCATION_CHECKS
      CHECK_TRUE(text.is_truncated());
#else
      CHECK_FALSE(text.is_truncated());
#endif

      is_equal = Equal(compare_text, text);
      CHECK(is_equal);

      offset = text.size();
      compare_text.assign(initial_text.cbegin(), initial_text.cend());
      text.assign(initial_text.cbegin(), initial_text.cend());
      compare_text.insert(compare_text.cbegin() + offset, INSERT_SIZE, INSERT_VALUE);
      compare_text.erase(compare_text.cend() - INSERT_SIZE, compare_text.cend());
      text.assign(initial_text.cbegin(), initial_text.cend());
      text.insert(text.cbegin() + offset, INSERT_SIZE, INSERT_VALUE);

#if ETL_HAS_STRING_TRUNCATION_CHECKS
      CHECK_TRUE(text.is_truncated());
#else
      CHECK_FALSE(text.is_truncated());
#endif

      is_equal = Equal(compare_text, text);
      CHECK(is_equal);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_insert_position_range)
    {
      const size_t INITIAL_SIZE = 5UL;

      for (size_t offset = 0UL; offset <= INITIAL_SIZE; ++offset)
      {
        TextSTD compare_text;
        Text text;

        text.assign(initial_text.cbegin(), initial_text.cbegin() + INITIAL_SIZE);
        compare_text.assign(initial_text.cbegin(), initial_text.cbegin() + INITIAL_SIZE);
        text.insert(text.cbegin() + offset, insert_text.cbegin(), insert_text.cend());
        compare_text.insert(compare_text.cbegin() + offset, insert_text.cbegin(), insert_text.cend());

        CHECK_FALSE(text.is_truncated());
#
        bool is_equal = Equal(compare_text, text);
        CHECK(is_equal);
      }
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_insert_position_range_excess)
    {
      const size_t INITIAL_SIZE = 5UL;
      const value_t INITIAL_VALUE  = STR('A');

      TextSTD compare_text;
      Text text;

      size_t offset = 0UL;

      compare_text.assign(INITIAL_SIZE, INITIAL_VALUE);
      text.assign(INITIAL_SIZE, INITIAL_VALUE);
      compare_text.insert(compare_text.cbegin() + offset, initial_text.cbegin(), initial_text.cend());
      compare_text.resize(initial_text.size());
      text.insert(text.cbegin() + offset, initial_text.cbegin(), initial_text.cend());

#if ETL_HAS_STRING_TRUNCATION_CHECKS
      CHECK_TRUE(text.is_truncated());
#else
      CHECK_FALSE(text.is_truncated());
#endif

      bool is_equal = Equal(compare_text, text);
      CHECK(is_equal);

      offset = 2;

      compare_text.assign(INITIAL_SIZE, INITIAL_VALUE);
      text.assign(INITIAL_SIZE, INITIAL_VALUE);
      compare_text.insert(compare_text.cbegin() + offset, initial_text.cbegin(), initial_text.cend());
      compare_text.resize(initial_text.size());
      text.insert(text.cbegin() + offset, initial_text.cbegin(), initial_text.cend());

#if ETL_HAS_STRING_TRUNCATION_CHECKS
      CHECK_TRUE(text.is_truncated());
#else
      CHECK_FALSE(text.is_truncated());
#endif

      is_equal = Equal(compare_text, text);
      CHECK(is_equal);


      offset = 4;

      compare_text.assign(INITIAL_SIZE, INITIAL_VALUE);
      text.assign(INITIAL_SIZE, INITIAL_VALUE);
      compare_text.insert(compare_text.cbegin() + offset, initial_text.cbegin(), initial_text.cend());
      compare_text.resize(initial_text.size());
      text.insert(text.cbegin() + offset, initial_text.cbegin(), initial_text.cend());

#if ETL_HAS_STRING_TRUNCATION_CHECKS
      CHECK_TRUE(text.is_truncated());
#else
      CHECK_FALSE(text.is_truncated());
#endif

      CHECK_EQUAL(compare_text.size(), text.size());
      is_equal = Equal(compare_text, text);
      CHECK(is_equal);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_insert_position_range_self)
    {
      size_t length = TextL::MAX_SIZE / 2UL;

      for (size_t offset = 10UL; offset < length; ++offset)
      {
        TextSTD compare_text = STR("ABCDEFGHIJKLMNOPQRSTUVWXYZ");
        TextL text = STR("ABCDEFGHIJKLMNOPQRSTUVWXYZ");

        text.insert(text.cbegin() + offset, text.cbegin() + 5, text.cbegin() + 10);
        compare_text.insert(compare_text.cbegin() + offset, compare_text.cbegin() + 5, compare_text.cbegin() + 10);

        bool is_equal = Equal(compare_text, text);
        CHECK(is_equal);
        CHECK_FALSE(text.is_truncated());
      }
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_insert_size_t_position_string)
    {
      for (size_t offset = 0UL; offset <= short_text.size(); ++offset)
      {
        TextSTD compare_text(short_text.cbegin(), short_text.cend());
        Text text(short_text.cbegin(), short_text.cend());
        Text insert(insert_text.cbegin(), insert_text.cend());

        text.insert(offset, insert);
        compare_text.insert(offset, insert_text);
        compare_text.resize(std::min(compare_text.size(), SIZE));

        bool is_equal = Equal(compare_text, text);
        CHECK(is_equal);
        CHECK_FALSE(text.is_truncated());
      }
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_insert_size_t_position_view)
    {
      for (size_t offset = 0UL; offset <= short_text.size(); ++offset)
      {
        TextSTD compare_text(short_text.cbegin(), short_text.cend());
        Text text(short_text.cbegin(), short_text.cend());
        View view(insert_text.data(), insert_text.size());

        text.insert(offset, view);
        compare_text.insert(offset, insert_text);
        compare_text.resize(std::min(compare_text.size(), SIZE));

        bool is_equal = Equal(compare_text, text);
        CHECK(is_equal);
        CHECK_FALSE(text.is_truncated());
      }
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_insert_size_t_position_string_excess)
    {
      for (size_t offset = 0UL; offset <= initial_text.size(); ++offset)
      {
        TextSTD compare_text(initial_text.cbegin(), initial_text.cend());
        Text text(initial_text.cbegin(), initial_text.cend());
        Text insert(insert_text.cbegin(), insert_text.cend());

        text.insert(offset, insert);
        compare_text.insert(offset, insert_text);
        compare_text.resize(std::min(compare_text.size(), SIZE));

        bool is_equal = Equal(compare_text, text);
        CHECK(is_equal);
#if ETL_HAS_STRING_TRUNCATION_CHECKS
        CHECK_TRUE(text.is_truncated());
#else
        CHECK_FALSE(text.is_truncated());
#endif
      }
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_insert_size_t_position_string_from_truncated)
    {
      for (size_t offset = 0UL; offset <= short_text.size(); ++offset)
      {
        TextSTD compare_text(short_text.cbegin(), short_text.cend());
        Text text(short_text.cbegin(), short_text.cend());
        Text insert(longer_text.cbegin(), longer_text.cend());
        insert.erase(insert.cbegin(), insert.cend());
        insert.append(insert_text.cbegin(), insert_text.cend());

        text.insert(offset, insert);
        compare_text.insert(offset, insert_text);
        compare_text.resize(std::min(compare_text.size(), SIZE));

        bool is_equal = Equal(compare_text, text);
        CHECK(is_equal);
#if ETL_HAS_STRING_TRUNCATION_CHECKS
        CHECK_TRUE(text.is_truncated());
#else
        CHECK_FALSE(text.is_truncated());
#endif
      }
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_insert_size_t_position_string_subpos_sunlen)
    {
      TextSTD compare_text(short_text.cbegin(), short_text.cend());
      Text text(short_text.cbegin(), short_text.cend());
      Text insert(insert_text.cbegin(), insert_text.cend());

      text.insert(0, insert, 0, insert.size());
      compare_text.insert(0, insert_text, 0, insert_text.size());
      compare_text.resize(std::min(compare_text.size(), SIZE));

      bool is_equal = Equal(compare_text, text);
      CHECK(is_equal);
      CHECK_FALSE(text.is_truncated());

      compare_text.assign(short_text.cbegin(), short_text.cend());
      text.assign(short_text.cbegin(), short_text.cend());

      text.insert(2, insert, 2, insert.size() - 2);
      compare_text.insert(2, insert_text, 2, insert_text.size() - 2);
      compare_text.resize(std::min(compare_text.size(), SIZE));

      CHECK_EQUAL(compare_text.size(), text.size());

      is_equal = Equal(compare_text, text);
      CHECK(is_equal);
      CHECK_FALSE(text.is_truncated());

      compare_text.assign(short_text.cbegin(), short_text.cend());
      text.assign(short_text.cbegin(), short_text.cend());

      text.insert(short_text.size(), insert, 0, insert.size());
      compare_text.insert(short_text.size(), insert_text, 0, insert_text.size());
      compare_text.resize(std::min(compare_text.size(), SIZE));

      is_equal = Equal(compare_text, text);
      CHECK(is_equal);
      CHECK_FALSE(text.is_truncated());
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_append_string)
    {
      TextSTD compare_text(short_text.c_str());
      Text text(short_text.c_str());
      Text append(insert_text.c_str());

      // Non-overflow.
      compare_text.append(insert_text);
      text.append(append);

      bool is_equal = Equal(compare_text, text);
      CHECK(is_equal);
      CHECK_FALSE(text.is_truncated());

      // Overflow.
      compare_text.assign(short_text.c_str());
      text.assign(short_text.c_str());
      append.assign(initial_text.c_str());

      compare_text.append(initial_text);
      compare_text.resize(std::min(compare_text.size(), SIZE));
      text.append(append);

      is_equal = Equal(compare_text, text);
      CHECK(is_equal);
#if ETL_HAS_STRING_TRUNCATION_CHECKS
      CHECK_TRUE(text.is_truncated());
#else
      CHECK_FALSE(text.is_truncated());
#endif
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_append_view)
    {
      TextSTD compare_text(short_text.c_str());
      Text text(short_text.c_str());
      View view(insert_text.data(), insert_text.size());

      // Non-overflow.
      compare_text.append(insert_text);
      text.append(view);

      bool is_equal = Equal(compare_text, text);
      CHECK(is_equal);
      CHECK_FALSE(text.is_truncated());

      // Overflow.
      compare_text.assign(short_text.c_str());
      text.assign(short_text.c_str());
      view.assign(initial_text.data(), initial_text.size());

      compare_text.append(initial_text);
      compare_text.resize(std::min(compare_text.size(), SIZE));
      text.append(view);

      is_equal = Equal(compare_text, text);
      CHECK(is_equal);
#if ETL_HAS_STRING_TRUNCATION_CHECKS
      CHECK_TRUE(text.is_truncated());
#else
      CHECK_FALSE(text.is_truncated());
#endif
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_append_truncated_string)
    {
      //#include "etl/private/diagnostic_array_bounds_push.h"
      Text text(short_text.c_str());
      TextS append(short_text.c_str());
#if ETL_HAS_STRING_TRUNCATION_CHECKS
      CHECK(append.is_truncated());
#endif

      text.append(append);
#if ETL_HAS_STRING_TRUNCATION_CHECKS
      CHECK_TRUE(text.is_truncated());
#else
      CHECK_FALSE(text.is_truncated());
#endif
      //#include "etl/private/diagnostic_pop.h"
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_append_string_to_self)
    {
      TextSTD compare_text(short_text.c_str());
      Text text(short_text.c_str());

      // Non-overflow.
      compare_text.append(compare_text);
      text.append(text);

      bool is_equal = Equal(compare_text, text);
      CHECK(is_equal);
#if ETL_HAS_STRING_TRUNCATION_CHECKS
      CHECK_FALSE(text.is_truncated());
#endif

      // Overflow.
      compare_text.assign(shorter_text.c_str());
      text.assign(shorter_text.c_str());

      compare_text.append(compare_text);
      compare_text.resize(std::min(compare_text.size(), SIZE));
      text.append(text);

      is_equal = Equal(compare_text, text);
      CHECK(is_equal);
#if ETL_HAS_STRING_TRUNCATION_CHECKS
      CHECK_TRUE(text.is_truncated());
#else
      CHECK_FALSE(text.is_truncated());
#endif
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_append_string_subpos_sublen)
    {
      TextSTD compare_text(short_text.c_str());
      Text text(short_text.c_str());
      Text append(insert_text.c_str());

      // Whole string.
      compare_text.append(insert_text, 0, TextSTD::npos);
      text.append(append, 0, Text::npos);

      bool is_equal = Equal(compare_text, text);
      CHECK(is_equal);
#if ETL_HAS_STRING_TRUNCATION_CHECKS
      CHECK_FALSE(text.is_truncated());
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
#if ETL_HAS_STRING_TRUNCATION_CHECKS
      CHECK_FALSE(text.is_truncated());
#endif

      // Overflow.
      compare_text.assign(short_text.c_str());
      text.assign(short_text.c_str());
      append.assign(initial_text.c_str());

      compare_text.append(initial_text, 1, initial_text.size() - 1);
      compare_text.resize(std::min(compare_text.size(), SIZE));
      text.append(append, 1, append.size() - 1);

      is_equal = Equal(compare_text, text);
      CHECK(is_equal);
#if ETL_HAS_STRING_TRUNCATION_CHECKS
      CHECK_TRUE(text.is_truncated());
#else
      CHECK_FALSE(text.is_truncated());
#endif
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_append_truncated_string_subpos_sublen)
    {
      Text text(short_text.c_str());
      TextS append(short_text.c_str());
#if ETL_HAS_STRING_TRUNCATION_CHECKS
      CHECK(append.is_truncated());
#endif

      text.append(append, 1, 2);
#if ETL_HAS_STRING_TRUNCATION_CHECKS
      CHECK_TRUE(text.is_truncated());
#else
      CHECK_FALSE(text.is_truncated());
#endif
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_append_c_string)
    {
      // Non-overflow.
      TextSTD compare_text(short_text.c_str());
      Text text(short_text.c_str());

      // Whole string.
      compare_text.append(insert_text.c_str());
      text.append(insert_text.c_str());

      bool is_equal = Equal(compare_text, text);
      CHECK(is_equal);
#if ETL_HAS_STRING_TRUNCATION_CHECKS
      CHECK_FALSE(text.is_truncated());
#endif

      // Overflow.
      compare_text.assign(short_text.c_str());
      text.assign(short_text.c_str());

      compare_text.append(initial_text.c_str());
      compare_text.resize(std::min(compare_text.size(), SIZE));
      text.append(initial_text.c_str());

      is_equal = Equal(compare_text, text);
      CHECK(is_equal);
#if ETL_HAS_STRING_TRUNCATION_CHECKS
      CHECK_TRUE(text.is_truncated());
#else
      CHECK_FALSE(text.is_truncated());
#endif
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_append_n_c)
    {
      // Non-overflow.
      TextSTD compare_text(short_text.c_str());
      Text text(short_text.c_str());

      // Non-overflow.
      compare_text.append(5, STR('A'));
      text.append(5, STR('A'));

      bool is_equal = Equal(compare_text, text);
      CHECK(is_equal);
#if ETL_HAS_STRING_TRUNCATION_CHECKS
      CHECK_FALSE(text.is_truncated());
#endif

      // Overflow.
      compare_text.assign(short_text.c_str());
      text.assign(short_text.c_str());

      compare_text.append(SIZE, STR('A'));
      compare_text.resize(std::min(compare_text.size(), SIZE));
      text.append(SIZE, STR('A'));

      is_equal = Equal(compare_text, text);
      CHECK(is_equal);
#if ETL_HAS_STRING_TRUNCATION_CHECKS
      CHECK_TRUE(text.is_truncated());
#else
      CHECK_FALSE(text.is_truncated());
#endif
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_append_range)
    {
      // Non-overflow.
      TextSTD compare_text(short_text.c_str());
      Text text(short_text.c_str());
      Text append(insert_text.c_str());

      compare_text.append(insert_text.begin(), insert_text.end());
      text.append(append.begin(), append.end());

      bool is_equal = Equal(compare_text, text);
      CHECK(is_equal);
#if ETL_HAS_STRING_TRUNCATION_CHECKS
      CHECK_FALSE(text.is_truncated());
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
#if ETL_HAS_STRING_TRUNCATION_CHECKS
      CHECK_TRUE(text.is_truncated());
#else
      CHECK_FALSE(text.is_truncated());
#endif
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_replace_position_length_string)
    {
      // Non-overflow short text, npos.
      TextSTD compare_text(short_text.c_str());
      Text text(short_text.c_str());

      compare_text.replace(2, TextSTD::npos, TextSTD(STR("Replace")));
      compare_text.resize(std::min(compare_text.size(), SIZE));
      text.replace(2, Text::npos, TextL(STR("Replace")));

      bool is_equal = Equal(compare_text, text);
      CHECK(is_equal);
#if ETL_HAS_STRING_TRUNCATION_CHECKS
      CHECK_FALSE(text.is_truncated());
#endif

      // Non-overflow short text.
      compare_text.assign(short_text.c_str());
      text.assign(short_text.c_str());

      compare_text.replace(2, 2, TextSTD(STR("Replace")));
      compare_text.resize(std::min(compare_text.size(), SIZE));
      text.replace(2, 2, TextL(STR("Replace")));

      is_equal = Equal(compare_text, text);
      CHECK(is_equal);
#if ETL_HAS_STRING_TRUNCATION_CHECKS
      CHECK_FALSE(text.is_truncated());
#endif

      // Overflow short text.
      compare_text.assign(short_text.c_str());
      text.assign(short_text.c_str());

      compare_text.replace(2, 2, TextSTD(STR("Replace with some text")));
      compare_text.resize(std::min(compare_text.size(), SIZE));
      text.replace(2, 2, TextL(STR("Replace with some text")));

      is_equal = Equal(compare_text, text);
      CHECK(is_equal);
#if ETL_HAS_STRING_TRUNCATION_CHECKS
      CHECK_TRUE(text.is_truncated());
#else
      CHECK_FALSE(text.is_truncated());
#endif

      // Overflow short text, npos.
      compare_text.assign(short_text.c_str());
      text.assign(short_text.c_str());

      compare_text.replace(2, TextSTD::npos, TextSTD(STR("Replace with some text")));
      compare_text.resize(std::min(compare_text.size(), SIZE));
      text.replace(2, Text::npos, TextL(STR("Replace with some text")));

      is_equal = Equal(compare_text, text);
      CHECK(is_equal);
#if ETL_HAS_STRING_TRUNCATION_CHECKS
      CHECK_TRUE(text.is_truncated());
#else
      CHECK_FALSE(text.is_truncated());
#endif

      // Non-overflow.
      compare_text.assign(initial_text.c_str());
      text.assign(initial_text.c_str());

      compare_text.replace(2, 7, TextSTD(STR("Replace")));
      compare_text.resize(std::min(compare_text.size(), SIZE));
      text.replace(2, 7, TextL(STR("Replace")));

      is_equal = Equal(compare_text, text);
      CHECK(is_equal);
#if ETL_HAS_STRING_TRUNCATION_CHECKS
      CHECK_FALSE(text.is_truncated());
#endif

      // Overflow.
      compare_text.assign(initial_text.c_str());
      text.assign(initial_text.c_str());

      compare_text.replace(2, 2, TextSTD(STR("Replace with some text")));
      compare_text.resize(std::min(compare_text.size(), SIZE));
      text.replace(2, 2, TextL(STR("Replace with some text")));

      is_equal = Equal(compare_text, text);
      CHECK(is_equal);
#if ETL_HAS_STRING_TRUNCATION_CHECKS
      CHECK_TRUE(text.is_truncated());
#else
      CHECK_FALSE(text.is_truncated());
#endif

      // Overflow, npos.
      compare_text.assign(initial_text.c_str());
      text.assign(initial_text.c_str());

      compare_text.replace(2, TextSTD::npos, TextSTD(STR("Replace with some text")));
      compare_text.resize(std::min(compare_text.size(), SIZE));
      text.replace(2, Text::npos, TextL(STR("Replace with some text")));

      is_equal = Equal(compare_text, text);
      CHECK(is_equal);
#if ETL_HAS_STRING_TRUNCATION_CHECKS
      CHECK_TRUE(text.is_truncated());
#else
      CHECK_FALSE(text.is_truncated());
#endif
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_replace_position_length_view)
    {
      // Non-overflow short text, npos.
      TextSTD compare_text(short_text.c_str());
      Text text(short_text.c_str());      

      compare_text.replace(2, TextSTD::npos, TextSTD(STR("Replace")));
      compare_text.resize(std::min(compare_text.size(), SIZE));
      text.replace(2, Text::npos, View(STR("Replace")));

      bool is_equal = Equal(compare_text, text);
      CHECK(is_equal);
#if ETL_HAS_STRING_TRUNCATION_CHECKS
      CHECK_FALSE(text.is_truncated());
#endif

      // Non-overflow short text.
      compare_text.assign(short_text.c_str());
      text.assign(short_text.c_str());

      compare_text.replace(2, 2, TextSTD(STR("Replace")));
      compare_text.resize(std::min(compare_text.size(), SIZE));
      text.replace(2, 2, View(STR("Replace")));

      is_equal = Equal(compare_text, text);
      CHECK(is_equal);
#if ETL_HAS_STRING_TRUNCATION_CHECKS
      CHECK_FALSE(text.is_truncated());
#endif

      // Overflow short text.
      compare_text.assign(short_text.c_str());
      text.assign(short_text.c_str());

      compare_text.replace(2, 2, TextSTD(STR("Replace with some text")));
      compare_text.resize(std::min(compare_text.size(), SIZE));
      text.replace(2, 2, View(STR("Replace with some text")));

      is_equal = Equal(compare_text, text);
      CHECK(is_equal);
#if ETL_HAS_STRING_TRUNCATION_CHECKS
      CHECK_TRUE(text.is_truncated());
#else
      CHECK_FALSE(text.is_truncated());
#endif

      // Overflow short text, npos.
      compare_text.assign(short_text.c_str());
      text.assign(short_text.c_str());

      compare_text.replace(2, TextSTD::npos, TextSTD(STR("Replace with some text")));
      compare_text.resize(std::min(compare_text.size(), SIZE));
      text.replace(2, Text::npos, View(STR("Replace with some text")));

      is_equal = Equal(compare_text, text);
      CHECK(is_equal);
#if ETL_HAS_STRING_TRUNCATION_CHECKS
      CHECK_TRUE(text.is_truncated());
#else
      CHECK_FALSE(text.is_truncated());
#endif

      // Non-overflow.
      compare_text.assign(initial_text.c_str());
      text.assign(initial_text.c_str());

      compare_text.replace(2, 7, TextSTD(STR("Replace")));
      compare_text.resize(std::min(compare_text.size(), SIZE));
      text.replace(2, 7, View(STR("Replace")));

      is_equal = Equal(compare_text, text);
      CHECK(is_equal);
#if ETL_HAS_STRING_TRUNCATION_CHECKS
      CHECK_FALSE(text.is_truncated());
#endif

      // Overflow.
      compare_text.assign(initial_text.c_str());
      text.assign(initial_text.c_str());

      compare_text.replace(2, 2, TextSTD(STR("Replace with some text")));
      compare_text.resize(std::min(compare_text.size(), SIZE));
      text.replace(2, 2, View(STR("Replace with some text")));

      is_equal = Equal(compare_text, text);
      CHECK(is_equal);
#if ETL_HAS_STRING_TRUNCATION_CHECKS
      CHECK_TRUE(text.is_truncated());
#else
      CHECK_FALSE(text.is_truncated());
#endif

      // Overflow, npos.
      compare_text.assign(initial_text.c_str());
      text.assign(initial_text.c_str());

      compare_text.replace(2, TextSTD::npos, TextSTD(STR("Replace with some text")));
      compare_text.resize(std::min(compare_text.size(), SIZE));
      text.replace(2, Text::npos, View(STR("Replace with some text")));

      is_equal = Equal(compare_text, text);
      CHECK(is_equal);
#if ETL_HAS_STRING_TRUNCATION_CHECKS
      CHECK_TRUE(text.is_truncated());
#else
      CHECK_FALSE(text.is_truncated());
#endif
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_replace_first_last_string)
    {
      // Non-overflow short text.
      TextSTD compare_text(short_text.c_str());
      Text text(short_text.c_str());

      compare_text.replace(compare_text.begin() + 2, compare_text.begin() + 4, TextSTD(STR("Replace")));
      compare_text.resize(std::min(compare_text.size(), SIZE));
      text.replace(text.begin() + 2, text.begin() + 4, TextL(STR("Replace")));

      bool is_equal = Equal(compare_text, text);
      CHECK(is_equal);
#if ETL_HAS_STRING_TRUNCATION_CHECKS
      CHECK_FALSE(text.is_truncated());
#endif

      // Overflow short text.
      compare_text.assign(short_text.c_str());
      text.assign(short_text.c_str());

      compare_text.replace(compare_text.begin() + 2, compare_text.begin() + 4, TextSTD(STR("Replace with some text")));
      compare_text.resize(std::min(compare_text.size(), SIZE));
      text.replace(text.begin() + 2, text.begin() + 4, TextL(STR("Replace with some text")));

      is_equal = Equal(compare_text, text);
      CHECK(is_equal);
#if ETL_HAS_STRING_TRUNCATION_CHECKS
      CHECK_TRUE(text.is_truncated());
#else
      CHECK_FALSE(text.is_truncated());
#endif

      // Non-overflow.
      compare_text.assign(initial_text.c_str());
      text.assign(initial_text.c_str());

      compare_text.replace(compare_text.begin() + 2, compare_text.begin() + 9, TextSTD(STR("Replace")));
      compare_text.resize(std::min(compare_text.size(), SIZE));
      text.replace(text.begin() + 2, text.begin() + 9, TextL(STR("Replace")));

      is_equal = Equal(compare_text, text);
      CHECK(is_equal);
#if ETL_HAS_STRING_TRUNCATION_CHECKS
      CHECK_FALSE(text.is_truncated());
#endif

      // Overflow.
      compare_text.assign(initial_text.c_str());
      text.assign(initial_text.c_str());

      compare_text.replace(compare_text.begin() + 2, compare_text.begin() + 4, TextSTD(STR("Replace with some text")));
      compare_text.resize(std::min(compare_text.size(), SIZE));
      text.replace(text.begin() + 2, text.begin() + 4, TextL(STR("Replace with some text")));

      is_equal = Equal(compare_text, text);
      CHECK(is_equal);
#if ETL_HAS_STRING_TRUNCATION_CHECKS
      CHECK_TRUE(text.is_truncated());
#else
      CHECK_FALSE(text.is_truncated());
#endif
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_replace_first_last_view)
    {
      // Non-overflow short text.
      TextSTD compare_text(short_text.c_str());
      Text text(short_text.c_str());

      compare_text.replace(compare_text.begin() + 2, compare_text.begin() + 4, TextSTD(STR("Replace")));
      compare_text.resize(std::min(compare_text.size(), SIZE));
      text.replace(text.begin() + 2, text.begin() + 4, View(STR("Replace")));

      bool is_equal = Equal(compare_text, text);
      CHECK(is_equal);
#if ETL_HAS_STRING_TRUNCATION_CHECKS
      CHECK_FALSE(text.is_truncated());
#endif

      // Overflow short text.
      compare_text.assign(short_text.c_str());
      text.assign(short_text.c_str());

      compare_text.replace(compare_text.begin() + 2, compare_text.begin() + 4, TextSTD(STR("Replace with some text")));
      compare_text.resize(std::min(compare_text.size(), SIZE));
      text.replace(text.begin() + 2, text.begin() + 4, View(STR("Replace with some text")));

      is_equal = Equal(compare_text, text);
      CHECK(is_equal);
#if ETL_HAS_STRING_TRUNCATION_CHECKS
      CHECK_TRUE(text.is_truncated());
#else
      CHECK_FALSE(text.is_truncated());
#endif

      // Non-overflow.
      compare_text.assign(initial_text.c_str());
      text.assign(initial_text.c_str());

      compare_text.replace(compare_text.begin() + 2, compare_text.begin() + 9, TextSTD(STR("Replace")));
      compare_text.resize(std::min(compare_text.size(), SIZE));
      text.replace(text.begin() + 2, text.begin() + 9, View(STR("Replace")));

      is_equal = Equal(compare_text, text);
      CHECK(is_equal);
#if ETL_HAS_STRING_TRUNCATION_CHECKS
      CHECK_FALSE(text.is_truncated());
#endif

      // Overflow.
      compare_text.assign(initial_text.c_str());
      text.assign(initial_text.c_str());

      compare_text.replace(compare_text.begin() + 2, compare_text.begin() + 4, TextSTD(STR("Replace with some text")));
      compare_text.resize(std::min(compare_text.size(), SIZE));
      text.replace(text.begin() + 2, text.begin() + 4, View(STR("Replace with some text")));

      is_equal = Equal(compare_text, text);
      CHECK(is_equal);
#if ETL_HAS_STRING_TRUNCATION_CHECKS
      CHECK_TRUE(text.is_truncated());
#else
      CHECK_FALSE(text.is_truncated());
#endif
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_replace_position_length_string_subposition_sublength)
    {
      // Non-overflow short text.
      TextSTD compare_text(short_text.c_str());
      Text text(short_text.c_str());

      compare_text.replace(2, 4, TextSTD(STR("Replace")), 1, 5);
      compare_text.resize(std::min(compare_text.size(), SIZE));
      text.replace(2, 4, TextL(STR("Replace")), 1, 5);

      bool is_equal = Equal(compare_text, text);
      CHECK(is_equal);
#if ETL_HAS_STRING_TRUNCATION_CHECKS
      CHECK_FALSE(text.is_truncated());
#endif

      // Non-overflow short text, npos.
      compare_text.assign(short_text.c_str());
      text.assign(short_text.c_str());

      compare_text.replace(2, TextSTD::npos, TextSTD(STR("Replace")), 1, TextSTD::npos);
      compare_text.resize(std::min(compare_text.size(), SIZE));
      text.replace(2, Text::npos, TextL(STR("Replace")), 1, Text::npos);

      is_equal = Equal(compare_text, text);
      CHECK(is_equal);
#if ETL_HAS_STRING_TRUNCATION_CHECKS
      CHECK_FALSE(text.is_truncated());
#endif

      // Overflow short text.
      compare_text.assign(short_text.c_str());
      text.assign(short_text.c_str());

      compare_text.replace(2, 4, TextSTD(STR("Replace with some text")), 1, 15);
      compare_text.resize(std::min(compare_text.size(), SIZE));
      text.replace(2, 4, TextL(STR("Replace with some text")), 1, 15);

      is_equal = Equal(compare_text, text);
      CHECK(is_equal);
#if ETL_HAS_STRING_TRUNCATION_CHECKS
      CHECK_TRUE(text.is_truncated());
#else
      CHECK_FALSE(text.is_truncated());
#endif

      // Overflow short text, npos.
      compare_text.assign(short_text.c_str());
      text.assign(short_text.c_str());

      compare_text.replace(2, TextSTD::npos, TextSTD(STR("Replace with some text")), 1, TextSTD::npos);
      compare_text.resize(std::min(compare_text.size(), SIZE));
      text.replace(2, Text::npos, TextL(STR("Replace with some text")), 1, Text::npos);

      is_equal = Equal(compare_text, text);
      CHECK(is_equal);
#if ETL_HAS_STRING_TRUNCATION_CHECKS
      CHECK_TRUE(text.is_truncated());
#else
      CHECK_FALSE(text.is_truncated());
#endif

      // Non-overflow.
      compare_text.assign(initial_text.c_str());
      text.assign(initial_text.c_str());

      compare_text.replace(2, 7, TextSTD(STR("Replace")), 1, 5);
      compare_text.resize(std::min(compare_text.size(), SIZE));
      text.replace(2, 7, TextL(STR("Replace")), 1, 5);

      is_equal = Equal(compare_text, text);
      CHECK(is_equal);
#if ETL_HAS_STRING_TRUNCATION_CHECKS
      CHECK_FALSE(text.is_truncated());
#endif

      // Non-overflow, npos.
      compare_text.assign(initial_text.c_str());
      text.assign(initial_text.c_str());

      compare_text.replace(2, TextSTD::npos, TextSTD(STR("Replace")), 1, TextSTD::npos);
      compare_text.resize(std::min(compare_text.size(), SIZE));
      text.replace(2, Text::npos, TextL(STR("Replace")), 1, Text::npos);

      is_equal = Equal(compare_text, text);
      CHECK(is_equal);
#if ETL_HAS_STRING_TRUNCATION_CHECKS
      CHECK_FALSE(text.is_truncated());
#endif

      // Overflow.
      compare_text.assign(initial_text.c_str());
      text.assign(initial_text.c_str());

      compare_text.replace(2, 4, TextSTD(STR("Replace with some text")), 1, 15);
      compare_text.resize(std::min(compare_text.size(), SIZE));
      text.replace(2, 4, TextL(STR("Replace with some text")), 1, 15);

      is_equal = Equal(compare_text, text);
      CHECK(is_equal);
#if ETL_HAS_STRING_TRUNCATION_CHECKS
      CHECK_TRUE(text.is_truncated());
#else
      CHECK_FALSE(text.is_truncated());
#endif

      // Overflow, npos.
      compare_text.assign(initial_text.c_str());
      text.assign(initial_text.c_str());

      compare_text.replace(2, TextSTD::npos, TextSTD(STR("Replace with some text")), 1, TextSTD::npos);
      compare_text.resize(std::min(compare_text.size(), SIZE));
      text.replace(2, Text::npos, TextL(STR("Replace with some text")), 1, Text::npos);

      is_equal = Equal(compare_text, text);
      CHECK(is_equal);
#if ETL_HAS_STRING_TRUNCATION_CHECKS
      CHECK_TRUE(text.is_truncated());
#else
      CHECK_FALSE(text.is_truncated());
#endif
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_replace_position_length_view_subposition_sublength)
    {
      // Non-overflow short text.
      TextSTD compare_text(short_text.c_str());
      Text text(short_text.c_str());

      compare_text.replace(2, 4, TextSTD(STR("Replace")), 1, 5);
      compare_text.resize(std::min(compare_text.size(), SIZE));
      text.replace(2, 4, View(STR("Replace")), 1, 5);

      bool is_equal = Equal(compare_text, text);
      CHECK(is_equal);
#if ETL_HAS_STRING_TRUNCATION_CHECKS
      CHECK_FALSE(text.is_truncated());
#endif

      // Non-overflow short text, npos.
      compare_text.assign(short_text.c_str());
      text.assign(short_text.c_str());

      compare_text.replace(2, TextSTD::npos, TextSTD(STR("Replace")), 1, TextSTD::npos);
      compare_text.resize(std::min(compare_text.size(), SIZE));
      text.replace(2, Text::npos, View(STR("Replace")), 1, Text::npos);

      is_equal = Equal(compare_text, text);
      CHECK(is_equal);
#if ETL_HAS_STRING_TRUNCATION_CHECKS
      CHECK_FALSE(text.is_truncated());
#endif

      // Overflow short text.
      compare_text.assign(short_text.c_str());
      text.assign(short_text.c_str());

      compare_text.replace(2, 4, TextSTD(STR("Replace with some text")), 1, 15);
      compare_text.resize(std::min(compare_text.size(), SIZE));
      text.replace(2, 4, View(STR("Replace with some text")), 1, 15);

      is_equal = Equal(compare_text, text);
      CHECK(is_equal);
#if ETL_HAS_STRING_TRUNCATION_CHECKS
      CHECK_TRUE(text.is_truncated());
#else
      CHECK_FALSE(text.is_truncated());
#endif

      // Overflow short text, npos.
      compare_text.assign(short_text.c_str());
      text.assign(short_text.c_str());

      compare_text.replace(2, TextSTD::npos, TextSTD(STR("Replace with some text")), 1, TextSTD::npos);
      compare_text.resize(std::min(compare_text.size(), SIZE));
      text.replace(2, Text::npos, View(STR("Replace with some text")), 1, Text::npos);

      is_equal = Equal(compare_text, text);
      CHECK(is_equal);
#if ETL_HAS_STRING_TRUNCATION_CHECKS
      CHECK_TRUE(text.is_truncated());
#else
      CHECK_FALSE(text.is_truncated());
#endif

      // Non-overflow.
      compare_text.assign(initial_text.c_str());
      text.assign(initial_text.c_str());

      compare_text.replace(2, 7, TextSTD(STR("Replace")), 1, 5);
      compare_text.resize(std::min(compare_text.size(), SIZE));
      text.replace(2, 7, View(STR("Replace")), 1, 5);

      is_equal = Equal(compare_text, text);
      CHECK(is_equal);
#if ETL_HAS_STRING_TRUNCATION_CHECKS
      CHECK_FALSE(text.is_truncated());
#endif

      // Non-overflow, npos.
      compare_text.assign(initial_text.c_str());
      text.assign(initial_text.c_str());

      compare_text.replace(2, TextSTD::npos, TextSTD(STR("Replace")), 1, TextSTD::npos);
      compare_text.resize(std::min(compare_text.size(), SIZE));
      text.replace(2, Text::npos, View(STR("Replace")), 1, Text::npos);

      is_equal = Equal(compare_text, text);
      CHECK(is_equal);
#if ETL_HAS_STRING_TRUNCATION_CHECKS
      CHECK_FALSE(text.is_truncated());
#endif

      // Overflow.
      compare_text.assign(initial_text.c_str());
      text.assign(initial_text.c_str());

      compare_text.replace(2, 4, TextSTD(STR("Replace with some text")), 1, 15);
      compare_text.resize(std::min(compare_text.size(), SIZE));
      text.replace(2, 4, View(STR("Replace with some text")), 1, 15);

      is_equal = Equal(compare_text, text);
      CHECK(is_equal);
#if ETL_HAS_STRING_TRUNCATION_CHECKS
      CHECK_TRUE(text.is_truncated());
#else
      CHECK_FALSE(text.is_truncated());
#endif

      // Overflow, npos.
      compare_text.assign(initial_text.c_str());
      text.assign(initial_text.c_str());

      compare_text.replace(2, TextSTD::npos, TextSTD(STR("Replace with some text")), 1, TextSTD::npos);
      compare_text.resize(std::min(compare_text.size(), SIZE));
      text.replace(2, Text::npos, View(STR("Replace with some text")), 1, Text::npos);

      is_equal = Equal(compare_text, text);
      CHECK(is_equal);
#if ETL_HAS_STRING_TRUNCATION_CHECKS
      CHECK_TRUE(text.is_truncated());
#else
      CHECK_FALSE(text.is_truncated());
#endif
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_replace_position_length_pointer)
    {
      // Non-overflow short text.
      TextSTD compare_text(short_text.c_str());
      Text text(short_text.c_str());

      compare_text.replace(2, 4, TextSTD(STR("Replace")).c_str());
      compare_text.resize(std::min(compare_text.size(), SIZE));
      text.replace(2, 4, TextL(STR("Replace")).c_str());

      bool is_equal = Equal(compare_text, text);
      CHECK(is_equal);
#if ETL_HAS_STRING_TRUNCATION_CHECKS
      CHECK_FALSE(text.is_truncated());
#endif

      // Non-overflow short text, npos.
      compare_text.assign(short_text.c_str());
      text.assign(short_text.c_str());

      compare_text.replace(2, TextSTD::npos, TextSTD(STR("Replace")).c_str());
      compare_text.resize(std::min(compare_text.size(), SIZE));
      text.replace(2, Text::npos, TextL(STR("Replace")).c_str());

      is_equal = Equal(compare_text, text);
      CHECK(is_equal);
#if ETL_HAS_STRING_TRUNCATION_CHECKS
      CHECK_FALSE(text.is_truncated());
#endif

      // Overflow short text.
      compare_text.assign(short_text.c_str());
      text.assign(short_text.c_str());

      compare_text.replace(2, 4, TextSTD(STR("Replace with some text")).c_str());
      compare_text.resize(std::min(compare_text.size(), SIZE));
      text.replace(2, 4, TextL(STR("Replace with some text")).c_str());

      is_equal = Equal(compare_text, text);
      CHECK(is_equal);
#if ETL_HAS_STRING_TRUNCATION_CHECKS
      CHECK_TRUE(text.is_truncated());
#else
      CHECK_FALSE(text.is_truncated());
#endif

      // Overflow short text, npos.
      compare_text.assign(short_text.c_str());
      text.assign(short_text.c_str());

      compare_text.replace(2, TextSTD::npos, TextSTD(STR("Replace with some text")).c_str());
      compare_text.resize(std::min(compare_text.size(), SIZE));
      text.replace(2, Text::npos, TextL(STR("Replace with some text")).c_str());

      is_equal = Equal(compare_text, text);
      CHECK(is_equal);
#if ETL_HAS_STRING_TRUNCATION_CHECKS
      CHECK_TRUE(text.is_truncated());
#else
      CHECK_FALSE(text.is_truncated());
#endif

      // Non-overflow.
      compare_text.assign(initial_text.c_str());
      text.assign(initial_text.c_str());

      compare_text.replace(2, 7, TextSTD(STR("Replace")).c_str());
      compare_text.resize(std::min(compare_text.size(), SIZE));
      text.replace(2, 7, TextL(STR("Replace")).c_str());

      is_equal = Equal(compare_text, text);
      CHECK(is_equal);
#if ETL_HAS_STRING_TRUNCATION_CHECKS
      CHECK_FALSE(text.is_truncated());
#endif

      // Non-overflow, npos.
      compare_text.assign(initial_text.c_str());
      text.assign(initial_text.c_str());

      compare_text.replace(2, TextSTD::npos, TextSTD(STR("Replace")).c_str());
      compare_text.resize(std::min(compare_text.size(), SIZE));
      text.replace(2, Text::npos, TextL(STR("Replace")).c_str());

      is_equal = Equal(compare_text, text);
      CHECK(is_equal);
#if ETL_HAS_STRING_TRUNCATION_CHECKS
      CHECK_FALSE(text.is_truncated());
#endif

      // Overflow.
      compare_text.assign(initial_text.c_str());
      text.assign(initial_text.c_str());

      compare_text.replace(2, 4, TextSTD(STR("Replace with some text")).c_str());
      compare_text.resize(std::min(compare_text.size(), SIZE));
      text.replace(2, 4, TextL(STR("Replace with some text")).c_str());

      is_equal = Equal(compare_text, text);
      CHECK(is_equal);
#if ETL_HAS_STRING_TRUNCATION_CHECKS
      CHECK_TRUE(text.is_truncated());
#else
      CHECK_FALSE(text.is_truncated());
#endif

      // Overflow, npos.
      compare_text.assign(initial_text.c_str());
      text.assign(initial_text.c_str());

      compare_text.replace(2, TextSTD::npos, TextSTD(STR("Replace with some text")).c_str());
      compare_text.resize(std::min(compare_text.size(), SIZE));
      text.replace(2, Text::npos, TextL(STR("Replace with some text")).c_str());

      is_equal = Equal(compare_text, text);
      CHECK(is_equal);
#if ETL_HAS_STRING_TRUNCATION_CHECKS
      CHECK_TRUE(text.is_truncated());
#else
      CHECK_FALSE(text.is_truncated());
#endif
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_replace_first_last_pointer)
    {
      // Non-overflow short text.
      TextSTD compare_text(short_text.c_str());
      Text text(short_text.c_str());

      compare_text.replace(compare_text.begin() + 2, compare_text.begin() + 4, TextSTD(STR("Replace")).c_str());
      compare_text.resize(std::min(compare_text.size(), SIZE));
      text.replace(text.begin() + 2, text.begin() + 4, TextL(STR("Replace")).c_str());

      bool is_equal = Equal(compare_text, text);
      CHECK(is_equal);
#if ETL_HAS_STRING_TRUNCATION_CHECKS
      CHECK_FALSE(text.is_truncated());
#endif

      // Overflow short text.
      compare_text.assign(short_text.c_str());
      text.assign(short_text.c_str());

      compare_text.replace(compare_text.begin() + 2, compare_text.begin() + 4, TextSTD(STR("Replace with some text")).c_str());
      compare_text.resize(std::min(compare_text.size(), SIZE));
      text.replace(text.begin() + 2, text.begin() + 4, TextL(STR("Replace with some text")).c_str());

      is_equal = Equal(compare_text, text);
      CHECK(is_equal);
#if ETL_HAS_STRING_TRUNCATION_CHECKS
      CHECK_TRUE(text.is_truncated());
#else
      CHECK_FALSE(text.is_truncated());
#endif

      // Non-overflow.
      compare_text.assign(initial_text.c_str());
      text.assign(initial_text.c_str());

      compare_text.replace(compare_text.begin() + 2, compare_text.begin() + 9, TextSTD(STR("Replace")).c_str());
      compare_text.resize(std::min(compare_text.size(), SIZE));
      text.replace(text.begin() + 2, text.begin() + 9, TextL(STR("Replace")).c_str());

      is_equal = Equal(compare_text, text);
      CHECK(is_equal);
#if ETL_HAS_STRING_TRUNCATION_CHECKS
      CHECK_FALSE(text.is_truncated());
#endif

      // Overflow.
      compare_text.assign(initial_text.c_str());
      text.assign(initial_text.c_str());

      compare_text.replace(compare_text.begin() + 2, compare_text.begin() + 4, TextSTD(STR("Replace with some text")).c_str());
      compare_text.resize(std::min(compare_text.size(), SIZE));
      text.replace(text.begin() + 2, text.begin() + 4, TextL(STR("Replace with some text")).c_str());

      is_equal = Equal(compare_text, text);
      CHECK(is_equal);
#if ETL_HAS_STRING_TRUNCATION_CHECKS
      CHECK_TRUE(text.is_truncated());
#else
      CHECK_FALSE(text.is_truncated());
#endif
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_replace_position_length_pointer_n)
    {
      // Non-overflow short text.
      TextSTD compare_text(short_text.c_str());
      Text text(short_text.c_str());

      compare_text.replace(2, 4, TextSTD(STR("Replace")).c_str(), 5);
      compare_text.resize(std::min(compare_text.size(), SIZE));
      text.replace(2, 4, TextL(STR("Replace")).c_str(), 5);

      bool is_equal = Equal(compare_text, text);
      CHECK(is_equal);
#if ETL_HAS_STRING_TRUNCATION_CHECKS
      CHECK_FALSE(text.is_truncated());
#endif

      // Non-overflow short text, npos.
      compare_text.assign(short_text.c_str());
      text.assign(short_text.c_str());

      compare_text.replace(2, TextSTD::npos, TextSTD(STR("Replace")).c_str(), 5);
      compare_text.resize(std::min(compare_text.size(), SIZE));
      text.replace(2, Text::npos, TextL(STR("Replace")).c_str(), 5);

      is_equal = Equal(compare_text, text);
      CHECK(is_equal);
#if ETL_HAS_STRING_TRUNCATION_CHECKS
      CHECK_FALSE(text.is_truncated());
#endif

      // Overflow short text.
      compare_text.assign(short_text.c_str());
      text.assign(short_text.c_str());

      compare_text.replace(2, 4, TextSTD(STR("Replace with some text")).c_str(), 15);
      compare_text.resize(std::min(compare_text.size(), SIZE));
      text.replace(2, 4, TextL(STR("Replace with some text")).c_str(), 15);

      is_equal = Equal(compare_text, text);
      CHECK(is_equal);
#if ETL_HAS_STRING_TRUNCATION_CHECKS
      CHECK_TRUE(text.is_truncated());
#else
      CHECK_FALSE(text.is_truncated());
#endif

      // Overflow short text, npos.
      compare_text.assign(short_text.c_str());
      text.assign(short_text.c_str());

      compare_text.replace(2, TextSTD::npos, TextSTD(STR("Replace with some text")).c_str(), 15);
      compare_text.resize(std::min(compare_text.size(), SIZE));
      text.replace(2, Text::npos, TextL(STR("Replace with some text")).c_str(), 15);

      is_equal = Equal(compare_text, text);
      CHECK(is_equal);
#if ETL_HAS_STRING_TRUNCATION_CHECKS
      CHECK_TRUE(text.is_truncated());
#else
      CHECK_FALSE(text.is_truncated());
#endif

      // Non-overflow.
      compare_text.assign(initial_text.c_str());
      text.assign(initial_text.c_str());

      compare_text.replace(2, 7, TextSTD(STR("Replace")).c_str(), 5);
      compare_text.resize(std::min(compare_text.size(), SIZE));
      text.replace(2, 7, TextL(STR("Replace")).c_str(), 5);

      is_equal = Equal(compare_text, text);
      CHECK(is_equal);
#if ETL_HAS_STRING_TRUNCATION_CHECKS
      CHECK_FALSE(text.is_truncated());
#endif

      // Non-overflow, npos.
      compare_text.assign(initial_text.c_str());
      text.assign(initial_text.c_str());

      compare_text.replace(2, TextSTD::npos, TextSTD(STR("Replace")).c_str(), 5);
      compare_text.resize(std::min(compare_text.size(), SIZE));
      text.replace(2, Text::npos, TextL(STR("Replace")).c_str(), 5);

      is_equal = Equal(compare_text, text);
      CHECK(is_equal);
#if ETL_HAS_STRING_TRUNCATION_CHECKS
      CHECK_FALSE(text.is_truncated());
#endif

      // Overflow.
      compare_text.assign(initial_text.c_str());
      text.assign(initial_text.c_str());

      compare_text.replace(2, 4, TextSTD(STR("Replace with some text")).c_str(), 15);
      compare_text.resize(std::min(compare_text.size(), SIZE));
      text.replace(2, 4, TextL(STR("Replace with some text")).c_str(), 15);

      is_equal = Equal(compare_text, text);
      CHECK(is_equal);
#if ETL_HAS_STRING_TRUNCATION_CHECKS
      CHECK_TRUE(text.is_truncated());
#else
      CHECK_FALSE(text.is_truncated());
#endif

      // Overflow, npos.
      compare_text.assign(initial_text.c_str());
      text.assign(initial_text.c_str());

      compare_text.replace(2, TextSTD::npos, TextSTD(STR("Replace with some text")).c_str(), 15);
      compare_text.resize(std::min(compare_text.size(), SIZE));
      text.replace(2, Text::npos, TextL(STR("Replace with some text")).c_str(), 15);

      is_equal = Equal(compare_text, text);
      CHECK(is_equal);
#if ETL_HAS_STRING_TRUNCATION_CHECKS
      CHECK_TRUE(text.is_truncated());
#else
      CHECK_FALSE(text.is_truncated());
#endif
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_replace_first_last_pointer_n)
    {
      // Non-overflow short text.
      TextSTD compare_text(short_text.c_str());
      Text text(short_text.c_str());

      compare_text.replace(compare_text.begin() + 2, compare_text.begin() + 4, TextSTD(STR("Replace")).c_str(), 5);
      compare_text.resize(std::min(compare_text.size(), SIZE));
      text.replace(text.begin() + 2, text.begin() + 4, TextL(STR("Replace")).c_str(), 5);

      bool is_equal = Equal(compare_text, text);
      CHECK(is_equal);
#if ETL_HAS_STRING_TRUNCATION_CHECKS
      CHECK_FALSE(text.is_truncated());
#endif

#include "etl/private/diagnostic_array_bounds_push.h"
#include "etl/private/diagnostic_stringop_overread_push.h"
      // Overflow short text.
      compare_text.assign(short_text.c_str());
      text.assign(short_text.c_str());

      compare_text.replace(compare_text.begin() + 2, compare_text.begin() + 4, TextSTD(STR("Replace with some text")).c_str(), 15);
      compare_text.resize(std::min(compare_text.size(), SIZE));
      text.replace(text.begin() + 2, text.begin() + 4, TextL(STR("Replace with some text")).c_str(), 15);

      is_equal = Equal(compare_text, text);
      CHECK(is_equal);
#if ETL_HAS_STRING_TRUNCATION_CHECKS
      CHECK_TRUE(text.is_truncated());
#else
      CHECK_FALSE(text.is_truncated());
#endif
#include "etl/private/diagnostic_pop.h"
#include "etl/private/diagnostic_pop.h"

      // Non-overflow.
      compare_text.assign(initial_text.c_str());
      text.assign(initial_text.c_str());

      compare_text.replace(compare_text.begin() + 2, compare_text.begin() + 9, TextSTD(STR("Replace")).c_str(), 5);
      compare_text.resize(std::min(compare_text.size(), SIZE));
      text.replace(text.begin() + 2, text.begin() + 9, TextL(STR("Replace")).c_str(), 5);

      is_equal = Equal(compare_text, text);
      CHECK(is_equal);
#if ETL_HAS_STRING_TRUNCATION_CHECKS
      CHECK_FALSE(text.is_truncated());
#endif

#include "etl/private/diagnostic_array_bounds_push.h"
#include "etl/private/diagnostic_stringop_overread_push.h"
      // Overflow.
      compare_text.assign(initial_text.c_str());
      text.assign(initial_text.c_str());

      compare_text.replace(compare_text.begin() + 2, compare_text.begin() + 4, TextSTD(STR("Replace with some text")).c_str(), 15);
      compare_text.resize(std::min(compare_text.size(), SIZE));
      text.replace(text.begin() + 2, text.begin() + 4, TextL(STR("Replace with some text")).c_str(), 15);

      is_equal = Equal(compare_text, text);
      CHECK(is_equal);
#if ETL_HAS_STRING_TRUNCATION_CHECKS
      CHECK_TRUE(text.is_truncated());
#else
      CHECK_FALSE(text.is_truncated());
#endif
#include "etl/private/diagnostic_pop.h"
#include "etl/private/diagnostic_pop.h"
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_replace_position_length_n_c)
    {
      // Non-overflow short text.
      TextSTD compare_text(short_text.c_str());
      Text text(short_text.c_str());

      compare_text.replace(2, 4, 7, STR('A'));
      compare_text.resize(std::min(compare_text.size(), SIZE));
      text.replace(2, 4, 7, STR('A'));

      bool is_equal = Equal(compare_text, text);
      CHECK(is_equal);
#if ETL_HAS_STRING_TRUNCATION_CHECKS
      CHECK_FALSE(text.is_truncated());
#endif

      // Non-overflow short text, npos.
      compare_text.assign(short_text.c_str());
      text.assign(short_text.c_str());

      compare_text.replace(2, TextSTD::npos, 7, STR('A'));
      compare_text.resize(std::min(compare_text.size(), SIZE));
      text.replace(2, Text::npos, 7, STR('A'));

      is_equal = Equal(compare_text, text);
      CHECK(is_equal);
#if ETL_HAS_STRING_TRUNCATION_CHECKS
      CHECK_FALSE(text.is_truncated());
#endif

      // Overflow short text.
      compare_text.assign(short_text.c_str());
      text.assign(short_text.c_str());

      compare_text.replace(2, 4, 15, STR('A'));
      compare_text.resize(std::min(compare_text.size(), SIZE));
      text.replace(2, 4, 15, STR('A'));

      is_equal = Equal(compare_text, text);
      CHECK(is_equal);
#if ETL_HAS_STRING_TRUNCATION_CHECKS
      CHECK_TRUE(text.is_truncated());
#else
      CHECK_FALSE(text.is_truncated());
#endif

      // Overflow short text, npos.
      compare_text.assign(short_text.c_str());
      text.assign(short_text.c_str());

      compare_text.replace(2, TextSTD::npos, 15, STR('A'));
      compare_text.resize(std::min(compare_text.size(), SIZE));
      text.replace(2, Text::npos, 15, STR('A'));

      is_equal = Equal(compare_text, text);
      CHECK(is_equal);
#if ETL_HAS_STRING_TRUNCATION_CHECKS
      CHECK_TRUE(text.is_truncated());
#else
      CHECK_FALSE(text.is_truncated());
#endif

      // Non-overflow.
      compare_text.assign(initial_text.c_str());
      text.assign(initial_text.c_str());

      compare_text.replace(2, 7, 7, STR('A'));
      compare_text.resize(std::min(compare_text.size(), SIZE));
      text.replace(2, 7, 7, STR('A'));

      is_equal = Equal(compare_text, text);
      CHECK(is_equal);
#if ETL_HAS_STRING_TRUNCATION_CHECKS
      CHECK_FALSE(text.is_truncated());
#endif

      // Non-overflow, npos.
      compare_text.assign(initial_text.c_str());
      text.assign(initial_text.c_str());

      compare_text.replace(2, TextSTD::npos, 7, STR('A'));
      compare_text.resize(std::min(compare_text.size(), SIZE));
      text.replace(2, Text::npos, 7, STR('A'));

      is_equal = Equal(compare_text, text);
      CHECK(is_equal);
#if ETL_HAS_STRING_TRUNCATION_CHECKS
      CHECK_FALSE(text.is_truncated());
#endif

      // Overflow.
      compare_text.assign(initial_text.c_str());
      text.assign(initial_text.c_str());

      compare_text.replace(2, 4, 15, STR('A'));
      compare_text.resize(std::min(compare_text.size(), SIZE));
      text.replace(2, 4, 15, STR('A'));

      is_equal = Equal(compare_text, text);
      CHECK(is_equal);
#if ETL_HAS_STRING_TRUNCATION_CHECKS
      CHECK_TRUE(text.is_truncated());
#else
      CHECK_FALSE(text.is_truncated());
#endif

      // Overflow, npos.
      compare_text.assign(initial_text.c_str());
      text.assign(initial_text.c_str());

      compare_text.replace(2, TextSTD::npos, 15, STR('A'));
      compare_text.resize(std::min(compare_text.size(), SIZE));
      text.replace(2, Text::npos, 15, STR('A'));

      is_equal = Equal(compare_text, text);
      CHECK(is_equal);
#if ETL_HAS_STRING_TRUNCATION_CHECKS
      CHECK_TRUE(text.is_truncated());
#else
      CHECK_FALSE(text.is_truncated());
#endif
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_replace_first_last_n_c)
    {
      // Non-overflow short text.
      TextSTD compare_text(short_text.c_str());
      Text text(short_text.c_str());

      compare_text.replace(compare_text.begin() + 2, compare_text.begin() + 4, 7, STR('A'));
      compare_text.resize(std::min(compare_text.size(), SIZE));
      text.replace(text.begin() + 2, text.begin() + 4, 7, STR('A'));

      bool is_equal = Equal(compare_text, text);
      CHECK(is_equal);
#if ETL_HAS_STRING_TRUNCATION_CHECKS
      CHECK_FALSE(text.is_truncated());
#endif

      // Overflow short text.
      compare_text.assign(short_text.c_str());
      text.assign(short_text.c_str());

      compare_text.replace(compare_text.begin() + 2, compare_text.begin() + 4, 15, STR('A'));
      compare_text.resize(std::min(compare_text.size(), SIZE));
      text.replace(text.begin() + 2, text.begin() + 4, 15, STR('A'));

      is_equal = Equal(compare_text, text);
      CHECK(is_equal);
#if ETL_HAS_STRING_TRUNCATION_CHECKS
      CHECK_TRUE(text.is_truncated());
#else
      CHECK_FALSE(text.is_truncated());
#endif

      // Non-overflow.
      compare_text.assign(initial_text.c_str());
      text.assign(initial_text.c_str());

      compare_text.replace(compare_text.begin() + 2, compare_text.begin() + 9, 7, STR('A'));
      compare_text.resize(std::min(compare_text.size(), SIZE));
      text.replace(text.begin() + 2, text.begin() + 9, 7, STR('A'));

      is_equal = Equal(compare_text, text);
      CHECK(is_equal);
#if ETL_HAS_STRING_TRUNCATION_CHECKS
      CHECK_FALSE(text.is_truncated());
#endif

      // Overflow.
      compare_text.assign(initial_text.c_str());
      text.assign(initial_text.c_str());

      compare_text.replace(compare_text.begin() + 2, compare_text.begin() + 4, 15, STR('A'));
      compare_text.resize(std::min(compare_text.size(), SIZE));
      text.replace(text.begin() + 2, text.begin() + 4, 15, STR('A'));

      is_equal = Equal(compare_text, text);
      CHECK(is_equal);
#if ETL_HAS_STRING_TRUNCATION_CHECKS
      CHECK_TRUE(text.is_truncated());
#else
      CHECK_FALSE(text.is_truncated());
#endif
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_replace_first_last_first_last)
    {
      // Non-overflow short text.
      TextSTD compare_text(short_text.c_str());
      Text text(short_text.c_str());

      TextSTD replace(STR("Replace"));
      TextSTD replace_long(STR("Replace with some text"));

      compare_text.replace(compare_text.begin() + 2, compare_text.begin() + 4, replace.begin() + 1, replace.begin() + 5);
      compare_text.resize(std::min(compare_text.size(), SIZE));
      text.replace(text.begin() + 2, text.begin() + 4, replace.begin() + 1, replace.begin() + 5);

      bool is_equal = Equal(compare_text, text);
      CHECK(is_equal);
#if ETL_HAS_STRING_TRUNCATION_CHECKS
      CHECK_FALSE(text.is_truncated());
#endif

      // Overflow short text.
      compare_text.assign(short_text.c_str());
      text.assign(short_text.c_str());

      compare_text.replace(compare_text.begin() + 2, compare_text.begin() + 4, replace_long.begin() + 1, replace_long.begin() + 15);
      compare_text.resize(std::min(compare_text.size(), SIZE));
      text.replace(text.begin() + 2, text.begin() + 4, replace_long.begin() + 1, replace_long.begin() + 15);

      is_equal = Equal(compare_text, text);
      CHECK(is_equal);
#if ETL_HAS_STRING_TRUNCATION_CHECKS
      CHECK_TRUE(text.is_truncated());
#else
      CHECK_FALSE(text.is_truncated());
#endif

      // Non-overflow.
      compare_text.assign(initial_text.c_str());
      text.assign(initial_text.c_str());

      compare_text.replace(compare_text.begin() + 2, compare_text.begin() + 7, replace.begin() + 1, replace.begin() + 5);
      compare_text.resize(std::min(compare_text.size(), SIZE));
      text.replace(text.begin() + 2, text.begin() + 7, replace_long.begin() + 1, replace_long.begin() + 5);

      is_equal = Equal(compare_text, text);
      CHECK(is_equal);
#if ETL_HAS_STRING_TRUNCATION_CHECKS
      CHECK_FALSE(text.is_truncated());
#endif

      // Overflow.
      compare_text.assign(initial_text.c_str());
      text.assign(initial_text.c_str());

      compare_text.replace(compare_text.begin() + 2, compare_text.begin() + 4, replace_long.begin() + 1, replace_long.begin() + 15);
      compare_text.resize(std::min(compare_text.size(), SIZE));
      text.replace(text.begin() + 2, text.begin() + 4, replace_long.begin() + 1, replace_long.begin() + 15);

      is_equal = Equal(compare_text, text);
      CHECK(is_equal);
#if ETL_HAS_STRING_TRUNCATION_CHECKS
      CHECK_TRUE(text.is_truncated());
#else
      CHECK_FALSE(text.is_truncated());
#endif
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_erase_single_iterator)
    {
      TextSTD compare_text(initial_text.c_str());
      Text text(initial_text.c_str());

      TextSTD::iterator citr = compare_text.erase(compare_text.begin() + 2);
      Text::iterator ditr = text.erase(text.begin() + 2);
      CHECK(*citr == *ditr);

      bool is_equal = Equal(compare_text, text);
      CHECK(is_equal);
#if ETL_HAS_STRING_TRUNCATION_CHECKS
      CHECK_FALSE(text.is_truncated());
#endif
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_erase_single_const_iterator)
    {
      TextSTD compare_text(initial_text.c_str());
      Text text(initial_text.c_str());

      TextSTD::iterator citr = compare_text.erase(compare_text.cbegin() + 2);
      Text::iterator ditr = text.erase(text.cbegin() + 2);
      CHECK(*citr == *ditr);

      bool is_equal = Equal(compare_text, text);
      CHECK(is_equal);
#if ETL_HAS_STRING_TRUNCATION_CHECKS
      CHECK_FALSE(text.is_truncated());
#endif
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_erase_range)
    {
      TextSTD compare_text(initial_text.c_str());
      Text text(initial_text.c_str());

      TextSTD::iterator citr = compare_text.erase(compare_text.cbegin() + 2, compare_text.cbegin() + 4);
      Text::iterator ditr = text.erase(text.cbegin() + 2, text.cbegin() + 4);
      CHECK(*citr == *ditr);

      bool is_equal = Equal(compare_text, text);
      CHECK(is_equal);
#if ETL_HAS_STRING_TRUNCATION_CHECKS
      CHECK_FALSE(text.is_truncated());
#endif
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_clear)
    {
      Text text(initial_text.c_str());
      text.clear();

      CHECK_EQUAL(text.size(), size_t(0));
#if ETL_HAS_STRING_TRUNCATION_CHECKS
      CHECK_FALSE(text.is_truncated());
#endif
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_iterator)
    {
      TextSTD compare_text(initial_text.c_str());
      Text text(initial_text.c_str());

      bool is_equal = std::equal(text.begin(), text.end(), compare_text.begin());
      CHECK(is_equal);
#if ETL_HAS_STRING_TRUNCATION_CHECKS
      CHECK_FALSE(text.is_truncated());
#endif
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_const_iterator)
    {
      TextSTD compare_text(initial_text.c_str());
      Text text(initial_text.c_str());

      bool is_equal = std::equal(text.cbegin(), text.cend(), compare_text.cbegin());
      CHECK(is_equal);
#if ETL_HAS_STRING_TRUNCATION_CHECKS
      CHECK_FALSE(text.is_truncated());
#endif
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_reverse_iterator)
    {
      TextSTD compare_text(initial_text.c_str());
      Text text(initial_text.c_str());

      bool is_equal = std::equal(text.rbegin(), text.rend(), compare_text.rbegin());
      CHECK(is_equal);
#if ETL_HAS_STRING_TRUNCATION_CHECKS
      CHECK_FALSE(text.is_truncated());
#endif
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_const_reverse_iterator)
    {
      TextSTD compare_text(initial_text.c_str());
      Text text(initial_text.c_str());

      bool is_equal = std::equal(text.crbegin(), text.crend(), compare_text.crbegin());
      CHECK(is_equal);
#if ETL_HAS_STRING_TRUNCATION_CHECKS
      CHECK_FALSE(text.is_truncated());
#endif
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_equal)
    {
      const Text initial1(initial_text.c_str());
      const Text initial2(initial_text.c_str());

      CHECK(initial1 == initial2);

      const Text different(different_text.c_str());

      CHECK(!(initial1 == different));

      const Text shorter(shorter_text.c_str());

      CHECK(!(shorter == initial1));
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_not_equal)
    {
      const Text initial1(initial_text.c_str());
      const Text initial2(initial_text.c_str());

      CHECK(!(initial1 != initial2));

      const Text different(different_text.begin(), different_text.end());

      CHECK(initial1 != different);

      const Text shorter(shorter_text.begin(), shorter_text.end());

      CHECK(shorter != initial1);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_less_than)
    {
      const Text less(less_text.c_str());
      const Text initial(initial_text.c_str());

      // String-String
      CHECK((less    < initial) == (less_text < initial_text));
      CHECK((initial < less) == (initial_text < less_text));

      const Text greater(greater_text.c_str());
      CHECK((greater < initial) == (greater_text < initial_text));
      CHECK((initial < greater) == (initial_text < greater_text));

      const Text shorter(shorter_text.c_str());
      CHECK((shorter < initial) == (shorter_text < initial_text));
      CHECK((initial < shorter) == (initial_text < shorter_text));

      CHECK((initial < initial) == (initial_text < initial_text));
      CHECK((initial < initial) == (initial_text < initial_text));

      // String-Pointer Pointer-String
      CHECK((less          < pinitial_text) == (less_text     < pinitial_text));
      CHECK((pinitial_text < less)          == (pinitial_text < less_text));

      CHECK((greater       < pinitial_text) == (greater_text  < pinitial_text));
      CHECK((pinitial_text < greater)       == (pinitial_text < greater_text));

      CHECK((shorter       < pinitial_text) == (shorter_text  < pinitial_text));
      CHECK((pinitial_text < shorter)       == (pinitial_text < shorter_text));

      CHECK((initial       < pinitial_text) == (initial_text  < pinitial_text));
      CHECK((pinitial_text < initial)       == (pinitial_text < initial_text));
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_less_than_or_equal)
    {
      const Text less(less_text.c_str());
      const Text initial(initial_text.c_str());

      // String-String
      CHECK((less    <= initial) == (less_text    <= initial_text));
      CHECK((initial <= less)    == (initial_text <= less_text));

      const Text greater(greater_text.c_str());
      CHECK((greater <= initial) == (greater_text <= initial_text));
      CHECK((initial <= greater) == (initial_text <= greater_text));

      const Text shorter(shorter_text.c_str());
      CHECK((shorter <= initial) == (shorter_text <= initial_text));
      CHECK((initial <= shorter) == (initial_text <= shorter_text));

      CHECK((initial <= initial) == (initial_text <= initial_text));
      CHECK((initial <= initial) == (initial_text <= initial_text));

      // String-Pointer Pointer-String
      CHECK((less          <= pinitial_text) == (less_text     <= pinitial_text));
      CHECK((pinitial_text <= less)          == (pinitial_text <= less_text));

      CHECK((greater       <= pinitial_text) == (greater_text  <= pinitial_text));
      CHECK((pinitial_text <= greater)       == (pinitial_text <= greater_text));

      CHECK((shorter       <= pinitial_text) == (shorter_text  <= pinitial_text));
      CHECK((pinitial_text <= shorter)       == (pinitial_text <= shorter_text));

      CHECK((initial       <= pinitial_text) == (initial_text  <= pinitial_text));
      CHECK((pinitial_text <= initial)       == (pinitial_text <= initial_text));
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_greater_than)
    {
      const Text less(less_text.c_str());
      const Text initial(initial_text.c_str());

      // String-String
      CHECK((less > initial) == (less_text > initial_text));
      CHECK((initial > less) == (initial_text > less_text));

      const Text greater(greater_text.c_str());
      CHECK((greater > initial) == (greater_text > initial_text));
      CHECK((initial > greater) == (initial_text > greater_text));

      const Text shorter(shorter_text.c_str());
      CHECK((shorter > initial) == (shorter_text > initial_text));
      CHECK((initial > shorter) == (initial_text > shorter_text));

      CHECK((initial > initial) == (initial_text > initial_text));
      CHECK((initial > initial) == (initial_text > initial_text));

      // String-Pointer Pointer-String
      CHECK((less          > pinitial_text) == (less_text     > pinitial_text));
      CHECK((pinitial_text > less)          == (pinitial_text > less_text));

      CHECK((greater       > pinitial_text) == (greater_text  > pinitial_text));
      CHECK((pinitial_text > greater)       == (pinitial_text > greater_text));

      CHECK((shorter       > pinitial_text) == (shorter_text  > pinitial_text));
      CHECK((pinitial_text > shorter)       == (pinitial_text > shorter_text));

      CHECK((initial       > pinitial_text) == (initial_text  > pinitial_text));
      CHECK((pinitial_text > initial)       == (pinitial_text > initial_text));
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_greater_than_or_equal)
    {
      const Text less(less_text.begin(), less_text.end());
      const Text initial(initial_text.begin(), initial_text.end());

      // String-String
      CHECK((less    >= initial) == (less_text    >= initial_text));
      CHECK((initial >= less)    == (initial_text >= less_text));

      const Text greater(greater_text.begin(), greater_text.end());
      CHECK((greater >= initial) == (greater_text >= initial_text));
      CHECK((initial >= greater) == (initial_text >= greater_text));

      const Text shorter(shorter_text.begin(), shorter_text.end());
      CHECK((shorter >= initial) == (shorter_text >= initial_text));
      CHECK((initial >= shorter) == (initial_text > shorter_text));

      CHECK((initial >= initial) == (initial_text >= initial_text));
      CHECK((initial >= initial) == (initial_text >= initial_text));

      // String-Pointer Pointer-String
      CHECK((less          >= pinitial_text) == (less_text     >= pinitial_text));
      CHECK((pinitial_text >= less)          == (pinitial_text >= less_text));

      CHECK((greater       >= pinitial_text) == (greater_text  >= pinitial_text));
      CHECK((pinitial_text >= greater)       == (pinitial_text >= greater_text));

      CHECK((shorter       >= pinitial_text) == (shorter_text  >= pinitial_text));
      CHECK((pinitial_text >= shorter)       == (pinitial_text >= shorter_text));

      CHECK((initial       >= pinitial_text) == (initial_text  >= pinitial_text));
      CHECK((pinitial_text >= initial)       == (pinitial_text >= initial_text));
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_copy)
    {
      TextSTD compare_text(initial_text.c_str());
      Text text(initial_text.c_str());

      value_t buffer1[SIZE];
      value_t buffer2[SIZE];

      size_t length1 = compare_text.copy(buffer1, 5, 2);
      buffer1[length1] = STR('\0');

      size_t length2 = text.copy(buffer2, 5, 2);
      buffer2[length2] = STR('\0');

      CHECK_EQUAL(length1, length2);
#if ETL_HAS_STRING_TRUNCATION_CHECKS
      CHECK_FALSE(text.is_truncated());
#endif

      bool is_equal = std::equal(buffer1,
                                 buffer1 + length1,
                                 buffer2);
      CHECK(is_equal);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_copy_start_pos_too_large)
    {
      Text text(initial_text.c_str());

      value_t buffer1[SIZE];

      size_t length1 = text.copy(buffer1, 5, SIZE);

      CHECK_EQUAL(0U, length1);
#if ETL_HAS_STRING_TRUNCATION_CHECKS
      CHECK_FALSE(text.is_truncated());
#endif
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_copy_count_equals_npos)
    {
      TextSTD compare_text(initial_text.c_str());
      Text text(initial_text.c_str());

      value_t buffer1[SIZE];
      value_t buffer2[SIZE];

      size_t length1 = compare_text.copy(buffer1, TextSTD::npos, 2);
      buffer1[length1] = STR('\0');

      size_t length2 = text.copy(buffer2, Text::npos, 2);
      buffer2[length2] = STR('\0');

      CHECK_EQUAL(length1, length2);
#if ETL_HAS_STRING_TRUNCATION_CHECKS
      CHECK_FALSE(text.is_truncated());
#endif

      bool is_equal = std::equal(buffer1,
                                 buffer1 + length1,
                                 buffer2);
      CHECK(is_equal);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_copy_count_too_large)
    {
      TextSTD compare_text(initial_text.c_str());
      Text text(initial_text.c_str());

      value_t buffer1[SIZE];
      value_t buffer2[SIZE];

      size_t length1 = compare_text.copy(buffer1, SIZE, 2);
      buffer1[length1] = STR('\0');

      size_t length2 = text.copy(buffer2, SIZE, 2);
      buffer2[length2] = STR('\0');

      CHECK_EQUAL(length1, length2);
#if ETL_HAS_STRING_TRUNCATION_CHECKS
      CHECK_FALSE(text.is_truncated());
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

      TextSTD compare_needle(STR("needle"));
      Text needle(STR("needle"));

      TextSTD compare_haystack(the_haystack);
      TextL haystack(the_haystack);

      size_t position1 = 0UL;
      size_t position2 = 0UL;

      position1 = compare_haystack.find(compare_needle, position1);
      position2 = haystack.find(needle, position2);
      CHECK_EQUAL(position1, position2);

      position1 = compare_haystack.find(compare_needle, position1 + 1);
      position2 = haystack.find(needle, position2 + 1);
      CHECK_EQUAL(position1, position2);

      position2 = haystack.find(needle, position2 + 1);
      CHECK_EQUAL(Text::npos, position2);

      Text pin(STR("pin"));
      position2 = haystack.find(pin);
      CHECK_EQUAL(TextL::npos, position2);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_find_view)
    {
      const value_t* the_haystack = STR("A haystack with a needle and another needle");

      TextSTD compare_needle(STR("needle"));
      Text needle(STR("needle"));
      View needle_view(needle);

      TextSTD compare_haystack(the_haystack);
      TextL haystack(the_haystack);

      size_t position1 = 0UL;
      size_t position2 = 0UL;

      position1 = compare_haystack.find(compare_needle, position1);
      position2 = haystack.find(needle_view, position2);
      CHECK_EQUAL(position1, position2);

      position1 = compare_haystack.find(compare_needle, position1 + 1);
      position2 = haystack.find(needle_view, position2 + 1);
      CHECK_EQUAL(position1, position2);

      position2 = haystack.find(needle_view, position2 + 1);
      CHECK_EQUAL(Text::npos, position2);

      View pin_view(STR("pin"));
      position2 = haystack.find(pin_view);
      CHECK_EQUAL(TextL::npos, position2);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_find_pointer)
    {
      const value_t* the_haystack = STR("A haystack with a needle and another needle");

      const value_t* needle = STR("needle");

      TextSTD compare_haystack(the_haystack);
      TextL haystack(the_haystack);

      size_t position1 = 0UL;
      size_t position2 = 0UL;

      position1 = compare_haystack.find(needle, position1);
      position2 = haystack.find(needle, position2);
      CHECK_EQUAL(position1, position2);

      position1 = compare_haystack.find(needle, position1 + 1);
      position2 = haystack.find(needle, position2 + 1);
      CHECK_EQUAL(position1, position2);

      position2 = haystack.find(needle, position2 + 1);
      CHECK_EQUAL(TextL::npos, position2);

      const value_t *pin = STR("pin");
      position2 = haystack.find(pin);
      CHECK_EQUAL(TextL::npos, position2);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_find_char_pointer_n)
    {
      const value_t* the_haystack = STR("A haystack with a needle and another needle");

      const value_t* needle = STR("needle");

      TextSTD compare_haystack(the_haystack);
      TextL haystack(the_haystack);

      size_t position1 = 0UL;
      size_t position2 = 0UL;

      position1 = compare_haystack.find(needle, position1, 3);
      position2 = haystack.find(needle, position2, 3);
      CHECK_EQUAL(position1, position2);

      position1 = compare_haystack.find(needle, position1 + 1, 3);
      position2 = haystack.find(needle, position2 + 1, 3);
      CHECK_EQUAL(position1, position2);

      position2 = haystack.find(needle, position2 + 1, 3);
      CHECK_EQUAL(TextL::npos, position2);

      const value_t *pin = STR("pin");
      position2 = haystack.find(pin, 0, 3);
      CHECK_EQUAL(TextL::npos, position2);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_contains_string)
    {
      TextL haystack(STR("A haystack with a needle and nothing else"));
      Text needle(STR("needle"));
      Text pin(STR("pin"));
      Text excess(STR("A really gigantic pin or needle that's really really big"));

      CHECK_TRUE(haystack.contains(needle));
      CHECK_FALSE(haystack.contains(pin));
      CHECK_FALSE(haystack.contains(excess));
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_contains_view)
    {
      TextL haystack(STR("A haystack with a needle and nothing else"));

      CHECK_TRUE(haystack.contains(View(STR("needle"))));
      CHECK_FALSE(haystack.contains(View(STR("pin"))));
      CHECK_FALSE(haystack.contains(View(STR("A really gigantic pin or needle that's really really big"))));
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_contains_pointer)
    {
      TextL haystack(STR("A haystack with a needle and nothing else"));

      CHECK_TRUE(haystack.contains(STR("needle")));
      CHECK_FALSE(haystack.contains(STR("pin")));
      CHECK_FALSE(haystack.contains(STR("A really gigantic pin or needle that's really really big")));
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_contains_char)
    {
      TextL haystack(STR("A haystack with a needle and nothing else"));

      CHECK_TRUE(haystack.contains(STR('l')));
      CHECK_FALSE(haystack.contains(STR('p')));
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_starts_with_string)
    {
      TextL haystack(STR("A haystack with a needle and nothing else"));
      Text start(STR("A haystack"));
      Text not_start(STR("a needle"));
      Text excess(STR("Really gigantic text that's really really big"));

      CHECK_TRUE(haystack.starts_with(start));
      CHECK_FALSE(haystack.starts_with(not_start));
      CHECK_FALSE(haystack.starts_with(excess));
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_starts_with_view)
    {
      TextL haystack(STR("A haystack with a needle and nothing else"));

      CHECK_TRUE(haystack.starts_with(View(STR("A haystack"))));
      CHECK_FALSE(haystack.starts_with(View(STR("a needle"))));
      CHECK_FALSE(haystack.starts_with(View(STR("Really gigantic text that's really really big"))));
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_starts_with_pointer)
    {
      TextL haystack(STR("A haystack with a needle and nothing else"));

      CHECK_TRUE(haystack.starts_with(STR("A haystack")));
      CHECK_FALSE(haystack.starts_with(STR("a needle")));
      CHECK_FALSE(haystack.starts_with(STR("Really gigantic text that's really really big")));
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_starts_with_char)
    {
      TextL haystack(STR("A haystack with a needle and nothing else"));

      CHECK_TRUE(haystack.starts_with(haystack[0]));
      CHECK_FALSE(haystack.starts_with(haystack[1]));
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_ends_with_string)
    {
      TextL haystack(STR("A haystack with a needle and nothing else"));
      Text end(STR("else"));
      Text not_end(STR("needle"));
      Text excess(STR("Really gigantic text that's really really big"));

      CHECK_TRUE(haystack.ends_with(end));
      CHECK_FALSE(haystack.ends_with(not_end));
      CHECK_FALSE(haystack.ends_with(excess));
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_ends_with_view)
    {
      TextL haystack(STR("A haystack with a needle and nothing else"));

      CHECK_TRUE(haystack.ends_with(View(STR("else"))));
      CHECK_FALSE(haystack.ends_with(View(STR("needle"))));
      CHECK_FALSE(haystack.ends_with(View(STR("Really gigantic text that's really really big"))));
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_ends_with_pointer)
    {
      TextL haystack(STR("A haystack with a needle and nothing else"));

      CHECK_TRUE(haystack.ends_with(STR("else")));
      CHECK_FALSE(haystack.ends_with(STR("needle")));
      CHECK_FALSE(haystack.ends_with(STR("Really gigantic text that's really really big")));
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_ends_with_char)
    {
      TextL haystack(STR("A haystack with a needle and nothing else"));

      CHECK_TRUE(haystack.ends_with(haystack[haystack.size() - 1]));
      CHECK_FALSE(haystack.ends_with(haystack[haystack.size() - 2]));
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_rfind_string)
    {
      const value_t* the_haystack = STR("A haystack with a needle and another needle");

      TextSTD compare_needle(STR("needle"));
      Text needle(STR("needle"));

      TextSTD compare_haystack(the_haystack);
      TextL haystack(the_haystack);

      size_t position1 = TextSTD::npos;
      size_t position2 = Text::npos;

      position1 = compare_haystack.rfind(compare_needle, position1);
      position2 = haystack.rfind(needle, position2);
      CHECK_EQUAL(position1, position2);

      position1 = compare_haystack.rfind(compare_needle, compare_haystack.size() - 10);
      position2 = haystack.rfind(needle, haystack.size() - 10);
      CHECK_EQUAL(position1, position2);

      Text pin(STR("pin"));
      position2 = haystack.rfind(pin);
      CHECK_EQUAL(TextL::npos, position2);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_rfind_view)
    {
      const value_t* the_haystack = STR("A haystack with a needle and another needle");

      TextSTD compare_needle(STR("needle"));
      Text needle(STR("needle"));
      View needle_view(needle);

      TextSTD compare_haystack(the_haystack);
      TextL haystack(the_haystack);

      size_t position1 = TextSTD::npos;
      size_t position2 = Text::npos;

      position1 = compare_haystack.rfind(compare_needle, position1);
      position2 = haystack.rfind(needle_view, position2);
      CHECK_EQUAL(position1, position2);

      position1 = compare_haystack.rfind(compare_needle, compare_haystack.size() - 10);
      position2 = haystack.rfind(needle_view, haystack.size() - 10);
      CHECK_EQUAL(position1, position2);

      View pin_view(STR("pin"));
      position2 = haystack.rfind(pin_view);
      CHECK_EQUAL(TextL::npos, position2);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_rfind_pointer)
    {
      const value_t*the_haystack = STR("A haystack with a needle and another needle");

      TextSTD compare_haystack(the_haystack);
      TextL haystack(the_haystack);

      const value_t* needle = STR("needle");

      size_t position1 = TextSTD::npos;
      size_t position2 = Text::npos;

      position1 = compare_haystack.rfind(needle, position1);
      position2 = haystack.rfind(needle, position2);
      CHECK_EQUAL(position1, position2);

      position1 = compare_haystack.rfind(needle, compare_haystack.size() - 10);
      position2 = haystack.rfind(needle, haystack.size() - 10);
      CHECK_EQUAL(position1, position2);

      Text pin(STR("pin"));
      position2 = haystack.rfind(pin);
      CHECK_EQUAL(TextL::npos, position2);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_rfind_pointer_n)
    {
      const value_t*the_haystack = STR("A haystack with a needle and another needle");

      TextSTD compare_haystack(the_haystack);
      TextL haystack(the_haystack);

      const value_t* needle = STR("needle");

      size_t position1 = TextSTD::npos;
      size_t position2 = Text::npos;

      position1 = compare_haystack.rfind(needle, position1, 3);
      position2 = haystack.rfind(needle, position2, 3);
      CHECK_EQUAL(position1, position2);

      position1 = compare_haystack.rfind(needle, compare_haystack.size() - 10, 3);
      position2 = haystack.rfind(needle, haystack.size() - 10, 3);
      CHECK_EQUAL(position1, position2);

      position2 = haystack.rfind(STR("pin"), 3);
      CHECK_EQUAL(TextL::npos, position2);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_rfind_c_position)
    {
      const value_t*the_haystack = STR("A haystack with a needle and another needle");

      TextSTD compare_haystack(the_haystack);
      TextL haystack(the_haystack);

      etl::iu8string::size_type position1 = TextL::npos;
      size_t position2 = Text::npos;

      position1 = compare_haystack.rfind(STR('e'), position1);
      position2 = haystack.rfind(STR('e'), position2);
      CHECK_EQUAL(position1, position2);

      position1 = compare_haystack.rfind(STR('e'), compare_haystack.size() - 10);
      position2 = haystack.rfind(STR('e'), haystack.size() - 10);
      CHECK_EQUAL(position1, position2);

      position2 = haystack.rfind(STR('z'));
      CHECK_EQUAL(TextL::npos, position2);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_substr)
    {
      TextSTD compare_text(initial_text.c_str());
      Text text(initial_text.c_str());

      TextSTD compare_result;
      Text result;

      // Equal.
      compare_result = compare_text.substr(compare_text.size());
      result = text.substr(text.size());
      CHECK(Equal(compare_result, result));

      // Whole string.
      compare_result = compare_text.substr();
      result = text.substr();
      CHECK(Equal(compare_result, result));

      // Starting from position 2.
      compare_result = compare_text.substr(2);
      result = text.substr(2);
      CHECK(Equal(compare_result, result));

      // Starting from position 2 for 3 characters.
      compare_result = compare_text.substr(2, 3);
      result = text.substr(2, 3);
      CHECK(Equal(compare_result, result));

      // Starting from position 2 for too many characters.
      compare_result = compare_text.substr(2, compare_text.size());
      result = text.substr(2, text.size());
      CHECK(Equal(compare_result, result));

      // Starting from beyond the end of the string.
      CHECK_THROW(text.substr(text.size() + 1), etl::string_out_of_bounds);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_compare_string)
    {
      TextSTD compare_text(STR("ABCDEF"));
      Text text(STR("ABCDEF"));

      int compare_result;
      int result;

      // Equal.
      compare_result = compare_text.compare(TextSTD(STR("ABCDEF")));
      result = text.compare(Text(STR("ABCDEF")));
      CHECK(compares_agree(compare_result, result));

      // Less.
      compare_result = compare_text.compare(TextSTD(STR("ABCDEE")));
      result = text.compare(Text(STR("ABCDEE")));
      CHECK(compares_agree(compare_result, result));

      // Greater.
      compare_result = compare_text.compare(TextSTD(STR("ABCDEG")));
      result = text.compare(Text(STR("ABCDEG")));
      CHECK(compares_agree(compare_result, result));

      // Shorter.
      compare_result = compare_text.compare(TextSTD(STR("ABCDE")));
      result = text.compare(Text(STR("ABCDE")));
      CHECK(compares_agree(compare_result, result));

      // Longer.
      compare_result = compare_text.compare(TextSTD(STR("ABCDEFG")));
      result = text.compare(Text(STR("ABCDEFG")));
      CHECK(compares_agree(compare_result, result));
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_compare_view)
    {
      TextSTD compare_text(STR("ABCDEF"));
      Text text(STR("ABCDEF"));

      int compare_result;
      int result;

      // Equal.
      compare_result = compare_text.compare(TextSTD(STR("ABCDEF")));
      result = text.compare(View(STR("ABCDEF")));
      CHECK(compares_agree(compare_result, result));

      // Less.
      compare_result = compare_text.compare(TextSTD(STR("ABCDEE")));
      result = text.compare(View(STR("ABCDEE")));
      CHECK(compares_agree(compare_result, result));

      // Greater.
      compare_result = compare_text.compare(TextSTD(STR("ABCDEG")));
      result = text.compare(View(STR("ABCDEG")));
      CHECK(compares_agree(compare_result, result));

      // Shorter.
      compare_result = compare_text.compare(TextSTD(STR("ABCDE")));
      result = text.compare(View(STR("ABCDE")));
      CHECK(compares_agree(compare_result, result));

      // Longer.
      compare_result = compare_text.compare(TextSTD(STR("ABCDEFG")));
      result = text.compare(View(STR("ABCDEFG")));
      CHECK(compares_agree(compare_result, result));
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_compare_position_length_string)
    {
      TextSTD compare_text(STR("xxxABCDEFyyy"));
      Text text(STR("xxxABCDEFyyy"));

      int compare_result;
      int result;

      // Equal.
      compare_result = compare_text.compare(3, 6, TextSTD(STR("ABCDEF")));
      result = text.compare(3, 6, Text(STR("ABCDEF")));
      CHECK(compares_agree(compare_result, result));

      // Less.
      compare_result = compare_text.compare(3, 6, TextSTD(STR("ABCDEE")));
      result = text.compare(3, 6, Text(STR("ABCDEE")));
      CHECK(compares_agree(compare_result, result));

      // Greater.
      compare_result = compare_text.compare(3, 6, TextSTD(STR("ABCDEG")));
      result = text.compare(3, 6, Text(STR("ABCDEG")));
      CHECK(compares_agree(compare_result, result));

      // Shorter.
      compare_result = compare_text.compare(3, 6, TextSTD(STR("ABCDE")));
      result = text.compare(3, 6, Text(STR("ABCDE")));
      CHECK(compares_agree(compare_result, result));

      // Longer.
      compare_result = compare_text.compare(3, 6, TextSTD(STR("ABCDEFG")));
      result = text.compare(3, 6, Text(STR("ABCDEFG")));
      CHECK(compares_agree(compare_result, result));
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_compare_position_length_view)
    {
      TextSTD compare_text(STR("xxxABCDEFyyy"));
      Text text(STR("xxxABCDEFyyy"));

      int compare_result;
      int result;

      // Equal.
      compare_result = compare_text.compare(3, 6, TextSTD(STR("ABCDEF")));
      result = text.compare(3, 6, View(STR("ABCDEF")));
      CHECK(compares_agree(compare_result, result));

      // Less.
      compare_result = compare_text.compare(3, 6, TextSTD(STR("ABCDEE")));
      result = text.compare(3, 6, View(STR("ABCDEE")));
      CHECK(compares_agree(compare_result, result));

      // Greater.
      compare_result = compare_text.compare(3, 6, TextSTD(STR("ABCDEG")));
      result = text.compare(3, 6, View(STR("ABCDEG")));
      CHECK(compares_agree(compare_result, result));

      // Shorter.
      compare_result = compare_text.compare(3, 6, TextSTD(STR("ABCDE")));
      result = text.compare(3, 6, View(STR("ABCDE")));
      CHECK(compares_agree(compare_result, result));

      // Longer.
      compare_result = compare_text.compare(3, 6, TextSTD(STR("ABCDEFG")));
      result = text.compare(3, 6, View(STR("ABCDEFG")));
      CHECK(compares_agree(compare_result, result));
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_compare_position_length_string_subposition_sublength)
    {
      TextSTD compare_text(STR("xxxABCDEFyyy"));
      Text text(STR("xxxABCDEFyyy"));

      int compare_result;
      int result;

      // Equal.
      compare_result = compare_text.compare(3, 6, TextSTD(STR("aaABCDEFbb")), 2, 6);
      result = text.compare(3, 6, Text(STR("aaABCDEFbb")), 2, 6);
      CHECK(compares_agree(compare_result, result));

      // Less.
      compare_result = compare_text.compare(3, 6, TextSTD(STR("aaABCDEEbb")), 2, 6);
      result = text.compare(3, 6, Text(STR("aaABCDEEbb")), 2, 6);
      CHECK(compares_agree(compare_result, result));

      // Greater.
      compare_result = compare_text.compare(3, 6, TextSTD(STR("aaABCDEGbb")), 2, 6);
      result = text.compare(3, 6, Text(STR("aaABCDEGbb")), 2, 6);
      CHECK(compares_agree(compare_result, result));

      // Shorter.
      compare_result = compare_text.compare(3, 6, TextSTD(STR("aaABCDEbb")), 2, 5);
      result = text.compare(3, 6, Text(STR("aaABCDEbb")), 2, 5);
      CHECK(compares_agree(compare_result, result));

      // Longer.
      compare_result = compare_text.compare(3, 6, TextSTD(STR("aaABCDEFGbb")), 2, 7);
      result = text.compare(3, 6, Text(STR("aaABCDEFGbb")), 2, 7);
      CHECK(compares_agree(compare_result, result));
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_compare_position_length_view_subposition_sublength)
    {
      TextSTD compare_text(STR("xxxABCDEFyyy"));
      Text text(STR("xxxABCDEFyyy"));

      int compare_result;
      int result;

      // Equal.
      compare_result = compare_text.compare(3, 6, TextSTD(STR("aaABCDEFbb")), 2, 6);
      result = text.compare(3, 6, View(STR("aaABCDEFbb")), 2, 6);
      CHECK(compares_agree(compare_result, result));

      // Less.
      compare_result = compare_text.compare(3, 6, TextSTD(STR("aaABCDEEbb")), 2, 6);
      result = text.compare(3, 6, View(STR("aaABCDEEbb")), 2, 6);
      CHECK(compares_agree(compare_result, result));

      // Greater.
      compare_result = compare_text.compare(3, 6, TextSTD(STR("aaABCDEGbb")), 2, 6);
      result = text.compare(3, 6, View(STR("aaABCDEGbb")), 2, 6);
      CHECK(compares_agree(compare_result, result));

      // Shorter.
      compare_result = compare_text.compare(3, 6, TextSTD(STR("aaABCDEbb")), 2, 5);
      result = text.compare(3, 6, View(STR("aaABCDEbb")), 2, 5);
      CHECK(compares_agree(compare_result, result));

      // Longer.
      compare_result = compare_text.compare(3, 6, TextSTD(STR("aaABCDEFGbb")), 2, 7);
      result = text.compare(3, 6, View(STR("aaABCDEFGbb")), 2, 7);
      CHECK(compares_agree(compare_result, result));
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_compare_c_string)
    {
      TextSTD compare_text(STR("ABCDEF"));
      Text text(STR("ABCDEF"));

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
    TEST_FIXTURE(SetupFixture, test_compare_position_length_c_string)
    {
      TextSTD compare_text(STR("xxxABCDEFyyy"));
      Text text(STR("xxxABCDEFyyy"));

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
    TEST_FIXTURE(SetupFixture, test_compare_position_length_c_string_n)
    {
      TextSTD compare_text(STR("xxxABCDEFyyy"));
      Text text(STR("xxxABCDEFyyy"));

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
      TextSTD compare_text(STR("ABCDEF"));
      Text text(STR("ABCDEF"));

      size_t position1 = compare_text.find_first_of(TextSTD(STR("ZCXF")));
      size_t position2 = text.find_first_of(Text(STR("ZCXF")));

      CHECK_EQUAL(position1, position2);

      position1 = compare_text.find_first_of(TextSTD(STR("WXYZ")));
      position2 = text.find_first_of(Text(STR("WXYZ")));

      CHECK_EQUAL(position1, position2);

      position1 = compare_text.find_first_of(TextSTD(STR("ZCXF")), 3);
      position2 = text.find_first_of(Text(STR("ZCXF")), 3);

      CHECK_EQUAL(position1, position2);

#include "etl/private/diagnostic_array_bounds_push.h"
      position1 = compare_text.find_first_of(TextSTD(STR("ZCXF")), 100);
      position2 = text.find_first_of(Text(STR("ZCXF")), 100);

      CHECK_EQUAL(position1, position2);
#include "etl/private/diagnostic_pop.h"
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_find_first_of_view_position)
    {
      TextSTD compare_text(STR("ABCDEF"));
      Text text(STR("ABCDEF"));

      size_t position1 = compare_text.find_first_of(TextSTD(STR("ZCXF")));
      size_t position2 = text.find_first_of(View(STR("ZCXF")));

      CHECK_EQUAL(position1, position2);

      position1 = compare_text.find_first_of(TextSTD(STR("WXYZ")));
      position2 = text.find_first_of(View(STR("WXYZ")));

      CHECK_EQUAL(position1, position2);

      position1 = compare_text.find_first_of(TextSTD(STR("ZCXF")), 3);
      position2 = text.find_first_of(View(STR("ZCXF")), 3);

      CHECK_EQUAL(position1, position2);

#include "etl/private/diagnostic_array_bounds_push.h"
      position1 = compare_text.find_first_of(TextSTD(STR("ZCXF")), 100);
      position2 = text.find_first_of(View(STR("ZCXF")), 100);

      CHECK_EQUAL(position1, position2);
#include "etl/private/diagnostic_pop.h"
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_find_first_of_pointer_position)
    {
      TextSTD compare_text(STR("ABCDEF"));
      Text text(STR("ABCDEF"));

      size_t position1 = compare_text.find_first_of(STR("ZCXF"));
      size_t position2 = text.find_first_of(STR("ZCXF"));

      CHECK_EQUAL(position1, position2);

      position1 = compare_text.find_first_of(STR("WXYZ"));
      position2 = text.find_first_of(STR("WXYZ"));

      CHECK_EQUAL(position1, position2);

      position1 = compare_text.find_first_of(STR("ZCXF"), 3);
      position2 = text.find_first_of(STR("ZCXF"), 3);

      CHECK_EQUAL(position1, position2);

#include "etl/private/diagnostic_array_bounds_push.h"
      position1 = compare_text.find_first_of(STR("ZCXF"), 100);
      position2 = text.find_first_of(STR("ZCXF"), 100);

      CHECK_EQUAL(position1, position2);
#include "etl/private/diagnostic_pop.h"
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_find_first_of_pointer_position_n)
    {
      TextSTD compare_text(STR("ABCDEF"));
      Text text(STR("ABCDEF"));

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

#include "etl/private/diagnostic_array_bounds_push.h"
      position1 = compare_text.find_first_of(STR("ZCXF"), 100);
      position2 = text.find_first_of(STR("ZCXF"), 100);

      CHECK_EQUAL(position1, position2);
#include "etl/private/diagnostic_pop.h"
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_find_first_of_character_position)
    {
      TextSTD compare_text(STR("ABCDEF"));
      Text text(STR("ABCDEF"));

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

#include "etl/private/diagnostic_array_bounds_push.h"
      position1 = compare_text.find_first_of(STR('C'), 100);
      position2 = text.find_first_of(STR('C'), 100);

      CHECK_EQUAL(position1, position2);
#include "etl/private/diagnostic_pop.h"
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_find_last_of_string_position)
    {
      TextSTD compare_text(STR("ABCDEFABCDE"));
      Text text(STR("ABCDEFABCDE"));

      size_t position1 = compare_text.find_last_of(TextSTD(STR("ZCXE")));
      size_t position2 = text.find_last_of(Text(STR("ZCXE")));

      CHECK_EQUAL(position1, position2);

      position1 = compare_text.find_last_of(TextSTD(STR("WXYZ")), 3);
      position2 = text.find_last_of(Text(STR("WXYZ")), 3);

      CHECK_EQUAL(position1, position2);

      position1 = compare_text.find_last_of(TextSTD(STR("ZCXE")), 5);
      position2 = text.find_last_of(Text(STR("ZCXE")), 5);

      CHECK_EQUAL(position1, position2);

      position1 = compare_text.find_last_of(TextSTD(STR("ZCXE")), compare_text.size());
      position2 = text.find_last_of(Text(STR("ZCXE")), text.size());

      CHECK_EQUAL(position1, position2);

#include "etl/private/diagnostic_array_bounds_push.h"
      position1 = compare_text.find_last_of(TextSTD(STR("ZCXE")), 100);
      position2 = text.find_last_of(Text(STR("ZCXE")), 100);

      CHECK_EQUAL(position1, position2);
#include "etl/private/diagnostic_pop.h"
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_find_last_of_view_position)
    {
      TextSTD compare_text(STR("ABCDEFABCDE"));
      Text text(STR("ABCDEFABCDE"));

      size_t position1 = compare_text.find_last_of(TextSTD(STR("ZCXE")));
      size_t position2 = text.find_last_of(View(STR("ZCXE")));

      CHECK_EQUAL(position1, position2);

      position1 = compare_text.find_last_of(TextSTD(STR("WXYZ")), 3);
      position2 = text.find_last_of(View(STR("WXYZ")), 3);

      CHECK_EQUAL(position1, position2);

      position1 = compare_text.find_last_of(TextSTD(STR("ZCXE")), 5);
      position2 = text.find_last_of(View(STR("ZCXE")), 5);

      CHECK_EQUAL(position1, position2);

      position1 = compare_text.find_last_of(TextSTD(STR("ZCXE")), compare_text.size());
      position2 = text.find_last_of(View(STR("ZCXE")), text.size());

      CHECK_EQUAL(position1, position2);

#include "etl/private/diagnostic_array_bounds_push.h"
      position1 = compare_text.find_last_of(TextSTD(STR("ZCXE")), 100);
      position2 = text.find_last_of(View(STR("ZCXE")), 100);

      CHECK_EQUAL(position1, position2);
#include "etl/private/diagnostic_pop.h"
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_find_last_of_pointer_position)
    {
      TextSTD compare_text(STR("ABCDEFABCDE"));
      Text text(STR("ABCDEFABCDE"));

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

#include "etl/private/diagnostic_array_bounds_push.h"
      position1 = compare_text.find_last_of(STR("ZCXE"), 100);
      position2 = text.find_last_of(STR("ZCXE"), 100);

      CHECK_EQUAL(position1, position2);
#include "etl/private/diagnostic_pop.h"
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_find_last_of_pointer_position_n)
    {
      TextSTD compare_text(STR("ABCDEFABCDE"));
      Text text(STR("ABCDEFABCDE"));

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
      TextSTD compare_text(STR("ABCDEF"));
      Text text(STR("ABCDEF"));

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

#include "etl/private/diagnostic_array_bounds_push.h"
      position1 = compare_text.find_last_of(STR('C'), 100);
      position2 = text.find_last_of(STR('C'), 100);

      CHECK_EQUAL(position1, position2);
#include "etl/private/diagnostic_pop.h"
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_find_first_not_of_string_position)
    {
      TextSTD compare_text(STR("ABCDEF"));
      Text text(STR("ABCDEF"));

      size_t position1 = compare_text.find_first_not_of(TextSTD(STR("ZAXB")));
      size_t position2 = text.find_first_not_of(Text(STR("ZAXB")));

      CHECK_EQUAL(position1, position2);

      position1 = compare_text.find_first_not_of(TextSTD(STR("ZAXB")));
      position2 = text.find_first_not_of(Text(STR("ZAXB")));

      CHECK_EQUAL(position1, position2);

      position1 = compare_text.find_first_not_of(TextSTD(STR("ZAXB")), 3);
      position2 = text.find_first_not_of(Text(STR("ZAXB")), 3);

      CHECK_EQUAL(position1, position2);

      position1 = compare_text.find_first_not_of(TextSTD(STR("ZAXB")), compare_text.size());
      position2 = text.find_first_not_of(Text(STR("ZAXB")), text.size());

      CHECK_EQUAL(position1, position2);

#include "etl/private/diagnostic_array_bounds_push.h"
      position1 = compare_text.find_first_not_of(TextSTD(STR("ZAXB")), 100);
      position2 = text.find_first_not_of(Text(STR("ZAXB")), 100);

      CHECK_EQUAL(position1, position2);
#include "etl/private/diagnostic_pop.h"
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_find_first_not_of_view_position)
    {
      TextSTD compare_text(STR("ABCDEF"));
      Text text(STR("ABCDEF"));

      size_t position1 = compare_text.find_first_not_of(TextSTD(STR("ZAXB")));
      size_t position2 = text.find_first_not_of(View(STR("ZAXB")));

      CHECK_EQUAL(position1, position2);

      position1 = compare_text.find_first_not_of(TextSTD(STR("ZAXB")));
      position2 = text.find_first_not_of(View(STR("ZAXB")));

      CHECK_EQUAL(position1, position2);

      position1 = compare_text.find_first_not_of(TextSTD(STR("ZAXB")), 3);
      position2 = text.find_first_not_of(View(STR("ZAXB")), 3);

      CHECK_EQUAL(position1, position2);

      position1 = compare_text.find_first_not_of(TextSTD(STR("ZAXB")), compare_text.size());
      position2 = text.find_first_not_of(View(STR("ZAXB")), text.size());

      CHECK_EQUAL(position1, position2);

#include "etl/private/diagnostic_array_bounds_push.h"
      position1 = compare_text.find_first_not_of(TextSTD(STR("ZAXB")), 100);
      position2 = text.find_first_not_of(View(STR("ZAXB")), 100);

      CHECK_EQUAL(position1, position2);
#include "etl/private/diagnostic_pop.h"
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_find_first_not_of_pointer_position)
    {
      TextSTD compare_text(STR("ABCDEF"));
      Text text(STR("ABCDEF"));

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

#include "etl/private/diagnostic_array_bounds_push.h"
      position1 = compare_text.find_first_not_of(STR("ZAXB"), 100);
      position2 = text.find_first_not_of(STR("ZAXB"), 100);

      CHECK_EQUAL(position1, position2);
#include "etl/private/diagnostic_pop.h"
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_find_first_not_of_pointer_position_n)
    {
      TextSTD compare_text(STR("ABCDEF"));
      Text text(STR("ABCDEF"));

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

#include "etl/private/diagnostic_array_bounds_push.h"
      position1 = compare_text.find_first_not_of(STR("ZAXB"), 100);
      position2 = text.find_first_not_of(STR("ZAXB"), 100);

      CHECK_EQUAL(position1, position2);
#include "etl/private/diagnostic_pop.h"
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_find_first_not_of_character_position)
    {
      TextSTD compare_text(STR("ABCDEF"));
      Text text(STR("ABCDEF"));

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

#include "etl/private/diagnostic_array_bounds_push.h"
      position1 = compare_text.find_first_not_of(STR('C'), 100);
      position2 = text.find_first_not_of(STR('C'), 100);

      CHECK_EQUAL(position1, position2);
#include "etl/private/diagnostic_pop.h"
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_find_last_not_of_string_position)
    {
      TextSTD compare_text(STR("ABCDEFABCDE"));
      Text text(STR("ABCDEFABCDE"));

      size_t position1 = compare_text.find_last_not_of(TextSTD(STR("ZEXD")));
      size_t position2 = text.find_last_not_of(Text(STR("ZEXD")));

      CHECK_EQUAL(position1, position2);

      position1 = compare_text.find_last_not_of(TextSTD(STR("ZEXD")), 3);
      position2 = text.find_last_not_of(Text(STR("ZEXD")), 3);

      CHECK_EQUAL(position1, position2);

      position1 = compare_text.find_last_not_of(TextSTD(STR("ZEXD")), 5);
      position2 = text.find_last_not_of(Text(STR("ZEXD")), 5);

      CHECK_EQUAL(position1, position2);

      position1 = compare_text.find_last_not_of(TextSTD(STR("ZEXD")), compare_text.size());
      position2 = text.find_last_not_of(Text(STR("ZEXD")), text.size());

      CHECK_EQUAL(position1, position2);

#include "etl/private/diagnostic_array_bounds_push.h"
      position1 = compare_text.find_last_not_of(TextSTD(STR("ZEXD")), 100);
      position2 = text.find_last_not_of(Text(STR("ZEXD")), 100);

      CHECK_EQUAL(position1, position2);
#include "etl/private/diagnostic_pop.h"
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_find_last_not_of_view_position)
    {
      TextSTD compare_text(STR("ABCDEFABCDE"));
      Text text(STR("ABCDEFABCDE"));

      size_t position1 = compare_text.find_last_not_of(TextSTD(STR("ZEXD")));
      size_t position2 = text.find_last_not_of(View(STR("ZEXD")));

      CHECK_EQUAL(position1, position2);

      position1 = compare_text.find_last_not_of(TextSTD(STR("ZEXD")), 3);
      position2 = text.find_last_not_of(View(STR("ZEXD")), 3);

      CHECK_EQUAL(position1, position2);

      position1 = compare_text.find_last_not_of(TextSTD(STR("ZEXD")), 5);
      position2 = text.find_last_not_of(View(STR("ZEXD")), 5);

      CHECK_EQUAL(position1, position2);

      position1 = compare_text.find_last_not_of(TextSTD(STR("ZEXD")), compare_text.size());
      position2 = text.find_last_not_of(View(STR("ZEXD")), text.size());

      CHECK_EQUAL(position1, position2);

#include "etl/private/diagnostic_array_bounds_push.h"
      position1 = compare_text.find_last_not_of(TextSTD(STR("ZEXD")), 100);
      position2 = text.find_last_not_of(View(STR("ZEXD")), 100);

      CHECK_EQUAL(position1, position2);
#include "etl/private/diagnostic_pop.h"
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_find_last_not_of_pointer_position)
    {
      TextSTD compare_text(STR("ABCDEFABCDE"));
      Text text(STR("ABCDEFABCDE"));

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

#include "etl/private/diagnostic_array_bounds_push.h"
      position1 = compare_text.find_last_not_of(STR("ZEXD"), 100);
      position2 = text.find_last_not_of(STR("ZEXD"), 100);

      CHECK_EQUAL(position1, position2);
#include "etl/private/diagnostic_pop.h"
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_find_last_not_of_pointer_position_n)
    {
      TextSTD compare_text(STR("ABCDEFABCDE"));
      Text text(STR("ABCDEFABCDE"));

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
      TextSTD compare_text(STR("ABCDEF"));
      Text text(STR("ABCDEF"));

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

#include "etl/private/diagnostic_array_bounds_push.h"
      position1 = compare_text.find_last_not_of(STR('C'), 100);
      position2 = text.find_last_not_of(STR('C'), 100);

      CHECK_EQUAL(position1, position2);
#include "etl/private/diagnostic_pop.h"
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_hash)
    {
      // Test with actual string type.
      Text text(STR("ABCDEFHIJKL"));
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
      Text text;

      text.assign(STR("ABCDEF"));

      char buffer[sizeof(Text)];

      memcpy(&buffer, (const void*)&text, sizeof(text));

      Text& rtext(*reinterpret_cast<Text*>(buffer));
      rtext.repair();

      CHECK(!rtext.empty());
      CHECK(!rtext.full());

      bool is_equal = Equal(text, rtext);
      CHECK(is_equal);

      text = STR("GHIJKL");
      is_equal = Equal(text, rtext);
      CHECK(!is_equal);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_memcpy_repair_virtual)
    {
      Text text;

      text.assign(STR("ABCDEF"));

      char buffer[sizeof(Text)];

      memcpy(&buffer, (const void*)&text, sizeof(text));

      IText& itext(*reinterpret_cast<IText*>(buffer));
      itext.repair();

      CHECK(!itext.empty());
      CHECK(!itext.full());

      bool is_equal = Equal(text, itext);
      CHECK(is_equal);

      text = STR("GHIJKL");
      is_equal = Equal(text, itext);
      CHECK(!is_equal);
    }

#if ETL_HAS_STRING_TRUNCATION_CHECKS
    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_truncate_over_many_operations)
    {
      Text text(short_text.c_str());
      CHECK_FALSE(text.is_truncated());

      text.insert(3, initial_text.c_str());
      CHECK_TRUE(text.is_truncated());

      while (text.size() != 0)
      {
        text.pop_back();
        CHECK_TRUE(text.is_truncated());
      }

      text.clear();
      CHECK_FALSE(text.is_truncated());

      text.assign(longer_text.c_str());
      CHECK_TRUE(text.is_truncated());

      text.assign(short_text.c_str());
      CHECK_FALSE(text.is_truncated());
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_add_from_truncated)
    {
      Text  text1(short_text.c_str());
      TextS text2(short_text.c_str());

      CHECK_FALSE(text1.is_truncated());
      CHECK_TRUE(text2.is_truncated());

      // text2 has the truncate flag set.
      text1 += text2;

      CHECK(text1.is_truncated());
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_add_to_truncated)
    {
      Text text1(longer_text.c_str());
      Text text2(short_text.c_str());

      CHECK(text1.is_truncated());
      CHECK_FALSE(text2.is_truncated());

      // Clear text but not the truncate flag.
      text1.erase(text1.begin(), text1.end());

      // text1 still has the truncate flag set.
      text1 += text2;

      CHECK(text1.is_truncated());
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_clear_truncated)
    {
      Text text(longer_text.c_str());
      CHECK_TRUE(text.is_truncated());

      text.clear_truncated();
      CHECK_FALSE(text.is_truncated());
    }
#endif

#if ETL_HAS_STRING_CLEAR_AFTER_USE
    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_secure_after_destructor)
    {
      char buffer[sizeof(Text)];
      std::fill_n(buffer, sizeof(Text), 0);
      ::new (buffer) Text(STR("ABCDEF"));

      Text& text = *reinterpret_cast<Text*>(buffer);
      text.set_secure();

      CHECK(Text(STR("ABCDEF")) == text);

      Text::pointer pb = text.begin();
      Text::pointer pe = text.end();

      // Destroy the text object.
      std::atomic_signal_fence(std::memory_order_seq_cst);
      text.~Text();
      std::atomic_signal_fence(std::memory_order_seq_cst);

      // Check there no non-zero values in the string.
      CHECK(std::find_if(pb, pe, [](Text::value_type x) { return x != 0; }) == pe);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_secure_after_assign)
    {
      Text text;
      text.set_secure();
      text.assign(STR("ABCDEF"));

      Text::pointer pe = text.end();

      text.assign(STR("ABC"));

      CHECK(std::find_if(text.end(), pe, [](Text::value_type x) { return x != 0; }) == pe);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_secure_after_resize_down)
    {
      Text text;
      text.set_secure();
      text.assign(STR("ABCDEF"));

      Text::pointer pe = text.end();

      text.resize(text.size() - 3U);

      CHECK(std::find_if(text.end(), pe, [](Text::value_type x) { return x != 0; }) == pe);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_secure_after_erase)
    {
      Text text;
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
      Text text;
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
      Text text;
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
      Text text1 = STR("Hello World");
      text1.set_secure();

      Text text2(text1);

      Text text3;
      text3 = text1;

      Text text4(text1, 6U, 3U);

      CHECK(text2.is_secure());
      CHECK(text3.is_secure());
      CHECK(text4.is_secure());
    }
#endif

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_initialize_free_space_empty_string)
    {
      Text empty;
      Text text;

      text.initialize_free_space();

      CHECK(text.empty());
      CHECK(text == empty);

      for (size_t i = text.size(); i < text.max_size(); ++i)
      {
        CHECK_EQUAL(0, text[i]);
      }
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_initialize_free_space_part_filled_string)
    {
      Text empty;
      Text initial = STR("ABC");
      Text text(initial);

      text.initialize_free_space();

      CHECK(text == initial);
      CHECK(text != empty);

      for (size_t i = text.size(); i < text.max_size(); ++i)
      {
        CHECK_EQUAL(0, text[i]);
      }
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_update_after_c_string_max_size)
    {
      Text text;

      text.initialize_free_space();
      std::fill(text.data(), text.data() + text.max_size(), STR('A'));
      text.trim_to_terminator();

#if ETL_HAS_STRING_TRUNCATION_CHECKS
      CHECK_FALSE(text.is_truncated());
#endif
      CHECK_EQUAL(text.max_size(), text.size());
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_update_after_c_string_shorter_size)
    {
      Text text;

      text.initialize_free_space();
      std::fill(text.data(), text.data() + text.max_size() - 1, STR('A'));
      text.trim_to_terminator();

#if ETL_HAS_STRING_TRUNCATION_CHECKS
      CHECK_FALSE(text.is_truncated());
#endif
      CHECK_EQUAL(text.max_size() - 1, text.size());
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_update_after_c_string_greater_size)
    {
      Text text;

      text.initialize_free_space();
      std::fill(text.data(), text.data() + text.max_size() + 1, STR('A')); // Overwrites to terminating null.
      text.trim_to_terminator();

#if ETL_HAS_STRING_TRUNCATION_CHECKS
      CHECK_TRUE(text.is_truncated());
#else
      CHECK_FALSE(text.is_truncated());
#endif
      CHECK_EQUAL(text.max_size(), text.size());
    }
  };
}
#endif