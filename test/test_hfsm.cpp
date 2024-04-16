/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl
https://www.etlcpp.com

Copyright(c) 2021

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

#include "etl/hfsm.h"
#include "etl/enum_type.h"
#include "etl/container.h"
#include "etl/packet.h"
#include "etl/queue.h"
#include "etl/circular_buffer.h"

#include <iostream>

// This test implements the following state machine:
//                +--------------------------------------------+
//                |                                            |
//                |     O       running                        |
//                |     |                                      |
//   O            |     v                                      |
//   |            |    +-----------+        +-----------+      |
//   v            |    |           |Timeout |           |      |
// +------+ Start |    | windingUp +------->|  atSpeed  |      |
// | idle +-------+--->|           |        |           |      |
// +------+       |    +------+----+        +------+----+      |
//  ^  ^          |           |                    |           |
//  |  |          |      Stop |                    |           |
//  |  |          |           v                    |Stop       |
//  |  |          |        +------------------+    |           |
//  |  | Stopped  |        |                  |    |           |
//  |  +----------+--------+   windingDown    |<---+           |
//  |             |        |                  |                |
//  | EStop       |        +------------------+                |
//  +-------------+                                            |
//                |                                            |
//                +--------------------------------------------+
// Created with asciiflow.com
namespace
{
  struct HFsmId
  {
    enum
    {
      Motor_Control
    };
  };

  //***************************************************************************
  // Events
  struct EventId
  {
    enum enum_type
    {
      Start,
      Stop,
      EStop,
      Stopped,
      Set_Speed,
      Recursive,
      Timeout,
      Unsupported
    };

    ETL_DECLARE_ENUM_TYPE(EventId, etl::message_id_t)
    ETL_ENUM_TYPE(Start,       "Start")
    ETL_ENUM_TYPE(Stop,        "Stop")
    ETL_ENUM_TYPE(EStop,       "E-Stop")
    ETL_ENUM_TYPE(Stopped,     "Stopped")
    ETL_ENUM_TYPE(Set_Speed,   "Set Speed")
    ETL_ENUM_TYPE(Recursive,   "Recursive")
    ETL_ENUM_TYPE(Timeout,     "Timeout")
    ETL_ENUM_TYPE(Unsupported, "Unsupported")
    ETL_END_ENUM_TYPE
  };

  //***********************************
  class Start : public etl::message<EventId::Start>
  {
  };

  //***********************************
  class Stop : public etl::message<EventId::Stop>
  {
  };

  //***********************************
  class EStop : public etl::message<EventId::EStop>
  {
  };

  //***********************************
  class SetSpeed : public etl::message<EventId::Set_Speed>
  {
  public:

    SetSpeed(int speed_) : speed(speed_) {}

    const int speed;
  };

  //***********************************
  class Stopped : public etl::message<EventId::Stopped>
  {
  };

  //***********************************
  class Recursive : public etl::message<EventId::Recursive>
  {
  };

  //***********************************
  class Timeout : public etl::message<EventId::Timeout>
  {
  };

  //***********************************
  class Unsupported : public etl::message<EventId::Unsupported>
  {
  };

  //***************************************************************************
  // States
  struct StateId
  {
    enum enum_type
    {
      Idle,
      Running,
      Winding_Up,
      Winding_Down,
      At_Speed,
      Number_Of_States
    };

    ETL_DECLARE_ENUM_TYPE(StateId, etl::fsm_state_id_t)
    ETL_ENUM_TYPE(Idle,         "Idle")
    ETL_ENUM_TYPE(Running,      "Running")
    ETL_ENUM_TYPE(Winding_Up,   "Winding Up")
    ETL_ENUM_TYPE(Winding_Down, "Winding Down")
    ETL_ENUM_TYPE(At_Speed,     "At Speed")
    ETL_END_ENUM_TYPE
  };

  //***********************************
  // The motor control FSM.
  //***********************************
  class MotorControl : public etl::hfsm
  {
  public:

    MotorControl()
      : hfsm(HFsmId::Motor_Control)
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
      windUpCompleteCount = 0;
      windUpStartCount = 0;
      unknownCount = 0;
      stoppedCount = 0;
      isLampOn = false;
      speed = 0;

      stateEnterHistory.clear();
      stateExitHistory.clear();
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

    typedef etl::largest<Start, Stop, EStop, SetSpeed, Stopped, Recursive, Timeout> Largest_t;

