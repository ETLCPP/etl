///\file

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

#ifndef ETL_INVOKE_INCLUDED
#define ETL_INVOKE_INCLUDED

#include "platform.h"
#include "functional.h"
#include "function_traits.h"
#include "type_traits.h"
#include "utility.h"

namespace etl 
{
  template <typename T> struct logical_not_t : etl::integral_constant<bool, !bool(T::value)> {};

  template <
    typename F, 
    typename ... TArgs, 
    typename = typename etl::enable_if<
      !etl::is_member_pointer<etl::decay_t<F>>::value>::type
    >
  ETL_CONSTEXPR auto invoke(F&& f, TArgs&& ... args) 
    -> decltype(etl::forward<F>(f)(etl::forward<TArgs>(args)...)) {
    return etl::forward<F>(f)(etl::forward<TArgs>(args)...);
  }

  template <
    typename F, 
    typename T, 
    typename... TArgs,
    typename = typename etl::enable_if<
      etl::is_member_function_pointer<etl::decay_t<F>>::value && 
      !etl::is_pointer<etl::decay_t<T>>::value
    >::type
  >
  ETL_CONSTEXPR auto invoke(F&& f, T&& t, TArgs&&... args)
      -> decltype((etl::forward<T>(t).*f)(etl::forward<TArgs>(args)...))
  {
      return (etl::forward<T>(t).*f)(etl::forward<TArgs>(args)...);
  }

    template <
    typename F, 
    typename T, 
    typename ... TArgs,
    typename = typename etl::enable_if<
      etl::is_member_function_pointer<etl::decay_t<F>>::value && 
      etl::is_pointer<etl::decay_t<T>>::value
    >::type
  >
  ETL_CONSTEXPR auto invoke(F&& f, T&& t, TArgs&&... args)
      -> decltype(((*etl::forward<T>(t)).*f)(etl::forward<TArgs>(args)...))
  {
      return ((*etl::forward<T>(t)).*f)(etl::forward<TArgs>(args)...);
  }

  template <
    typename F, 
    typename T,
    typename = typename etl::enable_if<
      etl::is_member_object_pointer<etl::decay_t<F>>::value && 
      !etl::is_pointer<etl::decay_t<T>>::value
    >::type
  >
  ETL_CONSTEXPR auto invoke(F&& f, T&& t)
      -> decltype(etl::forward<T>(t).*f)
  {
      return etl::forward<T>(t).*f;
  }

  template <
    typename F,
    typename T,
    typename = typename etl::enable_if<
      etl::is_member_object_pointer<etl::decay_t<F>>::value &&
      etl::is_pointer<etl::decay_t<T>>::value
    >::type
  >
  ETL_CONSTEXPR auto invoke(F&& f, T&& t)
      -> decltype(((*etl::forward<T>(t)).*f))
  {
      return ((*etl::forward<T>(t)).*f);
  }

  template <class F, class, class ... Us> struct invoke_result;

  template <typename F, typename... Us>
  struct invoke_result<
      F,
      etl::void_t<decltype(etl::invoke(etl::declval<F>(), etl::declval<Us>()...))>,
      Us...
    > {
    using type = decltype(etl::invoke(etl::declval<F>(), etl::declval<Us>()...));
  };

  template <typename F, typename... Us>
  using invoke_result_t = typename invoke_result<F, void, Us...>::type;




  // ---------- invoke implementation (SFINAE-friendly) ----------
  //
  // Implement detection in terms of the public etl::invoke overload set above
  // so overload resolution picks the correct form (object.*pmf vs (ptr->*pmf), etc.)
  // and we don't instantiate an invalid alternative.

  // ---------- internal helpers ----------
  namespace private_invoke
  {
    // Test if etl::invoke(F, Args...) is well-formed
    template <typename F, typename... Args>
    struct is_invocable_expr
    {
      template <typename U>
      static auto test(int) -> decltype((void)etl::invoke(etl::declval<U>(), etl::declval<Args>()...), etl::true_type{});
      template <typename>
      static etl::false_type test(...);

      using type  = decltype(test<F>(0));
      static ETL_CONSTANT bool value = type::value;
    };

    // Get result type of etl::invoke(F, Args...) (only used after positive invocability test)
    template <typename F, typename... Args>
    struct invoke_result_impl
    {
      template <typename U>
      static auto test(int) -> decltype(etl::invoke(etl::declval<U>(), etl::declval<Args>()...));
      template <typename>
      static void test(...);

      using type = decltype(test<F>(0));
    };
  }

  // Redirect bare function types to function pointer (matches std::invoke behavior)
  template <typename F>
  using effective_callable_t =
    typename etl::conditional<etl::is_function<etl::remove_reference_t<F>>::value,
    typename etl::add_pointer<etl::remove_reference_t<F>>::type,
    F>::type;

  // Base is_invocable
  template <typename F, typename... Args>
  struct is_invocable
    : etl::bool_constant<
    private_invoke::is_invocable_expr<effective_callable_t<F>, Args...>::value
    >
  {};

  // is_invocable_r<R, F, Args...>
  // R == void : same as is_invocable
  // R != void : invocable AND result convertible to R
  template <typename R, typename F, typename... Args>
  struct is_invocable_r
    : etl::conditional_t<
    etl::is_same<R, void>::value,
    is_invocable<F, Args...>,
    etl::conditional_t<
    is_invocable<F, Args...>::value,
    etl::bool_constant<
    etl::is_convertible<
    typename private_invoke::invoke_result_impl<effective_callable_t<F>, Args...>::type,
    R
    >::value>,
    etl::false_type>>
  {};

  // invoke_result<R, F, Args...> (void wildcard like std)
  template <typename R, typename F, typename... Args>
  struct invoke_result
  {
  private:
    using FC = effective_callable_t<F>;
    static const bool ok = private_invoke::is_invocable_expr<FC, Args...>::value;

  public:
    using type = typename etl::conditional<
      ok,
      typename private_invoke::invoke_result_impl<FC, Args...>::type,
      void>::type;
  };

  // is_nothrow_invocable<F, Args...>
  template <typename F, typename... Args>
  struct is_nothrow_invocable
    : etl::bool_constant<
    is_invocable<F, Args...>::value &&
    noexcept(etl::invoke(etl::declval<effective_callable_t<F>>(), etl::declval<Args>()...))
    >
  {};

  // is_nothrow_invocable_r<R, F, Args...>
  template <typename R, typename F, typename... Args>
  struct is_nothrow_invocable_r
    : etl::bool_constant<
    is_invocable_r<R, F, Args...>::value &&
    (
      etl::is_same<R, void>::value ||
      noexcept(etl::invoke(etl::declval<effective_callable_t<F>>(), etl::declval<Args>()...))
      )
    >
  {};

#if ETL_USING_CPP17
  template <typename F, typename... Args>
  inline constexpr bool is_invocable_v = is_invocable<F, Args...>::value;
  template <typename R, typename F, typename... Args>
  inline constexpr bool is_invocable_r_v = is_invocable_r<R, F, Args...>::value;
  template <typename F, typename... Args>
  inline constexpr bool is_nothrow_invocable_v = is_nothrow_invocable<F, Args...>::value;
#endif

}
#endif
