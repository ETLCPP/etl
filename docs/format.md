# ETL Format & Print

## 1. Overview

ETL provides text formatting facilities modelled on C++20 `std::format` and C++23
`std::print`.  They allow type-safe, positional formatting of values into strings
or directly to a character output device — without heap allocation.

**Minimum language standard:** C++11 (`ETL_USING_CPP11`).

**Headers:**

| Header | Provides |
|---|---|
| `etl/format.h` | `etl::format_to`, `etl::format_to_n`, `etl::formatted_size` |
| `etl/print.h`  | `etl::print`, `etl::println` (includes `etl/format.h`) |

## 2. `etl::format_to`

### Generic output-iterator overload

```cpp
template<typename OutputIt, class... Args>
OutputIt format_to(OutputIt out, format_string<Args...> fmt, Args&&... args);
```

Formats `args` according to the format string `fmt` and writes the result through
the output iterator `out`.  Returns an iterator past the last character written.

`OutputIt` can be any output iterator whose dereferenced type is assignable from
`char`, for example `etl::istring::iterator` or
`etl::back_insert_iterator<etl::istring>`.

```cpp
etl::string<100> s;

// Using a raw iterator — you must resize the string yourself
etl::istring::iterator result = etl::format_to(s.begin(), "{0} {1}", 34, 56);
s.uninitialized_resize(static_cast<size_t>(result - s.begin()));
// s == "34 56"

// Using a back_insert_iterator — string grows automatically
s.clear();
etl::back_insert_iterator<etl::istring> it(s);
etl::format_to(it, "{} {}", 65, 34);
// s == "65 34"
```

### `etl::istring&` overload (ETL-specific)

```cpp
template<class... Args>
etl::istring::iterator format_to(etl::istring& out,
                                  format_string<Args...> fmt,
                                  Args&&... args);
```

Convenience overload that writes into an `etl::istring` (or any derived
`etl::string<N>`).  The string is automatically resized to the number of
characters written, up to `out.max_size()`.  Returns an iterator past the
last character written.

```cpp
etl::string<100> s;
etl::format_to(s, "Hello, {}!", "world");
// s == "Hello, world!"
```

### `etl::format_to_n`

```cpp
template<typename OutputIt, class... Args>
OutputIt format_to_n(OutputIt out, size_t n,
                     format_string<Args...> fmt, Args&&... args);
```

Like `format_to`, but writes **at most** `n` characters.  Characters beyond the
limit are silently discarded.

```cpp
etl::string<10> s = "abcdefghij";
etl::format_to_n(s.begin(), 3, "xy{}", 123);
// s == "xy1defghij"   (only 3 chars written)
```

## 3. `etl::formatted_size`

```cpp
template<class... Args>
size_t formatted_size(format_string<Args...> fmt, Args&&... args);
```

Returns the total number of characters that `format_to` would produce, without
actually writing anything.  Useful for pre-computing buffer sizes.

```cpp
size_t n;
n = etl::formatted_size("");            // 0
n = etl::formatted_size("{}", "");      // 0
n = etl::formatted_size("xyz{}", 12);  // 5
n = etl::formatted_size("{}", "abc");   // 3
```

## 4. `etl::print` and `etl::println`

Declared in `etl/print.h`.

### `etl::print`

```cpp
template<class... Args>
void print(etl::format_string<Args...> fmt, Args&&... args);
```

Formats the arguments and outputs each character by calling `etl_putchar()`.

### `etl::println`

```cpp
// With arguments — prints formatted text followed by '\n'
template<class... Args>
void println(etl::format_string<Args...> fmt, Args&&... args);

// Without arguments — prints a bare newline
void println();
```

### Implementing `etl_putchar`

`etl/print.h` declares (but does not define) the following C-linkage function:

```cpp
extern "C" void etl_putchar(int c);
```

You **must** provide a definition in your project.  The `int` parameter follows
the convention of the standard `putchar()` and carries a single `char` value.

Typical implementations forward to a UART, a debug probe, `putchar`, or any
other single-character output sink:

```cpp
// Example: forward to standard putchar
extern "C" void etl_putchar(int c)
{
  putchar(c);
}
```

### Example

```cpp
etl::print("x = {}, y = {}\n", 10, 20);   // "x = 10, y = 20\n"
etl::println("Hello, {}!", "world");        // "Hello, world!\n"
etl::println();                             // "\n"
```

## 5. Format String Syntax

A format string is ordinary text with **replacement fields** delimited by braces:

```
"literal text {} more text {1:>10} end"
```

### Replacement field grammar

```
replacement_field ::= '{' [arg_id] [':' format_spec] '}'
arg_id            ::= integer            // e.g. 0, 1, 2 …
format_spec       ::= [[fill]align] [sign] ['#'] ['0'] [width] ['.' precision] ['L'] [type]
```

