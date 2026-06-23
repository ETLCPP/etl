---
title: "to_arithmetic"
---

{{< callout type="info">}}
  Header: `to_arithmetic.h`  
{{< /callout >}}

Functions that will convert a text representation of an arithmetic type to a value.  
Converts from binary, octal, decimal and hex integrals, and decimal floating point types.  
Supports conversion from `char`, `wchar_t`, `char8_t`, `char16_t`, and `char32_t` strings.  
The numeric strings must not contain leading or trailing non-numeric characters. It is the responsibility of the user to select the valid character range.  
Integral types may be `constexpr`.  

## to_arithmetic_status
An ETL enum type that defines the following enumeration values.  
The are the values that may be set as the error code in to_arithmetic_result.  

```cpp
Valid
Invalid_Radix
Invalid_Format
Invalid_Float
Signed_To_Unsigned
Overflow
```

## to_arithmetic_result
The result type returned from `etl::to_arithmetic`.

```cpp
template <typename TValue>
to_arithmetic_result
```

**Types**  
```cpp
value_type      TValue
error_type      etl::to_arithmetic_status
unexpected_type etl::unexpected<etl::to_arithmetic_status>
```

---

```cpp
ETL_CONSTEXPR14
to_arithmetic_result()
```
Default constructor.

---

```cpp
ETL_CONSTEXPR14
to_arithmetic_result(const to_arithmetic_result& other)
```
Copy constructor.

---

```cpp
ETL_NODISCARD
ETL_CONSTEXPR14
bool has_value() const
```
Returns `true` if the result has a valid value.  
If `false` then the value is undefined.

---

```cpp
ETL_NODISCARD
ETL_CONSTEXPR14
operator bool() const
```
Returns `true` if the result has a valid value.  
If `false` then the value is undefined.

---

```cpp
ETL_NODISCARD
ETL_CONSTEXPR14
value_type value() const
```
Returns the value, if valid.  
Otherwise undefined.

---

```cpp
ETL_NODISCARD
ETL_CONSTEXPR14
operator value_type() const
```
Returns the value, if valid.  
Otherwise undefined.

---

```cpp
ETL_NODISCARD
ETL_CONSTEXPR14
error_type error() const
```
Returns the conversion status.  
One of the following:-  
```cpp
Valid
Invalid_Radix
Invalid_Format
Invalid_Float
Signed_To_Unsigned
Overflow
```

---

```cpp
ETL_CONSTEXPR14
to_arithmetic_result& operator =(value_type value)
```
Assign from a value_type.

---

```cpp
ETL_CONSTEXPR14
to_arithmetic_result& operator =(unexpected_type status)
```
Assign from an unexpected_type.

---

## to_arithmetic
Convert a string to an integral or floating point value.  
Supports:  
`etl::string_view` (`etl::basic_string_view<TChar>`)
const pointer (to `char`, `wchar_t`, `char8_t`, `char16_t`, `char32_t`) + length  
`etl::string`, `etl::wstring`, `etl::u8string`, `etl::u16string`, or `etl::u32string`.  

```cpp
template <typename TValue, typename TChar>
ETL_NODISCARD
ETL_CONSTEXPR14
etl::to_arithmetic_result<TValue> to_arithmetic(const etl::basic_string_view<TChar>& view)
```
Text to integral or floating point from view and default decimal radix.

---

```cpp
template <typename TValue, typename TChar>
ETL_NODISCARD
ETL_CONSTEXPR14
etl::to_arithmetic_result<TValue> to_arithmetic(etl::basic_string_view<TChar> view,
                                                const etl::radix::value_type  radix)
```
Text to integral from view and radix value type.  
radix may be one of the following:-  
```cpp
etl::radix::binary
etl::radix::octal
etl::radix::decimal
etl::radix::hex
```

---

```cpp
ETL_NODISCARD
ETL_CONSTEXPR14
etl::to_arithmetic_result<TValue> to_arithmetic(const etl::basic_string_view<TChar>& view, 
                                                const etl::private_basic_format_spec::base_spec& spec)
```
Text to integral from view and radix format spec.  
spec may be one of the following:-  
```cpp
etl::bin
etl::oct
etl::dec
etl::hex
```

---

```cpp
template <typename TValue, typename TChar>
ETL_NODISCARD
ETL_CONSTEXPR14
etl::to_arithmetic_result<TValue> to_arithmetic(const TChar* cp, 
                                                size_t length)
```
Text to integral or floating point from pointer, length and default decimal radix.

---

```cpp
template <typename TValue, typename TChar>
ETL_NODISCARD
ETL_CONSTEXPR14
etl::to_arithmetic_result<TValue> to_arithmetic(const TChar* cp, 
                                                size_t length, 
                                                const etl::radix::value_type radix)
```
Text to integral from pointer, length and radix value type.  
radix may be one of the following:-  
```cpp
etl::radix::binary
etl::radix::octal
etl::radix::decimal
etl::radix::hex
```

---

