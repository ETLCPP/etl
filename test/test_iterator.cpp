/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl
https://www.etlcpp.com

Copyright(c) 2014 John Wellbelove

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

#include <string>
#include <list>
#include <queue>
#include <algorithm>
#include <vector>

#include "etl/iterator.h"

#include "iterators_for_unit_tests.h"
#include "data.h"

namespace
{
  using Item = TestDataM<std::string>;

  struct input : public etl::iterator<ETL_OR_STD::input_iterator_tag, int>
  {

  };

  struct output : public etl::iterator<ETL_OR_STD::output_iterator_tag, int>
  {

  };

  struct forward : public etl::iterator<ETL_OR_STD::forward_iterator_tag, int>
  {

  };

  struct bidirectional : public etl::iterator<ETL_OR_STD::bidirectional_iterator_tag, int>
  {

  };

  struct random : public etl::iterator<ETL_OR_STD::random_access_iterator_tag, int>
  {

  };

  typedef int* pointer;
  typedef const int* const_pointer;

  // A sentinel type for non_random_iterator, used to test the
  // advance(i, n, bound) overload where I != S (different types).
  template <typename T>
  struct non_random_sentinel
  {
    non_random_sentinel() : ptr(nullptr) {}
    non_random_sentinel(T* v) : ptr(v) {}
    T* ptr;
  };

  template <typename T>
  bool operator==(const non_random_iterator<T>& lhs, const non_random_sentinel<T>& rhs)
  {
    return lhs.ptr == rhs.ptr;
  }

  template <typename T>
  bool operator!=(const non_random_iterator<T>& lhs, const non_random_sentinel<T>& rhs)
  {
    return lhs.ptr != rhs.ptr;
  }

  template <typename T>
  bool operator==(const non_random_sentinel<T>& lhs, const non_random_iterator<T>& rhs)
  {
    return rhs == lhs;
  }

  template <typename T>
  bool operator!=(const non_random_sentinel<T>& lhs, const non_random_iterator<T>& rhs)
  {
    return rhs != lhs;
  }

  const size_t SIZE = 10UL;
  int dataA[SIZE] = { 2, 1, 4, 3, 6, 5, 8, 7, 10, 9 };

