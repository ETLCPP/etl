
#include <iostream>
#include <thread>
#include <atomic>
#include <string>
#include <chrono>

#include "etl/mutex.h"
#include "etl/message.h"
#include "etl/message_router.h"

//*************************************
struct Message1 : public etl::message<1>
{
};

//*************************************
struct Message2 : public etl::message<2>
{
};

//*************************************
class Router : public etl::message_router<Router, Message1, Message2>
{
public:

  using base_t = etl::message_router<Router, Message1, Message2>;

  //*****************
  // Overridden receive that protects access with mutexes
  void receive(const etl::imessage& msg) override
  {
    access.lock();
    base_t::receive(msg); // Send it to the message_router's receive.
    access.unlock();
  }

  //*****************
  void on_receive(const Message1&)
  {
    result.append("Message1\n");
  }

  //*****************
  void on_receive(const Message2&)
  {
    result.append("Message2\n");
  }

  //*****************
  void on_receive_unknown(const etl::imessage&)
  {
  }

  std::string result;

private:

  etl::mutex access;
};

//*************************************
etl::atomic<bool> start = false;
Router router;

//*************************************
void thread1()
{
  using namespace std::chrono_literals;

  while (!start.load());

  for (int i = 0; i < 10; ++i)
  {
    std::this_thread::sleep_for(1ms);
    router.receive(Message1());
  }
}

//*************************************
void thread2()
{
  using namespace std::chrono_literals;

  while (!start.load());

  for (int i = 0; i < 10; ++i)
  {
    std::this_thread::sleep_for(1ms);
    router.receive(Message2());
  }
}

//*************************************
int main()
{
  std::thread t1(thread1);
  std::thread t2(thread2);

  start.store(true);

  t1.join();
  t2.join();

  std::cout << router.result << "\n";

  return 0;
}


