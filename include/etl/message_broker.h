/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl
https://www.etlcpp.com

Copyright(c) 2017 John Wellbelove

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

#ifndef ETL_MESSAGE_BROKER_INCLUDED
#define ETL_MESSAGE_BROKER_INCLUDED

#include <stdint.h>

#include "platform.h"
#include "nullptr.h"
#include "message_types.h"
#include "message.h"
#include "message_router.h"
#include "array.h"
#include "span.h"

namespace etl
{
  //***************************************************************************
  /// Base exception class for message broker
  //***************************************************************************
  class message_broker_exception : public etl::exception
  {
  public:

    message_broker_exception(string_type reason_, string_type file_name_, numeric_type line_number_)
      : etl::exception(reason_, file_name_, line_number_)
    {
    }
  };

  //***************************************************************************
  /// Too many messages.
  //***************************************************************************
  class message_broker_too_many_mesaages : public etl::message_broker_exception
  {
  public:

    message_broker_too_many_mesaages(string_type file_name_, numeric_type line_number_)
      : message_broker_exception(ETL_ERROR_TEXT("message broker:too many messages", ETL_MESSAGE_BROKER_FILE_ID"A"), file_name_, line_number_)
    {
    }
  };

  //***************************************************************************
  /// Too many subscribers.
  //***************************************************************************
  class message_broker_too_many_subscribers : public etl::message_broker_exception
  {
  public:

    message_broker_too_many_subscribers(string_type file_name_, numeric_type line_number_)
      : message_broker_exception(ETL_ERROR_TEXT("message broker:too many subscribers", ETL_MESSAGE_BROKER_FILE_ID"B"), file_name_, line_number_)
    {
    }
  };

  //***************************************************************************
  /// Message broker
  //***************************************************************************
  class message_broker : public etl::imessage_router
  {
  public:

    //*******************************************
    class subscription
    {
    public:

      friend class message_broker;

      subscription(etl::imessage_router& router_,
                   etl::message_id_t*    p_message_ids_,
                   size_t                n_messages_)
        : p_router(&router_)
        , p_message_ids(p_message_ids_)
        , p_next(ETL_NULLPTR)
        , n_messages(uint16_t(n_messages_))
      {
      }

      subscription(etl::imessage_router& router_,
                   etl::message_id_t* first,
                   etl::message_id_t* last)
        : p_router(&router_)
        , p_message_ids(first)
        , p_next(ETL_NULLPTR)
        , n_messages(uint16_t(etl::distance(first, last)))
      {
      }

      template <size_t NMessages>
      subscription(etl::imessage_router& router_,
        etl::array<etl::message_id_t, NMessages> message_ids_)
        : p_router(&router_)
        , p_message_ids(message_ids_.data())
        , p_next(ETL_NULLPTR)
        , n_messages(uint16_t(NMessages))
      {
      }

#if ETL_USING_STL && ETL_USING_CPP11
      template <size_t NMessages>
      subscription(etl::imessage_router& router_,
                   std::array<etl::message_id_t, NMessages> message_ids_)
        : p_router(&router_)
        , p_message_ids(message_ids_.data())
        , p_next(ETL_NULLPTR)
        , n_messages(uint16_t(NMessages))
      {
      }
#endif

      etl::imessage_router* router() const
      {
        return p_router;
      }

      etl::span<const etl::message_id_t> message_id_list() const
      {
        return etl::span<const etl::message_id_t>(p_message_ids, p_message_ids + n_messages);
      }

      void next(subscription* sub)
      {
        p_next = sub;
      }

      subscription* next() const
      {
        return p_next;
      }

      void terminate()
      {
        next(ETL_NULLPTR);
      }

      void append(subscription* sub)
      {
        sub->next(next());
        next(sub);
      }

      void append(subscription& sub)
      {
        append(&sub);
      }

    private:

      subscription()
        : p_router(ETL_NULLPTR)
        , p_message_ids(ETL_NULLPTR)
        , p_next(ETL_NULLPTR)
        , n_messages(0U)
      {
      }

      etl::imessage_router* const    p_router;
      const etl::message_id_t* const p_message_ids;
      subscription*                  p_next;
      const uint16_t                 n_messages;
    };

    using etl::imessage_router::receive;
    
    //*******************************************
    /// Constructor.
    //*******************************************
    message_broker()
      : imessage_router(etl::imessage_router::MESSAGE_BROKER)
      , head()
    {
    }

    //*******************************************
    /// Constructor.
    //*******************************************
    message_broker(etl::message_router_id_t id_)
      : imessage_router(id_)
      , head()
    {
    }

    //*******************************************
    /// Constructor.
    //*******************************************
    message_broker(etl::message_broker::subscription & subscription_list_)
      : imessage_router(etl::imessage_router::MESSAGE_BROKER)
      , head()
    {
    }

