///\file

/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl
https://www.etlcpp.com

Copyright(c) 2021 jwellbelove

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

#ifndef ETL_OVERLOAD_INCLUDED
#define ETL_OVERLOAD_INCLUDED

#include "platform.h"

namespace etl
{
#if ETL_CPP11_SUPPORTED
  #if ETL_CPP17_SUPPORTED && !defined(ETL_OVERLOAD_FORCE_CPP11)
  //*************************************************************************
  /// Variadic template definition of overload.
  //*************************************************************************
  template<class... Ts>
  struct overload : Ts...
  {
    using Ts::operator()...;
  };

  //*************************************************************************
  /// Template deduction guide.
  //*************************************************************************
  template<class... Ts> overload(Ts...)->overload<Ts...>;

#else
  //*************************************************************************
  /// Variadic template definition of overload.
  //*************************************************************************
  template <typename T, typename... Ts>
  struct overload : T, overload<Ts...>
  {
    using T::operator();
    using overload<Ts...>::operator();
  };

  //*************************************************************************
  /// Template specialisation of overload for one type.
  //*************************************************************************
  template <typename T> struct overload<T> : T
  {
    using T::operator();
  };
#endif

  //*************************************************************************
  /// Make an overload.
  //*************************************************************************
  template <typename... T>
  constexpr auto make_overload(T&&... t)
  {
    return overload<T...>{ etl::forward<T>(t)... };
  }
}

#endif
#endif
