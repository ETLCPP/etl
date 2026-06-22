---
title: "Base64"
---

{{< callout type="info">}}
  Headers:  
  `base64.h` Common definitions  
  `base64_encoder.h` Encoder classes
  `base64_decoder.h` Decoder classes
  From: `20.38.17`
{{< /callout >}}

Encodes and decodes data to and from Base64 format.

The codecs are stream based. Data is fed in one element, or block of elements, at a time and the encoded or decoded result is written to the codec's internal output buffer. The result can be obtained in one of two ways:
- If the internal buffer is large enough to hold the entire result, it can be read directly from the codec via `begin()`/`end()` or `span()`.
- Otherwise a callback may be supplied that is called with a span of the output data each time the internal buffer becomes full, and once more with an empty span to signal that the final block has been processed.

A separate codec class is supplied for each supported Base64 variant.

## Encoder

The encoder classes are declared in `base64_encoder.h`. They all derive from `etl::ibase64_encoder`, which provides the interface described below.

| Class | Variant | Padding |
| :--- | :--- | :--- |
| `etl::base64_rfc2152_encoder<Buffer_Size>`             | RFC-2152     | No  |
| `etl::base64_rfc3501_encoder<Buffer_Size>`             | RFC-3501     | No  |
| `etl::base64_rfc4648_encoder<Buffer_Size>`             | RFC-4648     | No  |
| `etl::base64_rfc4648_padding_encoder<Buffer_Size>`     | RFC-4648     | Yes |
| `etl::base64_rfc4648_url_encoder<Buffer_Size>`         | RFC-4648-URL | No  |
| `etl::base64_rfc4648_url_padding_encoder<Buffer_Size>` | RFC-4648-URL | Yes |

### Template parameters
`Buffer_Size` The size of the internal output buffer, in characters.
Defaults to `etl::base64::Min_Encode_Buffer_Size` (4).
Must be a non-zero multiple of `etl::base64::Min_Encode_Buffer_Size`.

### Member types
```cpp
typedef etl::span<const char>                 span_type;
typedef etl::delegate<void(const span_type&)> callback_type;
```

### Constructors
```cpp
base64_rfcXXXX_encoder()
```
**Description**
Constructs an encoder with no callback. The encoded result is read from the internal output buffer.

---

```cpp
base64_rfcXXXX_encoder(callback_type callback)
```
**Description**
Constructs an encoder with a callback. The callback is called with a span of the encoded data each time the internal buffer becomes full, and with an empty span once the final block has been flushed.

### Encoding
```cpp
template <typename T>
bool encode(T value)
```
**Description**
Encodes a single 8 bit value.
Enabled if `etl::is_integral<T>` is `true` and `etl::integral_limits<T>::bits == 8`.

**Return**
`false` if the output buffer overflowed, otherwise `true`.

---

```cpp
template <typename TInputIterator>
bool encode(TInputIterator input_begin, size_t input_length)
```
**Description**  
Encodes `input_length` 8 bit values starting at `input_begin`.

**Return**
`false` if the output buffer overflowed, otherwise `true`.

---

```cpp
template <typename TInputIterator>
bool encode(TInputIterator input_begin, TInputIterator input_end)
```
**Description**
Encodes the 8 bit values in the range `input_begin` to `input_end`.

**Return**
`false` if the output buffer overflowed, otherwise `true`.

---

```cpp
template <typename TInputIterator>
bool encode_final(TInputIterator input_begin, size_t input_length)
```
**Description**
Encodes `input_length` 8 bit values starting at `input_begin`, then flushes the final block. Equivalent to `encode()` followed by `flush()`.

**Return**
`false` if the output buffer overflowed, otherwise `true`.

---

```cpp
template <typename TInputIterator>
bool encode_final(TInputIterator input_begin, TInputIterator input_end)
```
**Description**  
Encodes the 8 bit values in the range `input_begin` to `input_end`, then flushes the final block. Equivalent to `encode()` followed by `flush()`.

**Return**
`false` if the output buffer overflowed, otherwise `true`.

---

```cpp
bool flush()
```
**Description**
Encodes any remaining buffered input, including any required padding, and marks the end of the data. If a callback has been set it is called with any remaining data and then with an empty span.
Must be called once all of the input has been passed to `encode()`. Not required when `encode_final()` is used.

