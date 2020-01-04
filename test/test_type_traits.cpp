/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl
http://www.etlcpp.com

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

#include "UnitTest++.h"

#define IN_TYPE_TRAITS_TEST

#include "etl/type_traits.h"
#include <type_traits>

namespace
{
  struct TestData { };
}

namespace etl
{
  template <>
  struct size_of<TestData> : etlstd::integral_constant<size_t, 20U> {};
}

namespace
{
  // A class to test non-fundamental types.
  struct Test
  {
    int a;
  };

  // A class to test etl::is_one_of
  template <const int I>
  struct Type
  {
  };

  SUITE(test_type_traits)
  {
    //*************************************************************************
    TEST(test_is_integral)
    {
      CHECK(etlstd::is_integral<bool>::value               == std::is_integral<bool>::value);
      CHECK(etlstd::is_integral<char>::value               == std::is_integral<char>::value);
      CHECK(etlstd::is_integral<signed char>::value        == std::is_integral<signed char>::value);
      CHECK(etlstd::is_integral<unsigned char>::value      == std::is_integral<unsigned char>::value);
      CHECK(etlstd::is_integral<wchar_t>::value            == std::is_integral<wchar_t>::value);
      CHECK(etlstd::is_integral<short>::value              == std::is_integral<short>::value);
      CHECK(etlstd::is_integral<signed short>::value       == std::is_integral<signed short>::value);
      CHECK(etlstd::is_integral<unsigned short>::value     == std::is_integral<unsigned short>::value);
      CHECK(etlstd::is_integral<int>::value                == std::is_integral<int>::value);
      CHECK(etlstd::is_integral<signed int>::value         == std::is_integral<signed int>::value);
      CHECK(etlstd::is_integral<unsigned int>::value       == std::is_integral<unsigned int>::value);
      CHECK(etlstd::is_integral<long>::value               == std::is_integral<long>::value);
      CHECK(etlstd::is_integral<signed long>::value        == std::is_integral<signed long>::value);
      CHECK(etlstd::is_integral<unsigned long>::value      == std::is_integral<unsigned long>::value);
      CHECK(etlstd::is_integral<long long>::value          == std::is_integral<long long>::value);
      CHECK(etlstd::is_integral<signed long long>::value   == std::is_integral<signed long long>::value);
      CHECK(etlstd::is_integral<unsigned long long>::value == std::is_integral<unsigned long long>::value);
      CHECK(etlstd::is_integral<const int>::value          == std::is_integral<const int>::value);
      CHECK(etlstd::is_integral<volatile int>::value       == std::is_integral<volatile int>::value);
      CHECK(etlstd::is_integral<const int>::value          == std::is_integral<const int>::value);
      CHECK(etlstd::is_integral<const volatile int>::value == std::is_integral<const volatile int>::value);
      CHECK(etlstd::is_integral<float>::value              == std::is_integral<float>::value);
      CHECK(etlstd::is_integral<double>::value             == std::is_integral<double>::value);
      CHECK(etlstd::is_integral<long double>::value        == std::is_integral<long double>::value);
      CHECK(etlstd::is_integral<Test>::value               == std::is_integral<Test>::value);
    }

    //*************************************************************************
    TEST(test_is_signed)
    {
      CHECK(etlstd::is_signed<bool>::value               == std::is_signed<bool>::value);
      CHECK(etlstd::is_signed<char>::value               == std::is_signed<char>::value);
      CHECK(etlstd::is_signed<signed char>::value        == std::is_signed<signed char>::value);
      CHECK(etlstd::is_signed<unsigned char>::value      == std::is_signed<unsigned char>::value);
      CHECK(etlstd::is_signed<wchar_t>::value            == std::is_signed<wchar_t>::value);
      CHECK(etlstd::is_signed<short>::value              == std::is_signed<short>::value);
      CHECK(etlstd::is_signed<signed short>::value       == std::is_signed<signed short>::value);
      CHECK(etlstd::is_signed<unsigned short>::value     == std::is_signed<unsigned short>::value);
      CHECK(etlstd::is_signed<int>::value                == std::is_signed<int>::value);
      CHECK(etlstd::is_signed<signed int>::value         == std::is_signed<signed int>::value);
      CHECK(etlstd::is_signed<unsigned int>::value       == std::is_signed<unsigned int>::value);
      CHECK(etlstd::is_signed<long>::value               == std::is_signed<long>::value);
      CHECK(etlstd::is_signed<signed long>::value        == std::is_signed<signed long>::value);
      CHECK(etlstd::is_signed<unsigned long>::value      == std::is_signed<unsigned long>::value);
      CHECK(etlstd::is_signed<long long>::value          == std::is_signed<long long>::value);
      CHECK(etlstd::is_signed<signed long long>::value   == std::is_signed<signed long long>::value);
      CHECK(etlstd::is_signed<unsigned long long>::value == std::is_signed<unsigned long long>::value);
      CHECK(etlstd::is_signed<const int>::value          == std::is_signed<const int>::value);
      CHECK(etlstd::is_signed<volatile int>::value       == std::is_signed<volatile int>::value);
      CHECK(etlstd::is_signed<const int>::value          == std::is_signed<const int>::value);
      CHECK(etlstd::is_signed<const volatile int>::value == std::is_signed<const volatile int>::value);
      CHECK(etlstd::is_signed<float>::value              == std::is_signed<float>::value);
      CHECK(etlstd::is_signed<double>::value             == std::is_signed<double>::value);
      CHECK(etlstd::is_signed<long double>::value        == std::is_signed<long double>::value);
      CHECK(etlstd::is_signed<Test>::value               == std::is_signed<Test>::value);
    }

    //*************************************************************************
    TEST(test_is_unsigned)
    {
      CHECK(etlstd::is_unsigned<bool>::value               == std::is_unsigned<bool>::value);
      CHECK(etlstd::is_unsigned<char>::value               == std::is_unsigned<char>::value);
      CHECK(etlstd::is_unsigned<signed char>::value        == std::is_unsigned<signed char>::value);
      CHECK(etlstd::is_unsigned<unsigned char>::value      == std::is_unsigned<unsigned char>::value);
      CHECK(etlstd::is_unsigned<signed char>::value        == std::is_unsigned<signed char>::value);
      CHECK(etlstd::is_unsigned<wchar_t>::value            == std::is_unsigned<wchar_t>::value);
      CHECK(etlstd::is_unsigned<short>::value              == std::is_unsigned<short>::value);
      CHECK(etlstd::is_unsigned<signed short>::value       == std::is_unsigned<signed short>::value);
      CHECK(etlstd::is_unsigned<unsigned short>::value     == std::is_unsigned<unsigned short>::value);
      CHECK(etlstd::is_unsigned<int>::value                == std::is_unsigned<int>::value);
      CHECK(etlstd::is_unsigned<signed int>::value         == std::is_unsigned<signed int>::value);
      CHECK(etlstd::is_unsigned<unsigned int>::value       == std::is_unsigned<unsigned int>::value);
      CHECK(etlstd::is_unsigned<long>::value               == std::is_unsigned<long>::value);
      CHECK(etlstd::is_unsigned<signed long>::value        == std::is_unsigned<signed long>::value);
      CHECK(etlstd::is_unsigned<unsigned long>::value      == std::is_unsigned<unsigned long>::value);
      CHECK(etlstd::is_unsigned<long long>::value          == std::is_unsigned<long long>::value);
      CHECK(etlstd::is_unsigned<signed long long>::value   == std::is_unsigned<signed long long>::value);
      CHECK(etlstd::is_unsigned<unsigned long long>::value == std::is_unsigned<unsigned long long>::value);
      CHECK(etlstd::is_unsigned<const int>::value          == std::is_unsigned<const int>::value);
      CHECK(etlstd::is_unsigned<volatile int>::value       == std::is_unsigned<volatile int>::value);
      CHECK(etlstd::is_unsigned<const int>::value          == std::is_unsigned<const int>::value);
      CHECK(etlstd::is_unsigned<const volatile int>::value == std::is_unsigned<const volatile int>::value);
      CHECK(etlstd::is_unsigned<float>::value              == std::is_unsigned<float>::value);
      CHECK(etlstd::is_unsigned<double>::value             == std::is_unsigned<double>::value);
      CHECK(etlstd::is_unsigned<long double>::value        == std::is_unsigned<long double>::value);
      CHECK(etlstd::is_unsigned<Test>::value               == std::is_unsigned<Test>::value);
    }

    //*************************************************************************
    TEST(test_is_floating_point)
    {
      CHECK(etlstd::is_floating_point<bool>::value               == std::is_floating_point<bool>::value);
      CHECK(etlstd::is_floating_point<char>::value               == std::is_floating_point<char>::value);
      CHECK(etlstd::is_floating_point<signed char>::value        == std::is_floating_point<signed char>::value);
      CHECK(etlstd::is_floating_point<unsigned char>::value      == std::is_floating_point<unsigned char>::value);
      CHECK(etlstd::is_floating_point<wchar_t>::value            == std::is_floating_point<wchar_t>::value);
      CHECK(etlstd::is_floating_point<short>::value              == std::is_floating_point<short>::value);
      CHECK(etlstd::is_floating_point<signed short>::value       == std::is_floating_point<signed short>::value);
      CHECK(etlstd::is_floating_point<unsigned short>::value     == std::is_floating_point<unsigned short>::value);
      CHECK(etlstd::is_floating_point<int>::value                == std::is_floating_point<int>::value);
      CHECK(etlstd::is_floating_point<signed int>::value         == std::is_floating_point<signed int>::value);
      CHECK(etlstd::is_floating_point<unsigned int>::value       == std::is_floating_point<unsigned int>::value);
      CHECK(etlstd::is_floating_point<long>::value               == std::is_floating_point<long>::value);
      CHECK(etlstd::is_floating_point<signed long>::value        == std::is_floating_point<signed long>::value);
      CHECK(etlstd::is_floating_point<unsigned long>::value      == std::is_floating_point<unsigned long>::value);
      CHECK(etlstd::is_floating_point<long long>::value          == std::is_floating_point<long long>::value);
      CHECK(etlstd::is_floating_point<signed long long>::value   == std::is_floating_point<signed long long>::value);
      CHECK(etlstd::is_floating_point<unsigned long long>::value == std::is_floating_point<unsigned long long>::value);
      CHECK(etlstd::is_floating_point<const int>::value          == std::is_floating_point<const int>::value);
      CHECK(etlstd::is_floating_point<volatile int>::value       == std::is_floating_point<volatile int>::value);
      CHECK(etlstd::is_floating_point<const int>::value          == std::is_floating_point<const int>::value);
      CHECK(etlstd::is_floating_point<const volatile int>::value == std::is_floating_point<const volatile int>::value);
      CHECK(etlstd::is_floating_point<float>::value              == std::is_floating_point<float>::value);
      CHECK(etlstd::is_floating_point<double>::value             == std::is_floating_point<double>::value);
      CHECK(etlstd::is_floating_point<long double>::value        == std::is_floating_point<long double>::value);
      CHECK(etlstd::is_floating_point<Test>::value               == std::is_signed<Test>::value);
    }

    //*************************************************************************
    TEST(test_is_pointer)
    {
      CHECK(etlstd::is_pointer<int>::value                 == std::is_pointer<int>::value);
      CHECK(etlstd::is_pointer<int*>::value                == std::is_pointer<int*>::value);
      CHECK(etlstd::is_pointer<const int*>::value          == std::is_pointer<const int*>::value);
      CHECK(etlstd::is_pointer<volatile int*>::value       == std::is_pointer<volatile int*>::value);
      CHECK(etlstd::is_pointer<const volatile int*>::value == std::is_pointer<const volatile int*>::value);
    }

    //*************************************************************************
    TEST(test_is_reference)
    {
      CHECK(etlstd::is_reference<int>::value                 == std::is_reference<int>::value);
      CHECK(etlstd::is_reference<int&>::value                == std::is_reference<int&>::value);
      CHECK(etlstd::is_reference<const int&>::value          == std::is_reference<const int&>::value);
      CHECK(etlstd::is_reference<volatile int&>::value       == std::is_reference<volatile int&>::value);
      CHECK(etlstd::is_reference<const volatile int&>::value == std::is_reference<const volatile int&>::value);
    }

    //*************************************************************************
    TEST(test_is_same)
    {
      CHECK((etlstd::is_same<int, int>::value  == etlstd::is_same<int, int>::value));
      CHECK((etlstd::is_same<char, int>::value == etlstd::is_same<char, int>::value));
    }

    //*************************************************************************
    TEST(test_is_array)
    {
      CHECK(etlstd::is_array<int>::value     == std::is_array<int>::value);
      CHECK(etlstd::is_array<int[10]>::value == std::is_array<int[10]>::value);
    }

    //*************************************************************************
    TEST(test_remove_pointer)
    {
      CHECK((etlstd::is_same<etlstd::remove_pointer<int>::type,                       std::remove_pointer<int>::type>::value));
      CHECK((etlstd::is_same<etlstd::remove_pointer<const int>::type,                 std::remove_pointer<const int>::type>::value));
      CHECK((etlstd::is_same<etlstd::remove_pointer<int*>::type,                      std::remove_pointer<int*>::type>::value));
      CHECK((etlstd::is_same<etlstd::remove_pointer<int* const>::type,                std::remove_pointer<int*>::type>::value));
      CHECK((etlstd::is_same<etlstd::remove_pointer<const int*>::type,                std::remove_pointer<const int*>::type>::value));
      CHECK((etlstd::is_same<etlstd::remove_pointer<const int* const>::type,          std::remove_pointer<const int* const>::type>::value));

      CHECK((etlstd::is_same<etlstd::remove_pointer<volatile int>::type,              std::remove_pointer<volatile int>::type>::value));
      CHECK((etlstd::is_same<etlstd::remove_pointer<volatile const int>::type,        std::remove_pointer<volatile const int>::type>::value));
      CHECK((etlstd::is_same<etlstd::remove_pointer<volatile int*>::type,             std::remove_pointer<volatile int*>::type>::value));
      CHECK((etlstd::is_same<etlstd::remove_pointer<volatile int*const >::type,       std::remove_pointer<volatile int*>::type>::value));
      CHECK((etlstd::is_same<etlstd::remove_pointer<const volatile int*>::type,       std::remove_pointer<const volatile int*>::type>::value));
      CHECK((etlstd::is_same<etlstd::remove_pointer<const volatile int* const>::type, std::remove_pointer<const volatile int* const>::type>::value));
    }

    //*************************************************************************
    TEST(test_add_pointer)
    {
      CHECK((etlstd::is_same<etlstd::add_pointer<int>::type,                       std::add_pointer<int>::type>::value));
      CHECK((etlstd::is_same<etlstd::add_pointer<const int>::type,                 std::add_pointer<const int>::type>::value));
      CHECK((etlstd::is_same<etlstd::add_pointer<int*>::type,                      std::add_pointer<int*>::type>::value));
      CHECK((etlstd::is_same<etlstd::add_pointer<int* const >::type,               std::add_pointer<int* const>::type>::value));
      CHECK((etlstd::is_same<etlstd::add_pointer<const int*>::type,                std::add_pointer<const int*>::type>::value));
      CHECK((etlstd::is_same<etlstd::add_pointer<const int* const>::type,          std::add_pointer<const int* const>::type>::value));
      CHECK((etlstd::is_same<etlstd::add_pointer<volatile int*>::type,             std::add_pointer<volatile int*>::type>::value));
      CHECK((etlstd::is_same<etlstd::add_pointer<const volatile int*>::type,       std::add_pointer<const volatile int*>::type>::value));
      CHECK((etlstd::is_same<etlstd::add_pointer<volatile int* const>::type,       std::add_pointer<volatile int* const>::type>::value));
      CHECK((etlstd::is_same<etlstd::add_pointer<const volatile int* const>::type, std::add_pointer<const volatile int* const>::type>::value));

    }

    //*************************************************************************
    TEST(test_remove_reference)
    {
      CHECK((etlstd::is_same<etlstd::remove_reference<int>::type,                 std::remove_reference<int>::type>::value));
      CHECK((etlstd::is_same<etlstd::remove_reference<int&>::type,                std::remove_reference<int&>::type>::value));
      CHECK((etlstd::is_same<etlstd::remove_reference<const int&>::type,          std::remove_reference<const int&>::type>::value));
      CHECK((etlstd::is_same<etlstd::remove_reference<volatile int&>::type,       std::remove_reference<volatile int&>::type>::value));
      CHECK((etlstd::is_same<etlstd::remove_reference<const volatile int&>::type, std::remove_reference<const volatile int&>::type>::value));
    }

    //*************************************************************************
    TEST(test_remove_const)
    {
      CHECK((etlstd::is_same<etlstd::remove_const<int>::type,                std::remove_const<int>::type>::value));
      CHECK((etlstd::is_same<etlstd::remove_const<const int>::type,          std::remove_const<const int>::type>::value));
      CHECK((etlstd::is_same<etlstd::remove_const<const volatile int>::type, std::remove_const<const volatile int>::type>::value));
    }

    //*************************************************************************
    TEST(test_add_const)
    {
      CHECK((etlstd::is_same<etlstd::add_const<int>::type,                std::add_const<int>::type>::value));
      CHECK((etlstd::is_same<etlstd::add_const<const int>::type,          std::add_const<const int>::type>::value));
      CHECK((etlstd::is_same<etlstd::add_const<const volatile int>::type, std::add_const<const volatile int>::type>::value));
    }

    //*************************************************************************
    TEST(test_is_const)
    {
      CHECK(etlstd::is_const<int>::value                == std::is_const<int>::value);
      CHECK(etlstd::is_const<const int>::value          == std::is_const<const int>::value);
      CHECK(etlstd::is_const<const volatile int>::value == std::is_const<const volatile int>::value);
    }

    //*************************************************************************
    TEST(test_is_volatile)
    {
      CHECK(etlstd::is_volatile<int>::value                == std::is_volatile<int>::value);
      CHECK(etlstd::is_volatile<volatile int>::value       == std::is_volatile<volatile int>::value);
      CHECK(etlstd::is_volatile<const volatile int>::value == std::is_volatile<const volatile int>::value);
    }

    //*************************************************************************
    TEST(test_remove_volatile)
    {
      CHECK((etlstd::is_same<etlstd::remove_volatile<int>::type,                std::remove_volatile<int>::type>::value));
      CHECK((etlstd::is_same<etlstd::remove_volatile<volatile int>::type,       std::remove_volatile<volatile int>::type>::value));
      CHECK((etlstd::is_same<etlstd::remove_volatile<const volatile int>::type, std::remove_volatile<const volatile int>::type>::value));
    }

    //*************************************************************************
    TEST(test_add_volatile)
    {
      CHECK((etlstd::is_same<etlstd::add_volatile<int>::type,                std::add_volatile<int>::type>::value));
      CHECK((etlstd::is_same<etlstd::add_volatile<volatile int>::type,       std::add_volatile<volatile int>::type>::value));
      CHECK((etlstd::is_same<etlstd::add_volatile<const volatile int>::type, std::add_volatile<const volatile int>::type>::value));
    }

    //*************************************************************************
    TEST(test_remove_cv)
    {
      CHECK((etlstd::is_same<etlstd::remove_cv<int>::type,                std::remove_cv<int>::type>::value));
      CHECK((etlstd::is_same<etlstd::remove_cv<const int>::type,          std::remove_cv<const int>::type>::value));
      CHECK((etlstd::is_same<etlstd::remove_cv<volatile int>::type,       std::remove_cv<volatile int>::type>::value));
      CHECK((etlstd::is_same<etlstd::remove_cv<const volatile int>::type, std::remove_cv<const volatile int>::type>::value));
    }

    //*************************************************************************
    TEST(test_add_cv)
    {

      typedef etlstd::add_cv<int>::type t1;
      typedef std::add_cv<int>::type t2;

      bool pass = etlstd::is_same<t1, t2>::value;
        //etlstd::is_same<etlstd::add_cv<int>::type,                std::add_cv<int>::type>::value;

      CHECK(pass);
      CHECK((etlstd::is_same<etlstd::add_cv<const int>::type,          std::add_cv<const int>::type>::value));
      CHECK((etlstd::is_same<etlstd::add_cv<volatile int>::type,       std::add_cv<volatile int>::type>::value));
      CHECK((etlstd::is_same<etlstd::add_cv<const volatile int>::type, std::add_cv<const volatile int>::type>::value));
    }

    //*************************************************************************
    TEST(test_is_arithmetic)
    {
      CHECK(etlstd::is_arithmetic<bool>::value               == std::is_arithmetic<bool>::value);
      CHECK(etlstd::is_arithmetic<char>::value               == std::is_arithmetic<char>::value);
      CHECK(etlstd::is_arithmetic<signed char>::value        == std::is_arithmetic<signed char>::value);
      CHECK(etlstd::is_arithmetic<unsigned char>::value      == std::is_arithmetic<unsigned char>::value);
      CHECK(etlstd::is_arithmetic<wchar_t>::value            == std::is_arithmetic<wchar_t>::value);
      CHECK(etlstd::is_arithmetic<short>::value              == std::is_arithmetic<short>::value);
      CHECK(etlstd::is_arithmetic<signed short>::value       == std::is_arithmetic<signed short>::value);
      CHECK(etlstd::is_arithmetic<unsigned short>::value     == std::is_arithmetic<unsigned short>::value);
      CHECK(etlstd::is_arithmetic<int>::value                == std::is_arithmetic<int>::value);
      CHECK(etlstd::is_arithmetic<signed int>::value         == std::is_arithmetic<signed int>::value);
      CHECK(etlstd::is_arithmetic<unsigned int>::value       == std::is_arithmetic<unsigned int>::value);
      CHECK(etlstd::is_arithmetic<long>::value               == std::is_arithmetic<long>::value);
      CHECK(etlstd::is_arithmetic<signed long>::value        == std::is_arithmetic<signed long>::value);
      CHECK(etlstd::is_arithmetic<unsigned long>::value      == std::is_arithmetic<unsigned long>::value);
      CHECK(etlstd::is_arithmetic<long long>::value          == std::is_arithmetic<long long>::value);
      CHECK(etlstd::is_arithmetic<signed long long>::value   == std::is_arithmetic<signed long long>::value);
      CHECK(etlstd::is_arithmetic<unsigned long long>::value == std::is_arithmetic<unsigned long long>::value);
      CHECK(etlstd::is_arithmetic<const int>::value          == std::is_arithmetic<const int>::value);
      CHECK(etlstd::is_arithmetic<volatile int>::value       == std::is_arithmetic<volatile int>::value);
      CHECK(etlstd::is_arithmetic<const int>::value          == std::is_arithmetic<const int>::value);
      CHECK(etlstd::is_arithmetic<const volatile int>::value == std::is_arithmetic<const volatile int>::value);
      CHECK(etlstd::is_arithmetic<float>::value              == std::is_arithmetic<float>::value);
      CHECK(etlstd::is_arithmetic<double>::value             == std::is_arithmetic<double>::value);
      CHECK(etlstd::is_arithmetic<long double>::value        == std::is_arithmetic<long double>::value);
      CHECK(etlstd::is_arithmetic<Test>::value               == std::is_arithmetic<Test>::value);
    }

    //*************************************************************************
    TEST(test_is_fundamental)
    {
      CHECK(etlstd::is_fundamental<void>::value               == std::is_fundamental<void>::value);
      CHECK(etlstd::is_fundamental<bool>::value               == std::is_fundamental<bool>::value);
      CHECK(etlstd::is_fundamental<char>::value               == std::is_fundamental<char>::value);
      CHECK(etlstd::is_fundamental<signed char>::value        == std::is_fundamental<signed char>::value);
      CHECK(etlstd::is_fundamental<unsigned char>::value      == std::is_fundamental<unsigned char>::value);
      CHECK(etlstd::is_fundamental<wchar_t>::value            == std::is_fundamental<wchar_t>::value);
      CHECK(etlstd::is_fundamental<short>::value              == std::is_fundamental<short>::value);
      CHECK(etlstd::is_fundamental<signed short>::value       == std::is_fundamental<signed short>::value);
      CHECK(etlstd::is_fundamental<unsigned short>::value     == std::is_fundamental<unsigned short>::value);
      CHECK(etlstd::is_fundamental<int>::value                == std::is_fundamental<int>::value);
      CHECK(etlstd::is_fundamental<signed int>::value         == std::is_fundamental<signed int>::value);
      CHECK(etlstd::is_fundamental<unsigned int>::value       == std::is_fundamental<unsigned int>::value);
      CHECK(etlstd::is_fundamental<long>::value               == std::is_fundamental<long>::value);
      CHECK(etlstd::is_fundamental<signed long>::value        == std::is_fundamental<signed long>::value);
      CHECK(etlstd::is_fundamental<unsigned long>::value      == std::is_fundamental<unsigned long>::value);
      CHECK(etlstd::is_fundamental<long long>::value          == std::is_fundamental<long long>::value);
      CHECK(etlstd::is_fundamental<signed long long>::value   == std::is_fundamental<signed long long>::value);
      CHECK(etlstd::is_fundamental<unsigned long long>::value == std::is_fundamental<unsigned long long>::value);
      CHECK(etlstd::is_fundamental<const int>::value          == std::is_fundamental<const int>::value);
      CHECK(etlstd::is_fundamental<volatile int>::value       == std::is_fundamental<volatile int>::value);
      CHECK(etlstd::is_fundamental<const int>::value          == std::is_fundamental<const int>::value);
      CHECK(etlstd::is_fundamental<const volatile int>::value == std::is_fundamental<const volatile int>::value);
      CHECK(etlstd::is_fundamental<float>::value              == std::is_fundamental<float>::value);
      CHECK(etlstd::is_fundamental<double>::value             == std::is_fundamental<double>::value);
      CHECK(etlstd::is_fundamental<long double>::value        == std::is_fundamental<long double>::value);
      CHECK(etlstd::is_fundamental<Test>::value               == std::is_fundamental<Test>::value);
    }

    //*************************************************************************
    TEST(test_is_compound)
    {
      CHECK(etlstd::is_compound<void>::value               == std::is_compound<void>::value);
      CHECK(etlstd::is_compound<bool>::value               == std::is_compound<bool>::value);
      CHECK(etlstd::is_compound<char>::value               == std::is_compound<char>::value);
      CHECK(etlstd::is_compound<signed char>::value        == std::is_compound<signed char>::value);
      CHECK(etlstd::is_compound<unsigned char>::value      == std::is_compound<unsigned char>::value);
      CHECK(etlstd::is_compound<wchar_t>::value            == std::is_compound<wchar_t>::value);
      CHECK(etlstd::is_compound<short>::value              == std::is_compound<short>::value);
      CHECK(etlstd::is_compound<signed short>::value       == std::is_compound<signed short>::value);
      CHECK(etlstd::is_compound<unsigned short>::value     == std::is_compound<unsigned short>::value);
      CHECK(etlstd::is_compound<int>::value                == std::is_compound<int>::value);
      CHECK(etlstd::is_compound<signed int>::value         == std::is_compound<signed int>::value);
      CHECK(etlstd::is_compound<unsigned int>::value       == std::is_compound<unsigned int>::value);
      CHECK(etlstd::is_compound<long>::value               == std::is_compound<long>::value);
      CHECK(etlstd::is_compound<signed long>::value        == std::is_compound<signed long>::value);
      CHECK(etlstd::is_compound<unsigned long>::value      == std::is_compound<unsigned long>::value);
      CHECK(etlstd::is_compound<long long>::value          == std::is_compound<long long>::value);
      CHECK(etlstd::is_compound<signed long long>::value   == std::is_compound<signed long long>::value);
      CHECK(etlstd::is_compound<unsigned long long>::value == std::is_compound<unsigned long long>::value);
      CHECK(etlstd::is_compound<const int>::value          == std::is_compound<const int>::value);
      CHECK(etlstd::is_compound<volatile int>::value       == std::is_compound<volatile int>::value);
      CHECK(etlstd::is_compound<const int>::value          == std::is_compound<const int>::value);
      CHECK(etlstd::is_compound<const volatile int>::value == std::is_compound<const volatile int>::value);
      CHECK(etlstd::is_compound<float>::value              == std::is_compound<float>::value);
      CHECK(etlstd::is_compound<double>::value             == std::is_compound<double>::value);
      CHECK(etlstd::is_compound<long double>::value        == std::is_compound<long double>::value);
      CHECK(etlstd::is_compound<Test>::value               == std::is_compound<Test>::value);
    }

    //*************************************************************************
    TEST(test_is_void)
    {
      CHECK(etlstd::is_void<int>::value  == std::is_void<int>::value);
      CHECK(etlstd::is_void<void>::value == std::is_void<void>::value);
    }

    //*************************************************************************
    TEST(test_make_signed)
    {
      CHECK((etlstd::is_same<etlstd::make_signed<char>::type,               std::make_signed<char>::type>::value));
      CHECK((etlstd::is_same<etlstd::make_signed<signed char>::type,        std::make_signed<signed char>::type>::value));
      CHECK((etlstd::is_same<etlstd::make_signed<unsigned char>::type,      std::make_signed<unsigned char>::type>::value));
      CHECK(etlstd::is_signed<etlstd::make_signed<wchar_t>::type>::value && (sizeof(wchar_t) == sizeof(etlstd::make_signed<wchar_t>::type)));
      CHECK((etlstd::is_same<etlstd::make_signed<short>::type,              std::make_signed<short>::type>::value));
      CHECK((etlstd::is_same<etlstd::make_signed<signed short>::type,       std::make_signed<signed short>::type>::value));
      CHECK((etlstd::is_same<etlstd::make_signed<unsigned short>::type,     std::make_signed<unsigned short>::type>::value));
      CHECK((etlstd::is_same<etlstd::make_signed<int>::type,                std::make_signed<int>::type>::value));
      CHECK((etlstd::is_same<etlstd::make_signed<signed int>::type,         std::make_signed<signed int>::type>::value));
      CHECK((etlstd::is_same<etlstd::make_signed<unsigned int>::type,       std::make_signed<unsigned int>::type>::value));
      CHECK((etlstd::is_same<etlstd::make_signed<long>::type,               std::make_signed<long>::type>::value));
      CHECK((etlstd::is_same<etlstd::make_signed<signed long>::type,        std::make_signed<signed long>::type>::value));
      CHECK((etlstd::is_same<etlstd::make_signed<unsigned long>::type,      std::make_signed<unsigned long>::type>::value));
      CHECK((etlstd::is_same<etlstd::make_signed<long long>::type,          std::make_signed<long long>::type>::value));
      CHECK((etlstd::is_same<etlstd::make_signed<signed long long>::type,   std::make_signed<signed long long>::type>::value));
      CHECK((etlstd::is_same<etlstd::make_signed<unsigned long long>::type, std::make_signed<unsigned long long>::type>::value));
      CHECK((etlstd::is_same<etlstd::make_signed<const int>::type,          std::make_signed<const int>::type>::value));
      CHECK((etlstd::is_same<etlstd::make_signed<volatile int>::type,       std::make_signed<volatile int>::type>::value));
      CHECK((etlstd::is_same<etlstd::make_signed<const int>::type,          std::make_signed<const int>::type>::value));
      CHECK((etlstd::is_same<etlstd::make_signed<const volatile int>::type, std::make_signed<const volatile int>::type>::value));
      CHECK((etlstd::is_same<etlstd::make_signed<size_t>::type,             std::make_signed<size_t>::type>::value));
    }

    //*************************************************************************
    TEST(test_make_unsigned)
    {
      CHECK((etlstd::is_same<etlstd::make_unsigned<char>::type,               std::make_unsigned<char>::type>::value));
      CHECK((etlstd::is_same<etlstd::make_unsigned<signed char>::type,        std::make_unsigned<signed char>::type>::value));
      CHECK((etlstd::is_same<etlstd::make_unsigned<unsigned char>::type,      std::make_unsigned<unsigned char>::type>::value));
      CHECK(etlstd::is_unsigned<etlstd::make_unsigned<wchar_t>::type>::value && (sizeof(wchar_t) == sizeof(etlstd::make_unsigned<wchar_t>::type)));
      CHECK((etlstd::is_same<etlstd::make_unsigned<short>::type,              std::make_unsigned<short>::type>::value));
      CHECK((etlstd::is_same<etlstd::make_unsigned<signed short>::type,       std::make_unsigned<signed short>::type>::value));
      CHECK((etlstd::is_same<etlstd::make_unsigned<unsigned short>::type,     std::make_unsigned<unsigned short>::type>::value));
      CHECK((etlstd::is_same<etlstd::make_unsigned<int>::type,                std::make_unsigned<int>::type>::value));
      CHECK((etlstd::is_same<etlstd::make_unsigned<signed int>::type,         std::make_unsigned<signed int>::type>::value));
      CHECK((etlstd::is_same<etlstd::make_unsigned<unsigned int>::type,       std::make_unsigned<unsigned int>::type>::value));
      CHECK((etlstd::is_same<etlstd::make_unsigned<long>::type,               std::make_unsigned<long>::type>::value));
      CHECK((etlstd::is_same<etlstd::make_unsigned<signed long>::type,        std::make_unsigned<signed long>::type>::value));
      CHECK((etlstd::is_same<etlstd::make_unsigned<unsigned long>::type,      std::make_unsigned<unsigned long>::type>::value));
      CHECK((etlstd::is_same<etlstd::make_unsigned<long long>::type,          std::make_unsigned<long long>::type>::value));
      CHECK((etlstd::is_same<etlstd::make_unsigned<signed long long>::type,   std::make_unsigned<signed long long>::type>::value));
      CHECK((etlstd::is_same<etlstd::make_unsigned<unsigned long long>::type, std::make_unsigned<unsigned long long>::type>::value));
      CHECK((etlstd::is_same<etlstd::make_unsigned<const int>::type,          std::make_unsigned<const int>::type>::value));
      CHECK((etlstd::is_same<etlstd::make_unsigned<volatile int>::type,       std::make_unsigned<volatile int>::type>::value));
      CHECK((etlstd::is_same<etlstd::make_unsigned<const int>::type,          std::make_unsigned<const int>::type>::value));
      CHECK((etlstd::is_same<etlstd::make_unsigned<const volatile int>::type, std::make_unsigned<const volatile int>::type>::value));
      CHECK((etlstd::is_same<etlstd::make_unsigned<size_t>::type,             std::make_unsigned<size_t>::type>::value));
    }

    //*************************************************************************
    TEST(test_extent)
    {
      CHECK(std::extent<int>::value     == etlstd::extent<int>::value);
      CHECK(std::extent<int[]>::value   == etlstd::extent<int[]>::value);
      CHECK(std::extent<int[10]>::value == etlstd::extent<int[10]>::value);
    }

    //*************************************************************************
    TEST(test_remove_extent)
    {
      CHECK((etlstd::is_same<etlstd::remove_extent<int>::type,     std::remove_extent<int>::type>::value));
      CHECK((etlstd::is_same<etlstd::remove_extent<int[]>::type,   std::remove_extent<int[]>::type>::value));
      CHECK((etlstd::is_same<etlstd::remove_extent<int[10]>::type, std::remove_extent<int[10]>::type>::value));
    }

    //*************************************************************************
    TEST(test_remove_all_extents)
    {
      CHECK((etlstd::is_same<etlstd::remove_all_extents<int>::type,         std::remove_all_extents<int>::type>::value));
      CHECK((etlstd::is_same<etlstd::remove_all_extents<int[10]>::type,     std::remove_all_extents<int[10]>::type>::value));
      CHECK((etlstd::is_same<etlstd::remove_all_extents<int[10][10]>::type, std::remove_all_extents<int[10][10]>::type>::value));
    }

    //*************************************************************************
    TEST(test_rank)
    {
      CHECK(etlstd::rank<int>::value         == std::rank<int>::value);
      CHECK(etlstd::rank<int[10]>::value     == std::rank<int[10]>::value);
      CHECK(etlstd::rank<int[10][10]>::value == std::rank<int[10][10]>::value);
    }

    //*************************************************************************
    TEST(test_alignment_of)
    {
      struct Test
      {
        int   a;
        char  b;
        float c;
      };

      CHECK(std::alignment_of<char>::value               == etlstd::alignment_of<char>::value);
      CHECK(std::alignment_of<unsigned char>::value      == etlstd::alignment_of<unsigned char>::value);
      CHECK(std::alignment_of<short>::value              == etlstd::alignment_of<short>::value);
      CHECK(std::alignment_of<unsigned short>::value     == etlstd::alignment_of<unsigned short>::value);
      CHECK(std::alignment_of<int>::value                == etlstd::alignment_of<int>::value);
      CHECK(std::alignment_of<unsigned int>::value       == etlstd::alignment_of<unsigned int>::value);
      CHECK(std::alignment_of<long>::value               == etlstd::alignment_of<long>::value);
      CHECK(std::alignment_of<unsigned long>::value      == etlstd::alignment_of<unsigned long>::value);
      CHECK(std::alignment_of<long long>::value          == etlstd::alignment_of<long long>::value);
      CHECK(std::alignment_of<unsigned long long>::value == etlstd::alignment_of<unsigned long long>::value);
      CHECK(std::alignment_of<float>::value              == etlstd::alignment_of<float>::value);
      CHECK(std::alignment_of<double>::value             == etlstd::alignment_of<double>::value);
      CHECK(std::alignment_of<Test>::value               == etlstd::alignment_of<Test>::value);
    }

    //*************************************************************************
    TEST(test_is_one_of)
    {
      typedef Type<0> T0;
      typedef Type<1> T1;
      typedef Type<2> T2;
      typedef Type<3> T3;
      typedef Type<4> T4;

      CHECK(bool(etl::is_one_of<char, char>::value));
      CHECK(!(etl::is_one_of<char, T0>::value));

      CHECK(bool(etl::is_one_of<char, T0, char>::value));
      CHECK(!(etl::is_one_of<char, T0, T1>::value));

      CHECK(bool(etl::is_one_of<char, T0, T1, char>::value));
      CHECK(!(etl::is_one_of<char, T0, T1, T2>::value));

      CHECK(bool(etl::is_one_of<char, T0, T1, T2, char>::value));
      CHECK(!(etl::is_one_of<char, T0, T1, T2, T3>::value));

      CHECK(bool(etl::is_one_of<char, T0, T1, T2, char>::value));
      CHECK(!(etl::is_one_of<char, T0, T1, T2, T3>::value));

      CHECK(bool(etl::is_one_of<char, char, T0, T1, T2, T3>::value));
      CHECK(bool(etl::is_one_of<char, T0, char, T1, T2, T3>::value));
      CHECK(bool(etl::is_one_of<char, T0, T1, char, T2, T3>::value));
      CHECK(bool(etl::is_one_of<char, T0, T1, T2, char, T3>::value));
      CHECK(bool(etl::is_one_of<char, T0, T1, T2, T3, char>::value));
      CHECK(!(etl::is_one_of<char, T0, T1, T2, T3, T4>::value));
    }

    //*************************************************************************
    TEST(test_is_base_of)
    {
      struct A { };
      struct B : public A { };
      struct C { };

      CHECK((std::is_base_of<A, A>::value) == (etlstd::is_base_of<A, A>::value));
      CHECK((std::is_base_of<A, B>::value) == (etlstd::is_base_of<A, B>::value));
      CHECK((std::is_base_of<A, C>::value) == (etlstd::is_base_of<A, C>::value));
      CHECK((std::is_base_of<B, A>::value) == (etlstd::is_base_of<B, A>::value));
      CHECK((std::is_base_of<B, B>::value) == (etlstd::is_base_of<B, B>::value));
      CHECK((std::is_base_of<B, C>::value) == (etlstd::is_base_of<B, C>::value));
      CHECK((std::is_base_of<C, A>::value) == (etlstd::is_base_of<C, A>::value));
      CHECK((std::is_base_of<C, B>::value) == (etlstd::is_base_of<C, B>::value));
      CHECK((std::is_base_of<C, C>::value) == (etlstd::is_base_of<C, C>::value));

      CHECK((std::is_base_of<char, char>::value) == (etlstd::is_base_of<char, char>::value));
      CHECK((std::is_base_of<char, int>::value)  == (etlstd::is_base_of<char, int>::value));
      CHECK((std::is_base_of<int,  char>::value) == (etlstd::is_base_of<int,  char>::value));
    }

    //*************************************************************************
    TEST(test_types)
    {
      CHECK((etlstd::is_same<int,              etl::types<int>::type>::value));
      CHECK((etlstd::is_same<int&,             etl::types<int>::reference>::value));
      CHECK((etlstd::is_same<const int&,       etl::types<int>::const_reference>::value));
      CHECK((etlstd::is_same<int*,             etl::types<int>::pointer>::value));
      CHECK((etlstd::is_same<const int*,       etl::types<int>::const_pointer>::value));
      CHECK((etlstd::is_same<const int* const, etl::types<int>::const_pointer_const>::value));

      CHECK((etlstd::is_same<int,              etl::types<const int>::type>::value));
      CHECK((etlstd::is_same<int&,             etl::types<const int>::reference>::value));
      CHECK((etlstd::is_same<const int&,       etl::types<const int>::const_reference>::value));
      CHECK((etlstd::is_same<int*,             etl::types<const int>::pointer>::value));
      CHECK((etlstd::is_same<const int*,       etl::types<const int>::const_pointer>::value));
      CHECK((etlstd::is_same<const int* const, etl::types<const int>::const_pointer_const>::value));

      CHECK((etlstd::is_same<int,              etl::types<int*>::type>::value));
      CHECK((etlstd::is_same<int&,             etl::types<int*>::reference>::value));
      CHECK((etlstd::is_same<const int&,       etl::types<int*>::const_reference>::value));
      CHECK((etlstd::is_same<int*,             etl::types<int*>::pointer>::value));
      CHECK((etlstd::is_same<const int*,       etl::types<int*>::const_pointer>::value));
      CHECK((etlstd::is_same<const int* const, etl::types<int*>::const_pointer_const>::value));

      CHECK((etlstd::is_same<int,              etl::types<int* const>::type>::value));
      CHECK((etlstd::is_same<int&,             etl::types<int* const>::reference>::value));
      CHECK((etlstd::is_same<const int&,       etl::types<int* const>::const_reference>::value));
      CHECK((etlstd::is_same<int*,             etl::types<int* const>::pointer>::value));
      CHECK((etlstd::is_same<const int*,       etl::types<int* const>::const_pointer>::value));
      CHECK((etlstd::is_same<const int* const, etl::types<int* const>::const_pointer_const>::value));

      CHECK((etlstd::is_same<int,              etl::types<const int*>::type>::value));
      CHECK((etlstd::is_same<int&,             etl::types<const int*>::reference>::value));
      CHECK((etlstd::is_same<const int&,       etl::types<const int*>::const_reference>::value));
      CHECK((etlstd::is_same<int*,             etl::types<const int*>::pointer>::value));
      CHECK((etlstd::is_same<const int*,       etl::types<const int*>::const_pointer>::value));
      CHECK((etlstd::is_same<const int* const, etl::types<const int*>::const_pointer_const>::value));

      CHECK((etlstd::is_same<int,              etl::types<const int* const>::type>::value));
      CHECK((etlstd::is_same<int&,             etl::types<const int* const>::reference>::value));
      CHECK((etlstd::is_same<const int&,       etl::types<const int* const>::const_reference>::value));
      CHECK((etlstd::is_same<int*,             etl::types<const int* const>::pointer>::value));
      CHECK((etlstd::is_same<const int*,       etl::types<const int* const>::const_pointer>::value));
      CHECK((etlstd::is_same<const int* const, etl::types<const int* const>::const_pointer_const>::value));

      CHECK((etlstd::is_same<int,              etl::types<int&>::type>::value));
      CHECK((etlstd::is_same<int&,             etl::types<int&>::reference>::value));
      CHECK((etlstd::is_same<const int&,       etl::types<int&>::const_reference>::value));
      CHECK((etlstd::is_same<int*,             etl::types<int&>::pointer>::value));
      CHECK((etlstd::is_same<const int*,       etl::types<int&>::const_pointer>::value));
      CHECK((etlstd::is_same<const int* const, etl::types<int&>::const_pointer_const>::value));

      CHECK((etlstd::is_same<int,              etl::types<const int&>::type>::value));
      CHECK((etlstd::is_same<int&,             etl::types<const int&>::reference>::value));
      CHECK((etlstd::is_same<const int&,       etl::types<const int&>::const_reference>::value));
      CHECK((etlstd::is_same<int*,             etl::types<const int&>::pointer>::value));
      CHECK((etlstd::is_same<const int*,       etl::types<const int&>::const_pointer>::value));
      CHECK((etlstd::is_same<const int* const, etl::types<const int&>::const_pointer_const>::value));

      CHECK((etlstd::is_same<int,              etl::types<volatile int>::type>::value));
      CHECK((etlstd::is_same<int&,             etl::types<volatile int>::reference>::value));
      CHECK((etlstd::is_same<const int&,       etl::types<volatile int>::const_reference>::value));
      CHECK((etlstd::is_same<int*,             etl::types<volatile int>::pointer>::value));
      CHECK((etlstd::is_same<const int*,       etl::types<volatile int>::const_pointer>::value));
      CHECK((etlstd::is_same<const int* const, etl::types<volatile int>::const_pointer_const>::value));

      CHECK((etlstd::is_same<int,              etl::types<const volatile int>::type>::value));
      CHECK((etlstd::is_same<int&,             etl::types<const volatile int>::reference>::value));
      CHECK((etlstd::is_same<const int&,       etl::types<const int>::const_reference>::value));
      CHECK((etlstd::is_same<int*,             etl::types<const volatile int>::pointer>::value));
      CHECK((etlstd::is_same<const int*,       etl::types<const volatile int>::const_pointer>::value));
      CHECK((etlstd::is_same<const int* const, etl::types<const volatile int>::const_pointer_const>::value));

      CHECK((etlstd::is_same<int,              etl::types<volatile int*>::type>::value));
      CHECK((etlstd::is_same<int&,             etl::types<volatile int*>::reference>::value));
      CHECK((etlstd::is_same<const int&,       etl::types<volatile int*>::const_reference>::value));
      CHECK((etlstd::is_same<int*,             etl::types<volatile int*>::pointer>::value));
      CHECK((etlstd::is_same<const int*,       etl::types<volatile int*>::const_pointer>::value));
      CHECK((etlstd::is_same<const int* const, etl::types<volatile int*>::const_pointer_const>::value));

      CHECK((etlstd::is_same<int,              etl::types<volatile int* const>::type>::value));
      CHECK((etlstd::is_same<int&,             etl::types<volatile int* const>::reference>::value));
      CHECK((etlstd::is_same<const int&,       etl::types<volatile int* const>::const_reference>::value));
      CHECK((etlstd::is_same<int*,             etl::types<volatile int* const>::pointer>::value));
      CHECK((etlstd::is_same<const int*,       etl::types<volatile int* const>::const_pointer>::value));
      CHECK((etlstd::is_same<const int* const, etl::types<volatile int* const>::const_pointer_const>::value));

      CHECK((etlstd::is_same<int,              etl::types<const volatile int*>::type>::value));
      CHECK((etlstd::is_same<int&,             etl::types<const volatile int*>::reference>::value));
      CHECK((etlstd::is_same<const int&,       etl::types<const volatile int*>::const_reference>::value));
      CHECK((etlstd::is_same<int*,             etl::types<const volatile int*>::pointer>::value));
      CHECK((etlstd::is_same<const int*,       etl::types<const volatile int*>::const_pointer>::value));
      CHECK((etlstd::is_same<const int* const, etl::types<const volatile int*>::const_pointer_const>::value));

      CHECK((etlstd::is_same<int,              etl::types<const volatile int* const>::type>::value));
      CHECK((etlstd::is_same<int&,             etl::types<const volatile int* const>::reference>::value));
      CHECK((etlstd::is_same<const int&,       etl::types<const volatile int* const>::const_reference>::value));
      CHECK((etlstd::is_same<int*,             etl::types<const volatile int* const>::pointer>::value));
      CHECK((etlstd::is_same<const int*,       etl::types<const volatile int* const>::const_pointer>::value));
      CHECK((etlstd::is_same<const int* const, etl::types<const volatile int* const>::const_pointer_const>::value));

      CHECK((etlstd::is_same<int,              etl::types<volatile int&>::type>::value));
      CHECK((etlstd::is_same<int&,             etl::types<volatile int&>::reference>::value));
      CHECK((etlstd::is_same<const int&,       etl::types<volatile int&>::const_reference>::value));
      CHECK((etlstd::is_same<int*,             etl::types<volatile int&>::pointer>::value));
      CHECK((etlstd::is_same<const int*,       etl::types<volatile int&>::const_pointer>::value));
      CHECK((etlstd::is_same<const int* const, etl::types<volatile int&>::const_pointer_const>::value));

      CHECK((etlstd::is_same<int,              etl::types<const volatile int&>::type>::value));
      CHECK((etlstd::is_same<int&,             etl::types<const volatile int&>::reference>::value));
      CHECK((etlstd::is_same<const int&,       etl::types<const volatile int&>::const_reference>::value));
      CHECK((etlstd::is_same<int*,             etl::types<const volatile int&>::pointer>::value));
      CHECK((etlstd::is_same<const int*,       etl::types<const volatile int&>::const_pointer>::value));
      CHECK((etlstd::is_same<const int* const, etl::types<const volatile int&>::const_pointer_const>::value));
    }
  };

