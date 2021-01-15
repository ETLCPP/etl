// SharedMessage.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "etl/shared_message.h"
#include "etl/message.h"
#include "etl/reference_counted_message_pool.h"
#include "etl/message_router.h"
#include "etl/message_bus.h"
#include "etl/fixed_sized_memory_block_allocator.h"
#include "etl/queue.h"

#include <iostream>
#include <atomic>
#include <string>

constexpr etl::message_router_id_t RouterId1 = 1U;
constexpr etl::message_router_id_t RouterId2 = 2U;

//*****************************************************************************
struct Message1 : public etl::message<1>
{
  Message1(std::string s_)
    : s(s_)
  {
    
  }
  
  std::string s;
};

//*****************************************************************************
struct Message2 : public etl::message<2>
{
  Message2(std::string s_)
    : s(s_)
  {
    
  }
  
  std::string s;
};

//*****************************************************************************
void Print(const std::string& prefix, const etl::shared_message& sm)
{
  std::cout << prefix << " : " << int(sm.get_message().get_message_id()) << "\n";
}

//*****************************************************************************
class MessageRouter1 : public etl::message_router<MessageRouter1, Message1, Message2>
{
public:

  //****************************************
  MessageRouter1()
    : message_router(RouterId1)
  {
  }

  //****************************************
  void on_receive(etl::imessage_router& source, const Message1& msg)
  {
    std::cout << "on_receive Message1 : " << msg.s << "\n";
  }

  //****************************************
  void on_receive(etl::imessage_router& source, const Message2& msg)
  {
    std::cout << "on_receive Message2 : " << msg.s << "\n";
  }

  //****************************************
  void on_receive_unknown(etl::imessage_router& source, const etl::imessage& msg)
  {
    std::cout << "on_receive Unknown\n";
  }
};

//*****************************************************************************
class MessageRouter2 : public etl::message_router<MessageRouter2, Message1, Message2>
{
public:

  using base_t = etl::message_router<MessageRouter2, Message1, Message2>;

  //****************************************
  MessageRouter2()
    : message_router(RouterId2)
  {
  }

  using base_t::receive;

  //****************************************
  void receive(etl::imessage_router& source, etl::shared_message shared_msg) override
  {
    if (!queue.full())
    {
      Print("Queueing shared message", shared_msg);

      queue.push(QueuedData{ &source, shared_msg });
    }
  }

  //****************************************
  void process_queue()
  {
    while (!queue.empty())
    {
      // Get the shared message from the queue.
      QueuedData data = queue.front();

      Print("Process queued shared messages", data.shared_msg);

      // Send it to the base implementation for routing.
      base_t::receive(*data.source, data.shared_msg);

      queue.pop();
    }
  }

  //****************************************
  void on_receive(etl::imessage_router& source, const Message1& msg)
  {
    std::cout << "on_receive Message1 : " << msg.s << "\n";
  }

  //****************************************
  void on_receive(etl::imessage_router& source, const Message2& msg)
  {
    std::cout << "on_receive Message2 : " << msg.s << "\n";
  }

  //****************************************
  void on_receive_unknown(etl::imessage_router& source, const etl::imessage& msg)
  {
    std::cout << "on_receive Unknown\n";
  }

private:

  struct QueuedData
  {
    etl::imessage_router* source;
    etl::shared_message   shared_msg;
  };

  etl::queue<QueuedData, 10> queue;
};

//*****************************************************************************
struct Bus : public etl::message_bus<2U>
{
};

//*****************************************************************************
MessageRouter1 router1;
MessageRouter2 router2;
Bus bus;

//*****************************************************************************
// The memory block allocator that supplies the pool with memory 
// to store reference counted messages in. 
etl::fixed_sized_memory_block_allocator<100U, 4U, 4U> memory_pool;

//*****************************************************************************
// The pool that supplies reference counted messages.
// Uses memory_pool as its allocator.
etl::reference_counted_message_pool<std::atomic_int32_t> pool(memory_pool);

//*****************************************************************************
int main()
{
  Message1 m1("One");
  Message2 m2("Two");

  etl::shared_message sm1(pool, m1);
  etl::shared_message sm2(pool, m2);

  bus.subscribe(router1);
  bus.subscribe(router2);

  bus.receive(sm1);
  bus.receive(sm2);

  router2.process_queue();
}

