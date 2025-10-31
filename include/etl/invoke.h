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

namespace etl {
  template <typename T> struct logical_not_t : etl::integral_constant<bool, !bool(T::value)> {};

  /// is T a function -- a function cannot be const qualified like a variable
  template<typename T> struct is_function : public etl::integral_constant<bool, !etl::is_const<const T>::value> { };
  template<typename T> struct is_function<T&> : public etl::false_type { };
  template<typename T> struct is_function<T&&> : public etl::false_type { };

  /// is T a member pointer
  template<typename  T> struct is_member_pointer_helper : etl::false_type {};
  template<typename T, typename C> struct is_member_pointer_helper<T C::*> : etl::true_type {};
  template<typename T> struct is_member_pointer : is_member_pointer_helper<etl::remove_cv_t<T>> {};

  /// is T a member function pointer
  template <typename> struct is_member_function_pointer_helper : etl::false_type {};
  template <typename T, typename C> struct is_member_function_pointer_helper<T C::*> : public etl::is_function<T>::type {};
  template <typename T> struct is_member_function_pointer : public is_member_function_pointer_helper<etl::remove_cv_t<T>>::type {};

  /// is T a member object pointer
  template<typename> struct is_member_object_pointer_helper : public etl::false_type { };
  template<typename T, typename C> struct is_member_object_pointer_helper<T C::*> : public logical_not_t<etl::is_function<T>>::type { };
  template<typename T> struct is_member_object_pointer : public is_member_object_pointer_helper<etl::remove_cv_t<T>>::type {};

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
  
}

#endif