    typedef etl::packet<etl::imessage, Largest_t::size, Largest_t::alignment> Packet_t;

    etl::queue<Packet_t, 2> messageQueue;

    int startCount;
    int stopCount;
    int windUpCompleteCount;
    int windUpStartCount;
    int setSpeedCount;
    int unknownCount;
    int stoppedCount;
    bool isLampOn;
    int speed;

    // A circular buffer is used so that data overflows won't assert in tests which don't use this data
    etl::circular_buffer<StateId::enum_type, 10UL> stateEnterHistory{};
    etl::circular_buffer<StateId::enum_type, 10UL> stateExitHistory{};
  };

  //***********************************
  // The idle state.
  //***********************************
  class Idle : public etl::fsm_state<MotorControl, Idle, StateId::Idle, Start, Recursive>
  {
  public:

    //***********************************
    etl::fsm_state_id_t on_event(const Start&)
    {
      ++get_fsm_context().startCount;
      return StateId::Running;
    }

    //***********************************
    etl::fsm_state_id_t on_event(const Recursive&)
    {
      get_fsm_context().queue_recursive_message(Start());
      return StateId::Idle;
    }

    //***********************************
    etl::fsm_state_id_t on_event_unknown(const etl::imessage&)
    {
      ++get_fsm_context().unknownCount;
      return No_State_Change;
    }

    //***********************************
    etl::fsm_state_id_t on_enter_state()
    {
      auto& context = get_fsm_context();
      context.stateEnterHistory.push(static_cast<StateId::enum_type>(get_state_id()));
      context.TurnRunningLampOff();

      return No_State_Change;
    }

    //***********************************
    void on_exit_state()
    {
      auto& context = get_fsm_context();
      context.stateExitHistory.push(static_cast<StateId::enum_type>(get_state_id()));
    }
  };

  //***********************************
  // The running state.
  //***********************************
  class Running : public etl::fsm_state<MotorControl, Running, StateId::Running, EStop>
  {
  public:

    //***********************************
    etl::fsm_state_id_t on_event(const EStop&)
    {
      ++get_fsm_context().stopCount;

      return StateId::Idle;
    }

    //***********************************
    etl::fsm_state_id_t on_event_unknown(const etl::imessage&)
    {
      ++get_fsm_context().unknownCount;
      return No_State_Change;
    }

    //***********************************
    etl::fsm_state_id_t on_enter_state()
    {
      auto& context = get_fsm_context();
      context.stateEnterHistory.push(static_cast<StateId::enum_type>(get_state_id()));
      context.TurnRunningLampOn();

      return No_State_Change;
    }

    //***********************************
    void on_exit_state()
    {
      auto& context = get_fsm_context();
      context.stateExitHistory.push(static_cast<StateId::enum_type>(get_state_id()));
    }
  };

  //***********************************
  // The winding up state.
  //***********************************
  class WindingUp : public etl::fsm_state<MotorControl, WindingUp, StateId::Winding_Up, Stop, Timeout>
  {
  public:

    //***********************************
    etl::fsm_state_id_t on_event(const Stop&)
    {
      ++get_fsm_context().stopCount;
      return StateId::Winding_Down;
    }

    //***********************************
    etl::fsm_state_id_t on_event(const Timeout&)
    {
      ++get_fsm_context().windUpCompleteCount;
      return StateId::At_Speed;
    }

    //***********************************
    etl::fsm_state_id_t on_event_unknown(const etl::imessage&)
    {
      ++get_fsm_context().unknownCount;

      return No_State_Change;
    }

    //***********************************
    etl::fsm_state_id_t on_enter_state()
    {
      auto& context = get_fsm_context();
      context.stateEnterHistory.push(static_cast<StateId::enum_type>(get_state_id()));
      ++context.windUpStartCount;
      return No_State_Change;
    }

    //***********************************
    void on_exit_state()
    {
      auto& context = get_fsm_context();
      context.stateExitHistory.push(static_cast<StateId::enum_type>(get_state_id()));
    }
  };

  //***********************************
  // The at speed state.
  //***********************************
  class AtSpeed : public etl::fsm_state<MotorControl, AtSpeed, StateId::At_Speed, SetSpeed, Stop>
  {
  public:
    //***********************************
    etl::fsm_state_id_t on_event(const Stop&)
    {
      ++get_fsm_context().stopCount;
      return StateId::Winding_Down;
    }

