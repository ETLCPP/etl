/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl
https://www.etlcpp.com

Copyright(c) 2025 BMW AG

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

#include <atomic>
#include <string>

#if ETL_USING_CPP20
#include "etl/concepts.h"

namespace
{
  class A
  {};

  class B
  {};

  class C: public A
  {};

  class D: private A
  {};

  SUITE(test_concepts)
  {
    //*************************************************************************
    TEST(test_same_as)
    {
      static_assert(etl::same_as<A, A> == true);
      static_assert(etl::same_as<A, B> == false);
    }

    //*************************************************************************
    TEST(test_derived_from)
    {
      static_assert(etl::derived_from<C, C> == true);
      static_assert(etl::derived_from<int, int> == false);
      static_assert(etl::derived_from<C, A> == true);
      static_assert(etl::derived_from<D, A> == false);
    }

    //*************************************************************************
    TEST(test_convertible_to)
    {
      static_assert(etl::convertible_to<int, int> == true);
    }

    //*************************************************************************
    TEST(test_common_reference_with)
    {
      static_assert(etl::common_reference_with<int, int> == true);
    }

    //*************************************************************************
    TEST(test_common_with)
    {
      static_assert(etl::common_with<int, int> == true);
    }

    //*************************************************************************
    TEST(test_integral)
    {
      static_assert(etl::integral<bool> == true);
      static_assert(etl::integral<int> == true);
      static_assert(etl::integral<double> == false);
      static_assert(etl::integral<char*> == false);
    }

    //*************************************************************************
    TEST(test_signed_integral)
    {
      static_assert(etl::signed_integral<bool> == false);
      static_assert(etl::signed_integral<int> == true);
      static_assert(etl::signed_integral<unsigned int> == false);
      static_assert(etl::signed_integral<double> == false);
      static_assert(etl::signed_integral<char*> == false);
    }

    //*************************************************************************
    TEST(test_unsigned_integral)
    {
      static_assert(etl::unsigned_integral<bool> == true);
      static_assert(etl::unsigned_integral<int> == false);
      static_assert(etl::unsigned_integral<unsigned int> == true);
      static_assert(etl::unsigned_integral<double> == false);
      static_assert(etl::unsigned_integral<char*> == false);
    }

    //*************************************************************************
    TEST(test_floating_point)
    {
      static_assert(etl::floating_point<bool> == false);
      static_assert(etl::floating_point<int> == false);
      static_assert(etl::floating_point<unsigned int> == false);
      static_assert(etl::floating_point<float> == true);
      static_assert(etl::floating_point<double> == true);
      static_assert(etl::floating_point<long double> == true);
      static_assert(etl::floating_point<char*> == false);
    }

    //*************************************************************************
    TEST(test_assignable_from)
    {
      static_assert(etl::assignable_from<int&, int> == true);
      static_assert(etl::assignable_from<A, B> == false);
      static_assert(etl::assignable_from<std::string&, std::string> == true);
      static_assert(etl::assignable_from<int, int> == false);
      static_assert(etl::assignable_from<std::string&&, std::string> == false);
      static_assert(etl::assignable_from<std::string, std::string> == false);
      static_assert(etl::assignable_from<std::atomic<int>&, int> == false);
    }
  }
}
#endif
