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

#include "etl/container.h"

#include <list>

#if ETL_NOT_USING_STL

namespace
{
  SUITE(test_container)
  {
    //*************************************************************************
    TEST(test_stl_style_container)
    {
      const size_t SIZE = 10UL;
      std::list<int> data(SIZE);

      std::list<int>::iterator iBegin = etl::begin(data);
      CHECK(data.begin() == iBegin);

      std::list<int>::iterator iEnd = etl::end(data);
      CHECK(data.end() == iEnd);
    }

    //*************************************************************************
    TEST(test_const_stl_style_container)
    {
      const size_t SIZE = 10UL;
      const std::list<int> data(SIZE);

      std::list<int>::const_iterator iBegin = etl::begin(data);
      CHECK(data.begin() == iBegin);

      const std::list<int>::const_iterator iEnd = etl::end(data);
      CHECK(data.end() == iEnd);
    }

    //*************************************************************************
    TEST(test_c_array)
    {
      const size_t SIZE = 10UL;
      int data[SIZE];

      int* iBegin = etl::begin(data);
      CHECK(&data[0] == iBegin);

      int* iEnd = etl::end(data);
      CHECK((data + SIZE) == iEnd);
    }

    //*************************************************************************
    TEST(test_const_c_array)
    {
      const size_t SIZE = 10UL;
      const int data[SIZE] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };

      const int* const iBegin = etl::begin(data);
      CHECK(&data[0] == iBegin);

      const int* const iEnd = etl::end(data);
      CHECK((data + SIZE) == iEnd);
    }


    //*************************************************************************
    TEST(test_next)
    {
      const int data[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
      const int* p;

      p = etl::next(std::begin(data));
      CHECK_EQUAL(data[1], *p);

      p = etl::next(std::begin(data), 1);
      CHECK_EQUAL(data[1], *p);

      p = etl::next(std::begin(data), 5);
      CHECK_EQUAL(data[5], *p);
    }

    //*************************************************************************
    TEST(test_prev)
    {
      const int data[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
      const int* p;

      p = etl::prev(std::end(data));
      CHECK_EQUAL(data[9], *p);

      p = etl::prev(std::end(data), 1);
      CHECK_EQUAL(data[9], *p);

      p = etl::prev(std::end(data), 5);
      CHECK_EQUAL(data[5], *p);
    }

    //*************************************************************************
    TEST(test_stl_style_container_size)
    {
      const size_t SIZE = 10UL;
      std::list<int> data(SIZE);

      size_t runtime_size = ETL_OR_STD17::size(data);
      CHECK_EQUAL(SIZE, runtime_size);
    }

    //*************************************************************************
    TEST(test_c_array_size)
    {
      const size_t SIZE = 10UL;
      int data[SIZE];

      size_t runtime_size = ETL_OR_STD17::size(data);
      CHECK_EQUAL(SIZE, runtime_size);

      size_t compiletime_size = sizeof(etl::array_size(data));
      CHECK_EQUAL(SIZE, compiletime_size);
    }
  };
}

#endif
