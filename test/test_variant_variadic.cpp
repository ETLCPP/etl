/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl
https://www.etlcpp.com

Copyright(c) 2021 John Wellbelove

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

#include "etl/private/variant_variadic.h"
#include "etl/visitor.h"
#include "etl/overload.h"

#if ETL_USING_CPP14

#include <array>
#include <vector>
#include <algorithm>
#include <string>
#include <type_traits>

#if ETL_USING_CPP17
  #include <variant>
#endif

namespace
{
  // Test variant_etl types.
  using test_variant_etl_3 = etl::variant<char, int, std::string>;

  struct D1
  {
    D1(const std::string& a_) noexcept
      : a(a_)
    {
      copied = false;
      moved = false;
    }

    D1(const D1& other) noexcept
      : a(other.a)
    {
      copied = true;
      moved = false;
    }

    D1(D1&& other) noexcept
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
      : moved_from(false)
      , moved_to(false)
      , copied_to(false)
    {
    }

    Copyable(const Copyable&) noexcept
    {
      moved_from = false;
      moved_to = false;
      copied_to = true;
    }

    Copyable& operator =(const Copyable&) noexcept
    {
      moved_from = false;
      moved_to = false;
      copied_to = true;

      return *this;
    }

    bool moved_from;
    bool moved_to;
    bool copied_to;
  };

  //*********************************************
  struct Moveable
  {
    Moveable()
      : moved_from(false)
      , moved_to(false)
      , copied_to(false)
    {
    }

    Moveable(Moveable&& other) noexcept
    {
      moved_from = false;
      moved_to = true;
      copied_to = false;
      other.moved_from = true;
      other.moved_to = false;
      other.copied_to = false;
    }

    Moveable& operator =(Moveable&& rhs) noexcept
    {
      moved_from = false;
      moved_to = true;
      copied_to = false;
      rhs.moved_from = true;
      rhs.moved_to = false;
      rhs.copied_to = false;

      return *this;
    }

    Moveable(const Moveable& other) = delete;
    Moveable& operator =(const Moveable& rhs) = delete;

    bool moved_from;
    bool moved_to;
    bool copied_to;
  };

  //*********************************************
  struct MoveableCopyable
  {
    MoveableCopyable()
      : moved_from(false)
      , moved_to(false)
      , copied_to(false)
    {
    }

    MoveableCopyable(MoveableCopyable&& other) noexcept
    {
      moved_from = false;
      moved_to = true;
      copied_to = false;
      other.moved_from = true;
      other.moved_to = false;
      other.copied_to = false;
    }

    MoveableCopyable& operator =(MoveableCopyable&& rhs) noexcept
    {
      moved_from = false;
      moved_to = true;
      copied_to = false;
      rhs.moved_from = true;
      rhs.moved_to = false;
      rhs.copied_to = false;

      return *this;
    }

    MoveableCopyable(const MoveableCopyable&)
    {
      moved_from = false;
      moved_to = false;
      copied_to = true;
    }

    MoveableCopyable& operator =(const MoveableCopyable&)
    {
      moved_to = false;
      moved_from = false;
      copied_to = true;

      return *this;
    }

    bool moved_from;
    bool moved_to;
    bool copied_to;
  };
}

// Definitions for when the STL and compiler built-ins are not available.
#if ETL_NOT_USING_STL && !defined(ETL_USE_TYPE_TRAITS_BUILTINS)

using etl::is_copy_constructible;
using etl::is_move_constructible;

//*************************
template <>
struct etl::is_copy_constructible<D1> : public etl::true_type
{
};

template <>
struct etl::is_move_constructible<D1> : public etl::true_type
{
};

//*************************
template <>
struct etl::is_copy_constructible<D2> : public etl::true_type
{
};

template <>
struct etl::is_move_constructible<D2> : public etl::true_type
{
};

//*************************
template <>
struct etl::is_copy_constructible<D3> : public etl::true_type
{
};

template <>
struct etl::is_move_constructible<D3> : public etl::true_type
{
};

//*************************
template <>
struct etl::is_copy_constructible<D4> : public etl::true_type
{
};

template <>
struct etl::is_move_constructible<D4> : public etl::true_type
{
};

//*************************
template <>
struct etl::is_copy_constructible<std::string> : public etl::true_type
{
};

template <>
struct etl::is_move_constructible<std::string> : public etl::true_type
{
};

//*************************
template <>
struct etl::is_copy_constructible<Copyable> : public etl::true_type
{
};

template <>
struct etl::is_move_constructible<Copyable> : public etl::false_type
{
};

//*************************
template <>
struct etl::is_copy_constructible<Moveable> : public etl::false_type
{
};

template <>
struct etl::is_move_constructible<Moveable> : public etl::true_type
{
};

//*************************
template <>
struct etl::is_copy_constructible<MoveableCopyable> : public etl::true_type
{
};

template <>
struct etl::is_move_constructible<MoveableCopyable> : public etl::true_type
{
};
#endif

namespace
{
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
      CHECK(!etl::holds_alternative<int>(variant_etl));
      CHECK(!etl::holds_alternative<std::string>(variant_etl));

      CHECK(etl::holds_alternative<0U>(variant_etl));
      CHECK(!etl::holds_alternative<1U>(variant_etl));
      CHECK(!etl::holds_alternative<2U>(variant_etl));

