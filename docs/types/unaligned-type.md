---
title: "unaligned_type"
---

{{< callout type="info">}}
  Header: `unaligned_type.h`  
  From: `20.23.0`  
{{< /callout >}}

A wrapper for fundamental types around unaligned internal storage.  
Allows big and little endian storage.  

Marked as `packed` for MSVC, GCC amd Clang.  

```cpp
template <typename T, int Endian>
class unaligned_type
```

Where `Endian` is `etl::endian::big` or `etl::endian::little`.

---

**For C++11 or above**  
```cpp
template <typename T, int Endian>
using unaligned_type_t = typename etl::unaligned_type<T, Endian>::type;
```

---

**For C++17 or above**  
```cpp
template <typename T, int Endian>
constexpr size_t unaligned_type_v = etl::unaligned_type<T, Endian>::Size;
```

---

The following types are predefined.  

## Host order
Only defined if `ETL_ENDIANNESS_IS_CONSTEXPR`.
```cpp
host_char_t
host_schar_t
host_uchar_t
host_short_t
host_ushort_t
host_int_t
host_uint_t
host_long_t
host_ulong_t
host_long_long_t
host_ulong_long_t
host_int8_t       ETL_USING_8BIT_TYPES
host_uint8_t      ETL_USING_8BIT_TYPES
host_int16_t
host_uint16_t
host_int32_t
host_uint32_t
host_int64_t      ETL_USING_64BIT_TYPES
host_uint64_t     ETL_USING_64BIT_TYPES
host_float_t
host_double_t
host_long_double_t
```
## Little Endian
```cpp
le_char_t
le_schar_t
le_uchar_t
le_short_t
le_ushort_t
le_int_t
le_uint_t
le_long_t
le_ulong_t
le_long_long_t
le_ulong_long_t
le_int8_t       ETL_USING_8BIT_TYPES
le_uint8_t      ETL_USING_8BIT_TYPES
le_int16_t
le_uint16_t
le_int32_t
le_uint32_t
le_int64_t      ETL_USING_64BIT_TYPES
le_uint64_t     ETL_USING_64BIT_TYPES
le_float_t
le_double_t
le_long_double_t
```
## Big Endian
```cpp
be_char_t
be_schar_t
be_uchar_t
be_short_t
be_ushort_t
be_int_t
be_uint_t
be_long_t
be_ulong_t
be_long_long_t
be_ulong_long_t
be_int8_t       ETL_USING_8BIT_TYPES
be_uint8_t      ETL_USING_8BIT_TYPES
be_int16_t
be_uint16_t
be_int32_t
be_uint32_t
be_int64_t      ETL_USING_64BIT_TYPES
be_uint64_t     ETL_USING_64BIT_TYPES
be_float_t
be_double_t
be_long_double_t
```
## Network Order
Synonym for Big Endian
```cpp
using net_char_t        = be_char_t
using net_schar_t       = be_schar_t
using net_uchar_t       = be_uchar_t
using net_short_t       = be_short_t
using net_ushort_t      = be_ushort_t
using net_int_t         = be_int_t
using be_uint_t         = net_uint_t
using net_long_t        = be_long_t       
using net_ulong_t       = be_ulong_t
using net_long_long_t   = be_long_long_t  
using net_ulong_long_t  = be_ulong_long_t
using net_int8_t        = be_int8_t       ETL_USING_8BIT_TYPES
using net_uint8_t       = be_uint8_t      ETL_USING_8BIT_TYPES
using net_int16_t       = be_int16_t
using net_uint16_t      = be_uint16_t
using net_int32_t       = be_int32_t       
using net_uint32_t      = be_uint32_t
using net_int64_t       = be_int64_t      ETL_USING_64BIT_TYPES
using net_uint64_t      = be_uint64_t     ETL_USING_64BIT_TYPES
using net_float_t       = be_float_t
using net_double_t      = be_double_t
using net_long_double_t = be_long_double_t
```

## Constants
`int Endian`  
&emsp;The endianness of the type.  
`size_t Size`  
&emsp;The size, in `char`, of the type.

## Constructors

```cpp
ETL_CONSTEXPR unaligned_type()
```
**Description**  
Constructs an uninitialised `unaligned_type`.

---

```cpp
unaligned_type(T value)
```
**Description**  
Constructs with the supplied value.

---

```cpp
unaligned_type(const void* address)
```
**Description**  
Constructs from the `Size` bytes at the address.  
From: `20.39.5`

---

```cpp
unaligned_type(const void* address, size_t buffer_size)
```
**Description**  
Constructs from the `Size` bytes at the address.  
`buffer_size` must be greater than or equal to `Size`. This is a precondition,
checked by `ETL_ASSERT`. If checks are disabled then a smaller buffer size
results in a read beyond the end of the buffer.  
From: `20.39.5`

