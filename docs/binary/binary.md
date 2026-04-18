---
title: binary
---

{{< callout type="info">}}
  Header: `binary.h`  
  Since: `TBC`  
{{< /callout >}}

Utility functions for manipulating binary numbers.

## Rotate
Rotate the bits in the value left or right.

```cpp
template <typename T>
ETL_CONSTEXPR14 T rotate_left(T value)
```
**Return**  
`value` rotated left by one bit.

```cpp
template <typename T>
ETL_CONSTEXPR14 T rotate_left(T value, size_t distance)
```
**Return**  
`value` rotated left by `distance`.

```cpp
template <typename T>
ETL_CONSTEXPR14 T rotate_right(T value)
```
**Return**  
`value` rotated right by one bit.

```cpp
template <typename T>
ETL_CONSTEXPR14 T rotate_right(T value, size_t distance)
```
**Return**  
`value` rotated right by `distance`.

```cpp
template <typename T>
ETL_CONSTEXPR14 T rotate(T value, typename etl::make_signed<size_t>::type distance)
```
**Parameters**  
`distance` Positive is left, negative is right.  
**Return**  
`value` rotated left or right by `distance`.  

## reverse_bits
Reverse the order of the bits in a value.

```cpp
template <typename T>
ETL_CONSTEXPR14 T reverse_bits(T value)
```

The structures below define a member constant value that is Value reversed in bits.
```cpp
template <int8_t Value>
struct reverse_bits_const<int8_t, Value>
```
```cpp
template <uint8_t Value>
struct reverse_bits_const<uint8_t, Value>
```
```cpp
template <int16_t Value>
struct reverse_bits_const<int16_t, Value>
```
```cpp
template <uint16_t Value>
struct reverse_bits_const<uint16_t, Value>
```
```cpp
template <int32_t Value>
struct reverse_bits_const<int32_t, Value>
```
```cpp
template <uint32_t Value>
struct reverse_bits_const<uint32_t, Value>
```
```cpp
template <int64_t Value>
struct reverse_bits_const<int64_t, Value>
```
```cpp
template <uint64_t Value>
struct reverse_bits_const<uint64_t, Value>
```

Defines `value`  The reversed bits.

## reverse_bytes
Reverse the order of the bytes in a value.

```cpp
template <typename T>
ETL_CONSTEXPR T reverse_bytes(T value)
```

## gray_to_binary
```cpp
template <typename T>
ETL_CONSTEXPR14 T gray_to_binary(T value)
```
Converts a gray code value to binary.

## binary_to_gray
```cpp
template <typename T>
ETL_CONSTEXPR T binary_to_gray(T value)
```
Converts a binary value to the gray code equivalent.

## count_bits
```cpp
template <typename T>
ETL_CONSTEXPR14 T count_bits(T value)
```
**Return**  
`1` if the parity of the value is odd, `0` if it is even.

## parity
```cpp
template <typename T>
ETL_CONSTEXPR14 T parity(T value)
```
**Return**  
`1` if the parity of the value is odd, `0` if it is even.

## max_value_for_nbits
```cpp
template <size_t NBits>
struct max_value_for_nbits
```
**Return**  
Maximum unsigned value a particular number of bits can represent.

`value_type` The type for the value.
`value`      The maximum value.

## fold_bits
```cpp
template <typename TReturn, size_t NBits, typename TValue>
ETL_CONSTEXPR14 TReturn fold_bits(TValue value)
```
**Decsription**  
Fold a binary number down to a set number of bits using XOR.  

**Example**  
`0xE8C9AACCBC3D9A8F` folded down to 20 bits = `0x998E8`  

```cpp
uint32_t result = etl::fold_bits<uint32_t, 20>(0xE8C9AACCBC3D9A8F);
```

## sign_extend
Sign extends a binary number.  

```cpp
template <typename TReturn, const size_t NBits, typename TValue>
ETL_CONSTEXPR14 TReturn sign_extend(TValue value)
```
**Description**  
Converts an N bit binary number, where bit N-1 is the sign bit, to a signed integral type.

---
```cpp
template <typename TReturn, const size_t NBits, const size_t Shift, typename TValue>
ETL_CONSTEXPR14 TReturn sign_extend(TValue value)
```
**Description**  
Converts an N bit binary number, where bit N-1 is the sign bit, and `Shift` is the right shift amount, to a signed integral type.

---

```cpp
template <typename TReturn, typename TValue>
ETL_CONSTEXPR14 TReturn sign_extend(TValue value, size_t nbits)
```
**Description**  
Converts an N bit binary number, where bit N-1 is the sign bit, to a signed integral type.

