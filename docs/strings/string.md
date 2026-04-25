---
title: "basic_string"
---

{{< callout type="info">}}
  Header:  
  `string.h`  
  `wstring.h`  
  `u8string.h`  
  `u16string.h`  
  `u32string.h`  
  Similar to:  
  `std::string`  
  `std::wstring`  
  `std::u8string`  
  `std::u16string`  
  `std::u32string`
{{< /callout >}}

Fixed capacity string classes.

```cpp
etl::string<size_t Size>
etl::wstring<size_t Size>
etl::u8string<size_t Size>
etl::u16string<size_t Size>
etl::u32string<size_t Size>
```

```cpp
etl::string_ext
etl::wstring_ext
etl::u16string_ext
etl::u32string_ext
```

Inherits from `etl::istring`, `etl::iwstring`, `etl::iu8string`, `etl::iu16string` or `etl::iu32string`.  
The base classes may be used as a size independent pointer or reference type for any string instance.  

Has the ability to be copied by low level functions such as `memcpy` by use of a `repair()` function.  
See the function reference for an example of use.  

Note: `cstring.h` is deprecated  

## Macros

If `ETL_DISABLE_STRING_TRUNCATION_CHECKS` is defined then the string will not be checked for truncation and
the truncation functions will not be available.  

If `ETL_DISABLE_STRING_CLEAR_AFTER_USE` is defined then the string will not be cleared after use and the
'secure' functions will not be available.  

Defining both of these macros can reduce the overhead of an `etl::string` by up to 4 bytes.  

If `ETL_ENABLE_ERROR_ON_STRING_TRUNCATION` is defined then an error will be asserted if the string would truncate.  

## External buffers

```cpp
etl::string_ext
etl::wstring_ext
etl::u16string_ext
etl::u32string_ext
```
With these templates, a pointer to an external buffer, plus the size of the buffer must be supplied at construction.  
**Note** The buffer size must  include room for a terminating null.

**Example**  
```cpp
constexpr size_t Size = 10;

// A string of a maximum of 10 characters, using an internal buffer.  
etl::string<Size > text;

// A string of a maximum of 10 characters, using an external buffer.
char buffer[Size + 1];
etl::string_ext text(buffer, etl::size(buffer));
```

---

If the external buffer contains a C string and you wish this to be the initial value, then use the following constructor with the buffer as the initial text parameter.  

```cpp
etl::string_ext text(buffer, buffer, etl::size(buffer));
```
This constructs a string of capacity `etl::size(buffer)` containing the null terminated string in buffer.  
The constructor will detect that the initial text and the buffer are at the same address.

---

**Note**  
When using external buffered strings, a `memcpy` + `repair` will **not** produce a copy of the string.  
Both the original and copy will point to the same external buffer.

## Truncation

Strings contain an internal flag that records the truncation state.  
If a string Is constructed, assigned or modified in any way such that the text contained would have exceeded the capacity then the truncate flag is set. The string will be truncated at the maximum capacity.  

A string may become marked as truncated if it is assigned characters or constructed from another string marked as truncated. This will occur even if the resulting string is not truncated itself.  

The truncation flag is cleared if the string is assigned from a string that does not exceed the capacity or a non-truncated string. It can also be forcibly cleared by calling `clear_truncated()`.  

If  it is required that truncation be an error then define `ETL_STRING_TRUNCATION_IS_ERROR`.

---

```cpp
bool truncated() const
```
**Description**  
Returns `true` if the string was truncated (the assigned or inserted string was greater than capacity or marked as truncated), otherwise `false`.

---

```cpp
void clear_truncated()
```
**Description**  
Clears the internal truncated flag.

---

**Example**  
Assuming `ETL_STRING_TRUNCATION_IS_ERROR` is not defined.  

```cpp
etl::string<11> text1 = "Hello";     // Not truncated.
etl::string<6>  text2 = " World!!!"; // Truncated to " World".

text1 += text2; // Truncated. text1 inherits the truncation state from text2.
text1.assign("Hello World"); // Not truncated.
```

## Interfacing with C APIs

The ETL strings implement three functions to ease interfacing with C APIs.  
```cpp
initialize_free_space()
trim_to_terminator()
data_end()
```

---

```cpp
initialize_free_space()
```
**Description**  
Initializes the free space in the string to null terminators.  
Since: `20.3.0`

---

```cpp
trim_to_terminator()
```
**Description**  
Sets the string length to the first terminating null or the max size if the string.  
Since: `20.3.0`

---

```cpp
data_end()
```
**Description**  
Returns a pointer to the current terminating null.  
Since: `20.3.0`

