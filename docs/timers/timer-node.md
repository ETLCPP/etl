---
title: "timer_node"
---

{{< callout type="info">}}
  Header: `timer_node.h`  
  Since: `TBA`  
  From: `C++03`
{{< /callout >}}

## Class diagram
```mermaid
---
config:
  theme: neutral
  look: classic
  layout: dagre
---
classDiagram
  timer_node <|-- timer_callback_node
  timer_node <|-- timer_message_node

  timer_node "1" *-- "*" timer_config : contains

  class timer_config {
    +timer_config()
    +get_interval() const uint32_t 
    +get_mode() const uint32_t 
    +get_id() const etl::timer::id::type
    +clear() void
  }

  class timer_node {
    +timer_node(const etl::timer_config&)
    +get_config() timer_config&
    +get_delta() uint32_t
    +get_interval() uint32_t
    +get_mode() uint32_t
    +get_id() etl::timer::id::type
    +static end() etl::timer_node*
  }

  class timer_node {
    +virtual invoke() void
  }

  class timer_callback_node {
    <<template TCallback>>
    +timer_callback_node(const etl::timer_config&, TCallback, )
    +invoke() void override
  }

  class timer_message_node {
    <<template TMessage>>
    +timer_message_node(const etl::timer_config&, etl::imessage_router&, TMessage)
    +invoke() void override
  }
```

## timer_node
```cpp
class timer_node
```
**Description**  
Base class for all timer nodes.  
Used as the timer element in `etl::intrusive_timer`.

---

```cpp
timer_node(const timer_config& config_) ETL_NOEXCEPT
```
**Description**  
Construct from an `etl::timer_config`.    

---

```cpp
timer_node(const timer_node& node_) ETL_NOEXCEPT
```
**Description**  
Copy constructor.

---

```cpp
const timer_config& get_config() const ETL_NOEXCEPT
```
**Description**  
Returns the configuration of the timer.

---

```cpp
ETL_CONSTEXPR uint32_t get_delta() const ETL_NOEXCEPT
```
**Description**  
Returns the delta of the timer.

---

```cpp
ETL_CONSTEXPR uint32_t get_interval() const ETL_NOEXCEPT
```
**Description**  
Returns the interval of the timer.

---

```cpp
ETL_CONSTEXPR etl::timer::mode::type get_mode() const ETL_NOEXCEPT
```
**Description**  
Returns the mode of the timer.  
`etl::timer::mode::No_Mode`  
`etl::timer::mode::Single_Shot`  
`etl::timer::mode::Repeating`  

---

```cpp
ETL_CONSTEXPR etl::timer::id::type get_id() const ETL_NOEXCEPT
```
**Description**  
Returns the id of the timer.

---

```cpp
static etl::timer_node* end() ETL_NOEXCEPT
```
**Description**  
Returns a pointer to the static sentinel timer node used to mark the end of the intrusive timer lists.

## timer_callback_node
```cpp
template <typename TCallback>
class timer_callback_node : public etl::timer_node
```
**Description**  
A timer node for a function callback using `etl::delegate`, `etl::inplace_function`, or similar.  
Derived from `etl::timer_node`.  

## timer_message_node
```cpp
template <typename TMessage>
class timer_message_node : public etl::timer_node
```
**Description**  
A timer node for a message callback using an `etl::message` derived object.  
Derived from `etl::timer_node`.  
Static asserts if `TMessage` is not derived from `etl::imessage`.

## Example

etl::timer_config