---
title: "queue_lockable"
---

This class is designed to be an alternative to `etl::queue_spsc_locked`.  
It is an abstract class and requires that the user derives their own type from it.  

The derived class must override the two pure virtual functions `void lock() const` and `void unlock() const` to implement the required locking functionality, whether this be a mutex, ISR control or something else.  
They must perform the requisite memory barriers to preserve the order of execution.  

Many functions have two versions. One locks and unlocks access and is used from the foreground task. The other, with a `_from_unlocked` suffix, are called from the background task.

```cpp
etl::queue_lockable<typename T,
                    const size_t VSize,
                    const size_t VMemory_Model = etl::memory_model::MEMORY_MODEL_LARGE>
```

Inherits from `etl::iqueue_lockable<T, const size_t VMemory_Model>`.  
`etl::iqueue_lockable` may be used as a size independent pointer or reference type for any `etl::queue_lockable` instance of the same implementation.  

The memory model determines the type used internally for indexes and size, to allow for the most efficient implementation for the application.

## Maximum queue sizes

```cpp
MEMORY_MODEL_SMALL   255
MEMORY_MODEL_MEDIUM  65535
MEMORY_MODEL_LARGE   2147483647
MEMORY_MODEL_HUGE    9223372036854775807
```
See memory_model.h

## Member types

```cpp
value_type      T
size_type       <based on memory model>
pointer         value_type*
const_pointer   const value_type*
reference       value_type&
const_reference const value_type&
```

## Constructor

```cpp
queue_lockable()
```

## Capacity

```cpp
bool empty() const
bool empty_from_unlocked() const
```
**Description**  
Returns `true` if the size of the queue is zero, otherwise `false`.

---

```cpp
bool full() const
bool full_from_unlocked() const
```
**Description**  
Returns `true` if the size of the queue is SIZE, otherwise `false`.

---

```cpp
size_type size() const
size_type size_from_unlocked() const
```
**Description**  
Returns the size of the queue.

---

```cpp
size_type available() const
size_type available_from_unlocked() const
```
**Description**  
Returns the remaining available capacity in the queue.

---

```cpp
size_type max_size() const
```
**Description**  
Returns the maximum possible size of the queue.

---

```cpp
size_type capacity() const
```
**Description**  
Returns the maximum possible size of the queue.

## Modifiers

```cpp
bool push(const T& value)
bool push(T&& value)
bool push_from_unlocked(const T& value)
bool push_from_unlocked(T&& value)
```
**Description**  
Pushes a value to the back of the queue. 
Returns true if successful, otherwise false.

---

```cpp
bool pop()
bool pop_from_unlocked()
```
**Description**  
Pop a value from the front of the list.
Returns true if successful, otherwise false.

---

```cpp
bool pop(T& value)
bool pop_from_unlocked(T& value)
```
**Description**  
Pop a value from the front of the list and place it in value.
Returns true if successful, otherwise false.

---

```cpp
void clear()
void clear_from_unlocked()
```
**Description**  
Clears the queue to a size of zero.

---

### C++03
```cpp
bool emplace(const T1& value1);
bool emplace(const T1& value1, const T2& value2);
bool emplace(const T1& value1, const T2& value2, const T3& value3);
bool emplace(const T1& value1, const T2& value2, const T3& value3, const T4& value4);
```
**Description**  
Constructs an item in the the queue 'in place'.  

### C++11
```cpp
bool emplace(Args&&… args);
```
**Description**  
Constructs an item in the the queue 'in place'.  

## Notes

Remember that interrupts may occur between calls to the access protected functions. For example, a call to `empty()` may return `true`, but a subsequent call to `pop()` may succeed if an interrupt occurred between the two and pushed a new value.

## Example

```cpp
class InterruptControl
{
public:

  void lock()
  {
    // Store current interrupt control register and disable the relevant interrupt.
  }

  void unlock()
  {
    // Restore the interrupt control register.
  }
};

InterruptControl interruptControl;

// Create function wrappers with direct calls to the instance's member functions.
etl::function_imv<InterruptControl, interruptControl, &InterruptControl::lock>   lock;
etl::function_imv<InterruptControl, interruptControl, &InterruptControl::unlock> unlock;

etl::queue_spsc_locked<char, 10> queue(lock, unlock);

int main()
{
  while (true)
  {
    char c;

    if (queue.pop(c))
    {
      Print(c);
    }
  }
}

void ISR(char c)
{
  queue.push_from_unlocked(c);
}
```
