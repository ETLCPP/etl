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

#include "data.h"

#include <set>
#include <vector>
#include <string>

#include "etl/pool.h"
#include "etl/largest.h"

typedef TestDataDC<std::string>  Test_Data;
typedef TestDataNDC<std::string> Test_Data2;

namespace
{
  struct D0
  {
    D0()
    {
    }
  };

  struct D1
  {
    D1(const std::string& a_)
      : a(a_)
    {
    }

    std::string a;
  };

  struct D2
  {
    D2(const std::string& a_, const std::string& b_)
      : a(a_),
      b(b_)
    {
    }

    std::string a;
    std::string b;
  };

  struct D3
  {
    D3(const std::string& a_, const std::string& b_, const std::string& c_)
      : a(a_),
      b(b_),
      c(c_)
    {
    }

    std::string a;
    std::string b;
    std::string c;
  };

  struct D4
  {
    D4(const std::string& a_, const std::string& b_, const std::string& c_, const std::string& d_)
      : a(a_),
      b(b_),
      c(c_),
      d(d_)
    {
    }

    std::string a;
    std::string b;
    std::string c;
    std::string d;
  };

  bool operator == (const D0&, const D0&)
  {
    return true;
  }

  bool operator == (const D1& lhs, const D1& rhs)
  {
    return (lhs.a == rhs.a);
  }

  bool operator == (const D2& lhs, const D2& rhs)
  {
    return (lhs.a == rhs.a) && (lhs.b == rhs.b);
  }

  bool operator == (const D3& lhs, const D3& rhs)
  {
    return (lhs.a == rhs.a) && (lhs.b == rhs.b) && (lhs.c == rhs.c);
  }

  bool operator == (const D4& lhs, const D4& rhs)
  {
    return (lhs.a == rhs.a) && (lhs.b == rhs.b) && (lhs.c == rhs.c) && (lhs.d == rhs.d);
  }

  std::ostream& operator <<(std::ostream& os, const D0&)
  {
     return os;
  }

  std::ostream& operator <<(std::ostream& os, const D1& d)
  {
    os << d.a;
    return os;
  }

  std::ostream& operator <<(std::ostream& os, const D2& d)
  {
    os << d.a << " " << d.b;
    return os;
  }

  std::ostream& operator <<(std::ostream& os, const D3& d)
  {
    os << d.a << " " << d.b << " " << d.c;
    return os;
  }

  std::ostream& operator <<(std::ostream& os, const D4& d)
  {
    os << d.a << " " << d.b << " " << d.c << " " << d.d;
    return os;
  }

