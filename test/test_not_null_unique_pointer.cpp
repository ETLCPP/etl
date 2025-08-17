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

  SUITE(test_not_null_unique_pointer)
  {
    //*************************************************************************
    TEST(test_construct_from_non_null_unique_ptr)
    {
      using up_t = etl::unique_ptr<int>;
      up_t up(new int{ 123 });
      etl::not_null<up_t> nn(etl::move(up));

      CHECK_EQUAL(123, *nn);
      CHECK_EQUAL(123, *nn.get());
    }

    //*************************************************************************
    TEST(test_assign_from_unique_ptr)
    {
      using up_t = etl::unique_ptr<int>;
      up_t up1(new int{ 123 });
      etl::not_null<up_t> nn1(etl::move(up1));

      using up_t = etl::unique_ptr<int>;
      up_t up2(new int);
      *up2 = 456;

      nn1 = etl::move(up2);

      CHECK_EQUAL(456, *nn1.get());
    }

    //*************************************************************************
    TEST(test_implicit_conversion)
    {
      using up_t = etl::unique_ptr<S>;
      up_t up1(new S{ 123 });
      etl::not_null<up_t> nn1(etl::move(up1));

      S s = *nn1;

      CHECK_EQUAL(123, s.x);
    }

    //*************************************************************************
    TEST(test_arrow_operator)
    {
      S s{ 123 };

      using up_t = etl::unique_ptr<S>;
      up_t up1(new S{ 123 });
      etl::not_null<up_t> nn1(etl::move(up1));

      CHECK_EQUAL(s.x, nn1->x);
      CHECK_EQUAL(s.get(), nn1->get());
    }

    //*************************************************************************
    TEST(test_dereference_operator)
    {
      S s{ 123 };

      using up_t = etl::unique_ptr<S>;
      up_t up1(new S{ 123 });
      etl::not_null<up_t> nn1(etl::move(up1));

      CHECK_EQUAL(s.x, (*nn1).x);
      CHECK_EQUAL(s.get(), (*nn1).get());
    }

    //*************************************************************************
    TEST(test_construct_from_null_pointer_asserts)
    {
      using up_t = etl::unique_ptr<int>;
      up_t up1(nullptr);

      CHECK_THROW(etl::not_null<up_t> nn1(etl::move(up1)), etl::not_null_contains_null);
    }
  }
}