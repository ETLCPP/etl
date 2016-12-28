///\file

/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl
http://www.etlcpp.com

Copyright(c) 2016 jwellbelove

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

#ifndef __ETL_DEBOUNCE__
#define __ETL_DEBOUNCE__

#include <stdint.h>

#include "static_assert.h"

namespace etl
{
  namespace __private_debounce__
  {
    class debounce_base
    {
    public:

      void add(bool sample)
      {
        state &= ~CHANGED;

        // Changed from last time?
        if (sample != bool((state & LAST) != 0))
        {
          count = START_COUNT;
        }
      }

      //*************************************************************************
      /// Gets the current debouncer change state.
      ///\return 'true' if the debouncer has changed state.
      //*************************************************************************
      bool has_changed() const
      {
        return (state & CHANGED) != 0;
      }

      //*************************************************************************
      /// Gets the current debouncer state.
      ///\return 'true' if the debouncer is in the true state.
      //*************************************************************************
      bool is_set() const
      {
        return (state & CURRENT) != 0;
      }

      //*************************************************************************
      /// Gets the debouncer hold state.
      ///\return 'true' if the debouncer is in the hold state.
      //*************************************************************************
      bool is_held() const
      {
        return (state & HELD) != 0;
      }

      //*************************************************************************
      /// Gets the debouncer repeat state.
      ///\return 'true' if the debouncer is repeating.
      //*************************************************************************
      bool is_repeating() const
      {
        return (state & REPEATING) != 0;
      }

    protected:

      //*************************************************************************
      /// Constructor.
      ///\param initial_state The initial state. Default = false.
      //*************************************************************************
      debounce_base(bool initial_state = false)
        : state(initial_state ? (CURRENT | LAST) : 0),
          count(START_COUNT)
      {
      }

      enum
      {
        START_COUNT = 0
      };

      enum
      {
        CURRENT   = 1,
        LAST      = 2,
        HELD      = 4,
        CHANGED   = 8,
        REPEATING = 16
      };

      uint_least8_t state;

      /// The state count.
      uint16_t count;
    };
  }

  //***************************************************************************
  /// A class to debounce signals.
  /// The state is decided over N samples, defined by the VALID_COUNT value.
  /// If the samples are consistent for VALID_COUNT times then the debouncer state is defined.
  /// If the samples change then the debouncer will change state after VALID_COUNT samples.
  /// If the samples are true for a count of HOLD_COUNT then the debouncer input is 'held'.
  /// The debouncer may be constructed in either state.
  //***************************************************************************
  template <const uint16_t VALID_COUNT = 0, const uint16_t HOLD_COUNT = 0, const uint16_t REPEAT_COUNT = 0>
  class debounce : public __private_debounce__::debounce_base
  {
  private:

      enum
      {
        VALID_THRESHOLD  = VALID_COUNT,
        HOLD_THRESHOLD   = VALID_THRESHOLD + HOLD_COUNT,
        REPEAT_THRESHOLD = HOLD_THRESHOLD + REPEAT_COUNT
      };

      using debounce_base::add;

  public:

    //*************************************************************************
    /// Constructor.
    ///\param initial_state The initial state. Default = false.
    //*************************************************************************
    debounce(bool initial_state = false)
      : debounce_base(initial_state)
    {
    }

    //*************************************************************************
    /// Adds a new sample.
    /// Returns 'true' if the debouncer changes state from...
    /// 1. Clear to Set.
    /// 2. Set to Clear.
    /// 3. Not Held to Held.
    /// 4. Key repeats.
    ///\param sample The new sample.
    ///\return 'true' if the debouncer changed state.
    //*************************************************************************
    bool add(bool sample)
    {
      debounce_base::add(sample);

      if (count < REPEAT_THRESHOLD)
      {
        ++count;

        if (sample)
        {
          if (count == VALID_THRESHOLD)
          {
            // Set.
            state |= CHANGED;
            state |= CURRENT;
          }
          else if (count == HOLD_THRESHOLD)
          {
            // Held.
            state |= CHANGED;
            state |= HELD;
          }
          else if (count == REPEAT_THRESHOLD)
          {
            // Repeat.
            state |= CHANGED;
            state |= REPEATING;
            count = HOLD_THRESHOLD;
          }

          state |= LAST;        
        }
        else
        {
          if (count == VALID_THRESHOLD)
          {
            // Clear.
            state |= CHANGED;
            state &= ~CURRENT;
            state &= ~HELD;
            state &= ~REPEATING;
          }

          state &= ~LAST;
        }
      }

      return (state & CHANGED) != 0;
    }
  };
  
  template <const uint16_t VALID_COUNT, const uint16_t HOLD_COUNT>
  class debounce<VALID_COUNT, HOLD_COUNT, 0> : public __private_debounce__::debounce_base
  {
  private:

    enum
    {
      VALID_THRESHOLD = VALID_COUNT,
      HOLD_THRESHOLD  = VALID_THRESHOLD + HOLD_COUNT
    };

    using debounce_base::add;

  public:   

    //*************************************************************************
    /// Constructor.
    ///\param initial_state The initial state. Default = false.
    //*************************************************************************
    debounce(bool initial_state = false)
      : debounce_base(initial_state)
    {
    }

