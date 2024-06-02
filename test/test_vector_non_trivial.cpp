///******************************************************************************
//The MIT License(MIT)
//
//Embedded Template Library.
//https://github.com/ETLCPP/etl
//https://www.etlcpp.com
//
//Copyright(c) 2014 John Wellbelove
//
//Permission is hereby granted, free of charge, to any person obtaining a copy
//of this software and associated documentation files(the "Software"), to deal
//in the Software without restriction, including without limitation the rights
//to use, copy, modify, merge, publish, distribute, sublicense, and / or sell
//copies of the Software, and to permit persons to whom the Software is
//furnished to do so, subject to the following conditions :
//
//The above copyright notice and this permission notice shall be included in all
//copies or substantial portions of the Software.
//
//THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
//AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
//OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
//SOFTWARE.
//******************************************************************************/

#include "unit_test_framework.h"

#include <vector>
#include <array>
#include <algorithm>
#include <memory>

#include "etl/vector.h"
#include "etl/math.h"
#include "data.h"

namespace
{
  SUITE(test_vector_non_trivial)
  {
    static const size_t SIZE = 10;

    typedef TestDataNDC<std::string> NDC;
    typedef TestDataDC<std::string>  DC;

    static NDC ndc("NDC");

    typedef etl::vector<NDC, SIZE> DataNDC;
    typedef etl::ivector<NDC>      IDataNDC;
    typedef std::vector<NDC>       CompareDataNDC;

    typedef etl::vector<DC, SIZE> DataDC;
    typedef etl::ivector<DC>      IDataDC;
    typedef std::vector<DC>       CompareDataDC;

    CompareDataNDC initial_data;
    CompareDataNDC less_data;
    CompareDataNDC greater_data;
    CompareDataNDC shorter_data;
    CompareDataNDC different_data;
    CompareDataNDC insert_data;

    //*************************************************************************
    struct SetupFixture
    {
      SetupFixture()
      {
        NDC n[]         = { NDC("0"), NDC("1"), NDC("2"), NDC("3"), NDC("4"), NDC("5"), NDC("6"), NDC("7"), NDC("8"), NDC("9") };
        NDC n_insert[]  = { NDC("11"), NDC("12"), NDC("13") };
        NDC n_less[]    = { NDC("0"), NDC("1"), NDC("2"), NDC("3"), NDC("3"), NDC("5"), NDC("6"), NDC("7"), NDC("8"), NDC("9") };
        NDC n_greater[] = { NDC("0"), NDC("1"), NDC("2"), NDC("4"), NDC("4"), NDC("5"), NDC("6"), NDC("7"), NDC("8"), NDC("9") };

        initial_data.assign(std::begin(n), std::end(n));
        insert_data.assign(std::begin(n_insert), std::end(n_insert));
        less_data.assign(std::begin(n_less), std::end(n_less));
        greater_data.assign(std::begin(n_greater), std::end(n_greater));
        shorter_data.assign(std::begin(n_greater), std::end(n_greater) - 1);
        different_data.assign(initial_data.rbegin(), initial_data.rend());
      }
    };

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_default_constructor)
    {
      DataDC data;

      CHECK_EQUAL(data.size(), size_t(0));
      CHECK(data.empty());
      CHECK_EQUAL(data.capacity(), SIZE);
      CHECK_EQUAL(data.max_size(), SIZE);
    }

#if ETL_USING_CPP17 && ETL_HAS_INITIALIZER_LIST && !defined(ETL_TEMPLATE_DEDUCTION_GUIDE_TESTS_DISABLED)
    //*************************************************************************
    TEST(test_cpp17_deduced_constructor)
    {
      etl::vector data{ NDC("0"), NDC("1"), NDC("2"), NDC("3"), NDC("4"), NDC("5"), NDC("6"), NDC("7"), NDC("8"), NDC("9") };
      etl::vector<NDC, 10U> check = { NDC("0"), NDC("1"), NDC("2"), NDC("3"), NDC("4"), NDC("5"), NDC("6"), NDC("7"), NDC("8"), NDC("9") };

      CHECK(!data.empty());
      CHECK(data.full());
      CHECK(data.begin() != data.end());
      CHECK_EQUAL(0U, data.available());
      CHECK_EQUAL(10U, data.capacity());
      CHECK_EQUAL(10U, data.size());
      CHECK_EQUAL(10U, data.max_size());
      CHECK(data == check);
    }
#endif

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_destruct_via_ivector)
    {
      const size_t INITIAL_SIZE = 5;
      const NDC INITIAL_VALUE("1");

      int current_count = NDC::get_instance_count();

      DataNDC* p = new DataNDC(INITIAL_SIZE, INITIAL_VALUE);
      delete p;

      DataNDC* pdata = new DataNDC(INITIAL_SIZE, INITIAL_VALUE);
      CHECK_EQUAL(int(current_count + INITIAL_SIZE), NDC::get_instance_count());

      IDataNDC* pidata = pdata;
      delete pidata;
      CHECK_EQUAL(current_count, NDC::get_instance_count());
    }

    //*************************************************************************
    TEST(test_iterator_comparison_empty)
    {
      DataDC data;

      CHECK(data.begin()   == data.end());
      CHECK(data.cbegin()  == data.cend());
      CHECK(data.rbegin()  == data.rend());
      CHECK(data.crbegin() == data.crend());
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_constructor_size)
    {
      const size_t INITIAL_SIZE = 5;
      DataDC data(INITIAL_SIZE);

      CHECK(data.size() == INITIAL_SIZE);
      CHECK(!data.empty());
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_constructor_size_value)
    {
      const size_t INITIAL_SIZE = 5;
      const NDC INITIAL_VALUE("1");

      std::vector<NDC> compare_data(INITIAL_SIZE, INITIAL_VALUE);
      DataNDC data(size_t(5), NDC("1"));

      CHECK(data.size() == INITIAL_SIZE);
      CHECK(!data.empty());

      bool is_equal = std::equal(data.begin(),
                                 data.end(),
                                 compare_data.begin());

      CHECK(is_equal);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_constructor_size_excess)
    {
      CHECK_THROW(DataDC data(SIZE + 1), etl::vector_full);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_constructor_range)
    {
      CompareDataNDC compare_data(initial_data.begin(), initial_data.end());

      DataNDC data(compare_data.begin(), compare_data.end());

      CHECK(data.size() == SIZE);
      CHECK(!data.empty());
    }

