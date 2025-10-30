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
    using function_type  = TReturn(TArgs...);                      ///< The signature of the function.
    using return_type    = TReturn;                                ///< The return type.
    using object_type    = void;                                   ///< The object type, if a member function.
    using argument_types = etl::type_list<TArgs...>;               ///< An etl::type_list containing the function argument types.

    static constexpr bool   is_function        = true;             ///< <b>true</b> if the type is a free, static or global function, otherwise <b>false</b>.
    static constexpr bool   is_member_function = false;            ///< <b>true</b> if the type is a member function, otherwise <b>false</b>.
    static constexpr bool   is_volatile        = false;            ///< <b>true</b> if the type is a volatile member function, otherwise <b>false</b>.
    static constexpr bool   is_const           = false;            ///< <b>true</b> if the type is a const member function, otherwise <b>false</b>.
    static constexpr bool   is_noexcept        = false;            ///< <b>true</b> if the type is a noexcept, otherwise <b>false</b>.
    static constexpr size_t argument_count     = sizeof...(TArgs); ///< The number of arguments that the function takes.
  };

#if ETL_USING_CPP17
  template <typename TReturn, typename... TArgs>
  constexpr bool function_traits<TReturn(*)(TArgs...)>::is_function;

  template <typename TReturn, typename... TArgs>
  constexpr bool function_traits<TReturn(*)(TArgs...)>::is_member_function;

  template <typename TReturn, typename... TArgs>
  constexpr bool function_traits<TReturn(*)(TArgs...)>::is_const;

  template <typename TReturn, typename... TArgs>
  constexpr bool function_traits<TReturn(*)(TArgs...)>::is_volatile;

  template <typename TReturn, typename... TArgs>
  constexpr bool function_traits<TReturn(*)(TArgs...)>::is_noexcept;

  template <typename TReturn, typename... TArgs>
  constexpr size_t function_traits<TReturn(*)(TArgs...)>::argument_count;
#endif

  //***************************************************************************
  /// Specialisation for function references
  //***************************************************************************
  template <typename TReturn, typename... TArgs>
  struct function_traits<TReturn(&)(TArgs...)>
  {
    using function_type  = TReturn(TArgs...);                      ///< The signature of the function.
    using return_type    = TReturn;                                ///< The return type.
    using object_type    = void;                                   ///< The object type, if a member function.
    using argument_types = etl::type_list<TArgs...>;               ///< An etl::type_list containing the function argument types.

    static constexpr bool   is_function        = true;             ///< <b>true</b> if the type is a free, static or global function, otherwise <b>false</b>.
    static constexpr bool   is_member_function = false;            ///< <b>true</b> if the type is a member function, otherwise <b>false</b>.
    static constexpr bool   is_const           = false;            ///< <b>true</b> if the type is a const member function, otherwise <b>false</b>.
    static constexpr bool   is_volatile        = false;            ///< <b>true</b> if the type is a volatile member function, otherwise <b>false</b>.
    static constexpr bool   is_noexcept        = false;            ///< <b>true</b> if the type is a noexcept, otherwise <b>false</b>.
    static constexpr size_t argument_count     = sizeof...(TArgs); ///< The number of arguments that the function takes.
  };

#if ETL_USING_CPP17
  template <typename TReturn, typename... TArgs>
  constexpr bool function_traits<TReturn(&)(TArgs...)>::is_function;

  template <typename TReturn, typename... TArgs>
  constexpr bool function_traits<TReturn(&)(TArgs...)>::is_member_function;

  template <typename TReturn, typename... TArgs>
  constexpr bool function_traits<TReturn(&)(TArgs...)>::is_const;

  template <typename TReturn, typename... TArgs>
  constexpr bool function_traits<TReturn(&)(TArgs...)>::is_volatile;

  template <typename TReturn, typename... TArgs>
  constexpr bool function_traits<TReturn(&)(TArgs...)>::is_noexcept;

  template <typename TReturn, typename... TArgs>
  constexpr size_t function_traits<TReturn(&)(TArgs...)>::argument_count;