      CHECK(etl::holds_alternative(0U, variant_etl));
      CHECK(!etl::holds_alternative(1U, variant_etl));
      CHECK(!etl::holds_alternative(2U, variant_etl));
      CHECK(!etl::holds_alternative(99U, variant_etl));
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
    TEST(test_construct_multiple_parameters_by_type)
    {
#if ETL_USING_CPP17
      test_variant_emplace variant_etl1(etl::in_place_type<D1>, "1");
      CHECK(etl::holds_alternative<D1>(variant_etl1));
      CHECK_EQUAL(D1("1"), etl::get<D1>(variant_etl1));

      test_variant_emplace variant_etl2(etl::in_place_type<D2>, "1", "2");
      CHECK(etl::holds_alternative<D2>(variant_etl2));
      CHECK_EQUAL(D2("1", "2"), etl::get<D2>(variant_etl2));

      test_variant_emplace variant_etl3(etl::in_place_type<D3>, "1", "2", "3");
      CHECK(etl::holds_alternative<D3>(variant_etl3));
      CHECK_EQUAL(D3("1", "2", "3"), etl::get<D3>(variant_etl3));

      test_variant_emplace variant_etl4(etl::in_place_type<D4>, "1", "2", "3", "4");
      CHECK(etl::holds_alternative<D4>(variant_etl4));
      CHECK_EQUAL(D4("1", "2", "3", "4"), etl::get<D4>(variant_etl4));
#else
      test_variant_emplace variant_etl1(etl::in_place_type_t<D1>{}, "1");
      CHECK(etl::holds_alternative<D1>(variant_etl1));
      CHECK_EQUAL(D1("1"), etl::get<D1>(variant_etl1));

      test_variant_emplace variant_etl2(etl::in_place_type_t<D2>{}, "1", "2");
      CHECK(etl::holds_alternative<D2>(variant_etl2));
      CHECK_EQUAL(D2("1", "2"), etl::get<D2>(variant_etl2));

      test_variant_emplace variant_etl3(etl::in_place_type_t<D3>{}, "1", "2", "3");
      CHECK(etl::holds_alternative<D3>(variant_etl3));
      CHECK_EQUAL(D3("1", "2", "3"), etl::get<D3>(variant_etl3));

      test_variant_emplace variant_etl4(etl::in_place_type_t<D4>{}, "1", "2", "3", "4");
      CHECK(etl::holds_alternative<D4>(variant_etl4));
      CHECK_EQUAL(D4("1", "2", "3", "4"), etl::get<D4>(variant_etl4));
#endif
    }

    //*************************************************************************
    TEST(test_construct_multiple_parameters_by_index)
    {
#if ETL_USING_CPP17
      test_variant_emplace variant_etl1(etl::in_place_index<1>, "1");
      CHECK(etl::holds_alternative<D1>(variant_etl1));
      CHECK_EQUAL(D1("1"), etl::get<D1>(variant_etl1));

      test_variant_emplace variant_etl2(etl::in_place_index<2>, "1", "2");
      CHECK(etl::holds_alternative<D2>(variant_etl2));
      CHECK_EQUAL(D2("1", "2"), etl::get<D2>(variant_etl2));

      test_variant_emplace variant_etl3(etl::in_place_index<3>, "1", "2", "3");
      CHECK(etl::holds_alternative<D3>(variant_etl3));
      CHECK_EQUAL(D3("1", "2", "3"), etl::get<D3>(variant_etl3));

      test_variant_emplace variant_etl4(etl::in_place_index<4>, "1", "2", "3", "4");
      CHECK(etl::holds_alternative<D4>(variant_etl4));
      CHECK_EQUAL(D4("1", "2", "3", "4"), etl::get<D4>(variant_etl4));
#else
      test_variant_emplace variant_etl1(etl::in_place_index_t<1>{}, "1");
      CHECK(etl::holds_alternative<D1>(variant_etl1));
      CHECK_EQUAL(D1("1"), etl::get<D1>(variant_etl1));

      test_variant_emplace variant_etl2(etl::in_place_index_t<2>{}, "1", "2");
      CHECK(etl::holds_alternative<D2>(variant_etl2));
      CHECK_EQUAL(D2("1", "2"), etl::get<D2>(variant_etl2));

      test_variant_emplace variant_etl3(etl::in_place_index_t<3>{}, "1", "2", "3");
      CHECK(etl::holds_alternative<D3>(variant_etl3));
      CHECK_EQUAL(D3("1", "2", "3"), etl::get<D3>(variant_etl3));

      test_variant_emplace variant_etl4(etl::in_place_index_t<4>{}, "1", "2", "3", "4");
      CHECK(etl::holds_alternative<D4>(variant_etl4));
      CHECK_EQUAL(D4("1", "2", "3", "4"), etl::get<D4>(variant_etl4));
#endif
    }

#if ETL_HAS_INITIALIZER_LIST
    //*************************************************************************
    TEST(test_construct_with_initializer_list_by_type)
    {
      etl::variant<std::vector<int>, std::string> v(etl::in_place_type_t<std::vector<int>>{}, { 0, 1, 2, 3 });

      std::vector<int> expected = { 0, 1, 2, 3 };
      std::vector<int> result   = etl::get<std::vector<int>>(v);

      CHECK_EQUAL(expected.size(), result.size());
      CHECK_ARRAY_EQUAL(expected.data(), result.data(), expected.size());
    }

