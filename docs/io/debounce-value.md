---
title: "debounce_value"
---

{{< callout type="info">}}
  Header: `debounce_value.h`  
  Since: `20.50.0`  
{{< /callout >}}

This class is designed to take an input value that may take time to become stable, such as a binary rotary encoder, and indicate when it has settled to a stable state.

---

Assuming the stable count has been set to `4`.  
Initial value is `0`.  
`-` = Unstable  
`#` = Stable  
```
       Input:  0  1  2  3  4  5  6  6  6  6  6  6  5  4  3  3  3  3  3  3  
       State:  -  -  -  -  -  -  -  -  -  #  #  #  -  -  -  -  -  #  #  #
Stable Value:  0  0  0  0  0  0  0  0  0  6  6  6  6  6  6  6  6  3  3  3
```

---

```cpp
template <typename TValue, typename TCount>
etl::debounce_value;
```              

## Types

`value_type` The type of the input value.  
`count_type` The type for the counter.  

## Member functions

### Constructor
```cpp
debounce_value(value_type initial_value, count_type stable_count)
```
**Description**  
Constructor.

**Parameters**  
`initial_value` : The initial value of the 'last' sample.  

`stable_count` : The count at which an input is deemed to be stable.  

### bool update(value_type)
```cpp
bool update(value_type value)
```
**Description**  
Adds a new value.  
If the value has changed, the count is reset and the value is marked as unstable.

**Parameters**  
`value` : The next input value.  
    
**Return**  
Returns `true` if the value has changed.

### value_type get()
```cpp
value_type get() const
```
**Description**  
Gets the current value.  
Gets the last stable value.

### value_type()
```cpp
operator value_type() const
```
**Description**  
Conversion operator.  
Gets the last stable value.

### bool has_changed()
```cpp
bool has_changed() const
```
**Description**  
Checks if the input has changed to a new stable state.  
    
**Return**  
`true` if the stable value has changed.  

### bool is_stable()
```cpp
bool is_stable() const
```
**Description**  
Checks if the input is in a stable state.  

**Return**  
`true` if the value is stable.  

### void set_stable_count(count_type)
```cpp
void set_stable_count(count_type stable_count)
```
**Description**  
Sets the count at which a non-changing input value is seen as stable.
