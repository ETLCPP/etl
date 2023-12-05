///\file

/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl
https://www.etlcpp.com

Copyright(c) 2023 John Wellbelove

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

#ifndef ETL_MATH_INCLUDED
#define ETL_MATH_INCLUDED

#include "platform.h"

#if ETL_NOT_USING_STL && defined(ETL_COMPILER_ARM5) && !defined(__USE_C99_MATH)
  // Required for nan, nanf, nanl
  #define __USE_C99_MATH
#endif

#include <float.h>
#include <math.h>

#include "limits.h"
#include "type_traits.h"

namespace etl
{
  //***************************************************************************
  // is_nan
  //***************************************************************************
#if ETL_USING_CPP11 && !defined(ETL_NO_CPP_NAN_SUPPORT)
  template <typename T>
  ETL_CONSTEXPR
    typename etl::enable_if<etl::is_floating_point<T>::value, bool>::type
    is_nan(T value)
  {
    return fpclassify(value) == FP_NAN;
  }
#else
#include "private/diagnostic_float_equal_push.h"
  template <typename T>
  ETL_CONSTEXPR
    typename etl::enable_if<etl::is_floating_point<T>::value, bool>::type
    is_nan(T value)
  {
    return (value != value);
  }
#include "private/diagnostic_pop.h"
#endif

  template <typename T>
  ETL_CONSTEXPR
  typename etl::enable_if<etl::is_integral<T>::value, bool>::type
    is_nan(T)
  {
    return false;
  }

  //***************************************************************************
  // is_infinity
  //***************************************************************************
#if ETL_USING_CPP11 && !defined(ETL_NO_CPP_NAN_SUPPORT)
  template <typename T>
  ETL_CONSTEXPR
  typename etl::enable_if<etl::is_floating_point<T>::value, bool>::type
    is_infinity(T value)
  {
    return fpclassify(value) == FP_INFINITE;
  }
#else
#include "private/diagnostic_float_equal_push.h"
  template <typename T>
  ETL_CONSTEXPR
  typename etl::enable_if<etl::is_floating_point<T>::value, bool>::type
    is_infinity(T value)
  {
    return ((value == etl::numeric_limits<T>::infinity()) ||
            (value == -etl::numeric_limits<T>::infinity()));
  }
#include "private/diagnostic_pop.h"
#endif

  template <typename T>
  ETL_CONSTEXPR
  typename etl::enable_if<etl::is_integral<T>::value, bool>::type
    is_infinity(T)
  {
    return false;
  }

  //***************************************************************************
  // is_zero
  //***************************************************************************
#if ETL_USING_CPP11 && !defined(ETL_NO_CPP_NAN_SUPPORT)
  template <typename T>
  ETL_CONSTEXPR
  typename etl::enable_if<etl::is_floating_point<T>::value, bool>::type
    is_zero(T value)
  {
    return fpclassify(value) == FP_ZERO;
  }
#else
#include "private/diagnostic_float_equal_push.h"
  template <typename T>
  ETL_CONSTEXPR
  typename etl::enable_if<etl::is_floating_point<T>::value, bool>::type
    is_zero(T value)
  {
    return value == 0;
  }
#include "private/diagnostic_pop.h"
#endif

  template <typename T>
  ETL_CONSTEXPR
  typename etl::enable_if<etl::is_integral<T>::value, bool>::type
    is_zero(T value)
  {
    return (value == 0);
  }

  //***************************************************************************
  // is_exactly_equal
  //***************************************************************************
#include "private/diagnostic_float_equal_push.h"
  template <typename T>
  ETL_CONSTEXPR
  bool is_exactly_equal(T value1, T value2)
  {
    return value1 == value2;
  }
#include "private/diagnostic_pop.h"
}

#endif
