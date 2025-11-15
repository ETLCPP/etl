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
  // We implement a small set of overloads named invoke_impl that cover:
  //  1) general callable objects: f(args...)
  //  2) member function pointers: (obj.*pmf)(args...) or (obj->*pmf)(args...)
  //  3) member data pointers:  obj.*pmd or obj->*pmd
  //
  // etl::mem_fn is used for member pointers to handle object/pointer/ref_wrapper uniformly.

  namespace private_invoke {

    // primary general callable (call operator, function pointer, lambda, etc.)
    template <typename F, typename... Args>
    auto invoke_impl(F &&f, Args &&... args)
      -> decltype(etl::forward<F>(f)(etl::forward<Args>(args)...));

    // member function pointer (any cv / ref / noexcept permutation) handled via etl::mem_fn
    template <typename MF, typename Obj, typename... Args>
    auto invoke_impl(MF mf, Obj &&obj, Args &&... args)
      -> typename etl::enable_if<
      etl::is_member_function_pointer<typename etl::remove_reference<MF>::type>::value,
      decltype(etl::mem_fn(mf)(etl::forward<Obj>(obj), etl::forward<Args>(args)...))
      >::type;

    // member object pointer (data member)
    template <typename MD, typename Obj>
    auto invoke_impl(MD md, Obj &&obj)
      -> typename etl::enable_if<
      etl::is_member_object_pointer<typename etl::remove_reference<MD>::type>::value,
      decltype(etl::mem_fn(md)(etl::forward<Obj>(obj)))
      >::type;

    // Helper to detect if invoke_impl(F, Args...) is a valid expression.
    template <typename F, typename... Args>
    struct is_invocable_expr 
    {
    private:
      
      template <typename U>
      static auto test(int) -> decltype( (void) invoke_impl(etl::declval<U>(), etl::declval<Args>()...), etl::true_type{} );

      template <typename>
      static etl::false_type test(...);

    public:

      typedef decltype(test<F>(0)) type;
      static constexpr bool value = type::value;
    };

    // Helper to extract the result type of the invocation when it is valid.
    template <typename F, typename... Args>
    struct invoke_result_impl 
    {
    private:

      template <typename U>
      static auto test(int) -> decltype( invoke_impl(etl::declval<U>(), etl::declval<Args>()...) );

      template <typename>
      static void test(...);

    public:

      typedef decltype(test<F>(0)) type;
    };

  } // namespace private_invoke

    // ---------- public traits ----------

    // is_invocable<F, Args...>
  template <typename F, typename... Args>
  struct is_invocable : etl::integral_constant<bool, private_invoke::is_invocable_expr<F, Args...>::value> {};

  // is_invocable_r<R, F, Args...> — true if callable and result is convertible to R
  template <typename R, typename F, typename... Args>
  struct is_invocable_r 
  {
  private:
  
    static constexpr bool inv = private_invoke::is_invocable_expr<F, Args...>::value;
    using result_t = typename private_invoke::invoke_result_impl<F, Args...>::type;
  
  public:
  
    static constexpr bool value = inv && etl::is_convertible<result_t, R>::value;
  };

  template <typename R, typename F, typename... Args>
  struct invoke_result 
  {
    // type is void if not invocable
  private:
  
    static constexpr bool inv = private_invoke::is_invocable_expr<F, Args...>::value;
  
  public:
  
    typedef typename etl::conditional<inv, typename private_invoke::invoke_result_impl<F, Args...>::type, void>::type type;
  };

  // is_nothrow_invocable<F, Args...>
  template <typename F, typename... Args>
  struct is_nothrow_invocable 
  {
  private:
    
    template <typename U>
    static auto test(int) -> etl::integral_constant<bool, noexcept( private_invoke::invoke_impl(etl::declval<U>(), etl::declval<Args>()...) )>;

    template <typename>
    static etl::false_type test(...);

  public:

    typedef decltype(test<F>(0)) type;
    static constexpr bool value = type::value;
  };

  // is_nothrow_invocable_r<R, F, Args...>
  template <typename R, typename F, typename... Args>
  struct is_nothrow_invocable_r 
  {
  private:
  
    static constexpr bool inv = private_invoke::is_invocable_expr<F, Args...>::value;
    typedef typename private_invoke::invoke_result_impl<F, Args...>::type result_t;
    static constexpr bool conv = etl::is_convertible<result_t, R>::value;
  
  public:
  
    static constexpr bool value = inv && conv && noexcept( etl::declval<result_t>() );
  };

  //
  // Variable templates and _v/_t shorthands when the language supports them
  //
#if ETL_USING_CPP17
  template <typename F, typename... Args>
  constexpr bool is_invocable_v = is_invocable<F, Args...>::value;

  template <typename R, typename F, typename... Args>
  constexpr bool is_invocable_r_v = is_invocable_r<R, F, Args...>::value;

  template <typename F, typename... Args>
  constexpr bool is_nothrow_invocable_v = is_nothrow_invocable<F, Args...>::value;
#endif

}
#endif
