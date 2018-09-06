/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl
https://www.etlcpp.com

Copyright(c) 2018 jwellbelove

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

#include <stdint->h>

#include "etl/nullptr.h"

namespace etl
{
  //***************************************************************************
  /// Simple State Machine
  //***************************************************************************
  template <typename TObject>
  class sfsm
  {
  public:

    typedef uint32_t state_id_t;
    typedef uint32_t event_id_t;

    //*************************************************************************
    /// Transition definition
    //*************************************************************************
    struct transition
    {
      transition(const event_id_t event_id_,
                 const state_id_t current_state_id_,
                 const state_id_t next_state_id_,
                 void (TObject::* const action_)() = nullptr,
                 bool (TObject::* const guard_)()  = nullptr)
        : event_id(event_id_),
          current_state(current_state_),
          nextState(next_state_),
          action(action_),
          guard(guard_)
      {
      }

      const event_id_t event_id;
      const state_id_t current_state_id;
      const state_id_t next_state_id;
      void (TObject::* const action)();
      bool (TObject::* const guard)();
    };

    //*************************************************************************
    /// State definition
    //*************************************************************************
    struct state
    {
      state(const state_id_t state_id_,
            void (TObject::* const on_enter_)() = nullptr,
            void (TObject::* const on_exit_)() = nullptr)
        : state_id(state_id_),
          on_enter_(on_enter_),
          on_exit(on_exit_)
      {
      }

      state_id_t state_id;
      void (TObject::* const on_enter)();
      void (TObject::* const on_exit)();
    };

    //*************************************************************************
    ///
    //*************************************************************************
    template <const uint32_t TRANSITION_TABLE_SIZE, const uint32_t STATE_TABLE_SIZE>
    sfsm(TObject& object_, 
                  const transition(&transition_table_)[TRANSITION_TABLE_SIZE], 
                  const enter_exit(&state_table_)[STATE_TABLE_SIZE], 
                  const state_id_t state_id_)
      : object(object_),        
        current_state_id(state_id_),
        transition_table(&transition_table_[0]),
        transition_table_size(TRANSITION_TABLE_SIZE)
        state_table(&state_table_[0]),
        state_table_size(ENTER_EXIT_TABLE_SIZE)
    {
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
    void process_event(const event_id_t event_id)
    {
      // Scan the transition table.
      transition* transition_table_begin = transition_table;
      transition* transition_table_end   = transition_table + transition_table_size;

      transition* t = std::find_if(transition_table_begin,
                                   transition_table_end,
                                   is_transition(event_id, current_state_id));

      // Found an entry?
      if (t != transition_table_end)
      {
        // Shall we execute the transition?
        if ((t->guard == nullptr) || ((object->*t->guard)()))
        {
          // Are we moving to a new state?
          const bool to_new_state = (current_state_id != t->next_state_id);

          state* state_table_begin = state_table;
          state* state_table_end   = state_table + state_table_size;

          // See if we have a state entry.
          state* s = std::find_if(state_table_begin,
                                  state_table_end,
                                  is_state(current_state_id));

          // If the current state has an 'on_exit' and we are changing states, then call it.
          if (to_new_state && (s != state_table_end) && (s->on_exit != nullptr))
          {
            (object->*(s->on_exit))();
          }

          // Shall we execute the action?
          if (t->action != nullptr)
          {
            (object->*t->action)();
          }

          // See if we have a state entry.
          s = std::find_if(state_table_begin,
                           state_table_end,
                           is_state(t->next_state_id));

          // If the new state has an 'on_entry' and we are changing states, then call it.
          if (to_new_state && (s != state_table_end) && (s->on_exit != nullptr))
          {
            (object->*(s->on_entry))();
          }

          current_state_id = t->next_state_id;
        }
      }
    }

  private:

    state_id_t        current_state_id;      ///< The current state id.
    TObject&          object;                ///< The object that supplies codition and action member functions
    const transition* transition_table;      ///< The table of transitions.
    const uint32_t    transition_table_size; ///< The size of the transition table.
    const state*      state_table;           ///< The table of transitions.
    const uint32_t    state_table_size;      ///< The size of the transition table.
  };
}

#endif