#include <iostream>

#include "etl/memory.h"
#include "etl/pool.h"

struct S
{
  S(int a_, double b_)
    : a(a_), b(b_) 
  {
  }

  int    a;
  double b;
};

// Custom deleter that returns S* to the pool
// This class is defined to be able to log the release of objects back to the pool.
// Normally, you would use the predefined etl::ipool::destroyer
struct Deleter
{
  Deleter(etl::pool<S, 10>& p)
    : pool(&p) {}

  void operator()(S* ptr) const
  {
    if (ptr)
    {
      std::cout << "Releasing S(" << ptr->a << ", " << ptr->b << ") back to pool." << std::endl;
      pool->destroy(ptr);
    }
  }

  etl::pool<S, 10>* pool;
};

int main()
{
  etl::pool<S, 10> pool;
  Deleter pool_deleter(pool);
  using Unique = etl::unique_ptr<S, Deleter>;

  Unique us1(pool.create(1, 2), pool_deleter);
  std::cout << "Created S(" << us1->a << ", " << us1->b << ") from pool." << std::endl;

  {
    Unique us2(pool.create(3, 4), pool_deleter);
    std::cout << "Created S(" << us2->a << ", " << us2->b << ") from pool." << std::endl;
  }

  Unique us3(pool.create(5, 6), pool_deleter);
  std::cout << "Created S(" << us3->a << ", " << us3->b << ") from pool." << std::endl;
}
