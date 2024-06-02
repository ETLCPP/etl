/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl
https://www.etlcpp.com

Copyright(c) 2014 John Wellbelove

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

#include "etl/private/variant_legacy.h"

#include <array>
#include <vector>
#include <algorithm>
#include <string>

#include "etl/private/diagnostic_useless_cast_push.h"

namespace
{
  // Test classes for polymorphic tests.
  struct base
  {
    virtual ~base()
    {
    }

    base()
      : value(0)
    {
    }

    virtual void set() = 0;
    int value;
  };

  struct not_base
  {
    not_base()
      : value(0)
    {
    }

    virtual ~not_base() {}
    virtual void set() = 0;
    int value;
  };

  struct derived_1 : public base
  {
    void set()
    {
      value = 1;
    }
  };

  struct derived_2 : public base
  {
    void set()
    {
      value = 2;
    }
  };

  // Test variant types.
  typedef etl::legacy::variant<char, int, std::string> test_variant_3a;
  typedef etl::legacy::variant<int, short, double> test_variant_3b;

  typedef etl::legacy::variant<int8_t> test_variant_1;
  typedef etl::legacy::variant<int8_t, uint8_t> test_variant_2;
  typedef etl::legacy::variant<int8_t, uint8_t, int16_t> test_variant_3;
  typedef etl::legacy::variant<int8_t, uint8_t, int16_t, uint16_t> test_variant_4;
  typedef etl::legacy::variant<int8_t, uint8_t, int16_t, uint16_t, int32_t> test_variant_5;
  typedef etl::legacy::variant<int8_t, uint8_t, int16_t, uint16_t, int32_t, uint32_t> test_variant_6;
  typedef etl::legacy::variant<int8_t, uint8_t, int16_t, uint16_t, int32_t, uint32_t, int64_t> test_variant_7;
  typedef etl::legacy::variant<int8_t, uint8_t, int16_t, uint16_t, int32_t, uint32_t, int64_t, uint64_t> test_variant_8;

  typedef etl::legacy::variant<derived_1, derived_2> test_variant_polymorphic;
  typedef etl::legacy::variant<char, unsigned char, short, unsigned short, int, unsigned int, long, unsigned long> test_variant_max_types;

  // This line should compile with no errors.
  test_variant_max_types variant_max;

  struct D1
  {
    D1(const std::string& a_)
      : a(a_)
    {
    }

    std::string a;
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

  typedef etl::legacy::variant<D1, D2, D3, D4> test_variant_emplace;

