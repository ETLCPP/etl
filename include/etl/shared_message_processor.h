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

#ifndef ETL_SHARED_MESSAGE_PROCESSOR_INCLUDED
#define ETL_SHARED_MESSAGE_PROCESSOR_INCLUDED

#include "platform.h"
#include "shared_message.h"
#include "vector.h"

#include <stdint.h>

namespace etl
{
  typedef uint_least8_t shared_message_processor_id_t;

  //***************************************************************************
  /// This is the base of all shared message processors.
  //***************************************************************************
  class ishared_message_processor
  {
  public:

    //***************************************************************************
    /// 
    //***************************************************************************
    ishared_message_processor(etl::shared_message_processor_id_t id_ = etl::shared_message_processor_id_t())
      : id(id_)
    {
    }

    virtual void receive(etl::shared_message shared_msg) = 0;
    virtual void receive(shared_message_processor_id_t destination_processor_id, etl::shared_message shared_msg) = 0;
    virtual bool accepts(etl::message_id_t id) const = 0;

    //***************************************************************************
    /// 
    //***************************************************************************
    bool accepts(etl::shared_message shared_msg) const
    {
      return accepts(shared_msg.get_message().get_message_id());
    }

    //***************************************************************************
    /// 
    //***************************************************************************
    shared_message_processor_id_t get_shared_message_processor_id() const
    {
      return id;
    }

    enum
    {
      NULL_SHARED_MESSAGE_PROCESSOR = 255,
      SHARED_MESSAGE_BUS            = 254,
      ALL_SHARED_MESSAGE_PROCESSORS = 253,
      MAX_SHARED_MESSAGE_PROCESSOR  = 249
    };

  protected:

    etl::shared_message_processor_id_t id;
  };

  //***************************************************************************
  /// Derive user defined shared message processors from this.
  //***************************************************************************
  class shared_message_processor : public ishared_message_processor
  {
  public:

    //***************************************************************************
    /// 
    //***************************************************************************
    shared_message_processor(etl::shared_message_processor_id_t id_ = etl::shared_message_processor_id_t())
      : ishared_message_processor(id_)
    {
    }

    etl::ishared_message_processor::receive;

    //***************************************************************************
    /// 
    //***************************************************************************
    void receive(etl::shared_message shared_msg)
    {
      on_receive(shared_msg);
    }

    //***************************************************************************
    /// 
    //***************************************************************************
    void receive(shared_message_processor_id_t destination_processor_id, etl::shared_message shared_msg)
    {
      if (destination_processor_id == id)
      {
        on_receive(shared_msg);
      }
    }

    // User overridden.
    virtual void on_receive(etl::shared_message shared_msg) = 0;
  };

  //***************************************************************************
  /// Interface for shared message bus
  //***************************************************************************
  class ishared_message_bus : public etl::ishared_message_processor
  {
  private:

    typedef etl::ivector<etl::ishared_message_processor*> processor_list_t;

  public:

    using etl::ishared_message_processor::receive;

    //*******************************************
    /// Subscribe to the bus.
    //*******************************************
    bool subscribe(etl::ishared_message_processor& processor)
    {
      bool ok = true;

      ok = !processor_list.full();

      ETL_ASSERT(ok, ETL_ERROR(etl::shared_message_bus_too_many_subscribers));

      if (ok)
      {
        processor_list_t::iterator iprocessor = etl::upper_bound(processor_list.begin(),
                                                                 processor_list.end(),
                                                                 processor.get_shared_message_processor_id(),
                                                                 compare_processor_id());

        processor_list.insert(iprocessor, &processor);
      }

      return ok;
    }

    //*******************************************
    /// Unsubscribe from the bus.
    //*******************************************
    void unsubscribe(etl::shared_message_processor_id_t id)
    {
      if (id == etl::ishared_message_bus::ALL_SHARED_MESSAGE_PROCESSORS)
      {
        clear();
      }
      else
      {
        ETL_OR_STD::pair<processor_list_t::iterator, processor_list_t::iterator> range = etl::equal_range(processor_list.begin(),
                                                                                                          processor_list.end(),
                                                                                                          id,
                                                                                                          compare_processor_id());

        processor_list.erase(range.first, range.second);
      }
    }

