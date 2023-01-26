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

#include "etl/type_traits.h"
#include <type_traits>

namespace
{
  struct TestData { };
}

namespace etl
{
  template <>
  struct size_of<TestData> : integral_constant<size_t, 20U> {};
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

  // Unscoped enum
  enum Enum
  {
  };

  // Scoped enum
  enum class EnumClass
  {
  };

  // Class which can be implicitly converted to/from any default-constructable type
  struct ToAny 
  {
    ToAny() = default;
    template <typename T> ToAny(T){};
    template <typename T> operator T() { return T(); }
  };

  // Can't be default constructed
  struct NotDefaultConstructable 
  {
    NotDefaultConstructable() = delete;
  };

  //*********************************************
  struct Copyable
  {
    Copyable() {}
    Copyable(const Copyable&) noexcept {}
    Copyable& operator =(const Copyable&) noexcept { return *this; }

    Copyable(Copyable&&) = delete;
    Copyable& operator =(Copyable&) = delete;
  };

  //*********************************************
  struct Moveable
  {
    Moveable() {}
    Moveable(Moveable&&) noexcept { }
    Moveable& operator =(Moveable&&) noexcept { return *this; }

    Moveable(const Moveable&) = delete;
    Moveable& operator =(const Moveable&) = delete;
  };

  //*********************************************
  struct MoveableCopyable
  {
    MoveableCopyable() {}
    MoveableCopyable(MoveableCopyable&&) noexcept {}
    MoveableCopyable& operator =(MoveableCopyable&&) noexcept { return *this; }
    MoveableCopyable(const MoveableCopyable&) {}
    MoveableCopyable& operator =(const MoveableCopyable&) { return *this; }
  };
}

// Definitions for when the STL and compiler built-ins are not available.
#if ETL_NOT_USING_STL && !defined(ETL_USE_TYPE_TRAITS_BUILTINS)

using etl::is_assignable;
using etl::is_constructible;
using etl::is_copy_constructible;
using etl::is_move_constructible;

