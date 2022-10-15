/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl
https://www.etlcpp.com

Copyright(c) 2021 John Wellbelove

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

#include "platform.h"
#include "message.h"
#include "atomic.h"
#include "reference_counted_object.h"
#include "static_assert.h"
#include "type_traits.h"
#include "ireference_counted_message_pool.h"

#include <stdint.h>

namespace etl
{
  //***************************************************************************
  // Base class for all reference counted messages.
  //***************************************************************************
  class ireference_counted_message
  {
  public:

    virtual ~ireference_counted_message() {}
    ETL_NODISCARD virtual etl::imessage& get_message() = 0;                                 ///< Get a reference to the message.
    ETL_NODISCARD virtual const etl::imessage& get_message() const = 0;                     ///< Get a const reference to the message.
    ETL_NODISCARD virtual etl::ireference_counter& get_reference_counter() = 0;             ///< Get a reference to the reference counter.
    ETL_NODISCARD virtual const etl::ireference_counter& get_reference_counter() const = 0; ///< Get a const reference to the reference counter.
    virtual void release() = 0;                                                             ///< Release back to the owner.
  };

  //***************************************************************************
  // Reference counted message with a counter and owner.
  //***************************************************************************
  template <typename TMessage, typename TCounter>
  class reference_counted_message : public etl::ireference_counted_message
  {
  public:

    ETL_STATIC_ASSERT((etl::is_base_of<etl::imessage, TMessage>::value), "Not a message type");

    typedef TMessage message_type;
    typedef TCounter counter_type;

    //***************************************************************************
    /// Constructor
    /// \param owner The message owner.
    //***************************************************************************
    reference_counted_message(etl::ireference_counted_message_pool& owner_)
      : owner(owner_)
    {
    }

    //***************************************************************************
    /// Constructor
    /// \param msg   The message to count.
    /// \param owner The message owner.
    //***************************************************************************
    reference_counted_message(const TMessage& msg_, etl::ireference_counted_message_pool& owner_)
      : rc_object(msg_)
      , owner(owner_)
    {
    }

    //***************************************************************************
    /// Get a reference to the message.
    /// \return A reference to the message.
    //***************************************************************************
    ETL_NODISCARD virtual TMessage& get_message() ETL_OVERRIDE
    {
      return rc_object.get_object();
    }

    //***************************************************************************
    /// Get a const reference to the message.
    /// \return A const reference to the message.
    //***************************************************************************
    ETL_NODISCARD virtual const TMessage& get_message() const ETL_OVERRIDE
    {
      return rc_object.get_object();
    }

    //***************************************************************************
    /// Get a reference to the reference counter.
    /// \return A reference to the reference counter.
    //***************************************************************************
    ETL_NODISCARD virtual etl::ireference_counter& get_reference_counter() ETL_OVERRIDE
    {
      return rc_object.get_reference_counter();
    }

    //***************************************************************************
    /// Get a const reference to the reference counter.
    /// \return A const reference to the reference counter.
    //***************************************************************************
    ETL_NODISCARD virtual const etl::ireference_counter& get_reference_counter() const ETL_OVERRIDE
    {
      return rc_object.get_reference_counter();
    }

    //***************************************************************************
    /// Release back to the owner pool.
    /// \return A reference to the owner pool.
    //***************************************************************************
    virtual void release() ETL_OVERRIDE
    {
      owner.release(*this);
    }

  private:

    etl::reference_counted_object<TMessage, TCounter> rc_object; ///< The reference counted object.
    etl::ireference_counted_message_pool& owner;                 ///< The pool that owns this object.
  };

  //***************************************************************************
  // A persistent message with no counter and owner.
  //***************************************************************************
  template <typename TMessage>
  class persistent_message : public etl::ireference_counted_message
  {
  public:

    ETL_STATIC_ASSERT((etl::is_base_of<etl::imessage, TMessage>::value), "Not a message type");

    typedef TMessage message_type;
    typedef void counter_type;

    //***************************************************************************
    /// Constructor
    /// \param msg The message to count.
    //***************************************************************************
    explicit persistent_message(const TMessage& msg_)
      : rc_object(msg_)
    {
    }

    //***************************************************************************
    /// Get a reference to the message.
    /// \return A reference to the message.
    //***************************************************************************
    ETL_NODISCARD virtual TMessage& get_message() ETL_OVERRIDE
    {
      return rc_object.get_object();
    }

    //***************************************************************************
    /// Get a const reference to the message.
    /// \return A const reference to the message.
    //***************************************************************************
    ETL_NODISCARD virtual const TMessage& get_message() const ETL_OVERRIDE
    {
      return rc_object.get_object();
    }

    //***************************************************************************
    /// Get a reference to the reference counter.
    /// \return A reference to the reference counter.
    //***************************************************************************
    ETL_NODISCARD virtual etl::ireference_counter& get_reference_counter() ETL_OVERRIDE
    {
      return rc_object.get_reference_counter();
    }

    //***************************************************************************
    /// Get a const reference to the reference counter.
    /// \return A const reference to the reference counter.
    //***************************************************************************
    ETL_NODISCARD virtual const etl::ireference_counter& get_reference_counter() const ETL_OVERRIDE
    {
      return rc_object.get_reference_counter();
    }

    //***************************************************************************
    /// Does nothing for a persistent message
    /// \return A reference to the owner pool.
    //***************************************************************************
    virtual void release() ETL_OVERRIDE
    {
      // Do nothing.
    }

  private:

    etl::reference_counted_object<TMessage, void> rc_object; ///< The reference counted object.
  };

#if __cpp_alias_templates && ETL_HAS_ATOMIC
  //***************************************************************************
  /// Class for creating reference counted objects using an atomic counter.
  /// \tparam TObject  The type to be reference counted.
  //***************************************************************************
  template <typename TMessage>
  using atomic_counted_message = etl::reference_counted_message<TMessage, etl::atomic_int32_t>;
#endif
}

#endif