    //***********************************
    etl::fsm_state_id_t on_event(const SetSpeed& event)
    {
      ++get_fsm_context().setSpeedCount;
      get_fsm_context().SetSpeedValue(event.speed);
      //return No_State_Change;
      return this->get_state_id();
    }

    //***********************************
    etl::fsm_state_id_t on_event_unknown(const etl::imessage&)
    {
      ++get_fsm_context().unknownCount;
      return No_State_Change;
    }

    //***********************************
    void on_exit_state()
    {
      auto& context = get_fsm_context();
      context.stateExitHistory.push(static_cast<StateId::enum_type>(get_state_id()));
    }
  };

  //***********************************
  // The winding down state.
  //***********************************
  class WindingDown : public etl::fsm_state<MotorControl, WindingDown, StateId::Winding_Down, Stopped, EStop>
  {
  public:

    etl::fsm_state_id_t on_event(const EStop&)
    {
      ++get_fsm_context().stopCount;
      return Pass_To_Parent;
    }

    //***********************************
    etl::fsm_state_id_t on_event(const Stopped&)
    {
      ++get_fsm_context().stoppedCount;
      return StateId::Idle;
    }

    //***********************************
    etl::fsm_state_id_t on_event_unknown(const etl::imessage&)
    {
      ++get_fsm_context().unknownCount;
      return No_State_Change;
    }
  };

  // The states.
  Idle        idle;
  Running     running;
  WindingUp   windingUp;
  WindingDown windingDown;
  AtSpeed     atSpeed;

  etl::ifsm_state* stateList[StateId::Number_Of_States] =
  {
    &idle, &running, &windingUp, &windingDown, &atSpeed
  };

  etl::ifsm_state* childStates[] =
  {
    &windingUp, &atSpeed, &windingDown
  };

  MotorControl motorControl;

