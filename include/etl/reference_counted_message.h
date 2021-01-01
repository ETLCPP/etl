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

    virtual etl::imessage& get_message() = 0;             ///< Get a reference to the message.
    virtual const etl::imessage& get_message() const = 0; ///< Get a const reference to the message.

    virtual etl::ireference_counted_object& get_reference_counter() = 0;             ///< Get a reference to the reference counter.
    virtual const etl::ireference_counted_object& get_reference_counter() const = 0; ///< Get a const reference to the reference counter.
  };

  //***************************************************************************
  // Reference counted message type.
  //***************************************************************************
  template <typename TMessage, typename TCounter>
  class reference_counted_message : virtual public etl::ireference_counted_message
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
      rc_object.set_reference_count(0);
    }

    //***************************************************************************
    /// Get a reference to the message.
    /// \return A reference to the message.
    //***************************************************************************
    virtual TMessage& get_message() ETL_OVERRIDE
    {
      return rc_object.get_object();
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

#if ETL_HAS_ATOMIC
  //***************************************************************************
  /// Class for creating reference counted messages using an atomic counter.
  /// \tparam TMessage The type to be reference counted.
  //***************************************************************************
  template <typename TMessage>
  class atomic_counted_message : virtual public etl::reference_counted_message<TMessage, etl::atomic_uint32_t>
  {
  public:

    //***************************************************************************
    /// Constructor.
    /// \param msg The message to count.
    //***************************************************************************
    atomic_counted_message(const TMessage& msg_)
      : reference_counted_message<TMessage, etl::atomic_uint32_t>(msg_)
    {
    }

    typedef typename reference_counted_message<TMessage, etl::atomic_uint32_t>::message_type message_type;
    typedef typename reference_counted_message<TMessage, etl::atomic_uint32_t>::counter_type counter_type;
  };
#endif

  //***************************************************************************
  // Uncounted message type.
  //***************************************************************************
  template <typename TMessage>
  class uncounted_message : virtual public etl::ireference_counted_message
  {
  public:

    typedef TMessage message_type;
    typedef void     counter_type;

    //***************************************************************************
    /// Constructor
    /// \param msg The message to count.
    //***************************************************************************
    uncounted_message(const TMessage& msg_)
      : rc_object(msg_)
    {
    }

    //***************************************************************************
    /// Get a reference to the message.
    /// \return A reference to the message.
    //***************************************************************************
    virtual TMessage& get_message() ETL_OVERRIDE
    {
      return rc_object.get_object();
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
    etl::uncounted_object<TMessage> rc_object;
  };
}

#endif
