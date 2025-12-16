/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl
https://www.etlcpp.com

Copyright(c) 2024 John Wellbelove

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

#include "etl/function_traits.h"
#include "etl/type_list.h"

namespace
{
  //*****************************************************************************
  // The free function taking no parameters.
  //*****************************************************************************
  void free_void()
  {
  }

  //*****************************************************************************
  // The free noexcept function taking no parameters.
  //*****************************************************************************
  void free_void_noexcept() noexcept
  {
  }

  //*****************************************************************************
  // The free function taking an int parameter.
  //*****************************************************************************
  int free_int(int i, int j)
  {
    return i + j;
  }

  //*****************************************************************************
  // The test class with member functions.
  //*****************************************************************************
  class Object
  {
  public:

    //*******************************************
    // void
    void member_void()
    {
    }

    void member_void_const() const
    {
    }

    void member_void_noexcept() noexcept
    {
    }

    void member_void_const_noexcept() const noexcept
    {
    }

    void member_void_volatile() volatile
    {
    }

    void member_void_volatile_noexcept() volatile noexcept
    {
    }

    void member_void_const_volatile() const volatile
    {
    }

    void member_void_const_volatile_noexcept() const volatile noexcept
    {
    }

    //*******************************************
    // int
    int member_int(int i, int j)
    {
      return i + j;
    }

    int member_int_const(int i, int j) const
    {
      return i + j;
    }

    //*******************************************
    // static
    static void member_static(int j)
    {
      (void)j;
    }
  };

  //*****************************************************************************
  // A functor with a unique operator()
  //*****************************************************************************
  struct Functor { int operator()(int x) { return x + 1; } };

  //*****************************************************************************
  // A functor with a unique operator()
  //*****************************************************************************
  struct FunctorConst { int operator()(int x) const { return x + 1; } };
}