//*************************
template <>
struct etl::is_assignable<Copyable, Copyable> : public etl::true_type
{
};

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
struct etl::is_assignable<Moveable, Moveable> : public etl::true_type
{
};

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
struct etl::is_assignable<MoveableCopyable, MoveableCopyable> : public etl::true_type
{
};

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
      CHECK(etl::is_reference<int&&>::value                == std::is_reference<int&&>::value);
      CHECK(etl::is_reference<const int&&>::value          == std::is_reference<const int&&>::value);
      CHECK(etl::is_reference<volatile int&&>::value       == std::is_reference<volatile int&&>::value);
      CHECK(etl::is_reference<const volatile int&&>::value == std::is_reference<const volatile int&&>::value);
    }

    //*************************************************************************
    TEST(test_is_same)
    {
      CHECK((etl::is_same<int, int>::value  == etl::is_same<int, int>::value));
      CHECK((etl::is_same<char, int>::value == etl::is_same<char, int>::value));
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
      CHECK((etl::is_same<etl::remove_pointer<int>::type,                       std::remove_pointer<int>::type>::value));
      CHECK((etl::is_same<etl::remove_pointer<const int>::type,                 std::remove_pointer<const int>::type>::value));
      CHECK((etl::is_same<etl::remove_pointer<int*>::type,                      std::remove_pointer<int*>::type>::value));
      CHECK((etl::is_same<etl::remove_pointer<int* const>::type,                std::remove_pointer<int*>::type>::value));
      CHECK((etl::is_same<etl::remove_pointer<const int*>::type,                std::remove_pointer<const int*>::type>::value));
      CHECK((etl::is_same<etl::remove_pointer<const int* const>::type,          std::remove_pointer<const int* const>::type>::value));

      CHECK((etl::is_same<etl::remove_pointer<volatile int>::type,              std::remove_pointer<volatile int>::type>::value));
      CHECK((etl::is_same<etl::remove_pointer<volatile const int>::type,        std::remove_pointer<volatile const int>::type>::value));
      CHECK((etl::is_same<etl::remove_pointer<volatile int*>::type,             std::remove_pointer<volatile int*>::type>::value));
      CHECK((etl::is_same<etl::remove_pointer<volatile int*const >::type,       std::remove_pointer<volatile int*>::type>::value));
      CHECK((etl::is_same<etl::remove_pointer<const volatile int*>::type,       std::remove_pointer<const volatile int*>::type>::value));
      CHECK((etl::is_same<etl::remove_pointer<const volatile int* const>::type, std::remove_pointer<const volatile int* const>::type>::value));
    }

    //*************************************************************************
    TEST(test_add_pointer)
    {
      CHECK((etl::is_same<etl::add_pointer<int>::type,                       std::add_pointer<int>::type>::value));
      CHECK((etl::is_same<etl::add_pointer<const int>::type,                 std::add_pointer<const int>::type>::value));
      CHECK((etl::is_same<etl::add_pointer<int*>::type,                      std::add_pointer<int*>::type>::value));
      CHECK((etl::is_same<etl::add_pointer<int* const >::type,               std::add_pointer<int* const>::type>::value));
      CHECK((etl::is_same<etl::add_pointer<const int*>::type,                std::add_pointer<const int*>::type>::value));
      CHECK((etl::is_same<etl::add_pointer<const int* const>::type,          std::add_pointer<const int* const>::type>::value));
      CHECK((etl::is_same<etl::add_pointer<volatile int*>::type,             std::add_pointer<volatile int*>::type>::value));
      CHECK((etl::is_same<etl::add_pointer<const volatile int*>::type,       std::add_pointer<const volatile int*>::type>::value));
      CHECK((etl::is_same<etl::add_pointer<volatile int* const>::type,       std::add_pointer<volatile int* const>::type>::value));
      CHECK((etl::is_same<etl::add_pointer<const volatile int* const>::type, std::add_pointer<const volatile int* const>::type>::value));

    }

    //*************************************************************************
    TEST(test_remove_reference)
    {
      CHECK((etl::is_same<etl::remove_reference<int>::type,                 std::remove_reference<int>::type>::value));
      CHECK((etl::is_same<etl::remove_reference<int&>::type,                std::remove_reference<int&>::type>::value));
      CHECK((etl::is_same<etl::remove_reference<const int&>::type,          std::remove_reference<const int&>::type>::value));
      CHECK((etl::is_same<etl::remove_reference<volatile int&>::type,       std::remove_reference<volatile int&>::type>::value));
      CHECK((etl::is_same<etl::remove_reference<const volatile int&>::type, std::remove_reference<const volatile int&>::type>::value));
    }

    //*************************************************************************
    TEST(test_remove_const)
    {
      CHECK((etl::is_same<etl::remove_const<int>::type,                std::remove_const<int>::type>::value));
      CHECK((etl::is_same<etl::remove_const<const int>::type,          std::remove_const<const int>::type>::value));
      CHECK((etl::is_same<etl::remove_const<const volatile int>::type, std::remove_const<const volatile int>::type>::value));
    }

    //*************************************************************************
    TEST(test_add_const)
    {
      CHECK((etl::is_same<etl::add_const<int>::type,                std::add_const<int>::type>::value));
      CHECK((etl::is_same<etl::add_const<const int>::type,          std::add_const<const int>::type>::value));
      CHECK((etl::is_same<etl::add_const<const volatile int>::type, std::add_const<const volatile int>::type>::value));
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
      CHECK((etl::is_same<etl::remove_volatile<int>::type,                std::remove_volatile<int>::type>::value));
      CHECK((etl::is_same<etl::remove_volatile<volatile int>::type,       std::remove_volatile<volatile int>::type>::value));
      CHECK((etl::is_same<etl::remove_volatile<const volatile int>::type, std::remove_volatile<const volatile int>::type>::value));
    }

    //*************************************************************************
    TEST(test_add_volatile)
    {
      CHECK((etl::is_same<etl::add_volatile<int>::type,                std::add_volatile<int>::type>::value));
      CHECK((etl::is_same<etl::add_volatile<volatile int>::type,       std::add_volatile<volatile int>::type>::value));
      CHECK((etl::is_same<etl::add_volatile<const volatile int>::type, std::add_volatile<const volatile int>::type>::value));
    }

    //*************************************************************************
    TEST(test_remove_cv)
    {
      CHECK((etl::is_same<etl::remove_cv<int>::type,                std::remove_cv<int>::type>::value));
      CHECK((etl::is_same<etl::remove_cv<const int>::type,          std::remove_cv<const int>::type>::value));
      CHECK((etl::is_same<etl::remove_cv<volatile int>::type,       std::remove_cv<volatile int>::type>::value));
      CHECK((etl::is_same<etl::remove_cv<const volatile int>::type, std::remove_cv<const volatile int>::type>::value));
    }

    //*************************************************************************
    TEST(test_add_cv)
    {

      typedef etl::add_cv<int>::type t1;
      typedef std::add_cv<int>::type t2;

      bool pass = etl::is_same<t1, t2>::value;
        //etl::is_same<etl::add_cv<int>::type,                std::add_cv<int>::type>::value;

      CHECK(pass);
      CHECK((etl::is_same<etl::add_cv<const int>::type,          std::add_cv<const int>::type>::value));
      CHECK((etl::is_same<etl::add_cv<volatile int>::type,       std::add_cv<volatile int>::type>::value));
      CHECK((etl::is_same<etl::add_cv<const volatile int>::type, std::add_cv<const volatile int>::type>::value));
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
#if ETL_USING_STL
      CHECK(etl::is_fundamental<std::nullptr_t>::value     == std::is_fundamental<std::nullptr_t>::value);
#endif
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
      CHECK((etl::is_same<etl::make_signed<char>::type,               std::make_signed<char>::type>::value));
      CHECK((etl::is_same<etl::make_signed<signed char>::type,        std::make_signed<signed char>::type>::value));
      CHECK((etl::is_same<etl::make_signed<unsigned char>::type,      std::make_signed<unsigned char>::type>::value));
      CHECK(etl::is_signed<etl::make_signed<wchar_t>::type>::value && (sizeof(wchar_t) == sizeof(etl::make_signed<wchar_t>::type)));
      CHECK((etl::is_same<etl::make_signed<short>::type,              std::make_signed<short>::type>::value));
      CHECK((etl::is_same<etl::make_signed<signed short>::type,       std::make_signed<signed short>::type>::value));
      CHECK((etl::is_same<etl::make_signed<unsigned short>::type,     std::make_signed<unsigned short>::type>::value));
      CHECK((etl::is_same<etl::make_signed<int>::type,                std::make_signed<int>::type>::value));
      CHECK((etl::is_same<etl::make_signed<signed int>::type,         std::make_signed<signed int>::type>::value));
      CHECK((etl::is_same<etl::make_signed<unsigned int>::type,       std::make_signed<unsigned int>::type>::value));
      CHECK((etl::is_same<etl::make_signed<long>::type,               std::make_signed<long>::type>::value));
      CHECK((etl::is_same<etl::make_signed<signed long>::type,        std::make_signed<signed long>::type>::value));
      CHECK((etl::is_same<etl::make_signed<unsigned long>::type,      std::make_signed<unsigned long>::type>::value));
      CHECK((etl::is_same<etl::make_signed<long long>::type,          std::make_signed<long long>::type>::value));
      CHECK((etl::is_same<etl::make_signed<signed long long>::type,   std::make_signed<signed long long>::type>::value));
      CHECK((etl::is_same<etl::make_signed<unsigned long long>::type, std::make_signed<unsigned long long>::type>::value));
      CHECK((etl::is_same<etl::make_signed<const int>::type,          std::make_signed<const int>::type>::value));
      CHECK((etl::is_same<etl::make_signed<volatile int>::type,       std::make_signed<volatile int>::type>::value));
      CHECK((etl::is_same<etl::make_signed<const int>::type,          std::make_signed<const int>::type>::value));
      CHECK((etl::is_same<etl::make_signed<const volatile int>::type, std::make_signed<const volatile int>::type>::value));
      CHECK((etl::is_same<etl::make_signed<size_t>::type,             std::make_signed<size_t>::type>::value));

      enum class ue : uint8_t
      {
        One,
        Two
      };

      CHECK((etl::is_same<etl::make_signed<__underlying_type(ue)>::type, std::make_signed<ue>::type>::value));

      enum class se : int8_t
      {
        One,
        Two
      };

      CHECK((etl::is_same<etl::make_signed<__underlying_type(se)>::type, std::make_signed<se>::type>::value));
    }

    //*************************************************************************
    TEST(test_make_unsigned)
    {
      CHECK((etl::is_same<etl::make_unsigned<char>::type,               std::make_unsigned<char>::type>::value));
      CHECK((etl::is_same<etl::make_unsigned<signed char>::type,        std::make_unsigned<signed char>::type>::value));
      CHECK((etl::is_same<etl::make_unsigned<unsigned char>::type,      std::make_unsigned<unsigned char>::type>::value));
      CHECK(etl::is_unsigned<etl::make_unsigned<wchar_t>::type>::value && (sizeof(wchar_t) == sizeof(etl::make_unsigned<wchar_t>::type)));
      CHECK((etl::is_same<etl::make_unsigned<short>::type,              std::make_unsigned<short>::type>::value));
      CHECK((etl::is_same<etl::make_unsigned<signed short>::type,       std::make_unsigned<signed short>::type>::value));
      CHECK((etl::is_same<etl::make_unsigned<unsigned short>::type,     std::make_unsigned<unsigned short>::type>::value));
      CHECK((etl::is_same<etl::make_unsigned<int>::type,                std::make_unsigned<int>::type>::value));
      CHECK((etl::is_same<etl::make_unsigned<signed int>::type,         std::make_unsigned<signed int>::type>::value));
      CHECK((etl::is_same<etl::make_unsigned<unsigned int>::type,       std::make_unsigned<unsigned int>::type>::value));
      CHECK((etl::is_same<etl::make_unsigned<long>::type,               std::make_unsigned<long>::type>::value));
      CHECK((etl::is_same<etl::make_unsigned<signed long>::type,        std::make_unsigned<signed long>::type>::value));
      CHECK((etl::is_same<etl::make_unsigned<unsigned long>::type,      std::make_unsigned<unsigned long>::type>::value));
      CHECK((etl::is_same<etl::make_unsigned<long long>::type,          std::make_unsigned<long long>::type>::value));
      CHECK((etl::is_same<etl::make_unsigned<signed long long>::type,   std::make_unsigned<signed long long>::type>::value));
      CHECK((etl::is_same<etl::make_unsigned<unsigned long long>::type, std::make_unsigned<unsigned long long>::type>::value));
      CHECK((etl::is_same<etl::make_unsigned<const int>::type,          std::make_unsigned<const int>::type>::value));
      CHECK((etl::is_same<etl::make_unsigned<volatile int>::type,       std::make_unsigned<volatile int>::type>::value));
      CHECK((etl::is_same<etl::make_unsigned<const int>::type,          std::make_unsigned<const int>::type>::value));
      CHECK((etl::is_same<etl::make_unsigned<const volatile int>::type, std::make_unsigned<const volatile int>::type>::value));
      CHECK((etl::is_same<etl::make_unsigned<size_t>::type,             std::make_unsigned<size_t>::type>::value));

      enum class ue : uint8_t
      {
        One,
        Two
      };

      CHECK((etl::is_same<etl::make_unsigned<__underlying_type(ue)>::type, std::make_unsigned<ue>::type>::value));

      enum class se : int8_t
      {
        One,
        Two
      };

      CHECK((etl::is_same<etl::make_unsigned<__underlying_type(se)>::type, std::make_unsigned<se>::type>::value));
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
      CHECK((etl::is_same<etl::remove_extent<int>::type,     std::remove_extent<int>::type>::value));
      CHECK((etl::is_same<etl::remove_extent<int[]>::type,   std::remove_extent<int[]>::type>::value));
      CHECK((etl::is_same<etl::remove_extent<int[10]>::type, std::remove_extent<int[10]>::type>::value));
    }

    //*************************************************************************
    TEST(test_remove_all_extents)
    {
      CHECK((etl::is_same<etl::remove_all_extents<int>::type,         std::remove_all_extents<int>::type>::value));
      CHECK((etl::is_same<etl::remove_all_extents<int[10]>::type,     std::remove_all_extents<int[10]>::type>::value));
      CHECK((etl::is_same<etl::remove_all_extents<int[10][10]>::type, std::remove_all_extents<int[10][10]>::type>::value));
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

      CHECK((std::is_base_of<A, A>::value) == (etl::is_base_of<A, A>::value));
      CHECK((std::is_base_of<A, B>::value) == (etl::is_base_of<A, B>::value));
      CHECK((std::is_base_of<A, C>::value) == (etl::is_base_of<A, C>::value));
      CHECK((std::is_base_of<B, A>::value) == (etl::is_base_of<B, A>::value));
      CHECK((std::is_base_of<B, B>::value) == (etl::is_base_of<B, B>::value));
      CHECK((std::is_base_of<B, C>::value) == (etl::is_base_of<B, C>::value));
      CHECK((std::is_base_of<C, A>::value) == (etl::is_base_of<C, A>::value));
      CHECK((std::is_base_of<C, B>::value) == (etl::is_base_of<C, B>::value));
      CHECK((std::is_base_of<C, C>::value) == (etl::is_base_of<C, C>::value));

      CHECK((std::is_base_of<char, char>::value) == (etl::is_base_of<char, char>::value));
      CHECK((std::is_base_of<char, int>::value)  == (etl::is_base_of<char, int>::value));
      CHECK((std::is_base_of<int,  char>::value) == (etl::is_base_of<int,  char>::value));
    }

    //*************************************************************************
    TEST(test_types)
    {
      CHECK((etl::is_same<int,              etl::types<int>::type>::value));
      CHECK((etl::is_same<int&,             etl::types<int>::reference>::value));
      CHECK((etl::is_same<const int&,       etl::types<int>::const_reference>::value));
      CHECK((etl::is_same<int&&,            etl::types<int>::rvalue_reference>::value));
      CHECK((etl::is_same<int*,             etl::types<int>::pointer>::value));
      CHECK((etl::is_same<const int*,       etl::types<int>::const_pointer>::value));
      CHECK((etl::is_same<const int* const, etl::types<int>::const_pointer_const>::value));

      CHECK((etl::is_same<int,              etl::types<const int>::type>::value));
      CHECK((etl::is_same<int&,             etl::types<const int>::reference>::value));
      CHECK((etl::is_same<const int&,       etl::types<const int>::const_reference>::value));
      CHECK((etl::is_same<int&&,            etl::types<const int>::rvalue_reference>::value));
      CHECK((etl::is_same<int*,             etl::types<const int>::pointer>::value));
      CHECK((etl::is_same<const int*,       etl::types<const int>::const_pointer>::value));
      CHECK((etl::is_same<const int* const, etl::types<const int>::const_pointer_const>::value));

      CHECK((etl::is_same<int,              etl::types<int*>::type>::value));
      CHECK((etl::is_same<int&,             etl::types<int*>::reference>::value));
      CHECK((etl::is_same<const int&,       etl::types<int*>::const_reference>::value));
      CHECK((etl::is_same<int&&,            etl::types<int*>::rvalue_reference>::value));
      CHECK((etl::is_same<int*,             etl::types<int*>::pointer>::value));
      CHECK((etl::is_same<const int*,       etl::types<int*>::const_pointer>::value));
      CHECK((etl::is_same<const int* const, etl::types<int*>::const_pointer_const>::value));

      CHECK((etl::is_same<int,              etl::types<int* const>::type>::value));
      CHECK((etl::is_same<int&,             etl::types<int* const>::reference>::value));
      CHECK((etl::is_same<const int&,       etl::types<int* const>::const_reference>::value));
      CHECK((etl::is_same<int&&,            etl::types<int* const>::rvalue_reference>::value));
      CHECK((etl::is_same<int*,             etl::types<int* const>::pointer>::value));
      CHECK((etl::is_same<const int*,       etl::types<int* const>::const_pointer>::value));
      CHECK((etl::is_same<const int* const, etl::types<int* const>::const_pointer_const>::value));

      CHECK((etl::is_same<int,              etl::types<const int*>::type>::value));
      CHECK((etl::is_same<int&,             etl::types<const int*>::reference>::value));
      CHECK((etl::is_same<const int&,       etl::types<const int*>::const_reference>::value));
      CHECK((etl::is_same<int&&,            etl::types<const int*>::rvalue_reference>::value));
      CHECK((etl::is_same<int*,             etl::types<const int*>::pointer>::value));
      CHECK((etl::is_same<const int*,       etl::types<const int*>::const_pointer>::value));
      CHECK((etl::is_same<const int* const, etl::types<const int*>::const_pointer_const>::value));

      CHECK((etl::is_same<int,              etl::types<const int* const>::type>::value));
      CHECK((etl::is_same<int&,             etl::types<const int* const>::reference>::value));
      CHECK((etl::is_same<const int&,       etl::types<const int* const>::const_reference>::value));
      CHECK((etl::is_same<int&&,            etl::types<const int* const>::rvalue_reference>::value));
      CHECK((etl::is_same<int*,             etl::types<const int* const>::pointer>::value));
      CHECK((etl::is_same<const int*,       etl::types<const int* const>::const_pointer>::value));
      CHECK((etl::is_same<const int* const, etl::types<const int* const>::const_pointer_const>::value));

      CHECK((etl::is_same<int,              etl::types<int&>::type>::value));
      CHECK((etl::is_same<int&,             etl::types<int&>::reference>::value));
      CHECK((etl::is_same<const int&,       etl::types<int&>::const_reference>::value));
      CHECK((etl::is_same<int&&,            etl::types<int&>::rvalue_reference>::value));
      CHECK((etl::is_same<int*,             etl::types<int&>::pointer>::value));
      CHECK((etl::is_same<const int*,       etl::types<int&>::const_pointer>::value));
      CHECK((etl::is_same<const int* const, etl::types<int&>::const_pointer_const>::value));

      CHECK((etl::is_same<int,              etl::types<const int&>::type>::value));
      CHECK((etl::is_same<int&,             etl::types<const int&>::reference>::value));
      CHECK((etl::is_same<const int&,       etl::types<const int&>::const_reference>::value));
      CHECK((etl::is_same<int&&,            etl::types<const int&>::rvalue_reference>::value));
      CHECK((etl::is_same<int*,             etl::types<const int&>::pointer>::value));
      CHECK((etl::is_same<const int*,       etl::types<const int&>::const_pointer>::value));
      CHECK((etl::is_same<const int* const, etl::types<const int&>::const_pointer_const>::value));

      CHECK((etl::is_same<int,              etl::types<int&&>::type>::value));
      CHECK((etl::is_same<int&,             etl::types<int&&>::reference>::value));
      CHECK((etl::is_same<const int&,       etl::types<int&&>::const_reference>::value));
      CHECK((etl::is_same<int&&,            etl::types<int&&>::rvalue_reference>::value));
      CHECK((etl::is_same<int*,             etl::types<int&&>::pointer>::value));
      CHECK((etl::is_same<const int*,       etl::types<int&&>::const_pointer>::value));
      CHECK((etl::is_same<const int* const, etl::types<int&&>::const_pointer_const>::value));

      CHECK((etl::is_same<int,              etl::types<volatile int>::type>::value));
      CHECK((etl::is_same<int&,             etl::types<volatile int>::reference>::value));
      CHECK((etl::is_same<const int&,       etl::types<volatile int>::const_reference>::value));
      CHECK((etl::is_same<int&&,            etl::types<volatile int>::rvalue_reference>::value));
      CHECK((etl::is_same<int*,             etl::types<volatile int>::pointer>::value));
      CHECK((etl::is_same<const int*,       etl::types<volatile int>::const_pointer>::value));
      CHECK((etl::is_same<const int* const, etl::types<volatile int>::const_pointer_const>::value));

      CHECK((etl::is_same<int,              etl::types<const volatile int>::type>::value));
      CHECK((etl::is_same<int&,             etl::types<const volatile int>::reference>::value));
      CHECK((etl::is_same<const int&,       etl::types<const volatile int>::const_reference>::value));
      CHECK((etl::is_same<int&&,            etl::types<const volatile int>::rvalue_reference>::value));
      CHECK((etl::is_same<int*,             etl::types<const volatile int>::pointer>::value));
      CHECK((etl::is_same<const int*,       etl::types<const volatile int>::const_pointer>::value));
      CHECK((etl::is_same<const int* const, etl::types<const volatile int>::const_pointer_const>::value));

      CHECK((etl::is_same<int,              etl::types<volatile int*>::type>::value));
      CHECK((etl::is_same<int&,             etl::types<volatile int*>::reference>::value));
      CHECK((etl::is_same<const int&,       etl::types<volatile int*>::const_reference>::value));
      CHECK((etl::is_same<int&&,            etl::types<volatile int*>::rvalue_reference>::value));
      CHECK((etl::is_same<int*,             etl::types<volatile int*>::pointer>::value));
      CHECK((etl::is_same<const int*,       etl::types<volatile int*>::const_pointer>::value));
      CHECK((etl::is_same<const int* const, etl::types<volatile int*>::const_pointer_const>::value));

      CHECK((etl::is_same<int,              etl::types<volatile int* const>::type>::value));
      CHECK((etl::is_same<int&,             etl::types<volatile int* const>::reference>::value));
      CHECK((etl::is_same<const int&,       etl::types<volatile int* const>::const_reference>::value));
      CHECK((etl::is_same<int&&,            etl::types<volatile int* const>::rvalue_reference>::value));
      CHECK((etl::is_same<int*,             etl::types<volatile int* const>::pointer>::value));
      CHECK((etl::is_same<const int*,       etl::types<volatile int* const>::const_pointer>::value));
      CHECK((etl::is_same<const int* const, etl::types<volatile int* const>::const_pointer_const>::value));

      CHECK((etl::is_same<int,              etl::types<const volatile int*>::type>::value));
      CHECK((etl::is_same<int&,             etl::types<const volatile int*>::reference>::value));
      CHECK((etl::is_same<const int&,       etl::types<const volatile int*>::const_reference>::value));
      CHECK((etl::is_same<int&&,            etl::types<const volatile int*>::rvalue_reference>::value));
      CHECK((etl::is_same<int*,             etl::types<const volatile int*>::pointer>::value));
      CHECK((etl::is_same<const int*,       etl::types<const volatile int*>::const_pointer>::value));
      CHECK((etl::is_same<const int* const, etl::types<const volatile int*>::const_pointer_const>::value));

      CHECK((etl::is_same<int,              etl::types<const volatile int* const>::type>::value));
      CHECK((etl::is_same<int&,             etl::types<const volatile int* const>::reference>::value));
      CHECK((etl::is_same<const int&,       etl::types<const volatile int* const>::const_reference>::value));
      CHECK((etl::is_same<int&&,            etl::types<const volatile int* const>::rvalue_reference>::value));
      CHECK((etl::is_same<int*,             etl::types<const volatile int* const>::pointer>::value));
      CHECK((etl::is_same<const int*,       etl::types<const volatile int* const>::const_pointer>::value));
      CHECK((etl::is_same<const int* const, etl::types<const volatile int* const>::const_pointer_const>::value));

      CHECK((etl::is_same<int,              etl::types<volatile int&>::type>::value));
      CHECK((etl::is_same<int&,             etl::types<volatile int&>::reference>::value));
      CHECK((etl::is_same<const int&,       etl::types<volatile int&>::const_reference>::value));
      CHECK((etl::is_same<int&&,            etl::types<volatile int&>::rvalue_reference>::value));
      CHECK((etl::is_same<int*,             etl::types<volatile int&>::pointer>::value));
      CHECK((etl::is_same<const int*,       etl::types<volatile int&>::const_pointer>::value));
      CHECK((etl::is_same<const int* const, etl::types<volatile int&>::const_pointer_const>::value));

      CHECK((etl::is_same<int,              etl::types<const volatile int&>::type>::value));
      CHECK((etl::is_same<int&,             etl::types<const volatile int&>::reference>::value));
      CHECK((etl::is_same<const int&,       etl::types<const volatile int&>::const_reference>::value));
      CHECK((etl::is_same<int&&,            etl::types<const volatile int&>::rvalue_reference>::value));
      CHECK((etl::is_same<int*,             etl::types<const volatile int&>::pointer>::value));
      CHECK((etl::is_same<const int*,       etl::types<const volatile int&>::const_pointer>::value));
      CHECK((etl::is_same<const int* const, etl::types<const volatile int&>::const_pointer_const>::value));
    }
  };

  //*************************************************************************
  TEST(test_conditional_integral_constant)
  {
    int v1 = etl::conditional_integral_constant<true,  int, 1, 2>::value;
    int v2 = etl::conditional_integral_constant<false, int, 1, 2>::value;

    CHECK_EQUAL(1, v1);
    CHECK_EQUAL(2, v2);
  }

  //*************************************************************************
  TEST(test_size_of)
  {
#if ETL_USING_CPP17
    CHECK_EQUAL(1, etl::size_of_v<void>);
    CHECK_EQUAL(1, etl::size_of_v<char>);
    CHECK_EQUAL(2, etl::size_of_v<short>);
    CHECK_EQUAL(4, etl::size_of_v<int>);
    CHECK_EQUAL(20, etl::size_of_v<TestData>);
#else
    CHECK_EQUAL(1, etl::size_of<void>::value);
    CHECK_EQUAL(1, etl::size_of<char>::value);
    CHECK_EQUAL(2, etl::size_of<short>::value);
    CHECK_EQUAL(4, etl::size_of<int>::value);
    CHECK_EQUAL(20, etl::size_of<TestData>::value);
#endif
  }

  //*************************************************************************
  TEST(test_is_convertible)
  {
#if ETL_USING_CPP17
    CHECK((etl::is_convertible_v<char, int>));
    CHECK((etl::is_convertible_v<int, char>));
    CHECK((etl::is_convertible_v<unsigned char, int>));
    CHECK((etl::is_convertible_v<int, unsigned char>));
    CHECK((etl::is_convertible_v<double, int>));
    CHECK((etl::is_convertible_v<int, double>));
    CHECK((etl::is_convertible_v<int*, void*>));
    CHECK(!(etl::is_convertible_v<int(*)(), void(*)()>));
    CHECK(!(etl::is_convertible_v<int(*)(), int*>));
    CHECK(!(etl::is_convertible_v<int*, int(*)()>));
    CHECK(!(etl::is_convertible_v<void*, int*>));
#else
    CHECK((etl::is_convertible<char, int>::value));
    CHECK((etl::is_convertible<int,  char>::value));
    CHECK((etl::is_convertible<unsigned char, int>::value));
    CHECK((etl::is_convertible<int, unsigned char>::value));
    CHECK((etl::is_convertible<double, int>::value));
    CHECK((etl::is_convertible<int, double>::value));
    CHECK((etl::is_convertible<int*,  void*>::value));
    CHECK(!(etl::is_convertible<int(*)(), void(*)()>::value));
    CHECK(!(etl::is_convertible<int(*)(), int*>::value));
    CHECK(!(etl::is_convertible<int*, int(*)()>::value));
    CHECK(!(etl::is_convertible<void*, int*>::value));
#endif
  }

  //*************************************************************************
  TEST(test_add_lvalue_reference)
  {
#if ETL_USING_CPP17
    CHECK(!std::is_lvalue_reference_v<etl::add_lvalue_reference<void>::type>);
    CHECK(std::is_lvalue_reference_v<etl::add_lvalue_reference<int>::type>);
    CHECK(std::is_lvalue_reference_v<etl::add_lvalue_reference<int*>::type>);
    CHECK(std::is_lvalue_reference_v<etl::add_lvalue_reference<int&>::type>);
    CHECK(std::is_lvalue_reference_v<etl::add_lvalue_reference<int&&>::type>);
#else
#endif
  }

  //*************************************************************************
  TEST(test_add_rvalue_reference)
  {
#if ETL_USING_CPP17
    CHECK(!std::is_rvalue_reference_v<etl::add_rvalue_reference<void>::type>);
    CHECK(std::is_rvalue_reference_v<etl::add_rvalue_reference<int>::type>);
    CHECK(std::is_rvalue_reference_v<etl::add_rvalue_reference<int*>::type>);
    CHECK(!std::is_rvalue_reference_v<etl::add_rvalue_reference<int&>::type>);
    CHECK(std::is_rvalue_reference_v<etl::add_rvalue_reference<int&&>::type>);
#else
#endif
  }

  //*************************************************************************
  TEST(test_is_lvalue_reference)
  {
#if ETL_USING_CPP17
    CHECK_EQUAL(std::is_lvalue_reference_v<void>,  etl::is_lvalue_reference_v<void>);
    CHECK_EQUAL(std::is_lvalue_reference_v<int>,   etl::is_lvalue_reference_v<int>);
    CHECK_EQUAL(std::is_lvalue_reference_v<int*>,  etl::is_lvalue_reference_v<int*>);
    CHECK_EQUAL(std::is_lvalue_reference_v<int&>,  etl::is_lvalue_reference_v<int&>);
    CHECK_EQUAL(std::is_lvalue_reference_v<int&&>, etl::is_lvalue_reference_v<int&&>);
#else
#endif
  }

  //*************************************************************************
  TEST(test_is_rvalue_reference)
  {
#if ETL_USING_CPP17
    CHECK_EQUAL(std::is_rvalue_reference_v<void>,  etl::is_rvalue_reference_v<void>);
    CHECK_EQUAL(std::is_rvalue_reference_v<int>,   etl::is_rvalue_reference_v<int>);
    CHECK_EQUAL(std::is_rvalue_reference_v<int*>,  etl::is_rvalue_reference_v<int*>);
    CHECK_EQUAL(std::is_rvalue_reference_v<int&>,  etl::is_rvalue_reference_v<int&>);
    CHECK_EQUAL(std::is_rvalue_reference_v<int&&>, etl::is_rvalue_reference_v<int&&>);
#else
    CHECK_EQUAL(std::is_rvalue_reference<void>::value,  etl::is_rvalue_reference<void>::value);
    CHECK_EQUAL(std::is_rvalue_reference<int>::value,   etl::is_rvalue_reference<int>::value);
    CHECK_EQUAL(std::is_rvalue_reference<int*>::value,  etl::is_rvalue_reference<int*>::value);
    CHECK_EQUAL(std::is_rvalue_reference<int&>::value,  etl::is_rvalue_reference<int&>::value);
    CHECK_EQUAL(std::is_rvalue_reference<int&&>::value, etl::is_rvalue_reference<int&&>::value);
#endif
  }

  //*************************************************************************
