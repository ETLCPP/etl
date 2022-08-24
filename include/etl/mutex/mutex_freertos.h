/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl
https://www.etlcpp.com

Copyright(c) 2020 Phil Wise phil@phil-wise.com

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

#ifndef ETL_MUTEX_FREERTOS_INCLUDED
#define ETL_MUTEX_FREERTOS_INCLUDED

#include "../platform.h"

#include "FreeRTOS.h"
#include <semphr.h>

namespace etl
{
  //***************************************************************************
  ///\ingroup mutex
  ///\brief This mutex class is implemented using FreeRTOS's mutexes
  //***************************************************************************
  class mutex
  {
  public:

    mutex()
    {
      access = xSemaphoreCreateMutexStatic(&mutex_allocation);
    }

    void lock()
    {
      xSemaphoreTake(access, portMAX_DELAY); // portMAX_DELAY=block forever
    }

    bool try_lock()
    {
      return xSemaphoreTake(access, 0) == pdTRUE;
    }

    void unlock()
    {
      xSemaphoreGive(access);
    }

   private:
    // Non-copyable
    mutex(const mutex&) ETL_DELETE;
    mutex& operator=(const mutex&) ETL_DELETE;

    // Memory to hold the mutex
    StaticSemaphore_t mutex_allocation;
  
    // The mutex handle itself
    SemaphoreHandle_t access;
  };
}

#endif