**Example**  
```cpp
// Send the ETL string data to a C function that will append characters.
etl::string<20> text("Hello ");
text.initialize_free_space(); // If the C function does not add its own null terminators.
CFunction(text.data_end(), text.available());
text.trim_to_terminator();
```

---

## Member types
```cpp
value_type              T
size_type               size_t
difference_type         ptrdiff_t
reference               value_type&
const_reference         const value_type&
pointer                 value_type*
const_pointer           const value_type*
iterator                Random access iterator
const_iterator          Constant random access iterator
reverse_iterator        ETL_OR_STD::reverse_iterator<iterator>
const_reverse_iterator  ETL_OR_STD::reverse_iterator<const_iterator>
```

## Constructor

**Internal buffer**  
```cpp
etl::string<const size_t Size>();
etl::string<const size_t Size>(const istring& str);
etl::string<const size_t Size>(const istring& str, size_t pos, size_t len = npos);
etl::string<const size_t Size>(const char* s);
etl::string<const size_t Size>(const char* s, size_t n);
etl::string<const size_t Size>(size_t n, char c);
etl::string<const size_t Size>(const etl::string_view& view);
```

---

```cpp
template <typename TIterator>
etl::string<const size_t Size>(TIterator begin, TIterator end);
```
**Description**  
Truncates the string if too long.

**External buffer**  
```cpp
etl::string<const size_t Size>(char* buffer, size_t buffer_size);
etl::string<const size_t Size>(const istring& str, char* buffer, size_t buffer_size);
etl::string<const size_t Size>(const istring& str, char* buffer, size_t buffer_size, size_t pos, size_t len = npos);
etl::string<const size_t Size>(const char* s, char* buffer, size_t buffer_size);
etl::string<const size_t Size>(const char* s, size_t n, char* buffer, size_t buffer_size);
etl::string<const size_t Size>(size_t n, char c, char* buffer, size_t buffer_size);
etl::string<const size_t Size>(const etl::string_view& view, char* buffer, size_t buffer_size);
```
```cpp
template <typename TIterator>
etl::string<const size_t Size>(TIterator begin, TIterator end, char* buffer, size_t buffer_size);
```
Truncates the string if too long.

## Element access

```cpp
T& at(size_t i)
const T& at(size_t i) const
```
Returns a reference or const reference to the indexed element. Emits an etl::string_out_of_bounds if the index is out of range of the array.

---

```cpp
T& operator[](size_t i)
const T& operator[](size_t i) const
```
Returns a reference or const reference to the indexed element.

---

```cpp
T& front()
const T& front() const
```
Returns a reference or const reference to the first element.  
Undefined behaviour if the string is empty.

---

```cpp
T& back()
const T& back() const
```
Returns a reference or const reference to the last element.  
Undefined behaviour if the string is empty.

## Iterators

```cpp
iterator begin()
const_iterator begin() const
const_iterator cbegin() const
```
Returns an iterator to the beginning of the string.

---

```cpp
iterator end()
const_iterator end() const
const_iterator cend() const
```
Returns an iterator to the end of the string.

---

```cpp
iterator rbegin()
const_reverse_iterator rbegin() const
const_reverse_iterator crbegin() const
```
Returns a reverse iterator to the beginning of the string.

---

```cpp
iterator rend()
const_reverse_iterator rend() const
const_reverse_iterator crend() const
```
Returns a reverse iterator to the end of the string.

---

```cpp
value_type* data()
const value_type* data() const
```
Returns a pointer to the start of the string.

---

```cpp
value_type* data_end()
const value_type* data_end() const
```
Returns a pointer to the terminator of the string.

## Capacity

```cpp
bool empty() const
```
Returns true if the size of the string is zero, otherwise false.

---

```cpp
bool full() const
```
Returns true if the size of the string is SIZE, otherwise false.

---

```cpp
size_t size() const
```
Returns the size of the vector.

---

```cpp
void resize(size_t new_size, T value = T())
```
Resizes the string, up to the maximum capacity.  
Emits an etl::string_full if the string does not have the capacity.

---

```cpp
template <typename TOperation>
void resize_and_overwrite(size_type new_size, TOperation operation)
```
Resizes the string and overwrites to data using the operation.  
Since: `20.39.5`

---

```cpp
void uninitialized_resize(size_t new_size)
```
Resizes the string, up to the maximum capacity, without initialising the new elements.  
Since: `20.4.0`

---

```cpp
size_t max_size() const
```
Returns the maximum possible size of the string.

