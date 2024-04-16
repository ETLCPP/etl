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

#include "platform.h"
#include "nullptr.h"
#include "message_types.h"
#include "message.h"
#include "message_router.h"
#include "span.h"

#include <stdint.h>

namespace etl
{
  //***************************************************************************
  /// Message broker
  //***************************************************************************
  class message_broker : public etl::imessage_router
  {
  private:

    //*******************************************
    class subscription_node
    {
      friend class message_broker;

    protected:

      //*******************************
      subscription_node()
        : p_next(ETL_NULLPTR)
      {
      }

      //*******************************
      void set_next(subscription_node* sub)
      {
        p_next = sub;
      }

      //*******************************
      subscription_node* get_next() const
      {
        return p_next;
      }

      //*******************************
      void terminate()
      {
        set_next(ETL_NULLPTR);
      }

      //*******************************
      void append(subscription_node* sub)
      {
        if (sub != ETL_NULLPTR)
        {
          sub->set_next(get_next());
        }
        set_next(sub);
      }

      subscription_node* p_next;
    };

  public:

    typedef etl::span<const etl::message_id_t> message_id_span_t;

    //*******************************************
    class subscription : public subscription_node
    {
    public:

      friend class message_broker;

      //*******************************
      subscription(etl::imessage_router& router_)
        : p_router(&router_)
      {
      }

    private:

      //*******************************
      virtual message_id_span_t message_id_list() const = 0;

      //*******************************
      etl::imessage_router* get_router() const
      {
        return p_router;
      }

      //*******************************
      subscription* next_subscription() const
      {
        return static_cast<subscription*>(get_next());
      }

      etl::imessage_router* const p_router;
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
    message_broker(etl::imessage_router& successor_)
      : imessage_router(etl::imessage_router::MESSAGE_BROKER, successor_)
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
      ETL_ASSERT((id_ <= etl::imessage_router::MAX_MESSAGE_ROUTER) || (id_ == etl::imessage_router::MESSAGE_BROKER), ETL_ERROR(etl::message_router_illegal_id));
    }

    //*******************************************
    /// Constructor.
    //*******************************************
    message_broker(etl::message_router_id_t id_, etl::imessage_router& successor_)
      : imessage_router(id_, successor_)
      , head()
    {
      ETL_ASSERT((id_ <= etl::imessage_router::MAX_MESSAGE_ROUTER) || (id_ == etl::imessage_router::MESSAGE_BROKER), ETL_ERROR(etl::message_router_illegal_id));
    }

    //*******************************************
    /// Subscribe to the broker.
    //*******************************************
    void subscribe(etl::message_broker::subscription& new_sub)
    {
      initialise_insertion_point(new_sub.get_router(), &new_sub);
    }

    //*******************************************
    void unsubscribe(etl::imessage_router& router)
    {
      initialise_insertion_point(&router, ETL_NULLPTR);
    }

    //*******************************************
    virtual void receive(const etl::imessage& msg) ETL_OVERRIDE
    {
      receive(etl::imessage_router::ALL_MESSAGE_ROUTERS, msg);
    }

    virtual void receive(etl::shared_message shared_msg) ETL_OVERRIDE
    {
      receive(etl::imessage_router::ALL_MESSAGE_ROUTERS, shared_msg);
    }

    //*******************************************
    virtual void receive(etl::message_router_id_t destination_router_id,
                         const etl::imessage&     msg) ETL_OVERRIDE
    {
      const etl::message_id_t id = msg.get_message_id();

      if (!empty())
      {
        // Scan the subscription lists.
        subscription* sub = static_cast<subscription*>(head.get_next());

        while (sub != ETL_NULLPTR)
        {
          message_id_span_t message_ids = sub->message_id_list();

          message_id_span_t::iterator itr = etl::find(message_ids.begin(), message_ids.end(), id);

          if (itr != message_ids.end())
          {
            etl::imessage_router* router = sub->get_router();

            if (destination_router_id == etl::imessage_router::ALL_MESSAGE_ROUTERS ||
                destination_router_id == router->get_message_router_id())
            {
              router->receive(msg);
            }
          }

          sub = sub->next_subscription();
        }
      }

      // Always pass the message on to the successor.
      if (has_successor())
      {
        get_successor().receive(destination_router_id, msg);
      }
    }

    //*******************************************
    virtual void receive(etl::message_router_id_t destination_router_id, 
                         etl::shared_message      shared_msg) ETL_OVERRIDE
    {
      const etl::message_id_t id = shared_msg.get_message().get_message_id();

      if (!empty())
      {
        // Scan the subscription lists.
        subscription* sub = static_cast<subscription*>(head.get_next());

        while (sub != ETL_NULLPTR)
        {
          message_id_span_t message_ids = sub->message_id_list();

          message_id_span_t::iterator itr = etl::find(message_ids.begin(), message_ids.end(), id);

          if (itr != message_ids.end())
          {
            etl::imessage_router* router = sub->get_router();

            if (destination_router_id == etl::imessage_router::ALL_MESSAGE_ROUTERS ||
                destination_router_id == router->get_message_router_id())
            {
              router->receive(shared_msg);
            }
          }

          sub = sub->next_subscription();
        }
      }

      // Always pass the message on to a successor.
      if (has_successor())
      {
        get_successor().receive(destination_router_id, shared_msg);
      }
    }

    using imessage_router::accepts;

    //*******************************************
    /// Message brokers accept messages determined
    /// by the subscribed routers.
    //*******************************************
    virtual bool accepts(etl::message_id_t id) const ETL_OVERRIDE
    {
      if (!empty())
      {
        // Scan the subscription lists.
        subscription* sub = static_cast<subscription*>(head.get_next());

        while (sub != ETL_NULLPTR)
        {
          message_id_span_t message_ids = sub->message_id_list();

          message_id_span_t::iterator itr = etl::find(message_ids.begin(), message_ids.end(), id);

          if (itr != message_ids.end())
          {
            etl::imessage_router* router = sub->get_router();

            if (router->accepts(id))
            {
              return true;
            }
          }

          sub = sub->next_subscription();
        }
      }

      // Check any successor.
      if (has_successor())
      {
        if (get_successor().accepts(id))
        {
          return true;
        }
      }

      return false;

      //return true;
    }

    //*******************************************
    void clear()
    {
      head.terminate();
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
      return head.get_next() == ETL_NULLPTR;
    }

  private:

    //*******************************************
    void initialise_insertion_point(const etl::imessage_router* p_router, etl::message_broker::subscription* p_new_sub)
    {
      const etl::imessage_router* p_target_router = p_router;

      subscription_node* p_sub          = head.get_next();
      subscription_node* p_sub_previous = &head;

      while (p_sub != ETL_NULLPTR)
      {
        // Do we already have a subscription for the router?
        if (static_cast<subscription*>(p_sub)->get_router() == p_target_router)
        {
          // Then unlink it.
          p_sub_previous->set_next(p_sub->get_next()); // Jump over the subscription.
          p_sub->terminate();                          // Terminate the unlinked subscription.

          // We're done now.
          break;
        }

        // Move on up the list.
        p_sub = p_sub->get_next();
        p_sub_previous = p_sub_previous->get_next();
      }

      if (p_new_sub != ETL_NULLPTR)
      {
        // Link in the new subscription.
        p_sub_previous->append(p_new_sub);
      }
    }

    subscription_node head;
  };
}

#endif