**Return**
`false` if the output buffer overflowed, otherwise `true`.

---

```cpp
void restart()
```
**Description**  
Resets the encoder so that it can be reused to encode a new set of data.

### Reading the result
These functions return a useful value only when no callback has been set. The internal buffer must be large enough to hold the entire encoded result.

```cpp
const char* begin() const
const char* cbegin() const
```
**Return**
A pointer to the beginning of the encoded data in the internal buffer.

---

```cpp
const char* end() const
const char* cend() const
```
**Return**
A pointer to the end of the encoded data in the internal buffer.

---

```cpp
size_t size() const
```
**Return**
The number of encoded characters currently held in the internal buffer.

---

```cpp
size_t max_size() const
```
**Return**
The maximum size of the internal output buffer (`Buffer_Size`).

---

```cpp
span_type span() const
```
**Return**
A span over the encoded data in the internal buffer.

### Status
```cpp
bool overflow() const
```
**Return**
`true` if the output buffer has overflowed.

---

```cpp
bool error() const
```
**Return**
`true` if an error has been detected. For the encoder this is equivalent to `overflow()`.

### Static functions
```cpp
static size_t safe_output_buffer_size(size_t input_length)
```
**Return**
The internal buffer size required to encode `input_length` bytes in a single pass. Use this as the `Buffer_Size` template argument when the whole result is to be read from the internal buffer.

## Decoder

The decoder classes are declared in `base64_decoder.h`. They all derive from `etl::ibase64_decoder`, which provides the interface described below.

| Class | Variant | Padding |
| :--- | :--- | :--- |
| `etl::base64_rfc2152_decoder<Buffer_Size>`             | RFC-2152     | No  |
| `etl::base64_rfc3501_decoder<Buffer_Size>`             | RFC-3501     | No  |
| `etl::base64_rfc4648_decoder<Buffer_Size>`             | RFC-4648     | No  |
| `etl::base64_rfc4648_padding_decoder<Buffer_Size>`     | RFC-4648     | Yes |
| `etl::base64_rfc4648_url_decoder<Buffer_Size>`         | RFC-4648-URL | No  |
| `etl::base64_rfc4648_url_padding_decoder<Buffer_Size>` | RFC-4648-URL | Yes |

### Template parameters
`Buffer_Size` The size of the internal output buffer, in bytes.
Defaults to `etl::base64::Min_Decode_Buffer_Size` (3).
Must be greater than or equal to `etl::base64::Min_Decode_Buffer_Size`.

### Member types
```cpp
typedef etl::span<const unsigned char>        span_type;
typedef etl::delegate<void(const span_type&)> callback_type;
```

### Constructors
```cpp
base64_rfcXXXX_decoder()
```
**Description**
Constructs a decoder with no callback. The decoded result is read from the internal output buffer.

---

```cpp
base64_rfcXXXX_decoder(callback_type callback)
```
**Description**
Constructs a decoder with a callback. The callback is called with a span of the decoded data each time the internal buffer becomes full, and with an empty span once the final block has been flushed.

### Decoding
```cpp
template <typename T>
bool decode(T value)
```
**Description**
Decodes a single Base64 character.
Enabled if `etl::is_integral<T>` is `true` and `etl::integral_limits<T>::bits == 8`.

**Return**
`false` if an error was detected, otherwise `true`.

---

```cpp
template <typename TInputIterator>
bool decode(TInputIterator input_begin, TInputIterator input_end)
```
**Description**
Decodes the Base64 characters in the range `input_begin` to `input_end`.

**Return**
`false` if an error was detected, otherwise `true`.

---

```cpp
template <typename TInputIterator>
bool decode(TInputIterator input_begin, size_t input_length)
```
**Description**
Decodes `input_length` Base64 characters starting at `input_begin`.

**Return**
`false` if an error was detected, otherwise `true`.

---

```cpp
template <typename TInputIterator>
bool decode_final(TInputIterator input_begin, TInputIterator input_end)
```
**Description**
Decodes the Base64 characters in the range `input_begin` to `input_end`, then flushes the final block. Equivalent to `decode()` followed by `flush()`.

**Return**
`false` if an error was detected, otherwise `true`.

---

