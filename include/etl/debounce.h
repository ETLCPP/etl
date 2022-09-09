///\file

/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl
https://www.etlcpp.com

Copyright(c) 2016 John Wellbelove

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

#ifndef ETL_DEBOUNCE_INCLUDED
#define ETL_DEBOUNCE_INCLUDED

#include "platform.h"
#include "static_assert.h"

#include <stdint.h>

namespace etl
{
  namespace private_debounce
  {
    class debounce_base
    {
    public:

      typedef uint_least8_t flags_t;
      typedef uint16_t      count_t;

      //*************************************************************************
      /// Adds the new sample and clears the state change flag.
      /// If the sample has changed then the counter is reset.
      /// The last sample state is stored as a bit in the flags.
      //*************************************************************************
      void add_sample(bool sample)
      {
        // Changed from last time?
        if (sample != bool((flags & Sample) != 0))
        {
          count = 0;
          flags = (flags & ~Sample) | (sample ? Sample : 0);
        }

        flags &= ~Change;
      }

      //*************************************************************************
      /// Gets the current debouncer change state.
      ///\return 'true' if the debouncer has changed state.
      //*************************************************************************
      bool has_changed() const
      {
        return (flags & Change) != 0;
      }

      //*************************************************************************
      /// Gets the current debouncer state.
      ///\return 'true' if the debouncer is in the true state.
      //*************************************************************************
      bool is_set() const
      {
        return ((flags & State) > Off);
      }

      //*************************************************************************
      /// Gets the debouncer hold state.
      ///\return 'true' if the debouncer is in the hold state.
      //*************************************************************************
      bool is_held() const
      {
        return (flags & State) > On;
      }

      //*************************************************************************
      /// Gets the debouncer repeat state.
      ///\return 'true' if the debouncer is repeating.
      //*************************************************************************
      bool is_repeating() const
      {
        return ((flags & State) == Repeating);
      }

    protected:

      enum states
      {
        Off       = 0,
        On        = 1,
        Held      = 2,
        Repeating = 3,
        State     = 0x03U,
        Sample    = 4,
        Change    = 8
      };

      //*************************************************************************
      /// Constructor.
      //*************************************************************************
      debounce_base(bool initial_state)
        : flags(initial_state ? On : Off),
          count(0)
      {
      }

      //*************************************************************************
      /// Destructor.
      //*************************************************************************
      ~debounce_base()
      {
      }

      //*************************************************************************
      /// Gets the next state based on the inputs.
      //*************************************************************************
      void get_next(bool sample, bool condition_set, bool condition_clear, const uint_least8_t state_table[][2])
      {
        int index1 = ((flags & State) * 2) + (sample ? 1 : 0);
        int index2 = (sample ? (condition_set ? 0 : 1) : (condition_clear ? 0 : 1));

        flags_t next = flags;

        next &= ~State;
        next |= state_table[index1][index2];

        if (next != flags)
        {
          next |= Change;
        }
        else
        {
          next &= ~Change;
        }

        flags = next;
      }

      flags_t flags;
      count_t count;
    };

    //***************************************************************************
    /// State change logic for 2 state debounce.
    //***************************************************************************
    class debounce2 : public debounce_base
    {
    protected:

      debounce2(bool initial_state)
        : debounce_base(initial_state)
      {
      }

      //*************************************************************************
      /// Destructor.
      //*************************************************************************
      ~debounce2()
      {
      }

      //*************************************************************************
      ///
      //*************************************************************************
      void set_state(bool sample, bool condition_set, bool condition_clear)
      {
        static ETL_CONSTANT uint_least8_t state_table[4][2] =
        {
          /* Off 0 */{ debounce_base::Off, debounce_base::Off },
          /* Off 1 */{ debounce_base::On,  debounce_base::Off },
          /* On  0 */{ debounce_base::Off, debounce_base::On },
          /* On  1 */{ debounce_base::On,  debounce_base::On },
        };

        get_next(sample, condition_set, condition_clear, state_table);
      }

