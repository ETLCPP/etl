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
    ROUTER1,
    ROUTER2
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
      ++callback_count;
    }

    void on_receive_unknown(etl::imessage_router&, const etl::imessage&)
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
        callback_count(0)
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
      ++callback_count;
    }

    void on_receive_unknown(etl::imessage_router& sender, const etl::imessage&)
    {
      ++message_unknown_count;
      etl::send_message(sender, message5);
    }

    int message1_count;
    int message2_count;
    int message4_count;
    int message_unknown_count;
    int callback_count;
  };


  etl::imessage_router* p_router;

  SUITE(test_message_router)
  {
    //=========================================================================
    TEST(message_router)
    {
      Router1 r1;
      Router2 r2;

      p_router = &r1;

      p_router->receive(r2, message1);
      CHECK_EQUAL(1, r1.message1_count);
      CHECK_EQUAL(0, r1.message2_count);
      CHECK_EQUAL(0, r1.message3_count);
      CHECK_EQUAL(0, r1.message4_count);
      CHECK_EQUAL(0, r1.message_unknown_count);
      CHECK_EQUAL(1, r2.callback_count);

      p_router->receive(r2, message2);
      CHECK_EQUAL(1, r1.message1_count);
      CHECK_EQUAL(1, r1.message2_count);
      CHECK_EQUAL(0, r1.message3_count);
      CHECK_EQUAL(0, r1.message4_count);
      CHECK_EQUAL(0, r1.message_unknown_count);
      CHECK_EQUAL(2, r2.callback_count);

      p_router->receive(r2, message3);
      CHECK_EQUAL(1, r1.message1_count);
      CHECK_EQUAL(1, r1.message2_count);
      CHECK_EQUAL(1, r1.message3_count);
      CHECK_EQUAL(0, r1.message4_count);
      CHECK_EQUAL(0, r1.message_unknown_count);
      CHECK_EQUAL(3, r2.callback_count);

      p_router->receive(r2, message4);
      CHECK_EQUAL(1, r1.message1_count);
      CHECK_EQUAL(1, r1.message2_count);
      CHECK_EQUAL(1, r1.message3_count);
      CHECK_EQUAL(1, r1.message4_count);
      CHECK_EQUAL(0, r1.message_unknown_count);
      CHECK_EQUAL(4, r2.callback_count);

      p_router = &r2;

      p_router->receive(r1, message1);
      CHECK_EQUAL(1, r2.message1_count);
      CHECK_EQUAL(0, r2.message2_count);
      CHECK_EQUAL(0, r2.message4_count);
      CHECK_EQUAL(0, r2.message_unknown_count);
      CHECK_EQUAL(1, r1.callback_count);

      p_router->receive(r1, message2);
      CHECK_EQUAL(1, r2.message1_count);
      CHECK_EQUAL(1, r2.message2_count);
      CHECK_EQUAL(0, r2.message4_count);
      CHECK_EQUAL(0, r2.message_unknown_count);
      CHECK_EQUAL(2, r1.callback_count);

      p_router->receive(r1, message3);
      CHECK_EQUAL(1, r2.message1_count);
      CHECK_EQUAL(1, r2.message2_count);
      CHECK_EQUAL(0, r2.message4_count);
      CHECK_EQUAL(1, r2.message_unknown_count);
      CHECK_EQUAL(3, r1.callback_count);

      p_router->receive(r1, message4);
      CHECK_EQUAL(1, r2.message1_count);
      CHECK_EQUAL(1, r2.message2_count);
      CHECK_EQUAL(1, r2.message4_count);
      CHECK_EQUAL(1, r2.message_unknown_count);
      CHECK_EQUAL(4, r1.callback_count);
    }

    //=========================================================================
    TEST(message_null_router)
    {
      Router2 router;
      etl::null_message_router null_router;

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

    //=========================================================================
    TEST(message_router_accepts)
    {
      Router2 r2;

      CHECK(r2.accepts(message1));
      CHECK(r2.accepts(message1.message_id));

      CHECK(r2.accepts(message2));
      CHECK(r2.accepts(message2.message_id));

      CHECK(!r2.accepts(message3));
      CHECK(!r2.accepts(message3.message_id));

      CHECK(r2.accepts(message4));
      CHECK(r2.accepts(message4.message_id));

      CHECK(r2.accepts(message5));
      CHECK(r2.accepts(message5.message_id));
    }

    //=========================================================================
    TEST(message_router_queue)
    {
      Router1 r1;
      Router2 r2;

      typedef Router2::message_packet Packet;
      typedef etl::queue<Packet, 4> Queue;

      Queue queue;

      etl::imessage* im;

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
      r2.receive(r1, imr1);
      CHECK_EQUAL(1, r2.message1_count);
      CHECK_EQUAL(0, r2.message2_count);
      CHECK_EQUAL(0, r2.message4_count);
      CHECK_EQUAL(0, r2.message_unknown_count);
      CHECK_EQUAL(1, r1.callback_count);
      queue.pop();

      etl::imessage& imr2 = queue.front().get();
      r2.receive(r1, imr2);
      CHECK_EQUAL(1, r2.message1_count);
      CHECK_EQUAL(1, r2.message2_count);
      CHECK_EQUAL(0, r2.message4_count);
      CHECK_EQUAL(0, r2.message_unknown_count);
      CHECK_EQUAL(2, r1.callback_count);
      queue.pop();

      const etl::imessage& imr3 = queue.front().get();
      r2.receive(r1, imr3);
      CHECK_EQUAL(1, r2.message1_count);
      CHECK_EQUAL(1, r2.message2_count);
      CHECK_EQUAL(1, r2.message4_count);
      CHECK_EQUAL(0, r2.message_unknown_count);
      CHECK_EQUAL(3, r1.callback_count);
      queue.pop();

      const Queue& crqueue = queue;
      const etl::imessage& imr4 = crqueue.front().get();
      r2.receive(r1, imr4);
      CHECK_EQUAL(1, r2.message1_count);
      CHECK_EQUAL(1, r2.message2_count);
      CHECK_EQUAL(2, r2.message4_count);
      CHECK_EQUAL(0, r2.message_unknown_count);
      CHECK_EQUAL(4, r1.callback_count);
      queue.pop();
    }

    //=========================================================================
    TEST(message_router_successor)
    {
      Router1 r1;
      Router2 r2;

      r2.set_successor(r1);

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

      etl::send_message(r2, message3);
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
  };
}
