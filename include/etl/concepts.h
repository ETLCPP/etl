///\file

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

#ifndef ETL_CONCEPTS_INCLUDED
#define ETL_CONCEPTS_INCLUDED

#include "platform.h"

#include "invoke.h"
#include "type_traits.h"
#include "utility.h"

#if ETL_USING_CPP20

  #if ETL_USING_STL
    #include <concepts>
  #endif

namespace etl
{

  #if ETL_USING_STL

  using std::assignable_from;
  using std::common_reference_with;
  using std::common_with;
  using std::constructible_from;
  using std::convertible_to;
  using std::copy_constructible;
  using std::copyable;
  using std::default_initializable;
  using std::derived_from;
  using std::destructible;
  using std::equality_comparable;
  using std::equivalence_relation;
  using std::floating_point;
  using std::integral;
  using std::invocable;
  using std::movable;
  using std::move_constructible;
  using std::predicate;
  using std::regular;
  using std::regular_invocable;
  using std::relation;
  using std::same_as;
  using std::semiregular;
  using std::signed_integral;
  using std::strict_weak_order;
  using std::swappable;
  using std::swappable_with;
  using std::totally_ordered;
  using std::unsigned_integral;

  #else // not ETL_USING_STL

  namespace private_concepts
  {
    template <typename T, typename U>
    concept same_as_helper = etl::is_same_v<T, U>;
  }

  //***************************************************************************
  template <typename T, typename U>
  concept same_as = private_concepts::same_as_helper<T, U> && private_concepts::same_as_helper<U, T>;

  //***************************************************************************
  template <typename Derived, typename Base>
  concept derived_from = etl::is_base_of_v<Base, Derived> && etl::is_convertible_v<const volatile Derived*, const volatile Base*>;

  //***************************************************************************
  template <typename From, typename To>
  concept convertible_to = etl::is_convertible_v<From, To> && requires { static_cast<To>(etl::declval<From>()); };

  //***************************************************************************
  template < class T, typename U >
  concept common_reference_with = etl::same_as<etl::common_reference_t<T, U>, etl::common_reference_t<U, T> >
                                  && etl::convertible_to<T, etl::common_reference_t<T, U> > && etl::convertible_to<U, etl::common_reference_t<T, U> >;

  //***************************************************************************
  template <typename T, typename U>
  concept common_with = etl::same_as<etl::common_type_t<T, U>, etl::common_type_t<U, T> > && requires {
    static_cast<etl::common_type_t<T, U> >(etl::declval<T>());
    static_cast<etl::common_type_t<T, U> >(etl::declval<U>());
  } && etl::common_reference_with< etl::add_lvalue_reference_t<const T>, etl::add_lvalue_reference_t<const U> > && etl::common_reference_with< etl::add_lvalue_reference_t<etl::common_type_t<T, U> >, etl::common_reference_t< etl::add_lvalue_reference_t<const T>, etl::add_lvalue_reference_t<const U> > >;

  //***************************************************************************
  template <typename T>
  concept integral = etl::is_integral_v<T>;

  //***************************************************************************
  template <typename T>
  concept signed_integral = etl::integral<T> && etl::is_signed_v<T>;

  //***************************************************************************
  template <typename T>
  concept unsigned_integral = etl::integral<T> && !etl::signed_integral<T>;

  //***************************************************************************
  template <typename T>
  concept floating_point = etl::is_floating_point_v<T>;

  //***************************************************************************
  template <typename LHS, typename RHS>
  concept assignable_from =
    etl::is_lvalue_reference_v<LHS> && etl::common_reference_with< const etl::remove_reference_t<LHS>&, const etl::remove_reference_t<RHS>&>
    && requires(LHS lhs, RHS&& rhs) {
         { lhs = etl::forward<RHS>(rhs) } -> etl::same_as<LHS>;
       };

  //***************************************************************************
  template <typename F, typename... Args>
  concept invocable = etl::is_invocable_v<F, Args...>;

  //***************************************************************************
  template <typename F, typename... Args>
  concept regular_invocable = etl::invocable<F, Args...>;

