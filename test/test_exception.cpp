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
#include <string>

#include "etl/exception.h"

namespace 
{		
  SUITE(test_exception)
  {
    //*************************************************************************
    TEST(test_constructor)
    {
      etl::exception e("An exception", "Some file", 123);

      CHECK_EQUAL(std::string("An exception"), std::string(e.what()));
      CHECK_EQUAL(std::string("Some file"), std::string(e.file_name()));
      CHECK_EQUAL(123, e.line_number());
    }

    //*************************************************************************
    TEST(test_exception)
    {
      etl::exception e("An exception", "Some file", 123);

      try
      {
        throw e;
      }
      catch (etl::exception& c)
      {
        CHECK_EQUAL(std::string("An exception"), std::string(c.what()));
        CHECK_EQUAL(std::string("Some file"), std::string(c.file_name()));
        CHECK_EQUAL(123, c.line_number());
      }     
    }
  };
}
