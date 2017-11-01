/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl
https://www.etlcpp.com

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

#include "fsm.h"
#include "enum_type.h"
#include "container.h"

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
      UNSUPPORTED
    };

    ETL_DECLARE_ENUM_TYPE(EventId, etl::message_id_t)
    ETL_ENUM_TYPE(START,       "Start")
    ETL_ENUM_TYPE(STOP,        "Stop")
    ETL_ENUM_TYPE(STOPPED,     "Stopped")
    ETL_ENUM_TYPE(SET_SPEED,   "Set Speed")
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

    MotorControl(etl::ifsm_state** p_states, size_t size)
      : fsm(MOTOR_CONTROL)
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
    void SetSpeed(int speed_)
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
  class Idle : public etl::fsm_state<MotorControl, Idle, StateId::IDLE, Start>
  {
  public:

    //***********************************
    etl::fsm_state_id_t on_event(etl::imessage_router&, const Start&)
    {
      ++get_fsm_context().startCount;
      return StateId::RUNNING;
    }

    //***********************************
    etl::fsm_state_id_t on_event_unknown(etl::imessage_router&, const etl::imessage&)
    {
      ++get_fsm_context().unknownCount;
      return STATE_ID;
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
    etl::fsm_state_id_t on_event(etl::imessage_router&, const Stop& event)
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
    etl::fsm_state_id_t on_event(etl::imessage_router&, const SetSpeed& event)
    {
      ++get_fsm_context().setSpeedCount;
      get_fsm_context().SetSpeed(event.speed);
      return STATE_ID;
    }

    //***********************************
    etl::fsm_state_id_t on_event_unknown(etl::imessage_router&, const etl::imessage&)
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
    etl::fsm_state_id_t on_event(etl::imessage_router&, const Stopped&)
    {
      ++get_fsm_context().stoppedCount;
      return StateId::IDLE;
    }

    //***********************************
    etl::fsm_state_id_t on_event_unknown(etl::imessage_router&, const etl::imessage&)
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
    etl::fsm_state_id_t on_event_unknown(etl::imessage_router&, const etl::imessage&)
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

  MotorControl motorControl(stateList, etl::size(stateList));

  SUITE(test_map)
  {
    //*************************************************************************
    TEST(test_fsm)
    {
      etl::null_message_router nmr;

      motorControl.reset();
      motorControl.ClearStatistics();

      CHECK(!motorControl.is_started());

      // Start the FSM.
      motorControl.start();
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
      motorControl.receive(nmr, Stop());
      motorControl.receive(nmr, Stopped());
      motorControl.receive(nmr, SetSpeed(10));

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
      motorControl.receive(nmr, Start());

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
      motorControl.receive(nmr, Start());
      motorControl.receive(nmr, Stopped());

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
      motorControl.receive(nmr, SetSpeed(100));

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
      motorControl.receive(nmr, Stop());

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
      motorControl.receive(nmr, Start());
      motorControl.receive(nmr, Stop());
      motorControl.receive(nmr, SetSpeed(100));

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
      motorControl.receive(nmr, Stopped());

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

      motorControl.reset();
      motorControl.ClearStatistics();

      CHECK(!motorControl.is_started());

      // Start the FSM.
      motorControl.start();
      CHECK(motorControl.is_started());

      // Now in Idle state.

      // Send Start event.
      motorControl.receive(nmr, Start());

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
      motorControl.receive(nmr, Stop(true));

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
  };
}
