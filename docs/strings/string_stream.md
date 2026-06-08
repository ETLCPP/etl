---
title: "string_stream"
---

{{< callout type="info">}}
  Header: `string_stream.h`  
{{< /callout >}}

## Streaming types
`string_stream`  
`wstring_stream`  
`u16string_stream`  
`u32string_stream`  

Classes to build strings using a streaming API.
Uses `etl::basic_format_spec` to define the required formatting.

The documentation below is for `etl::string_stream`. 
The other streams have a similar API, though using `w`, `u8`, `u16` or `u32` types.

## Member functions
```cpp
explicit string_stream(etl::istring& str)
```
**Description**  
Construct, using the supplied string as a buffer.
Uses the default `format_spec`.

---

```cpp
string_stream(etl::istring& str, const etl::format_spec& spec)
```
**Description**  
Construct, using the supplied string as a buffer.
Initialised with the supplied `format_spec`.

---

```cpp
void set_format(const etl::format_spec& spec_)
```
**Description**  
Sets a new `format_spec` to use.

---

```cpp
const etl::format_spec& get_format() const
```
**Description**  
Gets the current `format_spec`.

---

```cpp
etl::istring& str()
```
**Description**  
Gets a reference to the internal string.

```cpp
const etl::istring& str() const
```
**Description**  
Gets a const reference to the internal string.

---

```cpp
void str(const etl::istring::const_pointer p)
```
**Description**  
Resets the internal string to the text pointed to by `p`.

---

```cpp
void str(const etl::istring& text)
```
**Description**  
Resets the internal string to `text`.

## Streaming operators
```cpp
string_stream& operator <<(string_stream& ss, T value)
```
**Description**  
Where `T` may be one of the following:-

Any fundamental type.  
`etl::format_spec`  
`etl::setb0ase(n)`  
`etl::bin`  
`etl::oct`  
`etl::dec`  
`etl::hex`  
`etl::setfill(c)`  
`etl::setw(n)`  
`etl::setprecision(n)`  
`etl::left`  
`etl::right`  
`etl::boolalpha`  
`etl::noboolalpha`  
`etl::uppercase`  
`etl::nouppercase`  
`etl::showbase`  
`etl::noshowbase`  
`etl::string`  
`etl::istring`  
`etl::string_view`  
`const char*`  

See `etl::format_spec`

Custom user types may be streamed by overloading the streaming operator.

## Examples

### Using format_spec
```cpp
etl::format_spec   format1 = etl::format_spec().hex().width(8).fill('0');
etl::format_spec   format2;
etl::format_spec   format3 = etl::format_spec().width(6).fill('#').precision(3);
etl::string<50>    text;
etl::string_stream stream(text);

stream << format1 << 1193046 << format2 << " " << format3 << 3.1415;
const etl::istring& result = stream.str();
```

`text` is set to `"00123456 #3.142"`.

### Using stream manipulators
```cpp
etl::string<50>    text;
etl::string_stream stream(text);

stream << etl::hex << etl::setw(8) << etl::setfill('0') << 1193046 
       << etl::setw(0) << " " 
       << etl::setw(6) << etl::setfill('#') << etl::setprecision(3) << 3.1415;
const etl::istring& result = stream.str();
```

`text` is set to `"00123456 #3.142"`.

