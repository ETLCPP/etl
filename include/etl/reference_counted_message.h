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

#ifndef ETL_REFERENCE_COUNTED_MESSAGE_INCLUDED
#define ETL_REFERENCE_COUNTED_MESSAGE_INCLUDED

#include <stdint.h>

#include "platform.h"
#include "message.h"
#include "atomic.h"
#include "reference_counted_object.h"

namespace etl
{
  //***************************************************************************
  // Base class for all reference counted messages.
  //***************************************************************************
  class ireference_counted_message
  {
  public:

    virtual const etl::imessage& get_message() const = 0; ///< Get a const reference to the message.

    virtual etl::ireference_counted_object& get_reference_counter() = 0;             ///< Get a reference to the reference counter.
    virtual const etl::ireference_counted_object& get_reference_counter() const = 0; ///< Get a const reference to the reference counter.
  };

  //***************************************************************************
  // Reference counted message type.
  //***************************************************************************
  template <typename TMessage, typename TCounter>
  class reference_counted_message : public etl::ireference_counted_message
  {
  public:

    typedef TMessage message_type;
    typedef TCounter counter_type;

    //***************************************************************************
    /// Constructor
    /// \param msg The message to count.
    //***************************************************************************
    reference_counted_message(const TMessage& msg_)
      : rc_object(msg_)
    {
    }

    //***************************************************************************
    /// Get a const reference to the message.
    /// \return A const reference to the message.
    //***************************************************************************
    virtual const TMessage& get_message() const ETL_OVERRIDE
    {
      return rc_object.get_object();
    }

    //***************************************************************************
    /// Get a reference to the reference counter.
    /// \return A reference to the reference counter.
    //***************************************************************************
    virtual etl::ireference_counted_object& get_reference_counter() ETL_OVERRIDE
    {
      return rc_object;
    }

    //***************************************************************************
    /// Get a const reference to the reference counter.
    /// \return A const reference to the reference counter.
    //***************************************************************************
    virtual const etl::ireference_counted_object& get_reference_counter() const ETL_OVERRIDE
    {
      return rc_object;
    }

  private:

    /// The reference counted object.
    etl::reference_counted_object<TMessage, TCounter> rc_object;
  };

  //***************************************************************************
  /// Persistent message type.
  /// The message type will always have a reference count of 1. 
  /// \tparam TMessage  The message type stored.
  //***************************************************************************
  template <typename TMessage>
  class persistent_message : virtual public etl::ireference_counted_message
  {
  public:

    typedef TMessage message_type;

    //***************************************************************************
    /// Constructor
    /// \param msg The message to count.
    //***************************************************************************
    persistent_message(const TMessage& msg_)
      : rc_object(msg_)
    {
    }

    //***************************************************************************
    /// Get a const reference to the message.
    /// \return A const reference to the message.
    //***************************************************************************
    virtual const TMessage& get_message() const ETL_OVERRIDE
    {
      return rc_object.get_object();
    }

    //***************************************************************************
    /// Get a reference to the reference counter.
    /// \return A reference to the reference counter.
    //***************************************************************************
    virtual etl::ireference_counted_object& get_reference_counter() ETL_OVERRIDE
    {
      return rc_object;
    }

    //***************************************************************************
    /// Get a const reference to the reference counter.
    /// \return A const reference to the reference counter.
    //***************************************************************************
    virtual const etl::ireference_counted_object& get_reference_counter() const ETL_OVERRIDE
    {
      return rc_object;
    }

  private:

    // This class must not be default contructed, copy constructed or assigned.
    persistent_message() ETL_DELETE;
    persistent_message(const persistent_message&) ETL_DELETE;
    persistent_message& operator =(const persistent_message&) ETL_DELETE;

    /// The reference counted object.
    etl::persistent_object<TMessage> rc_object;
  };

#if ETL_CPP11_SUPPORTED && ETL_HAS_ATOMIC
  //***************************************************************************
  /// Class for creating reference counted objects using an atomic counter.
  /// \tparam TObject  The type to be reference counted.
  //***************************************************************************
  template <typename TMessage>
  using atomic_counted_message = etl::reference_counted_message<TMessage, etl::atomic_int32_t>;
#endif
}

#endif