    //*************************************************************************
    TEST(test_construct_with_initializer_list_by_index)
    {
      etl::variant<std::vector<int>, std::string> v(etl::in_place_index_t<0U>{}, { 0, 1, 2, 3 });

      std::vector<int> expected = { 0, 1, 2, 3 };
      std::vector<int> result   = etl::get<std::vector<int>>(v);

      CHECK_EQUAL(expected.size(), result.size());
      CHECK_ARRAY_EQUAL(expected.data(), result.data(), expected.size());
    }
#endif

    //*************************************************************************
    TEST(test_emplace_value_by_type)
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
    TEST(test_emplace_value_by_index)
    {
      // Char.
      char c = 'a';
      test_variant_etl_3 variant_char_etl;

      c = variant_char_etl.emplace<0>(c);
      CHECK(c == 'a');
      CHECK(etl::holds_alternative<char>(variant_char_etl));
      CHECK_EQUAL(c, etl::get<char>(variant_char_etl));

      // Int.
      int i = 1;
      test_variant_etl_3 variant_int_etl;

      i = variant_int_etl.emplace<1>(i);
      CHECK(i == 1);
      CHECK(etl::holds_alternative<int>(variant_int_etl));
      CHECK_EQUAL(i, etl::get<int>(variant_int_etl));

      // String.
      std::string text("Some Text");
      test_variant_etl_3 variant_text_etl;

      text = variant_text_etl.emplace<2>(text);
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
      CHECK(etl::get<std::string>(variant_1_etl) != etl::get<std::string>(variant_2_etl));
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
#include "etl/private/diagnostic_self_assign_overloaded_push.h" 
      variant_etl = variant_etl;
#include "etl/private/diagnostic_pop.h" 

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
    TEST(test_emplace_multiple_parameters)
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
    TEST(test_variant_accept_visitor)
    {    
      struct Visitor : public etl::visitor<char&, int&, std::string&>
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
          ++c;
        }

        void visit(int& i)
        {
          result_i = i;
          ++i;
        }

        void visit(std::string& s)
        {
          result_s = s;
          s = "4";
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
      CHECK_EQUAL(2, etl::get<char>(variant_etl));
      
      variant_etl = int(2);
      variant_etl.accept(visitor);
      CHECK_EQUAL(2, visitor.result_i);
      CHECK_EQUAL(3, etl::get<int>(variant_etl));

      variant_etl = std::string("3");
      variant_etl.accept(visitor);
      CHECK_EQUAL("3", visitor.result_s);
      CHECK_EQUAL("4", etl::get<std::string>(variant_etl));
    }

    //*************************************************************************
    TEST(test_variant_accept_visitor_deprecated)
    {
      struct Visitor : public etl::visitor<char&, int&, std::string&>
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
          ++c;
        }

        void visit(int& i)
        {
          result_i = i;
          ++i;
        }

        void visit(std::string& s)
        {
          result_s = s;
          s = "4";
        }

        char result_c;
        int  result_i;
        std::string result_s;
      };

      Visitor visitor;

      test_variant_etl_3 variant_etl;

      variant_etl = char(1);
      variant_etl.accept_visitor(visitor);
      CHECK_EQUAL(1, visitor.result_c);
      CHECK_EQUAL(2, etl::get<char>(variant_etl));

      variant_etl = int(2);
      variant_etl.accept_visitor(visitor);
      CHECK_EQUAL(2, visitor.result_i);
      CHECK_EQUAL(3, etl::get<int>(variant_etl));

