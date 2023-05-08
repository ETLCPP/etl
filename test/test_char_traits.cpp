/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl
https://www.etlcpp.com

Documentation: 

Copyright(c) 2022 John Wellbelove

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

#include "etl/char_traits.h"

namespace
{
  template <typename T>
  size_t length(const T* text)
  {
    size_t count = 0U;

    while (*text != 0U)
    {
      ++text;
      ++count;
    }

    return count;
  }

  SUITE(test_char_traits)
  {
    //*************************************************************************
    TEST(test_strlen)
    {
      char data1[etl::strlen("qwerty")];
      char data2[etl::strlen(L"qwerty")];
      char data3[etl::strlen(u"qwerty")];
      char data4[etl::strlen(U"qwerty")];

      CHECK_EQUAL(6U, sizeof(data1));
      CHECK_EQUAL(6U, sizeof(data2));
      CHECK_EQUAL(6U, sizeof(data3));
      CHECK_EQUAL(6U, sizeof(data4));
    }

    //*************************************************************************
    TEST(test_char_traits_char_template)
    {
      using char_traits = etl::char_traits<char>;
      using char_type   = char_traits::char_type;
      using int_type    = char_traits::int_type;

      char_type r = 'A';
      char_type c = 'B';
      char_type src[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
      char_type dst[ETL_OR_STD17::size(src)];
      char_type filled[] = { 9, 9, 9, 9, 9, 9, 9, 9, 9, 9 };
      const char_type* p_src;
      char_type* p_dst;

      char_traits::assign(r, c);
      CHECK_EQUAL(r, 'B');

      CHECK(char_traits::eq(1, 1));
      CHECK(!char_traits::eq(1, 2));
      CHECK(!char_traits::eq(2, 1));

      CHECK(!char_traits::lt(1, 1));
      CHECK(char_traits::lt(1, 2));
      CHECK(!char_traits::lt(2, 1));

      CHECK_EQUAL(length<char_type>("ABCDEF"), char_traits::length("ABCDEF"));

      CHECK_EQUAL(0,  char_traits::compare("ABCDEF", "ABCDEF", 6U));
      CHECK_EQUAL(-1, char_traits::compare("ABCDEE", "ABCDEF", 6U));
      CHECK_EQUAL(1,  char_traits::compare("ABCDEF", "ABCDEE", 6U));

      p_dst = char_traits::assign(dst, ETL_OR_STD17::size(dst), 9);
      CHECK_ARRAY_EQUAL(filled, p_dst, ETL_OR_STD17::size(filled));

      std::fill_n(dst, ETL_OR_STD17::size(dst), 0);
      p_dst = char_traits::copy(dst, src, ETL_OR_STD17::size(src));
      CHECK_ARRAY_EQUAL(src, p_dst, ETL_OR_STD17::size(src));
      
      std::fill_n(dst, ETL_OR_STD17::size(dst), 0);
      p_dst = char_traits::move(dst, src, ETL_OR_STD17::size(src));
      CHECK_ARRAY_EQUAL(src, p_dst, ETL_OR_STD17::size(src));

      p_src = char_traits::find(src, ETL_OR_STD17::size(src), 4);
      CHECK_EQUAL(src[4], *p_src);

      CHECK_EQUAL(127, char_traits::to_char_type(int_type(127)));
      CHECK_EQUAL(127, char_traits::to_int_type(char_type(127)));

      CHECK(!char_traits::eq_int_type(0, 1));
      CHECK(char_traits::eq_int_type(1, 1));

      CHECK(int_type(char_traits::eof())     != char_traits::not_eof(char_traits::eof()));
      CHECK(int_type(char_traits::eof() + 1) == char_traits::not_eof(char_traits::eof() + 1));
    }

    //*************************************************************************
    TEST(test_char_traits_wchar_t_template)
    {
      using char_traits = etl::char_traits<wchar_t>;
      using char_type   = char_traits::char_type;
      using int_type    = char_traits::int_type;

      char_type r = L'A';
      char_type c = L'B';
      char_type src[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
      char_type dst[ETL_OR_STD17::size(src)];
      char_type filled[] = { 9, 9, 9, 9, 9, 9, 9, 9, 9, 9 };
      const char_type* p_src;
      char_type* p_dst;

      char_traits::assign(r, c);
      CHECK_EQUAL(r, L'B');

      CHECK(char_traits::eq(1, 1));
      CHECK(!char_traits::eq(1, 2));
      CHECK(!char_traits::eq(2, 1));

      CHECK(!char_traits::lt(1, 1));
      CHECK(char_traits::lt(1, 2));
      CHECK(!char_traits::lt(2, 1));

      CHECK_EQUAL(length<char_type>(L"ABCDEF"), char_traits::length(L"ABCDEF"));

      CHECK_EQUAL(0, char_traits::compare(L"ABCDEF", L"ABCDEF", 6U));
      CHECK_EQUAL(-1, char_traits::compare(L"ABCDEE", L"ABCDEF", 6U));
      CHECK_EQUAL(1, char_traits::compare(L"ABCDEF", L"ABCDEE", 6U));

      p_dst = char_traits::assign(dst, ETL_OR_STD17::size(dst), 9);
      CHECK_ARRAY_EQUAL(filled, p_dst, ETL_OR_STD17::size(filled));

      std::fill_n(dst, ETL_OR_STD17::size(dst), 0);
      p_dst = char_traits::copy(dst, src, ETL_OR_STD17::size(src));
      CHECK_ARRAY_EQUAL(src, p_dst, ETL_OR_STD17::size(src));

      std::fill_n(dst, ETL_OR_STD17::size(dst), 0);
      p_dst = char_traits::move(dst, src, ETL_OR_STD17::size(src));
      CHECK_ARRAY_EQUAL(src, p_dst, ETL_OR_STD17::size(src));

      p_src = char_traits::find(src, ETL_OR_STD17::size(src), 4);
      CHECK_EQUAL(src[4], *p_src);

      CHECK_TRUE(127 == char_traits::to_char_type(int_type(127)));
      CHECK_TRUE(127 == char_traits::to_int_type(char_type(127)));

      CHECK(!char_traits::eq_int_type(0, 1));
      CHECK(char_traits::eq_int_type(1, 1));

      CHECK(int_type(char_traits::eof())     != int_type(char_traits::not_eof(char_traits::eof())));
//      CHECK(int_type(char_traits::eof() + 1) == int_type(char_traits::not_eof(char_traits::eof() + 1)));
    }

    //*************************************************************************
    TEST(test_char_traits_char16_t_template)
    {
      using char_traits = etl::char_traits<char16_t>;
      using char_type   = char_traits::char_type;
      using int_type    = char_traits::int_type;

      char_type r = u'A';
      char_type c = u'B';
      char_type src[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
      char_type dst[ETL_OR_STD17::size(src)];
      char_type filled[] = { 9, 9, 9, 9, 9, 9, 9, 9, 9, 9 };
      const char_type* p_src;
      char_type* p_dst;

      char_traits::assign(r, c);
      CHECK_EQUAL(r, 'B');

      CHECK(char_traits::eq(1, 1));
      CHECK(!char_traits::eq(1, 2));
      CHECK(!char_traits::eq(2, 1));

      CHECK(!char_traits::lt(1, 1));
      CHECK(char_traits::lt(1, 2));
      CHECK(!char_traits::lt(2, 1));

      CHECK_EQUAL(length<char_type>(u"ABCDEF"), char_traits::length(u"ABCDEF"));

      CHECK_EQUAL(0, char_traits::compare(u"ABCDEF", u"ABCDEF", 6U));
      CHECK_EQUAL(-1, char_traits::compare(u"ABCDEE", u"ABCDEF", 6U));
      CHECK_EQUAL(1, char_traits::compare(u"ABCDEF", u"ABCDEE", 6U));

      p_dst = char_traits::assign(dst, ETL_OR_STD17::size(dst), 9);
      CHECK_ARRAY_EQUAL(filled, p_dst, ETL_OR_STD17::size(filled));

      std::fill_n(dst, ETL_OR_STD17::size(dst), 0);
      p_dst = char_traits::copy(dst, src, ETL_OR_STD17::size(src));
      CHECK_ARRAY_EQUAL(src, p_dst, ETL_OR_STD17::size(src));

      std::fill_n(dst, ETL_OR_STD17::size(dst), 0);
      p_dst = char_traits::move(dst, src, ETL_OR_STD17::size(src));
      CHECK_ARRAY_EQUAL(src, p_dst, ETL_OR_STD17::size(src));

      p_src = char_traits::find(src, ETL_OR_STD17::size(src), 4);
      CHECK_EQUAL(src[4], *p_src);

      CHECK_EQUAL(127, char_type(127));

      CHECK(!char_traits::eq_int_type(0, 1));
      CHECK(char_traits::eq_int_type(1, 1));

      CHECK(int_type(char_traits::eof())     != char_traits::not_eof(char_traits::eof()));
//      CHECK(int_type(char_traits::eof() + 1) == char_traits::not_eof(char_traits::eof() + 1));
    }

    //*************************************************************************
    TEST(test_char_traits_char32_t_template)
    {
      using char_traits = etl::char_traits<char32_t>;
      using char_type   = char_traits::char_type;
      using int_type    = char_traits::int_type;

      char_type r = U'A';
      char_type c = U'B';
      char_type src[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
      char_type dst[ETL_OR_STD17::size(src)];
      char_type filled[] = { 9, 9, 9, 9, 9, 9, 9, 9, 9, 9 };
      const char_type* p_src;
      char_type* p_dst;

      char_traits::assign(r, c);
      CHECK_EQUAL(r, 'B');

      CHECK(char_traits::eq(1, 1));
      CHECK(!char_traits::eq(1, 2));
      CHECK(!char_traits::eq(2, 1));

      CHECK(!char_traits::lt(1, 1));
      CHECK(char_traits::lt(1, 2));
      CHECK(!char_traits::lt(2, 1));

      CHECK_EQUAL(length<char_type>(U"ABCDEF"), char_traits::length(U"ABCDEF"));

      CHECK_EQUAL(0, char_traits::compare(U"ABCDEF", U"ABCDEF", 6U));
      CHECK_EQUAL(-1, char_traits::compare(U"ABCDEE", U"ABCDEF", 6U));
      CHECK_EQUAL(1, char_traits::compare(U"ABCDEF", U"ABCDEE", 6U));

      p_dst = char_traits::assign(dst, ETL_OR_STD17::size(dst), 9);
      CHECK_ARRAY_EQUAL(filled, p_dst, ETL_OR_STD17::size(filled));

      std::fill_n(dst, ETL_OR_STD17::size(dst), 0);
      p_dst = char_traits::copy(dst, src, ETL_OR_STD17::size(src));
      CHECK_ARRAY_EQUAL(src, p_dst, ETL_OR_STD17::size(src));

      std::fill_n(dst, ETL_OR_STD17::size(dst), 0);
      p_dst = char_traits::move(dst, src, ETL_OR_STD17::size(src));
      CHECK_ARRAY_EQUAL(src, p_dst, ETL_OR_STD17::size(src));

      p_src = char_traits::find(src, ETL_OR_STD17::size(src), 4);
      CHECK_EQUAL(src[4], *p_src);

      CHECK_EQUAL(127, char_traits::to_int_type(char_type(127)));

      CHECK(!char_traits::eq_int_type(0, 1));
      CHECK(char_traits::eq_int_type(1, 1));

      CHECK(int_type(char_traits::eof())     != char_traits::not_eof(char_traits::eof()));
      CHECK(int_type(char_traits::eof() + 1) == char_traits::not_eof(char_traits::eof() + 1));
    }
  };
}
