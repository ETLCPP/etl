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

#include "etl/timer_node.h"

namespace
{
  SUITE(test_timer_config_and_node)
  {
    //*************************************************************************
    TEST(test_timer_config)
    {
      constexpr etl::timer::config config1(100U, etl::timer::mode::Repeating);
      CHECK_EQUAL(100U, config1.get_interval());
      CHECK_EQUAL(etl::timer::mode::Repeating, config1.get_mode());

      etl::timer::config config2(200U, etl::timer::mode::Single_Shot);
      CHECK_EQUAL(200U, config2.get_interval());
      CHECK_EQUAL(etl::timer::mode::Single_Shot, config2.get_mode());

      etl::timer::config config3(config2);
      CHECK_EQUAL(config2.get_interval(), config3.get_interval());
      CHECK_EQUAL(etl::timer::mode::Single_Shot, config2.get_mode());

      etl::timer::config config4 = etl::timer::config(500U, etl::timer::mode::Repeating);
      CHECK_EQUAL(500U, config4.get_interval());
      CHECK_EQUAL(etl::timer::mode::Repeating, config4.get_mode());

      config2.clear();
      CHECK_EQUAL(etl::timer::interval::No_Interval, config2.get_interval());
      CHECK_EQUAL(etl::timer::mode::No_Mode, config2.get_mode());
    }

    //*************************************************************************
    TEST(test_timer_node)
    {
      constexpr etl::timer::config config1(100U, etl::timer::mode::Repeating);

      etl::timer::node node1(config1);
      etl::timer::node node2(node1);

      const etl::timer::config& config1_ref = node1.get_config();

      CHECK_EQUAL(config1.get_interval(), node1.get_config().get_interval());
      CHECK_EQUAL(config1.get_mode(), node1.get_config().get_mode());

      CHECK_EQUAL(node1.get_config().get_interval(), node2.get_config().get_interval());
      CHECK_EQUAL(node1.get_config().get_mode(), node2.get_config().get_mode());
    }
  }
} // namespace
