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

#ifndef DEBOUNCE_MULTI_BIT_INCLUDED
#define DEBOUNCE_MULTI_BIT_INCLUDED

//-----------------------------------------------------------------------------
// debounce_multi_bit
//
// Debounces a contiguous group of bits read from an IO port. Each bit is
// debounced independently, using the same semantics as etl::debounce:
//
//   Valid_Count  : Number of consecutive samples a bit must differ from its
//                  previous debounced state before the state changes (>= 1).
//   Hold_Count   : Number of samples a bit must remain set (after becoming
//                  valid) before a 'held' event is generated. 0 = disabled.
//   Repeat_Count : Once held, a 'repeat' event is generated every
//                  Repeat_Count samples. 0 = disabled.
//
// TValue  : unsigned integer type of the port (uint8_t/16_t/32_t/64_t).
// Shift   : Position of the lowest bit of the field within the port value.
// Width   : Number of contiguous bits in the field.
// TCount  : Type of the per-bit counters. Must be able to hold the largest of
//           Valid_Count, Hold_Count and Repeat_Count.
//
// All masks returned by this class are field-aligned, i.e. bit 0 of a mask
// corresponds to the lowest bit of the field (port bit 'Shift').
//
// Call update() at a fixed sampling interval (e.g. every 1ms from a timer).
//-----------------------------------------------------------------------------

#include "platform.h"
#include "binary.h"
#include "memory.h"
#include "static_assert.h"

#include <stddef.h>
#include <stdint.h>

namespace etl
{
  //*************************************************************************
  /// Debounces a contiguous group of bits read from an IO port.
  ///\tparam TValue      The type of the port value (uint8_t/16_t/32_t/64_t).
  ///\tparam Width       The number of bits in the field (1..sizeof(TValue) * 8).
  ///\tparam Valid_Count The number of consecutive samples a bit must differ
  ///                    from its previous debounced state before the state
  ///                    changes (>= 1).
  ///\tparam Hold_Count  The number of samples a bit must remain set (after
  ///                    becoming valid) before a 'held' event is generated.
  ///                    0 = disabled.
  ///\tparam Repeat_Count Once held, a 'repeat' event is generated every
  ///                     Repeat_Count samples. 0 = disabled.
  ///\tparam TCount      The type of the per-bit counters. Must be able to hold
  ///                    the largest of Valid_Count, Hold_Count and Repeat_Count.
  //*************************************************************************
  template <typename TValue, typename TCount, size_t Width, uint32_t Valid_Count, uint32_t Hold_Count = 0, uint32_t Repeat_Count = 0>
  class debounce_multi_bit
  {
  public:

    ETL_STATIC_ASSERT(std::is_integral<TValue>::value&& std::is_unsigned<TValue>::value, "TValue must be an unsigned integer type");
    ETL_STATIC_ASSERT(Width >= 1, "Width must be at least 1");
    ETL_STATIC_ASSERT(Width <= sizeof(TValue) * 8, "Field does not fit in the value type");
    ETL_STATIC_ASSERT(Valid_Count >= 1, "Valid_Count must be at least 1");
    ETL_STATIC_ASSERT(Valid_Count <= static_cast<TCount>(~TCount(0)) && Hold_Count <= static_cast<TCount>(~TCount(0))
                        && Repeat_Count <= static_cast<TCount>(~TCount(0)),
                      "TCount is too small for the configured counts");

    typedef TValue        value_type;
    typedef TCount        count_type;
    typedef uint_least8_t state_type;

    //***************************************************************************
    /// Constructor. All bits start in the 'clear' state.
    //***************************************************************************
    debounce_multi_bit(value_type initial = 0)
    {
      reset(initial);
    }

    //***************************************************************************
    /// Resets the debouncer state.
    //***************************************************************************
    void reset(value_type initial = 0)
    {
      sample   = initial;
      state_lo = 0;
      state_hi = 0;
      changed  = 0;

      // Clear the timers for all bits.
      etl::fill(ETL_OR_STD::begin(counts), ETL_OR_STD::end(counts), 0);
    }

