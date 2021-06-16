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

//*****************************************************************************
// This example demonstrates the basic method to queue messages to an etl::fsm
// derived class.
//*****************************************************************************

#include "etl/queue.h"
#include "etl/fsm.h"
#include "etl/message_packet.h"

#include <iostream>
#include <string>

//*****************************************************************************
// The messages.
//*****************************************************************************
struct Message1 : public etl::message<1>
{
  Message1(int i_)
    : i(i_)
  {
  }

  int i;
};

struct Message2 : public etl::message<2>
{
  Message2(double d_)
    : d(d_)
  {
  }

  double d;
};

struct Message3 : public etl::message<3>
{
  Message3(const std::string& s_)
    : s(s_)
  {
  }

  std::string s;
};

struct Message4 : public etl::message<4>
{
};

enum
{
  STATE1,
  STATE2
};

//*****************************************************************************
// The Finite State Machine.
//*****************************************************************************
class Fsm : public etl::fsm
{
public:

  //***************************************************************************
  // Constructor.
  //***************************************************************************
  Fsm()
    : fsm(1)
  {
  }

  //***************************************************************************
  // The overridden virtual receive function.
  //***************************************************************************
  void receive(const etl::imessage& msg_) override
  {
    if (accepts(msg_))
    {
      // Place in queue.
      queue.emplace(msg_);

      std::cout << "Queueing message " << int(msg_.get_message_id()) << std::endl;
    }
    else
    {
      std::cout << "Ignoring message " << int(msg_.get_message_id()) << std::endl;
    }
  }

  //***************************************************************************
  // The method to call to handle any queued messages.
  //***************************************************************************
  void process_queue()
  {
    while (!queue.empty())
    {
      message_packet& packet = queue.front();
      etl::imessage& msg = packet.get();
      std::cout << "Processing message " << int(msg.get_message_id()) << std::endl;

      // Call the base class's receive function.
      // This will route it to the correct 'on_event' handler.
      etl::fsm::receive(msg);

      queue.pop();
    }
  }

private:

  typedef etl::message_packet< Message1, Message2, Message3, Message4> message_packet;

  // The queue of message items.
  etl::queue<message_packet, 10> queue;
};

//*****************************************************************************
// State 1.
//*****************************************************************************
class State1 : public etl::fsm_state<Fsm, State1, STATE1, Message1, Message2, Message3>
{
public:

  //***************************************************************************
  // When we enter this state.
  //***************************************************************************
  etl::fsm_state_id_t on_enter_state() override
  {
    std::cout << "  S1 : Enter state" << std::endl;
    return STATE1;
  }

  //***************************************************************************
  void on_exit_state() override
  {
    std::cout << "  S1 : Exit state" << std::endl;
  }

  //***************************************************************************
  etl::fsm_state_id_t on_event(const Message1& msg)
  {
    std::cout << "  S1 : Received message " << int(msg.get_message_id()) << " : '" << msg.i << "'" << std::endl;
    std::cout.flush();
    return STATE1;
  }

  //***************************************************************************
  etl::fsm_state_id_t on_event(const Message2& msg)
  {
    std::cout << "  S1 : Received message " << int(msg.get_message_id()) << " : '" << msg.d << "'" << std::endl;
    std::cout.flush();
    return STATE1;
  }

  //***************************************************************************
  etl::fsm_state_id_t on_event(const Message3& msg)
  {
    std::cout << "  S1 : Received message " << int(msg.get_message_id()) << " : '" << msg.s << "'" << std::endl;
    std::cout.flush();
    return STATE1;
  }

  //***************************************************************************
  etl::fsm_state_id_t on_event_unknown(const etl::imessage& msg)
  {
    std::cout << "  S1 : Received unknown message " << int(msg.get_message_id()) << std::endl;
    std::cout.flush();
    return STATE2;
  }
};

//*****************************************************************************
// State 2.
//*****************************************************************************
class State2 : public etl::fsm_state<Fsm, State2, STATE2, Message1, Message2, Message3>
{
public:

  //***************************************************************************
  etl::fsm_state_id_t on_enter_state() override
  {
    std::cout << "  S2 : Enter state" << std::endl;
    return STATE2;
  }

  //***************************************************************************
  // When we enter this state.
  //***************************************************************************
  void on_exit_state() override
  {
    std::cout << "  S2 : Exit state" << std::endl;
  }

  //***************************************************************************
  etl::fsm_state_id_t on_event(const Message1& msg)
  {
    std::cout << "  S2 : Received message " << int(msg.get_message_id()) << " : '" << msg.i << "'" << std::endl;
    return STATE2;
  }

  //***************************************************************************
  etl::fsm_state_id_t on_event(const Message2& msg)
  {
    std::cout << "  S2 : Received message " << int(msg.get_message_id()) << " : '" << msg.d << "'" << std::endl;
    return STATE2;
  }

  //***************************************************************************
  etl::fsm_state_id_t on_event(const Message3& msg)
  {
    std::cout << "  S2 : Received message " << int(msg.get_message_id()) << " : '" << msg.s << "'" << std::endl;
    return STATE2;
  }

  //***************************************************************************
  etl::fsm_state_id_t on_event_unknown(const etl::imessage& msg)
  {
    std::cout << "  S2 : Received unknown message " << int(msg.get_message_id()) << std::endl;
    return STATE1;
  }
};

//*****************************************************************************
// The test application.
//*****************************************************************************
int main()
{
  Fsm fsm;

  State1 state1;
  State2 state2;

  // The list of states.
  etl::ifsm_state* state_list[] = { &state1, &state2 };

  // Define some messages.
  Message1 m1(1);
  Message2 m2(1.2);
  Message3 m3("Hello");

  // Set up the FSM
  fsm.set_states(state_list, 2);
  fsm.start();

  // Queue all of the messages.
  etl::send_message(fsm, m1);
  etl::send_message(fsm, Message1(2));
  etl::send_message(fsm, m2);
  etl::send_message(fsm, Message2(3.4));
  etl::send_message(fsm, m3);
  etl::send_message(fsm, Message3("World"));
  etl::send_message(fsm, Message4());

  std::cout << std::endl;

  // De-queue them
  fsm.process_queue();

  return 0;
}

