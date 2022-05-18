//*****************************************************************************
// Shared message example
//*****************************************************************************

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
#include <mutex>

constexpr etl::message_router_id_t RouterId1 = 1U;
constexpr etl::message_router_id_t RouterId2 = 2U;

//*****************************************************************************
// Message1
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
// Message2
//*****************************************************************************
struct Message2 : public etl::message<2>
{
  Message2(std::string s_)
    : s(s_)
  {
    
  }
  
  std::string s;
  char data[100];
};

//*****************************************************************************
// Message3
//*****************************************************************************
struct Message3 : public etl::message<3>
{
  Message3(std::string s_)
    : s(s_)
  {

  }

  std::string s;
};

//*****************************************************************************
// Prints the shared message
//*****************************************************************************
void Print(const std::string& prefix, etl::shared_message sm)
{
  std::cout << prefix << " : Message Id = " << int(sm.get_message().get_message_id()) << "\n";
}

//*****************************************************************************
// This router accepts Message1, Message2 and Message3 types.
// If a shared message it received, it will be processed immediately.
//*****************************************************************************
class MessageRouter1 : public etl::message_router<MessageRouter1, Message1, Message2, Message3>
{
public:

  //****************************************
  MessageRouter1()
    : message_router(RouterId1)
  {
  }

  //****************************************
  void on_receive(const Message1& msg)
  {
    std::cout << "MessageRouter1 : on_receive Message1 : " << msg.s << "\n";
  }

  //****************************************
  void on_receive(const Message2& msg)
  {
    std::cout << "MessageRouter1 : on_receive Message2 : " << msg.s << "\n";
  }

  //****************************************
  void on_receive(const Message3& msg)
  {
    std::cout << "MessageRouter1 : on_receive Message3 : " << msg.s << "\n";
  }

  //****************************************
  void on_receive_unknown(const etl::imessage& msg)
  {
    std::cout << "MessageRouter1 : on_receive Unknown\n";
  }
};

//*****************************************************************************
// This router accepts Message1, Message2 and Message3 types.
// If a shared message it received it will queue them.
// The messages will be processed when process_queue() is called.
//*****************************************************************************
class MessageRouter2 : public etl::message_router<MessageRouter2, Message1, Message2, Message3>
{
public:

  using base_t = etl::message_router<MessageRouter2, Message1, Message2, Message3>;

  //****************************************
  MessageRouter2()
    : message_router(RouterId2)
  {
  }

  using base_t::receive;

  //****************************************
  // Overridden receive.
  // Puts the shared messages into a queue.
  void receive(etl::shared_message shared_msg) override
  {
    if (!queue.full())
    {
      Print("MessageRouter2 : Queueing shared message", shared_msg);

      queue.push(shared_msg);
    }
  }

  //****************************************
  // Processes the queued shared messages.
  void process_queue()
  {
    while (!queue.empty())
    {
      // Get the shared message from the queue.
      etl::shared_message shared_msg = queue.front();

      Print("MessageRouter2 : Process queued shared message", shared_msg);

      // Send it to the base implementation for routing.
      base_t::receive(shared_msg);

      queue.pop();
    }
  }

  //****************************************
  void on_receive(const Message1& msg)
  {
    std::cout << "MessageRouter2 : on_receive Message1 : " << msg.s << "\n";
  }

  //****************************************
  void on_receive(const Message2& msg)
  {
    std::cout << "MessageRouter2 : on_receive Message2 : " << msg.s << "\n";
  }

  //****************************************
  void on_receive(const Message3& msg)
  {
    std::cout << "MessageRouter2 : on_receive Message3 : " << msg.s << "\n";
  }

  //****************************************
  void on_receive_unknown(const etl::imessage& msg)
  {
    std::cout << "MessageRouter2 : on_receive Unknown\n";
  }

private:

  etl::queue<etl::shared_message, 10> queue;
};

//*****************************************************************************
// A message bus that can accommodate two subscribers.
//*****************************************************************************
struct Bus : public etl::message_bus<2U>
{
};

//*****************************************************************************
// Define the routers and bus.
//*****************************************************************************
MessageRouter1 router1;
MessageRouter2 router2;
Bus bus;

//*****************************************************************************
// The thread safe message pool. Uses atomic uint32_t for counting.
class MessagePool : public etl::reference_counted_message_pool<std::atomic_int32_t>
{
public:

  MessagePool(etl::imemory_block_allocator& allocator)
    : reference_counted_message_pool(allocator)
  {
  }

  // Called before the memory block allocator is accessed.
  void lock() override
  {
    mut.lock();
  }

  // Called after the memory block allocator has been accessed.
  void unlock() override
  {
    mut.unlock();
  }

private:

  std::mutex mut;
};

//*****************************************************************************
// The memory block allocator that supplies the pool with memory 
// to store reference counted messages in. 

// The reference counted message parameters type for the messages we will use.
using message_parameters_small = MessagePool::pool_message_parameters<Message1, Message3>;
using message_parameters_large = MessagePool::pool_message_parameters<Message2>;

constexpr size_t max_size_small      = message_parameters_small::max_size;
constexpr size_t max_alignment_small = message_parameters_small::max_alignment;

constexpr size_t max_size_large      = message_parameters_large::max_size;
constexpr size_t max_alignment_large = message_parameters_large::max_alignment;

// A fixed memory block allocator for 4 items, using the parameters from the smaller messages.
etl::fixed_sized_memory_block_allocator<max_size_small, max_alignment_small, 4U> memory_allocator;

// A fixed memory block allocator for 4 items, using the parameters from the larger message.
etl::fixed_sized_memory_block_allocator<max_size_large, max_alignment_large, 4U> memory_allocator_successor;

//*****************************************************************************
// The pool that supplies reference counted messages.
// Uses memory_allocator as its allocator.
//*****************************************************************************
MessagePool message_pool(memory_allocator);

//*****************************************************************************
// A statically allocated reference counted message that is never allocated or released by a pool.
// Contains a copy of Message3("Three").
//*****************************************************************************
etl::persistent_message<Message3> pm3(Message3("Three"));

//*****************************************************************************
int main()
{
  // If memory_allocator can't allocate, then try memory_allocator_successor.
  memory_allocator.set_successor(memory_allocator_successor);

  Message1 m1("One");
  Message2 m2("Two");
  
  etl::shared_message sm1(message_pool, m1); // Created a shared message by allocating a reference counted message from message_pool containing a copy of m1.
  etl::shared_message sm2(message_pool, m2); // Created a shared message by allocating a reference counted message from message_pool containing a copy of m2.
  etl::shared_message sm3(pm3);              // Created a shared message from a statically allocated persistent message.

  bus.subscribe(router1); // Subscribe router1 to the bus.
  bus.subscribe(router2); // Subscribe router2 to the bus.

  bus.receive(sm1); // Send sm1 to the bus for distribution to the routers.
  bus.receive(sm2); // Send sm2 to the bus for distribution to the routers.
  bus.receive(sm3); // Send sm3 to the bus for distribution to the routers.

  router2.process_queue(); // Allow router2 to process its queued messages.
}