#endif

  //***************************************************************************
  /// Specialisation for member function pointers
  //***************************************************************************
  template <typename TReturn, typename TObject, typename... TArgs>
  struct function_traits<TReturn(TObject::*)(TArgs...)>
  {
    using function_type  = TReturn(TArgs...);                      ///< The signature of the function.
    using return_type    = TReturn;                                ///< The return type.
    using object_type    = TObject;                                ///< The object type, if a member function.
    using argument_types = etl::type_list<TArgs...>;               ///< An etl::type_list containing the function argument types.

    static constexpr bool   is_function        = false;            ///< <b>true</b> if the type is a free, static or global function, otherwise <b>false</b>.
    static constexpr bool   is_member_function = true;             ///< <b>true</b> if the type is a member function, otherwise <b>false</b>.
    static constexpr bool   is_const           = false;            ///< <b>true</b> if the type is a const member function, otherwise <b>false</b>.
    static constexpr bool   is_volatile        = false;            ///< <b>true</b> if the type is a volatile member function, otherwise <b>false</b>.
    static constexpr bool   is_noexcept        = false;            ///< <b>true</b> if the type is a noexcept, otherwise <b>false</b>.
    static constexpr size_t argument_count     = sizeof...(TArgs); ///< The number of arguments that the function takes.
  };

#if ETL_USING_CPP17
  template <typename TReturn, typename TObject, typename... TArgs>
  constexpr bool function_traits<TReturn(TObject::*)(TArgs...)>::is_function;

  template <typename TReturn, typename TObject, typename... TArgs>
  constexpr bool function_traits<TReturn(TObject::*)(TArgs...)>::is_member_function;

  template <typename TReturn, typename TObject, typename... TArgs>
  constexpr bool function_traits<TReturn(TObject::*)(TArgs...)>::is_const;

  template <typename TReturn, typename TObject, typename... TArgs>
  constexpr bool function_traits<TReturn(TObject::*)(TArgs...)>::is_volatile;

  template <typename TReturn, typename TObject, typename... TArgs>
  constexpr bool function_traits<TReturn(TObject::*)(TArgs...)>::is_noexcept;

  template <typename TReturn, typename TObject, typename... TArgs>
  constexpr size_t function_traits<TReturn(TObject::*)(TArgs...)>::argument_count;
#endif

  //***************************************************************************
  /// Specialisation for const member function pointers
  //***************************************************************************
  template <typename TReturn, typename TObject, typename... TArgs>
  struct function_traits<TReturn(TObject::*)(TArgs...) const>
  {
    using function_type  = TReturn(TArgs...);                      ///< The signature of the function.
    using return_type    = TReturn;                                ///< The return type.
    using object_type    = TObject;                                ///< The object type, if a member function.
    using argument_types = etl::type_list<TArgs...>;               ///< An etl::type_list containing the function argument types.

    static constexpr bool   is_function        = false;            ///< <b>true</b> if the type is a free, static or global function, otherwise <b>false</b>.
    static constexpr bool   is_member_function = true;             ///< <b>true</b> if the type is a member function, otherwise <b>false</b>.
    static constexpr bool   is_const           = true;             ///< <b>true</b> if the type is a const member function, otherwise <b>false</b>.
    static constexpr bool   is_volatile        = false;            ///< <b>true</b> if the type is a volatile member function, otherwise <b>false</b>.
    static constexpr bool   is_noexcept        = false;            ///< <b>true</b> if the type is a noexcept, otherwise <b>false</b>.
    static constexpr size_t argument_count     = sizeof...(TArgs); ///< The number of arguments that the function takes.
  };

