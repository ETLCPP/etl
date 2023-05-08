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
    virtual void value(int) = 0;

    virtual ~Base()
    {
    }

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

    void value(int i) override
    {
      v1 = i;
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

    void value(int i) override
    {
      v1 = i;
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
      CHECK(s.begin() == s.end());
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
      CHECK_EQUAL(ETL_OR_STD17::size(data), s.size());
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
      CHECK_EQUAL(ETL_OR_STD17::size(data), s.size());
      CHECK_EQUAL(s.size() * sizeof(Derived), s.size_bytes());
      CHECK_EQUAL(data[0].value(), s[0].value());
      CHECK_EQUAL(data[1].value(), s[1].value());
      CHECK_EQUAL(data[2].value(), s[2].value());
      CHECK_EQUAL(data[3].value(), s[3].value());
    }

#if ETL_USING_STL
    //*************************************************************************
    TEST(test_poly_span_construct_from_std_array) 
    {
      std::array<Derived, 4> data{ Derived(1), Derived(2), Derived(3), Derived(4) };
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
      const std::array<Derived, 4> data{ Derived(1), Derived(2), Derived(3), Derived(4) };
      etl::poly_span<const Base> s(data);
      CHECK(!s.empty());
      CHECK_EQUAL(data.size(), s.size());
      CHECK_EQUAL(s.size() * sizeof(Derived), s.size_bytes());
      CHECK_EQUAL(data[0].value(), s[0].value());
      CHECK_EQUAL(data[1].value(), s[1].value());
      CHECK_EQUAL(data[2].value(), s[2].value());
      CHECK_EQUAL(data[3].value(), s[3].value());
    }
#endif

    //*************************************************************************
    TEST(test_poly_span_construct_from_etl_array)
    {
      etl::array<Derived, 4> data{ Derived(1), Derived(2), Derived(3), Derived(4) };
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
      const etl::array<Derived, 4> data{ Derived(1), Derived(2), Derived(3), Derived(4) };
      etl::poly_span<const Base> s(data);
      CHECK(!s.empty());
      CHECK_EQUAL(data.size(), s.size());
      CHECK_EQUAL(s.size() * sizeof(Derived), s.size_bytes());
      CHECK_EQUAL(data[0].value(), s[0].value());
      CHECK_EQUAL(data[1].value(), s[1].value());
      CHECK_EQUAL(data[2].value(), s[2].value());
      CHECK_EQUAL(data[3].value(), s[3].value());
    }

#if ETL_USING_STL
    //*************************************************************************
    TEST(test_poly_span_copy_construct) 
    {
      std::array<Derived, 4> a = { 1, 2, 3, 4 };
      etl::poly_span<Base> s1 = a;
      etl::poly_span<Base> s2 = s1;
      const etl::poly_span<const Base> s3 = s1;
      CHECK(!s2.empty());
      CHECK(!s3.empty());
      CHECK(std::equal(s1.begin(), s1.end(), s2.begin()));
    }

    //*************************************************************************
    TEST(test_poly_span_assignment)
    {
      std::array<Derived, 4> a = { 1, 2, 3, 4 };
      etl::poly_span<Base> s1 = a;
      etl::poly_span<Base> s2;
      etl::poly_span<const Base> s3;

      s2 = s1;
      s3 = s1;

      CHECK(!s2.empty());
      CHECK(!s3.empty());
      CHECK(std::equal(s1.begin(), s1.end(), s2.begin()));
    }
#endif

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

    //*************************************************************************
    TEST(test_poly_span_front_back)
    {
      Derived data[] = { 1, 2, 3, 4 };
      etl::poly_span<Base> s(data);

      CHECK_EQUAL(data[0].value(), s.front().value());
      CHECK_EQUAL(data[3].value(), s.back().value());
    }

    //*************************************************************************
    TEST(test_poly_span_const_front_back)
    {
      const Derived data[] = { 1, 2, 3, 4 };
      const etl::poly_span<const Base> s(data);

      CHECK_EQUAL(data[0].value(), s.front().value());
      CHECK_EQUAL(data[3].value(), s.back().value());
    }

    //*************************************************************************
    TEST(test_poly_span_iterator)
    {
      Derived data[] = { 1, 2, 3, 4 };
      etl::poly_span<Base> s1(data);

      etl::poly_span<Base>::iterator itr1 = s1.begin();

      CHECK(itr1 != s1.end());

      CHECK_EQUAL(data[0].value(), (*itr1).value());
      CHECK_EQUAL(data[0].value(), itr1->value());
      ++itr1;
      CHECK_EQUAL(data[1].value(), (*itr1).value());
      CHECK_EQUAL(data[1].value(), itr1->value());
      itr1++;
      CHECK_EQUAL(data[2].value(), (*itr1).value());
      CHECK_EQUAL(data[2].value(), itr1->value());
      itr1 += 1;
      CHECK_EQUAL(data[3].value(), (*itr1).value());
      CHECK_EQUAL(data[3].value(), itr1->value());
      itr1 = itr1 + 1;
      CHECK(itr1 == s1.end());
      itr1 -= 1;
      CHECK_EQUAL(data[3].value(), (*itr1).value());
      CHECK_EQUAL(data[3].value(), itr1->value());
      itr1 = itr1 - 1;
      CHECK_EQUAL(data[2].value(), (*itr1).value());
      CHECK_EQUAL(data[2].value(), itr1->value());

      itr1 = s1.begin();
      etl::poly_span<Base>::iterator itr2 = itr1 + 1;

      CHECK(itr1 <  itr2);
      CHECK(itr1 <= itr1);
      CHECK(itr1 <= itr2);
      CHECK(itr2 >  itr1);
      CHECK(itr2 >= itr2);
      CHECK(itr2 >= itr1);     
      CHECK(itr1 == itr1);
      CHECK(itr2 == itr2);
      CHECK(itr1 != itr2);
      CHECK(itr2 != itr1);

      CHECK(!(itr1 > itr2));
      CHECK(!(itr2 <= itr1));
      CHECK(!(itr2 < itr1));
      CHECK(!(itr1 >= itr2));
      CHECK(!(itr1 == itr2));
      CHECK(!(itr2 == itr1));
      CHECK(!(itr1 != itr1));
      CHECK(!(itr2 != itr2));

      itr1 = s1.begin();

      (*itr1).value(2);
      CHECK_EQUAL(2, (*itr1).value());

      itr1->value(3);
      CHECK_EQUAL(3, itr1->value());

      etl::poly_span<Base> s2;
      CHECK(s2.begin() == s2.end());
    }

    //*************************************************************************
    TEST(test_poly_span_reverse_iterator)
    {
      Derived data[] = { 1, 2, 3, 4 };
      etl::poly_span<Base> s1(data);

      etl::poly_span<Base>::reverse_iterator itr1 = s1.rbegin();

      CHECK(itr1 != s1.rend());

      CHECK_EQUAL(data[3].value(), (*itr1).value());
      CHECK_EQUAL(data[3].value(), itr1->value());
      ++itr1;
      CHECK_EQUAL(data[2].value(), (*itr1).value());
      CHECK_EQUAL(data[2].value(), itr1->value());
      itr1++;
      CHECK_EQUAL(data[1].value(), (*itr1).value());
      CHECK_EQUAL(data[1].value(), itr1->value());
      itr1 += 1;
      CHECK_EQUAL(data[0].value(), (*itr1).value());
      CHECK_EQUAL(data[0].value(), itr1->value());
      itr1 = itr1 + 1;
      CHECK(itr1 == s1.rend());
      itr1 -= 1;
      CHECK_EQUAL(data[0].value(), (*itr1).value());
      CHECK_EQUAL(data[0].value(), itr1->value());
      itr1 = itr1 - 1;
      CHECK_EQUAL(data[1].value(), (*itr1).value());
      CHECK_EQUAL(data[1].value(), itr1->value());
    }

    //*************************************************************************
    TEST(test_poly_span_random_access_iterator_use) 
    {
      int data[] = { 25, 4, 3, -2, 1 };
      etl::poly_span<int> s = data;
      
      std::sort(s.begin(), s.end());
      CHECK(std::is_sorted(std::begin(data), std::end(data)));
    }

    //*************************************************************************
    TEST(test_poly_span_subspan) 
    {
      Derived data[] = { 1, 2, 3, 4, 5 };
      etl::poly_span<Base> s(data);

      auto s1 = s.subspan(1, 2);
      CHECK_EQUAL(2, s1.size());

      CHECK_EQUAL(s[1].value(), s1[0].value());
      CHECK_EQUAL(s[2].value(), s1[1].value());

      auto s2 = s.subspan(2);
      CHECK_EQUAL(3, s2.size());
      CHECK_EQUAL(s[2].value(), s2[0].value());
      CHECK_EQUAL(s[3].value(), s2[1].value());
      CHECK_EQUAL(s[4].value(), s2[2].value());
    }

    //*************************************************************************
    TEST(test_poly_span_first)
    {
      Derived data[] = { 1, 2, 3, 4 };
      etl::poly_span<Base> s1(data);

      auto s2 = s1.first(3);
      auto s3 = s1.first<3>();

      CHECK_EQUAL(data[0].value(), s2[0].value());
      CHECK_EQUAL(data[1].value(), s2[1].value());
      CHECK_EQUAL(data[2].value(), s2[2].value());

      CHECK_EQUAL(data[0].value(), s3[0].value());
      CHECK_EQUAL(data[1].value(), s3[1].value());
      CHECK_EQUAL(data[2].value(), s3[2].value());
    }

    //*************************************************************************
    TEST(test_poly_span_last)
    {
      Derived data[] = { 1, 2, 3, 4 };
      etl::poly_span<Base> s1(data);

      auto s2 = s1.last(3);
      auto s3 = s1.last<3>();

      CHECK_EQUAL(data[1].value(), s2[0].value());
      CHECK_EQUAL(data[2].value(), s2[1].value());
      CHECK_EQUAL(data[3].value(), s2[2].value());

      CHECK_EQUAL(data[1].value(), s3[0].value());
      CHECK_EQUAL(data[2].value(), s3[1].value());
      CHECK_EQUAL(data[3].value(), s3[2].value());
    }

    //*************************************************************************
    TEST(test_hash)
    {
      int data[] = { 1, 2, 3, 4 };
      etl::poly_span<int> s1(data);

      size_t hashdata = etl::private_hash::generic_hash<size_t>(reinterpret_cast<const uint8_t*>(&s1[0]),
                                                                reinterpret_cast<const uint8_t*>(&s1[s1.size()]));

      size_t hashview = etl::hash<etl::poly_span<int>>()(s1);

      CHECK_EQUAL(hashdata, hashview);
    }

#if ETL_USING_CPP17
    //*************************************************************************
    TEST(test_template_deduction_guide_for_c_array)
    {
      int data[] = { 1, 2, 3, 4 };

      etl::poly_span s = data;

      CHECK_EQUAL(ETL_OR_STD17::size(data), s.extent);
      CHECK_EQUAL(ETL_OR_STD17::size(data), s.size());
      CHECK((std::is_same_v<int, std::remove_reference_t<decltype(s.front())>>));
    }

#if ETL_USING_STL
    //*************************************************************************
    TEST(test_template_deduction_guide_for_std_array)
    {
      std::array<int, 4U> data = { 1, 2, 3, 4 };

      etl::poly_span s = data;

      CHECK_EQUAL(ETL_OR_STD17::size(data), s.extent);
      CHECK_EQUAL(ETL_OR_STD17::size(data), s.size());
      CHECK((std::is_same_v<int, std::remove_reference_t<decltype(s.front())>>));
    }
#endif

    //*************************************************************************
    TEST(test_template_deduction_guide_for_etl_array)
    {
      etl::array<int, 4U> data = { 1, 2, 3, 4 };

      etl::poly_span s = data;

      CHECK_EQUAL(ETL_OR_STD17::size(data), s.extent);
      CHECK_EQUAL(ETL_OR_STD17::size(data), s.size());
      CHECK((std::is_same_v<int, std::remove_reference_t<decltype(s.front())>>));
    }

    //*************************************************************************
    TEST(test_template_deduction_guide_for_iterators)
    {
      etl::array<int, 4U> data = { 1, 2, 3, 4 };

      etl::poly_span s{ data.begin(), data.end() };

      CHECK_EQUAL(etl::dynamic_extent, s.extent);
      CHECK_EQUAL(4U, s.size());
      CHECK((std::is_same_v<int, std::remove_reference_t<decltype(s.front())>>));
    }

    //*************************************************************************
    TEST(test_template_deduction_guide_for_iterator_and_size)
    {
      etl::array<int, 4U> data = { 1, 2, 3, 4 };

      etl::poly_span s{ data.begin(), data.size() };

      CHECK_EQUAL(etl::dynamic_extent, s.extent);
      CHECK_EQUAL(4U, s.size());
      CHECK((std::is_same_v<int, std::remove_reference_t<decltype(s.front())>>));
    }
#endif
  }
}