---

```cpp
size_t capacity() const
```
Returns the maximum possible size of the string.

---

```cpp
size_t available() const
```
Returns the remaining available capacity in the string.

---

## Modifiers

```cpp
void push_back(value_type value)
```
Pushes a value to the back of the string.  
No action if the string is already full.

---

```cpp
void pop_back();
```
Pop a value from the back of the string.  
No action if the string is already empty.

## Insert
```cpp
iterator insert(iterator position, T value)
```
Upto: `20.19.0`

---

```cpp
iterator insert(iterator position, size_type n, T value)
```

---

```cpp
template <typename TIterator>
iterator insert(iterator position, TIterator first, TIterator last)
```

---

```cpp
iterator insert(const_iterator position, T value)
```
Since: `20.20.0`

---

```cpp
iterator insert(const_iterator position, size_type n, T value)
```

---

```cpp
template <typename TIterator>
iterator insert(const_iterator position, TIterator first, TIterator last)
```

---

```cpp
etl::istring& insert(size_type position, const etl::istring& str)
```

---

```cpp
etl::istring& insert(size_type position, const etl::string_view& view)
Since: `20.39.5`

---

```cpp
etl::istring& insert(size_type position, 
                     const etl::istring& str,
                     size_type subposition,
                     size_type sublength)
```

---

```cpp
etl::istring& insert(size_type position, 
                     const etl::string_view& view,
                     size_type subposition,
                     size_type sublength)
