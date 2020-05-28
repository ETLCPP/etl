///\file

/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl
https://www.etlcpp.com

Copyright(c) 2017 jwellbelove

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

#ifndef ETL_SQRT_INCLUDED
#define ETL_SQRT_INCLUDED

#include <stddef.h>

#include "platform.h"
#include "type_traits.h"
#include "constant.h"

namespace etl 
{
  //***************************************************************************
  /// Calculates the smallest value that, when squared, will be not greater than VALUE.
  //***************************************************************************
  template <const size_t VALUE, const size_t I = 1>
  struct sqrt
  {
    typedef typename etl::conditional<((I * I) > VALUE), 
                                      etl::constant<intmax_t, I - 1>,
                                      etl::sqrt<VALUE, I + 1> >::type type;

#if ETL_CPP11_SUPPORTED
    static constexpr size_t value = type::value;
#else
    enum
    {
      // Recursive definition.
      value = type::value
    };
#endif
  };
}

#endif
