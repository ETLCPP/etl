---
title: "lock_policy"
weight: 103
---

{{< callout type="info">}}
  Header: `timer_lock_policy.h`  
  Namespace: `etl::timer`  
  Since: `20.50.0`  
  From: `C++03`
{{< /callout >}}

## Description

Policies that control access to intrusive timer elements.

## lock_policy
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

## no_lock_policy
```cpp
class timer_no_lock_policy : public timer_lock_policy
```
**Description**  
A predefined lock policy does no locking.  
Useful for single threaded code or testing purposes.  

## atomic_lock_policy
```cpp
class timer_atomic_lock_policy : public timer_lock_policy
```
**Description**  
A predefined lock policy that uses an atomic flag.  
Only defined if `ETL_HAS_ATOMIC` is `1`.
