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

#include "platform.h"
#include "nullptr.h"
#include "array.h"
#include "array_view.h"
#include "utility.h"
#include "delegate.h"

namespace etl
{
  namespace private_state_chart
  {
    typedef uint_least8_t state_id_t;
    typedef uint_least8_t event_id_t;

    //*************************************************************************
    /// Transition definition
    //*************************************************************************
    template <typename TObject, typename TParameter = void>
    struct transition
    {
      ETL_CONSTEXPR transition(const state_id_t current_state_id_,
                               const event_id_t event_id_,
                               const state_id_t next_state_id_,
                               void (TObject::* const action_)(TParameter) = ETL_NULLPTR,
                               bool (TObject::* const guard_)() = ETL_NULLPTR)
        : current_state_id(current_state_id_)
        , event_id(event_id_)
        , next_state_id(next_state_id_)
        , action(action_)
        , guard(guard_)
        , from_any_state(false)
      {
      }

      ETL_CONSTEXPR transition(const event_id_t event_id_,
                               const state_id_t next_state_id_,
                               void (TObject::* const action_)(TParameter) = ETL_NULLPTR,
                               bool (TObject::* const guard_)() = ETL_NULLPTR)
        : current_state_id(0)
        , event_id(event_id_)
        , next_state_id(next_state_id_)
        , action(action_)
        , guard(guard_)
        , from_any_state(true)
      {
      }

      const state_id_t current_state_id;
      const event_id_t event_id;
      const state_id_t next_state_id;
      void (TObject::* const action)(TParameter);
      bool (TObject::* const guard)();
      const bool       from_any_state;
    };

    //*************************************************************************
    /// Transition definition
    //*************************************************************************
    template <typename TObject>
    struct transition<TObject, void>
    {
      ETL_CONSTEXPR transition(const state_id_t current_state_id_,
                               const event_id_t event_id_,
                               const state_id_t next_state_id_,
                               void (TObject::* const action_)() = ETL_NULLPTR,
                               bool (TObject::* const guard_)() = ETL_NULLPTR)
        : current_state_id(current_state_id_)
        , event_id(event_id_)
        , next_state_id(next_state_id_)
        , action(action_)
        , guard(guard_)
        , from_any_state(false)
      {
      }

      ETL_CONSTEXPR transition(const event_id_t event_id_,
                               const state_id_t next_state_id_,
                               void (TObject::* const action_)() = ETL_NULLPTR,
                               bool (TObject::* const guard_)() = ETL_NULLPTR)
        : current_state_id(0)
        , event_id(event_id_)
        , next_state_id(next_state_id_)
        , action(action_)
        , guard(guard_)
        , from_any_state(true)
      {
      }

      const state_id_t current_state_id;
      const event_id_t event_id;
      const state_id_t next_state_id;
      void (TObject::* const action)();
      bool (TObject::* const guard)();
      const bool       from_any_state;
    };

    //*************************************************************************
    /// State definition
    //*************************************************************************
    template <typename TObject>
    struct state
    {
      ETL_CONSTEXPR state(const state_id_t state_id_,
                          void (TObject::* const on_entry_)() = ETL_NULLPTR,
                          void (TObject::* const on_exit_)() = ETL_NULLPTR)
        : state_id(state_id_)
        , on_entry(on_entry_)
        , on_exit(on_exit_)
      {
      }

      state_id_t state_id;
      void (TObject::* const on_entry)();
      void (TObject::* const on_exit)();
    };
  }

  //*************************************************************************
  /// istate_chart
  //*************************************************************************
  class istate_chart
  {
  public:

    typedef private_state_chart::state_id_t state_id_t;
    typedef private_state_chart::event_id_t event_id_t;

    //*************************************************************************
    /// Gets the current state id.
    /// \return The current state id.
    //*************************************************************************
    state_id_t get_state_id() const
    {
      return current_state_id;
    }

  protected:

    istate_chart(state_id_t current_state_id_)
      : current_state_id(current_state_id_)
    {
    }

    state_id_t current_state_id; ///< The current state id.
  };

  //***************************************************************************
  /// Simple Finite State Machine
  /// Data parameter for events.
  //***************************************************************************
  template <typename TObject, typename TParameter = void>
  class state_chart : public istate_chart
  {
  public:

