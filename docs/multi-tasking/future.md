---
title: "future / promise"
---

{{< callout type="info">}}
  Header: `future.h`  
  From: `20.50.0`  
{{< /callout >}}

A lightweight, heap-free `future` / `promise` pair for passing a single result from a producer (e.g. an ISR, a
thread or an RPC response handler) to a consumer.

Unlike `std::future`/`std::promise`, the shared state is **not** allocated. It is an `etl::shared_state<T>` object
whose storage and lifetime are managed by the user, for example as an element of a fixed-size pending-request table.
`etl::future<T>` and `etl::promise<T>` are non-owning handles to it.

```cpp
template <typename T>
class shared_state;

template <typename T>
class future;

template <typename T>
class promise;
```

All three are specialised for `void`, where there is no value, only a *ready* notification, and for references
(`T&`), where a pointer to the referenced object is stored. `T` must not be an array type.

**Differences from the standard library**  
- `etl::future<T>` has the semantics of `std::shared_future<T>`: it is copyable, and `get()` does not consume the
  value, so it may be called more than once, by every copy. `get()` returns `const T&` (`R&` for `future<R&>`).
- `etl::promise<T>` is, like `std::promise<T>`, move-only (C++11) or non-copyable (C++03).
- `get()` does not block. Poll `is_ready()` or use an external notification.
- There is no `set_exception()` and no *broken promise* notification. A consumer cannot distinguish a slow producer
  from one that has gone away.

**Availability**  
The header requires `etl::atomic_bool`. The classes are only defined when `ETL_HAS_ATOMIC` is `1`
(see `etl::atomic`).

**Thread safety**  
`set_value()` and `is_ready()`/`get()` may be called from different threads or contexts, provided that at most
**one** context calls `set_value()` and at most **one** context calls `get()`. The ready flag uses
release/acquire ordering, so a value written by `set_value()` is visible after `is_ready()` returns `true`.  
`reset()` must not be called concurrently with any other member.

## shared_state

Holds the result and an atomic *ready* flag. Non-copyable.

```cpp
shared_state()
```
**Description**  
Constructs a state that is not ready.

---

```cpp
~shared_state()
```
**Description**  
Destroys the stored value, if one has been set.

---

```cpp
void set_value(const T& value)
void set_value(T&& value)        // C++11
void set_value(T& value)         // shared_state<T&> only
void set_value()                 // shared_state<void> only
```
**Description**  
Stores the value (or, for `shared_state<T&>`, the address of the referenced object) and marks the state as ready.  
Asserts `etl::promise_already_satisfied` if the state is already ready. Call `reset()` first to reuse the state.

---

```cpp
bool is_ready() const noexcept
```
**Description**  
Returns `true` if a value has been set.

---

```cpp
T&       get()
const T& get() const
T&       get() const             // shared_state<T&> only
```
**Description**  
Returns a reference to the stored value.  
Not available for `shared_state<void>`.  
Asserts `etl::future_not_ready` if the state is not ready.

---

```cpp
void reset()
```
**Description**  
Destroys the stored value, if any, and marks the state as not ready, so that it can be reused.

## future

A non-owning handle to a `shared_state`. Copyable, with the semantics of `std::shared_future`.

```cpp
future() noexcept
```
**Description**  
Constructs a future that is not associated with a shared state. `valid()` returns `false`.

---

```cpp
explicit future(etl::shared_state<T>& state) noexcept
```
**Description**  
Constructs a future associated with `state`.

---

```cpp
bool valid() const noexcept
```
**Description**  
Returns `true` if the future is associated with a shared state.

---

```cpp
bool is_ready() const noexcept
```
**Description**  
Returns `true` if the future is valid and the result is available.

---

```cpp
const T& get() const
R&       get() const             // future<R&> only
void     get() const             // future<void> only
```
**Description**  
Returns the result.  
Asserts `etl::future_no_state` if the future is not valid (default constructed, no shared state).  
Asserts `etl::future_not_ready` if the future is valid but the result is not yet available.  
Does not block. Poll `is_ready()` or use an external notification first.  
The value is not moved out and the future remains valid, so `get()` may be called more than once.

## promise

A non-owning handle to a `shared_state` that fulfils it. Move-only (C++11) or non-copyable (C++03), so that there is
only one producer.

```cpp
promise() noexcept
```
**Description**  
Constructs a promise that is not associated with a shared state. `valid()` returns `false`.

---

```cpp
explicit promise(etl::shared_state<T>& state) noexcept
```
**Description**  
Constructs a promise associated with `state`.

---

```cpp
promise(promise&& other) noexcept             // C++11
promise& operator=(promise&& other) noexcept  // C++11
```
**Description**  
Transfers the association with the shared state. `other` is no longer valid.

---

```cpp
bool valid() const noexcept
```
**Description**  
Returns `true` if the promise is associated with a shared state.

---

```cpp
etl::future<T> get_future()
```
**Description**  
Returns a future associated with the same shared state.  
May be called more than once. All futures refer to the same state.  
Asserts `etl::future_no_state` if the promise is not valid.

---

```cpp
void set_value(const T& value)
void set_value(T&& value)        // C++11
void set_value(R& value)         // promise<R&> only
void set_value()                 // promise<void> only
```
**Description**  
Fulfils the promise.  
Asserts `etl::future_no_state` if the promise is not valid.  
Asserts `etl::promise_already_satisfied` if the shared state is already ready.

## Errors

| Exception                        | Reason                                                   | Error text                  |
| -------------------------------- | -------------------------------------------------------- | --------------------------- |
| `etl::future_exception`          | Base of all future/promise exceptions.                   |                             |
| `etl::future_not_ready`          | `get()` on a future or state that is not ready.          | `future:not ready`          |
| `etl::promise_already_satisfied` | `set_value()` on a state that is already ready.          | `promise:already satisfied` |
| `etl::future_no_state`           | Use of a `future` or `promise` that has no shared state. | `future:no state`           |

`etl::future_exception` is derived from `etl::exception`.

## Example

```cpp
#include "etl/future.h"

etl::shared_state<int> state;             // Owned by the application.

// Consumer
etl::promise<int> promise(state);
etl::future<int>  future = promise.get_future();

// Producer (e.g. in an ISR or another thread)
promise.set_value(42);

// Consumer
if (future.is_ready())
{
  int result = future.get();              // 42
}

// Reuse the state for the next request.
state.reset();
```
