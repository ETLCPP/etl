---
title: "duration_cast"
---

{{< callout type="info">}}
  Header: `chrono.h`  
  Since: `20.41.0`  
  Similar to: [std::chrono::duration_cast](https://en.cppreference.com/w/cpp/chrono/duration/duration_cast.html)  
{{< /callout >}}

Templates representing a time interval.

## duration_cast
```cpp
template <typename TToDuration, typename TRep, typename TPeriod>
ETL_CONSTEXPR14 
TToDuration duration_cast(const etl::chrono::duration<TRep, TPeriod>& d)
ETL_NOEXCEPT
```
**Description**  
Converts from one duration type to another.
