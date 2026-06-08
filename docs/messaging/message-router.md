---
title: message_router
weight: 3
---

A class that will automatically route incoming messages to specific handlers based on the message types declared in the template parameter list. Messages are passed to the receive member function which will static cast it to its real type and call the matching `on_receive` function in the derived class. A compilation error will occur if the matching `on_receive` does not exist.  

The `on_receive` functions are not virtual. The template base class uses `CRTP` to directly call the derived class's functions.  

**Defines the following classes**  
```cpp
etl::imessage_router
etl::message_router
etl::null_message_router
```

Note: This C++03 portion of this header is a generated from `message_router_generator.h`. To handle more than the standard 16 message types then a new one must be generated.  
See [Generators](./generators-tutorial)

## Message router ID
Allowable router IDs run from `0` to `MAX_MESSAGE_ROUTER` (`249`) inclusive.  

Each message router is given an ID. Whether this ID is unique or not depends on how you are using and identifying message routers.  

Note: A message router 'group' is deemed to be a set of routers with identical IDs.  
The default router id is `etl::imessage_router::MESSAGE_ROUTER`.

### Scenario 1
- You never send a message to a router using it's ID.  
- You never use the ID to uniquely identify a router.  
- You do not require the router ID to act as a priority level when subscribing to a message bus.  
- You do not require that messages can be sent to a group.  

All message router IDs can be identical.  

### Scenario 2
- You never use the ID to uniquely identify a router.  
- You may use the router ID to send to a particular router group.  
- You require the router ID to act as a priority level when subscribing to a message bus.  

Router IDs will be assigned in groups. i.e. Some routers may share IDs.  

### Scenario 3
- You use the ID to uniquely identify a router.  
- You use the router ID to send to a particular router.  
- You require the router ID to act as a priority level when subscribing to a message bus.  
- You require all priority levels to be unique.  

All router IDs are unique  

## imessage_router
The base class for all routers.

#### Member functions
```cpp
virtual ~imessage_router() {}
```

---

```cpp
virtual void receive(const etl::imessage& message) = 0;
```
Overridden by the derived class.

---

```cpp
virtual void receive(etl::message_router_id_t destination_router_id, 
                     const etl::imessage& message)
```                     
Receive a message for a specific destination id.  
If the destination id is the router's id, then the message is forwarded to `receive(message)`.    
Can be overridden by the derived class.

---

```cpp
virtual void receive(etl::shared_message shared_msg)
```
Receive a shared message.  
By default, forwards to `receive(shared_msg.get_message())`.  

---

```cpp
virtual void receive(etl::message_router_id_t destination_router_id, 
                     etl::shared_message shared_msg)
```
Receive a message for a specific destination id.  
By default, forwards to `receive(destination_router_id, shared_msg.get_message())`.  

---

```cpp
virtual bool accepts(etl::message_id_t id) const = 0;
```
Returns `true` if the router accepts the message id.  
Overridden by the derived class.

---

```cpp
bool accepts(const etl::imessage& msg) const;
```
Returns `true` if the router accepts the message.

---

```cpp
etl::message_router_id_t get_message_router_id() const;
```
Returns the router id.

---

```cpp
virtual bool is_null_router() const = 0;
```
Returns `true` if the router is a null message router, otherwise `false`.  
**Deprecated. **

---

```cpp
virtual bool is_producer() const = 0;
```
Returns `true` if the router is a producer of messages, otherwise `false`.

---

```cpp
virtual bool is_consumer() const = 0;
```
Returns `true` if the router is a consumer of messages, otherwise `false`.

---

```cpp
void set_successor(etl::imessage_router& successor);
```
Sets the successor router. Any unhandled message will be sent here.  
Allows the router to implement the Chain Of Responsibility design pattern.  

## Enumerations
```cpp
NULL_MESSAGE_ROUTER
MESSAGE_BUS
ALL_MESSAGE_ROUTERS
MAX_MESSAGE_ROUTER
```

