///\file

/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl
http://www.etlcpp.com

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

#ifndef ETL_DEBUG_COUNT_INCLUDED
#define ETL_DEBUG_COUNT_INCLUDED

#include <stdint.h>
#include <assert.h>

#include "platform.h"

///\defgroup debug_count debug count
///\ingroup utilities

#if defined(ETL_DEBUG_COUNT)

#define ETL_DECLARE_DEBUG_COUNT     etl::debug_count etl_debug_count;
#define ETL_INCREMENT_DEBUG_COUNT   ++etl_debug_count;
#define ETL_DECREMENT_DEBUG_COUNT   --etl_debug_count;
#define ETL_ADD_DEBUG_COUNT(n)      etl_debug_count += (n);
#define ETL_SUBTRACT_DEBUG_COUNT(n) etl_debug_count -= (n);
#define ETL_RESET_DEBUG_COUNT       etl_debug_count.clear();

namespace etl
{
  //***************************************************************************
  /// Used to count instances.
  /// Asserts if the count is decremented below zero.
  /// Asserts if the count is not zero when destructed.
  /// Does nothing in a non-debug build.
  ///\ingroup reference
  //***************************************************************************
  class debug_count
  {
  public:

    inline debug_count()
      : count(0)
    {
    }

    inline ~debug_count()
    {
      assert(count == 0);
    }

    inline debug_count& operator ++()
    {
      ++count;
      return *this;
    }

    inline debug_count& operator --()
    {
      --count;
      assert(count >= 0);
      return *this;
    }

    template <typename T>
    inline debug_count& operator +=(T n)
    {
      count += int32_t(n);
      return *this;
    }

    template <typename T>
    inline debug_count& operator -=(T n)
    {
      count -= int32_t(n);
      return *this;
    }

    inline operator int32_t()
    {
      return count;
    }

    inline void clear()
    {
      count = 0;
    }

  private:

    int32_t count;
  };

}

#else
  #define ETL_DECLARE_DEBUG_COUNT
  #define ETL_INCREMENT_DEBUG_COUNT
  #define ETL_DECREMENT_DEBUG_COUNT
  #define ETL_ADD_DEBUG_COUNT(n)
  #define ETL_SUBTRACT_DEBUG_COUNT(n)
  #define ETL_RESET_DEBUG_COUNT
#endif // ETL_DEBUG_COUNT

#endif
