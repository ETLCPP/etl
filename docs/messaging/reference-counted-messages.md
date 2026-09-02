---
title: "reference_counted_message"
---

{{< callout type="info">}}
  Header: `reference_counted_message.h`  
{{< /callout >}}

Reference counted message types that are used by `etl::shared_message`.  

**Defines the following classes**  

```cpp
etl::ireference_counted_message
```
The interface of all reference counted message types.

---

```cpp
etl::reference_counted_message<typename TMessage, typename TCounter>
```
Derived from `etl::ireference_counted_message`.

```cpp
etl::persistent_message<typename TMessage>
```
Derived from `etl::ireference_counted_message`.

## ireference_counted_message

```cpp
etl::ireference_counted_message
```
The interface of all reference counted message types.

---

```cpp
virtual ~ireference_counted_message()
```

---

```cpp
ETL_NODISCARD virtual etl::imessage& get_message() = 0;
```
Get a reference to the message.

---

```cpp
ETL_NODISCARD virtual const etl::imessage& get_message() const = 0;
```
Get a const reference to the message.

---

```cpp
ETL_NODISCARD virtual etl::ireference_counter& get_reference_counter() = 0;
```
Get a reference to the reference counter.

---

```cpp
ETL_NODISCARD virtual const etl::ireference_counter& get_reference_counter() const = 0; 
```
Get a const reference to the reference counter.

---

```cpp
virtual void release() = 0;
```
Release back to the owner.

## reference_counted_message

```cpp
etl::reference_counted_message<typename TMessage, typename TCounter>     
```
The implementation of reference counted messages owned by a pool.  
Will static assert if TMessage is no derived from etl::imessage.

---

```cpp
reference_counted_message(const TMessage& message, etl::ireference_counted_message_pool& owner)
```
Constructs from a message and the pool from which the reference counted message is allocated.  
The message is copied.

---

```cpp
reference_counted_message(etl::ireference_counted_message_pool& owner)
```
Constructs from a message and the pool from which the reference counted message is allocated.  
The message is default constructed.

---

```cpp
ETL_NODISCARD TMessage& get_message() ETL_OVERRIDE
```
Get a reference to the message.

---

```cpp
ETL_NODISCARD const TMessage& get_message() const ETL_OVERRIDE
```
Get a const reference to the message.

---

```cpp
ETL_NODISCARD etl::ireference_counter& get_reference_counter() ETL_OVERRIDE
```
Get a reference to the reference counter.

---

```cpp
ETL_NODISCARD const etl::ireference_counter& get_reference_counter() const ETL_OVERRIDE
```
Get a const reference to the reference counter.

---

```cpp
void release() ETL_OVERRIDE
```
Release back to the owner pool.

## persistent_message

```cpp
etl::persistent_message<typename TMessage>     
```

The implementation of reference counted messages not owned by a pool.  
It's counter type is `void`.  
Will static assert if `TMessage` is not derived from `etl::imessage`.

---

```cpp
persistent_message(const TMessage& message)
```
Constructs from a message.  
The message is copied.

---

```cpp
ETL_NODISCARD TMessage& get_message() ETL_OVERRIDE
```
Get a reference to the message.

---

```cpp
ETL_NODISCARD const TMessage& get_message() const ETL_OVERRIDE
```
Get a const reference to the message.

---

```cpp
ETL_NODISCARD etl::ireference_counter& get_reference_counter() ETL_OVERRIDE
```
Get a reference to the reference counter.

---

```cpp
ETL_NODISCARD const etl::ireference_counter& get_reference_counter() const ETL_OVERRIDE
```
Get a const reference to the reference counter.

---

```cpp
void release() ETL_OVERRIDE
```
Does nothing for a persistent message.

## For C++11, with atomic support.

```cpp
template <typename TMessage>
using atomic_counted_message = etl::reference_counted_message<TMessage, etl::atomic_int32_t>;
```
Defines an alias to a reference counted message that uses an atomic.

