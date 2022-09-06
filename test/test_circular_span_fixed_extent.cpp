/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl
https://www.etlcpp.com

Copyright(c) 2022 John Wellbelove

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

#include "etl/circular_span.h"
#include "etl/array.h"

#include <array>
#include <vector>
#include <algorithm>
#include <iterator>

namespace
{
  SUITE(test_circular_span_fixed_extent)
  {
    static const size_t SIZE = 10UL;

    typedef etl::array<int, SIZE> EtlData;
    typedef std::array<int, SIZE> StlData;
    typedef std::vector<int> StlVData;

    typedef etl::circular_span<int, 10U> View;
    typedef etl::circular_span<const int, 10U> CView;

    EtlData etldata = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
    StlData stldata = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
    StlVData stlvdata = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };

    EtlData etldatasmaller = { 0, 1, 2, 3, 4, 5, 5, 7, 8, 9 };

    EtlData etloriginal = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
    EtlData etlmodified = { 0, 1, 10, 10, 10, 10, 10, 10, 8, 9 };

    const EtlData cetldata = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
    const StlData cstldata = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
    const StlVData cstlvdata = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };

    int cdata[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
    int* pcdata = cdata;

    const int ccdata[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
    const int* pccdata = ccdata;

    //*************************************************************************
    TEST(test_default_constructor)
    {
      View view;

      CHECK_EQUAL(10U, view.size());
      CHECK_EQUAL(10U, view.max_size());
      CHECK(!view.empty());
    }

    //*************************************************************************
    TEST(test_constructor_etl_array_1)
    {
      View view(etldata);

      CHECK_EQUAL(etldata.size(), view.size());
      CHECK_EQUAL(etldata.max_size(), view.max_size());

      bool isEqual = std::equal(view.span_begin(), view.span_end(), etldata.begin());
      CHECK(isEqual);
    }

    //*************************************************************************
    TEST(test_constructor_etl_array_1_const)
    {
      CView view(cetldata);

      CHECK_EQUAL(cetldata.size(), view.size());
      CHECK_EQUAL(cetldata.max_size(), view.max_size());

      bool isEqual = std::equal(view.span_begin(), view.span_end(), cetldata.begin());
      CHECK(isEqual);
    }

    //*************************************************************************
    TEST(test_constructor_etl_array_2)
    {
      View view(etldata.begin(), etldata.end());

      CHECK_EQUAL(SIZE, view.size());
      CHECK_EQUAL(SIZE, view.max_size());

      bool isEqual = std::equal(view.span_begin(), view.span_end(), etldata.begin());
      CHECK(isEqual);
    }

    //*************************************************************************
    TEST(test_constructor_etl_array_2_const)
    {
      CView view(cetldata.begin(), cetldata.end());

      CHECK_EQUAL(SIZE, view.size());
      CHECK_EQUAL(SIZE, view.max_size());

      bool isEqual = std::equal(view.span_begin(), view.span_end(), cetldata.begin());
      CHECK(isEqual);
    }

    //*************************************************************************
    TEST(test_constructor_stl_array_1)
    {
      View view(stldata);

      CHECK_EQUAL(stldata.size(), view.size());
      CHECK_EQUAL(stldata.max_size(), view.max_size());

      bool isEqual = std::equal(view.span_begin(), view.span_end(), stldata.begin());
      CHECK(isEqual);
    }

    //*************************************************************************
    TEST(test_constructor_stl_array_1_const)
    {
      CView view(cstldata);

      CHECK_EQUAL(cstldata.size(), view.size());
      CHECK_EQUAL(cstldata.max_size(), view.max_size());

      bool isEqual = std::equal(view.span_begin(), view.span_end(), cstldata.begin());
      CHECK(isEqual);
    }

    //*************************************************************************
    TEST(test_constructor_stl_array_2)
    {
      View view(stldata.begin(), stldata.end());

      CHECK_EQUAL(stldata.size(), view.size());
      CHECK_EQUAL(stldata.max_size(), view.max_size());

      bool isEqual = std::equal(view.span_begin(), view.span_end(), stldata.begin());
      CHECK(isEqual);
    }

    //*************************************************************************
    TEST(test_constructor_stl_array_2_const)
    {
      CView view(cstldata.begin(), cstldata.end());

      CHECK_EQUAL(cstldata.size(), view.size());
      CHECK_EQUAL(cstldata.max_size(), view.max_size());

      bool isEqual = std::equal(view.span_begin(), view.span_end(), cstldata.begin());
      CHECK(isEqual);
    }

    //*************************************************************************
    TEST(test_constructor_stl_vector_1)
    {
      View view(stlvdata);

      CHECK_EQUAL(stlvdata.size(), view.size());
      CHECK_EQUAL(stlvdata.size(), view.max_size());

      bool isEqual = std::equal(view.span_begin(), view.span_end(), stlvdata.begin());
      CHECK(isEqual);
    }

    //*************************************************************************
    TEST(test_constructor_stl_vector_1_const)
    {
      CView view(cstlvdata);

      CHECK_EQUAL(cstlvdata.size(), view.size());
      CHECK_EQUAL(cstlvdata.size(), view.max_size());

      bool isEqual = std::equal(view.span_begin(), view.span_end(), cstlvdata.begin());
      CHECK(isEqual);
    }

    //*************************************************************************
    TEST(test_constructor_c_array_1)
    {
      View view(pcdata, SIZE);

      CHECK_EQUAL(SIZE, view.size());
      CHECK_EQUAL(SIZE, view.max_size());

      bool isEqual = std::equal(view.span_begin(), view.span_end(), pcdata);
      CHECK(isEqual);
    }

    //*************************************************************************
    TEST(test_constructor_c_array_1_const)
    {
      CView view(pccdata, SIZE);

      CHECK_EQUAL(SIZE, view.size());
      CHECK_EQUAL(SIZE, view.max_size());

      bool isEqual = std::equal(view.span_begin(), view.span_end(), pccdata);
      CHECK(isEqual);
    }

    //*************************************************************************
    TEST(test_constructor_c_array_2)
    {
      View view(cdata);

      CHECK_EQUAL(SIZE, view.size());
      CHECK_EQUAL(SIZE, view.max_size());

      bool isEqual = std::equal(view.span_begin(), view.span_end(), cdata);
      CHECK(isEqual);
    }

    //*************************************************************************
    TEST(test_constructor_c_array_2_const)
    {
      CView view(ccdata);

      CHECK_EQUAL(SIZE, view.size());
      CHECK_EQUAL(SIZE, view.max_size());

      bool isEqual = std::equal(view.span_begin(), view.span_end(), ccdata);
      CHECK(isEqual);
    }

    //*************************************************************************
    TEST(test_implicit_constructor_c_array_2)
    {
      CView view;

      view = ccdata;

      CHECK_EQUAL(SIZE, view.size());
      CHECK_EQUAL(SIZE, view.max_size());

      bool isEqual = std::equal(view.span_begin(), view.span_end(), ccdata);
      CHECK(isEqual);
    }

#if ETL_USING_CPP17 && ETL_HAS_INITIALIZER_LIST && !defined(ETL_TEMPLATE_DEDUCTION_GUIDE_TESTS_DISABLED)
    //*************************************************************************
    TEST(test_cpp17_deduced_constructor)
    {
      etl::array data{ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };

      etl::circular_span span1{ data };
      etl::circular_span span2{ data.begin(), data.end() };
      etl::circular_span span3{ data.begin(), data.size() };
      etl::circular_span span4{ span1 };

      int c_array[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
      etl::circular_span span5{ c_array };

      bool isEqual = false;

      isEqual = std::equal(span1.span_begin(), span1.span_end(), data.begin());
      CHECK(isEqual);

      isEqual = std::equal(span2.span_begin(), span2.span_end(), data.begin());
      CHECK(isEqual);

      isEqual = std::equal(span3.span_begin(), span3.span_end(), data.begin());
      CHECK(isEqual);

      isEqual = std::equal(span4.span_begin(), span4.span_end(), data.begin());
      CHECK(isEqual);

      isEqual = std::equal(span5.span_begin(), span5.span_end(), c_array);
      CHECK(isEqual);
    }
#endif

    //*************************************************************************
    TEST(test_assign_from_span)
    {
      View view1(etldata);
      View view2;

      CView cview1(etldata);
      CView cview2;

      view2 = view1;
      cview2 = cview1;

      CHECK_EQUAL(view1.size(), view2.size());
      CHECK_EQUAL(view1.max_size(), view2.max_size());

      CHECK_EQUAL(cview1.size(), cview2.size());
      CHECK_EQUAL(cview1.max_size(), cview2.max_size());

      bool isEqual;

      isEqual = std::equal(view1.span_begin(), view1.span_end(), view2.begin());
      CHECK(isEqual);

      isEqual = std::equal(cview1.span_begin(), cview1.span_end(), cview2.begin());
      CHECK(isEqual);
    }

    //*************************************************************************
    TEST(test_span_begin_span_end)
    {
      View  view(etldata.begin(), etldata.end());
      CView cview(etldata.begin(), etldata.end());

      CHECK_EQUAL(etldata.begin(), view.span_begin());
      CHECK_EQUAL(etldata.begin(), cview.begin());

      CHECK_EQUAL(etldata.end(), view.span_rbegin().base());
      CHECK_EQUAL(etldata.end(), cview.span_rbegin().base());

      CHECK_EQUAL(etldata.end(), view.span_end());
      CHECK_EQUAL(etldata.end(), cview.span_end());

      CHECK_EQUAL(etldata.begin(), view.span_rend().base());
      CHECK_EQUAL(etldata.begin(), cview.span_rend().base());
    }

    //*************************************************************************
    TEST(test_front_back)
    {
      View  view(etldata.begin(), etldata.end());
      CView cview(etldata.begin(), etldata.end());

      CHECK_EQUAL(etldata.front(), view.front());
      CHECK_EQUAL(etldata.front(), cview.front());

      CHECK_EQUAL(etldata.back(), view.back());
      CHECK_EQUAL(etldata.back(), cview.back());
    }

    //*************************************************************************
    TEST(test_data)
    {
      View  view(etldata.begin(), etldata.end());
      CView cview(etldata.begin(), etldata.end());

      CHECK_EQUAL(etldata.data(), view.data());
      CHECK_EQUAL(etldata.data(), cview.data());
    }

    //*************************************************************************
    TEST(test_index_operator)
    {
      View  view(etldata.begin(), etldata.end());
      CView cview(etldata.begin(), etldata.end());

      for (size_t i = 0UL; i < etldata.size(); ++i)
      {
        CHECK_EQUAL(etldata[i], view[i]);
        CHECK_EQUAL(etldata[i], cview[i]);
      }
    }

    //*************************************************************************
    TEST(test_assignment_operator)
    {
      View view(etldata);
      CView cview = view;

      CHECK_EQUAL(etldata.size(), cview.size());
      CHECK_EQUAL(etldata.max_size(), cview.max_size());

      bool isEqual = std::equal(cview.span_begin(), cview.span_end(), etldata.begin());
      CHECK(isEqual);
    }

    //*************************************************************************
    TEST(test_empty)
    {
      View view1(etldata.begin(), etldata.begin());
      CHECK(!view1.empty());

      View view2(etldata.begin(), etldata.begin() + 1);
      CHECK(!view2.empty());
    }

    //*************************************************************************
    TEST(test_size)
    {
      View view(etldata);

      CHECK_EQUAL(etldata.size(), view.size());
    }

    //*************************************************************************
    TEST(test_size_bytes)
    {
      View view(etldata);

      CHECK_EQUAL(etldata.size() * sizeof(EtlData::value_type), view.size_bytes());
    }

    //*************************************************************************
    TEST(test_swap)
    {
      View view1(etldata);
      View view2(etldatasmaller);

      std::swap(view1, view2);

      CHECK_EQUAL(etldata.size(), view1.size());
      CHECK_EQUAL(etldatasmaller.size(), view2.size());

      bool isEqual;

      isEqual = std::equal(view1.span_begin(), view1.span_end(), etldatasmaller.begin());
      CHECK(isEqual);

      isEqual = std::equal(view2.span_begin(), view2.span_end(), etldata.begin());
      CHECK(isEqual);
    }

    //*************************************************************************
    TEST(test_first)
    {
      std::vector<int> original = { 1, 2, 3, 4, 5, 6, 7, 8 };
      std::vector<int> first    = { 1, 2, 3, 4, 5, 6 };
      View view(original);
      CView cview(original);

      bool isEqual;

      auto result = view.first<6>();
      isEqual = std::equal(result.span_begin(), result.span_end(), first.begin());
      CHECK(isEqual);
      CHECK_EQUAL(first.size(), result.extent);
      CHECK_EQUAL(first.size(), result.size());

      auto cresult = cview.first<6>();
      isEqual = std::equal(cresult.span_begin(), cresult.span_end(), first.begin());
      CHECK(isEqual);
      CHECK_EQUAL(first.size(), cresult.extent);
      CHECK_EQUAL(first.size(), cresult.size());
    }

    //*************************************************************************
    TEST(test_first_2)
    {
      std::vector<int> original = {1, 2, 3, 4, 5, 6, 7, 8};
      std::vector<int> first = {1, 2, 3, 4, 5, 6};
      View view(original);
      CView cview(original);

      bool isEqual;

      auto result = view.first(6);
      isEqual = std::equal(result.span_begin(), result.span_end(), first.begin());
      CHECK(isEqual);
      CHECK_EQUAL(first.size(), result.size());

      auto cresult = cview.first(6);
      isEqual = std::equal(cresult.span_begin(), cresult.span_end(), first.begin());
      CHECK(isEqual);
      CHECK_EQUAL(first.size(), cresult.size());
    }

    //*************************************************************************
    TEST(test_last)
    {
      std::vector<int> original = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
      std::vector<int> last     = { 4, 5, 6, 7, 8, 9 };
      View view(original);
      CView cview(original);

      bool isEqual;

      auto result = view.last<6>();
      isEqual = std::equal(result.span_begin(), result.span_end(), last.begin());
      CHECK(isEqual);
      CHECK_EQUAL(last.size(), result.extent);
      CHECK_EQUAL(last.size(), result.size());

      auto cresult = cview.last<6>();
      isEqual = std::equal(cresult.span_begin(), cresult.span_end(), last.begin());
      CHECK(isEqual);
      CHECK_EQUAL(last.size(), cresult.extent);
      CHECK_EQUAL(last.size(), cresult.size());
    }

    //*************************************************************************
    TEST(test_last_2)
    {
      std::vector<int> original = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
      std::vector<int> last     = { 4, 5, 6, 7, 8, 9 };
      View view(original);
      CView cview(original);

      bool isEqual;

      auto result = view.last(6);
      isEqual = std::equal(result.span_begin(), result.span_end(), last.begin());
      CHECK(isEqual);
      CHECK_EQUAL(last.size(), result.size());

      auto cresult = cview.last(6);
      isEqual = std::equal(cresult.span_begin(), cresult.span_end(), last.begin());
      CHECK(isEqual);
      CHECK_EQUAL(last.size(), cresult.size());
    }

    //*************************************************************************
    TEST(test_subspan)
    {
      std::vector<int> original = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
      std::vector<int> sub1 = { 2, 3, 4, 5 };
      std::vector<int> sub2 = { 2, 3, 4, 5, 6, 7 };
      View view(original);
      CView cview(original);

      bool isEqual;

      auto span1 = view.subspan<2, 4>();
      isEqual = std::equal(span1.span_begin(), span1.span_begin() + sub1.size(), sub1.begin());
      CHECK(isEqual);

      auto cspan1 = cview.subspan<2, 4>();
      isEqual = std::equal(cspan1.span_begin(), cspan1.span_begin() + sub1.size(), sub1.begin());
      CHECK(isEqual);

      auto span2 = view.subspan<2>();
      isEqual = std::equal(span2.span_begin(), span2.span_begin() + sub2.size(), sub2.begin());
      CHECK(isEqual);

      auto cspan2 = cview.subspan<2>();
      isEqual = std::equal(cspan2.span_begin(), cspan2.span_begin() + sub2.size(), sub2.begin());
      CHECK(isEqual);

      auto span3 = view.subspan(2, 4);
      isEqual = std::equal(span3.span_begin(), span3.span_begin() + sub1.size(), sub1.begin());
      CHECK(isEqual);

      auto cspan3 = cview.subspan(2, 4);
      isEqual = std::equal(cspan3.span_begin(), cspan3.span_begin() + sub1.size(), sub1.begin());
      CHECK(isEqual);

      auto span4 = view.subspan(2);
      isEqual = std::equal(span4.span_begin(), span4.span_begin() + sub2.size(), sub2.begin());
      CHECK(isEqual);

      auto cspan4 = cview.subspan(2);
      isEqual = std::equal(cspan4.span_begin(), cspan4.span_begin() + sub2.size(), sub2.begin());
      CHECK(isEqual);
    }

    //*************************************************************************
    TEST(test_hash)
    {
      View  view(etldata.begin(), etldata.end());
      CView cview(etldata.begin(), etldata.end());

      size_t hashdata = etl::private_hash::generic_hash<size_t>(reinterpret_cast<const uint8_t*>(&etldata[0]),
                                                                reinterpret_cast<const uint8_t*>(&etldata[etldata.size()]));

      size_t hashview  = etl::hash<View>()(view);
      size_t hashcview = etl::hash<CView>()(cview);

      CHECK_EQUAL(hashdata, hashview);
      CHECK_EQUAL(hashdata, hashcview);
    }

    //*************************************************************************
    TEST(test_template_deduction_guide_for_c_array)
    {
      int data[] = { 1, 2, 3, 4 };

      etl::circular_span s = data;

      CHECK_EQUAL(std::size(data), s.extent);
      CHECK_EQUAL(std::size(data), s.size());
      CHECK((std::is_same_v<int, std::remove_reference_t<decltype(s.front())>>));
    }

#if ETL_USING_STL
    //*************************************************************************
    TEST(test_template_deduction_guide_for_std_array)
    {
      std::array<int, 4U> data = { 1, 2, 3, 4 };

      etl::circular_span s = data;

      CHECK_EQUAL(std::size(data), s.extent);
      CHECK_EQUAL(std::size(data), s.size());
      CHECK((std::is_same_v<int, std::remove_reference_t<decltype(s.front())>>));
    }
#endif

    //*************************************************************************
    TEST(test_template_deduction_guide_for_etl_array)
    {
      etl::array<int, 4U> data = { 1, 2, 3, 4 };

      etl::circular_span s = data;

      CHECK_EQUAL(std::size(data), s.extent);
      CHECK_EQUAL(std::size(data), s.size());
      CHECK((std::is_same_v<int, std::remove_reference_t<decltype(s.front())>>));
    }

    //*************************************************************************
    TEST(test_template_deduction_guide_for_iterators)
    {
      etl::array<int, 4U> data = { 1, 2, 3, 4 };

      etl::circular_span s{ data.begin(), data.end() };

      CHECK_EQUAL(etl::dynamic_extent, s.extent);
      CHECK_EQUAL(4U, s.size());
      CHECK((std::is_same_v<int, std::remove_reference_t<decltype(s.front())>>));
    }

    //*************************************************************************
    TEST(test_template_deduction_guide_for_iterator_and_size)
    {
      etl::array<int, 4U> data = { 1, 2, 3, 4 };

      etl::circular_span s{ data.begin(), data.size() };

      CHECK_EQUAL(etl::dynamic_extent, s.extent);
      CHECK_EQUAL(4U, s.size());
      CHECK((std::is_same_v<int, std::remove_reference_t<decltype(s.front())>>));
    }

    //*************************************************************************
    TEST(test_circular_iterator_pre_increment)
    {
      etl::array data{ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
      etl::array expected{ 1, 2, 3, 4, 5, 6, 7, 8, 9, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 0 };

      etl::circular_span<int, 10U> span{ data };

      etl::circular_span<int, 10U>::iterator csi = span.begin();

      for (int i = 0; i < 20; ++i)
      {
        CHECK_EQUAL(expected[i], *++csi);
      }
    }

    //*************************************************************************
    TEST(test_circular_iterator_pre_increment_for_subspan)
    {
      etl::array data{ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
      etl::array expected{ 3, 4, 5, 2, 3, 4, 5, 2, 3, 4, 5, 2, 3, 4, 5, 2, 3, 4, 5, 2 };

      etl::circular_span<int, 10U> span{ data };
      etl::circular_span<int, 4U> subspan = span.subspan<2, 4>();

      etl::circular_span<int, 10U>::iterator csi = subspan.begin();

      for (int i = 0; i < 20; ++i)
      {
        CHECK_EQUAL(expected[i], *++csi);
      }
    }

    //*************************************************************************
    TEST(test_circular_iterator_post_increment)
    {
      etl::array data{ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
      etl::array expected{ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };

      etl::circular_span<int, 10U> span{ data };

      etl::circular_span<int, 10U>::iterator csi = span.begin();

      for (int i = 0; i < 20; ++i)
      {
        CHECK_EQUAL(expected[i % std::size(expected)], *csi++);
      }
    }

    //*************************************************************************
    TEST(test_circular_iterator_post_increment_for_subspan)
    {
      etl::array data{ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
      etl::array expected{ 2, 3, 4, 5, 2, 3, 4, 5, 2, 3, 4, 5, 2, 3, 4, 5, 2, 3, 4, 5 };

      etl::circular_span<int, 10U> span{ data };
      etl::circular_span<int, 4U> subspan = span.subspan<2, 4>();

      etl::circular_span<int, 10U>::iterator csi = subspan.begin();

      for (int i = 0; i < 20; ++i)
      {
        CHECK_EQUAL(expected[i], *csi++);
      }
    }

    //*************************************************************************
    TEST(test_circular_reverse_iterator_pre_increment)
    {
      etl::array data{ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
      etl::array expected{ 8, 7, 6, 5, 4, 3, 2, 1, 0, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0, 9 };

      etl::circular_span<int, 10U> span{ data };

      etl::circular_span<int, 10U>::reverse_iterator csi = span.rbegin();

      for (int i = 0; i < 20; ++i)
      {
        CHECK_EQUAL(expected[i % std::size(expected)], *++csi);
      }
    }

    //*************************************************************************
    TEST(test_circular_reverse_iterator_pre_increment_for_subspan)
    {
      etl::array data{ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
      etl::array expected{ 4, 3, 2, 5, 4, 3, 2, 5, 4, 3, 2, 5, 4, 3, 2, 5, 4, 3, 2, 5 };

      etl::circular_span<int, 10U> span{ data };
      etl::circular_span<int, 4U> subspan = span.subspan<2, 4>();

      etl::circular_span<int, 10U>::reverse_iterator csi = subspan.rbegin();

      for (int i = 0; i < 20; ++i)
      {
        CHECK_EQUAL(expected[i % std::size(expected)], *++csi);
      }
    }

    //*************************************************************************
    TEST(test_circular_reverse_iterator_post_increment)
    {
      etl::array data{ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
      etl::array expected{ 9, 8, 7, 6, 5, 4, 3, 2, 1, 0, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0 };

      etl::circular_span<int, 10U> span{ data };

      etl::circular_span<int, 10U>::reverse_iterator csi = span.rbegin();

      for (int i = 0; i < 20; ++i)
      {
        CHECK_EQUAL(expected[i % std::size(expected)], *csi++);
      }
    }

    //*************************************************************************
    TEST(test_circular_reverse_iterator_post_increment_for_subspan)
    {
      etl::array data{ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
      etl::array expected{ 5, 4, 3, 2, 5, 4, 3, 2, 5, 4, 3, 2, 5, 4, 3, 2, 5, 4, 3, 2 };

      etl::circular_span<int, 10U> span{ data };
      etl::circular_span<int, 4U> subspan = span.subspan<2, 4>();

      etl::circular_span<int, 10U>::reverse_iterator csi = subspan.rbegin();

      for (int i = 0; i < 20; ++i)
      {
        CHECK_EQUAL(expected[i % std::size(expected)], *csi++);
      }
    }

    //*************************************************************************
    TEST(test_operator_plus_equals)
    {
      etl::array data{ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
      etl::array expected{ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };

      etl::circular_span<int, 10U> span{ data };

      for (int step = 1; step < 20; ++step)
      {
        etl::circular_span<int, 10U>::iterator csi = span.begin();

        for (int i = 0; i < 20; i += step)
        {
          CHECK_EQUAL(expected[i % 10], *csi);
          csi += step;
        }
      }
    }

    //*************************************************************************
    TEST(test_operator_plus)
    {
      etl::array data{ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
      etl::array expected{ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };

      etl::circular_span<int, 10U> span{ data };

      for (int step = 1; step < 20; ++step)
      {
        etl::circular_span<int, 10U>::iterator csi = span.begin();

        for (int i = 0; i < 20; i += step)
        {
          CHECK_EQUAL(expected[i % 10], *csi);
          csi = csi + step;
        }
      }
    }

    //*************************************************************************
    TEST(test_operator_minus_equals)
    {
      etl::array data{ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
      etl::array expected{ 0, 9, 8, 7, 6, 5, 4, 3, 2, 1 };

      etl::circular_span<int, 10U> span{ data };

      for (int step = 1; step < 20; ++step)
      {
        etl::circular_span<int, 10U>::iterator csi = span.begin();

        for (int i = 0; i < 20; i += step)
        {
          CHECK_EQUAL(expected[i % 10], *csi);
          csi -= step;
        }
      }
    }

    //*************************************************************************
    TEST(test_operator_minus)
    {
      etl::array data{ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
      etl::array expected{ 0, 9, 8, 7, 6, 5, 4, 3, 2, 1 };

      etl::circular_span<int, 10U> span{ data };

      for (int step = 1; step < 20; ++step)
      {
        etl::circular_span<int, 10U>::iterator csi = span.begin();

        for (int i = 0; i < 20; i += step)
        {
          CHECK_EQUAL(expected[i % 10], *csi);
          csi = csi - step;
        }
      }
    }
  };
}
