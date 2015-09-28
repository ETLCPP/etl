///\file

/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl

Copyright(c) 2014 jwellbelove

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

#ifndef __ETL_POW__
#define __ETL_POW__

#include <stddef.h>
#include "log.h"

///\defgroup power power
/// power<N, POWER> : Calculates N to the power POWER.
///\ingroup maths

namespace etl 
{
  //***************************************************************************
  ///\ingroup power
  /// Calculates powers.
  ///\note Only supports positive N.
  //***************************************************************************
  template <const size_t N, const size_t POWER>
  struct power
  {
    static const uint64_t value = N * power<N, POWER - 1>::value;
  };

  //***************************************************************************
  /// Calculates powers.
  ///\note Only supports positive N.
  /// Specialisation for POWER == 0.
  //***************************************************************************
  template <const size_t N>
  struct power<N, 0>
  {
    static const uint64_t value = 1;
  };

  //***************************************************************************
  /// Declaration of static 'value' for power.
  //***************************************************************************
  template <const size_t N, const size_t POWER> const uint64_t power<N, POWER>::value;

  //***************************************************************************
  ///\ingroup power
  /// Calculates the rounded up power of 2.
  //***************************************************************************
  template <const size_t N>
  struct power_of_2_round_up
  {
    enum value_type
    {
      value = 1 << (etl::log2<N - 1>::value + 1)
    };
  };

  //***************************************************************************
  ///\ingroup power
  /// Calculates the rounded up power of 2.
  /// Specialisation for 0.
  //***************************************************************************
  template <>
  struct power_of_2_round_up<0>
  {
    enum value_type
    {
      value = 2
    };
  };

  //***************************************************************************
  ///\ingroup power
  /// Calculates the rounded down power of 2.
  //***************************************************************************
  template <const size_t N>
  struct power_of_2_round_down
  {
    enum value_type
    {
      value = 1 << (etl::log2<N - 1>::value)
    };
  };

  //***************************************************************************
  ///\ingroup power
  /// Calculates the rounded down power of 2.
  /// Specialisation for 0.
  //***************************************************************************
  template <>
  struct power_of_2_round_down<0>
  {
    enum value_type
    {
      value = 2
    };
  };

  //***************************************************************************
  ///\ingroup power
  /// Calculates the rounded down power of 2.
  /// Specialisation for 1.
  //***************************************************************************
  template <>
  struct power_of_2_round_down<1>
  {
    enum value_type
    {
      value = 2
    };
  };

  //***************************************************************************
  ///\ingroup power
  /// Calculates the rounded down power of 2.
  /// Specialisation for 2.
  //***************************************************************************
  template <>
  struct power_of_2_round_down<2>
  {
    enum value_type
    {
      value = 2
    };
  };

  //***************************************************************************
  ///\ingroup power
  /// Checks if N is a power of 2.
  //***************************************************************************
  template <const size_t N>
  struct is_power_of_2
  {
    static const bool value = (N & (N - 1)) == 0;
  };

  //***************************************************************************
  ///\ingroup power
  /// Checks if N is a power of 2.
  /// Specialisation for 0.
  //***************************************************************************
  template <>
  struct is_power_of_2<0>
  {
    static const bool value = false;
  };

  //***************************************************************************
  ///\ingroup power
  /// Checks if N is a power of 2.
  /// Specialisation for 1.
  //***************************************************************************
  template <>
  struct is_power_of_2<1>
  {
    static const bool value = false;
  };
}

#endif
