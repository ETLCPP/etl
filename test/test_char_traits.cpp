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
    TEST(test_strcmp_char)
    {
      const char s1[] = "ABCDEF";
      const char s2[] = "ABCDEF";
      const char s3[] = "ABDDEF";
      const char s4[] = "ABCDE";

      CHECK_TRUE(etl::strcmp(s1, s2) == 0);
      CHECK_TRUE(etl::strcmp(s2, s1) == 0);
      CHECK_TRUE(etl::strcmp(s1, s3) < 0);
      CHECK_TRUE(etl::strcmp(s3, s1) > 0 );
      CHECK_TRUE(etl::strcmp(s1, s4) > 0);
      CHECK_TRUE(etl::strcmp(s4, s1) < 0);
    }

#if ETL_HAS_CHAR8_T
    //*************************************************************************
    TEST(test_strcmp_char8_t)
    {
      const char8_t s1[] = u8"ABCDEF";
      const char8_t s2[] = u8"ABCDEF";
      const char8_t s3[] = u8"ABDDEF";
      const char8_t s4[] = u8"ABCDE";

      CHECK_TRUE(etl::strcmp(s1, s2) == 0);
      CHECK_TRUE(etl::strcmp(s2, s1) == 0);
      CHECK_TRUE(etl::strcmp(s1, s3) < 0);
      CHECK_TRUE(etl::strcmp(s3, s1) > 0);
      CHECK_TRUE(etl::strcmp(s1, s4) > 0);
      CHECK_TRUE(etl::strcmp(s4, s1) < 0);
    }