```cpp
template <typename TInputIterator>
bool decode_final(TInputIterator input_begin, size_t input_length)
```
**Description**
Decodes `input_length` Base64 characters starting at `input_begin`, then flushes the final block. Equivalent to `decode()` followed by `flush()`.

**Return**
`false` if an error was detected, otherwise `true`.

---

```cpp
bool flush()
```
**Description**
Decodes any remaining buffered input and marks the end of the data. If a callback has been set it is called with any remaining data and then with an empty span.  
Must be called once all of the input has been passed to `decode()`. Not required when `decode_final()` is used.

**Return**
`false` if an error was detected, otherwise `true`.

---

```cpp
void restart()
```
**Description**
Resets the decoder so that it can be reused to decode a new set of data.

### Reading the result
These functions return a useful value only when no callback has been set. The internal buffer must be large enough to hold the entire decoded result.

```cpp
const unsigned char* begin() const
const unsigned char* cbegin() const
```
**Return**
A pointer to the beginning of the decoded data in the internal buffer.

---

```cpp
const unsigned char* end() const
const unsigned char* cend() const
```
**Return**
A pointer to the end of the decoded data in the internal buffer.

---

```cpp
size_t size() const
```
**Return**
The number of decoded bytes currently held in the internal buffer.

---

```cpp
size_t buffer_size() const
```
**Return**
The maximum size of the internal output buffer (`Buffer_Size`).

---

```cpp
span_type span() const
```
**Return**
A span over the decoded data in the internal buffer.

### Status
```cpp
bool overflow() const
```
**Return**
`true` if the output buffer has overflowed.

---

```cpp
bool invalid_data() const
```
**Return**
`true` if an invalid Base64 character was encountered.

---

```cpp
bool error() const
```
**Return**
`true` if an error has been detected. Equivalent to `overflow() || invalid_data()`.

### Static functions
```cpp
static size_t safe_output_buffer_size(size_t input_length)
```
**Return**
The internal buffer size required to decode `input_length` Base64 characters in a single pass. Use this as the `Buffer_Size` template argument when the whole result is to be read from the internal buffer.

## Examples

### Encode - reading the result from the internal buffer
```cpp
// The data to encode. "Hello World!"
constexpr size_t Input_Length = 12U;

const etl::array<char, Input_Length> input =
{
  0x48, 0x65, 0x6c, 0x6c, 0x6f, 0x20,
  0x57, 0x6f, 0x72, 0x6c, 0x64, 0x21
};

// An encoder whose internal buffer is large enough to hold the entire result.
using encoder_type = etl::base64_rfc4648_encoder<etl::base64_rfc4648_encoder<>::safe_output_buffer_size(Input_Length)>;

encoder_type encoder;

// Encode all of the input and flush the final block.
encoder.encode_final(input.begin(), input.end());

// Copy the result out of the encoder's internal buffer.
etl::string<16> encoded(encoder.begin(), encoder.end());
// encoded == "SGVsbG8gV29ybGQh"
```

---

### Encode - streaming the result through a callback
```cpp
etl::string<16> encoded;

// Called with each chunk of encoded data as the internal buffer fills,
// and finally with an empty span to signal the end of the data.
auto append = [&encoded](const etl::span<const char>& chunk)
{
  encoded.append(chunk.begin(), chunk.end());
};

etl::base64_rfc4648_encoder<>::callback_type callback = append;

// A minimum size encoder that streams its output through the callback.
etl::base64_rfc4648_encoder<> encoder(callback);

encoder.encode_final(input.begin(), input.end());
// encoded == "SGVsbG8gV29ybGQh"
```

---

### Decode - reading the result from the internal buffer
```cpp
const etl::string<16> encoded = "SGVsbG8gV29ybGQh";

// A decoder whose internal buffer is large enough to hold the entire result.
using decoder_type = etl::base64_rfc4648_decoder<etl::base64_rfc4648_decoder<>::safe_output_buffer_size(16U)>;

decoder_type decoder;

// Decode all of the input and flush the final block.
decoder.decode_final(encoded.begin(), encoded.end());

// Copy the result out of the decoder's internal buffer.
etl::vector<unsigned char, 12> output(decoder.begin(), decoder.end());
// output == { 0x48, 0x65, 0x6c, 0x6c, 0x6f, 0x20,
//             0x57, 0x6f, 0x72, 0x6c, 0x64, 0x21 }; // "Hello World!"
```

