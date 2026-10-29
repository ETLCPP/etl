---
title: "config"
weight: 101
---

{{< callout type="info">}}
  Header: `timer_config.h`  
  Namespace: `etl::timer`  
  Since: `20.50.0`  
  From: `C++03`
{{< /callout >}}

## config
```cpp
class config
```
**Description**  
Contains the configuration for a timer.  
Used to configure a timer for an `etl::timer::node` derived timer element.

---

```cpp
ETL_CONSTEXPR config(etl::timer::interval::type interval, etl::timer::mode::type mode) ETL_NOEXCEPT
```
**Description**  
Constructs a timer configuration.  

**Parameters**  
`etl::timer::interval::type interval` : The initial interval for the timer.  
`etl::timer::mode::type mode` : The initial mode of the timer.

---

```cpp
ETL_CONSTEXPR etl::timer::interval::type get_interval() const ETL_NOEXCEPT
```
**Description**  
Returns the interval of the timer.

---

```cpp
ETL_CONSTEXPR etl::timer::mode::type get_mode() const ETL_NOEXCEPT
```
**Description**  
Returns the mode of the timer.  
One of:-  
`etl::timer::mode::No_Mode`  
`etl::timer::mode::Single_Shot`  
`etl::timer::mode::Repeating`  

---

```cpp
ETL_CONSTEXPR etl::timer::id::type get_id() const ETL_NOEXCEPT
```
**Description**  
Returns the id of the timer.  
The timer ID is set by registering a node that contains this configuration with an `etl::intrusive_timer`.

---

```cpp
ETL_CONSTEXPR14 void clear() ETL_NOEXCEPT
```
**Description**  
Clears the timer configuration.  
Interval = `etl::timer::interval::No_Interval`  
Mode     = `etl::timer::mode::No_Mode`  
ID       = `etl::timer::id::No_Timer`  
