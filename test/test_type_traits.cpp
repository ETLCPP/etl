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
  struct TestData
  {
  };
} // namespace

namespace etl
{
  template <>
  struct size_of<TestData> : integral_constant<size_t, 20U>
  {
  };
} // namespace etl

namespace
{
  // A class to test non-fundamental types.
  struct Object
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

  // Class which can be implicitly converted to/from any default-constructable
  // type
  struct ToAny
  {
    ToAny() = default;
    template <typename T>
    ToAny(T)
    {
    }
    template <typename T>
    operator T()
    {
      return T();
    }
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
    Copyable& operator=(const Copyable&) noexcept
    {
      return *this;
    }

    Copyable(Copyable&&)            = delete;
    Copyable& operator=(Copyable&&) = delete;
  };

  //*********************************************
  struct Moveable
  {
    Moveable() {}
    Moveable(Moveable&&) noexcept {}
    Moveable& operator=(Moveable&&) noexcept
    {
      return *this;
    }

    Moveable(const Moveable&)            = delete;
    Moveable& operator=(const Moveable&) = delete;
  };

  //*********************************************
  struct MoveableCopyable
  {
    MoveableCopyable() {}
    MoveableCopyable(MoveableCopyable&&) noexcept {}
    MoveableCopyable& operator=(MoveableCopyable&&) noexcept
    {
      return *this;
    }
    MoveableCopyable(const MoveableCopyable&) {}
    MoveableCopyable& operator=(const MoveableCopyable&)
    {
      return *this;
    }
  };

  //*********************************************
  struct NotDefaultConstructible
  {
    NotDefaultConstructible() = delete;
    NotDefaultConstructible(const NotDefaultConstructible&) noexcept {}
    NotDefaultConstructible& operator=(const NotDefaultConstructible&) noexcept
    {
      return *this;
    }

    NotDefaultConstructible(NotDefaultConstructible&&)            = delete;
    NotDefaultConstructible& operator=(NotDefaultConstructible&&) = delete;
  };

  // The legacy compiler builtin for trivial construction does not test
  // accessibility, unlike is_trivially_constructible.
  struct PrivateTrivialDefaultConstructor
  {
  private:

    PrivateTrivialDefaultConstructor() = default;
  };

  // The legacy compiler builtin for trivial destruction does not test
  // accessibility or deletion, unlike is_trivially_destructible.
  struct PrivateTrivialDestructor
  {
  private:

    ~PrivateTrivialDestructor() = default;
  };

  struct DeletedTrivialDestructor
  {
    ~DeletedTrivialDestructor() = delete;
  };

  // A function to test etl::type_identity.
  template <typename T>
  T type_identity_test_add(T first, typename etl::type_identity<T>::type second)
  {
    return first + second;
  }

  // Structs to test is_specialized
  template <typename T>
  struct specialized
  {
  };

  template <typename T>
  struct other_specialized
  {
  };

  struct MF
  {
    int f(int)
    {
      return 0;
    }
    int fc(int) const
    {
      return 0;
    }
    int fv(int) volatile
    {
      return 0;
    }
    int fcv(int) const volatile
    {
      return 0;
    }

#if ETL_USING_CPP11
    int fl(int) &
    {
      return 0;
    }
    int flc(int) const&
    {
      return 0;
    }
    int flv(int) volatile&
    {
      return 0;
    }
    int flcv(int) const volatile&
    {
      return 0;
    }

    int fr(int) &&
    {
      return 0;
    }
    int frc(int) const&&
    {
      return 0;
    }
    int frv(int) volatile&&
    {
      return 0;
    }
    int frcv(int) const volatile&&
    {
      return 0;
    }
#endif

#if ETL_HAS_NOEXCEPT_FUNCTION_TYPE
    int fn(int) noexcept
    {
      return 0;
    }
    int fnc(int) const noexcept
    {
      return 0;
    }
    int fnv(int) volatile noexcept
    {
      return 0;
    }
    int fncv(int) const volatile noexcept
    {
      return 0;
    }

    int fnl(int) & noexcept
    {
      return 0;
    }
    int fnlc(int) const& noexcept
    {
      return 0;
    }
    int fnlv(int) volatile& noexcept
    {
      return 0;
    }
    int fnlcv(int) const volatile& noexcept
    {
      return 0;
    }

    int fnr(int) && noexcept
    {
      return 0;
    }
    int fnrc(int) const&& noexcept
    {
      return 0;
    }
    int fnrv(int) volatile&& noexcept
    {
      return 0;
    }
    int fnrcv(int) const volatile&& noexcept
    {
      return 0;
    }
#endif

    int fvar(int, ...)
    {
      return 0;
    }
    int fvarc(int, ...) const
    {
      return 0;
    }
  };

  struct MO
  {
    int data;
  };

  static int f(int)
  {
    return 0;
  }
  static int fvar(...)
  {
    return 0;
  }
  static int fvar2(int, ...)
  {
    return 0;
  }

  template <typename T, typename... TArgs>
  static T ft(TArgs...)
  {
    return T();
  }

#if ETL_HAS_NOEXCEPT_FUNCTION_TYPE
  static int fn(int) noexcept
  {
    return 0;
  }
  static int fnvar(...) noexcept
  {
    return 0;
  }
  static int fnvar2(int, ...) noexcept
  {
    return 0;
  }

  template <typename T, typename... TArgs>
  static T fnt(TArgs...) noexcept
  {
    return T();
  }
#endif
} // namespace

// Definitions for when the STL and compiler built-ins are not available.
#if ETL_NOT_USING_STL && !defined(ETL_USE_TYPE_TRAITS_BUILTINS)

using etl::is_assignable;
using etl::is_constructible;
using etl::is_copy_assignable;
using etl::is_copy_constructible;
using etl::is_move_assignable;
using etl::is_move_constructible;

//*************************
template <>
struct etl::is_assignable<Copyable, Copyable> : public etl::false_type
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

template <>
struct etl::is_copy_assignable<Copyable> : public etl::true_type
{
};

template <>
struct etl::is_move_assignable<Copyable> : public etl::false_type
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

template <>
struct etl::is_copy_assignable<Moveable> : public etl::false_type
{
};

template <>
struct etl::is_move_assignable<Moveable> : public etl::true_type
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

template <>
struct etl::is_copy_assignable<MoveableCopyable> : public etl::true_type
{
};

template <>
struct etl::is_move_assignable<MoveableCopyable> : public etl::true_type
{
};
#endif

#if ETL_USING_CPP11
namespace
{
  //*****************************************************************************
  // Types for the is_swappable tests.
  struct Swappable
  {
    int a;
  };

  struct NonSwappable
  {
    NonSwappable(const NonSwappable&)            = delete;
    NonSwappable& operator=(const NonSwappable&) = delete;
  };

  // Detects whether the compiler allows an rvalue to bind to a non-const lvalue
  // reference. This is a Microsoft extension, disabled by /permissive-, and it makes
  // types such as Swappable swappable as rvalues.
  template <typename T>
  void takes_lvalue_reference(T&);

  template <typename T, typename = void>
  struct rvalue_binds_to_lvalue_reference : etl::false_type
  {
  };

  template <typename T>
  struct rvalue_binds_to_lvalue_reference<T, etl::void_t<decltype(takes_lvalue_reference<T>(etl::declval<T>()))> > : etl::true_type
  {
  };

  // Types for the is_nothrow_swappable tests.
  // Swappable, as it is movable, but the move operations may throw.
  struct ThrowingSwappable
  {
    ThrowingSwappable() {}
    ThrowingSwappable(ThrowingSwappable&&) noexcept(false) {}
    ThrowingSwappable& operator=(ThrowingSwappable&&) noexcept(false)
    {
      return *this;
    }
  };
} // namespace

namespace adl
{
  // Not movable, so only the swap found by argument dependent lookup makes
  // this type swappable.
  struct AdlSwappable
  {
    AdlSwappable(const AdlSwappable&)            = delete;
    AdlSwappable& operator=(const AdlSwappable&) = delete;

    int a;
  };

  inline void swap(AdlSwappable&, AdlSwappable&) {}

  // As above, but the swap found by argument dependent lookup cannot throw.
  struct NothrowAdlSwappable
  {
    NothrowAdlSwappable(const NothrowAdlSwappable&)            = delete;
    NothrowAdlSwappable& operator=(const NothrowAdlSwappable&) = delete;

    int a;
  };

  inline void swap(NothrowAdlSwappable&, NothrowAdlSwappable&) noexcept {}

  // Types for the is_swappable_with tests.
  struct SwapLhs
  {
    int a;
  };

  struct SwapRhs
  {
    int a;
  };

  inline void swap(SwapLhs&, SwapRhs&) {}
  inline void swap(SwapRhs&, SwapLhs&) {}

  // Only swappable in one direction, so is_swappable_with is false.
  struct SwapOneWay
  {
    int a;
  };

  inline void swap(SwapLhs&, SwapOneWay&) {}

  // Swappable only as rvalues.
  struct RvalueSwappable
  {
    int a;
  };

  inline void swap(RvalueSwappable&&, RvalueSwappable&&) {}

  // Types for the is_nothrow_swappable_with tests.
  // Swappable with each other, in both directions, without throwing.
  struct NothrowSwapLhs
  {
    int a;
  };

  struct NothrowSwapRhs
  {
    int a;
  };

  inline void swap(NothrowSwapLhs&, NothrowSwapRhs&) noexcept {}
  inline void swap(NothrowSwapRhs&, NothrowSwapLhs&) noexcept {}

  // Swappable with NothrowSwapLhs in both directions, but one of the swaps may throw.
  struct HalfNothrowSwap
  {
    int a;
  };

  inline void swap(NothrowSwapLhs&, HalfNothrowSwap&) noexcept {}
  inline void swap(HalfNothrowSwap&, NothrowSwapLhs&) {}
} // namespace adl
#endif

namespace
{
  //*************************************************************************
  // std::is_trivial is deprecated in C++26.
  // Use the suggested replacement when compiling to that standard.
  //*************************************************************************
  template <typename T>
  struct std_is_trivial
#if ETL_USING_CPP26
    : std::integral_constant<bool, std::is_trivially_copyable<T>::value && std::is_trivially_default_constructible<T>::value>
#else
    : std::integral_constant<bool, std::is_trivial<T>::value>
#endif
  {
  };

