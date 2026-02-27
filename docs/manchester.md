# Manchester encoding and decoding

Efficient Manchester encoding and decoding of data. The Manchester code represents a data bit as a sequence of a 'high' and a 'low' value. In software this translates to a conversion from one to two bits, or in a practical situation, from `n` bytes to `n*2` bytes.

## See also

[Manchester code](https://en.wikipedia.org/wiki/Manchester_code)

## Features

- Normal and inverted Manchester encoding
- Support for multiple encoding chunk sizes: 8-bit, 16-bit and 32-bit
- Span-based operations or chunk based operations
- Constexpr functions for compile-time encoding/decoding (8-bit chunk size only)
- Validation of encoded data

## Algorithm background

To encode the value `0b11001100` we must first duplicate all bits to create the value `0b1111000011110000`. We then perform an XOR of this value with the constant `0b1010101010101010` (`0xAAAA`) to obtain the Manchester coded value of `0b1010010110100101`. We have now replace each `1` bit with the sequence `10` and each `0` bit with the sequence `01`.

### 2. Bit duplication

Bit duplication is achieved with the following steps. This is also called binary interleaving. The example shows encoding of an 8-bit value.

| Step | High Byte          | Low Byte           | Operation                  |
|------|--------------------|--------------------|----------------------------|
| 0    | `_ _ _ _ _ _ _ _`  | `A B C D E F G H`  | input value (i)            |
| 1    | `_ _ _ _ A B C D`  | `_ _ _ _ E F G H`  | `(i \| (i << 4)) & 0x0F0F` |
| 2    | `_ _ A B _ _ C D`  | `_ _ E F _ _ G H`  | `(i \| (i << 2)) & 0x3333` |
| 3    | `_ A _ B _ C _ D`  | `_ A _ B _ C _ D`  | `(i \| (i << 1)) & 0x5555` |
| 4    | `A A B B C C D D`  | `A A B B C C D D`  | `(i \| (i << 1))`          |

This process can be easily extended to 16-bit or 32-bit values by adding additional steps to the bit duplication.

### 3. Manchester Decoding

Manchester decoding is done in a similar, but reversed way.

### 4. Error Detection

Error detection in Manchester coded data is done by comparing 2 neighboring bits. If they are
equal, then there is an error in the encoded input data.

Comparing all 8 bit-pairs in a 16-bit word is done as follows.

| Step | Binary Value | Operation         | Description                                                                                   |
|------|--------------|-------------------|-----------------------------------------------------------------------------------------------|
| 1    | `11011000`   | Original          | First bit pair (lsb, 00) is invalid. Last bit pair is also invalid. Other bit pairs are valid |
| 2    | `01101100`   | Shift right by 1  | Shift the original value right by one bit                                                     |
| 3    | `10110100`   | XOR               | XOR the original with the shifted value                                                       |
| 4    | `01010101`   | Mask with 0x55    | Apply mask to isolate bit pairs                                                               |
| 5    | `00010100`   | Result            | If result is not equal to 0x55, there was an error in the input                               |

## Analysis

Most traditional ways to Manchester encode data consist of a loop over all bits and a nested if-statement to check the value of the current bit. This approach does not scale well to increasing number of bits. The algorithm implemented here contains no conditional code and scales well. Doubling the number of processed bit per step (the chunk size) adds a single row to the bit duplication table. Because of the lack of loops and conditional code, this algorithm is likely to perform better than traditional ones on simple processors or when compiler optimization is disabled. On modern, powerful processors with caches and advanced optimization possibilities this algorithm may not show much benefit. In any case, the performance of the algorithm depends heavily on the processor type, compiler and compiler (optimization) settings.

## API Reference

### Classes

Classes `etl::manchester` and `etl::manchester_inverted` contain static functions for encoding, decoding and validity checking. It is not necessary to instantiate objects of these classes.

#### etl::manchester

```cpp
typedef manchester_base<private_manchester::manchester_type_normal> manchester;
```

Manchester encoder using normal encoding (no inversion).

#### etl::manchester_inverted

```cpp
typedef manchester_base<private_manchester::manchester_type_inverted> manchester_inverted;
```

Manchester encoder using inverted encoding.

### Encoding Functions

#### Encode single value

```cpp
template <typename TDecoded>
static ETL_CONSTEXPR14 typename encoded<TDecoded>::type encode(TDecoded decoded)
```

Encodes a single value using Manchester encoding.

**Parameters:**

- `decoded`: The value to encode (`uint8_t`, `uint16_t`, or `uint32_t`)

**Returns:**

- The Manchester encoded value (twice the bit width of input)

**Example:**

```cpp
uint16_t encoded = etl::manchester::encode(0x55);
```

#### Encode range

```cpp
template <typename TChunk = uint_least8_t>
static ETL_CONSTEXPR14 void encode(etl::span<const uint_least8_t> decoded, 
                                   etl::span<uint_least8_t> encoded)
```

Encodes a span of data using the specified chunk size.

**Parameters:**

- `decoded`: Source data to encode
- `encoded`: Destination for encoded data (must be twice the size of `decoded`)

**Template Parameters:**

- `TChunk`: Chunk size for encoding (`uint8_t`, `uint16_t` or `uint32_t`)

**Example:**

```cpp
std::array<uint8_t, 4> data = {0x12, 0x34, 0x56, 0x78};
std::array<uint8_t, 8> encoded_data1{};
std::array<uint8_t, 8> encoded_data2{};

// Encode with TChunk == uint8_t
etl::manchester::encode(data, encoded_data1);

// Encode with TChunk == uint32_t
etl::manchester::encode<uint32_t>(data, encoded_data2);
```

### Decoding Functions

#### Decode single value

```cpp
template <typename TEncoded>
static ETL_CONSTEXPR14 typename decoded<TEncoded>::type decode(TEncoded encoded)
```

Decodes a single Manchester encoded value.

**Parameters:**

- `encoded`: The encoded value to decode (`uint16_t`, `uint32_t`, or `uint64_t`)

**Returns:**

- The Manchester decoded value (half the bit width of input)

**Example:**

```cpp
uint8_t decoded = etl::manchester::decode(0x5A5A);
```

#### Decode range

```cpp
template <typename TChunk = typename private_manchester::encoded<uint_least8_t>::type>
static ETL_CONSTEXPR14 void decode(etl::span<const uint_least8_t> encoded, 
                                   etl::span<uint_least8_t> decoded)
```

Decodes a span of Manchester encoded data.

**Parameters:**

- `encoded`: Source data to decode
- `decoded`: Destination for decoded data (must be half the size of `encoded`)

**Template Parameters:**

- `TChunk`: Chunk type for decoding (`uint16_t`, `uint32_t`, or `uint64_t`)

**Example:**

```cpp
std::array<uint8_t, 8> encoded = {/* ... */};
std::array<uint8_t, 4> decoded1 {};
std::array<uint8_t, 4> decoded2 {};

// Decode with TChunk == uint16_t
etl::manchester::decode(encoded, decoded1);

// Decode with TChunk == uint64_t
etl::manchester::decode<uint64_t>(encoded, decoded2);
```

### Validation Functions

#### Single value

```cpp
template <typename TChunk>
static ETL_CONSTEXPR14 bool is_valid(TChunk encoded)
```

Validates that a single value contains valid Manchester encoded data.

**Parameters:**

- `encoded`: The encoded value to validate

**Returns:**

- `true` if the value contains valid Manchester encoded data, `false` otherwise

**Example:**

```cpp
bool valid = etl::manchester::is_valid(0x5A5A);
```

#### Range

```cpp
static ETL_CONSTEXPR14 bool is_valid(etl::span<const uint_least8_t> encoded)
```

Validates that a range contains valid Manchester encoded data.

**Parameters:**

- `encoded`: The range of encoded data to validate

**Returns:**

- `true` if all data is valid Manchester encoding, `false` otherwise

**Example:**

```cpp
std::array<uint8_t, 8> encoded_data = {/* ... */};
bool valid = etl::manchester::is_valid(encoded_data);
```

## Supported Types

### Input/chunk types for encoding

- `uint8_t` → `uint16_t` (if 8-bit types are supported)
- `uint16_t` → `uint32_t`
- `uint32_t` → `uint64_t` (if 64-bit types are supported)

### Input/chunk types for decoding

- `uint16_t` → `uint8_t` (if 8-bit types are supported)
- `uint32_t` → `uint16_t`
- `uint64_t` → `uint32_t` (if 64-bit types are supported)
