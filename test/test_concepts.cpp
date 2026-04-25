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
#include <functional>
#include <string>

#if ETL_USING_CPP20
  #include "etl/concepts.h"

namespace
{
  class A
  {
  };

  class B
  {
  };

  class C : public A
  {
  };

  class D : private A
  {
  };

  struct NotDestructible
  {
    ~NotDestructible() = delete;
  };

  struct NotDefaultConstructible
  {
    NotDefaultConstructible(int) {}
  };

  struct NotCopyable
  {
    NotCopyable()                              = default;
    NotCopyable(const NotCopyable&)            = delete;
    NotCopyable(NotCopyable&&)                 = default;
    NotCopyable& operator=(const NotCopyable&) = delete;
    NotCopyable& operator=(NotCopyable&&)      = default;
  };

  struct NotMovable
  {
    NotMovable()                             = default;
    NotMovable(const NotMovable&)            = delete;
    NotMovable(NotMovable&&)                 = delete;
    NotMovable& operator=(const NotMovable&) = delete;
    NotMovable& operator=(NotMovable&&)      = delete;
  };

  struct NotEqualityComparable
  {
  };

  struct EqualityComparableType
  {
    bool operator==(const EqualityComparableType&) const = default;
  };

  struct OrderedType
  {
    int  value;
    auto operator<=>(const OrderedType&) const = default;
  };

  struct BoolPredicate
  {
    bool operator()(int) const
    {
      return true;
    }
  };

  struct IntRelation
  {
    bool operator()(int, int) const
    {
      return true;
    }
  };

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

    //*************************************************************************
    TEST(test_invocable)
    {
      struct Functor
      {
        void operator()() {}
      };
      struct FunctorWithArgs
      {
        int operator()(int, double)
        {
          return 0;
        }
      };

      static_assert(etl::invocable<void()> == true);
      static_assert(etl::invocable<void(int), int> == true);
      static_assert(etl::invocable<Functor> == true);
      static_assert(etl::invocable<FunctorWithArgs, int, double> == true);
      static_assert(etl::invocable<FunctorWithArgs> == false);
      static_assert(etl::invocable<int> == false);
    }

    //*************************************************************************
    TEST(test_regular_invocable)
    {
      struct Functor
      {
        void operator()() {}
      };

      static_assert(etl::regular_invocable<void()> == true);
      static_assert(etl::regular_invocable<Functor> == true);
      static_assert(etl::regular_invocable<int> == false);
    }

    //*************************************************************************
    TEST(test_destructible)
    {
      static_assert(etl::destructible<int> == true);
      static_assert(etl::destructible<std::string> == true);
      static_assert(etl::destructible<NotDestructible> == false);
    }

    //*************************************************************************
    TEST(test_constructible_from)
    {
      static_assert(etl::constructible_from<int> == true);
      static_assert(etl::constructible_from<int, int> == true);
      static_assert(etl::constructible_from<std::string, const char*> == true);
      static_assert(etl::constructible_from<NotDestructible> == false);
    }

    //*************************************************************************
    TEST(test_default_initializable)
    {
      static_assert(etl::default_initializable<int> == true);
      static_assert(etl::default_initializable<std::string> == true);
      static_assert(etl::default_initializable<NotDefaultConstructible> == false);
    }

    //*************************************************************************
    TEST(test_move_constructible)
    {
      static_assert(etl::move_constructible<int> == true);
      static_assert(etl::move_constructible<std::string> == true);
      static_assert(etl::move_constructible<NotMovable> == false);
    }

    //*************************************************************************
    TEST(test_copy_constructible)
    {
      static_assert(etl::copy_constructible<int> == true);
      static_assert(etl::copy_constructible<std::string> == true);
      static_assert(etl::copy_constructible<NotCopyable> == false);
    }

    //*************************************************************************
    TEST(test_equality_comparable)
    {
      static_assert(etl::equality_comparable<int> == true);
      static_assert(etl::equality_comparable<EqualityComparableType> == true);
      static_assert(etl::equality_comparable<NotEqualityComparable> == false);
    }

    //*************************************************************************
    TEST(test_totally_ordered)
    {
      static_assert(etl::totally_ordered<int> == true);
      static_assert(etl::totally_ordered<OrderedType> == true);
      static_assert(etl::totally_ordered<EqualityComparableType> == false);
    }

    //*************************************************************************
    TEST(test_swappable)
    {
      static_assert(etl::swappable<int> == true);
      static_assert(etl::swappable<std::string> == true);
      static_assert(etl::swappable<NotMovable> == false);
    }

    //*************************************************************************
    TEST(test_swappable_with)
    {
      // Positive cases: same-type lvalue references that are swappable
      static_assert(etl::swappable_with<int&, int&> == true);
      static_assert(etl::swappable_with<std::string&, std::string&> == true);

      // Negative cases: unrelated types (no valid swap overload)
      static_assert(etl::swappable_with<int, std::string> == false);

      // Negative case: non-movable type cannot be swapped
      static_assert(etl::swappable_with<NotMovable&, NotMovable&> == false);
    }

    //*************************************************************************
    TEST(test_movable)
    {
      static_assert(etl::movable<int> == true);
      static_assert(etl::movable<std::string> == true);
      static_assert(etl::movable<NotMovable> == false);
    }

    //*************************************************************************
    TEST(test_copyable)
    {
      static_assert(etl::copyable<int> == true);
      static_assert(etl::copyable<std::string> == true);
      static_assert(etl::copyable<NotCopyable> == false);
    }

    //*************************************************************************
    TEST(test_semiregular)
    {
      static_assert(etl::semiregular<int> == true);
      static_assert(etl::semiregular<std::string> == true);
      static_assert(etl::semiregular<NotDefaultConstructible> == false);
    }

    //*************************************************************************
    TEST(test_regular)
    {
      static_assert(etl::regular<int> == true);
      static_assert(etl::regular<std::string> == true);
      static_assert(etl::regular<NotEqualityComparable> == false);
    }

    //*************************************************************************
    TEST(test_predicate)
    {
      static_assert(etl::predicate<BoolPredicate, int> == true);
      static_assert(etl::predicate<bool(int), int> == true);
      static_assert(etl::predicate<void(int), int> == false);
    }

    //*************************************************************************
    TEST(test_relation)
    {
      static_assert(etl::relation<IntRelation, int, int> == true);
      static_assert(etl::relation<std::less<int>, int, int> == true);
      static_assert(etl::relation<BoolPredicate, int, int> == false);
    }

    //*************************************************************************
    TEST(test_equivalence_relation)
    {
      static_assert(etl::equivalence_relation<std::equal_to<int>, int, int> == true);
      static_assert(etl::equivalence_relation<BoolPredicate, int, int> == false);
    }

    //*************************************************************************
    TEST(test_strict_weak_order)
    {
      static_assert(etl::strict_weak_order<std::less<int>, int, int> == true);
      static_assert(etl::strict_weak_order<BoolPredicate, int, int> == false);
    }
  }
} // namespace
#endif
