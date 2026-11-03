---
title: "Intrusive timers"
weight: 100
---

{{< callout type="info">}}
  Since: `20.50.0`  
  From: `C++03`
{{< /callout >}}

Classes that implement intrusive timer-based services.

### etl::timer::config

Contains the configuration for a timer.  
Used to configure a timer for an `etl::timer::node` derived timer element.

### etl::timer::node

Base class for all timer nodes.  
Used as the timer element in `etl::intrusive_timer`.

Predefined implementations:-  
`callback_node` `callback_ref_node` `message_node`  

### etl::lock_policy

Interface class for custom lock policies.  

Predefined implementations:-  
`no_lock_policy` `atomic_lock_policy`

### time_to_next_notification

Base class for `time_to_next_notification` implementations.

### etl::intrusive_timer

The `etl::intrusive_timer` is a replacement for all legacy timers within the ETL.
