/////\file
//
///******************************************************************************
//The MIT License(MIT)
//
//Embedded Template Library.
//https://github.com/ETLCPP/etl
//https://www.etlcpp.com
//
//Copyright(c) 2021 John Wellbelove
//
//Permission is hereby granted, free of charge, to any person obtaining a copy
//of this software and associated documentation files(the "Software"), to deal
//in the Software without restriction, including without limitation the rights
//to use, copy, modify, merge, publish, distribute, sublicense, and / or sell
//copies of the Software, and to permit persons to whom the Software is
//furnished to do so, subject to the following conditions :
//
//The above copyright notice and this permission notice shall be included in all
//copies or substantial portions of the Software.
//
//THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
//AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
//OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
//SOFTWARE.
//******************************************************************************/

#ifndef ETL_MULTI_VECTOR_INCLUDED
#define ETL_MULTI_VECTOR_INCLUDED

#include "platform.h"
#include "vector.h"

///\defgroup multi_vector multi_vector
/// A multi dimensional vector.
///\ingroup containers

#if ETL_USING_CPP11

namespace etl
{
  namespace private_multi_vector
  {
    template <typename T, size_t TD1, size_t... TDn>
    struct multi_vector_t
    {
      using type = etl::vector<typename multi_vector_t<T, TDn...>::type, TD1>;
      static constexpr size_t Max_Size = TD1;
    };

    template <class T, size_t TD1>
    struct multi_vector_t<T, TD1>
    {
      using type = etl::vector<T, TD1>;
      static constexpr size_t Max_Size = TD1;
    };
  }

  template <typename T, const size_t... TDn>
  using multi_vector = typename private_multi_vector::multi_vector_t<T, TDn...>::type;
}

#endif
#endif