---

```cpp
ETL_CONSTEXPR14 unaligned_type(const unsigned char* address)
```
**Description**  
Constructs from the `Size` bytes at the address.  
Unlike the `const void*` overload, this requires no `reinterpret_cast` and so
may be used in a constexpr context.  
From: `20.48.1`

---

```cpp
ETL_CONSTEXPR14 unaligned_type(const unsigned char* address, size_t buffer_size)
```
**Description**  
Constructs from the `Size` bytes at the address.  
`buffer_size` must be greater than or equal to `Size`. This is a precondition,
checked by `ETL_ASSERT`. If checks are disabled then a smaller buffer size
results in a read beyond the end of the buffer.  
From: `20.48.1`

---

```cpp
template <int Endian_Other>
unaligned_type(const unaligned_type<T, Endian_Other>& other)
```
**Description**  
Constructs from another unaligned_type.
The endianness is converted, if necessary.

## Member types
```cpp
using storage_type           = unsigned char;
using pointer                = unsigned char*;
using const_pointer          = const unsigned char*;
using iterator               = unsigned char*;
using const_iterator         = const unsigned char*;
using reverse_iterator       = etl::reverse_iterator<iterator>;
using const_reverse_iterator = etl::reverse_iterator<const_iterator>;
```

## Member functions
```cpp
pointer data()
```
**Description**  
Pointer to the beginning of the storage.

---

```cpp
ETL_CONSTEXPR14 const_pointer data() const
```
**Description**  
Const pointer to the beginning of the storage.

---

```cpp
ETL_CONSTEXPR14 size_t size() const
```
**Description**  
Size of the storage.

---

```cpp
iterator begin()
```
**Description**  
Iterator to the beginning of the storage.

---

```cpp
ETL_CONSTEXPR14 const_iterator begin() const
```
**Description**  
Const iterator to the beginning of the storage.

---

```cpp
ETL_CONSTEXPR14 const_iterator cbegin() const
```
**Description**  
Const iterator to the beginning of the storage.

---

```cpp
reverse_iterator rbegin()
```
**Description**  
Reverse iterator to the beginning of the storage.

---

```cpp
ETL_CONSTEXPR14 const_reverse_iterator rbegin() const
```
**Description**  
Const reverse iterator to the beginning of the storage.

---

```cpp
ETL_CONSTEXPR14 const_reverse_iterator crbegin() const
```
**Description**  
Const reverse iterator to the beginning of the storage.

---

```cpp
iterator end()
```
**Description**  
Iterator to the end of the storage.

---

```cpp
ETL_CONSTEXPR14 const_iterator end() const
```
**Description**  
Const iterator to the end of the storage.

---

```cpp
ETL_CONSTEXPR14 const_iterator cend() const
```
**Description**  
Const iterator to the end of the storage.

---

```cpp
reverse_iterator rend()
```
**Description**  
Reverse iterator to the end of the storage.

---

```cpp
ETL_CONSTEXPR14 const_reverse_iterator rend() const
```
**Description**  
Const reverse iterator to the end of the storage.

---

```cpp
ETL_CONSTEXPR14 const_reverse_iterator crend() const
```
**Description**  
Const reverse iterator to the end of the storage.

---

```cpp
storage_type& operator[](int i)
```
**Description**  
Index operator.

---

```cpp
ETL_CONSTEXPR14 const storage_type& operator[](int i) const
```
**Description**  
Const index operator.

---

```cpp
ETL_CONSTEXPR14 unaligned_type& operator =(T value)
```
**Description**  
Assignment operator.

---

```cpp
ETL_CONSTEXPR14 operator T() const
```
**Description**  
Conversion operator.

---

```cpp
ETL_CONSTEXPR14 T value() const
```
**Description**  
Gets the value.

---

```cpp
static ETL_CONSTEXPR14 T value_from(const_pointer address)
```
**Description**  
Gets the value of the `Size` bytes at the address.  
Unlike construction from an address, the bytes are not copied to storage first.

---

```cpp
template <int Endian_Other>
ETL_CONSTEXPR14 unaligned_type& operator =(const unaligned_type<T, Endian_Other>& other)
```
**Description**  
Assignment operator from other endianness.

---

```cpp
ETL_CONSTEXPR14 bool operator ==(const unaligned_type& lhs, const unaligned_type& rhs)
```
**Description**  
Equality operator.  
Removed: `20.39.3`

---

```cpp
ETL_CONSTEXPR14 bool operator ==(const unaligned_type& lhs, T rhs)
```
**Description**  
Equality operator.  
Removed: `20.39.3`

---

```cpp
ETL_CONSTEXPR14 bool operator ==(T lhs, const unaligned_type& rhs)
```
**Description**  
Equality operator.  
Removed: `20.39.3`

