/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl
https://www.etlcpp.com

Copyright(c) 2018 John Wellbelove

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

#ifndef ETL_MUTEX_INCLUDED
#define ETL_MUTEX_INCLUDED

#include "platform.h"

#if ETL_USING_STL && ETL_USING_CPP11
  #include "mutex/mutex_std.h"
  #define ETL_HAS_MUTEX 1
#elif defined(ETL_TARGET_OS_CMSIS_OS2)
  #include "mutex/mutex_cmsis_os2.h"
  #define ETL_HAS_MUTEX 1
#elif defined(ETL_TARGET_OS_FREERTOS)
  #include "mutex/mutex_freertos.h"
  #define ETL_HAS_MUTEX 1
#elif defined(ETL_COMPILER_ARM5) || defined(ETL_COMPILER_ARM6) || defined(ETL_COMPILER_ARM7) || defined(ETL_COMPILER_ARM8)
  #include "mutex/mutex_arm.h"
  #define ETL_HAS_MUTEX 1
#elif defined(ETL_COMPILER_GCC)
  #include "mutex/mutex_gcc_sync.h"
  #define ETL_HAS_MUTEX 1
#elif defined(ETL_COMPILER_CLANG)
  #include "mutex/mutex_clang_sync.h"
  #define ETL_HAS_MUTEX 1
#else
  #define ETL_HAS_MUTEX 0
#endif

namespace etl
{
  namespace traits
  {
    static ETL_CONSTANT bool has_mutex = (ETL_HAS_MUTEX == 1);
  }

  //***************************************************************************
  /// lock_guard
  /// A mutex wrapper that provides an RAII mechanism for owning a mutex for 
  /// the duration of a scoped block.
  //***************************************************************************
  template <typename TMutex>
  class lock_guard
  {
  public:

    typedef TMutex mutex_type;

    //*****************************************************
    /// Constructor
    /// Locks the mutex.
    //*****************************************************
    explicit lock_guard(mutex_type& m_)
      : m(m_)
    {
      m.lock();
    }

    //*****************************************************
    /// Destructor
    //*****************************************************
    ~lock_guard()
    {
      m.unlock();
    }

  private:

    // Deleted.
    lock_guard(const lock_guard&) ETL_DELETE;

    mutex_type& m;
  };

}

#endif
