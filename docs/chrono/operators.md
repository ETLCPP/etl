---
title: "Operators"
---

{{< callout type="info">}}
  Header: `chrono.h`  
  Since: `20.41.0`  
  Similar to: [std::chrono::operator /](https://en.cppreference.com/w/cpp/chrono/operator_slash.html)
{{< /callout >}}

Operators for building dates.

## month_day

Create from month and day.

```cpp
ETL_CONSTEXPR14 
etl::chrono::month_day operator /(const etl::chrono::month& m,
                                  const etl::chrono::day& d) 
ETL_NOEXCEPT
```

```cpp
ETL_CONSTEXPR14 
etl::chrono::month_day operator /(const etl::chrono::month& m, 
                                  int d) 
ETL_NOEXCEPT
```

```cpp
ETL_CONSTEXPR14 
etl::chrono::month_day operator /(int m, 
                                  const etl::chrono::day& d) 
ETL_NOEXCEPT
```

```cpp
ETL_CONSTEXPR14 
etl::chrono::month_day operator /(const etl::chrono::day& d, 
                                  const etl::chrono::month& m) 
ETL_NOEXCEPT
```

```cpp
ETL_CONSTEXPR14 
etl::chrono::month_day operator /(const etl::chrono::day& d, 
                                  int m) 
ETL_NOEXCEPT
```

## month_day_last

Create from month and last_spec.

```cpp
ETL_CONSTEXPR14 
etl::chrono::month_day_last operator /(const etl::chrono::month& m,
                                       etl::chrono::last_spec)
ETL_NOEXCEPT
```

```cpp
ETL_CONSTEXPR14 
etl::chrono::month_day_last operator /(int m, 
                                       etl::chrono::last_spec) 
ETL_NOEXCEPT
```

```cpp
ETL_CONSTEXPR14 
etl::chrono::month_day_last operator /(etl::chrono::last_spec,
                                       const etl::chrono::month& m) 
ETL_NOEXCEPT
```
   
```cpp   
ETL_CONSTEXPR14 
etl::chrono::month_day_last operator /(etl::chrono::last_spec, 
                                      int m) 
ETL_NOEXCEPT
```

## month_weekday

Create from month and weekday.

```cpp
ETL_CONSTEXPR14 
etl::chrono::month_weekday operator /(const etl::chrono::month& m,
                                      const etl::chrono::weekday_indexed& wdi) 
ETL_NOEXCEPT
```

```cpp
ETL_CONSTEXPR14 
etl::chrono::month_weekday operator /(int m, 
                                      const etl::chrono::weekday_indexed& wdi) 
ETL_NOEXCEPT
```

```cpp
ETL_CONSTEXPR14 
etl::chrono::month_weekday operator /(const etl::chrono::weekday_indexed& wdi,
                                      const etl::chrono::month& m) 
ETL_NOEXCEPT
```

```cpp
ETL_CONSTEXPR14 
etl::chrono::month_weekday operator /(const etl::chrono::weekday_indexed& wdi, 
                                      int m) 
ETL_NOEXCEPT
```

## month_weekday_last

Create from month and weekday_last.

```cpp
ETL_CONSTEXPR14 
etl::chrono::month_weekday_last operator /(const etl::chrono::month& m,
                                           const etl::chrono::weekday_last& wdl)
ETL_NOEXCEPT
```

```cpp
ETL_CONSTEXPR14 
etl::chrono::month_weekday_last operator /(int m,
                                           const etl::chrono::weekday_last& wdl) 
ETL_NOEXCEPT
```

```cpp
ETL_CONSTEXPR14 
etl::chrono::month_weekday_last operator /(const etl::chrono::weekday_last& wdl,
                                           const etl::chrono::month& m) 
ETL_NOEXCEPT
```

```cpp
ETL_CONSTEXPR14 
etl::chrono::month_weekday_last operator /(const etl::chrono::weekday_last& wdl,
                                           int m) 
ETL_NOEXCEPT
```

## year_month

Create from year and month.

```cpp
ETL_CONSTEXPR14 
etl::chrono::year_month operator /(const etl::chrono::year& y,
                                   const etl::chrono::month& m) 
ETL_NOEXCEPT
```

```cpp
ETL_CONSTEXPR14 
etl::chrono::year_month operator /(const etl::chrono::year& y,
                                   int m) 
ETL_NOEXCEPT
```

## year_month_day

Create from year, month, and day.

```cpp
ETL_CONSTEXPR14 etl::chrono::year_month_day operator /(const etl::chrono::year_month& ym,
                                                       const etl::chrono::day& d) 
ETL_NOEXCEPT
```

```cpp
ETL_CONSTEXPR14 etl::chrono::year_month_day operator /(const etl::chrono::year_month& ym, 
                                                       int d ) 
ETL_NOEXCEPT
```

## year_month_day

Create from year and month_day.

```cpp
ETL_CONSTEXPR14 
etl::chrono::year_month_day operator /(const etl::chrono::year& y,
                                       const etl::chrono::month_day& md) 
ETL_NOEXCEPT
```

```cpp
ETL_CONSTEXPR14 
etl::chrono::year_month_day operator /(int y,
                                       const etl::chrono::month_day& md) 
ETL_NOEXCEPT
```

```cpp
ETL_CONSTEXPR14 
etl::chrono::year_month_day operator /(const etl::chrono::month_day& md,
                                       const etl::chrono::year& y) 
ETL_NOEXCEPT
```

```cpp
ETL_CONSTEXPR14 
etl::chrono::year_month_day operator /(const etl::chrono::month_day& md,
                                       int y) 
ETL_NOEXCEPT
```

## year_month_day_last

Create from year_month and last_spec.

```cpp
ETL_CONSTEXPR14 
etl::chrono::year_month_day_last operator /(const etl::chrono::year_month& ym,
                                            etl::chrono::last_spec) 
ETL_NOEXCEPT
```

```cpp
ETL_CONSTEXPR14 
etl::chrono::year_month_day_last operator /(const etl::chrono::year& y,
                                            const etl::chrono::month_day_last& mdl) 
ETL_NOEXCEPT
```

```cpp
ETL_CONSTEXPR14 
etl::chrono::year_month_day_last operator /(int y,
                                            const etl::chrono::month_day_last& mdl) 
ETL_NOEXCEPT
```

```cpp
ETL_CONSTEXPR14 
etl::chrono::year_month_day_last operator /(const etl::chrono::month_day_last& mdl,
                                            const etl::chrono::year& y)
ETL_NOEXCEPT
```

```cpp
ETL_CONSTEXPR14 
etl::chrono::year_month_day_last operator /(const etl::chrono::month_day_last& mdl,
                                            int y) 
ETL_NOEXCEPT
```

## year_month_weekday

Create from year_month and weekday.

```cpp
ETL_CONSTEXPR14 
etl::chrono::year_month_weekday operator /(const etl::chrono::year_month& ym,
                                           const etl::chrono::weekday_indexed& wdi) 
ETL_NOEXCEPT
```

```cpp
ETL_CONSTEXPR14 
etl::chrono::year_month_weekday operator /(const etl::chrono::year& y,
                                           const etl::chrono::month_weekday& mwd) 
ETL_NOEXCEPT
```

```cpp
ETL_CONSTEXPR14 
etl::chrono::year_month_weekday operator /(int y, 
                                           const etl::chrono::month_weekday& mwd) 
ETL_NOEXCEPT
```

```cpp
ETL_CONSTEXPR14 
etl::chrono::year_month_weekday operator /(const etl::chrono::month_weekday& mwd,
                                           const etl::chrono::year& y) 
ETL_NOEXCEPT
```

```cpp
ETL_CONSTEXPR14 
etl::chrono::year_month_weekday operator /(const etl::chrono::month_weekday& mwd, 
                                           int y) 
ETL_NOEXCEPT
```

## year_month_weekday_last

Create from year_month and weekday_last.

```cpp
ETL_CONSTEXPR14 
etl::chrono::year_month_weekday_last operator /(const etl::chrono::year_month& ym,
                                                const etl::chrono::weekday_last& wdl) 
ETL_NOEXCEPT
```

```cpp
ETL_CONSTEXPR14 
etl::chrono::year_month_weekday_last operator /(const etl::chrono::year& y,
                                                const etl::chrono::month_weekday_last& mwdl) 
ETL_NOEXCEPT
```

```cpp
ETL_CONSTEXPR14 
etl::chrono::year_month_weekday_last operator /(int y,
                                                const etl::chrono::month_weekday_last& mwdl) 
ETL_NOEXCEPT
```

```cpp
ETL_CONSTEXPR14 
etl::chrono::year_month_weekday_last operator /(const etl::chrono::month_weekday_last& mwdl,
                                                const etl::chrono::year& y) 
ETL_NOEXCEPT
```

```cpp
ETL_CONSTEXPR14 
etl::chrono::year_month_weekday_last operator /(const etl::chrono::month_weekday_last& mwdl,
                                                int y) 
ETL_NOEXCEPT
```
