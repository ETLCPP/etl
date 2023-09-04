/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl
https://www.etlcpp.com

Copyright(c) 2020 John Wellbelove

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

#include "etl/span.h"
#include "etl/array.h"

#include <array>
#include <vector>
#include <algorithm>
#include <iterator>
#if ETL_USING_CPP20
  #include <span>
#endif

namespace
{
  SUITE(test_span_fixed_extent)
  {
    static const size_t SIZE = 10UL;

    typedef etl::array<int, SIZE> EtlData;
    typedef std::array<int, SIZE> StlData;
    typedef std::vector<int> StlVData;

    typedef etl::span<int, 10U> View;
    typedef etl::span<int, 9U> SView;
    typedef etl::span<const int, 10U> CView;

#if ETL_USING_CPP20
    using StdView = std::span<int, 10U>;
#endif

    EtlData etldata = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
    StlData stldata = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
    StlVData stlvdata = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };

    EtlData etldatasmaller = { 0, 1, 2, 3, 4, 5, 5, 7, 8, 9 };

    EtlData etloriginal = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
    EtlData etlmodified = { 10, 10, 10, 10, 10, 10, 10, 10, 10, 10 };

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

#if ETL_USING_CPP20
    //*************************************************************************
    TEST(test_construct_from_std_span)
    {
      StdView stdview(stldata);
      View view(stdview);

      CHECK_EQUAL(stdview.size(), view.size());
      CHECK_EQUAL(stdview.size(), view.size());

      bool isEqual = std::equal(view.begin(), view.end(), stdview.begin());
      CHECK(isEqual);
    }
#endif

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

    //*************************************************************************
    TEST(test_implicit_constructor_c_array_2)
    {
      CView view;

      view = ccdata;

      CHECK_EQUAL(SIZE, view.size());
      CHECK_EQUAL(SIZE, view.max_size());

      bool isEqual = std::equal(view.begin(), view.end(), ccdata);
      CHECK(isEqual);
    }

#if ETL_USING_CPP17 && ETL_HAS_INITIALIZER_LIST && !defined(ETL_TEMPLATE_DEDUCTION_GUIDE_TESTS_DISABLED)
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
      View view(etldata.begin(), etldata.end());

      CHECK_EQUAL(etldata.size(), view.size());
      CHECK_EQUAL(etldata.size(), view.max_size());

