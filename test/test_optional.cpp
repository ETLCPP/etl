/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl
http://www.etlcpp.com

Copyright(c) 2015 jwellbelove

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

#include <string>
#include <ostream>

#include "optional.h"
#include "vector.h"
#include "data.h"

typedef TestDataNDC<std::string> Data;

std::ostream& operator << (std::ostream& os, const Data& data)
{
  os << data.value;
  return os;
}

std::ostream& operator << (std::ostream& os, const etl::optional<Data>& data)
{
  os << data.value();
  return os;
}

namespace
{
  SUITE(test_optional)
  {
    //*************************************************************************
    TEST(test_initialisation)
    {
      etl::optional<Data> data1;
      etl::optional<Data> data2;

      CHECK(!bool(data1));
      CHECK(!bool(data2));

      data1 = Data("Hello");
      CHECK(bool(data1));
      CHECK_EQUAL(Data("Hello"), data1);

      data1 = data2;
      CHECK(!bool(data1));
      CHECK(!bool(data2));

      data1 = Data("World");
      data2 = data1;
      CHECK(bool(data1));
      CHECK(bool(data2));

      etl::optional<Data> data3(data1);
      CHECK(bool(data3));
      CHECK_EQUAL(data1, data3);

      etl::optional<Data> data4;
      data4 = Data("Hello");
      data4 = etl::nullopt;
      CHECK(!bool(data4));
    }

    //*************************************************************************
    TEST(test_nullopt)
    {
      etl::optional<Data> data;
      data = Data("Hello");
      data = etl::nullopt;
      CHECK(!bool(data));
    }

    //*************************************************************************
    TEST(test_value_or)
    {
      etl::optional<Data> data;

      Data result = data.value_or(Data("Default"));
      CHECK_EQUAL(Data("Default"), result);

      data = Data("Value");
      result = data.value_or(Data("Default"));
      CHECK_EQUAL(Data("Value"), result);
    }

    //*************************************************************************
    TEST(test_equality)
    {
      etl::optional<Data> data1;
      etl::optional<Data> data2;

      CHECK(data1 == data2);

      data1 = Data("Data1");
      CHECK(!(data1 == data2));

      data1 = etl::nullopt;
      data2 = Data("Data2");
      CHECK(!(data1 == data2));

      data1 = Data("Data1");
      data2 = Data("Data1");
      CHECK(data1 == data2);

      data1 = Data("Data1");
      data2 = Data("Data2");
      CHECK(!(data1 == data2));

      CHECK(data1 == Data("Data1"));
      CHECK(!(data1 == Data("Data2")));
    }

    //*************************************************************************
    TEST(test_less_than)
    {
      etl::optional<Data> data1;
      etl::optional<Data> data2;

      CHECK(!(data1 < data2));

      data1 = Data("Data1");
      CHECK(!(data1 < data2));

      data1 = etl::nullopt;
      data2 = Data("Data2");
      CHECK(data1 < data2);

      data1 = Data("Data1");
      data2 = Data("Data2");
      CHECK(data1 < data2);
      CHECK(!(data2 < data1));

      CHECK(data1 < Data("Data2"));
      CHECK(!(data1 < Data("Data1")));
    }

    //*************************************************************************
    TEST(test_container_of_optional)
    {
      etl::vector<etl::optional<Data>, 10> container;

      container.resize(5, Data("1"));

      CHECK(bool(container[0]));
      CHECK(bool(container[1]));
      CHECK(bool(container[2]));
      CHECK(bool(container[3]));
      CHECK(bool(container[4]));
    }

    //*************************************************************************
    TEST(test_optional_container)
    {
      // The indexed access doesn't work in Linux for some reason!!!
#ifndef ETL_PLATFORM_LINUX
      etl::optional<etl::vector<Data, 10>> container;
      CHECK(!bool(container));//

      container = etl::vector<Data, 10>();
      CHECK(bool(container));

      container.value().resize(5, Data("1"));
      CHECK_EQUAL(5U, container.value().size());

      CHECK_EQUAL(Data("1"), container.value()[0]);
      CHECK_EQUAL(Data("1"), container.value()[1]);
      CHECK_EQUAL(Data("1"), container.value()[2]);
      CHECK_EQUAL(Data("1"), container.value()[3]);
      CHECK_EQUAL(Data("1"), container.value()[4]);
#endif
    }

    //*************************************************************************
    TEST(test_exception)
    {
      etl::optional<Data> data1;

      CHECK_THROW(Data d(data1.value()), etl::optional_invalid);
    }

    //*************************************************************************
    TEST(test_swap)
    {
      etl::optional<Data> original1(Data("1"));
      etl::optional<Data> original2(Data("2"));

      etl::optional<Data> data1;
      etl::optional<Data> data2;

      // Both invalid.
      swap(data1, data2);
      CHECK(!bool(data1));
      CHECK(!bool(data2));

      // Data1 valid;
      data1 = original1;
      data2 = etl::nullopt;
      swap(data1, data2);
      CHECK(!bool(data1));
      CHECK(bool(data2));
      CHECK_EQUAL(data2, original1);

      // Data2 valid;
      data1 = etl::nullopt;
      data2 = original2;
      swap(data1, data2);
      CHECK(bool(data1));
      CHECK(!bool(data2));
      CHECK_EQUAL(data1, original2);

      // Both valid;
      data1 = original1;
      data2 = original2;
      swap(data1, data2);
      CHECK(bool(data1));
      CHECK(bool(data2));
      CHECK_EQUAL(data1, original2);
      CHECK_EQUAL(data2, original1);
    }
  };
}