  SUITE(test_pool_external_buffer)
  {
    //*************************************************************************
    TEST(test_allocate)
    {
      const size_t SIZE = 4;
      auto buffer = new etl::pool_ext<Test_Data>::element[SIZE];
      etl::pool_ext<Test_Data> pool(buffer, SIZE);

      Test_Data* p1 = nullptr;
      Test_Data* p2 = nullptr;
      Test_Data* p3 = nullptr;
      Test_Data* p4 = nullptr;

      CHECK_NO_THROW(p1 = pool.allocate());
      CHECK_NO_THROW(p2 = pool.allocate());
      CHECK_NO_THROW(p3 = pool.allocate());
      CHECK_NO_THROW(p4 = pool.allocate());

      CHECK(p1 != p2);
      CHECK(p1 != p3);
      CHECK(p1 != p4);
      CHECK(p2 != p3);
      CHECK(p2 != p4);
      CHECK(p3 != p4);

      CHECK_THROW(pool.allocate(), etl::pool_no_allocation);

      delete[] buffer;
    }

    //*************************************************************************
    TEST(test_release)
    {
      const size_t SIZE = 4;
      auto buffer = new etl::pool_ext<Test_Data>::element[SIZE];
      etl::pool_ext<Test_Data> pool(buffer, SIZE);

      Test_Data* p1 = pool.allocate();
      Test_Data* p2 = pool.allocate();
      Test_Data* p3 = pool.allocate();
      Test_Data* p4 = pool.allocate();

      CHECK_NO_THROW(pool.release(p2));
      CHECK_NO_THROW(pool.release(p3));
      CHECK_NO_THROW(pool.release(p1));
      CHECK_NO_THROW(pool.release(p4));

      CHECK_EQUAL(4U, pool.available());

      CHECK_THROW(pool.release(p4), etl::pool_no_allocation);
      CHECK_EQUAL(4U, pool.available());

      Test_Data not_in_pool;

      CHECK_THROW(pool.release(&not_in_pool), etl::pool_object_not_in_pool);

      delete[] buffer;
    }

    //*************************************************************************
    TEST(test_allocate_release)
    {
      const size_t SIZE = 4;
      auto buffer = new etl::pool_ext<Test_Data>::element[SIZE];
      etl::pool_ext<Test_Data> pool(buffer, SIZE);

      Test_Data* p1 = pool.allocate();
      Test_Data* p2 = pool.allocate();
      Test_Data* p3 = pool.allocate();
      Test_Data* p4 = pool.allocate();

      // Allocated p1, p2, p3, p4

      CHECK_EQUAL(0U, pool.available());

      CHECK_NO_THROW(pool.release(p2));
      CHECK_NO_THROW(pool.release(p3));

      // Allocated p1, p4

      CHECK_EQUAL(2U, pool.available());

      Test_Data* p5 = pool.allocate();
      Test_Data* p6 = pool.allocate();

      // Allocated p1, p4, p5, p6

      CHECK_EQUAL(0U, pool.available());

      CHECK(p5 != p1);
      CHECK(p5 != p4);

      CHECK(p6 != p1);
      CHECK(p6 != p4);

      CHECK_NO_THROW(pool.release(p5));

      // Allocated p1, p4, p6

      CHECK_EQUAL(1U, pool.available());

      Test_Data* p7 = pool.allocate();

      // Allocated p1, p4, p6, p7

      CHECK(p7 != p1);
      CHECK(p7 != p4);
      CHECK(p7 != p6);

      CHECK(pool.full());

      delete[] buffer;
    }

    //*************************************************************************
    TEST(test_available)
    {
      const size_t SIZE = 4;
      auto buffer = new etl::pool_ext<Test_Data>::element[SIZE];
      etl::pool_ext<Test_Data> pool(buffer, SIZE);

      CHECK_EQUAL(4U, pool.available());

      Test_Data* p;
      (void)p;

      p = pool.allocate();
      CHECK_EQUAL(3U, pool.available());

      p = pool.allocate();
      CHECK_EQUAL(2U, pool.available());

      p = pool.allocate();
      CHECK_EQUAL(1U, pool.available());

      p = pool.allocate();
      CHECK_EQUAL(0U, pool.available());

      delete[] buffer;
    }

    //*************************************************************************
    TEST(test_max_size)
    {
      const size_t SIZE = 4;
      auto buffer = new etl::pool_ext<Test_Data>::element[SIZE];
      etl::pool_ext<Test_Data> pool(buffer, SIZE);

      CHECK(pool.max_size() == 4U);

      delete[] buffer;
    }

    //*************************************************************************
    TEST(test_size)
    {
      const size_t SIZE = 4;
      auto buffer = new etl::pool_ext<Test_Data>::element[SIZE];
      etl::pool_ext<Test_Data> pool(buffer, SIZE);

      CHECK_EQUAL(0U, pool.size());

      Test_Data* p;
      (void)p;

      p = pool.allocate();
      CHECK_EQUAL(1U, pool.size());

      p = pool.allocate();
      CHECK_EQUAL(2U, pool.size());

      p = pool.allocate();
      CHECK_EQUAL(3U, pool.size());

      p = pool.allocate();
      CHECK_EQUAL(4U, pool.size());

      delete[] buffer;
    }

    //*************************************************************************
    TEST(test_empty_full)
    {
      const size_t SIZE = 4;
      auto buffer = new etl::pool_ext<Test_Data>::element[SIZE];
      etl::pool_ext<Test_Data> pool(buffer, SIZE);

      CHECK(pool.empty());
      CHECK(!pool.full());

      Test_Data* p;
      (void)p;

      p = pool.allocate();
      CHECK(!pool.empty());
      CHECK(!pool.full());

      p = pool.allocate();
      CHECK(!pool.empty());
      CHECK(!pool.full());

      p = pool.allocate();
      CHECK(!pool.empty());
      CHECK(!pool.full());

      p = pool.allocate();
      CHECK(!pool.empty());
      CHECK(pool.full());

      delete[] buffer;
    }

    //*************************************************************************
    TEST(test_is_in_pool)
    {
      const size_t SIZE = 4;
      auto buffer = new etl::pool_ext<Test_Data>::element[SIZE];
      etl::pool_ext<Test_Data> pool(buffer, SIZE);

      Test_Data not_in_pool;

      Test_Data* p1 = pool.allocate();

      CHECK(pool.is_in_pool(p1));
      CHECK(!pool.is_in_pool(&not_in_pool));

      delete[] buffer;
    }

    //*************************************************************************
    TEST(test_type_error)
    {
      struct Test
      {
        uint64_t a;
        uint64_t b;
      };

      const size_t SIZE = 4;
      auto buffer = new etl::pool_ext<uint32_t>::element[SIZE];
      etl::pool_ext<uint32_t> pool(buffer, SIZE);

      etl::ipool& ip = pool;

      CHECK_THROW(ip.allocate<Test>(), etl::pool_element_size);

      delete[] buffer;
    }

    //*************************************************************************
    TEST(test_create_destroy)
    {
      const size_t SIZE = 4;

      auto buffer0 = new etl::pool_ext<D0>::element[SIZE];
      etl::pool_ext<D0> pool0(buffer0, SIZE);

      auto buffer1 = new etl::pool_ext<D1>::element[SIZE];
      etl::pool_ext<D1> pool1(buffer1, SIZE);

      auto buffer2 = new etl::pool_ext<D2>::element[SIZE];
      etl::pool_ext<D2> pool2(buffer2, SIZE);

      auto buffer3 = new etl::pool_ext<D3>::element[SIZE];
      etl::pool_ext<D3> pool3(buffer3, SIZE);

      auto buffer4 = new etl::pool_ext<D4>::element[SIZE];
      etl::pool_ext<D4> pool4(buffer4, SIZE);


      D0* p0 = pool0.create();
      D1* p1 = pool1.create("1");
      D2* p2 = pool2.create("1", "2");
      D3* p3 = pool3.create("1", "2", "3");
      D4* p4 = pool4.create("1", "2", "3", "4");

      (void)p1;
      (void)p2;
      (void)p3;
      (void)p4;

      CHECK_EQUAL(pool0.max_size() - 1, pool0.available());
      CHECK_EQUAL(1U, pool0.size());

      CHECK_EQUAL(pool1.max_size() - 1, pool1.available());
      CHECK_EQUAL(1U, pool1.size());

      CHECK_EQUAL(pool2.max_size() - 1, pool2.available());
      CHECK_EQUAL(1U, pool2.size());

      CHECK_EQUAL(pool3.max_size() - 1, pool3.available());
      CHECK_EQUAL(1U, pool3.size());

      CHECK_EQUAL(pool4.max_size() - 1, pool4.available());
      CHECK_EQUAL(1U, pool4.size());

      CHECK_EQUAL(D0(), *p0);
      CHECK_EQUAL(D1("1"), *p1);
      CHECK_EQUAL(D2("1", "2"), *p2);
      CHECK_EQUAL(D3("1", "2", "3"), *p3);
      CHECK_EQUAL(D4("1", "2", "3", "4"), *p4);

      pool0.destroy<D0>(p0);
      pool1.destroy<D1>(p1);
      pool2.destroy<D2>(p2);
      pool3.destroy<D3>(p3);
      pool4.destroy<D4>(p4);

      CHECK_EQUAL(pool0.max_size(), pool0.available());
      CHECK_EQUAL(0U, pool0.size());

      CHECK_EQUAL(pool1.max_size(), pool1.available());
      CHECK_EQUAL(0U, pool1.size());

      CHECK_EQUAL(pool2.max_size(), pool2.available());
      CHECK_EQUAL(0U, pool2.size());

      CHECK_EQUAL(pool3.max_size(), pool3.available());
      CHECK_EQUAL(0U, pool3.size());

      CHECK_EQUAL(pool4.max_size(), pool4.available());
      CHECK_EQUAL(0U, pool4.size());

      delete[] buffer0;
      delete[] buffer1;
      delete[] buffer2;
      delete[] buffer3;
      delete[] buffer4;
    }

    //*************************************************************************
    TEST(test_allocate_release_non_class)
    {
      const size_t SIZE = 4;
      auto buffer = new etl::pool_ext<int>::element[SIZE];
      etl::pool_ext<int> pool(buffer, SIZE);

      int* i = pool.allocate();
      pool.release(i);

      delete[] buffer;
    } 
  
    //*************************************************************************
    TEST(test_issue_406_pool_of_c_array)
    {
      using elem_type = uint8_t[10];

      const size_t SIZE = 3;
      auto buffer = new etl::pool_ext<elem_type>::element[SIZE];
      etl::pool_ext<elem_type> memPool(buffer, SIZE);

      CHECK_EQUAL(3, memPool.available());
      CHECK_EQUAL(0, memPool.size());

      elem_type* memory = memPool.allocate();

      CHECK_EQUAL(2, memPool.available());
      CHECK_EQUAL(1, memPool.size());

      memPool.release(memory);

      CHECK_EQUAL(3, memPool.available());
      CHECK_EQUAL(0, memPool.size());

      delete[] buffer;
    }
  }
}