---

## message_router
User defined message routers are derived from this class.  
Derived from `imessage_router`.

---

### For C++03
```cpp
template <typename TDerived,
          typename T1,         typename T2  = void, typename T3  = void, typename T4  = void, 
          typename T5  = void, typename T6  = void, typename T7  = void, typename T8  = void, 
          typename T9  = void, typename T10 = void, typename T11 = void, typename T12 = void, 
          typename T13 = void, typename T14 = void, typename T15 = void, typename T16 = void>
class message_router
```
**Template parameters**  
`TDerived` The derived class.  
`T1`       The first message type.  
`T2...`    The additional message types.  

The maximum number of types can be set by running the generator for this file.  
The default is 16.This may be changed by running a modified version of generate_message_router.bat.  
See [Generators](../tutorials/generators-tutorial)

---

### For C++11 and above
```cpp
template <typename TDerived, typename... TMessageTypes>
class message_router
```
**Template parameters**  
`TDerived`      The derived class.  
`TMessageTypes` The list of message types.  

**Before: 20.47.0**  
This definition will automatically be selected if the compiler supports C++17 or above. It uses fold expressions to resolve `on_receive()` calls.  
To use the older C++03 compatible definition, define `ETL_MESSAGE_ROUTER_FORCE_CPP03` as a project setting or in the optional `etl_profile.h`.

**Since: 20.47.0**  
This definition will automatically be selected if the compiler supports C++11 or above. It uses either an O(1) or O(logN) mechanism to resolve `on_receive()` calls.  
To use the older C++03 compatible definition, define `ETL_MESSAGE_ROUTER_FORCE_CPP03` as a project setting or in the optional `etl_profile.h`.

### The derived class must define the following member functions.  

```cpp
void on_receive(const Type& msg);
```
Replace Type with each concrete message type.  
Define for all of the template parameter types.

---

```cpp
void on_receive_unknown(const etl::imessage& msg)
```
Called when a message type is received that is not in the template list.

## message_packet

This is a nested member class.

See [etl::message_packet](./message-packet)  

---

### Member functions
```cpp
message_router()
```
Constructs the router.  
The router id is set to `etl::imassage_router::MESSAGE_ROUTER`.

---

```cpp
message_router(etl::imessage_router& successor)
```
Constructs the router.  
The router id must be between `0` and `MAX_MESSAGE_ROUTER` (`249`). Other IDs are reserved for ETL use.  
Emits an error if the id is outside the legal range.  
Routers may have duplicate ids.  
Sets the successor router to this one. Any unhandled message will be sent here.  

---

```cpp
message_router(etl::message_router_id_t id)
```
Constructs the router.  
The router id must be between `0` and `249`. Ids `250` to `255` are reserved for ETL use.  
Asserts an error if the id is outside the legal range.  
Routers may have duplicate ids.

---

```cpp
message_router(etl::message_router_id_t id, etl::imessage_router& successor)
```
Constructs the router.  
The router id must be between 0 and MAX_MESSAGE_ROUTER (249) . Other IDs are reserved for ETL use.  
Sets the successor router to this one. Any unhandled message will be sent here.

---

```cpp
void receive(const etl::imessage& msg)
```
Receives a message and routes to the `on_receive()` handler.  
If not handled, passes the message on to a successor, if it exists.

---

```cpp
template <typename TMessage>
void receive(const TMessage& msg)
```
Receives a message and routes directly to the `on_receive()` handler.  
This template  function will be chosen for concrete message types.  
If not handled, passes the message on to a successor, if it exists.

---

```cpp
bool accepts(etl::message_id_t id) const
```
Returns `true` if the router accepts the message id.

---

### Errors
```cpp
message_router_exception
```
Base exception for router errors.  
Derived from `etl::exception`.

---

```cpp
message_router_illegal_id
```
The router id is out of the legal range.  
Derived from `etl::message_router_exception`.