---

```cpp
template <typename TReturn, typename TValue>
ETL_CONSTEXPR14 TReturn sign_extend(TValue value, size_t nbits, size_t shift)
```
**Description**  
Converts an N bit binary number, where bit N-1 is the sign bit, and shift is the right shift amount, to a signed integral type.

## count_leading_zeros
```cpp
template <typename T>
ETL_CONSTEXPR14 T count_leading_zeros(T value)
```
**Decsription**  
Counts the number of leading zeros in a binary number

## count_trailing_zeros
```cpp
template <typename T>
ETL_CONSTEXPR14 T count_trailing_zeros(T value)
```
**Description**  
Counts the number of trailing zeros in a binary number

## count_leading_ones
```cpp
template <typename T>
ETL_CONSTEXPR14 T count_leading_ones(T value)
```
**Description**  
Counts the number of leading ones in a binary number

## count_trailing_ones
```cpp
template <typename T>
ETL_CONSTEXPR14 T count_trailing_ones(T value)
```
**Description**  
Counts the number of trailing ones in a binary number

## first_set_bit_position
```cpp
template <typename T>
ETL_CONSTEXPR14 uint_least8_t first_set_bit_position(T value)
```
**Description**  
Finds the index of the first set bit from lsb.

## first_clear_position
```cpp
template <typename T>
ETL_CONSTEXPR14 uint_least8_t first_clear_bit_position(T value)
```
**Description**  
Finds the index of the first clear bit from lsb.

## first_bit_position
```cpp
template <typename T>
ETL_CONSTEXPR14 uint_least8_t first_bit_position(bool state, T value)
```
**Description**  
Finds the index of the first bit in the specified state, from lsb.

## binary_fill
```cpp
template <typename TResult, typename TValue>
ETL_CONSTEXPR TResult binary_fill(TValue value)
```
Fills a value of the specified width with a repeating binary pattern.  
*Run time*  

Generate `0x12121212`  
```cpp
etl::binary_fill<uint32_t>(uint8_t(0x12));
```

---

```cpp
template <typename TResult, typename TValue, TValue N>
ETL_CONSTEXPR TResult binary_fill(TValue value)
```
*Partial compile time*  

Generate `0x12121212`
```cpp
etl::binary_fill<uint32_t, uint8_t, 0x12>();
```

## has_zero_byte
```cpp
template <typename TValue>
ETL_CONSTEXPR14 bool has_zero_byte(const TValue value)
```
*Run time*  
Checks to see if a value contains a byte of value zero.

**Example**  
`etl::has_zero_byte(uint32_t(0x01234567)) == false`  
`etl::has_zero_byte(uint32_t(0x01230067)) == true`

## has_byte_n
Checks to see if a value contains a byte of a particular value.

```cpp
template <typename TValue>
ETL_CONSTEXPR14 bool has_byte_n(TValue value, uint8_t n)
```
*Run time*  

```cpp
etl::has_byte_n(uint32_t(0x01234567), 0x12) == false
etl::has_byte_n(uint32_t(0x01234567), 0x45) == true
```

---

```cpp
template <typename TValue, TValue N>
ETL_CONSTEXPR14 bool has_byte_n(TValue value)
```
*Partial compile time*  

```cpp
etl::has_byte_n<0x12>(uint32_t(0x01234567)) == false
etl::has_byte_n<0x45>(uint32_t(0x01234567)) == true
```

## binary_merge
Merges two binary values according to a mask.  
Bits set in the mask select bits in the first value, clear bits select those in the second.

```cpp
template <typename T>
ETL_CONSTEXPR T binary_merge(T first, T second, T mask)
```
```cpp
uint8_t first  = 0x12;
uint8_t second = 0x34;

const uint8_t mask = 0xF0;

etl::binary_merge(first, second, mask) Equals 0x14
```

---

```cpp
template <typename T, T Mask>
ETL_CONSTEXPR T binary_merge(T first, T second)
```

```cpp
uint8_t first  = 0x12;
uint8_t second = 0x34;

const uint8_t mask = 0xF0;

etl::binary_merge<uint8_t, mask>(first, second) Equals 0x14
```

## binary_interleave
Interleaves two values such that bits abcd and efgh will result in eafbgchd.

