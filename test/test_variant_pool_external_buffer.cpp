/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl
https://www.etlcpp.com

Copyright(c) 2017 John Wellbelove

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

#include "etl/variant_pool.h"

#include <string>
#include <type_traits>

namespace
{
  bool destructor;

  //***********************************
  struct Base
  {
    Base()
    {
      destructor = false;
    }

    virtual ~Base()
    {
    }

    virtual void Set() = 0;
  };

  //***********************************
  struct Derived1 : public Base
  {
    int i;

    Derived1()
      : i(0)
    {
    }

    ~Derived1()
    {
      destructor = true;
    }

    void Set()
    {
      i = 1;
    }
  };

  //***********************************
  struct Derived2 : public Base
  {
    double d;

    Derived2()
      : d(0.0)
    {
    }

    ~Derived2()
    {
      destructor = true;
    }

    void Set()
    {
      d = 1.2;
    }
  };

  //***********************************
  struct Derived3 : public Base
  {
    std::string s;

    Derived3()
      : s("constructed")
    {
    }

    Derived3(const char* p1)
      : s("constructed")
    {
      s.append(p1);
    }

    Derived3(const char* p1, const std::string& p2)
      : s("constructed")
    {
      s.append(p1);
      s.append(p2);
    }

    Derived3(const char* p1, const std::string& p2, const char* p3)
      : s("constructed")
    {
      s.append(p1);
      s.append(p2);
      s.append(p3);
    }

    Derived3(const char* p1, const std::string& p2, const char* p3, const std::string& p4)
      : s("constructed")
    {
      s.append(p1);
      s.append(p2);
      s.append(p3);
      s.append(p4);
    }

    ~Derived3()
    {
      destructor = true;
    }

    void Set()
    {
      s = "set";
    }
  };

  //***********************************
  struct NonDerived
  {
    NonDerived()
      : s("constructed")
    {
    }

    ~NonDerived()
    {
      destructor = true;
    }

    void Set()
    {
      s = "set";
    }

    std::string s;
  };

  const size_t SIZE = 5;

  // Notice that the type declaration order is not important.
  typedef etl::variant_pool_ext<Derived1, NonDerived, Derived3, Derived2, int> Factory;

  SUITE(test_variant_pool_external_buffer)
  {
    //*************************************************************************
    TEST(test_sizes)
    {
      auto buffer = new Factory::element[SIZE];
      Factory variant_pool(buffer, SIZE);

      size_t ms = variant_pool.max_size();
      CHECK_EQUAL(SIZE, ms);
      CHECK_EQUAL(SIZE, variant_pool.available());
      CHECK_EQUAL(0U, variant_pool.size());
      CHECK(variant_pool.empty());
      CHECK(!variant_pool.full());

      variant_pool.create<Derived1>();
      CHECK_EQUAL(SIZE - 1U, variant_pool.available());
      CHECK_EQUAL(1U, variant_pool.size());
      CHECK(!variant_pool.empty());
      CHECK(!variant_pool.full());

      variant_pool.create<Derived1>();
      variant_pool.create<Derived1>();
      variant_pool.create<Derived1>();
      variant_pool.create<Derived1>();
      CHECK_EQUAL(0U, variant_pool.available());
      CHECK_EQUAL(SIZE, variant_pool.size());
      CHECK(!variant_pool.empty());
      CHECK(variant_pool.full());

      CHECK_THROW(variant_pool.create<Derived1>(), etl::pool_no_allocation);

      delete[] buffer;
    }

    //*************************************************************************
    TEST(test_create_release)
    {
      auto buffer = new Factory::element[SIZE];
      Factory variant_pool(buffer, SIZE);

      Base* p;

      // Derived 1
      p = variant_pool.create<Derived1>();
      Derived1* pd1 = static_cast<Derived1*>(p);
      CHECK_EQUAL(0, pd1->i);
      p->Set();
      CHECK_EQUAL(1, pd1->i);
      variant_pool.destroy(p);
      CHECK(destructor);

      // Derived 2
      destructor = false;
      p = variant_pool.create<Derived2>();
      Derived2* pd2 = static_cast<Derived2*>(p);
      CHECK_EQUAL(0.0, pd2->d);
      p->Set();
      CHECK_EQUAL(1.2, pd2->d);
      variant_pool.destroy(p);
      CHECK(destructor);

      // Derived 3
      destructor = false;
      p = variant_pool.create<Derived3>();
      Derived3* pd3 = static_cast<Derived3*>(p);
      CHECK_EQUAL("constructed", pd3->s);
      p->Set();
      CHECK_EQUAL("set", pd3->s);
      variant_pool.destroy(p);
      CHECK(destructor);

      // Non Derived
      destructor = false;
      NonDerived* pnd = variant_pool.create<NonDerived>();
      CHECK_EQUAL("constructed", pnd->s);
      pnd->Set();
      CHECK_EQUAL("set", pnd->s);
      variant_pool.destroy(pnd);
      CHECK(destructor);

      // Integral
      int* pi = variant_pool.create<int>();
      variant_pool.destroy(pi);

      delete[] buffer;
    }

    //*************************************************************************
    TEST(test_create_release_const)
    {
      auto buffer = new Factory::element[SIZE];
      Factory variant_pool(buffer, SIZE);

      const Derived1& d = *variant_pool.create<Derived1>();

      CHECK_EQUAL(0, d.i);
      variant_pool.destroy(&d);
      CHECK(destructor);

      delete[] buffer;
    }

    //*************************************************************************
    TEST(test_create_emplace)
    {
      auto buffer = new Factory::element[SIZE];
      Factory variant_pool(buffer, SIZE);

      Base* p;
      Derived3* pd3;

      p = variant_pool.create<Derived3>("1");
      pd3 = static_cast<Derived3*>(p);
      CHECK_EQUAL("constructed1", pd3->s);
      variant_pool.destroy(p);

      p = variant_pool.create<Derived3>("1", "2");
      pd3 = static_cast<Derived3*>(p);
      CHECK_EQUAL("constructed12", pd3->s);
      variant_pool.destroy(p);

      p = variant_pool.create<Derived3>("1", "2", "3");
      pd3 = static_cast<Derived3*>(p);
      CHECK_EQUAL("constructed123", pd3->s);
      variant_pool.destroy(p);

      p = variant_pool.create<Derived3>("1", "2", "3", "4");
      pd3 = static_cast<Derived3*>(p);
      CHECK_EQUAL("constructed1234", pd3->s);
      variant_pool.destroy(p);

      delete[] buffer;
    }

    //*************************************************************************
    TEST(test_did_not_create)
    {
      auto buffer1 = new Factory::element[SIZE];
      Factory variant_pool1(buffer1, SIZE);
      auto buffer2 = new Factory::element[SIZE];
      Factory variant_pool2(buffer2, SIZE);

      Base* p;

      p = variant_pool1.create<Derived1>();
      CHECK_NO_THROW(variant_pool1.destroy(p));

      p = variant_pool2.create<Derived1>();
      CHECK_THROW(variant_pool1.destroy(p), etl::pool_object_not_in_pool);

      delete[] buffer1;
      delete[] buffer2;
    }
  };
}
