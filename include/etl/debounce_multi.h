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

#ifndef ETL_DEBOUNCE_MULTI_BIT_INCLUDED
#define ETL_DEBOUNCE_MULTI_BIT_INCLUDED

#include "platform.h"
#include "array.h"
#include "binary.h"
#include "largest.h"
#include "memory.h"
#include "smallest.h"
#include "static_assert.h"

#include <stddef.h>
#include <stdint.h>

namespace etl
{
  //*************************************************************************
  /// Debounces a contiguous group of bits read from an IO port.
  ///\tparam TValue       The type of the port value (uint8_t/16_t/32_t/64_t).
  ///\tparam Width        The number of bits in the field (1..sizeof(TValue) * 8).
  ///\tparam Valid_Count  The number of consecutive samples a bit must differ
  ///                     from its previous debounced state before the state
  ///                     changes (>= 1).
  ///\tparam Hold_Count   The number of samples a bit must remain set (after
  ///                     becoming valid) before a 'held' event is generated.
  ///                     0 = disabled.
  ///\tparam Repeat_Count Once held, a 'repeat' event is generated every
  ///                     Repeat_Count samples. 0 = disabled.
  //*************************************************************************
  template <typename TValue, size_t Width, uint32_t Valid_Count, uint32_t Hold_Count = 0, uint32_t Repeat_Count = 0>
  class debounce_multi
  {
  private:

    // The types that can hold the counter values.
    // Used to determine the type of the per-bit counters.
    typedef typename etl::smallest_uint_for_value<Valid_Count>::type  valid_count_type;
    typedef typename etl::smallest_uint_for_value<Hold_Count>::type   hold_count_type;
    typedef typename etl::smallest_uint_for_value<Repeat_Count>::type repeat_count_type;

    // Determine the type of the per-bit counters.
    // Must be able to hold the largest of Valid_Count, Hold_Count and Repeat_Count.
    typedef typename etl::largest_type<valid_count_type, hold_count_type, repeat_count_type>::type count_type;

  public:

    // clang-format off
    ETL_STATIC_ASSERT(etl::is_integral<TValue>::value && etl::is_unsigned<TValue>::value, "TValue must be an unsigned integral type");
    ETL_STATIC_ASSERT(Width >= 1,                                                         "Width must be at least 1");
    ETL_STATIC_ASSERT(Width <= sizeof(TValue) * CHAR_BIT,                                 "The width does not fit in the value type");
    ETL_STATIC_ASSERT(Valid_Count >= 1,                                                   "Valid_Count must be at least 1");
    // clang-format on

    typedef TValue value_type;

    //***************************************************************************
    /// States fit in 2 bits.
    //***************************************************************************
    class bit_state
    {
    public:

      typedef uint_least8_t type;

      enum
      {
        Off       = 1,
        On        = 2,
        Held      = 3,
        Repeating = 4
      };
    };

    typedef typename bit_state::type bit_state_type;

    //***************************************************************************
    /// Constructor. All bits start in the 'Off' state.
    ///\param initial The initial port value. Default = the 'inactive' reading for all bits.
    //***************************************************************************
    debounce_multi(value_type initial = 0)
    {
      reset(initial);
    }

    //***************************************************************************
    /// Resets the debouncer state.
    ///\param initial The initial port value. Default = the 'inactive' reading for all bits.
    //***************************************************************************
    void reset(value_type initial = 0)
    {
      sample     = initial & field_mask();
      state_low  = sample;
      state_high = 0;
      changed    = 0;
      bit_index  = 0;

      // Clear the timers for all bits.
      counts.fill(0);
    }

