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

#include "etl/fsm.h"
#include "etl/enum_type.h"
#include "etl/container.h"
#include "etl/packet.h"
#include "etl/queue.h"

#include <iostream>


namespace
{
  const etl::message_router_id_t MOTOR_CONTROL = 0;


  //***************************************************************************
  // Events
  struct EventId
  {
    enum enum_type
    {
      START,
      STOP,
      STOPPED,
      SET_SPEED,
      RECURSIVE,
      UNSUPPORTED
    };

    ETL_DECLARE_ENUM_TYPE(EventId, etl::message_id_t)
    ETL_ENUM_TYPE(START,       "Start")
    ETL_ENUM_TYPE(STOP,        "Stop")
    ETL_ENUM_TYPE(STOPPED,     "Stopped")
    ETL_ENUM_TYPE(SET_SPEED,   "Set Speed")
    ETL_ENUM_TYPE(RECURSIVE,   "Recursive")
    ETL_ENUM_TYPE(UNSUPPORTED, "Unsupported")
    ETL_END_ENUM_TYPE
  };

  //***********************************
  class Start : public etl::message<EventId::START>
  {
  };

  //***********************************
  class Stop : public etl::message<EventId::STOP>
  {
  public:

    Stop() : isEmergencyStop(false) {}
    Stop(bool emergency) : isEmergencyStop(emergency) {}

    const bool isEmergencyStop;
  };

  //***********************************
  class SetSpeed : public etl::message<EventId::SET_SPEED>
  {
  public:

    SetSpeed(int speed_) : speed(speed_) {}

    const int speed;
  };

  //***********************************
  class Stopped : public etl::message<EventId::STOPPED>
  {
  };

  //***********************************
  class Recursive : public etl::message<EventId::RECURSIVE>
  {
  };

  //***********************************
  class Unsupported : public etl::message<EventId::UNSUPPORTED>
  {
  };

  //***************************************************************************
  // States
  struct StateId
  {
    enum enum_type
    {
      IDLE,
      RUNNING,
      WINDING_DOWN,
      LOCKED,
      NUMBER_OF_STATES
    };

    ETL_DECLARE_ENUM_TYPE(StateId, etl::fsm_state_id_t)
    ETL_ENUM_TYPE(IDLE,         "Idle")
    ETL_ENUM_TYPE(RUNNING,      "Running")
    ETL_ENUM_TYPE(WINDING_DOWN, "Winding Down")
    ETL_ENUM_TYPE(LOCKED,       "Locked")
    ETL_END_ENUM_TYPE
  };

  //***********************************
  // The motor control FSM.
  //***********************************
  class MotorControl : public etl::fsm
  {
  public:

    MotorControl()
      : fsm(MOTOR_CONTROL)
    {
    }

    //***********************************
    void Initialise(etl::ifsm_state** p_states, size_t size)
    {
      set_states(p_states, size);
      ClearStatistics();
    }

    //***********************************
    void ClearStatistics()
    {
      startCount = 0;
      stopCount = 0;
      setSpeedCount = 0;
      unknownCount = 0;
      stoppedCount = 0;
      isLampOn = false;
      speed = 0;
    }

    //***********************************
    void SetSpeedValue(int speed_)
    {
      speed = speed_;
    }

    //***********************************
    void TurnRunningLampOn()
    {
      isLampOn = true;
    }

    //***********************************
    void TurnRunningLampOff()
    {
      isLampOn = false;
    }

    //***********************************
    template <typename T>
    void queue_recursive_message(const T& message)
    {
      messageQueue.emplace(message);
    }

    typedef etl::largest<Start, Stop, SetSpeed, Stopped, Recursive> Largest_t;

    typedef etl::packet<etl::imessage, Largest_t::size, Largest_t::alignment> Packet_t;

    etl::queue<Packet_t, 2> messageQueue;

    int startCount;
    int stopCount;
    int setSpeedCount;
    int unknownCount;
    int stoppedCount;
    bool isLampOn;
    int speed;
  };

  //***********************************
  // The idle state.
  //***********************************
  class Idle : public etl::fsm_state<MotorControl, Idle, StateId::IDLE, Start, Recursive>
  {
  public:

    //***********************************
    etl::fsm_state_id_t on_event(const Start&)
    {
      ++get_fsm_context().startCount;
      return StateId::RUNNING;
    }

    //***********************************
    etl::fsm_state_id_t on_event(const Recursive&)
    {
      get_fsm_context().queue_recursive_message(Start());
      return StateId::IDLE;
    }

    //***********************************
    etl::fsm_state_id_t on_event_unknown(const etl::imessage&)
    {
      ++get_fsm_context().unknownCount;
      return StateId::IDLE; //No_State_Change;
    }

