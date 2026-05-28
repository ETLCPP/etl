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

#include <array>
#include <cstdint>
#include <ostream>
#include <string>
#include <type_traits>
#include <utility>

#include "data.h"
#include "etl/algorithm.h"
#include "etl/optional.h"
#include "etl/vector.h"

typedef TestDataNDC<std::string> Data;
typedef TestDataM<uint32_t>      DataM;

std::ostream& operator<<(std::ostream& os, const Data& data)
{
  os << data.value;
  return os;
}

std::ostream& operator<<(std::ostream& os, const etl::optional<Data>& data)
{
  os << data.value();
  return os;
}

namespace
{
#include "etl/private/diagnostic_unused_function_push.h"
  //*************************************************************************
  struct NonTrivial
  {
    constexpr NonTrivial()
      : a(0)
    {
    }

    constexpr NonTrivial(int a_)
      : a(a_)
    {
    }

    constexpr friend bool operator<(const NonTrivial& lhs, const NonTrivial& rhs)
    {
      return lhs.a < rhs.a;
    }

    constexpr friend bool operator==(const NonTrivial& lhs, const NonTrivial& rhs)
    {
      return lhs.a == rhs.a;
    }

    int a;
  };
#include "etl/private/diagnostic_pop.h"

  struct TestIL
  {
    constexpr TestIL()
      : a(0)
      , b(0)
      , c(0)
    {
    }

    ETL_CONSTEXPR20 TestIL(std::initializer_list<int> il, int a_, int b_, int c_)
      : a(a_)
      , b(b_)
      , c(c_)
    {
      etl::copy_n(il.begin(), std::min(il.size(), arr.size()), arr.begin());
    }

    std::array<int, 3> arr{};
    int                a;
    int                b;
    int                c;
  };