    typedef TParameter parameter_t;  
    typedef etl::delegate<void(const event_id_t, parameter_t)> delegate_t;
    typedef private_state_chart::transition<TObject, parameter_t> transition;
    typedef private_state_chart::state<TObject> state;

    //*************************************************************************
    /// Constructor.
    /// \param object_                 A reference to the implementation object.
    /// \param transition_table_begin_ The start of the table of transitions.
    /// \param transition_table_end_   The end of the table of transitions.
    /// \param state_id_               The initial state id.
    //*************************************************************************
    ETL_CONSTEXPR state_chart(TObject&                object_,
                              const transition* const transition_table_begin_,
                              const transition* const transition_table_end_,
                              const state_id_t        state_id_)
      : istate_chart(state_id_),
        object(object_),
        transition_table_begin(transition_table_begin_),
        state_table_begin(ETL_NULLPTR),
        transition_table_size(transition_table_end_ - transition_table_begin_),
        state_table_size(0U),
        started(false)
    {
    }

    //*************************************************************************
    /// Constructor.
    /// \param object_                 A reference to the implementation object.
    /// \param transition_table_begin_ The start of the table of transitions.
    /// \param transition_table_end_   The end of the table of transitions.
    /// \param state_table_begin_      The start of the state table.
    /// \param state_table_end_        The end of the state table.
    /// \param state_id_               The initial state id.
    //*************************************************************************
    ETL_CONSTEXPR state_chart(TObject&                 object_,
                              const transition* const  transition_table_begin_,
                              const transition* const  transition_table_end_,
                              const state* const       state_table_begin_,
                              const state* const       state_table_end_,
                              const state_id_t         state_id_)
      : istate_chart(state_id_),
        object(object_),
        transition_table_begin(transition_table_begin_),
        state_table_begin(state_table_begin_),
        transition_table_size(transition_table_end_ - transition_table_begin_),
        state_table_size(state_table_end_ - state_table_begin_),
        started(false)
    {
    }

    //*************************************************************************
    /// Sets the transition table.
    /// \param state_table_begin_ The start of the state table.
    /// \param state_table_end_   The end of the state table.
    //*************************************************************************
    void set_transition_table(const transition* transition_table_begin_,
                              const transition* transition_table_end_)
    {
      transition_table_begin = transition_table_begin_;
      transition_table_size  = transition_table_end_ - transition_table_begin_;
    }

