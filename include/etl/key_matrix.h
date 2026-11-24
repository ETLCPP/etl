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

#ifndef ETL_KEY_MATRIX_INCLUDED
#define ETL_KEY_MATRIX_INCLUDED

#include "platform.h"
#include "array.h"
#include "binary.h"
#include "debounce_multi.h"
#include "integral_limits.h"
#include "queue.h"
#include "smallest.h"

#include <stdint.h>

namespace etl
{
  //***************************************************************************
  // The state of a key in the key matrix.
  // It inherits from the debounce_state_id to provide the same state IDs.
  //***************************************************************************
  class key_matrix_state : public etl::debounce_state_id
  {
  public:

    //*******************************
    /// Constructor.
    //*******************************
    key_matrix_state()
      : state(key_matrix_state::Off)
    {
    }

    //*******************************
    /// Copy constructor.
    //*******************************
    key_matrix_state(const key_matrix_state& other)
      : state(other.state)
    {
    }

    //*******************************
    /// Construct from a key_matrix_state::type.
    //*******************************
    key_matrix_state(typename key_matrix_state::type state_)
      : state(state_)
    {
    }

    //*******************************
    /// Assignment operator from key_matrix_state.
    //*******************************
    key_matrix_state& operator=(const key_matrix_state& other)
    {
      state = other.state;

      return *this;
    }

    //*******************************
    /// Assignment operator from key_matrix_state::type.
    //*******************************
    key_matrix_state& operator=(const key_matrix_state::type state_)
    {
      state = state_;

      return *this;
    }

    //*******************************
    /// Returns true if the key is currently pressed.
    //*******************************
    bool is_pressed() const
    {
      return (state >= key_matrix_state::On);
    }

    //*******************************
    /// Returns true if the key is currently held.
    //*******************************
    bool is_held() const
    {
      return (state >= key_matrix_state::Held);
    }

    //*******************************
    /// Returns true if the key is currently repeating.
    //*******************************
    bool is_repeating() const
    {
      return (state == key_matrix_state::Repeating);
    }

    //*******************************
    /// Returns the key_matrix_state::type value.
    //*******************************
    key_matrix_state::type value() const
    {
      return state;
    }

    //*******************************
    /// Equality operator.
    //*******************************
    friend bool operator==(const key_matrix_state& lhs, const key_matrix_state& rhs)
    {
      return (lhs.state == rhs.state);
    }

    //*******************************
    /// Inequality operator.
    //*******************************
    friend bool operator!=(const key_matrix_state& lhs, const key_matrix_state& rhs)
    {
      return !(lhs == rhs);
    }

  private:

    key_matrix_state::type state;
  };

  //***************************************************************************
  /// Traits for the key matrix.
  ///\tparam N_Read    The number of read lines in the key matrix.
  ///\tparam N_Write The number of write lines in the key matrix.
  //***************************************************************************
  template <size_t N_Read, size_t N_Write>
  struct key_matrix_traits
  {
    // clang-format off
    typedef typename etl::smallest_uint_for_bits<N_Read>::type              read_type;          ///< The type that holds the value read from the key matrix read lines.
    typedef typename etl::smallest_uint_for_value<N_Write>::type          write_index_type; ///< The type that holds a write index that selects the active write.
    typedef typename etl::smallest_uint_for_value<N_Read * N_Write>::type key_type;          ///< The type that holds a key value.
    /// clang-format on

    typedef key_matrix_state key_state; ///< The type that holds the state of a key in the key matrix.

    // The invalid key code.
    // The range of supported keys is [0, Invalid_Key).
    static ETL_CONSTANT key_type Invalid_Key = etl::integral_limits<key_type>::max;

    // The maximum valid key code.
    static ETL_CONSTANT key_type Max_Key = Invalid_Key - 1U;

    //***************************************************************************
    /// A key event that is pushed into the key event buffer when a key changes state.
    //***************************************************************************
    struct key_event
    {
      typedef key_matrix_traits::key_type  key_type;
      typedef key_matrix_traits::key_state key_state;

      key_type  key;   // The key code of the key that has changed state.
      key_state state; // The new state of the key.
    };

    //***************************************************************************
    /// Returns the number of write lines in the key matrix.
    //***************************************************************************
    ETL_CONSTEXPR size_t write_size() const
    {
      return N_Write;
    }

    //***************************************************************************
    /// Returns the number of read lines in the key matrix.
    //***************************************************************************
    ETL_CONSTEXPR size_t read_size() const
    {
      return N_Read;
    }

