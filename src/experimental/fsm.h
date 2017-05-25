
#ifndef __ETL_FSM__
#define __ETL_FSM__

#include <stdint.h>

#include "../array.h"
#include "../nullptr.h"
#include "../error_handler.h"
#include "../exception.h"

#undef ETL_FILE
#define ETL_FILE "34"

namespace etl
{
#if !defined(ETL_FSM_STATE_ID_TYPE)
    typedef uint_least8_t fsm_state_id_t;
#else
    typedef ETL_FSM_STATE_ID_TYPE fsm_state_id_t;
#endif

#if !defined(ETL_FSM_EVENT_ID_TYPE)
    typedef uint_least8_t fsm_event_id_t;
#else
    typedef ETL_FSM_STATE_ID_TYPE fsm_event_id_t;
#endif

  //***************************************************************************
  /// Base exception class for FSM.
  //***************************************************************************
  class fsm_exception : public etl::exception
  {
  public:

    fsm_exception(string_type what, string_type file_name, numeric_type line_number)
      : etl::exception(what, file_name, line_number)
    {
    }
  };

  //***************************************************************************
  /// Exception for null state pointer.
  //***************************************************************************
  class fsm_nullstate_exception : public etl::fsm_exception
  {
  public:

    fsm_nullstate_exception(string_type file_name, numeric_type line_number)
      : etl::fsm_exception(ETL_ERROR_TEXT("fsm:null state", ETL_FILE"A"), file_name, line_number)
    {
    }
  };

  //***************************************************************************
  /// Exception for invalid state id.
  //***************************************************************************
  class fsm_state_id_exception : public etl::fsm_exception
  {
  public:

    fsm_state_id_exception(string_type file_name, numeric_type line_number)
      : etl::fsm_exception(ETL_ERROR_TEXT("fsm:state id", ETL_FILE"B"), file_name, line_number)
    {
    }
  };

  //***************************************************************************
  /// Interface class for FSM events.
  //***************************************************************************
  class ifsm_event
  {
  public:

    //*******************************************
    /// Gets the id for this event.
    //*******************************************
    etl::fsm_event_id_t get_event_id() const
    {
      return event_id;
    }

  protected:

    //*******************************************
    /// Constructor.
    //*******************************************
    ifsm_event(etl::fsm_event_id_t event_id_)
      : event_id(event_id_)
    {
    }

  private:

    // The event id.
    const etl::fsm_event_id_t event_id;
  };

  //***************************************************************************
  /// Base class for FSM events.
  //***************************************************************************
  template <const etl::fsm_event_id_t EVENT_ID_>
  class fsm_event : public etl::ifsm_event
  {
  public:

    enum
    {
      EVENT_ID = EVENT_ID_
    };

    //*******************************************
    /// Constructor.
    //*******************************************
    fsm_event()
      : ifsm_event(EVENT_ID)
    {
    }
  };

  //***************************************************************************
  /// Interface class for FSM states.
  //***************************************************************************
  class ifsm_state
  {
  public:

    //*******************************************
    /// Gets the id for this state.
    //*******************************************
    etl::fsm_state_id_t get_state_id() const
    {
      return state_id;
    }

    virtual etl::fsm_state_id_t on_event(const etl::ifsm_event& event) = 0;

  protected:

    //*******************************************
    /// Constructor.
    //*******************************************
    ifsm_state(etl::fsm_state_id_t state_id_)
      : state_id(state_id_)
    {
    }

    virtual void on_enter_state() {}; // By default, do nothing.
    virtual void on_exit_state() {};  // By default, do nothing.

  private:

    // The state id.
    const etl::fsm_state_id_t state_id;

    // Disabled.
    ifsm_state(const ifsm_state&);
    ifsm_state& operator =(const ifsm_state&);
  };

  //***************************************************************************
  // To be COG generated.
  /// Base class for FSM states.
  //***************************************************************************
  template <typename TState, const etl::fsm_state_id_t STATE_ID_, typename T1, typename T2 = void>
  class fsm_state : public ifsm_state
  {
  public:

    enum
    {
      STATE_ID = STATE_ID_
    };

