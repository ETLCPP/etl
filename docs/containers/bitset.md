---
title: "bitset"
---

---

{{< callout type="">}}
  Header: `bitset.h`  
  Supported: 20.33.0  
  Similar to: [std::bitset](https://en.cppreference.com/w/cpp/utility/bitset.html)
{{< /callout >}}

This is the new default bitset implementation, from 20.33.0 onwards.  
For the older version, see [etl::bitset (legacy)]().

A fixed capacity bitset.  
Has a number of extensions over std::bitset. Can be considered similar to an array of bool.

Internally defined buffers
```C++
etl::bitset<size_t N>
etl::bitset<size_t N, typename TElement>
```

Externally defined buffers from `20.34.0`  
```C++
etl::bitset_ext<size_t N>
etl::bitset_ext<size_t N, typename TElement>
```

The template parameters will determine whether the bitset uses a common implementation or one of four fixed sized 
implementations.

Note: `etl::ibitset` is no longer a reference type for any size `etl::bitset`.

---

`etl::bitset<size_t N>` and `etl::bitset_ext<size_t N>` on their own will use `unsigned char` as the underlying element type and the operations will be implemented by the generic protected member functions in `etl::ibitset`.

---

```C++
etl::bitset<size_t N, typename TElement>
```
```C++
etl::bitset_ext<size_t N, typename TElement>
```
Specifying a type for `TElement` will override the default element type and define it as the *unsigned* type of `TElement`.

**Specialisations for maximum efficiency**  

There are specialisations for when the required number of bits matches the number of bits in the element type.  
These specialisations are considerably faster and more efficient.

```C++
etl::bitset<8,  uint8_t>
etl::bitset<16, uint16_t>
etl::bitset<32, uint32_t>
etl::bitset<8,  uint64_t>
```

`any()`, `none()` and `all()` are overloaded in these specialisations to allow a mask to be specified, so that a bitset that requires less bits than the element type may still use the most efficient implementation.

```C++
etl::bitset<8, uint8_t> bst; // But only want to use 6 bits.
bool any = bst.any(0x3F);    // Check the lower 6 bits.
```

## Types

| Type            |                                                                                              |
| -               | -                                                                                            |  
|`span_type`      | A mutable span type.                                                                         |
|`const_span_type`| A non-mutable span type.                                                                     |
|`element_type`   | The type used as the internal storage for a bitset. By default, this is unsigned char.       |
|`buffer_type`    | The type used as the to define the external buffer. Defined in etl::bitset_ext only. 20.34.0 |

## Constants

All `npos` values are equivalent.
```C++
etl::bitset_constants::npos
```
```C++
etl::bitset<>::npos
```
```C++
template <size_t Size, typename TElement>
etl::bitset<Size, TElement>::npos
```
`Number_Of_Elements`
`Bits_Per_Element`
`Allocated_Bits`
`All_Set_Element`
`All_Clear_Element`

From: 20.38.11  
`Storage_Model` `etl::bitset_storage_model::Single` or `etl::bitset_storage_model::Multi`

## Constructors

The initial state of the bitset is all clear (`false`).

```C++
etl::bitset();
etl::bitset(unsigned long value);
etl::bitset(const char* str);
etl::bitset(const wchar_t* str);
etl::bitset(const char16_t* str);
etl::bitset(const char32_t* str);
```
**Description**  
The bitset is either default constructed, initialised with a numeric value, or a text string of zeros and ones.

---

```C++
etl::bitset_ext(element_type* pbuffer);
etl::bitset_ext(unsigned long value, element_type* pbuffer);
etl::bitset_ext(const char* str, element_type* pbuffer);
etl::bitset_ext(const wchar_t* str, element_type* pbuffer);
etl::bitset_ext(const char16_t* str, element_type* pbuffer);
etl::bitset_ext(const char32_t* str, element_type* pbuffer);
```
**Description**  
The bitset is either default constructed, initialised with a numeric value, or a text string of zeros and ones.
A pointer to the external must b supplied that is large enough to hold the bitset.
The buffer may be defined as follows:-
```C++
using Bitset = etl::bitset_ext<32>;

Bitset::element_type buffer[Bitset::Number_Of_Elements];
```
From: 20.34.0  

---

```C++
etl::bitset_ext(buffer_type& buffer);
etl::bitset_ext(unsigned long value, buffer_type& buffer);
etl::bitset_ext(const char* str, buffer_type& buffer);
etl::bitset_ext(const wchar_t* str, buffer_type& buffer);
etl::bitset_ext(const char16_t* str, buffer_type& buffer);
etl::bitset_ext(const char32_t* str, buffer_type& buffer);
```
**Description**  
The bitset is either default constructed, initialised with a numeric value, or a text string of zeros and ones.
A n external must b supplied that is large enough to hold the bitset.
The buffer may be defined as follows:-
```C++
using Bitset = etl::bitset_ext<32>;

Bitset::buffer_type buffer;
```
From: 20.34.0 

## Modifiers

```C++
etl::bitset& set();  
```
**Description**  
Set all bits.

---

```C++
etl::bitset& set(element_type value);
```
**Description**  
20.34.0
Set the bits to value.
Valid when the bitset width matches the element type width.

---

```C++
etl::bitset& set(const char* str);
etl::bitset& set(const wchar_t* str);
etl::bitset& set(const char16_t* str);
etl::bitset& set(const char32_t* str);
```
**Description**  
Set with a text string of  `0` and `1` characters.

---

```C++
etl::bitset& set(size_t position, bool value = true);
```
**Description**  
Set a position to a one or zero, default one.

---

```C++
etl::bitset& reset();  
```
**Description**  
Reset all bits.

---

```C++
etl::bitset& reset(size_t position);  
```
**Description**  
Set a position to a zero.

---

```C++
etl::bitset& from_string(const char*);
etl::bitset& from_string(const wchar_t*);
etl::bitset& from_string(const char16_t*);
etl::bitset& from_string(const char32_t*);  
```
**Description**  
Alias of set.
The bitset is built from a string of  `0` and `1` characters.

## Access

```C++
template <typename T>
T value() const
```
**Description**  
Returns the value corresponding to the bitset.
`T` specifies the integral type to convert to.

---

```C++
unsigned long to_ulong() const
unsigned long long to_ullong() const
```
**Description**  
Functions for compatibility with the STL.
Calls value<unsigned long>() or value<unsigned long long>().

If the type is too small to contain the bitset size, a compile time error will result.

---

```C++
template <typename TString>
TString to_string(typename TString::value_type zero = typename TString::value_type('0'),
                  typename TString::value_type one  = typename TString::value_type('1')) const
```
Returns the value as a string of `0` and `1` characters.
If the string type is not large enough to contain the digits then an `etl::bitset_string_too_small` is emitted.

---

```C++
span_type span()
const_span_type span() const
```
**Description**  
Returns an `etl::span` of the underlying binary data.  
The span is ordered LSB to MSB.

---

```C++
template <typename T>
ETL_CONSTEXPR14
T extract(size_t position, size_t length = etl::integral_limits<T>::bits) const
```
**Description**  
Extract an integral value from an arbitrary position and length.  
Run time position and length.

---

```C++
template <typename T, size_t Position, size_t Length = etl::integral_limits<T>::bits>
ETL_CONSTEXPR14
T extract() const
```
**Description**  
Extract an integral value from an arbitrary position and length.  
Compile time position and length.  
From: 20.38.11  
For C++11 and above

---

```C++
template <typename T, size_t Position, size_t Length>
T extract() const
```
**Description**  
Extract an integral value from an arbitrary position and length.
Compile time position and length.  
20.38.11  
For C++98/03

## Bit access

```C++
bool operator[](size_t position) const
```
**Description**  
Returns the boolean state of the indexed bit.
position is not checked for validity.

---

```C++
size_t count() const
```
**Description**  
Returns the number of set bits.

---

```C++
size_t size() const
```
**Description**  
Returns the number of bits supported by this bitset.

---

```C++
bool test(size_t position) const
```
**Description**  
Returns the boolean state of the indexed bit.
position is not checked for validity.

---

```C++
bool any() const
```
**Description**  
Returns true if any of the bits are set, otherwise false.

```C++
bool any(element_type mask) const
```
**Description**  
Only enabled for bitsets that fit within one element.
Returns true if any of the bits are set, after the mask has been applied, otherwise false.
Valid when the bitset width matches the element type width.
20.34.0

---

```C++
bool none() const
```
**Description**  
Returns true if none of the bits are set, otherwise false.

```C++
bool none(element_type mask) const
```
**Description**  
Only enabled for bitsets that fit within one element.
Returns true if none of the bits are set, after the mask has been applied, otherwise false.
Valid when the bitset width matches the element type width.
20.34.0

---

```C++
bool all() const
```
**Description**  
Returns true if al of the bits are set, otherwise false.

```C++
bool all(element_type mask) const
```
**Description**  
Only enabled for bitsets that fit within one element.
Returns true if al of the bits are set, after the mask has been applied, otherwise false.
Valid when the bitset width matches the element type width.
20.34.0

---

```C++
size_t find_first(bool state) const
```
**Description**  
Returns the position of the first bit in the specified state. If not found then returns bitset<>::npos.

---

```C++
size_t find_next(bool state, size_t position) const
```
**Description**  
Returns the position of the next bit in the specified state, starting from position. If not found then returns bitset<>::npos.
position is not checked for validity.

## Bit operations

```C++
bitset<size_t Size>& operator &= (const bitset<size_t Size>& rhs);
bitset<size_t Size>& operator |= (const bitset<size_t Size>& rhs);
bitset<size_t Size>& operator ^= (const bitset<size_t Size>& rhs);
bitset<size_t Size>& operator <<= (size_t shift);
bitset<size_t Size>& operator >>= (size_t shift);
bool operator == (const bitset<size_t Size>& rhs);
bool operator != (const bitset<size_t Size>& rhs);
```

## Non-member functions

```C++
void swap(etl::bitset<Size>& lhs, etl::bitset<Size>& rhs)
```
**Description**  
Swaps the contents of the two bitsets.  
The bitsets must be the same size.

