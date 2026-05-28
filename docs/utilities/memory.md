---
title: "memory"
---

## address_of
```cpp
template <typename T>
T* addressof(T& t)
```
**Description**  
Returns the address of an object.

## default_delete
```cpp
template <typename T>
struct default_delete
```

---

```cpp
template <typename T>
struct default_delete<T[]>
```

## create_default_at

```cpp
template <typename T>
void create_default_at(T* p)
```
Creates a default value. For POD types this will be undefined.  

---

```cpp
template <typename T, typename TCounter>
void create_default_at(T* p, TCounter count)
```
Creates a default value. For POD types this will be undefined.  
The supplied counter will be incremented.

## create_value_at
```cpp
template <typename T>
void create_value_at(T* p)
```
Creates a default value by constructing the item with `T()`.  

---

```cpp
template <typename T, typename TCounter>
void create_value_at(T* p, Tcounter count)
```
Creates a default value by constructing the item with `T()`.  
The supplied a counter will be incremented.

## create_copy_at
```cpp
template <typename T>
void create_copy_at(T* p)
```
Creates a default value by constructing the item with `T(value)`.  

---

```cpp
template <typename T, typename TCounter>
void create_copy_at(T* p, const T& value, Tcounter count)
```
Creates a default value by constructing the item with `T(value)`.  
The supplied counter will be incremented.  

## make_default_at

```cpp
template <typename T>
T& make_default_at(T* p)
```
Creates a default value. For POD types this will be undefined.  
Returns a reference to the new object.  

---

```cpp
template <typename T, typename TCounter>
T& make_default_at(T* p, Tcounter count)
```
**Description**  
Creates a default value. For POD types this will be undefined.  
Returns a reference to the new object.  
The supplied counter will be incremented.  

## make_value_at
```cpp
template <typename T>
T& make_value_at(T* p)
```
Creates a default value by constructing the  item with `T()`.  
Returns a reference to the new object.  

---

```cpp
template <typename T, typename TCounter>
T& make_value_at(T* p, Tcounter count)
```
Creates a default value by constructing the  item with `T()`.  
Returns a reference to the new object.  

## make_copy_at
Creates a default value by constructing the  item with `T(value)`.  
Returns a reference to the new object.  
The second version is supplied a counter, which will be incremented.  

```cpp
template <typename T>
T& make_copy_at(T* p)
```

---

```cpp
template <typename T, typename TCounter>
T& make_copy_at(T* p, const T& value, Tcounter count)
```

## destroy_at
```cpp
template <typename T>
void destroy_at(T* p)
```
Calls the destructor for non-pod types.  

---

```cpp
template <typename T, typename TCounter>
void destroy_at(T* p, TCounter& count)
```
Calls the destructor for non-pod types.  
The supplied counter will be decremented.  

## destroy_n
```cpp
template <typename T, typename TSize >
void destroy_n(T* p, TSize n)
```
Calls the destructor for a range of non-pod types, starting at address `p`.  

---

```cpp
template <typename T, typename TSize, typename TCounter>
void destroy_n(T* p, TSize n, TCounter& count)
```
Calls the destructor for a range of non-pod types, starting at address `p`.  
The supplied counter will be decremented by the number of items destructed.  

## destroy
```cpp
template <typename T>
void destroy(T* p, T* p_end)
```
Calls the destructor for a range of non-pod types, starting at address `p`.  

---

```cpp
template <typename T, typename TCounter>
void destroy(T* p, T* p_end, TCounter& count)
```
Calls the destructor for a range of non-pod types, starting at address `p`.  
The supplied counter will be decremented by the number of items destructed.  

## create_copy

```cpp
template <typename T>
struct create_copy
```
Derive from this, passing the derived class as the template parameter.  
Provides the following member functions that constructs a copy at the specified address.  

---

```cpp
void create_copy_at(void* p);
```

---

```cpp
template <typename TCounter>
void create_copy_at(void* p, Tcounter& count);
```

---

```cpp
T& make_copy_at(void* p);
```

---

```cpp
template <typename TCounter>
T& make_copy_at(void* p, Tcounter& count);
```