  SUITE(test_variant)
  {
    TEST(test_alignment)
    {
      typedef etl::legacy::variant<char, unsigned char> test_variant_a;
      typedef etl::legacy::variant<char, short>         test_variant_b;
      typedef etl::legacy::variant<char, int>           test_variant_c;
      typedef etl::legacy::variant<char, double>        test_variant_d;

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
      CHECK_NO_THROW(test_variant_3a variant);
    }

    //*************************************************************************
    TEST(test_constructor_value)
    {
      // Char.
      char c = 'a';
      test_variant_3a variant_char(c);

      CHECK(variant_char.is_type<char>());
      CHECK(variant_char.is_valid());
      CHECK_EQUAL(c, variant_char.get<char>());

      // Int.
      int i = 1;
      test_variant_3a variant_int(i);

      CHECK(variant_int.is_type<int>());
      CHECK(variant_int.is_valid());
      CHECK_EQUAL(i, variant_int.get<int>());

      // String.
      std::string text("Some Text");
      test_variant_3a variant_text(text);

      CHECK(variant_text.is_type<std::string>());
      CHECK(variant_text.is_valid());
      CHECK_EQUAL(text, variant_text.get<std::string>());
    }

    //*************************************************************************
    TEST(test_copy_constructor)
    {
      std::string text("Some Text");
      test_variant_3a variant_1(text);

      test_variant_3a variant_2(variant_1);

      CHECK(variant_1.is_same_type(variant_2));
      CHECK(variant_2.is_valid());
      CHECK_EQUAL(variant_1.get<std::string>(), variant_2.get<std::string>());
    }

    //*************************************************************************
    TEST(test_assign_from_value)
    {
      std::string text("Some Text");
      test_variant_3a variant;

      variant = text;

      CHECK_EQUAL(text, variant.get<std::string>());
      CHECK(variant.is_valid());
    }

    //*************************************************************************
    TEST(test_assign_from_variant)
    {
      std::string text("Some Text");
      test_variant_3a variant_1;
      test_variant_3a variant_2;

      variant_1 = text;
      variant_2 = variant_1;

      CHECK_EQUAL(text, variant_2.get<std::string>());
      CHECK(variant_2.is_valid());
    }

    //*************************************************************************
    TEST(test_assign_from_variant2)
    {
      std::string text("Some Text");
      int integer(99);
      test_variant_3a variant_1;
      test_variant_3a variant_2;

      variant_1 = text;
      variant_2 = integer;
      variant_2 = variant_1;

      CHECK_EQUAL(text, variant_2.get<std::string>());
      CHECK(variant_2.is_valid());
    }

    //*************************************************************************
    TEST(test_assignment_incorrect_type_exception)
    {
      std::string text("Some Text");
      test_variant_3a variant;
      variant = text;

      int i;
      CHECK_THROW(i = variant, etl::legacy::variant_incorrect_type_exception);
      (void)i;
    }

    //*************************************************************************
    TEST(test_is_same_type)
    {
      std::string text("Some Text");
      test_variant_3a variant_1;
      test_variant_3a variant_2;

      variant_1 = 1;
      variant_2 = 2;

      CHECK(variant_1.is_same_type(variant_2));

      variant_1 = 'a';
      variant_2 = text;

      CHECK(!variant_1.is_same_type(variant_2));
    }

    //*************************************************************************
    TEST(test_is_same_type_different_variants)
    {
      test_variant_3a variant_1;
      test_variant_3b variant_2;

      variant_1 = 1;
      variant_2 = 2;

      CHECK(variant_1.is_same_type(variant_2));

      variant_1 = 'a';
      variant_2 = 3.3;

      CHECK(!variant_1.is_same_type(variant_2));
    }

    //*************************************************************************
    TEST(test_is_supported_type)
    {
      CHECK(test_variant_3a::is_supported_type<char>());
      CHECK(test_variant_3a::is_supported_type<int>());
      CHECK(test_variant_3a::is_supported_type<std::string>());
      CHECK(!test_variant_3a::is_supported_type<short>());
    }

    //*************************************************************************
    TEST(test_is_valid)
    {
      test_variant_3a variant;

      CHECK(!variant.is_valid());

      variant = 1;
      CHECK(variant.is_valid());
    }

    //*************************************************************************
    TEST(test_is_type)
    {
      test_variant_3a variant;
      variant = 1;
      CHECK(variant.is_type<int>());

      variant = 'a';
      CHECK(variant.is_type<char>());

      variant = std::string("Some Text");
      CHECK(variant.is_type<std::string>());
    }

    //*************************************************************************
    TEST(test_get)
    {
      test_variant_3a variant;

      variant = 1;
      CHECK_EQUAL(1, variant.get<int>());

      variant = 'a';
      CHECK_EQUAL('a', variant.get<char>());

      variant = std::string("Some Text");
      CHECK_EQUAL(std::string("Some Text"), variant.get<std::string>());
    }

    //*************************************************************************
    TEST(test_get_const)
    {
      test_variant_3a variant;

      variant = 1;
      const test_variant_3a cvariant1(variant);
      CHECK_EQUAL(1, cvariant1.get<int>());

      variant = 'a';
      const test_variant_3a cvariant2(variant);
      CHECK_EQUAL('a', cvariant2.get<char>());

      variant = std::string("Some Text");
      const test_variant_3a cvariant3(variant);
      CHECK_EQUAL(std::string("Some Text"), cvariant3.get<std::string>());
    }

    //*************************************************************************
    TEST(test_assignment)
    {
      test_variant_3a variant1;
      test_variant_3a variant2;

      variant1 = 1;
      variant2 = variant1;

      CHECK_EQUAL(variant1.get<int>(), variant2.get<int>());
    }

    //*************************************************************************
    TEST(test_self_assignment)
    {
      test_variant_3a variant;

      variant = 1;
#include "etl/private/diagnostic_self_assign_overloaded_push.h" 
      variant = variant;
#include "etl/private/diagnostic_pop.h" 

      CHECK_EQUAL(1, variant.get<int>());
    }

    //*************************************************************************
    TEST(TestGetException)
    {
      test_variant_3a variant;
      variant = 1;

      char c;
      CHECK_THROW(c = variant.get<char>(), etl::legacy::variant_incorrect_type_exception);
      (void)c;
    }

    //*************************************************************************
    TEST(TestGetReference)
    {
      test_variant_3a variant;
      variant = 1;

      int& ir = variant.get<int>();
      CHECK_EQUAL(ir, variant.get<int>());

      ir = 2;
      CHECK_EQUAL(ir, variant.get<int>());
    }

    //*************************************************************************
    TEST(TestGetConstReference)
    {
      test_variant_3a variant;
      variant = 1;
      const int& ir = variant.get<int>();
      CHECK_EQUAL(1, ir);

      variant = 2;
      CHECK_EQUAL(2, ir);
    }

    //*************************************************************************
    TEST(TestCallReader)
    {
      class reader : public test_variant_3a::reader
      {
      public:

        reader() : c(' '), s(""), i(0)
        {
        }

        void read(char c_) override
        {
          c = c_;
        }

        void read(int i_) override
        {
          i = i_;
        }

        void read(const std::string& s_) override
        {
          s = s_;
        }

        char c;
        std::string s;
        int i;
      };

      test_variant_3a variant;
      reader reader;

      variant = 'a';
      variant.call(reader);
      CHECK_EQUAL(variant.get<char>(), reader.c);

      variant = std::string("Some Text");
      variant.call(reader);
      CHECK_EQUAL(variant.get<std::string>(), reader.s);

      variant = 1;
      variant.call(reader);
      CHECK_EQUAL(variant.get<int>(), reader.i);
    }

    //*************************************************************************
    TEST(test_conversion_operators)
    {
      int8_t   i8;
      uint8_t  ui8;
      int16_t  i16;
      uint16_t ui16;
      int32_t  i32;
      uint32_t ui32;
      int64_t  i64;
      uint64_t ui64;

      test_variant_1 variant1;
      test_variant_2 variant2;
      test_variant_3 variant3;
      test_variant_4 variant4;
      test_variant_5 variant5;
      test_variant_6 variant6;
      test_variant_7 variant7;
      test_variant_8 variant8;

      // Variant 1
      variant1 = int8_t(1);
      i8 = variant1;
      CHECK_EQUAL(i8, variant1.get<int8_t>());

      // Variant 2
      variant2 = int8_t(1);
      i8 = variant2;
      CHECK_EQUAL(i8, variant2.get<int8_t>());

      variant2 = uint8_t(2);
      ui8 = variant2;
      CHECK_EQUAL(ui8, variant2.get<uint8_t>());

      // Variant 3
      variant3 = int8_t(1);
      i8 = variant3;
      CHECK_EQUAL(i8, variant3.get<int8_t>());

      variant3 = uint8_t(2);
      ui8 = variant3;
      CHECK_EQUAL(ui8, variant3.get<uint8_t>());

      variant3 = int16_t(3);
      i16 = variant3;
      CHECK_EQUAL(i16, variant3.get<int16_t>());

      // Variant 4
      variant4 = int8_t(1);
      i8 = variant4;
      CHECK_EQUAL(i8, variant4.get<int8_t>());

      variant4 = uint8_t(2);
      ui8 = variant4;
      CHECK_EQUAL(ui8, variant4.get<uint8_t>());

      variant4 = int16_t(3);
      i16 = variant4;
      CHECK_EQUAL(i16, variant4.get<int16_t>());

      variant4 = uint16_t(4);
      ui16 = variant4;
      CHECK_EQUAL(ui16, variant4.get<uint16_t>());

      // Variant 5
      variant5 = int8_t(1);
      i8 = variant5;
      CHECK_EQUAL(i8, variant5.get<int8_t>());

      variant5 = uint8_t(2);
      ui8 = variant5;
      CHECK_EQUAL(ui8, variant5.get<uint8_t>());

      variant5 = int16_t(3);
      i16 = variant5;
      CHECK_EQUAL(i16, variant5.get<int16_t>());

      variant5 = uint16_t(4);
      ui16 = variant5;
      CHECK_EQUAL(ui16, variant5.get<uint16_t>());

      variant5 = int32_t(5);
      i32 = variant5;
      CHECK_EQUAL(i32, variant5.get<int32_t>());

      // Variant 6
      variant6 = int8_t(1);
      i8 = variant6;
      CHECK_EQUAL(i8, variant6.get<int8_t>());

      variant6 = uint8_t(2);
      ui8 = variant6;
      CHECK_EQUAL(ui8, variant6.get<uint8_t>());

      variant6 = int16_t(3);
      i16 = variant6;
      CHECK_EQUAL(i16, variant6.get<int16_t>());

      variant6 = uint16_t(4);
      ui16 = variant6;
      CHECK_EQUAL(ui16, variant6.get<uint16_t>());

      variant6 = int32_t(5);
      i32 = variant6;
      CHECK_EQUAL(i32, variant6.get<int32_t>());

      variant6 = uint32_t(6);
      ui32 = variant6;
      CHECK_EQUAL(ui32, variant6.get<uint32_t>());

      // Variant 7
      variant7 = int8_t(1);
      i8 = variant7;
      CHECK_EQUAL(i8, variant7.get<int8_t>());

      variant7 = uint8_t(2);
      ui8 = variant7;
      CHECK_EQUAL(ui8, variant7.get<uint8_t>());

      variant7 = int16_t(3);
      i16 = variant7;
      CHECK_EQUAL(i16, variant7.get<int16_t>());

      variant7 = uint16_t(4);
      ui16 = variant7;
      CHECK_EQUAL(ui16, variant7.get<uint16_t>());

      variant7 = int32_t(5);
      i32 = variant7;
      CHECK_EQUAL(i32, variant7.get<int32_t>());

      variant7 = uint32_t(6);
      ui32 = variant7;
      CHECK_EQUAL(ui32, variant7.get<uint32_t>());

      variant7 = int64_t(7);
      i64 = variant7;
      CHECK_EQUAL(i64, variant7.get<int64_t>());

      // Variant 8
      variant8 = int8_t(1);
      i8 = variant8;
      CHECK_EQUAL(i8, variant8.get<int8_t>());

      variant8 = uint8_t(2);
      ui8 = variant8;
      CHECK_EQUAL(ui8, variant8.get<uint8_t>());

      variant8 = int16_t(3);
      i16 = variant8;
      CHECK_EQUAL(i16, variant8.get<int16_t>());

      variant8 = uint16_t(4);
      ui16 = variant8;
      CHECK_EQUAL(ui16, variant8.get<uint16_t>());

#include "etl/private/diagnostic_useless_cast_push.h"
      variant8 = int32_t(5);
      i32 = variant8;
      CHECK_EQUAL(i32, variant8.get<int32_t>());
#include "etl/private/diagnostic_pop.h"

      variant8 = uint32_t(6);
      ui32 = variant8;
      CHECK_EQUAL(ui32, variant8.get<uint32_t>());

      variant8 = int64_t(7);
      i64 = variant8;
      CHECK_EQUAL(i64, variant8.get<int64_t>());

      variant8 = uint64_t(8);
      ui64 = variant8;
      CHECK_EQUAL(ui64, variant8.get<uint64_t>());
    }

    //*************************************************************************
    TEST(test_conversion_operators_const_reference)
    {
      test_variant_3a variant;
      variant = 1;
      const int& i = variant;
      CHECK_EQUAL(i, variant.get<int>());
      variant = 2;
      CHECK_EQUAL(i, variant.get<int>());

      variant = std::string("Some Text");
      const std::string& text = variant;
      CHECK_EQUAL(text, variant.get<std::string>());

      variant = std::string("Text2");
      CHECK_EQUAL(text, variant.get<std::string>());

      variant = 'a';
      const char& c = variant;
      CHECK_EQUAL(c, variant.get<char>());

      variant = 'B';
      CHECK_EQUAL(c, variant.get<char>());
    }

    //*************************************************************************
    TEST(test_swap)
    {
      test_variant_3a variant_1;
      test_variant_3a variant_2;

      variant_1 = 1;
      variant_2 = std::string("Some Text");

      std::swap(variant_1, variant_2);

      CHECK_EQUAL(1, variant_2.get<int>());
      CHECK_EQUAL(std::string("Some Text"), variant_1.get<std::string>());
    }

    //*************************************************************************
    TEST(test_clear)
    {
      test_variant_3a variant;

      variant = 1;
      variant.clear();

      CHECK(!variant.is_valid());
    }

    //*************************************************************************
    TEST(test_polymorphic)
    {
      derived_1 derived1;
      derived1.set();

      test_variant_polymorphic variant1;
      CHECK(!variant1.is_base_of<base>());
      CHECK(!variant1.is_base_of<not_base>());

      variant1 = derived1;
      CHECK(variant1.is_base_of<base>());
      CHECK(!variant1.is_base_of<not_base>());
      CHECK_EQUAL(1, variant1.upcast<base>().value);
      CHECK_THROW(variant1.upcast<not_base>(), etl::legacy::variant_not_a_base_exception);
      CHECK_EQUAL((void*)(0U), variant1.upcast_ptr<not_base>());

      derived_2 derived2;
      derived2.set();

      variant1 = derived2;
      CHECK(variant1.is_base_of<base>());
      CHECK(!variant1.is_base_of<not_base>());
      CHECK_EQUAL(2, variant1.upcast<base>().value);
      CHECK_THROW(variant1.upcast<not_base>(), etl::legacy::variant_not_a_base_exception);
      CHECK_EQUAL((void*)(0U), variant1.upcast_ptr<not_base>());
    }

    //*************************************************************************
    TEST(test_polymorphic_const)
    {
      derived_1 derived1;
      derived1.set();

      const test_variant_polymorphic variant1(derived1);
      CHECK(variant1.is_base_of<base>());
      CHECK(!variant1.is_base_of<not_base>());
      CHECK_EQUAL(1, variant1.upcast<base>().value);
      CHECK_THROW(variant1.upcast<not_base>(), etl::legacy::variant_not_a_base_exception);
      CHECK_EQUAL((void*)(0U), variant1.upcast_ptr<not_base>());

      derived_2 derived2;
      derived2.set();

      const test_variant_polymorphic variant2(derived2);
      CHECK(variant2.is_base_of<base>());
      CHECK(!variant2.is_base_of<not_base>());
      CHECK_EQUAL(2, variant2.upcast<base>().value);
      CHECK_THROW(variant2.upcast<not_base>(), etl::legacy::variant_not_a_base_exception);
      CHECK_EQUAL((void*)(0U), variant2.upcast_ptr<not_base>());
    }

    //*************************************************************************
    TEST(test_emplace)
    {
      test_variant_emplace variant;

      variant.emplace<D1>("1");
      CHECK(variant.is_type<D1>());
      CHECK_EQUAL(D1("1"), variant.get<D1>());

      variant.emplace<D2>("1", "2");
      CHECK(variant.is_type<D2>());
      CHECK_EQUAL(D2("1", "2"), variant.get<D2>());

      variant.emplace<D3>("1", "2", "3");
      CHECK(variant.is_type<D3>());
      CHECK_EQUAL(D3("1", "2", "3"), variant.get<D3>());

      variant.emplace<D4>("1", "2", "3", "4");
      CHECK(variant.is_type<D4>());
      CHECK_EQUAL(D4("1", "2", "3", "4"), variant.get<D4>());
    }

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

      template<typename T>
      int8_t operator()(T const&) const 
      {
        return -1;
      }
    };

    //*************************************************************************
    TEST(test_variant_visit)
    {
      test_variant_3 variant;
      variant = int8_t{};
      // c++98 should generate a const ref of dispatcher.
      int16_t type = etl::legacy::visit<int16_t>(variant_test_visit_dispatcher{}, variant);
      CHECK_EQUAL(1, type);
      test_variant_3 const& variant_const = variant;
      type = etl::legacy::visit<int16_t>(variant_test_visit_dispatcher{}, variant_const);
      CHECK_EQUAL(10, type);

      variant_test_visit_dispatcher visitor;
      type = etl::legacy::visit<int16_t>(visitor, variant_const);
      CHECK_EQUAL(50, type);

      variant = int16_t{};
      type = etl::legacy::visit<int16_t>(variant_test_visit_dispatcher{}, variant);
      CHECK_EQUAL(3, type);

      type = etl::legacy::visit<int16_t>(variant_test_visit_dispatcher{}, variant_const);
      CHECK_EQUAL(30, type);

      type = etl::legacy::visit<int16_t>(visitor, variant_const);
      CHECK_EQUAL(70, type);
    }
  };
}

#include "etl/private/diagnostic_pop.h"