  //***************************************************************************
  template <typename T>
  concept destructible = requires(T& t) {
    { t.~T() } noexcept;
  };

  //***************************************************************************
  template <typename T, typename... Args>
  concept constructible_from = etl::destructible<T> && etl::is_constructible_v<T, Args...>;

  //***************************************************************************
  template <typename T>
  concept default_initializable = etl::constructible_from<T> && requires {
    T{};
    ::new T;
  };

  //***************************************************************************
  template <typename T>
  concept move_constructible = etl::constructible_from<T, T> && etl::convertible_to<T, T>;

  //***************************************************************************
  template <typename T>
  concept copy_constructible =
    etl::move_constructible<T> && etl::constructible_from<T, T&> && etl::convertible_to<T&, T> && etl::constructible_from<T, const T&>
    && etl::convertible_to<const T&, T> && etl::constructible_from<T, const T> && etl::convertible_to<const T, T>;

  //***************************************************************************
  namespace private_concepts
  {
    template <typename T>
    concept boolean_testable = etl::convertible_to<T, bool> && requires(T&& t) {
      { !etl::forward<T>(t) } -> etl::convertible_to<bool>;
    };
  } // namespace private_concepts

  //***************************************************************************
  template <typename T>
  concept equality_comparable = requires(const etl::remove_reference_t<T>& a, const etl::remove_reference_t<T>& b) {
    { a == b } -> private_concepts::boolean_testable;
    { a != b } -> private_concepts::boolean_testable;
  };

  //***************************************************************************
  template <typename T>
  concept totally_ordered = etl::equality_comparable<T> && requires(const etl::remove_reference_t<T>& a, const etl::remove_reference_t<T>& b) {
    { a < b } -> private_concepts::boolean_testable;
    { a > b } -> private_concepts::boolean_testable;
    { a <= b } -> private_concepts::boolean_testable;
    { a >= b } -> private_concepts::boolean_testable;
  };

  //***************************************************************************
  template <typename T>
  concept swappable = requires(T& a, T& b) { etl::swap(a, b); };

  //***************************************************************************
  template <typename T, typename U>
  concept swappable_with = etl::common_reference_with<etl::remove_reference_t<T>&, etl::remove_reference_t<U>&> && requires(T&& t, U&& u) {
    etl::swap(etl::forward<T>(t), etl::forward<T>(t));
    etl::swap(etl::forward<U>(u), etl::forward<U>(u));
    etl::swap(etl::forward<T>(t), etl::forward<U>(u));
    etl::swap(etl::forward<U>(u), etl::forward<T>(t));
  };

  //***************************************************************************
  template <typename T>
  concept movable = etl::is_object_v<T> && etl::move_constructible<T> && etl::assignable_from<T&, T> && etl::swappable<T>;

  //***************************************************************************
  template <typename T>
  concept copyable = etl::copy_constructible<T> && etl::movable<T> && etl::assignable_from<T&, T&> && etl::assignable_from<T&, const T&>
                     && etl::assignable_from<T&, const T>;

  //***************************************************************************
  template <typename T>
  concept semiregular = etl::copyable<T> && etl::default_initializable<T>;

  //***************************************************************************
  template <typename T>
  concept regular = etl::semiregular<T> && etl::equality_comparable<T>;

  //***************************************************************************
  template <typename F, typename... Args>
  concept predicate = etl::regular_invocable<F, Args...> && private_concepts::boolean_testable<etl::invoke_result_t<F, Args...> >;

  //***************************************************************************
  template <typename R, typename T, typename U>
  concept relation = etl::predicate<R, T, T> && etl::predicate<R, U, U> && etl::predicate<R, T, U> && etl::predicate<R, U, T>;

  //***************************************************************************
  template <typename R, typename T, typename U>
  concept equivalence_relation = etl::relation<R, T, U>;

  //***************************************************************************
  template <typename R, typename T, typename U>
  concept strict_weak_order = etl::relation<R, T, U>;

  #endif
} // namespace etl
#endif
#endif
