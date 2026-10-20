/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl
https://www.etlcpp.com

Copyright(c) 2026 John Wellbelove

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

#ifndef ETL_TIMER_NODE_INCLUDED
#define ETL_TIMER_NODE_INCLUDED

#include "platform.h"
#include "nullptr.h"
#include "timer.h"

#include <stdint.h>

//*****************************************************************************
// Definitions common to intrusive timers.
//*****************************************************************************

namespace etl
{
  namespace private_timer
  {
    class timer_register_list;
    class timer_active_list;
    class timer_triggered_list;

    class intrusive_callback_timer;
  } // namespace private_timer

  //***************************************************************************
  /// A timer configuration data structure.
  //***************************************************************************
  class timer_config
  {
  public:

    friend class private_timer::timer_register_list;
    friend class private_timer::timer_active_list;
    friend class private_timer::timer_triggered_list;

    friend class intrusive_callback_timer;

    //*******************************************
    /// Constructor.
    //*******************************************
    ETL_CONSTEXPR timer_config(uint32_t period_, etl::timer::mode::type repeating_) ETL_NOEXCEPT
      : period(period_)
      , delta(period_)
      , mode(repeating_)
    {
    }

    //*******************************************
    /// Returns the period of the timer.
    //*******************************************
    ETL_CONSTEXPR uint32_t get_period() const ETL_NOEXCEPT
    {
      return period;
    }

    //*******************************************
    /// Returns the time remaining until the timer expires.
    //*******************************************
    ETL_CONSTEXPR uint32_t get_delta() const ETL_NOEXCEPT
    {
      return delta;
    }

    //*******************************************
    /// Returns the mode of the timer.
    //*******************************************
    ETL_CONSTEXPR uint32_t get_mode() const ETL_NOEXCEPT
    {
      return mode;
    }

    //*******************************************
    /// Clears the timer configuration.
    //*******************************************
    ETL_CONSTEXPR14 void clear() ETL_NOEXCEPT
    {
      period = etl::timer::state::Inactive;
      delta  = etl::timer::state::Inactive;
      mode   = etl::timer::mode::Repeating;
    }

  private:

    uint32_t               period; ///< The period of the timer.
    uint32_t               delta;  ///< The time remaining until the timer expires.
    etl::timer::mode::type mode;   ///< Whether the timer is repeating or single-shot.
  };

  //***************************************************************************
  /// A timer data structure that can be linked into the timer lists.
  /// Contains a timer configuration.
  //***************************************************************************
  class timer_node
  {
  public:

    friend class private_timer::timer_register_list;
    friend class private_timer::timer_active_list;
    friend class private_timer::timer_triggered_list;

    friend class intrusive_callback_timer;

    //*******************************************
    /// Constructor.
    //*******************************************
    timer_node(const timer_config& config_) ETL_NOEXCEPT
      : registered_next(unlinked())
      , active_next(unlinked())
      , triggered_next(unlinked())
      , config(config_)
    {
    }

    //*******************************************
    /// Copy constructor.
    //*******************************************
    timer_node(const timer_node& node_) ETL_NOEXCEPT
      : registered_next(unlinked())
      , active_next(unlinked())
      , triggered_next(unlinked())
      , config(node_.config)
    {
    }

    //*******************************************
    /// Returns the configuration of the timer.
    //*******************************************
    const timer_config& get_config() const ETL_NOEXCEPT
    {
      return config;
    }

  private:

    //*******************************************
    /// Returns a nullptr to indicate that the timer is not linked into any list.
    //*******************************************
    static etl::timer_node* unlinked() ETL_NOEXCEPT
    {
      return ETL_NULLPTR;
    }

    //***************************************************************************
    /// Returns a reference to the sentinel timer node used to mark the end of
    /// the intrusive timer lists.
    /// Implemented as a function-local static so that there is only ever one
    /// instance, regardless of how many translation units include this header.
    /// This avoids needing C++17 inline variables, so it remains compatible
    /// with C++03 and C++11.
    //***************************************************************************
    static etl::timer_node* sentinal() ETL_NOEXCEPT
    {
      static etl::timer_node timer_sentinel(etl::timer_config(0, 0));

      return &timer_sentinel;
    }

    //*******************************************
    /// Clear the next pointers.
    //*******************************************
    void clear() ETL_NOEXCEPT
    {
      registered_next = unlinked();
      active_next     = unlinked();
      triggered_next  = unlinked();
    }

    timer_node& operator=(const timer_node&) ETL_DELETE;

    timer_node* registered_next; ///< The next timer in the registered timer list. nullptr if the timer is not registered.
    timer_node* active_next;     ///< The next timer in the active timer list. nullptr if the timer is not active.
    timer_node* triggered_next;  ///< The next timer in the triggered timer list. nullptr if the timer is not triggered.

    timer_config config; ///< The configuration of the timer.
  };

  ////***************************************************************************
  ///// A timer data structure that includes a callback delegate.
  ////***************************************************************************
  // template <typename TCallback>
  // struct callback_timer_config : public timer_node
  //{
  //   callback_timer_config(TCallback callback_) ETL_NOEXCEPT
  //     : timer_config()
  //     , callback(callback_)
  //   {
  //   }

  //  TCallback callback; ///< The callback delegate for the timer.
  //};
} // namespace etl

#endif
