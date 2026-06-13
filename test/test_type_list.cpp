/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl
https://www.etlcpp.com

Copyright(c) 2025 BMW AG, John Wellbelove

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

#include "etl/type_list.h"

#include <type_traits>

namespace
{
#if ETL_USING_CPP11

  struct A
  {
    static constexpr int id = 0;
  };
  struct B
  {
    static constexpr int id = 1;
  };
  struct C
  {
    static constexpr int id = 2;
  };

  struct D
  {
    static constexpr int id = 3;
  };
  struct E
  {
    static constexpr int id = 4;
  };
  struct F
  {
    static constexpr int id = 5;
  };

  template <typename T>
  struct is_type_a : etl::bool_constant<std::is_same<T, A>::value>
  {
  };

  template <typename T>
  struct is_type_b : etl::bool_constant<std::is_same<T, B>::value>
  {
  };

  template <typename T>
  struct is_type_c : etl::bool_constant<std::is_same<T, C>::value>
  {
  };

  template <typename T>
  struct is_type_d : etl::bool_constant<std::is_same<T, D>::value>
  {
  };

  template <typename T>
  struct is_type_e : etl::bool_constant<std::is_same<T, E>::value>
  {
  };

  template <typename T>
  struct is_type_f : etl::bool_constant<std::is_same<T, F>::value>
  {
  };

  // Convenience comparator for types that expose a constexpr integral ID
  // (ascending)
  template <typename T1, typename T2>
  struct by_ascending_id : etl::bool_constant<(T1::id < T2::id)>
  {
  };

