/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl
https://www.etlcpp.com

Copyright(c) 2020 jwellbelove

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

#include "UnitTest++/UnitTest++.h"

#include "etl/span.h"
#include "etl/array.h"

#include <array>
#include <vector>
#include <algorithm>
#include <iterator>

namespace
{
  SUITE(test_span)
  {
    static const size_t SIZE = 10;

    typedef etl::array<int, SIZE> EtlData;
    typedef std::array<int, SIZE> StlData;
    typedef std::vector<int> StlVData;

    typedef etl::span<int> View;
    typedef etl::span<const int> CView;

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

      CHECK_EQUAL(0U, view.size());
      CHECK_EQUAL(0U, view.max_size());
      CHECK(view.empty());
    }

    //*************************************************************************
    TEST(test_constructor_etl_array_1)
    {
      View view(etldata);

      CHECK_EQUAL(etldata.size(), view.size());
      CHECK_EQUAL(etldata.max_size(), view.max_size());

      bool isEqual = std::equal(view.begin(), view.end(), etldata.begin());
      CHECK(isEqual);
    }

    //*************************************************************************
    TEST(test_constructor_etl_array_1_const)
    {
      CView view(cetldata);

      CHECK_EQUAL(cetldata.size(), view.size());
      CHECK_EQUAL(cetldata.max_size(), view.max_size());

      bool isEqual = std::equal(view.begin(), view.end(), cetldata.begin());
      CHECK(isEqual);
    }

    //*************************************************************************
    TEST(test_constructor_etl_array_2)
    {
      View view(etldata.begin(), etldata.end());

      CHECK_EQUAL(SIZE, view.size());
      CHECK_EQUAL(SIZE, view.max_size());

      bool isEqual = std::equal(view.begin(), view.end(), etldata.begin());
      CHECK(isEqual);
    }

    //*************************************************************************
    TEST(test_constructor_etl_array_2_const)
    {
      CView view(cetldata.begin(), cetldata.end());

      CHECK_EQUAL(SIZE, view.size());
      CHECK_EQUAL(SIZE, view.max_size());

      bool isEqual = std::equal(view.begin(), view.end(), cetldata.begin());
      CHECK(isEqual);
    }

    //*************************************************************************
    TEST(test_constructor_stl_array_1)
    {
      View view(stldata);

      CHECK_EQUAL(stldata.size(), view.size());
      CHECK_EQUAL(stldata.max_size(), view.max_size());

      bool isEqual = std::equal(view.begin(), view.end(), stldata.begin());
      CHECK(isEqual);
    }

    //*************************************************************************
    TEST(test_constructor_stl_array_1_const)
    {
      CView view(cstldata);

      CHECK_EQUAL(cstldata.size(), view.size());
      CHECK_EQUAL(cstldata.max_size(), view.max_size());

      bool isEqual = std::equal(view.begin(), view.end(), cstldata.begin());
      CHECK(isEqual);
    }

    //*************************************************************************
    TEST(test_constructor_stl_array_2)
    {
      View view(stldata.begin(), stldata.end());

      CHECK_EQUAL(stldata.size(), view.size());
      CHECK_EQUAL(stldata.max_size(), view.max_size());

      bool isEqual = std::equal(view.begin(), view.end(), stldata.begin());
      CHECK(isEqual);
    }

    //*************************************************************************
    TEST(test_constructor_stl_array_2_const)
    {
      CView view(cstldata.begin(), cstldata.end());

      CHECK_EQUAL(cstldata.size(), view.size());
      CHECK_EQUAL(cstldata.max_size(), view.max_size());

      bool isEqual = std::equal(view.begin(), view.end(), cstldata.begin());
      CHECK(isEqual);
    }

    //*************************************************************************
    TEST(test_constructor_stl_vector_1)
    {
      View view(stlvdata);

      CHECK_EQUAL(stlvdata.size(), view.size());
      CHECK_EQUAL(stlvdata.size(), view.max_size());

      bool isEqual = std::equal(view.begin(), view.end(), stlvdata.begin());
      CHECK(isEqual);
    }

    //*************************************************************************
    TEST(test_constructor_stl_vector_1_const)
    {
      CView view(cstlvdata);

      CHECK_EQUAL(cstlvdata.size(), view.size());
      CHECK_EQUAL(cstlvdata.size(), view.max_size());

      bool isEqual = std::equal(view.begin(), view.end(), cstlvdata.begin());
      CHECK(isEqual);
    }