### Example:
```cpp
class Test : public etl::create_copy<Test>
{
  // Other members.
};

// Allocate memory large enough to contain a 'Test' object.
char buffer[sizeof(Test)];

Test test;

// Copy construct 'test' into the buffer memory.
Test& t = test.make_copy_at(buffer);
```
---

Create / Destroy objects
Functions that create or destroy objects in uninitialised memory.
Variations of the functions above that don't require an explicit `reinterpret_cast`.

```cpp
template <typename TObject>
TObject& construct_object_at(void* p, TObject&& object)
```
Construct the object at `p`.  
In a debug build the pointer is checked for correct alignment.  
An `etl::alignment_error` is asserted if incorrect.  
From: `20.35.12`

---

```cpp
template <typename TObject, typename... TArgs>
TObject& construct_object_at(void* p, TArgs&&... args)
```
Construct the object at `p` from arguments.  
In a debug build the pointer is checked for correct alignment.  
An `etl::alignment_error` is asserted if incorrect.  
From: `20.35.12`

---

```cpp
template <typename TObject>
TObject& get_object_at(void* p)
```
Get the object at `p`.  
In a debug build the pointer is checked for correct alignment.  
An `etl::alignment_error` is asserted if incorrect.  
From: `20.35.12`

---

```cpp
template <typename TObject>
void destroy_object_at(void* p)
```
Destroy the object at `p`.  
In a debug build the pointer is checked for correct alignment. An etl::alignment_error is asserted if incorrect.  
From: `20.35.12`  

## uninitialized_fill
Fills uninitialised memory with N values.  

```cpp
template <typename TIterator, typename TSize, typename T>
TIterator uninitialized_fill_n(TIterator o_begin, TSize count, const T& value)
```

---

```cpp
template <typename TIterator, typename TSize, typename T, typename TCounter>
TIterator uninitialized_fill_n(TIterator o_begin, TSize count, const T& value, typename TCounter)
```
---

```cpp
template <typename TIterator, typename T>
TIterator uninitialized_fill(TIterator o_begin, TIterator o_end, const T& value)
```

---

```cpp
template <typename TIterator, typename TSize, typename T, typename TCounter>
TIterator uninitialized_fill_n(TIterator o_begin, TSize count, const T& value, typename TCounter)
```
Fills uninitialised memory range with a value.

## uninitialised_copy

```cpp
template <typename TInputIterator, typename TOutputIterator>
TIterator uninitialized_copy(TInputIterator i_begin, TInputIterator i_end, TOutputIterator o_begin)
```

```cpp
template <typename TInputIterator, typename TOutputIterator, typename TCounter>
TIterator uninitialized_copy(TInputIterator i_begin, TInputIterator i_end, TOutputIterator o_begin, typename TCounter)
```
Copies a range of objects to uninitialised memory. 

## uninitialized_copy_n

```cpp
template <typename TInputIterator, typename TSize, typename TOutputIterator>
TIterator uninitialized_copy_n(TInputIterator i_begin, TSize count, TOutputIterator o_begin)
```

---

```cpp
template <typename TInputIterator, typename TSize, typename TOutputIterator, typename TCounter>
TIterator uninitialized_copy_n(TInputIterator i_begin, TSize count, TOutputIterator o_begin, typename TCounter)
```
Copies N objects to uninitialised memory.

## uninitialized_move

```cpp
template <typename TInputIterator, typename TOutputIterator>
TIterator uninitialized_move(TInputIterator i_begin, TInputIterator i_end, TOutputIterator o_begin)
```

```cpp
template <typename TInputIterator, typename TOutputIterator, typename TCounter>
TIterator uninitialized_move(TInputIterator i_begin, TInputIterator i_end, TOutputIterator o_begin, typename TCounter)
```
Moves a range of objects to uninitialised memory.  

Note: If using C++03 then this function will call `etl::uninitialized_copy`  

## uninitialized_copy_n

```cpp
template <typename TInputIterator, typename TSize, typename TOutputIterator>
TIterator uninitialized_move_n(TInputIterator i_begin, TSize count, TOutputIterator o_begin)
```

---

```cpp
template <typename TInputIterator, typename TSize, typename TOutputIterator, typename TCounter>
TIterator uninitialized_move_n(TInputIterator i_begin, TSize count, TOutputIterator o_begin, typename TCounter)
```
Moves N objects to uninitialised memory.  

