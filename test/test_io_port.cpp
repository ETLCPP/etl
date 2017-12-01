/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl
http://www.etlcpp.com

Copyright(c) 2014 jwellbelove

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

#include "UnitTest++.h"

#include "io_port.h"

#include <stdint.h>
#include <array>
#include <algorithm>

#if defined(ETL_COMPILER_GCC)
  #pragma GCC diagnostic push
  #pragma GCC diagnostic ignored "-Wunused-variable"
#else
  #pragma warning(disable:4101) // Unused variable.
#endif

uint8_t rw  = 0x12;
uint8_t ro  = 0x34;
uint8_t wo  = 0x56;
uint8_t wos = 0x78;

namespace
{
  template <uintptr_t ADDRESS>
  struct serial_port
  {
    etl::io_port_ro<uint8_t, ADDRESS>     rxdata;
    etl::io_port_wo<uint8_t, ADDRESS + 1> txdata;
    etl::io_port_rw<uint16_t, ADDRESS + 2> control;
    etl::io_port_ro<uint16_t, ADDRESS + 4> status;
    etl::io_port_wos<uint8_t, ADDRESS + 6> control2;
  };

  struct dynamic_serial_port
  {
    dynamic_serial_port(uint8_t* base)
      : rxdata(base),
        txdata(base + 1),
        control(base + 2),
        status(base + 4),
        control2(base + 6)
    {
    }

    etl::io_port_ro<uint8_t>  rxdata;
    etl::io_port_wo<uint8_t>  txdata;
    etl::io_port_rw<uint16_t> control;
    etl::io_port_ro<uint16_t> status;
    etl::io_port_wos<uint8_t> control2;
  };

  etl::io_port_rw<uint8_t>  iop_rw;
  etl::io_port_ro<uint8_t>  iop_ro;
  etl::io_port_wo<uint8_t>  iop_wo;
  etl::io_port_wos<uint8_t> iop_wos;
}

namespace
{
  SUITE(test_io_ports)
  {
    //*************************************************************************
    TEST(test_io_port)
    {
      serial_port<0x1234> port;
    }

    //*************************************************************************
    TEST(test_dynamic_io_port)
    {
      uint8_t memory[7];
      memory[0] = 0x12;
      memory[1] = 0x00;
      memory[2] = 0x00;
      memory[3] = 0x00;
      memory[4] = 0xBC;
      memory[5] = 0x9A;
      memory[6] = 0x00;

      dynamic_serial_port port(memory);

      uint8_t rxdata = port.rxdata;
      CHECK_EQUAL(memory[0], rxdata);
      CHECK_EQUAL(memory[0], port.rxdata);

      port.txdata = 0x34;
      CHECK_EQUAL(0x34, memory[1]);

      port.control = 0x5678; // Little endian.
      CHECK_EQUAL(0x5678, memory[2] | (memory[3] << 8));

      uint16_t status = port.status;
      CHECK_EQUAL(0x9ABC, status);
      CHECK_EQUAL(0x9ABC, port.status);

      port.control2 = 0xDE;
      CHECK_EQUAL(0xDE, memory[6]);

      int control2 = port.control2;
      CHECK_EQUAL(0xDE, control2);
      CHECK_EQUAL(0xDE, port.control2);

      port.control2.set_address((void*)0x1000);
      volatile uint8_t* address = port.control2.get_address();
      CHECK_EQUAL(reinterpret_cast<volatile uint8_t*>(0x1000), address);
    }

    //*************************************************************************
    TEST(test_dynamic_io_port_iterators)
    {
      uint8_t memory_rw  = 0x12;
      uint8_t memory_ro  = 0x34;
      uint8_t memory_wo  = 0x56;
      uint8_t memory_wos = 0x78;

      iop_rw.set_address(&memory_rw);
      iop_ro.set_address(&memory_ro);
      iop_wo.set_address(&memory_wo);
      iop_wos.set_address(&memory_wos);

      std::array<uint8_t, 10> compare;
      std::array<uint8_t, 10> result;

      // Read from RW IOP.
      std::copy_n(iop_rw, result.size(), result.begin());
      compare.fill(0x12);

      for (size_t i = 0; i < compare.size(); ++i)
      {
        CHECK_EQUAL(compare[i], result[i]);
      }

      // Write to RW IOP.
      compare.fill(0x34);
      std::copy_n(compare.begin(), compare.size(), iop_rw);

      CHECK_EQUAL(compare[0], iop_rw);

      // Read from RO IOP.
      std::copy_n(iop_ro, result.size(), result.begin());
      compare.fill(0x34);

      for (size_t i = 0; i < compare.size(); ++i)
      {
        CHECK_EQUAL(compare[i], result[i]);
      }

      // Write to WO IOP.
      compare.fill(0x56);
      std::copy_n(compare.begin(), compare.size(), iop_wo);

      CHECK_EQUAL(compare[0], memory_wo);

      // Read from WOS IOP.
      iop_wos = 0x78;

      std::copy_n(iop_wos, result.size(), result.begin());
      compare.fill(0x78);

      for (size_t i = 0; i < compare.size(); ++i)
      {
        CHECK_EQUAL(compare[i], result[i]);
      }

      // Write to WOS IOP.
      compare.fill(0x90);
      std::copy_n(compare.begin(), compare.size(), iop_wos.get_iterator());

      CHECK_EQUAL(compare[0], iop_wos);
    }
    
    TEST(compile)
    {
    //  etl::io_port_rw<uint8_t,  uintptr_t(1)> p_rw;
    //  etl::io_port_ro<uint8_t,  uintptr_t(2)> p_ro;
    //  etl::io_port_wo<uint8_t,  uintptr_t(3)> p_wo;
    //  etl::io_port_wos<uint8_t, uintptr_t(4)> p_wos;

    //  uint8_t c;

    //  *p_rw = 1;
    //  c = *p_rw;

    //  *p_ro = 1;
    //  c = *p_ro;

    //  *p_wo = 1;
    //  c = *p_wo;

    //  *p_wos = 1;
    //  c = *p_wos;
    }
  };
}

#if defined(ETL_COMPILER_GCC)
  #pragma GCC diagnostic pop
#endif
