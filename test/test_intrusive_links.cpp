/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl
http://www.etlcpp.com

Copyright(c) 2016 jwellbelove

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

#include <UnitTest++/UnitTest++.h>
#include "ExtraCheckMacros.h"

#include "data.h"

#include "../intrusive_links.h"

namespace
{
  //*******************************************************
  typedef etl::bidirectional_link<0, etl::link_option::AUTO_UNLINK> FirstLink;
  typedef etl::bidirectional_link<1> SecondLink;

  struct Data : public FirstLink, public SecondLink
  {
    Data(int value)
      : value(value)
    {
    }

    int value;
  };

  //*******************************************************
  bool operator ==(const Data& lhs, const Data& rhs)
  {
    return lhs.value == rhs.value;
  }

  bool operator !=(const Data& lhs, const Data& rhs)
  {
    return !(lhs == rhs);
  }

  //*******************************************************
  template <typename TLink>
  void join(TLink* lhs, TLink* rhs)
  {
    if ((lhs == nullptr) && (rhs != nullptr))
    {
      rhs->etl_previous = nullptr;
    }
    else if ((lhs != nullptr) && (rhs == nullptr))
    {
      lhs->etl_next = nullptr;
    }
    else if ((lhs != nullptr) && (rhs != nullptr))
    {
      lhs->etl_next     = rhs;
      rhs->etl_previous = lhs;
    }
  }

  SUITE(test_forward_list)
  {   
    //*************************************************************************
    TEST(test_bidirectional_link)
    {
      // FirstLink is auto-unlink, SecondLink is not.

      Data* data0 = new Data(0);
      Data* data1 = new Data(1);
      Data* data2 = new Data(2);
      Data* data3 = new Data(3);

      join<FirstLink>(nullptr, data0);
      join<FirstLink>(data0,   data1);
      join<FirstLink>(data1,   data2);
      join<FirstLink>(data2,   data3);
      join<FirstLink>(data3,   nullptr);

      join<SecondLink>(nullptr, data0);
      join<SecondLink>(data0,   data1);
      join<SecondLink>(data1,   data2);
      join<SecondLink>(data2,   data3);
      join<SecondLink>(data3,   nullptr);

      delete data1;
      CHECK(data0->FirstLink::etl_next == data2);
      CHECK(data2->FirstLink::etl_previous == data0);

      CHECK(data0->SecondLink::etl_next != data2);
      CHECK(data0->SecondLink::etl_next != nullptr);
      CHECK(data2->SecondLink::etl_previous != data0);
      CHECK(data2->SecondLink::etl_previous != nullptr);

      delete data0;
      CHECK(data2->FirstLink::etl_next == data3);
      CHECK(data2->FirstLink::etl_previous == nullptr);
      CHECK(data3->FirstLink::etl_previous == data2);
      
      CHECK(data2->SecondLink::etl_next == data3);
      CHECK(data3->SecondLink::etl_previous != nullptr);

      delete data3;
      CHECK(data2->FirstLink::etl_next == nullptr);
      CHECK(data2->FirstLink::etl_previous == nullptr);

      CHECK(data2->SecondLink::etl_next != nullptr);
      CHECK(data2->SecondLink::etl_previous != nullptr);

      delete data2;
    }
  };
}