  SUITE(test_iterator)
  {
    // NOTE '!!' is required to keep GCC happy.

    //*************************************************************************
    TEST(distance_non_random)
    {
      ptrdiff_t d = etl::distance(non_random_iterator<int>(&dataA[0]), non_random_iterator<int>(dataA + SIZE));

      CHECK_EQUAL(SIZE, d);
    }

    //*************************************************************************
    TEST(distance_random)
    {
      ptrdiff_t d = etl::distance(random_iterator<int>(&dataA[0]), random_iterator<int>(dataA + SIZE));

      CHECK_EQUAL(SIZE, d);
    }

    //*************************************************************************
    TEST(advance_non_random)
    {
      int* itr1 = std::begin(dataA);
      non_random_iterator<int> itr2 = std::begin(dataA);

      std::advance(itr1, 4);
      etl::advance(itr2, 4);
      CHECK_EQUAL(*itr1, *itr2);

      std::advance(itr1, -3);
      etl::advance(itr2, -3);
      CHECK_EQUAL(*itr1, *itr2);
    }

    //*************************************************************************
    TEST(advance_random)
    {
      int* itr1 = std::begin(dataA);
      random_iterator<int> itr2 = std::begin(dataA);

      std::advance(itr1, 4);
      etl::advance(itr2, 4);
      CHECK_EQUAL(*itr1, *itr2);

      std::advance(itr1, -3);
      etl::advance(itr2, -3);
      CHECK_EQUAL(*itr1, *itr2);
    }

    //*************************************************************************
    TEST(prev)
    {
      int data[] = { 1, 2, 3, 4, 5, 6, 7 };

      size_t length = 6UL;

      int* itr = &data[0] + length;

      for (size_t i = 1UL; i <= length; ++i)
      {
        CHECK_EQUAL(data[length - i], *etl::prev(itr, static_cast<ptrdiff_t>(i)));
      }
    }

    //*************************************************************************
    TEST(next)
    {
      int data[] = { 1, 2, 3, 4, 5, 6, 7 };

      size_t length = 6UL;

      int* itr = &data[0];

      for (size_t i = 1UL; i <= length; ++i)
      {
        CHECK_EQUAL(data[i], *etl::next(itr, static_cast<ptrdiff_t>(i)));
      }
    }

    //*************************************************************************
    TEST(reverse_iterator)
    {
      int data[] = { 1, 2, 3, 4, 5, 6, 7 };

      std::reverse_iterator<int*> sri(&data[7]);
      etl::reverse_iterator<int*> eri(&data[7]);

      CHECK_EQUAL(*sri++, *eri++);
      CHECK_EQUAL(*sri++, *eri++);
      CHECK_EQUAL(*sri++, *eri++);

      CHECK(etl::reverse_iterator<int*>(&data[5]) < eri);
      CHECK(etl::reverse_iterator<int*>(&data[3]) > eri);
      CHECK(etl::reverse_iterator<int*>(&data[4]) <= eri);
      CHECK(etl::reverse_iterator<int*>(&data[5]) <= eri);
      CHECK(etl::reverse_iterator<int*>(&data[4]) >= eri);
      CHECK(etl::reverse_iterator<int*>(&data[3]) >= eri);

      CHECK_EQUAL(*sri++, *eri++);
      CHECK_EQUAL(*sri++, *eri++);
      CHECK_EQUAL(*sri++, *eri++);
      CHECK_EQUAL(*sri, *eri);

      CHECK(etl::reverse_iterator<int*>(&data[1]) == eri);
      CHECK(etl::reverse_iterator<int*>(&data[2]) != eri);
      CHECK(etl::reverse_iterator<int*>(&data[3]) != eri);
      CHECK(etl::reverse_iterator<int*>(&data[4]) != eri);
      CHECK(etl::reverse_iterator<int*>(&data[5]) != eri);
      CHECK(etl::reverse_iterator<int*>(&data[6]) != eri);
      CHECK(etl::reverse_iterator<int*>(&data[7]) != eri);

      sri = std::reverse_iterator<int*>(&data[7]);
      eri = etl::reverse_iterator<int*>(&data[7]);
      CHECK_EQUAL(sri[0], eri[0]);
      CHECK_EQUAL(sri[1], eri[1]);
      CHECK_EQUAL(sri[2], eri[2]);
      CHECK_EQUAL(sri[3], eri[3]);
      CHECK_EQUAL(sri[4], eri[4]);
      CHECK_EQUAL(sri[5], eri[5]);
      CHECK_EQUAL(sri[6], eri[6]);

      sri += 4;
      eri += 4;
      CHECK_EQUAL(*sri, *eri);

      sri -= 2;
      eri -= 2;
      CHECK_EQUAL(*sri, *eri);

      std::reverse_iterator<int*>    sri2 = sri + 3;
      etl::reverse_iterator<int*> eri2 = eri + 3;
      CHECK_EQUAL(*sri, *eri);

      sri2 = sri - 3;
      eri2 = eri - 3;
      CHECK_EQUAL(*sri, *eri);
    }

    //*************************************************************************
    TEST(reverse_iterator_const)
    {
      const int data[] = { 1, 2, 3, 4, 5, 6, 7 };

      std::reverse_iterator<const int*> sri(&data[7]);
      etl::reverse_iterator<const int*> eri(&data[7]);

      CHECK_EQUAL(*sri++, *eri++);
      CHECK_EQUAL(*sri++, *eri++);
      CHECK_EQUAL(*sri++, *eri++);

      CHECK(etl::reverse_iterator<const int*>(&data[5]) < eri);
      CHECK(etl::reverse_iterator<const int*>(&data[3]) > eri);
      CHECK(etl::reverse_iterator<const int*>(&data[4]) <= eri);
      CHECK(etl::reverse_iterator<const int*>(&data[5]) <= eri);
      CHECK(etl::reverse_iterator<const int*>(&data[4]) >= eri);
      CHECK(etl::reverse_iterator<const int*>(&data[3]) >= eri);

      CHECK_EQUAL(*sri++, *eri++);
      CHECK_EQUAL(*sri++, *eri++);
      CHECK_EQUAL(*sri++, *eri++);
      CHECK_EQUAL(*sri, *eri);

      CHECK(etl::reverse_iterator<const int*>(&data[1]) == eri);
      CHECK(etl::reverse_iterator<const int*>(&data[2]) != eri);
      CHECK(etl::reverse_iterator<const int*>(&data[3]) != eri);
      CHECK(etl::reverse_iterator<const int*>(&data[4]) != eri);
      CHECK(etl::reverse_iterator<const int*>(&data[5]) != eri);
      CHECK(etl::reverse_iterator<const int*>(&data[6]) != eri);
      CHECK(etl::reverse_iterator<const int*>(&data[7]) != eri);

      sri = std::reverse_iterator<const int*>(&data[7]);
      eri = etl::reverse_iterator<const int*>(&data[7]);
      CHECK_EQUAL(sri[0], eri[0]);
      CHECK_EQUAL(sri[1], eri[1]);
      CHECK_EQUAL(sri[2], eri[2]);
      CHECK_EQUAL(sri[3], eri[3]);
      CHECK_EQUAL(sri[4], eri[4]);
      CHECK_EQUAL(sri[5], eri[5]);
      CHECK_EQUAL(sri[6], eri[6]);

      CHECK_EQUAL(*sri, *eri);
      CHECK_EQUAL(*(sri + 1), *(eri + 1));
      CHECK_EQUAL(*(sri + 2), *(eri + 2));
      CHECK_EQUAL(*(sri + 3), *(eri + 3));
      CHECK_EQUAL(*(sri + 4), *(eri + 4));
      CHECK_EQUAL(*(sri + 5), *(eri + 5));
      CHECK_EQUAL(*(sri + 6), *(eri + 6));

      sri += 4;
      eri += 4;
      CHECK_EQUAL(*sri, *eri);

      sri -= 2;
      eri -= 2;
      CHECK_EQUAL(*sri, *eri);

      std::reverse_iterator<const int*>    sri2 = sri + 3;
      etl::reverse_iterator<const int*> eri2 = eri + 3;
      CHECK_EQUAL(*sri, *eri);

      sri2 = sri - 3;
      eri2 = eri - 3;
      CHECK_EQUAL(*sri, *eri);
    }

    //*************************************************************************
    TEST(test_input)
    {
      CHECK(!!etl::is_input_iterator<input>::value);
      CHECK(!etl::is_output_iterator<input>::value);
      CHECK(!etl::is_forward_iterator<input>::value);
      CHECK(!etl::is_bidirectional_iterator<input>::value);
      CHECK(!etl::is_random_iterator<input>::value);

      CHECK(!!etl::is_input_iterator_concept<input>::value);
      CHECK(!etl::is_output_iterator_concept<input>::value);
      CHECK(!etl::is_forward_iterator_concept<input>::value);
      CHECK(!etl::is_bidirectional_iterator_concept<input>::value);
      CHECK(!etl::is_random_iterator_concept<input>::value);
    }

    //*************************************************************************
    TEST(test_output)
    {
      CHECK(!etl::is_input_iterator<output>::value);
      CHECK(!!etl::is_output_iterator<output>::value);
      CHECK(!etl::is_forward_iterator<output>::value);
      CHECK(!etl::is_bidirectional_iterator<output>::value);
      CHECK(!etl::is_random_iterator<output>::value);

      CHECK(!etl::is_input_iterator_concept<output>::value);
      CHECK(etl::is_output_iterator_concept<output>::value == true);
      CHECK(!etl::is_forward_iterator_concept<output>::value);
      CHECK(!etl::is_bidirectional_iterator_concept<output>::value);
      CHECK(!etl::is_random_iterator_concept<output>::value);
    }

    //*************************************************************************
    TEST(test_forward)
    {
      CHECK(!etl::is_input_iterator<forward>::value);
      CHECK(!etl::is_output_iterator<forward>::value);
      CHECK(!!etl::is_forward_iterator<forward>::value);
      CHECK(!etl::is_bidirectional_iterator<forward>::value);
      CHECK(!etl::is_random_iterator<forward>::value);

      CHECK(!!etl::is_input_iterator_concept<forward>::value);
      CHECK(!!etl::is_output_iterator_concept<forward>::value);
      CHECK(!!etl::is_forward_iterator_concept<forward>::value);
      CHECK(!etl::is_bidirectional_iterator_concept<forward>::value);
      CHECK(!etl::is_random_iterator_concept<forward>::value);
    }

    //*************************************************************************
    TEST(test_bidirectional)
    {
      CHECK(!etl::is_input_iterator<bidirectional>::value);
      CHECK(!etl::is_output_iterator<bidirectional>::value);
      CHECK(!etl::is_forward_iterator<bidirectional>::value);
      CHECK(!!etl::is_bidirectional_iterator<bidirectional>::value);
      CHECK(!etl::is_random_iterator<bidirectional>::value);

      CHECK(!!etl::is_input_iterator_concept<bidirectional>::value);
      CHECK(!!etl::is_output_iterator_concept<bidirectional>::value);
      CHECK(!!etl::is_forward_iterator_concept<bidirectional>::value);
      CHECK(!!etl::is_bidirectional_iterator_concept<bidirectional>::value);
      CHECK(!etl::is_random_iterator_concept<bidirectional>::value);
    }

    //*************************************************************************
    TEST(test_random)
    {
      CHECK(!etl::is_input_iterator<random>::value);
      CHECK(!etl::is_output_iterator<random>::value);
      CHECK(!etl::is_forward_iterator<random>::value);
      CHECK(!etl::is_bidirectional_iterator<random>::value);
      CHECK(!!etl::is_random_iterator<random>::value);

      CHECK(!!etl::is_input_iterator_concept<random>::value);
      CHECK(!!etl::is_output_iterator_concept<random>::value);
      CHECK(!!etl::is_forward_iterator_concept<random>::value);
      CHECK(!!etl::is_bidirectional_iterator_concept<random>::value);
      CHECK(!!etl::is_random_iterator_concept<random>::value);
    }

    //*************************************************************************
    TEST(test_pointer)
    {
      CHECK(!etl::is_input_iterator<pointer>::value);
      CHECK(!etl::is_output_iterator<pointer>::value);
      CHECK(!etl::is_forward_iterator<pointer>::value);
      CHECK(!etl::is_bidirectional_iterator<pointer>::value);
      CHECK(!!etl::is_random_iterator<pointer>::value);

      CHECK(!!etl::is_input_iterator_concept<pointer>::value);
      CHECK(!!etl::is_output_iterator_concept<pointer>::value);
      CHECK(!!etl::is_forward_iterator_concept<pointer>::value);
      CHECK(!!etl::is_bidirectional_iterator_concept<pointer>::value);
      CHECK(!!etl::is_random_iterator_concept<pointer>::value);
    }

    //*************************************************************************
    TEST(test_const_pointer)
    {
      CHECK(!etl::is_input_iterator<const_pointer>::value);
      CHECK(!etl::is_output_iterator<const_pointer>::value);
      CHECK(!etl::is_forward_iterator<const_pointer>::value);
      CHECK(!etl::is_bidirectional_iterator<const_pointer>::value);
      CHECK(!!etl::is_random_iterator<const_pointer>::value);

      CHECK(!!etl::is_input_iterator_concept<const_pointer>::value);
      CHECK(!!etl::is_output_iterator_concept<const_pointer>::value);
      CHECK(!!etl::is_forward_iterator_concept<const_pointer>::value);
      CHECK(!!etl::is_bidirectional_iterator_concept<const_pointer>::value);
      CHECK(!!etl::is_random_iterator_concept<const_pointer>::value);
    }

    //*************************************************************************
    TEST(test_move_iterator_constructors)
    {
      Item list[] = { Item("1"), Item("2"), Item("3") };

      etl::move_iterator<Item*> mitr1(&list[0]);
      etl::move_iterator<Item*> mitr2(&list[1]);
      etl::move_iterator<Item*> mitr3(&list[1]);

      etl::move_iterator<Item*> mitr4 = etl::make_move_iterator(&list[2]);
      etl::move_iterator<Item*> mitr5(mitr4);

      CHECK(mitr1.base() == &list[0]);
      CHECK(mitr2.base() == &list[1]);
      CHECK(mitr3.base() == &list[1]);
      CHECK(mitr4.base() == &list[2]);
      CHECK(mitr5.base() == &list[2]);
    }

    //*************************************************************************
    TEST(test_move_iterator_relational_operators)
    {
      Item list[] = { Item("1"), Item("2"), Item("3") };

      etl::move_iterator<Item*> mitr1(&list[0]);
      etl::move_iterator<Item*> mitr2(&list[1]);
      etl::move_iterator<Item*> mitr3(&list[1]);

      etl::move_iterator<Item*> mitr4 = etl::make_move_iterator(&list[2]);
      etl::move_iterator<Item*> mitr5(mitr4);

      CHECK(mitr1 < mitr2);
      CHECK(!(mitr2 < mitr1));
      CHECK(!(mitr2 < mitr3));

      CHECK(mitr1 <= mitr2);
      CHECK(mitr2 <= mitr3);
      CHECK(!(mitr2 <= mitr1));

      CHECK(mitr2 > mitr1);
      CHECK(!(mitr1 > mitr2));
      CHECK(!(mitr3 > mitr2));

      CHECK(mitr2 >= mitr1);
      CHECK(mitr3 >= mitr2);
      CHECK(!(mitr1 >= mitr2));

      CHECK(mitr4 == mitr5);
      CHECK(mitr3 != mitr5);
    }

    //*************************************************************************
    TEST(test_move_iterator_access_operators)
    {
      Item item1("1");

      etl::move_iterator<Item*> mitr(&item1);

      CHECK_EQUAL("1", mitr->value);
      CHECK_EQUAL(true, mitr->valid);

      CHECK_EQUAL("1", (*mitr).value);
      CHECK_EQUAL(true, (*mitr).valid);

      Item item2 = *mitr; // Move item1

      CHECK_EQUAL(false, item1.valid);

      CHECK_EQUAL("1", item2.value);
      CHECK_EQUAL(true, item2.valid);
    }

    //*************************************************************************
    TEST(test_move_iterator_index)
    {
      Item list[] = { Item("1"), Item("2"), Item("3") };

      etl::move_iterator<Item*> mitr(&list[0]);

      CHECK_EQUAL("3", mitr[2].value);
      CHECK_EQUAL(true, mitr[2].valid);
    }

    //*************************************************************************
    TEST(test_move_iterator_increment_decrement)
    {
      Item list[] = { Item("1"), Item("2"), Item("3") };

      etl::move_iterator<Item*> mitr(&list[0]);

      mitr++;
      ++mitr;

      CHECK_EQUAL("3", mitr->value);
      CHECK_EQUAL(true, mitr->valid);

      mitr--;
      --mitr;

      CHECK_EQUAL("1", mitr->value);
      CHECK_EQUAL(true, mitr->valid);

      mitr += 1;

      CHECK_EQUAL("2", mitr->value);
      CHECK_EQUAL(true, mitr->valid);

      mitr -= 1;

      CHECK_EQUAL("1", mitr->value);
      CHECK_EQUAL(true, mitr->valid);

      mitr = mitr + 1;

      CHECK_EQUAL("2", mitr->value);
      CHECK_EQUAL(true, mitr->valid);

      mitr = mitr - 1;

      CHECK_EQUAL("1", mitr->value);
      CHECK_EQUAL(true, mitr->valid);
    }

    //*************************************************************************
    TEST(test_move_iterator_subtraction)
    {
      Item list[] = { Item("1"), Item("2"), Item("3") };

      etl::move_iterator<Item*> mitr1(&list[0]);
      etl::move_iterator<Item*> mitr2(&list[1]);

      etl::move_iterator<Item*>::difference_type d = mitr2 - mitr1;

      CHECK_EQUAL(1, d);
    }

    //*************************************************************************
    TEST(test_front_insert_iterator)
    {
      std::list<int> input    = {  0,  1,  2,  3,  4,  5, 6, 7, 8, 9 };
      std::list<int> expected = { 81, 64, 49, 36, 25, 16, 9, 4, 1, 0 };
      std::list<int> output;

      auto squared = [](int value)
      {
        return value * value;
      };

      std::transform(input.cbegin(), input.cend(), etl::front_inserter(output), squared);

      CHECK_EQUAL(expected.size(), output.size());
      CHECK(std::equal(output.begin(), output.end(), expected.begin()));
    }

    //*************************************************************************
    TEST(test_back_insert_iterator)
    {
      std::list<int> input    = { 0, 1, 2, 3,  4,  5,  6,  7,  8,  9 };
      std::list<int> expected = { 0, 1, 4, 9, 16, 25, 36, 49, 64, 81 };
      std::list<int> output;

      auto squared = [](int value) 
      {
        return value * value;
      };

      std::transform(input.cbegin(), input.cend(), etl::back_inserter(output), squared);

      CHECK_EQUAL(expected.size(), output.size());
      CHECK(std::equal(output.begin(), output.end(), expected.begin()));
    }

    //*************************************************************************
    //TEST(test_push_insert_iterator)
    //{
    //  std::list<int> input = { 0, 1, 2, 3,  4,  5,  6,  7,  8,  9 };
    //  std::list<int> expected = { 0, 1, 4, 9, 16, 25, 36, 49, 64, 81 };
    //  std::queue<int> output;

    //  auto squared = [](int value)
    //  {
    //    return value * value;
    //  };

    //  std::transform(input.begin(), input.cend(), etl::push_inserter(output), squared);

    //  CHECK_EQUAL(expected.size(), output.size());
    //  CHECK(std::equal(output.begin(), output.end(), expected.begin()));
    //}

#if ETL_USING_CPP17
    //*************************************************************************
    TEST(test_counted_iterator)
    {
      std::vector<int> v{1, 2, 3, 4};

      etl::counted_iterator<std::vector<int>::iterator> ci0{};
      CHECK_EQUAL(0, ci0.count());

      etl::counted_iterator ci1{v.begin(), 3};
      etl::counted_iterator ci2{v.begin(), 4};
      CHECK_EQUAL(3, ci1.count());

      CHECK_NOT_EQUAL(ci1.count(), ci2.count());
      CHECK(!(ci1 == ci2));

      ci2 = ci1;

      CHECK_EQUAL(ci1.count(), ci2.count());
      CHECK(ci1 == ci2);

      CHECK(ci1.base() == v.begin());

      CHECK(ci1.count() == 3);

      ci1++;

      CHECK(ci1.count() == 2);

      ++ci1;

      CHECK(ci1.count() == 1);

      ci1--;

      CHECK(ci1.count() == 2);
      --ci1;
      CHECK(ci1.count() == 3);

      ci1 += 2;
      CHECK(ci1.count() == 1);
      ci1 -= 2;
      CHECK(ci1.count() == 3);
      CHECK_EQUAL(ci1[0], 1);
      CHECK_EQUAL(ci1[1], 2);
      CHECK_EQUAL(ci1[2], 3);

      auto ci3 = ci1 + 3;
      CHECK(ci1.count() == 3);
      CHECK_EQUAL(ci3.count(), 0);
      auto ci4 = ci3 - 3;
      CHECK(ci1.count() == 3);
      CHECK_EQUAL(ci3.count(), 0);
      CHECK(ci4.count() == 3);

    }

    //*************************************************************************
    // etl::ranges::advance(i, n) with random access iterator
    //*************************************************************************
    TEST(ranges_advance_n_random_access_forward)
    {
      int data[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };

      int* itr = &data[0];
      etl::ranges::advance(itr, 4);
      CHECK_EQUAL(4, *itr);
    }

    //*************************************************************************
    TEST(ranges_advance_n_random_access_backward)
    {
      int data[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };

      int* itr = &data[7];
      etl::ranges::advance(itr, -3);
      CHECK_EQUAL(4, *itr);
    }

    //*************************************************************************
    TEST(ranges_advance_n_random_access_zero)
    {
      int data[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };

      int* itr = &data[5];
      etl::ranges::advance(itr, 0);
      CHECK_EQUAL(5, *itr);
    }

    //*************************************************************************
    // etl::ranges::advance(i, n) with bidirectional (non-random) iterator
    //*************************************************************************
    TEST(ranges_advance_n_bidirectional_forward)
    {
      std::list<int> data = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };

      auto itr = data.begin();
      etl::ranges::advance(itr, 4);
      CHECK_EQUAL(4, *itr);
    }