    //*************************************************************************
    /// Adds a new sample.
    /// Returns 'true' if the debouncer changes state from...
    /// 1. Clear to Set.
    /// 2. Set to Clear.
    /// 3. Not Held to Held.
    ///\param sample The new sample.
    ///\return 'true' if the debouncer changed state.
    //*************************************************************************
    bool add(bool sample)
    {
      debounce_base::add(sample);

      if (count < HOLD_THRESHOLD)
      {
        ++count;

        if (sample)
        {
          if (count == VALID_THRESHOLD)
          {
            // Set.
            state |= CHANGED;
            state |= CURRENT;
          }
          else if (count == HOLD_THRESHOLD)
          {
            // Held.
            state |= CHANGED;
            state |= HELD;
          }

          state |= LAST;
        }
        else
        {
          if (count == VALID_THRESHOLD)
          {
            // Clear.
            state |= CHANGED;
            state &= ~CURRENT;
            state &= ~HELD;
            state &= ~REPEATING;
          }

          state &= ~LAST;
        }
      }

      return (state & CHANGED) != 0;
    }
  };

  template <const uint16_t VALID_COUNT>
  class debounce<VALID_COUNT, 0, 0> : public __private_debounce__::debounce_base
  {
  private:

    enum
    {
      VALID_THRESHOLD = VALID_COUNT
    };
    
    using debounce_base::add;

  public:

    //*************************************************************************
    /// Constructor.
    ///\param initial_state The initial state. Default = false.
    //*************************************************************************
    debounce(bool initial_state = false)
      : debounce_base(initial_state)
    {
    }

    //*************************************************************************
    /// Adds a new sample.
    /// Returns 'true' if the debouncer changes state from...
    /// 1. Clear to Set.
    /// 2. Set to Clear.
    ///\param sample The new sample.
    ///\return 'true' if the debouncer changed state.
    //*************************************************************************
    bool add(bool sample)
    {
      debounce_base::add(sample);

      if (count < VALID_THRESHOLD)
      {
        ++count;

        if (sample)
        {
          if (count == VALID_THRESHOLD)
          {
            // Set.
            state |= CHANGED;
            state |= CURRENT;
          }

          state |= LAST;
        }
        else
        {
          if (count == VALID_THRESHOLD)
          {
            // Clear.
            state |= CHANGED;
            state &= ~CURRENT;
            state &= ~HELD;
            state &= ~REPEATING;
          }

          state &= ~LAST;
        }
      }

      return (state & CHANGED) != 0;
    }
  };

  template <>
  class debounce<0, 0, 0> : public __private_debounce__::debounce_base
  {
  public:

    using debounce_base::add;

    //*************************************************************************
    /// Constructor.
    ///\param initial_state The initial state. Default = false.
    //*************************************************************************
    debounce()
      : debounce_base(false),
        valid_threshold(1),
        hold_threshold(0),
        repeat_threshold(0)
    {
    }

    //*************************************************************************
    /// Constructor.
    ///\param initial_state The initial state.
    ///\param valid_count   The count for a valid state. Default = 1.
    ///\param hold_count    The count after valid_count for a hold state. Default = 0.
    ///\param repeat_count  The count after hold_count for a key repeat. Default = 0.
    //*************************************************************************
    debounce(bool initial_state, uint16_t valid_count = 1, uint16_t hold_count = 0, uint16_t repeat_count = 0)
      : debounce_base(initial_state)
    {
      set(valid_count, hold_count, repeat_count);
    }

    //*************************************************************************
    /// Constructor.
    ///\param initial_state The initial state. Default = false.
    //*************************************************************************
    void set(uint16_t valid_count, uint16_t hold_count = 0, uint16_t repeat_count = 0)
    {
      valid_threshold  = valid_count;
      hold_threshold   = valid_threshold + hold_count;
      repeat_threshold = hold_threshold + repeat_count;
    }

    //*************************************************************************
    /// Adds a new sample.
    /// Returns 'true' if the debouncer changes state from...
    /// 1. Clear to Set.
    /// 2. Set to Clear.
    /// 3. Not Held to Held.
    /// 4. Key repeats.
    ///\param sample The new sample.
    ///\return 'true' if the debouncer changed state.
    //*************************************************************************
    bool add(bool sample)
    {
      debounce_base::add(sample);

      if (count < repeat_threshold)
      {
        ++count;

        if (sample)
        {
          if (count == valid_threshold)
          {
            if (sample)
            {
              // Set.
              state |= CHANGED;
              state |= CURRENT;
            }
          }

          if (hold_threshold != valid_threshold)
          {
            if ((count == hold_threshold) && sample)
            {
              // Held.
              state |= CHANGED;
              state |= HELD;
            }
          }

          if (repeat_threshold != hold_threshold)
          {
            if ((count == repeat_threshold) && sample)
            {
              // Repeat.
              state |= CHANGED;
              state |= REPEATING;
              count = hold_threshold;
            }
          }

          state |= LAST;
        }
        else
        {
          if (count == valid_threshold)
          {
            // Clear.
            state |= CHANGED;
            state &= ~CURRENT;
            state &= ~HELD;
            state &= ~REPEATING;
          }

          state &= ~LAST;
        }
      }

      return (state & CHANGED) != 0;
    }

  private:

    uint16_t valid_threshold;
    uint16_t hold_threshold;
    uint16_t repeat_threshold;
  };
}

#endif
