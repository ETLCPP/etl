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

#include "unit_test_framework.h"

#include "etl/message_router.h"
#include "etl/message_bus.h"
#include "etl/queue.h"
#include "etl/largest.h"
#include "etl/packet.h"

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
    MESSAGE5,
    MESSAGE6,
    MESSAGE7
  };

  enum
  {
    ROUTER1 = 1,
    ROUTER2 = 2,
    ROUTER3 = 3,
    ROUTER4 = 4,
    ROUTER5 = 5
  };

  struct Message1 : public etl::message<MESSAGE1>
  {
    Message1(etl::imessage_router& callback_)
      : callback(callback_)
    {
    }

    etl::imessage_router& callback;
  };

  struct Message2 : public etl::message<MESSAGE2>
  {
    Message2(etl::imessage_router& callback_)
      : callback(callback_)
    {
    }

    etl::imessage_router& callback;
  };

  struct Message3 : public etl::message<MESSAGE3>
  {
    Message3(etl::imessage_router& callback_)
      : callback(callback_)
      , value()
    {
    }

    etl::imessage_router& callback;
    int value[10];
  };

  struct Message4 : public etl::message<MESSAGE4>
  {
    Message4(etl::imessage_router& callback_)
      : callback(callback_)
    {
    }

    etl::imessage_router& callback;
  };

  struct Response : public etl::message<MESSAGE5>
  {
  };

  Response response;

  struct Message6 : public etl::message<MESSAGE6>
  {
  };

  struct Message7 : public etl::message<MESSAGE7>
  {
  };

  int call_order;

  //***************************************************************************
  // Router that handles messages 1, 2, 3, 4, 5.
  //***************************************************************************
  class RouterA : public etl::message_router<RouterA, Message1, Message2, Message3, Message4, Response>
  {
  public:

    RouterA(etl::message_router_id_t id)
      : message_router(id),
        message1_count(0),
        message2_count(0),
        message3_count(0),
        message4_count(0),
        message5_count(0),
        message_unknown_count(0),
        order(0)
    {
    }

    void on_receive(const Message1& msg)
    {
      ++message1_count;
      etl::send_message(msg.callback, response);

      order = call_order++;
    }

    void on_receive(const Message2& msg)
    {
      ++message2_count;
      etl::send_message(msg.callback, response);
    }

    void on_receive(const Message3& msg)
    {
      ++message3_count;
      etl::send_message(msg.callback, response);
    }

    void on_receive(const Message4& msg)
    {
      ++message4_count;
      etl::send_message(msg.callback, response);
    }

    void on_receive(const Response&)
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

  //***************************************************************************
  // Router that handles messages 1, 2, 4 and 5 and returns nothing.
  //***************************************************************************
  class RouterB : public etl::message_router<RouterB, Message1, Message2, Message4, Response>
  {
  public:

    RouterB(etl::message_router_id_t id)
      : message_router(id),
        message1_count(0),
        message2_count(0),
        message4_count(0),
        message5_count(0),
        message_unknown_count(0)
    {
    }

    void on_receive(const Message1& msg)
    {
      ++message1_count;
      etl::send_message(msg.callback, response);
    }

    void on_receive(const Message2& msg)
    {
      ++message2_count;
      etl::send_message(msg.callback, response);
    }

    void on_receive(const Message4& msg)
    {
      ++message4_count;
      etl::send_message(msg.callback, response);
    }

    void on_receive(const Response&)
    {
      ++message5_count;
    }

    void on_receive_unknown(const etl::imessage&)
    {
      ++message_unknown_count;
      //etl::send_message(msg.callback, response);
    }

    int message1_count;
    int message2_count;
    int message4_count;
    int message5_count;
    int message_unknown_count;
  };

  //***************************************************************************
  // Router that handles message 6 and returns nothing.
  //***************************************************************************
  class RouterC : public etl::message_router<RouterC, Message6>
  {
  public:

    RouterC(etl::message_router_id_t id)
      : message_router(id)
    {
    }

    void on_receive(const Message6&)
    {
    }

    void on_receive_unknown(const etl::imessage&)
    {
    }
  };

  //***************************************************************************
  template <size_t Size>
  class MessageBus : public etl::message_bus<Size>
  {
  public:

    MessageBus()
      : message_count(0)
    {
    }

    using etl::message_bus<Size>::receive;

    // Hook 'receive' to count the incoming messages.
    void receive(etl::message_router_id_t id, const etl::imessage& msg)
    {
      ++message_count;
      etl::message_bus<Size>::receive(id, msg);
    }

    int message_count;
  };

  SUITE(test_message_bus)
  {
    //*************************************************************************
    TEST(message_bus_subscribe_unsubscribe)
    {
      MessageBus<2> bus1;

      RouterA router1(0);
      RouterB router2(1);
      RouterA router3(2);

      CHECK_EQUAL(0U, bus1.size());

      CHECK_NO_THROW(bus1.subscribe(router1));
      CHECK_EQUAL(1U, bus1.size());

      CHECK_NO_THROW(bus1.subscribe(router2));
      CHECK_EQUAL(2U, bus1.size());

      CHECK_THROW(bus1.subscribe(router3), etl::message_bus_too_many_subscribers);
      CHECK_EQUAL(2U, bus1.size());

      bus1.unsubscribe(router1);
      CHECK_EQUAL(1U, bus1.size());

      // Erase router not in list.
      bus1.unsubscribe(router3);
      CHECK_EQUAL(1U, bus1.size());

      // Erase using id.
      bus1.unsubscribe(router2.get_message_router_id());
      CHECK_EQUAL(0U, bus1.size());

      // Erase router from empty list.
      bus1.unsubscribe(router2);
      CHECK_EQUAL(0U, bus1.size());

      CHECK_EQUAL(0, bus1.message_count);
    }

    //*************************************************************************
    TEST(message_bus_subscribe_unsubscribe_sub_bus)
    {
      MessageBus<4> bus1;
      MessageBus<2> bus2;
      MessageBus<3> bus3;

      RouterA router1(ROUTER1);
      RouterA router2(ROUTER2);
      RouterA router3(ROUTER3);
      RouterA router4(ROUTER4);

      bus1.subscribe(bus2);
      bus1.subscribe(router1);
      bus1.subscribe(bus3);
      bus1.subscribe(router2);

      bus2.subscribe(router3);
      bus3.subscribe(router4);

      CHECK_EQUAL(4U, bus1.size());

      bus1.unsubscribe(etl::imessage_bus::MESSAGE_BUS);
      CHECK_EQUAL(2U, bus1.size());

      bus1.unsubscribe(etl::imessage_bus::ALL_MESSAGE_ROUTERS);
      CHECK_EQUAL(0U, bus1.size());

      CHECK_EQUAL(0, bus1.message_count);
      CHECK_EQUAL(0, bus2.message_count);
      CHECK_EQUAL(0, bus3.message_count);
    }

    //*************************************************************************
    TEST(message_bus_broadcast)
    {
      MessageBus<2> bus1;

      RouterA router1(ROUTER1);
      RouterB router2(ROUTER2);
      RouterA callback(ROUTER3);

      bus1.subscribe(router1);
      bus1.subscribe(router2);

      Message1 message1(callback);
      Message2 message2(callback);
      Message3 message3(callback);
      Message4 message4(callback);

      bus1.receive(message1);

      CHECK_EQUAL(1, router1.message1_count);
      CHECK_EQUAL(0, router1.message2_count);
      CHECK_EQUAL(0, router1.message3_count);
      CHECK_EQUAL(0, router1.message4_count);
      CHECK_EQUAL(0, router1.message5_count);
      CHECK_EQUAL(0, router1.message_unknown_count);

      CHECK_EQUAL(1, router2.message1_count);
      CHECK_EQUAL(0, router2.message2_count);
      CHECK_EQUAL(0, router2.message4_count);
      CHECK_EQUAL(0, router2.message5_count);
      CHECK_EQUAL(0, router2.message_unknown_count);

      CHECK_EQUAL(2, callback.message5_count);

      bus1.receive(message2);

      CHECK_EQUAL(1, router1.message1_count);
      CHECK_EQUAL(1, router1.message2_count);
      CHECK_EQUAL(0, router1.message3_count);
      CHECK_EQUAL(0, router1.message4_count);
      CHECK_EQUAL(0, router1.message5_count);
      CHECK_EQUAL(0, router1.message_unknown_count);

      CHECK_EQUAL(1, router2.message1_count);
      CHECK_EQUAL(1, router2.message2_count);
      CHECK_EQUAL(0, router2.message4_count);
      CHECK_EQUAL(0, router2.message5_count);
      CHECK_EQUAL(0, router2.message_unknown_count);

      CHECK_EQUAL(4, callback.message5_count);

      bus1.receive(message3);

      CHECK_EQUAL(1, router1.message1_count);
      CHECK_EQUAL(1, router1.message2_count);
      CHECK_EQUAL(1, router1.message3_count);
      CHECK_EQUAL(0, router1.message4_count);
      CHECK_EQUAL(0, router1.message5_count);
      CHECK_EQUAL(0, router1.message_unknown_count);

      CHECK_EQUAL(1, router2.message1_count);
      CHECK_EQUAL(1, router2.message2_count);
      CHECK_EQUAL(0, router2.message4_count);
      CHECK_EQUAL(0, router2.message5_count);
      CHECK_EQUAL(0, router2.message_unknown_count);

      CHECK_EQUAL(5, callback.message5_count);

      // Use global function.
      etl::send_message(bus1, message4);

      CHECK_EQUAL(1, router1.message1_count);
      CHECK_EQUAL(1, router1.message2_count);
      CHECK_EQUAL(1, router1.message3_count);
      CHECK_EQUAL(1, router1.message4_count);
      CHECK_EQUAL(0, router1.message5_count);
      CHECK_EQUAL(0, router1.message_unknown_count);

      CHECK_EQUAL(1, router2.message1_count);
      CHECK_EQUAL(1, router2.message2_count);
      CHECK_EQUAL(1, router2.message4_count);
      CHECK_EQUAL(0, router2.message5_count);
      CHECK_EQUAL(0, router2.message_unknown_count);

      CHECK_EQUAL(7, callback.message5_count);

      CHECK_EQUAL(4, bus1.message_count);
    }

    //*************************************************************************
    TEST(message_bus_broadcast_as_router)
    {
      MessageBus<2> bus1;

      RouterA router1(ROUTER1);
      RouterB router2(ROUTER2);
      RouterA callback(ROUTER3);

      bus1.subscribe(router1);
      bus1.subscribe(router2);

      Message1 message1(callback);
      Message2 message2(callback);
      Message3 message3(callback);
      Message4 message4(callback);

      // Reference to router sub-type
      etl::imessage_router& irouter = bus1;

      irouter.receive(message1);

      CHECK_EQUAL(1, router1.message1_count);
      CHECK_EQUAL(0, router1.message2_count);
      CHECK_EQUAL(0, router1.message3_count);
      CHECK_EQUAL(0, router1.message4_count);
      CHECK_EQUAL(0, router1.message5_count);
      CHECK_EQUAL(0, router1.message_unknown_count);

      CHECK_EQUAL(1, router2.message1_count);
      CHECK_EQUAL(0, router2.message2_count);
      CHECK_EQUAL(0, router2.message4_count);
      CHECK_EQUAL(0, router2.message5_count);
      CHECK_EQUAL(0, router2.message_unknown_count);

      CHECK_EQUAL(2, callback.message5_count);

      irouter.receive(message2);

      CHECK_EQUAL(1, router1.message1_count);
      CHECK_EQUAL(1, router1.message2_count);
      CHECK_EQUAL(0, router1.message3_count);
      CHECK_EQUAL(0, router1.message4_count);
      CHECK_EQUAL(0, router1.message5_count);
      CHECK_EQUAL(0, router1.message_unknown_count);

      CHECK_EQUAL(1, router2.message1_count);
      CHECK_EQUAL(1, router2.message2_count);
      CHECK_EQUAL(0, router2.message4_count);
      CHECK_EQUAL(0, router2.message5_count);
      CHECK_EQUAL(0, router2.message_unknown_count);

      CHECK_EQUAL(4, callback.message5_count);

      irouter.receive(message3);

      CHECK_EQUAL(1, router1.message1_count);
      CHECK_EQUAL(1, router1.message2_count);
      CHECK_EQUAL(1, router1.message3_count);
      CHECK_EQUAL(0, router1.message4_count);
      CHECK_EQUAL(0, router1.message5_count);
      CHECK_EQUAL(0, router1.message_unknown_count);

      CHECK_EQUAL(1, router2.message1_count);
      CHECK_EQUAL(1, router2.message2_count);
      CHECK_EQUAL(0, router2.message4_count);
      CHECK_EQUAL(0, router2.message5_count);
      CHECK_EQUAL(0, router2.message_unknown_count);

      CHECK_EQUAL(5, callback.message5_count);

      // Use global function.
      etl::send_message(irouter, message4);

      CHECK_EQUAL(1, router1.message1_count);
      CHECK_EQUAL(1, router1.message2_count);
      CHECK_EQUAL(1, router1.message3_count);
      CHECK_EQUAL(1, router1.message4_count);
      CHECK_EQUAL(0, router1.message5_count);
      CHECK_EQUAL(0, router1.message_unknown_count);

      CHECK_EQUAL(1, router2.message1_count);
      CHECK_EQUAL(1, router2.message2_count);
      CHECK_EQUAL(1, router2.message4_count);
      CHECK_EQUAL(0, router2.message5_count);
      CHECK_EQUAL(0, router2.message_unknown_count);

      CHECK_EQUAL(7, callback.message5_count);

      CHECK_EQUAL(4, bus1.message_count);
    }

    //*************************************************************************
    TEST(message_bus_addressed)
    {
      MessageBus<2> bus1;

      RouterA router1(ROUTER1);
      RouterB router2(ROUTER2);
      RouterA callback(ROUTER3);

      bus1.subscribe(router1);
      bus1.subscribe(router2);

      Message1 message1(callback);
      Message2 message2(callback);
      Message3 message3(callback);
      Message4 message4(callback);

      bus1.receive(ROUTER1, message1);

      CHECK_EQUAL(1, router1.message1_count);
      CHECK_EQUAL(0, router1.message2_count);
      CHECK_EQUAL(0, router1.message3_count);
      CHECK_EQUAL(0, router1.message4_count);
      CHECK_EQUAL(0, router1.message5_count);
      CHECK_EQUAL(0, router1.message_unknown_count);

      CHECK_EQUAL(0, router2.message1_count);
      CHECK_EQUAL(0, router2.message2_count);
      CHECK_EQUAL(0, router2.message4_count);
      CHECK_EQUAL(0, router2.message5_count);
      CHECK_EQUAL(0, router2.message_unknown_count);

      CHECK_EQUAL(1, callback.message5_count);

      bus1.receive(ROUTER2, message2);

      CHECK_EQUAL(1, router1.message1_count);
      CHECK_EQUAL(0, router1.message2_count);
      CHECK_EQUAL(0, router1.message3_count);
      CHECK_EQUAL(0, router1.message4_count);
      CHECK_EQUAL(0, router1.message5_count);
      CHECK_EQUAL(0, router1.message_unknown_count);

      CHECK_EQUAL(0, router2.message1_count);
      CHECK_EQUAL(1, router2.message2_count);
      CHECK_EQUAL(0, router2.message4_count);
      CHECK_EQUAL(0, router2.message5_count);
      CHECK_EQUAL(0, router2.message_unknown_count);

      CHECK_EQUAL(2, callback.message5_count);

      bus1.receive(ROUTER1, message3);

      CHECK_EQUAL(1, router1.message1_count);
      CHECK_EQUAL(0, router1.message2_count);
      CHECK_EQUAL(1, router1.message3_count);
      CHECK_EQUAL(0, router1.message4_count);
      CHECK_EQUAL(0, router1.message5_count);
      CHECK_EQUAL(0, router1.message_unknown_count);

      CHECK_EQUAL(0, router2.message1_count);
      CHECK_EQUAL(1, router2.message2_count);
      CHECK_EQUAL(0, router2.message4_count);
      CHECK_EQUAL(0, router2.message5_count);
      CHECK_EQUAL(0, router2.message_unknown_count);

      CHECK_EQUAL(3, callback.message5_count);

      // Use global function.
      etl::send_message(bus1, ROUTER2, message4);

      CHECK_EQUAL(1, router1.message1_count);
      CHECK_EQUAL(0, router1.message2_count);
      CHECK_EQUAL(1, router1.message3_count);
      CHECK_EQUAL(0, router1.message4_count);
      CHECK_EQUAL(0, router1.message5_count);
      CHECK_EQUAL(0, router1.message_unknown_count);

      CHECK_EQUAL(0, router2.message1_count);
      CHECK_EQUAL(1, router2.message2_count);
      CHECK_EQUAL(1, router2.message4_count);
      CHECK_EQUAL(0, router2.message5_count);
      CHECK_EQUAL(0, router2.message_unknown_count);

      CHECK_EQUAL(4, callback.message5_count);

      // Send to a router not subscribed to the bus.
      bus1.receive(ROUTER5, message1);

      CHECK_EQUAL(1, router1.message1_count);
      CHECK_EQUAL(0, router1.message2_count);
      CHECK_EQUAL(1, router1.message3_count);
      CHECK_EQUAL(0, router1.message4_count);
      CHECK_EQUAL(0, router1.message5_count);
      CHECK_EQUAL(0, router1.message_unknown_count);

      CHECK_EQUAL(0, router2.message1_count);
      CHECK_EQUAL(1, router2.message2_count);
      CHECK_EQUAL(1, router2.message4_count);
      CHECK_EQUAL(0, router2.message5_count);
      CHECK_EQUAL(0, router2.message_unknown_count);

      CHECK_EQUAL(4, callback.message5_count);
      
      CHECK_EQUAL(5, bus1.message_count);
    }

    //*************************************************************************
    TEST(message_bus_addressed_duplicate_router_id)
    {
      MessageBus<3> bus1;

      RouterA router1(ROUTER1);
      RouterB router2(ROUTER1);
      RouterB router3(ROUTER2);
      RouterA callback(ROUTER3);

      bus1.subscribe(router1);
      bus1.subscribe(router2);
      bus1.subscribe(router3);

      Message1 message1(callback);
      Message2 message2(callback);
      Message3 message3(callback);

      bus1.receive(ROUTER1, message1);

      CHECK_EQUAL(1, router1.message1_count);
      CHECK_EQUAL(0, router1.message2_count);
      CHECK_EQUAL(0, router1.message3_count);
      CHECK_EQUAL(0, router1.message4_count);
      CHECK_EQUAL(0, router1.message5_count);
      CHECK_EQUAL(0, router1.message_unknown_count);

      CHECK_EQUAL(1, router2.message1_count);
      CHECK_EQUAL(0, router2.message2_count);
      CHECK_EQUAL(0, router2.message4_count);
      CHECK_EQUAL(0, router2.message5_count);
      CHECK_EQUAL(0, router2.message_unknown_count);

      CHECK_EQUAL(0, router3.message1_count);
      CHECK_EQUAL(0, router3.message2_count);
      CHECK_EQUAL(0, router3.message4_count);
      CHECK_EQUAL(0, router3.message5_count);
      CHECK_EQUAL(0, router3.message_unknown_count);

      CHECK_EQUAL(2, callback.message5_count);

      CHECK_EQUAL(1, bus1.message_count);
    }

    //*************************************************************************
    TEST(message_bus_broadcast_addressed_sub_bus)
    {
      MessageBus<3> bus1;
      MessageBus<2> bus2;

      RouterA router1(ROUTER1);
      RouterA router2(ROUTER2);
      RouterA router3(ROUTER3);
      RouterA router4(ROUTER4);

      RouterA callback(ROUTER5);

      bus1.subscribe(router1);
      bus1.subscribe(router2);
      bus1.subscribe(bus2);

      bus2.subscribe(router3);
      bus2.subscribe(router4);

      Message1 message1(callback);
      Message2 message2(callback);
      Message3 message3(callback);
      Message4 message4(callback);

      // Broadcast to bus1
      bus1.receive(message1);

      CHECK_EQUAL(1, router1.message1_count);
      CHECK_EQUAL(0, router1.message2_count);
      CHECK_EQUAL(0, router1.message3_count);
      CHECK_EQUAL(0, router1.message4_count);
      CHECK_EQUAL(0, router1.message5_count);
      CHECK_EQUAL(0, router1.message_unknown_count);

      CHECK_EQUAL(1, router2.message1_count);
      CHECK_EQUAL(0, router2.message2_count);
      CHECK_EQUAL(0, router2.message4_count);
      CHECK_EQUAL(0, router2.message5_count);
      CHECK_EQUAL(0, router2.message_unknown_count);

      CHECK_EQUAL(1, router3.message1_count);
      CHECK_EQUAL(0, router3.message2_count);
      CHECK_EQUAL(0, router3.message4_count);
      CHECK_EQUAL(0, router3.message5_count);
      CHECK_EQUAL(0, router3.message_unknown_count);

      CHECK_EQUAL(1, router4.message1_count);
      CHECK_EQUAL(0, router4.message2_count);
      CHECK_EQUAL(0, router4.message4_count);
      CHECK_EQUAL(0, router4.message5_count);
      CHECK_EQUAL(0, router4.message_unknown_count);

      CHECK_EQUAL(4, callback.message5_count);

      // Addressed to ROUTER2
      bus1.receive(ROUTER2, message1);

      CHECK_EQUAL(1, router1.message1_count);
      CHECK_EQUAL(0, router1.message2_count);
      CHECK_EQUAL(0, router1.message3_count);
      CHECK_EQUAL(0, router1.message4_count);
      CHECK_EQUAL(0, router1.message5_count);
      CHECK_EQUAL(0, router1.message_unknown_count);

      CHECK_EQUAL(2, router2.message1_count);
      CHECK_EQUAL(0, router2.message2_count);
      CHECK_EQUAL(0, router2.message4_count);
      CHECK_EQUAL(0, router2.message5_count);
      CHECK_EQUAL(0, router2.message_unknown_count);

      CHECK_EQUAL(1, router3.message1_count);
      CHECK_EQUAL(0, router3.message2_count);
      CHECK_EQUAL(0, router3.message4_count);
      CHECK_EQUAL(0, router3.message5_count);
      CHECK_EQUAL(0, router3.message_unknown_count);

      CHECK_EQUAL(1, router4.message1_count);
      CHECK_EQUAL(0, router4.message2_count);
      CHECK_EQUAL(0, router4.message4_count);
      CHECK_EQUAL(0, router4.message5_count);
      CHECK_EQUAL(0, router4.message_unknown_count);

      CHECK_EQUAL(5, callback.message5_count);

      // Addressed to ROUTER3 via bus2
      bus1.receive(ROUTER3, message1);

      CHECK_EQUAL(1, router1.message1_count);
      CHECK_EQUAL(0, router1.message2_count);
      CHECK_EQUAL(0, router1.message3_count);
      CHECK_EQUAL(0, router1.message4_count);
      CHECK_EQUAL(0, router1.message5_count);
      CHECK_EQUAL(0, router1.message_unknown_count);

      CHECK_EQUAL(2, router2.message1_count);
      CHECK_EQUAL(0, router2.message2_count);
      CHECK_EQUAL(0, router2.message4_count);
      CHECK_EQUAL(0, router2.message5_count);
      CHECK_EQUAL(0, router2.message_unknown_count);

      CHECK_EQUAL(2, router3.message1_count);
      CHECK_EQUAL(0, router3.message2_count);
      CHECK_EQUAL(0, router3.message4_count);
      CHECK_EQUAL(0, router3.message5_count);
      CHECK_EQUAL(0, router3.message_unknown_count);

      CHECK_EQUAL(1, router4.message1_count);
      CHECK_EQUAL(0, router4.message2_count);
      CHECK_EQUAL(0, router4.message4_count);
      CHECK_EQUAL(0, router4.message5_count);
      CHECK_EQUAL(0, router4.message_unknown_count);

      CHECK_EQUAL(6, callback.message5_count);

      CHECK_EQUAL(3, bus1.message_count);
      CHECK_EQUAL(3, bus2.message_count);
    }

    //*************************************************************************
    TEST(message_bus_broadcast_order)
    {
      MessageBus<4> bus1;
      MessageBus<2> bus2;
      MessageBus<2> bus3;

      RouterA router1(ROUTER1);
      RouterA router2(ROUTER2);
      RouterA router3(ROUTER3);
      RouterA router4a(ROUTER4);
      RouterA router4b(ROUTER4);

      RouterA callback(ROUTER5);

      bus1.subscribe(router1);
      bus1.subscribe(bus3);
      bus1.subscribe(bus2);
      bus1.subscribe(router2);

      bus2.subscribe(router3);
      bus3.subscribe(router4b);
      bus3.subscribe(router4a);

      Message1 message1(callback);

      call_order = 0;

      bus1.receive(message1);

      CHECK_EQUAL(0, router1.order);
      CHECK_EQUAL(1, router2.order);
      CHECK_EQUAL(2, router4b.order);
      CHECK_EQUAL(3, router4a.order);
      CHECK_EQUAL(4, router3.order);

      CHECK_EQUAL(1, bus1.message_count);
      CHECK_EQUAL(1, bus2.message_count);
      CHECK_EQUAL(1, bus3.message_count);
    }

    //*************************************************************************
    TEST(message_bus_broadcast_addressed_successor_bus)
    {
      MessageBus<3> bus1;
      MessageBus<2> bus2;

      RouterA router1(ROUTER1);
      RouterA router2(ROUTER2);
      RouterA router3(ROUTER3);
      RouterA router4(ROUTER4);

      RouterA callback(ROUTER5);

      bus1.subscribe(router1);
      bus1.subscribe(router2);
      bus1.set_successor(bus2);

      bus2.subscribe(router3);
      bus2.subscribe(router4);

      Message1 message1(callback);
      Message2 message2(callback);
      Message3 message3(callback);
      Message4 message4(callback);

      // Broadcast to bus1
      bus1.receive(message1);

      CHECK_EQUAL(1, router1.message1_count);
      CHECK_EQUAL(0, router1.message2_count);
      CHECK_EQUAL(0, router1.message3_count);
      CHECK_EQUAL(0, router1.message4_count);
      CHECK_EQUAL(0, router1.message5_count);
      CHECK_EQUAL(0, router1.message_unknown_count);

      CHECK_EQUAL(1, router2.message1_count);
      CHECK_EQUAL(0, router2.message2_count);
      CHECK_EQUAL(0, router2.message4_count);
      CHECK_EQUAL(0, router2.message5_count);
      CHECK_EQUAL(0, router2.message_unknown_count);

      CHECK_EQUAL(1, router3.message1_count);
      CHECK_EQUAL(0, router3.message2_count);
      CHECK_EQUAL(0, router3.message4_count);
      CHECK_EQUAL(0, router3.message5_count);
      CHECK_EQUAL(0, router3.message_unknown_count);

      CHECK_EQUAL(1, router4.message1_count);
      CHECK_EQUAL(0, router4.message2_count);
      CHECK_EQUAL(0, router4.message4_count);
      CHECK_EQUAL(0, router4.message5_count);
      CHECK_EQUAL(0, router4.message_unknown_count);

      CHECK_EQUAL(4, callback.message5_count);

      // Addressed to ROUTER2
      bus1.receive(ROUTER2, message1);

      CHECK_EQUAL(1, router1.message1_count);
      CHECK_EQUAL(0, router1.message2_count);
      CHECK_EQUAL(0, router1.message3_count);
      CHECK_EQUAL(0, router1.message4_count);
      CHECK_EQUAL(0, router1.message5_count);
      CHECK_EQUAL(0, router1.message_unknown_count);

      CHECK_EQUAL(2, router2.message1_count);
      CHECK_EQUAL(0, router2.message2_count);
      CHECK_EQUAL(0, router2.message4_count);
      CHECK_EQUAL(0, router2.message5_count);
      CHECK_EQUAL(0, router2.message_unknown_count);

      CHECK_EQUAL(1, router3.message1_count);
      CHECK_EQUAL(0, router3.message2_count);
      CHECK_EQUAL(0, router3.message4_count);
      CHECK_EQUAL(0, router3.message5_count);
      CHECK_EQUAL(0, router3.message_unknown_count);

      CHECK_EQUAL(1, router4.message1_count);
      CHECK_EQUAL(0, router4.message2_count);
      CHECK_EQUAL(0, router4.message4_count);
      CHECK_EQUAL(0, router4.message5_count);
      CHECK_EQUAL(0, router4.message_unknown_count);

      CHECK_EQUAL(5, callback.message5_count);

      // Addressed to ROUTER3 via bus2
      bus1.receive(ROUTER3, message1);

      CHECK_EQUAL(1, router1.message1_count);
      CHECK_EQUAL(0, router1.message2_count);
      CHECK_EQUAL(0, router1.message3_count);
      CHECK_EQUAL(0, router1.message4_count);
      CHECK_EQUAL(0, router1.message5_count);
      CHECK_EQUAL(0, router1.message_unknown_count);

      CHECK_EQUAL(2, router2.message1_count);
      CHECK_EQUAL(0, router2.message2_count);
      CHECK_EQUAL(0, router2.message4_count);
      CHECK_EQUAL(0, router2.message5_count);
      CHECK_EQUAL(0, router2.message_unknown_count);

      CHECK_EQUAL(2, router3.message1_count);
      CHECK_EQUAL(0, router3.message2_count);
      CHECK_EQUAL(0, router3.message4_count);
      CHECK_EQUAL(0, router3.message5_count);
      CHECK_EQUAL(0, router3.message_unknown_count);

      CHECK_EQUAL(1, router4.message1_count);
      CHECK_EQUAL(0, router4.message2_count);
      CHECK_EQUAL(0, router4.message4_count);
      CHECK_EQUAL(0, router4.message5_count);
      CHECK_EQUAL(0, router4.message_unknown_count);

      CHECK_EQUAL(6, callback.message5_count);

      CHECK_EQUAL(3, bus1.message_count);
      CHECK_EQUAL(3, bus2.message_count);
    }

    //*************************************************************************
    TEST(message_bus_successor_to_message_producer)
    {
      MessageBus<2> bus;
      etl::message_producer producer(ROUTER2);
      RouterA router(ROUTER1);
      Response message;

      bus.subscribe(router);
      bus.subscribe(producer);
      producer.set_successor(bus);

      CHECK_EQUAL(0U, router.message5_count);

      producer.receive(message);

      CHECK_EQUAL(1U, router.message5_count);

      CHECK_EQUAL(1, bus.message_count);
    }

    //*************************************************************************
    TEST(message_bus_accepts)
    {
      MessageBus<2> bus1;
      MessageBus<2> bus2;

      RouterA router1(ROUTER1);
      RouterB router2(ROUTER2);
      RouterC router3(ROUTER3);

      bus1.subscribe(router1);
      bus1.subscribe(router2);
      bus1.set_successor(bus2);

      bus2.subscribe(router3);

      CHECK_TRUE(bus1.accepts(MESSAGE1));
      CHECK_TRUE(bus1.accepts(MESSAGE2));
      CHECK_TRUE(bus1.accepts(MESSAGE3));
      CHECK_TRUE(bus1.accepts(MESSAGE4));
      CHECK_TRUE(bus1.accepts(MESSAGE5));
      CHECK_TRUE(bus1.accepts(MESSAGE6));
      CHECK_FALSE(bus1.accepts(MESSAGE7));
    }
  };
}