    //***************************************************************************
    /// Adds a new next of the whole port value.
    /// Returns true if any bit changed state, became held, or repeated.
    //***************************************************************************
    void update(const value_type next)
    {
      value_type changed_bits = sample ^ next;
      sample                  = next;

      changed = 0;

      count_type* pcount = &counts[0];
      value_type  bit    = 1U;
      pcount             = &counts[0];

      for (size_t i = 0; i < Width; ++i)
      {
        bool bit_is_set      = (sample & bit) != 0;
        bool bit_has_changed = (changed_bits & bit) != 0;

        if (bit_has_changed)
        {
          // Reset the timer for this bit.
          *pcount = 0;
        }

        switch (get_state(bit))
        {
          case Off:
            {
              if (bit_is_set)
              {
                ++(*pcount);

                // If we have reached the 'Valid_Count' move to the On state.
                if (*pcount >= Valid_Count)
                {
                  set_state(On, bit);
                  changed |= bit;
                  *pcount = 0;
                }
              }
            }
            break;

          case On:
            {
              if (Hold_Count > 0)
              {
                ++(*pcount);

                if (bit_is_set)
                {
                  // If we have reached the 'Hold_Count', and hold is enabled, we move to the Held state.
                  if (*pcount >= Hold_Count)
                  {
                    set_state(Held, bit);
                    changed |= bit;
                    *pcount = 0;
                  }
                }
                else
                {
                  // If we have reached the 'Valid_Count' we move back to the Off state.
                  if (*pcount >= Valid_Count)
                  {
                    set_state(Off, bit);
                    changed |= bit;
                    *pcount = 0;
                  }
                }
              }
            }
            break;

          case Held:
            {
              if (Repeat_Count > 0)
              {
                ++(*pcount);

                if (bit_is_set)
                {
                  // If we have reached the 'repeat count', and repeat is enabled, we move to the Repeating state.
                  if (*pcount >= Repeat_Count)
                  {
                    set_state(Repeating, bit);
                    changed |= bit;
                    *pcount = 0;
                  }
                }
                else
                {
                  // If we have reached the 'Valid_Count' we move back to the Off state.
                  if (*pcount >= Valid_Count)
                  {
                    set_state(Off, bit);
                    changed |= bit;
                    *pcount = 0;
                  }
                }
              }
            }
            break;

          case Repeating:
            {
              ++(*pcount);

              if (bit_is_set)
              {
                // If we have reached the 'Repeat_Count', and repeat is enabled, we set the changed flag again.
                if (*pcount >= Repeat_Count)
                {
                  changed |= bit;
                  *pcount = 0;
                }
              }
              else
              {
                // If we have reached the 'Valid_Count' we move back to the Off state.
                if (*pcount >= Valid_Count)
                {
                  set_state(Off, bit);
                  changed |= bit;
                  *pcount = 0;
                }
              }
            }
            break;
        }

        // changed_bits = changed_bits >> 1U; // Move to the next bit.
        bit = bit << 1U; // Move to the next bit.
        ++pcount;        // Move to the next bit's timer.
      }
    }

    //***************************************************************************
    /// Returns true if the bit changed state on the last update.
    ///\param n The bit number (0..Width - 1).
    //***************************************************************************
    bool has_changed(size_t n) const
    {
      value_type bit = bit_mask(n);

      return (changed & bit) != 0;
    }

    //***************************************************************************
    /// Returns true if the bit is currently set.
    ///\param n The bit number (0..Width - 1).
    //***************************************************************************
    bool is_set(size_t n) const
    {
      value_type bit   = bit_mask(n);
      state_type state = get_state(bit);

      return (state == On) || (state == Held) || (state == Repeating);
    }

    //***************************************************************************
    /// Returns true if the bit is currently held.
    ///\param n The bit number (0..Width - 1).
    //***************************************************************************
    bool is_held(size_t n) const
    {
      value_type bit   = bit_mask(n);
      state_type state = get_state(bit);

      return (state == Held) || (state == Repeating);
    }

    //***************************************************************************
    /// Returns true if the bit is currently repeating.
    ///\param n The bit number (0..Width - 1).
    //***************************************************************************
    bool is_repeating(size_t n) const
    {
      value_type bit   = bit_mask(n);
      state_type state = get_state(bit);

      return (state == Repeating);
    }

  private:

    //***************************************************************************
    /// State fit in 2 bits, state_lo & state_hi.
    //***************************************************************************
    enum state
    {
      Off       = 0, // Debounced state is Off.
      On        = 1, // Debounced state is On.
      Held      = 2, // Debounced state is Held.
      Repeating = 3  // Debounced state is Repeating.
    };

    //***************************************************************************
    /// Gets the current debounce state for a specific bit.
    ///\return The current state of the bit.
    //***************************************************************************
    state_type get_state(const value_type bit) const
    {
      state_type lo = static_cast<state_type>((state_lo & bit) > 0);
      state_type hi = static_cast<state_type>((state_hi & bit) > 0);

      return static_cast<state_type>(lo | (hi << 1));
    }

    //***************************************************************************
    /// Gets the current debounce state for a specific bit.
    ///\return The current state of the bit.
    //***************************************************************************
    void set_state(state_type state, const value_type bit)
    {
      state_lo = state & 1 ? state_lo | bit : state_lo & ~bit;
      state_hi = state & 2 ? state_hi | bit : state_hi & ~bit;
    }

    //*************************************************************************
    /// Returns a mask with only the nth bit set.
    //*****************************************************************************
    static value_type bit_mask(size_t n)
    {
      return static_cast<value_type>(value_type(1) << n);
    }

    value_type sample;   // The last known state of the bits.
    value_type state_lo; // Low bits of the state.
    value_type state_hi; // High bits of the state.
    value_type changed;  // Bits that have changed at the last update.

    count_type counts[Width]; // Off / On / Hold / Repeat timers.
  };
} // namespace etl

#endif
