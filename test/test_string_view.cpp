/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl
https://www.etlcpp.com

Copyright(c) 2017 John Wellbelove

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
#include "etl/u8string.h"
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
  using View    = etl::string_view;
  using WView   = etl::wstring_view;
#if ETL_USING_CPP20
  using U8View  = etl::u8string_view;
#endif
  using U16View = etl::u16string_view;
  using U32View = etl::u32string_view;

  etl::string<11> etltext    = "Hello World";
  std::string text           = "Hello World";
  std::wstring wtext         = L"Hello World";
#if ETL_USING_CPP20
  std::u8string u8text       = u8"Hello World";
#endif
  std::u16string u16text     = u"Hello World";
  std::u32string u32text     = U"Hello World";
  std::string text_smaller   = "Hello Worlc";
  std::string text_shorter   = "Hello Worl";
  std::string text_different = "Goodbye!!!!";

  constexpr char     cctext[]   = { 'H', 'e', 'l', 'l', 'o', ' ', 'W', 'o', 'r', 'l', 'd', '\0' };
  constexpr wchar_t  cwtext[]   = { L'H', L'e', L'l', L'l', L'o', L' ', L'W', L'o', L'r', L'l', L'd', L'\0' };
  constexpr char16_t cu16text[] = { u'H', u'e', u'l', u'l', u'o', u' ', u'W', u'o', u'r', u'l', u'd', u'\0' };
  constexpr char32_t cu32text[] = { U'H', U'e', U'l', U'l', U'o', U' ', U'W', U'o', U'r', U'l', U'd', U'\0' };
  
  const char*     pctext   = cctext;
  const wchar_t*  pwtext   = cwtext;
  const char16_t* pu16text = cu16text;
  const char32_t* pu32text = cu32text;

  SUITE(test_string_view)
  {
    //*************************************************************************
#if ETL_USING_CPP14
    TEST(test_constexpr)
    {
      constexpr const char* text = "Hello";

      constexpr etl::string_view view1(text);
      constexpr etl::string_view view2(text, 5U);
      constexpr etl::string_view view3(text, text + 5U);
      constexpr etl::string_view view4(view3);

      constexpr const char* str1 = view1.begin();
      constexpr const char* str2 = view2.begin();
      constexpr const char* str3 = view3.begin();
      constexpr const char* str4 = view4.begin();

      CHECK_ARRAY_EQUAL(text, str1, 5U);
      CHECK_ARRAY_EQUAL(text, str2, 5U);
      CHECK_ARRAY_EQUAL(text, str3, 5U);
      CHECK_ARRAY_EQUAL(text, str4, 5U);
    }
#endif

    //*************************************************************************
    TEST(test_default_constructor)
    {
      View view;

      CHECK(0U == view.size());
      CHECK(0U == view.max_size());
      CHECK(view.empty());
    }

    //*************************************************************************
    TEST(test_constructor_pointer_range_char)
    {
      View view(pctext, pctext + etl::strlen(pctext));

      CHECK(text.size() == view.size());
      CHECK(text.size() == view.max_size());

      bool isEqual = std::equal(view.begin(), view.end(), text.begin());
      CHECK(isEqual);
    }

    //*************************************************************************
    TEST(test_constructor_pointer_range_wchar_t)
    {
      WView view(pwtext, pwtext + etl::strlen(pwtext));

      CHECK(text.size() == view.size());
      CHECK(text.size() == view.max_size());

      bool isEqual = std::equal(view.begin(), view.end(), text.begin());
      CHECK(isEqual);
    }

    //*************************************************************************
    TEST(test_constructor_pointer_range_u16char_t)
    {
      U16View view(pu16text, pu16text + etl::strlen(pu16text));

      CHECK(text.size() == view.size());
      CHECK(text.size() == view.max_size());

      bool isEqual = std::equal(view.begin(), view.end(), text.begin());
      CHECK(isEqual);
    }

    //*************************************************************************
    TEST(test_constructor_pointer_range_u32char_t)
    {
      U32View view(pu32text, pu32text + etl::strlen(pu32text));

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
      CHECK(std::equal(wview.begin(),   wview.end(),   wtext.begin()));
      CHECK(std::equal(u16view.begin(), u16view.end(), u16text.begin()));
      CHECK(std::equal(u32view.begin(), u32view.end(), u32text.begin()));
    }

    //*************************************************************************
#if ETL_USING_CPP14
    TEST(test_constexpr_make_string_view)
    {
      constexpr auto cview   = etl::make_string_view(cctext);
      constexpr auto wview   = etl::make_string_view(cwtext);
      constexpr auto u16view = etl::make_string_view(cu16text);
      constexpr auto u32view = etl::make_string_view(cu32text);

      CHECK(std::equal(cview.begin(),   cview.end(),   text.begin()));
      CHECK(std::equal(wview.begin(),   wview.end(),   text.begin()));
      CHECK(std::equal(u16view.begin(), u16view.end(), text.begin()));
      CHECK(std::equal(u32view.begin(), u32view.end(), text.begin()));
    }
#endif

    //*************************************************************************
    TEST(test_make_string_view_check_consistent_strings_from_arrays_of_char)
    {
      char text_extra_nulls[10] = { 'H', 'e', 'l', 'l', 'o' };
      char text_no_null[10]     = { 'H', 'e', 'l', 'l', 'o' };
      std::string text_expected = "Hello";

      auto view_extra_nulls = etl::make_string_view(text_extra_nulls);
      auto view_no_null     = etl::make_string_view(text_no_null);

      CHECK_EQUAL(text_expected.size(), view_extra_nulls.size());
      CHECK_EQUAL(text_expected.size(), view_no_null.size());

      CHECK(std::equal(view_extra_nulls.begin(), view_extra_nulls.end(), text_expected.begin()));
      CHECK(std::equal(view_no_null.begin(),     view_no_null.end(),     text_expected.begin()));
    }

    //*************************************************************************
    TEST(test_make_string_view_check_consistent_strings_from_arrays_of_wchar_t)
    {
      wchar_t text_extra_nulls[10] = { L'H', L'e', L'l', L'l', L'o' };
      wchar_t text_no_null[10] = { L'H', L'e', L'l', L'l', L'o' };
      std::wstring text_expected = L"Hello";

      auto view_extra_nulls = etl::make_string_view(text_extra_nulls);
      auto view_no_null = etl::make_string_view(text_no_null);

      CHECK_EQUAL(text_expected.size(), view_extra_nulls.size());
      CHECK_EQUAL(text_expected.size(), view_no_null.size());

      CHECK(std::equal(view_extra_nulls.begin(), view_extra_nulls.end(), text_expected.begin()));
      CHECK(std::equal(view_no_null.begin(), view_no_null.end(), text_expected.begin()));
    }

    //*************************************************************************
    TEST(test_make_string_view_check_consistent_strings_from_arrays_of_char16_t)
    {
      char16_t text_extra_nulls[10] = { u'H', u'e', u'l', u'l', u'o' };
      char16_t text_no_null[10] = { u'H', u'e', u'l', u'l', u'o' };
      std::u16string text_expected = u"Hello";

      auto view_extra_nulls = etl::make_string_view(text_extra_nulls);
      auto view_no_null = etl::make_string_view(text_no_null);

      CHECK_EQUAL(text_expected.size(), view_extra_nulls.size());
      CHECK_EQUAL(text_expected.size(), view_no_null.size());

      CHECK(std::equal(view_extra_nulls.begin(), view_extra_nulls.end(), text_expected.begin()));
      CHECK(std::equal(view_no_null.begin(), view_no_null.end(), text_expected.begin()));
    }

    //*************************************************************************
    TEST(test_make_string_view_check_consistent_strings_from_arrays_of_char32_t)
    {
      char32_t text_extra_nulls[10] = { U'H', U'e', U'l', U'l', U'o' };
      char32_t text_no_null[10] = { U'H', U'e', U'l', U'l', U'o' };
      std::u32string text_expected = U"Hello";

      auto view_extra_nulls = etl::make_string_view(text_extra_nulls);
      auto view_no_null = etl::make_string_view(text_no_null);

      CHECK_EQUAL(text_expected.size(), view_extra_nulls.size());
      CHECK_EQUAL(text_expected.size(), view_no_null.size());

      CHECK(std::equal(view_extra_nulls.begin(), view_extra_nulls.end(), text_expected.begin()));
      CHECK(std::equal(view_no_null.begin(), view_no_null.end(), text_expected.begin()));
    }

    //*************************************************************************
#if ETL_USING_CPP17 && ETL_HAS_INITIALIZER_LIST && !defined(ETL_TEMPLATE_DEDUCTION_GUIDE_TESTS_DISABLED)
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
    TEST(test_contains)
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

      CHECK_TRUE(view.contains(v1));
      CHECK_TRUE(view.contains(v2));
      CHECK_TRUE(view.contains(v3));
      CHECK_FALSE(view.contains(v4));
      CHECK_FALSE(view.contains(v5));

      CHECK_TRUE(view.contains('H'));
      CHECK_TRUE(view.contains('l'));
      CHECK_FALSE(view.contains('X'));

      CHECK_TRUE(view.contains(s1));
      CHECK_TRUE(view.contains(s2));
      CHECK_TRUE(view.contains(s3));
      CHECK_FALSE(view.contains(s4));
      CHECK_FALSE(view.contains(s5));
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

    //*************************************************************************
    TEST(string_view_literal)
    {
      typedef etl::string_view    View;
      typedef etl::wstring_view   WView;
      typedef etl::u16string_view U16View;
      typedef etl::u32string_view U32View;

      using namespace etl::literals::string_literals;

      View    view    =  "Hello World"_sv;
      WView   wview   = L"Hello World"_sv;
      U16View u16view = u"Hello World"_sv;
      U32View u32view = U"Hello World"_sv;

      CHECK_TRUE((view    == View{ "Hello World",     etl::strlen("Hello World") }));
      CHECK_TRUE((wview   == WView{ L"Hello World",   etl::strlen(L"Hello World") }));
      CHECK_TRUE((u16view == U16View{ u"Hello World", etl::strlen(u"Hello World") }));
      CHECK_TRUE((u32view == U32View{ U"Hello World", etl::strlen(U"Hello World") }));
    }
  };
}
