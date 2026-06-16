---
title: "error_handler"
---

{{< callout type="info">}}
  Header: `error_handler.h`  
{{< /callout >}}

Finding errors within an embedded system can be difficult due to the performance and space restrictions imposed upon the platform. The library allows a variety of methods to catch errors, allowing the performance and space overheads to be chosen according to the situation and requirements.  

The library allows the method to be chosen at compile time.  

You have a choice of:
- Exceptions
- Asserts
- Error log
- No error checking

The type of error handler used is dependant on the compile time macro defined.  
Note: This are usually set as a project wide definition.  

| Macro                   | Actions                                                                                              |
| ----------------------- | ---------------------------------------------------------------------------------------------------- |
| ETL_NO_CHECKS           | No checks are mode at all, not even in debug mode.                                                   |
| ETL_THROW_EXCEPTIONS    | Exceptions are thrown for an error.                                                                  |
| ETL_USE_ASSERT_FUNCTION | Errors are sent to a user defined assert handler.                                                    |
| ETL_LOG_ERRORS          | Errors are sent to a user defined error handler. This can be used in conjunction with other options. |

If none of the above macros are defined then the library will use assert. These are only active is `NDEBUG` is not defined.  

Errors are checked for by calling one of the following:- 

```cpp
ETL_ASSERT(condition, ETL_ERROR(error_exception_class))  
```
Raises the error if the condition is `false`.  

---

```cpp
ETL_ASSERT_AND_RETURN(condition, ETL_ERROR(error_exception_class))
```
Raises the error if the condition is `false` and calls `return`.  

---

```cpp
ETL_ASSERT_AND_RETURN_VALUE(condition, ETL_ERROR(error_exception_class), value)
```
Raises the error if the condition is `false` and calls `return value`.

---

```cpp
ETL_ALWAYS_ASSERT(ETL_ERROR(error_exception_class))
```
Raises the error.

---

```cpp
ETL_ALWAYS_ASSERT_AND_RETURN(ETL_ERROR(error_exception_class))
```
Raises the error and calls `return`.

---

```cpp
ETL_ALWAYS_ASSERT_AND_RETURN_VALUE(ETL_ERROR(error_exception_class), value)
```
Raises the error and calls `return value`.

---

Note: Not all error methods will call the return, such as when using C++ exceptions.  
The macro will call return for the following combinations:-
- `ETL_LOG_ERRORS` only.
- `ETL_DEBUG` not defined.

---

If `ETL_VERBOSE_ERRORS` is defined then the filename is included as part of the error, otherwise it will be omitted, so reducing storage requirements.  

Error messages by be declared using the `ETL_ERROR_TEXT` macro.  
```cpp
ETL_ERROR_TEXT("Verbose text", "terse text")
```
If `ETL_VERBOSE_ERRORS` is defined then `ETL_TEXT` uses the verbose text. By default the terse text is used.  

The terse text used in the library follows a `<numeric><alpha>` pattern. For example, errors in `etl::vector` start with `"17"` and the alpha code for 'vector full' is `"A"`. The return from the `what()` member function in this case will be `"17A"`.  

When `ETL_LOG_ERRORS` is defined, error exceptions are passed to `etl::error_handler::error()` before throwing the exception or calling the assert. This will do nothing until a user defined handler function is set. The user function may either be a free function or a member function.  

There is an additional switch that enables checks to be made on pushes and pops to containers, `ETL_CHECK_PUSH_POP`.  
This is not enabled by default as empty/full checks will usually be made by the calling code.  

---

There are versions of the assert macros that are only enabled when `ETL_IS_DEBUG_BUILD` is `true`:-   

```cpp
ETL_DEBUG_ASSERT(condition, ETL_ERROR(error_exception_class))  
```
Raises the error if the condition is `false`.  

---

```cpp
ETL_DEBUG_ASSERT_AND_RETURN(condition, ETL_ERROR(error_exception_class))
```
Raises the error if the condition is `false` and calls `return`.

---

```cpp
ETL_DEBUG_ASSERT_AND_RETURN_VALUE(condition, ETL_ERROR(error_exception_class), value)
```
Raises the error if the condition is `false` and calls `return value`.

---

```cpp
ETL_DEBUG_ALWAYS_ASSERT(ETL_ERROR(error_exception_class))
```
Raises the error.

---

```cpp
ETL_DEBUG_ALWAYS_ASSERT_AND_RETURN(ETL_ERROR(error_exception_class))
```
Raises the error and calls `return`.

---

```cpp
ETL_DEBUG_ALWAYS_ASSERT_AND_RETURN_VALUE(ETL_ERROR(error_exception_class), value)
```
Raises the error and calls return(value).

## Example macro combinations

*No error macros defined*  
&emsp;Asserts are generated when a check fails.

---

`ETL_LOG_ERRORS`  
&emsp;The error handler is called.

---

`ETL_NO_CHECKS`  
&emsp;No checks are made. No asserts or exceptions are generated.  
&emsp;No calls to the error handler are made, even if `ETL_LOG_ERRORS` is defined.  

---

`ETL_THROW_EXCEPTIONS`  
&emsp;An exception is thrown when a check fails.  

---

`ETL_USE_ASSERT_FUNCTION`  
&emsp;Calls a user defined assert function. Set with `etl::set_assert_function()`  
&emsp;The assert function must have the signature `void(const etl::exception&)`  
&emsp;If an assert handler is not specified then `assert(false)` is called.  

---

`ETL_LOG_ERRORS`  
`ETL_THROW_EXCEPTIONS`  
&emsp;When a check fails the error handler is called, then an exception is thrown.  

---

`ETL_LOG_ERRORS`  
`ETL_CHECK_PUSH_POP`  
&emsp;Asserts are generated when a check fails and the error handler is called and additional checks for pushes and pops are made.  

## Example error handlers

```cpp
void free_function(const etl::exception& e)
{
  std::cout << "The error was " << e.what() << " in " << e.file_name() << " at "
            << e.line_number() << "\n";
}

struct error_log
{
  void member_function(const etl::exception& e)
  {
    std::cout << "The error was " << e.what() << " in " << e.file_name() << " at "
              << e.line_number() << "\n";
  }
};
```

---

**Setting a free function as the recipient**

```cpp
int main()
{
  etl::error_handler::set_callback<free_function>();
}
```

---

**Setting a member function as the recipient**  

```cpp
error_log log;

// Run-time
int main()
{
  etl::error_handler::set_callback<error_log, &error_log::member_function>(log);
}

// Compile-time. 'log' must have static linkage.
int main()
{
  etl::error_handler::set_callback<error_log, log, &error_log::member_function>();
}
```

---

**Setting an `etl::ifunction` as the recipient**  
*This is not recommended for new applications.*  
*Use one of the methods above instead.*  

```cpp
// Free function
etl::function<void, const etl::exception&> error_callback(free_function);

// Member function
etl::function<TObject, const etl::exception&> error_callback(log, &error_log::member_function);

// Free function using the nested struct (Deprecated)
etl::error_handler::free_function error_callback(free_function);

// Member function using the nested struct (Deprecated)
etl::error_handler::member_function error_callback(log, error_log::member_function);

Use one of the above
int main()
{
  // Tell the error handler about it.
  etl::error_handler::set_callback(error_callback);
}
```

---

**Deprecated**
The nested structures `free_function` and `member_function` may still be used, but are deprecated.

