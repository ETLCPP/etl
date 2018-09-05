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

#ifndef ETL_LWFSM_INCLUDED
#define ETL_LWFSM_INCLUDED

#include <stdint.h>

#include "etl/nullptr.h"

namespace etl
{
  //***************************************************************************
  /// Light Weight Finite State Machine
  //***************************************************************************
  template <typename TObject>
  class state_machine
  {
  public:

    typedef uint32_t state_id_t;
    typedef uint32_t event_id_t;

    //*************************************************************************
    /// Transition definition
    //*************************************************************************
    struct transition
    {
      transition(event_id_t event_,
                 state_id_t current_state_,
                 state_id_t next_state_,
                 void (TObject::*action_)(),
                 bool (TObject::*condition_)()  = nullptr)
        : event(event_),
          current_state(current_state_),
          nextState(next_state_),
          action(action_),
          condition(condition_)
      {
      }

      event_id_t event;
      state_id_t current_state;
      state_id_t nextState;
      void (TObject::*action)();
      bool (TObject::*condition)();
    };

    //*************************************************************************
    ///
    //*************************************************************************
    state_machine(TObject& object_)
      : object(object_)
    {
    }

    //*************************************************************************
    ///
    //*************************************************************************
    state_machine(TObject& object_, state_id_t state_)
      : object(object_),
        current_state(state_)
    {
    }

    //*************************************************************************
    ///
    //*************************************************************************
    template <const uint32_t TRANSITION_TABLE_SIZE>
    void set_transition_table(transition<TObject>(&transition_table_)[TRANSITION_TABLE_SIZE])
    {
      transition_table      = &transition_table_[0];
      transition_table_size = TRANSITION_TABLE_SIZE;
    }

    //*************************************************************************
    ///
    //*************************************************************************
    void set_initial_state(state_id_t state)
    {
      current_state = state;
    }

    //*************************************************************************
    ///
    //*************************************************************************
    state_id_t get_state() const
    {
      return current_state;
    }

    //*************************************************************************
    ///
    //*************************************************************************
    void process_event(event_id_t event)
    {
      bool handled = false;

      // Scan the transition table.
      for (uint32_t i = 0; (i < transition_table_size) && (handled == false); ++i)
      {
        transition<TObject>& transition = transition_table[i];

        // Matching event and state?
        if ((transition.event == event) && (transition.current_state == current_state))
        {
          // Shall we execute the transition?
          bool do_transition = (transition.condition == nullptr) || ((object.*transition.condition)());

          if (do_transition)
          {
            (object.*transition.action)();
            current_state = transition.next_state;
            handled = true;
          }
        }
      }
    }

  private:

    state_id_t           current_state;         ///< The current state id.
    TObject&             object;                ///< The object that supplies codition and action member functions
    transition<TObject>* transition_table;      ///< The table of transitions.
    uint32_t             transition_table_size; ///< The size of the transition table.
  };
}

#endif