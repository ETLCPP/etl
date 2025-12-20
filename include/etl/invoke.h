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

#if ETL_USING_CPP11

namespace etl 
{
  //****************************************************************************
  // invoke implementation
  //****************************************************************************

  //****************************************************************************
  /// Pointer to member function + reference_wrapper
  template <typename TFunction, 
            typename TRefWrapper, 
            typename... TArgs,
            typename = etl::enable_if_t<etl::is_member_function_pointer<etl::decay_t<TFunction>>::value &&
                                        etl::is_reference_wrapper<etl::decay_t<TRefWrapper>>::value>>
  ETL_CONSTEXPR auto invoke(TFunction&& f, TRefWrapper&& ref_wrapper, TArgs&&... args)
    -> decltype((ref_wrapper.get().*f)(etl::forward<TArgs>(args)...))
  {
    return (ref_wrapper.get().*f)(etl::forward<TArgs>(args)...);
  }

  //****************************************************************************
  /// Pointer to member function + pointer to object
  template <typename TFunction, 
            typename TPtr, 
            typename... TArgs,
            typename = etl::enable_if_t<etl::is_member_function_pointer<etl::decay_t<TFunction>>::value &&
                                        etl::is_pointer<etl::decay_t<TPtr>>::value>>
  ETL_CONSTEXPR auto invoke(TFunction&& f, TPtr&& ptr, TArgs&&... args)
    -> decltype(((*etl::forward<TPtr>(ptr)).*f)(etl::forward<TArgs>(args)...))
  {
    return ((*etl::forward<TPtr>(ptr)).*f)(etl::forward<TArgs>(args)...);
  }

  //****************************************************************************
  /// Pointer to member function + object (or derived) reference
  template <typename TFunction, 
            typename TObject,
            typename... TArgs,
            typename = etl::enable_if_t<etl::is_member_function_pointer<etl::decay_t<TFunction>>::value &&
                                        !etl::is_pointer<etl::decay_t<TObject>>::value &&
                                        !is_reference_wrapper<etl::decay_t<TObject>>::value>>
  ETL_CONSTEXPR auto invoke(TFunction&& f, TObject&& obj, TArgs&&... args)
    -> decltype((etl::forward<TObject>(obj).*f)(etl::forward<TArgs>(args)...))
  {
    return (etl::forward<TObject>(obj).*f)(etl::forward<TArgs>(args)...);
  }

  //****************************************************************************
  /// Pointer to member object + reference_wrapper
  template <typename TFunction, 
            typename TRefWrapper,
            typename = etl::enable_if_t<etl::is_member_object_pointer<etl::decay_t<TFunction>>::value &&
                                        etl::is_reference_wrapper<etl::decay_t<TRefWrapper>>::value>>
  ETL_CONSTEXPR auto invoke(TFunction&& f, TRefWrapper&& ref_wrapper)
    -> decltype(ref_wrapper.get().*f)
  {
    return ref_wrapper.get().*f;
  }

  //****************************************************************************
  /// Pointer to member object + pointer to object
  template <typename TFunction, typename TPtr,
            typename = etl::enable_if_t<etl::is_member_object_pointer<etl::decay_t<TFunction>>::value &&
                                        etl::is_pointer<etl::decay_t<TPtr>>::value>>
  ETL_CONSTEXPR auto invoke(TFunction&& f, TPtr&& ptr)
    -> decltype(((*etl::forward<TPtr>(ptr)).*f))
  {
    return ((*etl::forward<TPtr>(ptr)).*f);
  }

  //****************************************************************************
  /// Pointer to member object + object (or derived) reference
  template <typename TFunction, 
            typename TObject,
            typename = etl::enable_if_t<etl::is_member_object_pointer<etl::decay_t<TFunction>>::value &&
                                        !etl::is_pointer<etl::decay_t<TObject>>::value &&
                                        !is_reference_wrapper<etl::decay_t<TObject>>::value>>
  ETL_CONSTEXPR auto invoke(TFunction&& f, TObject&& obj)
    -> decltype(etl::forward<TObject>(obj).*f)
  {
    return etl::forward<TObject>(obj).*f;
  }

  //****************************************************************************
  /// General callable (function object / lambda / function pointer)
  template <typename TFunction, 
            typename... TArgs,
            typename = etl::enable_if_t<!etl::is_member_pointer<etl::decay_t<TFunction>>::value>>
  ETL_CONSTEXPR auto invoke(TFunction&& f, TArgs&&... args)
    -> decltype(etl::forward<TFunction>(f)(etl::forward<TArgs>(args)...))
  {
    return etl::forward<TFunction>(f)(etl::forward<TArgs>(args)...);
  }

