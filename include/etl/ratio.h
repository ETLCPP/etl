///\file

/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl
https://www.etlcpp.com

Copyright(c) 2016 John Wellbelove

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

#ifndef ETL_RATIO_INCLUDED
#define ETL_RATIO_INCLUDED

#include "platform.h"

#include <stddef.h>
#include <stdint.h>

///\defgroup ratio ratio
///\ingroup maths

namespace etl
{
  template <const size_t NUM, const size_t DEN = 1UL>
  struct ratio
  {
    static ETL_CONSTANT intmax_t num = NUM;
    static ETL_CONSTANT intmax_t den = DEN;
  };

  #if INT_MAX > INT32_MAX
    typedef ratio<1, 1000000000000000000000000> yocto;
    typedef ratio<1, 1000000000000000000000>    zepto;
    typedef ratio<1, 1000000000000000000>       atto;
    typedef ratio<1, 1000000000000000>          femto;
    typedef ratio<1, 1000000000000>             pico;
  #endif

  #if (INT_MAX >= INT32_MAX)
    typedef ratio<1, 1000000000>                nano;
    typedef ratio<1, 1000000>                   micro;
  #endif

  #if (INT_MAX >= INT16_MAX)
    typedef ratio<1, 1000>                      milli;
    typedef ratio<1, 100>                       centi;
    typedef ratio<1, 10>                        deci;
    typedef ratio<10, 1>                        deca;
    typedef ratio<100, 1>                       hecto;
    typedef ratio<1000, 1>                      kilo;
  #endif

  #if (INT_MAX >= INT32_MAX)
    typedef ratio<1000000, 1>                   mega;
    typedef ratio<1000000000, 1>                giga;
  #endif

  #if INT_MAX > INT32_MAX
    typedef ratio<1000000000000, 1>             tera;
    typedef ratio<1000000000000000, 1>          peta;
    typedef ratio<1000000000000000000, 1>       exa;
    typedef ratio<1000000000000000000000, 1>    zetta;
    typedef ratio<1000000000000000000000000, 1> yotta;
  #endif

  /// An approximation of PI to 6 digits.
  typedef ratio<355, 113> ratio_pi;

  /// An approximation of root 2.
  typedef ratio<239, 169> ratio_root2;

  /// An approximation of e.
  typedef ratio<326, 120> ratio_e;
}

#endif

