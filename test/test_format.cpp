/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl
https://www.etlcpp.com

Copyright(c) 2025 BMW AG

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

#include "etl/format.h"

#include "etl/iterator.h"

#if ETL_USING_CPP11

namespace
{
  using iterator = etl::back_insert_iterator<etl::istring>;

  template<class... Args>
  etl::istring& test_format(etl::istring& s, etl::format_string<Args...> fmt, Args&&... args)
  {
    (void) etl::format_to(s, fmt, etl::forward<Args>(args)...);
    return s;
  }

  SUITE(test_format)
  {

    //*************************************************************************
    TEST(test_empty)
    {
      etl::string<100> s;

      const char* result = etl::format_to(s, "");
      CHECK_EQUAL(s.cbegin(), result);
      CHECK_EQUAL("", s);
    }

    //*************************************************************************
    TEST(test_format)
    {
      etl::string<100> s;

      const char* result = etl::format_to(s, "abc");
      CHECK_EQUAL(s.cbegin() + 3, result);
      CHECK_EQUAL("abc", s);
    }

    //*************************************************************************
    TEST(test_format_int)
    {
      etl::string<100> s;

      CHECK_EQUAL("1", test_format(s, "{}", 1));
      CHECK_EQUAL("123", test_format(s, "{}", 123));
      CHECK_EQUAL("4123", test_format(s, "{}", 4123));
      CHECK_EQUAL("1048962123", test_format(s, "{}", 1048962123));
      CHECK_EQUAL("1 2", test_format(s, "{} {}", 1, 2));
      CHECK_EQUAL("-123", test_format(s, "{}", -123));
      CHECK_EQUAL("-314748364", test_format(s, "{}", (int)-314748364));
      CHECK_EQUAL("2147483647", test_format(s, "{}", INT32_MAX));
      CHECK_EQUAL("-2147483648", test_format(s, "{}", INT32_MIN));
      CHECK_EQUAL("0", test_format(s, "{}", 0));
      CHECK_EQUAL("-1", test_format(s, "{}", -1));
    }

    //*************************************************************************
    TEST(test_format_unsigned_int)
    {
      etl::string<100> s;

      CHECK_EQUAL("0", test_format(s, "{}", static_cast<unsigned int>(0U)));
      CHECK_EQUAL("1", test_format(s, "{}", static_cast<unsigned int>(1U)));
      CHECK_EQUAL("12345678", test_format(s, "{}", static_cast<unsigned int>(12345678U)));
      CHECK_EQUAL("4123456780", test_format(s, "{}", static_cast<unsigned int>(4123456780U)));
    }

    //*************************************************************************
    TEST(test_format_long_long_int)
    {
      etl::string<100> s;

      CHECK_EQUAL("-1", test_format(s, "{}", static_cast<long long int>(-1LL)));
      CHECK_EQUAL("0", test_format(s, "{}", static_cast<long long int>(0LL)));
      CHECK_EQUAL("1", test_format(s, "{}", static_cast<long long int>(1LL)));
      CHECK_EQUAL("-12345678", test_format(s, "{}", static_cast<long long int>(-12345678LL)));
      CHECK_EQUAL("-4123456780", test_format(s, "{}", static_cast<long long int>(-4123456780LL)));
      CHECK_EQUAL("-123456781234", test_format(s, "{}", static_cast<long long int>(-123456781234LL)));
      CHECK_EQUAL("-412345678012", test_format(s, "{}", static_cast<long long int>(-412345678012LL)));
      CHECK_EQUAL("12345678", test_format(s, "{}", static_cast<long long int>(12345678LL)));
      CHECK_EQUAL("4123456780", test_format(s, "{}", static_cast<long long int>(4123456780LL)));
    }

    //*************************************************************************
    TEST(test_format_unsigned_long_long_int)
    {
      etl::string<100> s;

      CHECK_EQUAL("0", test_format(s, "{}", static_cast<unsigned long long int>(0LL)));
      CHECK_EQUAL("1", test_format(s, "{}", static_cast<unsigned long long int>(1LL)));
      CHECK_EQUAL("12345678", test_format(s, "{}", static_cast<unsigned long long int>(12345678LL)));
      CHECK_EQUAL("4123456780", test_format(s, "{}", static_cast<unsigned long long int>(4123456780LL)));
      CHECK_EQUAL("18446744073709551615", test_format(s, "{}", static_cast<unsigned long long int>(18446744073709551615ULL)));
      CHECK_EQUAL("1311768467463790320", test_format(s, "{}", static_cast<unsigned long long int>(0x123456789ABCDEF0ULL)));
    }

    //*************************************************************************
    TEST(test_format_other_int)
    {
      etl::string<100> s;

      CHECK_EQUAL("34", test_format(s, "{}", static_cast<uint8_t>(34)));
      CHECK_EQUAL("-14", test_format(s, "{}", static_cast<int8_t>(-14)));
      CHECK_EQUAL("6534", test_format(s, "{}", static_cast<uint16_t>(6534)));
      CHECK_EQUAL("-9414", test_format(s, "{}", static_cast<int16_t>(-9414)));
      CHECK_EQUAL("236534", test_format(s, "{}", static_cast<uint32_t>(236534)));
      CHECK_EQUAL("-6759414", test_format(s, "{}", static_cast<int32_t>(-6759414)));
    }

    //*************************************************************************
    TEST(test_format_float)
    {
      etl::string<100> s;

      CHECK_EQUAL("1.0", test_format(s, "{}", 1.0f));
      CHECK_EQUAL("1.234567", test_format(s, "{}", 1.234567f));
      CHECK_EQUAL("1.234567", test_format(s, "{}", 1.2345678f));
      CHECK_EQUAL("1.125", test_format(s, "{}", 1.125f));
    }

    //*************************************************************************
    TEST(test_format_double)
    {
      etl::string<100> s;

      CHECK_EQUAL("1.0", test_format(s, "{}", 1.0));
      CHECK_EQUAL("1.234564", test_format(s, "{}", 1.234564));
      CHECK_EQUAL("1.234567", test_format(s, "{}", 1.2345678));
      CHECK_EQUAL("1.5", test_format(s, "{}", 1.5));
    }

    //*************************************************************************
    TEST(test_format_long_double)
    {
      etl::string<100> s;

      CHECK_EQUAL("1.0", test_format(s, "{}", 1.0l));
      auto& result = test_format(s, "{}", 1.234567l);
      CHECK("1.234567" == result || "1.234566" == result);
      CHECK_EQUAL("1.234567", test_format(s, "{}", 1.2345678l));
      CHECK_EQUAL("1.25", test_format(s, "{}", 1.25l));
    }

    //*************************************************************************
    TEST(test_format_float_presentation)
    {
      etl::string<100> s;

      CHECK_EQUAL("1.000000e+00", test_format(s, "{:e}", 1.0f));
      CHECK_EQUAL("1.125000E+00", test_format(s, "{:E}", 1.125f));
      CHECK_EQUAL("-2.533324e-05", test_format(s, "{:e}", -0.00002533324f));
      CHECK_EQUAL("-2.500000e+11", test_format(s, "{:e}", -250000000000.0f));
      CHECK_EQUAL("1.000000", test_format(s, "{:f}", 1.0f));
      CHECK_EQUAL("1.125000", test_format(s, "{:F}", 1.125f));
      CHECK_EQUAL("1.000000", test_format(s, "{:g}", 1.0f));
      CHECK_EQUAL("1.125000", test_format(s, "{:G}", 1.125f));
      CHECK_EQUAL("1.000000e+10", test_format(s, "{:g}", 10000000000.0f));
      CHECK_EQUAL("1.000000E+10", test_format(s, "{:G}", 10000000000.0f));
      CHECK_EQUAL("nan", test_format(s, "{}", NAN));
      CHECK_EQUAL("nan", test_format(s, "{:e}", NAN));
      CHECK_EQUAL("NAN", test_format(s, "{:E}", NAN));
      CHECK_EQUAL("nan", test_format(s, "{:f}", NAN));
      CHECK_EQUAL("NAN", test_format(s, "{:F}", NAN));
      CHECK_EQUAL("nan", test_format(s, "{:g}", NAN));
      CHECK_EQUAL("NAN", test_format(s, "{:0.3G}", NAN));
      CHECK_EQUAL("inf", test_format(s, "{}", INFINITY));
      CHECK_EQUAL("inf", test_format(s, "{:e}", INFINITY));
      CHECK_EQUAL("INF", test_format(s, "{:E}", INFINITY));
      CHECK_EQUAL("inf", test_format(s, "{:f}", INFINITY));
      CHECK_EQUAL("INF", test_format(s, "{:F}", INFINITY));
      CHECK_EQUAL("inf", test_format(s, "{:g}", INFINITY));
      CHECK_EQUAL("INF", test_format(s, "{:0.3G}", INFINITY));
      CHECK_EQUAL("0x1.8p+0", test_format(s, "{:a}", 1.5f));
      CHECK_EQUAL("0X1.4CCCCCCCCCP+0", test_format(s, "{:A}", 1.3l));
      CHECK_EQUAL("0x2.49fp+4", test_format(s, "{:a}", 150000.0));
      CHECK_EQUAL("0x1.92a738p-5", test_format(s, "{:a}", 0.0000015f));
      CHECK_EQUAL("0x1.6345785d8ap+e", test_format(s, "{:a}", 100000000000000000.l));
    }

    //*************************************************************************
    TEST(test_format_char_array)
    {
      etl::string<100> s;

      CHECK_EQUAL("s", test_format(s, "{}", "s"));
      CHECK_EQUAL("abcd", test_format(s, "{}", "abcd"));
    }

    //*************************************************************************
    TEST(test_format_char)
    {
      etl::string<100> s;

      CHECK_EQUAL("a s b", test_format(s, "a {} b", 's'));
      CHECK_EQUAL("a s b", test_format(s, "a {:c} b", 's'));
      CHECK_EQUAL("a 's' b", test_format(s, "a {:?} b", 's'));
      CHECK_EQUAL("a \t b", test_format(s, "a {} b", '\t'));
      CHECK_EQUAL("a '\\t' b", test_format(s, "a {:?} b", '\t'));
      CHECK_EQUAL("a '\\n' b", test_format(s, "a {:?} b", '\n'));
      CHECK_EQUAL("a '\\r' b", test_format(s, "a {:?} b", '\r'));
      CHECK_EQUAL("a '\\\"' b", test_format(s, "a {:?} b", '"'));
      CHECK_EQUAL("a '\\'' b", test_format(s, "a {:?} b", '\''));
      CHECK_EQUAL("a '\\\\' b", test_format(s, "a {:?} b", '\\'));
      CHECK_EQUAL("a '\\\\' b", test_format(s, "a {:?} b", '\\'));
      CHECK_EQUAL("a 97 b", test_format(s, "a {:d} b", 'a'));
      CHECK_EQUAL("a 61 b", test_format(s, "a {:X} b", 'a'));
      CHECK_EQUAL("a 61 b", test_format(s, "a {:x} b", 'a'));
      CHECK_EQUAL("a 0x61 b", test_format(s, "a {:#x} b", 'a'));
    }

    //*************************************************************************
    TEST(test_format_bool)
    {
      etl::string<100> s;

      CHECK_EQUAL("1false2true3", test_format(s, "1{}2{}3", false, true));
      CHECK_EQUAL("true", test_format(s, "{:s}", true));
      CHECK_EQUAL("1", test_format(s, "{:d}", true));
      CHECK_EQUAL("1", test_format(s, "{:X}", true));
      CHECK_EQUAL("01", test_format(s, "{:#o}", true));
      CHECK_EQUAL("      true", test_format(s, "{:10}", true));
    }

    //*************************************************************************
    TEST(test_format_string_view)
    {
      etl::string<100> s;
      etl::string_view sv("data1");

      CHECK_EQUAL("data1", test_format(s, "{}", sv));
      CHECK_EQUAL("data1", test_format(s, "{:s}", sv));
      CHECK_THROW(test_format(s, "{:d}", sv), etl::bad_format_string_exception);
      CHECK_EQUAL("data1     ", test_format(s, "{:10s}", sv));
      CHECK_EQUAL("data1     ", test_format(s, "{:<10s}", sv));
      CHECK_EQUAL("     data1", test_format(s, "{:>10s}", sv));
      CHECK_EQUAL("  data1   ", test_format(s, "{:^10s}", sv));
      CHECK_EQUAL("data1", test_format(s, "{:3}", sv));
      CHECK_EQUAL("dat", test_format(s, "{:.3s}", sv));
      CHECK_EQUAL("dat", test_format(s, "{:^.3s}", sv));
      CHECK_EQUAL(".  dat   !", test_format(s, ".{:^8.3s}!", sv));
      CHECK_EQUAL("^dat     $", test_format(s, "^{:8.3s}$", sv));
    }

    //*************************************************************************
    TEST(test_format_string_view_escaped)
    {
      etl::string<100> s;
      etl::string_view sv("data1\n");

      CHECK_EQUAL("\"data1\\n\"", test_format(s, "{:?}", sv));
    }

    //*************************************************************************
    TEST(test_format_chars)
    {
      etl::string<100> s;
      const char* chars = "data1";

      CHECK_EQUAL("data1", test_format(s, "{}", chars));
      CHECK_EQUAL("data1", test_format(s, "{:s}", chars));
      CHECK_THROW(test_format(s, "{:d}", chars), etl::bad_format_string_exception);
      CHECK_EQUAL("data1     ", test_format(s, "{:10s}", chars));
      CHECK_EQUAL("data1     ", test_format(s, "{:<10s}", chars));
      CHECK_EQUAL("     data1", test_format(s, "{:>10s}", chars));
      CHECK_EQUAL("  data1   ", test_format(s, "{:^10s}", chars));
      CHECK_EQUAL("data1", test_format(s, "{:3}", chars));
      CHECK_EQUAL("dat", test_format(s, "{:.3s}", chars));
      CHECK_EQUAL("dat", test_format(s, "{:^.3s}", chars));
      CHECK_EQUAL(".  dat   !", test_format(s, ".{:^8.3s}!", chars));
      CHECK_EQUAL("^dat     $", test_format(s, "^{:8.3s}$", chars));
    }

    //*************************************************************************
    TEST(test_format_chars_escaped)
    {
      etl::string<100> s;
      const char* chars = "data2\n";

      CHECK_EQUAL("\"data2\\n\"", test_format(s, "{:?}", chars));
    }

    //*************************************************************************
    TEST(test_format_pointer)
    {
      etl::string<100> s;

      void* p = nullptr;

      CHECK_EQUAL("0x0", test_format(s, "{}", p));
      CHECK_EQUAL("0x0", test_format(s, "{:p}", p));
      CHECK_EQUAL("0X0", test_format(s, "{:P}", p));

      if (sizeof(uintptr_t) == 8)
      {
        p = reinterpret_cast<void*>(0x123456789abcdef0ULL);
        CHECK_EQUAL("0x123456789abcdef0", test_format(s, "{:p}", p));
        CHECK_EQUAL("0X123456789ABCDEF0", test_format(s, "{:P}", p));
      }
      else if (sizeof(uintptr_t) == 4)
      {
        p = reinterpret_cast<void*>(0x1abcdef0ULL);
        CHECK_EQUAL("0x1abcdef0", test_format(s, "{:p}", p));
        CHECK_EQUAL("0X1ABCDEF0", test_format(s, "{:P}", p));
      }
    }

    //*************************************************************************
    TEST(test_format_limit)
    {
      etl::string<10> s;

      CHECK_EQUAL("abcdefghij", test_format(s, "abcdefghijkl"));
      CHECK_EQUAL("abcdefgh12", test_format(s, "abcdefgh{}", 123));
    }

    //*************************************************************************
    TEST(test_format_escape)
    {
      etl::string<100> s;

      CHECK_EQUAL("abc{def", test_format(s, "abc{{def"));
      CHECK_EQUAL("}abc", test_format(s, "}}abc"));
    }

    //*************************************************************************
    TEST(test_format_invalid)
    {
      etl::string<100> s;

      CHECK_THROW(test_format(s, "a{b}", 1), etl::bad_format_string_exception); // bad format index spec
      // goal: rejected at compile time on C++20, error on <= C++17

      CHECK_THROW(test_format(s, "a{b"), etl::bad_format_string_exception); // closing brace missing
      // goal: rejected at compile time on C++20, error on <= C++17

      CHECK_THROW(test_format(s, "a{b}"), etl::bad_format_string_exception); // arg missing
      // goal: rejected at compile time on C++20, error on <= C++17

      CHECK_THROW(test_format(s, "a}b"), etl::bad_format_string_exception); // bad format: only escaped }} allowed
      // goal: rejected at compile time on C++20, error on <= C++17

      CHECK_EQUAL("123", test_format(s, "{:}", 123)); // valid
      CHECK_THROW(test_format(s, "{::}", 123), etl::bad_format_string_exception); // bad format spec
      CHECK_THROW(test_format(s, "{1}", 123), etl::bad_format_string_exception); // bad index
    }

    //*************************************************************************
    TEST(test_format_indexed)
    {
      etl::string<100> s;

      CHECK_EQUAL("34 56", test_format(s, "{0} {1}", 34, 56));
      CHECK_EQUAL("56 34", test_format(s, "{1} {0}", 34, 56));
      CHECK_EQUAL("134 134", test_format(s, "{0} {0}", 134));
      CHECK_EQUAL("a b c d e f g h i j k l m n", test_format(s, "{0} {1} {2} {3} {4} {5} {6} {7} {8} {9} {10} {11} {12} {13}", 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n'));
    }

    //*************************************************************************
    TEST(test_formatted_size)
    {
      CHECK_EQUAL(0, etl::formatted_size(""));
      CHECK_EQUAL(0, etl::formatted_size("{}", ""));
      CHECK_EQUAL(5, etl::formatted_size("xyz{}", 12));
      CHECK_EQUAL(3, etl::formatted_size("{}", "abc"));
    }

    //*************************************************************************
    TEST(test_iterator)
    {
      etl::string<100> s;

      etl::istring::iterator result = etl::format_to(s.begin(), "{0} {1}", 34, 56);
      s.uninitialized_resize(result - s.begin());
      CHECK_EQUAL("34 56", s);

      s.clear();
      etl::format_to(iterator(s), "");
      CHECK_EQUAL("", s);

      s.clear();
      etl::format_to(iterator(s), "{0} {1}", 65, 34);
      CHECK_EQUAL("65 34", s);

      s = "abcdefghij";
      etl::format_to_n(s.begin(), 3, "xy{}", 123);
      CHECK_EQUAL("xy1defghij", s);
    }

    //*************************************************************************
    TEST(test_format_spec_fill_and_align)
    {
      etl::string<100> s;

      CHECK_EQUAL("a   34", test_format(s, "a{:5}", 34));
      CHECK_EQUAL("a34", test_format(s, "a{:1}", 34));
      CHECK_EQUAL("a34", test_format(s, "a{:2}", 34));
      CHECK_EQUAL("  34", test_format(s, "{:>4}", 34));
      CHECK_EQUAL(" 34 ", test_format(s, "{:^4}", 34));
      CHECK_EQUAL(" 34  ", test_format(s, "{:^5}", 34));
      CHECK_EQUAL(" -65 ", test_format(s, "{:^5}", -65));
      CHECK_EQUAL("34  ", test_format(s, "{:<4}", 34));
      CHECK_THROW(test_format(s, "a{:*5}", 34), etl::bad_format_string_exception);
      CHECK_EQUAL("a*34**", test_format(s, "a{:*^5}", 34));
      CHECK_EQUAL("a*34**", test_format(s, "a{:*^5}", static_cast<unsigned int>(34)));
      CHECK_EQUAL("a***-341234567890****", test_format(s, "a{:*^20}", static_cast<long long int>(-341234567890)));
      CHECK_EQUAL("a****341234567890****", test_format(s, "a{:*^20}", static_cast<unsigned long long int>(341234567890)));
      CHECK_EQUAL("         x          ", test_format(s, "{: ^20}", 'x'));
      CHECK_EQUAL("x                   ", test_format(s, "{:20}", 'x'));
      CHECK_EQUAL("                   x", test_format(s, "{:>20}", 'x'));
      CHECK_EQUAL("x                   ", test_format(s, "{:<20}", 'x'));
    }

    //*************************************************************************
    TEST(test_format_spec_sign)
    {
      etl::string<100> s;

      CHECK_EQUAL("34", test_format(s, "{:-}", 34));
      CHECK_EQUAL("-34", test_format(s, "{:-}", -34));
      CHECK_EQUAL("-34", test_format(s, "{:+}", -34));
      CHECK_EQUAL("+34", test_format(s, "{:+}", 34));
      CHECK_EQUAL("+0", test_format(s, "{:+}", 0));
      CHECK_EQUAL("0", test_format(s, "{:-}", 0));
      CHECK_EQUAL("210", test_format(s, "{:-}", 210));
      CHECK_EQUAL("-210", test_format(s, "{:-}", -210));
      CHECK_EQUAL(" 0", test_format(s, "{: }", 0));
      CHECK_EQUAL(" 546", test_format(s, "{: }", 546));
      CHECK_EQUAL("-546", test_format(s, "{: }", -546));
    }

    //*************************************************************************
    TEST(test_format_int_presentation)
    {
      etl::string<100> s;

      CHECK_EQUAL("134", test_format(s, "{:d}", 134));
      CHECK_EQUAL("3f4", test_format(s, "{:x}", 0x3f4));
      CHECK_EQUAL("123456789abcdef0", test_format(s, "{:x}", 0x123456789abcdef0ULL));
      CHECK_EQUAL("0x3f4", test_format(s, "{:#x}", 0x3f4));
      CHECK_EQUAL("3F4", test_format(s, "{:X}", 0x3f4));
      CHECK_EQUAL("0X3F4", test_format(s, "{:#X}", 0x3f4));
      CHECK_EQUAL("34", test_format(s, "{:o}", 034));
      CHECK_EQUAL("034", test_format(s, "{:#o}", 034));
#if ETL_USING_CPP14
      CHECK_EQUAL("1010", test_format(s, "{:b}", 0b1010));
      CHECK_EQUAL("0b1010", test_format(s, "{:#b}", 0b1010));
      CHECK_EQUAL("1010", test_format(s, "{:B}", 0b1010));
      CHECK_EQUAL("0B1010", test_format(s, "{:#B}", 0b1010));
      CHECK_EQUAL("-0B1010", test_format(s, "{:#B}", -0b1010));
#endif
      CHECK_EQUAL("C", test_format(s, "{:c}", 67));
      CHECK_EQUAL("00067", test_format(s, "{:05d}", 67));
      CHECK_EQUAL("+00067", test_format(s, "{:+05d}", 67));
      CHECK_EQUAL("+0X00EF1", test_format(s, "{:+#05X}", 0xEF1));
      CHECK_THROW(test_format(s, "{:+#05.5X}", 0xEF1), etl::bad_format_string_exception);
    }
  }
}

#endif