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

  //***************************************************************************
  // Data
  struct Data
  {
    Data()
      : i(0)
    {
    } 

    explicit Data(int i_)
      : i(i_)
    {
    }

    Data(Data&& data)
      : i(std::move(data.i))
    {
    }

    Data& operator = (Data&& data)
    {
      i = std::move(data.i);
      return *this;
    }

    Data(const Data&) = delete;
    Data& operator = (const Data&) = delete;

    int i;
  };

  //***********************************
  // The motor control FSM.
  //***********************************
  class MotorControl : public etl::state_chart<MotorControl, Data&&>
  {
  public:

    MotorControl()
      : state_chart<MotorControl, Data&&>(*this, transitionTable.begin(), transitionTable.end(), stateTable.begin(), stateTable.end(), StateId::IDLE)
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
      data          = Data(0);
    }

    //***********************************
    void OnStart(parameter_t d)
    {
      data = std::move(d);
      ++startCount;
    }

    //***********************************
    void OnStop(parameter_t d)
    {
      data = std::move(d);
      ++stopCount;
    }

    //***********************************
    void OnStopped(parameter_t d)
    {
      data = std::move(d);
      ++stoppedCount;
    }

    //***********************************
    void OnSetSpeed(parameter_t d)
    {
      data = std::move(d);
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
    void Null(parameter_t d)
    {
      data = std::move(d);
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
    Data data;

    bool guard;

    static const etl::array<MotorControl::transition, 7> transitionTable;
    static const etl::array<MotorControl::state, 3>      stateTable;
  };

  //***************************************************************************
  constexpr etl::array<MotorControl::transition, 7> MotorControl::transitionTable =
  {
    MotorControl::transition(StateId::IDLE,         EventId::START,          StateId::RUNNING,      &MotorControl::OnStart, &MotorControl::Guard),
    MotorControl::transition(StateId::IDLE,         EventId::START,          StateId::IDLE,         &MotorControl::Null,    &MotorControl::NotGuard),
    MotorControl::transition(StateId::RUNNING,      EventId::STOP,           StateId::WINDING_DOWN, &MotorControl::OnStop),
    MotorControl::transition(StateId::RUNNING,      EventId::EMERGENCY_STOP, StateId::IDLE,         &MotorControl::OnStop),
    MotorControl::transition(StateId::RUNNING,      EventId::SET_SPEED,      StateId::RUNNING,      &MotorControl::OnSetSpeed),
    MotorControl::transition(StateId::WINDING_DOWN, EventId::STOPPED,        StateId::IDLE,         &MotorControl::OnStopped),
    MotorControl::transition(                       EventId::ABORT,          StateId::IDLE)
  };

  //***************************************************************************
  constexpr etl::array<MotorControl::state, 3> MotorControl::stateTable =
  {
    MotorControl::state(StateId::IDLE,         &MotorControl::OnEnterIdle,        nullptr),
    MotorControl::state(StateId::RUNNING,      &MotorControl::OnEnterRunning,     nullptr),
    MotorControl::state(StateId::WINDING_DOWN, &MotorControl::OnEnterWindingDown, &MotorControl::OnExitWindingDown)
  };

  MotorControl motorControl;

  SUITE(test_state_chart_class)
  {
    //*************************************************************************
    TEST(test_state_chart_with_rvalue_data_parameter)
    {    
      motorControl.ClearStatistics();

      // In Idle state.
      CHECK_EQUAL(StateId::IDLE, int(motorControl.get_state_id()));

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
      motorControl.process_event(EventId::START, Data(1));

      CHECK_EQUAL(StateId::IDLE, int(motorControl.get_state_id()));

      CHECK_EQUAL(0, motorControl.data.i);
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
      motorControl.start();

      CHECK_EQUAL(true, motorControl.entered_idle);

      // Send unhandled events.
      motorControl.process_event(EventId::STOP, Data(2));
      motorControl.process_event(EventId::STOPPED, Data(3));

      CHECK_EQUAL(StateId::IDLE, int(motorControl.get_state_id()));

      CHECK_EQUAL(0, motorControl.data.i);
      CHECK_EQUAL(false, motorControl.isLampOn);
      CHECK_EQUAL(0, motorControl.setSpeedCount);
      CHECK_EQUAL(0, motorControl.speed);
      CHECK_EQUAL(0, motorControl.startCount);
      CHECK_EQUAL(0, motorControl.stopCount);
      CHECK_EQUAL(0, motorControl.stoppedCount);
      CHECK_EQUAL(0, motorControl.windingDown);

      // Send Start event.
      motorControl.guard = false;
      motorControl.process_event(EventId::START, Data(4));

      // Still in Idle state.

      CHECK_EQUAL(StateId::IDLE, int(motorControl.get_state_id()));

      CHECK_EQUAL(4, motorControl.data.i);
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
      motorControl.process_event(EventId::START, Data(5));

      // Now in Running state.

      CHECK_EQUAL(StateId::RUNNING, int(motorControl.get_state_id()));

      CHECK_EQUAL(5, motorControl.data.i);
      CHECK_EQUAL(true, motorControl.isLampOn);
      CHECK_EQUAL(0, motorControl.setSpeedCount);
      CHECK_EQUAL(0, motorControl.speed);
      CHECK_EQUAL(1, motorControl.startCount);
      CHECK_EQUAL(0, motorControl.stopCount);
      CHECK_EQUAL(0, motorControl.stoppedCount);
      CHECK_EQUAL(0, motorControl.windingDown);
      CHECK_EQUAL(1, motorControl.null);

      // Send unhandled events.
      motorControl.process_event(EventId::START, Data(6));
      motorControl.process_event(EventId::STOPPED, Data(7));

      CHECK_EQUAL(StateId::RUNNING, int(motorControl.get_state_id()));

      CHECK_EQUAL(5, motorControl.data.i);
      CHECK_EQUAL(true, motorControl.isLampOn);
      CHECK_EQUAL(0, motorControl.setSpeedCount);
      CHECK_EQUAL(0, motorControl.speed);
      CHECK_EQUAL(1, motorControl.startCount);
      CHECK_EQUAL(0, motorControl.stopCount);
      CHECK_EQUAL(0, motorControl.stoppedCount);
      CHECK_EQUAL(0, motorControl.windingDown);
      CHECK_EQUAL(1, motorControl.null);

      // Send SetSpeed event.
      motorControl.process_event(EventId::SET_SPEED, Data(8));

      // Still in Running state.

      CHECK_EQUAL(StateId::RUNNING, int(motorControl.get_state_id()));

      CHECK_EQUAL(8, motorControl.data.i);
      CHECK_EQUAL(true, motorControl.isLampOn);
      CHECK_EQUAL(1, motorControl.setSpeedCount);
      CHECK_EQUAL(100, motorControl.speed);
      CHECK_EQUAL(1, motorControl.startCount);
      CHECK_EQUAL(0, motorControl.stopCount);
      CHECK_EQUAL(0, motorControl.stoppedCount);
      CHECK_EQUAL(0, motorControl.windingDown);
      CHECK_EQUAL(1, motorControl.null);

      // Send Stop event.
      motorControl.process_event(EventId::STOP, Data(9));

      // Now in WindingDown state.

      CHECK_EQUAL(StateId::WINDING_DOWN, int(motorControl.get_state_id()));

      CHECK_EQUAL(9, motorControl.data.i);
      CHECK_EQUAL(true, motorControl.isLampOn);
      CHECK_EQUAL(1, motorControl.setSpeedCount);
      CHECK_EQUAL(100, motorControl.speed);
      CHECK_EQUAL(1, motorControl.startCount);
      CHECK_EQUAL(1, motorControl.stopCount);
      CHECK_EQUAL(0, motorControl.stoppedCount);
      CHECK_EQUAL(1, motorControl.windingDown);
      CHECK_EQUAL(1, motorControl.null);

      // Send unhandled events.
      motorControl.process_event(EventId::START, Data(10));
      motorControl.process_event(EventId::STOP, Data(11));

      CHECK_EQUAL(StateId::WINDING_DOWN, int(motorControl.get_state_id()));

      CHECK_EQUAL(9, motorControl.data.i);
      CHECK_EQUAL(true, motorControl.isLampOn);
      CHECK_EQUAL(1, motorControl.setSpeedCount);
      CHECK_EQUAL(100, motorControl.speed);
      CHECK_EQUAL(1, motorControl.startCount);
      CHECK_EQUAL(1, motorControl.stopCount);
      CHECK_EQUAL(0, motorControl.stoppedCount);
      CHECK_EQUAL(1, motorControl.windingDown);
      CHECK_EQUAL(1, motorControl.null);

      // Send Stopped event.
      motorControl.process_event(EventId::STOPPED, Data(12));

      // Now in Idle state.
      CHECK_EQUAL(StateId::IDLE, int(motorControl.get_state_id()));

      CHECK_EQUAL(12, motorControl.data.i);
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
    TEST(test_fsm_emergency_stop)
    {
      motorControl.ClearStatistics();

      // Now in Idle state.

      // Send Start event.
      motorControl.process_event(EventId::START, Data(1));

      // Now in Running state.

      CHECK_EQUAL(StateId::RUNNING, int(motorControl.get_state_id()));

      CHECK_EQUAL(1, motorControl.data.i);
      CHECK_EQUAL(true, motorControl.isLampOn);
      CHECK_EQUAL(0, motorControl.setSpeedCount);
      CHECK_EQUAL(0, motorControl.speed);
      CHECK_EQUAL(1, motorControl.startCount);
      CHECK_EQUAL(0, motorControl.stopCount);
      CHECK_EQUAL(0, motorControl.stoppedCount);
      CHECK_EQUAL(0, motorControl.windingDown);

      // Send emergency Stop event.
      motorControl.process_event(EventId::EMERGENCY_STOP, Data(2));

      // Now in Idle state.
      CHECK_EQUAL(StateId::IDLE, int(motorControl.get_state_id()));

      CHECK_EQUAL(2, motorControl.data.i);
      CHECK_EQUAL(false, motorControl.isLampOn);
      CHECK_EQUAL(0, motorControl.setSpeedCount);
      CHECK_EQUAL(0, motorControl.speed);
      CHECK_EQUAL(1, motorControl.startCount);
      CHECK_EQUAL(1, motorControl.stopCount);
      CHECK_EQUAL(0, motorControl.stoppedCount);
      CHECK_EQUAL(0, motorControl.windingDown);
    }

    //*************************************************************************
    TEST(test_fsm_abort)
    {
      motorControl.ClearStatistics();

      // Now in Idle state.

      // Send Start event.
      motorControl.process_event(EventId::START, Data(1));

      // Now in Running state.

      // Send abort event.
      motorControl.process_event(EventId::ABORT, Data(2));
      CHECK_EQUAL(StateId::IDLE, int(motorControl.get_state_id()));

      // Send Start event.
      motorControl.process_event(EventId::START, Data(3));

      // Now in Running state.

      // Send Stop event.
      motorControl.process_event(EventId::STOP, Data(4));

      // Now in WindingDown state.
      motorControl.process_event(EventId::ABORT, Data(5));
      CHECK_EQUAL(StateId::IDLE, int(motorControl.get_state_id()));
    }
  };
}
