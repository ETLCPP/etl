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

#ifndef __ETL_DEBUG_COUNT__
#define __ETL_DEBUG_COUNT__

#include <stdint.h>
#include <assert.h>

#include "platform.h"

///\defgroup debug_count debug count
///\ingroup utilities

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
    
#if defined(ETL_DEBUG)
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

    inline debug_count& operator +=(int32_t n)
    {
      count += n;
      return *this;
    }

    inline debug_count& operator -=(int32_t n)
    {
      count -= n;
      return *this;
    }

    inline operator int32_t()
    {
      return count;
    }

  private:

    int32_t count;
#else
    inline debug_count()
    {
    }

    inline ~debug_count()
    {
    }

    inline debug_count& operator ++()
    {
      return *this;
    }

    inline debug_count& operator --()
    {
      return *this;
    }

    inline debug_count& operator +=(int32_t /*n*/)
    {
      return *this;
    }

    inline debug_count& operator -=(int32_t /*n*/)
    {
      return *this;
    }

    inline operator int32_t()
    {
      return 0;
    }
#endif
  };
}

#endif