    //*************************************************************************
    TEST(ranges_advance_n_bidirectional_backward)
    {
      std::list<int> data = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };

      auto itr = data.end();
      etl::ranges::advance(itr, -3);
      CHECK_EQUAL(7, *itr);
    }

    //*************************************************************************
    TEST(ranges_advance_n_bidirectional_zero)
    {
      std::list<int> data = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };

      auto itr = data.begin();
      etl::ranges::advance(itr, 5);
      etl::ranges::advance(itr, 0);
      CHECK_EQUAL(5, *itr);
    }

    //*************************************************************************
    TEST(ranges_advance_n_non_random_iterator_forward)
    {
      int data[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };

      non_random_iterator<int> itr(&data[0]);
      etl::ranges::advance(itr, 6);
      CHECK_EQUAL(6, *itr);
    }

    //*************************************************************************
    TEST(ranges_advance_n_non_random_iterator_backward)
    {
      int data[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };

      non_random_iterator<int> itr(&data[8]);
      etl::ranges::advance(itr, -3);
      CHECK_EQUAL(5, *itr);
    }

    //*************************************************************************
    // etl::ranges::advance(i, bound) — advance to sentinel
    //*************************************************************************
    TEST(ranges_advance_to_bound_random_access)
    {
      int data[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };

      int* itr = &data[0];
      int* bound = &data[7];
      etl::ranges::advance(itr, bound);
      CHECK_EQUAL(7, *itr);
      CHECK(itr == bound);
    }

    //*************************************************************************
    TEST(ranges_advance_to_bound_bidirectional)
    {
      std::list<int> data = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };

      auto itr = data.begin();
      auto bound = data.end();
      etl::ranges::advance(itr, bound);
      CHECK(itr == bound);
    }

    //*************************************************************************
    TEST(ranges_advance_to_bound_non_random)
    {
      int data[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };

      non_random_iterator<int> itr(&data[0]);
      non_random_iterator<int> bound(&data[5]);
      etl::ranges::advance(itr, bound);
      CHECK_EQUAL(5, *itr);
    }

    //*************************************************************************
    TEST(ranges_advance_to_bound_already_at_bound)
    {
      int data[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };

      int* itr = &data[3];
      int* bound = &data[3];
      etl::ranges::advance(itr, bound);
      CHECK(itr == bound);
      CHECK_EQUAL(3, *itr);
    }

    //*************************************************************************
    // etl::ranges::advance(i, n, bound) — advance with bound, returns remainder
    //*************************************************************************
    TEST(ranges_advance_n_bound_random_access_not_reaching_bound)
    {
      int data[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };

      int* itr = &data[0];
      int* bound = &data[9];
      auto remaining = etl::ranges::advance(itr, 4, bound);
      CHECK_EQUAL(4, *itr);
      CHECK_EQUAL(0, remaining);
    }

    //*************************************************************************
    TEST(ranges_advance_n_bound_random_access_reaching_bound)
    {
      int data[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };

      int* itr = &data[0];
      int* bound = &data[3];
      auto remaining = etl::ranges::advance(itr, 7, bound);
      CHECK(itr == bound);
      CHECK_EQUAL(3, *itr);
      CHECK_EQUAL(4, remaining);
    }

    //*************************************************************************
    TEST(ranges_advance_n_bound_random_access_exact_bound)
    {
      int data[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };

      int* itr = &data[0];
      int* bound = &data[5];
      auto remaining = etl::ranges::advance(itr, 5, bound);
      CHECK(itr == bound);
      CHECK_EQUAL(5, *itr);
      CHECK_EQUAL(0, remaining);
    }

    //*************************************************************************
    TEST(ranges_advance_n_bound_random_access_backward_not_reaching_bound)
    {
      int data[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };

      int* itr = &data[9];
      int* bound = &data[0];
      auto remaining = etl::ranges::advance(itr, -4, bound);
      CHECK_EQUAL(5, *itr);
      CHECK_EQUAL(0, remaining);
    }

    //*************************************************************************
    TEST(ranges_advance_n_bound_random_access_backward_reaching_bound)
    {
      int data[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };

      int* itr = &data[5];
      int* bound = &data[2];
      auto remaining = etl::ranges::advance(itr, -7, bound);
      CHECK(itr == bound);
      CHECK_EQUAL(2, *itr);
      CHECK_EQUAL(-4, remaining);
    }

    //*************************************************************************
    TEST(ranges_advance_n_bound_non_random_forward_not_reaching_bound)
    {
      int data[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };

      non_random_iterator<int> itr(&data[0]);
      non_random_sentinel<int> bound(&data[9]);
      auto remaining = etl::ranges::advance(itr, 4, bound);
      CHECK_EQUAL(4, *itr);
      CHECK_EQUAL(0, remaining);
    }

    //*************************************************************************
    TEST(ranges_advance_n_bound_non_random_forward_reaching_bound)
    {
      int data[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };

      non_random_iterator<int> itr(&data[0]);
      non_random_sentinel<int> bound(&data[3]);
      auto remaining = etl::ranges::advance(itr, 7, bound);
      CHECK_EQUAL(3, *itr);
      CHECK_EQUAL(4, remaining);
    }

    //*************************************************************************
    TEST(ranges_advance_n_bound_non_random_backward_not_reaching_bound)
    {
      int data[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };

      non_random_iterator<int> itr(&data[9]);
      non_random_sentinel<int> bound(&data[0]);
      auto remaining = etl::ranges::advance(itr, -3, bound);
      CHECK_EQUAL(6, *itr);
      CHECK_EQUAL(0, remaining);
    }

    //*************************************************************************
    TEST(ranges_advance_n_bound_non_random_backward_reaching_bound)
    {
      int data[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };

      non_random_iterator<int> itr(&data[3]);
      non_random_sentinel<int> bound(&data[1]);
      auto remaining = etl::ranges::advance(itr, -7, bound);
      CHECK_EQUAL(1, *itr);
      CHECK_EQUAL(-5, remaining);
    }

    //*************************************************************************
    TEST(ranges_advance_n_bound_zero_steps)
    {
      int data[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };

      int* itr = &data[5];
      int* bound = &data[8];
      auto remaining = etl::ranges::advance(itr, 0, bound);
      CHECK_EQUAL(5, *itr);
      CHECK_EQUAL(0, remaining);
    }

    //*************************************************************************
    // etl::ranges::prev(i) with random access iterator
    //*************************************************************************
    TEST(ranges_prev_random_access)
    {
      int data[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };

      int* itr = &data[5];
      int* result = etl::ranges::prev(itr);
      CHECK_EQUAL(4, *result);
      CHECK_EQUAL(5, *itr); // original unchanged
    }

    //*************************************************************************
    // etl::ranges::prev(i) with bidirectional (non-random) iterator
    //*************************************************************************
    TEST(ranges_prev_non_random)
    {
      int data[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };

      non_random_iterator<int> itr(&data[5]);
      non_random_iterator<int> result = etl::ranges::prev(itr);
      CHECK_EQUAL(4, *result);
      CHECK_EQUAL(5, *itr); // original unchanged
    }

    //*************************************************************************
    // etl::ranges::prev(i, n) with random access iterator — positive n
    //*************************************************************************
    TEST(ranges_prev_n_random_access_positive)
    {
      int data[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };

      int* itr = &data[7];
      int* result = etl::ranges::prev(itr, 3);
      CHECK_EQUAL(4, *result);
      CHECK_EQUAL(7, *itr); // original unchanged
    }

    //*************************************************************************
    // etl::ranges::prev(i, n) with random access iterator — zero n
    //*************************************************************************
    TEST(ranges_prev_n_random_access_zero)
    {
      int data[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };

      int* itr = &data[5];
      int* result = etl::ranges::prev(itr, 0);
      CHECK_EQUAL(5, *result);
    }

    //*************************************************************************
    // etl::ranges::prev(i, n) with random access iterator — negative n (moves forward)
    //*************************************************************************
    TEST(ranges_prev_n_random_access_negative)
    {
      int data[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };

      int* itr = &data[3];
      int* result = etl::ranges::prev(itr, -2);
      CHECK_EQUAL(5, *result);
      CHECK_EQUAL(3, *itr); // original unchanged
    }

    //*************************************************************************
    // etl::ranges::prev(i, n) with bidirectional (non-random) iterator — positive n
    //*************************************************************************
    TEST(ranges_prev_n_non_random_positive)
    {
      int data[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };

      non_random_iterator<int> itr(&data[7]);
      non_random_iterator<int> result = etl::ranges::prev(itr, 4);
      CHECK_EQUAL(3, *result);
      CHECK_EQUAL(7, *itr); // original unchanged
    }

    //*************************************************************************
    // etl::ranges::prev(i, n) with bidirectional (non-random) iterator — zero n
    //*************************************************************************
    TEST(ranges_prev_n_non_random_zero)
    {
      int data[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };

      non_random_iterator<int> itr(&data[5]);
      non_random_iterator<int> result = etl::ranges::prev(itr, 0);
      CHECK_EQUAL(5, *result);
    }

    //*************************************************************************
    // etl::ranges::prev(i, n) with bidirectional (non-random) iterator — negative n (moves forward)
    //*************************************************************************
    TEST(ranges_prev_n_non_random_negative)
    {
      int data[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };

      non_random_iterator<int> itr(&data[3]);
      non_random_iterator<int> result = etl::ranges::prev(itr, -2);
      CHECK_EQUAL(5, *result);
      CHECK_EQUAL(3, *itr); // original unchanged
    }

    //*************************************************************************
    // etl::ranges::prev(i, n, bound) with random access iterator — not reaching bound
    //*************************************************************************
    TEST(ranges_prev_n_bound_random_access_not_reaching_bound)
    {
      int data[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };

      int* itr = &data[8];
      int* bound = &data[2];
      int* result = etl::ranges::prev(itr, 4, bound);
      CHECK_EQUAL(4, *result);
    }

    //*************************************************************************
    // etl::ranges::prev(i, n, bound) with random access iterator — reaching bound
    //*************************************************************************
    TEST(ranges_prev_n_bound_random_access_reaching_bound)
    {
      int data[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };

      int* itr = &data[5];
      int* bound = &data[3];
      int* result = etl::ranges::prev(itr, 7, bound);
      CHECK_EQUAL(3, *result);
      CHECK(result == bound);
    }

    //*************************************************************************
    // etl::ranges::prev(i, n, bound) with random access iterator — zero steps
    //*************************************************************************
    TEST(ranges_prev_n_bound_random_access_zero_steps)
    {
      int data[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };

      int* itr = &data[5];
      int* bound = &data[2];
      int* result = etl::ranges::prev(itr, 0, bound);
      CHECK_EQUAL(5, *result);
    }

    //*************************************************************************
    // etl::ranges::prev(i, n, bound) with bidirectional (non-random) iterator — not reaching bound
    //*************************************************************************
    TEST(ranges_prev_n_bound_non_random_not_reaching_bound)
    {
      int data[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };

      non_random_iterator<int> itr(&data[8]);
      non_random_iterator<int> bound(&data[2]);
      non_random_iterator<int> result = etl::ranges::prev(itr, 3, bound);
      CHECK_EQUAL(5, *result);
    }

    //*************************************************************************
    // etl::ranges::prev(i, n, bound) with bidirectional (non-random) iterator — reaching bound
    //*************************************************************************
    TEST(ranges_prev_n_bound_non_random_reaching_bound)
    {
      int data[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };

      non_random_iterator<int> itr(&data[5]);
      non_random_iterator<int> bound(&data[3]);
      non_random_iterator<int> result = etl::ranges::prev(itr, 7, bound);
      CHECK_EQUAL(3, *result);
      CHECK(result.ptr == bound.ptr);
    }

    //*************************************************************************
    // etl::ranges::next(i) with random access iterator
    //*************************************************************************
    TEST(ranges_next_random_access)
    {
      int data[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };

      int* itr = &data[5];
      int* result = etl::ranges::next(itr);
      CHECK_EQUAL(6, *result);
      CHECK_EQUAL(5, *itr); // original unchanged
    }

    //*************************************************************************
    // etl::ranges::next(i) with bidirectional (non-random) iterator
    //*************************************************************************
    TEST(ranges_next_non_random)
    {
      int data[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };

      non_random_iterator<int> itr(&data[5]);
      non_random_iterator<int> result = etl::ranges::next(itr);
      CHECK_EQUAL(6, *result);
      CHECK_EQUAL(5, *itr); // original unchanged
    }

    //*************************************************************************
    // etl::ranges::next(i, n) with random access iterator — positive n
    //*************************************************************************
    TEST(ranges_next_n_random_access_positive)
    {
      int data[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };

      int* itr = &data[2];
      int* result = etl::ranges::next(itr, 5);
      CHECK_EQUAL(7, *result);
      CHECK_EQUAL(2, *itr); // original unchanged
    }

    //*************************************************************************
    // etl::ranges::next(i, n) with random access iterator — zero n
    //*************************************************************************
    TEST(ranges_next_n_random_access_zero)
    {
      int data[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };

      int* itr = &data[5];
      int* result = etl::ranges::next(itr, 0);
      CHECK_EQUAL(5, *result);
    }

    //*************************************************************************
    // etl::ranges::next(i, n) with random access iterator — negative n (moves backward)
    //*************************************************************************
    TEST(ranges_next_n_random_access_negative)
    {
      int data[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };

      int* itr = &data[7];
      int* result = etl::ranges::next(itr, -3);
      CHECK_EQUAL(4, *result);
      CHECK_EQUAL(7, *itr); // original unchanged
    }

    //*************************************************************************
    // etl::ranges::next(i, n) with bidirectional (non-random) iterator — positive n
    //*************************************************************************
    TEST(ranges_next_n_non_random_positive)
    {
      int data[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };

      non_random_iterator<int> itr(&data[1]);
      non_random_iterator<int> result = etl::ranges::next(itr, 4);
      CHECK_EQUAL(5, *result);
      CHECK_EQUAL(1, *itr); // original unchanged
    }

    //*************************************************************************
    // etl::ranges::next(i, n) with bidirectional (non-random) iterator — zero n
    //*************************************************************************
    TEST(ranges_next_n_non_random_zero)
    {
      int data[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };

      non_random_iterator<int> itr(&data[5]);
      non_random_iterator<int> result = etl::ranges::next(itr, 0);
      CHECK_EQUAL(5, *result);
    }

    //*************************************************************************
    // etl::ranges::next(i, n) with bidirectional (non-random) iterator — negative n (moves backward)
    //*************************************************************************
    TEST(ranges_next_n_non_random_negative)
    {
      int data[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };

      non_random_iterator<int> itr(&data[7]);
      non_random_iterator<int> result = etl::ranges::next(itr, -3);
      CHECK_EQUAL(4, *result);
      CHECK_EQUAL(7, *itr); // original unchanged
    }

    //*************************************************************************
    // etl::ranges::next(i, bound) with random access iterator
    //*************************************************************************
    TEST(ranges_next_bound_random_access)
    {
      int data[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };

      int* itr = &data[0];
      int* bound = &data[7];
      int* result = etl::ranges::next(itr, bound);
      CHECK_EQUAL(7, *result);
      CHECK(result == bound);
      CHECK_EQUAL(0, *itr); // original unchanged
    }

    //*************************************************************************
    // etl::ranges::next(i, bound) with non-random iterator
    //*************************************************************************
    TEST(ranges_next_bound_non_random)
    {
      int data[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };

      non_random_iterator<int> itr(&data[0]);
      non_random_sentinel<int> bound(&data[5]);
      non_random_iterator<int> result = etl::ranges::next(itr, bound);
      CHECK_EQUAL(5, *result);
      CHECK_EQUAL(0, *itr); // original unchanged
    }

    //*************************************************************************
    // etl::ranges::next(i, bound) already at bound
    //*************************************************************************
    TEST(ranges_next_bound_already_at_bound)
    {
      int data[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };

      int* itr = &data[4];
      int* bound = &data[4];
      int* result = etl::ranges::next(itr, bound);
      CHECK(result == bound);
      CHECK_EQUAL(4, *result);
    }

    //*************************************************************************
    // etl::ranges::next(i, n, bound) with random access iterator — not reaching bound
    //*************************************************************************
    TEST(ranges_next_n_bound_random_access_not_reaching_bound)
    {
      int data[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };

      int* itr = &data[0];
      int* bound = &data[9];
      int* result = etl::ranges::next(itr, 4, bound);
      CHECK_EQUAL(4, *result);
      CHECK_EQUAL(0, *itr); // original unchanged
    }

    //*************************************************************************
    // etl::ranges::next(i, n, bound) with random access iterator — reaching bound
    //*************************************************************************
    TEST(ranges_next_n_bound_random_access_reaching_bound)
    {
      int data[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };

      int* itr = &data[0];
      int* bound = &data[3];
      int* result = etl::ranges::next(itr, 7, bound);
      CHECK(result == bound);
      CHECK_EQUAL(3, *result);
    }

    //*************************************************************************
    // etl::ranges::next(i, n, bound) with random access iterator — exact bound
    //*************************************************************************
    TEST(ranges_next_n_bound_random_access_exact_bound)
    {
      int data[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };

      int* itr = &data[0];
      int* bound = &data[5];
      int* result = etl::ranges::next(itr, 5, bound);
      CHECK(result == bound);
      CHECK_EQUAL(5, *result);
    }

    //*************************************************************************
    // etl::ranges::next(i, n, bound) with random access iterator — backward not reaching bound
    //*************************************************************************
    TEST(ranges_next_n_bound_random_access_backward_not_reaching_bound)
    {
      int data[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };

      int* itr = &data[9];
      int* bound = &data[0];
      int* result = etl::ranges::next(itr, -4, bound);
      CHECK_EQUAL(5, *result);
    }

    //*************************************************************************
    // etl::ranges::next(i, n, bound) with random access iterator — backward reaching bound
    //*************************************************************************
    TEST(ranges_next_n_bound_random_access_backward_reaching_bound)
    {
      int data[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };

      int* itr = &data[5];
      int* bound = &data[2];
      int* result = etl::ranges::next(itr, -7, bound);
      CHECK(result == bound);
      CHECK_EQUAL(2, *result);
    }

    //*************************************************************************
    // etl::ranges::next(i, n, bound) with random access iterator — zero steps
    //*************************************************************************
    TEST(ranges_next_n_bound_random_access_zero_steps)
    {
      int data[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };

      int* itr = &data[5];
      int* bound = &data[8];
      int* result = etl::ranges::next(itr, 0, bound);
      CHECK_EQUAL(5, *result);
    }

    //*************************************************************************
    // etl::ranges::next(i, n, bound) with non-random iterator — forward not reaching bound
    //*************************************************************************
    TEST(ranges_next_n_bound_non_random_forward_not_reaching_bound)
    {
      int data[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };

      non_random_iterator<int> itr(&data[0]);
      non_random_sentinel<int> bound(&data[9]);
      non_random_iterator<int> result = etl::ranges::next(itr, 4, bound);
      CHECK_EQUAL(4, *result);
    }

    //*************************************************************************
    // etl::ranges::next(i, n, bound) with non-random iterator — forward reaching bound
    //*************************************************************************
    TEST(ranges_next_n_bound_non_random_forward_reaching_bound)
    {
      int data[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };

      non_random_iterator<int> itr(&data[0]);
      non_random_sentinel<int> bound(&data[3]);
      non_random_iterator<int> result = etl::ranges::next(itr, 7, bound);
      CHECK_EQUAL(3, *result);
    }

    //*************************************************************************
    // etl::ranges::next(i, n, bound) with non-random iterator — backward not reaching bound
    //*************************************************************************
    TEST(ranges_next_n_bound_non_random_backward_not_reaching_bound)
    {
      int data[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };

      non_random_iterator<int> itr(&data[9]);
      non_random_sentinel<int> bound(&data[0]);
      non_random_iterator<int> result = etl::ranges::next(itr, -3, bound);
      CHECK_EQUAL(6, *result);
    }

    //*************************************************************************
    // etl::ranges::next(i, n, bound) with non-random iterator — backward reaching bound
    //*************************************************************************
    TEST(ranges_next_n_bound_non_random_backward_reaching_bound)
    {
      int data[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };

      non_random_iterator<int> itr(&data[3]);
      non_random_sentinel<int> bound(&data[1]);
      non_random_iterator<int> result = etl::ranges::next(itr, -7, bound);
      CHECK_EQUAL(1, *result);
    }

    TEST(test_is_range)
    {
      std::vector<int> vec;
      int arr[3]{};
      int i{};
      static_assert(etl::is_range_v<decltype(vec)> == true, "Expected range");
      static_assert(etl::is_range_v<decltype(arr)> == true, "Expected range");
      static_assert(etl::is_range_v<decltype(i)> == false, "Expected non range");
    }

#endif
  }
}
