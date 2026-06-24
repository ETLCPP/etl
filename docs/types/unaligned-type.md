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
template <int Endian_Other>
unaligned_type(const unaligned_type<T, Endian_Other>& other)
```
**Description**  
Constructs from another unaligned_type.
The endianness is converted, if necessary.

## Member types
```cpp
using pointer                = char*;
using const_pointer          = const char*;
using iterator               = char*;
using const_iterator         = const char*;
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
char& operator[](int i)
```
**Description**  
Index operator.

---

```cpp
ETL_CONSTEXPR14 const char& operator[](int i) const
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
