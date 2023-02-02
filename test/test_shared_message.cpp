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

#include "etl/shared_message.h"
#include "etl/message.h"
#include "etl/message_router.h"
#include "etl/message_bus.h"
#include "etl/queue.h"
#include "etl/fixed_sized_memory_block_allocator.h"
#include "etl/reference_counted_message_pool.h"

namespace
{
  constexpr etl::message_id_t MessageId1 = 1U;
  constexpr etl::message_id_t MessageId2 = 2U;

  constexpr etl::message_router_id_t RouterId1 = 1U;
  constexpr etl::message_router_id_t RouterId2 = 2U;

  //*************************************************************************
  struct Message1 : public etl::message<MessageId1>
  {
    Message1(int i_)
      : i(i_)
    {
    }

    ~Message1()
    {

    }

    int i;
  };

  //*************************************************************************
  struct Message2 : public etl::message<MessageId2>
  {
    ~Message2()
    {

    }
  };

  //*************************************************************************
  struct Router1 : public etl::message_router<Router1, Message1, Message2>
  {
    Router1()
      : message_router(RouterId1)
      , count_message1(0)
      , count_message2(0)
      , count_unknown_message(0)
    {
    }

    void on_receive(const Message1&)
    {
      ++count_message1;
    }

    void on_receive(const Message2&)
    {
      ++count_message2;
    }

    void on_receive_unknown(const etl::imessage&)
    {

    }

    void clear()
    {
      count_message1 = 0;
      count_message2 = 0;
      count_unknown_message = 0;
    }

    int count_message1;
    int count_message2;
    int count_unknown_message;
  };

  //*************************************************************************
  struct Router2 : public etl::message_router<Router2, Message1>
  {
    Router2()
      : message_router(RouterId2)
      , count_message1(0)
      , count_message2(0)
      , count_unknown_message(0)
    {
    }

    void on_receive(const Message1&)
    {
      ++count_message1;
    }

    void on_receive_unknown(const etl::imessage&)
    {
      ++count_unknown_message;
    }

    void clear()
    {
      count_message1 = 0;
      count_message2 = 0;
      count_unknown_message = 0;
    }

    int count_message1;
    int count_message2;
    int count_unknown_message;
  };

  //*************************************************************************
  struct Bus : public etl::message_bus<2U>
  {
  };

