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

#ifndef ETL_INSTANCE_COUNT_INCLUDED
#define ETL_INSTANCE_COUNT_INCLUDED

#include "platform.h"

#include <stdint.h>

///\defgroup instance_count instance count
///\ingroup utilities

namespace etl
{
  //***************************************************************************
  /// Inherit from this to count instances of a type.
  ///\ingroup reference
  //***************************************************************************
  template <typename T, typename TCounter = int32_t>
  class instance_count
  {
  public:

    typedef T        type;
    typedef TCounter counter_type;

    //*************************************************************************
    /// Construct and add 1.
    //*************************************************************************
    instance_count()
    {
      ++current_instance_count();
    }

    //*************************************************************************
    /// Copy construct and add 1.
    //*************************************************************************
    instance_count(const instance_count&)
    {
      ++current_instance_count();
    }

    //*************************************************************************
    /// Assignment operator.
    //*************************************************************************
    instance_count& operator =(const instance_count&)
    {
      return *this;
    }

    //*************************************************************************
    /// Destruct and subtract 1.
    //*************************************************************************
    ~instance_count()
    {
      --current_instance_count();
    }

    //*************************************************************************
    /// Get how many instances we have.
    //*************************************************************************
    static const counter_type& get_instance_count()
    {
      return current_instance_count();
    }

    //*************************************************************************
    /// Get how many instances we have.
    //*************************************************************************
    static void reset_instance_count()
    {
      current_instance_count() = 0;
    }

  private:

    //*************************************************************************
    /// Get a reference to the count.
    //*************************************************************************
    static counter_type& current_instance_count()
    {
      static counter_type counter = { 0 };
      return counter;
    }
  };
}

#endif

