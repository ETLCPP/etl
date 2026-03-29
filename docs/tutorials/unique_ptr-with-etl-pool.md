---
title: "unique_ptr with pool"
---

---

This example shows objects being created from an `etl::pool`, stored in an `etl::unique_ptr`, and released back
to the pool when the `etl::unique_ptr` goes out of scope.

```C++
#include <iostream>

#include "etl/memory.h"
#include "etl/pool.h"

// The object stored in the pool
struct S
{
  S(int a_, double b_)
    : a(a_), b(b_) 
  {
  }

  int    a;
  double b;
};

int main()
{
  etl::pool<S, 10> pool;
  auto pool_deleter = [&pool](auto ptr) 
    { 
      std::cout << "Releasing S(" << ptr->a << ", " << ptr->b << ") back to pool." << std::endl; 
      pool.destroy(ptr); 
    };

  using Unique = etl::unique_ptr<S, decltype(pool_deleter)>;

  Unique us1(pool.create(1, 2), pool_deleter);
  std::cout << "Created S(" << us1->a << ", " << us1->b << ") from pool." << std::endl;

  {
    Unique us2(pool.create(3, 4), pool_deleter);
    std::cout << "Created S(" << us2->a << ", " << us2->b << ") from pool." << std::endl;
  }

  Unique us3(pool.create(5, 6), pool_deleter);
  std::cout << "Created S(" << us3->a << ", " << us3->b << ") from pool." << std::endl;
}
```

## Output

Created S(1, 2) from pool.
Created S(3, 4) from pool.
Releasing S(3, 4) back to pool.
Created S(5, 6) from pool.
Releasing S(5, 6) back to pool.
Releasing S(1, 2) back to pool.
