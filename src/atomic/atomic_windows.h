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

#ifndef __ETL_ATOMIC_WINDOWS__
#define __ETL_ATOMIC_WINDOWS__

#include "../platform.h"

#include <stdint.h>
#include <Windows.h>

namespace etl
{
  class atomic_uint32_t
  {
  public:

    atomic_uint32_t()
    {
      InterlockedExchange(&value, 0);
    }

    atomic_uint32_t(uint32_t v)
    {
      InterlockedExchange(&value, v);
    }

    atomic_uint32_t& operator =(uint32_t v)
    {
      InterlockedExchange(&value, v);

      return *this;
    }

    atomic_uint32_t& operator ++()
    {
      InterlockedIncrement(&value);

      return *this;
    }

    volatile atomic_uint32_t& operator ++() volatile
    {
      InterlockedIncrement(&value);

      return *this;
    }

    atomic_uint32_t& operator --()
    {
      InterlockedDecrement(&value);

      return *this;
    }

    volatile atomic_uint32_t& operator --() volatile
    {
      InterlockedDecrement(&value);

      return *this;
    }

    operator uint32_t () const
    {
      return InterlockedAdd((volatile long*)&value, 0);
    }

    operator uint32_t() volatile const
    {
      return InterlockedAdd((volatile long*)&value, 0);
    }

  private:

    uint32_t value;
  };
}

#endif