```
Since: `20.39.5`

---

```cpp
etl::istring& insert(size_type position, const_pointer s)
```

---

```cpp
etl::istring& insert(size_type position, const_pointer s, size_type n)
```

---

```cpp
etl::istring& insert(size_type position, size_type n, value_type c)
```
Inserts values in to the string.  
Truncates the string if too long.  
Note: Strings that insert from themselves are only supported if the insert position is after the input range.

## Erase

```cpp
etl::istring<T>& erase(size_type position, size_type length_ = npos)
```

---

```cpp
iterator erase(iterator i_element)
iterator erase(iterator first, iterator last)
```
Upto: `20.19.0`

---

```cpp
iterator erase(iterator i_element)
iterator erase(const_iterator i_element)
iterator erase(const_iterator first, const_iterator last)
```
Erases values in the string.  
Iterators are not checked.
Since: `20.20.0`

## Assign
```cpp
template <typename TIterator>
void assign(TIterator begin, TIterator end);
```

---

```cpp
void assign(size_t n, value_type value);
```
Fills the vector with the values. Emits `etl::string_iterator` if the distance between begin and end is illegal.  
Truncates the string if too long.

```cpp
void assign(const etl::istring& str);
void assign(const etl::string_view& str); 20.40.0
void assign(const std::string_view& str);
void assign(const etl::istring& str, size_t subpos, size_t sublen);
void assign(const char* s);
void assign(const char* s, size_t n);
void assign(size_t n, char c);
```

```cpp
template <typename TIterator>
void assign(TIterator first, TIterator last);
```
Assigns to the string. Truncates the string if too long.  
May inherit the truncation state if the parameter is an `etl::string`.

---

## Append

```cpp
etl::istring& append(const etl::istring& str);
etl::istring& append(const etl::istring& str, size_t subpos, size_t sublen);
etl::istring& append(const etl::string_view& view);
etl::istring& append(const etl::string_view& view, size_t subpos, size_t sublen);
etl::istring& append(const char* s);
etl::istring& append(const char* s, size_t n);
etl::istring& append(size_t n, char c);
```

```cpp
template <typename TIterator>
string& append(TIterator first, Titerator last);
```
Appends to the string. Truncates the string if too long.  
May inherit the truncation state if the parameter is an `etl::string`.

---

## Insert

```cpp
etl::istring& insert(size_t pos, const istring& str);
etl::istring& insert(size_t pos, const istring& str, size_t subpos, size_t sublen);
etl::istring& insert(size_t pos, const string_view& view);
etl::istring& insert(size_t pos, const string_view& view, size_t subpos, size_t sublen);
etl::istring& insert(size_t pos, const char* s);
etl::istring& insert(size_t pos, const char* s, size_t n);
etl::istring& insert(size_t pos, size_t n, char c);
void insert (iterator p, size_t n, char c);
iterator insert(iterator p, char c);
```

```cpp
template <typename TIterator>
void insert (iterator p, Titerator first, Titerator last);
```
Inserts into the string.  
May inherit the truncation state if the parameter is an `etl::string`.  

Note: Strings that insert from themselves are only supported if the insert position is after the input range.

---

## Replace

```cpp
string& replace(size_t pos,  size_t len,  const etl::istring& str);
string& replace(iterator i1, iterator i2, const etl::istring& str);
string& replace(size_t pos,  size_t len,  const etl::istring& str, size_t subpos, size_t sublen);
string& replace(size_t pos,  size_t len,  const char* s);
string& replace(iterator i1, iterator i2, const char* s);
string& replace(size_t pos,  size_t len,  const char* s, size_t n);
string& replace(iterator i1, iterator i2, const char* s, size_t n);
string& replace(size_t pos,  size_t len,  size_t n, char c);
string& replace(iterator i1, iterator i2, size_t n, char c);
```
```cpp
template <typename TIterator>
string& replace(iterator i1, iterator i2, Titerator first, Titerator last);
```
Replaces parts of the string. Truncates the string if too long.  
May inherit the truncation state if the parameter is an `etl::istring`.

---

## Repair
```cpp
void repair()
```
This function must be called if the string has been copied via a low level method such as `memcpy`.
This can only be called from an `etl::string`, `etl::wstring`, `etl::u8string`, `etl::u16string`, or `etl::u32string` instance.

If `ETL_ISTRING_REPAIR_ENABLE` is defined then `repair()` may be called from the `etl::istring` base class.  
Be aware that doing this introduces a virtual function to the class.  

Has no effect if the object has not been copied in this way.

**Example**  
```cpp
typedef etl::string<10> String;
String text = "ABCDEF";
char buffer[sizeof(String)];
memcpy(&buffer, &text, sizeof(String));
String& rtext(*reinterpret_cast<String*>(buffer));
// Do not access the string in any way until you have called this.
rtext.repair();
```

---

```cpp
void clear();
```
Clears the string to a size of zero.  
Resets the truncate flag

---

```cpp
void initialize_free_space()
```
This function will initialize the free space in a string to null terminators.

---

```cpp
void trim_to_terminator()
```
The string length will be set to the position of the first null terminator.  
If the last item in the buffer is not a null terminator then one will be inserted.

---

```cpp
string& operator = (const etl::istring& rhs)
string& operator = (const value_type* text)
```

## Assignment operator.
May inherit the truncation state is the parameter is an `etl::string`.

```cpp
string& operator += (const etl::istring& rhs)
string& operator += (const value_type* text)
```

## Addition operator.
May inherit the truncation state is the parameter is an `etl::istring`.

```cpp
void clear();
```
Clears the string to a size of zero.  
Resets the truncate flag.

## Find

```cpp
size_t find(const etl::istring& str, size_t pos = 0) const;
size_t find(const char* s, size_t pos = 0) const;
size_t find(const char* s, size_t pos, size_t n) const;
size_t find(char c, size_t pos = 0) const;
```

## Find in a string

```cpp
size_t rfind(const etl::istring& str, size_t pos = npos) const;
size_t rfind(const char* s, size_t pos = npos) const;
size_t rfind(const char* s, size_t pos, size_t n) const;
size_t rfind(char c, size_t pos = npos) const;
```
Reverse find in a string

---

```cpp
size_t find_first_of(const string& str, size_t pos = 0) const;
size_t find_first_of(const char* s, size_t pos = 0) const;
size_t find_first_of(const char* s, size_t pos, size_t n) const;
size_t find_first_of(char c, size_t pos = 0) const;
```
Find first of in a string

---

```cpp
size_t find_last_of(const etl::istring& str, size_t pos = npos) const;
size_t find_last_of(const char* s, size_t pos = npos) const;
size_t find_last_of(const char* s, size_t pos, size_t n) const;
size_t find_last_of(char c, size_t pos = npos) const;
```
Find last of in a string.

---

```cpp
size_t find_first_not_of(const etl::istring& str, size_t pos = 0) const;
size_t find_first_not_of(const char* s, size_t pos = 0) const;
size_t find_first_not_of(const char* s, size_t pos, size_t n) const;
size_t find_first_not_of(char c, size_t pos = 0) const;
```
Find first not of in a string

---

```cpp
size_t find_last_not_of(const etl::istring& str, size_t pos = npos) const;
size_t find_last_not_of(const char* s, size_t pos = npos) const;
size_t find_last_not_of(const char* s, size_t pos, size_t n) const;
size_t find_last_not_of(char c, size_t pos = npos) const;
```
Find last not of in a string

---

## contains

```cpp
bool contains(const etl::ibasic_string<T>& str) const 
```
Checks that the string is within this string.

---

```cpp
template <typename TTraits>
bool contains(const etl::basic_string_view<T, TTraits>& view) const 
```
Checks that the view is within this string

---

```cpp
bool contains(const_pointer s) const 
```
Checks that text is within this string

---

```cpp
bool contains(value_type c) const 
```
Checks that character is within this string

## starts_with

```cpp
bool starts_with(const etl::ibasic_string<T>& str) const 
```
Checks that the string is the start of this string

---

```cpp
template <typename TTraits>
bool starts_with(const etl::basic_string_view<T, TTraits>& view) const 
```
Checks that the view is the start of this string

---

```cpp
bool starts_with(const_pointer s) const 
```
Checks that the string is the start of this string

--

```cpp
bool starts_with(value_type c) const 
```
Checks that the character is the start of this string

## ends_with

```cpp
bool ends_with(const etl::ibasic_string<T>& str) const 
```
Checks that the string is the end of this string

---

```cpp
template <typename TTraits>
bool ends_with(const etl::basic_string_view<T, TTraits>& view) const 
```
Checks that the view is the end of this string

---

```cpp
bool ends_with(const_pointer s) const 
```
Checks that the string is the end of this string

---

```cpp
bool ends_with(value_type c) const 
```
Checks that the character is the end of this string

---

## Substr
```cpp
string substr(size_t pos = 0, size_t len = npos) const
```
Extract a substring

---

## Compare

```cpp
int compare(const etl::istring& str) const;
int compare(size_t pos, size_t len, const etl::istring& str) const;
int compare(size_t pos, size_t len, const etl::istring& str, size_t subpos, size_t sublen) const;
int compare(const char* s) const;
int compare(size_t pos, size_t len, const char* s) const;
int compare(size_t pos, size_t len, const char* s, size_t n) const;
```

## Compare strings

---

## Non-member functions

Make a string from a literal or array  
`char`
```cpp
template <const size_t Max_Size>
etl::string<Max_Size> make_string(const char (&text) [Max_Size])
```

---

```cpp
template <const size_t Max_Size, size_t Size>
etl::string<Max_Size> make_string_with_capacity(const char (&text) [Size])
```

---

`wchar_t`
```cpp
template <const size_t Max_Size>
etl::wstring<Max_Size> make_string(const wchar_t (&text) [Max_Size])
```

```cpp
template <const size_t Max_Size, size_t Size>
etl::wstring<Max_Size> make_string_with_capacity(const wchar(&text) [Size])
```

---

`char8_t`  
Since: `20.38.7`  
```cpp
template <const size_t Max_Size>
etl::u8string<Max_Size> make_string(const char8_t (&text) [Max_Size])
```

---

```cpp
template <const size_t Max_Size, size_t Size>
etl::u16string<Max_Size> make_string_with_capacity(const char16_t(&text) [Size])
```

`char16_t`  
```cpp
template <const size_t Max_Size>
etl::u16string<Max_Size> make_string(const char16_t (&text) [Max_Size])
```

---

```cpp
template <const size_t Max_Size, size_t Size>
etl::u16string<Max_Size> make_string_with_capacity(const char16_t(&text) [Size])