#if ETL_HAS_INITIALIZER_LIST
    //*************************************************************************
    TEST(test_constructor_initializer_list)
    {
      CompareDataNDC compare_data = { NDC("0"), NDC("1"), NDC("2"), NDC("3") };
      DataNDC data = { NDC("0"), NDC("1"), NDC("2"), NDC("3") };

      CHECK_EQUAL(compare_data.size(), data.size());
      CHECK(std::equal(compare_data.begin(), compare_data.end(), data.begin()));
    }
#endif

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_copy_constructor)
    {
      DataNDC data(initial_data.begin(), initial_data.end());
      DataNDC data2(data);
      CHECK(data2 == data);

      data2[2] = NDC("X");
      CHECK(data2 != data);
    }

    //*************************************************************************
    TEST(test_move_constructor)
    {
      typedef etl::vector<std::unique_ptr<uint32_t>, SIZE> Data;

      std::unique_ptr<uint32_t> p1(new uint32_t(1U));
      std::unique_ptr<uint32_t> p2(new uint32_t(2U));
      std::unique_ptr<uint32_t> p3(new uint32_t(3U));
      std::unique_ptr<uint32_t> p4(new uint32_t(4U));

      Data data1;
      data1.push_back(std::move(p1));
      data1.push_back(std::move(p2));
      data1.push_back(std::move(p3));
      data1.push_back(std::move(p4));

      Data data2(std::move(data1));

      CHECK_EQUAL(0U, data1.size());
      CHECK_EQUAL(4U, data2.size());

      CHECK_EQUAL(1U, *data2[0]);
      CHECK_EQUAL(2U, *data2[1]);
      CHECK_EQUAL(3U, *data2[2]);
      CHECK_EQUAL(4U, *data2[3]);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_assignment)
    {
      DataNDC data(initial_data.begin(), initial_data.end());
      DataNDC other_data;

      other_data = data;

      bool is_equal = std::equal(data.begin(),
                                 data.end(),
                                 other_data.begin());

      CHECK(is_equal);
    }

    //*************************************************************************
    TEST(test_move_assignment)
    {
      typedef etl::vector<std::unique_ptr<uint32_t>, SIZE> Data;

      std::unique_ptr<uint32_t> p1(new uint32_t(1U));
      std::unique_ptr<uint32_t> p2(new uint32_t(2U));
      std::unique_ptr<uint32_t> p3(new uint32_t(3U));
      std::unique_ptr<uint32_t> p4(new uint32_t(4U));

      Data data1;
      data1.push_back(std::move(p1));
      data1.push_back(std::move(p2));
      data1.push_back(std::move(p3));
      data1.push_back(std::move(p4));

      Data data2;
      data2 = std::move(data1);

      CHECK_EQUAL(0U, data1.size());
      CHECK_EQUAL(4U, data2.size());

      CHECK_EQUAL(1U, *data2[0]);
      CHECK_EQUAL(2U, *data2[1]);
      CHECK_EQUAL(3U, *data2[2]);
      CHECK_EQUAL(4U, *data2[3]);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_assignment_iterface)
    {
      DataNDC data1(initial_data.begin(), initial_data.end());
      DataNDC data2;

      IDataNDC& idata1 = data1;
      IDataNDC& idata2 = data2;

      idata2 = idata1;

      bool is_equal = std::equal(data1.begin(),
                                 data1.end(),
                                 data2.begin());

      CHECK(is_equal);
    }

    //*************************************************************************
    TEST(test_move_assignment_interface)
    {
      typedef etl::vector<std::unique_ptr<uint32_t>, SIZE> Data;
      typedef etl::ivector<std::unique_ptr<uint32_t>> IData;

      std::unique_ptr<uint32_t> p1(new uint32_t(1U));
      std::unique_ptr<uint32_t> p2(new uint32_t(2U));
      std::unique_ptr<uint32_t> p3(new uint32_t(3U));
      std::unique_ptr<uint32_t> p4(new uint32_t(4U));

      Data data1;
      data1.push_back(std::move(p1));
      data1.push_back(std::move(p2));
      data1.push_back(std::move(p3));
      data1.push_back(std::move(p4));

      Data data2;

      IData& idata1 = data1;
      IData& idata2 = data2;

      idata2 = std::move(idata1);

      CHECK_EQUAL(0U, data1.size());
      CHECK_EQUAL(4U, data2.size());

      CHECK_EQUAL(1U, *data2[0]);
      CHECK_EQUAL(2U, *data2[1]);
      CHECK_EQUAL(3U, *data2[2]);
      CHECK_EQUAL(4U, *data2[3]);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_self_assignment)
    {
      DataNDC data(initial_data.begin(), initial_data.end());
      DataNDC other_data(data);

#include "etl/private/diagnostic_self_assign_overloaded_push.h" 
      other_data = other_data;
#include "etl/private/diagnostic_pop.h" 

      bool is_equal = std::equal(data.begin(),
                                 data.end(),
                                 other_data.begin());

      CHECK(is_equal);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_begin)
    {
      DataDC data(10);
      const DataDC constData(10);

      CHECK_EQUAL(&data[0], data.begin());
      CHECK_EQUAL(&constData[0], constData.begin());
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_end)
    {
      DataDC data(10);
      const DataDC constData(10);

      CHECK_EQUAL(&data[10], data.end());
      CHECK_EQUAL(&constData[10], constData.end());
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_resize_up)
    {
      const size_t INITIAL_SIZE = 5;
      const size_t NEW_SIZE = 8;

      DataDC data(INITIAL_SIZE);
      data.resize(NEW_SIZE);

      CHECK_EQUAL(data.size(), NEW_SIZE);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_resize_up_value)
    {
      const size_t INITIAL_SIZE = 5;
      const size_t NEW_SIZE = 8;
      const NDC INITIAL_VALUE("1");

      DataNDC data(INITIAL_SIZE, INITIAL_VALUE);
      data.resize(NEW_SIZE, INITIAL_VALUE);

      std::vector<NDC> compare_data(NEW_SIZE, INITIAL_VALUE);

      CHECK_EQUAL(compare_data.size(), data.size());

      bool is_equal = std::equal(data.begin(), data.end(), compare_data.begin());

      CHECK(is_equal);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_resize_excess)
    {
      const size_t INITIAL_SIZE = 5;
      const size_t NEW_SIZE = SIZE + 1;

      DataDC data(INITIAL_SIZE);

      CHECK_THROW(data.resize(NEW_SIZE), etl::vector_full);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_resize_down)
    {
      const size_t INITIAL_SIZE = 5;
      const size_t NEW_SIZE = 2;

      DataDC data(INITIAL_SIZE);
      data.resize(NEW_SIZE);

      CHECK_EQUAL(data.size(), NEW_SIZE);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_resize_down_value)
    {
      const size_t INITIAL_SIZE = 5;
      const size_t NEW_SIZE = 2;
      const NDC INITIAL_VALUE("1");

      DataNDC data(INITIAL_SIZE, INITIAL_VALUE);
      data.resize(NEW_SIZE, INITIAL_VALUE);

      CHECK_EQUAL(data.size(), NEW_SIZE);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_empty)
    {
      DataDC data;
      data.resize(data.max_size());

      CHECK(data.full());
      CHECK(!data.empty());
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_full)
    {
      DataNDC data;

      CHECK(!data.full());
      CHECK(data.empty());
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_index)
    {
      CompareDataNDC compare_data(initial_data.begin(), initial_data.end());

      DataNDC data(compare_data.begin(), compare_data.end());

      for (size_t i = 0UL; i < data.size(); ++i)
      {
        CHECK_EQUAL(data[i], compare_data[i]);
      }
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_index_const)
    {
      const CompareDataNDC compare_data(initial_data.begin(), initial_data.end());

      const DataNDC data(compare_data.begin(), compare_data.end());

      for (size_t i = 0UL; i < data.size(); ++i)
      {
        CHECK_EQUAL(data[i], compare_data[i]);
      }
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_at)
    {
      CompareDataNDC compare_data(initial_data.begin(), initial_data.end());
      DataNDC data(initial_data.begin(), initial_data.end());

      for (size_t i = 0UL; i < data.size(); ++i)
      {
        CHECK_EQUAL(data.at(i), compare_data.at(i));
      }

      CHECK_THROW(data.at(data.size()), etl::vector_out_of_bounds);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_at_const)
    {
      const CompareDataNDC compare_data(initial_data.begin(), initial_data.end());
      const DataNDC data(initial_data.begin(), initial_data.end());

      for (size_t i = 0UL; i < data.size(); ++i)
      {
        CHECK_EQUAL(data.at(i), compare_data.at(i));
      }

      CHECK_THROW(data.at(data.size()), etl::vector_out_of_bounds);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_front)
    {
      CompareDataNDC compare_data(initial_data.begin(), initial_data.end());
      DataNDC data(initial_data.begin(), initial_data.end());

      CHECK(data.front() == compare_data.front());
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_front_const)
    {
      const CompareDataNDC compare_data(initial_data.begin(), initial_data.end());
      const DataNDC data(initial_data.begin(), initial_data.end());

      CHECK(data.front() == compare_data.front());
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_back)
    {
      CompareDataNDC compare_data(initial_data.begin(), initial_data.end());
      DataNDC data(initial_data.begin(), initial_data.end());

      CHECK(data.back() == compare_data.back());
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_back_const)
    {
      const CompareDataNDC compare_data(initial_data.begin(), initial_data.end());
      const DataNDC data(initial_data.begin(), initial_data.end());

      CHECK(data.back() == compare_data.back());
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_data)
    {
      CompareDataNDC compare_data(initial_data.begin(), initial_data.end());

      DataNDC data(compare_data.begin(), compare_data.end());

      bool is_equal = std::equal(data.data(),
        data.data() + data.size(),
        compare_data.begin());

      CHECK(is_equal);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_data_const)
    {
      CompareDataNDC compare_data(initial_data.begin(), initial_data.end());

      const DataNDC data(compare_data.begin(), compare_data.end());

      bool is_equal = std::equal(data.data(),
                                data.data() + data.size(),
                                compare_data.begin());

      CHECK(is_equal);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_assign_range)
    {
      CompareDataNDC compare_data(initial_data.begin(), initial_data.end());

      DataNDC data;

      data.assign(compare_data.begin(), compare_data.end());

      CHECK_EQUAL(compare_data.size(), data.size());

      bool is_equal = std::equal(data.begin(),
                                data.end(),
                                compare_data.begin());

      CHECK(is_equal);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_assign_size_value)
    {
      const size_t INITIAL_SIZE = 5;
      const NDC INITIAL_VALUE("1");
      std::vector<NDC> compare_data(INITIAL_SIZE, INITIAL_VALUE);

      DataNDC data;
      data.assign(INITIAL_SIZE, INITIAL_VALUE);

      CHECK_EQUAL(compare_data.size(), data.size());

      bool is_equal = std::equal(data.begin(),
                                 data.end(),
                                 compare_data.begin());

      CHECK(is_equal);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_assign_size_value_excess)
    {
      const size_t INITIAL_SIZE = SIZE;
      const size_t EXCESS_SIZE = SIZE + 1;
      const NDC INITIAL_VALUE("1");
      std::vector<NDC> compare_data(INITIAL_SIZE, INITIAL_VALUE);

      DataNDC data;

      CHECK_THROW(data.assign(EXCESS_SIZE, INITIAL_VALUE), etl::vector_full);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_push_back)
    {
      CompareDataNDC compare_data;
      DataNDC data;

      for (size_t i = 0UL; i < SIZE; ++i)
      {
        std::string value(" ");
        value[0] = char('A' + i);
        compare_data.push_back(NDC(value));
        data.push_back(NDC(value));
      }

      CHECK_EQUAL(compare_data.size(), data.size());

      bool is_equal = std::equal(data.begin(),
                                 data.end(),
                                 compare_data.begin());

      CHECK(is_equal);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_push_back_excess)
    {
      DataNDC data;

      for (size_t i = 0UL; i < SIZE; ++i)
      {
        std::string value(" ");
        value[0] = char('A' + i);
        data.push_back(NDC(value));
      }

      CHECK_THROW(data.push_back(NDC("Z")), etl::vector_full);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_push_back_unique_ptr)
    {
      etl::vector<std::unique_ptr<int>, SIZE> data;

      std::unique_ptr<int> p1(new int(1));
      std::unique_ptr<int> p2(new int(2));
      std::unique_ptr<int> p3(new int(3));
      std::unique_ptr<int> p4(new int(4));

      data.push_back(std::move(p1));
      data.push_back(std::move(p2));
      data.push_back(std::move(p3));
      data.push_back(std::move(p4));

      CHECK(!bool(p1));
      CHECK(!bool(p2));
      CHECK(!bool(p3));
      CHECK(!bool(p4));

      CHECK_EQUAL(1, *data[0]);
      CHECK_EQUAL(2, *data[1]);
      CHECK_EQUAL(3, *data[2]);
      CHECK_EQUAL(4, *data[3]);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_emplace_back)
    {
      CompareDataNDC compare_data;
      DataNDC data;

      for (size_t i = 0UL; i < SIZE; ++i)
      {
        std::string value(" ");
        value[0] = char('A' + i);
        compare_data.emplace_back(value, i);
        data.emplace_back(value, i);
      }

      CHECK_EQUAL(compare_data.size(), data.size());

      bool is_equal = std::equal(data.begin(),
        data.end(),
        compare_data.begin());

      CHECK(is_equal);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_emplace_back_return)
    {
      DataNDC data;

      data.emplace_back("A", 24);
      auto back = data.emplace_back("B", 42);
      CHECK_EQUAL(back, data.back());
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_insert_int)
    {
      etl::vector<int, SIZE> data;

      int p1(1);
      int p2(2);
      int p3(3);
      int p4(4);

      data.insert(data.begin(), p1);
      data.insert(data.begin(), p2);
      data.insert(data.begin(), p3);
      data.insert(data.begin(), p4);

      CHECK_EQUAL(4, data[0]);
      CHECK_EQUAL(3, data[1]);
      CHECK_EQUAL(2, data[2]);
      CHECK_EQUAL(1, data[3]);
    }