    //*************************************************************************
    TEST(test_constructor_c_array_1)
    {
      View view(pcdata, SIZE);

      CHECK_EQUAL(SIZE, view.size());
      CHECK_EQUAL(SIZE, view.max_size());

      bool isEqual = std::equal(view.begin(), view.end(), pcdata);
      CHECK(isEqual);
    }

    //*************************************************************************
    TEST(test_constructor_c_array_1_const)
    {
      CView view(pccdata, SIZE);

      CHECK_EQUAL(SIZE, view.size());
      CHECK_EQUAL(SIZE, view.max_size());

      bool isEqual = std::equal(view.begin(), view.end(), pccdata);
      CHECK(isEqual);
    }

    //*************************************************************************
    TEST(test_constructor_c_array_2)
    {
      View view(cdata);

      CHECK_EQUAL(SIZE, view.size());
      CHECK_EQUAL(SIZE, view.max_size());

      bool isEqual = std::equal(view.begin(), view.end(), cdata);
      CHECK(isEqual);
    }

    //*************************************************************************
    TEST(test_constructor_c_array_2_const)
    {
      CView view(ccdata);

      CHECK_EQUAL(SIZE, view.size());
      CHECK_EQUAL(SIZE, view.max_size());

      bool isEqual = std::equal(view.begin(), view.end(), ccdata);
      CHECK(isEqual);
    }

#if ETL_USING_STL && !defined(ETL_TEMPLATE_DEDUCTION_GUIDE_TESTS_DISABLED)
    //*************************************************************************
    TEST(test_cpp17_deduced_constructor)
    {
      etl::array data{ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };

      etl::span span1{ data };
      etl::span span2{ data.begin(), data.end() };
      etl::span span3{ data.begin(), data.size() };
      etl::span span4{ span1 };

      int c_array[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
      etl::span span5{ c_array };

      bool isEqual = false;

      isEqual = std::equal(span1.begin(), span1.end(), data.begin());
      CHECK(isEqual);

      isEqual = std::equal(span2.begin(), span2.end(), data.begin());
      CHECK(isEqual);

      isEqual = std::equal(span3.begin(), span3.end(), data.begin());
      CHECK(isEqual);

      isEqual = std::equal(span4.begin(), span4.end(), data.begin());
      CHECK(isEqual);

      isEqual = std::equal(span5.begin(), span5.end(), c_array);
      CHECK(isEqual);
    }
#endif

    //*************************************************************************
    TEST(test_constructor_range)
    {
      View view(etldata.begin() + 2, etldata.end() - 2);

      CHECK_EQUAL(etldata.size() - 4, view.size());
      CHECK_EQUAL(etldata.size() - 4, view.max_size());

      bool isEqual = std::equal(view.begin(), view.end(), etldata.begin() + 2);
      CHECK(isEqual);
    }

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

      isEqual = std::equal(view1.begin(), view1.end(), view2.begin());
      CHECK(isEqual);

      isEqual = std::equal(cview1.begin(), cview1.end(), cview2.begin());
      CHECK(isEqual);
    }

    //*************************************************************************
    TEST(test_modify_range)
    {
      View view(etloriginal.begin() + 2, etloriginal.end() - 2);

      CHECK_EQUAL(etloriginal.size() - 4, view.size());
      CHECK_EQUAL(etloriginal.size() - 4, view.max_size());

      std::fill(view.begin(), view.end(), 10);

      bool isEqual;

      isEqual = std::equal(etloriginal.begin(), etloriginal.end(), etlmodified.begin());
      CHECK(isEqual);

      isEqual = std::equal(etldata.begin(), etldata.end(), etlmodified.begin());
      CHECK(!isEqual);
    }

    //*************************************************************************
    TEST(test_begin_end)
    {
      View  view(etldata.begin(), etldata.end());
      CView cview(etldata.begin(), etldata.end());

      CHECK_EQUAL(etldata.begin(), view.begin());
      CHECK_EQUAL(etldata.begin(), cview.begin());

      CHECK_EQUAL(etldata.end(), view.rbegin().base());
      CHECK_EQUAL(etldata.end(), cview.rbegin().base());

      CHECK_EQUAL(etldata.end(), view.end());
      CHECK_EQUAL(etldata.end(), cview.end());

      CHECK_EQUAL(etldata.begin(), view.rend().base());
      CHECK_EQUAL(etldata.begin(), cview.rend().base());
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

      for (size_t i = 0; i < etldata.size(); ++i)
      {
        CHECK_EQUAL(etldata[i], view[i]);
        CHECK_EQUAL(etldata[i], cview[i]);
      }
    }

