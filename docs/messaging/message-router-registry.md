---
title: "message_router_registry"
weight: 4
---

{{< callout type="info">}}
  Header: `message_router_registry`  
  From: `20.6.0`  
{{< /callout >}}

A class that will act as a registry for all message router types.  
When iterating through the registry, routers with identical IDs are ordered by insertion.  

**Defines the following classes**  
```cpp
etl::imessage_router_registry
etl::message_router_registry
```

## imessage_router_registry

The base class for all router registries.  

### Iterators
```cpp
iterator
const_iterator
```

### Member functions

```cpp
iterator begin()
const_iterator begin() const
const_iterator cbegin() const
```
Get the beginning of the registry.

---

```cpp
iterator end()
const_iterator end() const
const_iterator cend() const
```
Get the end of the registry.

---

```cpp
iterator lower_bound(etl::message_router_id_t id)
const_iterator lower_bound(etl::message_router_id_t id) const
```
Get the lower bound in the registry of the router with the specified ID.

---

```cpp
iterator upper_bound(etl::message_router_id_t id)
const_iterator upper_bound(etl::message_router_id_t id) const
```
Get the upper bound in the registry of the router with the specified ID.

---

```cpp
etl::imessage_router* find(etl::message_router_id_t id)
const etl::imessage_router* find(etl::message_router_id_t id) const
```
Returns a pointer to the first router with the specified ID, or `ETL_NULLPTR` if it cannot be found.

---

```cpp
void add(etl::imessage_router& router)
void add(etl::imessage_router* p_router)
```
Registers a router.  
If the registry is full then an ETL assert is emitted (`etl::message_router_registry_full`).  
Duplicate routers will be ignored.

---

```cpp
template <typename TIterator>
void add(TIterator first, const TIterator& last)
```
Registers a collection of routers.  
If the registry becomes full then an ETL assert is emitted (`etl::message_router_registry_full`).  
Duplicate routers will be ignored.

---

```cpp
void remove(etl::message_router_id_t id)
```
Unregisters a router.

---

```cpp
bool contains(const etl::message_router_id_t id) const
bool contains(const etl::imessage_router* const p_router) const
bool contains(const etl::imessage_router& router) const
```
Returns `true` if the registry contains a router that has the specified ID or object.

---

```cpp
bool empty() const
```
Returns `true` if the registry is empty, otherwise `false`.

---

```cpp
bool full() const
```
Returns `true` if the registry is full, otherwise `false`.

---

```cpp
size_t size() const
```
Returns the size of the registry.

---

```cpp
size_t available() const
```
Returns the available size of the registry.

---

```cpp
size_t max_size() const
```
Returns the maximum size of the registry.

---

## message_router_registry

```cpp
message_router_registry()
```
Default constructor.

---

```cpp
template <typename TIterator>
message_router_registry(TIterator first, const TIterator& last)
```
Constructs from an iterator range.

---

```cpp
message_router_registry(std::initializer_list<etl::imessage_router*> init)
```
Initializer_list constructor.  
Enabled for C++11 or above.

---

```cpp
message_router_registry(const message_router_registry& rhs)
```
Copy constructor.

---

```cpp
message_router_registry& operator =(const message_router_registry& rhs)
```
Assignment operator.
