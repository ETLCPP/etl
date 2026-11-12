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
#include "largest.h"
#include "queue.h"
#include "smallest.h"
#include "integral_limits.h"

#include <stdint.h>

namespace etl
{
  //******************************************************************************
  // A key matrix decoder.
  // \tparam N_Rows       The number of rows in the key matrix. Rows are read from the key matrix.
  // \tparam N_Columns    The number of columns in the key matrix. Columns are written to the key matrix.
  // \tparam N_Key_Buffer The number of keys that may be buffered simultaneously.
  // \tparam Polarity     The polarity of the key matrix. Active_High = pressed key reads as '1', Active_Low = pressed key reads as '0'.
  // \tparam TCount       The type of the per-bit counters. Must be able to hold the largest of Valid_Count, Hold_Count and Repeat_Count.
  // \tparam TRow         The type of the read value (uint8_t/16_t/32_t/64_t).
  // \tparam TColumn      The type of the write value (uint8_t/16_t/32_t/64_t).
  // \tparam Valid_Count  The number of consecutive samples a bit must differ from its previous debounced state before the state changes (>= 1).
  // \tparam Hold_Count   The number of samples a bit must remain set (after becoming valid) before a 'held' event is generated. 0 = disabled.
  // \tparam Repeat_Count Once held, a 'repeat' event is generated every Repeat_Count samples. 0 = disabled.
  //******************************************************************************
  template <size_t N_Rows, size_t N_Columns, size_t N_Key_Buffer, etl::bit_polarity::type Polarity, typename TRow, typename TColumn,
            uint32_t Valid_Count, uint32_t Hold_Count = 0, uint32_t Repeat_Count = 0>
  class key_matrix
  {
  private:

    typedef typename etl::smallest_uint_for_value<Valid_Count>::type  valid_count_type;
    typedef typename etl::smallest_uint_for_value<Hold_Count>::type   hold_count_type;
    typedef typename etl::smallest_uint_for_value<Repeat_Count>::type repeat_count_type;

  public:

    typedef TRow    row_type;
    typedef TColumn column_type;

    // Determine the type of the per-bit counters. Must be able to hold the largest of Valid_Count, Hold_Count and Repeat_Count.
    typedef typename etl::largest_type<valid_count_type, hold_count_type, repeat_count_type>::type count_type;

    // The type that holds a key value
    typedef uint_least8_t key_type;

    // An invalid key code.
    static const key_type Invalid_Key = etl::integral_limits<key_type>::max;

    //***************************************************************************
    // The state of a key in the key matrix. It inherits from the debounce_multi state_id to provide the same state IDs.
    //***************************************************************************
    class key_state : public etl::debounce_multi<row_type, count_type, N_Rows, Polarity, Valid_Count, Hold_Count, Repeat_Count>::state_id
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
    ///\param column_index_ The index of the column to write to the key matrix.
    //***************************************************************************
    virtual void write_column(column_type column_index_) = 0;

    //***************************************************************************
    /// Read a value from the key matrix.
    ///\return The value read from the key matrix rows.
    //***************************************************************************
    virtual row_type read_row() = 0;

    //***************************************************************************
    /// Reset the key matrix.
    //***************************************************************************
    void reset()
    {
      column_index = 0;

      write_column(column_index);

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
      row_type read_value = read_row();

      // Get the current column debouncer.
      row_debouncer_type& column = columns[column_index];

      // Update the column debouncer with the new reading.
      if (column.update(read_value))
      {
        size_t row_index = 0;

        // Get the changed state of the column.
        row_type changed = column.get_changed();

        // One or more keys have changed state in this column.
        while (changed != 0)
        {
          if (changed & 1)
          {
            if (!key_buffer.full())
            {
              key_event event;

              event.key   = static_cast<key_type>((row_index * N_Columns) + column_index);
              event.state = column.get_state(row_index);

              key_buffer.push(event);
            }
          }

          changed >>= 1;
          ++row_index;
        }
      }

      ++column_index;

      if (column_index == N_Columns)
      {
        column_index = 0;
      }

      // Write the next column to the key matrix.
      write_column(column_index);

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
    size_t get_column_size() const
    {
      return N_Columns;
    }

    //***************************************************************************
    size_t get_row_size() const
    {
      return N_Rows;
    }

    //***************************************************************************
    size_t get_matrix_size() const
    {
      return N_Rows * N_Columns;
    }

    //***************************************************************************
    size_t get_key_event_max_size() const
    {
      return N_Key_Buffer;
    }

    //***************************************************************************
    size_t get_key_event_size() const
    {
      return key_buffer.size();
    }

    //***************************************************************************
    key_event get_key_event()
    {
      key_event event{Invalid_Key, key_state::Off};

      if (!key_buffer.empty())
      {
        key_buffer.pop_into(event);
      }

      return event;
    }

    //***************************************************************************
    size_t get_max_key_id() const
    {
      return ;
    }

  private:

    typedef etl::debounce_multi<row_type, count_type, N_Rows, Polarity, Valid_Count, Hold_Count, Repeat_Count> row_debouncer_type;

    etl::array<row_debouncer_type, N_Columns> columns;

    // The buffer of keys that have been detected in the last complete scan.
    etl::queue<key_event, N_Key_Buffer> key_buffer;

    column_type column_index;
  };
} // namespace etl

#endif
