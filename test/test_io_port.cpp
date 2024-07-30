/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl
https://www.etlcpp.com

Copyright(c) 2014 John Wellbelove

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

#include "etl/io_port.h"

#include <stdint.h>
#include <array>
#include <algorithm>

uint8_t rw  = 0x12U;
uint8_t ro  = 0x34U;
uint8_t wo  = 0x56U;
uint8_t wos = 0x78U;

namespace
{
  template <uintptr_t Address>
  struct serial_port
  {
    etl::io_port_ro<uint8_t,  Address>     rxdata;
    etl::io_port_wo<uint8_t,  Address + 1> txdata;
    etl::io_port_rw<uint16_t, Address + 2> control;
    etl::io_port_ro<uint16_t, Address + 4> status;
    etl::io_port_wos<uint8_t, Address + 6> control2;
  };

  struct dynamic_serial_port
  {
    dynamic_serial_port(uint8_t*  rxdata_reg,
                        uint8_t*  txdata_reg,
                        uint16_t* control_reg,
                        uint16_t* status_reg,
                        uint8_t*  control2_reg)
      : rxdata(rxdata_reg)
      , txdata(txdata_reg)
      , control(control_reg)
      , status(status_reg)
      , control2(control2_reg)
    {
    }

    etl::io_port_ro<uint8_t>  rxdata;
    etl::io_port_wo<uint8_t>  txdata;
    etl::io_port_rw<uint16_t> control;
    etl::io_port_ro<uint16_t> status;
    etl::io_port_wos<uint8_t> control2;
  };
}

