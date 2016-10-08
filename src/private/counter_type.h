///\file

/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl
http://www.etlcpp.com

Copyright(c) 2016 jwellbelove

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

#ifndef __ETL_COUNTER_TYPE__
#define __ETL_COUNTER_TYPE__

#include <stddef.h>

#include "../intrusive_links.h"

namespace etl
{
  //*************************************************************************
  /// Counter type based on intrusive link count option.
  //*************************************************************************
  template <const size_t OPTION>
  class counter_type
  {
  };

  //*************************************************************************
  /// Slow type.
  //*************************************************************************
  template <>
  class counter_type<etl::count_option::SLOW_COUNT>
  {
  public:

    counter_type& operator ++()
    {
      return *this;
    }

    counter_type& operator --()
    {
      return *this;
    }

    counter_type& operator =(size_t new_count)
    {
      return *this;
    }

    counter_type& operator +=(size_t diff)
    {
      return *this;
    }

    counter_type& operator -=(size_t diff)
    {
      return *this;
    }

    size_t get_count() const
    {
      return 0;
    }
  };

  //*************************************************************************
  /// Fast type.
  //*************************************************************************
  template <>
  class counter_type<etl::count_option::FAST_COUNT>
  {
  public:

    counter_type()
      : count(0)
    {
    }

    counter_type& operator ++()
    {
      ++count;
      return *this;
    }

    counter_type& operator --()
    {
      --count;
      return *this;
    }

    counter_type& operator =(size_t new_count)
    {
      count = new_count;
      return *this;
    }

    counter_type& operator +=(size_t diff)
    {
      count += diff;
      return *this;
    }

    counter_type& operator -=(size_t diff)
    {
      count -= diff;
      return *this;
    }

    size_t get_count() const
    {
      return count;
    }

    size_t count;
  };
}

#endif