    //*************************************************************************
    /// Sets the state table.
    /// \param state_table_begin_ The start of the state table.
    /// \param state_table_end_   The end of the state table.
    //*************************************************************************
    void set_state_table(const state* state_table_begin_,
                         const state* state_table_end_)
    {
      state_table_begin = state_table_begin_;
      state_table_size  = state_table_end_ - state_table_begin_;
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
    const state* find_state(state_id_t state_id)
    {
      if (state_table_begin == ETL_NULLPTR)
      {
        return state_table_end();
      }
      else
      {
        return etl::find_if(state_table_begin,
                            state_table_end(),
                            is_state(state_id));
      }
    }

    //*************************************************************************
    /// Start the state chart.
    //*************************************************************************
    void start(const bool on_entry_initial = true)
    {
      if (!started)
      {
        if (on_entry_initial)
        {
          // See if we have a state item for the initial state.
          const state* s = find_state(current_state_id);

          // If the initial state has an 'on_entry' then call it.
          if ((s != state_table_end()) && (s->on_entry != ETL_NULLPTR))
          {
            (object.*(s->on_entry))();
          }
        }

        started = true;
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
      process_event(event_id, typename etl::types<parameter_t>::type());
    }

    //*************************************************************************
    /// Processes the specified event.
    /// The state machine will action the <b>first</b> item in the transition table
    /// that satisfies the conditions for executing the action.
    /// \param event_id The id of the event to process.
    /// \param data     The data to pass to the action.
    //*************************************************************************
    void process_event(const event_id_t event_id, parameter_t data)
    {
      if (started)
      {
        const transition* t = transition_table_begin;

        // Keep looping until we execute a transition or reach the end of the table.
        while (t != transition_table_end())
        {
          // Scan the transition table from the latest position.
          t = etl::find_if(t,
                           transition_table_end(),
                           is_transition(event_id, current_state_id));

          // Found an entry?
          if (t != transition_table_end())
          {
            // Shall we execute the transition?
            if ((t->guard == ETL_NULLPTR) || ((object.*t->guard)()))
            {
              // Shall we execute the action?
              if (t->action != ETL_NULLPTR)
              {
#if ETL_CPP11_SUPPORTED
                (object.*t->action)(etl::forward<parameter_t>(data));
#else
                (object.*t->action)(data);
#endif
              }

              // Changing state?
              if (current_state_id != t->next_state_id)
              {
                const state* s;

                // See if we have a state item for the current state.
                s = find_state(current_state_id);

                // If the current state has an 'on_exit' then call it.
                if ((s != state_table_end()) && (s->on_exit != ETL_NULLPTR))
                {
                  (object.*(s->on_exit))();
                }

                current_state_id = t->next_state_id;

                // See if we have a state item for the new state.
                s = find_state(current_state_id);

                // If the new state has an 'on_entry' then call it.
                if ((s != state_table_end()) && (s->on_entry != ETL_NULLPTR))
                {
                  (object.*(s->on_entry))();
                }
              }

              t = transition_table_end();
            }
            else
            {
              // Start the search from the next item in the table.
              ++t;
            }
          }
        }
      }
    }

    //*************************************************************************
    /// Get a delegate to the process_event function.
    //*************************************************************************
    delegate_t get_process_event_delegate()
    {
      return delegate_t::template create<state_chart<TObject, TParameter>, &state_chart<TObject, TParameter>::process_event>(*this);
    }

  private:

    //*************************************************************************
    const transition* const transition_table_end() const
    {
      return transition_table_begin + transition_table_size;
    }

    //*************************************************************************
    const state* const state_table_end() const
    {
      return state_table_begin + state_table_size;
    }

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
        return (t.event_id == event_id) && (t.from_any_state || (t.current_state_id == state_id));
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

    TObject&          object;                 ///< The object that supplies guard and action member functions.
    const transition* transition_table_begin; ///< The start of the table of transitions.
    const state*      state_table_begin;      ///< The start of the table of states.
    uint_least8_t     transition_table_size;  ///< The size of the table of transitions.
    uint_least8_t     state_table_size;       ///< The size of the table of states.
    bool              started;                ///< Set if the state chart has been started.
  };

  //***************************************************************************
  /// Simple Finite State Machine
  //***************************************************************************
  template <typename TObject>
  class state_chart<TObject, void> : public istate_chart
  {
  public:

    typedef etl::delegate<void(const event_id_t)> delegate_t;
    typedef private_state_chart::transition<TObject, void> transition;
    typedef private_state_chart::state<TObject> state;

    //*************************************************************************
    /// Constructor.
    /// \param object_                 A reference to the implementation object.
    /// \param transition_table_begin_ The start of the table of transitions.
    /// \param transition_table_end_   The end of the table of transitions.
    /// \param state_id_               The initial state id.
    //*************************************************************************
    ETL_CONSTEXPR state_chart(TObject& object_,
                              const transition* transition_table_begin_,
                              const transition* transition_table_end_,
                              const state_id_t state_id_)
      : istate_chart(state_id_),
        object(object_),
        transition_table_begin(transition_table_begin_),
        state_table_begin(ETL_NULLPTR),
        transition_table_size(transition_table_end_ - transition_table_begin_),
        state_table_size(0U),
        started(false)
    {
    }

    //*************************************************************************
    /// Constructor.
    /// \param object_                 A reference to the implementation object.
    /// \param transition_table_begin_ The start of the table of transitions.
    /// \param transition_table_end_   The end of the table of transitions.
    /// \param state_table_begin_      The start of the state table.
    /// \param state_table_end_        The end of the state table.
    /// \param state_id_               The initial state id.
    //*************************************************************************
    ETL_CONSTEXPR state_chart(TObject& object_,
                              const transition* transition_table_begin_,
                              const transition* transition_table_end_,
                              const state* state_table_begin_,
                              const state* state_table_end_,
                              const state_id_t state_id_)
      : istate_chart(state_id_),
        object(object_),
        transition_table_begin(transition_table_begin_),
        state_table_begin(state_table_begin_),
        transition_table_size(transition_table_end_ - transition_table_begin_),
        state_table_size(state_table_end_ - state_table_begin_),
        started(false)
    {
    }

    //*************************************************************************
    /// Sets the transition table.
    /// \param state_table_begin_ The start of the state table.
    /// \param state_table_end_   The end of the state table.
    //*************************************************************************
    void set_transition_table(const transition* transition_table_begin_,
                              const transition* transition_table_end_)
    {
      transition_table_begin = transition_table_begin_;
      transition_table_size  = transition_table_end_ - transition_table_begin_;
    }

    //*************************************************************************
    /// Sets the state table.
    /// \param state_table_begin_ The start of the state table.
    /// \param state_table_end_   The end of the state table.
    //*************************************************************************
    void set_state_table(const state* state_table_begin_,
                         const state* state_table_end_)
    {
      state_table_begin = state_table_begin_;
      state_table_size  = state_table_end_ - state_table_begin_;
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
    const state* find_state(state_id_t state_id)
    {
      if (state_table_begin == ETL_NULLPTR)
      {
        return state_table_end();
      }
      else
      {
        return etl::find_if(state_table_begin,
                            state_table_end(),
                            is_state(state_id));
      }
    }

    //*************************************************************************
    /// Start the state chart.
    //*************************************************************************
    void start(const bool on_entry_initial = true)
    {
      if (!started)
      {
        if (on_entry_initial)
        {
          // See if we have a state item for the initial state.
          const state* s = find_state(current_state_id);

          // If the initial state has an 'on_entry' then call it.
          if ((s != state_table_end()) && (s->on_entry != ETL_NULLPTR))
          {
            (object.*(s->on_entry))();
          }
        }

        started = true;
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
      if (started)
      {
        const transition* t = transition_table_begin;

        // Keep looping until we execute a transition or reach the end of the table.
        while (t != transition_table_end())
        {
          // Scan the transition table from the latest position.
          t = etl::find_if(t,
                           transition_table_end(),
                           is_transition(event_id, current_state_id));

          // Found an entry?
          if (t != transition_table_end())
          {
            // Shall we execute the transition?
            if ((t->guard == ETL_NULLPTR) || ((object.*t->guard)()))
            {
              // Shall we execute the action?
              if (t->action != ETL_NULLPTR)
              {
                (object.*t->action)();
              }

              // Changing state?
              if (current_state_id != t->next_state_id)
              {
                const state* s;

                // See if we have a state item for the current state.
                s = find_state(current_state_id);

                // If the current state has an 'on_exit' then call it.
                if ((s != state_table_end()) && (s->on_exit != ETL_NULLPTR))
                {
                  (object.*(s->on_exit))();
                }

                current_state_id = t->next_state_id;

                // See if we have a state item for the new state.
                s = find_state(current_state_id);

                // If the new state has an 'on_entry' then call it.
                if ((s != state_table_end()) && (s->on_entry != ETL_NULLPTR))
                {
                  (object.*(s->on_entry))();
                }
              }

              t = transition_table_end();
            }
            else
            {
              // Start the search from the next item in the table.
              ++t;
            }
          }
        }
      }
    }

    //*************************************************************************
    /// Get a delegate to the process_event function.
    //*************************************************************************
    delegate_t get_process_event_delegate()
    {
      return delegate_t::template create<state_chart<TObject, void>, &state_chart<TObject, void>::process_event>(*this);
    }

  private:

    //*************************************************************************
    const transition* const transition_table_end() const
    {
      return transition_table_begin + transition_table_size;
    }

    //*************************************************************************
    const state* const state_table_end() const
    {
      return state_table_begin + state_table_size;
    }

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
        return (t.event_id == event_id) && (t.from_any_state || (t.current_state_id == state_id));
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

    TObject&          object;                 ///< The object that supplies guard and action member functions.
    const transition* transition_table_begin; ///< The start of the table of transitions.
    const state*      state_table_begin;      ///< The start of the table of states.
    uint_least8_t     transition_table_size;  ///< The size of the table of transitions.
    uint_least8_t     state_table_size;       ///< The size of the table of states.
    bool              started;                ///< Set if the state chart has been started.
  };
}

#endif
