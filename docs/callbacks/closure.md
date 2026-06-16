---
title: "closure"
---

`etl::closure` wraps an `etl::delegate` and a set of arguments.

**C++11 and above**  
```cpp
etl::closure<TReturn(TArgs...)>
```

**C++03.**
```cpp
etl::delegate<TReturn(TArg0)>
etl::delegate<TReturn(TArg0, TArg1)>
etl::delegate<TReturn(TArg0, TArg1, TArg2)>
etl::delegate<TReturn(TArg0, TArg1, TArg2, TArg3)>
etl::delegate<TReturn(TArg0, TArg1, TArg2, TArg3, TArg4)>
```

## Types

`delegate_type` The delegate type `etl::delegate<TReturn(TArgs...)>`

## Constructor

C++11 and above.
```cpp
ETL_CONSTEXPR14 closure(const delegate_type& f, const TArgs... args)
```

C++03.
```cpp
closure(const delegate_type& f, const TArg0 arg0)
closure(const delegate_type& f, const TArg0 arg0, const TArg1 arg1)
closure(const delegate_type& f, const TArg0 arg0, const TArg1 arg1, const TArg2 arg2)
closure(const delegate_type& f, const TArg0 arg0, const TArg1 arg1, const TArg2 arg2,
                                const TArg3 arg3)
closure(const delegate_type& f, const TArg0 arg0, const TArg1 arg1, const TArg2 arg2,
                                const TArg3 arg3, const TArg4 arg4)
```

## Operations
```cpp
template <size_t index, typename UArg>
void bind(UArg arg)
```
**Description**  
Bind a new value to the argument at the specified index.  
`UArg` must be one of `TArgs`.  
Only non-reference types can be rebound.  
Static asserts if `UArg` is not convertible to the closure's argument.  
Static asserts if `UArg` is a reference type.  
*C++11 and above.*  

---

```cpp
template <typename... UArgs>
void bind(UArgs&&... args)
```
**Description**  
Bind new values to all of the arguments at once.  
The number of arguments must match the delegate.  
Static asserts if the number of arguments don't match the delegate.  
*C++11 and above.*  

---

```cpp
ETL_CONSTEXPR14 TReturn operator()(TParams... args) const
```
**Description**  
Invokes the stored delegate with the bound arguments.