#if ETL_USING_CPP17
  template <typename TReturn, typename TObject, typename... TArgs>
  constexpr bool function_traits<TReturn(TObject::*)(TArgs...) const>::is_function;

  template <typename TReturn, typename TObject, typename... TArgs>
  constexpr bool function_traits<TReturn(TObject::*)(TArgs...) const>::is_member_function;

  template <typename TReturn, typename TObject, typename... TArgs>
  constexpr bool function_traits<TReturn(TObject::*)(TArgs...) const>::is_const;

  template <typename TReturn, typename TObject, typename... TArgs>
  constexpr bool function_traits<TReturn(TObject::*)(TArgs...) const>::is_volatile;

  template <typename TReturn, typename TObject, typename... TArgs>
  constexpr bool function_traits<TReturn(TObject::*)(TArgs...) const>::is_noexcept;

  template <typename TReturn, typename TObject, typename... TArgs>
  constexpr size_t function_traits<TReturn(TObject::*)(TArgs...) const>::argument_count;
#endif

  //***************************************************************************
  /// Specialisation for volatile member function pointers
  //***************************************************************************
  template <typename TReturn, typename TObject, typename... TArgs>
  struct function_traits<TReturn(TObject::*)(TArgs...) volatile>
  {
    using function_type  = TReturn(TArgs...);                      ///< The signature of the function.
    using return_type    = TReturn;                                ///< The return type.
    using object_type    = TObject;                                ///< The object type, if a member function.
    using argument_types = etl::type_list<TArgs...>;               ///< An etl::type_list containing the function argument types.

    static constexpr bool   is_function        = false;            ///< <b>true</b> if the type is a free, static or global function, otherwise <b>false</b>.
    static constexpr bool   is_member_function = true;             ///< <b>true</b> if the type is a member function, otherwise <b>false</b>.
    static constexpr bool   is_const           = false;            ///< <b>true</b> if the type is a const member function, otherwise <b>false</b>.
    static constexpr bool   is_volatile        = true;             ///< <b>true</b> if the type is a volatile member function, otherwise <b>false</b>.
    static constexpr bool   is_noexcept        = false;            ///< <b>true</b> if the type is a noexcept, otherwise <b>false</b>.
    static constexpr size_t argument_count     = sizeof...(TArgs); ///< The number of arguments that the function takes.
  };

#if ETL_USING_CPP17
  template <typename TReturn, typename TObject, typename... TArgs>
  constexpr bool function_traits<TReturn(TObject::*)(TArgs...) volatile>::is_function;

  template <typename TReturn, typename TObject, typename... TArgs>
  constexpr bool function_traits<TReturn(TObject::*)(TArgs...) volatile>::is_member_function;

  template <typename TReturn, typename TObject, typename... TArgs>
  constexpr bool function_traits<TReturn(TObject::*)(TArgs...) volatile>::is_const;

  template <typename TReturn, typename TObject, typename... TArgs>
  constexpr bool function_traits<TReturn(TObject::*)(TArgs...) volatile>::is_volatile;

  template <typename TReturn, typename TObject, typename... TArgs>
  constexpr bool function_traits<TReturn(TObject::*)(TArgs...) volatile>::is_noexcept;

  template <typename TReturn, typename TObject, typename... TArgs>
  constexpr size_t function_traits<TReturn(TObject::*)(TArgs...) volatile>::argument_count;
#endif

  //***************************************************************************
  /// Specialisation for const volatile member function pointers
  //***************************************************************************
  template <typename TReturn, typename TObject, typename... TArgs>
  struct function_traits<TReturn(TObject::*)(TArgs...) const volatile>
  {
    using function_type  = TReturn(TArgs...);                      ///< The signature of the function.
    using return_type    = TReturn;                                ///< The return type.
    using object_type    = TObject;                                ///< The object type, if a member function.
    using argument_types = etl::type_list<TArgs...>;               ///< An etl::type_list containing the function argument types.

    static constexpr bool   is_function        = false;            ///< <b>true</b> if the type is a free, static or global function, otherwise <b>false</b>.
    static constexpr bool   is_member_function = true;             ///< <b>true</b> if the type is a member function, otherwise <b>false</b>.
    static constexpr bool   is_const           = true;             ///< <b>true</b> if the type is a const member function, otherwise <b>false</b>.
    static constexpr bool   is_volatile        = true;             ///< <b>true</b> if the type is a volatile member function, otherwise <b>false</b>.
    static constexpr bool   is_noexcept        = false;            ///< <b>true</b> if the type is a noexcept, otherwise <b>false</b>.
    static constexpr size_t argument_count     = sizeof...(TArgs); ///< The number of arguments that the function takes.
  };