  //****************************************************************************
  // is_invocable implementation
  //****************************************************************************
  namespace private_invoke
  {
    //*******************************************
    // Core detection of invocability.
    // Succeeds if the invocation expression is well-formed.
    template <typename TFunction, typename... TArgs>
    struct is_invocable_expr
    {
      template <typename U>
      static auto test(int) -> decltype((void)etl::invoke(etl::declval<U>(), etl::declval<TArgs>()...), etl::true_type{});
      
      template <typename>
      static etl::false_type test(...);

      using type = decltype(test<TFunction>(0));

      static ETL_CONSTANT bool value = type::value;
    };

    //*******************************************
    // Core detection of invocability.
    // Succeeds if the invocation expression is well-formed.
    // Using etl::type_list for the argument list.
    template <typename TFunction, typename... TArgs>
    struct is_invocable_expr<TFunction, etl::type_list<TArgs...>>
    {
      template <typename U>
      static auto test(int) -> decltype((void)etl::invoke(etl::declval<U>(), etl::declval<TArgs>()...), etl::true_type{});

      template <typename>
      static etl::false_type test(...);

      using type = decltype(test<TFunction>(0));

      static ETL_CONSTANT bool value = type::value;
    };

    //*******************************************
    // Result type of a valid invocation.
    template <typename TFunction, typename... TArgs>
    struct invoke_result_impl
    {
      template <typename U>
      static auto test(int) -> decltype(etl::invoke(etl::declval<U>(), etl::declval<TArgs>()...));
      
      template <typename>
      static void test(...);

      using type = decltype(test<TFunction>(0));
    };

    //*******************************************
    // Result type of a valid invocation.
    template <typename TFunction, typename... TArgs>
    struct invoke_result_impl<TFunction, etl::type_list<TArgs...>>
    {
      template <typename U>
      static auto test(int) -> decltype(etl::invoke(etl::declval<U>(), etl::declval<TArgs>()...));

      template <typename>
      static void test(...);

      using type = decltype(test<TFunction>(0));
    };

    template <typename TFunction, typename... TArgs>
    using invoke_result_impl_t = typename invoke_result_impl<TFunction, TArgs...>::type;

    //*******************************************
    // Map raw function type to pointer.
    template <typename TFunction>
    using effective_callable_t = etl::conditional_t<etl::is_function<etl::remove_reference_t<TFunction>>::value,
                                                    etl::add_pointer_t<etl::remove_reference_t<TFunction>>,
                                                    TFunction>;
  }

  //****************************************************************************
  /// invoke_result<TFunction, TArgs...>
  template <typename TFunction, typename, typename... TArgs> 
  struct invoke_result
  {
    using type = void;
  };

  //*******************************************
  template <typename TFunction, typename... TArgs>
  struct invoke_result<TFunction,
                       etl::void_t<decltype(etl::invoke(etl::declval<TFunction>(), etl::declval<TArgs>()...))>,
                       TArgs...> 
  {
  private:

    using FC = private_invoke::effective_callable_t<TFunction>;
  
  public:
  
    using type = etl::conditional_t<private_invoke::is_invocable_expr<FC, TArgs...>::value,
                                    private_invoke::invoke_result_impl_t<FC, TArgs...>,
                                    void>;
  };

  //****************************************************************************
  /// invoke_result<TFunction, etl::type_list<TArgs...>>
  template <typename TFunction, typename... TArgs>
  struct invoke_result<TFunction, etl::type_list<TArgs...>>
  {
  private:

    using FC = private_invoke::effective_callable_t<TFunction>;

  public:

    using type = etl::conditional_t<private_invoke::is_invocable_expr<FC, TArgs...>::value,
      private_invoke::invoke_result_impl_t<FC, TArgs...>,
      void>;
  };

  //*******************************************
  // Specialization to allow `etl::type_list<...>` as the second template parameter.
  template <typename TFunction, typename... TArgs>
  struct invoke_result<TFunction,
                       etl::void_t<decltype(etl::invoke(etl::declval<TFunction>(), etl::declval<TArgs>()...))>,
                       etl::type_list<TArgs...>> 
  {
    using type = decltype(etl::invoke(etl::declval<TFunction>(), etl::declval<TArgs>()...));
  };

  //*******************************************
  template <typename TFunction, typename... TArgs>
  using invoke_result_t = typename invoke_result<TFunction, void, TArgs...>::type;

  //****************************************************************************
  /// is_invocable<TFunction, TArgs...>
  template <typename TFunction, typename... TArgs>
  struct is_invocable
    : etl::bool_constant<private_invoke::is_invocable_expr<private_invoke::effective_callable_t<TFunction>, TArgs...>::value>
  {
  };

