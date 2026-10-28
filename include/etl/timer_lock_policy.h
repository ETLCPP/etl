/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl
https://www.etlcpp.com

Copyright(c) 2026 John Wellbelove

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

#ifndef ETL_TIMER_LOCK_POLICY_INCLUDED
#define ETL_TIMER_LOCK_POLICY_INCLUDED

#include "platform.h"

#if ETL_HAS_ATOMIC
  #include "atomic.h"
#endif

namespace etl
{
  namespace timer
  {
    //***************************************************************************
    /// Interface for timer lock policy
    //***************************************************************************
    class lock_policy
    {
    public:

      virtual ~lock_policy() ETL_NOEXCEPT {}

      virtual void lock() ETL_NOEXCEPT      = 0;
      virtual void unlock() ETL_NOEXCEPT    = 0;
      virtual bool is_locked() ETL_NOEXCEPT = 0;
    };

    //***************************************************************************
    /// No lock policy for timers.
    //***************************************************************************
    class no_lock_policy : public lock_policy
    {
    public:

      void lock() ETL_NOEXCEPT ETL_OVERRIDE {}

      void unlock() ETL_NOEXCEPT ETL_OVERRIDE {}

      bool is_locked() ETL_NOEXCEPT ETL_OVERRIDE
      {
        return false;
      }
    };

#if ETL_HAS_ATOMIC
    //***************************************************************************
    /// Atomic lock policy for timers.
    //***************************************************************************
    class atomic_lock_policy : public lock_policy
    {
    public:

      void lock() ETL_NOEXCEPT ETL_OVERRIDE
      {
        lock_flag.store(true, etl::memory_order_seq_cst);
      }

      void unlock() ETL_NOEXCEPT ETL_OVERRIDE
      {
        lock_flag.store(false, etl::memory_order_seq_cst);
      }

      bool is_locked() ETL_NOEXCEPT ETL_OVERRIDE
      {
        return lock_flag.load(etl::memory_order_seq_cst);
      }

    private:

      etl::atomic_bool lock_flag;
    };
  }
}
#endif
#endif
