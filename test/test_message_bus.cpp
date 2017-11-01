/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl
http://www.etlcpp.com

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

#include "UnitTest++.h"
#include "ExtraCheckMacros.h"

#include "message_router.h"
#include "message_bus.h"
#include "queue.h"
#include "largest.h"
#include "packet.h"

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
    ROUTER1 = 1,
    ROUTER2 = 2,
    ROUTER3 = 3,
    ROUTER4 = 4,
    ROUTER5 = 5
  };

  struct Message1 : public etl::message<MESSAGE1>
  {
  };

  struct Message2 : public etl::message<MESSAGE2>
  {
  };

  struct Message3 : public etl::message<MESSAGE3>
  {
    int value[10];
  };

  struct Message4 : public etl::message<MESSAGE4>
  {
  };

  struct Message5 : public etl::message<MESSAGE5>
  {
  };

  Message1 message1;
  Message2 message2;
  Message3 message3;
  Message4 message4;
  Message5 message5;

  int call_order;

  //***************************************************************************
  // Router that handles messages 1, 2, 3, 4, 5.
  //***************************************************************************
  class RouterA : public etl::message_router<RouterA, Message1, Message2, Message3, Message4, Message5>
  {
  public:

    RouterA(etl::message_router_id_t id)
      : message_router(id),
        message1_count(0),
        message2_count(0),
        message3_count(0),
        message4_count(0),
        message5_count(0),
        message_unknown_count(0)
    {

    }

    void on_receive(etl::imessage_router& sender, const Message1&)
    {
      ++message1_count;
      etl::send_message(sender, message5);

      order = call_order++;
    }

    void on_receive(etl::imessage_router& sender, const Message2&)
    {
      ++message2_count;
      etl::send_message(sender, message5);
    }

    void on_receive(etl::imessage_router& sender, const Message3&)
    {
      ++message3_count;
      etl::send_message(sender, message5);
    }

    void on_receive(etl::imessage_router& sender, const Message4&)
    {
      ++message4_count;
      etl::send_message(sender, message5);
    }

    void on_receive(etl::imessage_router&, const Message5&)
    {
      ++message5_count;
    }

    void on_receive_unknown(etl::imessage_router&, const etl::imessage&)
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
  class RouterB : public etl::message_router<RouterB, Message1, Message2, Message4, Message5>
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

    void on_receive(etl::imessage_router& sender, const Message1&)
    {
      ++message1_count;
      etl::send_message(sender, message5);
    }

    void on_receive(etl::imessage_router& sender, const Message2&)
    {
      ++message2_count;
      etl::send_message(sender, message5);
    }

    void on_receive(etl::imessage_router& sender, const Message4&)
    {
      ++message4_count;
      etl::send_message(sender, message5);
    }

    void on_receive(etl::imessage_router&, const Message5&)
    {
      ++message5_count;
    }

    void on_receive_unknown(etl::imessage_router& sender, const etl::imessage&)
    {
      ++message_unknown_count;
      etl::send_message(sender, message5);
    }

    int message1_count;
    int message2_count;
    int message4_count;
    int message5_count;
    int message_unknown_count;
  };

  etl::imessage_router* p_router;
  etl::imessage_bus*    p_bus;

  SUITE(test_message_router)
  {
    //=========================================================================
    TEST(message_bus_subscribe_unsubscribe)
    {
      etl::message_bus<2> bus1;

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
    }

    //=========================================================================
    TEST(message_bus_subscribe_unsubscribe_sub_bus)
    {
      etl::message_bus<4> bus1;
      etl::message_bus<2> bus2;
      etl::message_bus<3> bus3;

      RouterA router1(ROUTER1);
      RouterA router2(ROUTER2);
      RouterA router3(ROUTER3);
      RouterA router4(ROUTER4);

      bus1.subscribe(router1);
      bus1.subscribe(router2);
      bus1.subscribe(bus2);
      bus1.subscribe(bus3);

      bus2.subscribe(router3);
      bus3.subscribe(router4);

      CHECK_EQUAL(4U, bus1.size());

      bus1.unsubscribe(etl::imessage_bus::MESSAGE_BUS);
      CHECK_EQUAL(2U, bus1.size());

      bus1.unsubscribe(etl::imessage_bus::ALL_MESSAGE_ROUTERS);
      CHECK_EQUAL(0U, bus1.size());
    }

    //=========================================================================
    TEST(message_bus_broadcast)
    {
      etl::message_bus<2> bus1;

      RouterA router1(ROUTER1);
      RouterB router2(ROUTER2);
      RouterA sender(ROUTER3);

      bus1.subscribe(router1);
      bus1.subscribe(router2);

      bus1.receive(sender, message1);

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

      CHECK_EQUAL(2, sender.message5_count);

      bus1.receive(sender, message2);

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

      CHECK_EQUAL(4, sender.message5_count);

      bus1.receive(sender, message3);

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

      CHECK_EQUAL(5, sender.message5_count);

      // Use global function.
      etl::send_message(sender, bus1, message4);

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

      CHECK_EQUAL(7, sender.message5_count);
    }

    //=========================================================================
    TEST(message_bus_broadcast_as_router)
    {
      etl::message_bus<2> bus1;

      RouterA router1(ROUTER1);
      RouterB router2(ROUTER2);
      RouterA sender(ROUTER3);

      bus1.subscribe(router1);
      bus1.subscribe(router2);

      // Reference to router sub-type
      etl::imessage_router& irouter = bus1;

      irouter.receive(sender, message1);

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

      CHECK_EQUAL(2, sender.message5_count);

      irouter.receive(sender, message2);

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

      CHECK_EQUAL(4, sender.message5_count);

      irouter.receive(sender, message3);

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

      CHECK_EQUAL(5, sender.message5_count);

      // Use global function.
      etl::send_message(sender, irouter, message4);

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

      CHECK_EQUAL(7, sender.message5_count);
    }

    //=========================================================================
    TEST(message_bus_addressed)
    {
      etl::message_bus<2> bus1;

      RouterA router1(ROUTER1);
      RouterB router2(ROUTER2);
      RouterA sender(ROUTER3);

      bus1.subscribe(router1);
      bus1.subscribe(router2);

      bus1.receive(sender, ROUTER1, message1);

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

      CHECK_EQUAL(1, sender.message5_count);

      bus1.receive(sender, ROUTER2, message2);

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

      CHECK_EQUAL(2, sender.message5_count);

      bus1.receive(sender, ROUTER1, message3);

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

      CHECK_EQUAL(3, sender.message5_count);

      // Use global function.
      etl::send_message(sender, bus1, ROUTER2, message4);

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

      CHECK_EQUAL(4, sender.message5_count);

      // Send to a router not subscribed to the bus.
      bus1.receive(sender, ROUTER5, message1);

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

      CHECK_EQUAL(4, sender.message5_count);
    }

    //=========================================================================
    TEST(message_bus_addressed_duplicate_router_id)
    {
      etl::message_bus<3> bus1;

      RouterA router1(ROUTER1);
      RouterB router2(ROUTER1);
      RouterB router3(ROUTER2);
      RouterA sender(ROUTER3);

      bus1.subscribe(router1);
      bus1.subscribe(router2);
      bus1.subscribe(router3);

      bus1.receive(sender, ROUTER1, message1);

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

      CHECK_EQUAL(2, sender.message5_count);
    }

    //=========================================================================
    TEST(message_bus_broadcast_addressed_sub_bus)
    {
      etl::message_bus<3> bus1;
      etl::message_bus<2> bus2;

      RouterA router1(ROUTER1);
      RouterA router2(ROUTER2);
      RouterA router3(ROUTER3);
      RouterA router4(ROUTER4);

      RouterA sender(ROUTER5);

      bus1.subscribe(router1);
      bus1.subscribe(router2);
      bus1.subscribe(bus2);

      bus2.subscribe(router3);
      bus2.subscribe(router4);

      // Broadcast to bus1
      bus1.receive(sender, message1);

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

      CHECK_EQUAL(4, sender.message5_count);

      // Addressed to ROUTER2
      bus1.receive(sender, ROUTER2, message1);

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

      CHECK_EQUAL(5, sender.message5_count);

      // Addressed to ROUTER3 via bus2
      bus1.receive(sender, ROUTER3, message1);

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

      CHECK_EQUAL(6, sender.message5_count);
    }

    //=========================================================================
    TEST(message_bus_broadcast_order)
    {
      etl::message_bus<4> bus1;
      etl::message_bus<2> bus2;
      etl::message_bus<2> bus3;

      RouterA router1(ROUTER1);
      RouterA router2(ROUTER2);
      RouterA router3(ROUTER3);
      RouterA router4a(ROUTER4);
      RouterA router4b(ROUTER4);

      RouterA sender(ROUTER5);

      bus1.subscribe(router1);
      bus1.subscribe(bus3);
      bus1.subscribe(bus2);
      bus1.subscribe(router2);

      bus2.subscribe(router3);
      bus3.subscribe(router4b);
      bus3.subscribe(router4a);

      call_order = 0;

      bus1.receive(sender, message1);

      CHECK_EQUAL(0, router1.order);
      CHECK_EQUAL(1, router2.order);
      CHECK_EQUAL(2, router4b.order);
      CHECK_EQUAL(3, router4a.order);
      CHECK_EQUAL(4, router3.order);
    }
  };
}
