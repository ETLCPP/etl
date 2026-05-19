---
title: "variant_pool"
---

This template class allows instances of multiple derived types to be constructed from a common store.  

**C++03**  
It may support up to 16 types and MAX_SIZE instances.  

```cpp
template <const size_t MAX_SIZE,
          typename T1,         typename T2  = void, typename T3  = void, typename T4  = void,
          typename T5  = void, typename T6  = void, typename T7  = void, typename T8  = void, 
          typename T9  = void, typename T10 = void, typename T11 = void, typename T12 = void,
          typename T13 = void, typename T14 = void, typename T15 = void, typename T16 = void>
```

---

**C++11 and above**  
It supports a variable number of types and `MAX_SIZE` instances.  

```cpp
template <size_t MAX_SIZE_, typename... Ts>
class variant_pool
```

```cpp
template <typename... Ts>
class variant_pool_ext : public etl::generic_pool_ext<etl::largest<Ts...>::size, etl::largest<Ts...>::alignment>
```

Note: The caller of the pools's `create` becomes the owner of the instance. It must call `destroy`.

## Constants

`MAX_SIZE`  
The maximum number of instances that may be created.

## Member functions

```cpp
template <typename T>
T* create()
```

**C++03**  
```cpp
template <typename T, typename TP1>
T* create(const TP1& p1)

template <typename T, typename TP1, typename TP2>
T* create(const TP1& p1, const TP2& p2)

template <typename T, typename TP1, typename TP2, typename TP3>
T* create(const TP1& p1, const TP2& p2, const TP3& p3)

template <typename T, typename TP1, typename TP2, typename TP3, typename TP4>
T* create(const TP1& p1, const TP2& p2, const TP3& p3, const TP4& p4)
```

Creates an instance of T with zero to four constructor parameters. The parameters are forwarded to T's constructor.  
Returns a pointer to the constructed instance or `nullptr`.  
If T if not one of those declared in the template parameter list then a compile time error will occur.  
If `T` cannot be created due to there being no spare capacity then an `etl::pool_element_size` is emitted. If exceptions are not configured to be thrown then the function returns `nullptr`.  

---

**C++11 and above**  
template <typename T, typename... Args>
T* create(Args&&... args)
**Description**  
Creates an instance of `T` with the supplied parameters. The parameters are forwarded to `T`'s constructor.
Returns a pointer to the constructed instance or `nullptr`.
If `T` if not one of those declared in the template parameter list then a compile time error will occur.
If `T` cannot be created due to there being no spare capacity then an `etl::pool_element_size` is emitted. If exceptions are not configured to be thrown then the function returns `nullptr`.

---

```cpp
bool destroy(const T* const p)
```
**Description**  
Destroys the instance pointed to by `p`.
Compile time error if `T` is not one of, or not a base of, one the supported types.
If the pool is empty or `p` does not belong to it then an `etl::pool_object_not_in_pool` is emitted.
If exceptions are not configured to be thrown then the function returns `true` for success and `false` for failure.

---

```cpp
size_t max_size() const
```
**Description**  
Returns the maximum number of items in the pool.

---

```cpp
size_t available() const
```
**Description**  
Returns the number of free items in the pool.

---

```cpp
size_t size() const
```
**Description**  
Returns the number of allocated items in the pool.

---

```cpp
bool empty() const
```
**Description**  
Checks to see if there are no allocated items in the pool.

---

```cpp
bool full() const
```
**Description**  
Checks to see if there are no free items in the pool.

## Example

```cpp
//***********************************
struct Base
{
  virtual ~Base()
  {
  }
};

//***********************************
struct Derived1 : public Base
{
  enum 
  {
    ID = 1
  };
};

//***********************************
struct Derived2 : public Base
{
  enum 
  {
    ID = 2
  };

  Derived2(int a_)
    : a(a_)
  {
  }

  int a;
};

//***********************************
struct Derived3 : public Base
{
  enum 
  {
    ID = 3
  };

  Derived3(int a_, double b_)
    : a(a_),
      b(b_)
  {
  }

  int a;
  double b;
}

//***********************************
struct Derived4 : public Base
{
  enum 
  {
    ID = 4
  };
};

// A pool that can create up to three instances of Derived1, Derived2, Derived3.
// Notice that the types can be declared in any order.
typedef etl::variant_pool<3, Derived1, Derived3, Derived2> VariantPool;

VariantPool pool;

// Create an instance of Derived1.
Derived1* pb1 = pool.create<Derived1>();

// Create an instance of Derived2 from its id, with constructor parameter 3.
Derived2* pb2 = pool.create<Derived2>(3);

// Create an instance of Derived3 with constructor parameters 4 and 1.2.
Derived3* pb3 = pool.create<Derived3>(4, 1.2);

// Create an instance of Derived4.
Derived4* pb4 = pool.create<Derived4>(); // Compile time error.

// Destroy the instances.
pool.destroy(pb2);
pool.destroy(pb1);
pool.destroy(pb3);
```
