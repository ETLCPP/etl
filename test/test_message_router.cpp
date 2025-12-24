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
#include "etl/queue.h"
#include "etl/largest.h"

//***************************************************************************
// The set of messages.
//***************************************************************************
namespace
{
  enum
  {
    MESSAGE0,
    MESSAGE1,
    MESSAGE2,
    MESSAGE3,
    MESSAGE4,
    MESSAGE5
  };

  enum
  {
    ROUTER1,
    ROUTER2,
    ROUTER3
  };

  //***********************************
  struct NotInterface
  {
    virtual ~NotInterface() {}
    virtual int VirtualFunction() const = 0;
  };

  ////***********************************
  // Uncomment to demonstrate static assert
  //struct Message0 : public etl::message<MESSAGE0, NotInterface>
  //{
  //};

  //***********************************
  struct Message1 : public etl::message<MESSAGE1>
  {
    Message1(etl::imessage_router& callback_)
      : callback(callback_)
    {
    }

    etl::imessage_router& callback;
  };

  //***********************************
  struct Message2 : public etl::message<MESSAGE2>
  {
    Message2(etl::imessage_router& callback_)
      : callback(callback_)
    {
    }

    etl::imessage_router& callback;
  };

  //***********************************
  struct Message3 : public etl::message<MESSAGE3>
  {
    Message3(etl::imessage_router& callback_)
      : callback(callback_)
    {
    }

    etl::imessage_router& callback;
    int value[10];
  };

  //***********************************
  struct Message4 : public etl::message<MESSAGE4>
  {
    Message4(etl::imessage_router& callback_)
      : callback(callback_)
    {
    }

    etl::imessage_router& callback;
  };

  //***********************************
  struct Message5 : public etl::message<MESSAGE5>
  {
  };

  Message5 message5;

  //***************************************************************************
  // Router that handles messages 1, 2, 3, 4 and 5 and returns nothing.
  //***************************************************************************
  class Router1 : public etl::message_router<Router1, Message1, Message2, Message3, Message4, Message5>
  {
  public:

    Router1()
      : message_router(ROUTER1),
        message1_count(0),
        message2_count(0),
        message3_count(0),
        message4_count(0),
        message_unknown_count(0),
        callback_count(0)
    {

    }

    void on_receive(const Message1& msg)
    {
      ++message1_count;
      etl::send_message(msg.callback, message5);
      //CHECK_EQUAL(1, msg.VirtualFunction());
    }

    void on_receive(const Message2& msg)
    {
      ++message2_count;
      etl::send_message(msg.callback, message5);
    }

    void on_receive(const Message3& msg)
    {
      ++message3_count;
      etl::send_message(msg.callback, message5);
    }

    void on_receive(const Message4& msg)
    {
      ++message4_count;
      etl::send_message(msg.callback, message5);
    }

    void on_receive(const Message5&)
    {
      ++callback_count;
    }

    void on_receive_unknown(const etl::imessage&)
    {
      ++message_unknown_count;
    }

    int message1_count;
    int message2_count;
    int message3_count;
    int message4_count;
    int message_unknown_count;
    int callback_count;
  };

  //***************************************************************************
  // Router that handles messages 1, 2, 4 and 5 and returns nothing.
  //***************************************************************************
  class Router2 : public etl::message_router<Router2, Message1, Message2, Message4, Message5>
  {
  public:

    Router2()
      : message_router(ROUTER2),
        message1_count(0),
        message2_count(0),
        message4_count(0),
        message_unknown_count(0),
        callback_count(0),
        sender_id(0)
    {

    }

    Router2(etl::imessage_router& successor_)
      : message_router(ROUTER2, successor_),
        message1_count(0),
        message2_count(0),
        message4_count(0),
        message_unknown_count(0),
        callback_count(0),
        sender_id(0)
    {

    }

    void on_receive(const Message1& msg)
    {
      ++message1_count;
      sender_id = msg.callback.get_message_router_id();
      etl::send_message(msg.callback, message5);
      //CHECK_EQUAL(1, msg.VirtualFunction());
    }

