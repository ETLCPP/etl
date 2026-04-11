---
title: "String Utilities"
---

A set of utilities to make string manipulation a little easier.

The documentation below is for handling `etl::string`. 
The other strings have a similar API, though using `w`,, `u8`, `u16` or `u32` types from the ETL and STL.
The string utilities are compatible with any string-like container that exposes a compatible API.

**Example**  
```cpp
void trim_whitespace_left(etl::istring& s)
void trim_whitespace_left(etl::iwstring& s)
void trim_whitespace_left(std::string& s)
void trim_whitespace_left(std::u32string& s)
```

## Whitespace
Whitespace characters are deemed as `' '`, `'\t'`, `'\n'`, `'\r'`, `'\f'`, `'\v'`

### Modifying functions
```cpp
void trim_whitespace_left(etl::istring& s)
```
**Description**  
Trims the whitespace characters from the left of s.

---
```cpp
void trim_from_left(etl::istring& s, 
                    etl::istring::const_pointer trim_characters)
```
**Description**  
Trims any of the characters in trim_characters from the left of `s`.  
Stops at the first character not in `trim_characters`.

---

```cpp
void trim_left(etl::istring& s, 
               etl::istring::const_pointer delimiters)
```
**Description**  
Trims all of the characters in up to the first character in `delimiters` from the left of `s`.

---

```cpp
void trim_whitespace_right(etl::istring& s)
```
**Description**  
Trims the whitespace characters from the right of `s`.

---

```cpp
void trim_from_right(etl::istring& s, 
                     etl::istring::const_pointer trim_characters)
```
**Description**  
Trims any of the characters in `trim_characters` from the right of `s`.  
Stops at the first character not in `trim_characters`.

---

```cpp
void trim_right(etl::istring& s, 
                etl::istring::const_pointer delimiters)
```
**Description**  
Trims all of the characters in up to the first character in `delimiters` from the right of `s`.

---

```cpp
void trim_whitespace(etl::istring& s)
```
**Description**  
Trims the whitespace characters from both ends of `s.`

---

```cpp
void trim_from(etl::istring& s, 
               etl::istring::const_pointer trim_characters)
```
**Description**  
Trims any of the characters in `trim_characters` from the right of `s`.  
Stops at the first character not in `trim_characters`.

---

```cpp
void trim(etl::istring& s, 
          etl::istring::const_pointer delimiters)
```
**Description**  
Trims all of the characters in up to the first character in `delimiters` from both ends of `s`.

---

...cpp
void reverse(etl::istring& s)
```
**Description**  
Reverses `s`.

---

```cpp
void left_n(etl::istring& s, 
            size_t n)
```
**Description**  
Trims `s` to the left `n` most characters.  
If the string is less than `n` characters long then it is left unchanged.

---

```cpp
void right_n(etl::istring& s, 
             size_t n)
```
**Description**  
Trims `s` to the right `n` most characters.  
If the string is less than `n` characters long then it is left unchanged.

---

```cpp
void pad_left(etl::istring& s, 
              size_t required_size, 
              etl::istring::value_type pad_char)
```
**Description**  
Pads `s` to length required_size by adding `pad_char` to the left.  
If the string length is greater than or equal to required_size then it is left unchanged.

---

```cpp
void pad_right(etl::istring& s, 
               size_t required_size, 
               etl::istring::value_type pad_char)
```
**Description**
Pads `s` to length required_size by adding `pad_char` to the right.  
If the string length is greater than or equal to  required_size then it is left unchanged.

---

```cpp
void pad(etl::istring& s, 
         size_t required_size, 
         string_pad_direction pad_direction, 
         etl::istring::value_type pad_char)
