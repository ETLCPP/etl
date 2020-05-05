/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl
https://www.etlcpp.com

Copyright(c) 2020 jwellbelove

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

#include "UnitTest++/UnitTest++.h"

#include "etl/pool.h"
#include "etl/shared_message.h"
#include "etl/message_pool.h"
#include "etl/variant_pool.h"
#include "etl/message.h"
#include "etl/atomic.h"

#include "etl/queue.h"
#include "etl/ishared_message_processor.h"

namespace
{
  //*************************************************************************
  struct Message1 : public etl::message<1, uint32_t>
  {
  };

  //*************************************************************************
  struct Router : public etl::message_router<Router, Message1>
  {
    Router()
      : message_router(1)
    {

    }

    void on_receive(const Message1& message)
    {

    }

    void on_receive(etl::imessage_router& source, const Message1& message)
    {

    }

    void on_receive_unknown(etl::imessage_router& source, const etl::imessage& message)
    {

    }
  };

  //*************************************************************************
  struct Processor : public etl::ishared_message_processor
  {
    Processor(etl::imessage_router& router_)
      : router(router_)
    {
    }

    void receive(etl::shared_message message)
    {
      uint32_t count1 = message.get_reference_count();

      smqueue.push(message);

      uint32_t count2 = message.get_reference_count();
    }

    void receive(etl::imessage_router& source, etl::shared_message message)
    {

    }

    void Process()
    {
      router.receive(smqueue.front().get_message());
      smqueue.pop();
    }

    etl::imessage_router& router;
    etl::queue<etl::shared_message, 10> smqueue;
  };

  Router    router;
  Processor processor(router);

  SUITE(test_shared_message)
  {
    constexpr size_t POOL_SIZE = 2;

    etl::variant_pool<POOL_SIZE, Message1> vpool;

    etl::message_pool mp(vpool);

    //*************************************************************************
    TEST(test_buffered)
    {     
      etl::message_pool& mpool = mp;

      {
        etl::shared_message* pbuffered_sm = nullptr;

        {
          etl::shared_message sm(mpool, *mpool.create<Message1>());
          uint32_t count1 = sm.get_reference_count();

          processor.receive(sm);

          uint32_t count1b = sm.get_reference_count();

          processor.Process();

          uint32_t count1c = sm.get_reference_count();

          pbuffered_sm = new etl::shared_message(sm);
          //uint32_t count2 = sm.get_reference_count();
          //uint32_t count3 = pbuffered_sm->get_reference_count();

          etl::shared_message sm2 = etl::shared_message(mpool, *mpool.create<Message1>());
          uint32_t count4 = sm2.get_reference_count();

          pbuffered_sm = pbuffered_sm;
        }

        size_t ps1 = vpool.size();

        //{
        //  uint32_t count = pbuffered_sm->get_reference_count();
        //  etl::message_id_t id = pbuffered_sm->get_message().get_message_id();
        //}

        size_t ps2 = vpool.size();

        delete pbuffered_sm;
      }

      size_t ps3 = vpool.size();

      vpool.size();
    }
  }

  ////*************************************************************************
  //TEST(test_construct_and_destruct)
  //{
  //  etl::pool<Message1, 2> pool;
  //  etl::pool_chain<1> pool_chain;

  //  pool_chain.add(pool);

  //  etl::shared_message buffered_sm(pool_chain);

  //  {
  //    etl::shared_message sm1(pool_chain, pool.create());
  //    CHECK_EQUAL(1, sm1.get_reference_count());
  //    CHECK_EQUAL(1, pool.size());
  //    {
  //      etl::shared_message sm2(sm1);
  //      CHECK_EQUAL(1, sm1.get_reference_count());
  //      CHECK_EQUAL(2, sm2.get_reference_count());
  //      CHECK_EQUAL(1, pool.size());
  //      {
  //        etl::shared_message sm3(pool_chain);
  //        sm3 = sm2;
  //        CHECK_EQUAL(1, sm1.get_reference_count());
  //        CHECK_EQUAL(2, sm2.get_reference_count());
  //        CHECK_EQUAL(3, sm3.get_reference_count());
  //        CHECK_EQUAL(1, pool.size());

  //      }
  //      CHECK_EQUAL(1, pool.size());
  //    }
  //    CHECK_EQUAL(1, pool.size());
  //  }
  //  CHECK_EQUAL(0, pool.size());
  //}

}