    //*******************************************
    /// Constructor.
    //*******************************************
    fsm_state()
      : ifsm_state(STATE_ID)
    {
    }

    //*******************************************
    /// Top level event handler for the state.
    //*******************************************
    etl::fsm_state_id_t on_event(const etl::ifsm_event& event)
    {
      etl::fsm_state_id_t new_state_id;
      etl::fsm_event_id_t id = event.get_event_id();

      switch (id)
      {
        case T1::EVENT_ID: new_state_id = static_cast<TState&>(*this).on_event(static_cast<const T1&>(event)); break;
        case T2::EVENT_ID: new_state_id = static_cast<TState&>(*this).on_event(static_cast<const T2&>(event)); break;
        default: new_state_id = static_cast<TState&>(*this).on_unknown_event(event); break;
      }

      return new_state_id;
    }

  private:

    // Disabled.
    fsm_state(const fsm_state&);
    fsm_state& operator =(const fsm_state&);
  };

  //***************************************************************************
  template <const etl::fsm_state_id_t MAX_STATES>
  class fsm
  {
  public:

    //*******************************************
    /// Constructor.
    //*******************************************
    fsm()
      : p_state(nullptr)
    {
      state_list.fill(nullptr);
    }

    //*******************************************
    /// Starts the FSM.
    /// Can only be called once.
    /// Subsequent calls will do nothing.
    //*******************************************
    void start()
    {
      // Can only be started once.
      if (p_state == nullptr)
      {
        p_state = state_list[0];
        ETL_ASSERT(p_state != nullptr, ETL_ERROR(etl::fsm_nullstate_exception));

        p_state->on_enter_state();
      }
    }

    //*******************************************
    /// Adds a state to the FSM.
    /// If the state has the same id as one already added
    /// then the current state will be overwritten.
    //*******************************************
    void add_state(etl::ifsm_state& state)
    {
      ETL_ASSERT(state.get_state_id() < MAX_STATES, ETL_ERROR(etl::fsm_state_id_exception));
      state_list[state.get_state_id()] = &state;
    }

    //*******************************************
    /// Top level event handler for the FSM.
    //*******************************************
    void on_event(const etl::ifsm_event& event)
    {
      etl::fsm_state_id_t next_state_id = p_state->on_event(event);
      ETL_ASSERT(next_state_id < MAX_STATES, ETL_ERROR(etl::fsm_state_id_exception));

      // Have we changed state?
      if (next_state_id != p_state->get_state_id())
      {
        p_state->on_exit_state();

        p_state = state_list[next_state_id];
        ETL_ASSERT(p_state != nullptr, ETL_ERROR(etl::fsm_nullstate_exception));

        p_state->on_enter_state();
      }
    }

    //*******************************************
    /// Gets the current state id.
    //*******************************************
    etl::fsm_state_id_t get_state_id() const
    {
      return p_state->get_state_id();
    }

    //*******************************************
    /// Gets a reference to the current state interface.
    //*******************************************
    ifsm_state& get_state()
    {
      return *p_state;
    }

    //*******************************************
    /// Gets a const reference to the current state interface.
    //*******************************************
    const ifsm_state& get_state() const
    {
      return *p_state;
    }

  private:

    etl::ifsm_state* p_state;                            ///< A pointer to the current state.
    etl::array<etl::ifsm_state*, MAX_STATES> state_list; ///< The list of added states.

    // Disabled.
    fsm(const fsm&);
    fsm& operator =(const fsm&);
  };
}

#undef ETL_FILE

// class Start : public etl::fsm_event<0>
// {

// };

// class Idle : public etl::fsm_state<Idle, 0, Start>
// {
// public:

//   etl::fsm_state_id_t on_event(const Start& event)
//   {
//     return 1;
//   }

//   etl::fsm_state_id_t on_unknown_event(const etl::ifsm_event& event)
//   {
//     return THIS_STATE_ID;
//   }
// };

// etl::fsm<3> machine;

// void F()
// {
//   Idle idle;

//   machine.add_state(idle);
//   machine.start();
//   machine.on_event(Start());
// }

#endif