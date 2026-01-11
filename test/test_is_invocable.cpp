/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl
https://www.etlcpp.com

Copyright(c) 2025 John Wellbelove

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

#include <type_traits>
#include <string>

#include "etl/type_traits.h"
#include "etl/invoke.h"
#include "etl/type_list.h"

namespace
{
  //*************************************************************************
  // Callable subjects
  int   free_add(int a, int b) { return a + b; }
  short free_short(int a) { return static_cast<short>(a); }
  int   free_noexcept(int v) noexcept { return v; }
  int   free_throw(int v) { throw v; return v; }

  // C-style variadic function
  int varfn(int first, ...)
  { 
    return first; 
  }

  //*********************************************
  struct Base
  {
    int data;
    Base(int d = 10) : data(d) {}

    int add(int v) { return data + v; }
    int add_const(int v) const { return data + v + 1; }

    int ref_only(int v)& { return data + v + 2; }
    int rref_only(int v)&& { return data + v + 3; }

    static int static_function(int v) { return v * 2; }
    int member_noexcept(int v) noexcept { return data + v; }
    int member_throw(int v) { throw v; return data + v; }
  };

  //*********************************************
  struct NotBase
  {
    int data;
    NotBase(int d = 0) : data(d) {}
  };

  //*********************************************
  struct Derived : Base
  {
    Derived(int d = 20) : Base(d) {}
  };

  //*********************************************
  struct NotDerived : NotBase
  {
    NotDerived(int d = 20) : NotBase(d) {}
  };

  //*********************************************
  struct VolatileBase
  {
    int x;
    VolatileBase(int v = 0) : x(v) {}
    int read() const volatile { return x; }
  };

  //*********************************************
  struct Functor
  {
    int factor;
    explicit Functor(int f) : factor(f) {}
    int operator()(int x) { return x * factor; }
  };

  //*********************************************
  struct ConstFunctor
  {
    int bias;
    explicit ConstFunctor(int b) : bias(b) {}
    int operator()(int x) const { return x + bias; }
  };

  //*********************************************
  struct OverloadedFunctor
  {
    int operator()(int)& { return 1; }
    int operator()(int)&& { return 2; }
  };

  //*********************************************
  struct MoveOnlyFunctor
  {
    MoveOnlyFunctor() = default;
    MoveOnlyFunctor(const MoveOnlyFunctor&) = delete;
    MoveOnlyFunctor(MoveOnlyFunctor&&) = default;
    int operator()(int x) { return x + 100; }
  };

  //*********************************************
  struct NothrowFunctor
  {
    int operator()() const noexcept { return 7; }
  };

  //*********************************************
  struct ThrowingFunctor
  {
    int operator()() { throw 42; }
  };

  //*********************************************
  struct MemberObj
  {
    int i;
  };

  //*********************************************
  template <typename T>
  struct IntegralOnly
  {
    template <typename U, typename = typename etl::enable_if<etl::is_integral<U>::value>::type>
    int operator()(U u) { return static_cast<int>(u) + 10; }

    template <typename U, typename = typename etl::enable_if<!etl::is_integral<U>::value>::type, typename = void>
    int operator()(U) = delete;
  };

  //*********************************************
  // Non-capturing lambda (convertible to function pointer)
  static auto lambda_nc = [](int a, int b) { return a + b; };

#if ETL_USING_CPP14
  //*********************************************
  // Generic lambda (C++14+)
  static auto lambda_generic = [](auto x, auto y) { return x + y; };
#endif

  //*********************************************
  // Overload set (not directly a single callable type for traits)
  int  overload(int)  { return 0; }
  long overload(long) { return 0; }

  int takes_ptr(const int* p) { return *p; }

  //*********************************************
  struct Selective
  {
    template <typename T, etl::enable_if_t<etl::is_same<int, T>::value, int> = 0>
    int operator()(T) { return 1; }

    template <typename T, etl::enable_if_t<etl::is_same<char, T>::value, int> = 0>
    char operator()(T) { return 1; }

    template <typename T, etl::enable_if_t<!etl::is_integral<T>::value, int> = 0>
    int operator()(T) = delete;
  };
}