```cpp
ETL_CONSTEXPR14 uint16_t binary_interleave(uint8_t  first, uint8_t  second);
ETL_CONSTEXPR14 int16_t  binary_interleave(int8_t   first, int8_t   second);
ETL_CONSTEXPR14 uint32_t binary_interleave(uint16_t first, uint16_t second);
ETL_CONSTEXPR14 int32_t  binary_interleave(int16_t  first, int16_t  second);
ETL_CONSTEXPR14 uint64_t binary_interleave(uint32_t first, uint32_t second);
ETL_CONSTEXPR14 int64_t  binary_interleave(int32_t  first, int32_t  second);
```

## Odd / Even
Determines the odd or evenness of a value.

```cpp
template <typename T>
ETL_CONSTEXPR bool is_odd(T value)
```

```cpp
template <typename T>
ETL_CONSTEXPR bool is_even(T value);
```

## Constants
```cpp
enum binary_constant
```
An enumeration of 256 constants from `b00000000` to `b11111111` (`0` to `255`)

---

```cpp
enum bit_constant
```
An enumeration of 32 constants from `b0` to `b31` (`1` to `4294967296`)

---

```cpp
template <size_t Position>
struct bit
```

`value_type` The type of the value.  
`value`      The value of the bit at `Position`.

## Creating bit masks
These classes and constexpr functions help create lsb and msb masks.

```cpp
template <typename T, size_t NBits>
class lsb_mask;
```
Defines the member constant value as a binary value of NBits `1` shift to the LSB.  
e.g. `lsb_mask<int8_t, 3>::value == 0b00000111`  
Since: `20.34.0`

---

```cpp
template <typename T>
ETL_CONSTEXPR T make_lsb_mask(size_t nbits)
```
Returns a binary value of nbits `1` shift to the LSB.
e.g. `make_lsb_mask<int8_t>(3) == 0b00000111`  
Since: `20.34.0`

---

```cpp
template <typename T, size_t NBits>
ETL_CONSTEXPR T make_lsb_mask()
```
**Description**  
Returns a binary value of nbits `1` shift to the LSB.  
e.g. `make_lsb_mask<int8_t, 3>() == 0b00000111`  
Since: `20.38.7`

---

```cpp
template <typename T, size_t NBits>
class msb_mask;
```
**Return**  
A binary value of nbits `1` shift to the MSB.  
e.g. `msb_mask<int8_t, 3>::value == 0b11100000`  
Since: `20.34.0`

---

```cpp
template <typename T>
ETL_CONSTEXPR T make_msb_mask(size_t nbits)
```
**Description**  
Defines the member constant value as a binary value of NBits `1` shift to the MSB.  
e.g. `make_msb_mask<int8_t>(3) == 0b11100000`  
20.34.0

---

```cpp
template <typename T, size_t NBits>
ETL_CONSTEXPR T make_msb_mask()
```
**Description**  
Defines the member constant value as a binary value of NBits `1` shift to the MSB.  
e.g. `make_msb_mask<int8_t, 3>() == 0b11100000`  
Since: `20.38.7`  

## Bit manipulation functors
These functors are most useful where lambdas are not available.

## binary_not
```cpp
template <typename T>
struct binary_not : public etl::unary_function<T, T>;
```
Since: `20.38.11`

---

```cpp
ETL_CONTEXPR 
binary_not()
```
**Description**  
Default constructor.

---

```cpp
ETL_CONTEXPR 
ETL_NODISCARD
T operator(T value)
```
**Return**  
~value.

## binary_and
```cpp
template <typename T>
struct binary_and : public etl::unary_function<T, T>;
```
Since: `20.38.11`  

---

```cpp
ETL_CONTEXPR 
binary_and(T and_value)
```
**Description**  
Constructor.  

---

```cpp
ETL_CONTEXPR 
ETL_NODISCARD
T operator(T value)
```
**Return**  
`value & and_value`.

## binary_or
```cpp
template <typename T>
struct binary_or : public etl::unary_function<T, T>;
```
Since: `20.38.11`

---

```cpp
ETL_CONTEXPR 
binary_and(T or_value)
```
**Description**  
Constructor.

---

``cpp
ETL_CONTEXPR 
ETL_NODISCARD
T operator(T value)
``
**Return**  
`value | or_value`.

## binary_xor
```cpp
template <typename T>
struct binary_xor : public etl::unary_function<T, T>;
```
Since: `20.38.11`

---

```cpp
ETL_CONTEXPR 
binary_xor(T xor_value)
```
**Description**  
Constructor.

---

```cpp
ETL_CONTEXPR 
ETL_NODISCARD
T operator(T value)
```
**Return**  
value ^ xor_value.
