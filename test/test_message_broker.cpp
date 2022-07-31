/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl
https://www.etlcpp.com

Copyright(c) 2022 John Wellbelove

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

#include "unit_test_framework.h"

#include "etl/message_broker.h"

#include <array>

//***************************************************************************
// The set of messages.
//***************************************************************************
namespace
{
  enum
  {
    MESSAGE1,
    MESSAGE2,
    MESSAGE3,
    MESSAGE4,
    MESSAGE5
  };

  enum
  {
    Router1 = 1,
  };

  struct Message1 : public etl::message<MESSAGE1>
  {
    Message1()
    {
    }
  };

  struct Message2 : public etl::message<MESSAGE2>
  {
    Message2()
    {
    }
  };

  struct Message3 : public etl::message<MESSAGE3>
  {
    Message3()
      : value()
    {
    }

    int value[10];
  };

  struct Message4 : public etl::message<MESSAGE4>
  {
    Message4()
    {
    }
  };

  struct Message5 : public etl::message<MESSAGE5>
  {
    Message5()
    {
    }
  };

  int call_order;

  constexpr uint16_t N_Routers  = 4U;
  constexpr uint16_t N_Messages = 5U;

  //***************************************************************************
  // Broker
  //***************************************************************************
  class Broker : public etl::message_broker
  {
  public:

    using etl::message_broker::receive;

    // Hook incoming messages and translate Message5 to Message4.
    void receive(const etl::imessage& msg) override
    {
      if (msg.get_message_id() == Message5::ID)
      {
        etl::message_broker::receive(Message4());
      }
      else
      {
        etl::message_broker::receive(msg);
      }
    }
  };

  //***************************************************************************
  // Router that handles messages 1, 2, 3, 4.
  //***************************************************************************
  class Router : public etl::message_router<Router, Message1, Message2, Message3, Message4>
  {
  public:

    Router(etl::message_router_id_t id)
      : message_router(id)
      , message1_count(0)
      , message2_count(0)
      , message3_count(0)
      , message4_count(0)
      , message5_count(0)
      , message_unknown_count(0)
      , order(0)
    {
    }

    void on_receive(const Message1&)
    {
      ++message1_count;
      order = call_order++;
    }

    void on_receive(const Message2&)
    {
      ++message2_count;
    }

    void on_receive(const Message3&)
    {
      ++message3_count;
    }

    void on_receive(const Message4&)
    {
      ++message4_count;
    }

    void on_receive(const Message5&)
    {
      ++message5_count;
    }

    void on_receive_unknown(const etl::imessage&)
    {
      ++message_unknown_count;
    }

    int message1_count;
    int message2_count;
    int message3_count;
    int message4_count;
    int message5_count;
    int message_unknown_count;
    int order;
  };


  SUITE(test_message_broker)
  {
    //*************************************************************************
    TEST(message_bus_subscribe_unsubscribe)
    {
      Broker broker;
      Router router1(1);
      Router router2(2);
      Router router3(3);

      CHECK(broker.empty());

      std::array<etl::message_id_t, 3U> subscription1_message_list = { Message1::ID, Message2::ID, Message3::ID };
      Broker::subscription subscription1(router1, subscription1_message_list.data(), subscription1_message_list.size());

      etl::array<etl::message_id_t, 2U> subscription2_message_list = { Message1::ID, Message2::ID };
      Broker::subscription subscription2(router2, subscription2_message_list);

      etl::array<etl::message_id_t, 2U> subscription3_message_list = { Message1::ID, Message3::ID };
      Broker::subscription subscription3(router2, subscription3_message_list);

      broker.subscribe(subscription1);
      CHECK(!broker.empty());

      broker.subscribe(subscription2);
      CHECK(!broker.empty());

      broker.subscribe(subscription3); // Duplicate router. Replace the old subscription.
      CHECK(!broker.empty());

      broker.unsubscribe(router1);
      CHECK(!broker.empty());

      broker.unsubscribe(router2);
      CHECK(broker.empty());

      broker.unsubscribe(router3);
      CHECK(broker.empty());
    }
  };
}
