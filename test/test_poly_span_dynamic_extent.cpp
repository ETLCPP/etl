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

#include "etl/poly_span.h"

#include <array>
#include <vector>
#include <algorithm>
#include <iterator>
#include <vector>

namespace
{
  //***************************************************************************
  struct Base 
  {
    virtual int value() const = 0;

    virtual bool operator ==(const Base& rhs) const 
    { 
      return value() == rhs.value(); 
    }
  };

  //***************************************************************************
  struct AnotherBase 
  {
    virtual double another_value() const = 0;

    virtual bool operator ==(const AnotherBase& rhs) const
    {
      return another_value() == rhs.another_value();
    }
  };

  //***************************************************************************
  struct Derived : Base
  {
    Derived(int v_)
      : v1(v_)
    {
    }

    int value() const override
    {
      return v1;
    }

    int v1;
  };

  //***************************************************************************
  struct MultiDerived : AnotherBase, Base 
  {
    MultiDerived(int v1_, double v2_) 
      : v1(v1_)
      , v2(v2_)
    {
    }

    int value() const override 
    { 
      return v1; 
    }

    double another_value() const override
    {
      return v2;
    }

    int v1;
    double v2;
  };

  //***************************************************************************
  template <typename TSpan, typename TSource>
  bool equal(const TSpan& span, TSource& source) 
  {
    auto sb = std::begin(source);
    return (span.size() == (std::end(source) - sb)) && std::equal(span.begin(), span.end(), sb);
  }