#endif

    //*************************************************************************
    TEST(test_strcmp_wchar_t)
    {
      const wchar_t s1[] = L"ABCDEF";
      const wchar_t s2[] = L"ABCDEF";
      const wchar_t s3[] = L"ABDDEF";
      const wchar_t s4[] = L"ABCDE";

      CHECK_TRUE(etl::strcmp(s1, s2) == 0);
      CHECK_TRUE(etl::strcmp(s2, s1) == 0);
      CHECK_TRUE(etl::strcmp(s1, s3) < 0);
      CHECK_TRUE(etl::strcmp(s3, s1) > 0);
      CHECK_TRUE(etl::strcmp(s1, s4) > 0);
      CHECK_TRUE(etl::strcmp(s4, s1) < 0);
    }

    //*************************************************************************
    TEST(test_strcmp_char16_t)
    {
      const char16_t s1[] = u"ABCDEF";
      const char16_t s2[] = u"ABCDEF";
      const char16_t s3[] = u"ABDDEF";
      const char16_t s4[] = u"ABCDE";

      CHECK_TRUE(etl::strcmp(s1, s2) == 0);
      CHECK_TRUE(etl::strcmp(s2, s1) == 0);
      CHECK_TRUE(etl::strcmp(s1, s3) < 0);
      CHECK_TRUE(etl::strcmp(s3, s1) > 0);
      CHECK_TRUE(etl::strcmp(s1, s4) > 0);
      CHECK_TRUE(etl::strcmp(s4, s1) < 0);
    }

    //*************************************************************************
    TEST(test_strcmp_char32_t)
    {
      const char32_t s1[] = U"ABCDEF";
      const char32_t s2[] = U"ABCDEF";
      const char32_t s3[] = U"ABDDEF";
      const char32_t s4[] = U"ABCDE";

      CHECK_TRUE(etl::strcmp(s1, s2) == 0);
      CHECK_TRUE(etl::strcmp(s1, s3) < 0);
      CHECK_TRUE(etl::strcmp(s3, s1) > 0);
      CHECK_TRUE(etl::strcmp(s1, s4) > 0);
      CHECK_TRUE(etl::strcmp(s4, s1) < 0);
    }

    //*************************************************************************
    TEST(test_strncmp_char)
    {
      const char s1[] = "ABCDEF";
      const char s2[] = "ABCDEF";
      const char s3[] = "ABDDEF";
      const char s4[] = "ABCDE";

      CHECK_TRUE(etl::strncmp(s1, s2, 0) == 0);
      CHECK_TRUE(etl::strncmp(s1, s2, 1) == 0);
      CHECK_TRUE(etl::strncmp(s1, s2, 2) == 0);
      CHECK_TRUE(etl::strncmp(s1, s2, 3) == 0);
      CHECK_TRUE(etl::strncmp(s1, s2, 4) == 0);
      CHECK_TRUE(etl::strncmp(s1, s2, 5) == 0);
      CHECK_TRUE(etl::strncmp(s1, s2, 6) == 0);
      CHECK_TRUE(etl::strncmp(s1, s3, 0) == 0);
      CHECK_TRUE(etl::strncmp(s3, s1, 0) == 0);
      CHECK_TRUE(etl::strncmp(s1, s3, 1) == 0);
      CHECK_TRUE(etl::strncmp(s3, s1, 1) == 0);
      CHECK_TRUE(etl::strncmp(s1, s3, 2) == 0);
      CHECK_TRUE(etl::strncmp(s3, s1, 2) == 0);
      CHECK_TRUE(etl::strncmp(s1, s3, 3) < 0);
      CHECK_TRUE(etl::strncmp(s3, s1, 3) > 0);
      CHECK_TRUE(etl::strncmp(s1, s3, 4) < 0);
      CHECK_TRUE(etl::strncmp(s3, s1, 4) > 0);
      CHECK_TRUE(etl::strncmp(s1, s3, 5) < 0);
      CHECK_TRUE(etl::strncmp(s3, s1, 5) > 0);
      CHECK_TRUE(etl::strncmp(s1, s3, 6) < 0);
      CHECK_TRUE(etl::strncmp(s3, s1, 6) > 0);
      CHECK_TRUE(etl::strncmp(s1, s4, 0) == 0);
      CHECK_TRUE(etl::strncmp(s4, s1, 0) == 0);
      CHECK_TRUE(etl::strncmp(s1, s4, 1) == 0);
      CHECK_TRUE(etl::strncmp(s4, s1, 1) == 0);
      CHECK_TRUE(etl::strncmp(s1, s4, 2) == 0);
      CHECK_TRUE(etl::strncmp(s4, s1, 2) == 0);
      CHECK_TRUE(etl::strncmp(s1, s4, 3) == 0);
      CHECK_TRUE(etl::strncmp(s4, s1, 3) == 0);
      CHECK_TRUE(etl::strncmp(s1, s4, 4) == 0);
      CHECK_TRUE(etl::strncmp(s4, s1, 4) == 0);
      CHECK_TRUE(etl::strncmp(s1, s4, 5) == 0);
      CHECK_TRUE(etl::strncmp(s4, s1, 5) == 0);
      CHECK_TRUE(etl::strncmp(s1, s4, 6) > 0);
      CHECK_TRUE(etl::strncmp(s4, s1, 6) < 0);
    }

