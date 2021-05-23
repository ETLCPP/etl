/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl
https://www.etlcpp.com

Copyright(c) 2014 jwellbelove

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

#include "etl/private/variant_new.h"

#include <array>
#include <vector>
#include <algorithm>
#include <string>

namespace
{
  // Test variant types.
  typedef etl::variant<char, int, std::string> test_variant_3;

  struct D1
  {
    D1(const std::string& a_)
      : a(a_)
    {
      copied = false;
      moved = false;
    }

    D1(const D1& other)
      : a(other.a)
    {
      copied = true;
      moved = false;
    }

    D1(D1&& other)
      : a(std::move(other.a))
    {
      copied = false;
      moved = true;
    }

    std::string a;
    bool copied;
    bool moved;
  };

  struct D2
  {
    D2(const std::string& a_, const std::string& b_)
      : a(a_),
      b(b_)
    {
    }

    std::string a;
    std::string b;
  };

  struct D3
  {
    D3(const std::string& a_, const std::string& b_, const std::string& c_)
      : a(a_),
      b(b_),
      c(c_)
    {
    }

    std::string a;
    std::string b;
    std::string c;
  };

  struct D4
  {
    D4(const std::string& a_, const std::string& b_, const std::string& c_, const std::string& d_)
      : a(a_),
      b(b_),
      c(c_),
      d(d_)
    {
    }

    std::string a;
    std::string b;
    std::string c;
    std::string d;
  };

  bool operator == (const D1& lhs, const D1& rhs)
  {
    return (lhs.a == rhs.a);
  }

  bool operator == (const D2& lhs, const D2& rhs)
  {
    return (lhs.a == rhs.a) && (lhs.b == rhs.b);
  }

  bool operator == (const D3& lhs, const D3& rhs)
  {
    return (lhs.a == rhs.a) && (lhs.b == rhs.b) && (lhs.c == rhs.c);
  }

  bool operator == (const D4& lhs, const D4& rhs)
  {
    return (lhs.a == rhs.a) && (lhs.b == rhs.b) && (lhs.c == rhs.c) && (lhs.d == rhs.d);
  }

  std::ostream& operator <<(std::ostream& os, const D1& d1)
  {
    os << d1.a;

    return os;
  }

  std::ostream& operator <<(std::ostream& os, const D2& d2)
  {
    os << d2.a << " " << d2.b;

    return os;
  }

  std::ostream& operator <<(std::ostream& os, const D3& d3)
  {
    os << d3.a << " " << d3.b << " " << d3.c;

    return os;
  }

  std::ostream& operator <<(std::ostream& os, const D4& d4)
  {
    os << d4.a << " " << d4.b << " " << d4.c << " " << d4.d;

    return os;
  }

  typedef etl::variant<etl::monostate, D1, D2, D3, D4> test_variant_emplace;

