---
title: "duration_values"
---

{{< callout type="info">}}
  Header: `chrono.h`  
  Since: `20.41.0`  
  Similar to: [std::chrono::duration_values](https://en.cppreference.com/w/cpp/chrono/duration_values.html)
{{< /callout >}}

Templates representing a time interval.

## duration_values
```cpp
template <typename TRep>
struct duration_values
```
```cpp
ETL_NODISCARD static ETL_CONSTEXPR TRep zero() ETL_NOEXCEPT
```
**Return**  
`TRep(0)`

---

```cpp
ETL_NODISCARD static ETL_CONSTEXPR14 TRep min() ETL_NOEXCEPT
```
**Return**  
`etl::numeric_limits<TRep>::min()`

---

```cpp
ETL_NODISCARD static ETL_CONSTEXPR14 TRep max() ETL_NOEXCEPT
```
**Returns**  
`etl::numeric_limits<TRep>::max()`