Note: If using C++03 then this function will call `etl::uninitialized_copy_n`

## uninitialized_default_construct

```cpp
template <typename TOutputIterator>
void uninitialized_default_construct(TOutputIterator o_begin, TOutputIterator o_end)
```

---

```cpp
template <typename TOutputIterator, typename TCounter>
void uninitialized_default_construct(TOutputIterator o_begin, TOutputIterator o_end, typename TCounter)
```

---

```cpp
template <typename TOutputIterator, typename TSize>
TOutputIterator uninitialized_default_construct_n(TOutputIterator o_begin, TSize n)
```

---

```cpp
template <typename TOutputIterator, typename TSize, typename TCounter>
TOutputIterator uninitialized_default_construct_n(TOutputIterator o_begin, TSize n, TCounter& count)
```
Creates default values. For POD types this will be undefined.

## uninitialized_value_construct

```cpp
template <typename TOutputIterator>
void uninitialized_value_construct(TOutputIterator o_begin, TOutputIterator o_end)
```

---

```cpp
template <typename TOutputIterator, typename TCounter>
void uninitialized_value_construct(TOutputIterator o_begin, TOutputIterator o_end, TCounter& count)
```

---

```cpp
template <typename TOutputIterator, typename TSize>
TOutputIterator uninitialized_value_construct_n(TOutputIterator o_begin, TSize n)
```

---

```cpp
template <typename TOutputIterator, typename TSize, typename TCounter>
TOutputIterator uninitialized_value_construct_n(TOutputIterator o_begin, TSize n, TCounter& count)
```
Creates values constructed with `T()`.

## unique_ptr
Like `std::unique_ptr`, `etl::unique_ptr` is a smart pointer that owns and manages another object through a pointer and disposes of that object when the `unique_ptr` goes out of scope.  

https://en.cppreference.com/w/cpp/memory/unique_ptr

```cpp
template<typename T>
class unique_ptr 
```

---

```cpp
template<typename T>
class unique_ptr<T[]> 
```

## Memory clear

```cpp
void memory_clear(volatile char* p, size_t n)
```

---

```cpp
template <typename T>
void memory_clear(volatile T &object)
```
A low level function that clears an object's memory to zero.  

## memory_clear_range

```cpp
template <typename T>
void memory_clear_range(volatile T* begin, size_t n)
```

---

```cpp
template <typename T>
void memory_clear_range(volatile T* begin, volatile T* end)
```
A low level function that clears a range to zero.  

## memory_set

```cpp
void memory_set(volatile char* p, size_t n, char value)
```

```cpp
template <typename T>
void memory_set(volatile T &object, char value)
```
Low level functions that clear an object's memory to a value.

## memory_set_range

```cpp
template <typename T>
void memory_set_range(volatile T* begin, size_t n, char value)
```

---

```cpp
template <typename T>
void memory_set_range(volatile T* begin, volatile T* end, char value)
```
Low level functions that set a range to a value.

## wipe_on_destruct

A template class that will wipe the derived objects storage to zero on destruction.  
Designed to eliminate sensitive data lurking around in memory after an object has been destructed.  

```cpp
template <typename T>
struct wipe_on_destruct
```
`T` is the derived class whose storage must be wiped.

### Example
```cpp
struct UserData : public etl::wipe_on_destruct<UserData>
{
  char secret_passcode[16];
  char sensitive_user_name[16];
};
```

When an instance of `UserData` is destructed, the memory that it occupied will be set to zero.

## uninitialized_buffer

```cpp
template <size_t VObject_Size, size_t VN_Objects, size_t VAlignment>
class uninitialized_buffer
```
Creates an uninitialized memory buffer of `VN_Objects` each of `VObject_Size` with alignment `VAlignment`.

## uninitialized_buffer_of

```cpp
template <typename T, size_t VN_Objects>
class uninitialized_buffer_of
```
Creates an uninitialized memory buffer of `VN_Objects` each of type `T`.

## mem_copy
From: `20.26.0`

```cpp
template <typename TPointer>
TPointer mem_copy(const TPointer sb, const TPointer se, TPointer db) ETL_NOEXCEPT
```
Template wrapper for `memcpy`.  
Copies all of the bytes in the source range to the destination range.  
Type must be trivially copyable.  
`sb` Pointer to source begin.  
`se` Pointer to source end.  
`db` Pointer to destination begin.  
Returns a pointer to the destination.  