  SUITE(test_type_traits)
  {
    //*************************************************************************
    TEST(test_is_integral)
    {
      CHECK(etl::is_integral<bool>::value == std::is_integral<bool>::value);
      CHECK(etl::is_integral<char>::value == std::is_integral<char>::value);
      CHECK(etl::is_integral<signed char>::value == std::is_integral<signed char>::value);
      CHECK(etl::is_integral<unsigned char>::value == std::is_integral<unsigned char>::value);
      CHECK(etl::is_integral<wchar_t>::value == std::is_integral<wchar_t>::value);
      CHECK(etl::is_integral<short>::value == std::is_integral<short>::value);
      CHECK(etl::is_integral<signed short>::value == std::is_integral<signed short>::value);
      CHECK(etl::is_integral<unsigned short>::value == std::is_integral<unsigned short>::value);
      CHECK(etl::is_integral<int>::value == std::is_integral<int>::value);
      CHECK(etl::is_integral<signed int>::value == std::is_integral<signed int>::value);
      CHECK(etl::is_integral<unsigned int>::value == std::is_integral<unsigned int>::value);
      CHECK(etl::is_integral<long>::value == std::is_integral<long>::value);
      CHECK(etl::is_integral<signed long>::value == std::is_integral<signed long>::value);
      CHECK(etl::is_integral<unsigned long>::value == std::is_integral<unsigned long>::value);
      CHECK(etl::is_integral<long long>::value == std::is_integral<long long>::value);
      CHECK(etl::is_integral<signed long long>::value == std::is_integral<signed long long>::value);
      CHECK(etl::is_integral<unsigned long long>::value == std::is_integral<unsigned long long>::value);
      CHECK(etl::is_integral<const int>::value == std::is_integral<const int>::value);
      CHECK(etl::is_integral<volatile int>::value == std::is_integral<volatile int>::value);
      CHECK(etl::is_integral<const int>::value == std::is_integral<const int>::value);
      CHECK(etl::is_integral<const volatile int>::value == std::is_integral<const volatile int>::value);
      CHECK(etl::is_integral<float>::value == std::is_integral<float>::value);
      CHECK(etl::is_integral<double>::value == std::is_integral<double>::value);
      CHECK(etl::is_integral<long double>::value == std::is_integral<long double>::value);
      CHECK(etl::is_integral<Test>::value == std::is_integral<Test>::value);
    }

    //*************************************************************************
    TEST(test_is_signed)
    {
      CHECK(etl::is_signed<bool>::value == std::is_signed<bool>::value);
      CHECK(etl::is_signed<char>::value == std::is_signed<char>::value);
      CHECK(etl::is_signed<signed char>::value == std::is_signed<signed char>::value);
      CHECK(etl::is_signed<unsigned char>::value == std::is_signed<unsigned char>::value);
      CHECK(etl::is_signed<wchar_t>::value == std::is_signed<wchar_t>::value);
      CHECK(etl::is_signed<short>::value == std::is_signed<short>::value);
      CHECK(etl::is_signed<signed short>::value == std::is_signed<signed short>::value);
      CHECK(etl::is_signed<unsigned short>::value == std::is_signed<unsigned short>::value);
      CHECK(etl::is_signed<int>::value == std::is_signed<int>::value);
      CHECK(etl::is_signed<signed int>::value == std::is_signed<signed int>::value);
      CHECK(etl::is_signed<unsigned int>::value == std::is_signed<unsigned int>::value);
      CHECK(etl::is_signed<long>::value == std::is_signed<long>::value);
      CHECK(etl::is_signed<signed long>::value == std::is_signed<signed long>::value);
      CHECK(etl::is_signed<unsigned long>::value == std::is_signed<unsigned long>::value);
      CHECK(etl::is_signed<long long>::value == std::is_signed<long long>::value);
      CHECK(etl::is_signed<signed long long>::value == std::is_signed<signed long long>::value);
      CHECK(etl::is_signed<unsigned long long>::value == std::is_signed<unsigned long long>::value);
      CHECK(etl::is_signed<const int>::value == std::is_signed<const int>::value);
      CHECK(etl::is_signed<volatile int>::value == std::is_signed<volatile int>::value);
      CHECK(etl::is_signed<const int>::value == std::is_signed<const int>::value);
      CHECK(etl::is_signed<const volatile int>::value == std::is_signed<const volatile int>::value);
      CHECK(etl::is_signed<float>::value == std::is_signed<float>::value);
      CHECK(etl::is_signed<double>::value == std::is_signed<double>::value);
      CHECK(etl::is_signed<long double>::value == std::is_signed<long double>::value);
      CHECK(etl::is_signed<Test>::value == std::is_signed<Test>::value);
    }

    //*************************************************************************
    TEST(test_is_unsigned)
    {
      CHECK(etl::is_unsigned<bool>::value == std::is_unsigned<bool>::value);
      CHECK(etl::is_unsigned<char>::value == std::is_unsigned<char>::value);
      CHECK(etl::is_unsigned<signed char>::value == std::is_unsigned<signed char>::value);
      CHECK(etl::is_unsigned<unsigned char>::value == std::is_unsigned<unsigned char>::value);
      CHECK(etl::is_unsigned<signed char>::value == std::is_unsigned<signed char>::value);
      CHECK(etl::is_unsigned<wchar_t>::value == std::is_unsigned<wchar_t>::value);
      CHECK(etl::is_unsigned<short>::value == std::is_unsigned<short>::value);
      CHECK(etl::is_unsigned<signed short>::value == std::is_unsigned<signed short>::value);
      CHECK(etl::is_unsigned<unsigned short>::value == std::is_unsigned<unsigned short>::value);
      CHECK(etl::is_unsigned<int>::value == std::is_unsigned<int>::value);
      CHECK(etl::is_unsigned<signed int>::value == std::is_unsigned<signed int>::value);
      CHECK(etl::is_unsigned<unsigned int>::value == std::is_unsigned<unsigned int>::value);
      CHECK(etl::is_unsigned<long>::value == std::is_unsigned<long>::value);
      CHECK(etl::is_unsigned<signed long>::value == std::is_unsigned<signed long>::value);
      CHECK(etl::is_unsigned<unsigned long>::value == std::is_unsigned<unsigned long>::value);
      CHECK(etl::is_unsigned<long long>::value == std::is_unsigned<long long>::value);
      CHECK(etl::is_unsigned<signed long long>::value == std::is_unsigned<signed long long>::value);
      CHECK(etl::is_unsigned<unsigned long long>::value == std::is_unsigned<unsigned long long>::value);
      CHECK(etl::is_unsigned<const int>::value == std::is_unsigned<const int>::value);
      CHECK(etl::is_unsigned<volatile int>::value == std::is_unsigned<volatile int>::value);
      CHECK(etl::is_unsigned<const int>::value == std::is_unsigned<const int>::value);
      CHECK(etl::is_unsigned<const volatile int>::value == std::is_unsigned<const volatile int>::value);
      CHECK(etl::is_unsigned<float>::value == std::is_unsigned<float>::value);
      CHECK(etl::is_unsigned<double>::value == std::is_unsigned<double>::value);
      CHECK(etl::is_unsigned<long double>::value == std::is_unsigned<long double>::value);
      CHECK(etl::is_unsigned<Test>::value == std::is_unsigned<Test>::value);
    }

    //*************************************************************************
    TEST(test_is_floating_point)
    {
      CHECK(etl::is_floating_point<bool>::value == std::is_floating_point<bool>::value);
      CHECK(etl::is_floating_point<char>::value == std::is_floating_point<char>::value);
      CHECK(etl::is_floating_point<signed char>::value == std::is_floating_point<signed char>::value);
      CHECK(etl::is_floating_point<unsigned char>::value == std::is_floating_point<unsigned char>::value);
      CHECK(etl::is_floating_point<wchar_t>::value == std::is_floating_point<wchar_t>::value);
      CHECK(etl::is_floating_point<short>::value == std::is_floating_point<short>::value);
      CHECK(etl::is_floating_point<signed short>::value == std::is_floating_point<signed short>::value);
      CHECK(etl::is_floating_point<unsigned short>::value == std::is_floating_point<unsigned short>::value);
      CHECK(etl::is_floating_point<int>::value == std::is_floating_point<int>::value);
      CHECK(etl::is_floating_point<signed int>::value == std::is_floating_point<signed int>::value);
      CHECK(etl::is_floating_point<unsigned int>::value == std::is_floating_point<unsigned int>::value);
      CHECK(etl::is_floating_point<long>::value == std::is_floating_point<long>::value);
      CHECK(etl::is_floating_point<signed long>::value == std::is_floating_point<signed long>::value);
      CHECK(etl::is_floating_point<unsigned long>::value == std::is_floating_point<unsigned long>::value);
      CHECK(etl::is_floating_point<long long>::value == std::is_floating_point<long long>::value);
      CHECK(etl::is_floating_point<signed long long>::value == std::is_floating_point<signed long long>::value);
      CHECK(etl::is_floating_point<unsigned long long>::value == std::is_floating_point<unsigned long long>::value);
      CHECK(etl::is_floating_point<const int>::value == std::is_floating_point<const int>::value);
      CHECK(etl::is_floating_point<volatile int>::value == std::is_floating_point<volatile int>::value);
      CHECK(etl::is_floating_point<const int>::value == std::is_floating_point<const int>::value);
      CHECK(etl::is_floating_point<const volatile int>::value == std::is_floating_point<const volatile int>::value);
      CHECK(etl::is_floating_point<float>::value == std::is_floating_point<float>::value);
      CHECK(etl::is_floating_point<double>::value == std::is_floating_point<double>::value);
      CHECK(etl::is_floating_point<long double>::value == std::is_floating_point<long double>::value);
      CHECK(etl::is_floating_point<Test>::value == std::is_signed<Test>::value);
    }

    //*************************************************************************
    TEST(test_is_pointer)
    {
      CHECK(etl::is_pointer<int>::value == std::is_pointer<int>::value);
      CHECK(etl::is_pointer<int*>::value == std::is_pointer<int*>::value);
      CHECK(etl::is_pointer<const int*>::value == std::is_pointer<const int*>::value);
      CHECK(etl::is_pointer<volatile int*>::value == std::is_pointer<volatile int*>::value);
      CHECK(etl::is_pointer<const volatile int*>::value == std::is_pointer<const volatile int*>::value);
    }

    //*************************************************************************
    TEST(test_is_bounded_array)
    {
      CHECK(etl::is_bounded_array<int[3]>::value);
      CHECK(etl::is_bounded_array<const int[3]>::value);
      CHECK(etl::is_bounded_array<int[3][4]>::value);
      CHECK(!etl::is_bounded_array<int[]>::value);
      CHECK(!etl::is_bounded_array<int>::value);
      CHECK(!etl::is_bounded_array<int*>::value);
      CHECK(!etl::is_bounded_array<void>::value);

#if ETL_USING_CPP20 && defined(__cpp_lib_bounded_array_traits)
      CHECK(etl::is_bounded_array<int[3]>::value == std::is_bounded_array<int[3]>::value);
      CHECK(etl::is_bounded_array<int[]>::value == std::is_bounded_array<int[]>::value);
      CHECK(etl::is_bounded_array<int>::value == std::is_bounded_array<int>::value);
#endif

#if ETL_USING_CPP17
      CHECK(etl::is_bounded_array_v<int[3]>);
      CHECK(!etl::is_bounded_array_v<int[]>);
      CHECK(!etl::is_bounded_array_v<int>);
#endif
    }

    //*************************************************************************
    TEST(test_is_unbounded_array)
    {
      CHECK(etl::is_unbounded_array<int[]>::value);
      CHECK(etl::is_unbounded_array<const int[]>::value);
      CHECK(etl::is_unbounded_array<int[][4]>::value);
      CHECK(!etl::is_unbounded_array<int[3]>::value);
      CHECK(!etl::is_unbounded_array<int[3][4]>::value);
      CHECK(!etl::is_unbounded_array<int>::value);
      CHECK(!etl::is_unbounded_array<int*>::value);
      CHECK(!etl::is_unbounded_array<void>::value);

#if ETL_USING_CPP20 && defined(__cpp_lib_bounded_array_traits)
      CHECK(etl::is_unbounded_array<int[]>::value == std::is_unbounded_array<int[]>::value);
      CHECK(etl::is_unbounded_array<int[3]>::value == std::is_unbounded_array<int[3]>::value);
      CHECK(etl::is_unbounded_array<int>::value == std::is_unbounded_array<int>::value);
#endif

#if ETL_USING_CPP17
      CHECK(etl::is_unbounded_array_v<int[]>);
      CHECK(!etl::is_unbounded_array_v<int[3]>);
      CHECK(!etl::is_unbounded_array_v<int>);
#endif
    }

    //*************************************************************************
    TEST(test_is_null_pointer)
    {
      CHECK(etl::is_null_pointer<std::nullptr_t>::value);
      CHECK(!etl::is_null_pointer<int>::value);
      CHECK(!etl::is_null_pointer<int*>::value);
      CHECK(!etl::is_null_pointer<void*>::value);

      CHECK(etl::is_null_pointer<const std::nullptr_t>::value);
      CHECK(etl::is_null_pointer<volatile std::nullptr_t>::value);
      CHECK(etl::is_null_pointer<const volatile std::nullptr_t>::value);

#if ETL_USING_CPP14
      CHECK(etl::is_null_pointer<std::nullptr_t>::value == std::is_null_pointer<std::nullptr_t>::value);
      CHECK(etl::is_null_pointer<int>::value == std::is_null_pointer<int>::value);
#endif

#if ETL_USING_CPP17
      CHECK(etl::is_null_pointer_v<std::nullptr_t>);
      CHECK(!etl::is_null_pointer_v<int>);
#endif
    }

    //*************************************************************************
    TEST(test_is_standard_layout)
    {
      struct StandardLayout
      {
        int a;
        int b;
      };

      struct NonStandardLayout
      {
        virtual void foo() {}
        int          a;
      };

      CHECK(etl::is_standard_layout<int>::value == std::is_standard_layout<int>::value);
      CHECK(etl::is_standard_layout<int*>::value == std::is_standard_layout<int*>::value);
      CHECK(etl::is_standard_layout<StandardLayout>::value == std::is_standard_layout<StandardLayout>::value);
      CHECK(etl::is_standard_layout<NonStandardLayout>::value == std::is_standard_layout<NonStandardLayout>::value);
      CHECK(etl::is_standard_layout<StandardLayout>::value);
      CHECK(!etl::is_standard_layout<NonStandardLayout>::value);

#if ETL_USING_CPP17
      CHECK(etl::is_standard_layout_v<StandardLayout>);
      CHECK(!etl::is_standard_layout_v<NonStandardLayout>);
#endif
    }

    //*************************************************************************
    TEST(test_is_trivial)
    {
      struct Trivial
      {
        int a;
        int b;
      };

      struct NonTrivialConstructor
      {
        NonTrivialConstructor()
          : a(1)
        {
        }
        int a;
      };

      struct NonTrivialCopy
      {
        NonTrivialCopy(const NonTrivialCopy&) {}
        int a;
      };

      struct NonTrivialDestructor
      {
        ~NonTrivialDestructor() {}
        int a;
      };

      CHECK(etl::is_trivial<int>::value == std_is_trivial<int>::value);
      CHECK(etl::is_trivial<int*>::value == std_is_trivial<int*>::value);
      CHECK(etl::is_trivial<int[4]>::value == std_is_trivial<int[4]>::value);
      CHECK(etl::is_trivial<const Trivial>::value == std_is_trivial<const Trivial>::value);
      CHECK(etl::is_trivial<Trivial>::value == std_is_trivial<Trivial>::value);
      CHECK(etl::is_trivial<NonTrivialConstructor>::value == std_is_trivial<NonTrivialConstructor>::value);
      CHECK(etl::is_trivial<NonTrivialCopy>::value == std_is_trivial<NonTrivialCopy>::value);
      CHECK(etl::is_trivial<NonTrivialDestructor>::value == std_is_trivial<NonTrivialDestructor>::value);

      CHECK(etl::is_trivial<int>::value);
      CHECK(etl::is_trivial<Trivial>::value);
      CHECK(!etl::is_trivial<NonTrivialConstructor>::value);
      CHECK(!etl::is_trivial<NonTrivialCopy>::value);
      CHECK(!etl::is_trivial<NonTrivialDestructor>::value);

#if ETL_USING_CPP17
      CHECK(etl::is_trivial_v<Trivial>);
      CHECK(!etl::is_trivial_v<NonTrivialConstructor>);
#endif
    }

    //*************************************************************************
#if ETL_USING_BUILTIN_IS_LAYOUT_COMPATIBLE || (ETL_USING_STL && ETL_USING_CPP20 && defined(__cpp_lib_is_layout_compatible))
    TEST(test_is_layout_compatible)
    {
      struct A
      {
        int a;
      };

      struct B
      {
        const int b;
      };

      struct C
      {
        int c1;
        int c2;
      };

      struct D
      {
        char d;
      };

      CHECK((etl::is_layout_compatible<int, int>::value));
      CHECK((etl::is_layout_compatible<int, const int>::value));
      CHECK((etl::is_layout_compatible<A, A>::value));
      CHECK((etl::is_layout_compatible<A, B>::value));
      CHECK((!etl::is_layout_compatible<A, C>::value));
      CHECK((!etl::is_layout_compatible<A, D>::value));
      CHECK((!etl::is_layout_compatible<int, char>::value));

  #if ETL_USING_STL && ETL_USING_CPP20 && defined(__cpp_lib_is_layout_compatible)
      CHECK((etl::is_layout_compatible<A, B>::value == std::is_layout_compatible<A, B>::value));
      CHECK((etl::is_layout_compatible<A, C>::value == std::is_layout_compatible<A, C>::value));
  #endif

  #if ETL_USING_CPP17
      CHECK((etl::is_layout_compatible_v<A, B>));
      CHECK((!etl::is_layout_compatible_v<A, C>));
  #endif
    }
#endif

    //*************************************************************************
#if ETL_USING_BUILTIN_IS_POINTER_INTERCONVERTIBLE_BASE_OF || (ETL_USING_STL && ETL_USING_CPP20 && defined(__cpp_lib_is_pointer_interconvertible))
    TEST(test_is_pointer_interconvertible_base_of)
    {
      struct Base
      {
        int b;
      };

      struct Derived : public Base
      {
      };

      struct VirtualDerived : virtual public Base
      {
      };

      struct Polymorphic
      {
        virtual ~Polymorphic() {}

        int p;
      };

      struct PolymorphicDerived : public Polymorphic
      {
      };

      struct Unrelated
      {
        int u;
      };

      CHECK((etl::is_pointer_interconvertible_base_of<Base, Base>::value));
      CHECK((etl::is_pointer_interconvertible_base_of<Base, Derived>::value));
      CHECK((!etl::is_pointer_interconvertible_base_of<Base, VirtualDerived>::value));
      CHECK((!etl::is_pointer_interconvertible_base_of<Polymorphic, PolymorphicDerived>::value));
      CHECK((!etl::is_pointer_interconvertible_base_of<Base, Unrelated>::value));
      CHECK((!etl::is_pointer_interconvertible_base_of<Derived, Base>::value));

  #if ETL_USING_STL && ETL_USING_CPP20 && defined(__cpp_lib_is_pointer_interconvertible)
      CHECK((etl::is_pointer_interconvertible_base_of<Base, Derived>::value == std::is_pointer_interconvertible_base_of<Base, Derived>::value));
      CHECK((etl::is_pointer_interconvertible_base_of<Base, VirtualDerived>::value
             == std::is_pointer_interconvertible_base_of<Base, VirtualDerived>::value));
  #endif

  #if ETL_USING_CPP17
      CHECK((etl::is_pointer_interconvertible_base_of_v<Base, Derived>));
      CHECK((!etl::is_pointer_interconvertible_base_of_v<Base, VirtualDerived>));
  #endif
    }
#endif

    //*************************************************************************
    TEST(test_is_destructible)
    {
#if ETL_USING_CPP11 || ETL_USING_BUILTIN_IS_DESTRUCTIBLE
      CHECK(etl::is_destructible<int>::value);
      CHECK(etl::is_destructible<int*>::value);
      CHECK(!etl::is_destructible<void>::value);
#endif

#if ETL_USING_CPP11
      struct Destructible
      {
        int a;
      };

      struct NonDestructible
      {
        ~NonDestructible() = delete;
      };

      struct ProtectedDestructor
      {
      protected:

        ~ProtectedDestructor() {}
      };

      CHECK(etl::is_destructible<int>::value == std::is_destructible<int>::value);
      CHECK(etl::is_destructible<void>::value == std::is_destructible<void>::value);
      CHECK(etl::is_destructible<int[2]>::value == std::is_destructible<int[2]>::value);
      CHECK(etl::is_destructible<int[]>::value == std::is_destructible<int[]>::value);
      CHECK(etl::is_destructible<int&>::value == std::is_destructible<int&>::value);
      CHECK(etl::is_destructible<int(int)>::value == std::is_destructible<int(int)>::value);
      CHECK(etl::is_destructible<Destructible>::value == std::is_destructible<Destructible>::value);
      CHECK(etl::is_destructible<NonDestructible>::value == std::is_destructible<NonDestructible>::value);
      CHECK(etl::is_destructible<ProtectedDestructor>::value == std::is_destructible<ProtectedDestructor>::value);

      CHECK(etl::is_destructible<Destructible>::value);
      CHECK(etl::is_destructible<Destructible[2]>::value);
      CHECK(etl::is_destructible<int&>::value);
      CHECK(!etl::is_destructible<NonDestructible>::value);
      CHECK(!etl::is_destructible<ProtectedDestructor>::value);
      CHECK(!etl::is_destructible<int[]>::value);
      CHECK(!etl::is_destructible<int(int)>::value);
#endif

#if ETL_USING_CPP17
      CHECK(etl::is_destructible_v<int>);
      CHECK(!etl::is_destructible_v<void>);
#endif
    }

    //*************************************************************************
    TEST(test_is_nothrow_destructible)
    {
#if ETL_USING_CPP11 || ETL_USING_BUILTIN_IS_NOTHROW_DESTRUCTIBLE
      CHECK(etl::is_nothrow_destructible<int>::value);
      CHECK(etl::is_nothrow_destructible<int*>::value);
      CHECK(!etl::is_nothrow_destructible<void>::value);
#endif

#if ETL_USING_CPP11
      struct NothrowDestructible
      {
        int a;
      };

      struct ThrowingDestructible
      {
        ~ThrowingDestructible() noexcept(false) {}
      };

      struct NotDestructible
      {
        ~NotDestructible() = delete;
      };

      CHECK(etl::is_nothrow_destructible<int>::value == std::is_nothrow_destructible<int>::value);
      CHECK(etl::is_nothrow_destructible<void>::value == std::is_nothrow_destructible<void>::value);
      CHECK(etl::is_nothrow_destructible<int[2]>::value == std::is_nothrow_destructible<int[2]>::value);
      CHECK(etl::is_nothrow_destructible<int[]>::value == std::is_nothrow_destructible<int[]>::value);
      CHECK(etl::is_nothrow_destructible<int&>::value == std::is_nothrow_destructible<int&>::value);
      CHECK(etl::is_nothrow_destructible<int(int)>::value == std::is_nothrow_destructible<int(int)>::value);
      CHECK(etl::is_nothrow_destructible<NothrowDestructible>::value == std::is_nothrow_destructible<NothrowDestructible>::value);
      CHECK(etl::is_nothrow_destructible<ThrowingDestructible>::value == std::is_nothrow_destructible<ThrowingDestructible>::value);
      CHECK(etl::is_nothrow_destructible<NotDestructible>::value == std::is_nothrow_destructible<NotDestructible>::value);

      CHECK(etl::is_nothrow_destructible<NothrowDestructible>::value);
      CHECK(etl::is_nothrow_destructible<NothrowDestructible[2]>::value);
      CHECK(etl::is_nothrow_destructible<int&>::value);
      CHECK(!etl::is_nothrow_destructible<ThrowingDestructible>::value);
      CHECK(!etl::is_nothrow_destructible<ThrowingDestructible[2]>::value);
      CHECK(!etl::is_nothrow_destructible<NotDestructible>::value);
      CHECK(!etl::is_nothrow_destructible<int[]>::value);
      CHECK(!etl::is_nothrow_destructible<int(int)>::value);
#endif

#if ETL_USING_CPP17
      CHECK(etl::is_nothrow_destructible_v<int>);
      CHECK(!etl::is_nothrow_destructible_v<void>);
#endif
    }

#if ETL_USING_CPP11
    //*************************************************************************
    TEST(test_is_swappable)
    {
      CHECK(etl::is_swappable<int>::value);
      CHECK(etl::is_swappable<int*>::value);
      CHECK(etl::is_swappable<Swappable>::value);
      CHECK(etl::is_swappable<Swappable[2]>::value);
      CHECK(etl::is_swappable<Swappable[2][3]>::value);

      // Not movable, so swappable only via the swap found by ADL.
      CHECK(etl::is_swappable<adl::AdlSwappable>::value);
      CHECK(etl::is_swappable<adl::AdlSwappable[2]>::value);

      CHECK(!etl::is_swappable<NonSwappable>::value);
      CHECK(!etl::is_swappable<NonSwappable[2]>::value);
      CHECK(!etl::is_swappable<void>::value);
      CHECK(!etl::is_swappable<const int>::value);
      CHECK(!etl::is_swappable<int(int)>::value);
      CHECK(!etl::is_swappable<int[]>::value);

  #if ETL_USING_CPP17
      CHECK(etl::is_swappable<int>::value == std::is_swappable<int>::value);
      CHECK(etl::is_swappable<int*>::value == std::is_swappable<int*>::value);
      CHECK(etl::is_swappable<int[2]>::value == std::is_swappable<int[2]>::value);
      CHECK(etl::is_swappable<int[]>::value == std::is_swappable<int[]>::value);
      CHECK(etl::is_swappable<int&>::value == std::is_swappable<int&>::value);
      CHECK(etl::is_swappable<const int>::value == std::is_swappable<const int>::value);
      CHECK(etl::is_swappable<void>::value == std::is_swappable<void>::value);
      CHECK(etl::is_swappable<int(int)>::value == std::is_swappable<int(int)>::value);
      CHECK(etl::is_swappable<Swappable>::value == std::is_swappable<Swappable>::value);
      CHECK(etl::is_swappable<Swappable[2]>::value == std::is_swappable<Swappable[2]>::value);
      CHECK(etl::is_swappable<NonSwappable>::value == std::is_swappable<NonSwappable>::value);
      CHECK(etl::is_swappable<NonSwappable[2]>::value == std::is_swappable<NonSwappable[2]>::value);
      CHECK(etl::is_swappable<adl::AdlSwappable>::value == std::is_swappable<adl::AdlSwappable>::value);

      CHECK(etl::is_swappable_v<int>);
      CHECK(!etl::is_swappable_v<void>);
  #endif
    }

    //*************************************************************************
    TEST(test_is_swappable_with)
    {
      // Note that is_swappable_with does not add references, so the value
      // categories of the arguments are those of T and U.
      CHECK((etl::is_swappable_with<int&, int&>::value));
      CHECK((etl::is_swappable_with<Swappable&, Swappable&>::value));
      CHECK((etl::is_swappable_with<Swappable(&)[2], Swappable(&)[2]>::value));
      CHECK((etl::is_swappable_with<adl::AdlSwappable&, adl::AdlSwappable&>::value));

      // Heterogeneous swaps, found by argument dependent lookup.
      CHECK((etl::is_swappable_with<adl::SwapLhs&, adl::SwapRhs&>::value));
      CHECK((etl::is_swappable_with<adl::SwapRhs&, adl::SwapLhs&>::value));

      // Swappable in one direction only.
      CHECK((!etl::is_swappable_with<adl::SwapLhs&, adl::SwapOneWay&>::value));
      CHECK((!etl::is_swappable_with<adl::SwapOneWay&, adl::SwapLhs&>::value));

      // Swappable as rvalues, via the swap found by ADL.
      CHECK((etl::is_swappable_with<adl::RvalueSwappable, adl::RvalueSwappable>::value));

      // Rvalues do not bind to the lvalue references taken by swap, unless the
      // compiler allows it as an extension.
      CHECK((!etl::is_swappable_with<int, int>::value));
      CHECK((etl::is_swappable_with<Swappable, Swappable>::value == rvalue_binds_to_lvalue_reference<Swappable>::value));

      CHECK((!etl::is_swappable_with<NonSwappable&, NonSwappable&>::value));
      CHECK((!etl::is_swappable_with<int&, Swappable&>::value));
      CHECK((!etl::is_swappable_with<const int&, const int&>::value));
      CHECK((!etl::is_swappable_with<void, void>::value));
      CHECK((!etl::is_swappable_with<int (&)(int), int (&)(int)>::value));

  #if ETL_USING_CPP17
      CHECK((etl::is_swappable_with<int&, int&>::value == std::is_swappable_with<int&, int&>::value));
      CHECK((etl::is_swappable_with<int, int>::value == std::is_swappable_with<int, int>::value));
      CHECK((etl::is_swappable_with<int&, Swappable&>::value == std::is_swappable_with<int&, Swappable&>::value));
      CHECK((etl::is_swappable_with<const int&, const int&>::value == std::is_swappable_with<const int&, const int&>::value));
      CHECK((etl::is_swappable_with<void, void>::value == std::is_swappable_with<void, void>::value));
      CHECK((etl::is_swappable_with<NonSwappable&, NonSwappable&>::value == std::is_swappable_with<NonSwappable&, NonSwappable&>::value));
      CHECK((etl::is_swappable_with<adl::SwapLhs&, adl::SwapRhs&>::value == std::is_swappable_with<adl::SwapLhs&, adl::SwapRhs&>::value));
      CHECK((etl::is_swappable_with<adl::SwapLhs&, adl::SwapOneWay&>::value == std::is_swappable_with<adl::SwapLhs&, adl::SwapOneWay&>::value));
      CHECK((etl::is_swappable_with<adl::RvalueSwappable, adl::RvalueSwappable>::value
             == std::is_swappable_with<adl::RvalueSwappable, adl::RvalueSwappable>::value));

      CHECK((etl::is_swappable_with_v<int&, int&>));
      CHECK((!etl::is_swappable_with_v<int, int>));
  #endif
    }

    //*************************************************************************
    TEST(test_is_nothrow_swappable)
    {
      CHECK(etl::is_nothrow_swappable<int>::value);
      CHECK(etl::is_nothrow_swappable<int*>::value);
      CHECK(etl::is_nothrow_swappable<Swappable>::value);
      CHECK(etl::is_nothrow_swappable<Swappable[2]>::value);
      CHECK(etl::is_nothrow_swappable<Swappable[2][3]>::value);

      // Not movable, so swappable only via the swap found by ADL, which is noexcept.
      CHECK(etl::is_nothrow_swappable<adl::NothrowAdlSwappable>::value);
      CHECK(etl::is_nothrow_swappable<adl::NothrowAdlSwappable[2]>::value);

      // Swappable, but the swap may throw.
      CHECK(!etl::is_nothrow_swappable<ThrowingSwappable>::value);
      CHECK(!etl::is_nothrow_swappable<ThrowingSwappable[2]>::value);

      // Swappable via the swap found by ADL, which is not noexcept.
      CHECK(!etl::is_nothrow_swappable<adl::AdlSwappable>::value);
      CHECK(!etl::is_nothrow_swappable<adl::AdlSwappable[2]>::value);

      // Not swappable at all.
      CHECK(!etl::is_nothrow_swappable<NonSwappable>::value);
      CHECK(!etl::is_nothrow_swappable<NonSwappable[2]>::value);
      CHECK(!etl::is_nothrow_swappable<void>::value);
      CHECK(!etl::is_nothrow_swappable<const int>::value);
      CHECK(!etl::is_nothrow_swappable<int(int)>::value);
      CHECK(!etl::is_nothrow_swappable<int[]>::value);

  #if ETL_USING_CPP17
      CHECK(etl::is_nothrow_swappable<int>::value == std::is_nothrow_swappable<int>::value);
      CHECK(etl::is_nothrow_swappable<int*>::value == std::is_nothrow_swappable<int*>::value);
      CHECK(etl::is_nothrow_swappable<int[2]>::value == std::is_nothrow_swappable<int[2]>::value);
      CHECK(etl::is_nothrow_swappable<int[]>::value == std::is_nothrow_swappable<int[]>::value);
      CHECK(etl::is_nothrow_swappable<int&>::value == std::is_nothrow_swappable<int&>::value);
      CHECK(etl::is_nothrow_swappable<const int>::value == std::is_nothrow_swappable<const int>::value);
      CHECK(etl::is_nothrow_swappable<void>::value == std::is_nothrow_swappable<void>::value);
      CHECK(etl::is_nothrow_swappable<int(int)>::value == std::is_nothrow_swappable<int(int)>::value);
      CHECK(etl::is_nothrow_swappable<Swappable>::value == std::is_nothrow_swappable<Swappable>::value);
      CHECK(etl::is_nothrow_swappable<Swappable[2]>::value == std::is_nothrow_swappable<Swappable[2]>::value);
      CHECK(etl::is_nothrow_swappable<ThrowingSwappable>::value == std::is_nothrow_swappable<ThrowingSwappable>::value);
      CHECK(etl::is_nothrow_swappable<ThrowingSwappable[2]>::value == std::is_nothrow_swappable<ThrowingSwappable[2]>::value);
      CHECK(etl::is_nothrow_swappable<NonSwappable>::value == std::is_nothrow_swappable<NonSwappable>::value);
      CHECK(etl::is_nothrow_swappable<adl::AdlSwappable>::value == std::is_nothrow_swappable<adl::AdlSwappable>::value);
      CHECK(etl::is_nothrow_swappable<adl::NothrowAdlSwappable>::value == std::is_nothrow_swappable<adl::NothrowAdlSwappable>::value);

      CHECK(etl::is_nothrow_swappable_v<int>);
      CHECK(!etl::is_nothrow_swappable_v<void>);
  #endif
    }

    //*************************************************************************
    TEST(test_is_nothrow_swappable_with)
    {
      CHECK((etl::is_nothrow_swappable_with<int&, int&>::value));
      CHECK((etl::is_nothrow_swappable_with<Swappable&, Swappable&>::value));

      // Swappable with each other, in both directions, without throwing.
      CHECK((etl::is_nothrow_swappable_with<adl::NothrowSwapLhs&, adl::NothrowSwapRhs&>::value));
      CHECK((etl::is_nothrow_swappable_with<adl::NothrowSwapRhs&, adl::NothrowSwapLhs&>::value));

      // Swappable in both directions, but one of the swaps may throw.
      CHECK((!etl::is_nothrow_swappable_with<adl::NothrowSwapLhs&, adl::HalfNothrowSwap&>::value));
      CHECK((!etl::is_nothrow_swappable_with<adl::HalfNothrowSwap&, adl::NothrowSwapLhs&>::value));

      // Swappable with each other, but the swaps may throw.
      CHECK((!etl::is_nothrow_swappable_with<adl::SwapLhs&, adl::SwapRhs&>::value));

      // Swappable, but the move operations may throw.
      CHECK((!etl::is_nothrow_swappable_with<ThrowingSwappable&, ThrowingSwappable&>::value));

      // Rvalues do not bind to the lvalue references taken by swap.
      CHECK((!etl::is_nothrow_swappable_with<int, int>::value));

      // Not swappable at all.
      CHECK((!etl::is_nothrow_swappable_with<NonSwappable&, NonSwappable&>::value));
      CHECK((!etl::is_nothrow_swappable_with<int&, Swappable&>::value));
      CHECK((!etl::is_nothrow_swappable_with<const int&, const int&>::value));
      CHECK((!etl::is_nothrow_swappable_with<void, void>::value));
      CHECK((!etl::is_nothrow_swappable_with<int (&)(int), int (&)(int)>::value));

  #if ETL_USING_CPP17
      CHECK((etl::is_nothrow_swappable_with<int&, int&>::value == std::is_nothrow_swappable_with<int&, int&>::value));
      CHECK((etl::is_nothrow_swappable_with<int, int>::value == std::is_nothrow_swappable_with<int, int>::value));
      CHECK((etl::is_nothrow_swappable_with<void, void>::value == std::is_nothrow_swappable_with<void, void>::value));
      CHECK((etl::is_nothrow_swappable_with<const int&, const int&>::value == std::is_nothrow_swappable_with<const int&, const int&>::value));
      CHECK(
        (etl::is_nothrow_swappable_with<NonSwappable&, NonSwappable&>::value == std::is_nothrow_swappable_with<NonSwappable&, NonSwappable&>::value));
      CHECK((etl::is_nothrow_swappable_with<ThrowingSwappable&, ThrowingSwappable&>::value
             == std::is_nothrow_swappable_with<ThrowingSwappable&, ThrowingSwappable&>::value));
      CHECK(
        (etl::is_nothrow_swappable_with<adl::SwapLhs&, adl::SwapRhs&>::value == std::is_nothrow_swappable_with<adl::SwapLhs&, adl::SwapRhs&>::value));
      CHECK((etl::is_nothrow_swappable_with<adl::NothrowSwapLhs&, adl::NothrowSwapRhs&>::value
             == std::is_nothrow_swappable_with<adl::NothrowSwapLhs&, adl::NothrowSwapRhs&>::value));
      CHECK((etl::is_nothrow_swappable_with<adl::NothrowSwapLhs&, adl::HalfNothrowSwap&>::value
             == std::is_nothrow_swappable_with<adl::NothrowSwapLhs&, adl::HalfNothrowSwap&>::value));

      CHECK((etl::is_nothrow_swappable_with_v<int&, int&>));
      CHECK((!etl::is_nothrow_swappable_with_v<int, int>));
  #endif
    }
#endif

    //*************************************************************************
    TEST(test_is_union)
    {
      union Union
      {
        int   a;
        float b;
      };

      struct NonUnion
      {
        int a;
        int b;
      };

      CHECK(etl::is_union<int>::value == std::is_union<int>::value);
      CHECK(etl::is_union<int*>::value == std::is_union<int*>::value);
      CHECK(etl::is_union<Union>::value == std::is_union<Union>::value);
      CHECK(etl::is_union<NonUnion>::value == std::is_union<NonUnion>::value);
      CHECK(etl::is_union<Union>::value);
      CHECK(!etl::is_union<NonUnion>::value);
      CHECK(!etl::is_union<int>::value);

#if ETL_USING_CPP17
      CHECK(etl::is_union_v<Union>);
      CHECK(!etl::is_union_v<NonUnion>);
#endif
    }

    //*************************************************************************
    TEST(test_is_empty)
    {
      struct Empty
      {
      };

      struct EmptyDerived : public Empty
      {
      };

      struct NotEmpty
      {
        int a;
      };

      struct Virtual
      {
        virtual void foo() {}
      };

      CHECK(etl::is_empty<Empty>::value == std::is_empty<Empty>::value);
      CHECK(etl::is_empty<EmptyDerived>::value == std::is_empty<EmptyDerived>::value);
      CHECK(etl::is_empty<NotEmpty>::value == std::is_empty<NotEmpty>::value);
      CHECK(etl::is_empty<Virtual>::value == std::is_empty<Virtual>::value);
      CHECK(etl::is_empty<int>::value == std::is_empty<int>::value);
      CHECK(etl::is_empty<int*>::value == std::is_empty<int*>::value);

      CHECK(etl::is_empty<Empty>::value);
      CHECK(etl::is_empty<EmptyDerived>::value);
      CHECK(!etl::is_empty<NotEmpty>::value);
      CHECK(!etl::is_empty<Virtual>::value);
      CHECK(!etl::is_empty<int>::value);

#if ETL_USING_CPP17
      CHECK(etl::is_empty_v<Empty>);
      CHECK(!etl::is_empty_v<NotEmpty>);
#endif
    }

    //*************************************************************************
    TEST(test_is_polymorphic)
    {
      struct NonPolymorphic
      {
        int a;
      };

      struct Polymorphic
      {
        virtual ~Polymorphic() {}
      };

      struct PolymorphicDerived : public Polymorphic
      {
      };

      struct AbstractPolymorphic
      {
        virtual void foo() = 0;
      };

      CHECK(etl::is_polymorphic<NonPolymorphic>::value == std::is_polymorphic<NonPolymorphic>::value);
      CHECK(etl::is_polymorphic<Polymorphic>::value == std::is_polymorphic<Polymorphic>::value);
      CHECK(etl::is_polymorphic<PolymorphicDerived>::value == std::is_polymorphic<PolymorphicDerived>::value);
      CHECK(etl::is_polymorphic<AbstractPolymorphic>::value == std::is_polymorphic<AbstractPolymorphic>::value);
      CHECK(etl::is_polymorphic<int>::value == std::is_polymorphic<int>::value);
      CHECK(etl::is_polymorphic<int*>::value == std::is_polymorphic<int*>::value);

      CHECK(etl::is_polymorphic<Polymorphic>::value);
      CHECK(etl::is_polymorphic<PolymorphicDerived>::value);
      CHECK(etl::is_polymorphic<AbstractPolymorphic>::value);
      CHECK(!etl::is_polymorphic<NonPolymorphic>::value);
      CHECK(!etl::is_polymorphic<int>::value);

#if ETL_USING_CPP17
      CHECK(etl::is_polymorphic_v<Polymorphic>);
      CHECK(!etl::is_polymorphic_v<NonPolymorphic>);
#endif
    }

    //*************************************************************************
    TEST(test_is_abstract)
    {
      struct NonAbstract
      {
        int a;
      };

      struct NonAbstractPolymorphic
      {
        virtual ~NonAbstractPolymorphic() {}
      };

      struct Abstract
      {
        virtual ~Abstract() {}
        virtual void foo() = 0;
      };

      struct AbstractDerived : public Abstract
      {
      };

      struct ConcreteDerived : public Abstract
      {
        void foo() ETL_OVERRIDE {}
      };

      CHECK(etl::is_abstract<NonAbstract>::value == std::is_abstract<NonAbstract>::value);
      CHECK(etl::is_abstract<NonAbstractPolymorphic>::value == std::is_abstract<NonAbstractPolymorphic>::value);
      CHECK(etl::is_abstract<Abstract>::value == std::is_abstract<Abstract>::value);
      CHECK(etl::is_abstract<AbstractDerived>::value == std::is_abstract<AbstractDerived>::value);
      CHECK(etl::is_abstract<ConcreteDerived>::value == std::is_abstract<ConcreteDerived>::value);
      CHECK(etl::is_abstract<int>::value == std::is_abstract<int>::value);
      CHECK(etl::is_abstract<int*>::value == std::is_abstract<int*>::value);

      CHECK(etl::is_abstract<Abstract>::value);
      CHECK(etl::is_abstract<AbstractDerived>::value);
      CHECK(!etl::is_abstract<ConcreteDerived>::value);
      CHECK(!etl::is_abstract<NonAbstract>::value);
      CHECK(!etl::is_abstract<NonAbstractPolymorphic>::value);
      CHECK(!etl::is_abstract<int>::value);

#if ETL_USING_CPP17
      CHECK(etl::is_abstract_v<Abstract>);
      CHECK(!etl::is_abstract_v<ConcreteDerived>);
#endif
    }

    //*************************************************************************
#if ETL_USING_CPP14
    TEST(test_is_final)
    {
      struct NonFinal
      {
        int a;
      };

      struct Final final
      {
        int a;
      };

      struct Base
      {
        virtual ~Base() {}
      };

      struct FinalDerived final : public Base
      {
      };

      struct NonFinalDerived : public Base
      {
      };

      union FinalUnion final
      {
        int a;
      };

      CHECK(etl::is_final<NonFinal>::value == std::is_final<NonFinal>::value);
      CHECK(etl::is_final<Final>::value == std::is_final<Final>::value);
      CHECK(etl::is_final<FinalDerived>::value == std::is_final<FinalDerived>::value);
      CHECK(etl::is_final<NonFinalDerived>::value == std::is_final<NonFinalDerived>::value);
      CHECK(etl::is_final<FinalUnion>::value == std::is_final<FinalUnion>::value);
      CHECK(etl::is_final<int>::value == std::is_final<int>::value);
      CHECK(etl::is_final<int*>::value == std::is_final<int*>::value);
      CHECK(etl::is_final<Final*>::value == std::is_final<Final*>::value);

      CHECK(etl::is_final<Final>::value);
      CHECK(etl::is_final<FinalDerived>::value);
      CHECK(etl::is_final<FinalUnion>::value);
      CHECK(!etl::is_final<NonFinal>::value);
      CHECK(!etl::is_final<NonFinalDerived>::value);
      CHECK(!etl::is_final<Base>::value);
      CHECK(!etl::is_final<int>::value);
      CHECK(!etl::is_final<Final*>::value);

  #if ETL_USING_CPP17
      CHECK(etl::is_final_v<Final>);
      CHECK(!etl::is_final_v<NonFinal>);
  #endif
    }
#endif

    //*************************************************************************
#if ETL_USING_CPP17
    TEST(test_is_aggregate)
    {
      struct Aggregate
      {
        int    a;
        double b;
      };

      struct AggregateDerived : public Aggregate
      {
        int c;
      };

      struct NonAggregate
      {
        NonAggregate(int a_)
          : a(a_)
        {
        }
        int a;
      };

      struct Virtual
      {
        virtual ~Virtual() {}
        int a;
      };

      union AggregateUnion
      {
        int    a;
        double b;
      };

      CHECK(etl::is_aggregate<Aggregate>::value == std::is_aggregate<Aggregate>::value);
      CHECK(etl::is_aggregate<AggregateDerived>::value == std::is_aggregate<AggregateDerived>::value);
      CHECK(etl::is_aggregate<NonAggregate>::value == std::is_aggregate<NonAggregate>::value);
      CHECK(etl::is_aggregate<Virtual>::value == std::is_aggregate<Virtual>::value);
      CHECK(etl::is_aggregate<AggregateUnion>::value == std::is_aggregate<AggregateUnion>::value);
      CHECK(etl::is_aggregate<int[4]>::value == std::is_aggregate<int[4]>::value);
      CHECK(etl::is_aggregate<int>::value == std::is_aggregate<int>::value);
      CHECK(etl::is_aggregate<int*>::value == std::is_aggregate<int*>::value);
      CHECK(etl::is_aggregate<const Aggregate>::value == std::is_aggregate<const Aggregate>::value);

      CHECK(etl::is_aggregate<Aggregate>::value);
      CHECK(etl::is_aggregate<AggregateDerived>::value);
      CHECK(etl::is_aggregate<AggregateUnion>::value);
      CHECK(etl::is_aggregate<int[4]>::value);
      CHECK(etl::is_aggregate<const Aggregate>::value);
      CHECK(!etl::is_aggregate<NonAggregate>::value);
      CHECK(!etl::is_aggregate<Virtual>::value);
      CHECK(!etl::is_aggregate<int>::value);
      CHECK(!etl::is_aggregate<int*>::value);

      CHECK(etl::is_aggregate_v<Aggregate>);
      CHECK(!etl::is_aggregate_v<NonAggregate>);
    }
#endif

    //*************************************************************************
    TEST(test_has_virtual_destructor)
    {
      struct NonVirtualDestructor
      {
        ~NonVirtualDestructor() {}
        int a;
      };

      struct VirtualDestructor
      {
        virtual ~VirtualDestructor() {}
        int a;
      };

      struct DerivedFromVirtualDestructor : public VirtualDestructor
      {
        int b;
      };

      struct VirtualFunctionOnly
      {
        virtual void foo() {}
        int          a;
      };

      CHECK(etl::has_virtual_destructor<NonVirtualDestructor>::value == std::has_virtual_destructor<NonVirtualDestructor>::value);
      CHECK(etl::has_virtual_destructor<VirtualDestructor>::value == std::has_virtual_destructor<VirtualDestructor>::value);
      CHECK(etl::has_virtual_destructor<DerivedFromVirtualDestructor>::value == std::has_virtual_destructor<DerivedFromVirtualDestructor>::value);
      CHECK(etl::has_virtual_destructor<VirtualFunctionOnly>::value == std::has_virtual_destructor<VirtualFunctionOnly>::value);
      CHECK(etl::has_virtual_destructor<int>::value == std::has_virtual_destructor<int>::value);
      CHECK(etl::has_virtual_destructor<int*>::value == std::has_virtual_destructor<int*>::value);
      CHECK(etl::has_virtual_destructor<const VirtualDestructor>::value == std::has_virtual_destructor<const VirtualDestructor>::value);

      CHECK(etl::has_virtual_destructor<VirtualDestructor>::value);
      CHECK(etl::has_virtual_destructor<DerivedFromVirtualDestructor>::value);
      CHECK(etl::has_virtual_destructor<const VirtualDestructor>::value);
      CHECK(!etl::has_virtual_destructor<NonVirtualDestructor>::value);
      CHECK(!etl::has_virtual_destructor<VirtualFunctionOnly>::value);
      CHECK(!etl::has_virtual_destructor<int>::value);
      CHECK(!etl::has_virtual_destructor<int*>::value);

#if ETL_USING_CPP17
      CHECK(etl::has_virtual_destructor_v<VirtualDestructor>);
      CHECK(!etl::has_virtual_destructor_v<NonVirtualDestructor>);
#endif
    }

    //*************************************************************************
#if ETL_USING_CPP17
    TEST(test_has_unique_object_representations)
    {
      struct NoPadding
      {
        int a;
        int b;
      };

      struct Padded
      {
        char a;
        int  b;
      };

      CHECK(etl::has_unique_object_representations<int>::value == std::has_unique_object_representations<int>::value);
      CHECK(etl::has_unique_object_representations<int*>::value == std::has_unique_object_representations<int*>::value);
      CHECK(etl::has_unique_object_representations<float>::value == std::has_unique_object_representations<float>::value);
      CHECK(etl::has_unique_object_representations<NoPadding>::value == std::has_unique_object_representations<NoPadding>::value);
      CHECK(etl::has_unique_object_representations<Padded>::value == std::has_unique_object_representations<Padded>::value);
      CHECK(etl::has_unique_object_representations<int[4]>::value == std::has_unique_object_representations<int[4]>::value);
      CHECK(etl::has_unique_object_representations<const int>::value == std::has_unique_object_representations<const int>::value);
      CHECK(etl::has_unique_object_representations<const NoPadding>::value == std::has_unique_object_representations<const NoPadding>::value);

      CHECK(etl::has_unique_object_representations<int>::value);
      CHECK(etl::has_unique_object_representations<const int>::value);
      CHECK(etl::has_unique_object_representations<int[4]>::value);
      CHECK(etl::has_unique_object_representations<NoPadding>::value);
      CHECK(!etl::has_unique_object_representations<Padded>::value);

      CHECK(etl::has_unique_object_representations_v<NoPadding>);
      CHECK(!etl::has_unique_object_representations_v<Padded>);
    }
#endif

    //*************************************************************************
    TEST(test_is_scalar)
    {
      struct Class
      {
        int  a;
        void foo() {}
      };

      union Union
      {
        int   a;
        float b;
      };

      enum Enum
      {
        e1
      };

      // Arithmetic types.
      CHECK(etl::is_scalar<bool>::value == std::is_scalar<bool>::value);
      CHECK(etl::is_scalar<char>::value == std::is_scalar<char>::value);
      CHECK(etl::is_scalar<int>::value == std::is_scalar<int>::value);
      CHECK(etl::is_scalar<double>::value == std::is_scalar<double>::value);
      CHECK(etl::is_scalar<const int>::value == std::is_scalar<const int>::value);
      CHECK(etl::is_scalar<volatile int>::value == std::is_scalar<volatile int>::value);
      CHECK(etl::is_scalar<const volatile int>::value == std::is_scalar<const volatile int>::value);

      // Enumerations.
      CHECK(etl::is_scalar<Enum>::value == std::is_scalar<Enum>::value);
      CHECK(etl::is_scalar<const Enum>::value == std::is_scalar<const Enum>::value);

      // Pointers.
      CHECK(etl::is_scalar<int*>::value == std::is_scalar<int*>::value);
      CHECK(etl::is_scalar<void*>::value == std::is_scalar<void*>::value);
      CHECK(etl::is_scalar<const Class*>::value == std::is_scalar<const Class*>::value);
      CHECK(etl::is_scalar<int* const>::value == std::is_scalar<int* const>::value);

      // Pointers to members.
      CHECK(etl::is_scalar<int Class::*>::value == std::is_scalar<int Class::*>::value);
      CHECK(etl::is_scalar<void (Class::*)()>::value == std::is_scalar<void (Class::*)()>::value);
      CHECK(etl::is_scalar<const int Class::*>::value == std::is_scalar<const int Class::*>::value);

      // Null pointer.
      CHECK(etl::is_scalar<std::nullptr_t>::value == std::is_scalar<std::nullptr_t>::value);
      CHECK(etl::is_scalar<const std::nullptr_t>::value == std::is_scalar<const std::nullptr_t>::value);

      // Non-scalars.
      CHECK(etl::is_scalar<Class>::value == std::is_scalar<Class>::value);
      CHECK(etl::is_scalar<Union>::value == std::is_scalar<Union>::value);
      CHECK(etl::is_scalar<void>::value == std::is_scalar<void>::value);
      CHECK(etl::is_scalar<int&>::value == std::is_scalar<int&>::value);
      CHECK(etl::is_scalar<int&&>::value == std::is_scalar<int&&>::value);
      CHECK(etl::is_scalar<int[3]>::value == std::is_scalar<int[3]>::value);
      CHECK(etl::is_scalar<int[]>::value == std::is_scalar<int[]>::value);
      CHECK(etl::is_scalar<void(int)>::value == std::is_scalar<void(int)>::value);

      CHECK(etl::is_scalar<int>::value);
      CHECK(etl::is_scalar<Enum>::value);
      CHECK(etl::is_scalar<int*>::value);
      CHECK(etl::is_scalar<int Class::*>::value);
      CHECK(etl::is_scalar<std::nullptr_t>::value);
      CHECK(!etl::is_scalar<Class>::value);
      CHECK(!etl::is_scalar<int&>::value);
      CHECK(!etl::is_scalar<int[3]>::value);

#if ETL_USING_CPP17
      CHECK(etl::is_scalar_v<int>);
      CHECK(etl::is_scalar_v<Enum>);
      CHECK(etl::is_scalar_v<int*>);
      CHECK(etl::is_scalar_v<std::nullptr_t>);
      CHECK(!etl::is_scalar_v<Class>);
      CHECK(!etl::is_scalar_v<int[3]>);
#endif
    }

    //*************************************************************************
    TEST(test_is_reference)
    {
      CHECK(etl::is_reference<int>::value == std::is_reference<int>::value);
      CHECK(etl::is_reference<int&>::value == std::is_reference<int&>::value);
      CHECK(etl::is_reference<const int&>::value == std::is_reference<const int&>::value);
      CHECK(etl::is_reference<volatile int&>::value == std::is_reference<volatile int&>::value);
      CHECK(etl::is_reference<const volatile int&>::value == std::is_reference<const volatile int&>::value);
      CHECK(etl::is_reference<int&&>::value == std::is_reference<int&&>::value);
      CHECK(etl::is_reference<const int&&>::value == std::is_reference<const int&&>::value);
      CHECK(etl::is_reference<volatile int&&>::value == std::is_reference<volatile int&&>::value);
      CHECK(etl::is_reference<const volatile int&&>::value == std::is_reference<const volatile int&&>::value);
    }

    //*************************************************************************
    TEST(test_is_same)
    {
      CHECK((etl::is_same<int, int>::value == etl::is_same<int, int>::value));
      CHECK((etl::is_same<char, int>::value == etl::is_same<char, int>::value));
    }

    //*************************************************************************
    TEST(test_is_array)
    {
      CHECK(etl::is_array<int>::value == std::is_array<int>::value);
      CHECK(etl::is_array<int[10]>::value == std::is_array<int[10]>::value);
    }

    //*************************************************************************
    TEST(test_remove_pointer)
    {
      CHECK((etl::is_same<etl::remove_pointer<int>::type, std::remove_pointer<int>::type>::value));
      CHECK((etl::is_same<etl::remove_pointer<const int>::type, std::remove_pointer<const int>::type>::value));
      CHECK((etl::is_same<etl::remove_pointer<int*>::type, std::remove_pointer<int*>::type>::value));
      CHECK((etl::is_same<etl::remove_pointer<int* const>::type, std::remove_pointer<int*>::type>::value));
      CHECK((etl::is_same<etl::remove_pointer<const int*>::type, std::remove_pointer<const int*>::type>::value));
      CHECK((etl::is_same<etl::remove_pointer<const int* const>::type, std::remove_pointer<const int* const>::type>::value));

      CHECK((etl::is_same<etl::remove_pointer<volatile int>::type, std::remove_pointer<volatile int>::type>::value));
      CHECK((etl::is_same<etl::remove_pointer<volatile const int>::type, std::remove_pointer<volatile const int>::type>::value));
      CHECK((etl::is_same<etl::remove_pointer<volatile int*>::type, std::remove_pointer<volatile int*>::type>::value));
      CHECK((etl::is_same<etl::remove_pointer<volatile int* const >::type, std::remove_pointer<volatile int*>::type>::value));
      CHECK((etl::is_same<etl::remove_pointer<const volatile int*>::type, std::remove_pointer<const volatile int*>::type>::value));
      CHECK((etl::is_same< etl::remove_pointer<const volatile int* const>::type, std::remove_pointer<const volatile int* const>::type>::value));
    }

    //*************************************************************************
    TEST(test_add_pointer)
    {
      CHECK((etl::is_same<etl::add_pointer<int>::type, std::add_pointer<int>::type>::value));
      CHECK((etl::is_same<etl::add_pointer<const int>::type, std::add_pointer<const int>::type>::value));
      CHECK((etl::is_same<etl::add_pointer<int*>::type, std::add_pointer<int*>::type>::value));
      CHECK((etl::is_same<etl::add_pointer<int* const >::type, std::add_pointer<int* const>::type>::value));
      CHECK((etl::is_same<etl::add_pointer<const int*>::type, std::add_pointer<const int*>::type>::value));
      CHECK((etl::is_same<etl::add_pointer<const int* const>::type, std::add_pointer<const int* const>::type>::value));
      CHECK((etl::is_same<etl::add_pointer<volatile int*>::type, std::add_pointer<volatile int*>::type>::value));
      CHECK((etl::is_same<etl::add_pointer<const volatile int*>::type, std::add_pointer<const volatile int*>::type>::value));
      CHECK((etl::is_same<etl::add_pointer<volatile int* const>::type, std::add_pointer<volatile int* const>::type>::value));
      CHECK((etl::is_same< etl::add_pointer<const volatile int* const>::type, std::add_pointer<const volatile int* const>::type>::value));
    }

    //*************************************************************************
    TEST(test_remove_reference)
    {
      CHECK((etl::is_same<etl::remove_reference<int>::type, std::remove_reference<int>::type>::value));
      CHECK((etl::is_same<etl::remove_reference<int&>::type, std::remove_reference<int&>::type>::value));
      CHECK((etl::is_same<etl::remove_reference<const int&>::type, std::remove_reference<const int&>::type>::value));
      CHECK((etl::is_same<etl::remove_reference<volatile int&>::type, std::remove_reference<volatile int&>::type>::value));
      CHECK((etl::is_same<etl::remove_reference<const volatile int&>::type, std::remove_reference<const volatile int&>::type>::value));
    }

    //*************************************************************************
    TEST(test_remove_const)
    {
      CHECK((etl::is_same<etl::remove_const<int>::type, std::remove_const<int>::type>::value));
      CHECK((etl::is_same<etl::remove_const<const int>::type, std::remove_const<const int>::type>::value));
      CHECK((etl::is_same<etl::remove_const<const volatile int>::type, std::remove_const<const volatile int>::type>::value));
    }

    //*************************************************************************
    TEST(test_add_const)
    {
      CHECK((etl::is_same<etl::add_const<int>::type, std::add_const<int>::type>::value));
      CHECK((etl::is_same<etl::add_const<const int>::type, std::add_const<const int>::type>::value));
      CHECK((etl::is_same<etl::add_const<const volatile int>::type, std::add_const<const volatile int>::type>::value));
    }

    //*************************************************************************
    TEST(test_is_const)
    {
      CHECK(etl::is_const<int>::value == std::is_const<int>::value);
      CHECK(etl::is_const<const int>::value == std::is_const<const int>::value);
      CHECK(etl::is_const<const volatile int>::value == std::is_const<const volatile int>::value);
    }

    //*************************************************************************
    TEST(test_is_volatile)
    {
      CHECK(etl::is_volatile<int>::value == std::is_volatile<int>::value);
      CHECK(etl::is_volatile<volatile int>::value == std::is_volatile<volatile int>::value);
      CHECK(etl::is_volatile<const volatile int>::value == std::is_volatile<const volatile int>::value);
    }

    //*************************************************************************
    TEST(test_remove_volatile)
    {
      CHECK((etl::is_same<etl::remove_volatile<int>::type, std::remove_volatile<int>::type>::value));
      CHECK((etl::is_same<etl::remove_volatile<volatile int>::type, std::remove_volatile<volatile int>::type>::value));
      CHECK((etl::is_same<etl::remove_volatile<const volatile int>::type, std::remove_volatile<const volatile int>::type>::value));
    }

    //*************************************************************************
    TEST(test_add_volatile)
    {
      CHECK((etl::is_same<etl::add_volatile<int>::type, std::add_volatile<int>::type>::value));
      CHECK((etl::is_same<etl::add_volatile<volatile int>::type, std::add_volatile<volatile int>::type>::value));
      CHECK((etl::is_same<etl::add_volatile<const volatile int>::type, std::add_volatile<const volatile int>::type>::value));
    }

    //*************************************************************************
    TEST(test_remove_cv)
    {
      CHECK((etl::is_same<etl::remove_cv<int>::type, std::remove_cv<int>::type>::value));
      CHECK((etl::is_same<etl::remove_cv<const int>::type, std::remove_cv<const int>::type>::value));
      CHECK((etl::is_same<etl::remove_cv<volatile int>::type, std::remove_cv<volatile int>::type>::value));
      CHECK((etl::is_same<etl::remove_cv<const volatile int>::type, std::remove_cv<const volatile int>::type>::value));
    }

    //*************************************************************************
    TEST(test_add_cv)
    {
      typedef etl::add_cv<int>::type t1;
      typedef std::add_cv<int>::type t2;

      bool pass = etl::is_same<t1, t2>::value;
      // etl::is_same<etl::add_cv<int>::type, std::add_cv<int>::type>::value;

      CHECK(pass);
      CHECK((etl::is_same<etl::add_cv<const int>::type, std::add_cv<const int>::type>::value));
      CHECK((etl::is_same<etl::add_cv<volatile int>::type, std::add_cv<volatile int>::type>::value));
      CHECK((etl::is_same<etl::add_cv<const volatile int>::type, std::add_cv<const volatile int>::type>::value));
    }

    //*************************************************************************
    TEST(test_is_arithmetic)
    {
      CHECK(etl::is_arithmetic<bool>::value == std::is_arithmetic<bool>::value);
      CHECK(etl::is_arithmetic<char>::value == std::is_arithmetic<char>::value);
      CHECK(etl::is_arithmetic<signed char>::value == std::is_arithmetic<signed char>::value);
      CHECK(etl::is_arithmetic<unsigned char>::value == std::is_arithmetic<unsigned char>::value);
      CHECK(etl::is_arithmetic<wchar_t>::value == std::is_arithmetic<wchar_t>::value);
      CHECK(etl::is_arithmetic<short>::value == std::is_arithmetic<short>::value);
      CHECK(etl::is_arithmetic<signed short>::value == std::is_arithmetic<signed short>::value);
      CHECK(etl::is_arithmetic<unsigned short>::value == std::is_arithmetic<unsigned short>::value);
      CHECK(etl::is_arithmetic<int>::value == std::is_arithmetic<int>::value);
      CHECK(etl::is_arithmetic<signed int>::value == std::is_arithmetic<signed int>::value);
      CHECK(etl::is_arithmetic<unsigned int>::value == std::is_arithmetic<unsigned int>::value);
      CHECK(etl::is_arithmetic<long>::value == std::is_arithmetic<long>::value);
      CHECK(etl::is_arithmetic<signed long>::value == std::is_arithmetic<signed long>::value);
      CHECK(etl::is_arithmetic<unsigned long>::value == std::is_arithmetic<unsigned long>::value);
      CHECK(etl::is_arithmetic<long long>::value == std::is_arithmetic<long long>::value);
      CHECK(etl::is_arithmetic<signed long long>::value == std::is_arithmetic<signed long long>::value);
      CHECK(etl::is_arithmetic<unsigned long long>::value == std::is_arithmetic<unsigned long long>::value);
      CHECK(etl::is_arithmetic<const int>::value == std::is_arithmetic<const int>::value);
      CHECK(etl::is_arithmetic<volatile int>::value == std::is_arithmetic<volatile int>::value);
      CHECK(etl::is_arithmetic<const int>::value == std::is_arithmetic<const int>::value);
      CHECK(etl::is_arithmetic<const volatile int>::value == std::is_arithmetic<const volatile int>::value);
      CHECK(etl::is_arithmetic<float>::value == std::is_arithmetic<float>::value);
      CHECK(etl::is_arithmetic<double>::value == std::is_arithmetic<double>::value);
      CHECK(etl::is_arithmetic<long double>::value == std::is_arithmetic<long double>::value);
      CHECK(etl::is_arithmetic<Test>::value == std::is_arithmetic<Test>::value);
    }

    //*************************************************************************
    TEST(test_is_fundamental)
    {
      CHECK(etl::is_fundamental<void>::value == std::is_fundamental<void>::value);
      CHECK(etl::is_fundamental<bool>::value == std::is_fundamental<bool>::value);
      CHECK(etl::is_fundamental<char>::value == std::is_fundamental<char>::value);
      CHECK(etl::is_fundamental<signed char>::value == std::is_fundamental<signed char>::value);
      CHECK(etl::is_fundamental<unsigned char>::value == std::is_fundamental<unsigned char>::value);
      CHECK(etl::is_fundamental<wchar_t>::value == std::is_fundamental<wchar_t>::value);
      CHECK(etl::is_fundamental<short>::value == std::is_fundamental<short>::value);
      CHECK(etl::is_fundamental<signed short>::value == std::is_fundamental<signed short>::value);
      CHECK(etl::is_fundamental<unsigned short>::value == std::is_fundamental<unsigned short>::value);
      CHECK(etl::is_fundamental<int>::value == std::is_fundamental<int>::value);
      CHECK(etl::is_fundamental<signed int>::value == std::is_fundamental<signed int>::value);
      CHECK(etl::is_fundamental<unsigned int>::value == std::is_fundamental<unsigned int>::value);
      CHECK(etl::is_fundamental<long>::value == std::is_fundamental<long>::value);
      CHECK(etl::is_fundamental<signed long>::value == std::is_fundamental<signed long>::value);
      CHECK(etl::is_fundamental<unsigned long>::value == std::is_fundamental<unsigned long>::value);
      CHECK(etl::is_fundamental<long long>::value == std::is_fundamental<long long>::value);
      CHECK(etl::is_fundamental<signed long long>::value == std::is_fundamental<signed long long>::value);
      CHECK(etl::is_fundamental<unsigned long long>::value == std::is_fundamental<unsigned long long>::value);
      CHECK(etl::is_fundamental<const int>::value == std::is_fundamental<const int>::value);
      CHECK(etl::is_fundamental<volatile int>::value == std::is_fundamental<volatile int>::value);
      CHECK(etl::is_fundamental<const int>::value == std::is_fundamental<const int>::value);
      CHECK(etl::is_fundamental<const volatile int>::value == std::is_fundamental<const volatile int>::value);
      CHECK(etl::is_fundamental<float>::value == std::is_fundamental<float>::value);
      CHECK(etl::is_fundamental<double>::value == std::is_fundamental<double>::value);
      CHECK(etl::is_fundamental<long double>::value == std::is_fundamental<long double>::value);
      CHECK(etl::is_fundamental<Test>::value == std::is_fundamental<Test>::value);
#if ETL_USING_STL
      // etl::is_fundamental does not report std::nullptr_t as fundamental when not using the STL.
      CHECK(etl::is_fundamental<std::nullptr_t>::value == std::is_fundamental<std::nullptr_t>::value);
#endif
    }

    //*************************************************************************
    TEST(test_is_compound)
    {
      CHECK(etl::is_compound<void>::value == std::is_compound<void>::value);
      CHECK(etl::is_compound<bool>::value == std::is_compound<bool>::value);
      CHECK(etl::is_compound<char>::value == std::is_compound<char>::value);
      CHECK(etl::is_compound<signed char>::value == std::is_compound<signed char>::value);
      CHECK(etl::is_compound<unsigned char>::value == std::is_compound<unsigned char>::value);
      CHECK(etl::is_compound<wchar_t>::value == std::is_compound<wchar_t>::value);
      CHECK(etl::is_compound<short>::value == std::is_compound<short>::value);
      CHECK(etl::is_compound<signed short>::value == std::is_compound<signed short>::value);
      CHECK(etl::is_compound<unsigned short>::value == std::is_compound<unsigned short>::value);
      CHECK(etl::is_compound<int>::value == std::is_compound<int>::value);
      CHECK(etl::is_compound<signed int>::value == std::is_compound<signed int>::value);
      CHECK(etl::is_compound<unsigned int>::value == std::is_compound<unsigned int>::value);
      CHECK(etl::is_compound<long>::value == std::is_compound<long>::value);
      CHECK(etl::is_compound<signed long>::value == std::is_compound<signed long>::value);
      CHECK(etl::is_compound<unsigned long>::value == std::is_compound<unsigned long>::value);
      CHECK(etl::is_compound<long long>::value == std::is_compound<long long>::value);
      CHECK(etl::is_compound<signed long long>::value == std::is_compound<signed long long>::value);
      CHECK(etl::is_compound<unsigned long long>::value == std::is_compound<unsigned long long>::value);
      CHECK(etl::is_compound<const int>::value == std::is_compound<const int>::value);
      CHECK(etl::is_compound<volatile int>::value == std::is_compound<volatile int>::value);
      CHECK(etl::is_compound<const int>::value == std::is_compound<const int>::value);
      CHECK(etl::is_compound<const volatile int>::value == std::is_compound<const volatile int>::value);
      CHECK(etl::is_compound<float>::value == std::is_compound<float>::value);
      CHECK(etl::is_compound<double>::value == std::is_compound<double>::value);
      CHECK(etl::is_compound<long double>::value == std::is_compound<long double>::value);
      CHECK(etl::is_compound<Test>::value == std::is_compound<Test>::value);
    }

    //*************************************************************************
    TEST(test_is_void)
    {
      CHECK(etl::is_void<int>::value == std::is_void<int>::value);
      CHECK(etl::is_void<void>::value == std::is_void<void>::value);
    }

    //*************************************************************************
    TEST(test_make_signed)
    {
      CHECK((etl::is_same<etl::make_signed<char>::type, std::make_signed<char>::type>::value));
      CHECK((etl::is_same<etl::make_signed<signed char>::type, std::make_signed<signed char>::type>::value));
      CHECK((etl::is_same<etl::make_signed<unsigned char>::type, std::make_signed<unsigned char>::type>::value));
      CHECK(etl::is_signed<etl::make_signed<wchar_t>::type>::value && (sizeof(wchar_t) == sizeof(etl::make_signed<wchar_t>::type)));
      CHECK((etl::is_same<etl::make_signed<short>::type, std::make_signed<short>::type>::value));
      CHECK((etl::is_same<etl::make_signed<signed short>::type, std::make_signed<signed short>::type>::value));
      CHECK((etl::is_same<etl::make_signed<unsigned short>::type, std::make_signed<unsigned short>::type>::value));
      CHECK((etl::is_same<etl::make_signed<int>::type, std::make_signed<int>::type>::value));
      CHECK((etl::is_same<etl::make_signed<signed int>::type, std::make_signed<signed int>::type>::value));
      CHECK((etl::is_same<etl::make_signed<unsigned int>::type, std::make_signed<unsigned int>::type>::value));
      CHECK((etl::is_same<etl::make_signed<long>::type, std::make_signed<long>::type>::value));
      CHECK((etl::is_same<etl::make_signed<signed long>::type, std::make_signed<signed long>::type>::value));
      CHECK((etl::is_same<etl::make_signed<unsigned long>::type, std::make_signed<unsigned long>::type>::value));
      CHECK((etl::is_same<etl::make_signed<long long>::type, std::make_signed<long long>::type>::value));
      CHECK((etl::is_same<etl::make_signed<signed long long>::type, std::make_signed<signed long long>::type>::value));
      CHECK((etl::is_same<etl::make_signed<unsigned long long>::type, std::make_signed<unsigned long long>::type>::value));
      CHECK((etl::is_same<etl::make_signed<const int>::type, std::make_signed<const int>::type>::value));
      CHECK((etl::is_same<etl::make_signed<volatile int>::type, std::make_signed<volatile int>::type>::value));
      CHECK((etl::is_same<etl::make_signed<const int>::type, std::make_signed<const int>::type>::value));
      CHECK((etl::is_same<etl::make_signed<const volatile int>::type, std::make_signed<const volatile int>::type>::value));
      CHECK((etl::is_same<etl::make_signed<size_t>::type, std::make_signed<size_t>::type>::value));

      enum class ue : uint8_t
      {
        One,
        Two
      };

      CHECK((etl::is_same< etl::make_signed<typename std::underlying_type<ue>::type>::type, std::make_signed<ue>::type>::value));

      enum class se : int8_t
      {
        One,
        Two
      };

      CHECK((etl::is_same< etl::make_signed<typename std::underlying_type<se>::type>::type, std::make_signed<se>::type>::value));
    }

    //*************************************************************************
    TEST(test_make_unsigned)
    {
      CHECK((etl::is_same<etl::make_unsigned<char>::type, std::make_unsigned<char>::type>::value));
      CHECK((etl::is_same<etl::make_unsigned<signed char>::type, std::make_unsigned<signed char>::type>::value));
      CHECK((etl::is_same<etl::make_unsigned<unsigned char>::type, std::make_unsigned<unsigned char>::type>::value));
      CHECK(etl::is_unsigned<etl::make_unsigned<wchar_t>::type>::value && (sizeof(wchar_t) == sizeof(etl::make_unsigned<wchar_t>::type)));
      CHECK((etl::is_same<etl::make_unsigned<short>::type, std::make_unsigned<short>::type>::value));
      CHECK((etl::is_same<etl::make_unsigned<signed short>::type, std::make_unsigned<signed short>::type>::value));
      CHECK((etl::is_same<etl::make_unsigned<unsigned short>::type, std::make_unsigned<unsigned short>::type>::value));
      CHECK((etl::is_same<etl::make_unsigned<int>::type, std::make_unsigned<int>::type>::value));
      CHECK((etl::is_same<etl::make_unsigned<signed int>::type, std::make_unsigned<signed int>::type>::value));
      CHECK((etl::is_same<etl::make_unsigned<unsigned int>::type, std::make_unsigned<unsigned int>::type>::value));
      CHECK((etl::is_same<etl::make_unsigned<long>::type, std::make_unsigned<long>::type>::value));
      CHECK((etl::is_same<etl::make_unsigned<signed long>::type, std::make_unsigned<signed long>::type>::value));
      CHECK((etl::is_same<etl::make_unsigned<unsigned long>::type, std::make_unsigned<unsigned long>::type>::value));
      CHECK((etl::is_same<etl::make_unsigned<long long>::type, std::make_unsigned<long long>::type>::value));
      CHECK((etl::is_same<etl::make_unsigned<signed long long>::type, std::make_unsigned<signed long long>::type>::value));
      CHECK((etl::is_same<etl::make_unsigned<unsigned long long>::type, std::make_unsigned<unsigned long long>::type>::value));
      CHECK((etl::is_same<etl::make_unsigned<const int>::type, std::make_unsigned<const int>::type>::value));
      CHECK((etl::is_same<etl::make_unsigned<volatile int>::type, std::make_unsigned<volatile int>::type>::value));
      CHECK((etl::is_same<etl::make_unsigned<const int>::type, std::make_unsigned<const int>::type>::value));
      CHECK((etl::is_same<etl::make_unsigned<const volatile int>::type, std::make_unsigned<const volatile int>::type>::value));
      CHECK((etl::is_same<etl::make_unsigned<size_t>::type, std::make_unsigned<size_t>::type>::value));

      enum class ue : uint8_t
      {
        One,
        Two
      };

      CHECK((etl::is_same< etl::make_unsigned<typename std::underlying_type<ue>::type>::type, std::make_unsigned<ue>::type>::value));

      enum class se : int8_t
      {
        One,
        Two
      };

      CHECK((etl::is_same< etl::make_unsigned<typename std::underlying_type<se>::type>::type, std::make_unsigned<se>::type>::value));
    }

    //*************************************************************************
    TEST(test_extent)
    {
      CHECK(std::extent<int>::value == etl::extent<int>::value);
      CHECK(std::extent<int[]>::value == etl::extent<int[]>::value);
      CHECK(std::extent<int[10]>::value == etl::extent<int[10]>::value);
    }

    //*************************************************************************
    TEST(test_remove_extent)
    {
      CHECK((etl::is_same<etl::remove_extent<int>::type, std::remove_extent<int>::type>::value));
      CHECK((etl::is_same<etl::remove_extent<int[]>::type, std::remove_extent<int[]>::type>::value));
      CHECK((etl::is_same<etl::remove_extent<int[10]>::type, std::remove_extent<int[10]>::type>::value));
    }

    //*************************************************************************
    TEST(test_remove_all_extents)
    {
      CHECK((etl::is_same<etl::remove_all_extents<int>::type, std::remove_all_extents<int>::type>::value));
      CHECK((etl::is_same<etl::remove_all_extents<int[10]>::type, std::remove_all_extents<int[10]>::type>::value));
      CHECK((etl::is_same<etl::remove_all_extents<int[10][10]>::type, std::remove_all_extents<int[10][10]>::type>::value));
    }

    //*************************************************************************
    TEST(test_rank)
    {
      CHECK(etl::rank<int>::value == std::rank<int>::value);
      CHECK(etl::rank<int[10]>::value == std::rank<int[10]>::value);
      CHECK(etl::rank<int[10][10]>::value == std::rank<int[10][10]>::value);
    }

    //*************************************************************************
    TEST(test_alignment_of)
    {
      struct Object
      {
        int   a;
        char  b;
        float c;
      };

      CHECK(std::alignment_of<char>::value == etl::alignment_of<char>::value);
      CHECK(std::alignment_of<unsigned char>::value == etl::alignment_of<unsigned char>::value);
      CHECK(std::alignment_of<short>::value == etl::alignment_of<short>::value);
      CHECK(std::alignment_of<unsigned short>::value == etl::alignment_of<unsigned short>::value);
      CHECK(std::alignment_of<int>::value == etl::alignment_of<int>::value);
      CHECK(std::alignment_of<unsigned int>::value == etl::alignment_of<unsigned int>::value);
      CHECK(std::alignment_of<long>::value == etl::alignment_of<long>::value);
      CHECK(std::alignment_of<unsigned long>::value == etl::alignment_of<unsigned long>::value);
      CHECK(std::alignment_of<long long>::value == etl::alignment_of<long long>::value);
      CHECK(std::alignment_of<unsigned long long>::value == etl::alignment_of<unsigned long long>::value);
      CHECK(std::alignment_of<float>::value == etl::alignment_of<float>::value);
      CHECK(std::alignment_of<double>::value == etl::alignment_of<double>::value);
      CHECK(std::alignment_of<Object>::value == etl::alignment_of<Object>::value);
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
      struct A
      {
      };
      struct B : public A
      {
      };
      struct C
      {
      };
      struct D : private A
      {
      };

      CHECK((std::is_base_of<A, A>::value) == (etl::is_base_of<A, A>::value));
      CHECK((std::is_base_of<A, B>::value) == (etl::is_base_of<A, B>::value));
      CHECK((std::is_base_of<A, C>::value) == (etl::is_base_of<A, C>::value));
      CHECK((std::is_base_of<B, A>::value) == (etl::is_base_of<B, A>::value));
      CHECK((std::is_base_of<B, B>::value) == (etl::is_base_of<B, B>::value));
      CHECK((std::is_base_of<B, C>::value) == (etl::is_base_of<B, C>::value));
      CHECK((std::is_base_of<C, A>::value) == (etl::is_base_of<C, A>::value));
      CHECK((std::is_base_of<C, B>::value) == (etl::is_base_of<C, B>::value));
      CHECK((std::is_base_of<C, C>::value) == (etl::is_base_of<C, C>::value));
      CHECK((std::is_base_of<D, A>::value) == (etl::is_base_of<D, A>::value));
      CHECK((std::is_base_of<A, D>::value) == (etl::is_base_of<A, D>::value));

      CHECK((std::is_base_of<char, char>::value) == (etl::is_base_of<char, char>::value));
      CHECK((std::is_base_of<char, int>::value) == (etl::is_base_of<char, int>::value));
      CHECK((std::is_base_of<int, char>::value) == (etl::is_base_of<int, char>::value));
    }

#if ETL_USING_BUILTIN_IS_VIRTUAL_BASE_OF
    //*************************************************************************
    TEST(test_is_virtual_base_of)
    {
      struct A
      {
      };
      struct B : public A // Non-virtual base
      {
      };
      struct C : virtual public A // Virtual base
      {
      };
      struct D
        : public B
        , virtual public A // A is both virtual and non-virtual base
      {
      };
      struct E : public C // A is indirect virtual base
      {
      };
      struct F // Unrelated class
      {
      };

      // A is not a virtual base of A (same class)
      CHECK_EQUAL(false, (etl::is_virtual_base_of<A, A>::value));

      // A is NOT a virtual base of B (it's a non-virtual base)
      CHECK_EQUAL(false, (etl::is_virtual_base_of<A, B>::value));

      // A IS a virtual base of C
      CHECK_EQUAL(true, (etl::is_virtual_base_of<A, C>::value));

      // A IS a virtual base of D (even though it's also a non-virtual base via B)
      CHECK_EQUAL(true, (etl::is_virtual_base_of<A, D>::value));

      // A IS a virtual base of E (indirect virtual base)
      CHECK_EQUAL(true, (etl::is_virtual_base_of<A, E>::value));

      // Unrelated classes
      CHECK_EQUAL(false, (etl::is_virtual_base_of<A, F>::value));
      CHECK_EQUAL(false, (etl::is_virtual_base_of<F, A>::value));

      // Fundamental types
      CHECK_EQUAL(false, (etl::is_virtual_base_of<int, int>::value));
      CHECK_EQUAL(false, (etl::is_virtual_base_of<int, char>::value));

  #if ETL_USING_CPP17
      // Test the _v helper
      CHECK_EQUAL(false, etl::is_virtual_base_of_v<A, B>);
      CHECK_EQUAL(true, etl::is_virtual_base_of_v<A, C>);
      CHECK_EQUAL(true, etl::is_virtual_base_of_v<A, D>);
      CHECK_EQUAL(true, etl::is_virtual_base_of_v<A, E>);
  #endif
    }
#endif

    //*************************************************************************
    TEST(test_types)
    {
      CHECK((etl::is_same<int, etl::types<int>::type>::value));
      CHECK((etl::is_same<int&, etl::types<int>::reference>::value));
      CHECK((etl::is_same<const int&, etl::types<int>::const_reference>::value));
      CHECK((etl::is_same<int&&, etl::types<int>::rvalue_reference>::value));
      CHECK((etl::is_same<int*, etl::types<int>::pointer>::value));
      CHECK((etl::is_same<const int*, etl::types<int>::const_pointer>::value));
      CHECK((etl::is_same<const int* const, etl::types<int>::const_pointer_const>::value));

      CHECK((etl::is_same<int, etl::types<const int>::type>::value));
      CHECK((etl::is_same<int&, etl::types<const int>::reference>::value));
      CHECK((etl::is_same<const int&, etl::types<const int>::const_reference>::value));
      CHECK((etl::is_same<int&&, etl::types<const int>::rvalue_reference>::value));
      CHECK((etl::is_same<int*, etl::types<const int>::pointer>::value));
      CHECK((etl::is_same<const int*, etl::types<const int>::const_pointer>::value));
      CHECK((etl::is_same<const int* const, etl::types<const int>::const_pointer_const>::value));

      CHECK((etl::is_same<int, etl::types<int*>::type>::value));
      CHECK((etl::is_same<int&, etl::types<int*>::reference>::value));
      CHECK((etl::is_same<const int&, etl::types<int*>::const_reference>::value));
      CHECK((etl::is_same<int&&, etl::types<int*>::rvalue_reference>::value));
      CHECK((etl::is_same<int*, etl::types<int*>::pointer>::value));
      CHECK((etl::is_same<const int*, etl::types<int*>::const_pointer>::value));
      CHECK((etl::is_same<const int* const, etl::types<int*>::const_pointer_const>::value));

      CHECK((etl::is_same<int, etl::types<int* const>::type>::value));
      CHECK((etl::is_same<int&, etl::types<int* const>::reference>::value));
      CHECK((etl::is_same<const int&, etl::types<int* const>::const_reference>::value));
      CHECK((etl::is_same<int&&, etl::types<int* const>::rvalue_reference>::value));
      CHECK((etl::is_same<int*, etl::types<int* const>::pointer>::value));
      CHECK((etl::is_same<const int*, etl::types<int* const>::const_pointer>::value));
      CHECK((etl::is_same<const int* const, etl::types<int* const>::const_pointer_const>::value));

      CHECK((etl::is_same<int, etl::types<const int*>::type>::value));
      CHECK((etl::is_same<int&, etl::types<const int*>::reference>::value));
      CHECK((etl::is_same<const int&, etl::types<const int*>::const_reference>::value));
      CHECK((etl::is_same<int&&, etl::types<const int*>::rvalue_reference>::value));
      CHECK((etl::is_same<int*, etl::types<const int*>::pointer>::value));
      CHECK((etl::is_same<const int*, etl::types<const int*>::const_pointer>::value));
      CHECK((etl::is_same<const int* const, etl::types<const int*>::const_pointer_const>::value));

      CHECK((etl::is_same<int, etl::types<const int* const>::type>::value));
      CHECK((etl::is_same<int&, etl::types<const int* const>::reference>::value));
      CHECK((etl::is_same<const int&, etl::types<const int* const>::const_reference>::value));
      CHECK((etl::is_same<int&&, etl::types<const int* const>::rvalue_reference>::value));
      CHECK((etl::is_same<int*, etl::types<const int* const>::pointer>::value));
      CHECK((etl::is_same<const int*, etl::types<const int* const>::const_pointer>::value));
      CHECK((etl::is_same< const int* const, etl::types<const int* const>::const_pointer_const>::value));

      CHECK((etl::is_same<int, etl::types<int&>::type>::value));
      CHECK((etl::is_same<int&, etl::types<int&>::reference>::value));
      CHECK((etl::is_same<const int&, etl::types<int&>::const_reference>::value));
      CHECK((etl::is_same<int&&, etl::types<int&>::rvalue_reference>::value));
      CHECK((etl::is_same<int*, etl::types<int&>::pointer>::value));
      CHECK((etl::is_same<const int*, etl::types<int&>::const_pointer>::value));
      CHECK((etl::is_same<const int* const, etl::types<int&>::const_pointer_const>::value));

      CHECK((etl::is_same<int, etl::types<const int&>::type>::value));
      CHECK((etl::is_same<int&, etl::types<const int&>::reference>::value));
      CHECK((etl::is_same<const int&, etl::types<const int&>::const_reference>::value));
      CHECK((etl::is_same<int&&, etl::types<const int&>::rvalue_reference>::value));
      CHECK((etl::is_same<int*, etl::types<const int&>::pointer>::value));
      CHECK((etl::is_same<const int*, etl::types<const int&>::const_pointer>::value));
      CHECK((etl::is_same<const int* const, etl::types<const int&>::const_pointer_const>::value));

      CHECK((etl::is_same<int, etl::types<int&&>::type>::value));
      CHECK((etl::is_same<int&, etl::types<int&&>::reference>::value));
      CHECK((etl::is_same<const int&, etl::types<int&&>::const_reference>::value));
      CHECK((etl::is_same<int&&, etl::types<int&&>::rvalue_reference>::value));
      CHECK((etl::is_same<int*, etl::types<int&&>::pointer>::value));
      CHECK((etl::is_same<const int*, etl::types<int&&>::const_pointer>::value));
      CHECK((etl::is_same<const int* const, etl::types<int&&>::const_pointer_const>::value));

      CHECK((etl::is_same<int, etl::types<volatile int>::type>::value));
      CHECK((etl::is_same<int&, etl::types<volatile int>::reference>::value));
      CHECK((etl::is_same<const int&, etl::types<volatile int>::const_reference>::value));
      CHECK((etl::is_same<int&&, etl::types<volatile int>::rvalue_reference>::value));
      CHECK((etl::is_same<int*, etl::types<volatile int>::pointer>::value));
      CHECK((etl::is_same<const int*, etl::types<volatile int>::const_pointer>::value));
      CHECK((etl::is_same<const int* const, etl::types<volatile int>::const_pointer_const>::value));

      CHECK((etl::is_same<int, etl::types<const volatile int>::type>::value));
      CHECK((etl::is_same<int&, etl::types<const volatile int>::reference>::value));
      CHECK((etl::is_same<const int&, etl::types<const volatile int>::const_reference>::value));
      CHECK((etl::is_same<int&&, etl::types<const volatile int>::rvalue_reference>::value));
      CHECK((etl::is_same<int*, etl::types<const volatile int>::pointer>::value));
      CHECK((etl::is_same<const int*, etl::types<const volatile int>::const_pointer>::value));
      CHECK((etl::is_same< const int* const, etl::types<const volatile int>::const_pointer_const>::value));

      CHECK((etl::is_same<int, etl::types<volatile int*>::type>::value));
      CHECK((etl::is_same<int&, etl::types<volatile int*>::reference>::value));
      CHECK((etl::is_same<const int&, etl::types<volatile int*>::const_reference>::value));
      CHECK((etl::is_same<int&&, etl::types<volatile int*>::rvalue_reference>::value));
      CHECK((etl::is_same<int*, etl::types<volatile int*>::pointer>::value));
      CHECK((etl::is_same<const int*, etl::types<volatile int*>::const_pointer>::value));
      CHECK((etl::is_same<const int* const, etl::types<volatile int*>::const_pointer_const>::value));

      CHECK((etl::is_same<int, etl::types<volatile int* const>::type>::value));
      CHECK((etl::is_same<int&, etl::types<volatile int* const>::reference>::value));
      CHECK((etl::is_same<const int&, etl::types<volatile int* const>::const_reference>::value));
      CHECK((etl::is_same< int&&, etl::types<volatile int* const>::rvalue_reference>::value));
      CHECK((etl::is_same<int*, etl::types<volatile int* const>::pointer>::value));
      CHECK((etl::is_same<const int*, etl::types<volatile int* const>::const_pointer>::value));
      CHECK((etl::is_same< const int* const, etl::types<volatile int* const>::const_pointer_const>::value));

      CHECK((etl::is_same<int, etl::types<const volatile int*>::type>::value));
      CHECK((etl::is_same<int&, etl::types<const volatile int*>::reference>::value));
      CHECK((etl::is_same<const int&, etl::types<const volatile int*>::const_reference>::value));
      CHECK((etl::is_same< int&&, etl::types<const volatile int*>::rvalue_reference>::value));
      CHECK((etl::is_same<int*, etl::types<const volatile int*>::pointer>::value));
      CHECK((etl::is_same<const int*, etl::types<const volatile int*>::const_pointer>::value));
      CHECK((etl::is_same< const int* const, etl::types<const volatile int*>::const_pointer_const>::value));

      CHECK((etl::is_same<int, etl::types<const volatile int* const>::type>::value));
      CHECK((etl::is_same<int&, etl::types<const volatile int* const>::reference>::value));
      CHECK((etl::is_same< const int&, etl::types<const volatile int* const>::const_reference>::value));
      CHECK((etl::is_same< int&&, etl::types<const volatile int* const>::rvalue_reference>::value));
      CHECK((etl::is_same<int*, etl::types<const volatile int* const>::pointer>::value));
      CHECK((etl::is_same< const int*, etl::types<const volatile int* const>::const_pointer>::value));
      CHECK((etl::is_same< const int* const, etl::types<const volatile int* const>::const_pointer_const>::value));

      CHECK((etl::is_same<int, etl::types<volatile int&>::type>::value));
      CHECK((etl::is_same<int&, etl::types<volatile int&>::reference>::value));
      CHECK((etl::is_same<const int&, etl::types<volatile int&>::const_reference>::value));
      CHECK((etl::is_same<int&&, etl::types<volatile int&>::rvalue_reference>::value));
      CHECK((etl::is_same<int*, etl::types<volatile int&>::pointer>::value));
      CHECK((etl::is_same<const int*, etl::types<volatile int&>::const_pointer>::value));
      CHECK((etl::is_same<const int* const, etl::types<volatile int&>::const_pointer_const>::value));

      CHECK((etl::is_same<int, etl::types<const volatile int&>::type>::value));
      CHECK((etl::is_same<int&, etl::types<const volatile int&>::reference>::value));
      CHECK((etl::is_same<const int&, etl::types<const volatile int&>::const_reference>::value));
      CHECK((etl::is_same< int&&, etl::types<const volatile int&>::rvalue_reference>::value));
      CHECK((etl::is_same<int*, etl::types<const volatile int&>::pointer>::value));
      CHECK((etl::is_same<const int*, etl::types<const volatile int&>::const_pointer>::value));
      CHECK((etl::is_same< const int* const, etl::types<const volatile int&>::const_pointer_const>::value));
    }

    //*************************************************************************
    TEST(test_conditional_integral_constant)
    {
      int v1 = etl::conditional_integral_constant<true, int, 1, 2>::value;
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
      CHECK(!(etl::is_convertible_v<int (*)(), void (*)()>));
      CHECK(!(etl::is_convertible_v<int (*)(), int*>));
      CHECK(!(etl::is_convertible_v<int*, int (*)()>));
      CHECK(!(etl::is_convertible_v<void*, int*>));
#else
      CHECK((etl::is_convertible<char, int>::value));
      CHECK((etl::is_convertible<int, char>::value));
      CHECK((etl::is_convertible<unsigned char, int>::value));
      CHECK((etl::is_convertible<int, unsigned char>::value));
      CHECK((etl::is_convertible<double, int>::value));
      CHECK((etl::is_convertible<int, double>::value));
      CHECK((etl::is_convertible<int*, void*>::value));
      CHECK(!(etl::is_convertible<int (*)(), void (*)()>::value));
      CHECK(!(etl::is_convertible<int (*)(), int*>::value));
      CHECK(!(etl::is_convertible<int*, int (*)()>::value));
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
      CHECK_EQUAL(std::is_lvalue_reference_v<void>, etl::is_lvalue_reference_v<void>);
      CHECK_EQUAL(std::is_lvalue_reference_v<int>, etl::is_lvalue_reference_v<int>);
      CHECK_EQUAL(std::is_lvalue_reference_v<int*>, etl::is_lvalue_reference_v<int*>);
      CHECK_EQUAL(std::is_lvalue_reference_v<int&>, etl::is_lvalue_reference_v<int&>);
      CHECK_EQUAL(std::is_lvalue_reference_v<int&&>, etl::is_lvalue_reference_v<int&&>);
#else
#endif
    }

    //*************************************************************************
    TEST(test_is_rvalue_reference)
    {
#if ETL_USING_CPP17
      CHECK_EQUAL(std::is_rvalue_reference_v<void>, etl::is_rvalue_reference_v<void>);
      CHECK_EQUAL(std::is_rvalue_reference_v<int>, etl::is_rvalue_reference_v<int>);
      CHECK_EQUAL(std::is_rvalue_reference_v<int*>, etl::is_rvalue_reference_v<int*>);
      CHECK_EQUAL(std::is_rvalue_reference_v<int&>, etl::is_rvalue_reference_v<int&>);
      CHECK_EQUAL(std::is_rvalue_reference_v<int&&>, etl::is_rvalue_reference_v<int&&>);
#else
      CHECK_EQUAL(std::is_rvalue_reference<void>::value, etl::is_rvalue_reference<void>::value);
      CHECK_EQUAL(std::is_rvalue_reference<int>::value, etl::is_rvalue_reference<int>::value);
      CHECK_EQUAL(std::is_rvalue_reference<int*>::value, etl::is_rvalue_reference<int*>::value);
      CHECK_EQUAL(std::is_rvalue_reference<int&>::value, etl::is_rvalue_reference<int&>::value);
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
    TEST(test_is_scoped_enum)
    {
      CHECK_FALSE((etl::is_scoped_enum<void>::value));
      CHECK_FALSE((etl::is_scoped_enum<int>::value));
      CHECK_FALSE((etl::is_scoped_enum<int*>::value));
      CHECK_FALSE((etl::is_scoped_enum<ToAny>::value));
      CHECK_FALSE((etl::is_scoped_enum<Enum>::value));
      CHECK_FALSE((etl::is_scoped_enum<const Enum>::value));
      CHECK_FALSE((etl::is_scoped_enum<Enum*>::value));

      CHECK_TRUE((etl::is_scoped_enum<EnumClass>::value));
      CHECK_TRUE((etl::is_scoped_enum<const EnumClass>::value));
      CHECK_FALSE((etl::is_scoped_enum<EnumClass*>::value));

#if ETL_USING_CPP17
      CHECK_FALSE((etl::is_scoped_enum_v<Enum>));
      CHECK_TRUE((etl::is_scoped_enum_v<EnumClass>));
#endif

#if ETL_USING_CPP23 && defined(__cpp_lib_is_scoped_enum)
      CHECK_EQUAL((std::is_scoped_enum<Enum>::value), (etl::is_scoped_enum<Enum>::value));
      CHECK_EQUAL((std::is_scoped_enum<EnumClass>::value), (etl::is_scoped_enum<EnumClass>::value));
      CHECK_EQUAL((std::is_scoped_enum<int>::value), (etl::is_scoped_enum<int>::value));
#endif
    }

    //*************************************************************************
    TEST(test_integral_constants)
    {
#if ETL_USING_CPP17
      CHECK_EQUAL(1, (etl::integral_constant_v<int, 1>));
      CHECK((std::is_same_v<int, etl::integral_constant<int, 1>::value_type>));

      CHECK_EQUAL(false, etl::bool_constant_v<false>);
      CHECK_EQUAL(true, etl::bool_constant_v<true>);
      CHECK((std::is_same_v<bool, etl::bool_constant<true>::value_type>));

      CHECK_EQUAL(true, etl::negation_v<etl::bool_constant<false>>);
      CHECK_EQUAL(false, etl::negation_v<etl::bool_constant<true>>);
      CHECK((std::is_same_v<bool, etl::bool_constant<false>::value_type>));
#else
      CHECK_EQUAL(1, (etl::integral_constant<int, 1>::value));
      CHECK((std::is_same<int, etl::integral_constant<int, 1>::value_type>::value));

      CHECK_EQUAL(false, (etl::bool_constant<false>::value));
      CHECK_EQUAL(true, (etl::bool_constant<true>::value));
      CHECK((std::is_same<bool, etl::bool_constant<true>::value_type>::value));

      CHECK_EQUAL(true, etl::negation<etl::bool_constant<false>>::value);
      CHECK_EQUAL(false, etl::negation<etl::bool_constant<true>>::value);
      CHECK((std::is_same<bool, etl::bool_constant<true>::value_type>::value));
#endif
    }

    //*************************************************************************
    TEST(test_are_all_same)
    {
#if ETL_USING_CPP17
      CHECK((etl::are_all_same_v<int, int, int, int, int> == true));
      CHECK((etl::are_all_same_v<int, int, int, char, int> == false));
#else
      CHECK((etl::are_all_same<int, int, int, int, int>::value == true));
      CHECK((etl::are_all_same<int, int, int, char, int>::value == false));
#endif
    }

    //*************************************************************************
    TEST(test_conjunction)
    {
#if ETL_USING_CPP17
      CHECK_TRUE((etl::conjunction_v<etl::true_type, etl::true_type, etl::true_type>));
      CHECK_FALSE((etl::conjunction_v<etl::true_type, etl::false_type, etl::true_type>));
#else
      CHECK_TRUE((etl::conjunction<etl::true_type, etl::true_type, etl::true_type>::value));
      CHECK_FALSE((etl::conjunction<etl::true_type, etl::false_type, etl::true_type>::value));
#endif
    }

    //*************************************************************************
    TEST(test_disjunction)
    {
#if ETL_USING_CPP17
      CHECK_TRUE((etl::disjunction_v<etl::false_type, etl::true_type, etl::false_type>));
      CHECK_FALSE((etl::disjunction_v<etl::false_type, etl::false_type, etl::false_type>));
#else
      CHECK_TRUE((etl::disjunction<etl::false_type, etl::true_type, etl::false_type>::value));
      CHECK_FALSE((etl::disjunction<etl::false_type, etl::false_type, etl::false_type>::value));
#endif
    }

    //*************************************************************************
    TEST(test_exclusive_disjunction)
    {
#if ETL_USING_CPP17
      CHECK_TRUE((etl::exclusive_disjunction_v<etl::false_type, etl::true_type, etl::false_type>));
      CHECK_FALSE((etl::exclusive_disjunction_v<etl::true_type, etl::false_type, etl::true_type>));
      CHECK_FALSE((etl::exclusive_disjunction_v<etl::false_type, etl::false_type, etl::false_type>));
#else
      CHECK_TRUE((etl::exclusive_disjunction<etl::false_type, etl::true_type, etl::false_type>::value));
      CHECK_FALSE((etl::exclusive_disjunction<etl::true_type, etl::false_type, etl::true_type>::value));
      CHECK_FALSE((etl::exclusive_disjunction<etl::false_type, etl::false_type, etl::false_type>::value));
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
      CHECK((etl::is_constructible_v<NotDefaultConstructible>) == (std::is_constructible_v<NotDefaultConstructible>));
#else
      CHECK((etl::is_constructible<Copyable>::value) == (std::is_constructible<Copyable>::value));
      CHECK((etl::is_constructible<Moveable>::value) == (std::is_constructible<Moveable>::value));
      CHECK((etl::is_constructible<MoveableCopyable>::value) == (std::is_constructible<MoveableCopyable>::value));
      CHECK((etl::is_constructible<NotDefaultConstructible>::value) == (std::is_constructible<NotDefaultConstructible>::value));
#endif
    }

    //*************************************************************************
    TEST(test_is_default_constructible)
    {
#if ETL_USING_CPP17
      CHECK((etl::is_default_constructible_v<Copyable>) == (std::is_default_constructible_v<Copyable>));
      CHECK((etl::is_default_constructible_v<Moveable>) == (std::is_default_constructible_v<Moveable>));
      CHECK((etl::is_default_constructible_v<MoveableCopyable>) == (std::is_default_constructible_v<MoveableCopyable>));
      CHECK((etl::is_default_constructible_v<NotDefaultConstructible>) == (std::is_default_constructible_v<NotDefaultConstructible>));
#else
      CHECK((etl::is_default_constructible<Copyable>::value) == (std::is_default_constructible<Copyable>::value));
      CHECK((etl::is_default_constructible<Moveable>::value) == (std::is_default_constructible<Moveable>::value));
      CHECK((etl::is_default_constructible<MoveableCopyable>::value) == (std::is_default_constructible<MoveableCopyable>::value));
      CHECK((etl::is_default_constructible<NotDefaultConstructible>::value) == (std::is_default_constructible<NotDefaultConstructible>::value));
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
    TEST(test_is_copy_assignable)
    {
#if ETL_USING_CPP17
      CHECK((etl::is_copy_assignable_v<Copyable>) == (std::is_copy_assignable_v<Copyable>));
      CHECK((etl::is_copy_assignable_v<Moveable>) == (std::is_copy_assignable_v<Moveable>));
      CHECK((etl::is_copy_assignable_v<MoveableCopyable>) == (std::is_copy_assignable_v<MoveableCopyable>));
#else
      CHECK((etl::is_copy_assignable<Copyable>::value) == (std::is_copy_assignable<Copyable>::value));
      CHECK((etl::is_copy_assignable<Moveable>::value) == (std::is_copy_assignable<Moveable>::value));
      CHECK((etl::is_copy_assignable<MoveableCopyable>::value) == (std::is_copy_assignable<MoveableCopyable>::value));
#endif
    }

    //*************************************************************************
    TEST(test_is_move_assignable)
    {
#if ETL_USING_CPP17
      CHECK((etl::is_move_assignable_v<Copyable>) == (std::is_move_assignable_v<Copyable>));
      CHECK((etl::is_move_assignable_v<Moveable>) == (std::is_move_assignable_v<Moveable>));
      CHECK((etl::is_move_assignable_v<MoveableCopyable>) == (std::is_move_assignable_v<MoveableCopyable>));
#else
      CHECK((etl::is_move_assignable<Copyable>::value) == (std::is_move_assignable<Copyable>::value));
      CHECK((etl::is_move_assignable<Moveable>::value) == (std::is_move_assignable<Moveable>::value));
      CHECK((etl::is_move_assignable<MoveableCopyable>::value) == (std::is_move_assignable<MoveableCopyable>::value));
#endif
    }

    //*************************************************************************
    TEST(test_is_nothrow_constructible)
    {
#if defined(ETL_USE_TYPE_TRAITS_BUILTINS) || (ETL_USING_STL && !defined(ETL_USER_DEFINED_TYPE_TRAITS))
      CHECK((etl::is_nothrow_constructible<int>::value) == true);
      CHECK((etl::is_nothrow_constructible<Copyable>::value) == false);
      CHECK((etl::is_nothrow_constructible<Moveable>::value) == false);
      CHECK((etl::is_nothrow_constructible<MoveableCopyable>::value) == false);
      CHECK((etl::is_nothrow_constructible<int>::value) == (std::is_nothrow_constructible<int>::value));
      CHECK((etl::is_nothrow_constructible<Copyable>::value) == (std::is_nothrow_constructible<Copyable>::value));
      CHECK((etl::is_nothrow_constructible<Moveable>::value) == (std::is_nothrow_constructible<Moveable>::value));
      CHECK((etl::is_nothrow_constructible<MoveableCopyable>::value) == (std::is_nothrow_constructible<MoveableCopyable>::value));
  #if ETL_USING_CPP17
      CHECK((etl::is_nothrow_constructible_v<int>) == (std::is_nothrow_constructible_v<int>));
      CHECK((etl::is_nothrow_constructible_v<Copyable>) == (std::is_nothrow_constructible_v<Copyable>));
      CHECK((etl::is_nothrow_constructible_v<Moveable>) == (std::is_nothrow_constructible_v<Moveable>));
      CHECK((etl::is_nothrow_constructible_v<MoveableCopyable>) == (std::is_nothrow_constructible_v<MoveableCopyable>));
  #endif
#endif
    }

    //*************************************************************************
    TEST(test_is_nothrow_default_constructible)
    {
#if defined(ETL_USE_TYPE_TRAITS_BUILTINS) || (ETL_USING_STL && !defined(ETL_USER_DEFINED_TYPE_TRAITS))
      CHECK((etl::is_nothrow_default_constructible<int>::value) == true);
      CHECK((etl::is_nothrow_default_constructible<Copyable>::value) == false);
      CHECK((etl::is_nothrow_default_constructible<Moveable>::value) == false);
      CHECK((etl::is_nothrow_default_constructible<MoveableCopyable>::value) == false);
      CHECK((etl::is_nothrow_default_constructible<int>::value) == (std::is_nothrow_default_constructible<int>::value));
      CHECK((etl::is_nothrow_default_constructible<Copyable>::value) == (std::is_nothrow_default_constructible<Copyable>::value));
      CHECK((etl::is_nothrow_default_constructible<Moveable>::value) == (std::is_nothrow_default_constructible<Moveable>::value));
      CHECK((etl::is_nothrow_default_constructible<MoveableCopyable>::value) == (std::is_nothrow_default_constructible<MoveableCopyable>::value));
  #if ETL_USING_CPP17
      CHECK((etl::is_nothrow_default_constructible_v<int>) == (std::is_nothrow_default_constructible_v<int>));
      CHECK((etl::is_nothrow_default_constructible_v<Copyable>) == (std::is_nothrow_default_constructible_v<Copyable>));
      CHECK((etl::is_nothrow_default_constructible_v<Moveable>) == (std::is_nothrow_default_constructible_v<Moveable>));
      CHECK((etl::is_nothrow_default_constructible_v<MoveableCopyable>) == (std::is_nothrow_default_constructible_v<MoveableCopyable>));
  #endif
#endif
    }

    //*************************************************************************
    TEST(test_is_nothrow_copy_constructible)
    {
#if defined(ETL_USE_TYPE_TRAITS_BUILTINS) || (ETL_USING_STL && !defined(ETL_USER_DEFINED_TYPE_TRAITS))
      CHECK((etl::is_nothrow_copy_constructible<int>::value) == true);
      CHECK((etl::is_nothrow_copy_constructible<Copyable>::value) == true);
      CHECK((etl::is_nothrow_copy_constructible<Moveable>::value) == false);
      CHECK((etl::is_nothrow_copy_constructible<MoveableCopyable>::value) == false);
      CHECK((etl::is_nothrow_copy_constructible<int>::value) == (std::is_nothrow_copy_constructible<int>::value));
      CHECK((etl::is_nothrow_copy_constructible<Copyable>::value) == (std::is_nothrow_copy_constructible<Copyable>::value));
      CHECK((etl::is_nothrow_copy_constructible<Moveable>::value) == (std::is_nothrow_copy_constructible<Moveable>::value));
      CHECK((etl::is_nothrow_copy_constructible<MoveableCopyable>::value) == (std::is_nothrow_copy_constructible<MoveableCopyable>::value));
  #if ETL_USING_CPP17
      CHECK((etl::is_nothrow_copy_constructible_v<int>) == (std::is_nothrow_copy_constructible_v<int>));
      CHECK((etl::is_nothrow_copy_constructible_v<Copyable>) == (std::is_nothrow_copy_constructible_v<Copyable>));
      CHECK((etl::is_nothrow_copy_constructible_v<Moveable>) == (std::is_nothrow_copy_constructible_v<Moveable>));
      CHECK((etl::is_nothrow_copy_constructible_v<MoveableCopyable>) == (std::is_nothrow_copy_constructible_v<MoveableCopyable>));
  #endif
#endif
    }

    //*************************************************************************
    TEST(test_is_nothrow_move_constructible)
    {
#if defined(ETL_USE_TYPE_TRAITS_BUILTINS) || (ETL_USING_STL && !defined(ETL_USER_DEFINED_TYPE_TRAITS))
      CHECK((etl::is_nothrow_move_constructible<int>::value) == true);
      CHECK((etl::is_nothrow_move_constructible<Copyable>::value) == false);
      CHECK((etl::is_nothrow_move_constructible<Moveable>::value) == true);
      CHECK((etl::is_nothrow_move_constructible<MoveableCopyable>::value) == true);
      CHECK((etl::is_nothrow_move_constructible<int>::value) == (std::is_nothrow_move_constructible<int>::value));
      CHECK((etl::is_nothrow_move_constructible<Copyable>::value) == (std::is_nothrow_move_constructible<Copyable>::value));
      CHECK((etl::is_nothrow_move_constructible<Moveable>::value) == (std::is_nothrow_move_constructible<Moveable>::value));
      CHECK((etl::is_nothrow_move_constructible<MoveableCopyable>::value) == (std::is_nothrow_move_constructible<MoveableCopyable>::value));
  #if ETL_USING_CPP17
      CHECK((etl::is_nothrow_move_constructible_v<int>) == (std::is_nothrow_move_constructible_v<int>));
      CHECK((etl::is_nothrow_move_constructible_v<Copyable>) == (std::is_nothrow_move_constructible_v<Copyable>));
      CHECK((etl::is_nothrow_move_constructible_v<Moveable>) == (std::is_nothrow_move_constructible_v<Moveable>));
      CHECK((etl::is_nothrow_move_constructible_v<MoveableCopyable>) == (std::is_nothrow_move_constructible_v<MoveableCopyable>));
  #endif
#endif
    }

    //*************************************************************************
    TEST(test_is_nothrow_assignable)
    {
#if defined(ETL_USE_TYPE_TRAITS_BUILTINS) || (ETL_USING_STL && !defined(ETL_USER_DEFINED_TYPE_TRAITS))
      CHECK((etl::is_nothrow_assignable<int&, int>::value) == true);
      CHECK((etl::is_nothrow_assignable<Copyable&, Copyable>::value) == false);
      CHECK((etl::is_nothrow_assignable<Moveable&, Moveable>::value) == true);
      CHECK((etl::is_nothrow_assignable<MoveableCopyable&, MoveableCopyable>::value) == true);
      CHECK((etl::is_nothrow_assignable<int&, int>::value) == (std::is_nothrow_assignable<int&, int>::value));
      CHECK((etl::is_nothrow_assignable<Copyable&, Copyable>::value) == (std::is_nothrow_assignable<Copyable&, Copyable>::value));
      CHECK((etl::is_nothrow_assignable<Moveable&, Moveable>::value) == (std::is_nothrow_assignable<Moveable&, Moveable>::value));
      CHECK((etl::is_nothrow_assignable<MoveableCopyable&, MoveableCopyable>::value)
            == (std::is_nothrow_assignable<MoveableCopyable&, MoveableCopyable>::value));
  #if ETL_USING_CPP17
      CHECK((etl::is_nothrow_assignable_v<int&, int>) == (std::is_nothrow_assignable_v<int&, int>));
      CHECK((etl::is_nothrow_assignable_v<Copyable&, Copyable>) == (std::is_nothrow_assignable_v<Copyable&, Copyable>));
      CHECK((etl::is_nothrow_assignable_v<Moveable&, Moveable>) == (std::is_nothrow_assignable_v<Moveable&, Moveable>));
      CHECK(
        (etl::is_nothrow_assignable_v<MoveableCopyable&, MoveableCopyable>) == (std::is_nothrow_assignable_v<MoveableCopyable&, MoveableCopyable>));
  #endif
#endif
    }

    //*************************************************************************
    TEST(test_is_nothrow_copy_assignable)
    {
#if defined(ETL_USE_TYPE_TRAITS_BUILTINS) || (ETL_USING_STL && !defined(ETL_USER_DEFINED_TYPE_TRAITS))
      CHECK((etl::is_nothrow_copy_assignable<int>::value) == true);
      CHECK((etl::is_nothrow_copy_assignable<Copyable>::value) == true);
      CHECK((etl::is_nothrow_copy_assignable<Moveable>::value) == false);
      CHECK((etl::is_nothrow_copy_assignable<MoveableCopyable>::value) == false);
      CHECK((etl::is_nothrow_copy_assignable<int>::value) == (std::is_nothrow_copy_assignable<int>::value));
      CHECK((etl::is_nothrow_copy_assignable<Copyable>::value) == (std::is_nothrow_copy_assignable<Copyable>::value));
      CHECK((etl::is_nothrow_copy_assignable<Moveable>::value) == (std::is_nothrow_copy_assignable<Moveable>::value));
      CHECK((etl::is_nothrow_copy_assignable<MoveableCopyable>::value) == (std::is_nothrow_copy_assignable<MoveableCopyable>::value));
  #if ETL_USING_CPP17
      CHECK((etl::is_nothrow_copy_assignable_v<int>) == (std::is_nothrow_copy_assignable_v<int>));
      CHECK((etl::is_nothrow_copy_assignable_v<Copyable>) == (std::is_nothrow_copy_assignable_v<Copyable>));
      CHECK((etl::is_nothrow_copy_assignable_v<Moveable>) == (std::is_nothrow_copy_assignable_v<Moveable>));
      CHECK((etl::is_nothrow_copy_assignable_v<MoveableCopyable>) == (std::is_nothrow_copy_assignable_v<MoveableCopyable>));
  #endif
#endif
    }

    //*************************************************************************
    TEST(test_is_nothrow_move_assignable)
    {
#if defined(ETL_USE_TYPE_TRAITS_BUILTINS) || (ETL_USING_STL && !defined(ETL_USER_DEFINED_TYPE_TRAITS))
      CHECK((etl::is_nothrow_move_assignable<int>::value) == true);
      CHECK((etl::is_nothrow_move_assignable<Copyable>::value) == false);
      CHECK((etl::is_nothrow_move_assignable<Moveable>::value) == true);
      CHECK((etl::is_nothrow_move_assignable<MoveableCopyable>::value) == true);
      CHECK((etl::is_nothrow_move_assignable<int>::value) == (std::is_nothrow_move_assignable<int>::value));
      CHECK((etl::is_nothrow_move_assignable<Copyable>::value) == (std::is_nothrow_move_assignable<Copyable>::value));
      CHECK((etl::is_nothrow_move_assignable<Moveable>::value) == (std::is_nothrow_move_assignable<Moveable>::value));
      CHECK((etl::is_nothrow_move_assignable<MoveableCopyable>::value) == (std::is_nothrow_move_assignable<MoveableCopyable>::value));
  #if ETL_USING_CPP17
      CHECK((etl::is_nothrow_move_assignable_v<int>) == (std::is_nothrow_move_assignable_v<int>));
      CHECK((etl::is_nothrow_move_assignable_v<Copyable>) == (std::is_nothrow_move_assignable_v<Copyable>));
      CHECK((etl::is_nothrow_move_assignable_v<Moveable>) == (std::is_nothrow_move_assignable_v<Moveable>));
      CHECK((etl::is_nothrow_move_assignable_v<MoveableCopyable>) == (std::is_nothrow_move_assignable_v<MoveableCopyable>));
  #endif
#endif
    }

    //*************************************************************************
    TEST(test_is_trivially_constructible)
    {
#if ETL_USING_STL || defined(ETL_USE_TYPE_TRAITS_BUILTINS) || defined(ETL_USER_DEFINED_TYPE_TRAITS) || ETL_USING_BUILTIN_IS_TRIVIALLY_CONSTRUCTIBLE \
  || ETL_USING_BUILTIN_HAS_TRIVIAL_CONSTRUCTOR
  #if ETL_USING_CPP17
      CHECK((etl::is_trivially_constructible_v<Copyable>) == (std::is_trivially_constructible_v<Copyable>));
      CHECK((etl::is_trivially_constructible_v<Moveable>) == (std::is_trivially_constructible_v<Moveable>));
      CHECK((etl::is_trivially_constructible_v<MoveableCopyable>) == (std::is_trivially_constructible_v<MoveableCopyable>));
  #else
      CHECK((etl::is_trivially_constructible<Copyable>::value) == (std::is_trivially_constructible<Copyable>::value));
      CHECK((etl::is_trivially_constructible<Moveable>::value) == (std::is_trivially_constructible<Moveable>::value));
      CHECK((etl::is_trivially_constructible<MoveableCopyable>::value) == (std::is_trivially_constructible<MoveableCopyable>::value));
  #endif
  #if ETL_USING_CPP11
      CHECK(!etl::is_trivially_constructible<PrivateTrivialDefaultConstructor>::value);
      CHECK((etl::is_trivially_constructible<PrivateTrivialDefaultConstructor>::value)
            == (std::is_trivially_constructible<PrivateTrivialDefaultConstructor>::value));
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
    TEST(test_is_trivially_default_constructible)
    {
#if (!(defined(ETL_COMPILER_GCC) && defined(ETL_USE_TYPE_TRAITS_BUILTINS)))
  #if ETL_USING_STL || defined(ETL_USE_TYPE_TRAITS_BUILTINS) || defined(ETL_USER_DEFINED_TYPE_TRAITS)
      CHECK((etl::is_trivially_default_constructible<int>::value) == (std::is_trivially_default_constructible<int>::value));
      CHECK((etl::is_trivially_default_constructible<Copyable>::value) == (std::is_trivially_default_constructible<Copyable>::value));
      CHECK((etl::is_trivially_default_constructible<Moveable>::value) == (std::is_trivially_default_constructible<Moveable>::value));
      CHECK((etl::is_trivially_default_constructible<MoveableCopyable>::value) == (std::is_trivially_default_constructible<MoveableCopyable>::value));

    #if ETL_USING_CPP17
      CHECK((etl::is_trivially_default_constructible_v<int>) == (std::is_trivially_default_constructible_v<int>));
      CHECK((etl::is_trivially_default_constructible_v<Copyable>) == (std::is_trivially_default_constructible_v<Copyable>));
    #endif
  #endif
#endif
    }

    //*************************************************************************
    TEST(test_is_trivially_move_constructible)
    {
#if (!(defined(ETL_COMPILER_GCC) && defined(ETL_USE_TYPE_TRAITS_BUILTINS)))
  #if ETL_USING_CPP11 && (ETL_USING_STL || defined(ETL_USE_TYPE_TRAITS_BUILTINS) || defined(ETL_USER_DEFINED_TYPE_TRAITS))
      CHECK((etl::is_trivially_move_constructible<int>::value) == (std::is_trivially_move_constructible<int>::value));
      CHECK((etl::is_trivially_move_constructible<Copyable>::value) == (std::is_trivially_move_constructible<Copyable>::value));
      CHECK((etl::is_trivially_move_constructible<Moveable>::value) == (std::is_trivially_move_constructible<Moveable>::value));
      CHECK((etl::is_trivially_move_constructible<MoveableCopyable>::value) == (std::is_trivially_move_constructible<MoveableCopyable>::value));

    #if ETL_USING_CPP17
      CHECK((etl::is_trivially_move_constructible_v<int>) == (std::is_trivially_move_constructible_v<int>));
      CHECK((etl::is_trivially_move_constructible_v<Moveable>) == (std::is_trivially_move_constructible_v<Moveable>));
    #endif
  #endif
#endif
    }

    //*************************************************************************
    TEST(test_is_trivially_destructible)
    {
#if ETL_USING_STL || defined(ETL_USE_TYPE_TRAITS_BUILTINS) || defined(ETL_USER_DEFINED_TYPE_TRAITS) || ETL_USING_BUILTIN_IS_TRIVIALLY_DESTRUCTIBLE \
  || ETL_USING_BUILTIN_HAS_TRIVIAL_DESTRUCTOR
  #if ETL_USING_CPP17
      CHECK((etl::is_trivially_destructible_v<Copyable>) == (std::is_trivially_destructible_v<Copyable>));
      CHECK((etl::is_trivially_destructible_v<Moveable>) == (std::is_trivially_destructible_v<Moveable>));
      CHECK((etl::is_trivially_destructible_v<MoveableCopyable>) == (std::is_trivially_destructible_v<MoveableCopyable>));
  #else
      CHECK((etl::is_trivially_destructible<Copyable>::value) == (std::is_trivially_destructible<Copyable>::value));
      CHECK((etl::is_trivially_destructible<Moveable>::value) == (std::is_trivially_destructible<Moveable>::value));
      CHECK((etl::is_trivially_destructible<MoveableCopyable>::value) == (std::is_trivially_destructible<MoveableCopyable>::value));
  #endif
  #if ETL_USING_CPP11
      CHECK(!etl::is_trivially_destructible<PrivateTrivialDestructor>::value);
      CHECK(!etl::is_trivially_destructible<DeletedTrivialDestructor>::value);
      CHECK((etl::is_trivially_destructible<PrivateTrivialDestructor>::value) == (std::is_trivially_destructible<PrivateTrivialDestructor>::value));
      CHECK((etl::is_trivially_destructible<DeletedTrivialDestructor>::value) == (std::is_trivially_destructible<DeletedTrivialDestructor>::value));
  #endif
#endif
    }

    //*************************************************************************
    TEST(test_is_trivially_assignable)
    {
#if ETL_USING_CPP11 && !defined(ETL_USER_DEFINED_TYPE_TRAITS) \
  && (ETL_CPP11_TYPE_TRAITS_IS_TRIVIAL_SUPPORTED || ETL_USING_BUILTIN_IS_TRIVIALLY_ASSIGNABLE)
      CHECK((etl::is_trivially_assignable<int&, int>::value) == (std::is_trivially_assignable<int&, int>::value));
      CHECK((etl::is_trivially_assignable<int, int>::value) == (std::is_trivially_assignable<int, int>::value));
      CHECK((etl::is_trivially_assignable<Copyable&, const Copyable&>::value) == (std::is_trivially_assignable<Copyable&, const Copyable&>::value));
      CHECK((etl::is_trivially_assignable<Moveable&, Moveable&&>::value) == (std::is_trivially_assignable<Moveable&, Moveable&&>::value));
      CHECK((etl::is_trivially_assignable<MoveableCopyable&, const MoveableCopyable&>::value)
            == (std::is_trivially_assignable<MoveableCopyable&, const MoveableCopyable&>::value));

  #if ETL_USING_CPP17
      CHECK((etl::is_trivially_assignable_v<int&, int>) == (std::is_trivially_assignable_v<int&, int>));
      CHECK((etl::is_trivially_assignable_v<Copyable&, const Copyable&>) == (std::is_trivially_assignable_v<Copyable&, const Copyable&>));
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
    TEST(test_is_trivially_move_assignable)
    {
#if ETL_USING_CPP11 && !defined(ETL_USER_DEFINED_TYPE_TRAITS) \
  && (ETL_CPP11_TYPE_TRAITS_IS_TRIVIAL_SUPPORTED || ETL_USING_BUILTIN_IS_TRIVIALLY_ASSIGNABLE)
      CHECK((etl::is_trivially_move_assignable<int>::value) == (std::is_trivially_move_assignable<int>::value));
      CHECK((etl::is_trivially_move_assignable<Copyable>::value) == (std::is_trivially_move_assignable<Copyable>::value));
      CHECK((etl::is_trivially_move_assignable<Moveable>::value) == (std::is_trivially_move_assignable<Moveable>::value));
      CHECK((etl::is_trivially_move_assignable<MoveableCopyable>::value) == (std::is_trivially_move_assignable<MoveableCopyable>::value));

  #if ETL_USING_CPP17
      CHECK((etl::is_trivially_move_assignable_v<int>) == (std::is_trivially_move_assignable_v<int>));
      CHECK((etl::is_trivially_move_assignable_v<Moveable>) == (std::is_trivially_move_assignable_v<Moveable>));
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

    //*************************************************************************
    TEST(test_is_trivially_relocatable)
    {
      // Trivially relocatable types (trivially copyable and trivially destructible)
      // Primitive types should always be detected as trivially relocatable
      CHECK_TRUE(etl::is_trivially_relocatable<int>::value);
      CHECK_TRUE(etl::is_trivially_relocatable<double>::value);
      CHECK_TRUE(etl::is_trivially_relocatable<int*>::value);
      CHECK_TRUE(etl::is_trivially_relocatable<const int*>::value);

      // POD struct should be trivially relocatable when proper detection is available
      struct TrivialStruct
      {
        int    x;
        double y;
      };

      // Struct with non-trivial destructor should NOT be trivially relocatable
      struct NonTrivialDestructor
      {
        ~NonTrivialDestructor() {}
      };
      CHECK_FALSE(etl::is_trivially_relocatable<NonTrivialDestructor>::value);

      // Struct with non-trivial copy constructor should NOT be trivially relocatable
      struct NonTrivialCopy
      {
        NonTrivialCopy() = default;
        NonTrivialCopy(const NonTrivialCopy&) {}
        NonTrivialCopy& operator=(const NonTrivialCopy&) = default;
      };
      CHECK_FALSE(etl::is_trivially_relocatable<NonTrivialCopy>::value);

#if ETL_USING_STL || ETL_USING_BUILTIN_IS_TRIVIALLY_RELOCATABLE
      // These tests require STL or compiler builtins to correctly detect struct/array triviality
      CHECK_TRUE(etl::is_trivially_relocatable<int[10]>::value);
      CHECK_TRUE(etl::is_trivially_relocatable<TrivialStruct>::value);
      CHECK_TRUE(etl::is_trivially_relocatable<TrivialStruct[5]>::value);
#endif

#if ETL_USING_CPP17
      // Test the _v helper variable
      CHECK_TRUE(etl::is_trivially_relocatable_v<int>);
      CHECK_TRUE(etl::is_trivially_relocatable_v<double>);
      CHECK_TRUE(etl::is_trivially_relocatable_v<int*>);
  #if ETL_USING_STL || ETL_USING_BUILTIN_IS_TRIVIALLY_RELOCATABLE
      CHECK_TRUE(etl::is_trivially_relocatable_v<TrivialStruct>);
  #endif
      CHECK_FALSE(etl::is_trivially_relocatable_v<NonTrivialDestructor>);
      CHECK_FALSE(etl::is_trivially_relocatable_v<NonTrivialCopy>);
#endif

      // Verify consistency: if a type is trivially_copyable AND trivially_destructible,
      // then it should be trivially_relocatable. The reverse may not hold when compiler
      // builtins provide more accurate detection than the fallback implementations.
      CHECK_TRUE(!(etl::is_trivially_copyable<int>::value && etl::is_trivially_destructible<int>::value)
                 || etl::is_trivially_relocatable<int>::value);
      CHECK_TRUE(!(etl::is_trivially_copyable<TrivialStruct>::value && etl::is_trivially_destructible<TrivialStruct>::value)
                 || etl::is_trivially_relocatable<TrivialStruct>::value);
      // Non-trivially destructible types should never be trivially relocatable
      CHECK_FALSE(etl::is_trivially_relocatable<NonTrivialDestructor>::value);
      // Non-trivially copyable types should never be trivially relocatable
      CHECK_FALSE(etl::is_trivially_relocatable<NonTrivialCopy>::value);
    }

    //*************************************************************************
    TEST(test_is_nothrow_relocatable)
    {
      // Trivially relocatable types should always be nothrow relocatable
      // Primitive types should always be detected correctly
      CHECK_TRUE(etl::is_nothrow_relocatable<int>::value);
      CHECK_TRUE(etl::is_nothrow_relocatable<double>::value);
      CHECK_TRUE(etl::is_nothrow_relocatable<int*>::value);
      CHECK_TRUE(etl::is_nothrow_relocatable<const int*>::value);

      // POD struct should be nothrow relocatable when proper detection is available
      struct TrivialStruct
      {
        int    x;
        double y;
      };

#if ETL_USING_STL || ETL_USING_BUILTIN_IS_TRIVIALLY_RELOCATABLE
      // These tests require STL or compiler builtins to correctly detect struct/array triviality
      CHECK_TRUE(etl::is_nothrow_relocatable<int[10]>::value);
      CHECK_TRUE(etl::is_nothrow_relocatable<TrivialStruct>::value);
      CHECK_TRUE(etl::is_nothrow_relocatable<TrivialStruct[5]>::value);
#endif

#if ETL_USING_CPP17
      // Test the _v helper variable
      CHECK_TRUE(etl::is_nothrow_relocatable_v<int>);
      CHECK_TRUE(etl::is_nothrow_relocatable_v<double>);
      CHECK_TRUE(etl::is_nothrow_relocatable_v<int*>);
  #if ETL_USING_STL || ETL_USING_BUILTIN_IS_TRIVIALLY_RELOCATABLE
      CHECK_TRUE(etl::is_nothrow_relocatable_v<TrivialStruct>);
  #endif
#endif

      // Verify consistency: nothrow_relocatable should be at least as permissive as trivially_relocatable
      CHECK_TRUE(!etl::is_trivially_relocatable<int>::value || etl::is_nothrow_relocatable<int>::value);
      CHECK_TRUE(!etl::is_trivially_relocatable<TrivialStruct>::value || etl::is_nothrow_relocatable<TrivialStruct>::value);
    }

    //*************************************************************************
    TEST(test_is_base_of_any)
    {
      struct Base
      {
      };
      struct D1 : Base
      {
      };
      struct D2 : Base
      {
      };
      struct D3 : Base
      {
      };
      struct D4
      {
      };

#if ETL_USING_CPP17
      CHECK_TRUE(bool(etl::is_base_of_any_v<Base, D1, D2, D3, D4>));
      CHECK_FALSE(bool(etl::is_base_of_any_v<Base, D4>));
#else
      CHECK_TRUE(bool(etl::is_base_of_any<Base, D1, D2, D3, D4>::value));
      CHECK_FALSE(bool(etl::is_base_of_any<Base, D4>::value));
#endif
    }

    //*************************************************************************
    TEST(test_is_base_of_all)
    {
      struct Base
      {
      };
      struct D1 : Base
      {
      };
      struct D2 : Base
      {
      };
      struct D3 : Base
      {
      };
      struct D4
      {
      };

#if ETL_USING_CPP17
      CHECK_TRUE(bool(etl::is_base_of_all_v<Base, D1, D2, D3>));
      CHECK_FALSE(bool(etl::is_base_of_all_v<Base, D1, D2, D3, D4>));
#else
      CHECK_TRUE(bool(etl::is_base_of_all<Base, D1, D2, D3>::value));
      CHECK_FALSE(bool(etl::is_base_of_all<Base, D1, D2, D3, D4>::value));
#endif
    }

    //*************************************************************************
    TEST(test_nth_base)
    {
      struct D0
      {
      };
      struct D1 : D0
      {
        using base_type = D0;
      };
      struct D2 : D1
      {
        using base_type = D1;
      };
      struct D3 : D2
      {
        using base_type = D2;
      };
      struct D4 : D3
      {
        using base_type = D3;
      };

      CHECK_TRUE((std::is_same<D4, etl::nth_base_t<0, D4>>::value));
      CHECK_TRUE((std::is_same<D3, etl::nth_base_t<1, D4>>::value));
      CHECK_TRUE((std::is_same<D2, etl::nth_base_t<2, D4>>::value));
      CHECK_TRUE((std::is_same<D1, etl::nth_base_t<3, D4>>::value));
      CHECK_TRUE((std::is_same<D0, etl::nth_base_t<4, D4>>::value));
    }

    //*************************************************************************
    TEST(test_type_identity)
    {
      CHECK_CLOSE(type_identity_test_add(1.5f, 2), 3.5f, 0.01f);
    }

    //*************************************************************************
#if ETL_USING_BUILTIN_UNDERLYING_TYPE
    TEST(test_underlying_type)
    {
      enum enum0_t : char
      {
      };

      enum enum1_t : uint32_t
      {
      };

      enum class enum2_t : short
      {
      };

      enum class enum3_t : size_t
      {
      };

      using enum4_t = enum1_t;
      using enum5_t = std::add_const<enum2_t>::type;

      CHECK_TRUE((std::is_same<etl::underlying_type<enum0_t>::type, char>::value));
      CHECK_TRUE((std::is_same<etl::underlying_type<enum1_t>::type, uint32_t>::value));
      CHECK_TRUE((std::is_same<etl::underlying_type<enum2_t>::type, short>::value));
      CHECK_TRUE((std::is_same<etl::underlying_type<enum3_t>::type, size_t>::value));
      CHECK_TRUE((std::is_same<etl::underlying_type<enum4_t>::type, uint32_t>::value));
      CHECK_TRUE((std::is_same<etl::underlying_type<enum5_t>::type, short>::value));
  #if ETL_USING_CPP11
      CHECK_TRUE((std::is_same<etl::underlying_type_t<enum0_t>, char>::value));
      CHECK_TRUE((std::is_same<etl::underlying_type_t<enum1_t>, uint32_t>::value));
      CHECK_TRUE((std::is_same<etl::underlying_type_t<enum2_t>, short>::value));
      CHECK_TRUE((std::is_same<etl::underlying_type_t<enum3_t>, size_t>::value));
      CHECK_TRUE((std::is_same<etl::underlying_type_t<enum4_t>, uint32_t>::value));
      CHECK_TRUE((std::is_same<etl::underlying_type_t<enum5_t>, short>::value));
  #endif
    }
#endif

    //*************************************************************************
    TEST(test_has_duplicates)
    {
#if ETL_USING_CPP17
      CHECK_FALSE((etl::has_duplicates_v<char>));
      CHECK_FALSE((etl::has_duplicates_v<char, int, double>));
      CHECK_TRUE((etl::has_duplicates_v<char, int, char>));
#else
      CHECK_FALSE((etl::has_duplicates<char>::value));
      CHECK_FALSE((etl::has_duplicates<char, int, double>::value));
      CHECK_TRUE((etl::has_duplicates<char, int, char>::value));
#endif
    }

    //*************************************************************************
    TEST(test_has_duplicates_of)
    {
#if ETL_USING_CPP17
      CHECK_FALSE((etl::has_duplicates_of_v<char>));
      CHECK_TRUE((etl::has_duplicates_of_v<char, char, int,
                                           char>)); // char is duplicated.
      CHECK_FALSE((etl::has_duplicates_of_v<int, char, int,
                                            char>)); // int is not duplicated.
#else
      CHECK_FALSE((etl::has_duplicates_of<char>::value));
      CHECK_TRUE((etl::has_duplicates_of<char, char, int,
                                         char>::value));                  // char is duplicated.
      CHECK_FALSE((etl::has_duplicates_of<int, char, int, char>::value)); // int is not
                                                                          // duplicated.
#endif
    }

    //*************************************************************************
    TEST(test_count_of)
    {
#if ETL_USING_CPP17
      CHECK_EQUAL(0, (etl::count_of_v<char>));
      CHECK_EQUAL(0, (etl::count_of_v<char, int>));
      CHECK_EQUAL(1, (etl::count_of_v<char, char>));
      CHECK_EQUAL(1, (etl::count_of_v<char, int, char>));
      CHECK_EQUAL(2, (etl::count_of_v<char, int, char, double, char>));
#else
      CHECK_EQUAL(0, (etl::count_of<char>::value));
      CHECK_EQUAL(0, (etl::count_of<char, int>::value));
      CHECK_EQUAL(1, (etl::count_of<char, char>::value));
      CHECK_EQUAL(1, (etl::count_of<char, int, char>::value));
      CHECK_EQUAL(2, (etl::count_of<char, int, char, double, char>::value));
#endif
    }

    //*************************************************************************
    TEST(test_is_specialization)
    {
#if ETL_USING_CPP17
      CHECK_TRUE((etl::is_specialization_v<specialized<int>, specialized>));
      CHECK_FALSE((etl::is_specialization_v<other_specialized<int>, specialized>));
#else
      CHECK_TRUE((etl::is_specialization<specialized<int>, specialized>::value));
      CHECK_FALSE((etl::is_specialization<other_specialized<int>, specialized>::value));
#endif
    }

#if ETL_USING_CPP11
    //*************************************************************************
    TEST(test_is_constant_evaluated)
    {
      constexpr bool c0 = etl::is_constant_evaluated();
  #if !ETL_USING_CPP23 && defined(ETL_COMPILER_MICROSOFT)
      // Not supported on MSVC via __has_builtin, see
      // determine_builtin_support.h
      CHECK_FALSE(c0);
  #else
      CHECK_TRUE(c0);
  #endif

      volatile int i  = 1;
      const bool   c1 = (((i == 1) && etl::is_constant_evaluated()) ? true : false);
      CHECK_FALSE(c1);
    }
#endif

    //*************************************************************************
    // Basic cv for member function pointers
    TEST(test_is_member_function_pointer)
    {
      CHECK_TRUE((etl::is_member_function_pointer<decltype(&MF::f)>::value));
      CHECK_TRUE((etl::is_member_function_pointer<decltype(&MF::fc)>::value));
      CHECK_TRUE((etl::is_member_function_pointer<decltype(&MF::fv)>::value));
      CHECK_TRUE((etl::is_member_function_pointer<decltype(&MF::fcv)>::value));

      CHECK_TRUE((etl::is_member_function_pointer<decltype(&MF::fl)>::value));
      CHECK_TRUE((etl::is_member_function_pointer<decltype(&MF::flc)>::value));
      CHECK_TRUE((etl::is_member_function_pointer<decltype(&MF::flv)>::value));
      CHECK_TRUE((etl::is_member_function_pointer<decltype(&MF::flcv)>::value));

      CHECK_TRUE((etl::is_member_function_pointer<decltype(&MF::fr)>::value));
      CHECK_TRUE((etl::is_member_function_pointer<decltype(&MF::frc)>::value));
      CHECK_TRUE((etl::is_member_function_pointer<decltype(&MF::frv)>::value));
      CHECK_TRUE((etl::is_member_function_pointer<decltype(&MF::frcv)>::value));

#if ETL_HAS_NOEXCEPT_FUNCTION_TYPE
      CHECK_TRUE((etl::is_member_function_pointer<decltype(&MF::fn)>::value));
      CHECK_TRUE((etl::is_member_function_pointer<decltype(&MF::fnc)>::value));
      CHECK_TRUE((etl::is_member_function_pointer<decltype(&MF::fnv)>::value));
      CHECK_TRUE((etl::is_member_function_pointer<decltype(&MF::fncv)>::value));

      CHECK_TRUE((etl::is_member_function_pointer<decltype(&MF::fnl)>::value));
      CHECK_TRUE((etl::is_member_function_pointer<decltype(&MF::fnlc)>::value));
      CHECK_TRUE((etl::is_member_function_pointer<decltype(&MF::fnlv)>::value));
      CHECK_TRUE((etl::is_member_function_pointer<decltype(&MF::fnlcv)>::value));

      CHECK_TRUE((etl::is_member_function_pointer<decltype(&MF::fnr)>::value));
      CHECK_TRUE((etl::is_member_function_pointer<decltype(&MF::fnrc)>::value));
      CHECK_TRUE((etl::is_member_function_pointer<decltype(&MF::fnrv)>::value));
      CHECK_TRUE((etl::is_member_function_pointer<decltype(&MF::fnrcv)>::value));
#endif

      CHECK_TRUE((etl::is_member_function_pointer<decltype(&MF::fvar)>::value));
      CHECK_TRUE((etl::is_member_function_pointer<decltype(&MF::fvarc)>::value));
    }

    //*************************************************************************
    // Negative tests for member function pointer trait
    TEST(test_is_member_function_pointer_negative)
    {
      (void)f(0);

      // Free function pointer
      CHECK_FALSE((etl::is_member_function_pointer<decltype(&f)>::value));

      // Member object pointer
      CHECK_FALSE((etl::is_member_function_pointer<int MF::*>::value));

      // Plain function type (not pointer)
      CHECK_FALSE((etl::is_member_function_pointer<int(int)>::value));

      // Non-function type
      CHECK_FALSE((etl::is_member_function_pointer<int>::value));
    }

    //*************************************************************************
    // Member object pointer trait
    TEST(test_is_member_object_pointer)
    {
      CHECK_TRUE((etl::is_member_object_pointer<int MO::*>::value));

      // Not a member object pointer
      CHECK_FALSE((etl::is_member_object_pointer<int (MF::*)(int)>::value));
      CHECK_FALSE((etl::is_member_object_pointer<int*>::value));
    }

    //*************************************************************************
    // Member pointer (either member object or member function pointer)
    TEST(test_is_member_pointer_any)
    {
      CHECK_TRUE((etl::is_member_pointer<int MF::*>::value));
      CHECK_TRUE((etl::is_member_pointer<int (MF::*)(int)>::value));

      // Not member pointers
      CHECK_FALSE((etl::is_member_pointer<int*>::value));
      CHECK_FALSE((etl::is_member_pointer<decltype(f)>::value));
    }

    //*************************************************************************
    // Function type detection
    TEST(test_is_function)
    {
      (void)f(0);
      (void)fvar();
      (void)fvar2(0);
      (void)ft<int, int, double>(0, 0.0);

      CHECK_TRUE((etl::is_function<decltype(f)>::value));
      CHECK_TRUE((etl::is_function<decltype(fvar)>::value));
      CHECK_TRUE((etl::is_function<decltype(fvar2)>::value));
      CHECK_TRUE((etl::is_function<decltype(ft<int, int, double>)>::value));

#if ETL_HAS_NOEXCEPT_FUNCTION_TYPE
      (void)fn(0);
      (void)fnvar();
      (void)fnvar2(0);
      (void)fnt<int, int, double>(0, 0.0);

      CHECK_TRUE((etl::is_function<decltype(fn)>::value));
      CHECK_TRUE((etl::is_function<decltype(fnvar)>::value));
      CHECK_TRUE((etl::is_function<decltype(fnvar2)>::value));
      CHECK_TRUE((etl::is_function<decltype(fnt<int, int, double>)>::value));
#endif

      CHECK_FALSE((etl::is_function<int>::value));
      CHECK_FALSE((etl::is_function<int*>::value));
      CHECK_FALSE((etl::is_function<int MF::*>::value));
      CHECK_FALSE((etl::is_function<int (MF::*)(int)>::value)); // pointer, not function
    }

    //*************************************************************************
    TEST(test_is_object)
    {
      CHECK_TRUE((etl::is_object<int>::value));
      CHECK_TRUE((etl::is_object<int*>::value));
      CHECK_TRUE((etl::is_object<int[]>::value));
      CHECK_TRUE((etl::is_object<int[3]>::value));
      CHECK_TRUE((etl::is_object<const int>::value));
      CHECK_TRUE((etl::is_object<volatile int>::value));
      CHECK_TRUE((etl::is_object<MF>::value));

      CHECK_FALSE((etl::is_object<void>::value));
      CHECK_FALSE((etl::is_object<const void>::value));
      CHECK_FALSE((etl::is_object<volatile void>::value));
      CHECK_FALSE((etl::is_object<const volatile void>::value));
      CHECK_FALSE((etl::is_object<int&>::value));
      CHECK_FALSE((etl::is_object<int&&>::value));
      CHECK_FALSE((etl::is_object<decltype(f)>::value));

      CHECK_TRUE((etl::is_void<const void>::value));
      CHECK_TRUE((etl::is_void<volatile void>::value));
      CHECK_TRUE((etl::is_void<const volatile void>::value));

#if ETL_USING_CPP17
      CHECK_TRUE((etl::is_object_v<int>));
      CHECK_TRUE((etl::is_object_v<int*>));
      CHECK_TRUE((etl::is_object_v<MF>));

      CHECK_FALSE((etl::is_object_v<void>));
      CHECK_FALSE((etl::is_object_v<const void>));
      CHECK_FALSE((etl::is_object_v<volatile void>));
      CHECK_FALSE((etl::is_object_v<const volatile void>));
      CHECK_FALSE((etl::is_object_v<int&>));
      CHECK_FALSE((etl::is_object_v<int&&>));
      CHECK_FALSE((etl::is_object_v<decltype(f)>));
#endif
    }

    //*************************************************************************
    TEST(test_is_signed_unsigned_char_types)
    {
#if ETL_HAS_NATIVE_CHAR8_T
      CHECK_FALSE(etl::is_signed<char8_t>::value);
      CHECK_TRUE(etl::is_unsigned<char8_t>::value);
#endif

#if ETL_HAS_NATIVE_CHAR16_T
      CHECK_FALSE(etl::is_signed<char16_t>::value);
      CHECK_TRUE(etl::is_unsigned<char16_t>::value);
#endif

#if ETL_HAS_NATIVE_CHAR32_T
      CHECK_FALSE(etl::is_signed<char32_t>::value);
      CHECK_TRUE(etl::is_unsigned<char32_t>::value);
#endif
    }

#if ETL_USING_CPP11
    //*************************************************************************
    TEST(test_common_type)
    {
      // Reference types must decay to the same result as std::common_type.
      CHECK((etl::is_same<etl::common_type_t<long&, int&>, long>::value));
      CHECK((etl::is_same<etl::common_type_t<long&, int&>, std::common_type<long&, int&>::type>::value));
      CHECK((etl::is_same<etl::common_type_t<int&, long&>, long>::value));
      CHECK((etl::is_same<etl::common_type_t<int&, long&>, std::common_type<int&, long&>::type>::value));
      CHECK((etl::is_same<etl::common_type_t<const char&, char&>, char>::value));
      CHECK((etl::is_same<etl::common_type_t<const char&, char&>, std::common_type<const char&, char&>::type>::value));

      // Already decayed types.
      CHECK((etl::is_same<etl::common_type_t<int, long, int>, long>::value));
      CHECK((etl::is_same<etl::common_type_t<int, long, int>, std::common_type<int, long, int>::type>::value));
    }
#endif
  }
} // namespace
