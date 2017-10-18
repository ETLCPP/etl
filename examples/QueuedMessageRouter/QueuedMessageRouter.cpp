
#include "queue.h"
#include "message_router.h"

#include <iostream>
#include <string>

//*****************************************************************************
// The messages.
//*****************************************************************************
struct Message1 : public etl::message<1>
{
  Message1(int i_)
    : i(i_)
  {
  }

  int i;
};

struct Message2 : public etl::message<2>
{
  Message2(double d_)
    : d(d_)
  {
  }

  double d;
};

struct Message3 : public etl::message<3>
{
  Message3(const std::string& s_)
    : s(s_)
  {
  }

  std::string s;
};

struct Message4 : public etl::message<4>
{
};

//*****************************************************************************
// The message router.
// Handles message types Message1, Message2, Message3.
//*****************************************************************************
class Router : public etl::message_router<Router, Message1, Message2, Message3>
{
public:

  typedef etl::message_router<Router, Message1, Message2, Message3> Base_t;

  using Base_t::receive;

  //***************************************************************************
  Router()
    : message_router(1)
  {
  }

  //***************************************************************************
  // Override the base class's receive function.
  void receive(etl::imessage_router& sender_, const etl::imessage& msg_)
  {
    if (accepts(msg_))
    {
      // Place in queue.
      queue.emplace(&sender_, msg_);

      std::cout << "Queueing message " << int(msg_.message_id) << std::endl;
    }
    else
    {
      std::cout << "Ignoring message " << int(msg_.message_id) << std::endl;
    }
  }

  //***************************************************************************
  void process_queue()
  {
    while (!queue.empty())
    {
      Item& item = queue.front();
      etl::imessage& msg = item.packet.get();
      etl::imessage_router& sender = *item.sender;
      std::cout << "Processing message " << int(msg.message_id) << std::endl;

      // Call the base class's receive function.
      // This will route it to the correct on_receive handler.
      Base_t::receive(sender, msg);

      queue.pop();
    }
  }

  //***************************************************************************
  void on_receive(etl::imessage_router& sender, const Message1& msg)
  {
    std::cout << "  Received message " << int(msg.message_id) << " : '" << msg.i << "'" << std::endl;
  }

  //***************************************************************************
  void on_receive(etl::imessage_router& sender, const Message2& msg)
  {
    std::cout << "  Received message " << int(msg.message_id) << " : '" << msg.d << "'" << std::endl;
  }

  //***************************************************************************
  void on_receive(etl::imessage_router& sender, const Message3& msg)
  {
    std::cout << "  Received message " << int(msg.message_id) << " : '" << msg.s << "'" << std::endl;
  }

  //***************************************************************************
  void on_receive_unknown(etl::imessage_router& sender, const etl::imessage& msg)
  {
    std::cout << "  Received unknown message " << int(msg.message_id) << std::endl;
  }

private:

  struct Item
  {
    Item(etl::imessage_router* sender_, const etl::imessage& msg_)
      : sender(sender_),
        packet(msg_)
    {
    }

    etl::imessage_router* sender;
    message_packet        packet;
  };

  etl::queue<Item, 10> queue;
};

//*****************************************************************************
// The test application.
//*****************************************************************************
int main()
{
  Router router;

  Message1 m1(1);
  Message2 m2(1.2);
  Message3 m3("Hello");

  etl::send_message(router, m1);
  etl::send_message(router, Message1(2));
  etl::send_message(router, m2);
  etl::send_message(router, Message2(3.4));
  etl::send_message(router, m3);
  etl::send_message(router, Message3("World"));
  etl::send_message(router, Message4());

  std::cout << std::endl;

  router.process_queue();

  return 0;
}

