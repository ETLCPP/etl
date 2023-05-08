/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl
https://www.etlcpp.com

Copyright(c) 2021 Jeremy Overesch, John Wellbelove

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

#ifndef ETL_HFSM_INCLUDED
#define ETL_HFSM_INCLUDED

#include "fsm.h"

namespace etl
{
  //***************************************************************************
  /// The HFSM class.
  /// Builds on the FSM class by overriding the receive function and adding
  /// state hierarchy walking functions.
  //***************************************************************************
  class hfsm : public etl::fsm
  {
  public:

    //*******************************************
    /// Constructor.
    //*******************************************
    hfsm(etl::message_router_id_t id)
      : fsm(id)
    {
    }

    //*******************************************
    /// Starts the HFSM.
    /// Can only be called once.
    /// Subsequent calls will do nothing.
    ///\param call_on_enter_state If true will call on_enter_state() for the first state. Default = true.
    /// If the first state has child states then they will be recersively entered.
    //*******************************************
    void start(bool call_on_enter_state = true) ETL_OVERRIDE
    {
      // Can only be started once.
      if (p_state == ETL_NULLPTR)
      {
        p_state = state_list[0];
        ETL_ASSERT(p_state != ETL_NULLPTR, ETL_ERROR(etl::fsm_null_state_exception));

        if (call_on_enter_state)
        {
          etl::fsm_state_id_t next_state = do_enters(ETL_NULLPTR, p_state, true);

          if (next_state != ifsm_state::No_State_Change)
          {
            p_state = state_list[next_state];
          }
        }
      }
    }

    //*******************************************
    /// Reset the HFSM to pre-started state.
    ///\param call_on_exit_state If true will call on_exit_state() for the current state. Default = false.
    //*******************************************
    virtual void reset(bool call_on_exit_state = false) ETL_OVERRIDE
    {
      if ((p_state != ETL_NULLPTR) && call_on_exit_state)
      {
        do_exits(ETL_NULLPTR, p_state);
      }

      p_state = ETL_NULLPTR;
    }

    using fsm::receive;

    //*******************************************
    /// Top level message handler for the HFSM.
    //*******************************************
    void receive(const etl::imessage& message) ETL_OVERRIDE
    {
      etl::fsm_state_id_t next_state_id = p_state->process_event(message);

      if (next_state_id != ifsm_state::No_State_Change)
      {
        ETL_ASSERT(next_state_id < number_of_states, ETL_ERROR(etl::fsm_state_id_exception));
        etl::ifsm_state* p_next_state = state_list[next_state_id];

        // Have we changed state?
        if (p_next_state != p_state)
        {
          etl::ifsm_state* p_root = common_ancestor(p_state, p_next_state);
          do_exits(p_root, p_state);

          p_state = p_next_state;

          next_state_id = do_enters(p_root, p_next_state, true);

          if (next_state_id != ifsm_state::No_State_Change)
          {
            ETL_ASSERT(next_state_id < number_of_states, ETL_ERROR(etl::fsm_state_id_exception));
            p_state = state_list[next_state_id];
          }
        }
      }
    }

  private:

    //*******************************************
    /// Return the first common ancestor of the two states.
    //*******************************************
    static etl::ifsm_state* common_ancestor(etl::ifsm_state* s1, etl::ifsm_state* s2)
    {
      size_t depth1 = get_depth(s1);
      size_t depth2 = get_depth(s2);

      // Adjust s1 and s2 to the same depth.
      if (depth1 > depth2)
      {
        s1 = adjust_depth(s1, depth1 - depth2);
      }
      else
      {
        s2 = adjust_depth(s2, depth2 - depth1);
      }

      // Now they're aligned to the same depth they can step towards the root together.
      while (s1 != s2)
      {
        s1 = s1->p_parent;
        s2 = s2->p_parent;
      }

      return s1;
    }

    //*******************************************
    /// Find the depth of the state.
    //*******************************************
    static size_t get_depth(etl::ifsm_state* s)
    {
      size_t depth = 0UL;

      while (s != ETL_NULLPTR)
      {
        s = s->p_parent;
        ++depth;
      }

      return depth;
    }

    //*******************************************
    /// Align the depths of the states.
    //*******************************************
    static etl::ifsm_state* adjust_depth(etl::ifsm_state* s, size_t offset)
    {
      while (offset != 0U)
      {
        s = s->p_parent;
        --offset;
      }

      return s;
    }

    //*******************************************
    /// Entering the state.
    //*******************************************
    static etl::fsm_state_id_t do_enters(const etl::ifsm_state* p_root, etl::ifsm_state* p_target, bool activate_default_children)
    {
      ETL_ASSERT(p_target != ETL_NULLPTR, ETL_ERROR(etl::fsm_null_state_exception));

      // We need to go recursively up the tree if the target and root don't match
      if ((p_root != p_target) && (p_target->p_parent != ETL_NULLPTR))
      {
        if (p_target->p_parent != p_root)
        {
          // The parent we're calling shouldn't activate its defaults, or this state will be deactivated.
          do_enters(p_root, p_target->p_parent, false);
        }

        // Set us as our parent's active child
        p_target->p_parent->p_active_child = p_target;
      }

      etl::fsm_state_id_t next_state = p_target->on_enter_state();
      ETL_ASSERT(ifsm_state::No_State_Change == next_state, ETL_ERROR(etl::fsm_state_composite_state_change_forbidden));

      // Activate default child if we need to activate any initial states in an active composite state.
      if (activate_default_children)
      {
        while (p_target->p_default_child != ETL_NULLPTR)
        {
          p_target = p_target->p_default_child;
          p_target->p_parent->p_active_child = p_target;
          next_state = p_target->on_enter_state();
          ETL_ASSERT(ifsm_state::No_State_Change == next_state, ETL_ERROR(etl::fsm_state_composite_state_change_forbidden));
        }

        next_state = p_target->get_state_id();
      }

      return next_state;
    }

    //*******************************************
    /// Exiting the state.
    //*******************************************
    static void do_exits(const etl::ifsm_state* p_root, etl::ifsm_state* p_source)
    {
      etl::ifsm_state* p_current = p_source;

      // Iterate down to the lowest child
      while (p_current->p_active_child != ETL_NULLPTR)
      {
        p_current = p_current->p_active_child;
      }

      // Run exit state on all states up to the root
      while (p_current != p_root)
      {
        p_current->on_exit_state();
        p_current = p_current->p_parent;
      }
    }
  };
}
#endif
