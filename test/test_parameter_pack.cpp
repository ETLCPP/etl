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

#include "etl/parameter_pack.h"

namespace
{
  using Pack = etl::parameter_pack<char, short, int>;

  SUITE(test_type_lookup)
  {
    //*************************************************************************
    TEST(test_index_of_type)
    {
#if ETL_USING_CPP17
      CHECK_EQUAL(0U, Pack::index_of_type_v<char>);
      CHECK_EQUAL(1U, Pack::index_of_type_v<short>);
      CHECK_EQUAL(2U, Pack::index_of_type_v<int>);

      // Static assert
      //CHECK_EQUAL(0U, Pack::index_of_type_v<long>);

      CHECK_EQUAL(0U, (etl::parameter_pack_v<char, char, short, int>));
      CHECK_EQUAL(1U, (etl::parameter_pack_v<short, char, short, int>));
      CHECK_EQUAL(2U, (etl::parameter_pack_v<int, char, short, int>));

      // Static assert
      //CHECK_EQUAL(0U, (etl::parameter_pack_v<long, char, short, int>));
#else
      CHECK_EQUAL(0U, Pack::index_of_type<char>::value);
      CHECK_EQUAL(1U, Pack::index_of_type<short>::value);
      CHECK_EQUAL(2U, Pack::index_of_type<int>::value);

      // Static assert
      //CHECK_EQUAL(0U, Pack::index_of_type_v<long>);

      CHECK_EQUAL(0U, (etl::parameter_pack<char, short, int>::index_of_type<char>::value));
      CHECK_EQUAL(1U, (etl::parameter_pack<char, short, int>::index_of_type<short>::value));
      CHECK_EQUAL(2U, (etl::parameter_pack<char, short, int>::index_of_type<int>::value));

      // Static assert
      //CHECK_EQUAL(0U, (etl::parameter_pack_v<long, char, short, int>));
#endif
    }

    //*************************************************************************
    TEST(test_type_from_index)
    {
      CHECK((std::is_same<char,  typename Pack::type_from_index_t<0U>>::value));
      CHECK((std::is_same<short, typename Pack::type_from_index_t<1U>>::value));
      CHECK((std::is_same<int,   typename Pack::type_from_index_t<2U>>::value));

      // Static assert
      //CHECK((std::is_same_v<long, typename Pack::type_from_index_t<3U>>));

      CHECK((std::is_same<char,  etl::parameter_pack_t<0U, char, short, int>>::value));
      CHECK((std::is_same<short, etl::parameter_pack_t<1U, char, short, int>>::value));
      CHECK((std::is_same<int,   etl::parameter_pack_t<2U, char, short, int>>::value));

      // Static assert
      //CHECK((std::is_same_v<long, etl::parameter_pack_t<3U, char, short, int>>));
    }
  };
}
