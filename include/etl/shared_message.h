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

#ifndef ETL_SHARED_MESSAGE_INCLUDED
#define ETL_SHARED_MESSAGE_INCLUDED

#include "platform.h"
#include "message_pool.h"
#include "message.h"

//*****************************************************************************
/// A wrapper for reference countable messages.
/// Contains pointers to a pool owner and a message defined with a ref count type.
/// See etl::message template.
//*****************************************************************************
namespace etl
{
  class shared_message
  {
  public:

    //*************************************************************************
    /// Constructor
    /// \param owner_   A reference to the owner pool.
    /// \param pmessage A reference to the message allocated from the pool.
    //*************************************************************************
    shared_message(etl::message_pool& owner_, etl::ireference_counted_message& message_)
      : p_message_owner(&owner_)
      , p_message(&message_)
    {
      p_message->set_reference_count(1U);
    }

    //*************************************************************************
    /// Copy constructor
    //*************************************************************************
    shared_message(const shared_message& other)
      : p_message_owner(other.p_message_owner)
      , p_message(other.p_message)
    {
      p_message->increment_reference_count();
    }

    //*************************************************************************
    /// Copy assignment operator
    //*************************************************************************
    shared_message& operator =(const shared_message& other)
    {
      if (&other != this)
      {
        // Deal with the current message.        
        if (p_message->decrement_reference_count() == 0U)
        {
          p_message_owner->destroy(p_message);
        }

        // Copy over the new one.
        p_message_owner = other.p_message_owner;
        p_message       = other.p_message;
        p_message->increment_reference_count();
      }

      return *this;
    }

    //*************************************************************************
    /// Destructor
    /// Returns the message back to the pool it it is the last copy.
    //*************************************************************************
    ~shared_message()
    {
      if (p_message->decrement_reference_count() == 0U)
      {
        p_message_owner->destroy(p_message);
      }
    }

    //*************************************************************************
    /// Get a reference to the contained message.
    //*************************************************************************
    etl::imessage& get_message()
    {
      return *p_message;
    }

    //*************************************************************************
    /// Get a const reference to the contained message.
    //*************************************************************************
    const etl::imessage& get_message() const
    {
      return *p_message;
    }

    //*************************************************************************
    /// Get the current reference count for this pool message.
    //*************************************************************************
    uint32_t get_reference_count() const
    {
      return p_message->get_reference_count();
    }

  private:

    etl::message_pool* p_message_owner;
    etl::ireference_counted_message* p_message;
  };
}

#endif

