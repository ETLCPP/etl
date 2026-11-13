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
#include "bit.h"
#include "debounce_multi.h"
#include "integral_limits.h"
#include "queue.h"

#include <stdint.h>

namespace etl
{
  // clang-format off
  //******************************************************************************
  // A key matrix decoder.
  // \tparam N_Rows       The number of rows in the key matrix. Rows are read from the key matrix.
  // \tparam N_Columns    The number of columns in the key matrix. Columns are written to the key matrix.
  // \tparam Polarity     The polarity of the key matrix. Active_High = pressed key reads as '1', Active_Low = pressed key reads as '0'.
  // \tparam Valid_Count  The number of consecutive samples a bit must differ from its previous debounced state before the state changes (>= 1).
  // \tparam Hold_Count   The number of samples a bit must remain set (after becoming valid) before a 'held' event is generated. 0 = disabled. Default = 0.
  // \tparam Repeat_Count Once held, a 'repeat' event is generated every Repeat_Count samples. 0 = disabled. Default = 0.
  // \tparam N_Key_Events The number of keys that may be buffered simultaneously. Default = 1.
  //******************************************************************************
  // clang-format on
  template <size_t N_Rows, size_t N_Columns, etl::bit_polarity::type Polarity, uint32_t Valid_Count, uint32_t Hold_Count = 0,
            uint32_t Repeat_Count = 0, size_t N_Key_Events = 1>
  class key_matrix
  {
  public:

    // The type that holds the value read from the key matrix column debouncer.
    typedef uint_least8_t column_type;

    // The type that holds a column index
    typedef uint_least8_t column_index_type;

  private:

    // The debouncer for each column in the key matrix.
    typedef etl::debounce_multi<column_type, N_Rows, Polarity, Valid_Count, Hold_Count, Repeat_Count> debounce_type;

  public:

    // The type that holds a key value
    typedef uint_least8_t key_type;

    // The invalid key code.
    // The range of supported keys is [0, Invalid_Key).
    static const key_type Invalid_Key = etl::integral_limits<key_type>::max;

    //***************************************************************************
    // The state of a key in the key matrix.
    // It inherits from the debounce_multi state_id to provide the same state IDs.
    //***************************************************************************
    class key_state : public debounce_type::state_id
    {
    public:

      //*******************************
      /// Constructor.
      //*******************************
      key_state()
        : state(key_state::Off)
      {
      }

      //*******************************
      /// Copy constructor.
      //*******************************
      key_state(const key_state& other)
        : state(other.state)
      {
      }

      //*******************************
      /// Construct from a key_state::type.
      //*******************************
      key_state(typename key_state::type state_)
        : state(state_)
      {
      }

      //*******************************
      /// Assignment operator from key_state.
      //*******************************
      key_state& operator=(const key_state& other)
      {
        state = other.state;

        return *this;
      }

      //*******************************
      /// Assignment operator from key_state::type.
      //*******************************
      key_state& operator=(const key_state::type state_)
      {
        state = state_;

        return *this;
      }

      //*******************************
      /// Returns true if the key is currently pressed.
      //*******************************
      bool is_pressed() const
      {
        return (state >= key_state::On);
      }

      //*******************************
      /// Returns true if the key is currently held.
      //*******************************
      bool is_held() const
      {
        return (state >= key_state::Held);
      }

      //*******************************
      /// Returns true if the key is currently repeating.
      //*******************************
      bool is_repeating() const
      {
        return (state == key_state::Repeating);
      }

    private:

      key_state::type state;
    };

    struct key_event
    {
      key_type  key;   // The key code of the key that has changed state.
      key_state state; // The new state of the key.
    };

    //***************************************************************************
    /// Constructor.
    //***************************************************************************
    key_matrix()
      : column_index(0)
    {
    }

    //***************************************************************************
    /// Destructor.
    //***************************************************************************
    virtual ~key_matrix() {}

