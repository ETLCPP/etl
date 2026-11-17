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
  const size_t   NRows                 = 3;
  const size_t   NColumns              = 4;
  const size_t   NKeys                 = 12;
  const uint32_t Valid_Count           = 4;
  const uint32_t Hold_Count            = 4;
  const uint32_t Repeat_Count          = 4;
  const bool     Filter_Ghost_Keys_On  = true;
  const bool     Filter_Ghost_Keys_Off = false;

  //*************************************************************************
  /// A port that provides the interface to the simulated key matrix hardware.
  //*************************************************************************
  struct MatrixPort
  {
    using row_type          = etl::key_matrix_traits<NRows, NColumns>::row_type;
    using column_index_type = etl::key_matrix_traits<NRows, NColumns>::column_index_type;

    //***************************************************************************
    /// Constructor.
    //***************************************************************************
    MatrixPort()
      : columns()
      , column_index(0)
    {
    }

    //***************************************************************************
    /// Write the column index to the key matrix.
    /// This function must be overridden to write the column index to the key matrix.
    ///\param column_index The index of the column to make active in the key matrix.
    //***************************************************************************
    void select_column(column_index_type column_index_)
    {
      column_index = column_index_;
    }

    //***************************************************************************
    /// Read a value from the key matrix.
    /// This function must be overridden to read a value from the key matrix.
    ///\return The value read from the key matrix rows for the currently selected column.
    //***************************************************************************
    row_type read_row()
    {
      return columns[column_index];
    }

    const row_type Row_Inactive = 0;

    // The following are used to simulate the key pad in the unit tests.
    volatile row_type          columns[NColumns];
    volatile column_index_type column_index;
  };

  //*************************************************************************
  // A key pad base class for testing.
  // NRows             = 3
  // NColumns          = 4
  // Valid_Count       = 4
  // Hold_Count        = 4
  // Repeat_Count      = 4
  // NKeys             = 12
  // Filter_Ghost_Keys = Filter_Ghost_Keys_On/Filter_Ghost_Keys_Off
  //*************************************************************************
  using key_pad_base_no_ghost_check =
    etl::key_matrix<MatrixPort, NRows, NColumns, Valid_Count, Hold_Count, Repeat_Count, NKeys, Filter_Ghost_Keys_Off>;

  using key_pad_base_ghost_check = etl::key_matrix<MatrixPort, NRows, NColumns, Valid_Count, Hold_Count, Repeat_Count, NKeys, Filter_Ghost_Keys_On>;

  //*************************************************************************
  // A simple key pad implementation for testing.
  //*************************************************************************
  template <typename TKeyPad>
  class KeyPad : public TKeyPad
  {
  public:

    using row_type          = typename TKeyPad::row_type;
    using column_index_type = typename TKeyPad::column_index_type;
    using key_type          = typename TKeyPad::key_type;

    //*********************************
    struct key_info
    {
      key_type id;
      size_t   column;
      size_t   row;
    };

    //*********************************
    KeyPad()
      : TKeyPad(port)
    {
    }

    //*********************************
    // Reset the key pad to its initial state.
    void set_all_keys_off()
    {
      std::fill(std::begin(port.columns), std::end(port.columns), Row_Inactive); // All keys are inactive (low)
    }

    //*********************************
    // Set a specific key to active (low) state.
    void set_key_on(const key_info& key)
    {
      port.columns[key.row] |= (1 << key.column); // Set the key to active (high)
    }

    //*********************************
    // Set a specific key to inactive (high) state.
    void set_key_off(const key_info& key)
    {
      port.columns[key.row] &= ~(1 << key.column); // Set the key to inactive (low)
    }

    //*********************************
    // Set a specific key to a state.
    void set_key(const key_info& key, bool pressed)
    {
      if (pressed)
      {
        set_key_on(key);
      }
      else
      {
        set_key_off(key);
      }
    }

    //*********************************
    // A list of key positions indexed as row/column.
    std::array<key_info, NRows * NColumns> key_positions = {
      {{0, 0, 0}, {1, 0, 1}, {2, 0, 2}, {3, 0, 3}, {4, 1, 0}, {5, 1, 1}, {6, 1, 2}, {7, 1, 3}, {8, 2, 0}, {9, 2, 1}, {10, 2, 2}, {11, 2, 3}}};

    key_info& Key1  = key_positions[0];  // Row 0, Column 0
    key_info& Key2  = key_positions[1];  // Row 0, Column 1
    key_info& Key3  = key_positions[2];  // Row 0, Column 2
    key_info& Key4  = key_positions[3];  // Row 0, Column 3
    key_info& Key5  = key_positions[4];  // Row 1, Column 0
    key_info& Key6  = key_positions[5];  // Row 1, Column 1
    key_info& Key7  = key_positions[6];  // Row 1, Column 2
    key_info& Key8  = key_positions[7];  // Row 1, Column 3
    key_info& Key9  = key_positions[8];  // Row 2, Column 0
    key_info& Key10 = key_positions[9];  // Row 2, Column 1
    key_info& Key11 = key_positions[10]; // Row 2, Column 2
    key_info& Key12 = key_positions[11]; // Row 2, Column 3

    const row_type Row_Inactive = 0; // All columns are inactive (high) when no keys are pressed.

    MatrixPort port;
  };

  using key_pad_no_ghost_check = KeyPad<key_pad_base_no_ghost_check>;
  using key_pad_ghost_check    = KeyPad<key_pad_base_ghost_check>;

  //*************************************************************************
  std::ostream& operator<<(std::ostream& os, const key_pad_no_ghost_check::key_event::key_state& state)
  {
    switch (state.value())
    {
      case etl::key_matrix_state::Off: os << "Off"; break;
      case etl::key_matrix_state::On: os << "On"; break;
      case etl::key_matrix_state::Held: os << "Held"; break;
      case etl::key_matrix_state::Repeating: os << "Repeating"; break;
      default: os << "Unknown"; break;
    }
    return os;
  }

  SUITE(test_key_matrix_active_high)
  {
    //*************************************************************************
    TEST(test_key_matrix_status)
    {
      key_pad_no_ghost_check pad;

      CHECK_EQUAL(NRows, pad.row_size());
      CHECK_EQUAL(NColumns, pad.column_size());
      CHECK_EQUAL(NRows * NColumns, pad.matrix_size());
      CHECK_EQUAL(NKeys, pad.max_number_of_key_events());
      CHECK_EQUAL(0, pad.number_of_key_events());
      CHECK_EQUAL(false, pad.has_key_events());
    }

    //*************************************************************************
    TEST(test_key_matrix_all_keys_pressed)
    {
      key_pad_no_ghost_check pad;

      using key_event = key_pad_no_ghost_check::key_event;

      //*******************************
      // Simulate pressing all keys on the key pad.
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

      size_t key_event_count = pad.number_of_key_events();

      std::vector<key_event> keys_pressed;

      for (size_t i = 0; i < key_event_count; ++i)
      {
        keys_pressed.push_back(pad.next_key_event());
      }

      std::sort(keys_pressed.begin(), keys_pressed.end(), [](const key_event& a, const key_event& b) { return a.key < b.key; });

      CHECK_EQUAL(12, keys_pressed.size());

      CHECK_EQUAL(0, keys_pressed[0].key);   // Key1
      CHECK_EQUAL(1, keys_pressed[1].key);   // Key2
      CHECK_EQUAL(2, keys_pressed[2].key);   // Key3
      CHECK_EQUAL(3, keys_pressed[3].key);   // Key4
      CHECK_EQUAL(4, keys_pressed[4].key);   // Key5
      CHECK_EQUAL(5, keys_pressed[5].key);   // Key6
      CHECK_EQUAL(6, keys_pressed[6].key);   // Key7
      CHECK_EQUAL(7, keys_pressed[7].key);   // Key8
      CHECK_EQUAL(8, keys_pressed[8].key);   // Key9
      CHECK_EQUAL(9, keys_pressed[9].key);   // Key10
      CHECK_EQUAL(10, keys_pressed[10].key); // Key11
      CHECK_EQUAL(11, keys_pressed[11].key); // Key12

      auto event = key_event::key_state::On;

      CHECK_EQUAL(key_event::key_state::On, keys_pressed[0].state);  // Key1
      CHECK_EQUAL(key_event::key_state::On, keys_pressed[1].state);  // Key2
      CHECK_EQUAL(key_event::key_state::On, keys_pressed[2].state);  // Key3
      CHECK_EQUAL(key_event::key_state::On, keys_pressed[3].state);  // Key4
      CHECK_EQUAL(key_event::key_state::On, keys_pressed[4].state);  // Key5
      CHECK_EQUAL(key_event::key_state::On, keys_pressed[5].state);  // Key6
      CHECK_EQUAL(key_event::key_state::On, keys_pressed[6].state);  // Key7
      CHECK_EQUAL(key_event::key_state::On, keys_pressed[7].state);  // Key8
      CHECK_EQUAL(key_event::key_state::On, keys_pressed[8].state);  // Key9
      CHECK_EQUAL(key_event::key_state::On, keys_pressed[9].state);  // Key10
      CHECK_EQUAL(key_event::key_state::On, keys_pressed[10].state); // Key11
      CHECK_EQUAL(key_event::key_state::On, keys_pressed[11].state); // Key12

      //*******************************
      // Simulate holding down Key1 for Hold_Count scans, then check that it is reported as Held.
      // All of the other keys are released, so they should be reported as Off.
      //*******************************
      pad.set_all_keys_off();
      pad.set_key_on(pad.Key1);
      keys_pressed.clear();

      for (size_t i = 0; i < Hold_Count; ++i)
      {
        pad.scan();
      }

      key_event_count = pad.number_of_key_events();

      for (size_t i = 0; i < key_event_count; ++i)
      {
        keys_pressed.push_back(pad.next_key_event());
      }

      std::sort(keys_pressed.begin(), keys_pressed.end(),
                [](const key_pad_no_ghost_check::key_event& a, const key_pad_no_ghost_check::key_event& b) { return a.key < b.key; });

      CHECK_EQUAL(12, keys_pressed.size());

      CHECK_EQUAL(0, keys_pressed[0].key);   // Key1
      CHECK_EQUAL(1, keys_pressed[1].key);   // Key2
      CHECK_EQUAL(2, keys_pressed[2].key);   // Key3
      CHECK_EQUAL(3, keys_pressed[3].key);   // Key4
      CHECK_EQUAL(4, keys_pressed[4].key);   // Key5
      CHECK_EQUAL(5, keys_pressed[5].key);   // Key6
      CHECK_EQUAL(6, keys_pressed[6].key);   // Key7
      CHECK_EQUAL(7, keys_pressed[7].key);   // Key8
      CHECK_EQUAL(8, keys_pressed[8].key);   // Key9
      CHECK_EQUAL(9, keys_pressed[9].key);   // Key10
      CHECK_EQUAL(10, keys_pressed[10].key); // Key11
      CHECK_EQUAL(11, keys_pressed[11].key); // Key12

      CHECK_EQUAL(key_event::key_state::Held, keys_pressed[0].state); // Key1
      CHECK_EQUAL(key_event::key_state::Off, keys_pressed[1].state);  // Key2
      CHECK_EQUAL(key_event::key_state::Off, keys_pressed[2].state);  // Key3
      CHECK_EQUAL(key_event::key_state::Off, keys_pressed[3].state);  // Key4
      CHECK_EQUAL(key_event::key_state::Off, keys_pressed[4].state);  // Key5
      CHECK_EQUAL(key_event::key_state::Off, keys_pressed[5].state);  // Key6
      CHECK_EQUAL(key_event::key_state::Off, keys_pressed[6].state);  // Key7
      CHECK_EQUAL(key_event::key_state::Off, keys_pressed[7].state);  // Key8
      CHECK_EQUAL(key_event::key_state::Off, keys_pressed[8].state);  // Key9
      CHECK_EQUAL(key_event::key_state::Off, keys_pressed[9].state);  // Key10
      CHECK_EQUAL(key_event::key_state::Off, keys_pressed[10].state); // Key11
      CHECK_EQUAL(key_event::key_state::Off, keys_pressed[11].state); // Key12

      //*******************************
      // Simulate holding down Key1 for Repeat_Count scans, then check that it is reported as Repeating.
      //*******************************
      keys_pressed.clear();

      for (size_t i = 0; i < Repeat_Count; ++i)
      {
        pad.scan();
      }

      key_event_count = pad.number_of_key_events();

      for (size_t i = 0; i < key_event_count; ++i)
      {
        keys_pressed.push_back(pad.next_key_event());
      }

      std::sort(keys_pressed.begin(), keys_pressed.end(),
                [](const key_pad_no_ghost_check::key_event& a, const key_pad_no_ghost_check::key_event& b) { return a.key < b.key; });

      CHECK_EQUAL(1, keys_pressed.size());
      CHECK_EQUAL(0, keys_pressed[0].key);                                 // Key1
      CHECK_EQUAL(key_event::key_state::Repeating, keys_pressed[0].state); // Key1

      //*******************************
      // Simulate releasing Key1, then check that it is reported as Off.
      //*******************************
      keys_pressed.clear();

      pad.set_all_keys_off();

      for (size_t i = 0; i < Hold_Count; ++i)
      {
        pad.scan();
      }

      key_event_count = pad.number_of_key_events();

      for (size_t i = 0; i < key_event_count; ++i)
      {
        keys_pressed.push_back(pad.next_key_event());
      }

      std::sort(keys_pressed.begin(), keys_pressed.end(),
                [](const key_pad_no_ghost_check::key_event& a, const key_pad_no_ghost_check::key_event& b) { return a.key < b.key; });

      CHECK_EQUAL(1, keys_pressed.size());
      CHECK_EQUAL(0, keys_pressed[0].key);                           // Key1
      CHECK_EQUAL(key_event::key_state::Off, keys_pressed[0].state); // Key1

      keys_pressed.clear();
    }

    //*************************************************************************
    TEST(test_key_matrix_overlapping_key_presses)
    {
      using key_event = key_pad_no_ghost_check::key_event;
      using key_state = key_event::key_state;

      const size_t Steps = 19;

      std::array<char, Steps> key1  = {1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
      std::array<char, Steps> key2  = {0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
      std::array<char, Steps> key3  = {0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
      std::array<char, Steps> key4  = {0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
      std::array<char, Steps> key5  = {0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
      std::array<char, Steps> key6  = {0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
      std::array<char, Steps> key7  = {0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0};
      std::array<char, Steps> key8  = {0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0};
      std::array<char, Steps> key9  = {0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0};
      std::array<char, Steps> key10 = {0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0};
      std::array<char, Steps> key11 = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0};
      std::array<char, Steps> key12 = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0};

      std::array<int, Steps> expected_key_events = {0, 0, 0, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 1, 1, 1, 1};

      std::array<key_state, Steps> key1_expected = {key_state::Off, key_state::Off, key_state::Off, key_state::On,  key_state::On,
                                                    key_state::On,  key_state::On,  key_state::Off, key_state::Off, key_state::Off,
                                                    key_state::Off, key_state::Off, key_state::Off, key_state::Off, key_state::Off,
                                                    key_state::Off, key_state::Off, key_state::Off, key_state::Off};

      std::array<key_state, Steps> key2_expected = {key_state::Off, key_state::Off, key_state::Off, key_state::Off, key_state::On,
                                                    key_state::On,  key_state::On,  key_state::On,  key_state::Off, key_state::Off,
                                                    key_state::Off, key_state::Off, key_state::Off, key_state::Off, key_state::Off,
                                                    key_state::Off, key_state::Off, key_state::Off, key_state::Off};

      std::array<key_state, Steps> key3_expected = {key_state::Off, key_state::Off, key_state::Off, key_state::Off, key_state::Off,
                                                    key_state::On,  key_state::On,  key_state::On,  key_state::On,  key_state::Off,
                                                    key_state::Off, key_state::Off, key_state::Off, key_state::Off, key_state::Off,
                                                    key_state::Off, key_state::Off, key_state::Off, key_state::Off};

      std::array<key_state, Steps> key4_expected = {key_state::Off, key_state::Off, key_state::Off, key_state::Off, key_state::Off,
                                                    key_state::Off, key_state::On,  key_state::On,  key_state::On,  key_state::On,
                                                    key_state::Off, key_state::Off, key_state::Off, key_state::Off, key_state::Off,
                                                    key_state::Off, key_state::Off, key_state::Off, key_state::Off};

      std::array<key_state, Steps> key5_expected = {key_state::Off, key_state::Off, key_state::Off, key_state::Off, key_state::Off,
                                                    key_state::Off, key_state::Off, key_state::On,  key_state::On,  key_state::On,
                                                    key_state::On,  key_state::Off, key_state::Off, key_state::Off, key_state::Off,
                                                    key_state::Off, key_state::Off, key_state::Off, key_state::Off};

      std::array<key_state, Steps> key6_expected = {key_state::Off, key_state::Off, key_state::Off, key_state::Off, key_state::Off,
                                                    key_state::Off, key_state::Off, key_state::Off, key_state::On,  key_state::On,
                                                    key_state::On,  key_state::On,  key_state::Off, key_state::Off, key_state::Off,
                                                    key_state::Off, key_state::Off, key_state::Off, key_state::Off};

      std::array<key_state, Steps> key7_expected = {key_state::Off, key_state::Off, key_state::Off, key_state::Off, key_state::Off,
                                                    key_state::Off, key_state::Off, key_state::Off, key_state::Off, key_state::On,
                                                    key_state::On,  key_state::On,  key_state::On,  key_state::Off, key_state::Off,
                                                    key_state::Off, key_state::Off, key_state::Off, key_state::Off};

      std::array<key_state, Steps> key8_expected = {key_state::Off, key_state::Off, key_state::Off, key_state::Off, key_state::Off,
                                                    key_state::Off, key_state::Off, key_state::Off, key_state::Off, key_state::Off,
                                                    key_state::On,  key_state::On,  key_state::On,  key_state::On,  key_state::Off,
                                                    key_state::Off, key_state::Off, key_state::Off, key_state::Off};

      std::array<key_state, Steps> key9_expected = {key_state::Off, key_state::Off, key_state::Off, key_state::Off, key_state::Off,
                                                    key_state::Off, key_state::Off, key_state::Off, key_state::Off, key_state::Off,
                                                    key_state::Off, key_state::On,  key_state::On,  key_state::On,  key_state::On,
                                                    key_state::Off, key_state::Off, key_state::Off, key_state::Off};

      std::array<key_state, Steps> key10_expected = {key_state::Off, key_state::Off, key_state::Off, key_state::Off, key_state::Off,
                                                     key_state::Off, key_state::Off, key_state::Off, key_state::Off, key_state::Off,
                                                     key_state::Off, key_state::Off, key_state::On,  key_state::On,  key_state::On,
                                                     key_state::On,  key_state::Off, key_state::Off, key_state::Off};

      std::array<key_state, Steps> key11_expected = {key_state::Off, key_state::Off, key_state::Off, key_state::Off, key_state::Off,
                                                     key_state::Off, key_state::Off, key_state::Off, key_state::Off, key_state::Off,
                                                     key_state::Off, key_state::Off, key_state::Off, key_state::On,  key_state::On,
                                                     key_state::On,  key_state::On,  key_state::Off, key_state::Off};

      std::array<key_state, Steps> key12_expected = {key_state::Off, key_state::Off, key_state::Off, key_state::Off, key_state::Off,
                                                     key_state::Off, key_state::Off, key_state::Off, key_state::Off, key_state::Off,
                                                     key_state::Off, key_state::Off, key_state::Off, key_state::Off, key_state::On,
                                                     key_state::On,  key_state::On,  key_state::On,  key_state::Off};

      key_pad_no_ghost_check pad;

      pad.set_all_keys_off();

      for (size_t i = 0; i < Steps; ++i)
      {
        pad.set_key(pad.Key1, key1[i]);
        pad.set_key(pad.Key2, key2[i]);
        pad.set_key(pad.Key3, key3[i]);
        pad.set_key(pad.Key4, key4[i]);
        pad.set_key(pad.Key5, key5[i]);
        pad.set_key(pad.Key6, key6[i]);
        pad.set_key(pad.Key7, key7[i]);
        pad.set_key(pad.Key8, key8[i]);
        pad.set_key(pad.Key9, key9[i]);
        pad.set_key(pad.Key10, key10[i]);
        pad.set_key(pad.Key11, key11[i]);
        pad.set_key(pad.Key12, key12[i]);

        pad.scan();

        CHECK_EQUAL(expected_key_events[i], pad.number_of_key_events());

        CHECK_EQUAL(key1_expected[i], pad.get_key_state(pad.Key1.id));
        CHECK_EQUAL(key2_expected[i], pad.get_key_state(pad.Key2.id));
        CHECK_EQUAL(key3_expected[i], pad.get_key_state(pad.Key3.id));
        CHECK_EQUAL(key4_expected[i], pad.get_key_state(pad.Key4.id));
        CHECK_EQUAL(key5_expected[i], pad.get_key_state(pad.Key5.id));
        CHECK_EQUAL(key6_expected[i], pad.get_key_state(pad.Key6.id));
        CHECK_EQUAL(key7_expected[i], pad.get_key_state(pad.Key7.id));
        CHECK_EQUAL(key8_expected[i], pad.get_key_state(pad.Key8.id));
        CHECK_EQUAL(key9_expected[i], pad.get_key_state(pad.Key9.id));
        CHECK_EQUAL(key10_expected[i], pad.get_key_state(pad.Key10.id));
        CHECK_EQUAL(key11_expected[i], pad.get_key_state(pad.Key11.id));
        CHECK_EQUAL(key12_expected[i], pad.get_key_state(pad.Key12.id));

        pad.clear_key_events();
      }
    }

    //*************************************************************************
    TEST(test_key_matrix_check_ghost_suppression)
    {
      key_pad_ghost_check pad;

      pad.set_all_keys_off();
      pad.set_key_on(pad.Key6);
      pad.set_key_on(pad.Key11);

      for (size_t i = 0; i < Valid_Count + 1; ++i)
      {
        if (i == 1)
        {
          // Simulate a ghosting condition by pressing two keys that would create a ghost key.
          pad.set_key_on(pad.Key7);
          pad.set_key_on(pad.Key10);
        }

        pad.scan();
      }

      size_t key_event_count = pad.number_of_key_events();

      std::vector<key_pad_ghost_check::key_event> keys_pressed;

      for (size_t i = 0; i < key_event_count; ++i)
      {
        keys_pressed.push_back(pad.next_key_event());
      }

      std::sort(keys_pressed.begin(), keys_pressed.end(),
                [](const key_pad_ghost_check::key_event& a, const key_pad_ghost_check::key_event& b) { return a.key < b.key; });

      CHECK_EQUAL(2, keys_pressed.size());
      CHECK_EQUAL(5, keys_pressed[0].key);  // Key6
      CHECK_EQUAL(10, keys_pressed[1].key); // Key11
    }
  }
} // namespace
