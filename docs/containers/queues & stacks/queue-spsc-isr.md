---
title: "queue_spsc_isr"
---

{{< callout >}}
  Header: `queue_spsc_isr.h`  
{{< /callout >}}

{{< callout type="warning">}}
**This class is deprecated.**  
Use `queue_spsc_locked` as a replacement.
{{< /callout >}}

**This class is deprecated. 
`Use queue_spsc_locked` as a replacement.**  

A fixed capacity single-producer, single-consumer queue for interrupt driven systems.  
The queue may be used to transfer data to or from an ISR. Calls to the queue from the ISR must use the `_from_isr` versions.  

A helper class must be supplied that defines static void `lock()` and void `unlock()` functions. They must perform the requisite memory barriers to preserve the order of execution.

```cpp
etl::queue_spsc_isr<typename T,
                    const size_t SIZE,
                    typename TAccess,
                    const size_t MEMORY_MODEL = etl::memory_model::MEMORY_MODEL_LARGE>
```

Inherits from `etl::iqueue_spsc_isr<T, TAccess>`.  
`etl::iqueue_spsc_isr` may be used as a size independent pointer or reference type for any `etl::queue_spsc_isr` instance of the same implementation.  

The memory model determines the type used internally for indexes and size, to allow for the most efficient implementation for the application.

## Maximum queue sizes
```cpp
MEMORY_MODEL_SMALL   255
MEMORY_MODEL_MEDIUM  65535
MEMORY_MODEL_LARGE   2147483647
MEMORY_MODEL_HUGE    9223372036854775807
```
See `memory_model.h`  

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
queue_spsc_isr()
```

## Capacity

```cpp
bool empty() const
bool empty_from_isr() const
```
**Description**  
Returns true if the size of the queue is zero, otherwise false.

---

```cpp
bool full() const
bool full_from_isr() const
```
**Description**  
Returns true if the size of the queue is SIZE, otherwise false.

---

```cpp
size_type size() const
size_type size_from_isr() const
```
**Description**  
Returns the size of the queue.

---

```cpp
size_type available() const
size_type available_from_isr() const
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
bool push_from_isr(const T& value)
bool push_from_isr(T&& value)
```
**Description**  
Pushes a value to the back of the queue.  
Returns true if successful, otherwise false.

---

```cpp
bool pop()
bool pop_from_isr()
```
**Description**  
Pop a value from the front of the list.  
Returns true if successful, otherwise false.

---

```cpp
bool pop(T& value)
bool pop_from_isr(T& value)
bool pop(T&& value)
bool pop_from_isr(T&& value)
```
**Description**  
Pop a value from the front of the list and place it in value.  
Returns true if successful, otherwise false.

---

```cpp
void clear()
void clear_from_isr()
```
**Description**  
Clears the queue to a size of zero.

### C++03
```cpp
bool emplace(const T1& value1)
bool emplace(const T1& value1, const T2& value2)
bool emplace(const T1& value1, const T2& value2, const T3& value3)
bool emplace(const T1& value1, const T2& value2, const T3& value3, const T4& value4)
```
**Description**  
Constructs an item in the the queue 'in place'.  

### C++11
```cpp
bool emplace(Args&&… args)
```
**Description**  
Constructs an item in the the queue 'in place'.  


## Notes

Remember that interrupts may occur between calls to the access protected functions. For example, a call to `empty()` may return `true`, but a subsequent call to pop() may succeed if an interrupt occurred between the two and pushed a new value.

## Example

```cpp
class InterruptControl
{
public:

  static void lock()
  {
    // Store current interrupt control register and disable the relevant interrupt.
  }

  static void unlock()
  {
    // Restore the interrupt control register.
  }
};

etl::queue_spsc_isr<char, 10, InterruptControl> queue;

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
  queue.push_from_isr(c);
}
```