    //***************************************************************************
    /// Write the column index to the key matrix.
    /// This function must be overridden to write the column index to the key matrix.
    ///\param column_index The index of the column to make active in the key matrix.
    //***************************************************************************
    virtual void select_column(column_index_type column_index_) = 0;

    //***************************************************************************
    /// Read a value from the key matrix.
    /// This function must be overridden to read a value from the key matrix.
    ///\return The value read from the key matrix rows for the currently selected column.
    //***************************************************************************
    virtual column_type read_column() = 0;

    //***************************************************************************
    /// Reset the key matrix.
    //***************************************************************************
    void reset()
    {
      column_index = 0;

      select_column(column_index);

      for (size_t i = 0; i < N_Columns; ++i)
      {
        columns[i].reset();
      }
    }

    //***************************************************************************
    /// Scan the key matrix.
    /// This function must be called repeatedly to scan the key matrix.
    /// Returns 'true' if all of the rows have been processed, otherwise 'false'.
    //***************************************************************************
    bool scan_incremental()
    {
      // Read the next column from the key matrix.
      column_type read_value = read_column();

      // Get the current column debouncer.
      debounce_type& column = columns[column_index];

      // Update the column debouncer with the new reading.
      if (column.update(read_value))
      {
        // Get the changed state of the column.
        column_type changed = column.get_changed();

        // One or more keys have changed state in this column.
        while ((changed != 0) && !key_event_buffer.full())
        {
          // Jump straight to the next set bit by using the count trailing zeros function.
          size_t row_index = static_cast<size_t>(etl::countr_zero(changed));

          // Add the key event to the buffer.
          key_type  key   = static_cast<key_type>((row_index * N_Columns) + column_index);
          key_state state = column.get_state(row_index);
          key_event event{key, state};

          key_event_buffer.push(event);

          // Clear the bit we've just processed.
          changed &= static_cast<column_type>(changed - 1);
        }
      }

      ++column_index;

      if (column_index == N_Columns)
      {
        column_index = 0;
      }

      // Write the next column to the key matrix.
      select_column(column_index);

      // Return true if all of the columns have been processed, otherwise false.
      return (column_index == 0);
    }

    //***************************************************************************
    /// Scan the key matrix.
    /// This function will scan all of the columns and row in the key matrix.
    //***************************************************************************
    void scan()
    {
      column_index = 0;

      while (!scan_incremental())
      {
        // Do nothing, just keep scanning until all columns have been processed.
      }
    }

    //***************************************************************************
    /// Returns the number of columns in the key matrix.
    //***************************************************************************
    size_t column_size() const
    {
      return N_Columns;
    }

    //***************************************************************************
    /// Returns the number of rows in the key matrix.
    //***************************************************************************
    size_t row_size() const
    {
      return N_Rows;
    }

    //***************************************************************************
    /// Returns the number of keys in the key matrix.
    //***************************************************************************
    size_t matrix_size() const
    {
      return N_Rows * N_Columns;
    }

    //***************************************************************************
    /// Returns the maximum number of key events that can be buffered.
    //***************************************************************************
    size_t key_event_max_size() const
    {
      return N_Key_Events;
    }

    //***************************************************************************
    /// Returns the number of key events that have been buffered.
    //***************************************************************************
    size_t key_event_size() const
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
    key_event get_next_key_event()
    {
      key_event event{Invalid_Key, key_state::Off};

      if (!key_event_buffer.empty())
      {
        key_event_buffer.pop_into(event);
      }

      return event;
    }

    //***************************************************************************
    size_t get_max_key_id() const
    {
      return Invalid_Key - 1U;
    }

  private:

    // The buffer of keys that have been detected in the last complete scan.
    etl::queue<key_event, N_Key_Events> key_event_buffer;

    // The list of debouncers for each column in the key matrix.
    etl::array<debounce_type, N_Columns> columns;

    // The index of the column that is currently being processed.
    column_index_type column_index;
  };
} // namespace etl

#endif
