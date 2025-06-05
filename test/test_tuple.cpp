/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl
https://www.etlcpp.com

Copyright(c) 2024 John Wellbelove

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

#include "data.h"

#include "etl/tuple.h"

#include <string>
#include <array>

namespace
{
  using Data  = TestDataDC<std::string>;
  using DataM = TestDataM<std::string>;

  template <std::size_t... Indices>
  auto to_array(etl::index_sequence<Indices...>)
    -> std::array<std::size_t, sizeof...(Indices)>
  {
    return std::array<std::size_t, sizeof...(Indices)>{Indices...};
  }

  //*********************************
  struct From 
  {
    From(int i_)
      : i(i_)
    {
    }

    int i;
  };

  //*********************************
  struct To 
  {
    explicit To(const From& from)
      : i(from.i)
    {
    }

    To& operator =(const From& from)
    {
      i = from.i;
      return *this;
    }

    int i;
  };
  
  SUITE(test_tuple)
  {
    //*************************************************************************
    TEST(test_tuple_size)
    {
      using Tuple = etl::tuple<int, double, int, Data>;

      Tuple tp;

      CHECK_EQUAL(4U, etl::tuple_size<Tuple>::value);
      CHECK_EQUAL(4U, tp.size());

#if (ETL_USING_CPP17)
      CHECK_EQUAL(4U, etl::tuple_size_v<Tuple>);
#endif
    }

    //*************************************************************************
    TEST(test_tuple_element)
    {
      using Tuple = etl::tuple<int, double, int, Data>;

      CHECK_TRUE((std::is_same<int,    etl::tuple_element_t<0, Tuple>>::value));
      CHECK_TRUE((std::is_same<double, etl::tuple_element_t<1, Tuple>>::value));
      CHECK_TRUE((std::is_same<int,    etl::tuple_element_t<2, Tuple>>::value));
      CHECK_TRUE((std::is_same<Data,   etl::tuple_element_t<3, Tuple>>::value));
    }

    //*************************************************************************
    TEST(test_default_constructor)
    {
      etl::tuple<int, double, int, Data> tp;

      int          i0 = etl::get<0>(tp);
      double       d1 = etl::get<1>(tp);
      int          i2 = etl::get<2>(tp);
      std::string& s3 = etl::get<3>(tp).value;

      CHECK_EQUAL(0, i0);
      CHECK_EQUAL(0, d1);
      CHECK_EQUAL(0, i2);
      CHECK_TRUE(s3.empty());
    }

    //*************************************************************************
    TEST(test_constructor_single_element)
    {
      etl::tuple<int> tp(1);

      CHECK_EQUAL(1, etl::get<0>(tp));
    }

    //*************************************************************************
    TEST(test_copy_constructor)
    {
      etl::tuple<int, double, int, Data> tp(1, 2.2, 3, Data("4"));
      etl::tuple<int, double, int, Data> otherTuple(tp);

      CHECK_EQUAL(etl::get<0>(tp), etl::get<0>(otherTuple));
      CHECK_EQUAL(etl::get<1>(tp), etl::get<1>(otherTuple));
      CHECK_EQUAL(etl::get<2>(tp), etl::get<2>(otherTuple));
      CHECK_EQUAL(etl::get<3>(tp), etl::get<3>(otherTuple));
    }

    //*************************************************************************
    TEST(test_copy_constructor_from_explicitly_convertible_type)
    {
      etl::tuple<From> from(1);
      etl::tuple<To> to(from);

      CHECK_EQUAL(etl::get<0>(to).i, etl::get<0>(from).i);
    }

    //*************************************************************************
    TEST(test_copy_constructor_from_const_explicitly_convertible_type)
    {
      const etl::tuple<From> from(1);
      etl::tuple<To> to(from);

      CHECK_EQUAL(etl::get<0>(to).i, etl::get<0>(from).i);
    }

    //*************************************************************************
    TEST(test_move_constructor)
    {
      etl::tuple<int, double, int, Data> tp(1, 2.2, 3, Data("4"));
      etl::tuple<int, double, int, Data> otherTuple(etl::move(tp));

      CHECK_EQUAL(etl::get<0>(tp), etl::get<0>(otherTuple));
      CHECK_EQUAL(etl::get<1>(tp), etl::get<1>(otherTuple));
      CHECK_EQUAL(etl::get<2>(tp), etl::get<2>(otherTuple));
      CHECK_EQUAL(etl::get<3>(tp), etl::get<3>(otherTuple));
    }

    //*************************************************************************
    TEST(test_move_constructor_from_const)
    {
      const etl::tuple<int, double, int, Data> tp(1, 2.2, 3, Data("4"));
      etl::tuple<int, double, int, Data> otherTuple(etl::move(tp));

      CHECK_EQUAL(etl::get<0>(tp), etl::get<0>(otherTuple));
      CHECK_EQUAL(etl::get<1>(tp), etl::get<1>(otherTuple));
      CHECK_EQUAL(etl::get<2>(tp), etl::get<2>(otherTuple));
      CHECK_EQUAL(etl::get<3>(tp), etl::get<3>(otherTuple));
    }

    //*************************************************************************
    TEST(test_move_constructor_from_implicitly_convertible_type)
    {
      etl::tuple<short, float, short, Data> tp(1, 2.2f, 3, Data("4"));
      etl::tuple<int, double, int, Data> otherTuple(etl::move(tp));

      CHECK_EQUAL(etl::get<0>(tp), etl::get<0>(otherTuple));
      CHECK_EQUAL(etl::get<1>(tp), etl::get<1>(otherTuple));
      CHECK_EQUAL(etl::get<2>(tp), etl::get<2>(otherTuple));
      CHECK_EQUAL(etl::get<3>(tp), etl::get<3>(otherTuple));
    }

    //*************************************************************************
    TEST(test_move_constructor_from_const_implicitly_convertible_type)
    {
      const etl::tuple<short, float, short, Data> tp(1, 2.2f, 3, Data("4"));
      etl::tuple<int, double, int, Data> otherTuple(etl::move(tp));

      CHECK_EQUAL(etl::get<0>(tp), etl::get<0>(otherTuple));
      CHECK_EQUAL(etl::get<1>(tp), etl::get<1>(otherTuple));
      CHECK_EQUAL(etl::get<2>(tp), etl::get<2>(otherTuple));
      CHECK_EQUAL(etl::get<3>(tp), etl::get<3>(otherTuple));
    }

    //*************************************************************************
    TEST(test_move_constructor_from_explicitly_convertible_type)
    {
      etl::tuple<From> from(1);
      etl::tuple<To> to(etl::move(from));

      CHECK_EQUAL(etl::get<0>(to).i, etl::get<0>(from).i);
    }

    //*************************************************************************
    TEST(test_move_constructor_from_const_explicitly_convertible_type)
    {
      const etl::tuple<From> from(1);
      etl::tuple<To> to(etl::move(from));

      CHECK_EQUAL(etl::get<0>(to).i, etl::get<0>(from).i);
    }

    //*************************************************************************
    TEST(test_copy_constructor_single_element)
    {
      etl::tuple<int> tp(1);
      etl::tuple<int> otherTuple(tp);

      CHECK_EQUAL(etl::get<0>(tp), etl::get<0>(otherTuple));
    }

    //*************************************************************************
    TEST(test_copy_assignment)
    {
      etl::tuple<int, double, int, Data> tp(1, 2.2, 3, Data("4"));
      etl::tuple<int, double, int, Data> otherTuple(5, 6.6, 7, Data("8"));

      otherTuple = tp;

      CHECK_EQUAL(etl::get<0>(tp), etl::get<0>(otherTuple));
      CHECK_EQUAL(etl::get<1>(tp), etl::get<1>(otherTuple));
      CHECK_EQUAL(etl::get<2>(tp), etl::get<2>(otherTuple));
      CHECK_EQUAL(etl::get<3>(tp), etl::get<3>(otherTuple));
    }

    //*************************************************************************
    TEST(test_move_assignment)
    {
      etl::tuple<int, double, int, DataM> tp(1, 2.2, 3, DataM("4"));
      etl::tuple<int, double, int, DataM> otherTuple(5, 6.6, 7, DataM("8"));

      otherTuple = std::move(tp);

      CHECK_EQUAL(etl::get<0>(otherTuple), 1);
      CHECK_EQUAL(etl::get<1>(otherTuple), 2.2);
      CHECK_EQUAL(etl::get<2>(otherTuple), 3);
      CHECK_EQUAL(etl::get<3>(otherTuple), DataM("4"));
    }

    //*************************************************************************
    TEST(test_construct_from_parameters)
    {
      etl::tuple<int, double, int, Data> tp(1, 2.2, 3, Data("4"));

      int          i0 = etl::get<0>(tp);
      double       d1 = etl::get<1>(tp);
      int          i2 = etl::get<2>(tp);
      std::string& s3 = etl::get<3>(tp).value;

      CHECK_EQUAL(1, i0);
      CHECK_CLOSE(2.2, d1, 0.01);
      CHECK_EQUAL(3, i2);
      CHECK_EQUAL(std::string("4"), s3);
    }

    //*************************************************************************
    TEST(test_construct_from_convertible_types)
    {
      etl::tuple<char, float, short> tp(char(1), float(2.2), short(3));

      int          i0 = etl::get<0>(tp);
      double       d1 = etl::get<1>(tp);
      int          i2 = etl::get<2>(tp);

      CHECK_EQUAL(1, i0);
      CHECK_CLOSE(2.2, d1, 0.01);
      CHECK_EQUAL(3, i2);
    }

    //*************************************************************************
    TEST(test_construct_from_parameters_including_move_only)
    {
      etl::tuple<int, double, int, DataM> tp(1, 2.2, 3, etl::move(DataM("4")));

      int          i0 = etl::get<0>(tp);
      double       d1 = etl::get<1>(tp);
      int          i2 = etl::get<2>(tp);
      std::string& s3 = etl::get<3>(tp).value;

      CHECK_EQUAL(1, i0);
      CHECK_CLOSE(2.2, d1, 0.01);
      CHECK_EQUAL(3, i2);
      CHECK_EQUAL(std::string("4"), s3);
    }

#if ETL_USING_CPP17
    //*************************************************************************
    TEST(test_constructor_from_parameters_using_template_deduction_guideline)
    {
      etl::tuple tp(1, 2.2, 3, Data("4"));

      int          i0 = etl::get<0>(tp);
      double       d1 = etl::get<1>(tp);
      int          i2 = etl::get<2>(tp);
      std::string& s3 = etl::get<3>(tp).value;

      CHECK_EQUAL(1, i0);
      CHECK_CLOSE(2.2, d1, 0.01);
      CHECK_EQUAL(3, i2);
      CHECK_EQUAL(std::string("4"), s3);
    }
#endif

    //*************************************************************************
    TEST(test_make_tuple)
    {
      auto tp1 = etl::make_tuple(1, 2.2, 3, Data("4"));

      int          i0 = etl::get<0>(tp1);
      double       d1 = etl::get<1>(tp1);
      int          i2 = etl::get<2>(tp1);
      std::string& s3 = etl::get<3>(tp1).value;

      CHECK_EQUAL(1, i0);
      CHECK_CLOSE(2.2, d1, 0.01);
      CHECK_EQUAL(3, i2);
      CHECK_EQUAL(std::string("4"), s3);
    }

    //*************************************************************************
    TEST(test_swap)
    {
      etl::tuple<int, int> tp1(1, 2);
      etl::tuple<int, int> tp2(3, 4);

      swap(tp1, tp2);

      CHECK_EQUAL(3, etl::get<0>(tp1));
      CHECK_EQUAL(4, etl::get<1>(tp1));

      CHECK_EQUAL(1, etl::get<0>(tp2));
      CHECK_EQUAL(2, etl::get<1>(tp2));
    }

    //*************************************************************************
    TEST(test_get_using_index_lvalue)
    {
      etl::tuple<Data, DataM>       data(Data("1"), DataM("3"));
      const etl::tuple<Data, DataM> const_data(Data("2"), DataM("4"));

      Data  d0 = etl::get<0>(data);
      Data  d1 = etl::get<0>(const_data);
      DataM d2 = etl::move(etl::get<1>(data));
      DataM d3 = etl::move(etl::get<1>(const_data));

      CHECK_EQUAL(std::string("1"), d0.value);
      CHECK_EQUAL(std::string("2"), d1.value);
      CHECK_EQUAL(std::string("3"), d2.value);
      CHECK_EQUAL(std::string("4"), d3.value);
    }

    //*************************************************************************
    TEST(test_get_using_index_rvalue)
    {
      etl::tuple<Data, DataM>       data(Data("1"), DataM("3"));
      const etl::tuple<Data, DataM> const_data(Data("2"), DataM("4"));

      Data&&        d0 = etl::get<0>(etl::move(data));
      const Data&&  d1 = etl::get<0>(etl::move(const_data));
      DataM&&       d2 = etl::move(etl::get<1>(etl::move(data)));
      const DataM&& d3 = etl::move(etl::get<1>(etl::move(const_data)));

      CHECK_EQUAL(std::string("1"), d0.value);
      CHECK_EQUAL(std::string("2"), d1.value);
      CHECK_EQUAL(std::string("3"), d2.value);
      CHECK_EQUAL(std::string("4"), d3.value);
    }

    //*************************************************************************
    TEST(test_get_using_type_lvalue)
    {
      etl::tuple<Data, DataM>       data(Data("1"), DataM("3"));
      const etl::tuple<Data, DataM> const_data(Data("2"), DataM("4"));

      Data  d0 = etl::get<Data>(data);
      Data  d1 = etl::get<Data>(const_data);
      DataM d2 = etl::move(etl::get<DataM>(data));
      DataM d3 = etl::move(etl::get<DataM>(const_data));

      CHECK_EQUAL(std::string("1"), d0.value);
      CHECK_EQUAL(std::string("2"), d1.value);
      CHECK_EQUAL(std::string("3"), d2.value);
      CHECK_EQUAL(std::string("4"), d3.value);
    }

    //*************************************************************************
    TEST(test_get_using_type_rvalue)
    {
      etl::tuple<Data, DataM>       data(Data("1"), DataM("3"));
      const etl::tuple<Data, DataM> const_data(Data("2"), DataM("4"));

      Data&&        d0 = etl::get<Data>(etl::move(data));
      const Data&&  d1 = etl::get<Data>(etl::move(const_data));
      DataM&&       d2 = etl::move(etl::get<DataM>(etl::move(data)));
      const DataM&& d3 = etl::move(etl::get<DataM>(etl::move(const_data)));

      CHECK_EQUAL(std::string("1"), d0.value);
      CHECK_EQUAL(std::string("2"), d1.value);
      CHECK_EQUAL(std::string("3"), d2.value);
      CHECK_EQUAL(std::string("4"), d3.value);
    }

    //*************************************************************************
    TEST(test_construct_from_pair)
    {
      ETL_OR_STD::pair<int, Data> p(1, Data("2"));

      etl::tuple<int, Data> tp(p);

      int  i = etl::get<0>(tp);
      Data d = etl::get<1>(tp);

      CHECK_EQUAL(1, i);
      CHECK_EQUAL(std::string("2"), d.value);
    }

    //*************************************************************************
    TEST(test_construct_from_const_pair)
    {
      const ETL_OR_STD::pair<int, Data> p(1, Data("2"));

      etl::tuple<int, Data> tp(p);

      int  i = etl::get<0>(tp);
      Data d = etl::get<1>(tp);

      CHECK_EQUAL(1, i);
      CHECK_EQUAL(std::string("2"), d.value);
    }

    //*************************************************************************
    TEST(test_construct_from_move_only_pair)
    {
      ETL_OR_STD::pair<int, DataM> p(1, etl::move(DataM("2")));

      etl::tuple<int, DataM> tp(etl::move(p));

      int   i = etl::get<0>(tp);
      DataM d = etl::move(etl::get<1>(tp));

      CHECK_EQUAL(1, i);
      CHECK_EQUAL(std::string("2"), d.value);
    }

    //*************************************************************************
    ETL_NODISCARD bool Get()
    {
      return true;
    }

    TEST(test_ignore_t)
    {
      // Should compile without error.
      etl::ignore = Get();
    }

    //*************************************************************************
    TEST(test_tie)
    {
      etl::tuple<int, double, int, Data> tp{1, 2.3, 4, Data("Data", 5)};

      int    i1;
      double d;
      int    i2;
      Data   data;

      etl::tie(i1, d, i2, data) = tp;

      CHECK_EQUAL(etl::get<0>(tp), i1);
      CHECK_CLOSE(etl::get<1>(tp), d, 0.01);
      CHECK_EQUAL(etl::get<2>(tp), i2);
      CHECK_EQUAL(etl::get<3>(tp), data);
    }

    //*************************************************************************
    TEST(test_moved_tie)
    {
      etl::tuple<int, double, int, Data> tp{1, 2.3, 4, Data("Data", 5)};

      int    i1;
      double d;
      int    i2;
      Data   data;

      etl::tie(i1, d, i2, data) = etl::move(tp);

      CHECK_EQUAL(etl::get<0>(tp), i1);
      CHECK_CLOSE(etl::get<1>(tp), d, 0.01);
      CHECK_EQUAL(etl::get<2>(tp), i2);
      CHECK_EQUAL(etl::get<3>(tp), data);
    }

    //*************************************************************************
    TEST(test_tie_from_convertible_types)
    {
      etl::tuple<int, double, int, Data> tp{1, 2.3, 4, Data("Data", 5)};

      char   c;
      long double ld;
      short  s;
      Data   data;

      etl::tie(c, ld, s, data) = tp;

      CHECK_EQUAL(etl::get<0>(tp), c);
      CHECK_CLOSE(etl::get<1>(tp), ld, 0.01);
      CHECK_EQUAL(etl::get<2>(tp), s);
      CHECK_EQUAL(etl::get<3>(tp), data);
    }

    //*************************************************************************
    TEST(test_tuple_cat)
    {
      etl::tuple<int, double> tp1{1, 2.3};
      etl::tuple<int, Data>   tp2{4, Data("Data", 5)};

      auto tp3 = etl::tuple_cat(tp1, tp2);

      CHECK_EQUAL(etl::get<0>(tp3), etl::get<0>(tp1));
      CHECK_EQUAL(etl::get<1>(tp3), etl::get<1>(tp1));
      CHECK_EQUAL(etl::get<2>(tp3), etl::get<0>(tp2));
      CHECK_EQUAL(etl::get<3>(tp3), etl::get<1>(tp2));
    }

    //*************************************************************************
    TEST(test_forward_as_tuple)
    {
      char        v1 = 1;
      int         v2 = 2;
      std::string v3 = "Hello";

      auto tp = etl::forward_as_tuple(v1, v2, v3);

      CHECK_EQUAL(v1, etl::get<0>(tp));
      CHECK_EQUAL(v2, etl::get<1>(tp));
      CHECK_EQUAL(v3, etl::get<2>(tp));
    }

    ////*************************************************************************
    // Uncomment to test.
    // TEST(test_static_assert)
    //{
    //   etl::tuple<int, double, int, Data> tp;

    //  auto result = etl::get<4>(tp);
    //}

    //*************************************************************************
    TEST(test_select_tuple_run_time)
    {
      using Tuple1 = etl::tuple<int, double, int, Data>;
      using Tuple2 = etl::tuple<Data, int, double, int>;

      Tuple1 tp1{1, 2.3, 4, Data("Data", 5)};

      using Sequence = etl::index_sequence<3, 2, 1, 0>;
      Tuple2 tp2 = etl::select_from_tuple(tp1, Sequence());

      CHECK_EQUAL(etl::get<0>(tp1), etl::get<3>(tp2));
      CHECK_EQUAL(etl::get<1>(tp1), etl::get<2>(tp2));
      CHECK_EQUAL(etl::get<2>(tp1), etl::get<1>(tp2));
      CHECK_EQUAL(etl::get<3>(tp1), etl::get<0>(tp2));
    }

    //*************************************************************************
    TEST(test_select_a_subset_of_a_tuple_using_a_run_time_index_sequence)
    {
      using Tuple1 = etl::tuple<int, double, int, Data>;
      using Tuple2 = etl::tuple<double, int>;

      Tuple1 tp1{1, 2.3, 4, Data("Data", 5)};

      using Sequence = etl::index_sequence<1, 0>;
      Tuple2 tp2 = etl::select_from_tuple(tp1, Sequence());

      CHECK_EQUAL(etl::get<1>(tp1), etl::get<0>(tp2));
      CHECK_EQUAL(etl::get<0>(tp1), etl::get<1>(tp2));
    }

    //*************************************************************************
    TEST(test_select_a_subset_of_a_tuple_using_a_compile_time_index_sequence)
    {
      using Tuple1 = etl::tuple<int, double, int, Data>;
      using Tuple2 = etl::tuple<double, int>;

      Tuple1 tp1{1, 2.3, 4, Data("Data", 5)};
      Tuple2 tp2 = etl::select_from_tuple<1, 0>(tp1);

      CHECK_EQUAL(etl::get<1>(tp1), etl::get<0>(tp2));
      CHECK_EQUAL(etl::get<0>(tp1), etl::get<1>(tp2));
    }

    //*************************************************************************
    TEST(test_move_select_tuple)
    {
      using Tuple1 = etl::tuple<int, double, int, DataM>;
      using Tuple2 = etl::tuple<DataM, int, double, int>;

      Tuple1 tp1{1, 2.3, 4, DataM("DataM")};

      using Sequence = etl::index_sequence<3, 2, 1, 0>;
      Tuple2 tp2 = etl::select_from_tuple(etl::move(tp1), Sequence());

      CHECK_EQUAL(etl::get<0>(tp1), etl::get<3>(tp2));
      CHECK_EQUAL(etl::get<1>(tp1), etl::get<2>(tp2));
      CHECK_EQUAL(etl::get<2>(tp1), etl::get<1>(tp2));
      CHECK_EQUAL(std::string("DataM"), etl::get<0>(tp2).value);
      CHECK_EQUAL(true, etl::get<0>(tp2).valid);

      // Check that the data was moved.
      CHECK_EQUAL(std::string(""), etl::get<3>(tp1).value);
      CHECK_EQUAL(false, etl::get<3>(tp1).valid);
    }

    //*************************************************************************
    TEST(test_index_sequence_type)
    {
      using Tuple = etl::tuple<int, double, int, Data>;

      auto             actual = to_array(Tuple::index_sequence_type{});
      decltype(actual) expected{0, 1, 2, 3};

      CHECK_EQUAL(expected.size(), actual.size());
      CHECK_ARRAY_EQUAL(expected.data(), actual.data(), expected.size());
    }

    //*************************************************************************
    TEST(test_comparison_operators)
    {
      etl::tuple<int, int> tp1(1, 2);
      etl::tuple<int, int> tp2(1, 2);
      etl::tuple<int, int> tp3(3, 4);
      etl::tuple<int, int> tp4(4, 3);

      CHECK_TRUE(tp1 == tp2);
      CHECK_FALSE(tp1 == tp3);

      CHECK_FALSE(tp1 != tp2);
      CHECK_TRUE(tp1 != tp3);

      CHECK_TRUE(tp1 < tp3);
      CHECK_FALSE(tp3 < tp1);
      CHECK_TRUE(tp3 < tp4);
      CHECK_FALSE(tp4 < tp3);

      CHECK_TRUE(tp1 <= tp2);
      CHECK_TRUE(tp1 <= tp3);
      CHECK_FALSE(tp3 <= tp1);
      CHECK_TRUE(tp3 <= tp4);
      CHECK_FALSE(tp4 <= tp3);

      CHECK_FALSE(tp1 > tp3);
      CHECK_TRUE(tp3 > tp1);
      CHECK_FALSE(tp3 > tp4);
      CHECK_TRUE(tp4 > tp3);

      CHECK_TRUE(tp1 >= tp2);
      CHECK_FALSE(tp1 >= tp3);
      CHECK_TRUE(tp3 >= tp1);
      CHECK_FALSE(tp3 >= tp4);
      CHECK_TRUE(tp4 >= tp3);
    }

#if ETL_USING_STL
    //*************************************************************************
    TEST(test_to_etl)
    {
      char        v1 = 1;
      int         v2 = 2;
      std::string v3 = "Hello";
      std::string v4 = "";

      std::tuple<char, int, std::string> tp_std(v1, v2, v3);

      auto tp_etl = etl::to_etl(tp_std);
      auto tp_etl_moved = etl::to_etl(etl::move(tp_std));

      CHECK_EQUAL(v1, etl::get<0>(tp_etl));
      CHECK_EQUAL(v2, etl::get<1>(tp_etl));
      CHECK_EQUAL(v3, etl::get<2>(tp_etl));

      CHECK_EQUAL(v1, etl::get<0>(tp_etl_moved));
      CHECK_EQUAL(v2, etl::get<1>(tp_etl_moved));
      CHECK_EQUAL(v3, etl::get<2>(tp_etl_moved));

      CHECK_EQUAL(v4, std::get<2>(tp_std));
    }

    //*************************************************************************
    TEST(test_to_std)
    {
      char        v1 = 1;
      int         v2 = 2;
      std::string v3 = "Hello";
      std::string v4 = "";

      etl::tuple<char, int, std::string> tp_etl(v1, v2, v3);

      auto tp_std = etl::to_std(tp_etl);
      auto tp_std_moved = etl::to_std(etl::move(tp_etl));

      CHECK_EQUAL(v1, std::get<0>(tp_std));
      CHECK_EQUAL(v2, std::get<1>(tp_std));
      CHECK_EQUAL(v3, std::get<2>(tp_std));

      CHECK_EQUAL(v1, std::get<0>(tp_std_moved));
      CHECK_EQUAL(v2, std::get<1>(tp_std_moved));
      CHECK_EQUAL(v3, std::get<2>(tp_std_moved));

      CHECK_EQUAL(v4, etl::get<2>(tp_etl));
    }
#endif

    //*************************************************************************
    TEST(test_common_type)
    {
      using Tuple = etl::tuple<char, int, double>;
      using Type = etl::common_type<Tuple>::type;

      CHECK_TRUE((std::is_same<double, Type>::value));
    }

#if ETL_USING_CPP17
    //*************************************************************************
    TEST(test_structured_bindings)
    {
      etl::tuple<char, int, std::string> tp(1, 2, "Hello");

      const auto &[c, i, s] = tp;

      CHECK_EQUAL(etl::get<0>(tp), c);
      CHECK_EQUAL(etl::get<1>(tp), i);
      CHECK_EQUAL(etl::get<2>(tp), s);
    }
#endif
  }
}
