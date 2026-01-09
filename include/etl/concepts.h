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

#include "utility.h"
#include "type_traits.h"

#if ETL_NOT_USING_CPP20 && !defined(ETL_IN_UNIT_TEST)
  #error NOT SUPPORTED FOR BELOW C++20
#endif

#if ETL_USING_CPP20

#if ETL_USING_STL
  #include <concepts>
#endif

namespace etl
{

#if ETL_USING_STL

  using std::same_as;
  using std::derived_from;
  using std::convertible_to;
  using std::common_reference_with;
  using std::common_with;
  using std::integral;
  using std::signed_integral;
  using std::unsigned_integral;
  using std::floating_point;
  using std::assignable_from;

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
  concept derived_from =
    etl::is_base_of_v<Base, Derived> &&
    etl::is_convertible_v<const volatile Derived*, const volatile Base*>;

  //***************************************************************************
  template <typename From, typename To>
  concept convertible_to =
    etl::is_convertible_v<From, To> &&
    requires {
      static_cast<To>(etl::declval<From>());
    };

  //***************************************************************************
  template< class T, typename U >
  concept common_reference_with =
    etl::same_as<etl::common_reference_t<T, U>, etl::common_reference_t<U, T>> &&
    etl::convertible_to<T, etl::common_reference_t<T, U>> &&
    etl::convertible_to<U, etl::common_reference_t<T, U>>;

  //***************************************************************************
  template <typename T, typename U>
  concept common_with =
    etl::same_as<etl::common_type_t<T, U>, etl::common_type_t<U, T>> &&
    requires {
        static_cast<etl::common_type_t<T, U>>(etl::declval<T>());
        static_cast<etl::common_type_t<T, U>>(etl::declval<U>());
    } &&
    etl::common_reference_with<
        etl::add_lvalue_reference_t<const T>,
        etl::add_lvalue_reference_t<const U>> &&
    etl::common_reference_with<
        etl::add_lvalue_reference_t<etl::common_type_t<T, U>>,
        etl::common_reference_t<
            etl::add_lvalue_reference_t<const T>,
            etl::add_lvalue_reference_t<const U>>>;

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
    etl::is_lvalue_reference_v<LHS> &&
    etl::common_reference_with<
        const etl::remove_reference_t<LHS>&,
        const etl::remove_reference_t<RHS>&> &&
    requires(LHS lhs, RHS&& rhs) {
        { lhs = etl::forward<RHS>(rhs) } -> etl::same_as<LHS>;
    };

#endif
}
#endif
#endif