---

```cpp
template <typename TPointer>
TPointer mem_copy(const TPointer sb, size_t n, TPointer db) ETL_NOEXCEPT
```
Template wrapper for `memcpy`.  
Copies all of the bytes in the source range to the destination range.  
Type must be trivially copyable.  
`sb` Pointer to source begin.  
`n`  Source length.  
`db` Pointer to destination begin.  
Returns a pointer to the destination.  

## mem_move
From: `20.26.0`  

```cpp
template <typename TPointer>
TPointer mem_move(const TPointer sb, const TPointer se, TPointer db) ETL_NOEXCEPT
```
Template wrapper for `memmove`.  
Moves all of the bytes in the source range to the destination range.  
Type must be trivially copyable.  
`sb` Pointer to source begin.  
`se` Pointer to source end.  
`db` Pointer to destination begin.  
Returns a pointer to the destination.  

---

```cpp
template <typename TPointer>
TPointer mem_move(const TPointer sb, size_t n, TPointer db) ETL_NOEXCEPT
```
Template wrapper for `memmove`.  
Moves all of the bytes in the source range to the destination range.  
Type must be trivially copyable.  
`sb` Pointer to source begin.  
`n`  Source length.  
`db` Pointer to destination begin.  
Returns a pointer to the destination.  

## mem_compare
From: `20.26.0`  

```cpp
template <typename TPointer>
TPointer mem_compare(const TPointer sb, const TPointer se, TPointer db) ETL_NOEXCEPT
```
Template wrapper for `memcmp`.  
Searches all of the bytes in the range for value.  
Type must be trivially copyable.  
`sb` Pointer to source begin.  
`se` Pointer to source end.  
`db` Pointer to destination begin.  
Returns `<0`, `0`, `>0`. See documentation for `memcmp`.  

---

```cpp
template <typename TPointer>
TPointer mem_compare(const TPointer sb, size_t n, TPointer db) ETL_NOEXCEPT
```
Template wrapper for `memcmp`.  
Type must be trivially copyable.  
`sb` Pointer to source begin.  
`n`  Source length.  
`db` Pointer to destination begin.  
Returns `<0`, `0`, `>0`. See documentation for `memcmp`.  

## mem_set
From: `20.26.0`  

```cpp
template <typename TPointer, typename T>
TPointer mem_set(const TPointer db, const TPointer de, T value) ETL_NOEXCEPT
```
Template wrapper for `memset`.  
Sets all of the bytes in the range to `value`.  
Type must be trivially copyable.  
`db`    Pointer to destination begin.  
`de`    Pointer to destination end.  
`value` The value to write to the memory. This value is cast to `char`.  
Returns a pointer to the destination.  

---

```cpp
template <typename TPointer, typename T>
TPointer mem_set(const TPointer db, size_t n, T value) ETL_NOEXCEPT
```
Template wrapper for `memset`.  
Sets all of the bytes in the range to `value`.  
Type must be trivially copyable.  
`db`    Pointer to destination begin.  
`n`     Destination length.  
`value` The value to write to the memory. This value is cast to `char`.  
Returns a pointer to the destination.  

## mem_char
From: `20.26.0`  

```cpp
template <typename TPointer, typename T>
TPointer mem_char(const TPointer sb, const TPointer se, T value) ETL_NOEXCEPT
```
Template wrapper for `memchr`.  
Searches all of the bytes in the range for `value`.  
Type must be trivially copyable.  
`db`    Pointer to source begin.  
`de`    Pointer to source begin.  
`value` The value to search for. This value is cast to `char`.  
Returns a pointer to the character or se if not found.  

---

```cpp
template <typename TPointer, typename T>
TPointer mem_char(const TPointer sb, size_t n, T value) ETL_NOEXCEPT
```
Template wrapper for `memchr`.  
Searches all of the bytes in the range for `value`.  
Type must be trivially copyable.  
`db`    Pointer to destination begin.  
`n`     Source length.  
`value` The value to search for. This value is cast to `char`.  
Returns a pointer to the character or `sb + n` if not found.  