namespace
{
  SUITE(test_io_ports)
  {
    //*************************************************************************
    TEST(test_io_port_type_traits)
    {
      using iop_rw_t  = etl::io_port_rw<uint8_t>;
      using iop_ro_t  = etl::io_port_ro<uint8_t>;
      using iop_wo_t  = etl::io_port_wo<uint8_t>;
      using iop_wos_t = etl::io_port_wos<uint8_t>;

      // Check IOP value_type
      CHECK_TRUE((std::is_same<uint8_t, iop_rw_t::value_type>::value));
      CHECK_TRUE((std::is_same<uint8_t, iop_ro_t::value_type>::value));
      CHECK_TRUE((std::is_same<uint8_t, iop_wo_t::value_type>::value));
      CHECK_TRUE((std::is_same<uint8_t, iop_wos_t::value_type>::value));

      // Check IOP::iterator io_port_type
      CHECK_TRUE((std::is_same<iop_rw_t,  iop_rw_t::iterator::io_port_type>::value));
      CHECK_TRUE((std::is_same<iop_wo_t,  iop_wo_t::iterator::io_port_type>::value));
      CHECK_TRUE((std::is_same<iop_wos_t, iop_wos_t::iterator::io_port_type>::value));

      // Check IOP::const_iterator io_port_type
      CHECK_TRUE((std::is_same<iop_rw_t,  iop_rw_t::const_iterator::io_port_type>::value));
      CHECK_TRUE((std::is_same<iop_ro_t,  iop_ro_t::const_iterator::io_port_type>::value));
      CHECK_TRUE((std::is_same<iop_wos_t, iop_wos_t::const_iterator::io_port_type>::value));

      // Check IOP::iterator value_type
      CHECK_TRUE((std::is_same<uint8_t, iop_rw_t::iterator::value_type>::value));
      CHECK_TRUE((std::is_same<uint8_t, iop_wo_t::iterator::value_type>::value));
      CHECK_TRUE((std::is_same<uint8_t, iop_wos_t::iterator::value_type>::value));

      // Check IOP::const_iterator value_type
      CHECK_TRUE((std::is_same<const uint8_t, iop_rw_t::const_iterator::value_type>::value));
      CHECK_TRUE((std::is_same<const uint8_t, iop_ro_t::const_iterator::value_type>::value));
      CHECK_TRUE((std::is_same<const uint8_t, iop_wos_t::const_iterator::value_type>::value));
    }   

    //*************************************************************************
    TEST(test_static_io_port)
    {
      // Without a compile time address, the static address IO ports cannot be tested.
      serial_port<0x1234U> port;
    }

    //*************************************************************************
    TEST(test_dynamic_io_port_rw_constructors_and_assignment)
    {
      uint8_t  data1 = 0x12U;
      uint8_t  data2 = 0x34U;

      etl::io_port_rw<uint8_t> data_rw1a(&data1);
      etl::io_port_rw<uint8_t> data_rw1b(data_rw1a);
      etl::io_port_rw<uint8_t> data_rw2a(&data2);
      etl::io_port_rw<uint8_t> data_rw2b(nullptr);
      data_rw2b = data_rw2a;

      CHECK_EQUAL(&data1, data_rw1a.get_address());
      CHECK_EQUAL(&data1, data_rw1b.get_address());
      CHECK_EQUAL(&data2, data_rw2a.get_address());
      CHECK_EQUAL(&data2, data_rw2b.get_address());
    }

    //*************************************************************************
    TEST(test_dynamic_io_port_ro_constructors_and_assignment)
    {
      uint8_t  data1 = 0x12U;
      uint8_t  data2 = 0x34U;

      etl::io_port_ro<uint8_t> data_ro1a(&data1);
      etl::io_port_ro<uint8_t> data_ro1b(data_ro1a);
      etl::io_port_ro<uint8_t> data_ro2a(&data2);
      etl::io_port_ro<uint8_t> data_ro2b(nullptr);
      data_ro2b = data_ro2a;

      CHECK_EQUAL(&data1, data_ro1a.get_address());
      CHECK_EQUAL(&data1, data_ro1b.get_address());
      CHECK_EQUAL(&data2, data_ro2a.get_address());
      CHECK_EQUAL(&data2, data_ro2b.get_address());
    }

    //*************************************************************************
    TEST(test_dynamic_io_port_wo_constructors_and_assignment)
    {
      uint8_t  data1 = 0x12U;
      uint8_t  data2 = 0x34U;

      etl::io_port_wo<uint8_t> data_wo1a(&data1);
      etl::io_port_wo<uint8_t> data_wo1b(data_wo1a);
      etl::io_port_wo<uint8_t> data_wo2a(&data2);
      etl::io_port_wo<uint8_t> data_wo2b(nullptr);
      data_wo2b = data_wo2a;

      CHECK_EQUAL(&data1, data_wo1a.get_address());
      CHECK_EQUAL(&data1, data_wo1b.get_address());
      CHECK_EQUAL(&data2, data_wo2a.get_address());
      CHECK_EQUAL(&data2, data_wo2b.get_address());
    }

    //*************************************************************************
    TEST(test_dynamic_io_port_wos_constructors_and_assignment)
    {
      uint8_t  data1 = 0x12U;
      uint8_t  data2 = 0x34U;

      etl::io_port_wos<uint8_t> data_wos1a(&data1);
      etl::io_port_wos<uint8_t> data_wos1b(data_wos1a);
      etl::io_port_wos<uint8_t> data_wos2a(&data2);
      etl::io_port_wos<uint8_t> data_wos2b(nullptr);
      data_wos2b = data_wos2a;

      CHECK_EQUAL(&data1, data_wos1a.get_address());
      CHECK_EQUAL(&data1, data_wos1b.get_address());
      CHECK_EQUAL(&data2, data_wos2a.get_address());
      CHECK_EQUAL(&data2, data_wos2b.get_address());
    }

    //*************************************************************************
    TEST(test_dynamic_io_port_access)
    {
      const uint8_t  RxData       = 0x01U;
      const uint8_t  TxData       = 0x12U;
      const uint16_t ControlData  = 0x2345U;
      const uint16_t StatuslData  = 0x3456U;
      const uint8_t  Control2Data = 0x67U;

      uint8_t  rxdata_register   = RxData;
      uint8_t  txdata_register   = 0;
      uint16_t control_register  = 0;
      uint16_t status_register   = StatuslData;
      uint8_t  control2_register = 0;

      dynamic_serial_port port(&rxdata_register,
                               &txdata_register,
                               &control_register,
                               &status_register,
                               &control2_register);

      // Check read from the Rx data register
      CHECK_EQUAL(RxData, port.rxdata);
      CHECK_EQUAL(RxData, rxdata_register);

      // Check write to the Tx data register
      port.txdata = TxData;
      CHECK_EQUAL(TxData, txdata_register);

      // Check write to the control register
      port.control = ControlData;
      CHECK_EQUAL(ControlData, control_register);

      // Check read from the status data register
      CHECK_EQUAL(StatuslData, port.status);
      CHECK_EQUAL(StatuslData, status_register);

      // Check write to the control2 register
      port.control2 = Control2Data;
      CHECK_EQUAL(Control2Data, control2_register);

      // Set and get an address      
      port.control2.set_address((void*)0x1000U);
      volatile uint8_t* address = port.control2.get_address();
      CHECK_EQUAL(reinterpret_cast<volatile uint8_t*>(0x1000U), address);
    }

    //*************************************************************************
    TEST(test_dynamic_io_port_iterators)
    {
      etl::io_port_rw<uint8_t>  iop_rw;
      etl::io_port_ro<uint8_t>  iop_ro;
      etl::io_port_wo<uint8_t>  iop_wo;
      etl::io_port_wos<uint8_t> iop_wos;

      uint8_t memory_rw  = 0x12U;
      uint8_t memory_ro  = 0x34U;
      uint8_t memory_wo  = 0x56U;
      uint8_t memory_wos = 0x78U;

      iop_rw.set_address(&memory_rw);
      iop_ro.set_address(&memory_ro);
      iop_wo.set_address(&memory_wo);
      iop_wos.set_address(&memory_wos);

      std::array<uint8_t, 10> compare;
      std::array<uint8_t, 10> result;

      // Read from RW IOP.
      std::copy_n(iop_rw.citer(), result.size(), result.begin());
      compare.fill(0x12U);

      for (size_t i = 0UL; i < compare.size(); ++i)
      {
        CHECK_EQUAL(compare[i], result[i]);
      }

      // Write to RW IOP.
      compare.fill(0x34U);
      std::copy_n(compare.begin(), compare.size(), iop_rw.iter());

      CHECK_EQUAL(compare[0], iop_rw);

      // Read from RO IOP.
      std::copy_n(iop_ro.citer(), result.size(), result.begin());
      compare.fill(0x34U);

      for (size_t i = 0UL; i < compare.size(); ++i)
      {
        CHECK_EQUAL(compare[i], result[i]);
      }

      // Write to WO IOP.
      compare.fill(0x56U);
      std::copy_n(compare.begin(), 1, iop_wo.iter());

      CHECK_EQUAL(compare[0], memory_wo);

      // Read from WOS IOP.
      iop_wos = 0x78U;

      std::copy_n(iop_wos.citer(), result.size(), result.begin());
      compare.fill(0x78U);

      for (size_t i = 0UL; i < compare.size(); ++i)
      {
        CHECK_EQUAL(compare[i], result[i]);
      }

      // Write to WOS IOP.
      compare.fill(0x90U);
      std::copy_n(compare.begin(), 1, iop_wos.iter());

      CHECK_EQUAL(compare[0], iop_wos);
    }

    //*************************************************************************
    TEST(test_dynamic_io_port_rw_binary_operators)
    {
      uint8_t value   = 0;
      uint8_t compare = 0x5A;

      etl::io_port_rw<uint8_t> iop_rw;

      iop_rw.set_address(&value);

      compare = value;
      iop_rw |= 0xA1;
      CHECK_EQUAL(compare | 0xA1, iop_rw.read());

      compare = value;
      iop_rw ^= 0xA5;
      CHECK_EQUAL(compare ^ 0xA5, iop_rw.read());

      compare = value;
      iop_rw <<= 1;
      CHECK_EQUAL(uint8_t(compare << 1), iop_rw.read());

      compare = value;
      iop_rw >>= 1;
      CHECK_EQUAL(uint8_t(compare >> 1), iop_rw.read());

      compare = value;
      uint8_t not_value = ~iop_rw;
      CHECK_EQUAL(uint8_t(~compare), not_value);
    }

    //*************************************************************************
    TEST(test_dynamic_io_port_wos_binary_operators)
    {
      uint8_t value   = 0;
      uint8_t compare = 0x5A;

      etl::io_port_wos<uint8_t> iop_wos;

      iop_wos.set_address(&value);
      iop_wos.write(0x5A);

      iop_wos &= 0x0F;
      CHECK_EQUAL(compare & 0x0F, iop_wos.read());

      compare = value;
      iop_wos |= 0xA1;
      CHECK_EQUAL(compare | 0xA1, iop_wos.read());

      compare = value;
      iop_wos ^= 0xA5;
      CHECK_EQUAL(compare ^ 0xA5, iop_wos.read());

      compare = value;
      iop_wos <<= 1;
      CHECK_EQUAL(uint8_t(compare << 1), iop_wos.read());

      compare = value;
      iop_wos >>= 1;
      CHECK_EQUAL(uint8_t(compare >> 1), iop_wos.read());

      compare = value;
      uint8_t not_value = ~iop_wos;
      CHECK_EQUAL(uint8_t(~compare), not_value);
    }

    //*************************************************************************
    TEST(test_io_port_rw_swap)
    {
      using iop_t = etl::io_port_rw<uint8_t>;

      uint8_t memory1 = 0x12U;
      uint8_t memory2 = 0x34U;

      iop_t iop1;
      iop_t iop2;

      iop1.set_address(&memory1);
      iop2.set_address(&memory2);

      // Swap io_ports
      ETL_OR_STD::swap(iop1, iop2);

      iop1.write(0x56);
      iop2.write(0x78);

      CHECK_EQUAL(memory1, iop2.read());
      CHECK_EQUAL(memory2, iop1.read());

      // Swap iterators
      auto itr1 = iop1.iter();
      auto itr2 = iop2.iter();

      ETL_OR_STD::swap(itr1, itr2);

      CHECK_EQUAL(iop1.read(), *itr2);
      CHECK_EQUAL(iop2.read(), *itr1);

      // Swap const iterators
      auto itr3 = iop1.citer();
      auto itr4 = iop2.citer();

      ETL_OR_STD::swap(itr3, itr4);

      CHECK_EQUAL(iop1.read(), *itr4);
      CHECK_EQUAL(iop2.read(), *itr3);
    }

    //*************************************************************************
    TEST(test_io_port_ro_swap)
    {
      using iop_t = etl::io_port_ro<uint8_t>;

      uint8_t memory1 = 0x12U;
      uint8_t memory2 = 0x34U;

      iop_t iop1;
      iop_t iop2;

      iop1.set_address(&memory1);
      iop2.set_address(&memory2);

      // Swap io_ports
      ETL_OR_STD::swap(iop1, iop2);

      CHECK_EQUAL(memory1, iop2.read());
      CHECK_EQUAL(memory2, iop1.read());

      // Swap const iterators
      auto itr1 = iop1.citer();
      auto itr2 = iop2.citer();

      ETL_OR_STD::swap(itr1, itr2);

      CHECK_EQUAL(iop1.read(), *itr2);
      CHECK_EQUAL(iop2.read(), *itr1);
    }

    //*************************************************************************
    TEST(test_io_port_wo_swap)
    {
      using iop_t = etl::io_port_wo<uint8_t>;

      uint8_t memory1 = 0x12U;
      uint8_t memory2 = 0x34U;

      iop_t iop1;
      iop_t iop2;

      iop1.set_address(&memory1);
      iop2.set_address(&memory2);

      // Swap io_ports
      ETL_OR_STD::swap(iop1, iop2);

      iop1.write(0x56);
      iop2.write(0x78);

      CHECK_EQUAL(memory1, 0x78);
      CHECK_EQUAL(memory2, 0x56);

      // Swap iterators
      auto itr1 = iop1.iter();
      auto itr2 = iop2.iter();

      ETL_OR_STD::swap(itr1, itr2);

      *itr1 = 0x9A;
      *itr2 = 0xBC;

      CHECK_EQUAL(memory1, 0x9A);
      CHECK_EQUAL(memory2, 0xBC);
    }

    //*************************************************************************
    TEST(test_io_port_wos_swap)
    {
      using iop_t = etl::io_port_wos<uint8_t>;

      uint8_t memory1 = 0x12U;
      uint8_t memory2 = 0x34U;

      iop_t iop1;
      iop_t iop2;

      iop1.set_address(&memory1);
      iop2.set_address(&memory2);

      // Swap io_ports
      ETL_OR_STD::swap(iop1, iop2);

      iop1.write(0x56);
      iop2.write(0x78);

      CHECK_EQUAL(memory1, iop2.read());
      CHECK_EQUAL(memory2, iop1.read());

      // Swap iterators
      auto itr1 = iop1.iter();
      auto itr2 = iop2.iter();

      ETL_OR_STD::swap(itr1, itr2);

      CHECK_EQUAL(iop1.read(), *itr2);
      CHECK_EQUAL(iop2.read(), *itr1);

      // Swap const iterators
      auto itr3 = iop1.citer();
      auto itr4 = iop2.citer();

      ETL_OR_STD::swap(itr3, itr4);

      CHECK_EQUAL(iop1.read(), *itr4);
      CHECK_EQUAL(iop2.read(), *itr3);
    }
  };
}
