/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl

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

#include <UnitTest++/UnitTest++.h>
#include "ExtraCheckMacros.h"

#include "../variant.h"

#include <array>
#include <vector>
#include <algorithm>

// Test classes for polymorphic tests.
struct base
{
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
typedef etl::variant<char, int, std::string> test_variant_1;
typedef etl::variant<int, short, double> test_variant_2;
typedef etl::variant<derived_1, derived_2> test_variant_polymorphic;
typedef etl::variant<char, unsigned char, short, unsigned short, int, unsigned int, long, unsigned long> test_variant_max_types;

// This line should compile with no errors.
test_variant_max_types variant_max;

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

      CHECK_EQUAL(int(etl::alignment_of<char>::value),   int(etl::alignment_of<test_variant_a>::value));
      CHECK_EQUAL(int(etl::alignment_of<short>::value),  int(etl::alignment_of<test_variant_b>::value));
      CHECK_EQUAL(int(etl::alignment_of<int>::value),    int(etl::alignment_of<test_variant_c>::value));
      CHECK_EQUAL(int(etl::alignment_of<double>::value), int(etl::alignment_of<test_variant_d>::value));
    }

    //*************************************************************************
    TEST(test_constructor_default)
    {
      CHECK_NO_THROW(test_variant_1 variant);
    }

    //*************************************************************************
    TEST(test_constructor_value)
    {
      // Char.
      char c = 'a';
      test_variant_1 variant_char(c);

      CHECK(variant_char.is_type<char>());
      CHECK(variant_char.is_valid());
      CHECK_EQUAL(c, variant_char.get<char>());

      // Int.
      int i = 1;
      test_variant_1 variant_int(i);

      CHECK(variant_int.is_type<int>());
      CHECK(variant_int.is_valid());
      CHECK_EQUAL(i, variant_int.get<int>());

      // String.
      std::string text("Some Text");
      test_variant_1 variant_text(text);

      CHECK(variant_text.is_type<std::string>());
      CHECK(variant_text.is_valid());
      CHECK_EQUAL(text, variant_text.get<std::string>());
    }

    //*************************************************************************
    TEST(test_copy_constructor)
    {
      std::string text("Some Text");
      test_variant_1 variant_1(text);

      test_variant_1 variant_2(variant_1);

      CHECK(variant_1.is_same_type(variant_2));
      CHECK(variant_2.is_valid());
      CHECK_EQUAL(variant_1.get<std::string>(), variant_2.get<std::string>());
    }

    //*************************************************************************
    TEST(test_assign_from_value)
    {
      std::string text("Some Text");
      test_variant_1 variant;

      variant = text;

      CHECK_EQUAL(text, variant.get<std::string>());
      CHECK(variant.is_valid());
    }

    //*************************************************************************
    TEST(test_assign_from_variant)
    {
      std::string text("Some Text");
      test_variant_1 variant_1;
      test_variant_1 variant_2;

      variant_1 = text;
      variant_2 = variant_1;

      CHECK_EQUAL(text, variant_2.get<std::string>());
      CHECK(variant_2.is_valid());
    }

    //*************************************************************************
    TEST(test_assignment_incorrect_type_exception)
    {
      std::string text("Some Text");
      test_variant_1 variant;
      variant = text;
        
      CHECK_THROW(int i = variant, etl::variant_incorrect_type_exception);
    }

    //*************************************************************************
    TEST(test_is_same_type)
    {
      std::string text("Some Text");
      test_variant_1 variant_1;
      test_variant_1 variant_2;

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
      test_variant_1 variant_1;
      test_variant_2 variant_2;

      variant_1 = 1;
      variant_2 = 2;

      CHECK(variant_1.is_same_type(variant_2));

      variant_1 = 'a';
      variant_2 = 3.3;

      CHECK(!variant_1.is_same_type(variant_2));
    } 
    
    //*************************************************************************
    TEST(Testis_supported_type)
    {
      CHECK(test_variant_1::is_supported_type<char>());
      CHECK(test_variant_1::is_supported_type<int>());
      CHECK(test_variant_1::is_supported_type<std::string>());
      CHECK(!test_variant_1::is_supported_type<short>());
    }

    //*************************************************************************
    TEST(TestIsValid)
    {
      test_variant_1 variant;

      CHECK(!variant.is_valid());

      variant = 1;
      CHECK(variant.is_valid());
    }

    //*************************************************************************
    TEST(TestIsType)
    {
      test_variant_1 variant;
      variant = 1;
      CHECK(variant.is_type<int>());

      variant = 'a';
      CHECK(variant.is_type<char>());

      variant = std::string("Some Text");
      CHECK(variant.is_type<std::string>());
    }

    //*************************************************************************
    TEST(TestGet)
    {
      test_variant_1 variant;

      variant = 1;
      CHECK_EQUAL(1, variant.get<int>());

      variant = 'a';
      CHECK_EQUAL('a', variant.get<char>());

      variant = std::string("Some Text");
      CHECK_EQUAL(std::string("Some Text"), variant.get<std::string>());
    }

    //*************************************************************************
    TEST(TestGetException)
    {
      test_variant_1 variant;
      variant = 1;
        
      CHECK_THROW(char c =variant.get<char>(), etl::variant_exception);
    }

    //*************************************************************************
    TEST(TestGetReference)
    {
      test_variant_1 variant;
      variant = 1;

      int& ir = variant.get<int>();
      CHECK_EQUAL(ir, variant.get<int>());

      ir = 2;
      CHECK_EQUAL(ir, variant.get<int>());
    }

    //*************************************************************************
    TEST(TestGetConstReference)
    {
      test_variant_1 variant;
      variant = 1;
      const int& ir = variant.get<int>();
      CHECK_EQUAL(1, ir);

      variant = 2;
      CHECK_EQUAL(2, ir);
    }

    //*************************************************************************
    TEST(TestCallReader)
    {
      class reader : public test_variant_1::reader
      {
      public:

        reader() : c(' '), s(""), i(0)
        {
        }

        void read(char c_) override
        {
          c = c_;
        }

        void read(int i_)
        {
          i = i_;
        }

        void read(const std::string& s_)
        {
          s = s_;
        }

        char c;       
        std::string s;
        int i;
      };

      test_variant_1 variant;
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
    TEST(TestConversionOperators)
    {
      test_variant_1 variant;
      variant = 1;
      int i = variant;
      CHECK_EQUAL(i, variant.get<int>());

      variant = std::string("Some Text");
      std::string text = variant;
      CHECK_EQUAL(text, variant.get<std::string>());

      variant = 'a';
      char c = variant;
      CHECK_EQUAL(c, variant.get<char>());
    }

    //*************************************************************************
    TEST(TestConversionOperatorsConstReference)
    {
      test_variant_1 variant;
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
    TEST(TestSwap)
    {
      test_variant_1 variant_1;
      test_variant_1 variant_2;

      variant_1 = 1;
      variant_2 = std::string("Some Text");

      std::swap(variant_1, variant_2);

      CHECK_EQUAL(1, variant_2.get<int>());
      CHECK_EQUAL(std::string("Some Text"), variant_1.get<std::string>());
    }

    //*************************************************************************
    TEST(TestClear)
    {
      test_variant_1 variant;

      variant = 1;
      variant.clear();

      CHECK(!variant.is_valid());
    }

    //*************************************************************************
    TEST(TestPolymorphic)
    {
      test_variant_polymorphic variant;

      variant = derived_1();
      variant.upcast<base>().set();
      CHECK_EQUAL(1, variant.get<derived_1>().value);

      variant = derived_2();
      variant.upcast<base>().set();
      CHECK_EQUAL(2, variant.get<derived_2>().value);
    }
  };
}