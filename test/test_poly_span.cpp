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

    virtual bool operator==(const Base& o) const { return value() == o.value(); }
  };

  //***************************************************************************
  struct Derived : Base 
  {
    Derived(int v) : m_value(v) {}

    int value() const override { return m_value; }

    int m_value;
  };

  //***************************************************************************
  struct AnotherBase 
  {
    virtual double another_value() const { return v; };

    double v = 0;
  };

  //***************************************************************************
  struct MultiDerived : AnotherBase, Base 
  {
    MultiDerived(int v) : m_value(v) {}

    int value() const override { return m_value; }

    int m_value;
  };

  //***************************************************************************
  template <typename TSpan, typename TSource>
  bool equal(const TSpan& span, TSource& source) 
  {
    auto sb = std::begin(source);
    return (span.size() == (std::end(source) - sb)) && std::equal(span.begin(), span.end(), sb);
  }

  SUITE(test_poly_span)
  {
    //*************************************************************************
    TEST(test_poly_span_defaultConstruct) 
    {
      etl::poly_span<Base> s{};
      CHECK(s.empty());
      CHECK_EQUAL(0, s.size());
      CHECK(s.begin() == s.end());
    }

    //*************************************************************************
    TEST(test_poly_span_constructFromVec) 
    {
      std::vector<int> v = { 1, 2, 3 };

      etl::poly_span<int> s = v;
      CHECK_EQUAL(s[0], v[0]);
      CHECK_EQUAL(s[1], v[1]);
      CHECK_EQUAL(s[2], v[2]);
      CHECK_EQUAL(v.size(), s.size());
      CHECK_EQUAL(sizeof(v[0]), s.element_size());
      CHECK(!s.empty());
    }

    //*************************************************************************
    TEST(test_poly_span_constructFromVecRespectConst) 
    {
      std::vector<Derived> v;
      const auto& cv = v;
      etl::poly_span<const Base> s1 = v;
      etl::poly_span<const Base>{v};
      etl::poly_span<const Base> s2 = cv;
      etl::poly_span<const Base>{cv};
      etl::poly_span<Derived> s3 = v;
      etl::poly_span<Base>{v};
      // The line below shouldn't work if uncommented
      // etl::poly_span<int>{cv};
    }

    //*************************************************************************
    TEST(test_poly_span_constructFromCArray) 
    {
      int a[] = { 1, 2, 3 };
      etl::poly_span<const int> s = a;
      CHECK_EQUAL(s[0], a[0]);
      CHECK_EQUAL(s[1], a[1]);
      CHECK_EQUAL(s[2], a[2]);
      CHECK_EQUAL(3, s.size());
      CHECK_EQUAL(sizeof(a[0]), s.element_size());
    }

    //*************************************************************************
    TEST(test_poly_span_constructFromCArrayRespectConst) 
    {
      Derived ar[2] = { 1, 2 };
      const auto& car = ar;
      etl::poly_span<const Base> s1 = ar;
      etl::poly_span<const Base>{ar};
      etl::poly_span<const Derived> s2 = car;
      etl::poly_span<const Derived>{car};
      etl::poly_span<Base> s3 = ar;
      etl::poly_span<Base>{ar};
      // The line below shouldn't work if uncommented
      // etl::poly_span<int>{car};
    }

    //*************************************************************************
    TEST(test_poly_span_constructFromArray) 
    {
      std::array<int, 3> a = { 1, 2, 3 };
      
      etl::poly_span<int> s = a;
      CHECK_EQUAL(s[0], a[0]);
      CHECK_EQUAL(s[1], a[1]);
      CHECK_EQUAL(s[2], a[2]);
    }

    //*************************************************************************
    TEST(test_poly_span_constructFromPolySpan) 
    {
      std::array<Derived, 3> a = { 1, 2, 3 };
      etl::poly_span<Derived> s1 = a;
      etl::poly_span<Base> s2 = s1;
      etl::poly_span<const Base>{s2};
      CHECK(equal(s1, s2));
    }

    //*************************************************************************
    TEST(test_poly_span_constructFromIterator) 
    {
      std::vector<int> v = { 1, 2, 3, 4 };
      
      etl::poly_span<int> s1{ v.begin() + 1, v.end() };
      CHECK_EQUAL(v.size() - 1, s1.size());
      CHECK_EQUAL(s1[0], v[1]);
      CHECK_EQUAL(s1[1], v[2]);
      CHECK_EQUAL(s1[2], v[3]);

      etl::poly_span<const int> s2{ v.data() + 1, v.data() + v.size() };
      CHECK(equal(s1, s2));
      
      etl::poly_span<const int> s3{ v.cbegin(), v.size() };
      CHECK(equal(s3, v));
    }

    //*************************************************************************
    TEST(test_poly_span_constructFromInitializerList) 
    {
      const int data[] = { 1, 2, 3, 4, 5 };
      etl::poly_span<const int> s(data);

      CHECK_EQUAL(5, s.size());
      
      for (int i = 0; i < 5; ++i) 
      {
        //int x = s[i];
        //CHECK_EQUAL(i + 1, s[i]);
      }
    }

    //*************************************************************************
    TEST(test_poly_span_DerivedToBase) 
    {
      std::vector<Derived> v1 = { 0, 1, 2 };
      
      Derived a1[] = { 0, 1, 2 };
      CHECK(sizeof(Derived) > sizeof(Base));
      
      etl::poly_span<const Base> s1 = v1;
      CHECK_EQUAL(sizeof(Derived), s1.element_size());
      etl::poly_span<Base> s2 = a1;

      auto it1 = s1.begin();
      auto it2 = s2.begin();

      for (int i = 0; i < 3; ++i, ++it1, ++it2) 
      {
        CHECK_EQUAL(i, s1[i].value());
        CHECK_EQUAL(i, s2[i].value());
        CHECK_EQUAL(i, it1->value());
        CHECK_EQUAL(i, it2->value());
      }
    }

    //*************************************************************************
    TEST(test_poly_span_MultiDerivedToBase) 
    {
      std::vector<MultiDerived> v1 = { 0, 1, 2 };

      etl::poly_span<Base> s1 = v1;
      CHECK(static_cast<void*>(v1.data()) != static_cast<void*>(&s1.front()));
      CHECK(equal(s1, v1));
    }

    //*************************************************************************
    TEST(test_poly_span_typeProperties) 
    {
      CHECK(std::is_trivial<etl::poly_span<int>::iterator>::value);
      CHECK(std::is_trivial<etl::poly_span<const Base>::iterator>::value);
      CHECK(std::is_trivial<etl::poly_span<int>::const_iterator>::value);
      CHECK(bool(std::is_base_of<ETL_OR_STD::random_access_iterator_tag, etl::poly_span<int>::iterator::iterator_category>::value));
      CHECK(std::is_trivially_copy_constructible<etl::poly_span<int>>::value);
      CHECK(std::is_trivially_copy_assignable<etl::poly_span<int>>::value);
    }

    //*************************************************************************
    //TEST(test_poly_span_iterators) 
    //{
    //  std::vector<int> v = { 1, 2, 3 };
    //  etl::poly_span<int> s = v;      
    //  using iterator = etl::poly_span<int>::iterator;
    //  
    //  CHECK_EQUAL(s.end() - s.begin(), s.size());
    //  CHECK_EQUAL(s.rend() - s.rbegin(), s.size());
    //  
    //  iterator b = s.begin();
    //  CHECK(b + s.size() == s.end());
    //  CHECK_EQUAL(b[0], 1);
    //  CHECK(b < s.end());
    //  CHECK(b != s.end());
    //  CHECK(b + 2 > s.end() - 1);

    //  etl::poly_span<int>::const_iterator bc = b;
    //  CHECK(bc <= s.end());
    //  CHECK_EQUAL(s.back(), *s.crbegin());
    //  CHECK_EQUAL(s.front(), s.crend()[-1]);
    //  
    //  bc += 1;
    //  bc -= 1;
    //  CHECK(bc == s.cbegin());
    //  CHECK(std::is_sorted(s.cbegin(), s.cend()));
    //}

    //*************************************************************************
    TEST(test_poly_span_randomAccessIteratorUse) 
    {
      int data[] = { 25, 4, 3, -2, 1 };
      etl::poly_span<int> s = data;
      
      std::sort(s.begin(), s.end());
      CHECK(std::is_sorted(std::begin(data), std::end(data)));
    }

    //*************************************************************************
    TEST(test_poly_span_subspan) 
    {
      const int data[] = { 1, 2, 3, 4, 5 };
      etl::poly_span<const int> s(data);

      auto s1 = s.subspan(1, 2);
      CHECK_EQUAL(2, s1.size());
      CHECK_EQUAL(s[1], s1[0]);
      CHECK_EQUAL(s[2], s1[1]);

      auto s2 = s.subspan(2);
      CHECK_EQUAL(3, s2.size());
      CHECK_EQUAL(s[2], s2[0]);
      CHECK_EQUAL(s[4], s2[2]);

      auto s3 = s.subspan(0, 10);
      CHECK_EQUAL(s.size(), s3.size());
      CHECK_THROW(s.subspan(6, 1), etl::poly_span_out_of_range);
    }

    //*************************************************************************
    TEST(test_poly_span_first) 
    {
      const int data[] = { 1, 2, 3, 4, 5 };
      etl::poly_span<const int> s(data);
      
      CHECK_THROW(s.first(6), etl::poly_span_out_of_range);
      CHECK_EQUAL(s.first(5).size(), 5);
      CHECK(s.first(0).empty());
      
      auto s1 = s.first(3);
      CHECK_EQUAL(3, s1.size());
      CHECK_EQUAL(1, s1[0]);
      CHECK_EQUAL(3, s1[2]);
    }

    //*************************************************************************
    TEST(test_poly_span_last) 
    {
      const int data[] = { 1, 2, 3, 4, 5 };
      etl::poly_span<const int> s(data);
      
      CHECK_THROW(s.last(6), etl::poly_span_out_of_range);
      CHECK_EQUAL(s.last(5).size(), 5);
      
      auto s2 = s.last(2);
      CHECK_EQUAL(2, s2.size());
      CHECK_EQUAL(4, s2[0]);
      CHECK_EQUAL(5, s2[1]);
    }

    //*************************************************************************
    TEST(test_poly_span_equal_view) 
    {
      std::vector<double> v = { 1.0, 2.0, 3.0 };
      etl::poly_span<double> s1 = v;
      etl::poly_span<const double> cs2 = v;

      CHECK(!etl::poly_span<const double>{}.equal_view(s1));
      CHECK(s1.equal_view(cs2));
      CHECK(s1.last(2).equal_view(cs2.subspan(1)));
    }

    //*************************************************************************
    TEST(test_poly_span_at) 
    {
      const int data[] = { 1, 2, 3 };
      etl::poly_span<const int> s(data);
      
      CHECK_EQUAL(s.at(0), 1);
      CHECK_EQUAL(s.at(2), 3);
      CHECK_THROW(s.at(3), etl::poly_span_out_of_range);
      CHECK_THROW(s.at(-1), etl::poly_span_out_of_range);
    }
  }
}
