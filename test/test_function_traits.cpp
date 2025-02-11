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
      CHECK_FALSE(traits::is_const);
      CHECK_EQUAL(0, traits::argument_count);
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
      CHECK_FALSE(traits::is_const);
      CHECK_EQUAL(2 , traits::argument_count);
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
      CHECK_FALSE(traits::is_const);
      CHECK_EQUAL(0, traits::argument_count);
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
      CHECK_TRUE(traits::is_const);
      CHECK_EQUAL(0, traits::argument_count);
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
      CHECK_FALSE(traits::is_const);
      CHECK_EQUAL(2, traits::argument_count);
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
      CHECK_TRUE(traits::is_const);
      CHECK_EQUAL(2, traits::argument_count);
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
      CHECK_FALSE(traits::is_const);
      CHECK_EQUAL(1, traits::argument_count);
    }
  };
}
