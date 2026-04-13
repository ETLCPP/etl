---
title: "Base64"
---

{{< callout type="info">}}
  Headers:  
  `base64.h` Common definitions  
  `base64_encoder.h` Encoder class  
  `base64_decoder.h` Decoder class  
  Since: `20.38.4`  
{{< /callout >}}

Encodes and decodes data to and from Base64 format.

## Encode

**Input -> pointer/length : Output -> pointer/length**  
```cpp
template <typename T>
ETL_CONSTEXPR14
static 
size_t encode(const T* input,  size_t input_length, 
              char*    output, size_t output_length)
```
**Description**  
Transforms the input data described by input and input_length to Base64 format to be stored in output output_length.
Enabled if `etl::is_integral<T>` is `true` and `etl::integral_limits<T>::bits == 8`.

**Return**  
The length of the encoded data.  

**Error**  
If output_length is not large enough to hold the encoded data then an `etl::base64_overflow` is raised.

---

**Input -> pointer/length : Output -> string**  
```cpp
template <typename T>
ETL_CONSTEXPR14
static 
size_t encode(const T* input,  size_t input_length, 
              etl::istring& output)
```
**Description**  
Transforms the input data described by input and input_length to Base64 format to be stored in output.  
Enabled if `etl::is_integral<T>` is `true` and `etl::integral_limits<T>::bits == 8`.  
The output string must have capacity of at least `etl::base64::encode_size(input)`.

**Return**  
The length of the encoded data.

**Error**  
If output_length is not large enough to hold the encoded data then an `etl::base64_overflow` is raised.

---

**Input -> pointer/pointer : Output -> pointer/pointer**  
```cpp
template <typename T>
ETL_CONSTEXPR14
static 
size_t encode(const T* input_begin,  const T* input_end,
              char*    output_begin, char*    output_end)
```
**Description**  
Transforms the input data described by input_begin and input_end to Base64 format to be stored in output_begin to output_end.  
Enabled if `etl::is_integral<T>` is `true` and `etl::integral_limits<T>::bits == 8`.  

**Return**  
The length of the encoded data.

**Error**  
If the output buffer is not large enough to hold the encoded data then an `etl::base64_overflow` is raised.

---

**Input -> pointer/pointer : Output -> string**  
```cpp
template <typename T>
ETL_CONSTEXPR14
static 
size_t encode(const T* input_begin, const T* input_end,
              etl::istring& output)
```
**Description**  
Transforms the input data described by input_begin and input_end to Base64 format to be stored in output.  
Enabled if `etl::is_integral<T>` is `true` and `etl::integral_limits<T>::bits == 8`.  
The output string must have capacity of at least `etl::base64::encode_size(input)`.

**Return**  
The length of the encoded data.

**Error**  
If the output buffer is not large enough to hold the encoded data then an `etl::base64_overflow` is raised.

---

**Input -> span : Output -> span**  
```cpp
template <typename T, size_t Length1, size_t Length2>
ETL_CONSTEXPR14
static 
size_t encode(const etl::span<const T, Length1>& input_span,
              const etl::span<char, Length2>&    output_span)
```
**Description**  
Transforms the input data described by input_span to Base64 format to be stored in output_span.
Enabled if etl::is_integral<T> is true and etl::integral_limits<T>::bits == 8.

**Return**  
The length of the encoded data.

**Error**  
If the output span is not large enough to hold the encoded data then an etl::base64_overflow is raised.

---

**Input -> span : Output -> string**  
```cpp
template <typename T, size_t Length>
ETL_CONSTEXPR14
static 
size_t encode(const etl::span<const T, Length>& input_span,
              etl::istring& output)
```
**Description**  
Transforms the input data described by input_span to Base64 format to be stored in output.
Enabled if etl::is_integral<T> is true and etl::integral_limits<T>::bits == 8
The output string  must have capacity of at least etl::base64::encode_size(input).

**Return**  
The length of the encoded data.

**Error**  
If the output span is not large enough to hold the encoded data then an etl::base64_overflow is raised.

---

```cpp
ETL_NODISCARD
ETL_CONSTEXPR14
static 
size_t encode_size(size_t input_length)
```
**Return**  
Returns the size of the output buffer required to encode the specified input data length.

## Decode

**Input -> pointer/length : Output -> pointer/length**  
```cpp
template <typename T>
ETL_CONSTEXPR14
static
size_t decode(const char* input,  size_t input_length, 
              T*          output, size_t output_length)
```
**Description**  
Transforms the Base64 format input data described by input and input_length to by output and output_length.  
Enabled if `etl::is_integral<T>` is `true` and `etl::integral_limits<T>::bits == 8`.