  //*************************************************************************
  TEST(conditional_integral_constant)
  {
    int v1 = etl::conditional_integral_constant<true,  int, 1, 2>::value;
    int v2 = etl::conditional_integral_constant<false, int, 1, 2>::value;

    CHECK_EQUAL(1, v1);
    CHECK_EQUAL(2, v2);
  }

  //*************************************************************************
  TEST(size_of)
  {
    CHECK_EQUAL(1, etl::size_of<void>::value);
    CHECK_EQUAL(1, etl::size_of<char>::value);
    CHECK_EQUAL(2, etl::size_of<short>::value);
    CHECK_EQUAL(4, etl::size_of<int>::value);
    CHECK_EQUAL(20, etl::size_of<TestData>::value);

    CHECK_EQUAL(1, etl::size_of_v<void>);
    CHECK_EQUAL(1, etl::size_of_v<char>);
    CHECK_EQUAL(2, etl::size_of_v<short>);
    CHECK_EQUAL(4, etl::size_of_v<int>);
    CHECK_EQUAL(20, etl::size_of_v<TestData>);
  }

  //*************************************************************************
  TEST(is_convertible)
  {
    CHECK((etlstd::is_convertible<char, int>::value));
    CHECK((etlstd::is_convertible<int,  char>::value));
    CHECK((etlstd::is_convertible<unsigned char, int>::value));
    CHECK((etlstd::is_convertible<int, unsigned char>::value));
    CHECK((etlstd::is_convertible<double, int>::value));
    CHECK((etlstd::is_convertible<int, double>::value));
    CHECK((etlstd::is_convertible<int*,  void*>::value));
    CHECK(!(etlstd::is_convertible<int(*)(), void(*)()>::value));
    CHECK(!(etlstd::is_convertible<int(*)(), int*>::value));
    CHECK(!(etlstd::is_convertible<int*, int(*)()>::value));
    CHECK(!(etlstd::is_convertible<void*, int*>::value));
  }

