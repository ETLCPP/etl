---
title: "debounce_multi_bit"
---

{{< callout type="info">}}
  Header: `debounce_multi_bit.h`  
  Since: `20.50.0`  
{{< /callout >}}

Debounces a contiguous group of bits read from an IO port.

template <typename TValue, typename TCount, size_t Width, uint32_t Valid_Count, uint32_t Hold_Count = 0, uint32_t Repeat_Count = 0>
class debounce_multi_bit

| Parameter | Description |
|-|-|
|`TValue`       | The type of the port value. Must be an unsigned integral.                                                              |
|`Width`        | The number of bits in the field. `[1, sizeof(TValue) * CHAR_BIT)`                                                      |
|`Valid_Count`  | The number of consecutive samples a bit must differ from its previous debounced state before the state changes (>= 1). |
|`Hold_Count`   | The number of samples a bit must remain set (after becoming valid) before the 'held' state is reached. `0` = disabled. |
|`Repeat_Count` | Once 'held', a 'repeat' state is generated every `Repeat_Count` samples. `0` = disabled.                               |
|`TCount`       | The type of the per-bit counters. Must be able to hold the largest of `Valid_Count`, `Hold_Count` and `Repeat_Count`.  |

## Types
    
| Type | Definitions |
| - | - |
|`value_type` | `TValue`|
|`count_type` | `TCount`|
|`state_id` | `Off`, `On`, `Held`, `Repeating`|
|`state_id::type` | The type to hold a `state_id`|

## Member functions

```cpp
debounce_multi_bit(value_type initial)
```
**Description**  
Constructor.  
All bits start in the `Off` state.

---

```cpp
void reset(value_type initial)
```
**Description**  
Resets the debouncer state.  

**Parameters**  
`initial` : The default initial value is 'all inactive'. The value is the physical state of the input.  

---

```cpp
void reset_state(size_t n, bool initial)
```
**Description**  
The state is set to `Off` and the count is reset to `0`.  

**Parameters**  
`n` : The bit number `[0, Width)`. If `n` is out of range then it is ignored.  
`initial` : The default initial value is 'inactive'. The value is the physical state of the input.  

---

```cpp
bool update_incremental(value_type next)
```
**Description**  
Updates a *single* bit.  
The value of `next` must not change between calls to `update_incremental()` until all bits have been processed.  
Do not call `update()` after `update_incremental()` on the same instance unless `update_incremental()` has completed a full cycle (returned `true`).  
    
**Parameters**  
`next` The value of the whole multi bit field.  

**Return**  
`true` if all of the bits have been processed.

---

```cpp
bool update(value_type next)
```
**Description**  
Updates the whole port value.  

**Parameters**  
`next` The value of the whole multi bit field.  

**Return**  
`true` if any bit changed state.  

---

```cpp
bool has_changed(size_t n) const
```
**Description**  
Checks if a bit's state has changed.  

**Parameters**  
`n` The bit number `[0, Width)`.  

**Return**  
Returns `true` if the bit changed state on the last update.  

---

```cpp
bool is_set(size_t n) const
```
**Description**  
Returns `true` if the bit is currently set.  

**Parameters**  
`n` : The bit number `[0, Width)`.

---

```cpp
bool is_held(size_t n) const
```
**Description**  
Returns `true` if the bit is currently held.  

**Parameters**  
 `n` :  The bit number [0, Width).

---

```cpp
bool is_repeating(size_t n) const
```
**Description**  
Returns `true` if the bit is currently repeating.  

**Parameters**  
 `n` :  The bit number [0, Width).

---

```cpp
size_t get_width() const
```
**Description**  
Returns the width of the bit field.

---

```cpp
value_type get_value() const
```
**Description**  
Returns the current stable debounced value of all bits as a `value_type`.  

---

```cpp
state_id::type get_state(size_t n) const
```
**Description**  
Returns the current state id of a bit.

**Return**  
One of:-
`state_id::Off`  
`state_id::On`  
`state_id::Held`  
`state_id::Repeating`  

---

```cpp
value_type get_changed() const
```
**Description**  
Returns the changed state of the bit field as a `value_type`.
