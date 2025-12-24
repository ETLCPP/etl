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
#include "etl/not_null.h"

namespace
{
  struct S
  {
    int x;

    int get() const
    {
      return x;
    }
  };

  SUITE(test_not_null_pointer)
  {
    //*************************************************************************
    TEST(test_construct_from_non_null_pointer)
    {
      int value = 123;
      etl::not_null<int*> nn(&value);
      CHECK_EQUAL(&value, nn.get());
      CHECK_EQUAL(123, *nn);
    }

    //*************************************************************************
    TEST(test_copy_construct)
    {
      int value = 123;
      etl::not_null<int*> nn1(&value);
      etl::not_null<int*> nn2(nn1); // Copy constructor
      CHECK_EQUAL(&value, nn2.get());
      CHECK_EQUAL(123, *nn2);
    }

    //*************************************************************************
    TEST(test_assign_from_pointer)
    {
      int value1 = 123;
      etl::not_null<int*> nn1(&value1);

      int value2 = 456;

      nn1 = &value2;
      CHECK_EQUAL(&value2, nn1.get());
      CHECK_EQUAL(456, *nn1);
    }

    //*************************************************************************
    TEST(test_assign_from_not_null)
    {
      int value1 = 123;
      etl::not_null<int*> nn1(&value1);

      int value2 = 456;
      etl::not_null<int*> nn2(&value2);

      nn1 = nn2;
      CHECK_EQUAL(&value2, nn2.get());
      CHECK_EQUAL(456, *nn2);
    }

    //*************************************************************************
    TEST(test_implicit_conversion)
    {
      S s{123};
      etl::not_null<S*> nn(&s);

      S* raw = nn;
      CHECK_EQUAL(&s, raw);
    }

    //*************************************************************************
    TEST(test_arrow_operator)
    {
      S s{123};
      etl::not_null<S*> nn(&s);

      CHECK_EQUAL(s.x, nn->x);
      CHECK_EQUAL(s.get(), nn->get());
    }

    //*************************************************************************
    TEST(test_dereference_operator)
    {
      S s{123};
      etl::not_null<S*> nn(&s);

      CHECK_EQUAL(s.x, (*nn).x);
      CHECK_EQUAL(s.get(), (*nn).get());
    }

    //*************************************************************************
    TEST(test_construct_from_null_pointer_asserts)
    {
      CHECK_THROW(etl::not_null<int*> nn(nullptr), etl::not_null_contains_null);
    }

    //*************************************************************************
    TEST(test_assign_null_pointer_asserts)
    {
      int value = 1;
      etl::not_null<int*> nn(&value);

      CHECK_THROW(nn = nullptr, etl::not_null_contains_null);
    }

    //*************************************************************************
    TEST(test_non_null_comparisons)
    {
      int value[2] = { 123, 456 };
      etl::not_null<int*> nn1(&value[0]);
      etl::not_null<int*> nn2(&value[1]);
      CHECK_TRUE(nn1 == nn1);
      CHECK_TRUE(nn1 != nn2);
      CHECK_TRUE(nn2 != nn1);

      CHECK_FALSE(nn1 < nn1);
      CHECK_TRUE(nn1 < nn2);
      CHECK_FALSE(nn2 < nn1);

      CHECK_TRUE(nn1 <= nn1);
      CHECK_TRUE(nn1 <= nn2);
      CHECK_FALSE(nn2 <= nn1);

      CHECK_FALSE(nn1 > nn1);
      CHECK_FALSE(nn1 > nn2);
      CHECK_TRUE(nn2 > nn1);

      CHECK_TRUE(nn1 >= nn1);
      CHECK_FALSE(nn1 >= nn2);
      CHECK_TRUE(nn2 >= nn1);
    }
  };
}