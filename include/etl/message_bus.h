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

#ifndef ETL_MESSAGE_BUS_INCLUDED
#define ETL_MESSAGE_BUS_INCLUDED

#include "platform.h"
#include "algorithm.h"
#include "error_handler.h"
#include "exception.h"
#include "message.h"
#include "message_processor.h"
#include "message_types.h"
#include "vector.h"

#include "private/diagnostic_unnamed_type_template_args_push.h"

#include <stdint.h>

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
      : message_bus_exception(ETL_ERROR_TEXT("message bus:too many subscribers", ETL_MESSAGE_BUS_FILE_ID"A"), file_name_, line_number_)
    {
    }
  };

  //***************************************************************************
  /// Interface for message bus
  //***************************************************************************
  class imessage_bus : public etl::message_processor
  {
  private:

    typedef etl::ivector<etl::message_processor*> processor_list_t;

  public:

    using etl::message_processor::receive;

    //*******************************************
    /// Subscribe to the bus.
    //*******************************************
    bool subscribe(etl::message_processor& processor)
    {
      bool ok = true;

      // There's no point adding processors that don't consume messages.
      if (processor.is_consumer())
      {
        ok = !processor_list.full();

        ETL_ASSERT(ok, ETL_ERROR(etl::message_bus_too_many_subscribers));

        if (ok)
        {
          processor_list_t::iterator iprocessor =
            etl::upper_bound(processor_list.begin(), processor_list.end(), processor.get_message_processor_id(), compare_processor_id());

          processor_list.insert(iprocessor, &processor);
        }
      }

      return ok;
    }

    //*******************************************
    /// Unsubscribe from the bus.
    //*******************************************
    void unsubscribe(etl::message_processor_id_t id)
    {
      if (id == etl::imessage_bus::ALL_MESSAGE_ROUTERS)
      {
        clear();
      }
      else
      {
        ETL_OR_STD::pair<processor_list_t::iterator, processor_list_t::iterator> range =
          etl::equal_range(processor_list.begin(), processor_list.end(), id, compare_processor_id());

        processor_list.erase(range.first, range.second);
      }
    }

    //*******************************************
    void unsubscribe(etl::message_processor& processor)
    {
      processor_list_t::iterator iprocessor = etl::find(processor_list.begin(), processor_list.end(), &processor);

      if (iprocessor != processor_list.end())
      {
        processor_list.erase(iprocessor);
      }
    }

    //*******************************************
    virtual void receive(const etl::imessage& message) ETL_OVERRIDE
    {
      receive(etl::message_processor::ALL_MESSAGE_ROUTERS, message);
    }

    //*******************************************
    virtual void receive(etl::shared_message shared_msg) ETL_OVERRIDE
    {
      receive(etl::message_processor::ALL_MESSAGE_ROUTERS, shared_msg);
    }

    //*******************************************
    virtual void receive(etl::message_processor_id_t destination_processor_id, const etl::imessage& message) ETL_OVERRIDE
    {
      switch (destination_processor_id)
      {
        //*****************************
        // Broadcast to all processors.
        case etl::message_processor::ALL_MESSAGE_ROUTERS:
          {
            processor_list_t::iterator iprocessor = processor_list.begin();

            // Broadcast to everyone.
            while (iprocessor != processor_list.end())
            {
              etl::message_processor& processor = **iprocessor;

              if (processor.accepts(message.get_message_id()))
              {
                processor.receive(message);
              }

              ++iprocessor;
            }

            break;
          }

        //*****************************
        // Must be an addressed message.
        default:
          {
            processor_list_t::iterator iprocessor = processor_list.begin();

            // Find processors with the id.
            ETL_OR_STD::pair<processor_list_t::iterator, processor_list_t::iterator> range =
              etl::equal_range(processor_list.begin(), processor_list.end(), destination_processor_id, compare_processor_id());

            // Call all of them.
            while (range.first != range.second)
            {
              if ((*(range.first))->accepts(message.get_message_id()))
              {
                (*(range.first))->receive(message);
              }

              ++range.first;
            }

            // Do any message buses.
            // These are always at the end of the list.
            iprocessor = etl::lower_bound(processor_list.begin(), processor_list.end(), etl::imessage_bus::MESSAGE_BUS, compare_processor_id());

            while (iprocessor != processor_list.end())
            {
              // So pass it on.
              (*iprocessor)->receive(destination_processor_id, message);

              ++iprocessor;
            }

            break;
          }
      }

      if (has_successor())
      {
        if (get_successor().accepts(message.get_message_id()))
        {
          get_successor().receive(destination_processor_id, message);
        }
      }
    }

    //********************************************
    virtual void receive(etl::message_processor_id_t destination_processor_id, etl::shared_message shared_msg) ETL_OVERRIDE
    {
      switch (destination_processor_id)
      {
          //*****************************
          // Broadcast to all processors.
        case etl::message_processor::ALL_MESSAGE_ROUTERS:
          {
            processor_list_t::iterator iprocessor = processor_list.begin();

            // Broadcast to everyone.
            while (iprocessor != processor_list.end())
            {
              etl::message_processor& processor = **iprocessor;

              if (processor.accepts(shared_msg.get_message().get_message_id()))
              {
                processor.receive(shared_msg);
              }

              ++iprocessor;
            }

            break;
          }

        //*****************************
        // Must be an addressed message.
        default:
          {
            // Find processors with the id.
            ETL_OR_STD::pair<processor_list_t::iterator, processor_list_t::iterator> range =
              etl::equal_range(processor_list.begin(), processor_list.end(), destination_processor_id, compare_processor_id());

            // Call all of them.
            while (range.first != range.second)
            {
              if ((*(range.first))->accepts(shared_msg.get_message().get_message_id()))
              {
                (*(range.first))->receive(shared_msg);
              }

              ++range.first;
            }

            // Do any message buses.
            // These are always at the end of the list.
            processor_list_t::iterator iprocessor =
              etl::lower_bound(processor_list.begin(), processor_list.end(), etl::imessage_bus::MESSAGE_BUS, compare_processor_id());

            while (iprocessor != processor_list.end())
            {
              // So pass it on.
              (*iprocessor)->receive(destination_processor_id, shared_msg);

              ++iprocessor;
            }

            break;
          }
      }

      if (has_successor())
      {
        if (get_successor().accepts(shared_msg.get_message().get_message_id()))
        {
          get_successor().receive(destination_processor_id, shared_msg);
        }
      }
    }

    using message_processor::accepts;

    //*******************************************
    /// Does this message bus accept the message id?
    /// Returns <b>true</b> on the first processor that does.
    //*******************************************
    bool accepts(etl::message_id_t id) const ETL_OVERRIDE
    {
      // Check the list of subscribed processors.
      processor_list_t::iterator iprocessor = processor_list.begin();

      while (iprocessor != processor_list.end())
      {
        etl::message_processor& processor = **iprocessor;

        if (processor.accepts(id))
        {
          return true;
        }

        ++iprocessor;
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
    }

    //*******************************************
    size_t size() const
    {
      return processor_list.size();
    }

    //*******************************************
    void clear()
    {
      processor_list.clear();
    }

    //********************************************
    ETL_DEPRECATED
    bool is_null_router() const ETL_OVERRIDE
    {
      return false;
    }

    //********************************************
    bool is_producer() const ETL_OVERRIDE
    {
      return true;
    }

    //********************************************
    bool is_consumer() const ETL_OVERRIDE
    {
      return true;
    }

  protected:

    //*******************************************
    /// Constructor.
    //*******************************************
    imessage_bus(processor_list_t& list)
      : message_processor(etl::message_processor::MESSAGE_BUS)
      , processor_list(list)
    {
    }

    //*******************************************
    /// Constructor.
    //*******************************************
    imessage_bus(processor_list_t& processor_list_, etl::message_processor& successor_)
      : message_processor(etl::message_processor::MESSAGE_BUS, successor_)
      , processor_list(processor_list_)
    {
    }

  private:

    //*******************************************
    // How to compare processors to processor ids.
    //*******************************************
    struct compare_processor_id
    {
      bool operator()(const etl::message_processor* pprocessor, etl::message_processor_id_t id) const
      {
        return pprocessor->get_message_processor_id() < id;
      }

      bool operator()(etl::message_processor_id_t id, const etl::message_processor* pprocessor) const
      {
        return id < pprocessor->get_message_processor_id();
      }
    };

    processor_list_t& processor_list;
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
      : imessage_bus(processor_list)
    {
    }

    //*******************************************
    /// Constructor.
    //*******************************************
    message_bus(etl::message_processor& successor_)
      : imessage_bus(processor_list, successor_)
    {
    }

  private:

    etl::vector<etl::message_processor*, MAX_ROUTERS_> processor_list;
  };
} // namespace etl

#include "private/diagnostic_pop.h"

#endif
