///\file

/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl
https://www.etlcpp.com

Copyright(c) 2014 John Wellbelove

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

#ifndef ETL_LOG_INCLUDED
#define ETL_LOG_INCLUDED

#include "platform.h"

#include <stddef.h>

///\defgroup log log
/// log<N, BASE> : Calculates logs to any base, rounded down to the nearest integer.<br>
/// log2<N>      : Calculates logs to base 2, rounded down to the nearest integer.<br>
/// log10<N>     : Calculates logs to base 10, rounded down to the nearest integer.<br>
///\ingroup maths

namespace etl
{
  //***************************************************************************
  ///\ingroup log
  /// The base generic log template.
  /// Defines <b>value</b> as the log of the number at the specified base.
  /// The result is rounded down to the next integer.
  ///\tparam NV   The number to find the log of.
  ///\tparam BASE The base of the log.
  //***************************************************************************
  template <size_t NV, size_t BASE>
  struct log
  {
    enum value_type
    {
      // Recursive definition.
      value = (NV >= BASE) ? 1 + log<NV / BASE, BASE>::value : 0
    };
  };

  //***************************************************************************
  // Specialisation for N = 1
  //***************************************************************************
  template <size_t BASE>
  struct log<1, BASE>
  {
    enum value_type
    {
      value = 0
    };
  };

  //***************************************************************************
  // Specialisation for N = 0
  //***************************************************************************
  template <size_t BASE>
  struct log<0, BASE>
  {
    enum value_type
    {
      value = 0
    };
  };

#if ETL_USING_CPP17
  template <size_t NV, size_t BASE>
  inline constexpr size_t log_v = log<NV, BASE>::value;
#endif

  //***************************************************************************
  ///\ingroup log
  /// Calculates base 2 logs.
  //***************************************************************************
  template <size_t NV>
  struct log2
  {
    enum value_type
    {
      value = log<NV, 2>::value
    };
  };

#if ETL_USING_CPP17
  template <size_t NV>
  inline constexpr size_t log2_v = log2<NV>::value;
#endif

  //***************************************************************************
  ///\ingroup log
  /// Calculates base 10 logs.
  //***************************************************************************
  template <size_t NV>
  struct log10
  {
    enum value_type
    {
      value = log<NV, 10>::value
    };
  };

#if ETL_USING_CPP17
  template <size_t NV>
  inline constexpr size_t log10_v = log10<NV>::value;
#endif
}

#endif