**Return**  
The length of the decoded data.

**Error**  
If `output_length` is not large enough to hold the decoded data then an `etl::base64_overflow` is raised.

---

**Input -> pointer/pointer: Output -> pointer/pointer**  
```cpp
template <typename T>
ETL_CONSTEXPR14
static
size_t decode(const char* input_begin,  const char* input_end, 
               T*         output_begin, T*          output_end)
```
**Description***  
Transforms the Base64 format input data described by `input_begin` and `input_end` to `output_begin` and `output_end`.  
Enabled if `etl::is_integral<T>` is `true` and `etl::integral_limits<T>::bits == 8`.

**Return**  
The length of the decoded data.

**Error**  
If the output buffer is not large enough to hold the decoded data then an `etl::base64_overflow` is raised.

---

**Input -> span: Output -> span**  
```cpp
template <typename T, size_t Length1, size_t Length2>
ETL_CONSTEXPR14
static 
size_t decode(const etl::span<const char, Length1>& input_span,
              const etl::span<T, Length2>&          output_span)
```
**Description**  
Transforms the Base64 format input data described by input_span to output_span.  
Enabled if `etl::is_integral<T>` is `true` and `etl::integral_limits<T>::bits == 8`.

**Return**  
The length of the decoded data.

**Error**  
If the output span is not large enough to hold the decoded data then an `etl::base64_overflow` is raised.

---

```cpp
ETL_NODISCARD
ETL_CONSTEXPR14
static 
size_t decode_size(const char* input, size_t input_length)
```
**Return**  
The size of the output buffer required to decode the specified input Base64 data length.

---

```cpp
ETL_NODISCARD
ETL_CONSTEXPR14
static 
size_t decode_size(const char* input_begin, const char* input_end)
```
**Return**  
The size of the output buffer required to decode the specified input Base64 data length.

---

```cpp
ETL_NODISCARD
ETL_CONSTEXPR14
static 
size_t decode_size(etl::span<const char> input)
```
**Return**  
The size of the output buffer required to decode the specified input Base64 data length.

## Examples

### Encode
```cpp
const etl::array<int8_t, 12> input =
{
  0x48, 0x65, 0x6c, 0x6c, 0x6f, 0x20, 
  0x57, 0x6f, 0x72, 0x6c, 0x64, 0x21 // "Hello World!"
};

etl::string<20> encoded;

size_t encoded_size = etl::base64::encode(input.data(),   input.size(), 
                                          encoded.data(), encoded.size());

encoded.resize(encoded_size); 
// encoded == "OycDQy37KCphrg=="
```

---

*For C++14 or above*  
```cpp
const etl::array<int8_t, 12> input =
{
  0x48, 0x65, 0x6c, 0x6c, 0x6f, 0x20, 
  0x57, 0x6f, 0x72, 0x6c, 0x64, 0x21 // "Hello World!"
};

constexpr size_t length = etl::base64::encode_size(input.size());
etl::string<length> output;
output.uninitialized_resize(output.capacity());

etl::base64::encode(input.data(), input.size(), output.data(), output.size());
// output == "OycDQy37KCphrg=="
```

---

### Decode
```cpp
const etl::string<16> encoded = "OycDQy37KCphrg==";

etl::vector<int8_t, 20> output;

size_t decoded_size = etl::base64::decode(encoded.data(), encoded.size(),
                                          output.data(),  output.size());

output.resize(decoded_size);
// output == { 0x48, 0x65, 0x6c, 0x6c, 0x6f, 0x20, 
//             0x57, 0x6f, 0x72, 0x6c, 0x64, 0x21 };
```

---

*For C++14 or above* 
```cpp
// "OycDQy37KCphrg=="
constexpr const etl::array<char, 16> input =
{
  0x4F, 0x79, 0x63, 0x44, 0x51, 0x79, 0x33, 0x37, 
  0x4B, 0x43, 0x70, 0x68, 0x72, 0x67, 0x3D, 0x3D
};
      
constexpr size_t length = etl::base64::decode_size(input.data(), input.size());
etl::vector<int8_t, length> output;

etl::base64::decode(input.data(), input.size(), output.data(), output.size());
// output == { 0x48, 0x65, 0x6c, 0x6c, 0x6f, 0x20, 
//             0x57, 0x6f, 0x72, 0x6c, 0x64, 0x21 };
```

