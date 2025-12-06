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
FITNESS FOR TArgs PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
******************************************************************************/

#ifndef ETL_FUNCTION_TRAITS_INCLUDED
#define ETL_FUNCTION_TRAITS_INCLUDED

#include "platform.h"
#include "type_list.h"
#include "type_traits.h"

#if ETL_USING_CPP11

namespace etl
{
  //***************************************************************************
  // Primary template (unspecialized)
  //***************************************************************************
  template <typename T, typename Enable = void>
  struct function_traits;

  //***************************************************************************
  // Base for plain function type TReturn(TArgs...)
  //***************************************************************************
  template <typename TReturn, typename... TArgs>
  struct function_traits<TReturn(TArgs...), void>
  {
  public:

    using function_type  = TReturn(TArgs...);
    using return_type    = TReturn;
    using object_type    = void;
    using argument_types = etl::type_list<TArgs...>;

    static constexpr bool   is_function        = true;
    static constexpr bool   is_member_function = false;
    static constexpr bool   is_functor         = false;
    static constexpr bool   is_const           = false;
    static constexpr bool   is_volatile        = false;
    static constexpr bool   is_noexcept        = false;
    static constexpr size_t arity              = sizeof...(TArgs);
    
    ETL_DEPRECATED_REASON("Use etl::function_traits::arity instead")
    static constexpr size_t argument_count     = arity;
  };

  //***************************************************************************
  // Free function pointer
  //***************************************************************************
  template <typename TReturn, typename... TArgs>
  struct function_traits<TReturn(*)(TArgs...), void> : function_traits<TReturn(TArgs...)> {};

  //***************************************************************************
  // Free function reference
  //***************************************************************************
  template <typename TReturn, typename... TArgs>
  struct function_traits<TReturn(&)(TArgs...), void> : function_traits<TReturn(TArgs...)> {};

#if ETL_HAS_NOEXCEPT_FUNCTION_TYPE
  //***************************************************************************
  // Free noexcept function pointer
  //***************************************************************************
  template <typename TReturn, typename... TArgs>
  struct function_traits<TReturn(*)(TArgs...) noexcept, void> : function_traits<TReturn(TArgs...)>
  {
    static constexpr bool is_noexcept = true;
  };

  //***************************************************************************
  // Free noexcept function reference.
  //***************************************************************************
  template <typename TReturn, typename... TArgs>
  struct function_traits<TReturn(&)(TArgs...) noexcept, void> : function_traits<TReturn(TArgs...)>
  {
    static constexpr bool is_noexcept = true;
  };
#endif

  //***************************************************************************
  // Member function pointers
  //***************************************************************************
  template <typename TReturn, typename TObject, typename... TArgs>
  struct function_traits<TReturn (TObject::*)(TArgs...), void> : function_traits<TReturn(TArgs...)>
  {
    using object_type = TObject;
    static constexpr bool is_function        = false;
    static constexpr bool is_member_function = true;
  };

  //***************************************************************************
  // Const member function pointers
  //***************************************************************************
  template <typename TReturn, typename TObject, typename... TArgs>
  struct function_traits<TReturn (TObject::*)(TArgs...) const, void> : function_traits<TReturn(TObject::*)(TArgs...)>
  {
    static constexpr bool is_const = true;
  };

  //***************************************************************************
  // Volatile member function pointers
  //***************************************************************************
  template <typename TReturn, typename TObject, typename... TArgs>
  struct function_traits<TReturn (TObject::*)(TArgs...) volatile, void> : function_traits<TReturn(TObject::*)(TArgs...)>
  {
    static constexpr bool is_volatile = true;
  };

  //***************************************************************************
  // Const volatile member function pointers
  //***************************************************************************
  template <typename TReturn, typename TObject, typename... TArgs>
  struct function_traits<TReturn (TObject::*)(TArgs...) const volatile, void> : function_traits<TReturn(TObject::*)(TArgs...)>
  {
    static constexpr bool is_const    = true;
    static constexpr bool is_volatile = true;
  };

#if ETL_HAS_NOEXCEPT_FUNCTION_TYPE
  //***************************************************************************
  // Noexcept member function pointers
  //***************************************************************************
  template <typename TReturn, typename TObject, typename... TArgs>
  struct function_traits<TReturn (TObject::*)(TArgs...) noexcept, void> : function_traits<TReturn(TObject::*)(TArgs...)>
  {
    static constexpr bool is_noexcept = true;
  };

  //***************************************************************************
  // Const noexcept member function pointers
  //***************************************************************************
  template <typename TReturn, typename TObject, typename... TArgs>
  struct function_traits<TReturn (TObject::*)(TArgs...) const noexcept, void> : function_traits<TReturn(TObject::*)(TArgs...) const>
  {
    static constexpr bool is_noexcept = true;
  };

  //***************************************************************************
  // Volatile noexcept member function pointers
  //***************************************************************************
  template <typename TReturn, typename TObject, typename... TArgs>
  struct function_traits<TReturn (TObject::*)(TArgs...) volatile noexcept, void> : function_traits<TReturn(TObject::*)(TArgs...) volatile>
  {
    static constexpr bool is_noexcept = true;
  };