    //*******************************************
    void unsubscribe(etl::shared_message_processor& processor)
    {
      processor_list_t::iterator iprocessor = etl::find(processor_list.begin(),
                                                        processor_list.end(),
                                                        &processor);

      if (iprocessor != processor_list.end())
      {
        processor_list.erase(iprocessor);
      }
    }

    //*******************************************
    void receive(shared_message shared_msg) ETL_OVERRIDE
    {
      receive(etl::shared_message_processor::ALL_SHARED_MESSAGE_PROCESSORS, shared_msg);
    }

    //*******************************************
    void receive(etl::shared_message_processor_id_t destination_processor_id,
                 etl::shared_message shared_msg) ETL_OVERRIDE
    {
      switch (destination_processor_id)
      {
        //*****************************
        // Broadcast to all processors.
        case etl::shared_message_processor::ALL_SHARED_MESSAGE_PROCESSORS:
        {
          processor_list_t::iterator iprocessor = processor_list.begin();

          // Broadcast to everyone.
          while (iprocessor != processor_list.end())
          {
            etl::ishared_message_processor& processor = **iprocessor;

            if (processor.accepts(shared_msg.get_message().get_message_id()))
            {
              processor.receive(shared_msg);
            }

            ++iprocessor;
          }

          break;
        }

        //*****************************
        // Must be an addressed shared_msg.
        default:
        {
          processor_list_t::iterator iprocessor = processor_list.begin();

          // Find processors with the id.
          ETL_OR_STD::pair<processor_list_t::iterator, processor_list_t::iterator> range = etl::equal_range(processor_list.begin(),
                                                                                                            processor_list.end(),
                                                                                                            destination_processor_id,
                                                                                                            compare_processor_id());

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
          iprocessor = etl::lower_bound(processor_list.begin(),
                                        processor_list.end(),
                                        etl::ishared_message_bus::SHARED_MESSAGE_BUS,
                                        compare_processor_id());

          while (iprocessor != processor_list.end())
          {
            // So pass it on.
            (*iprocessor)->receive(destination_processor_id, shared_msg);

            ++iprocessor;
          }

          break;
        }
      }
    }

    using ishared_message_processor::accepts;

    //*******************************************
    /// Does this message bus accept the message id?
    /// Yes!, it accepts everything!
    //*******************************************
    bool accepts(etl::message_id_t) const ETL_OVERRIDE
    {
      return true;
    }

    //*******************************************
    size_t size() const
    {
      return processor_list.size();
    }

    //*******************************************
    void clear()
    {
      return processor_list.clear();
    }

  protected:

    //*******************************************
    /// Constructor.
    //*******************************************
    ishared_message_bus(processor_list_t& list)
      : ishared_message_processor(etl::ishared_message_processor::SHARED_MESSAGE_BUS)
      , processor_list(list)
    {
    }

  private:

    //*******************************************
    // How to compare processors to processor ids.
    //*******************************************
    struct compare_processor_id
    {
      bool operator()(const etl::ishared_message_processor* prouter, etl::shared_message_processor_id_t id) const
      {
        return prouter->get_shared_message_processor_id() < id;
      }

      bool operator()(etl::shared_message_processor_id_t id, const etl::ishared_message_processor* prouter) const
      {
        return id < prouter->get_shared_message_processor_id();
      }
    };

    processor_list_t& processor_list;
  };

  //***************************************************************************
  /// The message bus
  //***************************************************************************
  template <uint_least8_t MAX_PROCESSORS_>
  class shared_message_bus : public etl::ishared_message_bus
  {
  public:

    //*******************************************
    /// Constructor.
    //*******************************************
    shared_message_bus()
      : ishared_message_bus(processor_list)
    {
    }

  private:

    etl::vector<etl::shared_message_processor*, MAX_PROCESSORS_> processor_list;
  };
}

#endif