      //*************************************************************************
      ///
      //*************************************************************************
      bool process(bool sample, count_t valid_count)
      {
        add_sample(sample);

        if (count < UINT16_MAX)
        {
          ++count;

          bool valid = (count == valid_count);

          switch (flags & State)
          {
            case Off:
            {
              set_state(sample, valid, valid);
              break;
            }

            case On:
            {
              set_state(sample, valid, valid);
              break;
            }

            default:
            {
              break;
            }
          }
        }

        if (flags & Change)
        {
          count = 0;
        }

        return (flags & Change);
      }
    };

    //***************************************************************************
    /// State change logic for 3 state debounce.
    //***************************************************************************
    class debounce3 : public debounce_base
    {
    protected:

      debounce3(bool initial_state)
        : debounce_base(initial_state)
      {
      }

      //*************************************************************************
      /// Destructor.
      //*************************************************************************
      ~debounce3()
      {
      }

      //*************************************************************************
      ///
      //*************************************************************************
      void set_state(bool sample, bool condition_set, bool condition_clear)
      {
        static ETL_CONSTANT uint_least8_t state_table[6][2] =
        {
          /* Off  0 */{ debounce_base::Off,  debounce_base::Off },
          /* Off  1 */{ debounce_base::On,   debounce_base::Off },
          /* On   0 */{ debounce_base::Off,  debounce_base::On },
          /* On   1 */{ debounce_base::Held, debounce_base::On },
          /* Held 0 */{ debounce_base::Off,  debounce_base::Held },
          /* Held 1 */{ debounce_base::Held, debounce_base::Held }
        };

        get_next(sample, condition_set, condition_clear, state_table);
      }

      //*************************************************************************
      ///
      //*************************************************************************
      bool process(bool sample, count_t valid_count, count_t hold_count)
      {
        add_sample(sample);

        if (count < UINT16_MAX)
        {
          ++count;

          bool valid = (count == valid_count);
          bool hold  = (count == hold_count);

          switch (flags & State)
          {
            case Off:
            {
              set_state(sample, valid, valid);
              break;
            }

            case On:
            {
              set_state(sample, hold, valid);
              break;
            }

            case Held:
            {
              set_state(sample, hold, valid);
              break;
            }

            default:
            {
              break;
            }
          }
        }

        if (flags & Change)
        {
          count = 0;
        }

        return (flags & Change);
      }
    };

    //***************************************************************************
    /// State change logic for 4 state debounce.
    //***************************************************************************
    class debounce4 : public debounce_base
    {
    protected:

      debounce4(bool initial_state)
        : debounce_base(initial_state)
      {
      }

      //*************************************************************************
      /// Destructor.
      //*************************************************************************
      ~debounce4()
      {
      }

      //*************************************************************************
      ///
      //*************************************************************************
      void set_state(bool sample, bool condition_set, bool condition_clear)
      {
        static ETL_CONSTANT uint_least8_t state_table[8][2] =
        {
          /* Off       0 */{ debounce_base::Off,       debounce_base::Off },
          /* Off       1 */{ debounce_base::On,        debounce_base::Off },
          /* On        0 */{ debounce_base::Off,       debounce_base::On },
          /* On        1 */{ debounce_base::Held,      debounce_base::On },
          /* Held      0 */{ debounce_base::Off,       debounce_base::Held },
          /* Held      1 */{ debounce_base::Repeating, debounce_base::Held },
          /* Repeating 0 */{ debounce_base::Off,       debounce_base::Repeating },
          /* Repeating 1 */{ debounce_base::Repeating, debounce_base::Repeating }
        };

        get_next(sample, condition_set, condition_clear, state_table);
      }

