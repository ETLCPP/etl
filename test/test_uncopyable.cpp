/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl
https://www.etlcpp.com

Copyright(c) 2024 BMW AG

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

#include "etl/uncopyable.h"

namespace
{
  class UncopyableClass: public etl::uncopyable
  {
  };

  SUITE(test_uncopyable)
  {
    //*************************************************************************
    TEST(test_construct)
    {
      UncopyableClass uc;

      (void) uc;
    }

    //*************************************************************************
    // This code is intentionally not compilable when activated
#if 0
    TEST(test_copy)
    {
      UncopyableClass uc1;

      // Can't be copied by construction
      UncopyableClass uc2{uc1}; // compile error
      (void) uc2;

      // Can't be copied by copy assignment
      UncopyableClass uc3;
      uc3 = uc1; // compile error
    }
#endif
  };
}