```cpp
template <typename TValue, typename TChar>
ETL_NODISCARD
ETL_CONSTEXPR14
etl::to_arithmetic_result<TValue> to_arithmetic(const TChar* cp, 
                                                size_t length, 
                                                const etl::private_basic_format_spec::base_spec& spec)
```
Text to integral from pointer, length and radix format spec.  
spec may be one of the following:-  
```cpp
etl::bin
etl::oct
etl::dec
etl::hex
```

---

```cpp
template <typename TValue, typename TChar>
ETL_NODISCARD
ETL_CONSTEXPR14
etl::to_arithmetic_result<TValue> to_arithmetic(const etl::ibasic_string<TChar>& str)
```
Text to integral or floating point from string and default decimal radix.

---

```cpp
template <typename TValue, typename TChar>
ETL_NODISCARD
ETL_CONSTEXPR14
etl::to_arithmetic_result<TValue> to_arithmetic(const etl::ibasic_string<TChar>& str, 
                                                const etl::radix::value_type radix)
```
Text to integral from string and radix value type.  
radix may be one of the following:-  
```cpp
etl::radix::binary
etl::radix::octal
etl::radix::decimal
etl::radix::hex
```

---

```cpp
template <typename TValue, typename TChar>
ETL_NODISCARD
ETL_CONSTEXPR14
etl::to_arithmetic_result<TValue> to_arithmetic(const etl::ibasic_string<TChar>& str, 
                                                const etl::private_basic_format_spec::base_spec& spec)
```
Text to integral from string and radix format spec.  
spec may be one of the following:-  
```cpp
etl::bin
etl::oct
etl::dec
etl::hex
```

## Non-member functions

```cpp
template <typename T>
ETL_CONSTEXPR14 bool operator ==(const etl::to_arithmetic_result<T>& lhs, 
                                 const etl::to_arithmetic_result<T>& rhs)
```
Equality test for etl::to_arithmetic_result

---

```cpp
template <typename T, typename U>
ETL_CONSTEXPR14 bool operator ==(const etl::to_arithmetic_result<T>& lhs, 
                                 const U& rhs)
```
Equality test for etl::to_arithmetic_result

---

```cpp
template <typename T, typename U>
ETL_CONSTEXPR14 bool operator ==(const T& lhs, 
                                 const etl::to_arithmetic_result<U>& rhs)
```
Equality test for etl::to_arithmetic_result

---

```cpp
template <typename T>
ETL_CONSTEXPR14 bool operator !=(const etl::to_arithmetic_result<T>& lhs, 
                                 const etl::to_arithmetic_result<T>& rhs)
```
Inequality test for etl::to_arithmetic_result

---

```cpp
template <typename T, typename U>
ETL_CONSTEXPR14 bool operator !=(const etl::to_arithmetic_result<T>& lhs, 
                                 const U& rhs)
```
Inequality test for etl::to_arithmetic_result

---

```cpp
template <typename T, typename U>
ETL_CONSTEXPR14 bool operator !=(const T& lhs, 
                                 const etl::to_arithmetic_result<T>& rhs)
```
Inequality test for etl::to_arithmetic_result

## Examples

**Valid conversions**  
```cpp
const char* BinaryText("1010011");
const etl::string<3> OctalText("123");
const etl::string<4> DecimalText(" 83 ");
const etl::string<2> HexText("85");
const etl::string<2> FloatText("-123e5");

// From char pointer to binary text. Format spec radix.
int v1 = etl::to_arithmetic<int>(BinaryText, strlen(BinaryText), etl::bin);

// From char pointer from octal text string. Radix spec value.
int v2 = etl::to_arithmetic<int>(OctalText.c_str(), OctalText.size(), etl::radix::octal);

// From view of decimal text string. Default base 10 radix.
etl::string_view view(DecimalText.data() + 1, DecimalText.data() + 3);
int v3 = etl::to_arithmetic<int>(view);

// From hex string. Base 16 radix.
int v4 = etl::to_arithmetic<int>(HexText, 16);
```
---

**Invalid conversions**  
```cpp
const etl::string<10> Text1("abcd");
const etl::string<10> Text2(" 83 ");
const etl::string<10> Text3("-83");
const etl::string<10> Text4("4294967296");
const etl::string<10> Text5("1.23E10000");

etl::to_arithmetic_result result;

result = etl::to_arithmetic<int32_t>(Text1, 5); // etl::to_arithmetic_status::Invalid_Radix

result = etl::to_arithmetic<int32_t>(Text1);    // etl::to_arithmetic_status::Invalid_Format

result = etl::to_arithmetic<int32_t>(Text2);    // etl::to_arithmetic_status::Invalid_Format

result = etl::to_arithmetic<uint32_t>(Text3);   // etl::to_arithmetic_status::Signed_To_Unsigned

result = etl::to_arithmetic<int32_t>(Text4);    // etl::to_arithmetic_status::Overflow

result = etl::to_arithmetic<float>(Text5);      // etl::to_arithmetic_status::Overflow
```