//*************************************************************************
// Unit tests for etl::is_invocable / etl::is_invocable_r
//*************************************************************************
SUITE(test_is_invocable)
{
  //*************************************************************************
  TEST(use_free_functions)
  {
    // Ensure that clang does not discard them as unused.
    CHECK_TRUE(free_add(1, 2) != 0);
    CHECK_TRUE(free_short(1) != 0);
    CHECK_TRUE(free_noexcept(1) != 0);
    CHECK_THROW(free_throw(1), int);
    CHECK_TRUE(lambda_nc(1, 2) != 0);
#if ETL_USING_CPP14
    CHECK_TRUE(lambda_generic(1, 2) != 0);
#endif
    CHECK_TRUE(varfn(1, 2, 3) != 0);

    int value = 10;
    CHECK_EQUAL(10, takes_ptr(&value));
  }

  //*************************************************************************
  TEST(test_free_function)
  {
    CHECK_TRUE((etl::is_invocable<decltype(free_add), int, int>::value));
    CHECK_FALSE((etl::is_invocable<decltype(free_add), int>::value));                      // Arity mismatch   
    CHECK_FALSE((etl::is_invocable<decltype(free_add), std::string, std::string>::value)); // Non-convertible argument

    // Use etl::type_list
    CHECK_TRUE((etl::is_invocable<decltype(free_add), etl::type_list<int, int>>::value));
    CHECK_FALSE((etl::is_invocable<decltype(free_add), etl::type_list<int>>::value));                      // Arity mismatch   
    CHECK_FALSE((etl::is_invocable<decltype(free_add), etl::type_list<std::string, std::string>>::value)); // Non-convertible argument
  }

  //*************************************************************************
  TEST(test_free_function_return)
  {
    CHECK_TRUE((etl::is_invocable_r<int, decltype(free_add), int, int>::value));
    CHECK_FALSE((etl::is_invocable_r<std::string, decltype(free_add), int, int>::value)); // Return mismatch
    CHECK_FALSE((etl::is_invocable_r<int, decltype(free_add), int>::value));              // Arity mismatch   
    CHECK_FALSE((etl::is_invocable_r<int, decltype(free_add), std::string, int>::value)); // Non-convertible argument

    // Use etl::type_list
    CHECK_TRUE((etl::is_invocable_r<int, decltype(free_add), etl::type_list<int, int>>::value));
    CHECK_FALSE((etl::is_invocable_r<std::string, decltype(free_add), etl::type_list<int, int>>::value)); // Return mismatch
    CHECK_FALSE((etl::is_invocable_r<int, decltype(free_add), etl::type_list<int>>::value));              // Arity mismatch   
    CHECK_FALSE((etl::is_invocable_r<int, decltype(free_add), etl::type_list<std::string, int>>::value)); // Non-convertible argument
  }

  //*************************************************************************
  TEST(test_free_function_pointer_variable)
  {
    int (*fp)(int, int) = &free_add;

    CHECK_TRUE((etl::is_invocable<decltype(fp), int, int>::value));
    CHECK_FALSE((etl::is_invocable<decltype(fp), int>::value));                     // Arity mismatch
    CHECK_FALSE((etl::is_invocable<decltype(fp), std::string>::value));             // Non-convertible argument

    // Use etl::type_list
    CHECK_TRUE((etl::is_invocable<decltype(fp), etl::type_list<int, int>>::value));
    CHECK_FALSE((etl::is_invocable<decltype(fp), etl::type_list<int>>::value));                     // Arity mismatch
    CHECK_FALSE((etl::is_invocable<decltype(fp), etl::type_list<std::string>>::value));             // Non-convertible argument
  }

  //*************************************************************************
  TEST(test_free_function_pointer_variable_return)
  {
    int (*fp)(int, int) = &free_add;

    CHECK_TRUE((etl::is_invocable_r<int,          decltype(fp), int, int>::value));
    CHECK_FALSE((etl::is_invocable_r<std::string, decltype(fp), int, int>::value));         // Return mismatch
    CHECK_FALSE((etl::is_invocable_r<int,         decltype(fp), int>::value));              // Arity mismatch
    CHECK_FALSE((etl::is_invocable_r<int,         decltype(fp), std::string, int>::value)); // Non-convertible argument

    // Use etl::type_list
    CHECK_TRUE((etl::is_invocable_r<int,          decltype(fp), etl::type_list<int, int>>::value));
    CHECK_FALSE((etl::is_invocable_r<std::string, decltype(fp), etl::type_list<int, int>>::value));         // Return mismatch
    CHECK_FALSE((etl::is_invocable_r<int,         decltype(fp), etl::type_list<int>>::value));              // Arity mismatch
    CHECK_FALSE((etl::is_invocable_r<int,         decltype(fp), etl::type_list<std::string, int>>::value)); // Non-convertible argument
  }

  //*************************************************************************
  TEST(test_function_pointer_const_qualification)
  {
    int (*const cfp)(int, int) = &free_add;

    CHECK_TRUE((etl::is_invocable<decltype(cfp), int, int>::value));
    CHECK_TRUE((etl::is_invocable_r<int, decltype(cfp), int, int>::value));

    // Use etl::type_list
    CHECK_TRUE((etl::is_invocable<decltype(cfp), etl::type_list<int, int>>::value));
    CHECK_TRUE((etl::is_invocable_r<int, decltype(cfp), etl::type_list<int, int>>::value));
  }

  //*************************************************************************
  TEST(test_free_function_noexcept)
  {
    CHECK_TRUE((etl::is_invocable<decltype(free_noexcept), int>::value));
    CHECK_FALSE((etl::is_invocable<decltype(free_noexcept), int, int>::value));                // Arity mismatch
    CHECK_FALSE((etl::is_invocable<decltype(free_noexcept), std::string>::value));             // Non-convertible argument

    // Use etl::type_list
    CHECK_TRUE((etl::is_invocable<decltype(free_noexcept), etl::type_list<int>>::value));
    CHECK_FALSE((etl::is_invocable<decltype(free_noexcept), etl::type_list<int, int>>::value));                // Arity mismatch
    CHECK_FALSE((etl::is_invocable<decltype(free_noexcept), etl::type_list<std::string>>::value));             // Non-convertible argument
  }

  //*************************************************************************
  TEST(test_free_function_noexcept_return)
  {
    CHECK_TRUE((etl::is_invocable_r<int, decltype(free_noexcept), int>::value));
    CHECK_FALSE((etl::is_invocable_r<std::string, decltype(free_noexcept), int>::value));      // Return mismatch
    CHECK_FALSE((etl::is_invocable<int, decltype(free_noexcept), int, int>::value));           // Arity mismatch
    CHECK_FALSE((etl::is_invocable_r<int, decltype(free_noexcept), std::string, int>::value)); // Non-convertible argument
    
    // Use etl::type_list
    CHECK_TRUE((etl::is_invocable_r<int, decltype(free_noexcept), etl::type_list<int>>::value));
    CHECK_FALSE((etl::is_invocable_r<std::string, decltype(free_noexcept), etl::type_list<int>>::value));      // Return mismatch
    CHECK_FALSE((etl::is_invocable<int, decltype(free_noexcept), etl::type_list<int, int>>::value));           // Arity mismatch
    CHECK_FALSE((etl::is_invocable_r<int, decltype(free_noexcept), std::string, etl::type_list<int>>::value)); // Non-convertible argument
  }

  //*************************************************************************
  TEST(test_free_function_throw)
  {
    CHECK_TRUE((etl::is_invocable<decltype(free_throw), int>::value));
    CHECK_FALSE((etl::is_invocable<decltype(free_throw), int, int>::value));                // Arity mismatch
    CHECK_FALSE((etl::is_invocable<decltype(free_throw), std::string>::value));             // Non-convertible argument

    // Use etl::type_list
    CHECK_TRUE((etl::is_invocable<decltype(free_throw), etl::type_list<int>>::value));
    CHECK_FALSE((etl::is_invocable<decltype(free_throw), etl::type_list<int, int>>::value));                // Arity mismatch
    CHECK_FALSE((etl::is_invocable<decltype(free_throw), etl::type_list<std::string>>::value));             // Non-convertible argument
  }

  //*************************************************************************
  TEST(test_free_function_throw_return)
  {
    CHECK_TRUE((etl::is_invocable_r<int, decltype(free_throw), int>::value));
    CHECK_FALSE((etl::is_invocable_r<std::string, decltype(free_throw), int>::value));      // Return mismatch
    CHECK_FALSE((etl::is_invocable_r<int, decltype(free_throw), int, int>::value));         // Arity mismatch
    CHECK_FALSE((etl::is_invocable_r<int, decltype(free_throw), std::string, int>::value)); // Non-convertible argument

    // Use etl::type_list
    CHECK_TRUE((etl::is_invocable_r<int, decltype(free_throw), etl::type_list<int>>::value));
    CHECK_FALSE((etl::is_invocable_r<std::string, decltype(free_throw), etl::type_list<int>>::value));      // Return mismatch
    CHECK_FALSE((etl::is_invocable_r<int, decltype(free_throw), etl::type_list<int, int>>::value));         // Arity mismatch
    CHECK_FALSE((etl::is_invocable_r<int, decltype(free_throw), etl::type_list<std::string, int>>::value)); // Non-convertible argument
  }

#if ETL_USING_CPP17
  //*************************************************************************
  TEST(test_no_throw_invocable)
  {
    CHECK_TRUE((etl::is_nothrow_invocable<decltype(free_noexcept), int>::value));
    CHECK_FALSE((etl::is_nothrow_invocable<decltype(free_noexcept), int, int>::value));    // Arity mismatch
    CHECK_FALSE((etl::is_nothrow_invocable<decltype(free_noexcept), std::string>::value)); // Non-convertible argument

    CHECK_FALSE((etl::is_nothrow_invocable<decltype(free_throw), int>::value));
    CHECK_FALSE((etl::is_nothrow_invocable<decltype(free_throw), int, int>::value));       // Arity mismatch
    CHECK_FALSE((etl::is_nothrow_invocable<decltype(free_throw), std::string>::value));    // Non-convertible argument

    CHECK_TRUE((etl::is_nothrow_invocable<decltype(&Base::member_noexcept), Base&, int>::value));
    CHECK_FALSE((etl::is_nothrow_invocable<decltype(&Base::member_throw), Base&, int>::value));

    // Use etl::type_list
    CHECK_TRUE((etl::is_nothrow_invocable<decltype(free_noexcept), etl::type_list<int>>::value));
    CHECK_FALSE((etl::is_nothrow_invocable<decltype(free_noexcept), etl::type_list<int, int>>::value));    // Arity mismatch
    CHECK_FALSE((etl::is_nothrow_invocable<decltype(free_noexcept), etl::type_list<std::string>>::value)); // Non-convertible argument

    CHECK_FALSE((etl::is_nothrow_invocable<decltype(free_throw), etl::type_list<int>>::value));
    CHECK_FALSE((etl::is_nothrow_invocable<decltype(free_throw), etl::type_list<int, int>>::value));       // Arity mismatch
    CHECK_FALSE((etl::is_nothrow_invocable<decltype(free_throw), etl::type_list<std::string>>::value));    // Non-convertible argument

    CHECK_TRUE((etl::is_nothrow_invocable<decltype(&Base::member_noexcept), Base&, etl::type_list<int>>::value));
    CHECK_FALSE((etl::is_nothrow_invocable<decltype(&Base::member_throw), Base&, etl::type_list<int>>::value));
  }

  //*************************************************************************
  TEST(test_no_throw_invocable_return)
  {
    CHECK_TRUE((etl::is_nothrow_invocable_r<int, decltype(free_noexcept), int>::value));
    CHECK_FALSE((etl::is_nothrow_invocable_r<std::string, decltype(free_noexcept), int>::value));      // Return mismatch
    CHECK_FALSE((etl::is_nothrow_invocable_r<int, decltype(free_noexcept), int, int>::value));         // Arity mismatch
    CHECK_FALSE((etl::is_nothrow_invocable_r<int, decltype(free_noexcept), std::string, int>::value)); // Non-convertible argument

    CHECK_FALSE((etl::is_nothrow_invocable_r<int, decltype(free_throw), int>::value));
    CHECK_FALSE((etl::is_nothrow_invocable_r<std::string, decltype(free_throw), int>::value));         // Return mismatch
    CHECK_FALSE((etl::is_nothrow_invocable_r<int, decltype(free_throw), int, int>::value));            // Arity mismatch
    CHECK_FALSE((etl::is_nothrow_invocable_r<int, decltype(free_throw), std::string, int>::value));    // Non-convertible argument

    CHECK_TRUE((etl::is_nothrow_invocable_r<int, decltype(&Base::member_noexcept), Base&, int>::value));
    CHECK_FALSE((etl::is_nothrow_invocable_r<int, decltype(&Base::member_throw), Base&, int>::value));

    // Use etl::type_list
    CHECK_TRUE((etl::is_nothrow_invocable_r<int, decltype(free_noexcept), etl::type_list<int>>::value));
    CHECK_FALSE((etl::is_nothrow_invocable_r<std::string, decltype(free_noexcept), etl::type_list<int>>::value));      // Return mismatch
    CHECK_FALSE((etl::is_nothrow_invocable_r<int, decltype(free_noexcept), etl::type_list<int, int>>::value));         // Arity mismatch
    CHECK_FALSE((etl::is_nothrow_invocable_r<int, decltype(free_noexcept), etl::type_list<std::string, int>>::value)); // Non-convertible argument

    CHECK_FALSE((etl::is_nothrow_invocable_r<int, decltype(free_throw), etl::type_list<int>>::value));
    CHECK_FALSE((etl::is_nothrow_invocable_r<std::string, decltype(free_throw), etl::type_list<int>>::value));         // Return mismatch
    CHECK_FALSE((etl::is_nothrow_invocable_r<int, decltype(free_throw), etl::type_list<int, int>>::value));            // Arity mismatch
    CHECK_FALSE((etl::is_nothrow_invocable_r<int, decltype(free_throw), etl::type_list<std::string, int>>::value));    // Non-convertible argument

    CHECK_TRUE((etl::is_nothrow_invocable_r<int, decltype(&Base::member_noexcept), Base&, etl::type_list<int>>::value));
    CHECK_FALSE((etl::is_nothrow_invocable_r<int, decltype(&Base::member_throw), Base&, etl::type_list<int>>::value));
  }
#endif

  //*************************************************************************
  TEST(test_static_member_function)
  {
    CHECK_TRUE((etl::is_invocable<decltype(&Base::static_function), int>::value));
    CHECK_FALSE((etl::is_invocable<decltype(&Base::static_function), int, int>::value));           // Arity mismatch
    CHECK_FALSE((etl::is_invocable<decltype(&Base::static_function), std::string>::value));        // Non-convertible argument

    // Use etl::type_list
    CHECK_TRUE((etl::is_invocable<decltype(&Base::static_function), etl::type_list<int>>::value));
    CHECK_FALSE((etl::is_invocable<decltype(&Base::static_function), etl::type_list<int, int>>::value));           // Arity mismatch
    CHECK_FALSE((etl::is_invocable<decltype(&Base::static_function), etl::type_list<std::string>>::value));        // Non-convertible argument
  }

  //*************************************************************************
  TEST(test_static_member_function_return)
  {
    CHECK_TRUE((etl::is_invocable_r<int, decltype(&Base::static_function), int>::value));
    CHECK_FALSE((etl::is_invocable_r<std::string, decltype(&Base::static_function), int>::value)); // Return mismatch
    CHECK_FALSE((etl::is_invocable_r<int, decltype(&Base::static_function), int, int>::value));    // Arity mismatch
    CHECK_FALSE((etl::is_invocable_r<int, decltype(&Base::static_function), std::string>::value)); // Non-convertible argument

    // Use etl::type_list
    CHECK_TRUE((etl::is_invocable_r<int, decltype(&Base::static_function), etl::type_list<int>>::value));
    CHECK_FALSE((etl::is_invocable_r<std::string, decltype(&Base::static_function), etl::type_list<int>>::value)); // Return mismatch
    CHECK_FALSE((etl::is_invocable_r<int, decltype(&Base::static_function), etl::type_list<int, int>>::value));    // Arity mismatch
    CHECK_FALSE((etl::is_invocable_r<int, decltype(&Base::static_function), etl::type_list<std::string>>::value)); // Non-convertible argument
  }

  //*************************************************************************
  TEST(test_member_function)
  {
    CHECK_TRUE((etl::is_invocable<decltype(&Base::add),  Base&,    int>::value));
    CHECK_FALSE((etl::is_invocable<decltype(&Base::add), Base&,    int, int>::value));    // Arity mismatch
    CHECK_FALSE((etl::is_invocable<decltype(&Base::add), NotBase&, int>::value));         // Wrong object type
    CHECK_FALSE((etl::is_invocable<decltype(&Base::add), NotBase&, std::string>::value)); // Non-convertible argument
    
    CHECK_TRUE((etl::is_invocable<decltype(&Base::add),  Base*,    int>::value));
    CHECK_FALSE((etl::is_invocable<decltype(&Base::add), Base*,    int, int>::value));    // Arity mismatch
    CHECK_FALSE((etl::is_invocable<decltype(&Base::add), NotBase*, int>::value));         // Wrong object type
    CHECK_FALSE((etl::is_invocable<decltype(&Base::add), NotBase*, std::string>::value)); // Non-convertible argument

    CHECK_TRUE((etl::is_invocable<decltype(&Base::add_const),  const Base&,    int>::value));
    CHECK_FALSE((etl::is_invocable<decltype(&Base::add_const), const Base&,    int, int>::value));    // Arity mismatch
    CHECK_FALSE((etl::is_invocable<decltype(&Base::add_const), const NotBase&, int>::value));         // Wrong object type
    CHECK_FALSE((etl::is_invocable<decltype(&Base::add_const), const Base&,    std::string>::value)); // Non-convertible argument
    CHECK_FALSE((etl::is_invocable<decltype(&Base::add),       const Base&,    int>::value));         // Not const function

    CHECK_TRUE((etl::is_invocable<decltype(&Base::add_const),  const Base*,    int>::value));
    CHECK_FALSE((etl::is_invocable<decltype(&Base::add_const), const Base*,    int, int>::value));    // Arity mismatch
    CHECK_FALSE((etl::is_invocable<decltype(&Base::add_const), const NotBase*, int>::value));         // Wrong object type
    CHECK_FALSE((etl::is_invocable<decltype(&Base::add_const), const Base*,    std::string>::value)); // Non-convertible argument
    CHECK_FALSE((etl::is_invocable<decltype(&Base::add),       const Base*,    int>::value));         // Not const function

    // Use etl::type_list
    CHECK_TRUE((etl::is_invocable<decltype(&Base::add),  Base&,    etl::type_list<int>>::value));
    CHECK_FALSE((etl::is_invocable<decltype(&Base::add), Base&,    etl::type_list<int, int>>::value));    // Arity mismatch
    CHECK_FALSE((etl::is_invocable<decltype(&Base::add), NotBase&, etl::type_list<int>>::value));         // Wrong object type
    CHECK_FALSE((etl::is_invocable<decltype(&Base::add), NotBase&, etl::type_list<std::string>>::value)); // Non-convertible argument

    CHECK_TRUE((etl::is_invocable<decltype(&Base::add),  Base*,    etl::type_list<int>>::value));
    CHECK_FALSE((etl::is_invocable<decltype(&Base::add), Base*,    etl::type_list<int, int>>::value));    // Arity mismatch
    CHECK_FALSE((etl::is_invocable<decltype(&Base::add), NotBase*, etl::type_list<int>>::value));         // Wrong object type
    CHECK_FALSE((etl::is_invocable<decltype(&Base::add), NotBase*, etl::type_list<std::string>>::value)); // Non-convertible argument

    CHECK_TRUE((etl::is_invocable<decltype(&Base::add_const),  const Base&,    etl::type_list<int>>::value));
    CHECK_FALSE((etl::is_invocable<decltype(&Base::add_const), const Base&,    etl::type_list<int, int>>::value));    // Arity mismatch
    CHECK_FALSE((etl::is_invocable<decltype(&Base::add_const), const NotBase&, etl::type_list<int>>::value));         // Wrong object type
    CHECK_FALSE((etl::is_invocable<decltype(&Base::add_const), const Base&,    etl::type_list<std::string>>::value)); // Non-convertible argument
    CHECK_FALSE((etl::is_invocable<decltype(&Base::add),       const Base&,    etl::type_list<int>>::value));         // Not const function

    CHECK_TRUE((etl::is_invocable<decltype(&Base::add_const),  const Base*,    etl::type_list<int>>::value));
    CHECK_FALSE((etl::is_invocable<decltype(&Base::add_const), const Base*,    etl::type_list<int, int>>::value));    // Arity mismatch
    CHECK_FALSE((etl::is_invocable<decltype(&Base::add_const), const NotBase*, etl::type_list<int>>::value));         // Wrong object type
    CHECK_FALSE((etl::is_invocable<decltype(&Base::add_const), const Base*,    etl::type_list<std::string>>::value)); // Non-convertible argument
    CHECK_FALSE((etl::is_invocable<decltype(&Base::add),       const Base*,    etl::type_list<int>>::value));         // Not const function
  }

  //*************************************************************************
  TEST(test_member_function_return)
  {
    CHECK_TRUE((etl::is_invocable_r<int,          decltype(&Base::add), Base&,    int>::value));
    CHECK_FALSE((etl::is_invocable_r<std::string, decltype(&Base::add), Base&,    int>::value));         // Return mismatch
    CHECK_FALSE((etl::is_invocable_r<int,         decltype(&Base::add), Base&,    int, int>::value));    // Arity mismatch
    CHECK_FALSE((etl::is_invocable_r<int,         decltype(&Base::add), NotBase&, int>::value));         // Wrong object type
    CHECK_FALSE((etl::is_invocable_r<int,         decltype(&Base::add), NotBase&, std::string>::value)); // Non-convertible argument

    CHECK_TRUE((etl::is_invocable_r<int,          decltype(&Base::add), Base*,    int>::value));
    CHECK_FALSE((etl::is_invocable_r<std::string, decltype(&Base::add), Base*,    int>::value));         // Return mismatch
    CHECK_FALSE((etl::is_invocable_r<int,         decltype(&Base::add), Base*,    int, int>::value));    // Arity mismatch
    CHECK_FALSE((etl::is_invocable_r<int,         decltype(&Base::add), NotBase*, int>::value));         // Wrong object type
    CHECK_FALSE((etl::is_invocable_r<int,         decltype(&Base::add), NotBase*, std::string>::value)); // Non-convertible argument

    CHECK_TRUE((etl::is_invocable_r<int,          decltype(&Base::add_const), const Base&,    int>::value));
    CHECK_FALSE((etl::is_invocable_r<std::string, decltype(&Base::add_const), const Base&,    int>::value));         // Return mismatch
    CHECK_FALSE((etl::is_invocable_r<int,         decltype(&Base::add_const), const Base&,    int, int>::value));    // Arity mismatch
    CHECK_FALSE((etl::is_invocable_r<int,         decltype(&Base::add_const), const NotBase&, int>::value));         // Wrong object type
    CHECK_FALSE((etl::is_invocable_r<int,         decltype(&Base::add_const), const Base&,    std::string>::value)); // Non-convertible argument
    CHECK_FALSE((etl::is_invocable_r<int,         decltype(&Base::add),       const Base&,    int>::value));         // Not const function

    CHECK_TRUE((etl::is_invocable_r<int,          decltype(&Base::add_const), const Base*,    int>::value));
    CHECK_FALSE((etl::is_invocable_r<std::string, decltype(&Base::add_const), const Base*,    int>::value));         // Return mismatch
    CHECK_FALSE((etl::is_invocable_r<int,         decltype(&Base::add_const), const Base*,    int, int>::value));    // Arity mismatch
    CHECK_FALSE((etl::is_invocable_r<int,         decltype(&Base::add_const), const NotBase*, int>::value));         // Wrong object type
    CHECK_FALSE((etl::is_invocable_r<int,         decltype(&Base::add_const), const Base*,    std::string>::value)); // Non-convertible argument
    CHECK_FALSE((etl::is_invocable_r<int,         decltype(&Base::add),       const Base*,    int>::value));         // Not const function

    // Use etl::type_list
    CHECK_TRUE((etl::is_invocable_r<int,          decltype(&Base::add), Base&,    etl::type_list<int>>::value));
    CHECK_FALSE((etl::is_invocable_r<std::string, decltype(&Base::add), Base&,    etl::type_list<int>>::value));         // Return mismatch
    CHECK_FALSE((etl::is_invocable_r<int,         decltype(&Base::add), Base&,    etl::type_list<int, int>>::value));    // Arity mismatch
    CHECK_FALSE((etl::is_invocable_r<int,         decltype(&Base::add), NotBase&, etl::type_list<int>>::value));         // Wrong object type
    CHECK_FALSE((etl::is_invocable_r<int,         decltype(&Base::add), NotBase&, etl::type_list<std::string>>::value)); // Non-convertible argument

    CHECK_TRUE((etl::is_invocable_r<int,          decltype(&Base::add), Base*,    etl::type_list<int>>::value));
    CHECK_FALSE((etl::is_invocable_r<std::string, decltype(&Base::add), Base*,    etl::type_list<int>>::value));         // Return mismatch
    CHECK_FALSE((etl::is_invocable_r<int,         decltype(&Base::add), Base*,    etl::type_list<int, int>>::value));    // Arity mismatch
    CHECK_FALSE((etl::is_invocable_r<int,         decltype(&Base::add), NotBase*, etl::type_list<int>>::value));         // Wrong object type
    CHECK_FALSE((etl::is_invocable_r<int,         decltype(&Base::add), NotBase*, etl::type_list<std::string>>::value)); // Non-convertible argument

    CHECK_TRUE((etl::is_invocable_r<int,          decltype(&Base::add_const), const Base&,    etl::type_list<int>>::value));
    CHECK_FALSE((etl::is_invocable_r<std::string, decltype(&Base::add_const), const Base&,    etl::type_list<int>>::value));         // Return mismatch
    CHECK_FALSE((etl::is_invocable_r<int,         decltype(&Base::add_const), const Base&,    etl::type_list<int, int>>::value));    // Arity mismatch
    CHECK_FALSE((etl::is_invocable_r<int,         decltype(&Base::add_const), const NotBase&, etl::type_list<int>>::value));         // Wrong object type
    CHECK_FALSE((etl::is_invocable_r<int,         decltype(&Base::add_const), const Base&,    etl::type_list<std::string>>::value)); // Non-convertible argument
    CHECK_FALSE((etl::is_invocable_r<int,         decltype(&Base::add),       const Base&,    etl::type_list<int>>::value));         // Not const function

    CHECK_TRUE((etl::is_invocable_r<int,          decltype(&Base::add_const), const Base*,    etl::type_list<int>>::value));
    CHECK_FALSE((etl::is_invocable_r<std::string, decltype(&Base::add_const), const Base*,    etl::type_list<int>>::value));         // Return mismatch
    CHECK_FALSE((etl::is_invocable_r<int,         decltype(&Base::add_const), const Base*,    etl::type_list<int, int>>::value));    // Arity mismatch
    CHECK_FALSE((etl::is_invocable_r<int,         decltype(&Base::add_const), const NotBase*, etl::type_list<int>>::value));         // Wrong object type
    CHECK_FALSE((etl::is_invocable_r<int,         decltype(&Base::add_const), const Base*,    etl::type_list<std::string>>::value)); // Non-convertible argument
    CHECK_FALSE((etl::is_invocable_r<int,         decltype(&Base::add),       const Base*,    etl::type_list<int>>::value));         // Not const function
  }

  //*************************************************************************
  TEST(test_member_function_noexcept)
  {
    CHECK_TRUE((etl::is_invocable<decltype(&Base::member_noexcept), Base&, int>::value));
    CHECK_TRUE((etl::is_invocable_r<int, decltype(&Base::member_noexcept), Base&, int>::value));

    // Use etl::type_list
    CHECK_TRUE((etl::is_invocable<decltype(&Base::member_noexcept), Base&, etl::type_list<int>>::value));
    CHECK_TRUE((etl::is_invocable_r<int, decltype(&Base::member_noexcept), Base&, etl::type_list<int>>::value));
  }

  //*************************************************************************
  TEST(test_member_functions_ref_qualification)
  {
    CHECK_TRUE((etl::is_invocable<decltype(&Base::ref_only), Base&, int>::value));
    CHECK_FALSE((etl::is_invocable<decltype(&Base::ref_only), Base&&, int>::value)); // rvalue object disallowed

    CHECK_TRUE((etl::is_invocable<decltype(&Base::rref_only), Base&&, int>::value));
    CHECK_FALSE((etl::is_invocable<decltype(&Base::rref_only), Base&, int>::value)); // lvalue object disallowed

    // Use etl::type_list
    CHECK_TRUE((etl::is_invocable<decltype(&Base::ref_only), Base&, etl::type_list<int>>::value));
    CHECK_FALSE((etl::is_invocable<decltype(&Base::ref_only), Base&&, etl::type_list<int>>::value)); // rvalue object disallowed

    CHECK_TRUE((etl::is_invocable<decltype(&Base::rref_only), Base&&, etl::type_list<int>>::value));
    CHECK_FALSE((etl::is_invocable<decltype(&Base::rref_only), Base&, etl::type_list<int>>::value)); // lvalue object disallowed
  }

  //************************************************************************* 
  TEST(test_inheritance_member_function)
  {
    CHECK_TRUE((etl::is_invocable<decltype(&Base::add), Derived&, int>::value));
    CHECK_TRUE((etl::is_invocable<decltype(&Base::add), Derived*, int>::value));

    CHECK_FALSE((etl::is_invocable<decltype(&Base::add), NotDerived&, int>::value));
    CHECK_FALSE((etl::is_invocable<decltype(&Base::add), NotDerived*, int>::value));

    // Use etl::type_list
    CHECK_TRUE((etl::is_invocable<decltype(&Base::add), Derived&, etl::type_list<int>>::value));
    CHECK_TRUE((etl::is_invocable<decltype(&Base::add), Derived*, etl::type_list<int>>::value));

    CHECK_FALSE((etl::is_invocable<decltype(&Base::add), NotDerived&, etl::type_list<int>>::value));
    CHECK_FALSE((etl::is_invocable<decltype(&Base::add), NotDerived*, etl::type_list<int>>::value));
  }

  //*************************************************************************
  TEST(test_member_function_volatile)
  {
    CHECK_TRUE((etl::is_invocable<decltype(&VolatileBase::read), const volatile VolatileBase&>::value));
    CHECK_FALSE((etl::is_invocable<decltype(&VolatileBase::read), Base&>::value));

    CHECK_TRUE((etl::is_invocable_r<int, decltype(&VolatileBase::read), volatile VolatileBase&>::value));

    // Use etl::type_list
    CHECK_TRUE((etl::is_invocable<decltype(&VolatileBase::read), const volatile VolatileBase&, etl::type_list<>>::value));
    CHECK_FALSE((etl::is_invocable<decltype(&VolatileBase::read), Base&, etl::type_list<>>::value));
    CHECK_TRUE((etl::is_invocable_r<int, decltype(&VolatileBase::read), volatile VolatileBase&, etl::type_list<>>::value));
  }

  //*************************************************************************
  TEST(test_member_object_pointer)
  {
    int MemberObj::* p_int = &MemberObj::i;

    CHECK_TRUE((etl::is_invocable<decltype(p_int), MemberObj&>::value));
    CHECK_TRUE((etl::is_invocable<decltype(p_int), MemberObj*>::value));
    CHECK_FALSE((etl::is_invocable<decltype(p_int)>::value));      // Needs object
    CHECK_FALSE((etl::is_invocable<decltype(p_int), int>::value)); // Wrong object type

    // Non-const object: result is int&, should satisfy int&
    CHECK_TRUE((etl::is_invocable_r<int&, decltype(p_int), MemberObj&>::value));

    // Const object: result is const int&, cannot bind to int&
    CHECK_FALSE((etl::is_invocable_r<int&, decltype(p_int), const MemberObj&>::value));

    // Const object can bind to const int&
    CHECK_TRUE((etl::is_invocable_r<const int&, decltype(p_int), const MemberObj&>::value));
  }

  //*************************************************************************
  TEST(test_functor_non_const)
  {
    CHECK_TRUE((etl::is_invocable<Functor, int>::value));
    CHECK_FALSE((etl::is_invocable<Functor, int, int>::value));           // Arity mismatch

    // Use etl::type_list
    CHECK_TRUE((etl::is_invocable<Functor, etl::type_list<int>>::value));
    CHECK_FALSE((etl::is_invocable<Functor, etl::type_list<int, int>>::value));           // Arity mismatch
  }

  //*************************************************************************
  TEST(test_functor_non_const_return)
  {
    CHECK_TRUE((etl::is_invocable_r<void,         Functor, int>::value));
    CHECK_TRUE((etl::is_invocable_r<int,          Functor, int>::value));
    CHECK_FALSE((etl::is_invocable_r<std::string, Functor, int>::value));           // Return mismatch
    CHECK_FALSE((etl::is_invocable_r<int,         Functor, int, int>::value));      // Arity mismatch

    // Use etl::type_list
    CHECK_TRUE((etl::is_invocable_r<void,         Functor, etl::type_list<int>>::value));
    CHECK_TRUE((etl::is_invocable_r<int,          Functor, etl::type_list<int>>::value));
    CHECK_FALSE((etl::is_invocable_r<std::string, Functor, etl::type_list<int>>::value));           // Return mismatch
    CHECK_FALSE((etl::is_invocable_r<int,         Functor, etl::type_list<int, int>>::value));      // Arity mismatch
  }

  //*************************************************************************
  TEST(test_functor_const)
  {
    CHECK_TRUE((etl::is_invocable<ConstFunctor,  int>::value));
    CHECK_FALSE((etl::is_invocable<ConstFunctor, int, int>::value)); // Arity mismatch

    // Use etl::type_list
    CHECK_TRUE((etl::is_invocable<ConstFunctor,  etl::type_list<int>>::value));
    CHECK_FALSE((etl::is_invocable<ConstFunctor, etl::type_list<int, int>>::value)); // Arity mismatch
  }

  //*************************************************************************
  TEST(test_functor_const_return)
  {
    CHECK_TRUE((etl::is_invocable_r<void,         ConstFunctor, int>::value));
    CHECK_TRUE((etl::is_invocable_r<int,          ConstFunctor, int>::value));
    CHECK_FALSE((etl::is_invocable_r<std::string, ConstFunctor, int>::value));      // Return mismatch
    CHECK_FALSE((etl::is_invocable_r<int,         ConstFunctor, int, int>::value)); // Arity mismatch
  
    // Use etl::type_list
    CHECK_TRUE((etl::is_invocable_r<void,         ConstFunctor, etl::type_list<int>>::value));
    CHECK_TRUE((etl::is_invocable_r<int,          ConstFunctor, etl::type_list<int>>::value));
    CHECK_FALSE((etl::is_invocable_r<std::string, ConstFunctor, etl::type_list<int>>::value));      // Return mismatch
    CHECK_FALSE((etl::is_invocable_r<int,         ConstFunctor, etl::type_list<int, int>>::value)); // Arity mismatch
  }

  //*************************************************************************
  TEST(test_move_only_functor)
  {
    CHECK_TRUE((etl::is_invocable<MoveOnlyFunctor,  int>::value));
    CHECK_FALSE((etl::is_invocable<MoveOnlyFunctor, int, int>::value)); // Arity mismatch
  
    // Use etl::type_list
    CHECK_TRUE((etl::is_invocable<MoveOnlyFunctor,  etl::type_list<int>>::value));
    CHECK_FALSE((etl::is_invocable<MoveOnlyFunctor, etl::type_list<int, int>>::value)); // Arity mismatch
  }

  //*************************************************************************
  TEST(test_move_only_functor_return)
  {
    CHECK_TRUE((etl::is_invocable_r<void,         MoveOnlyFunctor, int>::value));
    CHECK_TRUE((etl::is_invocable_r<int,          MoveOnlyFunctor, int>::value));
    CHECK_FALSE((etl::is_invocable_r<std::string, MoveOnlyFunctor, int>::value));      // Return mismatch
    CHECK_FALSE((etl::is_invocable_r<int,         MoveOnlyFunctor, int, int>::value)); // Arity mismatch

    // Use etl::type_list
    CHECK_TRUE((etl::is_invocable_r<void,         MoveOnlyFunctor, etl::type_list<int>>::value));
    CHECK_TRUE((etl::is_invocable_r<int,          MoveOnlyFunctor, etl::type_list<int>>::value));
    CHECK_FALSE((etl::is_invocable_r<std::string, MoveOnlyFunctor, etl::type_list<int>>::value));      // Return mismatch
    CHECK_FALSE((etl::is_invocable_r<int,         MoveOnlyFunctor, etl::type_list<int, int>>::value)); // Arity mismatch
  }

  //*************************************************************************
  TEST(test_overloaded_functor_ref_qualifier)
  {
    // lvalue should pick operator()&
    CHECK_TRUE((etl::is_invocable<OverloadedFunctor&, int>::value));
    CHECK_FALSE((etl::is_invocable<OverloadedFunctor&, int, int>::value));

    // rvalue temporary should pick operator()&&
    CHECK_TRUE((etl::is_invocable<OverloadedFunctor&&, int>::value));

    // Use etl::type_list
    // lvalue should pick operator()&
    CHECK_TRUE((etl::is_invocable<OverloadedFunctor&, etl::type_list<int>>::value));
    CHECK_FALSE((etl::is_invocable<OverloadedFunctor&, etl::type_list<int, int>>::value));

    // rvalue temporary should pick operator()&&
    CHECK_TRUE((etl::is_invocable<OverloadedFunctor&&, etl::type_list<int>>::value));
  }

  //*************************************************************************
  TEST(test_nothrow_and_throwing_functors)
  {
    CHECK_TRUE((etl::is_invocable<NothrowFunctor>::value));
    CHECK_TRUE((etl::is_invocable_r<int, NothrowFunctor>::value));

    CHECK_TRUE((etl::is_invocable<ThrowingFunctor>::value));
    CHECK_TRUE((etl::is_invocable_r<int, ThrowingFunctor>::value));
  }

  //*************************************************************************
  TEST(test_lambda_non_capturing)
  {
    CHECK_TRUE((etl::is_invocable<decltype(lambda_nc), int, int>::value));
    CHECK_FALSE((etl::is_invocable<decltype(lambda_nc), int>::value)); // Arity mismatch
  
    // Use etl::type_list
    CHECK_TRUE((etl::is_invocable<decltype(lambda_nc), etl::type_list<int, int>>::value));
    CHECK_FALSE((etl::is_invocable<decltype(lambda_nc), etl::type_list<int>>::value)); // Arity mismatch
  }

  //*************************************************************************
  TEST(test_lambda_non_capturing_return)
  {
    CHECK_TRUE((etl::is_invocable_r<void, decltype(lambda_nc), int, int>::value));
    CHECK_TRUE((etl::is_invocable_r<int, decltype(lambda_nc), int, int>::value));
    CHECK_FALSE((etl::is_invocable_r<std::string, decltype(lambda_nc), int>::value)); // Return mismatch
    CHECK_FALSE((etl::is_invocable_r<int,         decltype(lambda_nc), int>::value)); // Arity mismatch

    // Use etl::type_list
    CHECK_TRUE((etl::is_invocable_r<void, decltype(lambda_nc), etl::type_list<int, int>>::value));
    CHECK_TRUE((etl::is_invocable_r<int, decltype(lambda_nc), etl::type_list<int, int>>::value));
    CHECK_FALSE((etl::is_invocable_r<std::string, decltype(lambda_nc), etl::type_list<int>>::value)); // Return mismatch
    CHECK_FALSE((etl::is_invocable_r<int,         decltype(lambda_nc), etl::type_list<int>>::value)); // Arity mismatch
  }

  //*************************************************************************
  TEST(test_lambda_capturing)
  {
    int capture_value = 5;
    auto lambda_cap = [capture_value](int a) { return a + capture_value; };

    CHECK_TRUE((etl::is_invocable<decltype(lambda_cap), int>::value));
    CHECK_FALSE((etl::is_invocable<decltype(lambda_cap), int, int>::value)); // Arity mismatch

    // Use etl::type_list
    CHECK_TRUE((etl::is_invocable<decltype(lambda_cap), etl::type_list<int>>::value));
    CHECK_FALSE((etl::is_invocable<decltype(lambda_cap), etl::type_list<int>, int>::value)); // Arity mismatch
  }

  //*************************************************************************
  TEST(test_lambda_capturing_return)
  {
    int capture_value = 5;
    auto lambda_cap = [capture_value](int a) { return a + capture_value; };

    CHECK_TRUE((etl::is_invocable_r<void,         decltype(lambda_cap), int>::value));
    CHECK_TRUE((etl::is_invocable_r<int,          decltype(lambda_cap), int>::value));
    CHECK_FALSE((etl::is_invocable_r<std::string, decltype(lambda_cap), int>::value));      // Return mismatch
    CHECK_FALSE((etl::is_invocable_r<int,         decltype(lambda_cap), int, int>::value)); // Arity mismatch

    // Use etl::type_list
    CHECK_TRUE((etl::is_invocable_r<void,         decltype(lambda_cap), etl::type_list<int>>::value));
    CHECK_TRUE((etl::is_invocable_r<int,          decltype(lambda_cap), etl::type_list<int>>::value));
    CHECK_FALSE((etl::is_invocable_r<std::string, decltype(lambda_cap), etl::type_list<int>>::value));      // Return mismatch
    CHECK_FALSE((etl::is_invocable_r<int,         decltype(lambda_cap), etl::type_list<int, int>>::value)); // Arity mismatch
  }

#if ETL_USING_CPP14
  //*************************************************************************
  TEST(test_lambda_generic)
  {
    CHECK_TRUE((etl::is_invocable<decltype(lambda_generic), int, int>::value));
    CHECK_TRUE((etl::is_invocable<decltype(lambda_generic), long, short>::value));
    CHECK_FALSE((etl::is_invocable<decltype(lambda_generic), int>::value)); // Arity mismatch

    // Use etl::type_list
    CHECK_TRUE((etl::is_invocable<decltype(lambda_generic), etl::type_list<int, int>>::value));
    CHECK_TRUE((etl::is_invocable<decltype(lambda_generic), etl::type_list<long, short>>::value));
    CHECK_FALSE((etl::is_invocable<decltype(lambda_generic), etl::type_list<int>>::value)); // Arity mismatch
  }

  //*************************************************************************
  TEST(test_lambda_generic_return)
  {
    CHECK_TRUE((etl::is_invocable_r<int,          decltype(lambda_generic), int, int>::value));
    CHECK_TRUE((etl::is_invocable_r<long,         decltype(lambda_generic), long, short>::value));

    CHECK_FALSE((etl::is_invocable_r<std::string, decltype(lambda_generic), int, int>::value)); // Return mismatch
    CHECK_FALSE((etl::is_invocable_r<int,         decltype(lambda_generic), int>::value));      // Arity mismatch

    // Use etl::type_list
    CHECK_TRUE((etl::is_invocable_r<int,          decltype(lambda_generic), etl::type_list<int, int>>::value));
    CHECK_TRUE((etl::is_invocable_r<long,         decltype(lambda_generic), etl::type_list<long, short>>::value));
    CHECK_FALSE((etl::is_invocable_r<std::string, decltype(lambda_generic), etl::type_list<int, int>>::value)); // Return mismatch
    CHECK_FALSE((etl::is_invocable_r<int,         decltype(lambda_generic), etl::type_list<int>>::value));      // Arity mismatch
  }
#endif

  //*************************************************************************
  TEST(test_template_functor_integral_only)
  {
    CHECK_TRUE((etl::is_invocable<IntegralOnly<int>,        int>::value));
    CHECK_TRUE((etl::is_invocable_r<int, IntegralOnly<int>, int>::value));

    CHECK_FALSE((etl::is_invocable<IntegralOnly<int>, double>::value));
    CHECK_FALSE((etl::is_invocable<IntegralOnly<int>, float>::value));

    // Use etl::type_list
    CHECK_TRUE((etl::is_invocable<IntegralOnly<int>,        etl::type_list<int>>::value));
    CHECK_TRUE((etl::is_invocable_r<int, IntegralOnly<int>, etl::type_list<int>>::value));

    CHECK_FALSE((etl::is_invocable<IntegralOnly<int>,        etl::type_list<double>>::value));
    CHECK_FALSE((etl::is_invocable_r<int, IntegralOnly<int>, etl::type_list<float>>::value));
  }

  //*************************************************************************
  TEST(test_return_type_conversion)
  {
    CHECK_TRUE((etl::is_invocable_r<int, decltype(free_short), int>::value));          // short -> int
    CHECK_TRUE((etl::is_invocable_r<short, decltype(free_short), int>::value));        // exact
    CHECK_TRUE((etl::is_invocable_r<void, decltype(free_short), int>::value));         // short -> void
    CHECK_FALSE((etl::is_invocable_r<std::string, decltype(free_short), int>::value)); // mismatch

    // Use etl::type_list
    CHECK_TRUE((etl::is_invocable_r<int, decltype(free_short),         etl::type_list<int>>::value));  // short -> int
    CHECK_TRUE((etl::is_invocable_r<short, decltype(free_short),        etl::type_list<int>>::value)); // exact
    CHECK_TRUE((etl::is_invocable_r<void, decltype(free_short),         etl::type_list<int>>::value)); // short -> void
    CHECK_FALSE((etl::is_invocable_r<std::string, decltype(free_short), etl::type_list<int>>::value)); // mismatch
  }

  //*************************************************************************
  TEST(test_additional_user_defined_conversion)
  {
    struct Converter { operator int() const { return 11; } };

    CHECK_TRUE((etl::is_invocable<decltype(free_add), Converter, int>::value)); // Converter -> int
    CHECK_TRUE((etl::is_invocable_r<int, decltype(free_add), Converter, int>::value));

    // Use etl::type_list
    CHECK_TRUE((etl::is_invocable<decltype(free_add), Converter, etl::type_list<int>>::value)); // Converter -> int
    CHECK_TRUE((etl::is_invocable_r<int, decltype(free_add), Converter, etl::type_list<int>>::value));
  }

  //*************************************************************************
  TEST(test_overload_set)
  {
    // Selecting explicit overload
    int (*ov_i)(int)   = &overload;
    long (*ov_l)(long) = &overload;

    CHECK_TRUE((etl::is_invocable<decltype(ov_i), int>::value));
    CHECK_TRUE((etl::is_invocable<decltype(ov_l), long>::value));

    // Use etl::type_list
    CHECK_TRUE((etl::is_invocable<decltype(ov_i), etl::type_list<int>>::value));
    CHECK_TRUE((etl::is_invocable<decltype(ov_l), etl::type_list<long>>::value));
  }

  //*************************************************************************
  TEST(test_overload_set_return)
  {
    int (*ov_i)(int)   = &overload;
    long (*ov_l)(long) = &overload;

    CHECK_TRUE((etl::is_invocable_r<int,  decltype(ov_i), int>::value));
    CHECK_TRUE((etl::is_invocable_r<long, decltype(ov_l), long>::value));

    // Use etl::type_list
    CHECK_TRUE((etl::is_invocable_r<int,  decltype(ov_i), etl::type_list<int>>::value));
    CHECK_TRUE((etl::is_invocable_r<long, decltype(ov_l), etl::type_list<long>>::value));
  }

  //*************************************************************************
  TEST(test_deleted_overload)
  {
    struct DeletedOverload
    {
      int operator()(double) { return 0; }
      int operator()(int) = delete;
    };

    CHECK_TRUE((etl::is_invocable<DeletedOverload,  double>::value));
    CHECK_FALSE((etl::is_invocable<DeletedOverload, int>::value)); // deleted

    // Use etl::type_list
    CHECK_TRUE((etl::is_invocable<DeletedOverload,  etl::type_list<double>>::value));
    CHECK_FALSE((etl::is_invocable<DeletedOverload, etl::type_list<int>>::value)); // deleted
  }

  //*************************************************************************
  TEST(test_variadic_free_function)
  {
    CHECK_TRUE((etl::is_invocable<decltype(varfn), int>::value));
    CHECK_TRUE((etl::is_invocable<decltype(varfn), int, double, const char*>::value));
    CHECK_FALSE((etl::is_invocable<decltype(varfn)>::value)); // Needs at least one argument

    // Use etl::type_list
    CHECK_TRUE((etl::is_invocable<decltype(varfn), etl::type_list<int>>::value));
    CHECK_TRUE((etl::is_invocable<decltype(varfn), etl::type_list<int, double, const char*>>::value));
  }

  //*************************************************************************
  TEST(test_array_decay_argument)
  {  
    CHECK_TRUE((etl::is_invocable<decltype(takes_ptr), int*>::value));
    CHECK_TRUE((etl::is_invocable<decltype(takes_ptr), int[1]>::value)); // array decays
    CHECK_FALSE((etl::is_invocable<decltype(takes_ptr), long*>::value));
    CHECK_FALSE((etl::is_invocable<decltype(takes_ptr)>::value));

    // Use etl::type_list
    CHECK_TRUE((etl::is_invocable<decltype(takes_ptr), etl::type_list<int*>>::value));
    CHECK_TRUE((etl::is_invocable<decltype(takes_ptr), etl::type_list<int[1]>>::value));  // array decays
    CHECK_FALSE((etl::is_invocable<decltype(takes_ptr), etl::type_list<long*>>::value));
  }

  //*************************************************************************
  TEST(test_template_functor_multiple_enable_if)
  {
    CHECK_TRUE((etl::is_invocable<Selective, int>::value));
    CHECK_TRUE((etl::is_invocable<Selective, char>::value));
    CHECK_FALSE((etl::is_invocable<Selective, double>::value));

    // Use etl::type_list
    CHECK_TRUE((etl::is_invocable<Selective, etl::type_list<int>>::value));
    CHECK_TRUE((etl::is_invocable<Selective, etl::type_list<char>>::value));
    CHECK_FALSE((etl::is_invocable<Selective, etl::type_list<double>>::value));
  }

  //*************************************************************************
  TEST(test_rvalue_ref_member_on_temporary)
  {
    struct RR
    {
      int f() && { return 1; }
      int g() &  { return 2; }
    };

    CHECK_TRUE((etl::is_invocable<decltype(&RR::f), RR&&>::value));    // rvalue-qualified on temporary
    CHECK_FALSE((etl::is_invocable<decltype(&RR::f), RR&>::value));    // cannot call on lvalue
    CHECK_TRUE((etl::is_invocable<decltype(&RR::g), RR&>::value));     // lvalue-qualified
    CHECK_FALSE((etl::is_invocable<decltype(&RR::g), RR&&>::value));   // cannot call on rvalue
  }

  //*************************************************************************
  TEST(test_void_return_exactness)
  {
    struct VoidFn 
    { 
      void operator()(int) {} 
    };

    CHECK_TRUE((etl::is_invocable<VoidFn, int>::value));
    CHECK_TRUE((etl::is_invocable_r<void, VoidFn, int>::value));
    CHECK_FALSE((etl::is_invocable_r<int, VoidFn, int>::value)); // void not convertible to int

    // Use etl::type_list
    CHECK_TRUE((etl::is_invocable<VoidFn, etl::type_list<int>>::value));
    CHECK_TRUE((etl::is_invocable_r<void, VoidFn, etl::type_list<int>>::value));
    CHECK_FALSE((etl::is_invocable_r<int, VoidFn, etl::type_list<int>>::value));  // void not convertible to int
  }

  //*************************************************************************
  TEST(test_conversion_chain_failure)
  {
    enum class E : unsigned 
    { 
      A = 0 
    };
    
    struct NeedsE 
    { 
      int operator()(E) 
      { 
        return 0; 
      } 
    };

    // int not implicitly convertible to scoped enum
    CHECK_FALSE((etl::is_invocable<NeedsE, int>::value));
    
    // Underlying type convertible when cast explicit; still trait should be false without cast
    CHECK_FALSE((etl::is_invocable_r<int, NeedsE, int>::value));

    // Use etl::type_list
    CHECK_FALSE((etl::is_invocable<NeedsE, etl::type_list<int>>::value));
    CHECK_FALSE((etl::is_invocable_r<int, NeedsE, etl::type_list<int>>::value));
  }

  //*************************************************************************
  TEST(test_invoke_with_null_member_pointer_type_only)
  {
    int (Base::*pmf)(int) = nullptr;

    // Trait should report invocable even though runtime call would be UB
    CHECK_TRUE((etl::is_invocable<decltype(pmf), Base*, int>::value));

    // Use etl::type_list
    CHECK_TRUE((etl::is_invocable<decltype(pmf), Base*, etl::type_list<int>>::value));
  }
}
