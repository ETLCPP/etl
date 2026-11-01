///\file

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

#ifndef ETL_DEBOUNCE_VALUE_INCLUDED
#define ETL_DEBOUNCE_VALUE_INCLUDED

#include "platform.h"
#include "type_traits.h"

namespace etl
{
  template <typename TValue>
  class debounce_value_notification
  {
  public:

    virtual void on_debounce_value_notification(TValue value) = 0;
  };

  namespace private_debounce_value
  {
    template <typename TValue, typename TCount>
    class debounce_value_base
    {
    public:

      typedef TCount count_type;
      typedef TValue value_type;

      typedef debounce_value_notification<value_type> notification_type;

      //*************************************************************************
      /// Constructor.
      //*************************************************************************
      debounce_value_base(value_type initial_value)
        : value(initial_value)
        , last_value(initial_value)
        , count(0)
        , state(Unstable)
        , notification(nullptr)
      {
      }

      //*************************************************************************
      /// Adds a new value.
      /// If the value has changed, the counter is reset and the value is marked as unstable.
      /// Returns 'true' if the value is stable.
      //*************************************************************************
      bool add(value_type value, count_type stable_count)
      {
        // Changed from last time?
        if (value != last_value)
        {
          count = 1;
          state = Unstable;
        }
        else
        {
          // Increment the counter if we haven't reached the stable count yet.
          if (count < stable_count)
          {
            ++count;

            // Check if we've reached the stable count.
            if (count == stable_count)
            {
              state = static_cast<State>(Stable | Changed);

              if (notification)
              {
                notification->on_debounce_value_notification(value);
              }
            }
          }
          else
          {
            // If the counter has already reached the stable count then we are already stable, but no further change has occurred.
            state = Stable;
          }
        }

        last_value = value;

        return is_stable();
      }

      //*************************************************************************
      /// Sets the notification interface.
      ///\param notification_ The notification interface.
      //*************************************************************************
      void set_notification(notification_type& notification_)
      {
        notification = &notification_;
      }

      //*************************************************************************
      /// Clears the notification interface.
      //*************************************************************************
      void clear_notification()
      {
        notification = nullptr;
      }

      //*************************************************************************
      /// Gets the current value.
      //*************************************************************************
      value_type get() const
      {
        return value;
      }

      //*************************************************************************
      /// Gets the current change state.
      ///\return 'true' if the value has changed.
      //*************************************************************************
      bool has_changed() const
      {
        return (state & Changed) != 0;
      }

      //*************************************************************************
      /// Gets the current stable state.
      ///\return 'true' if the value is stable.
      //*************************************************************************
      bool is_stable() const
      {
        return (state & Stable) != 0;
      }

    protected:

      enum State
      {
        Unstable = 0,
        Stable   = 1,
        Changed  = 2
      };

      value_type         value;
      value_type         last_value;
      count_type         count;
      notification_type* notification;
      State              state;
    };
  } // namespace private_debounce_value

  template <typename TValue, typename TCount, typename TTag>
  class debounce_value;

  //*************************************************************************
  /// A debounced for a value.
  /// The debounced will only change its value if the new value has been stable for a given number of calls.
  /// Compile-time stable count.
  //*************************************************************************
  template <typename TValue, typename TCount, TCount Stable_Count>
  class debounce_value<TValue, TCount, etl::integral_constant<TCount, Stable_Count>> // (2) compile-time specialization
    : public private_debounce_value::debounce_value_base<TValue, TCount>
  {
  public:

    using value_type = typename private_debounce_value::debounce_value_base<TValue, TCount>::value_type;
    using count_type = typename private_debounce_value::debounce_value_base<TValue, TCount>::count_type;

    //*************************************************************************
    /// Constructor.
    //*************************************************************************
    explicit debounce_value(value_type initial_value)
      : private_debounce_value::debounce_value_base<TValue, TCount>(initial_value)
    {
    }

    //*************************************************************************
    /// Adds a new value to the debounced.
    /// If the value has changed, the counter is reset and the debounced is marked as unstable.
    //*************************************************************************
    bool add(value_type value)
    {
      return private_debounce_value::debounce_value_base<TValue, TCount>::add(value, Stable_Count);
    }
  };

  //*************************************************************************
  /// A debounced for a value.
  /// The debounced will only change its value if the new value has been stable for a given number of calls.
  /// Runtime stable count.
  //*************************************************************************
  struct debounce_runtime_tag
  {
  };

  template <typename TValue, typename TCount = uint16_t, typename TTag = debounce_runtime_tag>
  class debounce_value : public private_debounce_value::debounce_value_base<TValue, TCount>
  {
  public:

    using value_type = typename private_debounce_value::debounce_value_base<TValue, TCount>::value_type;
    using count_type = typename private_debounce_value::debounce_value_base<TValue, TCount>::count_type;

    //*************************************************************************
    /// Constructor.
    //*************************************************************************
    explicit debounce_value(value_type initial_value, count_type stable_count)
      : private_debounce_value::debounce_value_base<TValue, TCount>(initial_value)
      , stable_count(stable_count)
    {
    }

    //*************************************************************************
    /// Adds a new value to the debounced.
    /// If the value has changed, the counter is reset and the debounced is marked as unstable.
    //*************************************************************************
    bool add(value_type value)
    {
      return private_debounce_value::debounce_value_base<TValue, TCount>::add(value, stable_count);
    }

    //*************************************************************************
    /// Sets the stable count.
    //*************************************************************************
    void set_stable_count(count_type stable_count_)
    {
      stable_count = stable_count_;
    }

  private:

    count_type stable_count;
  };
} // namespace etl

#endif
