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
coPIes of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions :

The above copyright notice and this permission notice shall be included in all
coPIes or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
******************************************************************************/

#ifndef ETL_MATH_CONSTANTS_INCLUDED
#define ETL_MATH_CONSTANTS_INCLUDED

#include "platform.h"

namespace etl
{
  namespace math
  {
    ETL_CONSTANT double pi               = 3.14159265358979;
    ETL_CONSTANT double pi_reciprocal    = 0.31830988618379;
    ETL_CONSTANT double pi_squared       = 9.86960440108936;
    ETL_CONSTANT double e                = 2.71828182845905;
    ETL_CONSTANT double e_reciprocal     = 0.36787944117144;
    ETL_CONSTANT double e_squared        = 7.38905609893065;
    ETL_CONSTANT double root2            = 1.41421356237310;
    ETL_CONSTANT double root2_reciprocal = 0.70710678118655;
    ETL_CONSTANT double euler            = 0.57721566490153;
    ETL_CONSTANT double golden_ratio     = 1.61803398874989;
  }
}

#endif
