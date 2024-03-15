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
#include <vector>

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
    UNKNOWN_MESSAGE
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
    {
    }
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

  struct Message6 : public etl::message<MESSAGE6>
  {
    Message6()
    {
    }
  };

  struct UnknownMessage : public etl::message<UNKNOWN_MESSAGE>
  {
    UnknownMessage()
    {
    }
  };

  //***************************************************************************
  // Broker
  //***************************************************************************
  class Broker : public etl::message_broker
  {
  public:

    Broker()
      : message_broker()
    {
    }

    Broker(etl::message_router_id_t id)
      : message_broker(id)
    {
    }

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
  // Router that handles messages 1, 2, 3, 4, 5.
  //***************************************************************************
  class Router : public etl::message_router<Router, Message1, Message2, Message3, Message4, Message5, Message6>
  {
  public:

    Router(etl::message_router_id_t id)
      : message_router(id)
      , message1_count(0)
      , message2_count(0)
      , message3_count(0)
      , message4_count(0)
      , message5_count(0)
      , message6_count(0)
      , message_unknown_count(0)
    {
    }

    void clear()
    {
      message1_count = 0;
      message2_count = 0;
      message3_count = 0;
      message4_count = 0;
      message5_count = 0;
      message6_count = 0;
      message_unknown_count = 0;
    }

    void on_receive(const Message1&)
    {
      ++message1_count;
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

    void on_receive(const Message6&)
    {
      ++message6_count;
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
    int message6_count;
    int message_unknown_count;
  };

  //*************************************************************************
  class Subscription : public etl::message_broker::subscription
  {
  public:

    Subscription(etl::imessage_router& router, std::initializer_list<etl::message_id_t> init)
      : etl::message_broker::subscription(router)
      , id_list(init)
    {
    }

    virtual etl::message_broker::message_id_span_t message_id_list() const
    {
      return etl::message_broker::message_id_span_t(id_list.begin(), id_list.end());
    }

    std::vector<etl::message_id_t> id_list;
  };

  SUITE(test_message_broker)
  {
    //*************************************************************************
    TEST(test_message_check_broker_id)
    {
      Broker broker1;
      Broker broker2(2);

      CHECK_EQUAL(etl::imessage_router::MESSAGE_BROKER, broker1.get_message_router_id());
      CHECK_EQUAL(2, broker2.get_message_router_id());

      CHECK(broker1.is_consumer());
      CHECK(broker1.is_producer());
    }

    //*************************************************************************
    TEST(test_message_broker_subscribe_then_unsubscribe)
    {
      Router router1(1);
      Router router2(2);
      Router router3(3);

      Subscription subscription1{ router1, { Message1::ID, Message2::ID, Message3::ID, Message4::ID } };
      Subscription subscription3{ router2, { Message1::ID, Message3::ID } };

      Broker broker; 
      CHECK_TRUE(broker.empty());
      
      broker.subscribe(subscription1);
      CHECK_FALSE(broker.empty());

      broker.subscribe(subscription3);
      CHECK_FALSE(broker.empty());

      broker.unsubscribe(router1);
      CHECK_FALSE(broker.empty());

      broker.unsubscribe(router2);
      CHECK_TRUE(broker.empty());

      // There is no router3 subscription in 'broker'.
      broker.unsubscribe(router3);
      CHECK_TRUE(broker.empty());
    }

    //*************************************************************************
    TEST(test_message_broker_subscribe_then_clear)
    {
      Router router1(1);
      Router router2(2);
      Router router3(3);

      Subscription subscription1{ router1, { Message1::ID, Message2::ID, Message3::ID, Message4::ID } };
      Subscription subscription3{ router2, { Message1::ID, Message3::ID } };

      Broker broker;

      broker.subscribe(subscription1);
      broker.subscribe(subscription3); // Duplicate router. Replace the old subscription.
      broker.unsubscribe(router1);

      broker.clear();
      CHECK(broker.empty());

      broker.receive(Message1());
      broker.receive(Message2());
      broker.receive(Message3());
      broker.receive(Message4());
      broker.receive(Message5());
      broker.receive(Message6());
      broker.receive(UnknownMessage());

      CHECK_EQUAL(0, router1.message1_count);
      CHECK_EQUAL(0, router2.message1_count);
      CHECK_EQUAL(0, router3.message1_count);

      CHECK_EQUAL(0, router1.message2_count);
      CHECK_EQUAL(0, router2.message2_count);
      CHECK_EQUAL(0, router3.message2_count);

      CHECK_EQUAL(0, router1.message3_count);
      CHECK_EQUAL(0, router2.message3_count);
      CHECK_EQUAL(0, router3.message3_count);

      CHECK_EQUAL(0, router1.message4_count);
      CHECK_EQUAL(0, router2.message4_count);
      CHECK_EQUAL(0, router3.message4_count);

      CHECK_EQUAL(0, router1.message5_count);
      CHECK_EQUAL(0, router2.message5_count);
      CHECK_EQUAL(0, router3.message5_count);

      CHECK_EQUAL(0, router1.message6_count);
      CHECK_EQUAL(0, router2.message6_count);
      CHECK_EQUAL(0, router3.message6_count);

      CHECK_EQUAL(0, router1.message_unknown_count);
      CHECK_EQUAL(0, router2.message_unknown_count);
      CHECK_EQUAL(0, router3.message_unknown_count);
    }

    //*************************************************************************
    TEST(test_message_broker_send_messages_to_broker_with_no_subscribers)
    {
      Broker broker;
      Router router1(1);
      Router router2(2);
      Router router3(3);

      broker.receive(Message1());
      broker.receive(Message2());
      broker.receive(Message3());
      broker.receive(Message4());
      broker.receive(Message5());
      broker.receive(Message6());
      broker.receive(UnknownMessage());

      CHECK_EQUAL(0, router1.message1_count);
      CHECK_EQUAL(0, router2.message1_count);
      CHECK_EQUAL(0, router3.message1_count);

      CHECK_EQUAL(0, router1.message2_count);
      CHECK_EQUAL(0, router2.message2_count);
      CHECK_EQUAL(0, router3.message2_count);

      CHECK_EQUAL(0, router1.message3_count);
      CHECK_EQUAL(0, router2.message3_count);
      CHECK_EQUAL(0, router3.message3_count);

      CHECK_EQUAL(0, router1.message4_count);
      CHECK_EQUAL(0, router2.message4_count);
      CHECK_EQUAL(0, router3.message4_count);

      CHECK_EQUAL(0, router1.message5_count);
      CHECK_EQUAL(0, router2.message5_count);
      CHECK_EQUAL(0, router3.message5_count);

      CHECK_EQUAL(0, router1.message6_count);
      CHECK_EQUAL(0, router2.message6_count);
      CHECK_EQUAL(0, router3.message6_count);

      CHECK_EQUAL(0, router1.message_unknown_count);
      CHECK_EQUAL(0, router2.message_unknown_count);
      CHECK_EQUAL(0, router3.message_unknown_count);
    }

    //*************************************************************************
    TEST(test_message_broker_send_messages_to_subscribers)
    {
      Broker broker;
      Router router1(1);
      Router router2(2);
      Router router3(3);

      Subscription subscription1{ router1, { Message1::ID, Message2::ID, Message3::ID, Message4::ID } };
      Subscription subscription2{ router2, { Message1::ID, Message2::ID } };
      Subscription subscription3{ router2, { Message1::ID, Message3::ID } };

      broker.subscribe(subscription1);
      broker.subscribe(subscription2);
      broker.subscribe(subscription3); // Duplicate router. Replace the old subscription.
      broker.subscribe(subscription1); // Do subscription1 again to see if it breaks.

      broker.set_successor(router3);

      broker.receive(Message1());
      broker.receive(Message2());
      broker.receive(Message3());
      broker.receive(Message4());
      broker.receive(Message5());
      broker.receive(Message6());
      broker.receive(UnknownMessage());

      CHECK_EQUAL(1, router1.message1_count);
      CHECK_EQUAL(1, router2.message1_count);
      CHECK_EQUAL(1, router3.message1_count);

      CHECK_EQUAL(1, router1.message2_count);
      CHECK_EQUAL(0, router2.message2_count);
      CHECK_EQUAL(1, router3.message2_count);

      CHECK_EQUAL(1, router1.message3_count);
      CHECK_EQUAL(1, router2.message3_count);
      CHECK_EQUAL(1, router3.message3_count);

      CHECK_EQUAL(2, router1.message4_count);
      CHECK_EQUAL(0, router2.message4_count);
      CHECK_EQUAL(2, router3.message4_count);

      // Message5 is translated to Message4 in 'broker'.
      CHECK_EQUAL(0, router1.message5_count);
      CHECK_EQUAL(0, router2.message5_count);
      CHECK_EQUAL(0, router3.message5_count);

      CHECK_EQUAL(0, router1.message6_count);
      CHECK_EQUAL(0, router2.message6_count);
      CHECK_EQUAL(1, router3.message6_count);

      CHECK_EQUAL(0, router1.message_unknown_count);
      CHECK_EQUAL(0, router2.message_unknown_count);
      CHECK_EQUAL(1, router3.message_unknown_count);
    }

    //*************************************************************************
    TEST(test_message_broker_send_messages_to_specific_subscribers)
    {
      Broker broker;
      Router router1(1);
      Router router2(2);
      Router router3(3);

      Subscription subscription1{ router1, { Message1::ID, Message2::ID, Message3::ID, Message4::ID } };
      Subscription subscription2{ router2, { Message1::ID, Message2::ID } };
      Subscription subscription3{ router2, { Message1::ID, Message3::ID } };

      broker.subscribe(subscription1);
      broker.subscribe(subscription2);
      broker.set_successor(router3);

      broker.receive(Message1());
      broker.receive(1, Message1());
      broker.receive(2, Message2());
    }

    //*************************************************************************
    TEST(test_message_broker_send_messages_to_subscribers_after_unsubscribe)
    {
      Broker broker;
      Router router1(1);
      Router router2(2);
      Router router3(3);

      Subscription subscription1{ router1, { Message1::ID, Message2::ID, Message3::ID, Message4::ID } };
      Subscription subscription3{ router2, { Message1::ID, Message3::ID } };

      broker.subscribe(subscription1);
      broker.subscribe(subscription3);

      broker.set_successor(router3);

      broker.unsubscribe(router2);

      broker.receive(Message1());
      broker.receive(Message2());
      broker.receive(Message3());
      broker.receive(Message4());
      broker.receive(Message5());
      broker.receive(Message6());
      broker.receive(UnknownMessage());

      CHECK(broker.has_successor());

      CHECK_EQUAL(1, router1.message1_count);
      CHECK_EQUAL(0, router2.message1_count);
      CHECK_EQUAL(1, router3.message1_count);

      CHECK_EQUAL(1, router1.message2_count);
      CHECK_EQUAL(0, router2.message2_count);
      CHECK_EQUAL(1, router3.message2_count);

      CHECK_EQUAL(1, router1.message3_count);
      CHECK_EQUAL(0, router2.message3_count);
      CHECK_EQUAL(1, router3.message3_count);

      CHECK_EQUAL(2, router1.message4_count);
      CHECK_EQUAL(0, router2.message4_count);
      CHECK_EQUAL(2, router3.message4_count);

      // Message5 is translated to Message4 in 'broker'.
      CHECK_EQUAL(0, router1.message5_count);
      CHECK_EQUAL(0, router2.message5_count);
      CHECK_EQUAL(0, router3.message5_count);

      CHECK_EQUAL(0, router1.message6_count);
      CHECK_EQUAL(0, router2.message6_count);
      CHECK_EQUAL(1, router3.message6_count);

      CHECK_EQUAL(0, router1.message_unknown_count);
      CHECK_EQUAL(0, router2.message_unknown_count);
      CHECK_EQUAL(1, router3.message_unknown_count);
    }

    //*************************************************************************
    TEST(test_message_broker_accepts)
    {
      Router router1(1);
      Router router2(2);
      Router router3(3);

      Subscription subscription1{ router1, { Message1::ID, Message3::ID } };
      Subscription subscription2{ router2, { Message1::ID, Message2::ID, Message3::ID, Message4::ID } };

      // Default constructed broker.
      Broker broker;

      CHECK_FALSE(broker.accepts(MESSAGE1));
      CHECK_FALSE(broker.accepts(MESSAGE2));
      CHECK_FALSE(broker.accepts(MESSAGE3));
      CHECK_FALSE(broker.accepts(MESSAGE4));
      CHECK_FALSE(broker.accepts(MESSAGE5));
      CHECK_FALSE(broker.accepts(MESSAGE6));

      // Subscribe router1.
      broker.subscribe(subscription1);
      CHECK_TRUE(broker.accepts(MESSAGE1));
      CHECK_FALSE(broker.accepts(MESSAGE2));
      CHECK_TRUE(broker.accepts(MESSAGE3));
      CHECK_FALSE(broker.accepts(MESSAGE4));
      CHECK_FALSE(broker.accepts(MESSAGE5));
      CHECK_FALSE(broker.accepts(MESSAGE6));

      // Subscribe router2.
      broker.subscribe(subscription2);
      CHECK_TRUE(broker.accepts(MESSAGE1));
      CHECK_TRUE(broker.accepts(MESSAGE2));
      CHECK_TRUE(broker.accepts(MESSAGE3));
      CHECK_TRUE(broker.accepts(MESSAGE4));
      CHECK_FALSE(broker.accepts(MESSAGE5));
      CHECK_FALSE(broker.accepts(MESSAGE6));

      // Set router3 as a successor.
      broker.set_successor(router3);
      CHECK_TRUE(broker.accepts(MESSAGE1));
      CHECK_TRUE(broker.accepts(MESSAGE2));
      CHECK_TRUE(broker.accepts(MESSAGE3));
      CHECK_TRUE(broker.accepts(MESSAGE4));
      CHECK_TRUE(broker.accepts(MESSAGE5));
      CHECK_TRUE(broker.accepts(MESSAGE6));
    }
  };
}
