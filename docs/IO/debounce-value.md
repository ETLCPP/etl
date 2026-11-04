---
title: "debounce_value"
---

{{< callout type="info">}}
  Header: `debounce_value.h`  
  Since: `20.50.0`  
{{< /callout >}}

This class is designed to take an input value that may take time to become stable, such as a binary rotary encoder or DIP switches, and indicate when it has settled to a stable state.

```cpp
template <typename TValue, typename TCount>
etl::debounce_value;
```              

## Types

`value_type` : The type of the input value.  
`count_type` : The type for the counter.  


## Member functions

```cpp
debounce_value(value_type initial_value, count_type stable_count)
```
**Description**  
Constructor.

**Parameters**  
`initial_value` : The initial value of the 'last' sample.  

`stable_count` : The count at which an input is deemed to be stable.  

---

```cpp
bool add(value_type value)
```
**Description**  
Adds a new value.  
If the value has changed, the count is reset and the value is marked as unstable.

**Parameters**  
`value` : The next input value.  
    
**Return**  
Returns `true` if the value has changed.

---

```cpp
value_type get() const
```
**Description**  
Gets the current value.  
May not be stable unless `is_stable()` returns `true`.

---

```cpp
operator value_type() const
```
**Description**  
Conversion operator.  
May not be stable unless `is_stable()` returns `true`.

---

```cpp
bool has_changed() const
```
**Description**  
Gets the current change state.  
    
**Return**  
`true` if the stable value has changed.  

---

```cpp
bool is_stable() const
```
**Description**  
Gets the current stable state.  

**Return**  
`true` if the value is stable.  

---

```cpp
void set_stable_count(count_type stable_count)
```
**Description**  
Sets the count at which a non-changing input value is seen as stable.
