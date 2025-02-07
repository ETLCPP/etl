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

#ifndef ETL_FUNCTION_TRAITS_INCLUDED
#define ETL_FUNCTION_TRAITS_INCLUDED

#include "platform.h"
#include "type_list.h"
#include "type_traits.h"

#if ETL_USING_CPP11
namespace etl
{
  //***************************************************************************
  /// A template to extract the function type traits.
  //***************************************************************************
  template <typename T>
  struct function_traits;

  //***************************************************************************
  /// Specialisation for function pointers
  //***************************************************************************
  template <typename TReturn, typename... TArgs>
  struct function_traits<TReturn(*)(TArgs...)>
  {
    using function_type  = TReturn(TArgs...);                    ///< The signature of the function.
    using return_type    = TReturn;                              ///< The return type.
    using object_type    = void;                                 ///< The object type, if a member function.
    using argument_types = etl::type_list<TArgs...>;             ///< An etl::type_list containing the function argument types.

    static constexpr bool is_function        = true;             ///< <b>true</b> if the type is a free, static or global function, otherwise <b>false</b>.
    static constexpr bool is_member_function = false;            ///< <b>true</b> if the type is a member function, otherwise <b>false</b>.
    static constexpr bool is_const           = false;            ///< <b>true</b> if the type is a const member function, otherwise <b>false</b>.
    static constexpr size_t argument_count   = sizeof...(TArgs); ///< The number of arguments that the function takes.
  };

  template <typename TReturn, typename... TArgs>
  constexpr bool function_traits<TReturn(*)(TArgs...)>::is_function;

  template <typename TReturn, typename... TArgs>
  constexpr bool function_traits<TReturn(*)(TArgs...)>::is_member_function;

  template <typename TReturn, typename... TArgs>
  constexpr bool function_traits<TReturn(*)(TArgs...)>::is_const;

  template <typename TReturn, typename... TArgs>
  constexpr size_t function_traits<TReturn(*)(TArgs...)>::argument_count;

  //***************************************************************************
  /// Specialisation for member function pointers
  //***************************************************************************
  template <typename TReturn, typename TObject, typename... TArgs>
  struct function_traits<TReturn(TObject::*)(TArgs...)>
  {
    using function_type  = TReturn(TArgs...);                    ///< The signature of the function.
    using return_type    = TReturn;                              ///< The return type.
    using object_type    = TObject;                              ///< The object type, if a member function.
    using argument_types = etl::type_list<TArgs...>;             ///< An etl::type_list containing the function argument types.

    static constexpr bool is_function        = false;            ///< <b>true</b> if the type is a free, static or global function, otherwise <b>false</b>.
    static constexpr bool is_member_function = true;             ///< <b>true</b> if the type is a member function, otherwise <b>false</b>.
    static constexpr bool is_const           = false;            ///< <b>true</b> if the type is a const member function, otherwise <b>false</b>.
    static constexpr size_t argument_count   = sizeof...(TArgs); ///< The number of arguments that the function takes.
  };

  template <typename TReturn, typename TObject, typename... TArgs>
  constexpr bool function_traits<TReturn(TObject::*)(TArgs...)>::is_function;

  template <typename TReturn, typename TObject, typename... TArgs>
  constexpr bool function_traits<TReturn(TObject::*)(TArgs...)>::is_member_function;

  template <typename TReturn, typename TObject, typename... TArgs>
  constexpr bool function_traits<TReturn(TObject::*)(TArgs...)>::is_const;

  template <typename TReturn, typename TObject, typename... TArgs>
  constexpr size_t function_traits<TReturn(TObject::*)(TArgs...)>::argument_count;

  //***************************************************************************
  /// Specialisation for const member function pointers
  //***************************************************************************
  template <typename TReturn, typename TObject, typename... TArgs>
  struct function_traits<TReturn(TObject::*)(TArgs...) const>
  {
    using function_type  = TReturn(TArgs...);                    ///< The signature of the function.
    using return_type    = TReturn;                              ///< The return type.
    using object_type    = TObject;                              ///< The object type, if a member function.
    using argument_types = etl::type_list<TArgs...>;             ///< An etl::type_list containing the function argument types.

    static constexpr bool is_function        = false;            ///< <b>true</b> if the type is a free, static or global function, otherwise <b>false</b>.
    static constexpr bool is_member_function = true;             ///< <b>true</b> if the type is a member function, otherwise <b>false</b>.
    static constexpr bool is_const           = true;             ///< <b>true</b> if the type is a const member function, otherwise <b>false</b>.
    static constexpr size_t argument_count   = sizeof...(TArgs); ///< The number of arguments that the function takes.
  };

  template <typename TReturn, typename TObject, typename... TArgs>
  constexpr bool function_traits<TReturn(TObject::*)(TArgs...) const>::is_function;

  template <typename TReturn, typename TObject, typename... TArgs>
  constexpr bool function_traits<TReturn(TObject::*)(TArgs...) const>::is_member_function;

  template <typename TReturn, typename TObject, typename... TArgs>
  constexpr bool function_traits<TReturn(TObject::*)(TArgs...) const>::is_const;

  template <typename TReturn, typename TObject, typename... TArgs>
  constexpr size_t function_traits<TReturn(TObject::*)(TArgs...) const>::argument_count;
}
#endif

#endif
