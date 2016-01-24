/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl
http://www.etlcpp.com

Copyright(c) 2016 jwellbelove

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

#include <UnitTest++/UnitTest++.h>
#include "ExtraCheckMacros.h"

#include "data.h"

#include "../intrusive_links.h"

namespace
{
  //*******************************************************
  //Forward
  //*******************************************************
  typedef etl::forward_link<0> FirstFLink;
  typedef etl::forward_link<1> SecondFLink;

  struct FData : public FirstFLink, public SecondFLink
  {
    FData(int value)
      : value(value)
    {
    }

    int value;
  };

  //*******************************************************
  // Bidirectional
  //*******************************************************
  typedef etl::bidirectional_link<0, etl::link_option::AUTO_UNLINK> FirstBLinkAuto;
  typedef etl::bidirectional_link<0> FirstBLink;
  typedef etl::bidirectional_link<1> SecondBLink;

  struct BData : public FirstBLink, public SecondBLink
  {
    BData(int value)
      : value(value)
    {
    }

    int value;
  };

  struct BDataAuto : public FirstBLinkAuto, public SecondBLink
  {
    BDataAuto(int value)
      : value(value)
    {
    }

    int value;
  };

  //*******************************************************
  // Tree
  //*******************************************************
  typedef etl::tree_link<0> FirstTLink;
  typedef etl::tree_link<1> SecondTLink;

  struct TData : public FirstTLink, public SecondTLink
  {
    TData(int value)
      : value(value)
    {
    }

    int value;
  };

