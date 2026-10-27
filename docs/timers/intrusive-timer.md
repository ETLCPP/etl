---
title: "intrusive_timer"
---

{{< callout type="info">}}
  Header: `intrusive_timer.h`  
  Since: `20.46.0`  
  From: `C++03`
{{< /callout >}}

## Description

The `etl::intrusive_timer` is a replacement for all legacy timers within the ETL. As an intrusive container, it maintains zero internal configuration state; instead, timer nodes are allocated externally and registered with `etl::intrusive_timer`, which manages them via internal linked lists.
To ensure safe and predictable operation, the architecture enforces the following invariants:
- **Lifetime**  
  Timer nodes must match or outlive the lifetime of the `etl::intrusive_timer` instance.
- **Immutability**  
  Once defined, timer nodes are structurally non-mutating and can only be modified through the `etl::intrusive_timer` API.
- **Exclusivity**  
  A timer node can only be registered to one `etl::intrusive_timer` instance at a time.  
### Key Features
- **Flexible Notifications**  
  Supports both callback functions and message-passing interfaces upon expiry. Also supports custom user callback types.
- **Dynamic Scheduling**  
  Features a configurable notification hook that fires whenever the next due timer changes, yielding the precise 'time-to-next' interval to user code.

## timer_lock_policy
```cpp
class timer_lock_policy
```
**Description**  
Interface for a timer lock policy.  
Derive from this to implement your own lock policy.

Declares the following pure virtual functions.  

---

`virtual void lock() ETL_NOEXCEPT`  
Locks access.  

---

`virtual void unlock() ETL_NOEXCEPT`  
Unlocks access.  

---

`virtual bool is_locked() ETL_NOEXCEPT`  
Returns `true` if access is locked.  

## timer_no_lock_policy
```cpp
class timer_no_lock_policy : public timer_lock_policy
```
**Description**  
This lock policy does no locking.  
Useful for single threaded code or testing purposes.  

## timer_atomic_lock_policy
```cpp
class timer_atomic_lock_policy : public timer_lock_policy
```
**Description**  
This lock policy uses an atomic flag.  
Only defined if `ETL_HAS_ATOMIC` is `1`.

## intrusive_timer
```cpp
class intrusive_timer
```

---

```cpp
using time_to_next_notification_type = etl::delegate<void(uint32_t delta)>;
```
**Description**  
The notification type for 'time to next'.

---

```cpp
explicit intrusive_timer(timer_lock_policy& lock_policy) ETL_NOEXCEPT
```
**Description**  
Constructor.

**Parameters**  
`lock_policy` The lock policy which determines access to parts of the timer.

---

```cpp
bool register_timer(etl::timer_node& timer) ETL_NOEXCEPT
```
**Description**  
Register a timer.  
It will not be started until the `start()` method is called.

**Parameters**  
`etl::timer_node& timer` : The timer node to register.  
The timer node must not be already registered with anything else.  
The timer nodes's ID will be set here and this timer will be set as its owner.

**Return**  
`true` if the timer node was successfully registered.

---

```cpp
bool unregister_timer(etl::timer_node& timer) ETL_NOEXCEPT
```
**Description**  
Unregisters a timer node.  
Stops the timer, if active.  
Callbacks for any expired timers may still occur.

**Parameters**  
`etl::timer_node& timer` : The timer node to unregister.  
The timer node must be registered with this timer. If it isn't then the function will return false.  
The timer nodes's ID will be set here and this timer will be set as its owner.

---

```cpp
void enable(bool state) ETL_NOEXCEPT
```
**Description**  
Enable/disable the timer.

**Parameters**  
`state` : `true` to enable, `false` to disable.

---

```cpp
bool is_running() const ETL_NOEXCEPT
```
**Description**  
Get the enable/disable state.

---

```cpp
void clear() ETL_NOEXCEPT
```
**Description**  
Clears the timer of all registered nodes.  
Callbacks for any expired timers may still occur.

---

```cpp
bool tick(uint32_t count)
```
**Description**  
Called by the timer service to indicate the amount of time that has elapsed since the last successful call to 'tick'.  
Expired timer callbacks occur outside of the locked regions.