      variant_etl = std::string("3");
      variant_etl.accept_visitor(visitor);
      CHECK_EQUAL("3", visitor.result_s);
      CHECK_EQUAL("4", etl::get<std::string>(variant_etl));
    }

    //*************************************************************************
    TEST(test_const_variant_accept_visitor)
    {
      struct Visitor : public etl::visitor<const char&, const int&, const std::string&>
      {
        Visitor()
          : result_c(0)
          , result_i(0)
          , result_s("")
        {
        }

        void visit(const char& c)
        {
          result_c = c;
        }

        void visit(const int& i)
        {
          result_i = i;
        }

        void visit(const std::string& s)
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
      const test_variant_etl_3 const_variant_etl1(variant_etl);
      const_variant_etl1.accept(visitor);
      CHECK_EQUAL(1, visitor.result_c);

      variant_etl = int(2);
      const test_variant_etl_3 const_variant_etl2(variant_etl);
      const_variant_etl2.accept(visitor);
      CHECK_EQUAL(2, visitor.result_i);

      variant_etl = std::string("3");
      const test_variant_etl_3 const_variant_etl3(variant_etl);
      const_variant_etl3.accept(visitor);
      CHECK_EQUAL("3", visitor.result_s);
    }

    //*************************************************************************
    TEST(test_const_variant_accept_visitor_deprecated)
    {
      struct Visitor : public etl::visitor<char, int, const std::string&>
      {
        Visitor()
          : result_c(0)
          , result_i(0)
          , result_s("")
        {
        }

        void visit(char c)
        {
          result_c = c;
        }

        void visit(int i)
        {
          result_i = i;
        }

        void visit(const std::string& s)
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
      const test_variant_etl_3 const_variant_etl1(variant_etl);
      const_variant_etl1.accept(visitor);
      CHECK_EQUAL(1, visitor.result_c);

      variant_etl = int(2);
      const test_variant_etl_3 const_variant_etl2(variant_etl);
      //const_variant_etl2.accept_visitor(visitor);
      const_variant_etl2.accept(visitor);
      CHECK_EQUAL(2, visitor.result_i);

      variant_etl = std::string("3");
      const test_variant_etl_3 const_variant_etl3(variant_etl);
      //const_variant_etl3.accept_visitor(visitor);
      const_variant_etl3.accept(visitor);
      CHECK_EQUAL("3", visitor.result_s);
    }

    //*************************************************************************
    TEST(test_variant_accept_functor_with_functor_class)
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
          ++c;
        }

        void operator()(int& i)
        {
          result_i = i;
          ++i;
        }

        void operator()(std::string& s)
        {
          result_s = s;
          s = "4";
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
      CHECK_EQUAL(2, etl::get<char>(variant_etl));

      variant_etl = int(2);
      variant_etl.accept(visitor);
      CHECK_EQUAL(2, visitor.result_i);
      CHECK_EQUAL(3, etl::get<int>(variant_etl));

      variant_etl = std::string("3");
      variant_etl.accept(visitor);
      CHECK_EQUAL("3", visitor.result_s);
      CHECK_EQUAL("4", etl::get<std::string>(variant_etl));
    }

    //*************************************************************************
    TEST(test_variant_accept_functor_with_functor_class_deprecated)
    {
      struct Visitor
      {
        Visitor()
          : result_c(0)
          , result_i(0)
          , result_s("")
        {
        }

        void operator()(char c)
        {
          result_c = c;
        }

        void operator()(int i)
        {
          result_i = i;
        }

        void operator()(const std::string& s)
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
      variant_etl.accept_functor(visitor);
      CHECK_EQUAL(1, visitor.result_c);

      variant_etl = int(2);
      variant_etl.accept_functor(visitor);
      CHECK_EQUAL(2, visitor.result_i);

      variant_etl = std::string("3");
      //variant_etl.accept_functor(visitor);
      variant_etl.accept(visitor);
      CHECK_EQUAL("3", visitor.result_s);
    }

    //*************************************************************************
    TEST(test_const_variant_accept_functor_with_functor_class)
    {
      struct Visitor
      {
        Visitor()
          : result_c(0)
          , result_i(0)
          , result_s("")
        {
        }

        void operator()(char c)
        {
          result_c = c;
        }

        void operator()(int i)
        {
          result_i = i;
        }

        void operator()(const std::string& s)
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
      const test_variant_etl_3 const_variant_etl1(variant_etl);
      const_variant_etl1.accept(visitor);
      CHECK_EQUAL(1, int(visitor.result_c));

      variant_etl = int(2);
      const test_variant_etl_3 const_variant_etl2(variant_etl);
      const_variant_etl2.accept(visitor);
      CHECK_EQUAL(2, visitor.result_i);

      variant_etl = std::string("3");
      const test_variant_etl_3 const_variant_etl3(variant_etl);
      const_variant_etl3.accept(visitor);
      CHECK_EQUAL("3", visitor.result_s);
    }

    //*************************************************************************
    TEST(test_const_variant_accept_functor_with_functor_class_deprecated)
    {
      struct Visitor
      {
        Visitor()
          : result_c(0)
          , result_i(0)
          , result_s("")
        {
        }

        void operator()(char c)
        {
          result_c = c;
        }

        void operator()(int i)
        {
          result_i = i;
        }

        void operator()(const std::string& s)
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
      const test_variant_etl_3 const_variant_etl1(variant_etl);
      const_variant_etl1.accept_functor(visitor);
      CHECK_EQUAL(1, visitor.result_c);

      variant_etl = int(2);
      const test_variant_etl_3 const_variant_etl2(variant_etl);
      const_variant_etl2.accept(visitor);
      CHECK_EQUAL(2, visitor.result_i);

      variant_etl = std::string("3");
      const test_variant_etl_3 const_variant_etl3(variant_etl);
      const_variant_etl3.accept(visitor);
      CHECK_EQUAL("3", visitor.result_s);
    }

    //*************************************************************************
#if ETL_USING_CPP17
    TEST(test_variant_accept_functor_with_overload)
    {
      char result_c;
      int  result_i;
      std::string result_s;

      auto visitor = etl::make_overload([&result_c](char& c) { result_c = 1; ++c; },
                                        [&result_i](int& i) { result_i = 2; ++i; },
                                        [&result_s](std::string& s) { result_s = "3"; s = "4"; });

      test_variant_etl_3 variant_etl;

      variant_etl = char(1);
      variant_etl.accept(visitor);
      CHECK_EQUAL(1, result_c);
      CHECK_EQUAL(2, etl::get<char>(variant_etl));

      variant_etl = int(2);
      variant_etl.accept(visitor);
      CHECK_EQUAL(2, result_i);
      CHECK_EQUAL(3, etl::get<int>(variant_etl));

      variant_etl = std::string("3");
      variant_etl.accept(visitor);
      CHECK_EQUAL("3", result_s);
      CHECK_EQUAL("4", etl::get<std::string>(variant_etl));
    }

    //*************************************************************************
    TEST(test_variant_accept_functor_with_overload_deprecated)
    {
      char result_c;
      int  result_i;
      std::string result_s;

      auto visitor = etl::make_overload([&result_c](char) { result_c = 1; },
        [&result_i](int) { result_i = 2; },
        [&result_s](const std::string&) { result_s = "3"; });

      test_variant_etl_3 variant_etl;

      variant_etl = char(1);
      variant_etl.accept_functor(visitor);
      CHECK_EQUAL(1, result_c);

      variant_etl = int(2);
      variant_etl.accept(visitor);
      CHECK_EQUAL(2, result_i);

      variant_etl = std::string("3");
      variant_etl.accept(visitor);
      CHECK_EQUAL("3", result_s);
    }

    //*************************************************************************
    TEST(test_const_variant_accept_functor_with_overload)
    {
      char result_c;
      int  result_i;
      std::string result_s;

      auto visitor = etl::make_overload([&result_c](char) { result_c = 1; },
                                        [&result_i](int) { result_i = 2; },
                                        [&result_s](const std::string&) { result_s = "3"; });

      test_variant_etl_3 variant_etl;

      variant_etl = char(1);
      const test_variant_etl_3 const_variant_etl1(variant_etl);
      const_variant_etl1.accept(visitor);
      CHECK_EQUAL(1, result_c);

      variant_etl = int(2);
      const test_variant_etl_3 const_variant_etl2(variant_etl);
      const_variant_etl2.accept(visitor);
      CHECK_EQUAL(2, result_i);

      variant_etl = std::string("3");
      const test_variant_etl_3 const_variant_etl3(variant_etl);
      const_variant_etl3.accept(visitor);
      CHECK_EQUAL("3", result_s);
    }

    //*************************************************************************
    TEST(test_const_variant_accept_functor_with_overload_deprecated)
    {
      char result_c;
      int  result_i;
      std::string result_s;

      auto visitor = etl::make_overload([&result_c](char) { result_c = 1; },
                                        [&result_i](int) { result_i = 2; },
                                        [&result_s](const std::string&) { result_s = "3"; });

      test_variant_etl_3 variant_etl;

      variant_etl = char(1);
      const test_variant_etl_3 const_variant_etl1(variant_etl);
      const_variant_etl1.accept_functor(visitor);
      CHECK_EQUAL(1, result_c);

      variant_etl = int(2);
      const test_variant_etl_3 const_variant_etl2(variant_etl);
      const_variant_etl2.accept(visitor);
      CHECK_EQUAL(2, result_i);

      variant_etl = std::string("3");
      const test_variant_etl_3 const_variant_etl3(variant_etl);
      const_variant_etl3.accept(visitor);
      CHECK_EQUAL("3", result_s);
    }
#endif

    //*************************************************************************
    TEST(test_get_if_index)
    {
      test_variant_etl_3 variant_etl;

      variant_etl = char(1);
      CHECK(etl::get_if<0>(&variant_etl) != nullptr);
      CHECK(etl::get_if<1>(&variant_etl) == nullptr);
      CHECK(etl::get_if<2>(&variant_etl) == nullptr);

      variant_etl = int(2);
      CHECK(etl::get_if<0>(&variant_etl) == nullptr);
      CHECK(etl::get_if<1>(&variant_etl) != nullptr);
      CHECK(etl::get_if<2>(&variant_etl) == nullptr);

      variant_etl = std::string("3");
      CHECK(etl::get_if<0>(&variant_etl) == nullptr);
      CHECK(etl::get_if<1>(&variant_etl) == nullptr);
      CHECK(etl::get_if<2>(&variant_etl) != nullptr);
    }

    //*************************************************************************
    TEST(test_get_if_type)
    {
      test_variant_etl_3 variant_etl;

      variant_etl = char(1);
      CHECK(etl::get_if<char>(&variant_etl) != nullptr);
      CHECK(etl::get_if<int>(&variant_etl)  == nullptr);
      CHECK(etl::get_if<std::string>(&variant_etl) == nullptr);

      variant_etl = int(2);
      CHECK(etl::get_if<char>(&variant_etl) == nullptr);
      CHECK(etl::get_if<int>(&variant_etl)  != nullptr);
      CHECK(etl::get_if<std::string>(&variant_etl) == nullptr);

      variant_etl = std::string("3");
      CHECK(etl::get_if<char>(&variant_etl) == nullptr);
      CHECK(etl::get_if<int>(&variant_etl)  == nullptr);
      CHECK(etl::get_if<std::string>(&variant_etl) != nullptr);
    }

    //*************************************************************************
    TEST(test_variant_size)
    {
      test_variant_etl_3 variant_etl;

#if ETL_USING_CPP17
      CHECK_EQUAL(3U, etl::variant_size_v<test_variant_etl_3>);
#else
      CHECK_EQUAL(3U, etl::variant_size<test_variant_etl_3>::value);
#endif
    }

#if ETL_USING_CPP17
    //*************************************************************************
    TEST(test_compare_etl_and_stl_variant_with_moveable_type)
    {
      Moveable from_etl;
      Moveable to_etl;

      Moveable from_std;
      Moveable to_std;

      etl::variant<Moveable> variant_etl(etl::move(from_etl));
      std::variant<Moveable> variant_std(etl::move(from_std));

      variant_etl = etl::move(from_etl);
      variant_std = etl::move(from_std);

      CHECK_EQUAL(from_std.moved_from, from_etl.moved_from);
      CHECK_EQUAL(from_std.moved_to,   from_etl.moved_to);
      CHECK_EQUAL(from_std.copied_to,  from_etl.copied_to);

      to_etl = etl::move(etl::get<0>(variant_etl));
      to_std = etl::move(std::get<0>(variant_std));

      CHECK_EQUAL(to_std.moved_from, to_etl.moved_from);
      CHECK_EQUAL(to_std.moved_to,   to_etl.moved_to);
      CHECK_EQUAL(to_std.copied_to,  to_etl.copied_to);
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

      CHECK_EQUAL(from_std.moved_from, from_etl.moved_from);
      CHECK_EQUAL(from_std.moved_to,   from_etl.moved_to);
      CHECK_EQUAL(from_std.copied_to,  from_etl.copied_to);

      to_etl = etl::get<0>(variant_etl);
      to_std = std::get<0>(variant_std);

      CHECK_EQUAL(to_std.moved_from, to_etl.moved_from);
      CHECK_EQUAL(to_std.moved_to,   to_etl.moved_to);
      CHECK_EQUAL(to_std.copied_to,  to_etl.copied_to);
    }

    //*************************************************************************
    TEST(test_compare_etl_and_stl_variant_with_moveable_copyable_type)
    {
      MoveableCopyable from_etl;
      MoveableCopyable to_etl;

      MoveableCopyable from_std;
      MoveableCopyable to_std;

      etl::variant<MoveableCopyable> variant_etl;
      std::variant<MoveableCopyable> variant_std;

      variant_etl = from_etl;
      variant_std = from_std;

      CHECK_EQUAL(from_std.moved_from, from_etl.moved_from);
      CHECK_EQUAL(from_std.moved_to,   from_etl.moved_to);
      CHECK_EQUAL(from_std.copied_to,  from_etl.copied_to);

      variant_etl = etl::move(from_etl);
      variant_std = etl::move(from_std);

      CHECK_EQUAL(from_std.moved_from, from_etl.moved_from);
      CHECK_EQUAL(from_std.moved_to,   from_etl.moved_to);
      CHECK_EQUAL(from_std.copied_to,  from_etl.copied_to);

      to_etl = etl::get<0>(variant_etl);
      to_std = std::get<0>(variant_std);

      CHECK_EQUAL(to_std.moved_from, to_etl.moved_from);
      CHECK_EQUAL(to_std.moved_to,   to_etl.moved_to);
      CHECK_EQUAL(to_std.copied_to,  to_etl.copied_to);

      to_etl = etl::move(etl::get<0>(variant_etl));
      to_std = etl::move(std::get<0>(variant_std));

      CHECK_EQUAL(to_std.moved_from, to_etl.moved_from);
      CHECK_EQUAL(to_std.moved_to,   to_etl.moved_to);
      CHECK_EQUAL(to_std.copied_to,  to_etl.copied_to);
    }

    //*************************************************************************
    TEST(test_get_by_type)
    {
      MoveableCopyable value1;
      MoveableCopyable value2;
      MoveableCopyable value3;

      etl::variant<MoveableCopyable>        v_etl(value1);
      const etl::variant<MoveableCopyable>  cv_etl(value2);
      etl::variant<MoveableCopyable>&       rv_etl(v_etl);
      const etl::variant<MoveableCopyable>& crv_etl(value3);

      std::variant<MoveableCopyable>        v_std(value1);
      const std::variant<MoveableCopyable>  cv_std(value2);
      std::variant<MoveableCopyable>&       rv_std(v_std);
      const std::variant<MoveableCopyable>& crv_std(value3);

      // From variant reference
      MoveableCopyable value_vr_etl = etl::get<MoveableCopyable>(rv_etl);
      MoveableCopyable value_vr_std = std::get<MoveableCopyable>(rv_std);
      CHECK_EQUAL(value_vr_std.moved_from, value_vr_etl.moved_from);
      CHECK_EQUAL(value_vr_std.moved_to,   value_vr_etl.moved_to);
      CHECK_EQUAL(value_vr_std.copied_to,  value_vr_etl.copied_to);

      // From variant const reference
      const MoveableCopyable& value_vcr_etl = etl::get<MoveableCopyable>(crv_etl);
      const MoveableCopyable& value_vcr_std = std::get<MoveableCopyable>(crv_std);
      CHECK_EQUAL(value_vcr_std.moved_from, value_vcr_etl.moved_from);
      CHECK_EQUAL(value_vcr_std.moved_to,   value_vcr_etl.moved_to);
      CHECK_EQUAL(value_vcr_std.copied_to,  value_vcr_etl.copied_to);

      // From variant rvalue reference
      MoveableCopyable&& value_vrr_etl = etl::get<MoveableCopyable>(etl::move(v_etl));
      MoveableCopyable&& value_vrr_std = std::get<MoveableCopyable>(etl::move(v_std));
      CHECK_EQUAL(value_vrr_std.moved_from, value_vrr_etl.moved_from);
      CHECK_EQUAL(value_vrr_std.moved_to,   value_vrr_etl.moved_to);
      CHECK_EQUAL(value_vrr_std.copied_to,  value_vrr_etl.copied_to);
         
      // From variant const rvalue reference
      const MoveableCopyable&& value_vcrr_etl = etl::get<MoveableCopyable>(etl::move(cv_etl));
      const MoveableCopyable&& value_vcrr_std = std::get<MoveableCopyable>(etl::move(cv_std));
      CHECK_EQUAL(value_vcrr_std.moved_from, value_vcrr_etl.moved_from);
      CHECK_EQUAL(value_vcrr_std.moved_to,   value_vcrr_etl.moved_to);
      CHECK_EQUAL(value_vcrr_std.copied_to,  value_vcrr_etl.copied_to);
    }

    //*************************************************************************
    TEST(test_get_by_index)
    {
      MoveableCopyable value1;
      MoveableCopyable value2;
      MoveableCopyable value3;

      etl::variant<MoveableCopyable>        v_etl(value1);
      const etl::variant<MoveableCopyable>  cv_etl(value2);
      etl::variant<MoveableCopyable>&       rv_etl(v_etl);
      const etl::variant<MoveableCopyable>& crv_etl(value3);

      std::variant<MoveableCopyable>        v_std(value1);
      const std::variant<MoveableCopyable>  cv_std(value2);
      std::variant<MoveableCopyable>&       rv_std(v_std);
      const std::variant<MoveableCopyable>& crv_std(value3);

      // From variant reference
      MoveableCopyable value_vr_etl = etl::get<0U>(rv_etl);
      MoveableCopyable value_vr_std = std::get<0U>(rv_std);
      CHECK_EQUAL(value_vr_std.moved_from, value_vr_etl.moved_from);
      CHECK_EQUAL(value_vr_std.moved_to,   value_vr_etl.moved_to);
      CHECK_EQUAL(value_vr_std.copied_to,  value_vr_etl.copied_to);

      // From variant const reference
      const MoveableCopyable& value_vcr_etl = etl::get<0U>(crv_etl);
      const MoveableCopyable& value_vcr_std = std::get<0U>(crv_std);
      CHECK_EQUAL(value_vcr_std.moved_from, value_vcr_etl.moved_from);
      CHECK_EQUAL(value_vcr_std.moved_to,   value_vcr_etl.moved_to);
      CHECK_EQUAL(value_vcr_std.copied_to,  value_vcr_etl.copied_to);

      // From variant rvalue reference
      MoveableCopyable&& value_vrr_etl = etl::get<0U>(etl::move(v_etl));
      MoveableCopyable&& value_vrr_std = std::get<0U>(etl::move(v_std));
      CHECK_EQUAL(value_vrr_std.moved_from, value_vrr_etl.moved_from);
      CHECK_EQUAL(value_vrr_std.moved_to,   value_vrr_etl.moved_to);
      CHECK_EQUAL(value_vrr_std.copied_to,  value_vrr_etl.copied_to);

      // From variant const rvalue reference
      const MoveableCopyable&& value_vcrr_etl = etl::get<0U>(etl::move(cv_etl));
      const MoveableCopyable&& value_vcrr_std = std::get<0U>(etl::move(cv_std));
      CHECK_EQUAL(value_vcrr_std.moved_from, value_vcrr_etl.moved_from);
      CHECK_EQUAL(value_vcrr_std.moved_to,   value_vcrr_etl.moved_to);
      CHECK_EQUAL(value_vcrr_std.copied_to,  value_vcrr_etl.copied_to);
    }
#endif

    //*************************************************************************
    TEST(test_get_if_by_type)
    {
      int value;

      etl::variant<int, double> v(value);
      const etl::variant<int, double> cv(value);
      etl::variant<int, double>& rv(v);
      const etl::variant<int, double>& crv(v);

      int* pi;
      const int* pci;
      double* pd;
      const double* pcd;

      etl::variant<int, double>* pv = nullptr;

      // From nullptr
      pi = etl::get_if<int>(pv);
      CHECK(pi == nullptr);

      // From variant reference
      pi = etl::get_if<int>(&rv);
      CHECK(pi != nullptr);

      pd = etl::get_if<double>(&rv);
      CHECK(pd == nullptr);

      // From variant const reference
      pci = etl::get_if<int>(&crv);
      CHECK(pci != nullptr);

      pcd = etl::get_if<double>(&crv);
      CHECK(pcd == nullptr);
    }

    //*************************************************************************
    TEST(test_get_if_by_index)
    {
      int value;

      etl::variant<int, double> v(value);
      const etl::variant<int, double> cv(value);
      etl::variant<int, double>& rv(v);
      const etl::variant<int, double>& crv(v);

      int* pi;
      const int* pci;
      double* pd;
      const double* pcd;

      etl::variant<int, double>* pv = nullptr;

      // From nullptr
      pi = etl::get_if<0U>(pv);
      CHECK(pi == nullptr);

      // From variant reference
      pi = etl::get_if<0U>(&rv);
      CHECK(pi != nullptr);

      pd = etl::get_if<1U>(&rv);
      CHECK(pd == nullptr);

      // From variant const reference
      pci = etl::get_if<0U>(&crv);
      CHECK(pci != nullptr);

      pcd = etl::get_if<1U>(&crv);
      CHECK(pcd == nullptr);
    }

    //*************************************************************************
    struct variant_test_visit_dispatcher
    {
      // const overloads
      int8_t operator()(int8_t&) const
      {
        return 1;
      }

      int8_t operator()(int8_t const&) const
      {
        return 10;
      }

      int8_t operator()(uint8_t&) const
      {
        return 2;
      }

      int8_t operator()(uint8_t const&) const
      {
        return 20;
      }

      int8_t operator()(int16_t&) const
      {
        return 3;
      }

      int8_t operator()(int16_t const&) const
      {
        return 30;
      }

      // non-const overloads
      int8_t operator()(int8_t&)
      {
        return 5;
      }

      int8_t operator()(int8_t const&)
      {
        return 50;
      }

      int8_t operator()(uint8_t&)
      {
        return 6;
      }

      int8_t operator()(uint8_t const&)
      {
        return 60;
      }

      int8_t operator()(int16_t&)
      {
        return 7;
      }

      int8_t operator()(int16_t const&)
      {
        return 70;
      }

      template <typename T>
      int8_t operator()(T const&) const
      {
        return -1;
      }
    };

    TEST(test_variant_visit)
    {
      etl::variant<int8_t, uint8_t, int16_t> variant;
      variant = int8_t{};
      variant_test_visit_dispatcher visitor;
      auto const&                   visitor_const = visitor;
      int16_t                       type = etl::visit(visitor_const, variant);
      CHECK_EQUAL(1, type);

      auto const& variant_const = variant;
      type = etl::visit(visitor_const, variant_const);
      CHECK_EQUAL(10, type);

      type = etl::visit(visitor, variant_const);
      CHECK_EQUAL(50, type);

      variant = int16_t{};
      type = etl::visit(visitor_const, variant);
      CHECK_EQUAL(3, type);

      type = etl::visit(visitor_const, variant_const);
      CHECK_EQUAL(30, type);

      type = etl::visit(visitor, variant_const);
      CHECK_EQUAL(70, type);
    }

    //*************************************************************************
    struct test_variant_multiple_visit_helper
    {
      template <typename T1, typename T2>
      int16_t operator()(T1 v1, T2 v2) const
      {
        int16_t res{};

        if (std::is_same<T1, int8_t>::value)
          res = 1;
        else if (std::is_same<T1, uint8_t>::value)
          res = 2;
        if (std::is_same<T2, int8_t>::value)
          res += 10;
        else if (std::is_same<T2, uint16_t>::value)
          res += 20;
        else if (std::is_same<T2, uint8_t>::value)
          res += 30;
        return res - static_cast<int16_t>(v1) * static_cast<int16_t>(v2);
      }
    };

    TEST(test_variant_multiple_visit)
    {
      etl::variant<int8_t, uint8_t>           variant1;
      etl::variant<int8_t, uint16_t, uint8_t> variant2;
      variant1 = int8_t{3};
      variant2 = int8_t{1};
      
      auto res = etl::visit<int16_t>(test_variant_multiple_visit_helper{}, variant1, variant2);
      CHECK_EQUAL(11 - 3, res);
      
      variant2 = uint16_t{2};
      res = etl::visit<int16_t>(test_variant_multiple_visit_helper{}, variant1, variant2);
      CHECK_EQUAL(21 - 3 * 2, res);
      
      variant1 = uint8_t{};
      variant2 = uint8_t{};
      res = etl::visit<int16_t>(test_variant_multiple_visit_helper{}, variant1, variant2);
      CHECK_EQUAL(32, res);
    }

    //*************************************************************************
    TEST(test_variant_multiple_visit_auto_return)
    {
      etl::variant<int8_t, uint8_t>           variant1;
      etl::variant<int8_t, uint16_t, uint8_t> variant2;
      variant1 = int8_t{3};
      variant2 = int8_t{1};
      auto const f = [](auto v1, auto v2)
      {
        return v1 * v2;
      };

      auto res = etl::visit(f, variant1, variant2);
      CHECK_EQUAL(3, res);
      
      variant2 = uint16_t{2};
      res = etl::visit(f, variant1, variant2);
      CHECK_EQUAL(3 * 2, res);
    }
    
    //*************************************************************************
    TEST(test_variant_visit_void)
    {
      etl::variant<int8_t, uint8_t> variant1;

      bool       variant_was_signed{};
      auto const f = [&variant_was_signed](auto v)
      {
        variant_was_signed = etl::is_signed<etl::remove_reference_t<decltype(v)>>::value;
      };

      etl::visit(f, variant1);
      CHECK_EQUAL(true, variant_was_signed);

      variant1 = uint8_t{};
      etl::visit<void>(f, variant1);
      CHECK_EQUAL(false, variant_was_signed);
    }

#if ETL_USING_CPP17
    //*************************************************************************
    TEST(test_variant_visit_with_overload)
    {
      struct TypeA { };
      struct TypeB { };
      struct TypeC { };
      struct TypeD { };

      std::string result = "?";

      etl::variant<TypeA, TypeB, TypeC, TypeD> package;

      etl::visit(etl::overload
        {
          [&result](TypeA&) { result = "TypeA"; },
          [&result](TypeB&) { result = "TypeB"; },
          [&result](TypeC&) { result = "TypeC"; },
          [&result](TypeD&) { result = "TypeD"; }
        }, package);

      CHECK_EQUAL(std::string("TypeA"), result);

      package = TypeA{};

      etl::visit(etl::overload
        {
          [&result](TypeA&) { result = "TypeA"; },
          [&result](TypeB&) { result = "TypeB"; },
          [&result](TypeC&) { result = "TypeC"; },
          [&result](TypeD&) { result = "TypeD"; }
        }, package);

      CHECK_EQUAL(std::string("TypeA"), result);

      package = TypeB{};

      etl::visit(etl::overload
        {
          [&result](TypeA&) { result = "TypeA"; },
          [&result](TypeB&) { result = "TypeB"; },
          [&result](TypeC&) { result = "TypeC"; },
          [&result](TypeD&) { result = "TypeD"; }
        }, package);

      CHECK_EQUAL(std::string("TypeB"), result);

      package = TypeC{};

      etl::visit(etl::overload
        {
          [&result](TypeA&) { result = "TypeA"; },
          [&result](TypeB&) { result = "TypeB"; },
          [&result](TypeC&) { result = "TypeC"; },
          [&result](TypeD&) { result = "TypeD"; }
        }, package);

      CHECK_EQUAL(std::string("TypeC"), result);

      package = TypeD{};

      etl::visit(etl::overload
        {
          [&result](TypeA&) { result = "TypeA"; },
          [&result](TypeB&) { result = "TypeB"; },
          [&result](TypeC&) { result = "TypeC"; },
          [&result](TypeD&) { result = "TypeD"; }
        }, package);

      CHECK_EQUAL(std::string("TypeD"), result);
    }
#endif
  };
}

#endif
