---
title: "Interfacing with C"
weight: 3
---

Interfacing ETL containers with contiguous storage to a C API is fairly easy to do.

This is a C function that copies `count` characters , starting from `a`, to a buffer pointed to by `start`.

```cpp
size_t CFunctionCopyToBuffer(char* start, size_t count)
{
  size_t i = 0;
  while (i < count)
  {
    *p++ = 'a' + i;
    ++i;
  }
  return i;
}
```

---

**Array**  
The ETL array is simply a C array wrapped in a C++ interface. 

```cpp
size_t length;

etl::array<char, 26> buffer;

length = CFunctionCopyToBuffer(buffer.data(), buffer.max_size() / 2);
```

Array contains `[ a, b, c, d, e, f, g, h, i, j, k, l, m ]`

---

**Vector**  
To use a `vector` with C, you must make room for the expected new data, and then adjust to the new size after copying.

```cpp
etl::vector<char, 26> buffer;

// Make the vector as big as it can be.
buffer.uninitialized_resize(buffer.max_size());

size_t newLength = CFunctionCopyToBuffer(buffer.data(), buffer.max_size() / 2);

 // Now adjust it back to the actual new size.
buffer.resize(newLength);
```

Vector contains `[ a, b, c, d, e, f, g, h, i, j, k, l, m ]`

---

**String**  
Strings are terminated with null characters. We can use this to easily adjust the length after copying.

```cpp
etl::string<26> buffer;

// Ensure that the string's free space are terminator characters.
buffer.initialize_free_space();

CFunctionCopyToBuffer(buffer.data(), buffer.max_size() / 2);

// Trim the string length at the first null terminator.
buffer.trim_to_terminator();
```

String contains `"abcdefghijklm"`

```cpp
// Lets add some more characters.
// Start at data_end() so we append to what's already there.
// We'll use the alternative method to adjust the string length.
// We could still use trim_to_terminator() if we wanted to.

// Remember the current size.
size_t oldSize = buffer.size();

// Make the string as big as it can be.
buffer.uninitialized_resize(buffer.max_size()); 

// Append the characters.
size_t appendedSize = CFunctionCopyToBuffer(buffer.data_end(), buffer.max_size() / 2);

// Now adjust it back to the actual new size.
buffer.uninitialized_resize(oldSize + appendedSize); 
```

String contains `"abcdefghijklmabcdefghijklm"`