  SUITE(test_shared_message)
  {
    Router1 router1;
    Router2 router2;
    Bus     bus;

    using pool_message_parameters = etl::atomic_counted_message_pool::pool_message_parameters<Message1, Message2>;

    etl::fixed_sized_memory_block_allocator<pool_message_parameters::max_size,
                                            pool_message_parameters::max_alignment,
                                            4U> memory_allocator;

    etl::atomic_counted_message_pool message_pool(memory_allocator);

    //*************************************************************************
    class Message2Allocator : public etl::ireference_counted_message_pool
    {
      public:

        static etl::reference_counted_message<Message2, void>& Get()
        {
          static Message2Allocator allocator;
          static Message2 message2;
          static etl::reference_counted_message<Message2, void> rcm2(message2, allocator);

          return rcm2;
        }

        void release(const etl::ireference_counted_message&) override
        {
          // Do nothing.
        }
    };

    //*************************************************************************
    TEST(test_move_constructor)
    {
#include "etl/private/diagnostic_pessimizing_move_push.h"
      etl::shared_message sm1(std::move(etl::shared_message(message_pool, Message1(1))));
#include "etl/private/diagnostic_pop.h"
      CHECK_EQUAL(1, sm1.get_reference_count());
    }

    //*************************************************************************
    TEST(test_default_message_constructor)
    {
      etl::reference_counted_message<Message2, etl::atomic_int>* prcm = message_pool.allocate<Message2>();

      Message2&        m2 = prcm->get_message(); // Check that we can get a non-const reference to the message.
      const Message2& cm2 = prcm->get_message(); // Check that we can get a const reference to the message.

      (void)m2;
      (void)cm2;

      etl::shared_message sm1(*prcm);

      etl::imessage&        im = sm1.get_message(); // Check that we can get a non-const reference to the message.
      const etl::imessage& cim = sm1.get_message(); // Check that we can get a const reference to the message.

      (void)im;
      (void)cim;

      CHECK_EQUAL(1, sm1.get_reference_count());
      CHECK(sm1.is_valid());
    }
    
    //*************************************************************************
    TEST(test_move_assignment)
    {
      etl::shared_message sm2 = etl::shared_message(message_pool, Message1(2));
#include "etl/private/diagnostic_pessimizing_move_push.h"
      sm2 = std::move(etl::shared_message(message_pool, Message1(3)));
#include "etl/private/diagnostic_pop.h"
      CHECK_EQUAL(1, sm2.get_reference_count());
      CHECK(sm2.is_valid());
    }

    //*************************************************************************
    TEST(test_send_to_routers)
    {
      bus.clear();
      bus.subscribe(router1);
      bus.subscribe(router2);
      router1.clear();
      router2.clear();

      etl::shared_message sm1(message_pool, Message1(1)); // sm1 holds a Message1 that is owned by message_pool.
      etl::shared_message sm2(message_pool, Message2());  // sm2 holds a Message2 that is owned by message_pool.
      etl::shared_message sm3(Message2Allocator::Get());  // sm3 holds a Message2 that is owned by a statically allocated message pool.
      etl::shared_message sm4(sm1);                       // sm4 is a copy of sm1.

      bus.receive(sm1);
      bus.receive(sm2);
      bus.receive(sm1);
      bus.receive(sm3);
      etl::send_message(bus, sm4); // sm4 is a copy of sm1. Use non-member function.
      bus.receive(RouterId2, sm1); // Only send sm1 to Router2

      CHECK(sm1.is_valid());
      CHECK(sm2.is_valid());
      CHECK(sm3.is_valid());
      CHECK(sm4.is_valid());

      CHECK_EQUAL(2, sm1.get_reference_count());
      CHECK_EQUAL(1, sm2.get_reference_count());
      CHECK_EQUAL(1, sm3.get_reference_count());
      CHECK_EQUAL(2, sm4.get_reference_count());
      CHECK_EQUAL(3, router1.count_message1);
      CHECK_EQUAL(2, router1.count_message2);
      CHECK_EQUAL(0, router1.count_unknown_message);
      CHECK_EQUAL(4, router2.count_message1);
      CHECK_EQUAL(0, router2.count_message2);
      CHECK_EQUAL(0, router2.count_unknown_message);
    }

    //*************************************************************************
    TEST(test_reference_counted_pool_exceptions)
    {
      using pool_message_parameters = etl::atomic_counted_message_pool::pool_message_parameters<Message1, Message2>;

      etl::fixed_sized_memory_block_allocator<pool_message_parameters::max_size,
                                              pool_message_parameters::max_alignment,
                                              4U> memory_allocator;

      etl::atomic_counted_message_pool message_pool(memory_allocator);

      etl::reference_counted_message<Message1, etl::atomic_int>* prcm;
      CHECK_NO_THROW(prcm = message_pool.allocate<Message1>(1));
      CHECK_NO_THROW(prcm = message_pool.allocate<Message1>(2));
      CHECK_NO_THROW(prcm = message_pool.allocate<Message1>(3));
      CHECK_NO_THROW(prcm = message_pool.allocate<Message1>(4));
      
      (void)prcm;

      try
      {
        prcm = message_pool.allocate<Message1>(5);
      }
      catch (etl::exception e)
      {
        CHECK_EQUAL(std::string("reference_counted_message_pool:allocation failure"), std::string(e.what()));
      }
      
      Message1 message1(6);
      etl::reference_counted_message<Message1, etl::atomic_int> temp(message1, message_pool);

      try
      {
        message_pool.release(temp);
      }
      catch (etl::exception e)
      {
        CHECK_EQUAL(std::string("reference_counted_message_pool:release failure"), std::string(e.what()));
      }
    }
  }
}