  SUITE(test_type_list)
  {
    //*************************************************************************
    TEST(test_is_type_list)
    {
      using t0 = etl::type_list<>;
      using t1 = etl::type_list<char, int, uint32_t>;
      using t2 = int;

      CHECK_TRUE((etl::is_type_list<t0>::value));
      CHECK_TRUE((etl::is_type_list<t1>::value));
      CHECK_FALSE((etl::is_type_list<t2>::value));

  #if ETL_USING_CPP17
      CHECK_TRUE((etl::is_type_list_v<t0>));
      CHECK_TRUE((etl::is_type_list_v<t1>));
      CHECK_FALSE((etl::is_type_list_v<t2>));
  #endif
    }

    //*************************************************************************
    TEST(test_type_list_select)
    {
      typedef etl::type_list<char, int, uint32_t> t1;
      typedef etl::type_list<char, uint32_t>      t2;

      CHECK_TRUE((std::is_same<etl::type_list_select<t1, 0, 2>::type, t2>::value));
      CHECK_TRUE((std::is_same<etl::type_list_select_t<t1, 0, 2>, t2>::value));
    }

    //*************************************************************************
    TEST(test_type_list_select_from_indexes)
    {
      typedef etl::type_list<char, int, uint32_t> t1;
      typedef etl::type_list<char, uint32_t>      t2;

      CHECK_TRUE((std::is_same<etl::type_list_select_from_indexes<t1, 0, 2>::type, t2>::value));
      CHECK_TRUE((std::is_same<etl::type_list_select_from_indexes_t<t1, 0, 2>, t2>::value));
    }

    //*************************************************************************
    TEST(test_type_list_select_from_index_sequence)
    {
      typedef etl::type_list<char, int, uint32_t> t1;
      typedef etl::type_list<char, uint32_t>      t2;

      using index_sequence = etl::index_sequence<0, 2>;

      CHECK_TRUE((std::is_same< etl::type_list_select_from_index_sequence<t1, index_sequence>::type, t2>::value));
      CHECK_TRUE((std::is_same<etl::type_list_select_from_index_sequence_t<t1, index_sequence>, t2>::value));
    }

    //*************************************************************************
    TEST(test_type_list_size)
    {
      typedef etl::type_list<char, int, uint32_t> t1;
      typedef etl::type_list<char, uint32_t>      t2;
      typedef etl::type_list<>                    t3;

      CHECK_EQUAL(etl::type_list_size<t1>::value, 3);
      CHECK_EQUAL(etl::type_list_size<t2>::value, 2);
      CHECK_EQUAL(etl::type_list_size<t3>::value, 0);
    }

    //*************************************************************************
    TEST(test_type_list_cat)
    {
      typedef etl::type_list<char, uint16_t, int, uint32_t> t1;
      typedef etl::type_list<uint8_t, uint16_t, int>        t2;
      typedef etl::type_list<>                              t3;

      typedef etl::type_list<char, uint16_t, int, uint32_t, uint8_t, uint16_t, int> t_cat1;
      typedef etl::type_list<char, int, uint32_t, uint8_t, bool>                    t_cat2;

      CHECK_TRUE((std::is_same<etl::type_list_cat<t1, t2>::type, t_cat1>::value));
      CHECK_TRUE((std::is_same<etl::type_list_cat<t1, t2, t3>::type, t_cat1>::value));
      CHECK_FALSE((std::is_same<etl::type_list_cat<t1, t2>::type, t_cat2>::value));

      CHECK_TRUE((std::is_same<etl::type_list_cat_t<t1, t2>, t_cat1>::value));
      CHECK_TRUE((std::is_same<etl::type_list_cat_t<t1, t2, t3>, t_cat1>::value));
      CHECK_FALSE((std::is_same<etl::type_list_cat_t<t1, t2>, t_cat2>::value));
    }

    //*************************************************************************
    TEST(test_type_list_cat_unique)
    {
      using t1 = etl::type_list<char, uint16_t, int, uint32_t>;
      using t2 = etl::type_list<uint8_t, uint16_t, int>;
      using t3 = etl::type_list<>;

      using t_cat1 = etl::type_list<char, uint16_t, int, uint32_t, uint8_t>;
      using t_cat2 = etl::type_list<char, int, uint32_t, uint8_t, bool>;

      CHECK_TRUE((std::is_same<etl::type_list_cat_unique<t1, t2>::type, t_cat1>::value));
      CHECK_TRUE((std::is_same<etl::type_list_cat_unique<t1, t2, t3>::type, t_cat1>::value));
      CHECK_FALSE((std::is_same<etl::type_list_cat_unique<t1, t2>::type, t_cat2>::value));

      CHECK_TRUE((std::is_same<etl::type_list_cat_unique_t<t1, t2>, t_cat1>::value));
      CHECK_TRUE((std::is_same<etl::type_list_cat_unique_t<t1, t2, t3>, t_cat1>::value));
      CHECK_FALSE((std::is_same<etl::type_list_cat_unique_t<t1, t2>, t_cat2>::value));
    }

    //*************************************************************************
    TEST(test_type_list_contains)
    {
      typedef etl::type_list<char, int, uint32_t> t1;
      typedef etl::type_list<uint8_t, uint16_t>   t2;
      typedef etl::type_list<uint16_t>            t3;
      typedef etl::type_list<>                    t4;

      CHECK_TRUE((etl::type_list_contains<t1, char>::value));
      CHECK_FALSE((etl::type_list_contains<t1, uint8_t>::value));
      CHECK_FALSE((etl::type_list_contains<t2, int>::value));
      CHECK_TRUE((etl::type_list_contains<t2, uint16_t>::value));
      CHECK_TRUE((etl::type_list_contains<t3, uint16_t>::value));
      CHECK_FALSE((etl::type_list_contains<t3, uint32_t>::value));
      CHECK_FALSE((etl::type_list_contains<t4, uint32_t>::value));

  #if ETL_USING_CPP17
      CHECK_TRUE((etl::type_list_contains_v<t1, char>));
      CHECK_FALSE((etl::type_list_contains_v<t1, uint8_t>));
      CHECK_FALSE((etl::type_list_contains_v<t2, int>));
      CHECK_TRUE((etl::type_list_contains_v<t2, uint16_t>));
      CHECK_TRUE((etl::type_list_contains_v<t3, uint16_t>));
      CHECK_FALSE((etl::type_list_contains_v<t3, uint32_t>));
      CHECK_FALSE((etl::type_list_contains_v<t4, uint32_t>));
  #endif
    }

    //*************************************************************************
    TEST(test_type_list_has_duplicates_of)
    {
      typedef etl::type_list<char, int, uint32_t>        t1;
      typedef etl::type_list<uint8_t, uint16_t, uint8_t> t2;
      typedef etl::type_list<uint16_t>                   t3;
      typedef etl::type_list<>                           t4;

      CHECK_FALSE((etl::type_list_has_duplicates_of<t1, char>::value));
      CHECK_FALSE((etl::type_list_has_duplicates_of<t1, uint8_t>::value));
      CHECK_FALSE((etl::type_list_has_duplicates_of<t2, int>::value));
      CHECK_TRUE((etl::type_list_has_duplicates_of<t2, uint8_t>::value));
      CHECK_FALSE((etl::type_list_has_duplicates_of<t3, uint16_t>::value));
      CHECK_FALSE((etl::type_list_has_duplicates_of<t3, uint32_t>::value));
      CHECK_FALSE((etl::type_list_has_duplicates_of<t4, uint32_t>::value));

  #if ETL_USING_CPP17
      CHECK_FALSE((etl::type_list_has_duplicates_of_v<t1, char>));
      CHECK_FALSE((etl::type_list_has_duplicates_of_v<t1, uint8_t>));
      CHECK_FALSE((etl::type_list_has_duplicates_of_v<t2, int>));
      CHECK_TRUE((etl::type_list_has_duplicates_of_v<t2, uint8_t>));
      CHECK_FALSE((etl::type_list_has_duplicates_of_v<t3, uint16_t>));
      CHECK_FALSE((etl::type_list_has_duplicates_of_v<t3, uint32_t>));
      CHECK_FALSE((etl::type_list_has_duplicates_of_v<t4, uint32_t>));
  #endif
    }

    //*************************************************************************
    TEST(test_type_list_count_of)
    {
      typedef etl::type_list<char, int, uint32_t>        t1;
      typedef etl::type_list<uint8_t, uint16_t, uint8_t> t2;
      typedef etl::type_list<uint16_t>                   t3;
      typedef etl::type_list<>                           t4;

      CHECK_EQUAL(1, (etl::type_list_count_of<t1, char>::value));
      CHECK_EQUAL(0, (etl::type_list_count_of<t1, uint8_t>::value));
      CHECK_EQUAL(0, (etl::type_list_count_of<t2, int>::value));
      CHECK_EQUAL(2, (etl::type_list_count_of<t2, uint8_t>::value));
      CHECK_EQUAL(1, (etl::type_list_count_of<t3, uint16_t>::value));
      CHECK_EQUAL(0, (etl::type_list_count_of<t3, uint32_t>::value));
      CHECK_EQUAL(0, (etl::type_list_count_of<t4, uint32_t>::value));

  #if ETL_USING_CPP17
      CHECK_EQUAL(1, (etl::type_list_count_of_v<t1, char>));
      CHECK_EQUAL(0, (etl::type_list_count_of_v<t1, uint8_t>));
      CHECK_EQUAL(0, (etl::type_list_count_of_v<t2, int>));
      CHECK_EQUAL(2, (etl::type_list_count_of_v<t2, uint8_t>));
      CHECK_EQUAL(1, (etl::type_list_count_of_v<t3, uint16_t>));
      CHECK_EQUAL(0, (etl::type_list_count_of_v<t3, uint32_t>));
      CHECK_EQUAL(0, (etl::type_list_count_of_v<t4, uint32_t>));
  #endif
    }

    //*************************************************************************
    TEST(test_type_list_index_of_type)
    {
      typedef etl::type_list<char, int, uint32_t> t1;
      typedef etl::type_list<>                    t2;

      CHECK_EQUAL((etl::type_list_index_of_type<t1, char>::value), 0);
      CHECK_EQUAL((etl::type_list_index_of_type<t1, int>::value), 1);
      CHECK_EQUAL((etl::type_list_index_of_type<t1, uint32_t>::value), 2);
      CHECK_EQUAL((etl::type_list_index_of_type<t2, uint32_t>::value), etl::type_list_npos);

  #if ETL_USING_CPP17
      CHECK_EQUAL((etl::type_list_index_of_type_v<t1, char>), 0);
      CHECK_EQUAL((etl::type_list_index_of_type_v<t1, int>), 1);
      CHECK_EQUAL((etl::type_list_index_of_type_v<t1, uint32_t>), 2);
      CHECK_EQUAL((etl::type_list_index_of_type_v<t2, uint32_t>), etl::type_list_npos);
  #endif
    }

    //*************************************************************************
    TEST(test_type_list_indices_of_type)
    {
      using t1 = etl::type_list<char, int, char, uint32_t, char, char>;
      using t2 = etl::type_list<>;

      using indices1  = etl::type_list_indices_of_type_t<t1, char>;
      using expected1 = etl::index_sequence<0U, 2U, 4U, 5U>;

      using indices2  = etl::type_list_indices_of_type_t<t2, char>;
      using expected2 = etl::index_sequence<>;

      CHECK_TRUE((etl::is_same<indices1, expected1>::value));
      CHECK_TRUE((etl::is_same<indices2, expected2>::value));
    }

    //*************************************************************************
    TEST(test_type_list_type_at_index)
    {
      typedef etl::type_list<char, int, uint32_t> t1;

      CHECK_TRUE((std::is_same<char, etl::type_list_type_at_index<t1, 0>::type>::value));
      CHECK_TRUE((std::is_same<int, etl::type_list_type_at_index<t1, 1>::type>::value));
      CHECK_TRUE((std::is_same<uint32_t, etl::type_list_type_at_index<t1, 2>::type>::value));

      CHECK_TRUE((std::is_same<char, etl::type_list_type_at_index_t<t1, 0>>::value));
      CHECK_TRUE((std::is_same<int, etl::type_list_type_at_index_t<t1, 1>>::value));
      CHECK_TRUE((std::is_same<uint32_t, etl::type_list_type_at_index_t<t1, 2>>::value));
    }

    //*************************************************************************
    TEST(test_type_list_max_sizeof_type)
    {
      typedef etl::type_list<char, int16_t, uint32_t> t1;
      typedef etl::type_list<uint8_t, uint16_t>       t2;
      typedef etl::type_list<uint32_t>                t3;
      typedef etl::type_list<>                        t4;

      CHECK_EQUAL(etl::type_list_max_size<t1>::value, 4);
      CHECK_EQUAL(etl::type_list_max_size<t2>::value, 2);
      CHECK_EQUAL(etl::type_list_max_size<t3>::value, 4);
      CHECK_EQUAL(etl::type_list_max_size<t4>::value, 0);

  #if ETL_USING_CPP17
      CHECK_EQUAL((etl::type_list_max_size_v<t1>), 4);
      CHECK_EQUAL((etl::type_list_max_size_v<t2>), 2);
      CHECK_EQUAL((etl::type_list_max_size_v<t3>), 4);
      CHECK_EQUAL((etl::type_list_max_size_v<t4>), 0);
  #endif
    }

    //*************************************************************************
    TEST(test_type_list_max_alignment)
    {
      typedef etl::type_list<char, int16_t, uint32_t> t1;
      typedef etl::type_list<uint8_t, uint16_t>       t2;
      typedef etl::type_list<uint16_t>                t3;
      typedef etl::type_list<>                        t4;

      CHECK_EQUAL(etl::type_list_max_alignment<t1>::value, std::alignment_of<uint32_t>::value);
      CHECK_EQUAL(etl::type_list_max_alignment<t2>::value, std::alignment_of<uint16_t>::value);
      CHECK_EQUAL(etl::type_list_max_alignment<t3>::value, std::alignment_of<uint16_t>::value);
      CHECK_EQUAL(etl::type_list_max_alignment<t4>::value, 1);

  #if ETL_USING_CPP17
      CHECK_EQUAL((etl::type_list_max_alignment_v<t1>), std::alignment_of<uint32_t>::value);
      CHECK_EQUAL((etl::type_list_max_alignment_v<t2>), std::alignment_of<uint16_t>::value);
      CHECK_EQUAL((etl::type_list_max_alignment_v<t3>), std::alignment_of<uint16_t>::value);
      CHECK_EQUAL((etl::type_list_max_alignment_v<t4>), 1);
  #endif
    }

    //*************************************************************************
    TEST(test_type_lists_are_convertible)
    {
      typedef etl::type_list<char, int, uint32_t>         t1;
      typedef etl::type_list<short, long, uint64_t>       t2;
      typedef etl::type_list<char, std::string, uint32_t> t3;
      typedef etl::type_list<>                            t4;

      // Uncomment to generate static_assert error.
      // typedef etl::type_list<char,  int> t5;

      CHECK_TRUE((etl::type_lists_are_convertible<t1, t2>::value));
      CHECK_FALSE((etl::type_lists_are_convertible<t1, t3>::value));
      CHECK_TRUE((etl::type_lists_are_convertible<t4, t4>::value));

      // Uncomment to generate static_assert error.
      // CHECK_FALSE((etl::type_lists_are_convertible<t1, t5>::value));

  #if ETL_USING_CPP17
      CHECK_TRUE((etl::type_lists_are_convertible_v<t1, t2>));
      CHECK_FALSE((etl::type_lists_are_convertible_v<t1, t3>));
      CHECK_TRUE((etl::type_lists_are_convertible_v<t4, t4>));

        // Uncomment to generate static_assert error.
        // CHECK_FALSE((etl::type_lists_are_convertible_v<t1, t5>));
  #endif
    }

    //*************************************************************************
    TEST(test_type_list_sort_empty_list)
    {
      using list     = etl::type_list<>;
      using result   = etl::type_list_sort_t<list, by_ascending_id>;
      using expected = etl::type_list<>;

      CHECK((etl::is_same<result, expected>::value));
      CHECK_EQUAL(0U, etl::type_list_size<result>::value);
    }

    //*************************************************************************
    TEST(test_type_list_sort_single_list)
    {
      using list     = etl::type_list<A>;
      using result   = etl::type_list_sort_t<list, by_ascending_id>;
      using expected = etl::type_list<A>;

      CHECK((etl::is_same<result, expected>::value));
      CHECK_EQUAL(1U, etl::type_list_size<result>::value);
    }

    //*************************************************************************
    TEST(test_type_list_sort_multiple_list)
    {
      using list     = etl::type_list<E, B, F, C, A, D>;
      using result   = etl::type_list_sort_t<list, by_ascending_id>;
      using expected = etl::type_list<A, B, C, D, E, F>;

      CHECK((etl::is_same<result, expected>::value));
      CHECK_EQUAL(6U, etl::type_list_size<result>::value);
    }

    //*************************************************************************
    TEST(test_type_list_is_sorted_for_empty_list)
    {
      using list = etl::type_list<>;

      CHECK((etl::type_list_is_sorted<list, by_ascending_id>::value));
    }

    //*************************************************************************
    TEST(test_type_list_is_sorted_for_single_list)
    {
      using list = etl::type_list<A>;

      CHECK((etl::type_list_is_sorted<list, by_ascending_id>::value));
    }

    //*************************************************************************
    TEST(test_type_list_is_sorted_for_sorted_list)
    {
      using list = etl::type_list<A, B, C>;

      CHECK((etl::type_list_is_sorted<list, by_ascending_id>::value));
    }

    //*************************************************************************
    TEST(test_type_list_is_sorted_for_unsorted_list)
    {
      using list = etl::type_list<B, C, A>;

      CHECK((!etl::type_list_is_sorted<list, by_ascending_id>::value));
    }

    //*************************************************************************
    TEST(test_type_list_insert_sorted_into_empty_list)
    {
      using list     = etl::type_list<>;
      using result   = etl::type_list_insert_sorted_t<list, B, by_ascending_id>;
      using expected = etl::type_list<B>;

      CHECK((etl::is_same<result, expected>::value));
      CHECK_EQUAL(1U, etl::type_list_size<result>::value);
    }

    //*************************************************************************
    TEST(test_type_list_insert_sorted_before_head)
    {
      using list     = etl::type_list<B, C>;
      using result   = etl::type_list_insert_sorted_t<list, A, by_ascending_id>;
      using expected = etl::type_list<A, B, C>;

      CHECK((etl::is_same<result, expected>::value));
      CHECK_EQUAL(3U, etl::type_list_size<result>::value);
    }

    //*************************************************************************
    TEST(test_type_list_insert_sorted_in_middle)
    {
      using list     = etl::type_list<A, C>;
      using result   = etl::type_list_insert_sorted_t<list, B, by_ascending_id>;
      using expected = etl::type_list<A, B, C>;

      CHECK((etl::is_same<result, expected>::value));
      CHECK_EQUAL(3U, etl::type_list_size<result>::value);
    }

    //*************************************************************************
    TEST(test_type_list_insert_sorted_at_end)
    {
      using list     = etl::type_list<A, B>;
      using result   = etl::type_list_insert_sorted_t<list, C, by_ascending_id>;
      using expected = etl::type_list<A, B, C>;

      CHECK((etl::is_same<result, expected>::value));
      CHECK_EQUAL(3U, etl::type_list_size<result>::value);
    }

    //*************************************************************************
    TEST(test_type_list_push_front_to_empty_list)
    {
      using list     = etl::type_list<>;
      using result   = etl::type_list_push_front_t<list, A>;
      using expected = etl::type_list<A>;

      CHECK((etl::is_same<result, expected>::value));
      CHECK_EQUAL(1U, etl::type_list_size<result>::value);
    }

    //*************************************************************************
    TEST(test_type_list_push_front_to_non_empty_list)
    {
      using list     = etl::type_list<B, C>;
      using result   = etl::type_list_push_front_t<list, A>;
      using expected = etl::type_list<A, B, C>;

      CHECK((etl::is_same<result, expected>::value));
      CHECK_EQUAL(3U, etl::type_list_size<result>::value);
    }

    //*************************************************************************
    TEST(test_type_list_push_back_to_empty_list)
    {
      using list     = etl::type_list<>;
      using result   = etl::type_list_push_back_t<list, A>;
      using expected = etl::type_list<A>;

      CHECK((etl::is_same<result, expected>::value));
      CHECK_EQUAL(1U, etl::type_list_size<result>::value);
    }

    //*************************************************************************
    TEST(test_type_list_push_back_to_non_empty_list)
    {
      using list     = etl::type_list<A, B>;
      using result   = etl::type_list_push_back_t<list, C>;
      using expected = etl::type_list<A, B, C>;

      CHECK((etl::is_same<result, expected>::value));
      CHECK_EQUAL(3U, etl::type_list_size<result>::value);
    }

    //*************************************************************************
    TEST(test_type_list_insert_to_empty_list)
    {
      using list     = etl::type_list<>;
      using result   = etl::type_list_insert_t<list, A, 0>;
      using expected = etl::type_list<A>;

      CHECK((etl::is_same<result, expected>::value));
      CHECK_EQUAL(1U, etl::type_list_size<result>::value);
    }

    //*************************************************************************
    TEST(test_type_list_insert_to_non_empty_list)
    {
      using list      = etl::type_list<A, B>;
      using result1   = etl::type_list_insert_t<list, C, 0>;
      using result2   = etl::type_list_insert_t<list, C, 1>;
      using result3   = etl::type_list_insert_t<list, C, 2>;
      using expected1 = etl::type_list<C, A, B>;
      using expected2 = etl::type_list<A, C, B>;
      using expected3 = etl::type_list<A, B, C>;

      CHECK((etl::is_same<result1, expected1>::value));
      CHECK((etl::is_same<result2, expected2>::value));
      CHECK((etl::is_same<result3, expected3>::value));

      CHECK_EQUAL(3U, etl::type_list_size<result1>::value);
      CHECK_EQUAL(3U, etl::type_list_size<result2>::value);
      CHECK_EQUAL(3U, etl::type_list_size<result3>::value);
    }

    //*************************************************************************
    TEST(test_type_list_remove_from_empty_list)
    {
      // Uncomment to generate static_assert error.

      // using list     = etl::type_list<>;
      // using result1  = etl::type_list_remove_t<list, 0>;
    }

    //*************************************************************************
    TEST(test_type_list_remove_from_single_list)
    {
      using list      = etl::type_list<A>;
      using result1   = etl::type_list_remove_t<list, 0>;
      using expected1 = etl::type_list<>;

      CHECK((etl::is_same<result1, expected1>::value));

      CHECK_EQUAL(0U, etl::type_list_size<result1>::value);
    }

    //*************************************************************************
    TEST(test_type_list_remove_from_multiple_list)
    {
      using list      = etl::type_list<A, B, C>;
      using result1   = etl::type_list_remove_t<list, 0>;
      using result2   = etl::type_list_remove_t<list, 1>;
      using result3   = etl::type_list_remove_t<list, 2>;
      using expected1 = etl::type_list<B, C>;
      using expected2 = etl::type_list<A, C>;
      using expected3 = etl::type_list<A, B>;

      CHECK((etl::is_same<result1, expected1>::value));
      CHECK((etl::is_same<result2, expected2>::value));
      CHECK((etl::is_same<result3, expected3>::value));

      CHECK_EQUAL(2U, etl::type_list_size<result1>::value);
      CHECK_EQUAL(2U, etl::type_list_size<result2>::value);
      CHECK_EQUAL(2U, etl::type_list_size<result3>::value);
    }

    //*************************************************************************
    TEST(test_type_list_remove_if_from_empty_list)
    {
      using list      = etl::type_list<>;
      using result1   = etl::type_list_remove_if_t<list, is_type_b>;
      using expected1 = etl::type_list<>;

      CHECK((etl::is_same<result1, expected1>::value));

      CHECK_EQUAL(0U, etl::type_list_size<result1>::value);
    }

    //*************************************************************************
    TEST(test_type_list_remove_if_from_single_list)
    {
      using list1     = etl::type_list<A>;
      using list2     = etl::type_list<B>;
      using result1   = etl::type_list_remove_if_t<list1, is_type_b>;
      using result2   = etl::type_list_remove_if_t<list2, is_type_b>;
      using expected1 = etl::type_list<A>;
      using expected2 = etl::type_list<>;

      CHECK((etl::is_same<result1, expected1>::value));
      CHECK((etl::is_same<result2, expected2>::value));

      CHECK_EQUAL(1U, etl::type_list_size<result1>::value);
      CHECK_EQUAL(0U, etl::type_list_size<result2>::value);
    }

    //*************************************************************************
    TEST(test_type_list_remove_if_from_multiple_list)
    {
      using list      = etl::type_list<A, B, C>;
      using result1   = etl::type_list_remove_if_t<list, is_type_b>;
      using expected1 = etl::type_list<A, C>;

      CHECK((etl::is_same<result1, expected1>::value));

      CHECK_EQUAL(2U, etl::type_list_size<result1>::value);
    }

    //*************************************************************************
    TEST(test_type_list_pop_front_empty_list)
    {
      // Uncomment to generate static_assert error.

      // using list     = etl::type_list<>;
      // using result1  = etl::type_list_pop_front_t<list>;
    }

    //*************************************************************************
    TEST(test_type_list_pop_front_from_non_empty_list)
    {
      using list      = etl::type_list<A, B, C>;
      using result1   = etl::type_list_pop_front_t<list>;
      using result2   = etl::type_list_pop_front_t<result1>;
      using result3   = etl::type_list_pop_front_t<result2>;
      using expected1 = etl::type_list<B, C>;
      using expected2 = etl::type_list<C>;
      using expected3 = etl::type_list<>;

      CHECK((etl::is_same<result1, expected1>::value));
      CHECK((etl::is_same<result2, expected2>::value));
      CHECK((etl::is_same<result3, expected3>::value));

      CHECK_EQUAL(2U, etl::type_list_size<result1>::value);
      CHECK_EQUAL(1U, etl::type_list_size<result2>::value);
      CHECK_EQUAL(0U, etl::type_list_size<result3>::value);
    }

    //*************************************************************************
    TEST(test_type_list_pop_back_empty_list)
    {
      // Uncomment to generate static_assert error.

      // using list     = etl::type_list<>;
      // using result1  = etl::type_list_pop_back_t<list>;
    }

    //*************************************************************************
    TEST(test_type_list_pop_back_from_non_empty_list)
    {
      using list      = etl::type_list<A, B, C>;
      using result1   = etl::type_list_pop_back_t<list>;
      using result2   = etl::type_list_pop_back_t<result1>;
      using result3   = etl::type_list_pop_back_t<result2>;
      using expected1 = etl::type_list<A, B>;
      using expected2 = etl::type_list<A>;
      using expected3 = etl::type_list<>;

      CHECK((etl::is_same<result1, expected1>::value));
      CHECK((etl::is_same<result2, expected2>::value));
      CHECK((etl::is_same<result3, expected3>::value));

      CHECK_EQUAL(2U, etl::type_list_size<result1>::value);
      CHECK_EQUAL(1U, etl::type_list_size<result2>::value);
      CHECK_EQUAL(0U, etl::type_list_size<result3>::value);
    }

    //*************************************************************************
    TEST(test_type_list_unique_for_empty_list)
    {
      using list     = etl::type_list<>;
      using result   = etl::type_list_unique_t<list>;
      using expected = etl::type_list<>;

      CHECK((etl::is_same<result, expected>::value));
      CHECK_EQUAL(0U, etl::type_list_size<result>::value);
    }

    //*************************************************************************
    TEST(test_type_list_unique_for_non_empty_list)
    {
      using list      = etl::type_list<A, B, A, C, C, B, C>;
      using result1   = etl::type_list_unique_t<list>;
      using expected1 = etl::type_list<A, B, C>;

      CHECK((etl::is_same<result1, expected1>::value));

      CHECK_EQUAL(3U, etl::type_list_size<result1>::value);
    }

    //*************************************************************************
    TEST(test_type_list_all_of_for_empty_list)
    {
      using list1 = etl::type_list<>;

      CHECK_TRUE((etl::type_list_all_of<list1, is_type_a>::value));
      CHECK_TRUE((etl::type_list_all_of<list1, is_type_b>::value));
      CHECK_TRUE((etl::type_list_all_of<list1, is_type_c>::value));

  #if ETL_USING_CPP17
      CHECK_TRUE((etl::type_list_all_of_v<list1, is_type_a>));
      CHECK_TRUE((etl::type_list_all_of_v<list1, is_type_b>));
      CHECK_TRUE((etl::type_list_all_of_v<list1, is_type_c>));
  #endif
    }

    //*************************************************************************
    TEST(test_type_list_all_of_for_non_empty_list)
    {
      using list1 = etl::type_list<A, B, C>;
      using list2 = etl::type_list<B, B, B>;

      CHECK_FALSE((etl::type_list_all_of<list1, is_type_a>::value));
      CHECK_FALSE((etl::type_list_all_of<list1, is_type_b>::value));
      CHECK_FALSE((etl::type_list_all_of<list1, is_type_c>::value));

      CHECK_FALSE((etl::type_list_all_of<list2, is_type_a>::value));
      CHECK_TRUE((etl::type_list_all_of<list2, is_type_b>::value));
      CHECK_FALSE((etl::type_list_all_of<list2, is_type_c>::value));

  #if ETL_USING_CPP17
      CHECK_FALSE((etl::type_list_all_of_v<list1, is_type_a>));
      CHECK_FALSE((etl::type_list_all_of_v<list1, is_type_b>));
      CHECK_FALSE((etl::type_list_all_of_v<list1, is_type_c>));

      CHECK_FALSE((etl::type_list_all_of_v<list2, is_type_a>));
      CHECK_TRUE((etl::type_list_all_of_v<list2, is_type_b>));
      CHECK_FALSE((etl::type_list_all_of_v<list2, is_type_c>));
  #endif
    }

    //*************************************************************************
    TEST(test_type_list_any_of_for_empty_list)
    {
      using list1 = etl::type_list<>;

      CHECK_FALSE((etl::type_list_any_of<list1, is_type_a>::value));
      CHECK_FALSE((etl::type_list_any_of<list1, is_type_b>::value));
      CHECK_FALSE((etl::type_list_any_of<list1, is_type_c>::value));

  #if ETL_USING_CPP17
      CHECK_FALSE((etl::type_list_any_of_v<list1, is_type_a>));
      CHECK_FALSE((etl::type_list_any_of_v<list1, is_type_b>));
      CHECK_FALSE((etl::type_list_any_of_v<list1, is_type_c>));
  #endif
    }

    //*************************************************************************
    TEST(test_type_list_any_of_for_non_empty_list)
    {
      using list1 = etl::type_list<A, B, C>;
      using list2 = etl::type_list<B, B, B>;

      CHECK_TRUE((etl::type_list_any_of<list1, is_type_a>::value));
      CHECK_TRUE((etl::type_list_any_of<list1, is_type_b>::value));
      CHECK_TRUE((etl::type_list_any_of<list1, is_type_c>::value));

      CHECK_FALSE((etl::type_list_any_of<list2, is_type_a>::value));
      CHECK_TRUE((etl::type_list_any_of<list2, is_type_b>::value));
      CHECK_FALSE((etl::type_list_any_of<list2, is_type_c>::value));

  #if ETL_USING_CPP17
      CHECK_TRUE((etl::type_list_any_of_v<list1, is_type_a>));
      CHECK_TRUE((etl::type_list_any_of_v<list1, is_type_b>));
      CHECK_TRUE((etl::type_list_any_of_v<list1, is_type_c>));

      CHECK_FALSE((etl::type_list_any_of_v<list2, is_type_a>));
      CHECK_TRUE((etl::type_list_any_of_v<list2, is_type_b>));
      CHECK_FALSE((etl::type_list_any_of_v<list2, is_type_c>));
  #endif
    }

    //*************************************************************************
    TEST(test_type_list_none_of_for_empty_list)
    {
      using list1 = etl::type_list<>;

      CHECK_TRUE((etl::type_list_none_of<list1, is_type_a>::value));
      CHECK_TRUE((etl::type_list_none_of<list1, is_type_b>::value));
      CHECK_TRUE((etl::type_list_none_of<list1, is_type_c>::value));

  #if ETL_USING_CPP17
      CHECK_TRUE((etl::type_list_none_of_v<list1, is_type_a>));
      CHECK_TRUE((etl::type_list_none_of_v<list1, is_type_b>));
      CHECK_TRUE((etl::type_list_none_of_v<list1, is_type_c>));
  #endif
    }

    //*************************************************************************
    TEST(test_type_list_none_of_for_non_empty_list)
    {
      using list1 = etl::type_list<A, B, C>;
      using list2 = etl::type_list<B, B, B>;

      CHECK_FALSE((etl::type_list_none_of<list1, is_type_a>::value));
      CHECK_FALSE((etl::type_list_none_of<list1, is_type_b>::value));
      CHECK_FALSE((etl::type_list_none_of<list1, is_type_c>::value));

      CHECK_TRUE((etl::type_list_none_of<list2, is_type_a>::value));
      CHECK_FALSE((etl::type_list_none_of<list2, is_type_b>::value));
      CHECK_TRUE((etl::type_list_none_of<list2, is_type_c>::value));

  #if ETL_USING_CPP17
      CHECK_FALSE((etl::type_list_none_of_v<list1, is_type_a>));
      CHECK_FALSE((etl::type_list_none_of_v<list1, is_type_b>));
      CHECK_FALSE((etl::type_list_none_of_v<list1, is_type_c>));

      CHECK_TRUE((etl::type_list_none_of_v<list2, is_type_a>));
      CHECK_FALSE((etl::type_list_none_of_v<list2, is_type_b>));
      CHECK_TRUE((etl::type_list_none_of_v<list2, is_type_c>));
  #endif
    }

    //*************************************************************************
    TEST(test_type_list_is_unique_for_empty_list)
    {
      using list1 = etl::type_list<>;

      CHECK_TRUE((etl::type_list_is_unique<list1>::value));

  #if ETL_USING_CPP17
      CHECK_TRUE((etl::type_list_is_unique_v<list1>));
  #endif
    }

    //*************************************************************************
    TEST(test_type_list_is_unique_for_non_empty_list)
    {
      using list1 = etl::type_list<A, B, C>;
      using list2 = etl::type_list<A, B, A>;

      CHECK_TRUE((etl::type_list_is_unique<list1>::value));
      CHECK_FALSE((etl::type_list_is_unique<list2>::value));

  #if ETL_USING_CPP17
      CHECK_TRUE((etl::type_list_is_unique_v<list1>));
      CHECK_FALSE((etl::type_list_is_unique_v<list2>));
  #endif
    }

    //*************************************************************************
    TEST(test_type_list_is_empty)
    {
      using list1 = etl::type_list<>;
      using list2 = etl::type_list<A>;

      CHECK_TRUE((etl::type_list_is_empty<list1>::value));
      CHECK_FALSE((etl::type_list_is_empty<list2>::value));

  #if ETL_USING_CPP17
      CHECK_TRUE((etl::type_list_is_empty_v<list1>));
      CHECK_FALSE((etl::type_list_is_empty_v<list2>));
  #endif
    }

    //*************************************************************************
    TEST(test_type_list_in_all_lists)
    {
      using list1 = etl::type_list<A, B, F, C>;
      using list2 = etl::type_list<B, E, D>;
      using list3 = etl::type_list<A, B, E, D, E, F>;
      using list4 = etl::type_list<>;

  #if ETL_USING_CPP17
      constexpr bool type_list_in_all_lists0 = etl::type_list_in_all_lists_v<A>;
      constexpr bool type_list_in_all_lists1 = etl::type_list_in_all_lists_v<A, list1>;
      constexpr bool type_list_in_all_lists2 = etl::type_list_in_all_lists_v<A, list2>;
      constexpr bool type_list_in_all_lists3 = etl::type_list_in_all_lists_v<A, list1, list3>;
      constexpr bool type_list_in_all_lists4 = etl::type_list_in_all_lists_v<A, list4>;
      constexpr bool type_list_in_all_lists5 = etl::type_list_in_all_lists_v<A, list1, list2, list3>;
      constexpr bool type_list_in_all_lists6 = etl::type_list_in_all_lists_v<A, list1, list2, list3, list4>;
  #else
      constexpr bool type_list_in_all_lists0 = etl::type_list_in_all_lists<A>::value;
      constexpr bool type_list_in_all_lists1 = etl::type_list_in_all_lists<A, list1>::value;
      constexpr bool type_list_in_all_lists2 = etl::type_list_in_all_lists<A, list2>::value;
      constexpr bool type_list_in_all_lists3 = etl::type_list_in_all_lists<A, list1, list3>::value;
      constexpr bool type_list_in_all_lists4 = etl::type_list_in_all_lists<A, list4>::value;
      constexpr bool type_list_in_all_lists5 = etl::type_list_in_all_lists<A, list1, list2, list3>::value;
      constexpr bool type_list_in_all_lists6 = etl::type_list_in_all_lists<A, list1, list2, list3, list4>::value;
  #endif

      CHECK_FALSE(type_list_in_all_lists0);
      CHECK_TRUE(type_list_in_all_lists1);
      CHECK_FALSE(type_list_in_all_lists2);
      CHECK_TRUE(type_list_in_all_lists3);
      CHECK_FALSE(type_list_in_all_lists4);
      CHECK_FALSE(type_list_in_all_lists5);
      CHECK_FALSE(type_list_in_all_lists6);
    }

    //*************************************************************************
    TEST(test_type_list_in_any_list)
    {
      using list1 = etl::type_list<A, B, F, C>;
      using list2 = etl::type_list<B, E, D>;
      using list3 = etl::type_list<A, B, E, D, E, F>;
      using list4 = etl::type_list<>;

  #if ETL_USING_CPP17
      constexpr bool type_list_in_any_list0 = etl::type_list_in_any_list_v<A>;
      constexpr bool type_list_in_any_list1 = etl::type_list_in_any_list_v<A, list1>;
      constexpr bool type_list_in_any_list2 = etl::type_list_in_any_list_v<A, list2>;
      constexpr bool type_list_in_any_list3 = etl::type_list_in_any_list_v<A, list1, list3>;
      constexpr bool type_list_in_any_list4 = etl::type_list_in_any_list_v<A, list4>;
      constexpr bool type_list_in_any_list5 = etl::type_list_in_any_list_v<A, list1, list2, list3>;
      constexpr bool type_list_in_any_list6 = etl::type_list_in_any_list_v<A, list1, list2, list3, list4>;
  #else
      constexpr bool type_list_in_any_list0 = etl::type_list_in_any_list<A>::value;
      constexpr bool type_list_in_any_list1 = etl::type_list_in_any_list<A, list1>::value;
      constexpr bool type_list_in_any_list2 = etl::type_list_in_any_list<A, list2>::value;
      constexpr bool type_list_in_any_list3 = etl::type_list_in_any_list<A, list1, list3>::value;
      constexpr bool type_list_in_any_list4 = etl::type_list_in_any_list<A, list4>::value;
      constexpr bool type_list_in_any_list5 = etl::type_list_in_any_list<A, list1, list2, list3>::value;
      constexpr bool type_list_in_any_list6 = etl::type_list_in_any_list<A, list1, list2, list3, list4>::value;
  #endif

      CHECK_FALSE(type_list_in_any_list0);
      CHECK_TRUE(type_list_in_any_list1);
      CHECK_FALSE(type_list_in_any_list2);
      CHECK_TRUE(type_list_in_any_list3);
      CHECK_FALSE(type_list_in_any_list4);
      CHECK_TRUE(type_list_in_any_list5);
      CHECK_TRUE(type_list_in_any_list6);
    }

    //*************************************************************************
    TEST(test_type_list_in_no_lists)
    {
      using list1 = etl::type_list<A, B, F, C>;
      using list2 = etl::type_list<B, E, D>;
      using list3 = etl::type_list<A, B, E, D, E, F>;
      using list4 = etl::type_list<>;

  #if ETL_USING_CPP17
      constexpr bool type_list_in_no_lists0 = etl::type_list_in_no_lists_v<A>;
      constexpr bool type_list_in_no_lists1 = etl::type_list_in_no_lists_v<A, list1>;
      constexpr bool type_list_in_no_lists2 = etl::type_list_in_no_lists_v<A, list2>;
      constexpr bool type_list_in_no_lists3 = etl::type_list_in_no_lists_v<A, list1, list3>;
      constexpr bool type_list_in_no_lists4 = etl::type_list_in_no_lists_v<A, list4>;
      constexpr bool type_list_in_no_lists5 = etl::type_list_in_no_lists_v<A, list1, list2, list3>;
      constexpr bool type_list_in_no_lists6 = etl::type_list_in_no_lists_v<A, list1, list2, list3, list4>;
  #else
      constexpr bool type_list_in_no_lists0 = etl::type_list_in_no_lists<A>::value;
      constexpr bool type_list_in_no_lists1 = etl::type_list_in_no_lists<A, list1>::value;
      constexpr bool type_list_in_no_lists2 = etl::type_list_in_no_lists<A, list2>::value;
      constexpr bool type_list_in_no_lists3 = etl::type_list_in_no_lists<A, list1, list3>::value;
      constexpr bool type_list_in_no_lists4 = etl::type_list_in_no_lists<A, list4>::value;
      constexpr bool type_list_in_no_lists5 = etl::type_list_in_no_lists<A, list1, list2, list3>::value;
      constexpr bool type_list_in_no_lists6 = etl::type_list_in_no_lists<A, list1, list2, list3, list4>::value;
  #endif

      CHECK_TRUE(type_list_in_no_lists0);
      CHECK_FALSE(type_list_in_no_lists1);
      CHECK_TRUE(type_list_in_no_lists2);
      CHECK_FALSE(type_list_in_no_lists3);
      CHECK_TRUE(type_list_in_no_lists4);
      CHECK_FALSE(type_list_in_no_lists5);
      CHECK_FALSE(type_list_in_no_lists6);
    }

    //*************************************************************************
    TEST(test_type_list_cat_unique_of_no_list)
    {
      using type_list_cat_unique = etl::type_list_cat_unique_t<>;

      CHECK_TRUE((etl::is_same<type_list_cat_unique, etl::type_list<>>::value));
    }

    //*************************************************************************
    TEST(test_type_list_cat_unique_of_empty_list)
    {
      using list1 = etl::type_list<>;

      using type_list_cat_unique = etl::type_list_cat_unique_t<list1>;

      CHECK_TRUE((etl::is_same<type_list_cat_unique, etl::type_list<>>::value));
    }

    //*************************************************************************
    TEST(test_type_list_cat_unique_of_1_list)
    {
      using list1 = etl::type_list<A, D, A, B, F, C>;

      using type_list_cat_unique = etl::type_list_cat_unique_t<list1>;

      CHECK_TRUE(etl::type_list_is_unique<type_list_cat_unique>::value);
      CHECK_TRUE((etl::is_same<type_list_cat_unique, etl::type_list<A, D, B, F, C>>::value));
    }

    //*************************************************************************
    TEST(test_type_list_cat_unique_of_3_lists)
    {
      using list1 = etl::type_list<A, D, A, B, F, C>;
      using list2 = etl::type_list<>;
      using list3 = etl::type_list<A, B, E, D, E, F>;

      using type_list_cat_unique = etl::type_list_cat_unique_t<list1, list2, list3>;

      CHECK_TRUE(etl::type_list_is_unique<type_list_cat_unique>::value);
      CHECK_TRUE((etl::is_same<type_list_cat_unique, etl::type_list<A, D, B, F, C, E>>::value));
    }

    //*************************************************************************
    TEST(test_type_list_select_common_types_of_no_list)
    {
      using type_list_select_common_types = etl::type_list_select_common_types_t<>;

      CHECK_TRUE((etl::is_same<type_list_select_common_types, etl::type_list<>>::value));
    }

    //*************************************************************************
    TEST(test_type_list_select_common_types_of_empty_list)
    {
      using list1 = etl::type_list<>;

      using type_list_select_common_types = etl::type_list_select_common_types_t<list1>;

      CHECK_TRUE((etl::is_same<type_list_select_common_types, etl::type_list<>>::value));
    }

    //*************************************************************************
    TEST(test_type_list_select_common_types_of_1_list)
    {
      using list1 = etl::type_list<A, D, A, B, F, C>;

      using type_list_select_common_types = etl::type_list_select_common_types_t<list1>;

      CHECK_TRUE(etl::type_list_is_unique<type_list_select_common_types>::value);
      CHECK_TRUE((etl::is_same<type_list_select_common_types, etl::type_list<A, D, B, F, C>>::value));
    }

    //*************************************************************************
    TEST(test_type_list_select_common_types_of_3_lists)
    {
      using list1 = etl::type_list<A, D, A, B, F, C>;
      using list2 = etl::type_list<A, D, B, F, B>;
      using list3 = etl::type_list<A, B, E, D, E, F>;

      using type_list_select_common_types = etl::type_list_select_common_types_t<list1, list2, list3>;

      CHECK_TRUE(etl::type_list_is_unique<type_list_select_common_types>::value);
      CHECK_TRUE((etl::is_same<type_list_select_common_types, etl::type_list<A, D, B, F>>::value));
    }

    //*************************************************************************
    TEST(test_type_list_select_not_common_types_1)
    {
      using list1 = etl::type_list<A, B, C, D, E, F>;

      using type_list_select_not_common_types = etl::type_list_select_not_common_types_t<list1>;

      CHECK_TRUE(etl::type_list_is_unique<type_list_select_not_common_types>::value);
      CHECK_TRUE((etl::is_same<type_list_select_not_common_types, etl::type_list<>>::value));
    }

    //*************************************************************************
    TEST(test_type_list_select_not_common_types_2)
    {
      using list1 = etl::type_list<A, B, C, D, E, F>;
      using list2 = etl::type_list<A, B, C, D, E, F>;

      using type_list_select_not_common_types = etl::type_list_select_not_common_types_t<list1, list2>;

      CHECK_TRUE(etl::type_list_is_unique<type_list_select_not_common_types>::value);
      CHECK_TRUE((etl::is_same<type_list_select_not_common_types, etl::type_list<>>::value));

      CHECK_TRUE((std::is_same<etl::type_list_select_not_common_types_t<list1, list2>, etl::type_list<>>::value));
    }

    //*************************************************************************
    TEST(test_type_list_select_not_common_types_3)
    {
      using list1 = etl::type_list<A, B, C, D, E, F>;
      using list2 = etl::type_list<A, B, D, F>;
      using list3 = etl::type_list<B, C, D, E, F>;

      using type_list_select_not_common_types = etl::type_list_select_not_common_types_t<list1, list2, list3>;

      CHECK_TRUE(etl::type_list_is_unique<type_list_select_not_common_types>::value);
      CHECK_TRUE((etl::is_same<type_list_select_not_common_types, etl::type_list<A, C, E>>::value));
    }

    //*************************************************************************
    TEST(test_type_list_remove_from_1_list)
    {
      using list1 = etl::type_list<A, D, A, B, F, C>;

      using type_list_remove_from = etl::type_list_remove_from_t<list1>;

      CHECK_FALSE(etl::type_list_is_unique<type_list_remove_from>::value);
      CHECK_TRUE((etl::is_same<type_list_remove_from, etl::type_list<A, D, A, B, F, C>>::value));
    }

    //*************************************************************************
    TEST(test_type_list_remove_from_2_list)
    {
      using list1 = etl::type_list<A, D, A, B, F, C>;
      using list2 = etl::type_list<D, B>;

      using type_list_remove_from = etl::type_list_remove_from_t<list1, list2>;

      CHECK_FALSE(etl::type_list_is_unique<type_list_remove_from>::value);
      CHECK_TRUE((etl::is_same<type_list_remove_from, etl::type_list<A, A, F, C>>::value));
    }

    //*************************************************************************
    TEST(test_type_list_remove_from_3_lists)
    {
      using list1 = etl::type_list<A, D, A, B, F, C>;
      using list2 = etl::type_list<A, D, B, F, B>;
      using list3 = etl::type_list<A, E, D, E, F>;

      using type_list_remove_from = etl::type_list_remove_from_t<list1, list2, list3>;

      CHECK_TRUE(etl::type_list_is_unique<type_list_remove_from>::value);
      CHECK_TRUE((etl::is_same<type_list_remove_from, etl::type_list<C>>::value));
    }
  }
#endif
} // namespace
