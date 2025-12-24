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

#include <map>
#include <array>
#include <algorithm>
#include <utility>
#include <iterator>
#include <string>
#include <vector>

#include "data.h"

#include "etl/flat_multimap.h"

namespace
{
  static const size_t SIZE = 10UL;

  typedef TestDataDC<std::string>  DC;
  typedef TestDataNDC<std::string> NDC;
  typedef TestDataM<std::string>   MC;

  typedef ETL_OR_STD::pair<int, DC>  ElementDC;
  typedef ETL_OR_STD::pair<int, NDC> ElementNDC;

  typedef ETL_OR_STD::pair<int, int>  ElementInt;

  typedef etl::flat_multimap<int, DC, SIZE>  DataDC;
  typedef etl::flat_multimap<int, NDC, SIZE> DataNDC;
  typedef etl::iflat_multimap<int, DC>       IDataDC;
  typedef etl::iflat_multimap<int, NDC>      IDataNDC;

  typedef etl::flat_multimap<int, int, SIZE>  DataInt;

  typedef etl::flat_multimap<int, MC, SIZE> DataM;
  typedef etl::iflat_multimap<int, MC>      IDataM;

  typedef std::multimap<int, DC>  Compare_DataDC;
  typedef std::multimap<int, NDC> Compare_DataNDC;

  NDC N0 = NDC("A");
  NDC N1 = NDC("B");
  NDC N2 = NDC("C");
  NDC N3 = NDC("D");
  NDC N4 = NDC("E");
  NDC N5 = NDC("F");
  NDC N6 = NDC("G");
  NDC N7 = NDC("H");
  NDC N8 = NDC("I");
  NDC N9 = NDC("J");
  NDC N10 = NDC("K");
  NDC N11 = NDC("L");
  NDC N12 = NDC("M");
  NDC N13 = NDC("N");
  NDC N14 = NDC("O");
  NDC N15 = NDC("P");
  NDC N16 = NDC("Q");
  NDC N17 = NDC("R");
  NDC N18 = NDC("S");
  NDC N19 = NDC("T");

  std::vector<ElementNDC> initial_data;
  std::vector<ElementNDC> excess_data;
  std::vector<ElementNDC> different_data;
  std::vector<ElementNDC> multi_data;

  std::vector<ElementInt> int_data;

  struct D1
  {
    D1(const std::string& a_)
      : a(a_)
    {
    }

    std::string a;
  };

  struct D2
  {
    D2(const std::string& a_, const std::string& b_)
      : a(a_),
        b(b_)
    {
    }

    std::string a;
    std::string b;
  };

  struct D3
  {
    D3(const std::string& a_, const std::string& b_, const std::string& c_)
      : a(a_),
        b(b_),
        c(c_)
    {
    }

    std::string a;
    std::string b;
    std::string c;
  };

  struct D4
  {
    D4(const std::string& a_, const std::string& b_, const std::string& c_, const std::string& d_)
      : a(a_),
        b(b_),
        c(c_),
        d(d_)
    {
    }

    std::string a;
    std::string b;
    std::string c;
    std::string d;
  };

  bool operator == (const D1& lhs, const D1& rhs)
  {
    return (lhs.a == rhs.a);
  }

  bool operator == (const D2& lhs, const D2& rhs)
  {
    return (lhs.a == rhs.a) && (lhs.b == rhs.b);
  }

  bool operator == (const D3& lhs, const D3& rhs)
  {
    return (lhs.a == rhs.a) && (lhs.b == rhs.b) && (lhs.c == rhs.c);
  }

  bool operator == (const D4& lhs, const D4& rhs)
  {
    return (lhs.a == rhs.a) && (lhs.b == rhs.b) && (lhs.c == rhs.c) && (lhs.d == rhs.d);
  }

  bool operator != (const D1& lhs, const D1& rhs)
  {
    return !(lhs == rhs);
  }

  bool operator != (const D2& lhs, const D2& rhs)
  {
    return !(lhs == rhs);
  }

  bool operator != (const D3& lhs, const D3& rhs)
  {
    return !(lhs == rhs);
  }

  bool operator != (const D4& lhs, const D4& rhs)
  {
    return !(lhs == rhs);
  }

  typedef ETL_OR_STD::pair<const int, D1> Element1;
  typedef ETL_OR_STD::pair<const int, D2> Element2;
  typedef ETL_OR_STD::pair<const int, D3> Element3;
  typedef ETL_OR_STD::pair<const int, D4> Element4;

  typedef etl::flat_multimap<int, D1, SIZE> Data1;
  typedef etl::flat_multimap<int, D2, SIZE> Data2;
  typedef etl::flat_multimap<int, D3, SIZE> Data3;
  typedef etl::flat_multimap<int, D4, SIZE> Data4;

  typedef std::multimap<int, D1> Compare1;
  typedef std::multimap<int, D2> Compare2;
  typedef std::multimap<int, D3> Compare3;
  typedef std::multimap<int, D4> Compare4;

  struct Key
  {
    Key(int k_)
      : k(k_)
    {
    }

    int k;
  };

  bool operator <(const Key& lhs, const int& rhs)
  {
    return (lhs.k < rhs);
  }

