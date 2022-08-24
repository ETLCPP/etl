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

#include "etl/packet.h"
#include "etl/largest.h"
#include "etl/queue.h"
#include "etl/pool.h"

namespace
{
  // Test classes for polymorphic tests.
  struct base
  {
    base(int v_)
      : was_moved(false)
      , v(v_)
    {
    }

    virtual ~base()
    {
    }

    virtual int value() const = 0;

    bool was_moved;

  protected:

    const int v;
    
  };

  struct not_base
  {
    not_base()
    {
    }
  };

  struct derived_1 : public base
  {
    derived_1(int value_)
      : base(value_)
    {
    }

    derived_1(const derived_1& other)
      : base(other.value())
    {
      was_moved = false;
    }

    derived_1(derived_1&& other)
      : base(other.value())
    {
      was_moved = true;
    }

    int value() const
    {
      return v;
    }
  };

  struct derived_2 : public base
  {
    derived_2(int value_)
      : base(value_)
    {
    }

    derived_2(const derived_2& other)
      : base(other.value())
    {
      was_moved = false;
    }

    derived_2(derived_2&& other)
      : base(other.value())
    {
      was_moved = true;
    }

    int value() const
    {
      return v;
    }
  };

  typedef etl::largest<derived_1, derived_2> types;
  //
  //// Test packet types.
  typedef etl::packet<base, types::size, types::alignment> packet1_t;
  typedef etl::packet<not_base, types::size, types::alignment> packet2_t;

  SUITE(test_packet)
  {
    //*************************************************************************
    TEST(test_constructor_value)
    {
      derived_1 d1(1);
      derived_2 d2(2);

      packet1_t p11(d1);            // Uses copy constructor
      CHECK(p11.get().was_moved == false);

      packet1_t p12(derived_2(2));  // Uses move constructor
      CHECK(p12.get().was_moved == true);

      base* b;
      b = &p11.get();
      CHECK_EQUAL(d1.value(), b->value());

      b = &p12.get();
      CHECK_EQUAL(d2.value(), b->value());

      // These lines should fail to compile.
      //p11 = p12;
      //packet2_t p21(d1);
      //packet2_t p22(d1);
    }

    //*************************************************************************
    TEST(test_assignment)
    {
      derived_1 d1(1);
      derived_2 d2(2);
      derived_1 d3(3);

      packet1_t p(d1);
      base* b;
      b = &p.get();
      CHECK_EQUAL(d1.value(), b->value());

      p = d2;
      CHECK_EQUAL(d2.value(), b->value());

      p = derived_1(3);
      CHECK_EQUAL(d3.value(), b->value());
    }

    //*************************************************************************
    TEST(test_packet_queueing)
    {
      derived_1 da(1);
      derived_2 db(2);
      derived_1 dc(3);
      derived_2 dd(4);

      etl::queue<packet1_t, 4> queue;

      queue.emplace(da);
      queue.emplace(db);
      queue.emplace(dc);
      queue.emplace(dd);

      CHECK_EQUAL(da.value(), queue.front().get().value());
      queue.pop();

      CHECK_EQUAL(db.value(), queue.front().get().value());
      queue.pop();

      CHECK_EQUAL(dc.value(), queue.front().get().value());
      queue.pop();

      CHECK_EQUAL(dd.value(), queue.front().get().value());
      queue.pop();
    }

    //*************************************************************************
    TEST(test_packet_get)
    {
      derived_1 da(1);

      // Non-const.
      packet1_t p1(da);
      base& rb = p1.get();
      CHECK_EQUAL(da.value(), rb.value());

      // Const.
      const packet1_t p2(da);
      const base& crb = p2.get();
      CHECK_EQUAL(da.value(), crb.value());
    }
  }
}
