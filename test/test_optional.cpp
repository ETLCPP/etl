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

#include <optional>


#include "etl/optional.h"
#include "etl/vector.h"
#include "data.h"

typedef TestDataNDC<std::string> Data;
typedef TestDataM<uint32_t>      DataM;

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
      CHECK(!data1.has_value());
      CHECK(!data2.has_value());

      data1 = Data("Hello");
      CHECK(bool(data1));
      CHECK(data1.has_value());
      CHECK_EQUAL(Data("Hello"), data1);

      data1 = data2;
      CHECK(!bool(data1));
      CHECK(!bool(data2));
      CHECK(!data1.has_value());
      CHECK(!data2.has_value());

      data1 = Data("World");
      data2 = data1;
      CHECK(bool(data1));
      CHECK(bool(data2));
      CHECK(data1.has_value());
      CHECK(data2.has_value());

      etl::optional<Data> data3(data1);
      CHECK(bool(data3));
      CHECK(data3.has_value());
      CHECK_EQUAL(data1, data3);

      etl::optional<Data> data4;
      data4 = Data("Hello");
      data4 = etl::nullopt;
      CHECK(!bool(data4));
      CHECK(!data4.has_value());
    }

    //*************************************************************************
    TEST(test_deduced_initialisation)
    {
      Data data("Hello");

      etl::optional opt{ data };

      CHECK(opt.has_value());
      CHECK(bool(opt));
      CHECK_EQUAL(data, opt);
    }

    //*************************************************************************
    TEST(test_emplace)
    {
      etl::optional<DataM> data;

      data.emplace(1U);
      CHECK_EQUAL(1U, data.value().value);

      data.emplace(2U);
      CHECK_EQUAL(2U, data.value().value);

      data.emplace(3U);
      CHECK_EQUAL(3U, data.value().value);

      CHECK_EQUAL(1, DataM::get_instance_count());
    }

    //*************************************************************************
    TEST(test_moveable)
    {
#include "etl/private/diagnostic_pessimizing_move_push.h"
      etl::optional<DataM> data(std::move(DataM(1)));
      CHECK_EQUAL(1U, data.value().value);
      CHECK(bool(data));

      data = std::move(etl::optional<DataM>(std::move(DataM(2))));
      CHECK_EQUAL(2U, data.value().value);
      CHECK(bool(data));

      etl::optional<DataM> data2(etl::move(data));
      CHECK_EQUAL(2U, data2.value().value);
      CHECK(bool(data2));
#include "etl/private/diagnostic_pop.h"
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
      CHECK(data2 == data1);

      data1 = Data("Data1");
      CHECK(!(data1 == data2));
      CHECK(!(data2 == data1));

      data1 = etl::nullopt;
      data2 = Data("Data2");
      CHECK(!(data1 == data2));
      CHECK(!(data2 == data1));

      data1 = Data("Data1");
      data2 = Data("Data1");
      CHECK(data1 == data2);
      CHECK(data2 == data1);

      data1 = Data("Data1");
      data2 = Data("Data2");
      CHECK(!(data1 == data2));
      CHECK(!(data2 == data1));

      CHECK(!(etl::nullopt == data2));
      CHECK(!(data1 == etl::nullopt));

      CHECK(data1 == Data("Data1"));
      CHECK(!(data1 == Data("Data2")));
      CHECK(Data("Data1") == data1);
      CHECK(!(Data("Data2") == data1));
    }

    //*************************************************************************
    TEST(test_inequality)
    {
      etl::optional<Data> data1;
      etl::optional<Data> data2;

      CHECK(!(data1 != data2));
      CHECK(!(data2 != data1));

      data1 = Data("Data1");
      CHECK(data1 != data2);
      CHECK(data2 != data1);

      data1 = etl::nullopt;
      data2 = Data("Data2");
      CHECK(data1 != data2);
      CHECK(data2 != data1);

      data1 = Data("Data1");
      data2 = Data("Data1");
      CHECK(!(data1 != data2));
      CHECK(!(data2 != data1));

      data1 = Data("Data1");
      data2 = Data("Data2");
      CHECK(data1 != data2);
      CHECK(data2 != data1);

      CHECK(etl::nullopt != data2);
      CHECK(data1 != etl::nullopt);

      CHECK(!(data1 != Data("Data1")));
      CHECK(data1 != Data("Data2"));
      CHECK(!(Data("Data1") != data1));
      CHECK(Data("Data2") != data1);
    }

    //*************************************************************************
    TEST(test_less_than)
    {
      etl::optional<Data> data1;
      etl::optional<Data> data2;

      CHECK(!(data2 < data1));
      CHECK(!(data1 < data2));

      data1 = Data("Data1");
      CHECK(!(data1 < data2));
      CHECK(data2 < data1);

      data1 = etl::nullopt;
      data2 = Data("Data2");
      CHECK(data1 < data2);
      CHECK(!(data2 < data2));

      data1 = Data("Data1");
      data2 = Data("Data2");
      CHECK(data1 < data2);
      CHECK(!(data2 < data1));

      CHECK(etl::nullopt < data2);
      CHECK(!(data1 < etl::nullopt));

      CHECK(data1 < Data("Data2"));
      CHECK(!(data1 < Data("Data1")));
      CHECK(!(Data("Data2") < data1));
      CHECK(Data("Data1") < data2);
    }

    //*************************************************************************
    TEST(test_less_than_equal)
    {
      etl::optional<Data> data1;
      etl::optional<Data> data2;

      CHECK(data1 <= data2);
      CHECK(data2 <= data1);

      data1 = Data("Data1");
      CHECK(!(data1 <= data2));
      CHECK(data2 <= data1);

      data1 = etl::nullopt;
      data2 = Data("Data2");
      CHECK(data1 <= data2);
      CHECK(!(data2 <= data1));

      data1 = Data("Data1");
      data2 = Data("Data2");
      CHECK(data1 <= data2);
      CHECK(!(data2 <= data1));

      CHECK(etl::nullopt <= data2);
      CHECK(!(data1 <= etl::nullopt));

      CHECK(data1 <= Data("Data2"));
      CHECK(!(data2 <= Data("Data1")));
      CHECK(data1 <= Data("Data1"));
      CHECK(!(Data("Data2") <= data1));
      CHECK(Data("Data1") <= data2);
      CHECK(Data("Data1") <= data1);
    }

    //*************************************************************************
    TEST(test_greater_than)
    {
      etl::optional<Data> data1;
      etl::optional<Data> data2;

      CHECK(!(data1 > data2));
      CHECK(!(data2 > data1));

      data1 = Data("Data1");
      CHECK(data1 > data2);
      CHECK(!(data2 > data1));

      data1 = etl::nullopt;
      data2 = Data("Data2");
      CHECK(!(data1 > data2));
      CHECK(data2 > data1);

      data1 = Data("Data1");
      data2 = Data("Data2");
      CHECK(data2 > data1);
      CHECK(!(data1 > data2));

      CHECK(!(etl::nullopt > data2));
      CHECK(data1 > etl::nullopt);

      CHECK(!(data1 > Data("Data2")));
      CHECK(data2 > Data("Data1"));
      CHECK(Data("Data2") > data1);
      CHECK(!(Data("Data1") > data2));
    }

    //*************************************************************************
    TEST(test_greater_than_equal)
    {
      etl::optional<Data> data1;
      etl::optional<Data> data2;

      CHECK(data1 >= data2);
      CHECK(data2 >= data1);

      data1 = Data("Data1");
      CHECK(data1 >= data2);
      CHECK(!(data2 >= data1));

      data1 = etl::nullopt;
      data2 = Data("Data2");
      CHECK(!(data1 >= data2));
      CHECK(data2 >= data1);

      data1 = Data("Data1");
      data2 = Data("Data2");
      CHECK(!(data1 >= data2));
      CHECK(data2 >= data1);

      CHECK(!(etl::nullopt >= data2));
      CHECK(data1 >= etl::nullopt);

      CHECK(!(data1 >= Data("Data2")));
      CHECK(data2 >= Data("Data1"));
      CHECK(data1 >= Data("Data1"));
      CHECK(Data("Data2") >= data1);
      CHECK(!(Data("Data1") >= data2));
      CHECK(Data("Data1") >= data1);
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

    //*************************************************************************
    TEST(test_reset)
    {
      etl::optional<Data> data(Data("1"));
      CHECK(bool(data));

      data.reset();
      CHECK(!bool(data));
    }

    //*************************************************************************
    etl::optional<std::uint8_t> get_optional_test_bug_634()
    {
      etl::optional<std::uint8_t> result = 8;
      result.reset();
      
      return result;
    }

    TEST(test_bug_634)
    {
      etl::optional<std::uint8_t> result;

      result = get_optional_test_bug_634();

      CHECK_EQUAL(false, result.has_value());
    }

    //*************************************************************************
    TEST(test_optional_emplace_bug_636)
    {
      etl::optional<std::uint8_t> result = 1;
      result.emplace(2);

      CHECK_TRUE(result.has_value());
      CHECK_EQUAL(2, result.value());
    }
  };
}
