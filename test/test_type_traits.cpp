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

#include <UnitTest++/UnitTest++.h>

#if defined(ETL_COMPILER_GCC)
namespace std
{
  template <typename T>
  struct add_reference : public std::add_lvalue_reference<T>
  {
  };
}
#endif

#include "../src/type_traits.h"
#include <type_traits>

// A class to test non-fundamental types.
struct Test
{
  int a;
};

namespace
{
  SUITE(test_type_traits)
  {
    //*************************************************************************
    TEST(test_is_integral)
    {
      CHECK(etl::is_integral<bool>::value               == std::is_integral<bool>::value);
      CHECK(etl::is_integral<char>::value               == std::is_integral<char>::value);
      CHECK(etl::is_integral<signed char>::value        == std::is_integral<signed char>::value);
      CHECK(etl::is_integral<unsigned char>::value      == std::is_integral<unsigned char>::value);
      CHECK(etl::is_integral<wchar_t>::value            == std::is_integral<wchar_t>::value);
      CHECK(etl::is_integral<short>::value              == std::is_integral<short>::value);
      CHECK(etl::is_integral<signed short>::value       == std::is_integral<signed short>::value);
      CHECK(etl::is_integral<unsigned short>::value     == std::is_integral<unsigned short>::value);
      CHECK(etl::is_integral<int>::value                == std::is_integral<int>::value);
      CHECK(etl::is_integral<signed int>::value         == std::is_integral<signed int>::value);
      CHECK(etl::is_integral<unsigned int>::value       == std::is_integral<unsigned int>::value);
      CHECK(etl::is_integral<long>::value               == std::is_integral<long>::value);
      CHECK(etl::is_integral<signed long>::value        == std::is_integral<signed long>::value);
      CHECK(etl::is_integral<unsigned long>::value      == std::is_integral<unsigned long>::value);
      CHECK(etl::is_integral<long long>::value          == std::is_integral<long long>::value);
      CHECK(etl::is_integral<signed long long>::value   == std::is_integral<signed long long>::value);
      CHECK(etl::is_integral<unsigned long long>::value == std::is_integral<unsigned long long>::value);
      CHECK(etl::is_integral<const int>::value          == std::is_integral<const int>::value);
      CHECK(etl::is_integral<volatile int>::value       == std::is_integral<volatile int>::value);
      CHECK(etl::is_integral<const int>::value          == std::is_integral<const int>::value);
      CHECK(etl::is_integral<const volatile int>::value == std::is_integral<const volatile int>::value);
      CHECK(etl::is_integral<float>::value              == std::is_integral<float>::value);
      CHECK(etl::is_integral<double>::value             == std::is_integral<double>::value);
      CHECK(etl::is_integral<long double>::value        == std::is_integral<long double>::value);
      CHECK(etl::is_integral<Test>::value               == std::is_integral<Test>::value);
    }

    //*************************************************************************
    TEST(test_is_signed)
    {
      CHECK(etl::is_signed<bool>::value               == std::is_signed<bool>::value);
      CHECK(etl::is_signed<char>::value               == std::is_signed<char>::value);
      CHECK(etl::is_signed<signed char>::value        == std::is_signed<signed char>::value);
      CHECK(etl::is_signed<unsigned char>::value      == std::is_signed<unsigned char>::value);
      CHECK(etl::is_signed<wchar_t>::value            == std::is_signed<wchar_t>::value);
      CHECK(etl::is_signed<short>::value              == std::is_signed<short>::value);
      CHECK(etl::is_signed<signed short>::value       == std::is_signed<signed short>::value);
      CHECK(etl::is_signed<unsigned short>::value     == std::is_signed<unsigned short>::value);
      CHECK(etl::is_signed<int>::value                == std::is_signed<int>::value);
      CHECK(etl::is_signed<signed int>::value         == std::is_signed<signed int>::value);
      CHECK(etl::is_signed<unsigned int>::value       == std::is_signed<unsigned int>::value);
      CHECK(etl::is_signed<long>::value               == std::is_signed<long>::value);
      CHECK(etl::is_signed<signed long>::value        == std::is_signed<signed long>::value);
      CHECK(etl::is_signed<unsigned long>::value      == std::is_signed<unsigned long>::value);
      CHECK(etl::is_signed<long long>::value          == std::is_signed<long long>::value);
      CHECK(etl::is_signed<signed long long>::value   == std::is_signed<signed long long>::value);
      CHECK(etl::is_signed<unsigned long long>::value == std::is_signed<unsigned long long>::value);
      CHECK(etl::is_signed<const int>::value          == std::is_signed<const int>::value);
      CHECK(etl::is_signed<volatile int>::value       == std::is_signed<volatile int>::value);
      CHECK(etl::is_signed<const int>::value          == std::is_signed<const int>::value);
      CHECK(etl::is_signed<const volatile int>::value == std::is_signed<const volatile int>::value);
      CHECK(etl::is_signed<float>::value              == std::is_signed<float>::value);
      CHECK(etl::is_signed<double>::value             == std::is_signed<double>::value);
      CHECK(etl::is_signed<long double>::value        == std::is_signed<long double>::value);
      CHECK(etl::is_signed<Test>::value               == std::is_signed<Test>::value);
    }

    //*************************************************************************
    TEST(test_is_unsigned)
    {
      CHECK(etl::is_unsigned<bool>::value               == std::is_unsigned<bool>::value);
      CHECK(etl::is_unsigned<char>::value               == std::is_unsigned<char>::value);
      CHECK(etl::is_unsigned<signed char>::value        == std::is_unsigned<signed char>::value);
      CHECK(etl::is_unsigned<unsigned char>::value      == std::is_unsigned<unsigned char>::value);
      CHECK(etl::is_unsigned<signed char>::value        == std::is_unsigned<signed char>::value);
      CHECK(etl::is_unsigned<wchar_t>::value            == std::is_unsigned<wchar_t>::value);
      CHECK(etl::is_unsigned<short>::value              == std::is_unsigned<short>::value);
      CHECK(etl::is_unsigned<signed short>::value       == std::is_unsigned<signed short>::value);
      CHECK(etl::is_unsigned<unsigned short>::value     == std::is_unsigned<unsigned short>::value);
      CHECK(etl::is_unsigned<int>::value                == std::is_unsigned<int>::value);
      CHECK(etl::is_unsigned<signed int>::value         == std::is_unsigned<signed int>::value);
      CHECK(etl::is_unsigned<unsigned int>::value       == std::is_unsigned<unsigned int>::value);
      CHECK(etl::is_unsigned<long>::value               == std::is_unsigned<long>::value);
      CHECK(etl::is_unsigned<signed long>::value        == std::is_unsigned<signed long>::value);
      CHECK(etl::is_unsigned<unsigned long>::value      == std::is_unsigned<unsigned long>::value);
      CHECK(etl::is_unsigned<long long>::value          == std::is_unsigned<long long>::value);
      CHECK(etl::is_unsigned<signed long long>::value   == std::is_unsigned<signed long long>::value);
      CHECK(etl::is_unsigned<unsigned long long>::value == std::is_unsigned<unsigned long long>::value);
      CHECK(etl::is_unsigned<const int>::value          == std::is_unsigned<const int>::value);
      CHECK(etl::is_unsigned<volatile int>::value       == std::is_unsigned<volatile int>::value);
      CHECK(etl::is_unsigned<const int>::value          == std::is_unsigned<const int>::value);
      CHECK(etl::is_unsigned<const volatile int>::value == std::is_unsigned<const volatile int>::value);
      CHECK(etl::is_unsigned<float>::value              == std::is_unsigned<float>::value);
      CHECK(etl::is_unsigned<double>::value             == std::is_unsigned<double>::value);
      CHECK(etl::is_unsigned<long double>::value        == std::is_unsigned<long double>::value);
      CHECK(etl::is_unsigned<Test>::value               == std::is_unsigned<Test>::value);
    }

    //*************************************************************************
    TEST(test_is_floating_point)
    {
      CHECK(etl::is_floating_point<bool>::value               == std::is_floating_point<bool>::value);
      CHECK(etl::is_floating_point<char>::value               == std::is_floating_point<char>::value);
      CHECK(etl::is_floating_point<signed char>::value        == std::is_floating_point<signed char>::value);
      CHECK(etl::is_floating_point<unsigned char>::value      == std::is_floating_point<unsigned char>::value);
      CHECK(etl::is_floating_point<wchar_t>::value            == std::is_floating_point<wchar_t>::value);
      CHECK(etl::is_floating_point<short>::value              == std::is_floating_point<short>::value);
      CHECK(etl::is_floating_point<signed short>::value       == std::is_floating_point<signed short>::value);
      CHECK(etl::is_floating_point<unsigned short>::value     == std::is_floating_point<unsigned short>::value);
      CHECK(etl::is_floating_point<int>::value                == std::is_floating_point<int>::value);
      CHECK(etl::is_floating_point<signed int>::value         == std::is_floating_point<signed int>::value);
      CHECK(etl::is_floating_point<unsigned int>::value       == std::is_floating_point<unsigned int>::value);
      CHECK(etl::is_floating_point<long>::value               == std::is_floating_point<long>::value);
      CHECK(etl::is_floating_point<signed long>::value        == std::is_floating_point<signed long>::value);
      CHECK(etl::is_floating_point<unsigned long>::value      == std::is_floating_point<unsigned long>::value);
      CHECK(etl::is_floating_point<long long>::value          == std::is_floating_point<long long>::value);
      CHECK(etl::is_floating_point<signed long long>::value   == std::is_floating_point<signed long long>::value);
      CHECK(etl::is_floating_point<unsigned long long>::value == std::is_floating_point<unsigned long long>::value);
      CHECK(etl::is_floating_point<const int>::value          == std::is_floating_point<const int>::value);
      CHECK(etl::is_floating_point<volatile int>::value       == std::is_floating_point<volatile int>::value);
      CHECK(etl::is_floating_point<const int>::value          == std::is_floating_point<const int>::value);
      CHECK(etl::is_floating_point<const volatile int>::value == std::is_floating_point<const volatile int>::value);
      CHECK(etl::is_floating_point<float>::value              == std::is_floating_point<float>::value);
      CHECK(etl::is_floating_point<double>::value             == std::is_floating_point<double>::value);
      CHECK(etl::is_floating_point<long double>::value        == std::is_floating_point<long double>::value);
      CHECK(etl::is_floating_point<Test>::value               == std::is_signed<Test>::value);
    }

    //*************************************************************************
    TEST(test_is_pointer)
    {
      CHECK(etl::is_pointer<int>::value                 == std::is_pointer<int>::value);
      CHECK(etl::is_pointer<int*>::value                == std::is_pointer<int*>::value);
      CHECK(etl::is_pointer<const int*>::value          == std::is_pointer<const int*>::value);
      CHECK(etl::is_pointer<volatile int*>::value       == std::is_pointer<volatile int*>::value);
      CHECK(etl::is_pointer<const volatile int*>::value == std::is_pointer<const volatile int*>::value);
    }

    //*************************************************************************
    TEST(test_is_reference)
    {
      CHECK(etl::is_reference<int>::value                 == std::is_reference<int>::value);
      CHECK(etl::is_reference<int&>::value                == std::is_reference<int&>::value);
      CHECK(etl::is_reference<const int&>::value          == std::is_reference<const int&>::value);
      CHECK(etl::is_reference<volatile int&>::value       == std::is_reference<volatile int&>::value);
      CHECK(etl::is_reference<const volatile int&>::value == std::is_reference<const volatile int&>::value);
    }

    //*************************************************************************
    TEST(test_is_same)
    {
      CHECK((etl::is_same<int, int>::value  == std::is_same<int, int>::value));
      CHECK((etl::is_same<char, int>::value == std::is_same<char, int>::value));
    }

    //*************************************************************************
    TEST(test_is_array)
    {
      CHECK(etl::is_array<int>::value     == std::is_array<int>::value);
      CHECK(etl::is_array<int[10]>::value == std::is_array<int[10]>::value);
    }

    //*************************************************************************
    TEST(test_remove_pointer)
    {
      CHECK((std::is_same<etl::remove_pointer<int>::type,                 std::remove_pointer<int>::type>::value));
      CHECK((std::is_same<etl::remove_pointer<int*>::type, std::remove_pointer<int*>::type>::value));
      CHECK((std::is_same<etl::remove_pointer<const int*>::type, std::remove_pointer<const int*>::type>::value));
      CHECK((std::is_same<etl::remove_pointer<volatile int*>::type, std::remove_pointer<volatile int*>::type>::value));
      CHECK((std::is_same<etl::remove_pointer<const volatile int*>::type, std::remove_pointer<const volatile int*>::type>::value));
    }

    //*************************************************************************
    TEST(test_add_pointer)
    {
      CHECK((std::is_same<etl::add_pointer<int>::type,                 std::add_pointer<int>::type>::value));
      CHECK((std::is_same<etl::add_pointer<int*>::type,                std::add_pointer<int*>::type>::value));
      CHECK((std::is_same<etl::add_pointer<const int*>::type,          std::add_pointer<const int*>::type>::value));
      CHECK((std::is_same<etl::add_pointer<volatile int*>::type,       std::add_pointer<volatile int*>::type>::value));
      CHECK((std::is_same<etl::add_pointer<const volatile int*>::type, std::add_pointer<const volatile int*>::type>::value));
    }

    //*************************************************************************
    TEST(test_remove_reference)
    {
      CHECK((std::is_same<etl::remove_reference<int>::type,                 std::remove_reference<int>::type>::value));
      CHECK((std::is_same<etl::remove_reference<int&>::type,                std::remove_reference<int&>::type>::value));
      CHECK((std::is_same<etl::remove_reference<const int&>::type,          std::remove_reference<const int&>::type>::value));
      CHECK((std::is_same<etl::remove_reference<volatile int&>::type,       std::remove_reference<volatile int&>::type>::value));
      CHECK((std::is_same<etl::remove_reference<const volatile int&>::type, std::remove_reference<const volatile int&>::type>::value));
    }

    //*************************************************************************
    TEST(test_add_reference)
    {
      CHECK((std::is_same<etl::add_reference<int>::type,                 std::add_lvalue_reference<int>::type>::value));
      CHECK((std::is_same<etl::add_reference<int&>::type,                std::add_lvalue_reference<int&>::type>::value));
      CHECK((std::is_same<etl::add_reference<const int&>::type,          std::add_lvalue_reference<const int&>::type>::value));
      CHECK((std::is_same<etl::add_reference<volatile int&>::type,       std::add_lvalue_reference<volatile int&>::type>::value));
      CHECK((std::is_same<etl::add_reference<const volatile int&>::type, std::add_lvalue_reference<const volatile int&>::type>::value));

    }

    //*************************************************************************
    TEST(test_remove_const)
    {
      CHECK((std::is_same<etl::remove_const<int>::type,                std::remove_const<int>::type>::value));
      CHECK((std::is_same<etl::remove_const<const int>::type,          std::remove_const<const int>::type>::value));
      CHECK((std::is_same<etl::remove_const<const volatile int>::type, std::remove_const<const volatile int>::type>::value));
    }

    //*************************************************************************
    TEST(test_add_const)
    {
      CHECK((std::is_same<etl::add_const<int>::type,                std::add_const<int>::type>::value));
      CHECK((std::is_same<etl::add_const<const int>::type,          std::add_const<const int>::type>::value));
      CHECK((std::is_same<etl::add_const<const volatile int>::type, std::add_const<const volatile int>::type>::value));
    }

    //*************************************************************************
    TEST(test_is_const)
    {
      CHECK(etl::is_const<int>::value                == std::is_const<int>::value);
      CHECK(etl::is_const<const int>::value          == std::is_const<const int>::value);
      CHECK(etl::is_const<const volatile int>::value == std::is_const<const volatile int>::value);
    }

    //*************************************************************************
    TEST(test_is_volatile)
    {
      CHECK(etl::is_volatile<int>::value                == std::is_volatile<int>::value);
      CHECK(etl::is_volatile<volatile int>::value       == std::is_volatile<volatile int>::value);
      CHECK(etl::is_volatile<const volatile int>::value == std::is_volatile<const volatile int>::value);
    }

    //*************************************************************************
    TEST(test_remove_volatile)
    {
      CHECK((std::is_same<etl::remove_volatile<int>::type,                std::remove_volatile<int>::type>::value));
      CHECK((std::is_same<etl::remove_volatile<volatile int>::type,       std::remove_volatile<volatile int>::type>::value));
      CHECK((std::is_same<etl::remove_volatile<const volatile int>::type, std::remove_volatile<const volatile int>::type>::value));
    }

    //*************************************************************************
    TEST(test_add_volatile)
    {
      CHECK((std::is_same<etl::add_volatile<int>::type,                std::add_volatile<int>::type>::value));
      CHECK((std::is_same<etl::add_volatile<volatile int>::type,       std::add_volatile<volatile int>::type>::value));
      CHECK((std::is_same<etl::add_volatile<const volatile int>::type, std::add_volatile<const volatile int>::type>::value));
    }

    //*************************************************************************
    TEST(test_remove_cv)
    {
      CHECK((std::is_same<etl::remove_cv<int>::type,                std::remove_cv<int>::type>::value));
      CHECK((std::is_same<etl::remove_cv<const int>::type,          std::remove_cv<const int>::type>::value));
      CHECK((std::is_same<etl::remove_cv<volatile int>::type,       std::remove_cv<volatile int>::type>::value));
      CHECK((std::is_same<etl::remove_cv<const volatile int>::type, std::remove_cv<const volatile int>::type>::value));
    }

    //*************************************************************************
    TEST(test_add_cv)
    {

      typedef etl::add_cv<int>::type t1;
      typedef std::add_cv<int>::type t2;

      bool pass = std::is_same<t1, t2>::value;
        //std::is_same<etl::add_cv<int>::type,                std::add_cv<int>::type>::value;

      CHECK(pass);
      CHECK((std::is_same<etl::add_cv<const int>::type,          std::add_cv<const int>::type>::value));
      CHECK((std::is_same<etl::add_cv<volatile int>::type,       std::add_cv<volatile int>::type>::value));
      CHECK((std::is_same<etl::add_cv<const volatile int>::type, std::add_cv<const volatile int>::type>::value));
    }

    //*************************************************************************
    TEST(test_is_arithmetic)
    {
      CHECK(etl::is_arithmetic<bool>::value               == std::is_arithmetic<bool>::value);
      CHECK(etl::is_arithmetic<char>::value               == std::is_arithmetic<char>::value);
      CHECK(etl::is_arithmetic<signed char>::value        == std::is_arithmetic<signed char>::value);
      CHECK(etl::is_arithmetic<unsigned char>::value      == std::is_arithmetic<unsigned char>::value);
      CHECK(etl::is_arithmetic<wchar_t>::value            == std::is_arithmetic<wchar_t>::value);
      CHECK(etl::is_arithmetic<short>::value              == std::is_arithmetic<short>::value);
      CHECK(etl::is_arithmetic<signed short>::value       == std::is_arithmetic<signed short>::value);
      CHECK(etl::is_arithmetic<unsigned short>::value     == std::is_arithmetic<unsigned short>::value);
      CHECK(etl::is_arithmetic<int>::value                == std::is_arithmetic<int>::value);
      CHECK(etl::is_arithmetic<signed int>::value         == std::is_arithmetic<signed int>::value);
      CHECK(etl::is_arithmetic<unsigned int>::value       == std::is_arithmetic<unsigned int>::value);
      CHECK(etl::is_arithmetic<long>::value               == std::is_arithmetic<long>::value);
      CHECK(etl::is_arithmetic<signed long>::value        == std::is_arithmetic<signed long>::value);
      CHECK(etl::is_arithmetic<unsigned long>::value      == std::is_arithmetic<unsigned long>::value);
      CHECK(etl::is_arithmetic<long long>::value          == std::is_arithmetic<long long>::value);
      CHECK(etl::is_arithmetic<signed long long>::value   == std::is_arithmetic<signed long long>::value);
      CHECK(etl::is_arithmetic<unsigned long long>::value == std::is_arithmetic<unsigned long long>::value);
      CHECK(etl::is_arithmetic<const int>::value          == std::is_arithmetic<const int>::value);
      CHECK(etl::is_arithmetic<volatile int>::value       == std::is_arithmetic<volatile int>::value);
      CHECK(etl::is_arithmetic<const int>::value          == std::is_arithmetic<const int>::value);
      CHECK(etl::is_arithmetic<const volatile int>::value == std::is_arithmetic<const volatile int>::value);
      CHECK(etl::is_arithmetic<float>::value              == std::is_arithmetic<float>::value);
      CHECK(etl::is_arithmetic<double>::value             == std::is_arithmetic<double>::value);
      CHECK(etl::is_arithmetic<long double>::value        == std::is_arithmetic<long double>::value);
      CHECK(etl::is_arithmetic<Test>::value               == std::is_arithmetic<Test>::value);
    }

    //*************************************************************************
    TEST(test_is_fundamental)
    {
      CHECK(etl::is_fundamental<void>::value               == std::is_fundamental<void>::value);
      CHECK(etl::is_fundamental<bool>::value               == std::is_fundamental<bool>::value);
      CHECK(etl::is_fundamental<char>::value               == std::is_fundamental<char>::value);
      CHECK(etl::is_fundamental<signed char>::value        == std::is_fundamental<signed char>::value);
      CHECK(etl::is_fundamental<unsigned char>::value      == std::is_fundamental<unsigned char>::value);
      CHECK(etl::is_fundamental<wchar_t>::value            == std::is_fundamental<wchar_t>::value);
      CHECK(etl::is_fundamental<short>::value              == std::is_fundamental<short>::value);
      CHECK(etl::is_fundamental<signed short>::value       == std::is_fundamental<signed short>::value);
      CHECK(etl::is_fundamental<unsigned short>::value     == std::is_fundamental<unsigned short>::value);
      CHECK(etl::is_fundamental<int>::value                == std::is_fundamental<int>::value);
      CHECK(etl::is_fundamental<signed int>::value         == std::is_fundamental<signed int>::value);
      CHECK(etl::is_fundamental<unsigned int>::value       == std::is_fundamental<unsigned int>::value);
      CHECK(etl::is_fundamental<long>::value               == std::is_fundamental<long>::value);
      CHECK(etl::is_fundamental<signed long>::value        == std::is_fundamental<signed long>::value);
      CHECK(etl::is_fundamental<unsigned long>::value      == std::is_fundamental<unsigned long>::value);
      CHECK(etl::is_fundamental<long long>::value          == std::is_fundamental<long long>::value);
      CHECK(etl::is_fundamental<signed long long>::value   == std::is_fundamental<signed long long>::value);
      CHECK(etl::is_fundamental<unsigned long long>::value == std::is_fundamental<unsigned long long>::value);
      CHECK(etl::is_fundamental<const int>::value          == std::is_fundamental<const int>::value);
      CHECK(etl::is_fundamental<volatile int>::value       == std::is_fundamental<volatile int>::value);
      CHECK(etl::is_fundamental<const int>::value          == std::is_fundamental<const int>::value);
      CHECK(etl::is_fundamental<const volatile int>::value == std::is_fundamental<const volatile int>::value);
      CHECK(etl::is_fundamental<float>::value              == std::is_fundamental<float>::value);
      CHECK(etl::is_fundamental<double>::value             == std::is_fundamental<double>::value);
      CHECK(etl::is_fundamental<long double>::value        == std::is_fundamental<long double>::value);
      CHECK(etl::is_fundamental<Test>::value               == std::is_fundamental<Test>::value);
    }

    //*************************************************************************
    TEST(test_is_compound)
    {
      CHECK(etl::is_compound<void>::value               == std::is_compound<void>::value);
      CHECK(etl::is_compound<bool>::value               == std::is_compound<bool>::value);
      CHECK(etl::is_compound<char>::value               == std::is_compound<char>::value);
      CHECK(etl::is_compound<signed char>::value        == std::is_compound<signed char>::value);
      CHECK(etl::is_compound<unsigned char>::value      == std::is_compound<unsigned char>::value);
      CHECK(etl::is_compound<wchar_t>::value            == std::is_compound<wchar_t>::value);
      CHECK(etl::is_compound<short>::value              == std::is_compound<short>::value);
      CHECK(etl::is_compound<signed short>::value       == std::is_compound<signed short>::value);
      CHECK(etl::is_compound<unsigned short>::value     == std::is_compound<unsigned short>::value);
      CHECK(etl::is_compound<int>::value                == std::is_compound<int>::value);
      CHECK(etl::is_compound<signed int>::value         == std::is_compound<signed int>::value);
      CHECK(etl::is_compound<unsigned int>::value       == std::is_compound<unsigned int>::value);
      CHECK(etl::is_compound<long>::value               == std::is_compound<long>::value);
      CHECK(etl::is_compound<signed long>::value        == std::is_compound<signed long>::value);
      CHECK(etl::is_compound<unsigned long>::value      == std::is_compound<unsigned long>::value);
      CHECK(etl::is_compound<long long>::value          == std::is_compound<long long>::value);
      CHECK(etl::is_compound<signed long long>::value   == std::is_compound<signed long long>::value);
      CHECK(etl::is_compound<unsigned long long>::value == std::is_compound<unsigned long long>::value);
      CHECK(etl::is_compound<const int>::value          == std::is_compound<const int>::value);
      CHECK(etl::is_compound<volatile int>::value       == std::is_compound<volatile int>::value);
      CHECK(etl::is_compound<const int>::value          == std::is_compound<const int>::value);
      CHECK(etl::is_compound<const volatile int>::value == std::is_compound<const volatile int>::value);
      CHECK(etl::is_compound<float>::value              == std::is_compound<float>::value);
      CHECK(etl::is_compound<double>::value             == std::is_compound<double>::value);
      CHECK(etl::is_compound<long double>::value        == std::is_compound<long double>::value);
      CHECK(etl::is_compound<Test>::value               == std::is_compound<Test>::value);
    }

    //*************************************************************************
    TEST(test_is_void)
    {
      CHECK(etl::is_void<int>::value  == std::is_void<int>::value);
      CHECK(etl::is_void<void>::value == std::is_void<void>::value);
    }

    //*************************************************************************
    TEST(test_make_signed)
    {
      CHECK((std::is_same<etl::make_signed<char>::type,               std::make_signed<char>::type>::value));
      CHECK((std::is_same<etl::make_signed<signed char>::type,        std::make_signed<signed char>::type>::value));
      CHECK((std::is_same<etl::make_signed<unsigned char>::type,      std::make_signed<unsigned char>::type>::value));
      CHECK(std::is_signed<etl::make_signed<wchar_t>::type>::value && (sizeof(wchar_t) == sizeof(etl::make_signed<wchar_t>::type)));
      CHECK((std::is_same<etl::make_signed<short>::type,              std::make_signed<short>::type>::value));
      CHECK((std::is_same<etl::make_signed<signed short>::type,       std::make_signed<signed short>::type>::value));
      CHECK((std::is_same<etl::make_signed<unsigned short>::type,     std::make_signed<unsigned short>::type>::value));
      CHECK((std::is_same<etl::make_signed<int>::type,                std::make_signed<int>::type>::value));
      CHECK((std::is_same<etl::make_signed<signed int>::type,         std::make_signed<signed int>::type>::value));
      CHECK((std::is_same<etl::make_signed<unsigned int>::type,       std::make_signed<unsigned int>::type>::value));
      CHECK((std::is_same<etl::make_signed<long>::type,               std::make_signed<long>::type>::value));
      CHECK((std::is_same<etl::make_signed<signed long>::type,        std::make_signed<signed long>::type>::value));
      CHECK((std::is_same<etl::make_signed<unsigned long>::type,      std::make_signed<unsigned long>::type>::value));
      CHECK((std::is_same<etl::make_signed<long long>::type,          std::make_signed<long long>::type>::value));
      CHECK((std::is_same<etl::make_signed<signed long long>::type,   std::make_signed<signed long long>::type>::value));
      CHECK((std::is_same<etl::make_signed<unsigned long long>::type, std::make_signed<unsigned long long>::type>::value));
      CHECK((std::is_same<etl::make_signed<const int>::type,          std::make_signed<const int>::type>::value));
      CHECK((std::is_same<etl::make_signed<volatile int>::type,       std::make_signed<volatile int>::type>::value));
      CHECK((std::is_same<etl::make_signed<const int>::type,          std::make_signed<const int>::type>::value));
      CHECK((std::is_same<etl::make_signed<const volatile int>::type, std::make_signed<const volatile int>::type>::value));
    }

    //*************************************************************************
    TEST(test_make_unsigned)
    {
      CHECK((std::is_same<etl::make_unsigned<char>::type,               std::make_unsigned<char>::type>::value));
      CHECK((std::is_same<etl::make_unsigned<signed char>::type,        std::make_unsigned<signed char>::type>::value));
      CHECK((std::is_same<etl::make_unsigned<unsigned char>::type,      std::make_unsigned<unsigned char>::type>::value));
      CHECK(std::is_unsigned<etl::make_unsigned<wchar_t>::type>::value && (sizeof(wchar_t) == sizeof(etl::make_unsigned<wchar_t>::type)));
      CHECK((std::is_same<etl::make_unsigned<short>::type,              std::make_unsigned<short>::type>::value));
      CHECK((std::is_same<etl::make_unsigned<signed short>::type,       std::make_unsigned<signed short>::type>::value));
      CHECK((std::is_same<etl::make_unsigned<unsigned short>::type,     std::make_unsigned<unsigned short>::type>::value));
      CHECK((std::is_same<etl::make_unsigned<int>::type,                std::make_unsigned<int>::type>::value));
      CHECK((std::is_same<etl::make_unsigned<signed int>::type,         std::make_unsigned<signed int>::type>::value));
      CHECK((std::is_same<etl::make_unsigned<unsigned int>::type,       std::make_unsigned<unsigned int>::type>::value));
      CHECK((std::is_same<etl::make_unsigned<long>::type,               std::make_unsigned<long>::type>::value));
      CHECK((std::is_same<etl::make_unsigned<signed long>::type,        std::make_unsigned<signed long>::type>::value));
      CHECK((std::is_same<etl::make_unsigned<unsigned long>::type,      std::make_unsigned<unsigned long>::type>::value));
      CHECK((std::is_same<etl::make_unsigned<long long>::type,          std::make_unsigned<long long>::type>::value));
      CHECK((std::is_same<etl::make_unsigned<signed long long>::type,   std::make_unsigned<signed long long>::type>::value));
      CHECK((std::is_same<etl::make_unsigned<unsigned long long>::type, std::make_unsigned<unsigned long long>::type>::value));
      CHECK((std::is_same<etl::make_unsigned<const int>::type,          std::make_unsigned<const int>::type>::value));
      CHECK((std::is_same<etl::make_unsigned<volatile int>::type,       std::make_unsigned<volatile int>::type>::value));
      CHECK((std::is_same<etl::make_unsigned<const int>::type,          std::make_unsigned<const int>::type>::value));
      CHECK((std::is_same<etl::make_unsigned<const volatile int>::type, std::make_unsigned<const volatile int>::type>::value));
    }

    //*************************************************************************
    TEST(test_extent)
    {
      CHECK(std::extent<int>::value     == etl::extent<int>::value);
      CHECK(std::extent<int[]>::value   == etl::extent<int[]>::value);
      CHECK(std::extent<int[10]>::value == etl::extent<int[10]>::value);
    }

    //*************************************************************************
    TEST(test_remove_extent)
    {
      CHECK((std::is_same<etl::remove_extent<int>::type,     std::remove_extent<int>::type>::value));
      CHECK((std::is_same<etl::remove_extent<int[]>::type,   std::remove_extent<int[]>::type>::value));
      CHECK((std::is_same<etl::remove_extent<int[10]>::type, std::remove_extent<int[10]>::type>::value));
    }

    //*************************************************************************
    TEST(test_remove_all_extents)
    {
      CHECK((std::is_same<etl::remove_all_extents<int>::type,         std::remove_all_extents<int>::type>::value));
      CHECK((std::is_same<etl::remove_all_extents<int[10]>::type,     std::remove_all_extents<int[10]>::type>::value));
      CHECK((std::is_same<etl::remove_all_extents<int[10][10]>::type, std::remove_all_extents<int[10][10]>::type>::value));
    }

    //*************************************************************************
    TEST(test_rank)
    {
      CHECK(etl::rank<int>::value         == std::rank<int>::value);
      CHECK(etl::rank<int[10]>::value     == std::rank<int[10]>::value);
      CHECK(etl::rank<int[10][10]>::value == std::rank<int[10][10]>::value);
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

      CHECK(std::alignment_of<char>::value               == etl::alignment_of<char>::value);
      CHECK(std::alignment_of<unsigned char>::value      == etl::alignment_of<unsigned char>::value);
      CHECK(std::alignment_of<short>::value              == etl::alignment_of<short>::value);
      CHECK(std::alignment_of<unsigned short>::value     == etl::alignment_of<unsigned short>::value);
      CHECK(std::alignment_of<int>::value                == etl::alignment_of<int>::value);
      CHECK(std::alignment_of<unsigned int>::value       == etl::alignment_of<unsigned int>::value);
      CHECK(std::alignment_of<long>::value               == etl::alignment_of<long>::value);
      CHECK(std::alignment_of<unsigned long>::value      == etl::alignment_of<unsigned long>::value);
      CHECK(std::alignment_of<long long>::value          == etl::alignment_of<long long>::value);
      CHECK(std::alignment_of<unsigned long long>::value == etl::alignment_of<unsigned long long>::value);
      CHECK(std::alignment_of<float>::value              == etl::alignment_of<float>::value);
      CHECK(std::alignment_of<double>::value             == etl::alignment_of<double>::value);
      CHECK(std::alignment_of<Test>::value               == etl::alignment_of<Test>::value);
    }
  };
}
