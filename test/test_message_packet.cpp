/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl
http://www.etlcpp.com

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

#include "UnitTest++/UnitTest++.h"
#include "ExtraCheckMacros.h"

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
      : x(x_)
    {
    }

    int x;
  };

  struct Message2 : public etl::message<MESSAGE2>
  {
    Message2(double x_)
      : x(x_)
    {
    }

    double x;
  };

  struct Message3 : public etl::message<MESSAGE3>
  {
    Message3(std::string x_)
      : x(x_)
    {
    }

    std::string x;
  };

  struct Message4 : public etl::message<MESSAGE4>
  {
  };

  using Packet = etl::message_packet<Message1, Message2, Message3>;

  SUITE(test_message_packet)
  {
    //*************************************************************************
    TEST(message_packet_construction)
    {
      Message1 message1(1);
      Message2 message2(2.2);
      Message3 message3("3");
      Message4 message4;

      Packet packet1(message1);
      Packet packet2(message2);
      Packet packet3(message3);

      // The next line should result in a compile error.
      //Packet packet4(message4);

      CHECK_EQUAL(MESSAGE1, packet1.get().message_id);
      CHECK_EQUAL(MESSAGE2, packet2.get().message_id);
      CHECK_EQUAL(MESSAGE3, packet3.get().message_id);

      CHECK_EQUAL(1,   static_cast<Message1&>(packet1.get()).x);
      CHECK_EQUAL(2.2, static_cast<Message2&>(packet2.get()).x);
      CHECK_EQUAL("3", static_cast<Message3&>(packet3.get()).x);
    }

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

      // The next line should result in a compile error.
      //Packet packet4(message4);

      CHECK_EQUAL(MESSAGE1, packet1.get().message_id);
      CHECK_EQUAL(MESSAGE2, packet2.get().message_id);
      CHECK_EQUAL(MESSAGE3, packet3.get().message_id);

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

      CHECK_EQUAL(MESSAGE1, packet1.get().message_id);
      CHECK_EQUAL(MESSAGE2, packet2.get().message_id);
      CHECK_EQUAL(MESSAGE3, packet3.get().message_id);

      CHECK_EQUAL(1,   static_cast<Message1&>(packet1.get()).x);
      CHECK_EQUAL(2.2, static_cast<Message2&>(packet2.get()).x);
      CHECK_EQUAL("3", static_cast<Message3&>(packet3.get()).x);
    }
  };
}
