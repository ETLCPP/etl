/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl
https://www.etlcpp.com

Copyright(c) 2016 John Wellbelove

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

#include "etl/intrusive_links.h"

namespace
{
  //*******************************************************
  // Forward
  //*******************************************************
  typedef etl::forward_link<0> FLink0;
  typedef etl::forward_link<1> FLink1;

  struct FData : public FLink0, public FLink1
  {
    FData(int value_)
      : value(value_)
    {
    }

    int value;
  };

  //*******************************************************
  // Bidirectional
  //*******************************************************
  typedef etl::bidirectional_link<0> BLink0;
  typedef etl::bidirectional_link<1> BLink1;

  struct BData : public BLink0, public BLink1
  {
    BData(int value_)
      : value(value_)
    {
    }

    int value;
  };

  //*******************************************************
  // Tree
  //*******************************************************
  typedef etl::tree_link<0> TLink0;
  typedef etl::tree_link<1> TLink1;
  typedef etl::tree_link<2> TLink2;

  struct TData : public TLink0, public TLink1
  {
    TData(int value_)
      : value(value_)
    {
    }

    int value;
  };

  //*******************************************************
  // Mixed
  //*******************************************************
  struct MData : public FLink0, public BLink1, public TLink2
  {
    MData(int value_)
      : value(value_)
    {
    }

    int value;
  };