  //****************************************************************************
  // Specialization to allow `etl::type_list<...>` as the second template parameter.
  template <typename TFunction, typename... TArgs>
  struct is_invocable<TFunction, etl::type_list<TArgs...>>
    : is_invocable<TFunction, TArgs...>
  {
  };

  //****************************************************************************
  // is_invocable_r<TReturn, TFunction, TArgs...>
  template <typename TReturn, typename TFunction, typename... TArgs>
  struct is_invocable_r
    : etl::conditional_t<etl::is_same<TReturn, void>::value,
                         etl::is_invocable<TFunction, TArgs...>,
                         etl::conditional_t<is_invocable<TFunction, TArgs...>::value,
                                            etl::bool_constant<etl::is_convertible<invoke_result_t<TFunction, TArgs...>, TReturn>::value>,
                                            etl::false_type>>
  {
  };

  //****************************************************************************
  // Specialization to allow `etl::type_list<...>` as the second template parameter.
  template <typename TReturn, typename TFunction, typename... TArgs>
  struct is_invocable_r<TReturn, TFunction, etl::type_list<TArgs...>>
    : is_invocable_r<TReturn, TFunction, TArgs...>
  {
  };

  //****************************************************************************
  // Specialization to allow `etl::type_list<...>` when there is a preceding object argument.
  template <typename TFunction, typename TObject, typename... TArgs>
  struct is_invocable<TFunction, TObject, etl::type_list<TArgs...>>
    : is_invocable<TFunction, TObject, TArgs...>
  {
  };

  //****************************************************************************
  // Specialization for is_invocable_r with a preceding object argument.
  template <typename TReturn, typename TFunction, typename TObject, typename... TArgs>
  struct is_invocable_r<TReturn, TFunction, TObject, etl::type_list<TArgs...>>
    : is_invocable_r<TReturn, TFunction, TObject, TArgs...>
  {
  };

#if ETL_USING_CPP17
  //****************************************************************************
  /// is_nothrow_invocable<TFunction, TArgs...>
  template <typename TFunction, typename... TArgs>
  struct is_nothrow_invocable
    : etl::bool_constant<etl::is_invocable<TFunction, TArgs...>::value &&
                         etl::function_traits<private_invoke::effective_callable_t<TFunction>>::is_noexcept>
  {
  };

  //****************************************************************************
  /// is_nothrow_invocable_r<TReturn, TFunction, TArgs...>
  template <typename TReturn, typename TFunction, typename... TArgs>
  struct is_nothrow_invocable_r 
    : etl::bool_constant<etl::is_invocable_r<TReturn, TFunction, TArgs...>::value &&
                         etl::function_traits<private_invoke::effective_callable_t<TFunction>>::is_noexcept &&
                         (etl::is_same<TReturn, void>::value ||
                          etl::is_nothrow_convertible<invoke_result_t<TFunction, TArgs...>, TReturn>::value)>
  {
  };

  //****************************************************************************
  /// is_nothrow_invocable_r<TReturn, TFunction, etl::type_list<TArgs...>>
  template <typename TReturn, typename TFunction, typename... TArgs>
  struct is_nothrow_invocable_r<TReturn, TFunction, etl::type_list<TArgs...>>
    : etl::bool_constant<etl::is_invocable_r<TReturn, TFunction, TArgs...>::value &&
                         etl::function_traits<private_invoke::effective_callable_t<TFunction>>::is_noexcept &&
                         (etl::is_same<TReturn, void>::value ||
                          etl::is_nothrow_convertible<invoke_result_t<TFunction, TArgs...>, TReturn>::value)>
  {
  };

  //****************************************************************************
  // Specialization to allow `etl::type_list<...>` when there is a preceding object argument
  // for the nothrow-with-return trait.
  template <typename TReturn, typename TFunction, typename TObject, typename... TArgs>
  struct is_nothrow_invocable_r<TReturn, TFunction, TObject, etl::type_list<TArgs...>>
    : is_nothrow_invocable_r<TReturn, TFunction, TObject, TArgs...>
  {};
#endif

#if ETL_USING_CPP17
  template <typename TFunction, typename... TArgs>
  inline constexpr bool is_invocable_v = is_invocable<TFunction, TArgs...>::value;

  template <typename TReturn, typename TFunction, typename... TArgs>
  inline constexpr bool is_invocable_r_v = is_invocable_r<TReturn, TFunction, TArgs...>::value;
  
  template <typename TFunction, typename... TArgs>
  inline constexpr bool is_nothrow_invocable_v = is_nothrow_invocable<TFunction, TArgs...>::value;

  template <typename TFunction, typename... TArgs>
  inline constexpr bool is_nothrow_invocable_r_v = is_nothrow_invocable_r<TFunction, TArgs...>::value;
#endif
}

#endif  // ETL_USING_CPP11
#endif  // ETL_INVOKE_INCLUDED
