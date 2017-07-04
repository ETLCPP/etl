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
#include "message_router.h"
#include "integral_limits.h"
#include "largest.h"

#undef ETL_FILE
#define ETL_FILE "34"

#ifdef ETL_COMPILER_MICROSOFT
#undef max
#endif

namespace etl
{
  /// Allow alternative type for state id.
#if !defined(ETL_FSM_STATE_ID_TYPE)
    typedef uint_least8_t fsm_state_id_t;
#else
    typedef ETL_FSM_STATE_ID_TYPE fsm_state_id_t;
#endif

  // For internal FSM use.
  typedef typename etl::larger_type<etl::message_id_t>::type fsm_internal_id_t;

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
  /// Interface class for FSM states.
  //***************************************************************************
  class ifsm_state
  {
  public:

    /// Allows ifsm_state functions to be private.
    friend class fsm_helper;

    //*******************************************
    /// Gets the id for this state.
    //*******************************************
    etl::fsm_state_id_t get_state_id() const
    {
      return state_id;
    }

  protected:

    //*******************************************
    /// Constructor.
    //*******************************************
    ifsm_state(etl::fsm_state_id_t state_id_)
      : state_id(state_id_)
    {
    }

  private:

    virtual fsm_state_id_t process_event(etl::imessage_router& source, const etl::imessage& message) = 0;

    virtual fsm_state_id_t on_enter_state() { return state_id; }; // By default, do nothing.
    virtual void on_exit_state() {};  // By default, do nothing.

    // The state id.
    const etl::fsm_state_id_t state_id;

    // Disabled.
    ifsm_state(const ifsm_state&);
    ifsm_state& operator =(const ifsm_state&);
  };

  //***************************************************************************
  /// Helper class for FSM.
  /// Allows ifsm_state functions to be private.
  //***************************************************************************
  class fsm_helper
  {
  public:

    //*******************************************
    inline fsm_state_id_t process_event(etl::ifsm_state&      state,
                                        etl::imessage_router& source,
                                        const etl::imessage&  message)
    {
      return state.process_event(source, message);
    }

    //*******************************************
    inline fsm_state_id_t on_enter_state(etl::ifsm_state& state)
    {
      return state.on_enter_state();
    }

    //*******************************************
    inline void on_exit_state(etl::ifsm_state& state)
    {
      state.on_exit_state();
    }
  };

  /*[[[cog
  import cog
  cog.outl("//***************************************************************************")
  cog.outl("// The code below has been auto generated. Do not manually edit.")
  cog.outl("//***************************************************************************")
  cog.out("template <")
  for n in range(1, int(Handlers)):
      cog.outl("const fsm_internal_id_t ID%d = etl::integral_limits<fsm_internal_id_t>::max - %d, " % (n, n - 1))
      cog.out("          ")
  cog.out("const fsm_internal_id_t ID%d = etl::integral_limits<fsm_internal_id_t>::max - %d" % (int(Handlers), int(Handlers) - 1))
  cog.outl(">")
  cog.outl("//***************************************************************************")
  cog.outl("// The code above has been auto generated. Do not manually edit.")
  cog.outl("//***************************************************************************")
  ]]]*/
  /*[[[end]]]*/
  class fsm : public etl::imessage_router , protected etl::fsm_helper
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

      ETL_ASSERT((number_of_states > 0), ETL_ERROR(etl::fsm_state_list_exception));

      for (etl::fsm_state_id_t i = 0; i < size; ++i)
      {
        ETL_ASSERT((state_list[i] != nullptr), ETL_ERROR(etl::fsm_null_state_exception));
      }
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