```
Pads `s` to length `required_size` by adding `pad_char` to the end specified by `pad_direction`.  
If the string length is greater than or equal to  required_size then it is left unchanged.

---

```cpp
void to_upper_case(etl::istring& s)
```
**Description**
Change `s` to upper case.  
`"hElLo WoRLd"` => `"HELLO WORLD"`  

Valid for `etl::istring` only.  

---

```cpp
void to_lower_case(etl::istring& s)
```
**Description**  
Change `s` to lower case.  
`"hElLo WoRLd"` => `"hello world"`  

Valid for `etl::istring` only.  

---

```cpp
void to_sentence_case(etl::istring& s)
```
**Description**  
Change `s` to sentence case.  
`"hElLo WoRLd"` => `"Hello world"`

Valid for `etl::istring` only.

---

```cpp
void replace(etl::istring& s,
             const etl::pair<etl::istring::value_type, 
                             etl::istring::value_type>* pairsbegin, 
             const etl::pair<etl::istring::value_type, 
                             etl::istring::value_type>* pairsend)
```
**Arguments**  
`pairsbegin` Pointer to the first pair in the list.  
`pairsend` Pointer to one past the last pair in the list.

**Description**  
Replaces characters according the supplied lookup table of `etl::pair`.  
Each pair specifies an old/new character replacement.

---

```cpp
void replace(etl::istring& s,
             const etl::pair<const etl::istring::value_type*, 
                             const etl::istring::value_type*>* pairsbegin,
             const etl::pair<const etl::istring::value_type*, 
                             const etl::istring::value_type*>* pairsend)
```
**Arguments**  
`pairsbegin`  Pointer to the first pair in the list.
`pairsend`    Pointer to one past the last pair in the list.

**Description**  
Replaces strings according the supplied lookup table of `etl::pair`.
Each pair specifies an old/new string replacement.

## Non-modifying functions

```cpp
etl::string_view trim_view_whitespace_left(const etl::string_view& view)
```
**Description**  
Returns a `string_view` of the whitespace characters trimmed from the left of `view`.

---

```cpp
etl::string_view trim_from_view_left(const etl::string_view& view, 
                                     etl::istring::const_pointer trim_characters)
```
**Description**  
Returns a `string_view` of the characters in `trim_characters` trimmed from the left of `view`.
Stops at the first character not in `trim_characters`.

---

```cpp
etl::string_view trim_view_left(etl::string_view& view, 
                                etl::string_view::const_pointer delimiters)
```
**Description**  
Returns a `string_view` of the characters in up to the first character in `delimiters` from the left of `view`.

---

```cpp
etl::string_view trim_view_whitespace_right(const etl::string_view& view)
```
**Description**  
Returns a `string_view` of the whitespace characters trimmed from the right of `view`.

---

```cpp
etl::string_view trim_from_view_right(const etl::string_view& view, 
                                      etl::istring::const_pointer trim_characters)
```
**Description**  
Returns a `string_view` of the characters in `trim_characters` trimmed from the right of `view`.
Stops at the first character not in `trim_characters`.

---

```cpp
etl::string_view trim_view_right(const etl::string_view& view, 
                                 etl::istring::const_pointer delimiters)
```
**Description**  
Returns a `string_view` of the characters in up to the first character in `delimiters` from the right of `view`.

---

```cpp
etl::string_view trim_whitespace(const etl::string_view& view)
```
**Description**  
Returns a `string_view` of the whitespace characters trimmed from both ends of `view`.

---

```cpp
etl::string_view trim_view_from(const etl::string_view& view, 
                                etl::istring::const_pointer trim_characters)
```
**Description**  
Returns a `string_view` of the characters in `trim_characters` trimmed from both ends of `view`.
Stops at the first character not in `trim_characters`.

---

```cpp
etl::string_view trim(const etl::string_view& view, 
                      etl::istring::const_pointer delimiters)
```
**Description**  
Returns a `string_view` of the characters in up to the first character in `delimiters` from both ends of `view`.

---

```cpp
etl::string_view left_n_view(etl::string_view view, 
                             size_t n)
```
Returns a `string_view` to the left `n` most characters of `view`.
If the string is less than `n characters long then the returned view equal the supplied `view`.

---

```cpp
etl::string_view right_n_view(etl::string_view view, 
                              size_t n)
```
**Description**  
Returns a `string_view` to the right `n` most characters of `view`.
If the string is less than `n` characters long then the returned view equal the supplied `view`.

---

