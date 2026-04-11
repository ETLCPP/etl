---
title: "basic_format_spec"
---

`format_spec`  
`wformat_spec`  
`u16format_spec`  
`u32format_spec`  

A template class and four typedefs that allow a specification for string formatting functions and streams.
It defines specifications for strings, bool, integrals, floating point and pointers.
Used in conjunction with `etl::to_string` and string streams

The class stores the following specifications:
| Specification  | Description                                                                |
| -------------- | -------------------------------------------------------------------------- |
| base           | The number base. Predefined settings for binary, octal, decimal and hex.   |
| width          | The total minimum field width for the value's text representation.         |
| precision      | The total number of decimal places.                                        |
| upper_case     | If true then numerical digits for bases >10 are in upper case.             |
| left_justified | If true then the text representation is left justified with padding up to the field width on the right. |
| boolalpha      | If true then Boolean values are rendered as "true" and "false".            |
| showbase       | If true then binary, octal and hex values are prefixed with the base tag.  |
| fill           | Determines the character used for the padding for width.                   |

## basic_format_spec

```cpp
template <typename TString>
class basic_format_spec
```
---

```cpp
basic_format_spec()
```
**Description**  
The default constructor.
Sets:-
| Specification    | Default |
| ---------------- | ------- |
| `base`           | `10`    |
| `width`          | `0`     |
| `precision`      | `0`     |
| `upper case`     | `false` |
| `left justified` | `false` |  
| `boolalpha`      | `false` |
| `show base`      | `false` |  
| `fill`           | `' '`   |

```cpp
basic_format_spec& base(const uint32_t b)
```
**Description**  
Sets the base to b.

---

```cpp
basic_format_spec& binary()
```
**Description**  
Sets the base to binary.

---

```cpp
basic_format_spec& octal()
```
**Description**  
Sets the base to octal.

---

```cpp
basic_format_spec& hex()
```
**Description**  
Sets the base to hexadecimal.

---

```cpp
basic_format_spec& octal()
```
**Description**  
Sets the base to octal.

---

```cpp
uint32_t get_base() const
```
**Description**  
Returns the current setting for base.

---

```cpp
basic_format_spec& width(const uint32_t w)
```
**Description**  
Sets the width to `w`.

---

```cpp
uint32_t get_width() const
```
**Description**  
Returns the current width value.

---

```cpp
basic_format_spec& precision(const uint32_t p)
```
**Description**  
Sets the precision to `p`.

```cpp
uint32_t get_precision() const
```
**Description**  
Returns the current precision value.

---

```cpp
basic_format_spec& upper_case(const bool b)
```
**Description**  
Sets the upper case flag  to `b`.
Only affects numerical output such as hexadecimal.

---

```cpp
bool is_upper_case() const
```
**Description**  
Returns true if the upper case flag is set.

---

```cpp
basic_format_spec& show_base(const bool b)
```
**Description**  
Sets the showbase flag  to `b`.
Only affects binary and hexadecimal output.

---

```cpp
bool is_show_base() const
```
**Description**  
Returns `true` if the `showbase` flag is set.

---

```cpp
basic_format_spec& fill(const typename TString::value_type c)
```
**Description**  
Sets the fill character to `c`.

---

```cpp
typename TString::value_type get_fill() const
```
**Description**  
Returns the fill character.

---

```cpp
basic_format_spec& left()
```
**Description**  
Sets the `left` justified flag to `true`.

---

```cpp
bool is_left() const
```
**Description**  
Returns `true` if the `left` justified flag is set.

---

```cpp
basic_format_spec& right()
```
**Description**  
Sets the left justified flag  to false.

---

```cpp
bool is_right() const
```
**Description**  
Returns `true` if the `left` justified flag is clear.

```cpp
basic_format_spec& boolalpha(bool b)
```
**Description**  
Sets the `bool alpha` flag  to `b`.

---

```cpp
bool is_boolalpha() const
```
Returns `true` if the `bool alpha` flag is set.

## Typedefs

| Type           | Specialisation                          |
| -------------- | --------------------------------------- |
| format_spec    | etl::basic_format_spec<etl::istring>    |
| wformat_spec   | etl::basic_format_spec<etl::iwstring>   |
| u16format_spec | etl::basic_format_spec<etl::iu16string> |
| u32format_spec | etl::basic_format_spec<etl::iu32string> |

## Stream manipulators
These manipulators are used in conjunction with the ETL's string streams.  
Streams may also use a `format_spec` as a manipulator.

```cpp
setbase(uint32_t base)
```
**Description**  
Sets the base for numerical output to base.

---

```cpp
setw(uint32_t width)
```
**Description**  
Sets the format width to `width`.

---

```cpp
template <typename TChar>
setfill(TChar fill)
```
**Description**  
Sets the fill character to fill.

---

```cpp
setprecision(uint32_t precision)
```
**Description**  
Sets the number of decimal places to precision.

---

```cpp
bin
```
**Description**  
Sets the base for numerical output to binary.

---

```cpp
oct
```
**Description**  
Sets the base for numerical output to octal.

---

```cpp
dec
```
**Description**  
Sets the base for numerical output to decimal.

---

```cpp
hex
```
**Description**  
Sets the base for numerical output to hexadecimal.

---

```cpp
left
```
**Description**  
Sets the alignment to left.

---

```cpp
right
```
**Description**  
Sets the alignment to right.

---

```cpp
boolalpha
```
**Description**  
Sets the boolalpha flag to true.

---

```cpp
noboolalpha
```
**Description**  
Sets the boolalpha flag to false.

---

```cpp
uppercase
```
**Description**  
Sets the uppercase flag to true.

---

```cpp
nouppercase
```
**Description**  
Sets the uppercase flag to false.

---

```cpp
showbase
```
**Description**  
Sets the showbase flag to true.

---

```cpp
noshowbase
```
**Description**  
Sets the showbase flag to false.

## Example (to_string)

```cpp
etl::format_spec format;

// Format as a hex character, minimum fill width of 8, fill with zeros.
format.hex().width(8).fill('0');

etl::string<8> text;

// 'text' is set to "00123456"
etl::to_string(1193046, text, format);

// Format minimum fill width of 8, fill with space and three decimal digits.
format.width(8).fill(' ').precision(3); 

// 'text' is set to "   3.142"
etl::to_string(3.1415, text, format);
```

## Example (string_stream)
```cpp
etl::string<20> buffer;
etl::string_stream stream(buffer);

// Manipulators
stream << etl::showbase
       << etl::bin << 123456 << " "
       << etl::oct << 123456 << " "
       << etl::dec << 123456 << " "
       << etl::hex << 123456;
```
The generated string:-  

`0b11110001001000000 0361100 123456 0x1e240`

---

```cpp
// Format spec
etl::format_spec format;
format.show_base(true).hex();

stream << format << 123456;
```

The generated string:-  

`0x1e240`