  SUITE(test_optional)
  {
    //*************************************************************************
    TEST(test_initialisation)
    {
      etl::optional<Data> data1;
      etl::optional<Data> data2;

      CHECK(!static_cast<bool>(data1));
      CHECK(!static_cast<bool>(data2));
      CHECK(!data1.has_value());
      CHECK(!data2.has_value());

      data1 = Data("Hello");
      CHECK(static_cast<bool>(data1));
      CHECK(data1.has_value());
      CHECK_EQUAL(Data("Hello"), data1);

      data1 = data2;
      CHECK(!static_cast<bool>(data1));
      CHECK(!static_cast<bool>(data2));
      CHECK(!data1.has_value());
      CHECK(!data2.has_value());

      data1 = Data("World");
      data2 = data1;
      CHECK(static_cast<bool>(data1));
      CHECK(static_cast<bool>(data2));
      CHECK(data1.has_value());
      CHECK(data2.has_value());

      const etl::optional<Data> data3(data1);
      CHECK(static_cast<bool>(data3));
      CHECK(data3.has_value());
      CHECK_EQUAL(data1, data3);

      etl::optional<Data> data4;
      data4 = Data("Hello");
      data4 = etl::nullopt;
      CHECK(!static_cast<bool>(data4));
      CHECK(!data4.has_value());
    }

#if ETL_USING_CPP14
    //*************************************************************************
    TEST(test_emplace_construction_cpp14)
    {
      constexpr etl::optional<int> opt(etl::in_place_t{}, 1);

      CHECK_TRUE(opt.has_value());
      CHECK(static_cast<bool>(opt));
      CHECK_EQUAL(1, opt.value());
    }
#endif

#if ETL_USING_CPP20 && ETL_USING_STL
    //*************************************************************************
    TEST(test_emplace_construction_cpp20)
    {
      struct TestData
      {
        constexpr TestData(int a_, int b_)
          : a(a_)
          , b(b_)
        {
        }

        int a;
        int b;
      };

      constexpr etl::optional<TestData> opt(etl::in_place_t{}, 1, 2);

      CHECK_TRUE(opt.has_value());
      CHECK(static_cast<bool>(opt));
      CHECK_EQUAL(1, opt.value().a);
      CHECK_EQUAL(2, opt.value().b);
    }
#endif

    //*************************************************************************
    TEST(test_construct_from_initializer_list_and_arguments)
    {
      etl::optional<TestIL> opt(etl::in_place_t{}, {10, 11, 12}, 1, 2, 3);

      CHECK_EQUAL(10, opt.value().arr[0]);
      CHECK_EQUAL(11, opt.value().arr[1]);
      CHECK_EQUAL(12, opt.value().arr[2]);
      CHECK_EQUAL(1, opt.value().a);
      CHECK_EQUAL(2, opt.value().b);
      CHECK_EQUAL(3, opt.value().c);
    }

    //*************************************************************************
    TEST(test_deduced_initialisation)
    {
      Data data("Hello");

      const etl::optional<Data> opt{data};

      CHECK(opt.has_value());
      CHECK(static_cast<bool>(opt));
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
    TEST(test_emplace_zero_parameters_fundamental)
    {
      etl::optional<std::uint8_t> result = 1;
      CHECK_EQUAL(0, static_cast<int>(result.emplace()));

      CHECK_TRUE(result.has_value());
      CHECK_EQUAL(0, int(result.value()));
    }

    //*************************************************************************
    TEST(test_emplace_zero_parameters_non_fundamental)
    {
      etl::optional<std::string> result = std::string("abc");

      std::string& ref = result.emplace();
      CHECK_EQUAL(std::string(), ref);
      CHECK_EQUAL(&ref, &result.value());
      CHECK_TRUE(result.has_value());
      CHECK_EQUAL("", std::string(result.value()));
    }

    //*************************************************************************
    TEST(test_emplace_return)
    {
      etl::optional<DataM> data;

      DataM* datam = &data.emplace(1U);
      CHECK_EQUAL(datam, &data.value());
      CHECK(datam != nullptr);
    }

    //*************************************************************************
    TEST(test_moveable_not_fundamental)
    {
#include "etl/private/diagnostic_pessimizing_move_push.h"

      // Construct by moving value.
      etl::optional<DataM> data(std::move(DataM(1)));
      CHECK(data.has_value());
      CHECK(data->valid);
      CHECK_EQUAL(1U, data.value().value);
      CHECK(static_cast<bool>(data));

      // Assign by moving optional.
      {
        etl::optional<DataM> temp(DataM(2));
        data = std::move(temp);
        CHECK(temp.has_value() && !temp->valid); // NOLINT "Note that a moved-from optional still contains a value (although invalid one)."
        CHECK(data.has_value());
        CHECK(data->valid);
        CHECK(static_cast<bool>(data));
        CHECK_EQUAL(2U, data.value().value);
      }

      // Construct by moving optional.
      {
        etl::optional<DataM> data2(etl::move(data));
        CHECK(data.has_value() && !data->valid); // NOLINT "Note that a moved-from optional still contains a value (although invalid one)."
        CHECK(data2.has_value());
        CHECK(data2->valid);
        CHECK(static_cast<bool>(data2));
        CHECK_EQUAL(2U, data2.value().value);
      }

      // Try to move construct/assign from valueless.
      {
        etl::optional<DataM> temp;
        etl::optional<DataM> data2(etl::move(temp));
        CHECK(!data2.has_value());

        data2 = etl::move(etl::optional<DataM>());
        CHECK(!data2.has_value());
      }
#include "etl/private/diagnostic_pop.h"
    }

    //*************************************************************************
    TEST(test_moveable_fundamental)
    {
#include "etl/private/diagnostic_pessimizing_move_push.h"

      // Construct by moving value.
      etl::optional<std::uint8_t> data(1U);
      CHECK(data.has_value());
      CHECK_EQUAL(1U, data.value());
      CHECK(static_cast<bool>(data));

      // Assign by moving optional.
      {
        etl::optional<std::uint8_t> temp(2U);
        data = std::move(temp);
        CHECK(temp.has_value()); // NOLINT "Note that a moved-from optional still contains a value."
        CHECK(data.has_value());
        CHECK(static_cast<bool>(data));
        CHECK_EQUAL(2U, data.value());
      }

      // Construct by moving optional.
      {
        etl::optional<std::uint8_t> data2(etl::move(data));
        CHECK(data.has_value()); // NOLINT "Note that a moved-from optional still contains a value."
        CHECK(data2.has_value());
        CHECK(static_cast<bool>(data2));
        CHECK_EQUAL(2U, data2.value());
      }

      // Try to move construct/assign from valueless.
      {
        etl::optional<std::uint8_t> temp;
        etl::optional<std::uint8_t> data2(etl::move(temp));
        CHECK(!data2.has_value());

        data2 = etl::move(etl::optional<std::uint8_t>());
        CHECK(!data2.has_value());
      }
#include "etl/private/diagnostic_pop.h"
    }

    //*************************************************************************
    TEST(test_nullopt_pod)
    {
      etl::optional<int> data(etl::nullopt);
      data = 1;
      data = etl::nullopt;
      CHECK(!static_cast<bool>(data));
    }

    //*************************************************************************
    TEST(test_nullopt)
    {
      etl::optional<Data> data(etl::nullopt);
      data = Data("Hello");
      data = etl::nullopt;
      CHECK(!static_cast<bool>(data));
    }

    //*************************************************************************
    TEST(test_value_or)
    {
      etl::optional<Data> data;

      Data result = data.value_or(Data("Default"));
      CHECK_EQUAL(Data("Default"), result);

      data   = Data("Value");
      result = data.value_or(Data("Default"));
      CHECK_EQUAL(Data("Value"), result);
    }

    //*************************************************************************
    TEST(test_value_or_const)
    {
      using FundamentalType    = int;
      using NonFundamentalType = std::string;

      const FundamentalType constFT{5};
      int                   resultFT = etl::optional<FundamentalType>{}.value_or(constFT);
      CHECK_EQUAL(5, resultFT);

      const NonFundamentalType constNFT{"Default"};
      const NonFundamentalType resultNFT = etl::optional<NonFundamentalType>{}.value_or(constNFT);
      CHECK_EQUAL("Default", resultNFT);
    }

    //*************************************************************************
    struct github_bug_720_bug_helper
    {
      int value{5};

      etl::optional<int> get_valid() const
      {
        return value;
      }

      etl::optional<int> get_invalid() const
      {
        return etl::optional<int>();
      }
    };

    TEST(test_chained_value_or_github_bug_720)
    {
      const github_bug_720_bug_helper helper{};

      const int value1 = helper.get_valid().value_or(1);
      CHECK_EQUAL(5, value1);

      const int value2 = helper.get_invalid().value_or(1);
      CHECK_EQUAL(1, value2);
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

#if ETL_USING_CPP20 && ETL_USING_STL
    //*************************************************************************
    TEST(test_constexpr_equality_non_trivial)
    {
      constexpr etl::optional<NonTrivial> data1(1);
      constexpr etl::optional<NonTrivial> data2(2);

      constexpr bool eq1 = (data1 == data2);
      constexpr bool eq2 = (data1 == etl::nullopt);
      constexpr bool eq3 = (etl::nullopt == data1);

      CHECK_FALSE(eq1);
      CHECK_FALSE(eq2);
      CHECK_FALSE(eq3);
    }
#endif

#if ETL_USING_CPP14
    //*************************************************************************
    TEST(test_constexpr_equality_trivial)
    {
      constexpr etl::optional<int> data1(1);
      constexpr etl::optional<int> data2(2);

      constexpr bool eq1 = (data1 == data2);
      constexpr bool eq2 = (data1 == etl::nullopt);
      constexpr bool eq3 = (etl::nullopt == data1);

      CHECK_FALSE(eq1);
      CHECK_FALSE(eq2);
      CHECK_FALSE(eq3);
    }
#endif

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

#if ETL_USING_CPP20 && ETL_USING_STL
    //*************************************************************************
    TEST(test_constexpr_inequality_non_trivial)
    {
      constexpr etl::optional<NonTrivial> data1(1);
      constexpr etl::optional<NonTrivial> data2(2);

      constexpr bool eq1 = (data1 != data2);
      constexpr bool eq2 = (data1 != etl::nullopt);
      constexpr bool eq3 = (etl::nullopt != data1);

      CHECK_TRUE(eq1);
      CHECK_TRUE(eq2);
      CHECK_TRUE(eq3);
    }
#endif

#if ETL_USING_CPP14
    //*************************************************************************
    TEST(test_constexpr_inequality_trivial)
    {
      constexpr etl::optional<int> data1(1);
      constexpr etl::optional<int> data2(2);

      constexpr bool neq1 = (data1 != data2);
      constexpr bool neq2 = (data1 != etl::nullopt);
      constexpr bool neq3 = (etl::nullopt != data1);

      CHECK_TRUE(neq1);
      CHECK_TRUE(neq2);
      CHECK_TRUE(neq3);
    }
#endif

    //*************************************************************************
#include "etl/private/diagnostic_uninitialized_push.h"
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
#include "etl/private/diagnostic_pop.h"

#if ETL_USING_CPP20 && ETL_USING_STL
    //*************************************************************************
    TEST(test_constexpr_less_than_non_trivial)
    {
      constexpr etl::optional<NonTrivial> data1(1);
      constexpr etl::optional<NonTrivial> data2(2);

      constexpr bool lt1 = (data1 < data2);
      constexpr bool lt2 = (data2 < data1);
      constexpr bool lt3 = (data1 < etl::nullopt);
      constexpr bool lt4 = (etl::nullopt < data1);

      CHECK_TRUE(lt1);
      CHECK_FALSE(lt2);
      CHECK_FALSE(lt3);
      CHECK_TRUE(lt4);
    }
#endif

#if ETL_USING_CPP14
    //*************************************************************************
    TEST(test_constexpr_less_than_trivial)
    {
      constexpr etl::optional<int> data1(1);
      constexpr etl::optional<int> data2(2);

      constexpr bool lt1 = (data1 < data2);
      constexpr bool lt2 = (data2 < data1);
      constexpr bool lt3 = (data1 < etl::nullopt);
      constexpr bool lt4 = (etl::nullopt < data1);

      CHECK_TRUE(lt1);
      CHECK_FALSE(lt2);
      CHECK_FALSE(lt3);
      CHECK_TRUE(lt4);
    }
#endif

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

#if ETL_USING_CPP20 && ETL_USING_STL
    //*************************************************************************
    TEST(test_constexpr_less_than_equal_non_trivial)
    {
      constexpr etl::optional<NonTrivial> data1(1);
      constexpr etl::optional<NonTrivial> data2(2);

      constexpr bool lt1 = (data1 <= data2);
      constexpr bool lt2 = (data2 <= data1);
      constexpr bool lt3 = (data1 <= etl::nullopt);
      constexpr bool lt4 = (etl::nullopt <= data1);

      CHECK_TRUE(lt1);
      CHECK_FALSE(lt2);
      CHECK_FALSE(lt3);
      CHECK_TRUE(lt4);
    }
#endif

#if ETL_USING_CPP14
    //*************************************************************************
    TEST(test_constexpr_less_than_equal_trivial)
    {
      constexpr etl::optional<int> data1(1);
      constexpr etl::optional<int> data2(2);

      constexpr bool lt1 = (data1 <= data2);
      constexpr bool lt2 = (data2 <= data1);
      constexpr bool lt3 = (data1 <= etl::nullopt);
      constexpr bool lt4 = (etl::nullopt <= data1);

      CHECK_TRUE(lt1);
      CHECK_FALSE(lt2);
      CHECK_FALSE(lt3);
      CHECK_TRUE(lt4);
    }
#endif

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

#if ETL_USING_CPP20 && ETL_USING_STL
    //*************************************************************************
    TEST(test_constexpr_greater_than_non_trivial)
    {
      constexpr etl::optional<NonTrivial> data1(1);
      constexpr etl::optional<NonTrivial> data2(2);

      constexpr bool gt1 = (data1 > data2);
      constexpr bool gt2 = (data2 > data1);
      constexpr bool gt3 = (data1 > etl::nullopt);
      constexpr bool gt4 = (etl::nullopt > data1);

      CHECK_FALSE(gt1);
      CHECK_TRUE(gt2);
      CHECK_TRUE(gt3);
      CHECK_FALSE(gt4);
    }
#endif

#if ETL_USING_CPP14
    //*************************************************************************
    TEST(test_constexpr_greater_than_trivial)
    {
      constexpr etl::optional<int> data1(1);
      constexpr etl::optional<int> data2(2);

      constexpr bool gt1 = (data1 > data2);
      constexpr bool gt2 = (data2 > data1);
      constexpr bool gt3 = (data1 > etl::nullopt);
      constexpr bool gt4 = (etl::nullopt > data1);

      CHECK_FALSE(gt1);
      CHECK_TRUE(gt2);
      CHECK_TRUE(gt3);
      CHECK_FALSE(gt4);
    }
#endif

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

#if ETL_USING_CPP20 && ETL_USING_STL
    //*************************************************************************
    TEST(test_constexpr_greater_than_equal_non_trivial)
    {
      constexpr etl::optional<NonTrivial> data1(1);
      constexpr etl::optional<NonTrivial> data2(2);

      constexpr bool gte1 = (data1 >= data2);
      constexpr bool gte2 = (data2 >= data1);
      constexpr bool gte3 = (data1 >= etl::nullopt);
      constexpr bool gte4 = (etl::nullopt >= data1);

      CHECK_FALSE(gte1);
      CHECK_TRUE(gte2);
      CHECK_TRUE(gte3);
      CHECK_FALSE(gte4);
    }
#endif

#if ETL_USING_CPP14
    //*************************************************************************
    TEST(test_constexpr_greater_than_equal_trivial)
    {
      constexpr etl::optional<int> data1(1);
      constexpr etl::optional<int> data2(2);

      constexpr bool gte1 = (data1 >= data2);
      constexpr bool gte2 = (data2 >= data1);
      constexpr bool gte3 = (data1 >= etl::nullopt);
      constexpr bool gte4 = (etl::nullopt >= data1);

      CHECK_FALSE(gte1);
      CHECK_TRUE(gte2);
      CHECK_TRUE(gte3);
      CHECK_FALSE(gte4);
    }
#endif

    //*************************************************************************
    TEST(test_container_of_optional)
    {
      etl::vector<etl::optional<Data>, 10> container;

      container.resize(5, Data("1"));

      CHECK(static_cast<bool>(container[0]));
      CHECK(static_cast<bool>(container[1]));
      CHECK(static_cast<bool>(container[2]));
      CHECK(static_cast<bool>(container[3]));
      CHECK(static_cast<bool>(container[4]));
    }

    //*************************************************************************
    TEST(test_optional_container)
    {
      // The indexed access doesn't work in Linux for some reason!!!
#ifndef ETL_PLATFORM_LINUX
      etl::optional<etl::vector<Data, 10>> container;
      CHECK(!static_cast<bool>(container)); //

      container = etl::vector<Data, 10>();
      CHECK(static_cast<bool>(container));

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

      CHECK_THROW(data1.value(), etl::optional_invalid);
    }

    //*************************************************************************
    TEST(test_swap)
    {
      const etl::optional<Data> original1(Data("1"));
      const etl::optional<Data> original2(Data("2"));

      etl::optional<Data> data1;
      etl::optional<Data> data2;

      // Both invalid.
      swap(data1, data2);
      CHECK(!static_cast<bool>(data1));
      CHECK(!static_cast<bool>(data2));

      // data1 is valid
      data1 = original1;
      data2 = etl::nullopt;
      swap(data1, data2);
      CHECK(!static_cast<bool>(data1));
      CHECK(static_cast<bool>(data2));
      CHECK_EQUAL(data2, original1);

      // data2 is valid
      data1 = etl::nullopt;
      data2 = original2;
      swap(data1, data2);
      CHECK(static_cast<bool>(data1));
      CHECK(!static_cast<bool>(data2));
      CHECK_EQUAL(data1, original2);

      // both are valid
      data1 = original1;
      data2 = original2;
      swap(data1, data2);
      CHECK(static_cast<bool>(data1));
      CHECK(static_cast<bool>(data2));
      CHECK_EQUAL(data1, original2);
      CHECK_EQUAL(data2, original1);
    }

    //*************************************************************************
    TEST(test_swap_moveable)
    {
      etl::optional<DataM> data1(1U);
      etl::optional<DataM> data2(2U);
      swap(data1, data2);
      CHECK_EQUAL(2U, data1.value().value);
      CHECK_EQUAL(1U, data2.value().value);
    }

    //*************************************************************************
    TEST(test_reset)
    {
      etl::optional<Data> data(Data("1"));
      CHECK(static_cast<bool>(data));

      data.reset();
      CHECK(!static_cast<bool>(data));
    }

    //*************************************************************************
    etl::optional<std::uint8_t> get_optional_test_bug_634()
    {
      etl::optional<std::uint8_t> result = 8;
      result.reset();

      return result;
    }

    //*************************************************************************
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

    //*************************************************************************
    struct MyPODObject
    {
      MyPODObject() = delete;
      int value;
    };

    TEST(test_optional_pod_emplace_bug_712)
    {
      const etl::optional<MyPODObject> optionalObject; // The Test: Does this compile for an object with a
                                                       // deleted default constructor?

      // Make sure it isn't optimized away.
      CHECK_FALSE(optionalObject.has_value());
    }

    //*************************************************************************
    TEST(test_optional_pod_assign_bug_714)
    {
      etl::optional<int> opt = 42;
      opt                    = etl::nullopt;

      CHECK_EQUAL(false, opt.has_value());
    }

    //*************************************************************************
    TEST(test_dereference_operator_bug_730)
    {
      etl::optional<int> opt = 42;

      CHECK_EQUAL(42, *opt);
    }

    //*************************************************************************
    TEST(test_const_dereference_operator_bug_730)
    {
      const etl::optional<int> opt = 42;

      CHECK_EQUAL(42, *opt);
    }

    //*************************************************************************
    TEST(test_arrow_operator_bug_730)
    {
      struct Object
      {
        int value;
      };

      etl::optional<Object> opt = Object{42};

      CHECK_EQUAL(42, opt->value);
    }

    //*************************************************************************
    TEST(test_const_arrow_operator_bug_730)
    {
      struct Object
      {
        int value;
      };

      const etl::optional<Object> opt = Object{42};

      CHECK_EQUAL(42, opt->value);
    }

#if ETL_USING_CPP14
    //*************************************************************************
    TEST(test_optional_cannot_be_constexpr_765_pod)
    {
      constexpr etl::optional<int> opt(42);

      CHECK_EQUAL(42, *opt);
    }
#endif

#if ETL_USING_CPP20 && ETL_USING_STL
    //*************************************************************************
    TEST(test_optional_cannot_be_constexpr_765_non_pod)
    {
      struct NonPod
      {
        constexpr NonPod(int v_)
          : v(v_)
        {
        }

        int v;
      };

      constexpr NonPod                data(42);
      constexpr etl::optional<NonPod> opt = data;

      CHECK_EQUAL(42, (*opt).v);
    }
#endif

    //*************************************************************************
    TEST(test_optional_issue_819)
    {
      // The code below should compile without error.
      class optional_type
      {
      public:

        static etl::optional<optional_type> function()
        {
          return {};
        }
      };
    }

    //*************************************************************************
    using ItemType = etl::array<uint8_t, 2>;

    etl::optional<const ItemType> create_optional_issue_1171()
    {
      ItemType t;
      t[0] = 1;
      t[1] = 20;

      return etl::optional<const ItemType>(t);
    }

    TEST(test_optional_issue_1171)
    {
      etl::optional<const ItemType> opt1 = create_optional_issue_1171();
      CHECK_TRUE(opt1.has_value());
      CHECK_EQUAL(1, (*opt1)[0]);
      CHECK_EQUAL(20, (*opt1)[1]);

      etl::optional<const ItemType> opt2(create_optional_issue_1171());
      CHECK_TRUE(opt2.has_value());
      CHECK_EQUAL(1, (*opt2)[0]);
      CHECK_EQUAL(20, (*opt2)[1]);

      etl::optional<const ItemType> opt3;
      opt3.emplace(create_optional_issue_1171());
      CHECK_TRUE(opt3.has_value());
      CHECK_EQUAL(1, (*opt3)[0]);
      CHECK_EQUAL(20, (*opt3)[1]);
    }

    //*************************************************************************
    TEST(test_begin_end_with_value)
    {
      etl::optional<int> opt = 4;

      CHECK(opt.begin() != opt.end());
      CHECK_EQUAL(4, *opt.begin());
      CHECK_EQUAL(1, std::distance(opt.begin(), opt.end()));
    }

    TEST(test_begin_end_empty)
    {
      etl::optional<int> opt;

      CHECK(opt.begin() == ETL_NULLPTR);
      CHECK(opt.begin() == opt.end());
    }

    TEST(test_const_begin_end_with_value)
    {
      const etl::optional<int> opt = 4;

      CHECK(opt.begin() != opt.end());
      CHECK_EQUAL(4, *opt.begin());
      CHECK_EQUAL(1, std::distance(opt.begin(), opt.end()));
    }

    TEST(test_const_begin_end_empty)
    {
      const etl::optional<int> opt;

      CHECK(opt.begin() == ETL_NULLPTR);
      CHECK(opt.begin() == opt.end());
    }

    TEST(range_based_for_loop_with_value)
    {
      const etl::optional<int> opt = 4;

      int sum = 0;
      for (const int value : opt)
      {
        sum += value;
      }

      CHECK_EQUAL(4, sum);
    }

    TEST(range_based_for_loop_empty)
    {
      const etl::optional<int> opt;

      int sum = 0;
      for (const int value : opt)
      {
        sum += value;
      }

      CHECK_EQUAL(0, sum);
    }

    TEST(test_range_based_for_loop_non_trivial)
    {
      const etl::optional<Data> opt   = Data("TEST");
      int                       count = 0;

      for (const Data& value : opt)
      {
        CHECK_EQUAL(Data("TEST"), value);
        count++;
      }

      CHECK_EQUAL(1, count);
    }

    TEST(test_begin_end_modify_value)
    {
      etl::optional<int> opt = 4;

      *opt.begin() = 42;

      CHECK_EQUAL(42, *opt);
    }

    //*************************************************************************
    // GitHub issue #146: etl::optional doesn't compile with deleted copy
    // constructor
    //*************************************************************************
#if ETL_USING_CPP11
    struct Issue146_NonCopyable
    {
      Issue146_NonCopyable(int some)
        : _some(some)
      {
      }
      Issue146_NonCopyable(const Issue146_NonCopyable&)            = delete;
      Issue146_NonCopyable(Issue146_NonCopyable&&)                 = delete;
      Issue146_NonCopyable& operator=(const Issue146_NonCopyable&) = delete;

      int _some;
    };

    struct Issue146_Container
    {
      Issue146_Container(int a_val)
        : a(a_val)
      {
      }
      Issue146_Container()
        : a(etl::nullopt)
      {
      }

      etl::optional<Issue146_NonCopyable> a;
    };

    TEST(test_optional_issue_146_deleted_copy_ctor)
    {
      // etl::optional<T> should compile when T has deleted copy/move
      // constructors, as long as T is constructible from the given arguments.
      const Issue146_Container with_value(42);
      const Issue146_Container without_value;

      CHECK_TRUE(with_value.a.has_value());
      CHECK_EQUAL(42, with_value.a->_some);

      CHECK_FALSE(without_value.a.has_value());

      // in_place construction should also work
      const etl::optional<Issue146_NonCopyable> opt(etl::in_place_t{}, 99);
      CHECK_TRUE(opt.has_value());
      CHECK_EQUAL(99, opt->_some);
    }
#endif

    TEST(test_make_optional_1_lvalue)
    {
      const std::string         test_value("TEST");
      Data                      test_data(test_value);
      const etl::optional<Data> opt = etl::make_optional(test_data);
      CHECK_TRUE(opt.has_value());
      CHECK_EQUAL(test_value, opt.value().value);
    }

    TEST(test_make_optional_1_const_value)
    {
      const std::string         test_value("TEST");
      const Data                test_data(test_value);
      const etl::optional<Data> opt = etl::make_optional(test_data);
      CHECK_TRUE(opt.has_value());
      CHECK_EQUAL(test_data.value, opt.value().value);
    }

#if ETL_USING_CPP11
    TEST(test_make_optional_1_rvalue)
    {
      constexpr uint32_t         test_value = 42;
      DataM                      test_data(test_value);
      const etl::optional<DataM> opt = etl::make_optional(std::move(test_data));
      CHECK_TRUE(opt.has_value());
      CHECK_FALSE(test_data.valid);
      CHECK_EQUAL(test_value, opt.value().value);
    }
#endif

#if ETL_USING_CPP14
    TEST(test_make_optional_1_constexpr)
    {
      constexpr etl::optional<int> opt = etl::make_optional(42);
      CHECK_TRUE(opt.has_value());
      CHECK_EQUAL(42, opt.value());
    }
#endif

    TEST(test_make_optional_2_lvalue)
    {
      std::string test_value("TEST");
      const auto  opt = etl::make_optional<const Data>(test_value);
      CHECK_TRUE(opt.has_value());
      CHECK_EQUAL(test_value, opt.value().value);
    }

    TEST(test_make_optional_2_rvalue)
    {
      const etl::optional<DataM> opt = etl::make_optional<DataM>(42u);
      CHECK_TRUE(opt.has_value());
      CHECK_EQUAL(42, opt.value().value);
    }

#if ETL_USING_CPP14
    TEST(test_make_optional_2_constexpr)
    {
      constexpr etl::optional<uint32_t> opt = etl::make_optional<uint32_t>(42);
      CHECK_TRUE(opt.has_value());
      CHECK_EQUAL(42, opt.value());
    }
#endif

    TEST(test_make_optional_3)
    {
      int        test_value1(1);
      const int  test_value2(2);
      const auto opt = etl::make_optional<const TestIL>({10, 11, 12}, test_value1, test_value2, 3);
      CHECK_TRUE(opt.has_value());
      CHECK_EQUAL(10, opt->arr[0]);
      CHECK_EQUAL(11, opt->arr[1]);
      CHECK_EQUAL(12, opt->arr[2]);
      CHECK_EQUAL(test_value1, opt->a);
      CHECK_EQUAL(test_value2, opt->b);
      CHECK_EQUAL(3, opt->c);
    }

#if ETL_USING_CPP20 && ETL_USING_STL
    TEST(test_make_optional_3_constexpr)
    {
      constexpr etl::optional<TestIL> opt = etl::make_optional<TestIL>({1, 2}, 10, 20, 30);
      CHECK_TRUE(opt.has_value());
      CHECK_EQUAL(1, opt->arr[0]);
      CHECK_EQUAL(2, opt->arr[1]);
      CHECK_EQUAL(0, opt->arr[2]);
      CHECK_EQUAL(10, opt->a);
      CHECK_EQUAL(20, opt->b);
      CHECK_EQUAL(30, opt->c);
    }
#endif

    //*************************************************************************
    // Tests for noexcept properties of etl::optional
    // The noexcept specs only take effect when ETL_USING_EXCEPTIONS is enabled,
    // because ETL_NOEXCEPT_IF expands to nothing otherwise.
    // The etl::is_nothrow_* traits only work with STL or builtins.
    //*************************************************************************
#if ETL_USING_CPP11 && ETL_USING_EXCEPTIONS && (defined(ETL_USE_TYPE_TRAITS_BUILTINS) || (ETL_USING_STL && !defined(ETL_USER_DEFINED_TYPE_TRAITS)))
    struct NothrowAtAll
    {
      NothrowAtAll() noexcept {}
      NothrowAtAll(const NothrowAtAll&) noexcept {}
      NothrowAtAll(NothrowAtAll&&) noexcept {}
      NothrowAtAll(std::initializer_list<int>) noexcept {}
      NothrowAtAll& operator=(const NothrowAtAll&) noexcept
      {
        return *this;
      }
      NothrowAtAll& operator=(NothrowAtAll&&) noexcept
      {
        return *this;
      }
    };

    struct ThrowingCopy
    {
      ThrowingCopy() noexcept {}
      ThrowingCopy(const ThrowingCopy&) {} // may throw
      ThrowingCopy(ThrowingCopy&&) noexcept {}
      ThrowingCopy& operator=(const ThrowingCopy&) // may throw
      {
        return *this;
      }
      ThrowingCopy& operator=(ThrowingCopy&&) noexcept
      {
        return *this;
      }
    };

    struct ThrowingMove
    {
      ThrowingMove() noexcept {}
      ThrowingMove(const ThrowingMove&) noexcept {}
      ThrowingMove(ThrowingMove&&) {} // may throw
      ThrowingMove& operator=(const ThrowingMove&) noexcept
      {
        return *this;
      }
      ThrowingMove& operator=(ThrowingMove&&) // may throw
      {
        return *this;
      }
    };

    struct ThrowingAll
    {
      ThrowingAll() {}                           // may throw
      ThrowingAll(const ThrowingAll&) {}         // may throw
      ThrowingAll(ThrowingAll&&) {}              // may throw
      ThrowingAll(std::initializer_list<int>) {} // may throw
      ThrowingAll& operator=(const ThrowingAll&) // may throw
      {
        return *this;
      }
      ThrowingAll& operator=(ThrowingAll&&) // may throw
      {
        return *this;
      }
    };

    TEST(test_optional_nothrow_copy_constructible)
    {
      // When T is nothrow copy constructible, optional<T> should be too
      static_assert(etl::is_nothrow_copy_constructible<etl::optional<int>>::value, "optional<int> should be nothrow copy constructible");
      static_assert(etl::is_nothrow_copy_constructible<etl::optional<NothrowAtAll>>::value,
                    "optional<NothrowAtAll> should be nothrow copy constructible");

      // When T is NOT nothrow copy constructible, optional<T> should not be either
      static_assert(!etl::is_nothrow_copy_constructible<etl::optional<ThrowingCopy>>::value,
                    "optional<ThrowingCopy> should NOT be nothrow copy constructible");
      static_assert(!etl::is_nothrow_copy_constructible<etl::optional<ThrowingAll>>::value,
                    "optional<ThrowingAll> should NOT be nothrow copy constructible");

      // ThrowingMove has nothrow copy but throwing move
      static_assert(etl::is_nothrow_copy_constructible<etl::optional<ThrowingMove>>::value,
                    "optional<ThrowingMove> should be nothrow copy constructible");

      CHECK(true); // Placeholder for the static_asserts above
    }

    TEST(test_optional_nothrow_move_constructible)
    {
      // When T is nothrow move constructible, optional<T> (and swap) should be too
      static_assert(etl::is_nothrow_move_constructible<etl::optional<int>>::value, "optional<int> should be nothrow move constructible");
      static_assert(etl::is_nothrow_move_constructible<etl::optional<NothrowAtAll>>::value,
                    "optional<NothrowAtAll> should be nothrow move constructible");
      static_assert(noexcept(swap(std::declval<etl::optional<int>&>(), std::declval<etl::optional<int>&>())), "swap<int>() should be nothrow");
      static_assert(noexcept(swap(std::declval<etl::optional<NothrowAtAll>&>(), std::declval<etl::optional<NothrowAtAll>&>())),
                    "swap<NothrowAtAll>() should be nothrow");

      // When T is NOT nothrow move constructible, optional<T> (and swap) should not be either
      static_assert(!etl::is_nothrow_move_constructible<etl::optional<ThrowingMove>>::value,
                    "optional<ThrowingMove> should NOT be nothrow move constructible");
      static_assert(!etl::is_nothrow_move_constructible<etl::optional<ThrowingAll>>::value,
                    "optional<ThrowingAll> should NOT be nothrow move constructible");
      static_assert(!noexcept(swap(std::declval<etl::optional<ThrowingMove>&>(), std::declval<etl::optional<ThrowingMove>&>())),
                    "swap<ThrowingMove>() should NOT be nothrow");
      static_assert(!noexcept(swap(std::declval<etl::optional<ThrowingAll>&>(), std::declval<etl::optional<ThrowingAll>&>())),
                    "swap<ThrowingAll>() should NOT be nothrow");

      // ThrowingCopy has nothrow move but throwing copy
      static_assert(etl::is_nothrow_move_constructible<etl::optional<ThrowingCopy>>::value,
                    "optional<ThrowingCopy> should be nothrow move constructible");
      static_assert(noexcept(swap(std::declval<etl::optional<ThrowingCopy>&>(), std::declval<etl::optional<ThrowingCopy>&>())),
                    "swap<ThrowingCopy>() should be nothrow");

      CHECK(true); // Placeholder for the static_asserts above
    }

    TEST(test_optional_nothrow_default_constructible)
    {
      // Default construction of optional should always be noexcept
      static_assert(etl::is_nothrow_default_constructible<etl::optional<int>>::value, "optional<int> should be nothrow default constructible");
      static_assert(etl::is_nothrow_default_constructible<etl::optional<NothrowAtAll>>::value,
                    "optional<NothrowAtAll> should be nothrow default constructible");
      static_assert(etl::is_nothrow_default_constructible<etl::optional<ThrowingCopy>>::value,
                    "optional<ThrowingCopy> should be nothrow default constructible");
      static_assert(etl::is_nothrow_default_constructible<etl::optional<ThrowingAll>>::value,
                    "optional<ThrowingAll> should be nothrow default constructible");

      CHECK(true);
    }

    TEST(test_optional_nothrow_constructible_from_value)
    {
      // optional<T>(U&&) should be noexcept iff T is nothrow constructible from U&&
      static_assert(etl::is_nothrow_constructible<etl::optional<int>, int>::value, "optional<int> should be nothrow constructible from int");
      static_assert(etl::is_nothrow_constructible<etl::optional<int>, int&&>::value, "optional<int> should be nothrow constructible from int&&");

      CHECK(true);
    }

    TEST(test_optional_nothrow_copy_assignable)
    {
      // Copy assignment should propagate noexcept from T
      static_assert(etl::is_nothrow_copy_assignable<etl::optional<int>>::value, "optional<int> should be nothrow copy assignable");
      static_assert(etl::is_nothrow_copy_assignable<etl::optional<NothrowAtAll>>::value, "optional<NothrowAtAll> should be nothrow copy assignable");

      // ThrowingCopy has a throwing copy constructor, so copy assignment should not be noexcept
      static_assert(!etl::is_nothrow_copy_assignable<etl::optional<ThrowingCopy>>::value,
                    "optional<ThrowingCopy> should NOT be nothrow copy assignable");

      CHECK(true);
    }

    TEST(test_optional_nothrow_move_assignable)
    {
      // Move assignment should propagate noexcept from T
      static_assert(etl::is_nothrow_move_assignable<etl::optional<int>>::value, "optional<int> should be nothrow move assignable");
      static_assert(etl::is_nothrow_move_assignable<etl::optional<NothrowAtAll>>::value, "optional<NothrowAtAll> should be nothrow move assignable");

      // ThrowingMove has a throwing move constructor, so move assignment should not be noexcept
      static_assert(!etl::is_nothrow_move_assignable<etl::optional<ThrowingMove>>::value,
                    "optional<ThrowingMove> should NOT be nothrow move assignable");

      CHECK(true);
    }

    TEST(test_make_optional_nothrow)
    {
      // make_optional #1
      {
        NothrowAtAll nothrowAtAll{};
        static_assert(noexcept(etl::make_optional(nothrowAtAll)), "make_optional(NothrowAtAll&) should be nothrow");
        static_assert(noexcept(etl::make_optional(std::move(nothrowAtAll))), "make_optional(NothrowAtAll&&) should be nothrow");
        ThrowingAll throwingAll{};
        static_assert(!noexcept(etl::make_optional(throwingAll)), "make_optional(ThrowingAll&) should NOT be nothrow");
        static_assert(!noexcept(etl::make_optional(std::move(throwingAll))), "make_optional(ThrowingAll&&) should NOT be nothrow");
      }

      // make_optional #2
      {
        static_assert(noexcept(etl::make_optional<NothrowAtAll>()), "make_optional<NothrowAtAll>() should be nothrow");
        static_assert(noexcept(etl::make_optional<const NothrowAtAll>()), "make_optional<const NothrowAtAll>() should be nothrow");
        static_assert(!noexcept(etl::make_optional<ThrowingAll>()), "make_optional<ThrowingAll>() should NOT be nothrow");
        static_assert(!noexcept(etl::make_optional<const ThrowingAll>()), "make_optional<const ThrowingAll>() should NOT be nothrow");
      }

      // make_optional #3
      {
        static_assert(noexcept(etl::make_optional<NothrowAtAll>({1, 2, 3})), "make_optional<NothrowAtAll>(1,2,3) should be nothrow");
        static_assert(noexcept(etl::make_optional<const NothrowAtAll>({1, 2, 3})), "make_optional<const NothrowAtAll>(1,2,3) should be nothrow");
        static_assert(!noexcept(etl::make_optional<ThrowingAll>({1, 2, 3})), "make_optional<ThrowingAll>({1,2,3}) should NOT be nothrow");
        static_assert(!noexcept(etl::make_optional<const ThrowingAll>({1, 2, 3})), "make_optional<const ThrowingAll>({1,2,3}) should NOT be nothrow");
      }
    }
#endif
  }
} // namespace