  SUITE(test_intrusive_links)
  {
    //*************************************************************************
    TEST(test_link_forward_link)
    {
      FData data0(0);
      FData data1(1);
      FData data2(2);
      FData data3(3);

      etl::link_clear<FLink0>(data0);
      etl::link<FLink0>(data0,  data1);
      CHECK(data0.FLink0::etl_next == &data1);

      etl::link_clear<FLink0>(data0);
      etl::link<FLink0>(&data0, data1);
      CHECK(data0.FLink0::etl_next == &data1);

      etl::link_clear<FLink0>(data0);
      etl::link<FLink0>(data0,  &data1);
      CHECK(data0.FLink0::etl_next == &data1);

      etl::link_clear<FLink0>(data0);
      etl::link<FLink0>(&data0, &data1);
      CHECK(data0.FLink0::etl_next == &data1);

      etl::link<FLink0>(data1,  data2);
      etl::link<FLink0>(data2,  data3);
      etl::link<FLink0>(data3,  nullptr);

      etl::link<FLink1>(data3, data2);
      etl::link<FLink1>(data2, data1);
      etl::link<FLink1>(data1, data0);
      etl::link<FLink1>(data0, nullptr);

      CHECK(data1.FLink0::etl_next == &data2);
      CHECK(data2.FLink0::etl_next == &data3);
      CHECK(data3.FLink0::etl_next == nullptr);

      CHECK(data3.FLink1::etl_next == &data2);
      CHECK(data2.FLink1::etl_next == &data1);
      CHECK(data1.FLink1::etl_next == &data0);
      CHECK(data0.FLink1::etl_next == nullptr);

      FData* pdata;

      pdata = static_cast<FData*>(data0.FLink0::etl_next);
      CHECK_EQUAL(1, pdata->value);
      pdata = static_cast<FData*>(pdata->FLink0::etl_next);
      CHECK_EQUAL(2, pdata->value);
      pdata = static_cast<FData*>(pdata->FLink0::etl_next);
      CHECK_EQUAL(3, pdata->value);

      pdata = static_cast<FData*>(data3.FLink1::etl_next);
      CHECK_EQUAL(2, pdata->value);
      pdata = static_cast<FData*>(pdata->FLink1::etl_next);
      CHECK_EQUAL(1, pdata->value);
      pdata = static_cast<FData*>(pdata->FLink1::etl_next);
      CHECK_EQUAL(0, pdata->value);
    }

    //*************************************************************************
    TEST(test_link_forward_link_get_set)
    {
      FData data0(0);
      FData data1(1);
      FData data2(2);
      FData data3(3);

      // Use reference interface
      data0.FLink0::set_next(data1);
      data1.FLink0::set_next(data2);
      data2.FLink0::set_next(data3);
      data3.FLink0::clear();

      CHECK(data0.FLink0::get_next() == &data1);
      CHECK(data1.FLink0::get_next() == &data2);
      CHECK(data2.FLink0::get_next() == &data3);
      CHECK(data3.FLink0::get_next() == nullptr);

      // Use pointer interface
      data3.FLink0::set_next(&data2);
      data2.FLink0::set_next(&data1);
      data1.FLink0::set_next(&data0);
      data0.FLink0::set_next(nullptr);

      CHECK(data3.FLink0::get_next() == &data2);
      CHECK(data2.FLink0::get_next() == &data1);
      CHECK(data1.FLink0::get_next() == &data0);
      CHECK(data0.FLink0::get_next() == nullptr);
    }

    //*************************************************************************
    TEST(test_link_forward_link_has_links)
    {
      FData data0(0);
      FData data1(1);

      CHECK_FALSE(data0.FLink0::has_next());

      data0.FLink0::set_next(data1);
      CHECK_TRUE(data0.FLink0::has_next());
    }

    //*************************************************************************
    TEST(test_link_splice_forward_link)
    {
      FData data0(0);
      FData data1(1);
      FData data2(2);
      FData data3(3);

      data0.FLink0::clear();
      etl::link_splice<FLink0>(data0, data1);
      CHECK(data0.FLink0::etl_next == &data1);
      CHECK(data1.FLink0::etl_next == nullptr);

      data0.FLink0::clear();
      etl::link_splice<FLink0>(data0, &data1);
      CHECK(data0.FLink0::etl_next == &data1);
      CHECK(data1.FLink0::etl_next == nullptr);

      data0.FLink0::clear();
      etl::link_splice<FLink0>(&data0, data1);
      CHECK(data0.FLink0::etl_next == &data1);
      CHECK(data1.FLink0::etl_next == nullptr);

      data0.FLink0::clear();
      etl::link_splice<FLink0>(&data0, &data1);
      CHECK(data0.FLink0::etl_next == &data1);
      CHECK(data1.FLink0::etl_next == nullptr);

      data0.FLink0::clear();
      etl::link_splice<FLink0>(data0, data3);
      etl::link_splice<FLink0>(data0, data1);
      etl::link_splice<FLink0>(data1, data2);

      CHECK(data0.FLink0::etl_next == &data1);
      CHECK(data1.FLink0::etl_next == &data2);
      CHECK(data2.FLink0::etl_next == &data3);
      CHECK(data3.FLink0::etl_next == nullptr);
    }

    //*************************************************************************
    TEST(test_link_splice_forward_link_range)
    {
      FData data0(0);
      FData data1(1);
      FData data2(2);
      FData data3(3);
      FData data4(4);
      FData data5(5);
      FData data6(6);
      FData data7(7);

      // First range.
      data0.FLink0::clear();
      etl::link_splice<FLink0>(data0, data1);
      etl::link_splice<FLink0>(data1, data6);
      etl::link_splice<FLink0>(data6, data7);

      // Second range.
      data2.FLink0::clear();
      etl::link_splice<FLink0>(data2, data3);
      etl::link_splice<FLink0>(data3, data4);
      etl::link_splice<FLink0>(data4, data5);

      etl::link_splice<FLink0>(data1, data2, data5);

      CHECK(data0.FLink0::etl_next == &data1);
      CHECK(data1.FLink0::etl_next == &data2);
      CHECK(data2.FLink0::etl_next == &data3);
      CHECK(data3.FLink0::etl_next == &data4);
      CHECK(data4.FLink0::etl_next == &data5);
      CHECK(data5.FLink0::etl_next == &data6);
      CHECK(data6.FLink0::etl_next == &data7);
      CHECK(data7.FLink0::etl_next == nullptr);

      // Do it again with a pointer.
      // First range.
      data0.FLink0::clear();
      etl::link_splice<FLink0>(data0, data1);
      etl::link_splice<FLink0>(data1, data6);
      etl::link_splice<FLink0>(data6, data7);

      // Second range.
      data2.FLink0::clear();
      etl::link_splice<FLink0>(data2, data3);
      etl::link_splice<FLink0>(data3, data4);
      etl::link_splice<FLink0>(data4, data5);

      etl::link_splice<FLink0>(&data1, data2, data5);

      CHECK(data0.FLink0::etl_next == &data1);
      CHECK(data1.FLink0::etl_next == &data2);
      CHECK(data2.FLink0::etl_next == &data3);
      CHECK(data3.FLink0::etl_next == &data4);
      CHECK(data4.FLink0::etl_next == &data5);
      CHECK(data5.FLink0::etl_next == &data6);
      CHECK(data6.FLink0::etl_next == &data7);
      CHECK(data7.FLink0::etl_next == nullptr);

      // Do it again with a nullptr pointer.
      // Second range.
      data2.FLink0::clear();
      etl::link_splice<FLink0>(data2, data3);
      etl::link_splice<FLink0>(data3, data4);
      etl::link_splice<FLink0>(data4, data5);

      etl::link_splice<FLink0>(nullptr, data2, data5);

      CHECK(data2.FLink0::etl_next == &data3);
      CHECK(data3.FLink0::etl_next == &data4);
      CHECK(data4.FLink0::etl_next == &data5);
      CHECK(data5.FLink0::etl_next == nullptr);
    }


    //*************************************************************************
    TEST(test_unlink_after_forward_link)
    {
      FData data0(0);
      FData data1(1);
      FData data2(2);
      FData data3(3);

      etl::link<FLink0>(data0, data1);
      etl::link<FLink0>(data1, data2);
      etl::link<FLink0>(data2, data3);
      etl::link<FLink0>(data3, nullptr);

      etl::link<FLink1>(data3, data2);
      etl::link<FLink1>(data2, data1);
      etl::link<FLink1>(data1, data0);
      etl::link<FLink1>(data0, nullptr);

      etl::unlink_after<FLink0>(data1);
      data2.FLink0::clear();

      CHECK(data0.FLink0::etl_next  == &data1);
      CHECK(data1.FLink0::etl_next  == &data3);
      CHECK(data2.FLink0::etl_next  == nullptr);
      CHECK(data3.FLink0::etl_next  == nullptr);

      CHECK(data3.FLink1::etl_next == &data2);
      CHECK(data2.FLink1::etl_next == &data1);
      CHECK(data1.FLink1::etl_next == &data0);
      CHECK(data0.FLink1::etl_next == nullptr);

      etl::unlink_after<FLink1>(data2);
      data1.FLink1::clear();

      CHECK(data0.FLink0::etl_next  == &data1);
      CHECK(data1.FLink0::etl_next  == &data3);
      CHECK(data3.FLink0::etl_next  == nullptr);

      CHECK(data3.FLink1::etl_next == &data2);
      CHECK(data2.FLink1::etl_next == &data0);
      CHECK(data1.FLink1::etl_next == nullptr);
      CHECK(data0.FLink1::etl_next == nullptr);

      etl::unlink_after<FLink0>(data3);
      etl::unlink_after<FLink1>(data0);

      CHECK(data0.FLink0::etl_next  == &data1);
      CHECK(data1.FLink0::etl_next  == &data3);
      CHECK(data3.FLink0::etl_next  == nullptr);

      CHECK(data3.FLink1::etl_next == &data2);
      CHECK(data2.FLink1::etl_next == &data0);
      CHECK(data0.FLink1::etl_next == nullptr);
    }

    //*************************************************************************
    TEST(test_unlink_after_range_forward_link)
    {
      FData data0(0);
      FData data1(1);
      FData data2(2);
      FData data3(3);

      etl::link<FLink0>(data0, data1);
      etl::link<FLink0>(data1, data2);
      etl::link<FLink0>(data2, data3);
      etl::link<FLink0>(data3, nullptr);

      etl::link<FLink1>(data3, data2);
      etl::link<FLink1>(data2, data1);
      etl::link<FLink1>(data1, data0);
      etl::link<FLink1>(data0, nullptr);

      FLink0* start = etl::unlink_after<FLink0>(data0, data2);
      CHECK(data0.FLink0::etl_next == &data3);
      CHECK(data1.FLink0::etl_next == &data2);
      CHECK(data2.FLink0::etl_next == nullptr);
      CHECK(data3.FLink0::etl_next == nullptr);
      
      etl::link_clear_range(*start);
      CHECK(data0.FLink0::etl_next == &data3);
      CHECK(data1.FLink0::etl_next == nullptr);
      CHECK(data2.FLink0::etl_next == nullptr);
      CHECK(data3.FLink0::etl_next == nullptr);

      CHECK(data3.FLink1::etl_next == &data2);
      CHECK(data2.FLink1::etl_next == &data1);
      CHECK(data1.FLink1::etl_next == &data0);
      CHECK(data0.FLink1::etl_next == nullptr);
    }

    //*************************************************************************
    TEST(test_self_link_forward_link)
    {
      FData data0(0);

      etl::link<FLink0>(data0, data0);

      CHECK(data0.FLink0::etl_next == &data0);

      etl::unlink_after<FLink0>(data0);

      CHECK(data0.FLink0::etl_next == nullptr);
    }

    //*************************************************************************
    TEST(test_link_bidirectional_link)
    {
      BData data0(0);
      BData data1(1);
      BData data2(2);
      BData data3(3);

      etl::link<BLink0>(nullptr, data0);

      data1.BLink0::clear();
      etl::link<BLink0>(data0, data1);
      CHECK(data0.BLink0::etl_next     == &data1);
      CHECK(data1.BLink0::etl_previous == &data0);

      data1.BLink0::clear();
      etl::link<BLink0>(data0, data1);
      CHECK(data0.BLink0::etl_next     == &data1);
      CHECK(data1.BLink0::etl_previous == &data0);

      data1.BLink0::clear();
      etl::link<BLink0>(data0,  data1);
      CHECK(data0.BLink0::etl_next     == &data1);
      CHECK(data1.BLink0::etl_previous == &data0);

      data1.BLink0::clear();
      etl::link<BLink0>(data0, data1);
      CHECK(data0.BLink0::etl_next     == &data1);
      CHECK(data1.BLink0::etl_previous == &data0);

      etl::link<BLink0>(data1, data2);
      etl::link<BLink0>(data2, data3);
      etl::link<BLink0>(data3, nullptr);

      CHECK(data0.BLink0::etl_previous == nullptr);
      CHECK(data1.BLink0::etl_previous == &data0);
      CHECK(data1.BLink0::etl_next     == &data2);
      CHECK(data2.BLink0::etl_previous == &data1);
      CHECK(data2.BLink0::etl_next     == &data3);
      CHECK(data3.BLink0::etl_previous == &data2);
      CHECK(data3.BLink0::etl_next     == nullptr);

      etl::link<BLink1>(nullptr, data3);
      etl::link<BLink1>(data3,   data2);
      etl::link<BLink1>(data2,   data1);
      etl::link<BLink1>(data1,   data0);
      etl::link<BLink1>(data0,   nullptr);

      CHECK(data3.BLink1::etl_previous    == nullptr);
      CHECK(data3.BLink1::etl_next        == &data2);
      CHECK(data2.BLink1::etl_previous    == &data3);
      CHECK(data2.BLink1::etl_next        == &data1);
      CHECK(data1.BLink1::etl_previous    == &data2);
      CHECK(data1.BLink1::etl_next        == &data0);
      CHECK(data0.BLink1::etl_previous    == &data1);
      CHECK(data0.BLink1::etl_next        == nullptr);

      BData* pdata;

      pdata = static_cast<BData*>(data0.BLink0::etl_next);
      CHECK_EQUAL(1, pdata->value);
      pdata = static_cast<BData*>(pdata->BLink0::etl_next);
      CHECK_EQUAL(2, pdata->value);
      pdata = static_cast<BData*>(pdata->BLink0::etl_next);
      CHECK_EQUAL(3, pdata->value);

      pdata = static_cast<BData*>(data3.BLink0::etl_previous);
      CHECK_EQUAL(2, pdata->value);
      pdata = static_cast<BData*>(pdata->BLink0::etl_previous);
      CHECK_EQUAL(1, pdata->value);
      pdata = static_cast<BData*>(pdata->BLink0::etl_previous);
      CHECK_EQUAL(0, pdata->value);

      pdata = static_cast<BData*>(data3.BLink1::etl_next);
      CHECK_EQUAL(2, pdata->value);
      pdata = static_cast<BData*>(pdata->BLink1::etl_next);
      CHECK_EQUAL(1, pdata->value);
      pdata = static_cast<BData*>(pdata->BLink1::etl_next);
      CHECK_EQUAL(0, pdata->value);

      pdata = static_cast<BData*>(data0.BLink1::etl_previous);
      CHECK_EQUAL(1, pdata->value);
      pdata = static_cast<BData*>(pdata->BLink1::etl_previous);
      CHECK_EQUAL(2, pdata->value);
      pdata = static_cast<BData*>(pdata->BLink1::etl_previous);
      CHECK_EQUAL(3, pdata->value);

      data1.BLink0::unlink();
      CHECK(data0.BLink0::etl_next     == &data2);
      CHECK(data2.BLink0::etl_previous == &data0);

      CHECK(data3.BLink1::etl_previous    == nullptr);
      CHECK(data3.BLink1::etl_next        == &data2);
      CHECK(data2.BLink1::etl_previous    == &data3);
      CHECK(data2.BLink1::etl_next        != nullptr);
      CHECK(data0.BLink1::etl_previous    != nullptr);
      CHECK(data0.BLink1::etl_next        == nullptr);

      data0.BLink0::unlink();
      CHECK(data2.BLink0::etl_next     == &data3);
      CHECK(data2.BLink0::etl_previous == nullptr);
      CHECK(data3.BLink0::etl_previous == &data2);

      CHECK(data3.BLink1::etl_previous    == nullptr);
      CHECK(data3.BLink1::etl_next        == &data2);
      CHECK(data2.BLink1::etl_previous    == &data3);
      CHECK(data2.BLink1::etl_next        != nullptr);

      data3.BLink0::unlink();
      CHECK(data2.BLink0::etl_next     == nullptr);
      CHECK(data2.BLink0::etl_previous == nullptr);

      CHECK(data2.BLink1::etl_next        != nullptr);
      CHECK(data2.BLink1::etl_previous    != nullptr);

      data2.BLink0::unlink();
    }

    //*************************************************************************
    TEST(test_link_bidirectional_link_get_set)
    {
      BData data0(0);
      BData data1(1);
      BData data2(2);
      BData data3(3);

      // Use reference interface    
      data0.BLink0::set_next(data1);
      data1.BLink0::set_next(data2);
      data2.BLink0::set_next(data3);
      
      data1.BLink0::set_previous(data0);
      data2.BLink0::set_previous(data1);
      data3.BLink0::set_previous(data2);

      CHECK(data0.BLink0::get_next() == &data1);
      CHECK(data1.BLink0::get_next() == &data2);
      CHECK(data2.BLink0::get_next() == &data3);
      CHECK(data3.BLink0::get_next() == nullptr);

      CHECK(data3.BLink0::get_previous() == &data2);
      CHECK(data2.BLink0::get_previous() == &data1);
      CHECK(data1.BLink0::get_previous() == &data0);
      CHECK(data0.BLink0::get_previous() == nullptr);

      // Use pointer interface
      data0.BLink0::clear();
      data1.BLink0::clear();
      data2.BLink0::clear();
      data3.BLink0::clear();

      data3.BLink0::set_next(&data2);
      data2.BLink0::set_next(&data1);
      data1.BLink0::set_next(&data0);

      data0.BLink0::set_previous(&data1);
      data1.BLink0::set_previous(&data2);
      data2.BLink0::set_previous(&data3);

      CHECK(data3.BLink0::get_next() == &data2);
      CHECK(data2.BLink0::get_next() == &data1);
      CHECK(data1.BLink0::get_next() == &data0);
      CHECK(data0.BLink0::get_next() == nullptr);

      CHECK(data0.BLink0::get_previous() == &data1);
      CHECK(data1.BLink0::get_previous() == &data2);
      CHECK(data2.BLink0::get_previous() == &data3);
      CHECK(data3.BLink0::get_previous() == nullptr);
    }

    //*************************************************************************
    TEST(test_link_bidirectional_link_has_links)
    {
      BData data0(0);
      BData data1(1);
      BData data2(2);

      CHECK_FALSE(data1.BLink0::has_previous());
      CHECK_FALSE(data1.BLink0::has_next());

      data1.BLink0::set_previous(data0);
      CHECK_TRUE(data1.BLink0::has_previous());
      CHECK_FALSE(data1.BLink0::has_next());

      data1.BLink0::set_next(data2);
      CHECK_TRUE(data1.BLink0::has_previous());
      CHECK_TRUE(data1.BLink0::has_next());
    }

    //*************************************************************************
    TEST(test_link_splice_bidirectional_link)
    {
      BData data0(0);
      BData data1(1);
      BData data2(2);
      BData data3(3);

      data0.BLink0::clear();
      etl::link_splice<BLink0>(nullptr, data0);

      etl::link_splice<BLink0>(data0, data1);
      CHECK(data0.BLink0::etl_next     == &data1);
      CHECK(data1.BLink0::etl_previous == &data0);
      CHECK(data1.BLink0::etl_next     == nullptr);

      data0.BLink0::clear();
      etl::link_splice<BLink0>(nullptr, data0);

      etl::link_splice<BLink0>(data0, &data1);
      CHECK(data0.BLink0::etl_next     == &data1);
      CHECK(data1.BLink0::etl_previous == &data0);
      CHECK(data1.BLink0::etl_next     == nullptr);

      data0.BLink0::clear();
      etl::link_splice<BLink0>(nullptr, data0);

      etl::link_splice<BLink0>(&data0, data1);
      CHECK(data0.BLink0::etl_next     == &data1);
      CHECK(data1.BLink0::etl_previous == &data0);
      CHECK(data1.BLink0::etl_next     == nullptr);

      data0.BLink0::clear();
      etl::link_splice<BLink0>(nullptr, data0);

      etl::link_splice<BLink0>(&data0, &data1);
      CHECK(data0.BLink0::etl_next     == &data1);
      CHECK(data1.BLink0::etl_previous == &data0);
      CHECK(data1.BLink0::etl_next     == nullptr);

      data0.BLink0::clear();
      etl::link_splice<BLink0>(nullptr, data0);
      etl::link_splice<BLink0>(data0,   data3);
      etl::link_splice<BLink0>(data0,   data1);
      etl::link_splice<BLink0>(data1,   data2);

      CHECK(data0.BLink0::etl_previous == nullptr);
      CHECK(data0.BLink0::etl_next     == &data1);
      CHECK(data1.BLink0::etl_previous == &data0);
      CHECK(data1.BLink0::etl_next     == &data2);
      CHECK(data2.BLink0::etl_previous == &data1);
      CHECK(data2.BLink0::etl_next     == &data3);
      CHECK(data3.BLink0::etl_previous == &data2);
      CHECK(data3.BLink0::etl_next     == nullptr);
    }

    //*************************************************************************
    TEST(test_link_splice_range_bidirectional_link)
    {
      BData data0(0);
      BData data1(1);
      BData data2(2);
      BData data3(3);
      BData data4(4);
      BData data5(5);
      BData data6(6);
      BData data7(7);

      // Build the first range.
      data0.BLink0::clear();
      etl::link_splice<BLink0>(nullptr, data0);
      etl::link_splice<BLink0>(data0,   data1);
      etl::link_splice<BLink0>(data1,   data6);
      etl::link_splice<BLink0>(data6,   data7);

      // Build the second range.
      data2.BLink0::clear();
      etl::link_splice<BLink0>(nullptr, data2);
      etl::link_splice<BLink0>(data2,   data3);
      etl::link_splice<BLink0>(data3,   data4);
      etl::link_splice<BLink0>(data4,   data5);

      etl::link_splice<BLink0>(data1, data2, data5);

      CHECK(data0.BLink0::etl_previous == nullptr);
      CHECK(data0.BLink0::etl_next     == &data1);
      CHECK(data1.BLink0::etl_previous == &data0);
      CHECK(data1.BLink0::etl_next     == &data2);
      CHECK(data2.BLink0::etl_previous == &data1);
      CHECK(data2.BLink0::etl_next     == &data3);
      CHECK(data3.BLink0::etl_previous == &data2);
      CHECK(data3.BLink0::etl_next     == &data4);
      CHECK(data4.BLink0::etl_previous == &data3);
      CHECK(data4.BLink0::etl_next     == &data5);
      CHECK(data5.BLink0::etl_previous == &data4);
      CHECK(data5.BLink0::etl_next     == &data6);
      CHECK(data6.BLink0::etl_previous == &data5);
      CHECK(data6.BLink0::etl_next     == &data7);
      CHECK(data7.BLink0::etl_previous == &data6);
      CHECK(data7.BLink0::etl_next     == nullptr);

      // Do it again with a pointer parameter.
      // Build the first range.
      data0.BLink0::clear();
      etl::link_splice<BLink0>(nullptr, data0);
      etl::link_splice<BLink0>(data0, data1);
      etl::link_splice<BLink0>(data1, data6);
      etl::link_splice<BLink0>(data6, data7);

      // Build the second range.
      data2.BLink0::clear();
      etl::link_splice<BLink0>(nullptr, data2);
      etl::link_splice<BLink0>(data2, data3);
      etl::link_splice<BLink0>(data3, data4);
      etl::link_splice<BLink0>(data4, data5);

      etl::link_splice<BLink0>(&data1, data2, data5);

      CHECK(data0.BLink0::etl_previous == nullptr);
      CHECK(data0.BLink0::etl_next == &data1);
      CHECK(data1.BLink0::etl_previous == &data0);
      CHECK(data1.BLink0::etl_next == &data2);
      CHECK(data2.BLink0::etl_previous == &data1);
      CHECK(data2.BLink0::etl_next == &data3);
      CHECK(data3.BLink0::etl_previous == &data2);
      CHECK(data3.BLink0::etl_next == &data4);
      CHECK(data4.BLink0::etl_previous == &data3);
      CHECK(data4.BLink0::etl_next == &data5);
      CHECK(data5.BLink0::etl_previous == &data4);
      CHECK(data5.BLink0::etl_next == &data6);
      CHECK(data6.BLink0::etl_previous == &data5);
      CHECK(data6.BLink0::etl_next == &data7);
      CHECK(data7.BLink0::etl_previous == &data6);
      CHECK(data7.BLink0::etl_next == nullptr);

      // Do it again with a nullptr parameter.
      // Build the range.
      data2.BLink0::clear();
      etl::link_splice<BLink0>(nullptr, data2);
      etl::link_splice<BLink0>(data2,   data3);
      etl::link_splice<BLink0>(data3,   data4);
      etl::link_splice<BLink0>(data4,   data5);

      etl::link_splice<BLink0>(nullptr, data2, data5);

      CHECK(data2.BLink0::etl_previous == nullptr);
      CHECK(data2.BLink0::etl_next     == &data3);
      CHECK(data3.BLink0::etl_previous == &data2);
      CHECK(data3.BLink0::etl_next     == &data4);
      CHECK(data4.BLink0::etl_previous == &data3);
      CHECK(data4.BLink0::etl_next     == &data5);
      CHECK(data5.BLink0::etl_previous == &data4);
      CHECK(data5.BLink0::etl_next     == nullptr);
    }

    //*************************************************************************
    TEST(test_unlink_bidirectional_link)
    {
      BData data0(0);
      BData data1(1);
      BData data2(2);
      BData data3(3);

      etl::link<BLink0>(nullptr, data0);
      etl::link<BLink0>(data0,   data1);
      etl::link<BLink0>(data1,   data2);
      etl::link<BLink0>(data2,   data3);
      etl::link<BLink0>(data3,   nullptr);

      etl::link<BLink1>(nullptr, data3);
      etl::link<BLink1>(data3,   data2);
      etl::link<BLink1>(data2,   data1);
      etl::link<BLink1>(data1,   data0);
      etl::link<BLink1>(data0,   nullptr);

      etl::unlink<BLink0>(data1);
      data1.BLink0::clear();

      CHECK(data0.BLink0::etl_previous  == nullptr);
      CHECK(data0.BLink0::etl_next      == &data2);
      CHECK(data1.BLink0::etl_previous  == nullptr);
      CHECK(data1.BLink0::etl_next      == nullptr);
      CHECK(data2.BLink0::etl_previous  == &data0);
      CHECK(data2.BLink0::etl_next      == &data3);
      CHECK(data3.BLink0::etl_previous  == &data2);
      CHECK(data3.BLink0::etl_next      == nullptr);

      CHECK(data3.BLink1::etl_previous == nullptr);
      CHECK(data3.BLink1::etl_next     == &data2);
      CHECK(data2.BLink1::etl_previous == &data3);
      CHECK(data2.BLink1::etl_next     == &data1);
      CHECK(data1.BLink1::etl_previous == &data2);
      CHECK(data1.BLink1::etl_next     == &data0);
      CHECK(data0.BLink1::etl_previous == &data1);
      CHECK(data0.BLink1::etl_next     == nullptr);

      etl::unlink<BLink1>(data2);
      data2.BLink1::clear();

      CHECK(data0.BLink0::etl_previous  == nullptr);
      CHECK(data0.BLink0::etl_next      == &data2);
      CHECK(data1.BLink0::etl_previous  == nullptr);
      CHECK(data1.BLink0::etl_next      == nullptr);
      CHECK(data2.BLink0::etl_previous  == &data0);
      CHECK(data2.BLink0::etl_next      == &data3);
      CHECK(data3.BLink0::etl_previous  == &data2);
      CHECK(data3.BLink0::etl_next      == nullptr);

      CHECK(data3.BLink1::etl_previous == nullptr);
      CHECK(data3.BLink1::etl_next     == &data1);
      CHECK(data2.BLink1::etl_previous == nullptr);
      CHECK(data2.BLink1::etl_next     == nullptr);
      CHECK(data1.BLink1::etl_previous == &data3);
      CHECK(data1.BLink1::etl_next     == &data0);
      CHECK(data0.BLink1::etl_previous == &data1);
      CHECK(data0.BLink1::etl_next     == nullptr);

      etl::unlink<BLink0>(data0);
      data0.BLink0::clear();

      CHECK(data0.BLink0::etl_previous  == nullptr);
      CHECK(data0.BLink0::etl_next      == nullptr);
      CHECK(data1.BLink0::etl_previous  == nullptr);
      CHECK(data1.BLink0::etl_next      == nullptr);
      CHECK(data2.BLink0::etl_previous  == nullptr);
      CHECK(data2.BLink0::etl_next      == &data3);
      CHECK(data3.BLink0::etl_previous  == &data2);
      CHECK(data3.BLink0::etl_next      == nullptr);

      CHECK(data3.BLink1::etl_previous == nullptr);
      CHECK(data3.BLink1::etl_next     == &data1);
      CHECK(data2.BLink1::etl_previous == nullptr);
      CHECK(data2.BLink1::etl_next     == nullptr);
      CHECK(data1.BLink1::etl_previous == &data3);
      CHECK(data1.BLink1::etl_next     == &data0);
      CHECK(data0.BLink1::etl_previous == &data1);
      CHECK(data0.BLink1::etl_next     == nullptr);

      etl::unlink<BLink1>(data3);
      data3.BLink1::clear();

      CHECK(data0.BLink0::etl_previous  == nullptr);
      CHECK(data0.BLink0::etl_next      == nullptr);
      CHECK(data1.BLink0::etl_previous  == nullptr);
      CHECK(data1.BLink0::etl_next      == nullptr);
      CHECK(data2.BLink0::etl_previous  == nullptr);
      CHECK(data2.BLink0::etl_next      == &data3);
      CHECK(data3.BLink0::etl_previous  == &data2);
      CHECK(data3.BLink0::etl_next      == nullptr);

      CHECK(data3.BLink1::etl_previous == nullptr);
      CHECK(data3.BLink1::etl_next     == nullptr);
      CHECK(data2.BLink1::etl_previous == nullptr);
      CHECK(data2.BLink1::etl_next     == nullptr);
      CHECK(data1.BLink1::etl_previous == nullptr);
      CHECK(data1.BLink1::etl_next     == &data0);
      CHECK(data0.BLink1::etl_previous == &data1);
      CHECK(data0.BLink1::etl_next     == nullptr);
    }

    //*************************************************************************
    TEST(test_unlink_range_bidirectional_link)
    {
      BData data0(0);
      BData data1(1);
      BData data2(2);
      BData data3(3);

      etl::link<BLink0>(nullptr, data0);
      etl::link<BLink0>(data0, data1);
      etl::link<BLink0>(data1, data2);
      etl::link<BLink0>(data2, data3);
      etl::link<BLink0>(data3, nullptr);

      etl::link<BLink1>(nullptr, data3);
      etl::link<BLink1>(data3, data2);
      etl::link<BLink1>(data2, data1);
      etl::link<BLink1>(data1, data0);
      etl::link<BLink1>(data0, nullptr);

      etl::unlink<BLink0>(data1, data2);
      data1.BLink0::clear();
      data2.BLink0::clear();

      CHECK(data0.BLink0::etl_previous == nullptr);
      CHECK(data0.BLink0::etl_next == &data3);
      CHECK(data1.BLink0::etl_previous == nullptr);
      CHECK(data1.BLink0::etl_next == nullptr);
      CHECK(data2.BLink0::etl_previous == nullptr);
      CHECK(data2.BLink0::etl_next == nullptr);
      CHECK(data3.BLink0::etl_previous == &data0);
      CHECK(data3.BLink0::etl_next == nullptr);

      CHECK(data3.BLink1::etl_previous == nullptr);
      CHECK(data3.BLink1::etl_next == &data2);
      CHECK(data2.BLink1::etl_previous == &data3);
      CHECK(data2.BLink1::etl_next == &data1);
      CHECK(data1.BLink1::etl_previous == &data2);
      CHECK(data1.BLink1::etl_next == &data0);
      CHECK(data0.BLink1::etl_previous == &data1);
      CHECK(data0.BLink1::etl_next == nullptr);
    }

    //*************************************************************************
    TEST(test_self_link_bidirectional_link)
    {
      BData data0(0);

      etl::link<BLink0>(data0, data0);

      CHECK(data0.BLink0::etl_previous == &data0);
      CHECK(data0.BLink0::etl_next     == &data0);

      etl::unlink<BLink0>(data0);

      CHECK(data0.BLink0::etl_previous == ETL_NULLPTR);
      CHECK(data0.BLink0::etl_next     == ETL_NULLPTR);
    }

    //*************************************************************************
    TEST(test_tree_link)
    {
      TData data0(0);
      TData data1(1);
      TData data2(2);
      TData data3(3);
      TData data4(4);
      TData data5(5);
      TData data6(6);

      // First link
      data0.TLink0::clear();
      etl::link_left<TLink0>(data0,  data1);
      etl::link_right<TLink0>(data0, data2);
      CHECK(data0.TLink0::etl_left   == &data1);
      CHECK(data0.TLink0::etl_right  == &data2);
      CHECK(data1.TLink0::etl_parent == &data0);
      CHECK(data2.TLink0::etl_parent == &data0);

      data0.TLink0::clear();
      etl::link_left<TLink0>(&data0,  data1);
      etl::link_right<TLink0>(&data0, data2);
      CHECK(data0.TLink0::etl_left    == &data1);
      CHECK(data0.TLink0::etl_right   == &data2);
      CHECK(data1.TLink0::etl_parent  == &data0);
      CHECK(data2.TLink0::etl_parent  == &data0);

      data0.TLink0::clear();
      etl::link_left<TLink0>(data0,  &data1);
      etl::link_right<TLink0>(data0, &data2);
      CHECK(data0.TLink0::etl_left   == &data1);
      CHECK(data0.TLink0::etl_right  == &data2);
      CHECK(data1.TLink0::etl_parent == &data0);
      CHECK(data2.TLink0::etl_parent == &data0);

      data0.TLink0::clear();
      etl::link_left<TLink0>(&data0,  &data1);
      etl::link_right<TLink0>(&data0, &data2);

      etl::link_left<TLink0>(data1,  data3);
      etl::link_right<TLink0>(data1, data4);
      etl::link_left<TLink0>(data3,  nullptr);
      etl::link_right<TLink0>(data3, nullptr);
      etl::link_left<TLink0>(data4,  nullptr);
      etl::link_right<TLink0>(data4, nullptr);

      etl::link_left<TLink0>(data2,  data5);
      etl::link_right<TLink0>(data2, data6);
      etl::link_left<TLink0>(data5,  nullptr);
      etl::link_right<TLink0>(data5, nullptr);
      etl::link_left<TLink0>(data6,  nullptr);
      etl::link_right<TLink0>(data6, nullptr);

      // Second link
      data0.TLink1::clear();
      etl::link_left<TLink1>(&data6,  &data4);
      etl::link_right<TLink1>(&data6, &data5);

      etl::link_left<TLink1>(data4,  data0);
      etl::link_right<TLink1>(data4, data1);
      etl::link_left<TLink1>(data0,  nullptr);
      etl::link_right<TLink1>(data0, nullptr);
      etl::link_left<TLink1>(data1,  nullptr);
      etl::link_right<TLink1>(data1, nullptr);

      etl::link_left<TLink1>(data5,  data2);
      etl::link_right<TLink1>(data5, data3);
      etl::link_left<TLink1>(data2,  nullptr);
      etl::link_right<TLink1>(data2, nullptr);
      etl::link_left<TLink1>(data3,  nullptr);
      etl::link_right<TLink1>(data3, nullptr);

      // Check first
      CHECK(data0.TLink0::etl_left   == &data1);
      CHECK(data0.TLink0::etl_right  == &data2);
      CHECK(data1.TLink0::etl_parent == &data0);
      CHECK(data2.TLink0::etl_parent == &data0);

      CHECK(data1.TLink0::etl_left   == &data3);
      CHECK(data1.TLink0::etl_right  == &data4);
      CHECK(data3.TLink0::etl_parent == &data1);
      CHECK(data3.TLink0::etl_left   == nullptr);
      CHECK(data3.TLink0::etl_right  == nullptr);
      CHECK(data4.TLink0::etl_parent == &data1);
      CHECK(data4.TLink0::etl_left   == nullptr);
      CHECK(data4.TLink0::etl_right  == nullptr);

      CHECK(data2.TLink0::etl_left   == &data5);
      CHECK(data2.TLink0::etl_right  == &data6);
      CHECK(data5.TLink0::etl_parent == &data2);
      CHECK(data5.TLink0::etl_left   == nullptr);
      CHECK(data5.TLink0::etl_right  == nullptr);
      CHECK(data6.TLink0::etl_parent == &data2);
      CHECK(data6.TLink0::etl_left   == nullptr);
      CHECK(data6.TLink0::etl_right  == nullptr);

      // Check second
      CHECK(data6.TLink1::etl_left   == &data4);
      CHECK(data6.TLink1::etl_right  == &data5);
      CHECK(data4.TLink1::etl_parent == &data6);
      CHECK(data5.TLink1::etl_parent == &data6);

      CHECK(data4.TLink1::etl_left   == &data0);
      CHECK(data4.TLink1::etl_right  == &data1);
      CHECK(data0.TLink1::etl_parent == &data4);
      CHECK(data0.TLink1::etl_left   == nullptr);
      CHECK(data0.TLink1::etl_right  == nullptr);
      CHECK(data1.TLink1::etl_parent == &data4);
      CHECK(data1.TLink1::etl_left   == nullptr);
      CHECK(data1.TLink1::etl_right  == nullptr);

      CHECK(data5.TLink1::etl_left   == &data2);
      CHECK(data5.TLink1::etl_right  == &data3);
      CHECK(data2.TLink1::etl_parent == &data5);
      CHECK(data2.TLink1::etl_left   == nullptr);
      CHECK(data2.TLink1::etl_right  == nullptr);
      CHECK(data3.TLink1::etl_parent == &data5);
      CHECK(data3.TLink1::etl_left   == nullptr);
      CHECK(data3.TLink1::etl_right  == nullptr);
    }

    //*************************************************************************
    TEST(test_link_tree_link_get_set)
    {
      TData dataParent(0);
      TData dataNode(1);
      TData dataLeft(2);
      TData dataRight(3);

      // Use reference interface
      dataNode.TLink0::set_parent(dataParent);
      dataNode.TLink0::set_left(dataLeft);
      dataNode.TLink0::set_right(dataRight);

      CHECK(dataNode.TLink0::get_parent() == &dataParent);
      CHECK(dataNode.TLink0::get_left()   == &dataLeft);
      CHECK(dataNode.TLink0::get_right()  == &dataRight);

      // Use pointer interface
      dataNode.TLink0::clear();

      dataNode.TLink0::set_parent(&dataParent);
      dataNode.TLink0::set_left(&dataLeft);
      dataNode.TLink0::set_right(&dataRight);

      CHECK(dataNode.TLink0::get_parent() == &dataParent);
      CHECK(dataNode.TLink0::get_left()   == &dataLeft);
      CHECK(dataNode.TLink0::get_right()  == &dataRight);
    }

    //*************************************************************************
    TEST(test_link_tree_link_has_links)
    {
      TData dataParent(0);
      TData dataNode(1);
      TData dataLeft(2);
      TData dataRight(3);

      CHECK_FALSE(dataNode.TLink0::has_parent());
      CHECK_FALSE(dataNode.TLink0::has_left());
      CHECK_FALSE(dataNode.TLink0::has_right());

      dataNode.TLink0::set_parent(dataParent);
      CHECK_TRUE(dataNode.TLink0::has_parent());
      CHECK_FALSE(dataNode.TLink0::has_left());
      CHECK_FALSE(dataNode.TLink0::has_right());

      dataNode.TLink0::set_left(dataLeft);
      CHECK_TRUE(dataNode.TLink0::has_parent());
      CHECK_TRUE(dataNode.TLink0::has_left());
      CHECK_FALSE(dataNode.TLink0::has_right());

      dataNode.TLink0::set_right(dataRight);
      CHECK_TRUE(dataNode.TLink0::has_parent());
      CHECK_TRUE(dataNode.TLink0::has_left());
      CHECK_TRUE(dataNode.TLink0::has_right());
    }

    //*************************************************************************
    TEST(test_link_tree_link_mirror)
    {
      TData dataParent(0);
      TData dataNode(1);
      TData dataLeft(2);
      TData dataRight(3);

      dataNode.TLink0::set_parent(dataParent);
      dataNode.TLink0::set_left(dataLeft);
      dataNode.TLink0::set_right(dataRight);

      dataNode.TLink0::mirror();

      CHECK(dataNode.TLink0::get_parent() == &dataParent);
      CHECK(dataNode.TLink0::get_left()   == &dataRight);
      CHECK(dataNode.TLink0::get_right()  == &dataLeft);
    }

    //*************************************************************************
    TEST(test_is_linked)
    {
      // Forward link
      FData fdata(0);

      fdata.FLink0::clear();
      fdata.FLink1::clear();
      CHECK(!fdata.FLink0::is_linked());
      CHECK(!fdata.FLink1::is_linked());

      etl::link<FLink0>(fdata, fdata);
      CHECK(fdata.FLink0::is_linked());
      CHECK(!fdata.FLink1::is_linked());

      etl::link<FLink1>(fdata, fdata);
      CHECK(fdata.FLink0::is_linked());
      CHECK(fdata.FLink1::is_linked());

      // Bidirectional link
      BData bdata(0);

      bdata.BLink0::clear();
      bdata.BLink1::clear();
      CHECK(!bdata.BLink0::is_linked());
      CHECK(!bdata.BLink1::is_linked());

      etl::link<BLink0>(bdata, bdata);
      CHECK(bdata.BLink0::is_linked());
      CHECK(!bdata.BLink1::is_linked());

      etl::link<BLink1>(bdata, bdata);
      CHECK(bdata.BLink0::is_linked());
      CHECK(bdata.BLink1::is_linked());

      etl::unlink<BLink0>(bdata); // Global API
      CHECK(!bdata.BLink0::is_linked());
      CHECK(bdata.BLink1::is_linked());

      bdata.BLink1::unlink();     // Member API
      CHECK(!bdata.BLink0::is_linked());
      CHECK(!bdata.BLink1::is_linked());

      bdata.BLink0::clear();
      CHECK(!bdata.BLink0::is_linked());
      CHECK(!bdata.BLink1::is_linked());

      // Tree link
      TData tdata(0);

      tdata.TLink0::clear();
      tdata.TLink1::clear();
      CHECK(!tdata.TLink0::is_linked());
      CHECK(!tdata.TLink1::is_linked());

      etl::link_left<TLink0>(tdata, tdata);
      CHECK(tdata.TLink0::is_linked());
      CHECK(!tdata.TLink1::is_linked());

      etl::link_right<TLink1>(tdata, tdata);
      CHECK(tdata.TLink0::is_linked());
      CHECK(tdata.TLink1::is_linked());
    }

    //*************************************************************************
    TEST(test_link_mixed_links)
    {
      MData data0(0);
      MData data1(1);
      MData data2(2);
      MData data3(3);
      MData data4(4);
      MData data5(5);
      MData data6(6);

      // Forward
      data0.FLink0::clear();
      etl::link<FLink0>(data0, data1);
      etl::link<FLink0>(data1, data2);
      etl::link<FLink0>(data2, data3);
      etl::link<FLink0>(data3, nullptr);

      // Bidirectional
      etl::link<BLink1>(nullptr, data3);
      etl::link<BLink1>(data3,   data2);
      etl::link<BLink1>(data2,   data1);
      etl::link<BLink1>(data1,   data0);
      etl::link<BLink1>(data0,   nullptr);

      // Tree
      data0.TLink2::clear();
      etl::link_left<TLink2>(data0,  data1);
      etl::link_right<TLink2>(data0, data2);
      etl::link_left<TLink2>(data1,  data3);
      etl::link_right<TLink2>(data1, data4);
      etl::link_left<TLink2>(data3,  nullptr);
      etl::link_right<TLink2>(data3, nullptr);
      etl::link_left<TLink2>(data4,  nullptr);
      etl::link_right<TLink2>(data4, nullptr);
      etl::link_left<TLink2>(data2,  data5);
      etl::link_right<TLink2>(data2, data6);
      etl::link_left<TLink2>(data5,  nullptr);
      etl::link_right<TLink2>(data5, nullptr);
      etl::link_left<TLink2>(data6,  nullptr);
      etl::link_right<TLink2>(data6, nullptr);

      CHECK(data0.FLink0::etl_next == &data1);
      CHECK(data1.FLink0::etl_next == &data2);
      CHECK(data2.FLink0::etl_next == &data3);
      CHECK(data3.FLink0::etl_next == nullptr);

      CHECK(data3.BLink1::etl_previous == nullptr);
      CHECK(data3.BLink1::etl_next == &data2);
      CHECK(data2.BLink1::etl_previous == &data3);
      CHECK(data2.BLink1::etl_next == &data1);
      CHECK(data1.BLink1::etl_previous == &data2);
      CHECK(data1.BLink1::etl_next == &data0);
      CHECK(data0.BLink1::etl_previous == &data1);
      CHECK(data0.BLink1::etl_next == nullptr);

      CHECK(data0.TLink2::etl_left   == &data1);
      CHECK(data0.TLink2::etl_right  == &data2);
      CHECK(data1.TLink2::etl_parent == &data0);
      CHECK(data2.TLink2::etl_parent == &data0);

      CHECK(data1.TLink2::etl_left   == &data3);
      CHECK(data1.TLink2::etl_right  == &data4);
      CHECK(data3.TLink2::etl_parent == &data1);
      CHECK(data3.TLink2::etl_left   == nullptr);
      CHECK(data3.TLink2::etl_right  == nullptr);
      CHECK(data4.TLink2::etl_parent == &data1);
      CHECK(data4.TLink2::etl_left   == nullptr);
      CHECK(data4.TLink2::etl_right  == nullptr);

      CHECK(data2.TLink2::etl_left   == &data5);
      CHECK(data2.TLink2::etl_right  == &data6);
      CHECK(data5.TLink2::etl_parent == &data2);
      CHECK(data5.TLink2::etl_left   == nullptr);
      CHECK(data5.TLink2::etl_right  == nullptr);
      CHECK(data6.TLink2::etl_parent == &data2);
      CHECK(data6.TLink2::etl_left   == nullptr);
      CHECK(data6.TLink2::etl_right  == nullptr);
    }

    //*************************************************************************
    TEST(test_tree_link_rotate_left)
    {
      TLink0 r;
      TLink0 a;
      TLink0 b;
      TLink0 c;
      TLink0 d;
      TLink0 e;

      r.clear();
      c.clear();
      d.clear();
      e.clear();
      etl::link_left(r, b);
      etl::link_right(b, a);
      etl::link_left(b, d);
      etl::link_right(a, c);
      etl::link_left(a, e);

      etl::link_rotate_left(b, a);

      CHECK(a.etl_parent == &r);
      CHECK(b.etl_parent == &a);
      CHECK(e.etl_parent == &b);
      CHECK(d.etl_parent == &b);
      CHECK(c.etl_parent == &a);
      CHECK(a.etl_left   == &b);
      CHECK(a.etl_right  == &c);
      CHECK(b.etl_left   == &d);
      CHECK(b.etl_right  == &e);
    }

    //*************************************************************************
    TEST(test_tree_link_rotate_left_nullptr)
    {
      TLink0 r;
      TLink0 a;
      TLink0 b;
      TLink0 c;
      TLink0 d;

      r.clear();
      a.clear();
      c.clear();
      d.clear();
      etl::link_left(r, b);
      etl::link_right(b, a);
      etl::link_left(b, d);
      etl::link_right(a, c);

      etl::link_rotate_left(b, a);

      CHECK(a.etl_parent == &r);
      CHECK(b.etl_parent == &a);
      CHECK(d.etl_parent == &b);
      CHECK(c.etl_parent == &a);
      CHECK(a.etl_left   == &b);
      CHECK(a.etl_right  == &c);
      CHECK(b.etl_left   == &d);
      CHECK(b.etl_right  == nullptr);
    }

    //*************************************************************************
    TEST(test_tree_link_rotate_right)
    {
      TLink0 r;
      TLink0 a;
      TLink0 b;
      TLink0 c;
      TLink0 d;
      TLink0 e;

      r.clear();
      c.clear();
      d.clear();
      e.clear();
      etl::link_left(r, a);
      etl::link_left(a, b);
      etl::link_left(b, d);
      etl::link_right(a, c);
      etl::link_right(b, e);

      etl::link_rotate_right(a, b);

      CHECK(b.etl_parent == &r);
      CHECK(d.etl_parent == &b);
      CHECK(a.etl_parent == &b);
      CHECK(e.etl_parent == &a);
      CHECK(c.etl_parent == &a);
      CHECK(b.etl_left   == &d);
      CHECK(b.etl_right  == &a);
      CHECK(a.etl_left   == &e);
      CHECK(a.etl_right  == &c);
    }

    //*************************************************************************
    TEST(test_tree_link_rotate_right_nullptr)
    {
      TLink0 r;
      TLink0 a;
      TLink0 b;
      TLink0 c;
      TLink0 d;

      r.clear();
      b.clear();
      c.clear();
      d.clear();
      etl::link_left(r, a);
      etl::link_left(a, b);
      etl::link_left(b, d);
      etl::link_right(a, c);

      etl::link_rotate_right(a, b);

      CHECK(b.etl_parent == &r);
      CHECK(d.etl_parent == &b);
      CHECK(a.etl_parent == &b);
      CHECK(c.etl_parent == &a);
      CHECK(b.etl_left   == &d);
      CHECK(b.etl_right  == &a);
      CHECK(a.etl_left   == nullptr);
      CHECK(a.etl_right  == &c);
    }
  };
}
