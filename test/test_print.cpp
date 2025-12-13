/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl
https://www.etlcpp.com

Copyright(c) 2025 BMW AG

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

#include "etl/print.h"

#if ETL_USING_CPP11

namespace {
  using iterator = etl::back_insert_iterator<etl::istring>;

  etl::string<100> output;
}

// to be implemented in a concrete project, typically printing to a serial console
extern "C" void etl_putchar(int c)
{
  iterator output_it(output);
  *output_it = static_cast<etl::private_print::char_type>(c);
}

namespace
{
  SUITE(test_format)
  {
    //*************************************************************************
    TEST(test_print)
    {
      output.clear();
      etl::print("Hello Print!");
      CHECK_EQUAL("Hello Print!", output);

      output.clear();
      etl::print("{}", "Hello format!");
      CHECK_EQUAL("Hello format!", output);

      output.clear();
      etl::print("Hello {}!", 321);
      CHECK_EQUAL("Hello 321!", output);
    }

    //*************************************************************************
    TEST(test_println)
    {
      output.clear();
      etl::println("Hello Println!");
      CHECK_EQUAL("Hello Println!\n", output);

      output.clear();
      etl::println();
      CHECK_EQUAL("\n", output);
    }
  }
}

#endif