  bool operator <(const int& lhs, const Key& rhs)
  {
    return (lhs < rhs.k);
  }

  //*************************************************************************
  template <typename T1, typename T2>
  bool Check_Equal(T1 begin1, T1 end1, T2 begin2)
  {
    while (begin1 != end1)
    {
      if ((begin1->first != begin2->first) || (begin1->second != begin2->second))
      {
        return false;
      }

      ++begin1;
      ++begin2;
    }

    return true;
  }

  SUITE(test_flat_multimap)
  {
    //*************************************************************************
    struct SetupFixture
    {
      SetupFixture()
      {
        ElementNDC n[] =
        {
          ElementNDC(0, N0), ElementNDC(1, N1), ElementNDC(2, N2), ElementNDC(3, N3), ElementNDC(4, N4),
          ElementNDC(5, N5), ElementNDC(6, N6), ElementNDC(7, N7), ElementNDC(8, N8), ElementNDC(9, N9)
        };

        ElementNDC n2[] =
        {
          ElementNDC(0, N0), ElementNDC(1, N1), ElementNDC(2, N2), ElementNDC(3, N3), ElementNDC(4, N4),
          ElementNDC(5, N5), ElementNDC(6, N6), ElementNDC(7, N7), ElementNDC(8, N8), ElementNDC(9, N9),
          ElementNDC(10, N10)
        };

        ElementNDC n3[] =
        {
          ElementNDC(10, N10), ElementNDC(11, N11), ElementNDC(12, N12), ElementNDC(13, N13), ElementNDC(14, N14),
          ElementNDC(15, N15), ElementNDC(16, N16), ElementNDC(17, N17), ElementNDC(18, N18), ElementNDC(19, N19)
        };

        ElementNDC n4[] =
        {
          ElementNDC(0, N0), ElementNDC(1, N1), ElementNDC(2, N2), ElementNDC(1, N3), ElementNDC(3, N4),
          ElementNDC(4, N5), ElementNDC(4, N6), ElementNDC(5, N7), ElementNDC(4, N8), ElementNDC(0, N9)
        };

        ElementInt n5[] =
        {
          ElementInt(0, 0), ElementInt(1, 1), ElementInt(2, 2), ElementInt(3, 3), ElementInt(4, 4),
          ElementInt(5, 5), ElementInt(6, 6), ElementInt(7, 7), ElementInt(8, 8), ElementInt(9, 9)
        };

        initial_data.assign(std::begin(n), std::end(n));
        excess_data.assign(std::begin(n2), std::end(n2));
        different_data.assign(std::begin(n3), std::end(n3));
        multi_data.assign(std::begin(n4), std::end(n4));
        int_data.assign(std::begin(n5), std::end(n5));
      }
    };

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_default_constructor)
    {
      DataDC data;

      CHECK(data.size() == 0UL);
      CHECK(data.empty());
      CHECK(data.capacity() == SIZE);
      CHECK(data.max_size() == SIZE);
      CHECK(data.begin() == data.end());
    }

#if ETL_USING_CPP17 && ETL_HAS_INITIALIZER_LIST && !defined(ETL_TEMPLATE_DEDUCTION_GUIDE_TESTS_DISABLED)
    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_cpp17_deduced_constructor)
    {
      etl::flat_multimap data{ ElementNDC(0, N0), ElementNDC(1, N1), ElementNDC(2, N2), ElementNDC(3, N3), ElementNDC(4, N4),
                               ElementNDC(5, N5), ElementNDC(6, N6), ElementNDC(7, N7), ElementNDC(8, N8), ElementNDC(9, N9) };
      etl::flat_multimap<int, NDC, 10U> check = { ElementNDC(0, N0), ElementNDC(1, N1), ElementNDC(2, N2), ElementNDC(3, N3), ElementNDC(4, N4),
                                                  ElementNDC(5, N5), ElementNDC(6, N6), ElementNDC(7, N7), ElementNDC(8, N8), ElementNDC(9, N9) };

      CHECK(!data.empty());
      CHECK(data.full());
      CHECK(data.begin() != data.end());
      CHECK_EQUAL(10U, data.size());
      CHECK_EQUAL(0U, data.available());
      CHECK_EQUAL(10U, data.capacity());
      CHECK_EQUAL(10U, data.max_size());
      CHECK(data == check);
    }
#endif

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_destruct_via_iflat_multimap)
    {
      int current_count = NDC::get_instance_count();

      DataNDC* pdata = new DataNDC(initial_data.begin(), initial_data.end());
      CHECK(int(current_count + initial_data.size()) == NDC::get_instance_count());

      IDataNDC* pidata = pdata;
      delete pidata;
      CHECK(current_count == NDC::get_instance_count());
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_constructor_range)
    {
      Compare_DataNDC compare_data(initial_data.begin(), initial_data.end());

      DataNDC data(compare_data.begin(), compare_data.end());

      CHECK(data.size() == SIZE);
      CHECK(!data.empty());


      bool isEqual = std::equal(data.begin(),
                                data.end(),
                                compare_data.begin());
      CHECK(isEqual);
    }

