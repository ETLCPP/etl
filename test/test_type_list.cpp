/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl
https://www.etlcpp.com

Copyright(c) 2025 BMW AG

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
  SUITE(test_type_list)
  {
    //*************************************************************************
    TEST(test_type_list_select)
    {
      typedef etl::type_list<char, int, uint32_t> t1;
      typedef etl::type_list<char, uint32_t> t2;

      CHECK_TRUE((std::is_same<etl::type_list_select<t1, 0, 2>::type, t2>::value));
      CHECK_TRUE((std::is_same<etl::type_list_select_t<t1, 0, 2>, t2>::value));
    }

    //*************************************************************************
    TEST(test_type_list_size)
    {
      typedef etl::type_list<char, int, uint32_t> t1;
      typedef etl::type_list<char, uint32_t> t2;
      typedef etl::type_list<> t3;

      CHECK_EQUAL(etl::type_list_size<t1>::value, 3);
      CHECK_EQUAL(etl::type_list_size<t2>::value, 2);
      CHECK_EQUAL(etl::type_list_size<t3>::value, 0);
    }

    //*************************************************************************
    TEST(test_type_list_cat)
    {
      typedef etl::type_list<char, int, uint32_t> t1;
      typedef etl::type_list<uint8_t, uint16_t>   t2;
      typedef etl::type_list<> t3;

      typedef etl::type_list<char, int, uint32_t, uint8_t, uint16_t> t_cat1;
      typedef etl::type_list<char, int, uint32_t, uint8_t, bool>     t_cat2;

      CHECK_TRUE((std::is_same<etl::type_list_cat<t1, t2>::type,     t_cat1>::value));
      CHECK_TRUE((std::is_same<etl::type_list_cat<t1, t2, t3>::type, t_cat1>::value));
      CHECK_FALSE((std::is_same<etl::type_list_cat<t1, t2>::type,    t_cat2>::value));

      CHECK_TRUE((std::is_same<etl::type_list_cat_t<t1, t2>,     t_cat1>::value));
      CHECK_TRUE((std::is_same<etl::type_list_cat_t<t1, t2, t3>, t_cat1>::value));
      CHECK_FALSE((std::is_same<etl::type_list_cat_t<t1, t2>,    t_cat2>::value));
    }

    //*************************************************************************
    TEST(test_type_list_contains)
    {
      typedef etl::type_list<char, int, uint32_t> t1;
      typedef etl::type_list<uint8_t, uint16_t>   t2;
      typedef etl::type_list<uint16_t> t3;
      typedef etl::type_list<> t4;

      CHECK_TRUE((etl::type_list_contains<t1,  char>::value));
      CHECK_FALSE((etl::type_list_contains<t1, uint8_t>::value));
      CHECK_FALSE((etl::type_list_contains<t2, int>::value));
      CHECK_TRUE((etl::type_list_contains<t2,  uint16_t>::value));
      CHECK_TRUE((etl::type_list_contains<t3,  uint16_t>::value));
      CHECK_FALSE((etl::type_list_contains<t3, uint32_t>::value));
      CHECK_FALSE((etl::type_list_contains<t4, uint32_t>::value));

#if ETL_USING_CPP17
      CHECK_TRUE((etl::type_list_contains_v<t1,  char>));
      CHECK_FALSE((etl::type_list_contains_v<t1, uint8_t>));
      CHECK_FALSE((etl::type_list_contains_v<t2, int>));
      CHECK_TRUE((etl::type_list_contains_v<t2,  uint16_t>));
      CHECK_TRUE((etl::type_list_contains_v<t3,  uint16_t>));
      CHECK_FALSE((etl::type_list_contains_v<t3, uint32_t>));
      CHECK_FALSE((etl::type_list_contains_v<t4, uint32_t>));
#endif
    }

    //*************************************************************************
    TEST(test_type_list_index_of_type)
    {
      typedef etl::type_list<char, int, uint32_t> t1;
      typedef etl::type_list<> t2;

      CHECK_EQUAL((etl::type_list_index_of_type<t1, char>::value), 0);
      CHECK_EQUAL((etl::type_list_index_of_type<t1, int>::value), 1);
      CHECK_EQUAL((etl::type_list_index_of_type<t1, uint32_t>::value), 2);
      CHECK_EQUAL((etl::type_list_index_of_type<t2, uint32_t>::value), etl::type_list_npos);

#if ETL_USING_CPP17
      CHECK_EQUAL((etl::type_list_index_of_v<t1, char>), 0);
      CHECK_EQUAL((etl::type_list_index_of_v<t1, int>), 1);
      CHECK_EQUAL((etl::type_list_index_of_v<t1, uint32_t>), 2);
      CHECK_EQUAL((etl::type_list_index_of_v<t2, uint32_t>), etl::type_list_npos);
#endif
    }

    //*************************************************************************
    TEST(test_type_list_type_at_index)
    {
      typedef etl::type_list<char, int, uint32_t> t1;

      CHECK_TRUE((std::is_same<char,     etl::type_list_type_at_index<t1, 0>::type>::value));
      CHECK_TRUE((std::is_same<int,      etl::type_list_type_at_index<t1, 1>::type>::value));
      CHECK_TRUE((std::is_same<uint32_t, etl::type_list_type_at_index<t1, 2>::type>::value));

      CHECK_TRUE((std::is_same<char,     etl::type_list_type_at_index_t<t1, 0>>::value));
      CHECK_TRUE((std::is_same<int,      etl::type_list_type_at_index_t<t1, 1>>::value));
      CHECK_TRUE((std::is_same<uint32_t, etl::type_list_type_at_index_t<t1, 2>>::value));
    }

    //*************************************************************************
    TEST(test_type_list_max_sizeof_type)
    {
      typedef etl::type_list<char, int16_t, uint32_t> t1;
      typedef etl::type_list<uint8_t, uint16_t> t2;
      typedef etl::type_list<uint32_t> t3;
      typedef etl::type_list<> t4;

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
      typedef etl::type_list<uint8_t, uint16_t> t2;
      typedef etl::type_list<uint16_t> t3;
      typedef etl::type_list<> t4;

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

  };
#endif
}