  SUITE(test_poly_span_dynamic_extent)
  {
    //*************************************************************************
    TEST(test_poly_span_default_construct) 
    {
      etl::poly_span<Base> s;
      CHECK_EQUAL(0U, s.size_of_element());
      CHECK(s.empty());
      CHECK_EQUAL(0U, s.size());
      CHECK_EQUAL(0U, s.size_bytes());
      //CHECK(s.begin() == s.end());
    }

    //*************************************************************************
    TEST(test_poly_span_construct_from_iterators) 
    {
      std::vector<Derived> data = { 1, 2, 3, 4 };

      etl::poly_span<Base> s(data.begin(), data.end());
      CHECK_EQUAL(sizeof(Derived), s.size_of_element());
      CHECK(!s.empty());
      CHECK_EQUAL(data.size(), s.size());
      CHECK_EQUAL(s.size() * sizeof(Derived), s.size_bytes());
      CHECK_EQUAL(data[0].value(), s[0].value());
      CHECK_EQUAL(data[1].value(), s[1].value());
      CHECK_EQUAL(data[2].value(), s[2].value());
      CHECK_EQUAL(data[3].value(), s[3].value());
    }

    //*************************************************************************
    TEST(test_poly_span_construct_from_c_array) 
    {
      Derived data[] = { 1, 2, 3, 4 };
      etl::poly_span<Base> s(data);
      CHECK_EQUAL(sizeof(Derived), s.size_of_element());
      CHECK(!s.empty());
      CHECK_EQUAL(std::size(data), s.size());
      CHECK_EQUAL(s.size() * sizeof(Derived), s.size_bytes());
      CHECK_EQUAL(data[0].value(), s[0].value());
      CHECK_EQUAL(data[1].value(), s[1].value());
      CHECK_EQUAL(data[2].value(), s[2].value());
      CHECK_EQUAL(data[3].value(), s[3].value());
    }

    //*************************************************************************
    TEST(test_poly_span_construct_from_const_c_array)
    {
      const Derived data[] = { 1, 2, 3, 4 };
      etl::poly_span<const Base> s(data);
      CHECK_EQUAL(sizeof(Derived), s.size_of_element());
      CHECK(!s.empty());
      CHECK_EQUAL(std::size(data), s.size());
      CHECK_EQUAL(s.size() * sizeof(Derived), s.size_bytes());
      CHECK_EQUAL(data[0].value(), s[0].value());
      CHECK_EQUAL(data[1].value(), s[1].value());
      CHECK_EQUAL(data[2].value(), s[2].value());
      CHECK_EQUAL(data[3].value(), s[3].value());
    }

    //*************************************************************************
    TEST(test_poly_span_construct_from_std_array) 
    {
      std::array data{ Derived(1), Derived(2), Derived(3), Derived(4) };
      etl::poly_span<Base> s(data);
      CHECK(!s.empty());
      CHECK_EQUAL(data.size(), s.size());
      CHECK_EQUAL(s.size() * sizeof(Derived), s.size_bytes());
      CHECK_EQUAL(data[0].value(), s[0].value());
      CHECK_EQUAL(data[1].value(), s[1].value());
      CHECK_EQUAL(data[2].value(), s[2].value());
      CHECK_EQUAL(data[3].value(), s[3].value());
    }

    //*************************************************************************
    TEST(test_poly_span_construct_from_const_std_array)
    {
      const std::array data{ Derived(1), Derived(2), Derived(3), Derived(4) };
      etl::poly_span<const Base> s(data);
      CHECK(!s.empty());
      CHECK_EQUAL(data.size(), s.size());
      CHECK_EQUAL(s.size() * sizeof(Derived), s.size_bytes());
      CHECK_EQUAL(data[0].value(), s[0].value());
      CHECK_EQUAL(data[1].value(), s[1].value());
      CHECK_EQUAL(data[2].value(), s[2].value());
      CHECK_EQUAL(data[3].value(), s[3].value());
    }

    //*************************************************************************
    TEST(test_poly_span_construct_from_etl_array)
    {
      etl::array data{ Derived(1), Derived(2), Derived(3), Derived(4) };
      etl::poly_span<Base> s(data);
      CHECK(!s.empty());
      CHECK_EQUAL(data.size(), s.size());
      CHECK_EQUAL(s.size() * sizeof(Derived), s.size_bytes());
      CHECK_EQUAL(data[0].value(), s[0].value());
      CHECK_EQUAL(data[1].value(), s[1].value());
      CHECK_EQUAL(data[2].value(), s[2].value());
      CHECK_EQUAL(data[3].value(), s[3].value());
    }

    //*************************************************************************
    TEST(test_poly_span_construct_from_const_etl_array)
    {
      const etl::array data{ Derived(1), Derived(2), Derived(3), Derived(4) };
      etl::poly_span<const Base> s(data);
      CHECK(!s.empty());
      CHECK_EQUAL(data.size(), s.size());
      CHECK_EQUAL(s.size() * sizeof(Derived), s.size_bytes());
      CHECK_EQUAL(data[0].value(), s[0].value());
      CHECK_EQUAL(data[1].value(), s[1].value());
      CHECK_EQUAL(data[2].value(), s[2].value());
      CHECK_EQUAL(data[3].value(), s[3].value());
    }

    //*************************************************************************
    TEST(test_poly_span_copy_construct) 
    {
      std::array<Derived, 4> a = { 1, 2, 3, 4 };
      etl::poly_span<Base> s1 = a;
      etl::poly_span<Base> s2 = s1;
      const etl::poly_span<const Base> s3 = s1;
      //CHECK(equal(s1, s2));
      CHECK(!s2.empty());
      CHECK(!s3.empty());
    }

    //*************************************************************************
    TEST(test_poly_span_multi_derived) 
    {
      std::vector<MultiDerived> data = { { 0, 1.2 }, { 1, 2.3 }, { 2, 3.4 }, { 3, 4.5 } };

      etl::poly_span<Base> s1(data.begin(), data.end());
      etl::poly_span<AnotherBase> s2(data.begin(), data.end());

      CHECK_EQUAL(data[0].value(), s1[0].value());
      CHECK_EQUAL(data[1].value(), s1[1].value());
      CHECK_EQUAL(data[2].value(), s1[2].value());
      CHECK_EQUAL(data[3].value(), s1[3].value());

      CHECK_EQUAL(data[0].another_value(), s2[0].another_value());
      CHECK_EQUAL(data[1].another_value(), s2[1].another_value());
      CHECK_EQUAL(data[2].another_value(), s2[2].another_value());
      CHECK_EQUAL(data[3].another_value(), s2[3].another_value());
    }

    ////*************************************************************************
    //TEST(test_poly_span_typeProperties) 
    //{
    //  CHECK(std::is_trivial<etl::poly_span<int>::iterator>::value);
    //  CHECK(std::is_trivial<etl::poly_span<const Base>::iterator>::value);
    //  CHECK(std::is_trivial<etl::poly_span<int>::const_iterator>::value);
    //  CHECK(bool(std::is_base_of<ETL_OR_STD::random_access_iterator_tag, etl::poly_span<int>::iterator::iterator_category>::value));
    //  CHECK(std::is_trivially_copy_constructible<etl::poly_span<int>>::value);
    //  CHECK(std::is_trivially_copy_assignable<etl::poly_span<int>>::value);
    //}

    ////*************************************************************************
    ////TEST(test_poly_span_iterators) 
    ////{
    ////  std::vector<int> v = { 1, 2, 3 };
    ////  etl::poly_span<int> s = v;      
    ////  using iterator = etl::poly_span<int>::iterator;
    ////  
    ////  CHECK_EQUAL(s.end() - s.begin(), s.size());
    ////  CHECK_EQUAL(s.rend() - s.rbegin(), s.size());
    ////  
    ////  iterator b = s.begin();
    ////  CHECK(b + s.size() == s.end());
    ////  CHECK_EQUAL(b[0], 1);
    ////  CHECK(b < s.end());
    ////  CHECK(b != s.end());
    ////  CHECK(b + 2 > s.end() - 1);

    ////  etl::poly_span<int>::const_iterator bc = b;
    ////  CHECK(bc <= s.end());
    ////  CHECK_EQUAL(s.back(), *s.crbegin());
    ////  CHECK_EQUAL(s.front(), s.crend()[-1]);
    ////  
    ////  bc += 1;
    ////  bc -= 1;
    ////  CHECK(bc == s.cbegin());
    ////  CHECK(std::is_sorted(s.cbegin(), s.cend()));
    ////}

    //*************************************************************************
    TEST(test_poly_span_randomAccessIteratorUse) 
    {
      int data[] = { 25, 4, 3, -2, 1 };
      etl::poly_span<int> s = data;
      
//      std::sort(s.begin(), s.end());
//      CHECK(std::is_sorted(std::begin(data), std::end(data)));
    }

    //*************************************************************************
    TEST(test_poly_span_subspan) 
    {
      const Derived data[] = { 1, 2, 3, 4, 5 };
    //  etl::poly_span<const int> s(data);

    //  auto s1 = s.subspan(1, 2);
    //  CHECK_EQUAL(2, s1.size());
    //  CHECK_EQUAL(s[1], s1[0]);
    //  CHECK_EQUAL(s[2], s1[1]);

    //  auto s2 = s.subspan(2);
    //  CHECK_EQUAL(3, s2.size());
    //  CHECK_EQUAL(s[2], s2[0]);
    //  CHECK_EQUAL(s[4], s2[2]);

    //  auto s3 = s.subspan(0, 10);
    //  CHECK_EQUAL(s.size(), s3.size());
    //  CHECK_THROW(s.subspan(6, 1), etl::poly_span_out_of_range);
    }

    ////*************************************************************************
    //TEST(test_poly_span_first) 
    //{
    //  const int data[] = { 1, 2, 3, 4, 5 };
    //  etl::poly_span<const int> s(data);
    //  
    //  CHECK_THROW(s.first(6), etl::poly_span_out_of_range);
    //  CHECK_EQUAL(s.first(5).size(), 5);
    //  CHECK(s.first(0).empty());
    //  
    //  auto s1 = s.first(3);
    //  CHECK_EQUAL(3, s1.size());
    //  CHECK_EQUAL(1, s1[0]);
    //  CHECK_EQUAL(3, s1[2]);
    //}

    ////*************************************************************************
    //TEST(test_poly_span_last) 
    //{
    //  const int data[] = { 1, 2, 3, 4, 5 };
    //  etl::poly_span<const int> s(data);
    //  
    //  CHECK_THROW(s.last(6), etl::poly_span_out_of_range);
    //  CHECK_EQUAL(s.last(5).size(), 5);
    //  
    //  auto s2 = s.last(2);
    //  CHECK_EQUAL(2, s2.size());
    //  CHECK_EQUAL(4, s2[0]);
    //  CHECK_EQUAL(5, s2[1]);
    //}

    ////*************************************************************************
    //TEST(test_poly_span_equal_view) 
    //{
    //  std::vector<double> v = { 1.0, 2.0, 3.0 };
    //  etl::poly_span<double> s1 = v;
    //  etl::poly_span<const double> cs2 = v;

    //  CHECK(!etl::poly_span<const double>{}.equal_view(s1));
    //  CHECK(s1.equal_view(cs2));
    //  CHECK(s1.last(2).equal_view(cs2.subspan(1)));
    //}

    ////*************************************************************************
    //TEST(test_poly_span_at) 
    //{
    //  const int data[] = { 1, 2, 3 };
    //  etl::poly_span<const int> s(data);
    //  
    //  CHECK_EQUAL(s.at(0), 1);
    //  CHECK_EQUAL(s.at(2), 3);
    //  CHECK_THROW(s.at(3), etl::poly_span_out_of_range);
    //  CHECK_THROW(s.at(-1), etl::poly_span_out_of_range);
    //}
  }
}