#if ETL_USING_CPP17
  template <typename TReturn, typename TObject, typename... TArgs>
  constexpr bool function_traits<TReturn(TObject::*)(TArgs...) const volatile>::is_function;

  template <typename TReturn, typename TObject, typename... TArgs>
  constexpr bool function_traits<TReturn(TObject::*)(TArgs...) const volatile>::is_member_function;

  template <typename TReturn, typename TObject, typename... TArgs>
  constexpr bool function_traits<TReturn(TObject::*)(TArgs...) const volatile>::is_const;

  template <typename TReturn, typename TObject, typename... TArgs>
  constexpr bool function_traits<TReturn(TObject::*)(TArgs...) const volatile>::is_volatile;

  template <typename TReturn, typename TObject, typename... TArgs>
  constexpr bool function_traits<TReturn(TObject::*)(TArgs...) const volatile>::is_noexcept;

  template <typename TReturn, typename TObject, typename... TArgs>
  constexpr size_t function_traits<TReturn(TObject::*)(TArgs...) const volatile>::argument_count;
#endif

#if ETL_HAS_NOEXCEPT_FUNCTION_TYPE
  //***************************************************************************
  /// Specialisation for noexcept function pointers
  //***************************************************************************
  template <typename TReturn, typename... TArgs>
  struct function_traits<TReturn(*)(TArgs...) noexcept>
  {
    using function_type  = TReturn(TArgs...);                      ///< The signature of the function.
    using return_type    = TReturn;                                ///< The return type.
    using object_type    = void;                                   ///< The object type, if a member function.
    using argument_types = etl::type_list<TArgs...>;               ///< An etl::type_list containing the function argument types.

    static constexpr bool   is_function        = true;             ///< <b>true</b> if the type is a free, static or global function, otherwise <b>false</b>.
    static constexpr bool   is_member_function = false;            ///< <b>true</b> if the type is a member function, otherwise <b>false</b>.
    static constexpr bool   is_const           = false;            ///< <b>true</b> if the type is a const member function, otherwise <b>false</b>.
    static constexpr bool   is_volatile        = false;            ///< <b>true</b> if the type is a volatile member function, otherwise <b>false</b>.
    static constexpr bool   is_noexcept        = true;             ///< <b>true</b> if the type is a noexcept, otherwise <b>false</b>.
    static constexpr size_t argument_count     = sizeof...(TArgs); ///< The number of arguments that the function takes.
  };

#if ETL_USING_CPP17
  template <typename TReturn, typename... TArgs>
  constexpr bool function_traits<TReturn(*)(TArgs...) noexcept>::is_function;

  template <typename TReturn, typename... TArgs>
  constexpr bool function_traits<TReturn(*)(TArgs...) noexcept>::is_member_function;

  template <typename TReturn, typename... TArgs>
  constexpr bool function_traits<TReturn(*)(TArgs...) noexcept>::is_const;

  template <typename TReturn, typename... TArgs>
  constexpr bool function_traits<TReturn(*)(TArgs...) noexcept>::is_volatile;

  template <typename TReturn, typename... TArgs>
  constexpr bool function_traits<TReturn(*)(TArgs...) noexcept>::is_noexcept;

  template <typename TReturn, typename... TArgs>
  constexpr size_t function_traits<TReturn(*)(TArgs...) noexcept>::argument_count;
