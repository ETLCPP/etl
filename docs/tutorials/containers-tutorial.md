---
title: "Containers"
---

## Overview
The containers in the ETL are designed to complement those found in the STL. The main difference is that their capacity is defined at compile time and they contain their own storage. They do not allocate any storage from the heap at any time.  
This is an advantage on an embedded device as all of the memory required by the application will be defined upfront. Also due to the storage's contiguous nature, the containers are cache friendly.

Containers in the ETL library are mostly based around a common design theme of three classes.  
(Where container is the name of the relevant container).

```cpp
container<type, size>
icontainer<type>
container_base
```

**i.e.**
```cpp
etl::vector<type, size>
etl::ivector<type>
etl::vector_base
```

The `container_base` contains data and functions that are in independent of both the type and size.  
This is inherited by `icontainer<type>` that implements functionality that is dependent on the type, but not the size.  
Lastly, `container<type, size>` implements the small amount of functionality that requires both type and size information.  

The classes do not implement any virtual functions and, like STL containers, are not normally expected to be used as base classes themselves.  

Code using containers of a specific type can be written to be independent of the storage size through use of icontainer class references.  

## Example
```cpp
#include "etl/vector.h"
#include "etl/numeric.h"

#include <iostream>
#include <iterator>
#include <algorithm>

// Print takes a const reference to the interface container class.
void print(const etl::ivector<int>& v)
{
  std::copy(v.begin(), v.end(), std::ostream_iterator<int>(std::cout, " "));
  std::cout << "\n";
}

int main()
{
  // Declare the vector instances.
  etl::vector<int, 10> v1(10);
  etl::vector<int, 20> v2(20);

  etl::iota(v1.begin(), v1.end(), 0);  // Fill with 0 through 9
  etl::iota(v2.begin(), v2.end(), 10); // Fill with 10 through 29

  print(v1); // Prints 0 through 9
  print(v2); // Prints 10 through 29

  return 0;
}
```