    //*************************************************************************
    TEST(test_empty)
    {
      View view1(etldata.begin(), etldata.begin());
      CHECK(view1.empty());

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

      isEqual = std::equal(view1.begin(), view1.end(), etldatasmaller.begin());
      CHECK(isEqual);

      isEqual = std::equal(view2.begin(), view2.end(), etldata.begin());
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
      isEqual = std::equal(result.begin(), result.end(), first.begin());
      CHECK(isEqual);
      CHECK_EQUAL(first.size(), result.extent);
      CHECK_EQUAL(first.size(), result.size());

      auto cresult = cview.first<6>();
      isEqual = std::equal(cresult.begin(), cresult.end(), first.begin());
      CHECK(isEqual);
      CHECK_EQUAL(first.size(), cresult.extent);
      CHECK_EQUAL(first.size(), cresult.size());
    }

    //*************************************************************************
    TEST(test_last)
    {
      std::vector<int> original = { 1, 2, 3, 4, 5, 6, 7, 8 };
      std::vector<int> last     = { 3, 4, 5, 6, 7, 8 };
      View view(original);
      CView cview(original);

      bool isEqual;

      auto result = view.last<6>();
      isEqual = std::equal(result.begin(), result.end(), last.begin());
      CHECK(isEqual);
      CHECK_EQUAL(last.size(), result.extent);
      CHECK_EQUAL(last.size(), result.size());

      auto cresult = cview.last<6>();
      isEqual = std::equal(cresult.begin(), cresult.end(), last.begin());
      CHECK(isEqual);
      CHECK_EQUAL(last.size(), cresult.extent);
      CHECK_EQUAL(last.size(), cresult.size());
    }

    //*************************************************************************
    TEST(test_subspan)
    {
      std::vector<int> original = { 1, 2, 3, 4, 5, 6, 7, 8 };
      std::vector<int> sub1 = { 3, 4, 5, 6 };
      std::vector<int> sub2 = { 3, 4, 5, 6, 7, 8 };
      View view(original);
      CView cview(original);

      bool isEqual;

      auto span1 = view.subspan<2, 4>();
      isEqual = std::equal(span1.begin(), span1.end(), sub1.begin());
      CHECK(isEqual);
      CHECK_EQUAL(sub1.size(), span1.extent);
      CHECK_EQUAL(sub1.size(), span1.size());

      auto cspan1 = cview.subspan<2, 4>();
      isEqual = std::equal(cspan1.begin(), cspan1.end(), sub1.begin());
      CHECK(isEqual);
      CHECK_EQUAL(sub1.size(), cspan1.extent);
      CHECK_EQUAL(sub1.size(), cspan1.size());

      auto span2 = view.subspan<2>();
      isEqual = std::equal(span2.begin(), span2.end(), sub2.begin());
      CHECK(isEqual);
      CHECK_EQUAL(etl::dynamic_extent, span2.extent);
      CHECK_EQUAL(sub2.size(), span2.size());

      auto cspan2 = cview.subspan<2>();
      isEqual = std::equal(cspan2.begin(), cspan2.end(), sub2.begin());
      CHECK(isEqual);
      CHECK_EQUAL(etl::dynamic_extent, cspan2.extent);
      CHECK_EQUAL(sub2.size(), cspan2.size());

      auto span3 = view.subspan(2, 4);
      isEqual = std::equal(span3.begin(), span3.end(), sub1.begin());
      CHECK(isEqual);
      CHECK_EQUAL(etl::dynamic_extent, span3.extent);
      CHECK_EQUAL(sub1.size(), span3.size());

      auto cspan3 = cview.subspan(2, 4);
      isEqual = std::equal(cspan3.begin(), cspan3.end(), sub1.begin());
      CHECK(isEqual);
      CHECK_EQUAL(etl::dynamic_extent, cspan3.extent);
      CHECK_EQUAL(sub1.size(), cspan3.size());

      auto span4 = view.subspan(2);
      isEqual = std::equal(span4.begin(), span4.end(), sub2.begin());
      CHECK(isEqual);
      CHECK_EQUAL(etl::dynamic_extent, span4.extent);
      CHECK_EQUAL(sub2.size(), span4.size());

      auto cspan4 = cview.subspan(2);
      isEqual = std::equal(cspan4.begin(), cspan4.end(), sub2.begin());
      CHECK(isEqual);
      CHECK_EQUAL(etl::dynamic_extent, cspan4.extent);
      CHECK_EQUAL(sub2.size(), cspan4.size());

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
  };
}
