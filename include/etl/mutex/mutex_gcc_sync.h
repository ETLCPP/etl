/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl
https://www.etlcpp.com

Copyright(c) 2017 John Wellbelove

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

#ifndef ETL_MUTEX_GCC_SYNC_INCLUDED
#define ETL_MUTEX_GCC_SYNC_INCLUDED

#include "../platform.h"

#include <stdint.h>

namespace etl
{
  //***************************************************************************
  ///\ingroup mutex
  ///\brief This mutex class is implemented using GCC's __sync functions.
  //***************************************************************************
  class mutex
  {
  public:

    mutex()
      : flag(0)
    {
      __sync_lock_release(&flag);
    }

    void lock()
    {
      while (__sync_lock_test_and_set(&flag, 1U))
      {
      }
    }

    bool try_lock()
    {
      return (__sync_lock_test_and_set(&flag, 1U) == 0U);
    }

    void unlock()
    {
      __sync_lock_release(&flag);
    }

  private:

    mutex(const mutex&) ETL_DELETE;
    mutex& operator=(const mutex&) ETL_DELETE;

    char flag;
  };
}

#endif
