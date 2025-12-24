/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl
https://www.etlcpp.com

Copyright(c) 2017 John Wellbelove

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

#include "etl/array_view.h"
#include "etl/array.h"

#include <array>
#include <vector>
#include <algorithm>
#include <iterator>

namespace
{
  SUITE(test_array_view)
  {
    static const size_t SIZE = 10UL;

    typedef etl::array<int, SIZE> EtlData;
    typedef std::array<int, SIZE> StlData;
    typedef std::vector<int> StlVData;

    typedef etl::array_view<int> View;
    typedef etl::array_view<const int> CView;

    EtlData etldata = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
    StlData stldata = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
    StlVData stlvdata = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };

    EtlData etldatasmaller = { 0, 1, 2, 3, 4, 5, 5, 7, 8, 9 };
    EtlData etldatashorter = { 0, 1, 2, 3, 4, 5, 6, 7, 8 };

    EtlData etloriginal = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
    EtlData etlmodified = { 0, 1, 10, 10, 10, 10, 10, 10, 8, 9 };

    const EtlData cetldata = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
    const StlData cstldata = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
    const StlVData cstlvdata = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };

    const int FillData = -1;
    const EtlData filldata = { -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 };

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
    TEST(test_implicit_constructor_etl_array_1)
    {
      View view = etldata;

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
    TEST(test_implicit_constructor_stl_array_1)
    {
      View view = stldata;

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
    TEST(test_implicit_constructor_c_array)
    {
      View view = cdata;

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
#if ETL_USING_CPP17 && ETL_HAS_INITIALIZER_LIST && !defined(ETL_TEMPLATE_DEDUCTION_GUIDE_TESTS_DISABLED)
    TEST(test_cpp17_deduced_constructor)
    {
      etl::array data{ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };

      etl::array_view view1{ data };
      etl::array_view view2{ data.begin(), data.end() };
      etl::array_view view3{ data.begin(), data.size() };
      etl::array_view view4{ view1 };

      int c_array[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
      etl::array_view view5{ c_array };

      bool isEqual = false;

      isEqual = std::equal(view1.begin(), view1.end(), data.begin());
      CHECK(isEqual);

      isEqual = std::equal(view2.begin(), view2.end(), data.begin());
      CHECK(isEqual);

      isEqual = std::equal(view3.begin(), view3.end(), data.begin());
      CHECK(isEqual);

      isEqual = std::equal(view4.begin(), view4.end(), data.begin());
      CHECK(isEqual);

      isEqual = std::equal(view5.begin(), view5.end(), c_array);
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
    TEST(test_assign_from_array_view)
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
    TEST(test_assign_from_iterator_range)
    {
      View view;
      CView cview;

      view.assign(etldata.begin(), etldata.end());
      cview.assign(cetldata.begin(), cetldata.end());

      CHECK_EQUAL(etldata.size(), view.size());
      CHECK_EQUAL(etldata.max_size(), view.max_size());

      CHECK_EQUAL(cetldata.size(), cview.size());
      CHECK_EQUAL(cetldata.max_size(), cview.max_size());

      bool isEqual;

      isEqual = std::equal(etldata.begin(), etldata.end(), view.begin());
      CHECK(isEqual);

      isEqual = std::equal(cetldata.begin(), cetldata.end(), cview.begin());
      CHECK(isEqual);
    }

    //*************************************************************************
    TEST(test_assign_from_iterator_size)
    {
      View view;
      CView cview;

      view.assign(etldata.begin(), etldata.size());
      cview.assign(cetldata.begin(), cetldata.size());

      CHECK_EQUAL(etldata.size(), view.size());
      CHECK_EQUAL(etldata.max_size(), view.max_size());

      CHECK_EQUAL(cetldata.size(), cview.size());
      CHECK_EQUAL(cetldata.max_size(), cview.max_size());

      bool isEqual;

      isEqual = std::equal(etldata.begin(), etldata.end(), view.begin());
      CHECK(isEqual);

      isEqual = std::equal(cetldata.begin(), cetldata.end(), cview.begin());
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

      CHECK_EQUAL(view.begin(),  view.cbegin());
      CHECK_EQUAL(cview.begin(), cview.cbegin());

      CHECK_EQUAL(view.rbegin().base(),  view.crbegin().base());
      CHECK_EQUAL(cview.rbegin().base(), cview.crbegin().base());

      CHECK_EQUAL(view.end(),  view.cend());
      CHECK_EQUAL(cview.end(), cview.cend());

      CHECK_EQUAL(view.rend().base(),  view.crend().base());
      CHECK_EQUAL(cview.rend().base(), cview.crend().base());
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
    TEST(test_at)
    {
      View  view(etldata.begin(), etldata.end());
      CView cview(etldata.begin(), etldata.end());

      for (size_t i = 0UL; i < etldata.size(); ++i)
      {
        CHECK_EQUAL(etldata[i], view.at(i));
        CHECK_EQUAL(etldata[i], cview.at(i));
      }
    }

    //*************************************************************************
    TEST(test_at_uninitialised_exception)
    {
      View  view;
      CView cview;

      CHECK_THROW(view.at(0),  etl::array_view_uninitialised);
      CHECK_THROW(cview.at(0), etl::array_view_uninitialised);
    }

    //*************************************************************************
    TEST(test_at_bounds_exception)
    {
      View  view(etldata.begin(), etldata.end());
      CView cview(etldata.begin(), etldata.end());

      CHECK_THROW(view.at(view.size()), etl::array_view_bounds);
      CHECK_THROW(cview.at(cview.size()), etl::array_view_bounds);
    }

    //*************************************************************************
    TEST(test_non_member_same)
    {
      View  view1(etldata.begin(), etldata.end());
      View  view2(etldata.begin(), etldata.end());

      CView cview1(etldata.begin(), etldata.end());
      CView cview2(etldata.begin(), etldata.end());

      CHECK(view1 == view2);
      CHECK(cview1 == cview2);

      CHECK(view1 <= view2);
      CHECK(cview1 <= cview2);

      CHECK(view1 >= view2);
      CHECK(cview1 >= cview2);

      CHECK(!(view1 > view2));
      CHECK(!(cview1 > cview2));

      CHECK(!(view1 < view2));
      CHECK(!(cview1 < cview2));
    }

    //*************************************************************************
    TEST(test_non_member_smaller)
    {
      View  view1(etldata.begin(), etldata.end());
      View  view2(etldatasmaller.begin(), etldatasmaller.end());

      CView cview1(etldata.begin(), etldata.end());
      CView cview2(etldatasmaller.begin(), etldatasmaller.end());

      CHECK(!(view1  == view2));
      CHECK(!(cview1 == cview2));

      CHECK(!(view1  <= view2));
      CHECK(!(cview1 <= cview2));

      CHECK(view2  <= view1);
      CHECK(cview2 <= cview1);

      CHECK(view1  >= view2);
      CHECK(cview1 >= cview2);

      CHECK(!(view2  >= view1));
      CHECK(!(cview2 >= cview1));

      CHECK(view1  > view2);
      CHECK(cview1 > cview2);

      CHECK(!(view2  > view1));
      CHECK(!(cview2 > cview1));

      CHECK(!(view1  < view2));
      CHECK(!(cview1 < cview2));

      CHECK(view2  < view1);
      CHECK(cview2 < cview1);
    }

    //*************************************************************************
    TEST(test_non_member_shorter)
    {
      View  view1(etldata.begin(), etldata.end());
      View  view2(etldatashorter.begin(), etldatashorter.end());

      CView cview1(etldata.begin(), etldata.end());
      CView cview2(etldatashorter.begin(), etldatashorter.end());

      CHECK(!(view1 == view2));
      CHECK(!(cview1 == cview2));

      CHECK(!(view1 <= view2));
      CHECK(!(cview1 <= cview2));

      CHECK(view2 <= view1);
      CHECK(cview2 <= cview1);

      CHECK(view1 >= view2);
      CHECK(cview1 >= cview2);

      CHECK(!(view2 >= view1));
      CHECK(!(cview2 >= cview1));

      CHECK(view1  > view2);
      CHECK(cview1 > cview2);

      CHECK(!(view2  > view1));
      CHECK(!(cview2 > cview1));

      CHECK(!(view1  < view2));
      CHECK(!(cview1 < cview2));

      CHECK(view2  < view1);
      CHECK(cview2 < cview1);
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
    TEST(test_remove_prefix_suffix)
    {
      std::vector<int> original     = { 1, 2, 3, 4, 5, 6, 7, 8 };
      std::vector<int> prefix       = { 3, 4, 5, 6, 7, 8 };
      std::vector<int> prefixsuffix = { 3, 4, 5, 6 };
      View view(original);
      CView cview(original);

      bool isEqual;

      view.remove_prefix(2);
      isEqual = std::equal(view.begin(), view.end(), prefix.begin());
      CHECK(isEqual);

      cview.remove_prefix(2);
      isEqual = std::equal(cview.begin(), cview.end(), prefix.begin());
      CHECK(isEqual);

      view.remove_suffix(2);
      isEqual = std::equal(view.begin(), view.end(), prefixsuffix.begin());
      CHECK(isEqual);

      cview.remove_suffix(2);
      isEqual = std::equal(cview.begin(), cview.end(), prefixsuffix.begin());
      CHECK(isEqual);
    }

    //*************************************************************************
	  TEST(test_remove_prefix_boundary)
	  {
		  // On-point test
		  std::vector<int> original = { 1, 2, 3, 4, 5, 6, 7, 8 };

		  View view(original);

		  view.remove_prefix(original.size());

		  CHECK(view.empty());

		  // Off-point test

		  std::vector<int> original2 = { 1, 2, 3, 4, 5, 6, 7, 8 };

		  View view2(original2);

		  view2.remove_prefix(original.size() + 1);

		  CHECK(view2.empty());
	  }

    //*************************************************************************
	  TEST(test_remove_suffix_boundary)
	  {
		  // On-point test
		  std::vector<int> original = { 1, 2, 3, 4, 5, 6, 7, 8 };

		  View view(original);

		  view.remove_suffix(original.size());

		  CHECK(view.empty());

		  // Off-point test

		  std::vector<int> original2 = { 1, 2, 3, 4, 5, 6, 7, 8 };

		  View view2(original2);

		  view2.remove_suffix(original.size() + 1);

		  CHECK(view2.empty());
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

    //*************************************************************************
#include "etl/private/diagnostic_unused_function_push.h"

    struct C_issue_482 {};

    void f_issue_482(etl::array_view<char>)
    {
    }

    void f_issue_482(etl::array_view<C_issue_482>)
    {
    }

    TEST(test_issue_482_2)
    {
      etl::array<C_issue_482, 10> c;

      // Should compile without ambiguous function error.
      f_issue_482(c);
    }

#include "etl/private/diagnostic_pop.h"

    //*************************************************************************
    TEST(test_fill)
    {
      View view;

      view.assign(etldata.begin(), etldata.end());

      view.fill(FillData);

      bool isEqual = std::equal(filldata.begin(), filldata.end(), view.begin());
      CHECK(isEqual);
    }
  };
}
