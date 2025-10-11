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
#include "etl/string.h"
#include <array>

//                             entry && mode=RxEventDeviation
//                 ┌─────────────────────────────────────────────────────┐
//                 │                                                     │
// *      ┌────────▼─────────────┐      ToS6Event                        │
// │      │                      │     ┌────────────────────┐            │
// │      │   s3                 │     │                    │            │
// │      │  ┌─────────────────┐ │     │           ┌────────┼────────────┴──────────────┐
// │      │  │ s2*             │ │     │           │  s4    │                           │
// │      │  │ ┌────┐   ┌────┐ │ │     │           │   ┌────┴─────────────────────────┐ │
// │      │  │ │ s1*│   │ s6 ◄─┼─┼─────┘           │   │ s5*                          │ │
// │      │  │ │    │   │    │ │ │ entry && mode=  │   ├──────────────────────────────┤ │
// │      │  │ │    │   │    │ │ │  StartDeviation │   │ToS6Event && RxEventDuringTr./│ │
// └──────┼──► │    │   │    │ ├─┼─────────────────┼───►    receive(ToS5Event)**      │ │
//        │  │ │    │   │    │ │ │                 │   └─▲────────────────────────────┘ │
//        │  │ └┬───┘   └────┘ │ │                 │     │                              │
//        │  │  │              │ │                 └─────┼──▲───────────────────────────┘
//        │  └──┼──────────────┘ │                       │  │
//        │     │                │                       │  │
//        └─────┼─┬──────────────┘                       │  │
//              │ │                  ToS5Event           │  │
//              │ └──────────────────────────────────────┘  │
//              │                                           │
//              │  entry && mode=StartDeviationDefaultChild │
//              └───────────────────────────────────────────┘
//
//                    * indicates default state
//                   ** Tests an exception for recursive receives
// Diagram created with asciiflow.com

namespace
{
  class StateList;

  enum class Events : uint8_t
  {
    ToS5,
    ToS6
  };

  class ToS5Event : public etl::message<static_cast<uint8_t>(Events::ToS5)>
  {
  };

  class ToS6Event : public etl::message<static_cast<uint8_t>(Events::ToS6)>
  {
  };

  class EntryTestSM : public etl::hfsm
  {
  public:
    EntryTestSM();

    etl::string<30> test_data;
    enum RunMode
    {
      Normal,
      StartDeviation,
      RxEventDeviation,
      StartDeviationDefaultChild,
      RxEventDuringTransition
    };
    RunMode runMode = Normal;

    enum hfsmStates : uint8_t
    {
      S2,
      S3,
      S1,
      S4,
      S5,
      S6,
      Number_Of_States
    };

  private:
    std::unique_ptr<StateList> m_state_list;
  };

  class S1 : public etl::fsm_state<EntryTestSM, S1, EntryTestSM::S1>
  {
  public:
    etl::fsm_state_id_t on_enter_state() override
    {
      get_fsm_context().test_data += "E1";
      return (get_fsm_context().runMode == EntryTestSM::StartDeviationDefaultChild)
               ? static_cast<uint8_t>(EntryTestSM::S4) : No_State_Change;
    }

    static etl::fsm_state_id_t on_event_unknown(const etl::imessage&)
    {
      return No_State_Change;
    }

    void on_exit_state() override
    {
      get_fsm_context().test_data += "X1";
    }
  };

  class S2 : public etl::fsm_state<EntryTestSM, S2, EntryTestSM::S2>
  {
  public:
    etl::fsm_state_id_t on_enter_state() override
    {
      get_fsm_context().test_data += "E2";
      return (get_fsm_context().runMode == EntryTestSM::StartDeviation)
               ? static_cast<uint8_t>(EntryTestSM::S5) : No_State_Change;
    }

    static etl::fsm_state_id_t on_event_unknown(const etl::imessage&)
    {
      return No_State_Change;
    }

    void on_exit_state() override
    {
      get_fsm_context().test_data += "X2";
    }
  };

  class S3 : public etl::fsm_state<EntryTestSM, S3, EntryTestSM::S3, ToS5Event>
  {
  public:
    etl::fsm_state_id_t on_enter_state() override
    {
      if(get_fsm_context().runMode == EntryTestSM::RxEventDuringTransition)
      {
        get_fsm_context().receive(ToS5Event{});
      }
      get_fsm_context().test_data += "E3";
      return No_State_Change;
    }

    static etl::fsm_state_id_t on_event(ToS5Event const& /*event*/)
    {
      return EntryTestSM::S5;
    }

    static etl::fsm_state_id_t on_event_unknown(const etl::imessage&)
    {
      return No_State_Change;
    }

    void on_exit_state() override
    {
      get_fsm_context().test_data += "X3";
    }
  };

  class S4 : public etl::fsm_state<EntryTestSM, S4, EntryTestSM::S4>
  {
  public:
    etl::fsm_state_id_t on_enter_state() override
    {
      get_fsm_context().test_data += "E4";
      return (get_fsm_context().runMode == EntryTestSM::RxEventDeviation)
               ? static_cast<uint8_t>(EntryTestSM::S3) : No_State_Change;
    }