  //*************************************************************************
  TEST(add_lvalue_reference)
  {
    CHECK(!std::is_lvalue_reference_v<etlstd::add_lvalue_reference<void>::type>);
    CHECK(std::is_lvalue_reference_v<etlstd::add_lvalue_reference<int>::type>);
    CHECK(std::is_lvalue_reference_v<etlstd::add_lvalue_reference<int*>::type>);
    CHECK(std::is_lvalue_reference_v<etlstd::add_lvalue_reference<int&>::type>);
    CHECK(std::is_lvalue_reference_v<etlstd::add_lvalue_reference<int&&>::type>);
  }

  //*************************************************************************
  TEST(add_rvalue_reference)
  {
    CHECK(!std::is_rvalue_reference_v<etlstd::add_rvalue_reference<void>::type>);
    CHECK(std::is_rvalue_reference_v<etlstd::add_rvalue_reference<int>::type>);
    CHECK(std::is_rvalue_reference_v<etlstd::add_rvalue_reference<int*>::type>);
    CHECK(!std::is_rvalue_reference_v<etlstd::add_rvalue_reference<int&>::type>);
    CHECK(std::is_rvalue_reference_v<etlstd::add_rvalue_reference<int&&>::type>);
  }

  //*************************************************************************
  TEST(is_lvalue_reference)
  {
    CHECK_EQUAL(std::is_lvalue_reference_v<void>,  etlstd::is_lvalue_reference_v<void>);
    CHECK_EQUAL(std::is_lvalue_reference_v<int>,   etlstd::is_lvalue_reference_v<int>);
    CHECK_EQUAL(std::is_lvalue_reference_v<int*>,  etlstd::is_lvalue_reference_v<int*>);
    CHECK_EQUAL(std::is_lvalue_reference_v<int&>,  etlstd::is_lvalue_reference_v<int&>);
    CHECK_EQUAL(std::is_lvalue_reference_v<int&&>, etlstd::is_lvalue_reference_v<int&&>);
  }
}
