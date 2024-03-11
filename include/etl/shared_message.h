///\file

/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl
https://www.etlcpp.com

Copyright(c) 2020 John Wellbelove

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

#ifndef ETL_SHARED_MESSAGE_INCLUDED
#define ETL_SHARED_MESSAGE_INCLUDED

#include "platform.h"
#include "utility.h"
#include "reference_counted_message.h"
#include "ireference_counted_message_pool.h"
#include "message.h"
#include "type_traits.h"
#include "static_assert.h"

//*****************************************************************************
/// A wrapper for reference counted messages.
/// Contains pointers to a pool owner and a message defined with a ref count type.
//*****************************************************************************
namespace etl
{
  class shared_message
  {
  public:

#if ETL_USING_CPP11
    //*************************************************************************
    /// Creator for in-place instantiation
    //*************************************************************************
    template <typename TMessage, typename TPool, typename... TArgs>
    static shared_message create(TPool& owner, TArgs&&... args)
    {
      return shared_message(owner, etl::in_place_type_t<TMessage>(), etl::forward<TArgs>(args)...);
    }
#endif

    //*************************************************************************
    /// Constructor
    //*************************************************************************
    template <typename TPool, typename TMessage>
    shared_message(TPool& owner, const TMessage& message)
    {
      ETL_STATIC_ASSERT((etl::is_base_of<etl::ireference_counted_message_pool, TPool>::value), "TPool not derived from etl::ireference_counted_message_pool");
      ETL_STATIC_ASSERT((etl::is_base_of<etl::imessage, TMessage>::value), "TMessage not derived from etl::imessage");

      p_rcmessage = owner.allocate(message);

      if (p_rcmessage != ETL_NULLPTR)
      {
        p_rcmessage->get_reference_counter().set_reference_count(1U);
      }
    }

#if ETL_USING_CPP11
    //*************************************************************************
    /// Constructor
    //*************************************************************************
    template <typename TPool, typename TMessage, typename... TArgs>
    shared_message(TPool& owner, etl::in_place_type_t<TMessage>, TArgs&&... args)
    {
      ETL_STATIC_ASSERT((etl::is_base_of<etl::ireference_counted_message_pool, TPool>::value), "TPool not derived from etl::ireference_counted_message_pool");
      ETL_STATIC_ASSERT((etl::is_base_of<etl::imessage, TMessage>::value), "TMessage not derived from etl::imessage");

      p_rcmessage = owner.template allocate<TMessage>(etl::forward<TArgs>(args)...);

      if (p_rcmessage != ETL_NULLPTR)
      {
        p_rcmessage->get_reference_counter().set_reference_count(1U);
      }
    }
#endif

    //*************************************************************************
    /// Constructor
    //*************************************************************************
    shared_message(etl::ireference_counted_message& rcm)
    {
      p_rcmessage = &rcm;

      p_rcmessage->get_reference_counter().set_reference_count(1U);
    }

    //*************************************************************************
    /// Copy constructor
    //*************************************************************************
    shared_message(const etl::shared_message& other)
      : p_rcmessage(other.p_rcmessage)
    {
      p_rcmessage->get_reference_counter().increment_reference_count();
    }

#if ETL_USING_CPP11
    //*************************************************************************
    /// Move constructor
    //*************************************************************************
    shared_message(etl::shared_message&& other) ETL_NOEXCEPT
      : p_rcmessage(etl::move(other.p_rcmessage))
    {
      other.p_rcmessage = ETL_NULLPTR;
    }
#endif

    //*************************************************************************
    /// Copy assignment operator
    //*************************************************************************
    shared_message& operator =(const etl::shared_message& other)
    {
      if (&other != this)
      {
        // Deal with the current message.
        if (p_rcmessage->get_reference_counter().decrement_reference_count() == 0U)
        {
          p_rcmessage->release();
        }

        // Copy over the new one.
        p_rcmessage = other.p_rcmessage;
        p_rcmessage->get_reference_counter().increment_reference_count();
       }

      return *this;
    }

#if ETL_USING_CPP11
    //*************************************************************************
    /// Move assignment operator
    //*************************************************************************
    shared_message& operator =(etl::shared_message&& other) ETL_NOEXCEPT
    {
      if (&other != this)
      {
        // Deal with the current message.
        if (p_rcmessage->get_reference_counter().decrement_reference_count() == 0U)
        {
          p_rcmessage->release();
        }

        // Move over the new one.
        p_rcmessage = etl::move(other.p_rcmessage);
        other.p_rcmessage = ETL_NULLPTR;
      }

      return *this;
    }
#endif

    //*************************************************************************
    /// Destructor
    /// Returns the message back to the pool it it is the last copy.
    //*************************************************************************
    ~shared_message()
    {
      if ((p_rcmessage != ETL_NULLPTR) &&
          (p_rcmessage->get_reference_counter().decrement_reference_count() == 0U))
      {       
        p_rcmessage->release();
      }
    }

    //*************************************************************************
    /// Get a reference to the contained message.
    //***********************************************************************
    ETL_NODISCARD etl::imessage& get_message()
    {
      return p_rcmessage->get_message();
    }

    //*************************************************************************
    /// Get a const reference to the contained message.
    //*************************************************************************
    ETL_NODISCARD const etl::imessage& get_message() const
    {
      return p_rcmessage->get_message();
    }

    //*************************************************************************
    /// Get the current reference count for this shared message.
    //*************************************************************************
    ETL_NODISCARD uint32_t get_reference_count() const
    {
      return p_rcmessage->get_reference_counter().get_reference_count();
    }

    //*************************************************************************
    /// Checks if the shared message is valid.
    //*************************************************************************
    ETL_NODISCARD bool is_valid() const
    {
      return p_rcmessage != ETL_NULLPTR;
    }

  private:

    shared_message() ETL_DELETE;

    etl::ireference_counted_message* p_rcmessage; ///< A pointer to the reference  counted message.
  };
}

#endif