  SUITE(test_forward_list)
  {  
    //*************************************************************************
    TEST(test_link_forward_link)
    {
      FData data0(0);
      FData data1(1);
      FData data2(2);
      FData data3(3);

      data0.FirstFLink::clear();
      etl::link<FirstFLink>(data0,  data1);
      CHECK(data0.FirstFLink::etl_next == &data1);

      data0.FirstFLink::clear();
      etl::link<FirstFLink>(&data0, data1);
      CHECK(data0.FirstFLink::etl_next == &data1);

      data0.FirstFLink::clear();
      etl::link<FirstFLink>(data0,  &data1);
      CHECK(data0.FirstFLink::etl_next == &data1);

      data0.FirstFLink::clear();
      etl::link<FirstFLink>(&data0, &data1);
      CHECK(data0.FirstFLink::etl_next == &data1);

      etl::link<FirstFLink>(data1,  data2);
      etl::link<FirstFLink>(data2,  data3);
      etl::link<FirstFLink>(data3,  nullptr);

      etl::link<SecondFLink>(data3, data2);
      etl::link<SecondFLink>(data2, data1);
      etl::link<SecondFLink>(data1, data0);
      etl::link<SecondFLink>(data0, nullptr);

      CHECK(data1.FirstFLink::etl_next == &data2);
      CHECK(data2.FirstFLink::etl_next == &data3);
      CHECK(data3.FirstFLink::etl_next == nullptr);

      CHECK(data3.SecondFLink::etl_next == &data2);
      CHECK(data2.SecondFLink::etl_next == &data1);
      CHECK(data1.SecondFLink::etl_next == &data0);
      CHECK(data0.SecondFLink::etl_next == nullptr);
    }

    //*************************************************************************
    TEST(test_unlink_after_forward_link)
    {
      FData data0(0);
      FData data1(1);
      FData data2(2);
      FData data3(3);

      etl::link<FirstFLink>(data0, data1);
      etl::link<FirstFLink>(data1, data2);
      etl::link<FirstFLink>(data2, data3);
      etl::link<FirstFLink>(data3, nullptr);

      etl::link<SecondFLink>(data3, data2);
      etl::link<SecondFLink>(data2, data1);
      etl::link<SecondFLink>(data1, data0);
      etl::link<SecondFLink>(data0, nullptr);

      etl::unlink_after<FirstFLink>(data1);
      data2.FirstFLink::clear();

      CHECK(data0.FirstFLink::etl_next  == &data1);
      CHECK(data1.FirstFLink::etl_next  == &data3);
      CHECK(data2.FirstFLink::etl_next  == nullptr);
      CHECK(data3.FirstFLink::etl_next  == nullptr);

      CHECK(data3.SecondFLink::etl_next == &data2);
      CHECK(data2.SecondFLink::etl_next == &data1);
      CHECK(data1.SecondFLink::etl_next == &data0);
      CHECK(data0.SecondFLink::etl_next == nullptr);

      etl::unlink_after<SecondFLink>(data2);
      data1.SecondFLink::clear();

      CHECK(data0.FirstFLink::etl_next  == &data1);
      CHECK(data1.FirstFLink::etl_next  == &data3);
      CHECK(data3.FirstFLink::etl_next  == nullptr);

      CHECK(data3.SecondFLink::etl_next == &data2);
      CHECK(data2.SecondFLink::etl_next == &data0);
      CHECK(data1.SecondFLink::etl_next == nullptr);
      CHECK(data0.SecondFLink::etl_next == nullptr);

      etl::unlink_after<FirstFLink>(data3);
      etl::unlink_after<SecondFLink>(data0);

      CHECK(data0.FirstFLink::etl_next  == &data1);
      CHECK(data1.FirstFLink::etl_next  == &data3);
      CHECK(data3.FirstFLink::etl_next  == nullptr);

      CHECK(data3.SecondFLink::etl_next == &data2);
      CHECK(data2.SecondFLink::etl_next == &data0);
      CHECK(data0.SecondFLink::etl_next == nullptr);
    }

    //*************************************************************************
    TEST(test_self_link_forward_link)
    {
      FData data0(0);

      etl::link<FirstFLink>(data0, data0);

      CHECK(data0.FirstFLink::etl_next == &data0);

      etl::unlink_after<FirstFLink>(data0);

      CHECK(data0.FirstFLink::etl_next == &data0);
    }

    //*************************************************************************
    TEST(test_link_bidirectional_link)
    {
      // FirstBLinkAuto is auto-unlink, SecondBLink is not.

      BDataAuto* data0 = new BDataAuto(0);
      BDataAuto* data1 = new BDataAuto(1);
      BDataAuto* data2 = new BDataAuto(2);
      BDataAuto* data3 = new BDataAuto(3);

      etl::link<FirstBLinkAuto>(nullptr, data0);

      data1->FirstBLinkAuto::clear();
      etl::link<FirstBLinkAuto>(*data0, *data1);
      CHECK(data0->FirstBLinkAuto::etl_next     == data1);
      CHECK(data1->FirstBLinkAuto::etl_previous == data0);
      
      data1->FirstBLinkAuto::clear();
      etl::link<FirstBLinkAuto>(*data0, data1);
      CHECK(data0->FirstBLinkAuto::etl_next     == data1);
      CHECK(data1->FirstBLinkAuto::etl_previous == data0);

      data1->FirstBLinkAuto::clear();
      etl::link<FirstBLinkAuto>(data0,  *data1);
      CHECK(data0->FirstBLinkAuto::etl_next     == data1);
      CHECK(data1->FirstBLinkAuto::etl_previous == data0);

      data1->FirstBLinkAuto::clear();
      etl::link<FirstBLinkAuto>(data0, data1);
      CHECK(data0->FirstBLinkAuto::etl_next     == data1);
      CHECK(data1->FirstBLinkAuto::etl_previous == data0);

      etl::link<FirstBLinkAuto>(data1, data2);
      etl::link<FirstBLinkAuto>(data2, data3);
      etl::link<FirstBLinkAuto>(data3, nullptr);

      CHECK(data0->FirstBLinkAuto::etl_previous == nullptr);
      CHECK(data1->FirstBLinkAuto::etl_next == data2);
      CHECK(data2->FirstBLinkAuto::etl_previous == data1);
      CHECK(data2->FirstBLinkAuto::etl_next == data3);
      CHECK(data3->FirstBLinkAuto::etl_previous == data2);
      CHECK(data3->FirstBLinkAuto::etl_next == nullptr);

      etl::link<SecondBLink>(nullptr, data3);
      etl::link<SecondBLink>(data3,   data2);
      etl::link<SecondBLink>(data2,   data1);
      etl::link<SecondBLink>(data1,   data0);
      etl::link<SecondBLink>(data0,   nullptr);

      CHECK(data3->SecondBLink::etl_previous    == nullptr);
      CHECK(data3->SecondBLink::etl_next        == data2);
      CHECK(data2->SecondBLink::etl_previous    == data3);
      CHECK(data2->SecondBLink::etl_next        == data1);
      CHECK(data1->SecondBLink::etl_previous    == data2);
      CHECK(data1->SecondBLink::etl_next        == data0);
      CHECK(data0->SecondBLink::etl_previous    == data1);
      CHECK(data0->SecondBLink::etl_next        == nullptr);

      delete data1;
      CHECK(data0->FirstBLinkAuto::etl_next     == data2);
      CHECK(data2->FirstBLinkAuto::etl_previous == data0);

      CHECK(data3->SecondBLink::etl_previous    == nullptr);
      CHECK(data3->SecondBLink::etl_next        == data2);
      CHECK(data2->SecondBLink::etl_previous    == data3);
      CHECK(data2->SecondBLink::etl_next        != nullptr);
      CHECK(data0->SecondBLink::etl_previous    != nullptr);
      CHECK(data0->SecondBLink::etl_next        == nullptr);

      delete data0;
      CHECK(data2->FirstBLinkAuto::etl_next     == data3);
      CHECK(data2->FirstBLinkAuto::etl_previous == nullptr);
      CHECK(data3->FirstBLinkAuto::etl_previous == data2);
      
      CHECK(data3->SecondBLink::etl_previous    == nullptr);
      CHECK(data3->SecondBLink::etl_next        == data2);
      CHECK(data2->SecondBLink::etl_previous    == data3);
      CHECK(data2->SecondBLink::etl_next        != nullptr);

      delete data3;
      CHECK(data2->FirstBLinkAuto::etl_next     == nullptr);
      CHECK(data2->FirstBLinkAuto::etl_previous == nullptr);

      CHECK(data2->SecondBLink::etl_next        != nullptr);
      CHECK(data2->SecondBLink::etl_previous    != nullptr);

      delete data2;
    }

    //*************************************************************************
    TEST(test_unlink_bidirectional_link)
    {
      BData data0(0);
      BData data1(1);
      BData data2(2);
      BData data3(3);

      etl::link<FirstBLink>(nullptr, data0);
      etl::link<FirstBLink>(data0,   data1);
      etl::link<FirstBLink>(data1,   data2);
      etl::link<FirstBLink>(data2,   data3);
      etl::link<FirstBLink>(data3,   nullptr);

      etl::link<SecondBLink>(nullptr, data3);
      etl::link<SecondBLink>(data3,   data2);
      etl::link<SecondBLink>(data2,   data1);
      etl::link<SecondBLink>(data1,   data0);
      etl::link<SecondBLink>(data0,   nullptr);

      etl::unlink<FirstBLink>(data1);
      data1.FirstBLink::clear();

      CHECK(data0.FirstBLink::etl_previous  == nullptr);
      CHECK(data0.FirstBLink::etl_next      == &data2);
      CHECK(data1.FirstBLink::etl_previous  == nullptr);
      CHECK(data1.FirstBLink::etl_next      == nullptr);
      CHECK(data2.FirstBLink::etl_previous  == &data0);
      CHECK(data2.FirstBLink::etl_next      == &data3);
      CHECK(data3.FirstBLink::etl_previous  == &data2);
      CHECK(data3.FirstBLink::etl_next      == nullptr);

      CHECK(data3.SecondBLink::etl_previous == nullptr);
      CHECK(data3.SecondBLink::etl_next     == &data2);
      CHECK(data2.SecondBLink::etl_previous == &data3);
      CHECK(data2.SecondBLink::etl_next     == &data1);
      CHECK(data1.SecondBLink::etl_previous == &data2);
      CHECK(data1.SecondBLink::etl_next     == &data0);
      CHECK(data0.SecondBLink::etl_previous == &data1);
      CHECK(data0.SecondBLink::etl_next     == nullptr);

      etl::unlink<SecondBLink>(data2);
      data2.SecondBLink::clear();

      CHECK(data0.FirstBLink::etl_previous  == nullptr);
      CHECK(data0.FirstBLink::etl_next      == &data2);
      CHECK(data1.FirstBLink::etl_previous  == nullptr);
      CHECK(data1.FirstBLink::etl_next      == nullptr);
      CHECK(data2.FirstBLink::etl_previous  == &data0);
      CHECK(data2.FirstBLink::etl_next      == &data3);
      CHECK(data3.FirstBLink::etl_previous  == &data2);
      CHECK(data3.FirstBLink::etl_next      == nullptr);

      CHECK(data3.SecondBLink::etl_previous == nullptr);
      CHECK(data3.SecondBLink::etl_next     == &data1);
      CHECK(data2.SecondBLink::etl_previous == nullptr);
      CHECK(data2.SecondBLink::etl_next     == nullptr);
      CHECK(data1.SecondBLink::etl_previous == &data3);
      CHECK(data1.SecondBLink::etl_next     == &data0);
      CHECK(data0.SecondBLink::etl_previous == &data1);
      CHECK(data0.SecondBLink::etl_next     == nullptr);

      etl::unlink<FirstBLink>(data0);
      data0.FirstBLink::clear();

      CHECK(data0.FirstBLink::etl_previous  == nullptr);
      CHECK(data0.FirstBLink::etl_next      == nullptr);
      CHECK(data1.FirstBLink::etl_previous  == nullptr);
      CHECK(data1.FirstBLink::etl_next      == nullptr);
      CHECK(data2.FirstBLink::etl_previous  == nullptr);
      CHECK(data2.FirstBLink::etl_next      == &data3);
      CHECK(data3.FirstBLink::etl_previous  == &data2);
      CHECK(data3.FirstBLink::etl_next      == nullptr);

      CHECK(data3.SecondBLink::etl_previous == nullptr);
      CHECK(data3.SecondBLink::etl_next     == &data1);
      CHECK(data2.SecondBLink::etl_previous == nullptr);
      CHECK(data2.SecondBLink::etl_next     == nullptr);
      CHECK(data1.SecondBLink::etl_previous == &data3);
      CHECK(data1.SecondBLink::etl_next     == &data0);
      CHECK(data0.SecondBLink::etl_previous == &data1);
      CHECK(data0.SecondBLink::etl_next     == nullptr);

      etl::unlink<SecondBLink>(data3);
      data3.SecondBLink::clear();

      CHECK(data0.FirstBLink::etl_previous  == nullptr);
      CHECK(data0.FirstBLink::etl_next      == nullptr);
      CHECK(data1.FirstBLink::etl_previous  == nullptr);
      CHECK(data1.FirstBLink::etl_next      == nullptr);
      CHECK(data2.FirstBLink::etl_previous  == nullptr);
      CHECK(data2.FirstBLink::etl_next      == &data3);
      CHECK(data3.FirstBLink::etl_previous  == &data2);
      CHECK(data3.FirstBLink::etl_next      == nullptr);

      CHECK(data3.SecondBLink::etl_previous == nullptr);
      CHECK(data3.SecondBLink::etl_next     == nullptr);
      CHECK(data2.SecondBLink::etl_previous == nullptr);
      CHECK(data2.SecondBLink::etl_next     == nullptr);
      CHECK(data1.SecondBLink::etl_previous == nullptr);
      CHECK(data1.SecondBLink::etl_next     == &data0);
      CHECK(data0.SecondBLink::etl_previous == &data1);
      CHECK(data0.SecondBLink::etl_next     == nullptr);
    }

    //*************************************************************************
    TEST(test_self_link_bidirectional_link)
    {
      BData data0(0);

      etl::link<FirstBLink>(data0, data0);

      CHECK(data0.FirstBLink::etl_previous == &data0);
      CHECK(data0.FirstBLink::etl_next     == &data0);

      etl::unlink<FirstBLink>(data0);

      CHECK(data0.FirstBLink::etl_previous == &data0);
      CHECK(data0.FirstBLink::etl_next     == &data0);
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
      data0.FirstTLink::clear();     
      etl::link_left<FirstTLink>(data0,  data1);
      etl::link_right<FirstTLink>(data0, data2);
      CHECK(data0.FirstTLink::etl_left   == &data1);
      CHECK(data0.FirstTLink::etl_right  == &data2);
      CHECK(data1.FirstTLink::etl_parent == &data0);
      CHECK(data2.FirstTLink::etl_parent == &data0);

      data0.FirstTLink::clear();
      etl::link_left<FirstTLink>(&data0,  data1);
      etl::link_right<FirstTLink>(&data0, data2);
      CHECK(data0.FirstTLink::etl_left    == &data1);
      CHECK(data0.FirstTLink::etl_right   == &data2);
      CHECK(data1.FirstTLink::etl_parent  == &data0);
      CHECK(data2.FirstTLink::etl_parent  == &data0);

      data0.FirstTLink::clear();
      etl::link_left<FirstTLink>(data0,  &data1);
      etl::link_right<FirstTLink>(data0, &data2);
      CHECK(data0.FirstTLink::etl_left   == &data1);
      CHECK(data0.FirstTLink::etl_right  == &data2);
      CHECK(data1.FirstTLink::etl_parent == &data0);
      CHECK(data2.FirstTLink::etl_parent == &data0);

      data0.FirstTLink::clear();
      etl::link_left<FirstTLink>(&data0,  &data1);
      etl::link_right<FirstTLink>(&data0, &data2);

      etl::link_left<FirstTLink>(data1,  data3);
      etl::link_right<FirstTLink>(data1, data4);
      etl::link_left<FirstTLink>(data3,  nullptr);
      etl::link_right<FirstTLink>(data3, nullptr);
      etl::link_left<FirstTLink>(data4,  nullptr);
      etl::link_right<FirstTLink>(data4, nullptr);

      etl::link_left<FirstTLink>(data2,  data5);
      etl::link_right<FirstTLink>(data2, data6);
      etl::link_left<FirstTLink>(data5,  nullptr);
      etl::link_right<FirstTLink>(data5, nullptr);
      etl::link_left<FirstTLink>(data6,  nullptr);
      etl::link_right<FirstTLink>(data6, nullptr);

      // Second link
      data0.SecondTLink::clear();
      etl::link_left<SecondTLink>(&data6,  &data4);
      etl::link_right<SecondTLink>(&data6, &data5);

      etl::link_left<SecondTLink>(data4,  data0);
      etl::link_right<SecondTLink>(data4, data1);
      etl::link_left<SecondTLink>(data0,  nullptr);
      etl::link_right<SecondTLink>(data0, nullptr);
      etl::link_left<SecondTLink>(data1,  nullptr);
      etl::link_right<SecondTLink>(data1, nullptr);

      etl::link_left<SecondTLink>(data5,  data2);
      etl::link_right<SecondTLink>(data5, data3);
      etl::link_left<SecondTLink>(data2,  nullptr);
      etl::link_right<SecondTLink>(data2, nullptr);
      etl::link_left<SecondTLink>(data3,  nullptr);
      etl::link_right<SecondTLink>(data3, nullptr);

      // Check first
      CHECK(data0.FirstTLink::etl_left   == &data1);
      CHECK(data0.FirstTLink::etl_right  == &data2);
      CHECK(data1.FirstTLink::etl_parent == &data0);
      CHECK(data2.FirstTLink::etl_parent == &data0);

      CHECK(data1.FirstTLink::etl_left   == &data3);
      CHECK(data1.FirstTLink::etl_right  == &data4);
      CHECK(data3.FirstTLink::etl_parent == &data1);
      CHECK(data3.FirstTLink::etl_left   == nullptr);
      CHECK(data3.FirstTLink::etl_right  == nullptr);
      CHECK(data4.FirstTLink::etl_parent == &data1);
      CHECK(data4.FirstTLink::etl_left   == nullptr);
      CHECK(data4.FirstTLink::etl_right  == nullptr);

      CHECK(data2.FirstTLink::etl_left   == &data5);
      CHECK(data2.FirstTLink::etl_right  == &data6);
      CHECK(data5.FirstTLink::etl_parent == &data2);
      CHECK(data5.FirstTLink::etl_left   == nullptr);
      CHECK(data5.FirstTLink::etl_right  == nullptr);
      CHECK(data6.FirstTLink::etl_parent == &data2);
      CHECK(data6.FirstTLink::etl_left   == nullptr);
      CHECK(data6.FirstTLink::etl_right  == nullptr);

      // Check second
      CHECK(data6.SecondTLink::etl_left   == &data4);
      CHECK(data6.SecondTLink::etl_right  == &data5);
      CHECK(data4.SecondTLink::etl_parent == &data6);
      CHECK(data5.SecondTLink::etl_parent == &data6);
      
      CHECK(data4.SecondTLink::etl_left   == &data0);
      CHECK(data4.SecondTLink::etl_right  == &data1);
      CHECK(data0.SecondTLink::etl_parent == &data4);
      CHECK(data0.SecondTLink::etl_left   == nullptr);
      CHECK(data0.SecondTLink::etl_right  == nullptr);
      CHECK(data1.SecondTLink::etl_parent == &data4);
      CHECK(data1.SecondTLink::etl_left   == nullptr);
      CHECK(data1.SecondTLink::etl_right  == nullptr);

      CHECK(data5.SecondTLink::etl_left   == &data2);
      CHECK(data5.SecondTLink::etl_right  == &data3);
      CHECK(data2.SecondTLink::etl_parent == &data5);
      CHECK(data2.SecondTLink::etl_left   == nullptr);
      CHECK(data2.SecondTLink::etl_right  == nullptr);
      CHECK(data3.SecondTLink::etl_parent == &data5);
      CHECK(data3.SecondTLink::etl_left   == nullptr);
      CHECK(data3.SecondTLink::etl_right  == nullptr);
    }
  };
}