    //*******************************************
    /// Constructor.
    //*******************************************
    message_broker(etl::message_router_id_t id_, etl::message_broker::subscription& subscription_list_)
      : imessage_router(id_)
      , head()
    {
    }

    //*******************************************
    /// Subscribe to the broker.
    //*******************************************
    void subscribe(etl::message_broker::subscription& new_sub)
    {
      initialise_insertion_point(new_sub.router(), &new_sub);
    }

    //*******************************************
    void unsubscribe(etl::imessage_router& router)
    {
      initialise_insertion_point(&router, ETL_NULLPTR);
    }

    //*******************************************
    virtual void receive(const etl::imessage& msg) ETL_OVERRIDE
    {
      //const etl::message_id_t id = msg.get_message_id();

      //// Find the range of routers that are subscribed to this message.
      //ETL_OR_STD::pair<message_list_t, message_list_t> range = etl::equal_range(message_list_begin, message_list_end, message_element{ id, 0 }, CompareMessageElements);

      //// We have a valid range?
      //if (range.first != range.second)
      //{
      //  // Call receive() for each.
      //  for (message_list_t itr = range.first; itr != range.second; ++itr)
      //  {
      //    etl::imessage_router& router = *router_list_begin[itr->router_index];

      //    router.receive(msg);
      //  }
      //}
      //else
      //{
      //  // Pass on to the successor, if present.
      //  if (has_successor())
      //  {
      //    etl::imessage_router& successor = get_successor();

      //    if (successor.accepts(msg.get_message_id()))
      //    {
      //      successor.receive(msg);
      //    }
      //  }
      //}
    }

    //*******************************************
    virtual void receive(etl::shared_message shared_msg) ETL_OVERRIDE
    {
      //if (has_successor())
      //{
      //  etl::imessage_router& successor = get_successor();

      //  if (successor.accepts(shared_msg.get_message().get_message_id()))
      //  {
      //    successor.receive(shared_msg);
      //  }
      //}
    }

    using imessage_router::accepts;

    //*******************************************
    /// Does this message broker accept the message id?
    //*******************************************
    virtual bool accepts(etl::message_id_t id) const ETL_OVERRIDE
    {
      if (is_in_subscription_list(id))
      {
        return true;
      }
      else
      {
        if (has_successor())
        {
          return get_successor().accepts(id);
        }
        else
        {
          return false;
        }
      }
    }

    //*******************************************
    void clear()
    {
      head.next(ETL_NULLPTR);
    }

    //********************************************
    ETL_DEPRECATED virtual bool is_null_router() const ETL_OVERRIDE
    {
      return false;
    }

    //********************************************
    virtual bool is_producer() const ETL_OVERRIDE
    {
      return true;
    }

    //********************************************
    virtual bool is_consumer() const ETL_OVERRIDE
    {
      return true;
    }

    //********************************************
    bool empty() const
    {
      return head.next() == ETL_NULLPTR;
    }

  private:

    //*******************************************
    void initialise_insertion_point(const etl::imessage_router* p_router, etl::message_broker::subscription* p_new_sub)
    {
      const etl::imessage_router* p_target_router = p_router;

      etl::message_broker::subscription* p_sub = head.next();
      etl::message_broker::subscription* p_sub_previous = &head;

      while (p_sub != ETL_NULLPTR)
      {
        // Do we already have a subscription for the router?
        if (p_sub->router() == p_target_router)
        {
          // Then unlink it.
          p_sub_previous->next(p_sub->next()); // Jump over the subscription.
          p_sub->terminate(); // Terminate the unlinked subscription.

          // We're done now.
          break;
        }

        // Move on up the list.
        p_sub = p_sub->next();
        p_sub_previous = p_sub_previous->next();
      }

      if (p_new_sub != ETL_NULLPTR)
      {
        // Link in the new subscription.
        p_sub_previous->append(p_new_sub);
      }
    }

    //*******************************************
    /// Check if the message id is in the message list.
    //*******************************************
    bool is_in_subscription_list(etl::message_id_t id) const
    {
      //if (!empty())
      //{
      //  // Scan the subscription lists.
      //  subscription* list = subscription_list;
      //}

      return false;
    }

    static ETL_CONSTANT uint_least8_t Unused = etl::integral_limits<uint_least8_t>::max;

    subscription  head;
  };

  //***************************************************************************
  /// Send a message to a broker.
  //***************************************************************************
  //static inline void send_message(etl::imessage_broker& broker,
  //                                const etl::imessage&  message)
  //{
  //  broker.receive(message);
  //}

  ////***************************************************************************
  ///// Send a shared message to a broker.
  ////***************************************************************************
  //static inline void send_message(etl::imessage_broker& broker,
  //                                etl::shared_message   message)
  //{
  //  broker.receive(message);
  //}
}

#endif