#if ETL_HAS_CHAR8_T
    //*************************************************************************
    TEST(test_strncmp_char8_t)
    {
      const char8_t s1[] = u8"ABCDEF";
      const char8_t s2[] = u8"ABCDEF";
      const char8_t s3[] = u8"ABDDEF";
      const char8_t s4[] = u8"ABCDE";

      CHECK_TRUE(etl::strncmp(s1, s2, 0) == 0);
      CHECK_TRUE(etl::strncmp(s1, s2, 1) == 0);
      CHECK_TRUE(etl::strncmp(s1, s2, 2) == 0);
      CHECK_TRUE(etl::strncmp(s1, s2, 3) == 0);
      CHECK_TRUE(etl::strncmp(s1, s2, 4) == 0);
      CHECK_TRUE(etl::strncmp(s1, s2, 5) == 0);
      CHECK_TRUE(etl::strncmp(s1, s2, 6) == 0);
      CHECK_TRUE(etl::strncmp(s1, s3, 0) == 0);
      CHECK_TRUE(etl::strncmp(s3, s1, 0) == 0);
      CHECK_TRUE(etl::strncmp(s1, s3, 1) == 0);
      CHECK_TRUE(etl::strncmp(s3, s1, 1) == 0);
      CHECK_TRUE(etl::strncmp(s1, s3, 2) == 0);
      CHECK_TRUE(etl::strncmp(s3, s1, 2) == 0);
      CHECK_TRUE(etl::strncmp(s1, s3, 3) < 0);
      CHECK_TRUE(etl::strncmp(s3, s1, 3) > 0);
      CHECK_TRUE(etl::strncmp(s1, s3, 4) < 0);
      CHECK_TRUE(etl::strncmp(s3, s1, 4) > 0);
      CHECK_TRUE(etl::strncmp(s1, s3, 5) < 0);
      CHECK_TRUE(etl::strncmp(s3, s1, 5) > 0);
      CHECK_TRUE(etl::strncmp(s1, s3, 6) < 0);
      CHECK_TRUE(etl::strncmp(s3, s1, 6) > 0);
      CHECK_TRUE(etl::strncmp(s1, s4, 0) == 0);
      CHECK_TRUE(etl::strncmp(s4, s1, 0) == 0);
      CHECK_TRUE(etl::strncmp(s1, s4, 1) == 0);
      CHECK_TRUE(etl::strncmp(s4, s1, 1) == 0);
      CHECK_TRUE(etl::strncmp(s1, s4, 2) == 0);
      CHECK_TRUE(etl::strncmp(s4, s1, 2) == 0);
      CHECK_TRUE(etl::strncmp(s1, s4, 3) == 0);
      CHECK_TRUE(etl::strncmp(s4, s1, 3) == 0);
      CHECK_TRUE(etl::strncmp(s1, s4, 4) == 0);
      CHECK_TRUE(etl::strncmp(s4, s1, 4) == 0);
      CHECK_TRUE(etl::strncmp(s1, s4, 5) == 0);
      CHECK_TRUE(etl::strncmp(s4, s1, 5) == 0);
      CHECK_TRUE(etl::strncmp(s1, s4, 6) > 0);
      CHECK_TRUE(etl::strncmp(s4, s1, 6) < 0);
    }