#if ETL_USING_CPP17
  #define CHECK_EQUAL_FOR_TYPE(type) CHECK_EQUAL(std::is_enum_v<type>, etl::is_enum_v<type>)
#else
  #define CHECK_EQUAL_FOR_TYPE(type) CHECK_EQUAL(std::is_enum<type>::value, etl::is_enum<type>::value)
#endif

  TEST(test_is_enum) 
  {
    CHECK_EQUAL_FOR_TYPE(void);
    CHECK_EQUAL_FOR_TYPE(void*);
    CHECK_EQUAL_FOR_TYPE(int);
    CHECK_EQUAL_FOR_TYPE(int*);
    CHECK_EQUAL_FOR_TYPE(ToAny);
    CHECK_EQUAL_FOR_TYPE(NotDefaultConstructable);
    CHECK_EQUAL_FOR_TYPE(Enum);
    CHECK_EQUAL_FOR_TYPE(Enum&);
    CHECK_EQUAL_FOR_TYPE(Enum&&);
    CHECK_EQUAL_FOR_TYPE(Enum*);
    CHECK_EQUAL_FOR_TYPE(const Enum);
    CHECK_EQUAL_FOR_TYPE(volatile Enum);
    CHECK_EQUAL_FOR_TYPE(const volatile Enum);
    CHECK_EQUAL_FOR_TYPE(EnumClass);
    CHECK_EQUAL_FOR_TYPE(EnumClass&);
    CHECK_EQUAL_FOR_TYPE(EnumClass&&);
    CHECK_EQUAL_FOR_TYPE(EnumClass*);
    CHECK_EQUAL_FOR_TYPE(const EnumClass);
    CHECK_EQUAL_FOR_TYPE(volatile EnumClass);
    CHECK_EQUAL_FOR_TYPE(const volatile EnumClass);

  }
  #undef CHECK_EQUAL_FOR_TYPE

  //*************************************************************************
  TEST(test_integral_constants)
  {
#if ETL_USING_CPP17
    CHECK_EQUAL(1, (etl::integral_constant_v<int, 1>));
    CHECK((std::is_same_v<int, etl::integral_constant<int, 1>::value_type>));

    CHECK_EQUAL(false, etl::bool_constant_v<false>);
    CHECK_EQUAL(true,  etl::bool_constant_v<true>);
    CHECK((std::is_same_v<bool, etl::bool_constant<true>::value_type>));

    CHECK_EQUAL(true,  etl::negation_v<etl::bool_constant<false>>);
    CHECK_EQUAL(false, etl::negation_v<etl::bool_constant<true>>);
    CHECK((std::is_same_v<bool, etl::bool_constant<false>::value_type>));
#else
    CHECK_EQUAL(1, (etl::integral_constant<int, 1>::value));
    CHECK((std::is_same<int, etl::integral_constant<int, 1>::value_type>::value));

    CHECK_EQUAL(false, (etl::bool_constant<false>::value));
    CHECK_EQUAL(true,  (etl::bool_constant<true>::value));
    CHECK((std::is_same<bool, etl::bool_constant<true>::value_type>::value));

    CHECK_EQUAL(true,  etl::negation<etl::bool_constant<false>>::value);
    CHECK_EQUAL(false, etl::negation<etl::bool_constant<true>>::value);
    CHECK((std::is_same<bool, etl::bool_constant<true>::value_type>::value));
#endif
  }

  //*************************************************************************
  TEST(test_are_all_same)
  {
#if ETL_USING_CPP17
    CHECK((etl::are_all_same_v<int, int, int, int, int>  == true));
    CHECK((etl::are_all_same_v<int, int, int, char, int> == false));
#else
    CHECK((etl::are_all_same<int, int, int, int, int>::value  == true));
    CHECK((etl::are_all_same<int, int, int, char, int>::value == false));
#endif
  }

  //*************************************************************************
  TEST(test_conjunction)
  {
#if ETL_USING_CPP17
    CHECK((etl::conjunction_v<etl::true_type, etl::true_type, etl::true_type>));
    CHECK((!etl::conjunction_v<etl::true_type, etl::false_type, etl::true_type>));
#else
    CHECK((etl::conjunction<etl::true_type, etl::true_type, etl::true_type>::value));
    CHECK((!etl::conjunction<etl::true_type, etl::false_type, etl::true_type>::value));
#endif
  }

  //*************************************************************************
  TEST(test_disjunction)
  {
#if ETL_USING_CPP17
    CHECK((etl::disjunction_v<etl::false_type, etl::true_type, etl::false_type>));
    CHECK((!etl::disjunction_v<etl::false_type, etl::false_type, etl::false_type>));
#else
    CHECK((etl::disjunction<etl::false_type, etl::true_type, etl::false_type>::value));
    CHECK((!etl::disjunction<etl::false_type, etl::false_type, etl::false_type>::value));
#endif
  }

  //*************************************************************************
  TEST(test_is_assignable)
  {
#if ETL_USING_CPP17
    CHECK((etl::is_assignable_v<Copyable, Copyable>) == (std::is_assignable_v<Copyable, Copyable>));
    CHECK((etl::is_assignable_v<Moveable, Moveable>) == (std::is_assignable_v<Moveable, Moveable>));
    CHECK((etl::is_assignable_v<MoveableCopyable, MoveableCopyable>) == (std::is_assignable_v<MoveableCopyable, MoveableCopyable>));
#else
    CHECK((etl::is_assignable<Copyable, Copyable>::value) == (std::is_assignable<Copyable, Copyable>::value));
    CHECK((etl::is_assignable<Moveable, Moveable>::value) == (std::is_assignable<Moveable, Moveable>::value));
    CHECK((etl::is_assignable<MoveableCopyable, MoveableCopyable>::value) == (std::is_assignable<MoveableCopyable, MoveableCopyable>::value));
#endif
  }

  //*************************************************************************
  TEST(test_is_lvalue_assignable)
  {
#if ETL_USING_STL || defined(ETL_USE_TYPE_TRAITS_BUILTINS) || defined(ETL_USER_DEFINED_TYPE_TRAITS)
#if ETL_USING_CPP17
    CHECK((etl::is_lvalue_assignable_v<Copyable, Copyable>));
    CHECK(!(etl::is_lvalue_assignable_v<Moveable, Moveable>));
    CHECK((etl::is_lvalue_assignable_v<MoveableCopyable, MoveableCopyable>));
#else
    CHECK((etl::is_lvalue_assignable<Copyable, Copyable>::value)); 
    CHECK(!(etl::is_lvalue_assignable<Moveable, Moveable>::value));
    CHECK((etl::is_lvalue_assignable<MoveableCopyable, MoveableCopyable>::value));
#endif
#endif
  }

  //*************************************************************************
  TEST(test_is_constructible)
  {
#if ETL_USING_CPP17
    CHECK((etl::is_constructible_v<Copyable>) == (std::is_constructible_v<Copyable>));
    CHECK((etl::is_constructible_v<Moveable>) == (std::is_constructible_v<Moveable>));
    CHECK((etl::is_constructible_v<MoveableCopyable>) == (std::is_constructible_v<MoveableCopyable>));
#else
    CHECK((etl::is_constructible<Copyable>::value) == (std::is_constructible<Copyable>::value));
    CHECK((etl::is_constructible<Moveable>::value) == (std::is_constructible<Moveable>::value));
    CHECK((etl::is_constructible<MoveableCopyable>::value) == (std::is_constructible<MoveableCopyable>::value));
#endif
  }

  //*************************************************************************
  TEST(test_is_copy_constructible)
  {
#if ETL_USING_CPP17
    CHECK((etl::is_copy_constructible_v<Copyable>) == (std::is_copy_constructible_v<Copyable>));
    CHECK((etl::is_copy_constructible_v<Moveable>) == (std::is_copy_constructible_v<Moveable>));
    CHECK((etl::is_copy_constructible_v<MoveableCopyable>) == (std::is_copy_constructible_v<MoveableCopyable>));
#else
    CHECK((etl::is_copy_constructible<Copyable>::value) == (std::is_copy_constructible<Copyable>::value));
    CHECK((etl::is_copy_constructible<Moveable>::value) == (std::is_copy_constructible<Moveable>::value));
    CHECK((etl::is_copy_constructible<MoveableCopyable>::value) == (std::is_copy_constructible<MoveableCopyable>::value));
#endif
  }

  //*************************************************************************
  TEST(test_is_move_constructible)
  {
#if ETL_USING_CPP17
    CHECK((etl::is_move_constructible_v<Copyable>) == (std::is_move_constructible_v<Copyable>));
    CHECK((etl::is_move_constructible_v<Moveable>) == (std::is_move_constructible_v<Moveable>));
    CHECK((etl::is_move_constructible_v<MoveableCopyable>) == (std::is_move_constructible_v<MoveableCopyable>));
#else
    CHECK((etl::is_move_constructible<Copyable>::value) == (std::is_move_constructible<Copyable>::value));
    CHECK((etl::is_move_constructible<Moveable>::value) == (std::is_move_constructible<Moveable>::value));
    CHECK((etl::is_move_constructible<MoveableCopyable>::value) == (std::is_move_constructible<MoveableCopyable>::value));
#endif
  }

  //*************************************************************************
  TEST(test_is_trivially_constructible)
  {
#if ETL_USING_STL || defined(ETL_USE_TYPE_TRAITS_BUILTINS) || defined(ETL_USER_DEFINED_TYPE_TRAITS)
#if ETL_USING_CPP17
    CHECK((etl::is_trivially_constructible_v<Copyable>) == (std::is_trivially_constructible_v<Copyable>));
    CHECK((etl::is_trivially_constructible_v<Moveable>) == (std::is_trivially_constructible_v<Moveable>));
    CHECK((etl::is_trivially_constructible_v<MoveableCopyable>) == (std::is_trivially_constructible_v<MoveableCopyable>));
#else
    CHECK((etl::is_trivially_constructible<Copyable>::value) == (std::is_trivially_constructible<Copyable>::value));
    CHECK((etl::is_trivially_constructible<Moveable>::value) == (std::is_trivially_constructible<Moveable>::value));
    CHECK((etl::is_trivially_constructible<MoveableCopyable>::value) == (std::is_trivially_constructible<MoveableCopyable>::value));
#endif
#endif
  }

  //*************************************************************************
  TEST(test_is_trivially_copy_constructible)
  {
#if ETL_USING_STL || defined(ETL_USE_TYPE_TRAITS_BUILTINS) || defined(ETL_USER_DEFINED_TYPE_TRAITS)
#if ETL_USING_CPP17
    CHECK((etl::is_trivially_copy_constructible_v<Copyable>) == (std::is_trivially_copy_constructible_v<Copyable>));
    CHECK((etl::is_trivially_copy_constructible_v<Moveable>) == (std::is_trivially_copy_constructible_v<Moveable>));
    CHECK((etl::is_trivially_copy_constructible_v<MoveableCopyable>) == (std::is_trivially_copy_constructible_v<MoveableCopyable>));
#else
    CHECK((etl::is_trivially_copy_constructible<Copyable>::value) == (std::is_trivially_copy_constructible<Copyable>::value));
    CHECK((etl::is_trivially_copy_constructible<Moveable>::value) == (std::is_trivially_copy_constructible<Moveable>::value));
    CHECK((etl::is_trivially_copy_constructible<MoveableCopyable>::value) == (std::is_trivially_copy_constructible<MoveableCopyable>::value));
#endif
#endif
  }

  //*************************************************************************
  TEST(test_is_trivially_destructible)
  {
#if ETL_USING_STL || defined(ETL_USE_TYPE_TRAITS_BUILTINS) || defined(ETL_USER_DEFINED_TYPE_TRAITS)
#if ETL_USING_CPP17
    CHECK((etl::is_trivially_destructible_v<Copyable>) == (std::is_trivially_destructible_v<Copyable>));
    CHECK((etl::is_trivially_destructible_v<Moveable>) == (std::is_trivially_destructible_v<Moveable>));
    CHECK((etl::is_trivially_destructible_v<MoveableCopyable>) == (std::is_trivially_destructible_v<MoveableCopyable>));
#else
    CHECK((etl::is_trivially_destructible<Copyable>::value) == (std::is_trivially_destructible<Copyable>::value));
    CHECK((etl::is_trivially_destructible<Moveable>::value) == (std::is_trivially_destructible<Moveable>::value));
    CHECK((etl::is_trivially_destructible<MoveableCopyable>::value) == (std::is_trivially_destructible<MoveableCopyable>::value));
#endif
#endif
  }

  //*************************************************************************
  TEST(test_is_trivially_copy_assignable)
  {
#if (!(defined(ETL_COMPILER_GCC) && defined(ETL_USE_TYPE_TRAITS_BUILTINS)))
#if ETL_USING_STL || defined(ETL_USE_TYPE_TRAITS_BUILTINS) || defined(ETL_USER_DEFINED_TYPE_TRAITS)
#if ETL_USING_CPP17
    CHECK((etl::is_trivially_copy_assignable_v<Copyable>) == (std::is_trivially_copy_assignable_v<Copyable>));
    CHECK((etl::is_trivially_copy_assignable_v<Moveable>) == (std::is_trivially_copy_assignable_v<Moveable>));
    CHECK((etl::is_trivially_copy_assignable_v<MoveableCopyable>) == (std::is_trivially_copy_assignable_v<MoveableCopyable>));
#else
    CHECK((etl::is_trivially_copy_assignable<Copyable>::value) == (std::is_trivially_copy_assignable<Copyable>::value));
    CHECK((etl::is_trivially_copy_assignable<Moveable>::value) == (std::is_trivially_copy_assignable<Moveable>::value));
    CHECK((etl::is_trivially_copy_assignable<MoveableCopyable>::value) == (std::is_trivially_copy_assignable<MoveableCopyable>::value));
#endif
#endif
#endif
  }

  //*************************************************************************
  TEST(test_is_trivially_copyable)
  {
#if (!(defined(ETL_COMPILER_GCC) && defined(ETL_USE_TYPE_TRAITS_BUILTINS)))
#if ETL_USING_STL || defined(ETL_USE_TYPE_TRAITS_BUILTINS) || defined(ETL_USER_DEFINED_TYPE_TRAITS)
#if ETL_USING_CPP17
    CHECK((etl::is_trivially_copyable_v<Copyable>) == (std::is_trivially_copyable_v<Copyable>));
    CHECK((etl::is_trivially_copyable_v<Moveable>) == (std::is_trivially_copyable_v<Moveable>));
    CHECK((etl::is_trivially_copyable_v<MoveableCopyable>) == (std::is_trivially_copyable_v<MoveableCopyable>));
#else
    CHECK((etl::is_trivially_copyable<Copyable>::value) == (std::is_trivially_copyable<Copyable>::value));
    CHECK((etl::is_trivially_copyable<Moveable>::value) == (std::is_trivially_copyable<Moveable>::value));
    CHECK((etl::is_trivially_copyable<MoveableCopyable>::value) == (std::is_trivially_copyable<MoveableCopyable>::value));
#endif
#endif
#endif
  }
}
