---
title: "time_to_next_notification"
weight: 104
---

{{< callout type="info">}}
  Header: `timer_notification.h`  
  Namespace: `etl::timer`  
  Since: `20.50.0`  
  From: `C++03`
{{< /callout >}}

## Description

## time_to_next_notification
```cpp
timer_time_to_next_notification
```
**Description**  
This is the base class for 'time to next' notifications.  
Derive from this to define a custom implementation.  

```cpp
virtual void invoke(etl::timer::interval::type)
```
If not overridden then does nothing.  