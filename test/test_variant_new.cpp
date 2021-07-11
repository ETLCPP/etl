/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl
https://www.etlcpp.com

Copyright(c) 2021 jwellbelove

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
#include "etl/visitor.h"

#include <array>
#include <vector>
#include <algorithm>
#include <string>
#include <variant>
#include <type_traits>

namespace
{
  // Test variant_etl types.
  using test_variant_etl_3 = etl::variant<char, int, std::string>;
  using test_variant_std_3 = std::variant<char, int, std::string>;

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

  //*********************************************
  struct Copyable
  {
    Copyable()
      : copied_from(false)
      , copied_to(false)
    {
    }

    Copyable(const Copyable& other) noexcept
    {
      copied_to = true;
      copied_from = false;
    }

    Copyable& operator =(const Copyable& rhs) noexcept
    {
      copied_to = true;
      copied_from = false;

      return *this;
    }

    bool copied_from;
    bool copied_to;
  };

  //*********************************************
  struct Moveable
  {
    Moveable()
      : moved_from(false)
      , moved_to(false)
    {
    }

    Moveable(Moveable&& other) noexcept
    {
      moved_to = true;
      moved_from = false;
      other.moved_to = false;
      other.moved_from = true;
    }

    Moveable& operator =(Moveable&& rhs) noexcept
    {
      moved_to = true;
      moved_from = false;
      rhs.moved_to = false;
      rhs.moved_from = true;

      return *this;
    }

    Moveable(const Moveable& other) = delete;
    Moveable& operator =(const Moveable& rhs) = delete;

    bool moved_from;
    bool moved_to;
  };

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

