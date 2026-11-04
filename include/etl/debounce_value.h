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
  //***************************************************************************
  /// This class is designed to take an input value that may take time to become stable,
  /// such as a binary rotary encoder or DIP switches, and indicate when it has settled to a stable state.
  /// TValue : The type of the value being debounced.
  /// TCount : The type of the counter used to determine stability.
  //***************************************************************************
  template <typename TValue, typename TCount = uint16_t>
  class debounce_value
  {
  public:

    typedef TValue value_type;
    typedef TCount count_type;

    //*************************************************************************
    /// Constructor.
    //*************************************************************************
    debounce_value(value_type initial_value_, count_type stable_count_)
      : last_value(initial_value_)
      , count(0)
      , stable_count(stable_count_)
      , state(Unstable)
    {
    }

    //*************************************************************************
    /// Adds a new value.
    /// If the value has changed, the counter is reset and the value is marked as unstable.
    /// Returns 'true' if the value has changed.
    //*************************************************************************
    bool add(value_type value)
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

          // Have we reached the stable count?
          if (count == stable_count)
          {
            state = static_cast<State>(Stable | Changed);
          }
        }
        else
        {
          // If the counter has already reached the stable count then we are already stable, but no further change has occurred.
          state = Stable;
        }
      }

      last_value = value;

      return has_changed();
    }

    //*************************************************************************
    /// Gets the current value.
    //*************************************************************************
    value_type get() const
    {
      return last_value;
    }

    //*************************************************************************
    /// Conversion operator.
    //*************************************************************************
    operator value_type() const
    {
      return last_value;
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

    //*************************************************************************
    /// Sets the stable count.
    //*************************************************************************
    void set_stable_count(count_type stable_count_)
    {
      stable_count = stable_count_;
    }

  private:

    enum State
    {
      Unstable = 0,
      Stable   = 1,
      Changed  = 2
    };

    value_type last_value;
    count_type count;
    count_type stable_count;
    State      state;
  };
} // namespace etl

#endif
