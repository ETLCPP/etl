/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl
https://www.etlcpp.com

Copyright(c) 2020 John Wellbelove

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

#include "etl/platform.h"

#include "etl/message_packet.h"

#include <string>

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
    MESSAGE4
  };

  struct Message1 : public etl::message<MESSAGE1>
  {
    Message1(int x_)
      : etl::message<MESSAGE1>()
      , x(x_)
      , moved(false)
      , copied(false)
    {
    }

    Message1(const Message1& other)
      : etl::message<MESSAGE1>()
      , x(other.x)
      , moved(false)
      , copied(true)
    {
    }

    Message1(Message1&& other)
      : etl::message<MESSAGE1>()
      , x(std::move(other.x))
      , moved(true)
      , copied(false)
    {
    }

    Message1& operator =(const Message1& other)
    {
      x = other.x;
      moved = false;
      copied = true;
      return *this;
    }

    Message1& operator =(Message1&& other)
    {
      x = std::move(other.x);
      moved = true;
      copied = false;
      return *this;
    }

    int x;
    bool moved;
    bool copied;
  };

  struct Message2 : public etl::message<MESSAGE2>
  {
    Message2(double x_)
      : etl::message<MESSAGE2>()
      , x(x_)
      , moved(false)
      , copied(false)
    {
    }

    Message2(const Message2& other)
      : etl::message<MESSAGE2>()
      , x(other.x)
      , moved(false)
      , copied(true)
    {
    }

    Message2(Message2&& other)
      : etl::message<MESSAGE2>()
      , x(std::move(other.x))
      , moved(true)
      , copied(false)
    {
    }

    Message2& operator =(const Message2& other)
    {
      x = other.x;
      moved = false;
      copied = true;
      return *this;
    }

    Message2& operator =(Message2&& other)
    {
      x = std::move(other.x);
      moved = true;
      copied = false;
      return *this;
    }

    double x;
    bool moved;
    bool copied;
  };

  struct Message3 : public etl::message<MESSAGE3>
  {
    Message3(const std::string& x_)
      : etl::message<MESSAGE3>()
      , x(x_)
      , moved(false)
      , copied(false)
    {
    }

    Message3(std::string&& x_)
      : etl::message<MESSAGE3>()
      , x(std::move(x_))
      , moved(true)
      , copied(false)
    {
    }

    Message3(const Message3& other)
      : etl::message<MESSAGE3>()
      , x(other.x)
      , moved(false)
      , copied(true)
    {
    }

    Message3(Message3&& other)
      : etl::message<MESSAGE3>()
      , x(std::move(other.x))
      , moved(true)
      , copied(false)
    {
    }

    Message3& operator =(const Message3& other)
    {
      x = other.x;
      moved = false;
      copied = true;
      return *this;
    }

    Message3& operator =(Message3&& other)
    {
      x = std::move(other.x);
      moved = true;
      copied = false;
      return *this;
    }

    std::string x;
    bool moved;
    bool copied;
  };

  struct Message4 : public etl::message<MESSAGE4>
  {
  };

  using Packet = etl::message_packet<Message1, Message2, Message3>;

  struct Object
  {
    void Push(const etl::message_packet<Message1, Message2>& p)
    {
      ::new (buffer) etl::message_packet<Message1, Message2>(p);
    }

    char buffer[100];
  };

  SUITE(test_message_packet)
  {
    //*************************************************************************
    TEST(message_packet_construction)
    {
      Message1 message1(1);
      Message2 message2(2.2);
      const Message3 message3("3");
      Message4 message4;

      Packet packet1(message1);
      Packet packet2(message2);
      Packet packet3(message3);

      // Should cause a static assert.
      //Packet packet4(message4);
      //Packet packet4((Message4()));

      CHECK_EQUAL(MESSAGE1, packet1.get().get_message_id());
      CHECK_EQUAL(MESSAGE2, packet2.get().get_message_id());
      CHECK_EQUAL(MESSAGE3, packet3.get().get_message_id());

      CHECK(!static_cast<Message1&>(packet1.get()).moved);
      CHECK(!static_cast<Message2&>(packet2.get()).moved);
      CHECK(!static_cast<Message3&>(packet3.get()).moved);

      CHECK(static_cast<Message1&>(packet1.get()).copied);
      CHECK(static_cast<Message2&>(packet2.get()).copied);
      CHECK(static_cast<Message3&>(packet3.get()).copied);

      CHECK_EQUAL(1,   static_cast<Message1&>(packet1.get()).x);
      CHECK_EQUAL(2.2, static_cast<Message2&>(packet2.get()).x);
      CHECK_EQUAL("3", static_cast<Message3&>(packet3.get()).x);
    }

#if !defined(ETL_MESSAGE_PACKET_FORCE_CPP03_IMPLEMENTATION)
    //*************************************************************************
    TEST(message_packet_move_construction)
    {
      Message1 message1(1);
      Message2 message2(2.2);

      Packet packet1(message1);
      Packet packet2(std::move(message2));

      CHECK_EQUAL(MESSAGE1, packet1.get().get_message_id());
      CHECK_EQUAL(MESSAGE2, packet2.get().get_message_id());

      CHECK(!static_cast<Message1&>(packet1.get()).moved);
      CHECK(static_cast<Message2&>(packet2.get()).moved);

      CHECK(static_cast<Message1&>(packet1.get()).copied);
      CHECK(!static_cast<Message2&>(packet2.get()).copied);

      CHECK_EQUAL(1, static_cast<Message1&>(packet1.get()).x);
      CHECK_EQUAL(2.2, static_cast<Message2&>(packet2.get()).x);
    }
#endif

    //*************************************************************************
    TEST(message_constant_packet_construction)
    {
      Message1 message1(1);
      Message2 message2(2.2);
      Message3 message3("3");
      Message4 message4;

      const Packet packet1(message1);
      const Packet packet2(message2);
      const Packet packet3(message3);

      CHECK_EQUAL(MESSAGE1, packet1.get().get_message_id());
      CHECK_EQUAL(MESSAGE2, packet2.get().get_message_id());
      CHECK_EQUAL(MESSAGE3, packet3.get().get_message_id());

      CHECK_EQUAL(1,   static_cast<const Message1&>(packet1.get()).x);
      CHECK_EQUAL(2.2, static_cast<const Message2&>(packet2.get()).x);
      CHECK_EQUAL("3", static_cast<const Message3&>(packet3.get()).x);
    }

    //*************************************************************************
    TEST(message_packet_construction_from_base)
    {
      Message1 message1(1);
      Message2 message2(2.2);
      Message3 message3("3");
      Message4 message4;

      Packet packet1(static_cast<etl::imessage&>(message1));
      Packet packet2(static_cast<etl::imessage&>(message2));
      Packet packet3(static_cast<etl::imessage&>(message3));

      CHECK_THROW(Packet packet4(static_cast<etl::imessage&>(message4)), etl::unhandled_message_exception);

      CHECK_EQUAL(MESSAGE1, packet1.get().get_message_id());
      CHECK_EQUAL(MESSAGE2, packet2.get().get_message_id());
      CHECK_EQUAL(MESSAGE3, packet3.get().get_message_id());

      CHECK_EQUAL(1,   static_cast<Message1&>(packet1.get()).x);
      CHECK_EQUAL(2.2, static_cast<Message2&>(packet2.get()).x);
      CHECK_EQUAL("3", static_cast<Message3&>(packet3.get()).x);
    }

    //*************************************************************************
    TEST(message_packet_move_construction_from_base)
    {
      Message1 message1(1);
      Message2 message2(2.2);
      Message3 message3("3");
      Message4 message4;

      Packet packet1(static_cast<etl::imessage&&>(message1));
      Packet packet2(static_cast<etl::imessage&&>(message2));
      Packet packet3(static_cast<etl::imessage&&>(message3));

      CHECK_THROW(Packet packet4(static_cast<etl::imessage&>(message4)), etl::unhandled_message_exception);

      CHECK_EQUAL(MESSAGE1, packet1.get().get_message_id());
      CHECK_EQUAL(MESSAGE2, packet2.get().get_message_id());
      CHECK_EQUAL(MESSAGE3, packet3.get().get_message_id());

      CHECK_EQUAL(1, static_cast<Message1&>(packet1.get()).x);
      CHECK_EQUAL(2.2, static_cast<Message2&>(packet2.get()).x);
      CHECK_EQUAL("3", static_cast<Message3&>(packet3.get()).x);
    }

    //*************************************************************************
    TEST(message_packet_copy_constructor)
    {
      Message1 message1(1);
      Message2 message2(2.2);

      Packet packet1(message1);
      Packet packet2(packet1);

      CHECK_EQUAL(MESSAGE1, packet1.get().get_message_id());
      CHECK_EQUAL(MESSAGE1, packet2.get().get_message_id());

      CHECK(!static_cast<Message1&>(packet1.get()).moved);
      CHECK(!static_cast<Message1&>(packet2.get()).moved);

      CHECK(static_cast<Message1&>(packet1.get()).copied);
      CHECK(static_cast<Message1&>(packet2.get()).copied);

      CHECK_EQUAL(1, static_cast<Message1&>(packet1.get()).x);
      CHECK_EQUAL(1, static_cast<Message1&>(packet2.get()).x);
    }

#if !defined(ETL_MESSAGE_PACKET_FORCE_CPP03_IMPLEMENTATION)
    //*************************************************************************
    TEST(message_packet_move_constructor)
    {
      Message1 message1(1);
      Message2 message2(2.2);

      Packet packet1(message1);

#include "etl/private/diagnostic_pessimizing_move_push.h"
      Packet packet2(std::move(Packet(message1)));
#include "etl/private/diagnostic_pop.h"

      CHECK_EQUAL(MESSAGE1, packet1.get().get_message_id());
      CHECK_EQUAL(MESSAGE1, packet2.get().get_message_id());

      CHECK(!static_cast<Message1&>(packet1.get()).moved);
      CHECK(static_cast<Message1&>(packet2.get()).moved);

      CHECK(static_cast<Message1&>(packet1.get()).copied);
      CHECK(!static_cast<Message1&>(packet2.get()).copied);

      CHECK_EQUAL(1, static_cast<Message1&>(packet1.get()).x);
      CHECK_EQUAL(1, static_cast<Message1&>(packet2.get()).x);
    }
#endif

    //*************************************************************************
    TEST(message_packet_assignment)
    {
      Message1 message1(1);
      Message2 message2(2.2);

      Packet packet1(message1);
      Packet packet2;

      packet2 = packet1;

      CHECK_EQUAL(MESSAGE1, packet1.get().get_message_id());
      CHECK_EQUAL(MESSAGE1, packet2.get().get_message_id());

      CHECK(!static_cast<Message1&>(packet1.get()).moved);
      CHECK(!static_cast<Message1&>(packet2.get()).moved);

      CHECK(static_cast<Message1&>(packet1.get()).copied);
      CHECK(static_cast<Message1&>(packet2.get()).copied);

      CHECK_EQUAL(1, static_cast<Message1&>(packet1.get()).x);
      CHECK_EQUAL(1, static_cast<Message1&>(packet2.get()).x);
    }

#if !defined(ETL_MESSAGE_PACKET_FORCE_CPP03_IMPLEMENTATION)
    //*************************************************************************
    TEST(message_packet_move_assignment)
    {
      Message1 message1(1);
      Message2 message2(2.2);

      Packet packet1(message1);
      Packet packet2;

      packet2 = std::move(packet1);

      CHECK_EQUAL(MESSAGE1, packet1.get().get_message_id());
      CHECK_EQUAL(MESSAGE1, packet2.get().get_message_id());

      CHECK(!static_cast<Message1&>(packet1.get()).moved);
      CHECK(static_cast<Message1&>(packet2.get()).moved);

      CHECK(static_cast<Message1&>(packet1.get()).copied);
      CHECK(!static_cast<Message1&>(packet2.get()).copied);

      CHECK_EQUAL(1, static_cast<Message1&>(packet1.get()).x);
      CHECK_EQUAL(1, static_cast<Message1&>(packet2.get()).x);
    }
#endif

    //*************************************************************************
    TEST(message_packet_accepts)
    {
      Message1 message1(1);
      Message2 message2(2.2);
      Message3 message3("3");
      Message4 message4;

      Packet packet;

      // From message id.
      CHECK(Packet::accepts(message1.get_message_id()));
      CHECK(Packet::accepts(message2.get_message_id()));
      CHECK(Packet::accepts(message3.get_message_id()));
      CHECK(!Packet::accepts(message4.get_message_id()));

      // From message.
      CHECK(Packet::accepts(message1));
      CHECK(Packet::accepts(message2));
      CHECK(Packet::accepts(message3));
      CHECK(!Packet::accepts(message4));

      // From message type.
      CHECK(Packet::accepts<Message1>());
      CHECK(Packet::accepts<Message2>());
      CHECK(Packet::accepts<Message3>());
      CHECK(!Packet::accepts<Message4>());
       
      // From static message id.
      CHECK(Packet::accepts<MESSAGE1>());
      CHECK(Packet::accepts<MESSAGE2>());
      CHECK(Packet::accepts<MESSAGE3>());
      CHECK(!Packet::accepts<MESSAGE4>());
    }

    //*************************************************************************
    TEST(test_message_packet_push_to_queue_bug_845)
    {
      using Packet = etl::message_packet<Message1, Message2>;

      Object obj;

      Message1 message1(1);
      Message2 message2(1.2);
      Packet packet1(message1);
      Packet packet2(message2);

      obj.Push(packet1);
      obj.Push(packet2);
    }
  };
}