```cppp
etl::optional<etl::string_view> get_token(const INPUT_TYPE& s, 
                                          const char* delimiters, 
                                          const etl::optional<etl::string_view>& last_view,
                                          bool ignore_empty_tokens)
```
**Description**  
Where `INPUT_TYPE` is any container type that supports `data()` and `size()` member functions.  
Tokenizes the string.  
The returned token will be invalid for the call after the last token has been extracted.

`s`                   The string to tokenize.  
`delimiters`          The delimiters between tokens.  
`last_view`           The last returned token view or default constructed view.  
`ignore_empty_tokens` If `true`, empty tokens will be ignored, otherwise empty tokens will return an empty view.  

**Example**  
```cpp
using String     = etl::string<32>;
using StringView = etl::string_view;
using Vector     = etl::vector<String, 10>;
using Token      = etl::optional<StringView>;

String text("  The cat.sat,  on;the:mat .,;:");
Vector tokens;

Token token; // Default constructed token.
      
while ((token = etl::get_token(text, " .,;:", token, true))) // Exit once we get an invalid token.
{ 
  // Place it in the token list.
  tokens.emplace_back(token.value());
}
```

`tokens` will contain `"The", "cat", "sat", "on", "the", "mat"`.

---

```cpp
template <typename TInput, typename TOutput>
bool get_token_list(const TInput& input, 
                    TOutput& output, 
                    typename TInput::const_pointer delimiters, 
                    bool ignore_empty_tokens, 
                    size_t max_n_tokens = etl::integral_limits<size_t>::max)
```
>20.41.0  

**Description**  
Splits a string of tokens to a set of views, according to a set of `delimiters`.  

`input`               The input string.
`output`              A reference to an output container of string views.
`delimiters`          A pointer to a string of valid `delimiters`.
`ignore_empty_tokens` If `true` then empty tokens are ignored.
`max_n_tokens`        The maximum number of tokens to collect. Default: tokenise everything.

**Return**  
`true` if all tokens were added to the list, otherwise `false`.

The tokenisation stops if:
1. The end of the input text is reached.
2. The max_size() of the output container is reached.
3. The number of tokens found reaches max_n_tokens. 

The input container must define the type `const_pointer`.
The output container must define the type `value_type`.
The output container must define the member function `max_size()` that returns the maximum size of the container.
The output container must define the member function `push_back()` that pushes the view on to the back of the container.

**Example**  
```cpp
std::string text(",,,The,cat,sat,,on,the,mat");
std::vector<std::string_view> views;

bool all_views_found = etl::get_token_list(text, views, ",", true, 3);

all_views_found == false
views.size()    == 3
views[0]        == "The"
views[1]        == "cat"
views[2]        == "sat"
```

## Find functions
```cpp
etl::istring::iterator find_first_of(etl::istring::iterator first, 
                                     etl::istring::iterator last, 
                                     etl::istring::const_pointer delimiters)
```
**Description**  
Returns an iterator to the first instance of a character in `delimiters`.  
Returns `last` if not found.

---

```cpp
etl::istring::const_iterator find_first_of(etl::istring::const_iterator first,
                                           etl::istring::const_iterator last, 
                                           etl::istring::const_pointer delimiters)
```
**Description**  
Returns a `const_iterator` to the first instance of a character in `delimiters`.  
Returns `last` if not found.

---

```cpp
etl::istring::iterator find_first_of(etl::istring& s, 
                                     etl::istring::const_pointer delimiters)
```
**Description**  
Returns an `iterator` to the first instance of a character in `delimiters`.  
Returns `s.end()` if not found.

---

```cpp
etl::istring::const_iterator find_first_of(const etl::istring& s, 
                                           etl::istring::const_pointer delimiters)
```
**Description**  
Returns a `const_iterator` to the first instance of a character in `delimiters`.  
Returns `s.end()` if not found.

---

```cpp
etl::istring::const_iterator find_first_of(const etl::string_view& view, 
                                           etl::istring::const_pointer delimiters)
```
**Description**  
Returns a `const_iterator` to the first instance of a character in `delimiters`.  
Returns `view.end()` if not found.

---

