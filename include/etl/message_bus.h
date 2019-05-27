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

#ifndef ETL_MESSAGE_BUS_
#define ETL_MESSAGE_BUS_

#include <stdint.h>
#include "algorithm.h"

#include "platform.h"
#include "algorithm.h"
#include "vector.h"
#include "nullptr.h"
#include "error_handler.h"
#include "exception.h"
#include "message_types.h"
#include "message.h"
#include "message_router.h"

#undef ETL_FILE
#define ETL_FILE "39"

namespace etl
{
  //***************************************************************************
  /// Base exception class for message bus
  //***************************************************************************
  class message_bus_exception : public etl::exception
  {
  public:

    message_bus_exception(string_type reason_, string_type file_name_, numeric_type line_number_)
      : etl::exception(reason_, file_name_, line_number_)
    {
    }
  };

  //***************************************************************************
  /// Too many subscribers.
  //***************************************************************************
  class message_bus_too_many_subscribers : public etl::message_bus_exception
  {
  public:

    message_bus_too_many_subscribers(string_type file_name_, numeric_type line_number_)
      : message_bus_exception(ETL_ERROR_TEXT("message bus:too many subscribers", ETL_FILE"A"), file_name_, line_number_)
    {
    }
  };

  //***************************************************************************
  /// Interface for message bus
  //***************************************************************************
  class imessage_bus : public etl::imessage_router
  {
  private:

    typedef etl::ivector<etl::imessage_router*> router_list_t;

  public:

    using etl::imessage_router::receive;

    //*******************************************
    /// Subscribe to the bus.
    //*******************************************
    bool subscribe(etl::imessage_router& router)
    {
      bool ok = true;

      // There's no point actually adding null routers.
      if (!router.is_null_router())
      {
        ok = !router_list.full();

        ETL_ASSERT(ok, ETL_ERROR(etl::message_bus_too_many_subscribers));

        if (ok)
        {
          router_list_t::iterator irouter = std::upper_bound(router_list.begin(),
                                                             router_list.end(),
                                                             router.get_message_router_id(),
                                                             compare_router_id());

          router_list.insert(irouter, &router);
        }
      }

      return ok;
    }

    //*******************************************
    /// Unsubscribe from the bus.
    //*******************************************
    void unsubscribe(etl::message_router_id_t id)
    {
      if (id == etl::imessage_bus::ALL_MESSAGE_ROUTERS)
      {
        clear();
      }
      else
      {
        std::pair<router_list_t::iterator, router_list_t::iterator> range = std::equal_range(router_list.begin(),
                                                                                             router_list.end(),
                                                                                             id,
                                                                                             compare_router_id());

        router_list.erase(range.first, range.second);
      }
    }

    //*******************************************
    void unsubscribe(etl::imessage_router& router)
    {
      router_list_t::iterator irouter = std::find(router_list.begin(),
                                                  router_list.end(),
                                                  &router);

      if (irouter != router_list.end())
      {
        router_list.erase(irouter);
      }
    }

    //*******************************************
    void receive(const etl::imessage& message)
    {
      etl::null_message_router nmr;
      receive(nmr, etl::imessage_router::ALL_MESSAGE_ROUTERS, message);
    }

    //*******************************************
    void receive(etl::message_router_id_t destination_router_id,
                 const etl::imessage&     message)
    {
      etl::null_message_router nmr;
      receive(nmr, destination_router_id, message);
    }

    //*******************************************
    void receive(etl::imessage_router& source,
                 const etl::imessage&  message)
    {
      receive(source, etl::imessage_router::ALL_MESSAGE_ROUTERS, message);
    }

