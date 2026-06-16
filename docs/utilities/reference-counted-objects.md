---
title: "Reference Counted Objects"
---

Reference counted objects types.  
Defines the following classes.

```cpp
etl::ireference_counter
```
The interface of all reference counters.  

---

```cpp
etl::reference_counter<TCounter>
```
A reference counter using TCounter as the counter.  

---

```cpp
etl::reference_counter<void>
```
A null reference counter. The reference count always reports as 1.  

---

```cpp
etl::ireference_counted_object
```
The interface of all reference counted object types.  

---

```cpp
etl::reference_counted_object<typename TObject, typename TCounter>
```
Derived from `etl::ireference_counted_object`  

---

```cpp
etl::atomic_counted_object<typename TObject>
```
A template alias of `etl::reference_counted_object<typename TObject, etl::atomic_int32_t>`  

## ireference_counter

```cpp
etl::ireference_counter
```
The interface of all reference counters.

```cpp
virtual ~ireference_counter() {};
virtual void set_reference_count(int32_t value) = 0;
virtual void increment_reference_count() = 0;
ETL_NODISCARD virtual int32_t decrement_reference_count() = 0;
ETL_NODISCARD virtual int32_t get_reference_count() const = 0;
```

## reference_counter

```cpp
etl::reference_counter<TCounter>
```
A reference counter that uses type TCounter to count.

---

```cpp
etl::reference_counter<void>
```
A dummy reference counter that  always returns a count of 1.  
Used for persistent objects.

## ireference_counted_object

```cpp
etl::ireference_counted_object<typename TObject, typename TCounter>
```
The interface of all reference counted object types.

---

```cpp
virtual ~ireference_counted_object()
```

---

```cpp
ETL_NODISCARD virtual etl::ireference_counter& get_reference_counter() = 0;
```
Get a reference to the reference counter.

---

```cpp
ETL_NODISCARD virtual const etl::ireference_counter& get_reference_counter() const = 0; 
```
Get a const reference to the reference counter.

## reference_counted_object

```cpp
etl::reference_counted_object<typename TObject, typename TCounter>
```
The implementation of a reference counted object.

---

```cpp
reference_counted_object(const TObject& object)
```
Constructs from an object.  
The object is copied.

---

```cpp
reference_counted_object()
```
Constructs from an onject.  
The object is default constructed.

---

```cpp
ETL_NODISCARD TObject& get_object() ETL_OVERRIDE
```
Get a reference to the object.

---

```cpp
ETL_NODISCARD const TObject& get_object() const ETL_OVERRIDE
```
Get a const reference to the object.

---

```cpp
ETL_NODISCARD etl::ireference_counter& get_reference_counter() ETL_OVERRIDE
```
Get a reference to the reference counter.

---

```cpp
ETL_NODISCARD const etl::ireference_counter& get_reference_counter() const ETL_OVERRIDE
```
Get a const reference to the reference counter.

## For C++11, with atomic support.
```cpp
template <typename TObject>
using atomic_counted_object = etl::reference_counted_object<TObject, etl::atomic_int32_t>;
```
**Description**  
Defines an alias to a reference counted object that uses an atomic.