namespace
{
  SUITE(test_function_traits)
  {
    //*************************************************************************
    TEST(test_free_function_free_void)
    {
      free_void(); // Keep clang happy

      using traits = etl::function_traits<decltype(&free_void)>;

      CHECK_TRUE((std::is_same<void(void),       traits::function_type>::value));
      CHECK_TRUE((std::is_same<void,             traits::return_type>::value));
      CHECK_TRUE((std::is_same<void,             traits::object_type>::value));
      CHECK_TRUE((std::is_same<etl::type_list<>, traits::argument_types>::value));     

      CHECK_TRUE(traits::is_function);
      CHECK_FALSE(traits::is_member_function);
      CHECK_FALSE(traits::is_functor);
      CHECK_FALSE(traits::is_const);
      CHECK_FALSE(traits::is_volatile);
      CHECK_FALSE(traits::is_noexcept);
      CHECK_EQUAL(0, traits::arity);
    }

    //*************************************************************************
    TEST(test_free_function_free_void_noexcept)
    {
      free_void_noexcept(); // Keep clang happy

      using traits = etl::function_traits<decltype(&free_void_noexcept)>;

      CHECK_TRUE((std::is_same<void(void),       traits::function_type>::value));
      CHECK_TRUE((std::is_same<void,             traits::return_type>::value));
      CHECK_TRUE((std::is_same<void,             traits::object_type>::value));
      CHECK_TRUE((std::is_same<etl::type_list<>, traits::argument_types>::value));     

      CHECK_TRUE(traits::is_function);
      CHECK_FALSE(traits::is_member_function);
      CHECK_FALSE(traits::is_functor);
      CHECK_FALSE(traits::is_const);
      CHECK_FALSE(traits::is_volatile);
    }

    //*************************************************************************
    TEST(test_free_function_free_int)
    {
      free_int(1, 2); // Keep clang happy

      using traits = etl::function_traits<decltype(&free_int)>;

      CHECK_TRUE((std::is_same<int(int, int),            traits::function_type>::value));
      CHECK_TRUE((std::is_same<int,                      traits::return_type>::value));
      CHECK_TRUE((std::is_same<void,                     traits::object_type>::value));
      CHECK_TRUE((std::is_same<etl::type_list<int, int>, traits::argument_types>::value));
      
      CHECK_TRUE(traits::is_function);
      CHECK_FALSE(traits::is_member_function);
      CHECK_FALSE(traits::is_functor);
      CHECK_FALSE(traits::is_const);
      CHECK_FALSE(traits::is_volatile);
      CHECK_FALSE(traits::is_noexcept);
      CHECK_EQUAL(2, traits::arity);
    }

    //*************************************************************************
    TEST(test_member_function_void)
    {
      using traits = etl::function_traits<decltype(&Object::member_void)>;

      CHECK_TRUE((std::is_same<void(void),       traits::function_type>::value));
      CHECK_TRUE((std::is_same<void,             traits::return_type>::value));
      CHECK_TRUE((std::is_same<Object,           traits::object_type>::value));
      CHECK_TRUE((std::is_same<etl::type_list<>, traits::argument_types>::value));

      CHECK_FALSE(traits::is_function);
      CHECK_TRUE(traits::is_member_function);
      CHECK_FALSE(traits::is_functor);
      CHECK_FALSE(traits::is_const);
      CHECK_FALSE(traits::is_volatile);
      CHECK_FALSE(traits::is_noexcept);
      CHECK_EQUAL(0, traits::arity);
    }

    //*************************************************************************
    TEST(test_member_function_void_const)
    {
      using traits = etl::function_traits<decltype(&Object::member_void_const)>;

      CHECK_TRUE((std::is_same<void(void),       traits::function_type>::value));
      CHECK_TRUE((std::is_same<void,             traits::return_type>::value));
      CHECK_TRUE((std::is_same<Object,           traits::object_type>::value));
      CHECK_TRUE((std::is_same<etl::type_list<>, traits::argument_types>::value));

      CHECK_FALSE(traits::is_function);
      CHECK_TRUE(traits::is_member_function);
      CHECK_FALSE(traits::is_functor);
      CHECK_TRUE(traits::is_const);
      CHECK_FALSE(traits::is_volatile);
      CHECK_FALSE(traits::is_noexcept);
      CHECK_EQUAL(0, traits::arity);
    }

    //*************************************************************************
    TEST(test_member_function_void_noexcept)
    {
      using traits = etl::function_traits<decltype(&Object::member_void_noexcept)>;

      CHECK_TRUE((std::is_same<void(void),       traits::function_type>::value));
      CHECK_TRUE((std::is_same<void,             traits::return_type>::value));
      CHECK_TRUE((std::is_same<Object,           traits::object_type>::value));
      CHECK_TRUE((std::is_same<etl::type_list<>, traits::argument_types>::value));

      CHECK_FALSE(traits::is_function);
      CHECK_TRUE(traits::is_member_function);
      CHECK_FALSE(traits::is_functor);
      CHECK_FALSE(traits::is_const);
      CHECK_FALSE(traits::is_volatile);
#if ETL_HAS_NOEXCEPT_FUNCTION_TYPE
      CHECK_TRUE(traits::is_noexcept);
#else
      CHECK_FALSE(traits::is_noexcept);
#endif
      CHECK_EQUAL(0, traits::arity);
    }

    //*************************************************************************
    TEST(test_member_function_void_const_noexcept)
    {
      using traits = etl::function_traits<decltype(&Object::member_void_const_noexcept)>;

      CHECK_TRUE((std::is_same<void(void),       traits::function_type>::value));
      CHECK_TRUE((std::is_same<void,             traits::return_type>::value));
      CHECK_TRUE((std::is_same<Object,           traits::object_type>::value));
      CHECK_TRUE((std::is_same<etl::type_list<>, traits::argument_types>::value));

      CHECK_FALSE(traits::is_function);
      CHECK_TRUE(traits::is_member_function);
      CHECK_FALSE(traits::is_functor);
      CHECK_TRUE(traits::is_const);
      CHECK_FALSE(traits::is_volatile);
#if ETL_HAS_NOEXCEPT_FUNCTION_TYPE
      CHECK_TRUE(traits::is_noexcept);
#else
      CHECK_FALSE(traits::is_noexcept);
#endif
      CHECK_EQUAL(0, traits::arity);
    }

    //*************************************************************************
    TEST(test_member_function_void_volatile)
    {
      using traits = etl::function_traits<decltype(&Object::member_void_volatile)>;

      CHECK_TRUE((std::is_same<void(void),       traits::function_type>::value));
      CHECK_TRUE((std::is_same<void,             traits::return_type>::value));
      CHECK_TRUE((std::is_same<Object,           traits::object_type>::value));
      CHECK_TRUE((std::is_same<etl::type_list<>, traits::argument_types>::value));

      CHECK_FALSE(traits::is_function);
      CHECK_TRUE(traits::is_member_function);
      CHECK_FALSE(traits::is_functor);
      CHECK_FALSE(traits::is_const);
      CHECK_TRUE(traits::is_volatile);
      CHECK_FALSE(traits::is_noexcept);
      CHECK_EQUAL(0, traits::arity);
    }

    //*************************************************************************
    TEST(test_member_function_void_const_volatile)
    {
      using traits = etl::function_traits<decltype(&Object::member_void_const_volatile)>;

      CHECK_TRUE((std::is_same<void(void),       traits::function_type>::value));
      CHECK_TRUE((std::is_same<void,             traits::return_type>::value));
      CHECK_TRUE((std::is_same<Object,           traits::object_type>::value));
      CHECK_TRUE((std::is_same<etl::type_list<>, traits::argument_types>::value));

      CHECK_FALSE(traits::is_function);
      CHECK_TRUE(traits::is_member_function);
      CHECK_FALSE(traits::is_functor);
      CHECK_TRUE(traits::is_const);
      CHECK_TRUE(traits::is_volatile);
      CHECK_FALSE(traits::is_noexcept);
      CHECK_EQUAL(0, traits::arity);
    }

    //*************************************************************************
    TEST(test_member_function_void_volatile_noexcept)
    {
      using traits = etl::function_traits<decltype(&Object::member_void_volatile_noexcept)>;

      CHECK_TRUE((std::is_same<void(void),       traits::function_type>::value));
      CHECK_TRUE((std::is_same<void,             traits::return_type>::value));
      CHECK_TRUE((std::is_same<Object,           traits::object_type>::value));
      CHECK_TRUE((std::is_same<etl::type_list<>, traits::argument_types>::value));

      CHECK_FALSE(traits::is_function);
      CHECK_TRUE(traits::is_member_function);
      CHECK_FALSE(traits::is_functor);
      CHECK_FALSE(traits::is_const);
      CHECK_TRUE(traits::is_volatile);
#if ETL_HAS_NOEXCEPT_FUNCTION_TYPE
      CHECK_TRUE(traits::is_noexcept);
#else
      CHECK_FALSE(traits::is_noexcept);
#endif
      CHECK_EQUAL(0, traits::arity);
    }

    //*************************************************************************
    TEST(test_member_function_void_const_volatile_noexcept)
    {
      using traits = etl::function_traits<decltype(&Object::member_void_const_volatile_noexcept)>;

      CHECK_TRUE((std::is_same<void(void),       traits::function_type>::value));
      CHECK_TRUE((std::is_same<void,             traits::return_type>::value));
      CHECK_TRUE((std::is_same<Object,           traits::object_type>::value));
      CHECK_TRUE((std::is_same<etl::type_list<>, traits::argument_types>::value));

      CHECK_FALSE(traits::is_function);
      CHECK_TRUE(traits::is_member_function);
      CHECK_FALSE(traits::is_functor);
      CHECK_TRUE(traits::is_const);
      CHECK_TRUE(traits::is_volatile);
#if ETL_HAS_NOEXCEPT_FUNCTION_TYPE
      CHECK_TRUE(traits::is_noexcept);
#else
      CHECK_FALSE(traits::is_noexcept);
#endif
      CHECK_EQUAL(0, traits::arity);
    }

    //*************************************************************************
    TEST(test_member_function_int)
    {
      using traits = etl::function_traits<decltype(&Object::member_int)>;

      CHECK_TRUE((std::is_same<int(int, int),            traits::function_type>::value));
      CHECK_TRUE((std::is_same<int,                      traits::return_type>::value));
      CHECK_TRUE((std::is_same<Object,                   traits::object_type>::value));
      CHECK_TRUE((std::is_same<etl::type_list<int, int>, traits::argument_types>::value));

      CHECK_FALSE(traits::is_function);
      CHECK_TRUE(traits::is_member_function);
      CHECK_FALSE(traits::is_functor);
      CHECK_FALSE(traits::is_const);
      CHECK_FALSE(traits::is_volatile);
      CHECK_FALSE(traits::is_noexcept);
      CHECK_EQUAL(2, traits::arity);
    }

    //*************************************************************************
    TEST(test_member_function_int_const)
    {
      using traits = etl::function_traits<decltype(&Object::member_int_const)>;

      CHECK_TRUE((std::is_same<int(int, int),            traits::function_type>::value));
      CHECK_TRUE((std::is_same<int,                      traits::return_type>::value));
      CHECK_TRUE((std::is_same<Object,                   traits::object_type>::value));
      CHECK_TRUE((std::is_same<etl::type_list<int, int>, traits::argument_types>::value));

      CHECK_FALSE(traits::is_function);
      CHECK_TRUE(traits::is_member_function);
      CHECK_FALSE(traits::is_functor);
      CHECK_TRUE(traits::is_const);
      CHECK_FALSE(traits::is_volatile);
      CHECK_FALSE(traits::is_noexcept);
      CHECK_EQUAL(2, traits::arity);
    }

    //*************************************************************************
    TEST(test_member_function_static)
    {
      using traits = etl::function_traits<decltype(&Object::member_static)>;

      CHECK_TRUE((std::is_same<void(int),           traits::function_type>::value));
      CHECK_TRUE((std::is_same<void,                traits::return_type>::value));
      CHECK_TRUE((std::is_same<void,                traits::object_type>::value));
      CHECK_TRUE((std::is_same<etl::type_list<int>, traits::argument_types>::value));

      CHECK_TRUE(traits::is_function);
      CHECK_FALSE(traits::is_member_function);
      CHECK_FALSE(traits::is_functor);
      CHECK_FALSE(traits::is_const);
      CHECK_FALSE(traits::is_volatile);
      CHECK_FALSE(traits::is_noexcept);
      CHECK_EQUAL(1, traits::arity);
    }

    //*************************************************************************
    TEST(test_lambda)
    {
      auto lambda = [](int a, const std::string& s) -> long { return static_cast<long>(a + s.size()); };

      using traits = etl::function_traits<decltype(lambda)>;

      CHECK_TRUE((std::is_same<long(int, const std::string&),           traits::function_type>::value));
      CHECK_TRUE((std::is_same<long,                                    traits::return_type>::value));
      CHECK_TRUE((std::is_same<decltype(lambda),                        traits::object_type>::value));
      CHECK_TRUE((std::is_same<etl::type_list<int, const std::string&>, traits::argument_types>::value));

      CHECK_FALSE(traits::is_function);
      CHECK_TRUE(traits::is_member_function);
      CHECK_TRUE(traits::is_functor);
      CHECK_TRUE(traits::is_const);
      CHECK_FALSE(traits::is_volatile);
      CHECK_FALSE(traits::is_noexcept);
      CHECK_EQUAL(2, traits::arity);
    }

    //*************************************************************************
    TEST(test_functor)
    {
      struct functor
      {
        long operator()(int a, const std::string& s)
        {
          return static_cast<long>(a + s.size());
        }
      };

      using traits = etl::function_traits<functor>;

      CHECK_TRUE((std::is_same<long(int, const std::string&),           traits::function_type>::value));
      CHECK_TRUE((std::is_same<long,                                    traits::return_type>::value));
      CHECK_TRUE((std::is_same<functor,                                 traits::object_type>::value));
      CHECK_TRUE((std::is_same<etl::type_list<int, const std::string&>, traits::argument_types>::value));

      CHECK_FALSE(traits::is_function);
      CHECK_TRUE(traits::is_member_function);
      CHECK_TRUE(traits::is_functor);
      CHECK_FALSE(traits::is_const);
      CHECK_FALSE(traits::is_volatile);
      CHECK_FALSE(traits::is_noexcept);
      CHECK_EQUAL(2, traits::arity);
    }

    //*************************************************************************
    // Forwarding of top-level cv/ref on the whole type to the unqualified type
    TEST(test_function_traits_forward_cvref_free_ptr)
    {
      using ptr_t       = decltype(&free_void);
      using const_ptr_t = typename std::add_const<ptr_t>::type;   // void(* const)()
      using ref_ptr_t   = ptr_t&;                                  // void(*&)()

      using traits_c = etl::function_traits<const_ptr_t>;
      using traits_r = etl::function_traits<ref_ptr_t>;

      CHECK_TRUE((std::is_same<void(void), traits_c::function_type>::value));
      CHECK_TRUE((std::is_same<void(void), traits_r::function_type>::value));
      CHECK_TRUE(traits_c::is_function);
      CHECK_TRUE(traits_r::is_function);
      CHECK_FALSE(traits_c::is_member_function);
      CHECK_FALSE(traits_r::is_member_function);
    }

    //*************************************************************************
    TEST(test_function_traits_forward_cvref_member_ptr)
    {
      using mptr_t       = decltype(&Object::member_int);
      using const_mptr_t = typename std::add_const<mptr_t>::type; // int (Object::* const)(int,int)
      using ref_mptr_t   = mptr_t&;                               // int (Object::*&)(int,int)

      using traits_c = etl::function_traits<const_mptr_t>;
      using traits_r = etl::function_traits<ref_mptr_t>;

      CHECK_TRUE((std::is_same<int(int, int), traits_c::function_type>::value));
      CHECK_TRUE((std::is_same<int(int, int), traits_r::function_type>::value));
      CHECK_FALSE(traits_c::is_function);
      CHECK_FALSE(traits_r::is_function);
      CHECK_TRUE(traits_c::is_member_function);
      CHECK_TRUE(traits_r::is_member_function);
      CHECK_TRUE((std::is_same<Object, traits_c::object_type>::value));
      CHECK_TRUE((std::is_same<Object, traits_r::object_type>::value));
    }

    //*************************************************************************
    // Ensure function_traits resolves for Functor and const Functor
    TEST(test_function_traits_functor_and_const_functor)
    {
      using traits_f  = etl::function_traits<Functor>;
      using traits_cf = etl::function_traits<FunctorConst>;

      // Both should be recognized as functor types
      CHECK_TRUE(traits_f::is_functor);
      CHECK_TRUE(traits_cf::is_functor);

      // The function_type should be int(int)
      CHECK_TRUE((std::is_same<int(int), traits_f::function_type>::value));
      CHECK_TRUE((std::is_same<int(int), traits_cf::function_type>::value));

      // const Functor should be marked const
      CHECK_FALSE(traits_f::is_const);
      CHECK_TRUE(traits_cf::is_const);

      CHECK_TRUE((std::is_same<Functor, traits_f::object_type>::value));
      CHECK_TRUE((std::is_same<FunctorConst, traits_cf::object_type>::value));
      CHECK_EQUAL(1, traits_f::arity);
      CHECK_EQUAL(1, traits_cf::arity);
    }
  }
}
