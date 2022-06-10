/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl
https://www.etlcpp.com

Copyright(c) 2018 John Wellbelove

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

#include "etl/state_chart.h"
#include "etl/enum_type.h"
#include "etl/queue.h"
#include "etl/array.h"

#include <iterator>
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
      EMERGENCY_STOP,
      STOPPED,
      SET_SPEED,
      ABORT
    };

    ETL_DECLARE_ENUM_TYPE(EventId, etl::state_chart_traits::event_id_t)
    ETL_ENUM_TYPE(START,          "Start")
    ETL_ENUM_TYPE(STOP,           "Stop")
    ETL_ENUM_TYPE(EMERGENCY_STOP, "Emergency Stop")
    ETL_ENUM_TYPE(STOPPED,        "Stopped")
    ETL_ENUM_TYPE(SET_SPEED,      "Set Speed")
    ETL_ENUM_TYPE(ABORT,          "Abort")
    ETL_END_ENUM_TYPE
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

    ETL_DECLARE_ENUM_TYPE(StateId, etl::state_chart_traits::state_id_t)
    ETL_ENUM_TYPE(IDLE,         "Idle")
    ETL_ENUM_TYPE(RUNNING,      "Running")
    ETL_ENUM_TYPE(WINDING_DOWN, "Winding Down")
    ETL_END_ENUM_TYPE
  };

  //***********************************
  // The motor control FSM.
  //***********************************
  class MotorControl
  {
  public:

    MotorControl()
    {
      ClearStatistics();
    }

    //***********************************
    void ClearStatistics()
    {
      startCount    = 0;
      stopCount     = 0;
      setSpeedCount = 0;
      stoppedCount  = 0;
      isLampOn      = false;
      speed         = 0;
      windingDown   = 0;
      entered_idle  = false;
      null          = 0;
    }

    //***********************************
    void OnStart()
    {
      ++startCount;
    }

    //***********************************
    void OnStop()
    {
      ++stopCount;
    }

    //***********************************
    void OnStopped()
    {
      ++stoppedCount;
    }

    //***********************************
    void OnSetSpeed()
    {
      ++setSpeedCount;
      SetSpeedValue(100);
    }

    //***********************************
    void OnEnterIdle()
    {
      TurnRunningLampOff();
      entered_idle = true;
    }

    //***********************************
    void OnEnterRunning()
    {
      TurnRunningLampOn();
    }

    //***********************************
    void OnEnterWindingDown()
    {
      ++windingDown;
    }

    //***********************************
    void OnExitWindingDown()
    {
      --windingDown;
    }

    //***********************************
    void SetSpeedValue(int speed_)
    {
      speed = speed_;
    }

    //***********************************
    bool Guard()
    {
      return guard;
    }

    //***********************************
    bool NotGuard()
    {
      return !guard;
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
    void Null()
    {
      ++null;
    }

    int startCount;
    int stopCount;
    int setSpeedCount;
    int stoppedCount;
    bool isLampOn;
    int speed;
    int windingDown;
    bool entered_idle;
    int null;

    bool guard;
  };

  //***************************************************************************
  using transition = etl::state_chart_traits::transition<MotorControl>;

  constexpr transition transitionTable[7] =
  {
    transition(StateId::IDLE,         EventId::START,          StateId::RUNNING,      &MotorControl::OnStart, &MotorControl::Guard),
    transition(StateId::IDLE,         EventId::START,          StateId::IDLE,         &MotorControl::Null,    &MotorControl::NotGuard),
    transition(StateId::RUNNING,      EventId::STOP,           StateId::WINDING_DOWN, &MotorControl::OnStop),
    transition(StateId::RUNNING,      EventId::EMERGENCY_STOP, StateId::IDLE,         &MotorControl::OnStop),
    transition(StateId::RUNNING,      EventId::SET_SPEED,      StateId::RUNNING,      &MotorControl::OnSetSpeed),
    transition(StateId::WINDING_DOWN, EventId::STOPPED,        StateId::IDLE,         &MotorControl::OnStopped),
    transition(                       EventId::ABORT,          StateId::IDLE)
  };

  //***************************************************************************
  using state = etl::state_chart_traits::state<MotorControl>;

  constexpr state stateTable[3] =
  {
    state(StateId::IDLE,         &MotorControl::OnEnterIdle,        nullptr),
    state(StateId::RUNNING,      &MotorControl::OnEnterRunning,     nullptr),
    state(StateId::WINDING_DOWN, &MotorControl::OnEnterWindingDown, &MotorControl::OnExitWindingDown)
  };

  MotorControl motorControl;

  etl::state_chart_ct<MotorControl, 
                      motorControl, 
                      transitionTable,
                      7,
                      stateTable,
                      3,
                      StateId::IDLE> motorControlStateChart;

  SUITE(test_state_chart_compile_time)
  {
    //*************************************************************************
    TEST(test_state_chart)
    {
      motorControl.ClearStatistics();

      // In Idle state.
      CHECK_EQUAL(StateId::IDLE, int(motorControlStateChart.get_state_id()));

      CHECK_EQUAL(false, motorControl.isLampOn);
      CHECK_EQUAL(0, motorControl.setSpeedCount);
      CHECK_EQUAL(0, motorControl.speed);
      CHECK_EQUAL(0, motorControl.startCount);
      CHECK_EQUAL(0, motorControl.stopCount);
      CHECK_EQUAL(0, motorControl.stoppedCount);
      CHECK_EQUAL(0, motorControl.windingDown);
      CHECK_EQUAL(false, motorControl.entered_idle);

      // Send Start event (state chart not started).
      motorControl.guard = true;
      motorControlStateChart.process_event(EventId::START);

      CHECK_EQUAL(StateId::IDLE, int(motorControlStateChart.get_state_id()));

      CHECK_EQUAL(false, motorControl.isLampOn);
      CHECK_EQUAL(0, motorControl.setSpeedCount);
      CHECK_EQUAL(0, motorControl.speed);
      CHECK_EQUAL(0, motorControl.startCount);
      CHECK_EQUAL(0, motorControl.stopCount);
      CHECK_EQUAL(0, motorControl.stoppedCount);
      CHECK_EQUAL(0, motorControl.windingDown);
      CHECK_EQUAL(false, motorControl.entered_idle);

      // Start the state chart
      motorControl.guard = true;
      motorControlStateChart.start();

      CHECK_EQUAL(true, motorControl.entered_idle);

      // Send unhandled events.
      motorControlStateChart.process_event(EventId::STOP);
      motorControlStateChart.process_event(EventId::STOPPED);

      CHECK_EQUAL(StateId::IDLE, int(motorControlStateChart.get_state_id()));

      CHECK_EQUAL(false, motorControl.isLampOn);
      CHECK_EQUAL(0, motorControl.setSpeedCount);
      CHECK_EQUAL(0, motorControl.speed);
      CHECK_EQUAL(0, motorControl.startCount);
      CHECK_EQUAL(0, motorControl.stopCount);
      CHECK_EQUAL(0, motorControl.stoppedCount);
      CHECK_EQUAL(0, motorControl.windingDown);

      // Send Start event.
      motorControl.guard = false;
      motorControlStateChart.process_event(EventId::START);

      // Still in Idle state.

      CHECK_EQUAL(StateId::IDLE, int(motorControlStateChart.get_state_id()));

      CHECK_EQUAL(false, motorControl.isLampOn);
      CHECK_EQUAL(0, motorControl.setSpeedCount);
      CHECK_EQUAL(0, motorControl.speed);
      CHECK_EQUAL(0, motorControl.startCount);
      CHECK_EQUAL(0, motorControl.stopCount);
      CHECK_EQUAL(0, motorControl.stoppedCount);
      CHECK_EQUAL(0, motorControl.windingDown);
      CHECK_EQUAL(1, motorControl.null);

      // Send Start event.
      motorControl.guard = true;
      motorControlStateChart.process_event(EventId::START);

      // Now in Running state.

      CHECK_EQUAL(StateId::RUNNING, int(motorControlStateChart.get_state_id()));

      CHECK_EQUAL(true, motorControl.isLampOn);
      CHECK_EQUAL(0, motorControl.setSpeedCount);
      CHECK_EQUAL(0, motorControl.speed);
      CHECK_EQUAL(1, motorControl.startCount);
      CHECK_EQUAL(0, motorControl.stopCount);
      CHECK_EQUAL(0, motorControl.stoppedCount);
      CHECK_EQUAL(0, motorControl.windingDown);
      CHECK_EQUAL(1, motorControl.null);

      // Send unhandled events.
      motorControlStateChart.process_event(EventId::START);
      motorControlStateChart.process_event(EventId::STOPPED);

      CHECK_EQUAL(StateId::RUNNING, int(motorControlStateChart.get_state_id()));

      CHECK_EQUAL(true, motorControl.isLampOn);
      CHECK_EQUAL(0, motorControl.setSpeedCount);
      CHECK_EQUAL(0, motorControl.speed);
      CHECK_EQUAL(1, motorControl.startCount);
      CHECK_EQUAL(0, motorControl.stopCount);
      CHECK_EQUAL(0, motorControl.stoppedCount);
      CHECK_EQUAL(0, motorControl.windingDown);
      CHECK_EQUAL(1, motorControl.null);

      // Send SetSpeed event.
      motorControlStateChart.process_event(EventId::SET_SPEED);

      // Still in Running state.

      CHECK_EQUAL(StateId::RUNNING, int(motorControlStateChart.get_state_id()));

      CHECK_EQUAL(true, motorControl.isLampOn);
      CHECK_EQUAL(1, motorControl.setSpeedCount);
      CHECK_EQUAL(100, motorControl.speed);
      CHECK_EQUAL(1, motorControl.startCount);
      CHECK_EQUAL(0, motorControl.stopCount);
      CHECK_EQUAL(0, motorControl.stoppedCount);
      CHECK_EQUAL(0, motorControl.windingDown);
      CHECK_EQUAL(1, motorControl.null);

      // Send Stop event.
      motorControlStateChart.process_event(EventId::STOP);

      // Now in WindingDown state.

      CHECK_EQUAL(StateId::WINDING_DOWN, int(motorControlStateChart.get_state_id()));

      CHECK_EQUAL(true, motorControl.isLampOn);
      CHECK_EQUAL(1, motorControl.setSpeedCount);
      CHECK_EQUAL(100, motorControl.speed);
      CHECK_EQUAL(1, motorControl.startCount);
      CHECK_EQUAL(1, motorControl.stopCount);
      CHECK_EQUAL(0, motorControl.stoppedCount);
      CHECK_EQUAL(1, motorControl.windingDown);
      CHECK_EQUAL(1, motorControl.null);

      // Send unhandled events.
      motorControlStateChart.process_event(EventId::START);
      motorControlStateChart.process_event(EventId::STOP);

      CHECK_EQUAL(StateId::WINDING_DOWN, int(motorControlStateChart.get_state_id()));

      CHECK_EQUAL(true, motorControl.isLampOn);
      CHECK_EQUAL(1, motorControl.setSpeedCount);
      CHECK_EQUAL(100, motorControl.speed);
      CHECK_EQUAL(1, motorControl.startCount);
      CHECK_EQUAL(1, motorControl.stopCount);
      CHECK_EQUAL(0, motorControl.stoppedCount);
      CHECK_EQUAL(1, motorControl.windingDown);
      CHECK_EQUAL(1, motorControl.null);

      // Send Stopped event.
      motorControlStateChart.process_event(EventId::STOPPED);

      // Now in Idle state.
      CHECK_EQUAL(StateId::IDLE, int(motorControlStateChart.get_state_id()));

      CHECK_EQUAL(false, motorControl.isLampOn);
      CHECK_EQUAL(1, motorControl.setSpeedCount);
      CHECK_EQUAL(100, motorControl.speed);
      CHECK_EQUAL(1, motorControl.startCount);
      CHECK_EQUAL(1, motorControl.stopCount);
      CHECK_EQUAL(1, motorControl.stoppedCount);
      CHECK_EQUAL(0, motorControl.windingDown);
      CHECK_EQUAL(1, motorControl.null);
    }

    //*************************************************************************
    //TEST(test_state_chart_with_delegate)
    //{
    //  motorControl.ClearStatistics();

    //  auto process_event = motorControlStateChart.get_process_event_delegate();

    //  // Start the state chart
    //  motorControl.guard = true;
    //  motorControlStateChart.start();

    //  // Send unhandled events.
    //  process_event(EventId::STOP);
    //  process_event(EventId::STOPPED);

    //  CHECK_EQUAL(StateId::IDLE, int(motorControlStateChart.get_state_id()));

    //  CHECK_EQUAL(false, motorControl.isLampOn);
    //  CHECK_EQUAL(0, motorControl.setSpeedCount);
    //  CHECK_EQUAL(0, motorControl.speed);
    //  CHECK_EQUAL(0, motorControl.startCount);
    //  CHECK_EQUAL(0, motorControl.stopCount);
    //  CHECK_EQUAL(0, motorControl.stoppedCount);
    //  CHECK_EQUAL(0, motorControl.windingDown);

    //  // Send Start event.
    //  motorControl.guard = false;
    //  process_event(EventId::START);

    //  // Still in Idle state.

    //  CHECK_EQUAL(StateId::IDLE, int(motorControlStateChart.get_state_id()));

    //  CHECK_EQUAL(false, motorControl.isLampOn);
    //  CHECK_EQUAL(0, motorControl.setSpeedCount);
    //  CHECK_EQUAL(0, motorControl.speed);
    //  CHECK_EQUAL(0, motorControl.startCount);
    //  CHECK_EQUAL(0, motorControl.stopCount);
    //  CHECK_EQUAL(0, motorControl.stoppedCount);
    //  CHECK_EQUAL(0, motorControl.windingDown);
    //  CHECK_EQUAL(1, motorControl.null);

    //  // Send Start event.
    //  motorControl.guard = true;
    //  process_event(EventId::START);

    //  // Now in Running state.

    //  CHECK_EQUAL(StateId::RUNNING, int(motorControlStateChart.get_state_id()));

    //  CHECK_EQUAL(true, motorControl.isLampOn);
    //  CHECK_EQUAL(0, motorControl.setSpeedCount);
    //  CHECK_EQUAL(0, motorControl.speed);
    //  CHECK_EQUAL(1, motorControl.startCount);
    //  CHECK_EQUAL(0, motorControl.stopCount);
    //  CHECK_EQUAL(0, motorControl.stoppedCount);
    //  CHECK_EQUAL(0, motorControl.windingDown);
    //  CHECK_EQUAL(1, motorControl.null);

    //  // Send unhandled events.
    //  process_event(EventId::START);
    //  process_event(EventId::STOPPED);

    //  CHECK_EQUAL(StateId::RUNNING, int(motorControlStateChart.get_state_id()));

    //  CHECK_EQUAL(true, motorControl.isLampOn);
    //  CHECK_EQUAL(0, motorControl.setSpeedCount);
    //  CHECK_EQUAL(0, motorControl.speed);
    //  CHECK_EQUAL(1, motorControl.startCount);
    //  CHECK_EQUAL(0, motorControl.stopCount);
    //  CHECK_EQUAL(0, motorControl.stoppedCount);
    //  CHECK_EQUAL(0, motorControl.windingDown);
    //  CHECK_EQUAL(1, motorControl.null);

    //  // Send SetSpeed event.
    //  process_event(EventId::SET_SPEED);

    //  // Still in Running state.

    //  CHECK_EQUAL(StateId::RUNNING, int(motorControlStateChart.get_state_id()));

    //  CHECK_EQUAL(true, motorControl.isLampOn);
    //  CHECK_EQUAL(1, motorControl.setSpeedCount);
    //  CHECK_EQUAL(100, motorControl.speed);
    //  CHECK_EQUAL(1, motorControl.startCount);
    //  CHECK_EQUAL(0, motorControl.stopCount);
    //  CHECK_EQUAL(0, motorControl.stoppedCount);
    //  CHECK_EQUAL(0, motorControl.windingDown);
    //  CHECK_EQUAL(1, motorControl.null);

    //  // Send Stop event.
    //  process_event(EventId::STOP);

    //  // Now in WindingDown state.

    //  CHECK_EQUAL(StateId::WINDING_DOWN, int(motorControlStateChart.get_state_id()));

    //  CHECK_EQUAL(true, motorControl.isLampOn);
    //  CHECK_EQUAL(1, motorControl.setSpeedCount);
    //  CHECK_EQUAL(100, motorControl.speed);
    //  CHECK_EQUAL(1, motorControl.startCount);
    //  CHECK_EQUAL(1, motorControl.stopCount);
    //  CHECK_EQUAL(0, motorControl.stoppedCount);
    //  CHECK_EQUAL(1, motorControl.windingDown);
    //  CHECK_EQUAL(1, motorControl.null);

    //  // Send unhandled events.
    //  process_event(EventId::START);
    //  process_event(EventId::STOP);

    //  CHECK_EQUAL(StateId::WINDING_DOWN, int(motorControlStateChart.get_state_id()));

    //  CHECK_EQUAL(true, motorControl.isLampOn);
    //  CHECK_EQUAL(1, motorControl.setSpeedCount);
    //  CHECK_EQUAL(100, motorControl.speed);
    //  CHECK_EQUAL(1, motorControl.startCount);
    //  CHECK_EQUAL(1, motorControl.stopCount);
    //  CHECK_EQUAL(0, motorControl.stoppedCount);
    //  CHECK_EQUAL(1, motorControl.windingDown);
    //  CHECK_EQUAL(1, motorControl.null);

    //  // Send Stopped event.
    //  process_event(EventId::STOPPED);

    //  // Now in Idle state.
    //  CHECK_EQUAL(StateId::IDLE, int(motorControlStateChart.get_state_id()));

    //  CHECK_EQUAL(false, motorControl.isLampOn);
    //  CHECK_EQUAL(1, motorControl.setSpeedCount);
    //  CHECK_EQUAL(100, motorControl.speed);
    //  CHECK_EQUAL(1, motorControl.startCount);
    //  CHECK_EQUAL(1, motorControl.stopCount);
    //  CHECK_EQUAL(1, motorControl.stoppedCount);
    //  CHECK_EQUAL(0, motorControl.windingDown);
    //  CHECK_EQUAL(1, motorControl.null);
    //}

    ////*************************************************************************
    //TEST(test_fsm_emergency_stop)
    //{
    //  motorControl.ClearStatistics();

    //  // Now in Idle state.

    //  // Send Start event.
    //  motorControlStateChart.process_event(EventId::START);

    //  // Now in Running state.

    //  CHECK_EQUAL(StateId::RUNNING, int(motorControlStateChart.get_state_id()));

    //  CHECK_EQUAL(true, motorControl.isLampOn);
    //  CHECK_EQUAL(0, motorControl.setSpeedCount);
    //  CHECK_EQUAL(0, motorControl.speed);
    //  CHECK_EQUAL(1, motorControl.startCount);
    //  CHECK_EQUAL(0, motorControl.stopCount);
    //  CHECK_EQUAL(0, motorControl.stoppedCount);
    //  CHECK_EQUAL(0, motorControl.windingDown);

    //  // Send emergency Stop event.
    //  motorControlStateChart.process_event(EventId::EMERGENCY_STOP);

    //  // Now in Idle state.
    //  CHECK_EQUAL(StateId::IDLE, int(motorControlStateChart.get_state_id()));

    //  CHECK_EQUAL(false, motorControl.isLampOn);
    //  CHECK_EQUAL(0, motorControl.setSpeedCount);
    //  CHECK_EQUAL(0, motorControl.speed);
    //  CHECK_EQUAL(1, motorControl.startCount);
    //  CHECK_EQUAL(1, motorControl.stopCount);
    //  CHECK_EQUAL(0, motorControl.stoppedCount);
    //  CHECK_EQUAL(0, motorControl.windingDown);
    //}

    //*************************************************************************
    TEST(test_fsm_abort)
    {
      motorControl.ClearStatistics();

      // Now in Idle state.

      // Send Start event.
      motorControlStateChart.process_event(EventId::START);

      // Now in Running state.

      // Send abort event.
      motorControlStateChart.process_event(EventId::ABORT);
      CHECK_EQUAL(StateId::IDLE, int(motorControlStateChart.get_state_id()));

      // Send Start event.
      motorControlStateChart.process_event(EventId::START);

      // Now in Running state.

      // Send Stop event.
      motorControlStateChart.process_event(EventId::STOP);

      // Now in WindingDown state.
      motorControlStateChart.process_event(EventId::ABORT);
      CHECK_EQUAL(StateId::IDLE, int(motorControlStateChart.get_state_id()));
    }
  };
}
