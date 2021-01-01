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

#include <iostream>

#include "platform.h"
#include "reference_counted_message_pool.h"
#include "reference_counted_message.h"

//*****************************************************************************
/// A wrapper for reference counted messages.
/// Contains pointers to a pool owner and a message defined with a ref count type.
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
    shared_message(etl::ireference_counted_message& message_, etl::ireference_counted_message_pool& owner_)
      : p_message_owner(&owner_)
      , p_message(&message_)
    {
      p_message->get_reference_counter().set_reference_count(1U);
      std::cout << "shared_message  C : Reference " << int(p_message->get_message().get_message_id()) << " " << p_message->get_reference_counter().get_reference_count() << "\n";
    }

    shared_message(etl::ireference_counted_message& message_)
      : p_message_owner(ETL_NULLPTR)
      , p_message(&message_)
    {
      p_message->get_reference_counter().set_reference_count(1U);
      std::cout << "shared_message  C : Reference " << int(p_message->get_message().get_message_id()) << " No Owner\n";
    }

    //*************************************************************************
    /// Copy constructor
    //*************************************************************************
    shared_message(const shared_message& other)
      : p_message_owner(other.p_message_owner)
      , p_message(other.p_message)
    {
      p_message->get_reference_counter().increment_reference_count();
      std::cout << "shared_message CC : Reference " << int(p_message->get_message().get_message_id()) << " " << p_message->get_reference_counter().get_reference_count() << "\n";
    }

    //*************************************************************************
    /// Copy assignment operator
    //*************************************************************************
    shared_message& operator =(const shared_message& other)
    {
      if (&other != this)
      {
        // Deal with the current message.
        if (p_message->get_reference_counter().decrement_reference_count() == 0U)
        {
          std::cout << "shared_message  =: Destroy reference counted message\n";
          if (p_message_owner != ETL_NULLPTR)
          {
            p_message_owner->release(p_message);
          }
        }

        // Copy over the new one.
        p_message_owner = other.p_message_owner;
        p_message       = other.p_message;
        p_message->get_reference_counter().increment_reference_count();
        std::cout << "shared_message  =: Reference " << int(p_message->get_message().get_message_id()) << " " << p_message->get_reference_counter().get_reference_count() << "\n";
       }

      return *this;
    }

    //*************************************************************************
    /// Destructor
    /// Returns the message back to the pool it it is the last copy.
    //*************************************************************************
    ~shared_message()
    {
      if (p_message->get_reference_counter().decrement_reference_count() == 0U)
      {
        std::cout << "shared_message  ~: Destroy reference counted message\n";
        if (p_message_owner != ETL_NULLPTR)
        {
          p_message_owner->release(p_message);
        }
      }
      else
      {
        std::cout << "shared_message  ~: Reference " << int(p_message->get_message().get_message_id()) << " " << p_message->get_reference_counter().get_reference_count() << "\n";
      }
    }

    //*************************************************************************
    /// Get a pointer to the contained message.
    //*************************************************************************
    etl::imessage& get_message()
    {
      return p_message->get_message();
    }

    //*************************************************************************
    /// Get a const pointer to the contained message.
    //*************************************************************************
    const etl::imessage& get_message() const
    {
      return p_message->get_message();
    }

    //*************************************************************************
    /// Returns <b>true</b> if the shared message is owned by a pool.
    //*************************************************************************
    bool is_owned() const
    {
      return (p_message_owner != ETL_NULLPTR);
    }

    //*************************************************************************
    /// Get the current reference count for this shared message.
    //*************************************************************************
    uint32_t get_reference_count() const
    {
      return p_message->get_reference_counter().get_reference_count();
    }

  private:

    shared_message() ETL_DELETE;

    etl::ireference_counted_message_pool* p_message_owner;
    etl::ireference_counted_message*      p_message;
  };
}

#endif