#if ETL_HAS_INITIALIZER_LIST
    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_constructor_initializer_list)
    {
      Compare_DataNDC compare_data = { ElementNDC(0, N0), ElementNDC(1, N1), ElementNDC(2, N2), ElementNDC(3, N3), ElementNDC(4, N4) };

      DataNDC data = { ElementNDC(0, N0), ElementNDC(1, N1), ElementNDC(2, N2), ElementNDC(3, N3), ElementNDC(4, N4) };

      CHECK(compare_data.size() == data.size());
      CHECK(!data.empty());

      bool isEqual = std::equal(data.begin(),
                                data.end(),
                                compare_data.begin());
      CHECK(isEqual);
    }
#endif

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_move_constructor)
    {
      using Item = ETL_OR_STD::pair<int, MC>;

      Item p1(1, MC("1"));
      Item p2a(2, "2a");
      Item p2b(2, "2b");
      Item p3(3, "3");
      Item p4(4, MC("4"));

      DataM data1;
      data1.insert(std::move(p1));
      data1.insert(std::move(p2a));
      data1.insert(std::move(p3));
      data1.insert(std::move(p4));
      data1.insert(std::move(p2b));

      CHECK(!bool(p1.second));
      CHECK(!bool(p2a.second));
      CHECK(!bool(p2b.second));
      CHECK(!bool(p3.second));
      CHECK(!bool(p4.second));

      DataM data2(std::move(data1));

      CHECK(5U == data1.size()); // Move does not clear the source.
      CHECK(5U == data2.size());

      DataM::const_iterator itr = data2.begin();

      CHECK("1" == (*itr++).second.value);
      CHECK("2a" == (*itr++).second.value);
      CHECK("2b" == (*itr++).second.value);
      CHECK("3" == (*itr++).second.value);
      CHECK("4" == (*itr++).second.value);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_assignment)
    {
      DataNDC data(initial_data.begin(), initial_data.end());
      DataNDC other_data;

      other_data = data;

      bool isEqual = Check_Equal(data.begin(),
                                 data.end(),
                                 other_data.begin());

      CHECK(isEqual);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_assignment_interface)
    {
      DataNDC data1(initial_data.begin(), initial_data.end());
      DataNDC data2;

      IDataNDC& idata1 = data1;
      IDataNDC& idata2 = data2;

      idata2 = idata1;

      bool isEqual = Check_Equal(data1.begin(),
                                 data1.end(),
                                 data2.begin());

      CHECK(isEqual);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_self_assignment)
    {
      DataNDC data(initial_data.begin(), initial_data.end());
      DataNDC other_data(data);

#include "etl/private/diagnostic_self_assign_overloaded_push.h" 
      other_data = other_data;
#include "etl/private/diagnostic_pop.h" 

      bool isEqual = Check_Equal(data.begin(),
                                 data.end(),
                                 other_data.begin());

      CHECK(isEqual);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_begin)
    {
      DataNDC data(initial_data.begin(), initial_data.end());
      const DataNDC constData(data);

      CHECK(data.begin() == std::begin(data));
      CHECK(constData.begin() == std::begin(constData));
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_end)
    {
      DataNDC data(initial_data.begin(), initial_data.end());
      const DataNDC constData(data);

      CHECK(data.end() == std::end(data));
      CHECK(constData.end() == std::end(constData));
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_empty)
    {
      DataNDC data;
      data.insert(initial_data.begin(), initial_data.end());

      CHECK(data.full());
      CHECK(!data.empty());
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_full)
    {
      DataDC data;

      CHECK(!data.full());
      CHECK(data.empty());
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_assign_range)
    {
      Compare_DataNDC compare_data(initial_data.begin(), initial_data.end());

      DataNDC data;

      data.assign(compare_data.begin(), compare_data.end());

      bool isEqual = Check_Equal(data.begin(),
                                 data.end(),
                                 compare_data.begin());

      CHECK(isEqual);

      CHECK(std::is_sorted(data.begin(), data.end()));
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_insert_value)
    {
      Compare_DataNDC compare_data;
      DataNDC data;

      data.insert(DataNDC::value_type(0, N0));
      compare_data.insert(ETL_OR_STD::make_pair(0, N0));

      bool isEqual = Check_Equal(data.begin(),
                                 data.end(),
                                 compare_data.begin());

      CHECK(isEqual);

      data.insert(ETL_OR_STD::make_pair(2, N2));
      compare_data.insert(ETL_OR_STD::make_pair(2, N2));

      isEqual = Check_Equal(data.begin(),
                            data.end(),
                            compare_data.begin());

      CHECK(isEqual);

      data.insert(ETL_OR_STD::make_pair(1, N1));
      compare_data.insert(ETL_OR_STD::make_pair(1, N1));

      isEqual = Check_Equal(data.begin(),
                            data.end(),
                            compare_data.begin());

      CHECK(isEqual);

      CHECK(std::is_sorted(data.begin(), data.end()));
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_insert_value_multiple)
    {
      Compare_DataNDC compare_data;
      DataNDC data;

      data.insert(ETL_OR_STD::make_pair(0, N0));
      compare_data.insert(ETL_OR_STD::make_pair(0, N0));

      data.insert(ETL_OR_STD::make_pair(1, N1));
      compare_data.insert(ETL_OR_STD::make_pair(1, N1));

      data.insert(ETL_OR_STD::make_pair(2, N2));
      compare_data.insert(ETL_OR_STD::make_pair(2, N2));

      // Do it again.
      data.insert(ETL_OR_STD::make_pair(0, N0));
      compare_data.insert(ETL_OR_STD::make_pair(0, N0));

      data.insert(ETL_OR_STD::make_pair(1, N1));
      compare_data.insert(ETL_OR_STD::make_pair(1, N1));

      data.insert(ETL_OR_STD::make_pair(2, N2));
      compare_data.insert(ETL_OR_STD::make_pair(2, N2));

      CHECK(compare_data.size() == data.size());

      bool isEqual = Check_Equal(data.begin(),
                                 data.end(),
                                 compare_data.begin());

      CHECK(isEqual);

      CHECK(std::is_sorted(data.begin(), data.end()));
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_insert_value_excess)
    {
      DataNDC data(initial_data.begin(), initial_data.end());

      CHECK_THROW(data.insert(ETL_OR_STD::make_pair(10, N10)), etl::flat_multimap_full);

      CHECK(std::is_sorted(data.begin(), data.end()));
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_insert_range)
    {
      Compare_DataNDC compare_data;
      DataNDC data;

      data.insert(initial_data.begin(), initial_data.end());
      compare_data.insert(initial_data.begin(), initial_data.end());

      bool isEqual = Check_Equal(data.begin(),
                                 data.end(),
                                 compare_data.begin());

      CHECK(isEqual);

      CHECK(std::is_sorted(data.begin(), data.end()));
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_insert_range_excess)
    {
      DataNDC data;

      CHECK_THROW(data.insert(excess_data.begin(), excess_data.end()), etl::flat_multimap_full);

      CHECK(std::is_sorted(data.begin(), data.end()));
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_emplace_value1)
    {
      Compare1 compare;
      Data1 data;

      data.emplace(0, "0");
      compare.emplace(0, D1("0"));

      data.emplace(1, D1("1"));
      compare.emplace(1, D1("1"));

      data.emplace(ETL_OR_STD::make_pair(2, D1("2")));
      compare.emplace(ETL_OR_STD::make_pair(2, D1("2")));

      // Do it again.
      data.emplace(0, "0");
      compare.emplace(0, D1("0"));

      data.emplace(1, D1("1"));
      compare.emplace(1, D1("1"));

      data.emplace(ETL_OR_STD::make_pair(2, D1("2")));
      compare.emplace(ETL_OR_STD::make_pair(2, D1("2")));

      CHECK(compare.size() == data.size());

      bool isEqual = Check_Equal(data.begin(),
                                 data.end(),
                                 compare.begin());

      CHECK(isEqual);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_emplace_value2)
    {
      Compare2 compare;
      Data2 data;

      data.emplace(0, "0", "1");
      compare.emplace(0, D2("0", "1"));

      data.emplace(1, D2("1", "2"));
      compare.emplace(1, D2("1", "2"));

      data.emplace(ETL_OR_STD::make_pair(2, D2("2", "3")));
      compare.emplace(ETL_OR_STD::make_pair(2, D2("2", "3")));

      // Do it again.
      data.emplace(0, "0", "1");
      compare.emplace(0, D2("0", "1"));

      data.emplace(1, D2("1", "2"));
      compare.emplace(1, D2("1", "2"));

      data.emplace(ETL_OR_STD::make_pair(2, D2("2", "3")));
      compare.emplace(ETL_OR_STD::make_pair(2, D2("2", "3")));

      CHECK(compare.size() == data.size());

      bool isEqual = Check_Equal(data.begin(),
                                 data.end(),
                                 compare.begin());

      CHECK(isEqual);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_emplace_value3)
    {
      Compare3 compare;
      Data3 data;

      data.emplace(0, "0", "1", "2");
      compare.emplace(0, D3("0", "1", "2"));

      data.emplace(1, D3("1", "2", "3"));
      compare.emplace(1, D3("1", "2", "3"));

      data.emplace(ETL_OR_STD::make_pair(2, D3("2", "3", "4")));
      compare.emplace(ETL_OR_STD::make_pair(2, D3("2", "3", "4")));

      // Do it again.
      data.emplace(0, "0", "1", "2");
      compare.emplace(0, D3("0", "1", "2"));

      data.emplace(1, D3("1", "2", "3"));
      compare.emplace(1, D3("1", "2", "3"));

      data.emplace(ETL_OR_STD::make_pair(2, D3("2", "3", "4")));
      compare.emplace(ETL_OR_STD::make_pair(2, D3("2", "3", "4")));

      CHECK(compare.size() == data.size());

      bool isEqual = Check_Equal(data.begin(),
                                 data.end(),
                                 compare.begin());

      CHECK(isEqual);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_emplace_value4)
    {
      Compare4 compare;
      Data4 data;

      data.emplace(0, "0", "1", "2", "3");
      compare.emplace(0, D4("0", "1", "2", "3"));

      data.emplace(1, D4("1", "2", "3", "4"));
      compare.emplace(1, D4("1", "2", "3", "4"));

      data.emplace(ETL_OR_STD::make_pair(2, D4("2", "3", "4", "5")));
      compare.emplace(ETL_OR_STD::make_pair(2, D4("2", "3", "4", "5")));

      // Do it again.
      data.emplace(0, "0", "1", "2", "3");
      compare.emplace(0, D4("0", "1", "2", "3"));

      data.emplace(1, D4("1", "2", "3", "4"));
      compare.emplace(1, D4("1", "2", "3", "4"));

      data.emplace(ETL_OR_STD::make_pair(2, D4("2", "3", "4", "5")));
      compare.emplace(ETL_OR_STD::make_pair(2, D4("2", "3", "4", "5")));

      CHECK(compare.size() == data.size());

      bool isEqual = Check_Equal(data.begin(),
                                 data.end(),
                                 compare.begin());

      CHECK(isEqual);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_erase_key)
    {
      Compare_DataNDC compare_data(initial_data.begin(), initial_data.end());
      DataNDC data(initial_data.begin(), initial_data.end());

      size_t count_compare = compare_data.erase(5);
      size_t count         = data.erase(5);

      CHECK(count_compare == count);

      bool isEqual = Check_Equal(data.begin(),
                                 data.end(),
                                 compare_data.begin());

      CHECK(isEqual);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_erase_key_using_transparent_comparator)
    {
      using CMap = std::multimap<int, NDC>;
      CMap compare_data(initial_data.begin(), initial_data.end());

      using EMap = etl::flat_multimap<int, NDC, SIZE, etl::less<>>;
      EMap data(initial_data.begin(), initial_data.end());

      size_t count_compare = compare_data.erase(5);
      size_t count = data.erase(Key(5));

      CHECK(count_compare == count);

      bool isEqual = Check_Equal(data.begin(), data.end(), compare_data.begin());

      CHECK(isEqual);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_erase_single_iterator)
    {
      Compare_DataNDC compare_data(initial_data.begin(), initial_data.end());
      DataNDC data(initial_data.begin(), initial_data.end());

      Compare_DataNDC::iterator i_compare_begin = compare_data.begin();
      DataNDC::iterator         i_data_begin    = data.begin();

      Compare_DataNDC::iterator i_compare = i_compare_begin;
      DataNDC::iterator         i_data    = i_data_begin;

      Compare_DataNDC::iterator i_compare_expected = i_compare_begin;
      DataNDC::iterator         i_data_expected   = i_data_begin;

      std::advance(i_compare, 2);
      std::advance(i_data, 2);

      std::advance(i_compare_expected, 3);
      std::advance(i_data_expected, 3);

      ElementNDC compare_expected = *i_compare_expected;
      ElementNDC data_expected    = *i_data_expected;

      Compare_DataNDC::iterator i_compare_result = compare_data.erase(i_compare);
      DataNDC::iterator         i_data_result    = data.erase(i_data);

      CHECK(compare_expected.first == i_compare_result->first);
      CHECK(data_expected.first    == i_data_result->first);

      CHECK(compare_expected.second == i_compare_result->second);
      CHECK(data_expected.second   == i_data_result->second);

      bool isEqual = std::equal(data.begin(),
                                data.end(),
                                compare_data.begin());

      CHECK(isEqual);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_erase_single_const_iterator)
    {
      Compare_DataNDC compare_data(initial_data.begin(), initial_data.end());
      DataNDC data(initial_data.begin(), initial_data.end());

      Compare_DataNDC::iterator i_compare_begin = compare_data.begin();
      DataNDC::iterator         i_data_begin    = data.begin();

      Compare_DataNDC::const_iterator i_compare = i_compare_begin;
      DataNDC::const_iterator         i_data    = i_data_begin;

      Compare_DataNDC::iterator i_compare_expected = i_compare_begin;
      DataNDC::iterator         i_data_expected    = i_data_begin;

      std::advance(i_compare, 2);
      std::advance(i_data, 2);

      std::advance(i_compare_expected, 3);
      std::advance(i_data_expected, 3);

      ElementNDC compare_expected = *i_compare_expected;
      ElementNDC data_expected    = *i_data_expected;

      Compare_DataNDC::iterator i_compare_result = compare_data.erase(i_compare);
      DataNDC::iterator         i_data_result    = data.erase(i_data);

      CHECK(compare_expected.first == i_compare_result->first);
      CHECK(data_expected.first    == i_data_result->first);

      CHECK(compare_expected.second == i_compare_result->second);
      CHECK(data_expected.second    == i_data_result->second);

      bool isEqual = std::equal(data.begin(),
                                data.end(),
                                compare_data.begin());

      CHECK(isEqual);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_erase_range)
    {
      Compare_DataNDC compare_data(initial_data.begin(), initial_data.end());
      DataNDC data(initial_data.begin(), initial_data.end());

      Compare_DataNDC::const_iterator i_compare = compare_data.begin();
      DataNDC::const_iterator         i_data    = data.begin();

      Compare_DataNDC::const_iterator i_compare_end = compare_data.begin();
      DataNDC::const_iterator         i_data_end    = data.begin();

      std::advance(i_compare, 2);
      std::advance(i_data, 2);

      std::advance(i_compare_end, 4);
      std::advance(i_data_end, 4);

      ElementNDC compare_expected = *i_compare_end;
      ElementNDC data_expected    = *i_data_end;

      Compare_DataNDC::iterator i_compare_result = compare_data.erase(i_compare, i_compare_end);
      DataNDC::iterator         i_data_result    = data.erase(i_data, i_data_end);

      CHECK(compare_expected.first == i_compare_result->first);
      CHECK(data_expected.first    == i_data_result->first);

      CHECK(compare_expected.second == i_compare_result->second);
      CHECK(data_expected.second    == i_data_result->second);

      bool isEqual = std::equal(data.begin(),
                                data.end(),
                                compare_data.begin());

      CHECK(isEqual);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_clear)
    {
      Compare_DataNDC compare_data(initial_data.begin(), initial_data.end());

      DataNDC data(compare_data.begin(), compare_data.end());
      data.clear();
      CHECK(data.size() == 0UL);

      // Do it again to check that clear() didn't screw up the internals.
      data.assign(compare_data.begin(), compare_data.end());
      CHECK(data.size() == compare_data.size());
      data.clear();
      CHECK(data.size() == 0UL);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_clear_pod)
    {
      DataInt data(int_data.begin(), int_data.end());

      data.clear();
      CHECK(data.size() == 0UL);

      // Do it again to check that clear() didn't screw up the internals.
      data.assign(int_data.begin(), int_data.end());
      CHECK(data.size() == int_data.size());
      data.clear();
      CHECK(data.size() == 0UL);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_iterator)
    {
      Compare_DataNDC compare_data(initial_data.begin(), initial_data.end());

      DataNDC data(compare_data.begin(), compare_data.end());

      bool isEqual = Check_Equal(data.begin(),
                                 data.end(),
                                 compare_data.begin());

      CHECK(isEqual);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_const_iterator)
    {
      Compare_DataNDC compare_data(initial_data.begin(), initial_data.end());

      DataNDC data(compare_data.begin(), compare_data.end());

      bool isEqual = Check_Equal(data.cbegin(),
                                 data.cend(),
                                 compare_data.cbegin());

      CHECK(isEqual);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_reverse_iterator)
    {
      Compare_DataNDC compare_data(initial_data.begin(), initial_data.end());

      DataNDC data(compare_data.begin(), compare_data.end());

      bool isEqual = Check_Equal(data.rbegin(),
                                 data.rend(),
                                 compare_data.rbegin());

      CHECK(isEqual);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_const_reverse_iterator)
    {
      Compare_DataNDC compare_data(initial_data.begin(), initial_data.end());

      DataNDC data(compare_data.begin(), compare_data.end());

      bool isEqual = Check_Equal(data.crbegin(),
                                 data.crend(),
                                 compare_data.crbegin());

      CHECK(isEqual);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_find)
    {
      DataNDC data(initial_data.begin(), initial_data.end());

      DataNDC::iterator it = data.find(3);
      CHECK(N3 == it->second);

      it = data.find(19);
      CHECK(data.end() == it);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_find_using_transparent_comparator)
    {
      using EMap = etl::flat_multimap<int, NDC, SIZE, etl::less<>>;
      EMap data(initial_data.begin(), initial_data.end());

      EMap::iterator it = data.find(Key(3));
      CHECK(N3 == it->second);

      it = data.find(Key(19));
      CHECK(data.end() == it);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_find_not_present)
    {
      DataNDC data(initial_data.begin(), initial_data.end());

      DataNDC::iterator it = data.find(-1);
      CHECK(data.end() == it);

      it = data.find(10);
      CHECK(data.end() == it);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_find_not_present_using_transparent_comparator)
    {
      using EMap = etl::flat_multimap<int, NDC, SIZE, etl::less<>>;
      EMap data(initial_data.begin(), initial_data.end());

      EMap::iterator it = data.find(Key(-1));
      CHECK(data.end() == it);

      it = data.find(Key(10));
      CHECK(data.end() == it);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_find_const)
    {
      const DataNDC data(initial_data.begin(), initial_data.end());

      DataNDC::const_iterator it = data.find(3);
      CHECK(N3 == it->second);

      it = data.find(19);
      CHECK(data.end() == it);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_find_const_using_transparent_comparator)
    {
      using EMap = etl::flat_multimap<int, NDC, SIZE, etl::less<>>;
      const EMap data(initial_data.begin(), initial_data.end());

      EMap::const_iterator it = data.find(Key(3));
      CHECK(N3 == it->second);

      it = data.find(Key(19));
      CHECK(data.end() == it);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_find_const_not_present)
    {
      const DataNDC data(initial_data.begin(), initial_data.end());

      DataNDC::const_iterator it = data.find(-1);
      CHECK(data.end() == it);

      it = data.find(10);
      CHECK(data.end() == it);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_find_const_not_present_using_transparent_comparator)
    {
      using EMap = etl::flat_multimap<int, NDC, SIZE, etl::less<>>;
      const EMap data(initial_data.begin(), initial_data.end());

      EMap::const_iterator it = data.find(Key(-1));
      CHECK(data.end() == it);

      it = data.find(Key(10));
      CHECK(data.end() == it);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_lower_bound)
    {
      Compare_DataNDC compare_data(initial_data.begin(), initial_data.end());
      DataNDC data(initial_data.begin(), initial_data.end());

      Compare_DataNDC::iterator i_compare = compare_data.lower_bound(5);
      DataNDC::iterator         i_data    = data.lower_bound(5);

      CHECK(std::distance(compare_data.begin(), i_compare) == std::distance(data.begin(), i_data));
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_lower_bound_using_transparent_comparator)
    {
      using CMap = std::multimap<int, NDC>;
      CMap compare_data(initial_data.begin(), initial_data.end());

      using EMap = etl::flat_multimap<int, NDC, SIZE, etl::less<>>;
      EMap data(initial_data.begin(), initial_data.end());

      CMap::iterator i_compare = compare_data.lower_bound(5);
      EMap::iterator i_data    = data.lower_bound(Key(5));

      CHECK(std::distance(compare_data.begin(), i_compare) == std::distance(data.begin(), i_data));
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_upper_bound)
    {
      Compare_DataNDC compare_data(initial_data.begin(), initial_data.end());
      DataNDC data(initial_data.begin(), initial_data.end());

      Compare_DataNDC::iterator i_compare = compare_data.upper_bound(5);
      DataNDC::iterator         i_data    = data.upper_bound(5);

      CHECK(std::distance(compare_data.begin(), i_compare) == std::distance(data.begin(), i_data));
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_upper_bound_using_transparent_comparator)
    {
      using CMap = std::multimap<int, NDC>;
      CMap compare_data(initial_data.begin(), initial_data.end());

      using EMap = etl::flat_multimap<int, NDC, SIZE, etl::less<>>;
      EMap data(initial_data.begin(), initial_data.end());

      CMap::iterator i_compare = compare_data.upper_bound(5);
      EMap::iterator i_data    = data.upper_bound(Key(5));

      CHECK(std::distance(compare_data.begin(), i_compare) == std::distance(data.begin(), i_data));
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_equal_range)
    {
      Compare_DataNDC compare_data(initial_data.begin(), initial_data.end());
      DataNDC data(initial_data.begin(), initial_data.end());

      ETL_OR_STD::pair<Compare_DataNDC::iterator, Compare_DataNDC::iterator> i_compare = compare_data.equal_range(5);
      ETL_OR_STD::pair<DataNDC::iterator, DataNDC::iterator> i_data = data.equal_range(5);

      CHECK(std::distance(compare_data.begin(), i_compare.first) ==  std::distance(data.begin(), i_data.first));
      CHECK(std::distance(compare_data.begin(), i_compare.second) == std::distance(data.begin(), i_data.second));
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_equal_range_using_transparent_comparator)
    {
      using CMap = std::multimap<int, NDC>;
      CMap compare_data(initial_data.begin(), initial_data.end());

      using EMap = etl::flat_multimap<int, NDC, SIZE, etl::less<>>;
      EMap data(initial_data.begin(), initial_data.end());

      ETL_OR_STD::pair<CMap::iterator, CMap::iterator> i_compare = compare_data.equal_range(5);
      ETL_OR_STD::pair<EMap::iterator, EMap::iterator> i_data = data.equal_range(Key(5));

      CHECK(std::distance(compare_data.begin(), i_compare.first) == std::distance(data.begin(), i_data.first));
      CHECK(std::distance(compare_data.begin(), i_compare.second) == std::distance(data.begin(), i_data.second));
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_equal_range_not_present)
    {
      DataNDC data(initial_data.begin(), initial_data.end());

      ETL_OR_STD::pair<DataNDC::iterator, DataNDC::iterator> i_data;

      i_data = data.equal_range(-1);
      CHECK(data.begin() == i_data.first);
      CHECK(data.begin() == i_data.second);

      i_data = data.equal_range(99);
      CHECK(data.end() == i_data.first);
      CHECK(data.end() == i_data.second);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_equal_range_not_present_using_transparent_comparator)
    {
      using EMap = etl::flat_multimap<int, NDC, SIZE, etl::less<>>;
      EMap data(initial_data.begin(), initial_data.end());

      ETL_OR_STD::pair<EMap::iterator, EMap::iterator> i_data;

      i_data = data.equal_range(Key(-1));
      CHECK(data.begin() == i_data.first);
      CHECK(data.begin() == i_data.second);

      i_data = data.equal_range(Key(99));
      CHECK(data.end() == i_data.first);
      CHECK(data.end() == i_data.second);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_equal)
    {
      const DataNDC initial1(initial_data.begin(), initial_data.end());
      const DataNDC initial2(initial_data.begin(), initial_data.end());

      CHECK(initial1 == initial2);

      const DataNDC different(different_data.begin(), different_data.end());

      CHECK(!(initial1 == different));
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_not_equal)
    {
      const DataNDC initial1(initial_data.begin(), initial_data.end());
      const DataNDC initial2(initial_data.begin(), initial_data.end());

      CHECK(!(initial1 != initial2));

      const DataNDC different(different_data.begin(), different_data.end());

      CHECK(initial1 != different);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_multi)
    {
      Compare_DataNDC compare_data(multi_data.begin(), multi_data.end());
      DataNDC data(multi_data.begin(), multi_data.end());

      ETL_OR_STD::pair<Compare_DataNDC::iterator, Compare_DataNDC::iterator> compare_range;
      ETL_OR_STD::pair<DataNDC::iterator, DataNDC::iterator> test_range;

      compare_range = compare_data.equal_range(0);
      test_range    = data.equal_range(0);
      CHECK(std::distance(compare_range.first, compare_range.second) == std::distance(test_range.first, test_range.second));

      compare_range = compare_data.equal_range(1);
      test_range    = data.equal_range(1);
      CHECK(std::distance(compare_range.first, compare_range.second) == std::distance(test_range.first, test_range.second));

      compare_range = compare_data.equal_range(2);
      test_range    = data.equal_range(2);
      CHECK(std::distance(compare_range.first, compare_range.second) == std::distance(test_range.first, test_range.second));

      compare_range = compare_data.equal_range(3);
      test_range    = data.equal_range(3);
      CHECK(std::distance(compare_range.first, compare_range.second) == std::distance(test_range.first, test_range.second));

      compare_range = compare_data.equal_range(4);
      test_range    = data.equal_range(4);
      CHECK(std::distance(compare_range.first, compare_range.second) == std::distance(test_range.first, test_range.second));

      compare_range = compare_data.equal_range(5);
      test_range    = data.equal_range(5);
      CHECK(std::distance(compare_range.first, compare_range.second) == std::distance(test_range.first, test_range.second));
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_count)
    {
      Compare_DataNDC compare_data(multi_data.begin(), multi_data.end());
      DataNDC data(multi_data.begin(), multi_data.end());

      CHECK(compare_data.count(0) == data.count(0));
      CHECK(compare_data.count(1) == data.count(1));
      CHECK(compare_data.count(2) == data.count(2));
      CHECK(compare_data.count(3) == data.count(3));
      CHECK(compare_data.count(4) == data.count(4));
      CHECK(compare_data.count(5) == data.count(5));
    }

    //*************************************************************************
