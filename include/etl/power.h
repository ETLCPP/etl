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

#ifndef ETL_POW_INCLUDED
#define ETL_POW_INCLUDED

#include "platform.h"
#include "log.h"

#include <stddef.h>
#include <stdint.h>

///\defgroup power power
/// power<N, POWER> : Calculates N to the power POWER.
///\ingroup maths

namespace etl
{
  namespace private_power
  {
#if ETL_NOT_USING_64BIT_TYPES
    typedef uint32_t type;
#else
    typedef uint64_t type;
#endif
  }

  //***************************************************************************
  ///\ingroup power
  /// Calculates powers.
  ///\note Only supports positive N.
  //***************************************************************************
  template <size_t NV, size_t POWER>
  struct power
  {
    static ETL_CONSTANT private_power::type value = NV * power<NV, POWER - 1>::value;
  };

  template <size_t NV, size_t POWER>
  ETL_CONSTANT private_power::type power<NV, POWER>::value;

  //***************************************************************************
  /// Calculates powers.
  ///\note Only supports positive N.
  /// Specialisation for POWER == 0.
  //***************************************************************************
  template <size_t NV>
  struct power<NV, 0>
  {
    static ETL_CONSTANT private_power::type value = 1;
  };

  template <size_t NV>
  ETL_CONSTANT private_power::type power<NV, 0>::value;

#if ETL_USING_CPP17
  template <size_t NV, size_t POWER>
  inline constexpr size_t power_v = power<NV, POWER>::value;
#endif

  //***************************************************************************
  ///\ingroup power
  /// Calculates the rounded up power of 2.
  //***************************************************************************
  template <size_t NV>
  struct power_of_2_round_up
  {
    enum value_type
    {
      value = 1 << (etl::log2<NV - 1>::value + 1)
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

#if ETL_USING_CPP17
  template <size_t NV>
  inline constexpr size_t power_of_2_round_up_v = power_of_2_round_up<NV>::value;
#endif

  //***************************************************************************
  ///\ingroup power
  /// Calculates the rounded down power of 2.
  //***************************************************************************
  template <size_t NV>
  struct power_of_2_round_down
  {
    enum value_type
    {
      value = 1 << (etl::log2<NV - 1>::value)
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

#if ETL_USING_CPP17
  template <size_t NV>
  inline constexpr size_t power_of_2_round_down_v = power_of_2_round_down<NV>::value;
#endif

  //***************************************************************************
  ///\ingroup power
  /// Checks if N is a power of 2.
  //***************************************************************************
  template <size_t NV>
  struct is_power_of_2
  {
    static ETL_CONSTANT bool value = (NV & (NV - 1)) == 0;
  };

  //***************************************************************************
  ///\ingroup power
  /// Checks if N is a power of 2.
  /// Specialisation for 0.
  //***************************************************************************
  template <>
  struct is_power_of_2<0>
  {
    static ETL_CONSTANT bool value = false;
  };

  //***************************************************************************
  ///\ingroup power
  /// Checks if N is a power of 2.
  /// Specialisation for 1.
  //***************************************************************************
  template <>
  struct is_power_of_2<1>
  {
    static ETL_CONSTANT bool value = false;
  };

  template <size_t NV>
  ETL_CONSTANT bool is_power_of_2<NV>::value;

#if ETL_USING_CPP17
  template <size_t NV>
  inline constexpr size_t is_power_of_2_v = is_power_of_2<NV>::value;
#endif
}

#endif
