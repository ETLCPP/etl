/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl
http://www.etlcpp.com

Copyright(c) 2014 jwellbelove

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

#include <UnitTest++/UnitTest++.h>
#include "ExtraCheckMacros.h"

#include "data.h"

#include <set>
#include <vector>

#include "../src/pool.h"

#if defined(ETL_COMPILER_GCC)
  #pragma GCC diagnostic push
  #pragma GCC diagnostic ignored "-Wunused-but-set-variable"
#endif

typedef TestDataDC<std::string>  Test_Data;
typedef TestDataNDC<std::string> Test_Data2;

namespace
{
  SUITE(test_pool)
  {
    //*************************************************************************
    TEST(test_allocate)
    {
      etl::pool<Test_Data, 4> pool;

      Test_Data* p1 = nullptr;
      Test_Data* p2 = nullptr;
      Test_Data* p3 = nullptr;
      Test_Data* p4 = nullptr;

      CHECK_NO_THROW(p1 = pool.allocate<Test_Data>());
      CHECK_NO_THROW(p2 = pool.allocate<Test_Data>());
      CHECK_NO_THROW(p3 = pool.allocate<Test_Data>());
      CHECK_NO_THROW(p4 = pool.allocate<Test_Data>());

      CHECK(p1 != p2);
      CHECK(p1 != p3);
      CHECK(p1 != p4);
      CHECK(p2 != p3);
      CHECK(p2 != p4);
      CHECK(p3 != p4);

      CHECK_THROW(pool.allocate<Test_Data>(), etl::pool_no_allocation);
    }

    //*************************************************************************
    TEST(test_release)
    {
      etl::pool<Test_Data, 4> pool;

      Test_Data* p1 = pool.allocate<Test_Data>();
      Test_Data* p2 = pool.allocate<Test_Data>();
      Test_Data* p3 = pool.allocate<Test_Data>();
      Test_Data* p4 = pool.allocate<Test_Data>();

      CHECK_NO_THROW(pool.release(p2));
      CHECK_NO_THROW(pool.release(p3));
      CHECK_NO_THROW(pool.release(p1));
      CHECK_NO_THROW(pool.release(p4));

      CHECK_EQUAL(4U, pool.available());

      Test_Data not_in_pool;

      CHECK_THROW(pool.release(&not_in_pool), etl::pool_object_not_in_pool);
    }

    //*************************************************************************
    TEST(test_allocate_release)
    {
      etl::pool<Test_Data, 4> pool;

      Test_Data* p1 = pool.allocate<Test_Data>();
      Test_Data* p2 = pool.allocate<Test_Data>();
      Test_Data* p3 = pool.allocate<Test_Data>();
      Test_Data* p4 = pool.allocate<Test_Data>();

      // Allocated p1, p2, p3, p4

      CHECK_EQUAL(0U, pool.available());

      CHECK_NO_THROW(pool.release(p2));
      CHECK_NO_THROW(pool.release(p3));

      // Allocated p1, p4

      CHECK_EQUAL(2U, pool.available());

      Test_Data* p5 = pool.allocate<Test_Data>();
      Test_Data* p6 = pool.allocate<Test_Data>();

      // Allocated p1, p4, p5, p6

      CHECK_EQUAL(0U, pool.available());

      CHECK(p5 != p1);
      CHECK(p5 != p4);

      CHECK(p6 != p1);
      CHECK(p6 != p4);

      CHECK_NO_THROW(pool.release(p5));

      // Allocated p1, p4, p6

      CHECK_EQUAL(1U, pool.available());

      Test_Data* p7 = pool.allocate<Test_Data>();

      // Allocated p1, p4, p6, p7

      CHECK(p7 != p1);
      CHECK(p7 != p4);
      CHECK(p7 != p6);

      CHECK(pool.full());
    }

    //*************************************************************************
    TEST(test_available)
    {
      etl::pool<Test_Data, 4> pool;
      CHECK_EQUAL(4U, pool.available());

      Test_Data* p;

      p = pool.allocate<Test_Data>();
      CHECK_EQUAL(3U, pool.available());

      p = pool.allocate<Test_Data>();
      CHECK_EQUAL(2U, pool.available());

      p = pool.allocate<Test_Data>();
      CHECK_EQUAL(1U, pool.available());

      p = pool.allocate<Test_Data>();
      CHECK_EQUAL(0U, pool.available());
    }

    //*************************************************************************
    TEST(test_max_size)
    {
      etl::pool<Test_Data, 4> pool;

      CHECK(pool.max_items() == 4U);
    }

    //*************************************************************************
    TEST(test_size)
    {
      etl::pool<Test_Data, 4> pool;
      CHECK_EQUAL(0U, pool.size());

      Test_Data* p;

      p = pool.allocate<Test_Data>();
      CHECK_EQUAL(1U, pool.size());

      p = pool.allocate<Test_Data>();
      CHECK_EQUAL(2U, pool.size());

      p = pool.allocate<Test_Data>();
      CHECK_EQUAL(3U, pool.size());

      p = pool.allocate<Test_Data>();
      CHECK_EQUAL(4U, pool.size());
    }

    //*************************************************************************
    TEST(test_empty_full)
    {
      etl::pool<Test_Data, 4> pool;
      CHECK(pool.empty());
      CHECK(!pool.full());

      Test_Data* p;

      p = pool.allocate<Test_Data>();
      CHECK(!pool.empty());
      CHECK(!pool.full());

      p = pool.allocate<Test_Data>();
      CHECK(!pool.empty());
      CHECK(!pool.full());

      p = pool.allocate<Test_Data>();
      CHECK(!pool.empty());
      CHECK(!pool.full());

      p = pool.allocate<Test_Data>();
      CHECK(!pool.empty());
      CHECK(pool.full());
    }

    //*************************************************************************
    TEST(test_is_in_pool)
    {
      etl::pool<Test_Data, 4> pool;
      Test_Data not_in_pool;

      Test_Data* p1 = pool.allocate<Test_Data>();

      CHECK(pool.is_in_pool(p1));
      CHECK(!pool.is_in_pool(&not_in_pool));
    }

    //*************************************************************************
    TEST(test_begin_empty)
    {
      //etl::pool<Test_Data, 4> pool;

      //etl::pool<Test_Data, 4>::iterator it = pool.begin();
      //CHECK(it == pool.end());

      //etl::pool<Test_Data, 4>::const_iterator cit = pool.begin();
      //CHECK(cit == pool.end());

      //cit = pool.cbegin();
      //CHECK(cit == pool.end());
    }

    //*************************************************************************
    TEST(test_iterator)
    {
      //etl::pool<Test_Data2, 10> pool;

      //std::set<Test_Data2>     compare = { Test_Data2("0"), Test_Data2("2"), Test_Data2("4"), Test_Data2("6"), Test_Data2("8") };
      //std::set<Test_Data2>     test;
      //std::vector<Test_Data2*> objects;

      //// Build the set of objects.
      //objects.push_back(pool.allocate(Test_Data2("9")));
      //objects.push_back(pool.allocate(Test_Data2("7")));
      //objects.push_back(pool.allocate(Test_Data2("8")));
      //objects.push_back(pool.allocate(Test_Data2("6")));
      //objects.push_back(pool.allocate(Test_Data2("5")));
      //objects.push_back(pool.allocate(Test_Data2("3")));
      //objects.push_back(pool.allocate(Test_Data2("4")));
      //objects.push_back(pool.allocate(Test_Data2("2")));
      //objects.push_back(pool.allocate(Test_Data2("0")));
      //objects.push_back(pool.allocate(Test_Data2("1")));

      //// Release "1", "3", "5", "7", "9".
      //pool.release(objects[0]);
      //pool.release(objects[1]);
      //pool.release(objects[4]);
      //pool.release(objects[5]);
      //pool.release(objects[9]);

      //// Fill the test set with what we get from the iterator.
      //etl::pool<Test_Data2, 10>::iterator i_pool = pool.begin();

      //while (i_pool != pool.end())
      //{
      //  test.insert(*i_pool);
      //  ++i_pool;
      //}

      //// Compare the results.
      //std::set<Test_Data2>::iterator i_compare = compare.begin();
      //std::set<Test_Data2>::iterator i_test = test.begin();

      //CHECK_EQUAL(compare.size(), test.size());

      //while ((i_compare != compare.end()) && (i_test != test.end()))
      //{
      //  CHECK_EQUAL(*i_compare++, *i_test++);
      //}
    }

    //*************************************************************************
    TEST(test_const_iterator)
    {
      //etl::pool<Test_Data2, 10> pool;

      //std::set<Test_Data2>     compare = { Test_Data2("0"), Test_Data2("2"), Test_Data2("4"), Test_Data2("6"), Test_Data2("8") };
      //std::set<Test_Data2>     test;
      //std::vector<Test_Data2*> objects;

      //// Build the set of objects.
      //objects.push_back(pool.allocate(Test_Data2("9")));
      //objects.push_back(pool.allocate(Test_Data2("7")));
      //objects.push_back(pool.allocate(Test_Data2("8")));
      //objects.push_back(pool.allocate(Test_Data2("6")));
      //objects.push_back(pool.allocate(Test_Data2("5")));
      //objects.push_back(pool.allocate(Test_Data2("3")));
      //objects.push_back(pool.allocate(Test_Data2("4")));
      //objects.push_back(pool.allocate(Test_Data2("2")));
      //objects.push_back(pool.allocate(Test_Data2("0")));
      //objects.push_back(pool.allocate(Test_Data2("1")));

      //// Release "1", "3", "5", "7", "9".
      //pool.release(objects[0]);
      //pool.release(objects[1]);
      //pool.release(objects[4]);
      //pool.release(objects[5]);
      //pool.release(objects[9]);

      //// Fill the test set with what we get from the iterator.
      //etl::pool<Test_Data2, 10>::const_iterator i_pool = pool.begin();

      //while (i_pool != pool.end())
      //{
      //  test.insert(*i_pool);
      //  ++i_pool;
      //}

      //// Compare the results.
      //std::set<Test_Data2>::const_iterator i_compare = compare.begin();
      //std::set<Test_Data2>::const_iterator i_test    = test.begin();

      //CHECK_EQUAL(compare.size(), test.size());

      //while ((i_compare != compare.end()) && (i_test != test.end()))
      //{
      //  CHECK_EQUAL(*i_compare++, *i_test++);
      //}
    }

    //*************************************************************************
    TEST(test_get_iterator)
    {
      //typedef etl::pool<Test_Data, 4> Pool;

      //Pool pool;
      //Test_Data not_in_pool;

      //Test_Data* p1 = pool.allocate<Test_Data>();
      //Test_Data* p2 = pool.allocate<Test_Data>();

      //Pool::iterator i_data  = pool.get_iterator(*p1);
      //Pool::iterator i_data2 = pool.get_iterator(*p2);
      //Pool::iterator i_ndata = pool.get_iterator(not_in_pool);

      //CHECK(p1 == &*i_data);
      //CHECK(p2 != &*i_data);
      //CHECK(p2 == &*i_data2);
      //CHECK(pool.end() == i_ndata);
    }

    //*************************************************************************
    TEST(test_get_iterator_const)
    {
      //typedef etl::pool<Test_Data, 4> Pool;

      //Pool pool;
      //const Test_Data not_in_pool;

      //const Test_Data* p1 = pool.allocate<Test_Data>();
      //const Test_Data* p2 = pool.allocate<Test_Data>();

      //Pool::const_iterator i_data  = pool.get_iterator(*p1);
      //Pool::const_iterator i_data2 = pool.get_iterator(*p2);
      //Pool::const_iterator i_ndata = pool.get_iterator(not_in_pool);

      //CHECK(p1 == &*i_data);
      //CHECK(p2 != &*i_data);
      //CHECK(p2 == &*i_data2);
      //CHECK(pool.end() == i_ndata);
    }
  };
}

#if defined(ETL_COMPILER_GCC)
  #pragma GCC diagnostic pop
#endif