  SUITE(test_variant)
  {
    TEST(test_alignment)
    {
      typedef etl::variant<char, unsigned char> test_variant_a;
      typedef etl::variant<char, short>         test_variant_b;
      typedef etl::variant<char, int>           test_variant_c;
      typedef etl::variant<char, double>        test_variant_d;

      static test_variant_a a(char('1'));
      static test_variant_b b(short(2));
      static test_variant_c c(3);
      static test_variant_d d(4.5);

      CHECK((uintptr_t(&a.get<char>()) % uintptr_t(etl::alignment_of<char>::value)) == 0);
      CHECK((uintptr_t(&b.get<short>()) % uintptr_t(etl::alignment_of<short>::value)) == 0);
      CHECK((uintptr_t(&c.get<int>()) % uintptr_t(etl::alignment_of<int>::value)) == 0);
      CHECK((uintptr_t(&d.get<double>()) % uintptr_t(etl::alignment_of<double>::value)) == 0);
    }

    //*************************************************************************
    TEST(test_constructor_default)
    {
      CHECK_NO_THROW(test_variant_3 variant);

      test_variant_3 variant;

      CHECK(etl::holds_alternative<char>(variant));
    }

    //*************************************************************************
    TEST(test_constructor_value)
    {
      // Char.
      char c = 'a';
      test_variant_3 variant_char(c);

      CHECK(etl::holds_alternative<char>(variant_char));
      CHECK_EQUAL(c, etl::get<char>(variant_char));

      // Int.
      int i = 1;
      test_variant_3 variant_int(i);

      CHECK(etl::holds_alternative<int>(variant_int));
      CHECK_EQUAL(i, etl::get<int>(variant_int));

      // String.
      std::string text("Some Text");
      test_variant_3 variant_text(text);

      CHECK(etl::holds_alternative<std::string>(variant_text));
      CHECK_EQUAL(text, etl::get<std::string>(variant_text));
    }

    //*************************************************************************
    TEST(test_constructor_move_value)
    {
      // Char.
      char c = 'a';
      test_variant_3 variant_char(etl::move(c));

      CHECK(etl::holds_alternative<char>(variant_char));
      CHECK_EQUAL(c, etl::get<char>(variant_char));

      // Int.
      int i = 1;
      test_variant_3 variant_int(etl::move(i));

      CHECK(etl::holds_alternative<int>(variant_int));
      CHECK_EQUAL(i, etl::get<int>(variant_int));

      // String.
      std::string text("Some Text");
      test_variant_3 variant_text(etl::move(text));

      CHECK(etl::holds_alternative<std::string>(variant_text));
      CHECK_EQUAL(text, etl::get<std::string>(variant_text));
    }

    //*************************************************************************
    TEST(test_copy_constructor)
    {
      std::string text("Some Text");
      test_variant_3 variant_1(text);

      test_variant_3 variant_2(variant_1);

      CHECK_EQUAL(variant_1.index(), variant_2.index());
      CHECK_EQUAL(variant_1.get<std::string>(), variant_2.get<std::string>());
    }

    //*************************************************************************
    TEST(test_move_constructor)
    {
      std::string text("Some Text");
      test_variant_3 variant_1(text);

      test_variant_3 variant_2(etl::move(variant_1));

      CHECK_EQUAL(variant_1.index(), variant_2.index());
      CHECK_EQUAL(variant_1.get<std::string>(), variant_2.get<std::string>());
    }

    //*************************************************************************
    TEST(test_assign_from_value)
    {
      std::string text("Some Text");
      test_variant_3 variant;

      variant = text;

      CHECK_EQUAL(text, variant.get<std::string>());
    }

    //*************************************************************************
    TEST(test_assign_from_variant)
    {
      std::string text("Some Text");
      test_variant_3 variant_1;
      test_variant_3 variant_2;

      variant_1 = text;
      variant_2 = variant_1;

      CHECK_EQUAL(text, variant_2.get<std::string>());
    }

    //*************************************************************************
    TEST(test_assign_from_variant2)
    {
      std::string text("Some Text");
      int integer(99);
      test_variant_3 variant_1;
      test_variant_3 variant_2;

      variant_1 = text;
      variant_2 = integer;
      variant_2 = variant_1;

      CHECK_EQUAL(text, variant_2.get<std::string>());
    }

    //*************************************************************************
    TEST(test_assignment_incorrect_type_exception)
    {
      std::string text("Some Text");
      test_variant_3 variant(text);

      int i;
      CHECK_THROW(etl::get<int>(variant), etl::variant_incorrect_type_exception);
      (void)i;
    }

    //*************************************************************************
    TEST(test_self_assignment)
    {
      test_variant_3 variant;

      variant = 1;
      variant = variant;

      CHECK_EQUAL(1, etl::get<int>(variant));
    }

    //*************************************************************************
    TEST(test_member_swap_variants)
    {
      std::string text("Some Text");
      int integer(99);
      test_variant_3 variant_1(text);
      test_variant_3 variant_2(integer);

      variant_1.swap(variant_2);

      CHECK(etl::holds_alternative<int>(variant_1));
      CHECK_EQUAL(integer, etl::get<int>(variant_1));

      CHECK(etl::holds_alternative<std::string>(variant_2));
      CHECK_EQUAL(text, etl::get<std::string>(variant_2));
    }

    //*************************************************************************
    TEST(test_global_swap_variants)
    {
      std::string text("Some Text");
      int integer(99);
      test_variant_3 variant_1(text);
      test_variant_3 variant_2(integer);

      etl::swap(variant_1, variant_2);

      CHECK(etl::holds_alternative<int>(variant_1));
      CHECK_EQUAL(integer, etl::get<int>(variant_1));

      CHECK(etl::holds_alternative<std::string>(variant_2));
      CHECK_EQUAL(text, etl::get<std::string>(variant_2));
    }

    //*************************************************************************
    TEST(test_emplace)
    {
      test_variant_emplace variant;

      variant.emplace<D1>("1");
      CHECK(etl::holds_alternative<D1>(variant));
      CHECK_EQUAL(D1("1"), etl::get<D1>(variant));

      variant.emplace<D2>("1", "2");
      CHECK(etl::holds_alternative<D2>(variant));
      CHECK_EQUAL(D2("1", "2"), etl::get<D2>(variant));

      variant.emplace<D3>("1", "2", "3");
      CHECK(etl::holds_alternative<D3>(variant));
      CHECK_EQUAL(D3("1", "2", "3"), etl::get<D3>(variant));

      variant.emplace<D4>("1", "2", "3", "4");
      CHECK(etl::holds_alternative<D4>(variant));
      CHECK_EQUAL(D4("1", "2", "3", "4"), etl::get<D4>(variant));
    }

    //*************************************************************************
    D1 getD1()
    {
      return D1("1");
    }

    TEST(test_move)
    {
      D1 da("1");
      
      test_variant_emplace variant(etl::move(getD1()));

      D1 db = etl::move(etl::get<D1>(variant));
    }
  };
}
