/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl
https://www.etlcpp.com

Copyright(c) 2021 John Wellbelove

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
  SUITE(test_type_list)
  {
    //*************************************************************************
    TEST(test_type_list_nth_type)
    {
      using tl1 = etl::type_list<char>;
      using tl2 = etl::type_list<char, short>;
      using tl3 = etl::type_list<char, short, int>;
      using tl4 = etl::type_list<char, char,  char, char>;

      CHECK_TRUE((std::is_same<char, etl::nth_type_t<0, tl1>>::value));

      CHECK_TRUE((std::is_same<char,  etl::nth_type_t<0, tl2>>::value));
      CHECK_TRUE((std::is_same<short, etl::nth_type_t<1, tl2>>::value));

      CHECK_TRUE((std::is_same<char,  etl::nth_type_t<0, tl3>>::value));
      CHECK_TRUE((std::is_same<short, etl::nth_type_t<1, tl3>>::value));
      CHECK_TRUE((std::is_same<int,   etl::nth_type_t<2, tl3>>::value));

      CHECK_TRUE((std::is_same<char, etl::nth_type_t<0, tl4>>::value));
      CHECK_TRUE((std::is_same<char, etl::nth_type_t<1, tl4>>::value));
      CHECK_TRUE((std::is_same<char, etl::nth_type_t<2, tl4>>::value));
      CHECK_TRUE((std::is_same<char, etl::nth_type_t<3, tl4>>::value));
    }

    //*************************************************************************
    TEST(test_type_list_size_function)
    {
      using tl0 = etl::type_list<>;
      using tl1 = etl::type_list<char>;
      using tl2 = etl::type_list<char, short>;
      using tl3 = etl::type_list<char, short, int>;

      CHECK_EQUAL(0, tl0::size);
      CHECK_EQUAL(1, tl1::size);
      CHECK_EQUAL(2, tl2::size);
      CHECK_EQUAL(3, tl3::size);
    }

    //*************************************************************************
    TEST(test_type_list_size_struct)
    {
      using tl0 = etl::type_list<>;
      using tl1 = etl::type_list<char>;
      using tl2 = etl::type_list<char, short>;
      using tl3 = etl::type_list<char, short, int>;

      CHECK_EQUAL(0, etl::type_list_size<tl0>::value);
      CHECK_EQUAL(1, etl::type_list_size<tl1>::value);
      CHECK_EQUAL(2, etl::type_list_size<tl2>::value);
      CHECK_EQUAL(3, etl::type_list_size<tl3>::value);
    }

    //*************************************************************************
    TEST(test_unique_selected_type_list)
    {
      using original_type_list = etl::type_list<int, double, char>;
      using new_type_list      = etl::type_list_select_t<original_type_list ,2, 0, 1>;

      using original_type_list = etl::type_list<int, double, char>;
      using new_type_list      = etl::type_list_select_t<original_type_list, 2, 0, 1>;

      CHECK_TRUE((std::is_same<etl::type_list<char, int, double>, new_type_list>::value));

      CHECK_TRUE((std::is_same<char,   etl::nth_type_t<0, new_type_list>>::value));
      CHECK_TRUE((std::is_same<int,    etl::nth_type_t<1, new_type_list>>::value));
      CHECK_TRUE((std::is_same<double, etl::nth_type_t<2, new_type_list>>::value));
    }

    //*************************************************************************
    TEST(test_multiple_selected_type_list)
    {
      using original_type_list = etl::type_list<int, double, char>;
      using new_type_list      = etl::type_list_select_t<original_type_list, 2, 1, 1>;

      CHECK_TRUE((std::is_same<etl::type_list<char, double, double>, new_type_list>::value));

      CHECK_TRUE((std::is_same<char,   etl::nth_type_t<0, new_type_list>>::value));
      CHECK_TRUE((std::is_same<double, etl::nth_type_t<1, new_type_list>>::value));
      CHECK_TRUE((std::is_same<double, etl::nth_type_t<2, new_type_list>>::value));
    }

    //*************************************************************************
    TEST(test_partially_selected_type_list)
    {
      using original_type_list  = etl::type_list<int, double, char>;
      using new_type_list = etl::type_list_select_t<original_type_list, 2, 1>;

      CHECK_TRUE((std::is_same<char,   etl::nth_type_t<0, new_type_list>>::value));
      CHECK_TRUE((std::is_same<double, etl::nth_type_t<1, new_type_list>>::value));
    }

    //*************************************************************************
    TEST(test_concatonated_type_list)
    {
      using type_list1 = etl::type_list<char,  unsigned char>;
      using type_list2 = etl::type_list<>;
      using type_list3 = etl::type_list<int,   unsigned int>;
      using concatonated_type_list = etl::type_list_cat<type_list1, type_list2, type_list3>::type;

      CHECK_TRUE((std::is_same<char,          etl::nth_type_t<0, concatonated_type_list>>::value));
      CHECK_TRUE((std::is_same<unsigned char, etl::nth_type_t<1, concatonated_type_list>>::value));
      CHECK_TRUE((std::is_same<int,           etl::nth_type_t<2, concatonated_type_list>>::value));
      CHECK_TRUE((std::is_same<unsigned int,  etl::nth_type_t<3, concatonated_type_list>>::value));
    }

    //*************************************************************************
    TEST(test_type_list_index_sequence_type)
    {
      using type_list1 = etl::type_list<char, short, int>;

      using expected_index_sequence = etl::make_index_sequence<type_list1::size>;

      CHECK_TRUE((std::is_same<expected_index_sequence, type_list1::index_sequence_type>::value));
    }
  };
}