  SUITE(test_hfsm_states)
  {
    //*************************************************************************
    TEST(test_hfsm)
    {
      etl::null_message_router nmr;

      CHECK(motorControl.is_producer());
      CHECK(motorControl.is_consumer());

      running.set_child_states(childStates, ETL_OR_STD17::size(childStates));

      motorControl.Initialise(stateList, ETL_OR_STD17::size(stateList));
      motorControl.reset();
      motorControl.ClearStatistics();

      CHECK(!motorControl.is_started());

      // Start the FSM.
      motorControl.start(false);
      CHECK(motorControl.is_started());

      // Now in Idle state.

      CHECK_EQUAL(StateId::Idle, int(motorControl.get_state_id()));
      CHECK_EQUAL(StateId::Idle, int(motorControl.get_state().get_state_id()));

      CHECK_EQUAL(false, motorControl.isLampOn);
      CHECK_EQUAL(0, motorControl.setSpeedCount);
      CHECK_EQUAL(0, motorControl.speed);
      CHECK_EQUAL(0, motorControl.startCount);
      CHECK_EQUAL(0, motorControl.stopCount);
      CHECK_EQUAL(0, motorControl.windUpCompleteCount);
      CHECK_EQUAL(0, motorControl.windUpStartCount);
      CHECK_EQUAL(0, motorControl.stoppedCount);
      CHECK_EQUAL(0, motorControl.unknownCount);

      // Send unhandled events.
      motorControl.receive(Stop());
      motorControl.receive(Stopped());
      motorControl.receive(SetSpeed(10));

      CHECK_EQUAL(StateId::Idle, motorControl.get_state_id());
      CHECK_EQUAL(StateId::Idle, motorControl.get_state().get_state_id());

      CHECK_EQUAL(false, motorControl.isLampOn);
      CHECK_EQUAL(0, motorControl.setSpeedCount);
      CHECK_EQUAL(0, motorControl.speed);
      CHECK_EQUAL(0, motorControl.startCount);
      CHECK_EQUAL(0, motorControl.stopCount);
      CHECK_EQUAL(0, motorControl.stoppedCount);
      CHECK_EQUAL(3, motorControl.unknownCount);
      CHECK_EQUAL(0, motorControl.windUpCompleteCount);
      CHECK_EQUAL(0, motorControl.windUpStartCount);

      // Send Start event.
      motorControl.receive(Start());

      // Now in WindingUp state.

      CHECK_EQUAL(StateId::Winding_Up, int(motorControl.get_state_id()));
      CHECK_EQUAL(StateId::Winding_Up, int(motorControl.get_state().get_state_id()));

      CHECK_EQUAL(true, motorControl.isLampOn);
      CHECK_EQUAL(0, motorControl.setSpeedCount);
      CHECK_EQUAL(0, motorControl.speed);
      CHECK_EQUAL(1, motorControl.startCount);
      CHECK_EQUAL(0, motorControl.stopCount);
      CHECK_EQUAL(0, motorControl.stoppedCount);
      CHECK_EQUAL(3, motorControl.unknownCount);
      CHECK_EQUAL(0, motorControl.windUpCompleteCount);
      CHECK_EQUAL(1, motorControl.windUpStartCount);

      // Send unhandled events.
      motorControl.receive(Start());
      motorControl.receive(Stopped());

      CHECK_EQUAL(StateId::Winding_Up, int(motorControl.get_state_id()));
      CHECK_EQUAL(StateId::Winding_Up, int(motorControl.get_state().get_state_id()));

      CHECK_EQUAL(true, motorControl.isLampOn);
      CHECK_EQUAL(0, motorControl.setSpeedCount);
      CHECK_EQUAL(0, motorControl.speed);
      CHECK_EQUAL(1, motorControl.startCount);
      CHECK_EQUAL(0, motorControl.stopCount);
      CHECK_EQUAL(0, motorControl.stoppedCount);
      CHECK_EQUAL(5, motorControl.unknownCount);
      CHECK_EQUAL(0, motorControl.windUpCompleteCount);
      CHECK_EQUAL(1, motorControl.windUpStartCount);

      // Send Timeout event
      motorControl.receive(Timeout());

      CHECK_EQUAL(StateId::At_Speed, int(motorControl.get_state_id()));
      CHECK_EQUAL(StateId::At_Speed, int(motorControl.get_state().get_state_id()));

      CHECK_EQUAL(true, motorControl.isLampOn);
      CHECK_EQUAL(0, motorControl.setSpeedCount);
      CHECK_EQUAL(0, motorControl.speed);
      CHECK_EQUAL(1, motorControl.startCount);
      CHECK_EQUAL(0, motorControl.stopCount);
      CHECK_EQUAL(0, motorControl.stoppedCount);
      CHECK_EQUAL(5, motorControl.unknownCount);
      CHECK_EQUAL(1, motorControl.windUpCompleteCount);
      CHECK_EQUAL(1, motorControl.windUpStartCount);

      // Send SetSpeed event.
      motorControl.receive(SetSpeed(100));

      // Still in at speed state.

      CHECK_EQUAL(StateId::At_Speed, int(motorControl.get_state_id()));
      CHECK_EQUAL(StateId::At_Speed, int(motorControl.get_state().get_state_id()));

      CHECK_EQUAL(true, motorControl.isLampOn);
      CHECK_EQUAL(1, motorControl.setSpeedCount);
      CHECK_EQUAL(100, motorControl.speed);
      CHECK_EQUAL(1, motorControl.startCount);
      CHECK_EQUAL(0, motorControl.stopCount);
      CHECK_EQUAL(0, motorControl.stoppedCount);
      CHECK_EQUAL(5, motorControl.unknownCount);
      CHECK_EQUAL(1, motorControl.windUpCompleteCount);
      CHECK_EQUAL(1, motorControl.windUpStartCount);

      // Send Stop event.
      motorControl.receive(Stop());

      // Now in WindingDown state.

      CHECK_EQUAL(StateId::Winding_Down, int(motorControl.get_state_id()));
      CHECK_EQUAL(StateId::Winding_Down, int(motorControl.get_state().get_state_id()));

      CHECK_EQUAL(true, motorControl.isLampOn);
      CHECK_EQUAL(1, motorControl.setSpeedCount);
      CHECK_EQUAL(100, motorControl.speed);
      CHECK_EQUAL(1, motorControl.startCount);
      CHECK_EQUAL(1, motorControl.stopCount);
      CHECK_EQUAL(0, motorControl.stoppedCount);
      CHECK_EQUAL(5, motorControl.unknownCount);
      CHECK_EQUAL(1, motorControl.windUpCompleteCount);
      CHECK_EQUAL(1, motorControl.windUpStartCount);

      // Send unhandled events.
      motorControl.receive(Start());
      motorControl.receive(Stop());
      motorControl.receive(SetSpeed(100));

      CHECK_EQUAL(StateId::Winding_Down, int(motorControl.get_state_id()));
      CHECK_EQUAL(StateId::Winding_Down, int(motorControl.get_state().get_state_id()));

      CHECK_EQUAL(true, motorControl.isLampOn);
      CHECK_EQUAL(1, motorControl.setSpeedCount);
      CHECK_EQUAL(100, motorControl.speed);
      CHECK_EQUAL(1, motorControl.startCount);
      CHECK_EQUAL(1, motorControl.stopCount);
      CHECK_EQUAL(0, motorControl.stoppedCount);
      CHECK_EQUAL(8, motorControl.unknownCount);
      CHECK_EQUAL(1, motorControl.windUpCompleteCount);
      CHECK_EQUAL(1, motorControl.windUpStartCount);

      // Send Stopped event.
      motorControl.receive(Stopped());

      // Now in Idle state.
      CHECK_EQUAL(StateId::Idle, int(motorControl.get_state_id()));
      CHECK_EQUAL(StateId::Idle, int(motorControl.get_state().get_state_id()));

      CHECK_EQUAL(false, motorControl.isLampOn);
      CHECK_EQUAL(1, motorControl.setSpeedCount);
      CHECK_EQUAL(100, motorControl.speed);
      CHECK_EQUAL(1, motorControl.startCount);
      CHECK_EQUAL(1, motorControl.stopCount);
      CHECK_EQUAL(1, motorControl.stoppedCount);
      CHECK_EQUAL(8, motorControl.unknownCount);
      CHECK_EQUAL(1, motorControl.windUpCompleteCount);
      CHECK_EQUAL(1, motorControl.windUpStartCount);
    }

    //*************************************************************************
    TEST(test_hfsm_emergency_stop_from_winding_up)
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

      // Now in winding up state.

      CHECK_EQUAL(StateId::Winding_Up, int(motorControl.get_state_id()));
      CHECK_EQUAL(StateId::Winding_Up, int(motorControl.get_state().get_state_id()));

      CHECK_EQUAL(true, motorControl.isLampOn);
      CHECK_EQUAL(0, motorControl.setSpeedCount);
      CHECK_EQUAL(0, motorControl.speed);
      CHECK_EQUAL(1, motorControl.startCount);
      CHECK_EQUAL(0, motorControl.stopCount);
      CHECK_EQUAL(0, motorControl.stoppedCount);
      CHECK_EQUAL(0, motorControl.unknownCount);
      CHECK_EQUAL(0, motorControl.windUpCompleteCount);
      CHECK_EQUAL(1, motorControl.windUpStartCount);

      // Send emergency Stop event.
      motorControl.receive(EStop());

      // Now in Idle state.
      CHECK_EQUAL(StateId::Idle, int(motorControl.get_state_id()));
      CHECK_EQUAL(StateId::Idle, int(motorControl.get_state().get_state_id()));

      CHECK_EQUAL(false, motorControl.isLampOn);
      CHECK_EQUAL(0, motorControl.setSpeedCount);
      CHECK_EQUAL(0, motorControl.speed);
      CHECK_EQUAL(1, motorControl.startCount);
      CHECK_EQUAL(1, motorControl.stopCount);
      CHECK_EQUAL(0, motorControl.stoppedCount);
      CHECK_EQUAL(0, motorControl.unknownCount);
      CHECK_EQUAL(0, motorControl.windUpCompleteCount);
      CHECK_EQUAL(1, motorControl.windUpStartCount);
    }

