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

#ifndef __ETL_MESSAGE_BUS_
#define __ETL_MESSAGE_BUS_

#include <stdint.h>
#include <algorithm>

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

    message_bus_exception(string_type what, string_type file_name, numeric_type line_number)
      : etl::exception(what, file_name, line_number)
    {
    }
  };

  //***************************************************************************
  /// Too many subscribers.
  //***************************************************************************
  class message_bus_too_many_subscribers : public etl::message_bus_exception
  {
  public:

    message_bus_too_many_subscribers(string_type file_name, numeric_type line_number)
      : message_bus_exception(ETL_ERROR_TEXT("message bus:too many subscribers", ETL_FILE"A"), file_name, line_number)
    {
    }
  };

  //***************************************************************************
  /// Duplicate router id.
  //***************************************************************************
  class message_bus_duplicate_router_id : public etl::message_bus_exception
  {
  public:

    message_bus_duplicate_router_id(string_type file_name, numeric_type line_number)
      : message_bus_exception(ETL_ERROR_TEXT("message bus:duplicate router id", ETL_FILE"B"), file_name, line_number)
    {
    }
  };

  //***************************************************************************
  /// Base for message bus
  //***************************************************************************
  class imessage_bus : public etl::imessage_router
  {
  public:

    imessage_bus()
      : imessage_router(etl::imessage_router::MESSAGE_BUS)
    {
    }

    using etl::imessage_router::receive;

    virtual void receive(etl::message_router_id_t destination_router_id, const etl::imessage& message) = 0;
    virtual void receive(etl::imessage_router& source, etl::message_router_id_t destination_router_id, const etl::imessage& message) = 0;
    virtual bool subscribe(etl::imessage_router& router) = 0;
    virtual void unsubscribe(etl::message_router_id_t id) = 0;

    //*******************************************
    void unsubscribe(etl::imessage_router& router)
    {
      unsubscribe(router.get_message_router_id());
    }
  };

  //***************************************************************************
  /// The message bus
  //***************************************************************************
  template <uint_least8_t MAX_ROUTERS_>
  class message_bus : public etl::imessage_bus
  {
  private:

    //*******************************************
    // How to compare router ids.
    //*******************************************
    struct compare_router_id
    {
      bool operator()(etl::imessage_router* prouter, etl::message_router_id_t id) const
      {
        return prouter->get_message_router_id() > id;
      }

      bool operator()(etl::message_router_id_t id, etl::imessage_router* prouter) const
      {
        return id > prouter->get_message_router_id();
      }
    };

  public:

    //*******************************************
    /// Subscribe to the bus.
    //*******************************************
    bool subscribe(etl::imessage_router& router)
    {
      bool ok = true;

      // There's no point actually adding null routers.
      if (router.get_message_router_id() != etl::imessage_router::NULL_MESSAGE_ROUTER)
      {
        ok = !router_list.full();

        ETL_ASSERT(ok, ETL_ERROR(etl::message_bus_too_many_subscribers));

        if (ok)
        {
          // Insert in order.
          router_list_t::iterator irouter = std::lower_bound(router_list.begin(),
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
    using etl::imessage_bus::unsubscribe;

    void unsubscribe(etl::message_router_id_t id)
    {
      std::pair<router_list_t::iterator, router_list_t::iterator> range = std::equal_range(router_list.begin(),
                                                                                           router_list.end(), 
                                                                                           id,
                                                                                           compare_router_id());

      while (range.first != range.second)
      {
        router_list.erase(range.first);
        ++range.first;
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
        // Null message router. Should never get here as these routers can never be subscribed.
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

            if (router.get_message_router_id() == etl::imessage_router::MESSAGE_BUS)
            {
              // The router is actually a bus.
              etl::imessage_bus& bus = static_cast<etl::imessage_bus&>(router);

              // So pass it on.
              bus.receive(source, destination_router_id, message);
            }
            else if (router.accepts(message.message_id))
            {
              router.receive(source, message);
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

          // Do any message busses first. 
          // These are always at the start of the list.
          while ((*irouter)->get_message_router_id() == etl::imessage_router::MESSAGE_BUS)
          {
            // The router is actually a bus.
            etl::imessage_bus& bus = static_cast<etl::imessage_bus&>(**irouter);

            // So pass it on.
            bus.receive(source, destination_router_id, message);

            ++irouter;
          }
          
          // See if routers with the id exist.
          std::pair<router_list_t::iterator, router_list_t::iterator> range = std::equal_range(irouter,
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

          break;
        }
      }
    }

    using imessage_router::accepts;

    //*******************************************
    /// Does this message bus accept the message id?
    /// Yes!, it accepts everything!
    //*******************************************
    bool accepts(etl::message_id_t id) const
    {
      return true;
    }

    //*******************************************
    size_t size() const
    {
      return router_list.size();
    }

  private:

    typedef etl::vector<etl::imessage_router*, MAX_ROUTERS_> router_list_t;
    router_list_t router_list;
  };
}

#undef ETL_FILE

#endif
