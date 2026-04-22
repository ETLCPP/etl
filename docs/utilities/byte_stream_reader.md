---
title: "byte_stream_reader"
---

{{< callout type="info">}}
  Header: `byte_stream_reader.h`  
  Since: `20.17.0`  
{{< /callout >}}

A binary streaming utility that allows boolean, integral and floating point values to be read from an array of char or unsigned char. By default, values are stored in the byte stream in network order (Big Endian). This can be changed by specifying in the constructor.

```cpp
class byte_stream_reader
```

>Note for floating point: 
>Be aware that floating point representations can change between platforms.  
>For example, `long double` is 8 bytes for MS compilers and 12 for GCC.

## Types

```cpp
iterator         char*
const_iterator   const char*
```

## Constructors

```cpp
byte_stream_reader(etl::span<char> span, etl::endian stream_endianness)
```
**Description**  
Construct from span.

---

```cpp
byte_stream_reader(etl::span<const char> span, etl::endian stream_endianness)
```
**Description**  
Construct from span.

---

```cpp
byte_stream_reader(const void* begin, const void* end, etl::endian stream_endianness)
```
**Description**  
Construct from range.

---

```cpp
byte_stream_reader(const void* begin, size_t length, etl::endian stream_endianness)
```
**Description**  
Construct from begin and length.

---

```cpp
template <typename T, size_t Size>
byte_stream_reader(T(&begin)[Size], etl::endian stream_endianness)
```
**Description**  
Construct from array.

---

```cpp
template <typename T, size_t Size>
byte_stream_reader(const T(&begin)[Size], etl::endian stream_endianness)
```
**Description**  
Construct from const array.

## Read
**Checked**  
The following functions will only read from the buffer if there is enough room to do so.

```cpp
template <typename T>
etl::optional<T> read()
```
**Description**  
Read an integral value from the stream.

---

```cpp
template <typename T>
etl::optional<etl::span<T>> read(size_t n)
```
**Description**  
Read a byte range from the stream.

---

```cpp
template <typename T>
etl::optional<etl::span<const T>> read(etl::span<T> range)
```
**Description**  
Read a range of `T` from the stream.

---

```cpp
template <typename T>
etl::optional<etl::span<const T>> read(const T* start, size_t length)
```
**Description**  
Read a range of `T` from the stream.

**Unchecked**  
Since: `20.18.0`  
The following functions will always read from the buffer. They do not check for free space.

```cpp
template <typename T>
etl::optional<T> read_unchecked()
```
**Description**  
Read an integral value from the stream.

---

```cpp
template <typename T>
etl::optional<etl::span<T>> read_unchecked(size_t n)
```
**Description**  
Read a byte range from the stream.

---

```cpp
template <typename T>
etl::optional<etl::span<const T>> read_unchecked(etl::span<T> range)
```
**Description**  
Read a range of `T` from the stream.

---

```cpp
template <typename T>
etl::optional<etl::span<const T>> read_unchecked(T* start, size_t length)
```
**Description**  
Read a range of `T` from the stream.

## Status

```cpp
bool empty() const
```
**Description**  
Returns `true` if the byte stream is empty.

---

```cpp
size_t size_bytes() const
```
**Description**  
Returns the number of bytes used in the stream.

---

```cpp
template <typename T>
size_t available() const
```
**Description**  
The number of `T` left in the stream.

## Access

```cpp
void restart(size_t n = 0U)
```
**Description**  
Sets the index back to position n in the stream. Default = 0.

---

```cpp
template <typename T>
bool skip(size_t n)
```
**Description**  
Skip `n` items of `T`, if the total space is available.  
Returns `true` if the skip was possible.  
Returns `false` if the full skip size was not possible.  

---

```cpp
template <typename T>
bool skip(size_t n)
```
**Description**  
Skip `n` items of `T`, if the total space is available.  
Returns `true` if the skip was possible.  
Asserts `etl::byte_stream_overflow` and returns `false` if the skip size was not possible.  
Since: `20.29.1`

---

```cpp
const_iterator begin() const
```
**Description**  
Returns start of the stream.

---

```cpp
const_iterator end() const
```
**Description**  
Returns end of the stream.

---

```cpp
const_iterator cbegin() const
```
**Description**  
Returns start of the stream.

---

```cpp
const_iterator cend() const
```
**Description**  
Returns end of the stream.

---

```cpp
etl::span<char> used_data() const
```
**Description**  
Returns a span of the used portion of the stream.

---

```cpp
etl::span<char> free_data() const
```
**Description**  
Returns a span of the free portion of the stream.

---

```cpp
etl::span<char> data() const
```
**Description**  
Returns a span of whole the stream.

## Non-member functions
Default implementation of the read functions.  
For integral and floating point types only.  
Specialise these to support custom types.  

```cpp
template <typename T> 20.18.0
T read_unchecked(etl::byte_stream_reader& stream)
```

---

```cpp
template <typename T>
etl::optional<T> read(etl::byte_stream_reader& stream)
```