      bool isEqual = std::equal(view.begin(), view.end(), etldata.begin());
      CHECK(isEqual);
    }

    //*************************************************************************
    TEST(test_copy_constructor_from_same_span_type)
    {
      etl::array<char, 10> data = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };

      const etl::span<char, 10> view1(data.data(), data.data() + data.size());
      etl::span<char, 10> view2(view1);

      CHECK_EQUAL(data.size(), view1.size());
      CHECK_EQUAL(data.size(), view2.size());

      bool isEqual = std::equal(view1.begin(), view1.end(), view2.begin());
      CHECK(isEqual);
    }

    //*************************************************************************
    TEST(test_copy_constructor_from_different_span_type)
    {
      etl::array<char, 10> data = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };

      const etl::span<char, 10> view1(data.data(), data.data() + data.size());
      etl::span<const char, 10> view2(view1);

      CHECK_EQUAL(data.size(), view1.size());
      CHECK_EQUAL(data.size(), view2.size());

      bool isEqual = std::equal(view1.begin(), view1.end(), view2.begin());
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
      View view(etloriginal.begin(), etloriginal.end());

      CHECK_EQUAL(etloriginal.size(), view.size());
      CHECK_EQUAL(etloriginal.size(), view.max_size());

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

      bool isEqual = std::equal(cview.begin(), cview.end(), etldata.begin());
      CHECK(isEqual);
    }

    //*************************************************************************
    TEST(test_empty)
    {
      View view1(etldata.begin(), etldata.begin());
      CHECK(!view1.empty());
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
      std::vector<int> original = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
      std::vector<int> first    = { 0, 1, 2, 3, 4, 5 };
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
    TEST(test_first_2)
    {
      std::vector<int> original = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
      std::vector<int> first = { 0, 1, 2, 3, 4, 5 };
      View view(original);
      CView cview(original);

      bool isEqual;

      auto result = view.first(6);
      isEqual = std::equal(result.begin(), result.end(), first.begin());
      CHECK(isEqual);
      CHECK_EQUAL(first.size(), result.size());

      auto cresult = cview.first(6);
      isEqual = std::equal(cresult.begin(), cresult.end(), first.begin());
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
    TEST(test_last_2)
    {
      std::vector<int> original = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
      std::vector<int> last = { 4, 5, 6, 7, 8, 9 };
      View view(original);
      CView cview(original);

      bool isEqual;

      auto result = view.last(6);
      isEqual = std::equal(result.begin(), result.end(), last.begin());
      CHECK(isEqual);
      CHECK_EQUAL(last.size(), result.size());

      auto cresult = cview.last(6);
      isEqual = std::equal(cresult.begin(), cresult.end(), last.begin());
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
      isEqual = std::equal(sub2.begin(), sub2.end(), span2.begin());
      CHECK(isEqual);
      CHECK_EQUAL(span2.size(), span2.extent);

      auto cspan2 = cview.subspan<2>();
      isEqual = std::equal(sub2.begin(), sub2.end(), cspan2.begin());
      CHECK(isEqual);
      CHECK_EQUAL(original.size() - 2U, cspan2.extent);

      auto span3 = view.subspan(2, 4);
      isEqual = std::equal(sub1.begin(), sub1.end(), span3.begin());
      CHECK(isEqual);
      CHECK_EQUAL(etl::dynamic_extent, span3.extent);

      auto cspan3 = cview.subspan(2, 4);
      isEqual = std::equal(sub1.begin(), sub1.end(), cspan3.begin());
      CHECK(isEqual);
      CHECK_EQUAL(etl::dynamic_extent, cspan3.extent);

      auto span4 = view.subspan(2);
      isEqual = std::equal(sub2.begin(), sub2.end(), span4.begin());
      CHECK(isEqual);
      CHECK_EQUAL(etl::dynamic_extent, span4.extent);

      auto cspan4 = cview.subspan(2);
      isEqual = std::equal(sub2.begin(), sub2.end(), cspan4.begin());
      CHECK(isEqual);
      CHECK_EQUAL(etl::dynamic_extent, cspan4.extent);
    }

    //*************************************************************************
    TEST(test_hash)
    {
      View  view(etldata.begin(), etldata.end());
      CView cview(etldata.begin(), etldata.end());

      size_t hashdata = etl::private_hash::generic_hash<size_t>(reinterpret_cast<const uint8_t*>(etldata.data()),
                                                                reinterpret_cast<const uint8_t*>(etldata.data() + etldata.size()));

      size_t hashview  = etl::hash<View>()(view);
      size_t hashcview = etl::hash<CView>()(cview);

      CHECK_EQUAL(hashdata, hashview);
      CHECK_EQUAL(hashdata, hashcview);
    }

#if ETL_USING_CPP17
    //*************************************************************************
    TEST(test_template_deduction_guide_for_c_array)
    {
      int data[] = { 1, 2, 3, 4 };

      etl::span s = data;

      CHECK_EQUAL(ETL_OR_STD17::size(data), s.extent);
      CHECK_EQUAL(ETL_OR_STD17::size(data), s.size());
      CHECK((std::is_same_v<int, std::remove_reference_t<decltype(s.front())>>));
    }

#if ETL_USING_STL
    //*************************************************************************
    TEST(test_template_deduction_guide_for_std_array)
    {
      std::array<int, 4U> data = { 1, 2, 3, 4 };

      etl::span s = data;

      CHECK_EQUAL(ETL_OR_STD17::size(data), s.extent);
      CHECK_EQUAL(ETL_OR_STD17::size(data), s.size());
      CHECK((std::is_same_v<int, std::remove_reference_t<decltype(s.front())>>));
    }
#endif

    //*************************************************************************
    TEST(test_template_deduction_guide_for_etl_array)
    {
      etl::array<int, 4U> data = { 1, 2, 3, 4 };

      etl::span s = data;

      CHECK_EQUAL(ETL_OR_STD17::size(data), s.extent);
      CHECK_EQUAL(ETL_OR_STD17::size(data), s.size());
      CHECK((std::is_same_v<int, std::remove_reference_t<decltype(s.front())>>));
    }

    //*************************************************************************
    TEST(test_template_deduction_guide_for_iterators)
    {
      etl::array<int, 4U> data = { 1, 2, 3, 4 };

      etl::span s{ data.begin(), data.end() };

      CHECK_EQUAL(etl::dynamic_extent, s.extent);
      CHECK_EQUAL(4U, s.size());
      CHECK((std::is_same_v<int, std::remove_reference_t<decltype(s.front())>>));
    }

    //*************************************************************************
    TEST(test_template_deduction_guide_for_iterator_and_size)
    {
      etl::array<int, 4U> data = { 1, 2, 3, 4 };

      etl::span s{ data.begin(), data.size() };

      CHECK_EQUAL(etl::dynamic_extent, s.extent);
      CHECK_EQUAL(4U, s.size());
      CHECK((std::is_same_v<int, std::remove_reference_t<decltype(s.front())>>));
    }
#endif

    //*************************************************************************
    void f_issue_481(etl::span<const char, 10>)
    {
    }

    TEST(test_issue_481)
    {
      // Should compile.
      char c[10];
      f_issue_481(c);
    }

    //*************************************************************************
#include "etl/private/diagnostic_unused_function_push.h"
    
    struct C_issue_482 {};

    void f_issue_482(etl::span<char>)
    {
    }

    void f_issue_482(etl::span<C_issue_482>)
    {
    }

    TEST(test_issue_482)
    {
      etl::array<C_issue_482, 10> c;

      // Should compile without ambiguous function error.
      f_issue_482(c);
    }

    //*************************************************************************
    void f_issue_482_2(etl::span<char, 10>)
    {
    }

    void f_issue_482_2(etl::span<C_issue_482, 10>)
    {
    }

    TEST(test_issue_482_2)
    {
      etl::array<C_issue_482, 10> c;

      // Should compile without ambiguous function error.
      f_issue_482(c);
    }

    //*************************************************************************
    void f_issue_486(etl::span<const char, 11>)
    {
    }

    TEST(test_issue_486)
    {
      //std::array<char, 10> c;

      // Should not compile.
      //etl::span<char, 11> value(c);

      // Should not compile.
      //f_issue_486(c);
    }

    //*************************************************************************
    TEST(test_circular_iterator_pre_increment)
    {
      etl::array<int, 10> data{ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
      etl::array<int, 20> expected{ 1, 2, 3, 4, 5, 6, 7, 8, 9, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 0 };

      View view{ data };

      View::circular_iterator sci = view.begin_circular();

      for (int i = 0; i < 20; ++i)
      {
        CHECK_EQUAL(expected[i], *++sci);
      }
    }

    //*************************************************************************
    TEST(test_circular_iterator_pre_increment_for_subspan)
    {
      etl::array<int, 10> data{ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
      etl::array<int, 20> expected{ 3, 4, 5, 2, 3, 4, 5, 2, 3, 4, 5, 2, 3, 4, 5, 2, 3, 4, 5, 2 };

      View view{ data };
      etl::span<int, 4U> subspan = view.subspan<2, 4>();

      View::circular_iterator sci = subspan.begin_circular();

      for (int i = 0; i < 20; ++i)
      {
        CHECK_EQUAL(expected[i], *++sci);
      }
    }

    //*************************************************************************
    TEST(test_circular_iterator_post_increment)
    {
      etl::array<int, 10> data{ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
      etl::array<int, 20> expected{ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };

      View view{ data };

      View::circular_iterator sci = view.begin_circular();

      for (int i = 0; i < 20; ++i)
      {
        CHECK_EQUAL(expected[i % ETL_OR_STD17::size(expected)], *sci++);
      }
    }

    //*************************************************************************
    TEST(test_circular_iterator_post_increment_for_subspan)
    {
      etl::array<int, 10> data{ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
      etl::array<int, 20> expected{ 2, 3, 4, 5, 2, 3, 4, 5, 2, 3, 4, 5, 2, 3, 4, 5, 2, 3, 4, 5 };

      View view{ data };
      etl::span<int, 4U> subspan = view.subspan<2, 4>();

      View::circular_iterator sci = subspan.begin_circular();

      for (int i = 0; i < 20; ++i)
      {
        CHECK_EQUAL(expected[i], *sci++);
      }
    }

    //*************************************************************************
    TEST(test_circular_reverse_iterator_pre_increment)
    {
      etl::array<int, 10> data{ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
      etl::array<int, 20> expected{ 8, 7, 6, 5, 4, 3, 2, 1, 0, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0, 9 };

      View view{ data };

      View::reverse_circular_iterator sci = view.rbegin_circular();

      for (int i = 0; i < 20; ++i)
      {
        CHECK_EQUAL(expected[i % ETL_OR_STD17::size(expected)], *++sci);
      }
    }

    //*************************************************************************
    TEST(test_circular_reverse_iterator_pre_increment_for_subspan)
    {
      etl::array<int, 10> data{ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
      etl::array<int, 20> expected{ 4, 3, 2, 5, 4, 3, 2, 5, 4, 3, 2, 5, 4, 3, 2, 5, 4, 3, 2, 5 };

      View view{ data };
      etl::span<int, 4U> subspan = view.subspan<2, 4>();

      etl::span<int, 10U>::reverse_circular_iterator sci = subspan.rbegin_circular();

      for (int i = 0; i < 20; ++i)
      {
        CHECK_EQUAL(expected[i % ETL_OR_STD17::size(expected)], *++sci);
      }
    }

    //*************************************************************************
    TEST(test_circular_reverse_iterator_post_increment)
    {
      etl::array<int, 10> data{ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
      etl::array<int, 20> expected{ 9, 8, 7, 6, 5, 4, 3, 2, 1, 0, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0 };

      View view{ data };

      View::reverse_circular_iterator sci = view.rbegin_circular();

      for (int i = 0; i < 20; ++i)
      {
        CHECK_EQUAL(expected[i % ETL_OR_STD17::size(expected)], *sci++);
      }
    }

    //*************************************************************************
    TEST(test_circular_reverse_iterator_post_increment_for_subspan)
    {
      etl::array<int, 10> data{ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
      etl::array<int, 20> expected{ 5, 4, 3, 2, 5, 4, 3, 2, 5, 4, 3, 2, 5, 4, 3, 2, 5, 4, 3, 2 };

      View view{ data };
      etl::span<int, 4U> subspan = view.subspan<2, 4>();

      View::reverse_circular_iterator sci = subspan.rbegin_circular();

      for (int i = 0; i < 20; ++i)
      {
        CHECK_EQUAL(expected[i % ETL_OR_STD17::size(expected)], *sci++);
      }
    }

    //*************************************************************************
    TEST(test_operator_plus_equals)
    {
      etl::array<int, 10> data{ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
      etl::array<int, 20> expected{ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };

      View view{ data };

      for (int step = 1; step < 20; ++step)
      {
        View::circular_iterator sci = view.begin_circular();

        for (int i = 0; i < 20; i += step)
        {
          CHECK_EQUAL(expected[i % 10], *sci);
          sci += step;
        }
      }
    }

    //*************************************************************************
    TEST(test_operator_plus)
    {
      etl::array<int, 10> data{ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
      etl::array<int, 10> expected{ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };

      View view{ data };

      for (int step = 1; step < 20; ++step)
      {
        View::circular_iterator sci = view.begin_circular();

        for (int i = 0; i < 20; i += step)
        {
          CHECK_EQUAL(expected[i % 10], *sci);
          sci = sci + step;
        }
      }
    }

    //*************************************************************************
    TEST(test_operator_minus_equals)
    {
      etl::array<int, 10> data{ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
      etl::array<int, 10> expected{ 0, 9, 8, 7, 6, 5, 4, 3, 2, 1 };

      View view{ data };

      for (int step = 1; step < 20; ++step)
      {
        View::circular_iterator sci = view.begin_circular();

        for (int i = 0; i < 20; i += step)
        {
          CHECK_EQUAL(expected[i % 10], *sci);
          sci -= step;
        }
      }
    }

    //*************************************************************************
    TEST(test_operator_minus)
    {
      etl::array<int, 10> data{ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
      etl::array<int, 10> expected{ 0, 9, 8, 7, 6, 5, 4, 3, 2, 1 };

      View view{ data };

      for (int step = 1; step < 20; ++step)
      {
        View::circular_iterator sci = view.begin_circular();

        for (int i = 0; i < 20; i += step)
        {
          CHECK_EQUAL(expected[i % 10], *sci);
          sci = sci - step;
        }
      }
    }

    //*************************************************************************
    TEST(test_operator_equality)
    {
      etl::array<int, 10> data1{ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
      etl::array<int, 10> data2{ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
      etl::array<int, 10> data3{ 0, 1, 2, 3, 4, 4, 6, 7, 8, 9 };
      etl::array<int, 9>  data4{ 0, 1, 2, 3, 4, 5, 6, 7, 8 };

      View view1{ data1 };
      View view2{ data1 };
      View view3{ data2 };
      View view4{ data3 };
      View view6;
      View view7;
      SView view8{ data4 };

      CHECK_TRUE(etl::equal(view1, view2));
      CHECK_TRUE(etl::equal(view1, view3));
      CHECK_FALSE(etl::equal(view1, view4));
      CHECK_TRUE(etl::equal(view6, view6));
      CHECK_TRUE(etl::equal(view6, view7));
      CHECK_FALSE(etl::equal(view1, view8));

      CHECK_TRUE(view1 == view2);
      CHECK_FALSE(view1 == view3);
      CHECK_FALSE(view1 == view4);
      CHECK_TRUE(view6 == view6);
      CHECK_TRUE(view6 == view7);
      CHECK_FALSE(view1 == view8);
    }

    //*************************************************************************
    TEST(test_operator_equality_one_is_const)
    {
      etl::array<int, 10> data1{ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
      etl::array<int, 10> data2{ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
      etl::array<int, 10> data3{ 0, 1, 2, 3, 4, 4, 6, 7, 8, 9 };

      View view1{ data1 };
      CView view2{ data1 };
      CView view3{ data2 };
      CView view4{ data3 };
      View view6;
      CView view7;

      CHECK_TRUE(etl::equal(view1, view2));
      CHECK_TRUE(etl::equal(view1, view3));
      CHECK_FALSE(etl::equal(view1, view4));
      CHECK_TRUE(etl::equal(view6, view6));
      CHECK_TRUE(etl::equal(view6, view7));

      CHECK_TRUE(view1 == view2);
      CHECK_FALSE(view1 == view3);
      CHECK_FALSE(view1 == view4);
      CHECK_TRUE(view6 == view6);
      CHECK_TRUE(view6 == view7);
    }

    //*************************************************************************
    TEST(test_operator_not_equal)
    {
      etl::array<int, 10> data1{ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
      etl::array<int, 10> data2{ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
      etl::array<int, 10> data3{ 0, 1, 2, 3, 4, 4, 6, 7, 8, 9 };

      View view1{ data1 };
      View view2{ data1 };
      View view3{ data2 };
      View view4{ data3 };
      View view6;
      View view7;

      CHECK_TRUE(etl::equal(view1, view2));
      CHECK_TRUE(etl::equal(view1, view3));
      CHECK_FALSE(etl::equal(view1, view4));
      CHECK_TRUE(etl::equal(view6, view6));
      CHECK_TRUE(etl::equal(view6, view7));

      CHECK_FALSE(view1 != view2);
      CHECK_TRUE(view1 != view3);
      CHECK_TRUE(view1 != view4);
      CHECK_FALSE(view6 != view6);
      CHECK_FALSE(view6 != view7);
    }

    //*************************************************************************
    TEST(test_operator_not_equal_one_is_const)
    {
      etl::array<int, 10> data1{ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
      etl::array<int, 10> data2{ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
      etl::array<int, 10> data3{ 0, 1, 2, 3, 4, 4, 6, 7, 8, 9 };

      View view1{ data1 };
      CView view2{ data1 };
      CView view3{ data2 };
      CView view4{ data3 };
      View view6;
      View view7;

      CHECK_TRUE(etl::equal(view1, view2));
      CHECK_TRUE(etl::equal(view1, view3));
      CHECK_FALSE(etl::equal(view1, view4));
      CHECK_TRUE(etl::equal(view6, view6));
      CHECK_TRUE(etl::equal(view6, view7));

      CHECK_FALSE(view1 != view2);
      CHECK_TRUE(view1 != view3);
      CHECK_TRUE(view1 != view4);
      CHECK_FALSE(view6 != view6);
      CHECK_FALSE(view6 != view7);
    }

    //*************************************************************************
    TEST(test_convert_span_any_to_span_byte)
    {
      float data[2]{3.141592f, 2.71828f};

#if ETL_USING_CPP17
      auto const const_bytes    = etl::as_bytes(etl::span{ data });
      auto const writable_bytes = etl::as_writable_bytes(etl::span{ data });
#else
      auto const const_bytes    = etl::as_bytes(etl::span<float, 2>(data));
      auto const writable_bytes = etl::as_writable_bytes(etl::span<float, 2>(data));
#endif

      CHECK_EQUAL(const_bytes.size(),    sizeof(data));
      CHECK_EQUAL(writable_bytes.size(), sizeof(data));
    }

#include "etl/private/diagnostic_pop.h"
  };
}
