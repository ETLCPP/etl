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

#ifndef ETL_NTH_TYPE_INCLUDED
#define ETL_NTH_TYPE_INCLUDED

#include "platform.h"
#include "static_assert.h"

namespace etl
{
#if ETL_USING_CPP11 && !defined(ETL_NTH_TYPE_FORCE_CPP03_IMPLEMENTATION)
  namespace private_nth_type
  {
    //***********************************
    template <size_t N, typename T1, typename... TRest>
    struct nth_type_helper
    {
      using type = typename nth_type_helper<N - 1U, TRest...>::type;
    };

    template <typename T1, typename... TRest>
    struct nth_type_helper<0U, T1, TRest...>
    {
      using type = T1;
    };
  }

  //***********************************
  template <size_t N, typename... TTypes>
  struct nth_type
  {
    ETL_STATIC_ASSERT(N < sizeof...(TTypes), "etl::nth_type index 'N' out of bounds");

    using type = typename private_nth_type::nth_type_helper<N, TTypes...>::type;
  };

  //***********************************
  template <size_t N, typename... TTypes>
  using nth_type_t = typename nth_type<N, TTypes...>::type;
#else
  // Primary template: Not defined, will cause a compile error if used with N > 7 or not enough types.
  template <int N, typename T0 = void, typename T1 = void, typename T2 = void, typename T3 = void,
                   typename T4 = void, typename T5 = void, typename T6 = void, typename T7 = void>
  struct nth_type;

  // Specialisations for N = 0..7
  template <typename T0, typename T1, typename T2>
  struct nth_type<0, T0, T1, T2, void, void, void, void, void> { typedef T0 type; };

  template <typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7>
  struct nth_type<0, T0, T1, T2, T3, T4, T5, T6, T7> { typedef T0 type; };
  
  template <typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7>
  struct nth_type<1, T0, T1, T2, T3, T4, T5, T6, T7> { typedef T1 type; };

  template <typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7>
  struct nth_type<2, T0, T1, T2, T3, T4, T5, T6, T7> { typedef T2 type; };

  template <typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7>
  struct nth_type<3, T0, T1, T2, T3, T4, T5, T6, T7> { typedef T3 type; };

  template <typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7>
  struct nth_type<4, T0, T1, T2, T3, T4, T5, T6, T7> { typedef T4 type; };

  template <typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7>
  struct nth_type<5, T0, T1, T2, T3, T4, T5, T6, T7> { typedef T5 type; };

  template <typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7>
  struct nth_type<6, T0, T1, T2, T3, T4, T5, T6, T7> { typedef T6 type; };

  template <typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7>
  struct nth_type<7, T0, T1, T2, T3, T4, T5, T6, T7> { typedef T7 type; };
#endif
}
#endif
