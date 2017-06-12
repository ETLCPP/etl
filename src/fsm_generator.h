/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl
http://www.etlcpp.com

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

#ifndef __ETL_FSM__
#define __ETL_FSM__

#include <stdint.h>

#include "array.h"
#include "nullptr.h"
#include "error_handler.h"
#include "exception.h"
#include "user_type.h"

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
  class fsm_null_state_exception : public etl::fsm_exception
  {
  public:

    fsm_null_state_exception(string_type file_name, numeric_type line_number)
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
  /// Exception for incompatible state list.
  //***************************************************************************
  class fsm_state_list_exception : public etl::fsm_exception
  {
  public:

    fsm_state_list_exception(string_type file_name, numeric_type line_number)
      : etl::fsm_exception(ETL_ERROR_TEXT("fsm:state list", ETL_FILE"C"), file_name, line_number)
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

    friend class fsm;

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
  class fsm
  {
  public:

    //*******************************************
    /// Constructor.
    //*******************************************
    fsm()
      : p_state(nullptr)
    {
    }

    //*******************************************
    /// Set the states for the FSM
    //*******************************************
    template <typename TSize>
    void set_states(etl::ifsm_state** p_states, TSize size)
    {
      state_list       = p_states;
      number_of_states = etl::fsm_state_id_t(size);

      for (etl::fsm_state_id_t i = 0; i < size; ++i)
      {
        ETL_ASSERT((state_list[i] != nullptr), ETL_ERROR(etl::fsm_null_state_exception));
      }

      bool ok = (number_of_states > 0) &&
                etl::is_sorted(state_list, state_list + number_of_states, fsm::CompareStateId());

      ETL_ASSERT(ok, ETL_ERROR(etl::fsm_state_list_exception));
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
        ETL_ASSERT(p_state != nullptr, ETL_ERROR(etl::fsm_null_state_exception));

        p_state->on_enter_state();
      }
    }

    //*******************************************
    /// Top level event handler for the FSM.
    //*******************************************
    void on_event(const etl::ifsm_event& event)
    {
      etl::fsm_state_id_t next_state_id = p_state->on_event(event);
      ETL_ASSERT(next_state_id < number_of_states, ETL_ERROR(etl::fsm_state_id_exception));

      // Have we changed state?
      if (next_state_id != p_state->get_state_id())
      {
        p_state->on_exit_state();

        p_state = state_list[next_state_id];
        ETL_ASSERT(p_state != nullptr, ETL_ERROR(etl::fsm_null_state_exception));

        p_state->on_enter_state();
      }
    }

    //*******************************************
    /// Gets the current state id.
    //*******************************************
    etl::fsm_state_id_t get_state_id() const
    {
      ETL_ASSERT(p_state != nullptr, ETL_ERROR(etl::fsm_null_state_exception));
      return p_state->get_state_id();
    }

    //*******************************************
    /// Gets a reference to the current state interface.
    //*******************************************
    ifsm_state& get_state()
    {
      ETL_ASSERT(p_state != nullptr, ETL_ERROR(etl::fsm_null_state_exception));
      return *p_state;
    }

    //*******************************************
    /// Gets a const reference to the current state interface.
    //*******************************************
    const ifsm_state& get_state() const
    {
      ETL_ASSERT(p_state != nullptr, ETL_ERROR(etl::fsm_null_state_exception));
      return *p_state;
    }

    //*******************************************
    /// Checks if the FSM has been started.
    //*******************************************
    bool is_started() const
    {
      return p_state != nullptr;
    }

    //*******************************************
    /// Reset the FSM to pre-started state.
    //*******************************************
    void reset()
    {
      p_state = nullptr;
    }

  private:

    struct CompareStateId
    {
      bool operator()(etl::ifsm_state* lhs, etl::ifsm_state* rhs)
      {
        return lhs->get_state_id() < rhs->get_state_id();
      }
    };

    etl::ifsm_state*    p_state;          ///< A pointer to the current state.
    etl::ifsm_state**   state_list;       ///< The list of added states.
    etl::fsm_state_id_t number_of_states; ///< The number of states.
  };

  //***************************************************************************
  // To generate to header file, run this at the command line.
  // Note: You will need Python and COG installed.
  //
  // python -m cogapp -d -e -ofsm.h -DHandlers=<n> fsm_generator.h 
  // Where <n> is the number of messages to support.
  //
  // e.g.
  // To generate handlers for up to 16 events...
  // python -m cogapp -d -e -ofsm.h -DHandlers=16 fsm_generator.h 
  //
  // See CreateFSM.bat
  //***************************************************************************

  /*[[[cog
  import cog
  cog.outl("//***************************************************************************")
  cog.outl("// The code below has been auto generated. Do not manually edit.")
  cog.outl("//***************************************************************************")
  cog.outl("")
  ################################################
  # The first definition for all of the events.
  ################################################
  cog.outl("//***************************************************************************")
  cog.outl("// The definition for all %s message types." % Handlers)
  cog.outl("//***************************************************************************")
  cog.outl("template <typename TState, const etl::fsm_state_id_t STATE_ID_, ")
  cog.out("          ")
  for n in range(1, int(Handlers)):
      cog.out("typename T%s = void, " % n)
      if n % 4 == 0:
          cog.outl("")
          cog.out("          ")
  cog.outl("typename T%s = void>" % Handlers)
  cog.outl("class fsm_state : public ifsm_state")
  cog.outl("{")
  cog.outl("public:")
  cog.outl("")

  cog.outl("  friend class fsm;")
  cog.outl("")

  cog.outl("  enum")
  cog.outl("  {")
  cog.outl("    STATE_ID = STATE_ID_")
  cog.outl("  };")
  cog.outl("")
  cog.outl("  fsm_state()")
  cog.outl("    : ifsm_state(STATE_ID)")
  cog.outl("  {")
  cog.outl("  }")
  cog.outl("")
  cog.outl("  etl::fsm_state_id_t on_event(const etl::ifsm_event& event)")
  cog.outl("  {")
  cog.outl("    etl::fsm_state_id_t new_state_id;")
  cog.outl("    etl::fsm_event_id_t event_id = event.get_event_id();")
  cog.outl("")
  cog.outl("    switch (event_id)")
  cog.outl("    {")
  for n in range(1, int(Handlers) + 1):
      cog.out("      case T%d::EVENT_ID:" % n)
      cog.out(" new_state_id = static_cast<TState*>(this)->on_event(static_cast<const T%d&>(event));" % n)
      cog.outl(" break;")
  cog.out("      default:")
  cog.out(" new_state_id = static_cast<TState*>(this)->on_event_unknown(event);")
  cog.outl(" break;")
  cog.outl("    }")
  cog.outl("")
  cog.outl("    return new_state_id;")
  cog.outl("  }")
  cog.outl("};")

  ####################################
  # All of the other specialisations.
  ####################################
  for n in range(int(Handlers) - 1, 0, -1):
      cog.outl("")
      cog.outl("//***************************************************************************")
      if n == 1:
          cog.outl("// Specialisation for %d event type." % n)
      else:
          cog.outl("// Specialisation for %d event types." % n)
      cog.outl("//***************************************************************************")
      cog.outl("template <typename TState, const etl::fsm_state_id_t STATE_ID_, ")
      cog.out("          ")
      for t in range(1, n):
          cog.out("typename T%d, " % t)
          if t % 4 == 0:
              cog.outl("")
              cog.out("          ")
      cog.outl("typename T%d>" % n)
      cog.out("class fsm_state<TState, STATE_ID_, ")
      for t in range(1, n + 1):
          cog.out("T%d, " % t)
      if t % 16 == 0:
          cog.outl("")
          cog.out("               ")
      for t in range(n + 1, int(Handlers)):
          cog.out("void, ")
      if t % 16 == 0:
          cog.outl("")
          cog.out("               ")
      cog.outl("void> : public ifsm_state")
      cog.outl("{")
      cog.outl("public:")

      cog.outl("  friend class fsm;")
      cog.outl("")

      cog.outl("")
      cog.outl("  enum")
      cog.outl("  {")
      cog.outl("    STATE_ID = STATE_ID_")
      cog.outl("  };")
      cog.outl("")
      cog.outl("  fsm_state()")
      cog.outl("    : ifsm_state(STATE_ID)")
      cog.outl("  {")
      cog.outl("  }")
      cog.outl("")
      cog.outl("  etl::fsm_state_id_t on_event(const etl::ifsm_event& event)")
      cog.outl("  {")
      cog.outl("    etl::fsm_state_id_t new_state_id;")
      cog.outl("    etl::fsm_event_id_t event_id = event.get_event_id();")
      cog.outl("")
      cog.outl("    switch (event_id)")
      cog.outl("    {")
      for n in range(1, n + 1):
          cog.out("      case T%d::EVENT_ID:" % n)
          cog.out(" new_state_id = static_cast<TState*>(this)->on_event(static_cast<const T%d&>(event));" % n)
          cog.outl(" break;")
      cog.out("      default:")
      cog.out(" new_state_id = static_cast<TState*>(this)->on_event_unknown(event);")
      cog.outl(" break;")
      cog.outl("    }")
      cog.outl("")
      cog.outl("    return new_state_id;")
      cog.outl("  }")
      cog.outl("};")
  ]]]*/
  /*[[[end]]]*/
}

#undef ETL_FILE

#endif