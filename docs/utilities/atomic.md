---
title: atomic
---

{{< callout type="info">}}
  Header: `atomic.h`  
  Since: `11.0.0`  
  Similar to: [atomic](https://en.cppreference.com/cpp/atomic/atomic)
{{< /callout >}}

This header attempts to replicate some of the types from `std::atomic`.  

If `ETL_USING_CPP11` and `ETL_USING_STL` are defined as `1` then `etl::atomic` will be defined in terms of `std::atomic`.  
Otherwise it will be implemented in terms of the built-in support, if available, from the compiler. For example, early GCC and Arm compilers will use the `__sync` built-ins, later versions use `__atomic` built-ins.  

If a type cannot be handled by the built-ins then the types are wrapped by `etl::mutex`.  

If there is an ETL atomic type available for your platform then `ETL_HAS_ATOMIC` will be set to `1`, otherwise
it will be set to `0`.  

From: `20.40.0`  `etl::atomic` supports the `is_always_lock_free` property.  

For GCC and clang built-ins:  
Integrals and pointers are lock free.  
All other types are not lock free.  

---

```cpp
template <typename T>
struct atomic;
```
```cpp
template <typename T>
struct atomic<T*>;
```

## Memory order

```cpp
memory_order_relaxed  
memory_order_consume  
memory_order_acquire  
memory_order_release  
memory_order_acq_rel  
memory_order_seq_cst  
```

## Constructors

```cpp
ETL_CONSTEXPR atomic()
```
**Description**  
Value initialises `T`.

---

```cpp
ETL_CONSTEXPR atomic(T v)
```
**Description**  
Initialises `T` with `v`.

## Assignment
    
```cpp
T operator=(T v)
T operator=(T v) volatile
```
**Description**  
Stores the value 'v'.

## Increment

```cpp
 T operator++()
 T operator++() volatile
```
**Description**  
Pre-increments the atomic value by one.

---

```cpp
 T operator++(int)
 T operator++(int) volatile
 ```
**Description**  
Post-increments the atomic value by one.

 ## Decrement
 ```cpp
 T operator--()
 T operator--() volatile
 ```
**Description**  
Pre-decrements the atomic value by one.

---

 ```cpp
 T operator--(int)
 T operator--(int) volatile
```
**Description**  
Post-decrements the atomic value by one.

## Add

```cpp
T operator+=(T v)
T operator+=(T v) volatile
```
**Description**  
Adds to the atomic value.

## Subtract
```cpp
T operator-=(T v)
T operator-=(T v) volatile
```
**Description**  
Subtracts from the atomic value.

## And

```cpp
T operator&=(T v)
T operator&=(T v) volatile
```
**Description**  
Performs bitwise AND with the atomic value.

## Or

```cpp
T operator|=(T v)
T operator|=(T v) volatile
```
**Description**  
Performs bitwise OR with the atomic value.

## Exclusive or

```cpp
T operator^=(T v)
T operator^=(T v) volatile
```
**Description**  
Performs bitwise XOR with the atomic value.

## Conversion operator

```cpp
operator T() const
operator T() volatile const
```
**Description**  
Loads a value from an atomic object.

## Is lock free

```cpp
bool is_lock_free() const
bool is_lock_free() const volatile
```
**Description**  
Checks if the atomic object is lock-free.

## Store

```cpp
void store(T v, etl::memory_order order = etl::memory_order_seq_cst)
void store(T v, etl::memory_order order = etl::memory_order_seq_cst) volatile
```
**Description**  
Atomically replaces the value of the atomic object with a non-atomic argument.

## Load

```cpp
T load(etl::memory_order order = etl::memory_order_seq_cst) const
T load(etl::memory_order order = etl::memory_order_seq_cst) const volatile
```
**Description**  
Atomically obtains the value of the atomic object.

## Fetch add

```cpp
T fetch_add(T v, etl::memory_order order = etl::memory_order_seq_cst)
T fetch_add(T v, etl::memory_order order = etl::memory_order_seq_cst) volatile
```

```cpp
T* fetch_add(ptrdiff_t v, etl::memory_order order = etl::memory_order_seq_cst)
T* fetch_add(ptrdiff_t v, etl::memory_order order = etl::memory_order_seq_cst) volatile
```
**Description**  
Atomically adds the argument to the value stored in the atomic object and obtains the value held previously.

## Fetch subtract

```cpp
T fetch_sub(T v, etl::memory_order order = etl::memory_order_seq_cst)
T fetch_sub(T v, etl::memory_order order = etl::memory_order_seq_cst) volatile
```

```cpp
T* fetch_sub(ptrdiff_t v, etl::memory_order order = etl::memory_order_seq_cst)
T* fetch_sub(ptrdiff_t v, etl::memory_order order = etl::memory_order_seq_cst) volatile
```
**Description**  
Atomically subtracts the argument to the value stored in the atomic object and obtains the value held previously.

## Fetch or

```cpp
T fetch_or(T v, etl::memory_order order = etl::memory_order_seq_cst)
T fetch_or(T v, etl::memory_order order = etl::memory_order_seq_cst) volatile
```
**Description**  
Atomically performs bitwise OR between the argument and the value of the atomic object and obtains the value held previously.

## Fetch and

```cpp
T fetch_and(T v, etl::memory_order order = etl::memory_order_seq_cst)
T fetch_and(T v, etl::memory_order order = etl::memory_order_seq_cst) volatile
```
**Description**  
Atomically performs bitwise AND between the argument and the value of the atomic object and obtains the value held previously.

## Fetch exclusive or

```cpp
T fetch_xor(T v, etl::memory_order order = etl::memory_order_seq_cst)
T fetch_xor(T v, etl::memory_order order = etl::memory_order_seq_cst) volatile
```
**Description**  
Atomically performs bitwise XOR between the argument and the value of the atomic object and obtains the value held previously.

## Fetch max

```cpp
T fetch_max(T v, etl::memory_order order = etl::memory_order_seq_cst)
T fetch_max(T v, etl::memory_order order = etl::memory_order_seq_cst) volatile
```
**Description**  
Atomically performs max between the argument and the value of the atomic object and obtains the value held previously.

## Fetch min

```cpp
T fetch_min(T v, etl::memory_order order = etl::memory_order_seq_cst)
T fetch_min(T v, etl::memory_order order = etl::memory_order_seq_cst) volatile
```
**Description**  
Atomically performs min between the argument and the value of the atomic object and obtains the value held previously.

## Exchange

```cpp
T exchange(T v, etl::memory_order order = etl::memory_order_seq_cst)
T exchange(T v, etl::memory_order order = etl::memory_order_seq_cst) volatile
```
**Description**  
Atomically replaces the value of the atomic object and obtains the value held previously.

## Compare exchange weak

```cpp
bool compare_exchange_weak(T& expected, T desired, etl::memory_order order = etl::memory_order_seq_cst)
bool compare_exchange_weak(T& expected, T desired, etl::memory_order order = etl::memory_order_seq_cst) volatile
bool compare_exchange_weak(T& expected, T desired, etl::memory_order success, etl::memory_order failure)
bool compare_exchange_weak(T& expected, T desired, etl::memory_order success, etl::memory_order failure) volatile
```
**Description**  
Atomically compares the value of the atomic object with non-atomic argument and performs atomic exchange if equal or atomic load if not.

## Compare exchange strong

```cpp
bool compare_exchange_strong(T& expected, T desired, etl::memory_order order = etl::memory_order_seq_cst)
bool compare_exchange_strong(T& expected, T desired, etl::memory_order order = etl::memory_order_seq_cst) volatile
bool compare_exchange_strong(T& expected, T desired, etl::memory_order success, etl::memory_order failure)
bool compare_exchange_strong(T& expected, T desired, etl::memory_order success, etl::memory_order failure) volatile
```
**Description**  
Atomically compares the value of the atomic object with non-atomic argument and performs atomic exchange if equal or atomic load if not.
