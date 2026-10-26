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
#include "delegate.h"
#include "message_router.h"
#include "nullptr.h"
#include "timer.h"

#include <stdint.h>

//*****************************************************************************
// Definitions common to intrusive timers.
//*****************************************************************************

#if defined(ETL_IN_UNIT_TEST)
namespace
{
  class etl_unit_test_intrusive_timer;
}
#endif

namespace etl
{
  namespace private_timer
  {
    class timer_register_list;
    class timer_active_list;
    class timer_expired_list;
  } // namespace private_timer

  class timer_node;
  class intrusive_timer;

  //***************************************************************************
  /// A timer configuration data structure.
  //***************************************************************************
  class timer_config
  {
  public:

    friend class private_timer::timer_register_list;
    friend class private_timer::timer_active_list;
    friend class private_timer::timer_expired_list;

    friend class timer_node;
    friend class intrusive_timer;

    //*******************************************
    /// Default constructor.
    //*******************************************
    ETL_CONSTEXPR timer_config() ETL_NOEXCEPT
      : interval(etl::timer::interval::No_Active_Interval)
      , mode(etl::timer::mode::No_Mode_Set)
      , id(etl::timer::id::No_Timer)
    {
    }

    //*******************************************
    /// Constructor.
    //*******************************************
    ETL_CONSTEXPR timer_config(uint32_t interval_, etl::timer::mode::type mode_) ETL_NOEXCEPT
      : interval(interval_)
      , mode(mode_)
      , id(etl::timer::id::No_Timer)
    {
    }

    //*******************************************
    /// Returns the interval of the timer.
    //*******************************************
    ETL_CONSTEXPR uint32_t get_interval() const ETL_NOEXCEPT
    {
      return interval;
    }

    //*******************************************
    /// Returns the mode of the timer.
    //*******************************************
    ETL_CONSTEXPR etl::timer::mode::type get_mode() const ETL_NOEXCEPT
    {
      return mode;
    }

    //*******************************************
    /// Returns the id of the timer.
    //*******************************************
    ETL_CONSTEXPR etl::timer::id::type get_id() const ETL_NOEXCEPT
    {
      return id;
    }

    //*******************************************
    /// Clears the timer configuration.
    //*******************************************
    ETL_CONSTEXPR14 void clear() ETL_NOEXCEPT
    {
      interval = etl::timer::interval::No_Active_Interval;
      mode     = etl::timer::mode::No_Mode_Set;
      id       = etl::timer::id::No_Timer;
    }

  private:

    uint32_t               interval; ///< The interval of the timer.
    etl::timer::mode::type mode;     ///< Whether the timer is repeating or single-shot.
    etl::timer::id::type   id;       ///< The id of the timer.
  };

  //***************************************************************************
  /// A timer data structure that can be linked into the timer lists.
  /// Contains a timer configuration.
  ///
  /// registered_next points to the next node in registered list.
  /// - A value of etl::timer_node::unlinked() indicates that the timer is not in the registered list.
  /// - A value of etl::timer_node::end() indicates that the timer is the last in the list.
  ///
  /// active_next points to the next node in active list.
  /// - A value of etl::timer_node::unlinked() indicates that the timer is not in the active list.
  /// - A value of etl::timer_node::end() indicates that the timer is the last in the list.
  ///
  /// expired_next points to the next node in expired list.
  /// - A value of etl::timer_node::unlinked() indicates that the timer is not in the expired list.
  /// - A value of etl::timer_node::end() indicates that the timer is the last in the list.
  ///
  //***************************************************************************
  class timer_node
  {
  public:

    friend class private_timer::timer_register_list;
    friend class private_timer::timer_active_list;
    friend class private_timer::timer_expired_list;

    friend class intrusive_timer;

#if defined(ETL_IN_UNIT_TEST)
    friend class etl_unit_test_intrusive_timer;
#endif

    //*******************************************
    /// Constructor.
    //*******************************************
    timer_node(const timer_config& config_) ETL_NOEXCEPT
      : registered_next(unlinked())
      , active_next(unlinked())
      , expired_next(unlinked())
      , owner(etl::timer::owner::No_Owner)
      , config(config_)
      , delta(config_.get_interval())
    {
    }

    //*******************************************
    /// Copy constructor.
    //*******************************************
    timer_node(const timer_node& node_) ETL_NOEXCEPT
      : registered_next(unlinked())
      , active_next(unlinked())
      , expired_next(unlinked())
      , owner(etl::timer::owner::No_Owner)
      , config(node_.config)
      , delta(node_.config.get_interval())
    {
    }

    //*******************************************
    /// Returns the configuration of the timer.
    //*******************************************
    const timer_config& get_config() const ETL_NOEXCEPT
    {
      return config;
    }

    //*******************************************
    /// Returns the delta of the timer.
    //*******************************************
    ETL_CONSTEXPR uint32_t get_delta() const ETL_NOEXCEPT
    {
      return delta;
    }

    //*******************************************
    /// Returns the interval of the timer.
    //*******************************************
    ETL_CONSTEXPR uint32_t get_interval() const ETL_NOEXCEPT
    {
      return config.get_interval();
    }

    //*******************************************
    /// Returns the mode of the timer.
    //*******************************************
    ETL_CONSTEXPR etl::timer::mode::type get_mode() const ETL_NOEXCEPT
    {
      return config.get_mode();
    }

