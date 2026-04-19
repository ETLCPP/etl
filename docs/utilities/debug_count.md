---
title: "debug_count"
---

A utility class designed for debugging purposes. This is used in the ETL containers to check that all placement new'd items have been destructed. When neither `DEBUG` or `_DEBUG` is defined then this class does nothing and will be optimised away.

If the count is decremented below zero then an assert will be generated.  
If the destructor for this class is called and the count is not zero, then an assert will be generated.  

```cpp
#define ETL_DECLARE_DEBUG_COUNT              etl::debug_count etl_debug_count
#define ETL_SET_DEBUG_COUNT(n)               etl_debug_count.set(n)
#define ETL_GET_DEBUG_COUNT                  etl_debug_count.get()
#define ETL_INCREMENT_DEBUG_COUNT            ++etl_debug_count
#define ETL_DECREMENT_DEBUG_COUNT            --etl_debug_count
#define ETL_ADD_DEBUG_COUNT(n)               etl_debug_count += (n)
#define ETL_SUBTRACT_DEBUG_COUNT(n)          etl_debug_count -= (n)
#define ETL_RESET_DEBUG_COUNT                etl_debug_count.clear()
#define ETL_OBJECT_RESET_DEBUG_COUNT(object) object.etl_debug_count.clear()
#define ETL_OBJECT_GET_DEBUG_COUNT(object)   object.etl_debug_count.get()
```

## Example
```cpp
// A container class (very simplified)
class MyContainer
{
public:

  void push(value_type value)
  {
    value_type* p = pool.allocate<value_type>();
    new (p) value_type(value);
    ETL_INCREMENT_DEBUG_COUNT;
  }

  void pop()
  {
    p->~value_type();
    pool.release(p);
    ETL_DECREMENT_DEBUG_COUNT
  }

private:

  ETL_DECLARE_DEBUG_COUNT;
};
```
