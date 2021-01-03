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

#ifndef ETL_REFERENCE_COUNTED_MESSAGE_POOL_INCLUDED
#define ETL_REFERENCE_COUNTED_MESSAGE_POOL_INCLUDED

#include "platform.h"
#include "message.h"
#include "ireference_counted_message_pool.h"
#include "reference_counted_message.h"
#include "static_assert.h"
#include "error_handler.h"
#include "utility.h"
#include "memory.h"

#define ETL_FILE "57"

namespace etl
{
  //***************************************************************************
  ///
  //***************************************************************************
  class reference_counted_message_pool_allocation_exception : public etl::exception
  {
  public:

    reference_counted_message_pool_allocation_exception(string_type reason_, string_type file_name_, numeric_type line_number_)
      : exception(reason_, file_name_, line_number_)
    {
    }
  };

  //***************************************************************************
  ///
  //***************************************************************************
  class reference_counted_message_pool_allocation_failure : etl::reference_counted_message_pool_allocation_exception
  {
  public:

    reference_counted_message_pool_allocation_failure(string_type file_name_, numeric_type line_number_)
      : reference_counted_message_pool_allocation_exception(ETL_ERROR_TEXT("reference_counted_message_pool:allocation failure", ETL_FILE"A"), file_name_, line_number_)
    {
    }
  };

  //***************************************************************************
  /// A pool for allocating reference counted messages.
  //***************************************************************************
  template <typename TCounter>
  class reference_counted_message_pool : public ireference_counted_message_pool
  {
  public:

    //*************************************************************************
    /// Constructor
    //*************************************************************************
    reference_counted_message_pool(imemory_block_pool& memory_block_pool_)
      : memory_block_pool(memory_block_pool_)
    {
    }

    //*************************************************************************
    /// Allocate a reference counted message from the pool.
    //*************************************************************************
    template <typename TMessage>
    etl::ireference_counted_message* allocate(const TMessage& message)
    {
      ETL_STATIC_ASSERT((etl::is_base_of<etl::imessage, TMessage>::value), "Not a message type");

      typedef etl::reference_counted_message<TMessage, TCounter> rcm_t;
      typedef rcm_t* prcm_t;

      prcm_t p = ETL_NULLPTR;

      if (sizeof(rcm_t) <= memory_block_pool.get_memory_block_size())
      {
        p = static_cast<prcm_t>(memory_block_pool.allocate_memory_block(sizeof(rcm_t)));

        if (p != ETL_NULLPTR)
        {
          ::new(p) rcm_t(message, *this);
        }
      }

      ETL_ASSERT((p != ETL_NULLPTR), ETL_ERROR(etl::message_pool_allocation_failure));

      return p;
    }

    //*************************************************************************
    /// Destruct a message and send it back to the pool.
    //*************************************************************************
    void release(const etl::ireference_counted_message* const p_rcmessage)
    {
      if (p_rcmessage != ETL_NULLPTR)
      {
        p_rcmessage->~ireference_counted_message();
        memory_block_pool.release_memory_block(p_rcmessage);
      }
    }

    //*************************************************************************
    /// Destruct a message and send it back to the pool.
    //*************************************************************************
    void release(const etl::ireference_counted_message& msg)
    {
      release(&msg);
    }

  private:

    /// The raw memory block pool.
    imemory_block_pool& memory_block_pool;

    // Should not be copied.
    reference_counted_message_pool(const reference_counted_message_pool&) ETL_DELETE;
    reference_counted_message_pool& operator =(const reference_counted_message_pool&) ETL_DELETE;
  };
}

#undef ETL_FILE

#endif
