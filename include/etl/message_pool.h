///\file

/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl
https://www.etlcpp.com

Copyright(c) 2020 jwellbelove

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

#ifndef ETL_MESSAGE_POOL_INCLUDED
#define ETL_MESSAGE_POOL_INCLUDED

#include "platform.h"
#include "message.h"
#include "static_assert.h"
#include "error_handler.h"
#include "utility.h"

namespace etl
{
  //***************************************************************************
  ///
  //***************************************************************************
  class message_pool_allocation_exception : public etl::exception
  {
  public:

    message_pool_allocation_exception(string_type reason_, string_type file_name_, numeric_type line_number_)
      : exception(reason_, file_name_, line_number_)
    {
    }
  };

  //***************************************************************************
  ///
  //***************************************************************************
  class message_pool_allocation_failure : etl::message_pool_allocation_exception
  {
  public:

    message_pool_allocation_failure(string_type file_name_, numeric_type line_number_)
      : message_pool_allocation_exception(ETL_ERROR_TEXT("message_pool:allocation failure", ETL_FILE"A"), file_name_, line_number_)
    {
    }
  };

  //***************************************************************************
  ///
  //***************************************************************************
  class message_pool
  {
  public:

    //*************************************************************************
    /// Constructor
    //*************************************************************************
    message_pool(imemory_block_pool& memory_block_pool_)
      : memory_block_pool(memory_block_pool_)
    {
    }

#if ETL_CPP11_SUPPORTED && !defined(ETL_MESSAGE_POOL_FORCE_CPP03)
    //*************************************************************************
    /// Create a message from the pool.
    //*************************************************************************
    template <typename TMessage, typename... TArgs>
    TMessage* create(TArgs&&... args)
    {
      ETL_STATIC_ASSERT((etl::is_base_of<etl::imessage, TMessage>::value), "Not a message type");

      void* p = ETL_NULLPTR;

      if (sizeof(TMessage) <= memory_block_pool.get_memory_block_size())
      {
        p = memory_block_pool.allocate_memory_block(sizeof(TMessage));

        if (p != ETL_NULLPTR)
        {
          ::new(p) TMessage(etl::forward<Args>(args)...);
        }
        else
        {
          ETL_ALWAYS_ASSERT(ETL_ERROR(etl::message_pool_allocation_failure));
        }
      }

      return reinterpret_cast<TMessage*>(p);
    }
#else
    //*************************************************************************
    /// Create a message from the pool. No parameters.
    //*************************************************************************
    template <typename TMessage>
    TMessage* create()
    {
      ETL_STATIC_ASSERT((etl::is_base_of<etl::imessage, TMessage>::value), "Not a message type");

      void* p = ETL_NULLPTR;

      if (sizeof(TMessage) <= memory_block_pool.get_memory_block_size())
      {
        p = memory_block_pool.allocate_memory_block(sizeof(TMessage));

        if (p != ETL_NULLPTR)
        {
          ::new(p) TMessage();
        }
        else
        {
          ETL_ALWAYS_ASSERT(ETL_ERROR(etl::message_pool_allocation_failure));
        }
      }

      return reinterpret_cast<TMessage*>(p);
    }

    //*************************************************************************
    /// Create a message from the pool. One parameter.
    //*************************************************************************
    template <typename TMessage, typename T1>
    TMessage* create(const T1& t1)
    {
      ETL_STATIC_ASSERT(etl::is_base_of<etl::imessage, TMessage>::value, "Not a message type");

      void* p = ETL_NULLPTR;

      if (sizeof(TMessage) <= memory_block_pool.get_memory_block_size())
      {
        p = memory_block_pool.allocate_memory_block(sizeof(TMessage));

        if (p != ETL_NULLPTR)
        {
          ::new(p) TMessage(t1);
        }
        else
        {
          ETL_ALWAYS_ASSERT(ETL_ERROR(etl::message_pool_allocation_failure));
        }
      }

      return reinterpret_cast<TMessage*>(p);
    }

    //*************************************************************************
    /// Create a message from the pool. Two parameters.
    //*************************************************************************
    template <typename TMessage, typename T1, typename T2>
    TMessage* create(const T1& t1, const T2& t2)
    {
      ETL_STATIC_ASSERT(etl::is_base_of<etl::imessage, TMessage>::value, "Not a message type");

      void* p = ETL_NULLPTR;

      if (sizeof(TMessage) <= memory_block_pool.get_memory_block_size())
      {
        p = memory_block_pool.allocate_memory_block(sizeof(TMessage));

        if (p != ETL_NULLPTR)
        {
          ::new(p) TMessage(t1, t2);
        }
        else
        {
          ETL_ALWAYS_ASSERT(ETL_ERROR(etl::message_pool_allocation_failure));
        }
      }

      return reinterpret_cast<TMessage*>(p);
    }

    //*************************************************************************
    /// Create a message from the pool. Three parameters.
    //*************************************************************************
    template <typename TMessage, typename T1, typename T2, typename T3>
    TMessage* create(const T1& t1, const T2& t2, const T3& t3)
    {
      ETL_STATIC_ASSERT(etl::is_base_of<etl::imessage, TMessage>::value, "Not a message type");

      void* p = ETL_NULLPTR;

      if (sizeof(TMessage) <= memory_block_pool.get_memory_block_size())
      {
        p = memory_block_pool.allocate_memory_block(sizeof(TMessage));

        if (p != ETL_NULLPTR)
        {
          ::new(p) TMessage(t1, t2, t3);
        }
        else
        {
          ETL_ALWAYS_ASSERT(ETL_ERROR(etl::message_pool_allocation_failure));
        }
      }
      
      return reinterpret_cast<TMessage*>(p);
    }

    //*************************************************************************
    /// Create a message from the pool. Four parameters.
    //*************************************************************************
    template <typename TMessage, typename T1, typename T2, typename T3, typename T4>
    TMessage* create(const T1& t1, const T2& t2, const T3& t3, const T4& t4)
    {
      ETL_STATIC_ASSERT(etl::is_base_of<etl::imessage, TMessage>::value, "Not a message type");

      void* p = ETL_NULLPTR;

      if (sizeof(TMessage) <= memory_block_pool.get_memory_block_size())
      {
        p = memory_block_pool.allocate_memory_block(sizeof(TMessage));

        if (p != ETL_NULLPTR)
        {
          ::new(p) TMessage(t1, t2, t3, t4);
        }
        else
        {
          ETL_ALWAYS_ASSERT(ETL_ERROR(etl::message_pool_allocation_failure));
        }
      }

      return reinterpret_cast<TMessage*>(p)
    }
#endif

    //*************************************************************************
    /// Destruct a message and send it back to the pool.
    //*************************************************************************
    void destroy(const etl::imessage* const pmsg)
    {
      if (pmsg != ETL_NULLPTR)
      {
        pmsg->~imessage();
        memory_block_pool.release_memory_block(pmsg);
      }
    }

    //*************************************************************************
    /// Destruct a message and send it back to the pool.
    //*************************************************************************
    void destroy(const etl::imessage& msg)
    {
      destroy(&msg);
    }

  private:

    /// The raw memory block pool.
    imemory_block_pool& memory_block_pool;

    // Should not be copied.
    message_pool(const message_pool&) ETL_DELETE;
    message_pool& operator =(const message_pool&) ETL_DELETE;
  };
}

#undef ETL_FILE

#endif

