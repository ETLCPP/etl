/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl
https://www.etlcpp.com

Copyright(c) 2018 John Wellbelove

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

#ifndef ETL_TYPE_SELECT_INCLUDED
#define ETL_TYPE_SELECT_INCLUDED

#include "platform.h"
#include "null_type.h"
#include "type_traits.h"

#include <stddef.h>

namespace etl
{
#if ETL_USING_CPP11 && !defined(ETL_TYPE_SELECT_FORCE_CPP03_IMPLEMENTATION)
  //***************************************************************************
  // Variadic version.
  //***************************************************************************
  template <typename... TTypes>
  struct type_select
  {
  private:

    //***********************************
    template <size_t Id, size_t Index, typename T1, typename... TRest>
    struct type_select_helper
    {
      using type = typename etl::conditional< Id == Index, T1, typename type_select_helper<Id, Index + 1, TRest...>::type>::type;
    };

    //***********************************
    template <size_t Id, size_t Index, typename T1>
    struct type_select_helper<Id, Index, T1>
    {
      using type = T1;
    };

  public:

    template <size_t Id>
    struct select
    {
      static_assert(Id < sizeof...(TTypes), "Illegal type_select::select index");

      using type = typename type_select_helper<Id, 0, TTypes...>::type;
    };

    template <size_t Id>
    using select_t = typename select<Id>::type;
  };

  //***************************************************************************
  // Select type alias
  //***************************************************************************
  template <size_t Index, typename... TTypes>
  using type_select_t = typename etl::type_select<TTypes...>::template select_t<Index>;

#else
  #include "private/type_select_cpp03.h"
#endif
} // namespace etl

#endif
