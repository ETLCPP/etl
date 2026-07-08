---
title: "callback_timer_deferred_locked"
---

{{< callout type="info">}}
  Header: `callback_timer_deferred_locked.h`  
  From: `20.43.0`
{{< /callout >}}

A software timer class that can manage up to 254 timers. Each one may be repeating or single shot.  
When a timer triggers it will call the selected function. The function may be a class member or free function.  
The timers are driven from a call to `tick(uint32_t count)`. This call would normally be made from a high priority interrupt routine. The destination function will receive the callback in the same context as the tick call.  
The call to tick has a low overhead when a timer is not 'due'. Internally the timers are stored in 'first timeout' order so only the head of the list needs to be checked.  
This class is different from the similar `etl::callback_timer_locked` in that it will record any timer events during the call to `tick()` so that they may be handled by `handle_deferred()`.  

Each timer may have a period of up to 2<sup>32</sup>-2 ticks (4,294,967,294).  
At 1ms per tick this would equate to just over 49 days.  

**Defines the following classes**
```cpp
etl::icallback_timer_locked
etl::callback_timer_deferred_locked
```

The access to the timers is controlled by three external functions, supplied to the timer by the member function set_locks.  

Uses definitions from `timer.h`.  

**Important**  
For correct operation of the timer framework, the routine that calls tick must not be pre-emptible by another routine that calls a timer function. Also, calls to the timer framework may only be made from the caller of tick and one other, lower priority, thread of execution.  

---

## icallback_timer_locked
The base class for all timer controllers.  

### Type definitions
```cpp
callback_type    etl::delegate<void(void)>
```
The function type used for callbacks.

```cpp
try_lock_type    etl::delegate<bool(void)>
```
The function type used for 'try lock'.

```cpp
lock_type        etl::delegate<void(void)>
```
The function type used for 'lock'.

```cpp
unlock_type      etl::delegate<void(void)>
```
The function type used for 'unlock'.

### Member functions
```cpp
etl::timer::id::type register_timer(callback_type callback),
                                    uint32_t      period,                                        
                                    bool          repeating)
```
**Description**  
Registers a timer calling a free or static function.

**Parameters**  
`callback`  A delegate to the callback free function that will be called when the timer expires.  
`period`    The timer period in ticks.  
`repeating` `false` if single shot, `true` if repeating.  

**Return**  
The allocated timer id or `etl::timer::mode::NO_TIMER` if one was not available.

---

```cpp
bool unregister_timer(etl::timer::id::type id)
```
**Description**  
Unregisters a timer.  
If the timer is active then it will be stopped.  

**Return**  
`true` if a timer with the id was successfully unregistered.  

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
Clears the callback timer back to the initial state. All timers will be stopped and unregistered.

---

```cpp
bool tick(uint32_t count)
```
**Description**  
This function updates the internal tick counter (if enabled) and must pass the number of ticks that have occurred since the last call. If the count encompasses more than one period of a repeating timer then the timer will be triggered multiple times in one call to tick.  

**Return**  
`true` if the tick counter was updated, otherwise `false`. This may be used by the calling routine to accumulate unprocessed tick counts.  
Any triggered events will be recorded so that they can be deferred.  

---

```cpp
void handle_deferred()
```
**Description**  
Handles the work collected during the `tick()` call.  
You can call this function after `tick()` or you can call this on another task to handle the timer events.  

---

```cpp
bool start(etl::timer::id::type id, bool immediate = false)
```
**Description**  
Starts the timer with the specified id.  
If the timer is already running then the timer Is restarted from the current tick count.  
If immediate is true then the timer is triggered on the next call to tick(). Note: Single shot timers will only trigger once.  
If the id does not correspond to a registered timer then returns `false`.  

---

```cpp
bool stop(etl::timer::id::type id)
```
**Description**  
Stops the timer with the specified id.  
Does nothing if the timer is already stopped.  
If the id does not correspond to a registered timer then returns `false`.  

---

```cpp
bool set_period(etl::timer::id::type id, uint32_t period)
```
**Description**  
Stops the timer with the specified id.  
Sets a new timer period.  

**Return**  
`true` if successful.  

---
 
```cpp
bool set_mode(etl::timer::id::type id, bool repeating)
```
**Description**  
Stops the timer with the specified id.  
Sets a new timer mode.  

**Return**  
`true` if successful.  

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

### Constants
`Max_Timers`  
The maximum number of timers that can be handled.

---

## callback_timer_locked

```cpp
template <uint_least8_t Max_Timers, uint32_t Max_Handlers>
class callback_timer_deferred_locked
```

**Template parameters**  
`Max_Timers` The number of timers to be supported. The maximum number is 254.  
A value of 255 will result in a compile error. 

`Max_Handlers`  
The max number of deferred timer events.

---

```cpp
callback_timer_locked()
```
**Description**  
Default construct.  
The lock callback delegates are not set.

---

```cpp
callback_timer_locked(try_lock_type try_lock, lock_type lock, unlock_type unlock)
```
**Description**  
Construct from lock callback delegates.

## Example
```cpp
//***************************************************************************
// Class callback via etl::function
//***************************************************************************
class Test
{
public:

  Test()
    : ticks(0)
  {
  }

  void callback()
  {
    ++ticks;
  }

  int ticks;
};
  
using callback_type = etl::icallback_timer_locked::callback_type;

callback_type member_callback = callback_type::create<Test, test, &Test::callback>();

//***************************************************************************
// Free function callback via etl::function
//***************************************************************************
int free_ticks1 = 0;

void free_callback1()
{
  ++free_ticks1;
}

callback_type free_function_callback = callback_type::create<free_callback1>();

//***************************************************************************
// Timer controller.
//***************************************************************************
etl::callback_timer_deferred_locked<2, 4> timer_controller;

//***************************************************************************
// The main loop.
//***************************************************************************
int main()
{
  etl::timer::id::type id1 = timer_controller.register_timer(member_callback,
                                                             1000,
                                                             etl::timer::mode::SINGLE_SHOT);

  etl::timer::id::type id2 = timer_controller.register_timer(free_function_callback,
                                                             100,
                                                             etl::timer::mode::REPEATING);

  timer_controller.start(id1);
  timer_controller.start(id2);

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
