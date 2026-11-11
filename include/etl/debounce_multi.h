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

#include "platform.h"
#include "array.h"
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
  ///\tparam Polarity    The polarity of the bits. Active_High = valid input reads as '1', Active_Low = valid input reads as '0'.
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
  template <typename TValue, typename TCount, size_t Width, etl::bit_polarity::type Polarity, uint32_t Valid_Count, uint32_t Hold_Count = 0,
            uint32_t Repeat_Count = 0>
  class debounce_multi
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

    typedef TValue value_type;
    typedef TCount count_type;

    //***************************************************************************
    /// State fit in 2 bits, state_low & state_high.
    //***************************************************************************
    struct state_id
    {
      typedef uint_least8_t type;

      enum
      {
        Off       = 0, // Debounced state is Off.
        On        = 1, // Debounced state is On.
        Held      = 2, // Debounced state is Held. Implies On.
        Repeating = 3  // Debounced state is Repeating. Implies Held and On.
      };
    };

    //***************************************************************************
    /// Constructor. All bits start in the 'Off' state.
    ///\param initial The initial raw port value, in the same polarity as values passed to update(). Default = the 'inactive' reading for all bits.
    //***************************************************************************
    debounce_multi(value_type initial = initial_value())
    {
      reset(initial);
    }

    //***************************************************************************
    /// Resets the debouncer state.
    ///\param initial The initial raw port value, in the same polarity as values passed to update(). Default = the 'inactive' reading for all bits.
    //***************************************************************************
    void reset(value_type initial = initial_value())
    {
      sample     = logical_value(initial); // Converted to, and stored in, the internal logical (active = 1) form.
      state_low  = 0;
      state_high = 0;
      changed    = 0;
      bit_index  = 0;

      // Clear the timers for all bits.
      counts.fill(0);
    }

    //***************************************************************************
    /// Resets a bit to the 'Off' state.
    /// \param n The bit number [0, Width). An 'n' that is out of range is ignored.
    /// \param initial The raw reading for this bit, in the same polarity as values passed to update().
    ///                If true, the bit reads as it would when physically asserted; if false, as at rest.
    ///                Default is 'false'. The state is set to 'Off' and the count is reset to 0.
    //***************************************************************************
    void reset_bit(size_t n, bool initial = false)
    {
      // Only accept valid bit numbers.
      if (n < Width)
      {
        value_type bit = bit_mask(n);

        // Convert the raw reading to the internal logical (active = 1) form.
        const bool bit_is_active = logical_value(initial);

        if (bit_is_active)
        {
          sample |= bit;
        }
        else
        {
          sample &= ~bit;
        }

        clear_bit_state(bit);
        clear_changed_state(bit);
        clear_counter(n);
      }
    }

    //***************************************************************************
    /// Updates a bit.
    /// The value of 'raw_next' must not change between calls to update_incremental() until all bits have been processed.
    /// Do not call update() after update_incremental() on the same instance unless update_incremental() has completed a full cycle (returned true).
    /// Returns true if all of the bits have been processed.
    //***************************************************************************
    bool update_incremental(const value_type raw_next)
    {
      // Convert to the internal logical (active = 1) representation.
      const value_type next = logical_value(raw_next);

      // Determine which bits have changed since the last sample.
      const value_type changed_bits = sample ^ next;

      // First bit?
      if (bit_index == 0)
      {
        changed = 0;
      }

      count_type*      pcount          = &counts[bit_index];
      const value_type bit             = bit_mask(bit_index);
      const bool       bit_has_changed = (changed_bits & bit) != 0;
      const bool       bit_is_set      = (next & bit) != 0;

      update_bit(bit_has_changed, bit, pcount, bit_is_set);

      ++bit_index;

      // Have we processed all of the bits?
      if (bit_index == Width)
      {
        bit_index = 0;
        sample    = next;
        return true;
      }
      else
      {
        return false;
      }
    }

    //***************************************************************************
    /// Updates the whole port value.
    /// Returns true if any bit changed state.
    //***************************************************************************
    bool update(const value_type raw_next)
    {
      // Convert to the internal logical (active = 1) representation.
      const value_type next = logical_value(raw_next);

      // Determine which bits have changed since the last sample.
      const value_type changed_bits = sample ^ next;

      changed = 0;

      // The initial loop conditions.
      count_type* pcount = counts.data(); // Pointer to the first bit's timer.
      value_type  bit    = 1U;            // Mask for the first bit in the field.

      for (size_t i = 0; i < Width; ++i)
      {
        const bool bit_has_changed = (changed_bits & bit) != 0;
        const bool bit_is_set      = (next & bit) != 0;

        update_bit(bit_has_changed, bit, pcount, bit_is_set);

        bit = bit << 1U; // Move to the next bit.
        ++pcount;        // Move to the next bit's timer.
      }

      sample = next;

      return (changed != 0);
    }

    //***************************************************************************
    /// Returns true if the bit changed state on the last update.
    ///\param n The bit number [0, Width).
    //***************************************************************************
    bool has_changed(size_t n) const
    {
      const value_type bit = bit_mask(n);

      return (changed & bit) != 0;
    }

    //***************************************************************************
    /// Returns true if the bit is currently set.
    ///\param n The bit number [0, Width).
    //***************************************************************************
    bool is_set(size_t n) const
    {
      const value_type     bit   = bit_mask(n);
      const state_id::type state = get_bit_state(bit);

      return (state >= state_id::On);
    }

    //***************************************************************************
    /// Returns true if the bit is currently held.
    ///\param n The bit number [0, Width).
    //***************************************************************************
    bool is_held(size_t n) const
    {
      const value_type     bit   = bit_mask(n);
      const state_id::type state = get_bit_state(bit);

      return (state >= state_id::Held);
    }

    //***************************************************************************
    /// Returns true if the bit is currently repeating.
    ///\param n The bit number [0, Width).
    //***************************************************************************
    bool is_repeating(size_t n) const
    {
      const value_type     bit   = bit_mask(n);
      const state_id::type state = get_bit_state(bit);

      return (state == state_id::Repeating);
    }

    //***************************************************************************
    /// Returns the width of the bit field.
    //***************************************************************************
    size_t get_width() const
    {
      return Width;
    }

    //***************************************************************************
    /// Returns the stable (debounced) value of all bits, in the same polarity as the raw input.
    //***************************************************************************
    value_type get_value() const
    {
      // A bit is 'active' (state >= On) if it is set in either state_low or state_high.
      const value_type active_mask = (state_low | state_high);

      // Convert from the internal logical (active = 1) representation back to the raw input's polarity.
      if ETL_IF_CONSTEXPR ((Polarity == etl::bit_polarity::Active_High))
      {
        return active_mask;
      }
      else
      {
        return static_cast<value_type>(active_mask ^ field_mask());
      }
    }

    //***************************************************************************
    /// Returns the current state id of a bit.
    //***************************************************************************
    state_id::type get_state(size_t n) const
    {
      value_type bit = bit_mask(n);

      return get_bit_state(bit);
    }

    //***************************************************************************
    /// Returns the changed state of the bit field.
    //***************************************************************************
    value_type get_changed() const
    {
      return changed;
    }

  private:

    //***************************************************************************
    /// Returns the default initial value for the sample, expressed in the raw/physical polarity
    /// (i.e. the 'inactive' reading for all bits, matching what a real port would read at rest).
    //***************************************************************************
    static value_type initial_value()
    {
      return static_cast<value_type>(Polarity == etl::bit_polarity::Active_High) ? 0 : field_mask();
    }

    //***************************************************************************
    /// Converts a raw port value into the internal logical (active = 1) representation,
    /// independent of the configured polarity.
    //***************************************************************************
    static value_type logical_value(value_type v)
    {
      if ETL_IF_CONSTEXPR (Polarity == etl::bit_polarity::Active_High)
      {
        return v & field_mask();
      }
      else
      {
        return static_cast<value_type>(~v & field_mask());
      }
    }

    //***************************************************************************
    /// Converts a raw port bool into the internal logical (active = 1) representation,
    /// independent of the configured polarity.
    //***************************************************************************
    static bool logical_value(bool v)
    {
      if ETL_IF_CONSTEXPR (Polarity == etl::bit_polarity::Active_High)
      {
        return v;
      }
      else
      {
        return !v;
      }
    }

    //***************************************************************************
    /// Updates a bit.
    //***************************************************************************
    void update_bit(const bool bit_has_changed, const value_type bit, count_type* pcount, const bool bit_is_set)
    {
      if (bit_has_changed)
      {
        *pcount = 0;
      }

      switch (get_bit_state(bit))
      {
        case state_id::Off:
          {
            handle_off(pcount, bit, bit_is_set);
            break;
          }

        case state_id::On:
          {
            handle_on(pcount, bit, bit_is_set);
            break;
          }

        case state_id::Held:
          {
            handle_held(pcount, bit, bit_is_set);
            break;
          }

        case state_id::Repeating:
          {
            handle_repeating(pcount, bit, bit_is_set);
            break;
          }
      }
    }

    //***************************************************************************
    /// Gets the current debounce state for a specific bit.
    ///\return The current state of the bit.
    //***************************************************************************
    state_id::type get_bit_state(const value_type bit) const
    {
      const state_id::type lo = static_cast<state_id::type>((state_low & bit) > 0);
      const state_id::type hi = static_cast<state_id::type>((state_high & bit) > 0);

      return static_cast<state_id::type>(lo | (hi << 1));
    }

    //***************************************************************************
    /// Sets the current debounce state for a specific bit.
    //***************************************************************************
    void set_bit_state(const value_type bit, state_id::type state)
    {
      state_low  = state & 1 ? state_low | bit : state_low & ~bit;
      state_high = state & 2 ? state_high | bit : state_high & ~bit;
    }

    //***************************************************************************
    /// Clears the current debounce state for a specific bit.
    //***************************************************************************
    void clear_bit_state(const value_type bit)
    {
      state_low  = state_low & ~bit;
      state_high = state_high & ~bit;
    }

    //***************************************************************************
    /// Clears the current changed state for a specific bit.
    //***************************************************************************
    void clear_changed_state(const value_type bit)
    {
      changed &= ~bit;
    }

    //***************************************************************************
    /// Clears the current timer for a specific bit.
    //***************************************************************************
    void clear_counter(size_t n)
    {
      counts[n] = 0;
    }

    //***************************************************************************
    /// Increments *pcount; if it reaches 'threshold' the bit moves to
    /// 'new_state', the changed flag is set, and the timer is reset.
    //***************************************************************************
    void check_for_transition(count_type* pcount, uint32_t threshold, state_id::type new_state, value_type bit)
    {
      ++(*pcount);

      if (*pcount >= threshold)
      {
        set_bit_state(bit, new_state);
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
        check_for_transition(pcount, Valid_Count, state_id::On, bit);
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
          check_for_transition(pcount, Hold_Count, state_id::Held, bit);
        }
      }
      else
      {
        check_for_transition(pcount, Valid_Count, state_id::Off, bit);
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
          check_for_transition(pcount, Repeat_Count, state_id::Repeating, bit);
        }
      }
      else
      {
        check_for_transition(pcount, Valid_Count, state_id::Off, bit);
      }
    }

    //***************************************************************************
    /// Handler for the 'Repeating' state.
    //***************************************************************************
    void handle_repeating(count_type* pcount, value_type bit, bool bit_is_set)
    {
      if (bit_is_set)
      {
        check_for_transition(pcount, Repeat_Count, state_id::Repeating, bit);
      }
      else
      {
        check_for_transition(pcount, Valid_Count, state_id::Off, bit);
      }
    }

    //*************************************************************************
    /// Returns a mask with only the nth bit set.
    /// If 'n' is out of range then returns 0.
    //*************************************************************************
    static value_type bit_mask(size_t n)
    {
      return (n < Width) ? static_cast<value_type>(value_type(1) << n) : 0;
    }

    //*************************************************************************
    /// Returns a mask with only Width bits set.
    //*************************************************************************
    static value_type field_mask()
    {
      return static_cast<value_type>(etl::max_value_for_nbits<Width>::value);
    }

    value_type sample;     // The last known value of the bits. Note, this is not the same as the 'state' of the bits.
    value_type state_low;  // Low bits of the state.
    value_type state_high; // High bits of the state. Combined with state_low to form a 2-bit state for each bit in the field.
    value_type changed;    // Bits that have changed at the last update.

    etl::array<count_type, Width> counts; // Off / On / Hold / Repeat timers.

    uint_least8_t bit_index; // Index of the next bit to process in incremental mode.
  };
} // namespace etl

#endif
