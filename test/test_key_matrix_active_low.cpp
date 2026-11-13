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

#include "unit_test_framework.h"

#include "etl/key_matrix.h"

#include <array>
#include <vector>

namespace
{
  const size_t   NRows        = 3;
  const size_t   NColumns     = 4;
  const size_t   NKeys        = 12;
  const uint32_t Valid_Count  = 4;
  const uint32_t Hold_Count   = 4;
  const uint32_t Repeat_Count = 4;

  //*************************************************************************
  // A key pad base class for testing.
  // NRows        = 3
  // NColumns     = 4
  // Polarity     = Active_Low
  // Valid_Count  = 4
  // Hold_Count   = 4
  // Repeat_Count = 4
  // NKeys        = 12
  //*************************************************************************
  using key_pad_base = etl::key_matrix<NRows, NColumns, etl::bit_polarity::Active_Low, Valid_Count, Hold_Count, Repeat_Count, NKeys>;

  //*************************************************************************
  // A simple key pad implementation for testing.
  //*************************************************************************
  class key_pad : public key_pad_base
  {
  public:

    //*********************************
    key_pad()
      : columns{Row_Inactive, Row_Inactive, Row_Inactive, Row_Inactive} // All columns are inactive (high) at reset
      , column_index(0)
    {
    }

    //*********************************
    virtual ~key_pad() {}

    //*********************************
    // Overridden function to set the column index.
    void select_column(column_index_type column_index_) override
    {
      column_index = column_index_;
    }

    //*********************************
    // Overridden function to read the row value.
    column_type read_column() override
    {
      return columns[column_index];
    }

    //*********************************
    // Reset the key pad to its initial state.
    void set_all_keys_off()
    {
      std::fill(std::begin(columns), std::end(columns), Row_Inactive); // All keys are inactive (high)
    }

    //*********************************
    // Set a specific key to active (low) state.
    void set_key_on(const std::pair<size_t, size_t>& key)
    {
      columns[key.second] &= ~(1 << key.first); // Set the key to active (low)
    }

    //*********************************
    // Set a specific key to inactive (high) state.
    void set_key_off(const std::pair<size_t, size_t>& key)
    {
      columns[key.second] |= (1 << key.first); // Set the key to inactive (high)
    }

    //*********************************
    // A list of key positions indexed as row/column.
    std::array<std::pair<size_t, size_t>, NRows * NColumns> key_positions = {
      {{0, 0}, {0, 1}, {0, 2}, {0, 3}, {1, 0}, {1, 1}, {1, 2}, {1, 3}, {2, 0}, {2, 1}, {2, 2}, {2, 3}}};

    std::pair<size_t, size_t>& Key1  = key_positions[0];  // Row 0, Column 0
    std::pair<size_t, size_t>& Key2  = key_positions[1];  // Row 0, Column 1
    std::pair<size_t, size_t>& Key3  = key_positions[2];  // Row 0, Column 2
    std::pair<size_t, size_t>& Key4  = key_positions[3];  // Row 0, Column 3
    std::pair<size_t, size_t>& Key5  = key_positions[4];  // Row 1, Column 0
    std::pair<size_t, size_t>& Key6  = key_positions[5];  // Row 1, Column 1
    std::pair<size_t, size_t>& Key7  = key_positions[6];  // Row 1, Column 2
    std::pair<size_t, size_t>& Key8  = key_positions[7];  // Row 1, Column 3
    std::pair<size_t, size_t>& Key9  = key_positions[8];  // Row 2, Column 0
    std::pair<size_t, size_t>& Key10 = key_positions[9];  // Row 2, Column 1
    std::pair<size_t, size_t>& Key11 = key_positions[10]; // Row 2, Column 2
    std::pair<size_t, size_t>& Key12 = key_positions[11]; // Row 2, Column 3

    const column_type Row_Inactive = 7; // All columns are inactive (high) when no keys are pressed.

    // The following are used to simulate the key pad in the unit tests.
    volatile column_type       columns[NColumns];
    volatile column_index_type column_index;
  };

  SUITE(test_key_matrix_active_low)
  {
    //*************************************************************************
    TEST(test_key_matrix_status)
    {
      key_pad pad;

      CHECK_EQUAL(NRows, pad.row_size());
      CHECK_EQUAL(NColumns, pad.column_size());
      CHECK_EQUAL(NRows * NColumns, pad.matrix_size());
      CHECK_EQUAL(NKeys, pad.key_event_max_size());
      CHECK_EQUAL(0, pad.key_event_size());
      CHECK_EQUAL(false, pad.has_key_events());
    }

    //*************************************************************************
    TEST(test_key_matrix_one_key_becomes_pressed)
    {
      key_pad pad;

      //*******************************

      pad.set_all_keys_off();
      pad.set_key_on(pad.Key1);
      pad.set_key_on(pad.Key2);
      pad.set_key_on(pad.Key3);
      pad.set_key_on(pad.Key4);
      pad.set_key_on(pad.Key5);
      pad.set_key_on(pad.Key6);
      pad.set_key_on(pad.Key7);
      pad.set_key_on(pad.Key8);
      pad.set_key_on(pad.Key9);
      pad.set_key_on(pad.Key10);
      pad.set_key_on(pad.Key11);
      pad.set_key_on(pad.Key12);

      for (size_t i = 0; i < Valid_Count; ++i)
      {
        pad.scan();
      }

      size_t key_event_count = pad.key_event_size();

      std::vector<key_pad::key_event> keys_pressed;

      for (size_t i = 0; i < key_event_count; ++i)
      {
        keys_pressed.push_back(pad.get_next_key_event());
      }

      std::sort(keys_pressed.begin(), keys_pressed.end(), [](const key_pad::key_event& a, const key_pad::key_event& b) { return a.key < b.key; });

      //*******************************

      pad.set_all_keys_off();
      pad.set_key_on(pad.Key1);
      keys_pressed.clear();

      for (size_t i = 0; i < Hold_Count; ++i)
      {
        pad.scan();
      }

      key_event_count = pad.key_event_size();

      for (size_t i = 0; i < key_event_count; ++i)
      {
        keys_pressed.push_back(pad.get_next_key_event());
      }

      std::sort(keys_pressed.begin(), keys_pressed.end(), [](const key_pad::key_event& a, const key_pad::key_event& b) { return a.key < b.key; });

      //*******************************

      keys_pressed.clear();

      for (size_t i = 0; i < Repeat_Count; ++i)
      {
        pad.scan();
      }

      key_event_count = pad.key_event_size();

      for (size_t i = 0; i < key_event_count; ++i)
      {
        keys_pressed.push_back(pad.get_next_key_event());
      }

      std::sort(keys_pressed.begin(), keys_pressed.end(), [](const key_pad::key_event& a, const key_pad::key_event& b) { return a.key < b.key; });

      //*******************************

      keys_pressed.clear();

      pad.set_all_keys_off();

      for (size_t i = 0; i < Hold_Count; ++i)
      {
        pad.scan();
      }

      key_event_count = pad.key_event_size();

      for (size_t i = 0; i < key_event_count; ++i)
      {
        keys_pressed.push_back(pad.get_next_key_event());
      }

      std::sort(keys_pressed.begin(), keys_pressed.end(), [](const key_pad::key_event& a, const key_pad::key_event& b) { return a.key < b.key; });

      keys_pressed.clear();
    }
  }
} // namespace
