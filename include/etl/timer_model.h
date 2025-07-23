///\file

/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl
https://www.etlcpp.com

Copyright(c) 2024 John Wellbelove

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

#ifndef ETL_TIMER_MODEL_INCLUDED
#define ETL_TIMER_MODEL_INCLUDED

#include "platform.h"
#include "user_type.h"
#include "type_lookup.h"

#include <stdint.h>

namespace etl
{
  ETL_DECLARE_USER_TYPE(timer_model, int)
  ETL_USER_TYPE(TIMER_MODEL_SMALL,  0)
  ETL_USER_TYPE(TIMER_MODEL_MEDIUM, 1)
  ETL_USER_TYPE(TIMER_MODEL_LARGE,  2)
  ETL_USER_TYPE(TIMER_MODEL_HUGE,   3)
  ETL_END_USER_TYPE(timer_model)

  template <size_t timer_model>
  struct size_type_lookup;

  template <>
  struct size_type_lookup<etl::timer_model::TIMER_MODEL_SMALL>
  {
    typedef uint_least8_t type;

    // value range given specific width
    struct range
    {
      enum
      {
        MAX = 0xFFU,
        Max = 0xFFU
      };
    };
  
  };

  template <>
  struct size_type_lookup<etl::timer_model::TIMER_MODEL_MEDIUM>
  {
    typedef uint_least16_t type;

    // value range given specific width
    struct range
    {
      enum
      {
        MAX = 0xFFFFU,
        Max = 0xFFFFU
      };
    };
  
  };

  template <>
  struct size_type_lookup<etl::timer_model::TIMER_MODEL_LARGE>
  {
    typedef uint_least32_t type;

    // value range given specific width
    struct range
    {
      enum
      {
        MAX = 0xFFFFFFFFU,
        Max = 0xFFFFFFFFU
      };
    };
  
  };

  template <>
  struct size_type_lookup<etl::timer_model::TIMER_MODEL_HUGE>
  {
    typedef uint_least64_t type;

    // value range given specific width
    struct range
    {
      enum
      {
        MAX = 0xFFFFFFFFFFFFFFFFU,
        Max = 0xFFFFFFFFFFFFFFFFU
      };
    };

  };
}

#endif

