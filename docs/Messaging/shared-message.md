---
title: "shared_message"
---

{{< callout type="info">}}
  Header: `shared_message.h`  
{{< /callout >}}

Shared Messages  

The type used to encapsulate reference counted messages.  
Shared messages are usually passed by value.  

See the Shared Message Tutorial  

---

```cpp
template <typename TPool, typename TMessage>
shared_message(TPool& owner, const TMessage& message)
```
Static asserts if `TPool` is not derived from `etl::ireference_counted_message_pool`
Static asserts if `TMessage` not derived from `etl::imessage`.  

Requires that `TPool` implements the following member function to allocate from the pool.  

```cpp
template <typename TMessage>
etl::ireference_counted_message* allocate(const TMessage& message)
```
---

```cpp
explicit shared_message(etl::ireference_coutnted_message& message)
```
Construct from a reference counted message.

---

```cpp
shared_message(const etl::shared_message& other)
```
Copy constructor.

---

```cpp
shared_message& operator =(const etl::shared_message& other)
```
Assignment operator.

---

```cpp
~shared_message()
```
Destructor.

---

```cpp
ETL_NODISCARD etl::imessage& get_message()
```
Gets a reference to the contained message.

---

```cpp
ETL_NODISCARD const etl::imessage& get_message() const
```
Gets a const reference to the contained message.

---

```cpp
ETL_NODISCARD uint32_t get_reference_count() const
```
Gets the current reference count for this shared message.