#endif

    //*************************************************************************
    TEST(test_strncmp_wchar_t)
    {
      const wchar_t s1[] = L"ABCDEF";
      const wchar_t s2[] = L"ABCDEF";
      const wchar_t s3[] = L"ABDDEF";
      const wchar_t s4[] = L"ABCDE";

      CHECK_TRUE(etl::strncmp(s1, s2, 0) == 0);
      CHECK_TRUE(etl::strncmp(s1, s2, 1) == 0);
      CHECK_TRUE(etl::strncmp(s1, s2, 2) == 0);
      CHECK_TRUE(etl::strncmp(s1, s2, 3) == 0);
      CHECK_TRUE(etl::strncmp(s1, s2, 4) == 0);
      CHECK_TRUE(etl::strncmp(s1, s2, 5) == 0);
      CHECK_TRUE(etl::strncmp(s1, s2, 6) == 0);
      CHECK_TRUE(etl::strncmp(s1, s3, 0) == 0);
      CHECK_TRUE(etl::strncmp(s3, s1, 0) == 0);
      CHECK_TRUE(etl::strncmp(s1, s3, 1) == 0);
      CHECK_TRUE(etl::strncmp(s3, s1, 1) == 0);
      CHECK_TRUE(etl::strncmp(s1, s3, 2) == 0);
      CHECK_TRUE(etl::strncmp(s3, s1, 2) == 0);
      CHECK_TRUE(etl::strncmp(s1, s3, 3) < 0);
      CHECK_TRUE(etl::strncmp(s3, s1, 3) > 0);
      CHECK_TRUE(etl::strncmp(s1, s3, 4) < 0);
      CHECK_TRUE(etl::strncmp(s3, s1, 4) > 0);
      CHECK_TRUE(etl::strncmp(s1, s3, 5) < 0);
      CHECK_TRUE(etl::strncmp(s3, s1, 5) > 0);
      CHECK_TRUE(etl::strncmp(s1, s3, 6) < 0);
      CHECK_TRUE(etl::strncmp(s3, s1, 6) > 0);
      CHECK_TRUE(etl::strncmp(s1, s4, 0) == 0);
      CHECK_TRUE(etl::strncmp(s4, s1, 0) == 0);
      CHECK_TRUE(etl::strncmp(s1, s4, 1) == 0);
      CHECK_TRUE(etl::strncmp(s4, s1, 1) == 0);
      CHECK_TRUE(etl::strncmp(s1, s4, 2) == 0);
      CHECK_TRUE(etl::strncmp(s4, s1, 2) == 0);
      CHECK_TRUE(etl::strncmp(s1, s4, 3) == 0);
      CHECK_TRUE(etl::strncmp(s4, s1, 3) == 0);
      CHECK_TRUE(etl::strncmp(s1, s4, 4) == 0);
      CHECK_TRUE(etl::strncmp(s4, s1, 4) == 0);
      CHECK_TRUE(etl::strncmp(s1, s4, 5) == 0);
      CHECK_TRUE(etl::strncmp(s4, s1, 5) == 0);
      CHECK_TRUE(etl::strncmp(s1, s4, 6) > 0);
      CHECK_TRUE(etl::strncmp(s4, s1, 6) < 0);
    }

    //*************************************************************************
    TEST(test_strncmp_char16_t)
    {
      const char16_t s1[] = u"ABCDEF";
      const char16_t s2[] = u"ABCDEF";
      const char16_t s3[] = u"ABDDEF";
      const char16_t s4[] = u"ABCDE";

      CHECK_TRUE(etl::strncmp(s1, s2, 0) == 0);
      CHECK_TRUE(etl::strncmp(s1, s2, 1) == 0);
      CHECK_TRUE(etl::strncmp(s1, s2, 2) == 0);
      CHECK_TRUE(etl::strncmp(s1, s2, 3) == 0);
      CHECK_TRUE(etl::strncmp(s1, s2, 4) == 0);
      CHECK_TRUE(etl::strncmp(s1, s2, 5) == 0);
      CHECK_TRUE(etl::strncmp(s1, s2, 6) == 0);
      CHECK_TRUE(etl::strncmp(s1, s3, 0) == 0);
      CHECK_TRUE(etl::strncmp(s3, s1, 0) == 0);
      CHECK_TRUE(etl::strncmp(s1, s3, 1) == 0);
      CHECK_TRUE(etl::strncmp(s3, s1, 1) == 0);
      CHECK_TRUE(etl::strncmp(s1, s3, 2) == 0);
      CHECK_TRUE(etl::strncmp(s3, s1, 2) == 0);
      CHECK_TRUE(etl::strncmp(s1, s3, 3) < 0);
      CHECK_TRUE(etl::strncmp(s3, s1, 3) > 0);
      CHECK_TRUE(etl::strncmp(s1, s3, 4) < 0);
      CHECK_TRUE(etl::strncmp(s3, s1, 4) > 0);
      CHECK_TRUE(etl::strncmp(s1, s3, 5) < 0);
      CHECK_TRUE(etl::strncmp(s3, s1, 5) > 0);
      CHECK_TRUE(etl::strncmp(s1, s3, 6) < 0);
      CHECK_TRUE(etl::strncmp(s3, s1, 6) > 0);
      CHECK_TRUE(etl::strncmp(s1, s4, 0) == 0);
      CHECK_TRUE(etl::strncmp(s4, s1, 0) == 0);
      CHECK_TRUE(etl::strncmp(s1, s4, 1) == 0);
      CHECK_TRUE(etl::strncmp(s4, s1, 1) == 0);
      CHECK_TRUE(etl::strncmp(s1, s4, 2) == 0);
      CHECK_TRUE(etl::strncmp(s4, s1, 2) == 0);
      CHECK_TRUE(etl::strncmp(s1, s4, 3) == 0);
      CHECK_TRUE(etl::strncmp(s4, s1, 3) == 0);
      CHECK_TRUE(etl::strncmp(s1, s4, 4) == 0);
      CHECK_TRUE(etl::strncmp(s4, s1, 4) == 0);
      CHECK_TRUE(etl::strncmp(s1, s4, 5) == 0);
      CHECK_TRUE(etl::strncmp(s4, s1, 5) == 0);
      CHECK_TRUE(etl::strncmp(s1, s4, 6) > 0);
      CHECK_TRUE(etl::strncmp(s4, s1, 6) < 0);
    }

    //*************************************************************************
    TEST(test_strncmp_char32_t)
    {
      const char32_t s1[] = U"ABCDEF";
      const char32_t s2[] = U"ABCDEF";
      const char32_t s3[] = U"ABDDEF";
      const char32_t s4[] = U"ABCDE";

      CHECK_TRUE(etl::strncmp(s1, s2, 0) == 0);
      CHECK_TRUE(etl::strncmp(s1, s2, 1) == 0);
      CHECK_TRUE(etl::strncmp(s1, s2, 2) == 0);
      CHECK_TRUE(etl::strncmp(s1, s2, 3) == 0);
      CHECK_TRUE(etl::strncmp(s1, s2, 4) == 0);
      CHECK_TRUE(etl::strncmp(s1, s2, 5) == 0);
      CHECK_TRUE(etl::strncmp(s1, s2, 6) == 0);
      CHECK_TRUE(etl::strncmp(s1, s3, 0) == 0);
      CHECK_TRUE(etl::strncmp(s3, s1, 0) == 0);
      CHECK_TRUE(etl::strncmp(s1, s3, 1) == 0);
      CHECK_TRUE(etl::strncmp(s3, s1, 1) == 0);
      CHECK_TRUE(etl::strncmp(s1, s3, 2) == 0);
      CHECK_TRUE(etl::strncmp(s3, s1, 2) == 0);
      CHECK_TRUE(etl::strncmp(s1, s3, 3) < 0);
      CHECK_TRUE(etl::strncmp(s3, s1, 3) > 0);
      CHECK_TRUE(etl::strncmp(s1, s3, 4) < 0);
      CHECK_TRUE(etl::strncmp(s3, s1, 4) > 0);
      CHECK_TRUE(etl::strncmp(s1, s3, 5) < 0);
      CHECK_TRUE(etl::strncmp(s3, s1, 5) > 0);
      CHECK_TRUE(etl::strncmp(s1, s3, 6) < 0);
      CHECK_TRUE(etl::strncmp(s3, s1, 6) > 0);
      CHECK_TRUE(etl::strncmp(s1, s4, 0) == 0);
      CHECK_TRUE(etl::strncmp(s4, s1, 0) == 0);
      CHECK_TRUE(etl::strncmp(s1, s4, 1) == 0);
      CHECK_TRUE(etl::strncmp(s4, s1, 1) == 0);
      CHECK_TRUE(etl::strncmp(s1, s4, 2) == 0);
      CHECK_TRUE(etl::strncmp(s4, s1, 2) == 0);
      CHECK_TRUE(etl::strncmp(s1, s4, 3) == 0);
      CHECK_TRUE(etl::strncmp(s4, s1, 3) == 0);
      CHECK_TRUE(etl::strncmp(s1, s4, 4) == 0);
      CHECK_TRUE(etl::strncmp(s4, s1, 4) == 0);
      CHECK_TRUE(etl::strncmp(s1, s4, 5) == 0);
      CHECK_TRUE(etl::strncmp(s4, s1, 5) == 0);
      CHECK_TRUE(etl::strncmp(s1, s4, 6) > 0);
      CHECK_TRUE(etl::strncmp(s4, s1, 6) < 0);
    }

    //*************************************************************************
    TEST(test_strcpy_char)
    {
      const char src[] = "ABCDEF";
      char dst[7U];

      auto result = etl::strcpy(dst, src);

      CHECK_TRUE(etl::strcmp(src, dst) == 0);
      CHECK_TRUE(etl::strcmp(dst, result) == 0);
    }

