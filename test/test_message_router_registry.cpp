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

#include "etl/message_router.h"
#include "etl/message_router_registry.h"
#include "etl/vector.h"

//***************************************************************************
// The set of messages.
//***************************************************************************
namespace
{
  enum
  {
    ROUTER1 = 10,
    ROUTER2 = 20,
    ROUTER3 = 30,
    ROUTER4 = 40,
    ROUTER5 = 50
  };

  constexpr size_t Registry_Size = 4UL;

  struct Message1 : public etl::message<1>
  {
  };

  //***************************************************************************
  // Router1
  //***************************************************************************
  class Router1 : public etl::message_router<Router1, Message1>
  {
  public:

    Router1()
      : message_router(ROUTER1)
    {
    }

    void on_receive(const Message1&)
    {
    }

    void on_receive_unknown(const etl::imessage&)
    {
    }
  };

  //***************************************************************************
  // Router2
  //***************************************************************************
  class Router2 : public etl::message_router<Router2, Message1>
  {
  public:

    Router2()
      : message_router(ROUTER2)
    {
    }

    void on_receive(const Message1&)
    {
    }

    void on_receive_unknown(const etl::imessage&)
    {
    }
  };

  //***************************************************************************
  // Router3
  //***************************************************************************
  class Router3 : public etl::message_router<Router2, Message1>
  {
  public:

    Router3()
      : message_router(ROUTER3)
    {
    }

    void on_receive(const Message1&)
    {
    }

    void on_receive_unknown(const etl::imessage&)
    {
    }
  };

  //***************************************************************************
  // Router4
  //***************************************************************************
  class Router4 : public etl::message_router<Router4, Message1>
  {
  public:

    Router4()
      : message_router(ROUTER4)
    {
    }

    void on_receive(const Message1&)
    {
    }

    void on_receive_unknown(const etl::imessage&)
    {
    }
  };

  //***************************************************************************
  // Router5
  //***************************************************************************
  class Router5 : public etl::message_router<Router5, Message1>
  {
  public:

    Router5()
      : message_router(ROUTER5)
    {
    }

    void on_receive(const Message1&)
    {
    }

    void on_receive_unknown(const etl::imessage&)
    {
    }
  };

  Router1 router1;
  Router2 router2;
  Router2 router2b;
  Router2 router2c;
  Router3 router3;
  Router4 router4;
  Router5 router5;

