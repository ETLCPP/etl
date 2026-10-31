---
title: "message_timer_locked"
---

{{< callout type="info">}}
  Header: `message_timer_locked.h`  
{{< /callout >}}

A software timer class that can manage up to 254 timers. Each one may be repeating or single shot.  
When a timer triggers it will send the defined message to the selected message router or bus.  
The timers are driven from a call to `tick(uint32_t count)`. This call would normally be made from a high priority interrupt routine. The destination router will receive the message in the same context as the tick call.  
The call to tick has a low overhead when a timer is not 'due'. Internally the timers are stored in 'first timeout' order so only the head of the list needs to be checked.  

Each timer may have a period of up to 2<sup>32</sup>-2 ticks (4,294,967,294).  
At 1ms per tick this would equate to just over 49 days.  

**Defines the following classes**  
```cpp
etl::imessage_timer_locked
etl::message_timer_locked
```

The access to the timers is controlled by three external functions, supplied to the timer by the member function set_locks.  

**The three delegate parameters are**  
`Try Lock`  Attempts to set the lock. Returns `true` if successful, otherwise `false`.  
`Lock`      Sets the lock.  
`Unlock`    Resets the lock.  

Uses definitions from `timer.h`  

## imessage_timer_atomic

The base class for all timer controllers.

### Member functions
```cpp
etl::timer::id::type register_timer(const etl::imessage&     message,                                                   
                                    etl::imessage_router&    router,
                                    uint32_t                 period,
                                    bool                     repeating,
                                    etl::message_router_id_t destination_router_id =                   
                                                         etl::imessage_router::ALL_MESSAGE_ROUTERS)
```
**Description**  
Registers a timer.

**Parameters**  
`message`               A reference to the message that will be sent when the timer expires.  
`router`                A reference to a router or bus that will receive to message.  
`period`                The timer period in ticks.  
`repeating`             `false` if single shot, `true` if repeating.  
`destination_router_id` The id of the destination router. Only valid if the router is a bus. Default to all routers.

**Return**  
The allocated timer id or `etl::timer::id::NO_TIMER` if one was not available.

---

```cpp
bool unregister_timer(etl::timer::id::type id)
```
**Description**  
Unregisters a timer.
If the timer is active then it will be stopped.
Returns `true` if a timer with the id was successfully unregistered.

---

```cpp
void enable(bool state)
```
**Description**  
Enables or disables the timer manager according to the state.

---

```cpp
bool is_running() const
```
**Description**  
Returns `true` if the timer manager is enabled.

---

```cpp
void clear()
```
**Description**  
Clears the message timer back to the initial state. All timers will be stopped and unregistered.

---

```cpp
bool tick(uint32_t count)
```
**Description**  
This function updates the internal tick counter (if enabled) and must be passed the number of ticks that have occurred since the last call. If the count encompasses more than one period of a repeating timer then the timer will be triggered multiple times in one call to tick.
Returns `true` if the tick counter was updated, otherwise `false`. This may be used by the calling routine to accumulate unprocessed tick counts.

---

```cpp
bool start(etl::timer::id::type id, bool immediate = false)
```
**Description**  
Starts the timer with the specified id.
If the timer is already running then the timer Is restarted from the current tick count.
If immediate is `true` then the timer is triggered on the next call to `tick()`. Note: Single shot timers will only trigger once.
If the id does not correspond to a registered timer then returns false.

---

```cpp
bool stop(etl::timer::id::type id)
```
**Description**  
Stops the timer with the specified id.
Does nothing if the timer is already stopped.
if the id does not correspond to a registered timer then returns `false`.

---

```cpp
bool set_period(etl::timer::id::type id_, uint32_t period)
```
**Description**  
Stops the timer with the specified id.
Sets a new timer period.
Returns `true` if successful.

---

```cpp
bool set_mode(etl::timer::id::type id_, bool repeating)
```
**Description**  
Stops the timer with the specified id.
Sets a new timer mode.
Returns `true` if successful.

---

```cpp
void set_locks(try_lock_type try_lock_, lock_type lock_, lock_type unlock_)
```
**Description**  
Sets the try-lock, lock and unlock delegates.

---

```cpp
etl::timer::id::type time_to_next()
```
**Description**  
Returns the time to the next timeout.
From: `20.38.0`

---

### Constants
`MAX_TIMERS`

## message_timer_locked

**Template parameters**  
`MAX_TIMERS` The number of timers to be supported. The maximum number is 254.  
A value of 255 will result in a compile error.

---

```cpp
message_timer_locked()
```
**Description**  
Default construct.  
The lock callback delegates are not set.

---

```cpp
message_timer_locked(try_lock_type try_lock, lock_type lock, unlock_type unlock)
```
**Description**  
Construct from lock callback delegates are not set.

## Example

```cpp
//***************************************************************************
// The set of messages.
//***************************************************************************
enum
{
  MESSAGE1,
  MESSAGE2,
  MESSAGE3,
};

enum
{
  ROUTER1 = 1,
};

struct Message1 : public etl::message<MESSAGE1>
{
};

struct Message2 : public etl::message<MESSAGE2>
{
};

struct Message3 : public etl::message<MESSAGE3>
{
};

Message1 message1;
Message2 message2;
Message3 message3;

//***************************************************************************
// Router that handles messages 1, 2, 3
//***************************************************************************
class Router1 : public etl::message_router<Router1, Message1, Message2, Message3>
{
public:

  Router1()
    : message_router(ROUTER1)
  {
 }

  void on_receive(const Message1& msg)
  {
    printf("Message 1 received\n");
  }

  void on_receive(const Message2& msg)
  {
    printf("Message 2 received\n");
  }

  void on_receive(const Message3& msg)
  {
    printf("Message 3 received\n");
  }

  void on_receive_unknown(const etl::imessage& msg)
  {
  }
};
  
//***************************************************************************
// Bus that handles messages.
//***************************************************************************
class Bus1 : public etl::message_bus<1>
{
};

//***************************************************************************
// Router, bus and timer controller.
//***************************************************************************
Router1 router1;
Bus1    bus1;

etl::message_timer_locked<3> timer_controller;

//***************************************************************************
// The main loop.
//***************************************************************************
int main()
{
  bus1.subscribe(router1);

  etl::timer::id::type id1 = timer_controller.register_timer(message1, 
                                                             router1, 
                                                             1000,
                                                             etl::timer::mode::SINGLE_SHOT);

  etl::timer::id::type id2 = timer_controller.register_timer(message2, 
                                                             bus1, 
                                                             100, 
                                                             etl::timer::mode::REPEATING);

  etl::timer::id::type id3 = timer_controller.register_timer(message3, 
                                                             router1, 
                                                             10, 
                                                             etl::timer::mode::REPEATING);

  timer_controller.start(id1);
  timer_controller.start(id2);
  timer_controller.start(id3);

  timer_controller.enable(true);

  // Start timer interrupts here.

  while (true)
  {
    // Loop forever.
  }

  return 0;
}

//***************************************************************************
// The interrupt timer callback.
//***************************************************************************
void timer_interrupt()
{
  const uint32_t TICK = 1;
  static uint32_t nticks = TICK;

  if (timer_controller.tick(nticks))
  {
    nticks = TICK;
  }
  else
  {
    nticks += TICK;
  }
}
```