#if (!(defined(ETL_COMPILER_GCC) && defined(ETL_USE_TYPE_TRAITS_BUILTINS)))
    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_insert_unique_ptr)
    {
      etl::vector<std::unique_ptr<int>, SIZE> data;

      std::unique_ptr<int> p1(new int(1));
      std::unique_ptr<int> p2(new int(2));
      std::unique_ptr<int> p3(new int(3));
      std::unique_ptr<int> p4(new int(4));

      data.insert(data.begin(), std::move(p1));
      data.insert(data.begin(), std::move(p2));
      data.insert(data.begin(), std::move(p3));
      data.insert(data.begin(), std::move(p4));

      CHECK(!bool(p1));
      CHECK(!bool(p2));
      CHECK(!bool(p3));
      CHECK(!bool(p4));

      CHECK_EQUAL(1, *data[3]);
      CHECK_EQUAL(4, *data[0]);
      CHECK_EQUAL(3, *data[1]);
      CHECK_EQUAL(2, *data[2]);
    }
#endif

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_emplace_back_multiple)
    {
      class Data
      {
      public:
        std::string a;
        size_t b;
        double c;
        const char *d;
        Data(std::string w) : a(w), b(0), c(0.0), d(0){}
        Data(std::string w, size_t x) : a(w), b(x), c(0.0), d(0){}
        Data(std::string w, size_t x, double y) : a(w), b(x), c(y), d(0){}
        Data(std::string w, size_t x, double y, const char *z) : a(w), b(x), c(y), d(z){}
        bool operator == (const Data &other) const
        {
#include "etl/private/diagnostic_float_equal_push.h"
          return (a == other.a) &&
                 (b == other.b) &&
                 (c == other.c) &&
                 (((d == nullptr) && (other.d == nullptr)) || (strcmp(d, other.d) == 0));
#include "etl/private/diagnostic_pop.h"
        }
      };

      std::vector<Data> compare_data;
      etl::vector<Data, SIZE * 4> data;

      std::string s;
      for (size_t i = 0UL; i < SIZE; ++i)
      {
        s += "x";

        // 4 arguments
        compare_data.emplace_back(s, i, static_cast<double>(i) + 0.1234, "emplace_back");
        data.emplace_back(s, i, static_cast<double>(i) + 0.1234, "emplace_back");

        // 3 arguments
        compare_data.emplace_back(s, i, static_cast<double>(i) + 0.1234);
        data.emplace_back(s, i, static_cast<double>(i) + 0.1234);

        // 2 arguments
        compare_data.emplace_back(s, i);
        data.emplace_back(s, i);

        // 1 argument
        compare_data.emplace_back(s);
        data.emplace_back(s);
      }

      CHECK_EQUAL(compare_data.size(), data.size());

      const bool is_equal = std::equal(data.begin(),
                                       data.end(),
                                       compare_data.begin());

      CHECK(is_equal);
    }

    //*************************************************************************
    // The C++11 variadic version uses non-const rvalue references so has the ability
    // to emplace non-const reference members, the pre-C++11 const reference overloads
    // does not have the ability to pass const reference parameters to non-const
    // constructor parameters (like the members in Data below)
    // So this is only tested on C++11 onwards
    TEST_FIXTURE(SetupFixture, test_emplace_back_non_const_references)
    {
#if ETL_USING_CPP11 && ETL_NOT_USING_STLPORT && !defined(ETL_VECTOR_FORCE_CPP03_IMPLEMENTATION)
      class Data
      {
      public:
        std::string &a;
        size_t &b;
        double &c;
        const char *d;
        Data(std::string &w, size_t &x, double &y, const char *z) : a(w), b(x), c(y), d(z){}
        bool operator == (const Data &other) const
        {
          return (a == other.a) && (b == other.b) && !(c < other.c) && !(c > other.c) && (d == other.d);
        }
      };

      std::vector<Data> compare_data;
      etl::vector<Data, SIZE * 3> data;

      std::string a = "test_test_test";
      size_t b = 9999;
      double c = 123.456;
      const char *d = "abcdefghijklmnopqrstuvwxyz";
      for (size_t i = 0UL; i < SIZE; ++i)
      {
        data.emplace_back(a, b, c, d);
        compare_data.emplace_back(a, b, c, d);
      }

      CHECK_EQUAL(compare_data.size(), data.size());

      const bool is_equal = std::equal(data.begin(),
                                       data.end(),
                                       compare_data.begin());

      CHECK(is_equal);
#endif
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_pop_back)
    {
      CompareDataNDC compare_data(initial_data.begin(), initial_data.end());
      DataNDC data(initial_data.begin(), initial_data.end());

      compare_data.pop_back();
      compare_data.pop_back();

      data.pop_back();
      data.pop_back();

      CHECK_EQUAL(compare_data.size(), data.size());

      bool is_equal = std::equal(data.begin(),
                                 data.end(),
                                 compare_data.begin());

      CHECK(is_equal);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_pop_back_exception)
    {
      DataDC data;

      data.resize(2);

      data.pop_back();
      data.pop_back();

      CHECK_THROW(data.pop_back(), etl::vector_empty);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_insert_position_value)
    {
      const size_t INITIAL_SIZE = 5;
      const NDC INITIAL_VALUE("1");

      for (size_t offset = 0; offset <= INITIAL_SIZE; ++offset)
      {
        CompareDataNDC compare_data;
        DataNDC data;

        data.assign(initial_data.begin(), initial_data.begin() + INITIAL_SIZE);
        compare_data.assign(initial_data.begin(), initial_data.begin() + INITIAL_SIZE);

        data.insert(data.begin() + offset, INITIAL_VALUE);
        compare_data.insert(compare_data.begin() + offset, INITIAL_VALUE);

        CHECK_EQUAL(compare_data.size(), data.size());

        bool is_equal = std::equal(data.begin(),
                                   data.end(),
                                   compare_data.begin());

        CHECK(is_equal);
      }
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_emplace_position_value)
    {
      const size_t INITIAL_SIZE = 5;
      const std::string INITIAL_VALUE("1");

      for (size_t offset = 0; offset <= INITIAL_SIZE; ++offset)
      {
        CompareDataNDC compare_data;
        DataNDC data;

        data.assign(initial_data.begin(), initial_data.begin() + INITIAL_SIZE);
        compare_data.assign(initial_data.begin(), initial_data.begin() + INITIAL_SIZE);

        data.emplace(data.begin() + offset, INITIAL_VALUE, offset);
        compare_data.emplace(compare_data.begin() + offset, INITIAL_VALUE, offset);

        CHECK_EQUAL(compare_data.size(), data.size());

        bool is_equal = std::equal(data.begin(),
                                   data.end(),
                                   compare_data.begin());

        CHECK(is_equal);
      }
    }

    //*************************************************************************