    //*******************************************
    void receive(etl::imessage_router&    source,
                 etl::message_router_id_t destination_router_id,
                 const etl::imessage&     message)
    {
      switch (destination_router_id)
      {
        //*****************************
        // Null message router. These routers can never be subscribed.
        case etl::imessage_router::NULL_MESSAGE_ROUTER:
        {
          break;
        }

        //*****************************
        // Broadcast to all routers.
        case etl::imessage_router::ALL_MESSAGE_ROUTERS:
        {
          router_list_t::iterator irouter = router_list.begin();

          // Broadcast to everyone.
          while (irouter != router_list.end())
          {
            etl::imessage_router& router = **irouter;

            if (router.accepts(message.message_id))
            {
              router.receive(source, destination_router_id, message);
            }

            ++irouter;
          }

          break;
        }

        //*****************************
        // Must be an addressed message.
        default:
        {
          router_list_t::iterator irouter = router_list.begin();

          // Find routers with the id.
          std::pair<router_list_t::iterator, router_list_t::iterator> range = std::equal_range(router_list.begin(),
                                                                                               router_list.end(),
                                                                                               destination_router_id,
                                                                                               compare_router_id());

          // Call all of them.
          while (range.first != range.second)
          {
            if ((*(range.first))->accepts(message.message_id))
            {
              (*(range.first))->receive(source, message);
            }

            ++range.first;
          }

          // Do any message buses.
          // These are always at the end of the list.
          irouter = std::lower_bound(router_list.begin(),
                                     router_list.end(),
                                     etl::imessage_bus::MESSAGE_BUS,
                                     compare_router_id());

          while (irouter != router_list.end())
          {
            // So pass it on.
            (*irouter)->receive(source, destination_router_id, message);

            ++irouter;
          }

          break;
        }
      }
    }

    using imessage_router::accepts;

    //*******************************************
    /// Does this message bus accept the message id?
    /// Yes!, it accepts everything!
    //*******************************************
    bool accepts(etl::message_id_t) const
    {
      return true;
    }

    //*******************************************
    size_t size() const
    {
      return router_list.size();
    }

    //*******************************************
    void clear()
    {
      return router_list.clear();
    }

    //********************************************
    bool is_null_router() const
    {
      return false;
    }

  protected:

    //*******************************************
    /// Constructor.
    //*******************************************
    imessage_bus(router_list_t& list)
      : imessage_router(etl::imessage_router::MESSAGE_BUS),
        router_list(list)
    {
    }

  private:

    //*******************************************
    // How to compare routers to router ids.
    //*******************************************
    struct compare_router_id
    {
      bool operator()(const etl::imessage_router* prouter, etl::message_router_id_t id) const
      {
        return prouter->get_message_router_id() < id;
      }

      bool operator()(etl::message_router_id_t id, const etl::imessage_router* prouter) const
      {
        return id < prouter->get_message_router_id();
      }
    };

    router_list_t& router_list;
  };

  //***************************************************************************
  /// The message bus
  //***************************************************************************
  template <uint_least8_t MAX_ROUTERS_>
  class message_bus : public etl::imessage_bus
  {
  public:

    //*******************************************
    /// Constructor.
    //*******************************************
    message_bus()
      : imessage_bus(router_list)
    {
    }

  private:

    etl::vector<etl::imessage_router*, MAX_ROUTERS_> router_list;
  };

  //***************************************************************************
  /// Send a message to a bus.
  //***************************************************************************
  inline static void send_message(etl::imessage_bus&   bus,
                                  const etl::imessage& message)
  {
    bus.receive(message);
  }

  //***************************************************************************
  /// Send a message to a bus.
  //***************************************************************************
  inline static void send_message(etl::imessage_bus&       bus,
                                  etl::message_router_id_t id,
                                  const etl::imessage&     message)
  {
    bus.receive(id, message);
  }

  //***************************************************************************
  /// Send a message to a bus.
  //***************************************************************************
  inline static void send_message(etl::imessage_router& source,
                                  etl::imessage_bus&    bus,
                                  const etl::imessage&  message)
  {
    bus.receive(source, message);
  }

  //***************************************************************************
  /// Send a message to a bus.
  //***************************************************************************
  inline static void send_message(etl::imessage_router&    source,
                                  etl::imessage_bus&       bus,
                                  etl::message_router_id_t id,
                                  const etl::imessage&     message)
  {
    bus.receive(source, id, message);
  }
}

#undef ETL_FILE

#endif
