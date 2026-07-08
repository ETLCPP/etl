---
title: "Exceptions"
---

The base class for all ETL exceptions.

## Typedefs
```cpp
string_type = const char*;
numeric_type = int;
```

## Constructor
```cpp
exception(string_type reason, string_type file_name, numeric_type line_number)
```

## Access
```cpp
string_type what() const
```
**Description**  
Gets the reason for the exception.

## Example
```cpp
struct vector_full : public etl::exception
{
  vector_full(string_type file_name, numeric_type line_number)
    : exception(ETL_ERROR_TEXT("Vector: full", "E123"), file_name, line_number)
  {
  }
};
```

Usually error will be called via the ETL error macro.

```cpp
ETL_ERROR(derived_exception_class)
```