  SUITE(test_message_router_registry)
  {
    //*************************************************************************
    TEST(test_default_construction)
    {
      etl::message_router_registry<Registry_Size> registry;

      CHECK(registry.empty());
      CHECK(!registry.full());
      CHECK_EQUAL(0U,            registry.size());
      CHECK_EQUAL(Registry_Size, registry.available());
      CHECK_EQUAL(Registry_Size, registry.max_size());
    }

    //*************************************************************************
    TEST(test_iterator_construction)
    {
      etl::imessage_router* routers[] = { &router1, &router2, &router3, &router4 };
      etl::message_router_registry<Registry_Size> registry(std::begin(routers), std::end(routers));

      CHECK(!registry.empty());
      CHECK(registry.full());
      CHECK_EQUAL(Registry_Size, registry.size());
      CHECK_EQUAL(0U,            registry.available());
      CHECK_EQUAL(Registry_Size, registry.max_size());
    }

    //*************************************************************************
    TEST(test_iterator_construction_excess)
    {
      etl::imessage_router* routers[] = { &router1, &router2, &router3, &router4, &router5 };
      etl::message_router_registry<Registry_Size> registry;

      CHECK_THROW((registry.add(std::begin(routers), std::end(routers))), etl::message_router_registry_full);

      CHECK(!registry.empty());
      CHECK(registry.full());
      CHECK_EQUAL(Registry_Size, registry.size());
      CHECK_EQUAL(0U,            registry.available());
      CHECK_EQUAL(Registry_Size, registry.max_size());
    }

#if ETL_USING_STL
    //*************************************************************************
    TEST(test_initializer_list_construction)
    {
      etl::message_router_registry<Registry_Size> registry = { &router1, &router2, &router3, &router4 };

      CHECK(!registry.empty());
      CHECK(registry.full());
      CHECK_EQUAL(Registry_Size, registry.size());
      CHECK_EQUAL(0U,            registry.available());
      CHECK_EQUAL(Registry_Size, registry.max_size());
    }

    //*************************************************************************
    TEST(test_initializer_list_construction_part_full)
    {
      etl::message_router_registry<Registry_Size> registry = { &router1, &router2, &router3 };

      CHECK(!registry.empty());
      CHECK(!registry.full());
      CHECK_EQUAL(Registry_Size - 1U, registry.size());
      CHECK_EQUAL(1U,                 registry.available());
      CHECK_EQUAL(Registry_Size,      registry.max_size());
    }
#endif

    //*************************************************************************
    TEST(test_copy_construction)
    {
      etl::imessage_router* routers[] = { &router1, &router2, &router3, &router4 };
      etl::message_router_registry<Registry_Size> registry(std::begin(routers), std::end(routers));
      etl::message_router_registry<Registry_Size> registry2(registry);

      CHECK(!registry2.empty());
      CHECK(registry2.full());
      CHECK_EQUAL(Registry_Size, registry2.size());
      CHECK_EQUAL(0U,            registry2.available());
      CHECK_EQUAL(Registry_Size, registry2.max_size());
    }

        //*************************************************************************
    TEST(test_assignment)
    {
      etl::imessage_router* routers[] = { &router1, &router2, &router3, &router4 };
      etl::message_router_registry<Registry_Size> registry(std::begin(routers), std::end(routers));
      etl::message_router_registry<Registry_Size> registry2;

      registry2 = registry;

      CHECK(!registry2.empty());
      CHECK(registry2.full());
      CHECK_EQUAL(Registry_Size, registry2.size());
      CHECK_EQUAL(0U,            registry2.available());
      CHECK_EQUAL(Registry_Size, registry2.max_size());
    }

    //*************************************************************************
    TEST(test_registry_contains)
    {
      etl::imessage_router* routers[] = { &router1, &router2, &router3 };
      etl::message_router_registry<Registry_Size> registry(std::begin(routers), std::end(routers));

      CHECK(registry.contains(ROUTER1));
      CHECK(registry.contains(ROUTER2));
      CHECK(registry.contains(ROUTER3));
      CHECK(!registry.contains(ROUTER4));

      CHECK(registry.contains(router1));
      CHECK(registry.contains(router2));
      CHECK(registry.contains(router3));
      CHECK(!registry.contains(router4));

      CHECK(registry.contains(&router1));
      CHECK(registry.contains(&router2));
      CHECK(registry.contains(&router3));
      CHECK(!registry.contains(&router4));
    }

    //*************************************************************************
    TEST(test_find_message_router)
    {
      etl::imessage_router* routers[] = { &router1, &router2, &router3 };
      etl::message_router_registry<Registry_Size> registry(std::begin(routers), std::end(routers));

      CHECK(&router1 == registry.find(ROUTER1));
      CHECK(&router2 == registry.find(ROUTER2));
      CHECK(&router3 == registry.find(ROUTER3));
      CHECK(nullptr  == registry.find(ROUTER4));
    }

    //*************************************************************************
    TEST(test_multiple_message_routers_with_same_id)
    {
      etl::imessage_router* routers[] = { &router1, &router2, &router3, &router2b, &router4, &router2c, &router5 };
      etl::message_router_registry<7U> registry(std::begin(routers), std::end(routers));

      using iterator = etl::imessage_router_registry::iterator;

      iterator start = registry.lower_bound(ROUTER2);
      iterator end   = registry.upper_bound(ROUTER2);

      CHECK_EQUAL(1U, registry.count(ROUTER1));
      CHECK_EQUAL(3U, registry.count(ROUTER2));
      CHECK_EQUAL(1U, registry.count(ROUTER3));
      CHECK_EQUAL(1U, registry.count(ROUTER4));
      CHECK_EQUAL(1U, registry.count(ROUTER5));

      iterator itr = start;

      CHECK(&*itr == &router2);
      ++itr;
      CHECK(&*itr == &router2b);
      ++itr;
      CHECK(&*itr == &router2c);
      ++itr;
      CHECK(itr == end);
    }

    //*************************************************************************
    TEST(test_unregister_message_router)
    {
      etl::imessage_router* routers[] = { &router1, &router2, &router3, &router4 };
      etl::message_router_registry<Registry_Size> registry(std::begin(routers), std::end(routers));

      registry.remove(ROUTER3);

      CHECK(!registry.contains(router3));
      CHECK(!registry.contains(ROUTER3));

      CHECK(&router1 == registry.find(ROUTER1));
      CHECK(&router2 == registry.find(ROUTER2));
      CHECK(nullptr  == registry.find(ROUTER3));
      CHECK(&router4 == registry.find(ROUTER4));
    }

    //*************************************************************************
    TEST(test_iterators)
    {
      etl::imessage_router* routers[] = { &router1, &router2, &router3, &router4 };
      etl::message_router_registry<Registry_Size> registry(std::begin(routers), std::end(routers));

      etl::message_router_registry<Registry_Size>::iterator       bitr   = registry.begin();
      etl::message_router_registry<Registry_Size>::const_iterator cbitr  = registry.begin();
      etl::message_router_registry<Registry_Size>::const_iterator cbitr2 = registry.cbegin();

      etl::message_router_registry<Registry_Size>::iterator       eitr   = registry.end();
      etl::message_router_registry<Registry_Size>::const_iterator ceitr  = registry.end();
      etl::message_router_registry<Registry_Size>::const_iterator ceitr2 = registry.cend();

      CHECK_EQUAL(&router1, &*bitr++);
      CHECK_EQUAL(&router1, &*cbitr++);
      CHECK_EQUAL(&router1, &*cbitr2++);

      CHECK_EQUAL(&router2, &*bitr++);
      CHECK_EQUAL(&router2, &*cbitr++);
      CHECK_EQUAL(&router2, &*cbitr2++);

      CHECK_EQUAL(&router3, &*bitr++);
      CHECK_EQUAL(&router3, &*cbitr++);
      CHECK_EQUAL(&router3, &*cbitr2++);

      CHECK_EQUAL(&router4, &*bitr++);
      CHECK_EQUAL(&router4, &*cbitr++);
      CHECK_EQUAL(&router4, &*cbitr2++);

      CHECK(bitr   == eitr);
      CHECK(cbitr  == ceitr);
      CHECK(cbitr2 == ceitr2);
    }

    //*************************************************************************
    TEST(test_operators)
    {
      etl::imessage_router* routers[] = { &router1, &router2, &router3, &router4 };
      etl::message_router_registry<Registry_Size> registry(std::begin(routers), std::end(routers));

      etl::message_router_registry<Registry_Size>::iterator       itr   = registry.begin();
      etl::message_router_registry<Registry_Size>::const_iterator citr  = registry.cbegin();

      CHECK_EQUAL(ROUTER1, (*itr).get_message_router_id());
      CHECK_EQUAL(ROUTER1, itr->get_message_router_id());

      CHECK_EQUAL(ROUTER1, (*citr).get_message_router_id());
      CHECK_EQUAL(ROUTER1, citr->get_message_router_id());
    }
  };
}