#if ETL_USING_CPP17 && ETL_HAS_INITIALIZER_LIST && !defined(ETL_TEMPLATE_DEDUCTION_GUIDE_TESTS_DISABLED)
    TEST_FIXTURE(SetupFixture, test_flat_multimap_template_deduction)
    {
      using Pair = ETL_OR_STD::pair<const int, NDC>;

      etl::flat_multimap data{ Pair(0, NDC("A")), Pair(1, NDC("B")), Pair(1, NDC("B2")), Pair(2, NDC("C")), Pair(3, NDC("D")), Pair(4, NDC("E")), Pair(5, NDC("F")) };
      auto v = *data.begin();
      using Type = decltype(v);
      CHECK((std::is_same_v<Pair, Type>));

      decltype(data)::const_iterator itr = data.begin();

      CHECK_EQUAL(NDC("A"), itr->second);
      ++itr;
      CHECK_EQUAL(NDC("B"), itr->second);
      ++itr;
      CHECK_EQUAL(NDC("B2"), itr->second);
      ++itr;
      CHECK_EQUAL(NDC("C"), itr->second);
      ++itr;
      CHECK_EQUAL(NDC("D"), itr->second);
      ++itr;
      CHECK_EQUAL(NDC("E"), itr->second);
      ++itr;
      CHECK_EQUAL(NDC("F"), itr->second);
    }