```cpp
etl::istring::iterator find_first_not_of(etl::istring::iterator first, 
                                         etl::istring::iterator last, 
                                         etl::istring::const_pointer delimiters)
```
**Description**  
Returns an `iterator` to the first instance of a character not in `delimiters`.  
Returns `last` if not found.

---

```cpp
etl::istring::const_iterator find_first_not_of(etl::istring::const_iterator first,   
                                               etl::istring::const_iterator last, 
                                               etl::istring::const_pointer delimiters)
```
**Description**  
Returns a `const_iterator` to the first instance of a character not in `delimiters`.  
Returns `last` if not found.

---

```cpp
etl::istring::iterator find_first_not_of(etl::istring& s, 
                                         etl::istring::const_pointer delimiters)
```
**Description**  
Returns an `iterator` to the first instance of a character not in `delimiters`.  
Returns `s.end()` if not found.

---

```cpp
etl::istring::const_iterator find_first_not_of(const etl::istring& s, 
                                               etl::istring::const_pointer delimiters)
```
**Description**  
Returns a `const_iterator` to the first instance of a character not in `delimiters`.  
Returns `s.end()` if not found.

---

```cpp
etl::istring::const_iterator find_first_not_of(const etl::string_view& view, 
                                               etl::istring::const_pointer delimiters)
```
**Description**  
Returns a `const_iterator` to the first instance of a character not in `delimiters`.  
Returns `view.end()` if not found.

---

```cpp
etl::istring::iterator find_last_of(etl::istring::iterator first, 
                                    etl::istring::iterator last, 
                                    etl::istring::const_pointer delimiters)
```
**Description**  
Returns an `iterator` to the last instance of a character in `delimiters`.  
Returns `last` if not found.

---

```cpp
etl::istring::const_iterator find_last_of(etl::istring::const_iterator first,
                                          etl::istring::const_iterator last, 
                                          etl::istring::const_pointer delimiters)
```
**Description**  
Returns a `const_iterator` to the last instance of a character in `delimiters`.  
Returns `last` if not found.

---

```cpp
etl::istring::iterator find_last_of(etl::istring& s, etl::istring::const_pointer delimiters)
```
**Description**  
Returns an `iterator` to the last instance of a character in `delimiters`.  
Returns `s.end()` if not found.

---

```cpp
etl::istring::const_iterator find_last_of(const etl::istring& s, 
                                          etl::istring::const_pointer delimiters)
```
Returns a `const_iterator` to the last instance of a character in `delimiters`.  
Returns `s.end()` if not found.

---

```cpp
etl::istring::const_iterator find_last_of(const etl::string_view& view, 
                                          etl::istring::const_pointer delimiters)
```
**Description**  
Returns a `const_iterator` to the last instance of a character in `delimiters`.  
Returns `view.end()` if not found.

---

```cpp
etl::istring::iterator find_last_not_of(etl::istring::iterator first, 
                                        etl::istring::iterator last, 
                                        etl::istring::const_pointer delimiters)
```
**Description**  
Returns an `iterator` to the last instance of a character not in `delimiters`.  
Returns `last` if not found.

---

```cpp
etl::istring::const_iterator find_last_not_of(etl::istring::const_iterator first,
                                              etl::istring::const_iterator last, 
                                              etl::istring::const_pointer delimiters)
```
**Description**  
Returns a `const_iterator` to the last instance of a character not in `delimiters`.  
Returns `last` if not found.

---

```cpp
etl::istring::iterator find_last_not_of(etl::istring& s, 
                                        etl::istring::const_pointer delimiters)
```
**Description**  
Returns an `iterator` to the last instance of a character not in `delimiters`.  
Returns `last` if not found.

---

```cpp
etl::istring::const_iterator find_last_not_of(const etl::istring& s, 
                                              etl::istring::const_pointer delimiters)
```
**Description**  
Returns a `const_iterator` to the last instance of a character not in `delimiters`.  
Returns `s.end()` if not found.

---

```cpp
etl::istring::const_iterator find_last_not_of(const etl::string_view& view, 
                                              etl::istring::const_pointer delimiters)
```
**Description**  
Returns a `const_iterator` to the last instance of a character not in `delimiters`.  
Returns `view.end()` if not found.