---

### null_message_router
This router can be used as a sink for messages or a 'null source' router.  
Derived from `etl::imessage_router`.

---

```cpp
null_message_router()
```
Constructs a null message router.  
The router id will be `etl::imessage_router::NULL_MESSAGE_ROUTER`.

---

```cpp
void receive(const etl::imessage& message)
```
Receives a message.  
Does nothing, except pass on to a successor, if it exists.

---

```cpp
bool accepts(etl::message_id_t id) const
```
Returns `false` if no successor is set.  
Returns `true` if a successor accepts the message id.

---

```cpp
static null_message_router& instance()
```
Returns an instance of `etl::null_message_router`.

## message_producer
This router can be used as a producer-only of messages, such an interrupt routine.  
Derived from `etl::imessage_router`.

---

```cpp
message_producer(etl::message_router_id_t id);
```
Constructs the router.  
The router id must be between `0` and `249`. Ids `250` to `255` are reserved for ETL use.
Emits an error if the id is outside the legal range.  
Routers may have duplicate ids.  

---

```cpp
void receive(const etl::imessage& message)
```
Receives a message.  
Does nothing, except pass on to a successor, if it exists.

---

```cpp
bool accepts(etl::message_id_t id) const
```
Returns `false` if no successor is set.  
Returns `true` if a successor accepts the message id.

---

```cpp
static null_message_router& instance()
```
Returns an instance of `etl::null_message_router`.

---

## Global functions

```cpp
void send_message(etl::imessage_router& router,
                  const etl::imessage&  message)
```
Send the message to the router.

---

```cpp
void send_message(etl::imessage_router&    router,
                  etl::message_router_id_t id,
                  const etl::imessage&     message)
```
Send the message to the router if it has the specified id.

---

```cpp
void send_message(etl::imessage_router& router,
                  etl::shared_message&  message)
```
Send the shared message to the router.

---

```cpp
void send_message(etl::imessage_router&    router,
                  etl::message_router_id_t id,
                  etl::shared_message&     message)
```
Send the shared message to the router if it has the specified id.

## Example

```cpp
// Message ids.
enum
{
  START,
  STOP,
  SET_SPEED
};

// The start message.
struct Start : public etl::message<START>
{
};

// The stop message.
struct Stop : public etl::message<STOP>
{
};

// The set speed message.
struct SetSpeed : public etl::message<SET_SPEED>
{
  SetSpeed(uint32_t speed_)
    : speed(speed_)
  {
  }

  uint32_t speed;
};

// The router.
class Router : public etl::message_router<Router, Start, Stop, SetSpeed>
{
public:

  // Construct the router with an id of 0.
  Router()
    : message_router(0)
  {
  }

  // Received a start message.
  void on_receive(etl::imessage_router& sender, const Start& msg)
  {
    std::cout << "Start message received\n";
  }

  // Received a stop message.
  void on_receive(const Stop& msg)
  {
    std::cout << "Start message received\n";
  }

  // Received a set speed message.
  void on_receive(const SetSpeed& msg)
  {
    std::cout << "SetSpeed(" << msg.speed << ") message received\n";
  }

  // Received an unknown message.
  void on_receive_unknown(const etl::imessage& msg)
  {
    std::cout << "Unknown message " << msg.message_id << " received\n";
  }
}

// Router and message instances.
Router   router;
Start    start;
Stop     stop;
SetSpeed halfSpeed(50);
SetSpeed maxSpeed(100);

// A queue for Router messages.
etl::queue<Router::message_packet, 10> queue;

// Add to the queue.
queue.emplace(start);
queue.emplace(stop);
queue.emplace(maxSpeed);
queue.emplace(halfSpeed);

// Send the queued messages to 'router'.
while (!queue.empty())
{
  etl::send_message(router, queue.front().get());
  queue.pop();
}
```

---

An example of a queued message router can be found in the repository in `examples/QueuedMessageRouter`.
