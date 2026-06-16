---
title: "infinite_loop"
---

{{< callout type="info">}}
  Header: `infinite_loop.h`
{{< /callout >}}

A portable infinite loop that will not be optimised away by the compiler.

Before C++26, an empty infinite loop without side effects is considered undefined behaviour and may
be optimised away. This utility uses a compiler memory barrier to prevent that optimisation.

See [P2809R1](https://www.open-std.org/jtc1/sc22/wg21/docs/papers/2023/p2809r1.html) for background.

## Function

```cpp
[[noreturn]] inline void etl::infinite_loop()
```

**Description**
Enters an infinite loop that is guaranteed not to be removed by the compiler.

On GCC and Clang this is achieved with an inline assembly memory clobber.
On MSVC this uses `_ReadWriteBarrier()`.

## Example

```cpp
#include <etl/infinite_loop.h>

int main()
{
  // Initialise hardware...

  etl::infinite_loop();
}
```