    void on_receive(const Message2& msg)
    {
      ++message2_count;
      sender_id = msg.callback.get_message_router_id();
      etl::send_message(msg.callback, message5);
    }

    void on_receive(const Message4& msg)
    {
      ++message4_count;
      sender_id = msg.callback.get_message_router_id();
      etl::send_message(msg.callback, message5);
    }

    void on_receive(const Message5&)
    {
      ++callback_count;
    }

    void on_receive_unknown(const etl::imessage&)
    {
      ++message_unknown_count;
      sender_id = 0;
    }

    int message1_count;
    int message2_count;
    int message4_count;
    int message_unknown_count;
    int callback_count;
    int sender_id;
  };

  //***************************************************************************
  // Router that handles messages 1, 2, 3.
  // 'receive' is overridden.
  //***************************************************************************
  class Router3 : public etl::message_router<Router3, Message1, Message2, Message3>
  {
  public:

    using base = etl::message_router<Router3, Message1, Message2, Message3>;

    Router3()
      : message_router(ROUTER3)
      , message1_received(false)
      , message2_received(false)
      , message3_received(false)
      , unknown_message_received(false)
    {
    }

    void receive(const etl::imessage& msg) override
    {
      switch (msg.get_message_id())
      {
        case MESSAGE1:
        {
          message1_received = true;
          break;
        }

        case MESSAGE2:
        {
          message2_received = true;
          break;
        }

        case MESSAGE3:
        {
          message3_received = true;
          break;
        }

        default:
        {
          unknown_message_received = true;
          break;
        }
      }
    }

    void on_receive(const Message1&)
    {
    }

    void on_receive(const Message2&)
    {
    }

    void on_receive(const Message3&)
    {
    }

    void on_receive_unknown(const etl::imessage&)
    {
    }

    bool message1_received;
    bool message2_received;
    bool message3_received;
    bool unknown_message_received;
  };

  etl::imessage_router* p_router;