    //*******************************************
    /// Returns the id of the timer.
    //*******************************************
    ETL_CONSTEXPR etl::timer::id::type get_id() const ETL_NOEXCEPT
    {
      return config.get_id();
    }

    //***************************************************************************
    /// Returns a pointer to the static sentinel timer node used to mark the end of
    /// the intrusive timer lists.
    /// Implemented as a function-local static so that there is only ever one
    /// instance, regardless of how many translation units include this header.
    /// This avoids needing C++17 inline variables, so it remains compatible
    /// with C++03 and C++11.
    //***************************************************************************
    static etl::timer_node* end() ETL_NOEXCEPT
    {
      static etl::timer_node sentinel_node(etl::timer_config(etl::timer::interval::No_Active_Interval, etl::timer::mode::Single_Shot));

      return &sentinel_node;
    }

  private:

    //*******************************************
    /// Sets the configuration of the timer.
    //*******************************************
    void set_config(const timer_config& config_) ETL_NOEXCEPT
    {
      set_interval(config_.get_interval());
      set_mode(config_.get_mode());
    }

    //*******************************************
    /// Sets the interval of the timer.
    //*******************************************
    void set_interval(uint32_t interval_) ETL_NOEXCEPT
    {
      config.interval = interval_;
    }

    //*******************************************
    /// Sets the mode of the timer.
    //*******************************************
    void set_mode(etl::timer::mode::type mode_) ETL_NOEXCEPT
    {
      config.mode = mode_;
    }

    //*******************************************
    /// Sets the ID of the timer.
    //*******************************************
    void set_id(etl::timer::id::type id_) ETL_NOEXCEPT
    {
      config.id = id_;
    }

    //*******************************************
    /// Sets the owner of the timer.
    //*******************************************
    void set_owner(void* owner_) ETL_NOEXCEPT
    {
      owner = owner_;
    }

    //*******************************************
    /// Returns a nullptr to indicate that the timer is not linked into any list.
    //*******************************************
    static etl::timer_node* unlinked() ETL_NOEXCEPT
    {
      return ETL_NULLPTR;
    }

    //*******************************************
    /// Clear the node.
    //*******************************************
    void clear() ETL_NOEXCEPT
    {
      config.clear();
      registered_next = unlinked();
      active_next     = unlinked();
      expired_next    = unlinked();
      owner           = etl::timer::owner::No_Owner;
      delta           = config.get_interval();
    }

    //*******************************************
    /// Returns true if the timer is registered to the specified owner.
    //*******************************************
    bool is_registered_with(const void* owner_) const ETL_NOEXCEPT
    {
      return (registered_next != unlinked()) && (owner == owner_);
    }

    //*******************************************
    /// Returns true if the timer is registered to an owner.
    //*******************************************
    bool is_registered() const ETL_NOEXCEPT
    {
      return (registered_next != unlinked()) && (owner != ETL_NULLPTR);
    }

    //*******************************************
    /// Returns true if the timer is active.
    //*******************************************
    bool is_active() const ETL_NOEXCEPT
    {
      return active_next != unlinked();
    }

    //*******************************************
    /// Returns true if the timer has expired.
    //*******************************************
    bool has_expired() const ETL_NOEXCEPT
    {
      return expired_next != unlinked();
    }

    //*******************************************
    /// Invokes the timer's callback.
    //*******************************************
    virtual void invoke()
    {
      // Default implementation does nothing.
    }

    timer_node& operator=(const timer_node&) ETL_DELETE;

    timer_node*  registered_next; ///< The next timer in the registered timer list. unlinked() if the timer is not registered.
    timer_node*  active_next;     ///< The next timer in the active timer list. unlinked() if the timer is not active.
    timer_node*  expired_next;    ///< The next timer in the expired timer list. unlinked() if the timer is not expired.
    void*        owner;           ///< The owner of the timer.
    timer_config config;          ///< The configuration of the timer.
    uint32_t     delta;           ///< The time remaining until the timer expires.
  };

  //***************************************************************************
  /// A timer node that invokes a callback delegate type.
  //***************************************************************************
  template <typename TCallback>
  struct timer_callback_node : public timer_node
  {
    //*******************************************
    /// Constructor.
    //*******************************************
    timer_callback_node(const etl::timer_config& config_, TCallback callback_) ETL_NOEXCEPT
      : timer_node(config_)
      , callback(callback_)
    {
    }

    //*******************************************
    /// Invokes the timer's callback.
    //*******************************************
    virtual void invoke() ETL_OVERRIDE
    {
      callback();
    }

    TCallback callback; ///< The callback delegate for the timer.
  };

  //***************************************************************************
  /// A timer node that invokes a message callback.
  //***************************************************************************
  template <typename TMessage>
  struct timer_message_node : public timer_node
  {
    ETL_STATIC_ASSERT((etl::is_base_of<etl::imessage, TMessage>::value), "TMessage must derive from etl::imessage");

    //*******************************************
    /// Constructor.
    //*******************************************
    timer_message_node(const etl::timer_config& config_, etl::imessage_router& router_, TMessage message_) ETL_NOEXCEPT
      : timer_node(config_)
      , router(&router_)
      , message(message_)
    {
    }

    //*******************************************
    /// Invokes the timer's callback.
    //*******************************************
    virtual void invoke() ETL_OVERRIDE
    {
      router->receive(message);
    }

    etl::imessage_router* router;  ///< The message router to send the message to.
    TMessage              message; ///< The message to send when the timer expires.
  };
} // namespace etl

#endif
