---
title: "bit_stream_reader"
---

{{< callout type="info">}}
  Header: `bit_stream_reader.h`  
  Since: `TBC`  
{{< /callout >}}

A binary streaming utility that allows values to be read from an array of char or unsigned char using custom bits widths for efficient packing. Values may be streamed in either msb or lsb format.  

Read functions come in both checked and unchecked forms.  
- Unchecked reads return a value of the specified type.  
- Checked reads assert that there is enough space to read the value, and return an `etl::optional`.  

## Storage Size

- `bool`  
Stored as 1 bit.  

- Integrals  
By default, stored using full bit width, otherwise uses the specified width.  

- Custom types  
The user may create specializations of the non-member `etl::read` functions to stream custom types. Specialisations must be defined in the `etl` namespace.  

## Types

```cpp
value_type     char
const_iterator const char*
```

---

## Member functions

```cpp
bit_stream_reader(etl::span<char> span, etl::endian stream_endianness)
```
**Parameters**  
`span` A `char` span of the read buffer.  
`stream_endianness` The endianness of the stream. `etl::endian::little` or `etl::endian::big`.  

---

```cpp
bit_stream_reader(etl::span<unsigned char> span, etl::endian stream_endianness)
```
**Parameters**  
`span` `An unsigned char span` of the read buffer.  
`stream_endianness` The endianness of the stream. `etl::endian::little` or `etl::endian::big`.  

---

```cpp
bit_stream_reader(void* begin, void* end, etl::endian stream_endianness)
```
`begin` A pointer to the beginning of the read buffer.  
`end` A pointer to the beginning of the read buffer.  
`stream_endianness` The endianness of the stream. `etl::endian::little` or `etl::endian::big`.  

---

```cpp
bit_stream_reader(void* begin, size_t length, etl::endian stream_endianness)
```
`begin` A pointer to the beginning of the read buffer.  
`length` The length, in char, of the read buffer.  
`stream_endianness` The endianness of the stream. `etl::endian::little` or `etl::endian::big`.  

---

```cpp
void restart()
```
**Description**  
Sets the reader back to the beginning of the stream.

---

```cpp
bool read_unchecked<bool>()
```
**Description**  
Unchecked read for `bool` types.

---

```cpp
etl::optional<bool> read<bool>()
```
**Description**  
Checked read for `bool` types.

---

```cpp
template <typename T>
T read_unchecked(uint_least8_t nbits = CHAR_BIT * sizeof(T))
```
**Description**  
Unchecked read.  
Enabled for integral types.

---

```cpp
template <typename T>
etl::optional<T> read(uint_least8_t nbits = CHAR_BIT * sizeof(T))
```
**Description**  
Checked read.  
Enabled for integral types.

---

```cpp
bool skip(size_t nbits)
```
**Description**  
Skip forward in the stream by `nbits`.  
If there are not enough bits remaining in the stream it assert an `etl::bit_stream_overflow` and return `false`.

---

```cpp
size_t size_bytes() const
```
**Description**  
Returns the number of bytes in the stream buffer.

---

```cpp
size_t size_bits() const
```
**Description**  
Returns the number of bits in the stream buffer.

---

```cpp
const_iterator begin() const
const_iterator cbegin() const
```
**Description**  
Returns start of the stream buffer.

---

```cpp
const_iterator end() const
const_iterator cend() const
```
**Description**  
Returns end of the stream.

---

```cpp
etl::span<const char> data() const
```
**Description**  
Returns a span of whole the stream buffer.

---

## Non-member functions

```cpp
template <typename T>
T read_unchecked(etl::bit_stream_reader& stream)
```
Reads an unchecked type from a stream by calling `stream.read_unchecked<T>()`.

---

```cpp
template <typename T>
T read_unchecked(etl::bit_stream_reader& stream, uint_least8_t nbits)
```
**Description**  
Reads an unchecked type from a stream by calling `stream.read_unchecked<T>(nbits)`.

---

```cpp
template <typename T>
etl::optional<T> read(etl::bit_stream_reader& stream)
```
**Description**  
Reads a checked type from a stream by calling `stream.read<T>()`.

---

```cpp
template <typename T>
etl::optional<T> read(etl::bit_stream_reader& stream, uint_least8_t nbits)
```
**Description**  
Reads a checked type from a stream by calling `stream.read<T>(nbits)`.

---

```cpp
template <>
bool read_unchecked<bool>(etl::bit_stream_reader& stream)
```
Specialisation for bool.  
Reads an unchecked bool from a stream by calling `stream.read_unchecked<bool>()`.

---

```cpp
template <>
etl::optional<bool> read<bool>(etl::bit_stream_reader& stream)
```
Specialisation for bool.  
Reads a checked bool from a stream by calling stream.read<bool>()

---

By specialisation of the templates, the user may create readers for custom types.

## Example
```cpp
char c           = 26;         // 6 bits
unsigned short s = 6742;       // 13 bits
int32_t i        = 2448037;    // 23 bits

struct CustomType
{
char c           = -10;       // 7 bits
unsigned short s = 1878;      // 11 bits
int32_t i        = -10836646; // 25 bits
};

namespace etl
{
  // Specialisation for Custom.
  // Must be defined in the etl namespace.
  template <>
  etl::optional<CustomType> read_unchecked<CustomType>(etl::bit_stream_reader& stream)
  {
    char c    = stream.read_unchecked<char, 7U>();
    short s   = stream.read_unchecked<short , 11U>();
    int32_t i = stream.read_unchecked<int32_t, 25U>();

    return CustomType { c.value(), s.value(), i.value() };
  }
}

std::array<char, 100U> storage; // Assume the buffer gets filled with bit stream data.
etl::bit_stream_reader bit_stream(storage.data(), storage.size());

// Read unchecked values from the stream.
char c            = etl::read_unchecked<char>(bit_stream, 6U);
unsigned short s  = etl::read_unchecked<unsigned short>(bit_stream, 13U);
CustomType custom = etl::read_unchecked<Custom>(bit_stream);
int32_t i         = etl::read_unchecked<>(bit_stream, 23U);
```
