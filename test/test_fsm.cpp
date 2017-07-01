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
  //***************************************************************************
  // Events
  struct EventId
  {
    enum enum_type
    {
      START,
      STOP,
      STOPPED,
      SET_SPEED
    };

    ETL_DECLARE_ENUM_TYPE(EventId, etl::fsm_event_id_t)
    ETL_ENUM_TYPE(START,     "Start")
    ETL_ENUM_TYPE(STOP,      "Stop")
    ETL_ENUM_TYPE(STOPPED,   "Stopped")
    ETL_ENUM_TYPE(SET_SPEED, "Set Speed")
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

    SetSpeed(int speed) : speed(speed) {}

    const int speed;
  };

  //***********************************
  class Stopped : public etl::message<EventId::STOPPED>
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
      NUMBER_OF_STATES
    };

    ETL_DECLARE_ENUM_TYPE(StateId, etl::fsm_state_id_t)
    ETL_ENUM_TYPE(IDLE,         "Idle")
    ETL_ENUM_TYPE(RUNNING,      "Running")
    ETL_ENUM_TYPE(WINDING_DOWN, "Winding Down")
    ETL_END_ENUM_TYPE
  };

  class MotorControl;

  //***********************************
  // Common functionality
  //***********************************
  class Common
  {
  public:

    //***********************************
    Common()
    {
      ClearStatistics();
    }

    //***********************************
    void ClearStatistics()
    {
      startCount    = 0;
      stopCount     = 0;
      setSpeedCount = 0;
      unknownCount  = 0;
      stoppedCount  = 0;
      isLampOn      = false;
      speed         = 0;
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
  class Idle : public etl::fsm_state<Idle, StateId::IDLE, Start>
  {
  public:

    //***********************************
    Idle(Common& common)
      : common(common)
    {
    }

    //***********************************
    etl::fsm_state_id_t on_event(etl::imessage_router& sender, const Start& event)
    {
      ++common.startCount;
      return StateId::RUNNING;
    }

    //***********************************
    etl::fsm_state_id_t on_event_unknown(etl::imessage_router& sender, const etl::imessage& event)
    {
      ++common.unknownCount;
      return STATE_ID;
    }

    //***********************************
    void on_enter_state()
    {
      common.TurnRunningLampOff();
    }

    Common& common;
  };

  //***********************************
  // The running state.
  //***********************************
  class Running : public etl::fsm_state<Running, StateId::RUNNING, Stop, SetSpeed>
  {
  public:

    //***********************************
    Running(Common& common)
      : common(common)
    {
    }

    //***********************************
    etl::fsm_state_id_t on_event(etl::imessage_router& sender, const Stop& event)
    {
      ++common.stopCount;

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
    etl::fsm_state_id_t on_event(etl::imessage_router& sender, const SetSpeed& event)
    {
      ++common.setSpeedCount;
      common.SetSpeed(event.speed);
      return STATE_ID;
    }

    //***********************************
    etl::fsm_state_id_t on_event_unknown(etl::imessage_router& sender, const etl::imessage& event)
    {
      ++common.unknownCount;
      return STATE_ID;
    }

    void on_enter_state()
    {
      common.TurnRunningLampOn();
    }

    Common& common;
  };

  //***********************************
  // The winding down state.
  //***********************************
  class WindingDown : public etl::fsm_state<WindingDown, StateId::WINDING_DOWN, Stopped>
  {
  public:

    //***********************************
    WindingDown(Common& common)
      : common(common)
    {
    }

    //***********************************
    etl::fsm_state_id_t on_event(etl::imessage_router& source, const Stopped& event)
    {
      ++common.stoppedCount;
      return StateId::IDLE;
    }

    //***********************************
    etl::fsm_state_id_t on_event_unknown(etl::imessage_router& source, const etl::imessage& event)
    {
      ++common.unknownCount;
      return STATE_ID;
    }

    Common& common;
  };

  //***********************************
  // The motor control FSM.
  //***********************************
  class MotorControl : public etl::fsm
  {
  public:

    MotorControl()
      : idle(common),
        running(common),
        windingDown(common)
    {
      set_states(stateList, etl::size(stateList));
    }

    Common common;

  private:

    // The states.
    Idle        idle;
    Running     running;
    WindingDown windingDown;

    etl::ifsm_state* stateList[StateId::NUMBER_OF_STATES] =
    {
      &idle, &running, &windingDown
    };
  };

  MotorControl motorControl;

  SUITE(test_map)
  {
    //*************************************************************************
    TEST(test_fsm)
    {
      motorControl.reset();
      motorControl.common.ClearStatistics();

      CHECK(!motorControl.is_started());

      // Start the FSM.
      motorControl.start();
      CHECK(motorControl.is_started());

      // Now in Idle state.

      CHECK_EQUAL(StateId::IDLE, int(motorControl.get_state_id()));
      CHECK_EQUAL(StateId::IDLE, int(motorControl.get_state().get_state_id()));

      CHECK_EQUAL(false, motorControl.common.isLampOn);
      CHECK_EQUAL(0, motorControl.common.setSpeedCount);
      CHECK_EQUAL(0, motorControl.common.speed);
      CHECK_EQUAL(0, motorControl.common.startCount);
      CHECK_EQUAL(0, motorControl.common.stopCount);
      CHECK_EQUAL(0, motorControl.common.stoppedCount);
      CHECK_EQUAL(0, motorControl.common.unknownCount);

      // Send unhandled events.
      motorControl.receive(etl::null_message_router(), Stop());
      motorControl.receive(etl::null_message_router(), Stopped());
      motorControl.receive(etl::null_message_router(), SetSpeed(10));

      CHECK_EQUAL(StateId::IDLE, motorControl.get_state_id());
      CHECK_EQUAL(StateId::IDLE, motorControl.get_state().get_state_id());

      CHECK_EQUAL(false, motorControl.common.isLampOn);
      CHECK_EQUAL(0, motorControl.common.setSpeedCount);
      CHECK_EQUAL(0, motorControl.common.speed);
      CHECK_EQUAL(0, motorControl.common.startCount);
      CHECK_EQUAL(0, motorControl.common.stopCount);
      CHECK_EQUAL(0, motorControl.common.stoppedCount);
      CHECK_EQUAL(3, motorControl.common.unknownCount);

      // Send Start event.
      motorControl.receive(etl::null_message_router(), Start());

      // Now in Running state.

      CHECK_EQUAL(StateId::RUNNING, int(motorControl.get_state_id()));
      CHECK_EQUAL(StateId::RUNNING, int(motorControl.get_state().get_state_id()));

      CHECK_EQUAL(true, motorControl.common.isLampOn);
      CHECK_EQUAL(0, motorControl.common.setSpeedCount);
      CHECK_EQUAL(0, motorControl.common.speed);
      CHECK_EQUAL(1, motorControl.common.startCount);
      CHECK_EQUAL(0, motorControl.common.stopCount);
      CHECK_EQUAL(0, motorControl.common.stoppedCount);
      CHECK_EQUAL(3, motorControl.common.unknownCount);

      // Send unhandled events.
      motorControl.receive(etl::null_message_router(), Start());
      motorControl.receive(etl::null_message_router(), Stopped());

      CHECK_EQUAL(StateId::RUNNING, int(motorControl.get_state_id()));
      CHECK_EQUAL(StateId::RUNNING, int(motorControl.get_state().get_state_id()));

      CHECK_EQUAL(true, motorControl.common.isLampOn);
      CHECK_EQUAL(0, motorControl.common.setSpeedCount);
      CHECK_EQUAL(0, motorControl.common.speed);
      CHECK_EQUAL(1, motorControl.common.startCount);
      CHECK_EQUAL(0, motorControl.common.stopCount);
      CHECK_EQUAL(0, motorControl.common.stoppedCount);
      CHECK_EQUAL(5, motorControl.common.unknownCount);

      // Send SetSpeed event.
      motorControl.receive(etl::null_message_router(), SetSpeed(100));

      // Still in Running state.

      CHECK_EQUAL(StateId::RUNNING, int(motorControl.get_state_id()));
      CHECK_EQUAL(StateId::RUNNING, int(motorControl.get_state().get_state_id()));

      CHECK_EQUAL(true, motorControl.common.isLampOn);
      CHECK_EQUAL(1, motorControl.common.setSpeedCount);
      CHECK_EQUAL(100, motorControl.common.speed);
      CHECK_EQUAL(1, motorControl.common.startCount);
      CHECK_EQUAL(0, motorControl.common.stopCount);
      CHECK_EQUAL(0, motorControl.common.stoppedCount);
      CHECK_EQUAL(5, motorControl.common.unknownCount);

      // Send Stop event.
      motorControl.receive(etl::null_message_router(), Stop());

      // Now in WindingDown state.

      CHECK_EQUAL(StateId::WINDING_DOWN, int(motorControl.get_state_id()));
      CHECK_EQUAL(StateId::WINDING_DOWN, int(motorControl.get_state().get_state_id()));

      CHECK_EQUAL(true, motorControl.common.isLampOn);
      CHECK_EQUAL(1, motorControl.common.setSpeedCount);
      CHECK_EQUAL(100, motorControl.common.speed);
      CHECK_EQUAL(1, motorControl.common.startCount);
      CHECK_EQUAL(1, motorControl.common.stopCount);
      CHECK_EQUAL(0, motorControl.common.stoppedCount);
      CHECK_EQUAL(5, motorControl.common.unknownCount);

      // Send unhandled events.
      motorControl.receive(etl::null_message_router(), Start());
      motorControl.receive(etl::null_message_router(), Stop());
      motorControl.receive(etl::null_message_router(), SetSpeed(100));

      CHECK_EQUAL(StateId::WINDING_DOWN, int(motorControl.get_state_id()));
      CHECK_EQUAL(StateId::WINDING_DOWN, int(motorControl.get_state().get_state_id()));

      CHECK_EQUAL(true, motorControl.common.isLampOn);
      CHECK_EQUAL(1, motorControl.common.setSpeedCount);
      CHECK_EQUAL(100, motorControl.common.speed);
      CHECK_EQUAL(1, motorControl.common.startCount);
      CHECK_EQUAL(1, motorControl.common.stopCount);
      CHECK_EQUAL(0, motorControl.common.stoppedCount);
      CHECK_EQUAL(8, motorControl.common.unknownCount);

      // Send Stopped event.
      motorControl.receive(etl::null_message_router(), Stopped());

      // Now in Idle state.

      CHECK_EQUAL(StateId::IDLE, int(motorControl.get_state_id()));
      CHECK_EQUAL(StateId::IDLE, int(motorControl.get_state().get_state_id()));

      CHECK_EQUAL(false, motorControl.common.isLampOn);
      CHECK_EQUAL(1, motorControl.common.setSpeedCount);
      CHECK_EQUAL(100, motorControl.common.speed);
      CHECK_EQUAL(1, motorControl.common.startCount);
      CHECK_EQUAL(1, motorControl.common.stopCount);
      CHECK_EQUAL(1, motorControl.common.stoppedCount);
      CHECK_EQUAL(8, motorControl.common.unknownCount);
    }

    //*************************************************************************
    TEST(test_fsm_emergency_stop)
    {
      motorControl.reset();
      motorControl.common.ClearStatistics();

      CHECK(!motorControl.is_started());

      // Start the FSM.
      motorControl.start();
      CHECK(motorControl.is_started());

      // Now in Idle state.

      // Send Start event.
      motorControl.receive(etl::null_message_router(), Start());

      // Now in Running state.

      CHECK_EQUAL(StateId::RUNNING, int(motorControl.get_state_id()));
      CHECK_EQUAL(StateId::RUNNING, int(motorControl.get_state().get_state_id()));

      CHECK_EQUAL(true, motorControl.common.isLampOn);
      CHECK_EQUAL(0, motorControl.common.setSpeedCount);
      CHECK_EQUAL(0, motorControl.common.speed);
      CHECK_EQUAL(1, motorControl.common.startCount);
      CHECK_EQUAL(0, motorControl.common.stopCount);
      CHECK_EQUAL(0, motorControl.common.stoppedCount);
      CHECK_EQUAL(0, motorControl.common.unknownCount);

      // Send emergency Stop event.
      motorControl.receive(etl::null_message_router(), Stop(true));

      // Now in Idle state.

      CHECK_EQUAL(StateId::IDLE, int(motorControl.get_state_id()));
      CHECK_EQUAL(StateId::IDLE, int(motorControl.get_state().get_state_id()));

      CHECK_EQUAL(false, motorControl.common.isLampOn);
      CHECK_EQUAL(0, motorControl.common.setSpeedCount);
      CHECK_EQUAL(0, motorControl.common.speed);
      CHECK_EQUAL(1, motorControl.common.startCount);
      CHECK_EQUAL(1, motorControl.common.stopCount);
      CHECK_EQUAL(0, motorControl.common.stoppedCount);
      CHECK_EQUAL(0, motorControl.common.unknownCount);
    }
  };
}