    //***************************************************************************
    /// Returns the number of keys in the key matrix.
    //***************************************************************************
    ETL_CONSTEXPR size_t matrix_size() const
    {
      return N_Read * N_Write;
    }

    //***************************************************************************
    /// Returns the invalid key ID for tis key matrix.
    /// The range of supported keys is [0, invalid_key_id()].
    //***************************************************************************
    ETL_CONSTEXPR key_type invalid_key_id() const
    {
      return Invalid_Key;
    }

    //***************************************************************************
    /// Returns the maximum key ID that can be used with this key matrix.
    /// The range of supported keys is [0, max_key_id()].
    //***************************************************************************
    ETL_CONSTEXPR key_type max_key_id() const
    {
      return Invalid_Key - 1U;
    }
  };

  // clang-format off
  //******************************************************************************
  // A key matrix decoder.
  // \tparam TMatrix_Port      The type that provides the interface to the key matrix hardware.
  // \tparam N_Read            The number of read lines in the key matrix.
  // \tparam N_Write           The number of write lines in the key matrix.
  // \tparam Valid_Count       The number of consecutive samples a bit must differ from its previous debounced state before the state changes (>= 1).
  // \tparam Hold_Count        The number of samples a bit must remain set (after becoming valid) before a 'held' event is generated. 0 = disabled. Default = 0.
  // \tparam Repeat_Count      Once held, a 'repeat' event is generated every Repeat_Count samples. 0 = disabled. Default = 0.
  // \tparam N_Key_Events      The number of keys that may be buffered simultaneously. Default = 1.
  // \tparam Filter_Ghost_Keys If true, any newly changed bits that are ghosted are suppressed until the ambiguity resolves. Default = false.
  //******************************************************************************
  // clang-format on
  template <typename TMatrix_Port, size_t N_Read, size_t N_Write, uint32_t Valid_Count, uint32_t Hold_Count = 0, uint32_t Repeat_Count = 0,
            size_t N_Key_Events = 1, bool Filter_Ghost_Keys = false>
  class key_matrix : public key_matrix_traits<N_Read, N_Write>
  {
  public:

    using typename key_matrix_traits<N_Read, N_Write>::read_type;
    using typename key_matrix_traits<N_Read, N_Write>::write_index_type;
    using typename key_matrix_traits<N_Read, N_Write>::key_type;
    using typename key_matrix_traits<N_Read, N_Write>::key_event;

    using key_matrix_traits<N_Read, N_Write>::Invalid_Key;
    using key_matrix_traits<N_Read, N_Write>::read_size;
    using key_matrix_traits<N_Read, N_Write>::write_size;
    using key_matrix_traits<N_Read, N_Write>::matrix_size;
    using key_matrix_traits<N_Read, N_Write>::max_key_id;

    typedef typename etl::key_matrix_state key_state;

  private:

    // The debouncer for each write in the key matrix.
    typedef etl::debounce_multi<read_type, N_Read, Valid_Count, Hold_Count, Repeat_Count> read_channel_type;

  public:

    //***************************************************************************
    /// Constructor.
    //***************************************************************************
    key_matrix(TMatrix_Port& port_)
      : port(port_)
      , write_index(0)
    {
      port.write_key_matrix(write_index);
    }

    //***************************************************************************
    /// Reset the key matrix.
    /// This will reset the read_channels and clear the key event buffer.
    /// Selects the first write line in the key matrix.
    //***************************************************************************
    void reset()
    {
      write_index = 0;

      port.write_key_matrix(write_index);

      for (size_t i = 0; i < N_Write; ++i)
      {
        read_channels[i].reset();
      }

      key_event_buffer.clear();
    }

    //***************************************************************************
    /// Scans one write of the key matrix.
    /// This function must be called repeatedly to scan the whole key matrix.
    /// Returns 'true' if all of the read_channels have been processed, otherwise 'false'.
    //***************************************************************************
    bool scan_incremental()
    {
      // Read the next write from the key matrix.
      read_type read_value = static_cast<read_type>(port.read_key_matrix());

      // Get the current write debouncer.
      read_channel_type& read_channel = read_channels[write_index];

      read_channel.update(read_value);

      ++write_index;

      if (write_index == N_Write)
      {
        write_index = 0;

        // The full cycle is complete - every write now reflects the same
        // scan instant, so it's safe to resolve ghosting and push events.
        resolve_pending_key_events();
      }

      // Write the next write to the key matrix.
      port.write_key_matrix(write_index);

      // Return true if all of the debouncer have been processed, otherwise false.
      return (write_index == 0);
    }