    //***************************************************************************
    /// Resets a bit to the 'Off' state.
    /// \param n The bit number [0, Width). An 'n' that is out of range is ignored.
    /// \param initial The initial port value for this bit. Default is 'false'. The state is set to 'Off' and the count is reset to 0.
    //***************************************************************************
    void reset_bit(size_t n, bool initial = false)
    {
      // Only accept valid bit numbers.
      if (n < Width)
      {
        value_type bit = bit_mask(n);

        if (initial)
        {
          sample |= bit;
          set_bit_state(bit, bit_state::On);
        }
        else
        {
          sample &= ~bit;
          set_bit_state(bit, bit_state::Off);
        }

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
    bool update_incremental(const value_type next)
    {
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
    bool update(const value_type next)
    {
      // Determine which bits have changed since the last sample.
      const value_type changed_bits = sample ^ next;

      changed = 0;

      // Only bits that changed, are currently active, or are currently set can possibly transition.
      value_type relevant = changed_bits | state_low | state_high | next;

      while (relevant != 0)
      {
        const size_t     i   = static_cast<size_t>(etl::count_trailing_zeros(relevant));
        const value_type bit = bit_mask(i);

        const bool bit_has_changed = (changed_bits & bit) != 0;
        const bool bit_is_set      = (next & bit) != 0;

        update_bit(bit_has_changed, bit, &counts[i], bit_is_set);

        relevant = etl::clear_lowest_set_bit(relevant);
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
    bool is_off(size_t n) const
    {
      const value_type     bit   = bit_mask(n);
      const bit_state_type state = get_bit_state(bit);

      return (state == bit_state::Off);
    }

    //***************************************************************************
    /// Returns true if the bit is currently set.
    ///\param n The bit number [0, Width).
    //***************************************************************************
    bool is_on(size_t n) const
    {
      const value_type     bit   = bit_mask(n);
      const bit_state_type state = get_bit_state(bit);

      return (state >= bit_state::On);
    }

    //***************************************************************************
    /// Returns true if the bit is currently held.
    ///\param n The bit number [0, Width).
    //***************************************************************************
    bool is_held(size_t n) const
    {
      const value_type     bit   = bit_mask(n);
      const bit_state_type state = get_bit_state(bit);

      return (Hold_Count > 0) && (state >= bit_state::Held);
    }

    //***************************************************************************
    /// Returns true if the bit is currently repeating.
    ///\param n The bit number [0, Width).
    //***************************************************************************
    bool is_repeating(size_t n) const
    {
      const value_type     bit   = bit_mask(n);
      const bit_state_type state = get_bit_state(bit);

      return (Repeat_Count > 0) && (state >= bit_state::Repeating);
    }

    //***************************************************************************
    /// Returns the width of the bit field.
    //***************************************************************************
    ETL_CONSTEXPR size_t get_width() const
    {
      return Width;
    }

    //***************************************************************************
    /// Returns a mask of the current sample.
    //***************************************************************************
    value_type get_sample() const
    {
      return sample;
    }

    //***************************************************************************
    /// Returns a mask of all currently active bits (state >= On), where a set
    /// bit always means 'active'.
    //***************************************************************************
    value_type get_active() const
    {
      // A bit is 'active' (state >= On) if it is set in either state_low or state_high.
      return (state_low | state_high);
    }

    //***************************************************************************
    /// Returns the current state id of a bit.
    //***************************************************************************
    bit_state_type get_state(size_t n) const
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
    /// Describes a single state transition: the count threshold at which it
    /// fires (0 = never, i.e. no transition possible for this input), and the
    /// resulting state once the threshold is reached.
    //***************************************************************************
    struct transition
    {
      count_type     threshold;
      bit_state_type next_state;
    };

    //***************************************************************************
    /// Updates a bit.
    //***************************************************************************
    void update_bit(const bool bit_has_changed, const value_type bit, count_type* pcount, const bool bit_is_set)
    {
      if (bit_has_changed)
      {
        *pcount = 0;
      }

      // clang-format off
      static ETL_CONSTANT transition table[4][2] =
      {
        /* Off       */ {{0, bit_state::Off},
                        {Valid_Count, bit_state::On}},
        /* On        */ {{Valid_Count, bit_state::Off},
                        {(Hold_Count > 0) ? Hold_Count : Repeat_Count, (Hold_Count > 0) ? bit_state::Held
                                                                                          : (Repeat_Count > 0) ? bit_state::Repeating
                                                                                                               : bit_state::On}},
        /* Held      */ {{Valid_Count, bit_state::Off},
                        {Repeat_Count, bit_state::Repeating}},
        /* Repeating */ {{Valid_Count, bit_state::Off},
                        {Repeat_Count, bit_state::Repeating}},
      };
      // clang-format on

      const bit_state_type state = get_bit_state(bit);
      const transition&    t     = table[state][bit_is_set ? 1 : 0];

      if (t.threshold != 0)
      {
        check_for_transition(pcount, t.threshold, t.next_state, bit);
      }
    }

    //***************************************************************************
    /// Gets the current debounce state for a specific bit.
    ///\return The current state of the bit.
    //***************************************************************************
    bit_state_type get_bit_state(const value_type bit) const
    {
      const bit_state_type lo = static_cast<bit_state_type>((state_low & bit) > 0);
      const bit_state_type hi = static_cast<bit_state_type>((state_high & bit) > 0);

      return bit_state_type(lo | (hi << 1));
    }

    //***************************************************************************
    /// Sets the current debounce state for a specific bit.
    //***************************************************************************
    void set_bit_state(const value_type bit, bit_state_type state)
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
    void check_for_transition(count_type* pcount, uint32_t threshold, bit_state_type new_state, value_type bit)
    {
      ++(*pcount);

      if (*pcount >= threshold)
      {
        set_bit_state(bit, new_state);
        changed |= bit;
        *pcount = 0;
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
    static ETL_CONSTEXPR value_type field_mask()
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
