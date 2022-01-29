/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl
https://www.etlcpp.com

Copyright(c) 2017 jwellbelove

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

#include "etl/string_view.h"
#include "etl/string.h"
#include "etl/wstring.h"
#include "etl/u16string.h"
#include "etl/u32string.h"
#include "etl/hash.h"

#include <algorithm>
#include <iterator>
#include <string>
#include <array>
#include <ostream>

namespace
{
  typedef etl::string_view View;

  etl::string<11> etltext    = "Hello World";
  std::string text           = "Hello World";
  std::wstring wtext         = L"Hello World";
  std::u16string u16text     = u"Hello World";
  std::u32string u32text     = U"Hello World";
  std::string text_smaller   = "Hello Worlc";
  std::string text_shorter   = "Hello Worl";
  std::string text_different = "Goodbye!!!!";

  char ctext[] = { 'H', 'e', 'l', 'l', 'o', ' ', 'W', 'o', 'r', 'l', 'd', '\0' };
  char* pctext = ctext;

  SUITE(test_string_view)
  {
    //*************************************************************************
    TEST(test_constexpr)
    {
      constexpr const char* text = "Hello";

      constexpr etl::string_view view1(text);
      constexpr etl::string_view view2(text, 5U);
      constexpr etl::string_view view3(text, text + 5U);
      constexpr etl::string_view view4(view3);
    }

    //*************************************************************************
    TEST(test_default_constructor)
    {
      View view;

      CHECK(0U == view.size());
      CHECK(0U == view.max_size());
      CHECK(view.empty());
    }

    //*************************************************************************
    TEST(test_constructor_pointer_range)
    {
      View view(pctext, pctext + strlen(pctext));

      CHECK(text.size() == view.size());
      CHECK(text.size() == view.max_size());

      bool isEqual = std::equal(view.begin(), view.end(), text.begin());
      CHECK(isEqual);
    }

    //*************************************************************************
    TEST(test_constructor_etl_string)
    {
      View view(etltext);

      CHECK(etltext.size() == view.size());
      CHECK(etltext.size() == view.max_size());

      bool isEqual = std::equal(view.begin(), view.end(), etltext.begin());
      CHECK(isEqual);
    }

    //*************************************************************************
    TEST(test_constructor_pointer_size)
    {
      View view(pctext, strlen(pctext));

      CHECK(text.size() == view.size());
      CHECK(text.size() == view.max_size());

      bool isEqual = std::equal(view.begin(), view.end(), text.begin());
      CHECK(isEqual);
    }

    //*************************************************************************
    TEST(test_constructor_pointer)
    {
      View view(pctext);

      CHECK(text.size() == view.size());
      CHECK(text.size() == view.max_size());

      bool isEqual = std::equal(view.begin(), view.end(), text.begin());
      CHECK(isEqual);
    }

    //*************************************************************************
    TEST(test_constructor_literal)
    {
      View view("Hello World");

      CHECK(text.size() == view.size());
      CHECK(text.size() == view.max_size());

      bool isEqual = std::equal(view.begin(), view.end(), text.begin());
      CHECK(isEqual);
    }

    //*************************************************************************
    TEST(test_make_string_view)
    {
      auto cview   = etl::make_string_view("Hello World");
      auto wview   = etl::make_string_view(L"Hello World");
      auto u16view = etl::make_string_view(u"Hello World");
      auto u32view = etl::make_string_view(U"Hello World");
    
      CHECK(std::equal(cview.begin(),   cview.end(),   text.begin()));
      CHECK(std::equal(wview.begin(),   wview.end(),   text.begin()));
      CHECK(std::equal(u16view.begin(), u16view.end(), text.begin()));
      CHECK(std::equal(u32view.begin(), u32view.end(), text.begin()));
    }

    //*************************************************************************
#if ETL_CPP17_SUPPORTED && ETL_USING_INITIALIZER_LIST && !defined(ETL_TEMPLATE_DEDUCTION_GUIDE_TESTS_DISABLED)
    TEST(test_template_deduction)
    {
      etl::basic_string_view cview{ "Hello World" };
      etl::basic_string_view wview{ L"Hello World" };
      etl::basic_string_view u16view{ u"Hello World" };
      etl::basic_string_view u32view{ U"Hello World" };

      CHECK(std::equal(cview.begin(),   cview.end(),   text.begin()));
      CHECK(std::equal(wview.begin(),   wview.end(),   text.begin()));
      CHECK(std::equal(u16view.begin(), u16view.end(), text.begin()));
      CHECK(std::equal(u32view.begin(), u32view.end(), text.begin()));
    }
#endif

    //*************************************************************************
    TEST(test_assign_from_string_view)
    {
      View view1(text.c_str());
      View view2;

      view2 = view1;

      CHECK(view1.size() == view2.size());
      CHECK(view1.max_size() == view2.max_size());

      bool isEqual;

      isEqual = std::equal(view1.begin(), view1.end(), view2.begin());
      CHECK(isEqual);
    }

    //*************************************************************************
    TEST(test_assign_from_iterator_range)
    {
      View view;

      view.assign(text.c_str(), text.size());

      CHECK(text.size() == view.size());
      CHECK(text.size() == view.max_size());

      bool isEqual;

      isEqual = std::equal(text.begin(), text.end(), view.begin());
      CHECK(isEqual);
    }

    //*************************************************************************
    TEST(test_begin_end)
    {
      View  view(text.c_str(), text.size());

      CHECK(view.begin() ==         view.cbegin());
      CHECK(view.rbegin().base() == view.crbegin().base());
      CHECK(view.end() ==           view.cend());
      CHECK(view.rend().base() ==   view.crend().base());
    }

    //*************************************************************************
    TEST(test_front_back)
    {
      View  view(text.c_str(), text.size());

      CHECK(text.front() == view.front());
      CHECK(text.back() ==  view.back());
    }

    //*************************************************************************
    TEST(test_data)
    {
      View  view(text.c_str(), text.size());

      CHECK(text.data() == view.data());
    }

    //*************************************************************************
    TEST(test_index_operator)
    {
      View  view(text.c_str(), text.size());

      for (size_t i = 0UL; i < text.size(); ++i)
      {
        CHECK(text[i] == view[i]);
      }
    }

    //*************************************************************************
    TEST(test_at)
    {
      View  view(text.c_str(), text.size());

      for (size_t i = 0UL; i < text.size(); ++i)
      {
        CHECK(text[i] == view.at(i));
      }
    }

    //*************************************************************************
    TEST(test_at_uninitialised_exception)
    {
      View  view;

      CHECK_THROW(view.at(0),  etl::string_view_uninitialised);
    }

    //*************************************************************************
    TEST(test_at_bounds_exception)
    {
      View  view(text.c_str(), text.size());

      CHECK_THROW(view.at(view.size()), etl::string_view_bounds);
    }

    //*************************************************************************
    TEST(test_non_member_same)
    {
      View  view1(text.c_str(), text.size());
      View  view2(text.c_str(), text.size());

      CHECK(view1 == view2);
      CHECK(view1 <= view2);
      CHECK(view1 >= view2);
      CHECK(!(view1 > view2));
      CHECK(!(view1 < view2));
    }

    //*************************************************************************
    TEST(test_non_member_smaller)
    {
      View  view1(text.c_str(), text.size());
      View  view2(text_smaller.c_str(), text_smaller.size());

      CHECK(!(view1 == view2));
      CHECK(!(view1 <= view2));
      CHECK(view2 <= view1);
      CHECK(view1 >= view2);
      CHECK(!(view2 >= view1));
      CHECK(view1 > view2);
      CHECK(!(view2 > view1));
      CHECK(!(view1 < view2));
      CHECK(view2 < view1);
    }

    //*************************************************************************
    TEST(test_non_member_shorter)
    {
      View  view1(text.c_str(), text.size());
      View  view2(text_shorter.c_str(), text_shorter.size());

      CHECK(!(view1 == view2));
      CHECK(!(view1 <= view2));
      CHECK(view2 <= view1);
      CHECK(view1 >= view2);
      CHECK(!(view2 >= view1));
      CHECK(view1 > view2);
      CHECK(!(view2 > view1));
      CHECK(!(view1 < view2));
      CHECK(view2 < view1);
    }

    //*************************************************************************
    TEST(test_empty)
    {
      View view1(text.c_str(), text.c_str());
      CHECK(view1.empty());

      View view2(text.c_str(), text.c_str() + 1);
      CHECK(!view2.empty());
    }

    //*************************************************************************
    TEST(test_swap)
    {
      View view1(text.c_str());
      View view2(text_smaller.c_str());

      std::swap(view1, view2);

      CHECK(text.size() == view1.size());
      CHECK(text_smaller.size() == view2.size());

      bool isEqual;

      isEqual = std::equal(view1.begin(), view1.end(), text_smaller.begin());
      CHECK(isEqual);

      isEqual = std::equal(view2.begin(), view2.end(), text.begin());
      CHECK(isEqual);
    }

    //*************************************************************************
    TEST(test_remove_prefix_suffix)
    {
      std::string original     = "Hello World";
      std::string prefix       = "llo World";
      std::string prefixsuffix = "llo Wor";
      View view(original.c_str());

      bool isEqual;

      view.remove_prefix(2);
      isEqual = std::equal(view.begin(), view.end(), prefix.begin());
      CHECK(isEqual);

      view.remove_suffix(2);
      isEqual = std::equal(view.begin(), view.end(), prefixsuffix.begin());
      CHECK(isEqual);
    }

    //*************************************************************************
    TEST(test_copy)
    {
      View view(text.c_str());
      std::array<char, 12> destination;
      std::array<char, 12> blank;
      size_t count;

      blank.fill(0);

      // Full text.
      destination.fill(0);
      count = view.copy(destination.data(), text.size(), 0);
      CHECK(text.size() == count);
      CHECK(text.size() == strlen(destination.data()));
      CHECK_ARRAY_EQUAL(text.data(), destination.data(), text.size());

      // From position 2, count OK.
      destination.fill(0);
      count = view.copy(destination.data(), text.size() - 2, 2);
      CHECK(text.size() - 2 == count);
      CHECK(text.size() - 2 == strlen(destination.data()));
      CHECK_ARRAY_EQUAL(text.data() + 2, destination.data(), text.size() - 2);

      // From position 2, count too large.
      destination.fill(0);
      count = view.copy(destination.data(), text.size(), 2);
      CHECK(text.size() - 2 == count);
      CHECK(text.size() - 2 == strlen(destination.data()));
      CHECK_ARRAY_EQUAL(text.data() + 2, destination.data(), text.size() - 2);

      // Position too large.
      destination.fill(0);
      count = view.copy(destination.data(), text.size(), 11);
      CHECK(0U == count);
      CHECK_ARRAY_EQUAL(blank.data(), destination.data(), text.size());

      // From position 2, short range.
      destination.fill(0);
      count = view.copy(destination.data(), text.size() - 4, 2);
      CHECK(text.size() - 4 == count);
      CHECK(text.size() - 4 == strlen(destination.data()));
      CHECK_ARRAY_EQUAL(text.data() + 2, destination.data(), text.size() - 4);
    }

    //*************************************************************************
    TEST(test_substr)
    {
      View view1(text.c_str());
      View view2(text.c_str() + 2, text.size() - 2);
      View view3;
      View view4(text.c_str() + 2, text.size() - 2);
      View sub;

      // Whole string.
      sub = view1.substr(0, text.size());
      CHECK(view1 == sub);

      // From from +2 to -2.
      sub = view1.substr(2, text.size() - 2);
      CHECK(view2 == sub);

      // Position too large.
      sub = view1.substr(text.size(), 2);
      CHECK(view3 == sub);

      // Count too large.
      sub = view1.substr(2, text.size());
      CHECK(view4 == sub);
    }

    //*************************************************************************
    TEST(test_compare_basic_string_view)
    {
      View view(text.c_str());
      View view_smaller(text_smaller.c_str());
      View view_shorter(text_shorter.c_str());

      CHECK(0 ==  view.compare(view));
      CHECK(-1 == view_smaller.compare(view));
      CHECK(-1 == view_shorter.compare(view));
      CHECK(1 ==  view.compare(view_smaller));
      CHECK(1 ==  view.compare(view_shorter));
    }

    //*************************************************************************
    TEST(test_compare_position_count_view)
    {
      View view(text.c_str());
      View view_smaller(text_smaller.c_str());
      View view_shorter(text_shorter.c_str());

      std::string text_long = std::string("xx") + text + std::string("xx");
      View view_long(text_long.c_str());

      std::string text_smaller_long = std::string("xx") + text_smaller + std::string("xx");
      View view_smaller_long(text_smaller_long.c_str());

      std::string text_shorter_long = std::string("xx") + text_shorter + std::string("xx");
      View view_shorter_long(text_shorter_long.c_str());

      CHECK(0 ==  view_long.compare(2, view.size(), view));
      CHECK(-1 == view_smaller_long.compare(2, view.size(), view));
      CHECK(1 ==  view_long.compare(2, view.size(), view_smaller));
      CHECK(1 ==  view_long.compare(2, view.size(), view_shorter));
    }

    //*************************************************************************
    TEST(test_compare_position_count_view_position_count)
    {
      View view(text.c_str());

      std::string text_long = std::string("xx") + text + std::string("xx");
      View view_long(text_long.c_str());

      std::string text_smaller_long = std::string("xx") + text_smaller + std::string("xx");
      View view_smaller_long(text_smaller_long.c_str());

      std::string text_shorter_long = std::string("xx") + text_shorter + std::string("xx");
      View view_shorter_long(text_shorter_long.c_str());

      CHECK(0 ==  view_long.compare(2, view.size(), view_long, 2, view.size()));
      CHECK(-1 == view_smaller_long.compare(2, view.size(), view_long, 2, view.size()));
      CHECK(-1 == view_shorter_long.compare(2, text_shorter.size(), view_long, 2, view.size()));
      CHECK(1 ==  view_long.compare(2, view.size(), view_smaller_long, 2, view.size()));
      CHECK(1 ==  view_long.compare(2, view.size(), view_shorter_long, 2, text_shorter.size()));
    }

    //*************************************************************************
    TEST(test_compare_text)
    {
      View view(text.c_str());
      View view_smaller(text_smaller.c_str());
      View view_shorter(text_shorter.c_str());

      CHECK(0 ==  view.compare(view.data()));
      CHECK(-1 == view_smaller.compare(view.data()));
      CHECK(1 ==  view.compare(view_smaller.data()));
      CHECK(-1 == view_shorter.compare(view.data()));
      CHECK(1 ==  view.compare(view_shorter.data()));
    }

    //*************************************************************************
    TEST(test_compare_position_count_text)
    {
      View view(text.c_str());
      View view_smaller(text_smaller.c_str());
      View view_shorter(text_shorter.c_str());

      std::string text_long = std::string("xx") + text + std::string("xx");
      View view_long(text_long.c_str());

      std::string text_smaller_long = std::string("xx") + text_smaller + std::string("xx");
      View view_smaller_long(text_smaller_long.c_str());

      std::string text_shorter_long = std::string("xx") + text_shorter + std::string("xx");
      View view_shorter_long(text_shorter_long.c_str());

      CHECK(0 == view_long.compare(2, view.size(), view.data()));
      CHECK(-1 == view_smaller_long.compare(2, view.size(), view_long.data()));
      CHECK(-1 == view_shorter_long.compare(2, text_shorter.size(), view_long.data()));
      CHECK(1 == view_long.compare(2, view.size(), view_smaller.data()));
      CHECK(1 == view_long.compare(2, view.size(), view_shorter.data()));
    }

    //*************************************************************************
    TEST(test_compare_position_count_text_count)
    {
      View view(text.c_str());
      View view_smaller(text_smaller.c_str());
      View view_shorter(text_shorter.c_str());

      std::string text_long = std::string("xx") + text + std::string("xx");
      View view_long(text_long.c_str());

      std::string text_smaller_long = std::string("xx") + text_smaller + std::string("xx");
      View view_smaller_long(text_smaller_long.c_str());

      std::string text_shorter_long = std::string("xx") + text_shorter + std::string("xx");
      View view_shorter_long(text_shorter_long.c_str());

      CHECK(0 == view_long.compare(2, view.size(), view.data(), view.size()));
      CHECK(-1 == view_smaller_long.compare(2, view.size(), view_long.data(), view_long.size()));
      CHECK(-1 == view_shorter_long.compare(2, text_shorter.size(), view_long.data(), view_long.size()));
      CHECK(1 == view_long.compare(2, view.size(), view_smaller.data(), view_smaller.size()));
      CHECK(1 == view_long.compare(2, view.size(), view_shorter.data(), view_shorter.size()));
    }

    //*************************************************************************
    TEST(test_starts_with)
    {
      View view(text.c_str());
      View start("Hello");
      View notstart("Gello");
      View notstartlong("Hello Worldxxxxxx");

      CHECK(view.starts_with(start));
      CHECK(!view.starts_with(notstart));
      CHECK(!view.starts_with(notstartlong));

      CHECK(view.starts_with('H'));
      CHECK(!view.starts_with('G'));

      CHECK(view.starts_with("Hello"));
      CHECK(!view.starts_with("Gello"));
      CHECK(!view.starts_with("Hello Worldxxxxxx"));
    }

    //*************************************************************************
    TEST(test_ends_with)
    {
      View view(text.c_str());
      View end("World");
      View notend("Xorld");
      View notendlong("Hello Worldxxxxxx");

      CHECK(view.ends_with(end));
      CHECK(!view.ends_with(notend));
      CHECK(!view.ends_with(notendlong));

      CHECK(view.ends_with('d'));
      CHECK(!view.ends_with('e'));

      CHECK(view.ends_with("World"));
      CHECK(!view.ends_with("Xorld"));
      CHECK(!view.ends_with("Hello Worldxxxxxx"));
    }

    //*************************************************************************
    TEST(test_find)
    {
      const char* s1 = "Hello";
      const char* s2 = "llo Wor";
      const char* s3 = "World";
      const char* s4 = "Xorld";
      const char* s5 = "Hello Worldxxxxxx";

      View view(text.c_str());
      View v1(s1);
      View v2(s2);
      View v3(s3);
      View v4(s4);
      View v5(s5);

      CHECK(0U == view.find(v1));
      CHECK(2U == view.find(v2));
      CHECK(2U == view.find(v2, 2));
      CHECK(6U == view.find(v3));
      CHECK(View::npos == view.find(v3, 7));
      CHECK(View::npos == view.find(v4));
      CHECK(View::npos == view.find(v5));

      CHECK(0U == view.find('H'));
      CHECK(2U == view.find('l'));
      CHECK(2U == view.find('l', 2));
      CHECK(View::npos == view.find('X'));

      CHECK(0U == view.find(s1));
      CHECK(2U == view.find(s2));
      CHECK(2U == view.find(s2, 2));
      CHECK(6U == view.find(s3));
      CHECK(View::npos == view.find(s3, 7));
      CHECK(View::npos == view.find(s4));
      CHECK(View::npos == view.find(s5));

      CHECK(0U == view.find(s1, 0, 3));
      CHECK(2U == view.find(s2, 0, 3));
      CHECK(2U == view.find(s2, 2, 3));
      CHECK(6U == view.find(s3, 0, 3));
      CHECK(View::npos == view.find(s3, 7, 3));
      CHECK(View::npos == view.find(s4, 0, 3));
      CHECK(View::npos == view.find(s5, 0, 15));
    }

    //*************************************************************************
    TEST(test_rfind)
    {
      const char* s1 = "ab";
      const char* s2 = "cd";
      const char* s3 = "bcd";
      const char* s4 = "abcdabcdabcd";

      View view("abcdabcdab");
      View v1(s1);
      View v2(s2);
      View v3(s3);
      View v4(s4);

      CHECK(8U == view.rfind(v1));
      CHECK(6U == view.rfind(v2));
      CHECK(2U == view.rfind(v2, 5));
      CHECK(View::npos == view.rfind(v4));

      CHECK(8U == view.rfind('a'));
      CHECK(6U == view.rfind('c'));
      CHECK(2U == view.rfind('c', 5));

      CHECK(8U == view.rfind(s1));
      CHECK(6U == view.rfind(s2));
      CHECK(2U == view.rfind(s2, 5));
      CHECK(View::npos == view.rfind(s4));

      CHECK(1U == view.rfind(s3, 5, 2));
      CHECK(View::npos == view.rfind(s4, 0, 11));
    }

    //*************************************************************************
    TEST(test_find_first_of)
    {
      const char* s1 = "l";
      const char* s2 = "o";
      const char* s3 = "W";
      const char* s4 = "Wol";
      const char* s5 = "wOL";

      View view("Hello World");
      View v1(s1);
      View v2(s2);
      View v3(s3);
      View v4(s4);
      View v5(s5);

      CHECK(2U == view.find_first_of(v1));
      CHECK(4U == view.find_first_of(v2));
      CHECK(6U == view.find_first_of(v3));
      CHECK(2U == view.find_first_of(v4));
      CHECK(View::npos == view.find_first_of(v5));

      CHECK(3U == view.find_first_of(v1, 3));
      CHECK(4U == view.find_first_of(v2, 2));
      CHECK(View::npos == view.find_first_of(v3, 7));
      CHECK(6U == view.find_first_of(v4, 5));
      CHECK(View::npos == view.find_first_of(v5, 2));

      CHECK(2U == view.find_first_of('l'));
      CHECK(4U == view.find_first_of('o'));
      CHECK(6U == view.find_first_of('W'));
      CHECK(View::npos == view.find_first_of('w'));

      CHECK(2U == view.find_first_of(s1));
      CHECK(4U == view.find_first_of(s2));
      CHECK(6U == view.find_first_of(s3));
      CHECK(2U == view.find_first_of(s4));
      CHECK(View::npos == view.find_first_of(s5));

      CHECK(3U == view.find_first_of(s1, 3));
      CHECK(4U == view.find_first_of(s2, 2));
      CHECK(View::npos == view.find_first_of(s3, 7));
      CHECK(6U == view.find_first_of(s4, 5));
      CHECK(View::npos == view.find_first_of(s5, 2));

      CHECK(3U == view.find_first_of(s1, 3, 1));
      CHECK(4U == view.find_first_of(s2, 2, 1));
      CHECK(View::npos == view.find_first_of(s3, 7, 1));
      CHECK(4U == view.find_first_of(s4, 0, 2));
      CHECK(View::npos == view.find_first_of(s5, 2, 3));
    }

    //*************************************************************************
    TEST(test_find_last_of)
    {
      const char* s1 = "l";
      const char* s2 = "o";
      const char* s3 = "W";
      const char* s4 = "Wol";
      const char* s5 = "wOL";

      View view("Hello World");
      View v1(s1);
      View v2(s2);
      View v3(s3);
      View v4(s4);
      View v5(s5);

      CHECK(9U == view.find_last_of(v1));
      CHECK(7U == view.find_last_of(v2));
      CHECK(6U == view.find_last_of(v3));
      CHECK(9U == view.find_last_of(v4));
      CHECK(View::npos == view.find_last_of(v5));

      CHECK(3U == view.find_last_of(v1, 3));
      CHECK(View::npos == view.find_last_of(v2, 2));
      CHECK(6U == view.find_last_of(v3, 7));
      CHECK(4U == view.find_last_of(v4, 5));
      CHECK(View::npos == view.find_last_of(v5, 2));

      CHECK(9U == view.find_last_of('l'));
      CHECK(7U == view.find_last_of('o'));
      CHECK(6U == view.find_last_of('W'));
      CHECK(View::npos == view.find_last_of('w'));

      CHECK(9U == view.find_last_of(s1));
      CHECK(7U == view.find_last_of(s2));
      CHECK(6U == view.find_last_of(s3));
      CHECK(9U == view.find_last_of(s4));
      CHECK(View::npos == view.find_last_of(s5));

      CHECK(3U == view.find_last_of(s1, 3));
      CHECK(View::npos == view.find_last_of(s2, 2));
      CHECK(6U == view.find_last_of(s3, 7));
      CHECK(4U == view.find_last_of(s4, 5));
      CHECK(View::npos == view.find_last_of(s5, 2));

      CHECK(3U == view.find_last_of(s1, 3, 1));
      CHECK(View::npos == view.find_last_of(s2, 2, 1));
      CHECK(6U == view.find_last_of(s3, 7, 1));
      CHECK(4U == view.find_last_of(s4, 5, 2));
      CHECK(View::npos == view.find_last_of(s5, 2, 3));
    }

    //*************************************************************************
    TEST(test_find_first_not_of)
    {
      const char* s1 = "H";
      const char* s2 = "eH";
      const char* s3 = "leH";
      const char* s4 = " olleH";
      const char* s5 = "wOL";
      const char* s6 = "Helo Wrd";

      View view("Hello World");
      View v1(s1);
      View v2(s2);
      View v3(s3);
      View v4(s4);
      View v5(s5);
      View v6(s6);

      CHECK(1U == view.find_first_not_of(v1));
      CHECK(2U == view.find_first_not_of(v2));
      CHECK(4U == view.find_first_not_of(v3));
      CHECK(6U == view.find_first_not_of(v4));
      CHECK(0U == view.find_first_not_of(v5));
      CHECK(View::npos == view.find_first_not_of(v6));

      CHECK(3U == view.find_first_not_of(v1, 3));
      CHECK(2U == view.find_first_not_of(v2, 2));
      CHECK(7U == view.find_first_not_of(v3, 7));
      CHECK(6U == view.find_first_not_of(v4, 5));
      CHECK(2U == view.find_first_not_of(v5, 2));
      CHECK(View::npos == view.find_first_not_of(v6, 0));

      CHECK(1U == view.find_first_not_of('H'));
      CHECK(0U == view.find_first_not_of('e'));

      CHECK(1U == view.find_first_not_of(s1));
      CHECK(2U == view.find_first_not_of(s2));
      CHECK(4U == view.find_first_not_of(s3));
      CHECK(6U == view.find_first_not_of(s4));
      CHECK(0U == view.find_first_not_of(s5));
      CHECK(View::npos == view.find_first_not_of(s6));

      CHECK(3U == view.find_first_not_of(s1, 3));
      CHECK(2U == view.find_first_not_of(s2, 2));
      CHECK(7U == view.find_first_not_of(s3, 7));
      CHECK(6U == view.find_first_not_of(s4, 5));
      CHECK(2U == view.find_first_not_of(s5, 2));
      CHECK(View::npos == view.find_first_not_of(s6, 0));

      CHECK(3U == view.find_first_not_of(s1, 3, 1));
      CHECK(2U == view.find_first_not_of(s2, 2, 1));
      CHECK(7U == view.find_first_not_of(s3, 7, 1));
      CHECK(6U == view.find_first_not_of(s4, 5, 2));
      CHECK(2U == view.find_first_not_of(s5, 2, 3));
      CHECK(View::npos == view.find_first_not_of(s6, 0, 8));
    }

    //*************************************************************************
    TEST(test_find_last_not_of)
    {
      const char* s1 = "d";
      const char* s2 = "dl";
      const char* s3 = "dlr";
      const char* s4 = "dlroW";
      const char* s5 = "DLROw";
      const char* s6 = "Helo Wrd";

      View view("Hello World");
      View v1(s1);
      View v2(s2);
      View v3(s3);
      View v4(s4);
      View v5(s5);
      View v6(s6);

      CHECK(9U == view.find_last_not_of(v1));
      CHECK(8U == view.find_last_not_of(v2));
      CHECK(7U == view.find_last_not_of(v3));
      CHECK(5U == view.find_last_not_of(v4));
      CHECK(10U == view.find_last_not_of(v5));
      CHECK(View::npos == view.find_last_not_of(v6));

      CHECK(9U == view.find_last_not_of(v1, 10));
      CHECK(8U == view.find_last_not_of(v2, 9));
      CHECK(7U == view.find_last_not_of(v3, 8));
      CHECK(5U == view.find_last_not_of(v4, 8));
      CHECK(8U == view.find_last_not_of(v5, 8));
      CHECK(View::npos == view.find_last_not_of(v6, 10));

      CHECK(9U == view.find_last_not_of('d'));
      CHECK(10U == view.find_last_not_of('l'));

      CHECK(9U == view.find_last_not_of(s1));
      CHECK(8U == view.find_last_not_of(s2));
      CHECK(7U == view.find_last_not_of(s3));
      CHECK(5U == view.find_last_not_of(s4));
      CHECK(10U == view.find_last_not_of(s5));
      CHECK(View::npos == view.find_last_not_of(s6));

      CHECK(9U == view.find_last_not_of(s1, 10));
      CHECK(8U == view.find_last_not_of(s2, 9));
      CHECK(7U == view.find_last_not_of(s3, 8));
      CHECK(5U == view.find_last_not_of(s4, 8));
      CHECK(8U == view.find_last_not_of(s5, 8));
      CHECK(View::npos == view.find_last_not_of(s6, 10));

      CHECK(9U == view.find_last_not_of(s1, 10, 1));
      CHECK(9U == view.find_last_not_of(s2, 9, 1));
      CHECK(8U == view.find_last_not_of(s3, 9, 2));
      CHECK(7U == view.find_last_not_of(s4, 8, 3));
      CHECK(8U == view.find_last_not_of(s5, 8, 5));
      CHECK(View::npos == view.find_last_not_of(s6, 10, 8));
    }

    //*************************************************************************
    TEST(test_hash)
    {
      typedef etl::string<11>    Text;
      typedef etl::wstring<11>   WText;
      typedef etl::u16string<11> U16Text;
      typedef etl::u32string<11> U32Text;

      typedef etl::string_view    View;
      typedef etl::wstring_view   WView;
      typedef etl::u16string_view U16View;
      typedef etl::u32string_view U32View;

      Text    text("Hello World");
      WText   wtext(L"Hello World");
      U16Text u16text(u"Hello World");
      U32Text u32text(U"Hello World");

      View    view(text.data());
      WView   wview(wtext.data());
      U16View u16view(u16text.data());
      U32View u32view(u32text.data());

      CHECK(etl::hash<Text>()(text) == etl::hash<View>()(view));
      CHECK(etl::hash<WText>()(wtext) == etl::hash<WView>()(wview));
      CHECK(etl::hash<U16Text>()(u16text) == etl::hash<U16View>()(u16view));
      CHECK(etl::hash<U32Text>()(u32text) == etl::hash<U32View>()(u32view));
    }
  };
}
