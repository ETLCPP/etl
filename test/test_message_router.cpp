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

  struct Message1 : public etl::message<MESSAGE1>
  {

  };

  struct Message2 : public etl::message<MESSAGE2>
  {

  };

  struct Message3 : public etl::message<MESSAGE3>
  {

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

  // Router typedefs.
  typedef etl::imessage_router<int>  int_router_t;
  typedef etl::imessage_router<void> void_router_t;

  //***************************************************************************
  // Router that handles messages 1, 2, 3, 4 and 5 and returns nothing.
  //***************************************************************************
  class RouterVoid1 : public etl::message_router<RouterVoid1, void, Message1, Message2, Message3, Message4, Message5>
  {
  public:

    RouterVoid1()
      : message1_count(0),
        message2_count(0),
        message3_count(0),
        message4_count(0),
        message_unknown_count(0),
        callback_count(0)
    {

    }

    void on_receive(void_router_t& sender, const Message1& msg)
    {
      ++message1_count;
      send_to(sender, message5);
    }

    void on_receive(void_router_t& sender, const Message2& msg)
    {
      ++message2_count;
      send_to(sender, message5);
    }

    void on_receive(void_router_t& sender, const Message3& msg)
    {
      ++message3_count;
      send_to(sender, message5);
    }

    void on_receive(void_router_t& sender, const Message4& msg)
    {
      ++message4_count;
      send_to(sender, message5);
    }

    void on_receive(void_router_t& sender, const Message5& msg)
    {
      ++callback_count;
    }

    void on_receive_unknown(void_router_t& sender, const etl::imessage& msg)
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
  class RouterVoid2 : public etl::message_router<RouterVoid2, void, Message1, Message2, Message4, Message5>
  {
  public:

    RouterVoid2()
      : message1_count(0),
        message2_count(0),
        message4_count(0),
        message_unknown_count(0),
        callback_count(0)
    {

    }

    void on_receive(void_router_t& sender, const Message1& msg)
    {
      ++message1_count;
      send_to(sender, message5);
    }

    void on_receive(void_router_t& sender, const Message2& msg)
    {
      ++message2_count;
      send_to(sender, message5);
    }

    void on_receive(void_router_t& sender, const Message4& msg)
    {
      ++message4_count;
      send_to(sender, message5);
    }

    void on_receive(void_router_t& sender, const Message5& msg)
    {
      ++callback_count;
    }

    void on_receive_unknown(void_router_t& sender, const etl::imessage& msg)
    {
      ++message_unknown_count;
      send_to(sender, message5);
    }

    int message1_count;
    int message2_count;
    int message4_count;
    int message_unknown_count;
    int callback_count;
  };

  //***************************************************************************
  // Router that handles messages 1, 2, 3, 4 and 5 and returns an int.
  //***************************************************************************
  class RouterInt1 : public etl::message_router<RouterInt1, int, Message1, Message2, Message3, Message4, Message5>
  {
  public:

    RouterInt1()
      : message1_count(0),
        message2_count(0),
        message3_count(0),
        message4_count(0),
        message_unknown_count(0),
        callback_count(0)
    {

    }

    int on_receive(int_router_t& sender, const Message1& msg)
    {
      ++message1_count;
      send_to(sender, message5);
      return msg.ID;
    }

    int on_receive(int_router_t& sender, const Message2& msg)
    {
      ++message2_count;
      send_to(sender, message5);
      return msg.ID;
    }

    int on_receive(int_router_t& sender, const Message3& msg)
    {
      ++message3_count;
      send_to(sender, message5);
      return msg.ID;
    }

    int on_receive(int_router_t& sender, const Message4& msg)
    {
      ++message4_count;
      send_to(sender, message5);
      return msg.ID;
    }

    int on_receive(int_router_t& sender, const Message5& msg)
    {
      ++callback_count;
      return msg.ID;
    }

    int on_receive_unknown(int_router_t& sender, const etl::imessage& msg)
    {
      ++message_unknown_count;
      return msg.get_message_id();
    }

    int message1_count;
    int message2_count;
    int message3_count;
    int message4_count;
    int message_unknown_count;
    int callback_count;
  };

  //***************************************************************************
  // Router that handles messages 1, 2, 4 and 5 and returns an int.
  //***************************************************************************
  class RouterInt2 : public etl::message_router<RouterInt2, int, Message1, Message2, Message4, Message5>
  {
  public:

    RouterInt2()
      : message1_count(0),
        message2_count(0),
        message4_count(0),
        message_unknown_count(0),
        callback_count(0)
    {

    }

    int on_receive(int_router_t& sender, const Message1& msg)
    {
      ++message1_count;
      send_to(sender, message5);
      return msg.ID;
    }

    int on_receive(int_router_t& sender, const Message2& msg)
    {
      ++message2_count;
      send_to(sender, message5);
      return msg.ID;
    }

    int on_receive(int_router_t& sender, const Message4& msg)
    {
      ++message4_count;
      send_to(sender, message5);
      return msg.ID;
    }

    int on_receive(int_router_t& sender, const Message5& msg)
    {
      ++callback_count;
      return msg.ID;
    }

    int on_receive_unknown(int_router_t& sender, const etl::imessage& msg)
    {
      ++message_unknown_count;
      send_to(sender, message5);
      return msg.get_message_id();
    }

    int message1_count;
    int message2_count;
    int message4_count;
    int message_unknown_count;
    int callback_count;
  };

  // Null router types.
  typedef etl::null_message_router<int>  int_null_router_t;
  typedef etl::null_message_router<void> void_null_router_t;

  void_router_t* p_void_router;
  int_router_t*  p_int_router;

  SUITE(test_message_router)
  {
    //=========================================================================
    TEST(message_router_void)
    {
      RouterVoid1 r1;
      RouterVoid2 r2;

      p_void_router = &r1;

      p_void_router->receive(r2, message1);
      CHECK_EQUAL(1, r1.message1_count);
      CHECK_EQUAL(0, r1.message2_count);
      CHECK_EQUAL(0, r1.message3_count);
      CHECK_EQUAL(0, r1.message4_count);
      CHECK_EQUAL(0, r1.message_unknown_count);
      CHECK_EQUAL(1, r2.callback_count);

      p_void_router->receive(r2, message2);
      CHECK_EQUAL(1, r1.message1_count);
      CHECK_EQUAL(1, r1.message2_count);
      CHECK_EQUAL(0, r1.message3_count);
      CHECK_EQUAL(0, r1.message4_count);
      CHECK_EQUAL(0, r1.message_unknown_count);
      CHECK_EQUAL(2, r2.callback_count);

      p_void_router->receive(r2, message3);
      CHECK_EQUAL(1, r1.message1_count);
      CHECK_EQUAL(1, r1.message2_count);
      CHECK_EQUAL(1, r1.message3_count);
      CHECK_EQUAL(0, r1.message4_count);
      CHECK_EQUAL(0, r1.message_unknown_count);
      CHECK_EQUAL(3, r2.callback_count);

      p_void_router->receive(r2, message4);
      CHECK_EQUAL(1, r1.message1_count);
      CHECK_EQUAL(1, r1.message2_count);
      CHECK_EQUAL(1, r1.message3_count);
      CHECK_EQUAL(1, r1.message4_count);
      CHECK_EQUAL(0, r1.message_unknown_count);
      CHECK_EQUAL(4, r2.callback_count);

      p_void_router = &r2;

      p_void_router->receive(r1, message1);
      CHECK_EQUAL(1, r2.message1_count);
      CHECK_EQUAL(0, r2.message2_count);
      CHECK_EQUAL(0, r2.message4_count);
      CHECK_EQUAL(0, r2.message_unknown_count);
      CHECK_EQUAL(1, r1.callback_count);

      p_void_router->receive(r1, message2);
      CHECK_EQUAL(1, r2.message1_count);
      CHECK_EQUAL(1, r2.message2_count);
      CHECK_EQUAL(0, r2.message4_count);
      CHECK_EQUAL(0, r2.message_unknown_count);
      CHECK_EQUAL(2, r1.callback_count);

      p_void_router->receive(r1, message3);
      CHECK_EQUAL(1, r2.message1_count);
      CHECK_EQUAL(1, r2.message2_count);
      CHECK_EQUAL(0, r2.message4_count);
      CHECK_EQUAL(1, r2.message_unknown_count);
      CHECK_EQUAL(3, r1.callback_count);

      p_void_router->receive(r1, message4);
      CHECK_EQUAL(1, r2.message1_count);
      CHECK_EQUAL(1, r2.message2_count);
      CHECK_EQUAL(1, r2.message4_count);
      CHECK_EQUAL(1, r2.message_unknown_count);
      CHECK_EQUAL(4, r1.callback_count);
    }

    //=========================================================================
    TEST(message_router_int)
    {
      RouterInt1 r1;
      RouterInt2 r2;

      p_int_router = &r1;

      CHECK_EQUAL(int(message1.ID), (p_int_router->receive(r2, message1)));
      CHECK_EQUAL(1, r1.message1_count);
      CHECK_EQUAL(0, r1.message2_count);
      CHECK_EQUAL(0, r1.message3_count);
      CHECK_EQUAL(0, r1.message4_count);
      CHECK_EQUAL(0, r1.message_unknown_count);
      CHECK_EQUAL(1, r2.callback_count);

      CHECK_EQUAL(int(message2.ID), (p_int_router->receive(r2, message2)));
      CHECK_EQUAL(1, r1.message1_count);
      CHECK_EQUAL(1, r1.message2_count);
      CHECK_EQUAL(0, r1.message3_count);
      CHECK_EQUAL(0, r1.message4_count);
      CHECK_EQUAL(0, r1.message_unknown_count);
      CHECK_EQUAL(2, r2.callback_count);

      CHECK_EQUAL(int(message3.ID), (p_int_router->receive(r2, message3)));
      CHECK_EQUAL(1, r1.message1_count);
      CHECK_EQUAL(1, r1.message2_count);
      CHECK_EQUAL(1, r1.message3_count);
      CHECK_EQUAL(0, r1.message4_count);
      CHECK_EQUAL(0, r1.message_unknown_count);
      CHECK_EQUAL(3, r2.callback_count);

      CHECK_EQUAL(int(message4.ID), (p_int_router->receive(r2, message4)));
      CHECK_EQUAL(1, r1.message1_count);
      CHECK_EQUAL(1, r1.message2_count);
      CHECK_EQUAL(1, r1.message3_count);
      CHECK_EQUAL(1, r1.message4_count);
      CHECK_EQUAL(0, r1.message_unknown_count);
      CHECK_EQUAL(4, r2.callback_count);

      p_int_router = &r2;

      CHECK_EQUAL(int(message1.ID), (p_int_router->receive(r1, message1)));
      CHECK_EQUAL(1, r2.message1_count);
      CHECK_EQUAL(0, r2.message2_count);
      CHECK_EQUAL(0, r2.message4_count);
      CHECK_EQUAL(0, r2.message_unknown_count);
      CHECK_EQUAL(1, r1.callback_count);

      CHECK_EQUAL(int(message2.ID), (p_int_router->receive(r1, message2)));
      CHECK_EQUAL(1, r2.message1_count);
      CHECK_EQUAL(1, r2.message2_count);
      CHECK_EQUAL(0, r2.message4_count);
      CHECK_EQUAL(0, r2.message_unknown_count);
      CHECK_EQUAL(2, r1.callback_count);

      CHECK_EQUAL(int(message3.ID), (p_int_router->receive(r1, message3)));
      CHECK_EQUAL(1, r2.message1_count);
      CHECK_EQUAL(1, r2.message2_count);
      CHECK_EQUAL(0, r2.message4_count);
      CHECK_EQUAL(1, r2.message_unknown_count);
      CHECK_EQUAL(3, r1.callback_count);

      CHECK_EQUAL(int(message4.ID), (p_int_router->receive(r1, message4)));
      CHECK_EQUAL(1, r2.message1_count);
      CHECK_EQUAL(1, r2.message2_count);
      CHECK_EQUAL(1, r2.message4_count);
      CHECK_EQUAL(1, r2.message_unknown_count);
      CHECK_EQUAL(4, r1.callback_count);
    }

    //=========================================================================
    TEST(message_null_router_void)
    {
      RouterVoid2 router;
      void_null_router_t null_router;

      // Send from the null router.
      null_router.send_to(router, message1);
      CHECK_EQUAL(1, router.message1_count);
      CHECK_EQUAL(0, router.message2_count);
      CHECK_EQUAL(0, router.message4_count);
      CHECK_EQUAL(0, router.message_unknown_count);

      null_router.send_to(router, message2);
      CHECK_EQUAL(1, router.message1_count);
      CHECK_EQUAL(1, router.message2_count);
      CHECK_EQUAL(0, router.message4_count);
      CHECK_EQUAL(0, router.message_unknown_count);

      null_router.send_to(router, message3);
      CHECK_EQUAL(1, router.message1_count);
      CHECK_EQUAL(1, router.message2_count);
      CHECK_EQUAL(0, router.message4_count);
      CHECK_EQUAL(1, router.message_unknown_count);

      null_router.send_to(router, message4);
      CHECK_EQUAL(1, router.message1_count);
      CHECK_EQUAL(1, router.message2_count);
      CHECK_EQUAL(1, router.message4_count);
      CHECK_EQUAL(1, router.message_unknown_count);

      // Send to the null router.
      router.send_to(null_router, message1);
      CHECK_EQUAL(1, router.message1_count);
      CHECK_EQUAL(1, router.message2_count);
      CHECK_EQUAL(1, router.message4_count);
      CHECK_EQUAL(1, router.message_unknown_count);

      router.send_to(null_router, message2);
      CHECK_EQUAL(1, router.message1_count);
      CHECK_EQUAL(1, router.message2_count);
      CHECK_EQUAL(1, router.message4_count);
      CHECK_EQUAL(1, router.message_unknown_count);

      router.send_to(null_router, message3);
      CHECK_EQUAL(1, router.message1_count);
      CHECK_EQUAL(1, router.message2_count);
      CHECK_EQUAL(1, router.message4_count);
      CHECK_EQUAL(1, router.message_unknown_count);

      router.send_to(null_router, message4);
      CHECK_EQUAL(1, router.message1_count);
      CHECK_EQUAL(1, router.message2_count);
      CHECK_EQUAL(1, router.message4_count);
      CHECK_EQUAL(1, router.message_unknown_count);
    }
  };
}
