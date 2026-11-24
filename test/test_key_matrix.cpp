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

#include <algorithm>
#include <array>
#include <vector>

namespace
{
  const size_t   N_Read                = 3;
  const size_t   N_Write               = 4;
  const size_t   N_Key_Events          = 12;
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
    using read_type        = etl::key_matrix_traits<N_Read, N_Write>::read_type;
    using write_index_type = etl::key_matrix_traits<N_Read, N_Write>::write_index_type;

    //***************************************************************************
    /// Constructor.
    //***************************************************************************
    MatrixPort()
      : read_channels()
      , write_index(0)
    {
    }

    //***************************************************************************
    /// Set the write an index to the key matrix.
    //***************************************************************************
    void write_key_matrix(write_index_type write_index_)
    {
      write_index = write_index_;
    }

    //***************************************************************************
    /// Read a value from the key matrix.
    //***************************************************************************
    read_type read_key_matrix()
    {
      return read_channels[write_index];
    }

    const read_type Read_Inactive = 0;

    // The following are used to simulate the key pad in the unit tests.
    volatile read_type        read_channels[N_Write];
    volatile write_index_type write_index;
  };

  //*************************************************************************
  // A key pad base class for testing.
  // N_Read            = 3
  // N_Write           = 4
  // Valid_Count       = 4
  // Hold_Count        = 4
  // Repeat_Count      = 4
  // N_Key_Events      = 12
  // Filter_Ghost_Keys = Filter_Ghost_Keys_On/Filter_Ghost_Keys_Off
  //*************************************************************************
  using key_pad_base_no_ghost_check =
    etl::key_matrix<MatrixPort, N_Read, N_Write, Valid_Count, Hold_Count, Repeat_Count, N_Key_Events, Filter_Ghost_Keys_Off>;

  using key_pad_base_ghost_check =
    etl::key_matrix<MatrixPort, N_Read, N_Write, Valid_Count, Hold_Count, Repeat_Count, N_Key_Events, Filter_Ghost_Keys_On>;

  //*************************************************************************
  // A simple key pad implementation for testing.
  //*************************************************************************
  template <typename TKeyPad>
  class KeyPad : public TKeyPad
  {
  public:

    using read_type        = typename TKeyPad::read_type;
    using write_index_type = typename TKeyPad::write_index_type;
    using key_type         = typename TKeyPad::key_type;

    //*********************************
    struct key_info
    {
      key_type id;
      size_t   write_index;
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
      std::fill(std::begin(port.read_channels), std::end(port.read_channels), Read_Inactive); // All keys are inactive (low)
    }

    //*********************************
    // Set a specific key to active (low) state.
    void set_key_on(const key_info& key)
    {
      port.read_channels[key.row] |= (1 << key.write_index); // Set the key to active (high)
    }

    //*********************************
    // Set a specific key to inactive (high) state.
    void set_key_off(const key_info& key)
    {
      port.read_channels[key.row] &= ~(1 << key.write_index); // Set the key to inactive (low)
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
    // A list of key positions indexed as row/write_index.
    std::array<key_info, N_Read * N_Write> key_positions = {
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

    const read_type Read_Inactive = 0; // All read_channels are inactive (high) when no keys are pressed.

    MatrixPort port;
  };

  using key_pad_no_ghost_check = KeyPad<key_pad_base_no_ghost_check>;
  using key_pad_ghost_check    = KeyPad<key_pad_base_ghost_check>;

  SUITE(test_key_matrix_active_high)
  {
    //*************************************************************************
    TEST(test_key_matrix_status)
    {
      key_pad_no_ghost_check pad;

      CHECK_EQUAL(N_Read, pad.read_size());
      CHECK_EQUAL(N_Write, pad.write_size());
      CHECK_EQUAL(N_Read * N_Write, pad.matrix_size());
      CHECK_EQUAL(N_Key_Events, pad.max_number_of_key_events());
      CHECK_EQUAL(0, pad.number_of_key_events());
      CHECK_EQUAL(false, pad.has_key_events());
      CHECK_EQUAL(pad.Invalid_Key, pad.next_key_event().key);
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

      bool key_events_pending = false;

      for (size_t i = 0; i < Valid_Count; ++i)
      {
        key_events_pending = pad.scan();
      }

      CHECK_TRUE(key_events_pending);

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

      CHECK_EQUAL(key_event::key_state::On, keys_pressed[0].state.value());  // Key1
      CHECK_EQUAL(key_event::key_state::On, keys_pressed[1].state.value());  // Key2
      CHECK_EQUAL(key_event::key_state::On, keys_pressed[2].state.value());  // Key3
      CHECK_EQUAL(key_event::key_state::On, keys_pressed[3].state.value());  // Key4
      CHECK_EQUAL(key_event::key_state::On, keys_pressed[4].state.value());  // Key5
      CHECK_EQUAL(key_event::key_state::On, keys_pressed[5].state.value());  // Key6
      CHECK_EQUAL(key_event::key_state::On, keys_pressed[6].state.value());  // Key7
      CHECK_EQUAL(key_event::key_state::On, keys_pressed[7].state.value());  // Key8
      CHECK_EQUAL(key_event::key_state::On, keys_pressed[8].state.value());  // Key9
      CHECK_EQUAL(key_event::key_state::On, keys_pressed[9].state.value());  // Key10
      CHECK_EQUAL(key_event::key_state::On, keys_pressed[10].state.value()); // Key11
      CHECK_EQUAL(key_event::key_state::On, keys_pressed[11].state.value()); // Key12

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

      CHECK_EQUAL(key_event::key_state::Held, keys_pressed[0].state.value()); // Key1
      CHECK_EQUAL(key_event::key_state::Off, keys_pressed[1].state.value());  // Key2
      CHECK_EQUAL(key_event::key_state::Off, keys_pressed[2].state.value());  // Key3
      CHECK_EQUAL(key_event::key_state::Off, keys_pressed[3].state.value());  // Key4
      CHECK_EQUAL(key_event::key_state::Off, keys_pressed[4].state.value());  // Key5
      CHECK_EQUAL(key_event::key_state::Off, keys_pressed[5].state.value());  // Key6
      CHECK_EQUAL(key_event::key_state::Off, keys_pressed[6].state.value());  // Key7
      CHECK_EQUAL(key_event::key_state::Off, keys_pressed[7].state.value());  // Key8
      CHECK_EQUAL(key_event::key_state::Off, keys_pressed[8].state.value());  // Key9
      CHECK_EQUAL(key_event::key_state::Off, keys_pressed[9].state.value());  // Key10
      CHECK_EQUAL(key_event::key_state::Off, keys_pressed[10].state.value()); // Key11
      CHECK_EQUAL(key_event::key_state::Off, keys_pressed[11].state.value()); // Key12

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
      CHECK_EQUAL(0, keys_pressed[0].key);                                         // Key1
      CHECK_EQUAL(key_event::key_state::Repeating, keys_pressed[0].state.value()); // Key1

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
      CHECK_EQUAL(0, keys_pressed[0].key);                                   // Key1
      CHECK_EQUAL(key_event::key_state::Off, keys_pressed[0].state.value()); // Key1

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

        CHECK_EQUAL(key1_expected[i].value(), pad.get_key_state(pad.Key1.id).value());
        CHECK_EQUAL(key2_expected[i].value(), pad.get_key_state(pad.Key2.id).value());
        CHECK_EQUAL(key3_expected[i].value(), pad.get_key_state(pad.Key3.id).value());
        CHECK_EQUAL(key4_expected[i].value(), pad.get_key_state(pad.Key4.id).value());
        CHECK_EQUAL(key5_expected[i].value(), pad.get_key_state(pad.Key5.id).value());
        CHECK_EQUAL(key6_expected[i].value(), pad.get_key_state(pad.Key6.id).value());
        CHECK_EQUAL(key7_expected[i].value(), pad.get_key_state(pad.Key7.id).value());
        CHECK_EQUAL(key8_expected[i].value(), pad.get_key_state(pad.Key8.id).value());
        CHECK_EQUAL(key9_expected[i].value(), pad.get_key_state(pad.Key9.id).value());
        CHECK_EQUAL(key10_expected[i].value(), pad.get_key_state(pad.Key10.id).value());
        CHECK_EQUAL(key11_expected[i].value(), pad.get_key_state(pad.Key11.id).value());
        CHECK_EQUAL(key12_expected[i].value(), pad.get_key_state(pad.Key12.id).value());

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
