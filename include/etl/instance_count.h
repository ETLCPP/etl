///\file

/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl
http://www.etlcpp.com

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

#ifndef ETL_INSTANCE_COUNT_INCLUDED
#define ETL_INSTANCE_COUNT_INCLUDED

#include <stdint.h>

#include "platform.h"

///\defgroup instance_count instance count
///\ingroup utilities

namespace etl
{
  //***************************************************************************
  /// Inherit from this to count instances of a type.
  ///\ingroup reference
  //***************************************************************************
  template <typename T>
  class instance_count
  {
  public:

    //*************************************************************************
    /// Construct and add 1.
    //*************************************************************************
    instance_count()
    {
      ++how_many();
    }

    //*************************************************************************
    /// Copy construct and add 1.
    //*************************************************************************
    instance_count(const instance_count&)
    {
      ++how_many();
    }

    //*************************************************************************
    /// Destruct and subtract 1.
    //*************************************************************************
    ~instance_count()
    {
      --how_many();
    }

    //*************************************************************************
    /// Get how many instances we have.
    //*************************************************************************
    inline static int32_t get_instance_count()
    {
      return how_many();
    }

    //*************************************************************************
    /// Get how many instances we have.
    //*************************************************************************
    inline static void reset_instance_count()
    {
      how_many() = 0;
    }

  private:

    //*************************************************************************
    /// Get a referenceto the count.
    //*************************************************************************
    inline static int32_t& how_many()
    {
      static int32_t count = 0;
      return count;
    }
  };
}

#endif

