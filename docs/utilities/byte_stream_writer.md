---
title: "byte_stream_writer"
---

{{< callout type="info">}}
  Header: `byte_stream_writer.h`  
  Since: `20.17.0`  
{{< /callout >}}

A binary streaming utility that allows boolean, integral and floating point values to be written to an array of char or unsigned char. Values may be stored in the byte stream in big or little endian format. This is specified in the constructor.

If a callback delegate is set, then this will be called after each write. The stream buffer indexes will be reset after every write, allowing a small buffer to be used.  
Since: `20.29.0`  

```cpp
class byte_stream_writer
```

>Note for floating point  
>Be aware that floating point representations can change between platforms.  
>For example, `long double` is 8 bytes for MS compilers and 12 for GCC.

## Types

```cpp
iterator                 char*
const_iterator           const char*
callback_parameter_type  etl::span<char>
callback_type            etl::delegate<void(callback_parameter_type)>
```

## Constructors
```cpp
byte_stream_writer(etl::span<char> span, 
                   etl::endian stream_endianness, 
                   callback_type callback = callback_type())
```
**Description**  
Construct from span.
Callback parameter is available from `20.29.0`

---

```cpp
byte_stream_writer(etl::span<unsigned char> span, 
                   etl::endian stream_endianness, 
                   callback_type callback = callback_type())
```
**Description**  
Construct from span.

---

```cpp
byte_stream_writer(char* begin, 
                   char* end, 
                   etl::endian stream_endianness, 
                   callback_type callback = callback_type())
```
**Description**  
Construct from range.

---

```cpp
byte_stream_writer(char* begin, 
                   size_t length, 
                   etl::endian stream_endianness, 
                   callback_type callback = callback_type())
```
**Description**  
Construct from begin and length.

---

```cpp
template <typename T, size_t Size>
byte_stream_writer(T(&begin)[Size], 
                   etl::endian stream_endianness, 
                   callback_type callback = callback_type())
```
**Description**  
Construct from array.

## Write
**Checked**  
The following function will only write to the buffer if there is enough room to do so.

```cpp
bool write(bool value)
```
**Description**  
Write a boolean value to the stream.
Returns `true` if successful, otherwise `false`.

---

```cpp
template <typename T>
bool write(T value)
```
**Description**  
Write an integral or floating point value to the stream.
Returns `true` if successful, otherwise `false`.

---

```cpp
template <typename T>
bool write(const etl::span<T>& range)
```
**Description**  
Write a range of integral or floating point values to the stream.
Returns `true` if successful, otherwise `false`.

---

```cpp
template <typename T>
bool write(const T* start, size_t length)
```
**Description**  
Write a range of integral or floating point values to the stream.
Returns `true` if successful, otherwise `false`.

## Unchecked
Since: `20.18.0`  
The following functions will always write to the buffer. They do not check for free space.

```cpp
bool write_unchecked(bool value)
```
**Description**  
Write a boolean value to the stream.
Returns `true` if successful, otherwise `false`.

---

```cpp
template <typename T>
void  write_unchecked(T value)
```
**Description**  
Write an integral or floating point value to the stream.

---

```cpp
template <typename T>
void write_unchecked(const etl::span<T>& range)
```
**Description**  
Write a range of integral or floating point values to the stream.

---

```cpp
template <typename T>
void write_unchecked(const T* start, size_t length)
```
**Description**  
Write a range of integral or floating point values to the stream.

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
Asserts `etl::byte_stream_overflow` and returns `false` if the skip size was not possible.  
Since: `20.29.0`

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

## Status

```cpp
bool full() const
```
**Description**  
Returns `true` if the byte stream writer has reached the end.

---

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
size_t capacity() const
```
**Description**  
Returns the maximum number of bytes in the stream.

---

```cpp
template <typename T>
size_t available() const
```
**Description**  
The number of `T` left in the stream.

---

```cpp
size_t available_bytes() const
```
**Description**  
The number of bytes left in the stream.

---

```cpp
etl::endian get_endianness() const
```
**Description**  
Gets the endianness of the stream.

## Callback

```cpp
void set_callback(callback_type callback_)
```
**Description**  
Sets the delegate that is called after every write.  
Since: `20.29.0`

---

```cpp
callback_type get_callback() const
```
**Description**  
Gets the delegate that is called after every write.

## Non-member functions

Default implementation of the write functions.
For integral and floating point types only.
Specialise these to support custom types.

```cpp
template <typename T> 20.18.0
T write_unchecked(etl::byte_stream_writer& stream)
```

---

```cpp
template <typename T>
etl::optional<T> write(etl::byte_stream_writer& stream)
```

## Examples

**Using a large buffer and processing the bytes at the end**  

```cpp
void WriteBytesToConsole(etl::span<char> sp)
{
  for (auto itr = sp.begin(); itr != sp.end(); ++itr)
  {
    putc(*itr);
  }
}

std::array<int32_t, 4> data = { int32_t(0x00000001), int32_t(0xA55AA55A), 
                                int32_t(0x5AA55AA5), int32_t(0xFFFFFFFF) };

std::array<char, 4 * sizeof(int32_t)> storage;
etl::byte_stream_writer writer(storage.data(), storage.size(), etl::endian::big);

for (auto i : data)
{
  writer.write(i);
}

etl::span<char> sp = writer.used_data();
WriteBytesToConsole(sp);
```cpp

**Using a small buffer and processing the bytes after every write**  

void WriteBytesToConsole(etl::span<char> sp)
{
  for (auto itr = sp.begin(); itr != sp.end(); ++itr)
  {
    putc(*itr);
  }
}

std::array<int32_t, 4> data = { int32_t(0x00000001), int32_t(0xA55AA55A), 
                                int32_t(0x5AA55AA5), int32_t(0xFFFFFFFF) };

std::array<char, sizeof(int32_t)> storage;
etl::byte_stream_writer writer(storage.data(), storage.size(), etl::endian::big);

for (auto i : data)
{
  writer.write(i);
  etl::span<char> sp = writer.used_data();
  WriteBytesToConsole(sp);
  writer.restart();
}

**Using a small buffer and processing the bytes after every write with a callback**  

```cpp
void WriteBytesToConsole(etl::byte_stream_writer::callback_parameter_type sp)
{
  for (auto itr = sp.begin(); itr != sp.end(); ++itr)
  {
    putc(*itr);
  }
}

std::array<int32_t, 4> data = { int32_t(0x00000001), int32_t(0xA55AA55A), 
                                int32_t(0x5AA55AA5), int32_t(0xFFFFFFFF) };

std::array<char, sizeof(int32_t)> storage;

auto callback =
  etl::delegate<void<etl::byte_stream_writer::callback_parameter_type>>::create<WriteBytesToConsole>();

etl::byte_stream_writer writer(storage.data(), storage.size(), etl::endian::big, callback);

for (auto i : data)
{
  writer.write(i); // Sends the bytes to WriteBytesToConsole
}
```cpp
