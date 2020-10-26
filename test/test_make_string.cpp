/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl
https://www.etlcpp.com

Copyright(c) 2019 jwellbelove

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

#include "etl/string.h"
#include "etl/wstring.h"
#include "etl/u16string.h"
#include "etl/u32string.h"

namespace
{
  SUITE(test_make_string)
  {

    //*************************************************************************
    template <typename T1, typename T2>
    bool Equal(const T1& compare_text, const T2& text)
    {
      return (compare_text.size() == text.size()) &&
             std::equal(text.begin(), text.end(), compare_text.begin());
    }

    //*************************************************************************
    struct SetupFixture
    {
    };

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_make_string)
    {
      size_t length = strlen("Hello World");

      auto ctext   = etl::make_string("Hello World");
      auto wtext   = etl::make_string(L"Hello World");
      auto u16text = etl::make_string(u"Hello World");
      auto u32text = etl::make_string(U"Hello World");

      CHECK_EQUAL(length, ctext.max_size());
      CHECK_EQUAL(length, wtext.max_size());
      CHECK_EQUAL(length, u16text.max_size());
      CHECK_EQUAL(length, u32text.max_size());

      CHECK(Equal(std::string("Hello World"), ctext));
      CHECK(Equal(std::wstring(L"Hello World"), ctext));
      CHECK(Equal(std::u16string(u"Hello World"), ctext));
      CHECK(Equal(std::u32string(U"Hello World"), ctext));
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_make_string_with_capacity)
    {
      constexpr size_t CAPACITY = 20;
      size_t length = strlen("Hello World");

      auto ctext   = etl::make_string_with_capacity<CAPACITY>("Hello World");
      auto wtext   = etl::make_string_with_capacity<CAPACITY>(L"Hello World");
      auto u16text = etl::make_string_with_capacity<CAPACITY>(u"Hello World");
      auto u32text = etl::make_string_with_capacity<CAPACITY>(U"Hello World");

      CHECK_EQUAL(CAPACITY, ctext.max_size());
      CHECK_EQUAL(length, ctext.size());
      CHECK(!ctext.truncated());

      CHECK_EQUAL(CAPACITY, wtext.max_size());
      CHECK_EQUAL(length, wtext.size());
      CHECK(!wtext.truncated());

      CHECK_EQUAL(CAPACITY, u16text.max_size());
      CHECK_EQUAL(length, u16text.size());
      CHECK(!u16text.truncated());

      CHECK_EQUAL(CAPACITY, u32text.max_size());
      CHECK_EQUAL(length, u32text.size());
      CHECK(!u32text.truncated());

      CHECK(Equal(std::string("Hello World"), ctext));
      CHECK(Equal(std::wstring(L"Hello World"), ctext));
      CHECK(Equal(std::u16string(u"Hello World"), ctext));
      CHECK(Equal(std::u32string(U"Hello World"), ctext));
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_make_string_with_capacity_truncated)
    {
      constexpr size_t CAPACITY = 10;
      size_t length = strlen("Hello World");

#if defined(ETL_STRING_TRUNCATION_IS_ERROR)
      CHECK_THROW(auto ctext   = etl::make_string_with_capacity<CAPACITY>("Hello World"),  etl::string_truncation);
      CHECK_THROW(auto wtext   = etl::make_string_with_capacity<CAPACITY>(L"Hello World"), etl::string_truncation);;
      CHECK_THROW(auto u16text = etl::make_string_with_capacity<CAPACITY>(u"Hello World"), etl::string_truncation);;
      CHECK_THROW(auto u32text = etl::make_string_with_capacity<CAPACITY>(U"Hello World"), etl::string_truncation);;
#else
      auto ctext   = etl::make_string_with_capacity<CAPACITY>("Hello World");
      auto wtext   = etl::make_string_with_capacity<CAPACITY>(L"Hello World");
      auto u16text = etl::make_string_with_capacity<CAPACITY>(u"Hello World");
      auto u32text = etl::make_string_with_capacity<CAPACITY>(U"Hello World");

      CHECK_EQUAL(CAPACITY, ctext.max_size());
      CHECK_EQUAL(length - 1, ctext.size());
      CHECK(ctext.truncated());

      CHECK_EQUAL(CAPACITY, wtext.max_size());
      CHECK_EQUAL(length - 1, wtext.size());
      CHECK(wtext.truncated());

      CHECK_EQUAL(CAPACITY, u16text.max_size());
      CHECK_EQUAL(length - 1, u16text.size());
      CHECK(u16text.truncated());

      CHECK_EQUAL(CAPACITY, u32text.max_size());
      CHECK_EQUAL(length - 1, u32text.size());
      CHECK(u32text.truncated());

      CHECK(Equal(std::string("Hello Worl"), ctext));
      CHECK(Equal(std::wstring(L"Hello Worl"), ctext));
      CHECK(Equal(std::u16string(u"Hello Worl"), ctext));
      CHECK(Equal(std::u32string(U"Hello Worl"), ctext));
#endif
    }
  };
}