        fsm_helper::on_enter_state(*p_state);
      }
    }

    //*******************************************
    /// Top level message handlers for the FSM.
    //*******************************************
    void receive(const etl::imessage& message)
    {
      etl::null_message_router nmr;
      receive(nmr, message);
    }

    //*******************************************
    void receive(etl::imessage_router& source, const etl::imessage& message)
    {
      etl::fsm_state_id_t next_state_id = fsm_helper::process_event(*p_state, source, message);
      ETL_ASSERT(next_state_id < number_of_states, ETL_ERROR(etl::fsm_state_id_exception));

      etl::ifsm_state* p_next_state = state_list[next_state_id];

      // Have we changed state?
      if (p_next_state != p_state)
      {
        do
        {
          p_state = p_next_state;
          fsm_helper::on_exit_state(*p_state);

          next_state_id = fsm_helper::on_enter_state(*p_state);
          ETL_ASSERT(next_state_id < number_of_states, ETL_ERROR(etl::fsm_state_id_exception));

          p_next_state = state_list[next_state_id];

        } while (p_next_state != p_state); // Have we changed state again?
      }
    }

    //*******************************************
    /// Does this FSM accept the message?
    //*******************************************
    bool accepts(const etl::imessage& msg) const
    {
      return accepts(msg.get_message_id());
    }

    //*******************************************
    /// Does this FSM accept the message id?
    //*******************************************
    bool accepts(etl::message_id_t id) const
    {
      /*[[[cog
      import cog
      cog.outl("//***************************************************************************")
      cog.outl("// The code below has been auto generated. Do not manually edit.")
      cog.outl("//***************************************************************************")
      cog.outl("switch (fsm_internal_id_t(id))")
      cog.outl("{")
      cog.out("  ")
      for n in range(1, int(Handlers) + 1):
          cog.out("case ID%d: " % n)
          if n % 8 == 0:
              cog.outl("")
              cog.out("  ")
      cog.outl("  return true; break;")
      cog.outl("  default:")
      cog.outl("    return false; break;")
      cog.outl("}")
      cog.outl("//***************************************************************************")
      cog.outl("// The code above has been auto generated. Do not manually edit.")
      cog.outl("//***************************************************************************")
      ]]]*/
      /*[[[end]]]*/
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
  cog.outl("private:")
  cog.outl("")
  cog.outl("  etl::fsm_state_id_t process_event(etl::imessage_router& source, const etl::imessage& message)")
  cog.outl("  {")
  cog.outl("    etl::fsm_state_id_t new_state_id;")
  cog.outl("    etl::message_id_t event_id = message.get_message_id();")
  cog.outl("")
  cog.outl("    switch (event_id)")
  cog.outl("    {")
  for n in range(1, int(Handlers) + 1):
      cog.out("      case T%d::ID:" % n)
      cog.out(" new_state_id = static_cast<TState*>(this)->on_event(source, static_cast<const T%d&>(message));" % n)
      cog.outl(" break;")
  cog.out("      default:")
  cog.out(" new_state_id = static_cast<TState*>(this)->on_event_unknown(source, message);")
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
          cog.outl("// Specialisation for %d message type." % n)
      else:
          cog.outl("// Specialisation for %d message types." % n)
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
      cog.outl("private:")
      cog.outl("")
      cog.outl("  etl::fsm_state_id_t process_event(etl::imessage_router& source, const etl::imessage& message)")
      cog.outl("  {")
      cog.outl("    etl::fsm_state_id_t new_state_id;")
      cog.outl("    etl::message_id_t event_id = message.get_message_id();")
      cog.outl("")
      cog.outl("    switch (event_id)")
      cog.outl("    {")
      for n in range(1, n + 1):
          cog.out("      case T%d::ID:" % n)
          cog.out(" new_state_id = static_cast<TState*>(this)->on_event(source, static_cast<const T%d&>(message));" % n)
          cog.outl(" break;")
      cog.out("      default:")
      cog.out(" new_state_id = static_cast<TState*>(this)->on_event_unknown(source, message);")
      cog.outl(" break;")
      cog.outl("    }")
      cog.outl("")
      cog.outl("    return new_state_id;")
      cog.outl("  }")
      cog.outl("};")
  ####################################
  # Specialisation for zero messages.
  ####################################
  cog.outl("")
  cog.outl("//***************************************************************************")
  cog.outl("// Specialisation for 0 message types.")
  cog.outl("//***************************************************************************")
  cog.outl("template <typename TState, const etl::fsm_state_id_t STATE_ID_>")
  cog.out("class fsm_state<TState, STATE_ID_, ")
  for t in range(1, int(Handlers)):
      cog.out("void, ")
  if t % 16 == 0:
      cog.outl("")
      cog.out("               ")
  cog.outl("void> : public ifsm_state")
  cog.outl("{")
  cog.outl("public:")
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
  cog.outl("private:")
  cog.outl("")
  cog.outl("  etl::fsm_state_id_t process_event(etl::imessage_router& source, const etl::imessage& message)")
  cog.outl("  {")
  cog.outl("    return static_cast<TState*>(this)->on_event_unknown(source, message);")
  cog.outl("  }")
  cog.outl("};")
  ]]]*/
  /*[[[end]]]*/
}

#undef ETL_FILE

#ifdef ETL_COMPILER_MICROSOFT
#define max(a,b) (((a) > (b)) ? (a) : (b))
#endif

#endif