#endif

  //***************************************************************************
  /// Specialisation for noexcept function references
  //***************************************************************************
  template <typename TReturn, typename... TArgs>
  struct function_traits<TReturn(&)(TArgs...) noexcept>
  {
    using function_type  = TReturn(TArgs...);                      ///< The signature of the function.
    using return_type    = TReturn;                                ///< The return type.
    using object_type    = void;                                   ///< The object type, if a member function.
    using argument_types = etl::type_list<TArgs...>;               ///< An etl::type_list containing the function argument types.

    static constexpr bool   is_function        = true;             ///< <b>true</b> if the type is a free, static or global function, otherwise <b>false</b>.
    static constexpr bool   is_member_function = false;            ///< <b>true</b> if the type is a member function, otherwise <b>false</b>.
    static constexpr bool   is_const           = false;            ///< <b>true</b> if the type is a const member function, otherwise <b>false</b>.
    static constexpr bool   is_volatile        = false;            ///< <b>true</b> if the type is a volatile member function, otherwise <b>false</b>.
    static constexpr bool   is_noexcept        = true;             ///< <b>true</b> if the type is a noexcept, otherwise <b>false</b>.
    static constexpr size_t argument_count     = sizeof...(TArgs); ///< The number of arguments that the function takes.
  };

#if ETL_USING_CPP17
  template <typename TReturn, typename... TArgs>
  constexpr bool function_traits<TReturn(&)(TArgs...) noexcept>::is_function;

  template <typename TReturn, typename... TArgs>
  constexpr bool function_traits<TReturn(&)(TArgs...) noexcept>::is_member_function;

  template <typename TReturn, typename... TArgs>
  constexpr bool function_traits<TReturn(&)(TArgs...) noexcept>::is_const;

  template <typename TReturn, typename... TArgs>
  constexpr bool function_traits<TReturn(&)(TArgs...) noexcept>::is_volatile;

  template <typename TReturn, typename... TArgs>
  constexpr bool function_traits<TReturn(&)(TArgs...) noexcept>::is_noexcept;

  template <typename TReturn, typename... TArgs>
  constexpr size_t function_traits<TReturn(&)(TArgs...) noexcept>::argument_count;
#endif

  //***************************************************************************
  /// Specialisation for noexcept member function pointers
  //***************************************************************************
  template <typename TReturn, typename TObject, typename... TArgs>
  struct function_traits<TReturn(TObject::*)(TArgs...) noexcept>
  {
    using function_type  = TReturn(TArgs...);                      ///< The signature of the function.
    using return_type    = TReturn;                                ///< The return type.
    using object_type    = TObject;                                ///< The object type, if a member function.
    using argument_types = etl::type_list<TArgs...>;               ///< An etl::type_list containing the function argument types.

    static constexpr bool   is_function        = false;            ///< <b>true</b> if the type is a free, static or global function, otherwise <b>false</b>.
    static constexpr bool   is_member_function = true;             ///< <b>true</b> if the type is a member function, otherwise <b>false</b>.
    static constexpr bool   is_const           = false;            ///< <b>true</b> if the type is a const member function, otherwise <b>false</b>.
    static constexpr bool   is_volatile        = false;            ///< <b>true</b> if the type is a volatile member function, otherwise <b>false</b>.
    static constexpr bool   is_noexcept        = true;             ///< <b>true</b> if the type is a noexcept, otherwise <b>false</b>.
    static constexpr size_t argument_count     = sizeof...(TArgs); ///< The number of arguments that the function takes.
  };

#if ETL_USING_CPP17
  template <typename TReturn, typename TObject, typename... TArgs>
  constexpr bool function_traits<TReturn(TObject::*)(TArgs...) noexcept>::is_function;

  template <typename TReturn, typename TObject, typename... TArgs>
  constexpr bool function_traits<TReturn(TObject::*)(TArgs...) noexcept>::is_member_function;

  template <typename TReturn, typename TObject, typename... TArgs>
  constexpr bool function_traits<TReturn(TObject::*)(TArgs...) noexcept>::is_const;

  template <typename TReturn, typename TObject, typename... TArgs>
  constexpr bool function_traits<TReturn(TObject::*)(TArgs...) noexcept>::is_volatile;

  template <typename TReturn, typename TObject, typename... TArgs>
  constexpr bool function_traits<TReturn(TObject::*)(TArgs...) noexcept>::is_noexcept;

  template <typename TReturn, typename TObject, typename... TArgs>
  constexpr size_t function_traits<TReturn(TObject::*)(TArgs...) noexcept>::argument_count;
