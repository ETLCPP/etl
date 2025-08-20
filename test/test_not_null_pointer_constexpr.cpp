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

#if ETL_USING_CPP14
namespace
{
  struct S
  {
    int x;

    constexpr int get() const
    {
      return x;
    }
  };

  //*************************************************************************
  constexpr etl::not_null<const int*> CreateNotNullAssignFromPointer(const int* p1, const int* p2)
  {
    // Create a not_null pointer from a pointer.
    etl::not_null<const int*> nn(p1);

    // Assign a different pointer to the not_null.
    nn = p2;

    return nn;
  }

  //*************************************************************************
  constexpr etl::not_null<const int*> CreateNotNullAssignFromNotNull(const int* p1, const int* p2)
  {
    // Create a not_null pointer from a pointer.
    etl::not_null<const int*> nn1(p1);
    etl::not_null<const int*> nn2(p2);

    nn1 = nn2;

    return nn1;
  }

  SUITE(test_not_null_pointer)
  {
    //*************************************************************************
    TEST(test_construct_from_non_null_pointer)
    {
      static constexpr const int value = 123;
      static constexpr etl::not_null<const int*> nn(&value);
      static constexpr const int* p = nn.get();
      static constexpr const int  v = *nn;
      CHECK_EQUAL(&value, p);
      CHECK_EQUAL(123, v);
    }

    //*************************************************************************
    TEST(test_copy_construct)
    {
      static constexpr const int value = 123;
      static constexpr etl::not_null<const int*> nn1(&value);
      static constexpr etl::not_null<const int*> nn2(nn1); // Copy constructor
      static constexpr const int* p = nn2.get();
      static constexpr const int  v = *nn2;
      CHECK_EQUAL(&value, p);
      CHECK_EQUAL(123, v);
    }

    //*************************************************************************
    TEST(test_assign_from_pointer)
    {
      static constexpr const int value1 = 123;
      static constexpr const int value2 = 456;
      static constexpr etl::not_null<const int*> nn(CreateNotNullAssignFromPointer(&value1, &value2));

      CHECK_EQUAL(&value2, nn.get());
      CHECK_EQUAL(456, *nn);
    }

    //*************************************************************************
    TEST(test_assign_from_not_null)
    {
      static constexpr const int value1 = 123;
      static constexpr const int value2 = 456;
      static constexpr etl::not_null<const int*> nn(CreateNotNullAssignFromNotNull(&value1, &value2));

      CHECK_EQUAL(&value2, nn.get());
      CHECK_EQUAL(456, *nn);
    }

    //*************************************************************************
    TEST(test_implicit_conversion)
    {
      static constexpr const S s{123};
      static constexpr etl::not_null<const S*> nn(&s);

      static constexpr const S* raw = nn;
      CHECK_EQUAL(&s, raw);
    }

    //*************************************************************************
    TEST(test_arrow_operator)
    {
      static constexpr const S s{123};
      static constexpr etl::not_null<const S*> nn(&s);

      static constexpr int x1 = nn->x;
      static constexpr int x2 = nn->get();

      CHECK_EQUAL(s.x, x1);
      CHECK_EQUAL(s.get(), x2);
    }

    //*************************************************************************
    TEST(test_dereference_operator)
    {
      static constexpr const S s{123};
      static constexpr etl::not_null<const S*> nn(&s);

      static constexpr int x1 = (*nn).x;
      static constexpr int x2 = (*nn).get();

      CHECK_EQUAL(s.x, x1);
      CHECK_EQUAL(s.get(), x2);
    }
  };
}

#endif