  //***************************************************************************
  // Const volatile noexcept member function pointers
  //***************************************************************************
  template <typename TReturn, typename TObject, typename... TArgs>
  struct function_traits<TReturn (TObject::*)(TArgs...) const volatile noexcept, void> : function_traits<TReturn(TObject::*)(TArgs...) const volatile>
  {
    static constexpr bool is_noexcept = true;
  };
#endif

  //***************************************************************************
  // Forward cv/ref on the whole type to the unqualified type.
  //***************************************************************************
  template <typename T>
  struct function_traits<T, etl::enable_if_t<!etl::is_same<T, etl::remove_cvref_t<T>>::value &&
                                             !etl::is_class<etl::decay_t<T>>::value>>
    : function_traits<etl::remove_cvref_t<T>>
  {
  };

  //***************************************************************************
  // Functors / lambdas: enable only for class types that have a unique operator()
  //***************************************************************************
  namespace private_function_traits
  {
    //*********************************
    // Helper to get pointer to call operator
    //*********************************
    template <typename U>
    using call_operator_ptr_t = decltype(&U::operator());
  }

  //***************************************************************************
  /// Functors / lambdas specialisation
  //***************************************************************************
  template <typename T>
  struct function_traits<T, etl::enable_if_t<etl::is_class<etl::decay_t<T>>::value&&
                            etl::has_unique_call_operator<T>::value>>
    : function_traits<private_function_traits::call_operator_ptr_t<etl::decay_t<T>> >
  {
    static constexpr bool is_functor = true;
  };

  //***************************************************************************
  // Out-of-class definitions for the function_traits static members
  //***************************************************************************
  // free/function primary template
  template <typename TReturn, typename... TArgs>
  constexpr bool function_traits<TReturn(TArgs...), void>::is_function;

  template <typename TReturn, typename... TArgs>
  constexpr bool function_traits<TReturn(TArgs...), void>::is_member_function;
  
  template <typename TReturn, typename... TArgs>
  constexpr bool function_traits<TReturn(TArgs...), void>::is_functor;
  
  template <typename TReturn, typename... TArgs>
  constexpr bool function_traits<TReturn(TArgs...), void>::is_const;
  
  template <typename TReturn, typename... TArgs>
  constexpr bool function_traits<TReturn(TArgs...), void>::is_volatile;
  
  template <typename TReturn, typename... TArgs>
  constexpr bool function_traits<TReturn(TArgs...), void>::is_noexcept;
  
  template <typename TReturn, typename... TArgs>
  constexpr size_t function_traits<TReturn(TArgs...), void>::arity;

  // member-function-pointer specialization
  template <typename TReturn, typename TObject, typename... TArgs>
  constexpr bool function_traits<TReturn (TObject::*)(TArgs...), void>::is_function;

  template <typename TReturn, typename TObject, typename... TArgs>
  constexpr bool function_traits<TReturn (TObject::*)(TArgs...), void>::is_member_function;

  // cv/ref-qualified member-function pointer flags
  template <typename TReturn, typename TObject, typename... TArgs>
  constexpr bool function_traits<TReturn (TObject::*)(TArgs...) const, void>::is_const;
  
  template <typename TReturn, typename TObject, typename... TArgs>
  constexpr bool function_traits<TReturn (TObject::*)(TArgs...) volatile, void>::is_volatile;
  
  template <typename TReturn, typename TObject, typename... TArgs>
  constexpr bool function_traits<TReturn (TObject::*)(TArgs...) const volatile, void>::is_const;
  
  template <typename TReturn, typename TObject, typename... TArgs>
  constexpr bool function_traits<TReturn (TObject::*)(TArgs...) const volatile, void>::is_volatile;

#if ETL_HAS_NOEXCEPT_FUNCTION_TYPE
  template <typename TReturn, typename... TArgs>
  constexpr bool function_traits<TReturn(*)(TArgs...) noexcept, void>::is_noexcept;

  template <typename TReturn, typename TObject, typename... TArgs>
  constexpr bool function_traits<TReturn (TObject::*)(TArgs...) noexcept, void>::is_noexcept;

  template <typename TReturn, typename TObject, typename... TArgs>
  constexpr bool function_traits<TReturn (TObject::*)(TArgs...) const noexcept, void>::is_noexcept;

  template <typename TReturn, typename TObject, typename... TArgs>
  constexpr bool function_traits<TReturn (TObject::*)(TArgs...) volatile noexcept, void>::is_noexcept;

  template <typename TReturn, typename TObject, typename... TArgs>
  constexpr bool function_traits<TReturn (TObject::*)(TArgs...) const volatile noexcept, void>::is_noexcept;
#endif

  //***************************************************************************
  // Functor / lambda specialisation: provide out-of-class definition for is_functor
  //***************************************************************************
  template <typename T>
  constexpr bool function_traits<T, etl::enable_if_t<etl::is_class<etl::decay_t<T>>::value &&
                                                     etl::has_unique_call_operator<T>::value>>::is_functor;
}

#endif
#endif