      //*************************************************************************
      ///
      //*************************************************************************
      bool process(bool sample, count_t valid_count, count_t hold_count, count_t repeat_count)
      {
        add_sample(sample);

        if (count < UINT16_MAX)
        {
          ++count;

          bool valid  = (count == valid_count);
          bool hold   = (count == hold_count);
          bool repeat = (count == repeat_count);

          switch (flags & State)
          {
            case Off:
            {
              set_state(sample, valid, valid);
              break;
            }

            case On:
            {
              set_state(sample, hold, valid);
              break;
            }

            case Held:
            {
              set_state(sample, repeat, valid);
              break;
            }

            case Repeating:
            {
              set_state(sample, repeat, valid);

              if (sample && repeat)
              {
                count = 0;
                flags |= Change;
              }
              break;
            }

            default:
            {
              break;
            }
          }
        }

        if (flags & Change)
        {
          count = 0;
        }

        return (flags & Change);
      }
    };
  }

  //***************************************************************************
  /// A class to debounce signals.
  /// Fixed Valid/Hold/Repeating values.
  //***************************************************************************
  template <const uint16_t VALID_COUNT = 0, const uint16_t HOLD_COUNT = 0, const uint16_t REPEAT_COUNT = 0>
  class debounce : public private_debounce::debounce4
  {
  public:

    //*************************************************************************
    /// Constructor.
    //*************************************************************************
    debounce(bool initial_state = false)
      : debounce4(initial_state)
    {
    }

    //*************************************************************************
    /// Adds a new sample.
    /// Returns 'true' if the debouncer changes state.
    ///\param sample The new sample.
    ///\return 'true' if the debouncer changed state.
    //*************************************************************************
    bool add(bool sample)
    {
      return process(sample, VALID_COUNT, HOLD_COUNT, REPEAT_COUNT);
    }
  };

  //***************************************************************************
  /// A class to debounce signals.
  /// Fixed Valid/Hold values.
  //***************************************************************************
  template <const uint16_t VALID_COUNT, const uint16_t HOLD_COUNT>
  class debounce<VALID_COUNT, HOLD_COUNT, 0> : public private_debounce::debounce3
  {
  public:

    //*************************************************************************
    /// Constructor.
    //*************************************************************************
    debounce(bool initial_state = false)
      : debounce3(initial_state)
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
      return process(sample, VALID_COUNT, HOLD_COUNT);
    }
  };

  //***************************************************************************
  /// A class to debounce signals.
  /// Fixed Valid value.
  //***************************************************************************
  template <const uint16_t VALID_COUNT>
  class debounce<VALID_COUNT, 0, 0> : public private_debounce::debounce2
  {
  public:

    //*************************************************************************
    /// Constructor.
    //*************************************************************************
    debounce(bool initial_state = false)
      : debounce2(initial_state)
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
      return process(sample, VALID_COUNT);
    }
  };

  //***************************************************************************
  /// A class to debounce signals.
  /// Variable Valid/Hold/Repeating values.
  //***************************************************************************
  template <>
  class debounce<0, 0, 0> : public private_debounce::debounce4
  {
  public:

    //*************************************************************************
    /// Constructor.
    ///\param initial_state The initial state. Default = false.
    //*************************************************************************
    debounce(bool initial_state = false)
      : debounce4(initial_state),
        valid_count(1),
        hold_count(0),
        repeat_count(0)
    {
    }

    //*************************************************************************
    /// Constructor.
    ///\param valid_count   The count for a valid state..
    ///\param hold_count    The count after valid_count for a hold state. Default = 0.
    ///\param repeat_count  The count after hold_count for a key repeat. Default = 0.
    //*************************************************************************
    debounce(count_t valid, count_t hold = 0, count_t repeat = 0)
      : debounce4(false)
    {
      set(valid, hold, repeat);
    }

    //*************************************************************************
    /// Constructor.
    //*************************************************************************
    void set(count_t valid, count_t hold = 0, count_t repeat = 0)
    {
      valid_count  = valid;
      hold_count   = hold;
      repeat_count = repeat;
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
      return process(sample, valid_count, hold_count, repeat_count);
    }

  private:

    count_t valid_count;
    count_t hold_count;
    count_t repeat_count;
  };
}

#endif
