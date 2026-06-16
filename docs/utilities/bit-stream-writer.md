---
title: "bit_stream_writer"
---

{{< callout type="info">}}
  Header: `bit_stream_writer.h`  
  Since: `TBC`  
{{< /callout >}}

A binary streaming utility that allows values to be written to an array of char or unsigned char using custom bits widths for efficient packing. Values may be streamed in either msb or lsb format.

Write functions come in both checked and unchecked forms.
- Unchecked writes return `void`.
- Checked writes assert that there is enough space to store the value, and return a `bool` indicating success or failure.

A callback delegate can be assigned that will be called with a span of filled bytes.
If the callback is used then the stream is reset to empty or the last unfilled byte after each callback. This allows a very small buffer to be assigned.

## Storage
- `bool`  
Stored as 1 bit.  

- Integrals  
By default, stored using full bit width, otherwise uses the specified width.  

- Custom types  
The user may create specializations of the non-member etl::write functions to stream custom types. Specialisations must be defined in the `etl` namespace.  

## Types

```cpp
value_type              char
iterator                value_type*
const_iterator          const value_type*   
callback_parameter_type etl::span<value_type>
callback_type           etl::delegate<void(callback_parameter_type)>
```

## Member functions
```cpp
bit_stream_writer(etl::span<char> span, etl::endian stream_endianness, callback_type callback = callback_type())
```
**Description**  
Construct from span.  
**Parameters**  
`span` A char span of the read buffer.  
`stream_endianness` The endianness of the stream. `etl::endian::little` or `etl::endian::big`.   
`callback` An optional callback.

---

```cpp
bit_stream_writer(etl::span<unsigned char> span, etl::endian stream_endianness, callback_type callback = callback_type())
```
**Description**  
Construct from span.  
**Parameters**  
`span` A char span of the read buffer.  
`stream_endianness` The endianness of the stream. etl::endian::little or etl::endian::big.  
`callback` An optional callback.

---

```cpp
bit_stream_writer(void* begin, void* end, etl::endian stream_endianness, callback_type callback = callback_type())
```
**Description**  
Construct from range.  
**Parameters**  
`begin` A pointer to the beginning of the read buffer.  
`end` A pointer to the beginning of the read buffer.  
`stream_endianness` The endianness of the stream. `etl::endian::little` or `etl::endian::big`.  
`callback` An optional callback.

---

```cpp
bit_stream_writer(void* begin, size_t length_chars, etl::endian stream_endianness, callback_type callback = callback_type())
```
**Description**  
Construct from begin and length.  
**Parameters**  
`begin` A pointer to the beginning of the read buffer.  
`length` The length, in char, of the read buffer.  
`stream_endianness` The endianness of the stream. `etl::endian::little` or `etl::endian::big`.  

---

```cpp
void restart()
```
**Description**  
Sets the indexes back to the beginning of the stream.

---

```cpp
size_t capacity_bytes() const
```
**Description**  
Returns the maximum capacity in bytes.

---

```cpp
size_t capacity_bits() const
```
**Description**  
Returns the maximum capacity in bits.

---

```cpp
bool empty() const
```
**Description**  
Returns true if the bitsteam indexes have been reset

---

```cpp
bool full() const
```
**Description**  
Returns true if the bitsteam indexes have reached the end

---

```cpp
void write_unchecked(bool value)
```
**Description**  
Writes a boolean to the stream.

---

```cpp
bool write(bool value)
```
**Description**  
Writes a boolean to the stream.

---

```cpp
template <typename T>
void write_unchecked(T value, uint_least8_t nbits = CHAR_BIT * sizeof(T))
```
**Description**  
Enabled for integral types.

---

```cpp
template <typename T>
bool write(T value, uint_least8_t nbits = CHAR_BIT * sizeof(T))
```
**Description**  
Enabled for integral types.

---

```cpp
bool skip(size_t nbits)
```
**Description**  
Skip forward in the stream by nbits.  
If there are not enough bits remaining in the stream it assert an etl::bit_stream_overflow and return false.

---

```cpp
size_t size_bytes() const
```
**Description**  
Returns the number of bytes used in the stream.

---

```cpp
size_t size_bits() const
```
**Description**  
Returns the number of bits used in the stream.

---

```cpp
template <size_t Nbits>
size_t available() const
```
**Description**  
The number of multiples of Nbits available in the stream.  
Compile time.

---

```cpp
template <typename T>
size_t available() const
```
**Description**  
The number of T available in the stream.  
Compile time.

---

```cpp
size_t available(size_t nbits) const
```
**Description**  
The number of 'bit width' available in the stream.  
Run time.

---

```cpp
size_t available_bits() const
```
**Description**  
The number of bits left in the stream.

---

```cpp
iterator begin()
```
**Description**  
Returns start of the stream.

---

```cpp
const_iterator begin() const
```
**Description**  
Returns start of the stream.

---

```cpp
const_iterator cbegin() const
```
**Description**  
Returns start of the stream.

---

```cpp
iterator end()
```
**Description**  
Returns end of the stream.

---

```cpp
const_iterator end() const
```
**Description**  
Returns end of the stream.

---

```cpp
const_iterator cend() const
```
**Description**  
Returns end of the stream.

---

```cpp
etl::span<char> used_data()
```
**Description**  
Returns a span of the used portion of the stream.

---

```cpp
etl::span<const char> used_data() const
```
**Description**  
Returns a span of the used portion of the stream.

---

```cpp
etl::span<char> data()
```
**Description**  
Returns a span of whole the stream.

---

```cpp
etl::span<const char> data() const
```
**Description**  
Returns a span of whole the stream.

---

```cpp
void flush()
```
**Description**  
Flush the last byte, if partially filled, to the callback.
This is only required when a callback delegate has been set.

---

```cpp
void set_callback(callback_type callback_)
```
**Description**  
Sets the delegate to call after every write.

---

```cpp
callback_type get_callback() const
```
**Description**  
Gets the function to call afer every write.

---

## Non-member functions

```cpp
void write_unchecked(etl::bit_stream_writer& stream, bool value)
```
**Description**  
Implementation of the write unchecked function.
For `bool` types only.

---

```cpp
bool write(etl::bit_stream_writer& stream, bool value)
```
**Description**  
Implementation of the write function.
For `bool` types only.

---

```cpp
template <typename T>
void write_unchecked(etl::bit_stream_writer& stream, const T& value, uint_least8_t nbits = CHAR_BIT * sizeof(T))
```
**Description**  
  Default implementation of the write function.
  For integral types only (but not bool).
  Overload this to support custom types.

---

```cpp
template <typename T>
bool write(etl::bit_stream_writer& stream, const T& value, uint_least8_t nbits = CHAR_BIT * sizeof(T))
```
**Description**  
Default implementation of the write function.
For integral types only (but not bool).
Overload this to support custom types.

---

By specialisation of the templates, the user may create writers for custom types.

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
  void write_unchecked<CustomType>(etl::bit_stream_reader& stream)
  {
    stream.write_unchecked<char, 7U>();
    stream.write_unchecked<short , 11U>();
    stream.write_unchecked<int32_t, 25U>();
  }
}

std::array<char, 100U> storage; // Assume the buffer gets filled with bit stream data.
etl::bit_stream_reader bit_stream(storage.data(), storage.size());

// Write unchecked values to the stream.
etl::write_unchecked<char>(bit_stream, 6U);
etl::write_unchecked<unsigned short>(bit_stream, 13U);
etl::write_unchecked<Custom>(bit_stream);
etl::write_unchecked<>(bit_stream, 23U);
```
