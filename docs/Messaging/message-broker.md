---
title: "message_broker"
---

{{< callout type="info">}}
  Header: `message_broker.h`  
{{< /callout >}}

Message Broker  

A variant of the observer pattern in that message routers and derived types are be able to subscribe to selected sets of messages. The message_broker is similar to the message_bus, but it provides more control over the routing of messages.  While the message_bus simply broadcasts every message to all subscribers, the message_broker allows you to specify which subscribers should receive each message.  

Derived from `imessage_router`.

## Types

```cpp
message_id_span_t    etl::span<const etl::message_id_t>
```

## subscription
A nested class of `etl::message_broker`.
The base for broker subscription information.  
Derive from this to define your subscription class.  
See Example.

---

```cpp
subscription(etl::imessage_router& router)
```
Constructor.

---

```cpp
virtual message_id_span_t message_id_list() const = 0;
```
Override this to return a span of message ids.

## message_broker

```cpp
message_broker()
```
The broker is constructed with an id of `etl::imessage_router::MESSAGE_BROKER`.

---

```cpp
message_broker(etl::imessage_router& successor)
```
The broker is constructed with an id of `etl::imessage_router::MESSAGE_BROKER`.  
Sets the successor.

---

```cpp
message_broker(etl::message_router_id_t id)
```
The broker is constructed with the specified id.

---

```cpp
message_broker(etl::message_router_id_t id, etl::imessage_router& successor)
```
The broker is constructed with the specified id.  
Sets the successor.

---

```cpp
void subscribe(etl::imessage_router& router)
```
Subscribes an `etl::imessage_router` derived class to the broker.  
A subscription object must have a lifetime of at least the same as the broker.  
A subscription cannot be shared with another broker.

---

```cpp
void unsubscribe(etl::imessage_router& router)
```
Unsubscribes the specified `etl::imessage_router` derived class from the bus.  
Does not unsubscribe from nested buses.

---

```cpp
void receive(const etl::imessage& message)
void receive(etl::shared_message message)
```
Receives a message and distributes it to all subscribers that have registered to receive the message type.  
Forwards the message to any successor.  

Override this in a derived class if you wish to capture messages sent to the broker.  
Call the base receive function from here to allow normal operation to continue.

---

```cpp
bool accepts(etl::message_id_t id) const
```
Always returns `true`.

---

```cpp
void clear()
```
Clears the broker of all subscribers.

---

```cpp
ETL_DEPRECATED bool is_null_router() const ETL_OVERRIDE
```
Always returns `false`.

---

```cpp
bool is_producer() const ETL_OVERRIDE
```
Always returns `true`.

---

```cpp
bool is_consumer() const ETL_OVERRIDE
```
Always returns `true`.

---

```cpp
bool empty() const
```
Returns `true` is the are are no subscribers.

## Example
```cpp
// Some router ids.
enum
{
  ROUTER_ID_1,
  ROUTER_ID_2,
};

// Custom subscription type.
class Subscription : public etl::message_broker::subscription
{
public:

  Subscription(etl::imessage_router& router, std::initializer_list<etl::message_id_t> id_list_)
    : etl::message_broker::subscription(router)
    , id_list(id_list_)
  {
  }

  etl::message_broker::message_id_span_t message_id_list() const override
  {
    return etl::message_broker::message_id_span_t(id_list.begin(), id_list.end());
  }

  std::vector<etl::message_id_t> id_list;
};

// Instances of messages.
Message1 message1;
Message2 message2;
Message3 message3;
Message4 message4;

// Custom broker.
class Broker : public etl::message_broker
{
public:

  using etl::message_broker::receive;

  // Hook incoming messages and translate Message4 to Message3.
  void receive(const etl::imessage& msg) override
  {
    if (msg.get_message_id() == Message4::ID)
    {
      etl::message_broker::receive(Message3());
    }
    else
    {
      etl::message_broker::receive(msg);
    }
  }
};

// Instances of message routers.
Router1 router1;
Router2 router2;

// The subscriptions.
Subscription subscription1{ router1, { Message1::ID, Message2::ID } };
Subscription subscription2{ router2, { Message2::ID, Message3::ID } };

// Instance of message broker.
etl::message_broker broker;

// Subscribe router1 and router1 to the broker.
broker.subscribe(subscription1);
broker.subscribe(subscription2);

broker.receive(message1); // Received by router1
broker.receive(message2); // Received by router1 and router2
broker.receive(message3); // Received by router2
broker.receive(message4); // Received by router2 as a Message3
```