---

```cpp
ETL_CONSTEXPR14 bool operator !=(const unaligned_type& lhs, T rhs)
```
**Description**  
Inequality operator.  
Removed: `20.39.3`

---

```cpp
ETL_CONSTEXPR14 bool operator !=(const unaligned_type& lhs, const unaligned_type& rhs)
```
**Description**  
Inequality operator.  
Removed: `20.39.3`

---

```cpp
ETL_CONSTEXPR14 bool operator !=(T lhs, const unaligned_type& rhs)
```
**Description**  
Inequality operator.  
Removed: `20.39.3`

---

# Arbitrary width integers

{{< callout type="info">}}
  From: `20.50.0`  
{{< /callout >}}

`etl::unaligned_type` is templated on a fundamental type, so it can only store
integers whose width is that of a standard integer type. For protocols that use
odd widths, such as 24 or 48 bit integers, the following types are templated on
the **number of bytes of storage** instead.

```cpp
template <size_t Size_, int Endian_>
class unaligned_uint_type

template <size_t Size_, int Endian_>
class unaligned_int_type
```

Where `Size_` is the number of bytes of storage and `Endian_` is
`etl::endian::big` or `etl::endian::little`.

`unaligned_uint_type` stores an unsigned integer.  
`unaligned_int_type` stores a signed, two's complement integer, which is sign
extended when read.

The value is interfaced to via `value_type`: the smallest standard integer type
that can hold `Size_` bytes. For example, `unaligned_uint_type<3U, ...>` has a
`value_type` of `uint32_t`.

Values that do not fit in `Size_` bytes are truncated, in the same way as a
narrowing `static_cast`.

Marked as `packed` for MSVC, GCC and Clang.

## Constants
`int Endian`  
&emsp;The endianness of the type.  
`size_t Size`  
&emsp;The size, in bytes, of the type.  
`value_type Min_Value`  
&emsp;The lowest value that may be stored.  
`value_type Max_Value`  
&emsp;The highest value that may be stored.

## Predefined types

The 40, 48 and 56 bit types are only defined if `ETL_USING_64BIT_TYPES`.  
The host order types are only defined if `ETL_HAS_CONSTEXPR_ENDIANNESS`.

### Little Endian
```cpp
le_uint24_t
le_int24_t
le_uint40_t     ETL_USING_64BIT_TYPES
le_int40_t      ETL_USING_64BIT_TYPES
le_uint48_t     ETL_USING_64BIT_TYPES
le_int48_t      ETL_USING_64BIT_TYPES
le_uint56_t     ETL_USING_64BIT_TYPES
le_int56_t      ETL_USING_64BIT_TYPES
```

### Big Endian
```cpp
be_uint24_t
be_int24_t
be_uint40_t     ETL_USING_64BIT_TYPES
be_int40_t      ETL_USING_64BIT_TYPES
be_uint48_t     ETL_USING_64BIT_TYPES
be_int48_t      ETL_USING_64BIT_TYPES
be_uint56_t     ETL_USING_64BIT_TYPES
be_int56_t      ETL_USING_64BIT_TYPES
```

### Network Order
Synonyms for Big Endian.
```cpp
using net_uint24_t = be_uint24_t
using net_int24_t  = be_int24_t
using net_uint40_t = be_uint40_t
using net_int40_t  = be_int40_t
using net_uint48_t = be_uint48_t
using net_int48_t  = be_int48_t
using net_uint56_t = be_uint56_t
using net_int56_t  = be_int56_t
```

### Host order
```cpp
host_uint24_t
host_int24_t
host_uint40_t
host_int40_t
host_uint48_t
host_int48_t
host_uint56_t
host_int56_t
```

## Constructors

```cpp
unaligned_uint_type()
unaligned_int_type()
```
**Description**  
Constructs an uninitialised object.

---

```cpp
ETL_CONSTEXPR14 unaligned_uint_type(value_type value)
ETL_CONSTEXPR14 unaligned_int_type(value_type value)
```
**Description**  
Constructs with the supplied value.  
Values that do not fit in `Size` bytes are truncated.

---

```cpp
unaligned_uint_type(const void* address)
unaligned_int_type(const void* address)
```
**Description**  
Constructs from the `Size` bytes at the address.

---

```cpp
unaligned_uint_type(const void* address, size_t buffer_size)
unaligned_int_type(const void* address, size_t buffer_size)
```
**Description**  
Constructs from the `Size` bytes at the address.  
`buffer_size` must be greater than or equal to `Size`. This is a precondition,
checked by `ETL_ASSERT`. If checks are disabled then a smaller buffer size
results in a read beyond the end of the buffer.

---

