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

#include <iostream>

namespace
{
  struct HFsmId
  {
    enum
    {
      State_Machine
    };
  };

  //***********************************
  class Message : public etl::message<0>
  {
  };

  //***************************************************************************
  // States
  struct StateId
  {
    enum enum_type
    {
      State1,
      State1_1,
      State1_1_1,
      State1_1_2,
      State1_2,
      Number_Of_States
    };

    ETL_DECLARE_ENUM_TYPE(StateId, etl::fsm_state_id_t)
    ETL_ENUM_TYPE(State1,     "State1")
    ETL_ENUM_TYPE(State1_1,   "State1_1")
    ETL_ENUM_TYPE(State1_1_1, "State1_1_1 Up")
    ETL_ENUM_TYPE(State1_1_2, "State1_1_2")
    ETL_ENUM_TYPE(State1_2,   "State1_2")
    ETL_END_ENUM_TYPE
  };

  //***********************************
  // The state machine.
  //***********************************
  class StateMachine : public etl::hfsm
  {
  public:

    StateMachine()
      : hfsm(HFsmId::State_Machine)
    {
    }

    //***********************************
    void Initialise(etl::ifsm_state** p_states, size_t size)
    {
      set_states(p_states, size);
    }
  };

  //***********************************
  // State1
  //***********************************
  class State1 : public etl::fsm_state<StateMachine, State1, StateId::State1, Message>
  {
  public:

    //***********************************
    etl::fsm_state_id_t on_event(const Message&)
    {
      return etl::ifsm_state::No_State_Change;
    }

    //***********************************
    etl::fsm_state_id_t on_event_unknown(const etl::imessage&)
    {
      return No_State_Change;
    }
  };

  //***********************************
  // State1_1
  //***********************************
  class State1_1 : public etl::fsm_state<StateMachine, State1_1, StateId::State1_1, Message>
  {
  public:

    //***********************************
    etl::fsm_state_id_t on_event(const Message&)
    {
      return etl::ifsm_state::No_State_Change;
    }

    //***********************************
    etl::fsm_state_id_t on_event_unknown(const etl::imessage&)
    {
      return No_State_Change;
    }
  };

  //***********************************
  // State1_1_1
  //***********************************
  class State1_1_1 : public etl::fsm_state<StateMachine, State1_1_1, StateId::State1_1_1, Message>
  {
  public:

    //***********************************
    etl::fsm_state_id_t on_event(const Message&)
    {
      return etl::ifsm_state::No_State_Change;
    }

    //***********************************
    etl::fsm_state_id_t on_event_unknown(const etl::imessage&)
    {
      return No_State_Change;
    }
  };

  //***********************************
  // State1_1_2
  //***********************************
  class State1_1_2 : public etl::fsm_state<StateMachine, State1_1_2, StateId::State1_1_2, Message>
  {
  public:

    //***********************************
    etl::fsm_state_id_t on_event(const Message&)
    {
      return etl::ifsm_state::No_State_Change;
    }

    //***********************************
    etl::fsm_state_id_t on_event_unknown(const etl::imessage&)
    {
      return No_State_Change;
    }
  };

  //***********************************
  // State1_2
  //***********************************
  class State1_2 : public etl::fsm_state<StateMachine, State1_2, StateId::State1_2, Message>
  {
  public:

    //***********************************
    etl::fsm_state_id_t on_event(const Message&)
    {
      return etl::ifsm_state::No_State_Change;
    }

    //***********************************
    etl::fsm_state_id_t on_event_unknown(const etl::imessage&)
    {
      return No_State_Change;
    }
  };

  SUITE(test_hfsm_recurse_states)
  {
    //*************************************************************************
    TEST(test_hfsm_no_enter_state_on_start)
    {
      StateMachine stateMachine;

      State1     state1;
      State1_1   state1_1;
      State1_1_1 state1_1_1;
      State1_1_2 state1_1_2;
      State1_2   state1_2;

      etl::ifsm_state* stateList[StateId::Number_Of_States] =
      {
        &state1, &state1_1, &state1_1_1, &state1_1_2, &state1_2
      };

      // Set up the state list.
      stateMachine.set_states(stateList, StateId::Number_Of_States);

      // Start the FSM.
      stateMachine.start(false);

      CHECK_EQUAL(StateId::State1, stateMachine.get_state_id());
    }

    //*************************************************************************
    TEST(test_hfsm_enter_state_on_start)
    {
      StateMachine stateMachine;

      State1     state1;
      State1_1   state1_1;
      State1_1_1 state1_1_1;
      State1_1_2 state1_1_2;
      State1_2   state1_2;

      etl::ifsm_state* stateList[StateId::Number_Of_States] =
      {
        &state1, &state1_1, &state1_1_1, &state1_1_2, &state1_2
      };

      etl::ifsm_state* childStates1[] =
      {
        &state1_1, &state1_2
      };

      etl::ifsm_state* childStates1_1[] =
      {
        &state1_1_1, &state1_1_2
      };

      // Set up the state list.
      stateMachine.set_states(stateList, StateId::Number_Of_States);

      // Set up the child states.
      state1.set_child_states(childStates1, ETL_OR_STD17::size(childStates1));

      state1_1.set_child_states(childStates1_1, ETL_OR_STD17::size(childStates1_1));

      // Start the FSM.
      stateMachine.start(true);

      CHECK_EQUAL(StateId::State1_1_1, stateMachine.get_state_id());
    }
  };
}
