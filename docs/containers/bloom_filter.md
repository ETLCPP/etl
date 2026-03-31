---
title: "bloom_filter"
---

{{< callout >}}
  Header: `bloom_filter.h`  
  Supported: All versions  
{{< /callout >}}

```cpp
etl::bloom_filter<size_t Width, typename THash1, typename THash2, typename THash3>
```

A Bloom filter that supports up to three hash functions. `Thash2` & `Thash3` are optional.

The width specified is the desired width ib bits.  
The class may use more if the underlying bitset naturally has spare capacity.
i.e. A bloom filter with a specified capacity of 195 bits will be rounded up to 200 bits as the bitset has a block size of 8.

## Constructor

The initial state of the Bloom filter is clear.

```cpp
bloom_filter();
```

## Operations

```cpp
void clear()
```
**Description**  
Clears the filter of all entries.

---

```cpp
void add(parameter_t key)
```
**Description**  
Adds a key to the filter, where parameter_t is derived from the first hash argument type.

---

```cpp
bool exists(parameter_t key) const
```
**Description**  
Checks to see if a key may exist in the filter, where parameter_t is derived from the first hash argument type.

---

```cpp
size_t usage() const
```
**Description**  
Returns the usage of the filter in percent.  
Equal to `100 * count() / width()`

---

```cpp
bool count() const
```
**Description**  
Returns the number of elements in use in the filter.

---

```cpp
bool width() const
```
**Description**  
Returns the width of the filter.  
Equal to the template parameter WIDTH.

## Example

```cpp
#include "bloom_filter.h"
#include "fnv-1.h"
#include "string.h"

struct hash_t
{
  typedef const char* argument_type;

  size_t operator ()(argument_type text) const
  {
    return etl::fnv_1a_32(text, text + strlen(text));
  }
};

etl::bloom_filter<64, hash_t> bloom;

bloom.add("Hello");
bloom.add("World");
bloom.add("the");

bool test;

test = bloom.exists("Hello"); // Returns true
test = bloom.exists("You");   // Returns false
test = bloom.exists("you");   // Returns true (false positive)
```
