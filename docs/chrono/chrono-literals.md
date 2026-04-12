---
title: "Chrono literals"
---

The ETL Chrono literals are define slightly differently from the STL in that they are user defined, as opposed to language defined.

**Example**  
For STL, the literal to define year `2025` would be `2025y`. 
For ETL, the literal is `2025_y`.

By default, the ETL uses the designations of the STL.  
As this may clash with other user defined literals, the ETL allows more verbose forms to be used, by defining the macro `ETL_USE_VERBOSE_CHRONO_LITERALS`.  
If enabled, the example of `2025_y` would be written as `2025_year`.

| Duration type               | STL like   | Verbose             |
| --------------------------- | ---------- | ------------------- |
| `etl::chrono::year`         | `2025_y`   | `2025_year`         |
| `etl::chrono::day`          | `10_d`     | `10_day`            |
| `etl::chrono::hours`        | `14_h`     | `14_hours`          |
| `etl::chrono::minutes`      | `30_min`   | `30_minutes`        |
| `etl::chrono::seconds`      | `45_s`     | `45_seconds`        |
| `etl::chrono::milliseconds` | `500_ms`   | `500_milliseconds`  |
| `etl::chrono::microseconds` | `500_us`   | `500_microseconds`  |
| `etl::chrono::nanoseconds`  | `500_ns`   | `500_nanoseconds`   |

Chrono literals may by accessed by using one of the following:-

```cpp
using namespace etl::chrono;
using namespace etl::literals;
using namespace etl::chrono_literals;
```