    //*************************************************************************
    TEST(test_hfsm_emergency_stop_from_at_speed)
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
      motorControl.receive(Timeout());

      // Now in at speed state.

      CHECK_EQUAL(StateId::At_Speed, int(motorControl.get_state_id()));
      CHECK_EQUAL(StateId::At_Speed, int(motorControl.get_state().get_state_id()));

      CHECK_EQUAL(true, motorControl.isLampOn);
      CHECK_EQUAL(0, motorControl.setSpeedCount);
      CHECK_EQUAL(0, motorControl.speed);
      CHECK_EQUAL(1, motorControl.startCount);
      CHECK_EQUAL(0, motorControl.stopCount);
      CHECK_EQUAL(0, motorControl.stoppedCount);
      CHECK_EQUAL(0, motorControl.unknownCount);
      CHECK_EQUAL(1, motorControl.windUpCompleteCount);
      CHECK_EQUAL(1, motorControl.windUpStartCount);

      // Send emergency Stop event.
      motorControl.receive(EStop());

      // Now in Idle state.
      CHECK_EQUAL(StateId::Idle, int(motorControl.get_state_id()));
      CHECK_EQUAL(StateId::Idle, int(motorControl.get_state().get_state_id()));

      CHECK_EQUAL(false, motorControl.isLampOn);
      CHECK_EQUAL(0, motorControl.setSpeedCount);
      CHECK_EQUAL(0, motorControl.speed);
      CHECK_EQUAL(1, motorControl.startCount);
      CHECK_EQUAL(1, motorControl.stopCount);
      CHECK_EQUAL(0, motorControl.stoppedCount);
      CHECK_EQUAL(0, motorControl.unknownCount);
      CHECK_EQUAL(1, motorControl.windUpCompleteCount);
      CHECK_EQUAL(1, motorControl.windUpStartCount);
    }

    //*************************************************************************
    TEST(test_hfsm_emergency_stop_from_winding_down)
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
      motorControl.receive(Timeout());
      motorControl.receive(Stop());

      // Now in at speed state.

      CHECK_EQUAL(StateId::Winding_Down, int(motorControl.get_state_id()));
      CHECK_EQUAL(StateId::Winding_Down, int(motorControl.get_state().get_state_id()));

      CHECK_EQUAL(true, motorControl.isLampOn);
      CHECK_EQUAL(0, motorControl.setSpeedCount);
      CHECK_EQUAL(0, motorControl.speed);
      CHECK_EQUAL(1, motorControl.startCount);
      CHECK_EQUAL(1, motorControl.stopCount);
      CHECK_EQUAL(0, motorControl.stoppedCount);
      CHECK_EQUAL(0, motorControl.unknownCount);
      CHECK_EQUAL(1, motorControl.windUpCompleteCount);
      CHECK_EQUAL(1, motorControl.windUpStartCount);

      // Send emergency Stop event.
      motorControl.receive(EStop());

      // Now in Idle state.
      CHECK_EQUAL(StateId::Idle, int(motorControl.get_state_id()));
      CHECK_EQUAL(StateId::Idle, int(motorControl.get_state().get_state_id()));

      CHECK_EQUAL(false, motorControl.isLampOn);
      CHECK_EQUAL(0, motorControl.setSpeedCount);
      CHECK_EQUAL(0, motorControl.speed);
      CHECK_EQUAL(1, motorControl.startCount);
      CHECK_EQUAL(3, motorControl.stopCount);
      CHECK_EQUAL(0, motorControl.stoppedCount);
      CHECK_EQUAL(0, motorControl.unknownCount);
      CHECK_EQUAL(1, motorControl.windUpCompleteCount);
      CHECK_EQUAL(1, motorControl.windUpStartCount);
    }

    //*************************************************************************
    TEST(test_hfsm_recursive_event)
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

      // Now in winding up state.

      CHECK_EQUAL(StateId::Winding_Up, int(motorControl.get_state_id()));
      CHECK_EQUAL(StateId::Winding_Up, int(motorControl.get_state().get_state_id()));

      CHECK_EQUAL(true, motorControl.isLampOn);
      CHECK_EQUAL(0, motorControl.setSpeedCount);
      CHECK_EQUAL(0, motorControl.speed);
      CHECK_EQUAL(1, motorControl.startCount);
      CHECK_EQUAL(0, motorControl.stopCount);
      CHECK_EQUAL(0, motorControl.stoppedCount);
      CHECK_EQUAL(0, motorControl.unknownCount);
    }

    //*************************************************************************
    TEST(test_hfsm_supported)
    {
      CHECK(motorControl.accepts(EventId::Set_Speed));
      CHECK(motorControl.accepts(EventId::Start));
      CHECK(motorControl.accepts(EventId::Stop));
      CHECK(motorControl.accepts(EventId::Stopped));
      CHECK(motorControl.accepts(EventId::Unsupported));

      CHECK(motorControl.accepts(SetSpeed(0)));
      CHECK(motorControl.accepts(Start()));
      CHECK(motorControl.accepts(Stop()));
      CHECK(motorControl.accepts(Stopped()));
      CHECK(motorControl.accepts(Unsupported()));
    }

    //*************************************************************************
    TEST(test_hfsm_no_states)
    {
      MotorControl mc;

      // No states.
      etl::ifsm_state** stateList = nullptr;

      CHECK_THROW(mc.set_states(stateList, 0U), etl::fsm_state_list_exception);
    }

    //*************************************************************************
    TEST(test_hfsm_null_state)
    {
      MotorControl mc;

      // Null state.
      etl::ifsm_state* stateList[StateId::Number_Of_States] =
      {
        &idle, &running, &windingUp, &windingDown, nullptr
      };

      CHECK_THROW(mc.set_states(stateList, StateId::Number_Of_States), etl::fsm_null_state_exception);
    }

    //*************************************************************************
    TEST(test_hfsm_incorrect_state_order)
    {
      MotorControl mc;

      // Incorrect order.
      etl::ifsm_state* stateList[StateId::Number_Of_States] =
      {
        &idle, &running, &windingDown, &windingUp, &atSpeed
      };

      CHECK_THROW(mc.set_states(stateList, StateId::Number_Of_States), etl::fsm_state_list_order_exception);
    }

    //*************************************************************************
    TEST(test_hfsm_start_enters)
    {
      MotorControl mc;

      motorControl.Initialise(stateList, ETL_OR_STD17::size(stateList));
      motorControl.reset();
      motorControl.ClearStatistics();

      CHECK(!motorControl.is_started());

      // Start the HFSM (with enters)
      motorControl.start(true);
      CHECK(motorControl.is_started());

      // Now in Idle state.

      CHECK_EQUAL(StateId::Idle, int(motorControl.get_state_id()));
      CHECK_EQUAL(StateId::Idle, int(motorControl.get_state().get_state_id()));

      CHECK_EQUAL(false, motorControl.isLampOn);
      CHECK_EQUAL(0, motorControl.setSpeedCount);
      CHECK_EQUAL(0, motorControl.speed);
      CHECK_EQUAL(0, motorControl.startCount);
      CHECK_EQUAL(0, motorControl.stopCount);
      CHECK_EQUAL(0, motorControl.windUpCompleteCount);
      CHECK_EQUAL(0, motorControl.windUpStartCount);
      CHECK_EQUAL(0, motorControl.stoppedCount);
      CHECK_EQUAL(0, motorControl.unknownCount);

      etl::array<StateId::enum_type, 1UL> expectedEnters{StateId::Idle};
      CHECK_EQUAL(expectedEnters.size(), motorControl.stateEnterHistory.size());
      // No enters should have been performed
      CHECK(motorControl.stateExitHistory.empty());

      bool entersCorrect = std::equal(motorControl.stateEnterHistory.begin(), motorControl.stateEnterHistory.end(), expectedEnters.begin());
      CHECK(entersCorrect);
    }

    //*************************************************************************
    TEST(test_hfsm_reset_exits)
    {
      MotorControl mc;

      motorControl.Initialise(stateList, ETL_OR_STD17::size(stateList));
      motorControl.reset();
      motorControl.ClearStatistics();

      CHECK(!motorControl.is_started());

      // Start the HFSM (with enters)
      motorControl.start(false);
      CHECK(motorControl.is_started());

      // Now in Idle state.

      // Send Start event.
      motorControl.receive(Start());

      // Send Timeout event
      motorControl.receive(Timeout());

      // Send SetSpeed event.
      motorControl.receive(SetSpeed(100));
      CHECK_EQUAL(StateId::At_Speed, int(motorControl.get_state_id()));
      CHECK_EQUAL(StateId::At_Speed, int(motorControl.get_state().get_state_id()));

      // Clean statistics before reset for clean results
      motorControl.ClearStatistics();

      // Reset HFSM (with exits)
      motorControl.reset(true);

      etl::array<StateId::enum_type, 2UL> expectedExits{StateId::At_Speed, StateId::Running};
      CHECK_EQUAL(expectedExits.size(), motorControl.stateExitHistory.size());
      // No enters should have been performed
      CHECK(motorControl.stateEnterHistory.empty());

      bool exitsCorrect = std::equal(motorControl.stateExitHistory.begin(), motorControl.stateExitHistory.end(), expectedExits.begin());
      CHECK(exitsCorrect);
    }

    //*************************************************************************
    TEST(test_fsm_no_states_and_no_start)
    {
      MotorControl mc;

      CHECK_THROW(mc.receive(Start()), etl::fsm_not_started);
    }
  };
}
