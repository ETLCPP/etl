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
#include "message.h"
#include "message_router.h"
#include "nullptr.h"
#include "timer.h"
#include "timer_config.h"

#include <stdint.h>

//*****************************************************************************
// Definitions common to intrusive timers.
//*****************************************************************************

#if defined(ETL_IN_UNIT_TEST)
namespace etl_unit_test
{
  class etl_unit_test_intrusive_timer;
}
#endif

namespace etl
{
  namespace private_timer
  {
    class register_list;
    class active_list;
    class expired_list;
  } // namespace private_timer

  class intrusive_timer;
  class imessage_router;

  namespace timer
  {
    //***************************************************************************
    /// A timer data structure that can be linked into the timer lists.
    /// Contains a timer configuration.
    ///
    /// registered_next points to the next node in registered list.
    /// - A value of etl::timer::node::unlinked() indicates that the timer is not in the registered list.
    /// - A value of etl::timer::node::end() indicates that the timer is the last in the list.
    ///
    /// active_next points to the next node in active list.
    /// - A value of etl::timer::node::unlinked() indicates that the timer is not in the active list.
    /// - A value of etl::timer::node::end() indicates that the timer is the last in the list.
    ///
    /// expired_next points to the next node in expired list.
    /// - A value of etl::timer::node::unlinked() indicates that the timer is not in the expired list.
    /// - A value of etl::timer::node::end() indicates that the timer is the last in the list.
    ///
    //***************************************************************************
    class node
    {
    public:

      friend class etl::private_timer::register_list;
      friend class etl::private_timer::active_list;
      friend class etl::private_timer::expired_list;

      friend class etl::intrusive_timer;

#if defined(ETL_IN_UNIT_TEST)
      friend class etl_unit_test::etl_unit_test_intrusive_timer;
#endif

      //*******************************************
      /// Constructor.
      //*******************************************
      node(const config& config_) ETL_NOEXCEPT
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
      node(const node& node_) ETL_NOEXCEPT
        : registered_next(unlinked())
        , active_next(unlinked())
        , expired_next(unlinked())
        , owner(etl::timer::owner::No_Owner)
        , config(node_.config)
        , delta(node_.config.get_interval())
      {
      }

      //*******************************************
      /// Destructor (virtual).
      //*******************************************
      virtual ~node() ETL_NOEXCEPT {}

      //*******************************************
      /// Returns the configuration of the timer.
      //*******************************************
      const etl::timer::config& get_config() const ETL_NOEXCEPT
      {
        return config;
      }

      //*******************************************
      /// Returns the delta of the timer.
      //*******************************************
      etl::timer::interval::type get_delta() const ETL_NOEXCEPT
      {
        return delta;
      }

      //*******************************************
      /// Returns the interval of the timer.
      //*******************************************
      etl::timer::interval::type get_interval() const ETL_NOEXCEPT
      {
        return config.get_interval();
      }

      //*******************************************
      /// Returns the mode of the timer.
      //*******************************************
      etl::timer::mode::type get_mode() const ETL_NOEXCEPT
      {
        return config.get_mode();
      }

      //*******************************************
      /// Returns the id of the timer.
      //*******************************************
      etl::timer::id::type get_id() const ETL_NOEXCEPT
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
      static etl::timer::node* end() ETL_NOEXCEPT
      {
        static etl::timer::node sentinel_node(etl::timer::config(etl::timer::interval::No_Interval, etl::timer::mode::Single_Shot));

        return &sentinel_node;
      }

    private:

      //*******************************************
      /// Sets the configuration of the timer.
      //*******************************************
      void set_config(const config& config_) ETL_NOEXCEPT
      {
        set_interval(config_.get_interval());
        set_mode(config_.get_mode());
      }

      //*******************************************
      /// Sets the interval of the timer.
      //*******************************************
      void set_interval(etl::timer::interval::type interval_) ETL_NOEXCEPT
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
      static etl::timer::node* unlinked() ETL_NOEXCEPT
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

      node& operator=(const node&) ETL_DELETE;

      node*                      registered_next; ///< The next timer in the registered timer list. unlinked() if the timer is not registered.
      node*                      active_next;     ///< The next timer in the active timer list. unlinked() if the timer is not active.
      node*                      expired_next;    ///< The next timer in the expired timer list. unlinked() if the timer is not expired.
      const void*                owner;           ///< The owner of the timer.
      etl::timer::config         config;          ///< The configuration of the timer.
      etl::timer::interval::type delta;           ///< The time remaining until the timer expires.
    };

    //***************************************************************************
    /// A timer node that invokes a callback type.
    /// The callback is copied.
    //***************************************************************************
    template <typename TCallback>
    struct callback_node : public etl::timer::node
    {
      //*******************************************
      /// Constructor.
      //*******************************************
      callback_node(const etl::timer::config& config_, TCallback callback_) ETL_NOEXCEPT
        : node(config_)
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

      TCallback callback; ///< The callback for the timer.
    };

    //***************************************************************************
    /// A timer node that invokes a functor type.
    /// Stores a reference to the functor.
    //***************************************************************************
    template <typename TCallback>
    struct callback_ref_node : public etl::timer::node
    {
      //*******************************************
      /// Constructor.
      //*******************************************
      callback_ref_node(const etl::timer::config& config_, TCallback& callback_) ETL_NOEXCEPT
        : node(config_)
        , callback(&callback_)
      {
      }

      //*******************************************
      /// Invokes the timer's callback.
      //*******************************************
      virtual void invoke() ETL_OVERRIDE
      {
        (*callback)();
      }

      TCallback* callback; ///< The callback for the timer.
    };

    //***************************************************************************
    /// A timer node that invokes a message callback.
    //***************************************************************************
    template <typename TMessage>
    struct message_node : public etl::timer::node
    {
      ETL_STATIC_ASSERT((etl::is_base_of<etl::imessage, TMessage>::value), "TMessage must derive from etl::imessage");

      //*******************************************
      /// Constructor.
      //*******************************************
      message_node(const etl::timer::config& config_, etl::imessage_router& router_, TMessage message_) ETL_NOEXCEPT
        : node(config_)
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
  } // namespace timer
} // namespace etl

#endif
