---
title: "last_spec"
---

{{< callout type="info">}}
  Header: `chrono.h`  
  Since: `20.41.0`  
  Similar to: [std::chrono::last_spec](https://en.cppreference.com/w/cpp/chrono/last_spec.html)
{{< /callout >}}

`last_spec` is a tag type that is used in conjunction with other calendar types to indicate the last thing in a sequence.

```cpp
struct last_spec
{
  explicit last_spec() = default;
};

inline constexpr last_spec last{};
```

**Example**  

`2025_y/March/last` for last day of March 2025 i.e. 2025-03-31

