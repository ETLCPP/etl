/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl
https://www.etlcpp.com

Copyright(c) 2026 BMW AG

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

#include "etl/ranges.h"
#include "etl/vector.h"

#include <array>
#include <ios>
#include <list>
#include <sstream>
#include <vector>

// Issue #1391: include <ranges> when available to verify etl::operator|
// does not conflict with std::ranges::operator|.
#if ETL_USING_STL && ETL_USING_CPP20 && defined(__has_include)
  #if __has_include(<ranges>)
    #include <ranges>
  #endif
#endif

#if ETL_USING_CPP17

  // C++03 does not support move semantics as used in the ranges library
  #if !defined(ETL_FORCE_TEST_CPP03_IMPLEMENTATION)

namespace
{
  class MoveInt
  {
  public:

    MoveInt(int v)
      : _v{v}
    {
    }

    MoveInt(const MoveInt& other)
      : _v{other._v}
    {
    }

    MoveInt& operator=(const MoveInt& other)
    {
      _v = other._v;
      return *this;
    }

    MoveInt(MoveInt&& other)
      : _v{other._v}
    {
      other._v = 0;
    }

    MoveInt& operator=(MoveInt&& other)
    {
      _v       = other._v;
      other._v = 0;
      return *this;
    }

    int get() const
    {
      return _v;
    }

    bool operator==(const MoveInt& other) const
    {
      return _v == other._v;
    }

    bool operator!=(const MoveInt& other) const
    {
      return !(*this == other);
    }

  private:

    int _v;
  };

} // namespace

namespace std
{
  template < typename Container, etl::enable_if_t< etl::is_base_of_v<std::vector<int>, Container> || etl::is_base_of_v<etl::ivector<int>, Container>
                                                     || etl::is_base_of_v<etl::ranges::view_interface<Container>, Container>,
                                                   int> = 0>
  std::basic_ostream<char>& operator<<(std::basic_ostream<char>& s, const Container& v)
  {
    auto it    = v.cbegin();
    auto e     = v.cend();
    bool first = true;
    for (; it != e; ++it)
    {
      if (!first)
      {
        s << " ";
      }
      s << *it;
      first = false;
    }
    return s;
  }

  std::ostringstream& operator<<(std::ostringstream& s, const etl::ivector<MoveInt>& v)
  {
    auto it    = v.cbegin();
    auto e     = v.cend();
    bool first = true;
    for (; it != e; ++it)
    {
      if (!first)
      {
        s << " ";
      }
      s << it->get();
      first = false;
    }
    return s;
  }

  template <class T>
  std::ostringstream& operator<<(std::ostringstream& s, const etl::ranges::range_iterator<T>& v)
  {
    auto value{v.get()};
    s << value;
    return s;
  }
} // namespace std