---

`char32_t`  
```cpp
template <const size_t Max_Size>
etl::u32string<Max_Size> make_string(const char32_t (&text) [Max_Size])
```

---

```cpp
template <const size_t Max_Size, size_t Size>
etl::u32string<Max_Size> make_string_with_capacity(const char32_t(&text) [Size])
```

## Examples
```cpp
// 'text' contains "Hello World" with a size and capacity of 11.
auto text = etl::make_string("Hello World");

// 'text' contains "Hello World" with a size of 11 and capacity of 20.
auto text = etl::make_string_with_capacity<20>("Hello World");

// 'text' contains "Hello Worl" with a size of 10 and capacity of 10. Marked as truncated.
auto text = etl::make_string_with_capacity<10>("Hello World");
```

## Comparisons
`==`  `true` if the contents of the vectors are equal,  
otherwise `false`.  
`!=`  `true` if the contents of the vectors are not equal,  
otherwise `false`.  
`< `  `true` if the contents of the lhs are lexicographically less than the contents of the rhs,  
otherwise `false`.  
`<=`  `true` if the contents of the lhs are lexicographically less than or equal to the contents of the rhs,  
otherwise `false`.  
`> `  `true` if the contents of the lhs are lexicographically greater than the contents of the rhs,  
otherwise `false`.  
`>=`  `true` if the contents of the lhs are lexicographically greater than or equal to the contents of the rhs,  
otherwise `false`.  