    //***************************************************************************
    /// Scans the whole key matrix.
    /// This function will scan all of the write lines and read lines in the key matrix.
    /// Returns 'true' if any key events have been produced, otherwise 'false'.
    //***************************************************************************
    bool scan()
    {
      write_index = 0;

      while (!scan_incremental())
      {
        // Do nothing, just keep scanning until all write lines have been processed.
      }

      return has_key_events();
    }

    //***************************************************************************
    key_state get_key_state(size_t key) const
    {
      size_t row   = key / N_Write;
      size_t write = key % N_Write;

      return read_channels[write].get_state(row);
    }

    //***************************************************************************
    /// Returns the number of key events that have been buffered.
    //***************************************************************************
    size_t number_of_key_events() const
    {
      return key_event_buffer.size();
    }

    //***************************************************************************
    /// Returns true if there are key events in the buffer.
    //***************************************************************************
    bool has_key_events() const
    {
      return !key_event_buffer.empty();
    }

    //***************************************************************************
    /// Returns the next key event from the buffer.
    /// If the buffer is empty then returns a key event with key = Invalid_Key and state = Off.
    //***************************************************************************
    key_event next_key_event()
    {
      key_event event{Invalid_Key, key_state::Off};

      if (!key_event_buffer.empty())
      {
        key_event_buffer.pop_into(event);
      }

      return event;
    }

    //***************************************************************************
    /// Clears the key event buffer.
    //***************************************************************************
    void clear_key_events()
    {
      key_event_buffer.clear();
    }

    //***************************************************************************
    /// Returns the maximum number of key events that can be buffered.
    //***************************************************************************
    ETL_CONSTEXPR size_t max_number_of_key_events() const
    {
      return N_Key_Events;
    }

  private:

    //***************************************************************************
    /// Pushes any newly changed keys into the key event buffer.
    /// If Filter_Ghost_Keys is true, then any newly changed keys that are ghosted are removed.
    /// Must only be called once every write has been updated for this cycle.
    //***************************************************************************
    void resolve_pending_key_events()
    {
      // Uses the classic 'seen in 2+ sets' bit trick.
      read_type seen_once  = 0;
      read_type seen_twice = 0;

      if ETL_IF_CONSTEXPR (Filter_Ghost_Keys)
      {
        for (write_index_type wi = 0; wi < N_Write; ++wi)
        {
          read_channel_type& read_channel = read_channels[wi];

          const read_type mask = read_channel.get_active_mask();

          // Ghost keys only occur in read_channels that have multiple keys active at the same time.
          if (has_multiple_set_bits(mask))
          {
            // Accumulate any keys that are already in 'seen_once' into 'seen_twice'.
            // For each key in the new mask, if it is already in 'seen_once', then it is now in 'seen_twice'.
            seen_twice = static_cast<read_type>(seen_twice | (seen_once & mask));

            // Accumulate all of the keys that have been seen in any write.
            seen_once = static_cast<read_type>(seen_once | mask);
          }
        }
      }

      // Loop through all of the read_channels and push any changed keys into the key event buffer.
      for (write_index_type wi = 0; wi < N_Write; ++wi)
      {
        read_channel_type& read_channel = read_channels[wi];

        read_type changed = read_channel.get_changed();

        if (changed != 0)
        {
          if ETL_IF_CONSTEXPR (Filter_Ghost_Keys)
          {
            // Clear any changed keys that are ghosted (active in 2+ ambiguous read_channels this scan).
            changed = static_cast<read_type>(changed & ~seen_twice);
          }

          // Push all of the changed keys into the key event buffer.
          while ((changed != 0) && !key_event_buffer.full())
          {
            size_t row_index = static_cast<size_t>(etl::count_trailing_zeros(changed));

            key_type  key   = static_cast<key_type>((row_index * N_Write) + wi);
            key_state state = read_channel.get_state(row_index);
            key_event event{key, state};

            key_event_buffer.push(event);

            changed = etl::clear_lowest_set_bit(changed);
          }
        }
      }
    }

    key_matrix(const key_matrix&) ETL_DELETE;
    key_matrix& operator=(const key_matrix&) ETL_DELETE;

    // The port that provides the interface to the key matrix hardware.
    TMatrix_Port& port;

    // The buffer of keys that have been detected in the last complete scan.
    etl::queue<key_event, N_Key_Events> key_event_buffer;

    // The list of debouncers for each write in the key matrix.
    etl::array<read_channel_type, N_Write> read_channels;

    // The index of the write that is currently being processed.
    write_index_type write_index;
  };
} // namespace etl

#endif