    //***********************************
    etl::fsm_state_id_t on_enter_state()
    {
      get_fsm_context().TurnRunningLampOff();
      return StateId::LOCKED;
    }
  };

  //***********************************
  // The running state.
  //***********************************
  class Running : public etl::fsm_state<MotorControl, Running, StateId::RUNNING, Stop, SetSpeed>
  {
  public:

    //***********************************
    etl::fsm_state_id_t on_event(const Stop& event)
    {
      ++get_fsm_context().stopCount;

      if (event.isEmergencyStop)
      {
        return StateId::IDLE;
      }
      else
      {
        return StateId::WINDING_DOWN;
      }
    }

    //***********************************
    etl::fsm_state_id_t on_event(const SetSpeed& event)
    {
      ++get_fsm_context().setSpeedCount;
      get_fsm_context().SetSpeedValue(event.speed);
      return STATE_ID;
    }

    //***********************************
    etl::fsm_state_id_t on_event_unknown(const etl::imessage&)
    {
      ++get_fsm_context().unknownCount;
      return STATE_ID;
    }

    //***********************************
    etl::fsm_state_id_t on_enter_state()
    {
      get_fsm_context().TurnRunningLampOn();

      return STATE_ID;
    }
  };

  //***********************************
  // The winding down state.
  //***********************************
  class WindingDown : public etl::fsm_state<MotorControl, WindingDown, StateId::WINDING_DOWN, Stopped>
  {
  public:

    //***********************************
    etl::fsm_state_id_t on_event(const Stopped&)
    {
      ++get_fsm_context().stoppedCount;
      return StateId::IDLE;
    }

    //***********************************
    etl::fsm_state_id_t on_event_unknown(const etl::imessage&)
    {
      ++get_fsm_context().unknownCount;
      return STATE_ID;
    }
  };

  //***********************************
  // The locked state.
  //***********************************
  class Locked : public etl::fsm_state<MotorControl, Locked, StateId::LOCKED>
  {
  public:

    //***********************************
    etl::fsm_state_id_t on_event_unknown(const etl::imessage&)
    {
      ++get_fsm_context().unknownCount;
      return STATE_ID;
    }
  };

  // The states.
  Idle        idle;
  Running     running;
  WindingDown windingDown;
  Locked      locked;

  etl::ifsm_state* stateList[StateId::NUMBER_OF_STATES] =
  {
    &idle, &running, &windingDown, &locked
  };

  MotorControl motorControl;