#include "etl/private/diagnostic_array_bounds_push.h"
    TEST_FIXTURE(SetupFixture, test_insert_position_value_excess)
    {
      const size_t INITIAL_SIZE     = SIZE;
      const NDC INITIAL_VALUE("1");
      const NDC UNINITIALISED_VALUE("Z");

      DataNDC data(INITIAL_SIZE, INITIAL_VALUE);

      size_t offset = 2;

      CHECK_THROW(data.insert(data.begin() + offset, INITIAL_VALUE), etl::vector_full);

      offset = 0;

      CHECK_THROW(data.insert(data.begin() + offset, INITIAL_VALUE), etl::vector_full);

      offset = data.size();

      CHECK_THROW(data.insert(data.begin() + offset, INITIAL_VALUE), etl::vector_full);
    }
#include "etl/private/diagnostic_pop.h"

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_insert_position_n_value)
    {
      const size_t INITIAL_SIZE     = 5;
      const size_t INSERT_SIZE      = 3;
      const NDC INITIAL_VALUE("1");

      for (size_t offset = 0; offset <= INITIAL_SIZE; ++offset)
      {
        CompareDataNDC compare_data;
        DataNDC data;

        data.assign(initial_data.begin(), initial_data.begin() + INITIAL_SIZE);
        compare_data.assign(initial_data.begin(), initial_data.begin() + INITIAL_SIZE);
        data.insert(data.begin() + offset, INSERT_SIZE, INITIAL_VALUE);
        compare_data.insert(compare_data.begin() + offset, INSERT_SIZE, INITIAL_VALUE);

        CHECK_EQUAL(compare_data.size(), data.size());

        bool is_equal = std::equal(data.begin(),
                                   data.end(),
                                   compare_data.begin());

        CHECK(is_equal);
      }
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_insert_position_n_value_excess)
    {
      const size_t INITIAL_SIZE = SIZE;
      const size_t INSERT_SIZE  = 4;
      const NDC INITIAL_VALUE("1");

      DataNDC data(INITIAL_SIZE, INITIAL_VALUE);

      size_t offset = 0;

      CHECK_THROW(data.insert(data.begin() + offset, INSERT_SIZE, INITIAL_VALUE), etl::vector_full);

      offset = 2;

      CHECK_THROW(data.insert(data.begin() + offset, INSERT_SIZE, INITIAL_VALUE), etl::vector_full);

      offset = 4;

      CHECK_THROW(data.insert(data.begin() + offset, INSERT_SIZE, INITIAL_VALUE), etl::vector_full);

      offset = data.size();

      CHECK_THROW(data.insert(data.begin() + offset, INSERT_SIZE, INITIAL_VALUE), etl::vector_full);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_insert_position_range)
    {
      const size_t INITIAL_SIZE = 5;
      const NDC INITIAL_VALUE("1");

      for (size_t offset = 0; offset <= INITIAL_SIZE; ++offset)
      {
        CompareDataNDC compare_data;
        DataNDC data;

        data.resize(SIZE, NDC("Z"));

        data.assign(initial_data.begin(), initial_data.begin() + INITIAL_SIZE);
        compare_data.assign(initial_data.begin(), initial_data.begin() + INITIAL_SIZE);
        data.insert(data.begin() + offset, insert_data.begin(), insert_data.end());
        compare_data.insert(compare_data.begin() + offset, insert_data.begin(), insert_data.end());

        CHECK_EQUAL(compare_data.size(), data.size());

        bool is_equal = std::equal(data.begin(),
                                   data.end(),
                                   compare_data.begin());

        CHECK(is_equal);
      }
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_insert_position_range_excess)
    {
      const size_t INITIAL_SIZE = 5;
      const NDC INITIAL_VALUE("1");

      DataNDC data(INITIAL_SIZE, INITIAL_VALUE);

      size_t offset = 0;

      CHECK_THROW(data.insert(data.begin() + offset, initial_data.begin(), initial_data.end()), etl::vector_full);

      offset = 2;

      CHECK_THROW(data.insert(data.begin() + offset, initial_data.begin(), initial_data.end()), etl::vector_full);

      offset = 4;

      CHECK_THROW(data.insert(data.begin() + offset, initial_data.begin(), initial_data.end()), etl::vector_full);

      offset = data.size();

      CHECK_THROW(data.insert(data.begin() + offset, initial_data.begin(), initial_data.end()), etl::vector_full);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_erase_single)
    {
      CompareDataNDC compare_data(initial_data.begin(), initial_data.end());
      DataNDC data(initial_data.begin(), initial_data.end());

      compare_data.erase(compare_data.begin() + 2);

      data.erase(data.begin() + 2);

      CHECK_EQUAL(compare_data.size(), data.size());

      bool is_equal = std::equal(data.begin(),
                                data.end(),
                                compare_data.begin());

      CHECK(is_equal);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_erase_range)
    {
      CompareDataNDC compare_data(initial_data.begin(), initial_data.end());
      DataNDC data(initial_data.begin(), initial_data.end());

      compare_data.erase(compare_data.begin() + 2, compare_data.begin() + 4);

      data.erase(data.begin() + 2, data.begin() + 4);

      CHECK_EQUAL(compare_data.size(), data.size());

      bool is_equal = std::equal(data.begin(),
                                data.end(),
                                compare_data.begin());

      CHECK(is_equal);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_clear)
    {
      CompareDataNDC compare_data(initial_data.begin(), initial_data.end());

      DataNDC data(compare_data.begin(), compare_data.end());
      data.clear();

      CHECK_EQUAL(data.size(), size_t(0));
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_iterator)
    {
      CompareDataNDC compare_data(initial_data.begin(), initial_data.end());

      DataNDC data(compare_data.begin(), compare_data.end());

      bool is_equal = std::equal(data.begin(),
                                 data.end(),
                                 compare_data.begin());

      CHECK(is_equal);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_const_iterator)
    {
      CompareDataNDC compare_data(initial_data.begin(), initial_data.end());

      DataNDC data(compare_data.begin(), compare_data.end());

      bool is_equal = std::equal(data.cbegin(),
                                 data.cend(),
                                 compare_data.cbegin());

      CHECK(is_equal);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_reverse_iterator)
    {
      CompareDataNDC compare_data(initial_data.begin(), initial_data.end());

      DataNDC data(compare_data.begin(), compare_data.end());

      bool is_equal = std::equal(data.rbegin(),
                                 data.rend(),
                                 compare_data.rbegin());

      CHECK(is_equal);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_const_reverse_iterator)
    {
      const CompareDataNDC compare_data(initial_data.begin(), initial_data.end());

      const DataNDC data(compare_data.begin(), compare_data.end());

      bool is_equal = std::equal(data.crbegin(),
                                 data.crend(),
                                 compare_data.crbegin());

      CHECK(is_equal);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_const_reverse_iterator2)
    {
      const CompareDataNDC compare_data(initial_data.begin(), initial_data.end());

      const DataNDC data(compare_data.begin(), compare_data.end());

      bool is_equal = std::equal(data.rbegin(),
                                 data.rend(),
                                 compare_data.rbegin());

      CHECK(is_equal);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_equal)
    {
      const DataNDC initial1(initial_data.begin(), initial_data.end());
      const DataNDC initial2(initial_data.begin(), initial_data.end());

      CHECK(initial1 == initial2);

      const DataNDC different(different_data.begin(), different_data.end());

      CHECK(!(initial1 == different));

      const DataNDC shorter(shorter_data.begin(), shorter_data.end());

      CHECK(!(shorter == initial1));
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_not_equal)
    {
      const DataNDC initial1(initial_data.begin(), initial_data.end());
      const DataNDC initial2(initial_data.begin(), initial_data.end());

      CHECK(!(initial1 != initial2));

      const DataNDC different(different_data.begin(), different_data.end());

      CHECK(initial1 != different);

      const DataNDC shorter(shorter_data.begin(), shorter_data.end());

      CHECK(shorter != initial1);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_less_than)
    {
      const DataNDC less(less_data.begin(), less_data.end());
      const DataNDC initial(initial_data.begin(), initial_data.end());

      CHECK((less < initial) == (less_data < initial_data));

      const DataNDC greater(greater_data.begin(), greater_data.end());

      CHECK((greater < initial) == (greater_data < initial_data));

      const DataNDC shorter(shorter_data.begin(), shorter_data.end());

      CHECK((shorter < initial) == (shorter_data < initial_data));
      CHECK((initial < shorter) == (initial_data < shorter_data));
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_less_than_or_equal)
    {
      const DataNDC less(less_data.begin(), less_data.end());
      const DataNDC initial(initial_data.begin(), initial_data.end());

      CHECK((less <= initial) == (less_data <= initial_data));

      const DataNDC greater(greater_data.begin(), greater_data.end());

      CHECK((greater <= initial) == (greater_data <= initial_data));

      const DataNDC shorter(shorter_data.begin(), shorter_data.end());

      CHECK((shorter <= initial) == (shorter_data <= initial_data));
      CHECK((initial <= shorter) == (initial_data <= shorter_data));

      const DataNDC initial2(initial_data.begin(), initial_data.end());
      CHECK((initial <= initial2) == (initial_data <= initial_data));
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_greater_than)
    {
      const DataNDC less(less_data.begin(), less_data.end());
      const DataNDC initial(initial_data.begin(), initial_data.end());

      CHECK((less > initial) == (less_data > initial_data));

      const DataNDC greater(greater_data.begin(), greater_data.end());

      CHECK((greater > initial) == (greater_data > initial_data));

      const DataNDC shorter(shorter_data.begin(), shorter_data.end());

      CHECK((shorter > initial) == (shorter_data > initial_data));
      CHECK((initial > shorter) == (initial_data > shorter_data));
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_greater_than_or_equal)
    {
      const DataNDC less(less_data.begin(), less_data.end());
      const DataNDC initial(initial_data.begin(), initial_data.end());

      CHECK((less >= initial) == (less_data >= initial_data));

      const DataNDC greater(greater_data.begin(), greater_data.end());

      CHECK((greater >= initial) == (greater_data >= initial_data));

      const DataNDC shorter(shorter_data.begin(), shorter_data.end());

      CHECK((shorter >= initial) == (shorter_data >= initial_data));
      CHECK((initial >= shorter) == (initial_data > shorter_data));

      const DataNDC initial2(initial_data.begin(), initial_data.end());
      CHECK((initial >= initial2) == (initial_data >= initial_data));
    }
  };
}
