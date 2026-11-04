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

    // clang-format off
    ETL_STATIC_ASSERT(etl::is_integral<TValue>::value && etl::is_unsigned<TValue>::value, "TValue must be an unsigned integer type");
    ETL_STATIC_ASSERT(Width >= 1,                                                         "Width must be at least 1");
    ETL_STATIC_ASSERT(Width <= sizeof(TValue) * 8,                                        "The width does not fit in the value type");
    ETL_STATIC_ASSERT(Valid_Count >= 1,                                                   "Valid_Count must be at least 1");
    ETL_STATIC_ASSERT((Valid_Count <= etl::integral_limits<TCount>::max) &&
                      (Hold_Count <= etl::integral_limits<TCount>::max) &&
                      (Repeat_Count <= etl::integral_limits<TCount>::max),                "TCount is too small for the configured counts");
    ETL_STATIC_ASSERT((Repeat_Count == 0) || (Hold_Count != 0),                           "If Repeat_Count is non-zero, Hold_Count must be non-zero");
    // clang-format on

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
    bool update(const value_type next)
    {
      // Determine which bits have changed since the last sample.
      const value_type changed_bits = sample ^ next;

      sample  = next;
      changed = 0;

      // The initial loop conditions.
      count_type* pcount = &counts[0]; // Pointer to the first bit's timer.
      value_type  bit    = 1U;         // Mask for the first bit in the field.

      for (size_t i = 0; i < Width; ++i)
      {
        const bool bit_is_set      = (sample & bit) != 0;
        const bool bit_has_changed = (changed_bits & bit) != 0;

        if (bit_has_changed)
        {
          *pcount = 0;
        }

        switch (get_state(bit))
        {
          case Off:
            {
              handle_off(pcount, bit, bit_is_set);
              break;
            }

          case On:
            {
              handle_on(pcount, bit, bit_is_set);
              break;
            }

          case Held:
            {
              handle_held(pcount, bit, bit_is_set);
              break;
            }

          case Repeating:
            {
              handle_repeating(pcount, bit, bit_is_set);
              break;
            }
        }

        bit = bit << 1U; // Move to the next bit.
        ++pcount;        // Move to the next bit's timer.
      }

      return (changed != 0);
    }

    //***************************************************************************
    /// Returns true if the bit changed state on the last update.
    ///\param n The bit number (0..Width - 1).
    //***************************************************************************
    bool has_changed(size_t n) const
    {
      const value_type bit = bit_mask(n);

      return (changed & bit) != 0;
    }

    //***************************************************************************
    /// Returns true if the bit is currently set.
    ///\param n The bit number (0..Width - 1).
    //***************************************************************************
    bool is_set(size_t n) const
    {
      const value_type bit   = bit_mask(n);
      const state_type state = get_state(bit);

      return (state == On) || (state == Held) || (state == Repeating);
    }

    //***************************************************************************
    /// Returns true if the bit is currently held.
    ///\param n The bit number (0..Width - 1).
    //***************************************************************************
    bool is_held(size_t n) const
    {
      const value_type bit   = bit_mask(n);
      const state_type state = get_state(bit);

      return (state == Held) || (state == Repeating);
    }

    //***************************************************************************
    /// Returns true if the bit is currently repeating.
    ///\param n The bit number (0..Width - 1).
    //***************************************************************************
    bool is_repeating(size_t n) const
    {
      const value_type bit   = bit_mask(n);
      const state_type state = get_state(bit);

      return (state == Repeating);
    }

  private:

    //***************************************************************************
    /// State fit in 2 bits, state_lo & state_hi.
    //***************************************************************************
    enum state_id
    {
      Off       = 0, // Debounced state is Off.
      On        = 1, // Debounced state is On.
      Held      = 2, // Debounced state is Held. Implies On.
      Repeating = 3  // Debounced state is Repeating. Implies Held and On.
    };

    //***************************************************************************
    /// Gets the current debounce state for a specific bit.
    ///\return The current state of the bit.
    //***************************************************************************
    state_type get_state(const value_type bit) const
    {
      const state_type lo = static_cast<state_type>((state_lo & bit) > 0);
      const state_type hi = static_cast<state_type>((state_hi & bit) > 0);

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

    //***************************************************************************
    /// Increments *pcount; if it reaches 'threshold' the bit moves to
    /// 'new_state', the changed flag is set, and the timer is reset.
    //***************************************************************************
    void try_transition(count_type* pcount, uint32_t threshold, state_type new_state, value_type bit)
    {
      ++(*pcount);

      if (*pcount >= threshold)
      {
        set_state(new_state, bit);
        changed |= bit;
        *pcount = 0;
      }
    }

    //***************************************************************************
    /// Handler for the 'Off' state.
    //***************************************************************************
    void handle_off(count_type* pcount, value_type bit, bool bit_is_set)
    {
      if (bit_is_set)
      {
        try_transition(pcount, Valid_Count, On, bit);
      }
    }

    //***************************************************************************
    /// Handler for the 'On' state.
    //***************************************************************************
    void handle_on(count_type* pcount, value_type bit, bool bit_is_set)
    {
      if (bit_is_set)
      {
        if ETL_IF_CONSTEXPR (Hold_Count > 0)
        {
          try_transition(pcount, Hold_Count, Held, bit);
        }
      }
      else
      {
        try_transition(pcount, Valid_Count, Off, bit);
      }
    }

    //***************************************************************************
    /// Handler for the 'Held' state.
    //***************************************************************************
    void handle_held(count_type* pcount, value_type bit, bool bit_is_set)
    {
      if (bit_is_set)
      {
        if ETL_IF_CONSTEXPR (Repeat_Count > 0)
        {
          try_transition(pcount, Repeat_Count, Repeating, bit);
        }
      }
      else
      {
        try_transition(pcount, Valid_Count, Off, bit);
      }
    }

    //***************************************************************************
    /// Handler for the 'Repeating' state.
    //***************************************************************************
    void handle_repeating(count_type* pcount, value_type bit, bool bit_is_set)
    {
      if (bit_is_set)
      {
        try_transition(pcount, Repeat_Count, Repeating, bit);
      }
      else
      {
        try_transition(pcount, Valid_Count, Off, bit);
      }
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
    value_type state_hi; // High bits of the state. Combined with state_lo to form a 2-bit state for each bit in the field.
    value_type changed;  // Bits that have changed at the last update.

    count_type counts[Width]; // Off / On / Hold / Repeat timers.
  };
} // namespace etl

#endif