#endif

  //***************************************************************************
  /// Specialisation for const noexcept member function pointers
  //***************************************************************************
  template <typename TReturn, typename TObject, typename... TArgs>
  struct function_traits<TReturn(TObject::*)(TArgs...) const noexcept>
  {
    using function_type  = TReturn(TArgs...);                      ///< The signature of the function.
    using return_type    = TReturn;                                ///< The return type.
    using object_type    = TObject;                                ///< The object type, if a member function.
    using argument_types = etl::type_list<TArgs...>;               ///< An etl::type_list containing the function argument types.

    static constexpr bool   is_function        = false;            ///< <b>true</b> if the type is a free, static or global function, otherwise <b>false</b>.
    static constexpr bool   is_member_function = true;             ///< <b>true</b> if the type is a member function, otherwise <b>false</b>.
    static constexpr bool   is_const           = true;             ///< <b>true</b> if the type is a const member function, otherwise <b>false</b>.
    static constexpr bool   is_volatile        = false;            ///< <b>true</b> if the type is a volatile member function, otherwise <b>false</b>.
    static constexpr bool   is_noexcept        = true;             ///< <b>true</b> if the type is a noexcept, otherwise <b>false</b>.
    static constexpr size_t argument_count     = sizeof...(TArgs); ///< The number of arguments that the function takes.
  };

#if ETL_USING_CPP17
  template <typename TReturn, typename TObject, typename... TArgs>
  constexpr bool function_traits<TReturn(TObject::*)(TArgs...) const noexcept>::is_function;

  template <typename TReturn, typename TObject, typename... TArgs>
  constexpr bool function_traits<TReturn(TObject::*)(TArgs...) const noexcept>::is_member_function;

  template <typename TReturn, typename TObject, typename... TArgs>
  constexpr bool function_traits<TReturn(TObject::*)(TArgs...) const noexcept>::is_const;

  template <typename TReturn, typename TObject, typename... TArgs>
  constexpr bool function_traits<TReturn(TObject::*)(TArgs...) const noexcept>::is_volatile;

  template <typename TReturn, typename TObject, typename... TArgs>
  constexpr bool function_traits<TReturn(TObject::*)(TArgs...) const noexcept>::is_noexcept;

  template <typename TReturn, typename TObject, typename... TArgs>
  constexpr size_t function_traits<TReturn(TObject::*)(TArgs...) const noexcept>::argument_count;
#endif

  //***************************************************************************
  /// Specialisation for volatile noexcept member function pointers
  //***************************************************************************
  template <typename TReturn, typename TObject, typename... TArgs>
  struct function_traits<TReturn(TObject::*)(TArgs...) volatile noexcept>
  {
    using function_type  = TReturn(TArgs...);                      ///< The signature of the function.
    using return_type    = TReturn;                                ///< The return type.
    using object_type    = TObject;                                ///< The object type, if a member function.
    using argument_types = etl::type_list<TArgs...>;               ///< An etl::type_list containing the function argument types.

    static constexpr bool   is_function        = false;            ///< <b>true</b> if the type is a free, static or global function, otherwise <b>false</b>.
    static constexpr bool   is_member_function = true;             ///< <b>true</b> if the type is a member function, otherwise <b>false</b>.
    static constexpr bool   is_const           = false;            ///< <b>true</b> if the type is a const member function, otherwise <b>false</b>.
    static constexpr bool   is_volatile        = true;             ///< <b>true</b> if the type is a volatile member function, otherwise <b>false</b>.
    static constexpr bool   is_noexcept        = true;             ///< <b>true</b> if the type is a noexcept, otherwise <b>false</b>.
    static constexpr size_t argument_count     = sizeof...(TArgs); ///< The number of arguments that the function takes.
  };

