/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl
https://www.etlcpp.com

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

#include "unit_test_framework.h"

#include <iterator>
#include <string>
#include <vector>
#include <stdint.h>

#include "etl/crc8_ccitt.h"
#include "etl/crc8_rohc.h"
#include "etl/crc16.h"
#include "etl/crc16_ccitt.h"
#include "etl/crc16_aug_ccitt.h"
#include "etl/crc16_kermit.h"
#include "etl/crc16_modbus.h"
#include "etl/crc16_usb.h"
#include "etl/crc16_xmodem.h"
#include "etl/crc16_genibus.h"
#include "etl/crc16_x25.h"
#include "etl/crc32.h"
#include "etl/crc32_c.h"
#include "etl/crc32_bzip2.h"
#include "etl/crc32_mpeg2.h"
#include "etl/crc32_posix.h"
#include "etl/crc64_ecma.h"

//*****************************************************************************
// The results for these tests were created from https://crccalc.com/
//*****************************************************************************

namespace
{
  SUITE(test_crc)
  {
    //*************************************************************************
    TEST(test_crc8_ccitt_constructor)
    {
      std::string data("123456789");

      uint8_t crc = etl::crc8_ccitt(data.begin(), data.end());

      CHECK_EQUAL(0xF4, int(crc));
    }

    //*************************************************************************
    TEST(test_crc8_ccitt_add_values)
    {
      std::string data("123456789");

      etl::crc8_ccitt crc_calculator;

      for (size_t i = 0UL; i < data.size(); ++i)
      {
        crc_calculator.add(data[i]);
      }

      uint8_t crc = crc_calculator;

      CHECK_EQUAL(0xF4, int(crc));
    }

    //*************************************************************************
    TEST(test_crc8_ccitt_add_range)
    {
      std::string data("123456789");

      etl::crc8_ccitt crc_calculator;

      crc_calculator.add(data.begin(), data.end());

      uint8_t crc = crc_calculator.value();

      CHECK_EQUAL(0xF4, int(crc));
    }

    //*************************************************************************
    TEST(test_crc8_ccitt_add_range_via_iterator)
    {
      std::string data("123456789");

      etl::crc8_ccitt crc_calculator;

      std::copy(data.begin(), data.end(), crc_calculator.input());

      uint8_t crc = crc_calculator.value();

      CHECK_EQUAL(0xF4, int(crc));
    }

    //*************************************************************************
    TEST(test_crc8_ccitt_add_range_endian)
    {
      std::vector<uint8_t>  data1 = { 0x01U, 0x02U, 0x03U, 0x04U, 0x05U, 0x06U, 0x07U, 0x08U };
      std::vector<uint32_t> data2 = { 0x04030201UL, 0x08070605UL };
      std::vector<uint8_t>  data3 = { 0x08U, 0x07U, 0x06U, 0x05U, 0x04U, 0x03U, 0x02U, 0x01U };

      uint8_t crc1 = etl::crc32(data1.begin(), data1.end());
      uint8_t crc2 = etl::crc32((uint8_t*)&data2[0], (uint8_t*)(&data2[0] + data2.size()));
      CHECK_EQUAL(int(crc1), int(crc2));

      uint8_t crc3 = etl::crc32(data3.rbegin(), data3.rend());
      CHECK_EQUAL(int(crc1), int(crc3));
    }

    //*************************************************************************
    TEST(test_crc8_rohc_constructor)
    {
      std::string data("123456789");

      uint8_t crc = etl::crc8_rohc(data.begin(), data.end());

      CHECK_EQUAL(0xD0, int(crc));
    }

    //*************************************************************************
    TEST(test_crc8_rohc_add_values)
    {
      std::string data("123456789");

      etl::crc8_rohc crc_calculator;

      for (size_t i = 0UL; i < data.size(); ++i)
      {
        crc_calculator.add(data[i]);
      }

      uint8_t crc = crc_calculator;

      CHECK_EQUAL(0xD0, int(crc));
    }

    //*************************************************************************
    TEST(test_crc8_rohc_add_range)
    {
      std::string data("123456789");

      etl::crc8_rohc crc_calculator;

      crc_calculator.add(data.begin(), data.end());

      uint8_t crc = crc_calculator.value();

      CHECK_EQUAL(0xD0, int(crc));
    }

    //*************************************************************************
    TEST(test_crc8_rohc_add_range_via_iterator)
    {
      std::string data("123456789");

      etl::crc8_rohc crc_calculator;

      std::copy(data.begin(), data.end(), crc_calculator.input());

      uint8_t crc = crc_calculator.value();

      CHECK_EQUAL(0xD0, int(crc));
    }

    //*************************************************************************
    TEST(test_crc8_rohc_add_range_endian)
    {
      std::vector<uint8_t>  data1 = { 0x01U, 0x02U, 0x03U, 0x04U, 0x05U, 0x06U, 0x07U, 0x08U };
      std::vector<uint32_t> data2 = { 0x04030201UL, 0x08070605UL };
      std::vector<uint8_t>  data3 = { 0x08U, 0x07U, 0x06U, 0x05U, 0x04U, 0x03U, 0x02U, 0x01U };

      uint8_t crc1 = etl::crc32(data1.begin(), data1.end());
      uint8_t crc2 = etl::crc32((uint8_t*)&data2[0], (uint8_t*)(&data2[0] + data2.size()));
      CHECK_EQUAL(int(crc1), int(crc2));

      uint8_t crc3 = etl::crc32(data3.rbegin(), data3.rend());
      CHECK_EQUAL(int(crc1), int(crc3));
    }

    //*************************************************************************
    TEST(test_crc16)
    {
      std::string data("123456789");

      uint16_t crc = etl::crc16(data.begin(), data.end());

      CHECK_EQUAL(0xBB3DU, crc);
    }

    //*************************************************************************
    TEST(test_crc16_add_values)
    {
      std::string data("123456789");

      etl::crc16 crc_calculator;

      for (size_t i = 0UL; i < data.size(); ++i)
      {
        crc_calculator.add(data[i]);
      }

      uint16_t crc = crc_calculator;

      CHECK_EQUAL(0xBB3DU, crc);
    }

    //*************************************************************************
    TEST(test_crc16_add_range)
    {
      std::string data("123456789");

      etl::crc16 crc_calculator;

      crc_calculator.add(data.begin(), data.end());

      uint16_t crc = crc_calculator.value();

      CHECK_EQUAL(0xBB3DU, crc);
    }

    //*************************************************************************
    TEST(test_crc16_add_range_via_iterator)
    {
      std::string data("123456789");

      etl::crc16 crc_calculator;

      std::copy(data.begin(), data.end(), crc_calculator.input());

      uint16_t crc = crc_calculator.value();

      CHECK_EQUAL(0xBB3DU, crc);
    }

    //*************************************************************************
    TEST(test_crc16_add_range_endian)
    {
      std::vector<uint8_t>  data1 = { 0x01U, 0x02U, 0x03U, 0x04U, 0x05U, 0x06U, 0x07U, 0x08U };
      std::vector<uint32_t> data2 = { 0x04030201UL, 0x08070605UL };
      std::vector<uint8_t>  data3 = { 0x08U, 0x07U, 0x06U, 0x05U, 0x04U, 0x03U, 0x02U, 0x01U };

      uint16_t crc1 = etl::crc16(data1.begin(), data1.end());
      uint16_t crc2 = etl::crc16((uint8_t*)&data2[0], (uint8_t*)(&data2[0] + data2.size()));
      CHECK_EQUAL(crc1, crc2);

      uint16_t crc3 = etl::crc16(data3.rbegin(), data3.rend());
      CHECK_EQUAL(crc1, crc3);
    }

    //*************************************************************************
    TEST(test_crc16_ccitt)
    {
      std::string data("123456789");

      uint16_t crc = etl::crc16_ccitt(data.begin(), data.end());

      CHECK_EQUAL(0x29B1U, crc);
    }

    //*************************************************************************
    TEST(test_crc16_ccitt_add_values)
    {
      std::string data("123456789");

      etl::crc16_ccitt crc_calculator;

      for (size_t i = 0UL; i < data.size(); ++i)
      {
        crc_calculator.add(data[i]);
      }

      uint16_t crc = crc_calculator;

      CHECK_EQUAL(0x29B1U, crc);
    }

    //*************************************************************************
    TEST(test_crc16_ccitt_add_range)
    {
      std::string data("123456789");

      etl::crc16_ccitt crc_calculator;

      crc_calculator.add(data.begin(), data.end());

      uint16_t crc = crc_calculator.value();

      CHECK_EQUAL(0x29B1U, crc);
    }

    //*************************************************************************
    TEST(test_crc16_ccitt_add_range_via_iterator)
    {
      std::string data("123456789");

      etl::crc16_ccitt crc_calculator;

      std::copy(data.begin(), data.end(), crc_calculator.input());

      uint16_t crc = crc_calculator.value();

      CHECK_EQUAL(0x29B1U, crc);
    }

    //*************************************************************************
    TEST(test_crc16_ccitt_add_range_endian)
    {
      std::vector<uint8_t>  data1 = { 0x01U, 0x02U, 0x03U, 0x04U, 0x05U, 0x06U, 0x07U, 0x08U };
      std::vector<uint32_t> data2 = { 0x04030201UL, 0x08070605UL };
      std::vector<uint8_t>  data3 = { 0x08U, 0x07U, 0x06U, 0x05U, 0x04U, 0x03U, 0x02U, 0x01U };

      uint16_t crc1 = etl::crc16_ccitt(data1.begin(), data1.end());
      uint16_t crc2 = etl::crc16_ccitt((uint8_t*)&data2[0], (uint8_t*)(&data2[0] + data2.size()));
      CHECK_EQUAL(crc1, crc2);

      uint16_t crc3 = etl::crc16_ccitt(data3.rbegin(), data3.rend());
      CHECK_EQUAL(crc1, crc3);
    }

    //*************************************************************************
    TEST(test_crc16_kermit)
    {
      std::string data("123456789");

      uint16_t crc = etl::crc16_kermit(data.begin(), data.end());

      CHECK_EQUAL(0x2189U, crc);
    }

    //*************************************************************************
    TEST(test_crc16_kermit_add_values)
    {
      std::string data("123456789");

      etl::crc16_kermit crc_calculator;

      for (size_t i = 0UL; i < data.size(); ++i)
      {
        crc_calculator.add(data[i]);
      }

      uint16_t crc = crc_calculator;

      CHECK_EQUAL(0x2189U, crc);
    }

    //*************************************************************************
    TEST(test_crc16_kermit_add_range)
    {
      std::string data("123456789");

      etl::crc16_kermit crc_calculator;

      crc_calculator.add(data.begin(), data.end());

      uint16_t crc = crc_calculator.value();

      CHECK_EQUAL(0x2189U, crc);
    }

    //*************************************************************************
    TEST(test_crc16_kermit_add_range_via_iterator)
    {
      std::string data("123456789");

      etl::crc16_kermit crc_calculator;

      std::copy(data.begin(), data.end(), crc_calculator.input());

      uint16_t crc = crc_calculator.value();

      CHECK_EQUAL(0x2189U, crc);
    }

    //*************************************************************************
    TEST(test_crc16_kermit_add_range_endian)
    {
      std::vector<uint8_t>  data1 = { 0x01U, 0x02U, 0x03U, 0x04U, 0x05U, 0x06U, 0x07U, 0x08U };
      std::vector<uint32_t> data2 = { 0x04030201UL, 0x08070605UL };
      std::vector<uint8_t>  data3 = { 0x08U, 0x07U, 0x06U, 0x05U, 0x04U, 0x03U, 0x02U, 0x01U };

      uint16_t crc1 = etl::crc16_kermit(data1.begin(), data1.end());
      uint16_t crc2 = etl::crc16_kermit((uint8_t*)&data2[0], (uint8_t*)(&data2[0] + data2.size()));
      CHECK_EQUAL(crc1, crc2);

      uint16_t crc3 = etl::crc16_kermit(data3.rbegin(), data3.rend());
      CHECK_EQUAL(crc1, crc3);
    }

    //*************************************************************************
    TEST(test_crc16_modbus)
    {
      std::string data("123456789");

      uint16_t crc = etl::crc16_modbus(data.begin(), data.end());

      CHECK_EQUAL(0x4B37U, crc);
    }

    //*************************************************************************
    TEST(test_crc16_modbus_add_values)
    {
      std::string data("123456789");

      etl::crc16_modbus crc_calculator;

      for (size_t i = 0UL; i < data.size(); ++i)
      {
        crc_calculator.add(data[i]);
      }

      uint16_t crc = crc_calculator;

      CHECK_EQUAL(0x4B37U, crc);
    }

    //*************************************************************************
    TEST(test_crc16_modbus_add_range)
    {
      std::string data("123456789");

      etl::crc16_modbus crc_calculator;

      crc_calculator.add(data.begin(), data.end());

      uint16_t crc = crc_calculator.value();

      CHECK_EQUAL(0x4B37U, crc);
    }

    //*************************************************************************
    TEST(test_crc16_modbus_add_range_via_iterator)
    {
      std::string data("123456789");

      etl::crc16_modbus crc_calculator;

      std::copy(data.begin(), data.end(), crc_calculator.input());

      uint16_t crc = crc_calculator.value();

      CHECK_EQUAL(0x4B37U, crc);
    }

    //*************************************************************************
    TEST(test_crc16_modbus_add_range_endian)
    {
      std::vector<uint8_t>  data1 = { 0x01U, 0x02U, 0x03U, 0x04U, 0x05U, 0x06U, 0x07U, 0x08U };
      std::vector<uint32_t> data2 = { 0x04030201UL, 0x08070605UL };
      std::vector<uint8_t>  data3 = { 0x08U, 0x07U, 0x06U, 0x05U, 0x04U, 0x03U, 0x02U, 0x01U };

      uint16_t crc1 = etl::crc16_modbus(data1.begin(), data1.end());
      uint16_t crc2 = etl::crc16_modbus((uint8_t*)&data2[0], (uint8_t*)(&data2[0] + data2.size()));
      CHECK_EQUAL(crc1, crc2);

      uint16_t crc3 = etl::crc16_modbus(data3.rbegin(), data3.rend());
      CHECK_EQUAL(crc1, crc3);
    }

    //*************************************************************************
    TEST(test_crc16_usb)
    {
      std::string data("123456789");

      uint16_t crc = etl::crc16_usb(data.begin(), data.end());

      CHECK_EQUAL(0xB4C8U, crc);
    }

    //*************************************************************************
    TEST(test_crc16_usb_add_values)
    {
      std::string data("123456789");

      etl::crc16_usb crc_calculator;

      for (size_t i = 0UL; i < data.size(); ++i)
      {
        crc_calculator.add(data[i]);
      }

      uint16_t crc = crc_calculator;

      CHECK_EQUAL(0xB4C8U, crc);
    }

    //*************************************************************************
    TEST(test_crc16_usb_add_range)
    {
      std::string data("123456789");

      etl::crc16_usb crc_calculator;

      crc_calculator.add(data.begin(), data.end());

      uint16_t crc = crc_calculator.value();

      CHECK_EQUAL(0xB4C8U, crc);
    }

    //*************************************************************************
    TEST(test_crc16_usb_add_range_via_iterator)
    {
      std::string data("123456789");

      etl::crc16_usb crc_calculator;

      std::copy(data.begin(), data.end(), crc_calculator.input());

      uint16_t crc = crc_calculator.value();

      CHECK_EQUAL(0xB4C8U, crc);
    }

    //*************************************************************************
    TEST(test_crc16_usb_add_range_endian)
    {
      std::vector<uint8_t>  data1 = { 0x01U, 0x02U, 0x03U, 0x04U, 0x05U, 0x06U, 0x07U, 0x08U };
      std::vector<uint32_t> data2 = { 0x04030201UL, 0x08070605UL };
      std::vector<uint8_t>  data3 = { 0x08U, 0x07U, 0x06U, 0x05U, 0x04U, 0x03U, 0x02U, 0x01U };

      uint16_t crc1 = etl::crc16_usb(data1.begin(), data1.end());
      uint16_t crc2 = etl::crc16_usb((uint8_t*)&data2[0], (uint8_t*)(&data2[0] + data2.size()));
      CHECK_EQUAL(crc1, crc2);

      uint16_t crc3 = etl::crc16_usb(data3.rbegin(), data3.rend());
      CHECK_EQUAL(crc1, crc3);
    }

    //*************************************************************************
    TEST(test_crc16_xmodem)
    {
      std::string data("123456789");

      uint16_t crc = etl::crc16_xmodem(data.begin(), data.end());

      CHECK_EQUAL(0x31C3U, crc);
    }

    //*************************************************************************
    TEST(test_crc16_xmodem_add_values)
    {
      std::string data("123456789");

      etl::crc16_xmodem crc_calculator;

      for (size_t i = 0UL; i < data.size(); ++i)
      {
        crc_calculator.add(data[i]);
      }

      uint16_t crc = crc_calculator;

      CHECK_EQUAL(0x31C3U, crc);
    }

    //*************************************************************************
    TEST(test_crc16_xmodem_add_range)
    {
      std::string data("123456789");

      etl::crc16_xmodem crc_calculator;

      crc_calculator.add(data.begin(), data.end());

      uint16_t crc = crc_calculator.value();

      CHECK_EQUAL(0x31C3U, crc);
    }

    //*************************************************************************
    TEST(test_crc16_xmodem_add_range_via_iterator)
    {
      std::string data("123456789");

      etl::crc16_xmodem crc_calculator;

      std::copy(data.begin(), data.end(), crc_calculator.input());

      uint16_t crc = crc_calculator.value();

      CHECK_EQUAL(0x31C3U, crc);
    }

    //*************************************************************************
    TEST(test_crc16_xmodem_add_range_endian)
    {
      std::vector<uint8_t>  data1 = { 0x01U, 0x02U, 0x03U, 0x04U, 0x05U, 0x06U, 0x07U, 0x08U };
      std::vector<uint32_t> data2 = { 0x04030201UL, 0x08070605UL };
      std::vector<uint8_t>  data3 = { 0x08U, 0x07U, 0x06U, 0x05U, 0x04U, 0x03U, 0x02U, 0x01U };

      uint16_t crc1 = etl::crc16_xmodem(data1.begin(), data1.end());
      uint16_t crc2 = etl::crc16_xmodem((uint8_t*)&data2[0], (uint8_t*)(&data2[0] + data2.size()));
      CHECK_EQUAL(crc1, crc2);

      uint16_t crc3 = etl::crc16_xmodem(data3.rbegin(), data3.rend());
      CHECK_EQUAL(crc1, crc3);
    }

    //*************************************************************************
    TEST(test_crc16_aug_ccitt)
    {
      std::string data("123456789");

      uint16_t crc = etl::crc16_aug_ccitt(data.begin(), data.end());

      CHECK_EQUAL(0xE5CCU, crc);
    }

    //*************************************************************************
    TEST(test_crc16_aug_ccitt_add_values)
    {
      std::string data("123456789");

      etl::crc16_aug_ccitt crc_calculator;

      for (size_t i = 0UL; i < data.size(); ++i)
      {
        crc_calculator.add(data[i]);
      }

      uint16_t crc = crc_calculator;

      CHECK_EQUAL(0xE5CCU, crc);
    }

    //*************************************************************************
    TEST(test_crc16_aug_ccitt_add_range)
    {
      std::string data("123456789");

      etl::crc16_aug_ccitt crc_calculator;

      crc_calculator.add(data.begin(), data.end());

      uint16_t crc = crc_calculator.value();

      CHECK_EQUAL(0xE5CCU, crc);
    }

    //*************************************************************************
    TEST(test_crc16_aug_ccitt_add_range_via_iterator)
    {
      std::string data("123456789");

      etl::crc16_aug_ccitt crc_calculator;

      std::copy(data.begin(), data.end(), crc_calculator.input());

      uint16_t crc = crc_calculator.value();

      CHECK_EQUAL(0xE5CCU, crc);
    }

    //*************************************************************************
    TEST(test_crc16_aug_ccitt_add_range_endian)
    {
      std::vector<uint8_t>  data1 = { 0x01U, 0x02U, 0x03U, 0x04U, 0x05U, 0x06U, 0x07U, 0x08U };
      std::vector<uint32_t> data2 = { 0x04030201UL, 0x08070605UL };
      std::vector<uint8_t>  data3 = { 0x08U, 0x07U, 0x06U, 0x05U, 0x04U, 0x03U, 0x02U, 0x01U };

      uint16_t crc1 = etl::crc16_aug_ccitt(data1.begin(), data1.end());
      uint16_t crc2 = etl::crc16_aug_ccitt((uint8_t*)&data2[0], (uint8_t*)(&data2[0] + data2.size()));
      CHECK_EQUAL(crc1, crc2);

      uint16_t crc3 = etl::crc16_aug_ccitt(data3.rbegin(), data3.rend());
      CHECK_EQUAL(crc1, crc3);
    }

    //*************************************************************************
    TEST(test_crc16_genibus)
    {
      std::string data("123456789");

      uint16_t crc = etl::crc16_genibus(data.begin(), data.end());

      CHECK_EQUAL(0xD64EU, crc);
    }

    //*************************************************************************
    TEST(test_crc16_genibus_add_values)
    {
      std::string data("123456789");

      etl::crc16_genibus crc_calculator;

      for (size_t i = 0UL; i < data.size(); ++i)
      {
        crc_calculator.add(data[i]);
      }

      uint16_t crc = crc_calculator;

      CHECK_EQUAL(0xD64EU, crc);
    }

    //*************************************************************************
    TEST(test_crc16_genibus_add_range)
    {
      std::string data("123456789");

      etl::crc16_genibus crc_calculator;

      crc_calculator.add(data.begin(), data.end());

      uint16_t crc = crc_calculator.value();

      CHECK_EQUAL(0xD64EU, crc);
    }

    //*************************************************************************
    TEST(test_crc16_genibus_add_range_via_iterator)
    {
      std::string data("123456789");

      etl::crc16_genibus crc_calculator;

      std::copy(data.begin(), data.end(), crc_calculator.input());

      uint16_t crc = crc_calculator.value();

      CHECK_EQUAL(0xD64EU, crc);
    }

    //*************************************************************************
    TEST(test_crc16_genibus_add_range_endian)
    {
      std::vector<uint8_t>  data1 = { 0x01U, 0x02U, 0x03U, 0x04U, 0x05U, 0x06U, 0x07U, 0x08U };
      std::vector<uint32_t> data2 = { 0x04030201UL, 0x08070605UL };
      std::vector<uint8_t>  data3 = { 0x08U, 0x07U, 0x06U, 0x05U, 0x04U, 0x03U, 0x02U, 0x01U };

      uint16_t crc1 = etl::crc16_genibus(data1.begin(), data1.end());
      uint16_t crc2 = etl::crc16_genibus((uint8_t*)&data2[0], (uint8_t*)(&data2[0] + data2.size()));
      CHECK_EQUAL(crc1, crc2);

      uint16_t crc3 = etl::crc16_genibus(data3.rbegin(), data3.rend());
      CHECK_EQUAL(crc1, crc3);
    }

    //*************************************************************************
    TEST(test_crc16_x25)
    {
      std::string data("123456789");

      uint16_t crc = etl::crc16_x25(data.begin(), data.end());

      CHECK_EQUAL(0x906EU, crc);
    }

    //*************************************************************************
    TEST(test_crc16_x25_add_values)
    {
      std::string data("123456789");

      etl::crc16_x25 crc_calculator;

      for (size_t i = 0UL; i < data.size(); ++i)
      {
        crc_calculator.add(data[i]);
      }

      uint16_t crc = crc_calculator;

      CHECK_EQUAL(0x906EU, crc);
    }

    //*************************************************************************
    TEST(test_crc16_x25_add_range)
    {
      std::string data("123456789");

      etl::crc16_x25 crc_calculator;

      crc_calculator.add(data.begin(), data.end());

      uint16_t crc = crc_calculator.value();

      CHECK_EQUAL(0x906EU, crc);
    }

    //*************************************************************************
    TEST(test_crc16_x25_add_range_via_iterator)
    {
      std::string data("123456789");

      etl::crc16_x25 crc_calculator;

      std::copy(data.begin(), data.end(), crc_calculator.input());

      uint16_t crc = crc_calculator.value();

      CHECK_EQUAL(0x906EU, crc);
    }

    //*************************************************************************
    TEST(test_crc16_x25_add_range_endian)
    {
      std::vector<uint8_t>  data1 = { 0x01U, 0x02U, 0x03U, 0x04U, 0x05U, 0x06U, 0x07U, 0x08U };
      std::vector<uint32_t> data2 = { 0x04030201UL, 0x08070605UL };
      std::vector<uint8_t>  data3 = { 0x08U, 0x07U, 0x06U, 0x05U, 0x04U, 0x03U, 0x02U, 0x01U };

      uint16_t crc1 = etl::crc16_x25(data1.begin(), data1.end());
      uint16_t crc2 = etl::crc16_x25((uint8_t*)&data2[0], (uint8_t*)(&data2[0] + data2.size()));
      CHECK_EQUAL(crc1, crc2);

      uint16_t crc3 = etl::crc16_x25(data3.rbegin(), data3.rend());
      CHECK_EQUAL(crc1, crc3);
    }

    //*************************************************************************
    TEST(test_crc32)
    {
      std::string data("123456789");

      uint32_t crc = etl::crc32(data.begin(), data.end());

      CHECK_EQUAL(0xCBF43926UL, crc);
    }

    //*************************************************************************
    TEST(test_crc32_add_values)
    {
      std::string data("123456789");

      etl::crc32 crc_calculator;

      for (size_t i = 0UL; i < data.size(); ++i)
      {
        crc_calculator.add(data[i]);
      }

      uint32_t crc = crc_calculator;


      CHECK_EQUAL(0xCBF43926UL, crc);
    }

    //*************************************************************************
    TEST(test_crc32_add_range)
    {
      std::string data("123456789");

      etl::crc32 crc_calculator;

      crc_calculator.add(data.begin(), data.end());

      uint32_t crc = crc_calculator.value();

      CHECK_EQUAL(0xCBF43926UL, crc);
    }

    //*************************************************************************
    TEST(test_crc32_add_range_via_iterator)
    {
      std::string data("123456789");

      etl::crc32 crc_calculator;

      std::copy(data.begin(), data.end(), crc_calculator.input());

      uint32_t crc = crc_calculator.value();

      CHECK_EQUAL(0xCBF43926UL, crc);
    }

    //*************************************************************************
    TEST(test_crc32_add_range_endian)
    {
      std::vector<uint8_t>  data1 = { 0x01U, 0x02U, 0x03U, 0x04U, 0x05U, 0x06U, 0x07U, 0x08U };
      std::vector<uint32_t> data2 = { 0x04030201UL, 0x08070605UL };
      std::vector<uint8_t>  data3 = { 0x08U, 0x07U, 0x06U, 0x05U, 0x04U, 0x03U, 0x02U, 0x01U };

      uint32_t crc1 = etl::crc32(data1.begin(), data1.end());
      uint32_t crc2 = etl::crc32((uint8_t*)&data2[0], (uint8_t*)(&data2[0] + data2.size()));
      CHECK_EQUAL(crc1, crc2);

      uint32_t crc3 = etl::crc32(data3.rbegin(), data3.rend());
      CHECK_EQUAL(crc1, crc3);
    }

    //*************************************************************************
    TEST(test_crc32_c)
    {
      std::string data("123456789");

      uint32_t crc = etl::crc32_c(data.begin(), data.end());

      CHECK_EQUAL(0xE3069283UL, crc);
    }

    //*************************************************************************
    TEST(test_crc32_c_add_values)
    {
      std::string data("123456789");

      etl::crc32_c crc_calculator;

      for (size_t i = 0UL; i < data.size(); ++i)
      {
        crc_calculator.add(data[i]);
      }

      uint32_t crc = crc_calculator;


      CHECK_EQUAL(0xE3069283UL, crc);
    }

    //*************************************************************************
    TEST(test_crc32_c_add_range)
    {
      std::string data("123456789");

      etl::crc32_c crc_calculator;

      crc_calculator.add(data.begin(), data.end());

      uint32_t crc = crc_calculator.value();

      CHECK_EQUAL(0xE3069283UL, crc);
    }

    //*************************************************************************
    TEST(test_crc32_c_add_range_via_iterator)
    {
      std::string data("123456789");

      etl::crc32_c crc_calculator;

      std::copy(data.begin(), data.end(), crc_calculator.input());

      uint32_t crc = crc_calculator.value();

      CHECK_EQUAL(0xE3069283UL, crc);
    }

    //*************************************************************************
    TEST(test_crc32_c_add_range_endian)
    {
      std::vector<uint8_t>  data1 = { 0x01U, 0x02U, 0x03U, 0x04U, 0x05U, 0x06U, 0x07U, 0x08U };
      std::vector<uint32_t> data2 = { 0x04030201UL, 0x08070605UL };
      std::vector<uint8_t>  data3 = { 0x08U, 0x07U, 0x06U, 0x05U, 0x04U, 0x03U, 0x02U, 0x01U };

      uint32_t crc1 = etl::crc32_c(data1.begin(), data1.end());
      uint32_t crc2 = etl::crc32_c((uint8_t*)&data2[0], (uint8_t*)(&data2[0] + data2.size()));
      CHECK_EQUAL(crc1, crc2);

      uint32_t crc3 = etl::crc32_c(data3.rbegin(), data3.rend());
      CHECK_EQUAL(crc1, crc3);
    }

    //*************************************************************************
    TEST(test_crc32_bzip2)
    {
      std::string data("123456789");

      uint32_t crc = etl::crc32_bzip2(data.begin(), data.end());

      CHECK_EQUAL(0xFC891918UL, crc);
    }

    //*************************************************************************
    TEST(test_crc32_bzip2_add_values)
    {
      std::string data("123456789");

      etl::crc32_bzip2 crc_calculator;

      for (size_t i = 0UL; i < data.size(); ++i)
      {
        crc_calculator.add(data[i]);
      }

      uint32_t crc = crc_calculator;


      CHECK_EQUAL(0xFC891918UL, crc);
    }

    //*************************************************************************
    TEST(test_crc32_bzip2_add_range)
    {
      std::string data("123456789");

      etl::crc32_bzip2 crc_calculator;

      crc_calculator.add(data.begin(), data.end());

      uint32_t crc = crc_calculator.value();

      CHECK_EQUAL(0xFC891918UL, crc);
    }

    //*************************************************************************
    TEST(test_crc32_bzip2_add_range_via_iterator)
    {
      std::string data("123456789");

      etl::crc32_bzip2 crc_calculator;

      std::copy(data.begin(), data.end(), crc_calculator.input());

      uint32_t crc = crc_calculator.value();

      CHECK_EQUAL(0xFC891918UL, crc);
    }

    //*************************************************************************
    TEST(test_crc32_bzip2_add_range_endian)
    {
      std::vector<uint8_t>  data1 = { 0x01U, 0x02U, 0x03U, 0x04U, 0x05U, 0x06U, 0x07U, 0x08U };
      std::vector<uint32_t> data2 = { 0x04030201UL, 0x08070605UL };
      std::vector<uint8_t>  data3 = { 0x08U, 0x07U, 0x06U, 0x05U, 0x04U, 0x03U, 0x02U, 0x01U };

      uint32_t crc1 = etl::crc32_bzip2(data1.begin(), data1.end());
      uint32_t crc2 = etl::crc32_bzip2((uint8_t*)&data2[0], (uint8_t*)(&data2[0] + data2.size()));
      CHECK_EQUAL(crc1, crc2);

      uint32_t crc3 = etl::crc32_bzip2(data3.rbegin(), data3.rend());
      CHECK_EQUAL(crc1, crc3);
    }

    //*************************************************************************
    TEST(test_crc32_posix)
    {
      std::string data("123456789");

      uint32_t crc = etl::crc32_posix(data.begin(), data.end());

      CHECK_EQUAL(0x765E7680UL, crc);
    }

    //*************************************************************************
    TEST(test_crc32_posix_add_values)
    {
      std::string data("123456789");

      etl::crc32_posix crc_calculator;

      for (size_t i = 0UL; i < data.size(); ++i)
      {
        crc_calculator.add(data[i]);
      }

      uint32_t crc = crc_calculator;


      CHECK_EQUAL(0x765E7680UL, crc);
    }

    //*************************************************************************
    TEST(test_crc32_posix_add_range)
    {
      std::string data("123456789");

      etl::crc32_posix crc_calculator;

      crc_calculator.add(data.begin(), data.end());

      uint32_t crc = crc_calculator.value();

      CHECK_EQUAL(0x765E7680UL, crc);
    }

    //*************************************************************************
    TEST(test_crc32_posix_add_range_via_iterator)
    {
      std::string data("123456789");

      etl::crc32_posix crc_calculator;

      std::copy(data.begin(), data.end(), crc_calculator.input());

      uint32_t crc = crc_calculator.value();

      CHECK_EQUAL(0x765E7680UL, crc);
    }

    //*************************************************************************
    TEST(test_crc32_posix_add_range_endian)
    {
      std::vector<uint8_t>  data1 = { 0x01U, 0x02U, 0x03U, 0x04U, 0x05U, 0x06U, 0x07U, 0x08U };
      std::vector<uint32_t> data2 = { 0x04030201UL, 0x08070605UL };
      std::vector<uint8_t>  data3 = { 0x08U, 0x07U, 0x06U, 0x05U, 0x04U, 0x03U, 0x02U, 0x01U };

      uint32_t crc1 = etl::crc32_posix(data1.begin(), data1.end());
      uint32_t crc2 = etl::crc32_posix((uint8_t*)&data2[0], (uint8_t*)(&data2[0] + data2.size()));
      CHECK_EQUAL(crc1, crc2);

      uint32_t crc3 = etl::crc32_posix(data3.rbegin(), data3.rend());
      CHECK_EQUAL(crc1, crc3);
    }

    //*************************************************************************
    TEST(test_crc32_mpeg2)
    {
      std::string data("123456789");

      uint32_t crc = etl::crc32_mpeg2(data.begin(), data.end());

      CHECK_EQUAL(0x0376E6E7UL, crc);
    }

    //*************************************************************************
    TEST(test_crc32_mpeg2_add_values)
    {
      std::string data("123456789");

      etl::crc32_mpeg2 crc_calculator;

      for (size_t i = 0UL; i < data.size(); ++i)
      {
        crc_calculator.add(data[i]);
      }

      uint32_t crc = crc_calculator;


      CHECK_EQUAL(0x0376E6E7UL, crc);
    }

    //*************************************************************************
    TEST(test_crc32_mpeg2_add_range)
    {
      std::string data("123456789");

      etl::crc32_mpeg2 crc_calculator;

      crc_calculator.add(data.begin(), data.end());

      uint32_t crc = crc_calculator.value();

      CHECK_EQUAL(0x0376E6E7UL, crc);
    }

    //*************************************************************************
    TEST(test_crc32_mpeg2_add_range_via_iterator)
    {
      std::string data("123456789");

      etl::crc32_mpeg2 crc_calculator;

      std::copy(data.begin(), data.end(), crc_calculator.input());

      uint32_t crc = crc_calculator.value();

      CHECK_EQUAL(0x0376E6E7UL, crc);
    }

    //*************************************************************************
    TEST(test_crc32_mpeg2_add_range_endian)
    {
      std::vector<uint8_t>  data1 = { 0x01U, 0x02U, 0x03U, 0x04U, 0x05U, 0x06U, 0x07U, 0x08U };
      std::vector<uint32_t> data2 = { 0x04030201UL, 0x08070605UL };
      std::vector<uint8_t>  data3 = { 0x08U, 0x07U, 0x06U, 0x05U, 0x04U, 0x03U, 0x02U, 0x01U };

      uint32_t crc1 = etl::crc32_mpeg2(data1.begin(), data1.end());
      uint32_t crc2 = etl::crc32_mpeg2((uint8_t*)&data2[0], (uint8_t*)(&data2[0] + data2.size()));
      CHECK_EQUAL(crc1, crc2);

      uint32_t crc3 = etl::crc32_mpeg2(data3.rbegin(), data3.rend());
      CHECK_EQUAL(crc1, crc3);
    }

    //*************************************************************************
    TEST(test_crc64_ecma)
    {
      std::string data("123456789");

      uint64_t crc = etl::crc64_ecma(data.begin(), data.end());

      CHECK_EQUAL(0x6C40DF5F0B497347ULL, crc);
    }

    //*************************************************************************
    TEST(test_crc64_ecma_add_values)
    {
      std::string data("123456789");

      etl::crc64_ecma crc_calculator;

      for (size_t i = 0UL; i < data.size(); ++i)
      {
        crc_calculator.add(data[i]);
      }

      uint64_t crc = crc_calculator;

      CHECK_EQUAL(0x6C40DF5F0B497347ULL, crc);
    }

    //*************************************************************************
    TEST(test_crc64_ecma_add_range)
    {
      std::string data("123456789");

      etl::crc64_ecma crc_calculator;

      crc_calculator.add(data.begin(), data.end());

      uint64_t crc = crc_calculator.value();

      CHECK_EQUAL(0x6C40DF5F0B497347ULL, crc);
    }

    //*************************************************************************
    TEST(test_crc64_ecma_add_range_via_iterator)
    {
      std::string data("123456789");

      etl::crc64_ecma crc_calculator;

      std::copy(data.begin(), data.end(), crc_calculator.input());

      uint64_t crc = crc_calculator.value();

      CHECK_EQUAL(0x6C40DF5F0B497347ULL, crc);
    }

    //*************************************************************************
    TEST(test_crc64_ecma_add_range_endian)
    {
      std::vector<uint8_t>  data1 = { 0x01U, 0x02U, 0x03U, 0x04U, 0x05U, 0x06U, 0x07U, 0x08U };
      std::vector<uint32_t> data2 = { 0x04030201UL, 0x08070605UL };
     std::vector<uint8_t>  data3 = { 0x08U, 0x07U, 0x06U, 0x05U, 0x04U, 0x03U, 0x02U, 0x01U };

      uint64_t crc1 = etl::crc64_ecma(data1.begin(), data1.end());
      uint64_t crc2 = etl::crc64_ecma((uint8_t*)&data2[0], (uint8_t*)(&data2[0] + data2.size()));
      CHECK_EQUAL(crc1, crc2);

      uint64_t crc3 = etl::crc64_ecma(data3.rbegin(), data3.rend());
      CHECK_EQUAL(crc1, crc3);
    }
  };
}

