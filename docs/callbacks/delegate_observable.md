---
title: "delegate_observable"
---

{{< callout type="info">}}
  Header: `delegate_observable.h`  
  Since: `TBC`  
{{< /callout >}}

`etl::delegate_observable` is a variation on the observer pattern idea, but using delegates as the callback mechanism.

```cpp
template <typename TNotification, size_t Max_Observers>
class delegate_observable
```

`TNotification` is the notification type.
`Max_Observers` is the maximum number of observers that can be handled.

## Template deduction guide

```cpp
template <typename TNotification, typename... TDelegates>
delegate_observable(TNotification, TDelegates...)
  -> delegate_observable<TNotification, sizeof...(TDelegates)>;
```  

**Example**  
```cpp
etl::delegate<void(int)> delegate1;
etl::delegate<void(int)> delegate2;

etl::delegate_observable observable(int{}, delegate1, delegate2);
```

## Public types

| Type                | Description                                 |
| ------------------- | ------------------------------------------- |
| `delegate_type`     | The type of delegate used in this observer. |
| `size_type`         | The type used internally for sizes.         |
| `notification_type` | The type of the notification.               |

## Construction

```cpp
ETL_CONSTEXPR14 delegate_observable()
```
**Description**  
Default constructor.

---

```cpp
template <typename... TDelegate>
ETL_CONSTEXPR14 delegate_observable(TDelegate&&... delegates)
```
**Description**  
Construct from a collection of observers.

---

```cpp
template <typename... TDelegate>
ETL_CONSTEXPR14 delegate_observable(notification_type, TDelegate&&... delegates)
```
**Description**  
Construct from notification type and a list of observers.  
Variant for template deduction guide.  
The notification value is ignored. It is here to allow deduction of the notification type for the template deduction guide.  

## Modifiers

```cpp
ETL_CONSTEXPR14 bool add_observer(delegate_type observer)
```
**Description**  
Add an observer to the list.  

**Return**  
`true` if the observer was removed, `false` if not.

---

```cpp
ETL_CONSTEXPR14 bool remove_observer(const delegate_type& observer)
```
**Description**  
Remove a particular observer from the list.  
**Return**  
`true if the observer was removed, false if not.

---

```cpp
ETL_CONSTEXPR14 void clear_observers()
```
**Description**  
Clear all observers.

## Status

```cpp
ETL_CONSTEXPR14 size_type number_of_observers() const
```
**Return**  
The number of observers.

## Notofication

```cpp
ETL_CONSTEXPR14 void notify_observers(notification_type n) const
```
**Description**  
Notify all of the observers, sending them the notification.