#if ETL_USING_CPP17
  template <typename TReturn, typename TObject, typename... TArgs>
  constexpr bool function_traits<TReturn(TObject::*)(TArgs...) volatile noexcept>::is_function;

  template <typename TReturn, typename TObject, typename... TArgs>
  constexpr bool function_traits<TReturn(TObject::*)(TArgs...) volatile noexcept>::is_member_function;

  template <typename TReturn, typename TObject, typename... TArgs>
  constexpr bool function_traits<TReturn(TObject::*)(TArgs...) volatile noexcept>::is_const;

  template <typename TReturn, typename TObject, typename... TArgs>
  constexpr bool function_traits<TReturn(TObject::*)(TArgs...) volatile noexcept>::is_volatile;

  template <typename TReturn, typename TObject, typename... TArgs>
  constexpr bool function_traits<TReturn(TObject::*)(TArgs...) volatile noexcept>::is_noexcept;

  template <typename TReturn, typename TObject, typename... TArgs>
  constexpr size_t function_traits<TReturn(TObject::*)(TArgs...) volatile noexcept>::argument_count;
#endif

  //***************************************************************************
  /// Specialisation for volatile const noexcept member function pointers
  //***************************************************************************
  template <typename TReturn, typename TObject, typename... TArgs>
  struct function_traits<TReturn(TObject::*)(TArgs...) const volatile noexcept>
  {
    using function_type  = TReturn(TArgs...);                      ///< The signature of the function.
    using return_type    = TReturn;                                ///< The return type.
    using object_type    = TObject;                                ///< The object type, if a member function.
    using argument_types = etl::type_list<TArgs...>;               ///< An etl::type_list containing the function argument types.

    static constexpr bool   is_function        = false;            ///< <b>true</b> if the type is a free, static or global function, otherwise <b>false</b>.
    static constexpr bool   is_member_function = true;             ///< <b>true</b> if the type is a member function, otherwise <b>false</b>.
    static constexpr bool   is_const           = true;             ///< <b>true</b> if the type is a const member function, otherwise <b>false</b>.
    static constexpr bool   is_volatile        = true;             ///< <b>true</b> if the type is a volatile member function, otherwise <b>false</b>.
    static constexpr bool   is_noexcept        = true;             ///< <b>true</b> if the type is a noexcept, otherwise <b>false</b>.
    static constexpr size_t argument_count     = sizeof...(TArgs); ///< The number of arguments that the function takes.
  };

#if ETL_USING_CPP17
  template <typename TReturn, typename TObject, typename... TArgs>
  constexpr bool function_traits<TReturn(TObject::*)(TArgs...) const volatile noexcept>::is_function;

  template <typename TReturn, typename TObject, typename... TArgs>
  constexpr bool function_traits<TReturn(TObject::*)(TArgs...) const volatile noexcept>::is_member_function;

  template <typename TReturn, typename TObject, typename... TArgs>
  constexpr bool function_traits<TReturn(TObject::*)(TArgs...) const volatile noexcept>::is_const;

  template <typename TReturn, typename TObject, typename... TArgs>
  constexpr bool function_traits<TReturn(TObject::*)(TArgs...) const volatile noexcept>::is_volatile;

  template <typename TReturn, typename TObject, typename... TArgs>
  constexpr bool function_traits<TReturn(TObject::*)(TArgs...) const volatile noexcept>::is_noexcept;

  template <typename TReturn, typename TObject, typename... TArgs>
  constexpr size_t function_traits<TReturn(TObject::*)(TArgs...) const volatile noexcept>::argument_count;
#endif

#endif

  // Forward cv/ref to the unqualified type
  template <typename T>
  struct function_traits<T&> : function_traits<etl::remove_reference_t<T>> {};

  template <typename T>
  struct function_traits<T&&> : function_traits<etl::remove_reference<T>> {};

  template <typename T>
  struct function_traits<const T> : function_traits<etl::remove_const<T>> {};

  template <typename T>
  struct function_traits<volatile T> : function_traits<etl::remove_volatile<T>> {};

  template <typename T>
  struct function_traits<const volatile T> : function_traits<etl::remove_cv<T>> {};

  // Catch-all for functors/lambdas that have a single, non-template operator()
  // Delegates to the existing member function pointer specialization via &T::operator().
  template <typename T>
  struct function_traits : function_traits<decltype(&T::operator())>
  {
  };
}
#endif

#endif