**Parameters**  
`uint32_t count` : The amount of time that has elapsed since the last successful call to 'tick'  

**Return**  
`true` if the tick was processed, `false` if not.  
If the tick was not processed then the next tick must accommodate for the extra amount of time that has passed.

---

```cpp
bool start(etl::timer_node& timer, 
           bool             immediate = false)
```
**Description**  
Starts a timer.  
It will be started only if:-  
- Must not be active.  
- Must have a valid interval.  
- Must have a valid mode.  

If the started timer will now be the first to expire then a 'time to next' notification will be sent.

**Parameters**  
`etl::timer_node& timer` : The timer node to start.  
`bool immediate` : If `true` then the timer expires immediately.

**Return**  
`true` if the timer was started, otherwise `false`.

---

```cpp
bool stop(etl::timer_node& timer) ETL_NOEXCEPT
```
**Description** 
Stops a timer.  
A 'time to next' notification will be sent for the next timer to expire.  
Expired timer callbacks occur outside of the locked regions.

**Parameters**  
`etl::timer_node& timer` : The timer node to stop.  

**Return**  
`true` if the timer was stopped,

---

```cpp
bool set_interval(etl::timer_node& timer, 
                  uint32_t         interval) ETL_NOEXCEPT
```
**Description**  
Sets a timer's interval.  
The timer node must be registered with the timer.  
The timer will be stopped if active.  
If the timer node was the next to expire then a 'time to next' notification will be sent.

**Parameters**  
`etl::timer_node& timer` : The timer node to start.  
`uint32_t interval` : The new interval.

**Return**  
`true` if the timer node was updated, otherwise `false`.

---

```cpp
bool set_mode(etl::timer_node&       timer, 
              etl::timer::mode::type mode) ETL_NOEXCEPT
```
**Description**  
Sets a timer's mode.  
The timer node must be registered with the timer.  
The timer will be stopped if active.  
If the timer node was the next to expire then a 'time to next' notification will be sent.

**Parameters**  
`etl::timer_node& timer` : The timer node to start.  
`etl::timer::mode::type mode` : The new mode.

**Return**  
`true` if the timer node was updated, otherwise `false`.

---

```cpp
bool set_config(etl::timer_node&         timer,
                const etl::timer_config& config) ETL_NOEXCEPT
```
**Description**  
Sets a timer's configuration.  
The timer node must be registered with the timer.  
The timer will be stopped if active.  
If the timer node was the next to expire then a 'time to next' notification will be sent.

**Parameters**  
`etl::timer_node& timer` : The timer node to start.  
`const etl::timer_config& config` : The new configuration.

**Return**  
`true` if the timer node was updated, otherwise `false`.
 
 ---
 
 ```cpp
bool has_active_timer() const ETL_NOEXCEPT
 ```
**Description**  
Check if there is an active timer.  

**Return**  
`true` if there is an active timer, otherwise `false`.

---

```cpp
uint32_t time_to_next() const ETL_NOEXCEPT
```
**Description**  
Get the time to the next timer event.

**Return**  
Returns `etl::timer::interval::No_Active_Interval` if there is no active timer.

---

```cpp
bool is_registered(const etl::timer_node& timer) const ETL_NOEXCEPT
```
**Description**  
Check if a timer is registered with this timer.

**Return**  
`true` if registered, otherwise `false`.

---

```cpp
size_t number_of_timers() const ETL_NOEXCEPT
```
**Description**  
Returns the number of registered timers.

---

```cpp
void set_next_timer_notification(time_to_next_notification_type callback) ETL_NOEXCEPT
```
**Description**  
Sets the update notification callback.

**Parametetrs**  
`time_to_next_notification_type callback` : The callback for 'time to next' notification.

---

```cpp
void clear_next_timer_notification() ETL_NOEXCEPT
```
**Description**  
Clears the update notification callback.

---

```cpp
etl::timer_node* find_timer(etl::timer::id::type id) ETL_NOEXCEPT
```
**Description**  
Finds a timer node by its id.  

**Parameters**  
`etl::timer::id::type id` : The ID of the timer node.  

**Return**  
A pointer to the node or `etl::timer_node::end()`
