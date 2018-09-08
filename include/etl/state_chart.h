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

#ifndef ETL_STATE_CHART_INCLUDED
#define ETL_STATE_CHART_INCLUDED

#include <stdint.h>

#include "etl/platform.h"
#include "etl/nullptr.h"
#include "etl/array.h"
#include "etl/array_view.h"

namespace etl
{
  //***************************************************************************
  /// Simple Finite State Machine Interface
  //***************************************************************************
  class istate_chart
  {
  public:

    typedef uint32_t state_id_t;
    typedef uint32_t event_id_t;

    virtual state_id_t get_state_id() const = 0;
    virtual void process_event(const event_id_t event_id) = 0;

    virtual ~istate_chart()
    {
    }
  };

  //***************************************************************************
  /// Simple Finite State Machine
  //***************************************************************************
  template <typename TObject>
  class state_chart : public istate_chart
  {
  public:

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
        current_state_id(current_state_id_),
          next_state_id(next_state_id_),
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
            void (TObject::* const on_entry_)() = nullptr,
            void (TObject::* const on_exit_)()  = nullptr)
        : state_id(state_id_),
          on_entry(on_entry_),
          on_exit(on_exit_)
      {
      }

      state_id_t state_id;
      void (TObject::* const on_entry)();
      void (TObject::* const on_exit)();
    };

    //*************************************************************************
    /// Constructor.
    /// \tparam TRANSITION_TABLE_SIZE The transition table size.
    /// \param object_           A reference to the implementation object.
    /// \param transition_table_ The table of transitions.
    /// \param state_id_         The initial state id.
    //*************************************************************************
    template <const uint32_t TRANSITION_TABLE_SIZE>
    state_chart(TObject& object_,
                const etl::array<transition, TRANSITION_TABLE_SIZE>& transition_table_,
                const state_id_t state_id_)
      : object(object_),
        current_state_id(state_id_),
        transition_table(transition_table_.begin(), transition_table_.end())
    {
    }

    //*************************************************************************
    /// Sets the state table.
    /// \tparam STATE_TABLE_SIZE The state table size.
    /// \param state_table_ A reference to the state table.
    //*************************************************************************
    template <const uint32_t STATE_TABLE_SIZE>
    void set_state_table(const etl::array<state, STATE_TABLE_SIZE>& state_table_)
    {
      state_table.assign(state_table_.begin(), state_table_.end());
    }

    //*************************************************************************
    /// Gets a reference to the implementation object.
    /// \return Reference to the implementation object.
    //*************************************************************************
    TObject& get_object()
    {
      return object;
    }

    //*************************************************************************
    /// Gets a const reference to the implementation object.
    /// \return Const reference to the implementation object.
    //*************************************************************************
    const TObject& get_object() const
    {
      return object;
    }

    //*************************************************************************
    /// Gets the current state id.
    /// \return The current state id.
    //*************************************************************************
    state_id_t get_state_id() const
    {
      return current_state_id;
    }

    //*************************************************************************
    /// Gets the current state id.
    /// \return The current state id.
    //*************************************************************************
    const state* find_state(state_id_t state_id)
    {
      if (state_table.empty())
      {
        return state_table.end();
      }
      else
      {
        return std::find_if(state_table.begin(),
                            state_table.end(),
                            is_state(state_id));
      }
    }

    //*************************************************************************
    /// Processes the specified event.
    /// The state machine will action the <b>first</b> item in the transition table
    /// that satisfies the conditions for executing the action.
    /// \param event_id The id of the event to process.
    //*************************************************************************
    void process_event(const event_id_t event_id)
    {
      // Scan the transition table.
      const transition* t = std::find_if(transition_table.begin(),
                                         transition_table.end(),
                                         is_transition(event_id, current_state_id));

      // Found an entry?
      if (t != transition_table.end())
      {
        // Shall we execute the transition?
        if ((t->guard == nullptr) || ((object.*t->guard)()))
        {
          const bool to_new_state = (current_state_id != t->next_state_id);

          // Execute the state exit if necessary.
          if (to_new_state)
          {
            // See if we have a state item for the current state.
            const state* s = find_state(current_state_id);

            // If the current state has an 'on_exit' then call it.
            if ((s != state_table.end()) && (s->on_exit != nullptr))
            {
              (object.*(s->on_exit))();
            }
          }

          // Shall we execute the action?
          if (t->action != nullptr)
          {
            (object.*t->action)();
          }

          // Execute the state entry if necessary.
          if (to_new_state)
          {
            // See if we have a state item for the next state.
            const state* s = find_state(t->next_state_id);

            // If the new state has an 'on_entry' then call it.
            if ((s != state_table.end()) && (s->on_entry != nullptr))
            {
              (object.*(s->on_entry))();
            }
          }

          current_state_id = t->next_state_id;
        }
      }
    }

  private:

    //*************************************************************************
    struct is_transition
    {
      is_transition(event_id_t event_id_, state_id_t state_id_)
        : event_id(event_id_),
          state_id(state_id_)
      {
      }

      bool operator()(const transition& t) const
      {
        return (t.event_id == event_id) && (t.current_state_id == state_id);
      }

      const event_id_t event_id;
      const state_id_t state_id;
    };

    //*************************************************************************
    struct is_state
    {
      is_state(state_id_t state_id_)
        : state_id(state_id_)
      {
      }

      bool operator()(const state& s) const
      {
        return (s.state_id == state_id);
      }

      const state_id_t state_id;
    };

    // Disabled
    state_chart(const state_chart&) ETL_DELETE;
    state_chart& operator =(const state_chart&) ETL_DELETE;

    TObject&                                object;           ///< The object that supplies guard and action member functions.
    state_id_t                              current_state_id; ///< The current state id.
    const etl::array_view<const transition> transition_table; ///< The table of transitions.
    etl::array_view<const state>            state_table;      ///< The table of states.
  };
}

#endif
