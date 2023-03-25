///\file

/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl
https://www.etlcpp.com

Copyright(c) 2021 John Wellbelove

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
#include "utility.h"
#include "type_traits.h"

namespace etl
{
#if ETL_USING_CPP17 && !defined(ETL_OVERLOAD_FORCE_CPP14)

  //*************************************************************************
  /// Variadic template definition of overload for C++17 and above.
  //*************************************************************************
  template<typename... TOverloads>
  struct overload : TOverloads...
  {
    using TOverloads::operator()...;
  };

  //*************************************************************************
  /// Template deduction guide.
  //*************************************************************************
  template<typename... TOverloads> overload(TOverloads...)->overload<TOverloads...>;

#else

  //*************************************************************************
  /// Variadic template definition of overload for C++14.
  //*************************************************************************
  template <typename TFirst, typename... TOthers>
  struct overload : TFirst, overload<TOthers...>
  {
    using TFirst::operator();
    using overload<TOthers...>::operator();
  };

  template <typename TFirst> 
  struct overload<TFirst> : TFirst
  {
    using TFirst::operator();
  };

#endif

#if ETL_USING_CPP17
  //*************************************************************************
  /// Make an overload.
  //*************************************************************************
  template <typename... TOverloads>
  constexpr overload<TOverloads...> make_overload(TOverloads&&... overloads)
  {
    return overload<TOverloads...>{ etl::forward<TOverloads>(overloads)... };
  }
#endif
}

#endif
