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
#include <vector>
#include <ostream>

#include "etl/fixed_iterator.h"

template <typename TIterator>
std::ostream& operator << (std::ostream& os, const etl::fixed_iterator<TIterator>& fi)
{
  os << TIterator(fi);

  return os;
}

namespace 
{		
  SUITE(test_fixed_iterator)
  {
    //*************************************************************************
    TEST(test_default_constructor)
    {
      etl::fixed_iterator<const int*> fi;

      CHECK_EQUAL((const int*)0, fi);
    }

    //*************************************************************************
    TEST(test_copy_constructor)
    {
      int a{};

      etl::fixed_iterator<const int*> fi1(&a);
      etl::fixed_iterator<const int*> fi2(fi1);

      CHECK_EQUAL(fi1, fi2);
    }

    //*************************************************************************
    TEST(test_constructor)
    {
      int a{};

      etl::fixed_iterator<const int*> fi(&a);

      CHECK_EQUAL(&a, fi);
    }

    //*************************************************************************
    TEST(test_make)
    {
      int a{};

      etl::fixed_iterator<const int*> fi = &a;

      CHECK_EQUAL(&a, fi);
    }

    //*************************************************************************
    TEST(test_increment)
    {
      int compare[] = { 1, 2, 3, 4 };

      etl::fixed_iterator<const int*> fi = &compare[1];

      for (int i = 0; i < 10; ++i)
      {
        CHECK_EQUAL(compare[1], *fi);
        ++fi;
        fi++;
      }
    }

    //*************************************************************************
    TEST(test_member_dereference_operator)
    {
      struct Object
      {
        int a;
        int b;
      } object = { 1, 2 };

      etl::fixed_iterator<Object*> fi(&object);

      CHECK_EQUAL(object.a, fi->a);
      CHECK_EQUAL(object.b, fi->b);

      *fi = { 3, 4 };

      CHECK_EQUAL(object.a, fi->a);
      CHECK_EQUAL(object.b, fi->b);
    }

    //*************************************************************************
    TEST(test_conversion_operator)
    {
      int a{};

      etl::fixed_iterator<const int*> fi(&a);

      CHECK_EQUAL(&a, fi);
    }

    //*************************************************************************
    TEST(test_operator_plus_equals)
    {
      int compare[] = { 1, 2, 3, 4 };

      etl::fixed_iterator<const int*> fi = &compare[1];

      for (int i = 0; i < 10; ++i)
      {
        CHECK_EQUAL(compare[1], *fi);
        fi += 1;
      }
    }

    //*************************************************************************
    TEST(test_operator_plus)
    {
      int compare[] = { 1, 2, 3, 4 };

      etl::fixed_iterator<const int*> fi = &compare[1];

      for (int i = 0; i < 10; ++i)
      {
        CHECK_EQUAL(compare[1], *fi);
        fi = fi + 1;
      }
    }

    //*************************************************************************
    TEST(test_decrement)
    {
      int compare[] = { 1, 2, 3, 4 };

      etl::fixed_iterator<const int*> fi = &compare[1];

      for (int i = 0; i < 10; ++i)
      {
        CHECK_EQUAL(compare[1], *fi);
        --fi;
        fi--;
      }
    }

    //*************************************************************************
    TEST(test_operator_minus_equals)
    {
      int compare[] = { 1, 2, 3, 4 };

      etl::fixed_iterator<const int*> fi = &compare[1];

      for (int i = 0; i < 10; ++i)
      {
        CHECK_EQUAL(compare[1], *fi);
        fi -= 1;
      }
    }

    //*************************************************************************
    TEST(test_operator_minus)
    {
      int compare[] = { 1, 2, 3, 4 };

      etl::fixed_iterator<const int*> fi = &compare[1];

      for (int i = 0; i < 10; ++i)
      {
        CHECK_EQUAL(compare[1], *fi);
        fi = fi - 1;
      }
    }

    //*************************************************************************
    TEST(test_assignment)
    {
      int a{};
      int b{};

      etl::fixed_iterator<int*> fi = &a;
      fi = &b;

      CHECK_EQUAL(&b, fi);

      fi = &a;

      CHECK_EQUAL(&a, fi);
    }

    //*************************************************************************
    TEST(test_equality)
    {
      int a{};
      int b{};

      etl::fixed_iterator<int*> fi1 = &a;
      etl::fixed_iterator<int*> fi2 = &a;
      etl::fixed_iterator<int*> fi3 = &b;

      CHECK(fi1 == fi2);
      CHECK(fi1 != fi3);
    }
  };
}
