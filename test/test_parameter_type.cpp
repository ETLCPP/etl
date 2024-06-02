/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl
https://www.etlcpp.com

Copyright(c) 2015 John Wellbelove

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
#include <ostream>

#include "etl/parameter_type.h"

namespace
{
  class Object
  {
  public:

    Object()
      : p(new int())
    {
      *p = 4;
    }

    ~Object()
    {
      delete p;
    }

    int* p;
  };

  class Object2 : public Object
  {
  };
}

namespace etl
{
  // Object2 is a value parameter type.
  template <>
  struct parameter_type<Object2>
  {
    typedef Object2 type;
  };
}

namespace
{
  SUITE(test_parameter_type)
  {
    //*************************************************************************
    TEST(test_parameters)
    {
      bool b;

      b = !etl::is_reference<etl::parameter_type<int>::type>::value;
      CHECK(b);

      b = etl::is_reference<etl::parameter_type<Object>::type>::value;
      CHECK(b);

      b = !etl::is_reference<etl::parameter_type<Object2>::type>::value;
      CHECK(b);
    }
  };
}
