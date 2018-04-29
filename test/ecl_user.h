/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl
https://www.etlcpp.com

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

#ifndef ECL_USER_INCLUDED
#define ECL_USER_INCLUDED

#include <stdint.h>

extern uint32_t timer_semaphore;

#if defined(_MSC_VER)
  #include <Windows.h>

  #define ECL_TIMER_DISABLE_PROCESSING InterlockedIncrement((volatile long*)&timer_semaphore)
  #define ECL_TIMER_ENABLE_PROCESSING  InterlockedDecrement((volatile long*)&timer_semaphore)
  #define ECL_TIMER_PROCESSING_ENABLED (InterlockedAdd((volatile long*)&timer_semaphore, 0) == 0)
#else
  #define ECL_TIMER_DISABLE_PROCESSING __sync_fetch_and_add(&timer_semaphore, 1)
  #define ECL_TIMER_ENABLE_PROCESSING  __sync_fetch_and_sub(&timer_semaphore, 1)
  #define ECL_TIMER_PROCESSING_ENABLED (__sync_fetch_and_add(&timer_semaphore, 0) == 0)
#endif

#endif
