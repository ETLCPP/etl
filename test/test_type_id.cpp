/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl
https://www.etlcpp.com

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

#include "etl/type_id.h"
#include "etl/type_traits.h"

namespace
{
  struct A {};
  struct B {};
  struct C {};

  SUITE(test_type_id)
  {
    //*************************************************************************
    TEST(test_default_construction)
    {
      etl::type_id id_a1;;
      etl::type_id id_a2;

      CHECK_TRUE(id_a1 == etl::type_id::invalid_id());
      CHECK_TRUE(id_a2 == etl::type_id::invalid_id());

      CHECK_TRUE(id_a1 == id_a2);
    }

    //*************************************************************************
    TEST(test_get_same_type_returns_same_id)
    {
      etl::type_id id_a1 = etl::type_id::get<A>();
      etl::type_id id_a2 = etl::type_id::get<A>();

      CHECK_TRUE(id_a1 != etl::type_id::invalid_id());
      CHECK_TRUE(id_a2 != etl::type_id::invalid_id());

      CHECK_TRUE(id_a1 == id_a2);
    }

    //*************************************************************************
    TEST(test_different_types_have_different_ids)
    {
      etl::type_id id_a = etl::type_id::get<A>();
      etl::type_id id_b = etl::type_id::get<B>();
      etl::type_id id_c = etl::type_id::get<C>();

      CHECK_TRUE(id_a != etl::type_id::invalid_id());
      CHECK_TRUE(id_b != etl::type_id::invalid_id());
      CHECK_TRUE(id_c != etl::type_id::invalid_id());

      CHECK_TRUE(id_a != id_b);
      CHECK_TRUE(id_a != id_c);
      CHECK_TRUE(id_b != id_c);

      CHECK_TRUE((id_a < id_b) || (id_b < id_a));
      CHECK_TRUE((id_a < id_c) || (id_c < id_a));
      CHECK_TRUE((id_b < id_c) || (id_c < id_b));
    }

    //*************************************************************************
    TEST(test_constexpr_type_id)
    {
      constexpr etl::type_id id_a = etl::type_id::get<A>();
      constexpr etl::type_id id_b = etl::type_id::get<B>();
      constexpr etl::type_id id_c = etl::type_id::get<C>();

      constexpr etl::type_id invalid_id = etl::type_id::invalid_id();

      constexpr bool id_a_valid = (id_a != invalid_id);
      constexpr bool id_b_valid = (id_b != invalid_id);
      constexpr bool id_c_valid = (id_c != invalid_id);

      constexpr bool id_a_b_not_equal = (id_a != id_b);
      constexpr bool id_a_c_not_equal = (id_a != id_c);
      constexpr bool id_b_c_not_equal = (id_b != id_c);

      CHECK_TRUE(id_a_valid);
      CHECK_TRUE(id_b_valid);
      CHECK_TRUE(id_c_valid);

      CHECK_TRUE(id_a_b_not_equal);
      CHECK_TRUE(id_a_c_not_equal);
      CHECK_TRUE(id_b_c_not_equal);
    }

    //*************************************************************************
    TEST(test_cv_and_ref_qualifiers_are_removed)
    {
      using value_type = A;

      etl::type_id id_value          = etl::type_id::get<value_type>();
      etl::type_id id_const          = etl::type_id::get<const value_type>();
      etl::type_id id_volatile       = etl::type_id::get<volatile value_type>();
      etl::type_id id_const_volatile = etl::type_id::get<const volatile value_type>();

      etl::type_id id_ref            = etl::type_id::get<value_type&>();
      etl::type_id id_const_ref      = etl::type_id::get<const value_type&>();
      etl::type_id id_volatile_ref   = etl::type_id::get<volatile value_type&>();
      etl::type_id id_cv_ref         = etl::type_id::get<const volatile value_type&>();

      CHECK_TRUE(id_value          != etl::type_id::invalid_id());
      CHECK_TRUE(id_const          != etl::type_id::invalid_id());
      CHECK_TRUE(id_volatile       != etl::type_id::invalid_id());
      CHECK_TRUE(id_const_volatile != etl::type_id::invalid_id());
      CHECK_TRUE(id_ref            != etl::type_id::invalid_id());
      CHECK_TRUE(id_const_ref      != etl::type_id::invalid_id());
      CHECK_TRUE(id_volatile_ref   != etl::type_id::invalid_id());
      CHECK_TRUE(id_cv_ref         != etl::type_id::invalid_id());

      // All cv/ref variations of A should map to the same underlying id
      CHECK_TRUE(id_value == id_const);
      CHECK_TRUE(id_value == id_volatile);
      CHECK_TRUE(id_value == id_const_volatile);
      CHECK_TRUE(id_value == id_ref);
      CHECK_TRUE(id_value == id_const_ref);
      CHECK_TRUE(id_value == id_volatile_ref);
      CHECK_TRUE(id_value == id_cv_ref);
    }

    //*************************************************************************
    TEST(test_pointer_types_have_consistent_ids)
    {
      using value_type = A;

      etl::type_id id_value        = etl::type_id::get<value_type>();
      etl::type_id id_ptr          = etl::type_id::get<value_type*>();
      etl::type_id id_const_ptr    = etl::type_id::get<value_type* const>();
      etl::type_id id_volatile_ptr = etl::type_id::get<value_type* volatile>();
      etl::type_id id_cv_ptr       = etl::type_id::get<value_type* const volatile>();

      CHECK_TRUE(id_value        != etl::type_id::invalid_id());
      CHECK_TRUE(id_ptr          != etl::type_id::invalid_id());
      CHECK_TRUE(id_const_ptr    != etl::type_id::invalid_id());
      CHECK_TRUE(id_volatile_ptr != etl::type_id::invalid_id());
      CHECK_TRUE(id_cv_ptr       != etl::type_id::invalid_id());

      // All cv variations of A* should map to the same underlying pointer id
      CHECK_TRUE(id_ptr == id_const_ptr);
      CHECK_TRUE(id_ptr == id_volatile_ptr);
      CHECK_TRUE(id_ptr == id_cv_ptr);

      // But pointer id must be distinct from the non-pointer A id
      CHECK_TRUE(id_value != id_ptr);
    }

    //*************************************************************************
    TEST(test_different_underlying_types_remain_distinct_with_cvref)
    {
      etl::type_id id_a       = etl::type_id::get<A>();
      etl::type_id id_a_const = etl::type_id::get<const A&>();
      etl::type_id id_b       = etl::type_id::get<B>();
      etl::type_id id_b_const = etl::type_id::get<const B&>();

      CHECK_TRUE(id_a       != etl::type_id::invalid_id());
      CHECK_TRUE(id_a_const != etl::type_id::invalid_id());
      CHECK_TRUE(id_b       != etl::type_id::invalid_id());
      CHECK_TRUE(id_b_const != etl::type_id::invalid_id());

      CHECK_TRUE(id_a == id_a_const);
      CHECK_TRUE(id_b == id_b_const);
      CHECK_TRUE(id_a != id_b);
      CHECK_TRUE(id_a_const != id_b_const);
    }

    //*************************************************************************
    TEST(test_conversion_operators)
    {
      etl::type_id id_a1 = etl::type_id::get<A>();
      etl::type_id id_a2 = etl::type_id::get<A>();
      etl::type_id id_b1 = etl::type_id::get<B>();
      etl::type_id id_b2 = etl::type_id::get<B>();

      constexpr etl::type_id id_c1;
      constexpr etl::type_id id_c2 = etl::type_id::get<A>();
      constexpr etl::type_id id_c3 = etl::type_id::get<B>();
      
      intptr_t  intptr_a1 = intptr_t(id_a1);
      intptr_t  intptr_a2 = intptr_t(id_a2);
      intptr_t  intptr_b1 = intptr_t(id_b1);
      intptr_t  intptr_b2 = intptr_t(id_b2);
      
      uintptr_t uintptr_a1 = uintptr_t(id_a1);
      uintptr_t uintptr_a2 = uintptr_t(id_a2);
      uintptr_t uintptr_b1 = uintptr_t(id_b1);
      uintptr_t uintptr_b2 = uintptr_t(id_b2);

      constexpr bool id_c1_bool = bool(id_c1);
      constexpr bool id_c2_bool = bool(id_c2);
      constexpr bool id_c3_bool = bool(id_c3);

      CHECK_TRUE(intptr_a1 == intptr_a2);
      CHECK_TRUE(intptr_b1 == intptr_b2);
      CHECK_TRUE(intptr_a1 != intptr_b1);

      CHECK_TRUE(uintptr_a1 == uintptr_a2);
      CHECK_TRUE(uintptr_b1 == uintptr_b2);
      CHECK_TRUE(uintptr_a1 != uintptr_b1);

      CHECK_FALSE(id_c1_bool);
      CHECK_TRUE(id_c2_bool);
      CHECK_TRUE(id_c3_bool);
    }
  }
}