#endif

    //*************************************************************************
#if ETL_HAS_INITIALIZER_LIST
    TEST_FIXTURE(SetupFixture, test_make_flat_multimap)
    {
      using Pair = ETL_OR_STD::pair<const int, NDC>;

      auto data = etl::make_flat_multimap<const int, NDC>(Pair(0, NDC("A")), Pair(1, NDC("B")), Pair(1, NDC("B2")), Pair(2, NDC("C")), Pair(3, NDC("D")), Pair(4, NDC("E")), Pair(5, NDC("F")));

      auto v = *data.begin();
      using Type = decltype(v);
      CHECK((std::is_same<Pair, Type>::value));

      decltype(data)::const_iterator itr = data.begin();

      CHECK_EQUAL(NDC("A"), itr->second);
      ++itr;
      CHECK_EQUAL(NDC("B"), itr->second);
      ++itr;
      CHECK_EQUAL(NDC("B2"), itr->second);
      ++itr;
      CHECK_EQUAL(NDC("C"), itr->second);
      ++itr;
      CHECK_EQUAL(NDC("D"), itr->second);
      ++itr;
      CHECK_EQUAL(NDC("E"), itr->second);
      ++itr;
      CHECK_EQUAL(NDC("F"), itr->second);
    }
#endif

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_contains)
    {
      DataNDC data(initial_data.begin(), initial_data.end());

      CHECK(data.contains(1));
      CHECK(!data.contains(99));
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_contains_with_transparent_comparator)
    {
      using EMap = etl::flat_multimap<int, NDC, SIZE, etl::less<>>;
      EMap data(initial_data.begin(), initial_data.end());

      CHECK(data.contains(Key(1)));
      CHECK(!data.contains(Key(99)));
    }
  };
}