namespace
{
    #include "etl/private/diagnostic_null_dereference_push.h"
  SUITE(test_ranges)
  {
    //*************************************************************************
    // Iterators.
    //*************************************************************************
    TEST(test_ranges_begin)
    {
      etl::vector<int, 10> v_in{3, 2, 1};

      auto it = etl::ranges::begin(v_in);

      CHECK_EQUAL(*it, 3);
    }

    TEST(test_ranges_end)
    {
      etl::vector<int, 10> v_in{3, 2, 1};

      auto it = etl::ranges::end(v_in);

      CHECK_EQUAL(it, ETL_OR_STD::end(v_in));
    }

    TEST(test_ranges_cbegin)
    {
      etl::vector<int, 10> v_in{3, 2, 1};

      auto it = etl::ranges::cbegin(v_in);

      CHECK_EQUAL(*it, 3);
    }

    TEST(test_ranges_cend)
    {
      etl::vector<int, 10> v_in{3, 2, 1};

      auto it = etl::ranges::cend(v_in);

      CHECK_EQUAL(it, ETL_OR_STD::cend(v_in));
    }

    TEST(test_ranges_rbegin)
    {
      etl::vector<int, 10> v_in{3, 2, 1};

      auto it = etl::ranges::rbegin(v_in);

      CHECK_EQUAL(*it, 1);
    }

    TEST(test_ranges_rend)
    {
      etl::vector<int, 10> v_in{3, 2, 1};

      auto it = etl::ranges::rend(v_in);

      CHECK_EQUAL(&(*it), &(*ETL_OR_STD::rend(v_in)));
    }

    TEST(test_ranges_crbegin)
    {
      etl::vector<int, 10> v_in{3, 2, 1};

      auto it = etl::ranges::crbegin(v_in);

      CHECK_EQUAL(*it, 1);
    }

    TEST(test_ranges_crend)
    {
      etl::vector<int, 10> v_in{3, 2, 1};

      auto it = etl::ranges::crend(v_in);

      CHECK(it == ETL_OR_STD::crend(v_in));
    }

    TEST(test_ranges_size)
    {
      etl::vector<int, 10> v_in{11, 3, 2, 1};

      CHECK_EQUAL(etl::ranges::size(v_in), 4);

      using size_type = decltype(etl::ranges::size(v_in));
      static_assert(etl::is_signed<size_type>::value == false, "Result of size must be unsigned");
    }

    TEST(test_ranges_ssize)
    {
      etl::vector<int, 10> v_in{11, 3, 2, 1};

      CHECK_EQUAL(etl::ranges::ssize(v_in), 4);

      using signed_type   = decltype(etl::ranges::ssize(v_in));
      using unsigned_type = decltype(etl::ranges::size(v_in));
      static_assert(etl::is_signed<signed_type>::value, "Result of ssize must be signed");
      static_assert(sizeof(signed_type) >= sizeof(unsigned_type), "Signed size type needs to be as wide as unsigned size type");
    }

    TEST(test_ranges_empty)
    {
      etl::vector<int, 10> v_in0{11, 3, 2, 1};
      etl::vector<int, 10> v_in1{};

      CHECK_EQUAL(etl::ranges::empty(v_in0), false);
      CHECK_EQUAL(etl::ranges::empty(v_in1), true);
    }

    TEST(test_ranges_data)
    {
      etl::vector<int, 10> v_in{11, 3, 2, 1};

      CHECK_EQUAL(*etl::ranges::data(v_in), 11);
    }

    TEST(test_ranges_cdata)
    {
      etl::vector<int, 10> v_in{11, 3, 2, 1};

      CHECK_EQUAL(*etl::ranges::cdata(v_in), 11);
    }

    //*************************************************************************
    // Range primitives.
    //*************************************************************************
    TEST(test_ranges_iterator_t)
    {
      using range_type = etl::vector<int, 10>;

      static_assert(etl::is_same<etl::ranges::iterator_t<range_type>, int*>::value, "Bad iterator type from etl::ranges::iterator_t");
    }

    TEST(test_ranges_const_iterator_t)
    {
      using range_type = etl::vector<int, 10>;

      static_assert(etl::is_same<etl::ranges::const_iterator_t<range_type>, const int*>::value,
                    "Bad iterator type from etl::ranges::const_iterator_t");
    }

    TEST(test_ranges_sentinel_t)
    {
      using range_type = etl::vector<int, 10>;

      static_assert(etl::is_same<etl::ranges::sentinel_t<range_type>, int*>::value, "Bad sentinel type from etl::ranges::sentinel_t");
    }

    TEST(test_ranges_const_sentinel_t)
    {
      using range_type = etl::vector<int, 10>;

      static_assert(etl::is_same<etl::ranges::const_sentinel_t<range_type>, const int*>::value,
                    "Bad sentinel type from etl::ranges::const_sentinel_t");
    }

    TEST(test_ranges_range_size_t)
    {
      using range_type0 = int[10];
      using range_type1 = etl::vector<int, 10>;
      using range_type2 = etl::ranges::empty_view<int>;

      static_assert(etl::is_same<etl::ranges::range_size_t<range_type0>, size_t>::value, "Bad size type from etl::ranges::range_size_t");
      static_assert(etl::is_same<etl::ranges::range_size_t<range_type1>, size_t>::value, "Bad size type from etl::ranges::range_size_t");
      static_assert(etl::is_same<etl::ranges::range_size_t<range_type2>, size_t>::value, "Bad size type from etl::ranges::range_size_t");
    }

    TEST(test_ranges_range_difference_t)
    {
      using range_type0 = int[10];
      using range_type1 = etl::vector<int, 10>;
      using range_type2 = etl::ranges::empty_view<int>;

      static_assert(etl::is_same<etl::ranges::range_difference_t<range_type0>, ptrdiff_t>::value,
                    "Bad size type from etl::ranges::range_difference_t");
      static_assert(etl::is_same<etl::ranges::range_difference_t<range_type1>, ptrdiff_t>::value,
                    "Bad size type from etl::ranges::range_difference_t");
      static_assert(etl::is_same<etl::ranges::range_difference_t<range_type2>, ptrdiff_t>::value,
                    "Bad size type from etl::ranges::range_difference_t");
    }

    TEST(test_ranges_range_value_t)
    {
      using range_type0 = int[10];
      using range_type1 = etl::vector<int, 10>;
      using range_type2 = etl::ranges::empty_view<int>;

      static_assert(etl::is_same<etl::ranges::range_value_t<range_type0>, int>::value, "Bad size type from etl::ranges::range_value_t");
      static_assert(etl::is_same<etl::ranges::range_value_t<range_type1>, int>::value, "Bad size type from etl::ranges::range_value_t");
      static_assert(etl::is_same<etl::ranges::range_value_t<range_type2>, int>::value, "Bad size type from etl::ranges::range_value_t");
    }

    TEST(test_ranges_subrange)
    {
      etl::vector<int, 10> v{0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
      auto                 sr = etl::ranges::subrange{v.begin(), v.end()};

      CHECK_EQUAL(sr.begin(), v.begin());
      CHECK_EQUAL(sr.end(), v.end());
      CHECK_EQUAL(sr.empty(), false);
      CHECK_EQUAL(sr.size(), 10);

      sr.advance(1);
      CHECK_EQUAL(sr.size(), 9);
      CHECK_EQUAL(sr[0], 1);

      sr.advance(2);
      CHECK_EQUAL(sr.size(), 7);
      CHECK_EQUAL(sr[0], 3);

      CHECK_EQUAL(sr.next().size(), 6);
      CHECK_EQUAL(sr.next()[0], 4);
      CHECK_EQUAL(sr.next(1).size(), 6);
      CHECK_EQUAL(sr.next(1)[0], 4);
      CHECK_EQUAL(sr.next(2).size(), 5);
      CHECK_EQUAL(sr.next(2)[0], 5);

      CHECK_EQUAL(sr.prev().size(), 8);
      CHECK_EQUAL(sr.prev()[0], 2);
      CHECK_EQUAL(sr.prev(1).size(), 8);
      CHECK_EQUAL(sr.prev(1)[0], 2);
      CHECK_EQUAL(sr.prev(2).size(), 9);
      CHECK_EQUAL(sr.prev(2)[0], 1);
    }

    //*************************************************************************
    // Range factories.
    //*************************************************************************
    TEST(test_ranges_empty_view)
    {
      auto ev = etl::ranges::empty_view<int>{};

      CHECK_EQUAL(ev.begin(), nullptr);
      CHECK_EQUAL(ev.end(), nullptr);
      CHECK_EQUAL(ev.data(), nullptr);
      CHECK_EQUAL(ev.size(), 0);
      CHECK_EQUAL(ev.empty(), true);
    }

    TEST(test_ranges_views_empty)
    {
      auto e = etl::ranges::views::empty<int>;

      CHECK_EQUAL(e.begin(), nullptr);
      CHECK_EQUAL(e.end(), nullptr);
      CHECK_EQUAL(e.data(), nullptr);
      CHECK_EQUAL(e.size(), 0);
      CHECK_EQUAL(e.empty(), true);
    }

    TEST(test_ranges_single_view)
    {
      auto s0 = etl::ranges::single_view<int>(12);
      CHECK_EQUAL(s0.size(), 1);

      auto s = etl::ranges::single_view(23);

      CHECK_EQUAL(*s.begin(), 23);
      CHECK_EQUAL(s.end(), s.begin() + 1);
      CHECK_EQUAL(*s.data(), 23);
      CHECK_EQUAL(s.size(), 1);
      CHECK_EQUAL(s.empty(), false);

      *s.begin() = 45;
      CHECK_EQUAL(*s.data(), 45);
      CHECK_EQUAL(*s.begin(), 45);
    }

    TEST(test_ranges_views_single)
    {
      auto s = etl::ranges::views::single(23);

      CHECK_EQUAL(*s.begin(), 23);
      CHECK_EQUAL(s.end(), s.begin() + 1);
      CHECK_EQUAL(*s.data(), 23);
      CHECK_EQUAL(s.size(), 1);
      CHECK_EQUAL(s.empty(), false);

      *s.begin() = 45;
      CHECK_EQUAL(*s.data(), 45);
      CHECK_EQUAL(*s.begin(), 45);
    }

    TEST(test_ranges_iota_view)
    {
      auto iv = etl::ranges::iota_view(1, 7);

      int compare = 1;
      for (auto i : iv)
      {
        CHECK_EQUAL(i, compare);
        ++compare;
      }

      CHECK_EQUAL(*iv.begin(), 1);
      CHECK_EQUAL(iv.end(), iv.begin() + 6);
      CHECK_EQUAL(iv.size(), 6);
      CHECK_EQUAL(iv.empty(), false);

      CHECK_EQUAL(iv[4], 5);

      auto iv2 = etl::ranges::iota_view(3);
      CHECK_EQUAL(iv2[0], 3);
      CHECK_EQUAL(iv2[1], 4);
      CHECK_EQUAL(iv2[2], 5);
      CHECK_EQUAL(iv2[3], 6);
      CHECK_EQUAL(iv2[4], 7);

      auto iv3 = etl::ranges::iota_view<int>();
      CHECK_EQUAL(iv3.size(), 0);
      CHECK_EQUAL(iv3.end(), iv3.begin());
      CHECK_EQUAL(iv3.empty(), true);
    }

    TEST(test_ranges_views_iota)
    {
      auto iv = etl::ranges::views::iota(1, 7);

      CHECK_EQUAL(*iv.begin(), 1);
      CHECK_EQUAL(iv.end(), iv.begin() + 6);
      CHECK_EQUAL(iv.size(), 6);
      CHECK_EQUAL(iv.empty(), false);

      CHECK_EQUAL(iv[4], 5);
    }

    TEST(test_ranges_iota_view_pipe_take)
    {
      auto iv = etl::ranges::iota_view(3) | etl::views::take(5);

      auto i = iv.begin();
      auto j = i;
      CHECK_EQUAL(*i, 3);
      i++;
      CHECK_EQUAL(*i, 4);
      ++i;
      CHECK_EQUAL(*i, 5);

      CHECK_EQUAL(i - j, 2);

      CHECK_EQUAL(iv.size(), 5);
      CHECK_EQUAL(iv.front(), 3);
      CHECK_EQUAL(iv.back(), 7);
      CHECK_EQUAL(iv[0], 3);
      CHECK_EQUAL(iv[1], 4);
      CHECK_EQUAL(iv[2], 5);
      CHECK_EQUAL(iv[3], 6);
      CHECK_EQUAL(iv[4], 7);
    }

    TEST(test_ranges_repeat_view)
    {
      // bounded
      auto iv = etl::ranges::repeat_view(1, 7);

      for (auto i : iv)
      {
        CHECK_EQUAL(i, 1);
      }

      CHECK_EQUAL(*iv.begin(), 1);
      CHECK_EQUAL(iv.end(), iv.begin() + 7);
      CHECK_EQUAL(iv.size(), 7);
      CHECK_EQUAL(iv.empty(), false);

      CHECK_EQUAL(iv[4], 1);

      // unbounded
      auto iv2 = etl::ranges::repeat_view(3);
      CHECK_EQUAL(iv2[0], 3);
      CHECK_EQUAL(iv2[1], 3);
      CHECK_EQUAL(iv2[2], 3);
      CHECK_EQUAL(iv2[3], 3);
      CHECK_EQUAL(iv2[4], 3);

      auto iv3 = etl::ranges::repeat_view<int>();
      CHECK_EQUAL(iv3.size(), 0);
      CHECK_EQUAL(iv3.end(), iv3.begin());
      CHECK_EQUAL(iv3.empty(), true);
    }

    TEST(test_ranges_views_repeat)
    {
      auto iv = etl::ranges::views::repeat(1, 7);

      CHECK_EQUAL(*iv.begin(), 1);
      CHECK_EQUAL(iv.end(), iv.begin() + 7);
      CHECK_EQUAL(iv.size(), 7);
      CHECK_EQUAL(iv.empty(), false);

      CHECK_EQUAL(iv[4], 1);
    }

    TEST(test_ranges_repeat_view_pipe_take)
    {
      auto iv = etl::ranges::repeat_view(3) | etl::views::take(5);

      auto i = iv.begin();
      CHECK_EQUAL(*i, 3);
      i++;
      CHECK_EQUAL(*i, 3);
      ++i;
      CHECK_EQUAL(*i, 3);

      CHECK_EQUAL(iv.size(), 5);
      CHECK_EQUAL(iv.front(), 3);
      CHECK_EQUAL(iv.back(), 3);
      CHECK_EQUAL(iv[0], 3);
      CHECK_EQUAL(iv[1], 3);
      CHECK_EQUAL(iv[2], 3);
      CHECK_EQUAL(iv[3], 3);
      CHECK_EQUAL(iv[4], 3);
    }

    TEST(test_ranges_repeat_view_pipe_take_bounded)
    {
      auto iv = etl::ranges::repeat_view(3, 30) | etl::views::take(5);

      auto i = iv.begin();
      CHECK_EQUAL(*i, 3);
      i++;
      CHECK_EQUAL(*i, 3);
      ++i;
      CHECK_EQUAL(*i, 3);

      CHECK_EQUAL(iv.size(), 5);
      CHECK_EQUAL(iv.front(), 3);
      CHECK_EQUAL(iv.back(), 3);
      CHECK_EQUAL(iv[0], 3);
      CHECK_EQUAL(iv[1], 3);
      CHECK_EQUAL(iv[2], 3);
      CHECK_EQUAL(iv[3], 3);
      CHECK_EQUAL(iv[4], 3);
    }

    TEST(test_ranges_repeat_view_pipe_take_bounded_limited)
    {
      auto iv = etl::ranges::repeat_view(3, 4) | etl::views::take(5);

      auto i = iv.begin();
      CHECK_EQUAL(*i, 3);
      i++;
      CHECK_EQUAL(*i, 3);
      ++i;
      CHECK_EQUAL(*i, 3);

      CHECK_EQUAL(iv.size(), 4);
      CHECK_EQUAL(iv.front(), 3);
      CHECK_EQUAL(iv.back(), 3);
      CHECK_EQUAL(iv[0], 3);
      CHECK_EQUAL(iv[1], 3);
      CHECK_EQUAL(iv[2], 3);
      CHECK_EQUAL(iv[3], 3);
    }

    //*************************************************************************
    // Range adaptors
    //*************************************************************************
    TEST(test_ranges_iterate_c_array)
    {
      auto even = [](int i) -> bool
      {
        return 0 == i % 2;
      };

      int                  v_in[10] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
      etl::vector<int, 10> v_out;
      etl::vector<int, 10> v_out_expected{0, 2, 4, 6, 8};

      for (int i : etl::views::filter(v_in, even))
      {
        v_out.push_back(i);
      }

      CHECK_EQUAL(v_out_expected, v_out);
    }

    TEST(test_ranges_iterate_plain)
    {
      auto even = [](int i) -> bool
      {
        return 0 == i % 2;
      };

      etl::vector<int, 10> v_in{0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
      etl::vector<int, 10> v_out;
      etl::vector<int, 10> v_out_expected{0, 2, 4, 6, 8};

      for (int i : etl::views::filter(v_in, even))
      {
        v_out.push_back(i);
      }

      CHECK_EQUAL(v_out_expected, v_out);
    }

    TEST(test_ranges_iterate_pipe)
    {
      auto even = [](int i) -> bool
      {
        return 0 == i % 2;
      };

      etl::vector<int, 10> v_in{0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
      etl::vector<int, 10> v_out;
      etl::vector<int, 10> v_out_expected{0, 2, 4, 6, 8};

      for (int i : v_in | etl::views::filter(even))
      {
        v_out.push_back(i);
      }

      CHECK_EQUAL(v_out_expected, v_out);
    }

    TEST(test_ranges_drop_functional)
    {
      etl::vector<int, 10> v_in{0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
      etl::vector<int, 10> v_out;
      etl::vector<int, 10> v_out_expected{2, 3, 4, 5, 6, 7, 8, 9};

      auto rv = etl::views::drop(v_in, 2);

      CHECK_EQUAL(etl::views::all(v_out_expected), rv);
      CHECK_EQUAL(rv.base().base(), v_in);
      CHECK_EQUAL(*rv.begin(), 2);
      CHECK_EQUAL(rv.size(), 8);

      CHECK_EQUAL(*rv.cbegin(), 2);
      CHECK_EQUAL(rv.empty(), false);
      CHECK_EQUAL(rv, true);
      CHECK_EQUAL(rv.front(), 2);
      CHECK_EQUAL(rv.back(), 9);
      CHECK_EQUAL(rv[7], 9);

      rv[1] = 33;
      CHECK_EQUAL(rv[1], 33);
      CHECK_EQUAL(v_in[3], 33);

      v_in[2] = 44;
      CHECK_EQUAL(rv[0], 44);
      CHECK_EQUAL(v_in[2], 44);
    }

    TEST(test_ranges_iterate_pipe_drop)
    {
      etl::vector<int, 10> v_in{0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
      etl::vector<int, 10> v_out;
      etl::vector<int, 10> v_out_expected{2, 3, 4, 5, 6, 7, 8, 9};

      auto rv = v_in | etl::views::drop(2);
      for (int i : rv)
      {
        v_out.push_back(i);
      }

      CHECK_EQUAL(v_out_expected, v_out);
      CHECK_EQUAL(rv.base().base(), v_in);
      CHECK_EQUAL(*rv.begin(), 2);
      CHECK_EQUAL(rv.size(), 8);

      CHECK_EQUAL(*rv.cbegin(), 2);
      CHECK_EQUAL(rv.empty(), false);
      CHECK_EQUAL(rv, true);
      CHECK_EQUAL(rv.front(), 2);
      CHECK_EQUAL(rv.back(), 9);
      CHECK_EQUAL(rv[7], 9);

      rv[1] = 33;
      CHECK_EQUAL(rv[1], 33);
      CHECK_EQUAL(v_in[3], 33);

      v_in[2] = 44;
      CHECK_EQUAL(rv[0], 44);
      CHECK_EQUAL(v_in[2], 44);
    }

    TEST(test_ranges_iterate_pipe_drop_out_of_bounds)
    {
      etl::vector<int, 10> v_in{0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
      etl::vector<int, 10> v_out;
      etl::vector<int, 10> v_out_expected{};

      auto rv = v_in | etl::views::drop(12);
      for (int i : rv)
      {
        v_out.push_back(i);
      }

      CHECK_EQUAL(v_out_expected, v_out);
      CHECK_EQUAL(rv.size(), 0);
    }

    TEST(test_ranges_iterate_pipe_twice)
    {
      auto even = [](int i) -> bool
      {
        return 0 == i % 2;
      };

      etl::vector<int, 10> v_in{0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
      etl::vector<int, 10> v_out;
      etl::vector<int, 10> v_out_expected{2, 4, 6, 8};

      for (int i : v_in | etl::views::filter(even) | etl::views::drop(1))
      {
        v_out.push_back(i);
      }

      CHECK_EQUAL(v_out_expected, v_out);
    }

    TEST(test_ranges_iterate_pipe_drop_while)
    {
      auto below_three = [](int i) -> bool
      {
        return i < 3;
      };

      etl::vector<int, 10> v_in{0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
      etl::vector<int, 10> v_out;
      etl::vector<int, 10> v_out_expected{3, 4, 5, 6, 7, 8, 9};

      auto rv = v_in | etl::views::drop_while(below_three);
      for (int i : rv)
      {
        v_out.push_back(i);
      }

      CHECK_EQUAL(v_out_expected, v_out);
      CHECK_EQUAL(rv.base().base(), v_in);
      CHECK_EQUAL(*rv.begin(), 3);
      CHECK_EQUAL(rv.size(), 7);

      CHECK_EQUAL(*rv.cbegin(), 3);
      CHECK_EQUAL(rv.empty(), false);
      CHECK_EQUAL(rv, true);
      CHECK_EQUAL(rv.front(), 3);
      CHECK_EQUAL(rv.back(), 9);
      CHECK_EQUAL(rv[6], 9);
    }

    TEST(test_ranges_take_functional)
    {
      etl::vector<int, 10> v_in{0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
      etl::vector<int, 10> v_out_expected{0, 1, 2, 3};

      auto rv = etl::views::take(v_in, 4);

      CHECK_EQUAL(etl::views::all(v_out_expected), rv);
      CHECK_EQUAL(rv.base().base(), v_in);
      CHECK_EQUAL(*rv.begin(), 0);
      CHECK_EQUAL(rv.size(), 4);

      CHECK_EQUAL(*rv.cbegin(), 0);
      CHECK_EQUAL(rv.empty(), false);
      CHECK_EQUAL(rv, true);
      CHECK_EQUAL(rv.front(), 0);
      CHECK_EQUAL(rv.back(), 3);
      CHECK_EQUAL(rv[2], 2);

      rv[2] = 33;
      CHECK_EQUAL(rv[2], 33);
      CHECK_EQUAL(v_in[2], 33);

      v_in[3] = 44;
      CHECK_EQUAL(rv[3], 44);
      CHECK_EQUAL(v_in[3], 44);
    }

    TEST(test_ranges_iterate_pipe_take)
    {
      etl::vector<int, 10> v_in{0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
      etl::vector<int, 10> v_out;
      etl::vector<int, 10> v_out_expected{0, 1, 2, 3};

      auto rv = v_in | etl::views::take(4);
      for (int i : rv)
      {
        v_out.push_back(i);
      }

      CHECK_EQUAL(v_out_expected, v_out);
      CHECK_EQUAL(rv.base().base(), v_in);
      CHECK_EQUAL(*rv.begin(), 0);
      CHECK_EQUAL(rv.size(), 4);

      CHECK_EQUAL(*rv.cbegin(), 0);
      CHECK_EQUAL(rv.empty(), false);
      CHECK_EQUAL(rv, true);
      CHECK_EQUAL(rv.front(), 0);
      CHECK_EQUAL(rv.back(), 3);
      CHECK_EQUAL(rv[2], 2);

      rv[2] = 33;
      CHECK_EQUAL(rv[2], 33);
      CHECK_EQUAL(v_in[2], 33);

      v_in[3] = 44;
      CHECK_EQUAL(rv[3], 44);
      CHECK_EQUAL(v_in[3], 44);
    }

    TEST(test_ranges_iterate_pipe_take_while)
    {
      auto below_three = [](int i) -> bool
      {
        return i < 3;
      };

      etl::vector<int, 10> v_in{0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
      etl::vector<int, 10> v_out;
      etl::vector<int, 10> v_out_expected{0, 1, 2};

      auto rv = v_in | etl::views::take_while(below_three);
      for (int i : rv)
      {
        v_out.push_back(i);
      }

      CHECK_EQUAL(v_out_expected, v_out);
      CHECK_EQUAL(rv.base().base(), v_in);
      CHECK_EQUAL(*rv.begin(), 0);
      CHECK_EQUAL(rv.size(), 3);

      CHECK_EQUAL(*rv.cbegin(), 0);
      CHECK_EQUAL(rv.empty(), false);
      CHECK_EQUAL(rv, true);
      CHECK_EQUAL(rv.front(), 0);
      CHECK_EQUAL(rv.back(), 2);
      CHECK_EQUAL(rv[2], 2);
    }

    TEST(test_ranges_reverse_view_functional)
    {
      etl::vector<int, 10> v_in{0, 1, 2, 3, 4};
      etl::vector<int, 10> v_out;
      etl::vector<int, 10> v_out_expected{4, 3, 2, 1, 0};

      auto rv = etl::ranges::reverse_view(v_in);
      for (int i : rv)
      {
        v_out.push_back(i);
      }

      CHECK_EQUAL(v_out_expected, v_out);
      CHECK_EQUAL(rv.base().base(), v_in);
      CHECK_EQUAL(*rv.begin(), 4);
      CHECK_EQUAL(rv.size(), 5);

      CHECK_EQUAL(*rv.cbegin(), 4);
      CHECK_EQUAL(rv.empty(), false);
      CHECK_EQUAL(rv, true);
      CHECK_EQUAL(rv.front(), 4);
      CHECK_EQUAL(rv.back(), 0);
      CHECK_EQUAL(rv[2], 2);

      rv[0] = 22;
      CHECK_EQUAL(rv[0], 22);
      CHECK_EQUAL(v_in[4], 22);

      v_in[0] = 11;
      CHECK_EQUAL(rv.back(), 11);
      CHECK_EQUAL(rv[4], 11);
    }

    TEST(test_ranges_iterate_pipe_reverse)
    {
      etl::vector<int, 10> v_in{0, 1, 2, 3, 4};
      etl::vector<int, 10> v_out;
      etl::vector<int, 10> v_out_expected{4, 3, 2, 1, 0};

      auto rv = v_in | etl::views::reverse;
      for (int i : rv)
      {
        v_out.push_back(i);
      }

      CHECK_EQUAL(v_out_expected, v_out);
      CHECK_EQUAL(rv.base().base(), v_in);
      CHECK_EQUAL(*rv.begin(), 4);
      CHECK_EQUAL(rv.size(), 5);
      CHECK_EQUAL(rv.front(), 4);
      CHECK_EQUAL(rv.back(), 0);
    }

    TEST(test_ranges_iterate_pipe_reverse_reverse)
    {
      etl::vector<int, 10> v_in{0, 1, 2, 3, 4};
      etl::vector<int, 10> v_out;
      etl::vector<int, 10> v_out_expected{0, 1, 2, 3, 4};

      auto rv = v_in | etl::views::reverse | etl::views::reverse;
      for (int i : rv)
      {
        v_out.push_back(i);
      }

      CHECK_EQUAL(v_out_expected, v_out);
      CHECK_EQUAL(rv.base(), v_in);
      CHECK_EQUAL(*rv.begin(), 0);
      CHECK_EQUAL(rv.size(), 5);
      CHECK_EQUAL(rv.front(), 0);
      CHECK_EQUAL(rv.back(), 4);
    }

    TEST(test_ranges_iterate_pipe_all)
    {
      etl::vector<int, 10> v_in{0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
      etl::vector<int, 10> v_out;
      etl::vector<int, 10> v_out_expected{0, 1, 2, 3, 4, 5, 6, 7, 8, 9};

      for (int i : v_in | etl::views::all)
      {
        v_out.push_back(i);
      }

      CHECK_EQUAL(v_out_expected, v_out);
    }

    TEST(test_ranges_iterate_pipe_ref)
    {
      etl::vector<int, 10> v_in{0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
      etl::vector<int, 10> v_out;
      etl::vector<int, 10> v_out_expected{0, 1, 2, 3, 4, 5, 6, 7, 8, 9};

      auto r = v_in | etl::views::ref();
      for (int i : r)
      {
        v_out.push_back(i);
      }

      CHECK_EQUAL(v_out_expected, v_out);

      v_in[9] = 99;
      CHECK_EQUAL(r[9], 99);

      CHECK_EQUAL(r.base(), v_in);
      CHECK_EQUAL(*r.begin(), 0);
      CHECK_EQUAL(r.end(), r.begin() + 10);
      CHECK_EQUAL(r.empty(), false);
      CHECK_EQUAL(r.size(), 10);
      CHECK_EQUAL(r.data(), v_in.data());
    }

    TEST(test_ranges_iterate_pipe_owning)
    {
      etl::vector<int, 10> v_in{0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
      etl::vector<int, 10> v_out;
      etl::vector<int, 10> v_out_expected{0, 1, 2, 3, 4, 5, 6, 7, 8, 9};

      auto r = v_in | etl::views::owning();
      for (int i : r)
      {
        v_out.push_back(i);
      }

      CHECK_EQUAL(v_out_expected, v_out);

      CHECK_EQUAL(0, v_in.size());

      etl::ranges::owning_view<etl::vector<int, 10>> ov2;

      CHECK_NOT_EQUAL(r.base(), v_in);
      CHECK_EQUAL(*r.begin(), 0);
      CHECK_EQUAL(r.end(), r.begin() + 10);
      CHECK_EQUAL(r.empty(), false);
      CHECK_EQUAL(r.size(), 10);
      CHECK_NOT_EQUAL(r.data(), v_in.data());

      // ov2 = r; // expected: compile error!
      ov2 = etl::move(r);
    }

    TEST(test_ranges_iterate_pipe_to)
    {
      auto even = [](int i) -> bool
      {
        return 0 == i % 2;
      };

      etl::vector<int, 10> v_in{0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
      etl::vector<int, 10> v_out_expected{2, 4, 6, 8};

      auto v_out = v_in | etl::views::filter(even) | etl::views::drop(1) | etl::ranges::to<etl::vector<int, 10>>();

      CHECK_EQUAL(v_out_expected, v_out);
    }

    TEST(test_ranges_iterate_pipe_as_rvalue)
    {
      etl::vector<MoveInt, 10> v_in{0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
      etl::vector<MoveInt, 10> v_out;

      etl::vector<MoveInt, 10> v_out_expected_0{0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
      etl::vector<MoveInt, 10> v_out_expected_1{0, 1, 2, 3, 4, 5, 6, 7, 8, 9};

      for (auto&& i : v_in | etl::views::as_rvalue)
      {
        v_out.emplace_back(etl::move(i));
      }

      CHECK_EQUAL(v_out_expected_0, v_in);
      CHECK_EQUAL(v_out_expected_1, v_out);
    }

    TEST(test_ranges_iterate_pipe_as_rvalue_functional)
    {
      etl::vector<MoveInt, 10> v_in{0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
      etl::vector<MoveInt, 10> v_out;

      etl::vector<MoveInt, 10> v_out_expected_0{0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
      etl::vector<MoveInt, 10> v_out_expected_1{0, 1, 2, 3, 4, 5, 6, 7, 8, 9};

      v_out = etl::views::as_rvalue(v_in) | etl::ranges::to<etl::vector<MoveInt, 10>>();

      CHECK_EQUAL(v_out_expected_0, v_in);
      CHECK_EQUAL(v_out_expected_1, v_out);
    }

    TEST(test_ranges_as_const_view_functional)
    {
      etl::vector<int, 10> v_in{0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
      etl::vector<int, 10> v_out;
      etl::vector<int, 10> v_out_expected{0, 1, 2, 3, 4, 5, 6, 7, 8, 9};

      auto cv = etl::ranges::as_const_view(v_in);

      // Iterator type should be const_iterator
      using cv_reference = decltype(*cv.begin());
      static_assert(etl::is_const_v<etl::remove_reference_t<cv_reference>>, "as_const_view iterator should dereference to const");

      for (auto i : cv)
      {
        v_out.push_back(i);
      }

      CHECK_EQUAL(v_out_expected, v_out);
      CHECK_EQUAL(cv.size(), 10u);
      CHECK_EQUAL(cv.empty(), false);
      CHECK_EQUAL(cv.front(), 0);
      CHECK_EQUAL(cv.back(), 9);
    }

    TEST(test_ranges_as_const_view_reflects_base_changes)
    {
      etl::vector<int, 10> v_in{0, 1, 2, 3, 4};

      auto cv = etl::ranges::as_const_view(v_in);

      CHECK_EQUAL(cv.front(), 0);
      CHECK_EQUAL(cv.back(), 4);
      CHECK_EQUAL(cv.size(), 5u);

      v_in[0] = 99;
      v_in[4] = 77;
      CHECK_EQUAL(cv.front(), 99);
      CHECK_EQUAL(cv.back(), 77);
    }

    TEST(test_ranges_as_const_view_pipe)
    {
      etl::vector<int, 10> v_in{0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
      etl::vector<int, 10> v_out;
      etl::vector<int, 10> v_out_expected{0, 1, 2, 3, 4, 5, 6, 7, 8, 9};

      auto cv = v_in | etl::views::as_const();

      using cv_reference = decltype(*cv.begin());
      static_assert(etl::is_const_v<etl::remove_reference_t<cv_reference>>, "piped as_const view iterator should dereference to const");

      for (auto i : cv)
      {
        v_out.push_back(i);
      }

      CHECK_EQUAL(v_out_expected, v_out);
      CHECK_EQUAL(cv.size(), 10u);
    }

    TEST(test_ranges_views_as_const_functional)
    {
      etl::vector<int, 10> v_in{0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
      etl::vector<int, 10> v_out;
      etl::vector<int, 10> v_out_expected{0, 1, 2, 3, 4, 5, 6, 7, 8, 9};

      auto cv = etl::views::as_const(v_in);

      using cv_reference = decltype(*cv.begin());
      static_assert(etl::is_const_v<etl::remove_reference_t<cv_reference>>, "views::as_const iterator should dereference to const");

      for (auto i : cv)
      {
        v_out.push_back(i);
      }

      CHECK_EQUAL(v_out_expected, v_out);
    }

    TEST(test_ranges_as_const_view_pipe_chained_with_take)
    {
      etl::vector<int, 10> v_in{0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
      etl::vector<int, 10> v_out;
      etl::vector<int, 10> v_out_expected{0, 1, 2};

      auto cv = v_in | etl::views::as_const() | etl::views::take(3);

      for (auto i : cv)
      {
        v_out.push_back(i);
      }

      CHECK_EQUAL(v_out_expected, v_out);
    }

    TEST(test_ranges_as_const_view_pipe_chained_with_drop)
    {
      etl::vector<int, 10> v_in{0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
      etl::vector<int, 10> v_out;
      etl::vector<int, 10> v_out_expected{7, 8, 9};

      auto cv = v_in | etl::views::as_const() | etl::views::drop(7);

      for (auto i : cv)
      {
        v_out.push_back(i);
      }

      CHECK_EQUAL(v_out_expected, v_out);
    }

    TEST(test_ranges_as_const_view_pipe_chained_with_reverse)
    {
      etl::vector<int, 10> v_in{0, 1, 2, 3, 4};
      etl::vector<int, 10> v_out;
      etl::vector<int, 10> v_out_expected{4, 3, 2, 1, 0};

      auto cv = v_in | etl::views::as_const() | etl::views::reverse;

      for (auto i : cv)
      {
        v_out.push_back(i);
      }

      CHECK_EQUAL(v_out_expected, v_out);
    }

    TEST(test_ranges_as_const_view_with_std_vector)
    {
      std::vector<int> v_in{0, 1, 2, 3, 4};
      std::vector<int> v_out;
      std::vector<int> v_out_expected{0, 1, 2, 3, 4};

      auto cv = etl::ranges::as_const_view(v_in);

      using cv_reference = decltype(*cv.begin());
      static_assert(etl::is_const_v<etl::remove_reference_t<cv_reference>>, "as_const_view over std::vector should dereference to const");

      for (auto i : cv)
      {
        v_out.push_back(i);
      }

      CHECK_EQUAL(v_out_expected.size(), v_out.size());
      for (size_t idx = 0; idx < v_out_expected.size(); ++idx)
      {
        CHECK_EQUAL(v_out_expected[idx], v_out[idx]);
      }
    }

    //*************************************************************************
    /// cache_latest_view tests
    //*************************************************************************
    TEST(test_ranges_cache_latest_view_functional)
    {
      etl::vector<int, 10> v_in{0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
      etl::vector<int, 10> v_out;
      etl::vector<int, 10> v_out_expected{0, 1, 2, 3, 4, 5, 6, 7, 8, 9};

      auto cv = etl::ranges::cache_latest_view(v_in);

      for (auto i : cv)
      {
        v_out.push_back(i);
      }

      CHECK_EQUAL(v_out_expected, v_out);
      CHECK_EQUAL(cv.size(), 10u);
      CHECK_EQUAL(cv.empty(), false);
    }

    TEST(test_ranges_cache_latest_view_caches_value)
    {
      int  dereference_count  = 0;
      auto counting_transform = [&dereference_count](int i) -> int
      {
        ++dereference_count;
        return i * 2;
      };

      etl::vector<int, 10> v_in{1, 2, 3};

      auto tv = v_in | etl::views::transform(counting_transform) | etl::views::cache_latest();

      auto it = tv.begin();

      // First dereference should evaluate
      dereference_count = 0;
      int val1          = *it;
      CHECK_EQUAL(1, dereference_count);
      CHECK_EQUAL(2, val1);

      // Second dereference of same position should use cache (no extra
      // transform call)
      int val2 = *it;
      CHECK_EQUAL(1, dereference_count);
      CHECK_EQUAL(2, val2);

      // Advance and dereference
      ++it;
      int val3 = *it;
      CHECK_EQUAL(2, dereference_count);
      CHECK_EQUAL(4, val3);
    }

    TEST(test_ranges_cache_latest_view_pipe)
    {
      etl::vector<int, 10> v_in{0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
      etl::vector<int, 10> v_out;
      etl::vector<int, 10> v_out_expected{0, 1, 2, 3, 4, 5, 6, 7, 8, 9};

      auto cv = v_in | etl::views::cache_latest();

      for (auto i : cv)
      {
        v_out.push_back(i);
      }

      CHECK_EQUAL(v_out_expected, v_out);
    }

    TEST(test_ranges_views_cache_latest_functional)
    {
      etl::vector<int, 10> v_in{0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
      etl::vector<int, 10> v_out;
      etl::vector<int, 10> v_out_expected{0, 1, 2, 3, 4, 5, 6, 7, 8, 9};

      auto cv = etl::views::cache_latest(v_in);

      for (auto i : cv)
      {
        v_out.push_back(i);
      }

      CHECK_EQUAL(v_out_expected, v_out);
    }

    TEST(test_ranges_cache_latest_view_pipe_chained_with_take)
    {
      etl::vector<int, 10> v_in{0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
      etl::vector<int, 10> v_out;
      etl::vector<int, 10> v_out_expected{0, 1, 2};

      auto cv = v_in | etl::views::cache_latest() | etl::views::take(3);

      for (auto i : cv)
      {
        v_out.push_back(i);
      }

      CHECK_EQUAL(v_out_expected, v_out);
    }

    TEST(test_ranges_cache_latest_view_pipe_chained_with_transform)
    {
      auto square = [](int i) -> int
      {
        return i * i;
      };

      etl::vector<int, 10> v_in{0, 1, 2, 3, 4};
      etl::vector<int, 10> v_out;
      etl::vector<int, 10> v_out_expected{0, 1, 4, 9, 16};

      auto cv = v_in | etl::views::transform(square) | etl::views::cache_latest();

      for (auto i : cv)
      {
        v_out.push_back(i);
      }

      CHECK_EQUAL(v_out_expected, v_out);
    }

    TEST(test_ranges_cache_latest_view_empty_range)
    {
      etl::vector<int, 10> v_in;

      auto cv = etl::ranges::cache_latest_view(v_in);

      CHECK_EQUAL(cv.empty(), true);
      CHECK_EQUAL(cv.size(), 0u);
      CHECK(cv.begin() == cv.end());
    }

    TEST(test_ranges_cache_latest_view_with_std_vector)
    {
      std::vector<int> v_in{0, 1, 2, 3, 4};
      std::vector<int> v_out;
      std::vector<int> v_out_expected{0, 1, 2, 3, 4};

      auto cv = etl::ranges::cache_latest_view(v_in);

      for (auto i : cv)
      {
        v_out.push_back(i);
      }

      CHECK_EQUAL(v_out_expected.size(), v_out.size());
      for (size_t idx = 0; idx < v_out_expected.size(); ++idx)
      {
        CHECK_EQUAL(v_out_expected[idx], v_out[idx]);
      }
    }

    TEST(test_ranges_iterate_transform)
    {
      auto square = [](int i) -> int
      {
        return i * i;
      };

      etl::vector<int, 10> v_in{0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
      etl::vector<int, 10> v_out;
      etl::vector<int, 10> v_out_expected{0, 1, 4, 9, 16, 25, 36, 49, 64, 81};

      for (int i : v_in | etl::views::transform(square))
      {
        v_out.push_back(i);
      }

      CHECK_EQUAL(v_out_expected, v_out);
    }

    //*************************************************************************
    /// transform_view tests
    //*************************************************************************
    TEST(test_ranges_transform_view_functional)
    {
      auto square = [](int i) -> int
      {
        return i * i;
      };

      etl::vector<int, 10> v_in{0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
      etl::vector<int, 10> v_out;
      etl::vector<int, 10> v_out_expected{0, 1, 4, 9, 16, 25, 36, 49, 64, 81};

      auto tv = etl::views::transform(v_in, square);

      for (auto i : tv)
      {
        v_out.push_back(i);
      }

      CHECK_EQUAL(v_out_expected, v_out);
    }

    TEST(test_ranges_transform_view_direct_construction)
    {
      auto negate = [](int i) -> int
      {
        return -i;
      };

      etl::vector<int, 5> v_in{1, 2, 3, 4, 5};
      etl::vector<int, 5> v_out;
      etl::vector<int, 5> v_out_expected{-1, -2, -3, -4, -5};

      auto tv = etl::ranges::transform_view(etl::views::all(v_in), negate);

      for (auto i : tv)
      {
        v_out.push_back(i);
      }

      CHECK_EQUAL(v_out_expected, v_out);
    }

    TEST(test_ranges_transform_view_pipe)
    {
      auto triple = [](int i) -> int
      {
        return i * 3;
      };

      etl::vector<int, 5> v_in{1, 2, 3, 4, 5};
      etl::vector<int, 5> v_out;
      etl::vector<int, 5> v_out_expected{3, 6, 9, 12, 15};

      auto tv = v_in | etl::views::transform(triple);

      for (auto i : tv)
      {
        v_out.push_back(i);
      }

      CHECK_EQUAL(v_out_expected, v_out);
    }

    TEST(test_ranges_transform_view_size)
    {
      auto identity = [](int i) -> int
      {
        return i;
      };

      etl::vector<int, 10> v_in{0, 1, 2, 3, 4, 5, 6, 7, 8, 9};

      auto tv = v_in | etl::views::transform(identity);

      CHECK_EQUAL(10u, tv.size());
    }

    TEST(test_ranges_transform_view_empty)
    {
      auto identity = [](int i) -> int
      {
        return i;
      };

      etl::vector<int, 10> v_in;

      auto tv = v_in | etl::views::transform(identity);

      CHECK_EQUAL(true, tv.empty());
      CHECK_EQUAL(0u, tv.size());
    }

    TEST(test_ranges_transform_view_non_empty)
    {
      auto identity = [](int i) -> int
      {
        return i;
      };

      etl::vector<int, 10> v_in{42};

      auto tv = v_in | etl::views::transform(identity);

      CHECK_EQUAL(false, tv.empty());
      CHECK_EQUAL(1u, tv.size());
    }

    TEST(test_ranges_transform_view_front)
    {
      auto square = [](int i) -> int
      {
        return i * i;
      };

      etl::vector<int, 5> v_in{3, 4, 5};

      auto tv = v_in | etl::views::transform(square);

      CHECK_EQUAL(9, tv.front());
    }

    TEST(test_ranges_transform_view_index_operator)
    {
      auto square = [](int i) -> int
      {
        return i * i;
      };

      etl::vector<int, 5> v_in{1, 2, 3, 4, 5};

      auto tv = v_in | etl::views::transform(square);

      CHECK_EQUAL(1, tv[0]);
      CHECK_EQUAL(4, tv[1]);
      CHECK_EQUAL(9, tv[2]);
      CHECK_EQUAL(16, tv[3]);
      CHECK_EQUAL(25, tv[4]);
    }

    TEST(test_ranges_transform_view_type_change)
    {
      auto to_double = [](int i) -> double
      {
        return i * 1.5;
      };

      etl::vector<int, 5> v_in{2, 4, 6};

      auto tv = v_in | etl::views::transform(to_double);

      auto it = tv.begin();
      CHECK_CLOSE(3.0, *it, 0.001);
      ++it;
      CHECK_CLOSE(6.0, *it, 0.001);
      ++it;
      CHECK_CLOSE(9.0, *it, 0.001);
      ++it;
      CHECK(it == tv.end());
    }

    TEST(test_ranges_transform_view_iterator_increment)
    {
      auto square = [](int i) -> int
      {
        return i * i;
      };

      etl::vector<int, 5> v_in{1, 2, 3};

      auto tv = v_in | etl::views::transform(square);

      // Pre-increment
      auto it = tv.begin();
      CHECK_EQUAL(1, *it);
      ++it;
      CHECK_EQUAL(4, *it);

      // Post-increment
      auto it2 = tv.begin();
      auto old = it2++;
      CHECK_EQUAL(1, *old);
      CHECK_EQUAL(4, *it2);
    }

    TEST(test_ranges_transform_view_iterator_equality)
    {
      auto identity = [](int i) -> int
      {
        return i;
      };

      etl::vector<int, 5> v_in{1, 2, 3};

      auto tv = v_in | etl::views::transform(identity);

      auto b = tv.begin();
      auto e = tv.end();
      CHECK(b != e);
      CHECK(b == tv.begin());
      CHECK(e == tv.end());
    }

    TEST(test_ranges_transform_view_chained_with_take)
    {
      auto square = [](int i) -> int
      {
        return i * i;
      };

      etl::vector<int, 10> v_in{0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
      etl::vector<int, 10> v_out;
      etl::vector<int, 10> v_out_expected{0, 1, 4};

      auto tv = v_in | etl::views::transform(square) | etl::views::take(3);

      for (auto i : tv)
      {
        v_out.push_back(i);
      }

      CHECK_EQUAL(v_out_expected, v_out);
    }

    TEST(test_ranges_transform_view_chained_with_temporary)
    {
      etl::vector<int, 10> v_in{0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
      etl::vector<int, 10> v_out;
      etl::vector<int, 10> v_out_expected{0, 1, 4};

      auto tv = v_in | etl::views::transform([](int i) -> int { return i * i; }) | etl::views::take(3);

      for (auto i : tv)
      {
        v_out.push_back(i);
      }

      CHECK_EQUAL(v_out_expected, v_out);
    }

    TEST(test_ranges_transform_view_chained_with_drop)
    {
      auto square = [](int i) -> int
      {
        return i * i;
      };

      etl::vector<int, 10> v_in{0, 1, 2, 3, 4};
      etl::vector<int, 10> v_out;
      etl::vector<int, 10> v_out_expected{9, 16};

      auto tv = v_in | etl::views::transform(square) | etl::views::drop(3);

      for (auto i : tv)
      {
        v_out.push_back(i);
      }

      CHECK_EQUAL(v_out_expected, v_out);
    }

    TEST(test_ranges_transform_view_chained_transforms)
    {
      auto add_one = [](int i) -> int
      {
        return i + 1;
      };
      auto multiply = [](int i) -> int
      {
        return i * 10;
      };

      etl::vector<int, 5> v_in{0, 1, 2, 3, 4};
      etl::vector<int, 5> v_out;
      etl::vector<int, 5> v_out_expected{10, 20, 30, 40, 50};

      auto tv = v_in | etl::views::transform(add_one) | etl::views::transform(multiply);

      for (auto i : tv)
      {
        v_out.push_back(i);
      }

      CHECK_EQUAL(v_out_expected, v_out);
    }

    TEST(test_ranges_transform_view_with_std_vector)
    {
      auto square = [](int i) -> int
      {
        return i * i;
      };

      std::vector<int> v_in{1, 2, 3, 4, 5};
      std::vector<int> v_out;
      std::vector<int> v_out_expected{1, 4, 9, 16, 25};

      auto tv = v_in | etl::views::transform(square);

      for (auto i : tv)
      {
        v_out.push_back(i);
      }

      CHECK_EQUAL(v_out_expected.size(), v_out.size());
      for (size_t idx = 0; idx < v_out_expected.size(); ++idx)
      {
        CHECK_EQUAL(v_out_expected[idx], v_out[idx]);
      }
    }

    TEST(test_ranges_transform_view_with_std_array)
    {
      auto square = [](int i) -> int
      {
        return i * i;
      };

      std::array<int, 5> v_in{1, 2, 3, 4, 5};
      std::vector<int>   v_out;
      std::vector<int>   v_out_expected{1, 4, 9, 16, 25};

      auto tv = v_in | etl::views::transform(square);

      for (auto i : tv)
      {
        v_out.push_back(i);
      }

      CHECK_EQUAL(v_out_expected.size(), v_out.size());
      for (size_t idx = 0; idx < v_out_expected.size(); ++idx)
      {
        CHECK_EQUAL(v_out_expected[idx], v_out[idx]);
      }
    }

    TEST(test_ranges_transform_view_single_element)
    {
      auto negate = [](int i) -> int
      {
        return -i;
      };

      etl::vector<int, 5> v_in{42};
      etl::vector<int, 5> v_out;
      etl::vector<int, 5> v_out_expected{-42};

      auto tv = v_in | etl::views::transform(negate);

      for (auto i : tv)
      {
        v_out.push_back(i);
      }

      CHECK_EQUAL(v_out_expected, v_out);
    }

    //*************************************************************************
    /// filter_view tests
    //*************************************************************************
    TEST(test_ranges_filter_view_functional)
    {
      auto even = [](int i) -> bool
      {
        return 0 == i % 2;
      };

      etl::vector<int, 10> v_in{0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
      etl::vector<int, 10> v_out;
      etl::vector<int, 10> v_out_expected{0, 2, 4, 6, 8};

      auto fv = etl::views::filter(v_in, even);

      for (auto i : fv)
      {
        v_out.push_back(i);
      }

      CHECK_EQUAL(v_out_expected, v_out);
    }

    TEST(test_ranges_filter_view_direct_construction)
    {
      auto odd = [](int i) -> bool
      {
        return 1 == i % 2;
      };

      etl::vector<int, 10> v_in{0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
      etl::vector<int, 10> v_out;
      etl::vector<int, 10> v_out_expected{1, 3, 5, 7, 9};

      auto fv = etl::ranges::filter_view(etl::views::all(v_in), odd);

      for (auto i : fv)
      {
        v_out.push_back(i);
      }

      CHECK_EQUAL(v_out_expected, v_out);
    }

    TEST(test_ranges_filter_view_pipe)
    {
      auto even = [](int i) -> bool
      {
        return 0 == i % 2;
      };

      etl::vector<int, 10> v_in{0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
      etl::vector<int, 10> v_out;
      etl::vector<int, 10> v_out_expected{0, 2, 4, 6, 8};

      auto fv = v_in | etl::views::filter(even);

      for (auto i : fv)
      {
        v_out.push_back(i);
      }

      CHECK_EQUAL(v_out_expected, v_out);
    }

    TEST(test_ranges_filter_view_size)
    {
      auto even = [](int i) -> bool
      {
        return 0 == i % 2;
      };

      etl::vector<int, 10> v_in{0, 1, 2, 3, 4, 5, 6, 7, 8, 9};

      auto fv = v_in | etl::views::filter(even);

      CHECK_EQUAL(5u, fv.size());
    }

    TEST(test_ranges_filter_view_empty)
    {
      auto even = [](int i) -> bool
      {
        return 0 == i % 2;
      };

      etl::vector<int, 10> v_in;

      auto fv = v_in | etl::views::filter(even);

      CHECK_EQUAL(true, fv.empty());
      CHECK_EQUAL(0u, fv.size());
    }

    TEST(test_ranges_filter_view_non_empty)
    {
      auto even = [](int i) -> bool
      {
        return 0 == i % 2;
      };

      etl::vector<int, 10> v_in{42};

      auto fv = v_in | etl::views::filter(even);

      CHECK_EQUAL(false, fv.empty());
      CHECK_EQUAL(1u, fv.size());
    }

    TEST(test_ranges_filter_view_all_filtered_out)
    {
      auto even = [](int i) -> bool
      {
        return 0 == i % 2;
      };

      etl::vector<int, 10> v_in{1, 3, 5, 7, 9};

      auto fv = v_in | etl::views::filter(even);

      CHECK_EQUAL(true, fv.empty());
      CHECK_EQUAL(0u, fv.size());
    }

    TEST(test_ranges_filter_view_all_pass)
    {
      auto even = [](int i) -> bool
      {
        return 0 == i % 2;
      };

      etl::vector<int, 10> v_in{0, 2, 4, 6, 8};
      etl::vector<int, 10> v_out;
      etl::vector<int, 10> v_out_expected{0, 2, 4, 6, 8};

      auto fv = v_in | etl::views::filter(even);

      for (auto i : fv)
      {
        v_out.push_back(i);
      }

      CHECK_EQUAL(v_out_expected, v_out);
      CHECK_EQUAL(5u, fv.size());
    }

    TEST(test_ranges_filter_view_front)
    {
      auto even = [](int i) -> bool
      {
        return 0 == i % 2;
      };

      etl::vector<int, 10> v_in{1, 2, 3, 4, 5};

      auto fv = v_in | etl::views::filter(even);

      CHECK_EQUAL(2, fv.front());
    }

    TEST(test_ranges_filter_view_index_operator)
    {
      auto even = [](int i) -> bool
      {
        return 0 == i % 2;
      };

      etl::vector<int, 10> v_in{0, 1, 2, 3, 4, 5, 6, 7, 8, 9};

      auto fv = v_in | etl::views::filter(even);

      CHECK_EQUAL(0, fv[0]);
      CHECK_EQUAL(2, fv[1]);
      CHECK_EQUAL(4, fv[2]);
      CHECK_EQUAL(6, fv[3]);
      CHECK_EQUAL(8, fv[4]);
    }

    TEST(test_ranges_filter_view_iterator_increment)
    {
      auto even = [](int i) -> bool
      {
        return 0 == i % 2;
      };

      etl::vector<int, 10> v_in{0, 1, 2, 3, 4};

      auto fv = v_in | etl::views::filter(even);

      // Pre-increment
      auto it = fv.begin();
      CHECK_EQUAL(0, *it);
      ++it;
      CHECK_EQUAL(2, *it);

      // Post-increment
      auto it2 = fv.begin();
      auto old = it2++;
      CHECK_EQUAL(0, *old);
      CHECK_EQUAL(2, *it2);
    }

    TEST(test_ranges_filter_view_iterator_equality)
    {
      auto even = [](int i) -> bool
      {
        return 0 == i % 2;
      };

      etl::vector<int, 10> v_in{0, 1, 2, 3, 4};

      auto fv = v_in | etl::views::filter(even);

      auto b = fv.begin();
      auto e = fv.end();
      CHECK(b != e);
      CHECK(b == fv.begin());
      CHECK(e == fv.end());
    }

    TEST(test_ranges_filter_view_chained_with_take)
    {
      auto even = [](int i) -> bool
      {
        return 0 == i % 2;
      };

      etl::vector<int, 10> v_in{0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
      etl::vector<int, 10> v_out;
      etl::vector<int, 10> v_out_expected{0, 2};

      auto fv = v_in | etl::views::filter(even) | etl::views::take(2);

      for (auto i : fv)
      {
        v_out.push_back(i);
      }

      CHECK_EQUAL(v_out_expected, v_out);
    }

    TEST(test_ranges_filter_view_chained_with_temporary)
    {
      etl::vector<int, 10> v_in{0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
      etl::vector<int, 10> v_out;
      etl::vector<int, 10> v_out_expected{0, 2};

      auto fv = v_in | etl::views::filter([](int i) -> bool { return 0 == i % 2; }) | etl::views::take(2);

      for (auto i : fv)
      {
        v_out.push_back(i);
      }

      CHECK_EQUAL(v_out_expected, v_out);
    }

    TEST(test_ranges_filter_view_chained_with_drop)
    {
      auto even = [](int i) -> bool
      {
        return 0 == i % 2;
      };

      etl::vector<int, 10> v_in{0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
      etl::vector<int, 10> v_out;
      etl::vector<int, 10> v_out_expected{4, 6, 8};

      auto fv = v_in | etl::views::filter(even) | etl::views::drop(2);

      for (auto i : fv)
      {
        v_out.push_back(i);
      }

      CHECK_EQUAL(v_out_expected, v_out);
    }

    TEST(test_ranges_filter_view_chained_with_transform)
    {
      auto even = [](int i) -> bool
      {
        return 0 == i % 2;
      };
      auto square = [](int i) -> int
      {
        return i * i;
      };

      etl::vector<int, 10> v_in{0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
      etl::vector<int, 10> v_out;
      etl::vector<int, 10> v_out_expected{0, 4, 16, 36, 64};

      auto fv = v_in | etl::views::filter(even) | etl::views::transform(square);

      for (auto i : fv)
      {
        v_out.push_back(i);
      }

      CHECK_EQUAL(v_out_expected, v_out);
    }

    TEST(test_ranges_filter_view_with_std_vector)
    {
      auto even = [](int i) -> bool
      {
        return 0 == i % 2;
      };

      std::vector<int> v_in{0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
      std::vector<int> v_out;
      std::vector<int> v_out_expected{0, 2, 4, 6, 8};

      auto fv = v_in | etl::views::filter(even);

      for (auto i : fv)
      {
        v_out.push_back(i);
      }

      CHECK_EQUAL(v_out_expected.size(), v_out.size());
      for (size_t idx = 0; idx < v_out_expected.size(); ++idx)
      {
        CHECK_EQUAL(v_out_expected[idx], v_out[idx]);
      }
    }

    TEST(test_ranges_filter_view_with_std_array)
    {
      auto even = [](int i) -> bool
      {
        return 0 == i % 2;
      };

      std::array<int, 10> v_in{0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
      std::vector<int>    v_out;
      std::vector<int>    v_out_expected{0, 2, 4, 6, 8};

      auto fv = v_in | etl::views::filter(even);

      for (auto i : fv)
      {
        v_out.push_back(i);
      }

      CHECK_EQUAL(v_out_expected.size(), v_out.size());
      for (size_t idx = 0; idx < v_out_expected.size(); ++idx)
      {
        CHECK_EQUAL(v_out_expected[idx], v_out[idx]);
      }
    }

    TEST(test_ranges_filter_view_single_element)
    {
      auto even = [](int i) -> bool
      {
        return 0 == i % 2;
      };

      etl::vector<int, 5> v_in{42};
      etl::vector<int, 5> v_out;
      etl::vector<int, 5> v_out_expected{42};

      auto fv = v_in | etl::views::filter(even);

      for (auto i : fv)
      {
        v_out.push_back(i);
      }

      CHECK_EQUAL(v_out_expected, v_out);
    }

    TEST(test_ranges_filter_view_c_array)
    {
      auto even = [](int i) -> bool
      {
        return 0 == i % 2;
      };

      int                  v_in[10] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
      etl::vector<int, 10> v_out;
      etl::vector<int, 10> v_out_expected{0, 2, 4, 6, 8};

      auto fv = etl::views::filter(v_in, even);

      for (auto i : fv)
      {
        v_out.push_back(i);
      }

      CHECK_EQUAL(v_out_expected, v_out);
    }

    TEST(test_ranges_filter_view_c_array_pipe)
    {
      auto even = [](int i) -> bool
      {
        return 0 == i % 2;
      };

      int                  v_in[10] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
      etl::vector<int, 10> v_out;
      etl::vector<int, 10> v_out_expected{0, 2, 4, 6, 8};

      auto fv = v_in | etl::views::filter(even);

      for (auto i : fv)
      {
        v_out.push_back(i);
      }

      CHECK_EQUAL(v_out_expected, v_out);
    }

    TEST(test_ranges_join_functional)
    {
      using range_type = etl::vector<int, 3>;
      etl::vector<range_type, 4> v{{1, 2, 3}, {4, 5, 6}, {7, 8, 9}, {10, 11, 12}};

      auto result = etl::views::join(v);

      using result_type = etl::vector<int, 30>;
      result_type v_expected{1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12};

      auto expected_view = etl::views::all(v_expected);
      CHECK_EQUAL(result, expected_view);
    }

    TEST(test_ranges_join_pipe)
    {
      using range_type = etl::vector<int, 3>;
      etl::vector<range_type, 4> v{{1, 2, 3}, {4, 5, 6}, {7, 8, 9}, {10, 11, 12}};

      auto result = v | etl::views::join;

      using result_type = etl::vector<int, 30>;
      result_type v_expected{1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12};

      auto expected_view = etl::views::all(v_expected);
      CHECK_EQUAL(result, expected_view);
    }

    TEST(test_ranges_join_degenerated_1)
    {
      using range_type = etl::vector<int, 3>;
      etl::vector<range_type, 6> v{{1}, {1, 2, 3}, {1}, {7, 8, 9}, {1}};

      auto result = v | etl::views::join;

      using result_type = etl::vector<int, 30>;
      result_type v_expected{1, 1, 2, 3, 1, 7, 8, 9, 1};
      auto        expected_view = etl::views::all(v_expected);

      CHECK_EQUAL(9, result.size());
      CHECK_EQUAL(expected_view, result);
    }

    TEST(test_ranges_join_degenerated_2)
    {
      using range_type = etl::vector<int, 3>;
      etl::vector<range_type, 7> v{{}, {1, 2, 3}, {}, {7, 8, 9}, {}};

      auto rv = v | etl::views::join;

      using result_type = etl::vector<int, 30>;
      result_type v_expected{1, 2, 3, 7, 8, 9};
      auto        expected_view = etl::views::all(v_expected);

      CHECK_EQUAL(6, rv.size());
      CHECK_EQUAL(expected_view, rv);
    }

    TEST(test_ranges_join_iterate)
    {
      using range_type = etl::vector<int, 3>;
      etl::vector<range_type, 4> v{{1, 2, 3}, {4, 5, 6}, {7, 8, 9}, {10, 11, 12}};

      using result_type = etl::vector<int, 30>;
      auto result       = v | etl::views::join | etl::ranges::to<result_type>();

      result_type result_v;
      for (auto i : result)
      {
        result_v.emplace_back(i);
      }
      result_type v_expected{1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12};
      CHECK_EQUAL(result_v, v_expected);
    }

    TEST(test_ranges_join_view)
    {
      using range_type = etl::vector<int, 3>;
      etl::vector<range_type, 4> v{{1, 2, 3}, {4, 5, 6}, {7, 8, 9}, {10, 11, 12}};

      using result_type = etl::vector<int, 30>;
      auto result       = v | etl::views::join | etl::ranges::to<result_type>();

      result_type v_expected{1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12};
      CHECK_EQUAL(result, v_expected);
    }

    TEST(test_ranges_join_with_functional)
    {
      using range_type = etl::vector<int, 3>;
      etl::vector<range_type, 4> v{{1, 2, 3}, {4, 5, 6}, {7, 8, 9}, {10, 11, 12}};
      range_type                 pattern{111, 222};

      auto result = etl::views::join_with(v, pattern);

      using result_type = etl::vector<int, 30>;
      result_type v_expected{1, 2, 3, 111, 222, 4, 5, 6, 111, 222, 7, 8, 9, 111, 222, 10, 11, 12};

      auto expected_view = etl::views::all(v_expected);
      CHECK_EQUAL(result, expected_view);
    }

    TEST(test_ranges_join_with_pipe)
    {
      using range_type = etl::vector<int, 3>;
      etl::vector<range_type, 4> v{{1, 2, 3}, {4, 5, 6}, {7, 8, 9}, {10, 11, 12}};
      range_type                 pattern{111, 222};

      auto result = v | etl::views::join_with(pattern);

      using result_type = etl::vector<int, 30>;
      result_type v_expected{1, 2, 3, 111, 222, 4, 5, 6, 111, 222, 7, 8, 9, 111, 222, 10, 11, 12};

      auto expected_view = etl::views::all(v_expected);
      CHECK_EQUAL(result, expected_view);
    }

    TEST(test_ranges_join_with_degenerated_1)
    {
      using range_type = etl::vector<int, 3>;
      etl::vector<range_type, 6> v{{1}, {1, 2, 3}, {1}, {7, 8, 9}, {1}};
      range_type                 pattern{111, 222};

      auto result = v | etl::views::join_with(pattern);

      using result_type = etl::vector<int, 30>;
      result_type v_expected{1, 111, 222, 1, 2, 3, 111, 222, 1, 111, 222, 7, 8, 9, 111, 222, 1};
      auto        expected_view = etl::views::all(v_expected);

      CHECK_EQUAL(17, result.size());
      CHECK_EQUAL(expected_view, result);
    }

    TEST(test_ranges_join_with_degenerated_2)
    {
      using range_type = etl::vector<int, 3>;
      etl::vector<range_type, 7> v{{}, {1, 2, 3}, {}, {7, 8, 9}, {}};
      range_type                 pattern{111, 222};

      auto rv = v | etl::views::join_with(pattern);

      using result_type = etl::vector<int, 30>;
      result_type v_expected{111, 222, 1, 2, 3, 111, 222, 111, 222, 7, 8, 9, 111, 222};
      auto        expected_view = etl::views::all(v_expected);

      CHECK_EQUAL(14, rv.size());
      CHECK_EQUAL(expected_view, rv);
    }

    TEST(test_ranges_join_with_iterate)
    {
      using range_type = etl::vector<int, 3>;
      etl::vector<range_type, 4> v{{1, 2, 3}, {4, 5, 6}, {7, 8, 9}, {10, 11, 12}};
      range_type                 pattern{111, 222};

      using result_type = etl::vector<int, 30>;
      auto result       = v | etl::views::join_with(pattern) | etl::ranges::to<result_type>();

      result_type result_v;
      for (auto i : result)
      {
        result_v.emplace_back(i);
      }
      result_type v_expected{1, 2, 3, 111, 222, 4, 5, 6, 111, 222, 7, 8, 9, 111, 222, 10, 11, 12};
      CHECK_EQUAL(result_v, v_expected);
    }

    TEST(test_ranges_join_with_view)
    {
      using range_type = etl::vector<int, 3>;
      etl::vector<range_type, 4> v{{1, 2, 3}, {4, 5, 6}, {7, 8, 9}, {10, 11, 12}};
      range_type                 pattern{111, 222};

      using result_type = etl::vector<int, 30>;
      auto result       = v | etl::views::join_with(pattern) | etl::ranges::to<result_type>();

      result_type v_expected{1, 2, 3, 111, 222, 4, 5, 6, 111, 222, 7, 8, 9, 111, 222, 10, 11, 12};
      CHECK_EQUAL(result, v_expected);
    }

    TEST(test_ranges_join_with_functional_single)
    {
      using range_type = etl::vector<int, 3>;
      etl::vector<range_type, 4> v{{1, 2, 3}, {4, 5, 6}, {7, 8, 9}, {10, 11, 12}};
      int                        pattern{111};

      auto result = etl::views::join_with(v, pattern);

      using result_type = etl::vector<int, 30>;
      result_type v_expected{1, 2, 3, 111, 4, 5, 6, 111, 7, 8, 9, 111, 10, 11, 12};

      auto expected_view = etl::views::all(v_expected);
      CHECK_EQUAL(result, expected_view);
    }

    TEST(test_ranges_join_with_functional_single_immediate)
    {
      using range_type = etl::vector<int, 3>;
      etl::vector<range_type, 4> v{{1, 2, 3}, {4, 5, 6}, {7, 8, 9}, {10, 11, 12}};

      auto result = etl::views::join_with(v, 111);

      using result_type = etl::vector<int, 30>;
      result_type v_expected{1, 2, 3, 111, 4, 5, 6, 111, 7, 8, 9, 111, 10, 11, 12};

      auto expected_view = etl::views::all(v_expected);
      CHECK_EQUAL(result, expected_view);
    }

    TEST(test_ranges_join_with_pipe_single)
    {
      using range_type = etl::vector<int, 3>;
      etl::vector<range_type, 4> v{{1, 2, 3}, {4, 5, 6}, {7, 8, 9}, {10, 11, 12}};
      int                        pattern{111};

      auto result = v | etl::views::join_with(pattern);

      using result_type = etl::vector<int, 30>;
      result_type v_expected{1, 2, 3, 111, 4, 5, 6, 111, 7, 8, 9, 111, 10, 11, 12};

      auto expected_view = etl::views::all(v_expected);
      CHECK_EQUAL(result, expected_view);
    }

    TEST(test_ranges_join_with_iterate_single)
    {
      using range_type = etl::vector<int, 3>;
      etl::vector<range_type, 4> v{{1, 2, 3}, {4, 5, 6}, {7, 8, 9}, {10, 11, 12}};
      int                        pattern{111};

      using result_type = etl::vector<int, 30>;
      auto result       = v | etl::views::join_with(pattern) | etl::ranges::to<result_type>();

      result_type result_v;
      for (auto i : result)
      {
        result_v.emplace_back(i);
      }
      result_type v_expected{1, 2, 3, 111, 4, 5, 6, 111, 7, 8, 9, 111, 10, 11, 12};
      CHECK_EQUAL(result_v, v_expected);
    }

    TEST(test_ranges_join_with_view_single)
    {
      using range_type = etl::vector<int, 3>;
      etl::vector<range_type, 4> v{{1, 2, 3}, {4, 5, 6}, {7, 8, 9}, {10, 11, 12}};
      int                        pattern{111};

      using result_type = etl::vector<int, 30>;
      auto result       = v | etl::views::join_with(pattern) | etl::ranges::to<result_type>();

      result_type v_expected{1, 2, 3, 111, 4, 5, 6, 111, 7, 8, 9, 111, 10, 11, 12};
      CHECK_EQUAL(result, v_expected);
    }

    //*************************************************************************
    // split_view and views::split tests
    //*************************************************************************
    TEST(test_ranges_split_view_int_vector)
    {
      etl::vector<int, 10>          v_in{1, 2, 0, 3, 4, 0, 5};
      auto                          sv = etl::ranges::split_view(v_in, 0);
      std::vector<std::vector<int>> expected{{1, 2}, {3, 4}, {5}};
      size_t                        idx = 0;
      for (auto sub : sv)
      {
        std::vector<int> actual(sub.begin(), sub.end());
        CHECK_EQUAL(expected[idx], actual);
        ++idx;
      }
      CHECK_EQUAL(idx, expected.size());
    }

    TEST(test_ranges_split_view_functional_single_delim)
    {
      // Split {1, 2, 0, 3, 4, 0, 5, 6} by 0 => {1,2}, {3,4}, {5,6}
      std::vector<int>              v_in{1, 2, 0, 3, 4, 0, 5, 6};
      auto                          sv = etl::ranges::split_view(v_in, 0);
      std::vector<std::vector<int>> expected{{1, 2}, {3, 4}, {5, 6}};
      size_t                        idx = 0;
      for (auto sub : sv)
      {
        std::vector<int> actual(sub.begin(), sub.end());
        CHECK_EQUAL(expected[idx], actual);
        ++idx;
      }
      CHECK_EQUAL(idx, expected.size());
    }

    TEST(test_ranges_split_view_multi_pattern)
    {
      // Split {1, 2, 0, 0, 3, 4, 0, 0, 5} by pattern {0, 0} => {1,2}, {3,4},
      // {5}
      std::vector<int>              v_in{1, 2, 0, 0, 3, 4, 0, 0, 5};
      std::vector<int>              delim{0, 0};
      auto                          sv = etl::ranges::split_view(v_in, delim);
      std::vector<std::vector<int>> expected{{1, 2}, {3, 4}, {5}};
      size_t                        idx = 0;
      for (auto sub : sv)
      {
        std::vector<int> actual(sub.begin(), sub.end());
        CHECK_EQUAL(expected[idx], actual);
        ++idx;
      }
      CHECK_EQUAL(idx, expected.size());
    }

    TEST(test_ranges_split_view_pipe)
    {
      // Pipe syntax: {1, 0, 2, 0, 3} | split(0) => {1}, {2}, {3}
      std::vector<int>              v_in{1, 0, 2, 0, 3};
      auto                          sv = v_in | etl::views::split(0);
      std::vector<std::vector<int>> expected{{1}, {2}, {3}};
      size_t                        idx = 0;
      for (auto sub : sv)
      {
        std::vector<int> actual(sub.begin(), sub.end());
        CHECK_EQUAL(expected[idx], actual);
        ++idx;
      }
      CHECK_EQUAL(idx, expected.size());
    }

    TEST(test_ranges_split_view_empty_input)
    {
      // Split empty range => one empty subrange
      std::vector<int> v_in{};
      auto             sv    = etl::ranges::split_view(v_in, 0);
      size_t           count = 0;
      for (auto sub : sv)
      {
        std::vector<int> actual(sub.begin(), sub.end());
        CHECK_EQUAL(actual.size(), 0U);
        ++count;
      }
      CHECK_EQUAL(count, 1U);
    }

    TEST(test_ranges_split_view_no_delim)
    {
      // No delimiter found => one subrange with all elements
      std::vector<int> v_in{1, 2, 3};
      auto             sv    = etl::ranges::split_view(v_in, 0);
      size_t           count = 0;
      for (auto sub : sv)
      {
        std::vector<int> actual(sub.begin(), sub.end());
        std::vector<int> expected_sub{1, 2, 3};
        CHECK_EQUAL(expected_sub, actual);
        ++count;
      }
      CHECK_EQUAL(count, 1U);
    }

    TEST(test_ranges_split_view_delim_at_edges)
    {
      // Delimiter at start and end: {0, 1, 2, 0} => {}, {1,2}, {}
      std::vector<int>              v_in{0, 1, 2, 0};
      auto                          sv = etl::ranges::split_view(v_in, 0);
      std::vector<std::vector<int>> expected{{}, {1, 2}, {}};
      size_t                        idx = 0;
      for (auto sub : sv)
      {
        std::vector<int> actual(sub.begin(), sub.end());
        CHECK_EQUAL(expected[idx], actual);
        ++idx;
      }
      CHECK_EQUAL(idx, expected.size());
    }

    TEST(test_ranges_split_view_consecutive_delim)
    {
      // Consecutive delimiters: {1, 0, 0, 2} => {1}, {}, {2}
      std::vector<int>              v_in{1, 0, 0, 2};
      auto                          sv = etl::ranges::split_view(v_in, 0);
      std::vector<std::vector<int>> expected{{1}, {}, {2}};
      size_t                        idx = 0;
      for (auto sub : sv)
      {
        std::vector<int> actual(sub.begin(), sub.end());
        CHECK_EQUAL(expected[idx], actual);
        ++idx;
      }
      CHECK_EQUAL(idx, expected.size());
    }

    TEST(test_ranges_split_view_pipe_multi_pattern)
    {
      // Pipe with multi-element pattern: {1, 2, 9, 9, 3, 4, 9, 9, 5} |
      // split({9,9}) => {1,2}, {3,4}, {5}
      std::vector<int>              v_in{1, 2, 9, 9, 3, 4, 9, 9, 5};
      std::vector<int>              delim{9, 9};
      auto                          sv = v_in | etl::views::split(delim);
      std::vector<std::vector<int>> expected{{1, 2}, {3, 4}, {5}};
      size_t                        idx = 0;
      for (auto sub : sv)
      {
        std::vector<int> actual(sub.begin(), sub.end());
        CHECK_EQUAL(expected[idx], actual);
        ++idx;
      }
      CHECK_EQUAL(idx, expected.size());
    }

    TEST(test_ranges_split_view_etl_vector)
    {
      // Works with etl::vector too
      etl::vector<int, 10>          v_in{1, 0, 2, 0, 3};
      auto                          sv = etl::ranges::split_view(v_in, 0);
      std::vector<std::vector<int>> expected{{1}, {2}, {3}};
      size_t                        idx = 0;
      for (auto sub : sv)
      {
        std::vector<int> actual(sub.begin(), sub.end());
        CHECK_EQUAL(expected[idx], actual);
        ++idx;
      }
      CHECK_EQUAL(idx, expected.size());
    }

    //*************************************************************************
    // lazy_split_view and views::lazy_split tests
    //*************************************************************************
    TEST(test_ranges_lazy_split_view_int_vector)
    {
      etl::vector<int, 10>          v_in{1, 2, 0, 3, 4, 0, 5};
      auto                          sv = etl::ranges::lazy_split_view(v_in, 0);
      std::vector<std::vector<int>> expected{{1, 2}, {3, 4}, {5}};
      size_t                        idx = 0;
      for (auto sub : sv)
      {
        std::vector<int> actual(sub.begin(), sub.end());
        CHECK_EQUAL(expected[idx], actual);
        ++idx;
      }
      CHECK_EQUAL(idx, expected.size());
    }

    TEST(test_ranges_lazy_split_view_functional_single_delim)
    {
      // Split {1, 2, 0, 3, 4, 0, 5, 6} by 0 => {1,2}, {3,4}, {5,6}
      std::vector<int>              v_in{1, 2, 0, 3, 4, 0, 5, 6};
      auto                          sv = etl::ranges::lazy_split_view(v_in, 0);
      std::vector<std::vector<int>> expected{{1, 2}, {3, 4}, {5, 6}};
      size_t                        idx = 0;
      for (auto sub : sv)
      {
        std::vector<int> actual(sub.begin(), sub.end());
        CHECK_EQUAL(expected[idx], actual);
        ++idx;
      }
      CHECK_EQUAL(idx, expected.size());
    }

    TEST(test_ranges_lazy_split_view_multi_pattern)
    {
      // Split {1, 2, 0, 0, 3, 4, 0, 0, 5} by pattern {0, 0} => {1,2}, {3,4},
      // {5}
      std::vector<int>              v_in{1, 2, 0, 0, 3, 4, 0, 0, 5};
      std::vector<int>              delim{0, 0};
      auto                          sv = etl::ranges::lazy_split_view(v_in, delim);
      std::vector<std::vector<int>> expected{{1, 2}, {3, 4}, {5}};
      size_t                        idx = 0;
      for (auto sub : sv)
      {
        std::vector<int> actual(sub.begin(), sub.end());
        CHECK_EQUAL(expected[idx], actual);
        ++idx;
      }
      CHECK_EQUAL(idx, expected.size());
    }

    TEST(test_ranges_lazy_split_view_pipe)
    {
      // Pipe syntax: {1, 0, 2, 0, 3} | lazy_split(0) => {1}, {2}, {3}
      std::vector<int>              v_in{1, 0, 2, 0, 3};
      auto                          sv = v_in | etl::views::lazy_split(0);
      std::vector<std::vector<int>> expected{{1}, {2}, {3}};
      size_t                        idx = 0;
      for (auto sub : sv)
      {
        std::vector<int> actual(sub.begin(), sub.end());
        CHECK_EQUAL(expected[idx], actual);
        ++idx;
      }
      CHECK_EQUAL(idx, expected.size());
    }

    TEST(test_ranges_lazy_split_view_empty_input)
    {
      // Split empty range => one empty subrange
      std::vector<int> v_in{};
      auto             sv    = etl::ranges::lazy_split_view(v_in, 0);
      size_t           count = 0;
      for (auto sub : sv)
      {
        std::vector<int> actual(sub.begin(), sub.end());
        CHECK_EQUAL(actual.size(), 0U);
        ++count;
      }
      CHECK_EQUAL(count, 1U);
    }

    TEST(test_ranges_lazy_split_view_no_delim)
    {
      // No delimiter found => one subrange with all elements
      std::vector<int> v_in{1, 2, 3};
      auto             sv    = etl::ranges::lazy_split_view(v_in, 0);
      size_t           count = 0;
      for (auto sub : sv)
      {
        std::vector<int> actual(sub.begin(), sub.end());
        std::vector<int> expected_sub{1, 2, 3};
        CHECK_EQUAL(expected_sub, actual);
        ++count;
      }
      CHECK_EQUAL(count, 1U);
    }

    TEST(test_ranges_lazy_split_view_delim_at_edges)
    {
      // Delimiter at start and end: {0, 1, 2, 0} => {}, {1,2}, {}
      std::vector<int>              v_in{0, 1, 2, 0};
      auto                          sv = etl::ranges::lazy_split_view(v_in, 0);
      std::vector<std::vector<int>> expected{{}, {1, 2}, {}};
      size_t                        idx = 0;
      for (auto sub : sv)
      {
        std::vector<int> actual(sub.begin(), sub.end());
        CHECK_EQUAL(expected[idx], actual);
        ++idx;
      }
      CHECK_EQUAL(idx, expected.size());
    }

    TEST(test_ranges_lazy_split_view_consecutive_delim)
    {
      // Consecutive delimiters: {1, 0, 0, 2} => {1}, {}, {2}
      std::vector<int>              v_in{1, 0, 0, 2};
      auto                          sv = etl::ranges::lazy_split_view(v_in, 0);
      std::vector<std::vector<int>> expected{{1}, {}, {2}};
      size_t                        idx = 0;
      for (auto sub : sv)
      {
        std::vector<int> actual(sub.begin(), sub.end());
        CHECK_EQUAL(expected[idx], actual);
        ++idx;
      }
      CHECK_EQUAL(idx, expected.size());
    }

    TEST(test_ranges_lazy_split_view_pipe_multi_pattern)
    {
      // Pipe with multi-element pattern: {1, 2, 9, 9, 3, 4, 9, 9, 5} |
      // lazy_split({9,9}) => {1,2}, {3,4}, {5}
      std::vector<int>              v_in{1, 2, 9, 9, 3, 4, 9, 9, 5};
      std::vector<int>              delim{9, 9};
      auto                          sv = v_in | etl::views::lazy_split(delim);
      std::vector<std::vector<int>> expected{{1, 2}, {3, 4}, {5}};
      size_t                        idx = 0;
      for (auto sub : sv)
      {
        std::vector<int> actual(sub.begin(), sub.end());
        CHECK_EQUAL(expected[idx], actual);
        ++idx;
      }
      CHECK_EQUAL(idx, expected.size());
    }

    TEST(test_ranges_lazy_split_view_etl_vector)
    {
      // Works with etl::vector too
      etl::vector<int, 10>          v_in{1, 0, 2, 0, 3};
      auto                          sv = etl::ranges::lazy_split_view(v_in, 0);
      std::vector<std::vector<int>> expected{{1}, {2}, {3}};
      size_t                        idx = 0;
      for (auto sub : sv)
      {
        std::vector<int> actual(sub.begin(), sub.end());
        CHECK_EQUAL(expected[idx], actual);
        ++idx;
      }
      CHECK_EQUAL(idx, expected.size());
    }

    TEST(test_ranges_lazy_split_view_inner_range_lazy_iteration)
    {
      // Verify that the inner range can be iterated element-by-element (lazily)
      std::vector<int> v_in{10, 20, 0, 30, 40, 50};
      auto             sv       = etl::ranges::lazy_split_view(v_in, 0);
      auto             outer_it = sv.begin();

      // First segment: {10, 20}
      auto inner1   = *outer_it;
      auto inner_it = inner1.begin();
      CHECK_EQUAL(*inner_it, 10);
      ++inner_it;
      CHECK_EQUAL(*inner_it, 20);
      ++inner_it;
      CHECK(inner_it == inner1.end());

      // Second segment: {30, 40, 50}
      ++outer_it;
      auto inner2    = *outer_it;
      auto inner_it2 = inner2.begin();
      CHECK_EQUAL(*inner_it2, 30);
      ++inner_it2;
      CHECK_EQUAL(*inner_it2, 40);
      ++inner_it2;
      CHECK_EQUAL(*inner_it2, 50);
      ++inner_it2;
      CHECK(inner_it2 == inner2.end());

      ++outer_it;
      CHECK(outer_it == sv.end());
    }

    TEST(test_ranges_lazy_split_view_inner_range_empty_check)
    {
      // Inner range empty() method
      std::vector<int> v_in{0, 1, 0};
      auto             sv = etl::ranges::lazy_split_view(v_in, 0);
      auto             it = sv.begin();

      // First segment is empty
      auto seg0 = *it;
      CHECK(seg0.empty());

      ++it;
      auto seg1 = *it;
      CHECK(!seg1.empty());

      ++it;
      auto seg2 = *it;
      CHECK(seg2.empty());
    }

    TEST(test_counted)
    {
      {
        std::vector<int> vec{1, 2, 3, 4, 5};

        auto result = etl::views::counted(vec.begin(), 3);

        CHECK_EQUAL(result.size(), 3);
        CHECK_EQUAL(result[0], 1);
        CHECK_EQUAL(result[1], 2);
        CHECK_EQUAL(result[2], 3);
      }
      {
        std::list<int> list{1, 2, 3, 4, 5};

        auto result = etl::views::counted(list.begin(), 3);

        CHECK_EQUAL(result.size(), 3);
        CHECK_EQUAL(result[0], 1);
        CHECK_EQUAL(result[1], 2);
        CHECK_EQUAL(result[2], 3);
      }
    }

    TEST(test_ranges_concat_view_1)
    {
      std::vector<int> vec{1, 2, 3, 4};

      using result_type  = std::vector<int>;
      auto        cv     = etl::views::concat(vec);
      result_type result = etl::views::concat(vec) | etl::ranges::to<result_type>();

      result_type expected_result{1, 2, 3, 4};

      CHECK_EQUAL(cv.size(), 4);
      CHECK_EQUAL(result.size(), 4);

      CHECK_EQUAL(result, expected_result);
    }

    TEST(test_ranges_concat_view_2)
    {
      std::vector<int> vec{1, 2, 3, 4};
      std::list<int>   list{6, 7, 8, 9, 10};

      auto cv            = etl::views::concat(vec, list);
      using result_type  = std::vector<int>;
      result_type result = etl::views::concat(vec, list) | etl::ranges::to<result_type>();

      result_type expected_result{1, 2, 3, 4, 6, 7, 8, 9, 10};

      CHECK_EQUAL(cv.size(), 9);
      CHECK_EQUAL(result.size(), 9);

      CHECK_EQUAL(result, expected_result);
    }

    TEST(test_ranges_concat_view_2_same_type)
    {
      std::vector<int> vec0{1, 2, 3, 4};
      std::vector<int> vec1{6, 7, 8, 9, 10};

      auto cv = etl::views::concat(vec0, vec1);

      using result_type  = std::vector<int>;
      result_type result = etl::views::concat(vec0, vec1) | etl::ranges::to<result_type>();

      std::vector<int> expected_result{1, 2, 3, 4, 6, 7, 8, 9, 10};

      CHECK_EQUAL(cv.size(), 9);
      CHECK_EQUAL(result.size(), 9);

      CHECK_EQUAL(result, expected_result);
    }

    TEST(test_ranges_concat_view_3)
    {
      std::vector<int>   vec{1, 2, 3, 4};
      std::list<int>     list{6, 7, 8, 9, 10};
      std::array<int, 3> arr{20, 21, 22};

      auto cv = etl::views::concat(vec, list, arr);

      using result_type  = std::vector<int>;
      result_type result = etl::views::concat(vec, list, arr) | etl::ranges::to<result_type>();

      std::vector<int> expected_result{1, 2, 3, 4, 6, 7, 8, 9, 10, 20, 21, 22};

      CHECK_EQUAL(cv.size(), 12);
      CHECK_EQUAL(result.size(), 12);

      CHECK_EQUAL(result, expected_result);

      auto it = cv.begin();
      CHECK(it != cv.end());
      CHECK_EQUAL(*it, 1);

      ++it;
      CHECK_EQUAL(*it, 2);
      it++;
      CHECK_EQUAL(*it, 3);
      CHECK_EQUAL(it[0], 3);
      CHECK_EQUAL(it[1], 4);
      CHECK_EQUAL(it[2], 6);
      it += 3;
      CHECK_EQUAL(*it, 7);
      it -= 2;
      CHECK_EQUAL(*it, 4);
      --it;
      CHECK_EQUAL(*it, 3);
      it--;
      CHECK_EQUAL(*it, 2);
      CHECK(it != cv.end());
      it += 8;
      CHECK_EQUAL(*it, 20);
      it += 2;
      CHECK_EQUAL(*it, 22);
      CHECK(it != cv.end());
      ++it;
      CHECK(it == cv.end());
    }

    TEST(test_ranges_concat_view_3_same_type)
    {
      std::vector<int> vec0{1, 2, 3, 4};
      std::vector<int> vec1{6, 7, 8, 9, 10};
      std::vector<int> vec2{20, 21, 22};

      auto cv = etl::views::concat(vec0, vec1, vec2);

      using result_type  = std::vector<int>;
      result_type result = etl::views::concat(vec0, vec1, vec2) | etl::ranges::to<result_type>();

      std::vector<int> expected_result{1, 2, 3, 4, 6, 7, 8, 9, 10, 20, 21, 22};

      CHECK_EQUAL(cv.size(), 12);
      CHECK_EQUAL(result.size(), 12);

      CHECK_EQUAL(result, expected_result);

      auto it = cv.begin();
      CHECK(it != cv.end());
      CHECK_EQUAL(*it, 1);

      ++it;
      CHECK_EQUAL(*it, 2);
      it++;
      CHECK_EQUAL(*it, 3);
      CHECK_EQUAL(it[0], 3);
      CHECK_EQUAL(it[1], 4);
      CHECK_EQUAL(it[2], 6);
      it += 3;
      CHECK_EQUAL(*it, 7);
      it -= 2;
      CHECK_EQUAL(*it, 4);
      --it;
      CHECK_EQUAL(*it, 3);
      it--;
      CHECK_EQUAL(*it, 2);
      CHECK(it != cv.end());
      it += 8;
      CHECK_EQUAL(*it, 20);
      it += 2;
      CHECK_EQUAL(*it, 22);
      CHECK(it != cv.end());
      ++it;
      CHECK(it == cv.end());
    }

    TEST(test_ranges_concat_view_3_same_range)
    {
      std::vector<int> vec0{1, 2, 3, 4, 5};

      auto cv = etl::views::concat(vec0, vec0, vec0);

      using result_type  = std::vector<int>;
      result_type result = etl::views::concat(vec0, vec0, vec0) | etl::ranges::to<result_type>();

      std::vector<int> expected_result{1, 2, 3, 4, 5, 1, 2, 3, 4, 5, 1, 2, 3, 4, 5};

      CHECK_EQUAL(cv.size(), 15);
      CHECK_EQUAL(result.size(), 15);

      CHECK_EQUAL(result, expected_result);

      auto it = cv.begin();
      CHECK(it != cv.end());
      CHECK_EQUAL(*it, 1);

      ++it;
      CHECK_EQUAL(*it, 2);
      it++;
      CHECK_EQUAL(*it, 3);
      CHECK_EQUAL(it[0], 3);
      CHECK_EQUAL(it[1], 4);
      CHECK_EQUAL(it[2], 5);
      it += 3;
      CHECK_EQUAL(*it, 1);
      it -= 2;
      CHECK_EQUAL(*it, 4);
      --it;
      CHECK_EQUAL(*it, 3);
      it--;
      CHECK_EQUAL(*it, 2);
      CHECK(it != cv.end());
      it += 11;
      CHECK_EQUAL(*it, 3);
      it += 2;
      CHECK_EQUAL(*it, 5);
      CHECK(it != cv.end());
      ++it;
      CHECK(it == cv.end());
    }

    //*************************************************************************
    // common_view tests
    //*************************************************************************

    TEST(test_ranges_common_view_functional)
    {
      etl::vector<int, 10> v_in{0, 1, 2, 3, 4};
      etl::vector<int, 10> v_out;
      etl::vector<int, 10> v_out_expected{0, 1, 2, 3, 4};

      auto cv = etl::ranges::common_view(v_in);
      for (int i : cv)
      {
        v_out.push_back(i);
      }

      CHECK_EQUAL(v_out_expected, v_out);
      CHECK_EQUAL(*cv.begin(), 0);
      CHECK_EQUAL(cv.size(), 5);

      CHECK_EQUAL(*cv.cbegin(), 0);
      CHECK_EQUAL(cv.empty(), false);
      CHECK_EQUAL(cv, true);
      CHECK_EQUAL(cv.front(), 0);
      CHECK_EQUAL(cv.back(), 4);
      CHECK_EQUAL(cv[2], 2);
    }

    TEST(test_ranges_common_view_from_view)
    {
      etl::vector<int, 10> v_in{0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
      etl::vector<int, 10> v_out;
      etl::vector<int, 10> v_out_expected{3, 4, 5, 6, 7, 8, 9};

      auto cv = etl::ranges::common_view(etl::views::drop(v_in, 3));
      for (int i : cv)
      {
        v_out.push_back(i);
      }

      CHECK_EQUAL(v_out_expected, v_out);
      CHECK_EQUAL(*cv.begin(), 3);
      CHECK_EQUAL(cv.size(), 7);
      CHECK_EQUAL(cv.empty(), false);
      CHECK_EQUAL(cv.front(), 3);
      CHECK_EQUAL(cv.back(), 9);
    }

    TEST(test_ranges_common_view_pipe)
    {
      etl::vector<int, 10> v_in{0, 1, 2, 3, 4};
      etl::vector<int, 10> v_out;
      etl::vector<int, 10> v_out_expected{0, 1, 2, 3, 4};

      auto cv = v_in | etl::views::common;
      for (int i : cv)
      {
        v_out.push_back(i);
      }

      CHECK_EQUAL(v_out_expected, v_out);
      CHECK_EQUAL(*cv.begin(), 0);
      CHECK_EQUAL(cv.size(), 5);
      CHECK_EQUAL(cv.empty(), false);
      CHECK_EQUAL(cv.front(), 0);
      CHECK_EQUAL(cv.back(), 4);
    }

    TEST(test_ranges_common_view_pipe_chain)
    {
      etl::vector<int, 10> v_in{0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
      etl::vector<int, 10> v_out;
      etl::vector<int, 10> v_out_expected{4, 3, 2, 1, 0};

      auto cv = v_in | etl::views::take(5) | etl::views::reverse | etl::views::common;
      for (int i : cv)
      {
        v_out.push_back(i);
      }

      CHECK_EQUAL(v_out_expected, v_out);
      CHECK_EQUAL(cv.size(), 5);
      CHECK_EQUAL(cv.front(), 4);
      CHECK_EQUAL(cv.back(), 0);
    }

    TEST(test_ranges_common_view_empty)
    {
      etl::vector<int, 10> v_in{};

      auto cv = etl::ranges::common_view(v_in);

      CHECK_EQUAL(cv.size(), 0);
      CHECK_EQUAL(cv.empty(), true);
      CHECK_EQUAL(cv.begin(), cv.end());
    }

    TEST(test_ranges_common_view_same_begin_end_type)
    {
      // For a common range (begin/end same type), common_view should work as
      // pass-through
      etl::vector<int, 10> v_in{10, 20, 30};

      auto cv       = etl::views::common(v_in);
      auto it_begin = cv.begin();
      auto it_end   = cv.end();

      // Verify same type via compilation and correct values
      CHECK(it_begin != it_end);
      CHECK_EQUAL(*it_begin, 10);
      CHECK_EQUAL(cv.size(), 3);
      CHECK_EQUAL(cv.front(), 10);
      CHECK_EQUAL(cv.back(), 30);
    }

    TEST(test_ranges_common_view_iterate)
    {
      etl::vector<int, 10> v_in{1, 2, 3, 4, 5};

      using result_type  = etl::vector<int, 10>;
      result_type result = v_in | etl::views::common | etl::ranges::to<result_type>();

      result_type v_expected{1, 2, 3, 4, 5};
      CHECK_EQUAL(result, v_expected);
    }

    TEST(test_ranges_common_view_with_filter)
    {
      auto even = [](int i) -> bool
      {
        return 0 == i % 2;
      };

      etl::vector<int, 10> v_in{0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
      etl::vector<int, 10> v_out;
      etl::vector<int, 10> v_out_expected{0, 2, 4, 6, 8};

      auto cv = v_in | etl::views::filter(even) | etl::views::common();
      for (int i : cv)
      {
        v_out.push_back(i);
      }

      CHECK_EQUAL(v_out_expected, v_out);
    }

    TEST(test_ranges_common_view_with_transform)
    {
      auto times_two = [](int i) -> int
      {
        return i * 2;
      };

      etl::vector<int, 10> v_in{1, 2, 3, 4, 5};
      etl::vector<int, 10> v_out;
      etl::vector<int, 10> v_out_expected{2, 4, 6, 8, 10};

      auto cv = v_in | etl::views::transform(times_two) | etl::views::common;
      for (int i : cv)
      {
        v_out.push_back(i);
      }

      CHECK_EQUAL(v_out_expected, v_out);
    }

    TEST(test_ranges_common_view_std_vector)
    {
      std::vector<int> v_in{10, 20, 30, 40, 50};
      std::vector<int> v_out;
      std::vector<int> v_out_expected{10, 20, 30, 40, 50};

      auto cv = etl::ranges::common_view(etl::ranges::ref_view(v_in));
      for (int i : cv)
      {
        v_out.push_back(i);
      }

      CHECK_EQUAL(v_out_expected, v_out);
      CHECK_EQUAL(cv.size(), 5);
      CHECK_EQUAL(cv.front(), 10);
      CHECK_EQUAL(cv.back(), 50);
    }

    TEST(test_ranges_common_view_c_array)
    {
      int                  v_in[] = {1, 2, 3, 4, 5};
      etl::vector<int, 10> v_out;
      etl::vector<int, 10> v_out_expected{1, 2, 3, 4, 5};

      auto cv = etl::ranges::common_view(etl::ranges::ref_view(v_in));
      for (int i : cv)
      {
        v_out.push_back(i);
      }

      CHECK_EQUAL(v_out_expected, v_out);
      CHECK_EQUAL(cv.size(), 5);
    }

    //*************************************************************************
    TEST(test_ranges_elements_of_from_lvalue)
    {
      std::vector<int> v{1, 2, 3, 4, 5};

      auto eo = etl::ranges::elements_of{v};

      // The wrapped range should refer to the same data
      CHECK_EQUAL(v.size(), eo.range.size());
      CHECK_EQUAL(v[0], eo.range[0]);
      CHECK_EQUAL(v[4], eo.range[4]);
    }

    //*************************************************************************
    TEST(test_ranges_elements_of_from_rvalue)
    {
      auto eo = etl::ranges::elements_of{std::vector<int>{10, 20, 30}};

      CHECK_EQUAL(3U, eo.range.size());
      CHECK_EQUAL(10, eo.range[0]);
      CHECK_EQUAL(20, eo.range[1]);
      CHECK_EQUAL(30, eo.range[2]);
    }

    //*************************************************************************
    TEST(test_ranges_elements_of_deduction_guide)
    {
      std::vector<int> v{1, 2, 3};

      // CTAD should deduce elements_of<std::vector<int>&>
      auto eo = etl::ranges::elements_of{v};
      (void)eo;

      // CTAD from rvalue should deduce elements_of<std::vector<int>&&>
      auto eo2 = etl::ranges::elements_of{std::vector<int>{4, 5, 6}};
      (void)eo2;
    }

    //*************************************************************************
    TEST(test_ranges_elements_of_c_array)
    {
      int arr[] = {10, 20, 30, 40, 50};

      auto eo = etl::ranges::elements_of{arr};

      CHECK_EQUAL(10, eo.range[0]);
      CHECK_EQUAL(50, eo.range[4]);
    }

    //*************************************************************************
    TEST(test_ranges_elements_view_pair_first)
    {
      std::vector<std::pair<int, double>> v = {{1, 1.1}, {2, 2.2}, {3, 3.3}};

      auto ev = etl::ranges::elements_view<etl::ranges::views::all_t<decltype(v)&>, 0>(etl::ranges::views::all(v));

      auto it = ev.begin();
      CHECK_EQUAL(1, *it);
      ++it;
      CHECK_EQUAL(2, *it);
      ++it;
      CHECK_EQUAL(3, *it);
      ++it;
      CHECK(it == ev.end());
    }

    //*************************************************************************
    TEST(test_ranges_elements_view_pair_second)
    {
      std::vector<std::pair<int, double>> v = {{1, 1.1}, {2, 2.2}, {3, 3.3}};

      auto ev = etl::ranges::elements_view<etl::ranges::views::all_t<decltype(v)&>, 1>(etl::ranges::views::all(v));

      auto it = ev.begin();
      CHECK_CLOSE(1.1, *it, 0.001);
      ++it;
      CHECK_CLOSE(2.2, *it, 0.001);
      ++it;
      CHECK_CLOSE(3.3, *it, 0.001);
      ++it;
      CHECK(it == ev.end());
    }

    //*************************************************************************
    TEST(test_ranges_elements_view_tuple)
    {
      std::vector<etl::tuple<int, double, char>> v = {etl::make_tuple(10, 1.5, 'a'), etl::make_tuple(20, 2.5, 'b'), etl::make_tuple(30, 3.5, 'c')};

      // Extract element 2 (char)
      auto ev = etl::ranges::elements_view<etl::ranges::views::all_t<decltype(v)&>, 2>(etl::ranges::views::all(v));

      auto it = ev.begin();
      CHECK_EQUAL('a', *it);
      ++it;
      CHECK_EQUAL('b', *it);
      ++it;
      CHECK_EQUAL('c', *it);
      ++it;
      CHECK(it == ev.end());
    }

    //*************************************************************************
    TEST(test_ranges_elements_view_size)
    {
      std::vector<std::pair<int, int>> v = {{1, 10}, {2, 20}, {3, 30}, {4, 40}};

      auto ev = etl::ranges::elements_view<etl::ranges::views::all_t<decltype(v)&>, 0>(etl::ranges::views::all(v));

      CHECK_EQUAL(4U, ev.size());
    }

    //*************************************************************************
    TEST(test_ranges_elements_view_empty)
    {
      std::vector<std::pair<int, int>> v;

      auto ev = etl::ranges::elements_view<etl::ranges::views::all_t<decltype(v)&>, 0>(etl::ranges::views::all(v));

      CHECK(ev.empty());
      CHECK(ev.begin() == ev.end());
    }

    //*************************************************************************
    TEST(test_ranges_views_elements_adaptor)
    {
      std::vector<std::pair<int, double>> v = {{1, 1.1}, {2, 2.2}, {3, 3.3}};

      auto ev = etl::views::elements<0>(v);

      auto it = ev.begin();
      CHECK_EQUAL(1, *it);
      ++it;
      CHECK_EQUAL(2, *it);
      ++it;
      CHECK_EQUAL(3, *it);
      ++it;
      CHECK(it == ev.end());
    }

    //*************************************************************************
    TEST(test_ranges_views_elements_pipe)
    {
      std::vector<std::pair<int, double>> v = {{1, 1.1}, {2, 2.2}, {3, 3.3}};

      auto ev = v | etl::views::elements<1>;

      auto it = ev.begin();
      CHECK_CLOSE(1.1, *it, 0.001);
      ++it;
      CHECK_CLOSE(2.2, *it, 0.001);
      ++it;
      CHECK_CLOSE(3.3, *it, 0.001);
      ++it;
      CHECK(it == ev.end());
    }

    //*************************************************************************
    TEST(test_ranges_keys_view_alias)
    {
      std::vector<std::pair<int, double>> v = {{10, 1.1}, {20, 2.2}, {30, 3.3}};

      using range_t = etl::ranges::views::all_t<decltype(v)&>;
      etl::ranges::keys_view<range_t> kv(etl::ranges::views::all(v));

      auto it = kv.begin();
      CHECK_EQUAL(10, *it);
      ++it;
      CHECK_EQUAL(20, *it);
      ++it;
      CHECK_EQUAL(30, *it);
      ++it;
      CHECK(it == kv.end());
    }

    //*************************************************************************
    TEST(test_ranges_values_view_alias)
    {
      std::vector<std::pair<int, double>> v = {{10, 1.1}, {20, 2.2}, {30, 3.3}};

      using range_t = etl::ranges::views::all_t<decltype(v)&>;
      etl::ranges::values_view<range_t> vv(etl::ranges::views::all(v));

      auto it = vv.begin();
      CHECK_CLOSE(1.1, *it, 0.001);
      ++it;
      CHECK_CLOSE(2.2, *it, 0.001);
      ++it;
      CHECK_CLOSE(3.3, *it, 0.001);
      ++it;
      CHECK(it == vv.end());
    }

    //*************************************************************************
    TEST(test_ranges_views_keys)
    {
      std::vector<std::pair<int, double>> v = {{10, 1.1}, {20, 2.2}, {30, 3.3}};

      auto ev = v | etl::views::keys;

      auto it = ev.begin();
      CHECK_EQUAL(10, *it);
      ++it;
      CHECK_EQUAL(20, *it);
      ++it;
      CHECK_EQUAL(30, *it);
      ++it;
      CHECK(it == ev.end());
    }

    //*************************************************************************
    TEST(test_ranges_views_values)
    {
      std::vector<std::pair<int, double>> v = {{10, 1.1}, {20, 2.2}, {30, 3.3}};

      auto ev = v | etl::views::values;

      auto it = ev.begin();
      CHECK_CLOSE(1.1, *it, 0.001);
      ++it;
      CHECK_CLOSE(2.2, *it, 0.001);
      ++it;
      CHECK_CLOSE(3.3, *it, 0.001);
      ++it;
      CHECK(it == ev.end());
    }

    //*************************************************************************
    TEST(test_ranges_views_keys_direct_call)
    {
      std::vector<std::pair<int, double>> v = {{10, 1.1}, {20, 2.2}, {30, 3.3}};

      auto ev = etl::views::keys(v);

      auto it = ev.begin();
      CHECK_EQUAL(10, *it);
      ++it;
      CHECK_EQUAL(20, *it);
      ++it;
      CHECK_EQUAL(30, *it);
      ++it;
      CHECK(it == ev.end());
    }

    //*************************************************************************
    TEST(test_ranges_views_values_direct_call)
    {
      std::vector<std::pair<int, double>> v = {{10, 1.1}, {20, 2.2}, {30, 3.3}};

      auto ev = etl::views::values(v);

      auto it = ev.begin();
      CHECK_CLOSE(1.1, *it, 0.001);
      ++it;
      CHECK_CLOSE(2.2, *it, 0.001);
      ++it;
      CHECK_CLOSE(3.3, *it, 0.001);
      ++it;
      CHECK(it == ev.end());
    }

    //*************************************************************************
    TEST(test_ranges_elements_view_with_etl_tuple)
    {
      std::vector<etl::tuple<int, double, char>> v = {etl::make_tuple(1, 10.0, 'x'), etl::make_tuple(2, 20.0, 'y'), etl::make_tuple(3, 30.0, 'z')};

      // Extract element 0 via pipe
      auto ev0 = v | etl::views::elements<0>;
      auto it0 = ev0.begin();
      CHECK_EQUAL(1, *it0);
      ++it0;
      CHECK_EQUAL(2, *it0);
      ++it0;
      CHECK_EQUAL(3, *it0);
      ++it0;
      CHECK(it0 == ev0.end());

      // Extract element 1 via pipe
      auto ev1 = v | etl::views::elements<1>;
      auto it1 = ev1.begin();
      CHECK_CLOSE(10.0, *it1, 0.001);
      ++it1;
      CHECK_CLOSE(20.0, *it1, 0.001);
      ++it1;
      CHECK_CLOSE(30.0, *it1, 0.001);
      ++it1;
      CHECK(it1 == ev1.end());

      // Extract element 2 via pipe
      auto ev2 = v | etl::views::elements<2>;
      auto it2 = ev2.begin();
      CHECK_EQUAL('x', *it2);
      ++it2;
      CHECK_EQUAL('y', *it2);
      ++it2;
      CHECK_EQUAL('z', *it2);
      ++it2;
      CHECK(it2 == ev2.end());
    }

    //*************************************************************************
    TEST(test_ranges_enumerate_view_basic)
    {
      std::vector<int> v  = {10, 20, 30};
      auto             ev = etl::ranges::enumerate_view(etl::ranges::views::all(v));

      auto it = ev.begin();
      CHECK_EQUAL(0U, etl::get<0>(*it));
      CHECK_EQUAL(10, etl::get<1>(*it));
      ++it;
      CHECK_EQUAL(1U, etl::get<0>(*it));
      CHECK_EQUAL(20, etl::get<1>(*it));
      ++it;
      CHECK_EQUAL(2U, etl::get<0>(*it));
      CHECK_EQUAL(30, etl::get<1>(*it));
      ++it;
      CHECK(it == ev.end());
    }

    //*************************************************************************
    TEST(test_ranges_enumerate_view_pipe)
    {
      std::vector<std::string> v  = {"a", "b", "c"};
      auto                     ev = v | etl::views::enumerate;

      auto it = ev.begin();
      CHECK_EQUAL(0U, etl::get<0>(*it));
      CHECK_EQUAL(std::string("a"), etl::get<1>(*it));
      ++it;
      CHECK_EQUAL(1U, etl::get<0>(*it));
      CHECK_EQUAL(std::string("b"), etl::get<1>(*it));
      ++it;
      CHECK_EQUAL(2U, etl::get<0>(*it));
      CHECK_EQUAL(std::string("c"), etl::get<1>(*it));
      ++it;
      CHECK(it == ev.end());
    }

    //*************************************************************************
    TEST(test_ranges_enumerate_view_empty)
    {
      std::vector<int> v;
      auto             ev = v | etl::views::enumerate;

      CHECK(ev.begin() == ev.end());
      CHECK_EQUAL(0U, ev.size());
    }

    //*************************************************************************
    TEST(test_ranges_enumerate_view_size)
    {
      std::vector<int> v  = {1, 2, 3, 4, 5};
      auto             ev = v | etl::views::enumerate;

      CHECK_EQUAL(5U, ev.size());
    }

    //*************************************************************************
    TEST(test_ranges_enumerate_view_range_for)
    {
      std::vector<int> v  = {100, 200, 300};
      auto             ev = v | etl::views::enumerate;

      size_t expected_index    = 0;
      int    expected_values[] = {100, 200, 300};

      for (auto&& [idx, val] : ev)
      {
        CHECK_EQUAL(expected_index, idx);
        CHECK_EQUAL(expected_values[expected_index], val);
        ++expected_index;
      }

      CHECK_EQUAL(3U, expected_index);
    }

    //*************************************************************************
    TEST(test_ranges_enumerate_view_chained_with_take)
    {
      std::vector<int> v  = {10, 20, 30, 40, 50};
      auto             ev = v | etl::views::enumerate | etl::views::take(3);

      auto it = ev.begin();
      CHECK_EQUAL(0U, etl::get<0>(*it));
      CHECK_EQUAL(10, etl::get<1>(*it));
      ++it;
      CHECK_EQUAL(1U, etl::get<0>(*it));
      CHECK_EQUAL(20, etl::get<1>(*it));
      ++it;
      CHECK_EQUAL(2U, etl::get<0>(*it));
      CHECK_EQUAL(30, etl::get<1>(*it));
      ++it;
      CHECK(it == ev.end());
    }

    //*************************************************************************
    TEST(test_ranges_enumerate_view_single_element)
    {
      std::vector<int> v  = {42};
      auto             ev = v | etl::views::enumerate;

      auto it = ev.begin();
      CHECK_EQUAL(0U, etl::get<0>(*it));
      CHECK_EQUAL(42, etl::get<1>(*it));
      ++it;
      CHECK(it == ev.end());
    }

    //*************************************************************************
    TEST(test_ranges_zip_view_basic)
    {
      std::vector<int>         v1 = {1, 2, 3};
      std::vector<std::string> v2 = {"a", "b", "c"};

      auto zv = etl::views::zip(v1, v2);

      auto it = zv.begin();
      CHECK_EQUAL(1, etl::get<0>(*it));
      CHECK_EQUAL(std::string("a"), etl::get<1>(*it));
      ++it;
      CHECK_EQUAL(2, etl::get<0>(*it));
      CHECK_EQUAL(std::string("b"), etl::get<1>(*it));
      ++it;
      CHECK_EQUAL(3, etl::get<0>(*it));
      CHECK_EQUAL(std::string("c"), etl::get<1>(*it));
      ++it;
      CHECK(it == zv.end());
    }

    //*************************************************************************
    TEST(test_ranges_zip_view_different_lengths)
    {
      std::vector<int>    v1 = {1, 2, 3, 4, 5};
      std::vector<double> v2 = {10.0, 20.0, 30.0};

      auto zv = etl::views::zip(v1, v2);

      CHECK_EQUAL(3U, zv.size());

      auto it = zv.begin();
      CHECK_EQUAL(1, etl::get<0>(*it));
      CHECK_CLOSE(10.0, etl::get<1>(*it), 0.01);
      ++it;
      CHECK_EQUAL(2, etl::get<0>(*it));
      CHECK_CLOSE(20.0, etl::get<1>(*it), 0.01);
      ++it;
      CHECK_EQUAL(3, etl::get<0>(*it));
      CHECK_CLOSE(30.0, etl::get<1>(*it), 0.01);
      ++it;
      CHECK(it == zv.end());
    }

    //*************************************************************************
    TEST(test_ranges_zip_view_three_ranges)
    {
      std::vector<int>    v1 = {1, 2, 3};
      std::vector<double> v2 = {1.5, 2.5, 3.5};
      std::array<char, 3> v3 = {'x', 'y', 'z'};

      auto zv = etl::views::zip(v1, v2, v3);

      CHECK_EQUAL(3U, zv.size());

      auto it = zv.begin();
      CHECK_EQUAL(1, etl::get<0>(*it));
      CHECK_CLOSE(1.5, etl::get<1>(*it), 0.01);
      CHECK_EQUAL('x', etl::get<2>(*it));
      ++it;
      CHECK_EQUAL(2, etl::get<0>(*it));
      CHECK_CLOSE(2.5, etl::get<1>(*it), 0.01);
      CHECK_EQUAL('y', etl::get<2>(*it));
      ++it;
      CHECK_EQUAL(3, etl::get<0>(*it));
      CHECK_CLOSE(3.5, etl::get<1>(*it), 0.01);
      CHECK_EQUAL('z', etl::get<2>(*it));
      ++it;
      CHECK(it == zv.end());
    }

    //*************************************************************************
    TEST(test_ranges_zip_view_empty)
    {
      std::vector<int>    v1;
      std::vector<double> v2 = {1.0, 2.0};

      auto zv = etl::views::zip(v1, v2);

      CHECK_EQUAL(0U, zv.size());
      CHECK(zv.begin() == zv.end());
    }

    //*************************************************************************
    TEST(test_ranges_zip_view_single_range)
    {
      std::vector<int> v1 = {10, 20, 30};

      auto zv = etl::views::zip(v1);

      CHECK_EQUAL(3U, zv.size());

      auto it = zv.begin();
      CHECK_EQUAL(10, etl::get<0>(*it));
      ++it;
      CHECK_EQUAL(20, etl::get<0>(*it));
      ++it;
      CHECK_EQUAL(30, etl::get<0>(*it));
      ++it;
      CHECK(it == zv.end());
    }

    //*************************************************************************
    TEST(test_ranges_zip_view_range_for)
    {
      std::vector<int> v1 = {1, 2, 3};
      std::vector<int> v2 = {10, 20, 30};

      auto zv = etl::views::zip(v1, v2);

      int    expected_first[]  = {1, 2, 3};
      int    expected_second[] = {10, 20, 30};
      size_t index             = 0;

      for (auto&& [a, b] : zv)
      {
        CHECK_EQUAL(expected_first[index], a);
        CHECK_EQUAL(expected_second[index], b);
        ++index;
      }

      CHECK_EQUAL(3U, index);
    }

    //*************************************************************************
    TEST(test_ranges_zip_view_with_list)
    {
      std::vector<int> v1 = {1, 2, 3, 4};
      std::list<int>   l1 = {100, 200, 300};

      auto zv = etl::views::zip(v1, l1);

      CHECK_EQUAL(3U, zv.size());

      auto it = zv.begin();
      CHECK_EQUAL(1, etl::get<0>(*it));
      CHECK_EQUAL(100, etl::get<1>(*it));
      ++it;
      CHECK_EQUAL(2, etl::get<0>(*it));
      CHECK_EQUAL(200, etl::get<1>(*it));
      ++it;
      CHECK_EQUAL(3, etl::get<0>(*it));
      CHECK_EQUAL(300, etl::get<1>(*it));
      ++it;
      CHECK(it == zv.end());
    }

    //*************************************************************************
    TEST(test_ranges_zip_view_chained_with_take)
    {
      std::vector<int> v1 = {1, 2, 3, 4, 5};
      std::vector<int> v2 = {10, 20, 30, 40, 50};

      auto zv = etl::views::zip(v1, v2) | etl::views::take(2);

      auto it = zv.begin();
      CHECK_EQUAL(1, etl::get<0>(*it));
      CHECK_EQUAL(10, etl::get<1>(*it));
      ++it;
      CHECK_EQUAL(2, etl::get<0>(*it));
      CHECK_EQUAL(20, etl::get<1>(*it));
      ++it;
      CHECK(it == zv.end());
    }

    //*************************************************************************
    TEST(test_ranges_zip_transform_view_basic)
    {
      std::vector<int> v1 = {1, 2, 3};
      std::vector<int> v2 = {10, 20, 30};

      auto ztv = etl::views::zip_transform([](int a, int b) { return a + b; }, v1, v2);

      auto it = ztv.begin();
      CHECK_EQUAL(11, *it);
      ++it;
      CHECK_EQUAL(22, *it);
      ++it;
      CHECK_EQUAL(33, *it);
      ++it;
      CHECK(it == ztv.end());
    }

    //*************************************************************************
    TEST(test_ranges_zip_transform_view_different_lengths)
    {
      std::vector<int>    v1 = {1, 2, 3, 4, 5};
      std::vector<double> v2 = {10.5, 20.5, 30.5};

      auto ztv = etl::views::zip_transform([](int a, double b) { return a + b; }, v1, v2);

      CHECK_EQUAL(3U, ztv.size());

      auto it = ztv.begin();
      CHECK_CLOSE(11.5, *it, 0.01);
      ++it;
      CHECK_CLOSE(22.5, *it, 0.01);
      ++it;
      CHECK_CLOSE(33.5, *it, 0.01);
      ++it;
      CHECK(it == ztv.end());
    }

    //*************************************************************************
    TEST(test_ranges_zip_transform_view_three_ranges)
    {
      std::vector<int> v1 = {1, 2, 3};
      std::vector<int> v2 = {10, 20, 30};
      std::vector<int> v3 = {100, 200, 300};

      auto ztv = etl::views::zip_transform([](int a, int b, int c) { return a + b + c; }, v1, v2, v3);

      CHECK_EQUAL(3U, ztv.size());

      auto it = ztv.begin();
      CHECK_EQUAL(111, *it);
      ++it;
      CHECK_EQUAL(222, *it);
      ++it;
      CHECK_EQUAL(333, *it);
      ++it;
      CHECK(it == ztv.end());
    }

    //*************************************************************************
    TEST(test_ranges_zip_transform_view_empty)
    {
      std::vector<int> v1;
      std::vector<int> v2 = {1, 2, 3};

      auto ztv = etl::views::zip_transform([](int a, int b) { return a * b; }, v1, v2);

      CHECK_EQUAL(0U, ztv.size());
      CHECK(ztv.begin() == ztv.end());
    }

    //*************************************************************************
    TEST(test_ranges_zip_transform_view_single_range)
    {
      std::vector<int> v1 = {10, 20, 30};

      auto ztv = etl::views::zip_transform([](int a) { return a * 2; }, v1);

      CHECK_EQUAL(3U, ztv.size());

      auto it = ztv.begin();
      CHECK_EQUAL(20, *it);
      ++it;
      CHECK_EQUAL(40, *it);
      ++it;
      CHECK_EQUAL(60, *it);
      ++it;
      CHECK(it == ztv.end());
    }

    //*************************************************************************
    TEST(test_ranges_zip_transform_view_range_for)
    {
      std::vector<int> v1 = {1, 2, 3};
      std::vector<int> v2 = {10, 20, 30};

      auto ztv = etl::views::zip_transform([](int a, int b) { return a * b; }, v1, v2);

      int    expected[] = {10, 40, 90};
      size_t index      = 0;

      for (auto val : ztv)
      {
        CHECK_EQUAL(expected[index], val);
        ++index;
      }

      CHECK_EQUAL(3U, index);
    }

    //*************************************************************************
    TEST(test_ranges_zip_transform_view_with_list)
    {
      std::vector<int> v1 = {1, 2, 3, 4};
      std::list<int>   l1 = {100, 200, 300};

      auto ztv = etl::views::zip_transform([](int a, int b) { return a + b; }, v1, l1);

      CHECK_EQUAL(3U, ztv.size());

      auto it = ztv.begin();
      CHECK_EQUAL(101, *it);
      ++it;
      CHECK_EQUAL(202, *it);
      ++it;
      CHECK_EQUAL(303, *it);
      ++it;
      CHECK(it == ztv.end());
    }

    //*************************************************************************
    TEST(test_ranges_zip_transform_view_chained_with_take)
    {
      std::vector<int> v1 = {1, 2, 3, 4, 5};
      std::vector<int> v2 = {10, 20, 30, 40, 50};

      auto ztv = etl::views::zip_transform([](int a, int b) { return a + b; }, v1, v2) | etl::views::take(2);

      auto it = ztv.begin();
      CHECK_EQUAL(11, *it);
      ++it;
      CHECK_EQUAL(22, *it);
      ++it;
      CHECK(it == ztv.end());
    }

    //*************************************************************************
    TEST(test_ranges_zip_transform_view_string_concat)
    {
      std::vector<std::string> v1 = {"hello", "good", "nice"};
      std::vector<std::string> v2 = {" world", " morning", " day"};

      auto ztv = etl::views::zip_transform([](const std::string& a, const std::string& b) { return a + b; }, v1, v2);

      auto it = ztv.begin();
      CHECK_EQUAL(std::string("hello world"), *it);
      ++it;
      CHECK_EQUAL(std::string("good morning"), *it);
      ++it;
      CHECK_EQUAL(std::string("nice day"), *it);
      ++it;
      CHECK(it == ztv.end());
    }

    //*************************************************************************
    TEST(test_ranges_zip_transform_view_returns_different_type)
    {
      std::vector<int> v1 = {1, 2, 3};
      std::vector<int> v2 = {4, 5, 6};

      auto ztv = etl::views::zip_transform([](int a, int b) -> double { return static_cast<double>(a) / b; }, v1, v2);

      auto it = ztv.begin();
      CHECK_CLOSE(0.25, *it, 0.01);
      ++it;
      CHECK_CLOSE(0.4, *it, 0.01);
      ++it;
      CHECK_CLOSE(0.5, *it, 0.01);
      ++it;
      CHECK(it == ztv.end());
    }

    //*************************************************************************
    TEST(test_ranges_adjacent_view_basic)
    {
      std::vector<int> v = {1, 2, 3, 4, 5};

      auto av = etl::views::adjacent<2>(v);

      CHECK_EQUAL(4U, av.size());

      auto it = av.begin();
      CHECK_EQUAL(1, etl::get<0>(*it));
      CHECK_EQUAL(2, etl::get<1>(*it));
      ++it;
      CHECK_EQUAL(2, etl::get<0>(*it));
      CHECK_EQUAL(3, etl::get<1>(*it));
      ++it;
      CHECK_EQUAL(3, etl::get<0>(*it));
      CHECK_EQUAL(4, etl::get<1>(*it));
      ++it;
      CHECK_EQUAL(4, etl::get<0>(*it));
      CHECK_EQUAL(5, etl::get<1>(*it));
      ++it;
      CHECK(it == av.end());
    }

    //*************************************************************************
    TEST(test_ranges_adjacent_view_triple)
    {
      std::vector<int> v = {10, 20, 30, 40, 50};

      auto av = etl::views::adjacent<3>(v);

      CHECK_EQUAL(3U, av.size());

      auto it = av.begin();
      CHECK_EQUAL(10, etl::get<0>(*it));
      CHECK_EQUAL(20, etl::get<1>(*it));
      CHECK_EQUAL(30, etl::get<2>(*it));
      ++it;
      CHECK_EQUAL(20, etl::get<0>(*it));
      CHECK_EQUAL(30, etl::get<1>(*it));
      CHECK_EQUAL(40, etl::get<2>(*it));
      ++it;
      CHECK_EQUAL(30, etl::get<0>(*it));
      CHECK_EQUAL(40, etl::get<1>(*it));
      CHECK_EQUAL(50, etl::get<2>(*it));
      ++it;
      CHECK(it == av.end());
    }

    //*************************************************************************
    TEST(test_ranges_adjacent_view_single_window)
    {
      std::vector<int> v = {1};

      auto av = etl::views::adjacent<1>(v);

      CHECK_EQUAL(1U, av.size());

      auto it = av.begin();
      CHECK_EQUAL(1, etl::get<0>(*it));
      ++it;
      CHECK(it == av.end());
    }

    //*************************************************************************
    TEST(test_ranges_adjacent_view_window_equals_range_size)
    {
      std::vector<int> v = {1, 2, 3};

      auto av = etl::views::adjacent<3>(v);

      CHECK_EQUAL(1U, av.size());

      auto it = av.begin();
      CHECK_EQUAL(1, etl::get<0>(*it));
      CHECK_EQUAL(2, etl::get<1>(*it));
      CHECK_EQUAL(3, etl::get<2>(*it));
      ++it;
      CHECK(it == av.end());
    }

    //*************************************************************************
    TEST(test_ranges_adjacent_view_window_larger_than_range)
    {
      std::vector<int> v = {1, 2};

      auto av = etl::views::adjacent<5>(v);

      CHECK_EQUAL(0U, av.size());
      CHECK(av.begin() == av.end());
    }

    //*************************************************************************
    TEST(test_ranges_adjacent_view_empty_range)
    {
      std::vector<int> v;

      auto av = etl::views::adjacent<2>(v);

      CHECK_EQUAL(0U, av.size());
      CHECK(av.begin() == av.end());
    }

    //*************************************************************************
    TEST(test_ranges_adjacent_view_range_for)
    {
      std::vector<int> v = {1, 2, 3, 4};

      auto av = etl::views::adjacent<2>(v);

      std::vector<int> firsts;
      std::vector<int> seconds;

      for (auto [a, b] : av)
      {
        firsts.push_back(a);
        seconds.push_back(b);
      }

      CHECK_EQUAL(3U, firsts.size());
      CHECK_EQUAL(1, firsts[0]);
      CHECK_EQUAL(2, firsts[1]);
      CHECK_EQUAL(3, firsts[2]);
      CHECK_EQUAL(2, seconds[0]);
      CHECK_EQUAL(3, seconds[1]);
      CHECK_EQUAL(4, seconds[2]);
    }

    //*************************************************************************
    TEST(test_ranges_adjacent_view_pairwise)
    {
      std::vector<int> v = {1, 2, 3, 4};

      auto av = etl::views::pairwise(v);

      CHECK_EQUAL(3U, av.size());

      auto it = av.begin();
      CHECK_EQUAL(1, etl::get<0>(*it));
      CHECK_EQUAL(2, etl::get<1>(*it));
      ++it;
      CHECK_EQUAL(2, etl::get<0>(*it));
      CHECK_EQUAL(3, etl::get<1>(*it));
      ++it;
      CHECK_EQUAL(3, etl::get<0>(*it));
      CHECK_EQUAL(4, etl::get<1>(*it));
      ++it;
      CHECK(it == av.end());
    }

    //*************************************************************************
    TEST(test_ranges_adjacent_view_pipe)
    {
      std::vector<int> v = {10, 20, 30, 40, 50};

      auto av = v | etl::views::adjacent<2>();

      CHECK_EQUAL(4U, av.size());

      auto it = av.begin();
      CHECK_EQUAL(10, etl::get<0>(*it));
      CHECK_EQUAL(20, etl::get<1>(*it));
      ++it;
      CHECK_EQUAL(20, etl::get<0>(*it));
      CHECK_EQUAL(30, etl::get<1>(*it));
      ++it;
      CHECK_EQUAL(30, etl::get<0>(*it));
      CHECK_EQUAL(40, etl::get<1>(*it));
      ++it;
      CHECK_EQUAL(40, etl::get<0>(*it));
      CHECK_EQUAL(50, etl::get<1>(*it));
      ++it;
      CHECK(it == av.end());
    }

    //*************************************************************************
    TEST(test_ranges_adjacent_view_chained_with_take)
    {
      std::vector<int> v = {1, 2, 3, 4, 5};

      auto av = v | etl::views::adjacent<2>() | etl::views::take(2);

      auto it = av.begin();
      CHECK_EQUAL(1, etl::get<0>(*it));
      CHECK_EQUAL(2, etl::get<1>(*it));
      ++it;
      CHECK_EQUAL(2, etl::get<0>(*it));
      CHECK_EQUAL(3, etl::get<1>(*it));
      ++it;
      CHECK(it == av.end());
    }

    //*************************************************************************
    TEST(test_ranges_adjacent_view_with_strings)
    {
      std::vector<std::string> v = {"a", "b", "c", "d"};

      auto av = etl::views::adjacent<2>(v);

      CHECK_EQUAL(3U, av.size());

      auto it = av.begin();
      CHECK_EQUAL(std::string("a"), etl::get<0>(*it));
      CHECK_EQUAL(std::string("b"), etl::get<1>(*it));
      ++it;
      CHECK_EQUAL(std::string("b"), etl::get<0>(*it));
      CHECK_EQUAL(std::string("c"), etl::get<1>(*it));
      ++it;
      CHECK_EQUAL(std::string("c"), etl::get<0>(*it));
      CHECK_EQUAL(std::string("d"), etl::get<1>(*it));
      ++it;
      CHECK(it == av.end());
    }

    //*************************************************************************
    TEST(test_ranges_adjacent_view_quad)
    {
      std::vector<int> v = {1, 2, 3, 4, 5, 6};

      auto av = etl::views::adjacent<4>(v);

      CHECK_EQUAL(3U, av.size());

      auto it = av.begin();
      CHECK_EQUAL(1, etl::get<0>(*it));
      CHECK_EQUAL(2, etl::get<1>(*it));
      CHECK_EQUAL(3, etl::get<2>(*it));
      CHECK_EQUAL(4, etl::get<3>(*it));
      ++it;
      CHECK_EQUAL(2, etl::get<0>(*it));
      CHECK_EQUAL(3, etl::get<1>(*it));
      CHECK_EQUAL(4, etl::get<2>(*it));
      CHECK_EQUAL(5, etl::get<3>(*it));
      ++it;
      CHECK_EQUAL(3, etl::get<0>(*it));
      CHECK_EQUAL(4, etl::get<1>(*it));
      CHECK_EQUAL(5, etl::get<2>(*it));
      CHECK_EQUAL(6, etl::get<3>(*it));
      ++it;
      CHECK(it == av.end());
    }

    //*************************************************************************
    TEST(test_ranges_adjacent_transform_view_basic_sum)
    {
      std::vector<int> v = {1, 2, 3, 4, 5};

      auto atv = etl::views::adjacent_transform<2>(v, [](int a, int b) { return a + b; });

      CHECK_EQUAL(4U, atv.size());

      auto it = atv.begin();
      CHECK_EQUAL(3, *it); // 1+2
      ++it;
      CHECK_EQUAL(5, *it); // 2+3
      ++it;
      CHECK_EQUAL(7, *it); // 3+4
      ++it;
      CHECK_EQUAL(9, *it); // 4+5
      ++it;
      CHECK(it == atv.end());
    }

    //*************************************************************************
    TEST(test_ranges_adjacent_transform_view_triple_product)
    {
      std::vector<int> v = {1, 2, 3, 4, 5};

      auto atv = etl::views::adjacent_transform<3>(v, [](int a, int b, int c) { return a * b * c; });

      CHECK_EQUAL(3U, atv.size());

      auto it = atv.begin();
      CHECK_EQUAL(6, *it); // 1*2*3
      ++it;
      CHECK_EQUAL(24, *it); // 2*3*4
      ++it;
      CHECK_EQUAL(60, *it); // 3*4*5
      ++it;
      CHECK(it == atv.end());
    }

    //*************************************************************************
    TEST(test_ranges_adjacent_transform_view_single_window)
    {
      std::vector<int> v = {10, 20, 30};

      // Window size 1: function applied to each element individually
      auto atv = etl::views::adjacent_transform<1>(v, [](int a) { return a * 2; });

      CHECK_EQUAL(3U, atv.size());

      auto it = atv.begin();
      CHECK_EQUAL(20, *it);
      ++it;
      CHECK_EQUAL(40, *it);
      ++it;
      CHECK_EQUAL(60, *it);
      ++it;
      CHECK(it == atv.end());
    }

    //*************************************************************************
    TEST(test_ranges_adjacent_transform_view_window_equals_range_size)
    {
      std::vector<int> v = {1, 2, 3};

      auto atv = etl::views::adjacent_transform<3>(v, [](int a, int b, int c) { return a + b + c; });

      CHECK_EQUAL(1U, atv.size());

      auto it = atv.begin();
      CHECK_EQUAL(6, *it); // 1+2+3
      ++it;
      CHECK(it == atv.end());
    }

    //*************************************************************************
    TEST(test_ranges_adjacent_transform_view_window_larger_than_range)
    {
      std::vector<int> v = {1, 2};

      auto atv = etl::views::adjacent_transform<3>(v, [](int a, int b, int c) { return a + b + c; });

      CHECK_EQUAL(0U, atv.size());
      CHECK(atv.begin() == atv.end());
    }

    //*************************************************************************
    TEST(test_ranges_adjacent_transform_view_empty_range)
    {
      std::vector<int> v = {};

      auto atv = etl::views::adjacent_transform<2>(v, [](int a, int b) { return a + b; });

      CHECK_EQUAL(0U, atv.size());
      CHECK(atv.begin() == atv.end());
    }

    //*************************************************************************
    TEST(test_ranges_adjacent_transform_view_range_for)
    {
      std::vector<int> v        = {1, 2, 3, 4, 5};
      std::vector<int> expected = {3, 5, 7, 9};

      auto atv = etl::views::adjacent_transform<2>(v, [](int a, int b) { return a + b; });

      std::vector<int> result;
      for (auto val : atv)
      {
        result.push_back(val);
      }

      CHECK_EQUAL(expected.size(), result.size());
      for (size_t i = 0; i < expected.size(); ++i)
      {
        CHECK_EQUAL(expected[i], result[i]);
      }
    }

    //*************************************************************************
    TEST(test_ranges_adjacent_transform_view_pairwise_transform)
    {
      std::vector<int> v = {10, 20, 30, 40};

      auto atv = etl::views::pairwise_transform(v, [](int a, int b) { return b - a; });

      CHECK_EQUAL(3U, atv.size());

      auto it = atv.begin();
      CHECK_EQUAL(10, *it); // 20-10
      ++it;
      CHECK_EQUAL(10, *it); // 30-20
      ++it;
      CHECK_EQUAL(10, *it); // 40-30
      ++it;
      CHECK(it == atv.end());
    }

    //*************************************************************************
    TEST(test_ranges_adjacent_transform_view_pipe)
    {
      std::vector<int> v = {1, 2, 3, 4, 5};

      auto sum = [](int a, int b)
      {
        return a + b;
      };
      auto atv = v | etl::views::adjacent_transform<2>(sum);

      CHECK_EQUAL(4U, atv.size());

      auto it = atv.begin();
      CHECK_EQUAL(3, *it); // 1+2
      ++it;
      CHECK_EQUAL(5, *it); // 2+3
      ++it;
      CHECK_EQUAL(7, *it); // 3+4
      ++it;
      CHECK_EQUAL(9, *it); // 4+5
      ++it;
      CHECK(it == atv.end());
    }

    //*************************************************************************
    TEST(test_ranges_adjacent_transform_view_chained_with_take)
    {
      std::vector<int> v = {1, 2, 3, 4, 5};

      auto sum = [](int a, int b)
      {
        return a + b;
      };
      auto atv = v | etl::views::adjacent_transform<2>(sum) | etl::views::take(2);

      auto it = atv.begin();
      CHECK_EQUAL(3, *it); // 1+2
      ++it;
      CHECK_EQUAL(5, *it); // 2+3
      ++it;
      CHECK(it == atv.end());
    }

    //*************************************************************************
    TEST(test_ranges_adjacent_transform_view_different_return_type)
    {
      std::vector<int> v = {1, 2, 3, 4};

      // Returns double instead of int
      auto avg = [](int a, int b)
      {
        return (a + b) / 2.0;
      };
      auto atv = etl::views::adjacent_transform<2>(v, avg);

      CHECK_EQUAL(3U, atv.size());

      auto it = atv.begin();
      CHECK_CLOSE(1.5, *it, 0.001); // (1+2)/2.0
      ++it;
      CHECK_CLOSE(2.5, *it, 0.001); // (2+3)/2.0
      ++it;
      CHECK_CLOSE(3.5, *it, 0.001); // (3+4)/2.0
      ++it;
      CHECK(it == atv.end());
    }

    //*************************************************************************
    TEST(test_ranges_adjacent_transform_view_pairwise_transform_pipe)
    {
      std::vector<int> v = {5, 10, 15, 20};

      auto diff = [](int a, int b)
      {
        return b - a;
      };
      auto atv = v | etl::views::pairwise_transform(diff);

      CHECK_EQUAL(3U, atv.size());

      auto it = atv.begin();
      CHECK_EQUAL(5, *it);
      ++it;
      CHECK_EQUAL(5, *it);
      ++it;
      CHECK_EQUAL(5, *it);
      ++it;
      CHECK(it == atv.end());
    }

    //*************************************************************************
    TEST(test_ranges_chunk_view_basic)
    {
      std::vector<int> v  = {1, 2, 3, 4, 5, 6, 7, 8, 9};
      auto             cv = etl::ranges::chunk_view(v, 3);

      std::vector<std::vector<int>> expected{{1, 2, 3}, {4, 5, 6}, {7, 8, 9}};
      size_t                        idx = 0;
      for (auto chunk : cv)
      {
        std::vector<int> actual(chunk.begin(), chunk.end());
        CHECK_EQUAL(expected[idx], actual);
        ++idx;
      }
      CHECK_EQUAL(expected.size(), idx);
    }

    //*************************************************************************
    TEST(test_ranges_chunk_view_remainder)
    {
      // Range size not evenly divisible by chunk size
      std::vector<int> v  = {1, 2, 3, 4, 5, 6, 7};
      auto             cv = etl::ranges::chunk_view(v, 3);

      std::vector<std::vector<int>> expected{{1, 2, 3}, {4, 5, 6}, {7}};
      size_t                        idx = 0;
      for (auto chunk : cv)
      {
        std::vector<int> actual(chunk.begin(), chunk.end());
        CHECK_EQUAL(expected[idx], actual);
        ++idx;
      }
      CHECK_EQUAL(expected.size(), idx);
    }

    //*************************************************************************
    TEST(test_ranges_chunk_view_single_element_chunks)
    {
      std::vector<int> v  = {10, 20, 30};
      auto             cv = etl::ranges::chunk_view(v, 1);

      std::vector<std::vector<int>> expected{{10}, {20}, {30}};
      size_t                        idx = 0;
      for (auto chunk : cv)
      {
        std::vector<int> actual(chunk.begin(), chunk.end());
        CHECK_EQUAL(expected[idx], actual);
        ++idx;
      }
      CHECK_EQUAL(expected.size(), idx);
    }

    //*************************************************************************
    TEST(test_ranges_chunk_view_chunk_larger_than_range)
    {
      std::vector<int> v  = {1, 2, 3};
      auto             cv = etl::ranges::chunk_view(v, 10);

      size_t count = 0;
      for (auto chunk : cv)
      {
        std::vector<int> actual(chunk.begin(), chunk.end());
        std::vector<int> expected{1, 2, 3};
        CHECK_EQUAL(expected, actual);
        ++count;
      }
      CHECK_EQUAL(1U, count);
    }

    //*************************************************************************
    TEST(test_ranges_chunk_view_empty_range)
    {
      std::vector<int> v;
      auto             cv = etl::ranges::chunk_view(v, 3);

      size_t count = 0;
      for (auto chunk : cv)
      {
        (void)chunk;
        ++count;
      }
      CHECK_EQUAL(0U, count);
    }

    //*************************************************************************
    TEST(test_ranges_chunk_view_pipe)
    {
      std::vector<int> v  = {1, 2, 3, 4, 5, 6};
      auto             cv = v | etl::views::chunk(2);

      std::vector<std::vector<int>> expected{{1, 2}, {3, 4}, {5, 6}};
      size_t                        idx = 0;
      for (auto chunk : cv)
      {
        std::vector<int> actual(chunk.begin(), chunk.end());
        CHECK_EQUAL(expected[idx], actual);
        ++idx;
      }
      CHECK_EQUAL(expected.size(), idx);
    }

    //*************************************************************************
    TEST(test_ranges_chunk_view_pipe_with_remainder)
    {
      std::vector<int> v  = {1, 2, 3, 4, 5};
      auto             cv = v | etl::views::chunk(2);

      std::vector<std::vector<int>> expected{{1, 2}, {3, 4}, {5}};
      size_t                        idx = 0;
      for (auto chunk : cv)
      {
        std::vector<int> actual(chunk.begin(), chunk.end());
        CHECK_EQUAL(expected[idx], actual);
        ++idx;
      }
      CHECK_EQUAL(expected.size(), idx);
    }

    //*************************************************************************
    TEST(test_ranges_chunk_view_functional_call)
    {
      std::vector<int> v  = {1, 2, 3, 4, 5, 6};
      auto             cv = etl::views::chunk(v, 3);

      std::vector<std::vector<int>> expected{{1, 2, 3}, {4, 5, 6}};
      size_t                        idx = 0;
      for (auto chunk : cv)
      {
        std::vector<int> actual(chunk.begin(), chunk.end());
        CHECK_EQUAL(expected[idx], actual);
        ++idx;
      }
      CHECK_EQUAL(expected.size(), idx);
    }

    //*************************************************************************
    TEST(test_ranges_chunk_view_etl_vector)
    {
      etl::vector<int, 10> v  = {1, 2, 3, 4, 5, 6, 7, 8};
      auto                 cv = etl::ranges::chunk_view(v, 3);

      std::vector<std::vector<int>> expected{{1, 2, 3}, {4, 5, 6}, {7, 8}};
      size_t                        idx = 0;
      for (auto chunk : cv)
      {
        std::vector<int> actual(chunk.begin(), chunk.end());
        CHECK_EQUAL(expected[idx], actual);
        ++idx;
      }
      CHECK_EQUAL(expected.size(), idx);
    }

    //*************************************************************************
    TEST(test_ranges_chunk_view_array)
    {
      std::array<int, 6> a  = {10, 20, 30, 40, 50, 60};
      auto               cv = etl::ranges::chunk_view(a, 2);

      std::vector<std::vector<int>> expected{{10, 20}, {30, 40}, {50, 60}};
      size_t                        idx = 0;
      for (auto chunk : cv)
      {
        std::vector<int> actual(chunk.begin(), chunk.end());
        CHECK_EQUAL(expected[idx], actual);
        ++idx;
      }
      CHECK_EQUAL(expected.size(), idx);
    }

    //*************************************************************************
    TEST(test_ranges_chunk_view_pipe_chain_take_chunk)
    {
      // Take first 6, then chunk by 2
      std::vector<int> v  = {1, 2, 3, 4, 5, 6, 7, 8, 9};
      auto             cv = v | etl::views::take(6) | etl::views::chunk(2);

      std::vector<std::vector<int>> expected{{1, 2}, {3, 4}, {5, 6}};
      size_t                        idx = 0;
      for (auto chunk : cv)
      {
        std::vector<int> actual(chunk.begin(), chunk.end());
        CHECK_EQUAL(expected[idx], actual);
        ++idx;
      }
      CHECK_EQUAL(expected.size(), idx);
    }

    //*************************************************************************
    TEST(test_ranges_chunk_view_pipe_chain_drop_chunk)
    {
      // Drop first 2, then chunk by 3
      std::vector<int> v  = {1, 2, 3, 4, 5, 6, 7, 8};
      auto             cv = v | etl::views::drop(2) | etl::views::chunk(3);

      std::vector<std::vector<int>> expected{{3, 4, 5}, {6, 7, 8}};
      size_t                        idx = 0;
      for (auto chunk : cv)
      {
        std::vector<int> actual(chunk.begin(), chunk.end());
        CHECK_EQUAL(expected[idx], actual);
        ++idx;
      }
      CHECK_EQUAL(expected.size(), idx);
    }

    //*************************************************************************
    // slide_view tests
    //*************************************************************************

    //*************************************************************************
    TEST(test_ranges_slide_view_basic)
    {
      std::vector<int> v  = {1, 2, 3, 4, 5};
      auto             sv = etl::ranges::slide_view(v, 3);

      std::vector<std::vector<int>> expected{{1, 2, 3}, {2, 3, 4}, {3, 4, 5}};
      size_t                        idx = 0;
      for (auto window : sv)
      {
        std::vector<int> actual(window.begin(), window.end());
        CHECK_EQUAL(expected[idx], actual);
        ++idx;
      }
      CHECK_EQUAL(expected.size(), idx);
    }

    //*************************************************************************
    TEST(test_ranges_slide_view_window_size_1)
    {
      std::vector<int> v  = {10, 20, 30};
      auto             sv = etl::ranges::slide_view(v, 1);

      std::vector<std::vector<int>> expected{{10}, {20}, {30}};
      size_t                        idx = 0;
      for (auto window : sv)
      {
        std::vector<int> actual(window.begin(), window.end());
        CHECK_EQUAL(expected[idx], actual);
        ++idx;
      }
      CHECK_EQUAL(expected.size(), idx);
    }

    //*************************************************************************
    TEST(test_ranges_slide_view_window_equals_range)
    {
      std::vector<int> v  = {1, 2, 3};
      auto             sv = etl::ranges::slide_view(v, 3);

      size_t count = 0;
      for (auto window : sv)
      {
        std::vector<int> actual(window.begin(), window.end());
        std::vector<int> expected{1, 2, 3};
        CHECK_EQUAL(expected, actual);
        ++count;
      }
      CHECK_EQUAL(1U, count);
    }

    //*************************************************************************
    TEST(test_ranges_slide_view_window_larger_than_range)
    {
      std::vector<int> v  = {1, 2, 3};
      auto             sv = etl::ranges::slide_view(v, 5);

      size_t count = 0;
      for (auto window : sv)
      {
        (void)window;
        ++count;
      }
      CHECK_EQUAL(0U, count);
    }

    //*************************************************************************
    TEST(test_ranges_slide_view_empty_range)
    {
      std::vector<int> v;
      auto             sv = etl::ranges::slide_view(v, 3);

      size_t count = 0;
      for (auto window : sv)
      {
        (void)window;
        ++count;
      }
      CHECK_EQUAL(0U, count);
    }

    //*************************************************************************
    TEST(test_ranges_slide_view_size)
    {
      std::vector<int> v = {1, 2, 3, 4, 5};

      CHECK_EQUAL(3U, etl::ranges::slide_view(v, 3).size());
      CHECK_EQUAL(5U, etl::ranges::slide_view(v, 1).size());
      CHECK_EQUAL(1U, etl::ranges::slide_view(v, 5).size());
      CHECK_EQUAL(0U, etl::ranges::slide_view(v, 6).size());
    }

    //*************************************************************************
    TEST(test_ranges_slide_view_pipe)
    {
      std::vector<int> v  = {1, 2, 3, 4, 5};
      auto             sv = v | etl::views::slide(3);

      std::vector<std::vector<int>> expected{{1, 2, 3}, {2, 3, 4}, {3, 4, 5}};
      size_t                        idx = 0;
      for (auto window : sv)
      {
        std::vector<int> actual(window.begin(), window.end());
        CHECK_EQUAL(expected[idx], actual);
        ++idx;
      }
      CHECK_EQUAL(expected.size(), idx);
    }

    //*************************************************************************
    TEST(test_ranges_slide_view_functional_call)
    {
      std::vector<int> v  = {1, 2, 3, 4, 5};
      auto             sv = etl::views::slide(v, 2);

      std::vector<std::vector<int>> expected{{1, 2}, {2, 3}, {3, 4}, {4, 5}};
      size_t                        idx = 0;
      for (auto window : sv)
      {
        std::vector<int> actual(window.begin(), window.end());
        CHECK_EQUAL(expected[idx], actual);
        ++idx;
      }
      CHECK_EQUAL(expected.size(), idx);
    }

    //*************************************************************************
    TEST(test_ranges_slide_view_etl_vector)
    {
      etl::vector<int, 10> v  = {1, 2, 3, 4, 5, 6};
      auto                 sv = etl::ranges::slide_view(v, 4);

      std::vector<std::vector<int>> expected{{1, 2, 3, 4}, {2, 3, 4, 5}, {3, 4, 5, 6}};
      size_t                        idx = 0;
      for (auto window : sv)
      {
        std::vector<int> actual(window.begin(), window.end());
        CHECK_EQUAL(expected[idx], actual);
        ++idx;
      }
      CHECK_EQUAL(expected.size(), idx);
    }

    //*************************************************************************
    TEST(test_ranges_slide_view_array)
    {
      std::array<int, 5> a  = {10, 20, 30, 40, 50};
      auto               sv = etl::ranges::slide_view(a, 2);

      std::vector<std::vector<int>> expected{{10, 20}, {20, 30}, {30, 40}, {40, 50}};
      size_t                        idx = 0;
      for (auto window : sv)
      {
        std::vector<int> actual(window.begin(), window.end());
        CHECK_EQUAL(expected[idx], actual);
        ++idx;
      }
      CHECK_EQUAL(expected.size(), idx);
    }

    //*************************************************************************
    TEST(test_ranges_slide_view_pipe_chain_take_slide)
    {
      // Take first 5, then slide with window 3
      std::vector<int> v  = {1, 2, 3, 4, 5, 6, 7, 8, 9};
      auto             sv = v | etl::views::take(5) | etl::views::slide(3);

      std::vector<std::vector<int>> expected{{1, 2, 3}, {2, 3, 4}, {3, 4, 5}};
      size_t                        idx = 0;
      for (auto window : sv)
      {
        std::vector<int> actual(window.begin(), window.end());
        CHECK_EQUAL(expected[idx], actual);
        ++idx;
      }
      CHECK_EQUAL(expected.size(), idx);
    }

    //*************************************************************************
    TEST(test_ranges_slide_view_pipe_chain_drop_slide)
    {
      // Drop first 2, then slide with window 3
      std::vector<int> v  = {1, 2, 3, 4, 5, 6, 7};
      auto             sv = v | etl::views::drop(2) | etl::views::slide(3);

      std::vector<std::vector<int>> expected{{3, 4, 5}, {4, 5, 6}, {5, 6, 7}};
      size_t                        idx = 0;
      for (auto window : sv)
      {
        std::vector<int> actual(window.begin(), window.end());
        CHECK_EQUAL(expected[idx], actual);
        ++idx;
      }
      CHECK_EQUAL(expected.size(), idx);
    }

    //*************************************************************************
    // chunk_by_view tests
    //*************************************************************************

    //*************************************************************************
    TEST(test_ranges_chunk_by_view_basic)
    {
      // Group consecutive equal elements
      std::vector<int> v  = {1, 1, 2, 2, 2, 3, 3, 1};
      auto             cv = etl::ranges::chunk_by_view(v, [](int a, int b) { return a == b; });

      std::vector<std::vector<int>> expected{{1, 1}, {2, 2, 2}, {3, 3}, {1}};
      size_t                        idx = 0;
      for (auto chunk : cv)
      {
        std::vector<int> actual(chunk.begin(), chunk.end());
        CHECK_EQUAL(expected[idx], actual);
        ++idx;
      }
      CHECK_EQUAL(expected.size(), idx);
    }

    //*************************************************************************
    TEST(test_ranges_chunk_by_view_less_than)
    {
      // Split where values stop being strictly increasing
      std::vector<int> v  = {1, 2, 3, 1, 2, 1};
      auto             cv = etl::ranges::chunk_by_view(v, [](int a, int b) { return a < b; });

      std::vector<std::vector<int>> expected{{1, 2, 3}, {1, 2}, {1}};
      size_t                        idx = 0;
      for (auto chunk : cv)
      {
        std::vector<int> actual(chunk.begin(), chunk.end());
        CHECK_EQUAL(expected[idx], actual);
        ++idx;
      }
      CHECK_EQUAL(expected.size(), idx);
    }

    //*************************************************************************
    TEST(test_ranges_chunk_by_view_single_element)
    {
      std::vector<int> v  = {42};
      auto             cv = etl::ranges::chunk_by_view(v, [](int a, int b) { return a == b; });

      size_t count = 0;
      for (auto chunk : cv)
      {
        std::vector<int> actual(chunk.begin(), chunk.end());
        std::vector<int> expected{42};
        CHECK_EQUAL(expected, actual);
        ++count;
      }
      CHECK_EQUAL(1U, count);
    }

    //*************************************************************************
    TEST(test_ranges_chunk_by_view_empty_range)
    {
      std::vector<int> v;
      auto             cv = etl::ranges::chunk_by_view(v, [](int a, int b) { return a == b; });

      size_t count = 0;
      for (auto chunk : cv)
      {
        (void)chunk;
        ++count;
      }
      CHECK_EQUAL(0U, count);
    }

    //*************************************************************************
    TEST(test_ranges_chunk_by_view_all_same)
    {
      // All elements satisfy predicate => single chunk
      std::vector<int> v  = {5, 5, 5, 5};
      auto             cv = etl::ranges::chunk_by_view(v, [](int a, int b) { return a == b; });

      size_t count = 0;
      for (auto chunk : cv)
      {
        std::vector<int> actual(chunk.begin(), chunk.end());
        std::vector<int> expected{5, 5, 5, 5};
        CHECK_EQUAL(expected, actual);
        ++count;
      }
      CHECK_EQUAL(1U, count);
    }

    //*************************************************************************
    TEST(test_ranges_chunk_by_view_all_different)
    {
      // No adjacent pair satisfies predicate => each element is its own chunk
      std::vector<int> v  = {1, 2, 1, 2, 1};
      auto             cv = etl::ranges::chunk_by_view(v, [](int a, int b) { return a == b; });

      std::vector<std::vector<int>> expected{{1}, {2}, {1}, {2}, {1}};
      size_t                        idx = 0;
      for (auto chunk : cv)
      {
        std::vector<int> actual(chunk.begin(), chunk.end());
        CHECK_EQUAL(expected[idx], actual);
        ++idx;
      }
      CHECK_EQUAL(expected.size(), idx);
    }

    //*************************************************************************
    TEST(test_ranges_chunk_by_view_pipe)
    {
      std::vector<int> v  = {1, 1, 2, 2, 3};
      auto             cv = v | etl::views::chunk_by([](int a, int b) { return a == b; });

      std::vector<std::vector<int>> expected{{1, 1}, {2, 2}, {3}};
      size_t                        idx = 0;
      for (auto chunk : cv)
      {
        std::vector<int> actual(chunk.begin(), chunk.end());
        CHECK_EQUAL(expected[idx], actual);
        ++idx;
      }
      CHECK_EQUAL(expected.size(), idx);
    }

    //*************************************************************************
    TEST(test_ranges_chunk_by_view_functional_call)
    {
      std::vector<int> v  = {1, 2, 3, 1, 2};
      auto             cv = etl::views::chunk_by(v, [](int a, int b) { return a < b; });

      std::vector<std::vector<int>> expected{{1, 2, 3}, {1, 2}};
      size_t                        idx = 0;
      for (auto chunk : cv)
      {
        std::vector<int> actual(chunk.begin(), chunk.end());
        CHECK_EQUAL(expected[idx], actual);
        ++idx;
      }
      CHECK_EQUAL(expected.size(), idx);
    }

    //*************************************************************************
    TEST(test_ranges_chunk_by_view_etl_vector)
    {
      etl::vector<int, 10> v  = {1, 1, 2, 3, 3};
      auto                 cv = etl::ranges::chunk_by_view(v, [](int a, int b) { return a == b; });

      std::vector<std::vector<int>> expected{{1, 1}, {2}, {3, 3}};
      size_t                        idx = 0;
      for (auto chunk : cv)
      {
        std::vector<int> actual(chunk.begin(), chunk.end());
        CHECK_EQUAL(expected[idx], actual);
        ++idx;
      }
      CHECK_EQUAL(expected.size(), idx);
    }

    //*************************************************************************
    TEST(test_ranges_chunk_by_view_array)
    {
      std::array<int, 6> a  = {1, 1, 2, 2, 3, 3};
      auto               cv = etl::ranges::chunk_by_view(a, [](int x, int y) { return x == y; });

      std::vector<std::vector<int>> expected{{1, 1}, {2, 2}, {3, 3}};
      size_t                        idx = 0;
      for (auto chunk : cv)
      {
        std::vector<int> actual(chunk.begin(), chunk.end());
        CHECK_EQUAL(expected[idx], actual);
        ++idx;
      }
      CHECK_EQUAL(expected.size(), idx);
    }

    //*************************************************************************
    TEST(test_ranges_chunk_by_view_pipe_chain_take_chunk_by)
    {
      // Take first 5, then chunk_by equal
      std::vector<int> v  = {1, 1, 2, 2, 3, 3, 4};
      auto             cv = v | etl::views::take(5) | etl::views::chunk_by([](int a, int b) { return a == b; });

      std::vector<std::vector<int>> expected{{1, 1}, {2, 2}, {3}};
      size_t                        idx = 0;
      for (auto chunk : cv)
      {
        std::vector<int> actual(chunk.begin(), chunk.end());
        CHECK_EQUAL(expected[idx], actual);
        ++idx;
      }
      CHECK_EQUAL(expected.size(), idx);
    }

    //*************************************************************************
    TEST(test_ranges_chunk_by_view_pipe_chain_drop_chunk_by)
    {
      // Drop first 2, then chunk_by strictly increasing
      std::vector<int> v  = {1, 1, 2, 3, 1, 2};
      auto             cv = v | etl::views::drop(2) | etl::views::chunk_by([](int a, int b) { return a < b; });

      std::vector<std::vector<int>> expected{{2, 3}, {1, 2}};
      size_t                        idx = 0;
      for (auto chunk : cv)
      {
        std::vector<int> actual(chunk.begin(), chunk.end());
        CHECK_EQUAL(expected[idx], actual);
        ++idx;
      }
      CHECK_EQUAL(expected.size(), idx);
    }

    //*************************************************************************
    // stride_view tests
    //*************************************************************************

    //*************************************************************************
    TEST(test_ranges_stride_view_basic)
    {
      std::vector<int> v  = {1, 2, 3, 4, 5, 6, 7, 8, 9};
      auto             sv = etl::ranges::stride_view(v, 3);

      std::vector<int> expected{1, 4, 7};
      std::vector<int> actual;
      for (auto val : sv)
      {
        actual.push_back(val);
      }
      CHECK_EQUAL(expected, actual);
    }

    //*************************************************************************
    TEST(test_ranges_stride_view_stride_of_one)
    {
      std::vector<int> v  = {10, 20, 30};
      auto             sv = etl::ranges::stride_view(v, 1);

      std::vector<int> expected{10, 20, 30};
      std::vector<int> actual;
      for (auto val : sv)
      {
        actual.push_back(val);
      }
      CHECK_EQUAL(expected, actual);
    }

    //*************************************************************************
    TEST(test_ranges_stride_view_stride_of_two)
    {
      std::vector<int> v  = {1, 2, 3, 4, 5, 6};
      auto             sv = etl::ranges::stride_view(v, 2);

      std::vector<int> expected{1, 3, 5};
      std::vector<int> actual;
      for (auto val : sv)
      {
        actual.push_back(val);
      }
      CHECK_EQUAL(expected, actual);
    }

    //*************************************************************************
    TEST(test_ranges_stride_view_stride_larger_than_range)
    {
      std::vector<int> v  = {1, 2, 3};
      auto             sv = etl::ranges::stride_view(v, 10);

      std::vector<int> expected{1};
      std::vector<int> actual;
      for (auto val : sv)
      {
        actual.push_back(val);
      }
      CHECK_EQUAL(expected, actual);
    }

    //*************************************************************************
    TEST(test_ranges_stride_view_stride_equals_range_size)
    {
      std::vector<int> v  = {1, 2, 3};
      auto             sv = etl::ranges::stride_view(v, 3);

      std::vector<int> expected{1};
      std::vector<int> actual;
      for (auto val : sv)
      {
        actual.push_back(val);
      }
      CHECK_EQUAL(expected, actual);
    }

    //*************************************************************************
    TEST(test_ranges_stride_view_empty_range)
    {
      std::vector<int> v;
      auto             sv = etl::ranges::stride_view(v, 3);

      size_t count = 0;
      for (auto val : sv)
      {
        (void)val;
        ++count;
      }
      CHECK_EQUAL(0U, count);
    }

    //*************************************************************************
    TEST(test_ranges_stride_view_single_element)
    {
      std::vector<int> v  = {42};
      auto             sv = etl::ranges::stride_view(v, 5);

      std::vector<int> expected{42};
      std::vector<int> actual;
      for (auto val : sv)
      {
        actual.push_back(val);
      }
      CHECK_EQUAL(expected, actual);
    }

    //*************************************************************************
    TEST(test_ranges_stride_view_pipe)
    {
      std::vector<int> v  = {1, 2, 3, 4, 5, 6, 7, 8, 9};
      auto             sv = v | etl::views::stride(3);

      std::vector<int> expected{1, 4, 7};
      std::vector<int> actual;
      for (auto val : sv)
      {
        actual.push_back(val);
      }
      CHECK_EQUAL(expected, actual);
    }

    //*************************************************************************
    TEST(test_ranges_stride_view_pipe_stride_of_two)
    {
      std::vector<int> v  = {10, 20, 30, 40, 50};
      auto             sv = v | etl::views::stride(2);

      std::vector<int> expected{10, 30, 50};
      std::vector<int> actual;
      for (auto val : sv)
      {
        actual.push_back(val);
      }
      CHECK_EQUAL(expected, actual);
    }

    //*************************************************************************
    TEST(test_ranges_stride_view_functional_call)
    {
      std::vector<int> v  = {1, 2, 3, 4, 5, 6};
      auto             sv = etl::views::stride(v, 2);

      std::vector<int> expected{1, 3, 5};
      std::vector<int> actual;
      for (auto val : sv)
      {
        actual.push_back(val);
      }
      CHECK_EQUAL(expected, actual);
    }

    //*************************************************************************
    TEST(test_ranges_stride_view_etl_vector)
    {
      etl::vector<int, 10> v  = {1, 2, 3, 4, 5, 6, 7, 8};
      auto                 sv = etl::ranges::stride_view(v, 3);

      std::vector<int> expected{1, 4, 7};
      std::vector<int> actual;
      for (auto val : sv)
      {
        actual.push_back(val);
      }
      CHECK_EQUAL(expected, actual);
    }

    //*************************************************************************
    TEST(test_ranges_stride_view_array)
    {
      std::array<int, 6> a  = {10, 20, 30, 40, 50, 60};
      auto               sv = etl::ranges::stride_view(a, 2);

      std::vector<int> expected{10, 30, 50};
      std::vector<int> actual;
      for (auto val : sv)
      {
        actual.push_back(val);
      }
      CHECK_EQUAL(expected, actual);
    }

    //*************************************************************************
    TEST(test_ranges_stride_view_not_evenly_divisible)
    {
      std::vector<int> v  = {1, 2, 3, 4, 5, 6, 7};
      auto             sv = etl::ranges::stride_view(v, 3);

      std::vector<int> expected{1, 4, 7};
      std::vector<int> actual;
      for (auto val : sv)
      {
        actual.push_back(val);
      }
      CHECK_EQUAL(expected, actual);
    }

    //*************************************************************************
    TEST(test_ranges_stride_view_pipe_chain_take_stride)
    {
      // Take first 6, then stride by 2
      std::vector<int> v  = {1, 2, 3, 4, 5, 6, 7, 8, 9};
      auto             sv = v | etl::views::take(6) | etl::views::stride(2);

      std::vector<int> expected{1, 3, 5};
      std::vector<int> actual;
      for (auto val : sv)
      {
        actual.push_back(val);
      }
      CHECK_EQUAL(expected, actual);
    }

    //*************************************************************************
    TEST(test_ranges_stride_view_pipe_chain_drop_stride)
    {
      // Drop first 2, then stride by 3
      std::vector<int> v  = {1, 2, 3, 4, 5, 6, 7, 8, 9};
      auto             sv = v | etl::views::drop(2) | etl::views::stride(3);

      std::vector<int> expected{3, 6, 9};
      std::vector<int> actual;
      for (auto val : sv)
      {
        actual.push_back(val);
      }
      CHECK_EQUAL(expected, actual);
    }

    //*************************************************************************
    TEST(test_ranges_stride_view_pipe_chain_stride_transform)
    {
      // Stride by 2, then transform (multiply by 10)
      std::vector<int> v     = {1, 2, 3, 4, 5};
      auto             mul10 = [](int x)
      {
        return x * 10;
      };
      auto sv = v | etl::views::stride(2) | etl::views::transform(mul10);

      std::vector<int> expected{10, 30, 50};
      std::vector<int> actual;
      for (auto val : sv)
      {
        actual.push_back(val);
      }
      CHECK_EQUAL(expected, actual);
    }

    //*************************************************************************
    TEST(test_ranges_stride_view_pipe_chain_filter_stride)
    {
      // Filter even, then stride by 2
      std::vector<int> v       = {1, 2, 3, 4, 5, 6, 7, 8};
      auto             is_even = [](int x)
      {
        return x % 2 == 0;
      };
      auto sv = v | etl::views::filter(is_even) | etl::views::stride(2);

      std::vector<int> expected{2, 6};
      std::vector<int> actual;
      for (auto val : sv)
      {
        actual.push_back(val);
      }
      CHECK_EQUAL(expected, actual);
    }

    //*************************************************************************
    TEST(test_ranges_cartesian_product_view_basic)
    {
      std::vector<int>         v1 = {1, 2};
      std::vector<std::string> v2 = {"a", "b", "c"};

      auto cp = etl::views::cartesian_product(v1, v2);

      auto it = cp.begin();
      CHECK_EQUAL(1, etl::get<0>(*it));
      CHECK_EQUAL(std::string("a"), etl::get<1>(*it));
      ++it;
      CHECK_EQUAL(1, etl::get<0>(*it));
      CHECK_EQUAL(std::string("b"), etl::get<1>(*it));
      ++it;
      CHECK_EQUAL(1, etl::get<0>(*it));
      CHECK_EQUAL(std::string("c"), etl::get<1>(*it));
      ++it;
      CHECK_EQUAL(2, etl::get<0>(*it));
      CHECK_EQUAL(std::string("a"), etl::get<1>(*it));
      ++it;
      CHECK_EQUAL(2, etl::get<0>(*it));
      CHECK_EQUAL(std::string("b"), etl::get<1>(*it));
      ++it;
      CHECK_EQUAL(2, etl::get<0>(*it));
      CHECK_EQUAL(std::string("c"), etl::get<1>(*it));
      ++it;
      CHECK(it == cp.end());
    }

    //*************************************************************************
    TEST(test_ranges_cartesian_product_view_size)
    {
      std::vector<int> v1 = {1, 2, 3};
      std::vector<int> v2 = {10, 20};

      auto cp = etl::views::cartesian_product(v1, v2);

      CHECK_EQUAL(6U, cp.size());
    }

    //*************************************************************************
    TEST(test_ranges_cartesian_product_view_three_ranges)
    {
      std::vector<int>    v1 = {1, 2};
      std::vector<char>   v2 = {'a', 'b'};
      std::vector<double> v3 = {0.5};

      auto cp = etl::views::cartesian_product(v1, v2, v3);

      CHECK_EQUAL(4U, cp.size());

      std::vector<etl::tuple<int, char, double>> expected = {{1, 'a', 0.5}, {1, 'b', 0.5}, {2, 'a', 0.5}, {2, 'b', 0.5}};

      size_t idx = 0;
      for (auto val : cp)
      {
        CHECK_EQUAL(etl::get<0>(expected[idx]), etl::get<0>(val));
        CHECK_EQUAL(etl::get<1>(expected[idx]), etl::get<1>(val));
        CHECK_CLOSE(etl::get<2>(expected[idx]), etl::get<2>(val), 0.01);
        ++idx;
      }
      CHECK_EQUAL(4U, idx);
    }

    //*************************************************************************
    TEST(test_ranges_cartesian_product_view_empty_range)
    {
      std::vector<int> v1 = {1, 2, 3};
      std::vector<int> v2;

      auto cp = etl::views::cartesian_product(v1, v2);

      CHECK_EQUAL(0U, cp.size());
      CHECK(cp.begin() == cp.end());
    }

    //*************************************************************************
    TEST(test_ranges_cartesian_product_view_single_range)
    {
      std::vector<int> v1 = {10, 20, 30};

      auto cp = etl::views::cartesian_product(v1);

      CHECK_EQUAL(3U, cp.size());

      auto it = cp.begin();
      CHECK_EQUAL(10, etl::get<0>(*it));
      ++it;
      CHECK_EQUAL(20, etl::get<0>(*it));
      ++it;
      CHECK_EQUAL(30, etl::get<0>(*it));
      ++it;
      CHECK(it == cp.end());
    }

    //*************************************************************************
    TEST(test_ranges_cartesian_product_view_range_for)
    {
      std::vector<int> v1 = {1, 2};
      std::vector<int> v2 = {10, 20};

      auto cp = etl::views::cartesian_product(v1, v2);

      std::vector<etl::tuple<int, int>> result;
      for (auto val : cp)
      {
        result.push_back(val);
      }

      CHECK_EQUAL(4U, result.size());
      CHECK_EQUAL(1, etl::get<0>(result[0]));
      CHECK_EQUAL(10, etl::get<1>(result[0]));
      CHECK_EQUAL(1, etl::get<0>(result[1]));
      CHECK_EQUAL(20, etl::get<1>(result[1]));
      CHECK_EQUAL(2, etl::get<0>(result[2]));
      CHECK_EQUAL(10, etl::get<1>(result[2]));
      CHECK_EQUAL(2, etl::get<0>(result[3]));
      CHECK_EQUAL(20, etl::get<1>(result[3]));
    }

    //*************************************************************************
    TEST(test_ranges_cartesian_product_view_with_array)
    {
      std::array<int, 2>  a1 = {1, 2};
      std::array<char, 3> a2 = {'x', 'y', 'z'};

      auto cp = etl::views::cartesian_product(a1, a2);

      CHECK_EQUAL(6U, cp.size());

      auto it = cp.begin();
      CHECK_EQUAL(1, etl::get<0>(*it));
      CHECK_EQUAL('x', etl::get<1>(*it));
      ++it;
      CHECK_EQUAL(1, etl::get<0>(*it));
      CHECK_EQUAL('y', etl::get<1>(*it));
      ++it;
      CHECK_EQUAL(1, etl::get<0>(*it));
      CHECK_EQUAL('z', etl::get<1>(*it));
      ++it;
      CHECK_EQUAL(2, etl::get<0>(*it));
      CHECK_EQUAL('x', etl::get<1>(*it));
      ++it;
      CHECK_EQUAL(2, etl::get<0>(*it));
      CHECK_EQUAL('y', etl::get<1>(*it));
      ++it;
      CHECK_EQUAL(2, etl::get<0>(*it));
      CHECK_EQUAL('z', etl::get<1>(*it));
      ++it;
      CHECK(it == cp.end());
    }

    //*************************************************************************
    TEST(test_ranges_cartesian_product_view_chained_with_take)
    {
      std::vector<int> v1 = {1, 2, 3};
      std::vector<int> v2 = {10, 20, 30};

      auto cp = etl::views::cartesian_product(v1, v2) | etl::views::take(4);

      std::vector<etl::tuple<int, int>> result;
      for (auto val : cp)
      {
        result.push_back(val);
      }

      CHECK_EQUAL(4U, result.size());
      CHECK_EQUAL(1, etl::get<0>(result[0]));
      CHECK_EQUAL(10, etl::get<1>(result[0]));
      CHECK_EQUAL(1, etl::get<0>(result[1]));
      CHECK_EQUAL(20, etl::get<1>(result[1]));
      CHECK_EQUAL(1, etl::get<0>(result[2]));
      CHECK_EQUAL(30, etl::get<1>(result[2]));
      CHECK_EQUAL(2, etl::get<0>(result[3]));
      CHECK_EQUAL(10, etl::get<1>(result[3]));
    }

    //*************************************************************************
    TEST(test_ranges_cartesian_product_view_single_elements)
    {
      std::vector<int>  v1 = {42};
      std::vector<char> v2 = {'z'};

      auto cp = etl::views::cartesian_product(v1, v2);

      CHECK_EQUAL(1U, cp.size());

      auto it = cp.begin();
      CHECK_EQUAL(42, etl::get<0>(*it));
      CHECK_EQUAL('z', etl::get<1>(*it));
      ++it;
      CHECK(it == cp.end());
    }

    //*************************************************************************
    TEST(test_ranges_cartesian_product_view_first_range_empty)
    {
      std::vector<int> v1;
      std::vector<int> v2 = {1, 2, 3};

      auto cp = etl::views::cartesian_product(v1, v2);

      CHECK_EQUAL(0U, cp.size());
      CHECK(cp.begin() == cp.end());
    }

    //*************************************************************************
    /// to_input_view tests
    //*************************************************************************
    TEST(test_ranges_to_input_view_functional)
    {
      etl::vector<int, 10> v_in{0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
      etl::vector<int, 10> v_out;
      etl::vector<int, 10> v_out_expected{0, 1, 2, 3, 4, 5, 6, 7, 8, 9};

      auto iv = etl::ranges::to_input_view(v_in);

      // Iterator category should be input_iterator_tag
      using iv_iterator = decltype(iv.begin());
      using iv_category = typename etl::iterator_traits<iv_iterator>::iterator_category;
      static_assert(etl::is_same_v<iv_category, ETL_OR_STD::input_iterator_tag>, "to_input_view iterator should have input_iterator_tag");

      for (auto i : iv)
      {
        v_out.push_back(i);
      }

      CHECK_EQUAL(v_out_expected, v_out);
      CHECK_EQUAL(iv.size(), 10u);
      CHECK_EQUAL(iv.empty(), false);
    }

    //*************************************************************************
    TEST(test_ranges_to_input_view_empty_range)
    {
      etl::vector<int, 10> v_in;

      auto iv = etl::ranges::to_input_view(v_in);

      CHECK_EQUAL(iv.size(), 0u);
      CHECK_EQUAL(iv.empty(), true);
      CHECK(iv.begin() == iv.end());
    }

    //*************************************************************************
    TEST(test_ranges_to_input_view_reflects_base_changes)
    {
      etl::vector<int, 10> v_in{0, 1, 2, 3, 4};

      auto iv = etl::ranges::to_input_view(v_in);

      CHECK_EQUAL(iv.front(), 0);
      CHECK_EQUAL(iv.size(), 5u);

      v_in[0] = 99;
      CHECK_EQUAL(iv.front(), 99);
    }

    //*************************************************************************
    TEST(test_ranges_to_input_view_pipe)
    {
      etl::vector<int, 10> v_in{0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
      etl::vector<int, 10> v_out;
      etl::vector<int, 10> v_out_expected{0, 1, 2, 3, 4, 5, 6, 7, 8, 9};

      auto iv = v_in | etl::views::to_input();

      using iv_iterator = decltype(iv.begin());
      using iv_category = typename etl::iterator_traits<iv_iterator>::iterator_category;
      static_assert(etl::is_same_v<iv_category, ETL_OR_STD::input_iterator_tag>, "piped to_input view iterator should have input_iterator_tag");

      for (auto i : iv)
      {
        v_out.push_back(i);
      }

      CHECK_EQUAL(v_out_expected, v_out);
      CHECK_EQUAL(iv.size(), 10u);
    }

    //*************************************************************************
    TEST(test_ranges_views_to_input_functional)
    {
      etl::vector<int, 10> v_in{0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
      etl::vector<int, 10> v_out;
      etl::vector<int, 10> v_out_expected{0, 1, 2, 3, 4, 5, 6, 7, 8, 9};

      auto iv = etl::views::to_input(v_in);

      using iv_iterator = decltype(iv.begin());
      using iv_category = typename etl::iterator_traits<iv_iterator>::iterator_category;
      static_assert(etl::is_same_v<iv_category, ETL_OR_STD::input_iterator_tag>, "views::to_input iterator should have input_iterator_tag");

      for (auto i : iv)
      {
        v_out.push_back(i);
      }

      CHECK_EQUAL(v_out_expected, v_out);
    }

    //*************************************************************************
    TEST(test_ranges_to_input_view_pipe_chained_with_take)
    {
      etl::vector<int, 10> v_in{0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
      etl::vector<int, 10> v_out;
      etl::vector<int, 10> v_out_expected{0, 1, 2};

      auto iv = v_in | etl::views::to_input() | etl::views::take(3);

      for (auto i : iv)
      {
        v_out.push_back(i);
      }

      CHECK_EQUAL(v_out_expected, v_out);
    }

    //*************************************************************************
    TEST(test_ranges_to_input_view_pipe_chained_with_drop)
    {
      etl::vector<int, 10> v_in{0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
      etl::vector<int, 10> v_out;
      etl::vector<int, 10> v_out_expected{7, 8, 9};

      auto iv = v_in | etl::views::to_input() | etl::views::drop(7);

      for (auto i : iv)
      {
        v_out.push_back(i);
      }

      CHECK_EQUAL(v_out_expected, v_out);
    }

    //*************************************************************************
    TEST(test_ranges_to_input_view_pipe_chained_with_transform)
    {
      etl::vector<int, 10> v_in{0, 1, 2, 3, 4};
      etl::vector<int, 10> v_out;
      etl::vector<int, 10> v_out_expected{0, 2, 4, 6, 8};

      auto doubler = [](int i)
      {
        return i * 2;
      };
      auto iv = v_in | etl::views::to_input() | etl::views::transform(doubler);

      for (auto i : iv)
      {
        v_out.push_back(i);
      }

      CHECK_EQUAL(v_out_expected, v_out);
    }

    //*************************************************************************
    TEST(test_ranges_to_input_view_with_std_vector)
    {
      std::vector<int> v_in{0, 1, 2, 3, 4};
      std::vector<int> v_out;
      std::vector<int> v_out_expected{0, 1, 2, 3, 4};

      auto iv = etl::ranges::to_input_view(v_in);

      using iv_iterator = decltype(iv.begin());
      using iv_category = typename etl::iterator_traits<iv_iterator>::iterator_category;
      static_assert(etl::is_same_v<iv_category, ETL_OR_STD::input_iterator_tag>, "to_input_view over std::vector should have input_iterator_tag");

      for (auto i : iv)
      {
        v_out.push_back(i);
      }

      CHECK_EQUAL(v_out_expected.size(), v_out.size());
      for (size_t idx = 0; idx < v_out_expected.size(); ++idx)
      {
        CHECK_EQUAL(v_out_expected[idx], v_out[idx]);
      }
    }

    //*************************************************************************
    TEST(test_ranges_to_input_view_downgrades_from_random_access)
    {
      // std::vector has random_access_iterator; verify to_input downgrades it
      std::vector<int> v_in{10, 20, 30};

      using original_category = typename etl::iterator_traits< std::vector<int>::iterator>::iterator_category;
      static_assert(etl::is_same_v<original_category, ETL_OR_STD::random_access_iterator_tag>, "std::vector iterator should be random_access");

      auto iv = etl::ranges::to_input_view(v_in);

      using iv_iterator = decltype(iv.begin());
      using iv_category = typename etl::iterator_traits<iv_iterator>::iterator_category;
      static_assert(etl::is_same_v<iv_category, ETL_OR_STD::input_iterator_tag>, "to_input_view should downgrade to input_iterator_tag");

      etl::vector<int, 10> v_out;
      for (auto i : iv)
      {
        v_out.push_back(i);
      }

      CHECK_EQUAL(3u, v_out.size());
      CHECK_EQUAL(10, v_out[0]);
      CHECK_EQUAL(20, v_out[1]);
      CHECK_EQUAL(30, v_out[2]);
    }

    //*************************************************************************
    TEST(test_ranges_to_input_view_downgrades_from_bidirectional)
    {
      // std::list has bidirectional_iterator; verify to_input downgrades it
      std::list<int> l_in{10, 20, 30};

      using original_category = typename etl::iterator_traits< std::list<int>::iterator>::iterator_category;
      static_assert(etl::is_same_v<original_category, ETL_OR_STD::bidirectional_iterator_tag>, "std::list iterator should be bidirectional");

      auto iv = etl::ranges::to_input_view(l_in);

      using iv_iterator = decltype(iv.begin());
      using iv_category = typename etl::iterator_traits<iv_iterator>::iterator_category;
      static_assert(etl::is_same_v<iv_category, ETL_OR_STD::input_iterator_tag>, "to_input_view should downgrade to input_iterator_tag");

      etl::vector<int, 10> v_out;
      for (auto i : iv)
      {
        v_out.push_back(i);
      }

      CHECK_EQUAL(3u, v_out.size());
      CHECK_EQUAL(10, v_out[0]);
      CHECK_EQUAL(20, v_out[1]);
      CHECK_EQUAL(30, v_out[2]);
    }
  }
    #include "etl/private/diagnostic_pop.h"
} // namespace

  #endif
#endif
