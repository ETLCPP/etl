---
title: "message_bus"
---

{{< callout type="info">}}
  Header: `message_broker.h`  
  Since: `20.33.0`
{{< /callout >}}

Message Bus  

This page documents version `20.0.0` and above.  

A variant of the observer pattern in that message routers and derived types are be able to subscribe to messages on a bus. The messages can be either broadcast, to be automatically picked up by any router that has a handler, or addressed to a particular router or router id. Message buses may be nested  by setting a successor.  

## imessage_bus
Derived from imessage_router.  

The base for all message buses.  
Inherits publicly from `etl::imessage_router`.  
Message buses are therefore also a type of  router.  
Objects of type `etl::imessage_bus` cannot be directly constructed.  

## Member functions

```cpp
bool subscribe(etl::imessage_router& router)
```
Subscribes an `etl::imessage_router` derived class to the bus.  
Returns `true` on success.

---

```cpp
void unsubscribe(etl::imessage_router& router)
```
Unsubscribes the specified `etl::imessage_router` derived class from the bus.  
Does not unsubscribe from nested buses.

---

```cpp
void unsubscribe(etl::message_router_id_t id)
```
Unsubscribes routers with the specified id from the bus.  
Does not unsubscribe from nested buses.  

`etl::imessage::MESSAGE_BUS` will unsubscribe all message buses.  
`etl::imessage::ALL_MESSAGE_ROUTERS` will unsubscribe all routers and buses. Equivalent to calling `clear()`.

---

```cpp
void receive(const etl::imessage& message)
void receive(etl::shared_message message)
```
Receives a message and distributes it to all subscribers.  
Forwards the message to any successor.  

The routers are called first, in order of ascending router id.  
Routers with the duplicate ids will be called in subscribe order.  
Any nested message buses are called in subscribe order.

---

```cpp
void receive(etl::message_router_id_t destination_router_id, 
             const etl::imessage&     message);
void receive(etl::message_router_id_t destination_router_id, 
             etl::shared_message message)
```
Receives a message and distributes it to all subscribers that have the specified router id.  
Forwards the message to any successor.  

Routers with the duplicate ids will be called in subscribe order.  
Any nested message buses are called in subscribe order.  

Override this in a derived class if you wish to capture messages sent to the bus.  
Call the base receive function from here to allow normal operation to continue.

---

```cpp
bool accepts(etl::message_id_t id) const
```
Always returns `true`.

---

```cpp
size_t size() const
```
Returns the number of subscribers.

---

```cpp
void clear()
```
Clears the bus of all subscribers.  

Message buses inherit all of the public functions of `etl::imessage_router`.

## Errors

```cpp
message_bus_exception
```
Base error class for `etl::message_bus`. Inherits from `etl::exception`.

---

```cpp
message_bus_too_many_subscribers
```
Emitted when the number of subscribers exceeds the capacity. Inherits from `etl::message_bus_exception`.

## message_bus
Derived from `imessage_bus`.

```cpp
template <const uint_least8_t MAX_ROUTERS>
class message_bus
```
`MAX_ROUTERS` The maximum number of routers that can be subscribed.

## Member functions

```cpp
message_bus()
```
Constructs a message bus.  
Message buses always have a router id of `etl::imessage::MESSAGE_BUS`.

```cpp
message_bus(etl::imessage_router& successor)
```
Constructs a message bus and sets the successor.  
Message buses always have a router id of `etl::imessage::MESSAGE_BUS`.

## Example
```cpp
// Some router ids.
enum
{
  ROUTER_ID_1,
  ROUTER_ID_2,
  ROUTER_ID_3
};

// Instances of messages.
MessageA messageA;

// Instances of message routers.
RouterA routerA(ROUTER_ID_1);
RouterB routerB(ROUTER_ID_1);
RouterC routerC(ROUTER_ID_2);
RouterD routerD(ROUTER_ID_3);
RouterE routerE(ROUTER_ID_1);

// Instances of message buses.
etl::message_bus<4> bus1;
etl::message_bus<2> bus2;
etl::message_bus<1> bus3;

// Subscribe bus2 & bus3 to bus1.
bus1.subscribe(bus3);
bus1.subscribe(bus2);

// Subscribe routerB & routerA to bus1.
bus1.subscribe(routerB);
bus1.subscribe(routerA);

// Subscribe routerD & routerC to bus2.
bus2.subscribe(routerD);
bus2.subscribe(routerC);

// Subscribe routerE to bus3.
bus3.subscibe(routerE);

// Assume all routers accept the same messages.

// Broadcast messageA to everyone.
bus1.receive(messageA);

// The call order will be...
// routerB
// routerA
// routerE
// routerC
// routerD

// Address messageA to routers with id ROUTER_ID_1.
bus1.receive(ROUTER_ID_1, messageA);

// The call order will be...
// routerB
// routerA
// routerE

// Address messageA to routers with id ROUTER_ID_3.
bus1.receive(ROUTER_ID_3, messageA);
```
// The call order will be...
// routerD
```
