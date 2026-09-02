---
title: "Messages"
---

A tutorial on how the ETL's messaging framework can be used.

## Overview

The messaging framework is designed to allow a powerful, but easy to use, message routing and handling system to be created. It comprises of a collection of messages, message routers, message buses and finite state machines.  
Routers, buses and state machines share a unified API that allows different parts of the framework to be mixed and matched.  

For example, a message source may pass messages to a message bus. The message bus may then forward messages to a number of other message recipients such as other buses, routers and FSMs. The sender of the message does not need to know the exact type of the receiver, only that it can receive messages.

## Messages

```cpp
etl::message
```

All messages are untimately derived from `etl::imessage`. 
They are directly derived from the template `etl::message<const etl::message_id_t ID>`.
Each message must have an id that is at least unique to the receiver, though normally ids would be unique across the application.

```cpp
enum class MessageId : etl::message_id_t
{
  StartMotorId,
  StopMotorId
};

struct StartMotor : public etl::message<StartMotorId>
{
};

struct StopMotor : public etl::message<StopMotorId>
{
};
```

Messages are normally passed around as `const references` to `etl::imessage`.

By default, messages are not virtual. This may be changed by defining `ETL_POLYMORPHIC_MESSAGES` either in the project properties or in `etl_profile.h`.

By default the message id is a `uint_least8_t`. This may be changed by defining `ETL_MESSAGE_ID_TYPE` as the required type.

## Message Routers

```cpp
etl::message_router
```
Messages routers are defined to handle a specified set of messages. Defining a message router will mandate that a specific set of `on_receive` functions to handle each message type be defined, along with an 'unknown' handler.
Failure to define the full set of handler functions will result in a compile error.

Message routers may either handle the message directly or implement message queues to defer handling.
The message router uses a message packet class for implementing heterogeneous message queues.
See the `QueuedMessageRouter` project in the `examples` directory.

## Message Buses

```cpp
etl::message_bus
```
Message buses are a type of message router.
Message buses distribute messages to other message recipients. They do not process any messages.
Other message recipients (those classes derived from etl::imessage_router)  may subscribe to the bus.
Messages sent to the bus may either be broadcast or directed only to routers with a specified id.

## Message Timer

```cpp
etl::message_timer
```
The message timer is a generator of messages. Up to 254 timers may be defined that can be configured to send a message to a router, bus or FSM at periodic intervals.

## Finite State Machines (FSM & HFSM)

```cpp
etl::fsm
etl::hfsm
```
FSMs are also a type of message router.
Messages sent to FSMs are handled in on_event handlers in classes derived from `etl::fsm_state`.
The states are defined to handle a specified set of messages. Failure to define the full set of event functions will result in a compile error. Messages sent to states that have not been define to handle them will call an 'Unknown' handler.
The FSM class uses the state pattern to change message handlers on state changes.

By default the state id is a `uint_least8_t`. This may be changed by defining `ETL_STATE_ID_TYPE` as the required type.
