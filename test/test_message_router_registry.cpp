/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl
https://www.etlcpp.com

Copyright(c) 2021 jwellbelove

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
#include "etl/array.h"

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

  constexpr size_t Registry_Size = 4U;

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

    void on_receive(const Message1& msg)
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

    void on_receive(const Message1& msg)
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

    void on_receive(const Message1& msg)
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

    void on_receive(const Message1& msg)
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

    void on_receive(const Message1& msg)
    {
    }

    void on_receive_unknown(const etl::imessage&)
    {
    }
  };

  Router1 router1;
  Router2 router2;
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
      etl::array<etl::imessage_router*, Registry_Size> routers{ &router1, &router2, &router3, &router4 };

      etl::message_router_registry<Registry_Size> registry(routers.begin(), routers.end());

      CHECK(!registry.empty());
      CHECK(registry.full());
      CHECK_EQUAL(Registry_Size, registry.size());
      CHECK_EQUAL(0U,            registry.available());
      CHECK_EQUAL(Registry_Size, registry.max_size());
    }

    //*************************************************************************
    TEST(test_iterator_construction_excess)
    {
      etl::array<etl::imessage_router*, Registry_Size + 1U> routers{ &router1, &router2, &router3, &router4, &router5 };

      etl::message_router_registry<Registry_Size> registry;

      CHECK_THROW((registry.add(routers.begin(), routers.end())), etl::message_router_registry_full);

      CHECK(!registry.empty());
      CHECK(registry.full());
      CHECK_EQUAL(Registry_Size, registry.size());
      CHECK_EQUAL(0U,            registry.available());
      CHECK_EQUAL(Registry_Size, registry.max_size());
    }

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

    //*************************************************************************
    TEST(test_registery_contains)
    {
      etl::message_router_registry<Registry_Size> registry = { &router1, &router2, &router3 };

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
    TEST(test_get_message_router)
    {
      etl::message_router_registry<Registry_Size> registry = { &router1, &router2, &router3 };

      CHECK_EQUAL(&router1, registry.get_message_router(ROUTER1));
      CHECK_EQUAL(&router2, registry.get_message_router(ROUTER2));
      CHECK_EQUAL(&router3, registry.get_message_router(ROUTER3));
      CHECK_EQUAL(nullptr,  registry.get_message_router(ROUTER4));
    }

    //*************************************************************************
    TEST(test_unregister_message_router)
    {
      etl::message_router_registry<Registry_Size> registry = { &router1, &router2, &router3, &router4 };

      registry.remove(ROUTER3);

      CHECK(!registry.contains(router3));

      CHECK_EQUAL(&router1, registry.get_message_router(ROUTER1));
      CHECK_EQUAL(&router2, registry.get_message_router(ROUTER2));
      CHECK_EQUAL(nullptr,  registry.get_message_router(ROUTER3));
      CHECK_EQUAL(&router4, registry.get_message_router(ROUTER4));
    }
  };
}
