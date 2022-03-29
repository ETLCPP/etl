/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl
https://www.etlcpp.com

Copyright(c) 2021 John Wellbelove

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

#ifndef ETL_MUTEX_CMSIS_RTOS2_INCLUDED
#define ETL_MUTEX_CMSIS_RTOS2_INCLUDED

#include "../platform.h"

#include <cmsis_os2.h>

namespace etl
{
  //***************************************************************************
  ///\ingroup mutex
  ///\brief This mutex class is implemented using CMSIS's RTOS2 mutexes
  //***************************************************************************
  class mutex
  {
  public:

    mutex()
      : id(0)
    {
      osMutexAttr_t attr = { "ETL", osMutexRecursive | osMutexPrioInherit | osMutexRobust, 0, 0 };
      id = osMutexNew(&attr);
    }

    void lock()
    {
      osMutexAcquire(id, osWaitForever);
    }

    bool try_lock()
    {
      return osMutexAcquire(id, 0) == osOK;
    }

    void unlock()
    {
      osMutexRelease(id);
    }

  private:

    mutex(const mutex&) ETL_DELETE;
    mutex& operator=(const mutex&) ETL_DELETE;

    osMutexId_t id;
  };
}

#endif