  SUITE(test_message_router)
  {
    //*************************************************************************
    TEST(message_router)
    {
      Router1 r1;
      Router2 r2;

      p_router = &r1;

      Message1 message1(r2);
      Message2 message2(r2);
      Message3 message3(r2);
      Message4 message4(r2);

      // CHECK(!r1.is_null_router());
      CHECK(r1.is_producer());
      CHECK(r1.is_consumer());

      p_router->receive(message1);
      CHECK_EQUAL(1, r1.message1_count);
      CHECK_EQUAL(0, r1.message2_count);
      CHECK_EQUAL(0, r1.message3_count);
      CHECK_EQUAL(0, r1.message4_count);
      CHECK_EQUAL(0, r1.message_unknown_count);
      CHECK_EQUAL(1, r2.callback_count);

      p_router->receive(message2);
      CHECK_EQUAL(1, r1.message1_count);
      CHECK_EQUAL(1, r1.message2_count);
      CHECK_EQUAL(0, r1.message3_count);
      CHECK_EQUAL(0, r1.message4_count);
      CHECK_EQUAL(0, r1.message_unknown_count);
      CHECK_EQUAL(2, r2.callback_count);

      p_router->receive(message3);
      CHECK_EQUAL(1, r1.message1_count);
      CHECK_EQUAL(1, r1.message2_count);
      CHECK_EQUAL(1, r1.message3_count);
      CHECK_EQUAL(0, r1.message4_count);
      CHECK_EQUAL(0, r1.message_unknown_count);
      CHECK_EQUAL(3, r2.callback_count);

      p_router->receive(message4);
      CHECK_EQUAL(1, r1.message1_count);
      CHECK_EQUAL(1, r1.message2_count);
      CHECK_EQUAL(1, r1.message3_count);
      CHECK_EQUAL(1, r1.message4_count);
      CHECK_EQUAL(0, r1.message_unknown_count);
      CHECK_EQUAL(4, r2.callback_count);
    }

    //*************************************************************************
    TEST(message_null_router)
    {
      Router2 router;
      etl::null_message_router null_router;

      Message1 message1(null_router);
      Message2 message2(null_router);
      Message3 message3(null_router);
      Message4 message4(null_router);

      // CHECK(null_router.is_null_router());
      CHECK(!null_router.is_producer());
      CHECK(!null_router.is_consumer());

      // Send from the null router.
      etl::send_message(router, message1);
      CHECK_EQUAL(1, router.message1_count);
      CHECK_EQUAL(0, router.message2_count);
      CHECK_EQUAL(0, router.message4_count);
      CHECK_EQUAL(0, router.message_unknown_count);

      etl::send_message(router, message2);
      CHECK_EQUAL(1, router.message1_count);
      CHECK_EQUAL(1, router.message2_count);
      CHECK_EQUAL(0, router.message4_count);
      CHECK_EQUAL(0, router.message_unknown_count);

      etl::send_message(router, message3);
      CHECK_EQUAL(1, router.message1_count);
      CHECK_EQUAL(1, router.message2_count);
      CHECK_EQUAL(0, router.message4_count);
      CHECK_EQUAL(1, router.message_unknown_count);

      etl::send_message(router, message4);
      CHECK_EQUAL(1, router.message1_count);
      CHECK_EQUAL(1, router.message2_count);
      CHECK_EQUAL(1, router.message4_count);
      CHECK_EQUAL(1, router.message_unknown_count);

      // Send to the null router.
      etl::send_message(null_router, message1);
      CHECK_EQUAL(1, router.message1_count);
      CHECK_EQUAL(1, router.message2_count);
      CHECK_EQUAL(1, router.message4_count);
      CHECK_EQUAL(1, router.message_unknown_count);

      etl::send_message(null_router, message2);
      CHECK_EQUAL(1, router.message1_count);
      CHECK_EQUAL(1, router.message2_count);
      CHECK_EQUAL(1, router.message4_count);
      CHECK_EQUAL(1, router.message_unknown_count);

      etl::send_message(null_router, message3);
      CHECK_EQUAL(1, router.message1_count);
      CHECK_EQUAL(1, router.message2_count);
      CHECK_EQUAL(1, router.message4_count);
      CHECK_EQUAL(1, router.message_unknown_count);

      etl::send_message(null_router, message4);
      CHECK_EQUAL(1, router.message1_count);
      CHECK_EQUAL(1, router.message2_count);
      CHECK_EQUAL(1, router.message4_count);
      CHECK_EQUAL(1, router.message_unknown_count);
    }

    //*************************************************************************
    TEST(message_producer)
    {
      Router2 router;
      etl::message_producer producer(ROUTER3);

      Message1 message1(producer);
      Message2 message2(producer);
      Message3 message3(producer);
      Message4 message4(producer);

      // CHECK(!producer.is_null_router());
      CHECK(producer.is_producer());
      CHECK(!producer.is_consumer());

      CHECK_EQUAL(0, router.sender_id);

      // Send from the producer.
      router.receive(message1);
      //etl::send_message(router, message1);
      CHECK_EQUAL(1, router.message1_count);
      CHECK_EQUAL(0, router.message2_count);
      CHECK_EQUAL(0, router.message4_count);
      CHECK_EQUAL(0, router.message_unknown_count);
      CHECK_EQUAL(ROUTER3, router.sender_id);

      etl::send_message(router, message2);
      CHECK_EQUAL(1, router.message1_count);
      CHECK_EQUAL(1, router.message2_count);
      CHECK_EQUAL(0, router.message4_count);
      CHECK_EQUAL(0, router.message_unknown_count);
      CHECK_EQUAL(ROUTER3, router.sender_id);

      etl::send_message(router, message3);
      CHECK_EQUAL(1, router.message1_count);
      CHECK_EQUAL(1, router.message2_count);
      CHECK_EQUAL(0, router.message4_count);
      CHECK_EQUAL(1, router.message_unknown_count);
      CHECK_EQUAL(0, router.sender_id);

      etl::send_message(router, message4);
      CHECK_EQUAL(1, router.message1_count);
      CHECK_EQUAL(1, router.message2_count);
      CHECK_EQUAL(1, router.message4_count);
      CHECK_EQUAL(1, router.message_unknown_count);
      CHECK_EQUAL(ROUTER3, router.sender_id);

      // Send to the producer.
      etl::send_message(producer, message1);
      CHECK_EQUAL(1, router.message1_count);
      CHECK_EQUAL(1, router.message2_count);
      CHECK_EQUAL(1, router.message4_count);
      CHECK_EQUAL(1, router.message_unknown_count);

      etl::send_message(producer, message2);
      CHECK_EQUAL(1, router.message1_count);
      CHECK_EQUAL(1, router.message2_count);
      CHECK_EQUAL(1, router.message4_count);
      CHECK_EQUAL(1, router.message_unknown_count);

      etl::send_message(producer, message3);
      CHECK_EQUAL(1, router.message1_count);
      CHECK_EQUAL(1, router.message2_count);
      CHECK_EQUAL(1, router.message4_count);
      CHECK_EQUAL(1, router.message_unknown_count);

      etl::send_message(producer, message4);
      CHECK_EQUAL(1, router.message1_count);
      CHECK_EQUAL(1, router.message2_count);
      CHECK_EQUAL(1, router.message4_count);
      CHECK_EQUAL(1, router.message_unknown_count);
    }

    //*************************************************************************
    TEST(message_router_accepts)
    {
      Router2 r2;

      etl::null_message_router null_router;

      Message1 message1(null_router);
      Message2 message2(null_router);
      Message3 message3(null_router);
      Message4 message4(null_router);

      CHECK(r2.accepts(message1));
      CHECK(r2.accepts(message1.get_message_id()));

      CHECK(r2.accepts(message2));
      CHECK(r2.accepts(message2.get_message_id()));

      CHECK(!r2.accepts(message3));
      CHECK(!r2.accepts(message3.get_message_id()));

      CHECK(r2.accepts(message4));
      CHECK(r2.accepts(message4.get_message_id()));

      CHECK(r2.accepts(message5));
      CHECK(r2.accepts(message5.get_message_id()));
    }

    //*************************************************************************
    TEST(message_router_accepts_successors)
    {
      Router1 r1; // M1, M2, M3, M4, M5
      Router2 r2; // M1, M2, M4, M5

      r2.set_successor(r1);

      etl::null_message_router null_router;

      Message1 message1(null_router);
      Message2 message2(null_router);
      Message3 message3(null_router);
      Message4 message4(null_router);

      CHECK(r2.accepts(message1));
      CHECK(r2.accepts(message1.get_message_id()));

      CHECK(r2.accepts(message2));
      CHECK(r2.accepts(message2.get_message_id()));

      CHECK(r2.accepts(message3));
      CHECK(r2.accepts(message3.get_message_id()));

      CHECK(r2.accepts(message4));
      CHECK(r2.accepts(message4.get_message_id()));

      CHECK(r2.accepts(message5));
      CHECK(r2.accepts(message5.get_message_id()));
    }

#if ETL_HAS_VIRTUAL_MESSAGES
    //*************************************************************************
    TEST(message_router_queue)
    {
      Router1 r1;
      Router2 r2;

      typedef Router2::message_packet Packet;
      typedef etl::queue<Packet, 4> Queue;

      Queue queue;

      etl::imessage* im;

      Message1 message1(r1);
      Message2 message2(r1);
      Message3 message3(r1);
      Message4 message4(r1);

      // Queue some messages in the message packet queue.
      im = &message1;
      queue.emplace(*im);

      im = &message2;
      queue.emplace(*im);

      // The router2 queue doesn't accept Message3 types.
      im = &message3;
      CHECK_THROW(queue.emplace(*im), etl::unhandled_message_exception);

      im = &message4;
      queue.emplace(*im);

      im = &message4;
      queue.emplace(*im);

      etl::imessage& imr1 = queue.front().get();
      r2.receive(imr1);
      CHECK_EQUAL(1, r2.message1_count);
      CHECK_EQUAL(0, r2.message2_count);
      CHECK_EQUAL(0, r2.message4_count);
      CHECK_EQUAL(0, r2.message_unknown_count);
      CHECK_EQUAL(1, r1.callback_count);
      queue.pop();

      etl::imessage& imr2 = queue.front().get();
      r2.receive(imr2);
      CHECK_EQUAL(1, r2.message1_count);
      CHECK_EQUAL(1, r2.message2_count);
      CHECK_EQUAL(0, r2.message4_count);
      CHECK_EQUAL(0, r2.message_unknown_count);
      CHECK_EQUAL(2, r1.callback_count);
      queue.pop();

      const etl::imessage& imr3 = queue.front().get();
      r2.receive(imr3);
      CHECK_EQUAL(1, r2.message1_count);
      CHECK_EQUAL(1, r2.message2_count);
      CHECK_EQUAL(1, r2.message4_count);
      CHECK_EQUAL(0, r2.message_unknown_count);
      CHECK_EQUAL(3, r1.callback_count);
      queue.pop();

      const Queue& crqueue = queue;
      const etl::imessage& imr4 = crqueue.front().get();
      r2.receive(imr4);
      CHECK_EQUAL(1, r2.message1_count);
      CHECK_EQUAL(1, r2.message2_count);
      CHECK_EQUAL(2, r2.message4_count);
      CHECK_EQUAL(0, r2.message_unknown_count);
      CHECK_EQUAL(4, r1.callback_count);
      queue.pop();
    }
#endif

    //*************************************************************************
    TEST(message_router_successor)
    {
      Router1 r1;
      Router2 r2(r1);

      etl::null_message_router null_router;

      Message1 message1(r2);
      Message2 message2(r2);
      Message3 message3(r2);
      Message4 message4(r2);

      etl::send_message(r2, message1);
      CHECK_EQUAL(1, r2.message1_count);
      CHECK_EQUAL(0, r2.message2_count);
      CHECK_EQUAL(0, r2.message4_count);
      CHECK_EQUAL(0, r2.message_unknown_count);

      CHECK_EQUAL(0, r1.message1_count);
      CHECK_EQUAL(0, r1.message2_count);
      CHECK_EQUAL(0, r1.message3_count);
      CHECK_EQUAL(0, r1.message4_count);
      CHECK_EQUAL(0, r1.message_unknown_count);

      etl::send_message(r2, message2);
      CHECK_EQUAL(1, r2.message1_count);
      CHECK_EQUAL(1, r2.message2_count);
      CHECK_EQUAL(0, r2.message4_count);
      CHECK_EQUAL(0, r2.message_unknown_count);

      CHECK_EQUAL(0, r1.message1_count);
      CHECK_EQUAL(0, r1.message2_count);
      CHECK_EQUAL(0, r1.message3_count);
      CHECK_EQUAL(0, r1.message4_count);
      CHECK_EQUAL(0, r1.message_unknown_count);

      r2.receive(message3);
      CHECK_EQUAL(1, r2.message1_count);
      CHECK_EQUAL(1, r2.message2_count);
      CHECK_EQUAL(0, r2.message4_count);
      CHECK_EQUAL(0, r2.message_unknown_count);

      CHECK_EQUAL(0, r1.message1_count);
      CHECK_EQUAL(0, r1.message2_count);
      CHECK_EQUAL(1, r1.message3_count);
      CHECK_EQUAL(0, r1.message4_count);
      CHECK_EQUAL(0, r1.message_unknown_count);

      etl::send_message(r2, message4);
      CHECK_EQUAL(1, r2.message1_count);
      CHECK_EQUAL(1, r2.message2_count);
      CHECK_EQUAL(1, r2.message4_count);
      CHECK_EQUAL(0, r2.message_unknown_count);

      CHECK_EQUAL(0, r1.message1_count);
      CHECK_EQUAL(0, r1.message2_count);
      CHECK_EQUAL(1, r1.message3_count);
      CHECK_EQUAL(0, r1.message4_count);
      CHECK_EQUAL(0, r1.message_unknown_count);
    }

    //*************************************************************************
    TEST(message_router_with_overloaded_receive)
    {
      Router3 router;

      Message1 message1(router);
      Message2 message2(router);
      Message3 message3(router);

      router.receive(message1);
      CHECK_TRUE(router.message1_received);

      router.receive(message2);
      CHECK_TRUE(router.message2_received);

      router.receive(message3);
      CHECK_TRUE(router.message3_received);

      CHECK_FALSE(router.unknown_message_received);
    }
  };
}