  SUITE(test_fsm_states)
  {
    //*************************************************************************
    TEST(test_fsm)
    {
      etl::null_message_router nmr;

      CHECK(motorControl.is_producer());
      CHECK(motorControl.is_consumer());

      motorControl.Initialise(stateList, ETL_OR_STD17::size(stateList));
      motorControl.reset();
      motorControl.ClearStatistics();

      CHECK(!motorControl.is_started());

      // Start the FSM.
      motorControl.start(false);
      CHECK(motorControl.is_started());

      // Now in Idle state.

      CHECK_EQUAL(StateId::IDLE, int(motorControl.get_state_id()));
      CHECK_EQUAL(StateId::IDLE, int(motorControl.get_state().get_state_id()));

      CHECK_EQUAL(false, motorControl.isLampOn);
      CHECK_EQUAL(0, motorControl.setSpeedCount);
      CHECK_EQUAL(0, motorControl.speed);
      CHECK_EQUAL(0, motorControl.startCount);
      CHECK_EQUAL(0, motorControl.stopCount);
      CHECK_EQUAL(0, motorControl.stoppedCount);
      CHECK_EQUAL(0, motorControl.unknownCount);

      // Send unhandled events.
      motorControl.receive(Stop());
      motorControl.receive(Stopped());
      motorControl.receive(SetSpeed(10));

      CHECK_EQUAL(StateId::IDLE, motorControl.get_state_id());
      CHECK_EQUAL(StateId::IDLE, motorControl.get_state().get_state_id());

      CHECK_EQUAL(false, motorControl.isLampOn);
      CHECK_EQUAL(0, motorControl.setSpeedCount);
      CHECK_EQUAL(0, motorControl.speed);
      CHECK_EQUAL(0, motorControl.startCount);
      CHECK_EQUAL(0, motorControl.stopCount);
      CHECK_EQUAL(0, motorControl.stoppedCount);
      CHECK_EQUAL(3, motorControl.unknownCount);

      // Send Start event.
      motorControl.receive(Start());

      // Now in Running state.

      CHECK_EQUAL(StateId::RUNNING, int(motorControl.get_state_id()));
      CHECK_EQUAL(StateId::RUNNING, int(motorControl.get_state().get_state_id()));

      CHECK_EQUAL(true, motorControl.isLampOn);
      CHECK_EQUAL(0, motorControl.setSpeedCount);
      CHECK_EQUAL(0, motorControl.speed);
      CHECK_EQUAL(1, motorControl.startCount);
      CHECK_EQUAL(0, motorControl.stopCount);
      CHECK_EQUAL(0, motorControl.stoppedCount);
      CHECK_EQUAL(3, motorControl.unknownCount);

      // Send unhandled events.
      motorControl.receive(Start());
      motorControl.receive(Stopped());

      CHECK_EQUAL(StateId::RUNNING, int(motorControl.get_state_id()));
      CHECK_EQUAL(StateId::RUNNING, int(motorControl.get_state().get_state_id()));

      CHECK_EQUAL(true, motorControl.isLampOn);
      CHECK_EQUAL(0, motorControl.setSpeedCount);
      CHECK_EQUAL(0, motorControl.speed);
      CHECK_EQUAL(1, motorControl.startCount);
      CHECK_EQUAL(0, motorControl.stopCount);
      CHECK_EQUAL(0, motorControl.stoppedCount);
      CHECK_EQUAL(5, motorControl.unknownCount);

      // Send SetSpeed event.
      motorControl.receive(SetSpeed(100));

      // Still in Running state.

      CHECK_EQUAL(StateId::RUNNING, int(motorControl.get_state_id()));
      CHECK_EQUAL(StateId::RUNNING, int(motorControl.get_state().get_state_id()));

      CHECK_EQUAL(true, motorControl.isLampOn);
      CHECK_EQUAL(1, motorControl.setSpeedCount);
      CHECK_EQUAL(100, motorControl.speed);
      CHECK_EQUAL(1, motorControl.startCount);
      CHECK_EQUAL(0, motorControl.stopCount);
      CHECK_EQUAL(0, motorControl.stoppedCount);
      CHECK_EQUAL(5, motorControl.unknownCount);

      // Send Stop event.
      motorControl.receive(Stop());

      // Now in WindingDown state.

      CHECK_EQUAL(StateId::WINDING_DOWN, int(motorControl.get_state_id()));
      CHECK_EQUAL(StateId::WINDING_DOWN, int(motorControl.get_state().get_state_id()));

      CHECK_EQUAL(true, motorControl.isLampOn);
      CHECK_EQUAL(1, motorControl.setSpeedCount);
      CHECK_EQUAL(100, motorControl.speed);
      CHECK_EQUAL(1, motorControl.startCount);
      CHECK_EQUAL(1, motorControl.stopCount);
      CHECK_EQUAL(0, motorControl.stoppedCount);
      CHECK_EQUAL(5, motorControl.unknownCount);

      // Send unhandled events.
      motorControl.receive(Start());
      motorControl.receive(Stop());
      motorControl.receive(SetSpeed(100));

      CHECK_EQUAL(StateId::WINDING_DOWN, int(motorControl.get_state_id()));
      CHECK_EQUAL(StateId::WINDING_DOWN, int(motorControl.get_state().get_state_id()));

      CHECK_EQUAL(true, motorControl.isLampOn);
      CHECK_EQUAL(1, motorControl.setSpeedCount);
      CHECK_EQUAL(100, motorControl.speed);
      CHECK_EQUAL(1, motorControl.startCount);
      CHECK_EQUAL(1, motorControl.stopCount);
      CHECK_EQUAL(0, motorControl.stoppedCount);
      CHECK_EQUAL(8, motorControl.unknownCount);

      // Send Stopped event.
      motorControl.receive(Stopped());

      // Now in Locked state via Idle state.
      CHECK_EQUAL(StateId::LOCKED, int(motorControl.get_state_id()));
      CHECK_EQUAL(StateId::LOCKED, int(motorControl.get_state().get_state_id()));

      CHECK_EQUAL(false, motorControl.isLampOn);
      CHECK_EQUAL(1, motorControl.setSpeedCount);
      CHECK_EQUAL(100, motorControl.speed);
      CHECK_EQUAL(1, motorControl.startCount);
      CHECK_EQUAL(1, motorControl.stopCount);
      CHECK_EQUAL(1, motorControl.stoppedCount);
      CHECK_EQUAL(8, motorControl.unknownCount);
    }

    //*************************************************************************
    TEST(test_fsm_emergency_stop)
    {
      etl::null_message_router nmr;

      motorControl.Initialise(stateList, ETL_OR_STD17::size(stateList)); 
      motorControl.reset();
      motorControl.ClearStatistics();

      CHECK(!motorControl.is_started());

      // Start the FSM.
      motorControl.start(false);
      CHECK(motorControl.is_started());

      // Now in Idle state.

      // Send Start event.
      motorControl.receive(Start());

      // Now in Running state.

      CHECK_EQUAL(StateId::RUNNING, int(motorControl.get_state_id()));
      CHECK_EQUAL(StateId::RUNNING, int(motorControl.get_state().get_state_id()));

      CHECK_EQUAL(true, motorControl.isLampOn);
      CHECK_EQUAL(0, motorControl.setSpeedCount);
      CHECK_EQUAL(0, motorControl.speed);
      CHECK_EQUAL(1, motorControl.startCount);
      CHECK_EQUAL(0, motorControl.stopCount);
      CHECK_EQUAL(0, motorControl.stoppedCount);
      CHECK_EQUAL(0, motorControl.unknownCount);

      // Send emergency Stop event.
      motorControl.receive(Stop(true));

      // Now in Locked state via Idle state.
      CHECK_EQUAL(StateId::LOCKED, int(motorControl.get_state_id()));
      CHECK_EQUAL(StateId::LOCKED, int(motorControl.get_state().get_state_id()));

      CHECK_EQUAL(false, motorControl.isLampOn);
      CHECK_EQUAL(0, motorControl.setSpeedCount);
      CHECK_EQUAL(0, motorControl.speed);
      CHECK_EQUAL(1, motorControl.startCount);
      CHECK_EQUAL(1, motorControl.stopCount);
      CHECK_EQUAL(0, motorControl.stoppedCount);
      CHECK_EQUAL(0, motorControl.unknownCount);
    }

    //*************************************************************************
    TEST(test_fsm_recursive_event)
    {
      etl::null_message_router nmr;

      motorControl.Initialise(stateList, ETL_OR_STD17::size(stateList));
      motorControl.reset();
      motorControl.ClearStatistics();

      motorControl.messageQueue.clear();

      // Start the FSM.
      motorControl.start(false);

      // Now in Idle state.
      // Send Start event.
      motorControl.receive(Recursive());

      CHECK_EQUAL(1U, motorControl.messageQueue.size());

      // Send the queued message.
      motorControl.receive(motorControl.messageQueue.front().get());
      motorControl.messageQueue.pop();

      // Now in Running state.

      CHECK_EQUAL(StateId::RUNNING, int(motorControl.get_state_id()));
      CHECK_EQUAL(StateId::RUNNING, int(motorControl.get_state().get_state_id()));

      CHECK_EQUAL(true, motorControl.isLampOn);
      CHECK_EQUAL(0, motorControl.setSpeedCount);
      CHECK_EQUAL(0, motorControl.speed);
      CHECK_EQUAL(1, motorControl.startCount);
      CHECK_EQUAL(0, motorControl.stopCount);
      CHECK_EQUAL(0, motorControl.stoppedCount);
      CHECK_EQUAL(0, motorControl.unknownCount);
    }

    //*************************************************************************
    TEST(test_fsm_supported)
    {
      CHECK(motorControl.accepts(EventId::SET_SPEED));
      CHECK(motorControl.accepts(EventId::START));
      CHECK(motorControl.accepts(EventId::STOP));
      CHECK(motorControl.accepts(EventId::STOPPED));
      CHECK(motorControl.accepts(EventId::UNSUPPORTED));

      CHECK(motorControl.accepts(SetSpeed(0)));
      CHECK(motorControl.accepts(Start()));
      CHECK(motorControl.accepts(Stop()));
      CHECK(motorControl.accepts(Stopped()));
      CHECK(motorControl.accepts(Unsupported()));
    }

    //*************************************************************************
    TEST(test_fsm_no_states)
    {
      MotorControl mc;

      // No states.
      etl::ifsm_state** stateList = nullptr;

      CHECK_THROW(mc.set_states(stateList, 0U), etl::fsm_state_list_exception);
    }

    //*************************************************************************
    TEST(test_fsm_null_state)
    {
      MotorControl mc;

      // Null state.
      etl::ifsm_state* stateList[StateId::NUMBER_OF_STATES] =
      {
        &idle, &running,& windingDown, nullptr
      };

      CHECK_THROW(mc.set_states(stateList, StateId::NUMBER_OF_STATES), etl::fsm_null_state_exception);
    }

    //*************************************************************************
    TEST(test_fsm_incorrect_state_order)
    {
      MotorControl mc;

      // Incorrect order.
      etl::ifsm_state* stateList[StateId::NUMBER_OF_STATES] =
      {
        &idle, &windingDown, &running, &locked
      };

      CHECK_THROW(mc.set_states(stateList, StateId::NUMBER_OF_STATES), etl::fsm_state_list_order_exception);
    }
  };
}
