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
#include <cstdint>

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
#include "etl/private/diagnostic_unused_function_push.h"
  //*************************************************************************
  struct NonTrivial
  {
    constexpr NonTrivial() : a(0) {}

    constexpr NonTrivial(int a_) : a(a_) {}

    constexpr friend bool operator <(const NonTrivial& lhs, const NonTrivial& rhs)
    {
      return lhs.a < rhs.a;
    }

    constexpr friend bool operator ==(const NonTrivial& lhs, const NonTrivial& rhs)
    {
      return lhs.a == rhs.a;
    }

    int a;
  };
#include "etl/private/diagnostic_pop.h"

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

      etl::optional<Data> opt{ data };

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
    TEST(test_emplace_zero_parameters)
    {
      etl::optional<std::uint8_t> result = 1;
      result.emplace();

      CHECK_TRUE(result.has_value());
      CHECK_EQUAL(0, int(result.value()));
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
    TEST(test_nullopt_pod)
    {
      etl::optional<int> data(etl::nullopt);
      data = 1;
      data = etl::nullopt;
      CHECK(!bool(data));
    }

    //*************************************************************************
    TEST(test_nullopt)
    {
      etl::optional<Data> data(etl::nullopt);
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
    struct github_bug_720_bug_helper
    {
      int value{ 5 };

      etl::optional<int> get_valid() const
      {
        return value;
      }

      etl::optional<int> get_invalid() const
      {
        return etl::optional<int>();
      }
    };

    TEST(test_chained_value_or_github_bug_720 )
    {
      github_bug_720_bug_helper helper {};

      int value1 = helper.get_valid().value_or(1);
      CHECK_EQUAL(5, value1);

      int value2 = helper.get_invalid().value_or(1);
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

      CHECK_THROW(data1.value(), etl::optional_invalid);
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
      etl::optional<MyPODObject> optionalObject; // The Test: Does this compile for an object with a deleted default constructor?

      // Make sure it isn't optimised away.
      CHECK_FALSE(optionalObject.has_value());
    }

    //*************************************************************************
    TEST(test_optional_pod_assign_bug_714)
    {
      etl::optional<int> opt = 42;
      opt = etl::nullopt;

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

      etl::optional<Object> opt = Object{ 42 };

      CHECK_EQUAL(42, opt->value);
    }

    //*************************************************************************   
    TEST(test_const_arrow_operator_bug_730)
    {
      struct Object
      {
        int value;
      };

      const etl::optional<Object> opt = Object{ 42 };

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

      constexpr NonPod data(42);
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
  };
}