#if ETL_HAS_CHAR8_T
    //*************************************************************************
    TEST(test_strcpy_char8_t)
    {
      const char8_t src[] = u8"ABCDEF";
      char8_t dst[7U];

      auto result = etl::strcpy(dst, src);

      CHECK_TRUE(etl::strcmp(src, dst) == 0);
      CHECK_TRUE(etl::strcmp(dst, result) == 0);
    }
#endif

    //*************************************************************************
    TEST(test_strcpy_wchar_t)
    {
      const wchar_t src[] = L"ABCDEF";
      wchar_t dst[7U];

      auto result = etl::strcpy(dst, src);

      CHECK_TRUE(etl::strcmp(src, dst) == 0);
      CHECK_TRUE(etl::strcmp(dst, result) == 0);
    }

    //*************************************************************************
    TEST(test_strcpy_char16_t)
    {
      const char16_t src[] = u"ABCDEF";
      char16_t dst[7U];

      auto result = etl::strcpy(dst, src);

      CHECK_TRUE(etl::strcmp(src, dst) == 0);
      CHECK_TRUE(etl::strcmp(dst, result) == 0);
    }

    //*************************************************************************
    TEST(test_strcpy_char32_t)
    {
      const char32_t src[] = U"ABCDEF";
      char32_t dst[7U];

      auto result = etl::strcpy(dst, src);

      CHECK_TRUE(etl::strcmp(src, dst) == 0);
      CHECK_TRUE(etl::strcmp(dst, result) == 0);
    }

    //*************************************************************************
    TEST(test_strncpy_char)
    {
      const char src[] = "ABCDEF";
      const char expected[] = "ABCDE";
      char dst[7U];

      auto result = etl::strncpy(dst, src, 5);

      CHECK_TRUE(etl::strcmp(expected, dst) == 0);
      CHECK_TRUE(etl::strcmp(dst, result) == 0);
    }

