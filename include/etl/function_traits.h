///\file

/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl
https://www.etlcpp.com

Copyright(c) 2024 John Wellbelove

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
  // Helper template to extract the function type from a member function pointer
  //***************************************************************************
  template <typename T>
  struct function_traits
  {
    using function_type  = void;
    using return_type    = void;
    using object_type    = void;
    using argument_types = etl::type_list<>;

    static constexpr bool is_function        = false;
    static constexpr bool is_member_function = false;
    static constexpr bool is_const           = false;
    static constexpr size_t argument_count   = 0;
  };

  //***************************************************************************
  // Specialisation for function pointers
  //***************************************************************************
  template <typename TReturn, typename... TArgs>
  struct function_traits<TReturn(*)(TArgs...)>
  {
    using function_type  = TReturn(TArgs...);
    using return_type    = TReturn;
    using object_type    = void;
    using argument_types = etl::type_list<TArgs...>;

    static constexpr bool is_function        = true;
    static constexpr bool is_member_function = false;
    static constexpr bool is_const           = false;
    static constexpr size_t argument_count   = sizeof...(TArgs);
  };

  //***************************************************************************
  // Specialisation for member function pointers
  //***************************************************************************
  template <typename TReturn, typename TObject, typename... TArgs>
  struct function_traits<TReturn(TObject::*)(TArgs...)>
  {
    using function_type  = TReturn(TArgs...);
    using return_type    = TReturn;
    using object_type    = TObject;
    using argument_types = etl::type_list<TArgs...>;

    static constexpr bool is_function        = false;
    static constexpr bool is_member_function = true;
    static constexpr bool is_const           = false;
    static constexpr size_t argument_count   = sizeof...(TArgs);
  };

  //***************************************************************************
  // Specialisation for const member function pointers
  //***************************************************************************
  template <typename TReturn, typename TObject, typename... TArgs>
  struct function_traits<TReturn(TObject::*)(TArgs...) const>
  {
    using function_type  = TReturn(TArgs...);
    using return_type    = TReturn;
    using object_type    = TObject;
    using argument_types = etl::type_list<TArgs...>;

    static constexpr bool is_function        = false;
    static constexpr bool is_member_function = true;
    static constexpr bool is_const           = true;
    static constexpr size_t argument_count   = sizeof...(TArgs);
  };
}
#endif

#endif
