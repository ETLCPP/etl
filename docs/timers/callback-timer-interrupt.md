---
title: "callback_timer_interrupt"
---

{{< callout type="info">}}
  Header: `callback_timer_interrupt.h`  
  From: `20.25.0`  
{{< /callout >}}

A software timer class that can manage up to 254 timers. Each one may be repeating or single shot.  
When a timer triggers it will call the selected function. The function may be a class member or free function.  
The timers are driven from a call to `tick(uint32_t count)`. This call would normally be made from a high priority interrupt routine. The destination function will receive the callback in the same context as the tick call.  
The call to tick has a low overhead when a timer is not 'due'. Internally the timers are stored in 'first timeout' order so only the head of the list needs to be checked.  

Each timer may have a period of up to 2<sup>32</sup>-2 ticks (4,294,967,294).  
At 1ms per tick this would equate to just over 49 days.  

The framework relies on a supplied interrupt enable/disable guard type. With this mechanism, calls to tick are disabled if certain member functions are called. If the program flow is within a disable/enable section when the timer interrupt is activated then the tick update will be deferred until the next tick period. The timer interrupt may interrogate the return value of tick() to check whether the update was deferred. The guard allows timer functions to be to be called from the callback called by tick().  

**Defines the following classes**  
```cpp
etl::icallback_timer_interrupt<uint_least8_t MAX_TIMERS, typename TInterruptGuard>
etl::callback_timer_interrupt<typename TInterruptGuard>
```

Uses definitions from `timer.h`.

**Important**  
For correct operation of the timer framework, the routine that calls tick must not be preemptible by another routine that calls a timer function. Also, calls to the timer framework may only be made from the caller of tick and one other, lower priority, thread of execution.  

## icallback_timer_interrupt
The base class for all timer controllers.  

**Template parameters**  
`MAX_TIMERS` The number of timers to be supported. The maximum number is 254.  
A value of 255 will result in a compile error.

`TInterruptGuard` The type that enables/disables interrupts.  

**Type definitions**  
```cpp
callback_type etl::delegate<void(void)>
```
The function type used for callbacks.

### Member functions
```cpp
etl::timer::id::type register_timer(callback_type callback,
                                    uint32_t      period,                                        
                                    bool          repeating)
```
**Description**  
Registers a timer calling a free or static function.  

**Parameters**  
callback   A delegate to the callback function that will be called when the timer expires.  
period     The timer period in ticks.  
repeating  false if single shot, true if repeating.  

**Return**  
The allocated timer id or etl::timer::mode::NO_TIMER if one was not available.

---

```cpp
bool unregister_timer(etl::timer::id::type id)
```
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
Clears the callback timer back to the initial state. All timers will be stopped and unregistered.

---

```cpp
bool tick(uint32_t count)
```
**Description**  
This function updates the internal tick counter (if enabled) and must pass the number of ticks that have occurred since the last call. If the count encompasses more than one period of a repeating timer then the timer will be triggered multiple times in one call to tick.  
Returns `true` if the tick counter was updated, otherwise `false`. This may be used by the calling routine to accumulate unprocessed tick counts.

---

```cpp
bool start(etl::timer::id::type id, bool immediate = false)
```
**Description**  
Starts the timer with the specified id.  
If the timer is already running then the timer Is restarted from the current tick count.  
If immediate is `true` then the timer is triggered on the next call to `tick()`. Note: Single shot timers will only trigger once.  
If the id does not correspond to a registered timer then returns `false`.

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
bool set_period(etl::timer::id::type id, uint32_t period)
```
**Description**  
Stops the timer with the specified id.  
Sets a new timer period.  
Returns `true` if successful.

---

```cpp
bool set_mode(etl::timer::id::type id, bool repeating)
```
**Description**  
Stops the timer with the specified id.  
Sets a new timer mode.  
Returns `true` if successful.

---

```cpp
etl::timer::id::type time_to_next()
```
**Description**  
Returns the time to the next timeout.  
From: `20.38.0`

### Constants
`MAX_TIMERS`
The maximum number of timer that can be handled.

## callback_timer_atomic

**Template parameters**  
`MAX_TIMERS`
The number of timers to be supported. The maximum number is 254.  
A value of 255 will result in a compile error.

`TInterruptGuard`
The type that enables/disables interrupts.

## message_timer_interrupt()
**Description**  
Default constructor.

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
  
Test test;

//***************************************************************************
// The interrupt guard type.
// Saves and disables on contruction.
// Restores on destruction.
//***************************************************************************
struct InterruptGuard
{
  InterruptGuard()
  {
    state = __save_interrupts();
    __disable_interrupts();
  }

  ~InterruptGuard()
  {
    __restore_interrupts(state);
  }

  int state;
};

using callback_type = etl::icallback_timer_interrupt<InterruptGuard>::callback_type;

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
// Free function callback via function pointer
//***************************************************************************
int free_ticks2 = 0;

void free_callback2()
{
  ++free_ticks2;
}

//***************************************************************************
// Timer controller.
//***************************************************************************
etl::callback_timer_interrupt<2, InterruptGuard> timer_controller;

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