```cpp
ETL_CONSTEXPR14 unaligned_uint_type(const unsigned char* address)
ETL_CONSTEXPR14 unaligned_int_type(const unsigned char* address)
```
**Description**  
Constructs from the `Size` bytes at the address.  
Unlike the `const void*` overload, this requires no `reinterpret_cast` and so
may be used in a constexpr context.

---

```cpp
ETL_CONSTEXPR14 unaligned_uint_type(const unsigned char* address, size_t buffer_size)
ETL_CONSTEXPR14 unaligned_int_type(const unsigned char* address, size_t buffer_size)
```
**Description**  
Constructs from the `Size` bytes at the address.  
`buffer_size` must be greater than or equal to `Size`. This is a precondition,
checked by `ETL_ASSERT`.

---

```cpp
template <int Endian_Other>
unaligned_uint_type(const unaligned_uint_type<Size, Endian_Other>& other)
```
**Description**  
Constructs from another type of the same size.  
The endianness is converted, if necessary.

## Member functions

The storage interface (`data()`, `size()`, the iterators and `operator[]`) is
the same as that of `etl::unaligned_type`.

```cpp
ETL_CONSTEXPR14 unaligned_uint_type& operator =(value_type value)
```
**Description**  
Assignment operator.

---

```cpp
ETL_CONSTEXPR14 operator value_type() const
```
**Description**  
Conversion operator.  
For `unaligned_int_type` the stored value is sign extended.

---

```cpp
ETL_CONSTEXPR14 value_type value() const
```
**Description**  
Gets the value.  
For `unaligned_int_type` the stored value is sign extended.

---

```cpp
static ETL_CONSTEXPR14 value_type value_from(const_pointer address)
```
**Description**  
Gets the value of the `Size` bytes at the address.  
Unlike construction from an address, the bytes are not copied to storage first.

---

```cpp
template <int Endian_Other>
ETL_CONSTEXPR14 unaligned_uint_type& operator =(const unaligned_uint_type<Size, Endian_Other>& other)
```
**Description**  
Assignment operator from other endianness.

---

# Arbitrary width integers with an external buffer

{{< callout type="info">}}
  From: `20.50.0`  
{{< /callout >}}

```cpp
template <size_t Size_, int Endian_>
class unaligned_uint_type_ext

template <size_t Size_, int Endian_>
class unaligned_int_type_ext
```

These are the equivalents of `unaligned_uint_type` and `unaligned_int_type`
that do not own their storage, but reference an externally supplied buffer.
They have the same `value_type`, constants and value interface.

The default constructor is deleted: the storage must always be supplied.

### Predefined types
As for the internal storage versions, but with an `_ext_t` suffix.
```cpp
le_uint24_ext_t   be_uint24_ext_t   net_uint24_ext_t   host_uint24_ext_t
le_int24_ext_t    be_int24_ext_t    net_int24_ext_t    host_int24_ext_t
le_uint40_ext_t   be_uint40_ext_t   net_uint40_ext_t   host_uint40_ext_t
le_int40_ext_t    be_int40_ext_t    net_int40_ext_t    host_int40_ext_t
le_uint48_ext_t   be_uint48_ext_t   net_uint48_ext_t   host_uint48_ext_t
le_int48_ext_t    be_int48_ext_t    net_int48_ext_t    host_int48_ext_t
le_uint56_ext_t   be_uint56_ext_t   net_uint56_ext_t   host_uint56_ext_t
le_int56_ext_t    be_int56_ext_t    net_int56_ext_t    host_int56_ext_t
```

## Constructors

```cpp
unaligned_uint_type_ext() ETL_DELETE
```
**Description**  
Deleted. The storage must always be supplied.

---

```cpp
unaligned_uint_type_ext(pointer storage)
```
**Description**  
Constructs, referencing the supplied storage.  
The storage is not modified.

---

```cpp
unaligned_uint_type_ext(value_type value, pointer storage)
```
**Description**  
Constructs, referencing the supplied storage, and writes the value to it.

---

```cpp
template <int Endian_Other>
unaligned_uint_type_ext(const unaligned_uint_type_ext<Size, Endian_Other>& other, pointer storage)
```
**Description**  
Constructs, referencing the supplied storage, and copies the value of `other`
to it, converting the endianness if necessary.  
**Note**  
The storage of `other` must either be the same storage as that supplied, or
completely separate. Partially overlapping storage is undefined behaviour.

## Member functions

```cpp
void set_storage(pointer storage)
```
**Description**  
Sets the storage for the type.  
The storage is not modified.

---

```cpp
template <int Endian_Other>
unaligned_uint_type_ext& operator =(const unaligned_uint_type_ext<Size, Endian_Other>& other)
```
**Description**  
Assignment operator from other endianness.  
**Note**  
The storage of `other` must either be the same storage as this object's, or
completely separate. Partially overlapping storage is undefined behaviour.