    static etl::fsm_state_id_t on_event_unknown(const etl::imessage&)
    {
      return No_State_Change;
    }

    void on_exit_state() override
    {
      get_fsm_context().test_data += "X4";
    }
  };

  class S5 : public etl::fsm_state<EntryTestSM, S5, EntryTestSM::S5, ToS6Event>
  {
  public:
    etl::fsm_state_id_t on_enter_state() override
    {
      get_fsm_context().test_data += "E5";
      return No_State_Change;
    }

    etl::fsm_state_id_t on_event(ToS6Event const& /*event*/)
    {
      if (get_fsm_context().runMode == EntryTestSM::RxEventDuringTransition)
      {
        get_fsm_context().receive(ToS5Event{});
      }
      return EntryTestSM::S6;
    }

    static etl::fsm_state_id_t on_event_unknown(const etl::imessage&)
    {
      return No_State_Change;
    }

    void on_exit_state() override
    {
      get_fsm_context().test_data += "X5";
    }
  };

  class S6 : public etl::fsm_state<EntryTestSM, S6, EntryTestSM::S6>
  {
  public:
    etl::fsm_state_id_t on_enter_state() override
    {
      get_fsm_context().test_data += "E6";
      return No_State_Change;
    }

    static etl::fsm_state_id_t on_event_unknown(const etl::imessage&)
    {
      return No_State_Change;
    }

    void on_exit_state() override
    {
      get_fsm_context().test_data += "X6";
    }
  };

  class StateList
  {
  public:
    StateList()
    {
      m_s3.add_child_state(m_s2);
      m_s2.add_child_state(m_s1);
      m_s2.add_child_state(m_s6);

      m_s4.add_child_state(m_s5);
    }

    std::array<etl::ifsm_state*, static_cast<uint8_t>(EntryTestSM::Number_Of_States)> stateList = {
      &m_s2, &m_s3, &m_s1, &m_s4, &m_s5, &m_s6};

  private:
    // The states.
    S1 m_s1;
    S2 m_s2;
    S3 m_s3;
    S4 m_s4;
    S5 m_s5;
    S6 m_s6;
  };

  EntryTestSM::EntryTestSM()
    : hfsm(0), m_state_list(new StateList{})
  {
    set_states(m_state_list->stateList.data(), m_state_list->stateList.size());
  }

  SUITE(test_hfsm_transition_on_enter)
  {
    //*************************************************************************
    TEST(start_regular)
    {
      EntryTestSM sm;
      sm.start(true);
      CHECK_EQUAL("E3E2E1", sm.test_data.c_str());
    }

    //*************************************************************************
    TEST(start_deviation)
    {
      // Checks default child wanting to go to another state
      EntryTestSM sm;
      sm.runMode = EntryTestSM::StartDeviation;
      sm.start(true);
      CHECK_EQUAL("E3E2X2X3E4E5", sm.test_data.c_str());
    }

    //*************************************************************************
    TEST(start_deviation_with_default_child_activation)
    {
      // Checks default child wanting to go to another state
      EntryTestSM sm;
      sm.runMode = EntryTestSM::StartDeviationDefaultChild;
      sm.start(true);
      CHECK_EQUAL("E3E2E1X1X2X3E4E5", sm.test_data.c_str());
    }

    //*************************************************************************
    TEST(receive_event_normal)
    {
      EntryTestSM sm;
      sm.start(true);
      sm.test_data.clear();
      sm.receive(ToS5Event{});
      CHECK_EQUAL("X1X2X3E4E5", sm.test_data.c_str());
    }

    //*************************************************************************
    TEST(receive_event_with_deviation)
    {
      // Test utilizes default child state entry
      EntryTestSM sm;
      sm.start(true);
      sm.test_data.clear();
      sm.runMode = EntryTestSM::RxEventDeviation;
      sm.receive(ToS5Event{});
      CHECK_EQUAL("X1X2X3E4X4E3E2E1", sm.test_data.c_str());
    }

    //*************************************************************************
    TEST(go_to_non_default_state_and_back)
    {
      EntryTestSM sm;
      // Skip enters so test data doesn't need to be cleared
      sm.start(false);
      sm.receive(ToS5Event{});
      sm.test_data.clear();
      sm.receive(ToS6Event{});
      CHECK_EQUAL("X5X4E3E2E6", sm.test_data.c_str());
      sm.test_data.clear();
      sm.receive(ToS5Event{});
      CHECK_EQUAL("X6X2X3E4E5", sm.test_data.c_str());
    }

    //*************************************************************************
    TEST(reset_exits)
    {
      EntryTestSM sm;
      sm.start(true);
      sm.test_data.clear();
      sm.reset(true);
      CHECK_EQUAL("X1X2X3", sm.test_data.c_str());
    }
  }

}  // namespace
