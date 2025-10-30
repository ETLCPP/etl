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

#include "etl/invoke.h"
#include <etl/type_traits.h>

namespace
{
  struct TestClass
  {
    int         member_obj = 42;
    std::string member_str = "hello";

    int get_int()
    {
      return member_obj;
    }

    int const_get_int() const
    {
      return member_obj + 10;
    }
    void set_int(int v)
    {
      member_obj = v;
    }
    static int static_func(int x)
    {
      return x * 2;
    }
  };

  int standalone_func(int a, int b)
  {
    return a + b;
  }

  struct Functor
  {
    int operator()(int x) const
    {
      return x * 5;
    }
  };

  SUITE(test_invoke)
  {
    //*************************************************************************
    TEST(test_type_traits_functions)
    {
      CHECK_TRUE(etl::is_function<int(int)>::value);
      CHECK_TRUE(!etl::is_function<int>::value);
      CHECK_TRUE(!etl::is_function<int*>::value);
      CHECK_TRUE(!etl::is_function<int&>::value);
      CHECK_TRUE(!etl::is_function<int&&>::value);
      CHECK_TRUE(!etl::logical_not_t<etl::true_type>::value);
      CHECK_TRUE(etl::logical_not_t<etl::false_type>::value);
    }

    //*************************************************************************
    TEST(test_type_traits_member_pointers)
    {
      using MemObjPtr = int TestClass::*;
      using MemFnPtr = int (TestClass::*)();

      CHECK_TRUE(etl::is_member_pointer<MemObjPtr>::value);
      CHECK_TRUE(etl::is_member_pointer<MemFnPtr>::value);
      CHECK_TRUE(!etl::is_member_pointer<int*>::value);

      CHECK_TRUE(etl::is_member_object_pointer<MemObjPtr>::value);
      CHECK_TRUE(!etl::is_member_object_pointer<MemFnPtr>::value);

      CHECK_TRUE(!etl::is_member_function_pointer<MemObjPtr>::value);
      CHECK_TRUE(etl::is_member_function_pointer<MemFnPtr>::value);
    }

    //*************************************************************************
    TEST(test_invoke_callable)
    {
      CHECK_EQUAL(30, etl::invoke(standalone_func, 10, 20));

      auto lambda = [](int x)
      { return x * 3; };
      CHECK_EQUAL(15, etl::invoke(lambda, 5));

      Functor f;
      CHECK_EQUAL(60, etl::invoke(f, 12));

      CHECK_EQUAL(8, etl::invoke(TestClass::static_func, 4));
    }

    //*************************************************************************
    TEST(test_invoke_mem_func_ptr)
    {
      TestClass       obj;
      TestClass*      ptr = &obj;
      const TestClass const_obj;

      CHECK_EQUAL(42, etl::invoke(&TestClass::get_int, obj));
      CHECK_EQUAL(42, etl::invoke(&TestClass::get_int, ptr));
      CHECK_EQUAL(52, etl::invoke(&TestClass::const_get_int, const_obj));

      etl::invoke(&TestClass::set_int, obj, 99);
      CHECK_EQUAL(99, obj.member_obj);

      etl::invoke(&TestClass::set_int, ptr, 101);
      CHECK_EQUAL(101, ptr->member_obj);
    }

    //*************************************************************************
    TEST(test_invoke_mem_obj_ptr)
    {
      TestClass  obj;
      TestClass* ptr = &obj;

      CHECK_EQUAL(42, etl::invoke(&TestClass::member_obj, obj));
      CHECK_EQUAL(42, etl::invoke(&TestClass::member_obj, ptr));
      CHECK_EQUAL("hello", etl::invoke(&TestClass::member_str, obj));

      etl::invoke(&TestClass::member_obj, obj) = 1000;
      CHECK_EQUAL(1000, obj.member_obj);

      etl::invoke(&TestClass::member_obj, ptr) = 2000;
      CHECK_EQUAL(2000, ptr->member_obj);
    }

    //*************************************************************************
    TEST(test_invoke_result_t)
    {
      using MemFnPtr = decltype(&TestClass::get_int);
      using ConstMemFnPtr = decltype(&TestClass::const_get_int);
      using MemObjPtr_int = decltype(&TestClass::member_obj);
      using FnPtr = int (*)(int, int);
    
      auto val = etl::is_same<etl::invoke_result_t<FnPtr, int, int>, int>::value;
      CHECK_TRUE(val);

      val = etl::is_same<etl::invoke_result_t<MemFnPtr, TestClass&>, int>::value;
      CHECK_TRUE(val);
      
      val = etl::is_same<etl::invoke_result_t<MemFnPtr, TestClass*>, int>::value;
      CHECK_TRUE(val);
      
      val = etl::is_same<etl::invoke_result_t<ConstMemFnPtr, const TestClass&>, int>::value;
      CHECK_TRUE(val);
      
      val = etl::is_same<etl::invoke_result_t<MemObjPtr_int, TestClass&>, int&>::value;
      CHECK_TRUE(val);
      
      val = etl::is_same<etl::invoke_result_t<MemObjPtr_int, const TestClass&>, const int&>::value;
      CHECK_TRUE(val);
      
      val = etl::is_same<etl::invoke_result_t<MemObjPtr_int, TestClass*>, int&>::value;
      CHECK_TRUE(val);
    }
  }
}