| Component | Syntax | Description |
|---|---|---|
| **Argument index** | `{0}`, `{1}`, … | Manual positional indexing. Cannot be mixed with automatic indexing. |
| **Automatic index** | `{}` | Uses the next argument in order. Cannot be mixed with manual indexing. |
| **Fill character** | any character except `{` or `}` | Used together with an alignment specifier. Default is space (` `). |
| **Alignment** | `<` left, `>` right, `^` center | Aligns the formatted value within the given *width*. |
| **Sign** | `+` always, `-` negative only (default), ` ` space for positive | Controls sign display for numeric types. |
| **`#` (alt form)** | `#` | Adds `0x`/`0X` for hex, `0b`/`0B` for binary, `0` for octal. |
| **`0` (zero-pad)** | `0` | Pads the number with leading zeros (after sign/prefix). |
| **Width** | integer, or `{}` / `{n}` | Minimum field width.  Supports nested replacement fields for dynamic width. |
| **Precision** | `.` integer, or `.{}` / `.{n}` | For strings: maximum characters to output.  For floats: number of decimal digits.  Supports nested replacement fields. |
| **`L`** | `L` | Locale-specific flag (parsed but currently ignored). |
| **Type** | see [Presentation Types](#7-presentation-types-per-argument-kind) | Selects the output representation. |

### Examples

```cpp
etl::format_to(s, "{:>10}", 42);       // "        42"
etl::format_to(s, "{:*^10}", 42);      // "****42****"
etl::format_to(s, "{:+05d}", 67);      // "+00067"
etl::format_to(s, "{:#x}", 0x3f4);     // "0x3f4"
etl::format_to(s, "{:.3s}", "abcdef"); // "abc"
etl::format_to(s, "{1} {0}", 1, 2);    // "2 1"
```

## 6. Supported Argument Types

The core set of formattable types (matching `std::basic_format_arg`):

| Category | Types |
|---|---|
| Boolean | `bool` |
| Character | `char` |
| Signed integer | `int`, `long long int` |
| Unsigned integer | `unsigned int`, `unsigned long long int` |
| Floating-point *(opt-in)* | `float`, `double`, `long double` — requires `ETL_USING_FORMAT_FLOATING_POINT` |
| String | `const char*`, `etl::string_view` |
| Pointer | `const void*` |

### Implicit conversions

Types not listed above are converted automatically before formatting:

| Source type | Stored as |
|---|---|
| `short` | `int` |
| `unsigned short`, `uint16_t` | `unsigned int` |
| `long int` | `int` or `long long int` (platform-dependent) |
| `unsigned long int`, `size_t` | `unsigned int` or `unsigned long long int` |
| `int8_t` (`signed char`) | `char` |
| `uint8_t` (`unsigned char`) | `char` |
| `int16_t` | `int` |
| `uint32_t` | `unsigned int` |
| `int32_t` | `int` |
| `etl::string<N>` | `etl::string_view` (lifetime of the temporary is guaranteed) |
| any pointer `T*` | `const void*` |

## 7. Presentation Types per Argument Kind

### Integers (`int`, `unsigned int`, `long long int`, `unsigned long long int`)

| Type | Meaning | Example |
|---|---|---|
| `d` *(default)* | Decimal | `134` → `"134"` |
| `x` | Lowercase hexadecimal | `0x3f4` → `"3f4"` |
| `X` | Uppercase hexadecimal | `0x3f4` → `"3F4"` |
| `o` | Octal | `034` → `"34"` |
| `b` | Lowercase binary | `0b1010` → `"1010"` |
| `B` | Uppercase binary | `0b1010` → `"1010"` |
| `c` | Character (value as char) | `67` → `"C"` |

With `#`: prefixes `0x`/`0X`, `0b`/`0B`, or leading `0` for octal.

### Characters (`char`, `signed char`, `unsigned char`)

| Type | Meaning | Example |
|---|---|---|
| `c` *(default)* | Character itself | `'s'` → `"s"` |
| `?` | Debug / escaped | `'\n'` → `"'\\n'"` |
| `d` | Decimal code point | `'a'` → `"97"` |
| `x` / `X` | Hex code point | `'a'` → `"61"` |

### Booleans (`bool`)

| Type | Meaning | Example |
|---|---|---|
| *(default)* | `false` / `true` | `true` → `"true"` |
| `s` | Same as default | `true` → `"true"` |
| `d` | `0` / `1` | `true` → `"1"` |
| `x` / `X` | Hex `0` / `1` | `true` → `"1"` |
| `o` | Octal (with `#`: `01`) | `true` → `"01"` |

### Strings (`const char*`, `etl::string_view`, `etl::string<N>`)

| Type | Meaning | Example |
|---|---|---|
| `s` *(default)* | String output | `"data1"` → `"data1"` |
| `?` | Debug / escaped | `"data1\n"` → `"\"data1\\n\""` |

Width and precision apply: width sets the minimum field width; precision (`.N`)
truncates the string to at most *N* characters.

```cpp
etl::format_to(s, "{:>10s}", "data1");    // "     data1"
etl::format_to(s, "{:.3s}", "abcdef");    // "abc"
etl::format_to(s, ".{:^8.3s}!", "data1"); // ".  dat   !"
```

### Pointers (`const void*`)

| Type | Meaning | Example |
|---|---|---|
| `p` *(default)* | Lowercase hex with `0x` prefix | `nullptr` → `"0x0"` |
| `P` | Uppercase hex with `0X` prefix | `nullptr` → `"0X0"` |

### Floating-point (`float`, `double`, `long double`)

Requires `ETL_USING_FORMAT_FLOATING_POINT`.

| Type | Meaning | Example |
|---|---|---|
| *(default)* | Shortest representation | `1.5f` → `"1.5"` |
| `e` / `E` | Scientific notation | `1.0f` → `"1.000000e+00"` |
| `f` / `F` | Fixed-point notation | `1.125f` → `"1.125000"` |
| `g` / `G` | General (fixed or scientific) | `1e10f` → `"1.000000e+10"` |
| `a` / `A` | Hexadecimal floating-point | `1.5f` → `"0x1.8p+0"` |

`nan`, `inf` (lowercase for `e`/`f`/`g`/`a`, uppercase for `E`/`F`/`G`/`A`).

## 8. Escape Sequences and Literal Braces

### Literal braces

Because `{` and `}` delimit replacement fields, they must be escaped by
doubling:

| Input | Output |
|---|---|
| `{{` | `{` |
| `}}` | `}` |

```cpp
etl::format_to(s, "abc{{def");  // "abc{def"
etl::format_to(s, "}}abc");     // "}abc"
```

### Debug / escaped presentation (`?`)

The `?` type specifier produces a debug representation:

- **Characters** are wrapped in single quotes with C-style escape sequences:

  | Character | Output |
  |---|---|
  | `\t` | `'\\t'` |
  | `\n` | `'\\n'` |
  | `\r` | `'\\r'` |
  | `"` | `'\\\"'` |
  | `'` | `'\\''` |
  | `\\` | `'\\\\'` |

- **Strings** are wrapped in double quotes with the same escape sequences:

  ```cpp
  etl::format_to(s, "{:?}", "data1\n");  // "\"data1\\n\""
  ```

## 9. Error Handling

Invalid format strings cause an `etl::bad_format_string_exception` (derived from
`etl::format_exception`, which is derived from `etl::exception`).

Common error conditions:

| Condition | Example |
|---|---|
| Missing closing brace | `"a{b"` |
| Unescaped `}` without matching `{` | `"a}b"` |
| Invalid characters inside `{}` | `"a{b}"` |
| Argument index out of range | `"{1}"` with only one argument |
| Mixing manual and automatic indexing | `"{0} {}"` |
| Invalid type specifier for the argument | `"{:d}"` on a `string_view` |
| Double colon in format spec | `"{::}"` |
| Precision on an integer | `"{:+#05.5X}"` on an `int` |

```cpp
etl::string<100> s;
// These all throw etl::bad_format_string_exception:
etl::format_to(s, "a{b}",  1);      // bad index spec
etl::format_to(s, "a{b",   1);      // closing brace missing
etl::format_to(s, "a}b");           // unescaped }
etl::format_to(s, "{:d}", sv);      // invalid type for string_view
```

> **Note:** On C++20 and later, format strings are validated at compile time
> via `consteval`.  The checks cover syntax (balanced braces, valid format spec
> grammar, index bounds, no mixing of automatic and manual indexing) as well as
> type/specifier compatibility (e.g. `{:d}` is rejected for string arguments).
> A malformed format string produces a compile error whose diagnostic mentions
> `please_note_this_is_error_message_format_string_syntax_error`.
>
> On C++11–C++17, the same checks run at runtime and throw
> `etl::bad_format_string_exception`.

## 10. Differences from `std::format`

| Area | `std::format` (C++20/23) | ETL |
|---|---|---|
| **Output target** | Returns `std::string` | Writes through an output iterator or into `etl::istring&` — no heap allocation. |
| **`etl::istring&` overload** | Not available | `format_to(etl::istring&, ...)` automatically resizes the string. |
| **`print` / `println` output** | Writes to `FILE*` / `stdout` | Writes character-by-character via user-defined `etl_putchar(int)`. |
| **Floating-point support** | Always available | Opt-in via `ETL_USING_FORMAT_FLOATING_POINT`. |
| **User-defined formatters** | `std::formatter<T>` specialisations | Not yet supported. |
| **Locale** | `L` flag uses `std::locale` | `L` flag is parsed but has no effect. |
| **Compile-time validation** | Enforced via `consteval` on C++20 | Enforced via `consteval` on C++20 (syntax and type/specifier compatibility); validates at run time and throws `etl::bad_format_string_exception` on C++11–C++17. |
| **`format_to_n` return type** | `std::format_to_n_result` | Returns the underlying `OutputIt` directly. |
