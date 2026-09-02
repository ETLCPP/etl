/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl
https://www.etlcpp.com

Copyright(c) 2026 Julien Hecht

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

#include "clocks_for_unit_tests.h"
#include "etl/atomic.h"
#include "etl/chrono.h"

namespace test
{
  namespace clocks
  {
    etl::atomic<int> nanoseconds{0};
    etl::atomic<int> milliseconds{0};
    etl::atomic<int> seconds{0};
  } // namespace clocks
} // namespace test

extern "C"
{
  etl::chrono::system_clock::rep etl_get_system_clock()
  {
    return etl::chrono::system_clock::rep(test::clocks::milliseconds);
  }

  etl::chrono::high_resolution_clock::rep etl_get_high_resolution_clock()
  {
    return etl::chrono::high_resolution_clock::rep(test::clocks::nanoseconds);
  }

  etl::chrono::steady_clock::rep etl_get_steady_clock()
  {
    return etl::chrono::steady_clock::rep(test::clocks::seconds);
  }
}
