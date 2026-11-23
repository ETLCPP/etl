---
title: "timer"
---

{{< callout type="info">}}
  Header: `timer.h`  
  Namespace: `etl::timer`  
{{< /callout >}}

Contains definitions common to timers.

## mode
**Types**  
`type`
The type for timer mode.  

**Constants**  
`No_Mode`  
When the timer has no mode set.  

`Singe_Shot`  
The timer expires once and then stops.  

`Repeating`  
The timer is restarted after every timeout.  

---

From: C++11  
`mode_t` = `mode::type`

## start
**Types**  
`type`
The type for timer mode.  

**Constants**  
`Immediate`  
- *Single Shot*  
The timer is triggered on the next tick. The normal timeout does not occur.  

- *Repeating*  
The timer is triggered on the next tick and then on subsequent timeouts.  

`Delayed`  
The timer is triggered on the timeout.

---

From: C++11  
`start_t` = `start::type`

## id
**Types**  
`type`  
The type for timer identifiers.  

**Constants**  
`No_Timer`  
The id of an unregistered timer.

---

From: C++11  
`id_t` = `id::type`

## interval
**Types**  
`type`  
The type for timer intervals.  

**Constants**  
`No_Interval`  
The when the timer has no interval set.

---

From: C++11  
`interval_t` = `interval::type`

## owner

**Types**  
`type`  
The type for timer owners.  

**Constants**  
`No_Owner`  
The when the timer node is not registered to a timer controller.

---

From: C++11  
`owner_t` = `owner::type`
