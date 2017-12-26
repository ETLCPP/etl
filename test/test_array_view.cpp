/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl
http://www.etlcpp.com

Copyright(c) 2017 jwellbelove

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

#include "UnitTest++.h"

#include "array_view.h"
#include "array.h"

#include <array>
#include <vector>
#include <algorithm>
#include <iterator>

namespace
{
  SUITE(test_array_view)
  {
    static const size_t SIZE = 10;

    typedef etl::array<int, SIZE> EtlData;
    typedef std::array<int, SIZE> StlData;
    typedef std::vector<int> StlVData;

    typedef etl::array_view<int> View;
    typedef etl::const_array_view<int> CView;

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
    
    int cdata[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
    int* pcdata = cdata;

    const int ccdata[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
    const int* pccdata = ccdata;

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
    TEST(test_constructor_range)
    {
      View view(etldata.begin() + 2, etldata.end() - 2);

      CHECK_EQUAL(etldata.size() - 4, view.size());
      CHECK_EQUAL(etldata.size() - 4, view.max_size());

      bool isEqual = std::equal(view.begin(), view.end(), etldata.begin() + 2);
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
      View  view(etldata.begin(), etldata.begin());
      CView cview(etldata.begin(), etldata.begin());

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

      for (size_t i = 0; i < etldata.size(); ++i)
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

      for (size_t i = 0; i < etldata.size(); ++i)
      {
        CHECK_EQUAL(etldata[i], view.at(i));
        CHECK_EQUAL(etldata[i], cview.at(i));
      }
    }

    //*************************************************************************
    TEST(test_at_exception)
    {
      View  view(etldata.begin(), etldata.end());
      CView cview(etldata.begin(), etldata.end());

      CHECK_THROW(view.at(view.size()),   etl::array_view_bounds);
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
  };
}