#if ETL_HAS_CHAR8_T
    //*************************************************************************
    TEST(test_strncpy_char8_t)
    {
      const char8_t src[] = u8"ABCDEF";
      const char8_t expected[] = u8"ABCDE";
      char8_t dst[7U];

      auto result = etl::strncpy(dst, src, 5);

      CHECK_TRUE(etl::strcmp(expected, dst) == 0);
      CHECK_TRUE(etl::strcmp(dst, result) == 0);
    }
#endif

    //*************************************************************************
    TEST(test_strncpy_wchar_t)
    {
      const wchar_t src[] = L"ABCDEF";
      const wchar_t expected[] = L"ABCDE";
      wchar_t dst[7U];

      auto result = etl::strncpy(dst, src, 5);

      CHECK_TRUE(etl::strcmp(expected, dst) == 0);
      CHECK_TRUE(etl::strcmp(dst, result) == 0);
    }

    //*************************************************************************
    TEST(test_strncpy_char16_t)
    {
      const char16_t src[] = u"ABCDEF";
      const char16_t expected[] = u"ABCDE";
      char16_t dst[7U];

      auto result = etl::strncpy(dst, src, 5);

      CHECK_TRUE(etl::strcmp(expected, dst) == 0);
      CHECK_TRUE(etl::strcmp(dst, result) == 0);
    }

    //*************************************************************************
    TEST(test_strncpy_char32_t)
    {
      const char32_t src[] = U"ABCDEF";
      const char32_t expected[] = U"ABCDE";
      char32_t dst[7U];

      auto result = etl::strncpy(dst, src, 5);

      CHECK_TRUE(etl::strcmp(expected, dst) == 0);
      CHECK_TRUE(etl::strcmp(dst, result) == 0);
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