      CHECK((uintptr_t(&etl::get<char>(a)) % uintptr_t(etl::alignment_of<char>::value)) == 0);
      CHECK((uintptr_t(&etl::get<short>(b)) % uintptr_t(etl::alignment_of<short>::value)) == 0);
      CHECK((uintptr_t(&etl::get<int>(c)) % uintptr_t(etl::alignment_of<int>::value)) == 0);
      CHECK((uintptr_t(&etl::get<double>(d)) % uintptr_t(etl::alignment_of<double>::value)) == 0);
    }

    //*************************************************************************
    TEST(test_constructor_default)
    {
      CHECK_NO_THROW(test_variant_etl_3 variant_etl);

      test_variant_etl_3 variant_etl;

      CHECK(etl::holds_alternative<char>(variant_etl));
    }

    //*************************************************************************
    TEST(test_constructor_value)
    {
      // Char.
      char c = 'a';
      test_variant_etl_3 variant_char_etl(c);
      CHECK(c == 'a');
      CHECK(etl::holds_alternative<char>(variant_char_etl));
      CHECK_EQUAL(c, etl::get<char>(variant_char_etl));

      // Int.
      int i = 1;
      test_variant_etl_3 variant_int_etl(i);
      CHECK(i == 1);
      CHECK(etl::holds_alternative<int>(variant_int_etl));
      CHECK_EQUAL(i, etl::get<int>(variant_int_etl));

      // String.
      std::string text("Some Text");
      test_variant_etl_3 variant_text_etl(text);
      CHECK(text == "Some Text");
      CHECK(etl::holds_alternative<std::string>(variant_text_etl));
      CHECK_EQUAL(text, etl::get<std::string>(variant_text_etl));
    }

    //*************************************************************************
    TEST(test_constructor_move_value)
    {
      // Char.
      char c = 'a';
      test_variant_etl_3 variant_char_etl(etl::move(c));
      CHECK(etl::holds_alternative<char>(variant_char_etl));
      CHECK_EQUAL(c, etl::get<char>(variant_char_etl));

      // Int.
      int i = 1;
      test_variant_etl_3 variant_int_etl(etl::move(i));
      CHECK(etl::holds_alternative<int>(variant_int_etl));
      CHECK_EQUAL(i, etl::get<int>(variant_int_etl));

      // String.
      std::string text("Some Text");
      test_variant_etl_3 variant_text_etl(etl::move(text));
      CHECK(etl::holds_alternative<std::string>(variant_text_etl));
      CHECK_EQUAL(std::string("Some Text"), etl::get<std::string>(variant_text_etl));
    }

    //*************************************************************************
    TEST(test_emplace_value)
    {
      // Char.
      char c = 'a';
      test_variant_etl_3 variant_char_etl;
      
      variant_char_etl.emplace<char>(c);
      CHECK(c == 'a');
      CHECK(etl::holds_alternative<char>(variant_char_etl));
      CHECK_EQUAL(c, etl::get<char>(variant_char_etl));

      // Int.
      int i = 1;
      test_variant_etl_3 variant_int_etl;

      variant_int_etl.emplace<int>(i);
      CHECK(i == 1);
      CHECK(etl::holds_alternative<int>(variant_int_etl));
      CHECK_EQUAL(i, etl::get<int>(variant_int_etl));

      // String.
      std::string text("Some Text");
      test_variant_etl_3 variant_text_etl;

      variant_text_etl.emplace<std::string>(text);
      CHECK(text == "Some Text");
      CHECK(etl::holds_alternative<std::string>(variant_text_etl));
      CHECK_EQUAL(text, etl::get<std::string>(variant_text_etl));
    }

    //*************************************************************************
    TEST(test_copy_constructor)
    {
      std::string text("Some Text");
      test_variant_etl_3 variant_1_etl(text);

      test_variant_etl_3 variant_2_etl(variant_1_etl);

      CHECK_EQUAL(variant_1_etl.index(), variant_2_etl.index());
      CHECK_EQUAL(etl::get<std::string>(variant_1_etl), etl::get<std::string>(variant_2_etl));
    }

    //*************************************************************************
    TEST(test_copy_constructor_from_empty)
    {
      std::string text("Some Text");
      test_variant_etl_3 variant_1_etl;

      test_variant_etl_3 variant_2_etl(variant_1_etl);

      CHECK_EQUAL(variant_1_etl.index(), variant_2_etl.index());
    }

    //*************************************************************************
    TEST(test_move_constructor)
    {
      std::string text("Some Text");
      test_variant_etl_3 variant_1_etl(text);

      test_variant_etl_3 variant_2_etl(etl::move(variant_1_etl));

      CHECK_EQUAL(variant_1_etl.index(), variant_2_etl.index());
      CHECK_EQUAL(etl::get<std::string>(variant_1_etl), etl::get<std::string>(variant_2_etl));
    }

    //*************************************************************************
    TEST(test_move_constructor_from_empty)
    {
      std::string text("Some Text");
      test_variant_etl_3 variant_1_etl;

      test_variant_etl_3 variant_2_etl(etl::move(variant_1_etl));

      CHECK_EQUAL(variant_1_etl.index(), variant_2_etl.index());
    }

    //*************************************************************************
    TEST(test_assign_from_value)
    {
      std::string text("Some Text");
      test_variant_etl_3 variant_etl;

      variant_etl = text;

      CHECK_EQUAL(text, etl::get<std::string>(variant_etl));
    }

    //*************************************************************************
    TEST(test_assign_from_variant)
    {
      std::string text("Some Text");
      test_variant_etl_3 variant_1_etl;
      test_variant_etl_3 variant_2_etl;

      variant_1_etl = text;
      variant_2_etl = variant_1_etl;

      CHECK_EQUAL(text, etl::get<std::string>(variant_2_etl));
    }

    //*************************************************************************
    TEST(test_assign_from_variant2)
    {
      std::string text("Some Text");
      int integer(99);
      test_variant_etl_3 variant_1_etl;
      test_variant_etl_3 variant_2_etl;

      variant_1_etl = text;
      variant_2_etl = integer;
      variant_2_etl = variant_1_etl;

      CHECK_EQUAL(text, etl::get<std::string>(variant_2_etl));
    }

    //*************************************************************************
    TEST(test_assignment_incorrect_type_exception)
    {
      std::string text("Some Text");
      test_variant_etl_3 variant_etl(text);

      int i;
      CHECK_THROW(etl::get<int>(variant_etl), etl::variant_incorrect_type_exception);
      (void)i;
    }

    //*************************************************************************
    TEST(test_self_assignment)
    {
      test_variant_etl_3 variant_etl;

      variant_etl = 1;
      variant_etl = variant_etl;

      CHECK_EQUAL(1, etl::get<int>(variant_etl));
    }

    //*************************************************************************
    TEST(test_member_swap_variants)
    {
      std::string text("Some Text");
      int integer(99);
      test_variant_etl_3 variant_1_etl(text);
      test_variant_etl_3 variant_2_etl(integer);

      variant_1_etl.swap(variant_2_etl);

      CHECK(etl::holds_alternative<int>(variant_1_etl));
      CHECK_EQUAL(integer, etl::get<int>(variant_1_etl));

      CHECK(etl::holds_alternative<std::string>(variant_2_etl));
      CHECK_EQUAL(text, etl::get<std::string>(variant_2_etl));
    }

    //*************************************************************************
    TEST(test_global_swap_variants)
    {
      std::string text("Some Text");
      int integer(99);
      test_variant_etl_3 variant_1_etl(text);
      test_variant_etl_3 variant_2_etl(integer);

      etl::swap(variant_1_etl, variant_2_etl);

      CHECK(etl::holds_alternative<int>(variant_1_etl));
      CHECK_EQUAL(integer, etl::get<int>(variant_1_etl));

      CHECK(etl::holds_alternative<std::string>(variant_2_etl));
      CHECK_EQUAL(text, etl::get<std::string>(variant_2_etl));
    }

    //*************************************************************************
    TEST(test_emplace)
    {
      test_variant_emplace variant_etl;

      variant_etl.emplace<D1>("1");
      CHECK(etl::holds_alternative<D1>(variant_etl));
      CHECK_EQUAL(D1("1"), etl::get<D1>(variant_etl));

      variant_etl.emplace<D2>("1", "2");
      CHECK(etl::holds_alternative<D2>(variant_etl));
      CHECK_EQUAL(D2("1", "2"), etl::get<D2>(variant_etl));

      variant_etl.emplace<D3>("1", "2", "3");
      CHECK(etl::holds_alternative<D3>(variant_etl));
      CHECK_EQUAL(D3("1", "2", "3"), etl::get<D3>(variant_etl));

      variant_etl.emplace<D4>("1", "2", "3", "4");
      CHECK(etl::holds_alternative<D4>(variant_etl));
      CHECK_EQUAL(D4("1", "2", "3", "4"), etl::get<D4>(variant_etl));
    }

    //*************************************************************************
    D1 getD1()
    {
      return D1("1");
    }

    TEST(test_move)
    {
      D1 da("1");
      
      test_variant_emplace variant_etl(etl::move(getD1()));

      D1 db = etl::move(etl::get<D1>(variant_etl));
    }

    //*************************************************************************
    TEST(test_variant_visitor)
    {    
      struct Visitor : public etl::visitor<char, int, std::string>
      {
        Visitor()
          : result_c(0)
          , result_i(0)
          , result_s("")
        {
        }

        void visit(char& c)
        {
          result_c = c;
        }

        void visit(int& i)
        {
          result_i = i;
        }

        void visit(std::string& s)
        {
          result_s = s;
        }

        char result_c;
        int  result_i;
        std::string result_s;
      };

      Visitor visitor;

      test_variant_etl_3 variant_etl;

      variant_etl = char(1);
      variant_etl.accept(visitor);
      CHECK_EQUAL(1, visitor.result_c);
      
      variant_etl = int(2);
      variant_etl.accept(visitor);
      CHECK_EQUAL(2, visitor.result_i);

      variant_etl = std::string("3");
      variant_etl.accept(visitor);
      CHECK_EQUAL("3", visitor.result_s);
    }

    //*************************************************************************
    TEST(test_variant_operator_visit)
    {
      struct Visitor
      {
        Visitor()
          : result_c(0)
          , result_i(0)
          , result_s("")
        {
        }

        void operator()(char& c)
        {
          result_c = c;
        }

        void operator()(int& i)
        {
          result_i = i;
        }

        void operator()(std::string& s)
        {
          result_s = s;
        }

        char result_c;
        int  result_i;
        std::string result_s;
      };

      Visitor visitor;

      test_variant_etl_3 variant_etl;      

      variant_etl = char(1);
      variant_etl(visitor);
      CHECK_EQUAL(1, visitor.result_c);
      
      variant_etl = int(2);
      variant_etl(visitor);
      CHECK_EQUAL(2, visitor.result_i);

      variant_etl = std::string("3");
      variant_etl(visitor);
      CHECK_EQUAL("3", visitor.result_s);
    }

    //*************************************************************************
    TEST(test_get_if_index)
    {
      test_variant_etl_3 variant_etl;

      variant_etl = char(1);
      CHECK(etl::get_if<0>(&variant_etl) != nullptr);
      CHECK(etl::get_if<0>(variant_etl)  != nullptr);
      CHECK(etl::get_if<1>(&variant_etl) == nullptr);
      CHECK(etl::get_if<1>(variant_etl)  == nullptr);
      CHECK(etl::get_if<2>(&variant_etl) == nullptr);
      CHECK(etl::get_if<2>(variant_etl)  == nullptr);

      variant_etl = int(2);
      CHECK(etl::get_if<0>(&variant_etl) == nullptr);
      CHECK(etl::get_if<0>(variant_etl)  == nullptr);
      CHECK(etl::get_if<1>(&variant_etl) != nullptr);
      CHECK(etl::get_if<1>(variant_etl)  != nullptr);
      CHECK(etl::get_if<2>(&variant_etl) == nullptr);
      CHECK(etl::get_if<2>(variant_etl)  == nullptr);

      variant_etl = std::string("3");
      CHECK(etl::get_if<0>(&variant_etl) == nullptr);
      CHECK(etl::get_if<0>(variant_etl)  == nullptr);
      CHECK(etl::get_if<1>(&variant_etl) == nullptr);
      CHECK(etl::get_if<1>(variant_etl)  == nullptr);
      CHECK(etl::get_if<2>(&variant_etl) != nullptr);
      CHECK(etl::get_if<2>(variant_etl)  != nullptr);
    }

    //*************************************************************************
    TEST(test_get_if_type)
    {
      test_variant_etl_3 variant_etl;

      variant_etl = char(1);
      CHECK(etl::get_if<char>(&variant_etl) != nullptr);
      CHECK(etl::get_if<char>(variant_etl)  != nullptr);
      CHECK(etl::get_if<int>(&variant_etl)  == nullptr);
      CHECK(etl::get_if<int>(variant_etl)   == nullptr);
      CHECK(etl::get_if<std::string>(&variant_etl) == nullptr);
      CHECK(etl::get_if<std::string>(variant_etl)  == nullptr);

      variant_etl = int(2);
      CHECK(etl::get_if<char>(&variant_etl) == nullptr);
      CHECK(etl::get_if<char>(variant_etl)  == nullptr);
      CHECK(etl::get_if<int>(&variant_etl) != nullptr);
      CHECK(etl::get_if<int>(variant_etl)  != nullptr);
      CHECK(etl::get_if<std::string>(&variant_etl) == nullptr);
      CHECK(etl::get_if<std::string>(variant_etl)  == nullptr);

      variant_etl = std::string("3");
      CHECK(etl::get_if<char>(&variant_etl) == nullptr);
      CHECK(etl::get_if<char>(variant_etl)  == nullptr);
      CHECK(etl::get_if<int>(&variant_etl)  == nullptr);
      CHECK(etl::get_if<int>(variant_etl)   == nullptr);
      CHECK(etl::get_if<std::string>(&variant_etl) != nullptr);
      CHECK(etl::get_if<std::string>(variant_etl)  != nullptr);
    }

    //*************************************************************************
    TEST(test_variant_size)
    {
      test_variant_etl_3 variant_etl;

      CHECK_EQUAL(3U, etl::variant_size_v<test_variant_etl_3>);
    }

    //*************************************************************************
    TEST(test_compare_etl_and_stl_variant_with_moveable_type)
    {
      Moveable from_etl;
      Moveable to_etl;

      Moveable from_std;
      Moveable to_std;

      etl::variant<Moveable> variant_etl;
      std::variant<Moveable> variant_std;

      variant_etl = etl::move(from_etl);
      variant_std = etl::move(from_std);

      CHECK_EQUAL(from_std.moved_from, from_etl.moved_from);
      CHECK_EQUAL(from_std.moved_to,   from_etl.moved_to);

      to_etl = etl::move(etl::get<0>(variant_etl));
      to_std = etl::move(std::get<0>(variant_std));

      CHECK_EQUAL(to_std.moved_from, to_etl.moved_from);
      CHECK_EQUAL(to_std.moved_to,   to_etl.moved_to);
    }

    //*************************************************************************
    TEST(test_compare_etl_and_stl_variant_with_copyable_type)
    {
      Copyable from_etl;
      Copyable to_etl;

      Copyable from_std;
      Copyable to_std;

      etl::variant<Copyable> variant_etl;
      std::variant<Copyable> variant_std;

      variant_etl = from_etl;
      variant_std = from_std;

      CHECK_EQUAL(from_std.copied_from, from_etl.copied_from);
      CHECK_EQUAL(from_std.copied_to, from_etl.copied_to);

      to_etl = etl::get<0>(variant_etl);
      to_std = std::get<0>(variant_std);

      CHECK_EQUAL(to_std.copied_from, to_etl.copied_from);
      CHECK_EQUAL(to_std.copied_to, to_etl.copied_to);
    }
  };
}
